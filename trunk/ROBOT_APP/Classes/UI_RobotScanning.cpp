#include <ImageList.h>

#include <HelloWorldScene.h>
#include <libplayerc++/playerc++.h>
#include <pthread.h>
#include <semaphore.h>
#include <wifiscan.h>
#include <MAP>
#include "player.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
#include <boost/format.hpp>
#endif
USING_NS_CC;
using namespace std;
using namespace PlayerCc;


extern HelloWorld* m_MainView; 
/// Player Client
extern PlayerClient *pPlayerclient;
extern UI_RobotScanning* m_pUIRobotSacnning;
extern UI_RobotMain* m_pUIRobotMain;
extern UI_Reservation* m_pUIReservation;
extern DispatcherProxy *pDisProxy;
extern PowerProxy	 *pPowerProxy;
extern Position2dProxy *pPosition2dProxy;
//extern MapProxy *pMapProxy;
//extern PlannerProxy *pPlannerProxy;
extern player_map_info_t stMapInfo;
//WIFI_SCAN m_WifiScan;


//extern sem_t semRobMapRelease;
UI_RobotScanning::UI_RobotScanning()
{	
	WifiList_DrawLineNum=0;
	WifiList_StartIdx=0;
	PageType = SCANPG_LAYER;
	m_WifiListSSIDName= CCArray::create();
	m_WifiListSSIDName->retain();
	m_WifiListSSIDLabel= CCArray::create();
	m_WifiListSSIDLabel->retain();
	m_WifiListAPName= CCArray::create();
	m_WifiListAPName->retain();
	m_WifiListAPLabel= CCArray::create();
	m_WifiListAPLabel->retain();
	WifiList_SelAPIdx=0;
	iIconSpeed_Search=1;
	iIconSpeed_Connect=1;
	MsgBoxShow_RobotTimeFailGet=false;
	MsgBoxShow_RobotStateFailGet=false;
	MsgBoxShow_RobotPowerFailGet=false;	
	RobotPowerValue=0.0f;
	m_SearchType=Search_None;
	InitRobotTime();	
}

UI_RobotScanning::~UI_RobotScanning()
{
	GFUN_FreeWifiScanMemory();
	for(int i=0;i<SCANPG_SIZE;i++)
		m_BtnArray[i]->release();
	m_WifiListSSIDName->release();
	m_WifiListSSIDLabel->release();
	m_WifiListAPName->release();
	m_WifiListAPLabel->release();

	if(pPlayerclient!=NULL)
	{
		pPlayerclient->StopThread();
		delete pPlayerclient;
		pPlayerclient=NULL;
	}

}

int UI_RobotScanning::GetSelApIdx()
{
	return WifiList_SelAPIdx;
}

void UI_RobotScanning::ConnectFailSetting(int iType)
{
	m_SearchType=iType;
}

//�s��ROBOT 
//�åB�^�ǳs�����A
int UI_RobotScanning::ConnectRobot(float dt)
{
	m_SearchType=Search_None;
	string s = "localhost";
	s= "192.168.30.1";
	int iConnectState=RobSt_UnknowFail;
	GuiData_Init();
	try
	{
		m_SearchType=CONNECT_WaitRobotAck;
		iConnectState=RobSt_NewFail;
		if(pPlayerclient!=NULL)
		{
		pPlayerclient->StopThread();
		delete pPlayerclient;
		}
		pPlayerclient=new PlayerClient(s, 6665, PLAYERC_TRANSPORT_TCP);	
		pPlayerclient->SetRequestTimeout(MAX_REQUEST_TIME);
		iConnectState=RobSt_ConnectFail;
		GuiData_Connect(); //�A���إ߳s�u
		if(pPlayerclient!=NULL)
		{
			

			pDisProxy=new DispatcherProxy(pPlayerclient,0); 

			//pDisProxy->ConnectReadSignal(boost::bind(&dispatcher_callback, &pDisProxy));
			int result =  pDisProxy->LogIn("00:0c:29:f3:e8:a2");
			
			//default UI status
			pDisProxy->SetUiStatus(PLAYER_DISPATCH_INIT_DEST_UI,PLAYER_DISPATCH_UI_STANDBY); 

			pPowerProxy=new PowerProxy(pPlayerclient, 0);
			//pMapProxy=new MapProxy(pPlayerclient, 0); //(OLD)
			//pPlannerProxy =  new PlannerProxy(pPlayerclient, 0); //(OLD)
			pPosition2dProxy=new Position2dProxy(pPlayerclient, 2); //be used to get the robot's position.

			pPlayerclient->StartThread(); 

			RobotMapUpdate();//	sem_post(&semRobMapRelease); //��s�a��
			m_SearchType=CONNECT_Success;
			iConnectState=RobSt_OK;
		}
		else
		{
			m_SearchType=CONNECT_Fail_NoProxy;
			iConnectState=RobSt_NoProxyFail;
			std::string sValue=boost::str(boost::format( "%s" ) % STR_ERR_ConnFail_NoProxy_ENG );
			CCMessageBox(sValue.c_str(),"ERROR");
			//MessageBoxA(NULL,(LPCSTR)sValue.c_str(),"ERROR",MB_OK+MB_SYSTEMMODAL); //���ݹL�{�@m_SearchType�@�����|�Q��g
			m_SearchType=CONNECT_Fail_NoProxy;

		}
		
		
	}
	catch (PlayerCc::PlayerError &e) //�s�����`�|���ܦ��B
	{
		
		m_SearchType=CONNECT_Fail_NoRobotAck;

		//���B�ݧאּ�̤W�h �B�����[�j
		std::string sValue=boost::str(boost::format( "code %d:%s(LinkState:%d)" ) % e.GetErrorCode() % STR_ERR_ConnFail_NoRobotAck_ENG % iConnectState );
		CCMessageBox(sValue.c_str(),e.GetErrorStr().c_str());
		m_SearchType=CONNECT_Fail_NoRobotAck;
	}
	return iConnectState;
}

//Get robot name.
_RobotCommandState_ UI_RobotScanning::GetRobotName(char** pRobotName)
{
	_RobotCommandState_ iConnectState=RobCmdSt_Unknow;
	try
	{
		if(pDisProxy)
		{
			*pRobotName = (char *) pDisProxy->ReqUiName(PLAYER_DISPATCH_INIT_DEST_RD);
			iConnectState = RobCmdSt_OK;
		}
		else
		{
			iConnectState = RobCmdSt_Nopoint;		
		}
		
	}
	catch (PlayerCc::PlayerError &e)
	{

		std::string sValue=boost::str(boost::format( "code %d:%s" ) % e.GetErrorCode() % STR_ERR_ConnFail_RobotNameRegFail_CHT );
		CCMessageBox(sValue.c_str(),e.GetErrorStr().c_str());
		iConnectState=RobCmdSt_Timeout;
	}
	return iConnectState;
}

//SetUiTime �]�wROBOT �ɶ�
_RobotCommandState_ UI_RobotScanning::GetRobotTime()
{
	_RobotCommandState_ iConnectState=RobCmdSt_Unknow;
	try
	{
		if(pDisProxy)
		{
			//���o�����ɶ�
			//pDisProxy->GetReqUiTime((uint8_t *)&RobotTime);
			//pDisProxy->GetUiStatus(PLAYER_DISPATCH_INIT_DEST_RD, &uiStatus); //���o���x���A
			GDisProxy_ReqUiTime();
			
			MsgBoxShow_RobotTimeFailGet=false;
			iConnectState = RobCmdSt_OK;
		}
		else
			iConnectState = RobCmdSt_Nopoint;		
	}
	catch (PlayerCc::PlayerError &e)
	{
		//m_SearchType=Search_ConnectRobotFail;
		if(!MsgBoxShow_RobotTimeFailGet)
		{ //����ܤ@��
			MsgBoxShow_RobotTimeFailGet=true;
			std::string sValue=boost::str(boost::format( "code %d:%s" ) % e.GetErrorCode() % STR_ERR_ConnFail_NoRobotTime_ENG );
			CCMessageBox(sValue.c_str(),e.GetErrorStr().c_str());
		}
		iConnectState=RobCmdSt_Timeout;

	}
	return iConnectState;
}

//SetUiTime �]�wROBOT �ɶ�
_RobotCommandState_ UI_RobotScanning::GetRobotState(uint32_t &aiStatus,uint32_t &cleanStatus,uint32_t &error,uint32_t &worry)
{
	_RobotCommandState_ iConnectState=RobCmdSt_Unknow;
	try
	{
		if(pDisProxy)
		{
			//���o�����ɶ�
			//pDisProxy->GetReqUiTime((uint8_t *)&RobotTime);
			//pDisProxy->GetUiStatus(PLAYER_DISPATCH_INIT_DEST_RD, &uiStatus); //���o���x���A
			pDisProxy->GetAiStatus(PLAYER_DISPATCH_INIT_DEST_RD,
				&aiStatus,
				&cleanStatus,
				&error,
				&worry);
			MsgBoxShow_RobotStateFailGet=false;
			iConnectState = RobCmdSt_OK;
		}
		else
			iConnectState = RobCmdSt_Nopoint;		
	}
	catch (PlayerCc::PlayerError &e)
	{
		//m_SearchType=Search_ConnectRobotFail;
		if(!MsgBoxShow_RobotStateFailGet)
		{ //����ܤ@��
			MsgBoxShow_RobotStateFailGet=true;
			std::string sValue=boost::str(boost::format( "code %d:%s" ) % e.GetErrorCode() % STR_ERR_ConnFail_NoRobotTime_ENG );
			CCMessageBox(sValue.c_str(),e.GetErrorStr().c_str());			
		}

		iConnectState=RobCmdSt_Timeout;

	}
	return iConnectState;
}

//SetUiTime �]�wROBOT �ɶ�
float UI_RobotScanning::GetRobotPowerValue()
{
	return RobotPowerValue;
}

_RobotCommandState_ UI_RobotScanning::GetRobotPower( )
{
	_RobotCommandState_ iConnectState=RobCmdSt_Unknow;
	try
	{
		if(pPowerProxy)
		{
			MsgBoxShow_RobotPowerFailGet=false;
			RobotPowerValue=pPowerProxy->GetPercent(); 			
			MsgBoxShow_RobotPowerFailGet=true;
			iConnectState = RobCmdSt_OK;
		}
		else
			iConnectState = RobCmdSt_Nopoint;		
	}
	catch (PlayerCc::PlayerError &e)
	{
		if(!MsgBoxShow_RobotPowerFailGet)
		{ //����ܤ@��
			MsgBoxShow_RobotPowerFailGet=true;
			std::string sValue=boost::str(boost::format( "code %d:%s" ) % e.GetErrorCode() % STR_ERR_ConnFail_NoRobotPOWER_ENG );
			CCMessageBox(sValue.c_str(),e.GetErrorStr().c_str());
		}
		iConnectState=RobCmdSt_Timeout;

	}
	return iConnectState;
}

//SetUiTime �]�wROBOT �ɶ�
_RobotCommandState_ UI_RobotScanning::GetRobotSchedule()
{
	_RobotCommandState_ iConnectState=RobCmdSt_Unknow;
	try
	{
		if(pDisProxy)
		{			
			uint32_t dataCount;
			player_dispatcher_ui_alarm_time_t *pAlarm;
			pAlarm=pDisProxy->ReqUiSchedule(PLAYER_DISPATCH_INIT_DEST_RD, &dataCount);
			if(dataCount>=G_ROBOT_SCHEDULE_MAX) //�ƶq�L�h �j���100��
			{
				CCLog("Schedule %d Over Size %d",dataCount,G_ROBOT_SCHEDULE_MAX);
			}

			iConnectState = RobCmdSt_OK;
			for(int i=0;i<dataCount;i++)
			{
				m_pUIReservation->SaveRobotSachedule((void*)&pAlarm[i]);				
			}
			m_pUIReservation->ResetSettingByRobotSchedule(); //�ھ�ROBOT �]�w �ק糧���]�w
		}
		else
			iConnectState = RobCmdSt_Nopoint;		
	}
	catch (PlayerCc::PlayerError &e)
	{
		iConnectState=RobCmdSt_Timeout;
	}
	return iConnectState;
}


bool UI_RobotScanning::init()
{  
	if (!CCLayer::init()) 
	{  
		return false;  
	}  
	InitUI();  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	RobotConnectThread_Cread();
	
#endif
	return true;  
}  

void UI_RobotScanning::InitUI()
{
	winSize = CCDirector::sharedDirector()->getVisibleSize(); //���o�����j�p
	InitUI_ScaningPage();
	InitUI_ErrorNoDevice(); //ERROR MSG - No DEVICE
	InitUI_WifiList();
	InitUI_ConnectAni();
	InitUI_ErrConnFail();
	//schedule( schedule_selector(UI_RobotScanning::LinkRobotLoop));
	scheduleUpdate();     //�Ұ�update()

}

void UI_RobotScanning::SearchIconStop()
{
	iIconSpeed_Search =0;
}

void UI_RobotScanning::SearchIconSlow()
{
	iIconSpeed_Search =1;
}

void UI_RobotScanning::SearchIconFast()
{
	iIconSpeed_Search =5;
}

void UI_RobotScanning::PAGEANI_SEARCH()
{
	switch(m_SearchType)
	{
	case  Search_Loop:  //�j�M�~�ϥܱ��� 
		{
				CCSprite* pSpr=NULL;
				pSpr= (CCSprite*)m_BtnArray[SCANPG_SEARCH]->objectAtIndex(UISCAN_ICON);
				pSpr->setRotation(pSpr->getRotationX()+iIconSpeed_Search);
				pSpr= (CCSprite*)m_BtnArray[SCANPG_SEARCH]->objectAtIndex(UISCAN_DOWNBTN); //�����j�M�s
				pSpr->setColor(COLOR_STRING_GREY);
		}
		break;
	default:
	case Search_APReady:
	case Search_None:
		break;
	case  Search_APFail:
		{
			//MessageBox(nullptr,L"Robot no Ready",L"ERROR",MB_OK);
			CCSprite* pSpr=NULL;
			pSpr= (CCSprite*)m_BtnArray[SCANPG_SEARCH]->objectAtIndex(UISCAN_DOWNBTN);
			pSpr->setColor(COLOR_STRING_GREEN);

			Hide_AllPage();
			Show_ErrNoDevice();
			m_SearchType =Search_None;
		}
		break;
	case  Search_APSuccess:
		{
			CCSprite* pSpr=NULL;
			pSpr= (CCSprite*)m_BtnArray[SCANPG_SEARCH]->objectAtIndex(UISCAN_DOWNBTN);
			pSpr->setColor(COLOR_STRING_GREEN);
			Hide_AllPage();
			Show_WifiList();
			Reload_WifiList();
			m_SearchType=Search_APReady;
		}
		break;	
	}

}

void UI_RobotScanning::Reload_WifiList()
{
	int iCount=SSIDName.size();
	m_WifiListSSIDName->removeAllObjects();
	m_WifiListAPName->removeAllObjects();
	vector<string>::iterator iterSSID = SSIDName.begin();
	vector<wstring>::iterator iterAP = APName.begin();
	vector<int>::iterator iterState = iLinkReady.begin();
	for(int i=0;iterSSID!=SSIDName.end();i++,iterSSID++,iterAP++,iterState++)
	{
		if(!iterSSID->empty() || !iterAP->empty())
		{
			m_WifiListSSIDName->addObject(CCString::createWithFormat("%s",iterSSID->c_str()));				
			m_WifiListAPName->addObject(CCString::createWithFormat("%s",iterAP->c_str()));				
		}
	}				
	WifiListLabelLoad();
	WifiListLabelDisplay(0);

}

//�ϥܿ���t�׳]�w
void UI_RobotScanning::ChangConnectIconSpeed()
{
	iIconSpeed_Connect=m_SearchType*3+1;
}

void UI_RobotScanning::PAGEANI_CONNECT()
{
	switch(m_SearchType)
	{
	case CONNECT_Fail_NoAP:
	case CONNECT_Fail_NoIP:
	case CONNECT_Fail_NoRobotAck:
	case CONNECT_Fail_NoProxy:
		{
			Hide_AllPage();
			Show_ErrConnFail();
			m_SearchType =Search_None;
		}
		break;
	case CONNECT_Ready:
	case CONNECT_WaitIP:
	case CONNECT_WaitRobotAck:
		{ //�ϥܱ���
			CCSprite* pSpr=NULL;
			pSpr= (CCSprite*)m_BtnArray[SCANPG_CONNECTANI]->objectAtIndex(CONNECTANI_ICON);
			pSpr->setRotation(pSpr->getRotationX()+iIconSpeed_Connect);
		}
		break;
	case CONNECT_Success: 
		{//������D����
			ChangLayer_Main();
			m_SearchType=Search_None;
		}
		break;

	}

}

void UI_RobotScanning::update(float dt)
{

//�ھڭ������P �M�w�ʵe���
	switch(PageType)
	{
	case SCANPG_SEARCH:
		{
			PAGEANI_SEARCH();
		}
		break;
	case SCANPG_CONNECTANI:
		{
			PAGEANI_CONNECT();
		}
		break;
	}
	
}

void UI_RobotScanning::InitUI_ScaningPage()
{
	m_BtnArray[SCANPG_SEARCH] = CCArray::createWithCapacity(UISCAN_SIZE); //���s�s��
	//for(int i=0;i<UISCAN_SIZE;i++)
	//	m_BtnArray[SCANPG_SEARCH]->addObject(CCNode::create());
	//m_BtnArray[SCANPG_SEARCH]=CCArray::create(); //�M�wARRAY �j�p
	m_BtnArray[SCANPG_SEARCH]->retain();
	int iCount=m_BtnArray[SCANPG_SEARCH]->count();
	//�]�wScanning �I��
	float fx=0,fy=0,fx2=0,fy2=0;
	CCSprite *pSpriteScanning=CCSprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
	pSpriteScanning->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	addChild(pSpriteScanning); //�[�J layer �h
	m_BtnArray[SCANPG_SEARCH]->insertObject(pSpriteScanning,UISCAN_BK);
	//m_BtnArray[SCANPG_SEARCH]->replaceObjectAtIndex(UISCAN_BK,pSpriteScanning);
	//�W����s ���אּUI_UNIT ��ƴ��@(�ϥ�,�W��)
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteScanning->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //���s�ϥ�
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSprBtnIcon->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_FunRobot, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pLabel->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SCANPG_SEARCH]->insertObject(pSpriteAction,UISCAN_TOPBTN);
	//m_BtnArray[SCANPG_SEARCH]->replaceObjectAtIndex(UISCAN_TOPBTN,pSpriteScanning);
	//�����ϥ� 
	CCSprite *pSpr=CCSprite::createWithSpriteFrameName(S_STATE_SCANING_ICON); //���A�ϥ�
	pSpr->setPosition(ccp(winSize.width/2,winSize.height*4/5));
	pSpriteScanning->addChild(pSpr,10);
	m_BtnArray[SCANPG_SEARCH]->insertObject(pSpr,UISCAN_ICON);
	//m_BtnArray[SCANPG_SEARCH]->replaceObjectAtIndex(UISCAN_TOPBTN,pSpriteScanning);

	//�ϥܤU���r
	pLabel = CCLabelTTF::create(STR_STATE_Scaning, S_FONE, *pSTR_STATE_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*3/5));
	pSpriteScanning->addChild(pLabel,10);

	//������r

	pLabel = CCLabelTTF::create(STR_HELP_PlzWait, S_FONE, *pSTR_HELP_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*2/4));
	pSpriteScanning->addChild(pLabel,10);

	pLabel = CCLabelTTF::create(STR_HELP_AutoSearch, S_FONE, *pSTR_HELP_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	int iHeight=pLabel->getContentSize().height;
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*2/4-iHeight*1.5 ));
	pSpriteScanning->addChild(pLabel,10);
	//�������s
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	pSpriteAction->setPosition(ccp(0,0));
	pSpriteAction->setColor(COLOR_STRING_GREY);
	pSpriteScanning->addChild(pSpriteAction);
	pLabel = CCLabelTTF::create(STR_BTN_Refresh, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(pSpriteAction->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel,10);
	m_BtnArray[SCANPG_SEARCH]->insertObject(pSpriteAction,UISCAN_DOWNBTN);

	pSpriteScanning->setVisible(false);
}

void UI_RobotScanning::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true);
	Show_SacnPage();
}

void UI_RobotScanning::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();
}

void UI_RobotScanning::Hide_AllPage()
{
		Hide_SacnPage();
		Hide_ErrNoDevice();
		Hide_ConnectAni();
		Hide_WifiList();
		Hide_ErrConnFail();
		PageType = SCANPG_LAYER;
		WifiList_DrawLineNum=0;
}

void UI_RobotScanning::Show_SacnPage()
{
	 
	 CCSprite* pSprite= (CCSprite*)m_BtnArray[SCANPG_SEARCH]->objectAtIndex(UISCAN_BK);
	 pSprite->setVisible(true);
	 PageType = SCANPG_SEARCH;
	 CallThread_SearchRobot();
}

void UI_RobotScanning::Hide_SacnPage()
{

	CCSprite* pSprite= (CCSprite*)m_BtnArray[SCANPG_SEARCH]->objectAtIndex(UISCAN_BK);
	pSprite->setVisible(false);
//	setVisible(false);
//	setTouchEnabled(false);
}

void UI_RobotScanning::removeWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
}

void UI_RobotScanning::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}

//////////////////////////////////////////////////////////////////////////
//Robot Sacnning View
void UI_RobotScanning::InitUI_ErrorNoDevice()
{
	
	//m_BtnArray[SCANPG_ERR_NODEVICE] = CCArray::createWithCapacity(UINODEVICE_SIZE); //���s�s��
	m_BtnArray[SCANPG_ERR_NODEVICE] = CCArray::create();
	m_BtnArray[SCANPG_ERR_NODEVICE]->retain();
	//�]�wScanning �I��
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	addChild(pSpriteBK); //�[�J layer �h
	m_BtnArray[SCANPG_ERR_NODEVICE]->insertObject(pSpriteBK,UINODEVICE_BK);

	//�W����s ���אּUI_UNIT ��ƴ��@(�ϥ�,�W��)
	CCSprite *pSpriteTopBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteTopBtn->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	pSpriteTopBtn->setPosition(ccp(0,winSize.height-pSpriteTopBtn->getContentSize().height));
	pSpriteBK->addChild(pSpriteTopBtn);
	CCSprite *pSpriteBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //���s�ϥ�
	pSpriteBtnIcon->setPosition(ccp(pSpriteBtnIcon->getContentSize().width/2,pSpriteBtnIcon->getContentSize().height/2));
	pSpriteTopBtn->addChild(pSpriteBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_FunRobot, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSpriteBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pLabel->getContentSize().height/2));
	pSpriteTopBtn->addChild(pLabel, 10);
	m_BtnArray[SCANPG_ERR_NODEVICE]->insertObject(pSpriteTopBtn,UINODEVICE_TOPBTN);

	//�����ϥ� 
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_ERROR_NODEVICE_ICON); //���A�ϥ�
	pSprite->setPosition(ccp(winSize.width/2,winSize.height*4/5));
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[SCANPG_ERR_NODEVICE]->insertObject(pSprite,UINODEVICE_ICON);

	//�ϥܤU���r
	pLabel = CCLabelTTF::create(STR_ERR_NODevice, S_FONE, *pSTR_ERROR_SIZE);
	pLabel->setColor(COLOR_STRING_ERRBLACK);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*3/5));
	pSpriteBK->addChild(pLabel,10);

	//������r
	//�۰��_�� �ϰ�]�m //�a�����
	pLabel = CCLabelTTF::create(STR_HELP_PlzConf, S_FONE, *pSTR_HELP_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentLeft);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*2/4));
	pSpriteBK->addChild(pLabel,10);
	int iHeight=pLabel->getContentSize().height;

	pLabel = CCLabelTTF::create(STR_HELP_RobotConn, S_FONE, *pSTR_HELP_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentLeft);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*2/4-iHeight-pLabel->getContentSize().height*0.5 ));
	pSpriteBK->addChild(pLabel,10);
	iHeight += pLabel->getContentSize().height;

	pLabel = CCLabelTTF::create(STR_HELP_TheMobile, S_FONE, *pSTR_HELP_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentLeft);
		
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*2/4-iHeight-pLabel->getContentSize().height*0.5 ));
	pSpriteBK->addChild(pLabel,10);



	//�������s
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_BTN_Refresh, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(pSpriteDownBtn->getContentSize().width/2,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
	m_BtnArray[SCANPG_ERR_NODEVICE]->insertObject(pSpriteDownBtn,UISCAN_DOWNBTN);

	//�s����r
	pLabel = CCLabelTTF::create(STR_LINK_TrouShoot, S_FONE, *pSTR_LINK_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentCenter);
	iHeight = pLabel->getContentSize().height;
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*1/5-iHeight));
	pLabel->enableStroke(ccWHITE,4,true);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[SCANPG_ERR_NODEVICE]->insertObject(pLabel,UINODEVICE_REPORTLINK);

	pSpriteBK->setVisible(false);
}

void UI_RobotScanning::Show_ErrNoDevice()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SCANPG_ERR_NODEVICE]->objectAtIndex(UINODEVICE_BK);
	pSprite->setVisible(true);
	PageType = SCANPG_ERR_NODEVICE;
}

void UI_RobotScanning::Hide_ErrNoDevice()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SCANPG_ERR_NODEVICE]->objectAtIndex(UINODEVICE_BK);
	pSprite->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
//WIFI LIST
void UI_RobotScanning::InitUI_WifiList()
{
	
	//m_BtnArray[SCANPG_ERR_NODEVICE] = CCArray::createWithCapacity(UINODEVICE_SIZE); //���s�s��
	m_BtnArray[SCANPG_WIFILIST] = CCArray::create();
	m_BtnArray[SCANPG_WIFILIST]->retain();
	//�]�wScanning �I��
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	addChild(pSpriteBK); //�[�J layer �h
	m_BtnArray[SCANPG_WIFILIST]->insertObject(pSpriteBK,UIWIFILIST_BK);

	//�W����s ���אּUI_UNIT ��ƴ��@(�ϥ�,�W��)
	CCSprite *pSpriteTopBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteTopBtn->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	pSpriteTopBtn->setPosition(ccp(0,winSize.height-pSpriteTopBtn->getContentSize().height));
	pSpriteBK->addChild(pSpriteTopBtn);
	CCSprite *pSpriteBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //���s�ϥ�
	pSpriteBtnIcon->setPosition(ccp(pSpriteBtnIcon->getContentSize().width/2,pSpriteBtnIcon->getContentSize().height/2));
	pSpriteTopBtn->addChild(pSpriteBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_FunRobot, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSpriteBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pLabel->getContentSize().height/2));
	pSpriteTopBtn->addChild(pLabel, 10);
	m_BtnArray[SCANPG_WIFILIST]->insertObject(pSpriteTopBtn,UIWIFILIST_TOPBTN);

	//������r
	pLabel = CCLabelTTF::create(STR_STATE_Welcome, S_FONE, *pSTR_STATE_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*4/5));
	pSpriteBK->addChild(pLabel,10);
	int iHeight=pLabel->getContentSize().height;
	pLabel = CCLabelTTF::create(STR_STATE_PlzSel, S_FONE, *pSTR_HELP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*4/5-iHeight));
	pSpriteBK->addChild(pLabel,10);

	//�������s
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_BTN_Refresh, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(pSpriteDownBtn->getContentSize().width/2,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
	m_BtnArray[SCANPG_WIFILIST]->insertObject(pSpriteDownBtn,UIWIFILIST_DOWNBTN);

	//�s����r
	pLabel = CCLabelTTF::create(STR_LINK_TrouShoot, S_FONE, *pSTR_LINK_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentCenter);
	iHeight = pLabel->getContentSize().height+pSpriteDownBtn->getContentSize().height;
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(winSize.width/2,iHeight));
	pLabel->enableStroke(ccWHITE,4,true);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[SCANPG_WIFILIST]->insertObject(pLabel,UIWIFILIST_REPORTLINK);

	//�[�J���� �s���Ÿ�
	for(int i=0;i<MAX_DISPLAY_WIFINAME;i++)
	{
		CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_WIFILIST_ICON);
		iHeight = pSprite->getContentSize().height;
		pSprite->setPosition(ccp(winSize.width-160,winSize.height*3/5-iHeight*2*i));
		pSpriteBK->addChild(pSprite,11);
		m_BtnArray[SCANPG_WIFILIST]->insertObject(pSprite,UIWIFILIST_WIFILIST+i);
		pSprite->setVisible(false);
	}
	pSpriteBK->setVisible(false);
}

void UI_RobotScanning::Show_WifiList()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_BK);
	pSprite->setVisible(true);
	PageType = SCANPG_WIFILIST;

}

void UI_RobotScanning::Hide_WifiList()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_BK);
	pSprite->setVisible(false);
	WifiList_DrawLineNum=0;
}

//����WIFI List name
void UI_RobotScanning::WifiListLabelRemove()
{
	int iWifiNum=m_WifiListSSIDLabel->count();
	CCSprite* pSpriteBK= (CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_BK);
	for(int i=0;i<iWifiNum;i++) //�����Ҧ�Label
	{
		CCNode* pNode=(CCNode*)m_WifiListSSIDLabel->objectAtIndex(i);
		pSpriteBK->removeChild(pNode,true);
		pNode=(CCNode*)m_WifiListAPLabel->objectAtIndex(i);
		pSpriteBK->removeChild(pNode,true);
	}
	m_WifiListSSIDLabel->removeAllObjects();

}

//�bWIFI LIST page ����WIFI
void UI_RobotScanning::WifiListLabelHide()
{
	int iTotalCount=m_WifiListSSIDLabel->count();
	//����LABLE
	for(int i=0;i<iTotalCount;i++)
	{
		CCNode* pNode=(CCNode* )m_WifiListSSIDLabel->objectAtIndex(i);
		pNode->setVisible(false);
		pNode=(CCNode* )m_WifiListAPLabel->objectAtIndex(i);
		pNode->setVisible(false);
	}
	//���ùϥ�
	for(int i=0;i<MAX_DISPLAY_WIFINAME;i++)
	{		
		CCSprite* pSprite=(CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_WIFILIST+i);
		pSprite->setVisible(false);
	}
	
}

//�bWIFI LIST page ���WIFI
void UI_RobotScanning::WifiListLabelDisplay(int iStart)
{
	WifiListLabelHide();
	int iWifiNum=m_WifiListSSIDLabel->count();
	CCSprite* pSpriteBK= (CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_BK);

	//�M�w���AP �_�l
	if(iStart>iWifiNum-MAX_DISPLAY_WIFINAME)
		iStart=iWifiNum-MAX_DISPLAY_WIFINAME;
	if(iStart<0)
		iStart=0;
	WifiList_StartIdx=iStart; //���w�_�lAP�W��
	int icount=0;
	while(icount<MAX_DISPLAY_WIFINAME && iStart<iWifiNum)
	{
		CCLabelTTF* pLabel=(CCLabelTTF*)m_WifiListSSIDLabel->objectAtIndex(iStart);
		bool bShow=pLabel->isVisible();
		int iHeight=pLabel->getContentSize().height;
		pLabel->setPosition(ccp(winSize.width/2,winSize.height*3/5-iHeight*3*icount));
		pLabel->setVisible(true);

		pLabel=(CCLabelTTF*)m_WifiListAPLabel->objectAtIndex(iStart);
		pLabel->setPosition(ccp(winSize.width/2,winSize.height*3/5-iHeight*3*icount+iHeight));
		pLabel->setVisible(true);

		CCSprite* pSprite=(CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_WIFILIST+icount);
		pSprite->setPosition(ccp(winSize.width-160+pSprite->getContentSize().width/2 , winSize.height*3/ 5-iHeight*3*icount-15));
		pSprite->setVisible(true);
		icount++;
		iStart++;
	}
	WifiList_DrawLineNum=icount;
	
	//Remove_WifiLabel();
	//iWifiNum=m_WifiListName->count();
	//pSpriteBK= (CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_BK);
	//for(int i=0;i<iWifiNum;i++)//WIFI �W��	
	//{
	//	//�۰��_�� �ϰ�]�m //�a�����
	//	CCString* pCCstring=(CCString*)m_WifiListName->objectAtIndex(i);
	//	CCLabelTTF* pLabel = CCLabelTTF::create(pCCstring->m_sString.c_str(), S_FONE, *pSTR_WIFILIST_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentLeft);
	//	pLabel->setColor(COLOR_STRING_BLACK);
	//	int iHeight=pLabel->getContentSize().height;
	//	pLabel->setPosition(ccp(winSize.width/2,winSize.height*3/5-iHeight*3*i));
	//	pLabel->setVisible(true);
	//	pSpriteBK->addChild(pLabel);
	//	m_WifiListLabel->addObject(pLabel);
	//}
}

//���J�Ҧ�WIFI SSID AP�W��
void UI_RobotScanning::WifiListLabelLoad()
{
	WifiListLabelRemove();
	int iWifiNum=m_WifiListSSIDName->count();
	int iCount=m_WifiListSSIDLabel->count();
	CCSprite* pSpriteBK= (CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_BK);
	ccColor3B pCOLOR;
	for(int i=0;i<iWifiNum;i++)//WIFI SSID �W��	
	{
		//SSID �۰��_�� �ϰ�]�m //�a�����
		CCString* pCCstring=(CCString*)m_WifiListSSIDName->objectAtIndex(i);
		CCLabelTTF* pLabel = CCLabelTTF::create(pCCstring->m_sString.c_str(), S_FONE, *pSTR_WIFILIST_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentLeft);
		if(iLinkReady[i]==0) //�P�_�O�_�w�g�s�u
			pCOLOR=COLOR_STRING_BLACK;
		else
			pCOLOR=COLOR_STRING_GREEN;

		pLabel->setColor(pCOLOR);
		int iHeight=pLabel->getContentSize().height;
		pLabel->setPosition(ccp(winSize.width/2,winSize.height*3/5-iHeight*3*i));
		pLabel->setVisible(false);
		pSpriteBK->addChild(pLabel);
		m_WifiListSSIDLabel->addObject(pLabel);

		//AP
		//if(m_WifiListAPName->lastObject()>i)
		{
			pCCstring=(CCString*)m_WifiListAPName->objectAtIndex(i);
			pLabel = CCLabelTTF::create(pCCstring->m_sString.c_str(), S_FONE, *pSTR_WIFILIST_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentLeft);
			pLabel->setPosition(ccp(winSize.width/2,winSize.height*3/5-iHeight*3*i+iHeight));
			pLabel->setVisible(false);
			pSpriteBK->addChild(pLabel);
			m_WifiListAPLabel->addObject(pLabel);
		}
		

	}	
}

//�������A �[�ֹϥ�
void WifiSearchIcon_AddSpeed()
{
	m_pUIRobotSacnning->SearchIconFast();
	//m_SearchType=Search_Loop2;
}
//�j�MWIFI�W��AP NAME
//�]��cocos2d ��thread �޲z�O���馳���D �ɶq�קKcreate ��ƨϥ� ���new

void UI_RobotScanning::WifiEnum()
{

	m_SearchType=Search_Loop;
	m_pUIRobotSacnning->SearchIconSlow();
	GFUN_CreadWifi();	
	int iResult = GFUN_MainSearchNothread(APName,SSIDName,iLinkReady);
	if(iResult ==ERROR_SUCCESS) //�ǿ馨�\
	{
		m_SearchType=Search_APSuccess;
	}
	else //�ǿ饢�Ѷi�J
	{
		m_SearchType=Search_APFail;
	}
	//m_WifiScan.MainSearch();
}

void UI_RobotScanning::Function_ErrNoDevice(int iBtnType)
{
	switch(iBtnType)
	{
	default:
	case 	UINODEVICE_BK:
	case UINODEVICE_TOPBTN:
	case UINODEVICE_ICON:
		break;	
	case UINODEVICE_REPORTLINK: //�}�ҳs��
		break;
	case UINODEVICE_DOWNBTN: //Refreash
		Hide_ErrNoDevice();
		Show_SacnPage();
		break;
	}
}
void UI_RobotScanning::Function_WifiList(int iBtnType)
{
	switch(iBtnType)
	{	
	case UIWIFILIST_BK:
	case UIWIFILIST_TOPBTN:
	case UIWIFILIST_REPORTLINK:
		break;	
	case UIWIFILIST_DOWNBTN:  //Refreash
		Hide_WifiList();
		Show_SacnPage();
		break;
	default:
	case UIWIFILIST_WIFILIST:
		ConnectRobotStart(iBtnType-UIWIFILIST_WIFILIST+WifiList_StartIdx);
		break;
	}
}
void UI_RobotScanning::Function_ErrCopnnFail(int iBtnType)
{
	switch(iBtnType)
	{	
	case ERRCONNFAIL_BK:
	case ERRCONNFAIL_ICON:
	case ERRCONNFAIL_REPORTLINK:
		break;	
	case ERRCONNFAIL_CANCEL:  //Refreash
		Hide_ErrConnFail();
		//Show_WifiList();
		Show_SacnPage();
		break;
	default:
		break;
	case ERRCONNFAIL_RETRY:
		Hide_ErrConnFail();
		ConnectRobotStart(m_pUIRobotSacnning->GetSelApIdx());
		break;
	}
}

//�����C��
//�I������W�� ���W�j�M
//�I������U�� ���U�j�M
void UI_RobotScanning::WifiLabelUpDown(CCPoint MousePoint)
{
	if(MousePoint.y > winSize.height/2)
		WifiListLabelDisplay(WifiList_StartIdx-1);
	else
		WifiListLabelDisplay(WifiList_StartIdx+1);
}

//////////////////////////////////////////////////////////////////////////
//Robot Sacnning View
void UI_RobotScanning::InitUI_ConnectAni()
{

	//m_BtnArray[SCANPG_ERR_NODEVICE] = CCArray::createWithCapacity(UINODEVICE_SIZE); //���s�s��
	m_BtnArray[SCANPG_CONNECTANI] = CCArray::create();
	m_BtnArray[SCANPG_CONNECTANI]->retain();
	//�]�wScanning �I��
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	addChild(pSpriteBK); //�[�J layer �h
	m_BtnArray[SCANPG_CONNECTANI]->insertObject(pSpriteBK,CONNECTANI_BK);


	//�����ϥ� 
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_STATE_SCANING_ICON); //���A�ϥ�
	pSprite->setPosition(ccp(winSize.width/2,winSize.height*4/5));
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[SCANPG_CONNECTANI]->insertObject(pSprite,CONNECTANI_ICON);

	//�ϥܤU���r
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_STATE_Connecting, S_FONE, *pSTR_STATE_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*3/5));
	pSpriteBK->addChild(pLabel,10);

	//������r
	//�۰��_�� �ϰ�]�m //�a�����
	//AP name
	pLabel = CCLabelTTF::create("Machine Name", S_FONE, *pSTR_HELP_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentCenter);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*2/4));
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[SCANPG_CONNECTANI]->insertObject(pLabel,CONNECTANI_Name);
	//SSID anme
	int iHeight=pLabel->getContentSize().height;
	pLabel = CCLabelTTF::create("Machine SSID", S_FONE, *pSTR_HELP_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentCenter);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*2/4-iHeight-pLabel->getContentSize().height*0.5 ));
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[SCANPG_CONNECTANI]->insertObject(pLabel,CONNECTANI_SSID);

	pSpriteBK->setVisible(false);
}

void UI_RobotScanning::Show_ConnectAni()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SCANPG_CONNECTANI]->objectAtIndex(UINODEVICE_BK);
	pSprite->setVisible(true);
	m_SearchType=CONNECT_Ready; 
	PageType = SCANPG_CONNECTANI;
}

void UI_RobotScanning::Hide_ConnectAni()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SCANPG_CONNECTANI]->objectAtIndex(UINODEVICE_BK);
	pSprite->setVisible(false);
}

//Select your Robot machine and get the machinc name pointed at pCCstring pointer.
void UI_RobotScanning::ConnectRobotStart(int iBtnNum)
{	
	CCLabelTTF* pLabel = (CCLabelTTF*)m_BtnArray[SCANPG_CONNECTANI]->objectAtIndex(CONNECTANI_SSID);
	//How to save the Robot name for your app.
	CCString* pCCstring=(CCString*)m_WifiListSSIDName->objectAtIndex(iBtnNum);
	pLabel->setString(pCCstring->getCString());
	//pLabel->setString(m_WifiScan.GetSSIDName(iBtnNum)); //���ýX ����i

	Hide_AllPage();
	Show_ConnectAni();	
	WifiList_SelAPIdx=iBtnNum;//�����I��AP
	ConnectRobotFun();
}

//�s��ROBOT
void UI_RobotScanning::ConnectRobotFun()
{
	SetRobotFun_ConnectAp();
	m_SearchType=CONNECT_WaitIP;
}
//////////////////////////////////////////////////////////////////////////

//�s��ROBOT ����
void UI_RobotScanning::InitUI_ErrConnFail()
{
	//m_BtnArray[SCANPG_ERR_NODEVICE] = CCArray::createWithCapacity(UINODEVICE_SIZE); //���s�s��
	m_BtnArray[SCANPG_ERR_CONNFAIL] = CCArray::create();
	m_BtnArray[SCANPG_ERR_CONNFAIL]->retain();
	//�]�wScanning �I��
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	addChild(pSpriteBK); //�[�J layer �h
	m_BtnArray[SCANPG_ERR_CONNFAIL]->insertObject(pSpriteBK,ERRCONNFAIL_BK);


	//�����ϥ� 
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_ERROR_CONNFAIL_ICON); //���A�ϥ�
	pSprite->setPosition(ccp(winSize.width/2,winSize.height*4/5));
	pSprite->setScale(2); // ��j�⭿
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[SCANPG_ERR_CONNFAIL]->insertObject(pSprite,ERRCONNFAIL_ICON);

	//�ϥܤU���r
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_ERR_ConnFail, S_FONE, *pSTR_STATE_SIZE);
	pLabel->setColor(COLOR_STRING_RED);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*3/5));
	pSpriteBK->addChild(pLabel,10);

	//������r
	//�۰��_�� �ϰ�]�m //�a�����
	pLabel = CCLabelTTF::create(STR_HELP_PlzConf, S_FONE, *pSTR_HELP_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentLeft);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*2/4));
	pSpriteBK->addChild(pLabel,10);
	int iHeight=pLabel->getContentSize().height;

	pLabel = CCLabelTTF::create(STR_HELP_RobotConn, S_FONE, *pSTR_HELP_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentLeft);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*2/4-iHeight-pLabel->getContentSize().height*0.5 ));
	pSpriteBK->addChild(pLabel,10);
	iHeight += pLabel->getContentSize().height;

	pLabel = CCLabelTTF::create(STR_HELP_TheMobile, S_FONE, *pSTR_HELP_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentLeft);

	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*2/4-iHeight-pLabel->getContentSize().height*0.5 ));
	pSpriteBK->addChild(pLabel,10);

	//�s����r
	pLabel = CCLabelTTF::create(STR_LINK_TrouShoot, S_FONE, *pSTR_LINK_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentCenter);
	iHeight = pLabel->getContentSize().height;
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(winSize.width/2,winSize.height*1/5-iHeight));
	pLabel->enableStroke(ccWHITE,4,true);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[SCANPG_ERR_CONNFAIL]->insertObject(pLabel,ERRCONNFAIL_REPORTLINK);


	//�������s
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setScaleX(0.5);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);

	pLabel = CCLabelTTF::create(STR_BTN_Canel, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setScaleX(2);
	pLabel->setPosition(ccp(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
	m_BtnArray[SCANPG_ERR_CONNFAIL]->insertObject(pSpriteDownBtn,ERRCONNFAIL_CANCEL);

	pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	pSpriteDownBtn->setPosition(ccp(0+winSize.width*0.5+4,0)); //+4 FOR �����u
	pSpriteDownBtn->setScaleX(0.5);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_BTN_Retry, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setScaleX(2);
	pLabel->setPosition(ccp(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
	m_BtnArray[SCANPG_ERR_CONNFAIL]->insertObject(pSpriteDownBtn,ERRCONNFAIL_RETRY);


	pSpriteBK->setVisible(false);

}

void UI_RobotScanning::Show_ErrConnFail()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SCANPG_ERR_CONNFAIL]->objectAtIndex(ERRCONNFAIL_BK);
	pSprite->setVisible(true);
	PageType = SCANPG_ERR_CONNFAIL;
}

void UI_RobotScanning::Hide_ErrConnFail()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SCANPG_ERR_CONNFAIL]->objectAtIndex(ERRCONNFAIL_BK);
	pSprite->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
//Preass even program
//�ƹ��I�U!�P�_�O���Ӥ���
bool UI_RobotScanning::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    

	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	if(!isVisible()) //����ܤ~�~��
		return false;
	bool bTouchEvenOver=false;
	switch(PageType)
	{
	case 	SCANPG_SEARCH:
		{
			if(IsRobotThreadIdle()) // �קK�b�j�M�� �S�I��e��
			{
				CallThread_SearchRobot();
			}
		}
		break;
	case	SCANPG_ERR_NODEVICE:
		{
			//�P�_���@�Ӥ���
			for (int i = UINODEVICE_TOPBTN; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch)
				{
					Function_ErrNoDevice(i);
					break;
				}
			}  		
		}
		break;
	case SCANPG_WIFILIST:
		{
			//�P�_���@�Ӥ���
			for (int i = UIWIFILIST_TOPBTN; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch)
				{
#ifdef ROBOT_DISABLE
					m_SearchType=CONNECT_Success; //�j��s�u���Q
					ChangLayer_Main();
#else
					Function_WifiList(i);
#endif
					break;
				}
			}  
			//�p�G�O�I��ୱ ����List
			WifiLabelUpDown(touchLocation);
		}
		break;
	case SCANPG_ERR_CONNFAIL:
		//�P�_���@�Ӥ���
		for (int i = ERRCONNFAIL_REPORTLINK; i < m_BtnArray[PageType]->count(); i++)
		{  
			CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
			bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
			if(bTouch)
			{
				Function_ErrCopnnFail(i);
				break;
			}
		}  
		break;
	default:
		break;
	}

	return true;
}

void UI_RobotScanning::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
}

void UI_RobotScanning::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
}
