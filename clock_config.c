/***********************************************************************************************************************
 *
 * ID100 Utility
 * Clock Configuration
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
#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "utils.h"
#include "file.h"
#include "clock_config.h"
#include "bitmap.h"

/***********************************************************************************************************************
 * Parse time string to absolute seconds
 **********************************************************************************************************************/
static void ClockConfigParseTime(char *timestamp, uint32_t *fromAbsSec, uint32_t *toAbsSec)
{
  unsigned int fromHour, fromMinute, fromSecond, toHour, toMinute, toSecond;

  // Parse time, try with from - to
  if(sscanf(timestamp, "%u:%u:%u-%u:%u:%u", &fromHour, &fromMinute, &fromSecond, &toHour, &toMinute, &toSecond) != 6) {
    // If fails try with exact time
    if(sscanf(timestamp, "%u:%u:%u", &fromHour, &fromMinute, &fromSecond) != 3) {
      // Error if it also fails
      ExitWithError("Invalid timestamp: %s", timestamp);
    }
    else {
      // Exact time, make end time equal to start time
      toHour = fromHour;
      toMinute = fromMinute;
      toSecond = fromSecond;
    }
  }

  // Calculate absolte seconds
  *fromAbsSec = (fromHour * 60 * 60) + (fromMinute * 60) + fromSecond;
  *toAbsSec = (toHour * 60 * 60) + (toMinute * 60) + toSecond;

  // Check if we are in bounds
  if((*fromAbsSec >= (60 * 60 *24)) || (*toAbsSec >= (60 * 60 *24))) {
    ExitWithError("Invalid timestamp");
  }
}

/***********************************************************************************************************************
 * Read Clock Configuration from device
 **********************************************************************************************************************/
void ClockConfigRead(char *filename, char *device, char *timestamp, char dotchar, char commentchar)
{
  uint32_t secIdx, to;

  // Convert timestamp to page information
  ClockConfigParseTime(timestamp, &secIdx, &to);

  // Open file
  FILE *file = FileOpen(filename, true);

  // Check if we are writing binary data
  if(dotchar == 0) {
    FileCheckBinaryTerminal(file);
  }

  // Init device
  AppInit(device);

  uint16_t oldPage = -1;
  for(; secIdx <= to; secIdx++) {
    AppFlashConfigPageType config;
    uint16_t page = secIdx / APP_CLOCK_CONFIG_PER_PAGES;
    uint8_t pageSec = secIdx % APP_CLOCK_CONFIG_PER_PAGES;

    // Load page if necessary
    if(page != oldPage) {
      AppGetFlashConfigPage(page, &config);
      oldPage = page;
    }

    // Check if we are writing binary data
    if(dotchar == 0) {
      FileWrite(file,
        config.config.clockConfig.matrixBitmap[pageSec], sizeof(config.config.clockConfig.matrixBitmap[pageSec]));
    }
    else {
      // Calculate time
      uint8_t hour   = secIdx / (60 * 60);
      uint8_t minute = (secIdx / 60) % 60;
      uint8_t second = secIdx % 60;
      // Print header
      fprintf(file, "%c %02u:%02u:%02u %u,%u\n", commentchar, hour, minute, second, page, pageSec);
      // Print Config
      BitmapPrint(file, config.config.clockConfig.matrixBitmap[pageSec], dotchar);
    }
  }

  // Cleanup
  AppCleanup();
  FileClose(file);
}

/***********************************************************************************************************************
 * Write Clock Configuration into device
 **********************************************************************************************************************/
void ClockConfigWrite(char *filename, char *device, char dotchar, char commentchar)
{
  // Open file
  FILE *file = FileOpen(filename, false);

  // Check if we are writing binary data
  if(dotchar == 0) {
    FileCheckBinaryTerminal(file);
  }

  // Init device
  AppInit(device);

  // Loop all pages
  uint16_t page;
  for(page = 0; page < APP_CLOCK_CONFIG_FLASH_PAGES; page++) {
    AppFlashConfigPageType config;

    // Erase pages if necessary
    if((page % APP_FLASH_PAGES_PER_SECTOR) == 0) {
      AppEraseFlashConfigSector(page);
    }

    // Read config data page
    if(dotchar == 0) {
      FileRead(file, &(config.config.clockConfig), sizeof(config.config.clockConfig));
    }
    else {
      uint8_t pagesec;
      for(pagesec = 0; pagesec < APP_CLOCK_CONFIG_PER_PAGES; pagesec++) {
        if(BitmapRead(file, config.config.clockConfig.matrixBitmap[pagesec], dotchar, commentchar) != false) {
          ExitWithError("Invalid Input");
        }
      }
    }

    // Set page number and write page
    config.pageNumber = page;
    AppSetFlashConfig(&config);
  }

  // Cleanup
  AppCleanup();
  FileClose(file);
}
