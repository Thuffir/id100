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
#include <unistd.h>
#include "app.h"
#include "utils.h"

// Device to use
static char *device = "/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_MECIDDVULMNCDVMP-if00-port0";
// File name for input / output
static char *filename = NULL;

/***********************************************************************************************************************
 * Save flash config
 **********************************************************************************************************************/
static void SaveClockConfig(void)
{
  // File
  FILE *file = stdout;

  // Open file
  if(filename != NULL) {
    if((file = fopen(filename, "wb")) == NULL) {
      ExitWithError("Unable to open file: %s", filename);
    }
  }

  AppInit(device);

  // Save all pages
  uint16_t page;
  for(page = 0; page < APP_CLOCK_CONFIG_FLASH_PAGES; page++) {
    AppFlashConfigPageType config;
    // Get Page
    AppGetFlashConfigPage(page, &config);
    if(file != stdout) {
      printf("Saving page %u of %u (%u%%)...\r", page, APP_CLOCK_CONFIG_FLASH_PAGES, (uint32_t)page * 100 / APP_CLOCK_CONFIG_FLASH_PAGES);
      fflush(stdout);
    }
    // Write page
    if(fwrite(&config.config.clockConfig, sizeof(config.config.clockConfig), 1, file) != 1) {
      ExitWithError("Unable to write %u bytes", sizeof(config.config.clockConfig));
    }
  }
  if(file != stdout) {
    printf("\n");
  }

  AppCleanup();

  // Close File
  if(file != stdout) {
    if(fclose(file) != 0) {
      ExitWithError("Unable to close file: %s", filename);
    }
  }
}

/***********************************************************************************************************************
 * Main
 **********************************************************************************************************************/
int main(int numberOfArguments, char *arguments[])
{
  // This tells us what to do
  enum {
    DoNoting,
    SaveCLockConfig
  } whatToDo = DoNoting;

  int option;
  // Check for options
  opterr = 0;
  while((option = getopt(numberOfArguments, arguments, "f:cd:")) != -1) {
    switch(option) {
      case 'f' : {
        filename = optarg;
      }
      break;

      case 'd' : {
        device = optarg;
      }
      break;

      case 'c' : {
        whatToDo = SaveCLockConfig;
      }
      break;

      // Bad arguments
      case '?':
      default: {
        printf("Illegal option: '%c'\n", optopt);
        whatToDo = DoNoting;
        goto ExitGetOpt;
      }
      break;
    }
  }
  ExitGetOpt:

  // Decide what to do
  switch(whatToDo) {
    case SaveCLockConfig: {
      SaveClockConfig();
    }
    break;

    // Nothign to do
    default:
    case DoNoting: {
      // Print usage
      printf(
        "ID100 Utility ("__DATE__" "__TIME__")\n"
        "Usage:\n"
        " -d device     Use device instead of /dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_MECIDDVULMNCDVMP-if00-port0\n"
        " -f filename   Use file for input / output operations instead of STDIN / STDOUT\n"
        " -c            Save clock configuration\n"
      );
    }
    break;
  }

  return EXIT_SUCCESS;
}
