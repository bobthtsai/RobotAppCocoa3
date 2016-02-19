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
	RESMAIN_DAILY,
	RESMAIN_SIZE,
    RESMAIN_LAB_SINGAL = RESMAIN_SIZE,
    RESMAIN_LAB_SINGALHELP,
    RESMAIN_LAB_SINGALVALUE,
    RESMAIN_LAB_DAILY,
    RESMAIN_LAB_DAILYHELP,
    RESMAIN_LAB_DAILYVALUE,
    RESMAIN_LAB_SIZE,
	RESMAIN_BTNSPACENUM=4, //每個背景上面元件數量

};
enum _RESSINGALITEM_
{
	RESSINGAL_BK=0,
	RESSINGAL_TITAL,	
	RESSINGAL_SWITCHBK, 
	RESSINGAL_SWITCHBTN, 
	RESSINGAL_TIMEBK,
	//以下座標不一樣 (圖 文 順序不能變)
	RESSINGAL_TIMEHOUR_UP_BTN,
	RESSINGAL_TIMEHOUR_UP_LAB_VALUE,
	RESSINGAL_TIMEHOUR_NOW_BTN,
	RESSINGAL_TIMEHOUR_NOW_LAB_VALUE,
	RESSINGAL_TIMEHOUR_DOWN_BTN,
	RESSINGAL_TIMEHOUR_DOWN_LAB_VALUE,
	RESSINGAL_TIMEMIN_UP_BTN,
	RESSINGAL_TIMEMIN_UP_LAB_VALUE,
	RESSINGAL_TIMEMIN_NOW_BTN,
	RESSINGAL_TIMEMIN_NOW_LAB_VALUE,
	RESSINGAL_TIMEMIN_DOWN_BTN,
	RESSINGAL_TIMEMIN_DOWN_LAB_VALUE,
	//以上座標不一樣
	RESSINGAL_LAB_TIME,
	RESSINGAL_LAB_VALUETIME,
	RESSINGAL_BTNCANEL,
	RESSINGAL_BTNSAVE,

	RESSINGAL_SIZE,
	RESSINGAL_TIME_START=RESSINGAL_TIMEHOUR_UP_BTN,
	RESSINGAL_TIME_END=RESSINGAL_TIMEMIN_DOWN_LAB_VALUE,

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
	RESDAILY_TIMEHOUR_UP_LAB_VALUE,
	RESDAILY_TIMEHOUR_NOW_BTN,
	RESDAILY_TIMEHOUR_NOW_LAB_VALUE,
	RESDAILY_TIMEHOUR_DOWN_BTN,
	RESDAILY_TIMEHOUR_DOWN_LAB_VALUE,
	RESDAILY_TIMEMIN_UP_BTN,
	RESDAILY_TIMEMIN_UP_LAB_VALUE,
	RESDAILY_TIMEMIN_NOW_BTN,
	RESDAILY_TIMEMIN_NOW_LAB_VALUE,
	RESDAILY_TIMEMIN_DOWN_BTN,
	RESDAILY_TIMEMIN_DOWN_LAB_VALUE,
	//以上座標不一樣	RESDAILY_LABTIME,
	RESDAILY_LAB_TIME,
	RESDAILY_LAB_VALUETIME,
	RESDAILY_BTNCANEL,
	RESDAILY_BTNSAVE,
	RESDAILY_SIZE,
	RESDAILY_TIME_START=RESDAILY_TIMEHOUR_UP_BTN,
	RESDAILY_TIME_END=RESDAILY_TIMEMIN_DOWN_LAB_VALUE,

};
class UI_Reservation: public Layer
{
private:  
	int PageType;
    Size winSize;
    Vec2 winOrigin;
    std::vector<SpriteVector* > m_SpriteVectors;    //需要控制的圖形元件
    std::vector<LabelVector* >  m_LabelVectors;     //需要控制的標籤元件
    std::map<int,long> m_itemIdx; //記住元件在陣列中的位置
    
	int iScheduleTotalNumber;	//ROBOT行程總數量
	int iScheduleCount;			//ROBOT行程計數
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
//	virtual void registerWithTouchDispatcher();
//	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
//	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
//	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	void* AddRobotSachedule(int iNameSize); //增加行程
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
    inline SpriteVector* getSpriteArray(int pageType);
    inline LabelVector* getLabelArray(int pageType);
    void saveSprite(int Page,int iIDx,Sprite* pSprite);
    void saveLabel(int Page,int iIDx,Label* pSprite);

public:
	CREATE_FUNC(UI_Reservation); 
	virtual bool init() override;
	UI_Reservation();
	~UI_Reservation();
	void Function_RESMAIN(int iBtnIdx);
	void Function_RESSINGAL(int iBtnIdx);
	void Function_RESDAILY(int iBtnIdx);
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