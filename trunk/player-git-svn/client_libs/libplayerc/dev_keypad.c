#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "playerc.h"
#include "error.h"


//-------------------------------------------------------------------------------------------------
// Process incoming data
void playerc_keypad_putmsg(playerc_keypad_t *device, player_msghdr_t *header, uint8_t *data, size_t len);
//-------------------------------------------------------------------------------------------------
playerc_keypad_t *keypad_create (playerc_client_t *client, int index)
{
  playerc_keypad_t *device;

  device = (playerc_keypad_t*) malloc (sizeof (playerc_keypad_t));
  memset (device, 0, sizeof (playerc_keypad_t));
  playerc_device_init (&device->info, client, PLAYER_KEYPAD_CODE, index,
                       (playerc_putmsg_fn_t) playerc_keypad_putmsg);

  return device;
}
//-------------------------------------------------------------------------------------------------
void keypad_destroy (playerc_keypad_t *device)
{
  playerc_device_term (&device->info);
  free (device);
}
//-------------------------------------------------------------------------------------------------
int keypad_subscribe (playerc_keypad_t *device, int access)
{
  return playerc_device_subscribe (&device->info, access);
}
//-------------------------------------------------------------------------------------------------
int keypad_unsubscribe (playerc_keypad_t *device)
{
  return playerc_device_unsubscribe (&device->info);
}
//-------------------------------------------------------------------------------------------------
void playerc_keypad_putmsg(playerc_keypad_t *device, player_msghdr_t *header, uint8_t *data, size_t len)
{
  if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_KEYPAD_DATA_STATE) )
  {
    player_keypad_data_t *keyData = (player_keypad_data_t *)data;
    assert(header->size > 0);

    device->keyPadData=(*keyData);
  }//end if ( (header->type == PLAYER_MSGTYPE_DATA) && (header->subtype == PLAYER_KEYPAD_DATA_STATE) )
  else
     PLAYERC_WARN2("skipping keypad message with unknown type/subtype: %s/%d\n",
                  msgtype_to_str(header->type), header->subtype);
}
//-------------------------------------------------------------------------------------------------
int keypad_get_data(playerc_keypad_t *device, uint32_t *timestamp, uint8_t *key, uint8_t *status)
{
  *timestamp=device->keyPadData.timestamp;
  *key=device->keyPadData.key;
  *status=device->keyPadData.status;

  return 0;
}
//-------------------------------------------------------------------------------------------------
int keypad_get_config(playerc_keypad_t *device, uint32_t *de_bounce)
{
  player_keypad_config_t *config;

  if(playerc_client_request(device->info.client, &device->info,
                            PLAYER_KEYPAD_REQ_SET_CONFIG,
                            NULL, (void**)&config) < 0)
    return(-1);

  *de_bounce= config->de_bounce;
  player_keypad_config_t_free(config);

  return 0;
}
//-------------------------------------------------------------------------------------------------
int keypad_set_config(playerc_keypad_t *device, uint32_t de_bounce)
{
  player_keypad_config_t config;

  config.de_bounce=de_bounce;

  if(playerc_client_request(device->info.client, &device->info,
                            PLAYER_KEYPAD_REQ_SET_CONFIG,
                            (void*)&config, NULL) < 0)
    return -1;

return 0;
}
//-------------------------------------------------------------------------------------------------







