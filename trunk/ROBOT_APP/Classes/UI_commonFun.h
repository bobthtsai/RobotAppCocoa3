//Headers for WLAN SCAN
#ifndef __UICOMMONFUN_H__
#define __UICOMMONFUN_H__
#define G_ROBOTMAP_RELEASE_TIME 5.0f	//  地圖每幾秒更新一次
#define G_ROBOTSTATE_RELEASE_TIME 5.0f	//  畫面每幾秒更新一次
//set maximum request time here. once request time out of this value, exception occur.
//default is 5 seconds.
#define MAX_REQUEST_TIME 1
#define  G_ROBOT_SCHEDULE_MAX  100		//最多可以備份機台行程數量
#define G_SCHEDULE_JOBNNME "Auto mode"	//行程預約固定的名稱
//#define G_ROBOTMAP_UPDATE_TIME 3	//  地圖每幾秒與機器連接更新 (10 秒為一單位)

enum __LayerType_
{
	Layer_Logo=0,
	Layer_Main,
	Layer_Scanning,
	Layer_Remote,
	Layer_Map,
	Layer_CleanJob,
	Layer_Reservation,
	Layer_RobotSetup,
};
void WifiSearchIcon_AddSpeed();//搜尋圖示旋轉加快
void HideLayer_All(); //隱藏所有主頁面
void ChangLayer_Main();//控制頁面切換
void ChangLayer_Scanning();
void ChangLayer_Remote();
void ChangLayer_Map();
void ChangLayer_CleanJob();
void ChangLayer_Reservation();
void ChangLayer_RobotSetup();



struct Setting_Reservation
{
	int SingleOn;
	int SingleTime;
	int DailyOn;
	int DailyTime;
};
struct Setting_TipMsg
{
	int TimeSynTip;
	int CancelJobTip;
};
struct Setting
{
	Setting_Reservation Reser;
	Setting_TipMsg TipMsg;
};
enum _SettingType_
{
	Setting_Reser=0,
	Setting_TipMsg
};
struct AI_STATE
{
	unsigned int AIStatus;
	unsigned int CleanStatus;
	unsigned int Error;
	unsigned int Worry;
};

enum _ThreadEven_
{
	ThreadFun_NONE=0,
	ThreadFun_SearchAP,
	ThreadFun_ConnectAP,
	ThreadFun_WaitAPGiveIP,
	ThreadFun_ConnectRobot,
	ThreadFun_RequationRobotTime,
	ThreadFun_RequationRobotName,
	ThreadFun_RequationRobotState,
	ThreadFun_RequationRobotPower,
	ThreadFun_RequationRobotSchedule,

};
enum _RobotLinkState_
{
	RobSt_OK=0,
	RobSt_NewFail,
	RobSt_ConnectFail,
	RobSt_NoProxyFail,
	RobSt_UnknowFail,
};
enum _RobotCommandState_
{
	RobCmdSt_OK=0,
	RobCmdSt_Nopoint,
	RobCmdSt_Timeout,
	RobCmdSt_Unknow,
};

enum _RESERVATION_SCHEDULETYPE_
{
	RESPGALARM_ONCE=0, // 預約種類
	RESPGALARM_DAILY,
	RESPGALARM_SIZE

};

void RobotConnectThread_Cread();
bool CallThread_GetRobotTime();
void CallThread_SearchRobot();
bool CallThread_RobotSchedule(); //取得現在安排的預約時間
bool CallThread_GetRobotName();//查詢機器名稱
bool CallThread_GetRobotState();
bool CallThread_GetRobotPower();
void SetWindwsSizeIdx(int iNum);
int GetWindwsSizeIdx();
void SaveSetting_All(); //儲存設定檔
void LoadSetting_All(); //讀取設定檔
void SaveSetting_Reservation(); //儲存Reservation設定檔
void LoadSetting_Reservation(); //讀取Reservation設定檔
void SaveSetting_TipMsg(); 
void LoadSetting_TipMsg();
void* GetSetting(int iSettingType);  //取得現在設定檔
void SaveSetting(int iSettingType,void* pData);//寫入現在設定檔
int CheckSyncTimeAlarmSetting(); // 檢查是否需要顯示同步警告
void SetSyncTimeAlarmSettingONOFF(int bOnOff);
int CheckCheckCJobAlarmSetting();
void SetCleanJobAlarmSettingONOFF(int bOnOff);
void SetRobotFun_ConnectAp();
bool IsRobotThreadIdle();
int RelaseRobotTime(void* pdata);
void InitRobotTime();
void GDisProxy_ReqUiTime();
void InitWinIni();

void GDisProxy_SetUiTime();
void SavePcTimeToRobotTime();
int GetPcTimeMinuate();	//取得系統時間 並且轉化成分鐘
int GetRobotTimeMinuate();
//WIFI SCAN 公用函數
int GFUN_WifiConnectAP(int iIdx);
int GFUN_WifiAPCheck(int iIdx);
int GFUN_WifiDisconnectAP(int iIdx);
void GFUN_WaitWifiEvenSingle(int iTime);
void GFUN_FreeWifiScanMemory();
void GFUN_CreadWifi();
int CheckPcTimeAndRobotTime();
void * GetRobotTime(); //取得現在系統時間
void * GetRobotZone();
void * GetSchedulTime(int iType);
#endif __UICOMMONFUN_H__
