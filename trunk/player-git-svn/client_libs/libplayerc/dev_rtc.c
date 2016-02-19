#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <libplayercommon/playercommon.h>
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
// Process incoming data
void playerc_rtc_putmsg(playerc_rtc_t *device, player_msghdr_t *header, uint8_t *data, size_t len);
//-------------------------------------------------------------------------------------------------
playerc_rtc_t *rtc_create (playerc_client_t *client, int index)
{
  playerc_rtc_t *device;

  device = (playerc_rtc_t *) malloc (sizeof (playerc_rtc_t));
  memset (device, 0, sizeof (playerc_rtc_t));
  playerc_device_init (&device->info, client, PLAYER_RTC_CODE, index,
                       (playerc_putmsg_fn_t) playerc_rtc_putmsg);

  return device;
}
//-------------------------------------------------------------------------------------------------
void rtc_destroy (playerc_rtc_t *device)
{
  playerc_device_term (&device->info);
  free (device);
}
//-------------------------------------------------------------------------------------------------
int rtc_subscribe (playerc_rtc_t *device, int access)
{
  return playerc_device_subscribe (&device->info, access);
}
//-------------------------------------------------------------------------------------------------
int rtc_unsubscribe (playerc_rtc_t *device)
{
  return playerc_device_unsubscribe (&device->info);
}
//-------------------------------------------------------------------------------------------------
void playerc_rtc_putmsg(playerc_rtc_t *device, player_msghdr_t *header, uint8_t *data, size_t len)
{
  if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_RTC_DATA_TIME) )
  {
    
    player_rtc_time_t *rtcData = (player_rtc_time_t *)data;
    TRACE("PLAYER_MSGTYPE_DATA:PLAYER_RTC_DATA_TIME\n");
	assert(header->size > 0);

    memcpy( (void *)&device->rtc , (void *)rtcData, sizeof(player_rtc_time_t) );
  }//end if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_RTC_DATA_TIME) )
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_RTC_DATA_ALARM) )
  {
    
    player_rtc_alarm_t *alarmData = (player_rtc_alarm_t *)data;
    TRACE("PLAYER_MSGTYPE_DATA:PLAYER_RTC_DATA_ALARM\n");
	assert(header->size > 0);

    memcpy( (void *)&device->alarm, (void *)alarmData, sizeof(player_rtc_alarm_t));
  }//end else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_RTC_DATA_ALARM) )
  else
  {
     PLAYERC_WARN2("skipping rtc message with unknown type/subtype: %s/%d\n",
                  msgtype_to_str(header->type), header->subtype);
  }
}
//-------------------------------------------------------------------------------------------------
int rtc_get_time (playerc_rtc_t *device, uint32_t *utc_time, int32_t *minuteswest, int32_t *dsttime)
{
  TRACE(">>%s\n", __FUNCTION__);

  *utc_time=device->rtc.time;
  *minuteswest=device->rtc.minuteswest;
  *dsttime=device->rtc.dsttime;

  return 0;
}
//-------------------------------------------------------------------------------------------------
int rtc_set_time (playerc_rtc_t *device, uint32_t utc_time, int32_t minuteswest, int32_t dsttime)
{
  
  player_rtc_time_t rtc;
  TRACE(">>%s\n", __FUNCTION__);

  memset(&rtc, 0, sizeof(rtc));
  rtc.time = utc_time;
  rtc.minuteswest = minuteswest;
  rtc.dsttime = dsttime;

  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_RTC_CMD_SET_TIME,
                              &rtc, NULL);

  return 0;
}
//-------------------------------------------------------------------------------------------------
int rtc_get_alarm (playerc_rtc_t *device, uint32_t *utc_time)
{
  TRACE(">>%s\n", __FUNCTION__);
  *utc_time=device->alarm.time;

  return 0;
}
//-------------------------------------------------------------------------------------------------
int rtc_set_alarm (playerc_rtc_t *device, uint32_t utc_time)
{
  player_rtc_alarm_t alarm;
  TRACE(">>%s\n", __FUNCTION__);


  memset(&alarm, 0, sizeof(alarm));
  alarm.time = utc_time;

  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_RTC_CMD_SET_ALARM,
                              &alarm, NULL);
}
//-------------------------------------------------------------------------------------------------
//req_type
//0:CLEAR_ALARM
//1:KEEP_ALARM
int rtc_is_on_alarm (playerc_rtc_t *device, uint8_t req_type, uint8_t *is_on_alarm)
{
  TRACE(">>%s\n", __FUNCTION__);
  //CLEAR_ALARM
  if ( device->alarm.is_alarm && req_type==0 )
  {
    *is_on_alarm=TRUE;
    device->alarm.is_alarm=FALSE;
  }
  else if (device->alarm.is_alarm && req_type==1 )
  {
    *is_on_alarm=TRUE;
  }
  else
  {
    *is_on_alarm=FALSE;
  }


  return 0;
}
//-------------------------------------------------------------------------------------------------








