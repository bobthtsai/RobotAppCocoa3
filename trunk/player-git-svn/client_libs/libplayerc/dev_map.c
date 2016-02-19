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
 * Desc: Map device proxy
 * Author: Brian gerkey
 * Date: June 2004
 * CVS: $Id$
 **************************************************************************/

#include <config.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "playerc.h"
#include "error.h"
#include <zlib.h>
// #include <lz4.h>
#include <time.h>
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
#define TRACE(...)                           printf(__VA_ARGS__)
#else
#define TRACE(...)                           ((void)0)
#endif

#if WARN_EN
#define WARN(...)                            printf(__VA_ARGS__)
#else
#define WARN(...)                            ((void)0)
#endif

#if ERROR_EN
#define ERROR(...)                           printf(__VA_ARGS__)
#else
#define ERROR(...)                           ((void)0)
#endif

#if FATAL_EN
#define FATAL(...)                           printf(__VA_ARGS__)
#else
#define FATAL(...)                           ((void)0)
#endif

#if DEBUG_EN
#define DEBUG(...)                           printf(__VA_ARGS__)
#else
#define DEBUG(...)                           ((void)0)
#endif

#if MSG_EN
#define MSG(...)                             printf(__VA_ARGS__)
#else
#define MSG(...)                             ((void)0)
#endif
//-------------------------------------------------------------------------------------------------
#if defined (WIN32)
  #define snprintf _snprintf
#endif

#define DEBUG_TIME 0

//-------------------------------------------------------------------------------------------------
#if DEBUG_TIME
uint32_t TimeGet()
{
    struct  timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (time.tv_sec*1000+time.tv_nsec/1000000);  //unit: ms
}
//-------------------------------------------------------------------------------------------------
uint32_t DiffTime(uint32_t time1, uint32_t time0)
{
  return (time1-time0);
}
#endif
//-------------------------------------------------------------------------------------------------
// Create a new map proxy
playerc_map_t *playerc_map_create(playerc_client_t *client, int index)
{
  playerc_map_t *device;

  device = malloc(sizeof(playerc_map_t));
  memset(device, 0, sizeof(playerc_map_t));
  playerc_device_init(&device->info, client, PLAYER_MAP_CODE, index,
                      (playerc_putmsg_fn_t) NULL);

  // Debug.
  device->lock = 0;
  device->cells = NULL;
  device->segments = NULL;

  return device;
}
//-------------------------------------------------------------------------------------------------
// Destroy a map proxy
void playerc_map_destroy(playerc_map_t *device)
{
  playerc_device_term(&device->info);
  if (device->cells)
  free(device->cells);
  if ( device->segments )
  free(device->segments);
  free(device);
}
//-------------------------------------------------------------------------------------------------
// Subscribe to the map device
int playerc_map_subscribe(playerc_map_t *device, int access)
{
  return playerc_device_subscribe(&device->info, access);
}
//-------------------------------------------------------------------------------------------------
// Un-subscribe from the map device
int playerc_map_unsubscribe(playerc_map_t *device)
{
  return playerc_device_unsubscribe(&device->info);
}
//-------------------------------------------------------------------------------------------------
int playerc_map_get_map_info(playerc_map_t* device)
{
#if DEBUG_TIME
  uint32_t total = TimeGet();
#endif
    player_map_info_t *info_req = NULL;
  TRACE("%s\n", __FUNCTION__);

    if( device == NULL )
    {
        PLAYERC_ERR("null pointer for get_map_info");
        return (-1);
    }

    // first, get the map info
    if(playerc_client_request(device->info.client,
                              &device->info,
                              PLAYER_MAP_REQ_GET_INFO,
                              NULL, (void**)&info_req) < 0)
  {
      PLAYERC_ERR("failed to get map info");
      player_map_info_t_free(info_req);
      return(-1);
  }

    device->resolution = info_req->scale;
    device->width = info_req->width;
    device->height = info_req->height;
    device->origin[0] = info_req->origin.px;
    device->origin[1] = info_req->origin.py;
  device->lock = info_req->lock;

    player_map_info_t_free(info_req);

	#if DEBUG_TIME
  printf("%s cost %d ms\r\n", __FUNCTION__, DiffTime(TimeGet(), total));
#endif
    return (0);
}
//-------------------------------------------------------------------------------------------------
int playerc_map_get_partial_map(playerc_map_t* device,uint32_t col,uint32_t row,uint32_t width,uint32_t height, BOOL compress)
{
#if DEBUG_TIME
  uint32_t total = TimeGet();
#endif 

  TRACE("%s\n", __FUNCTION__);



  // first, get the map info.
  // Make sure there is a information for map.
  // And we use the information to check input arguments.
  //if already have map info don't need t request map info again
  if ( device->height==0 && device->width==0 )
  {
  if( playerc_map_get_map_info(device) < 0 )
  {
    PLAYERC_ERR("failed to get partial map");
    return (-1);
  }
  }
  // Allocate space for the whole map
  if ( device->cells==NULL )
  {
    TRACE("device->cells = (char*)malloc\n");
    device->cells = (char*)malloc(sizeof(char)*device->width*device->height); // ?�張?��??�大�?
    assert(device->cells);
  }

  //check data first.
  if( ( col+width ) > device->width
   || ( row+height) > device->height )
  {
    PLAYERC_ERR("playerc_map_get_partial_map input is out of range.");
    ERROR("wrong:%lu %lu %lu %lu\n",col,row,width,height);
    return (-1);
  }




  if (compress)
  {
    int i,j;
    uLongf unzipped_data_len= width * height; //BOBMODIFY 20151229
    char *buffer = (char *)malloc(sizeof(char)*unzipped_data_len);
    player_map_data_t data_req;
    player_map_data_t *data_resp = NULL;
    


    data_req.col = col;
    data_req.row = row;
    data_req.width = width;
    data_req.height = height;
    data_req.data_count=0;
    data_req.data=NULL;
    if(playerc_client_request(device->info.client, &device->info,
                              PLAYER_MAP_REQ_GET_COMPRESSED_DATA,
                              (void*)&data_req, (void**)&data_resp) < 0)
    {
      PLAYERC_ERR("failed to get map data");
      player_map_data_t_free(data_resp);
      free(buffer);

      return(-1);
    }

    //this time request data have some problem
    if ( width*height<data_resp->data_count )
    {
      PLAYERC_ERR1("data_resp->data_count = %d\n",data_resp->data_count);
      player_map_data_t_free(data_resp);
      free(buffer);

      return (-1);
    }


    
    //zlib
    if(uncompress((Bytef*)buffer, &unzipped_data_len,
                  (uint8_t*)data_resp->data, data_resp->data_count) != Z_OK)
    
    //lz4
    // if (LZ4_decompress_fast((char*)data_resp->data,(char*)buffer,unzipped_data_len)<0)
    {
      PLAYERC_ERR("failed to decompress map data");
      player_map_data_t_free(data_resp);
      free(buffer);
      
      return(-1);
    }
    assert( unzipped_data_len==(width*height) );


    device->data_range = data_resp->data_range;
    //checkout the partition data to big map data.
    /// Noted, the size of data_resp->data might be smaller than device->cells
    for( j=0; j<height; ++j )
    {
      for( i=0; i<width; ++i )
      {
        device->cells[(j+row)*device->width+(i+col)] = buffer[j*height+i];
      }
    }

    player_map_data_t_free(data_resp);
    free(buffer);
  } // end of compress
  else
  {
    int i,j;
    player_map_data_t data_req;
    player_map_data_t *data_resp = NULL;


    data_req.col = col;
    data_req.row = row;
    data_req.width = width;
    data_req.height = height;
    data_req.data_count=0;
    data_req.data=NULL;
    if (playerc_client_request(device->info.client, &device->info,
                              PLAYER_MAP_REQ_GET_DATA,
                              (void*)&data_req, (void**)&data_resp) < 0)
    {
      PLAYERC_ERR("failed to get map data");
      player_map_data_t_free(data_resp);

      return(-1);
    }
    device->data_range = data_resp->data_range;
    /// Noted, the size of data_resp->data might be smaller than device->cells
    for( j=0; j<height; ++j )
    {
      for( i=0; i<width; ++i )
      {
        device->cells[(j+row)*device->width+(i+col)] = data_resp->data[j*height+i];
      }
    }

  player_map_data_t_free(data_resp);
  }
  
  


#if DEBUG_TIME
  printf("(%d) %s cost %d ms\r\n", compress, __FUNCTION__, DiffTime(TimeGet(), total));
#endif
  return(0);
}
//-------------------------------------------------------------------------------------------------
int playerc_map_get_map(playerc_map_t* device)
{
#if DEBUG_TIME
  uint32_t total = TimeGet();
#endif 

  player_map_data_t data_req;
  player_map_data_t *data_resp=NULL;

  int i,j;
  int oi,oj;
  int sx,sy;
  int si,sj;
  char* cell;
  TRACE("%s\n", __FUNCTION__);

  //if already have map info don't need t request map info again
  if ( device->height==0 && device->width==0 )
  {
  if( playerc_map_get_map_info(device) < 0 )
  {
    PLAYERC_ERR("failed to get map");
    return (-1);
  }
  }

  // Allocate space for the whole map
  if ( device->cells==NULL )
  {
    device->cells = (char*)malloc(sizeof(char) * device->width * device->height);
  assert(device->cells);
  }

  // Tile size
  sy = sx = 640;
  oi=oj=0;

  while((oi < device->width) && (oj < device->height))
  {
    si = MIN(sx, device->width - oi);
    sj = MIN(sy, device->height - oj);

    memset(&data_req,0,sizeof(data_req));
    data_req.col = oi;
    data_req.row = oj;
    data_req.width = si;
    data_req.height = sj;

    if(playerc_client_request(device->info.client, &device->info,
                              PLAYER_MAP_REQ_GET_DATA,
                              (void*)&data_req, (void**)&data_resp) < 0)
    {
      PLAYERC_ERR("failed to get map data");
      player_map_data_t_free(data_resp);

      data_resp = NULL;
      return(-1);
    }

    device->data_range = data_resp->data_range;

    // copy the map data
    for(j=0;j<sj;j++)
    {
      for(i=0;i<si;i++)
      {
        cell = device->cells + PLAYERC_MAP_INDEX(device,oi+i,oj+j);
        *cell = data_resp->data[j*si + i];
      }
    }

    oi += si;
    if(oi >= device->width)
    {
      oi = 0;
      oj += sj;
    }

    player_map_data_t_free(data_resp);
  }


#if DEBUG_TIME
  printf("%s cost %d ms\r\n", __FUNCTION__, DiffTime(TimeGet(), total));
#endif
  return(0);
}
//-------------------------------------------------------------------------------------------------
int playerc_map_get_mapp(playerc_map_t* device)
{
  player_map_data_t data_req;
  player_map_data_t *data_resp = NULL;
  char* cell;
  uLongf unzipped_data_len;
  TRACE("%s\n", __FUNCTION__);

  //if already have map info don't need t request map info again
  if ( device->height==0 && device->width==0 )
  {
    if( playerc_map_get_map_info(device) < 0 )
    {
      PLAYERC_ERR("failed to get map");
      return (-1);
    }
  }

  // Allocate space for the whole map
  if ( device->cells==NULL )
  {
    device->cells = (char*)malloc(sizeof(char) * device->width * device->height);
  assert(device->cells);
  }


  memset(&data_req,0,sizeof(data_req));
  data_req.col = 0;
  data_req.row = 0;
  data_req.width = device->width;
  data_req.height = device->height;

  unzipped_data_len = device->width*device->height;

//  DEBUG("data_req->col :%d\n",data_req->col);
//  DEBUG("data_req->row :%d\n",data_req->row);
//  DEBUG("data_req->width :%d\n",device->width);
//  DEBUG("data_req->height :%d\n",device->height);

  if(playerc_client_request(device->info.client, &device->info,
                            PLAYER_MAP_REQ_GET_COMPRESSED_DATA,
                            (void*)&data_req, (void**)&data_resp) < 0)
  {
    PLAYERC_ERR("failed to get map data");
    player_map_data_t_free(data_resp);

    return(-1);
  }

  if(uncompress((Bytef*)device->cells, &unzipped_data_len,
                (uint8_t*)data_resp->data, data_resp->data_count) != Z_OK)
  {
    PLAYERC_ERR("failed to decompress map data");
    player_map_data_t_free(data_resp);

    return(-1);
  }

  device->data_range = data_resp->data_range;

#if (1)//FOR_PYTHON_SIMULATOR
  { //BOBMODIFY 20151229
	  char *cellp;
	  int j;
	  cellp = device->cells;
	  for(j=0;j<unzipped_data_len;j++,cellp++)
	  {
		  *cellp = *cellp + '1';
	  }
  }
#endif


  player_map_data_t_free(data_resp);


  return(0);
}
//-------------------------------------------------------------------------------------------------
int
playerc_map_get_vector(playerc_map_t* device)
{
  player_map_data_vector_t* vmap;
  TRACE("%s\n", __FUNCTION__);

  if(playerc_client_request(device->info.client,
                            &device->info,
                            PLAYER_MAP_REQ_GET_VECTOR,
                            NULL, (void**)&vmap) < 0)
  {
    PLAYERC_ERR("failed to get map vector data");
    return(-1);
  }

  // Copy meta-data
  device->vminx = vmap->minx;
  device->vminy = vmap->miny;
  device->vmaxx = vmap->maxx;
  device->vmaxy = vmap->maxy;
  device->num_segments = vmap->segments_count;

  // Allocate space into which we'll copy the segments.
  if(device->segments)
    free(device->segments);
  device->segments = (player_segment_t*)malloc(device->num_segments *
                                               sizeof(player_segment_t));
  assert(device->segments);
  memcpy(device->segments,
         vmap->segments,
         device->num_segments*sizeof(player_segment_t));
  player_map_data_vector_t_free(vmap);
  return(0);
}
//-------------------------------------------------------------------------------------------------
int
playerc_map_set_partial_map(playerc_map_t *device,char *map_data,uint32_t col,uint32_t row,uint32_t width,uint32_t height)
{
  int  i,j,k;
  player_map_data_t map;

  uLongf partial_map_size = width*height;
  uLongf compressed_data_size = compressBound(partial_map_size);
  TRACE("%s\n", __FUNCTION__);

  // malloc memory
  //check data first.
  if( ( col+width ) > device->width
   || ( row+height) > device->height )
  {
    PLAYERC_ERR("playerc_map_set_partial_map input is out of range.");
    ERROR("wrong:%lu %lu %lu %lu\n",col,row,width,height);
    return (-1);
  }

  //copy local data.
  k=0;
  for(i=0;i<height;++i)
  {
      for(j=0;j<width;++j)
      {
          device->cells[(i+row)*width+(j+col)] = map_data[k];
          ++k;
      }
  }

  map.col = col;
  map.row = row;
  map.width = width;
  map.height = height;
  map.data = (char *)malloc( sizeof(char)*compressed_data_size );
  assert(map.data!=NULL);

  //compress data.
  if( compress((Bytef *)map.data, &compressed_data_size,(char *)map_data, partial_map_size) != Z_OK )
  {
    PLAYERC_ERR("compress map data error");
    free(map.data);

    return(-1);
  }

  map.data_count = compressed_data_size;

  if(playerc_client_request(device->info.client,
                            &device->info,
                            PLAYER_MAP_REQ_SET_COMPRESSED_DATA,
                            (void *)&map, (void**)NULL) < 0)
  {
    free(map.data);

    PLAYERC_ERR("set compressed map request fail");
    return(-1);
  }

  free(map.data);

  return(0);
}
//-------------------------------------------------------------------------------------------------
int playerc_map_set_map(playerc_map_t *device,char *map_data)
{
  player_map_data_t map;
  int i;

  // Backup lock signal
  int8_t bakcup_lock = device->lock;
  TRACE("%s\n", __FUNCTION__);

  // allocate memory

  map.col = 0;
  map.row = 0;
  map.width = device->width;
  map.height = device->height;
  map.data_count = device->width * device->height;
  map.data = (char *)malloc(map.data_count);// allocate memory
  assert(map.data!=NULL);

  for(i=0;i<map.data_count;++i)
  {
    device->cells[i]=map_data[i];
    map.data[i]=map_data[i];
  }


  if(playerc_client_request(device->info.client,
                            &device->info,
                            PLAYER_MAP_REQ_SET_DATA,
                            (void *)&map, (void**)NULL) < 0)
  {
    PLAYERC_ERR("send map request fail");
    free(map.data);

    return(-1);
  }

  device->lock = bakcup_lock;
  free(map.data);

  return(0);
}
//-------------------------------------------------------------------------------------------------
//lock:1 lock lock:0 unlock
int playerc_map_save(playerc_map_t *device)
{
  TRACE("%s\n", __FUNCTION__);
  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_MAP_CMD_SAVE,
                              NULL, NULL);
}
//-------------------------------------------------------------------------------------------------
//lock:1 lock lock:0 unlock
int playerc_map_load(playerc_map_t *device)
 {
  TRACE("%s\n", __FUNCTION__);
   return playerc_client_write(device->info.client, &device->info,
                              PLAYER_MAP_CMD_LOAD,
                              NULL, NULL);
}
//-------------------------------------------------------------------------------------------------
int playerc_map_set_map_info(playerc_map_t *device, player_map_info_t *info_req)
{
  TRACE("%s\n", __FUNCTION__);
  if( playerc_client_write(device->info.client, &device->info,
                           PLAYER_MAP_CMD_SET_INFO,(void *)info_req, NULL) < 0 )
  {
    PLAYERC_ERR("client write fail");
    return (-1);
  }

  if ( device->width != info_req->width || device->height != info_req->height)
  {
    // Allocate space for the whole map
    device->cells = (char*)realloc(device->cells, sizeof(char) *
                                   device->width * device->height);
    assert(device->cells);
  }

  if( playerc_map_get_map_info(device) < 0 )
  {
    PLAYERC_ERR("failed to set map info");
    return (-1);
  }

  return (0);
}
//-------------------------------------------------------------------------------------------------
