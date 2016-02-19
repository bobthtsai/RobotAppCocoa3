#ifndef PLAYERNAV_GUI_H
#define PLAYERNAV_GUI_H

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libgnomecanvas/libgnomecanvas.h>

#include <libplayerc/playerc.h>
#include <string.h>

#define MIN_DISPLAY_WIDTH 10
#define WINDOW_SIZE_W 720
#define WINDOW_SIZE_H 580
#define MAX_HOSTNAME_LEN 256
#define DATA_FREQ 2

#define ROBOT_RADIUS 0.3 // Robot's size
#define ROBOT_V_ANGLE DTOR(30.0) // Robot圖示上代表前方的Ｖ型角度
#define DOCK_V_ANGLE  DTOR(30.0) // Dock 圖示上代表朝向角的Ｖ型角度
#define MAX_DOCK_NUM 5  // 顯示充電站之最大值

#define COLOR_BLACK     GNOME_CANVAS_COLOR_A(  0,  0,  0,255)
#define COLOR_RED       GNOME_CANVAS_COLOR_A(255,  0,  0,255)
#define COLOR_GREEN     GNOME_CANVAS_COLOR_A(  0,255,  0,255)
#define COLOR_BLUE      GNOME_CANVAS_COLOR_A(  0,  0,255,255)
#define COLOR_PINK      GNOME_CANVAS_COLOR_A(255,  0,255,255)
#define COLOR_YELLOW    GNOME_CANVAS_COLOR_A(255,255,  0,255)
#define COLOR_LIGHTBLUE GNOME_CANVAS_COLOR_A(  0,255,255,255)
#define COLOR_WHITE     GNOME_CANVAS_COLOR_A(255,255,255,255)

#define PATHNAME_MAXLEN 260
#define ROBOT_ALPHA 128
#define PARTICLE_LENGTH 0.25
#define ZOOM_SCALE  1.2
#define VW_START    10 // 虛擬牆起始值
#define VW_END      29 // 虛擬牆結束值
#define ZONE_START  30
#define ZONE_END    49
#define POWER_START 50
#define POWER_END   69

#define MAP_OBSTACLE   39
#define MAP_TRAJECTORY 70

gboolean dragging;
gboolean setting_theta;
gboolean setting_goal;

typedef struct
{
  // GTK 框架
  GtkWindow* main_window;
  GtkBox* vbox;
  GtkBox* hbox;
  GtkScrolledWindow* map_window;
  GnomeCanvas* map_canvas;

  // scrollbar for zooming
  GtkVScrollbar* zoom_scrollbar;
  GtkAdjustment* zoom_adjustment;
  double zoom_value;

  // client
  char* device;
  char* hostname;
  int   port;
  playerc_client_t* client;

  double goal[3];
  int robot_enable_state;  
  int mapSize;
  int mapWidth; // 浮點數計算Index會有誤差
  int mapHeight;
  int mapVW_idx;  // virtual wall map index
  int mapZONE_idx;
  int mapPOWER_idx;
  player_pose2d_t poseMin; // position2d 的最大最小值
  player_pose2d_t poseMax;

  playerc_fiducial_t *fiducial;  
  playerc_map_t* map;   // map0
  playerc_map_t* mapVW;   // map virtual wall
  playerc_map_t* mapZONE; // map target zone
  playerc_map_t* mapPOWER;
  playerc_map_t* mapFP; // map70 foot print
  playerc_map_t* mapOB;   // map obstacle
  playerc_position2d_t* position2d;
  playerc_planner_t* planner;
  player_pose2d_t robot_pose;
  player_pose2d_t onmap_pose;  // 機器位置轉換為地圖位置
  player_pose2d_t dock_pose[MAX_DOCK_NUM]; // Dock位置 (地圖座標)
  player_pose2d_t mouse_pose;       // mouse coordinate
  player_pose2d_t mouse_onmap_pose; // mouse coordinate -> map coord

  player_pose2d_t origin_pose; // 原點，設定為(0,0)

  // GnomeCanvas
  GnomeCanvasItem* imageitem;
  GnomeCanvasItem* robot_item;  // 機器人圖像
  GnomeCanvasItem* robot_label; // 機器人上顯示的名稱
  GnomeCanvasItem* robot_goal;  // M2T目的地
  GnomeCanvasItem* robot_path;
  GnomeCanvasItem* robot_particle;
  GnomeCanvasItem* robot_uncertainty;
  GnomeCanvasItem* dock_item[MAX_DOCK_NUM]; // Dock圖像
  guchar* map_pixels; // 儲存地圖影像 

  int aa;
  int* moving_path;  // 儲存走過路徑，存在一個仿地圖陣列上
} gui_data_t;


typedef struct node { 
  player_pose2d_t data; 
    struct node *next; 
}Node; 
Node *front, *rear; 

void queue_create(); 
void queue_showfront(); 
void queue_add(player_pose2d_t); 
void queue_delete(); 
void queue_showqueue(); 

void update_whole_map(gui_data_t* gui_data);
void update_partial_map(gui_data_t* gui_data);
void update_robot_info(gui_data_t* gui_data, player_pose2d_t pose);
int  update_robot_pose(gui_data_t* gui_data);
void update_status_bar(gui_data_t* gui_data);
int  update_waypoints(gui_data_t* gui_data);
void update_dock_pose(gui_data_t* gui_data);

void create_robot(gui_data_t* gui_data, player_pose2d_t pose);
void create_dock(gui_data_t* gui_data, int idx);
void create_map_image(gui_data_t* gui_data, int ori_X, int ori_Y, int draw_width, int draw_height);
void create_map_menu(gui_data_t* gui_data, int idx_start, int idx_end, GtkMenu* menu, GtkCheckMenuItem** item, int mode);
void create_menu(gui_data_t* gui_data);

void draw_waypoints(gui_data_t* gui_data);
void draw_goal(gui_data_t* gui_data);
void draw_extra_image(gui_data_t* gui_data, int i, int j, int r, int g, int b, int boldSize, int transparent);

void adjustment_zoom_value(gui_data_t* gui_data, int mode);
void destroy_waypoints(gui_data_t* gui_data);
void move_item(GnomeCanvasItem* item, player_pose2d_t pose, int raise, int showMes);
void platPos_to_mapPos(player_pose2d_t* mapPos, player_pose2d_t platPos); // pass struct* will error at *tmp.px
void mapPos_to_platPos(player_pose2d_t* platPos, player_pose2d_t mapPos);
void transfer_dock_coordinate(player_pose2d_t* dock_pose, gui_data_t* gui_data, player_pose3d_t fiducial_pose); // 轉換Dock座標
void reset(gui_data_t* gui_data);
int  subscribe_map_and_load(gui_data_t* gui_data, playerc_map_t* map, int mapIdx, GtkWidget* item, int mode);
int  subscribe_planner(gui_data_t* gui_data); // connect planner plug-in
void unsubscribe_planner(gui_data_t* gui_data); // connect planner plug-in
void switch_map_index(int mapIdx, int* tmpMapIdx, int idxStart, int idxEnd, GtkCheckMenuItem** item);

void init_gui(gui_data_t* gui_data, int argc, char** argv);
void fini_gui(gui_data_t* gui_data);
int  init_player(gui_data_t* gui_data);
void fini_player(gui_data_t* gui_data);

/* Parse command line arguments, of the form host:port */
int parse_args(int argc, char** argv, gui_data_t* gui_data);
void print_usage(int argc, char** argv);

double mapOrigin[2];
double mapResolution;
double zoom_MAX, zoom_MIN; // 地圖縮放的最大、最小值
GtkWidget* status_bar;  // 狀態列 - 顯示訊息

// gtk的menu_check_item，因為要在不同函式控制打勾與否，因此設為全域可用
GtkCheckMenuItem* footprints_item_map; // map
GtkCheckMenuItem* obstacle_item_map; // map
GtkCheckMenuItem* waypoints_item;
#endif
