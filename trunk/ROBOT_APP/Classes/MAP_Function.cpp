#include <ImageList.h>
#include <HelloWorldScene.h>
#include <libplayerc++/playerc++.h>
#include "MAP_Function.h"
USING_NS_CC;
using namespace std;
using namespace PlayerCc;
extern MapProxy *pMapProxy;
//extern PlannerProxy *pPlannerProxy;
//extern 
player_map_info_t stMapInfo;
//////////////////////////////////////////////////////////////////////////
int8_t *pMapData=NULL;
int8_t *pMap_Info=NULL;
int8_t *pMap_Data=NULL;
const int G_BMPHead=54; //sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER)
//取得圖檔資訊
int8_t *GetMapData()
{
	return pMapData;
}
player_map_info_t* GetMapInfo()
{
	return &stMapInfo;
}
void ReleaseMapData()
{
	if(pMapData!=NULL)
		delete []pMapData;
	pMapData=NULL;
}

void InitMapData()
{
	if(pMapData==NULL) //只有執行第一次 日後改為連線第一次
	{
		player_map_info_t *pinfo=&stMapInfo;
		ObtainMapInfo(pMapProxy,pinfo);
		
		int iSize =pinfo->height*pinfo->width*4;
		int iHeadSize=G_BMPHead; //40+14=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER)
		pMapData = new int8_t[iSize+iHeadSize]; //建立圖檔空間
		memset(pMapData, 0x01, sizeof(int8_t)*iSize);
		pMap_Data=CreatBitmpHead(pMapData,pinfo);
		pMap_Info=&pMapData[0];		
	}	


}
void ObtainMapInfo(MapProxy *pMap ,player_map_info_t *pinfo)
{
	pMap->RequestMap();
	pinfo->height = pMap->GetHeight();
	pinfo->width = pMap->GetWidth();
	pinfo->scale = pMap->GetResolution();
	pinfo->origin.px = pMap->GetOriginX();
	pinfo->origin.py = pMap->GetOriginY();
}
bool IsMapDataReady()
{
	if(pMapData!=NULL)
		return true;
	return false;
}
void SaveMapDataToBMP()
{
	//檔案匯出測試
	FILE *ftemp=NULL; //輸出測試!  
	ftemp=fopen("DebugMap.bmp","wb");
	player_map_info_t *pinfo=&stMapInfo;
	if(ftemp!=NULL)
	{ 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
		int iSize =pinfo->height*pinfo->width*3+G_BMPHead; //多了BMP HEAD
#else
		int iSize =pinfo->height*pinfo->width*4;
#endif
		for(int i=0;i<iSize;i++)
			fprintf(ftemp,"%c",pMapData[i]);

		fclose(ftemp);
	}

}
uint32_t trans_coordinate(double x,player_map_info_t* pmap_info)
{
	return ((uint32_t)(((x) - pmap_info->origin.px) / pmap_info->scale + 0.5));
}

int8_t *RequestRawMap()
{
#define MAPGETSIZE 512
	//if (_type == kRawMap)
	{
		//c_time c;
		player_pose2d_t pose={0,0,0};
		player_map_info_t* pmap_info=&stMapInfo;
		//pose= pPlannerProxy->GetPose();
		uint32_t col = trans_coordinate(pose.px,pmap_info);
		uint32_t row = trans_coordinate(pose.py,pmap_info);
		col = std::max((uint32_t)0, col-(MAPGETSIZE/2));
		row = std::max((uint32_t)0, row-(MAPGETSIZE/2));
		int width, height;
		width = height = MAPGETSIZE;
		width = col+width > pmap_info->width?pmap_info->width-col:width;
		height = row+height > pmap_info->width?pmap_info->width-row:height;
		//printf("Partial get from (%u, %u) to (%d, %d)\r\n", col, row, width, height);
		pMapProxy->RequestMap(col, row, width, height, false);
		pMapProxy->GetMap(pMap_Data);

	}
	return pMap_Data; //return array pointer
}

int8_t* CreatBitmpHead(int8_t* pImage,player_map_info_t *pstMapInfo)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	BITMAPFILEHEADER *pstBmpFile=(BITMAPFILEHEADER *)&pImage[0];
	int iHeadSize=sizeof(BITMAPFILEHEADER);
	BITMAPINFOHEADER *pstBmpInfo=(BITMAPINFOHEADER *)&pImage[iHeadSize];
	int iInfoSize=sizeof(BITMAPINFOHEADER);

	memset(pstBmpFile,0,iHeadSize);	
	pstBmpFile->bfType=0x4D42;//"BM"	
	pstBmpFile->bfOffBits=iHeadSize+iInfoSize;//==G_BMPHead 54; //沒有 RGBQUAD
	pstBmpFile->bfSize=pstMapInfo->width*pstMapInfo->height*16+pstBmpFile->bfOffBits;
	memset(pstBmpInfo,0,iInfoSize);	

	pstBmpInfo->biSize=iInfoSize;//0x28; //Bitmap Info Header 的長度
	pstBmpInfo->biWidth=pstMapInfo->width;
	pstBmpInfo->biHeight=pstMapInfo->height;
	pstBmpInfo->biPlanes=01; //點陣圖的位元圖層數
	pstBmpInfo->biBitCount=16; //每個像素的位元數
	pstBmpInfo->biCompression=0;//壓縮方式
	pstBmpInfo->biSizeImage=0; //點陣圖資料的大小  若沒有壓縮 則此欄數值可設為 0
	pstBmpInfo->biXPelsPerMeter= 0x0b12;//pstMapInfo->height/39.37; //水平解析度（吋/公尺）
	pstBmpInfo->biYPelsPerMeter= 0x0b12;//pstMapInfo->height/39.37; //垂直解析度（吋/公尺）
	pstBmpInfo->biClrUsed= 0;	//點陣圖使用的調色盤顏色數
	pstBmpInfo->biClrImportant =0;	//重要的顏色數
	return &pImage[pstBmpFile->bfOffBits];
#else
	return pImage;
#endif
	

}


