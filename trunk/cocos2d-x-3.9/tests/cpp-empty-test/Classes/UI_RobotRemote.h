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

class UI_RobotRemote: public Layer
{
private:
    //layer 基礎變數
    int PageType;
    Size winSize;
    Vec2 winOrigin;
    std::vector<SpriteVector* > m_SpriteVectors;    //需要控制的圖形元件
    std::vector<LabelVector* >  m_LabelVectors;     //需要控制的標籤元件
    std::map<int,long> m_itemIdx; //記住元件在陣列中的位置
    
    //layer 獨特變數
	int iSelRemoteBtn;
	void InitUI();
    void InitUI_RemotePage();
    void SetupUIBtn(Sprite* pSprite,int iSel,bool bOnOff);
    
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
    
	
	
;//設定畫面按鈕
public:
	CREATE_FUNC(UI_RobotRemote); 
	virtual bool init() override;
	UI_RobotRemote();
	~UI_RobotRemote();
	//顯示預設頁面
	void ShowLayer();
	void HideLayer();
	void Hide_AllPage();
	void ControlFunction(int iTag);
	void Show_RemotePage();
	void Hide_RemotePage();	
	
};


#endif // __UIVIEW_H__