/** @file
  Header file for SystemTimeLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/
#ifndef BASE_SYSTEM_TIME_LIB_H_
#define BASE_SYSTEM_TIME_LIB_H_

#include <Uefi.h>
#include <Library/DebugLib.h>

#define RTC_INDEX_REGISTER        (0x70)
#define RTC_TARGET_REGISTER       (0x71)

#define RTC_INDEX_MASK            (0x7F)
#define RTC_BANK_SIZE             (0x80)

#define RTC_SECONDS               (0x00)
#define RTC_MINUTES               (0x02)
#define RTC_HOURS                 (0x04)
#define RTC_DAY_OF_MONTH          (0x07)
#define RTC_MONTH                 (0x08)
#define RTC_YEAR                  (0x09)
#define CMOS_REGA                 (0x0A)
#define CMOS_REGB                 (0x0B)
#define CMOS_REGC                 (0x0C)
#define CMOS_REGD                 (0x0D)

#define RTC_UPDATE_IN_PROGRESS    (0x80)
#define RTC_HOLD                  (0x80)
#define RTC_MODE_24HOUR           (0x02)
#define RTC_CLOCK_DIVIDER         (0x20)
#define RTC_RATE_SELECT           (0x06)

#define BCD2BINARY(A)             (((((A) >> 4) & 0xF) * 10) + ((A) & 0xF))
#define CENTURY_OFFSET            (2000)

#pragma pack(1)
typedef union {
  struct {
    UINT8  Seconds;
    UINT8  Minutes;
    UINT8  Hours;
    UINT8  DayOfMonth;
    UINT8  Month;
    UINT16 Year;
    UINT8  Reserved;
  } Data;
  UINT64 Data64;
} SYSTEM_TIME;
#pragma pack()

/**
  This funtion will output the system time in (YYYY-MM-DD-HH-MM-SS) format

  @param[out] SYSTEM_TIME      typedef struct for system time
**/
VOID
GetSystemTime (
  OUT SYSTEM_TIME *const SystemTime
  );

/**
  This funtion returns Unix Time: The number of milliseconds since 1 January 1970.

  @param[in] SYSTEM_TIME      typedef struct for system time

  @retval  UINT64             Total time in milli seconds
**/
UINT64
ConvertSystemTimeToUnixFormat (
  IN SYSTEM_TIME *SystemTime
  );

#endif /* BASE_SYSTEM_TIME_LIB_H_ */
