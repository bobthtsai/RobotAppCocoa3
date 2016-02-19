#include <stdio.h> 
#include <stdlib.h> 
#include "playernav.h"

void queue_create() { 
    front = rear = (Node*) malloc(sizeof(Node)); 
    front->next = rear->next = NULL; 
} 

void queue_showfront(){ 
    if(front->next == NULL) 
        printf("\n佇列為空！"); 
    else 
        printf("\n頂端值：%d", front->next->data); 
} 

void queue_add(player_pose2d_t data) { 
    Node *newnode; 

    newnode = (Node*) malloc(sizeof(Node)); 

    if(front->next == NULL)  
        front->next = newnode; 
    
    newnode->data = data; 
    newnode->next = NULL; 
    rear->next = newnode; 
    rear = newnode; 
} 

void queue_delete() { 
    Node* tmpnode; 

    if(front->next == NULL) { 
        printf("\n佇列已空！"); 
        return; 
    } 

    tmpnode = front->next; 
    front->next = tmpnode->next; 
    free(tmpnode);    
} 

void queue_showqueue() { 
    Node* tmpnode;

    tmpnode = front->next; 

    printf("\nContent:"); 
    while(tmpnode != NULL) { 
        printf("%d ", tmpnode->data); 
        tmpnode = tmpnode->next; 
    } 
} 


/*
 // if(gui_data->robot_pose)
  double transX, transY;
  double radius, radian;
  int degree;

  radius = 150;

  for(degree =0; degree < 360; degree++)
  {
    radian = (degree * 3.14 / 180) + gui_data->robot_pose.pa; // 度數轉為弧度
  
    transX = (gui_data->map->width/2)  + (gui_data->robot_pose.px / gui_data->map->resolution);
    transY = (gui_data->map->height/2) + (gui_data->robot_pose.py / gui_data->map->resolution);

    transX = transX  +  (radius * cos(radian)); 
    transY = transY  +  (radius * sin(radian));   // point OK

    int tempIdx, tempX, tempY;

    int addWidth; // 加寬格數
    addWidth = 1;

    // printf("Decode position (%d,%d)\n", pX, pY);
    for(tempY = transY-addWidth ; tempY < transY+addWidth; tempY++)
    {
      for(tempX = transX-addWidth ; tempX < transX+addWidth; tempX++)
      {
        tempIdx = ((gui_data->map->height - tempY - 1) * gui_data->map->width + tempX) * 3;

        if( tempIdx >= 0 && tempIdx < (mapsize * 3) )
        {
          pixels[tempIdx]     = 0;
          pixels[tempIdx + 1] = 200;
          pixels[tempIdx + 2] = 0;
        }
      }
    }
  }

 // printf("Position:( %.3f, %.3f, %.3f)\n", gui_data->robot_pose.px, gui_data->robot_pose.py, gui_data->robot_pose.pa);
/*
  for(i =0; i < 8; i ++)
  {
    radian = (degree * 3.14 / 180) + gui_data->robot_pose.pa; // 度數轉為弧度
  
    transX = (gui_data->map->width/2)  + (gui_data->robot_pose.px / gui_data->map->resolution);
    transY = (gui_data->map->height/2) + (gui_data->robot_pose.py / gui_data->map->resolution);

    transX = transX  +  (radius * cos(radian)); 
    transY = transY  +  (radius * sin(radian));   // point OK

    int tempIdx, tempX, tempY;

    int addWidth; // 加寬格數
    addWidth = 3;

    // printf("Decode position (%d,%d)\n", pX, pY);
    for(tempY = transY-addWidth ; tempY < transY+addWidth; tempY++)
    {
      for(tempX = transX-addWidth ; tempX < transX+addWidth; tempX++)
      {
        tempIdx = ((gui_data->map->height - tempY - 1) * gui_data->map->width + tempX) * 3;

        if( tempIdx >= 0 && tempIdx < (mapsize * 3) )
        {

          switch(i)
          {
            case 0:
              pixels[tempIdx]     = 200;
              pixels[tempIdx + 1] = 100;
              pixels[tempIdx + 2] = 100;
              break;
            case 1:
              pixels[tempIdx]     = 100;
              pixels[tempIdx + 1] = 200;
              pixels[tempIdx + 2] = 100;
              break;            
            case 2:
              pixels[tempIdx]     = 225;
              pixels[tempIdx + 1] = 0;
              pixels[tempIdx + 2] = 0;
              break;
            case 3:
              pixels[tempIdx]     = 0;
              pixels[tempIdx + 1] = 255;
              pixels[tempIdx + 2] = 0;
              break;
            case 4:
              pixels[tempIdx]     = 0;
              pixels[tempIdx + 1] = 0;
              pixels[tempIdx + 2] = 255;
              break;
            case 5:
              pixels[tempIdx]     = 255;
              pixels[tempIdx + 1] = 255;
              pixels[tempIdx + 2] = 0;
              break;
            case 6:
              pixels[tempIdx]     = 255;
              pixels[tempIdx + 1] = 0;
              pixels[tempIdx + 2] = 255;
              break;
            case 7:
              pixels[tempIdx]     = 0;
              pixels[tempIdx + 1] = 255;
              pixels[tempIdx + 2] = 255;
              break;
            }
            degree+=45;
        }
      }
    }
  }
*/

//  map_coverage = map_footprint / mapsize_occupied;

//  snprintf(maptext_occupied, sizeof(maptext_occupied), "Map occupied size:");
//  snprintf(maptext_coverage, sizeof(maptext_coverage), "Map coverage ");

  /*
  // cause program crash
  g_assert((mapCanvas_occupied =
            gnome_canvas_item_new(gnome_canvas_root(gui_data->map_canvas),
                                  gnome_canvas_text_get_type(),
                                  "text", maptext_occupied,
                                  "x", -10.0,
                                  "y", -12.0,
                                  "x-offset",  2.0*ROBOT_RADIUS,
                                  "y-offset", -2.0*ROBOT_RADIUS,
                                  "fill-color-rgba", COLOR_BLUE,
                                  NULL)));

  g_assert((mapCanvas_coverage =
            gnome_canvas_item_new(gnome_canvas_root(gui_data->map_canvas),
                                  gnome_canvas_text_get_type(),
                                  "text", maptext_coverage,
                                  "x", -10.0,
                                  "y", -11.0,
                                  "x-offset",  2.0*ROBOT_RADIUS,
                                  "y-offset", -2.0*ROBOT_RADIUS,
                                  "fill-color-rgba", COLOR_BLUE,
                                  NULL)));*/
/////////////////////////////////////

                                  /*
// 機器人按鍵 (滑鼠事件)實作
static gboolean
_robot_button_callback(GnomeCanvasItem *item,
                       GdkEvent *event,
                       gpointer data)
{
  gboolean onrobot=FALSE; // 點在機器人上
  gboolean ongoal=FALSE;  // 點在目的地上

  double theta; // angle
  static GnomeCanvasPoints* points = NULL;
  static GnomeCanvasItem* setting_theta_line = NULL;
  pose_t pose;
  double mean[3];
  static double cov[6] = {0.5*0.5, 0.5*0.5, (M_PI/6.0)*(M_PI/6.0), 0, 0, 0};

  gui_data_t* gui_data = (gui_data_t*)data;

  pose.pa = 0.0;
  pose.px =  event->button.x;
  pose.py = -event->button.y;

  // lookup (and store) which robot (or goal) was clicked
  if((item != (GnomeCanvasItem*)gnome_canvas_root(gui_data->map_canvas)) &&
     !setting_theta && !setting_goal)
  {
    if(item == gui_data->robot_item)
    {
      onrobot = TRUE; //點在機器人上
    }
    else if(item == gui_data->robot_goal)
    {
      ongoal = TRUE;  // 點在目的地上
    }
    
    if(!show_robot_names)
      gnome_canvas_item_hide(gui_data->robot_label);
  }

  // 滑鼠事件型態
  switch(event->type)
  {
    case GDK_2BUTTON_PRESS:
      switch(event->button.button)
      {
        case 1: // left button
          printf("Double clicked at ( %f, %f) OnRobot:%d\n",  pose.px, pose.py, onrobot); 

          if(!onrobot){            

            if(!points)
              g_assert((points = gnome_canvas_points_new(2)));

            points->coords[0] = pose.px;
            points->coords[1] = -pose.py;
            points->coords[2] = pose.px;
            points->coords[3] = -pose.py;

            theta = atan2(-points->coords[3] + points->coords[1], 
                            points->coords[2] - points->coords[0]);
            mean[0] = points->coords[0];
            mean[1] =-points->coords[1];
            mean[2] = theta;
            
            if(gui_data->planner)
            {
              printf("setting goal for robot to (%.3f, %.3f, %.3f)\n", mean[0], mean[1], mean[2]);
              playerc_planner_set_cmd_pose(gui_data->planner, mean[0], mean[1], mean[2], 0);

              gui_data->goal[0] = mean[0];
              gui_data->goal[1] = mean[1];
              gui_data->goal[2] = mean[2];
              gui_data->planner->waypoint_count = -1;
            }
          }
          break;
      }
      break;
    case GDK_MOTION_NOTIFY: 
     // printf("Motion notify\n");

      break;
    case GDK_BUTTON_RELEASE:
     // printf("Release\n");
  
      break;
    default:
      break;
  }

  return(TRUE);
}

void
canvas_to_meters(gui_data_t* gui_data, double* dx, double* dy, int cx, int cy)
{
  gnome_canvas_c2w(gui_data->map_canvas,cx,cy,dx,dy);
  *dy = -*dy;
}

void
item_to_meters(GnomeCanvasItem* item,
               double* dx, double* dy,
               double ix, double iy)
{
  *dx=ix;
  *dy=iy;
  gnome_canvas_item_i2w(item, dx, dy);
  *dy = -*dy;
}

void
meters_to_canvas(gui_data_t* gui_data, int* cx, int* cy, double dx, double dy)
{
  double x,y;
  x = dx;
  y =-dy;
  gnome_canvas_w2c(gui_data->map_canvas,x,y,cx,cy);
}
*/