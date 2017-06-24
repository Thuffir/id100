/***********************************************************************************************************************
 *
 * ID100 Utility
 * Display Functions
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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "display.h"
#include "app.h"
#include "file.h"
#include "bitmap.h"
#include "utils.h"

/***********************************************************************************************************************
 * Set display to normal (clock) mode
 **********************************************************************************************************************/
void DisplaySetNormalMode(char *device)
{
  AppInit(device);
  AppSetNormalMode();
  AppCleanup();
}

/***********************************************************************************************************************
 * Show user content
 **********************************************************************************************************************/
void DisplayShowContent(char *filename, bool binary, char *device, char dotchar, char commentchar, uint32_t delay, uint32_t repeat)
{
  // Open file
  FILE *file = FileOpen(filename, false);

  // Check if we are writing binary data
  if(binary) {
    FileCheckBinaryTerminal(file);
  }

  // Make delay microseconds
  delay *=1000;

  // Init device
  AppInit(device);

  AppMatrixBitmapType bitmap;
  bool once = true;
  uint8_t size;
  while(repeat--) {
    // Read all frames
    for(;;) {
      // Read next frame
      size = binary ?
          fread(bitmap, 1, sizeof(bitmap), file) :
          BitmapRead(file, bitmap, dotchar, commentchar);

      // If no more frames
      if(size == 0) {
        break;
      }

      // Check if we have all data
      if(size != (binary ? sizeof(bitmap) : BITMAP_ROWS)) {
        ExitWithError("Invalid bitmap size: %u", size);
      }

      // Transmit frame
      AppSetPreviewMatrix(bitmap);
      // We set the preview mode after the first frame to avoid flicker
      if(once) {
        AppSetPreviewMode();
        once = false;
      }
      usleep(delay);
    }
    rewind(file);
  }

  // Cleanup
  AppCleanup();
  FileClose(file);
}
