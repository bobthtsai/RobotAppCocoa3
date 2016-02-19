//其他方式從陣列建立圖檔
//CCTexture2D *Texture = new CCTexture2D();
//
//static Color4B Data[1024*1024];
//
//for(int i = 0; i < 1024*1024; i++) 
//	Data[i] = black;
//Texture->initWithData(Data, sizeof(Color4B)*1024*1024, kCCTexture2DPixelFormat_RGBA8888, 1024, 1024, CCSize(1024, 1024));
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
#include <libplayerc++/playerc++.h>
#include <MAP_Function.h>
#include <player.h>
#include <cocos2d.h>
#include "dbg.h"
#define NO_ELLIPSE		//無橢圓
//#define NO_EDITZOOMINOUT	 // 編輯放棄縮放
USING_NS_CC;
using namespace std;
using namespace PlayerCc;

extern HelloWorld* m_MainView; 

extern DispatcherProxy *pDisProxy;

/// Player Client
extern PlayerClient *pPlayerclient;
const int G_RandMaxSize=4096;
const int G_PenEdgeSize=4;	//畫筆大小
const int G_MAP_ZLEVEL =8; //地圖的高度
MapRangInfo stRangInfo[G_RandMaxSize];
const ccColor4F RangEdgeColor_Black =ccc4FFromccc3B(ccc3(255,0,0)); //外框黑線
const ccColor4F RangColor_FullRed =ccc4f(0.9,0.9,0.9,0.1); //內層顏色
const ccColor4F RangColor_SelRed =ccc4f(1,0,0,0.6f); //內層被選中時顏色
const ccColor4F RangColor_EdgeRed =ccc4f(0,0,0,0.6f); //內層被選中時顏色
 
UI_CLEANJOB::UI_CLEANJOB()
{
	PageType=MAPPG_LAYER;
	fZoomRate=GUIMAPSize_DEFRATE;
	
}
UI_CLEANJOB::~UI_CLEANJOB()
{
	for(int i=0;i<MAPPG_SIZE;i++)
		m_BtnArray[i]->release();
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
	CCSprite* pSpriteMap= (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPagIdx]);	
	pSpriteMap->removeAllChildren();
}
bool UI_CLEANJOB::init()
{  
	if (!CCLayer::init()) 
	{  
		return false;  
	}  
	InitUI();  
	return true;  
}  

void UI_CLEANJOB::InitUI()
{
	winSize = CCDirector::sharedDirector()->getVisibleSize(); //取得視窗大小
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
	m_BtnArray[CLEJOBPG_MAIN] = CCArray::createWithCapacity(CLEJOBMAIN_SIZE); //按鈕存放
	m_BtnArray[CLEJOBPG_MAIN]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	m_BtnArray[CLEJOBPG_MAIN]->addObject(pSpriteBK);

	CCPoint BtnSite_Corner=CCPointMake(48*W_unit,winSize.height-244*H_unit);	
	CCPoint BtnSite_Multi=CCPointMake(48*W_unit,winSize.height-517*H_unit);	
	CCPoint BtnSite_Move=CCPointMake(48*W_unit,winSize.height-790*H_unit);	
	CCPoint BtnSite_Auto=CCPointMake(48*W_unit,winSize.height-1063*H_unit);	
	CCPoint BtnSite_Spiral=CCPointMake(48*W_unit,winSize.height-1336*H_unit);	
	CCPoint BtnSite_Wall=CCPointMake(48*W_unit,winSize.height-1609*H_unit);	
	CCPoint BtnSiteIn_Icon=CCPointMake(88*W_unit,(248-20)*H_unit);	
	CCPoint BtnSiteIn_Lab=CCPointMake(320*W_unit,(248-53)*H_unit);

	CCPoint SpriteSite_Corner=CCPointMake(137*W_unit,winSize.height-267*H_unit);	
	CCPoint LabelSite_Corner=CCPointMake(368*W_unit,winSize.height-316*H_unit);
	CCPoint SpriteSite_Multi=CCPointMake(137*W_unit,winSize.height-537*H_unit);	
	CCPoint LabelSite_Multi=CCPointMake(368*W_unit,winSize.height-587*H_unit);
	CCPoint SpriteSite_Move=CCPointMake(137*W_unit,winSize.height-810*H_unit);	
	CCPoint LabelSite_Move=CCPointMake(368*W_unit,winSize.height-860*H_unit);
	CCPoint SpriteSite_Auto=CCPointMake(137*W_unit,winSize.height-1083*H_unit);	
	CCPoint LabelSite_Auto=CCPointMake(368*W_unit,winSize.height-1133*H_unit);
	CCPoint SpriteSite_Spiral=CCPointMake(137*W_unit,winSize.height-1354*H_unit);	
	CCPoint LabelSite_Spiral=CCPointMake(368*W_unit,winSize.height-1404*H_unit);
	CCPoint SpriteSite_Wall=CCPointMake(137*W_unit,winSize.height-1628*H_unit);	
	CCPoint LabelSite_Wall=CCPointMake(368*W_unit,winSize.height-1678*H_unit);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_CleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[CLEJOBPG_MAIN]->insertObject(pSpriteAction,CLEJOBMAIN_TITAL);

	//6個工作JOB 含背景圖
	int ObjIdx_bg[6]={CLEJOBMAIN_CORNER,CLEJOBMAIN_MULTI,CLEJOBMAIN_MOVE,CLEJOBMAIN_AUTO,CLEJOBMAIN_SPIRAL,CLEJOBMAIN_WALL,};
	int ObjIdx_lab[6]={CLEJOBMAIN_LABCORNER,CLEJOBMAIN_LABMULTI,CLEJOBMAIN_LABMOVE,CLEJOBMAIN_LABAUTO,CLEJOBMAIN_LABSPIRAL,CLEJOBMAIN_LABWALL,};
	//char sIconName={{S_CLEAN_BTN_CORNER},{S_CLEAN_BTN_MULTI},{S_CLEAN_BTN_MOVE},{S_CLEAN_BTN_AUTO},{S_CLEAN_BTN_SPIRAL},{S_CLEAN_BTN_WALL}};	
	
	CCPoint pPointp[6]={BtnSite_Corner,BtnSite_Multi,BtnSite_Move,BtnSite_Auto,BtnSite_Spiral,BtnSite_Wall};
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
		CCSprite *pBtnbk=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP); //背景 
		pBtnbk->setAnchorPoint(CCPointMake(0,1));
		pBtnbk->setPosition(pPointp[i]);
		pBtnbk->setColor(COLOR_STRING_WHITE);
		pSpriteBK->addChild(pBtnbk,10);
		m_BtnArray[CLEJOBPG_MAIN]->insertObject(pBtnbk,ObjIdx_bg[i]);	
		CCSprite *pSprite=CCSprite::createWithSpriteFrameName(pIconName[i]);  //圖示
		pSprite->setAnchorPoint(CCPointMake(0,1));
		pSprite->setPosition(BtnSiteIn_Icon);
		pBtnbk->addChild(pSprite,10);	
		pLabel = CCLabelTTF::create(pLabName[i], S_FONE, *pSTR_CLEANJOB_SIZE,CCSizeMake(600*W_unit,144*H_unit),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop); //文字
		pLabel->setAnchorPoint(CCPointMake(0,1));
		pLabel->setColor(COLOR_STRING_BLACK);
		pLabel->setPosition(BtnSiteIn_Lab);
		pBtnbk->addChild(pLabel,10);
		m_BtnArray[CLEJOBPG_MAIN]->insertObject(pLabel,ObjIdx_lab[i]);	
	}

}
void UI_CLEANJOB::InitUI_CORNER()
{
	m_BtnArray[CLEJOBPG_CORNER] = CCArray::createWithCapacity(CLECORNER_SIZE); //按鈕存放
	m_BtnArray[CLEJOBPG_CORNER]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint IconSite_Help=CCPointMake(955*W_unit,winSize.height-(99-75)*H_unit);		
	CCPoint IconSite_ZoomOut=CCPointMake(782*W_unit,winSize.height-238*H_unit);		
	CCPoint IconSite_ZoomIn=CCPointMake(922*W_unit,winSize.height-238*H_unit);		
	CCPoint LabSite_ZoomRate=CCPointMake(550*W_unit,winSize.height-238*H_unit);		
	CCPoint LabSite_Clean=CCPointMake(90*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Virual=CCPointMake(425*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Undo=CCPointMake(705*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Delete=CCPointMake(860*W_unit,winSize.height-1519*H_unit);
	CCPoint IconSite_OVAL=CCPointMake(90*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_RECL=CCPointMake(265*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_WALL=CCPointMake(485*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_UNDO=CCPointMake(705*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_DELETE=CCPointMake(860*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_MAPBG=CCPointMake(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	pSpriteBK->setColor(COLOR_STRING_BLACK);
	m_BtnArray[CLEJOBPG_CORNER]->addObject(pSpriteBK);

	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));
	//pSprBtnIcon->setPosition(ccp(pSpriteAction->getContentSize().width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprBtnIcon,CLECORNER_HELP);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSpriteAction,CLECORNER_TITAL);

	
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	
	//ZOOM IN OUT
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_ZOOMOUT);
#ifdef NO_EDITZOOMINOUT
	pSprite->setVisible(false);
#endif
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_ZOOMIN);
#ifdef NO_EDITZOOMINOUT
	pSprite->setVisible(false);
#endif
	//縮放倍率
	pLabel = CCLabelTTF::create("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pLabel,CLECORNER_ZOOMLAB);

	//下方標籤
	pLabel = CCLabelTTF::create(STR_Clean_CleanStamps, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11); 

	//下方按鈕
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_OVAL_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_OVAL);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_OVAL);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_RECT_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_RECL);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_RECT);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_WALL);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_UNDO);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_DELETE);
	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSpriteDownBtn,CLECORNER_GO);

	pLabel = CCLabelTTF::create(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel);

	//機器人圖示
	pSprite=CCSprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_ROBOT);

	//地圖
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
	m_BtnArray[CLEJOBPG_CORNER]->insertObject(pSprite,CLECORNER_MAP);

}
void UI_CLEANJOB::InitUI_CORNERHELP()
{
	m_BtnArray[CLEJOBPG_CORNERHELP] = CCArray::create(); //按鈕存放
	m_BtnArray[CLEJOBPG_CORNERHELP]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	CCPoint IconSite_BG=CCPointMake(65*W_unit,winSize.height-156*H_unit);		
	CCPoint IconSiteIN[9]=
	{CCPointMake((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,CCPointMake((140-65)*W_unit,(1710+156-539)*H_unit)
	,CCPointMake((587-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake((464-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake(( 90-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((265-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((485-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((705-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((860-65)*W_unit,(1710+156-1490)*H_unit)};

	CCPoint LabSiteIN[9]={
		CCPointMake(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		CCPointMake((487-65)*W_unit,(1710+156-615)*H_unit),
		CCPointMake((487-65)*W_unit,(1710+156-665)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1084)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1134)*H_unit),
		CCPointMake((90-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((425-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((705-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((860-65)*W_unit,(1710+156-1422)*H_unit)		
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
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
	pSpriteBK->setAnchorPoint(CCPointMake(0,1));
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	

	m_BtnArray[CLEJOBPG_CORNERHELP]->addObject(pSpriteBK);	
	for(int i=0;i<9;i++) //按鈕
	{
		CCSprite *pSprite=CCSprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(CCPointMake(0,1));
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	CCLabelTTF* pLabel = CCLabelTTF::create(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(pSpriteBK->getContentSize().width,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<9;i++) //字串
	{
		CCLabelTTF* pLabel = CCLabelTTF::create(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
		pLabel->setAnchorPoint(CCPointMake(0,1));
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
}

void UI_CLEANJOB::InitUI_MULTI()
{
	m_BtnArray[CLEJOBPG_MULTI] = CCArray::createWithCapacity(CLEMULTI_SIZE); //按鈕存放
	m_BtnArray[CLEJOBPG_MULTI]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint IconSite_Help=CCPointMake(955*W_unit,winSize.height-(99-75)*H_unit);		
	CCPoint IconSite_ZoomOut=CCPointMake(782*W_unit,winSize.height-238*H_unit);		
	CCPoint IconSite_ZoomIn=CCPointMake(922*W_unit,winSize.height-238*H_unit);	
	CCPoint LabSite_ZoomRate=CCPointMake(550*W_unit,winSize.height-238*H_unit);		
	CCPoint LabSite_Clean=CCPointMake(90*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Virual=CCPointMake(425*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Undo=CCPointMake(705*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Delete=CCPointMake(860*W_unit,winSize.height-1519*H_unit);
	CCPoint IconSite_OVAL=CCPointMake(90*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_RECL=CCPointMake(265*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_WALL=CCPointMake(485*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_UNDO=CCPointMake(705*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_DELETE=CCPointMake(860*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_MAPBG=CCPointMake(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	pSpriteBK->setColor(COLOR_STRING_BLACK);
	m_BtnArray[CLEJOBPG_MULTI]->addObject(pSpriteBK);

	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));
	//pSprBtnIcon->setPosition(ccp(pSpriteAction->getContentSize().width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);

	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprBtnIcon,CLEMULTI_HELP);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSpriteAction,CLEMULTI_TITAL);


	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	//ZOOM IN OUT
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_ZOOMOUT);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_ZOOMIN);

	//縮放倍率
	pLabel = CCLabelTTF::create("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pLabel,CLEMULTI_ZOOMLAB);

	//下方標籤
	pLabel = CCLabelTTF::create(STR_Clean_CleanStamps, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11);
	//下方按鈕
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_MUTIOVAL_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_OVAL);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_OVAL);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_MUTIRECT_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_RECL);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_RECT);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_WALL);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_UNDO);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_DELETE);
	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSpriteDownBtn,CLEMULTI_GO);
	pLabel = CCLabelTTF::create(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 11);

	//機器人圖示
	pSprite=CCSprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_ROBOT);
	//地圖
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
	m_BtnArray[CLEJOBPG_MULTI]->insertObject(pSprite,CLEMULTI_MAP);

}
void UI_CLEANJOB::InitUI_MULTIHELP()
{
	m_BtnArray[CLEJOBPG_MULTIHELP] = CCArray::create(); //按鈕存放
	m_BtnArray[CLEJOBPG_MULTIHELP]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	CCPoint IconSite_BG=CCPointMake(65*W_unit,winSize.height-156*H_unit);		
	CCPoint IconSiteIN[9]=
	{CCPointMake((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,CCPointMake((140-65)*W_unit,(1710+156-539)*H_unit)
	,CCPointMake((587-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake((464-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake(( 90-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((265-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((485-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((705-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((860-65)*W_unit,(1710+156-1490)*H_unit)};

	CCPoint LabSiteIN[9]={
		CCPointMake(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		CCPointMake((487-65)*W_unit,(1710+156-615)*H_unit),
		CCPointMake((487-65)*W_unit,(1710+156-665)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1084)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1134)*H_unit),
		CCPointMake((90-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((425-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((705-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((860-65)*W_unit,(1710+156-1422)*H_unit)		
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
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
	pSpriteBK->setAnchorPoint(CCPointMake(0,1));
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	

	m_BtnArray[CLEJOBPG_MULTIHELP]->addObject(pSpriteBK);	
	for(int i=0;i<9;i++) //按鈕
	{
		CCSprite *pSprite=CCSprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(CCPointMake(0,1));
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	CCLabelTTF* pLabel = CCLabelTTF::create(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(pSpriteBK->getContentSize().width,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<9;i++) //字串
	{
		CCLabelTTF* pLabel = CCLabelTTF::create(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
		pLabel->setAnchorPoint(CCPointMake(0,1));
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
}
void UI_CLEANJOB::InitUI_MOVETO()
{
	m_BtnArray[CLEJOBPG_MOVETO] = CCArray::createWithCapacity(CLEMOVETO_SIZE); //按鈕存放
	m_BtnArray[CLEJOBPG_MOVETO]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint IconSite_Help=CCPointMake(955*W_unit,winSize.height-(99-75)*H_unit);		
	CCPoint IconSite_ZoomOut=CCPointMake(782*W_unit,winSize.height-238*H_unit);		
	CCPoint IconSite_ZoomIn=CCPointMake(922*W_unit,winSize.height-238*H_unit);		
	CCPoint LabSite_ZoomRate=CCPointMake(550*W_unit,winSize.height-238*H_unit);
	CCPoint LabSite_Clean=CCPointMake(90*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Virual=CCPointMake(425*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Undo=CCPointMake(705*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Delete=CCPointMake(860*W_unit,winSize.height-1519*H_unit);
	CCPoint IconSite_WALL=CCPointMake(485*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_UNDO=CCPointMake(705*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_DELETE=CCPointMake(860*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_TARGET=CCPointMake(winSize.width*0.5,winSize.height*0.6);		
	CCPoint IconSite_MAPBG=CCPointMake(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	pSpriteBK->setColor(COLOR_STRING_BLACK);
	m_BtnArray[CLEJOBPG_MOVETO]->addObject(pSpriteBK);

	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprBtnIcon,CLEMOVETO_HELP);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSpriteAction,CLEMOVETO_TITAL);


	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	//ZOOM IN OUT
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_ZOOMOUT);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_ZOOMIN);
	//縮放倍率
	pLabel = CCLabelTTF::create("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pLabel,CLEMOVETO_ZOOMLAB);
	//下方標籤
	pLabel = CCLabelTTF::create(STR_BTN_Movetotargetjob, S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(winSize.width*0.4,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);

	pLabel = CCLabelTTF::create(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11);
	//下方按鈕
	//pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_OVAL_DEF); 
	//pSprite->setAnchorPoint(CCPointMake(0,1));
	//pSprite->setPosition(IconSite_OVAL);
	//pSpriteBK->addChild(pSprite,12);
	//m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_OVAL);
	//pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_RECT_DEF); 
	//pSprite->setAnchorPoint(CCPointMake(0,1));
	//pSprite->setPosition(IconSite_RECL);
	//pSpriteBK->addChild(pSprite,12);
	//m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_RECT);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_WALL);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_UNDO);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_DELETE);
	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSpriteDownBtn,CLEMOVETO_GO);
	pLabel = CCLabelTTF::create(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 11);
	//目的地(圖釘)
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_MOVETO_TAGGET); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_TARGET);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_TARGET);

	
	//機器人圖示
	pSprite=CCSprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_ROBOT);
	//地圖

	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
	m_BtnArray[CLEJOBPG_MOVETO]->insertObject(pSprite,CLEMOVETO_MAP);

}
void UI_CLEANJOB::InitUI_MOVETOHELP()
{
	m_BtnArray[CLEJOBPG_MOVETOHELP] = CCArray::create(); //按鈕存放
	m_BtnArray[CLEJOBPG_MOVETOHELP]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	CCPoint IconSite_BG=CCPointMake(65*W_unit,winSize.height-156*H_unit);		
	CCPoint IconSiteIN[7]=
	{CCPointMake((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,CCPointMake((140-65)*W_unit,(1710+156-539)*H_unit)
	,CCPointMake((587-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake((464-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake((485-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((705-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((860-65)*W_unit,(1710+156-1490)*H_unit)};

	CCPoint LabSiteIN[9]={
		CCPointMake(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		CCPointMake((487-65)*W_unit,(1710+156-615)*H_unit),
		CCPointMake((487-65)*W_unit,(1710+156-665)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1084)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1134)*H_unit),
		CCPointMake((425-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((705-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((860-65)*W_unit,(1710+156-1422)*H_unit)		
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
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
	pSpriteBK->setAnchorPoint(CCPointMake(0,1));
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	

	m_BtnArray[CLEJOBPG_MOVETOHELP]->addObject(pSpriteBK);	
	for(int i=0;i<7;i++) //按鈕
	{
		CCSprite *pSprite=CCSprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(CCPointMake(0,1));
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	CCLabelTTF* pLabel = CCLabelTTF::create(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(pSpriteBK->getContentSize().width,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<8;i++) //字串
	{
		CCLabelTTF* pLabel = CCLabelTTF::create(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,CCSizeMake(pSpriteBK->getContentSize().width*0.5,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
		pLabel->setAnchorPoint(CCPointMake(0,1));
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
}
void UI_CLEANJOB::InitUI_AUTO()
{
	m_BtnArray[CLEJOBPG_AUTO] = CCArray::createWithCapacity(CLEAUTO_SIZE); //按鈕存放
	m_BtnArray[CLEJOBPG_AUTO]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint IconSite_Help=CCPointMake(955*W_unit,winSize.height-(99-75)*H_unit);		
	CCPoint IconSite_ZoomOut=CCPointMake(782*W_unit,winSize.height-238*H_unit);		
	CCPoint IconSite_ZoomIn=CCPointMake(922*W_unit,winSize.height-238*H_unit);		
	CCPoint LabSite_ZoomRate=CCPointMake(550*W_unit,winSize.height-238*H_unit);
	CCPoint LabSite_Clean=CCPointMake(90*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Virual=CCPointMake(425*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Undo=CCPointMake(705*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Delete=CCPointMake(860*W_unit,winSize.height-1519*H_unit);
	CCPoint IconSite_WALL=CCPointMake(485*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_UNDO=CCPointMake(705*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_DELETE=CCPointMake(860*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_MAPBG=CCPointMake(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	pSpriteBK->setColor(COLOR_STRING_BLACK);
	m_BtnArray[CLEJOBPG_AUTO]->addObject(pSpriteBK);

	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprBtnIcon,CLEAUTO_HELP);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSpriteAction,CLEAUTO_TITAL);


	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	//ZOOM IN OUT
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_ZOOMOUT);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_ZOOMIN);
	//縮放倍率
	pLabel = CCLabelTTF::create("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pLabel,CLEAUTO_ZOOMLAB);
	//下方標籤
	pLabel = CCLabelTTF::create(STR_BTN_Automode, S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(winSize.width*0.3,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);

	pLabel = CCLabelTTF::create(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11);
	//下方按鈕
	//pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_OVAL_DEF); 
	//pSprite->setAnchorPoint(CCPointMake(0,1));
	//pSprite->setPosition(IconSite_OVAL);
	//pSpriteBK->addChild(pSprite,12);
	//m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprBtnIcon,CLEAUTO_OVAL);
	//pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_RECT_DEF); 
	//pSprite->setAnchorPoint(CCPointMake(0,1));
	//pSprite->setPosition(IconSite_RECL);
	//pSpriteBK->addChild(pSprite,12);
	//m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprBtnIcon,CLEAUTO_RECT);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_WALL);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_UNDO);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_DELETE);
	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSpriteDownBtn,CLEAUTO_GO);
	pLabel = CCLabelTTF::create(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 11);
	//機器人圖示
	pSprite=CCSprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_ROBOT);
	//地圖
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
	m_BtnArray[CLEJOBPG_AUTO]->insertObject(pSprite,CLEAUTO_MAP);

}
void UI_CLEANJOB::InitUI_AUTOHELP()
{
	m_BtnArray[CLEJOBPG_AUTOHELP] = CCArray::create(); //按鈕存放
	m_BtnArray[CLEJOBPG_AUTOHELP]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	CCPoint IconSite_BG=CCPointMake(65*W_unit,winSize.height-156*H_unit);		
	CCPoint IconSiteIN[6]=
	{CCPointMake((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,CCPointMake((587-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake((464-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake((485-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((705-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((860-65)*W_unit,(1710+156-1490)*H_unit)};

	CCPoint LabSiteIN[9]={
		CCPointMake(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		CCPointMake((155-65)*W_unit,(1710+156-564)*H_unit),
		CCPointMake((155-65)*W_unit,(1710+156-700)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1084)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1134)*H_unit),
		CCPointMake((425-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((705-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((860-65)*W_unit,(1710+156-1422)*H_unit)		
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
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
	pSpriteBK->setAnchorPoint(CCPointMake(0,1));
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	

	m_BtnArray[CLEJOBPG_AUTOHELP]->addObject(pSpriteBK);	
	for(int i=0;i<6;i++) //按鈕
	{
		CCSprite *pSprite=CCSprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(CCPointMake(0,1));
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	CCLabelTTF* pLabel = CCLabelTTF::create(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(pSpriteBK->getContentSize().width,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<8;i++) //字串
	{
		if(i<3)
			pLabel = CCLabelTTF::create(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,CCSizeMake(pSpriteBK->getContentSize().width*0.8,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
		else
			pLabel = CCLabelTTF::create(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
		
		pLabel->setAnchorPoint(CCPointMake(0,1));
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
}

void UI_CLEANJOB::InitUI_SPIRAL()
{
	m_BtnArray[CLEJOBPG_SPIRAL] = CCArray::createWithCapacity(CLESPIRAL_SIZE); //按鈕存放
	m_BtnArray[CLEJOBPG_SPIRAL]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint IconSite_Help=CCPointMake(955*W_unit,winSize.height-(99-75)*H_unit);		
	CCPoint IconSite_ZoomOut=CCPointMake(782*W_unit,winSize.height-238*H_unit);		
	CCPoint IconSite_ZoomIn=CCPointMake(922*W_unit,winSize.height-238*H_unit);		
	CCPoint LabSite_ZoomRate=CCPointMake(550*W_unit,winSize.height-238*H_unit);
	CCPoint LabSite_Clean=CCPointMake(90*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Virual=CCPointMake(425*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Undo=CCPointMake(705*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Delete=CCPointMake(860*W_unit,winSize.height-1519*H_unit);
	CCPoint IconSite_WALL=CCPointMake(485*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_UNDO=CCPointMake(705*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_DELETE=CCPointMake(860*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_MAPBG=CCPointMake(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	pSpriteBK->setColor(COLOR_STRING_BLACK);
	m_BtnArray[CLEJOBPG_SPIRAL]->addObject(pSpriteBK);

	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprBtnIcon,CLESPIRAL_HELP);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSpriteAction,CLESPIRAL_TITAL);


	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	//ZOOM IN OUT
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_ZOOMOUT);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_ZOOMIN);
	//縮放倍率
	pLabel = CCLabelTTF::create("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pLabel,CLESPIRAL_ZOOMLAB);
	//下方標籤
	pLabel = CCLabelTTF::create(STR_BTN_Spiral, S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(winSize.width*0.4,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);

	pLabel = CCLabelTTF::create(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11);

	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_WALL);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_UNDO);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_DELETE);
	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSpriteDownBtn,CLESPIRAL_GO);
	pLabel = CCLabelTTF::create(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 11);
	//機器人圖示
	pSprite=CCSprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_ROBOT);
	//地圖
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
	m_BtnArray[CLEJOBPG_SPIRAL]->insertObject(pSprite,CLESPIRAL_MAP);

}
void UI_CLEANJOB::InitUI_SPIRALHELP()
{
	m_BtnArray[CLEJOBPG_SPIRALHELP] = CCArray::create(); //按鈕存放
	m_BtnArray[CLEJOBPG_SPIRALHELP]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	CCPoint IconSite_BG=CCPointMake(65*W_unit,winSize.height-156*H_unit);		
	CCPoint IconSiteIN[6]=
	{CCPointMake((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,CCPointMake((587-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake((464-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake((485-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((705-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((860-65)*W_unit,(1710+156-1490)*H_unit)};

	CCPoint LabSiteIN[9]={
		CCPointMake(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		CCPointMake((155-65)*W_unit,(1710+156-564)*H_unit),
		CCPointMake((155-65)*W_unit,(1710+156-700)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1084)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1134)*H_unit),
		CCPointMake((425-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((705-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((860-65)*W_unit,(1710+156-1422)*H_unit)		
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
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
	pSpriteBK->setAnchorPoint(CCPointMake(0,1));
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	

	m_BtnArray[CLEJOBPG_SPIRALHELP]->addObject(pSpriteBK);	
	for(int i=0;i<6;i++) //按鈕
	{
		CCSprite *pSprite=CCSprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(CCPointMake(0,1));
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	CCLabelTTF* pLabel = CCLabelTTF::create(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(pSpriteBK->getContentSize().width,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<8;i++) //字串
	{
		if(i<3)
			pLabel = CCLabelTTF::create(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,CCSizeMake(pSpriteBK->getContentSize().width*0.8,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
		else
			pLabel = CCLabelTTF::create(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,CCSizeMake(pSpriteBK->getContentSize().width*0.5,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
		
		pLabel->setAnchorPoint(CCPointMake(0,1));
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
}

void UI_CLEANJOB::InitUI_WALL()
{
	m_BtnArray[CLEJOBPG_WALL] = CCArray::createWithCapacity(CLEWALL_SIZE); //按鈕存放
	m_BtnArray[CLEJOBPG_WALL]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint IconSite_Help=CCPointMake(955*W_unit,winSize.height-(99-75)*H_unit);		
	CCPoint IconSite_ZoomOut=CCPointMake(782*W_unit,winSize.height-238*H_unit);		
	CCPoint IconSite_ZoomIn=CCPointMake(922*W_unit,winSize.height-238*H_unit);		
	CCPoint LabSite_ZoomRate=CCPointMake(550*W_unit,winSize.height-238*H_unit);
	CCPoint LabSite_Clean=CCPointMake(90*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Virual=CCPointMake(425*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Undo=CCPointMake(705*W_unit,winSize.height-1519*H_unit);		
	CCPoint LabSite_Delete=CCPointMake(860*W_unit,winSize.height-1519*H_unit);
	CCPoint IconSite_WALL=CCPointMake(485*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_UNDO=CCPointMake(705*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_DELETE=CCPointMake(860*W_unit,winSize.height-1583*H_unit);		
	CCPoint IconSite_MAPBG=CCPointMake(winSize.width*0.5,winSize.height*0.5);		
	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	pSpriteBK->setColor(COLOR_STRING_BLACK);
	m_BtnArray[CLEJOBPG_WALL]->addObject(pSpriteBK);

	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_CLEAN_ICON_HELP); //HELP圖示
	pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));
	pSprBtnIcon->setPosition(IconSite_Help);	
	pSpriteBK->addChild(pSprBtnIcon,12);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprBtnIcon,CLEWALL_HELP);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSpriteAction,CLEWALL_TITAL);


	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_EditCleanJob, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	//ZOOM IN OUT
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_ZOOMOUT);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_ZOOMIN);
	//縮放倍率
	pLabel = CCLabelTTF::create("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pLabel,CLEWALL_ZOOMLAB);
	//下方標籤
	pLabel = CCLabelTTF::create(STR_BTN_Wallfollowing, S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(winSize.width*0.4,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSite_Clean);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_VirtualWall, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Virual);
	pSpriteBK->addChild(pLabel, 11);

	pLabel = CCLabelTTF::create(STR_Clean_Undo, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Undo);
	pSpriteBK->addChild(pLabel, 11);
	pLabel = CCLabelTTF::create(STR_Clean_Delete, S_FONE, *pSTR_CLEAN_LABSIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_Delete);
	pSpriteBK->addChild(pLabel, 11);
	//下方按鈕
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_WALL_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_WALL);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_WALL);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_UNDO_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_UNDO);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_UNDO);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_BTN_EDIT_DELETE_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_DELETE);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_DELETE);
	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn,10);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSpriteDownBtn,CLEWALL_GO);
	pLabel = CCLabelTTF::create(STR_Clean_CleanNow, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	pSpriteDownBtn->addChild(pLabel, 11);
	//機器人圖示
	pSprite=CCSprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,11);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_ROBOT);
	//地圖
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_MAP_BG); 
	pSprite->setAnchorPoint(CCPointMake(0.5,0.5));
	pSprite->setPosition(IconSite_MAPBG);
	pSpriteBK->addChild(pSprite,G_MAP_ZLEVEL);
	m_BtnArray[CLEJOBPG_WALL]->insertObject(pSprite,CLEWALL_MAP);

}
void UI_CLEANJOB::InitUI_WALLHELP()
{
	m_BtnArray[CLEJOBPG_WALLHELP] = CCArray::create(); //按鈕存放
	m_BtnArray[CLEJOBPG_WALLHELP]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	CCPoint IconSite_BG=CCPointMake(65*W_unit,winSize.height-156*H_unit);		
	CCPoint IconSiteIN[6]=
	{CCPointMake((425-65)*W_unit,(1710+156-158)*H_unit)		//425 1710 為底圖大小
	,CCPointMake((587-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake((464-65)*W_unit,(1710+156-872)*H_unit)
	,CCPointMake((485-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((705-65)*W_unit,(1710+156-1490)*H_unit)
	,CCPointMake((860-65)*W_unit,(1710+156-1490)*H_unit)};

	CCPoint LabSiteIN[9]={
		CCPointMake(0,(1710+156-412)*H_unit),		//425 1710 為底圖大小
		CCPointMake((155-65)*W_unit,(1710+156-564)*H_unit),
		CCPointMake((155-65)*W_unit,(1710+156-700)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1084)*H_unit),
		CCPointMake((540-65)*W_unit,(1710+156-1134)*H_unit),
		CCPointMake((425-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((705-65)*W_unit,(1710+156-1422)*H_unit),
		CCPointMake((860-65)*W_unit,(1710+156-1422)*H_unit)		
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
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_CLEAN_HELP_BG);
	pSpriteBK->setAnchorPoint(CCPointMake(0,1));
	pSpriteBK->setPosition(IconSite_BG);
	pSpriteBK->setColor(COLOR_STRING_BLACK2);
	pSpriteBK->setOpacity(255*0.8);
	addChild(pSpriteBK); //加入 layer 層	

	m_BtnArray[CLEJOBPG_WALLHELP]->addObject(pSpriteBK);	
	for(int i=0;i<6;i++) //按鈕
	{
		CCSprite *pSprite=CCSprite::createWithSpriteFrameName(pIconName[i]); 
		pSprite->setAnchorPoint(CCPointMake(0,1));
		pSprite->setPosition(IconSiteIN[i]);
		pSpriteBK->addChild(pSprite,10);
	}
	CCLabelTTF* pLabel = CCLabelTTF::create(pLabName[0], S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(pSpriteBK->getContentSize().width,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_WHITE);
	pLabel->setPosition(LabSiteIN[0]);
	pSpriteBK->addChild(pLabel, 10);

	for(int i=1;i<8;i++) //字串
	{
		if(i<3)
			pLabel = CCLabelTTF::create(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,CCSizeMake(pSpriteBK->getContentSize().width*0.8,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
		else
			pLabel = CCLabelTTF::create(pLabName[i], S_FONE, *pSTR_CLEAN_LABSIZE,CCSizeMake(pSpriteBK->getContentSize().width*0.5,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);		
		pLabel->setAnchorPoint(CCPointMake(0,1));
		pLabel->setColor(COLOR_STRING_GREEN);
		pLabel->setPosition(LabSiteIN[i]);
		pSpriteBK->addChild(pLabel, 10);
	}
	//底下按鈕狀
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteDownBtn->setScaleX(475.0f/492.0f);
	pSpriteDownBtn->setScaleY(0.5);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_Clean_Gotit, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.25));
	pSpriteBK->addChild(pLabel, 11);
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
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_MAIN]->objectAtIndex(CLEJOBMAIN_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_MAIN;
}
void UI_CLEANJOB::Hide_CLEANJOBMAIN()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_MAIN]->objectAtIndex(CLEJOBMAIN_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_CORNER()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_CORNER]->objectAtIndex(CLECORNER_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_CORNER;
	pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_CORNER]->objectAtIndex(CLECORNER_MAP);
	LoadGUIMAPData(pSprite);
	MAPRetunZero(JOBIDX_CORNER);
	MAPZoomLabUpdate(JOBIDX_CORNER);
}
void UI_CLEANJOB::Hide_CORNER()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_CORNER]->objectAtIndex(CLECORNER_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_CORNERHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_CORNERHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_CORNERHELP;
}
void UI_CLEANJOB::Hide_CORNERHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_CORNERHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_MULTI()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_MULTI]->objectAtIndex(CLEMULTI_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_MULTI;
	pSprite= (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEMULTI_MAP);	//MAP
	LoadGUIMAPData(pSprite);												//MAP
	MAPRetunZero(JOBIDX_MULTI);
	MAPZoomLabUpdate(JOBIDX_MULTI);

}
void UI_CLEANJOB::Hide_MULTI()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_MULTI]->objectAtIndex(CLEMULTI_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_MULTIHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_MULTIHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_MULTIHELP;
}
void UI_CLEANJOB::Hide_MULTIHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_MULTIHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}

void UI_CLEANJOB::Show_MOVETO()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_MOVETO]->objectAtIndex(CLEMOVETO_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_MOVETO;
	pSprite= (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEMOVETO_MAP);	
	LoadGUIMAPData(pSprite);												
	MAPRetunZero(JOBIDX_MOVETO);
	MAPZoomLabUpdate(JOBIDX_MOVETO);
}
void UI_CLEANJOB::Hide_MOVETO()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_MOVETO]->objectAtIndex(CLEMOVETO_BK);
	pSprite->setVisible(false);
	
}
void UI_CLEANJOB::Show_MOVETOHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_MOVETOHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_MOVETOHELP;
}
void UI_CLEANJOB::Hide_MOVETOHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_MOVETOHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}

void UI_CLEANJOB::Show_AUTO()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_AUTO]->objectAtIndex(CLEAUTO_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_AUTO;	
	pSprite= (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEAUTO_MAP);	
	LoadGUIMAPData(pSprite);			
	MAPRetunZero(JOBIDX_AUTO);
	MAPZoomLabUpdate(JOBIDX_AUTO);

}
void UI_CLEANJOB::Hide_AUTO()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_AUTO]->objectAtIndex(CLEAUTO_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_AUTOHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_AUTOHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_AUTOHELP;
}
void UI_CLEANJOB::Hide_AUTOHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_AUTOHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_SPIRAL()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_SPIRAL]->objectAtIndex(CLESPIRAL_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_SPIRAL;
	pSprite= (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLESPIRAL_MAP);	
	LoadGUIMAPData(pSprite);	
	MAPRetunZero(JOBIDX_SPIRAL);
	MAPZoomLabUpdate(JOBIDX_SPIRAL);
}
void UI_CLEANJOB::Hide_SPIRAL()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_SPIRAL]->objectAtIndex(CLESPIRAL_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_SPIRALHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_SPIRALHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_SPIRALHELP;
}
void UI_CLEANJOB::Hide_SPIRALHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_SPIRALHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_WALL()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_WALL]->objectAtIndex(CLEWALL_BK);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_WALL;
	pSprite= (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEWALL_MAP);	
	LoadGUIMAPData(pSprite);												
	MAPRetunZero(JOBIDX_WALL);
	MAPZoomLabUpdate(JOBIDX_WALL);
}
void UI_CLEANJOB::Hide_WALL()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_WALL]->objectAtIndex(CLEWALL_BK);
	pSprite->setVisible(false);
}
void UI_CLEANJOB::Show_WALLHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_WALLHELP]->objectAtIndex(0);
	pSprite->setVisible(true);
	PageType = CLEJOBPG_WALLHELP;
}
void UI_CLEANJOB::Hide_WALLHELP()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[CLEJOBPG_WALLHELP]->objectAtIndex(0);
	pSprite->setVisible(false);
}
//載入地圖
void UI_CLEANJOB::LoadGUIMAPData(CCSprite *pSpriteMap)
{
	if(!IsRobotMapReady()) //第一次一定要等待地圖更新
		WaitRobotMapUpdate();

	int iWidth=pSpriteMap->getContentSize().width;
	int iHeight=pSpriteMap->getContentSize().height;
	CCTexture2D *texture = new CCTexture2D();		
	bool val = texture->initWithData(GetGUIMAP(),kCCTexture2DPixelFormat_RGB888,GetGUIMAP_Width(),GetGUIMAP_Height(),CCSizeMake(GetGUIMAP_Width(),GetGUIMAP_Height()));
	pSpriteMap->setTexture(texture);
	pSpriteMap->setTextureRect(CCRectMake(0,0,GetGUIMAP_Width(),GetGUIMAP_Height()));
	pSpriteMap->setColor(ccGRAY);
	pSpriteMap->setAnchorPoint(CCPointMake(0.5,0.5));
	pSpriteMap->setPosition(CCPointMake(winSize.width*0.5,winSize.height*0.5));

	fZoomRate=MAPReSetSize(pSpriteMap,1.0f); // 強制會到原始大小
}
//設定地圖縮放
float UI_CLEANJOB::MAPReSetSize(CCSprite* pSprite ,float fSize) 
{
	if(fSize<=GUIMAPSize_MINRATE)
		fSize=GUIMAPSize_MINRATE;
	if(fSize>=GUIMAPSize_MAXRATE)
		fSize=GUIMAPSize_MAXRATE;

	CCPoint stCenter=pSprite->getPosition();
	float fOldZoomRate=pSprite->getScale();
	if(fSize==fOldZoomRate) 
		return fSize;
	pSprite->setScale(fSize);
	return fSize;
}
//回歸原點
void UI_CLEANJOB::MAPRetunZero(int iPagIdx)
{
	pose_t robot_pose={0.0,0.0,0.0};
	pose_t robot_mappose;
	robot_mappose = getRobotSiteByPose2d(robot_pose);
	float fWidthRate=winSize.width/GetGUIMAP_Width();
	float fHeightRate=winSize.height/GetGUIMAP_Height();
	CCSprite* pSpriteMAP = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPagIdx]);
	pSpriteMAP->setPosition(CCPoint(robot_mappose.px*fWidthRate,robot_mappose.py*fHeightRate));
	CCSprite* pSpriteROBOT = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	pSpriteROBOT->setPosition(CCPoint(robot_mappose.px*fWidthRate,robot_mappose.py*fHeightRate));
	
}
void UI_CLEANJOB::MAPZoomLabUpdate(int iPagIdx)
{
	CCLabelTTF* pZoomLab= (CCLabelTTF*)m_BtnArray[PageType]->objectAtIndex(ZOOMLAB_IDX[iPagIdx]);	
	char sTemp[12];
	sprintf_s(sTemp,12,"X %.1f",fZoomRate);
	pZoomLab->setString(sTemp);

}
void UI_CLEANJOB::MAPZoomIn(int iPagIdx)
{
	if(fZoomRate>=1.0)
		fZoomRate+=GUIMAPSize_UP_SPACE;
	else
		fZoomRate+=GUIMAPSize_DOWN_SPACE;
	CCSprite* pSpriteMap= (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPagIdx]);	
	fZoomRate=MAPReSetSize(pSpriteMap,fZoomRate);
	CCSprite* pSpriteROBOT = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	pSpriteROBOT->setPosition(pSpriteMap->getPosition());
	pSpriteROBOT->setScale(1);
	CCObject* pObj = nullptr;
	CCARRAY_FOREACH(pSpriteMap->getChildren(), pObj) //搜尋所有節點 改變大小
	{
		CCDrawNode* pNode=(CCDrawNode*) pObj;
		ResetContentSizeRang(pNode);		
	}
	MAPZoomLabUpdate(iPagIdx);
	//ROBOTICON_IDX
	//float fWidthRate=winSize.width/GetGUIMAP_Width();
	//float fHeightRate=winSize.height/GetGUIMAP_Height();
	//CCSprite* pSpriteROBOT = (CCSprite*)m_BtnArray[iPagIdx]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	//CCPoint RobotPoint=pSpriteROBOT->getPosition();
	//pSpriteROBOT->setPosition(CCPointMake(RobotPoint.x*fWidthRate,RobotPoint.y*fHeightRate));
}
void UI_CLEANJOB::MAPZoomOut(int iPagIdx)
{
	if(fZoomRate<1.1)
		fZoomRate-=GUIMAPSize_DOWN_SPACE;		
	else
		fZoomRate-=GUIMAPSize_UP_SPACE;
		
	CCSprite* pSpriteMap= (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPagIdx]);
	fZoomRate=MAPReSetSize(pSpriteMap,fZoomRate);
	CCSprite* pSpriteROBOT = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	pSpriteROBOT->setPosition(pSpriteMap->getPosition());
	pSpriteROBOT->setScale(1);

	CCObject* pObj = nullptr;
	CCARRAY_FOREACH(pSpriteMap->getChildren(), pObj) //搜尋所有節點 改變大小
	{
		CCDrawNode* pNode=(CCDrawNode*) pObj;
		ResetContentSizeRang(pNode);		
	}

	MAPZoomLabUpdate(iPagIdx);
	//float fWidthRate=winSize.width/GetGUIMAP_Width();
	//float fHeightRate=winSize.height/GetGUIMAP_Height();
	//CCSprite* pSpriteROBOT = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	//CCPoint RobotPoint=pSpriteROBOT->getPosition();
	//pSpriteROBOT->setPosition(CCPointMake(RobotPoint.x*fWidthRate,RobotPoint.y*fHeightRate));

}
//縮放回復到預設
void UI_CLEANJOB::MAPZoomDefault(int iPagIdx)
{
	CCSprite* pSpriteMap= (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPagIdx]);
	//地圖顯示位置
	pSpriteMap->setAnchorPoint(CCPoint(0.5,0.5));
	pSpriteMap->setPosition(CCPoint(GetGUIMAP_Width()*0.5,GetGUIMAP_Height()*0.5));
	fZoomRate=MAPReSetSize(pSpriteMap,GUIMAPSize_DEFRATE);
	//ROBOT 位置
	CCSprite* pSpriteRobot= (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	pSpriteRobot->setAnchorPoint(CCPoint(0.5,0.5));
	pSpriteRobot->setPosition(CCPoint(GetGUIMAP_Width()*0.5,GetGUIMAP_Height()*0.5));

}

//////////////////////////////////////////////////////////////////////////
void UI_CLEANJOB::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
void UI_CLEANJOB::TargetMove_ByMouse(CCSprite *pSprite,CCPoint MousePosition )
{
	CCPoint MapSite;
	MapSite.x=StartPoint_Target.x+(MousePosition.x-StartPoint_Mouse.x);//fZoomRate;
	MapSite.y=StartPoint_Target.y+(MousePosition.y-StartPoint_Mouse.y);//fZoomRate;
	pSprite->setPosition(MapSite);
	
}
void UI_CLEANJOB::TargetPointSave()
{
	CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEMOVETO_TARGET); 
	StartPoint_Target=pSprite ->getPosition();
}

void UI_CLEANJOB::TargetMove(CCPoint  nodePosition)
{
	CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEMOVETO_TARGET); 
	TargetMove_ByMouse(pSprite,nodePosition);
}
void UI_CLEANJOB::TargetSet(CCPoint  nodePosition)
{
	
	CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(CLEMOVETO_TARGET); 
	bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
	bMoveKeep=false;
	if(bTouch && pSprite ->isVisible())
	{		
		//int iPageIdx=GetPageIDxFromPageType();
		
		//CCSprite* pSpriteM = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
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
		CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
		CCDrawNode *pRang=(CCDrawNode *)pSprite->getChildByTag(GetMapRangTag());
		SelMapRang_DrawBlackBox(pRang);
		InitMapRangTag();
	}


}
 //去除點選紅框 改為黑框
void UI_CLEANJOB::SelMapRang_DrawBlackBox(CCDrawNode *pRang)
{	
	pMapRangInfo pRageInfo = (pMapRangInfo)pRang->getUserData();
	pRageInfo->bSel=false;
	pRageInfo->fX=pRang->getPositionX();
	pRageInfo->fY=pRang->getPositionY();
	DrawMapRang(pRang);	 //外框黑色		
}
void UI_CLEANJOB::SelMapRang_DrawRedBox(CCDrawNode *pRang)
{	
	pMapRangInfo pRageInfo = (pMapRangInfo)pRang->getUserData();
	pRageInfo->bSel=true;
	StartPoint_RangWidth=pRageInfo->iWidth;
	StartPoint_RangHeight=pRageInfo->iHeight;
	DrawMapRang(pRang);			

}
void UI_CLEANJOB::MouseDown_Function(CCPoint  nodePosition)
{
	//判斷那一個元件
	bMoveKeep=false;
	iChangRangFlag=0;
	int iPageIdx=GetPageIDxFromPageType();
	for (int i = STARTITEM_IDX[iPageIdx]; i < ENDITEM_IDX[iPageIdx]; i++) //檢查所有已知的元件 如果是動態增加的原件 會歸類到地圖
	{  
		CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
		bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
		if(i< ROBOTICON_IDX[iPageIdx] && bTouch)
			break;

		if( (i==MAPBK_IDX[iPageIdx] ||  i==ROBOTICON_IDX[iPageIdx]) && bTouch)
		{//點選到地圖或是機台圖示

			bMoveKeep=true;
			CCSprite* pSpriteM = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
			StartPoint_Map=pSpriteM->getPosition();
			StartPoint_Mouse=nodePosition;

			int iReturnValue=SelMapRang(nodePosition); //判斷是否有點選到RANG 或是編輯按鈕
			if(iReturnValue >= G_RandMaxSize && IsMapRangTagReady())
			{	//點選到編輯按鈕
				iRangBtnType=iReturnValue-G_RandMaxSize; //判別點選到那一個按鍵
				CCDrawNode *pRang=(CCDrawNode *)pSpriteM->getChildByTag(GetMapRangTag());
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
						CCDrawNode *pRang=(CCDrawNode *)pSpriteM->getChildByTag(GetMapRangTag());
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
bool UI_CLEANJOB::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	if(!isVisible()) //有顯示才繼續
		return false;

	switch(PageType)
	{
	default:
		break;
	case CLEJOBPG_MAIN:
		{
			//判斷那一個元件
			for (int i = CLEJOBMAIN_CORNER; i <= CLEJOBMAIN_WALL; i+=CLEJOBMAIN_BTNSPACENUM)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				BtnColorSetting(i,bTouch);
				InitPageArge();
			}  
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
void UI_CLEANJOB::MapMove(CCPoint MousePosition )
{
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCPoint MapSite;
	MapSite.x=StartPoint_Map.x+(MousePosition.x-StartPoint_Mouse.x);
	MapSite.y=StartPoint_Map.y+(MousePosition.y-StartPoint_Mouse.y);
	pSprite->setPosition(MapSite);
	pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPageIdx]); //ROBOT
	pSprite ->setPosition(MapSite);
	if(PageType==CLEJOBPG_MOVETO)
	{ //移動圖釘
		TargetMove(MousePosition);		
	}
}

void UI_CLEANJOB::MapRangMove(CCPoint MousePosition )
{
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCDrawNode* pRangNode=(CCDrawNode*)pSprite->getChildByTag(GetMapRangTag());
	if(pRangNode==NULL)
		return;
	CCPoint MapSite;
	MapSite.x=StartPoint_Rang.x+(MousePosition.x-StartPoint_Mouse.x)/fZoomRate;
	MapSite.y=StartPoint_Rang.y+(MousePosition.y-StartPoint_Mouse.y)/fZoomRate;
	pRangNode->setPosition(MapSite);
}
void UI_CLEANJOB::MapRangReRoation(CCPoint MousePosition )
{
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCDrawNode* pRangNode=(CCDrawNode*)pSprite->getChildByTag(GetMapRangTag());
	//pMapRangInfo pInfo=(pMapRangInfo)pRangNode->getUserData();
	CCPoint MapSite,RangSite=pRangNode->getPosition();
	MapSite.x=StartPoint_Rang.x+(MousePosition.x-StartPoint_Mouse.x)/fZoomRate;
	MapSite.y=StartPoint_Rang.y+(MousePosition.y-StartPoint_Mouse.y)/fZoomRate;
	float fDx=MapSite.x-RangSite.x;
	float fDy=MapSite.y-RangSite.y;
	//float fRoation=atan2(-fDy,fDx);
	float fRoation=atan2(fDy,fDx);
	float fAngel=fRoation/M_PI*180;

	pRangNode->setRotation(-fAngel);
}
void UI_CLEANJOB::MapRangReSize_H(CCPoint MousePosition )
{
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCDrawNode* pRangNode=(CCDrawNode*)pSprite->getChildByTag(GetMapRangTag());
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
void UI_CLEANJOB::MapRangReSize_V(CCPoint MousePosition )
{	
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCDrawNode* pRangNode=(CCDrawNode*)pSprite->getChildByTag(GetMapRangTag());
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
	
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCDrawNode* pRangNode=(CCDrawNode*)pSprite->getChildByTag(GetMapRangTag());
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

}

void UI_CLEANJOB::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
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
		CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
		CCDrawNode* pRangNode=(CCDrawNode*)pSprite->getChildByTag(GetMapRangTag());				
		pMapRangInfo pInfo=(pMapRangInfo)pRangNode->getUserData();	

		//內層座標移動至外層座標左下角，方便日後可以選取
		CCDrawNode* pDrawPen=(CCDrawNode*)pRangNode->getChildByTag(G_DRAWPENTAG);								
		float fNewX=pRangNode->getPositionX()+pInfo->fEdgeLeft;
		float fNewY=pRangNode->getPositionY()+pInfo->fEdgeDown;
		CCPoint OldSite=pDrawPen->getPosition();
		pDrawPen->setPosition(CCPointMake(-pInfo->fEdgeLeft,-pInfo->fEdgeDown)); //因為起始點為00 所以直接減
		//pRangNode->setPosition(CCPointMake(fNewX,fNewY));
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

void UI_CLEANJOB::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	switch(PageType)
	{
	default:
		break;
	case CLEJOBPG_WALLHELP:
		{
			CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(0); 
			bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
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
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch && pSprite->isVisible())
				{
					Function_WALL(i);
					break;
				}
			}  
			MAPRangItemCheck(); 
		}
		break;

	case CLEJOBPG_SPIRALHELP:
		{
			CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(0); 
			bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
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
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch && pSprite->isVisible())
				{
					Function_SPIRAL(i);
					break;
				}
			}  
			MAPRangItemCheck(); 
		}
		break;
	case CLEJOBPG_AUTOHELP:
		{
			CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(0); 
			bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
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
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch && pSprite->isVisible())
				{
					Function_AUTO(i);
					break;
				}
			}  
			MAPRangItemCheck(); 
		}
		break;

	case CLEJOBPG_MOVETOHELP:
		{
			CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(0); 
			bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
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
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch && pSprite->isVisible())
				{
					Function_MOVETO(i);
					break;
				}
			}  
			MAPRangItemCheck(); 
		}
		break;
	case CLEJOBPG_MULTIHELP:
		{
			CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(0); 
			bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
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
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch && pSprite->isVisible())
				{
					Function_MULTI(i);
					break;
				}
			}  
			MAPRangItemCheck(); //MAP
		}
		break;


	case CLEJOBPG_CORNERHELP:
		{
			CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(0); 
			bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
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
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				CCPoint stTempPoint=pSprite ->getPosition();
				if(bTouch && pSprite->isVisible())
				{
					Function_CORNER(i);
					break;
				}
			}  
			MAPRangItemCheck(); 
		}
		break;

	case CLEJOBPG_MAIN:
		{
			//判斷那一個元件
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch && pSprite->isVisible())
				{
					Function_CLEANJOBMAIN(i);
					break;
				}
			}  
		}
		break;
	}
}
//按鈕變色
const ccColor3B BtnCleanJob[6]=
{
	ccc3(128,93,152),
	ccc3(152,213,233),
	ccc3(113,205,190),
	ccc3(201,222,113),
	ccc3(248,206,100),
	ccc3(247,137,120),
};
void UI_CLEANJOB::BtnColorSetting(int iBtnIdx,bool bSel)
{
	if(bSel)
	{
		CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(iBtnIdx); 
		int iSel=(iBtnIdx-CLEJOBMAIN_CORNER)/2;
		pSprite ->setColor(BtnCleanJob[iSel]);
		CCLabelTTF* pLabel= (CCLabelTTF*)m_BtnArray[PageType]->objectAtIndex(iBtnIdx+1); 
		pLabel->setColor(COLOR_STRING_WHITE);

	}
	else
	{
		CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(iBtnIdx); 
		pSprite ->setColor(COLOR_STRING_ORIGIN);
		CCLabelTTF* pLabel= (CCLabelTTF*)m_BtnArray[PageType]->objectAtIndex(iBtnIdx+1); 
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
		RobotGo(PLAYER_DISPATCH_AI_CORNER);
		ChangLayer_Main();
		break;
	}

	if(iRangBtnType==MRBTN_PEN)
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[CLEJOBPG_CORNER]->objectAtIndex(CLECORNER_WALL);
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[CLEJOBPG_CORNER]->objectAtIndex(CLECORNER_WALL);
		pSprite ->setColor(COLOR_STRING_WHITE);
	}
}
 //統計現在MAPRang範圍
void UI_CLEANJOB::MapRangToArray()
{
	int iPageIdx=GetPageIDxFromPageType(); 
	CCSprite* pSpriteMap = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCPoint PointMap=pSpriteMap->getPosition();
	if(pSpriteMap->getChildrenCount()==0)
		return;
	int iHeight=pSpriteMap->getContentSize().height;
	int iWidth=pSpriteMap->getContentSize().width;
	CCRenderTexture* pTexture_Rang= CCRenderTexture::create( iWidth,iHeight, CCTexture2DPixelFormat::kCCTexture2DPixelFormat_RGBA8888);
	CCObject* pObj = nullptr;
	pMapRangInfo pInfo;
	CCSprite* pNewMap=CCSprite::create();	
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
				if(pInfo->iType==MRType_Circle ||pInfo->iType==MRType_Square)
				{
					CCDrawNode *pNewRand=CCDrawNode::create();
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
#ifndef ROBOT_DISABLE
	  MapRangToRobot();
#endif
	//DEBUG 儲存RANG 畫面
	pTexture_Rang->saveToFile("MapRang.png",kCCImageFormatPNG);  //存檔
}

//統計現在MAPRang範圍
void UI_CLEANJOB::MapPenToArray()
{
	int iPageIdx=GetPageIDxFromPageType(); 
	CCSprite* pSpriteMap = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCPoint PointMap=pSpriteMap->getPosition();
	if(pSpriteMap->getChildrenCount()==0)
		return;
	int iHeight=pSpriteMap->getContentSize().height;
	int iWidth=pSpriteMap->getContentSize().width;
	CCRenderTexture* pTexture_Pen= CCRenderTexture::create( iWidth,iHeight, CCTexture2DPixelFormat::kCCTexture2DPixelFormat_RGBA8888);
	CCObject* pObj = nullptr;
	pMapRangInfo pInfo;
	CCSprite* pNewMap=CCSprite::create();	
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
						pDrawPen->setPosition(CCPointMake(pInfo->fX-pInfo->fEdgeLeft,pInfo->fY-pInfo->fEdgeDown));
						pDrawPen->visit();//將現在畫面記錄到pTexture
						pDrawPen->setPosition(CCPointMake(-pInfo->fEdgeLeft,-pInfo->fEdgeDown));
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
#ifndef ROBOT_DISABLE
	MapPenToRobot();
#endif
	//DEBUG 儲存RANG 畫面
	pTexture_Pen->saveToFile("MapRangPen.png",kCCImageFormatPNG);  //存檔
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

		RobotGo(PLAYER_DISPATCH_AI_AUTO);
		ChangLayer_Main();
		break;
//MAP-----------------------------------------------
	}
	if(iRangBtnType==MRBTN_PEN)
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[CLEJOBPG_MULTI]->objectAtIndex(CLEMULTI_WALL);
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[CLEJOBPG_MULTI]->objectAtIndex(CLEMULTI_WALL);
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
	if(iRangBtnType==MRBTN_PEN)
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[CLEJOBPG_MOVETO]->objectAtIndex(CLEMOVETO_WALL);
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[CLEJOBPG_MOVETO]->objectAtIndex(CLEMOVETO_WALL);
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

		RobotGo(PLAYER_DISPATCH_AI_AUTO);
		ChangLayer_Main();
		break;
	}
	if(iRangBtnType==MRBTN_PEN)
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[CLEJOBPG_AUTO]->objectAtIndex(CLEAUTO_WALL);
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[CLEJOBPG_AUTO]->objectAtIndex(CLEAUTO_WALL);
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

		RobotGo(PLAYER_DISPATCH_AI_SPIRAL_LARGE);
		ChangLayer_Main();
		break;

	}
	if(iRangBtnType==MRBTN_PEN)
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[CLEJOBPG_SPIRAL]->objectAtIndex(CLESPIRAL_WALL);
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[CLEJOBPG_SPIRAL]->objectAtIndex(CLESPIRAL_WALL);
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

		RobotGo(PLAYER_DISPATCH_AI_WALL_FOLLOW);
		ChangLayer_Main();
		break;


	}
	if(iRangBtnType==MRBTN_PEN)
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[PageType]->objectAtIndex(CLEWALL_WALL);
		pSprite ->setColor(COLOR_STRING_GREEN);
	}
	else
	{
		CCSprite *pSprite = (CCSprite *)m_BtnArray[PageType]->objectAtIndex(CLEWALL_WALL);
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
	memset(&stRangInfo[iIdx],0,sizeof(MapRangInfo));
	stRangInfo[iIdx].iBefIdx=-1;
	stRangInfo[iIdx].ID=iIdx;
}
//取得新的紀錄空間
int UI_CLEANJOB::GetNewRangSpace()
{
	int iIdx=-1;
	for(int i=0;i<G_RandMaxSize;i++)
		if(stRangInfo[i].bUse==false)
		{
			iIdx=i;
			break;
		}
	return iIdx;
}
//刪除所有記錄
void UI_CLEANJOB::DelRangSpace(int iIdx)
{

	int iBefIdx=0;
	do
	{
		iBefIdx=stRangInfo[iIdx].iBefIdx;
		InitOneRangInfo(iIdx);
		iIdx=iBefIdx; //刪除之前所有UNDO記錄
	}while(iBefIdx>=0 && iBefIdx<G_RandMaxSize);
}
// 紀錄圖框相關訊息 
int UI_CLEANJOB::SaveNewRangInfo(int iWidth,int iHeight,int iAngle,CCPoint stPoint,int iType)
{
	int iIDx=GetNewRangSpace();
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
int  UI_CLEANJOB::GetMAXMAPZord(CCSprite *pMapSprite)
{
	int iMaxZord=0;
	CCObject* pObj = nullptr;
	CCARRAY_FOREACH(pMapSprite->getChildren(), pObj) //搜尋所有截點
	{
		CCDrawNode* pSprite=(CCDrawNode*) pObj;
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
	CCSprite* pMapSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCDrawNode *pRang = CCDrawNode::create();
	//CCSprite* pSpriteROBOT = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(ROBOTICON_IDX[iPagIdx]);
	//CCPoint stCen=pSpriteROBOT->getPosition(); //取得機器人位置
	//取得現在最大Zord
	iMaxZord=GetMAXMAPZord(pMapSprite);
	iMaxZord++;
	CCSize MapSize=pMapSprite->getContentSize();	//顯示在固定位置!!日後再做修正
	CCPoint stCen=pMapSprite->getPosition();
	
	//CCSize MapSize=pMapSprite->boundingBox().size;
	//CCPoint stCen=pMapSprite->boundingBox().origin;
	//MapSize=pMapSprite->boundingBox().size;
	//stCen=pMapSprite->boundingBox().origin;
	int iPolySize=50;
	CCPoint DrawSite=CCPointMake(MapSize.width*0.5+iPolySize-stCen.x, MapSize.height*0.7-stCen.y-iPolySize);
	
	//長 寬 角度 位置 種類	
	int iRangTag=SaveNewRangInfo(iPolySize*2,iPolySize*2,0,DrawSite,MRType_Circle);
	if(iRangTag!=-1)
	{
		int iWidth=iPolySize*2;
		pMapSprite->addChild(pRang,iMaxZord,iRangTag);	
		pRang->setUserData(&stRangInfo[iRangTag]); //記錄資訊
		//增加三個拉框
		CCSprite* pBtnSprite = CCSprite::createWithSpriteFrameName(S_CLEAN_RANG_ROTATE_DEF);		
#ifdef NO_ELLIPSE
#else
		pBtnSprite->setRotationX(45);
		pBtnSprite->setPosition(CCPointMake(0,0));
		pRang->addChild(pBtnSprite,10,MRBTN_ROTATE);

		pBtnSprite = CCSprite::createWithSpriteFrameName(S_CLEAN_RANG_HORIZONTAL_DEF);
		pRang->addChild(pBtnSprite,10,MRBTN_HORIZONTAL);
#endif

		pBtnSprite = CCSprite::createWithSpriteFrameName(S_CLEAN_RANG_VERTICAL_DEF);
		pRang->addChild(pBtnSprite,10,MRBTN_VERTICAL);
		DrawMapRang(pRang);

	}


	////建立節點範圍
	//pRang->setContentSize( CCSizeMake(iPolySize*2, iPolySize*2) );	
	//pRang->setPosition(DrawSite);
	//
	//// 繪製圓區域
	//pRang->drawDot(CCPointMake(iPolySize,iPolySize), iPolySize*0.8, ccc4FFromccc3B(ccc3(255,115,115)));
	//// Draw polygons	
	//CCPoint points[] = { CCPoint(0,iPolySize*2),
	//					 CCPoint(iPolySize*2,iPolySize*2), 
	//					 CCPoint(iPolySize*2,0),
	//					CCPoint(0,0)};
	//pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), ccc4f(0.9,0.9,0.9,0.2), 2, ccc4f(0,0,0,0.6f)); //位置來源,點數.塗滿顏色,外框線,外框顏色	


}

void UI_CLEANJOB::AddMAPRang_RECL()
{
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pMapSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCDrawNode *pRang = CCDrawNode::create();
	//取得現在最大Zord
	iMaxZord=GetMAXMAPZord(pMapSprite);
	iMaxZord++;
	CCSize MapSize=pMapSprite->getContentSize();
	CCPoint stCen=pMapSprite->getPosition();
	int iPolySize=50;
	//長 寬 角度 位置 種類
	CCPoint DrawSite=CCPointMake(MapSize.width*0.5+iPolySize-stCen.x, MapSize.height*0.7-stCen.y-iPolySize);


	int iRangTag=SaveNewRangInfo(iPolySize*2,iPolySize*2,0,DrawSite,MRType_Square);
	if(iRangTag!=-1)
	{
		int iWidth=iPolySize*2;
		pMapSprite->addChild(pRang,iMaxZord,iRangTag);	
		pRang->setUserData(&stRangInfo[iRangTag]); //記錄資訊
		//增加三個拉框
		CCSprite* pBtnSprite = CCSprite::createWithSpriteFrameName(S_CLEAN_RANG_ROTATE_DEF);		
		pBtnSprite->setRotationX(45);
		pBtnSprite->setPosition(CCPointMake(0,0));
		pRang->addChild(pBtnSprite,10,MRBTN_ROTATE);

		pBtnSprite = CCSprite::createWithSpriteFrameName(S_CLEAN_RANG_HORIZONTAL_DEF);		
		pRang->addChild(pBtnSprite,10,MRBTN_HORIZONTAL);

		pBtnSprite = CCSprite::createWithSpriteFrameName(S_CLEAN_RANG_VERTICAL_DEF);		
		pRang->addChild(pBtnSprite,10,MRBTN_VERTICAL);

		DrawMapRang(pRang);

	}
}
// 增加畫筆(比較特別 內部還有另外一張圖 記錄 現在的VIRUAL WALL)
int UI_CLEANJOB::AddMAPRang_PEN(CCPoint MousePosition)
{	
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pMapSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCDrawNode *pRang = CCDrawNode::create();	
	iMaxZord=GetMAXMAPZord(pMapSprite);
	iMaxZord++;
	CCSize MapSize=pMapSprite->boundingBox().size;	//pMapSprite->getContentSize();
	CCPoint stCen=pMapSprite->boundingBox().origin;
	CCPoint MouseOnMap=CCPointMake((MousePosition.x-stCen.x)/fZoomRate,(MousePosition.y-stCen.y)/fZoomRate);
	pRang->setPosition(MouseOnMap);
	CCPoint MouseOnRang=CCPointMake(pRang->getPositionX(),pRang->getPositionY()); //轉換成Rang 座標
	//長 寬 角度 位置 種類	
	int iRangTag=SaveNewRangInfo(G_PenEdgeSize,G_PenEdgeSize,0,MouseOnRang,MRType_Pen);
	if(iRangTag!=-1)
	{
		pMapSprite->addChild(pRang,iMaxZord,iRangTag);	
		pRang->setUserData(&stRangInfo[iRangTag]); //記錄資訊
		CCDrawNode *pDrawPen = CCDrawNode::create();
		pDrawPen->cleanup();
		pDrawPen->drawDot(CCPointZero, G_PenEdgeSize, RangEdgeColor_Black);//ccc4FFromccc3B(ccc3(255,0,0)));
		pRang->addChild(pDrawPen,0,G_DRAWPENTAG);
//		pRang->drawDot(CCPointZero, G_PenEdgeSize+10, ccc4FFromccc3B(ccc3(255,255,0))); //DEBUG 繪製外層起點
		//pRang->drawDot(CCPointZero, G_PenEdgeSize, ccc4FFromccc3B(ccc3(255,0,0)));
		//MinPenPoint=CCPointMake(-G_PenEdgeSize,-G_PenEdgeSize);
		//MaxPenPoint=CCPointMake(G_PenEdgeSize,G_PenEdgeSize);	
		//MinPenPoint=CCPointMake(MouseOnRang.x-G_PenEdgeSize,MouseOnRang.y-G_PenEdgeSize);
		//MaxPenPoint=CCPointMake(MouseOnRang.x+G_PenEdgeSize,MouseOnRang.y+G_PenEdgeSize);
	}
	return iRangTag;
}
void UI_CLEANJOB::DrawMapRangByPEN(CCPoint MousePosition)
{
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pMapSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 
	CCDrawNode *pRang=(CCDrawNode *)pMapSprite->getChildByTag(GetMapRangTag());
	pMapRangInfo pInfo=(pMapRangInfo) pRang->getUserData();
	CCSize MapSize=pMapSprite->boundingBox().size;	//pMapSprite->getContentSize();
	CCPoint stCen=pMapSprite->boundingBox().origin;
	CCPoint MouseOnMap=CCPointMake((MousePosition.x-stCen.x)/fZoomRate,(MousePosition.y-stCen.y)/fZoomRate);
	//CCSize MapSize=pMapSprite->getContentSize();
	//CCPoint MouseOnMap=CCPointMake(MousePosition.x+MapSize.width*0.5-StartPoint_Map.x,MousePosition.y+MapSize.height*0.5-StartPoint_Map.y);
	CCPoint MouseOnRang=CCPointMake(MouseOnMap.x-pRang->getPositionX(),MouseOnMap.y-pRang->getPositionY()); //轉換成Rang 座標
	CCDrawNode* pDrawPen=(CCDrawNode* )pRang->getChildByTag(G_DRAWPENTAG);
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
void UI_CLEANJOB::ResetContentSizeRang(CCDrawNode *pRang)
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
	pRang->setContentSize( CCSizeMake(pRangInfo->iWidth*fZoomRate+5, pRangInfo->iHeight*fZoomRate+5) );	
//	pRang->drawDot(CCPointZero, G_PenEdgeSize, ccc4FFromccc3B(ccc3(255,255,0))); //DEBUG 繪製外層起點
	//DEBUG 畫出圈選範圍
	//CCPoint points[] = { CCPoint(0,pRangInfo->iHeight),
	//	CCPoint(pRangInfo->iWidth,pRangInfo->iHeight), 
	//	CCPoint(pRangInfo->iWidth,0),
	//	CCPoint(0,0)};
	//pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), ccc4f(0.1,0.1,0.1,0.01), 2, ccc4f(1,0.5,0,1.0f)); //位置來源,點數.塗滿顏色,外框線,外框顏色	
}
 //繪製實際範圍
void UI_CLEANJOB::DrawMapRang_RealRangByType(CCDrawNode *pRang,pMapRangInfo pRangInfo)
{
	pRang->setPosition(CCPointMake(pRangInfo->fX,pRangInfo->fY));
	if(pRangInfo->iType==MRType_Circle)
	{	// 繪製圓區域 (橢圓需要自己重新包裝)
		pRang->drawDot(CCPointMake(pRangInfo->iWidth*0.5,pRangInfo->iHeight*0.5), pRangInfo->iWidth*0.5*0.8, ccc4f(1.0f,0.451f,0.451f,0.3));		
	}
	else if(pRangInfo->iType==MRType_Square)
	{	// 繪製矩形區域
		CCPoint points2[] = { CCPoint(pRangInfo->iWidth*0.1,pRangInfo->iHeight*0.9),
			CCPoint(pRangInfo->iWidth*0.9,pRangInfo->iHeight*0.9), 
			CCPoint(pRangInfo->iWidth*0.9,pRangInfo->iHeight*0.1),
			CCPoint(pRangInfo->iWidth*0.1,pRangInfo->iHeight*0.1)};

		pRang->drawPolygon(points2, sizeof(points2)/sizeof(points2[0]), ccc4f(1.0f,0.451f,0.451f,0.3), 2, ccc4f(1.0f,0.451f,0.451f,0.4)); //位置來源,點數.塗滿顏色,外框線,外框顏色

	}


}
void UI_CLEANJOB::DrawMapRang(CCDrawNode *pRang)
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
		CCDrawNode *pDrawPen=(CCDrawNode *)pRang->getChildByTag(G_DRAWPENTAG);
//		pDrawPen->drawDot(CCPointZero, G_PenEdgeSize+5, ccc4FFromccc3B(ccc3(255,100,0))); //DEBUG 繪製外層起點
//		pRang->drawDot(CCPointZero, G_PenEdgeSize+15, ccc4FFromccc3B(ccc3(255,255,0))); //DEBUG 繪製外層起點
		//建立節點範圍
		pRangInfo->iWidth=fabs(pRangInfo->fEdgeLeft-pRangInfo->fEdgeRight);
		pRangInfo->iHeight=fabs(pRangInfo->fEdgeTop-pRangInfo->fEdgeDown);
		pRang->setContentSize( CCSizeMake(pRangInfo->iWidth+5, pRangInfo->iHeight+5) );	 //建立點選範圍
		//pRang->setPosition(CCPointMake(pRangInfo->fX,pRangInfo->fY));
		//pDrawPen->drawDot(pDrawPen->getPosition(), G_PenEdgeSize+5, ccc4FFromccc3B(ccc3(255,100,0))); //DEBUG 繪製外層起點
//		pRang->drawDot(CCPointZero, G_PenEdgeSize+10, ccc4FFromccc3B(ccc3(255,255,100))); //DEBUG 繪製外層起點
		CCPoint points[] = { CCPoint(0,pRangInfo->iHeight),
			CCPoint(pRangInfo->iWidth,pRangInfo->iHeight), 
			CCPoint(pRangInfo->iWidth,0),
			CCPoint(0,0)};

		/*CCPoint points[] = { CCPoint(pRangInfo->fEdgeLeft,pRangInfo->fEdgeTop),
			CCPoint(pRangInfo->fEdgeRight,pRangInfo->fEdgeTop), 
			CCPoint(pRangInfo->fEdgeRight,pRangInfo->fEdgeDown),
			CCPoint(pRangInfo->fEdgeLeft,pRangInfo->fEdgeDown)};*/
		if(pRangInfo->bSel) //正在被點選
			pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), RangColor_FullRed, 2, RangColor_SelRed ); //位置來源,點數.塗滿顏色,外框線,外框顏色	
		else
			pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), RangColor_FullRed, 2, RangColor_EdgeRed); //位置來源,點數.塗滿顏色,外框線,外框顏色

	}
	else
	{
		
		pRang->setContentSize( CCSizeMake(pRangInfo->iWidth+5, pRangInfo->iHeight+5) );	//建立節點範圍
		//pRang->setPosition(CCPointMake(pRangInfo->fX,pRangInfo->fY));

		CCPoint points[] = { CCPoint(0,pRangInfo->iHeight),
			CCPoint(pRangInfo->iWidth,pRangInfo->iHeight), 
			CCPoint(pRangInfo->iWidth,0),
			CCPoint(0,0)};

		if(pRangInfo->bSel) //正在被點選
		{ //顯示編輯框
			pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), ccc4f(0.9,0.9,0.9,0.2), 2, ccc4f(1,0,0,0.6f)); //位置來源,點數.塗滿顏色,外框線,外框顏色	
			CCObject* pObj = nullptr;
			CCARRAY_FOREACH(pRang->getChildren(), pObj) //搜尋所有 圖框
			{
				CCSprite * pSprite=(CCSprite *)pObj;
				pSprite->setVisible(true);
				switch(pSprite->getTag())
				{
				case MRBTN_ROTATE:
					break;
				case MRBTN_HORIZONTAL:
					pSprite->setPosition(CCPointMake(pRangInfo->iWidth,pRangInfo->iHeight*0.5));
					break;
				case MRBTN_VERTICAL:
					pSprite->setPosition(CCPointMake(pRangInfo->iWidth*0.5,pRangInfo->iHeight));
					break;
				}
			}
		}
		else
		{
			pRang->drawPolygon(points, sizeof(points)/sizeof(points[0]), ccc4f(0.9,0.9,0.9,0.2), 2, ccc4f(0,0,0,0.6f)); //位置來源,點數.塗滿顏色,外框線,外框顏色	
			CCObject* pObj = nullptr;
			CCARRAY_FOREACH(pRang->getChildren(), pObj) //搜尋所有 圖框
			{
				CCSprite * pSprite=(CCSprite *)pObj;
				pSprite->setVisible(false);
			}

		}
	}


}

//判斷點選的位置是否是在範圍元件上
int  UI_CLEANJOB::SelMapRang(CCPoint MousePosition )
{
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pMapSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 	
	CCObject* pObj = nullptr;
	int iTag=-1;
	//CCSize MapSize=pMapSprite->getContentSize();
	//CCSize MapSize=pMapSprite->boundingBox().size; //實際畫布尺寸
	CCPoint MapSourcePoint=pMapSprite->boundingBox().origin; //實際畫布位置
	CCPoint MouseOnMap=CCPointMake((MousePosition.x-MapSourcePoint.x)/fZoomRate,(MousePosition.y-MapSourcePoint.y)/fZoomRate); //先計算滑鼠跟實際畫布貼圖距離，再將取得差距乘上倍率
	//CCPoint MouseOnMap=CCPointMake(MousePosition.x+MapSize.width*0.5-StartPoint_Map.x,MousePosition.y+MapSize.height*0.5-StartPoint_Map.y);
	//滑鼠位置 = 現在位置 + 底圖的起點 (地圖縮放的中心點-地圖寬度)
	int iZord=0;
	if(IsMapRangTagReady()) // 有點選範圍
	{
		CCDrawNode *pRang=(CCDrawNode *)pMapSprite->getChildByTag(GetMapRangTag());
		pMapRangInfo pInfo=NULL;
		if(pRang!=NULL)
			pInfo = (pMapRangInfo )pRang->getUserData();
		if(pInfo !=NULL && pInfo->bSel && pInfo->iType!=MRType_Pen) // 判斷是否有點選編輯按鈕
		{
			CCPoint MouseOnRang=CCPointMake(MouseOnMap.x-pRang->getPositionX(),MouseOnMap.y-pRang->getPositionY()); //轉換成Rang 座標
			//pRang->drawDot(MouseOnRang,10,ccc4f(1,0,1,1.0)); //確定位置
			float angel=pRang->getRotationX()*M_PI/180;
			CCPoint MouseOnRangAngel=CCPointMake(MouseOnRang.x*cos(angel)-MouseOnRang.y*sin(angel),MouseOnRang.x*sin(angel)+MouseOnRang.y*cos(angel)); //轉換成Rang 座標*角度
			//pRang->drawDot(MouseOnRangAngel,15,ccc4f(0,0,1,1.0)); //確定位置
			//iTag=G_RandMaxSize+MRBTN_NONE;
			CCARRAY_FOREACH(pRang->getChildren(), pObj) //搜尋所有截點
			{
				CCSprite *pSprite=(CCSprite *)pObj;
				CCPoint SpriteSite=pSprite->getPosition();
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
		//pRang->drawDot(MouseOnMap,10,ccc4f(1,0,1,1.0)); //繪製滑鼠定位座標
		//pMapSprite->addChild(pRang);

		CCARRAY_FOREACH(pMapSprite->getChildren(), pObj) //搜尋所有截點
		{
			CCDrawNode* pSprite=(CCDrawNode*) pObj;
			
			bool bTouch=pSprite->boundingBox().containsPoint(MouseOnMap);
			CCPoint NewPoint=pSprite->getPosition();

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
	int iTagIdx=GetMapRangTag();
	if(iTagIdx==-1)		return;	
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pMapSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 	
	CCDrawNode *pRang=(CCDrawNode *)pMapSprite->getChildByTag(iTagIdx);
	pMapRangInfo pInfo = (pMapRangInfo )pRang->getUserData();
	if(pInfo->bSel) //有被選中才刪除
	{
		DelRangSpace(pInfo->ID);//刪除空間記錄
//		pRang->removeAllChildren(); //理論上這一行不用 因為內部幫忙處理了
		pMapSprite->removeChild(pRang,true);
		InitMapRangTag();		
	}
}
//回到上一步指定元件
void  UI_CLEANJOB::UndoMapRang()
{
	int iTagIdx=GetMapRangTag();
	if(iTagIdx==-1)		return;	
	int iPageIdx=GetPageIDxFromPageType();
	CCSprite* pMapSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(MAPBK_IDX[iPageIdx]); 	
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
}

//啟動掃地工作
void  UI_CLEANJOB::RobotGo(int iScanType)
{
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
}

//移動到指定地點
void  UI_CLEANJOB::RobotGo_Target()
{
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
}
