/***********************************************************************************************************************
 *
 * ID100 Utility
 * Application Layer
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
#ifndef APP_H_
#define APP_H_

#include <stdint.h>

#define __packed __attribute__((__packed__))

typedef struct __packed {
  uint8_t major[2];
  uint8_t minor[2];
  uint8_t revision[2];
} AppVersionType;

typedef struct __packed {
  uint8_t day;
  uint8_t month;
  uint8_t year;
  uint8_t weekDay;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t summerTime;
} AppDateTimeType;

void AppInit(void *ctx);
void AppCleanup(void);
AppVersionType *AppGetVersion(void);
AppDateTimeType *AppGetDateTime(void);

#endif // APP_H_
