/** @file
  Source file for BaseSystemTimeLib.

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

#include <Library/SystemTimeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>

/**
  This funtion will output the system time in (YYYY-MM-DD-HH-MM-SS) format

  @param[out] SYSTEM_TIME      typedef struct for system time
**/
VOID
GetSystemTime (
  OUT SYSTEM_TIME *const SystemTime
  )
{
  UINT32           Timeout;

  //
  // Wait until RTC "update in progress" bit goes low.
  //
  Timeout = 0x0FFFFF;
  do {
    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGA);
    if ((IoRead8 (RTC_TARGET_REGISTER) & RTC_UPDATE_IN_PROGRESS) != RTC_UPDATE_IN_PROGRESS) {
      break;
    }
  } while (--Timeout > 0);

  if (0 == Timeout) {
    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGB);
    IoWrite8 (RTC_TARGET_REGISTER, RTC_HOLD | RTC_MODE_24HOUR);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGA);
    IoWrite8 (RTC_TARGET_REGISTER, RTC_CLOCK_DIVIDER | RTC_RATE_SELECT);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGC);
    IoRead8 (RTC_TARGET_REGISTER);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGD);
    IoRead8 (RTC_TARGET_REGISTER);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGB);
    IoWrite8 (RTC_TARGET_REGISTER, RTC_MODE_24HOUR);
  }
  //
  // Read seconds
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_SECONDS);
  SystemTime->Data.Seconds = IoRead8(RTC_TARGET_REGISTER);

  //
  // Read minutes
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_MINUTES);
  SystemTime->Data.Minutes = IoRead8(RTC_TARGET_REGISTER);

  //
  // Read hours
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_HOURS);
  SystemTime->Data.Hours = IoRead8(RTC_TARGET_REGISTER);

  //
  // Read day of month
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_DAY_OF_MONTH);
  SystemTime->Data.DayOfMonth = IoRead8(RTC_TARGET_REGISTER);

  //
  // Read month
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_MONTH);
  SystemTime->Data.Month = IoRead8(RTC_TARGET_REGISTER);

  //
  // Read year and add current century.
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_YEAR);
  SystemTime->Data.Year = IoRead8(RTC_TARGET_REGISTER);

  SystemTime->Data.Seconds = BCD2BINARY(SystemTime->Data.Seconds);
  SystemTime->Data.Minutes = BCD2BINARY(SystemTime->Data.Minutes);
  SystemTime->Data.Hours = BCD2BINARY(SystemTime->Data.Hours);
  SystemTime->Data.DayOfMonth = BCD2BINARY(SystemTime->Data.DayOfMonth);
  SystemTime->Data.Month = BCD2BINARY(SystemTime->Data.Month);
  SystemTime->Data.Year = BCD2BINARY(SystemTime->Data.Year) + CENTURY_OFFSET;

}

/**
  This funtion returns Unix Time: The number of milliseconds since 1 January 1970.

  @param[in] SYSTEM_TIME      typedef struct for system time

  @retval  UINT64             Total time in milli seconds
**/
UINT64
ConvertSystemTimeToUnixFormat (
  IN SYSTEM_TIME *SystemTime
)
{
  UINT64                 NoOfMilliSec;
  UINT8                  LeapYearCount;
  UINT16                 TotalYears;
  UINT8                  Month;
  UINT16                 AddDays;
  UINT16                 Year;

  LeapYearCount = 0;
  NoOfMilliSec = 0;
  TotalYears = 0;
  Month = 0;
  AddDays = 0;

  //
  // Calculate Total time in milli seconds till previous year.
  //
  if (SystemTime->Data.Year > 1970) {
    TotalYears = SystemTime->Data.Year - 1970 - 1;
  }
  // check for total leap years
  for (Year = 1970; Year < SystemTime->Data.Year; Year++) {
    if (Year % 4 == 0) {
      if (Year % 100 == 0) {
        if (Year % 400 == 0) {
          LeapYearCount++;
        }
      } else {
        LeapYearCount++;
      }
    }
  }
  // time in milli seconds till previous year
  NoOfMilliSec = (UINT64)(TotalYears * (365 + LeapYearCount) * 24 * 60 * 60 * 1000);

  //
  // Calculate time in milli seconds for current year.
  //
  while (Month <= SystemTime->Data.Month) {
    if (Month != SystemTime->Data.Month) {
      if (Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 || Month == 10 || Month == 12) {
        AddDays += 31;
      } else if (Month == 4 || Month == 6 || Month == 9 || Month == 11) {
        AddDays += 30;
      } else if (Month == 2) {
        if (SystemTime->Data.Year % 4 == 0) {
          if (SystemTime->Data.Year % 100 == 0) {
            if (SystemTime->Data.Year % 400 == 0) {
              AddDays += 29;
            } else {
              AddDays += 28;
            }
          } else {
            AddDays += 29;
          }
        } else {
          AddDays += 28;
        }
      }
    } else {
      AddDays += (SystemTime->Data.DayOfMonth - 1);
      NoOfMilliSec += (AddDays * SystemTime->Data.Hours * SystemTime->Data.Minutes * SystemTime->Data.Seconds * 1000);
      break;
    }
    Month++;
  }

  return (UINT64)(NoOfMilliSec);

}