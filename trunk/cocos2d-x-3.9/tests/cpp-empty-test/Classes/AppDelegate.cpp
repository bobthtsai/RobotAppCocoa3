#define ROBOT_MAIN_CPP

#include "AppDefine.h"
#include "AppDelegate.h"
#include "ImageList.h"
#include <vector>
#include <string>

#include "HelloWorldScene.h"
#include "AppMacros.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("RobotApp");
        //glview->setFrameSize(480, 320);
        // 在這裡設置創建窗口的尺寸，手機上這個就不用啦，因為手機有固定的屏幕
        director->setOpenGLView(glview);
    }
    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER); //NO_BORDER SHOW_ALL

	Size frameSize = glview->getFrameSize(); // 獲取屏幕尺寸
    
    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
    // 這裡是實現的重點，比較屏幕的高和設定的三種適配尺寸的高，選擇合適的圖片
    // 然後將對應圖片的路徑添加到搜索路徑中，那麼cocos2d-x就會到該目錄去尋找圖片
    // 因為是直立式的 所以寬高需要對調
    // 因為ＰＣ版的縮放觀念有誤，所以少了design層
	if (frameSize.width > largeResource.size.width)
	{
        searchPath.push_back(WinImageResource.directory);
        
//        director->setContentScaleFactor(MAX(WinImageResource.size.height/designResolutionSize.height, WinImageResource.size.width/designResolutionSize.width));
        director->setContentScaleFactor(MAX(WinImageResource.size.height/frameSize.height, WinImageResource.size.width/frameSize.width));
        SetWindwsSizeIdx(WINFRAME_XXH);

	}
    else if (frameSize.width > mediumResource.size.width)
    {
        searchPath.push_back(largeResource.directory);
        
        director->setContentScaleFactor(MAX(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
//        director->setContentScaleFactor(MAX(largeResource.size.height/frameSize.height, largeResource.size.width/frameSize.width));

         SetWindwsSizeIdx(WINFRAME_XH);
        
    }

    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.width > smallResource.size.width)
    {
        searchPath.push_back(mediumResource.directory);
        
        director->setContentScaleFactor(MAX(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
//        director->setContentScaleFactor(MAX(mediumResource.size.height/frameSize.height, mediumResource.size.width/frameSize.width));

        SetWindwsSizeIdx(WINFRAME_H);
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPath.push_back(smallResource.directory);

//        director->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
        director->setContentScaleFactor(MAX(smallResource.size.height/frameSize.height, smallResource.size.width/frameSize.width));

        SetWindwsSizeIdx(WINFRAME_M);
    }

   // searchPath.push_back(WinImageResource.directory);
    // set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);
	
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    SetSize(); //set font size
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::scene();
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void AppDelegate::SetSize()
{ //日後改變位置！設定字體大小
    auto iSizeZoom=0.6 * FONT_SIZE;
    auto iWinIdx=GetWindwsSizeIdx();
//因為ＩＯＳ無法強制轉型 所以需要建立空間存放
    static float strFontArray[30];
    pSTR_TITAL_SIZE		= &strFontArray[0];
    pSTR_ERROR_SIZE		= &strFontArray[1];
    pSTR_STATE_SIZE		= &strFontArray[2];
    pSTR_WIFILIST_SIZE	= &strFontArray[3];
    pSTR_ROBOTMAIN_LABSIZE	= &strFontArray[4];
    pSTR_CLEAN_LABSIZE	= &strFontArray[5];
    pSTR_RESERV_LABSIZE	= &strFontArray[6];
    pSTR_HELP_SIZE		= &strFontArray[7];
    pSTR_DATE_SIZE		= &strFontArray[8];
    pSTR_TIME_SIZE		= &strFontArray[9];
    pSTR_LINK_SIZE		= &strFontArray[10];
    pSTR_SETUP_SIZE		= &strFontArray[11];
    pSTR_CLEANJOB_SIZE	= &strFontArray[12];
    pSTR_BTN_SIZE		= &strFontArray[13];
    pSTR_TOAST_SIZE		= &strFontArray[14];
    pSTR_DIALOG_SIZE	= &strFontArray[15];

    switch (iWinIdx) {
            
        default:
        case WINFRAME_M:
        {
            *pSTR_TITAL_SIZE = STR_TITAL_SIZE_M  * iSizeZoom;
            *pSTR_ERROR_SIZE = STR_ERROR_SIZE_M  * iSizeZoom;
            *pSTR_STATE_SIZE = STR_STATE_SIZE_M  * iSizeZoom;
            *pSTR_WIFILIST_SIZE = STR_WIFILIST_SIZE_M  * iSizeZoom;
            *pSTR_ROBOTMAIN_LABSIZE = STR_ROBOTMAIN_LABSIZE_M  * iSizeZoom;
            *pSTR_CLEAN_LABSIZE = STR_CLEAN_LABSIZE_M  * iSizeZoom;
            *pSTR_RESERV_LABSIZE = STR_RESERV_LABSIZE_M  * iSizeZoom;
            *pSTR_HELP_SIZE = STR_HELP_SIZE_M  * iSizeZoom;
            *pSTR_DATE_SIZE = STR_DATE_SIZE_M  * iSizeZoom;
            *pSTR_TIME_SIZE = STR_TIME_SIZE_M  * iSizeZoom;
            *pSTR_LINK_SIZE = STR_LINK_SIZE_M  * iSizeZoom;
            *pSTR_SETUP_SIZE = STR_SETUP_SIZE_M  * iSizeZoom;
            *pSTR_CLEANJOB_SIZE = STR_CLEANJOB_SIZE_M  * iSizeZoom;
            *pSTR_BTN_SIZE = STR_BTN_SIZE_M  * iSizeZoom;
            *pSTR_TOAST_SIZE = STR_TOAST_SIZE_M  * iSizeZoom;
            *pSTR_DIALOG_SIZE = STR_DIALOG_SIZE_M * iSizeZoom;
        }
        break;
        case WINFRAME_H:
        {
            *pSTR_TITAL_SIZE = STR_TITAL_SIZE_H  * iSizeZoom;
            *pSTR_ERROR_SIZE = STR_ERROR_SIZE_H  * iSizeZoom;
            *pSTR_STATE_SIZE = STR_STATE_SIZE_H  * iSizeZoom;
            *pSTR_WIFILIST_SIZE = STR_WIFILIST_SIZE_H  * iSizeZoom;
            *pSTR_ROBOTMAIN_LABSIZE = STR_ROBOTMAIN_LABSIZE_H  * iSizeZoom;
            *pSTR_CLEAN_LABSIZE = STR_CLEAN_LABSIZE_H  * iSizeZoom;
            *pSTR_RESERV_LABSIZE = STR_RESERV_LABSIZE_H  * iSizeZoom;
            *pSTR_HELP_SIZE = STR_HELP_SIZE_H  * iSizeZoom;
            *pSTR_DATE_SIZE = STR_DATE_SIZE_H  * iSizeZoom;
            *pSTR_TIME_SIZE = STR_TIME_SIZE_H  * iSizeZoom;
            *pSTR_LINK_SIZE = STR_LINK_SIZE_H  * iSizeZoom;
            *pSTR_SETUP_SIZE = STR_SETUP_SIZE_H  * iSizeZoom;
            *pSTR_CLEANJOB_SIZE = STR_CLEANJOB_SIZE_H  * iSizeZoom;
            *pSTR_BTN_SIZE = STR_BTN_SIZE_H  * iSizeZoom;
            *pSTR_TOAST_SIZE = STR_TOAST_SIZE_H  * iSizeZoom;
            *pSTR_DIALOG_SIZE = STR_DIALOG_SIZE_H * iSizeZoom;
        }
            break;
        case WINFRAME_XH:
        {
            *pSTR_TITAL_SIZE = STR_TITAL_SIZE_XH  * iSizeZoom;
            *pSTR_ERROR_SIZE = STR_ERROR_SIZE_XH  * iSizeZoom;
            *pSTR_STATE_SIZE = STR_STATE_SIZE_XH  * iSizeZoom;
            *pSTR_WIFILIST_SIZE = STR_WIFILIST_SIZE_XH  * iSizeZoom;
            *pSTR_ROBOTMAIN_LABSIZE = STR_ROBOTMAIN_LABSIZE_XH  * iSizeZoom;
            *pSTR_CLEAN_LABSIZE = STR_CLEAN_LABSIZE_XH  * iSizeZoom;
            *pSTR_RESERV_LABSIZE = STR_RESERV_LABSIZE_XH  * iSizeZoom;
            *pSTR_HELP_SIZE = STR_HELP_SIZE_XH  * iSizeZoom;
            *pSTR_DATE_SIZE = STR_DATE_SIZE_XH  * iSizeZoom;
            *pSTR_TIME_SIZE = STR_TIME_SIZE_XH  * iSizeZoom;
            *pSTR_LINK_SIZE = STR_LINK_SIZE_XH  * iSizeZoom;
            *pSTR_SETUP_SIZE = STR_SETUP_SIZE_XH  * iSizeZoom;
            *pSTR_CLEANJOB_SIZE = STR_CLEANJOB_SIZE_XH  * iSizeZoom;
            *pSTR_BTN_SIZE = STR_BTN_SIZE_XH  * iSizeZoom;
            *pSTR_TOAST_SIZE = STR_TOAST_SIZE_XH  * iSizeZoom;
            *pSTR_DIALOG_SIZE = STR_DIALOG_SIZE_XH * iSizeZoom;
        }
            break;
        case WINFRAME_XXH:
        {
            *pSTR_TITAL_SIZE = STR_TITAL_SIZE_XXH  * iSizeZoom;
            *pSTR_ERROR_SIZE = STR_ERROR_SIZE_XXH  * iSizeZoom;
            *pSTR_STATE_SIZE = STR_STATE_SIZE_XXH  * iSizeZoom;
            *pSTR_WIFILIST_SIZE = STR_WIFILIST_SIZE_XXH  * iSizeZoom;
            *pSTR_ROBOTMAIN_LABSIZE = STR_ROBOTMAIN_LABSIZE_XXH  * iSizeZoom;
            *pSTR_CLEAN_LABSIZE = STR_CLEAN_LABSIZE_XXH  * iSizeZoom;
            *pSTR_RESERV_LABSIZE = STR_RESERV_LABSIZE_XXH  * iSizeZoom;
            *pSTR_HELP_SIZE = STR_HELP_SIZE_XXH  * iSizeZoom;
            *pSTR_DATE_SIZE = STR_DATE_SIZE_XXH  * iSizeZoom;
            *pSTR_TIME_SIZE = STR_TIME_SIZE_XXH  * iSizeZoom;
            *pSTR_LINK_SIZE = STR_LINK_SIZE_XXH  * iSizeZoom;
            *pSTR_SETUP_SIZE = STR_SETUP_SIZE_XXH  * iSizeZoom;
            *pSTR_CLEANJOB_SIZE = STR_CLEANJOB_SIZE_XXH  * iSizeZoom;
            *pSTR_BTN_SIZE = STR_BTN_SIZE_XXH  * iSizeZoom;
            *pSTR_TOAST_SIZE = STR_TOAST_SIZE_XXH  * iSizeZoom;
            *pSTR_DIALOG_SIZE = STR_DIALOG_SIZE_XXH * iSizeZoom;
        }
            break;

    }

}
