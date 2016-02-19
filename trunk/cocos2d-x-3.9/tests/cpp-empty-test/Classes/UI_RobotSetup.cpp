#include <ImageList.h>
#include <HelloWorldScene.h>
#if PLAYERSTAGE
#include <libplayerc++/playerc++.h>
#endif

USING_NS_CC;
using namespace std;
extern HelloWorld* m_MainView;
extern UI_RobotMain* m_pUIRobotMain;
#if PLAYERSTAGE
/// Player Client
using namespace PlayerCc;
extern PlayerClient *pPlayerclient;
extern DispatcherProxy *pDisProxy;
#endif

UI_RobotSetup::UI_RobotSetup():
touchEnabled(false),
touchListener(nullptr),
PageType(SETUPPG_LAYER)
{
    for(int i=0;i<RESPG_SIZE;i++)
    {
        auto Page = new SpriteVector;
        m_SpriteVectors.push_back(Page);
        auto Page2 = new LabelVector;
        m_LabelVectors.push_back(Page2);
    }

}
UI_RobotSetup::~UI_RobotSetup()
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

bool UI_RobotSetup::init()
{  
	if (!Layer::init())
	{  
		return false;  
	}  
	InitUI();  
	return true;  
}  

void UI_RobotSetup::InitUI()
{
    winSize = Director::getInstance()->getVisibleSize();
    winOrigin = Director::getInstance()->getVisibleOrigin();
    InitUI_RobotSetupMAIN();
	InitUI_RobotSetupNotif();
}
void UI_RobotSetup::InitUI_RobotSetupMAIN()
{
    //    auto pageSprite = m_SpriteVectors.at(RESPG_MAIN);
    //    auto pageLabel = m_LabelVectors.at(RESPG_MAIN);
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point LabeSite_RobotModel=Vec2(45*W_unit,winSize.height-250*H_unit);
	Point LabeSite_RobotModel_Value=Vec2(winSize.width*0.5,winSize.height-250*H_unit);
	Point LabeSite_RobotVersion=Vec2(45*W_unit,winSize.height-345*H_unit);
	Point LabeSite_RobotVersion_Vaule=Vec2(winSize.width*0.5,winSize.height-345*H_unit);
	Point LabeSite_ApplicationVersion=Vec2(45*W_unit,winSize.height-440*H_unit);
	Point LabeSite_ApplicationVersion_Value=Vec2(winSize.width*0.5,winSize.height-440*H_unit);

	Point BtnSite_Name=Vec2(0*W_unit,winSize.height-540*H_unit);
	Point BtnSite_HomeAP=Vec2(0*W_unit,winSize.height-730*H_unit);
	Point BtnSite_SyncTime=Vec2(0*W_unit,winSize.height-(730+144*1)*H_unit);
	Point BtnSite_Notification=Vec2(0*W_unit,winSize.height-(730+144*2)*H_unit);
	Point BtnSite_Log=Vec2(0*W_unit,winSize.height-(730+144*3)*H_unit);
	Point BtnSite_Help=Vec2(0*W_unit,winSize.height-(1346)*H_unit);
	Point BtnSite_Legal=Vec2(0*W_unit,winSize.height-(1346+144)*H_unit);
	Point BtnSite_Disconnection=Vec2(0*W_unit,winSize.height-1674*H_unit);

	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
//	pSpriteBK->setAnchorPoint(Vec2::ZERO); //設定錨點
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中

	addChild(pSpriteBK); //加入 layer 層
    saveSprite(SETUPPG_MAIN, SETUPMAIN_BK, pSpriteBK);

//	m_BtnArray[SETUPPG_MAIN]->addObject(pSpriteBK);

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	auto pLabel = Label::createWithSystemFont(STR_TITAL_Setting, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	saveSprite(SETUPPG_MAIN, SETUPMAIN_TITAL, pSpriteAction);
//    m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_TITAL);
	//Robot F/W infornation
	//MODEL
    pLabel = Label::createWithSystemFont(S_RobotSetup_RobotModel, S_FONE, *pSTR_SETUP_SIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_RobotModel);
	pSpriteBK->addChild(pLabel,10);

	pLabel = Label::createWithSystemFont(S_RobotSetup_RobotModel, S_FONE, *pSTR_SETUP_SIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2(0,1));
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(LabeSite_RobotModel_Value);
	pSpriteBK->addChild(pLabel,10);
    saveLabel(SETUPPG_MAIN, SETUPMAIN_LAB_MODEL, pLabel);
//  m_BtnArray[SETUPPG_MAIN]->insertObject(pLabel,SETUPMAIN_MODEL);

	//VERSION
	pLabel = Label::createWithSystemFont(S_RobotSetup_RobotVersion, S_FONE, *pSTR_SETUP_SIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_RobotVersion);
	pSpriteBK->addChild(pLabel,10);

	pLabel = Label::createWithSystemFont(S_RobotSetup_RobotVersion, S_FONE, *pSTR_SETUP_SIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2(0,1));
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(LabeSite_RobotVersion_Vaule);
	pSpriteBK->addChild(pLabel,10);
    saveLabel(SETUPPG_MAIN, SETUPMAIN_LAB_VERSION, pLabel);
//	m_BtnArray[SETUPPG_MAIN]->insertObject(pLabel,SETUPMAIN_VERSION);

	//APP VERSION
	pLabel = Label::createWithSystemFont(S_RobotSetup_ApplicationVersion, S_FONE, *pSTR_SETUP_SIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_ApplicationVersion);
	pSpriteBK->addChild(pLabel,10);

	pLabel = Label::createWithSystemFont(S_RobotSetup_ApplicationVersion, S_FONE, *pSTR_SETUP_SIZE,Size::Size(winSize.width*0.5,0),TextHAlignment::LEFT);
	pLabel->setAnchorPoint(Vec2(0,1));
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(LabeSite_ApplicationVersion_Value);
	pSpriteBK->addChild(pLabel,10);
    saveLabel(SETUPPG_MAIN, SETUPMAIN_LAB_APPVER, pLabel);

//	m_BtnArray[SETUPPG_MAIN]->insertObject(pLabel,SETUPMAIN_APPVER);

	//按鈕設定 Name
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Name);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = Label::createWithSystemFont(S_RobotSetup_Name, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	pLabel = Label::createWithSystemFont(S_RobotSetup_Name, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(Vec2(pSpriteAction->getContentSize().width*0.5,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveLabel(SETUPPG_MAIN, SETUPMAIN_LAB_NAME, pLabel);

//	m_BtnArray[SETUPPG_MAIN]->insertObject(pLabel,SETUPMAIN_NAMELAB);
    saveLabel(SETUPPG_MAIN, SETUPMAIN_LAB_NAME, pLabel);
    saveSprite(SETUPPG_MAIN, SETUPMAIN_NAME, pSpriteAction);

//	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_NAME);
	//Home AP
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_HomeAP);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = Label::createWithSystemFont(S_RobotSetup_HomeAP, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(SETUPPG_MAIN, SETUPMAIN_HOMEAP, pSpriteAction);
//	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_HOMEAP);

	//Sync Time
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_SyncTime);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = Label::createWithSystemFont(S_RobotSetup_SyncTime, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(SETUPPG_MAIN, SETUPMAIN_SYNCTIME, pSpriteAction);
//	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_SYNCTIME);

	//Notification
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Notification);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = Label::createWithSystemFont(S_RobotSetup_Notification, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(SETUPPG_MAIN, SETUPMAIN_NOTIFICATION, pSpriteAction);
//	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_NOTIFICATION);

	//Log
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Log);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = Label::createWithSystemFont(S_RobotSetup_Log, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(SETUPPG_MAIN, SETUPMAIN_LOG, pSpriteAction);
//    m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_LOG);

	//HELP
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Help);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = Label::createWithSystemFont(S_RobotSetup_Help, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(SETUPPG_MAIN, SETUPMAIN_HELP, pSpriteAction);
    //m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_HELP);

	//Legal
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Legal);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = Label::createWithSystemFont(S_RobotSetup_Legal, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(SETUPPG_MAIN, SETUPMAIN_LEGAL, pSpriteAction);
//	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_LEGAL);

	//Disconnection
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Disconnection);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = Label::createWithSystemFont(S_RobotSetup_Disconnection, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(SETUPPG_MAIN, SETUPMAIN_DISCONNECTION, pSpriteAction);
//	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_DISCONNECTION);
    pSpriteBK->setVisible(false);
}

void UI_RobotSetup::InitUI_RobotSetupNotif()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	Point BtnSite_SyncTime=Vec2(0*W_unit,winSize.height-(10+144*1)*H_unit);
	Point BtnSite_CleanJob=Vec2(0*W_unit,winSize.height-(10+144*2)*H_unit);

	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中
	addChild(pSpriteBK); //加入 layer 層
    saveSprite(SETUPPG_NOTIF, SETUPNOTIF_BK, pSpriteBK);

//	m_BtnArray[SETUPPG_NOTIF]->addObject(pSpriteBK);

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	auto pLabel = Label::createWithSystemFont(S_RobotSetup_Notification, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(SETUPPG_NOTIF, SETUPNOTIF_TITAL, pSpriteAction);
//	m_BtnArray[SETUPPG_NOTIF]->insertObject(pSpriteAction,SETUPNOTIF_TITAL);

	//TIMESYNC BTN
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_SyncTime);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_RESERVATION_SWITH_OFF); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(winSize.width-pSprBtnIcon->getContentSize().width/2-20,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = Label::createWithSystemFont(S_RobotSetupNotif_TIPSYN, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(Vec2(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(SETUPPG_NOTIF, SETUPNOTIF_TIMESYNC_BTN, pSpriteAction);
//	m_BtnArray[SETUPPG_NOTIF]->insertObject(pSpriteAction,SETUPNOTIF_TIMESYNC_BTN);
    saveSprite(SETUPPG_NOTIF, SETUPNOTIF_TIMESYNC_CHECK, pSpriteAction);
//	m_BtnArray[SETUPPG_NOTIF]->insertObject(pSprBtnIcon,SETUPNOTIF_TIMESYNC_CHECK);

	//CleanJOB BTN
	pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_CleanJob);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_RESERVATION_SWITH_OFF); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(winSize.width-pSprBtnIcon->getContentSize().width/2-20,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = Label::createWithSystemFont(S_RobotSetupNotif_STOPINTO, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(Vec2(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(SETUPPG_NOTIF, SETUPNOTIF_CLEANJOB_BTN, pSpriteAction);
//	m_BtnArray[SETUPPG_NOTIF]->insertObject(pSpriteAction,SETUPNOTIF_CLEANJOB_BTN);
    saveSprite(SETUPPG_NOTIF, SETUPNOTIF_CLEANJOB_CHECK, pSpriteAction);
//	m_BtnArray[SETUPPG_NOTIF]->insertObject(pSprBtnIcon,SETUPNOTIF_CLEANJOB_CHECK);
    pSpriteBK->setVisible(false);
}


void UI_RobotSetup::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true);
	Show_RobotSetupMAIN();
}
void UI_RobotSetup::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();
}
void UI_RobotSetup::Hide_AllPage()
{
	Hide_RobotSetupMAIN();
	Hide_RobotSetupNOTIF();
	PageType = SETUPPG_LAYER;
}
void UI_RobotSetup::Show_RobotSetupMAIN()
{
    auto pSprite=getSpritePoint(SETUPPG_MAIN, SETUPMAIN_BK);
//    (CCSprite*)m_BtnArray[SETUPPG_MAIN]->objectAtIndex(SETUPMAIN_BK);
	pSprite->setVisible(true);
	ReleasePage_SetupMAIN();
	PageType = SETUPPG_MAIN;
}
void UI_RobotSetup::Hide_RobotSetupMAIN()
{
    auto pSprite=getSpritePoint(SETUPPG_MAIN, SETUPMAIN_BK);
	pSprite->setVisible(false);
}
void UI_RobotSetup::Show_RobotSetupNOTIF()
{
	auto pSprite= getSpritePoint(SETUPPG_NOTIF, SETUPNOTIF_BK);
    //(CCSprite*)m_BtnArray[SETUPPG_NOTIF]->objectAtIndex(SETUPMAIN_BK);
	pSprite->setVisible(true);
	PageType = SETUPPG_NOTIF;
}
void UI_RobotSetup::Hide_RobotSetupNOTIF()
{
	auto pSprite= getSpritePoint(SETUPPG_NOTIF, SETUPNOTIF_BK);
//    (CCSprite*)m_BtnArray[SETUPPG_NOTIF]->objectAtIndex(SETUPMAIN_BK);
	pSprite->setVisible(false);

}

void UI_RobotSetup::ReleasePage_SetupMAIN()
{
	//ROBOT NAME
	const char* pName=m_pUIRobotMain->GetRobotName();
    auto pLabel = getLabelPoint(SETUPPG_MAIN, SETUPMAIN_LAB_NAME);
    //(CCLabelTTF*)m_BtnArray[SETUPPG_MAIN]->objectAtIndex(SETUPMAIN_NAMELAB);
	pLabel->setString(pName);
	//APP VERSION
//    auto glviewData=GLView::shardOpenGlview();
//    auto sverionName=GLView::getVersionName();
//	pLabel = getLabelPoint(SETUPPG_MAIN, SETUPMAIN_LAB_APPVER);
//    //(CCLabelTTF*)m_BtnArray[SETUPPG_MAIN]->objectAtIndex(SETUPMAIN_APPVER);
//	pLabel->setString(pName);
}
void UI_RobotSetup::ReleasePage_Notifo()
{//根據設定改變按鈕圖示
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

    auto pSprite=getSpritePoint(SETUPPG_NOTIF, SETUPNOTIF_TIMESYNC_CHECK);
//    (CCSprite*)m_BtnArray[SETUPPG_NOTIF]->objectAtIndex(SETUPNOTIF_TIMESYNC_CHECK);
    
	if(CheckSyncTimeAlarmSetting())
	{		
		SpriteFrame *pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_RESERVATION_SWITH_ON);
		pSprite->setDisplayFrame(pFrame);
	}
	else
	{
		SpriteFrame *pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_RESERVATION_SWITH_OFF);
		pSprite->setDisplayFrame(pFrame);
	}

	pSprite=getSpritePoint(SETUPPG_NOTIF, SETUPNOTIF_CLEANJOB_CHECK);
//    (CCSprite*)m_BtnArray[SETUPPG_NOTIF]->objectAtIndex(SETUPNOTIF_CLEANJOB_CHECK);
	if(CheckCheckCJobAlarmSetting())
	{		
		SpriteFrame *pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_RESERVATION_SWITH_ON);
		pSprite->setDisplayFrame(pFrame);
	}
	else
	{
		SpriteFrame *pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_RESERVATION_SWITH_OFF);
		pSprite->setDisplayFrame(pFrame);
	}
#endif
}
//////////////////////////////////////////////////////////////////////////
//void UI_RobotSetup::registerWithTouchDispatcher()
//{
//	CCDirector* pDirector = CCDirector::sharedDirector();
//	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
//}
//滑鼠點下!判斷是那個元件
bool UI_RobotSetup::onTouchBegan(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr){ return true; }
    log("UI_RobotSetup:%s",__func__);
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );

	if(isVisible()) //有顯示才繼續
		return true;
	else
		return false;
}
void UI_RobotSetup::onTouchMoved(Touch* touch, Event* event)
{
	Point touchLocation = touch->getLocation();    
	Point nodePosition = convertToNodeSpace( touchLocation );
}
void UI_RobotSetup::onTouchEnded(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr){ return;}
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
	switch(PageType)
	{
	default:
		break;
	case SETUPPG_NOTIF:
		{
			//判斷那一個元件
            bool bTouch = false;
            auto sprites=getSpriteArray(SETUPPG_NOTIF);
            for(auto pSprite:*sprites)
            {
                auto btnIndex=pSprite->getTag();
                if(btnIndex!=0 && pSprite->isVisible())
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        Function_UINOTIF(btnIndex);
                        break;
                    }
                }

                
            }
//			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
//			{  
//				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
//				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
//				if(bTouch)
//				{
//					Function_UINOTIF(i);
//					break;
//				}
//			}  
		}
		break;
	case SETUPPG_MAIN:
		{
			//判斷那一個元件
            bool bTouch = false;
            auto sprites=getSpriteArray(SETUPPG_MAIN);
            for(auto pSprite:*sprites)
            {
                auto btnIndex=pSprite->getTag();
                if(btnIndex!=0 && pSprite->isVisible())
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        Function_UISETUP(btnIndex);
                        break;
                    }
                }
                
                
            }
//			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
//			{  
//				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
//				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
//				if(bTouch)
//				{
//					Function_UISETUP(i);
//					break;
//				}
//			}  
		}
		break;
	}
}
void UI_RobotSetup::MsgBtnCallback(Node *pNode)
{
	if(pNode->getTag()==1)
	{//CCMessageBox(STR_BTN_Ok,"CLOSE");
#if PLAYERSTAGE
		int result =  pDisProxy->LogOut("00:0c:29:f3:e8:a2");
		if(result>=0)
		{ //登出順利
			ChangLayer_Scanning();
		}
		else
		{ //登出失敗
			Toast* toast = Toast::create();
			toast->initToast(S_RobotSetup_Disconnection_Fail,2.5f);
			this->addChild(toast,12);

		}
#endif
	}
	else
	{//CCMessageBox(STR_BTN_Canel,"CLOSE");
		
		
	}
}
// 顯示警告視窗
void UI_RobotSetup::ShowDisconnectBox()
{
	PopupLayer* pl = PopupLayer::createWithSpriteFrameName(S_MSGBOX_BK);
	//CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//pl->setContentSize(Size::Size(winSize.width*0.7, winSize.height*0.4)); //須改為根據解析度變更
	//pl->setTitle("ERROR");
	pl->setContentText("Disconnect Robot...", *pSTR_DIALOG_SIZE, 60, 250);
	// 設置回調函數，回調傳回一個 CCNode 以獲取 tag 判斷點擊的按鈕
	// 這只是作為一種封裝實現，如果使用 delegate 那就能夠更靈活的控制參數了
	pl->setCallbackFunc(this, callfuncN_selector(UI_RobotSetup::MsgBtnCallback));
	pl->addButtonBySpriteFrame(S_MSGBOX_BTN_BK, S_MSGBOX_BTN_BK, STR_BTN_Ok, 1);
	pl->addButtonBySpriteFrame(S_MSGBOX_BTN_BK, S_MSGBOX_BTN_BK, STR_BTN_Canel, 0);
	// 添加到當前層
	this->addChild(pl);
}
void UI_RobotSetup::Function_UISETUP(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case SETUPMAIN_NOTIFICATION:
		Hide_RobotSetupMAIN();
		Show_RobotSetupNOTIF();
		ReleasePage_Notifo();
		break;
	case SETUPMAIN_TITAL:
		ChangLayer_Main();
		break;
	case SETUPMAIN_SYNCTIME:
		{ // 顯示提示視窗
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			SavePcTimeToRobotTime();
#endif
			Toast* toast = Toast::create();
			toast->initToast(S_RobotSetupMsg_SyncTime,2.5f);
			this->addChild(toast,12);

		}
		break;
	case SETUPMAIN_DISCONNECTION:
		//CCMessageBox("TEST ","CLOSE");
		{//彈出選擇視窗
			ShowDisconnectBox();
			//Toast* toast = Toast::create();
			//toast->initToast("Test",2.5f);
			//this->addChild(toast,12);

		}
		break;
	}
}

void UI_RobotSetup::Function_UINOTIF(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case SETUPNOTIF_TITAL: // 往返回上一頁
		Hide_AllPage();
		Show_RobotSetupMAIN();
		break;
	case SETUPNOTIF_CLEANJOB_BTN:
	case SETUPNOTIF_CLEANJOB_CHECK:
		{ //改變CLEANJOB 設定
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

			int iType=CheckCheckCJobAlarmSetting();
			if(iType==0)
				iType=1;
			else
				iType=0;
			SetCleanJobAlarmSettingONOFF(iType);
			SaveSetting_TipMsg();
#endif
			ReleasePage_Notifo();
		}
		break;
	case SETUPNOTIF_TIMESYNC_BTN:
	case SETUPNOTIF_TIMESYNC_CHECK:
		{ //改變TIME SYNC  設定
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)            
			int iType=CheckSyncTimeAlarmSetting();
			if(iType==0)
				iType=1;
			else
				iType=0;
			SetSyncTimeAlarmSettingONOFF(iType);
			SaveSetting_TipMsg();
#endif
			ReleasePage_Notifo();
		}
		break;
	}
}



void UI_RobotSetup::setTouchEnabled(bool enable)
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
        
        touchListener->onTouchBegan = CC_CALLBACK_2(UI_RobotSetup::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(UI_RobotSetup::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(UI_RobotSetup::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
    }
    else
    {
        _eventDispatcher->removeEventListener(touchListener);
        CC_SAFE_RELEASE_NULL(touchListener);
    }
}
bool UI_RobotSetup::isTouchEnabled() const
{
    return touchEnabled;
}
Sprite* UI_RobotSetup::getSpritePoint(int PageType,int Idx)
{
    return m_SpriteVectors.at(PageType)->at(m_itemIdx[PageType*100+Idx]);
}
Label* UI_RobotSetup::getLabelPoint(int PageType,int Idx)
{
    return m_LabelVectors.at(PageType)->at(m_itemIdx[PageType*100+Idx]);
}
void UI_RobotSetup::saveSprite(int PageType,int Idx,Sprite* pSprite)
{
    pSprite->setTag(Idx);
    m_itemIdx[PageType*100+Idx]=m_SpriteVectors.at(PageType)->size();
    m_SpriteVectors.at(PageType)->pushBack(pSprite);
}
void UI_RobotSetup::saveLabel(int PageType,int Idx,Label* pLabel)
{
    pLabel->setTag(Idx);
    m_itemIdx[PageType*100+Idx]=m_LabelVectors.at(PageType)->size();
    m_LabelVectors.at(PageType)->pushBack(pLabel);
    
}
SpriteVector* UI_RobotSetup::getSpriteArray(int pageType)
{
    return m_SpriteVectors.at(PageType);
}
LabelVector* UI_RobotSetup::getLabelArray(int pageType)
{
    return m_LabelVectors.at(PageType);
}
