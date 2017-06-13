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
#include "crc16.h"

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
Crc16Type crc16UpdateByte(Crc16Type crc16, uint8_t byte)
{
  static const Crc16Type polynom = 0x8005;
  uint_fast8_t bit;

  for(bit = 0; bit < 8; bit++) {
    if(((crc16 & 0x8000) ^ ((byte & 0x80) << 8)) == 0x8000) {
      crc16 = (crc16 << 1) ^ polynom;
    }
    else {
      crc16 <<= 1;
    }
    byte <<= 1;
  }

  return crc16;
}

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
Crc16Type crc16UpdateBuffer(Crc16Type crc16, uint8_t *buffer, uint32_t length)
{
  while(length--) {
    crc16 = crc16UpdateByte(crc16, *buffer++);
  }

  return crc16;
}
