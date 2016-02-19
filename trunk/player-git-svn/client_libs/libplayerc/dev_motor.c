/*
 *  libplayerc : a Player client library
 *  Copyright (C) LeonLi 2003-2004
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
 *  Copyright (C) LeonLi 2003
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
//-------------------------------------------------------------------------------------------------
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "playerc.h"
#include "error.h"
//-------------------------------------------------------------------------------------------------
// Local declarations
void playerc_motor_putmsg(playerc_motor_t *device, player_msghdr_t *header, uint8_t *data, size_t len);
//-------------------------------------------------------------------------------------------------
// Create a new motor proxy
playerc_motor_t *motor_create(playerc_client_t *client, int index)
{
  playerc_motor_t *device;

  device = malloc(sizeof(playerc_motor_t));
  memset(device, 0, sizeof(playerc_motor_t));
  playerc_device_init(&device->info, client, PLAYER_MOTOR_CODE, index,
                      (playerc_putmsg_fn_t) playerc_motor_putmsg);


  return device;
}
//-------------------------------------------------------------------------------------------------
// Destroy a motor proxy
void motor_destroy(playerc_motor_t *device)
{
  playerc_device_term(&device->info);
  free(device);

  return;
}
//-------------------------------------------------------------------------------------------------
// Subscribe to the motor device
int motor_subscribe(playerc_motor_t *device, int access)
{
  return playerc_device_subscribe(&device->info, access);
}
//-------------------------------------------------------------------------------------------------
// Un-subscribe from the motor device
int motor_unsubscribe(playerc_motor_t *device)
{
  return playerc_device_unsubscribe(&device->info);
}
//-------------------------------------------------------------------------------------------------
// Process incoming data
void playerc_motor_putmsg(playerc_motor_t *device, player_msghdr_t *header, uint8_t *data, size_t len)
{
  if( (header->type == PLAYER_MSGTYPE_DATA) &&
       (header->subtype == PLAYER_MOTOR_DATA_STATE) )
  {
    player_motor_data_t *motorData = (player_motor_data_t *) data;
    assert(header->size > 0);

    device->motorData=(*motorData);
  }
  else
    PLAYERC_WARN2("skipping motor message with unknown type/subtype: %s/%d\n",
                 msgtype_to_str(header->type), header->subtype);
}
//-------------------------------------------------------------------------------------------------
int motor_get_status (playerc_motor_t *device, uint8_t *state, float *current, float *watts, float *rpm, float *position)
{
  *state=device->motorData.state;
  *current=device->motorData.current;
  *watts=device->motorData.watts;
  *rpm=device->motorData.rpm;
  *position=device->motorData.position;

return 0;
}
//-------------------------------------------------------------------------------------------------
int motor_set_pwm (playerc_motor_t *device, float pwm)
{
  player_motor_cmd_pwm_t motorCmd;

  motorCmd.pwm=pwm;


  return playerc_client_write(device->info.client, &device->info,
                   PLAYER_MOTOR_CMD_PWM,
                   &motorCmd, NULL);
}
//-------------------------------------------------------------------------------------------------
//velocity unit: r.p.m
int motor_set_velocity (playerc_motor_t *device, float velocity, float acceleration)
{
  player_motor_cmd_velocity_t motorCmd;

  motorCmd.velocity=velocity;
  motorCmd.acceleration=acceleration;


  return playerc_client_write(device->info.client, &device->info,
                   PLAYER_MOTOR_CMD_VELOCITY,
                   &motorCmd, NULL);
}
//-------------------------------------------------------------------------------------------------
int motor_set_position (playerc_motor_t *device, float velocity, float acceleration, float position)
{
  player_motor_cmd_position_t motorCmd;

  motorCmd.velocity=velocity;
  motorCmd.acceleration=acceleration;
  motorCmd.position=position;


  return playerc_client_write(device->info.client, &device->info,
                   PLAYER_MOTOR_CMD_POSITION,
                   &motorCmd, NULL);
}
//-------------------------------------------------------------------------------------------------
int motor_set_delta_position (playerc_motor_t *device, float velocity, float acceleration, float deltaPosition)
{
  player_motor_cmd_delta_position_t motorCmd;

  motorCmd.velocity=velocity;
  motorCmd.acceleration=acceleration;
  motorCmd.deltaPosition=deltaPosition;


  return playerc_client_write(device->info.client, &device->info,
                   PLAYER_MOTOR_CMD_DELTA_POSITION,
                   &motorCmd, NULL);
}
//-------------------------------------------------------------------------------------------------

