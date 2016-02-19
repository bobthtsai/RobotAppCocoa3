#include "playerc++.h"
#ifdef WIN32
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include "replace.h"
#else
#include <sys/time.h>
#endif
#ifdef CLOCKGETTIME_REPLACE_GETTIMEOFDAY
//	#include <utils/gettimereplace/gettimereplace.h>
	#include <replace/gettimereplace/gettimereplace.h>
	#define gettimeofday clockgettime_replace_gettimeofday
#endif

#define TIME_OUT                                 30000 //unit:ms
//-------------------------------------------------------------------------------------------------
using namespace PlayerCc;

//-------------------------------------------------------------------------------------------------
DispatcherProxy::DispatcherProxy(PlayerClient *aPc, uint32_t aIndex)
  : ClientProxy(aPc, aIndex),
  mDevice(NULL)
{
  Subscribe(aIndex);
  // how can I get this into the clientproxy.cc?
  // right now, we're dependent on knowing its device type
  mInfo = &(mDevice->info);
}
//-------------------------------------------------------------------------------------------------
DispatcherProxy::~DispatcherProxy()
{
  Unsubscribe();
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::Subscribe(uint32_t aIndex)
{
  scoped_lock_t lock(mPc->mMutex);

  mDevice = dispatcher_create(mClient, aIndex);
  if (NULL==mDevice)
  { 
    throw PlayerError("DispatcherProxy::Subscribe()", "could not create");
  }

  if (0 != dispatcher_subscribe(mDevice, PLAYER_OPEN_MODE))
  {
    throw PlayerError("DispatcherProxy::Subscribe()", "could not subscribe");
  }
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::Unsubscribe()
{
  assert(NULL!=mDevice);
  scoped_lock_t lock(mPc->mMutex);
  dispatcher_unsubscribe(mDevice);
  dispatcher_destroy(mDevice);
  mDevice = NULL;
}
//-------------------------------------------------------------------------------------------------
std::ostream&
std::operator << (std::ostream &os, const PlayerCc::DispatcherProxy &c)
{
  os << "#DispatcherProxy (" << c.GetInterface() << ":" << c.GetIndex() << ")" << std::endl;
  return os;
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::SetAiCmd(uint8_t initiator,
    uint32_t cmd,
    uint32_t power,
    uint32_t map_index_count,
    uint32_t *map_index,
    player_pose2d_t *target_point,
    uint32_t name_count,
    int8_t *name,
    uint32_t data_count,
    int8_t *data,
    uint32_t stop_cond)
{
  scoped_lock_t lock(mPc->mMutex);
  player_pose2d_t targetPoint;


  targetPoint.px=0.0;
  targetPoint.py=0.0;
  targetPoint.pa=0.0;
  if ( target_point!=NULL )
    targetPoint=*target_point;

  if ( dispatcher_set_cmd_ai(mDevice, initiator, cmd, power, map_index_count, map_index, targetPoint, name_count, name, data_count, data, stop_cond)!=0 )
    throw PlayerError("DispatcherProxy::SetAiCmd()");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::SetPmCmd(uint8_t initiator, uint32_t cmd)
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_set_cmd_pm(mDevice, initiator, cmd)!=0 )
    throw PlayerError("DispatcherProxy::SetPmCmd()");
}
//-------------------------------------------------------------------------------------------------

void
DispatcherProxy::SetAiStatus(uint8_t initiator,
    uint32_t aiStatus,
    uint32_t power,
    uint32_t error,
    uint32_t worry )
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_set_status_ai(mDevice, initiator, aiStatus, power, error, worry)!=0 )
    throw PlayerError("DispatcherProxy::SetAiStatus()");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::SetPmStatus(uint8_t initiator,
    uint32_t pmStatus,
    uint32_t error)
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_set_status_pm(mDevice, initiator, pmStatus, error)!=0 )
    throw PlayerError("DispatcherProxy::SetPmStatus()");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::SetUiStatus(uint8_t initiator,
    uint32_t uiStatus)
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_set_status_ui(mDevice, initiator, uiStatus)!=0 )
    throw PlayerError("DispatcherProxy::SetUiStatus()");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::GetAiCmd(uint8_t *initiator,
    uint32_t *cmd,
    uint32_t *power,
    player_pose2d_t *targetPoint,
    uint32_t *stop_cond)
{
  scoped_lock_t lock(mPc->mMutex);
  mDevice->isSetAiCmd=FALSE;
  *initiator=mDevice->aiCmdData.init_des.initiator;
  *cmd=mDevice->aiCmdData.job.cmd;
  *power=mDevice->aiCmdData.job.power;
  *targetPoint=mDevice->aiCmdData.job.target_point;
  *stop_cond=mDevice->aiCmdData.job.stop_cond;
}
//-------------------------------------------------------------------------------------------------
uint32_t *
DispatcherProxy::GetAiMapLayer(uint32_t *map_index_count)
{
  *map_index_count=mDevice->aiCmdData.job.map_index_count;
  return mDevice->aiCmdData.job.map_index;
}
//-------------------------------------------------------------------------------------------------
int8_t *
DispatcherProxy::GetAiData(uint32_t *data_count)
{
  *data_count=mDevice->aiCmdData.job.data_count;
  return mDevice->aiCmdData.job.data;
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::GetPmCmd(uint8_t *initiator, uint32_t *cmd)
{
  scoped_lock_t lock(mPc->mMutex);
  mDevice->isSetPmCmd=FALSE;
  *initiator=mDevice->pmCmdData.init_des.initiator;
  *cmd=mDevice->pmCmdData.cmd;
}
//-------------------------------------------------------------------------------------------------

void
DispatcherProxy::GetAiStatus(uint8_t initiator,
    uint32_t *aiStatus,
    uint32_t *cleanStatus,
    uint32_t *error,
    uint32_t *worry)
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_get_status_ai(mDevice, initiator, aiStatus, cleanStatus, error, worry)!=0 )
    throw PlayerError("DispatcherProxy::GetAiStatus()");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::GetPmStatus(uint8_t initiator,
    uint32_t *pmStatus,
    uint32_t *error)
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_get_status_pm(mDevice, initiator, pmStatus, error)!=0 )
    throw PlayerError("DispatcherProxy::GetAiStatus()");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::GetUiStatus(uint8_t initiator,
    uint32_t *uiStatus)
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_get_status_ui(mDevice, initiator, uiStatus)!=0 )
    throw PlayerError("DispatcherProxy::GetUiStatus()");
}
//-------------------------------------------------------------------------------------------------
bool
DispatcherProxy::IsSetUiTime()
{
  if ( mDevice->isSetUiTime==TRUE )
  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isSetUiTime=FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
//-------------------------------------------------------------------------------------------------
bool
DispatcherProxy::IsSetUiSchedule()
{
  if ( mDevice->isSetUiSchedule==TRUE )
  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isSetUiSchedule=FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
//-------------------------------------------------------------------------------------------------
bool
DispatcherProxy::IsReqUiTime()
{
  if ( mDevice->isReqUiTime==TRUE )
  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isReqUiTime=FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
//-------------------------------------------------------------------------------------------------
bool
DispatcherProxy::IsReqUiSchedule()
{
  if ( mDevice->isReqUiSchedule==TRUE )
  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isReqUiSchedule=FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
//-------------------------------------------------------------------------------------------------
//check Ok by Leon @2014/01/08
void
DispatcherProxy::SetUiTime(uint8_t initiator,
    player_dispatcher_time_t time,
    player_dispatcher_timezone_t zone)
{
  scoped_lock_t lock(mPc->mMutex);

  if (  dispatcher_set_ui_time(mDevice, initiator, time, zone)!=0 )
    throw PlayerError("DispatcherProxy::SetUiTime()");
}
//-------------------------------------------------------------------------------------------------
//check Ok by Leon @2014/01/08
void
DispatcherProxy::SetUiSchedule(uint8_t initiator, uint32_t count, player_dispatcher_ui_alarm_time_t *pAlarm)
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_set_ui_schedule(mDevice, initiator, count, pAlarm)!=0 )
    throw PlayerError("DispatcherProxy::SetUiSchedule()");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::GetUiTime(uint8_t *initiator,
    player_dispatcher_time_t *time,
    player_dispatcher_timezone_t *zone)
{
  if ( dispatcher_get_ui_time(mDevice, initiator, time, zone)!=0 )
    throw PlayerError("DispatcherProxy::GetUiTime()");
}
//-------------------------------------------------------------------------------------------------
player_dispatcher_ui_alarm_time_t *
DispatcherProxy::GetUiSchedule(uint8_t *initiator, uint32_t *count)
{
  return dispatcher_get_ui_schedule(mDevice, initiator, count);
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::ReqUiTime(uint8_t initiator,
    player_dispatcher_time_t *time,
    player_dispatcher_timezone_t *zone )
{
  uint32_t startTime=0;
  struct timeval timeVal, timeVal2;

  {
    scoped_lock_t lock(mPc->mMutex);
    player_dispatcher_ui_time_t uiTimeCmd;

    uiTimeCmd.init_des.initiator=initiator;
    uiTimeCmd.init_des.destination=PLAYER_DISPATCH_INIT_DEST_UI;


    //send command to request UI time data
    playerc_client_write(mDevice->info.client, &mDevice->info,
                                PLAYER_DISPATCHER_CMD_REQ_UI_TIME,
                                &uiTimeCmd, NULL);
  }


  gettimeofday(&timeVal, 0);
  startTime=(uint32_t)( (uint32_t)timeVal.tv_sec*1000+ (uint32_t)timeVal.tv_usec/1000);
  while( mDevice->isReqAckUiTime==FALSE  )
  {
    uint32_t curentTime;


    gettimeofday(&timeVal2, 0);
    curentTime=(uint32_t)( (uint32_t)timeVal2.tv_sec*1000+ (uint32_t)timeVal2.tv_usec/1000);
    if ( (curentTime-startTime)>TIME_OUT )
    {
      scoped_lock_t lock(mPc->mMutex);
      mDevice->isReqAckUiTime=FALSE;
      throw PlayerError("DispatcherProxy::ReqUiTime::TIME_OUT");
    }

    usleep(1000);
  }

  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isReqAckUiTime=FALSE;
    *time=mDevice->uiTime.data;
    *zone=mDevice->uiTime.zone;
  }
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy:: GetReqUiTime(uint8_t *initiator)
{
  if ( dispatcher_get_req_ui_time(mDevice, initiator) )
    throw PlayerError("DispatcherProxy::GetReqUiTime");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::SetReqAckUiTime( uint8_t destination,
    player_dispatcher_time_t time,
    player_dispatcher_timezone_t zone)
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_set_req_ack_ui_time(mDevice, destination, time, zone) )
    throw PlayerError("DispatcherProxy::GetReqAckUiTime");
}
//-------------------------------------------------------------------------------------------------
player_dispatcher_ui_alarm_time_t *
DispatcherProxy::ReqUiSchedule(uint8_t initiator, uint32_t *count)
{
  player_dispatcher_ui_alarm_time_t *alarmTime=NULL;
  uint32_t startTime=0;
  struct timeval timeVal;


  {
    scoped_lock_t lock(mPc->mMutex);
    player_dispatcher_ui_schedule_t uiScheduleCmd={};


    uiScheduleCmd.init_des.initiator=initiator;
    uiScheduleCmd.init_des.destination=PLAYER_DISPATCH_INIT_DEST_UI;
    uiScheduleCmd.data_count=0;
    uiScheduleCmd.data=NULL;

    //send command to request UI schedule data
    if ( playerc_client_write(mDevice->info.client, &mDevice->info, PLAYER_DISPATCHER_CMD_REQ_UI_SCHEDULE,&uiScheduleCmd, NULL) <= -1 )
      return NULL;
  }


  *count=0;

  gettimeofday(&timeVal, 0);
  startTime=(uint32_t)( (uint32_t)timeVal.tv_sec*1000+ (uint32_t)timeVal.tv_usec/1000);
  while( mDevice->isReqAckUiSchedule==FALSE  )
  {
      uint32_t curentTime;

      gettimeofday(&timeVal, 0);
      curentTime=(uint32_t)( (uint32_t)timeVal.tv_sec*1000+ (uint32_t)timeVal.tv_usec/1000);
      if ( (curentTime-startTime)>TIME_OUT )
      {
        scoped_lock_t lock(mPc->mMutex);
        mDevice->isReqAckUiSchedule=FALSE;
        throw PlayerError("DispatcherProxy::ReqUiSchedule::TIME_OUT");
      }

      usleep(1000);
  }


  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isReqAckUiSchedule=FALSE;
    *count = mDevice->uiSchedule.data_count;
  }

  return mDevice->uiSchedule.data;
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::GetReqUiSchedule(uint8_t *initiator)
{
  if (  dispatcher_get_req_ui_schedule(mDevice, initiator) )
    throw PlayerError("DispatcherProxy::GetReqUiSchedule");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::SetReqAckUiSchedule(uint8_t destination, uint32_t count,  player_dispatcher_ui_alarm_time_t data[])
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_set_req_ack_ui_schedule(mDevice , destination, count,  data) )
    throw PlayerError("DispatcherProxy::SetReqAckUiSchedule");
}
//-------------------------------------------------------------------------------------------------
uint8_t
DispatcherProxy::DataDestination()
{
  return mDevice->dataDestination;
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::SetFile( uint8_t initiator,
    const char *name,
    uint32_t name_size,
    const char *data,
    uint32_t data_size)
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_set_file(mDevice, initiator, (const int8_t *)name, name_size, (const int8_t *)data, data_size) )
    throw PlayerError("DispatcherProxy::SetFile");
}
//-------------------------------------------------------------------------------------------------
char *
DispatcherProxy::GetFile( uint8_t initiator,
    const char *name,
    uint32_t name_size,
    uint32_t *data_size)
{
  scoped_lock_t lock(mPc->mMutex);

  return (char *)dispatcher_get_file(mDevice, initiator, (const int8_t *)name, name_size, data_size);
}
//-------------------------------------------------------------------------------------------------
//data <0 is playerc_client_request error
//data =0 is playerc_client_request OK
//data >0 is playerc_client_request success but pairing error
int
DispatcherProxy::StartPairing(uint8_t initiator,
    const char *mac )
{
  scoped_lock_t lock(mPc->mMutex);
  int result;

  result=dispatcher_start_pairing(mDevice, initiator, (const uint8_t *) mac);
  if ( result<0 )
    throw PlayerError("DispatcherProxy::StartPairing");

  return result;
}
//-------------------------------------------------------------------------------------------------
//return the login logout result if
//data <0 is playerc_client_request error
//data =0 is playerc_client_request OK
//data >0 is playerc_client_request success but login or logout error
int DispatcherProxy::LogIn(const char *mac)
{
  scoped_lock_t lock(mPc->mMutex);
  int result;

  result=dispatcher_log_in_out(mDevice, 1, (const uint8_t *)mac);
  if ( result<0 )
    throw PlayerError("DispatcherProxy::LogIn");

  return result;
}
//-------------------------------------------------------------------------------------------------
//return the login logout result if
//data <0 is playerc_client_request error
//data =0 is playerc_client_request OK
//data >0 is playerc_client_request success but login or logout error
int DispatcherProxy::LogOut(const char *mac)
{
  scoped_lock_t lock(mPc->mMutex);
  int result;

  result=dispatcher_log_in_out(mDevice, 0, (const uint8_t *)mac);
  if ( result<0 )
    throw PlayerError("DispatcherProxy::LogOut");

  return result;
}
//-------------------------------------------------------------------------------------------------
bool
DispatcherProxy::IsSetAiCmd()
{
  if ( mDevice->isSetAiCmd==TRUE )
  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isSetAiCmd=FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
//-------------------------------------------------------------------------------------------------
bool
DispatcherProxy::IsSetPmCmd()
{
  if ( mDevice->isSetPmCmd==TRUE )
  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isSetPmCmd=FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
//-------------------------------------------------------------------------------------------------
bool
DispatcherProxy::IsSetUiCmd()
{
  if ( mDevice->isSetUiCmd==TRUE )
  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isSetUiCmd=FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
//-------------------------------------------------------------------------------------------------
void DispatcherProxy::Status( int8_t *login, int8_t *pairing)
{
   scoped_lock_t lock(mPc->mMutex);

   *login=mDevice->statusData.login;
   *pairing=mDevice->statusData.pairing;
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::SetUiCmd(uint8_t initiator, uint32_t cmd)
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_set_cmd_ui(mDevice, initiator, cmd)!=0 )
    throw PlayerError("DispatcherProxy::SetUiCmd()");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::GetUiCmd(uint8_t *initiator, uint32_t *cmd)
{
  scoped_lock_t lock(mPc->mMutex);
  mDevice->isSetUiCmd=FALSE;
  *initiator=mDevice->uiCmdData.init_des.initiator;
  *cmd=mDevice->uiCmdData.cmd;
}
//-------------------------------------------------------------------------------------------------
bool
DispatcherProxy::IsReqUiName()
{
  if ( mDevice->isReqUiName==TRUE )
  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isReqUiName=FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
//-------------------------------------------------------------------------------------------------
bool
DispatcherProxy::IsSetUiName()
{
  if ( mDevice->isSetUiName==TRUE )
  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isSetUiName=FALSE;
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::SetUiName(uint8_t initiator, uint8_t *name)
{
  scoped_lock_t lock(mPc->mMutex);

  if (  dispatcher_set_ui_name(mDevice, initiator, name)!=0 )
    throw PlayerError("DispatcherProxy::SetUiName()");
}
//-------------------------------------------------------------------------------------------------
uint8_t *
DispatcherProxy::GetUiName(uint8_t *initiator)
{
  return dispatcher_get_ui_name(mDevice, initiator);
}
//-------------------------------------------------------------------------------------------------
uint8_t *
DispatcherProxy::ReqUiName(uint8_t initiator)
{
  uint32_t startTime=0;
  struct timeval timeVal, timeVal2;

  {
    scoped_lock_t lock(mPc->mMutex);
    player_dispatcher_ui_name_t uiNameCmd={};

    uiNameCmd.init_des.initiator=initiator;
    uiNameCmd.init_des.destination=PLAYER_DISPATCH_INIT_DEST_UI;
    uiNameCmd.name_count=0;
    uiNameCmd.name=NULL;

    //send command to request UI time data
    if ( playerc_client_write(mDevice->info.client, &mDevice->info, PLAYER_DISPATCHER_CMD_REQ_UI_NAME, &uiNameCmd, NULL) <= -1 )
      return NULL;
  }


  gettimeofday(&timeVal, 0);
  startTime=(uint32_t)( (uint32_t)timeVal.tv_sec*1000+ (uint32_t)timeVal.tv_usec/1000);
  while( mDevice->isReqAckUiName==FALSE  )
  {
    uint32_t curentTime;


    gettimeofday(&timeVal2, 0);
    curentTime=(uint32_t)( (uint32_t)timeVal2.tv_sec*1000+ (uint32_t)timeVal2.tv_usec/1000);
    if ( (curentTime-startTime)>TIME_OUT )
    {
      scoped_lock_t lock(mPc->mMutex);
      mDevice->isReqAckUiName=FALSE;
      throw PlayerError("DispatcherProxy::ReqUiName::TIME_OUT");
    }

    usleep(1000);
  }

  {
    scoped_lock_t lock(mPc->mMutex);
    mDevice->isReqAckUiName=FALSE;
  }

  return mDevice->uiName.name;
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::GetReqUiName(uint8_t *initiator)
{
  if ( dispatcher_get_req_ui_name(mDevice, initiator) )
    throw PlayerError("DispatcherProxy::GetReqUiName");
}
//-------------------------------------------------------------------------------------------------
void
DispatcherProxy::SetReqAckUiName( uint8_t destination, uint8_t *name )
{
  scoped_lock_t lock(mPc->mMutex);

  if ( dispatcher_set_req_ack_ui_name(mDevice, destination, name) )
    throw PlayerError("DispatcherProxy::GetReqAckUiName");
}
//-------------------------------------------------------------------------------------------------









