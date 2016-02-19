#include <ImageList.h>
#include <HelloWorldScene.h>

USING_NS_CC;
using namespace std;

#if PLAYERSTAGE
#include <libplayerc++/playerc++.h>

#endif

extern HelloWorld* m_MainView;

#if PLAYERSTAGE
using namespace PlayerCc;
extern PlayerClient *pPlayerclient;
extern DispatcherProxy *pDisProxy;
extern UI_RobotScanning* m_pUIRobotSacnning;
#endif

UI_RobotRemote::UI_RobotRemote():
touchEnabled(false),
touchListener(nullptr),
PageType(REMOTEPG_LAYER),
iSelRemoteBtn(REMOTEBTN_BK)
{
    for(int i=0;i<REMOTEPG_SIZE;i++)
    {
        auto Page = new SpriteVector;
        m_SpriteVectors.push_back(Page);
        auto Page2 = new LabelVector;
        m_LabelVectors.push_back(Page2);
    }

}
UI_RobotRemote::~UI_RobotRemote()
{
    for(auto pData:m_SpriteVectors)
    {
        delete pData;
    }
    for(auto pData:m_LabelVectors)
    {
        delete pData;
    }
    m_SpriteVectors.clear();
    m_LabelVectors.clear();
}

bool UI_RobotRemote::init()
{  
	if (!Layer::init()) 
	{  
		return false;  
	}  
	InitUI();  
	return true;  
}  
void UI_RobotRemote::SetupUIBtn(Sprite* pSprite,int iSel,bool bOnOff)
{
	SpriteFrame *pFrame=NULL;
	switch(iSel)
	{
	case REMOTEBTN_STOP:
		if(bOnOff)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_STOP_PRE);
		else
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_STOP_DEF);
		break;
	case REMOTEBTN_FORWARD:
		if(bOnOff)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_UP_PRE);
		else
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_UP_DEF);
		break;
	case REMOTEBTN_LEFT:
		if(bOnOff)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_LEFT_PRE);
		else
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_LEFT_DEF);
		break;
	case REMOTEBTN_RIGHT:
		if(bOnOff)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_RIGHT_PRE);
		else
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_RIGHT_DEF);
		break;
	case REMOTEBTN_GODOCK:
		if(bOnOff)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_GO_PRE);
		else
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_GO_DEF);
		break;
	case REMOTEBTN_AUTO:		
		if(iSelRemoteBtn==iSel)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_AUTO_FOCUS);
		else if(bOnOff)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_AUTO_PRE);
		else
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_AUTO_DEF);
		break;
	case REMOTEBTN_SPIRAL:
		if(iSelRemoteBtn==iSel)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_SPIRAL_FOCUS);
		else if(bOnOff)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_SPIRAL_PRE);
		else
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_SPIRAL_DEF);
		break;
	case REMOTEBTN_WALL:
		if(iSelRemoteBtn==iSel)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_WALL_FOCUS);
		else if(bOnOff)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_WALL_PRE);
		else
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_WALL_DEF);
		break;
	case REMOTEBTN_EXPLOREMAP:
		if(iSelRemoteBtn==iSel)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_EXPLORE_FOCUS);
		else if(bOnOff)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_EXPLORE_PRE);
		else
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_EXPLORE_DEF);
		break;
	case REMOTEBTN_VOICE:
		if(iSelRemoteBtn==iSel)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_VOICE_FOCUS);
		else if(bOnOff)
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_VOICE_PRE);
		else
			pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_ROBOT_BTN_VOICE_DEF);
		break;

	default: // 沒有退應的按鍵
		return;
	}
	if(pFrame!=NULL)
        pSprite->setDisplayFrame(pFrame);
}

void UI_RobotRemote::InitUI_RemotePage()
{
    auto pageSprite = m_SpriteVectors.at(REMOTEPG_STATE);
//    auto pageLabel = m_LabelVectors.at(REMOTEPG_STATE);
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍

	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point BtnSite_Forward=Vec2(242*W_unit,winSize.height-370*H_unit);
	Point BtnSite_Left=Vec2(111*W_unit,winSize.height-503*H_unit);
	Point BtnSite_Right=Vec2(731*W_unit,winSize.height-503*H_unit);
	Point BtnSite_GoDock=Vec2(244*W_unit,winSize.height-990*H_unit);
	Point BtnSite_Stop=Vec2(330*W_unit,winSize.height-590*H_unit);
	Point BtnSite_Auto=Vec2(70*W_unit,winSize.height-1395*H_unit);
	Point BtnSite_Spiral=Vec2(320*W_unit,winSize.height-1395*H_unit);
	Point BtnSite_WallFollow=Vec2(570*W_unit,winSize.height-1395*H_unit);
	Point BtnSite_ExploreMap=Vec2(820*W_unit,winSize.height-1395*H_unit);


	Point LabeSite_Welcome=Vec2(77*W_unit,winSize.height-375*H_unit);

	//設定背景
//	Sprite *pSpriteBK=Sprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
    addChild(pSpriteBK); //加入 layer 層
    pSpriteBK->setTag(REMOTEBTN_BK);
    pageSprite->pushBack(pSpriteBK);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_BK]=pageSprite->size()-1;

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	auto pLabel = Label::createWithSystemFont(STR_TITAL_RemoteControl, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    pSpriteAction->setTag(REMOTEBTN_TITAL);
    pageSprite->pushBack(pSpriteAction);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_TITAL]=pageSprite->size()-1;
    
	//FORWARD
	auto pSprite=Sprite::createWithSpriteFrameName(S_ROBOT_BTN_UP_DEF);
    pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(BtnSite_Forward);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(REMOTEBTN_FORWARD);
    pageSprite->pushBack(pSprite);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_FORWARD]=pageSprite->size()-1;
    
//	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_FORWARD);
	//LEFT
	pSprite=Sprite::createWithSpriteFrameName(S_ROBOT_BTN_LEFT_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(BtnSite_Left);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(REMOTEBTN_LEFT);
    pageSprite->pushBack(pSprite);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_LEFT]=pageSprite->size()-1;
//	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_LEFT);
	//RIGHT
	pSprite=Sprite::createWithSpriteFrameName(S_ROBOT_BTN_RIGHT_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(BtnSite_Right);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(REMOTEBTN_RIGHT);
    pageSprite->pushBack(pSprite);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_RIGHT]=pageSprite->size()-1;
//	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_RIGHT);
	//GODOCK
	pSprite=Sprite::createWithSpriteFrameName(S_ROBOT_BTN_GO_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(BtnSite_GoDock);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(REMOTEBTN_GODOCK);
    pageSprite->pushBack(pSprite);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_GODOCK]=pageSprite->size()-1;
//	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_GODOCK);
	//STOP
	pSprite=Sprite::createWithSpriteFrameName(S_ROBOT_BTN_STOP_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(BtnSite_Stop);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(REMOTEBTN_STOP);
    pageSprite->pushBack(pSprite);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_STOP]=pageSprite->size()-1;
//	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_STOP);


	//下面四個自動按鈕
	pSprite=Sprite::createWithSpriteFrameName(S_ROBOT_BTN_AUTO_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(BtnSite_Auto);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(REMOTEBTN_AUTO);
    pageSprite->pushBack(pSprite);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_AUTO]=pageSprite->size()-1;
//	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_AUTO);

	pSprite=Sprite::createWithSpriteFrameName(S_ROBOT_BTN_SPIRAL_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(BtnSite_Spiral);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(REMOTEBTN_SPIRAL);
    pageSprite->pushBack(pSprite);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_SPIRAL]=pageSprite->size()-1;
//	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_SPIRAL);

	pSprite=Sprite::createWithSpriteFrameName(S_ROBOT_BTN_WALL_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(BtnSite_WallFollow);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(REMOTEBTN_WALL);
    pageSprite->pushBack(pSprite);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_WALL]=pageSprite->size()-1;
//	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_WALL);

	pSprite=Sprite::createWithSpriteFrameName(S_ROBOT_BTN_EXPLORE_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(BtnSite_ExploreMap);
	pSpriteBK->addChild(pSprite,10);
    pSprite->setTag(REMOTEBTN_EXPLOREMAP);
    pageSprite->pushBack(pSprite);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_EXPLOREMAP]=pageSprite->size()-1;
//	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_EXPLOREMAP);

	//底部按鈕
	Sprite *pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2::ZERO);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
	pSprite=Sprite::createWithSpriteFrameName(S_ROBOT_BTN_VOICE_DEF); //聲音圖示
	pSprite->setPosition(Vec2(pSpriteDownBtn->getContentSize()/2));
	pSpriteDownBtn->addChild(pSprite,10);
    pSprite->setTag(REMOTEBTN_VOICE);
    pageSprite->pushBack(pSprite);
    m_itemIdx[REMOTEPG_STATE*100+REMOTEBTN_VOICE]=pageSprite->size()-1;
//	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_VOICE);
    pSpriteBK->setVisible(false);
    

}
void UI_RobotRemote::InitUI()
{
    winSize = Director::getInstance()->getVisibleSize();
    winOrigin = Director::getInstance()->getVisibleOrigin();
	InitUI_RemotePage();	
}
//顯示預設頁面
void UI_RobotRemote::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true);
	Show_RemotePage();
}
void UI_RobotRemote::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();

}

void UI_RobotRemote::Show_RemotePage()
{
    auto pSprite=getSpritePoint(REMOTEPG_STATE,REMOTEBTN_BK);

//	 Sprite* pSprite= (Sprite*)m_BtnArray[REMOTEPG_STATE]->objectAtIndex(REMOTEBTN_BK);
	 pSprite->setVisible(true);
}
void UI_RobotRemote::Hide_RemotePage()
{
    auto pSprite=getSpritePoint(REMOTEPG_STATE,REMOTEBTN_BK);

//	Sprite* pSprite= (Sprite*)m_BtnArray[REMOTEPG_STATE]->objectAtIndex(REMOTEBTN_BK);
	pSprite->setVisible(true);
}
void UI_RobotRemote::Hide_AllPage()
{
	Hide_RemotePage();
	PageType = REMOTEPG_LAYER;
}

//滑鼠點下!判斷是那個元件
bool UI_RobotRemote::onTouchBegan(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr)
    {
        return true;
    }
    log("UI_RobotRemote:%s",__func__);
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
    auto sprites=m_SpriteVectors.at(REMOTEPG_STATE);
    auto bTouch =false;
    for(auto pSprite:*sprites)
    {
        auto iIdx=pSprite->getTag();
        if(pSprite->isVisible() && iIdx!=0)
        {
            auto rectBox= pSprite->getBoundingBox();
            rectBox.origin+=winOrigin;
            bTouch = rectBox.containsPoint(nodePosition);
            if(bTouch)
            {
                SetupUIBtn(pSprite ,iIdx,bTouch);
                iSelRemoteBtn=iIdx;
                ControlFunction(iIdx);
                break;
            }
        }
    }
//	for (int i = REMOTEBTN_TITAL; i < m_BtnArray[REMOTEPG_STATE]->count(); i++)
//	{  
//		Sprite* pSprite = (Sprite*)m_BtnArray[REMOTEPG_STATE]->objectAtIndex(i); 
//		bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
//		if(bTouch)
//		{
//			SetupUIBtn(pSprite ,i,bTouch); 
//			iSelRemoteBtn=i;
//			ControlFunction(i);
//			break;
//		}
//	}  
	return true;
}
void UI_RobotRemote::ControlFunction(int iTag)
{
#if PLAYERSTAGE
	int iCmdNum=PLAYER_DISPATCH_AI_STOP;
#else
//    int iCmdNum=0;
    iTag=REMOTEBTN_TITAL;
#endif
	 uint32_t mapIndex[1];
	 mapIndex[0]=10;
    
#if PLAYERSTAGE
	 if(pDisProxy==NULL) //尚未連接
	 {
		 iTag=REMOTEBTN_TITAL; 
	 }
#endif
    
	switch(iTag)
	{
#if PLAYERSTAGE
	case REMOTEBTN_STOP:

		iCmdNum=PLAYER_DISPATCH_AI_STOP;
		pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
		iCmdNum,											
		PLAYER_DISPATCH_PM_STANDBY,							
		sizeof(mapIndex)/sizeof(uint32_t),					
		mapIndex);
		RobotMapUpdate(); //更新地圖
		break;
	case REMOTEBTN_FORWARD:

		iCmdNum=PLAYER_DISPATCH_AI_MOVE_FORWARD;
			pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
			iCmdNum,											
			PLAYER_DISPATCH_PM_STANDBY,							
			sizeof(mapIndex)/sizeof(uint32_t),					
			mapIndex);
		break;
	case REMOTEBTN_LEFT:
		iCmdNum=PLAYER_DISPATCH_AI_TURN_LEFT;
			pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
			iCmdNum,											
			PLAYER_DISPATCH_PM_STANDBY,							
			sizeof(mapIndex)/sizeof(uint32_t),					
			mapIndex);
		break;
	case REMOTEBTN_RIGHT:
		iCmdNum=PLAYER_DISPATCH_AI_TURN_RIGHT;
			pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
			iCmdNum,											
			PLAYER_DISPATCH_PM_STANDBY,							
			sizeof(mapIndex)/sizeof(uint32_t),					
			mapIndex);
		break;
	case REMOTEBTN_GODOCK:
		{ //goto target 0,0,0
			uint32_t mapIndex[1];
			mapIndex[0]=0;
			player_pose2d_t goal;
			goal.px=0; //unit:m
			goal.py=0; //unit:m
			goal.pa =0;  //unit:rad

			pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
				PLAYER_DISPATCH_AI_MOVE2TARGET,											
				PLAYER_DISPATCH_PM_TURBO,							
				sizeof(mapIndex)/sizeof(uint32_t),					
				mapIndex,
				&goal);
		}
            
		//回到DOCK
		//iCmdNum=PLAYER_DISPATCH_AI_FIND_DOCK;
		//pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
		//	iCmdNum,											
		//	PLAYER_DISPATCH_PM_STANDBY,							
		//	sizeof(mapIndex)/sizeof(uint32_t),					
		//	mapIndex);	
		break;
	case REMOTEBTN_AUTO:
		iCmdNum=PLAYER_DISPATCH_AI_AUTO;
		pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
			iCmdNum,											
			PLAYER_DISPATCH_PM_STANDBY,							
			sizeof(mapIndex)/sizeof(uint32_t),					
			mapIndex);		
		break;
	case REMOTEBTN_SPIRAL:
		iCmdNum=PLAYER_DISPATCH_AI_SPIRAL_LARGE;
		pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
			iCmdNum,											
			PLAYER_DISPATCH_PM_STANDBY,							
			sizeof(mapIndex)/sizeof(uint32_t),					
			mapIndex);	
		break;
	case REMOTEBTN_WALL:
		iCmdNum=PLAYER_DISPATCH_AI_WALL_FOLLOW;
		pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
			iCmdNum,											
			PLAYER_DISPATCH_PM_STANDBY,							
			sizeof(mapIndex)/sizeof(uint32_t),					
			mapIndex);	
		break;
	case REMOTEBTN_EXPLOREMAP:
		iCmdNum=PLAYER_DISPATCH_AI_EXPLORE;
		pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
			iCmdNum,											
			PLAYER_DISPATCH_PM_STANDBY,							
			sizeof(mapIndex)/sizeof(uint32_t),					
			mapIndex);	
		break;
#endif
            
	case REMOTEBTN_TITAL: //返回上一頁
		HideLayer();
		ChangLayer_Main();
		break;
	}
}
void UI_RobotRemote::onTouchMoved(Touch* touch, Event* event)
{
//	CCPoint touchLocation = touch->getLocation();    
//	CCPoint nodePosition = convertToNodeSpace( touchLocation );

	//m_test->MouseMove(b2Vec2(nodePosition.x,nodePosition.y));        
}

void UI_RobotRemote::onTouchEnded(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr){ return;}
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
    
//	CCPoint touchLocation = touch->getLocation();
//	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	//執行函數
    auto sprites=m_SpriteVectors.at(REMOTEPG_STATE);
//    auto bTouch = false;
    
    for(auto pSprite:*sprites)
//	for (int i = REMOTEBTN_TITAL; i < m_BtnArray[REMOTEPG_STATE]->count(); i++)
	{
        auto iIdx=pSprite->getTag();
        SetupUIBtn(pSprite ,iIdx,false); //現階段採用笨方式 不斷重畫!日後改為onmousemove 事件

//		Sprite* pSprite = (Sprite*)m_BtnArray[REMOTEPG_STATE]->objectAtIndex(i);
//		bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
		SetupUIBtn(pSprite ,iIdx,false); //現階段採用笨方式 不斷重畫!日後改為onmousemove 事件

	} 
	//m_test->MouseUp(b2Vec2(nodePosition.x,nodePosition.y));
}
void UI_RobotRemote::setTouchEnabled(bool enable)
{
    if (enable == touchEnabled)
    {
        return;
    }
    touchEnabled = enable;
    
    if(touchEnabled)
    {
        touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true); //設置是否想下傳遞觸摸
        CC_SAFE_RETAIN(touchListener);
        
        touchListener->onTouchBegan = CC_CALLBACK_2(UI_RobotRemote::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(UI_RobotRemote::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(UI_RobotRemote::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
    }
    else
    {
        _eventDispatcher->removeEventListener(touchListener);
        CC_SAFE_RELEASE_NULL(touchListener);
    }
}
bool UI_RobotRemote::isTouchEnabled() const
{
    return touchEnabled;
}
Sprite* UI_RobotRemote::getSpritePoint(int iPageTyep,int iPageIdx)
{
    return m_SpriteVectors.at(iPageTyep)->at(m_itemIdx[iPageTyep*100+iPageIdx]);
}
Label* UI_RobotRemote::getLabelPoint(int iPageTyep,int iPageIdx)
{
    return m_LabelVectors.at(iPageTyep)->at(m_itemIdx[iPageTyep*100+iPageIdx]);
}

//////////////////////////////////////////////////////////////////////////
//Robot Sacnning View
