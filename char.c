/***********************************************************************************************************************
 *
 * ID100 Utility
 * Character Generation Functions
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
#include <string.h>
#include "utils.h"
#include "file.h"
#include "char.h"
#include "app.h"
#include "bitmap.h"

#define CHAR_WIDTH    3
#define CHAR_HEIGTH   5

// MAx characters per line
#define CHAR_PER_LINE (BITMAP_COLS / (CHAR_WIDTH + 1))

/***********************************************************************************************************************
 * Put a character to a requested position
 **********************************************************************************************************************/
void CharPutChar(AppMatrixBitmapType bitmap, uint8_t ascii, uint8_t row, uint8_t column)
{
  // This holds the whole character set as bitmap, 16 bit per character
  static const uint16_t charSet[] = {
#include "charset.h"
  };

  // Limit ascii char
  if((ascii < ' ') || (ascii > '~')) {
    ascii = ' ';
  }

  // Character and bit mask
  uint16_t chr = charSet[ascii - ' '], bitmask = 0x4000;
  uint8_t r, c;

  // For each row
  for(r = 0; r < CHAR_HEIGTH; r++) {
    // and each column
    for(c = 0; c < CHAR_WIDTH; c++) {
      uint8_t tr = r + row, tc = c + column;
      // Set dot if also set in the character bitmap
      if((chr & bitmask) && (tr < BITMAP_ROWS) && (tc < BITMAP_COLS)) {
        BitmapSetDot(bitmap, BitmapDotSet, tr, tc);
      }
      // Shift bit mask to get next dot
      bitmask >>= 1;
    }
  }
}


/***********************************************************************************************************************
 * Put text to a requested position
 **********************************************************************************************************************/
void CharPutText(AppMatrixBitmapType bitmap, char *text, uint8_t row, uint8_t column)
{
  uint8_t chrIdx, chr, col;
  for(chrIdx = 0; (chr = text[chrIdx]) && ((col = ((chrIdx * CHAR_PER_LINE) + column)) < BITMAP_COLS); chrIdx++ ) {
    CharPutChar(bitmap, text[chrIdx], row, col);
  }
}

/***********************************************************************************************************************
 * Overlay text(s) on a bitmap
 **********************************************************************************************************************/
void CharOverlayText(char *filename, bool binary, char *device, char *overlay, char dotchar, char commentchar)
{
  AppMatrixBitmapType bitmap;
  // Open file
  FILE *file = FileOpen(filename, false);

  // Read bitmap
  if(binary) {
    FileCheckBinaryTerminal(file);
    FileRead(file, bitmap, sizeof(bitmap));
  }
  else {
    if(BitmapRead(file, bitmap, dotchar, commentchar) != BITMAP_ROWS) {
      ExitWithError("Invalid bitmap");
    }
  }
  FileClose(file);

  char *opt;
  // Parse options
  for(opt = strtok(overlay, " "); opt; opt = strtok(NULL, " ")) {
    unsigned int row, col;
    char text[CHAR_PER_LINE + 1];
    // Parse overlay options
    if(sscanf(opt, "%u,%u,%4s", &row, &col, text) != 3) {
      ExitWithError("Bad option: '%s'", opt);
    }
    // Format text into bitmap
    CharPutText(bitmap, text, row, col);
  }

  // Show bitmap
  AppInit(device);
  AppSetPreviewMatrix(bitmap);
  AppSetPreviewMode();
  AppCleanup();
}
