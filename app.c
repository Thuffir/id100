/***********************************************************************************************************************
 *
 * ID100 Utility
 * Application Layer
 *
 * (C) 2017 Gergely Budai
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 **********************************************************************************************************************/
#include <stddef.h>
#include "app.h"
#include "utils.h"
#include "link.h"

typedef enum {
  APP_COMMAND_VERSION,
  APP_COMMAND_TIME_GET,

  APP_COMMAND_INVALID
} AppCommandType;

static struct __packed AppPacketStruct {
  uint8_t command;
  union {
    AppVersionType version;
    AppDateTimeType dateTime;
  } data;
} appPacket;

// Helper macro to get the length until the end of a specific member
#define APP_PACKET_GET_LENGTH_UNTIL(name) (offsetof(struct AppPacketStruct, name) + sizeof(((struct AppPacketStruct *)NULL)->name))

static void AppRequestAndReceive(AppCommandType command)
{
  static const struct {
    const uint8_t  commandByte;
    const uint16_t requestLength;
    const uint16_t answerLength;
  } commandInfos[] = {
    [APP_COMMAND_VERSION]   = { 'v', APP_PACKET_GET_LENGTH_UNTIL(command), APP_PACKET_GET_LENGTH_UNTIL(data.version) },
    [APP_COMMAND_TIME_GET]  = { 't', APP_PACKET_GET_LENGTH_UNTIL(command), APP_PACKET_GET_LENGTH_UNTIL(data.dateTime) },
  };

  uint16_t length;

  if(command >= APP_COMMAND_INVALID) {
    ExitWithError("Invalid command %u", command);
  }

  appPacket.command = commandInfos[command].commandByte;
  LinkSendBuffer(&appPacket, commandInfos[command].requestLength);

  length = LinkReceiveBuffer(&appPacket, sizeof(appPacket));
  if(length != commandInfos[command].answerLength) {
    ExitWithError("Invalid length received: %u", length);
  }
  if(appPacket.command != commandInfos[command].commandByte) {
    ExitWithError("Invalid answer received: '%c'", appPacket.command);
  }
}

void AppInit(void *ctx)
{
  LinkConnect(ctx);
}

void AppCleanup(void)
{
  LinkDisconnect();
}

AppVersionType *AppGetVersion(void)
{
  AppRequestAndReceive(APP_COMMAND_VERSION);
  return(&appPacket.data.version);
}

AppDateTimeType *AppGetDateTime(void)
{
  AppRequestAndReceive(APP_COMMAND_TIME_GET);
  return(&appPacket.data.dateTime);
}
