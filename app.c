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

static void AppRequestAndReceive(uint8_t command, void *sendBuf, uint16_t sendBufLen, void *recvBuf, uint16_t recvBufLen)
{
  uint8_t recvCmd;
  uint16_t recvLength;

  // Send command and optional data
  LinkSendCommandAndBuffer(command, sendBuf, sendBufLen);

  recvLength = LinkReceiveCommandAndBuffer(&recvCmd, recvBuf, recvBufLen);
  // Check received command
  if(recvCmd != command) {
    ExitWithError("Invalid answer command received: '%c'", recvCmd);
  }
  // Check received buffer length
  if(recvLength != recvBufLen) {
    ExitWithError("Invalid length received: %u", recvLength);
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

void AppGetVersion(AppVersionType *version)
{
  AppRequestAndReceive('v', NULL, 0, version, sizeof(*version));
}

void AppGetDateTime(AppDateTimeType *dateTime)
{
  AppRequestAndReceive('t', NULL, 0, dateTime, sizeof(*dateTime));
}
