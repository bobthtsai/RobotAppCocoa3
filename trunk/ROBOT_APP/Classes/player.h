#ifndef __MAPPLAYER_H__
#define __MAPPLAYER_H__
using namespace std;
using namespace PlayerCc;

///////////////////////////////////////////////////////////////////////////
const float GUIMAPSize_DEFRATE=1.0f; //預設初始化面大小
const float GUIMAPSize_MINRATE=0.2f; //縮小程度 (小解析畫面 剛好可以塞下4096 的圖)
const float GUIMAPSize_MAXRATE=10.0f; //放大程度
const float GUIMAPSize_UP_SPACE=0.5f; //1倍以上放大間距
const float GUIMAPSize_DOWN_SPACE=0.1f; //1倍以下縮小間距

//參考playernav.c 
typedef struct
{
	double px,py,pa;  // x,y and direct
} pose_t;

typedef struct
{
	// GTK 框架

	// scrollbar for zooming
	//GtkVScrollbar* zoom_scrollbar;
	//GtkAdjustment* zoom_adjustment;
	double initial_zoom;

	// client
	char* hostname;
	int   port;
	playerc_client_t* client;

	double goal[3];
	int robot_enable_state;  
	int mapSize;
	int mapWidth; // 浮點數計算Index會有誤差
	int mapHeight;
	playerc_map_t* mapArray[71];   // map0
	playerc_map_t* map;   // map0
	playerc_map_t* mapVW; // map10 virtual wall
	playerc_map_t* mapZone; // map30 Zone
	playerc_map_t* mapFP; // map70 foot print
	playerc_planner_t* planner;
	pose_t robot_pose;
	pose_t onmap_pose;  // 機器位置轉換為地圖位置

	// GnomeCanvas
	//GnomeCanvasItem* imageitem;
	//GnomeCanvasItem* robot_item;
	//GnomeCanvasItem* robot_label;
	//GnomeCanvasItem* robot_goal;
	//GnomeCanvasItem* robot_path;
	//GnomeCanvasItem* robot_particle;
	//GnomeCanvasItem* robot_uncertainty;
	unsigned char* map_pixels; // 儲存地圖影像 

	int aa;
	int* moving_path;  // 儲存走過路徑，存在一個仿地圖陣列上

	int testZone;
} gui_data_t;

void GuiData_Init();
int  GuiData_Connect();
void GuiData_Free();
int init_player(gui_data_t* gui_data);
void fini_player(gui_data_t* gui_data);

// 機器位置轉地圖位置
void platPos_to_mapPos(gui_data_t* gui_data, pose_t robot_pose);
// 地圖位置轉機器位置
pose_t mapPos_to_platPos(pose_t mappos,gui_data_t* gui_data);
player_pose2d_t MapPointToRobotPoint(float x,float y);
pose_t getRobotSite(pose_t& robot_pose);
pose_t getRobotSiteByPose2d(pose_t& robot_pose);
//更新地圖
void update_map(gui_data_t* gui_data);
void create_map_image(gui_data_t* gui_data);
void release_map_image(gui_data_t* gui_data);
bool InitGUIMAP(); //初始化地圖
bool IsGUIMAPReady(); //檢查是否曾經下載過地圖
unsigned char* GetGUIMAP();
unsigned char* GetGUIMAPDATA();
unsigned char* GetGUIMAP_Rang();
unsigned char* GetGUIMAP_Pen();
int GetGUIMAP_Height();
int GetGUIMAP_Width();
void SaveGUIMAP(); //備份地圖檔
void FreeImageData(); //釋放記憶體空間
unsigned char*  AddBitmpHead(unsigned char* pImage,int iWidth,int iHeight);
void SaveRangMapToGuidata(CCImage* pRangMap,int iWidth,int iHeight);
void RangMapToGuiData(gui_data_t* gui_data,CCImage* pRangMap,int iWidth,int iHeight);
void SaveMapPenImage(CCImage* pMap,int iWidth,int iHeight);
void PenMapToGuiData(gui_data_t* gui_data,CCImage* pImage,int iWidth,int iHeight);
void MapRangToRobot();//傳輸地圖至ROBOT
void MapPenToRobot();
bool player_MapRelease(gui_data_t* gui_data);
int player_FirstMapRelease(gui_data_t* gui_data);
bool  GuiData_FirstMapRelease(); //  首次建立地圖
bool GuiData_MapRelease();		//更新地圖
void GuiData_MapReleaseShowData(); //將更新資料儲放至顯示陣列中
#endif // __MAPPLAYER_H__