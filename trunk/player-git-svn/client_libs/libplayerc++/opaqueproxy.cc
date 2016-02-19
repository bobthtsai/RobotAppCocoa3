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

#include "config.h"

#include <cstring>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <cstring>

#include "playerc++.h"
#include "debug.h"

using namespace PlayerCc;

OpaqueProxy::OpaqueProxy(PlayerClient *aPc, uint32_t aIndex)
  : ClientProxy(aPc, aIndex),
  mDevice(NULL)
{
  Subscribe(aIndex);
  // how can I get this into the clientproxy.cc?
  // right now, we're dependent on knowing its device type
  mInfo = &(mDevice->info);
}

OpaqueProxy::~OpaqueProxy()
{
  Unsubscribe();
}

void
OpaqueProxy::Subscribe(uint32_t aIndex)
{
  scoped_lock_t lock(mPc->mMutex);
  mDevice = playerc_opaque_create(mClient, aIndex);

  if (NULL==mDevice)
    throw PlayerError("OpaqueProxy::OpaqueProxy()", "could not create");

  if (0 != playerc_opaque_subscribe(mDevice, PLAYER_OPEN_MODE))
    throw PlayerError("OpaqueProxy::OpaqueProxy()", "could not subscribe");
}

void
OpaqueProxy::SendCmd(player_opaque_data_t* aData)
{
  scoped_lock_t lock(mPc->mMutex);
  playerc_opaque_cmd(mDevice, aData);
}

int
OpaqueProxy::SendReq(player_opaque_data_t* aRequest)
{
  scoped_lock_t lock(mPc->mMutex);
  player_opaque_data_t *aReply;
//printf("dd01 %d   %d\n",mDevice,aRequest);
  int result = playerc_opaque_req(mDevice, aRequest, &aReply);
//printf("dd02 %d   %d\n",mDevice,aRequest);
//printf("dd02__ %d  %d\n",aReply->data_count,aRequest->data_count);
//printf("dd02__ %s  %s\n",aReply->data,aRequest->data);
//printf("mDevice->data_count=%d %d",mDevice->data_count,mDevice->data);

  if (result == 0)
  {

	if (mDevice->data_count==0){
		mDevice->data=0;
		mDevice->data=(uint8_t*)malloc(sizeof(*aReply->data)*aReply->data_count);
	}else{
		free(mDevice->data);
		mDevice->data=(uint8_t*)malloc(sizeof(*aReply->data)*aReply->data_count);
	}

//printf("dd03\n");
//??????
//may be new for mDevice->data will be ok.
    memcpy(mDevice->data, aReply->data, aReply->data_count);
//printf("dd04\n");
    mDevice->data_count = aReply->data_count;


	if (aRequest->data_count==0){
		aRequest->data=0;
		aRequest->data=(uint8_t*)malloc(sizeof(*aReply->data)*aReply->data_count);
	}else{
		free(aRequest->data);
		aRequest->data=(uint8_t*)malloc(sizeof(*aReply->data)*aReply->data_count);
	}
    memcpy(aRequest->data, aReply->data, aReply->data_count);
    aRequest->data_count = aReply->data_count;
  }
//printf("dd05\n");
  player_opaque_data_t_free(aReply);
//printf("dd06\n");
  return result;
}

void
OpaqueProxy::Unsubscribe()
{
  assert(NULL!=mDevice);
  scoped_lock_t lock(mPc->mMutex);
  playerc_opaque_unsubscribe(mDevice);
  playerc_opaque_destroy(mDevice);
  mDevice = NULL;
}

std::ostream& std::operator << (std::ostream& os, const PlayerCc::OpaqueProxy& c)
{
	os << "Count is: "<< c.GetCount() << "Data:" << endl;
	uint8_t * data;
	data = new uint8_t[4096];
	c.GetData(data);
	for(unsigned int i = 0; i < c.GetCount(); i++)
	{
		os << hex << setw(2) << setfill('0') << static_cast<unsigned int> (data[i]);
	}
  return os;
}
