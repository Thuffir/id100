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

// Macro to correct endianness (ID100 is Big Endian)
#if(__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
#define APP_SWAP_ENDIAN_16(num) ((num) = ((num) << 8) | ((num) >> 8))
#else
#define APP_SWAP_ENDIAN_16(num)
#endif

// NUmber of flash pages holding the clock configuration
#define APP_CLOCK_CONFIG_FLASH_PAGES 14400

/***********************************************************************************************************************
 * Send command and data to link layer and receive answer from it
 **********************************************************************************************************************/
static void AppSendAndReceive(const uint8_t command, const void *sendBuf, const uint16_t sendBufLen,
  void *recvBuf, const uint16_t recvBufLen)
{
  uint8_t recvCmd;
  uint16_t recvLength;

  // Send command and optional data
  LinkSendCommandAndBuffer(command, sendBuf, sendBufLen);

  // Receive answer
  recvLength = LinkReceiveCommandAndBuffer(&recvCmd, recvBuf, recvBufLen);
  // Check the received command
  if(recvCmd != command) {
    ExitWithError("Invalid answer command received: '%c'", recvCmd);
  }
  // Check received buffer length
  if(recvLength != recvBufLen) {
    ExitWithError("Invalid length received: %u", recvLength);
  }
}

/***********************************************************************************************************************
 * Initialize the application and lower layers
 **********************************************************************************************************************/
void AppInit(void *ctx)
{
  LinkConnect(ctx);
}

/***********************************************************************************************************************
 * Clean up the application and lower layers
 **********************************************************************************************************************/
void AppCleanup(void)
{
  LinkDisconnect();
}

/***********************************************************************************************************************
 * Get the firmware version
 **********************************************************************************************************************/
void AppGetVersion(AppVersionType *version)
{
  AppSendAndReceive('v', NULL, 0, version, sizeof(*version));
  APP_SWAP_ENDIAN_16(version->major);
  APP_SWAP_ENDIAN_16(version->minor);
  APP_SWAP_ENDIAN_16(version->revision);
}

/***********************************************************************************************************************
 * Get the actual date and time
 **********************************************************************************************************************/
void AppGetDateTime(AppDateTimeType *dateTime)
{
  AppSendAndReceive('t', NULL, 0, dateTime, sizeof(*dateTime));
}

/***********************************************************************************************************************
 * Set the actual date and time
 **********************************************************************************************************************/
void AppSetDateTime(const AppDateTimeType *dateTime)
{
  AppSendAndReceive('T', dateTime, sizeof(*dateTime), NULL, 0);
}

/***********************************************************************************************************************
 * Set normal mode
 **********************************************************************************************************************/
void AppSetNormalMode(void)
{
  AppSendAndReceive('A', NULL, 0, NULL, 0);
}

/***********************************************************************************************************************
 * Set preview mode
 **********************************************************************************************************************/
void AppSetPreviewMode(void)
{
  AppSendAndReceive('a', NULL, 0, NULL, 0);
}

/***********************************************************************************************************************
 * Factory Reset
 **********************************************************************************************************************/
void AppFactoryReset(void)
{
  AppSendAndReceive('X', NULL, 0, NULL, 0);
}

/***********************************************************************************************************************
 * Activate Bootloader
 **********************************************************************************************************************/
void AppActivateBootloader(void)
{
  AppSendAndReceive('!', NULL, 0, NULL, 0);
}

/***********************************************************************************************************************
 * Set preview picture matrix
 **********************************************************************************************************************/
void AppSetPreviewMatrix(const AppMatrixBitmapType *matrix)
{
  AppSendAndReceive('D', matrix, sizeof(*matrix), NULL, 0);
}

/***********************************************************************************************************************
 * Get the standard intensity
 **********************************************************************************************************************/
void AppGetIntensity(AppIntensityType *intensity)
{
  AppSendAndReceive('b', NULL, 0, intensity, sizeof(*intensity));
}

/***********************************************************************************************************************
 * Set the standard intensity
 **********************************************************************************************************************/
void AppSetIntensity(const AppIntensityType *intensity)
{
  AppSendAndReceive('B', intensity, sizeof(*intensity), NULL, 0);
}

/***********************************************************************************************************************
 * Get last calibration data
 **********************************************************************************************************************/
void AppGetLastCalibration(AppLastCalibrationType *lastCalibration)
{
  AppSendAndReceive('c', NULL, 0, lastCalibration, sizeof(*lastCalibration));
}

/***********************************************************************************************************************
 * Set RTC Calibration value
 **********************************************************************************************************************/
void AppSetRtcCalibration(const AppRtcCalibrationValueType *calibration)
{
  AppSendAndReceive('C', calibration, sizeof(*calibration), NULL, 0);
}

/***********************************************************************************************************************
 * Get Standby times
 **********************************************************************************************************************/
void AppGetStandby(AppStandbyType *standby)
{
  AppSendAndReceive('s', NULL, 0, standby, sizeof(*standby));
}

/***********************************************************************************************************************
 * Set Standby times
 **********************************************************************************************************************/
void AppSetStandby(const AppStandbyType *standby)
{
  AppSendAndReceive('S', standby, sizeof(*standby), NULL, 0);
}

/***********************************************************************************************************************
 * Get Flash Clock configuration
 **********************************************************************************************************************/
void AppGetFlashClockConfig(uint16_t pageNumber, AppClockConfigPageReadType *clockConfig)
{
  // Check page number
  if(pageNumber >= APP_CLOCK_CONFIG_FLASH_PAGES) {
    ExitWithError("Invalid flash page number: %u", pageNumber);
  }

  APP_SWAP_ENDIAN_16(pageNumber);
  AppSendAndReceive('f', &pageNumber, sizeof(pageNumber), clockConfig, sizeof(*clockConfig));
  APP_SWAP_ENDIAN_16(pageNumber);
  APP_SWAP_ENDIAN_16(clockConfig->clockConfigPage.pageNumber);

  if(clockConfig->clockConfigPage.pageNumber != pageNumber) {
    ExitWithError("Bad page number: %u", clockConfig->clockConfigPage.pageNumber);
  }
}

/***********************************************************************************************************************
 * Set Flash Clock configuration
 **********************************************************************************************************************/
void AppSetFlashClockConfig(AppClockConfigPageType *clockConfig)
{
  uint16_t pageNumber;

  // Check page number
  if(clockConfig->pageNumber >= APP_CLOCK_CONFIG_FLASH_PAGES) {
    ExitWithError("Invalid flash page number: %u", clockConfig->pageNumber);
  }

  APP_SWAP_ENDIAN_16(clockConfig->pageNumber);
  AppSendAndReceive('F', clockConfig, sizeof(*clockConfig), &pageNumber, sizeof(pageNumber));
  APP_SWAP_ENDIAN_16(clockConfig->pageNumber);
  APP_SWAP_ENDIAN_16(pageNumber);

  if(pageNumber != clockConfig->pageNumber) {
    ExitWithError("Bad page number: %u", pageNumber);
  }
}
