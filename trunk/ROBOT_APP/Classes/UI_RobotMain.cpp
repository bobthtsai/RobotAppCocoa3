#include <ImageList.h>

#include <HelloWorldScene.h>
#include <libplayerc++/playerc++.h>
#include <pthread.h>
#include <semaphore.h>
#include <MAP>
#include "player.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
#include <boost/format.hpp>
#endif
//PLAYER_DISPATCH_UI_NUMBER 
//PLAYER_DISPATCH_PM_BUMBER
//PLAYER_DISPATCH_PM_POWER_NUMBER
//以上三組定義是手動加到player_insterfaces.h
//#ifndef PLAYER_DISPATCH_UI_NUMBER
//#error AI STATE OVER
//#endif
#define PLAYER_DISPATCH_PM_BUMBER 12 //PLAYER_DISPATCH_PM_CARPET_CLEAN +1 

#ifndef PLAYER_DISPATCH_PM_BUMBER
#error UI STATE OVER
#endif

#define PLAYER_DISPATCH_PM_POWER_NUMBER 5
#ifndef PLAYER_DISPATCH_PM_POWER_NUMBER 
#error POWER STATE OVER
#endif
static const char UI_STATE_STR[PLAYER_DISPATCH_PM_BUMBER][20]={
	"NONE",
	"OK",
	"BACK",
	"AUTO",          
	"TURBO",           
	"NORMAL",          
	"WALK",            
	"TURBO_AIR_CLEAN", 
	"NORMAL_AIR_CLEAN",
	"STANDBY",         
	"DEEP_CLEAN",      
	"CARPET_CLEAN",    	
};

static char POWER_STATE_STR[PLAYER_DISPATCH_PM_POWER_NUMBER][20]={
	"VERY_LOW_POWER",
	"SLEEP",         
	"ON_CHARGER",    
	"CHARGE",        
	"CHARGE_FULL",   
};


USING_NS_CC;
using namespace std;
using namespace PlayerCc;
extern HelloWorld* m_MainView; 
/// Player Client
extern PlayerClient *pPlayerclient;
extern PowerProxy	 *pPowerProxy;
extern DispatcherProxy *pDisProxy;
extern UI_RobotMain* m_pUIRobotMain;
extern UI_RobotScanning* m_pUIRobotSacnning;
extern UI_RobotRemote* m_UIRobotRemote;

player_dispatcher_time_t lastRobotTime;
player_dispatcher_timezone_t lastRobotZone;

static CCPoint SelBKSite[MSTAET_BTNSize];

sem_t semRobMapRelease;	//地圖更新信號
static bool bRobotMapReady=false;
//static bool bRobotMapUpdate=false;
sem_t semRobMapUpdateOver;	//地圖傳輸完畢
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
//建立執行緒 底層不斷更新地圖(每分鐘一次)
static pthread_t s_RobotMAPThread; //連接ROBOT thread
static int m_RobotMapStat=ThreadFun_NONE;
//static int m_RobotFunStateByThreadNEXT=ThreadFun_NONE;

//嘗試連接ROBOT
static void* RobotMapReleaseThrFun(void* data)
{
	
	int iResult=0;	
	sem_wait(&semRobMapRelease);
	bRobotMapReady=false;
	//bRobotMapUpdate=false;
	bool bState=GuiData_FirstMapRelease();	
	int iErrorTime=0;	
	if(!bState)
	{ // 建立地圖失敗
		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
			std::string sValue=boost::str(boost::format( "%s" ) % STR_ERR_ConnFail_MAPINIT_ENG );
			MessageBoxA(NULL,(LPCSTR)sValue.c_str(),"ERROR",MB_OK+MB_SYSTEMMODAL);
			
#endif

	}
	RobotMapUpdate(); //第一次強制跟新
	while (true)
	{
		// create autorelease pool for iOS
		CCThread thread;
		thread.createAutoreleasePool();		
		sem_wait(&semRobMapRelease);
		//sem_trywait(&semRobMapUpdateOver);	//清除之前更新訊號
		bState=GuiData_MapRelease();				//地圖更新		
		if(bState)
		{
			bRobotMapReady=true;				//只要拿過一次就設為TRUE
			GuiData_MapReleaseShowData();
			sem_post(&semRobMapUpdateOver);		//告知已經更新完畢
			
			//while(sem_trywait(&semRobMapRelease)); //清空更新訊號
			iErrorTime=0;
		}
		else
		{
			iErrorTime++;
			sem_post(&semRobMapRelease);
		}
		if(iErrorTime>=30) //超過10次連線失敗 顯示提示視窗  中止程式
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
			std::string sValue=boost::str(boost::format( "%s" ) % STR_ERR_ConnFail_MAPUPDATE_CHT );
			MessageBoxA(NULL,(LPCSTR)sValue.c_str(),"ERROR",MB_OK+MB_SYSTEMMODAL);

#endif
			break;
		}
	}
	return 0;
}

#endif
void WaitRobotMapUpdate()
{
	sem_wait(&semRobMapUpdateOver);	//清除之前更新訊號
}
int WaittryRobotMapUpdate()
{
	return sem_trywait(&semRobMapUpdateOver);	//清除之前更新訊號
}
void RobotMapUpdate()
{
	sem_post(&semRobMapRelease);
}
bool IsRobotMapReady()
{
	return bRobotMapReady;
}
void RobotMapThread_Init()
{
	sem_init(&semRobMapRelease,0,0);
	sem_init(&semRobMapUpdateOver,0,0);
	
}

UI_RobotMain::UI_RobotMain()
{
	PageType=MAINPG_LAYER;
	fPowerPart=-100;
	bShowMSG_TimeSync=false;
	RobotNameClean();
	memset(RobotFlag,0,sizeof(RobotFlag));
	memset(FailCount,0,sizeof(FailCount));	
	memset(&stRobotState,0,sizeof(stRobotState));	
	memset(&stOldRobotState,0,sizeof(stOldRobotState));	
	bTimeSyncEven=false;
}
UI_RobotMain::~UI_RobotMain()
{
	for(int i=0;i<MAINPG_SIZE;i++)
		m_BtnArray[i]->release();

}
bool UI_RobotMain::init()
{  
	if (!CCLayer::init()) 
	{  
		return false;  
	}  
	InitUI();  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	//建立更新地圖thread
	RobotMapThread_Init();
	
	pthread_create(&s_RobotMAPThread, NULL, RobotMapReleaseThrFun, NULL);
#endif
	return true;  
}  
void UI_RobotMain::Function_RobotInfoAllright(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case MINFORI_TITAL:
		Hide_AllPage();
		Show_MainPage();
		break;
	}

}
void UI_RobotMain::Function_RobotInfoWarning(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case MINFOWA_TITAL:
		Hide_AllPage();
		Show_MainPage();
		break;
	}

}
void UI_RobotMain::Function_RobotMain(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case MSTAET_LABALARM:
		Hide_AllPage();
		Show_InfoAllRight();
		break;
	case MSTAET_ICONALARM: //日後改為判斷是否有錯誤
		Hide_AllPage();
		Show_InfoWarning();
		break;
	case MSTAET_LABNAME:	//點選名稱 從ROBOT 更新名稱
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
		RobotNameClean();
		CallThread_GetRobotName();
		
#endif
		break;
	case MSTAET_LABDATE:	//點選時間 從ROBOT 更新時間
	case MSTAET_LABTIME:
		RobotTimeClean();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
		CallThread_GetRobotTime();		
#endif
		break;
	case MSTAET_BTNRESERVATION:
		ChangLayer_Reservation();
		break;
	case MSTAET_BTNJOB:
		ChangLayer_CleanJob();
		break;
	case MSTAET_BTNREMOTE:
		ChangLayer_Remote();
		break;
	case MSTAET_BTNMAP:
		ChangLayer_Map();
		break;
	case MSTAET_BTNSETUP:
		ChangLayer_RobotSetup();
		break;
	}
		
}
//////////////////////////////////////////////////////////////////////////
// 取得ROBOT 時間
void UI_RobotMain::ReleaseLab_RobotDateTime()
{
	//畫面設定
	int iResult=RelaseRobotTime((void* )&lastRobotTime);
	switch(iResult)
	{
		case  2:
			ReleaseLab_RobotDate();	
		case  1:
			ReleaseLab_RobotTime();
			break;
		default:
			break;
	}

}
//void UI_RobotMain::ReleaselastRobotTime()
//{
//	player_dispatcher_time_t *pDate=NULL;
//	pDate=(player_dispatcher_time_t *)GetRobotTime();
//	lastRobotTime=*pDate;	
//}
void UI_RobotMain::ReleaseLab_UpdateDateTime(void *pData)	
{
	player_dispatcher_time_t *pDate=(player_dispatcher_time_t *)pData;
	CCLabelTTF* pLabel =(CCLabelTTF*)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABDATE);		
	char sValue[24];
	sprintf(sValue,STR_RobotMain_Date,pDate->tm_year,pDate->tm_mon,pDate->tm_mday);
	pLabel->setString(sValue);
	pLabel =(CCLabelTTF*)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABTIME);		
	sprintf(sValue,STR_RobotMain_Time,pDate->tm_hour,pDate->tm_min);
	pLabel->setString(sValue);

}
void UI_RobotMain::ReleaseLab_RobotDate()
{
	CCLabelTTF* pLabel =(CCLabelTTF*)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABDATE);		
	char sValue[24];
	sprintf(sValue,STR_RobotMain_Date,lastRobotTime.tm_year,lastRobotTime.tm_mon,lastRobotTime.tm_mday);
	pLabel->setString(sValue);

}
void UI_RobotMain::ReleaseLab_RobotTime()
{
	CCLabelTTF* pLabel =(CCLabelTTF*)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABTIME);		
	char sValue[24];
	sprintf(sValue,STR_RobotMain_Time,lastRobotTime.tm_hour,lastRobotTime.tm_min);
	pLabel->setString(sValue);
	RobotFlag[RobFlag_Time]=RobFlagSt_ReleaseOver;

}
void UI_RobotMain::ReleaseLab_RobotState()
{
	CCLabelTTF* pLabel =(CCLabelTTF*)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABALARM);			
	if(stOldRobotState.CleanStatus != stRobotState.CleanStatus && stRobotState.CleanStatus<PLAYER_DISPATCH_PM_BUMBER && stRobotState.CleanStatus>=0)
	{
		char sValue[24];
		sprintf(sValue,"%s",UI_STATE_STR[stRobotState.CleanStatus]);
		pLabel->setString(sValue);
		stOldRobotState.CleanStatus = stRobotState.CleanStatus;
	}	
	
	RobotFlag[RobFlag_State]=RobFlagSt_ReleaseOver;
}
void UI_RobotMain::ReleaseIcon_RobotState()
{
	CCSprite* pSprite=(CCSprite*)m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_ICONALARM); //警告圖示	
	if(stOldRobotState.Error != stRobotState.Error && stRobotState.Error!=0)
	{

		CCSpriteFrame *pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_MAIN_ALARM_ERROR_ICON);
		pSprite->setDisplayFrame(pFrame);
		pSprite->setVisible(true);
		stOldRobotState.Error = stRobotState.Error;
	}	
	else if(stOldRobotState.Worry != stRobotState.Worry  && stRobotState.Worry !=0)
	{
		CCSpriteFrame *pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_MAIN_ALARM_WARN_ICON);
		pSprite->setDisplayFrame(pFrame);
		pSprite->setVisible(true);
		stOldRobotState.Worry = stRobotState.Worry;
	}
	else if(pSprite->isVisible() && stRobotState.Error==0 && stRobotState.Worry==0 )
	{
		pSprite->setVisible(false);		
	}	
}
void UI_RobotMain::ReleaseLab_RobotName()
{		
	CCLabelTTF* pLabel =(CCLabelTTF*)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABNAME);	
	pLabel->setString(sRobotName.c_str());		
	RobotFlag[RobFlag_Name]=RobFlagSt_ReleaseOver;

}
void UI_RobotMain::ReleaseLab_RobotPower()
{
	CCLabelTTF* pLabel =(CCLabelTTF*)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABPOWER);		
	char sNump[12];
	sprintf(sNump,"%.1f%%",fPowerPart);
	pLabel->setString(sNump);
	RobotFlag[RobFlag_Power]=RobFlagSt_ReleaseOver;
}
//檢查現在名稱狀態!決定是否要傳送指令

// 回傳true 表示已經在執行, false 表示忙碌不執行
bool UI_RobotMain::CheckRobotName()
{
	if(RobotFlag[RobFlag_Name]==RobFlagSt_None) //沒有機台名稱
	{			
		if(CallThread_GetRobotName())
		{
			RobotFlag[RobFlag_Name]=RobFlagSt_Wait;
			return true;
		}
	}
	return false;
}
//檢查現在時間狀態!決定是否要傳送指令
// 回傳true 表示已經在執行, false 表示忙碌不執行
bool UI_RobotMain::CheckRobotTime()
{
	//if(RobotFlag[RobFlag_Time]!=RobFlagSt_Error) //可以順利抓取時間
	if(RobotFlag[RobFlag_Time]==RobFlagSt_None) //沒有機台時間
	{
		if(CallThread_GetRobotTime())
		{
			RobotFlag[RobFlag_Time]=RobFlagSt_Wait;
			return true;		
		}		
	}
	return false;
}
bool UI_RobotMain::CheckRobotState()
{
	//if(RobotFlag[RobFlag_State]!=RobFlagSt_Error) //可以順利抓取狀態
	if(RobotFlag[RobFlag_State]==RobFlagSt_None) //沒有機台狀態
	{
		if(CallThread_GetRobotState())
		{
			RobotFlag[RobFlag_State]=RobFlagSt_Wait;
			return true;		
		}		
	}
	return false;
}
bool UI_RobotMain::CheckRobotPower()
{
	if(RobotFlag[RobFlag_Power]!=RobFlagSt_Error) //可以順利抓取電量
	{
		if(CallThread_GetRobotPower())
		{
			RobotFlag[RobFlag_Power]=RobFlagSt_Wait;
			return true;		
		}		
	}
	return false;
}

bool UI_RobotMain::CheckRobotSchedule()
{
	if(RobotFlag[RobFlag_Schedule]!=RobFlagSt_Error) //可以順利抓取時間
	{
		if(CallThread_RobotSchedule())
		{
			RobotFlag[RobFlag_Schedule]=RobFlagSt_Wait;
			return true;		
		}		
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
//每10秒一次 查詢ROBOT 上面時間 跟狀態
static int iCommandLoopCount=0;
void UI_RobotMain::ReleseRobotState(float dt)
{
	//float fpower=fPowerPart;
	if(pPowerProxy!=NULL)
	{
		RobotPowerRetry();

		//if(pPowerProxy->IsValid()) //確認資料有效
		//{
			//fPowerPart=pPowerProxy->GetCharge(); //0 表示異常 Not in chargin
			//fPowerPart=pPowerProxy->GetPercent(); //0 表示異常 Not in chargin			
			//if(fabs(fpower-fPowerPart)>=0.5f) //數值有變化
			//{ //更新畫面
			//	CCLabelTTF* pLabel =(CCLabelTTF*)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABPOWER);		
			//	char sNump[12];
			//	sprintf(sNump,"%.3f%%",fPowerPart);
			//	pLabel->setString(sNump);
			//	fpower=fPowerPart;
			//}
		//}
		//else
		//{
		//	if(fPowerPart!=0)
		//	{
		//		CCLabelTTF* pLabel =(CCLabelTTF*)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABPOWER);		
		//		pLabel->setString("Invalid");
		//		//SetRobotTimeFailToGet(); //資料失效 會順便關閉時間更新功能 
		//	}
		//	fPowerPart=0;
		//}
		
	}

	if(pDisProxy!=NULL)
	{
		//RobotTimeRetry(); 
		RobotTimeClean(); 
		RobotStateRetry();
		//CheckRobtoTimeSync();
		//iCommandLoopCount++;
		//bool bRun=CheckRobotName();

		//if(!bRun)
		//{
		//	if(iCommandLoopCount%2)
		//		CheckRobotTime();		
		//	else
		//		CheckRobotState();
		//}
	}
	

}
//默認的定時器 每秒60次 每個FPS 一次
static int RobotMainUpdateTime=0;
void UI_RobotMain::update(float dt)
{
	if(RobotMainUpdateTime%60==0) //更新狀態 OR 時間
	{ //每一秒鐘
		if(RobotFlag[RobFlag_Time]==RobFlagSt_Get) //可以順利抓取時間
		{
			//將ROBOT 現在時間存到現在系統
			//ReleaselastRobotTime();
			ReleaseLab_RobotDateTime(); //更新時間
		}
		else //根據系統時間更新
		{

		}

		if(RobotFlag[RobFlag_Name]==RobFlagSt_Get) //更新名稱
			ReleaseLab_RobotName();

		if(RobotFlag[RobFlag_State]==RobFlagSt_Get) //更新狀態
		{
			ReleaseLab_RobotState();
			ReleaseIcon_RobotState();
		}
		if(RobotFlag[RobFlag_Power]==RobFlagSt_Get) //更新電量
		{
			ReleaseLab_RobotPower();
		}
		if(!bTimeSyncEven)
		{
			CheckRobtoTimeSync();
		}
	}
	if(RobotFlag[RobFlag_All]==RobFlagSt_None && (RobotMainUpdateTime%5 ==0)) //每5 FPS 傳送一次
	{
		for(int i=0;i<RobFlag_Number;i++)
		{
			if(RobotFlag[i]==RobFlagSt_None )
			{ //一次只有送達一個
				if(i==RobFlag_All)
				{ //送過一輪後 檢查是否全部送出
					int j=0;
					for(j=0;j<RobFlag_All;j++)
					{
						if(RobotFlag[j]==RobFlagSt_Wait)
							break;
					}
					if(j==RobFlag_All) //表示全部都已經送過一輪了
						RobotFlag[RobFlag_All]=RobFlagSt_Get;

				}
				else
				{
					UpdateRobotByReleaseCommad(i);
				}
				
				break;
			}
		}
	}
	RobotMainUpdateTime++;

}
//更新ROBOT 指令
void UI_RobotMain::UpdateRobotByReleaseCommad(int iCmdId)
{
	switch(iCmdId)
	{
	case RobFlag_Name:
		CheckRobotName();
		break;
	case RobFlag_Time:
		CheckRobotTime();
		break;
	case RobFlag_State:
		CheckRobotState();
	case RobFlag_Power:
		CheckRobotPower();
	case RobFlag_Schedule:
		CheckRobotSchedule();
	default:
		break;
	}
}
void UI_RobotMain::registerWithTouchDispatcher()
{
	// higher priority than dragging
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
//按鈕底色位置
void UI_RobotMain::PressSpriteMove(int iBtnIdx)
{
	CCSprite* pSprite =(CCSprite*)m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_BTNSELBK);  //原因待查
	if(iBtnIdx<0 || iBtnIdx>=MSTAET_BTNSize)
	{
		pSprite ->setVisible(false);
	}
	else
	{
		pSprite ->setVisible(true);
		pSprite ->setPosition(SelBKSite[iBtnIdx]);		
	}
	


}
//滑鼠點下!判斷是那個元件
bool UI_RobotMain::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );

	switch(PageType)
	{
	default:
		break;
	case MAINPG_STATE:
		{
			//判斷那一個元件
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch)
				{
					PressSpriteMove(i-MSTAET_BTNRESERVATION);
					break;
				}
			}  
		}
		break;

	}
	if(isVisible()) //有顯示才繼續
		return true;
	else
		return false;
}

void UI_RobotMain::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
}

void UI_RobotMain::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	switch(PageType)
	{
	default:
		break;
	case MAINPG_INFOWARNING:
		{
			//判斷那一個元件
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch)
				{
					Function_RobotInfoWarning(i);
					break;
				}
			}  
		}
		break;
	case MAINPG_INFOAllRIGHT:
		{
			//判斷那一個元件
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch)
				{
					Function_RobotInfoAllright(i);
					break;
				}
			}  
		}
		break;		
	case MAINPG_STATE:
		{
			//判斷那一個元件
			for (int i = MSTAET_ICON; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				if(pSprite->isVisible())
				{
					bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
					if(bTouch)
					{
						PressSpriteMove(i-MSTAET_BTNRESERVATION);
						Function_RobotMain(i);
						break;
					}
				}
			}  
		}
		break;
	

	}
}
void UI_RobotMain::InitUI()
{
	winSize = CCDirector::sharedDirector()->getVisibleSize(); //取得視窗大小
	InitUI_MainPage();
	InitUI_RobotInfoAllRight();
	InitUI_RobotInfoWarning();
	
}
void UI_RobotMain::InitUI_MainPage()
{
	m_BtnArray[MAINPG_STATE] = CCArray::createWithCapacity(MSTAET_SIZE); //按鈕存放
	m_BtnArray[MAINPG_STATE]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint IconSite_Robot=CCPointMake(565*W_unit,winSize.height-210*H_unit);	
	CCPoint LabeSite_Welcome=CCPointMake(77*W_unit,winSize.height-375*H_unit);
	CCPoint LabeSite_RobotName=CCPointMake(77*W_unit,winSize.height-485*H_unit);
	CCPoint LabeSite_Date=CCPointMake(77*W_unit,winSize.height-600*H_unit);
	CCPoint LabeSite_Time=CCPointMake(77*W_unit,winSize.height-667*H_unit);
	CCPoint LabeSite_Status=CCPointMake(595*W_unit,winSize.height-692*H_unit);
	CCPoint LabeSite_StatusValue=CCPointMake(595*W_unit,winSize.height-760*H_unit);
	CCPoint IconSite_Alarm=CCPointMake(735*W_unit,winSize.height-695*H_unit);
	CCPoint LabeSite_Power=CCPointMake(837*W_unit,winSize.height-692*H_unit);
	CCPoint LabeSite_PowerValue=CCPointMake(837*W_unit,winSize.height-760*H_unit);
	CCPoint SpriSite_MainBK=CCPointMake(40*W_unit,winSize.height-963*H_unit);
	
	CCPoint SpriSite_Schedule=CCPointMake(40*W_unit,winSize.height-963*H_unit);
	CCPoint SpriSite_Job=CCPointMake(540*W_unit,winSize.height-963*H_unit);
	CCPoint SpriSite_Remote=CCPointMake(40*W_unit,winSize.height-1319*H_unit);
	CCPoint SpriSite_Map=CCPointMake(540*W_unit,winSize.height-1319*H_unit);

	CCPoint IconSite_Schedule=CCPointMake(180*W_unit,winSize.height-985*H_unit);
	CCPoint LabeSite_Schedule=CCPointMake(281*W_unit,winSize.height-1173*H_unit);	
	CCPoint IconSite_Job=CCPointMake(680*W_unit,winSize.height-985*H_unit);
	CCPoint LabeSite_Job=CCPointMake(791*W_unit,winSize.height-1173*H_unit);	
	CCPoint IconSite_Remote=CCPointMake(180*W_unit,winSize.height-1341*H_unit);
	CCPoint LabeSite_Remote=CCPointMake(281*W_unit,winSize.height-1529*H_unit);	
	CCPoint IconSite_Map=CCPointMake(680*W_unit,winSize.height-1341*H_unit);
	CCPoint LabeSite_Map=CCPointMake(791*W_unit,winSize.height-1529*H_unit);

	SelBKSite[MSTAET_BTN1]=SpriSite_Schedule;
	SelBKSite[MSTAET_BTN2]=SpriSite_Job;
	SelBKSite[MSTAET_BTN3]=SpriSite_Remote;
	SelBKSite[MSTAET_BTN4]=SpriSite_Map;
	//設定Scanning 背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層
	m_BtnArray[MAINPG_STATE]->insertObject(pSpriteBK,MSTAET_BK);
	//m_BtnArray[MAINPG_STATE]->addObject(pSpriteBK);
	//ROBOT圖示 
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_MAIN_ROBOTICON); //狀態圖示
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_Robot);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[MAINPG_STATE]->insertObject(pSprite,MSTAET_ICON);
	//文字
		//welcome
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_STATE_Welcome, S_FONE, *pSTR_STATE_SIZE,CCSizeMake(winSize.width-200*W_unit,0),kCCTextAlignmentLeft);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_Welcome);
	pSpriteBK->addChild(pLabel,10);

		//robot name
	sRobotName="Name";
	pLabel = CCLabelTTF::create(sRobotName.c_str(), S_FONE, *pSTR_BTN_SIZE,CCSizeMake(winSize.width-200*W_unit,0),kCCTextAlignmentLeft);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_RobotName);
	m_BtnArray[MAINPG_STATE]->insertObject(pLabel,MSTAET_LABNAME);
	pSpriteBK->addChild(pLabel,10);
	

		//date //用ROBOT 的下源當作起點
	pLabel = CCLabelTTF::create("2015/01/01", S_FONE, *pSTR_DATE_SIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Date);
	m_BtnArray[MAINPG_STATE]->insertObject(pLabel,MSTAET_LABDATE);
	pSpriteBK->addChild(pLabel,10);
	
		//TIME
	pLabel = CCLabelTTF::create("15:30", S_FONE, *pSTR_TIME_SIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Time);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[MAINPG_STATE]->insertObject(pLabel,MSTAET_LABTIME);
		//status lab
	pLabel = CCLabelTTF::create(STR_RobotMain_Status, S_FONE, *pSTR_ROBOTMAIN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_LIGHTBLACK);
	pLabel->setPosition(LabeSite_Status);
	pSpriteBK->addChild(pLabel,10);
	//pLabel = CCLabelTTF::create(STR_RobotMain_standby, S_FONE, *pSTR_ROBOTMAIN_LABSIZE); //機台狀態
	pLabel = CCLabelTTF::create(UI_STATE_STR[0], S_FONE, *pSTR_ROBOTMAIN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_StatusValue);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[MAINPG_STATE]->insertObject(pLabel,MSTAET_LABALARM);

	pSprite=CCSprite::createWithSpriteFrameName(S_MAIN_ALARM_ERROR_ICON); //警告圖示
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_Alarm);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[MAINPG_STATE]->insertObject(pSprite,MSTAET_ICONALARM);

		//power lab
	pLabel = CCLabelTTF::create(STR_RobotMain_Power, S_FONE, *pSTR_ROBOTMAIN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_LIGHTBLACK);
	pLabel->setPosition(LabeSite_Power);
	pSpriteBK->addChild(pLabel,10);	
	pLabel = CCLabelTTF::create("80%", S_FONE, *pSTR_ROBOTMAIN_LABSIZE); //機台電量
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_PowerValue);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[MAINPG_STATE]->insertObject(pLabel,MSTAET_LABPOWER);

	//按鈕
	CCSprite *pSpriteBtn=CCSprite::createWithSpriteFrameName(S_MAIN_BTNBK); //十字背景
	pSpriteBtn->setAnchorPoint(CCPointMake(0,1));
	pSpriteBtn->setPosition(SpriSite_MainBK);
	pSpriteBK->addChild(pSpriteBtn,10);

		//scheduled按鈕
	//pSpriteBtn=CCSprite::createWithSpriteFrameName(S_MAIN_BTNBKONE); //按鈕背景
	//pSpriteBtn->setAnchorPoint(CCPointMake(0,1));
	//iHeight=pSpriteBtn->getContentSize().height;
	//iWidth=pSpriteBtn->getContentSize().width;
	//pSpriteBtn->setPosition(SpriSite_Schedule);
	//pSpriteBK->addChild(pSpriteBtn,11);

	pSprite=CCSprite::createWithSpriteFrameName(S_MAIN_BTNSCHEDULED); //按鈕圖示
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_Schedule);
	pSpriteBK->addChild(pSprite,12);								 
	m_BtnArray[MAINPG_STATE]->insertObject(pSprite,MSTAET_BTNRESERVATION);

	pLabel = CCLabelTTF::create(STR_BTN_Reservation, S_FONE, *pSTR_BTN_SIZE); //按鈕文字
	//pLabel ->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);						
	pLabel->setPosition(LabeSite_Schedule);
	pSpriteBK->addChild(pLabel,12);

		//JOB按鈕
	//pSpriteBtn=CCSprite::createWithSpriteFrameName(S_MAIN_BTNBKONE); 
	//pSpriteBtn->setAnchorPoint(CCPointMake(0,1));
	//pSpriteBtn->setPosition(SpriSite_Job);
	//pSpriteBK->addChild(pSpriteBtn,11);
	pSprite=CCSprite::createWithSpriteFrameName(S_MAIN_BTNJOB); //按鈕圖示
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_Job);	
	pSpriteBK->addChild(pSprite,11);	
	m_BtnArray[MAINPG_STATE]->insertObject(pSprite,MSTAET_BTNJOB);
	pLabel = CCLabelTTF::create(STR_BTN_CleanJob, S_FONE, *pSTR_BTN_SIZE);  //按鈕文字	
	//pLabel ->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Job);
	pSpriteBK->addChild(pLabel,10);
	
	//REMOTE按鈕
	//pSpriteBtn=CCSprite::createWithSpriteFrameName(S_MAIN_BTNBKONE); //按鈕背景
	//pSpriteBtn->setAnchorPoint(CCPointMake(0,1));
	//pSpriteBtn->setPosition(SpriSite_Remote);
	//pSpriteBK->addChild(pSpriteBtn,11);
	
	pSprite=CCSprite::createWithSpriteFrameName(S_MAIN_BTNREMOTE); //按鈕圖示
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_Remote);
	pSpriteBK->addChild(pSprite,11);
	m_BtnArray[MAINPG_STATE]->insertObject(pSprite,MSTAET_BTNREMOTE);
	pLabel = CCLabelTTF::create(STR_BTN_RemoteControl, S_FONE, *pSTR_BTN_SIZE); //按鈕文字
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Remote);
	pSpriteBK->addChild(pLabel,10);

	//MAP按鈕
	pSprite=CCSprite::createWithSpriteFrameName(S_MAIN_BTNMAP); //按鈕圖示
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_Map);
	pSpriteBK->addChild(pSprite,11);
	m_BtnArray[MAINPG_STATE]->insertObject(pSprite,MSTAET_BTNMAP);
	pLabel = CCLabelTTF::create(STR_BTN_Map, S_FONE, *pSTR_BTN_SIZE); //按鈕文字
	//pLabel ->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Map);
	pSpriteBK->addChild(pLabel,10);

	//點選提示背景
	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_MAIN_BTNBKONE); 
	pSpriteBtn->setAnchorPoint(CCPointMake(0,1));
	pSpriteBtn->setPosition(SpriSite_Map);
	pSpriteBK->addChild(pSpriteBtn,11);	
	pSpriteBtn->setVisible(false);
	m_BtnArray[MAINPG_STATE]->insertObject(pSpriteBtn,MSTAET_BTNSELBK);
	


	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
	pSprite=CCSprite::createWithSpriteFrameName(S_MAIN_BTNSETUP); //按鈕圖示
	pSprite->setPosition(ccp(pSpriteBK->getContentSize().width*0.5,pSprite->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pSprite,10);
	m_BtnArray[MAINPG_STATE]->insertObject(pSpriteDownBtn,MSTAET_BTNSETUP);


	pSpriteBK->setVisible(false);
}

void UI_RobotMain::InitUI_RobotInfoAllRight()
{
	m_BtnArray[MAINPG_INFOAllRIGHT] = CCArray::createWithCapacity(MINFORI_SIZE); //按鈕存放
	m_BtnArray[MAINPG_INFOAllRIGHT]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint IconSite_Robot=CCPointMake(280*W_unit,winSize.height-255*H_unit);	
	CCPoint LabeSite_AllRight=CCPointMake(winSize.width*0.5,winSize.height-815*H_unit);
	CCPoint LabeSite_Enjoy=CCPointMake(winSize.width*0.5,winSize.height-935*H_unit);

	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層
	m_BtnArray[MAINPG_INFOAllRIGHT]->addObject(pSpriteBK);
	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_RobotInfomation, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[MAINPG_INFOAllRIGHT]->insertObject(pSpriteAction,MINFORI_TITAL);

	//ROBOT圖示 
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_MAIN_ROBOTICON); //狀態圖示
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_Robot);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[MAINPG_INFOAllRIGHT]->insertObject(pSprite,MINFORI_ICON);
	//文字
	//Everything
	pLabel = CCLabelTTF::create(STR_RobotMain_Everything, S_FONE, *pSTR_STATE_SIZE,CCSizeMake(winSize.width-200*W_unit,0),kCCTextAlignmentCenter);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_AllRight);
	pSpriteBK->addChild(pLabel,10);
	//Enjoy
	pLabel = CCLabelTTF::create(STR_RobotMain_Enjoy, S_FONE, *pSTR_BTN_SIZE,CCSizeMake(winSize.width-200*W_unit,0),kCCTextAlignmentCenter);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_Enjoy);
	pSpriteBK->addChild(pLabel,10);

	pSpriteBK->setVisible(false);
}

void UI_RobotMain::InitUI_RobotInfoWarning()
{
	m_BtnArray[MAINPG_INFOWARNING] = CCArray::createWithCapacity(MINFOWA_SIZE); //按鈕存放
	m_BtnArray[MAINPG_INFOWARNING]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint IconSite_Robot=CCPointMake(280*W_unit,winSize.height-255*H_unit);	

	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層
	m_BtnArray[MAINPG_INFOWARNING]->addObject(pSpriteBK);
	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_RobotInfomation, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[MAINPG_INFOWARNING]->insertObject(pSpriteAction,MINFOWA_TITAL);

	//ROBOT圖示 
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_MAIN_ROBOTICON); //狀態圖示
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_Robot);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[MAINPG_INFOWARNING]->insertObject(pSprite,MINFOWA_ICON);
	//文字

	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_RobotMain_Go, S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentCenter);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(ccp(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 10);
	m_BtnArray[MAINPG_INFOWARNING]->insertObject(pSpriteDownBtn,MINFOWA_BUTTON);

	pSpriteBK->setVisible(false);
}
void UI_RobotMain::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true);
	Show_MainPage();
	schedule(schedule_selector(UI_RobotMain::ReleseRobotState),G_ROBOTSTATE_RELEASE_TIME);     //啟動update() 每10秒1次
	scheduleUpdate(); //開啟默認定時器
	
}

void UI_RobotMain::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();
	//RobotNameRetry();	//強迫下次重抓ROBOT 名稱
	unschedule(schedule_selector(UI_RobotMain::ReleseRobotState)); // 關閉更新程式
	unscheduleUpdate();            //取消默認定時器
}

void UI_RobotMain::Hide_AllPage()
{
	Hide_MainPage();
	Hide_InfoAllRight();
	Hide_InfoWarning();
	PageType = MAINPG_LAYER;
}
void UI_RobotMain::Show_MainPage()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_BK);
	pSprite->setVisible(true);
	PageType = MAINPG_STATE;
	//CCLabelTTF* pLabel =(CCLabelTTF*)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABNAME);	
	//pLabel->setString("Reset");		

	//強制更新訊息
	//CheckRobtoTimeSync();
	RobotTimeClean(); 
	RobotStateClean();
	RobotPowerClean();
	
}
void UI_RobotMain::Hide_MainPage()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_BK);
	pSprite->setVisible(false);
}
void UI_RobotMain::Show_InfoAllRight()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[MAINPG_INFOAllRIGHT]->objectAtIndex(MINFORI_BK);
	pSprite->setVisible(true);
	PageType = MAINPG_INFOAllRIGHT;
}
void UI_RobotMain::Hide_InfoAllRight()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[MAINPG_INFOAllRIGHT]->objectAtIndex(MINFORI_BK);
	pSprite->setVisible(false);
}
void UI_RobotMain::Show_InfoWarning()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[MAINPG_INFOWARNING]->objectAtIndex(MINFORI_BK);
	pSprite->setVisible(true);
	PageType = MAINPG_INFOWARNING;
}
void UI_RobotMain::Hide_InfoWarning()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[MAINPG_INFOWARNING]->objectAtIndex(MINFOWA_BK);
	pSprite->setVisible(false);
}
 //回傳機台名稱
const char* UI_RobotMain::GetRobotName()
{
	return sRobotName.c_str();
}
void UI_RobotMain::SaveRobotName(char* pName)
{
	sRobotName=pName;	
}
void UI_RobotMain::RobotFlagClean(int iType)
{
	RobotFlag[iType]=RobFlagSt_None;
	RobotFlag[RobFlag_All]=RobFlagSt_None;
}
void UI_RobotMain::RobotFlagRetry(int iType)
{
	RobotFlag[iType]=RobFlagSt_Retry;
	RobotFlag[RobFlag_All]=RobFlagSt_None;
}

void UI_RobotMain::RobotFlagOk(int iType)
{
	RobotFlag[iType]=RobFlagSt_Get;
	FailCount[iType]=0;
}
void UI_RobotMain::RobotFlagFail(int iType)
{
	FailCount[iType]++;
	if(FailCount[iType]>3) //超過三次無法取得 就不再傳輸
		RobotFlag[iType]=RobFlagSt_Error;
	else
		RobotFlag[iType]=RobFlagSt_Fail;

}
void UI_RobotMain::RobotNameClean()
{
	RobotFlagClean(RobFlag_Name);
}

void UI_RobotMain::RobotNameRetry()
{
	RobotFlagRetry(RobFlag_Name);
}
void UI_RobotMain::RobotNameOk()
{ 					
	RobotFlagOk(RobFlag_Name);
}
void UI_RobotMain::RobotNameFail()
{ 					
	RobotFlagFail(RobFlag_Name);	
}
void UI_RobotMain::RobotTimeClean()
{
	RobotFlagClean(RobFlag_Time);
}
void UI_RobotMain::RobotTimeRetry()
{
	RobotFlagRetry(RobFlag_Time);
}
void UI_RobotMain::RobotTimeOk()
{ 					
	RobotFlagOk(RobFlag_Time);
}
void UI_RobotMain::RobotTimeFail()
{ 					
	RobotFlagFail(RobFlag_Time);	
}
void UI_RobotMain::RobotStateClean()
{
	RobotFlagClean(RobFlag_State);
}
void UI_RobotMain::RobotStateRetry()
{
	RobotFlagRetry(RobFlag_State);
}
void UI_RobotMain::RobotStateOk()
{ 					
	RobotFlagOk(RobFlag_State);
}
void UI_RobotMain::RobotStateFail()
{ 					
	RobotFlagFail(RobFlag_State);	
}

void UI_RobotMain::RobotPowerClean()
{
	RobotFlagClean(RobFlag_Power);
}
void UI_RobotMain::RobotPowerRetry()
{
	RobotFlagRetry(RobFlag_Power);
}

void UI_RobotMain::RobotPowerOk()
{ 					
	RobotFlagOk(RobFlag_Power);
}
void UI_RobotMain::RobotPowerFail()
{ 					
	RobotFlagFail(RobFlag_Power);	
}

void UI_RobotMain::SaveRobotPower(float fValue)
{
	fPowerPart=fValue;
}
void UI_RobotMain::SaveRobotState(AI_STATE AIStates)
{
	stRobotState=AIStates;
	//stRobotState.CleanStatus=AIStates.CleanStatus;
}
// 時間同步(視窗點選後)
void UI_RobotMain::MsgBoxTimeSync(cocos2d::CCNode *pNode)
{
	if(pNode->getTag()==1)
	{//CCMessageBox(STR_BTN_Ok,"CLOSE");
		SavePcTimeToRobotTime();
	}	
	// bShowMSG_TimeSync=false;  警告視窗只會開啟一次
}
// 顯示警告視窗_時間同步
void UI_RobotMain::ShowTimeSyncBox()
{
	PopupLayer* pl = PopupLayer::createWithSpriteFrameName(S_MSGBOX_BK);
	//CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//pl->setContentSize(CCSizeMake(winSize.width*0.7, winSize.height*0.4)); 
	pl->setContentText(STR_MSG_TimeSynchronize_ENG, *pSTR_DIALOG_SIZE, 60, 250);
	// 設置回調函數，回調傳回一個 CCNode 以獲取 tag 判斷點擊的按鈕
	// 這只是作為一種封裝實現，如果使用 delegate 那就能夠更靈活的控制參數了
	pl->setCallbackFunc(this, callfuncN_selector(UI_RobotMain::MsgBoxTimeSync));
	pl->addButtonBySpriteFrame(S_MSGBOX_BTN_BK, S_MSGBOX_BTN_BK, STR_BTN_Ok, 1);
	pl->addButtonBySpriteFrame(S_MSGBOX_BTN_BK, S_MSGBOX_BTN_BK, STR_BTN_Canel, 0);
	// 添加到當前層
	this->addChild(pl);
}
//檢查是否需要時間同步
void UI_RobotMain::CheckRobtoTimeSync()
{
	if(bShowMSG_TimeSync)
		return ;
	if(CheckSyncTimeAlarmSetting())
	{//如果有設定檢查 才強迫每次顯示同步警告視窗
		int iState=CheckPcTimeAndRobotTime();
		if(iState==1)
		{
			ShowTimeSyncBox();
			bShowMSG_TimeSync=true;
		}
		if(iState!=-1)
			bTimeSyncEven=true;
	}

}

//////////////////////////////////////////////////////////////////////////
//