#ifndef __UIROBOTREMOTE_H__
#define __UIROBOTREMOTE_H__
#include "cocos2d.h"
using namespace cocos2d;

enum _REMOTEPGETYPE_
{
	REMOTEPG_STATE,
	REMOTEPG_SIZE,	//�����ƶq
	REMOTEPG_LAYER  //�L��ܭ���
};
enum _REMOTEBTN_TAG_
{
	REMOTEBTN_BK=0,
	REMOTEBTN_TITAL,
	REMOTEBTN_FORWARD,
	REMOTEBTN_LEFT,
	REMOTEBTN_RIGHT,	
	REMOTEBTN_GODOCK,
	REMOTEBTN_STOP,
	REMOTEBTN_AUTO,
	REMOTEBTN_SPIRAL,
	REMOTEBTN_WALL,
	REMOTEBTN_EXPLOREMAP,
	REMOTEBTN_VOICE,
	REMOTEBTN_SIZE
};

class UI_RobotRemote: public CCLayer
{
private:  
	CCSize winSize;  
	CCArray* m_BtnArray[REMOTEPG_SIZE];//�s��C�ӭ����Ҧ����s
	int PageType;
	int iSelRemoteBtn;
	void InitUI();
	virtual void registerWithTouchDispatcher();
	virtual void removeWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	void SetupUIBtn(CCSprite* pSprite,int iSel,bool bOnOff);
	
;//�]�w�e�����s
public:
	CREATE_FUNC(UI_RobotRemote); 
	virtual bool init();  
	UI_RobotRemote();
	~UI_RobotRemote();
	//��ܹw�]����
	void ShowLayer();
	void HideLayer();
	void Hide_AllPage();
	void ControlFunction(int iTag);
	void Show_RemotePage();
	void Hide_RemotePage();	
	void InitUI_RemotePage();
};


#endif // __UIVIEW_H__