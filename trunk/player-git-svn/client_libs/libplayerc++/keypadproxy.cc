#include "playerc++.h"

//-------------------------------------------------------------------------------------------------
using namespace PlayerCc;

//-------------------------------------------------------------------------------------------------
KeypadProxy::KeypadProxy(PlayerClient *aPc, uint32_t aIndex)
  : ClientProxy(aPc, aIndex),
  mDevice(NULL)
{
  Subscribe(aIndex);
  // how can I get this into the clientproxy.cc?
  // right now, we're dependent on knowing its device type
  mInfo = &(mDevice->info);
}
//-------------------------------------------------------------------------------------------------
KeypadProxy::~KeypadProxy()
{
  Unsubscribe();
}
//-------------------------------------------------------------------------------------------------
void
KeypadProxy::Subscribe(uint32_t aIndex)
{
  scoped_lock_t lock(mPc->mMutex);

  mDevice = keypad_create(mClient, aIndex);
  if (NULL==mDevice)
  {
    throw PlayerError("KeypadProxy::Subscribe()", "could not create");
  }

  if ( keypad_subscribe(mDevice, PLAYER_OPEN_MODE)!=0 )
  {
    throw PlayerError("KeypadProxy::Subscribe()", "could not subscribe");
  }
}
//-------------------------------------------------------------------------------------------------
void
KeypadProxy::Unsubscribe()
{
  assert(NULL!=mDevice);
  scoped_lock_t lock(mPc->mMutex);
  keypad_unsubscribe(mDevice);
  keypad_destroy(mDevice);
  mDevice = NULL;
}
//-------------------------------------------------------------------------------------------------
std::ostream&
std::operator << (std::ostream &os, const PlayerCc::KeypadProxy &c)
{
  os << "#KeypadProxy (" << c.GetInterface() << ":" << c.GetIndex() << ")" << std::endl;
  return os;
}
//-------------------------------------------------------------------------------------------------
void
KeypadProxy::GetData(uint32_t *timestamp, uint8_t *key, uint8_t *status)
{
  scoped_lock_t lock(mPc->mMutex);
  if ( keypad_get_data(mDevice, timestamp, key, status)!=0 )
    throw PlayerError("KeypadProxy::GetData()");
}
//-------------------------------------------------------------------------------------------------
void
KeypadProxy::GetConfig(uint32_t *de_bounce)
{
  scoped_lock_t lock(mPc->mMutex);
  if ( keypad_get_config(mDevice, de_bounce)!=0 )
    throw PlayerError("KeypadProxy::GetConfig()");
}
//-------------------------------------------------------------------------------------------------
void
KeypadProxy::SetConfig(uint32_t de_bounce)
{
  scoped_lock_t lock(mPc->mMutex);
  if ( keypad_set_config(mDevice, de_bounce)!=0 )
    throw PlayerError("KeypadProxy::SetConfig()");
}
//-------------------------------------------------------------------------------------------------



