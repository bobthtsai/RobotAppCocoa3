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
/*  dio Proxy for libplayerc library.
 *  Structure based on the rest of libplayerc.
 */
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "playerc.h"
#include "error.h"


//-------------------------------------------------------------------------------------------------
#define ASSERT_EN                                1
#define TRACE_EN                                 0
#define WARN_EN                                  0
#define ERROR_EN                                 1
#define FATAL_EN                                 1
#define DEBUG_EN                                 0
#define MSG_EN                                   0


#if ASSERT_EN
#define ASSERT(expr)                        ((expr) ? (void)0 : printf( "ASSERT:", (char *)__FILE__, __LINE__, "\r\n"))
#else
#define ASSERT(expr)                         ((void)0)
#endif

#if TRACE_EN
#define TRACE(...)                           printf("TRACE:");printf(__VA_ARGS__)
#else
#define TRACE(...)                           ((void)0)
#endif

#if WARN_EN
#define WARN(...)                            printf("WARN:");printf(__VA_ARGS__)
#else
#define WARN(...)                            ((void)0)
#endif

#if ERROR_EN
#define ERROR(...)                           printf("ERROR:");printf(__VA_ARGS__)
#else
#define ERROR(...)                           ((void)0)
#endif

#if FATAL_EN
#define FATAL(...)                           printf("FATAL:");printf(__VA_ARGS__)
#else
#define FATAL(...)                           ((void)0)
#endif

#if DEBUG_EN
#define DEBUG(...)                           printf("DEBUG:");printf(__VA_ARGS__)
#else
#define DEBUG(...)                           ((void)0)
#endif

#if MSG_EN
#define MSG(...)                             printf(__VA_ARGS__)
#else
#define MSG(...)                             ((void)0)
#endif
//-------------------------------------------------------------------------------------------------
// Local declarations
void playerc_dio_putmsg(playerc_dio_t *device,
    player_msghdr_t *header,
    uint8_t *data,
    size_t len);
//-------------------------------------------------------------------------------------------------
// Create a new dio proxy
playerc_dio_t *playerc_dio_create(playerc_client_t *client, int index)
{
  playerc_dio_t *device;

  device = malloc(sizeof(playerc_dio_t));
  memset(device, 0, sizeof(playerc_dio_t));
  playerc_device_init(&device->info, client, PLAYER_DIO_CODE, index,
                      (playerc_putmsg_fn_t) playerc_dio_putmsg);

  return device;
}
//-------------------------------------------------------------------------------------------------
// Destroy a dio proxy
void playerc_dio_destroy(playerc_dio_t *device)
{
  playerc_device_term(&device->info);
  free(device);
}
//-------------------------------------------------------------------------------------------------
// Subscribe to the dio device
int playerc_dio_subscribe(playerc_dio_t *device, int access)
{
  return playerc_device_subscribe(&device->info, access);
}
//-------------------------------------------------------------------------------------------------
// Un-subscribe from the dio device
int playerc_dio_unsubscribe(playerc_dio_t *device)
{
  return playerc_device_unsubscribe(&device->info);
}
//-------------------------------------------------------------------------------------------------
// Process incoming data
void playerc_dio_putmsg(playerc_dio_t *device,
    player_msghdr_t *header,
    uint8_t *data,
    size_t len)
{
  if((header->type == PLAYER_MSGTYPE_DATA) &&
     (header->subtype == PLAYER_DIO_DATA_VALUES))
  {
    
	 player_dio_data_t *dioData = (player_dio_data_t *) data;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DIO_DATA_VALUES\n"); 

    device->data.count=dioData->count;
    device->data.bits=dioData->bits;
  }
  else if((header->type == PLAYER_MSGTYPE_DATA) &&
    (header->subtype == PLAYER_DIO_DATA_VALUES_ARRAY))
  {
    
    player_dio_data_array_t *dioData = (player_dio_data_array_t *) data;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DIO_VALUES_ARRAY\n");


    device->data_array.dio_count=dioData->dio_count;

    if ( device->data_array.dio!=0 )
      free ( device->data_array.dio );

    device->data_array.dio=malloc(sizeof(player_dio_data_t)*dioData->dio_count);
    ASSERT(device->data_array.dio);

    memcpy(device->data_array.dio, dioData->dio, sizeof(player_dio_data_t)*dioData->dio_count);
  }
}
//-------------------------------------------------------------------------------------------------
/* Set the output for the dio device. */
int playerc_dio_set_output(playerc_dio_t *device, uint8_t output_count, uint32_t bits)
{
  
  //player_dio_data_t cmd;
  player_dio_cmd_t cmd;
  TRACE("%s\n", __FUNCTION__);
  memset(&cmd, 0, sizeof(cmd));

  cmd.count = output_count;
  cmd.digout = bits;

  return playerc_client_write(device->info.client,
    &device->info, PLAYER_DIO_CMD_VALUES,&cmd,NULL);
}
//-------------------------------------------------------------------------------------------------
/* Set the output for the dio device. */
int playerc_dio_set_output_array(playerc_dio_t *device, uint8_t num, player_dio_cmd_t *dio)
{
  
  //player_dio_data_t cmd;
  player_dio_cmd_array_t cmd;
  int i;
  TRACE("%s\n", __FUNCTION__);


  cmd.dio_count=num;
  cmd.dio=dio;

  if (playerc_client_write(device->info.client, &device->info,
       PLAYER_DIO_CMD_VALUES_ARRAY,
       (void*)&cmd, NULL) < 0)
    return -1;

return 0;
}
//-------------------------------------------------------------------------------------------------
