#ifndef __UISETUP_H__
#define __UISETUP_H__
#include "cocos2d.h"
using namespace cocos2d;
//��������
enum _RobotSetupPGETYPE_
{
	SETUPPG_MAIN,
	SETUPPG_NOTIF,
	SETUPPG_SIZE, //�����ƶq
	SETUPPG_LAYER //�L��ܭ���
};
enum _RobotSetupMAINITEM_
{
	SETUPMAIN_BK=0,
	SETUPMAIN_TITAL,
	SETUPMAIN_MODEL,
	SETUPMAIN_VERSION,
	SETUPMAIN_APPVER,
	SETUPMAIN_NAMELAB,
	SETUPMAIN_NAME,
	SETUPMAIN_HOMEAP,
	SETUPMAIN_SYNCTIME,
	SETUPMAIN_NOTIFICATION,
	SETUPMAIN_LOG,
	SETUPMAIN_HELP,
	SETUPMAIN_LEGAL,
	SETUPMAIN_DISCONNECTION,
	SETUPMAIN_SIZE
};
enum _RobotSetupNOTIFITEM_
{
	SETUPNOTIF_BK=0,
	SETUPNOTIF_TITAL,
	SETUPNOTIF_TIMESYNC_BTN,
	SETUPNOTIF_TIMESYNC_CHECK,
	SETUPNOTIF_CLEANJOB_BTN,
	SETUPNOTIF_CLEANJOB_CHECK,
	SETUPNOTIF_SIZE
};
class UI_RobotSetup: public CCLayer
{
private:  
	int PageType;
	CCSize winSize;  
	CCArray* m_BtnArray[SETUPPG_SIZE];//�s��Ҧ����s
	void InitUI();
	void InitUI_RobotSetupMAIN();
	void InitUI_RobotSetupNotif();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);	
	

public:
	CREATE_FUNC(UI_RobotSetup); 
	virtual bool init();  
	UI_RobotSetup();
	~UI_RobotSetup();
	void Function_UISETUP(int iBtnIdx); // �ƹ���}�l�ƥ�
	void Function_UINOTIF(int iBtnIdx);
	void ShowLayer();
	void HideLayer();	
	void Hide_AllPage();
	void Show_RobotSetupMAIN();
	void Hide_RobotSetupMAIN();
	void Show_RobotSetupNOTIF();
	void Hide_RobotSetupNOTIF();

	void ReleasePage_SetupMAIN();
	void ReleasePage_Notifo();

	// ���ĵ�i����
	void ShowDisconnectBox();
	void MsgBtnCallback(cocos2d::CCNode *pNode);

};


#endif // __UISETUP_H__