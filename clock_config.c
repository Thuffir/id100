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
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "utils.h"
#include "file.h"
#include "clock_config.h"
#include "bitmap.h"

/***********************************************************************************************************************
 * Save clock config from flash
 **********************************************************************************************************************/
void ClockConfigSave(char *filename, char *device, FILE *tty)
{
  // Open file
  FILE *file = FileOpen(filename, true);

  // Check if someone is trying to write the config to the terminal
  if((file == stdout) && isatty(STDOUT_FILENO)) {
    ExitWithError("Won't write binary data to terminal.");
  }

  // Init Device
  AppInit(device);

  uint16_t page;
  // Save all pages
  for(page = 0; page < APP_CLOCK_CONFIG_FLASH_PAGES; page++) {
    AppFlashConfigPageType config;

    fprintf(tty, "Saving page %u of %u (%u%%)...\r", page + 1, APP_CLOCK_CONFIG_FLASH_PAGES,
      ((uint32_t)page + 1) * 100 / APP_CLOCK_CONFIG_FLASH_PAGES);
    fflush(tty);

    // Get Page
    AppGetFlashConfigPage(page, &config);
    // Write page
    FileWrite(file, &config.config.clockConfig, sizeof(config.config.clockConfig));
  }
  fprintf(tty, "\n");

  // Close device
  AppCleanup();
  // Close File
  FileClose(file);
}

/***********************************************************************************************************************
 * Load clock config into flash
 **********************************************************************************************************************/
void ClockConfigLoad(char *filename, char *device, FILE *tty)
{
  // Open file
  FILE *file = FileOpen(filename, false);
  // Open device
  AppInit(device);

  uint16_t page;
  // Load all pages
  for(page = 0; page < APP_CLOCK_CONFIG_FLASH_PAGES; page++) {
    AppFlashConfigPageType config;

    fprintf(tty, "Loading page %u of %u (%u%%)...\r", page + 1, APP_CLOCK_CONFIG_FLASH_PAGES,
      ((uint32_t)page + 1) * 100 / APP_CLOCK_CONFIG_FLASH_PAGES);
    fflush(tty);

    // Read page
    FileRead(file, &config.config.clockConfig, sizeof(config.config.clockConfig));
    // Set page number
    config.pageNumber = page;
    // Write page into device
    AppSetFlashConfig(&config);
  }
  fprintf(tty, "\n");

  AppCleanup();

  // Close File
  FileClose(file);
}

/***********************************************************************************************************************
 * Print clock configuration as ASCII
 **********************************************************************************************************************/
void ClockConfigPrint(char *device)
{
  // Init Device
  AppInit(device);

  uint32_t abssecond = 0;
  uint16_t page;
  // All pages
  for(page = 0; page < APP_CLOCK_CONFIG_FLASH_PAGES; page++) {
    uint8_t pagesec;
    AppFlashConfigPageType config;
    // Get Page
    AppGetFlashConfigPage(page, &config);

    // One page holds multiple clock configs
    for(pagesec = 0;
        pagesec < (sizeof(config.config.clockConfig.matrixBitmap) / sizeof(config.config.clockConfig.matrixBitmap[0]));
        pagesec++) {

      // Calculate time
      uint8_t hour   = abssecond / (60 * 60);
      uint8_t minute = (abssecond / 60) % 60;
      uint8_t second = abssecond % 60;

      // Print header
      printf("# %02u:%02u:%02u (%u,%u)\n", hour, minute, second, page + 1, pagesec + 1);
      // Print bitmap
      BitmapPrint(config.config.clockConfig.matrixBitmap[pagesec]);

      abssecond++;
    }
  }

  // Close device
  AppCleanup();
}

