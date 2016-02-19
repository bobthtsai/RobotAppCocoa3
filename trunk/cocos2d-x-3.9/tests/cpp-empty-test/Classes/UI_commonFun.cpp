#include "AppDefine.h"
#include <ImageList.h>
#include <HelloWorldScene.h>

#if PLAYERSTAGE
#include <libplayerc++/playerc++.h>
#include "player.h"
#endif

#if ROBOT
#include <wifiscan.h>
#endif

#include <pthread.h>
#include <semaphore.h>
#include <MAP>
#include <time.h> 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <boost/format.hpp>
#include "Ini.h"
#include "dbg.h"

TCHAR sExePath[MAX_PATH];
TCHAR sIniFileName[MAX_PATH];

#endif

USING_NS_CC;
using namespace std;

#if PLAYERSTAGE
using namespace PlayerCc;
/// Player Client
extern PlayerClient *pPlayerclient;
extern DispatcherProxy *pDisProxy;
extern PowerProxy	 *pPowerProxy;
extern Position2dProxy *pPosition2dProxy;
extern player_map_info_t stMapInfo;
//紀錄ROBOT 狀態數值
player_dispatcher_time_t RobotTime;
player_dispatcher_timezone_t RobotZone;

#endif

extern HelloWorld* m_MainView; 
extern UI_RobotScanning* m_pUIRobotSacnning;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

extern UI_RobotMain* m_pUIRobotMain;
extern UI_Reservation* m_pUIReservation;
#endif

//#if ROBOT
pthread_t s_ConnectRobotThread ; //連接ROBOT thread
//#endif
static int m_RobotFunStateByThread=ThreadFun_NONE;
static int m_RobotFunStateByThreadNEXT=ThreadFun_NONE;
// static 	int iRetryTime=0;

void CallThread_SearchRobot()
{
	if(m_RobotFunStateByThread==ThreadFun_NONE) //沒有執行其他指令 才能查詢
		m_RobotFunStateByThread=ThreadFun_SearchAP;
	else
	{
		m_RobotFunStateByThreadNEXT=ThreadFun_SearchAP;
		m_pUIRobotSacnning->SearchIconStop();  //停止旋轉
	}
}
bool CallThread_GetRobotTime()
{
	if(m_RobotFunStateByThread==ThreadFun_NONE && m_RobotFunStateByThreadNEXT==ThreadFun_NONE) 	
	{	//沒有執行其他指令 才能查詢
		m_RobotFunStateByThread=ThreadFun_RequationRobotTime;
		return true;
	}
	return false;
}
bool CallThread_GetRobotState()
{
	if(m_RobotFunStateByThread==ThreadFun_NONE && m_RobotFunStateByThreadNEXT==ThreadFun_NONE) 	
	{	//沒有執行其他指令 才能查詢
		m_RobotFunStateByThread=ThreadFun_RequationRobotState;
		return true;
	}
	return false;
}
bool CallThread_GetRobotPower()
{
	if(m_RobotFunStateByThread==ThreadFun_NONE && m_RobotFunStateByThreadNEXT==ThreadFun_NONE) 	
	{	//沒有執行其他指令 才能查詢
		m_RobotFunStateByThread=ThreadFun_RequationRobotPower;
		return true;
	}
	return false;
}

bool CallThread_RobotSchedule()
{
	if(m_RobotFunStateByThread==ThreadFun_NONE ) 	
	{	//沒有執行其他指令 才能查詢
		m_RobotFunStateByThread=ThreadFun_RequationRobotSchedule;
		return true;
	}
	return false;
}

//查詢機器名稱
bool CallThread_GetRobotName()
{
	if(m_RobotFunStateByThread==ThreadFun_NONE) 
	{
		m_RobotFunStateByThread=ThreadFun_RequationRobotName;
		return true;
	}
	return false;
}
//嘗試連接ROBOT
static void* ConnectRobotThrFun(void* data)
{
	m_RobotFunStateByThread=ThreadFun_NONE;
    while (true)
	{
		// create autorelease pool for iOS
//		CCThread thread;
//		thread.createAutoreleasePool();
		switch(m_RobotFunStateByThread)
		{
		case ThreadFun_SearchAP:
			m_pUIRobotSacnning->WifiEnum(); //列出所有AP
			m_RobotFunStateByThread=ThreadFun_NONE;
			break;
		case ThreadFun_ConnectAP: //連接ap
#if ROBOT
            {
			int iResult=GFUN_WifiConnectAP(m_pUIRobotSacnning->GetSelApIdx());
			if(iResult!=ERROR_SUCCESS) //失敗重新連結
			{
				m_pUIRobotSacnning->ConnectFailSetting(CONNECT_Fail_NoAP);
				m_RobotFunStateByThread=ThreadFun_NONE;
			}
			else //成功往下跑
			{
				m_RobotFunStateByThread=ThreadFun_WaitAPGiveIP;
				iRetryTime=0;
			}
            }
#else
            m_RobotFunStateByThread=ThreadFun_ConnectRobot;
    
#endif
			break;
		case ThreadFun_WaitAPGiveIP: // 等待AP 給IP (每5秒檢查一次)
			
#if ROBOT
            {
				iRetryTime++;
				if(GFUN_WifiAPCheck(m_pUIRobotSacnning->GetSelApIdx()))
				{
					m_RobotFunStateByThread=ThreadFun_ConnectRobot;
				}					
				else
				{
					//WaitForSingleObject(m_WifiScan.EventWait,5000); //等待5秒 讓WINDOWS 來得及反映(日後改為偵測IP)
					GFUN_WaitWifiEvenSingle(3000);
					//if(iRetryTime>5) //超過25秒
					{					
						if(GFUN_WifiAPCheck(m_pUIRobotSacnning->GetSelApIdx())) //最後檢查
						{
							m_RobotFunStateByThread=ThreadFun_ConnectRobot;


						}
						else
						{
							if(iRetryTime>3) //失敗3次
							{
								m_pUIRobotSacnning->ConnectFailSetting(CONNECT_Fail_NoIP);
								//GFUN_WifiDisconnectAP(m_pUIRobotSacnning->GetSelApIdx()); //連線失敗 先強制斷線
								m_RobotFunStateByThread=ThreadFun_NONE;

							}
						}
					}

				}
				//MapProxy->GetCellIndex(iResult,2);
				//MapProxy->GetDataTime();

			}
#else
     m_RobotFunStateByThread=ThreadFun_NONE;
#endif
			break;
  		case ThreadFun_ConnectRobot:
			{				
//				int iLinkState=m_pUIRobotSacnning->ConnectRobot(0); //連接robot
//				if(iLinkState!=1) // ==1連接成功
//				{//連線失敗
//					//	m_WifiScan.WifiDisconnectAP(m_pUIRobotSacnning->GetSelApIdx()); //連線失敗 先強制斷線
//                    
//				}
				
				m_RobotFunStateByThread=ThreadFun_NONE;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				if(iLinkState==0) // ==1連接成功
				{
					
					m_pUIRobotMain->CheckRobotTime(); //取時間
				}
#endif
			}			
			break;
		case ThreadFun_RequationRobotTime:
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				CCLog("send command to get robot time");
				int iResult=m_pUIRobotSacnning->GetRobotTime(); //會卡住 不往下跑 直到有回應為止
				if(iResult==RobCmdSt_OK)
				{
					m_pUIRobotMain->RobotTimeOk();
				}
				else 
				{//發生異常
					CCLog("Fail by get robot time");
					m_pUIRobotMain->RobotTimeFail();
				}
#endif
				m_RobotFunStateByThread=ThreadFun_NONE;
			}
			break;
		case ThreadFun_RequationRobotName:
			{
				char *pName=NULL;
				_RobotCommandState_ bState=RobCmdSt_Unknow;
				log("send command to get robot name");
				bState=m_pUIRobotSacnning->GetRobotName(&pName); //會卡住 不往下跑 直到有回應為止

				m_RobotFunStateByThread=ThreadFun_NONE;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				if(bState== RobCmdSt_OK && pName!=NULL ) 
				{ //正確拿到機台名稱
					m_pUIRobotMain->SaveRobotName(pName);
					m_pUIRobotMain->RobotNameOk();
				}
				else
				{
					m_pUIRobotMain->RobotNameFail();
				}
#endif
			}
			break;
		case ThreadFun_RequationRobotState:
			{
				log("send command to get robot state");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				AI_STATE AIStates;
				int iResult=m_pUIRobotSacnning->GetRobotState(AIStates.AIStatus,AIStates.CleanStatus,AIStates.Error,AIStates.Worry); //會卡住 不往下跑 直到有回應為止
				if(iResult==RobCmdSt_OK ) 
				{
					m_pUIRobotMain->SaveRobotState(AIStates);
					m_pUIRobotMain->RobotStateOk();
				}
				else
				{
					m_pUIRobotMain->RobotStateFail();
				}
#endif
				m_RobotFunStateByThread=ThreadFun_NONE;
			}
			break;
		case ThreadFun_RequationRobotPower:
			{
				log("send command to get robot power");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				int iResult=m_pUIRobotSacnning->GetRobotPower();
				if(iResult==RobCmdSt_OK ) 
				{
					m_pUIRobotMain->SaveRobotPower(m_pUIRobotSacnning->GetRobotPowerValue());
					m_pUIRobotMain->RobotPowerOk();
				}
				else
				{
					m_pUIRobotMain->RobotPowerFail();
				}
#endif
				m_RobotFunStateByThread=ThreadFun_NONE;
			}
			break;
		case ThreadFun_RequationRobotSchedule: //取得現在預約行程
			{
				log("send command to get robot Schedule");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				int iResult=m_pUIRobotSacnning->GetRobotSchedule();
				if(iResult==RobCmdSt_OK ) 
				{
					//m_pUIReservation->SaveRobotPower(m_pUIRobotSacnning->GetRobotPowerValue());
				}
				else
				{
				}
#endif
				m_RobotFunStateByThread=ThreadFun_NONE;
			}
			break;
//        case ThreadFun_NONE:
//                sleep(1);
//                break;
		}

		//指定空閒之後的下一個指令
		if(m_RobotFunStateByThreadNEXT && m_RobotFunStateByThread==ThreadFun_NONE)
		{
			m_RobotFunStateByThread=m_RobotFunStateByThreadNEXT;
			m_RobotFunStateByThreadNEXT=ThreadFun_NONE;
		}
	}
}

bool IsRobotThreadIdle()
{
	if(m_RobotFunStateByThread==ThreadFun_NONE)

		return true;
	return false;
}
void SetRobotFun_ConnectAp()
{
	m_RobotFunStateByThread=ThreadFun_ConnectAP;
}
void RobotConnectThread_Cread()
{
	auto retState=pthread_create(&s_ConnectRobotThread, NULL, ConnectRobotThrFun, NULL);
    if(retState!=0)
    {
        log("Creat pthread fail!!!!");
    }
}
static int iWindowsSizeIDx=0;

void SetWindwsSizeIdx(int iNum)
{
    iWindowsSizeIDx=iNum;
}
int GetWindwsSizeIdx()
{
    return iWindowsSizeIDx;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
int RelaseRobotTime(void* pdata)
{
	player_dispatcher_time_t *pLastTime=(player_dispatcher_time_t *)pdata;
	int iResult=0;
	if(pLastTime->tm_hour!= RobotTime.tm_hour || pLastTime->tm_min!= RobotTime.tm_min)
	{
		pLastTime->tm_hour = RobotTime.tm_hour;
		pLastTime->tm_min  = RobotTime.tm_min;
		iResult=1;
	}

	if(pLastTime->tm_year!= RobotTime.tm_year || pLastTime->tm_mon!= RobotTime.tm_mon|| pLastTime->tm_mday!= RobotTime.tm_mday)
	{
		pLastTime->tm_year = RobotTime.tm_year;
		pLastTime->tm_mon  = RobotTime.tm_mon;
		pLastTime->tm_mday = RobotTime.tm_mday;
		iResult=2;
	}

	return iResult;
}


//////////////////////////////////////////////////////////////////////////
//儲存Reservation設定檔

CIni WinSetting;
Setting RobotSetting;
void InitWinIni()
{

	LPTSTR lpBuffer=NULL;
	GetCurrentDirectory(MAX_PATH,sExePath); //取得現在執行檔目錄
	wsprintf(sIniFileName,L"%s\\RobotSetting.ini",sExePath);
	WinSetting.SetPathName(sIniFileName);
	int iResutle=0;
	iResutle=WinSetting.GetSectionNames(lpBuffer,1024);
	memset((void*)&RobotSetting,0,sizeof(Setting));

	RobotSetting.TipMsg.TimeSynTip=1;		//預設值

	if(iResutle<=2) //沒有此檔案
	{
		SaveSetting_All(); //將預設檔存為現在檔案
	}
	else
	{
		LoadSetting_All();//讀取檔案 進入設定
	}

	
}
void SaveSetting_Reservation()
{
	const TCHAR sSecions[]=L"Reservation";
	WinSetting.WriteInt(sSecions,L"DailyTime",RobotSetting.Reser.DailyTime);
	WinSetting.WriteInt(sSecions,L"DailyOn",RobotSetting.Reser.DailyOn);
	WinSetting.WriteInt(sSecions,L"SingleTime",RobotSetting.Reser.SingleTime);
	WinSetting.WriteInt(sSecions,L"SingleOn",RobotSetting.Reser.SingleOn);
}
void SaveSetting_TipMsg()
{
	const TCHAR sSecions[]=L"TipMsg";
	WinSetting.WriteInt(sSecions,L"TimeSyn",RobotSetting.TipMsg.TimeSynTip);
	WinSetting.WriteInt(sSecions,L"CancelJob",RobotSetting.TipMsg.CancelJobTip);

}

//讀取Reservation設定檔
void LoadSetting_Reservation()
{
	const TCHAR sSecions[]=L"Reservation";
	RobotSetting.Reser.DailyTime = WinSetting.GetInt(sSecions,L"DailyTime",RobotSetting.Reser.DailyTime);
	RobotSetting.Reser.DailyOn = WinSetting.GetInt(sSecions,L"DailyOn",RobotSetting.Reser.DailyOn);
	RobotSetting.Reser.SingleTime = WinSetting.GetInt(sSecions,L"SingleTime",RobotSetting.Reser.SingleTime);
	RobotSetting.Reser.SingleOn = WinSetting.GetInt(sSecions,L"SingleOn",RobotSetting.Reser.SingleOn);
}
void LoadSetting_TipMsg()
{
	const TCHAR sSecions[]=L"TipMsg";
	RobotSetting.TipMsg.TimeSynTip = WinSetting.GetInt(sSecions,L"TimeSyn",RobotSetting.TipMsg.TimeSynTip);
	RobotSetting.TipMsg.CancelJobTip = WinSetting.GetInt(sSecions,L"CancelJob",RobotSetting.TipMsg.CancelJobTip);
}

//儲存設定檔
void SaveSetting_All()
{
	SaveSetting_Reservation();
	SaveSetting_TipMsg();
}
//讀取設定檔
void LoadSetting_All()
{
	LoadSetting_Reservation();
	LoadSetting_TipMsg();
}
//取得現在設定檔
void SaveSetting(int iLayerType,void* pData)  
{
	switch (iLayerType)
	{
	default:
		break;
	case Setting_Reser:
		memcpy(&RobotSetting.Reser,pData,sizeof(Setting_Reservation));
		break;
	}
}
void* GetSetting(int iLayerType)  //取得現在設定檔
{
	switch (iLayerType)
	{
	default:
		break;
	case Setting_Reser:
		return (void*)&RobotSetting.Reser;
		break;
	case Setting_TipMsg:
		return (void*)&RobotSetting.TipMsg;
		break;
	}
	return NULL;
}
int CheckSyncTimeAlarmSetting()
{
	return RobotSetting.TipMsg.TimeSynTip;
}
void SetSyncTimeAlarmSettingONOFF(int bOnOff)
{
	RobotSetting.TipMsg.TimeSynTip=bOnOff;
}
int CheckCheckCJobAlarmSetting()
{
	return RobotSetting.TipMsg.CancelJobTip;
}
void SetCleanJobAlarmSettingONOFF(int bOnOff)
{
	RobotSetting.TipMsg.CancelJobTip=bOnOff;
}
//////////////////////////////////////////////////////////////////////////


void InitRobotTime()
{
	memset(&RobotTime,0,sizeof(player_dispatcher_time_t));
	memset(&RobotZone,0,sizeof(player_dispatcher_timezone_t));

}
void GDisProxy_ReqUiTime()
{
pDisProxy->ReqUiTime(PLAYER_DISPATCH_INIT_DEST_RD, &RobotTime, &RobotZone);
}

void GDisProxy_SetUiTime()
{
	pDisProxy->SetUiTime(PLAYER_DISPATCH_INIT_DEST_RD, RobotTime, RobotZone);
}
//取得現在電腦時間 並且轉化成分鐘(0 ~ 24*60-1)
int GetPcTimeMinuate()
{
	struct tm *tm;
	int iTimeMinuate=0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	//取得系統時間	
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
#else
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL); 
	long millSecond=now.tv_sec * 1000 + now.tv_usec / 1000;	
	tm = localtime(&now.tv_sec);	
#endif
	iTimeMinuate=tm->tm_hour*60+tm->tm_min;
	return iTimeMinuate;
}
//取得現在機器時間 並且轉化成分鐘(0 ~ 24*60-1)
int GetRobotTimeMinuate()
{
	int iTimeMinuate=0;
	iTimeMinuate=RobotTime.tm_hour*60+RobotTime.tm_min;
	return iTimeMinuate;
}

void SavePcTimeToRobotTime()
{
	struct tm *tm;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
//取得系統時間	
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
#else
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL); 
	long millSecond=now.tv_sec * 1000 + now.tv_usec / 1000;	
	tm = localtime(&now.tv_sec);	
#endif
	RobotTime.tm_year = tm->tm_year + 1900;
	RobotTime.tm_mon = tm->tm_mon + 1;
	RobotTime.tm_mday = tm->tm_mday;
	RobotTime.tm_hour =tm->tm_hour;
	RobotTime.tm_min =tm->tm_min;
	RobotTime.tm_sec =tm->tm_sec;
	GDisProxy_SetUiTime(); //系統同步時間

}
void * GetRobotTime()
{
	return (void*)&RobotTime;
}
void * GetRobotZone()
{
	return (void*)&RobotZone;
}
//檢查系統時間跟現在時間差異!
//如果超過兩分鐘就回傳 0;
//如果沒有超過兩分鐘就回傳 1;
int CheckPcTimeAndRobotTime()
{ //差異超過2分鐘 就要詢問是否同步
	struct tm tmRobot;
	time_t timeRobot,timePC;
	//轉化ROBOT 時間
	tmRobot.tm_year = RobotTime.tm_year -1900;
	tmRobot.tm_mon =RobotTime.tm_mon - 1;
	tmRobot.tm_mday = RobotTime.tm_mday ;
	tmRobot.tm_hour = RobotTime.tm_hour ;
	tmRobot.tm_min = RobotTime.tm_min ;
	tmRobot.tm_sec = RobotTime.tm_sec ;
	tmRobot.tm_isdst=0;
	timeRobot = mktime(&tmRobot);
	if(timeRobot==-1) //尚未取得時間
		return -1;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	//取得系統時間	
	time(&timePC);
	//tmPc = localtime(&timep);
#else
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL); 
	long millSecond=now.tv_sec * 1000 + now.tv_usec / 1000;	
	tmPc = localtime(&now.tv_sec);	
#endif
	//計算差值
	double dSec=difftime(timeRobot,timePC);
		 
	if(dSec<120 && dSec>-121 ) //超過1分鐘
		return 0;
	return 1;

}

void * GetSchedulTime(int iType)
{
	struct tm *ptmTotal,tmRobot;//tmSchedule;
	double dSec=0;
	time_t timeRobot,timeTotal;//timeSchedule
	//轉化ROBOT 時間
	tmRobot.tm_year = RobotTime.tm_year -1900;
	tmRobot.tm_mon =RobotTime.tm_mon - 1;
	tmRobot.tm_mday = RobotTime.tm_mday ;
	tmRobot.tm_hour = RobotTime.tm_hour ;
	tmRobot.tm_min = RobotTime.tm_min ;
	tmRobot.tm_sec = RobotTime.tm_sec ;
	tmRobot.tm_isdst=0;
	//memset(&tmSchedule,0,sizeof(tmSchedule));	
	if(iType==RESPGALARM_ONCE)
	{
		//tmSchedule.tm_hour=RobotSetting.Reser.SingleTime/60;
		//tmSchedule.tm_min=RobotSetting.Reser.SingleTime%60;		
		dSec=RobotSetting.Reser.SingleTime*60;
	}
	else
	{
		//tmSchedule.tm_hour=RobotSetting.Reser.DailyTime/60;
		//tmSchedule.tm_min=RobotSetting.Reser.DailyTime%60;		
		dSec=RobotSetting.Reser.DailyTime*60;

	}
	timeRobot = mktime(&tmRobot);
	//timeSchedule = mktime(&tmSchedule);
	timeTotal=timeRobot+dSec;
	//timeTotal=timeRobot+60;
	ptmTotal = localtime(&timeTotal);
	player_dispatcher_time_t SacheduleTime;
	SacheduleTime.tm_year  = ptmTotal ->tm_year  + 1900;
	SacheduleTime.tm_mon  = ptmTotal->tm_mon + 1;
	SacheduleTime.tm_mday = ptmTotal->tm_mday  ;
	SacheduleTime.tm_hour = ptmTotal->tm_hour ;
	SacheduleTime.tm_min = ptmTotal->tm_min  ;
	SacheduleTime.tm_sec = ptmTotal->tm_sec ;
	return &SacheduleTime;

}
#endif