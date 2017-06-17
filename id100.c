/***********************************************************************************************************************
 *
 * ID100 Utility
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
#include <stdio.h>
#include <stdlib.h>
#include "app.h"
#include "utils.h"

int main(void)
{
  AppInit("/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_MECIDDVULMNCDVMP-if00-port0");

  AppVersionType version;
  AppGetVersion(&version);
  PrintBuffer(&version, sizeof(version), "Version: ");

  AppDateTimeType dateTime;
  AppGetDateTime(&dateTime);
  PrintBuffer(&dateTime, sizeof(dateTime), "Date and Time: ");

  AppIntensityType intensity;
  AppGetIntensity(&intensity);
  PrintBuffer(&intensity, sizeof(intensity), "Intensity: ");

  AppLastCalibrationType lastCalibration;
  AppGetLastCalibration(&lastCalibration);
  PrintBuffer(&lastCalibration, sizeof(lastCalibration), "Calibration: ");

  AppStandbyType standby;
  AppGetStandby(&standby);
  PrintBuffer(&standby, sizeof(standby), "Standby: ");

  FlashConfigPageType config;
  AppGetFlashConfigPage(14400, &config);
  PrintBuffer(&config, sizeof(config), "Config: ");

  AppointmentsConfigType appointments;
  AppGetAppointments(&appointments);
  PrintBuffer(&appointments, sizeof(appointments), "Appointments: ");

  AppCleanup();

  return EXIT_SUCCESS;
}
