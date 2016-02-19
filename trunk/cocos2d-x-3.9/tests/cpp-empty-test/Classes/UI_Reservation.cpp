#include <ImageList.h>
#include <HelloWorldScene.h>
#if PLAYERSTAGE
#include <libplayerc++/playerc++.h>
#endif
USING_NS_CC;
using namespace std;

Setting_Reservation ReserSetting;


extern HelloWorld* m_MainView;
#if PLAYERSTAGE
using namespace PlayerCc;
player_dispatcher_ui_alarm_time_t *pUiSchedule[RESPGALARM_SIZE];
player_dispatcher_ui_alarm_time_t stRobotShedule[G_ROBOT_SCHEDULE_MAX];
/// Player Client
extern PlayerClient *pPlayerclient;
extern DispatcherProxy *pDisProxy;
#endif

UI_Reservation::UI_Reservation():
touchEnabled(false),
touchListener(nullptr),
PageType(RESPG_LAYER),
iScheduleTotalNumber(-1),
iScheduleCount (0)
{
	memset((void*)&ReserSetting,0,sizeof(Setting_Reservation));

    for(int i=0;i<RESPG_SIZE;i++)
    {
        auto Page = new SpriteVector;
        m_SpriteVectors.push_back(Page);
        auto Page2 = new LabelVector;
        m_LabelVectors.push_back(Page2);
    }
    
#if PLAYERSTAGE
    memset(pUiSchedule,0,sizeof(pUiSchedule));
    memset(stRobotShedule,0,sizeof(player_dispatcher_ui_alarm_time_t)*G_ROBOT_SCHEDULE_MAX);
#endif

}
UI_Reservation::~UI_Reservation()
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
    
#if PLAYERSTAGE
	for(int i=0;i<iScheduleTotalNumber;i++)
	{
		if ( stRobotShedule[i].job.map_index_count!=0 )
			free(stRobotShedule[i].job.map_index);
		if ( stRobotShedule[i].job.data_count!=0  )
			free(stRobotShedule[i].job.data);
		if ( stRobotShedule[i].job.name_count!=0 )
			free(stRobotShedule[i].job.name);
	}
#endif
}

bool UI_Reservation::init()
{  
	if (!Layer::init())
	{  
		return false;  
	}  
	InitUI();  
	return true;  
}  

void UI_Reservation::InitUI()
{
    winSize = Director::getInstance()->getVisibleSize();
    winOrigin = Director::getInstance()->getVisibleOrigin();
    InitUI_RESMAIN();
	InitUI_RESSINGAL();
	InitUI_RESDAILY();
}
void UI_Reservation::InitUI_RESMAIN()
{
//    auto pageSprite = m_SpriteVectors.at(RESPG_MAIN);
//    auto pageLabel = m_LabelVectors.at(RESPG_MAIN);
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
    
//	m_BtnArray[RESPG_MAIN] = CCArray::createWithCapacity(RESMAIN_SIZE); //按鈕存放
//	m_BtnArray[RESPG_MAIN]->retain();
//	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	char sTempStr[20];
	//設定背景
//	auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
//	pSpriteBK->setAnchorPoint(Vec2::ZERO); //設定錨點
    pSpriteBK->setPosition(winOrigin+Vec2(winSize.width/2, winSize.height/2)); //圖檔置中

	addChild(pSpriteBK); //加入 layer 層
    pSpriteBK->setTag(RESMAIN_BK);
    saveSprite(RESPG_MAIN, RESMAIN_BK, pSpriteBK);
//    pageSprite->pushBack(pSpriteBK);
//    m_itemIdx[RESPG_MAIN*100+RESMAIN_BK]=pageSprite->size()-1;
//	m_BtnArray[RESPG_MAIN]->addObject(pSpriteBK);

	Point SpriteSite_Singal=Vec2(48*W_unit,winSize.height-244*H_unit);
	Point LabelSite_Singal=Vec2(368*W_unit,winSize.height-271*H_unit);
	Point LabelSite_Tital_OnBtn = Vec2(303*W_unit,(248-53)*H_unit);
	Point LabelSite_Help_OnBtn  = Vec2(303*W_unit,(248-150)*H_unit);
	Point LabelSite_Value_OnBtn = Vec2(712*W_unit,(248-150)*H_unit);

	Point SpriteSite_Daily=Vec2(48*W_unit,winSize.height-516*H_unit);
	
	

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	auto pLabel = Label::createWithSystemFont(STR_TITAL_Reservation, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
    pSpriteAction->addChild(pLabel, 10);
    saveSprite(RESPG_MAIN, RESMAIN_TITAL, pSpriteAction);
//	m_BtnArray[RESPG_MAIN]->insertObject(pSpriteAction,RESMAIN_TITAL);

	//2個工作JOB 增加 背景版
	auto pSprite=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP); //SINGAL圖示
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(SpriteSite_Singal);
	pSpriteBK->addChild(pSprite,10);
    saveSprite(RESPG_MAIN, RESMAIN_SINGAL, pSprite);

//	m_BtnArray[RESPG_MAIN]->insertObject(pSprite,RESMAIN_SINGAL);
    
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_RESERVATION_BTN_SINGAL); 
	pSprBtnIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprBtnIcon->setPosition(Vec2(0,pSprite->getContentSize().height));
	pSprite->addChild(pSprBtnIcon,10);
    
    pLabel = Label::createWithSystemFont(STR_BTN_SingleAppointment, S_FONE, *pSTR_CLEANJOB_SIZE,Size::Size(600*W_unit,144*H_unit),TextHAlignment::LEFT,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Tital_OnBtn);
	pSprite->addChild(pLabel,10);
    saveLabel(RESPG_MAIN, RESMAIN_LAB_SINGAL, pLabel);
 //	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABSINGAL);

    //下方說明文字
	pLabel = Label::createWithSystemFont(STR_Reservation_Robotwillstartcleaning, S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(600*W_unit,144*H_unit),TextHAlignment::LEFT,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Help_OnBtn);
	pSprite->addChild(pLabel,10);
    saveLabel(RESPG_MAIN, RESMAIN_LAB_SINGALHELP, pLabel);

//	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABSINGALHELP);
	//啟動時間
	
	sprintf(sTempStr,STR_Reservation_ValueTime,0,0);
	pLabel = Label::createWithSystemFont(sTempStr, S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(600*W_unit,144*H_unit),TextHAlignment::LEFT,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_RED);
	pLabel->setPosition(LabelSite_Value_OnBtn);
	pSprite->addChild(pLabel,10);
    saveLabel(RESPG_MAIN, RESMAIN_LAB_SINGALVALUE, pLabel);
//	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABSINGALVALUE);
	

	pSprite=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP); //DAILY圖示
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(SpriteSite_Daily);
	pSpriteBK->addChild(pSprite,10);
    saveSprite(RESPG_MAIN, RESMAIN_DAILY, pSprite);

//	m_BtnArray[RESPG_MAIN]->insertObject(pSprite,RESMAIN_DAILY);
    
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_RESERVATION_BTN_DAILY); 
	pSprBtnIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprBtnIcon->setPosition(Vec2(0,pSprite->getContentSize().height));
	pSprite->addChild(pSprBtnIcon,10);
	pLabel = Label::createWithSystemFont(STR_BTN_Dailycleaning, S_FONE, *pSTR_CLEANJOB_SIZE,Size::Size(600*W_unit,144*H_unit),TextHAlignment::LEFT,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Tital_OnBtn);
	pSprite->addChild(pLabel,10);
    saveLabel(RESPG_MAIN, RESMAIN_LAB_DAILY, pLabel);
//	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABDAILY);

	//下方說明文字
	pLabel = Label::createWithSystemFont(STR_Reservation_Robotwillstartcleaning, S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(600*W_unit,144*H_unit),TextHAlignment::LEFT,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Help_OnBtn);
	pSprite->addChild(pLabel,10);
    saveLabel(RESPG_MAIN, RESMAIN_LAB_DAILYHELP, pLabel);
//	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABDAILYHELP);
    
	//啟動時間
	pLabel = Label::createWithSystemFont(sTempStr, S_FONE, *pSTR_CLEAN_LABSIZE,Size::Size(600*W_unit,144*H_unit),TextHAlignment::LEFT,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_RED);
	pLabel->setPosition(LabelSite_Value_OnBtn);
	pSprite->addChild(pLabel,10);
    saveLabel(RESPG_MAIN, RESMAIN_LAB_DAILYVALUE, pLabel);
//	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABDAILYVALUE);
     pSpriteBK->setVisible(false);

}

void UI_Reservation::InitUI_RESSINGAL()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	
	//設定背景
    pSpriteBK->setPosition(winOrigin+Vec2(winSize/2)); //圖檔置中
	addChild(pSpriteBK); //加入 layer 層
    saveSprite(RESPG_SINGAL, RESSINGAL_BK, pSpriteBK);
//	m_BtnArray[RESPG_SINGAL]->addObject(pSpriteBK);

	Point SpriteSite_Singal=Vec2(0*W_unit,winSize.height-150*H_unit);
    Point SpriteSite_OnOff=Vec2(840*W_unit,(238-38)*H_unit);
	Point LabelSiteOnBtn_Lab1=Vec2(10*W_unit,(238-38)*H_unit);
	Point LabelSiteOnBtn_Lab2=Vec2(10*W_unit,(238-150)*H_unit);
	Point SpriteSite_SetTime=Vec2(0*W_unit,winSize.height-572*H_unit);	
	Point LabelSite_SetTime=Vec2(368*W_unit,winSize.height-541*H_unit);
	Point LabelSiteOnBtn_SetTime=Vec2(328*W_unit,(720-10)*H_unit);
	Point LabelSite_Lab1=Vec2(380*W_unit,winSize.height-1356*H_unit);
	Point LabelSite_Lab2=Vec2(880*W_unit,winSize.height-1356*H_unit);

	//上方標題列
	auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
    
	auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
    
	auto pLabel = Label::createWithSystemFont(STR_TITAL_Reservation, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
    saveSprite(RESPG_SINGAL, RESSINGAL_TITAL, pSpriteAction);
    //	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteAction,RESSINGAL_TITAL);

	//ON/OFF 背景
	auto pSprite=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP2); 
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(SpriteSite_Singal);
	pSprite->setColor(COLOR_STRING_WHITE);	
	pSpriteBK->addChild(pSprite,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_SWITCHBK, pSprite);
//	m_BtnArray[RESPG_SINGAL]->insertObject(pSprite,RESSINGAL_SWITCHBK);
//關閉圖示
	pSprBtnIcon=Sprite::createWithSpriteFrameName(S_RESERVATION_SWITH_OFF);
	pSprBtnIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprBtnIcon->setPosition(SpriteSite_OnOff);
	pSprBtnIcon->setVisible(true);	
	pSprite->addChild(pSprBtnIcon,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_SWITCHBTN, pSprBtnIcon);
//	m_BtnArray[RESPG_SINGAL]->insertObject(pSprBtnIcon,RESSINGAL_SWITCHBTN);

    
    pLabel = Label::createWithSystemFont(STR_BTN_SingleAppointment, S_FONE, *pSTR_STATE_SIZE,Size::Size(0,0));
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSiteOnBtn_Lab1);
	pSprite->addChild(pLabel,10);
    
	pLabel = Label::createWithSystemFont(STR_Reservation_Choosetime, S_FONE, *pSTR_RESERV_LABSIZE,Size::Size(0,0));
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_LIGHTBLACK);
	pLabel->setPosition(LabelSiteOnBtn_Lab2);
	pSprite->addChild(pLabel,10);

	

	//Set Start Time
	pSprite=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BK); //背景圖案
	pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pSprite->setPosition(SpriteSite_SetTime);
	pSprite->setColor(COLOR_STRING_LIGHTGREY);
	pSpriteBK->addChild(pSprite,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_TIMEBK, pSprite);
//	m_BtnArray[RESPG_SINGAL]->insertObject(pSprite,RESSINGAL_TIMEBK);
    
	pLabel = Label::createWithSystemFont(STR_Reservation_SetStartTime, S_FONE, *pSTR_CLEANJOB_SIZE,Size::Size(0,0));
	pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabelSiteOnBtn_SetTime);
	pSprite->addChild(pLabel,10);

	//設定按紐位置
	Size TimeSetSize	= pSprite->boundingBox().size;
	float W_TimeSetunit = TimeSetSize.width/8;
	float H_TimeSetunit = TimeSetSize.height/4;

	Point BtnSite_HourUP=Vec2(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit);
	Point BtnSite_HourNow=Vec2(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*2);
	Point BtnSite_HourDown=Vec2(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*3);
	Point BtnSite_MinUP=Vec2(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit);
	Point BtnSite_MinNow=Vec2(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*2);
	Point BtnSite_MinDown=Vec2(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*3);

	//時間設定按鈕
	auto pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_HourUP);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_TIMEHOUR_UP_BTN, pSprite);
//	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEHOUR_UP_BTN);
    
	pLabel = Label::createWithSystemFont("00", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_SINGAL, RESSINGAL_TIMEHOUR_UP_LAB_VALUE, pLabel);
//	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEHOUR_UP_VALUE);


	pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_HourNow);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_TIMEHOUR_NOW_BTN, pSpriteBtn);
//	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEHOUR_NOW_BTN);
    
	pLabel = Label::createWithSystemFont("01", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_SINGAL, RESSINGAL_TIMEHOUR_NOW_LAB_VALUE, pLabel);
//	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEHOUR_NOW_VALUE);


	pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_HourDown);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_TIMEHOUR_DOWN_BTN, pSpriteBtn);
//	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEHOUR_DOWN_BTN);
    
	pLabel = Label::createWithSystemFont("02", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_SINGAL, RESDAILY_TIMEHOUR_DOWN_LAB_VALUE, pLabel);
//	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEHOUR_DOWN_VALUE);


	pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_MinUP);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_TIMEMIN_UP_BTN, pSpriteBtn);

//	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEMIN_UP_BTN);
	pLabel = Label::createWithSystemFont("00", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_SINGAL, RESDAILY_TIMEMIN_UP_LAB_VALUE, pLabel);

//	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEMIN_UP_VALUE);


	pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_MinNow);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_TIMEMIN_NOW_BTN, pSpriteBtn);

//	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEMIN_NOW_BTN);
	pLabel = Label::createWithSystemFont("01", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_SINGAL, RESDAILY_TIMEMIN_NOW_LAB_VALUE, pLabel);

//	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEMIN_NOW_VALUE);


	pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_MinDown);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_TIMEMIN_DOWN_BTN, pSpriteBtn);

//	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEMIN_DOWN_BTN);
	pLabel = Label::createWithSystemFont("02", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_SINGAL, RESDAILY_TIMEMIN_DOWN_LAB_VALUE, pLabel);

//	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEMIN_DOWN_VALUE);

	//時間提示
	pLabel = Label::createWithSystemFont(STR_Reservation_Robotwillstartcleaning, S_FONE, *pSTR_CLEANJOB_SIZE,Size::Size(winSize.width,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2(0.5,0.5));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Lab1);
	pSpriteBK->addChild(pLabel,10);
    saveLabel(RESPG_SINGAL, RESDAILY_LAB_TIME, pLabel);

//	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_LABTIME);

	pLabel = Label::createWithSystemFont("last 09:00", S_FONE, *pSTR_CLEANJOB_SIZE,Size::Size(winSize.width,0),TextHAlignment::CENTER,TextVAlignment::TOP);
	pLabel->setAnchorPoint(Vec2(0.5,0.5));
	pLabel->setColor(COLOR_STRING_RED);
	pLabel->setPosition(LabelSite_Lab2);
	pSpriteBK->addChild(pLabel,10);
    saveLabel(RESPG_SINGAL, RESDAILY_LAB_VALUETIME, pLabel);

//	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_VALUETIME);


	//底下兩個按鈕
	//底部按鈕
	auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0,0));
	pSpriteDownBtn->setScaleX(0.5);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);

	pLabel = Label::createWithSystemFont(STR_BTN_Canel, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setScaleX(2);
	pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize()/2));
	pSpriteDownBtn->addChild(pLabel,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_BTNCANEL, pSpriteDownBtn);

//	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteDownBtn,RESSINGAL_BTNCANEL);

	pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
	pSpriteDownBtn->setPosition(Vec2(0+winSize.width*0.5,0)); //+4 FOR 中間線
	pSpriteDownBtn->setScaleX(0.5);
	pSpriteDownBtn->setColor(COLOR_STRING_GRAY);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = Label::createWithSystemFont(STR_BTN_Save, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setScaleX(2);
	pLabel->setPosition(pSpriteDownBtn->getContentSize()/2);
	pSpriteDownBtn->addChild(pLabel,10);
    saveSprite(RESPG_SINGAL, RESSINGAL_BTNSAVE, pSpriteDownBtn);
//	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteDownBtn,RESSINGAL_BTNSAVE);
    pSpriteBK->setVisible(false);

}

void UI_Reservation::InitUI_RESDAILY()
{
    auto pSpriteBK=Sprite::createWithSpriteFrameName(S_MAIN_BKIMG);
    winSize=pSpriteBK->getBoundingBox().size;   //改用底圖當作視窗範圍
    float W_unit=winSize.width/1080;
    float H_unit=winSize.height/1920;
    //設定背景
    pSpriteBK->setPosition(winOrigin+Vec2(winSize/2)); //圖檔置中
    addChild(pSpriteBK); //加入 layer 層
    saveSprite(RESPG_DAILY, RESDAILY_BK, pSpriteBK);
    //	m_BtnArray[RESPG_DAILY]->addObject(pSpriteBK);
    
    Point SpriteSite_Singal=Vec2(0*W_unit,winSize.height-150*H_unit);
    Point SpriteSite_OnOff=Vec2(840*W_unit,(238-38)*H_unit);
    Point LabelSiteOnBtn_Lab1=Vec2(10*W_unit,(238-38)*H_unit);
    Point LabelSiteOnBtn_Lab2=Vec2(10*W_unit,(238-150)*H_unit);
    Point SpriteSite_SetTime=Vec2(0*W_unit,winSize.height-572*H_unit);
    Point LabelSite_SetTime=Vec2(368*W_unit,winSize.height-541*H_unit);
    Point LabelSiteOnBtn_SetTime=Vec2(328*W_unit,(720-10)*H_unit);
    Point LabelSite_Lab1=Vec2(380*W_unit,winSize.height-1356*H_unit);
    Point LabelSite_Lab2=Vec2(880*W_unit,winSize.height-1356*H_unit);
    
    //上方標題列
    auto pSpriteAction=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteAction->setAnchorPoint(Vec2::ZERO); //設定錨點
    pSpriteAction->setPosition(Vec2(0,winSize.height-pSpriteAction->getContentSize().height));
    pSpriteBK->addChild(pSpriteAction);
    auto pSprBtnIcon=Sprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
    pSprBtnIcon->setPosition(Vec2(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
    pSpriteAction->addChild(pSprBtnIcon);
    auto pLabel = Label::createWithSystemFont(STR_TITAL_Reservation, S_FONE, *pSTR_TITAL_SIZE);
    pLabel->setColor(COLOR_STRING_TITAL);
    pLabel->setPosition(Vec2(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
    pSpriteAction->addChild(pLabel, 10);
    saveSprite(RESPG_DAILY, RESDAILY_TITAL, pSpriteAction);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteAction,RESDAILY_TITAL);
    
    //ON/OFF 背景
    auto pSprite=Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP2);
    pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSprite->setPosition(SpriteSite_Singal);
    pSprite->setColor(COLOR_STRING_WHITE);
    pSpriteBK->addChild(pSprite,10);
    saveSprite(RESPG_DAILY, RESDAILY_SWITCHBK, pSprite);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pSprite,RESDAILY_SWITCHBK);
    
    pSprBtnIcon=Sprite::createWithSpriteFrameName(S_RESERVATION_SWITH_OFF); //關閉圖示
    pSprBtnIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSprBtnIcon->setPosition(SpriteSite_OnOff);
    pSprBtnIcon->setVisible(true);
    pSprite->addChild(pSprBtnIcon,10);
    saveSprite(RESPG_DAILY, RESDAILY_SWITCHBTN, pSprBtnIcon);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pSprBtnIcon,RESDAILY_SWITCHBTN);
    
    pLabel = Label::createWithSystemFont(STR_Reservation_Dailycleaning, S_FONE, *pSTR_TITAL_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setColor(COLOR_STRING_BLACK);
    pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pLabel->setPosition(LabelSiteOnBtn_Lab1);
    pSprite->addChild(pLabel,10);
    
    pLabel = Label::createWithSystemFont(STR_Reservation_Choosetime, S_FONE, *pSTR_RESERV_LABSIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setColor(COLOR_STRING_LIGHTBLACK);
    pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pLabel->setPosition(LabelSiteOnBtn_Lab2);
    pSprite->addChild(pLabel,10);
    
    
    
    //Set Start Time
    pSprite=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BK); //背景圖案
    pSprite->setColor(COLOR_STRING_LIGHTGREY);
    pSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pSprite->setPosition(SpriteSite_SetTime);
    pSpriteBK->addChild(pSprite,10);
    saveSprite(RESPG_DAILY, RESDAILY_TIMEBK, pSprite);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pSprite,RESDAILY_TIMEBK);
    
    pLabel = Label::createWithSystemFont(STR_Reservation_SetStartTime, S_FONE, *pSTR_CLEANJOB_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    pLabel->setColor(COLOR_STRING_GREEN);
    pLabel->setPosition(LabelSiteOnBtn_SetTime);
    pSprite->addChild(pLabel,10);
    //設定按紐位置
    Size TimeSetSize	= pSprite->boundingBox().size;
    float W_TimeSetunit = TimeSetSize.width/8;
    float H_TimeSetunit = TimeSetSize.height/4;
    
    Point BtnSite_HourUP=Vec2(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit);
    Point BtnSite_HourNow=Vec2(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*2);
    Point BtnSite_HourDown=Vec2(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*3);
    Point BtnSite_MinUP=Vec2(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit);
    Point BtnSite_MinNow=Vec2(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*2);
    Point BtnSite_MinDown=Vec2(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*3);
    
    //時間設定按鈕
    auto pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案
    pSpriteBtn->setPosition(BtnSite_HourUP);
    pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
    pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_DAILY, RESDAILY_TIMEHOUR_UP_BTN, pSpriteBtn);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEHOUR_UP_BTN);
    pLabel = Label::createWithSystemFont("00", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setColor(COLOR_STRING_BLACK);
    pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
    pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_DAILY, RESDAILY_TIMEHOUR_UP_LAB_VALUE, pLabel);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEHOUR_UP_VALUE);
    
    
    pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案
    pSpriteBtn->setPosition(BtnSite_HourNow);
    pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
    pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_DAILY, RESDAILY_TIMEHOUR_NOW_BTN, pSpriteBtn);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEHOUR_NOW_BTN);
    pLabel = Label::createWithSystemFont("01", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setColor(COLOR_STRING_GREEN);
    pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
    pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_DAILY, RESDAILY_TIMEHOUR_NOW_LAB_VALUE, pLabel);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEHOUR_NOW_VALUE);
    
    
    pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案
    pSpriteBtn->setPosition(BtnSite_HourDown);
    pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
    pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_DAILY, RESDAILY_TIMEHOUR_DOWN_BTN, pSpriteBtn);
    //    m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEHOUR_DOWN_BTN);
    pLabel = Label::createWithSystemFont("02", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setColor(COLOR_STRING_BLACK);
    pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
    pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_DAILY, RESDAILY_TIMEHOUR_DOWN_LAB_VALUE, pLabel);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEHOUR_DOWN_VALUE);
    
    
    pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案
    pSpriteBtn->setPosition(BtnSite_MinUP);
    pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
    pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_DAILY, RESDAILY_TIMEMIN_UP_BTN, pSpriteBtn);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEMIN_UP_BTN);
    pLabel = Label::createWithSystemFont("00", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setColor(COLOR_STRING_BLACK);
    pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
    pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_DAILY, RESDAILY_TIMEMIN_UP_LAB_VALUE, pLabel);
    //    m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEMIN_UP_VALUE);
    
    
    pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案
    pSpriteBtn->setPosition(BtnSite_MinNow);
    pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
    pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_DAILY, RESDAILY_TIMEMIN_NOW_BTN, pSpriteBtn);
    //    m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEMIN_NOW_BTN);
    pLabel = Label::createWithSystemFont("01", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setColor(COLOR_STRING_GREEN);
    pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
    pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_DAILY, RESDAILY_TIMEMIN_NOW_LAB_VALUE, pLabel);
    //    m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEMIN_NOW_VALUE);
    
    
    pSpriteBtn=Sprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案
    pSpriteBtn->setPosition(BtnSite_MinDown);
    pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
    pSprite->addChild(pSpriteBtn,10);
    saveSprite(RESPG_DAILY, RESDAILY_TIMEMIN_DOWN_BTN, pSpriteBtn);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEMIN_DOWN_BTN);
    pLabel = Label::createWithSystemFont("02", S_FONE, *pSTR_BTN_SIZE,Size::Size(0,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setColor(COLOR_STRING_BLACK);
    pLabel->setPosition(Vec2(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
    pSpriteBtn->addChild(pLabel,10);
    saveLabel(RESPG_DAILY, RESDAILY_TIMEMIN_DOWN_LAB_VALUE, pLabel);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEMIN_DOWN_VALUE);
    
    //時間提示
    pLabel = Label::createWithSystemFont(STR_Reservation_Robotwillstartcleaning, S_FONE, *pSTR_CLEANJOB_SIZE,Size::Size(winSize.width,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setAnchorPoint(Vec2(0.5,0.5));
    pLabel->setColor(COLOR_STRING_BLACK);
    pLabel->setPosition(LabelSite_Lab1);
    pSpriteBK->addChild(pLabel,10);
    saveLabel(RESPG_DAILY, RESDAILY_LAB_TIME, pLabel);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_LABTIME);
    
    pLabel = Label::createWithSystemFont("last 09:00", S_FONE, *pSTR_CLEANJOB_SIZE,Size::Size(winSize.width,0),TextHAlignment::CENTER,TextVAlignment::TOP);
    pLabel->setAnchorPoint(Vec2(0.5,0.5));
    pLabel->setColor(COLOR_STRING_RED);
    pLabel->setPosition(LabelSite_Lab2);
    pSpriteBK->addChild(pLabel,10);
    saveLabel(RESPG_DAILY, RESDAILY_LAB_VALUETIME, pLabel);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_VALUETIME);
    
    
    
    
    //底下兩個按鈕
    //底部按鈕
    auto pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
    pSpriteDownBtn->setPosition(Vec2(0,0));
    pSpriteDownBtn->setScaleX(0.5);
    pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
    pSpriteBK->addChild(pSpriteDownBtn);
    
    pLabel = Label::createWithSystemFont(STR_BTN_Canel, S_FONE, *pSTR_BTN_SIZE);
    pLabel->setColor(COLOR_STRING_BLACK);
    pLabel->setScaleX(2);
    pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height/2));
    pSpriteDownBtn->addChild(pLabel,10);
    saveSprite(RESPG_DAILY, RESDAILY_BTNCANEL, pSpriteDownBtn);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteDownBtn,RESDAILY_BTNCANEL);
    
    pSpriteDownBtn=Sprite::createWithSpriteFrameName(S_TITAL_BKIMG);
    pSpriteDownBtn->setAnchorPoint(Vec2::ZERO); //設定錨點
    pSpriteDownBtn->setPosition(Vec2(0+winSize.width*0.5+4,0)); //+4 FOR 中間線
    pSpriteDownBtn->setScaleX(0.5);
    pSpriteDownBtn->setColor(COLOR_STRING_GRAY);
    pSpriteBK->addChild(pSpriteDownBtn);
    pLabel = Label::createWithSystemFont(STR_BTN_Save, S_FONE, *pSTR_BTN_SIZE);
    pLabel->setColor(COLOR_STRING_BLACK);
    pLabel->setScaleX(2);
    pLabel->setPosition(Vec2(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height/2));
    pSpriteDownBtn->addChild(pLabel,10);
    saveSprite(RESPG_DAILY, RESDAILY_BTNSAVE, pSpriteDownBtn);
    //	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteDownBtn,RESDAILY_BTNSAVE);
    pSpriteBK->setVisible(false);
    
}

//根據設定 修改說明標籤剩餘時間
void UI_Reservation::ResetUI_RESMAINLab()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
    auto pLabelHelp = getLabelPoint(RESPG_MAIN, RESMAIN_LAB_SINGALHELP);
//    (CCLabelTTF*)m_BtnArray[RESPG_MAIN]->objectAtIndex(RESMAIN_LABSINGALHELP);
	auto pLabelValue =getLabelPoint(RESPG_MAIN,RESMAIN_LAB_SINGALVALUE);
	if(pData->SingleOn)
	{
		pLabelHelp->setString(STR_Reservation_Robotwillstartcleaning);
		int iLastMin=pData->SingleTime - GetRobotTimeMinuate();
		if(iLastMin<0)
			iLastMin+=24*60;
		int iHour = iLastMin/60;
		int iMin = iLastMin%60;
		char sTempStr[20];
		sprintf(sTempStr,STR_Reservation_ValueTime,iHour,iMin);
		pLabelValue->setString(sTempStr);
		pLabelValue->setVisible(true);
	}
	else
	{
		pLabelHelp->setString(STR_Reservation_Choosetime);
		pLabelValue->setVisible(false);
	}

	pLabelHelp = getLabelPoint(RESPG_MAIN,RESMAIN_LAB_DAILYHELP);
//    (CCLabelTTF*)m_BtnArray[RESPG_MAIN]->objectAtIndex(RESMAIN_LABDAILYHELP);
	pLabelValue = getLabelPoint(RESPG_MAIN,RESMAIN_LAB_DAILYVALUE);
//    (CCLabelTTF*)m_BtnArray[RESPG_MAIN]->objectAtIndex(RESMAIN_LABDAILYVALUE);
	if(pData->DailyOn)
	{
		pLabelHelp->setString(STR_Reservation_Robotwillstartcleaning);
		
		int iLastMin=pData->DailyTime - GetRobotTimeMinuate();
		if(iLastMin<0)
			iLastMin+=24*60;
		int iHour = iLastMin/60;
		int iMin = iLastMin%60;
		char sTempStr[20];
		sprintf(sTempStr,STR_Reservation_ValueTime,iHour,iMin);
		pLabelValue->setString(sTempStr);
		pLabelValue->setVisible(true);
	}
	else
	{
		pLabelHelp->setString(STR_Reservation_Choosetime);
		pLabelValue->setVisible(false);
	}
#endif
}
//根據設定檔 修改顯示畫面
void UI_Reservation::ResetUI_RESSINGALByIni()
{
	Setting_Reservation *pData=&ReserSetting;
	if(pData->SingleOn==1)
	{
		//CCLabelTTF* pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHOFF);
		//pLabel ->setVisible(false);
		//pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHON);
		//pLabel ->setVisible(true);
		
        auto pSprite=getSpritePoint(RESPG_SINGAL, RESSINGAL_TIMEBK);
        pSprite->setVisible(true);
		pSprite=getSpritePoint(RESPG_SINGAL, RESSINGAL_SWITCHBTN);
//        (auto)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHBTN);
		auto pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_RESERVATION_SWITH_ON);
		pSprite->setDisplayFrame(pFrame);

        auto pLabel = getLabelPoint(RESPG_SINGAL, RESSINGAL_LAB_TIME);
//        (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_LABTIME);
		pLabel ->setVisible(true);

		pLabel =  getLabelPoint(RESPG_SINGAL, RESSINGAL_LAB_VALUETIME);
		pLabel ->setVisible(true);

		pSprite= getSpritePoint(RESPG_SINGAL, RESSINGAL_SWITCHBK);
//        (auto )m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHBK);
		pSprite->setColor(COLOR_STRING_WHITE);


	}
	else
	{
		//CCLabelTTF* pLabel =(CCLabelTTF*) m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHOFF);
		//pLabel ->setVisible(true);
		//pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHON);
		//pLabel ->setVisible(false);
		auto pSprite=getSpritePoint(RESPG_SINGAL, RESSINGAL_TIMEBK);
//        (auto)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEBK);
		pSprite->setVisible(false);
		pSprite=getSpritePoint(RESPG_SINGAL, RESSINGAL_SWITCHBTN);
//        (auto)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHBTN);
		auto pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_RESERVATION_SWITH_OFF);
		pSprite->setDisplayFrame(pFrame);
		auto pLabel =getLabelPoint(RESPG_SINGAL, RESSINGAL_LAB_TIME);
//        (CCLabelTTF*) m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_LABTIME);
		pLabel ->setVisible(false);
		pLabel =getLabelPoint(RESPG_SINGAL, RESSINGAL_LAB_VALUETIME);
//        (CCLabelTTF*) m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_VALUETIME);
		pLabel ->setVisible(false);
	}

}
//檢查設定是否一致
void UI_Reservation::ResetUI_RESSINGALCheckSaveBtn()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
	if(memcmp(pData,&ReserSetting,sizeof(ReserSetting))==0)
	{
        auto pSprite=getSpritePoint(RESPG_SINGAL, RESSINGAL_BTNSAVE);
//        (auto)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_BTNSAVE);
		pSprite->setColor(COLOR_STRING_GRAY);
		bAllowSave=false;
	}
	else
	{
		auto pSprite=getSpritePoint(RESPG_SINGAL, RESSINGAL_BTNSAVE);
		pSprite->setColor(COLOR_STRING_GREEN);
		bAllowSave=true;
	}
#endif
}
void UI_Reservation::ResetUI_RESSINGALTime()
{ //根據預約時間修改
	int iShowTime=ReserSetting.SingleTime;
	int iHout = iShowTime/60;
	int iMin = iShowTime%60;
    auto pLabel = getLabelPoint(RESPG_SINGAL, RESSINGAL_TIMEHOUR_UP_LAB_VALUE);
//    (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEHOUR_UP_VALUE);
	char sTimeStr[20];
	if(iHout<0 || iHout>24)
		iHout=0;
	if(iHout==0)
		sprintf(sTimeStr,"23");		
	else
		sprintf(sTimeStr,"%.02d",iHout-1);
	pLabel->setString(sTimeStr);


	pLabel = getLabelPoint(RESPG_SINGAL, RESSINGAL_TIMEHOUR_UP_LAB_VALUE);
    //(CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEHOUR_NOW_VALUE);
	sprintf(sTimeStr,"%.02d",iHout);
	pLabel->setString(sTimeStr);

	pLabel = getLabelPoint(RESPG_SINGAL, RESSINGAL_TIMEHOUR_DOWN_LAB_VALUE);
//    (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEHOUR_DOWN_VALUE);
	if(iHout==23)
		sprintf(sTimeStr,"00");
	else
		sprintf(sTimeStr,"%.02d",iHout+1);
	pLabel->setString(sTimeStr);	


	pLabel =  getLabelPoint(RESPG_SINGAL, RESSINGAL_TIMEMIN_UP_LAB_VALUE);
//    (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEMIN_UP_VALUE);
	if(iMin<0 || iMin>59)
		iMin=0;
	if(iMin==0)
		sprintf(sTimeStr,"59");		
	else
		sprintf(sTimeStr,"%.02d",iMin-1);
	pLabel->setString(sTimeStr);


	pLabel = getLabelPoint(RESPG_SINGAL, RESSINGAL_TIMEMIN_NOW_LAB_VALUE);
//    (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEMIN_NOW_VALUE);
	sprintf(sTimeStr,"%.02d",iMin);
	pLabel->setString(sTimeStr);

	pLabel =  getLabelPoint(RESPG_SINGAL, RESSINGAL_TIMEMIN_DOWN_LAB_VALUE);
//    (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEMIN_DOWN_VALUE);
	if(iMin==59)
		sprintf(sTimeStr,"00");
	else
		sprintf(sTimeStr,"%.02d",iMin+1);
	pLabel->setString(sTimeStr);	


	pLabel = getLabelPoint(RESPG_SINGAL, RESSINGAL_LAB_VALUETIME);
//    (CCLabelTTF*) m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_VALUETIME);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	int iLastTime=iShowTime-GetRobotTimeMinuate();
#else
    int iLastTime=0;
#endif
	if(iLastTime<0)
		iLastTime+= 24*60;
	iHout = iLastTime/60;
	iMin = iLastTime%60;
	sprintf(sTimeStr,STR_Reservation_ValueTime,iHout,iMin);
	pLabel->setString(sTimeStr);	


}


//根據設定檔 修改顯示畫面
void UI_Reservation::ResetUI_RESDAILYByIni()
{
	Setting_Reservation *pData=&ReserSetting;
	if(pData->DailyOn==1)
	{
        auto pSprite=getSpritePoint(RESPG_DAILY, RESDAILY_TIMEBK);
		pSprite->setVisible(true);
		pSprite=getSpritePoint(RESPG_DAILY,RESDAILY_SWITCHBTN);
		auto pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_RESERVATION_SWITH_ON);
		pSprite->setDisplayFrame(pFrame);


        auto pLabel = getLabelPoint(RESPG_DAILY, RESDAILY_LAB_TIME);
        pLabel ->setVisible(true);

		pLabel = getLabelPoint(RESPG_DAILY,RESDAILY_LAB_VALUETIME);
		pLabel ->setVisible(true);

	}
	else
	{
		//CCLabelTTF* pLabel =(CCLabelTTF*) m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_SWITCHOFF);
		//pLabel ->setVisible(true);
		//pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_SWITCHON);
		//pLabel ->setVisible(false);
		auto pSprite=getSpritePoint(RESPG_DAILY, RESDAILY_TIMEBK);
		pSprite->setVisible(false);
		pSprite=getSpritePoint(RESPG_DAILY,RESDAILY_SWITCHBTN);
        auto pFrame= SpriteFrameCache::getInstance()->getSpriteFrameByName(S_RESERVATION_SWITH_OFF);
		pSprite->setDisplayFrame(pFrame);

		auto pLabel = getLabelPoint(RESPG_DAILY, RESDAILY_LAB_TIME);
		pLabel ->setVisible(false);
		pLabel = getLabelPoint(RESPG_DAILY,RESDAILY_LAB_VALUETIME);
		pLabel ->setVisible(false);
	}

}
//檢查設定是否一致
void UI_Reservation::ResetUI_RESDAILYCheckSaveBtn()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
	if(memcmp(pData,&ReserSetting,sizeof(ReserSetting))==0)
	{
		auto pSprite=getSpritePoint(RESPG_DAILY, (RESDAILY_BTNSAVE));
		pSprite->setColor(COLOR_STRING_GRAY);
		bAllowSave=false;
	}
	else
	{
		auto pSprite=getSpritePoint(RESPG_DAILY, (RESDAILY_BTNSAVE));
		pSprite->setColor(COLOR_STRING_GREEN);
		bAllowSave=true;
	}
#endif
}

void UI_Reservation::ResetUI_RESDAILYTime()
{ //根據預約時間修改
	int iShowTime=ReserSetting.DailyTime;
	int iHout = iShowTime/60;
	int iMin = iShowTime%60;
	auto pLabel =getLabelPoint(RESPG_DAILY, RESDAILY_TIMEHOUR_UP_LAB_VALUE);
	char sTimeStr[20];
	if(iHout<0 || iHout>24)
		iHout=0;
	if(iHout==0)
		sprintf(sTimeStr,"23");		
	else
		sprintf(sTimeStr,"%.02d",iHout-1);
	pLabel->setString(sTimeStr);


	pLabel = getLabelPoint(RESPG_DAILY, RESDAILY_TIMEHOUR_NOW_LAB_VALUE);
	sprintf(sTimeStr,"%.02d",iHout);
	pLabel->setString(sTimeStr);

	pLabel = getLabelPoint(RESPG_DAILY, RESDAILY_TIMEHOUR_DOWN_LAB_VALUE);
	if(iHout==23)
		sprintf(sTimeStr,"00");
	else
		sprintf(sTimeStr,"%.02d",iHout+1);
	pLabel->setString(sTimeStr);	


	pLabel = getLabelPoint(RESPG_DAILY, RESDAILY_TIMEMIN_UP_LAB_VALUE);
	if(iMin<0 || iMin>59)
		iMin=0;
	if(iMin==0)
		sprintf(sTimeStr,"59");		
	else
		sprintf(sTimeStr,"%.02d",iMin-1);
	pLabel->setString(sTimeStr);


	pLabel = getLabelPoint(RESPG_DAILY, RESDAILY_TIMEMIN_NOW_LAB_VALUE);
	sprintf(sTimeStr,"%.02d",iMin);
	pLabel->setString(sTimeStr);

	pLabel = getLabelPoint(RESPG_DAILY, RESDAILY_TIMEMIN_DOWN_LAB_VALUE);
	if(iMin==59)
		sprintf(sTimeStr,"00");
	else
		sprintf(sTimeStr,"%.02d",iMin+1);
	pLabel->setString(sTimeStr);	


	pLabel =getLabelPoint(RESPG_DAILY, RESDAILY_LAB_VALUETIME);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    int iLastTime=iShowTime-GetRobotTimeMinuate();
#else
	int iLastTime=0;
#endif
	if(iLastTime<0)
		iLastTime+= 24*60;
	iHout = iLastTime/60;
	iMin = iLastTime%60;
	sprintf(sTimeStr,STR_Reservation_ValueTime,iHout,iMin);
	pLabel->setString(sTimeStr);	


}



void UI_Reservation::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true);
	Show_RESMAIN();
}
void UI_Reservation::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();
}
void UI_Reservation::Hide_AllPage()
{
	Hide_RESMAIN();
	Hide_RESSINGAL();
	Hide_RESDAILY();
	PageType = RESPG_LAYER;
}
void UI_Reservation::Show_RESMAIN()
{
    auto pSprite= getSpritePoint(RESPG_MAIN, RESMAIN_BK);
    pSprite->setVisible(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    
	//ResetSettingByRobotSchedule(); //根據ROBOT 設定 修改本機設定
	Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser); //讀取本機設定
	memcpy((void*)&ReserSetting,pData,sizeof(Setting_Reservation));
#endif
	ResetUI_RESMAINLab();
	//GetSchedule(); //取得預約時間
	PageType = RESPG_MAIN;
}
void UI_Reservation::Hide_RESMAIN()
{
	auto pSprite= getSpritePoint(RESPG_MAIN, RESMAIN_BK);
	pSprite->setVisible(false);
}
void UI_Reservation::Show_RESSINGAL()
{
	auto pSprite= getSpritePoint(RESPG_SINGAL, RESSINGAL_BK);
	pSprite->setVisible(true);	
	ResetUI_RESSINGALByIni();
	ResetUI_RESSINGALCheckSaveBtn();
	ResetUI_RESSINGALTime();
	PageType = RESPG_SINGAL;
}
void UI_Reservation::Hide_RESSINGAL()
{
	auto pSprite= getSpritePoint(RESPG_SINGAL, RESSINGAL_BK);
	pSprite->setVisible(false);
}

void UI_Reservation::Show_RESDAILY()
{
	auto pSprite= getSpritePoint(RESPG_DAILY, RESDAILY_BK);
	pSprite->setVisible(true);
	ResetUI_RESDAILYByIni();
	ResetUI_RESDAILYCheckSaveBtn();
	ResetUI_RESDAILYTime();
	PageType = RESPG_DAILY;
}
void UI_Reservation::Hide_RESDAILY()
{
    auto pSprite= getSpritePoint(RESPG_DAILY, RESDAILY_BK);
	pSprite->setVisible(false);
}

//滑鼠點下!判斷是那個元件
bool UI_Reservation::onTouchBegan(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr){ return true; }
    log("UI_Reservation:%s",__func__);
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
	if(!isVisible()) //有顯示才繼續
		return false;
    auto sprites=getSpriteArray(REMOTEPG_STATE);
    auto bTouch =false;
	switch(PageType)
	{
	default:
		break;
	case RESPG_SINGAL:
		break;
	case RESPG_MAIN:
		{
			//判斷那一個元件
            for(auto pSprite:*sprites)
            {
                auto iIdx=pSprite->getTag();
                if(pSprite->isVisible() && iIdx>=RESMAIN_SINGAL)
                {
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                }
            }
//			for (int i = RESMAIN_SINGAL; i < m_BtnArray[PageType]->count(); i+=RESMAIN_BTNSPACENUM)
//			{  
//				auto pSprite = (auto)m_BtnArray[PageType]->objectAtIndex(i); 
//				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
//				BtnColorSetting(i,bTouch);
//			}  
		}
		break;
	}
		return true;
}
void UI_Reservation::onTouchMoved(Touch* touch, Event* event)
{
//	Point touchLocation = touch->getLocation();    
//	Point nodePosition = convertToNodeSpace( touchLocation );
}
void UI_Reservation::onTouchEnded(Touch* touch, Event* event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    if(target==nullptr){ return;}
    Vec2 touchLocation = touch->getLocation();
    Vec2 nodePosition = target->convertToNodeSpace( touchLocation );
    
	switch(PageType)
	{
	default:
		break;
	case RESPG_DAILY:
		{
			//判斷那一個元件
			bool bTimeBtn = false;
			bool bTouch = false;
            auto sprites=getSpriteArray(RESPG_DAILY);
            for(auto pSprite:*sprites)
            {
                auto btnIndex=pSprite->getTag();
                if(btnIndex!=0)
                {
                    
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        if(btnIndex==RESDAILY_TIMEBK) //中間的時間設定
                        {//點選到中間 需改變touch的座標
                            bTimeBtn=true;
                            nodePosition += pSprite->boundingBox().origin;
                        }
                        else
                        {
                            Function_RESDAILY(btnIndex);
                            break;
                        }
                    }
                    if(bTimeBtn && btnIndex >RESDAILY_TIME_END)
                    {
                        break;
                    }
                }
                
            }
		}
		break;

	case RESPG_SINGAL:
		{
            //判斷那一個元件
            bool bTimeBtn = false;
            bool bTouch = false;
            auto sprites=getSpriteArray(RESPG_SINGAL);
            for(auto pSprite:*sprites)
            {
                auto btnIndex=pSprite->getTag();
                if(btnIndex!=0)
                {
                    
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        if(btnIndex==RESSINGAL_TIMEBK) //中間的時間設定
                        {//點選到中間 需改變touch的座標
                            bTimeBtn=true;
                            nodePosition += pSprite->boundingBox().origin;
                        }
                        else
                        {
                            Function_RESSINGAL(btnIndex);
                            break;
                        }
                    }
                    if(bTimeBtn && btnIndex >RESSINGAL_TIME_END)
                    {
                        break;
                    }
                }
                
            }
            
//			//判斷那一個元件
//			bool bTimeBtn = false;
//			bool bTouch = false;
//			Point nodePositionByTimeBK;
//			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
//			{  
//				auto pSprite = (auto)m_BtnArray[PageType]->objectAtIndex(i); 
//				if(bTimeBtn)
//				{
//					if(i>=RESSINGAL_TIME_START && i <= RESSINGAL_TIME_END)
//						bTouch=pSprite ->boundingBox().containsPoint(nodePositionByTimeBK);				
//					else
//						bTouch=false;
//				}
//				else if(i<RESSINGAL_TIME_START || i > RESSINGAL_TIME_END)
//					bTouch=pSprite ->boundingBox().containsPoint(nodePosition);				
//				else
//					bTouch=false;
//				if(bTouch)
//				{
//					if(i==RESSINGAL_TIMEBK) //中間的時間設定
//					{
//						bTimeBtn=true;
//						nodePositionByTimeBK.x=nodePosition.x-pSprite->boundingBox().origin.x;
//						nodePositionByTimeBK.y=nodePosition.y-pSprite->boundingBox().origin.y;
//					}
//					int iChanglayer=Function_RESSINGAL(i); 
//					if(iChanglayer==RESSINGAL_TITAL) //退回上一頁
//						break;
//				}
//
//			}  
		}
		break;
	case RESPG_MAIN:
		{
			//判斷那一個元件
            bool bTouch = false;
            auto sprites=getSpriteArray(RESPG_MAIN);
            for(auto pSprite:*sprites)
            {
                auto btnIndex=pSprite->getTag();
                if(btnIndex!=0)
                {
                    
                    auto rectBox= pSprite->getBoundingBox();
                    rectBox.origin+=winOrigin;
                    bTouch = rectBox.containsPoint(nodePosition);
                    if(bTouch)
                    {
                        Function_RESMAIN(btnIndex);
                        break;
                    }
                    
                }
                
            }
//			int iType=RESPG_MAIN;
//			for (int i = 1; i < m_BtnArray[iType]->count(); i++) 
//			{  
//				auto pSprite = (auto)m_BtnArray[iType]->objectAtIndex(i); 
//				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
//				if(bTouch)
//				{
//					int iChanglayer=Function_RESMAIN(i);
//                    break;
//				}
////				if(i==RESMAIN_SINGAL || i==RESMAIN_DAILY)
////					BtnColorSetting(i,bTouch);
//			}  
		}
		break;
	}


	
}
//按鈕變色
void UI_Reservation::BtnColorSetting(int iBtnIdx,bool bSel)
{
	if(bSel)
	{
        auto pSprite = getSpritePoint(PageType, iBtnIdx);
		pSprite ->setColor(COLOR_STRING_GREEN);
		auto pLabel= getLabelPoint(PageType, iBtnIdx+1);
		pLabel->setColor(COLOR_STRING_WHITE);
	}
	else
	{
        auto pSprite = getSpritePoint(PageType, iBtnIdx);
		pSprite ->setColor(COLOR_STRING_ORIGIN);
        auto pLabel= getLabelPoint(PageType, iBtnIdx+1);
		pLabel->setColor(COLOR_STRING_BLACK);

	}
}

//按下事件處理
void  UI_Reservation::Function_RESMAIN(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case RESMAIN_DAILY:
		Hide_AllPage();
		Show_RESDAILY();
		break;
	case RESMAIN_SINGAL:
		Hide_AllPage();
		Show_RESSINGAL();
		break;
	case RESMAIN_TITAL:
        Hide_AllPage();
		ChangLayer_Main();
		break;
	}
}

//按下事件處理
void  UI_Reservation::Function_RESSINGAL(int iBtnIdx)
{
	bool bChang=false;
	bool bChangTime=false;
	bool bReturnMain=false;
	switch(iBtnIdx) 
	{
	default:
		break;
	case RESSINGAL_SWITCHBK:
	//case RESSINGAL_SWITCHBTN: //不能判斷 會誤判 因為圖在BTN上面
	//case RESSINGAL_SWITCHOFF:
		if(ReserSetting.SingleOn==1)
			ReserSetting.SingleOn=0;
		else
			ReserSetting.SingleOn=1;
		bChang=true;
		break;
	case RESSINGAL_BTNCANEL:
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
            memcpy((void*)&ReserSetting,pData,sizeof(Setting_Reservation));
#endif
			bChang=true;
			bChangTime =true ;
		}
		break;
	case RESSINGAL_BTNSAVE:
		{
			if(bAllowSave)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				SaveSetting(Setting_Reser,(void*)&ReserSetting);
				SaveSetting_Reservation();
#endif
				SetSchedule(); //傳送資料到ROBOT

				bChang=true;
				bReturnMain=true;
			}
		}
		break;
	case RESSINGAL_TIMEHOUR_UP_BTN:
		{
			if(ReserSetting.SingleTime>=60)
				ReserSetting.SingleTime-=60;
			else
				ReserSetting.SingleTime+=(23*60);
			bChangTime =true ;
		}
		break;
	case RESSINGAL_TIMEHOUR_DOWN_BTN:
		{
			if(ReserSetting.SingleTime<23*60)
				ReserSetting.SingleTime+=60;
			else
				ReserSetting.SingleTime-=(23*60);
			bChangTime =true ;
		}
		break;
	case RESSINGAL_TIMEMIN_UP_BTN:
		{
			if(ReserSetting.SingleTime%60)
				ReserSetting.SingleTime--;
			else
				ReserSetting.SingleTime+=59;
			bChangTime =true ;
		}
		break;
	case RESSINGAL_TIMEMIN_DOWN_BTN:
		{
			if(ReserSetting.SingleTime%60!=59)
				ReserSetting.SingleTime++;
			else
				ReserSetting.SingleTime-=59;
			bChangTime =true ;
		}
		break;
	case RESSINGAL_TITAL:
            bReturnMain =true;
		break;
	}
	if(bChangTime)
	{
		ResetUI_RESSINGALTime();
		bChang=true;
	}
	if(bChang)
	{
		ResetUI_RESSINGALByIni();
		ResetUI_RESSINGALCheckSaveBtn();		
	}
	if(bReturnMain)
	{
		Hide_AllPage();
		Show_RESMAIN();
	}
}
//按下事件處理
void  UI_Reservation::Function_RESDAILY(int iBtnIdx)
{
	bool bChang=false;
	bool bChangTime=false;
	bool bReturnMain=false;
	switch(iBtnIdx) 
	{
	default:
		break;
	case RESDAILY_SWITCHBK:
	//case RESDAILY_SWITCHBTN:
		if(ReserSetting.DailyOn==1)
			ReserSetting.DailyOn=0;
		else
			ReserSetting.DailyOn=1;
		bChang=true;
		break;
	case RESDAILY_BTNCANEL:
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

			Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
			memcpy((void*)&ReserSetting,pData,sizeof(Setting_Reservation));
#endif
			bChang=true;
			bChangTime=true;
		}
		break;
	case RESDAILY_BTNSAVE:
		{
			if(bAllowSave)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				SaveSetting(Setting_Reser,(void*)&ReserSetting);
  				SaveSetting_Reservation();
#endif
				SetSchedule(); //傳送資料到ROBOT
				bChang=true;
				bReturnMain=true;
			}
		}
		break;
	case RESDAILY_TITAL:
        bReturnMain=true;
		break;
	case RESDAILY_TIMEHOUR_UP_BTN:
		{
			if(ReserSetting.DailyTime>=60)
				ReserSetting.DailyTime-=60;
			else
				ReserSetting.DailyTime+=(23*60);
			bChangTime =true ;
		}
		break;
	case RESDAILY_TIMEHOUR_DOWN_BTN:
		{
			if(ReserSetting.DailyTime<23*60)
				ReserSetting.DailyTime+=60;
			else
				ReserSetting.DailyTime-=(23*60);
			bChangTime =true ;
		}
		break;
	case RESDAILY_TIMEMIN_UP_BTN:
		{
			if(ReserSetting.DailyTime%60)
				ReserSetting.DailyTime--;
			else
				ReserSetting.DailyTime+=59;
			bChangTime =true ;
		}
		break;
	case RESDAILY_TIMEMIN_DOWN_BTN:
		{
			if(ReserSetting.DailyTime%60!=59)
				ReserSetting.DailyTime++;
			else
				ReserSetting.DailyTime-=59;
			bChangTime =true ;
		}
		break;

	}
	if(bChangTime)
	{
		ResetUI_RESDAILYTime();
		bChang=true;
	}
	if(bChang)
	{
		ResetUI_RESDAILYByIni();
		ResetUI_RESDAILYCheckSaveBtn();		
	}
	if(bReturnMain)
	{
		Hide_AllPage();
		Show_RESMAIN();
	}
	//return 0;
}

void UI_Reservation::SaveTimeToSchedule(int iType)
{
#if PLAYERSTAGE
    Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
	player_dispatcher_ui_alarm_time_t* pAlarm=pUiSchedule[iType];
	if(pAlarm==NULL) //如果從未設定過 指向下一個空個位置
	{
		pUiSchedule[iType] = (player_dispatcher_ui_alarm_time_t*)AddRobotSachedule(strlen(G_SCHEDULE_JOBNNME)+1);
		pAlarm=pUiSchedule[iType];
	}
	
	bool bSwitch=false;
	if(iType==RESPGALARM_ONCE)
	{
		bSwitch=pData->SingleOn;
		pAlarm->type = PLAYER_DISPATCH_UI_ONCE_ALARM;
	}
	else
	{
		bSwitch=pData->DailyOn;
		pAlarm->type = PLAYER_DISPATCH_UI_DAILY_ARMARM;
	}
	if(bSwitch==0)
		pAlarm->play_type  = PLAYER_DISPATCH_UI_SUSPEND_ALARM;
	else
		pAlarm->play_type  = PLAYER_DISPATCH_UI_RESUME_ALARM;


	pAlarm->uuid[0]=0x00;
	pAlarm->uuid[1]=0x0C;
	pAlarm->uuid[2]=0x29;
	pAlarm->uuid[3]=0xf3;
	pAlarm->uuid[4]=0xe8;
	pAlarm->uuid[5]=0xa2;
	pAlarm->uuid[6]=0x00;	
	pAlarm->uuid[7]=iType;
	pAlarm->time = *(player_dispatcher_time_t*)GetSchedulTime(iType);
	pAlarm->zone = *(player_dispatcher_timezone_t *)GetRobotZone();		
	pAlarm->job.cmd=PLAYER_DISPATCH_AI_AUTO;
	pAlarm->job.power=PLAYER_DISPATCH_PM_AUTO;
	pAlarm->job.target_point.px=0;
	pAlarm->job.target_point.py=0;
	pAlarm->job.target_point.pa=0;
	pAlarm->job.map_index_count=0;
	pAlarm->job.map_index=NULL;
	pAlarm->job.name_count=strlen(G_SCHEDULE_JOBNNME)+1; //因為手機APP 那邊不知為何是10 多1
	pAlarm->job.name = (int8_t*)malloc( sizeof(int8_t)*pAlarm->job.name_count);
	sprintf((char*)pAlarm->job.name,G_SCHEDULE_JOBNNME);
	pAlarm->job.data_count=0;
	pAlarm->job.data=NULL;
	pAlarm->job.stop_cond=PLAYER_DISPATCH_AI_COND_JOB_DONE;
#endif

}
void UI_Reservation::SaveRobotSachedule(void* pData)
{	
	if(iScheduleTotalNumber<0)
		iScheduleTotalNumber=0;
	iScheduleTotalNumber++; //紀錄總共數量
 #if PLAYERSTAGE
	player_dispatcher_ui_alarm_time_t *pAlarm=(player_dispatcher_ui_alarm_time_t *)pData;
	//memcpy(&stRobotShedule[iScheduleCount],pAlarm,sizeof(player_dispatcher_ui_alarm_time_t));
	//釋放舊空間
	if ( stRobotShedule[iScheduleCount].job.map_index_count!=0 )
		free(stRobotShedule[iScheduleCount].job.map_index);
	if ( stRobotShedule[iScheduleCount].job.data_count!=0  )
		free(stRobotShedule[iScheduleCount].job.data);
	if ( stRobotShedule[iScheduleCount].job.name_count!=0 )
		free(stRobotShedule[iScheduleCount].job.name);
	//儲放新資料
	memcpy(stRobotShedule[iScheduleCount].uuid, pAlarm->uuid, sizeof(stRobotShedule[iScheduleCount].uuid));
	stRobotShedule[iScheduleCount].type	=	pAlarm->type;
	stRobotShedule[iScheduleCount].play_type=pAlarm->play_type;
	
	stRobotShedule[iScheduleCount].time	=pAlarm->time;
	stRobotShedule[iScheduleCount].zone	=pAlarm->zone;
	stRobotShedule[iScheduleCount].job.cmd	=pAlarm->job.cmd;
	stRobotShedule[iScheduleCount].job.power=pAlarm->job.power;
	stRobotShedule[iScheduleCount].job.map_index_count	=pAlarm->job.map_index_count;
	stRobotShedule[iScheduleCount].job.data_count	=pAlarm->job.data_count;
	stRobotShedule[iScheduleCount].job.name_count	=pAlarm->job.name_count;

	stRobotShedule[iScheduleCount].job.target_point	=pAlarm->job.target_point;
	stRobotShedule[iScheduleCount].job.stop_cond	=pAlarm->job.stop_cond;
	if ( pAlarm->job.map_index_count!=0 )
	{
		stRobotShedule[iScheduleCount].job.map_index = (uint32_t*) malloc( sizeof(uint32_t)*pAlarm->job.map_index_count );
		 CC_ASSERT(stRobotShedule[iScheduleCount].job.map_index);
		memcpy(stRobotShedule[iScheduleCount].job.map_index,
			pAlarm->job.map_index,
			sizeof(uint32_t)*pAlarm->job.map_index_count);
	}

	if ( pAlarm->job.data_count!=0 )
	{
		stRobotShedule[iScheduleCount].job.data=(int8_t*)malloc( sizeof(int8_t)*pAlarm->job.data_count );
		CC_ASSERT(stRobotShedule[iScheduleCount].job.data);
		memcpy(stRobotShedule[iScheduleCount].job.data,
			pAlarm->job.data,
			sizeof(uint8_t)*pAlarm->job.data_count);
	}

	if ( pAlarm->job.name_count!=0 )
	{
		stRobotShedule[iScheduleCount].job.name=(int8_t*)malloc( sizeof(int8_t)*pAlarm->job.name_count );
		CC_ASSERT(stRobotShedule[iScheduleCount].job.name);
		memcpy(stRobotShedule[iScheduleCount].job.name,
			pAlarm->job.name,
			sizeof(uint8_t)*pAlarm->job.name_count);
	}
#endif

	iScheduleCount++;
	if(iScheduleCount>=G_ROBOT_SCHEDULE_MAX) //數量過多 覆蓋之前舊的
		iScheduleCount=0;	
	if(iScheduleTotalNumber>G_ROBOT_SCHEDULE_MAX) //只有取100個
		iScheduleTotalNumber=G_ROBOT_SCHEDULE_MAX;
}
void* UI_Reservation::AddRobotSachedule(int iNameSize)
{
#if PLAYERSTAGE
	int iSpaceIdx=iScheduleCount;
#endif
	iScheduleTotalNumber++; //紀錄總共數量
	iScheduleCount++;
	if(iScheduleCount>=G_ROBOT_SCHEDULE_MAX) //數量過多 覆蓋之前舊的
		iScheduleCount=0;	
	if(iScheduleTotalNumber>G_ROBOT_SCHEDULE_MAX) //只有取100個
		iScheduleTotalNumber=G_ROBOT_SCHEDULE_MAX;
 #if PLAYERSTAGE
	//釋放舊空間
	if ( stRobotShedule[iSpaceIdx].job.map_index_count!=0 )
		free(stRobotShedule[iSpaceIdx].job.map_index);
	if ( stRobotShedule[iSpaceIdx].job.data_count!=0  )
		free(stRobotShedule[iSpaceIdx].job.data);
	if ( stRobotShedule[iSpaceIdx].job.name_count!=0 )
		free(stRobotShedule[iSpaceIdx].job.name);

	return (void*)&stRobotShedule[iSpaceIdx];
#else
    return nullptr;
#endif
}
//根據機器的Schedule 修改現有設定
void UI_Reservation::ResetSettingByRobotSchedule()
{
#if PLAYERSTAGE
	Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
	player_dispatcher_ui_alarm_time_t *pAlarm;

	if(iScheduleTotalNumber>=0) //ROBOT裡沒有任何紀錄 若為-1 不做任何動作
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		memset(pData,0,sizeof(Setting_Reservation)); //清空InI
		SaveSetting_Reservation();
#endif	
	}
	
	bool bSingleOK=false;
	bool bDailyOK=false;
	bool bAutoModeSchedule=false;
	int iAutoModeCount=strlen(G_SCHEDULE_JOBNNME)+1; //因為手機APP 那邊不知為何是10 所以+1
	//for(int i=iScheduleTotalNumber-1;i>=0 &&  (bSingleOK==false || bDailyOK==false);i--)
	for(int i=0;i<iScheduleTotalNumber &&  (bSingleOK==false || bDailyOK==false);i++)
	{
		pAlarm=&stRobotShedule[i];
		bAutoModeSchedule =false;
		if(pAlarm->job.name_count==iAutoModeCount)
		{
			if(strcmp((char*)pAlarm->job.name,G_SCHEDULE_JOBNNME)==0)
				bAutoModeSchedule =true;
		}
		
		if(bAutoModeSchedule && bSingleOK==false)
		{ //偵測單次設定
			
			if(pAlarm->type==PLAYER_DISPATCH_UI_ONCE_ALARM)
			{
				if(pAlarm->play_type==PLAYER_DISPATCH_UI_SUSPEND_ALARM)
				{
					pData->SingleOn=0;
				}
				else if(pAlarm->play_type==PLAYER_DISPATCH_UI_RESUME_ALARM)
				{
					pData->SingleOn=1;
					//取得ROBOT 預約時間
					int ScheduleMin=pAlarm->time.tm_hour*60+pAlarm->time.tm_min;
					//int iLastMin=ScheduleMin- GetRobotTimeMinuate();
					//if(iLastMin<0)
					//	iLastMin+=24*60;
					//pData->SingleTime=iLastMin;
					pData->SingleTime=ScheduleMin;

				}
				bSingleOK=true;
				pUiSchedule[RESPGALARM_ONCE]=pAlarm;
			}
		}
		if(bAutoModeSchedule && bDailyOK==false)
		{
			if(pAlarm->type==PLAYER_DISPATCH_UI_DAILY_ARMARM)
			{
				if(pAlarm->play_type==PLAYER_DISPATCH_UI_SUSPEND_ALARM)
				{
					pData->DailyOn=0;
				}
				else if(pAlarm->play_type==PLAYER_DISPATCH_UI_RESUME_ALARM)
				{
					pData->DailyOn=1;
					//取得ROBOT 預約時間
					int ScheduleMin=pAlarm->time.tm_hour*60+pAlarm->time.tm_min;
					//int iLastMin=ScheduleMin- GetRobotTimeMinuate();
					//if(iLastMin<0)
					//	iLastMin+=24*60;
					//pData->DailyTime=iLastMin;
					pData->DailyTime=ScheduleMin;
				}
				bDailyOK=true;
				pUiSchedule[RESPGALARM_DAILY]=pAlarm;
			}		
		}
	}
#endif
}
//ROBOT 設定預約掃地
void UI_Reservation::SetSchedule()
{
#if PLAYERSTAGE
	uint8_t initiator;
	pDisProxy->GetReqUiSchedule(&initiator);
	SaveTimeToSchedule(RESPGALARM_ONCE);
	SaveTimeToSchedule(RESPGALARM_DAILY);
	//pDisProxy->SetReqAckUiSchedule(initiator, RESPGALARM_SIZE, &UiSchedule[0]);
	pDisProxy->SetUiSchedule(PLAYER_DISPATCH_INIT_DEST_RD, iScheduleTotalNumber, &stRobotShedule[0]);
#endif
}
//取得ROBOT 設定 (不使用了 改為ReleqaseUI)
void UI_Reservation::GetSchedule()
{
#if PLAYERSTAGE

	uint32_t count;
	uint8_t initiator;
	//player_dispatcher_ui_alarm_time_t *alarm=pDisProxy->GetUiSchedule(&initiator, &count);
	player_dispatcher_ui_alarm_time_t *alarm;
	alarm=pDisProxy->ReqUiSchedule(PLAYER_DISPATCH_INIT_DEST_RD, &count);
	if(count==0)
	{
		CCLog("No Schedule");
	}
	for ( int i=0; i<count; i++ )
	{
		switch (alarm[i].type)
		{
		case PLAYER_DISPATCH_UI_NO_ALARM:
			CCLog("no alarm\n");
			break;

		case PLAYER_DISPATCH_UI_ONCE_ALARM:
			CCLog("repeat once alarm:");
			break;

		case PLAYER_DISPATCH_UI_DAILY_ARMARM:
			CCLog("repeat daily alarm:");
			break;

		case PLAYER_DISPATCH_UI_WEEKLY_ARMARM:
			CCLog("repeat weekly alarm:");
			break;

		case PLAYER_DISPATCH_UI_PER_YEAR_ALARM:
			CCLog("repeat year alarm:");
			break;

		default:
			CCLog("unknow alarm type:");
			break;
		}//end switch (alarm[i].type)

		switch (alarm[i].play_type)
		{
		case PLAYER_DISPATCH_UI_SUSPEND_ALARM:
			CCLog("(suspend)");
			break;

		case PLAYER_DISPATCH_UI_RESUME_ALARM:
			CCLog("(resume)");
			break;

		default:
			CCLog("unknow alarm play_type\n");
			break;
		}//end switch (alarm[i].play_type)

		//CCLog("initiator=%d \n", initiator);

		//CCLog("UUID:");
		//for ( int j=0; j<8; j++)
		//{
		//	CCLog("%02x ",alarm[i].uuid[j]);
		//}

		CCLog("\nData= %.4d%.02d%.02d %.02d:%.02d:%.02d",
			alarm[i].time.tm_year, alarm[i].time.tm_mon, alarm[i].time.tm_mday, alarm[i].time.tm_hour, alarm[i].time.tm_min, alarm[i].time.tm_sec);
		//CCLog("minuteswest=%d dsttime=%d\n",
		//	alarm[i].zone.minuteswest, alarm[i].zone.dsttime);
		//CCLog("job.cmd=%d job.power=%d job.map_index_count=%d job.name=%s job.data_count=%d\n",
		//	alarm[i].job.cmd, alarm[i].job.power, alarm[i].job.map_index_count, alarm[i].job.name,  alarm[i].job.data_count);

		if ( alarm[i].job.map_index_count )
		{
			for ( int i=0; i<alarm[i].job.map_index_count ; i++)
			{
				CCLog("alarm[i].job.map_index=%d\n", alarm[i].job.map_index[i] );
			}//end for ( int i=0; i<alarm[i].job.map_index_count ; i++)
		}//end if ( alarm[i].job.map_index_count )
		if ( alarm[i].job.data_count )
		{
		}//else if ( alarm[i].job.data_count )
	}
#endif

}
void UI_Reservation::setTouchEnabled(bool enable)
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
        
        touchListener->onTouchBegan = CC_CALLBACK_2(UI_Reservation::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(UI_Reservation::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(UI_Reservation::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
    }
    else
    {
        _eventDispatcher->removeEventListener(touchListener);
        CC_SAFE_RELEASE_NULL(touchListener);
    }
}
bool UI_Reservation::isTouchEnabled() const
{
    return touchEnabled;
}
Sprite* UI_Reservation::getSpritePoint(int iPageTyep,int iPageIdx)
{
    return m_SpriteVectors.at(iPageTyep)->at(m_itemIdx[iPageTyep*100+iPageIdx]);
}
Label* UI_Reservation::getLabelPoint(int iPageTyep,int iPageIdx)
{
    return m_LabelVectors.at(iPageTyep)->at(m_itemIdx[iPageTyep*100+iPageIdx]);
}
void UI_Reservation::saveSprite(int iPageTyep,int iPageIdx,Sprite* pSprite)
{
    pSprite->setTag(iPageIdx);
    m_itemIdx[iPageTyep*100+iPageIdx]=m_SpriteVectors.at(iPageTyep)->size();
    m_SpriteVectors.at(iPageTyep)->pushBack(pSprite);
}
void UI_Reservation::saveLabel(int iPageTyep,int iPageIdx,Label* pLabel)
{
    pLabel->setTag(iPageIdx);
    m_itemIdx[iPageTyep*100+iPageIdx]=m_LabelVectors.at(iPageTyep)->size();
    m_LabelVectors.at(iPageTyep)->pushBack(pLabel);
    
}
SpriteVector* UI_Reservation::getSpriteArray(int pageType)
{
    return m_SpriteVectors.at(PageType);
}
LabelVector* UI_Reservation::getLabelArray(int pageType)
{
    return m_LabelVectors.at(PageType);
}
