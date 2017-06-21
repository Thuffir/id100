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
  // Time stamp
  char *timestamp = "00:00:00-23:59:59";
  // Dot character for ASCII pictures
  char dotchar = '#';
  // Comment character
  char commentchar = ':';

  // This tells us what to do
  enum {
    DoNoting,
    ReadClockConfig,
    WriteClockConfig,
    SetDisplay
  } whatToDo = DoNoting;

  int option;
  // Check for options
  opterr = 0;
  while((option = getopt(numberOfArguments, arguments, "bcCd:f:st:")) != -1) {
    switch(option) {
      case 'b' : {
        dotchar = 0;
      }
      break;

      case 'd' : {
        device = optarg;
      }
      break;

      case 'f' : {
        filename = optarg;
      }
      break;

      case 't' : {
        timestamp = optarg;
      }
      break;

      case 'c' : {
        whatToDo = ReadClockConfig;
      }
      break;

      case 'C' : {
        whatToDo = WriteClockConfig;
      }
      break;

        case 's' : {
        whatToDo = SetDisplay;
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

  // Decide what to do
  switch(whatToDo) {
    case ReadClockConfig: {
      ClockConfigRead(filename, device, timestamp, dotchar, commentchar);
    }
    break;

    case WriteClockConfig: {
    }
    break;

    case SetDisplay: {
    }
    break;

    // Nothign to do
    default:
    case DoNoting: {
      // Print usage
      fprintf(stderr,
        "ID100 Utility ("__DATE__" "__TIME__")\n"
        "Usage:\n"
        " -d device               Use device instead of %s\n"
        " -f file                 Use filename for input / output\n"
        " -t hh:mm:ss[-hh:mm:ss]  Specify time or time range\n"
        " -c                      Read clock configuration from device\n"
//        " -C                      Write clock configuration into device\n"
//        " -s                      Set display contents\n"
        , defaultDevice
      );
    }
    break;
  }

  return EXIT_SUCCESS;
}
