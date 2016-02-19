#include <ImageList.h>
#include <HelloWorldScene.h>
#include <libplayerc++/playerc++.h>
USING_NS_CC;
using namespace std;
using namespace PlayerCc;

extern HelloWorld* m_MainView; 
/// Player Client
extern PlayerClient *m_client;

UI_SAMPLE::UI_SAMPLE()
{
	PageType=MAPPG_LAYER;

}
UI_SAMPLE::~UI_SAMPLE()
{
	for(int i=0;i<MAPPG_SIZE;i++)
		m_BtnArray[i]->release();
}

bool UI_SAMPLE::init()
{  
	if (!CCLayer::init()) 
	{  
		return false;  
	}  
	InitUI();  
	return true;  
}  

void UI_SAMPLE::InitUI()
{
	winSize = CCDirector::sharedDirector()->getVisibleSize(); //���o�����j�p

}
void UI_SAMPLE::InitUI_SAMPLEMAIN()
{
	m_BtnArray[SAMPLEPG_MAIN] = CCArray::createWithCapacity(SAMPLEMAIN_SIZE); //���s�s��
	m_BtnArray[SAMPLEPG_MAIN]->retain();
	int iHeight,iWidth;
	float W_unit=winSize.width/1080;
	float H_unit=winSize.height/1920;

	//�]�w�I��
	CCSprite *pSpriteBK=CCSprite::createWithSpriteFrameName(S_MAIN_BKIMG);
	pSpriteBK->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	addChild(pSpriteBK); //�[�J layer �h	
	m_BtnArray[SAMPLEPG_MAIN]->addObject(pSpriteBK);

	//�W����D�C
	CCSprite *pSpriteAction=CCSprite::createWithSpriteFrameName(S_TITAL_BKIMG);
	pSpriteAction->setAnchorPoint(CCPoint(0,0)); //�]�w���I
	pSpriteAction->setPosition(ccp(0,winSize.height-pSpriteAction->getContentSize().height));
	pSpriteBK->addChild(pSpriteAction);
	CCSprite *pSprBtnIcon=CCSprite::createWithSpriteFrameName(S_TITAL_BACK_ICON); //���s�ϥ�
	pSprBtnIcon->setPosition(ccp(pSprBtnIcon->getContentSize().width/2,pSprBtnIcon->getContentSize().height/2));
	pSpriteAction->addChild(pSprBtnIcon);
	CCLabelTTF* pLabel = CCLabelTTF::create(STR_TITAL_CoverageMap, S_FONE, STR_TITAL_SIZE);
	pLabel->setColor(COLOR_STRING_TITAL);
	pLabel->setPosition(ccp(pSprBtnIcon->getContentSize().width+ pLabel->getContentSize().width/2,pLabel->getContentSize().height/2));
	pSpriteAction->addChild(pLabel, 10);
	m_BtnArray[SAMPLEPG_MAIN]->insertObject(pSpriteAction,SAMPLEMAIN_TITAL);


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
void UI_SAMPLE::ShowLayer()
{
	setVisible(true);
	setTouchEnabled(true);
	Show_SAMPLEMAIN();
}
void UI_SAMPLE::HideLayer()
{
	setVisible(false);
	setTouchEnabled(false);
	Hide_AllPage();
}
void UI_SAMPLE::Hide_AllPage()
{
	Hide_SAMPLEMAIN();
	PageType = SAMPLEPG_LAYER;
}
void UI_SAMPLE::Show_SAMPLEMAIN()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SAMPLEPG_MAIN]->objectAtIndex(SAMPLEMAIN_BK);
	pSprite->setVisible(true);
	PageType = SAMPLEPG_MAIN;
}
void UI_SAMPLE::Hide_SAMPLEMAIN()
{
	CCSprite* pSprite= (CCSprite*)m_BtnArray[SAMPLEPG_MAIN]->objectAtIndex(SAMPLEMAIN_BK);
	pSprite->setVisible(false);
}
//////////////////////////////////////////////////////////////////////////
void UI_SAMPLE::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
//�ƹ��I�U!�P�_�O���Ӥ���
bool UI_SAMPLE::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );


	if(isVisible()) //����ܤ~�~��
		return true;
	else
		return false;
}
void UI_SAMPLE::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
}
void UI_SAMPLE::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );
	switch(PageType)
	{
	default:
		break;
	case SAMPLEPG_MAIN:
		{
			//�P�_���@�Ӥ���
			for (int i = 1; i < m_BtnArray[PageType]->count(); i++)
			{  
				CCSprite* pSprite = (CCSprite*)m_BtnArray[PageType]->objectAtIndex(i); 
				bool bTouch=pSprite ->boundingBox().containsPoint(nodePosition);
				if(bTouch)
				{
					Function_UISAMPLE(i);
					break;
				}
			}  
		}
		break;
	}
}


void UI_SAMPLE::Function_UISAMPLE(int iBtnIdx)
{
	switch(iBtnIdx) 
	{
	default:
		break;
	case SAMPLEMAIN_TITAL:
		ChangLayer_Main();
		break;
	}
}



//////////////////////////////////////////////////////////////////////////
//Robot Sacnning View
