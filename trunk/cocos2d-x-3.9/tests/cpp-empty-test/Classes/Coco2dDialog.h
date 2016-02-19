
#ifndef __COCOS2DDIALOG_H__
#define __COCOS2DDIALOG_H__
#include "cocos2d.h"
using namespace cocos2d;

USING_NS_CC;
using namespace std;

class Toast: public LayerColor
{
public:
    Size winSize;
    Vec2 winOrigin;
//	CCSize visibleSize;
//	CCPoint origin;
	//CCScale9Sprite* bg;
	Sprite *bg;
public:
	Toast();
	~Toast();
	void removeSelf();
	virtual bool init();
	void initToast(string msg,float time);
	CREATE_FUNC(Toast);
	void onExit();
};

class PopupLayer: public Layer{
public:
	PopupLayer();
	~PopupLayer();

	virtual bool init() override;
	CREATE_FUNC(PopupLayer);

//	virtual void registerWithTouchDispatcher(void);
//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	static PopupLayer* create(const char* backgroundImage);
	static PopupLayer* createWithSpriteFrameName(const char* backgroundImage);
	
	

	void setTitle(const char* title, int fontsize = 20);
	void setContentText(const char* text, int fontsize = 20, int padding = 50, int paddintTop = 100);

	void setCallbackFunc(Ref* target, SEL_CallFuncN callfun);

	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);    
	bool addButtonBySpriteFrame(const char *normalImage, const char *selectedImage, const char *title, int tag);
	virtual void onEnter() override;
	virtual void onExit() override;
    
protected: //touch 修改
    bool touchEnabled;
    EventListenerTouchOneByOne* touchListener;
    bool onTouchBegan(Touch* touch, Event* event) override;
    void setTouchEnabled(bool enable);
private:

	void buttonCallback(Ref* pSender);

	// 文字內容兩邊的空白區
	int m_contentPadding;
	int m_contentPaddingTop;

	Ref* m_callbackListener;
	SEL_CallFuncN m_callback;

	CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
	//CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(Label*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(Label*, m__ltContentText, LabelContentText);


};

#endif //__COCOS2DDIALOG_H__