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
#define ROBOT_V_ANGLE DTOR(30.0)

#define COLOR_BLACK     GNOME_CANVAS_COLOR_A(  0,  0,  0,255)
#define COLOR_RED       GNOME_CANVAS_COLOR_A(255,  0,  0,255)
#define COLOR_GREEN     GNOME_CANVAS_COLOR_A(  0,255,  0,255)
#define COLOR_BLUE      GNOME_CANVAS_COLOR_A(  0,  0,255,255)
#define COLOR_PINK      GNOME_CANVAS_COLOR_A(255,  0,255,255)
#define COLOR_YELLOW    GNOME_CANVAS_COLOR_A(255,255,  0,255)
#define COLOR_LIGHTBLUE GNOME_CANVAS_COLOR_A(  0,255,255,255)
#define COLOR_WHITE     GNOME_CANVAS_COLOR_A(255,255,255,255)

gboolean dragging;
gboolean setting_theta;
gboolean setting_goal;

typedef struct
{
	double px,py,pa;  // x,y and direct
} pose_t;

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
	double initial_zoom;

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

	playerc_map_t* map;   // map0
	playerc_map_t* mapVW; // map virtual wall
	playerc_map_t* mapZONE; // map target zone
	playerc_map_t* mapPOWER; // map target zone
	playerc_map_t* mapFP; // map70 foot print
	playerc_planner_t* planner;
	pose_t robot_pose;
	pose_t onmap_pose;  // 機器位置轉換為地圖位置

	// GnomeCanvas
	GnomeCanvasItem* imageitem;
	GnomeCanvasItem* robot_item;
	GnomeCanvasItem* robot_label;
	GnomeCanvasItem* robot_goal;
	GnomeCanvasItem* robot_path;
	GnomeCanvasItem* robot_particle;
	GnomeCanvasItem* robot_uncertainty;
	guchar* map_pixels; // 儲存地圖影像 

	int aa;
	int* moving_path;  // 儲存走過路徑，存在一個仿地圖陣列上

	int testZone;
} gui_data_t;


typedef struct node { 
	pose_t data; 
	struct node *next; 
}Node; 
Node *front, *rear; 

void queue_create(); 
void queue_showfront(); 
void queue_add(pose_t); 
void queue_delete(); 
void queue_showqueue(); 

void update_whole_map(gui_data_t* gui_data);
void update_partial_map(gui_data_t* gui_data);
void update_robot_name(gui_data_t* gui_data, pose_t pose);
void update_status_bar(gui_data_t* gui_data);

void create_map_image(gui_data_t* gui_data, int ori_X, int ori_Y, int draw_width, int draw_height);
void create_robot(gui_data_t* gui_data, pose_t pose);
void draw_waypoints(gui_data_t* gui_data);
void draw_goal(gui_data_t* gui_data);
void draw_extra_image(gui_data_t* gui_data, int i, int j, int r, int g, int b, int boldSize);
void make_map_menu(gui_data_t* gui_data, int idx_start, int idx_end, GtkMenu* menu, GtkCheckMenuItem** item, int mode);
void make_menu(gui_data_t* gui_data);
void move_item(GnomeCanvasItem* item, pose_t pose, int raise, int showMes);
void platPos_to_mapPos(gui_data_t* gui_data, pose_t robot_pose);
void reset(gui_data_t* gui_data);
int subscribe_and_read_map(gui_data_t* gui_data, playerc_map_t* map, int mapIdx, GtkWidget* item, int mode);
void switch_map_index(int mapIdx, int* tmpMapIdx, int idxStart, int idxEnd, GtkCheckMenuItem** item);


void init_gui(gui_data_t* gui_data, int argc, char** argv);
void fini_gui(gui_data_t* gui_data);
int  init_player(gui_data_t* gui_data);
void fini_player(gui_data_t* gui_data);

/* Parse command line arguments, of the form host:port */
int parse_args(int argc, char** argv, gui_data_t* gui_data);

double mapOrigin[2];
double mapResolution;
GtkWidget* status_bar;  // 狀態列 - 顯示訊息

// gtk的menu_check_item，因為要在不同函式控制打勾與否，因此設為全域可用
GtkCheckMenuItem* footprints_item_map; // map
#endif
