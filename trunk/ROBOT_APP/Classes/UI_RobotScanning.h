#ifndef __UIROBORSCANNING_H__
#define __UIROBORSCANNING_H__
#include "cocos2d.h"
#include <vector>
#include <sstream>
using namespace cocos2d;
using namespace std;

static const int TAG_UI_UIROBORSCANNING_Bk =10;
static const int MAX_DISPLAY_WIFINAME =6;
enum _SCANITEM_
{
	UISCAN_BK=0,
	UISCAN_TOPBTN,
	UISCAN_ICON,					
	UISCAN_DOWNBTN,
	UISCAN_SIZE
};
enum _NODEVICEITEM_
{
	UINODEVICE_BK=0,
	UINODEVICE_TOPBTN,
	UINODEVICE_ICON,

	UINODEVICE_DOWNBTN,
	UINODEVICE_REPORTLINK,
	UINODEVICE_SIZE
};
enum _WIFILISTITEM_
{
	UIWIFILIST_BK=0,
	UIWIFILIST_TOPBTN,
	UIWIFILIST_DOWNBTN,
	UIWIFILIST_REPORTLINK,
	UIWIFILIST_WIFILIST,	//大於此項目 皆為AP
	UIWIFILIST_WIFILISTMAX=UIWIFILIST_WIFILIST+MAX_DISPLAY_WIFINAME, // 最多六組

};
enum _CONNECTANIITEM_
{
	CONNECTANI_BK=0,
	CONNECTANI_ICON,
	CONNECTANI_Name,
	CONNECTANI_SSID,
	CONNECTANI_SIZE
};
enum _ERRCONNFAILITEM_
{
	ERRCONNFAIL_BK=0,
	ERRCONNFAIL_ICON,
	ERRCONNFAIL_REPORTLINK,
	ERRCONNFAIL_CANCEL,
	ERRCONNFAIL_RETRY,
	ERRCONNFAIL_SIZE
};
enum _SCANPAGETYPE_
{
	SCANPG_SEARCH,
	SCANPG_ERR_NODEVICE,
	SCANPG_WIFILIST,
	SCANPG_CONNECTANI,
	SCANPG_ERR_CONNFAIL,
	SCANPG_SIZE,
	SCANPG_LAYER
};

enum _SEARCHTYPE_
{
	Search_None,
	Search_Loop,	
	Search_APFail,
	Search_APSuccess,
	Search_APReady,
};
enum _CONNECTTYPE_
{
	CONNECT_Ready,
	CONNECT_WaitIP,
	CONNECT_WaitRobotAck,
	CONNECT_Success,
	CONNECT_Fail_NoAP,
	CONNECT_Fail_NoIP,
	CONNECT_Fail_NoRobotAck,
	CONNECT_Fail_NoProxy,
	CONNECT_Fail_NoRobotName,
	//Search_ConnectRobot,
	//Search_ConnectRobotLoop,
	//Search_ConnectRobotSuccess,
	//Search_ConnectRobotFail,
};





class UI_RobotScanning: public CCLayer
{
private:
	//int m_RobotFunState;		//連線狀態
	int PageType;
	int WifiList_DrawLineNum;	//畫線數量
	int WifiList_StartIdx;		//起始AP
	int WifiList_SelAPIdx;		//AP選項
	int iIconSpeed_Search;	//Icon 旋轉速度
	int iIconSpeed_Connect;	//Icon 旋轉速度
	int m_SearchType;			//搜尋狀態
	float RobotPowerValue;	 //電池電量
	bool MsgBoxShow_RobotTimeFailGet;	//顯示警告視窗(時間同步錯誤)
	bool MsgBoxShow_RobotStateFailGet;	//顯示警告視窗(狀態讀取錯誤)
	bool MsgBoxShow_RobotPowerFailGet;	//顯示警告視窗(狀態讀取錯誤)	
	CCSize winSize;		//視窗大小
	//主要函數 回傳是否找到，並且將得到資料記錄下來
	std::vector<std::string> SSIDName;
	std::vector<std::wstring> APName;
	std::vector<int> iLinkReady;
	//std::vector<int> NetworkLinkIndex;

private:  

	CCArray* m_BtnArray[SCANPG_SIZE];//存放Scan page所有按鈕
	CCArray* m_WifiListSSIDName;//存放所有WIFI SSID name string
	CCArray* m_WifiListSSIDLabel;//存放所有WIFI SSID  name Label
	CCArray* m_WifiListAPName;//存放所有WIFI AP name string
	CCArray* m_WifiListAPLabel;//存放所有WIFI AP name Label
	void InitUI();
	void InitUI_ScaningPage();
	void InitUI_ErrorNoDevice();
	void InitUI_WifiList();
	void InitUI_ConnectAni();
	void InitUI_ErrConnFail();
	virtual void registerWithTouchDispatcher();
	virtual void removeWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	void update(float dt);  
	void PAGEANI_SEARCH(); //頁面動畫
	void PAGEANI_CONNECT(); //頁面動畫
	
public:
	CREATE_FUNC(UI_RobotScanning); 
	virtual bool init();  
	UI_RobotScanning();
	~UI_RobotScanning();
	void ShowLayer();
	void HideLayer();

	void Show_SacnPage();
	void Hide_SacnPage();
	void Show_ErrNoDevice();
	void Hide_ErrNoDevice();
	void Show_WifiList();
	void Hide_WifiList();
	void Reload_WifiList();
	void Show_ConnectAni();
	void Hide_ConnectAni();
	void Show_ErrConnFail();
	void Hide_ErrConnFail();

	void Hide_AllPage();
	void Function_ErrNoDevice(int iBtnType);
	void Function_WifiList(int iBtnType);
	void Function_ErrCopnnFail(int iBtnType);
	void WifiEnum();
	void WifiListLabelLoad();
	void WifiListLabelRemove();
	void WifiListLabelDisplay(int iStart);
	void WifiListLabelHide();
	void WifiLabelUpDown(CCPoint MousePoint);
	void ConnectRobotStart(int iBtnNum);
	void ConnectRobotFun();
	void SearchIconStop();//圖示旋轉快速
	void SearchIconFast();//圖示旋轉快速
	void SearchIconSlow();//圖示旋轉緩慢
	void ChangConnectIconSpeed();//圖示旋轉速度改變	
	void ConnectFailSetting(int iType); //失敗原因設定
	int GetSelApIdx();
	///robor connect function
	int ConnectRobot(float dt);
	
	_RobotCommandState_ GetRobotTime();
	_RobotCommandState_ GetRobotName(char** pRobotName);
	_RobotCommandState_ GetRobotState(uint32_t &aiStatus,uint32_t &cleanStatus,uint32_t &error,uint32_t &worry);
	_RobotCommandState_ GetRobotPower();
	_RobotCommandState_ GetRobotSchedule();
	float GetRobotPowerValue();
};


#endif // __UIVIEW_H__