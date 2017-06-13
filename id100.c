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
#include <stdint.h>
#include <stdbool.h>

#include "crc16.h"

static const uint8_t STX = 0x02;
static const uint8_t ENQ = 0x10;

static Crc16Type crc = 0xFFFF;

static void OutputByte(uint8_t byte)
{
  static bool escape = false;

  if(escape == true) {
    if((byte == STX) || (byte == ENQ)) {
      putchar(ENQ);
      crc = crc16UpdateByte(crc, ENQ);
      byte += 0x80;
    }
  }
  else {
    escape = true;
  }

  putchar(byte);
  crc = crc16UpdateByte(crc, byte);
}

/*
int main(void)
{
  uint8_t bitmap[26], i;
  Crc16Type finalCrc;

  for(i = 0; i < sizeof(bitmap); i++) {
    bitmap[i] = 0xAA;
  }

  OutputByte(STX);
  OutputByte(0);
  OutputByte(27);
  OutputByte(0x44);
  for(i = 0; i < sizeof(bitmap); i++) {
    OutputByte(bitmap[i]);
  }
  finalCrc = crc;
  OutputByte((finalCrc >> 8) & 0xFF);
  OutputByte(finalCrc & 0xFF);

  return 0;
}
*/

int main(void)
{
  Crc16Type finalCrc;

  OutputByte(STX);
  OutputByte(0);
  OutputByte(1);
  OutputByte('A');
  finalCrc = crc;
  OutputByte((finalCrc >> 8) & 0xFF);
  OutputByte(finalCrc & 0xFF);

  return 0;
}
