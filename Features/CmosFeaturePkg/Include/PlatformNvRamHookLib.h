/** @file
  Platform Nv RAM Hook Library Header Definition which will oprovide the absraction
  to the different NVRAM types like eMMC, ECRAM, secondary SPI part etc.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _PLATFORM_NVRAM_HOOK_LIB_H_
#define _PLATFORM_NVRAM_HOOK_LIB_H_

#ifndef DEFAULT_VALUE
#define DEFAULT_VALUE         0
#endif

#define TXT_ACHECK_REQ_CLEAR_MASK     0xFE
#define TXT_POWERDOWN_REQ_CLEAR_MASK  0xFD

/**
  Test to see if Checksum is bad.

  @retval   TRUE  - Checksum content is bad
            FALSE - Checksum content is good
**/
BOOLEAN
IsChecksumBad (
  VOID
  );

/**
  Clear Diagnostic Status.
**/
VOID
ClearDiagnosticStatus (
  VOID
  );

/**
  Check to reset setup variable and Clear CMOS 0E when CMOS content is bad

  @retval Flag to Reset Setup variable :  TRUE  - When CMOS content is bad
                                          FALSE - Otherwise
**/
BOOLEAN
NeedResetBoardDefaultVariableHob(
  VOID
  );

/**
  Load CMOS default on RTC battery failure.
**/
VOID
SecondaryNvRamInit(
  VOID
  );

/**
  Get Boot Flag Status.

  @retval Value of current boot status
**/
UINT8
GetFastBootFlagStatus(
  VOID
  );

/**
  Update Boot Flag Status.

  @param [in] BootStatus   Current Boot value which to be updated.
**/
VOID
UpdateFastBootFlagStatus(
  UINT8  BootStatus
  );

/**
  Get RTC 3 Address.

  @retval Current value of RTC 3 Address
**/
UINT8
GetRtc3Address(
  VOID
  );

/**
  Get Post Code Break for Port 80 and 81

  @param [out] *Port80   Pointer to return value of post code 80

  @param [out] *Port81   Pointer to return value of post code 81
**/
VOID
GetPostCodeBreak(
  UINT8 *Port80,
  UINT8 *Port81
);

/**
  Get TXT Alias Check Request and Reset it.

  @retval TXT Alias Check Request
**/
UINT8
GetTxtAliasCheckAndReset(
  VOID
  );

/**
  Save the TXT Alias Check Request.

  @param [in] ACheckRequest   Request of TXT Alias Check.
**/
VOID
SaveTxtAliasCheck(
  UINT8  ACheckRequest
  );

/**
  Get TXT Powerdown Request and Reset it.

  @retval TXT Powerdown Request
**/
UINT8
GetTxtPowerdownRequest(
  VOID
  );

/**
  Save the TXT Memory Powerdown Request.

  @param [in] PowerdownRequest   Request of TXT Memory Powerdown Request.
**/
VOID
SaveTxtPowerdown(
  UINT8  PowerdownRequest
  );

/**
  Save the CPU Ratio.

  @param [in] CpuRatio   The Cpu ratio value to be saved.
**/
VOID
SaveCpuRatio(
  UINT8  CpuRatio
  );

/**
  Set the BCLK Ramp Flag.

  @param [in] BclkRampFlag   The BCLK Ramp Flag to be set.
**/
VOID
SetBclkRampFlag(
  UINT8  BclkRampFlag
  );


/**
  Get TBT Host Router Status.

  CMOS_TBTHR_PRESENT_ON_RESUME: Bits 0-3 is for DTBT and Bits 4-7 is for ITBT (HIA0/1/2/Reserved)

  @retval Current value of TBT Host Router Status
**/
UINT8
GetTbtHostRouterStatus(
  VOID
  );

/**
  Save TBT Host Router Status.

  CMOS_TBTHR_PRESENT_ON_RESUME: Bits 0-3 is for DTBT and Bits 4-7 is for ITBT (HIA0/1/2/Reserved)

  @param [in] TbtHrStatus   The TBT Host Router Status to be udpated.
**/
VOID
SaveTbtHostRouterStatus(
  UINT8  TbtHrStatus
  );
#endif // _PLATFORM_NVRAM_HOOK_LIB_H_
