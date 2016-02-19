/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2013-2014
 *     LeonLi
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
//-------------------------------------------------------------------------------------------------
#include "playerc++.h"

//-------------------------------------------------------------------------------------------------
using namespace PlayerCc;

//-------------------------------------------------------------------------------------------------
MotorProxy::MotorProxy(PlayerClient *aPc, uint32_t aIndex)
  : ClientProxy(aPc, aIndex),
  mDevice(NULL)
{
  Subscribe(aIndex);
  // how can I get this into the clientproxy.cc?
  // right now, we're dependent on knowing its device type
  mInfo = &(mDevice->info);
}
//-------------------------------------------------------------------------------------------------
MotorProxy::~MotorProxy()
{
  Unsubscribe();
}
//-------------------------------------------------------------------------------------------------
void
MotorProxy::Subscribe(uint32_t aIndex)
{
  scoped_lock_t lock(mPc->mMutex);
  mDevice = motor_create(mClient, aIndex);
  if (NULL==mDevice)
    throw PlayerError("MotorProxy::MotorProxy()", "could not create");

  if (0 != motor_subscribe(mDevice, PLAYER_OPEN_MODE))
    throw PlayerError("MotorProxy::MotorProxy()", "could not subscribe");
}
//-------------------------------------------------------------------------------------------------
void
MotorProxy::Unsubscribe()
{
  assert(NULL!=mDevice);
  scoped_lock_t lock(mPc->mMutex);
  motor_unsubscribe(mDevice);
  motor_destroy(mDevice);
  mDevice = NULL;
}
//-------------------------------------------------------------------------------------------------
void
MotorProxy::GetStatus (uint8_t *state, float *current, float *watts, float *rpm, float *position)
{
  scoped_lock_t lock(mPc->mMutex);
  if ( motor_get_status(mDevice, state, current, watts,rpm,position)!=0 )
    throw PlayerError("MotorProxy::GetStatus()");
}
//-------------------------------------------------------------------------------------------------
void
MotorProxy::SetPwm (float pwm)
{
  scoped_lock_t lock(mPc->mMutex);
  if ( motor_set_pwm(mDevice, pwm)!=0 )
    throw PlayerError("MotorProxy::SetPwm()");
}
//-------------------------------------------------------------------------------------------------
void
MotorProxy::SetVelocity (float velocity, float acceleration)
{
  scoped_lock_t lock(mPc->mMutex);
  if ( motor_set_velocity(mDevice, velocity, acceleration)!=0 )
    throw PlayerError("MotorProxy::SetVelocity()");
}
//-------------------------------------------------------------------------------------------------
void
MotorProxy::SetPosition (float velocity, float acceleration, float position)
{
  scoped_lock_t lock(mPc->mMutex);
  if ( motor_set_position(mDevice, velocity, acceleration, position)!=0 )
    throw PlayerError("MotorProxy::SetPosition()");
}
//-------------------------------------------------------------------------------------------------
void MotorProxy::SetDeltaPosition (float velocity, float acceleration, float deltaPosition)
{
  scoped_lock_t lock(mPc->mMutex);
  if ( motor_set_delta_position(mDevice, velocity, acceleration, deltaPosition)!=0 )
    throw PlayerError("MotorProxy::SetDeltaPosition()");
}
//-------------------------------------------------------------------------------------------------
std::ostream&
std::operator << (std::ostream &os, const PlayerCc::MotorProxy &c)
{
  os << "#Motor (" << c.GetInterface() << ":" << c.GetIndex() << ")" << std::endl;

  return os;
}
//-------------------------------------------------------------------------------------------------

