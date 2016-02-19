/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2004
 *     Brian Gerkey
 */

#include <assert.h>
#include "playernav.h"

/*
 * Connect to player at each host:port pair, as specified by the global
 * vars 'hostnames' and 'ports'.  Also subscribes to each device, and adds 
 * each client into a new multiclient (which is returned)
 */
int init_player(gui_data_t* gui_data)
{
  /* Connect to Player */
  gui_data->client = playerc_client_create(NULL, gui_data->hostname, gui_data->port);
  assert(gui_data->client);
  
  if(playerc_client_connect(gui_data->client) < 0)
  {
    fprintf(stderr, "Failed to connect to %s:%d\n\n", gui_data->hostname, gui_data->port);
    return -1;
  }

  gui_data->fiducial = playerc_fiducial_create(gui_data->client, 0);
  assert(gui_data->fiducial);

  if (playerc_fiducial_subscribe(gui_data->fiducial, PLAYER_OPEN_MODE) < 0)
  {
    fprintf(stderr, "Failed to subscribe to fiducial\n\n");
    return -1;
  }
  // create map
  gui_data->map = playerc_map_create(gui_data->client, 0);
  assert(gui_data->map);
  if(playerc_map_subscribe(gui_data->map, PLAYER_OPEN_MODE) < 0)
  {
    fprintf(stderr, "Failed to subscribe to map (map0)\n\n");
    return -1;
  }
/*  
  // virtual wall map 預先讀取，可在選單更換Subscribe
  gui_data->mapVW = playerc_map_create(gui_data->client, gui_data->mapVW_idx);
  assert(gui_data->mapVW);
  if(playerc_map_subscribe(gui_data->mapVW, PLAYER_OPEN_MODE) < 0)
  {
    printf("Failed to subscribe to virtual wall map (map%d)\n\n", gui_data->mapVW_idx);
    gui_data->mapVW = NULL;
  }
*/

  // foot print map
  gui_data->mapOB = playerc_map_create(gui_data->client, MAP_OBSTACLE);
  assert(gui_data->mapOB);
  if(playerc_map_subscribe(gui_data->mapOB, PLAYER_OPEN_MODE) < 0)
  {
    printf("Failed to subscribe to foot print map (map%d)\n\n",MAP_OBSTACLE);
    gui_data->mapOB = NULL;
  }

  // foot print map
  gui_data->mapFP = playerc_map_create(gui_data->client, MAP_TRAJECTORY);
  assert(gui_data->mapFP);
  if(playerc_map_subscribe(gui_data->mapFP, PLAYER_OPEN_MODE) < 0)
  {
    printf("Failed to subscribe to foot print map (map%d)\n\n",MAP_TRAJECTORY);
    gui_data->mapFP = NULL;
  }

  /* Get the map, just so we know how big to make the window */
  if(playerc_map_get_map(gui_data->map) < 0)
  {
    fprintf(stderr, "Failed to get map\n");
    // no map yet; guess some values to initialize the GUI
    gui_data->map->width = gui_data->map->height = 400;
    gui_data->map->resolution = 0.1;
    gui_data->map->origin[0] = -20.0;
    gui_data->map->origin[1] = -20.0;
  }
  gui_data->mapSize  = gui_data->map->width * gui_data->map->height;
  gui_data->mapWidth  = gui_data->map->width; // int
  gui_data->mapHeight = gui_data->map->height;
  mapOrigin[0]  = gui_data->map->origin[0];
  mapOrigin[1]  = gui_data->map->origin[1];
  mapResolution = gui_data->map->resolution;

  printf("Map size:(%d,%d), origin:(%.4f,%.4f) resolution:%.4f\n"
    , gui_data->map->width,     gui_data->map->height
    , gui_data->map->origin[0], gui_data->map->origin[1], gui_data->map->resolution);

  player_pose2d_t tmp_pose;
  tmp_pose.px = 0;
  tmp_pose.py = 0;
  mapPos_to_platPos(&gui_data->poseMin, tmp_pose);

  tmp_pose.px = gui_data->mapWidth;
  tmp_pose.py = gui_data->mapHeight;
  mapPos_to_platPos(&gui_data->poseMax, tmp_pose);

  printf("Position2d - min:(%.2f,%.2f) max:(%.2f,%.2f)\n\n"
    , gui_data->poseMin.px, gui_data->poseMin.py
    , gui_data->poseMax.px, gui_data->poseMax.py);

  // 創立與初始化行經路線圖
  gui_data->moving_path = malloc(  gui_data->mapSize * sizeof(int));
  memset(gui_data->moving_path, 0, gui_data->mapSize * sizeof(int));

  gui_data->position2d = playerc_position2d_create(gui_data->client, 2); // channel from SLAM
  assert(gui_data->position2d);
  if(playerc_position2d_subscribe(gui_data->position2d, PLAYER_OPEN_MODE) < 0)
  {
    fprintf(stderr, "Warning: Failed to subscribe to position2d on robot.\n");
    playerc_position2d_destroy(gui_data->position2d);
    gui_data->position2d = NULL;
  }
  return 1;
}


int subscribe_map_and_load(gui_data_t* gui_data, playerc_map_t* map, int mapIdx, GtkWidget* item, int mode)
{
  if(map)
    playerc_map_destroy(map);

  assert(map = playerc_map_create(gui_data->client, mapIdx));
  switch(mode)
  {
    case 0:
      gui_data->mapVW = map;
      break;
    case 1:
      gui_data->mapZONE = map;
      break;
    case 2:
      gui_data->mapPOWER = map;
      break;
  }  
  if(playerc_map_subscribe(map, PLAYER_OPEN_MODE) < 0)
  {
    printf("Failed to subscribe to map%d\n\n", mapIdx);
    return 0;
  }
  printf("Successfully subscribe to map%d\n\n", mapIdx);

  // 讀取地圖，因為這些地圖值是固定的，所以只需要讀取一次，如果要重複讀取，則使用refresh
  if(playerc_map_get_map(map) < 0)
  {
    printf("Get map%d error\n\n", mapIdx);
    return 0;
  }
  else
  {
    char buf[32];
    switch(mode)
    {
      case 0:
        sprintf(buf, "Virtual wall (map%d)", mapIdx);
        gui_data->mapVW = map;
        break;
      case 1:
        sprintf(buf, "Zone map (map%d)", mapIdx);
        gui_data->mapZONE = map;
        break;
      case 2:
        sprintf(buf, "Power map (map%d)", mapIdx);
        gui_data->mapPOWER = map;
        break;
    }                
    gtk_menu_item_set_label(item, buf);
  }
  return 1;
}

//planner是用來擷取 waypoint值的，因為subscribe後SLAM的CPU使用率會爆增(120%->160%+)，因此自由選用是否要subscribe
int subscribe_planner(gui_data_t* gui_data)
{  
  if(!gui_data->planner) // planner is NULL
  {
  gui_data->planner = playerc_planner_create(gui_data->client, 0);
  assert(gui_data->planner);
  }
  
  if(playerc_planner_subscribe(gui_data->planner, PLAYER_OPEN_MODE) < 0)
  {
    fprintf(stderr, "Warning: Failed to subscribe to planner on robot; you won't be able to set robot's goal.\n");
    playerc_planner_destroy(gui_data->planner);
    gui_data->planner = NULL;
    return -1;
  }
  return 1;
}
void unsubscribe_planner(gui_data_t* gui_data)
{
  if(gui_data->planner)
    playerc_planner_unsubscribe(gui_data->planner);
}

void fini_player(gui_data_t* gui_data)
{
  puts("\nDestroy Connect\n");
  if(gui_data->fiducial)
  {
    playerc_fiducial_unsubscribe(gui_data->fiducial);
    playerc_fiducial_destroy(gui_data->fiducial);
  }
  if(gui_data->map)
    playerc_map_destroy(gui_data->map);
  if(gui_data->mapVW)
    playerc_map_destroy(gui_data->mapVW);
  if(gui_data->mapZONE)
    playerc_map_destroy(gui_data->mapZONE);
  if(gui_data->mapPOWER)
    playerc_map_destroy(gui_data->mapPOWER);
  if(gui_data->mapOB)
    playerc_map_destroy(gui_data->mapOB);
  if(gui_data->mapFP)
    playerc_map_destroy(gui_data->mapFP);
  if(gui_data->position2d)
    playerc_position2d_destroy(gui_data->position2d);
  if(gui_data->planner)
    playerc_planner_destroy(gui_data->planner);
  playerc_client_destroy(gui_data->client); 
}
