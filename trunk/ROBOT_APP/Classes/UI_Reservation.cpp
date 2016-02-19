#include <ImageList.h>
#include <HelloWorldScene.h>
#include <libplayerc++/playerc++.h>
USING_NS_CC;
using namespace std;
using namespace PlayerCc;
Setting_Reservation ReserSetting;
player_dispatcher_ui_alarm_time_t *pUiSchedule[RESPGALARM_SIZE];
player_dispatcher_ui_alarm_time_t stRobotShedule[G_ROBOT_SCHEDULE_MAX];

extern HelloWorld* m_MainView; 
/// Player Client
extern PlayerClient *pPlayerclient;
extern DispatcherProxy *pDisProxy;


UI_Reservation::UI_Reservation()
{
	PageType=MAPPG_LAYER;
	memset((void*)&ReserSetting,0,sizeof(Setting_Reservation));
	//memset(UiSchedule,0,sizeof(player_dispatcher_ui_alarm_time_t)*RESPGALARM_SIZE);
	memset(pUiSchedule,0,sizeof(pUiSchedule));
	memset(stRobotShedule,0,sizeof(player_dispatcher_ui_alarm_time_t)*G_ROBOT_SCHEDULE_MAX);
	iScheduleTotalNumber = -1; //有可能初始讀取失敗
	iScheduleCount =0 ;
}

UI_Reservation::~UI_Reservation()
{
	for(int i=0;i<MAPPG_SIZE;i++)
		m_BtnArray[i]->release();

	for(int i=0;i<iScheduleTotalNumber;i++)
	{
		if ( stRobotShedule[i].job.map_index_count!=0 )
			free(stRobotShedule[i].job.map_index);
		if ( stRobotShedule[i].job.data_count!=0  )
			free(stRobotShedule[i].job.data);
		if ( stRobotShedule[i].job.name_count!=0 )
			free(stRobotShedule[i].job.name);
	}
}

bool UI_Reservation::init()
{  
	if (!CCLayer::init()) 
	{  
		return false;  
	}  
	InitUI();  
	return true;  
}  

void UI_Reservation::InitUI()
{
	winSize = CCDirector::sharedDirector()->getVisibleSize(); //取得視窗大小
	InitUI_RESMAIN();
	InitUI_RESSINGAL();
	InitUI_RESDAILY();
}

void UI_Reservation::InitUI_RESMAIN()
{
	m_BtnArray[RESPG_MAIN] = CCArray::createWithCapacity(RESMAIN_SIZE); //按鈕存放
	m_BtnArray[RESPG_MAIN]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	char sTempStr[20];
	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	m_BtnArray[RESPG_MAIN]->addObject(pSpriteBK);

	CCPoint SpriteSite_Singal=CCPointMake(48*W_unit,winSize.height-244*H_unit);	
	CCPoint LabelSite_Singal=CCPointMake(368*W_unit,winSize.height-271*H_unit);
	CCPoint LabelSite_Tital_OnBtn = CCPointMake(303*W_unit,(248-53)*H_unit);
	CCPoint LabelSite_Help_OnBtn  = CCPointMake(303*W_unit,(248-150)*H_unit);
	CCPoint LabelSite_Value_OnBtn = CCPointMake(712*W_unit,(248-150)*H_unit);

	CCPoint SpriteSite_Daily=CCPointMake(48*W_unit,winSize.height-516*H_unit);	
	
	

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_Reservation, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[RESPG_MAIN]->insertObject(pSpriteAction,RESMAIN_TITAL);

	//2個工作JOB 增加 背景版
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP); //SINGAL圖示
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(SpriteSite_Singal);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[RESPG_MAIN]->insertObject(pSprite,RESMAIN_SINGAL);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_RESERVATION_BTN_SINGAL); 
	pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));
	pSprBtnIcon->setPosition(CCPointMake(0,pSprite->getContentSize().height));
	pSprite->addChild(pSprBtnIcon,10);
	pLabel = CCLabelTTF::create(STR_BTN_SingleAppointment, S_FONE, *pSTR_CLEANJOB_SIZE,CCSizeMake(600*W_unit,144*H_unit),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Tital_OnBtn);
	pSprite->addChild(pLabel,10);
	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABSINGAL);
	
	//下方說明文字
	pLabel = CCLabelTTF::create(STR_Reservation_Robotwillstartcleaning, S_FONE, *pSTR_LATER_SIZE, CCSizeMake(600*W_unit,144*H_unit), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Help_OnBtn);
	pSprite->addChild(pLabel,10);
	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABSINGALHELP);
	//啟動時間
	
	sprintf(sTempStr,STR_Reservation_ValueTime,0,0);
	pLabel = CCLabelTTF::create(sTempStr, S_FONE, *pSTR_LATER_SIZE, CCSizeMake(600*W_unit,144*H_unit), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_RED);
	pLabel->setPosition(LabelSite_Value_OnBtn);
	pSprite->addChild(pLabel,10);
	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABSINGALVALUE);
	

	pSprite=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP); //DAILY圖示
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(SpriteSite_Daily);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[RESPG_MAIN]->insertObject(pSprite,RESMAIN_DAILY);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_RESERVATION_BTN_DAILY); 
	pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));
	pSprBtnIcon->setPosition(CCPointMake(0,pSprite->getContentSize().height));
	pSprite->addChild(pSprBtnIcon,10);
	pLabel = CCLabelTTF::create(STR_BTN_Dailycleaning, S_FONE, *pSTR_CLEANJOB_SIZE,CCSizeMake(600*W_unit,144*H_unit),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Tital_OnBtn);
	pSprite->addChild(pLabel,10);
	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABDAILY);

	//下方說明文字
	pLabel = CCLabelTTF::create(STR_Reservation_Robotwillstartcleaning, S_FONE, *pSTR_LATER_SIZE, CCSizeMake(600*W_unit,144*H_unit), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Help_OnBtn);
	pSprite->addChild(pLabel,10);
	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABDAILYHELP);

	//啟動時間
	pLabel = CCLabelTTF::create(sTempStr, S_FONE, *pSTR_LATER_SIZE, CCSizeMake(600*W_unit,144*H_unit), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_RED);
	pLabel->setPosition(LabelSite_Value_OnBtn);
	pSprite->addChild(pLabel,10);
	m_BtnArray[RESPG_MAIN]->insertObject(pLabel,RESMAIN_LABDAILYVALUE);

}

void UI_Reservation::InitUI_RESSINGAL()
{
	m_BtnArray[RESPG_SINGAL] = CCArray::createWithCapacity(RESSINGAL_SIZE); //按鈕存放
	m_BtnArray[RESPG_SINGAL]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	
	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	m_BtnArray[RESPG_SINGAL]->addObject(pSpriteBK);

	CCPoint SpriteSite_Singal=CCPointMake(0*W_unit,winSize.height-150*H_unit);		
	CCPoint LabelSiteOnBtn_Lab1=CCPointMake(125*W_unit,(238-75)*H_unit);
	CCPoint LabelSiteOnBtn_Lab2=CCPointMake(125*W_unit,(238-171)*H_unit);	
	CCPoint SpriteSite_SetTime=CCPointMake(0*W_unit,winSize.height-572*H_unit);	
	CCPoint LabelSite_SetTime=CCPointMake(368*W_unit,winSize.height-541*H_unit);
	CCPoint LabelSiteOnBtn_SetTime=CCPointMake(328*W_unit,(680-40)*H_unit);
	CCPoint LabelSite_Lab1=CCPointMake(380*W_unit,winSize.height-1356*H_unit);
	CCPoint LabelSite_Lab2=CCPointMake(880*W_unit,winSize.height-1356*H_unit);



	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_Reservation, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteAction,RESSINGAL_TITAL);

	//ON/OFF 背景
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP2); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(SpriteSite_Singal);
	pSprite->setColor(COLOR_STRING_WHITE);	
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSprite,RESSINGAL_SWITCHBK);

	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_RESERVATION_SWITH_OFF); //關閉圖示
	pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));
	pSprBtnIcon->setPosition(CCPointMake(834*W_unit,pSprite->getContentSize().height*0.8));
	pSprBtnIcon->setVisible(true);	
	pSprite->addChild(pSprBtnIcon,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSprBtnIcon,RESSINGAL_SWITCHBTN);
	//pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_RESERVATION_SWITH_ON); //開啟圖示
	//pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));
	//pSprBtnIcon->setPosition(CCPointMake(834*W_unit,pSprite->getContentSize().height*0.8));
	//pSprite->addChild(pSprBtnIcon,10);
	//pSprBtnIcon->setVisible(false);
	//m_BtnArray[RESPG_SINGAL]->insertObject(pSprBtnIcon,RESSINGAL_SWITCHON);
	pLabel = CCLabelTTF::create(STR_BTN_SingleAppointment, S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSiteOnBtn_Lab1);
	pSprite->addChild(pLabel,10);
	pLabel = CCLabelTTF::create(STR_Reservation_Choosetime, S_FONE, *pSTR_RESERV_LABSIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_LIGHTBLACK);
	pLabel->setPosition(LabelSiteOnBtn_Lab2);
	pSprite->addChild(pLabel,10);

	

	//Set Start Time
	pSprite=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BK); //背景圖案
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(SpriteSite_SetTime);
	pSprite->setColor(COLOR_STRING_LIGHTGREY);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSprite,RESSINGAL_TIMEBK);
	pLabel = CCLabelTTF::create(STR_Reservation_SetStartTime, S_FONE, *pSTR_CLEANJOB_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabelSiteOnBtn_SetTime);
	pSprite->addChild(pLabel,10);

	//設定按紐位置
	CCSize TimeSetSize	= pSprite->boundingBox().size;
	float W_TimeSetunit = TimeSetSize.width/8;
	float H_TimeSetunit = TimeSetSize.height/4;

	CCPoint BtnSite_HourUP=CCPointMake(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit);
	CCPoint BtnSite_HourNow=CCPointMake(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*2);
	CCPoint BtnSite_HourDown=CCPointMake(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*3);
	CCPoint BtnSite_MinUP=CCPointMake(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit);
	CCPoint BtnSite_MinNow=CCPointMake(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*2);
	CCPoint BtnSite_MinDown=CCPointMake(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*3);

	//時間設定按鈕
	CCSprite *pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_HourUP);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEHOUR_UP_BTN);
	pLabel = CCLabelTTF::create("00", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEHOUR_UP_VALUE);


	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_HourNow);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEHOUR_NOW_BTN);
	pLabel = CCLabelTTF::create("01", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEHOUR_NOW_VALUE);


	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_HourDown);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEHOUR_DOWN_BTN);
	pLabel = CCLabelTTF::create("02", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEHOUR_DOWN_VALUE);


	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_MinUP);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEMIN_UP_BTN);
	pLabel = CCLabelTTF::create("00", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEMIN_UP_VALUE);


	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_MinNow);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEMIN_NOW_BTN);
	pLabel = CCLabelTTF::create("01", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEMIN_NOW_VALUE);


	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_MinDown);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteBtn,RESSINGAL_TIMEMIN_DOWN_BTN);
	pLabel = CCLabelTTF::create("02", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_TIMEMIN_DOWN_VALUE);

	//時間提示
	pLabel = CCLabelTTF::create(STR_Reservation_Robotwillstartcleaning, S_FONE, *pSTR_CLEANJOB_SIZE,CCSizeMake(winSize.width,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0.5,0.5));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Lab1);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_LABTIME);

	pLabel = CCLabelTTF::create("last 09:00", S_FONE, *pSTR_CLEANJOB_SIZE,CCSizeMake(winSize.width,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0.5,0.5));
	pLabel->setColor(COLOR_STRING_RED);
	pLabel->setPosition(LabelSite_Lab2);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pLabel,RESSINGAL_VALUETIME);


	//底下兩個按鈕
	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setScaleX(0.5);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);

	pLabel = CCLabelTTF::create(STR_BTN_Canel, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setScaleX(2);
	pLabel->setPosition(ccp(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteDownBtn,RESSINGAL_BTNCANEL);

	pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0+winSize.width*0.5+4,0)); //+4 FOR 中間線
	pSpriteDownBtn->setScaleX(0.5);
	pSpriteDownBtn->setColor(COLOR_STRING_GREY);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_BTN_Save, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setScaleX(2);
	pLabel->setPosition(ccp(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_SINGAL]->insertObject(pSpriteDownBtn,RESSINGAL_BTNSAVE);
}

//根據設定 修改說明標籤剩餘時間
void UI_Reservation::ResetUI_RESMAINLab()
{
	Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
	CCLabelTTF* pLabelHelp = (CCLabelTTF*)m_BtnArray[RESPG_MAIN]->objectAtIndex(RESMAIN_LABSINGALHELP);
	CCLabelTTF* pLabelValue = (CCLabelTTF*)m_BtnArray[RESPG_MAIN]->objectAtIndex(RESMAIN_LABSINGALVALUE);
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

	pLabelHelp = (CCLabelTTF*)m_BtnArray[RESPG_MAIN]->objectAtIndex(RESMAIN_LABDAILYHELP);
	pLabelValue = (CCLabelTTF*)m_BtnArray[RESPG_MAIN]->objectAtIndex(RESMAIN_LABDAILYVALUE);
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
		
		CCSprite* pSprite=(CCSprite*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEBK);
		pSprite->setVisible(true);
		pSprite=(CCSprite*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHBTN);
		CCSpriteFrame *pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_RESERVATION_SWITH_ON);
		pSprite->setDisplayFrame(pFrame);

		CCLabelTTF* pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_LABTIME);
		pLabel ->setVisible(true);

		pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_VALUETIME);
		pLabel ->setVisible(true);

		pSprite= (CCSprite* )m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHBK);
		pSprite->setColor(COLOR_STRING_WHITE);	


	}
	else
	{
		//CCLabelTTF* pLabel =(CCLabelTTF*) m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHOFF);
		//pLabel ->setVisible(true);
		//pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHON);
		//pLabel ->setVisible(false);
		CCSprite* pSprite=(CCSprite*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEBK);
		pSprite->setVisible(false);
		pSprite=(CCSprite*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_SWITCHBTN);
		CCSpriteFrame *pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_RESERVATION_SWITH_OFF);
		pSprite->setDisplayFrame(pFrame);
		CCLabelTTF* pLabel =(CCLabelTTF*) m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_LABTIME);
		pLabel ->setVisible(false);
		pLabel =(CCLabelTTF*) m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_VALUETIME);
		pLabel ->setVisible(false);
	}

}

//檢查設定是否一致
void UI_Reservation::ResetUI_RESSINGALCheckSaveBtn()
{
	Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
	if(memcmp(pData,&ReserSetting,sizeof(ReserSetting))==0)
	{
		CCSprite* pSprite=(CCSprite*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_BTNSAVE);
		pSprite->setColor(COLOR_STRING_GREY);
		bAllowSave=false;
	}
	else
	{
		CCSprite* pSprite=(CCSprite*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_BTNSAVE);
		pSprite->setColor(COLOR_STRING_GREEN);
		bAllowSave=true;
	}
}

void UI_Reservation::ResetUI_RESSINGALTime()
{ //根據預約時間修改
	int iShowTime=ReserSetting.SingleTime;
	int iHout = iShowTime/60;
	int iMin = iShowTime%60;
	CCLabelTTF *pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEHOUR_UP_VALUE);
	char sTimeStr[20];
	if(iHout<0 || iHout>24)
		iHout=0;
	if(iHout==0)
		sprintf(sTimeStr,"23");		
	else
		sprintf(sTimeStr,"%.02d",iHout-1);
	pLabel->setString(sTimeStr);


	pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEHOUR_NOW_VALUE);
	sprintf(sTimeStr,"%.02d",iHout);
	pLabel->setString(sTimeStr);

	pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEHOUR_DOWN_VALUE);
	if(iHout==23)
		sprintf(sTimeStr,"00");
	else
		sprintf(sTimeStr,"%.02d",iHout+1);
	pLabel->setString(sTimeStr);	


	pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEMIN_UP_VALUE);
	if(iMin<0 || iMin>59)
		iMin=0;
	if(iMin==0)
		sprintf(sTimeStr,"59");		
	else
		sprintf(sTimeStr,"%.02d",iMin-1);
	pLabel->setString(sTimeStr);


	pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEMIN_NOW_VALUE);
	sprintf(sTimeStr,"%.02d",iMin);
	pLabel->setString(sTimeStr);

	pLabel = (CCLabelTTF*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_TIMEMIN_DOWN_VALUE);
	if(iMin==59)
		sprintf(sTimeStr,"00");
	else
		sprintf(sTimeStr,"%.02d",iMin+1);
	pLabel->setString(sTimeStr);	


	pLabel =(CCLabelTTF*) m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_VALUETIME);
	int iLastTime=iShowTime-GetRobotTimeMinuate();
	if(iLastTime<0)
		iLastTime+= 24*60;
	iHout = iLastTime/60;
	iMin = iLastTime%60;
	sprintf(sTimeStr,STR_Reservation_ValueTime,iHout,iMin);
	pLabel->setString(sTimeStr);	


}

void UI_Reservation::InitUI_RESDAILY()
{
	m_BtnArray[RESPG_DAILY] = CCArray::createWithCapacity(RESDAILY_SIZE); //按鈕存放
	m_BtnArray[RESPG_DAILY]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	m_BtnArray[RESPG_DAILY]->addObject(pSpriteBK);

	CCPoint SpriteSite_Singal=CCPointMake(0*W_unit,winSize.height-150*H_unit);		
	CCPoint LabelSiteOnBtn_Lab1=CCPointMake(125*W_unit,(238-75)*H_unit);
	CCPoint LabelSiteOnBtn_Lab2=CCPointMake(125*W_unit,(238-171)*H_unit);	
	CCPoint SpriteSite_SetTime=CCPointMake(0*W_unit,winSize.height-572*H_unit);	
	CCPoint LabelSite_SetTime=CCPointMake(368*W_unit,winSize.height-541*H_unit);
	CCPoint LabelSiteOnBtn_SetTime=CCPointMake(328*W_unit,(680-40)*H_unit);
	CCPoint LabelSite_Lab1=CCPointMake(380*W_unit,winSize.height-1356*H_unit);
	CCPoint LabelSite_Lab2=CCPointMake(880*W_unit,winSize.height-1356*H_unit);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_Reservation, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteAction,RESDAILY_TITAL);

	//ON/OFF 背景
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_BTN_BACKGROUNP2); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(SpriteSite_Singal);
	pSprite->setColor(COLOR_STRING_WHITE);
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSprite,RESDAILY_SWITCHBK);

	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_RESERVATION_SWITH_OFF); //關閉圖示
	pSprBtnIcon->setAnchorPoint(CCPointMake(0,1));	
	pSprBtnIcon->setPosition(CCPointMake(834*W_unit,pSprite->getContentSize().height*0.8));
	pSprBtnIcon->setVisible(true);
	pSprite->addChild(pSprBtnIcon,10);	
	m_BtnArray[RESPG_DAILY]->insertObject(pSprBtnIcon,RESDAILY_SWITCHBTN);

	pLabel = CCLabelTTF::create(STR_Reservation_Dailycleaning, S_FONE, *pSTR_TITAL_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setAnchorPoint(CCPointMake(0,1));	
	pLabel->setPosition(LabelSiteOnBtn_Lab1);
	pSprite->addChild(pLabel,10);

	pLabel = CCLabelTTF::create(STR_Reservation_Choosetime, S_FONE, *pSTR_RESERV_LABSIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_LIGHTBLACK);
	pLabel->setAnchorPoint(CCPointMake(0,1));	
	pLabel->setPosition(LabelSiteOnBtn_Lab2);
	pSprite->addChild(pLabel,10);



	//Set Start Time
	pSprite=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BK); //背景圖案
	pSprite->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(SpriteSite_SetTime);	
	pSpriteBK->addChild(pSprite,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSprite,RESDAILY_TIMEBK);

	pLabel = CCLabelTTF::create(STR_Reservation_SetStartTime, S_FONE, *pSTR_CLEANJOB_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabelSiteOnBtn_SetTime);
	pSprite->addChild(pLabel,10);
	//設定按紐位置
	CCSize TimeSetSize	= pSprite->boundingBox().size;
	float W_TimeSetunit = TimeSetSize.width/8;
	float H_TimeSetunit = TimeSetSize.height/4;

	CCPoint BtnSite_HourUP=CCPointMake(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit);
	CCPoint BtnSite_HourNow=CCPointMake(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*2);
	CCPoint BtnSite_HourDown=CCPointMake(3*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*3);
	CCPoint BtnSite_MinUP=CCPointMake(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit);
	CCPoint BtnSite_MinNow=CCPointMake(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*2);
	CCPoint BtnSite_MinDown=CCPointMake(5*W_TimeSetunit,TimeSetSize.height-H_TimeSetunit*3);

	//時間設定按鈕
	CCSprite *pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_HourUP);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEHOUR_UP_BTN);
	pLabel = CCLabelTTF::create("00", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEHOUR_UP_VALUE);


	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_HourNow);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEHOUR_NOW_BTN);
	pLabel = CCLabelTTF::create("01", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEHOUR_NOW_VALUE);


	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_HourDown);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEHOUR_DOWN_BTN);
	pLabel = CCLabelTTF::create("02", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEHOUR_DOWN_VALUE);


	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_MinUP);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEMIN_UP_BTN);
	pLabel = CCLabelTTF::create("00", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEMIN_UP_VALUE);


	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_MinNow);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEMIN_NOW_BTN);
	pLabel = CCLabelTTF::create("01", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEMIN_NOW_VALUE);


	pSpriteBtn=CCSprite::createWithSpriteFrameName(S_RESERVATION_SETTIME_BTN); //背景圖案	
	pSpriteBtn->setPosition(BtnSite_MinDown);
	pSpriteBtn->setColor(COLOR_STRING_LIGHTGREY);
	pSprite->addChild(pSpriteBtn,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteBtn,RESDAILY_TIMEMIN_DOWN_BTN);
	pLabel = CCLabelTTF::create("02", S_FONE, *pSTR_BTN_SIZE,CCSizeMake(0,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(CCPointMake(pSpriteBtn->boundingBox().size.width*0.5,pSpriteBtn->boundingBox().size.height*0.5));
	pSpriteBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_TIMEMIN_DOWN_VALUE);

	//時間提示
	pLabel = CCLabelTTF::create(STR_Reservation_Robotwillstartcleaning, S_FONE, *pSTR_CLEANJOB_SIZE,CCSizeMake(winSize.width,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0.5,0.5));
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setPosition(LabelSite_Lab1);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_LABTIME);

	pLabel = CCLabelTTF::create("last 09:00", S_FONE, *pSTR_CLEANJOB_SIZE,CCSizeMake(winSize.width,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
	pLabel->setAnchorPoint(CCPointMake(0.5,0.5));
	pLabel->setColor(COLOR_STRING_RED);
	pLabel->setPosition(LabelSite_Lab2);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pLabel,RESDAILY_VALUETIME);



	
	//底下兩個按鈕
	//底部按鈕
	CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0,0));
	pSpriteDownBtn->setScaleX(0.5);
	pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	pSpriteBK->addChild(pSpriteDownBtn);

	pLabel = CCLabelTTF::create(STR_BTN_Canel, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setScaleX(2);
	pLabel->setPosition(ccp(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteDownBtn,RESDAILY_BTNCANEL);

	pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteDownBtn->setPosition(ccp(0+winSize.width*0.5+4,0)); //+4 FOR 中間線
	pSpriteDownBtn->setScaleX(0.5);
	pSpriteDownBtn->setColor(COLOR_STRING_GREY);
	pSpriteBK->addChild(pSpriteDownBtn);
	pLabel = CCLabelTTF::create(STR_BTN_Save, S_FONE, *pSTR_BTN_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	pLabel->setScaleX(2);
	pLabel->setPosition(ccp(pSpriteDownBtn->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height/2));
	pSpriteDownBtn->addChild(pLabel,10);
	m_BtnArray[RESPG_DAILY]->insertObject(pSpriteDownBtn,RESDAILY_BTNSAVE);
}

//根據設定檔 修改顯示畫面
void UI_Reservation::ResetUI_RESDAILYByIni()
{
	Setting_Reservation *pData=&ReserSetting;
	if(pData->DailyOn==1)
	{
		//CCLabelTTF* pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_SWITCHOFF);
		//pLabel ->setVisible(false);
		//pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_SWITCHON);
		//pLabel ->setVisible(true);

		CCSprite* pSprite=(CCSprite*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_TIMEBK);
		pSprite->setVisible(true);
		pSprite=(CCSprite*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_SWITCHBTN);
		CCSpriteFrame *pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_RESERVATION_SWITH_ON);
		pSprite->setDisplayFrame(pFrame);


		CCLabelTTF* pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_LABTIME);
		pLabel ->setVisible(true);

		pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_VALUETIME);
		pLabel ->setVisible(true);

	}
	else
	{
		//CCLabelTTF* pLabel =(CCLabelTTF*) m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_SWITCHOFF);
		//pLabel ->setVisible(true);
		//pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_SWITCHON);
		//pLabel ->setVisible(false);
		CCSprite* pSprite=(CCSprite*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_TIMEBK);
		pSprite->setVisible(false);
		pSprite=(CCSprite*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_SWITCHBTN);
		CCSpriteFrame *pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_RESERVATION_SWITH_OFF);
		pSprite->setDisplayFrame(pFrame);

		CCLabelTTF* pLabel =(CCLabelTTF*) m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_LABTIME);
		pLabel ->setVisible(false);
		pLabel =(CCLabelTTF*) m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_VALUETIME);
		pLabel ->setVisible(false);
	}

}

//檢查設定是否一致
void UI_Reservation::ResetUI_RESDAILYCheckSaveBtn()
{
	Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
	if(memcmp(pData,&ReserSetting,sizeof(ReserSetting))==0)
	{
		CCSprite* pSprite=(CCSprite*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_BTNSAVE);
		pSprite->setColor(COLOR_STRING_GREY);
		bAllowSave=false;
	}
	else
	{
		CCSprite* pSprite=(CCSprite*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_BTNSAVE);
		pSprite->setColor(COLOR_STRING_GREEN);
		bAllowSave=true;
	}
}

void UI_Reservation::ResetUI_RESDAILYTime()
{ //根據預約時間修改
	int iShowTime=ReserSetting.DailyTime;
	int iHout = iShowTime/60;
	int iMin = iShowTime%60;
	CCLabelTTF *pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_TIMEHOUR_UP_VALUE);
	char sTimeStr[20];
	if(iHout<0 || iHout>24)
		iHout=0;
	if(iHout==0)
		sprintf(sTimeStr,"23");		
	else
		sprintf(sTimeStr,"%.02d",iHout-1);
	pLabel->setString(sTimeStr);


	pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_TIMEHOUR_NOW_VALUE);
	sprintf(sTimeStr,"%.02d",iHout);
	pLabel->setString(sTimeStr);

	pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_TIMEHOUR_DOWN_VALUE);
	if(iHout==23)
		sprintf(sTimeStr,"00");
	else
		sprintf(sTimeStr,"%.02d",iHout+1);
	pLabel->setString(sTimeStr);	


	pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_TIMEMIN_UP_VALUE);
	if(iMin<0 || iMin>59)
		iMin=0;
	if(iMin==0)
		sprintf(sTimeStr,"59");		
	else
		sprintf(sTimeStr,"%.02d",iMin-1);
	pLabel->setString(sTimeStr);


	pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_TIMEMIN_NOW_VALUE);
	sprintf(sTimeStr,"%.02d",iMin);
	pLabel->setString(sTimeStr);

	pLabel = (CCLabelTTF*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_TIMEMIN_DOWN_VALUE);
	if(iMin==59)
		sprintf(sTimeStr,"00");
	else
		sprintf(sTimeStr,"%.02d",iMin+1);
	pLabel->setString(sTimeStr);	


	pLabel =(CCLabelTTF*) m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_VALUETIME);
	int iLastTime=iShowTime-GetRobotTimeMinuate();
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
	CCSprite* pSprite= (CCSprite*)m_BtnArray[RESPG_MAIN]->objectAtIndex(RESMAIN_BK);
	pSprite->setVisible(true);
	//ResetSettingByRobotSchedule(); //根據ROBOT 設定 修改本機設定
	Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser); //讀取本機設定
	memcpy((void*)&ReserSetting,pData,sizeof(Setting_Reservation));

	ResetUI_RESMAINLab();
	//GetSchedule(); //取得預約時間
	PageType = RESPG_MAIN;
}

void UI_Reservation::Hide_RESMAIN()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[RESPG_MAIN]->objectAtIndex(RESMAIN_BK);
	pSprite->setVisible(false);
}

void UI_Reservation::Show_RESSINGAL()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_BK);
	pSprite->setVisible(true);	
	ResetUI_RESSINGALByIni();
	ResetUI_RESSINGALCheckSaveBtn();
	ResetUI_RESSINGALTime();
	PageType = RESPG_SINGAL;
}

void UI_Reservation::Hide_RESSINGAL()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[RESPG_SINGAL]->objectAtIndex(RESSINGAL_BK);
	pSprite->setVisible(false);
}

void UI_Reservation::Show_RESDAILY()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_BK);
	pSprite->setVisible(true);
	//Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
	//memcpy((void*)&ReserSetting,pData,sizeof(Setting_Reservation));
	ResetUI_RESDAILYByIni();
	ResetUI_RESDAILYCheckSaveBtn();
	ResetUI_RESDAILYTime();
	PageType = RESPG_DAILY;
}

void UI_Reservation::Hide_RESDAILY()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[RESPG_DAILY]->objectAtIndex(RESDAILY_BK);
	pSprite->setVisible(false);
}


//////////////////////////////////////////////////////////////////////////
void UI_Reservation::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

//滑鼠點下!判斷是那個元件
bool UI_Reservation::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	if(!isVisible()) //有顯示才繼續
		return false;
	switch(PageType)
	{
	default:
		break;
	case RESPG_SINGAL:
		{
			//判斷那一個元件
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);				
			}  
		}
		break;
	case RESPG_MAIN:
		{
			//判斷那一個元件
			for (int i = RESMAIN_SINGAL; i < m_BtnArray[PageType]->count(); i+=RESMAIN_BTNSPACENUM)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				BtnColorSetting(i,bTouch);
			}  
		}
		break;
	}
		return true;
}

void UI_Reservation::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
}

void UI_Reservation::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	switch(PageType)
	{
	default:
		break;
	case RESPG_DAILY:
		{
			//判斷那一個元件
			bool bTimeBtn = false;
			bool bTouch = false;
			CCPoint nodePositionByTimeBK;
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				if(bTimeBtn)
				{
					bTouch=pSprite ->boundingBox().containsPoint(nodePositionByTimeBK);				
				}
				else if(i<RESDAILY_TIME_START || i > RESDAILY_TIME_END)
					bTouch=pSprite ->boundingBox().containsPoint(nodePosition);				
				else
					bTouch=false;

				if(bTouch)
				{
					if(i==RESDAILY_TIMEBK) //中間的時間設定
					{
						bTimeBtn=true;
						nodePositionByTimeBK.x=nodePosition.x-pSprite->boundingBox().origin.x;
						nodePositionByTimeBK.y=nodePosition.y-pSprite->boundingBox().origin.y;
					}
					int iChanglayer=Function_RESDAILY(i); 
					if(iChanglayer==RESDAILY_TITAL ) //退回上一頁
						break;
				}

			}  
		}
		break;

	case RESPG_SINGAL:
		{
			//判斷那一個元件
			bool bTimeBtn = false;
			bool bTouch = false;
			CCPoint nodePositionByTimeBK;
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				if(bTimeBtn)
				{
					if(i>=RESSINGAL_TIME_START && i <= RESSINGAL_TIME_END)
						bTouch=pSprite ->boundingBox().containsPoint(nodePositionByTimeBK);				
					else
						bTouch=false;
				}
				else if(i<RESSINGAL_TIME_START || i > RESSINGAL_TIME_END)
					bTouch=pSprite ->boundingBox().containsPoint(nodePosition);				
				else
					bTouch=false;
				if(bTouch)
				{
					if(i==RESSINGAL_TIMEBK) //中間的時間設定
					{
						bTimeBtn=true;
						nodePositionByTimeBK.x=nodePosition.x-pSprite->boundingBox().origin.x;
						nodePositionByTimeBK.y=nodePosition.y-pSprite->boundingBox().origin.y;
					}
					int iChanglayer=Function_RESSINGAL(i); 
					if(iChanglayer==RESSINGAL_TITAL) //退回上一頁
						break;
				}

			}  
		}
		break;
	case RESPG_MAIN:
		{
			//判斷那一個元件
			int iType=RESPG_MAIN;
			for (int i = 1; i < m_BtnArray[iType]->count(); i++) 
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[iType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch)
				{
					int iChanglayer=Function_RESMAIN(i); 
					if(iChanglayer==RESMAIN_TITAL)
						break;
				}
//				if(i==RESMAIN_SINGAL || i==RESMAIN_DAILY)
//					BtnColorSetting(i,bTouch);
			}  
		}
		break;
	}


	
}

//按鈕變色
void UI_Reservation::BtnColorSetting(int iBtnIdx,bool bSel)
{
	if(bSel)
	{
		CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(iBtnIdx); 
		pSprite ->setColor(COLOR_STRING_GREEN);
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

//按下事件處理
int  UI_Reservation::Function_RESMAIN(int iBtnIdx)
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
		ChangLayer_Main();
		return 1;
		break;
	}
	return 0;
}

//按下事件處理
int  UI_Reservation::Function_RESSINGAL(int iBtnIdx)
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
			Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
			memcpy((void*)&ReserSetting,pData,sizeof(Setting_Reservation));
			bChang=true;
			bChangTime =true ;
		}
		break;
	case RESSINGAL_BTNSAVE:
		{
			if(bAllowSave)
			{
				SaveSetting(Setting_Reser,(void*)&ReserSetting);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
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
		Hide_AllPage();
		Show_RESMAIN();
		return 1;
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
	return 0;
}

//按下事件處理
int  UI_Reservation::Function_RESDAILY(int iBtnIdx)
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
			Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
			memcpy((void*)&ReserSetting,pData,sizeof(Setting_Reservation));
			bChang=true;
			bChangTime=true;
		}
		break;
	case RESDAILY_BTNSAVE:
		{
			if(bAllowSave)
			{
				SaveSetting(Setting_Reser,(void*)&ReserSetting);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
				SaveSetting_Reservation();
#endif
				SetSchedule(); //傳送資料到ROBOT
				bChang=true;
				bReturnMain=true;
			}
		}
		break;
	case RESDAILY_TITAL:
		Hide_AllPage();
		Show_RESMAIN();
		return RESDAILY_TITAL;
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
	return 0;
}

void UI_Reservation::SaveTimeToSchedule(int iType)
{
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

}

void UI_Reservation::SaveRobotSachedule(void* pData)
{	
	if(iScheduleTotalNumber<0)
		iScheduleTotalNumber=0;
	iScheduleTotalNumber++; //紀錄總共數量
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

	iScheduleCount++;
	if(iScheduleCount>=G_ROBOT_SCHEDULE_MAX) //數量過多 覆蓋之前舊的
		iScheduleCount=0;	
	if(iScheduleTotalNumber>G_ROBOT_SCHEDULE_MAX) //只有取100個
		iScheduleTotalNumber=G_ROBOT_SCHEDULE_MAX;
}

void* UI_Reservation::AddRobotSachedule(int iNameSize)
{
	int iSpaceIdx=iScheduleCount;
	iScheduleTotalNumber++; //紀錄總共數量
	iScheduleCount++;
	if(iScheduleCount>=G_ROBOT_SCHEDULE_MAX) //數量過多 覆蓋之前舊的
		iScheduleCount=0;	
	if(iScheduleTotalNumber>G_ROBOT_SCHEDULE_MAX) //只有取100個
		iScheduleTotalNumber=G_ROBOT_SCHEDULE_MAX;
	//釋放舊空間
	if ( stRobotShedule[iSpaceIdx].job.map_index_count!=0 )
		free(stRobotShedule[iSpaceIdx].job.map_index);
	if ( stRobotShedule[iSpaceIdx].job.data_count!=0  )
		free(stRobotShedule[iSpaceIdx].job.data);
	if ( stRobotShedule[iSpaceIdx].job.name_count!=0 )
		free(stRobotShedule[iSpaceIdx].job.name);

	return (void*)&stRobotShedule[iSpaceIdx];
}

//根據機器的Schedule 修改現有設定
void UI_Reservation::ResetSettingByRobotSchedule()
{
	Setting_Reservation *pData=(Setting_Reservation *)GetSetting(Setting_Reser);
	player_dispatcher_ui_alarm_time_t *pAlarm;
	if(iScheduleTotalNumber>=0) //ROBOT裡沒有任何紀錄 若為-1 不做任何動作
	{
		memset(pData,0,sizeof(Setting_Reservation)); //清空InI
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
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
}

//ROBOT 設定預約掃地
void UI_Reservation::SetSchedule()
{
	
	uint8_t initiator;
	pDisProxy->GetReqUiSchedule(&initiator);
	SaveTimeToSchedule(RESPGALARM_ONCE);
	SaveTimeToSchedule(RESPGALARM_DAILY);
	//pDisProxy->SetReqAckUiSchedule(initiator, RESPGALARM_SIZE, &UiSchedule[0]);
	pDisProxy->SetUiSchedule(PLAYER_DISPATCH_INIT_DEST_RD, iScheduleTotalNumber, &stRobotShedule[0]);
}

//取得ROBOT 設定 (不使用了 改為ReleqaseUI)
void UI_Reservation::GetSchedule()
{
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


}