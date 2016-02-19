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
	MAINPG_SIZE, //�����ƶq
	MAINPG_LAYER //�L��ܭ���
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
//ROBOT �즸��TFLAG
//�p�G�٨S������ �|�H�ɧ�s
enum _ROBOTOINOFFLAG_
{
	RobFlag_Name=0,
	RobFlag_Time,
	RobFlag_State,
	RobFlag_Power,
	RobFlag_Schedule,
	RobFlag_All,	//�P�_�O�_�Ҧ���T�Ҥw�g����
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
	void PressSpriteMove(int iBtnIdx);	//���s�����m

	CCSize winSize;		//�����j�p
	//�D�n��� �^�ǬO�_���A�åB�N�o���ưO���U��
	std::vector<std::string> SSIDName;
	std::vector<std::wstring> APName;
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
private:  
	CCArray* m_BtnArray[MAINPG_SIZE];//�s��Scan page�Ҧ����s
	void update(float dt);  
	void ReleseRobotState(float dt); 
	float fPowerPart; //�Ѿl�q�q
	string sRobotName;
	bool bShowMSG_TimeSync; // �O�_���}�Үɶ��P�Bĵ�i����
	bool bShowMSG_EvenOpen; // �O�_���}�Үɶ��P�Bĵ�i����
	bool bTimeSyncEven;		// �O�_���ˬd�L�P�B
	//bool bRelesesRobotName;
	//int iRobotStateNumber;
	AI_STATE stRobotState,stOldRobotState;
	
	int RobotFlag[RobFlag_Number];	//�Ω�thread �ǿ���Oflag
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
	//�e����s���O��
	void ReleaseLab_RobotName();
	void ReleaseLab_RobotState();
	void ReleaseLab_RobotDateTime();
	void ReleaseLab_UpdateDateTime(void *pDate); //��s�ɶ�����
	void ReleaseLab_RobotDate();
	void ReleaseLab_RobotTime();
	void ReleaseIcon_RobotState();
	void ReleaseLab_RobotPower();
	//void ReleaselastRobotTime();
	//�U�F���P����s���O
	void UpdateRobotByReleaseCommad(int i);
	// ���oROBOT �ɶ�	
	
//	void SetRobotTimeFailToGet();
//	void CleanRobotTimeFailToGet();
	//Robot flag �]�w(�Ω�M�w�O�_�n�۰ʫ��O)
	void RobotFlagClean(int iType);
	void RobotFlagOk(int iType);
	void RobotFlagFail(int iType);
	void RobotFlagRetry(int iType);
	//�ק�ROBOT �W��	
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
	// �˹�{�b�Ҧ����x���A	
	bool CheckRobotName();//�ˬd�{�b�W�٪��A!�M�w�O�_�n�ǰe���O
	bool CheckRobotTime();//�ˬd�{�b�ɶ����A!�M�w�O�_�n�ǰe���O
	bool CheckRobotState();//�ˬd�{�b���x���A!�M�w�O�_�n�ǰe���O
	bool CheckRobotPower();//�ˬd�{�b�q�q���A!�M�w�O�_�n�ǰe���O
	bool CheckRobotSchedule(); //���o�{�b�w�ƹw����{��
	const char* GetRobotName(); //�^�Ǿ��x�W��
	void ShowTimeSyncBox();	
	void MsgBoxTimeSync(cocos2d::CCNode *pNode);// �ɶ��P�B
	void CheckRobtoTimeSync(); //�ˬd�O�_�ݭn�ɶ��P�B
};
bool IsRobotMapReady();
void RobotMapUpdate();//�a�ϧ�s���O
void WaitRobotMapUpdate(); //���ݦa�Ϫ즸��s
int WaittryRobotMapUpdate();//���ݦa�Ϫ즸��s
#endif //__UIROBORMAIN_H__