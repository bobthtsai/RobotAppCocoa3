
#ifndef __COCOS2DDIALOG_H__
#define __COCOS2DDIALOG_H__
#include "cocos2d.h"
//#include "cocos-ext.h"

USING_NS_CC;
//USING_NS_CC_EXT;
using namespace std;

class Toast: public CCLayerColor
{
public:
	CCSize visibleSize;
	CCPoint origin;
	//CCScale9Sprite* bg;
	CCSprite *bg;
public:
	Toast();
	~Toast();
	void removeSelf();
	virtual bool init();
	void initToast(string msg,float time);
	CREATE_FUNC(Toast);
	void onExit();
};

class PopupLayer: public CCLayer{
public:
	PopupLayer();
	~PopupLayer();

	virtual bool init();
	CREATE_FUNC(PopupLayer);

	virtual void registerWithTouchDispatcher(void);
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	static PopupLayer* create(const char* backgroundImage);
	static PopupLayer* createWithSpriteFrameName(const char* backgroundImage);
	
	

	void setTitle(const char* title, int fontsize = 20);
	void setContentText(const char* text, int fontsize = 20, int padding = 50, int paddintTop = 100);

	void setCallbackFunc(CCObject* target, SEL_CallFuncN callfun);

	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);    
	bool addButtonBySpriteFrame(const char *normalImage, const char *selectedImage, const char *title, int tag);
	virtual void onEnter();
	virtual void onExit();

private:

	void buttonCallback(CCObject* pSender);

	// 文字內容兩邊的空白區
	int m_contentPadding;
	int m_contentPaddingTop;

	CCObject* m_callbackListener;
	SEL_CallFuncN m_callback;

	CC_SYNTHESIZE_RETAIN(CCMenu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(CCSprite*, m__sfBackGround, SpriteBackGround);
	//CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltContentText, LabelContentText);


};

#endif //__COCOS2DDIALOG_H__