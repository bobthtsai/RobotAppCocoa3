#ifndef __MAPFUNCTION_H__
#define __MAPFUNCTION_H__
#include "cocos2d.h"
using namespace cocos2d;
USING_NS_CC;
using namespace std;
using namespace PlayerCc;


void ObtainMapInfo(MapProxy *pMap ,player_map_info_t *info);
int8_t *RequestRawMap();
uint32_t trans_coordinate(double x,player_map_info_t* pmap_info);		
int8_t* CreatBitmpHead(int8_t* pImage,player_map_info_t *pstMapInfo);	//取得圖檔標頭
void ReleaseMapData();	//釋放圖檔記憶體空間
void InitMapData();	// 建立圖檔記憶體空間
bool IsMapDataReady();	//檢查是否有載入圖檔
void SaveMapDataToBMP();	//將現有地圖檔 會出固定檔名

int8_t *GetMapData();		//取得圖檔資訊
player_map_info_t* GetMapInfo();



//typedef struct node { 
//	pose_t data; 
//	struct node *next; 
//}Node; 
//Node *front, *rear; 


#endif // __MAPFUNCTION_H__