#ifndef __UIRESERVATION_H__
#define __UIRESERVATION_H__
#include "cocos2d.h"
#include <time.h> 
using namespace cocos2d;
//頁面種類
enum _RESERVATIONPGETYPE_
{
	RESPG_MAIN,
	RESPG_SINGAL,
	RESPG_DAILY,
	RESPG_SIZE, //頁面數量
	RESPG_LAYER, //無顯示頁面
};

enum _RESMAINITEM_
{
	RESMAIN_BK=0,
	RESMAIN_TITAL,	
	RESMAIN_SINGAL,
	RESMAIN_LABSINGAL, 
	RESMAIN_LABSINGALHELP, 
	RESMAIN_LABSINGALVALUE, 
	RESMAIN_DAILY,
	RESMAIN_LABDAILY,
	RESMAIN_LABDAILYHELP,
	RESMAIN_LABDAILYVALUE,
	RESMAIN_SIZE,
	RESMAIN_BTNSPACENUM=4, //每個背景上面元件數量

};
enum _RESSINGALITEM_
{
	RESSINGAL_BK=0,
	RESSINGAL_TITAL,	
	RESSINGAL_SWITCHBK, 
	RESSINGAL_SWITCHBTN, 
	RESSINGAL_TIMEBK,
	//以下座標不一樣
	RESSINGAL_TIMEHOUR_UP_BTN,
	RESSINGAL_TIMEHOUR_UP_VALUE,
	RESSINGAL_TIMEHOUR_NOW_BTN,
	RESSINGAL_TIMEHOUR_NOW_VALUE,
	RESSINGAL_TIMEHOUR_DOWN_BTN,
	RESSINGAL_TIMEHOUR_DOWN_VALUE,
	RESSINGAL_TIMEMIN_UP_BTN,
	RESSINGAL_TIMEMIN_UP_VALUE,
	RESSINGAL_TIMEMIN_NOW_BTN,
	RESSINGAL_TIMEMIN_NOW_VALUE,
	RESSINGAL_TIMEMIN_DOWN_BTN,
	RESSINGAL_TIMEMIN_DOWN_VALUE,
	//以上座標不一樣
	RESSINGAL_LABTIME,
	RESSINGAL_VALUETIME,
	RESSINGAL_BTNCANEL,
	RESSINGAL_BTNSAVE,

	RESSINGAL_SIZE,
	RESSINGAL_TIME_START=RESSINGAL_TIMEHOUR_UP_BTN,
	RESSINGAL_TIME_END=RESSINGAL_TIMEMIN_DOWN_VALUE,

};

enum _RESDAILYITEM_
{
	RESDAILY_BK=0,
	RESDAILY_TITAL,	
	RESDAILY_SWITCHBK, 
	RESDAILY_SWITCHBTN,
	RESDAILY_TIMEBK,
	//以下座標不一樣
	RESDAILY_TIMEHOUR_UP_BTN,
	RESDAILY_TIMEHOUR_UP_VALUE,
	RESDAILY_TIMEHOUR_NOW_BTN,
	RESDAILY_TIMEHOUR_NOW_VALUE,
	RESDAILY_TIMEHOUR_DOWN_BTN,
	RESDAILY_TIMEHOUR_DOWN_VALUE,
	RESDAILY_TIMEMIN_UP_BTN,
	RESDAILY_TIMEMIN_UP_VALUE,
	RESDAILY_TIMEMIN_NOW_BTN,
	RESDAILY_TIMEMIN_NOW_VALUE,
	RESDAILY_TIMEMIN_DOWN_BTN,
	RESDAILY_TIMEMIN_DOWN_VALUE,
	//以上座標不一樣	RESDAILY_LABTIME,
	RESDAILY_LABTIME,
	RESDAILY_VALUETIME,
	RESDAILY_BTNCANEL,
	RESDAILY_BTNSAVE,
	RESDAILY_SIZE,
	RESDAILY_TIME_START=RESDAILY_TIMEHOUR_UP_BTN,
	RESDAILY_TIME_END=RESDAILY_TIMEMIN_DOWN_VALUE,

};
class UI_Reservation: public CCLayer
{
private:  
	int PageType;
	int iScheduleTotalNumber;	//ROBOT行程總數量
	int iScheduleCount;			//ROBOT行程計數
	CCSize winSize;  
	CCArray* m_BtnArray[RESMAIN_SIZE];//存放所有按鈕
	bool bAllowSave;
	void InitUI();
	void InitUI_RESMAIN();
	void InitUI_RESSINGAL();	
	void InitUI_RESDAILY();
	void ResetUI_RESMAINLab();
	void ResetUI_RESSINGALByIni();			//根據設定檔 修改顯示畫面
	void ResetUI_RESSINGALCheckSaveBtn();	//檢查設定是否一致
	void ResetUI_RESSINGALTime();			//更新預約時間

	void ResetUI_RESDAILYByIni();			//根據設定檔 修改顯示畫面
	void ResetUI_RESDAILYCheckSaveBtn();	//檢查設定是否一致	
	void ResetUI_RESDAILYTime();			//更新預約時間	
	

	void BtnColorSetting(int iBtnIdx,bool bSel);//按鈕變色
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

	void* AddRobotSachedule(int iNameSize); //增加行程

public:
	CREATE_FUNC(UI_Reservation); 
	virtual bool init();  
	UI_Reservation();
	~UI_Reservation();
	int Function_RESMAIN(int iBtnIdx);
	int Function_RESSINGAL(int iBtnIdx);
	int Function_RESDAILY(int iBtnIdx);
	void ShowLayer();
	void HideLayer();	
	void Hide_AllPage();
	void Show_RESMAIN();
	void Hide_RESMAIN();
	void Show_RESSINGAL();
	void Hide_RESSINGAL();
	void Show_RESDAILY();
	void Hide_RESDAILY();
	//取得ROBOT 設定
	void GetSchedule();
	//ROBOT 設定預約掃地()
	void SetSchedule();
	void SaveTimeToSchedule(int iType); //將現在設定傳到機器上
	void SaveRobotSachedule(void* pData); //存放Schedule
	void ResetSettingByRobotSchedule();		//根據ROBOT上的數據修改設定
};


#endif // __UIRESERVATION_H__