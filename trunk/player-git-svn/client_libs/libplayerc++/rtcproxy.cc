#include "playerc++.h"

//-------------------------------------------------------------------------------------------------
#define ASSERT_EN                                0
#define TRACE_EN                                 0
#define WARN_EN                                  0
#define ERROR_EN                                 0
#define FATAL_EN                                 0
#define DEBUG_EN                                 0
#define MSG_EN                                   0
//-------------------------------------------------------------------------------------------------
#if ASSERT_EN
#define ASSERT(expr)                        ((expr) ?  : printf( "ASSERT:", (char *)__FILE__, __LINE__, "\r\n"))
#else
#define ASSERT(...)                         ((void)0)
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
using namespace PlayerCc;

//-------------------------------------------------------------------------------------------------
RtcProxy::RtcProxy(PlayerClient *aPc, uint32_t aIndex)
  : ClientProxy(aPc, aIndex),
  mDevice(NULL)
{
  Subscribe(aIndex);
  // how can I get this into the clientproxy.cc?
  // right now, we're dependent on knowing its device type
  mInfo = &(mDevice->info);
}
//-------------------------------------------------------------------------------------------------
RtcProxy::~RtcProxy()
{
  Unsubscribe();
}
//-------------------------------------------------------------------------------------------------
void
RtcProxy::Subscribe(uint32_t aIndex)
{
  scoped_lock_t lock(mPc->mMutex);

  mDevice = rtc_create(mClient, aIndex);
  if (NULL==mDevice)
  {
    throw PlayerError("RtcProxy::Subscribe()", "could not create");
  }

  if (0 != rtc_subscribe(mDevice, PLAYER_OPEN_MODE))
  {
    throw PlayerError("RtcProxy::Subscribe()", "could not subscribe");
  }
}
//-------------------------------------------------------------------------------------------------
void
RtcProxy::Unsubscribe()
{
  assert(NULL!=mDevice);
  scoped_lock_t lock(mPc->mMutex);
  rtc_unsubscribe(mDevice);
  rtc_destroy(mDevice);
  mDevice = NULL;
}
//-------------------------------------------------------------------------------------------------
std::ostream&
std::operator << (std::ostream &os, const PlayerCc::RtcProxy &c)
{
  os << "#RtcProxy (" << c.GetInterface() << ":" << c.GetIndex() << ")" << std::endl;
  return os;
}
//-------------------------------------------------------------------------------------------------
void
RtcProxy::GetTime(uint32_t *utc_time, int32_t *minuteswest, int32_t *dsttime)
{
  TRACE("%s\r\n",__FUNCTION__);
  scoped_lock_t lock(mPc->mMutex);
  int32_t tmp_minuteswest;
  int32_t tmp_dsttime;


  if ( rtc_get_time(mDevice, utc_time, &tmp_minuteswest, &tmp_dsttime)!=0 )
    throw PlayerError("RtcProxy::GetTime()");

  if ( minuteswest!=NULL )
  	*minuteswest=tmp_minuteswest;
  if ( dsttime!=NULL )
  	*dsttime=tmp_dsttime;
}
//-------------------------------------------------------------------------------------------------
void
RtcProxy::SetTime( uint32_t utc_time, int32_t minuteswest, int32_t dsttime)
{
  TRACE("%s\r\n",__FUNCTION__);
  scoped_lock_t lock(mPc->mMutex);

  if ( rtc_set_time(mDevice, utc_time, minuteswest, dsttime)!=0 )
    throw PlayerError("RtcProxy::SetTime()");
}
//-------------------------------------------------------------------------------------------------
void
RtcProxy::GetAlarm(uint32_t *time)
{
  TRACE("%s\r\n",__FUNCTION__);
  scoped_lock_t lock(mPc->mMutex);

  if ( rtc_get_alarm(mDevice, time)!=0 )
    throw PlayerError("RtcProxy::GetAlarm()");
}
//-------------------------------------------------------------------------------------------------
void
RtcProxy::SetAlarm( uint32_t time)
{
  TRACE("%s\r\n",__FUNCTION__);
  scoped_lock_t lock(mPc->mMutex);

  if ( rtc_set_alarm(mDevice, time)!=0 )
    throw PlayerError("RtcProxy::SetAlarm()");
}
//-------------------------------------------------------------------------------------------------
//req_type
//0:CLEAR_ALARM
//1:KEEP_ALARM
bool
RtcProxy::IsOnAlarm(uint8_t req_type)
{
  TRACE("%s\r\n",__FUNCTION__);
  uint8_t isOnAlarm;

  scoped_lock_t lock(mPc->mMutex);

  if ( rtc_is_on_alarm(mDevice, req_type, &isOnAlarm)!=0 )
    throw PlayerError("RtcProxy::GetAlarm()");

  return isOnAlarm;
}
//-------------------------------------------------------------------------------------------------











