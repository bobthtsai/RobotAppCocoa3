#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
USING_NS_CC;
const int G_ACT_TAG=1000;
const int G_ACTION_TEST1_TAG =G_ACT_TAG+1;

class HelloWorld : public cocos2d::CCLayer
{
private:
	bool bInLayer;
	int UpdateTime;
public:
	~HelloWorld();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();
	
	// a selector callback
	void menuCloseCallback(CCObject* pSender);
	
	// implement the "static node()" method manually
	CREATE_FUNC(HelloWorld);
	//BOB ADD
	void LoadRobotImage();
	void InitSpriteEven();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	
	void Hide_allLayer(); //隱藏所有頁面
	void LoadSetting(); //載入設定檔
	void InitSetting(); //初始化設定	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	void onWin32KeyEvent( UINT message,WPARAM wParam, LPARAM lParam ) ;
	void KeyDownEven_Remote( WPARAM wParam);
#endif
	void update(float dt); //5秒後登入

};

#endif // __HELLOWORLD_SCENE_H__
