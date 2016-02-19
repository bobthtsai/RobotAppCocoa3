#define ROBOT_MAIN_CPP
#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "ImageList.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
#include <Windows.h>
#endif
USING_NS_CC;
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// create the application instance
	AppDelegate app;
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();

	eglView->setViewName("ROBOT_APP"); 
	eglView->setVersionName("1.0.1127");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  	
	int ScreenHeight, ScreenWidth;
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN); //取得高度的像素
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN); //取得寬度的像素
	
	int iIdx_H =0 ,iIdx_W=0,iIdx_win=0; //找出最適合辨識率
	for(int i=0;i<4;i++)
	{
		if(IMAGE_SIZE_TABLE_H[i]<ScreenHeight)
			iIdx_H=i;
		if(IMAGE_SIZE_TABLE_W[i]<ScreenWidth)
			iIdx_W=i;
	}

	if(iIdx_H<iIdx_W)
		iIdx_win=iIdx_H;
	else
		iIdx_win=iIdx_W;
	//讀取設定!強制改變解析度
	FILE *pFile=fopen("WIN.INI","r");
	if(pFile!=NULL)
	{
		char sTemp[256];
		int iValue;
		fscanf(pFile,"%d %[^\n]",&iValue,sTemp);
		if(iValue>=0 && iValue<4)
			iIdx_win=iValue;
		fclose(pFile);
	}
	//決定字體大小
	switch(iIdx_win)
	{
	case WINFRAME_M:
		pSTR_TITAL_SIZE		= &STR_TITAL_SIZE_M;
		pSTR_ERROR_SIZE		= &STR_ERROR_SIZE_M;
		pSTR_STATE_SIZE		= &STR_STATE_SIZE_M;
		pSTR_WIFILIST_SIZE	= &STR_WIFILIST_SIZE_M;
		pSTR_ROBOTMAIN_LABSIZE	= &STR_ROBOTMAIN_LABSIZE_M;
		pSTR_CLEAN_LABSIZE	= &STR_CLEAN_LABSIZE_M;
		pSTR_RESERV_LABSIZE	= &STR_RESERV_LABSIZE_M;
		pSTR_HELP_SIZE		= &STR_HELP_SIZE_M;
		pSTR_DATE_SIZE		= &STR_DATE_SIZE_M;
		pSTR_TIME_SIZE		= &STR_TIME_SIZE_M;
		pSTR_LINK_SIZE		= &STR_LINK_SIZE_M;
		pSTR_SETUP_SIZE		= &STR_SETUP_SIZE_M;
		pSTR_CLEANJOB_SIZE	= &STR_CLEANJOB_SIZE_M;
		pSTR_BTN_SIZE		= &STR_BTN_SIZE_M;
		pSTR_TOAST_SIZE		= &STR_TOAST_SIZE_M;
		pSTR_DIALOG_SIZE	= &STR_DIALOG_SIZE_M;
		pSTR_LATER_SIZE		= &STR_LATER_SIZE_M;
		break;
	case WINFRAME_XH:
		pSTR_TITAL_SIZE		= &STR_TITAL_SIZE_XH;
		pSTR_ERROR_SIZE		= &STR_ERROR_SIZE_XH;
		pSTR_STATE_SIZE		= &STR_STATE_SIZE_XH;
		pSTR_WIFILIST_SIZE	= &STR_WIFILIST_SIZE_XH;
		pSTR_ROBOTMAIN_LABSIZE	= &STR_ROBOTMAIN_LABSIZE_XH;
		pSTR_CLEAN_LABSIZE	= &STR_CLEAN_LABSIZE_XH;
		pSTR_RESERV_LABSIZE	= &STR_RESERV_LABSIZE_XH;
		pSTR_HELP_SIZE		= &STR_HELP_SIZE_XH;
		pSTR_DATE_SIZE		= &STR_DATE_SIZE_XH;
		pSTR_TIME_SIZE		= &STR_TIME_SIZE_XH;
		pSTR_LINK_SIZE		= &STR_LINK_SIZE_XH;
		pSTR_SETUP_SIZE		= &STR_SETUP_SIZE_XH;
		pSTR_CLEANJOB_SIZE	= &STR_CLEANJOB_SIZE_XH;
		pSTR_BTN_SIZE		= &STR_BTN_SIZE_XH;
		pSTR_TOAST_SIZE		= &STR_TOAST_SIZE_XH;
		pSTR_DIALOG_SIZE	= &STR_DIALOG_SIZE_XH;
		pSTR_LATER_SIZE		= &STR_LATER_SIZE_XH;
		break;
	case WINFRAME_XXH:
		pSTR_TITAL_SIZE		= &STR_TITAL_SIZE_XXH;
		pSTR_ERROR_SIZE		= &STR_ERROR_SIZE_XXH;
		pSTR_STATE_SIZE		= &STR_STATE_SIZE_XXH;
		pSTR_WIFILIST_SIZE	= &STR_WIFILIST_SIZE_XXH;
		pSTR_ROBOTMAIN_LABSIZE	= &STR_ROBOTMAIN_LABSIZE_XXH;
		pSTR_CLEAN_LABSIZE	= &STR_CLEAN_LABSIZE_XXH;
		pSTR_RESERV_LABSIZE	= &STR_RESERV_LABSIZE_XXH;
		pSTR_HELP_SIZE		= &STR_HELP_SIZE_XXH;
		pSTR_DATE_SIZE		= &STR_DATE_SIZE_XXH;
		pSTR_TIME_SIZE		= &STR_TIME_SIZE_XXH;
		pSTR_LINK_SIZE		= &STR_LINK_SIZE_XXH;
		pSTR_SETUP_SIZE		= &STR_SETUP_SIZE_XXH;
		pSTR_CLEANJOB_SIZE	= &STR_CLEANJOB_SIZE_XXH;
		pSTR_BTN_SIZE		= &STR_BTN_SIZE_XXH;
		pSTR_TOAST_SIZE		= &STR_TOAST_SIZE_XXH;
		pSTR_DIALOG_SIZE	= &STR_DIALOG_SIZE_XXH;
		pSTR_LATER_SIZE		= &STR_LATER_SIZE_XXH;
		break;
	default:
	case WINFRAME_H:
		pSTR_TITAL_SIZE		= &STR_TITAL_SIZE_H;
		pSTR_ERROR_SIZE		= &STR_ERROR_SIZE_H;
		pSTR_STATE_SIZE		= &STR_STATE_SIZE_H;
		pSTR_WIFILIST_SIZE	= &STR_WIFILIST_SIZE_H;
		pSTR_ROBOTMAIN_LABSIZE	= &STR_ROBOTMAIN_LABSIZE_H;
		pSTR_CLEAN_LABSIZE	= &STR_CLEAN_LABSIZE_H;
		pSTR_RESERV_LABSIZE	= &STR_RESERV_LABSIZE_H;
		pSTR_HELP_SIZE		= &STR_HELP_SIZE_H;
		pSTR_DATE_SIZE		= &STR_DATE_SIZE_H;
		pSTR_TIME_SIZE		= &STR_TIME_SIZE_H;
		pSTR_LINK_SIZE		= &STR_LINK_SIZE_H;
		pSTR_SETUP_SIZE		= &STR_SETUP_SIZE_H;
		pSTR_CLEANJOB_SIZE	= &STR_CLEANJOB_SIZE_H;
		pSTR_BTN_SIZE		= &STR_BTN_SIZE_H;
		pSTR_TOAST_SIZE		= &STR_TOAST_SIZE_H;
		pSTR_DIALOG_SIZE	= &STR_DIALOG_SIZE_H;
		pSTR_LATER_SIZE		= &STR_LATER_SIZE_H;
		break;


	}
	
	eglView->setFrameSize(IMAGE_SIZE_TABLE_W[iIdx_win],IMAGE_SIZE_TABLE_H[iIdx_win] );
	SetWindwsSizeIdx(iIdx_win);
#else 
	//setDesignResolutionSize 手機上用此函數動作
	eglView->setViewName("ROBOT_APP");
	eglView->setFrameSize(540, 960);
#endif
	int iRunState=CCApplication::sharedApplication()->run();
	//這邊增加 釋放THREAD 資源的函數
	CCApplication::sharedApplication()->ManualFree();
	return iRunState;
}


//取得目前螢幕解析度
//#include "windows.h"
//
//	int ScreenHeight, ScreenWidth;
//ScreenHeight = GetSystemMetrics(SM_CYSCREEN); //取得高度的像素
//ScreenWidth = GetSystemMetrics(SM_CXSCREEN); //取得寬度的像素