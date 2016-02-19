#include <ImageList.h>
#include <HelloWorldScene.h>

#if PLAYERSTAGE
#include <libplayerc++/playerc++.h>
#include <pthread.h>
#include <semaphore.h>
#include <MAP>
#include "player.h"

#endif



#if ROBOT
#include <wifiscan.h>
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <boost/format.hpp>
#endif

USING_NS_CC;
using namespace std;
extern HelloWorld* m_MainView;

#if PLAYERSTAGE
using namespace PlayerCc;
/// Player Client
extern PlayerClient *pPlayerclient;
extern DispatcherProxy *pDisProxy;
extern PowerProxy	 *pPowerProxy;
extern Position2dProxy *pPosition2dProxy;
extern player_map_info_t stMapInfo;
#endif

extern UI_RobotScanning* m_pUIRobotSacnning;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
extern UI_RobotMain* m_pUIRobotMain;
extern UI_Reservation* m_pUIReservation;
#endif


//extern sem_t semRobMapRelease;
UI_RobotScanning::UI_RobotScanning():
WifiList_DrawLineNum(0),
WifiList_StartIdx(0),
WifiList_SelAPIdx(0),
PageType(SCANPG_LAYER),
m_SearchType(Search_None),
iIconSpeed_Search(1),
iIconSpeed_Connect(1),
MsgBoxShow_RobotTimeFailGet(false),
MsgBoxShow_RobotStateFailGet(false),
MsgBoxShow_RobotPowerFailGet(false),
RobotPowerValue(0.0),
touchEnabled(false),
touchListener(nullptr)
{

    
    for(int i=0;i<SCANPG_SIZE;i++)
    {
        auto Page = new SpriteVector;
        m_SpriteVectors.push_back(Page);
        auto Page2 = new LabelVector;
        m_LabelVectors.push_back(Page2);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	InitRobotTime();
#endif
}
UI_RobotScanning::~UI_RobotScanning()
{
#if WIFI
	GFUN_FreeWifiScanMemory();
#endif

    for(auto pData:m_SpriteVectors)
    {
        delete pData;
    }
    for(auto pData:m_LabelVectors)
    {
        delete pData;
    }
    m_SpriteVectors.clear();
    m_LabelVectors.clear();
    m_WifiListSSIDName.clear();
    m_WifiListSSIDLabel.clear();
    m_WifiListAPName.clear();
    m_WifiListAPLabel.clear();
#if PLAYERSTAGE
	if(pPlayerclient!=NULL)
	{
		pPlayerclient->StopThread();
		delete pPlayerclient;
		pPlayerclient=NULL;
	}
#endif

}
int UI_RobotScanning::GetSelApIdx()
{
	return WifiList_SelAPIdx;
}


void UI_RobotScanning::ConnectFailSetting(int iType)
{
	m_SearchType=iType;
}
//≥s±µROBOT 
//®√•B¶^∂«≥s±µ™¨∫A
int UI_RobotScanning::ConnectRobot(float dt)
{
#if PLAYERSTAGE
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
		GuiData_Connect(); //¶A¶∏´ÿ•ﬂ≥sΩu
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

			RobotMapUpdate();//	sem_post(&semRobMapRelease); //ßÛ∑s¶aπœ
			m_SearchType=CONNECT_Success;
			iConnectState=RobSt_OK;
		}
		else
		{
			m_SearchType=CONNECT_Fail_NoProxy;
			iConnectState=RobSt_NoProxyFail;
			std::string sValue=boost::str(boost::format( "%s" ) % STR_ERR_ConnFail_NoProxy_ENG );
			CCMessageBox(sValue.c_str(),"ERROR");
			//MessageBoxA(NULL,(LPCSTR)sValue.c_str(),"ERROR",MB_OK+MB_SYSTEMMODAL); //µ•´›πLµ{°@m_SearchType°@¶≥æ˜∑|≥QßÔºg
			m_SearchType=CONNECT_Fail_NoProxy;

		}
		
		
	}
	catch (PlayerCc::PlayerError &e) //≥s±µ≤ß±`∑|∏ı¶‹¶π≥B
	{
		
		m_SearchType=CONNECT_Fail_NoRobotAck;

		//¶π≥Bª›ßÔ¨∞≥Ã§Wºh •Bµ¯µ°•[§j
		std::string sValue=boost::str(boost::format( "code %d:%s(LinkState:%d)" ) % e.GetErrorCode() % STR_ERR_ConnFail_NoRobotAck_ENG % iConnectState );
		CCMessageBox(sValue.c_str(),e.GetErrorStr().c_str());
		m_SearchType=CONNECT_Fail_NoRobotAck;
	}
	return iConnectState;
#else
    return 1;
#endif
}
//Get robot name.
_RobotCommandState_ UI_RobotScanning::GetRobotName(char** pRobotName)
{
	_RobotCommandState_ iConnectState=RobCmdSt_Unknow;
#if PLAYERSTAGE
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
#else
    iConnectState = RobCmdSt_OK;
#endif
	return iConnectState;
}
//SetUiTime ≥]©wROBOT Æ…∂°
_RobotCommandState_ UI_RobotScanning::GetRobotTime()
{
	_RobotCommandState_ iConnectState=RobCmdSt_Unknow;
#if PLAYERSTAGE
	try
	{
		if(pDisProxy)
		{
			//®˙±oæ˜æπÆ…∂°
			//pDisProxy->GetReqUiTime((uint8_t *)&RobotTime);
			//pDisProxy->GetUiStatus(PLAYER_DISPATCH_INIT_DEST_RD, &uiStatus); //®˙±oæ˜•x™¨∫A
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
		{ //∂»≈„•‹§@¶∏
			MsgBoxShow_RobotTimeFailGet=true;
			std::string sValue=boost::str(boost::format( "code %d:%s" ) % e.GetErrorCode() % STR_ERR_ConnFail_NoRobotTime_ENG );
			CCMessageBox(sValue.c_str(),e.GetErrorStr().c_str());
		}
		iConnectState=RobCmdSt_Timeout;

	}
#else
    iConnectState = RobCmdSt_OK;
#endif
	return iConnectState;
}

//SetUiTime ≥]©wROBOT Æ…∂°
_RobotCommandState_ UI_RobotScanning::GetRobotState(uint32_t &aiStatus,uint32_t &cleanStatus,uint32_t &error,uint32_t &worry)
{
	_RobotCommandState_ iConnectState=RobCmdSt_Unknow;
#if PLAYERSTAGE
	try
	{
		if(pDisProxy)
		{
			//®˙±oæ˜æπÆ…∂°
			//pDisProxy->GetReqUiTime((uint8_t *)&RobotTime);
			//pDisProxy->GetUiStatus(PLAYER_DISPATCH_INIT_DEST_RD, &uiStatus); //®˙±oæ˜•x™¨∫A
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
		{ //∂»≈„•‹§@¶∏
			MsgBoxShow_RobotStateFailGet=true;
			std::string sValue=boost::str(boost::format( "code %d:%s" ) % e.GetErrorCode() % STR_ERR_ConnFail_NoRobotTime_ENG );
			CCMessageBox(sValue.c_str(),e.GetErrorStr().c_str());			
		}

		iConnectState=RobCmdSt_Timeout;

	}
#else
    iConnectState = RobCmdSt_OK;
#endif
	return iConnectState;
}

//SetUiTime ≥]©wROBOT Æ…∂°
float UI_RobotScanning::GetRobotPowerValue()
{
	return RobotPowerValue;
}
_RobotCommandState_ UI_RobotScanning::GetRobotPower( )
{
	_RobotCommandState_ iConnectState=RobCmdSt_Unknow;
#if PLAYERSTAGE
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
		{ //∂»≈„•‹§@¶∏
			MsgBoxShow_RobotPowerFailGet=true;
			std::string sValue=boost::str(boost::format( "code %d:%s" ) % e.GetErrorCode() % STR_ERR_ConnFail_NoRobotPOWER_ENG );
			CCMessageBox(sValue.c_str(),e.GetErrorStr().c_str());
		}
		iConnectState=RobCmdSt_Timeout;

	}
#else
    iConnectState = RobCmdSt_OK;
#endif
	return iConnectState;
}

//SetUiTime ≥]©wROBOT Æ…∂°
_RobotCommandState_ UI_RobotScanning::GetRobotSchedule()
{
	_RobotCommandState_ iConnectState=RobCmdSt_Unknow;
#if PLAYERSTAGE
	try
	{
		if(pDisProxy)
		{			
			uint32_t dataCount;
			player_dispatcher_ui_alarm_time_t *pAlarm;
			pAlarm=pDisProxy->ReqUiSchedule(PLAYER_DISPATCH_INIT_DEST_RD, &dataCount);
			if(dataCount>=G_ROBOT_SCHEDULE_MAX) //º∆∂qπL¶h ±j®Ó®˙100≠”
			{
				CCLog("Schedule %d Over Size %d",dataCount,G_ROBOT_SCHEDULE_MAX);
			}

			iConnectState = RobCmdSt_OK;
			for(int i=0;i<dataCount;i++)
			{
				m_pUIReservation->SaveRobotSachedule((void*)&pAlarm[i]);				
			}
			m_pUIReservation->ResetSettingByRobotSchedule(); //Æ⁄æ⁄ROBOT ≥]©w ≠◊ßÔ•ªæ˜≥]©w
		}
		else
			iConnectState = RobCmdSt_Nopoint;		
	}
	catch (PlayerCc::PlayerError &e)
	{
		iConnectState=RobCmdSt_Timeout;
	}
#else
    iConnectState = RobCmdSt_OK;
#endif
	return iConnectState;
}


bool UI_RobotScanning::init()
{  
	if (!Layer::init())
	{  
		return false;  
	}  
	InitUI();  
	RobotConnectThread_Cread();
	return true;
}  

void UI_RobotScanning::InitUI()
{
    winSize = Director::getInstance()->getVisibleSize();
    winOrigin = Director::getInstance()->getVisibleOrigin();
    InitUI_ScaningPage();
	InitUI_ErrorNoDevice(); //ERROR MSG - No DEVICE
	InitUI_WifiList();
	InitUI_ConnectAni();
	InitUI_ErrConnFail();
	//schedule( schedule_selector(UI_RobotScanning::LinkRobotLoop));
	scheduleUpdate();     //±“∞ update()

}
Sprite* UI_RobotScanning::getSpritePoint(int iPageTyep,int iPageIdx)
{
    return m_SpriteVectors.at(iPageTyep)->at(m_itemIdx[iPageTyep*100+iPageIdx]);
}
Label* UI_RobotScanning::getLabelPoint(int iPageTyep,int iPageIdx)
{
    return m_LabelVectors.at(iPageTyep)->at(m_itemIdx[iPageTyep*100+iPageIdx]);
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
	case  Search_Loop:  //∑j¥M§~πœ•‹±€¬‡ 
		{
            
            auto pSpr = getSpritePoint(SCANPG_SEARCH,UISCAN_ICON);
            pSpr->setRotation(pSpr->getRotationX()+iIconSpeed_Search);
            pSpr = getSpritePoint(SCANPG_SEARCH,UISCAN_DOWNBTN);
            pSpr->setColor(COLOR_STRING_GRAY);
		}
		break;
	default:
	case Search_APReady:
	case Search_None:
		break;
	case  Search_APFail:
		{
            auto pSpr = getSpritePoint(SCANPG_SEARCH,UISCAN_DOWNBTN);
			pSpr->setColor(COLOR_STRING_GREEN);
			Hide_AllPage();
			Show_ErrNoDevice();
			m_SearchType =Search_None;
		}
		break;
	case  Search_APSuccess:
		{
			auto pSpr = getSpritePoint(SCANPG_SEARCH,UISCAN_DOWNBTN);
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
#if WIFI
	// int iCount=SSIDName.size();
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
	
#endif
    WifiListLabelDisplay(0);
}
//πœ•‹øÔ¬‡≥t´◊≥]©w
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
		{ //πœ•‹±€¬‡
            auto pSpr = getSpritePoint(SCANPG_SEARCH,CONNECTANI_ICON);
            pSpr->setRotation(pSpr->getRotationX()+iIconSpeed_Connect);
		}
		break;
	case CONNECT_Success: 
		{//§¡¥´®Ï•D§∂≠±
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			ChangLayer_Main();
#endif
			m_SearchType=Search_None;
		}
		break;

	}

}
void UI_RobotScanning::update(float dt)
{

//Æ⁄æ⁄≠∂≠±§£¶P ®M©w∞ µe≈„•‹
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
    auto pageSprite = m_SpriteVectors.at(SCANPG_SEARCH);
//    auto pageLabel = m_LabelVectors.at(SCANPG_SEARCH);
    
	auto pSpriteScanning=Sprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
    winSize=pSpriteScanning->getBoundingBox().size; //改用底圖當作視窗範圍
    
    pSpriteScanning->setPosition(winOrigin+Vec2(winSize/2));
	addChild(pSpriteScanning);
    pSpriteScanning->setTag(UISCAN_BK);
    pageSprite->pushBack(pSpriteScanning);
    m_itemIdx[SCANPG_SEARCH*100+UISCAN_BK]=pageSprite->size()-1;
    
    //上方灰色抬頭
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setPosition(Vec2(pSpriteAction->getBoundingBox().size.width/2,winSize.height- pSpriteAction->getBoundingBox().size.height/2));
	pSpriteScanning->addChild(pSpriteAction);
    
	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //返回上一頁
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSprBtnIcon->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
    auto pLabel = Label::createWithSystemFont(STR_TITAL_FunRobot, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pLabel->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    pSpriteAction->setTag(UISCAN_TOPBTN);
    pageSprite->pushBack(pSpriteAction);
    m_itemIdx[SCANPG_SEARCH*100+UISCAN_TOPBTN]=pageSprite->size()-1;
   
    //搜尋圖示
	auto pSpr=Sprite::createWithSpriteFrameName(S_STATE_SCANING_ICON);
	pSpr->setPosition(Vec2(winSize.width/2,winSize.height*4/5));
	pSpriteScanning->addChild(pSpr,10);
    //使其旋轉
    auto SpriteReturn= RepeatForever::create( RotateBy::create(2,360)); //兩秒轉一圈
    pSpr->runAction(SpriteReturn);
    pSpr->setTag(UISCAN_ICON);
    pageSprite->pushBack(pSpr);
    m_itemIdx[SCANPG_SEARCH*100+UISCAN_ICON]=pageSprite->size()-1;

	//πœ•‹§U§Ë§Â¶r
	pLabel = Label::createWithSystemFont(STR_STATE_Scaning, S_FONE, *pSTR_STATE_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*3/5));
	pSpriteScanning->addChild(pLabel,10);

	//ª°©˙§Â¶r

	pLabel = Label::createWithSystemFont(STR_HELP_PlzWait, S_FONE, *pSTR_HELP_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*2/4));
	pSpriteScanning->addChild(pLabel,10);

	pLabel = Label::createWithSystemFont(STR_HELP_AutoSearch, S_FONE, *pSTR_HELP_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	int iHeight=pLabel->getContentSize().height;
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*2/4-iHeight*1.5 ));
	pSpriteScanning->addChild(pLabel,10);
	//©≥≥°´ˆ∂s
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //≥]©w¡„¬I
	pSpriteAction->setPosition(Vec2::ZERO);
	pSpriteAction->setColor(COLOR_STRING_GRAY);
	pSpriteScanning->addChild(pSpriteAction);
	pLabel = Label::createWithSystemFont(STR_BTN_Refresh, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteAction->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel,10);
    pSpriteAction->setTag(UISCAN_DOWNBTN);
    pageSprite->pushBack(pSpriteAction);
    m_itemIdx[SCANPG_SEARCH*100+UISCAN_DOWNBTN]=pageSprite->size()-1;
    
	pSpriteScanning->setVisible(false);
}
void UI_RobotScanning::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true); //ＩＯＳ 預設開啟所以不用設定
	Show_SacnPage();
//    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this); //開啟觸碰功能
    
}

void UI_RobotScanning::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();
//    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this); //停止觸碰功能

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
    auto pSpr = getSpritePoint(SCANPG_SEARCH,UISCAN_BK);
    pSpr->setVisible(true);
    PageType = SCANPG_SEARCH;
    CallThread_SearchRobot();
}
void UI_RobotScanning::Hide_SacnPage()
{
    auto pSpr = getSpritePoint(SCANPG_SEARCH,UISCAN_BK);
	pSpr->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
//Robot Sacnning View
void UI_RobotScanning::InitUI_ErrorNoDevice()
{
    auto pageSprite = m_SpriteVectors.at(SCANPG_ERR_NODEVICE);
    auto pageLabel = m_LabelVectors.at(SCANPG_ERR_NODEVICE);
	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
    
    pSpriteBK->setAnchorPoint(Vec2::ZERO); //≥]©w¡„¬I
	addChild(pSpriteBK); //•[§J layer ºh
    pSpriteBK->setTag(UINODEVICE_BK);
    pageSprite->pushBack(pSpriteBK);
    m_itemIdx[SCANPG_ERR_NODEVICE*100+UINODEVICE_BK]=pageSprite->size()-1;
    

    //上方灰色抬頭
    auto pSpriteTopBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
//    pSpriteTopBtn->setAnchorPoint(Vec2::ZERO); //≥]©w¡„¬I
    pSpriteTopBtn->setPosition(Vec2(pSpriteTopBtn->getBoundingBox().size.width/2,winSize.height- pSpriteTopBtn->getBoundingBox().size.height/2));

//	pSpriteTopBtn->setPosition(Vec2(0,winSize.height-pSpriteTopBtn->getContentSize().height));
	pSpriteBK->addChild(pSpriteTopBtn);
	auto pSpriteBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //´ˆ∂sπœ•‹
	pSpriteBtnIcon->setPosition(Vec2(pSpriteBtnIcon->getContentSize().width/2,pSpriteBtnIcon->getContentSize().height/2));
	pSpriteTopBtn->addChild(pSpriteBtnIcon);
	auto pLabel = Label::createWithSystemFont(STR_TITAL_FunRobot, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSpriteBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pLabel->getContentSize().height/2));
	pSpriteTopBtn->addChild(pLabel, 10);
    pSpriteTopBtn->setTag(UINODEVICE_TOPBTN);
    pageSprite->pushBack(pSpriteTopBtn);
    m_itemIdx[SCANPG_ERR_NODEVICE*100+UINODEVICE_TOPBTN]=pageSprite->size()-1;

	//§§∂°πœ•‹ 
	auto  pSprite=Sprite::createWithSpriteFrameName(S_ERROR_NODEVICE_ICON); //™¨∫Aπœ•‹
	pSprite->setPosition(Vec2(winSize.width/2,winSize.height*4/5));
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(UINODEVICE_ICON);
    pageSprite->pushBack(pSprite);
    m_itemIdx[SCANPG_ERR_NODEVICE*100+UINODEVICE_ICON]=pageSprite->size()-1;
	//m_BtnArray[SCANPG_ERR_NODEVICE]->insertObject(pSprite,UINODEVICE_ICON);

	//πœ•‹§U§Ë§Â¶r
	pLabel = Label::createWithSystemFont(STR_ERR_NODevice, S_FONE, *pSTR_ERROR_SIZE);
	pLabel->setColor(COLOR_STRING_ERRBLACK);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*3/5));
	pSpriteBK->addChild(pLabel,10);

	//ª°©˙§Â¶r
	//¶€∞ ¬_¶Ê ∞œ∞Ï≥]∏m //æa•™πÔªÙ
    pLabel = Label::createWithSystemFont(STR_HELP_PlzConf, S_FONE, *pSTR_HELP_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*2/4));
	pSpriteBK->addChild(pLabel,10);
	auto iHeight=pLabel->getContentSize().height;

	pLabel = Label::createWithSystemFont(STR_HELP_RobotConn, S_FONE, *pSTR_HELP_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*2/4-iHeight-pLabel->getContentSize().height*0.5 ));
	pSpriteBK->addChild(pLabel,10);
	iHeight += pLabel->getContentSize().height;

    pLabel = Label::createWithSystemFont(STR_HELP_TheMobile, S_FONE, *pSTR_HELP_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::LEFT);
		
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*2/4-iHeight-pLabel->getContentSize().height*0.5 ));
	pSpriteBK->addChild(pLabel,10);



	//©≥≥°´ˆ∂s
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //≥]©w¡„¬I
	pSpriteDownBtn->setPosition(Vec2::ZERO);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = Label::createWithSystemFont(STR_BTN_Refresh, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width/2,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
    pSpriteDownBtn->setTag(UISCAN_DOWNBTN);
    pageSprite->pushBack(pSpriteDownBtn);
    m_itemIdx[SCANPG_ERR_NODEVICE*100+UISCAN_DOWNBTN]=pageSprite->size()-1;

	//≥sµ≤§Â¶r
    pLabel = Label::createWithSystemFont(STR_LINK_TrouShoot, S_FONE, *pSTR_LINK_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::LEFT);
	iHeight = pLabel->getContentSize().height;
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*1/5-iHeight));
	pSpriteBK->addChild(pLabel,10);
    pLabel->setTag(UINODEVICE_LAB_REPORTLINK);
    pageLabel->pushBack(pLabel);
    m_itemIdx[SCANPG_ERR_NODEVICE*100+UINODEVICE_LAB_REPORTLINK]=pageLabel->size()-1;

	pSpriteBK->setVisible(false);
}

void UI_RobotScanning::Show_ErrNoDevice()
{
    auto pSprite =getSpritePoint(SCANPG_ERR_NODEVICE,UINODEVICE_BK);
	pSprite->setVisible(true);
	PageType = SCANPG_ERR_NODEVICE;
}
void UI_RobotScanning::Hide_ErrNoDevice()
{
    auto pSprite = getSpritePoint(SCANPG_ERR_NODEVICE,UINODEVICE_BK);
	pSprite->setVisible(false);
}
//////////////////////////////////////////////////////////////////////////
//WIFI LIST
void UI_RobotScanning::InitUI_WifiList()
{
	
	//m_BtnArray[SCANPG_ERR_NODEVICE] = CCArray::createWithCapacity(UINODEVICE_SIZE);
    auto pageSprite = m_SpriteVectors.at(SCANPG_WIFILIST);
    auto pageLabel = m_LabelVectors.at(SCANPG_WIFILIST);
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
    

    pSpriteBK->setPosition(Vec2(winSize / 2) + winOrigin);
    addChild(pSpriteBK); //•[§J layer ºh
    pSpriteBK->setTag(UIWIFILIST_BK);
    pageSprite->pushBack(pSpriteBK);
    m_itemIdx[SCANPG_WIFILIST*100+UIWIFILIST_BK]=pageSprite->size()-1;
    
    //上方灰色抬頭
    auto pSpriteTopBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteTopBtn->setPosition(Vec2(pSpriteTopBtn->getBoundingBox().size.width/2,winSize.height- pSpriteTopBtn->getBoundingBox().size.height/2));
	pSpriteBK->addChild(pSpriteTopBtn);
    auto pSpriteBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON);
	pSpriteBtnIcon->setPosition(Vec2(pSpriteBtnIcon->getContentSize().width/2,pSpriteBtnIcon->getContentSize().height/2));
	pSpriteTopBtn->addChild(pSpriteBtnIcon);
	auto pLabel = Label::createWithSystemFont(STR_TITAL_FunRobot, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSpriteBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pLabel->getContentSize().height/2));
	pSpriteTopBtn->addChild(pLabel, 10);
    pSpriteTopBtn->setTag(UIWIFILIST_TOPBTN);
    pageSprite->pushBack(pSpriteTopBtn);
    m_itemIdx[SCANPG_WIFILIST*100+UIWIFILIST_TOPBTN]=pageSprite->size()-1;

    //§§∂°§Â¶r
	pLabel = Label::createWithSystemFont(STR_STATE_Welcome, S_FONE, *pSTR_STATE_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*4/5));
	pSpriteBK->addChild(pLabel,10);
	auto  iHeight=pLabel->getContentSize().height;
	pLabel = Label::createWithSystemFont(STR_STATE_PlzSel, S_FONE, *pSTR_HELP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*4/5-iHeight));
	pSpriteBK->addChild(pLabel,10);

	//©≥≥°´ˆ∂s
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //≥]©w¡„¬I
	pSpriteDownBtn->setPosition(Vec2::ZERO);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = Label::createWithSystemFont(STR_BTN_Refresh, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width/2,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
    pSpriteDownBtn->setTag(UIWIFILIST_DOWNBTN);
    pageSprite->pushBack(pSpriteDownBtn);
    m_itemIdx[SCANPG_WIFILIST*100+UIWIFILIST_DOWNBTN]=pageSprite->size()-1;

	//≥sµ≤§Â¶r
    pLabel = Label::createWithSystemFont(STR_LINK_TrouShoot, S_FONE, *pSTR_LINK_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::CENTER);
	iHeight = pLabel->getContentSize().height+pSpriteDownBtn->getContentSize().height;
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(winSize.width/2,iHeight));
	pSpriteBK->addChild(pLabel,10);
    pLabel->setTag(UIWIFILIST_LAB_REPORTLINK);
    pageLabel->pushBack(pLabel);
    m_itemIdx[SCANPG_WIFILIST*100+UIWIFILIST_LAB_REPORTLINK]=pageLabel->size()-1;

	//•[§J§ª≤’ ≥s±µ≤≈∏π
	for(auto i=0;i<MAX_DISPLAY_WIFINAME;i++)
	{
		auto pSprite=Sprite::createWithSpriteFrameName(S_WIFILIST_ICON);
		iHeight = pSprite->getContentSize().height;
		pSprite->setPosition(Vec2(winSize.width-160,winSize.height*3/5-iHeight*2*i));
		pSpriteBK->addChild(pSprite,11);
        pSprite->setTag(UIWIFILIST_WIFILIST+i);
        pageSprite->pushBack(pSprite);
        m_itemIdx[SCANPG_WIFILIST*100+UIWIFILIST_WIFILIST+i]=pageSprite->size()-1;
		pSprite->setVisible(false);
	}
    
	pSpriteBK->setVisible(false);
}
void UI_RobotScanning::Show_WifiList()
{
    auto pSprite = getSpritePoint(SCANPG_WIFILIST,UIWIFILIST_BK);
	pSprite->setVisible(true);
	PageType = SCANPG_WIFILIST;

}
void UI_RobotScanning::Hide_WifiList()
{
    auto pSprite = getSpritePoint(SCANPG_WIFILIST,UIWIFILIST_BK);
	pSprite->setVisible(false);
	WifiList_DrawLineNum=0;
}
//≤æ∞£WIFI List name
void UI_RobotScanning::WifiListLabelRemove()
{
    auto pSpriteBK = getSpritePoint(SCANPG_WIFILIST,UIWIFILIST_BK);
    for(auto pSprite: m_WifiListSSIDLabel)
    {
        pSpriteBK->removeChild(pSprite, true);
    }
    for(auto pSprite: m_WifiListAPLabel)
    {
        pSpriteBK->removeChild(pSprite, true);
    }
    m_WifiListSSIDLabel.clear();
    m_WifiListAPLabel.clear();
}
//¶bWIFI LIST page ¡Ù¬√WIFI
void UI_RobotScanning::WifiListLabelHide()
{
    for(auto pSprite: m_WifiListSSIDLabel)
    {
        pSprite->setVisible(false);
    }
    for(auto pSprite: m_WifiListAPLabel)
    {
        pSprite->setVisible(false);
    }

	for(int i=0;i<MAX_DISPLAY_WIFINAME;i++)
	{		
        auto pSprite = getSpritePoint(SCANPG_WIFILIST,UIWIFILIST_WIFILIST+i);
		pSprite->setVisible(false);
	}
	
}

//¶bWIFI LIST page ≈„•‹WIFI
void UI_RobotScanning::WifiListLabelDisplay(int iStart)
{
	
#if WIFI
    WifiListLabelHide();
	int iWifiNum=m_WifiListSSIDLabel->count();

//	CCSprite* pSpriteBK= (CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_BK);

	//®M©w≈„•‹AP ∞_©l
	if(iStart>iWifiNum-MAX_DISPLAY_WIFINAME)
		iStart=iWifiNum-MAX_DISPLAY_WIFINAME;
	if(iStart<0)
		iStart=0;
	WifiList_StartIdx=iStart; //´¸©w∞_©lAP¶W∫Ÿ
	int icount=0;
    auto Page = m_SpriteVectors.at(SCANPG_WIFILIST);
	while(icount<MAX_DISPLAY_WIFINAME && iStart<iWifiNum)
	{
		auto pLabel=m_WifiListSSIDLabel.at(iStart);
//		bool bShow=pLabel->isVisible();
		int iHeight=pLabel->getContentSize().height;
		pLabel->setPosition(Vec2(winSize.width/2,winSize.height*3/5-iHeight*3*icount));
		pLabel->setVisible(true);

		pLabel=m_WifiListAPLabel.at(iStart);
		pLabel->setPosition(Vec2(winSize.width/2,winSize.height*3/5-iHeight*3*icount+iHeight));
		pLabel->setVisible(true);

		//CCSprite* pSprite=(CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_WIFILIST+icount);
        auto pSprite = Page->at(UIWIFILIST_WIFILIST+icount);
		pSprite->setPosition(Vec2(winSize.width-160+pSprite->getContentSize().width/2 , winSize.height*3/ 5-iHeight*3*icount-15));
		pSprite->setVisible(true);
		icount++;
		iStart++;
	}
	WifiList_DrawLineNum=icount;
	
	//Remove_WifiLabel();
	//iWifiNum=m_WifiListName->count();
	//pSpriteBK= (CCSprite*)m_BtnArray[SCANPG_WIFILIST]->objectAtIndex(UIWIFILIST_BK);
	//for(int i=0;i<iWifiNum;i++)//WIFI ¶W∫Ÿ	
	//{
	//	//¶€∞ ¬_¶Ê ∞œ∞Ï≥]∏m //æa•™πÔªÙ
	//	CCString* pCCstring=(CCString*)m_WifiListName->objectAtIndex(i);
	//	CCLabelTTF* pLabel = CCLabelTTF::create(pCCstring->m_sString.c_str(), S_FONE, *pSTR_WIFILIST_SIZE,CCSizeMake(winSize.width-100,0),kCCTextAlignmentLeft);
	//	pLabel->setColor(COLOR_STRING_BLACK);
	//	int iHeight=pLabel->getContentSize().height;
	//	pLabel->setPosition(ccp(winSize.width/2,winSize.height*3/5-iHeight*3*i));
	//	pLabel->setVisible(true);
	//	pSpriteBK->addChild(pLabel);
	//	m_WifiListLabel->addObject(pLabel);
	//}
#else
    //製作假連結點選
    auto pSpriteBk = getSpritePoint(SCANPG_WIFILIST,UIWIFILIST_BK);
    auto icount =0;
    auto pLabel=Label::createWithSystemFont("Click To", S_FONE, *pSTR_BTN_SIZE);
    int iHeight=pLabel->getContentSize().height;
    pLabel->setPosition(Vec2(winSize.width/2,winSize.height*3/5-iHeight*3*icount));
    pLabel->setVisible(true);
    pLabel->setColor(COLOR_STRING_BLACK);
    pSpriteBk->addChild(pLabel);
    
    pLabel=Label::createWithSystemFont("MainPage", S_FONE, *pSTR_BTN_SIZE);
    pLabel->setPosition(Vec2(winSize.width/2,winSize.height*3/5-iHeight*3*icount+iHeight));
    pLabel->setVisible(true);
    pLabel->setColor(COLOR_STRING_BLACK);
    pSpriteBk->addChild(pLabel);
    
    auto pSprite = getSpritePoint(SCANPG_WIFILIST,UIWIFILIST_WIFILIST+icount);
    pSprite->setPosition(Vec2(winSize.width-160+pSprite->getContentSize().width/2 , winSize.height*3/ 5-iHeight*3*icount-15));
    pSprite->setVisible(true);
    icount++;
    WifiList_DrawLineNum=icount;
#endif
}

//∏¸§J©“¶≥WIFI SSID AP¶W∫Ÿ
void UI_RobotScanning::WifiListLabelLoad()
{
	WifiListLabelRemove();
    auto pSpriteBK=getSpritePoint(SCANPG_WIFILIST,UIWIFILIST_BK);
    auto i=0;
    for(auto pString:m_WifiListSSIDName)
    {
        auto pLabel = Label::createWithSystemFont(pString, S_FONE, *pSTR_WIFILIST_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::LEFT);
        if(iLinkReady[i]==0)
            pLabel->setColor(COLOR_STRING_BLACK);
        else
            pLabel->setColor(COLOR_STRING_GREEN);
        
        auto iHeight=pLabel->getContentSize().height;
        pLabel->setPosition(Vec2(winSize.width/2,winSize.height*3/5-iHeight*3*i));
        pLabel->setVisible(false);
        pSpriteBK->addChild(pLabel);
        m_WifiListSSIDLabel.pushBack(pLabel);
        i++;
        auto pAPString=m_WifiListAPName.at(i);
        pLabel = Label::createWithSystemFont(pAPString, S_FONE, *pSTR_WIFILIST_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::LEFT);
        pLabel->setPosition(Vec2(winSize.width/2,winSize.height*3/5-iHeight*3*i+iHeight));
        pLabel->setVisible(false);
        pSpriteBK->addChild(pLabel);
        m_WifiListAPLabel.pushBack(pLabel);

    }
}
//§¡¥´™¨∫A •[ß÷πœ•‹
void WifiSearchIcon_AddSpeed()
{
	m_pUIRobotSacnning->SearchIconFast();
}
//∑j¥MWIFI§W™∫AP NAME
//¶]¨∞cocos2d ™∫thread ∫ﬁ≤z∞Oæ–≈È¶≥∞›√D ∫…∂q¡◊ßKcreate ®Áº∆®œ•Œ ßÔ•Œnew

void UI_RobotScanning::WifiEnum()
{
	m_SearchType=Search_Loop;
	m_pUIRobotSacnning->SearchIconSlow();
#if WIFI
	GFUN_CreadWifi();	
	int iResult = GFUN_MainSearchNothread(APName,SSIDName,iLinkReady);
	if(iResult ==ERROR_SUCCESS) //∂«øÈ¶®•\
	{
		m_SearchType=Search_APSuccess;
	}
	else //∂«øÈ•¢±—∂i§J
	{
		m_SearchType=Search_APFail;
	}
#else
    m_SearchType=Search_APSuccess;
	//m_WifiScan.MainSearch();
#endif
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
	case UINODEVICE_LAB_REPORTLINK: //∂}±“≥sµ≤
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
	case UIWIFILIST_LAB_REPORTLINK:
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
	case ERRCONNFAIL_LAB_REPORTLINK:
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

//§¡¥´¶C™Ì
//¬IøÔµ¯µ°§W§Ë ©π§W∑j¥M
//¬IøÔµ¯µ°§U§Ë ©π§U∑j¥M
void UI_RobotScanning::WifiLabelUpDown(Vec2 MousePoint)
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
    auto pageSprite = m_SpriteVectors.at(SCANPG_CONNECTANI);
    auto pageLabel = m_LabelVectors.at(SCANPG_CONNECTANI);
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
    

    pSpriteBK->setAnchorPoint(Vec2::ZERO); //≥]©w¡„¬I
	addChild(pSpriteBK); //•[§J layer ºh
    pSpriteBK->setTag(CONNECTANI_BK);
    pageSprite->pushBack(pSpriteBK);
    m_itemIdx[SCANPG_CONNECTANI*100+CONNECTANI_BK]=pageSprite->size()-1;


	//§§∂°πœ•‹ 
	auto pSprite=Sprite::createWithSpriteFrameName(S_STATE_SCANING_ICON); //™¨∫Aπœ•‹
	pSprite->setPosition(Vec2(winSize.width/2,winSize.height*4/5));
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(CONNECTANI_ICON);
    pageSprite->pushBack(pSprite);
    m_itemIdx[SCANPG_CONNECTANI*100+CONNECTANI_ICON]=pageSprite->size()-1;

	//πœ•‹§U§Ë§Â¶r
	auto pLabel = Label::createWithSystemFont(STR_STATE_Connecting, S_FONE, *pSTR_STATE_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*3/5));
	pSpriteBK->addChild(pLabel,10);

	//ª°©˙§Â¶r
	//¶€∞ ¬_¶Ê ∞œ∞Ï≥]∏m //æa•™πÔªÙ
	//AP name
    pLabel = Label::createWithSystemFont("Machine Name", S_FONE, *pSTR_HELP_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::CENTER);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*2/4));
	pSpriteBK->addChild(pLabel,10);
    pLabel->setTag(CONNECTANI_LAB_NAME);
    pageLabel->pushBack(pLabel);
    m_itemIdx[SCANPG_CONNECTANI*100+CONNECTANI_LAB_NAME]=pageLabel->size()-1;

    //SSID anme
	auto iHeight=pLabel->getContentSize().height;
	pLabel = Label::createWithSystemFont("Machine SSID", S_FONE, *pSTR_HELP_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::CENTER);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*2/4-iHeight-pLabel->getContentSize().height*0.5 ));
	pSpriteBK->addChild(pLabel,10);
	//m_BtnArray[SCANPG_CONNECTANI]->insertObject(pLabel,CONNECTANI_SSID);
    pLabel->setTag(CONNECTANI_LAB_SSID);
    pageLabel->pushBack(pLabel);
    m_itemIdx[SCANPG_CONNECTANI*100+CONNECTANI_LAB_SSID]=pageLabel->size()-1;

    pSpriteBK->setVisible(false);
}

void UI_RobotScanning::Show_ConnectAni()
{
    auto pSprite = getSpritePoint(SCANPG_CONNECTANI, UINODEVICE_BK);
	pSprite->setVisible(true);
	m_SearchType=CONNECT_Ready; 
	PageType = SCANPG_CONNECTANI;
}
void UI_RobotScanning::Hide_ConnectAni()
{
    auto pSprite=getSpritePoint(SCANPG_CONNECTANI, UINODEVICE_BK);
	pSprite->setVisible(false);
}

void UI_RobotScanning::ConnectRobotStart(int iBtnNum)
{
    auto pLabel=getLabelPoint(SCANPG_CONNECTANI, CONNECTANI_LAB_SSID);
	auto pCCstring=m_WifiListSSIDName.at(iBtnNum);
	pLabel->setString(pCCstring);

	Hide_AllPage();
	Show_ConnectAni();	
	WifiList_SelAPIdx=iBtnNum;
	ConnectRobotFun();

}
//≥sµ≤ROBOT
void UI_RobotScanning::ConnectRobotFun()
{
#if WIFI
	SetRobotFun_ConnectAp();
#endif
	m_SearchType=CONNECT_WaitIP;
}
//////////////////////////////////////////////////////////////////////////
//≥s±µROBOT •¢±—
void UI_RobotScanning::InitUI_ErrConnFail()
{
    auto pageSprite = m_SpriteVectors.at(SCANPG_ERR_CONNFAIL);
    auto pageLabel = m_LabelVectors.at(SCANPG_ERR_CONNFAIL);
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
    
 
    pSpriteBK->setAnchorPoint(Vec2::ZERO); //≥]©w¡„¬I
	addChild(pSpriteBK); //•[§J layer ºh
    pSpriteBK->setTag(ERRCONNFAIL_BK);
    pageSprite->pushBack(pSpriteBK);
    m_itemIdx[SCANPG_ERR_CONNFAIL*100+ERRCONNFAIL_BK]=pageSprite->size()-1;

	//§§∂°πœ•‹ 
	auto pSprite=Sprite::createWithSpriteFrameName(S_ERROR_CONNFAIL_ICON); //™¨∫Aπœ•‹
	pSprite->setPosition(Vec2(winSize.width/2,winSize.height*4/5));
	pSprite->setScale(2); // ©Ò§j®‚≠ø
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(ERRCONNFAIL_ICON);
    pageSprite->pushBack(pSprite);
    m_itemIdx[SCANPG_ERR_CONNFAIL*100+ERRCONNFAIL_ICON]=pageSprite->size()-1;

	//πœ•‹§U§Ë§Â¶r
	auto pLabel = Label::createWithSystemFont(STR_ERR_ConnFail, S_FONE, *pSTR_STATE_SIZE);
	pLabel->setColor(COLOR_STRING_RED);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*3/5));
	pSpriteBK->addChild(pLabel,10);

	//ª°©˙§Â¶r
	//¶€∞ ¬_¶Ê ∞œ∞Ï≥]∏m //æa•™πÔªÙ
    pLabel = Label::createWithSystemFont(STR_HELP_PlzConf, S_FONE, *pSTR_HELP_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*2/4));
	pSpriteBK->addChild(pLabel,10);
	int iHeight=pLabel->getContentSize().height;

	pLabel = Label::createWithSystemFont(STR_HELP_RobotConn, S_FONE, *pSTR_HELP_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*2/4-iHeight-pLabel->getContentSize().height*0.5 ));
	pSpriteBK->addChild(pLabel,10);
	iHeight += pLabel->getContentSize().height;

	pLabel = Label::createWithSystemFont(STR_HELP_TheMobile, S_FONE, *pSTR_HELP_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::LEFT);

	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*2/4-iHeight-pLabel->getContentSize().height*0.5 ));
	pSpriteBK->addChild(pLabel,10);

	//≥sµ≤§Â¶r
	pLabel = Label::createWithSystemFont(STR_LINK_TrouShoot, S_FONE, *pSTR_LINK_SIZE,Size::Size(winSize.width-100,0),TextHAlignment::CENTER);
	iHeight = pLabel->getContentSize().height;
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(winSize.width/2,winSize.height*1/5-iHeight));
	pSpriteBK->addChild(pLabel,10);
    pLabel->setTag(ERRCONNFAIL_LAB_REPORTLINK);
    pageLabel->pushBack(pLabel);
    m_itemIdx[SCANPG_ERR_CONNFAIL*100+ERRCONNFAIL_LAB_REPORTLINK]=pageLabel->size()-1;


	//©≥≥°´ˆ∂s
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //≥]©w¡„¬I
	pSpriteDownBtn->setPosition(Vec2::ZERO);
	pSpriteDownBtn->setScaleX(0.5);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);

	pLabel = Label::createWithSystemFont(STR_BTN_Canel, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setScaleX(2);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
    pSpriteDownBtn->setTag(ERRCONNFAIL_CANCEL);
    pageSprite->pushBack(pSpriteDownBtn);
    m_itemIdx[SCANPG_ERR_CONNFAIL*100+ERRCONNFAIL_CANCEL]=pageSprite->size()-1;

	pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //≥]©w¡„¬I
	pSpriteDownBtn->setPosition(Vec2(0+winSize.width*0.5+4,0)); //+4 FOR §§∂°Ωu
	pSpriteDownBtn->setScaleX(0.5);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = Label::createWithSystemFont(STR_BTN_Retry, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setScaleX(2);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
    pSpriteDownBtn->setTag(ERRCONNFAIL_RETRY);
    pageSprite->pushBack(pSpriteDownBtn);
    m_itemIdx[SCANPG_ERR_CONNFAIL*100+ERRCONNFAIL_RETRY]=pageSprite->size()-1;


	pSpriteBK->setVisible(false);

}
void UI_RobotScanning::Show_ErrConnFail()
{

    auto pSprite=getSpritePoint(SCANPG_ERR_CONNFAIL, ERRCONNFAIL_BK);
	pSprite->setVisible(true);
	PageType = SCANPG_ERR_CONNFAIL;
}
void UI_RobotScanning::Hide_ErrConnFail()
{

    auto pSprite=getSpritePoint(SCANPG_ERR_CONNFAIL, ERRCONNFAIL_BK);
    pSprite->setVisible(false);
}
/////////////////////////////////
//觸碰設定
void UI_RobotScanning::setTouchEnabled(bool enable)
{
    if (enable == touchEnabled)
    {
        return;
    }
    touchEnabled = enable;
    
    if(touchEnabled)
    {
        touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true); //設置是否想下傳遞觸摸
        CC_SAFE_RETAIN(touchListener);
        
        touchListener->onTouchBegan = CC_CALLBACK_2(UI_RobotScanning::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(UI_RobotScanning::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(UI_RobotScanning::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
    }
    else
    {
        _eventDispatcher->removeEventListener(touchListener);
        CC_SAFE_RELEASE_NULL(touchListener);
    }
}
bool UI_RobotScanning::isTouchEnabled() const
{
    return touchEnabled;
}

//////////////////////////////////////////////////////////////////////////
//Preass even program
//∑∆π´¬I§U!ßP¬_¨O®∫≠”§∏•Û
bool UI_RobotScanning::onTouchBegan(Touch* touch, Event* event)
{
    auto target = static_cast<Layer*>(event->getCurrentTarget());
    if(target==nullptr)
    {
        return true;
    }
    log("UI_RobotScanning:%s",__func__);
	Vec2 touchLocation = touch->getLocation();
	Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
	if(!isVisible()) //¶≥≈„•‹§~ƒ~ƒÚ
		return false;
	switch(PageType)
	{
	case 	SCANPG_SEARCH:
		{
			if(IsRobotThreadIdle()) // ¡◊ßK¶b∑j¥M§§ §S¬IøÔµe≠±
			{
				CallThread_SearchRobot();
			}
		}
		break;
	case	SCANPG_ERR_NODEVICE:
		{
	        //取得現在點擊座標
            auto sprites = m_SpriteVectors.at(SCANPG_ERR_NODEVICE);
            for(auto pSprite:*sprites)
            {
                auto iIdx=pSprite->getTag();
                if(pSprite->isVisible() && iIdx>=UINODEVICE_TOPBTN)
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    auto bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        Function_ErrNoDevice(iIdx);
                        break;
                    }
                }

            }
            
		}
		break;
	case SCANPG_WIFILIST:
		{
            auto sprites = m_SpriteVectors.at(SCANPG_WIFILIST);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            {
                auto iIdx=pSprite->getTag();
                if(pSprite->isVisible() && iIdx>=UIWIFILIST_TOPBTN)
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
#if ROBOT
                        Function_WifiList(iIdx);
#else
                        m_SearchType=CONNECT_Success; //±j®Ó≥sΩu∂∂ßQ
                        ChangLayer_Main();
                        
#endif
                        break;
                    }
                }

            }

			//¶p™G¨O¬IøÔÆ‡≠± §¡¥´List
			WifiLabelUpDown(touchLocation);
		}
		break;
	case SCANPG_ERR_CONNFAIL:
		//ßP¬_®∫§@≠”§∏•Û
        {
            auto sprites = m_LabelVectors.at(SCANPG_ERR_CONNFAIL);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            //for (int i = ERRCONNFAIL_LAB_REPORTLINK; i < sprites->size(); i++)
            {
                auto iIdx=pSprite->getTag();
                if(pSprite->isVisible() && iIdx>=ERRCONNFAIL_LAB_REPORTLINK)
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        Function_ErrCopnnFail(iIdx);
                        break;
                    }
                    
                }

            }
        }
		break;
	default:
		break;
	}

	return true;
}

void UI_RobotScanning::onTouchMoved(Touch* touch, Event* event)
{
	Vec2 touchLocation = touch->getLocation();
	Vec2 nodePosition = convertToNodeSpace( touchLocation );
}

void UI_RobotScanning::onTouchEnded(Touch* touch, Event* event)
{
	Vec2 touchLocation = touch->getLocation();
	Vec2 nodePosition = convertToNodeSpace( touchLocation );
}
