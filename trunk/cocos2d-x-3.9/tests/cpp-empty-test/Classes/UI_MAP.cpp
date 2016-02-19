#include <ImageList.h>
#include <HelloWorldScene.h>

#if PLAYERSTAGE
#include <libplayerc++/playerc++.h>
#include "player.h"
#include "MAP_Function.h"
#endif
USING_NS_CC;
using namespace std;

extern HelloWorld* m_MainView;

#if PLAYERSTAGE
using namespace PlayerCc;
extern PlayerClient *pPlayerclient;

#endif



UI_MAP::UI_MAP():
touchEnabled(false),
touchListener(nullptr),
PageType(MAPPG_LAYER),
#if PLAYERSTAGE
fZoomRate(GUIMAPSize_DEFRATE),
#endif
bMoveKeep(false),
pMapTexture(NULL),
iRelseTime(0)
{
    for(int i=0;i<MAPPG_SIZE;i++)
    {
        auto Page = new SpriteVector;
        m_SpriteVectors.push_back(Page);
        auto Page2 = new LabelVector;
        m_LabelVectors.push_back(Page2);
    }
    
}
UI_MAP::~UI_MAP()
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
    
	
	if(pMapTexture!=NULL)
	{
        pMapTexture->release();
	}
#if PLAYERSTAGE
    FreeImageData();
	GuiData_Free();
#endif
}

bool UI_MAP::init()
{  
    if (!Layer::init())
    {
        return false;
    }
    InitUI();
    return true;
}  

void UI_MAP::InitUI()
{
	winSize = Director::getInstance()->getVisibleSize();
    winOrigin = Director::getInstance()->getVisibleOrigin();
	InitUI_MAPMAIN();


}
void UI_MAP::InitUI_MAPMAIN()
{
    auto pageSprite = m_SpriteVectors.at(MAPPG_MAIN);
    auto pageLabel = m_LabelVectors.at(MAPPG_MAIN);
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
    
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	Point SpriteSite_MAPBK=Vec2(0*W_unit,winSize.height-150*H_unit);  //144 為灰色抬頭的高度
	Point IconSite_ZoomIn=Vec2(922*W_unit,winSize.height-200*H_unit);
	Point IconSite_ZoomOut=Vec2(782*W_unit,winSize.height-200*H_unit);
	Point LabSite_ZoomRate=Vec2(450*W_unit,winSize.height-200*H_unit);
	Point IconSite_ReturnZero=Vec2(1000*W_unit,winSize.height-1500*H_unit);
    Point IconSite_RobotSite=Vec2(winSize.width/2, winSize.height/2+150*H_unit);
	//底圖
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	addChild(pSpriteBK);
    pSpriteBK->setTag(MSTAET_BK);
    pageSprite->pushBack(pSpriteBK);
    m_itemIdx[MAPPG_MAIN*100+MSTAET_BK]=pageSprite->size()-1;

	//上方灰色抬頭
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteAction->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pSpriteAction->setPosition(Vec2(pSpriteAction->getBoundingBox().size.width/2,winSize.height- pSpriteAction->getBoundingBox().size.height/2));
	pSpriteBK->addChild(pSpriteAction,10);
    
	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //返回上一頁
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	auto pLabel = Label::createWithSystemFont(STR_TITAL_CoverageMap, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_MAP_HISTORY_DEF); //歷史軌跡
	pSprBtnIcon->setPosition(Vec2(pSpriteAction->getContentSize().width-pSprBtnIcon->getContentSize().width*0.7,winSize.height-pSpriteAction->getContentSize().height/2));
	pSpriteBK->addChild(pSprBtnIcon,12);
    
    pSprBtnIcon->setTag(MAPMAIN_HISTORY);
    pageSprite->pushBack(pSprBtnIcon);
    m_itemIdx[MAPPG_MAIN*100+MAPMAIN_HISTORY]=pageSprite->size()-1;
    pSpriteAction->setTag(MAPMAIN_TITAL);
    pageSprite->pushBack(pSpriteAction);
    m_itemIdx[MAPPG_MAIN*100+MAPMAIN_TITAL]=pageSprite->size()-1;
    

	//ZOOM IN OUT
	auto pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
    pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
    pSprite->setTag(MAPMAIN_ZOOMOUT);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAPPG_MAIN*100+MAPMAIN_ZOOMOUT]=pageSprite->size()-1;

    pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
    pSprite->setTag(MAPMAIN_ZOOMIN);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAPPG_MAIN*100+MAPMAIN_ZOOMIN]=pageSprite->size()-1;
    
	//縮放倍率
	pLabel = Label::createWithSystemFont("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
    pLabel->setTag(MAPMAIN_LAB_ZOOMLAB);
    pageLabel->pushBack(pLabel);
    m_itemIdx[MAPPG_MAIN*100+MAPMAIN_LAB_ZOOMLAB]=pageLabel->size()-1;
	
	//預設倍率
	pSprite=Sprite::createWithSpriteFrameName(S_MAP_RETURN_ZERO_DEF); 
	pSprite->setPosition(IconSite_ReturnZero);
	pSpriteBK->addChild(pSprite,12);
    pSprite->setTag(MAPMAIN_RETIRNZERO);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAPPG_MAIN*100+MAPMAIN_RETIRNZERO]=pageSprite->size()-1;
    
    //更新時間提示
    auto pTimeLab=Label::createWithCharMap(IMAGELIST_UPDATE_TIME_NUMBER,35,40,'0');
	pTimeLab->setPosition(Vec2(LabSite_ZoomRate.x-100,LabSite_ZoomRate.y));
	pSpriteBK->addChild(pTimeLab,12);
    pTimeLab->setTag(MAPMAIN_LAB_RESTIME);
    pageLabel->pushBack(pTimeLab);
    m_itemIdx[MAPPG_MAIN*100+MAPMAIN_LAB_RESTIME]=pageLabel->size()-1;

	//Robot 圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setPosition(IconSite_RobotSite);
	pSpriteBK->addChild(pSprite,11);
    pSprite->setTag(MAPMAIN_ROBOT);
    pageSprite->pushBack(pSprite);
    m_itemIdx[MAPPG_MAIN*100+MAPMAIN_ROBOT]=pageSprite->size()-1;

    //地圖底圖（要擺在最後）
	auto pSpriteMap=Sprite::createWithSpriteFrameName(S_CLEAN_MAP_BG);
    pSpriteMap->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT); //≥]©w¡„¬I
	pSpriteMap->setPosition(SpriteSite_MAPBK);
	pSpriteBK->addChild(pSpriteMap,9);
	pSpriteMap->setColor(COLOR_STRING_GRAY);
    pSpriteMap->setTag(MAPMAIN_MAPBK);
    pageSprite->pushBack(pSpriteMap);
    m_itemIdx[MAPPG_MAIN*100+MAPMAIN_MAPBK]=pageSprite->size()-1;
    
    pSpriteBK->setVisible(false);

}
Sprite* UI_MAP::getSpritePoint(int iPageTyep,int iPageIdx)
{
    return m_SpriteVectors.at(iPageTyep)->at(m_itemIdx[iPageTyep*100+iPageIdx]);
}
Label* UI_MAP::getLabelPoint(int iPageTyep,int iPageIdx)
{
    return m_LabelVectors.at(iPageTyep)->at(m_itemIdx[iPageTyep*100+iPageIdx]);
}

void UI_MAP::update(float dt)
{
    static int MAPUpdateTime=0;
	if(MAPUpdateTime%60==0) //更新時間提示
	{
		iRelseTime++;
        auto pTimeLab= getLabelPoint(MAPPG_MAIN,MAPMAIN_LAB_RESTIME);
        char sTemp[20];
        sprintf(sTemp,"%d",iRelseTime);
        pTimeLab->setString(sTemp);
	}
	MAPUpdateTime++;
}
//地圖更新
void UI_MAP::UpdateMAPSchedule(float dt)
{
    if(IsRobotMapReady())
    { // ©w¥¡ßÛ∑s¶aπœ
        RobotMapUpdate();
    }
    if(WaittryRobotMapUpdate()==0)
    { // µ•®Ï∑s™∫∞T∏π,ßÛ∑s¶aπœ ≠´∑s√∏ªs
        ReloadRobotMap();
        iRelseTime=0;
    }
    auto pTimeLab= getLabelPoint(MAPPG_MAIN,MAPMAIN_LAB_RESTIME);
    char sTemp[20];
    sprintf(sTemp,"%d",iRelseTime);
    pTimeLab->setString(sTemp);
}

void UI_MAP::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true);
	Show_MapMain();
	schedule(schedule_selector(UI_MAP::UpdateMAPSchedule),G_ROBOTMAP_RELEASE_TIME);     //地圖更新thread
	scheduleUpdate();     //啟動更新時間計數
}
void UI_MAP::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();
	unschedule(schedule_selector(UI_MAP::UpdateMAPSchedule));     //地圖更新thread
	unscheduleUpdate();            //關閉更新時間計數
}
void UI_MAP::Hide_AllPage()
{
	Hide_MapMain();
	PageType = MAINPG_LAYER;
}


void UI_MAP::Show_MapMain()
{
    auto pSprite=getSpritePoint(MAPPG_MAIN,MAPMAIN_BK);
	pSprite->setVisible(true);
	PageType = MAPPG_MAIN;

	RobotMapUpdate(); //±j®Ó©≥ºh¶aπœßÛ∑s
	if(!IsRobotMapReady()) //™Ï¶∏∞ı¶Ê§@©w≠nµ•´›ßÛ∑s
		WaitRobotMapUpdate();

    
#if PLAYERSTAGE
	if(IsGUIMAPReady()) //∏¸§JπœßŒ
	{
		ReloadRobotMap();	//≈„•‹¶aπœ
		MAPZoomDefault();	//¶^¥_πw≥]§j§p
		MAPRetunZero();		//¶aπœ¶^®Ï§§§ﬂ
		MAPZoomLabUpdate();  //ßÛ∑s≠ø≤v¶r¶Í
	}
#endif
}

void UI_MAP::ReloadRobotMap()
{
#if PLAYERSTAGE
    auto pSpriteMap= getSpritePoint(MAPPG_MAIN,MAPMAIN_MAPBK);
    if(pMapTexture!=NULL)
        pMapTexture->release();
    pMapTexture = new CCTexture2D();
	bool val = pMapTexture->initWithData(GetGUIMAP(),CCTexture2DPixelFormat::RGB888,GetGUIMAP_Width(),GetGUIMAP_Height(),Vec2(GetGUIMAP_Width(),GetGUIMAP_Height()));
    pSpriteMap->setTexture(pMapTexture);
    pSpriteMap->setTextureRect(Rect(0,0,GetGUIMAP_Width(),GetGUIMAP_Height()));
    pSpriteMap->setColor(COLOR_STRING_GRAY);
#endif


}
void UI_MAP::Hide_MapMain()
{
    auto pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_BK);
	pSprite->setVisible(false);
}
//////////////////////////////////////////////////////////////////////////
bool UI_MAP::onTouchBegan(Touch* touch, Event* event)
{
 
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr)
    {
        return true;
    }
    log("UI_MAP:%s",__func__);
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
	if(isVisible())
	{
		switch(PageType)
		{
		default:
			break;
		case MAPPG_MAIN:
			{
                auto sprites=m_SpriteVectors.at(PageType);
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
                            Function_UIMAP_START(iIdx);
                            if(bMoveKeep) //©Ï©‘≤æ∞  ∞Oø˝∞_©l¬I
                            {
                                StartPoint_Mouse=nodePosition;
                            }
                            break;
                        }
                    }
                }
                if(!bTouch)
                {
                    auto labels=m_LabelVectors.at(PageType);
                    
                    for(auto pLabel:*labels)
                    {
                        auto iIdx=pLabel->getTag();
                        if(pLabel->isVisible() && iIdx!=0)
                        {
                            auto rectBox= pLabel->getBoundingBox();
                            rectBox.origin+=winOrigin;
                            auto bTouch = rectBox.containsPoint(nodePosition);
                            if(bTouch)
                            {
                                Function_UIMAP_START(iIdx);
                                if(bMoveKeep) //©Ï©‘≤æ∞  ∞Oø˝∞_©l¬I
                                {
                                    StartPoint_Mouse=nodePosition;
                                }
                                break;
                            }
                        }
                    }
                    
                }
 
			}
			break;
		}

		return true;
	}
	else
		return false;
}
void UI_MAP::onTouchMoved(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr){ return;}
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
    if(bMoveKeep)
	{
		MapMove(nodePosition );
	}
}
void UI_MAP::onTouchEnded(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr){ return;}
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
	switch(PageType)
	{
	default:
		break;
	case MAPPG_MAIN:
		{
            auto sprites=m_SpriteVectors.at(MAPPG_MAIN);
            auto bTouch = false;
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
                         Function_UIMAP(iIdx);
                        break;
                    }
                }
            }
            if(!bTouch)
            {
                auto labels=m_LabelVectors.at(PageType);
                
                for(auto pLabel:*labels)
                {
                    auto iIdx=pLabel->getTag();
                    if(pLabel->isVisible() && iIdx!=0)
                    {
                        auto rectBox= pLabel->getBoundingBox();
                        rectBox.origin+=winOrigin;
                        auto bTouch = rectBox.containsPoint(nodePosition);
                        if(bTouch)
                        {
                            Function_UIMAP(iIdx);
                            break;
                        }
                    }
                }
                
            }
            
		}
		break;
	}
	bMoveKeep=false; //∞±§Ó≤æ∞ 
}
//¶aπœ≤æ∞ 
void UI_MAP::MapMove(Vec2 nodePosition )
{
    auto pSprite=getSpritePoint(MAPPG_MAIN,MAPMAIN_MAPBK);
	Point MapSite;
	MapSite.x=StartPoint_Map.x+(nodePosition.x-StartPoint_Mouse.x);
	MapSite.y=StartPoint_Map.y+(nodePosition.y-StartPoint_Mouse.y);
	pSprite->setPosition(MapSite);
    pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_ROBOT);
	pSprite ->setPosition(MapSite);
}
float UI_MAP::MAPReSetSize(float fSize)
{
#if PLAYERSTAGE
	if(fSize<=GUIMAPSize_MINRATE)
		fSize=GUIMAPSize_MINRATE;
	if(fSize>=GUIMAPSize_MAXRATE)
		fSize=GUIMAPSize_MAXRATE;
#endif
    auto pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_MAPBK);
    auto stCenter=pSprite->getPosition();
	float fOldZoomRate=pSprite->getScale();
	if(fSize==fOldZoomRate) 
		return fSize;
	pSprite->setScale(fSize);
	pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_ROBOT);
	pSprite->setScale(fSize*0.6);

	return fSize;

}
//ßÛ∑s≠ø≤v¶r¶Í
void UI_MAP::MAPZoomLabUpdate()
{
    auto pZoomLab=getLabelPoint(MAPPG_MAIN,MAPMAIN_LAB_ZOOMLAB);
	char sTemp[12];
	sprintf(sTemp,"X %.1f",fZoomRate);
	pZoomLab->setString(sTemp);
}
void UI_MAP::MAPZoomIn()
{
#if PLAYERSTAGE
	if(fZoomRate>=1.0)
		fZoomRate+=GUIMAPSize_UP_SPACE;
	else
		fZoomRate+=GUIMAPSize_DOWN_SPACE;
	fZoomRate=MAPReSetSize(fZoomRate);
#else
    fZoomRate+=1;
#endif
    auto pSpriteROBOT= getSpritePoint(MAPPG_MAIN,MAPMAIN_ROBOT);
    auto pSpriteMAP= getSpritePoint(MAPPG_MAIN,(MAPMAIN_MAPBK));
	pSpriteROBOT->setPosition(pSpriteMAP->getPosition());
	MAPZoomLabUpdate();
}
void UI_MAP::MAPZoomOut()
{
#if PLAYERSTAGE
	if(fZoomRate>1.0)
		fZoomRate-=GUIMAPSize_UP_SPACE;
	else
		fZoomRate-=GUIMAPSize_DOWN_SPACE;
	fZoomRate=MAPReSetSize(fZoomRate);
#else
    fZoomRate-=1;
#endif
    auto pSpriteROBOT= getSpritePoint(MAPPG_MAIN,MAPMAIN_ROBOT);
    auto pSpriteMAP= getSpritePoint(MAPPG_MAIN,MAPMAIN_MAPBK);
    pSpriteROBOT->setPosition(pSpriteMAP->getPosition());
	MAPZoomLabUpdate();
}


//¶^¬k≠Ï¬I
void UI_MAP::MAPRetunZero()
{
#if PLAYERSTAGE
	pose_t robot_pose={0.0,0.0,0.0};
	pose_t robot_mappose;
	robot_mappose = getRobotSiteByPose2d(robot_pose);
    auto pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_ROBOT);
    pSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	float fWidthRate=winSize.width/GetGUIMAP_Width();
	float fHeightRate=winSize.height/GetGUIMAP_Height();
    pSprite ->setPosition(Vec2(robot_mappose.px*fWidthRate,robot_mappose.py*fHeightRate));
    //≤æ∞ ©≥πœ®Ï§§•°
    pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_MAPBK);
    pSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pSprite ->setPosition(Vec2(robot_mappose.px*fWidthRate,robot_mappose.py*fHeightRate));

#else
    auto pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_ROBOT);
    pSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pSprite ->setPosition(Vec2(winSize.width/2,winSize.height/2));
    pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_MAPBK);
    pSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pSprite ->setPosition(Vec2(winSize.width/2,winSize.height/2));
#endif
}
void UI_MAP::MAPZoomDefault()
{
#if PLAYERSTAGE
    auto mapX=GetGUIMAP_Width()*0.5;
    auto mapY=GetGUIMAP_Height()*0.5;
    fZoomRate=MAPReSetSize(GUIMAPSize_DEFRATE);
#else
    auto mapX=2048*0.5;
    auto mapY=2048*0.5;
    fZoomRate=MAPReSetSize(1.0f);
#endif
	auto  pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_MAPBK);
	pSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pSprite->setPosition(Vec2(mapX,mapY));
	
	
	//ß‰¥MROBOT ¶Ï∏m
	pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_ROBOT); //ROBOT
	pSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pSprite->setPosition(Vec2(mapX,mapY));
	pSprite->setScale(fZoomRate*0.6);

}
///
//注意此處切換頁面 容易會造成系統異常
//因為 ontouch 事件還沒有跑完
void UI_MAP::Function_UIMAP(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case MAPMAIN_ZOOMIN:
		MAPZoomIn();
		break;
	case MAPMAIN_ZOOMOUT:
		MAPZoomOut();
		break;
	case MAPMAIN_RETIRNZERO:
		MAPRetunZero();
		break;
	case MAPMAIN_TITAL:
		MAPZoomDefault();
        ChangLayer_Main();
		break;
	}
}

//¬IøÔ´·∂}©lßP¬_¨Oß_•i•H≤æ∞ 
void UI_MAP::Function_UIMAP_START(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		bMoveKeep=false;
		break;
	case MAPMAIN_ROBOT:
	case MAPMAIN_MAPBK:
		{
            auto pSprite= getSpritePoint(MAPPG_MAIN,MAPMAIN_MAPBK);
			StartPoint_Map=pSprite->getPosition();
		}
		bMoveKeep=true;
		
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
/////////////////////////////////
//觸碰設定
void UI_MAP::setTouchEnabled(bool enable)
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
        
        touchListener->onTouchBegan = CC_CALLBACK_2(UI_MAP::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(UI_MAP::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(UI_MAP::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
    }
    else
    {
        _eventDispatcher->removeEventListener(touchListener);
        CC_SAFE_RELEASE_NULL(touchListener);
    }
}
bool UI_MAP::isTouchEnabled() const
{
    return touchEnabled;
}