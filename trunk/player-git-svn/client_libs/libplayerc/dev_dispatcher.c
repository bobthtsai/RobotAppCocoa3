#include <math.h>
#include <stdlib.h>
#include <string.h>
#if !defined (WIN32)
#include <unistd.h>
#endif
#include "playerc.h"
#include "error.h"

#ifdef CLOCKGETTIME_REPLACE_GETTIMEOFDAY
//	#include <utils/gettimereplace/gettimereplace.h>
	#include <replace/gettimereplace/gettimereplace.h>
	#define gettimeofday clockgettime_replace_gettimeofday
#endif


//-------------------------------------------------------------------------------------------------
#define ASSERT_EN                                1
#define TRACE_EN                                 0
#define WARN_EN                                  0
#define ERROR_EN                                 0
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
void playerc_dispatcher_putmsg(playerc_dispatcher_t *device, player_msghdr_t *header, uint8_t *data, size_t len);
//-------------------------------------------------------------------------------------------------
playerc_dispatcher_t *dispatcher_create (playerc_client_t *client, int index)
{
  playerc_dispatcher_t *device;

  device = (playerc_dispatcher_t *) malloc (sizeof (playerc_dispatcher_t));
  memset (device, 0, sizeof (playerc_dispatcher_t));
  playerc_device_init (&device->info, client, PLAYER_DISPATCHER_CODE, index, 
                       (playerc_putmsg_fn_t) playerc_dispatcher_putmsg);

  //initial default status
  device->aiStatus.init_des.destination=(PLAYER_DISPATCH_INIT_DEST_UI+PLAYER_DISPATCH_INIT_DEST_PM+PLAYER_DISPATCH_INIT_DEST_RD);
  device->aiStatus.status=PLAYER_DISPATCH_AI_STANDBY;
  device->aiStatus.power=PLAYER_DISPATCH_PM_STANDBY;
  device->aiStatus.error=PLAYER_DISPATCH_NO_ERROR;
  device->aiStatus.worry=PLAYER_DISPATCH_NO_WORRY;

  device->uiStatus.init_des.destination=(PLAYER_DISPATCH_INIT_DEST_AI+PLAYER_DISPATCH_INIT_DEST_PM+PLAYER_DISPATCH_INIT_DEST_RD);
  device->uiStatus.status=PLAYER_DISPATCH_UI_STANDBY;

  device->pmStatus.init_des.destination=(PLAYER_DISPATCH_INIT_DEST_UI+PLAYER_DISPATCH_INIT_DEST_AI+PLAYER_DISPATCH_INIT_DEST_RD);
  device->pmStatus.status=PLAYER_DISPATCH_PM_STANDBY;
  device->pmStatus.error=PLAYER_DISPATCH_NO_ERROR;

  return device;
}
//-------------------------------------------------------------------------------------------------
void dispatcher_destroy (playerc_dispatcher_t *device)
{
  int i;
  playerc_device_term (&device->info);

  if ( device->fileData!=NULL  )
    free( device->fileData );
  if ( device->uiName.name!=NULL  )
    free( device->uiName.name );
  if ( device->aiCmdData.job.map_index_count!=0 )
    free(device->aiCmdData.job.map_index);
  if ( device->aiCmdData.job.data_count!=0  )
    free(device->aiCmdData.job.data);
  if ( device->aiCmdData.job.name_count!=0  )
    free(device->aiCmdData.job.name);


  if ( device->uiSchedule.data_count!=0  )
  {
      for ( i=0; i<device->uiSchedule.data_count; i++ )
      {
        if ( device->uiSchedule.data[i].job.map_index_count!=0  )
          free(device->uiSchedule.data[i].job.map_index);
        if ( device->uiSchedule.data[i].job.data_count!=0  )
          free(device->uiSchedule.data[i].job.data);
        if ( device->uiSchedule.data[i].job.name_count!=0 )
          free(device->uiSchedule.data[i].job.name);
      }//end for ( int i=0; i<device->uiSchedule.data_count; i++ )
      free(device->uiSchedule.data);
  }//end if ( device->uiSchedule.data_count!=0 )


  free (device);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_subscribe (playerc_dispatcher_t *device, int access)
{
  return playerc_device_subscribe (&device->info, access);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_unsubscribe (playerc_dispatcher_t *device)
{
  return playerc_device_unsubscribe (&device->info);
}
//-------------------------------------------------------------------------------------------------
void playerc_dispatcher_putmsg(playerc_dispatcher_t *device, player_msghdr_t *header, uint8_t *data, size_t len)
{
  if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_AI) )
  {
    
    player_dispatcher_ai_cmd_data_t *aiCmdData = (player_dispatcher_ai_cmd_data_t *) data;
    TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_AI\n");
	ASSERT(header->size > 0);



    device->aiCmdData.init_des=aiCmdData->init_des;
    device->aiCmdData.job.cmd=aiCmdData->job.cmd;
    device->aiCmdData.job.power=aiCmdData->job.power;
    device->aiCmdData.job.target_point=aiCmdData->job.target_point;
    device->aiCmdData.job.stop_cond=aiCmdData->job.stop_cond;

    device->aiCmdData.job.map_index_count=aiCmdData->job.map_index_count;
    DEBUG("aiCmdData->job.map_index_count=%d\n", aiCmdData->job.map_index_count);
    if ( device->aiCmdData.job.map_index_count!=0 )
      free(device->aiCmdData.job.map_index);
    device->aiCmdData.job.map_index = malloc(aiCmdData->job.map_index_count*sizeof(uint32_t));
    ASSERT(device->aiCmdData.job.map_index);
    memcpy(device->aiCmdData.job.map_index , aiCmdData->job.map_index, aiCmdData->job.map_index_count*sizeof(uint32_t));

    device->aiCmdData.job.data_count=aiCmdData->job.data_count;
    DEBUG("aiCmdData->job.data_count=%d\n", aiCmdData->job.data_count);
    if ( device->aiCmdData.job.data_count!=0  )
      free(device->aiCmdData.job.data);
    device->aiCmdData.job.data = malloc(aiCmdData->job.data_count*sizeof(uint8_t));
    ASSERT(device->aiCmdData.job.data );
    memcpy(device->aiCmdData.job.data , aiCmdData->job.data, aiCmdData->job.data_count*sizeof(uint8_t));

    device->aiCmdData.job.name_count=aiCmdData->job.name_count;
    DEBUG("aiCmdData->job.name_count=%d\n", aiCmdData->job.name_count);
    if ( device->aiCmdData.job.name_count!=0  )
      free(device->aiCmdData.job.name);
    device->aiCmdData.job.name = malloc(aiCmdData->job.name_count*sizeof(uint8_t));
    ASSERT(device->aiCmdData.job.name );
    memcpy(device->aiCmdData.job.name , aiCmdData->job.name, aiCmdData->job.name_count*sizeof(uint8_t));

    device->dataDestination=aiCmdData->init_des.destination;
    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isSetAiCmd=TRUE;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_PM) )
  {
    
    player_dispatcher_pm_cmd_data_t *pmCmdData = (player_dispatcher_pm_cmd_data_t *) data;
    TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_PM\n");
	ASSERT(header->size > 0);


    device->pmCmdData=*pmCmdData;
    device->dataDestination=pmCmdData->init_des.destination;
    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isSetPmCmd=TRUE;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_SET_UI_TIME) )
  {//check Ok
    
    player_dispatcher_ui_time_t *uiTime = (player_dispatcher_ui_time_t *) data;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_SET_UI_TIME\n");
    ASSERT(header->size > 0);

    device->uiTime=*uiTime;
    device->dataDestination=uiTime->init_des.destination;
    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isSetUiTime=TRUE;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_SET_UI_SCHEDULE) )
  {//check OK
    
    player_dispatcher_ui_schedule_t *uiSchedule = (player_dispatcher_ui_schedule_t *) data;
    int i;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_SET_UI_SCHEDULE\n");

    ASSERT(header->size > 0);

    if ( device->uiSchedule.data_count!=0  )
    {
        for ( i=0; i<device->uiSchedule.data_count; i++ )
        {
          if ( device->uiSchedule.data[i].job.map_index_count!=0 )
            free(device->uiSchedule.data[i].job.map_index);
          if ( device->uiSchedule.data[i].job.data_count!=0  )
            free(device->uiSchedule.data[i].job.data);
          if ( device->uiSchedule.data[i].job.name_count!=0 )
            free(device->uiSchedule.data[i].job.name);
        }//end for ( int i=0; i<device->uiSchedule.data_count; i++ )
        free(device->uiSchedule.data);
    }//end if ( device->uiSchedule.data_count!=0 )


    device->uiSchedule.data_count=uiSchedule->data_count;
    device->uiSchedule.data=malloc( sizeof(player_dispatcher_ui_alarm_time_t) * (uiSchedule->data_count) );
    ASSERT(device->uiSchedule.data);
    for ( i=0; i<uiSchedule->data_count; i++)
    {
      memcpy(device->uiSchedule.data[i].uuid, uiSchedule->data[i].uuid, sizeof(uiSchedule->data[i].uuid));
      device->uiSchedule.data[i].type=uiSchedule->data[i].type;
      device->uiSchedule.data[i].play_type=uiSchedule->data[i].play_type;

      device->uiSchedule.data[i].time=uiSchedule->data[i].time;
      device->uiSchedule.data[i].zone=uiSchedule->data[i].zone;
      device->uiSchedule.data[i].job.cmd=uiSchedule->data[i].job.cmd;
      device->uiSchedule.data[i].job.power=uiSchedule->data[i].job.power;
      device->uiSchedule.data[i].job.map_index_count=uiSchedule->data[i].job.map_index_count;
      device->uiSchedule.data[i].job.data_count=uiSchedule->data[i].job.data_count;
      device->uiSchedule.data[i].job.name_count=uiSchedule->data[i].job.name_count;

      device->uiSchedule.data[i].job.target_point=uiSchedule->data[i].job.target_point;
      device->uiSchedule.data[i].job.stop_cond=uiSchedule->data[i].job.stop_cond;

      if ( uiSchedule->data[i].job.map_index_count!=0 )
      {
        device->uiSchedule.data[i].job.map_index=malloc( sizeof(uint32_t)*uiSchedule->data[i].job.map_index_count );
        ASSERT(device->uiSchedule.data[i].job.map_index);
        memcpy(device->uiSchedule.data[i].job.map_index,
            uiSchedule->data[i].job.map_index,
            sizeof(uint32_t)*uiSchedule->data[i].job.map_index_count);
      }

      if ( uiSchedule->data[i].job.data_count!=0 )
      {
        device->uiSchedule.data[i].job.data=malloc( sizeof(uint8_t)*uiSchedule->data[i].job.data_count );
        ASSERT(device->uiSchedule.data[i].job.data);
        memcpy(device->uiSchedule.data[i].job.data,
            uiSchedule->data[i].job.data,
            sizeof(uint8_t)*uiSchedule->data[i].job.data_count);
      }

      if ( uiSchedule->data[i].job.name_count!=0 )
      {
        device->uiSchedule.data[i].job.name=malloc( sizeof(uint8_t)*uiSchedule->data[i].job.name_count );
        ASSERT(device->uiSchedule.data[i].job.name);
        memcpy(device->uiSchedule.data[i].job.name,
            uiSchedule->data[i].job.name,
            sizeof(uint8_t)*uiSchedule->data[i].job.name_count);
      }
    }//end for ( int i=0; i<uiSchedule->data_count; i++)


    device->dataDestination=uiSchedule->init_des.destination;
    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isSetUiSchedule=TRUE;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_REQ_UI_TIME) )
  {
    
    player_dispatcher_ui_time_t *uiTime = (player_dispatcher_ui_time_t *) data;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_REQ_UI_TIME\n");

    device->dataDestination=uiTime->init_des.destination;
    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isReqUiTime=TRUE;
    //TRACE("CUSTOM_TRACE:device->device->isReqUiTime=TRUE\n");
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_REQ_ACK_UI_TIME) )
  {
    
    player_dispatcher_ui_time_t *uiTime = (player_dispatcher_ui_time_t *) data;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_REQ_ACK_UI_TIME\n");

    ASSERT(header->size > 0);

    device->uiTime=*uiTime;
    device->dataDestination=uiTime->init_des.destination;
    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isReqAckUiTime=TRUE;
	//TRACE("CUSTOM_TRACE:device->isReqAckUiTime=TRUE\n");
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_REQ_UI_SCHEDULE) )
  {
    
    player_dispatcher_ui_schedule_t *uiSchedule = (player_dispatcher_ui_schedule_t *) data;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_REQ_UI_SCHEDULE\n");
    device->dataDestination=uiSchedule->init_des.destination;
    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isReqUiSchedule=TRUE;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_REQ_ACK_UI_SCHEDULE) )
  {
    
    player_dispatcher_ui_schedule_t *uiSchedule = (player_dispatcher_ui_schedule_t *) data;
    int i;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_REQ_ACK_UI_SCHEDULE\n");

    if ( device->uiSchedule.data_count!=0 )
    {
        for ( i=0; i<device->uiSchedule.data_count; i++ )
        {
          if ( device->uiSchedule.data[i].job.map_index_count!=0 )
            free(device->uiSchedule.data[i].job.map_index);
          if ( device->uiSchedule.data[i].job.data_count!=0 )
            free(device->uiSchedule.data[i].job.data);
          if ( device->uiSchedule.data[i].job.name_count!=0 )
            free(device->uiSchedule.data[i].job.name);
        }//end for ( int i=0; i<device->uiSchedule.data_count; i++ )
        free(device->uiSchedule.data);
    }//end if ( device->uiSchedule.data_count!=0 )


    device->uiSchedule.data_count=uiSchedule->data_count;
    device->uiSchedule.data=malloc( sizeof(player_dispatcher_ui_alarm_time_t) * (uiSchedule->data_count) );
    ASSERT(device->uiSchedule.data);

    for ( i=0; i<uiSchedule->data_count; i++)
    {
        memcpy(device->uiSchedule.data[i].uuid, uiSchedule->data[i].uuid, sizeof(uiSchedule->data[i].uuid));
        device->uiSchedule.data[i].type=uiSchedule->data[i].type;
        device->uiSchedule.data[i].play_type=uiSchedule->data[i].play_type;

        device->uiSchedule.data[i].time=uiSchedule->data[i].time;
        device->uiSchedule.data[i].zone=uiSchedule->data[i].zone;
        device->uiSchedule.data[i].job.cmd=uiSchedule->data[i].job.cmd;
        device->uiSchedule.data[i].job.power=uiSchedule->data[i].job.power;
        device->uiSchedule.data[i].job.map_index_count=uiSchedule->data[i].job.map_index_count;
        device->uiSchedule.data[i].job.data_count=uiSchedule->data[i].job.data_count;
        device->uiSchedule.data[i].job.name_count=uiSchedule->data[i].job.name_count;

        device->uiSchedule.data[i].job.target_point=uiSchedule->data[i].job.target_point;
        device->uiSchedule.data[i].job.stop_cond=uiSchedule->data[i].job.stop_cond;

        if ( uiSchedule->data[i].job.map_index_count!=0 )
        {
          device->uiSchedule.data[i].job.map_index=malloc( sizeof(uint32_t)*uiSchedule->data[i].job.map_index_count );
          ASSERT(device->uiSchedule.data[i].job.map_index);
          memcpy(device->uiSchedule.data[i].job.map_index,
              uiSchedule->data[i].job.map_index,
              sizeof(uint32_t)*uiSchedule->data[i].job.map_index_count);
        }

        if ( uiSchedule->data[i].job.data_count!=0 )
        {
          device->uiSchedule.data[i].job.data=malloc( sizeof(uint8_t)*uiSchedule->data[i].job.data_count );
          ASSERT(device->uiSchedule.data[i].job.data);
          memcpy(device->uiSchedule.data[i].job.data,
              uiSchedule->data[i].job.data,
              sizeof(uint8_t)*uiSchedule->data[i].job.data_count);
        }

        if ( uiSchedule->data[i].job.name_count!=0 )
        {
          device->uiSchedule.data[i].job.name=malloc( sizeof(uint8_t)*uiSchedule->data[i].job.name_count );
          ASSERT(device->uiSchedule.data[i].job.name);
          memcpy(device->uiSchedule.data[i].job.name,
              uiSchedule->data[i].job.name,
              sizeof(uint8_t)*uiSchedule->data[i].job.name_count);
        }
    }//end for ( int i=0; i<uiSchedule->data_count; i++)


    device->dataDestination=uiSchedule->init_des.destination;
    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isReqAckUiSchedule=TRUE;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_STATUS) )
  {
    
    player_dispatcher_status_t *status = (player_dispatcher_status_t *) data;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_STATUS\n");
    device->dataDestination= status->init_des.destination;
    device->statusData = *status ;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_UI) )
  {
    
    player_dispatcher_ui_cmd_data_t *uiCmdData = (player_dispatcher_ui_cmd_data_t *) data;
    TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_UI\n");
	ASSERT(header->size > 0);
	

    device->uiCmdData=*uiCmdData;
    device->dataDestination=uiCmdData->init_des.destination;
    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isSetUiCmd=TRUE;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_SET_UI_NAME) )
  {
    
    player_dispatcher_ui_name_t *uiName = (player_dispatcher_ui_name_t *) data;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_SET_UI_NAME\n");
    ASSERT(header->size > 0);

    device->uiName.init_des = uiName->init_des;
    device->uiName.name_count = uiName->name_count;
    if ( device->uiName.name)
      free(device->uiName.name);
    device->uiName.name = malloc(device->uiName.name_count);
    ASSERT(device->uiName.name);
    memcpy(device->uiName.name, uiName->name, device->uiName.name_count);

    DEBUG("dataDestination=%d name=%s name_count=%d\n", device->dataDestination, device->uiName.name, device->uiName.name_count);
    device->isSetUiName=TRUE;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_REQ_UI_NAME) )
  {
    
    player_dispatcher_ui_name_t *uiName = (player_dispatcher_ui_name_t *) data;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_REQ_UI_NAME\n");
    device->dataDestination=uiName->init_des.destination;
    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isReqUiName=TRUE;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_REQ_ACK_UI_NAME) )
  {
    
    player_dispatcher_ui_name_t *uiName = (player_dispatcher_ui_name_t *) data;
	TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_REQ_ACK_UI_NAME\n");
    ASSERT(header->size > 0);

    device->uiName.init_des = uiName->init_des;
    device->uiName.name_count = uiName->name_count;
    if ( device->uiName.name)
      free(device->uiName.name);
    device->uiName.name = malloc(device->uiName.name_count);
    ASSERT(device->uiName.name);
    memcpy(device->uiName.name, uiName->name, device->uiName.name_count);


    DEBUG("dataDestination=%d\n", device->dataDestination);
    device->isReqAckUiName=TRUE;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_AI_STATUS ) )
  {
    TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_AI_STATUS\n");
    device->aiStatus = *(player_dispatcher_ai_status_t *) data;
    ASSERT(header->size > 0);

    DEBUG("status=%d power=%d error=%d worry=%d\n" ,
        device->aiStatus.status,
        device->aiStatus.power,
        device->aiStatus.error,
        device->aiStatus.worry);
    device->dataDestination=device->aiStatus.init_des.destination;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_UI_STATUS ) )
  {
    TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_UI_STATUS\n");
    device->uiStatus = *(player_dispatcher_ui_status_t *) data;
    ASSERT(header->size > 0);

    DEBUG("status=%d \n" ,
        device->uiStatus.status);
    device->dataDestination=device->uiStatus.init_des.destination;
  }
  else if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_DISPATCHER_DATA_PM_STATUS ) )
  {
    TRACE("PLAYER_MSGTYPE_DATA:PLAYER_DISPATCHER_DATA_PM_STATUS\n");
    device->pmStatus = *(player_dispatcher_pm_status_t *) data;
    ASSERT(header->size > 0);

    DEBUG("status=%d error=%d\n" ,
        device->pmStatus.status,
        device->pmStatus.error);
    device->dataDestination=device->pmStatus.init_des.destination;
  }
  else
    PLAYERC_WARN2("skipping dispatch message with unknown type/subtype: %s/%d\n",
                 msgtype_to_str(header->type), header->subtype);

}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_cmd_ai(playerc_dispatcher_t *device,
    uint8_t initiator,
    uint32_t cmd,
    uint32_t power,
    uint32_t map_index_count,
    uint32_t *map_index,
    player_pose2d_t target_point,
    uint32_t name_count,
    int8_t *name,
    uint32_t data_count,
    int8_t *data,
    uint32_t stop_cond)
{
  player_dispatcher_ai_cmd_data_t aiCmdData;


  memset(&aiCmdData, 0, sizeof(aiCmdData));
  aiCmdData.init_des.initiator=initiator;
  aiCmdData.init_des.destination=PLAYER_DISPATCH_INIT_DEST_AI;
  aiCmdData.job.cmd=cmd;
  aiCmdData.job.power=power;
  aiCmdData.job.map_index_count=map_index_count;
  aiCmdData.job.map_index=map_index;
  aiCmdData.job.target_point=target_point;
  aiCmdData.job.name_count=name_count;
  aiCmdData.job.name=name;
  aiCmdData.job.data_count=data_count;
  aiCmdData.job.data=data;
  aiCmdData.job.stop_cond=stop_cond;

  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_AI,
                              &aiCmdData, NULL);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_cmd_pm(playerc_dispatcher_t *device, uint8_t initiator, uint32_t cmd)
{
  player_dispatcher_pm_cmd_data_t pmCmdData;


  memset(&pmCmdData, 0, sizeof(pmCmdData));
  pmCmdData.init_des.initiator=initiator;
  pmCmdData.init_des.destination=PLAYER_DISPATCH_INIT_DEST_PM;
  pmCmdData.cmd = cmd;

  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_PM,
                              &pmCmdData, NULL);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_status_ai(playerc_dispatcher_t *device,
    uint8_t initiator,
    uint32_t aiStatus,
    uint32_t power,
    uint32_t error,
    uint32_t worry)
{
  player_dispatcher_ai_status_t ai;


  ai.init_des.initiator=initiator;
  ai.init_des.destination=(PLAYER_DISPATCH_INIT_DEST_UI+PLAYER_DISPATCH_INIT_DEST_PM+PLAYER_DISPATCH_INIT_DEST_RD);
  ai.status=aiStatus;
  ai.power=power;
  ai.error=error;
  ai.worry=worry;


  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_AI_STATUS,
                              &ai, NULL);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_status_pm(playerc_dispatcher_t *device,
    uint8_t initiator,
    uint32_t pmStatus,
    uint32_t error)
{
  player_dispatcher_pm_status_t pm;


  pm.init_des.initiator=initiator;
  pm.init_des.destination=(PLAYER_DISPATCH_INIT_DEST_UI+PLAYER_DISPATCH_INIT_DEST_AI+PLAYER_DISPATCH_INIT_DEST_RD);
  pm.status=pmStatus;
  pm.error=error;

  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_PM_STATUS,
                              &pm, NULL);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_status_ui(playerc_dispatcher_t *device,
    uint8_t initiator,
    uint32_t uiStatus)
{
  player_dispatcher_ui_status_t ui;


  ui.init_des.initiator=initiator;
  ui.init_des.destination=(PLAYER_DISPATCH_INIT_DEST_AI+PLAYER_DISPATCH_INIT_DEST_PM+PLAYER_DISPATCH_INIT_DEST_RD);
  ui.status=uiStatus;

  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_UI_STATUS,
                              &ui, NULL);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_get_status_ai(playerc_dispatcher_t *device,
    uint8_t initiator,
    uint32_t *aiStatus,
    uint32_t *cleanStatus,
    uint32_t *error,
    uint32_t *worry)
{
  *aiStatus = device->aiStatus.status;
  *cleanStatus = device->aiStatus.power;
  *error = device->aiStatus.error;
  *worry = device->aiStatus.worry;


  //send tick to dispatcher plug-in to prevent logout
  if ( initiator==PLAYER_DISPATCH_INIT_DEST_RD )
  {
    player_dispatcher_tick_t tick;

    tick.init_des.initiator=PLAYER_DISPATCH_INIT_DEST_RD;
    tick.init_des.destination=PLAYER_DISPATCH_INIT_DEST_DIS;

    return playerc_client_write(device->info.client, &device->info,
                                PLAYER_DISPATCHER_CMD_TICK,
                                &tick, NULL);
  }

  return 0;
}
//-------------------------------------------------------------------------------------------------
int dispatcher_get_status_pm(playerc_dispatcher_t *device,
    uint8_t initiator,
    uint32_t *pmStatus,
    uint32_t *error)
{
  *pmStatus = device->pmStatus.status;
  *error = device->pmStatus.error;

  //send tick to dispatcher plug-in to prevent logout
  if ( initiator==PLAYER_DISPATCH_INIT_DEST_RD )
  {
    player_dispatcher_tick_t tick;

    tick.init_des.initiator=PLAYER_DISPATCH_INIT_DEST_RD;
    tick.init_des.destination=PLAYER_DISPATCH_INIT_DEST_DIS;

    return playerc_client_write(device->info.client, &device->info,
                                PLAYER_DISPATCHER_CMD_TICK,
                                &tick, NULL);
  }

  return 0;
}
//-------------------------------------------------------------------------------------------------
int dispatcher_get_status_ui(playerc_dispatcher_t *device,
    uint8_t initiator,
    uint32_t *uiStatus)
{
  *uiStatus = device->uiStatus.status;

  //send tick to dispatcher plug-in to prevent logout
  if ( initiator==PLAYER_DISPATCH_INIT_DEST_RD )
  {
    player_dispatcher_tick_t tick;

    tick.init_des.initiator=PLAYER_DISPATCH_INIT_DEST_RD;
    tick.init_des.destination=PLAYER_DISPATCH_INIT_DEST_DIS;

    return playerc_client_write(device->info.client, &device->info,
                                PLAYER_DISPATCHER_CMD_TICK,
                                &tick, NULL);
  }

  return 0;
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_ui_time(playerc_dispatcher_t *device,
		uint8_t initiator,
		player_dispatcher_time_t time,
		player_dispatcher_timezone_t zone)
{
  player_dispatcher_ui_time_t uiTimeCmd;


  memset(&uiTimeCmd, 0, sizeof(uiTimeCmd));
  uiTimeCmd.init_des.initiator=initiator;
  uiTimeCmd.init_des.destination=PLAYER_DISPATCH_INIT_DEST_UI;
  uiTimeCmd.data = time;
  uiTimeCmd.zone = zone;

  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_SET_UI_TIME,
                              &uiTimeCmd, NULL);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_ui_schedule(playerc_dispatcher_t *device,
		uint8_t initiator,
		uint32_t count,
		player_dispatcher_ui_alarm_time_t data[])
{
  player_dispatcher_ui_schedule_t uiScheduleCmd;

  memset(&uiScheduleCmd, 0, sizeof(uiScheduleCmd));
  uiScheduleCmd.init_des.initiator=initiator;
  uiScheduleCmd.init_des.destination=PLAYER_DISPATCH_INIT_DEST_UI;
  uiScheduleCmd.data_count=count;
  uiScheduleCmd.data=data;


  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_SET_UI_SCHEDULE,
                              &uiScheduleCmd, NULL);
}
//-------------------------------------------------------------------------------------------------
//check Ok by Leon @2014/01/08
int dispatcher_get_ui_time(playerc_dispatcher_t *device,
		uint8_t *initiator,
		player_dispatcher_time_t *time,
		player_dispatcher_timezone_t *zone)
{
  *initiator = device->uiTime.init_des.initiator;
  *time = device->uiTime.data;
  *zone = device->uiTime.zone;

  return 0;
}
//-------------------------------------------------------------------------------------------------
//check Ok by Leon @2014/01/08
player_dispatcher_ui_alarm_time_t *
dispatcher_get_ui_schedule(playerc_dispatcher_t *device,
		uint8_t *initiator,
		uint32_t *count)
{//check OK
  *initiator = device->uiSchedule.init_des.initiator;
  *count = device->uiSchedule.data_count;

  return device->uiSchedule.data;
}
//-------------------------------------------------------------------------------------------------
int dispatcher_get_req_ui_time(playerc_dispatcher_t *device, uint8_t *initiator)
{
  *initiator=device->uiTime.init_des.initiator;

  return 0;
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_req_ack_ui_time(playerc_dispatcher_t *device,
		uint8_t destination,
		player_dispatcher_time_t data,
		player_dispatcher_timezone_t zone)
{
  player_dispatcher_ui_time_t uiTimeCmd;


  uiTimeCmd.init_des.initiator=PLAYER_DISPATCH_INIT_DEST_UI;
  uiTimeCmd.init_des.destination=destination;
  uiTimeCmd.data=data;
  uiTimeCmd.zone=zone;

  //send command to request ack UI time
  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_REQ_ACK_UI_TIME,
                              &uiTimeCmd, NULL);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_get_req_ui_schedule(playerc_dispatcher_t *device, uint8_t *initiator)
{
  *initiator=device->uiSchedule.init_des.initiator;

  return 0;
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_req_ack_ui_schedule(playerc_dispatcher_t *device,
		uint8_t destination,
		uint32_t count,
		player_dispatcher_ui_alarm_time_t data[])
{
  player_dispatcher_ui_schedule_t uiScheduleCmd;


  uiScheduleCmd.init_des.initiator=PLAYER_DISPATCH_INIT_DEST_UI;
  uiScheduleCmd.init_des.destination=destination;
  uiScheduleCmd.data_count=count;
  uiScheduleCmd.data=data;


  //send command to request ack UI time
  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_REQ_ACK_UI_SCHEDULE,
                              &uiScheduleCmd, NULL);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_file(playerc_dispatcher_t *device,
    uint8_t initiator,
    const int8_t *name,
    uint32_t name_size,
    const int8_t *data,
    uint32_t data_size)
{
  player_dispatcher_file_t file;


  file.init_des.initiator=initiator;
  file.init_des.destination=PLAYER_DISPATCH_INIT_DEST_DIS;
  file.name_count=name_size;
  file.name=name;
  file.data_count=data_size;
  file.data=data;

  if(playerc_client_request(device->info.client, &device->info,
                            PLAYER_DISPATCHER_REQ_SET_FILE,
                            (void*)&file, NULL) < 0)
    return -1;

return 0;
}
//-------------------------------------------------------------------------------------------------
int8_t *  dispatcher_get_file(playerc_dispatcher_t *device,
    uint8_t initiator,
    const int8_t *name,
    uint32_t name_size,
    uint32_t *data_size)
{
  player_dispatcher_file_t file;
  player_dispatcher_file_t *getFile;



  file.init_des.initiator=initiator;
  file.init_des.destination=PLAYER_DISPATCH_INIT_DEST_DIS;
  file.name_count=name_size;
  file.name=name;
  file.data_count=0;
  file.data=NULL;

  if(playerc_client_request(device->info.client, &device->info,
                            PLAYER_DISPATCHER_REQ_GET_FILE,
                            (void*)&file, (void**)&getFile) < 0)
    return(NULL);

  if ( device->fileData!=NULL )
    free( device->fileData );

  device->fileData=malloc(getFile->data_count);
  memcpy(device->fileData, getFile->data, getFile->data_count);
  *data_size=getFile->data_count;
  player_dispatcher_file_t_free(getFile);


  return device->fileData;
}
//-------------------------------------------------------------------------------------------------
//return the pairing result if
//data <0 is playerc_client_request error
//data =0 is playerc_client_request OK
//data >0 is playerc_client_request success but pairing error
int dispatcher_start_pairing(playerc_dispatcher_t *device,
		uint8_t initiator,
    const uint8_t *mac)
{
  player_dispatcher_mac_t pairingCmd;
  player_dispatcher_mac_t *pairing;
  uint32_t result;


  pairingCmd.init_des.initiator=initiator;
  pairingCmd.init_des.destination=PLAYER_DISPATCH_INIT_DEST_DIS;
  pairingCmd.mac_count=strlen(mac);
  pairingCmd.mac=mac;
  pairingCmd.result=0;

  //send command to request pairing
  if(playerc_client_request(device->info.client, &device->info,
                            PLAYER_DISPATCHER_REQ_PAIRING,
                            (void*)&pairingCmd, (void**)&pairing) < 0)
    return -1;

  DEBUG("pairing->result=%d\n", pairing->result);
  result=pairing->result;
  player_dispatcher_mac_t_free(pairing);

  return result;
}
//-------------------------------------------------------------------------------------------------
//log_in: 1
//log_out: 0
//return the login logout result if
//data <0 is playerc_client_request error
//data =0 is playerc_client_request OK
//data >0 is playerc_client_request success but login or logout error
int dispatcher_log_in_out(playerc_dispatcher_t *device,
    uint8_t log_in_out,
    const uint8_t *mac)
{
  player_dispatcher_mac_t logInOutCmd;
  player_dispatcher_mac_t *logInOut;
  uint32_t result;


  logInOutCmd.init_des.initiator=0;
  logInOutCmd.init_des.destination=PLAYER_DISPATCH_INIT_DEST_DIS;
  logInOutCmd.mac_count=strlen(mac);
  logInOutCmd.mac=mac;
  logInOutCmd.result=0;




  //log in
  if ( log_in_out==1 )
  {
    //send command to request login
    if(playerc_client_request(device->info.client, &device->info,
                              PLAYER_DISPATCHER_REQ_LOGIN,
                              (void*)&logInOutCmd, (void**)&logInOut) < 0)
      return -1;
  }
  else  //log out
  {
    //send command to request logout
    //send command to request login
    if(playerc_client_request(device->info.client, &device->info,
                              PLAYER_DISPATCHER_REQ_LOGOUT,
                              (void*)&logInOutCmd, (void**)&logInOut) < 0)
    	return -1;
  }

  DEBUG("logInOut->result=%d\n", logInOut->result);
  result=logInOut->result;
  player_dispatcher_mac_t_free(logInOut);

  return result;
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_cmd_ui(playerc_dispatcher_t *device,
    uint8_t initiator,
    uint32_t cmd )
{
  player_dispatcher_ui_cmd_data_t uiCmdData;


  uiCmdData.init_des.initiator=initiator;
  uiCmdData.init_des.destination=PLAYER_DISPATCH_INIT_DEST_UI;
  uiCmdData.cmd=cmd;

  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_UI,
                              &uiCmdData, NULL);
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_ui_name(playerc_dispatcher_t *device,
    uint8_t initiator,
    uint8_t *name)
{
  player_dispatcher_ui_name_t uiNameCmd;


  memset(&uiNameCmd, 0, sizeof(uiNameCmd));
  uiNameCmd.init_des.initiator=initiator;
  uiNameCmd.init_des.destination=PLAYER_DISPATCH_INIT_DEST_UI;
  uiNameCmd.name_count = strlen(name)+1; //add '\0'
  uiNameCmd.name = name;

  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_SET_UI_NAME,
                              &uiNameCmd, NULL);
}
//-------------------------------------------------------------------------------------------------
uint8_t * dispatcher_get_ui_name(playerc_dispatcher_t *device,
    uint8_t *initiator)
{
  *initiator = device->uiName.init_des.initiator;
  return device->uiName.name;
}
//-------------------------------------------------------------------------------------------------
int dispatcher_get_req_ui_name(playerc_dispatcher_t *device,
    uint8_t *initiator)
{
  *initiator=device->uiName.init_des.initiator;

  return 0;
}
//-------------------------------------------------------------------------------------------------
int dispatcher_set_req_ack_ui_name(playerc_dispatcher_t *device,
    uint8_t destination,
    uint8_t *name)
{
  player_dispatcher_ui_name_t uiNameCmd;


  uiNameCmd.init_des.initiator=PLAYER_DISPATCH_INIT_DEST_UI;
  uiNameCmd.init_des.destination=destination;
  uiNameCmd.name_count = strlen(name)+1;//add '\0'
  uiNameCmd.name = name;

  //send command to request ack UI name
  return playerc_client_write(device->info.client, &device->info,
                              PLAYER_DISPATCHER_CMD_REQ_ACK_UI_NAME,
                              &uiNameCmd, NULL);
}
//-------------------------------------------------------------------------------------------------







