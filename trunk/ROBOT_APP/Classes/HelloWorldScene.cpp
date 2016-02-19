#include "ImageList.h"
#include "HelloWorldScene.h"
#include <libplayerc++/playerc++.h>
USING_NS_CC;
using namespace std;
using namespace PlayerCc;

HelloWorld* m_MainView = NULL; 
UI_RobotRemote* m_UIRobotRemote = NULL;
UI_RobotScanning* m_pUIRobotSacnning = NULL;
UI_RobotMain* m_pUIRobotMain = NULL;
UI_MAP* m_pUIRobotMap = NULL; 
UI_CLEANJOB* m_pUICleanJOB = NULL;
UI_Reservation* m_pUIReservation = NULL;
UI_RobotSetup* m_pUIRobotSetup = NULL;

/// Player Client
PlayerClient *pPlayerclient=NULL;
PowerProxy	 *pPowerProxy=NULL;		// powerProxy
DispatcherProxy *pDisProxy=NULL;
MapProxy *pMapProxy=NULL;
/// proxy for path-planning
//PlannerProxy *pPlannerProxy=NULL;
Position2dProxy *pPosition2dProxy=NULL;
//player_map_info_t stMapInfo;

static int G_LayerIdx=Layer_Logo;

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();
	m_MainView= layer; 
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	//處理鍵盤事件
	extern void Win32SetKeyLayer(HelloWorld *layer);  
	Win32SetKeyLayer(layer);  
#endif
	//m_RobotView.LoadRobotControl();
	//畫面初始化
	m_UIRobotRemote=UI_RobotRemote::create();
	m_MainView->addChild(m_UIRobotRemote,10,TAG_UI_RobotRemote_Bk);
	

	m_pUIRobotSacnning=UI_RobotScanning::create();
	m_MainView->addChild(m_pUIRobotSacnning,10,TAG_UI_RobotScanning_Bk);
	

	m_pUIRobotMain=UI_RobotMain::create();
	m_MainView->addChild(m_pUIRobotMain,10,TAG_UI_RobotMain_Bk);

	m_pUIRobotMap=UI_MAP::create();
	m_MainView->addChild(m_pUIRobotMap,10,TAG_UI_RobotMap_Bk );
	
	m_pUICleanJOB=UI_CLEANJOB::create();
	m_MainView->addChild(m_pUICleanJOB,10,TAG_UI_CleanJob_Bk );

	m_pUIReservation=UI_Reservation::create();
	m_MainView->addChild(m_pUIReservation,10,TAG_UI_Reservation_Bk );

	m_pUIRobotSetup=UI_RobotSetup::create();
	m_MainView->addChild(m_pUIRobotSetup,10,TAG_UI_RobotSetup_Bk );
	m_MainView->Hide_allLayer(); //隱藏所有頁面

	return scene;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
void HelloWorld::KeyDownEven_Remote( WPARAM wParam)
{
	switch( wParam )
	{
	case 0x41://A
	case VK_LEFT:
		m_UIRobotRemote->ControlFunction(REMOTEBTN_LEFT);
		break;
	case 0x44://D
	case VK_RIGHT:
		m_UIRobotRemote->ControlFunction(REMOTEBTN_RIGHT);
		break;
	case 0x57://W
	case VK_UP:
		m_UIRobotRemote->ControlFunction(REMOTEBTN_FORWARD);
		break;
	case 0x53://S
	case VK_DOWN:
		m_UIRobotRemote->ControlFunction(REMOTEBTN_STOP);
		break;
	}

}
void HelloWorld::onWin32KeyEvent( UINT message,WPARAM wParam, LPARAM lParam )  
{  
	 CCLog("onWin32KeyEvent message %d wParam 0x%x lParam %d", message, wParam, lParam);  
	/*  kVK_ANSI_W
	// Up  
	Win32KeyHook message 256 wParam 38 lParam 21495809  
	onWin32KeyEvent message 256 wParam 38 lParam 21495809  
	Win32KeyHook message 257 wParam 38 lParam -1052246015  
	onWin32KeyEvent message 257 wParam 38 lParam -1052246015  
	// Down  
	Win32KeyHook message 256 wParam 40 lParam 22020097  
	onWin32KeyEvent message 256 wParam 40 lParam 22020097  
	Win32KeyHook message 257 wParam 40 lParam -1051721727  
	onWin32KeyEvent message 257 wParam 40 lParam -1051721727  
	// Left  
	Win32KeyHook message 256 wParam 37 lParam 21692417  
	onWin32KeyEvent message 256 wParam 37 lParam 21692417  
	Win32KeyHook message 257 wParam 37 lParam -1052049407  
	onWin32KeyEvent message 257 wParam 37 lParam -1052049407  
	// Right  
	Win32KeyHook message 256 wParam 39 lParam 21823489  
	onWin32KeyEvent message 256 wParam 39 lParam 21823489  
	Win32KeyHook message 257 wParam 39 lParam -1051918335  
	onWin32KeyEvent message 257 wParam 39 lParam -1051918335  
	*/  
	 switch(G_LayerIdx)
	 { //根據現在不同頁面 來決定 要做何事
	 case Layer_Main:
	 case Layer_Remote:
	 case Layer_Map:
		 switch( message )
		 {
		 case WM_KEYDOWN:
			 KeyDownEven_Remote( wParam );
			 break;
		 case WM_KEYUP:
			 break;
		 case WM_CHAR: //按下 放開都會觸發
			 // Deliberately empty - all handled through key up and down events
			 break;
		 default:
			 // Not expected to get here!!
			 CC_ASSERT( false );
			 break;
		 }


		 break;
	 case Layer_Logo:	 
	 case Layer_Scanning:	 
	 case Layer_CleanJob:
	 case Layer_Reservation:
	 case Layer_RobotSetup:
	 default:
		 break;
	 }
	 //if (message == 256)  
	 //{  
		// switch( wParam )
		// {
		// case VK_LEFT:
		//	 break;
		// case VK_RIGHT:
		//	 break;
		// case VK_UP:
		//	 break;
		// case VK_DOWN:
		//	 break;
		// }
	 //}  
}  
#endif  

HelloWorld::~HelloWorld()
{
	if(pPlayerclient!=NULL)
	{
		pPlayerclient->StopThread();
		delete pPlayerclient;
		pPlayerclient=NULL;
	}

	//if(pDisProxy!=NULL)
	//	delete pDisProxy;
	//if(pPowerProxy!=NULL)
	//	delete pPowerProxy;

	//if(pMapProxy!=NULL)
	//	delete pMapProxy;
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	//BOBAdd
	LoadSetting();	//載入設定檔
	LoadRobotImage(); //載入所有圖檔
	
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
 #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback));
	
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
								origin.y + pCloseItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);
#endif
	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label
	
	CCLabelTTF* pLabel = CCLabelTTF::create("MSI ROBOT", "Arial", 24);
	
	// position the label on the center of the screen
	pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
							origin.y + visibleSize.height - pLabel->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	CCSprite* pSprite = CCSprite::createWithSpriteFrameName(S_LOGO_BKIMG); // 第一次載圖 如果沒有圖 會在此掛掉
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);
	
	CCSprite* pLogoSprite = CCSprite::createWithSpriteFrameName(S_LOGO_ICON);
	pLogoSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height*2/3 + origin.y));
	pSprite->addChild(pLogoSprite);

	//載圖測試
	//FILE *pfp=fopen("V:\\MSI_PCAPP\\2015ROBOTAPP\\Resources\\DebugGUIMap.bmp","rb");
	//unsigned char *pImageTemp;
	//int iHeadSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	//int iSize=sizeof(unsigned char)*3 * 2048*2048 +iHeadSize;
	//pImageTemp=(unsigned char*)malloc (iSize);
	//int iRetSize=fread(pImageTemp,sizeof(unsigned char),iSize,pfp);
	//if(iRetSize!=iSize)
	//{
	//	DBG("EEROR");
	//}
	//fclose(pfp);
	//CCTexture2D *texture = new CCTexture2D();		
	//bool val = texture->initWithData(pImageTemp,kCCTexture2DPixelFormat_RGB888,2048,2048,CCSizeMake(2048,2048));
	//
	////CCSprite * pMap =CCSprite::create("RobotAll.png");
	//CCSprite * pMap =CCSprite::create();
	//pMap->setTexture(texture);
	//pMap->setTextureRect(CCRectMake(0,0,2048,2048));
	//int iWidth=pMap->getContentSize().width;
	//pLogoSprite->addChild(pMap);

	//CCSprite * pMap2 =CCSprite::create("DebugGUIMap.png");
	//pLogoSprite->addChild(pMap2);
	//BOB add even setting
	InitSpriteEven();
	bInLayer=true;
	UpdateTime=0;
	scheduleUpdate(); //開啟默認定時器
	return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}
//隱藏所有頁面
void  HelloWorld::Hide_allLayer()
{
	m_UIRobotRemote->HideLayer();
	m_pUIRobotSacnning->HideLayer();
	m_pUIRobotMain->HideLayer();	
	m_pUIRobotMap->HideLayer();
	m_pUICleanJOB->HideLayer();
	m_pUIReservation-> HideLayer();
	m_pUIRobotSetup->HideLayer();
	G_LayerIdx=Layer_Logo;
}
void HideLayer_All()
{
	m_MainView->Hide_allLayer();	
}
//控制頁面切換
void ChangLayer_Main()
{
	m_MainView->Hide_allLayer();	
	G_LayerIdx=Layer_Main;
	m_pUIRobotMain->ShowLayer();
}
void ChangLayer_Scanning()
{
	m_MainView->Hide_allLayer();	
	G_LayerIdx=Layer_Scanning;
	m_pUIRobotSacnning->ShowLayer();
}
void ChangLayer_Remote()
{
	m_MainView->Hide_allLayer();	
	G_LayerIdx=Layer_Remote;
	m_UIRobotRemote->ShowLayer();
}
void ChangLayer_Map()
{
	m_MainView->Hide_allLayer();	
	G_LayerIdx=Layer_Map;
	m_pUIRobotMap->ShowLayer();
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

//載入所有圖檔至frameCache
void HelloWorld::LoadRobotImage()
{
	CCSpriteFrameCache* cache=CCSpriteFrameCache::sharedSpriteFrameCache();

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
			CCMessageBox("ERROR","NO Image file");
		}
		
	}
	//cache->addSpriteFramesWithFile(IMAGELIST_ROBOT_CONTROL);

	//cache->addSpriteFramesWithFile(IMAGELIST_ROBOT_CONTROL_02);
	
}
//建立事件表
void HelloWorld::InitSpriteEven()
{
	// 創建一個事件監聽器類型為 OneByOne 的單點觸摸
	//auto listener1 = EventListenerTouchOneByOne::create();
	registerWithTouchDispatcher(); //註冊背景點選事件
}
//默認的定時器 每秒60次 每個FPS 一次
//如果5秒沒有點選 主動進入

void HelloWorld::update(float dt)
{
	UpdateTime++;
	if(UpdateTime>=120 && bInLayer) //更新狀態 OR 時間
	{
		unscheduleUpdate();    //取消默認定時器
		ChangLayer_Scanning(); // 主程式
		bInLayer=false;
	}
	if(!bInLayer)
		unscheduleUpdate();    //取消默認定時器
}
void HelloWorld::registerWithTouchDispatcher()
{
	// higher priority than dragging
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

//滑鼠點下!判斷是那個元件
bool HelloWorld::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    

	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	
	if(bInLayer)
	{
		unscheduleUpdate();    //取消默認定時器

		ChangLayer_Scanning(); // 主程式

		//ChangLayer_Remote();		
		//ChangLayer_Main();
		//ChangLayer_CleanJob();
		//ChangLayer_Reservation();
		bInLayer=false;
	}

	//if(bInLayer)
	//{
	//  m_pUIRobotSacnning->Show_SacnPage();
	//  bInLayer=false;
	//}
	return true;
}

void HelloWorld::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	//m_test->MouseMove(b2Vec2(nodePosition.x,nodePosition.y));        
}

void HelloWorld::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );

	//m_test->MouseUp(b2Vec2(nodePosition.x,nodePosition.y));
}
void HelloWorld::InitSetting() //初始化設定
{
	
}
//載入設定檔
void HelloWorld::LoadSetting()
{
	InitSetting();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	InitWinIni();
	
#endif
}
