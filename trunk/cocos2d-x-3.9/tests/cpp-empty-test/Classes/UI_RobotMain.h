#ifndef __UIROBORMAIN_H__
#define __UIROBORMAIN_H__
#include "cocos2d.h"
#include <vector>
#include <sstream>
using namespace cocos2d;
using namespace std;
//觸碰元件的辨識 改為 settag 而並非用原本的順序關係
//m_array 分為兩種sprite 跟lab vectvor

enum _MAINPGETYPE_
{
	MAINPG_STATE,
	MAINPG_INFOAllRIGHT,
	MAINPG_INFOWARNING,
	MAINPG_SIZE, //≠∂≠±º∆∂q
	MAINPG_LAYER //µL≈„•‹≠∂≠±
};
enum _MAINSTATEITEM_
{
	MSTAET_BK=0,
	MSTAET_ICON,
	MSTAET_ICONALARM,
	MSTAET_BTNRESERVATION,
	MSTAET_BTNJOB,
	MSTAET_BTNREMOTE,
	MSTAET_BTNMAP,
	MSTAET_BTNSELBK,
	MSTAET_BTNSETUP,
	MSTAET_SIZE,
    MSTAET_LAB_NAME=MSTAET_SIZE,
    MSTAET_LAB_DATE,
    MSTAET_LAB_TIME,
    MSTAET_LAB_ALARM,
    MSTAET_LAB_POWER,
    MSTAET_LAB_SIZE,
	MSTAET_BTN1=0,
	MSTAET_BTN2,
	MSTAET_BTN3,
	MSTAET_BTN4,
	MSTAET_BTNSize,


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
	MINFOWA_SIZE,
    MINFOWA_LAB_BUTTON = MINFOWA_SIZE,
    MINFOWA_LAB_SIZE
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
//ROBOT ™Ï¶∏∏Í∞TFLAG
//¶p™G¡Ÿ®S¶≥Æ≥®Ï ∑|¿HÆ…ßÛ∑s
enum _ROBOTOINOFFLAG_
{
	RobFlag_Name=0,
	RobFlag_Time,
	RobFlag_State,
	RobFlag_Power,
	RobFlag_Schedule,
	RobFlag_All,	//ßP¬_¨Oß_©“¶≥∏Í∞T¨“§w∏gÆ≥®Ï
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



class UI_RobotMain: public Layer
{
private:
	int PageType;
	//bool bGetTimeNoError;
	//int  iRobotNameState;
	void InitUI();
	void InitUI_MainPage();
	void InitUI_RobotInfoAllRight();
	void InitUI_RobotInfoWarning();
	void PressSpriteMove(int iBtnIdx);	//´ˆ∂s©≥¶‚¶Ï∏m

    Size winSize;		//µ¯µ°§j§p
    Vec2 winOrigin;
	//•D≠n®Áº∆ ¶^∂«¨Oß_ß‰®Ï°A®√•B±N±o®Ï∏ÍÆ∆∞Oø˝§U®”
	std::vector<std::string> SSIDName;
	std::vector<std::wstring> APName;
    //觸摸事件
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
    void update(float dt) override;
    std::vector<SpriteVector* > m_SpriteVectors;
    std::vector<LabelVector* >  m_LabelVectors;
    std::map<int,long> m_itemIdx; //記住元件在陣列中的位置
	void ReleseRobotState(float dt);
	float fPowerPart; //≥—ælπq∂q
	string sRobotName;
	bool bShowMSG_TimeSync; // ¨Oß_¶≥∂}±“Æ…∂°¶P®Bƒµßiµ¯µ°
	bool bShowMSG_EvenOpen; // ¨Oß_¶≥∂}±“Æ…∂°¶P®Bƒµßiµ¯µ°
	bool bTimeSyncEven;		// ¨Oß_¶≥¿À¨dπL¶P®B
	//bool bRelesesRobotName;
	//int iRobotStateNumber;
	AI_STATE stRobotState,stOldRobotState;
	
	int RobotFlag[RobFlag_Number];	//•Œ©Ûthread ∂«øÈ´¸•Oflag
	int FailCount[RobFlag_Number];
public:
	CREATE_FUNC(UI_RobotMain); 
	virtual bool init() override;
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
	//µe≠±ßÛ∑s´¸•O√˛
	void ReleaseLab_RobotName();
	void ReleaseLab_RobotState();
	void ReleaseLab_RobotDateTime();
	void ReleaseLab_UpdateDateTime(void *pDate); //ßÛ∑sÆ…∂°∏Ú§È¥¡
	void ReleaseLab_RobotDate();
	void ReleaseLab_RobotTime();
	void ReleaseIcon_RobotState();
	void ReleaseLab_RobotPower();
    void InitTouchEven();
	//void ReleaselastRobotTime();
	//§UπF§£¶P™∫ßÛ∑s´¸•O
	void UpdateRobotByReleaseCommad(int i);
	// ®˙±oROBOT Æ…∂°	
	
//	void SetRobotTimeFailToGet();
//	void CleanRobotTimeFailToGet();
	//Robot flag ≥]©w(•Œ©Û®M©w¨Oß_≠n¶€∞ ´¸•O)
	void RobotFlagClean(int iType);
	void RobotFlagOk(int iType);
	void RobotFlagFail(int iType);
	void RobotFlagRetry(int iType);
	//≠◊ßÔROBOT ¶W∫Ÿ	
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
	// ¿ÀπÓ≤{¶b©“¶≥æ˜•x™¨∫A	
	bool CheckRobotName();//¿À¨d≤{¶b¶W∫Ÿ™¨∫A!®M©w¨Oß_≠n∂«∞e´¸•O
	bool CheckRobotTime();//¿À¨d≤{¶bÆ…∂°™¨∫A!®M©w¨Oß_≠n∂«∞e´¸•O
	bool CheckRobotState();//¿À¨d≤{¶bæ˜•x™¨∫A!®M©w¨Oß_≠n∂«∞e´¸•O
	bool CheckRobotPower();//¿À¨d≤{¶bπq∂q™¨∫A!®M©w¨Oß_≠n∂«∞e´¸•O
	bool CheckRobotSchedule(); //®˙±o≤{¶b¶w±∆πw¨˘¶Êµ{™Ì
	const char* GetRobotName(); //¶^∂«æ˜•x¶W∫Ÿ
	void ShowTimeSyncBox();	
	void MsgBoxTimeSync(Node *pNode);// Æ…∂°¶P®B
	void CheckRobtoTimeSync(); //¿À¨d¨Oß_ª›≠nÆ…∂°¶P®B

};
//bool IsRobotMapReady();
//void RobotMapUpdate();//¶aπœßÛ∑s´¸•O
//void WaitRobotMapUpdate(); //µ•´›¶aπœ™Ï¶∏ßÛ∑s
//int WaittryRobotMapUpdate();//µ•´›¶aπœ™Ï¶∏ßÛ∑s
#endif //__UIROBORMAIN_H__