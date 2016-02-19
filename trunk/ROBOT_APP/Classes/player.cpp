/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2004
 *     Brian Gerkey
 */

#include <assert.h>
#include <ImageList.h>
#include <HelloWorldScene.h>
#include <libplayerc++/playerc++.h>

#include "player.h"
#include "MAP_Function.h"
#include <dbg.h>

#include <semaphore.h>

//////////////////////////////////////////////////////////////////////////
//參考 V:\MSI_TASK\SlamRobot\brain\applications\external\player-git-svn\utils\playernav2\playernav.c
//重新撰寫Get MAP 跟 增加 update_partial_map 函數
//playerc_map_get_partial_map
//登入範例
//client = new PlayerCc::PlayerClient(cAddr, SERVER_PORT);
//client->SetRequestTimeout(MAX_REQUEST_TIME);
//設定掃地時間範例
//player_dispatcher_time_t time;
//player_dispatcher_timezone_t zone;
//zone = getCalendarTimeZone(env, calendar);
//time = getCalendarTime(env, calendar);
//PlayerCc::DispatcherProxy* dispatcher = (PlayerCc::DispatcherProxy*) proxy;
//dispatcher->SetUiTime(PLAYER_DISPATCH_INIT_DEST_RD, time, zone);
//機台狀態範例
//error and worry code please reference 168_dispatcher.def
//uint32_t aiStatus;
//uint32_t cleanStatus;
//uint32_t error;
//uint32_t worry;
//
//dispatcherProxy.GetAiStatus(PLAYER_DISPATCH_INIT_DEST_RD,
//	&aiStatus,
//	&cleanStatus,
//	&error,
//	&worry);
//printf("aiStatus=%d cleanStatus=%d error=%d worry=%d\n", aiStatus, cleanStatus, error, worry);
//設定機台時間
//		  player_dispatcher_time_t time;
//player_dispatcher_timezone_t zone;
//
//time.tm_year= 2014;
//time.tm_mon= 1;     //1~12
//time.tm_mday= 7;    //1~31
//time.tm_hour= 10;   //0~23
//time.tm_min= 30;    //0~59
//time.tm_sec= 30;    //0~59
//zone.minuteswest=0;
//zone.dsttime=0;
//dispatcherProxy.SetUiTime(PLAYER_DISPATCH_INIT_DEST_RD, time, zone);
gui_data_t stgui_data;  // 所有 GUI 的資訊
static unsigned char* pImageData= NULL;
static unsigned char* pRangImageData= NULL;
static unsigned char* pPenImageData= NULL;
const int refreshMapSize = 450; //預設每次讀取地圖的範圍
extern Position2dProxy *pPosition2dProxy;
sem_t semMapBeUsd;	//更新地圖被使用中
void GuiData_Init()
{
	memset(&stgui_data, 0, sizeof(stgui_data));
	stgui_data.initial_zoom = 1.0;
	stgui_data.aa = 1;
	stgui_data.map_pixels = NULL;
	stgui_data.testZone = 0;
	stgui_data.port=6665;
	stgui_data.hostname="192.168.30.1";	

	sem_init(&semMapBeUsd,0,0);
}
int GetGUIMAP_Width()
{
#ifdef ROBOT_DISABLE
	return 2048;
#endif
	return stgui_data.map->width;
}
int GetGUIMAP_Height()
{
#ifdef ROBOT_DISABLE
	return 2048;
#endif

	return stgui_data.map->height;
}
unsigned char* GetGUIMAP()
{
	return pImageData;
}
unsigned char* GetGUIMAP_Rang()
{
	return pRangImageData;
}
unsigned char* GetGUIMAP_Pen()
{
	return pPenImageData;
}
unsigned char* GetGUIMAPDATA()
{
	BITMAPFILEHEADER *pstBmpFile=(BITMAPFILEHEADER *)&pImageData[0];
	return &pImageData[pstBmpFile->bfOffBits];
}
int  GuiData_Connect()
{
	return init_player(&stgui_data);
}

bool  GuiData_FirstMapRelease()
{
	return InitGUIMAP();	
}
bool GuiData_MapRelease()
{
	return player_MapRelease(&stgui_data);
}
void GuiData_MapReleaseShowData()
{
	release_map_image(&stgui_data);
};
void GuiData_Free()
{
	fini_player(&stgui_data);
}

pose_t getRobotSite(pose_t& robot_pose)
{
  if(stgui_data.planner && stgui_data.planner->info.fresh)
  {
	robot_pose.px = stgui_data.planner->px;
	robot_pose.py = stgui_data.planner->py;
	robot_pose.pa = stgui_data.planner->pa;	
	//onmap_pose = stgui_data.onmap_pose;
  }
  platPos_to_mapPos(&stgui_data, robot_pose); // 更新地圖位置
  return stgui_data.onmap_pose;
}
//不透過planner 改為Position2dProxy
pose_t getRobotSiteByPose2d(pose_t& robot_pose)
{
	if(pPosition2dProxy!=NULL)
	{
		robot_pose.px = pPosition2dProxy->GetXPos();
		robot_pose.py = pPosition2dProxy->GetYPos();
		robot_pose.pa = pPosition2dProxy->GetYaw();	
	}
	platPos_to_mapPos(&stgui_data, robot_pose); // 更新地圖位置
	return stgui_data.onmap_pose;
}
// 機器位置轉地圖位置
void platPos_to_mapPos(gui_data_t* gui_data, pose_t robot_pose)
{
  // 化為地圖單位
  gui_data->onmap_pose.px = (robot_pose.px - gui_data->map->origin[0]) / gui_data->map->resolution +0.5;
  gui_data->onmap_pose.py = (robot_pose.py - gui_data->map->origin[1]) / gui_data->map->resolution +0.5;
}
player_pose2d_t MapPointToRobotPoint(float x,float y)
{
	player_pose2d_t RobotPoint;
	pose_t mappos;
	mappos.px=x;
	mappos.py=y;
	mappos.pa=0;
	mappos= mapPos_to_platPos(mappos,&stgui_data);
	RobotPoint.px=mappos.px;
	RobotPoint.py=mappos.py;
	RobotPoint.pa=mappos.pa;
	return RobotPoint;
}
// 地圖位置轉機器位置
pose_t mapPos_to_platPos(pose_t mappos,gui_data_t* gui_data)
{
	pose_t RobotPose;
	// 化為機器位置
	RobotPose.px = (mappos.px-0.5) * gui_data->map->resolution + gui_data->map->origin[0]; 
	RobotPose.py = (mappos.py-0.5) * gui_data->map->resolution + gui_data->map->origin[1];	
	RobotPose.pa = 0;	
	return RobotPose;
}

/*
 * Connect to player at each host:port pair, as specified by the global
 * vars 'hostnames' and 'ports'.  Also subscribes to each device, and adds 
 * each client into a new multiclient (which is returned)
 */
int init_player(gui_data_t* gui_data)
{
  /* Connect to Player */
  gui_data->client = playerc_client_create(NULL, gui_data->hostname, gui_data->port);
  assert(gui_data->client);
  
  if(playerc_client_connect(gui_data->client) < 0)
  {
	fprintf(stderr, "Failed to connect to %s:%d\n\n", gui_data->hostname, gui_data->port);
	return -1;
  }
  for(int i=0;i<71;i++)
  {
	  gui_data->mapArray[i] = playerc_map_create(gui_data->client, i);
	  assert(gui_data->mapArray[i]);
	  if(playerc_map_subscribe(gui_data->mapArray[i], PLAYER_OPEN_MODE) < 0)
	  {		  
		  DBG("Failed to subscribe to map%d\n\n",i);
		  return -1;
	  }
  }
  gui_data->map = gui_data->mapArray[0];
  gui_data->mapVW = gui_data->mapArray[10];
  gui_data->mapZone = gui_data->mapArray[30];
  gui_data->mapFP = gui_data->mapArray[70];
 // // create map
 // gui_data->map = playerc_map_create(gui_data->client, 0);
 // assert(gui_data->map);
 // if(playerc_map_subscribe(gui_data->map, PLAYER_OPEN_MODE) < 0)
 // {
	//fprintf(stderr, "Failed to subscribe to map (map0)\n\n");
	//return -1;
 // }
 // 
 // // virtual wall map
 // gui_data->mapVW = playerc_map_create(gui_data->client, 10);
 // assert(gui_data->mapVW);
 // if(playerc_map_subscribe(gui_data->mapVW, PLAYER_OPEN_MODE) < 0)
 // {
	//DBG("Failed to subscribe to virtual wall map (map10)\n\n");
	//gui_data->mapVW = NULL;
 // }
 // gui_data->mapZone = playerc_map_create(gui_data->client, 30);
 // assert(gui_data->mapZone);
 // if(playerc_map_subscribe(gui_data->mapZone, PLAYER_OPEN_MODE) < 0)
 // {
	//  DBG("Failed to subscribe to ZONE map (map30)\n\n");
	//  gui_data->mapZone = NULL;
 // }
 // // foot print map
 // gui_data->mapFP = playerc_map_create(gui_data->client, 70);
 // assert(gui_data->mapFP);
 // if(playerc_map_subscribe(gui_data->mapFP, PLAYER_OPEN_MODE) < 0)
 // {
	//DBG("Failed to subscribe to foot print map (map70)\n\n");
	//gui_data->mapFP = NULL;
 // }

  /* Get the map, just so we know how big to make the window */
 // if(playerc_map_get_map(gui_data->map) < 0)
 // {
	//DBG("init_player:Failed to get map\n");//fprintf(stderr, "Failed to get map\n");
	// no map yet; guess some values to initialize the GUI
	//gui_data->map->width = gui_data->map->height = 400;
	//gui_data->map->resolution = 0.1;
	//gui_data->map->origin[0] = -20.0;
	//gui_data->map->origin[1] = -20.0;
 // }
  //bobadd for VW and FP 
  //playerc_map_get_map(gui_data->mapFP);
  //playerc_map_get_map(gui_data->mapVW);

 // gui_data->mapSize  = gui_data->map->width * gui_data->map->height;
 // gui_data->mapWidth  = gui_data->map->width; // int
 // gui_data->mapHeight = gui_data->map->height;
 // DBG("Map size: ( %d, %d), origin: ( %.4f, %.4f) resolution: %.4f\n"
	//, gui_data->map->width,     gui_data->map->height
	//, gui_data->map->origin[0], gui_data->map->origin[1], gui_data->map->resolution);

 // // 創立與初始化行經路線圖
 // gui_data->moving_path = (int* )malloc(  gui_data->mapSize * sizeof(int));
 // memset(gui_data->moving_path, 0, gui_data->mapSize * sizeof(int));

  //gui_data->planner = playerc_planner_create(gui_data->client, 0);
  //assert(gui_data->planner);
  //if(playerc_planner_subscribe(gui_data->planner, PLAYER_OPEN_MODE) < 0)
  //{
	 // fprintf(stderr, "Warning: Failed to subscribe to planner on robot; you won't be able to set robot's goal.\n");
	 // playerc_planner_destroy(gui_data->planner);
	 // gui_data->planner = NULL;
  //}
  
  return 1;
}

int player_FirstMapRelease(gui_data_t* gui_data)
{	
	
	return 1;
}
//修改於 V:\MSI_TASK\SlamRobot\brain\applications\external\player-git-svn\utils\playernav2\playernav.c 
//	void update_partial_map(gui_data_t* gui_data)
bool player_MapRelease(gui_data_t* gui_data)
{
		int oriIntX, oriIntY;
		int remainX, remainY;
		oriIntX = (int)gui_data->onmap_pose.px - (refreshMapSize / 2);
		oriIntY = (int)gui_data->onmap_pose.py - (refreshMapSize / 2);

		oriIntX = (oriIntX < 0) ? 0 : oriIntX;
		oriIntY = (oriIntY < 0) ? 0 : oriIntY;

		remainX = (oriIntX + refreshMapSize) > gui_data->map->width  ? gui_data->map->width  - oriIntX : refreshMapSize;
		remainY = (oriIntY + refreshMapSize) > gui_data->map->height ? gui_data->map->height - oriIntY : refreshMapSize;

		if(playerc_map_get_partial_map(gui_data->map, 
			oriIntX, oriIntY, remainX, remainY, true) < 0)
		{
			DBG("player_MapRelease:Failed to get partial map \n");
			return false;
		}
		//if(showFootprintsMap_Flag)
		//{
		//	if(playerc_map_get_partial_map(gui_data->mapFP, 
		//		oriIntX, oriIntY, remainX, remainY, false) < 0)
		//	{
		//		fprintf(stderr, "Press \"show footprints by map\" but Failed to get coverage map (map70)\n");
		//		showFootprintsMap_Flag = !showFootprintsMap_Flag;
		//		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(footprints_item_map), FALSE);
		//	}
		//}
		return true;
}

void fini_player(gui_data_t* gui_data)
{
  puts("\nDestroy Connect\n");
  if(gui_data->map)
	playerc_map_destroy(gui_data->map);
  if(gui_data->mapVW)
	playerc_map_destroy(gui_data->mapVW);

  if(gui_data->mapZone)
	  playerc_map_destroy(gui_data->mapZone);

  if(gui_data->mapFP)
	playerc_map_destroy(gui_data->mapFP);
  if(gui_data->planner)
	playerc_planner_destroy(gui_data->planner);
  if(gui_data->client)
	playerc_client_destroy(gui_data->client); 
}
bool InitGUIMAP()
{
#ifdef ROBOT_DISABLE
	update_map(&stgui_data);
	return;
#endif
	update_map(&stgui_data);
	int peek = playerc_client_peek(stgui_data.client, 10); //設定十秒TIMEOUT
	if(peek < 0)  // wrong
	{
		DBG("Error on peek\n");		
		return false;
	}
	if(playerc_client_requestdata(stgui_data.client) < 0)
	{
		DBG("playerc_client_requestdata errored \n");
	}
	playerc_client_read_nonblock(stgui_data.client);
	sem_post(&semMapBeUsd);	//告知可以取圖了
	return true;
}

bool IsGUIMAPReady()
{
	if(pImageData!=NULL)
		return true;
	return false;
}
void update_map(gui_data_t* gui_data)
{
#ifdef ROBOT_DISABLE
	if(pImageData==NULL)  // 已經有圖，free之後重創一張
	{
		int iHeadSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
		pImageData = (unsigned char*)malloc(sizeof(unsigned char) * 3 * 2048*2048 +iHeadSize )
		assert(pImageData == NULL);
	}
	unsigned char* pixels=AddBitmpHead(pImageData,2048,2048);
	memset(pixels,100,2048*2048*3);
	
	return;
#endif

	if(playerc_map_get_map(gui_data->map) < 0)
	{
		//fprintf(stderr, "Failed to get map\n");
		DBG("update_map: Failed to get map\n");
	}
	else
	{
		//if(gui_data->imageitem)
		//	gtk_object_destroy(GTK_OBJECT(gui_data->imageitem));
		create_map_image(gui_data);
	}
}
// 回存地圖
void ReSet_Rangmap(gui_data_t* gui_data)
{
	playerc_map_set_map(gui_data->mapZone,gui_data->mapZone->cells);
}
void ReSet_Penmap(gui_data_t* gui_data)
{
	playerc_map_set_map(gui_data->mapVW,gui_data->mapVW->cells);
}
//////////////////////////////////////////////////////////////////////////
void SaveGUIMAP()
{
	//檔案匯出測試
	FILE *ftemp=NULL; //輸出測試!  
	ftemp=fopen("DebugGUIMap.bmp","wb");
	if(ftemp!=NULL)
	{ 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
		int iSize =2048*2048*3+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); //多了BMP HEAD
#else
		int iSize =pinfo->height*pinfo->width*4;
#endif
		fwrite(pImageData,sizeof(unsigned char),iSize,ftemp);
		//for(int i=0;i<iSize;i++)
		//	fprintf(ftemp,"%c",pImageData[i]);
		fclose(ftemp);
	}
}
unsigned char*  AddBitmpHead(unsigned char* pImage,int iWidth,int iHeight)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	BITMAPFILEHEADER *pstBmpFile=(BITMAPFILEHEADER *)&pImage[0];
	int iHeadSize=sizeof(BITMAPFILEHEADER);
	BITMAPINFOHEADER *pstBmpInfo=(BITMAPINFOHEADER *)&pImage[iHeadSize];
	int iInfoSize=sizeof(BITMAPINFOHEADER);

	memset(pstBmpFile,0,iHeadSize);	
	pstBmpFile->bfType=0x4D42;//"BM"	
	pstBmpFile->bfOffBits=iHeadSize+iInfoSize;//==G_BMPHead 54; //沒有 RGBQUAD
	pstBmpFile->bfSize=iWidth*iHeight*3+pstBmpFile->bfOffBits;
	memset(pstBmpInfo,0,iInfoSize);	

	pstBmpInfo->biSize=iInfoSize;//0x28; //Bitmap Info Header 的長度
	pstBmpInfo->biWidth=iWidth;
	pstBmpInfo->biHeight=iHeight;
	pstBmpInfo->biPlanes=1; //點陣圖的位元圖層數
	pstBmpInfo->biBitCount=24; //每個像素的位元數
	pstBmpInfo->biCompression=0;//壓縮方式
	pstBmpInfo->biSizeImage=0; //點陣圖資料的大小  若沒有壓縮 則此欄數值可設為 0
	pstBmpInfo->biXPelsPerMeter= iWidth/39.37; //水平解析度（吋/公尺）
	pstBmpInfo->biYPelsPerMeter= iHeight/39.37; //垂直解析度（吋/公尺）
	pstBmpInfo->biClrUsed= 0;	//點陣圖使用的調色盤顏色數
	pstBmpInfo->biClrImportant =0;	//重要的顏色數
	return &pImage[pstBmpFile->bfOffBits];
#else
	return pImage;
#endif


}
void FreeImageData()
{
	if(pImageData!=NULL)
		free(pImageData);
	if(pRangImageData!=NULL)
		free(pRangImageData);
	if(pPenImageData!=NULL)
		free(pPenImageData);


}

//傳輸地圖至ROBOT
void MapRangToRobot()
{
	 playerc_map_set_map(stgui_data.mapZone,(char*)pRangImageData);
}
//記錄區域範圍
void SaveRangMapToGuidata(CCImage* pRangMap,int iWidth,int iHeight)
{
	RangMapToGuiData(&stgui_data,pRangMap,iWidth,iHeight);
}
void RangMapToGuiData(gui_data_t* gui_data,CCImage* pImage,int iWidth,int iHeight)
{
	gui_data->mapZone->width=iWidth;
	gui_data->mapZone->height=iHeight;
	unsigned char* data_ = pImage->getData();	
	unsigned int *pImageStart= (unsigned int *)data_;
	unsigned int *pixel = NULL;
	ccColor4B PixelColor;
	int iFlage=0;
	for(int j=0; j < gui_data->mapZone->height; j++)
	{
		for(int i=0; i < gui_data->mapZone->width; i++)
		{
			pixel =pImageStart+ (j * iWidth) * 1 + i * 1;
			PixelColor.r =  *pixel & 0xff;
			PixelColor.g =  (*pixel >> 8) & 0xff;
			PixelColor.b=  (*pixel >> 16) & 0xff;
			PixelColor.a =  (*pixel >> 24) & 0xff;
			if(PixelColor.r!=0 ) // 有紅色==RangColor_FullRed.r
			{ 
				pRangImageData[PLAYERC_MAP_INDEX(gui_data->mapZone,i,j)] =1;
				iFlage++;	
			}
			else
				pRangImageData[PLAYERC_MAP_INDEX(gui_data->mapZone,i,j)] =0;
		}
	}

	DBG("Rang map iFlage=%d\n",iFlage);
}
//VW WALL 
void MapPenToRobot()
{
	playerc_map_set_map(stgui_data.mapVW,(char*)pPenImageData);
}

void SaveMapPenImage(CCImage* pMap,int iWidth,int iHeight)
{
	PenMapToGuiData(&stgui_data,pMap,iWidth,iHeight);
}
void PenMapToGuiData(gui_data_t* gui_data,CCImage* pImage,int iWidth,int iHeight)
{
	gui_data->mapVW->width=iWidth;
	gui_data->mapVW->height=iHeight;
	unsigned char* data_ = pImage->getData();	
	unsigned int *pImageStart= (unsigned int *)data_;
	unsigned int *pixel = NULL;
	ccColor4B PixelColor;
	int iFlage=0;

	for(int j=0; j < gui_data->mapVW->height; j++)
	{
		for(int i=0; i < gui_data->mapVW->width; i++)
		{
			pixel =pImageStart+ (j * iWidth) * 1 + i * 1;
			PixelColor.r =  *pixel & 0xff;
			PixelColor.g =  (*pixel >> 8) & 0xff;
			PixelColor.b=  (*pixel >> 16) & 0xff;
			PixelColor.a =  (*pixel >> 24) & 0xff;
			if(PixelColor.r!=0 ) // 有紅色==RangColor_FullRed.r
			{ 
				pPenImageData[PLAYERC_MAP_INDEX(gui_data->mapVW,i,j)] =1;
				iFlage++;	
			}
			else
				pPenImageData[PLAYERC_MAP_INDEX(gui_data->mapVW,i,j)] =0;
		}
	}

	DBG("Pen map iFlage=%d\n",iFlage);


}
//將gui_data->Map 更新至pImageData
void release_map_image(gui_data_t* gui_data)
{
	sem_wait(&semMapBeUsd); //確認沒有在使用 pImageData
	//日後此迴圈應該改為函數
	unsigned char* pixels=AddBitmpHead(pImageData,gui_data->map->width,gui_data->map->height);
	memset(pixels,100,gui_data->map->height*gui_data->map->width*3);
	int i,j;
	for(j=0; j < gui_data->map->height; j++)
	{
		for(i=0; i < gui_data->map->width; i++)
		{
			int tempIdx;

			tempIdx = ((gui_data->map->height - j - 1) * gui_data->map->width + i) * 3;
			// tempIdx = gui_data->map->width * j + i)*3; // Old <--- 因為原點不在中央？
			// Map cell : empty = -1, unknown = 0, occupied = +1
			if(gui_data->map->cells[PLAYERC_MAP_INDEX(gui_data->map,i,j)] == -1)
			{
				pixels[tempIdx]     = 255;  // 白
				pixels[tempIdx + 1] = 255;
				pixels[tempIdx + 2] = 255;
			}
			// else if(gui_data->map->cells[PLAYERC_MAP_INDEX(gui_data->map,i,j)] == 0)
			// {
			//pixels[tempIdx]     = 100;  // 灰
			//pixels[tempIdx + 1] = 100;
			//pixels[tempIdx + 2] = 100;
			// }
			else if(gui_data->map->cells[PLAYERC_MAP_INDEX(gui_data->map,i,j)] != 0)
			{
				pixels[tempIdx]     = 5;    // 黑
				pixels[tempIdx + 1] = 5;
				pixels[tempIdx + 2] = 5;
			}			
		}
	}
	sem_post(&semMapBeUsd);

}
void create_map_image(gui_data_t* gui_data)
{
  //GnomeCanvasItem* mapCanvas_empty;
  //GnomeCanvasItem* mapCanvas_occupied;
  //GnomeCanvasItem* mapCanvas_coverage;

  //GdkPixbuf* pixbuf;
  

  int i,j;
  int mapsize;
//  double map_coverage;

  mapsize = gui_data->map->width * gui_data->map->height;

  if(!gui_data->map->width || !gui_data->map->height)
  {
	DBG("No map");
	//gui_data->imageitem = NULL;
	return;
  }

  if(pImageData == NULL)  // 已經有圖，free之後重創一張
  {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	  int iHeadSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	  pImageData = (unsigned char*)malloc(sizeof(unsigned char) * 3 * mapsize +iHeadSize );
	  assert(pImageData != NULL);
#else
	  pImageData = (unsigned char*)malloc(sizeof(unsigned char) * 3 * mapsize +iHeadSize );	  
	  assert(pImageData !=NULL);
#endif

  }
	
  
  unsigned char* pixels=AddBitmpHead(pImageData,gui_data->map->width,gui_data->map->height);
  memset(pixels,100,gui_data->map->height*gui_data->map->width*3);
  for(j=0; j < gui_data->map->height; j++)
  {
	  for(i=0; i < gui_data->map->width; i++)
	  {
		  int tempIdx;

	  tempIdx = ((gui_data->map->height - j - 1) * gui_data->map->width + i) * 3;
	  // tempIdx = gui_data->map->width * j + i)*3; // Old <--- 因為原點不在中央？
	  
	  // Map cell : empty = -1, unknown = 0, occupied = +1
	  if(gui_data->map->cells[PLAYERC_MAP_INDEX(gui_data->map,i,j)] == -1)
	  {
		pixels[tempIdx]     = 255;  // 白
		pixels[tempIdx + 1] = 255;
		pixels[tempIdx + 2] = 255;
	  }
	 // else if(gui_data->map->cells[PLAYERC_MAP_INDEX(gui_data->map,i,j)] == 0)
	 // {
		//pixels[tempIdx]     = 100;  // 灰
		//pixels[tempIdx + 1] = 100;
		//pixels[tempIdx + 2] = 100;
	 // }
	  else if(gui_data->map->cells[PLAYERC_MAP_INDEX(gui_data->map,i,j)] != 0)
	  {
		pixels[tempIdx]     = 5;    // 黑
		pixels[tempIdx + 1] = 5;
		pixels[tempIdx + 2] = 5;
	  }
	  //範圍地圖
	  if(pRangImageData == NULL)
	  {
		  pRangImageData= (unsigned char*)malloc(sizeof(unsigned char) * 3 * mapsize);
		  assert(pRangImageData != NULL);
	  }

	  // Virtual Wall
	  if(pPenImageData == NULL)
	  {
		  pPenImageData = (unsigned char*)malloc(sizeof(unsigned char) * 3 * mapsize);		  
		  assert(pRangImageData != NULL);
	  }
	  //if(showVirtualWall_Flag  && gui_data->mapVW->cells[PLAYERC_MAP_INDEX(gui_data->mapVW,i,j)] != 0)        
	  //{
	  //  // 加粗線條
	  //  int tempX, tempY;
	  //  int addWidth; // 加寬格數

	  //  addWidth = 3;

	  //  //    printf("Decode position (%d,%d)\n", pX, pY);
	  //  for(tempY = j-addWidth ; tempY < j+addWidth; tempY++){
	  //    for(tempX = i-addWidth ; tempX < i+addWidth; tempX++){              

	  //      tempIdx = ((gui_data->map->height - tempY - 1) * gui_data->map->width + tempX) * 3;

	  //      if( (tempX >= 0 && tempX < gui_data->map->width) &&
	  //          (tempY >= 0 && tempY < gui_data->map->height) &&
	  //           tempIdx < (mapsize * 3) )
	  //      {
	  //        pixels[tempIdx]     = 0;
	  //        pixels[tempIdx + 1] = 225;
	  //        pixels[tempIdx + 2] = 0;
	  //      }
	  //    }
	  //  }
	  //}

	  // ------軌跡 - Start------ 
	  // 目前指令為畫軌跡且軌跡地圖中有值，不能寫在畫地圖前是因為線條要加寬
	  //if(showFootprints_Flag && gui_data->moving_path[tempIdx/3] > 0)
	  //{
	  //  // 加粗線條
	  //  int tempX, tempY;
	  //  int addWidth; // 加寬格數

	  //  addWidth = 3;

	  //  //    printf("Decode position (%d,%d)\n", pX, pY);
	  //  for(tempY = j-addWidth ; tempY < j+addWidth; tempY++){
	  //    for(tempX = i-addWidth ; tempX < i+addWidth; tempX++){              

	  //      tempIdx = ((gui_data->map->height - tempY - 1) * gui_data->map->width + tempX) * 3;

	  //      if( (tempX >= 0 && tempX < gui_data->map->width) &&
	  //          (tempY >= 0 && tempY < gui_data->map->height) &&
	  //           tempIdx < (mapsize * 3) )
	  //      {
	  //        pixels[tempIdx]     = 225;
	  //        pixels[tempIdx + 1] = 0;
	  //        pixels[tempIdx + 2] = 0;
	  //      }
	  //    }
	  //  }
	  //}
	  // ------ 軌跡 -- End -----
	}
  }
  //Pen Wall 空間
  if(gui_data->mapZone->cells==NULL) //建立區域空間
  {
	gui_data->mapZone->cells = (char*)realloc(gui_data->mapZone->cells,sizeof(char)*mapsize); 
	assert(gui_data->mapZone->cells);
  }
  //Virtual Wall 空間
  
  if(gui_data->mapVW->cells==NULL) //建立區域空間
  {
	gui_data->mapVW->cells = (char*)realloc(gui_data->mapVW->cells,sizeof(char)*mapsize); 
	assert(gui_data->mapVW->cells);
  }


}
