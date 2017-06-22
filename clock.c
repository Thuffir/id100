/***********************************************************************************************************************
 *
 * ID100 Utility
 * Clock Functions
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
#include <time.h>
#include <stdbool.h>
#include "clock.h"
#include "app.h"
#include "file.h"

/***********************************************************************************************************************
 * Get Clock
 **********************************************************************************************************************/
void ClockGet(char *filename, char *device)
{
  // Weekdays
  static const char *weekDays[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Friday",
    "Saturday"
  };

  // Get date and time
  AppInit(device);
  AppDateTimeType dateTime;
  AppGetDateTime(&dateTime);
  AppCleanup();

  // Print it
  FILE *file = FileOpen(filename, true);
  fprintf(file, "%s, %04u.%02u.%02u %02u:%02u:%02u%s\n", weekDays[dateTime.weekDay],
    dateTime.year + 2000, dateTime.month, dateTime.day,
    dateTime.hour, dateTime.minute, dateTime.second,
    (dateTime.daylightSaving == AppSummerTime) ? " (DST)" :"");
  FileClose(file);
}

/***********************************************************************************************************************
 * Set clock to current time
 **********************************************************************************************************************/
void ClockSet(char *device)
{
  // Get system time
  time_t t = time(NULL);
  struct tm *unixTime = localtime(&t);

  // Convert it
  AppDateTimeType dateTime;
  dateTime.year           = unixTime->tm_year - 100;
  dateTime.month          = unixTime->tm_mon + 1;
  dateTime.day            = unixTime->tm_mday;
  dateTime.hour           = unixTime->tm_hour;
  dateTime.minute         = unixTime->tm_min;
  dateTime.second         = unixTime->tm_sec;
  dateTime.weekDay        = unixTime->tm_wday;
  dateTime.daylightSaving = unixTime->tm_isdst;

  // Set it
  AppInit(device);
  AppSetDateTime(&dateTime);
  AppCleanup();
}
