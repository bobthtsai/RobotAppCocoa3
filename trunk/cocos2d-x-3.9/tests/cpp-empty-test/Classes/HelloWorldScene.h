#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
private:
    bool bInLayer;
    int UpdateTime;
    Size winSize;
    Point winOrigin;
    virtual void update(float dt) override;
    //觸摸事件
protected:
    bool touchEnabled;
    EventListenerTouchOneByOne* touchListener;
public:
    typedef Layer super;
    typedef HelloWorld self;
    HelloWorld(void);
    virtual ~HelloWorld();
     bool onTouchBegan(Touch* touch, Event* event) override;
//    virtual void onTouchMoved(const cocos2d::Touch* touch, cocos2d::Event* event) override;
     void onTouchEnded(Touch* touch, Event* event) override;
//    virtual void onTouchCancelled(const cocos2d::Touch* touch, cocos2d::Event* event) override;
    
    virtual bool init() override;

    static cocos2d::Scene* scene();

    //BOB ADD
    void LoadRobotImage();
    
    void CreatUIInterFace();  //建立所有介面畫面
    void Hide_allLayer();
    // a selector callback
    void menuCloseCallback(Ref* sender);

    
    void ChangLaverToScanning(); //切換頁面 修改變數
    void  HideLayer();
    void ShowLayer();
    
   
    bool isTouchEnabled() const;
    virtual void setTouchEnabled(bool enabled);
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
