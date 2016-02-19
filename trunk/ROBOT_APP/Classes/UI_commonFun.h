//Headers for WLAN SCAN
#ifndef __UICOMMONFUN_H__
#define __UICOMMONFUN_H__
#define G_ROBOTMAP_RELEASE_TIME 5.0f	//  �a�ϨC�X���s�@��
#define G_ROBOTSTATE_RELEASE_TIME 5.0f	//  �e���C�X���s�@��
//set maximum request time here. once request time out of this value, exception occur.
//default is 5 seconds.
#define MAX_REQUEST_TIME 1
#define  G_ROBOT_SCHEDULE_MAX  100		//�̦h�i�H�ƥ����x��{�ƶq
#define G_SCHEDULE_JOBNNME "Auto mode"	//��{�w���T�w���W��
//#define G_ROBOTMAP_UPDATE_TIME 3	//  �a�ϨC�X��P�����s����s (10 ���@���)

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
void WifiSearchIcon_AddSpeed();//�j�M�ϥܱ���[��
void HideLayer_All(); //���éҦ��D����
void ChangLayer_Main();//���������
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
	RESPGALARM_ONCE=0, // �w������
	RESPGALARM_DAILY,
	RESPGALARM_SIZE

};

void RobotConnectThread_Cread();
bool CallThread_GetRobotTime();
void CallThread_SearchRobot();
bool CallThread_RobotSchedule(); //���o�{�b�w�ƪ��w���ɶ�
bool CallThread_GetRobotName();//�d�߾����W��
bool CallThread_GetRobotState();
bool CallThread_GetRobotPower();
void SetWindwsSizeIdx(int iNum);
int GetWindwsSizeIdx();
void SaveSetting_All(); //�x�s�]�w��
void LoadSetting_All(); //Ū���]�w��
void SaveSetting_Reservation(); //�x�sReservation�]�w��
void LoadSetting_Reservation(); //Ū��Reservation�]�w��
void SaveSetting_TipMsg(); 
void LoadSetting_TipMsg();
void* GetSetting(int iSettingType);  //���o�{�b�]�w��
void SaveSetting(int iSettingType,void* pData);//�g�J�{�b�]�w��
int CheckSyncTimeAlarmSetting(); // �ˬd�O�_�ݭn��ܦP�Bĵ�i
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
int GetPcTimeMinuate();	//���o�t�ήɶ� �åB��Ʀ�����
int GetRobotTimeMinuate();
//WIFI SCAN ���Ψ��
int GFUN_WifiConnectAP(int iIdx);
int GFUN_WifiAPCheck(int iIdx);
int GFUN_WifiDisconnectAP(int iIdx);
void GFUN_WaitWifiEvenSingle(int iTime);
void GFUN_FreeWifiScanMemory();
void GFUN_CreadWifi();
int CheckPcTimeAndRobotTime();
void * GetRobotTime(); //���o�{�b�t�ήɶ�
void * GetRobotZone();
void * GetSchedulTime(int iType);
#endif __UICOMMONFUN_H__
