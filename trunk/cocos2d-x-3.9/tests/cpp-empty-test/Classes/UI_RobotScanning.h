#ifndef __UIROBORSCANNING_H__
#define __UIROBORSCANNING_H__
#include "cocos2d.h"
#include <vector>
#include <sstream>
#include "ImageList.h"
using namespace cocos2d;
using namespace std;

static const int TAG_UI_UIROBORSCANNING_Bk =10;
static const int MAX_DISPLAY_WIFINAME =6;   //不可以大於100 因為m_itemIdx 預設page 100個元件
enum _SCANITEM_m_itemIdx
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
    UINODEVICE_SIZE,
	UINODEVICE_LAB_REPORTLINK=UINODEVICE_SIZE,
	UINODEVICE_LAB_SIZE
};
enum _WIFILISTITEM_
{
	UIWIFILIST_BK=0,
	UIWIFILIST_TOPBTN,
	UIWIFILIST_DOWNBTN,
	UIWIFILIST_WIFILIST,	//§j©Û¶π∂µ•ÿ ¨“¨∞AP
	UIWIFILIST_WIFILISTMAX=UIWIFILIST_WIFILIST+MAX_DISPLAY_WIFINAME, // ≥Ã¶h§ª≤’
    UIWIFILIST_SIZE,
    UIWIFILIST_LAB_REPORTLINK=UIWIFILIST_SIZE,
    UIWIFILIST_LAB_SIZE

};
enum _CONNECTANIITEM_
{
	CONNECTANI_BK=0,
	CONNECTANI_ICON,
	CONNECTANI_SIZE,
    CONNECTANI_LAB_NAME =CONNECTANI_SIZE ,
	CONNECTANI_LAB_SSID,
	CONNECTANI_LAB_SIZE
};
enum _ERRCONNFAILITEM_
{
	ERRCONNFAIL_BK=0,
	ERRCONNFAIL_ICON,
	ERRCONNFAIL_CANCEL,
	ERRCONNFAIL_RETRY,
	ERRCONNFAIL_SIZE,
    ERRCONNFAIL_LAB_REPORTLINK=ERRCONNFAIL_SIZE,
    ERRCONNFAIL_LAB_SIZE,
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

//
//
//typedef Vector<Sprite* > SpriteVector;
//typedef Vector<Label* > LabelVector;

class UI_RobotScanning: public Layer
{
private:
	//int m_RobotFunState;		//≥sΩu™¨∫A
	int PageType;
    Size winSize;
    Vec2 winOrigin;
    
	int WifiList_DrawLineNum;	//µeΩuº∆∂q
	int WifiList_StartIdx;		//∞_©lAP
	int WifiList_SelAPIdx;		//APøÔ∂µ
	int iIconSpeed_Search;	//Icon ±€¬‡≥t´◊
	int iIconSpeed_Connect;	//Icon ±€¬‡≥t´◊
	int m_SearchType;			//∑j¥M™¨∫A
	float RobotPowerValue;	 //πq¶¿πq∂q
	bool MsgBoxShow_RobotTimeFailGet;	//≈„•‹ƒµßiµ¯µ°(Æ…∂°¶P®Bø˘ª~)
	bool MsgBoxShow_RobotStateFailGet;	//≈„•‹ƒµßiµ¯µ°(™¨∫A≈™®˙ø˘ª~)
	bool MsgBoxShow_RobotPowerFailGet;	//≈„•‹ƒµßiµ¯µ°(™¨∫A≈™®˙ø˘ª~)	

	//•D≠n®Áº∆ ¶^∂«¨Oß_ß‰®Ï°A®√•B±N±o®Ï∏ÍÆ∆∞Oø˝§U®”
	std::vector<std::string> SSIDName;
	std::vector<std::wstring> APName;
	std::vector<int> iLinkReady;
	//std::vector<int> NetworkLinkIndex;
protected: //touch 修改
    bool touchEnabled;
    EventListenerTouchOneByOne* touchListener;
    bool onTouchBegan(Touch* touch, Event* event) override;
    void onTouchMoved(Touch* touch, Event* event) override;
    void onTouchEnded(Touch* touch, Event* event) override;
    bool isTouchEnabled() const;
    virtual void setTouchEnabled(bool enabled);
    Sprite* getSpritePoint(int iPageTyep,int iPageIdx);
    Label* getLabelPoint(int iPageTyep,int iPageIdx);
    
private:  
    //Vector<SpriteVector* > m_BtnArray;
    std::vector<SpriteVector* > m_SpriteVectors;
    std::vector<LabelVector* >  m_LabelVectors;
    std::map<int,long> m_itemIdx; //記住元件在陣列中的位置
	//CCArray* m_BtnArray[SCANPG_SIZE];//¶s©ÒScan page©“¶≥´ˆ∂s
    std::vector <std::string>m_WifiListSSIDName;
    Vector <Node* >m_WifiListSSIDLabel;
    std::vector <std::string>m_WifiListAPName;
    Vector <Node* >m_WifiListAPLabel;
    
//	CCArray* m_WifiListSSIDName;//¶s©Ò©“¶≥WIFI SSID name string
//	CCArray* m_WifiListSSIDLabel;//¶s©Ò©“¶≥WIFI SSID  name Label
//	CCArray* m_WifiListAPName;//¶s©Ò©“¶≥WIFI AP name string
//	CCArray* m_WifiListAPLabel;//¶s©Ò©“¶≥WIFI AP name Label
	void InitUI();
	void InitUI_ScaningPage();
	void InitUI_ErrorNoDevice();
	void InitUI_WifiList();
	void InitUI_ConnectAni();
	void InitUI_ErrConnFail();

	void update(float dt) override;
	void PAGEANI_SEARCH(); //≠∂≠±∞ µe
	void PAGEANI_CONNECT(); //≠∂≠±∞ µe
	
public:
	CREATE_FUNC(UI_RobotScanning); 
	virtual bool init() override;  
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
	void WifiLabelUpDown(Vec2 MousePoint);
	void ConnectRobotStart(int iBtnNum);
	void ConnectRobotFun();
	void SearchIconStop();//πœ•‹±€¬‡ß÷≥t
	void SearchIconFast();//πœ•‹±€¬‡ß÷≥t
	void SearchIconSlow();//πœ•‹±€¬‡Ωw∫C
	void ChangConnectIconSpeed();//πœ•‹±€¬‡≥t´◊ßÔ≈‹	
	void ConnectFailSetting(int iType); //•¢±—≠Ï¶]≥]©w
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