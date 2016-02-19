#ifndef __UISETUP_H__
#define __UISETUP_H__
#include "cocos2d.h"
using namespace cocos2d;
//頁面種類
enum _RobotSetupPGETYPE_
{
	SETUPPG_MAIN,
	SETUPPG_NOTIF,
	SETUPPG_SIZE, //頁面數量
	SETUPPG_LAYER //無顯示頁面
};
enum _RobotSetupMAINITEM_
{
	SETUPMAIN_BK=0,
	SETUPMAIN_TITAL,
	SETUPMAIN_NAME,
	SETUPMAIN_HOMEAP,
	SETUPMAIN_SYNCTIME,
	SETUPMAIN_NOTIFICATION,
	SETUPMAIN_LOG,
	SETUPMAIN_HELP,
	SETUPMAIN_LEGAL,
	SETUPMAIN_DISCONNECTION,
	SETUPMAIN_SIZE,
    SETUPMAIN_LAB_MODEL=SETUPMAIN_SIZE,
    SETUPMAIN_LAB_VERSION,
    SETUPMAIN_LAB_APPVER,
    SETUPMAIN_LAB_NAME,
    SETUPMAIN_LAB_SIZE
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
class UI_RobotSetup: public Layer
{
private:  
    int PageType;
    Size winSize;
    Vec2 winOrigin;
    std::vector<SpriteVector* > m_SpriteVectors;    //需要控制的圖形元件
    std::vector<LabelVector* >  m_LabelVectors;     //需要控制的標籤元件
    std::map<int,long> m_itemIdx; //記住元件在陣列中的位置
    
	void InitUI();
	void InitUI_RobotSetupMAIN();
	void InitUI_RobotSetupNotif();
//	virtual void registerWithTouchDispatcher();
//	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
//	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
//	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);	
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
    SpriteVector* getSpriteArray(int pageType);
    LabelVector* getLabelArray(int pageType);
    void saveSprite(int Page,int iIDx,Sprite* pSprite);
    void saveLabel(int Page,int iIDx,Label* pSprite);

public:
	CREATE_FUNC(UI_RobotSetup); 
	virtual bool init() override;
	UI_RobotSetup();
	~UI_RobotSetup();
	void Function_UISETUP(int iBtnIdx); // 滑鼠放開始事件
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

	// 顯示警告視窗
	void ShowDisconnectBox();
	void MsgBtnCallback(Node *pNode);

};


#endif // __UISETUP_H__