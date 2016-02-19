#include <ImageList.h>

#include <HelloWorldScene.h>
#include <pthread.h>
#include <semaphore.h>
#include <MAP>

#if PLAYERSTAGE

#include <libplayerc++/playerc++.h>
#include "player.h"

#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <boost/format.hpp>
#endif
//PLAYER_DISPATCH_UI_NUMBER 
//PLAYER_DISPATCH_PM_BUMBER
//PLAYER_DISPATCH_PM_POWER_NUMBER
//•H§W§T≤’©w∏q¨O§‚∞ •[®Ïplayer_insterfaces.h
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

//static char POWER_STATE_STR[PLAYER_DISPATCH_PM_POWER_NUMBER][20]={
//	"VERY_LOW_POWER",
//	"SLEEP",         
//	"ON_CHARGER",    
//	"CHARGE",        
//	"CHARGE_FULL",   
//};


USING_NS_CC;
using namespace std;
extern HelloWorld* m_MainView;
extern UI_RobotMain* m_pUIRobotMain;
extern UI_RobotScanning* m_pUIRobotSacnning;
extern UI_RobotRemote* m_UIRobotRemote;

#if PLAYERSTAGE
using namespace PlayerCc;
/// Player Client
extern PlayerClient *pPlayerclient;
extern PowerProxy	 *pPowerProxy;
extern DispatcherProxy *pDisProxy;
player_dispatcher_time_t lastRobotTime;
player_dispatcher_timezone_t lastRobotZone;

#endif


static Vec2 SelBKSite[MSTAET_BTNSize];

sem_t semRobMapRelease;	//地圖更新信號
static bool bRobotMapReady=false;
//static bool bRobotMapUpdate=false;
sem_t semRobMapUpdateOver;	//地圖傳輸完畢

sem_t* pMapRelease;
sem_t* pMapUpdateOver;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
//建立執行緒 底層不斷更新地圖(每分鐘一次)
static pthread_t s_RobotMAPThread; //≥s±µROBOT thread
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
	{// 建立地圖失敗
		
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
		//sem_trywait(&semRobMapUpdateOver);	//≤M∞£§ß´eßÛ∑s∞T∏π
		bState=GuiData_MapRelease();				//¶aπœßÛ∑s		
		if(bState)
		{
			bRobotMapReady=true;				//•u≠nÆ≥πL§@¶∏¥N≥]¨∞TRUE
			GuiData_MapReleaseShowData();
			sem_post(&semRobMapUpdateOver);		//ßi™æ§w∏gßÛ∑sßπ≤¶
			
			//while(sem_trywait(&semRobMapRelease)); //≤M™≈ßÛ∑s∞T∏π
			iErrorTime=0;
		}
		else
		{
			iErrorTime++;
			sem_post(&semRobMapRelease);
		}
		if(iErrorTime>=30) //∂WπL10¶∏≥sΩu•¢±— ≈„•‹¥£•‹µ¯µ°  §§§Óµ{¶°
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

//此部分的　semphone 可以移動到commond
void WaitRobotMapUpdate()
{
	sem_wait(pMapUpdateOver);	//≤M∞£§ß´eßÛ∑s∞T∏π
}
int WaittryRobotMapUpdate()
{
	return sem_trywait(pMapUpdateOver);	//≤M∞£§ß´eßÛ∑s∞T∏π
}
void RobotMapUpdate()
{
	sem_post(pMapRelease);
}
bool IsRobotMapReady()
{
	return bRobotMapReady;
}
void RobotMapThread_Init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//MAC 不支援 sem_init 需改為sem_open
    
    sem_init(&semRobMapRelease,0,0);
	sem_init(&semRobMapUpdateOver,0,0);
    pMapRelease=&semRobMapRelease;
    pMapUpdateOver=&semRobMapUpdateOver;
#else
    
    pMapRelease = sem_open("mapRelease", O_CREAT, S_IRUSR|S_IWUSR);

    pMapUpdateOver  = sem_open("mapUpdateOver", O_CREAT, S_IRUSR|S_IWUSR);
    
#endif
}
//sempho 建立結束
UI_RobotMain::UI_RobotMain():
touchEnabled(false),
touchListener(nullptr),
PageType(MAINPG_LAYER),
fPowerPart(-100),
bShowMSG_TimeSync(false),
bTimeSyncEven(false)
{
    for(int i=0;i<MAINPG_SIZE;i++)
    {
        auto Page = new SpriteVector;
        m_SpriteVectors.push_back(Page);
        auto Page2 = new LabelVector;
        m_LabelVectors.push_back(Page2);
    }
	RobotNameClean();
	memset(RobotFlag,0,sizeof(RobotFlag));
	memset(FailCount,0,sizeof(FailCount));	
	memset(&stRobotState,0,sizeof(stRobotState));	
	memset(&stOldRobotState,0,sizeof(stOldRobotState));	
}

UI_RobotMain::~UI_RobotMain()
{
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
    SSIDName.clear();
    APName.clear();
	//for(int i=0;i<MAINPG_SIZE;i++)
	//	m_BtnArray[i]->release();

}
bool UI_RobotMain::init()
{  
	if (!Layer::init())
	{  
		return false;  
	}  
	InitUI();  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	//´ÿ•ﬂßÛ∑s¶aπœthread
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
//傳入參數
//int iTyep: 0 表示sprite 1:表示Label
void UI_RobotMain::Function_RobotMain(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
    case MSTAET_ICONALARM: //§È´·ßÔ¨∞ßP¬_¨Oß_¶≥ø˘ª~
            Hide_AllPage();
            Show_InfoWarning();
        break;
    case MSTAET_BTNMAP:
            ChangLayer_Map();
        break;
    case MSTAET_BTNREMOTE:
            ChangLayer_Remote();
        break;
    case MSTAET_BTNRESERVATION:
       		ChangLayer_Reservation();
   		break;
    case MSTAET_BTNJOB:
            ChangLayer_CleanJob();
        break;
	case MSTAET_BTNSETUP:
		ChangLayer_RobotSetup();
		break;
    case MSTAET_LAB_ALARM:
            Hide_AllPage();
            Show_InfoAllRight();
        break;
            
    case MSTAET_LAB_NAME:	//¬IøÔ¶W∫Ÿ ±qROBOT ßÛ∑s¶W∫Ÿ
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            RobotNameClean();
            CallThread_GetRobotName();
            
#endif
        break;
    case MSTAET_LAB_DATE:	//¬IøÔÆ…∂° ±qROBOT ßÛ∑sÆ…∂°
    case MSTAET_LAB_TIME:
            RobotTimeClean();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
            CallThread_GetRobotTime();		
#endif
        break;
	}
		
}
//////////////////////////////////////////////////////////////////////////
// ®˙±oROBOT Æ…∂°
void UI_RobotMain::ReleaseLab_RobotDateTime()
{
	//µe≠±≥]©w
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	int iResult=RelaseRobotTime((void* )&lastRobotTime);
#else
    auto iResult = 3; //FOXME MAC暫時關閉時間更新
#endif
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
#if PLAYERSTAGE
	player_dispatcher_time_t *pDate=(player_dispatcher_time_t *)pData;
	auto pLabel =(auto)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABDATE);		
	char sValue[24];
	sprintf(sValue,STR_RobotMain_Date,pDate->tm_year,pDate->tm_mon,pDate->tm_mday);
	pLabel->setString(sValue);
	pLabel =(auto)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABTIME);		
	sprintf(sValue,STR_RobotMain_Time,pDate->tm_hour,pDate->tm_min);
	pLabel->setString(sValue);
#else
    auto pLabel =m_LabelVectors.at(MAINPG_STATE)->at(MSTAET_LAB_DATE);
    char sValue[24];
    sprintf(sValue,STR_RobotMain_Date,2015,01,01);
    pLabel->setString(sValue);
    pLabel =m_LabelVectors.at(MAINPG_STATE)->at(MSTAET_LAB_TIME);
    sprintf(sValue,STR_RobotMain_Time,0,0);
    pLabel->setString(sValue);
    
#endif

}
void UI_RobotMain::ReleaseLab_RobotDate()
{
	//auto pLabel =(auto)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABDATE);
    auto pLabel =m_LabelVectors.at(MAINPG_STATE)->at(MSTAET_LAB_DATE);
	char sValue[24];
#if PLAYERSTAGE
	sprintf(sValue,STR_RobotMain_Date,lastRobotTime.tm_year,lastRobotTime.tm_mon,lastRobotTime.tm_mday);
#else
    sprintf(sValue,STR_RobotMain_Date,2015,01,01);
#endif
	pLabel->setString(sValue);

}
void UI_RobotMain::ReleaseLab_RobotTime()
{
//	auto pLabel =(auto)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABTIME);
    auto pLabel =m_LabelVectors.at(MAINPG_STATE)->at(MSTAET_LAB_TIME);
	char sValue[24];
#if PLAYERSTAGE
	sprintf(sValue,STR_RobotMain_Time,lastRobotTime.tm_hour,lastRobotTime.tm_min);
#else
    sprintf(sValue,STR_RobotMain_Time,0,0);
#endif

	pLabel->setString(sValue);
	RobotFlag[RobFlag_Time]=RobFlagSt_ReleaseOver;

}
void UI_RobotMain::ReleaseLab_RobotState()
{
//	auto pLabel =(auto)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABALARM);
    auto pLabel =m_LabelVectors.at(MAINPG_STATE)->at(MSTAET_LAB_ALARM);
	if(stOldRobotState.CleanStatus != stRobotState.CleanStatus && stRobotState.CleanStatus<PLAYER_DISPATCH_PM_BUMBER )
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
	//auto pSprite=(auto)m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_ICONALARM); //ƒµßiπœ•‹
    auto pSprite =m_SpriteVectors.at(MAINPG_STATE)->at(MSTAET_ICONALARM); //警告圖示
	if(stOldRobotState.Error != stRobotState.Error && stRobotState.Error!=0)
	{
        auto cache = SpriteFrameCache::getInstance();
        auto pFrame = cache->getSpriteFrameByName( S_MAIN_ALARM_ERROR_ICON );
//		auto pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_MAIN_ALARM_ERROR_ICON);
		pSprite->setDisplayFrame(pFrame);
		pSprite->setVisible(true);
		stOldRobotState.Error = stRobotState.Error;
	}	
	else if(stOldRobotState.Worry != stRobotState.Worry  && stRobotState.Worry !=0)
	{
        auto cache = SpriteFrameCache::getInstance();
        auto pFrame = cache->getSpriteFrameByName( S_MAIN_ALARM_WARN_ICON );

//		auto pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_MAIN_ALARM_WARN_ICON);
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
//	auto pLabel =(auto)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABNAME);
    auto pLabel =m_LabelVectors.at(MAINPG_STATE)->at(MSTAET_LAB_NAME);
	pLabel->setString(sRobotName.c_str());
	RobotFlag[RobFlag_Name]=RobFlagSt_ReleaseOver;

}
void UI_RobotMain::ReleaseLab_RobotPower()
{
//	auto pLabel =(auto)	m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_LABPOWER);
    auto pLabel =m_LabelVectors.at(MAINPG_STATE)->at(MSTAET_LAB_POWER);
	char sNump[12];
	sprintf(sNump,"%.1f%%",fPowerPart);
	pLabel->setString(sNump);
	RobotFlag[RobFlag_Power]=RobFlagSt_ReleaseOver;
}
//¿À¨d≤{¶b¶W∫Ÿ™¨∫A!®M©w¨Oß_≠n∂«∞e´¸•O

// ¶^∂«true ™Ì•‹§w∏g¶b∞ı¶Ê, false ™Ì•‹¶£∏L§£∞ı¶Ê
bool UI_RobotMain::CheckRobotName()
{
	if(RobotFlag[RobFlag_Name]==RobFlagSt_None) //®S¶≥æ˜•x¶W∫Ÿ
	{			
		if(CallThread_GetRobotName())
		{
			RobotFlag[RobFlag_Name]=RobFlagSt_Wait;
			return true;
		}
	}
	return false;
}
//¿À¨d≤{¶bÆ…∂°™¨∫A!®M©w¨Oß_≠n∂«∞e´¸•O
// ¶^∂«true ™Ì•‹§w∏g¶b∞ı¶Ê, false ™Ì•‹¶£∏L§£∞ı¶Ê
bool UI_RobotMain::CheckRobotTime()
{
	//if(RobotFlag[RobFlag_Time]!=RobFlagSt_Error) //•i•H∂∂ßQßÏ®˙Æ…∂°
	if(RobotFlag[RobFlag_Time]==RobFlagSt_None) //®S¶≥æ˜•xÆ…∂°
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
	//if(RobotFlag[RobFlag_State]!=RobFlagSt_Error) //•i•H∂∂ßQßÏ®˙™¨∫A
	if(RobotFlag[RobFlag_State]==RobFlagSt_None) //®S¶≥æ˜•x™¨∫A
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
	if(RobotFlag[RobFlag_Power]!=RobFlagSt_Error) //•i•H∂∂ßQßÏ®˙πq∂q
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
	if(RobotFlag[RobFlag_Schedule]!=RobFlagSt_Error) //•i•H∂∂ßQßÏ®˙Æ…∂°
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
//®C10¨Ì§@¶∏ ¨d∏ﬂROBOT §W≠±Æ…∂° ∏Ú™¨∫A
//static int iCommandLoopCount=0;
void UI_RobotMain::ReleseRobotState(float dt)
{
	//float fpower=fPowerPart;
#if PLAYERSTAGE
	if(pPowerProxy!=NULL)
	{
		RobotPowerRetry();
	}

	if(pDisProxy!=NULL)
	{
		RobotTimeRetry(); 
		RobotStateRetry();
	}
#endif

}
//¿qª{™∫©wÆ…æπ ®C¨Ì60¶∏ ®C≠”FPS §@¶∏
static int RobotMainUpdateTime=0;
void UI_RobotMain::update(float dt)
{
	if(RobotMainUpdateTime%60==0) //ßÛ∑s™¨∫A OR Æ…∂°
	{ //®C§@¨Ìƒ¡
		if(RobotFlag[RobFlag_Time]==RobFlagSt_Get) //•i•H∂∂ßQßÏ®˙Æ…∂°
		{
			//±NROBOT ≤{¶bÆ…∂°¶s®Ï≤{¶b®t≤Œ
			//ReleaselastRobotTime();
			ReleaseLab_RobotDateTime(); //ßÛ∑sÆ…∂°
		}
		else //Æ⁄æ⁄®t≤ŒÆ…∂°ßÛ∑s
		{

		}

		if(RobotFlag[RobFlag_Name]==RobFlagSt_Get) //ßÛ∑s¶W∫Ÿ
			ReleaseLab_RobotName();

		if(RobotFlag[RobFlag_State]==RobFlagSt_Get) //ßÛ∑s™¨∫A
		{
			ReleaseLab_RobotState();
			ReleaseIcon_RobotState();
		}
		if(RobotFlag[RobFlag_Power]==RobFlagSt_Get) //ßÛ∑sπq∂q
		{
			ReleaseLab_RobotPower();
		}
		if(!bTimeSyncEven)
		{
			CheckRobtoTimeSync();
		}
	}
	if(RobotFlag[RobFlag_All]==RobFlagSt_None && (RobotMainUpdateTime%5 ==0)) //®C5 FPS ∂«∞e§@¶∏
	{
		for(int i=0;i<RobFlag_Number;i++)
		{
			if(RobotFlag[i]==RobFlagSt_None )
			{ //§@¶∏•u¶≥∞eπF§@≠”
				if(i==RobFlag_All)
				{ //∞eπL§@Ω¸´· ¿À¨d¨Oß_•˛≥°∞e•X
					int j=0;
					for(j=0;j<RobFlag_All;j++)
					{
						if(RobotFlag[j]==RobFlagSt_Wait)
							break;
					}
					if(j==RobFlag_All) //™Ì•‹•˛≥°≥£§w∏g∞eπL§@Ω¸§F
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
//ßÛ∑sROBOT ´¸•O
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
//void UI_RobotMain::registerWithTouchDispatcher()
//{
//	// higher priority than dragging
//	CCDirector* pDirector = CCDirector::sharedDirector();
//	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
//}
//按鈕底色位置
void UI_RobotMain::PressSpriteMove(int iBtnIdx)
{
    auto sprites=m_SpriteVectors.at(PageType);
    auto pSprite =sprites->at(MSTAET_BTNSELBK);
//	auto pSprite =(auto)m_BtnArray[MAINPG_STATE]->objectAtIndex(MSTAET_BTNSELBK);  //≠Ï¶]´›¨d
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

void UI_RobotMain::InitUI()
{
	winSize = Director::getInstance()->getVisibleSize();
    winOrigin = Director::getInstance()->getVisibleOrigin();

	InitUI_MainPage();
	InitUI_RobotInfoAllRight();
	InitUI_RobotInfoWarning();
	
}
void UI_RobotMain::InitUI_MainPage()
{
    auto pageSprite = m_SpriteVectors.at(MAINPG_STATE);
    auto pageLabel = m_LabelVectors.at(MAINPG_STATE);
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍

    float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point IconSite_Robot=Vec2(565*W_unit,winSize.height-210*H_unit);
	Point LabeSite_Welcome=Vec2(77*W_unit,winSize.height-375*H_unit);
	Point LabeSite_RobotName=Vec2(77*W_unit,winSize.height-485*H_unit);
	Point LabeSite_Date=Vec2(77*W_unit,winSize.height-600*H_unit);
	Point LabeSite_Time=Vec2(77*W_unit,winSize.height-667*H_unit);
	Point LabeSite_Status=Vec2(595*W_unit,winSize.height-692*H_unit);
	Point LabeSite_StatusValue=Vec2(595*W_unit,winSize.height-760*H_unit);
	Point IconSite_Alarm=Vec2(735*W_unit,winSize.height-695*H_unit);
	Point LabeSite_Power=Vec2(837*W_unit,winSize.height-692*H_unit);
	Point LabeSite_PowerValue=Vec2(837*W_unit,winSize.height-760*H_unit);
	Point SpriSite_MainBK=Vec2(40*W_unit,winSize.height-963*H_unit);
	
	Point SpriSite_Schedule=Vec2(40*W_unit,winSize.height-963*H_unit);
	Point SpriSite_Job=Vec2(540*W_unit,winSize.height-963*H_unit);
	Point SpriSite_Remote=Vec2(40*W_unit,winSize.height-1319*H_unit);
	Point SpriSite_Map=Vec2(540*W_unit,winSize.height-1319*H_unit);

	Point IconSite_Schedule=Vec2(180*W_unit,winSize.height-985*H_unit);
	Point LabeSite_Schedule=Vec2(281*W_unit,winSize.height-1173*H_unit);
	Point IconSite_Job=Vec2(680*W_unit,winSize.height-985*H_unit);
	Point LabeSite_Job=Vec2(791*W_unit,winSize.height-1173*H_unit);
	Point IconSite_Remote=Vec2(180*W_unit,winSize.height-1341*H_unit);
	Point LabeSite_Remote=Vec2(281*W_unit,winSize.height-1529*H_unit);
	Point IconSite_Map=Vec2(680*W_unit,winSize.height-1341*H_unit);
	Point LabeSite_Map=Vec2(791*W_unit,winSize.height-1529*H_unit);

    //四個按鈕位置
	SelBKSite[MSTAET_BTN1]=SpriSite_Schedule;
	SelBKSite[MSTAET_BTN2]=SpriSite_Job;
	SelBKSite[MSTAET_BTN3]=SpriSite_Remote;
	SelBKSite[MSTAET_BTN4]=SpriSite_Map;

    
    pSpriteBK->setPosition(winOrigin+Vec2(winSize/2)); //圖檔置中
	addChild(pSpriteBK);
    pSpriteBK->setTag(MSTAET_BK);
    pageSprite->pushBack(pSpriteBK);
    m_itemIdx[MAINPG_STATE*100+MSTAET_BK]=pageSprite->size()-1;

    //ROBOT 圖示
	auto pSprite=Sprite::createWithSpriteFrameName(S_MAIN_ROBOTICON);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_Robot);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(MSTAET_ICON);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAINPG_STATE*100+MSTAET_ICON]=pageSprite->size()-1;
    
    //welcome
    auto pLabel = Label::createWithSystemFont(STR_STATE_Welcome, S_FONE, *pSTR_STATE_SIZE,Size::Size(winSize.width-200*W_unit,0),TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_Welcome);
	pSpriteBK->addChild(pLabel,10);

    //robot name
	sRobotName="Name";
	pLabel = Label::createWithSystemFont(sRobotName.c_str(), S_FONE, *pSTR_BTN_SIZE,Size::Size(winSize.width-200*W_unit,0),TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_RobotName);
    pLabel->setTag(MSTAET_LAB_NAME);
    pageLabel->pushBack(pLabel);
    m_itemIdx[MAINPG_STATE*100+MSTAET_LAB_NAME]=pageLabel->size()-1;
	pSpriteBK->addChild(pLabel,10);
	
    //date 對齊ＲＯＢＯＴ圖示下緣
	pLabel = Label::createWithSystemFont("2015/01/01", S_FONE, *pSTR_DATE_SIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT);
    pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Date);
    pLabel->setTag(MSTAET_LAB_DATE);
    pageLabel->pushBack(pLabel);
    m_itemIdx[MAINPG_STATE*100+MSTAET_LAB_DATE]=pageLabel->size()-1;
    pSpriteBK->addChild(pLabel,10);
	
    //TIME
	pLabel = Label::createWithSystemFont("00:00", S_FONE, *pSTR_TIME_SIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Time);
	pSpriteBK->addChild(pLabel,10);
    pLabel->setTag(MSTAET_LAB_TIME);
    pageLabel->pushBack(pLabel);
    m_itemIdx[MAINPG_STATE*100+MSTAET_LAB_TIME]=pageLabel->size()-1;
    
    //status lab
	pLabel = Label::createWithSystemFont(STR_RobotMain_Status, S_FONE, *pSTR_ROBOTMAIN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_LIGHTBLACK);
	pLabel->setPosition(LabeSite_Status);
	pSpriteBK->addChild(pLabel,10);

    pLabel = Label::createWithSystemFont(UI_STATE_STR[0], S_FONE, *pSTR_ROBOTMAIN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_StatusValue);
	pSpriteBK->addChild(pLabel,10);
    pLabel->setTag(MSTAET_LAB_ALARM);
    pageLabel->pushBack(pLabel);
    m_itemIdx[MAINPG_STATE*100+MSTAET_LAB_ALARM]=pageLabel->size()-1;

	pSprite=Sprite::createWithSpriteFrameName(S_MAIN_ALARM_ERROR_ICON);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_Alarm);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(MSTAET_ICONALARM);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAINPG_STATE*100+MSTAET_ICONALARM]=pageSprite->size()-1;

    //power lab
	pLabel = Label::createWithSystemFont(STR_RobotMain_Power, S_FONE, *pSTR_ROBOTMAIN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_LIGHTBLACK);
	pLabel->setPosition(LabeSite_Power);
	pSpriteBK->addChild(pLabel,10);
    
	pLabel = Label::createWithSystemFont("0%", S_FONE, *pSTR_ROBOTMAIN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_PowerValue);
	pSpriteBK->addChild(pLabel,10);
    pLabel->setTag(MSTAET_LAB_POWER);
    pageLabel->pushBack(pLabel);
    m_itemIdx[MAINPG_STATE*100+MSTAET_LAB_POWER]=pageLabel->size()-1;

	//按鈕背景 十字背景
	auto pSpriteBtn=Sprite::createWithSpriteFrameName(S_MAIN_BTNBK);
	pSpriteBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSpriteBtn->setPosition(SpriSite_MainBK);
	pSpriteBK->addChild(pSpriteBtn,10);

    //行程預約 圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAIN_BTNSCHEDULED);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_Schedule);
	pSpriteBK->addChild(pSprite,12);								 
    pSprite->setTag(MSTAET_BTNRESERVATION);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAINPG_STATE*100+MSTAET_BTNRESERVATION]=pageSprite->size()-1;
    
    //行程預約 文字
	pLabel = Label::createWithSystemFont(STR_BTN_Reservation, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Schedule);
	pSpriteBK->addChild(pLabel,12);
    
    //各種掃地指令 圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAIN_BTNJOB);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_Job);	
	pSpriteBK->addChild(pSprite,11);	
    pSprite->setTag(MSTAET_BTNJOB);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAINPG_STATE*100+MSTAET_BTNJOB]=pageSprite->size()-1;

    //各種掃地指令 文字
	pLabel = Label::createWithSystemFont(STR_BTN_CleanJob, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Job);
	pSpriteBK->addChild(pLabel,10);
	
    //遠端遙控 圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAIN_BTNREMOTE);
    pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_Remote);
	pSpriteBK->addChild(pSprite,11);
    pSprite->setTag(MSTAET_BTNREMOTE);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAINPG_STATE*100+MSTAET_BTNREMOTE]=pageSprite->size()-1;
    
    //遠端遙控 文字
	pLabel = Label::createWithSystemFont(STR_BTN_RemoteControl, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Remote);
	pSpriteBK->addChild(pLabel,10);

	//地圖切換 圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAIN_BTNMAP);
    pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_Map);
	pSpriteBK->addChild(pSprite,11);
    pSprite->setTag(MSTAET_BTNMAP);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAINPG_STATE*100+MSTAET_BTNMAP]=pageSprite->size()-1;
    
    //地圖切換 文字
    pLabel = Label::createWithSystemFont(STR_BTN_Map, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabeSite_Map);
	pSpriteBK->addChild(pLabel,10);

	//點選提示
	pSpriteBtn=Sprite::createWithSpriteFrameName(S_MAIN_BTNBKONE); 
	pSpriteBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSpriteBtn->setPosition(SpriSite_Map);
	pSpriteBK->addChild(pSpriteBtn,11);	
	pSpriteBtn->setVisible(false);
    pSpriteBtn->setTag(MSTAET_BTNSELBK);
    pageSprite->pushBack(pSpriteBtn);
    m_itemIdx[MAINPG_STATE*100+MSTAET_BTNSELBK]=pageSprite->size()-1;

	//最底部設定按鈕
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteDownBtn->setAnchorPoint(Vec2::ZERO);
	pSpriteDownBtn->setPosition(Vec2::ZERO);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
    
    //設定按鈕 圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAIN_BTNSETUP);
	pSprite->setPosition(Vec2(pSpriteDownBtn->getBoundingBox().size/2));
	pSpriteDownBtn->addChild(pSprite,10);
    pSpriteDownBtn->setTag(MSTAET_BTNSETUP);
    pageSprite->pushBack(pSpriteDownBtn);
    m_itemIdx[MAINPG_STATE*100+MSTAET_BTNSETUP]=pageSprite->size()-1;


	pSpriteBK->setVisible(false);
}

void UI_RobotMain::InitUI_RobotInfoAllRight()
{
    auto pageSprite = m_SpriteVectors.at(MAINPG_INFOAllRIGHT);
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point IconSite_Robot=Vec2(280*W_unit,winSize.height-255*H_unit);	
	Point LabeSite_AllRight=Vec2(winSize.width*0.5,winSize.height-815*H_unit);
	Point LabeSite_Enjoy=Vec2(winSize.width*0.5,winSize.height-965*H_unit);

	//底圖
	pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	addChild(pSpriteBK);
    pSpriteBK->setTag(MINFORI_BK);
    pageSprite->pushBack(pSpriteBK);
    m_itemIdx[MAINPG_INFOAllRIGHT*100+MINFORI_BK]=pageSprite->size()-1;
    
	//上方灰色抬頭
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO);
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
    
	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON);
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
    
	auto pLabel = Label::createWithSystemFont(STR_TITAL_RobotInfomation, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    pSpriteAction->setTag(MINFORI_TITAL);
    pageSprite->pushBack(pSpriteAction);
    m_itemIdx[MAINPG_INFOAllRIGHT*100+MINFORI_TITAL]=pageSprite->size()-1;

	//ROBOTπœ•‹ 
	auto pSprite=Sprite::createWithSpriteFrameName(S_MAIN_ROBOTICON); //™¨∫Aπœ•‹
    pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_Robot);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(MINFORI_ICON);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAINPG_INFOAllRIGHT*100+MINFORI_ICON]=pageSprite->size()-1;
	
	//Everything
	pLabel = Label::createWithSystemFont(STR_RobotMain_Everything, S_FONE, *pSTR_STATE_SIZE,Size::Size(winSize.width-200*W_unit,0),TextHAlignment::CENTER);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_AllRight);
	pSpriteBK->addChild(pLabel,10);
    
	//Enjoy
	pLabel = Label::createWithSystemFont(STR_RobotMain_Enjoy, S_FONE, *pSTR_BTN_SIZE,Size::Size(winSize.width-200*W_unit,0),TextHAlignment::CENTER);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_Enjoy);
	pSpriteBK->addChild(pLabel,10);

	pSpriteBK->setVisible(false);
}

void UI_RobotMain::InitUI_RobotInfoWarning()
{
    auto pageSprite = m_SpriteVectors.at(MAINPG_INFOWARNING);
    auto pageLabel = m_LabelVectors.at(MAINPG_INFOWARNING);
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍

	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point IconSite_Robot=Vec2(280*W_unit,winSize.height-255*H_unit);	

	//底圖
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
    addChild(pSpriteBK);
    pSpriteBK->setTag(MINFOWA_BK);
    pageSprite->pushBack(pSpriteBK);
    m_itemIdx[MAINPG_INFOWARNING*100+MINFOWA_BK]=pageSprite->size()-1;

    //上方灰色抬頭
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO);
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
    
	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON);
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
    
	auto pLabel = Label::createWithSystemFont(STR_TITAL_RobotInfomation, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    pSpriteAction->setTag(MINFOWA_TITAL);
    pageSprite->pushBack(pSpriteAction);
    m_itemIdx[MAINPG_INFOWARNING*100+MINFOWA_TITAL]=pageSprite->size()-1;

	//ROBOT ICON
	auto pSprite=Sprite::createWithSpriteFrameName(S_MAIN_ROBOTICON);
    pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_Robot);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(MAINPG_INFOWARNING);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAINPG_INFOWARNING*100+MAINPG_INFOWARNING]=pageSprite->size()-1;

	//底下按鈕
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO);
	pSpriteDownBtn->setPosition(Vec2::ZERO);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
    
	pLabel = Label::createWithSystemFont(STR_RobotMain_Go, S_FONE, *pSTR_TITAL_SIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::CENTER);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize()*0.5));
	pSpriteDownBtn->addChild(pLabel, 10);
    pLabel->setTag(MINFOWA_LAB_BUTTON);
    pageLabel->pushBack(pLabel);
    m_itemIdx[MAINPG_INFOWARNING*100+MINFOWA_LAB_BUTTON]=pageLabel->size()-1;

	pSpriteBK->setVisible(false);
}

Sprite* UI_RobotMain::getSpritePoint(int iPageTyep,int iPageIdx)
{
    return m_SpriteVectors.at(iPageTyep)->at(m_itemIdx[iPageTyep*100+iPageIdx]);
}
Label* UI_RobotMain::getLabelPoint(int iPageTyep,int iPageIdx)
{
    return m_LabelVectors.at(iPageTyep)->at(m_itemIdx[iPageTyep*100+iPageIdx]);
}
void UI_RobotMain::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true);
	Show_MainPage();
	schedule(schedule_selector(UI_RobotMain::ReleseRobotState),G_ROBOTSTATE_RELEASE_TIME);     //±“∞ update() ®C10¨Ì1¶∏
	scheduleUpdate(); //∂}±“¿qª{©wÆ…æπ
}

void UI_RobotMain::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();
	unschedule(schedule_selector(UI_RobotMain::ReleseRobotState)); // √ˆ≥¨ßÛ∑sµ{¶°
	unscheduleUpdate();            //®˙Æ¯¿qª{©wÆ…æπ

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
    auto pSprite= m_SpriteVectors.at(MAINPG_STATE)->at(MSTAET_BK);
	pSprite->setVisible(true);
	PageType = MAINPG_STATE;
//    setTouchEnabled(true);
	RobotTimeClean();
	RobotStateClean();
	RobotPowerClean();
	
}
void UI_RobotMain::Hide_MainPage()
{
    auto pSprite= m_SpriteVectors.at(MAINPG_STATE)->at(MSTAET_BK);
	pSprite->setVisible(false);
//    setTouchEnabled(false);

}
void UI_RobotMain::Show_InfoAllRight()
{
    auto pSprite= getSpritePoint(MAINPG_INFOAllRIGHT,MINFORI_BK);
	pSprite->setVisible(true);
	PageType = MAINPG_INFOAllRIGHT;
}
void UI_RobotMain::Hide_InfoAllRight()
{
    auto pSprite= getSpritePoint(MAINPG_INFOAllRIGHT,MINFORI_BK);
	pSprite->setVisible(false);
}
void UI_RobotMain::Show_InfoWarning()
{
    auto pSprite= getSpritePoint(MAINPG_INFOWARNING,MINFOWA_BK);
	pSprite->setVisible(true);
	PageType = MAINPG_INFOWARNING;
}
void UI_RobotMain::Hide_InfoWarning()
{
    auto pSprite= getSpritePoint(MAINPG_INFOWARNING,MINFOWA_BK);
	pSprite->setVisible(false);
}
 //¶^∂«æ˜•x¶W∫Ÿ
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
	if(FailCount[iType]>3) //∂WπL§T¶∏µL™k®˙±o ¥N§£¶A∂«øÈ
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
// Æ…∂°¶P®B(µ¯µ°¬IøÔ´·)
void UI_RobotMain::MsgBoxTimeSync(Node *pNode)
{
    //FIXME 先關閉時間同步功能
#if TIMESYNC
	if(pNode->getTag()==1)
	{//CCMessageBox(STR_BTN_Ok,"CLOSE");
		SavePcTimeToRobotTime();
	}
#endif
	// bShowMSG_TimeSync=false;  ƒµßiµ¯µ°•u∑|∂}±“§@¶∏
}
// ≈„•‹ƒµßiµ¯µ°_Æ…∂°¶P®B
void UI_RobotMain::ShowTimeSyncBox()
{

	PopupLayer* pl = PopupLayer::createWithSpriteFrameName(S_MSGBOX_BK);

	pl->setContentText(STR_MSG_TimeSynchronize_ENG, *pSTR_DIALOG_SIZE, 60, 250);

	pl->setCallbackFunc(this, callfuncN_selector(UI_RobotMain::MsgBoxTimeSync));
	pl->addButtonBySpriteFrame(S_MSGBOX_BTN_BK, S_MSGBOX_BTN_BK, STR_BTN_Ok, 1);
	pl->addButtonBySpriteFrame(S_MSGBOX_BTN_BK, S_MSGBOX_BTN_BK, STR_BTN_Canel, 0);
	this->addChild(pl);
    
}
void UI_RobotMain::CheckRobtoTimeSync()
{
	if(bShowMSG_TimeSync)
		return ;
    //FOXME 先關閉時間同步功能
#if TIMESYNC
	if(CheckSyncTimeAlarmSetting())
	{//¶p™G¶≥≥]©w¿À¨d §~±j≠¢®C¶∏≈„•‹¶P®Bƒµßiµ¯µ°
		int iState=CheckPcTimeAndRobotTime();
		if(iState==1)
		{
			ShowTimeSyncBox();
			bShowMSG_TimeSync=true;
		}
		if(iState!=-1)
			bTimeSyncEven=true;
	}
#endif

}

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////
//∑∆π´¬I§U!ßP¬_¨O®∫≠”§∏•Û
bool UI_RobotMain::onTouchBegan(Touch* touch, Event* event)
{

    auto target = static_cast<Layer*>(event->getCurrentTarget());
    if(target==nullptr)
    {
        return true;
    }
    if(!isVisible()) //¶≥≈„•‹§~ƒ~ƒÚ
        return false;
    
    log("UI_RobotMain:%s",__func__);
    
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
    
    switch(PageType)
    {
        default:
            break;
        case MAINPG_STATE:
        {
            auto sprites=m_SpriteVectors.at(MAINPG_STATE);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            {
                auto iIdx=pSprite->getTag();
                if(pSprite->isVisible() && iIdx!=0) //顯示點選的提示圖案
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        PressSpriteMove(iIdx-MSTAET_BTNRESERVATION);
                        break;
                    }
                }
            }
            
            if(!bTouch)
            {
                auto labels=m_LabelVectors.at(MAINPG_STATE);
                for(auto pLabel:*labels)
                {
                    auto iIdx=pLabel->getTag();
                    if(pLabel->isVisible() && iIdx!=0) //顯示點選的提示圖案
                    {
                        auto rectBox= pLabel->getBoundingBox();
                        rectBox.origin+=winOrigin;
                        bTouch = rectBox.containsPoint(nodePosition);
                        if(bTouch)
                        {
                            PressSpriteMove(iIdx-MSTAET_BTNRESERVATION);
                            break;
                        }
                    }
                }
            }
        }
        break;
            
    }
    return true;
}

void UI_RobotMain::onTouchMoved(Touch* touch, Event* event)
{
    //	Point touchLocation = touch->getLocation();
    //	Point nodePosition = convertToNodeSpace( touchLocation );
}

void UI_RobotMain::onTouchEnded(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite *>(event->getCurrentTarget());
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation ); //get touch point
    //Vec2 nodePosition = this->getParent()->convertToNodeSpace( touchLocation );
    switch(PageType)
    {
        default:
            break;
        case MAINPG_INFOWARNING:
        {
            auto sprites=m_SpriteVectors.at(PageType);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            {
                //auto pSprite = (auto)m_BtnArray[PageType]->objectAtIndex(i);
                auto iIdx=pSprite->getTag();
                if(pSprite->isVisible() && iIdx!=0)
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        Function_RobotInfoWarning(iIdx);
                        break;
                    }
                    
                    
                }
            }
        }
        break;
        case MAINPG_INFOAllRIGHT:
        {
            //ßP¬_®∫§@≠”§∏•Û
            auto rectBox= this->getBoundingBox();
            auto sprites=m_SpriteVectors.at(PageType);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            {
                auto iIdx=pSprite->getTag();
                if(pSprite->isVisible() && iIdx!=0)
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        Function_RobotInfoAllright(iIdx);
                        break;
                    }
                    
                }
            }
        }
            break;
        case MAINPG_STATE:
        {
            auto sprites=m_SpriteVectors.at(PageType);
            auto bTouch = false;
            for(auto pSprite:*sprites) //圖檔檢測
            {
                auto iIdx=pSprite->getTag();
                if(pSprite->isVisible() && iIdx!=0)
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        PressSpriteMove(iIdx-MSTAET_BTNRESERVATION);
                        Function_RobotMain(iIdx);
                        break;
                    }
//                    if(iIdx==6) //debug draw touch rect
//                    {
//                        DrawNode *pDebug=DrawNode::create();
//                        pDebug->drawPoint(nodePosition, sizeof(nodePosition), Color4F::BLUE);
//                        pDebug->drawPoint(rectBox.origin, sizeof(rectBox.origin), Color4F::GRAY);
//                        pDebug->drawRect(rectBox.origin, rectBox.size, Color4F::GREEN);
//                        pSprite->getParent()->addChild(pDebug);
//                    }
                }
            }
            if(!bTouch)
            {
                auto labels=m_LabelVectors.at(PageType);//字串標籤檢測
                for(auto pLabel:*labels)
                {
                    auto iIdx=pLabel->getTag();
                    auto rectBox= pLabel->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        Function_RobotMain(iIdx);
                        break;
                    }
                }
            }
            
        }
            break;
            
            
    }
}
//觸碰設定
void UI_RobotMain::setTouchEnabled(bool enable)
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
        
        touchListener->onTouchBegan = CC_CALLBACK_2(UI_RobotMain::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(UI_RobotMain::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(UI_RobotMain::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
    }
    else
    {
        _eventDispatcher->removeEventListener(touchListener);
        CC_SAFE_RELEASE_NULL(touchListener);
    }
}
bool UI_RobotMain::isTouchEnabled() const
{
    return touchEnabled;
}
