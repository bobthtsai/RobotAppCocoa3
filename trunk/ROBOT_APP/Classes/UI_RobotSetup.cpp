#include <ImageList.h>
#include <HelloWorldScene.h>
#include <libplayerc++/playerc++.h>
#include "Coco2dDialog.h"
USING_NS_CC;
using namespace std;
using namespace PlayerCc;

extern HelloWorld* m_MainView; 
extern UI_RobotMain* m_pUIRobotMain;
/// Player Client
extern PlayerClient *pPlayerclient;
extern DispatcherProxy *pDisProxy;

UI_RobotSetup::UI_RobotSetup()
{
	PageType=SETUPPG_LAYER;

}
UI_RobotSetup::~UI_RobotSetup()
{
	for(int i=0;i<SETUPPG_SIZE;i++)
		m_BtnArray[i]->release();
}

bool UI_RobotSetup::init()
{  
	if (!CCLayer::init()) 
	{  
		return false;  
	}  
	InitUI();  
	return true;  
}  

void UI_RobotSetup::InitUI()
{
	winSize = CCDirector::sharedDirector()->getVisibleSize(); //取得視窗大小
	InitUI_RobotSetupMAIN();
	InitUI_RobotSetupNotif();
}
void UI_RobotSetup::InitUI_RobotSetupMAIN()
{
	m_BtnArray[SETUPPG_MAIN] = CCArray::createWithCapacity(SETUPMAIN_SIZE); //按鈕存放
	m_BtnArray[SETUPPG_MAIN]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint LabeSite_RobotModel=CCPointMake(45*W_unit,winSize.height-250*H_unit);
	CCPoint LabeSite_RobotModel_Value=CCPointMake(winSize.width*0.5,winSize.height-250*H_unit);
	CCPoint LabeSite_RobotVersion=CCPointMake(45*W_unit,winSize.height-345*H_unit);
	CCPoint LabeSite_RobotVersion_Vaule=CCPointMake(winSize.width*0.5,winSize.height-345*H_unit);
	CCPoint LabeSite_ApplicationVersion=CCPointMake(45*W_unit,winSize.height-440*H_unit);
	CCPoint LabeSite_ApplicationVersion_Value=CCPointMake(winSize.width*0.5,winSize.height-440*H_unit);

	CCPoint BtnSite_Name=CCPointMake(0*W_unit,winSize.height-540*H_unit);
	CCPoint BtnSite_HomeAP=CCPointMake(0*W_unit,winSize.height-730*H_unit);
	CCPoint BtnSite_SyncTime=CCPointMake(0*W_unit,winSize.height-(730+144*1)*H_unit);
	CCPoint BtnSite_Notification=CCPointMake(0*W_unit,winSize.height-(730+144*2)*H_unit);
	CCPoint BtnSite_Log=CCPointMake(0*W_unit,winSize.height-(730+144*3)*H_unit);
	CCPoint BtnSite_Help=CCPointMake(0*W_unit,winSize.height-(1346)*H_unit);
	CCPoint BtnSite_Legal=CCPointMake(0*W_unit,winSize.height-(1346+144)*H_unit);
	CCPoint BtnSite_Disconnection=CCPointMake(0*W_unit,winSize.height-1674*H_unit);

	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	m_BtnArray[SETUPPG_MAIN]->addObject(pSpriteBK);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_Setting, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_TITAL);
	//Robot F/W infornation
	//MODEL
	pLabel = CCLabelTTF::create(S_RobotSetup_RobotModel, S_FONE, *pSTR_SETUP_SIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_RobotModel);
	pSpriteBK->addChild(pLabel,10);

	pLabel = CCLabelTTF::create(S_RobotSetup_RobotModel, S_FONE, *pSTR_SETUP_SIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(LabeSite_RobotModel_Value);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pLabel,SETUPMAIN_MODEL);

	//VERSION
	pLabel = CCLabelTTF::create(S_RobotSetup_RobotVersion, S_FONE, *pSTR_SETUP_SIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_RobotVersion);
	pSpriteBK->addChild(pLabel,10);

	pLabel = CCLabelTTF::create(S_RobotSetup_RobotVersion, S_FONE, *pSTR_SETUP_SIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(LabeSite_RobotVersion_Vaule);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pLabel,SETUPMAIN_VERSION);

	//APP VERSION
	pLabel = CCLabelTTF::create(S_RobotSetup_ApplicationVersion, S_FONE, *pSTR_SETUP_SIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabeSite_ApplicationVersion);
	pSpriteBK->addChild(pLabel,10);

	pLabel = CCLabelTTF::create(S_RobotSetup_ApplicationVersion, S_FONE, *pSTR_SETUP_SIZE,CCSizeMake(winSize.width*0.5,0),kCCTextAlignmentLeft);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(LabeSite_ApplicationVersion_Value);
	pSpriteBK->addChild(pLabel,10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pLabel,SETUPMAIN_APPVER);

	//按鈕設定 Name
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPointMake(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Name);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(CCPointMake(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = CCLabelTTF::create(S_RobotSetup_Name, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(CCPointMake(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	pLabel = CCLabelTTF::create(S_RobotSetup_Name, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(CCPointMake(pSpriteAction->getContentSize().width*0.5,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pLabel,SETUPMAIN_NAMELAB);
	
	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_NAME);
	//Home AP
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPointMake(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_HomeAP);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = CCLabelTTF::create(S_RobotSetup_HomeAP, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_HOMEAP);

	//Sync Time
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPointMake(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_SyncTime);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = CCLabelTTF::create(S_RobotSetup_SyncTime, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_SYNCTIME);

	//Notification
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPointMake(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Notification);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = CCLabelTTF::create(S_RobotSetup_Notification, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_NOTIFICATION);

	//Log
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPointMake(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Log);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = CCLabelTTF::create(S_RobotSetup_Log, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_LOG);

	//HELP
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPointMake(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Help);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = CCLabelTTF::create(S_RobotSetup_Help, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_HELP);

	//Legal
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPointMake(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Legal);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = CCLabelTTF::create(S_RobotSetup_Legal, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_LEGAL);

	//Disconnection
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPointMake(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_Disconnection);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_FORWARD_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(winSize.width-pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = CCLabelTTF::create(S_RobotSetup_Disconnection, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_MAIN]->insertObject(pSpriteAction,SETUPMAIN_DISCONNECTION);
}

void UI_RobotSetup::InitUI_RobotSetupNotif()
{
	m_BtnArray[SETUPPG_NOTIF] = CCArray::createWithCapacity(SETUPNOTIF_SIZE); //按鈕存放
	m_BtnArray[SETUPPG_NOTIF]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	CCPoint BtnSite_SyncTime=CCPointMake(0*W_unit,winSize.height-(10+144*1)*H_unit);
	CCPoint BtnSite_CleanJob=CCPointMake(0*W_unit,winSize.height-(10+144*2)*H_unit);

	//設定背景
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //設定錨點
	addChild(pSpriteBK); //加入 layer 層	
	m_BtnArray[SETUPPG_NOTIF]->addObject(pSpriteBK);

	//上方標題列
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //設定錨點
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(S_RobotSetup_Notification, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_NOTIF]->insertObject(pSpriteAction,SETUPNOTIF_TITAL);

	//TIMESYNC BTN
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPointMake(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_SyncTime);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_RESERVATION_SWITH_OFF); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(winSize.width-pSprBtnIcon->getContentSize().width/2-20,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = CCLabelTTF::create(S_RobotSetupNotif_TIPSYN, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_NOTIF]->insertObject(pSpriteAction,SETUPNOTIF_TIMESYNC_BTN);
	m_BtnArray[SETUPPG_NOTIF]->insertObject(pSprBtnIcon,SETUPNOTIF_TIMESYNC_CHECK);

	//CleanJOB BTN
	pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPointMake(0,1)); //設定錨點
	pSpriteAction->setPosition(BtnSite_CleanJob);
	pSpriteBK->addChild(pSpriteAction);
	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_RESERVATION_SWITH_OFF); //按鈕圖示
	pSprBtnIcon->setPosition(ccp(winSize.width-pSprBtnIcon->getContentSize().width/2-20,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	pLabel = CCLabelTTF::create(S_RobotSetupNotif_STOPINTO, S_FONE, *pSTR_SETUP_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK2);
	pLabel->setPosition(ccp(pLabel->getContentSize().width/2+45*W_unit,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SETUPPG_NOTIF]->insertObject(pSpriteAction,SETUPNOTIF_CLEANJOB_BTN);
	m_BtnArray[SETUPPG_NOTIF]->insertObject(pSprBtnIcon,SETUPNOTIF_CLEANJOB_CHECK);
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
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SETUPPG_MAIN]->objectAtIndex(SETUPMAIN_BK);
	pSprite->setVisible(true);
	ReleasePage_SetupMAIN();
	PageType = SETUPPG_MAIN;
}
void UI_RobotSetup::Hide_RobotSetupMAIN()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SETUPPG_MAIN]->objectAtIndex(SETUPMAIN_BK);
	pSprite->setVisible(false);
}
void UI_RobotSetup::Show_RobotSetupNOTIF()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SETUPPG_NOTIF]->objectAtIndex(SETUPMAIN_BK);
	pSprite->setVisible(true);
	PageType = SETUPPG_NOTIF;
}
void UI_RobotSetup::Hide_RobotSetupNOTIF()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SETUPPG_NOTIF]->objectAtIndex(SETUPMAIN_BK);
	pSprite->setVisible(false);

}

void UI_RobotSetup::ReleasePage_SetupMAIN()
{
	//ROBOT NAME
	const char* pName=m_pUIRobotMain->GetRobotName();
	CCLabelTTF* pLabel = (CCLabelTTF*)m_BtnArray[SETUPPG_MAIN]->objectAtIndex(SETUPMAIN_NAMELAB);
	pLabel->setString(pName);
	//APP VERSION
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	pName=eglView->getAppVerSion();
	pLabel = (CCLabelTTF*)m_BtnArray[SETUPPG_MAIN]->objectAtIndex(SETUPMAIN_APPVER);
	pLabel->setString(pName);
}
void UI_RobotSetup::ReleasePage_Notifo()
{//根據設定改變按鈕圖示
	CCSprite* pSprite=(CCSprite*)m_BtnArray[SETUPPG_NOTIF]->objectAtIndex(SETUPNOTIF_TIMESYNC_CHECK);
	if(CheckSyncTimeAlarmSetting())
	{		
		CCSpriteFrame *pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_RESERVATION_SWITH_ON);
		pSprite->setDisplayFrame(pFrame);
	}
	else
	{
		CCSpriteFrame *pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_RESERVATION_SWITH_OFF);
		pSprite->setDisplayFrame(pFrame);
	}

	pSprite=(CCSprite*)m_BtnArray[SETUPPG_NOTIF]->objectAtIndex(SETUPNOTIF_CLEANJOB_CHECK);
	if(CheckCheckCJobAlarmSetting())
	{		
		CCSpriteFrame *pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_RESERVATION_SWITH_ON);
		pSprite->setDisplayFrame(pFrame);
	}
	else
	{
		CCSpriteFrame *pFrame= CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(S_RESERVATION_SWITH_OFF);
		pSprite->setDisplayFrame(pFrame);
	}

}
//////////////////////////////////////////////////////////////////////////
void UI_RobotSetup::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
//滑鼠點下!判斷是那個元件
bool UI_RobotSetup::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );


	if(isVisible()) //有顯示才繼續
		return true;
	else
		return false;
}
void UI_RobotSetup::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
}
void UI_RobotSetup::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	switch(PageType)
	{
	default:
		break;
	case SETUPPG_NOTIF:
		{
			//判斷那一個元件
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch)
				{
					Function_UINOTIF(i);
					break;
				}
			}  
		}
		break;
	case SETUPPG_MAIN:
		{
			//判斷那一個元件
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch)
				{
					Function_UISETUP(i);
					break;
				}
			}  
		}
		break;
	}
}
void UI_RobotSetup::MsgBtnCallback(cocos2d::CCNode *pNode)
{
	if(pNode->getTag()==1)
	{//CCMessageBox(STR_BTN_Ok,"CLOSE");
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
	//pl->setContentSize(CCSizeMake(winSize.width*0.7, winSize.height*0.4)); //須改為根據解析度變更
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
			SavePcTimeToRobotTime();
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
			int iType=CheckCheckCJobAlarmSetting();
			if(iType==0)
				iType=1;
			else
				iType=0;
			SetCleanJobAlarmSettingONOFF(iType);
			SaveSetting_TipMsg();
			ReleasePage_Notifo();
		}
		break;
	case SETUPNOTIF_TIMESYNC_BTN:
	case SETUPNOTIF_TIMESYNC_CHECK:
		{ //改變TIME SYNC  設定
			int iType=CheckSyncTimeAlarmSetting();
			if(iType==0)
				iType=1;
			else
				iType=0;
			SetSyncTimeAlarmSettingONOFF(iType);
			SaveSetting_TipMsg();
			ReleasePage_Notifo();
		}
		break;
	}
}



//////////////////////////////////////////////////////////////////////////
//Robot Sacnning View
