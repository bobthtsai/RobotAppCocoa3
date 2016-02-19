#include <ImageList.h>
#include <HelloWorldScene.h>
#include <libplayerc++/playerc++.h>
#include "player.h"
#include "MAP_Function.h"
USING_NS_CC;
using namespace std;
using namespace PlayerCc;


extern HelloWorld* m_MainView; 
//extern UI_MAP* m_pUIRobotMap; 
extern PlayerClient *pPlayerclient;
 
//extern MapProxy *pMapProxy;
//extern PlannerProxy *pPlannerProxy;


UI_MAP::UI_MAP()
{
	PageType=MAPPG_LAYER;
	fZoomRate=GUIMAPSize_DEFRATE;
	bMoveKeep=false;
	pMapTexture=NULL;
	iRelseTime=0;
}
UI_MAP::~UI_MAP()
{
	for(int i=0;i<MAPPG_SIZE;i++)
		m_BtnArray[i]->release();
	//ReleaseMapData();
	FreeImageData();
	if(pMapTexture!=NULL)
	{
	pMapTexture->release();
	}
	GuiData_Free();
}

bool UI_MAP::init()
{  
	if (!CCLayer::init()) 
	{  
		return false;  
	}  
	InitUI();  
	return true;  
}  
//�C10�� ��s�@���a��
void UI_MAP::UpdateMAPSchedule(float dt)
{
	if(IsRobotMapReady())
	{ // �w����s�a��
		RobotMapUpdate();
	}
	//iRobotTime++;
	
	if(WaittryRobotMapUpdate()==0)
	//WaitRobotMapUpdate();	
	{ // ����s���T��,��s�a�� ���sø�s
		ReloadRobotMap();
		iRelseTime=0;
	}
//#ifdef DEBUG //�W�[��s���
	CCLabelAtlas *pTimeLab=(CCLabelAtlas*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_RESTIME);
	char sTemp[20];
	sprintf(sTemp,"%d",iRelseTime);
	pTimeLab->setString(sTemp);	
	
	//CCLabelAtlas::labelWithString("0","JW_M_sk_bo_n3.png",30,40,'0');
	//m_pFPSLabel->setIgnoreContentScaleFactor(true);
	//m_pFPSLabel->initWithString("00.0", texture, 12, 32 , '.');
	//m_pFPSLabel->setScale(factor);

//#endif
	
}
void UI_MAP::InitUI()
{
	winSize = CCDirector::sharedDirector()->getVisibleSize(); //���o�����j�p
	InitUI_MAPMAIN();


}
void UI_MAP::InitUI_MAPMAIN()
{
	m_BtnArray[MAPPG_MAIN] = CCArray::createWithCapacity(MAPMAIN_SIZE); //���s�s��
	m_BtnArray[MAPPG_MAIN]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;
	CCPoint SpriteSite_MAPBK=CCPointMake(10*W_unit,winSize.height-218*H_unit);	
	CCPoint IconSite_ZoomIn=CCPointMake(922*W_unit,winSize.height-238*H_unit);		
	CCPoint IconSite_ZoomOut=CCPointMake(782*W_unit,winSize.height-238*H_unit);		
	CCPoint LabSite_ZoomRate=CCPointMake(550*W_unit,winSize.height-238*H_unit);		
	CCPoint IconSite_ReturnZero=CCPointMake(1000*W_unit,winSize.height-1800*H_unit);		

	//�]�w�I��
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	addChild(pSpriteBK); //�[�J layer �h	
	m_BtnArray[MAPPG_MAIN]->addObject(pSpriteBK);

	//�W����D�C
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction,10);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //���s�ϥ�
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_CoverageMap, S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pSpriteAction->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);

	pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_MAP_HISTORY_DEF); //�y��ϥ�
	pSprBtnIcon->setPosition(ccp(pSpriteAction->getContentSize().width-pSprBtnIcon->getContentSize().width*0.7,winSize.height-pSpriteAction->getContentSize().height/2));
	pSpriteBK->addChild(pSprBtnIcon,12);
	m_BtnArray[MAPPG_MAIN]->insertObject(pSprBtnIcon,MAPMAIN_HISTORY);
	m_BtnArray[MAPPG_MAIN]->insertObject(pSpriteAction,MAPMAIN_TITAL);


	//ZOOM IN OUT
	CCSprite *pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_OUT_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomOut);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[MAPPG_MAIN]->insertObject(pSprite,MAPMAIN_ZOOMOUT);
	pSprite=CCSprite::createWithSpriteFrameName(S_CLEAN_ZOOM_IN_DEF); 
	pSprite->setAnchorPoint(CCPointMake(0,1));
	pSprite->setPosition(IconSite_ZoomIn);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[MAPPG_MAIN]->insertObject(pSprite,MAPMAIN_ZOOMIN);	
	//�Y�񭿲v
	pLabel = CCLabelTTF::create("X 1.0", S_FONE, *pSTR_TITAL_SIZE);
	pLabel->setAnchorPoint(CCPointMake(0,1));
	pLabel->setColor(COLOR_STRING_GREEN);
	pLabel->setPosition(LabSite_ZoomRate);
	pSpriteBK->addChild(pLabel, 12);
	m_BtnArray[MAPPG_MAIN]->insertObject(pLabel,MAPMAIN_ZOOMLAB);	
	
	//�^�k���I
	pSprite=CCSprite::createWithSpriteFrameName(S_MAP_RETURN_ZERO_DEF); 
	pSprite->setPosition(IconSite_ReturnZero);
	pSpriteBK->addChild(pSprite,12);
	m_BtnArray[MAPPG_MAIN]->insertObject(pSprite,MAPMAIN_RETIRNZERO);
//#ifdef DEBUG //�W�[��s���
	CCLabelAtlas *pTimeLab=CCLabelAtlas::create("02",IMAGELIST_UPDATE_TIME_NUMBER,35,40,'0');
	pTimeLab->setPosition(CCPointMake(LabSite_ZoomRate.x-100,LabSite_ZoomRate.y));
	pSpriteBK->addChild(pTimeLab,12);
	m_BtnArray[MAPPG_MAIN]->insertObject(pTimeLab,MAPMAIN_RESTIME);
	//CCLabelAtlas::labelWithString("0","JW_M_sk_bo_n3.png",30,40,'0');
	//m_pFPSLabel->setIgnoreContentScaleFactor(true);
	//m_pFPSLabel->initWithString("00.0", texture, 12, 32 , '.');
	//m_pFPSLabel->setScale(factor);

//#endif
	//�����H�ϥ�
	pSprite=CCSprite::createWithSpriteFrameName(S_MAP_ROBOT); 
	pSprite->setPosition(IconSite_ReturnZero);
	pSpriteBK->addChild(pSprite,11);
	m_BtnArray[MAPPG_MAIN]->insertObject(pSprite,MAPMAIN_ROBOT);
	//�a��
	CCSprite *pSpriteMap=CCSprite::createWithSpriteFrameName(S_CLEAN_MAP_BG);
	pSpriteMap->setAnchorPoint(CCPoint(0,1)); //�]�w���I
	pSpriteMap->setPosition(SpriteSite_MAPBK);
	pSpriteBK->addChild(pSpriteMap,9);
	pSpriteMap->setColor(ccRED);
	m_BtnArray[MAPPG_MAIN]->insertObject(pSpriteMap,MAPMAIN_MAPBK);


	//�������s
	//CCSprite *pSpriteDownBtn=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	//pSpriteDownBtn->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	//pSpriteDownBtn->setPosition(ccp(0,0));
	//pSpriteDownBtn->setColor(COLOR_STRING_GREEN);
	//pSpriteBK->addChild(pSpriteDownBtn);
	//pSprite=CCSprite::createWithSpriteFrameName(S_ROBOT_BTN_VOICE_DEF); //�n���ϥ�
	//pSprite->setPosition(ccp(pSpriteBK->getContentSize().width*0.5,pSpriteDownBtn->getContentSize().height*0.5));
	//pSpriteDownBtn->addChild(pSprite,10);
	//m_BtnArray[MAPPG_MAIN]->insertObject(pSprite,MAPMAIN_CENTERICON);


}
static int MAPUpdateTime=0;

void UI_MAP::update(float dt)
{
	
//#ifdef DEBUG //�W�[��s���
	if(MAPUpdateTime%60==0) //�C��I�s�@��
	{
		iRelseTime++;
		CCLabelAtlas *pTimeLab=(CCLabelAtlas*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_RESTIME);
		if(pTimeLab!=NULL)
		{
			char sTemp[20];
			sprintf(sTemp,"%d",iRelseTime);
			pTimeLab->setString(sTemp);	

		}

	}
	MAPUpdateTime++;
//#endif
	
}
void UI_MAP::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true);
	Show_MapMain();
	schedule(schedule_selector(UI_MAP::UpdateMAPSchedule),G_ROBOTMAP_RELEASE_TIME);     //�Ұ�update() �C10��1��
	//resumeSchedulerAndActions();
	scheduleUpdate();     //�Ұ�update()
}
void UI_MAP::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();

	unschedule(schedule_selector(UI_MAP::UpdateMAPSchedule));     //�Ұ�update() �C10��1��
	unscheduleUpdate();            //�����q�{�w�ɾ�
	//pauseSchedulerAndActions ();
}
void UI_MAP::Hide_AllPage()
{
	Hide_MapMain();
	PageType = MAINPG_LAYER;
}


void UI_MAP::Show_MapMain()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_BK);
	pSprite->setVisible(true);
	PageType = MAPPG_MAIN;

	RobotMapUpdate(); //�j��h�a�ϧ�s
	if(!IsRobotMapReady()) //�즸����@�w�n���ݧ�s
		WaitRobotMapUpdate();

	//DEBUG �j���X����
	//pMapProxy->RequestMap(0, 0, stMapInfo.width, stMapInfo.height, false);	
	//pMapProxy->GetMap(pMapData);
	//SaveGUIMAPToMap(); //(OLD)
	//SaveGUIMAP();
	//DEBUG �ˬd�O�_���`
	//	int iSize=	pMapProxy->GetDataRange(); // (default: empty = -1, unknown = 0, occupied = +1). 
	//pMapProxy->Save();
	//if(IsMapDataReady()) //���J�ϧ�(OLD)

	if(IsGUIMAPReady()) //���J�ϧ�
	{
		ReloadRobotMap();	//��ܦa��
		MAPZoomDefault();	//�^�_�w�]�j�p
		MAPRetunZero();		//�a�Ϧ^�줤��
		MAPZoomLabUpdate();  //��s���v�r��
	}
}
void UI_MAP::ReloadRobotMap()
{
	CCSprite* pSpriteMap= (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_MAPBK);
	if(pMapTexture!=NULL)
		pMapTexture->release();
	pMapTexture = new CCTexture2D();		
	bool val = pMapTexture->initWithData(GetGUIMAP(),kCCTexture2DPixelFormat_RGB888,GetGUIMAP_Width(),GetGUIMAP_Height(),CCSizeMake(GetGUIMAP_Width(),GetGUIMAP_Height()));
	pSpriteMap->setTexture(pMapTexture);
	pSpriteMap->setTextureRect(CCRectMake(0,0,GetGUIMAP_Width(),GetGUIMAP_Height()));
	pSpriteMap->setColor(ccGRAY);

}
void UI_MAP::Hide_MapMain()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_BK);
	pSprite->setVisible(false);
}
//////////////////////////////////////////////////////////////////////////
void UI_MAP::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
//�ƹ��I�U!�P�_�O���Ӥ���
bool UI_MAP::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );


	if(isVisible()) //����ܤ~�~��
	{
		switch(PageType)
		{
		default:
			break;
		case MAPPG_MAIN:
			{
				//�P�_���@�Ӥ���
				for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
				{  
					CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
					bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
					if(bTouch)
					{
						Function_UIMAP_START(i);
						if(bMoveKeep) //��Բ��� �O���_�l�I
						{
							StartPoint_Mouse=nodePosition;
							
						}
						break;
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
void UI_MAP::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	if(bMoveKeep)
	{ //�p�G������
		MapMove(nodePosition );
	
	}
}
void UI_MAP::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	switch(PageType)
	{
	default:
		break;
	case MAPPG_MAIN:
		{
			//�P�_���@�Ӥ���
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch)
				{
					Function_UIMAP(i);
					break;
				}
			}  
		}
		break;
	}
	bMoveKeep=false; //�����
}
//�a�ϲ���
void UI_MAP::MapMove(CCPoint nodePosition )
{
	CCSprite* pSprite = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_MAPBK); 
	CCPoint MapSite;
	MapSite.x=StartPoint_Map.x+(nodePosition.x-StartPoint_Mouse.x);
	MapSite.y=StartPoint_Map.y+(nodePosition.y-StartPoint_Mouse.y);
	pSprite->setPosition(MapSite);
	pSprite = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_ROBOT); //ROBOT
	pSprite ->setPosition(MapSite);
}
float UI_MAP::MAPReSetSize(float fSize)
{
	if(fSize<=GUIMAPSize_MINRATE)
		fSize=GUIMAPSize_MINRATE;
	if(fSize>=GUIMAPSize_MAXRATE)
		fSize=GUIMAPSize_MAXRATE;

	CCSprite* pSprite = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_MAPBK); 
	CCPoint stCenter=pSprite->getPosition();
	//float fOldZoomRateX=pSprite->getScaleX();
	//float fOldZoomRateY=pSprite->getScaleY();
	float fOldZoomRate=pSprite->getScale();
	if(fSize==fOldZoomRate) 
		return fSize;
	pSprite->setScale(fSize);
	//stCenter.x *= (fSize/fOldZoomRate);
	//stCenter.y *= (fSize/fOldZoomRate);
//	pSprite->setPosition(stCenter);
	pSprite = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_ROBOT); //ROBOT
	pSprite->setScale(fSize*0.6);

	return fSize;

}
//��s���v�r��
void UI_MAP::MAPZoomLabUpdate()
{
	CCLabelTTF* pZoomLab= (CCLabelTTF*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_ZOOMLAB);	
	char sTemp[12];
	sprintf_s(sTemp,12,"X %.1f",fZoomRate);
	pZoomLab->setString(sTemp);

}
void UI_MAP::MAPZoomIn()
{
	if(fZoomRate>=1.0)
		fZoomRate+=GUIMAPSize_UP_SPACE;
	else
		fZoomRate+=GUIMAPSize_DOWN_SPACE;
	fZoomRate=MAPReSetSize(fZoomRate);
	CCSprite* pSpriteROBOT = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_ROBOT);
	CCSprite* pSpriteMAP = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_MAPBK);
	pSpriteROBOT->setPosition(pSpriteMAP->getPosition());	
	MAPZoomLabUpdate();
}
void UI_MAP::MAPZoomOut()
{
	if(fZoomRate>1.0)
		fZoomRate-=GUIMAPSize_UP_SPACE;
	else
		fZoomRate-=GUIMAPSize_DOWN_SPACE;
	fZoomRate=MAPReSetSize(fZoomRate);
	CCSprite* pSpriteROBOT = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_ROBOT);
	CCSprite* pSpriteMAP = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_MAPBK);
	pSpriteROBOT->setPosition(pSpriteMAP->getPosition());
	MAPZoomLabUpdate();
}
//�^�k���I
void UI_MAP::MAPRetunZero()
{
	pose_t robot_pose={0.0,0.0,0.0};
	pose_t robot_mappose;
	robot_mappose = getRobotSiteByPose2d(robot_pose);
	CCSprite* pSprite = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_ROBOT); //ROBOT
	pSprite->setAnchorPoint(CCPoint(0.5,0.5));
	float fWidthRate=winSize.width/GetGUIMAP_Width();
	float fHeightRate=winSize.height/GetGUIMAP_Height();
	pSprite ->setPosition(CCPoint(robot_mappose.px*fWidthRate,robot_mappose.py*fHeightRate));
	//���ʩ��Ϩ줤��
	pSprite = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_MAPBK); 
	pSprite->setAnchorPoint(CCPoint(0.5,0.5));
	pSprite ->setPosition(CCPoint(robot_mappose.px*fWidthRate,robot_mappose.py*fHeightRate));
}
void UI_MAP::MAPZoomDefault()
{
	CCSprite* pSprite = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_MAPBK); 
	pSprite->setAnchorPoint(CCPoint(0.5,0.5));
	pSprite->setPosition(CCPoint(GetGUIMAP_Width()*0.5,GetGUIMAP_Height()*0.5));
	
	fZoomRate=MAPReSetSize(GUIMAPSize_DEFRATE);
	//��MROBOT ��m
	pSprite = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_ROBOT); //ROBOT
	pSprite->setAnchorPoint(CCPoint(0.5,0.5));
	pSprite->setPosition(CCPoint(GetGUIMAP_Width()*0.5,GetGUIMAP_Height()*0.5));
	pSprite->setScale(fZoomRate*0.6);

}
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

//�I���}�l�P�_�O�_�i�H����
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
			CCSprite* pSprite = (CCSprite*)m_BtnArray[MAPPG_MAIN]->objectAtIndex(MAPMAIN_MAPBK); 
			StartPoint_Map=pSprite->getPosition();
		}
		bMoveKeep=true;
		
		break;
	}
}