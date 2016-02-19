/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2004
 *     Brian Gerkey
 *
 *  playernav2
 *     Carlos Lai
 *    2014/06/18 Playernav2 - 簡化版的 Playernav，用以輔助AI程式開發
 *    2014/08/13 優化程式、可顯示軌跡
 *    2014/11/14 可另存畫面截圖、顯示虛擬牆
 *    2014/12/12 縮小地圖、可顯示 CoverageMap、RobotLocation
 *    2015/02/06 新增 StatusBar顯示訊息、以滑鼠PP
 *    2015/02/25 設定Target時 StatusBar顯示目標座標、可切換選擇Virtual Wall map
 *    2015/03/05 可顯示Virtual、Zone、Power地圖，並可切換地圖索引值
 *    2015/08/13 Use position2d to replace planner, and can select using planner or not (that will increase CPU's usage)
 *    2015/08/17 Show mouse's coordinate on status bar
 *    2015/08/18 Map zoomin, zoomout (use mouse middle buttom or Page Up/Down)
 *    2015/11/17 增加顯示Dock位置
 */

// TODO:
//   * Make initial window size / zoom fit the whole map.
//   * Make the zoom work better / look nicer

#include <assert.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#ifndef WIN32
  #include <sys/time.h>
#endif

#include "playernav.h"

// flag and index for robot currently being moved by user (if any)
int robot_moving_p;
int robot_moving_idx;

int flag_showObstacleMap;
int flag_showFootprints;
int flag_showFootprintsMap;
int flag_showMouseCoordinate;
int flag_showRobotName;
int flag_showWaypoints;
int flag_dockSimultion; // 如果不是遠端連線就在原點建一個模擬的充電站

double mapupdatefreq = 0.0;
int refreshMapSize = 450;

void update_whole_map(gui_data_t* gui_data);
void update_partial_map(gui_data_t* gui_data);

static void _interrupt_callback(int signum)
{
  printf("\ninterrupt !!\n");
  gtk_main_quit();
}

// Function to read from player; will be called when GTK is idle
gboolean player_read_func(gpointer* arg)
{
  gui_data_t* gui_data = (gui_data_t*)arg;  // 轉型回來
  static struct timeval lastmapupdate = {0, 0}; // 最後更新時間

  struct timeval curr;
  double diff;
  int peek;

  // update map 
  if(mapupdatefreq) // 地圖更新頻率，似乎沒在用
  {
    puts("In MapUpdate frequence");    
    gettimeofday(&curr,NULL);
    diff = (curr.tv_sec + curr.tv_usec/1e6) - 
           (lastmapupdate.tv_sec + lastmapupdate.tv_usec/1e6);

    if(diff >= 1.0/mapupdatefreq) // 是否需要更新
    {
      update_partial_map(gui_data);
      lastmapupdate = curr; // 最後更新時間
    }
  }
/*
  Check whether there is data waiting on the connection, 
  blocking for up to timeout milliseconds (set to 0 to not block).

  Returns:   0 if there is no data waiting
             1 if there is data waiting
            -1 if something went wrong
*/
  peek = playerc_client_peek(gui_data->client, 10);
  if(peek < 0)  // wrong
  {
    fprintf(stderr, "Error on peek\n");
    //gtk_main_quit();
    return(TRUE);
  }
  else if(peek == 0)  // no data waiting
    return(TRUE);

  // read new data
  // Request a round of data; only valid when in a request/reply (aka PULL) mode
  if(playerc_client_requestdata(gui_data->client) < 0)
    fprintf(stderr,"playerc_client_requestdata errored \n");
  
  playerc_client_read_nonblock(gui_data->client);

  if(update_robot_pose(gui_data) || update_waypoints(gui_data)) 

	  update_partial_map(gui_data);

  update_dock_pose(gui_data);
  // raise the robot's canvas item, so that the user can select it
    gnome_canvas_item_raise_to_top(gui_data->robot_item);

  return(TRUE);
}

void update_whole_map(gui_data_t* gui_data)
{
  if(gui_data->imageitem)
    gtk_object_destroy(GTK_OBJECT(gui_data->imageitem));

  if(gui_data->map_pixels == NULL)
    assert(gui_data->map_pixels = (guchar*)malloc(sizeof(unsigned char) * 3 * gui_data->mapSize));

  if(playerc_map_get_map(gui_data->map) < 0)
  {
    fprintf(stderr, "Failed to get map\n");
    return;
  }
  create_map_image(gui_data, 0, 0, gui_data->map->width, gui_data->map->height); 
  update_robot_info(gui_data, gui_data->robot_pose);
  if(!setting_goal)
  update_status_bar(gui_data);
}

void update_partial_map(gui_data_t* gui_data)
{
  if(gui_data->imageitem)
    gtk_object_destroy(GTK_OBJECT(gui_data->imageitem));

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
    fprintf(stderr, "Failed to get partial map\n");
    return;
  }
  if(flag_showObstacleMap)
  {
    if(playerc_map_get_partial_map(gui_data->mapOB, 
          oriIntX, oriIntY, remainX, remainY, true) < 0)
    {
      fprintf(stderr, "Press \"show obstacle map\" but Failed to get obstacle map\n");
      flag_showObstacleMap = !flag_showObstacleMap;
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(obstacle_item_map), FALSE);
    }
  }
  if(flag_showFootprintsMap)
  {
    if(playerc_map_get_partial_map(gui_data->mapFP, 
          oriIntX, oriIntY, remainX, remainY, true) < 0)
    {
      fprintf(stderr, "Press \"show footprints by map\" but Failed to get coverage map\n");
      flag_showFootprintsMap = !flag_showFootprintsMap;
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(footprints_item_map), FALSE);
    }
  }
  create_map_image(gui_data, oriIntX, oriIntY, oriIntX + remainX, oriIntY + remainY);
  update_robot_info(gui_data, gui_data->robot_pose);
  if(!setting_goal)
  update_status_bar(gui_data);
}

int update_robot_pose(gui_data_t* gui_data)
{
  static int preMovingIdx=-100;  
  player_pose2d_t robot_pose;
  player_pose2d_t onmap_pose;
  gboolean onmap;
  
  // 擷取機器人位置 from position2d
  // Try to get the pose from the position2d
  if(gui_data->position2d && gui_data->position2d->info.fresh)
  {
    robot_pose.px = gui_data->position2d->px;
    robot_pose.py = gui_data->position2d->py;
    robot_pose.pa = gui_data->position2d->pa;

    platPos_to_mapPos(&gui_data->onmap_pose, robot_pose); // 更新地圖位置
    onmap_pose = gui_data->onmap_pose;

    // is the robot localized within the map?
    onmap = (onmap_pose.px >= 0) && (onmap_pose.px < gui_data->map->width)
         && (onmap_pose.py >= 0) && (onmap_pose.py < gui_data->map->height);

    // if it's off the map, put it in the middle
    if(!onmap)
      robot_pose.px = robot_pose.py = 0.0;

    // don't draw the new pose if the user is in the middle of moving the robot
    // 移動期間不要畫新位置
    if(!robot_moving_p)
    {
      // also don't draw it if the pose hasn't changed since last time
      // 沒有移動位置也不用更新
      if((gui_data->robot_pose.px != robot_pose.px) 
       ||(gui_data->robot_pose.py != robot_pose.py) 
       ||(gui_data->robot_pose.pa != robot_pose.pa))
      {
        move_item(gui_data->robot_item, robot_pose, 1, 0); // 用的是 position2d 的位置 (robot_pose)
        // If we have a localizer, retrieve and draw particle cloud
        // 有移動位置，記錄機器人位置到moving_path地圖上
        
        // 注意座標關係：視窗原點(0,0)在地圖中心 - Shift (Y軸顛倒)
        int movingIdx;
        movingIdx = ((gui_data->mapHeight - (int)onmap_pose.py - 1) * gui_data->mapWidth) + (int)onmap_pose.px ;

        if(movingIdx != preMovingIdx)
        {
          gui_data->moving_path[movingIdx]++; // 自己記錄移動位置
          preMovingIdx = movingIdx;
        }
      }
    }
    // regardless, store this pose for comparison on next iteration
    gui_data->robot_pose = robot_pose;    
    // Reset freshness flag(s)
    gui_data->position2d->info.fresh = 0;

    return 1;
  }
  else
    return 0;
}

void update_robot_info(gui_data_t* gui_data, player_pose2d_t pose)
{
  if(gui_data->robot_label != NULL)
    gtk_object_destroy(GTK_OBJECT(gui_data->robot_label));

  GnomeCanvasItem* robot_text;
  char robotname[256];

  snprintf(robotname, sizeof(robotname), "%s:%d\n( %.3f, %.3f)\n",
          gui_data->hostname, gui_data->port, pose.px, pose.py);

  g_assert((robot_text =
            gnome_canvas_item_new(gui_data->robot_item ,
                                  gnome_canvas_text_get_type(),
                                  "text", robotname,
                                  "x", 0.0,
                                  "y", 0.0,
                                  "x-offset",  2.0*ROBOT_RADIUS,
                                  "y-offset", -2.0*ROBOT_RADIUS,
                                  "fill-color-rgba", COLOR_BLACK,
                                  NULL)));
  
  gui_data->robot_label = robot_text;

  if(flag_showRobotName)
    gnome_canvas_item_show(gui_data->robot_label);
  else
    gnome_canvas_item_hide(gui_data->robot_label);
}

void update_status_bar(gui_data_t* gui_data)
{
  // 因為字型的關係，限制%4d之類的對齊沒甚麼用 =,=
  char buff[100];
  if(gui_data->device == "Target")
  {
    g_snprintf(buff, 100, "%s at(%.2f,%.2f)\t[%4d,%4d]"
      , gui_data->device
      , gui_data->robot_pose.px, gui_data->robot_pose.py
      , (int)gui_data->onmap_pose.px, (int)gui_data->onmap_pose.py);    
  }
  else // Robot position
  {
    g_snprintf(buff, 100, "%s at(%.2f,%.2f,%.2f)\t[%4d,%4d]"
      , gui_data->device
    , gui_data->robot_pose.px, gui_data->robot_pose.py, gui_data->robot_pose.pa
    , (int)gui_data->onmap_pose.px, (int)gui_data->onmap_pose.py);
  }

  if(flag_showWaypoints && gui_data->planner && gui_data->planner->path_valid)
  {
    char tmp[100];
    g_snprintf(tmp, 100, "\t-> Target (%.2f,%.2f)", gui_data->planner->gx, gui_data->planner->gy);
    strcat(buff, tmp);  
  }

  if(flag_showMouseCoordinate)
  {
    char tmp2[100];
    g_snprintf(tmp2, 100, "   \t Mouse at(%.2f,%.2f)\t[%4d,%4d]"
      , gui_data->mouse_pose.px, gui_data->mouse_pose.py
      , (int)gui_data->mouse_onmap_pose.px, (int)gui_data->mouse_onmap_pose.py);
    strcat(buff, tmp2);
  }
  // 沒什麼作用的contextId，不像說明文件上說的 "為了讓狀態列區別哪個訊息屬於哪個元件或操作，GtkStatusBar使用Context ID來加以識別" 
  // 值都不會更動
  gint contextId = gtk_statusbar_get_context_id( GTK_STATUSBAR (status_bar), "Path planning Information");

  gtk_statusbar_pop (GTK_STATUSBAR (status_bar), contextId);
  gtk_statusbar_push(GTK_STATUSBAR (status_bar), contextId, buff);
}

int update_waypoints(gui_data_t* gui_data)
{
  static player_pose2d_t lastwaypt;
  static player_pose2d_t lastgoal;

  // 擷取機器人位置 from planner
  // Try to get the pose from the planner
  if( flag_showWaypoints && gui_data->planner && gui_data->planner->info.fresh)
  {
    // If we have a planner
    if(gui_data->planner)
    {
      // If the current goal or waypoint changed, get the whole list 
      // of waypoints again
      if( gui_data->planner->waypoint_count > 0)
      {
        if(playerc_planner_get_waypoints(gui_data->planner) < 0)
        {
          fprintf(stderr, "error while getting waypoints for robot\n");
        }
        else
        { 
          draw_waypoints(gui_data);
        }
      }
      else
      {
        draw_goal(gui_data);
      }        
    }
    // Reset freshness flag(s)
    gui_data->planner->info.fresh = 0;
    return 1;
  }
  return 0;
}

void update_dock_pose(gui_data_t* gui_data)
{
  if(!flag_dockSimultion && gui_data->fiducial->fiducials_count > 0)
  {
    int i;
    int max_dock_num = (gui_data->fiducial->fiducials_count > MAX_DOCK_NUM) ? MAX_DOCK_NUM : gui_data->fiducial->fiducials_count;
    player_pose2d_t tmp_dock_pose;

    for(i=0; i<max_dock_num; i++)
    {
      transfer_dock_coordinate(&tmp_dock_pose, gui_data, gui_data->fiducial->fiducials[i].pose);

      if(  tmp_dock_pose.px > gui_data->poseMin.px && tmp_dock_pose.px <= gui_data->poseMax.px
        && tmp_dock_pose.py > gui_data->poseMin.py && tmp_dock_pose.py <= gui_data->poseMax.py)
      {
        gui_data->dock_pose[i] = tmp_dock_pose;
        printf("Dock Pose[%d]: (%.2f,%.2f,%.2f)\n", i, gui_data->dock_pose[i].px, gui_data->dock_pose[i].py, gui_data->dock_pose[i].pa);
    
        if(gui_data->dock_item[i]==NULL)
          create_dock(gui_data, i);
        else
          move_item(gui_data->dock_item[i], gui_data->dock_pose[i], 1, 0); 
      }  
    }
  }
  else if(gui_data->dock_item[0]==NULL) // 創一個充電站模擬
  {
    create_dock(gui_data, 0);
    move_item(gui_data->dock_item[0], gui_data->origin_pose, 1, 0);
  }
}

void transfer_dock_coordinate(player_pose2d_t* dock_pose, gui_data_t* gui_data, player_pose3d_t fiducial_pose)
{
  dock_pose->px = 
     (cos(gui_data->robot_pose.pa)*fiducial_pose.px - sin(gui_data->robot_pose.pa)*fiducial_pose.py) 
    + gui_data->robot_pose.px + 0.1*cos(3.14 + gui_data->robot_pose.pa); 
  dock_pose->py = 
     (sin(gui_data->robot_pose.pa)*fiducial_pose.px + cos(gui_data->robot_pose.pa)*fiducial_pose.py) 
    + gui_data->robot_pose.py + 0.1*sin(3.14 + gui_data->robot_pose.pa); 
  dock_pose->pa = fiducial_pose.pyaw * 57.296;

//  dock_pose->px = gui_data->robot_pose.px + fiducial_pose.px;
//  dock_pose->py = gui_data->robot_pose.py + fiducial_pose.py;
//  dock_pose->pa = 1.570; //fiducial_pose.proll;
}

void reset(gui_data_t* gui_data)
{
  int i;

  gui_data->hostname   = "localhost";
  gui_data->port       = 6665;
  gui_data->zoom_value = 16.0; // 設定縮放大小，對執行時間影響很大。
  gui_data->aa = 1;

  gui_data->origin_pose.px = 0; // 原點( 0, 0, 0)
  gui_data->origin_pose.py = 0;
  gui_data->origin_pose.pa = 0;

  gui_data->map_pixels = NULL;
  gui_data->mapVW      = NULL;
  gui_data->mapZONE    = NULL;
  gui_data->mapPOWER   = NULL;
  gui_data->planner    = NULL;

  gui_data->mapVW_idx    = 0;
  gui_data->mapZONE_idx  = 0;
  gui_data->mapPOWER_idx = 0;
  gui_data->device = "Robot"; // 裝置名稱

  gui_data->robot_pose.px = 0;
  gui_data->robot_pose.py = 0;
  gui_data->robot_pose.pa = 0;

  gui_data->mouse_pose.px = 0;
  gui_data->mouse_pose.py = 0;
  gui_data->mouse_pose.pa = 0;
  gui_data->mouse_onmap_pose.px = 0;
  gui_data->mouse_onmap_pose.py = 0;
  gui_data->mouse_onmap_pose.pa = 0;

  // map zoom value
  zoom_MAX = 55;
  zoom_MIN = gui_data->zoom_value;

  // Veiw default
  flag_showObstacleMap     = 0;
  flag_showFootprints      = 0;
  flag_showFootprintsMap   = 0;
  flag_showMouseCoordinate = 0;
  flag_showRobotName       = 0;
  flag_showWaypoints       = 0;
  flag_dockSimultion       = 1;

  dragging      = FALSE;
  setting_theta = FALSE;
  setting_goal  = FALSE;

  for(i=0; i<MAX_DOCK_NUM; i++)
    gui_data->dock_item[i] = NULL;
}

int main(int argc, char** argv)
{
  gui_data_t gui_data;  // 所有 GUI 的資訊

  memset(&gui_data, 0, sizeof(gui_data));
  reset(&gui_data);

  if(parse_args(argc, argv, &gui_data) < 0)
  {  
    printf("Hostname or else input error\n");
    print_usage(argc, argv);
    exit(-1); // Input problem, print usage and exit with error
  }

  if (signal(SIGINT, _interrupt_callback) ==SIG_ERR)
    exit(-1); // Couldn't register signal callback, exit with error

  // connect to robot
  if( init_player(&gui_data) < 0)
    exit(-1); // Connection to Player failed, exit with error

  // GTK初始化 (框架、表單...etc)
  init_gui(&gui_data, argc, argv);  // initialize Gtk GUI
  
  if(flag_showWaypoints) // open planner by argv
  {
    flag_showWaypoints = 0;
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(waypoints_item), TRUE); 
    // will execute funciton _show_waypoints()
  }

  create_robot(&gui_data, gui_data.robot_pose);  // 創代表機器人的紅點和M2T目標點
  update_whole_map(&gui_data); // 第一次更新地圖

  gtk_widget_show((GtkWidget*)(gui_data.main_window));  // 顯示視窗

  // setup read function to be called when idle
  g_idle_add((GSourceFunc)player_read_func, (gpointer*)&gui_data); // 將 gui_data 轉型傳入
  gtk_main();

  fini_player(&gui_data);
  fini_gui(&gui_data);

  return(0);
}

/*
  SIGINT是信號名稱或代碼（Ctrl-C會產生這個信號）
  sig_int是你的一段程序
  singal(SIGINT, sig_int)就是當鍵入Ctrl-C的時候，當前執行程序調用
  function sig_int(), sig_int執行完後，再返回原來執行的地方接著往下走




  我們知道glib提供了一個名為g_idle_add的函數，這個函數的功能很容易理解：增加一
  個空閒任務，讓應用程序在空閒時執行指定的函數。這種機制非常有用，如果沒有這種機制，
  很多事情將非常麻煩。它的功能雖然簡單，但並不是所有人都知道如何充分發揮它的潛力，
  這裡說說它的幾個主要用途吧。

  1.在空閒時執行低優先級任務。有的任務優先級比較低，但費耗時間比較長，像屏幕刷新等
    操作，我們不希望它阻礙當前操作太久，此時可以把它放到空閒任務裡去做。實際上GTK+
    裡面也是這樣做的，這樣可以獲得更好的響應性。

  2.將同步操作異步化。我們知道在GTK+中，它使用glib的signal作為窗口/控件之間的通
    信方式，signal的執行是直接調用函數，即整個signal的執行過程是同步完成的。這在
    多數情況下工作得很好，但有時會出現重入的問題，你調我，我再調你，可能會遇到麻煩。
    此時我們不得不採用異步方式，而GTK+沒有提供像Win32下的PostMessage之類的異步
    消息，幸好我們可以用g_idle_add函數來模擬。

  3.串行化對GUI的訪問。在大多數平台下，對GUI資源的訪問都是需要串行化的，即在一個
    GUI應用程序中，只有一個線程可以直接操作GUI資源。這是因為出於效率的考慮， 
    GUI資源是沒有加鎖保護的，GTK+也是這樣的。如果另外一個線程要訪問GUI資源，比如
    要顯示一條信息，怎麼辦呢？這可以通過g_idle_add增加一個空閒任務來實現，idle
    任務是GUI線程(主線程)中執行的，所以串行了對GUI資源的訪問。這裡要注意，idle任
    務並不是一個獨立的線程或者進程，而在是主線程中執行的。所謂空閒是指，當 main loop
    沒有其它消息要處理，而且沒有更高優先級的工作要做時，就認為處於空閒狀態。
*/