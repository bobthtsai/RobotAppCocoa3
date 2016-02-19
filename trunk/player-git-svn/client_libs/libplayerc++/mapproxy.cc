/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2000-2003
 *     Brian Gerkey, Kasper Stoy, Richard Vaughan, & Andrew Howard
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */
/********************************************************************
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
 *
 ********************************************************************/

/*
 * $Id$
 */
//-------------------------------------------------------------------------------------------------
#include "playerc++.h"

//-------------------------------------------------------------------------------------------------
using namespace PlayerCc;

//-------------------------------------------------------------------------------------------------
MapProxy::MapProxy(PlayerClient *aPc, uint32_t aIndex)
  : ClientProxy(aPc, aIndex),
  mDevice(NULL)
{
  Subscribe(aIndex);
  // how can I get this into the clientproxy.cc?
  // right now, we're dependent on knowing its device type
  mInfo = &(mDevice->info);
}
//-------------------------------------------------------------------------------------------------
MapProxy::~MapProxy()
{
  Unsubscribe();
}
//-------------------------------------------------------------------------------------------------
void
MapProxy::Subscribe(uint32_t aIndex)
{
  scoped_lock_t lock(mPc->mMutex);
  mDevice = playerc_map_create(mClient, aIndex);
  if (NULL==mDevice)
    throw PlayerError("MapProxy::MapProxy()", "could not create");

  if (0 != playerc_map_subscribe(mDevice, PLAYER_OPEN_MODE))
    throw PlayerError("MapProxy::MapProxy()", "could not subscribe");
}
//-------------------------------------------------------------------------------------------------
void
MapProxy::Unsubscribe()
{
  assert(NULL!=mDevice);
  scoped_lock_t lock(mPc->mMutex);
  playerc_map_unsubscribe(mDevice);
  playerc_map_destroy(mDevice);
  mDevice = NULL;
}
//-------------------------------------------------------------------------------------------------
std::ostream&
std::operator << (std::ostream &os, const PlayerCc::MapProxy &c)
{
  os << "#Map (" << c.GetInterface() << ":" << c.GetIndex() << ")" << std::endl;

  return os;
}
//-------------------------------------------------------------------------------------------------
int
MapProxy::RequestMap(uint32_t orgin_col,uint32_t orgin_row,uint32_t map_width,uint32_t map_height, bool compress)
{
  scoped_lock_t lock(mPc->mMutex);

  return playerc_map_get_partial_map((mDevice),orgin_col,orgin_row,map_width,map_height, compress);
}
//-------------------------------------------------------------------------------------------------
int
MapProxy::RequestMap()
{
  scoped_lock_t lock(mPc->mMutex);

  return playerc_map_get_map(mDevice);
}
//-------------------------------------------------------------------------------------------------
int
MapProxy::SetWholeMap(int8_t *map_data)
{
  scoped_lock_t lock(mPc->mMutex);

  return playerc_map_set_map((mDevice),(char *)map_data);
}
//-------------------------------------------------------------------------------------------------
int
MapProxy::SetPartialMap(int8_t *map_data,uint32_t orgin_col,uint32_t orgin_row,uint32_t map_width,uint32_t map_height)
{
  scoped_lock_t lock(mPc->mMutex);

  return playerc_map_set_partial_map((mDevice),(char *)map_data,orgin_col,orgin_row,map_width,map_height);
}
//-------------------------------------------------------------------------------------------------
void
MapProxy::Clean()
{
  scoped_lock_t lock(mPc->mMutex);
  player_map_info_t info_req,info_backup;

  // backup all data.
  info_backup.scale  = mDevice->resolution ;
  info_backup.width  = mDevice->width;
  info_backup.height = mDevice->height;
  info_backup.origin.px = mDevice->origin[0];
  info_backup.origin.py = mDevice->origin[1];
  info_backup.lock   = mDevice->lock;

  // set all parameters zero to clean data.
  memset( (void *)&info_req,0,sizeof(player_map_info_t) );

  // clean map.
  if ( 0 != playerc_map_set_map_info((mDevice),&info_req) )
    throw PlayerError("MapProxy::CleanMap()", "error CleanMap");

  // recover all information.
  if ( 0 != playerc_map_set_map_info((mDevice),&info_backup) )
    throw PlayerError("MapProxy::CleanMap()", "error CleanMap");
}
//-------------------------------------------------------------------------------------------------
void
MapProxy::Save()
{
  scoped_lock_t lock(mPc->mMutex);
  if ( 0 != playerc_map_save((mDevice)) )
    throw PlayerError("MapProxy::Save()");
}
//-------------------------------------------------------------------------------------------------
void
MapProxy::Load()
{
  scoped_lock_t lock(mPc->mMutex);
  if ( 0 != playerc_map_load((mDevice)) )
    throw PlayerError("MapProxy::Load()");
}
//-------------------------------------------------------------------------------------------------
int
MapProxy::SetMapInfo(float resolution,int width,int height,double origin_x,double origin_y)
{
  player_map_info_t info_req;
  scoped_lock_t lock(mPc->mMutex);

  info_req.scale = resolution;
  info_req.width = width;
  info_req.height = height;
  info_req.origin.px = origin_x;
  info_req.origin.py = origin_y;
  info_req.lock = mDevice->lock;


  return playerc_map_set_map_info((mDevice),&info_req);
}
//-------------------------------------------------------------------------------------------------
void
MapProxy::GetMapInfo(player_map_info_t *info)
{
  if ( mDevice->width==0 && mDevice->height==0 )
  {
    scoped_lock_t lock(mPc->mMutex);
    playerc_map_get_map_info(mDevice);
  }

  info->scale = mDevice->resolution;
  info->width = mDevice->width;
  info->height = mDevice->height;
  info->origin.px = mDevice->origin[0];
  info->origin.py = mDevice->origin[1];
  info->lock = mDevice->lock;
} 
//-------------------------------------------------------------------------------------------------
void 
MapProxy::SetLock(bool map_lock)
{
  player_map_info_t info_req;
  scoped_lock_t lock(mPc->mMutex);

  info_req.scale = mDevice->resolution;
  info_req.width = mDevice->width;
  info_req.height = mDevice->height;
  info_req.origin.px = mDevice->origin[0];
  info_req.origin.py = mDevice->origin[1];
  info_req.lock = map_lock;

  if( 0 != playerc_map_set_map_info((mDevice),&info_req) )
    throw PlayerError("MapProxy::SetLock()");
}
//-------------------------------------------------------------------------------------------------
