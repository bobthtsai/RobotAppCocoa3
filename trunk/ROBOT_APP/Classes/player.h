#ifndef __MAPPLAYER_H__
#define __MAPPLAYER_H__
using namespace std;
using namespace PlayerCc;

///////////////////////////////////////////////////////////////////////////
const float GUIMAPSize_DEFRATE=1.0f; //�w�]��l�ƭ��j�p
const float GUIMAPSize_MINRATE=0.2f; //�Y�p�{�� (�p�ѪR�e�� ��n�i�H��U4096 ����)
const float GUIMAPSize_MAXRATE=10.0f; //��j�{��
const float GUIMAPSize_UP_SPACE=0.5f; //1���H�W��j���Z
const float GUIMAPSize_DOWN_SPACE=0.1f; //1���H�U�Y�p���Z

//�Ѧ�playernav.c 
typedef struct
{
	double px,py,pa;  // x,y and direct
} pose_t;

typedef struct
{
	// GTK �ج[

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
	int mapWidth; // �B�I�ƭp��Index�|���~�t
	int mapHeight;
	playerc_map_t* mapArray[71];   // map0
	playerc_map_t* map;   // map0
	playerc_map_t* mapVW; // map10 virtual wall
	playerc_map_t* mapZone; // map30 Zone
	playerc_map_t* mapFP; // map70 foot print
	playerc_planner_t* planner;
	pose_t robot_pose;
	pose_t onmap_pose;  // ������m�ഫ���a�Ϧ�m

	// GnomeCanvas
	//GnomeCanvasItem* imageitem;
	//GnomeCanvasItem* robot_item;
	//GnomeCanvasItem* robot_label;
	//GnomeCanvasItem* robot_goal;
	//GnomeCanvasItem* robot_path;
	//GnomeCanvasItem* robot_particle;
	//GnomeCanvasItem* robot_uncertainty;
	unsigned char* map_pixels; // �x�s�a�ϼv�� 

	int aa;
	int* moving_path;  // �x�s���L���|�A�s�b�@�ӥ�a�ϰ}�C�W

	int testZone;
} gui_data_t;

void GuiData_Init();
int  GuiData_Connect();
void GuiData_Free();
int init_player(gui_data_t* gui_data);
void fini_player(gui_data_t* gui_data);

// ������m��a�Ϧ�m
void platPos_to_mapPos(gui_data_t* gui_data, pose_t robot_pose);
// �a�Ϧ�m�������m
pose_t mapPos_to_platPos(pose_t mappos,gui_data_t* gui_data);
player_pose2d_t MapPointToRobotPoint(float x,float y);
pose_t getRobotSite(pose_t& robot_pose);
pose_t getRobotSiteByPose2d(pose_t& robot_pose);
//��s�a��
void update_map(gui_data_t* gui_data);
void create_map_image(gui_data_t* gui_data);
void release_map_image(gui_data_t* gui_data);
bool InitGUIMAP(); //��l�Ʀa��
bool IsGUIMAPReady(); //�ˬd�O�_���g�U���L�a��
unsigned char* GetGUIMAP();
unsigned char* GetGUIMAPDATA();
unsigned char* GetGUIMAP_Rang();
unsigned char* GetGUIMAP_Pen();
int GetGUIMAP_Height();
int GetGUIMAP_Width();
void SaveGUIMAP(); //�ƥ��a����
void FreeImageData(); //����O����Ŷ�
unsigned char*  AddBitmpHead(unsigned char* pImage,int iWidth,int iHeight);
void SaveRangMapToGuidata(CCImage* pRangMap,int iWidth,int iHeight);
void RangMapToGuiData(gui_data_t* gui_data,CCImage* pRangMap,int iWidth,int iHeight);
void SaveMapPenImage(CCImage* pMap,int iWidth,int iHeight);
void PenMapToGuiData(gui_data_t* gui_data,CCImage* pImage,int iWidth,int iHeight);
void MapRangToRobot();//�ǿ�a�Ϧ�ROBOT
void MapPenToRobot();
bool player_MapRelease(gui_data_t* gui_data);
int player_FirstMapRelease(gui_data_t* gui_data);
bool  GuiData_FirstMapRelease(); //  �����إߦa��
bool GuiData_MapRelease();		//��s�a��
void GuiData_MapReleaseShowData(); //�N��s����x�����ܰ}�C��
#endif // __MAPPLAYER_H__