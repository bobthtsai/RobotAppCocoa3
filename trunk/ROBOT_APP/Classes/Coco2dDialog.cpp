#include <ImageList.h>
#include "Coco2dDialog.h"
//////////////////////////////////////////////////////////////////////////
//�إ߳q�μu������
//�Ѧ�http://www.android100.net/html/201406/04/18200.html
Toast::Toast()
{
}

Toast::~Toast()
{
}

bool Toast::init()
{
	bool bRet = false;
	do {
		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)));//ccc4(0, 0, 0, 125)
		visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		origin = CCDirector::sharedDirector()->getVisibleOrigin();
		//====================================
		bg = CCSprite::createWithSpriteFrameName(S_MSGBOX_BK);
		//bg = CCScale9Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
		bg->setPosition(ccp(origin.x+visibleSize.width/2,origin.y+130*visibleSize.height/1080));
		//============================
		bRet = true;
	} while (0);
	return bRet;
}

void Toast::onExit()
{
	CCLayerColor::onExit();
}

void Toast::initToast( string msg,float time )
{
	CCLabelTTF* pLabel = CCLabelTTF::create(msg.c_str(), S_FONE, *pSTR_TOAST_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	CCSize tSizeOrig=bg->getContentSize();
	CCSize tTargetSize=pLabel->getContentSize();
	//bg->setContentSize(CCSizeMake(pLabel->getContentSize().width+10,pLabel->getContentSize().height+10));
	bg->setScaleX( ( tTargetSize.width / tSizeOrig.width )); //�Q��CCSPRITE ���B�~�i���Y��
	bg->setScaleY( ( tTargetSize.height / tSizeOrig.height)); //�Q��CCSPRITE ���B�~�i���Y��
	bg->setColor(ccc3(240,240,240));
	bg->addChild(pLabel, 1);
	this->addChild(bg,10);
	pLabel->setPosition(ccp(bg->getContentSize().width/2,bg->getContentSize().height/2));
	pLabel->setScaleX( ( tSizeOrig.width /tTargetSize.width  )); //�Q��CCSPRITE ���B�~�i���Y��
	pLabel->setScaleY( ( tSizeOrig.height/ tTargetSize.height )); //�Q��CCSPRITE ���B�~�i���Y��
	pLabel->runAction(CCSequence::create(CCFadeIn::create(time/5),
		CCDelayTime::create(time/5*3),CCFadeOut::create(time/5),NULL));
	bg->runAction(CCSequence::create(CCFadeIn::create(time/5),
		CCDelayTime::create(time/5*3),CCFadeOut::create(time/5),
		CCCallFunc::create(this,callfunc_selector(Toast::removeSelf)),NULL));
	//=========================
	//this->scheduleOnce(schedule_selector(XYToast::removeSelf),time);
}

void Toast::removeSelf()
{
	this->removeFromParentAndCleanup(true);
}
//////////////////////////////////////////////////////////////////////////
//�u������
PopupLayer::PopupLayer():
m__pMenu(NULL)
	, m_contentPadding(0)
	, m_contentPaddingTop(0)
	, m_callbackListener(NULL)
	, m_callback(NULL)
	, m__sfBackGround(NULL)
//	, m__s9BackGround(NULL)
	, m__ltContentText(NULL)
	, m__ltTitle(NULL)
{

}

PopupLayer::~PopupLayer(){
	CC_SAFE_RELEASE(m__pMenu);
	CC_SAFE_RELEASE(m__sfBackGround);
	CC_SAFE_RELEASE(m__ltContentText);
	CC_SAFE_RELEASE(m__ltTitle);
//	CC_SAFE_RELEASE(m__s9BackGround);
}

bool PopupLayer::init(){
	bool bRef = false;
	do{
		CC_BREAK_IF(!CCLayer::init());
		this->setContentSize(CCSizeZero);

		// ��l��MENU
		CCMenu* menu = CCMenu::create();
		menu->setPosition(CCPointZero);
		setMenuButton(menu);

		setTouchEnabled(true);

		bRef = true;
	}while(0);
	return bRef;
}

void PopupLayer::registerWithTouchDispatcher(){
	// �]�wTOUCH �u���v
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);

}

bool PopupLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	//
	CCLog("PopupLayer touch");
	return true;
}

PopupLayer* PopupLayer::createWithSpriteFrameName(const char *backgroundImage){
	PopupLayer* ml = PopupLayer::create();
	ml->setSpriteBackGround(CCSprite::createWithSpriteFrameName(backgroundImage));
	//ml->setSprite9BackGround(CCScale9Sprite::createWithSpriteFrameName(backgroundImage));
	return ml;
}
PopupLayer* PopupLayer::create(const char *backgroundImage){
	PopupLayer* ml = PopupLayer::create();
	ml->setSpriteBackGround(CCSprite::create(backgroundImage));
	//ml->setSprite9BackGround(CCScale9Sprite::create(backgroundImage));
	return ml;
}

void PopupLayer::setTitle(const char *title, int fontsize){
	CCLabelTTF* ltfTitle = CCLabelTTF::create(title, "", fontsize);
	setLabelTitle(ltfTitle);
}

void PopupLayer::setContentText(const char *text, int fontsize, int padding, int paddingTop){
	CCLabelTTF* ltf = CCLabelTTF::create(text, "", fontsize);
	setLabelContentText(ltf);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}

void PopupLayer::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun){
	m_callbackListener = target;
	m_callback = callfun;    
}


bool PopupLayer::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);

	// �إ߹Ϥ�
	CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImage, selectedImage, this, menu_selector(PopupLayer::buttonCallback));
	menuImage->setTag(tag);
	menuImage->setPosition(pCenter);

	// ������r
	CCSize imenu = menuImage->getContentSize();
	CCLabelTTF* ttf = CCLabelTTF::create(title, "", *pSTR_BTN_SIZE);
	ttf->setColor(COLOR_STRING_BLACK);
	ttf->setPosition(ccp(imenu.width / 2, imenu.height / 2));
	menuImage->addChild(ttf);

	getMenuButton()->addChild(menuImage);
	return true;
}

bool PopupLayer::addButtonBySpriteFrame(const char *normalImage, const char *selectedImage, const char *title, int tag){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
	CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);

	// �إ߹Ϥ�
	CCMenuItemSprite * menuImage = CCMenuItemSprite ::create(CCSprite::createWithSpriteFrameName(normalImage), CCSprite::createWithSpriteFrameName(selectedImage), this, menu_selector(PopupLayer::buttonCallback));
	CCSize ImageSize = menuImage->getContentSize();

	menuImage->setTag(tag);
	menuImage->setPosition(pCenter);
	menuImage->setColor(COLOR_STRING_TITAL);
	//menuImage->setScaleX(0.25f);
	//menuImage->setContentSize(CCSizeMake(ImageSize.width /8,ImageSize.height/4 )); //�Y�p��l��

	// ������r
	CCSize imenu = menuImage->getContentSize();
	CCLabelTTF* ttf = CCLabelTTF::create(title, "", *pSTR_BTN_SIZE);
	ttf->setColor(COLOR_STRING_BLACK);
	ttf->setPosition(ccp(imenu.width / 2, imenu.height / 2));
	//ttf->setScaleX(4);
	menuImage->addChild(ttf);

	getMenuButton()->addChild(menuImage);
	return true;
}

void PopupLayer::buttonCallback(cocos2d::CCObject *pSender){
	CCNode* node = dynamic_cast<CCNode*>(pSender);
	CCLog("touch tag: %d", node->getTag());
	if (m_callback && m_callbackListener){
		(m_callbackListener->*m_callback)(node);
	}
	this->removeFromParent();
}

void PopupLayer::onEnter(){
	CCLayer::onEnter();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);

	CCSize contentSize;
	getSpriteBackGround()->setPosition(ccp(winSize.width / 2, winSize.height / 2)); //�]�w�줤��
	getSpriteBackGround()->setColor(ccc3(240,240,240)); //BOBADD
	//getSpriteBackGround()->setScaleY(0.7f);		// �]���S�����T���ťչ��� �ҥH�ݭn�i���Y��
	//getSpriteBackGround()->setScaleX(0.8f);
	this->addChild(getSpriteBackGround(), 0, 0);
	contentSize = getSpriteBackGround()->boundingBox().size;

	// �]�w�Ѽ� ����ɸ��J
	//if (getContentSize().equals(CCSizeZero)) {		
		//getSpriteBackGround()->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		//getSpriteBackGround()->setColor(ccc3(240,240,240)); //BOBADD
		//this->addChild(getSpriteBackGround(), 0, 0);
		//contentSize = getSpriteBackGround()->getTexture()->getContentSize();		
	//} else {
	//	//CCScale9Sprite *background = getSprite9BackGround();
	//	background->setContentSize(getContentSize());
	//	background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	//	background->setColor(ccc3(240,240,240));
	//	this->addChild(background, 0, 0);
	//	contentSize = getContentSize();
	//}


	// �W�[���s �]�w��m
	this->addChild(getMenuButton());
	float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount() + 1);

	CCArray* array = getMenuButton()->getChildren();
	CCObject* pObj = NULL;
	int i = 0;
	CCARRAY_FOREACH(array, pObj){
		CCNode* node = dynamic_cast<CCNode*>(pObj);
		node->setPosition(ccp( btnWidth * (i+1 )+(btnWidth*0.4f)*i, winSize.height / 2 - contentSize.height / 3));
		i++;
	}


	// ��ܮؼ��D
	if (getLabelTitle()){
		getLabelTitle()->setPosition(ccpAdd(pCenter, ccp(0, contentSize.height / 2 - 35.0f)));		
		this->addChild(getLabelTitle());
	}

	// ��ܰT��
	if (getLabelContentText()){
		CCLabelTTF* ltf = getLabelContentText();
		ltf->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		ltf->setDimensions(CCSizeMake(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop));
		ltf->setHorizontalAlignment(kCCTextAlignmentLeft);
		ltf->setColor(COLOR_STRING_BLACK);
		this->addChild(ltf);
	}

	// �u�X�ĪG
	CCAction* popupLayer = CCSequence::create(CCScaleTo::create(0.0, 0.0),
		CCScaleTo::create(0.06, 1.05),
		CCScaleTo::create(0.08, 0.95),
		CCScaleTo::create(0.08, 1.0), NULL);
	this->runAction(popupLayer);

}

void PopupLayer::onExit(){

	CCLog("popup on exit.");
	CCLayer::onExit();
}