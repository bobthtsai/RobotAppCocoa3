//其他方式從陣列建立圖檔
//Texture2D *Texture = new Texture2D();
//
//static Color4B Data[1024*1024];
//
//for(int i = 0; i < 1024*1024; i++) 
//	Data[i] = black;
//Texture->initWithData(Data, sizeof(Color4B)*1024*1024, kTexture2DPixelFormat_RGBA8888, 1024, 1024, CCSize(1024, 1024));
//
//CCRenderTexture 讀取IMAGE的方式
//CCRenderTexture* r = CCRenderTexture::renderTextureWithWidthAndHeight(fragment->getTexture()->getPixelsWide(), fragment->getTexture()->getPixelsHigh());
//
//r->beginWithClear(1, 1, 1, 0);
//fragment->visit();
//r->end();
//
//CCImage *testImage = r->newCCImage();
//unsigned char *data = testImage->getData();
#include <ImageList.h>
#include <HelloWorldScene.h>

#if PLAYERSTAGE
#include <libplayerc++/playerc++.h>
#include <player.h>
#endif

#if !NO_MAP
#include <MAP_Function.h>
#endif

#if DBG
#include "dbg.h"
#endif

//#define NO_ELLIPSE		//無橢圓
//#define NO_EDITZOOMINOUT	 // 編輯放棄縮放
USING_NS_CC;
using namespace std;
extern HelloWorld* m_MainView;

#if PLAYERSTAGE
using namespace PlayerCc;
extern DispatcherProxy *pDisProxy;
/// Player Client
extern PlayerClient *pPlayerclient;
#endif

const int G_RandMaxSize=4096;
const int G_PenEdgeSize=4;	//畫筆大小
const int G_MAP_ZLEVEL =8; //地圖的高度
#if !NO_MAP
MapRangInfo stRangInfo[G_RandMaxSize];
#endif
const Color4F RangEdgeColor_Black =Color4F(Color3B(255,0,0)); //外框黑線
const Color4F RangColor_FullRed =Color4F(0.9,0.9,0.9,0.1); //內層顏色
const Color4F RangColor_SelRed =Color4F(1,0,0,0.6f); //內層被選中時顏色
const Color4F RangColor_EdgeRed =Color4F(0,0,0,0.6f); //內層被選中時顏色
 
UI_CLEANJOB::UI_CLEANJOB():
touchEnabled(false),
touchListener(nullptr),
PageType(MAPPG_LAYER)
#if !NO_MAP
,fZoomRate(GUIMAPSize_DEFRATE)
#endif
{
    for(int i=0;i<CLEJOBPG_SIZE;i++)
    {
        auto Page = new SpriteVector;
        m_SpriteVectors.push_back(Page);
        auto Page2 = new LabelVector;
        m_LabelVectors.push_back(Page2);
    }
	
}
UI_CLEANJOB::~UI_CLEANJOB()
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
void UI_CLEANJOB::InitPageArge()
{
	bMoveKeep=false;
	InitMapRangTag();
	InitRangInfo(); //清空記錄
}

//清空現在畫面上所有元件
void UI_CLEANJOB::CleanMAPRang(int iPagIdx)
{
    auto pSpriteMap= getSpritePoint(PageType, MAPBK_IDX[iPagIdx]);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPagIdx]);
	pSpriteMap->removeAllChildren();
}
bool UI_CLEANJOB::init()
{  
	if (!Layer::init()) 
	{  
		return false;  
	}  
	InitUI();  
	return true;  
}  

void UI_CLEANJOB::InitUI()
{
    winSize = Director::getInstance()->getVisibleSize();
    winOrigin = Director::getInstance()->getVisibleOrigin();
    InitUI_CLEANJOBMAIN();
	InitUI_CORNER();
	InitUI_CORNERHELP();
	InitUI_MULTI();
	InitUI_MULTIHELP();
	InitUI_MOVETO();
	InitUI_MOVETOHELP();
	InitUI_AUTO();
	InitUI_AUTOHELP();
	InitUI_SPIRAL();
	InitUI_SPIRALHELP();
	InitUI_WALL();
	InitUI_WALLHELP();
}
void UI_CLEANJOB::InitUI_CLEANJOBMAIN()
{
//	m_BtnArray[CLEJOBPG_MAIN] = CCArray::createWithCapacity(CLEJOBMAIN_SIZE); //按鈕存放
//	m_BtnArray[CLEJOBPG_MAIN]->retain();
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
    float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	addChild(pSpriteBK); //加入 layer 層
    saveSprite(CLEJOBPG_MAIN, CLEJOBMAIN_BK, pSpriteBK);
//	m_BtnArray[CLEJOBPG_MAIN]->addObject(pSpriteBK);

	Point BtnSite_Corner=Vec2(8*W_unit,winSize.height-244*H_unit);
	Point BtnSite_Multi=Vec2(8*W_unit,winSize.height-517*H_unit);
	Point BtnSite_Move=Vec2(8*W_unit,winSize.height-790*H_unit);
	Point BtnSite_Auto=Vec2(8*W_unit,winSize.height-1063*H_unit);
	Point BtnSite_Spiral=Vec2(8*W_unit,winSize.height-1336*H_unit);
	Point BtnSite_Wall=Vec2(8*W_unit,winSize.height-1609*H_unit);
	Point BtnSiteIn_Icon=Vec2(48*W_unit,(248-5)*H_unit);
	Point BtnSiteIn_Lab=Vec2(320*W_unit,(248-73)*H_unit);

	Point SpriteSite_Corner=Vec2(137*W_unit,winSize.height-267*H_unit);	
	Point LabelSite_Corner=Vec2(368*W_unit,winSize.height-316*H_unit);
	Point SpriteSite_Multi=Vec2(137*W_unit,winSize.height-537*H_unit);	
	Point LabelSite_Multi=Vec2(368*W_unit,winSize.height-587*H_unit);
	Point SpriteSite_Move=Vec2(137*W_unit,winSize.height-810*H_unit);	
	Point LabelSite_Move=Vec2(368*W_unit,winSize.height-860*H_unit);
	Point SpriteSite_Auto=Vec2(137*W_unit,winSize.height-1083*H_unit);	
	Point LabelSite_Auto=Vec2(368*W_unit,winSize.height-1133*H_unit);
	Point SpriteSite_Spiral=Vec2(137*W_unit,winSize.height-1354*H_unit);	
	Point LabelSite_Spiral=Vec2(368*W_unit,winSize.height-1404*H_unit);
	Point SpriteSite_Wall=Vec2(137*W_unit,winSize.height-1628*H_unit);	
	Point LabelSite_Wall=Vec2(368*W_unit,winSize.height-1678*H_unit);

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	auto pLabel = Label::createWithSystemFont(STR_TITAL_CleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(CLEJOBPG_MAIN, CLEJOBMAIN_TITAL, pSpriteAction);
//	m_BtnArray[CLEJOBPG_MAIN]->insertObject(pSpriteAction,CLEJOBMAIN_TITAL);

	//6個工作JOB 含背景圖
	int ObjIdx_bg[6]={CLEJOBMAIN_CORNER,CLEJOBMAIN_MULTI,CLEJOBMAIN_MOVE,CLEJOBMAIN_AUTO,CLEJOBMAIN_SPIRAL,CLEJOBMAIN_WALL,};
	int ObjIdx_lab[6]={CLEJOBMAIN_LAB_CORNER,CLEJOBMAIN_LAB_MULTI,CLEJOBMAIN_LAB_MOVE,CLEJOBMAIN_LAB_AUTO,CLEJOBMAIN_LAB_SPIRAL,CLEJOBMAIN_LAB_WALL,};
	Point pPointp[6]={BtnSite_Corner,BtnSite_Multi,BtnSite_Move,BtnSite_Auto,BtnSite_Spiral,BtnSite_Wall};
	const char *pIconName[6];
	pIconName[0]=S_CLEAN_BTN_CORNER;
	pIconName[1]=S_CLEAN_BTN_MULTI;
	pIconName[2]=S_CLEAN_BTN_MOVE;
	pIconName[3]=S_CLEAN_BTN_AUTO;
	pIconName[4]=S_CLEAN_BTN_SPIRAL;
	pIconName[5]=S_CLEAN_BTN_WALL;

	const char *pLabName[6];
	pLabName[0]=STR_BTN_Cornercleaningjob;
	pLabName[1]=STR_BTN_Multiroomsjob;
	pLabName[2]=STR_BTN_Movetotargetjob;
	pLabName[3]=STR_BTN_Automode;
	pLabName[4]=STR_BTN_Spiral;
	pLabName[5]=STR_BTN_Wallfollowing;

	for(int i=0;i<6;i++) //建立六組BTN
	{
		auto pBtnbk=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP); //背景 
        pBtnbk->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pBtnbk->setPosition(pPointp[i]);
		pBtnbk->setColor(COLOR_STRING_WHITE);
		pSpriteBK->addChild(pBtnbk,10);
        saveSprite(CLEJOBPG_MAIN, ObjIdx_bg[i], pBtnbk);
//        m_BtnArray[CLEJOBPG_MAIN]->insertObject(pBtnbk,ObjIdx_bg[i]);
		auto pSprite=Sprite::createWithSpriteFrameName(pIconName[i]);  //圖示
		pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pSprite->setPosition(BtnSiteIn_Icon);
		pBtnbk->addChild(pSprite,10);	
        pLabel = Label::createWithSystemFont(pLabName[i], S_FONE, *pSTR_CLEANJOB_SIZE,Size::Size(600*W_unit,144*H_unit),TextHAlignment::LEFT,TextVAlignment::TOP); //文字
		pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pLabel->setColor(COLOR_STRING_BLACK);
		pLabel->setPosition(BtnSiteIn_Lab);
		pBtnbk->addChild(pLabel,10);
        saveLabel(CLEJOBPG_MAIN, ObjIdx_lab[i], pLabel);
//		m_BtnArray[CLEJOBPG_MAIN]->insertObject(pLabel,ObjIdx_lab[i]);
	}
    pSpriteBK->setVisible(false);

}
void UI_CLEANJOB::InitUI_CORNER()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point IconSite_Help=Vec2(955*W_unit,winSize.height-(99-75)*H_unit);		
	Point IconSite_ZoomOut=Vec2(782*W_unit,winSize.height-238*H_unit);		
	Point IconSite_ZoomIn=Vec2(922*W_unit,winSize.height-238*H_unit);		
	Point LabSite_ZoomRate=Vec2(550*W_unit,winSize.height-238*H_unit);		
	Point LabSite_Clean=Vec2(90*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Virual=Vec2(425*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Undo=Vec2(705*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Delete=Vec2(860*W_unit,winSize.height-1519*H_unit);
	Point IconSite_OVAL=Vec2(90*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_RECL=Vec2(265*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_WALL=Vec2(485*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_UNDO=Vec2(705*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_DELETE=Vec2(860*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_MAPBG=Vec2(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	addChild(pSpriteBK); //加入 layer 層
	pSpriteBK->setColor(COLOR_STRING_BLACK);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_BK, pSpriteBK);
//	m_BtnArray[CLEJOBPG_CORNER]->addObject(pSpriteBK);

	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//pSprBtnIcon->setPosition(Vec2(pSpriteAction->getContentSize().width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_HELP, pSprBtnIcon);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprBtnIcon,CLECORNER_HELP);

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_TITAL, pSpriteAction);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSpriteAction,CLECORNER_TITAL);

	
	auto pLabel = Label::createWithSystemFont(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	
	//ZOOM IN OUT
	auto pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_ZOOMOUT, pSprite);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_ZOOMOUT);
#ifdef NO_EDITZOOMINOUT
	pSprite->setVisible(false);
#endif
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_ZOOMIN, pSprite);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_ZOOMIN);
#ifdef NO_EDITZOOMINOUT
	pSprite->setVisible(false);
#endif
	//縮放倍率
	pLabel = Label::createWithSystemFont("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
    saveLabel(CLEJOBPG_CORNER, CLECORNER_ZOOMLAB, pLabel);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pLabel,CLECORNER_ZOOMLAB);

	//下方標籤
	pLabel = Label::createWithSystemFont(STR_Clean_CleanStamps, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11); 

	//下方按鈕
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_OVAL_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_OVAL);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_OVAL, pSprite);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_OVAL);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_RECT_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_RECL);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_RECT, pSprite);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_RECT);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_WALL, pSprite);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_WALL);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_UNDO, pSprite);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_UNDO);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_DELETE, pSprite);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_DELETE);
	//底部按鈕
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_GO, pSpriteDownBtn);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSpriteDownBtn,CLECORNER_GO);

	pLabel = Label::createWithSystemFont(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel);

	//機器人圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_ROBOT, pSprite);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_ROBOT);

	//地圖
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
    saveSprite(CLEJOBPG_CORNER, CLECORNER_MAP, pSprite);
//	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_MAP);
    pSpriteBK->setVisible(false);

}
void UI_CLEANJOB::InitUI_CORNERHELP()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	Point IconSite_BG=Vec2(65*W_unit,winSize.height-156*H_unit);		
	Point IconSiteIN[9]=
	{Vec2((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,Vec2((140-65)*W_unit,(1710+156-539)*H_unit)
	,Vec2((587-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2((464-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2(( 90-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((265-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((485-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((705-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((860-65)*W_unit,(1710+156-1490)*H_unit)};

	Point LabSiteIN[9]={
		Vec2(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		Vec2((487-65)*W_unit,(1710+156-615)*H_unit),
		Vec2((487-65)*W_unit,(1710+156-665)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1084)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1134)*H_unit),
		Vec2((90-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((425-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((705-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((860-65)*W_unit,(1710+156-1422)*H_unit)		
	};
		
	const char* pIconName[9];
	pIconName[0]=S_CLEAN_BTN_CORNER;
	pIconName[1]=S_CLEAN_BTN_HELP_STAMP_CORNER;
	pIconName[2]=S_CLEAN_BTN_HELP_PEN;
	pIconName[3]=S_CLEAN_BTN_HELP_ARROW;
	pIconName[4]=S_CLEAN_BTN_EDIT_OVAL_DEF;
	pIconName[5]=S_CLEAN_BTN_EDIT_RECT_DEF;
	pIconName[6]=S_CLEAN_BTN_EDIT_WALL_DEF;
	pIconName[7]=S_CLEAN_BTN_EDIT_UNDO_DEF;
	pIconName[8]=S_CLEAN_BTN_EDIT_DELETE_DEF;
	const char* pLabName[9];
	pLabName[0]=STR_Clean_Cornercleaningjob;
	pLabName[1]=STR_Clean_Cleaningstickers;
	pLabName[2]=STR_Clean_cleaningarea;
	pLabName[3]=STR_Clean_VirtualWall;
	pLabName[4]=STR_Clean_NoEntry;
	pLabName[5]=STR_Clean_CleanStamps;
	pLabName[6]=STR_Clean_VirtualWall;
	pLabName[7]=STR_Clean_Undo;
	pLabName[8]=STR_Clean_Delete;
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	
    saveSprite(CLEJOBPG_CORNERHELP, 0, pSpriteBK);

//	m_BtnArray[CLEJOBPG_CORNERHELP]->addObject(pSpriteBK);	
	for(int i=0;i<9;i++) //按鈕
	{
		auto pSprite=Sprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
    auto pLabel = Label::createWithSystemFont(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,Size::Size(pSpriteBK->getContentSize().width,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<9;i++) //字串
	{
		auto pLabel = Label::createWithSystemFont(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT,TextVAlignment::TOP);
		pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = Label::createWithSystemFont(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
    pSpriteBK->setVisible(false);
}

void UI_CLEANJOB::InitUI_MULTI()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point IconSite_Help=Vec2(955*W_unit,winSize.height-(99-75)*H_unit);		
	Point IconSite_ZoomOut=Vec2(782*W_unit,winSize.height-238*H_unit);		
	Point IconSite_ZoomIn=Vec2(922*W_unit,winSize.height-238*H_unit);	
	Point LabSite_ZoomRate=Vec2(550*W_unit,winSize.height-238*H_unit);		
	Point LabSite_Clean=Vec2(90*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Virual=Vec2(425*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Undo=Vec2(705*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Delete=Vec2(860*W_unit,winSize.height-1519*H_unit);
	Point IconSite_OVAL=Vec2(90*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_RECL=Vec2(265*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_WALL=Vec2(485*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_UNDO=Vec2(705*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_DELETE=Vec2(860*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_MAPBG=Vec2(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
//	pSpriteBK->setAnchorPoint(Vec2::ZERO); //設定錨點
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	addChild(pSpriteBK); //加入 layer 層
	pSpriteBK->setColor(COLOR_STRING_BLACK);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_BK, pSpriteBK);
//	m_BtnArray[CLEJOBPG_MULTI]->addObject(pSpriteBK);

	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//pSprBtnIcon->setPosition(Vec2(pSpriteAction->getContentSize().width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_HELP, pSprBtnIcon);

//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprBtnIcon,CLEMULTI_HELP);

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_TITAL, pSpriteAction);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSpriteAction,CLEMULTI_TITAL);


	auto pLabel = Label::createWithSystemFont(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	//ZOOM IN OUT
	auto pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_ZOOMOUT, pSprite);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_ZOOMOUT);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_ZOOMIN, pSprite);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_ZOOMIN);

	//縮放倍率
	pLabel = Label::createWithSystemFont("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
    saveLabel(CLEJOBPG_MULTI, CLEMULTI_ZOOMLAB, pLabel);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pLabel,CLEMULTI_ZOOMLAB);

	//下方標籤
	pLabel = Label::createWithSystemFont(STR_Clean_CleanStamps, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11);
	//下方按鈕
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_MUTIOVAL_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_OVAL);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_OVAL, pSprite);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_OVAL);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_MUTIRECT_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_RECL);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_RECT, pSprite);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_RECT);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_BK, pSpriteBK);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_WALL);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_UNDO, pSprite);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_UNDO);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_DELETE, pSprite);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_DELETE);
	//底部按鈕
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_GO, pSpriteDownBtn);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSpriteDownBtn,CLEMULTI_GO);
	pLabel = Label::createWithSystemFont(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 11);

	//機器人圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_ROBOT, pSprite);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_ROBOT);
	//地圖
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
    saveSprite(CLEJOBPG_MULTI, CLEMULTI_MAP, pSprite);
//	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_MAP);
    pSpriteBK->setVisible(false);

}
void UI_CLEANJOB::InitUI_MULTIHELP()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	Point IconSite_BG=Vec2(65*W_unit,winSize.height-156*H_unit);		
	Point IconSiteIN[9]=
	{Vec2((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,Vec2((140-65)*W_unit,(1710+156-539)*H_unit)
	,Vec2((587-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2((464-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2(( 90-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((265-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((485-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((705-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((860-65)*W_unit,(1710+156-1490)*H_unit)};

	Point LabSiteIN[9]={
		Vec2(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		Vec2((487-65)*W_unit,(1710+156-615)*H_unit),
		Vec2((487-65)*W_unit,(1710+156-665)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1084)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1134)*H_unit),
		Vec2((90-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((425-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((705-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((860-65)*W_unit,(1710+156-1422)*H_unit)		
	};

	const char* pIconName[9];
	pIconName[0]=S_CLEAN_BTN_MULTI;
	pIconName[1]=S_CLEAN_BTN_HELP_STAMP_MUTLI;
	pIconName[2]=S_CLEAN_BTN_HELP_PEN;
	pIconName[3]=S_CLEAN_BTN_HELP_ARROW;
	pIconName[4]=S_CLEAN_BTN_EDIT_MUTIOVAL_DEF;
	pIconName[5]=S_CLEAN_BTN_EDIT_MUTIRECT_DEF;
	pIconName[6]=S_CLEAN_BTN_EDIT_WALL_DEF;
	pIconName[7]=S_CLEAN_BTN_EDIT_UNDO_DEF;
	pIconName[8]=S_CLEAN_BTN_EDIT_DELETE_DEF;
	const char* pLabName[9];
	pLabName[0]=STR_BTN_Multiroomsjob;
	pLabName[1]=STR_Clean_Cleaningstickers;
	pLabName[2]=STR_Clean_cleaningarea;
	pLabName[3]=STR_Clean_VirtualWall;
	pLabName[4]=STR_Clean_NoEntry;
	pLabName[5]=STR_Clean_CleanStamps;
	pLabName[6]=STR_Clean_VirtualWall;
	pLabName[7]=STR_Clean_Undo;
	pLabName[8]=STR_Clean_Delete;
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
//	pSpriteBK->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	
    saveSprite(CLEJOBPG_MULTIHELP, 0, pSpriteBK);

//	m_BtnArray[CLEJOBPG_MULTIHELP]->addObject(pSpriteBK);	
	for(int i=0;i<9;i++) //按鈕
	{
		auto pSprite=Sprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	auto pLabel = Label::createWithSystemFont(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,Size::Size(pSpriteBK->getContentSize().width,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<9;i++) //字串
	{
		auto pLabel = Label::createWithSystemFont(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT,TextVAlignment::TOP);
		pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = Label::createWithSystemFont(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
    pSpriteBK->setVisible(false);

}
void UI_CLEANJOB::InitUI_MOVETO()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point IconSite_Help=Vec2(955*W_unit,winSize.height-(99-75)*H_unit);		
	Point IconSite_ZoomOut=Vec2(782*W_unit,winSize.height-238*H_unit);		
	Point IconSite_ZoomIn=Vec2(922*W_unit,winSize.height-238*H_unit);		
	Point LabSite_ZoomRate=Vec2(550*W_unit,winSize.height-238*H_unit);
	Point LabSite_Clean=Vec2(90*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Virual=Vec2(425*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Undo=Vec2(705*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Delete=Vec2(860*W_unit,winSize.height-1519*H_unit);
	Point IconSite_WALL=Vec2(485*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_UNDO=Vec2(705*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_DELETE=Vec2(860*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_TARGET=Vec2(winSize.width*0.5,winSize.height*0.6);		
	Point IconSite_MAPBG=Vec2(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	addChild(pSpriteBK); //加入 layer 層
	pSpriteBK->setColor(COLOR_STRING_BLACK);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_BK, pSpriteBK);

//	m_BtnArray[CLEJOBPG_MOVETO]->addObject(pSpriteBK);

	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_HELP, pSprBtnIcon);

//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprBtnIcon,CLEMOVETO_HELP);

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_TITAL, pSpriteAction);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSpriteAction,CLEMOVETO_TITAL);


	auto pLabel = Label::createWithSystemFont(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	//ZOOM IN OUT
	auto pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_ZOOMOUT, pSprite);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_ZOOMOUT);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_ZOOMIN, pSprite);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_ZOOMIN);
	//縮放倍率
	pLabel = Label::createWithSystemFont("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
    saveLabel(CLEJOBPG_MOVETO, CLEMOVETO_ZOOMLAB, pLabel);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pLabel,CLEMOVETO_ZOOMLAB);
	//下方標籤
	pLabel = Label::createWithSystemFont(STR_BTN_Movetotargetjob, S_FONE, *pSTR_TITAL_SIZE,Size::Size(winSize.width*0.4,0),TextHAlignment::LEFT,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);

	pLabel = Label::createWithSystemFont(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11);
	//下方按鈕
	//pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_OVAL_DEF); 
	//pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//pSprite->setPosition(IconSite_OVAL);
	//pSpriteBK->addChild(pSprite,12);
	//m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_OVAL);
	//pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_RECT_DEF); 
	//pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//pSprite->setPosition(IconSite_RECL);
	//pSpriteBK->addChild(pSprite,12);
	//m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_RECT);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_WALL, pSprite);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_WALL);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_UNDO, pSprite);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_UNDO);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_DELETE, pSprite);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_DELETE);
	//底部按鈕
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_GO, pSpriteDownBtn);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSpriteDownBtn,CLEMOVETO_GO);
	pLabel = Label::createWithSystemFont(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 11);
	//目的地(圖釘)
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_MOVETO_TAGGET); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_TARGET);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_TARGET, pSprite);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_TARGET);

	
	//機器人圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_ROBOT, pSprite);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_ROBOT);
	//地圖

	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
    saveSprite(CLEJOBPG_MOVETO, CLEMOVETO_MAP, pSprite);
//	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_MAP);
    pSpriteBK->setVisible(false);

}
void UI_CLEANJOB::InitUI_MOVETOHELP()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	Point IconSite_BG=Vec2(65*W_unit,winSize.height-156*H_unit);		
	Point IconSiteIN[7]=
	{Vec2((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,Vec2((140-65)*W_unit,(1710+156-539)*H_unit)
	,Vec2((587-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2((464-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2((485-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((705-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((860-65)*W_unit,(1710+156-1490)*H_unit)};

	Point LabSiteIN[9]={
		Vec2(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		Vec2((487-65)*W_unit,(1710+156-615)*H_unit),
		Vec2((487-65)*W_unit,(1710+156-665)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1084)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1134)*H_unit),
		Vec2((425-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((705-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((860-65)*W_unit,(1710+156-1422)*H_unit)		
	};

	const char* pIconName[7];
	pIconName[0]=S_CLEAN_BTN_MOVE;
	pIconName[1]=S_CLEAN_BTN_HELP_TARGET1;
	pIconName[2]=S_CLEAN_BTN_HELP_PEN;
	pIconName[3]=S_CLEAN_BTN_HELP_ARROW;
	pIconName[4]=S_CLEAN_BTN_EDIT_WALL_DEF;
	pIconName[5]=S_CLEAN_BTN_EDIT_UNDO_DEF;
	pIconName[6]=S_CLEAN_BTN_EDIT_DELETE_DEF;
	const char* pLabName[8];
	pLabName[0]=STR_Clean_Cornercleaningjob;
	pLabName[1]=STR_HELP_Target;
	pLabName[2]=STR_HELP_Setatarget;
	pLabName[3]=STR_Clean_VirtualWall;
	pLabName[4]=STR_Clean_NoEntry;
	pLabName[5]=STR_Clean_VirtualWall;
	pLabName[6]=STR_Clean_Undo;
	pLabName[7]=STR_Clean_Delete;
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
//	pSpriteBK->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	
    saveSprite(CLEJOBPG_MOVETOHELP, 0, pSpriteBK);

//	m_BtnArray[CLEJOBPG_MOVETOHELP]->addObject(pSpriteBK);	
	for(int i=0;i<7;i++) //按鈕
	{
		auto pSprite=Sprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	auto pLabel = Label::createWithSystemFont(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,Size::Size(pSpriteBK->getContentSize().width,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<8;i++) //字串
	{
		auto pLabel = Label::createWithSystemFont(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(pSpriteBK->getContentSize().width*0.5,0),TextHAlignment::LEFT,TextVAlignment::TOP);
		pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = Label::createWithSystemFont(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
    pSpriteBK->setVisible(false);

}
void UI_CLEANJOB::InitUI_AUTO()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point IconSite_Help=Vec2(955*W_unit,winSize.height-(99-75)*H_unit);		
	Point IconSite_ZoomOut=Vec2(782*W_unit,winSize.height-238*H_unit);		
	Point IconSite_ZoomIn=Vec2(922*W_unit,winSize.height-238*H_unit);		
	Point LabSite_ZoomRate=Vec2(550*W_unit,winSize.height-238*H_unit);
	Point LabSite_Clean=Vec2(90*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Virual=Vec2(425*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Undo=Vec2(705*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Delete=Vec2(860*W_unit,winSize.height-1519*H_unit);
	Point IconSite_WALL=Vec2(485*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_UNDO=Vec2(705*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_DELETE=Vec2(860*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_MAPBG=Vec2(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	addChild(pSpriteBK); //加入 layer 層
	pSpriteBK->setColor(COLOR_STRING_BLACK);
//	m_BtnArray[CLEJOBPG_AUTO]->addObject(pSpriteBK);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_BK, pSpriteBK);

	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_HELP, pSprBtnIcon);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprBtnIcon,CLEAUTO_HELP);

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_TITAL, pSpriteAction);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSpriteAction,CLEAUTO_TITAL);


	auto pLabel = Label::createWithSystemFont(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	//ZOOM IN OUT
	auto pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_ZOOMOUT, pSprite);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_ZOOMOUT);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_ZOOMIN, pSprite);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_ZOOMIN);
	//縮放倍率
	pLabel = Label::createWithSystemFont("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
    saveLabel(CLEJOBPG_AUTO, CLEAUTO_ZOOMLAB, pLabel);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pLabel,CLEAUTO_ZOOMLAB);
	//下方標籤
	pLabel = Label::createWithSystemFont(STR_BTN_Automode, S_FONE, *pSTR_TITAL_SIZE,Size::Size(winSize.width*0.3,0),TextHAlignment::LEFT,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);

	pLabel = Label::createWithSystemFont(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11);
	//下方按鈕
	//pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_OVAL_DEF); 
	//pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//pSprite->setPosition(IconSite_OVAL);
	//pSpriteBK->addChild(pSprite,12);
	//m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprBtnIcon,CLEAUTO_OVAL);
	//pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_RECT_DEF); 
	//pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//pSprite->setPosition(IconSite_RECL);
	//pSpriteBK->addChild(pSprite,12);
	//m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprBtnIcon,CLEAUTO_RECT);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_WALL, pSprite);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_WALL);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_UNDO, pSprite);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_UNDO);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_DELETE, pSprite);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_DELETE);
	//底部按鈕
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_GO, pSpriteDownBtn);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSpriteDownBtn,CLEAUTO_GO);
	pLabel = Label::createWithSystemFont(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 11);
	//機器人圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_ROBOT, pSprite);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_ROBOT);
	//地圖
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
    saveSprite(CLEJOBPG_AUTO, CLEAUTO_MAP, pSprite);
//	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_MAP);
    pSpriteBK->setVisible(false);

}
void UI_CLEANJOB::InitUI_AUTOHELP()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	Point IconSite_BG=Vec2(65*W_unit,winSize.height-156*H_unit);		
	Point IconSiteIN[6]=
	{Vec2((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,Vec2((587-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2((464-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2((485-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((705-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((860-65)*W_unit,(1710+156-1490)*H_unit)};

	Point LabSiteIN[9]={
		Vec2(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		Vec2((155-65)*W_unit,(1710+156-564)*H_unit),
		Vec2((155-65)*W_unit,(1710+156-700)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1084)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1134)*H_unit),
		Vec2((425-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((705-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((860-65)*W_unit,(1710+156-1422)*H_unit)		
	};

	const char* pIconName[6];
	pIconName[0]=S_CLEAN_BTN_AUTO;
	pIconName[1]=S_CLEAN_BTN_HELP_PEN;
	pIconName[2]=S_CLEAN_BTN_HELP_ARROW;
	pIconName[3]=S_CLEAN_BTN_EDIT_WALL_DEF;
	pIconName[4]=S_CLEAN_BTN_EDIT_UNDO_DEF;
	pIconName[5]=S_CLEAN_BTN_EDIT_DELETE_DEF;
	const char* pLabName[8];
	pLabName[0]=STR_BTN_Automode;
	pLabName[1]=STR_HELP_Automatic;
	pLabName[2]=STR_HELP_Automatic2;
	pLabName[3]=STR_Clean_VirtualWall;
	pLabName[4]=STR_Clean_NoEntry;
	pLabName[5]=STR_Clean_VirtualWall;
	pLabName[6]=STR_Clean_Undo;
	pLabName[7]=STR_Clean_Delete;
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
//	pSpriteBK->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	
    saveSprite(CLEJOBPG_AUTOHELP, 0, pSpriteBK);

//	m_BtnArray[CLEJOBPG_AUTOHELP]->addObject(pSpriteBK);	
	for(int i=0;i<6;i++) //按鈕
	{
		auto pSprite=Sprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	auto pLabel = Label::createWithSystemFont(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,Size::Size(pSpriteBK->getContentSize().width,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<8;i++) //字串
	{
		if(i<3)
			pLabel = Label::createWithSystemFont(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(pSpriteBK->getContentSize().width*0.8,0),TextHAlignment::CENTER,TextVAlignment::TOP);
		else
			pLabel = Label::createWithSystemFont(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT,TextVAlignment::TOP);
		
		pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = Label::createWithSystemFont(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
    pSpriteBK->setVisible(false);

}

void UI_CLEANJOB::InitUI_SPIRAL()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point IconSite_Help=Vec2(955*W_unit,winSize.height-(99-75)*H_unit);		
	Point IconSite_ZoomOut=Vec2(782*W_unit,winSize.height-238*H_unit);		
	Point IconSite_ZoomIn=Vec2(922*W_unit,winSize.height-238*H_unit);		
	Point LabSite_ZoomRate=Vec2(550*W_unit,winSize.height-238*H_unit);
	Point LabSite_Clean=Vec2(90*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Virual=Vec2(425*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Undo=Vec2(705*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Delete=Vec2(860*W_unit,winSize.height-1519*H_unit);
	Point IconSite_WALL=Vec2(485*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_UNDO=Vec2(705*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_DELETE=Vec2(860*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_MAPBG=Vec2(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	addChild(pSpriteBK); //加入 layer 層
	pSpriteBK->setColor(COLOR_STRING_BLACK);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_BK, pSpriteBK);
//	m_BtnArray[CLEJOBPG_SPIRAL]->addObject(pSpriteBK);

	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_HELP, pSprBtnIcon);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprBtnIcon,CLESPIRAL_HELP);

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_TITAL, pSpriteAction);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSpriteAction,CLESPIRAL_TITAL);


	auto pLabel = Label::createWithSystemFont(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	//ZOOM IN OUT
	auto pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_ZOOMOUT, pSprite);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_ZOOMOUT);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_ZOOMIN, pSprite);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_ZOOMIN);
	//縮放倍率
	pLabel = Label::createWithSystemFont("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
    saveLabel(CLEJOBPG_SPIRAL, CLESPIRAL_ZOOMLAB, pLabel);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pLabel,CLESPIRAL_ZOOMLAB);
	//下方標籤
	pLabel = Label::createWithSystemFont(STR_BTN_Spiral, S_FONE, *pSTR_TITAL_SIZE,Size::Size(winSize.width*0.4,0),TextHAlignment::LEFT,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);

	pLabel = Label::createWithSystemFont(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11);

	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_WALL, pSprite);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_WALL);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_UNDO, pSprite);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_UNDO);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_DELETE, pSprite);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_DELETE);
	//底部按鈕
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_GO, pSprite);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSpriteDownBtn,CLESPIRAL_GO);
	pLabel = Label::createWithSystemFont(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 11);
	//機器人圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_ROBOT, pSprite);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_ROBOT);
	//地圖
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
    saveSprite(CLEJOBPG_SPIRAL, CLESPIRAL_MAP, pSprite);
//	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_MAP);
    pSpriteBK->setVisible(false);


}
void UI_CLEANJOB::InitUI_SPIRALHELP()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	Point IconSite_BG=Vec2(65*W_unit,winSize.height-156*H_unit);		
	Point IconSiteIN[6]=
	{Vec2((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,Vec2((587-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2((464-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2((485-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((705-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((860-65)*W_unit,(1710+156-1490)*H_unit)};

	Point LabSiteIN[9]={
		Vec2(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		Vec2((155-65)*W_unit,(1710+156-564)*H_unit),
		Vec2((155-65)*W_unit,(1710+156-700)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1084)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1134)*H_unit),
		Vec2((425-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((705-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((860-65)*W_unit,(1710+156-1422)*H_unit)		
	};

	const char* pIconName[6];
	pIconName[0]=S_CLEAN_BTN_SPIRAL;
	pIconName[1]=S_CLEAN_BTN_HELP_PEN;
	pIconName[2]=S_CLEAN_BTN_HELP_ARROW;
	pIconName[3]=S_CLEAN_BTN_EDIT_WALL_DEF;
	pIconName[4]=S_CLEAN_BTN_EDIT_UNDO_DEF;
	pIconName[5]=S_CLEAN_BTN_EDIT_DELETE_DEF;
	const char* pLabName[8];
	pLabName[0]=STR_BTN_Spiral;
	pLabName[1]=STR_HELP_Automatic;
	pLabName[2]=STR_HELP_Automatic2;
	pLabName[3]=STR_Clean_VirtualWall;
	pLabName[4]=STR_Clean_NoEntry;
	pLabName[5]=STR_Clean_VirtualWall;
	pLabName[6]=STR_Clean_Undo;
	pLabName[7]=STR_Clean_Delete;
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
//	pSpriteBK->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	
    saveSprite(CLEJOBPG_SPIRALHELP, 0, pSpriteBK);

//	m_BtnArray[CLEJOBPG_SPIRALHELP]->addObject(pSpriteBK);	
	for(int i=0;i<6;i++) //按鈕
	{
		auto pSprite=Sprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	auto pLabel = Label::createWithSystemFont(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,Size::Size(pSpriteBK->getContentSize().width,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<8;i++) //字串
	{
		if(i<3)
			pLabel = Label::createWithSystemFont(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(pSpriteBK->getContentSize().width*0.8,0),TextHAlignment::CENTER,TextVAlignment::TOP);
		else
			pLabel = Label::createWithSystemFont(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(pSpriteBK->getContentSize().width*0.5,0),TextHAlignment::LEFT,TextVAlignment::TOP);
		
		pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = Label::createWithSystemFont(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
    pSpriteBK->setVisible(false);

}

void UI_CLEANJOB::InitUI_WALL()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point IconSite_Help=Vec2(955*W_unit,winSize.height-(99-75)*H_unit);		
	Point IconSite_ZoomOut=Vec2(782*W_unit,winSize.height-238*H_unit);		
	Point IconSite_ZoomIn=Vec2(922*W_unit,winSize.height-238*H_unit);		
	Point LabSite_ZoomRate=Vec2(550*W_unit,winSize.height-238*H_unit);
	Point LabSite_Clean=Vec2(90*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Virual=Vec2(425*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Undo=Vec2(705*W_unit,winSize.height-1519*H_unit);		
	Point LabSite_Delete=Vec2(860*W_unit,winSize.height-1519*H_unit);
	Point IconSite_WALL=Vec2(485*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_UNDO=Vec2(705*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_DELETE=Vec2(860*W_unit,winSize.height-1583*H_unit);		
	Point IconSite_MAPBG=Vec2(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	addChild(pSpriteBK); //加入 layer 層
	pSpriteBK->setColor(COLOR_STRING_BLACK);
    saveSprite(CLEJOBPG_WALL, CLEWALL_BK, pSpriteBK);

//	m_BtnArray[CLEJOBPG_WALL]->addObject(pSpriteBK);

	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
    saveSprite(CLEJOBPG_WALL, CLEWALL_HELP, pSprBtnIcon);
    //m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprBtnIcon,CLEWALL_HELP);

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
    saveSprite(CLEJOBPG_WALL, CLEWALL_TITAL, pSpriteAction);
//	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSpriteAction,CLEWALL_TITAL);


	auto pLabel = Label::createWithSystemFont(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	//ZOOM IN OUT
	auto pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_WALL, CLEWALL_ZOOMOUT, pSprite);
//	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_ZOOMOUT);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_WALL, CLEWALL_ZOOMIN, pSprite);
//	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_ZOOMIN);
	//縮放倍率
	pLabel = Label::createWithSystemFont("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
    saveLabel(CLEJOBPG_WALL, CLEWALL_ZOOMLAB, pLabel);
//	m_BtnArray[CLEJOBPG_WALL]->insertObject(pLabel,CLEWALL_ZOOMLAB);
	//下方標籤
	pLabel = Label::createWithSystemFont(STR_BTN_Wallfollowing, S_FONE, *pSTR_TITAL_SIZE,Size::Size(winSize.width*0.4,0),TextHAlignment::LEFT,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);

	pLabel = Label::createWithSystemFont(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = Label::createWithSystemFont(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11);
	//下方按鈕
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_WALL, CLEWALL_WALL, pSprite);
//	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_WALL);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_WALL, CLEWALL_UNDO, pSprite);
//	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_UNDO);
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF);
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
    saveSprite(CLEJOBPG_WALL, CLEWALL_DELETE, pSprite);
//	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_DELETE);
	//底部按鈕
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
    saveSprite(CLEJOBPG_WALL, CLEWALL_GO, pSpriteDownBtn);
//	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSpriteDownBtn,CLEWALL_GO);
	pLabel = Label::createWithSystemFont(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 11);
	//機器人圖示
	pSprite=Sprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
    saveSprite(CLEJOBPG_WALL, CLEWALL_ROBOT, pSprite);
//	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_ROBOT);
	//地圖
	pSprite=Sprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(Vec2(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
    saveSprite(CLEJOBPG_WALL, CLEWALL_MAP, pSprite);
//	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_MAP);
    pSpriteBK->setVisible(false);
}
void UI_CLEANJOB::InitUI_WALLHELP()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	Point IconSite_BG=Vec2(65*W_unit,winSize.height-156*H_unit);		
	Point IconSiteIN[6]=
	{Vec2((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,Vec2((587-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2((464-65)*W_unit,(1710+156-872)*H_unit)
	,Vec2((485-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((705-65)*W_unit,(1710+156-1490)*H_unit)
	,Vec2((860-65)*W_unit,(1710+156-1490)*H_unit)};

	Point LabSiteIN[9]={
		Vec2(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		Vec2((155-65)*W_unit,(1710+156-564)*H_unit),
		Vec2((155-65)*W_unit,(1710+156-700)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1084)*H_unit),
		Vec2((540-65)*W_unit,(1710+156-1134)*H_unit),
		Vec2((425-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((705-65)*W_unit,(1710+156-1422)*H_unit),
		Vec2((860-65)*W_unit,(1710+156-1422)*H_unit)		
	};

	const char* pIconName[6];
	pIconName[0]=S_CLEAN_BTN_WALL;
	pIconName[1]=S_CLEAN_BTN_HELP_PEN;
	pIconName[2]=S_CLEAN_BTN_HELP_ARROW;
	pIconName[3]=S_CLEAN_BTN_EDIT_WALL_DEF;
	pIconName[4]=S_CLEAN_BTN_EDIT_UNDO_DEF;
	pIconName[5]=S_CLEAN_BTN_EDIT_DELETE_DEF;
	const char* pLabName[8];
	pLabName[0]=STR_BTN_Wallfollowing;
	pLabName[1]=STR_HELP_Automatic;
	pLabName[2]=STR_HELP_Automatic2;
	pLabName[3]=STR_Clean_VirtualWall;
	pLabName[4]=STR_Clean_NoEntry;
	pLabName[5]=STR_Clean_VirtualWall;
	pLabName[6]=STR_Clean_Undo;
	pLabName[7]=STR_Clean_Delete;
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
//	pSpriteBK->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	
    saveSprite(CLEJOBPG_WALLHELP, 0, pSpriteBK);

//	m_BtnArray[CLEJOBPG_WALLHELP]->addObject(pSpriteBK);	
	for(int i=0;i<6;i++) //按鈕
	{
		auto pSprite=Sprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	auto pLabel = Label::createWithSystemFont(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,Size::Size(pSpriteBK->getContentSize().width,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<8;i++) //字串
	{
		if(i<3)
			pLabel = Label::createWithSystemFont(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(pSpriteBK->getContentSize().width*0.8,0),TextHAlignment::CENTER,TextVAlignment::TOP);
		else
			pLabel = Label::createWithSystemFont(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(pSpriteBK->getContentSize().width*0.5,0),TextHAlignment::LEFT,TextVAlignment::TOP);		
		pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = Label::createWithSystemFont(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
    pSpriteBK->setVisible(false);

}




void UI_CLEANJOB::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true);
	Show_CLEANJOBMAIN();
	//強制載入地圖
	RobotMapUpdate(); //通知地圖更新
	//InitMapData(); //載入地圖檔(OLD)
	// InitGUIMAP(); //改為RobotMain 載入
}
void UI_CLEANJOB::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();
}
void UI_CLEANJOB::Hide_AllPage()
{
	Hide_CLEANJOBMAIN();
	Hide_CORNER();
	Hide_CORNERHELP();
	Hide_MULTI();
	Hide_MULTIHELP();
	Hide_MOVETO();
	Hide_MOVETOHELP();
	Hide_AUTO();
	Hide_AUTOHELP();
	Hide_SPIRAL();
	Hide_SPIRALHELP();
	Hide_WALL();
	Hide_WALLHELP();

	PageType = CLEJOBPG_LAYER;
}
void UI_CLEANJOB::Show_CLEANJOBMAIN()
{
    auto pSprite= getSpritePoint(CLEJOBPG_MAIN, CLEJOBMAIN_BK);
    //(CCSprite*)m_BtnArray[CLEJOBPG_MAIN]->objectAtIndex(CLEJOBMAIN_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_MAIN;
}
void UI_CLEANJOB::Hide_CLEANJOBMAIN()
{
	auto pSprite= getSpritePoint(CLEJOBPG_MAIN, CLEJOBMAIN_BK);
    //(CCSprite*)m_BtnArray[CLEJOBPG_MAIN]->objectAtIndex(CLEJOBMAIN_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_CORNER()
{
	auto pSprite= getSpritePoint(CLEJOBPG_CORNER, CLECORNER_BK);
    //(CCSprite*)m_BtnArray[CLEJOBPG_CORNER]->objectAtIndex(CLECORNER_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_CORNER;
	pSprite= getSpritePoint(CLEJOBPG_CORNER, CLECORNER_MAP);
    //(CCSprite*)m_BtnArray[CLEJOBPG_CORNER]->objectAtIndex(CLECORNER_MAP);
	LoadGUIMAPData(pSprite);
	MAPRetunZero(JOBIDX_CORNER);
	MAPZoomLabUpdate(JOBIDX_CORNER);
}
void UI_CLEANJOB::Hide_CORNER()
{
	auto pSprite= getSpritePoint(CLEJOBPG_CORNER, CLECORNER_BK);
    //(CCSprite*)m_BtnArray[CLEJOBPG_CORNER]->objectAtIndex(CLECORNER_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_CORNERHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_CORNERHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_CORNERHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_CORNERHELP;
}
void UI_CLEANJOB::Hide_CORNERHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_CORNERHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_CORNERHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_MULTI()
{
	auto pSprite= getSpritePoint(CLEJOBPG_MULTI, CLEMULTI_BK);
//    (CCSprite*)m_BtnArray[CLEJOBPG_MULTI]->objectAtIndex(CLEMULTI_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_MULTI;
	pSprite= getSpritePoint(CLEJOBPG_MULTI, CLEMULTI_MAP);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEMULTI_MAP);	//MAP
	LoadGUIMAPData(pSprite);												//MAP
	MAPRetunZero(JOBIDX_MULTI);
	MAPZoomLabUpdate(JOBIDX_MULTI);

}
void UI_CLEANJOB::Hide_MULTI()
{
	auto pSprite= getSpritePoint(CLEJOBPG_MULTI, CLEMULTI_BK);
//    (CCSprite*)m_BtnArray[CLEJOBPG_MULTI]->objectAtIndex(CLEMULTI_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_MULTIHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_MULTIHELP, 0);
    //(CCSprite*)m_BtnArray[CLEJOBPG_MULTIHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_MULTIHELP;
}
void UI_CLEANJOB::Hide_MULTIHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_MULTIHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_MULTIHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}

void UI_CLEANJOB::Show_MOVETO()
{
	auto pSprite= getSpritePoint(CLEJOBPG_MOVETO, CLEMOVETO_BK);
//    (CCSprite*)m_BtnArray[CLEJOBPG_MOVETO]->objectAtIndex(CLEMOVETO_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_MOVETO;
	pSprite= getSpritePoint(CLEJOBPG_MOVETO, CLEMOVETO_MAP);
    //(CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEMOVETO_MAP);
	LoadGUIMAPData(pSprite);												
	MAPRetunZero(JOBIDX_MOVETO);
	MAPZoomLabUpdate(JOBIDX_MOVETO);
}
void UI_CLEANJOB::Hide_MOVETO()
{
	auto pSprite= getSpritePoint(CLEJOBPG_MOVETO, CLEMOVETO_BK);
//    (CCSprite*)m_BtnArray[CLEJOBPG_MOVETO]->objectAtIndex(CLEMOVETO_BK);
	pSprite->setVisible(false);
	
}
void UI_CLEANJOB::Show_MOVETOHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_MOVETOHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_MOVETOHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_MOVETOHELP;
}
void UI_CLEANJOB::Hide_MOVETOHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_MOVETOHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_MOVETOHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}

void UI_CLEANJOB::Show_AUTO()
{
	auto pSprite= getSpritePoint(CLEJOBPG_AUTO, CLEAUTO_BK);
//    (CCSprite*)m_BtnArray[CLEJOBPG_AUTO]->objectAtIndex(CLEAUTO_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_AUTO;	
	pSprite= getSpritePoint(CLEJOBPG_AUTO, CLEAUTO_MAP);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEAUTO_MAP);
	LoadGUIMAPData(pSprite);
	MAPRetunZero(JOBIDX_AUTO);
	MAPZoomLabUpdate(JOBIDX_AUTO);

}
void UI_CLEANJOB::Hide_AUTO()
{
	auto pSprite= getSpritePoint(CLEJOBPG_AUTO, CLEAUTO_BK);
//    (CCSprite*)m_BtnArray[CLEJOBPG_AUTO]->objectAtIndex(CLEAUTO_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_AUTOHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_AUTOHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_AUTOHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_AUTOHELP;
}
void UI_CLEANJOB::Hide_AUTOHELP()
{
	auto pSprite=  getSpritePoint(CLEJOBPG_AUTOHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_AUTOHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_SPIRAL()
{
	auto pSprite=  getSpritePoint(CLEJOBPG_SPIRAL, CLESPIRAL_BK);
//    (CCSprite*)m_BtnArray[CLEJOBPG_SPIRAL]->objectAtIndex(CLESPIRAL_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_SPIRAL;
	pSprite= getSpritePoint(CLEJOBPG_SPIRAL, CLESPIRAL_MAP);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLESPIRAL_MAP);
	LoadGUIMAPData(pSprite);
	MAPRetunZero(JOBIDX_SPIRAL);
	MAPZoomLabUpdate(JOBIDX_SPIRAL);
}
void UI_CLEANJOB::Hide_SPIRAL()
{
	auto pSprite= getSpritePoint(CLEJOBPG_SPIRAL, CLESPIRAL_BK);
//    (CCSprite*)m_BtnArray[CLEJOBPG_SPIRAL]->objectAtIndex(CLESPIRAL_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_SPIRALHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_SPIRALHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_SPIRALHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_SPIRALHELP;
}
void UI_CLEANJOB::Hide_SPIRALHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_SPIRALHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_SPIRALHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_WALL()
{
	auto pSprite= getSpritePoint(CLEJOBPG_WALL, CLEWALL_BK);
//    (CCSprite*)m_BtnArray[CLEJOBPG_WALL]->objectAtIndex(CLEWALL_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_WALL;
	pSprite= getSpritePoint(CLEJOBPG_WALL, CLEWALL_MAP);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEWALL_MAP);
	LoadGUIMAPData(pSprite);
	MAPRetunZero(JOBIDX_WALL);
	MAPZoomLabUpdate(JOBIDX_WALL);
}
void UI_CLEANJOB::Hide_WALL()
{
	auto pSprite= getSpritePoint(CLEJOBPG_WALL, CLEWALL_BK);
//    (CCSprite*)m_BtnArray[CLEJOBPG_WALL]->objectAtIndex(CLEWALL_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_WALLHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_WALLHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_WALLHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_WALLHELP;
}
void UI_CLEANJOB::Hide_WALLHELP()
{
	auto pSprite= getSpritePoint(CLEJOBPG_WALLHELP, 0);
//    (CCSprite*)m_BtnArray[CLEJOBPG_WALLHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}
//載入地圖
void UI_CLEANJOB::LoadGUIMAPData(Sprite* pSpriteMap)
{
	if(!IsRobotMapReady()) //第一次一定要等待地圖更新
		WaitRobotMapUpdate();
#if !NO_MAP
	int iWidth=pSpriteMap->getContentSize().width;
	int iHeight=pSpriteMap->getContentSize().height;
	Texture2D *texture = new Texture2D();

	bool val = texture->initWithData(GetGUIMAP(),Texture2D::PixelFormat::RGB888,GetGUIMAP_Width(),GetGUIMAP_Height(),Size::Size(GetGUIMAP_Width(),GetGUIMAP_Height()));
	pSpriteMap->setTexture(texture);
	pSpriteMap->setTextureRect(CCRectMake(0,0,GetGUIMAP_Width(),GetGUIMAP_Height()));
#endif
	pSpriteMap->setColor(Color3B::GRAY);
	pSpriteMap->setAnchorPoint(Vec2(0.5,0.5));
	pSpriteMap->setPosition(Vec2(winSize.width*0.5,winSize.height*0.5));

	fZoomRate=MAPReSetSize(pSpriteMap,1.0f); // 強制會到原始大小
}
//設定地圖縮放
float UI_CLEANJOB::MAPReSetSize(Sprite* pSprite ,float fSize)
{
#if !NO_MAP
	if(fSize<=GUIMAPSize_MINRATE)
		fSize=GUIMAPSize_MINRATE;
	if(fSize>=GUIMAPSize_MAXRATE)
		fSize=GUIMAPSize_MAXRATE;
#endif
	Point stCenter=pSprite->getPosition();
	float fOldZoomRate=pSprite->getScale();
	if(fSize==fOldZoomRate) 
		return fSize;
	pSprite->setScale(fSize);
	return fSize;
}
//回歸原點
void UI_CLEANJOB::MAPRetunZero(int iPagIdx)
{
#if !NO_MAP
	pose_t robot_pose={0.0,0.0,0.0};
	pose_t robot_mappose;
	robot_mappose = getRobotSiteByPose2d(robot_pose);
	float fWidthRate=winSize.width/GetGUIMAP_Width();
	float fHeightRate=winSize.height/GetGUIMAP_Height();
	auto pSpriteMAP = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPagIdx]);
	pSpriteMAP->setPosition(Point(robot_mappose.px*fWidthRate,robot_mappose.py*fHeightRate));
	auto pSpriteROBOT = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	pSpriteROBOT->setPosition(Point(robot_mappose.px*fWidthRate,robot_mappose.py*fHeightRate));
#endif
	
}
void UI_CLEANJOB::MAPZoomLabUpdate(int iPagIdx)
{
    auto pZoomLab= getLabelPoint(PageType, ZOOMLAB_IDX[iPagIdx]);
//    (CCLabelTTF*)m_BtnArray[PageType]->objectAtIndex(ZOOMLAB_IDX[iPagIdx]);
	char sTemp[12];
	sprintf(sTemp,"X %.1f",fZoomRate);
	pZoomLab->setString(sTemp);

}
void UI_CLEANJOB::MAPZoomIn(int iPagIdx)
{
#if !NO_MAP
	if(fZoomRate>=1.0)
		fZoomRate+=GUIMAPSize_UP_SPACE;
	else
		fZoomRate+=GUIMAPSize_DOWN_SPACE;
#else
    fZoomRate+=1.0;
#endif
    auto pSpriteMap= getSpritePoint(PageType, MAPBK_IDX[iPagIdx]);
    //(CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPagIdx]);
	fZoomRate=MAPReSetSize(pSpriteMap,fZoomRate);
	auto pSpriteROBOT = getSpritePoint(PageType, ROBOTICON_IDX[iPagIdx]);
    //(CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	pSpriteROBOT->setPosition(pSpriteMap->getPosition());
	pSpriteROBOT->setScale(1);
//	CCObject* pObj = nullptr;
//	CCARRAY_FOREACH(pSpriteMap->getChildren(), pObj) //搜尋所有節點 改變大小
    for(auto pObj:pSpriteMap->getChildren())
	{
		DrawNode* pNode=(DrawNode*) pObj;
		ResetContentSizeRang(pNode);
	}
	MAPZoomLabUpdate(iPagIdx);
	//ROBOTICON_IDX
	//float fWidthRate=winSize.width/GetGUIMAP_Width();
	//float fHeightRate=winSize.height/GetGUIMAP_Height();
	//auto pSpriteROBOT = (CCSprite*)m_BtnArray[iPagIdx]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	//Point RobotPoint=pSpriteROBOT->getPosition();
	//pSpriteROBOT->setPosition(Vec2(RobotPoint.x*fWidthRate,RobotPoint.y*fHeightRate));
}
void UI_CLEANJOB::MAPZoomOut(int iPagIdx)
{
#if !NO_MAP
	if(fZoomRate<1.1)
		fZoomRate-=GUIMAPSize_DOWN_SPACE;		
	else
		fZoomRate-=GUIMAPSize_UP_SPACE;
#else
    fZoomRate-=1.0;
#endif
	auto pSpriteMap= getSpritePoint(PageType, MAPBK_IDX[iPagIdx]);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPagIdx]);
	fZoomRate=MAPReSetSize(pSpriteMap,fZoomRate);
	auto pSpriteROBOT = getSpritePoint(PageType, ROBOTICON_IDX[iPagIdx]);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	pSpriteROBOT->setPosition(pSpriteMap->getPosition());
	pSpriteROBOT->setScale(1);

//	CCObject* pObj = nullptr;
//	CCARRAY_FOREACH(pSpriteMap->getChildren(), pObj) //搜尋所有節點 改變大小
    for(auto pObj:pSpriteMap->getChildren())
	{
		DrawNode* pNode=(DrawNode*) pObj;
		ResetContentSizeRang(pNode);		
	}

	MAPZoomLabUpdate(iPagIdx);
	//float fWidthRate=winSize.width/GetGUIMAP_Width();
	//float fHeightRate=winSize.height/GetGUIMAP_Height();
	//auto pSpriteROBOT = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	//Point RobotPoint=pSpriteROBOT->getPosition();
	//pSpriteROBOT->setPosition(Vec2(RobotPoint.x*fWidthRate,RobotPoint.y*fHeightRate));

}
//縮放回復到預設
void UI_CLEANJOB::MAPZoomDefault(int iPagIdx)
{
#if !NO_MAP
	auto pSpriteMap= getSpritePoint(PageType, MAPBK_IDX[iPagIdx]);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPagIdx]);
	//地圖顯示位置
    pSpriteMap->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pSpriteMap->setPosition(Point(GetGUIMAP_Width()*0.5,GetGUIMAP_Height()*0.5));
	fZoomRate=MAPReSetSize(pSpriteMap,GUIMAPSize_DEFRATE);
	//ROBOT 位置
	auto pSpriteRobot= getSpritePoint(PageType, ROBOTICON_IDX[iPagIdx]);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	pSpriteRobot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pSpriteRobot->setPosition(Point(GetGUIMAP_Width()*0.5,GetGUIMAP_Height()*0.5));
#else
    auto pSpriteMap= getSpritePoint(PageType, MAPBK_IDX[iPagIdx]);
    pSpriteMap->setPosition(winSize/2);
    auto pSpriteRobot= getSpritePoint(PageType, ROBOTICON_IDX[iPagIdx]);
    pSpriteRobot->setPosition(winSize/2);
#endif

}

//////////////////////////////////////////////////////////////////////////
//void UI_CLEANJOB::registerWithTouchDispatcher()
//{
//	CCDirector* pDirector = CCDirector::sharedDirector();
//	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
//}
void UI_CLEANJOB::TargetMove_ByMouse(Sprite* pSprite,Point MousePosition )
{
	Point MapSite = StartPoint_Target+MousePosition - StartPoint_Mouse;
//	MapSite.x=StartPoint_Target.x+(MousePosition.x-StartPoint_Mouse.x);//fZoomRate;
//	MapSite.y=StartPoint_Target.y+(MousePosition.y-StartPoint_Mouse.y);//fZoomRate;
	pSprite->setPosition(MapSite);
	
}
void UI_CLEANJOB::TargetPointSave()
{
    auto pSprite = getSpritePoint(CLEJOBPG_MOVETO, CLEMOVETO_TARGET);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEMOVETO_TARGET);
	StartPoint_Target=pSprite ->getPosition();
}

void UI_CLEANJOB::TargetMove(Point  nodePosition)
{
	auto pSprite = getSpritePoint(CLEJOBPG_MOVETO, CLEMOVETO_TARGET);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEMOVETO_TARGET);
	TargetMove_ByMouse(pSprite,nodePosition);
}
void UI_CLEANJOB::TargetSet(Point  nodePosition)
{
	
	auto pSprite = getSpritePoint(CLEJOBPG_MOVETO, CLEMOVETO_TARGET);
//    (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEMOVETO_TARGET);
	bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
	bMoveKeep=false;
	if(bTouch && pSprite ->isVisible())
	{		
		//int iPageIdx=GetPageIDxFromPageType();
		
		//auto pSpriteM = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 
		//StartPoint_Map=pSpriteM->getPosition();
		StartPoint_Target=pSprite ->getPosition();
		StartPoint_Mouse=nodePosition;
		TargetMove_ByMouse(pSprite,nodePosition);
		bMoveKeep=true;
		iChangRangFlag=1;

	}

}
//檢查是否有點選 框 將其改為未點選
void UI_CLEANJOB::SelMapRang_UnSel()
{

	if(IsMapRangTagReady()) //之前有點選 尚未刪除
	{
		int iPageIdx=GetPageIDxFromPageType();
		auto pSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
//        getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
		DrawNode *pRang=(DrawNode *)pSprite->getChildByTag(GetMapRangTag());
		SelMapRang_DrawBlackBox(pRang);
		InitMapRangTag();
	}


}
 //去除點選紅框 改為黑框
void UI_CLEANJOB::SelMapRang_DrawBlackBox(DrawNode *pRang)
{
#if !NO_MAP
	pMapRangInfo pRageInfo = (pMapRangInfo)pRang->getUserData();
	pRageInfo->bSel=false;
	pRageInfo->fX=pRang->getPositionX();
	pRageInfo->fY=pRang->getPositionY();
	DrawMapRang(pRang);	 //外框黑色
#endif
}
void UI_CLEANJOB::SelMapRang_DrawRedBox(DrawNode *pRang)
{
#if !NO_MAP
	pMapRangInfo pRageInfo = (pMapRangInfo)pRang->getUserData();
	pRageInfo->bSel=true;
	StartPoint_RangWidth=pRageInfo->iWidth;
	StartPoint_RangHeight=pRageInfo->iHeight;
	DrawMapRang(pRang);			
#endif
}
void UI_CLEANJOB::MouseDown_Function(Point  nodePosition)
{
	//判斷那一個元件
	bMoveKeep=false;
	iChangRangFlag=0;
	int iPageIdx=GetPageIDxFromPageType();
	for (int i = STARTITEM_IDX[iPageIdx]; i < ENDITEM_IDX[iPageIdx]; i++) //檢查所有已知的元件 如果是動態增加的原件 會歸類到地圖
	{  
        auto pSprite = getSpritePoint(PageType, i);
        auto iIdx=pSprite->getTag();
//        (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i);
        auto rectBox= pSprite->getBoundingBox();
        rectBox.origin+=winOrigin;
		bool bTouch=rectBox.containsPoint(nodePosition);
		if(iIdx< ROBOTICON_IDX[iPageIdx] && bTouch)
			break;

		if( (iIdx==MAPBK_IDX[iPageIdx] ||  iIdx==ROBOTICON_IDX[iPageIdx]) && bTouch)
		{//點選到地圖或是機台圖示

			bMoveKeep=true;
            auto pSpriteM = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
            //getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
			StartPoint_Map=pSpriteM->getPosition();
			StartPoint_Mouse=nodePosition;

			int iReturnValue=SelMapRang(nodePosition); //判斷是否有點選到RANG 或是編輯按鈕
			if(iReturnValue >= G_RandMaxSize && IsMapRangTagReady())
			{	//點選到編輯按鈕
				iRangBtnType=iReturnValue-G_RandMaxSize; //判別點選到那一個按鍵
				DrawNode *pRang=(DrawNode *)pSpriteM->getChildByTag(GetMapRangTag());
				pMapRangInfo pRageInfo = (pMapRangInfo)pRang->getUserData();
				StartPoint_RangWidth=pRageInfo->iWidth;
				StartPoint_RangHeight=pRageInfo->iHeight;
				iChangRangFlag=1;
			}
			else 
			{
				if(iRangBtnType==MRBTN_PEN) //使用畫筆
				{
					iReturnValue=AddMAPRang_PEN(nodePosition);
					SetMapRangTag(iReturnValue);				
					iChangRangFlag=1;
				}
				else
				{
					iRangBtnType=MRBTN_NONE; //編輯按鈕沒有被點選
					SelMapRang_UnSel();

					
					SetMapRangTag(iReturnValue);				//iRangTagIdx=iReturnValue;

					if(IsMapRangTagReady())
					{
						DrawNode *pRang=(DrawNode *)pSpriteM->getChildByTag(GetMapRangTag());
						StartPoint_Rang=pRang->getPosition();							
						if(pRang->getZOrder()<iMaxZord) 
						{
							iMaxZord++;
							pSpriteM->reorderChild(pRang,iMaxZord);
						}
						//外框變紅色
						SelMapRang_DrawRedBox(pRang);
						iChangRangFlag=1;
					}


				}
			}					
			break;
		}					
	}  

}

//滑鼠點下!判斷是那個元件
bool UI_CLEANJOB::onTouchBegan(Touch* touch, Event* event)
{
	Point touchLocation = touch->getLocation();    
	Point nodePosition = convertToNodeSpace( touchLocation );
	if(!isVisible()) //有顯示才繼續
		return false;

	switch(PageType)
	{
	default:
		break;
	case CLEJOBPG_MAIN:
		{
			//判斷那一個元件
           auto sprites=getSpriteArray(CLEJOBPG_MAIN);
           for(auto pSprite:*sprites)
			{
                auto iIdx=pSprite->getTag();
                if(iIdx>=CLEJOBMAIN_CORNER && iIdx<=CLEJOBMAIN_WALL)
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bool bTouch = rectBox.containsPoint(nodePosition);
                    BtnColorSetting(iIdx,bTouch);
                }
				
			}
           // InitPageArge();
		}
		break;
	case CLEJOBPG_MOVETO:
		TargetSet(nodePosition);
		if(bMoveKeep)
			break;
	case CLEJOBPG_CORNER:
	case CLEJOBPG_MULTI:	
	case CLEJOBPG_AUTO:
	case CLEJOBPG_SPIRAL:
	case CLEJOBPG_WALL:	
		MouseDown_Function(nodePosition);
		break;
		break;

	}

	return true;
}
int UI_CLEANJOB::GetPageIDxFromPageType()
{
	switch(PageType)
	{

	default:
	case	CLEJOBPG_CORNER:
		return JOBIDX_CORNER;
		break;
	case 	CLEJOBPG_MULTI:
		return JOBIDX_MULTI;
	case 	CLEJOBPG_MOVETO:
		return	JOBIDX_MOVETO;
	case 	CLEJOBPG_AUTO:
		return JOBIDX_AUTO;
	case 	CLEJOBPG_SPIRAL:
		return JOBIDX_SPIRAL;
	case 	CLEJOBPG_WALL:
		return JOBIDX_WALL;
	}
}
void UI_CLEANJOB::MapMove(Point MousePosition )
{
	int iPageIdx=GetPageIDxFromPageType();
    auto pSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
    //getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
	Point MapSite = StartPoint_Map+MousePosition-StartPoint_Mouse;
//	MapSite.x=StartPoint_Map.x+(MousePosition.x-StartPoint_Mouse.x);
//	MapSite.y=StartPoint_Map.y+(MousePosition.y-StartPoint_Mouse.y);
	pSprite->setPosition(MapSite);
	pSprite = getSpritePoint(PageType, ROBOTICON_IDX[iPageIdx]);
    //(CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPageIdx]); //ROBOT
	pSprite ->setPosition(MapSite);
	if(PageType==CLEJOBPG_MOVETO)
	{ //移動圖釘
		TargetMove(MousePosition);		
	}
}

void UI_CLEANJOB::MapRangMove(Point MousePosition )
{
	int iPageIdx=GetPageIDxFromPageType();
	auto pSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
    //getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
	DrawNode* pRangNode=(DrawNode*)pSprite->getChildByTag(GetMapRangTag());
	if(pRangNode==NULL)
		return;
	Point MapSite;
	MapSite.x=StartPoint_Rang.x+(MousePosition.x-StartPoint_Mouse.x)/fZoomRate;
	MapSite.y=StartPoint_Rang.y+(MousePosition.y-StartPoint_Mouse.y)/fZoomRate;
	pRangNode->setPosition(MapSite);
}
void UI_CLEANJOB::MapRangReRoation(Point MousePosition )
{
	int iPageIdx=GetPageIDxFromPageType();
	auto pSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
    //getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
	DrawNode* pRangNode=(DrawNode*)pSprite->getChildByTag(GetMapRangTag());
	//pMapRangInfo pInfo=(pMapRangInfo)pRangNode->getUserData();
	Point MapSite,RangSite=pRangNode->getPosition();
	MapSite.x=StartPoint_Rang.x+(MousePosition.x-StartPoint_Mouse.x)/fZoomRate;
	MapSite.y=StartPoint_Rang.y+(MousePosition.y-StartPoint_Mouse.y)/fZoomRate;
	float fDx=MapSite.x-RangSite.x;
	float fDy=MapSite.y-RangSite.y;
	//float fRoation=atan2(-fDy,fDx);
	float fRoation=atan2(fDy,fDx);
	float fAngel=fRoation/M_PI*180;

	pRangNode->setRotation(-fAngel);
}
void UI_CLEANJOB::MapRangReSize_H(Point MousePosition )
{
	int iPageIdx=GetPageIDxFromPageType();
	auto pSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
    //getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
	DrawNode* pRangNode=(DrawNode*)pSprite->getChildByTag(GetMapRangTag());
	pMapRangInfo pInfo=(pMapRangInfo)pRangNode->getUserData();	
	pInfo->fX=pRangNode->getPositionX();
	pInfo->fY=pRangNode->getPositionY();
	pInfo->iWidth=StartPoint_RangWidth+(MousePosition.x-StartPoint_Mouse.x)/fZoomRate;
#ifdef NO_ELLIPSE
	if(pInfo->iType==MRType_Circle) //暫時無法畫出橢圓
		pInfo->iHeight= pInfo->iWidth;
#endif
	DrawMapRang(pRangNode);

}
void UI_CLEANJOB::MapRangReSize_V(Point MousePosition )
{	
	int iPageIdx=GetPageIDxFromPageType();
	auto pSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
    //getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
	DrawNode* pRangNode=(DrawNode*)pSprite->getChildByTag(GetMapRangTag());
	pMapRangInfo pInfo=(pMapRangInfo)pRangNode->getUserData();
	pInfo->fX=pRangNode->getPositionX();
	pInfo->fY=pRangNode->getPositionY();
	pInfo->iHeight = StartPoint_RangHeight+(MousePosition.y-StartPoint_Mouse.y)/fZoomRate;
#ifdef NO_ELLIPSE
	if(pInfo->iType==MRType_Circle) //暫時無法畫出橢圓
		pInfo->iWidth = pInfo->iHeight;
#endif
	DrawMapRang(pRangNode);
}

void UI_CLEANJOB::SaveRangStateToInfo()
{
#if !NO_MAP
	int iPageIdx=GetPageIDxFromPageType();
	auto pSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
    //getSpritePoint(PageType, MAPBK_IDX[iPageIdx]);
	DrawNode* pRangNode=(DrawNode*)pSprite->getChildByTag(GetMapRangTag());
	pMapRangInfo pInfo=(pMapRangInfo)pRangNode->getUserData();
	int iSpaceIdx=GetNewRangSpace();
	if(iSpaceIdx==-1)	{return;} //空間不足

	pMapRangInfo pNewInfo=&stRangInfo[iSpaceIdx];
	memcpy(pNewInfo,pInfo,sizeof(MapRangInfo));
	pNewInfo->ID=iSpaceIdx;
	pNewInfo->fX=pRangNode->getPositionX();
	pNewInfo->fY=pRangNode->getPositionY();
	pNewInfo->iAngle=pRangNode->getRotation();
	pNewInfo->iBefIdx=pInfo->ID;
	pRangNode->setUserData(pNewInfo);
#endif
}

void UI_CLEANJOB::onTouchMoved(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr){ return;}
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
    
	if(bMoveKeep)
	{
		if(!IsMapRangTagReady()) //沒有點選範圍
		{
			if(iChangRangFlag==1 && PageType== CLEJOBPG_MOVETO) // move TARGET
			{
				TargetMove(nodePosition);
			}
			else
				MapMove(nodePosition );
		}
		else
		{
			switch(iRangBtnType)
			{
			case MRBTN_NONE://移動位置 
				MapRangMove(nodePosition );
				iChangRangFlag=2;
				break;
			case MRBTN_ROTATE://改變方向
				MapRangReRoation(nodePosition );
				iChangRangFlag=2;
				break;
			case MRBTN_HORIZONTAL://改變大小
				if(iChangRangFlag==1) //只有第一次記錄改變
					SaveRangStateToInfo();
				MapRangReSize_H(nodePosition );
				iChangRangFlag=3;
				break;
			case MRBTN_VERTICAL://改變大小
				if(iChangRangFlag==1) //只有第一次記錄改變
					SaveRangStateToInfo();
				MapRangReSize_V(nodePosition );
				iChangRangFlag=3;
				break;
			case MRBTN_PEN:
				DrawMapRangByPEN(nodePosition);
				break;
			}
			
			

			
		}
		
	}
}
//檢查標示範圍點選
void UI_CLEANJOB::MAPRangItemCheck()
{
	if(iChangRangFlag==2) //移動範圍 或是選轉角度 
	{
		SaveRangStateToInfo();
		iChangRangFlag=0;
	}

	if(iRangBtnType==MRBTN_PEN && iChangRangFlag==1 && GetMapRangTag()!=-1) //使用畫筆 
	{
		iRangBtnType=MRBTN_NONE;
		iChangRangFlag=0;
		int iPageIdx=GetPageIDxFromPageType();
		auto pSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 
		DrawNode* pRangNode=(DrawNode*)pSprite->getChildByTag(GetMapRangTag());
		pMapRangInfo pInfo=(pMapRangInfo)pRangNode->getUserData();	

		//內層座標移動至外層座標左下角，方便日後可以選取
		DrawNode* pDrawPen=(DrawNode*)pRangNode->getChildByTag(G_DRAWPENTAG);
		float fNewX=pRangNode->getPositionX()+pInfo->fEdgeLeft;
		float fNewY=pRangNode->getPositionY()+pInfo->fEdgeDown;
		Point OldSite=pDrawPen->getPosition();
		pDrawPen->setPosition(Vec2(-pInfo->fEdgeLeft,-pInfo->fEdgeDown)); //因為起始點為00 所以直接減
		//pRangNode->setPosition(Vec2(fNewX,fNewY));
		pInfo->fX=fNewX;
		pInfo->fY=fNewY;
		pInfo->bSel=true;
		DrawMapRang(pRangNode);
	}
	if(iRangBtnType==MRBTN_PEN && iChangRangFlag==0) //使用畫筆 初次點選
	{
		SelMapRang_UnSel();//檢查是否選定範圍
	}
}

void UI_CLEANJOB::onTouchEnded(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr){ return;}
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
	switch(PageType)
	{
	default:
		break;
	case CLEJOBPG_WALLHELP:
		{
            auto pSprite = getSpritePoint(CLEJOBPG_WALLHELP, 0);
            auto rectBox= pSprite->getBoundingBox();
            rectBox.origin+=winOrigin;
            auto bTouch = rectBox.containsPoint(nodePosition);
			if(bTouch)
			{
				Hide_WALLHELP();
				Show_WALL();
			}

		}

		break;
	case CLEJOBPG_WALL:
		{
			//判斷那一個元件
            auto sprites=getSpriteArray(CLEJOBPG_WALL);
            auto bTouch = false;
            for(auto pSprite:*sprites)
			{
                auto Idx=pSprite->getTag();
                auto rectBox= pSprite->getBoundingBox();
                rectBox.origin+=winOrigin;
                bTouch = rectBox.containsPoint(nodePosition);
				if(bTouch && Idx!=0 && pSprite->isVisible())
				{
					Function_WALL(Idx);
					break;
				}
			}  
			MAPRangItemCheck(); 
		}
		break;

	case CLEJOBPG_SPIRALHELP:
		{
            auto pSprite = getSpritePoint(CLEJOBPG_SPIRALHELP, 0);
            auto rectBox= pSprite->getBoundingBox();
            rectBox.origin+=winOrigin;
            auto bTouch = rectBox.containsPoint(nodePosition);
			if(bTouch)
			{
				Hide_SPIRALHELP();
				Show_SPIRAL();
			}

		}

		break;
	case CLEJOBPG_SPIRAL:
		{
            //判斷那一個元件
            auto sprites=getSpriteArray(CLEJOBPG_SPIRAL);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            {
                auto Idx=pSprite->getTag();
                auto rectBox= pSprite->getBoundingBox();
                rectBox.origin+=winOrigin;
                bTouch = rectBox.containsPoint(nodePosition);
                if(bTouch && Idx!=0 && pSprite->isVisible())
                {
                    Function_SPIRAL(Idx);
                    break;
                }
            }  
            MAPRangItemCheck();

		}
		break;
	case CLEJOBPG_AUTOHELP:
		{
            auto pSprite = getSpritePoint(CLEJOBPG_AUTOHELP, 0);
            auto rectBox= pSprite->getBoundingBox();
            rectBox.origin+=winOrigin;
            auto bTouch = rectBox.containsPoint(nodePosition);
//			auto pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(0);
//			bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
			if(bTouch)
			{
				Hide_AUTOHELP();
				Show_AUTO();
			}

		}

		break;
	case CLEJOBPG_AUTO:
		{
            //判斷那一個元件
            auto sprites=getSpriteArray(CLEJOBPG_AUTO);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            {
                auto Idx=pSprite->getTag();
                auto rectBox= pSprite->getBoundingBox();
                rectBox.origin+=winOrigin;
                bTouch = rectBox.containsPoint(nodePosition);
                if(bTouch && Idx!=0 && pSprite->isVisible())
                {
                    Function_AUTO(Idx);
                    break;
                }
            }
            MAPRangItemCheck();

		}
		break;

	case CLEJOBPG_MOVETOHELP:
		{
            auto pSprite = getSpritePoint(CLEJOBPG_MOVETOHELP, 0);
            auto rectBox= pSprite->getBoundingBox();
            rectBox.origin+=winOrigin;
            auto bTouch = rectBox.containsPoint(nodePosition);
//			auto pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(0);
//			bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
			if(bTouch)
			{
				Hide_MOVETOHELP();
				Show_MOVETO();
			}

		}

		break;
	case CLEJOBPG_MOVETO:
		{
            //判斷那一個元件
            auto sprites=getSpriteArray(CLEJOBPG_MOVETO);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            {
                auto Idx=pSprite->getTag();
                auto rectBox= pSprite->getBoundingBox();
                rectBox.origin+=winOrigin;
                bTouch = rectBox.containsPoint(nodePosition);
                if(bTouch && Idx!=0 && pSprite->isVisible())
                {
                    Function_MOVETO(Idx);
                    break;
                }
            }
            MAPRangItemCheck();

		}
		break;
	case CLEJOBPG_MULTIHELP:
		{
            auto pSprite = getSpritePoint(CLEJOBPG_MULTIHELP, 0);
            auto rectBox= pSprite->getBoundingBox();
            rectBox.origin+=winOrigin;
            auto bTouch = rectBox.containsPoint(nodePosition);
			if(bTouch)
			{
				Hide_MULTIHELP();
				Show_MULTI();
			}

		}

		break;
	case CLEJOBPG_MULTI:
		{
            //判斷那一個元件
            auto sprites=getSpriteArray(CLEJOBPG_MULTI);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            {
                auto Idx=pSprite->getTag();
                auto rectBox= pSprite->getBoundingBox();
                rectBox.origin+=winOrigin;
                bTouch = rectBox.containsPoint(nodePosition);
                if(bTouch && pSprite->isVisible() && Idx!=0)
                {
                    Function_MULTI(Idx);
                    break;
                }
            }
            MAPRangItemCheck();

		}
		break;


	case CLEJOBPG_CORNERHELP:
		{
            auto pSprite = getSpritePoint(CLEJOBPG_CORNERHELP, 0);
            auto rectBox= pSprite->getBoundingBox();
            rectBox.origin+=winOrigin;
            auto bTouch = rectBox.containsPoint(nodePosition);
			if(bTouch)
			{
				Hide_CORNERHELP();
				Show_CORNER();
			}

		}
		
		break;
	case CLEJOBPG_CORNER:
		{
            //判斷那一個元件
            auto sprites=getSpriteArray(CLEJOBPG_CORNER);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            {
                auto Idx=pSprite->getTag();
                auto rectBox= pSprite->getBoundingBox();
                rectBox.origin+=winOrigin;
                bTouch = rectBox.containsPoint(nodePosition);
                if(bTouch && pSprite->isVisible())
                {
                    Function_CORNER(Idx);
                    break;
                }
            }
            MAPRangItemCheck();
 
		}
		break;

	case CLEJOBPG_MAIN:
		{
            //判斷那一個元件
            auto sprites=getSpriteArray(CLEJOBPG_MAIN);
            auto bTouch = false;
            for(auto pSprite:*sprites)
            {
                auto Idx=pSprite->getTag();
                auto rectBox= pSprite->getBoundingBox();
                rectBox.origin+=winOrigin;
                bTouch = rectBox.containsPoint(nodePosition);
                if(bTouch && pSprite->isVisible() && Idx!=0)
                {
                    Function_CLEANJOBMAIN(Idx);
                    break;
                }
            }

		}
		break;
	}
}
//按鈕變色
const Color3B BtnCleanJob[6]=
{
	Color3B(128,93,152),
	Color3B(152,213,233),
	Color3B(113,205,190),
	Color3B(201,222,113),
	Color3B(248,206,100),
	Color3B(247,137,120),
};
void UI_CLEANJOB::BtnColorSetting(int iBtnIdx,bool bSel)
{
    auto pSprite = getSpritePoint(CLEJOBPG_MAIN, iBtnIdx);
    auto pLabel= getLabelPoint(CLEJOBPG_MAIN, iBtnIdx+CLEJOBMAIN_LAB_SPACE);

	if(bSel)
	{
		pSprite ->setColor(BtnCleanJob[iBtnIdx-CLEJOBMAIN_CORNER]);
		pLabel->setColor(COLOR_STRING_WHITE);
	}
	else
	{
		pSprite ->setColor(COLOR_STRING_ORIGIN);
		pLabel->setColor(COLOR_STRING_BLACK);
	}
}

void UI_CLEANJOB::Function_CLEANJOBMAIN(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case CLEJOBMAIN_WALL:
		Hide_AllPage();
		Show_WALL();
		CleanMAPRang(JOBIDX_WALL); 
		break;
	case CLEJOBMAIN_SPIRAL:
		Hide_AllPage();
		Show_SPIRAL();
		CleanMAPRang(JOBIDX_SPIRAL); 
		break;
	case CLEJOBMAIN_AUTO:
		Hide_AllPage();
		Show_AUTO();
		CleanMAPRang(JOBIDX_AUTO); 
		break;

	case CLEJOBMAIN_MOVE:
		Hide_AllPage();
		Show_MOVETO();
		CleanMAPRang(JOBIDX_MOVETO); 
		break;

	case CLEJOBMAIN_MULTI:
		Hide_AllPage();
		Show_MULTI();		
		CleanMAPRang(JOBIDX_MULTI); //MAP
		break;

	case CLEJOBMAIN_CORNER:
		Hide_AllPage();
		Show_CORNER();
		CleanMAPRang(JOBIDX_CORNER);
		break;
	case CLEJOBMAIN_TITAL:
		ChangLayer_Main();
		break;
	}
}

void UI_CLEANJOB::Function_CORNER(int iBtnIdx)
{
	
	switch(iBtnIdx) 
	{
	default:
		break;
	case CLECORNER_HELP:
		Show_CORNERHELP();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLECORNER_ZOOMIN:
		MAPZoomIn(JOBIDX_CORNER);
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLECORNER_ZOOMOUT:
		MAPZoomOut(JOBIDX_CORNER);
		iRangBtnType=MRBTN_NONE;
		break;
	case CLECORNER_OVAL: //增加圓框
		AddMAPRang_OVAL();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLECORNER_RECT:
		AddMAPRang_RECL();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLECORNER_WALL:
		iRangBtnType=MRBTN_PEN; 
		//AddMAPRang_PEN();
		break;

	case CLECORNER_TITAL:
		Hide_AllPage();
		Show_CLEANJOBMAIN();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLECORNER_UNDO:
		UndoMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLECORNER_DELETE:
		DeleteMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLECORNER_GO: // 現有資料傳至Robot
		SelMapRang_UnSel();
		MapRangToArray(); //統計現在MAPRang範圍
		MapPenToArray();
		iRangBtnType=MRBTN_NONE; 
		//啟動機器 回到主畫面
#if PLAYERSTAGE
		RobotGo(PLAYER_DISPATCH_AI_CORNER);
#endif
		ChangLayer_Main();
		break;
	}
    auto pSprite = getSpritePoint(CLEJOBPG_CORNER, CLECORNER_WALL);

	if(iRangBtnType==MRBTN_PEN)
	{
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		pSprite ->setColor(COLOR_STRING_WHITE);
	}
}
 //統計現在MAPRang範圍
void UI_CLEANJOB::MapRangToArray()
{
#if !NO_MAP
	int iPageIdx=GetPageIDxFromPageType();
	auto pSpriteMap = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 
	Point PointMap=pSpriteMap->getPosition();
	if(pSpriteMap->getChildrenCount()==0)
		return;
	int iHeight=pSpriteMap->getContentSize().height;
	int iWidth=pSpriteMap->getContentSize().width;
	RenderTexture* pTexture_Rang= RenderTexture::create( iWidth,iHeight, Texture2D::PixelFormat::RGBA8888);
//	CCObject* pObj = nullptr;
//	pMapRangInfo pInfo;
//	auto pNewMap=Sprite::create();	
//	CCARRAY_FOREACH(pSpriteMap->getChildren(), pObj) //搜尋所有截點
    for(auto pObj:pSpriteMap->getChildren())
	{
		DrawNode* pRangNode=(DrawNode*) pObj;
		if(pRangNode!=nullptr)
		{
			pInfo=(pMapRangInfo)pRangNode->getUserData();
			float fAngel=pRangNode->getRotationX();
			//根據Info會至新畫面至新地圖
			if(pInfo!=NULL && pInfo->bUse)
			{
				if(pInfo->iType==MRType_Circle ||pInfo->iType==MRType_Square)
				{
					DrawNode *pNewRand=DrawNode::create();
					pNewMap->addChild(pNewRand);				
					DrawMapRang_RealRangByType(pNewRand,pInfo);
					pNewRand->setRotation(fAngel);
				}
			}
		}		
	}
	pTexture_Rang->begin();
	pNewMap->visit(); //將現在畫面記錄到pTexture
	pTexture_Rang->end(); 

	//將現在圖面資訊存檔至記憶體中 
	 CCImage* pImage = pTexture_Rang->newCCImage();
	 SaveRangMapToGuidata(pImage,iWidth,iHeight);

	 // 傳送至 ROBOT
#if ROBOT
	  MapRangToRobot();
#endif
	//DEBUG 儲存RANG 畫面
	pTexture_Rang->saveToFile("MapRang.png",Image::Format::PNG);  //存檔
#endif
}

//統計現在MAPRang範圍
void UI_CLEANJOB::MapPenToArray()
{
#if !NO_MAP
	int iPageIdx=GetPageIDxFromPageType(); 
	auto pSpriteMap = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 
	Point PointMap=pSpriteMap->getPosition();
	if(pSpriteMap->getChildrenCount()==0)
		return;
	int iHeight=pSpriteMap->getContentSize().height;
	int iWidth=pSpriteMap->getContentSize().width;
	RenderTexture* pTexture_Pen= RenderTexture::create( iWidth,iHeight, Texture2D::PixelFormat::RGBA8888);
	CCObject* pObj = nullptr;
	pMapRangInfo pInfo;
	auto pNewMap=Sprite::create();	
	pTexture_Pen->begin();
	CCARRAY_FOREACH(pSpriteMap->getChildren(), pObj) //搜尋所有截點
	{		
		CCDrawNode* pRangNode=(CCDrawNode*) pObj;
		if(pRangNode!=NULL)
		{
			pInfo=(pMapRangInfo)pRangNode->getUserData();
			float fAngel=pRangNode->getRotationX();
			//根據Info會至新畫面至新地圖
			if(pInfo!=NULL && pInfo->bUse)
			{
				if(pInfo->iType==MRType_Pen)
				{
					CCDrawNode *pDrawPen=(CCDrawNode *)pRangNode->getChildByTag(G_DRAWPENTAG);

					if(pDrawPen!=NULL)
					{
						pDrawPen->setPosition(Vec2(pInfo->fX-pInfo->fEdgeLeft,pInfo->fY-pInfo->fEdgeDown));
						pDrawPen->visit();//將現在畫面記錄到pTexture
						pDrawPen->setPosition(Vec2(-pInfo->fEdgeLeft,-pInfo->fEdgeDown));
					}
				}
				//else
				//{
				//	CCDrawNode *pNewRand=CCDrawNode::create();
				//	pNewMap->addChild(pNewRand);				
				//	DrawMapRang_RealRangByType(pNewRand,pInfo);
				//	pNewRand->setRotation(fAngel);
				//}
			}
		}		
	}
	pTexture_Pen->end();

	//將現在圖面資訊存檔至記憶體中 
	CCImage* pImage = pTexture_Pen->newCCImage();
	SaveMapPenImage(pImage,iWidth,iHeight);

	// 傳送至 ROBOT
#if ROBOT
	MapPenToRobot();
#endif
	//DEBUG 儲存RANG 畫面
	pTexture_Pen->saveToFile("MapRangPen.png",Image::Format::PNG);  //存檔
#endif
}

void UI_CLEANJOB::Function_MULTI(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case CLEMULTI_HELP:
		Show_MULTIHELP();
		break;
	case CLEMULTI_ZOOMIN:
		MAPZoomIn(JOBIDX_MULTI);
		break;
	case CLEMULTI_ZOOMOUT:
		MAPZoomOut(JOBIDX_MULTI);
		break;
	case CLEMULTI_TITAL:
		Hide_AllPage();
		Show_CLEANJOBMAIN();
		break;
//MAP-----------------------------------------------
	case CLEMULTI_OVAL: //增加圓框
		AddMAPRang_OVAL();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLEMULTI_RECT:
		AddMAPRang_RECL();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLEMULTI_WALL:
		iRangBtnType=MRBTN_PEN; 
		//AddMAPRang_PEN();
		break;
	case CLEMULTI_UNDO:
		UndoMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLEMULTI_DELETE:
		DeleteMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLEMULTI_GO:
		SelMapRang_UnSel();
		MapRangToArray(); //統計現在MAPRang範圍
		MapPenToArray();
		iRangBtnType=MRBTN_NONE; 
#if PLAYERSTAGE
		RobotGo(PLAYER_DISPATCH_AI_AUTO);
#endif
		ChangLayer_Main();
		break;
//MAP-----------------------------------------------
	}
    auto pSprite = getSpritePoint(CLEJOBPG_MULTI, CLEMULTI_WALL);
	if(iRangBtnType==MRBTN_PEN)
	{
        pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		pSprite ->setColor(COLOR_STRING_WHITE);
	}
}
void UI_CLEANJOB::Function_MOVETO(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case CLEMOVETO_HELP:
		Show_MOVETOHELP();
		break;
	case CLEMOVETO_ZOOMIN:
		MAPZoomIn(JOBIDX_MOVETO);
		break;
	case CLEMOVETO_ZOOMOUT:
		MAPZoomOut(JOBIDX_MOVETO);
		break;

	case CLEMOVETO_TITAL:
		Hide_AllPage();
		Show_CLEANJOBMAIN();
		break;

	case CLEMOVETO_WALL:
		iRangBtnType=MRBTN_PEN; 
		break;
	case CLEMOVETO_UNDO:
		UndoMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLEMOVETO_DELETE:
		DeleteMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLEMOVETO_TARGET: //紀錄現在位置
		TargetPointSave();
		break;
	case CLEMOVETO_GO:
		SelMapRang_UnSel();
		MapPenToArray();
		iRangBtnType=MRBTN_NONE; 
		RobotGo_Target();
		//RobotGo(PLAYER_DISPATCH_AI_MOVE2TARGET);
		ChangLayer_Main();
		break;
	}
    auto pSprite = getSpritePoint(CLEJOBPG_MOVETO, CLEMOVETO_WALL);

	if(iRangBtnType==MRBTN_PEN)
	{
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		pSprite ->setColor(COLOR_STRING_WHITE);
	}
}
void UI_CLEANJOB::Function_AUTO(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case CLEAUTO_HELP:
		Show_AUTOHELP();
		break;
	case CLEAUTO_ZOOMIN:
		MAPZoomIn(JOBIDX_AUTO);
		break;
	case CLEAUTO_ZOOMOUT:
		MAPZoomOut(JOBIDX_AUTO);
		break;

	case CLEAUTO_TITAL:
		Hide_AllPage();
		Show_CLEANJOBMAIN();
		break;

	case CLEAUTO_WALL:
		iRangBtnType=MRBTN_PEN; 
		break;
	case CLEAUTO_UNDO:
		UndoMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLEAUTO_DELETE:
		DeleteMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLEAUTO_GO:
		SelMapRang_UnSel();
		MapPenToArray();
		iRangBtnType=MRBTN_NONE; 
#if PLAYERSTAGE
		RobotGo(PLAYER_DISPATCH_AI_AUTO);
#endif
		ChangLayer_Main();
		break;
	}
    auto pSprite = getSpritePoint(CLEJOBPG_AUTO, CLEAUTO_WALL);

	if(iRangBtnType==MRBTN_PEN)
	{
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		pSprite ->setColor(COLOR_STRING_WHITE);
	}
}
void UI_CLEANJOB::Function_SPIRAL(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case CLESPIRAL_HELP:
		Show_SPIRALHELP();
		break;
	case CLESPIRAL_ZOOMIN:
		MAPZoomIn(JOBIDX_SPIRAL);
		break;
	case CLESPIRAL_ZOOMOUT:
		MAPZoomOut(JOBIDX_SPIRAL);
		break;

	case CLESPIRAL_TITAL:
		Hide_AllPage();
		Show_CLEANJOBMAIN();
		break;

	case CLESPIRAL_WALL:
		iRangBtnType=MRBTN_PEN; 
		break;
	case CLESPIRAL_UNDO:
		UndoMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLESPIRAL_DELETE:
		DeleteMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLESPIRAL_GO:
		SelMapRang_UnSel();
		MapPenToArray();
		iRangBtnType=MRBTN_NONE; 
#if PLAYERSTAGE
		RobotGo(PLAYER_DISPATCH_AI_SPIRAL_LARGE);
#endif
		ChangLayer_Main();
		break;

	}
    auto pSprite = getSpritePoint(CLEJOBPG_SPIRAL, CLESPIRAL_WALL);

	if(iRangBtnType==MRBTN_PEN)
	{
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		pSprite ->setColor(COLOR_STRING_WHITE);
	}
}
void UI_CLEANJOB::Function_WALL(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case CLEWALL_HELP:
		Show_WALLHELP();
		break;
	case CLEWALL_ZOOMIN:
		MAPZoomIn(JOBIDX_WALL);
		break;
	case CLEWALL_ZOOMOUT:
		MAPZoomOut(JOBIDX_WALL);
		break;
	case CLEWALL_TITAL:
		Hide_AllPage();
		Show_CLEANJOBMAIN();
		break;
	case CLEWALL_WALL:
		iRangBtnType=MRBTN_PEN; 
		break;
	case CLEWALL_UNDO:
		UndoMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLEWALL_DELETE:
		DeleteMapRang();
		iRangBtnType=MRBTN_NONE; 
		break;
	case CLEWALL_GO:
		SelMapRang_UnSel();
		MapPenToArray();
		iRangBtnType=MRBTN_NONE; 
#if PLAYERSTAGE
		RobotGo(PLAYER_DISPATCH_AI_WALL_FOLLOW);
#endif
		ChangLayer_Main();
		break;


	}
    auto pSprite = getSpritePoint(CLEJOBPG_WALL, CLEWALL_WALL);

	if(iRangBtnType==MRBTN_PEN)
	{
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		pSprite ->setColor(COLOR_STRING_WHITE);
	}
}
//////////////////////////////////////////////////////////////////////////
//範圍記錄管理
void UI_CLEANJOB::InitRangInfo()
{
	for(int i=0;i<G_RandMaxSize;i++)
	{
		InitOneRangInfo(i);
	}
}
inline void UI_CLEANJOB::InitOneRangInfo(int iIdx)
{
#if !NO_MAP
	memset(&stRangInfo[iIdx],0,sizeof(MapRangInfo));
	stRangInfo[iIdx].iBefIdx=-1;
	stRangInfo[iIdx].ID=iIdx;
#endif
}
//取得新的紀錄空間
int UI_CLEANJOB::GetNewRangSpace()
{
	int iIdx=-1;
#if !NO_MAP
	for(int i=0;i<G_RandMaxSize;i++)
		if(stRangInfo[i].bUse==false)
		{
			iIdx=i;
			break;
		}
#endif
	return iIdx;
}
//刪除所有記錄
void UI_CLEANJOB::DelRangSpace(int iIdx)
{
#if !NO_MAP
	int iBefIdx=0;
	do
	{
		iBefIdx=stRangInfo[iIdx].iBefIdx;
		InitOneRangInfo(iIdx);
		iIdx=iBefIdx; //刪除之前所有UNDO記錄
	}while(iBefIdx>=0 && iBefIdx<G_RandMaxSize);
#endif
}
// 紀錄圖框相關訊息 
int UI_CLEANJOB::SaveNewRangInfo(int iWidth,int iHeight,int iAngle,Point stPoint,int iType)
{
    
	int iIDx=GetNewRangSpace();
#if !NO_MAP
	if(iIDx!=-1)
	{
		stRangInfo[iIDx].ID=iIDx;
		stRangInfo[iIDx].iWidth=iWidth;
		stRangInfo[iIDx].iHeight=iHeight;
		stRangInfo[iIDx].iAngle=iAngle;
		stRangInfo[iIDx].bUse=true;
		stRangInfo[iIDx].bSel=0;
		stRangInfo[iIDx].iType=iType;
		stRangInfo[iIDx].iBefIdx=-1;
		stRangInfo[iIDx].fX=stPoint.x;
		stRangInfo[iIDx].fY=stPoint.y;		
		stRangInfo[iIDx].fEdgeLeft=-G_PenEdgeSize;
		stRangInfo[iIDx].fEdgeRight=G_PenEdgeSize;
		stRangInfo[iIDx].fEdgeTop=G_PenEdgeSize;
		stRangInfo[iIDx].fEdgeDown=-G_PenEdgeSize;
	}
#endif
	return iIDx;
}

//////////////////////////////////////////////////////////////////////////
// MAP functino
inline int UI_CLEANJOB::GetMapRangTag()
{
	if(iRangTagIdx<0 || iRangTagIdx>=G_RandMaxSize)
		return -1;
	return iRangTagIdx;
}
inline void UI_CLEANJOB::InitMapRangTag()
{
	iRangTagIdx=-1;
}
inline void UI_CLEANJOB::SetMapRangTag(int iValue)
{
	if(iValue>=0 && iValue<G_RandMaxSize)
		iRangTagIdx=iValue;
}
inline bool  UI_CLEANJOB::IsMapRangTagReady()
{
	if(iRangTagIdx<0 || iRangTagIdx>=G_RandMaxSize)
		return false;
	return true;
}
//回傳指定圖檔中最高的ZORD 若無圖 則是回傳0
int  UI_CLEANJOB::GetMAXMAPZord(Sprite* pMapSprite)
{
	int iMaxZord=0;
//	CCObject* pObj = nullptr;
//	CCARRAY_FOREACH(pMapSprite->getChildren(), pObj) //搜尋所有截點
    for(auto pSprite:pMapSprite->getChildren())
	{
//		CCDrawNode* pSprite=(CCDrawNode*) pObj;
		int iZOrder=pSprite->getZOrder();
		if(iMaxZord<=iZOrder)
		{
			iMaxZord=iZOrder;
		}
	}
	return iMaxZord;

}
//建立新的圖框! 隱藏編輯
void UI_CLEANJOB::AddMAPRang_OVAL()
{
	int iPageIdx=GetPageIDxFromPageType();
	auto pMapSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 
	DrawNode *pRang = DrawNode::create();
	//auto pSpriteROBOT = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	//Point stCen=pSpriteROBOT->getPosition(); //取得機器人位置
	//取得現在最大Zord
	iMaxZord=GetMAXMAPZord(pMapSprite);
	iMaxZord++;
	Size MapSize=pMapSprite->getContentSize();	//顯示在固定位置!!日後再做修正
	Point stCen=pMapSprite->getPosition();
	
	//CCSize MapSize=pMapSprite->boundingBox().size;
	//Point stCen=pMapSprite->boundingBox().origin;
	//MapSize=pMapSprite->boundingBox().size;
	//stCen=pMapSprite->boundingBox().origin;
	int iPolySize=50;
	Point DrawSite=Vec2(MapSize.width*0.5+iPolySize-stCen.x, MapSize.height*0.7-stCen.y-iPolySize);
	
	//長 寬 角度 位置 種類	
	int iRangTag=SaveNewRangInfo(iPolySize*2,iPolySize*2,0,DrawSite,MRType_Circle);
	if(iRangTag!=-1)
	{
#if !NO_MAP
		int iWidth=iPolySize*2;
		pMapSprite->addChild(pRang,iMaxZord,iRangTag);

		pRang->setUserData(&stRangInfo[iRangTag]); //記錄資訊
#endif
		//增加三個拉框
		auto pBtnSprite = Sprite::createWithSpriteFrameName(S_CLEAN_RANG_ROTATE_DEF);		
#ifdef ELLIPSE_WALL
		pBtnSprite->setRotationX(45);
		pBtnSprite->setPosition(Vec2(0,0));
		pRang->addChild(pBtnSprite,10,MRBTN_ROTATE);

		pBtnSprite = Sprite::createWithSpriteFrameName(S_CLEAN_RANG_HORIZONTAL_DEF);
		pRang->addChild(pBtnSprite,10,MRBTN_HORIZONTAL);
#endif

		pBtnSprite = Sprite::createWithSpriteFrameName(S_CLEAN_RANG_VERTICAL_DEF);
		pRang->addChild(pBtnSprite,10,MRBTN_VERTICAL);
		DrawMapRang(pRang);

	}


	////建立節點範圍
	//pRang->setContentSize( Size::Size(iPolySize*2, iPolySize*2) );	
	//pRang->setPosition(DrawSite);
	//
	//// 繪製圓區域
	//pRang->drawDot(Vec2(iPolySize,iPolySize), iPolySize*0.8, Color4FFromccc3B(ccc3(255,115,115)));
	//// Draw polygons	
	//Point points[] = { Point(0,iPolySize*2),
	//					 Point(iPolySize*2,iPolySize*2), 
	//					 Point(iPolySize*2,0),
	//					Vec2::ZERO};
	//pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), Color4F(0.9,0.9,0.9,0.2), 2, Color4F(0,0,0,0.6f)); //位置來源,點數.塗滿顏色,外框線,外框顏色	


}

void UI_CLEANJOB::AddMAPRang_RECL()
{
	int iPageIdx=GetPageIDxFromPageType();
	auto pMapSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 
	DrawNode *pRang = DrawNode::create();
	//取得現在最大Zord
	iMaxZord=GetMAXMAPZord(pMapSprite);
	iMaxZord++;
	Size MapSize=pMapSprite->getContentSize();
	Point stCen=pMapSprite->getPosition();
	int iPolySize=50;
	//長 寬 角度 位置 種類
	Point DrawSite=Vec2(MapSize.width*0.5+iPolySize-stCen.x, MapSize.height*0.7-stCen.y-iPolySize);


	int iRangTag=SaveNewRangInfo(iPolySize*2,iPolySize*2,0,DrawSite,MRType_Square);
	if(iRangTag!=-1)
	{
#if !NO_MAP
		int iWidth=iPolySize*2;
		pMapSprite->addChild(pRang,iMaxZord,iRangTag);
		pRang->setUserData(&stRangInfo[iRangTag]); //記錄資訊
#endif
		//增加三個拉框
		auto pBtnSprite = Sprite::createWithSpriteFrameName(S_CLEAN_RANG_ROTATE_DEF);		
		pBtnSprite->setRotationX(45);
		pBtnSprite->setPosition(Vec2(0,0));
		pRang->addChild(pBtnSprite,10,MRBTN_ROTATE);

		pBtnSprite = Sprite::createWithSpriteFrameName(S_CLEAN_RANG_HORIZONTAL_DEF);		
		pRang->addChild(pBtnSprite,10,MRBTN_HORIZONTAL);

		pBtnSprite = Sprite::createWithSpriteFrameName(S_CLEAN_RANG_VERTICAL_DEF);		
		pRang->addChild(pBtnSprite,10,MRBTN_VERTICAL);

		DrawMapRang(pRang);

	}
}
// 增加畫筆(比較特別 內部還有另外一張圖 記錄 現在的VIRUAL WALL)
int UI_CLEANJOB::AddMAPRang_PEN(Point MousePosition)
{	
	int iPageIdx=GetPageIDxFromPageType();
	auto pMapSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 
	DrawNode *pRang = DrawNode::create();
	iMaxZord=GetMAXMAPZord(pMapSprite);
	iMaxZord++;
	Size MapSize=pMapSprite->boundingBox().size;	//pMapSprite->getContentSize();
	Point stCen=pMapSprite->boundingBox().origin;
	Point MouseOnMap=Vec2((MousePosition.x-stCen.x)/fZoomRate,(MousePosition.y-stCen.y)/fZoomRate);
	pRang->setPosition(MouseOnMap);
	Point MouseOnRang=Vec2(pRang->getPositionX(),pRang->getPositionY()); //轉換成Rang 座標
	//長 寬 角度 位置 種類	
	int iRangTag=SaveNewRangInfo(G_PenEdgeSize,G_PenEdgeSize,0,MouseOnRang,MRType_Pen);
	if(iRangTag!=-1)
	{
#if !NO_MAP
		pMapSprite->addChild(pRang,iMaxZord,iRangTag);
		pRang->setUserData(&stRangInfo[iRangTag]); //記錄資訊
		CCDrawNode *pDrawPen = CCDrawNode::create();
		pDrawPen->cleanup();
		pDrawPen->drawDot(PointZero, G_PenEdgeSize, RangEdgeColor_Black);//Color4FFromccc3B(ccc3(255,0,0)));
		pRang->addChild(pDrawPen,0,G_DRAWPENTAG);
#endif
//		pRang->drawDot(PointZero, G_PenEdgeSize+10, Color4FFromccc3B(ccc3(255,255,0))); //DEBUG 繪製外層起點
		//pRang->drawDot(PointZero, G_PenEdgeSize, Color4FFromccc3B(ccc3(255,0,0)));
		//MinPenPoint=Vec2(-G_PenEdgeSize,-G_PenEdgeSize);
		//MaxPenPoint=Vec2(G_PenEdgeSize,G_PenEdgeSize);	
		//MinPenPoint=Vec2(MouseOnRang.x-G_PenEdgeSize,MouseOnRang.y-G_PenEdgeSize);
		//MaxPenPoint=Vec2(MouseOnRang.x+G_PenEdgeSize,MouseOnRang.y+G_PenEdgeSize);
	}
	return iRangTag;
}
void UI_CLEANJOB::DrawMapRangByPEN(Point MousePosition)
{
	int iPageIdx=GetPageIDxFromPageType();
	auto pMapSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 
	DrawNode *pRang=(DrawNode *)pMapSprite->getChildByTag(GetMapRangTag());
	pMapRangInfo pInfo=(pMapRangInfo) pRang->getUserData();
	Size MapSize=pMapSprite->boundingBox().size;	//pMapSprite->getContentSize();
	Point stCen=pMapSprite->boundingBox().origin;
	Point MouseOnMap=Vec2((MousePosition.x-stCen.x)/fZoomRate,(MousePosition.y-stCen.y)/fZoomRate);
	//CCSize MapSize=pMapSprite->getContentSize();
	//Point MouseOnMap=Vec2(MousePosition.x+MapSize.width*0.5-StartPoint_Map.x,MousePosition.y+MapSize.height*0.5-StartPoint_Map.y);
	Point MouseOnRang=Vec2(MouseOnMap.x-pRang->getPositionX(),MouseOnMap.y-pRang->getPositionY()); //轉換成Rang 座標
	DrawNode* pDrawPen=(DrawNode* )pRang->getChildByTag(G_DRAWPENTAG);
	pDrawPen->drawDot(MouseOnRang, G_PenEdgeSize, RangEdgeColor_Black); //畫在內層RangEdgeColor_Black
	if(MouseOnRang.x-G_PenEdgeSize<pInfo->fEdgeLeft)
		pInfo->fEdgeLeft=MouseOnRang.x-G_PenEdgeSize;
	if(MouseOnRang.x+G_PenEdgeSize>pInfo->fEdgeRight)
		pInfo->fEdgeRight=MouseOnRang.x+G_PenEdgeSize;
	if(MouseOnRang.y+G_PenEdgeSize>pInfo->fEdgeTop)
		pInfo->fEdgeTop=MouseOnRang.y+G_PenEdgeSize;
	if(MouseOnRang.y-G_PenEdgeSize<pInfo->fEdgeDown)
		pInfo->fEdgeDown=MouseOnRang.y-G_PenEdgeSize;

	

}
void UI_CLEANJOB::ResetContentSizeRang(DrawNode *pRang)
{
	pMapRangInfo pRangInfo=(pMapRangInfo)pRang->getUserData();
	if(pRangInfo==NULL) //非標準繪製物件(例如DEBUG 用)
		return;
	if(pRangInfo->bUse==false) //資料已經被清除
	{ // 自我移除
		pRang->removeAllChildren();
		pRang->getParent()->removeChild(pRang,true);
		InitMapRangTag();
		return;
	}
	pRang->setContentSize( Size::Size(pRangInfo->iWidth*fZoomRate+5, pRangInfo->iHeight*fZoomRate+5) );	
//	pRang->drawDot(PointZero, G_PenEdgeSize, Color4FFromccc3B(ccc3(255,255,0))); //DEBUG 繪製外層起點
	//DEBUG 畫出圈選範圍
	//Point points[] = { Point(0,pRangInfo->iHeight),
	//	Point(pRangInfo->iWidth,pRangInfo->iHeight), 
	//	Point(pRangInfo->iWidth,0),
	//	Vec2::ZERO};
	//pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), Color4F(0.1,0.1,0.1,0.01), 2, Color4F(1,0.5,0,1.0f)); //位置來源,點數.塗滿顏色,外框線,外框顏色	
}
 //繪製實際範圍
void UI_CLEANJOB::DrawMapRang_RealRangByType(DrawNode *pRang,pMapRangInfo pRangInfo)
{
	pRang->setPosition(Vec2(pRangInfo->fX,pRangInfo->fY));
	if(pRangInfo->iType==MRType_Circle)
	{	// 繪製圓區域 (橢圓需要自己重新包裝)
		pRang->drawDot(Vec2(pRangInfo->iWidth*0.5,pRangInfo->iHeight*0.5), pRangInfo->iWidth*0.5*0.8, Color4F(1.0f,0.451f,0.451f,0.3));		
	}
	else if(pRangInfo->iType==MRType_Square)
	{	// 繪製矩形區域
		Point points2[] = { Point(pRangInfo->iWidth*0.1,pRangInfo->iHeight*0.9),
			Point(pRangInfo->iWidth*0.9,pRangInfo->iHeight*0.9), 
			Point(pRangInfo->iWidth*0.9,pRangInfo->iHeight*0.1),
			Point(pRangInfo->iWidth*0.1,pRangInfo->iHeight*0.1)};

		pRang->drawPolygon(points2, sizeof(points2)/sizeof(points2[0]), Color4F(1.0f,0.451f,0.451f,0.3), 2, Color4F(1.0f,0.451f,0.451f,0.4)); //位置來源,點數.塗滿顏色,外框線,外框顏色

	}


}
void UI_CLEANJOB::DrawMapRang(DrawNode *pRang)
{
	pMapRangInfo pRangInfo=(pMapRangInfo)pRang->getUserData();
	if(pRangInfo->bUse==false) //資料已經被清除
	{ // 自我移除
		pRang->removeAllChildren();
		pRang->getParent()->removeChild(pRang,true);
		InitMapRangTag();
		return;
	}
	pRang->clear(); //清圖
	DrawMapRang_RealRangByType(pRang,pRangInfo); //繪製實際範圍


	//建立外框
	if(pRangInfo->iType==MRType_Pen)
	{
//		DrawNode *pDrawPen=(DrawNode *)pRang->getChildByTag(G_DRAWPENTAG);
//		pDrawPen->drawDot(PointZero, G_PenEdgeSize+5, Color4FFromccc3B(ccc3(255,100,0))); //DEBUG 繪製外層起點
//		pRang->drawDot(PointZero, G_PenEdgeSize+15, Color4FFromccc3B(ccc3(255,255,0))); //DEBUG 繪製外層起點
		//建立節點範圍
		pRangInfo->iWidth=fabs(pRangInfo->fEdgeLeft-pRangInfo->fEdgeRight);
		pRangInfo->iHeight=fabs(pRangInfo->fEdgeTop-pRangInfo->fEdgeDown);
		pRang->setContentSize( Size::Size(pRangInfo->iWidth+5, pRangInfo->iHeight+5) );	 //建立點選範圍
		//pRang->setPosition(Vec2(pRangInfo->fX,pRangInfo->fY));
		//pDrawPen->drawDot(pDrawPen->getPosition(), G_PenEdgeSize+5, Color4FFromccc3B(ccc3(255,100,0))); //DEBUG 繪製外層起點
//		pRang->drawDot(PointZero, G_PenEdgeSize+10, Color4FFromccc3B(ccc3(255,255,100))); //DEBUG 繪製外層起點
		Point points[] = { Point(0,pRangInfo->iHeight),
			Point(pRangInfo->iWidth,pRangInfo->iHeight), 
			Point(pRangInfo->iWidth,0),
			Vec2::ZERO};

		/*Point points[] = { Point(pRangInfo->fEdgeLeft,pRangInfo->fEdgeTop),
			Point(pRangInfo->fEdgeRight,pRangInfo->fEdgeTop), 
			Point(pRangInfo->fEdgeRight,pRangInfo->fEdgeDown),
			Point(pRangInfo->fEdgeLeft,pRangInfo->fEdgeDown)};*/
		if(pRangInfo->bSel) //正在被點選
			pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), RangColor_FullRed, 2, RangColor_SelRed ); //位置來源,點數.塗滿顏色,外框線,外框顏色	
		else
			pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), RangColor_FullRed, 2, RangColor_EdgeRed); //位置來源,點數.塗滿顏色,外框線,外框顏色

	}
	else
	{
		
		pRang->setContentSize( Size::Size(pRangInfo->iWidth+5, pRangInfo->iHeight+5) );	//建立節點範圍
		//pRang->setPosition(Vec2(pRangInfo->fX,pRangInfo->fY));

		Point points[] = { Point(0,pRangInfo->iHeight),
			Point(pRangInfo->iWidth,pRangInfo->iHeight), 
			Point(pRangInfo->iWidth,0),
			Vec2::ZERO};

		if(pRangInfo->bSel) //正在被點選
		{ //顯示編輯框
			pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), Color4F(0.9,0.9,0.9,0.2), 2, Color4F(1,0,0,0.6f)); //位置來源,點數.塗滿顏色,外框線,外框顏色	
//			CCObject* pObj = nullptr;
//			CCARRAY_FOREACH(pRang->getChildren(), pObj) //搜尋所有 圖框
            for(auto pSprite:pRang->getChildren())
			{
//				auto  pSprite=(auto )pObj;
				pSprite->setVisible(true);
				switch(pSprite->getTag())
				{
				case MRBTN_ROTATE:
					break;
				case MRBTN_HORIZONTAL:
					pSprite->setPosition(Vec2(pRangInfo->iWidth,pRangInfo->iHeight*0.5));
					break;
				case MRBTN_VERTICAL:
					pSprite->setPosition(Vec2(pRangInfo->iWidth*0.5,pRangInfo->iHeight));
					break;
				}
			}
		}
		else
		{
			pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), Color4F(0.9,0.9,0.9,0.2), 2, Color4F(0,0,0,0.6f)); //位置來源,點數.塗滿顏色,外框線,外框顏色	
//			CCObject* pObj = nullptr;
//			CCARRAY_FOREACH(pRang->getChildren(), pObj) //搜尋所有 圖框
            for(auto pSprite:pRang->getChildren())
			{
//				auto  pSprite=(auto )pObj;
				pSprite->setVisible(false);
			}

		}
	}


}

//判斷點選的位置是否是在範圍元件上
int  UI_CLEANJOB::SelMapRang(Point MousePosition )
{
	int iPageIdx=GetPageIDxFromPageType();
	auto pMapSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 	
//	CCObject* pObj = nullptr;
	int iTag=-1;
	//CCSize MapSize=pMapSprite->getContentSize();
	//CCSize MapSize=pMapSprite->boundingBox().size; //實際畫布尺寸
	Point MapSourcePoint=pMapSprite->boundingBox().origin; //實際畫布位置
	Point MouseOnMap=Vec2((MousePosition.x-MapSourcePoint.x)/fZoomRate,(MousePosition.y-MapSourcePoint.y)/fZoomRate); //先計算滑鼠跟實際畫布貼圖距離，再將取得差距乘上倍率
	//Point MouseOnMap=Vec2(MousePosition.x+MapSize.width*0.5-StartPoint_Map.x,MousePosition.y+MapSize.height*0.5-StartPoint_Map.y);
	//滑鼠位置 = 現在位置 + 底圖的起點 (地圖縮放的中心點-地圖寬度)
	int iZord=0;
	if(IsMapRangTagReady()) // 有點選範圍
	{
		DrawNode *pRang=(DrawNode *)pMapSprite->getChildByTag(GetMapRangTag());
		pMapRangInfo pInfo=NULL;
		if(pRang!=NULL)
			pInfo = (pMapRangInfo )pRang->getUserData();
		if(pInfo !=NULL && pInfo->bSel && pInfo->iType!=MRType_Pen) // 判斷是否有點選編輯按鈕
		{
			Point MouseOnRang=Vec2(MouseOnMap.x-pRang->getPositionX(),MouseOnMap.y-pRang->getPositionY()); //轉換成Rang 座標
			//pRang->drawDot(MouseOnRang,10,Color4F(1,0,1,1.0)); //確定位置
			float angel=pRang->getRotationX()*M_PI/180;
			Point MouseOnRangAngel=Vec2(MouseOnRang.x*cos(angel)-MouseOnRang.y*sin(angel),MouseOnRang.x*sin(angel)+MouseOnRang.y*cos(angel)); //轉換成Rang 座標*角度
			//pRang->drawDot(MouseOnRangAngel,15,Color4F(0,0,1,1.0)); //確定位置
			//iTag=G_RandMaxSize+MRBTN_NONE;
//			CCARRAY_FOREACH(pRang->getChildren(), pObj) //搜尋所有截點
            for(auto pSprite:pRang->getChildren())

			{
//				auto pSprite=(auto )pObj;
				Point SpriteSite=pSprite->getPosition();
				if(pSprite->boundingBox().containsPoint(MouseOnRangAngel))
				{
					iTag=G_RandMaxSize+pSprite->getTag();
					break;
				}
			}
			
		}
	}
	if(iTag==-1) 
	{
		//DEBUG 顯示滑鼠點選位置
		//CCDrawNode *pRang=CCDrawNode::create();
		//pRang->drawDot(MouseOnMap,10,Color4F(1,0,1,1.0)); //繪製滑鼠定位座標
		//pMapSprite->addChild(pRang);

//		CCARRAY_FOREACH(pMapSprite->getChildren(), pObj) //搜尋所有截點
        for(auto pSprite:pMapSprite->getChildren())

		{
//			CCDrawNode* pSprite=(CCDrawNode*) pObj;
			
			bool bTouch=pSprite->boundingBox().containsPoint(MouseOnMap);
			Point NewPoint=pSprite->getPosition();

			if(bTouch)
			{
				int iZTemp=pSprite->getZOrder();
				if(iZTemp>=iZord) // 抓取浮在最上頭的
				{
					iTag=pSprite->getTag();				
				}
			}
		}

	}

	return iTag; 
}
//刪除指定元件
void UI_CLEANJOB::DeleteMapRang()
{
#if !NO_MAP
	int iTagIdx=GetMapRangTag();
	if(iTagIdx==-1)		return;	
	int iPageIdx=GetPageIDxFromPageType();
	auto pMapSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 	
	DrawNode *pRang=(DrawNode *)pMapSprite->getChildByTag(iTagIdx);
	pMapRangInfo pInfo = (pMapRangInfo )pRang->getUserData();
	if(pInfo->bSel) //有被選中才刪除
	{
		DelRangSpace(pInfo->ID);//刪除空間記錄
//		pRang->removeAllChildren(); //理論上這一行不用 因為內部幫忙處理了
		pMapSprite->removeChild(pRang,true);
		InitMapRangTag();		
	}
#endif
}
//回到上一步指定元件
void  UI_CLEANJOB::UndoMapRang()
{
#if !NO_MAP

	int iTagIdx=GetMapRangTag();
	if(iTagIdx==-1)		return;	
	int iPageIdx=GetPageIDxFromPageType();
	auto pMapSprite = getSpritePoint(PageType, MAPBK_IDX[iPageIdx]); 	
	CCDrawNode *pRang=(CCDrawNode *)pMapSprite->getChildByTag(iTagIdx);
	pMapRangInfo pInfo = (pMapRangInfo )pRang->getUserData();
	if(pInfo->bSel && pInfo->iBefIdx!=-1) //有被選中才回上一步
	{
		int iUndoIdx=pInfo->iBefIdx;
		InitOneRangInfo(pInfo->ID);
		pInfo = &stRangInfo[iUndoIdx];
		pRang->setUserData(pInfo);
		DrawMapRang(pRang);
	}
#endif
}

//啟動掃地工作
void  UI_CLEANJOB::RobotGo(int iScanType)
{
#if PLAYERSTAGE
	int iPageIdx=GetPageIDxFromPageType();
	uint32_t mapIndex[2];
	int iMapSize=1;	
	mapIndex[0]=VIRUALWALLMAP_IDX[iPageIdx];
	if(iPageIdx==JOBIDX_CORNER || iPageIdx==JOBIDX_MULTI)
	{
		mapIndex[0]=ZONEMAP_IDX[iPageIdx];
		mapIndex[1]=VIRUALWALLMAP_IDX[iPageIdx];
		iMapSize=2;			
	}
	pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
		iScanType,											
		PLAYER_DISPATCH_PM_STANDBY,							
		iMapSize,					
		mapIndex);
#endif
}

//移動到指定地點
void  UI_CLEANJOB::RobotGo_Target()
{
#if PLAYERSTAGE

	uint32_t mapIndex[1];
	mapIndex[0]=VIRUALWALLMAP_IDX[CLEMOVETO_ZOOMLAB];
	player_pose2d_t goal;
	goal=MapPointToRobotPoint(StartPoint_Target.x,StartPoint_Target.y);
	//goal.px=1; //unit:m
	//goal.py=0.5; //unit:m
	//goal.pa =0.1;  //unit:rad

	pDisProxy->SetAiCmd(PLAYER_DISPATCH_INIT_DEST_AI,	
		PLAYER_DISPATCH_AI_MOVE2TARGET,											
		PLAYER_DISPATCH_PM_TURBO,							
		sizeof(mapIndex)/sizeof(uint32_t),					
		mapIndex,
		&goal);
#endif
}

void UI_CLEANJOB::setTouchEnabled(bool enable)
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
        
        touchListener->onTouchBegan = CC_CALLBACK_2(UI_CLEANJOB::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(UI_CLEANJOB::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(UI_CLEANJOB::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
    }
    else
    {
        _eventDispatcher->removeEventListener(touchListener);
        CC_SAFE_RELEASE_NULL(touchListener);
    }
}
bool UI_CLEANJOB::isTouchEnabled() const
{
    return touchEnabled;
}
Sprite* UI_CLEANJOB::getSpritePoint(int PageType,int Idx)
{
    return m_SpriteVectors.at(PageType)->at(m_itemIdx[PageType*100+Idx]);
}
Label* UI_CLEANJOB::getLabelPoint(int PageType,int Idx)
{
    return m_LabelVectors.at(PageType)->at(m_itemIdx[PageType*100+Idx]);
}
void UI_CLEANJOB::saveSprite(int PageType,int Idx,Sprite* pSprite)
{
    pSprite->setTag(Idx);
    m_itemIdx[PageType*100+Idx]=m_SpriteVectors.at(PageType)->size();
    m_SpriteVectors.at(PageType)->pushBack(pSprite);
}
void UI_CLEANJOB::saveLabel(int PageType,int Idx,Label* pLabel)
{
    pLabel->setTag(Idx);
    m_itemIdx[PageType*100+Idx]=m_LabelVectors.at(PageType)->size();
    m_LabelVectors.at(PageType)->pushBack(pLabel);
    
}
SpriteVector* UI_CLEANJOB::getSpriteArray(int pageType)
{
    return m_SpriteVectors.at(PageType);
}
LabelVector* UI_CLEANJOB::getLabelArray(int pageType)
{
    return m_LabelVectors.at(PageType);
}