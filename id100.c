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
#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "utils.h"

/***********************************************************************************************************************
 * Open file
 **********************************************************************************************************************/
static FILE *OpenFile(char *filename, bool openWrite)
{
  // File
  FILE *file;

  // Open file
  if(filename != NULL) {
    if((file = fopen(filename, openWrite ? "wb" : "rb")) == NULL) {
      ExitWithError("Unable to open file: %s", filename);
    }
  }
  else {
    file = openWrite ? stdout : stdin;
  }

  return file;
}

/***********************************************************************************************************************
 * Close file
 **********************************************************************************************************************/
static void CloseFile(FILE *file)
{
  // Close File
  if((file != stdout) && (file != stdin)) {
    if(fclose(file) != 0) {
      ExitWithError("Unable to close file");
    }
  }
}

/***********************************************************************************************************************
 * Save clock config from flash
 **********************************************************************************************************************/
static void SaveClockConfig(char *filename, char *device, FILE *tty)
{
  // Open file
  FILE *file = OpenFile(filename, true);

  // Check if someone is trying to write the config to the terminal
  if((file == stdout) && isatty(STDOUT_FILENO)) {
    ExitWithError("Won't write binary data to terminal.");
  }

  AppInit(device);

  // Save all pages
  uint16_t page;
  for(page = 0; page < APP_CLOCK_CONFIG_FLASH_PAGES; page++) {
    AppFlashConfigPageType config;

    fprintf(tty, "Saving page %u of %u (%u%%)...\r", page + 1, APP_CLOCK_CONFIG_FLASH_PAGES,
      ((uint32_t)page + 1) * 100 / APP_CLOCK_CONFIG_FLASH_PAGES);
    fflush(tty);

    // Get Page
    AppGetFlashConfigPage(page, &config);
    // Write page
    if(fwrite(&config.config.clockConfig, sizeof(config.config.clockConfig), 1, file) != 1) {
      ExitWithError("Unable to write %u bytes", sizeof(config.config.clockConfig));
    }
  }
  fprintf(tty, "\n");

  AppCleanup();

  // Close File
  CloseFile(file);
}

/***********************************************************************************************************************
 * Load clock config into flash
 **********************************************************************************************************************/
static void LoadClockConfig(char *filename, char *device, FILE *tty)
{
  // Open file
  FILE *file = OpenFile(filename, false);

  AppInit(device);

  // Save all pages
  uint16_t page;
  for(page = 0; page < APP_CLOCK_CONFIG_FLASH_PAGES; page++) {
    AppFlashConfigPageType config;

    fprintf(tty, "Loading page %u of %u (%u%%)...\r", page + 1, APP_CLOCK_CONFIG_FLASH_PAGES,
      ((uint32_t)page + 1) * 100 / APP_CLOCK_CONFIG_FLASH_PAGES);
    fflush(tty);

    // Read page
    if(fread(&config.config.clockConfig, sizeof(config.config.clockConfig), 1, file) != 1) {
      ExitWithError("Unable to read %u bytes", sizeof(config.config.clockConfig));
    }
    // Set page number and write page
    config.pageNumber = page;
    AppSetFlashConfig(&config);
  }
  fprintf(tty, "\n");

  AppCleanup();

  // Close File
  CloseFile(file);
}

/***********************************************************************************************************************
 * Main
 **********************************************************************************************************************/
int main(int numberOfArguments, char *arguments[])
{
  // Default device
  static const char defaultDevice[] =
      "/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_MECIDDVULMNCDVMP-if00-port0";
  // Device to use
  char *device = (char *)defaultDevice;
  // File name for input / output
  char *filename = NULL;
  // tty to print messages
  FILE *tty = NULL;

  // This tells us what to do
  enum {
    DoNoting,
    SaveCLockConfig,
    LoadCLockConfig
  } whatToDo = DoNoting;

  bool quiet = false;

  int option;
  // Check for options
  opterr = 0;
  while((option = getopt(numberOfArguments, arguments, "f:cCd:q")) != -1) {
    switch(option) {
      case 'f' : {
        filename = optarg;
      }
      break;

      case 'd' : {
        device = optarg;
      }
      break;

      case 'q' : {
        quiet = true;
      }
      break;

      case 'c' : {
        whatToDo = SaveCLockConfig;
      }
      break;

      case 'C' : {
        whatToDo = LoadCLockConfig;
      }
      break;

      // Bad arguments
      case '?':
      default: {
        fprintf(stderr, "Illegal option: '-%c'\n", optopt);
        whatToDo = DoNoting;
        goto ExitGetOpt;
      }
      break;
    }
  }
  ExitGetOpt:

  // Determine whether to print messages or not
  if(isatty(STDOUT_FILENO) && (quiet == false)) {
    tty = stdout;
  }
  else {
    if((tty = fopen("/dev/null", "wt")) == NULL) {
      ExitWithError("Unable to open /dev/null");
    }
  }

  // Decide what to do
  switch(whatToDo) {
    case SaveCLockConfig: {
      SaveClockConfig(filename, device, tty);
    }
    break;

    case LoadCLockConfig: {
      LoadClockConfig(filename, device, tty);
    }
    break;

    // Nothign to do
    default:
    case DoNoting: {
      // Print usage
      fprintf(stderr,
        "ID100 Utility ("__DATE__" "__TIME__")\n"
        "Usage:\n"
        " -d device     Use device instead of %s\n"
        " -f filename   Use file for input / output operations instead of STDIN / STDOUT\n"
        " -q            Be quiet\n"
        " -c            Save clock configuration from device\n"
        " -C            Load clock configuration into device\n"
        , defaultDevice
      );
    }
    break;
  }

  return EXIT_SUCCESS;
}
