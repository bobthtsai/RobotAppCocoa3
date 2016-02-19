#include <ImageList.h>
#include "Coco2dDialog.h"
//////////////////////////////////////////////////////////////////////////
//建立通用彈跳視窗
//參考http://www.android100.net/html/201406/04/18200.html
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
		CC_BREAK_IF(!LayerColor::initWithColor(Color4B(0, 0, 0, 0)));//ccc4(0, 0, 0, 125)
		winSize = Director::getInstance()->getVisibleSize();
		winOrigin = Director::getInstance()->getVisibleOrigin();
		//====================================
		bg = Sprite::createWithSpriteFrameName(S_MSGBOX_BK);
		//bg = CCScale9Sprite::createWithSpriteFrameName(S_BTN_BACKGROUNP);
		bg->setPosition(Vec2(winOrigin.x+winSize.width/2,winOrigin.y+130*winSize.height/1080));
		//============================
		bRet = true;
	} while (0);
	return bRet;
}

void Toast::onExit()
{
	LayerColor::onExit();
}

void Toast::initToast( string msg,float time )
{
	auto pLabel = Label::createWithSystemFont(msg.c_str(), S_FONE, *pSTR_TOAST_SIZE);
	pLabel->setColor(COLOR_STRING_BLACK);
	Size tSizeOrig=bg->getContentSize();
	Size tTargetSize=pLabel->getContentSize();
	//bg->setContentSize(CCSizeMake(pLabel->getContentSize().width+10,pLabel->getContentSize().height+10));
	bg->setScaleX( ( tTargetSize.width / tSizeOrig.width )); //利用CCSPRITE 需額外進行縮放
	bg->setScaleY( ( tTargetSize.height / tSizeOrig.height)); //利用CCSPRITE 需額外進行縮放
	bg->setColor(Color3B(240,240,240));
	bg->addChild(pLabel, 1);
	this->addChild(bg,10);
	pLabel->setPosition(bg->getContentSize()/2);
	pLabel->setScaleX( ( tSizeOrig.width /tTargetSize.width  )); //利用CCSPRITE 需額外進行縮放
	pLabel->setScaleY( ( tSizeOrig.height/ tTargetSize.height )); //利用CCSPRITE 需額外進行縮放
	pLabel->runAction(Sequence::create(FadeIn::create(time/5),
		DelayTime::create(time/5*3),FadeOut::create(time/5),NULL));
	bg->runAction(Sequence::create(FadeIn::create(time/5),
		DelayTime::create(time/5*3),FadeOut::create(time/5),
		//CallFunc::create(this,callfunc_selector(Toast::removeSelf)),NULL));
        CallFunc::create(CC_CALLBACK_0(Toast::removeSelf, this)),NULL));

	//=========================
	//this->scheduleOnce(schedule_selector(XYToast::removeSelf),time);
}

void Toast::removeSelf()
{
	this->removeFromParentAndCleanup(true);
}
//////////////////////////////////////////////////////////////////////////
//彈跳視窗
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
		CC_BREAK_IF(!Layer::init());
		this->setContentSize(Size::ZERO);

		// 初始化MENU
		Menu* menu = Menu::create();
        menu->setPosition(Vec2::ZERO);
		setMenuButton(menu);
		bRef = true;
	}while(0);
	return bRef;
}

//void PopupLayer::registerWithTouchDispatcher(){
//	// 設定TOUCH 優先權
//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
//
//}

bool PopupLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	//
	log("PopupLayer touch");
	return true;
}

PopupLayer* PopupLayer::createWithSpriteFrameName(const char *backgroundImage){
	PopupLayer* ml = PopupLayer::create();
	ml->setSpriteBackGround(Sprite::createWithSpriteFrameName(backgroundImage));
	//ml->setSprite9BackGround(CCScale9Sprite::createWithSpriteFrameName(backgroundImage));
	return ml;
}
PopupLayer* PopupLayer::create(const char *backgroundImage){
	PopupLayer* ml = PopupLayer::create();
	ml->setSpriteBackGround(Sprite::create(backgroundImage));
	//ml->setSprite9BackGround(CCScale9Sprite::create(backgroundImage));
	return ml;
}

void PopupLayer::setTitle(const char *title, int fontsize){
	auto ltfTitle = Label::createWithSystemFont(title, "", fontsize);
	setLabelTitle(ltfTitle);
}

void PopupLayer::setContentText(const char *text, int fontsize, int padding, int paddingTop){
	auto ltf = Label::createWithSystemFont(text, "", fontsize);
	setLabelContentText(ltf);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}

void PopupLayer::setCallbackFunc(Ref *target, SEL_CallFuncN callfun){
	m_callbackListener = target;
	m_callback = callfun;    
}


bool PopupLayer::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag)
{
    auto winSize = Director::getInstance()->getVisibleSize();
	auto pCenter = winSize/2;

	// 建立圖片
	MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage,  CC_CALLBACK_1(PopupLayer::buttonCallback, this));
	menuImage->setTag(tag);
	menuImage->setPosition(pCenter);

	// 說明文字
	Size imenu = menuImage->getContentSize();
	auto ttf = Label::createWithSystemFont(title, "", *pSTR_BTN_SIZE);
	ttf->setColor(COLOR_STRING_BLACK);
	ttf->setPosition(Vec2(imenu.width / 2, imenu.height / 2));
	menuImage->addChild(ttf);

	getMenuButton()->addChild(menuImage);
	return true;
}

bool PopupLayer::addButtonBySpriteFrame(const char *normalImage, const char *selectedImage, const char *title, int tag){
    auto winSize = Director::getInstance()->getVisibleSize();
    auto pCenter = winSize/2;

	// 建立圖片
//    auto pNormalImage=Sprite::createWithSpriteFrameName(normalImage);
//    auto pSelectImage=Sprite::createWithSpriteFrameName(selectedImage);
	MenuItemImage * menuImage = MenuItemImage::create(normalImage, selectedImage, CC_CALLBACK_1(PopupLayer::buttonCallback, this));
	Size ImageSize = menuImage->getContentSize();

	menuImage->setTag(tag);
	menuImage->setPosition(pCenter);
	menuImage->setColor(COLOR_STRING_TITAL);

    // 說明文字
	Size imenu = menuImage->getContentSize();
	auto ttf = Label::createWithSystemFont(title, "", *pSTR_BTN_SIZE);
	ttf->setColor(COLOR_STRING_BLACK);
	ttf->setPosition(Vec2(imenu.width / 2, imenu.height / 2));
	//ttf->setScaleX(4);
	menuImage->addChild(ttf);

	getMenuButton()->addChild(menuImage);
	return true;
}

void PopupLayer::buttonCallback(Ref *pSender){
	Node* node = dynamic_cast<Node*>(pSender);
	log("touch tag: %d", node->getTag());
	if (m_callback && m_callbackListener){
		(m_callbackListener->*m_callback)(node);
	}
	this->removeFromParent();
}

void PopupLayer::onEnter(){
	Layer::onEnter();

    auto winSize = Director::getInstance()->getVisibleSize();
    auto pCenter = winSize/2;

	Size contentSize;
	getSpriteBackGround()->setPosition(pCenter); //設定到中間
	getSpriteBackGround()->setColor(Color3B(240,240,240)); //BOBADD
	//getSpriteBackGround()->setScaleY(0.7f);		// 因為沒有正確的空白圖檔 所以需要進行縮放
	//getSpriteBackGround()->setScaleX(0.8f);
	this->addChild(getSpriteBackGround(), 0, 0);
	contentSize = getSpriteBackGround()->boundingBox().size;

	// 設定參數 執行時載入
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


	// 增加按鈕 設定位置
	this->addChild(getMenuButton());
	float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount() + 1);

//	CCArray* array = getMenuButton()->getChildren();
//	CCObject* pObj = NULL;
//	int i = 0;
//	CCARRAY_FOREACH(array, pObj)
    int i=0;
    for(auto pObj:getMenuButton()->getChildren())
    {
		Node* node = dynamic_cast<Node*>(pObj);
		node->setPosition(Vec2( btnWidth * (i+1 )+(btnWidth*0.4f)*i, winSize.height / 2 - contentSize.height / 3));
		i++;
	}


	// 對話框標題
	if (getLabelTitle()){
        getLabelTitle()->setPosition(Vec2(pCenter)+ Vec2(0, contentSize.height / 2 - 35.0f));
		this->addChild(getLabelTitle());
	}

	// 顯示訊息
	if (getLabelContentText()){
		auto  ltf = getLabelContentText();
		ltf->setPosition(pCenter);
		ltf->setDimensions(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop);
        ltf->setHorizontalAlignment(TextHAlignment::LEFT);
		ltf->setColor(COLOR_STRING_BLACK);
		this->addChild(ltf);
	}

	// 彈出效果
	Action* popupLayer = Sequence::create(ScaleTo::create(0.0, 0.0),
		ScaleTo::create(0.06, 1.05),
		ScaleTo::create(0.08, 0.95),
		ScaleTo::create(0.08, 1.0), NULL);
	this->runAction(popupLayer);
    setTouchEnabled(true);
}

void PopupLayer::onExit(){

	log("popup on exit.");
    setTouchEnabled(false);

	Layer::onExit();
}
void PopupLayer::setTouchEnabled(bool enable)
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
        
        touchListener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
//        touchListener->onTouchMoved = CC_CALLBACK_2(UI_CLEANJOB::onTouchMoved, this);
//        touchListener->onTouchEnded = CC_CALLBACK_2(UI_CLEANJOB::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
    }
    else
    {
        _eventDispatcher->removeEventListener(touchListener);
        CC_SAFE_RELEASE_NULL(touchListener);
    }
}