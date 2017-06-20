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
#include "file.h"
#include "clock_config.h"

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
  char *timestamp = NULL;

  // This tells us what to do
  enum {
    DoNoting,
    SaveCLockConfig,
    LoadCLockConfig,
    PrintCLockConfig
  } whatToDo = DoNoting;

  bool quiet = false;

  int option;
  // Check for options
  opterr = 0;
  while((option = getopt(numberOfArguments, arguments, "c::C::d:qp::")) != -1) {
    switch(option) {
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
        filename = optarg;
      }
      break;

      case 'C' : {
        whatToDo = LoadCLockConfig;
        filename = optarg;
      }
      break;

      case 'p' : {
        timestamp = optarg;
        whatToDo = PrintCLockConfig;
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
      ClockConfigSave(filename, device, tty);
    }
    break;

    case LoadCLockConfig: {
      ClockConfigLoad(filename, device, tty);
    }
    break;

    case PrintCLockConfig: {
      if(timestamp == NULL) {
        ClockConfigPrintAll(device);
      }
      else {
        ClockConfigPrint(device, timestamp);
      }
    }
    break;

    // Nothign to do
    default:
    case DoNoting: {
      // Print usage
      fprintf(stderr,
        "ID100 Utility ("__DATE__" "__TIME__")\n"
        "Usage:\n"
        " -ddevice      Use device instead of %s\n"
        " -q            Be quiet\n"
        " -c[file]      Save clock configuration from device to file or stdout\n"
        " -C[file]      Load clock configuration into device from file or stdin\n"
        " -p[hh:mm:ss]  Print clock configuration as ASCII for a given time\n"
        , defaultDevice
      );
    }
    break;
  }

  return EXIT_SUCCESS;
}
