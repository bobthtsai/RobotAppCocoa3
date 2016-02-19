#include "AppDefine.h"
#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "ImageList.h"

USING_NS_CC;

using namespace std;
HelloWorld* m_MainView = NULL;
UI_RobotScanning* m_pUIRobotSacnning = NULL;
UI_RobotMain* m_pUIRobotMain = NULL;
UI_MAP* m_pUIRobotMap = NULL;
UI_RobotRemote* m_UIRobotRemote = NULL;
UI_Reservation* m_pUIReservation = NULL;
UI_CLEANJOB* m_pUICleanJOB = NULL;
UI_RobotSetup* m_pUIRobotSetup = NULL;


#if  PLAYERSTAGE
using namespace PlayerCc;
/// Player Client
PlayerClient *pPlayerclient=NULL;
PowerProxy	 *pPowerProxy=NULL;		// powerProxy
DispatcherProxy *pDisProxy=NULL;
MapProxy *pMapProxy=NULL;
/// proxy for path-planning
//PlannerProxy *pPlannerProxy=NULL;
Position2dProxy *pPosition2dProxy=NULL;
//player_map_info_t stMapInfo;
#endif

static int G_LayerIdx=Layer_Logo;

HelloWorld::HelloWorld():
touchEnabled(false),
touchListener(nullptr),
bInLayer(true),
UpdateTime(0)
{
    
}
HelloWorld::~HelloWorld()
{
}
Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();  //call HelloWorld()
    m_MainView = layer;
    // add layer as a child to scene
    scene->addChild(layer);
    
    //BOBADD
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    //PC版按鍵控制
    extern void Win32SetKeyLayer(HelloWorld *layer);
    Win32SetKeyLayer(layer);
#endif
    m_MainView->CreatUIInterFace();

    // return the scene
    return scene;
}
void HelloWorld::CreatUIInterFace()
{
    m_pUIRobotSacnning=UI_RobotScanning::create();
    m_MainView->addChild(m_pUIRobotSacnning,10,TAG_UI_RobotScanning_Bk);
   
    m_pUIRobotMain=UI_RobotMain::create();
    m_MainView->addChild(m_pUIRobotMain,10,TAG_UI_RobotMain_Bk);
    
    m_pUIRobotMap=UI_MAP::create();
    m_MainView->addChild(m_pUIRobotMap,10,TAG_UI_RobotMap_Bk );
    
    m_UIRobotRemote=UI_RobotRemote::create();
    m_MainView->addChild(m_UIRobotRemote,10,TAG_UI_RobotRemote_Bk);
   
    m_pUIReservation=UI_Reservation::create();
    m_MainView->addChild(m_pUIReservation,10,TAG_UI_Reservation_Bk );
    
    m_pUICleanJOB=UI_CLEANJOB::create();
    m_MainView->addChild(m_pUICleanJOB,10,TAG_UI_CleanJob_Bk );
    
    
    m_pUIRobotSetup=UI_RobotSetup::create();
    m_MainView->addChild(m_pUIRobotSetup,10,TAG_UI_RobotSetup_Bk );
    m_MainView->Hide_allLayer();
    
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    auto origin = Director::getInstance()->getVisibleOrigin();

    LoadRobotImage();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
//    auto closeItem = MenuItemImage::create(
//                                        "CloseNormal.png",
//                                        "CloseSelected.png",
//                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
//    
//    closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
//    auto menu = Menu::create(closeItem, nullptr);
//    menu->setPosition(Vec2::ZERO);
//    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    auto pSprite = Sprite::createWithSpriteFrameName(S_LOGO_BKIMG);
    winSize = pSprite->getBoundingBox().size;
    winOrigin =Director::getInstance()->getVisibleOrigin();
    auto plabel = Label::createWithTTF("MSI ROBOT", "fonts/arial.ttf", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
//    label->setPosition(origin.x + visibleSize.width/2,
//                            origin.y + visibleSize.height - label->getContentSize().height);
    plabel->setPosition(Vec2(winSize.width/2,winSize.height/5*4));
    // add the label as a child to this layer
    pSprite->addChild(plabel, 1);

    //載入背景
    
    pSprite->setPosition(Vec2(winSize / 2) + winOrigin);
    
    auto pLogoSprite = Sprite::createWithSpriteFrameName(S_LOGO_ICON);
    pLogoSprite->setPosition(Vec2(winSize.width/2, winSize.height/5*2 ));
    pSprite->addChild(pLogoSprite);
    
    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    // add "HelloWorld" splash screen" 原始測試程式
//    auto sprite = Sprite::create("HelloWorld.png");
//
//    // position the sprite on the center of the screen
//    sprite->setPosition(Vec2(visibleSize / 2) + origin);
//
//    // add the sprite as a child to this layer
//    this->addChild(sprite);

    scheduleUpdate();
    ShowLayer();
    return true;
}
/////////////////////////////////
//觸碰設定
void HelloWorld::setTouchEnabled(bool enable)
{
    if (enable == touchEnabled)
    {
        return;
    }
    touchEnabled = enable;
    
    if(touchEnabled)
    {
        //多點觸碰
        // auto listener = EventListenerTouchAllAtOnce::create();
        //單點觸碰
        touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true); //設置是否想下傳遞觸摸
        CC_SAFE_RETAIN(touchListener);
        
        touchListener->onTouchBegan = CC_CALLBACK_2(self::onTouchBegan, this);
        //    listener->onTouchMoved = CC_CALLBACK_2(self::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(self::onTouchEnded, this);
        //    listener->onTouchCancelled = CC_CALLBACK_2(self::onTouchCancelled, this);
        
         _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
    }
    else
    {
        _eventDispatcher->removeEventListener(touchListener);
        CC_SAFE_RELEASE_NULL(touchListener);
    }
 }
bool HelloWorld::isTouchEnabled() const
{
    return touchEnabled;
}
///
//事件處理
////座標轉換公式
//inline Point locationInGLFromTouch(Touch& touch)
//{
//    auto director = Director::getInstance();
//    return director->convertToGL(touch.getLocationInView());
//}
//重要觸摸事件設定
bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, Event* event)
{
     log("HelloWorld:%s",__func__);
    return true;
}
//void HelloWorld::onTouchMoved(const cocos2d::Touch* touch, Event* event){}
//void HelloWorld::onTouchCancelled(const cocos2d::Touch* touch, Event* event){}
void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    log("HelloWorld::onTouchesEnded.. ");
    target->setOpacity(255);
    ChangLaverToScanning();

}
void HelloWorld::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

//∏¸§J©“¶≥πœ¿…¶‹frameCache
void HelloWorld::LoadRobotImage()
{
    SpriteFrameCache* cache=SpriteFrameCache::getInstance();
    int iWinIDx=GetWindwsSizeIdx();
    int iLoop =pIMAGELISTSize[iWinIDx];
    char *pName;
    
    for(int i=0;i<iLoop ;i++)
    {
        switch(iWinIDx)
        {
            case 0:
                pName=(char*)IMAGELIST_M[i];break;
            case 1:
                pName=(char*)IMAGELIST_H[i];break;
            case 2:
                pName=(char*)IMAGELIST_XH[i];break;
            case 3:
                pName=(char*)IMAGELIST_XXH[i];break;
            default:
                pName=NULL;
        }
        if(pName!=NULL)
            cache->addSpriteFramesWithFile(pName);
        else
        {
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) //BOBFORIOS
            MessageBox("ERROR","NO Image file");
//#endif
        }
        
    }
}
void HelloWorld::ChangLaverToScanning()
{
    unscheduleUpdate();
    ChangLayer_Scanning();
//    ChangLayer_Map(); //debug
    bInLayer=false;
    
    
}
void HelloWorld::update(float dt)
{
    UpdateTime++;
    if(UpdateTime>=120 && bInLayer) //ßÛ∑s™¨∫A OR Æ…∂°
    {
        UpdateTime =0;
        ChangLaverToScanning();
    }
    if(!bInLayer)
        unscheduleUpdate();    //®˙Æ¯¿qª{©wÆ…æπ
}
void HelloWorld::ShowLayer()
{
    setTouchEnabled(true);
}

void  HelloWorld::HideLayer()
{
    setTouchEnabled(false);
}
//¡Ù¬√©“¶≥≠∂≠±
void  HelloWorld::Hide_allLayer()
{
    HideLayer();
    m_pUIRobotSacnning->HideLayer();
    m_pUIRobotMain->HideLayer();
    m_pUIRobotMap->HideLayer();
    m_pUIReservation-> HideLayer();
    m_UIRobotRemote->HideLayer();
    m_pUICleanJOB->HideLayer();
    m_pUIRobotSetup->HideLayer();
    G_LayerIdx=Layer_Logo;
}
void HideLayer_All()
{
    m_MainView->Hide_allLayer();
}

void ChangLayer_Scanning()
{
    m_MainView->Hide_allLayer();
    G_LayerIdx=Layer_Scanning;
    m_pUIRobotSacnning->ShowLayer();
}
void ChangLayer_Main()
{
    m_MainView->Hide_allLayer();
    G_LayerIdx=Layer_Main;
    m_pUIRobotMain->ShowLayer();
}

void ChangLayer_Map()
{
    m_MainView->Hide_allLayer();
    G_LayerIdx=Layer_Map;
    m_pUIRobotMap->ShowLayer();
}

void ChangLayer_Remote()
{
    m_MainView->Hide_allLayer();
    G_LayerIdx=Layer_Remote;
    m_UIRobotRemote->ShowLayer();
}

void ChangLayer_CleanJob()
{
    m_MainView->Hide_allLayer();
    G_LayerIdx=Layer_CleanJob;
    m_pUICleanJOB->ShowLayer();
}
void ChangLayer_Reservation()
{
    m_MainView->Hide_allLayer();
    G_LayerIdx=Layer_Reservation;
    m_pUIReservation->ShowLayer();
}
void ChangLayer_RobotSetup()
{
    m_MainView->Hide_allLayer();
    G_LayerIdx=Layer_RobotSetup;
    m_pUIRobotSetup->ShowLayer();
}
