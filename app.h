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

// Definition for packed structures
#define __packed __attribute__((__packed__))

/***********************************************************************************************************************
 * Common Stuff
 **********************************************************************************************************************/
  typedef enum __packed {
    AppInactive = 0,
    AppActive   = 1
  } AppActiveType;

/***********************************************************************************************************************
 * Layer init and cleanup
 **********************************************************************************************************************/
void AppInit(void *ctx);
void AppCleanup(void);

/***********************************************************************************************************************
 * Firmware Version
 **********************************************************************************************************************/
typedef struct __packed {
  uint16_t major;
  uint16_t minor;
  uint16_t revision;
} AppVersionType;

void AppGetVersion(AppVersionType *version);

/***********************************************************************************************************************
 * Date and time
 **********************************************************************************************************************/
typedef enum __packed {
  AppWinterTime = 0,
  AppSummerTime = 1
} AppDaylightSavingType;

typedef enum __packed {
  AppSunday    = 0,
  AppMonday    = 1,
  AppTuesday   = 2,
  AppWednesday = 3,
  AppThursday  = 4,
  AppFriday    = 5,
  AppSaturday  = 6
} AppWeekDayType;

typedef struct __packed {
  uint8_t day;
  uint8_t month;
  uint8_t year;
  AppWeekDayType weekDay;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  AppDaylightSavingType daylightSaving;
} AppDateTimeType;

void AppGetDateTime(AppDateTimeType *dateTime);
void AppSetDateTime(const AppDateTimeType *dateTime);

/***********************************************************************************************************************
 * Operating modes
 **********************************************************************************************************************/
void AppFactoryReset(void);
void AppActivateBootloader(void);
void AppSetNormalMode(void);
void AppSetPreviewMode(void);

/***********************************************************************************************************************
 * Display bitmap matrix
 **********************************************************************************************************************/
typedef uint8_t AppMatrixBitmapType[26];
void AppSetPreviewMatrix(const AppMatrixBitmapType matrix);

/***********************************************************************************************************************
 * Intensity
 **********************************************************************************************************************/
typedef enum __packed {
  AppIntensity1 = 0x08,
  AppIntensity2 = 0x0C,
  AppIntensity3 = 0x13,
  AppIntensity4 = 0x1D,
  AppIntensity5 = 0x2D,
  AppIntensity6 = 0x46,
  AppIntensity7 = 0x6B,
  AppIntensity8 = 0xA6,
  AppIntensity9 = 0xFF,
} AppIntensityType;

void AppGetIntensity(AppIntensityType *intensity);
void AppSetIntensity(const AppIntensityType *intensity);

/***********************************************************************************************************************
 * RTC Calibration
 **********************************************************************************************************************/
typedef uint8_t AppRtcCalibrationValueType[4];
void AppSetRtcCalibration(const AppRtcCalibrationValueType *calibration);

typedef struct __packed {
  uint8_t day;
  uint8_t month;
  uint8_t year;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  AppDaylightSavingType daylightSaving;
} AppRtcCalibrationDateTime;

typedef struct __packed {
  AppRtcCalibrationDateTime actualDateTime;
  AppRtcCalibrationDateTime lastCalibrationDateTime;
} AppLastCalibrationType;

void AppGetLastCalibration(AppLastCalibrationType *lastCalibration);

/***********************************************************************************************************************
 * Standby
 **********************************************************************************************************************/
typedef struct __packed {
  uint8_t hourOn;
  uint8_t minuteOn;
  uint8_t hourOff;
  uint8_t minuteOff;
  AppActiveType active;
} AppStandbyType;

void AppGetStandby(AppStandbyType *standby);
void AppSetStandby(const AppStandbyType *standby);

/***********************************************************************************************************************
 * Flash configuration
 **********************************************************************************************************************/
// NUmber of flash pages holding the clock configuration
#define APP_CLOCK_CONFIG_FLASH_PAGES 14400
// Number of clock configurations per flash pages
#define APP_CLOCK_CONFIG_PER_PAGES 6

// Appointments
#define APP_APPOINTMENT_MONTH_EVERY   0
#define APP_APPOINTMENT_DAY_EVERY     0
#define APP_APPOINTMENT_WEEKDAY_EVERY 7
typedef struct __packed {
  AppActiveType active;
  AppActiveType overlay;
  uint8_t month;
  uint8_t day;
  AppWeekDayType weekDay;
  uint8_t hour;
  uint8_t minute;
  // Seconds should be always zero
  uint8_t second;
} AppAppointmentType;

typedef union __packed {
  // Clock configuration
  struct __packed {
    AppMatrixBitmapType matrixBitmap[APP_CLOCK_CONFIG_PER_PAGES];
  } clockConfig;
  // Appointment configuration
  struct __packed {
    AppMatrixBitmapType matrixBitmap;
    AppAppointmentType appointment;
  } appointmentConfig;
} AppFlashConfigType;

typedef struct __packed {
  uint16_t pageNumber;
  AppFlashConfigType config;
  uint8_t dummy[256 - sizeof(AppFlashConfigType)];
} AppFlashConfigPageType;

void AppGetFlashConfigPage(uint16_t pageNumber, AppFlashConfigPageType *config);
void AppSetFlashConfig(AppFlashConfigPageType *config);

// Number of pages per sector (erase unit, 16 * 256 = 4k)
#define APP_FLASH_PAGES_PER_SECTOR  16

void AppEraseFlashConfigSector(uint16_t startPage);

/***********************************************************************************************************************
 * Appointments
 **********************************************************************************************************************/
typedef AppAppointmentType AppointmentsConfigType[20];
void AppGetAppointments(AppointmentsConfigType appointments);
void AppSetAppointments(const AppointmentsConfigType appointments);

#endif // APP_H_
