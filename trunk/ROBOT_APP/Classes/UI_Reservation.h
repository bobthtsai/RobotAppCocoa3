#ifndef __UIRESERVATION_H__
#define __UIRESERVATION_H__
#include "cocos2d.h"
#include <time.h> 
using namespace cocos2d;
//��������
enum _RESERVATIONPGETYPE_
{
	RESPG_MAIN,
	RESPG_SINGAL,
	RESPG_DAILY,
	RESPG_SIZE, //�����ƶq
	RESPG_LAYER, //�L��ܭ���
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
	RESMAIN_BTNSPACENUM=4, //�C�ӭI���W������ƶq

};
enum _RESSINGALITEM_
{
	RESSINGAL_BK=0,
	RESSINGAL_TITAL,	
	RESSINGAL_SWITCHBK, 
	RESSINGAL_SWITCHBTN, 
	RESSINGAL_TIMEBK,
	//�H�U�y�Ф��@��
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
	//�H�W�y�Ф��@��
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
	//�H�U�y�Ф��@��
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
	//�H�W�y�Ф��@��	RESDAILY_LABTIME,
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
	int iScheduleTotalNumber;	//ROBOT��{�`�ƶq
	int iScheduleCount;			//ROBOT��{�p��
	CCSize winSize;  
	CCArray* m_BtnArray[RESMAIN_SIZE];//�s��Ҧ����s
	bool bAllowSave;
	void InitUI();
	void InitUI_RESMAIN();
	void InitUI_RESSINGAL();	
	void InitUI_RESDAILY();
	void ResetUI_RESMAINLab();
	void ResetUI_RESSINGALByIni();			//�ھڳ]�w�� �ק���ܵe��
	void ResetUI_RESSINGALCheckSaveBtn();	//�ˬd�]�w�O�_�@�P
	void ResetUI_RESSINGALTime();			//��s�w���ɶ�

	void ResetUI_RESDAILYByIni();			//�ھڳ]�w�� �ק���ܵe��
	void ResetUI_RESDAILYCheckSaveBtn();	//�ˬd�]�w�O�_�@�P	
	void ResetUI_RESDAILYTime();			//��s�w���ɶ�	
	

	void BtnColorSetting(int iBtnIdx,bool bSel);//���s�ܦ�
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

	void* AddRobotSachedule(int iNameSize); //�W�[��{

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
	//���oROBOT �]�w
	void GetSchedule();
	//ROBOT �]�w�w�����a()
	void SetSchedule();
	void SaveTimeToSchedule(int iType); //�N�{�b�]�w�Ǩ�����W
	void SaveRobotSachedule(void* pData); //�s��Schedule
	void ResetSettingByRobotSchedule();		//�ھ�ROBOT�W���ƾڭק�]�w
};


#endif // __UIRESERVATION_H__