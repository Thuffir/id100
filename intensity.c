/***********************************************************************************************************************
 *
 * ID100 Utility
 * Intensity Functions
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
#include <stdlib.h>
#include "intensity.h"
#include "file.h"
#include "app.h"
#include "utils.h"

// Intensity mapping
static const AppIntensityType intensityMap[] = {
  AppIntensity1,
  AppIntensity2,
  AppIntensity3,
  AppIntensity4,
  AppIntensity5,
  AppIntensity6,
  AppIntensity7,
  AppIntensity8,
  AppIntensity9,
};
static const uint8_t intensityNumberOf = sizeof(intensityMap) / sizeof(intensityMap[0]);

/***********************************************************************************************************************
 * Print Intensity
 **********************************************************************************************************************/
void IntensityPrint(char *filename, char *device)
{
  // Get intensity
  AppInit(device);
  AppIntensityType intensity = AppGetIntensity();
  AppCleanup();

  // Convert it to number
  uint8_t idx;
  for(idx = 0; idx < intensityNumberOf; idx++) {
    if(intensityMap[idx] == intensity) {
      break;
    }
  }

  // Check it
  if(idx >= intensityNumberOf) {
    ExitWithError("Invalid intensity %u", intensity);
  }

  // Print it
  FILE *file = FileOpen(filename, true);
  fprintf(file, "%u\n", idx + 1);
  FileClose(file);
}

/***********************************************************************************************************************
 * Set Intensity
 **********************************************************************************************************************/
void IntensitySet(char *device, char *intensityParam)
{
  // Convert and check intensity to int
  uint8_t idx = atoi(intensityParam) - 1;
  if(idx >= intensityNumberOf) {
    ExitWithError("Invalid intensity: %s", intensityParam);
  }

  // Convert to intensity and set it
  AppInit(device);
  AppSetIntensity(intensityMap[idx]);
  AppCleanup();
}
