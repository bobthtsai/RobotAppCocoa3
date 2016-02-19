/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2004
 *     Brian Gerkey
 */

#include <math.h>
#include <gdk/gdkkeysyms.h>
#include <libplayercommon/playercommon.h>
#include <libplayerutil/localization.h>
#include "playernav.h"

#if defined WIN32
  #define snprintf _snprintf
#else
  #include <sys/time.h>
#endif

// flag and index for robot currently being moved by user (if any)
extern int robot_moving_p;
extern int robot_moving_idx;
extern int refreshMapSize;

static guint32 robot_colors[] = { GNOME_CANVAS_COLOR_A(255,  0,  0,ROBOT_ALPHA),
                                  GNOME_CANVAS_COLOR_A(  0,255,  0,ROBOT_ALPHA),
                                  GNOME_CANVAS_COLOR_A(  0,  0,255,ROBOT_ALPHA),
                                  GNOME_CANVAS_COLOR_A(255,  0,255,ROBOT_ALPHA),
                                  GNOME_CANVAS_COLOR_A(255,255,  0,ROBOT_ALPHA),
                                  GNOME_CANVAS_COLOR_A(  0,255,255,ROBOT_ALPHA) };
static size_t num_robot_colors = sizeof(robot_colors) / sizeof(robot_colors[0]);

int tmpVW_idx; // 暫存 virtual map index，用以連繫兩個不同的button function
int tmpZONE_idx;
int tmpPOWER_idx;

int showVirtualWall_Flag;
int showZoneMap_Flag;
int showPowerMap_Flag;
extern int flag_showObstacleMap;
extern int flag_showFootprints;
extern int flag_showFootprintsMap;
extern int flag_showMouseCoordinate;
extern int flag_showRobotName;
extern int flag_showWaypoints;

GtkWidget* virtualwall_item;
GtkWidget* zone_item;
GtkWidget* power_item;
GtkCheckMenuItem* virtualwall_mapIdx_item[(VW_END-VW_START+1)];
GtkCheckMenuItem* zone_mapIdx_item[(ZONE_END-ZONE_START+1)];
GtkCheckMenuItem* power_mapIdx_item[(POWER_END-POWER_START+1)];

/*
 * handle quit events, by setting a flag that will make the main loop exit
 */
static gboolean
_quit_callback(GtkWidget *widget,
               GdkEvent *event,
               gpointer data)
{
  gtk_main_quit();
  return(TRUE);
}

static gboolean
_save_screenshot(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  gui_data_t* gui_data = (gui_data_t*) widget; // =,= not gpointer data
  static int idx = 0;
  static GdkPixbuf* screenshot = NULL;
  GdkWindow* win;
  char fname[PATHNAME_MAXLEN];
  gint width, height;
  g_assert((win = ((GtkWidget*)gui_data->map_canvas)->window));

  if(gdk_window_is_viewable(win))
  {
    gdk_window_get_size(win, &width, &height);
    g_assert((screenshot =
              gdk_pixbuf_get_from_drawable(screenshot,
                                           (GdkDrawable*)win,
                                           gdk_colormap_get_system(),
                                           0,0,0,0,
                                           width,height)));
    snprintf(fname,sizeof(fname),"playernav2-img-%04d.png",idx);
    printf("writing screenshot to %s\n", fname);

    if(!(gdk_pixbuf_save(screenshot, fname, "png", NULL, NULL)))
      puts("FAILED");

    idx++;
  }
  return(TRUE);
}

static gboolean 
_show_names(GtkWidget *widget,
            GdkEvent *event,
            gpointer data)
{
  flag_showRobotName = !flag_showRobotName;
  return(TRUE);
}

static gboolean
_show_obstacle_map(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  gui_data_t* gui_data = (gui_data_t*)widget;
  flag_showObstacleMap = !flag_showObstacleMap;

  if(flag_showObstacleMap)
  {
    if(playerc_map_get_map(gui_data->mapOB) < 0)
    {
      fprintf(stderr, "Press \"show obstacle map\" but Failed to get obstacle map\n");
      flag_showObstacleMap = !flag_showObstacleMap;
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(obstacle_item_map), FALSE);
    }
  }
  update_whole_map((gui_data_t*)gui_data);
  return(TRUE);
}

static gboolean
_show_footprints(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  flag_showFootprints = !flag_showFootprints;
  update_whole_map((gui_data_t*)widget);
  return(TRUE);
}

static gboolean
_show_footprints_map(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  gui_data_t* gui_data = (gui_data_t*)widget;
  flag_showFootprintsMap = !flag_showFootprintsMap;

  if(flag_showFootprintsMap)
  {
    if(playerc_map_get_map(gui_data->mapFP) < 0)
    {
      fprintf(stderr, "Press \"show footprints by map\" but Failed to get coverage map\n");
      flag_showFootprintsMap = !flag_showFootprintsMap;
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(footprints_item_map), FALSE);
    }
  }
  update_whole_map((gui_data_t*)gui_data);
  return(TRUE);
}

static gboolean
_show_virtualwall(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  gui_data_t* gui_data = (gui_data_t*)widget;

  if(gui_data->mapVW_idx != tmpVW_idx)  // 更改了 mapIndex
  {
    gui_data->mapVW_idx = tmpVW_idx;
    showVirtualWall_Flag = subscribe_map_and_load(gui_data, gui_data->mapVW, gui_data->mapVW_idx, virtualwall_item, 0);
  }
  else
    showVirtualWall_Flag = !showVirtualWall_Flag;

  if(!showVirtualWall_Flag)
    gtk_menu_item_set_label(virtualwall_item, "Show virtual wall");
  
  update_whole_map((gui_data_t*)gui_data);
  return(TRUE);
}

static gboolean
_show_waypoints(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  gui_data_t* gui_data = (gui_data_t*)widget;
  flag_showWaypoints = !flag_showWaypoints;

  if(flag_showWaypoints)
  {
    if( subscribe_planner(gui_data) < 0 )
      flag_showWaypoints = 0;
  }
  else
  {
    unsubscribe_planner(gui_data);
    destroy_waypoints(gui_data);
    gnome_canvas_item_hide(gui_data->robot_goal);
  }

  return(TRUE);
}

static gboolean
_show_zone(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  gui_data_t* gui_data = (gui_data_t*)widget;

  if(gui_data->mapZONE_idx != tmpZONE_idx )  // 更改了 mapIndex
  {
    gui_data->mapZONE_idx = tmpZONE_idx;
    showZoneMap_Flag = subscribe_map_and_load(gui_data, gui_data->mapZONE, gui_data->mapZONE_idx, zone_item, 1);
  }
  else
    showZoneMap_Flag = !showZoneMap_Flag;

  if(!showZoneMap_Flag)
    gtk_menu_item_set_label(zone_item, "Show zone map");

  update_whole_map((gui_data_t*)gui_data);
  return(TRUE);
}

static gboolean
_show_power(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  gui_data_t* gui_data = (gui_data_t*)widget;
  
  if(gui_data->mapPOWER_idx != tmpPOWER_idx)  // 更改了 mapIndex
  {
    gui_data->mapPOWER_idx = tmpPOWER_idx;
    showPowerMap_Flag = subscribe_map_and_load(gui_data, gui_data->mapPOWER, gui_data->mapPOWER_idx, power_item, 2);
  }
  else
    showPowerMap_Flag= !showPowerMap_Flag;

  if(!showPowerMap_Flag)
    gtk_menu_item_set_label(power_item, "Show power map");

  update_whole_map((gui_data_t*)gui_data);
  return(TRUE);
}

static gboolean
_switch_virtualwall(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  switch_map_index((int)widget, &tmpVW_idx, VW_START, VW_END, virtualwall_mapIdx_item);
  return(TRUE);
}

static gboolean
_switch_zone(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  switch_map_index((int)widget, &tmpZONE_idx, ZONE_START, ZONE_END, zone_mapIdx_item);
  return(TRUE);
}

static gboolean
_switch_power(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  switch_map_index((int)widget, &tmpPOWER_idx, POWER_START, POWER_END, power_mapIdx_item);
  return(TRUE);
}

// do one more again update_map()
static gboolean
_refresh_map(GtkWidget *widget,
             GdkEvent *event,
             gpointer data)
{
  gui_data_t* gui_data = (gui_data_t*)widget;
  if(showVirtualWall_Flag)
  {
    if(playerc_map_get_map(gui_data->mapVW) < 0)
    {
      printf("Get virtual map error\n");
      showVirtualWall_Flag = 0;
    }
  }
  if(showZoneMap_Flag)
  {
    if(playerc_map_get_map(gui_data->mapZONE) < 0)
    {
      printf("Get zone map error\n");
      showZoneMap_Flag = 0;
    }
  }
  if(showPowerMap_Flag)
  {
    if(playerc_map_get_map(gui_data->mapPOWER) < 0)
    {
      printf("Get power map error\n");
      showPowerMap_Flag = 0;
    }
  }
  if(flag_showObstacleMap)
  {
    if(playerc_map_get_map(gui_data->mapOB) < 0)
    {
      printf("Get obstacle map error\n");
      flag_showObstacleMap = 0;
    }
  }
  if(flag_showFootprintsMap)
  {
    if(playerc_map_get_map(gui_data->mapFP) < 0)
    {
      printf("Get footprints map error\n");
      flag_showFootprintsMap = 0;
    }
  }
  update_whole_map(gui_data);
  return(TRUE);
}

static void
_zoom_callback(GtkAdjustment* adjustment,
               gpointer data)
{
  double newzoom;
  gui_data_t* gui_data = (gui_data_t*)data;

  newzoom = gtk_adjustment_get_value(adjustment);
  gnome_canvas_set_pixels_per_unit(gui_data->map_canvas, newzoom); // 放大縮小
}

static gboolean
_mouse_callback(GnomeCanvasItem *item,
                       GdkEvent *event,
                       gpointer data)
{
  static GnomeCanvasPoints* points = NULL;
  gboolean onrobot = FALSE;
  gboolean ongoal  = FALSE;
  player_pose2d_t button_pose;
  double mean[3];

  gui_data_t* gui_data = (gui_data_t*)data;

  button_pose.pa = 0.0;
  button_pose.px =  event->button.x;  // 按鍵點下位置
  button_pose.py = -event->button.y;

  flag_showMouseCoordinate = 0;
  // lookup (and store) which robot (or goal) was clicked
  if((item != (GnomeCanvasItem*)gnome_canvas_root(gui_data->map_canvas)) &&
     !setting_theta && !setting_goal)
  {
    if(item == gui_data->robot_item)
    {
      onrobot = TRUE;
    }
    else if(item == gui_data->robot_goal)
    {
      ongoal = TRUE;
    }
  }
  else if(item == (GnomeCanvasItem*)gnome_canvas_root(gui_data->map_canvas))  // get mouse point's coordinate
  {
    flag_showMouseCoordinate = 1;

    if( event->type == GDK_MOTION_NOTIFY &&  (event->button.x != 0 && -event->button.y != 0) ) 
    {
      gui_data->mouse_pose.px =  event->button.x;
      gui_data->mouse_pose.py = -event->button.y;
    }
    platPos_to_mapPos(&gui_data->mouse_onmap_pose, gui_data->mouse_pose);
  }

  switch(event->type)
  {
    case GDK_BUTTON_PRESS:  // 按下按鈕      
      switch(event->button.button)
      {
        case 3: // Right button
          if((onrobot || ongoal) && !setting_theta)
          {
            setting_goal=TRUE;
            move_item(gui_data->robot_goal, button_pose, 0, 1);
            gnome_canvas_item_show(gui_data->robot_goal);
          }
        case 1: // Left button
          if(!setting_theta)
          {
            if(onrobot || (ongoal && event->button.button == 3))
            {
              // Get item and let it can move by mouse.
              gnome_canvas_item_grab(item,
                                     GDK_POINTER_MOTION_MASK |
                                     GDK_BUTTON_RELEASE_MASK,
                                     NULL, event->button.time);
              dragging = TRUE;  // 拖曳
              // set these so that the robot's pose won't be updated in
              // the GUI while we're dragging  the robot (that happens
              // in playernav.c)
              if(event->button.button == 1)
              {
                robot_moving_p = 1;
              }
            }
          }
          else
          {
            mean[0] =  points->coords[0];
            mean[1] = -points->coords[1];
            mean[2] = 0;//theta;

            if(setting_goal)
            {
              if(gui_data->planner)
              {
                printf("setting goal (by planner) for robot to (%.3f, %.3f, %.3f)\n", mean[0], mean[1], mean[2]);
                
                if(playerc_planner_set_cmd_pose(gui_data->planner, mean[0], mean[1], mean[2], 0) < 0 )
                {
                  fprintf(stderr, "error while setting goal on robot\n");
                }
                else
                {
                  gui_data->goal[0] = mean[0];
                  gui_data->goal[1] = mean[1];
                  gui_data->goal[2] = mean[2];
                  gui_data->planner->waypoint_count = -1;
                }
              }
              else
              {
                puts("WARNING: NOT setting goal; couldn't connect to planner\n");
              }
            }
            setting_theta = FALSE;
            setting_goal = FALSE;

            robot_moving_p = 0;
          }
          break;
        default:
          break;
      }
      break;
    case GDK_MOTION_NOTIFY: // 位置變更
      if(dragging)
      {
        if(setting_goal)
          move_item(gui_data->robot_goal, button_pose, 0, 1);
        else
          move_item(item, button_pose, 1, 0);
      }
      break;
    case GDK_BUTTON_RELEASE:
      if(dragging)
      {
        dragging = FALSE;
        setting_theta = TRUE;

        // 放開按鍵時的位置
        if(!points)
          g_assert((points = gnome_canvas_points_new(2)));
        points->coords[0] =  button_pose.px;
        points->coords[1] = -button_pose.py;
        points->coords[2] =  button_pose.px;
        points->coords[3] = -button_pose.py;

        gnome_canvas_item_ungrab(item, event->button.time);
      }
      break;
    case GDK_SCROLL:
        if(((GdkEventScroll*)event)->direction == GDK_SCROLL_UP)
          adjustment_zoom_value((gui_data_t*)data, 1);
        else if(((GdkEventScroll*)event)->direction == GDK_SCROLL_DOWN)
          adjustment_zoom_value((gui_data_t*)data, -1);
      break;
    default:
      break;
  }
  return(TRUE);
}

static gboolean
_keyboard_callback(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
  switch (event->keyval)
        {
    case GDK_Page_Up:
      adjustment_zoom_value((gui_data_t*)data, 1);
      break;
    case GDK_Page_Down:
      adjustment_zoom_value((gui_data_t*)data, -1);
      break;
    default:
      return (FALSE); 
  }
  return (FALSE); 
}
void adjustment_zoom_value(gui_data_t* gui_data, int mode)
{
  if(mode > 0)
    gui_data->zoom_value += ZOOM_SCALE;
  else if(mode < 0)
    gui_data->zoom_value -= ZOOM_SCALE;

  if(gui_data->zoom_value <= zoom_MAX && gui_data->zoom_value >= zoom_MIN)
  {
    gtk_adjustment_set_value(gui_data->zoom_adjustment, gui_data->zoom_value);
    gtk_adjustment_value_changed(gui_data->zoom_adjustment);
        }
        else
        {
    gui_data->zoom_value = (gui_data->zoom_value >= zoom_MAX) ? zoom_MAX : gui_data->zoom_value;
    gui_data->zoom_value = (gui_data->zoom_value <= zoom_MIN) ? zoom_MIN : gui_data->zoom_value;
        }
}

void switch_map_index(int mapIdx, int* tmpMapIdx, int idxStart, int idxEnd, GtkCheckMenuItem** item)
{
  if(*tmpMapIdx != mapIdx)
  {
    if(*tmpMapIdx >= idxStart && *tmpMapIdx <= idxEnd) // 取消之前選項的勾
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item[*tmpMapIdx-idxStart]), FALSE);
    *tmpMapIdx = mapIdx;
      }
}

void create_map_menu(gui_data_t* gui_data, int idx_start, int idx_end, GtkMenu* menu, GtkCheckMenuItem** item, int mode)
{
  int i, idx;

  for(i = idx_start; i <= idx_end; i++)
  {
    char buf[12];
    idx = i - idx_start;
    sprintf(buf, "map_%d", i);
    item[idx] = gtk_check_menu_item_new_with_label(buf);
    gtk_menu_append(GTK_MENU (menu), item[idx]);
    gtk_widget_show((GtkWidget*)item[idx]);

    switch(mode)
    {
      case 0:
        g_signal_connect_swapped(G_OBJECT (item[idx]), "activate",
                   G_CALLBACK(_switch_virtualwall), (gpointer) i); // mapIdx
        g_signal_connect_swapped(G_OBJECT (item[idx]), "activate",
                   G_CALLBACK(_show_virtualwall), (gpointer) gui_data);  // show mapVW
      break;
      case 1:
        g_signal_connect_swapped(G_OBJECT (item[idx]), "activate",
                   G_CALLBACK(_switch_zone), (gpointer) i); // mapIdx
        g_signal_connect_swapped(G_OBJECT (item[idx]), "activate",
                   G_CALLBACK(_show_zone), (gpointer) gui_data);  // show mapVW
        break;
      case 2:
        g_signal_connect_swapped(G_OBJECT (item[idx]), "activate",
                   G_CALLBACK(_switch_power), (gpointer) i); // mapIdx
        g_signal_connect_swapped(G_OBJECT (item[idx]), "activate",
                   G_CALLBACK(_show_power), (gpointer) gui_data);  // show mapVW
      break;
  }

  }
}

// 設定menu
void create_menu(gui_data_t* gui_data)
{
  GtkAccelGroup *accel_group;
  GtkMenuBar* menu_bar;
  GtkMenu* file_menu;
  GtkMenuItem* file_item; // show label "File"
  GtkMenuItem* screenshot_item;
  GtkMenuItem* quit_item;

  GtkMenu* view_menu;
  GtkMenu* vwIdx_menu;  // check virtual wall map index
  GtkMenu* zoneIdx_menu;  
  GtkMenu* powerIdx_menu; 

  GtkMenuItem* view_item; // show label "View"
  GtkCheckMenuItem* show_names_item;
  GtkCheckMenuItem* footprints_item; // playernav2's trajectories record  
  GtkCheckMenuItem* refresh_map_item;

  tmpVW_idx    = gui_data->mapVW_idx;
  tmpZONE_idx  = gui_data->mapZONE_idx;
  tmpPOWER_idx = gui_data->mapPOWER_idx;
  /* Create a GtkAccelGroup and add it to the main window. */
  accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group(gui_data->main_window, accel_group);

  file_menu = (GtkMenu*)gtk_menu_new();    /* Don't need to show menus */
  view_menu = (GtkMenu*)gtk_menu_new();    /* Don't need to show menus */
  vwIdx_menu    = (GtkMenu*)gtk_menu_new();    /* Don't need to show menus */
  zoneIdx_menu  = (GtkMenu*)gtk_menu_new();    /* Don't need to show menus */
  powerIdx_menu = (GtkMenu*)gtk_menu_new();    /* Don't need to show menus */

  /* Create the menu items */
  screenshot_item = (GtkMenuItem*)gtk_menu_item_new_with_label("Screen shot");
  quit_item = (GtkMenuItem*)gtk_menu_item_new_with_label("Quit");
  show_names_item     = (GtkCheckMenuItem*)gtk_check_menu_item_new_with_label("Show robot name"); // CheckMenuItem
  obstacle_item_map   = (GtkCheckMenuItem*)gtk_check_menu_item_new_with_label("Show obstacle map from Map39");
  footprints_item     = (GtkCheckMenuItem*)gtk_check_menu_item_new_with_label("Show robot footprints from Nav2");
  footprints_item_map = (GtkCheckMenuItem*)gtk_check_menu_item_new_with_label("Show robot footprints from Map70");
  waypoints_item      = (GtkCheckMenuItem*)gtk_check_menu_item_new_with_label("Show path planning's waypoints");
  virtualwall_item    = (GtkMenuItem*)gtk_menu_item_new_with_label("Show virtual wall");
  zone_item           = (GtkMenuItem*)gtk_menu_item_new_with_label("Show zone map");
  power_item          = (GtkMenuItem*)gtk_menu_item_new_with_label("Show power map");  
  refresh_map_item    = (GtkCheckMenuItem*)gtk_menu_item_new_with_label("Refresh map");
// 想要做一個子選單的話，那儲存子選單目錄的 gtk_menu就不能命名，不然會無法顯示，所以 vwIdx_menu 不能命名

  create_map_menu(gui_data, VW_START, VW_END, vwIdx_menu, virtualwall_mapIdx_item, 0);
  create_map_menu(gui_data, ZONE_START, ZONE_END, zoneIdx_menu, zone_mapIdx_item, 1);
  create_map_menu(gui_data, POWER_START, POWER_END, powerIdx_menu, power_mapIdx_item, 2);
  
  /* Setup hot-keys accelerators:
     ex:  Ctrl-s screen shot     */
  gtk_widget_add_accelerator((GtkWidget*)screenshot_item, "activate",
                             accel_group, GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator((GtkWidget*)quit_item, "activate",
                             accel_group, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator((GtkWidget*)show_names_item, "activate",
                             accel_group, GDK_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator((GtkWidget*)obstacle_item_map, "activate",
                             accel_group, GDK_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator((GtkWidget*)footprints_item_map, "activate",
                             accel_group, GDK_f, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator((GtkWidget*)waypoints_item, "activate",
                             accel_group, GDK_w, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator((GtkWidget*)refresh_map_item, "activate",
                             accel_group, GDK_KEY_F5, 0, GTK_ACCEL_VISIBLE);

  /* Add them to the menu */
  gtk_menu_shell_append (GTK_MENU_SHELL(file_menu), (GtkWidget*)screenshot_item);
  gtk_menu_shell_append (GTK_MENU_SHELL(file_menu), gtk_separator_menu_item_new());
  gtk_menu_shell_append (GTK_MENU_SHELL(file_menu), (GtkWidget*)quit_item);
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), (GtkWidget*)show_names_item);
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), (GtkWidget*)obstacle_item_map);
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), (GtkWidget*)footprints_item);
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), (GtkWidget*)footprints_item_map);
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), (GtkWidget*)waypoints_item);
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), gtk_separator_menu_item_new());
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), (GtkWidget*)virtualwall_item);
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), (GtkWidget*)zone_item);
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), (GtkWidget*)power_item);
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), gtk_separator_menu_item_new());
  gtk_menu_shell_append (GTK_MENU_SHELL(view_menu), (GtkWidget*)refresh_map_item);
  
  // add submenu
  gtk_menu_item_set_submenu(GTK_MENU_ITEM (virtualwall_item), vwIdx_menu); 
  gtk_menu_item_set_submenu(GTK_MENU_ITEM (zone_item), zoneIdx_menu); 
  gtk_menu_item_set_submenu(GTK_MENU_ITEM (power_item), powerIdx_menu); 
  /* We can attach the Quit menu item to
   * our exit function */  
  g_signal_connect_swapped(G_OBJECT (quit_item), "activate",
                           G_CALLBACK(_quit_callback),
                           (gpointer) "file.quit");
  g_signal_connect_swapped(G_OBJECT (screenshot_item), "activate",
                           G_CALLBACK(_save_screenshot),
                           (gpointer) gui_data);
  g_signal_connect_swapped(G_OBJECT (show_names_item), "activate",
                           G_CALLBACK(_show_names),
                           (gpointer) gui_data);
  g_signal_connect_swapped(G_OBJECT (obstacle_item_map), "activate",
                           G_CALLBACK(_show_obstacle_map),
                           (gpointer) gui_data);
  g_signal_connect_swapped(G_OBJECT (footprints_item), "activate",
                           G_CALLBACK(_show_footprints),
                           (gpointer) gui_data);
  g_signal_connect_swapped(G_OBJECT (footprints_item_map), "activate",
                           G_CALLBACK(_show_footprints_map),
                           (gpointer) gui_data);
  g_signal_connect_swapped(G_OBJECT (waypoints_item), "activate",
                           G_CALLBACK(_show_waypoints),
                           (gpointer) gui_data);
  g_signal_connect_swapped(G_OBJECT (refresh_map_item), "activate",
                           G_CALLBACK(_refresh_map),
                           (gpointer) gui_data);
  
  /* We do need to show menu items */
  gtk_widget_show((GtkWidget*)screenshot_item);
  gtk_widget_show((GtkWidget*)quit_item);
  gtk_widget_show((GtkWidget*)show_names_item);
  gtk_widget_show((GtkWidget*)obstacle_item_map);
  gtk_widget_show((GtkWidget*)footprints_item);
  gtk_widget_show((GtkWidget*)footprints_item_map);
  gtk_widget_show((GtkWidget*)waypoints_item);
  gtk_widget_show((GtkWidget*)virtualwall_item);
  gtk_widget_show((GtkWidget*)zone_item);
  gtk_widget_show((GtkWidget*)power_item);
  gtk_widget_show((GtkWidget*)refresh_map_item);
  
  menu_bar = (GtkMenuBar*)gtk_menu_bar_new ();
  gtk_box_pack_start(gui_data->vbox, (GtkWidget*)(menu_bar), FALSE, FALSE, 2);
  gtk_widget_show((GtkWidget*)menu_bar);

  file_item = (GtkMenuItem*)gtk_menu_item_new_with_label ("File");
  gtk_widget_show((GtkWidget*)file_item);
  view_item = (GtkMenuItem*)gtk_menu_item_new_with_label ("View");
  gtk_widget_show((GtkWidget*)view_item);
  
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), (GtkWidget*)file_menu);
  gtk_menu_bar_append(GTK_MENU_BAR (menu_bar), (GtkWidget*)file_item);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_item), (GtkWidget*)view_menu);
  gtk_menu_bar_append(GTK_MENU_BAR (menu_bar), (GtkWidget*)view_item);  
}

void init_gui(gui_data_t* gui_data, int argc, char** argv)
{
  double max_zoom, diff_zoom;
  GtkAdjustment* adjust;

  // glib庫中所有的built-in類型都是在這個函數中註冊，也就是說，如果想使用gobject類似的類型需要先調用g_type_init()
  g_type_init();  // initial
  gtk_init(&argc, &argv);

  g_assert((gui_data->main_window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL)));
  gtk_window_set_title(GTK_WINDOW(gui_data->main_window), "PlayerNav2");
  gtk_widget_set_size_request((GtkWidget*)(gui_data->main_window), MIN_DISPLAY_WIDTH, MIN_DISPLAY_WIDTH);

  // display the map "natural size" + a bit for the scrollbars
  // Scroll Bar
  gtk_window_resize(gui_data->main_window, WINDOW_SIZE_W + 60, WINDOW_SIZE_H + 60);

  /* a box to hold everything else */
  g_assert((gui_data->vbox = (GtkBox*)gtk_vbox_new(FALSE, 5)));
  /* a box to hold everything else */
  g_assert((gui_data->hbox = (GtkBox*)gtk_hbox_new(FALSE, 5)));
  g_assert((gui_data->map_window = (GtkScrolledWindow*)gtk_scrolled_window_new(NULL, NULL)));

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gui_data->map_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  // Initialize horizontal scroll bars
  adjust = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(gui_data->map_window));
  adjust->step_increment = 5;
  gtk_adjustment_changed(adjust);
  gtk_adjustment_set_value(adjust, adjust->value - GTK_WIDGET(gui_data->map_window)->allocation.width / 2);
  
  // Initialize vertical scroll bars
  adjust = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(gui_data->map_window));
  adjust->step_increment = 5;
  gtk_adjustment_changed(adjust);
  gtk_adjustment_set_value(adjust, adjust->value + GTK_WIDGET(gui_data->map_window)->allocation.height / 2);

  gtk_widget_push_visual(gdk_rgb_get_visual());
  gtk_widget_push_colormap(gdk_rgb_get_cmap());
  if(gui_data->aa)
    g_assert((gui_data->map_canvas = (GnomeCanvas*)gnome_canvas_new_aa())); // Creates a new empty canvas in non-antialiased mode. 
  else
    g_assert((gui_data->map_canvas = (GnomeCanvas*)gnome_canvas_new()));    // Creates a new empty canvas in antialiased mode. 
  gtk_widget_pop_colormap();
  gtk_widget_pop_visual();

  // When the scrolling region of the canvas is smaller than the canvas window, 
  // e.g. the allocation of the canvas, it can be either centered on the window or 
  // simply made to be on the upper-left corner on the window. 
  // This function lets you configure this property.
  gnome_canvas_set_center_scroll_region(gui_data->map_canvas, TRUE);

  // Resolution = (Pixels / Meter) , so 
  gnome_canvas_set_scroll_region(gui_data->map_canvas, 
                                 -(gui_data->map->width  * gui_data->map->resolution)/2.0,
                                 -(gui_data->map->height * gui_data->map->resolution)/2.0,
                                  (gui_data->map->width  * gui_data->map->resolution)/2.0,
                                  (gui_data->map->height * gui_data->map->resolution)/2.0);
  // the zoom scrollbar
  // set canvas zoom to make the map fill the window
  // zoom in at most 10 times
  max_zoom  = 10.0 * gui_data->zoom_value;
  diff_zoom = max_zoom - gui_data->zoom_value;
  g_assert((gui_data->zoom_adjustment =
            (GtkAdjustment*)gtk_adjustment_new(gui_data->zoom_value,  // 滾動條當前的位置。這個變量必須處於lower和upper值之間
                                               gui_data->zoom_value,  // 滾動條upper值
                                               max_zoom,      // 滾動條lower值
                                               diff_zoom/1e3, // 設置步長 -> 點擊向下箭頭
                                               diff_zoom/1e2, // 設置步長 -> 按下向下翻頁鍵
                                               diff_zoom/1e2))); // page_size是在屏幕上一次可見的區域，它取決於構件的大小。
  g_assert((gui_data->zoom_scrollbar =
            (GtkVScrollbar*)gtk_vscrollbar_new(gui_data->zoom_adjustment)));

  gtk_container_add(GTK_CONTAINER(gui_data->main_window), (GtkWidget*)(gui_data->vbox));
  create_menu(gui_data);  // 創上方選單
  g_assert(status_bar = gtk_statusbar_new()); // 狀態列顯示訊息

  /* gtk_box_pack_start 的參數值 :
      (1)GtkBox *box,     (2)GtkWidget *child,
      (3)gboolean expand, (4)gboolean fill,    (5)guint padding); */
  gtk_box_pack_start(gui_data->vbox,
                     (GtkWidget*)(gui_data->hbox),
                     TRUE, TRUE, 0);
  gtk_box_pack_start(gui_data->hbox,
                     (GtkWidget*)(gui_data->zoom_scrollbar),
                     FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(gui_data->map_window), (GtkWidget*)(gui_data->map_canvas));
  gtk_box_pack_start(gui_data->hbox, 
                     (GtkWidget*)(gui_data->map_window),
                     TRUE, TRUE, 0);
  gtk_box_pack_start(gui_data->vbox, status_bar, FALSE, FALSE, 0);
  
  gtk_widget_show((GtkWidget*)(gui_data->vbox));
  gtk_widget_show((GtkWidget*)(gui_data->hbox));
  gtk_widget_show((GtkWidget*)(gui_data->zoom_scrollbar));
  gtk_widget_show((GtkWidget*)(gui_data->map_window));
  gtk_widget_show((GtkWidget*)(gui_data->map_canvas));
  gtk_widget_show(status_bar);

  g_signal_connect(G_OBJECT(gui_data->main_window),     "delete-event", G_CALLBACK(_quit_callback), NULL);
  g_signal_connect(G_OBJECT(gui_data->main_window),    "destroy-event", G_CALLBACK(_quit_callback), NULL);
  g_signal_connect(G_OBJECT(gui_data->zoom_adjustment),"value-changed", G_CALLBACK(_zoom_callback), (void*)gui_data);
  g_signal_connect(G_OBJECT(gui_data->main_window),  "key_press_event", G_CALLBACK(_keyboard_callback),(void*)gui_data);

  gtk_signal_connect(GTK_OBJECT(gnome_canvas_root(gui_data->map_canvas)), "event",
                  (GtkSignalFunc)(_mouse_callback),(void*)gui_data);

  gtk_adjustment_set_value(gui_data->zoom_adjustment, gui_data->zoom_value);
  gtk_adjustment_value_changed(gui_data->zoom_adjustment);

#if 0


  g_signal_connect(G_OBJECT(gui_data->main_window),"size-allocate",
                   G_CALLBACK(_resize_window_callback),(void*)gui_data);
#endif
}

void fini_gui(gui_data_t* gui_data)
{
  assert(gui_data->main_window);
  gtk_widget_destroy((GtkWidget*)(gui_data->main_window));
}

/*
 * create the background map image and put it on the canvas
 * 創造背景地圖，並放入map_canvas中
 */
void create_map_image(gui_data_t* gui_data, int ori_X, int ori_Y, int draw_width, int draw_height)
{
  GdkPixbuf* pixbuf;
  int i, j;

  for(j=ori_Y; j < draw_height; j++) {
    for(i=ori_X; i < draw_width; i++) {

      int tempIdx;
      tempIdx = ((gui_data->mapHeight - j - 1) * gui_data->mapWidth + i) * 3; // 原點在左上角，Y軸顛倒才能符合Player的座標系統
      tempIdx = (tempIdx > 0) ? tempIdx : 0;
      tempIdx = (tempIdx < (gui_data->mapSize * 3)) ? tempIdx : (gui_data->mapSize * 3) - 1;

      // Map cell : empty = -1, unknown = 0, occupied = +1
      if(flag_showObstacleMap && gui_data->mapOB->cells[PLAYERC_MAP_INDEX(gui_data->mapOB,i,j)] == 1)
      {
        gui_data->map_pixels[tempIdx]     = 255;
        gui_data->map_pixels[tempIdx + 1] = 255;
        gui_data->map_pixels[tempIdx + 2] = 0;
      }
      else if(flag_showFootprintsMap && gui_data->mapFP->cells[PLAYERC_MAP_INDEX(gui_data->mapFP,i,j)] == 1)
      {
        gui_data->map_pixels[tempIdx]     = 255;
        gui_data->map_pixels[tempIdx + 1] = 0;
        gui_data->map_pixels[tempIdx + 2] = 0;
      }
      else if(gui_data->map->cells[PLAYERC_MAP_INDEX(gui_data->map,i,j)] == -1)
      {
        gui_data->map_pixels[tempIdx]     = 255;  // 白
        gui_data->map_pixels[tempIdx + 1] = 255;
        gui_data->map_pixels[tempIdx + 2] = 255;
      }
      else if(gui_data->map->cells[PLAYERC_MAP_INDEX(gui_data->map,i,j)] == 0)
      {
        gui_data->map_pixels[tempIdx]     = 100;  // 灰
        gui_data->map_pixels[tempIdx + 1] = 100;
        gui_data->map_pixels[tempIdx + 2] = 100;
      }
      else
      {
        gui_data->map_pixels[tempIdx]     = 0;    // 黑
        gui_data->map_pixels[tempIdx + 1] = 0;
        gui_data->map_pixels[tempIdx + 2] = 0;
      }

      // ------Power Map-------
      if(showPowerMap_Flag && gui_data->mapPOWER->cells[PLAYERC_MAP_INDEX(gui_data->mapPOWER,i,j)] == 1)
        draw_extra_image(gui_data, i, j, 255, 155, 0, 1, 1);  // Orange

      // ------Zone Map-------
      if(showZoneMap_Flag && gui_data->mapZONE->cells[PLAYERC_MAP_INDEX(gui_data->mapZONE,i,j)] == 1)
        draw_extra_image(gui_data, i, j, 100, 100, 255, 1, 1);  // Blue

      // ------Virtual Wall-------
      if(showVirtualWall_Flag && gui_data->mapVW->cells[PLAYERC_MAP_INDEX(gui_data->mapVW,i,j)] == 1)
      {
        draw_extra_image(gui_data, i, j, 0, 255, 0, 3, 0);  // Green
      }

      // ------自己儲存的路徑圖------ 
      if(flag_showFootprints && gui_data->moving_path[tempIdx/3] > 0)
        draw_extra_image(gui_data, i, j, 0, 175, 175, 7, 0); // light yellow
    }
  }
  // create the pixbuf
  g_assert((pixbuf = gdk_pixbuf_new_from_data(gui_data->map_pixels,
                                              GDK_COLORSPACE_RGB,
                                              FALSE,
                                              8,
                                              gui_data->map->width,
                                              gui_data->map->height,
                                              3*gui_data->map->width,
                                              NULL,
                                              NULL)));
// ZoomIn
//  g_assert((pixbuf = gdk_pixbuf_scale_simple(pixbuf, WINDOW_SIZE_W, WINDOW_SIZE_H, GDK_INTERP_BILINEAR)));
  g_assert((gui_data->imageitem = gnome_canvas_item_new(
                                    gnome_canvas_root(gui_data->map_canvas),
                                    gnome_canvas_pixbuf_get_type(),
                                    "width-set" ,  TRUE,
                                    "height-set",  TRUE,
                                    "width" , gui_data->map->width * gui_data->map->resolution,
                                    "height", gui_data->map->height* gui_data->map->resolution,
                                    "x",   gui_data->map->origin[0],
                                    "y", -(gui_data->map->origin[1] + (gui_data->map->height * gui_data->map->resolution)),
                                    "pixbuf", pixbuf,
                                    NULL)));

  gnome_canvas_item_lower_to_bottom((GnomeCanvasItem*)gui_data->imageitem);
  gnome_canvas_item_show((GnomeCanvasItem*)gui_data->imageitem);

/* Map origin, in meters (i.e., the real-world coordinates of cell 0,0)*/
/*  gnome_canvas_set_scroll_region( gui_data->map_canvas,
          gui_data->map->origin[0],
        -(gui_data->map->origin[1] + gui_data->map->height * gui_data->map->resolution),
         (gui_data->map->origin[0] + gui_data->map->width  * gui_data->map->resolution),
         -gui_data->map->origin[1]);*/

  g_object_unref((GObject*)pixbuf);
}

// 創出機器人
void create_robot(gui_data_t* gui_data, player_pose2d_t pose)
{
/*
--  GnomeCanvasGroup是一個包含其他GnomeCanvasItem的GnomeCanvasItem。
    它創建一個GnomeCanvas中畫布項的分層次樹狀結構。GnomeCanvas創建一個成
    為“根”的特殊的GnomeCanvasGroup，所有用戶創建的畫布項加到“根”組。
--  GnomeCanvasItem 是畫布項的抽象基類。
*/
  GnomeCanvasGroup* robot;
  GnomeCanvasItem* robot_circle;
  GnomeCanvasItem* robot_v;

  GnomeCanvasItem* robot_goal;
  GnomeCanvasPoints* points;

  // 在地圖上 gui_data->map_canvas, 創立機器人GnomeCanvasGroup 與設定位置
  g_assert((robot = (GnomeCanvasGroup*)
            gnome_canvas_item_new(gnome_canvas_root(gui_data->map_canvas),
                                  gnome_canvas_group_get_type(),
                                  "x", 0.0, "y", 0.0,
                                  NULL)));

  // 建立代表機器人的橢圓，圓形、邊框顏色、填滿的顏色
  g_assert((robot_circle =
            gnome_canvas_item_new(robot,
                                  gnome_canvas_ellipse_get_type(),
                                  "x1", -ROBOT_RADIUS,
                                  "y1", -ROBOT_RADIUS,
                                  "x2",  ROBOT_RADIUS,
                                  "y2",  ROBOT_RADIUS,
                                  "outline_color_rgba", COLOR_BLACK,
                                  "fill_color_rgba", COLOR_RED,
                                  "width_pixels", 1,
                                  NULL)));

  g_assert((points = gnome_canvas_points_new(3)));
  points->coords[0] = ROBOT_RADIUS * cos( ROBOT_V_ANGLE);
  points->coords[1] = ROBOT_RADIUS * sin( ROBOT_V_ANGLE);
  points->coords[2] = 0.0;
  points->coords[3] = 0.0;
  points->coords[4] = ROBOT_RADIUS * cos( ROBOT_V_ANGLE);
  points->coords[5] = ROBOT_RADIUS * sin(-ROBOT_V_ANGLE);

  // 機器人面對方向的Ｖ型線條
  g_assert((robot_v =
            gnome_canvas_item_new(robot,
                                  gnome_canvas_line_get_type(),
                                  "points", points,
                                  "fill_color_rgba", COLOR_BLUE,
                                  "width_pixels", 1,
                                  NULL)));

  // a triangle to mark the goal
  points->coords[0] = ROBOT_RADIUS * cos(   M_PI/2.0);
  points->coords[1] = ROBOT_RADIUS * sin(   M_PI/2.0);
  points->coords[2] = ROBOT_RADIUS * cos( 7*M_PI/6.0);
  points->coords[3] = ROBOT_RADIUS * sin( 7*M_PI/6.0);
  points->coords[4] = ROBOT_RADIUS * cos(11*M_PI/6.0);
  points->coords[5] = ROBOT_RADIUS * sin(11*M_PI/6.0);

  g_assert((robot_goal =
            gnome_canvas_item_new(gnome_canvas_root(gui_data->map_canvas),
                                  gnome_canvas_polygon_get_type(),
                                  "points", points,
                                  "outline_color_rgba", COLOR_BLACK,
                                  "fill_color_rgba",  COLOR_RED,
                                  "width_pixels", 1,
                                  NULL)));
  // 還沒設定目的地，先隱藏顯示
  gnome_canvas_item_hide(robot_goal);
  gnome_canvas_points_unref(points);
  move_item((GnomeCanvasItem*)robot,pose, 1, 0);

  gui_data->robot_item  = (GnomeCanvasItem*)robot;
  gui_data->robot_goal  = robot_goal;
  gui_data->robot_pose  = pose;

  update_robot_info(gui_data, pose);  // show message on status bar

  gtk_signal_connect(GTK_OBJECT(robot_goal), "event",
                     (GtkSignalFunc)_mouse_callback, (void*)gui_data);
  gtk_signal_connect(GTK_OBJECT(robot), "event",
                     (GtkSignalFunc)_mouse_callback, (void*)gui_data);

}

// 創出充電站位置
void create_dock(gui_data_t* gui_data, int idx)
{
/*
--  GnomeCanvasGroup是一個包含其他GnomeCanvasItem的GnomeCanvasItem。
    它創建一個GnomeCanvas中畫布項的分層次樹狀結構。GnomeCanvas創建一個成
    為“根”的特殊的GnomeCanvasGroup，所有用戶創建的畫布項加到“根”組。
--  GnomeCanvasItem 是畫布項的抽象基類。
*/
  GnomeCanvasGroup* dock;
  GnomeCanvasItem* dock_square;
  GnomeCanvasItem* dock_v;

  GnomeCanvasPoints* points;

  // 在地圖上 gui_data->map_canvas, 創立機器人GnomeCanvasGroup 與設定位置
  g_assert((dock = (GnomeCanvasGroup*)
            gnome_canvas_item_new(gnome_canvas_root(gui_data->map_canvas),
                                  gnome_canvas_group_get_type(),
                                  "x", 0.0, "y", 0.0,
                                  NULL)));

  // 建立代表充電站的長方形、邊框顏色、填滿的顏色
  g_assert((dock_square =
            gnome_canvas_item_new(dock,
                                  gnome_canvas_rect_get_type (),
                                  "x1", -(ROBOT_RADIUS/2),
                                  "y1", -ROBOT_RADIUS,
                                  "x2",  ROBOT_RADIUS/2,
                                  "y2",  ROBOT_RADIUS,
                                  "outline_color_rgba", COLOR_BLACK,
                                  "fill_color_rgba", COLOR_BLUE,
                                  "width_pixels", 1,
                                  NULL)));

  g_assert((points = gnome_canvas_points_new(3)));
  points->coords[0] = -((ROBOT_RADIUS/2) * cos( DOCK_V_ANGLE));
  points->coords[1] = -((ROBOT_RADIUS/2) * sin( DOCK_V_ANGLE));
  points->coords[2] = 0.0;
  points->coords[3] = 0.0;
  points->coords[4] = -((ROBOT_RADIUS/2) * cos( DOCK_V_ANGLE));
  points->coords[5] = -((ROBOT_RADIUS/2) * sin(-DOCK_V_ANGLE));

  // 面對方向的Ｖ型線條
  g_assert((dock_v =
            gnome_canvas_item_new(dock,
                                  gnome_canvas_line_get_type(),
                                  "points", points,
                                  "fill_color_rgba", COLOR_RED,
                                  "width_pixels", 2,
                                  NULL)));

  gnome_canvas_points_unref(points);
  move_item((GnomeCanvasItem*)dock, gui_data->dock_pose[idx], 1, 0);

  gui_data->dock_item[idx]  = (GnomeCanvasItem*)dock;
}

void move_item(GnomeCanvasItem* item, player_pose2d_t pose, int raise, int showMes)
{
  double t[6];

  t[0] =  cos(pose.pa);
  t[1] = -sin(pose.pa);
  t[2] =  sin(pose.pa);
  t[3] =  cos(pose.pa);
  t[4] =  pose.px;
  t[5] = -pose.py;
  gnome_canvas_item_affine_absolute(item, t);
  if(raise)
    gnome_canvas_item_raise_to_top(item); // 提到圖層最上方

  if(showMes)
  {
    gui_data_t goal_data;
    goal_data.device = "Target";
    goal_data.robot_pose = pose;
    goal_data.planner = NULL;
    platPos_to_mapPos(&goal_data.onmap_pose, pose);
    update_status_bar(&goal_data);
  }
}

// 機器位置轉地圖位置
void platPos_to_mapPos(player_pose2d_t* mapPos, player_pose2d_t platPos)
{
  // 化為地圖單位
  mapPos->px = (platPos.px - mapOrigin[0]) / mapResolution + 0.5;
  mapPos->py = (platPos.py - mapOrigin[1]) / mapResolution + 0.5;
  mapPos->pa = platPos.pa;
}

// 地圖位置轉機器位置
void mapPos_to_platPos(player_pose2d_t* platPos, player_pose2d_t mapPos)
{
  platPos->px = (mapPos.px * mapResolution) + mapOrigin[0];
  platPos->py = (mapPos.py * mapResolution) + mapOrigin[0];
  platPos->pa = mapPos.pa;
}

// 行進路線
void draw_waypoints(gui_data_t* gui_data)
{
  int i;
  GnomeCanvasPoints* points;
  GnomeCanvasPoints* linepoints;
  GnomeCanvasItem* waypoint = NULL;
  GnomeCanvasItem* line;
  player_pose2d_t pose;

  destroy_waypoints(gui_data);

  if(gui_data->planner->path_done)
  {
    // 非拖行且非設定角度
    if(!dragging && !setting_theta)
      gnome_canvas_item_hide(gui_data->robot_goal);
  }
  else if(gui_data->planner->path_valid)
  {
    g_assert((gui_data->robot_path =
              gnome_canvas_item_new(gnome_canvas_root(gui_data->map_canvas),
                                    gnome_canvas_group_get_type(),
                                    "x", 0.0, "y", 0.0,
                                    NULL)));
    g_assert((points = gnome_canvas_points_new(3)));
    g_assert((linepoints = gnome_canvas_points_new(2)));

    // a small triangle to mark each waypoint
    points->coords[0] = 0.5 * ROBOT_RADIUS * cos(   M_PI/2.0);
    points->coords[1] = 0.5 * ROBOT_RADIUS * sin(   M_PI/2.0);
    points->coords[2] = 0.5 * ROBOT_RADIUS * cos( 7*M_PI/6.0);
    points->coords[3] = 0.5 * ROBOT_RADIUS * sin( 7*M_PI/6.0);
    points->coords[4] = 0.5 * ROBOT_RADIUS * cos(11*M_PI/6.0);
    points->coords[5] = 0.5 * ROBOT_RADIUS * sin(11*M_PI/6.0);

    for( i = MAX(gui_data->planner->curr_waypoint-1, 0);
         i < gui_data->planner->waypoint_count;
         i++)
    {
      if(i < gui_data->planner->waypoint_count-1) // draw waypoint
      {
        g_assert((waypoint =
                  gnome_canvas_item_new((GnomeCanvasGroup*)gui_data->robot_path,
                                        gnome_canvas_polygon_get_type(),
                                        "points", points,
                                        "outline_color_rgba", COLOR_BLACK,
                                        "fill_color_rgba",
                                        robot_colors[0],
                                        "width_pixels", 1,
                                        NULL)));
      }
      else
      {
        if(!dragging && !setting_theta)
        {
          gnome_canvas_item_show(gui_data->robot_goal); // show goal
          waypoint = gui_data->robot_goal;
        }
      }

      if(waypoint)
      {
        pose.px =  gui_data->planner->waypoints[i][0];
        pose.py =  gui_data->planner->waypoints[i][1];
        pose.pa = 0.0;
        move_item(waypoint, pose, 0, 0);
      }

      if( i > 0)
      {
        linepoints->coords[0] =  gui_data->planner->waypoints[i-1][0];
        linepoints->coords[1] = -gui_data->planner->waypoints[i-1][1];
        linepoints->coords[2] =  gui_data->planner->waypoints[i][0];
        linepoints->coords[3] = -gui_data->planner->waypoints[i][1];

        g_assert((line =
                  gnome_canvas_item_new((GnomeCanvasGroup*)gui_data->robot_path,
                                        gnome_canvas_line_get_type(),
                                        "points", linepoints,
                                        "width_pixels", 3,
                                        "fill-color-rgba",
                                        robot_colors[1],
                                        NULL)));
      }
    }
    gnome_canvas_points_unref(points);
    gnome_canvas_points_unref(linepoints);
  }
}

// 目標物
void draw_goal(gui_data_t* gui_data)
{
  if(!dragging && !setting_theta)
  {
    player_pose2d_t p;
    p.px = gui_data->planner->gx;
    p.py = gui_data->planner->gy;
    p.pa = gui_data->planner->ga;
    move_item(gui_data->robot_goal, p, 0, 0);
    gnome_canvas_item_show(gui_data->robot_goal);
  }
}

// 地圖加工
void draw_extra_image(gui_data_t* gui_data, int i, int j, int r, int g, int b, int boldSize, int transparent)
{
  // 加粗線條
  int tempX, tempY, tempIdx;
  for(tempY = j-boldSize ; tempY < j+boldSize; tempY++){
    for(tempX = i-boldSize ; tempX < i+boldSize; tempX++){

      tempIdx = ((gui_data->map->height - tempY - 1) * gui_data->map->width + tempX) * 3;

      if( (tempX >= 0 && tempX < gui_data->map->width)
       && (tempY >= 0 && tempY < gui_data->map->height)
       &&  tempIdx < (gui_data->mapSize * 3) )
      {

        if( transparent 
         && (  (   gui_data->map_pixels[tempIdx]     == 0 
                && gui_data->map_pixels[tempIdx + 1] == 0 
                && gui_data->map_pixels[tempIdx + 2] == 0 )
             ||(   gui_data->map_pixels[tempIdx]     == 255 
                && gui_data->map_pixels[tempIdx + 1] == 0 
                && gui_data->map_pixels[tempIdx + 2] == 0 )))
        {
          // do nothing
        }
        else
        {
        gui_data->map_pixels[tempIdx]     = r;
        gui_data->map_pixels[tempIdx + 1] = g;
        gui_data->map_pixels[tempIdx + 2] = b;
      }
    }
  }
  }
}

void destroy_waypoints(gui_data_t* gui_data)
{
  if(gui_data->robot_path)
  {
    gtk_object_destroy(GTK_OBJECT(gui_data->robot_path));
    gui_data->robot_path = NULL;
  }
}
/*
  gdouble lower_value, upper_value, page_size, max_value, line_height, to_value;
  
  lower_value = gtk_adjustment_get_lower(adjust);
  upper_value = gtk_adjustment_get_upper(adjust);
  page_size = gtk_adjustment_get_page_size(adjust);

  printf("increment: %f   lower: %f   upper: %f   page size: %f\n",adjust->step_increment, lower_value, upper_value, page_size);

--------------------

ScrollBar's value

typedef struct
{
  gdouble value; // 滾動條當前的位置。這個變量必須處於lower和upper值之間
  gdouble upper; // 滾動條upper值
  gdouble lower; // 滾動條lower值
  gdouble step_increment; // 設置步長 -> 點擊向下箭頭
  gdouble page_increment; // 設置步長 -> 按下向下翻頁鍵
  gdouble page_size; // page_size是在屏幕上一次可見的區域，它取決於構件的大小。
} GtkAdjustment;
*/
