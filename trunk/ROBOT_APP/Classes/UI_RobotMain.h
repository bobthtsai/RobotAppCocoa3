#ifndef __UIROBORMAIN_H__
#define __UIROBORMAIN_H__
#include "cocos2d.h"
#include <vector>
#include <sstream>
using namespace cocos2d;
using namespace std;
enum _MAINPGETYPE_
{
	MAINPG_STATE,
	MAINPG_INFOAllRIGHT,
	MAINPG_INFOWARNING,
	MAINPG_SIZE, //頁面數量
	MAINPG_LAYER //無顯示頁面
};
enum _MAINSTATEITEM_
{
	MSTAET_BK=0,
	MSTAET_ICON,
	MSTAET_LABNAME,	
	MSTAET_LABDATE,
	MSTAET_LABTIME,
	MSTAET_LABALARM,
	MSTAET_ICONALARM,
	MSTAET_LABPOWER,
	MSTAET_BTNRESERVATION,
	MSTAET_BTNJOB,
	MSTAET_BTNREMOTE,
	MSTAET_BTNMAP,
	MSTAET_BTNSELBK,
	MSTAET_BTNSETUP,
	MSTAET_SIZE,
	MSTAET_BTN1=0,
	MSTAET_BTN2,
	MSTAET_BTN3,
	MSTAET_BTN4,
	MSTAET_BTNSize

};
enum _MAININFOALLRIGHTITEM_
{
	MINFORI_BK=0,
	MINFORI_TITAL,
	MINFORI_ICON,	
	MINFORI_SIZE
};
enum _MAININFOWARNINGITEM_
{
	MINFOWA_BK=0,
	MINFOWA_TITAL,
	MINFOWA_ICON,	
	MINFOWA_BUTTON,
	MINFOWA_SIZE
};
enum _ROBOTMAPSTATES_
{
	RobMap_Empty=0,
	RobMap_Ready,
	RobMap_Release,	
	RobMap_TimeWait,
	RobMap_GetCommand,
};

enum _ROBOTNAMESTATES_
{
	RobName_Empty=0,
	RobName_Wait,
	RobName_Get,	
	RobName_Fail,
};
//ROBOT 初次資訊FLAG
//如果還沒有拿到 會隨時更新
enum _ROBOTOINOFFLAG_
{
	RobFlag_Name=0,
	RobFlag_Time,
	RobFlag_State,
	RobFlag_Power,
	RobFlag_Schedule,
	RobFlag_All,	//判斷是否所有資訊皆已經拿到
	RobFlag_Number,
};
enum _ROBOTFLAGSTATES_
{
	RobFlagSt_None=0,
	RobFlagSt_Release,
	RobFlagSt_Wait,
	RobFlagSt_Get,	
	RobFlagSt_Fail,
	RobFlagSt_ReleaseOver,
	RobFlagSt_Error,
	RobFlagSt_Retry,
};
class UI_RobotMain: public CCLayer
{
private:
	int PageType;
	//bool bGetTimeNoError;
	//int  iRobotNameState;
	void InitUI();
	void InitUI_MainPage();
	void InitUI_RobotInfoAllRight();
	void InitUI_RobotInfoWarning();
	void PressSpriteMove(int iBtnIdx);	//按鈕底色位置

	CCSize winSize;		//視窗大小
	//主要函數 回傳是否找到，並且將得到資料記錄下來
	std::vector<std::string> SSIDName;
	std::vector<std::wstring> APName;
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
private:  
	CCArray* m_BtnArray[MAINPG_SIZE];//存放Scan page所有按鈕
	void update(float dt);  
	void ReleseRobotState(float dt); 
	float fPowerPart; //剩餘電量
	string sRobotName;
	bool bShowMSG_TimeSync; // 是否有開啟時間同步警告視窗
	bool bShowMSG_EvenOpen; // 是否有開啟時間同步警告視窗
	bool bTimeSyncEven;		// 是否有檢查過同步
	//bool bRelesesRobotName;
	//int iRobotStateNumber;
	AI_STATE stRobotState,stOldRobotState;
	
	int RobotFlag[RobFlag_Number];	//用於thread 傳輸指令flag
	int FailCount[RobFlag_Number];
public:
	CREATE_FUNC(UI_RobotMain); 
	virtual bool init();  
	UI_RobotMain();
	~UI_RobotMain();
	void ShowLayer();
	void HideLayer();	
	void Hide_AllPage();
	void Function_RobotMain(int iBtnIdx);
	void Function_RobotInfoAllright(int iBtnIdx);
	void Function_RobotInfoWarning(int iBtnIdx);
	void Show_MainPage();
	void Hide_MainPage();
	void Show_InfoAllRight();
	void Hide_InfoAllRight();
	void Show_InfoWarning();
	void Hide_InfoWarning();
	//畫面更新指令類
	void ReleaseLab_RobotName();
	void ReleaseLab_RobotState();
	void ReleaseLab_RobotDateTime();
	void ReleaseLab_UpdateDateTime(void *pDate); //更新時間跟日期
	void ReleaseLab_RobotDate();
	void ReleaseLab_RobotTime();
	void ReleaseIcon_RobotState();
	void ReleaseLab_RobotPower();
	//void ReleaselastRobotTime();
	//下達不同的更新指令
	void UpdateRobotByReleaseCommad(int i);
	// 取得ROBOT 時間	
	
//	void SetRobotTimeFailToGet();
//	void CleanRobotTimeFailToGet();
	//Robot flag 設定(用於決定是否要自動指令)
	void RobotFlagClean(int iType);
	void RobotFlagOk(int iType);
	void RobotFlagFail(int iType);
	void RobotFlagRetry(int iType);
	//修改ROBOT 名稱	
	void RobotNameOk();
	void RobotNameFail();
	void RobotNameClean();
	void RobotNameRetry();
	void RobotTimeOk();
	void RobotTimeFail();
	void RobotTimeClean();
	void RobotTimeRetry();
	void RobotStateOk();
	void RobotStateFail();
	void RobotStateClean();
	void RobotStateRetry();
	void RobotPowerOk();
	void RobotPowerFail();
	void RobotPowerClean();
	void RobotPowerRetry();
	void SaveRobotName(char* pName);
	void SaveRobotState(AI_STATE AIStates);
	void SaveRobotPower(float fValue);
	// 檢察現在所有機台狀態	
	bool CheckRobotName();//檢查現在名稱狀態!決定是否要傳送指令
	bool CheckRobotTime();//檢查現在時間狀態!決定是否要傳送指令
	bool CheckRobotState();//檢查現在機台狀態!決定是否要傳送指令
	bool CheckRobotPower();//檢查現在電量狀態!決定是否要傳送指令
	bool CheckRobotSchedule(); //取得現在安排預約行程表
	const char* GetRobotName(); //回傳機台名稱
	void ShowTimeSyncBox();	
	void MsgBoxTimeSync(cocos2d::CCNode *pNode);// 時間同步
	void CheckRobtoTimeSync(); //檢查是否需要時間同步
};
bool IsRobotMapReady();
void RobotMapUpdate();//地圖更新指令
void WaitRobotMapUpdate(); //等待地圖初次更新
int WaittryRobotMapUpdate();//等待地圖初次更新
#endif //__UIROBORMAIN_H__