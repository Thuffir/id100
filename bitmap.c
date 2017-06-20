/***********************************************************************************************************************
 *
 * ID100 Utility
 * Matrix Bitmap Functions
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
#include "bitmap.h"

/***********************************************************************************************************************
 * Print a bitmap as ACII
 **********************************************************************************************************************/
void BitmapPrint(AppMatrixBitmapType bitmap, FILE *tty)
{
  int row, column;

  // For each row,
  for(row = 0; row < APP_MATRIX_ROWS; row++) {
    char line[(APP_MATRIX_COLS * 2)];
    // Format line
    for(column = 0; column < APP_MATRIX_COLS; column++) {
      line[column * 2] = (AppGetMatrixBitmapDot(bitmap, row, column) == AppMatrixDotSet) ? '*' : ' ';
      line[(column * 2) + 1] = ' ';
    }
    // Remove trailing spaces
    for(column = sizeof(line) - 1; column >= 0; column--) {
      if(line[column] == ' ') {
        line[column] = 0;
      }
      else {
        break;
      }
    }
    // Print line
    fprintf(tty, "%s\n", line);
  }
}
