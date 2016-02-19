#ifndef __UIROBOTREMOTE_H__
#define __UIROBOTREMOTE_H__
#include "cocos2d.h"
using namespace cocos2d;

enum _REMOTEPGETYPE_
{
	REMOTEPG_STATE,
	REMOTEPG_SIZE,	//頁面數量
	REMOTEPG_LAYER  //無顯示頁面
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
	CCArray* m_BtnArray[REMOTEPG_SIZE];//存放每個頁面所有按鈕
	int PageType;
	int iSelRemoteBtn;
	void InitUI();
	virtual void registerWithTouchDispatcher();
	virtual void removeWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	void SetupUIBtn(CCSprite* pSprite,int iSel,bool bOnOff);
	
;//設定畫面按鈕
public:
	CREATE_FUNC(UI_RobotRemote); 
	virtual bool init();  
	UI_RobotRemote();
	~UI_RobotRemote();
	//顯示預設頁面
	void ShowLayer();
	void HideLayer();
	void Hide_AllPage();
	void ControlFunction(int iTag);
	void Show_RemotePage();
	void Hide_RemotePage();	
	void InitUI_RemotePage();
};


#endif // __UIVIEW_H__