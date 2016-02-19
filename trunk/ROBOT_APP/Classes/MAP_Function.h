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
int8_t* CreatBitmpHead(int8_t* pImage,player_map_info_t *pstMapInfo);	//���o���ɼ��Y
void ReleaseMapData();	//������ɰO����Ŷ�
void InitMapData();	// �إ߹��ɰO����Ŷ�
bool IsMapDataReady();	//�ˬd�O�_�����J����
void SaveMapDataToBMP();	//�N�{���a���� �|�X�T�w�ɦW

int8_t *GetMapData();		//���o���ɸ�T
player_map_info_t* GetMapInfo();



//typedef struct node { 
//	pose_t data; 
//	struct node *next; 
//}Node; 
//Node *front, *rear; 


#endif // __MAPFUNCTION_H__