/* 
 *  libplayerc : a Player client library
 *  Copyright (C) Andrew Howard 2002-2003
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
 *
 */
/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) Andrew Howard 2003
 *                      
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */
/***************************************************************************
 * Desc: Planner device proxy
 * Author: Brian Gerkey
 * Date: June 2004
 * CVS: $Id$
 **************************************************************************/

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "playerc.h"
#include "error.h"

//-------------------------------------------------------------------------------------------------
// Local declarations
void playerc_planner_putmsg(playerc_planner_t *device, player_msghdr_t *header,
                              player_planner_data_t *data, size_t len);

//-------------------------------------------------------------------------------------------------
// Create a new planner proxy
playerc_planner_t *playerc_planner_create(playerc_client_t *client, int index)
{
  playerc_planner_t *device;

  device = malloc(sizeof(playerc_planner_t));
  memset(device, 0, sizeof(playerc_planner_t));
  playerc_device_init(&device->info, client, PLAYER_PLANNER_CODE, index,
                      (playerc_putmsg_fn_t) playerc_planner_putmsg);

  
  return device;
}
//-------------------------------------------------------------------------------------------------
// Destroy a planner proxy
void playerc_planner_destroy(playerc_planner_t *device)
{
  playerc_device_term(&device->info);
  free(device->waypoints);
  free(device);

  return;
}
//-------------------------------------------------------------------------------------------------
// Subscribe to the planner device
int playerc_planner_subscribe(playerc_planner_t *device, int access)
{
  return playerc_device_subscribe(&device->info, access);
}
//-------------------------------------------------------------------------------------------------
// Un-subscribe from the planner device
int playerc_planner_unsubscribe(playerc_planner_t *device)
{
  return playerc_device_unsubscribe(&device->info);
}
//-------------------------------------------------------------------------------------------------
// Process incoming data
void 
playerc_planner_putmsg(playerc_planner_t *device, player_msghdr_t *header,
                       player_planner_data_t *data, size_t len)
{
  if((header->type == PLAYER_MSGTYPE_DATA) &&
     (header->subtype == PLAYER_PLANNER_DATA_STATE))
  {
    device->path_valid = data->valid;
    device->path_done = data->done;

    device->px = data->pos.px;
    device->py = data->pos.py;
    device->pa = data->pos.pa;

    device->gx = data->goal.px;
    device->gy = data->goal.py;
    device->ga = data->goal.pa;

    device->wx = data->waypoint.px;
    device->wy = data->waypoint.py;
    device->wa = data->waypoint.pa;

    device->goalidx = data->goalidx;

    device->curr_waypoint = data->waypoint_idx;
    device->waypoint_count = data->waypoints_count;
  }
}
//-------------------------------------------------------------------------------------------------
//if mapIndex=0 that means don't need to reference other map layer
int 
playerc_planner_set_cmd_pose(playerc_planner_t *device, 
                                 double gx, double gy, double ga, uint32_t mapIndex)
{
  player_planner_cmd_t cmd;

  cmd.goal.px = gx;
  cmd.goal.py = gy;
  cmd.goal.pa = ga;
  cmd.map_index = mapIndex;

  return playerc_client_write(device->info.client, &device->info, 
                              PLAYER_PLANNER_CMD_GOAL,
                              &cmd, NULL);
}
//-------------------------------------------------------------------------------------------------
//if mapIndex=0 that means don't need to reference other map layer
int 
playerc_planner_set_cmd_pose_list(playerc_planner_t *device, player_pose2d_t *list, uint32_t count, uint32_t mapIndex)
{
  player_planner_goallist_cmd_t cmd;

  cmd.goals_count = count;
  cmd.goals = list;
  cmd.map_index = mapIndex;

  return playerc_client_write(device->info.client, &device->info, 
                              PLAYER_PLANNER_CMD_GOALLIST,
                              &cmd, NULL);
}
//-------------------------------------------------------------------------------------------------
// Get the list of waypoints.  The writes the result into the proxy
// rather than returning it to the caller.
int playerc_planner_get_waypoints(playerc_planner_t *device)
{
  int i;
  player_planner_waypoints_req_t *config;

  if(playerc_client_request(device->info.client, 
                            &device->info,
                            PLAYER_PLANNER_REQ_GET_WAYPOINTS,
                            NULL, (void**)&config) < 0)
    return -1;
  
  device->waypoint_count = config->waypoints_count;
  device->waypoints = realloc(device->waypoints,sizeof(*device->waypoints)*device->waypoint_count);
  for(i=0;i<device->waypoint_count;i++)
  {
    device->waypoints[i][0] = config->waypoints[i].px;
    device->waypoints[i][1] = config->waypoints[i].py;
    device->waypoints[i][2] = config->waypoints[i].pa;
  }
  player_planner_waypoints_req_t_free(config);
  return 0;
}
//-------------------------------------------------------------------------------------------------
// Enable/disable robot motion
int playerc_planner_enable(playerc_planner_t *device, int state, int explore_state)
{
  player_planner_enable_req_t config;

  config.state = state;
  config.explore_state = explore_state;

  if(playerc_client_request(device->info.client, &device->info, 
                            PLAYER_PLANNER_REQ_ENABLE,
                            &config, NULL) < 0)
    return(-1);
  else
    return(0);
}
//-------------------------------------------------------------------------------------------------

