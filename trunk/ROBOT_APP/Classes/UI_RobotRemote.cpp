#include <ImageList.h>
#include <HelloWorldScene.h>
#include <libplayerc++/playerc++.h>
USING_NS_CC;
using namespace std;
using namespace PlayerCc;

extern HelloWorld* m_MainView; 
/// Player Client
extern PlayerClient *pPlayerclient;
extern DispatcherProxy *pDisProxy;
extern UI_RobotScanning* m_pUIRobotSacnning;

UI_RobotRemote::UI_RobotRemote()
{
	PageType = REMOTEPG_LAYER;
	iSelRemoteBtn=REMOTEBTN_BK;
}
UI_RobotRemote::~UI_RobotRemote()
{
	for(int i=0;i<REMOTEPG_SIZE;i++)
		m_BtnArray[i]->release();

}

bool UI_RobotRemote::init()
{  
	if (!CCLayer::init()) 
	{  
		return false;  
	}  
	InitUI();  
	return true;  
}  
void UI_RobotRemote::SetupUIBtn(CCSprite* pSprite,int iSel,bool bOnOff)
{
	CCSpriteFrame *pFrame=NULL;
	switch(iSel)
	{
	case REMOTEBTN_STOP:
		if(bOnOff)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_STOP_PRE);
		else
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_STOP_DEF);
		break;
	case REMOTEBTN_FORWARD:
		if(bOnOff)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_UP_PRE);
		else
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_UP_DEF);
		break;
	case REMOTEBTN_LEFT:
		if(bOnOff)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_LEFT_PRE);
		else
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_LEFT_DEF);
		break;
	case REMOTEBTN_RIGHT:
		if(bOnOff)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_RIGHT_PRE);
		else
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_RIGHT_DEF);
		break;
	case REMOTEBTN_GODOCK:
		if(bOnOff)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_GO_PRE);
		else
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_GO_DEF);
		break;
	case REMOTEBTN_AUTO:		
		if(iSelRemoteBtn==iSel)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_AUTO_FOCUS);
		else if(bOnOff)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_AUTO_PRE);
		else
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_AUTO_DEF);
		break;
	case REMOTEBTN_SPIRAL:
		if(iSelRemoteBtn==iSel)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_SPIRAL_FOCUS);
		else if(bOnOff)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_SPIRAL_PRE);
		else
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_SPIRAL_DEF);
		break;
	case REMOTEBTN_WALL:
		if(iSelRemoteBtn==iSel)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_WALL_FOCUS);
		else if(bOnOff)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_WALL_PRE);
		else
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_WALL_DEF);
		break;
	case REMOTEBTN_EXPLOREMAP:
		if(iSelRemoteBtn==iSel)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_EXPLORE_FOCUS);
		else if(bOnOff)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_EXPLORE_PRE);
		else
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_EXPLORE_DEF);
		break;
	case REMOTEBTN_VOICE:
		if(iSelRemoteBtn==iSel)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_VOICE_FOCUS);
		else if(bOnOff)
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_VOICE_PRE);
		else
			pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_ROBOT_BTN_VOICE_DEF);
		break;

	default: // 沒有退應的按鍵
		return;
	}
	
	pSprite->setDisplayFrame(pFrame);
}

void UI_RobotRemote::InitUI_RemotePage()
{
	m_BtnArray[REMOTEPG_STATE] =  CCArray::createWithCapacity(MINFORI_SIZE); //按鈕存放
	m_BtnArray[REMOTEPG_STATE]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint BtnSite_Forward=CCPointMake(242*W_unit,winSize.height-370*H_unit);	
	CCPoint BtnSite_Left=CCPointMake(111*W_unit,winSize.height-503*H_unit);	
	CCPoint BtnSite_Right=CCPointMake(731*W_unit,winSize.height-503*H_unit);	
	CCPoint BtnSite_GoDock=CCPointMake(244*W_unit,winSize.height-990*H_unit);	
	CCPoint BtnSite_Stop=CCPointMake(330*W_unit,winSize.height-590*H_unit);	
	CCPoint BtnSite_Auto=CCPointMake(70*W_unit,winSize.height-1395*H_unit);	
	CCPoint BtnSite_Spiral=CCPointMake(320*W_unit,winSize.height-1395*H_unit);	
	CCPoint BtnSite_WallFollow=CCPointMake(570*W_unit,winSize.height-1395*H_unit);	
	CCPoint BtnSite_ExploreMap=CCPointMake(820*W_unit,winSize.height-1395*H_unit);	


	CCPoint LabeSite_Welcome=CCPointMake(77*W_unit,winSize.height-375*H_unit);

	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_ROBOT_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	m_BtnArray[REMOTEPG_STATE]->addObject(pSpriteBK);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_RemoteControl, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSpriteAction,REMOTEBTN_TITAL);
	//FORWARD
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_UP_DEF);
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(BtnSite_Forward);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_FORWARD);
	//LEFT
	pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_LEFT_DEF);
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(BtnSite_Left);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_LEFT);
	//RIGHT
	pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_RIGHT_DEF);
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(BtnSite_Right);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_RIGHT);
	//GODOCK
	pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_GO_DEF);
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(BtnSite_GoDock);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_GODOCK);
	//STOP
	pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_STOP_DEF);
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(BtnSite_Stop);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_STOP);


	//下面四個自動按鈕
	pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_AUTO_DEF);
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(BtnSite_Auto);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_AUTO);

	pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_SPIRAL_DEF);
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(BtnSite_Spiral);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_SPIRAL);

	pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_WALL_DEF);
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(BtnSite_WallFollow);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_WALL);

	pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_EXPLORE_DEF);
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(BtnSite_ExploreMap);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_EXPLOREMAP);

	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);
	pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_VOICE_DEF); //聲音圖示
	pSprite->setPosition(ccp(pSpriteBK->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pSprite,10);
	m_BtnArray[REMOTEPG_STATE]->insertObject(pSprite,REMOTEBTN_VOICE);

}
void UI_RobotRemote::InitUI()
{
	winSize = CCDirector::sharedDirector()->getVisibleSize(); //取得視窗大小
	InitUI_RemotePage();	
}
//顯示預設頁面
void UI_RobotRemote::ShowLayer()
{
	setVisible(true);
	//registerWithTouchDispatcher();
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
	 CCSprite* pSprite= (CCSprite*)m_BtnArray[REMOTEPG_STATE]->objectAtIndex(REMOTEBTN_BK);
	 pSprite->setVisible(true);	 
}
void UI_RobotRemote::Hide_RemotePage()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[REMOTEPG_STATE]->objectAtIndex(REMOTEBTN_BK);
	pSprite->setVisible(true);
}
void UI_RobotRemote::Hide_AllPage()
{
	Hide_RemotePage();
	PageType = REMOTEPG_LAYER;
}
void UI_RobotRemote::removeWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
}
void UI_RobotRemote::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}
//滑鼠點下!判斷是那個元件
bool UI_RobotRemote::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    

	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	for (int i = REMOTEBTN_TITAL; i < m_BtnArray[REMOTEPG_STATE]->count(); i++)
	{  
		CCSprite* pSprite = (CCSprite*)m_BtnArray[REMOTEPG_STATE]->objectAtIndex(i); 
		bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
		if(bTouch)
		{
			SetupUIBtn(pSprite ,i,bTouch); 
			iSelRemoteBtn=i;
			ControlFunction(i);
			break;
		}
	}  
	return true;
}
void UI_RobotRemote::ControlFunction(int iTag)
{
	int iCmdNum=PLAYER_DISPATCH_AI_STOP;
	 uint32_t mapIndex[1];
	 mapIndex[0]=10;

	 if(pDisProxy==NULL) //尚未連接
	 {
		 iTag=REMOTEBTN_TITAL; 
	 }

	switch(iTag)
	{
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
	case REMOTEBTN_TITAL: //返回上一頁
		HideLayer();
		ChangLayer_Main();
		break;
	}
}
void UI_RobotRemote::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );

	//m_test->MouseMove(b2Vec2(nodePosition.x,nodePosition.y));        
}

void UI_RobotRemote::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	//執行函數
	for (int i = REMOTEBTN_TITAL; i < m_BtnArray[REMOTEPG_STATE]->count(); i++)
	{  
		CCSprite* pSprite = (CCSprite*)m_BtnArray[REMOTEPG_STATE]->objectAtIndex(i); 
//		bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
		SetupUIBtn(pSprite ,i,false); //現階段採用笨方式 不斷重畫!日後改為onmousemove 事件

	} 
	//m_test->MouseUp(b2Vec2(nodePosition.x,nodePosition.y));
}

//////////////////////////////////////////////////////////////////////////
//Robot Sacnning View
