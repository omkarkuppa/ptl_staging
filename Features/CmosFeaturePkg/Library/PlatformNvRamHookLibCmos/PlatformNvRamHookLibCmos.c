/** @file
  Platform NvRam CMOS Hook Library which will provide the CMOS instances to PlatformNvRamHookLib.h.

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

#include <Base.h>
#include <PlatformNvRamHookLib.h>
#include "CmosMap.h"
#include <Library/CmosAccessLib.h>
#include <Library/DebugLib.h>
/**
  This function checks if RTC Power Failure occurred by
  reading RTC_PWR_FLR bit

  @retval RTC Power Failure state: TRUE  - Battery is always present.
                                   FALSE - CMOS is cleared.
**/
BOOLEAN
EFIAPI
PmcIsRtcBatteryGood(
  VOID
  );

/**
  CMOS test to see if Checksum is bad.

  @retval State of CMOS content : TRUE  - CMOS content is bad
                                  FALSE - CMOS content is good
**/
BOOLEAN
IsChecksumBad (
  VOID
  )
{
  if (CmosRead8((UINT8)CMOS_BAD_REG) & (BIT6 + BIT7)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Clear Diagnostic Status.
**/
VOID
ClearDiagnosticStatus (
  VOID
  )
{
  UINT8      Value;

  Value = CmosRead8((UINT8)CMOS_BAD_REG);
  CmosWrite8((UINT8)CMOS_BAD_REG, (Value & 0x3F));
}

/**
  Check to reset setup variable and Clear CMOS 0E when CMOS content is bad

  @retval Flag to Reset Setup variable :  TRUE  - When CMOS content is bad
                                          FALSE - Otherwise
**/
BOOLEAN
NeedResetBoardDefaultVariableHob(
  )
{
  //
  // Check whether the CMOS content is corrupted.
  //
  if (IsChecksumBad ()) {
    DEBUG ((DEBUG_ERROR, "CMOS battery is healthy but CMOS content is bad. Reset the SMOS 0Eh and the Setup variable.\n"));
    ClearDiagnosticStatus ();
    return TRUE;
  }
  return FALSE;
}

/**
  Load CMOS default on RTC battery failure.
**/
VOID
SecondaryNvRamInit(
  VOID
  )
{
  //
  // Perform a checksum computation and verify if the checksum is correct. If the checksum is incorrect
  // initialize all the CMOS location to their default values and recalculate the checksum.
  //
  if (PmcIsRtcBatteryGood ()) {
    CmosInit (FALSE);
  } else {
    DEBUG ((EFI_D_ERROR, "RTC battery is failure. Load CMOS default.\n"));
    CmosInit (TRUE);
  }
}

/**
  Get Boot Flag Status.

  @retval Value of current boot status
**/
UINT8
GetFastBootFlagStatus(
  VOID
  )
{
  return CmosRead8(CMOS_FAST_BOOT_REG);
}

/**
Update Boot Flag Status.

@param [in] BootStatus   Current Boot value which to be updated.
**/
VOID
UpdateFastBootFlagStatus(
  UINT8  BootStatus
  )
{
  CmosWrite8(CMOS_FAST_BOOT_REG, BootStatus);
}

/**
  Get RTC 3 Address.

  @retval Current value of RTC 3 Address
**/
UINT8
GetRtc3Address(
  VOID
  )
{
  return CmosRead8(0x4F);
}

/**
  Get Post Code Break for Port 80 and 81

  @param [out] *Port80   Pointer to return value of post code 80

  @param [out] *Port81   Pointer to return value of post code 81
**/

VOID
GetPostCodeBreak(
  UINT8 *Port80,
  UINT8 *Port81
  )
{
  *Port80 = CmosRead8(CMOS_POST_CODE_BREAK_REG);
  *Port81 = CmosRead8(CMOS_POST_CODE_BREAK_1_REG);
}

/**
  Get TXT Alias Check Request and Reset it.

  @retval TXT Alias Check Request
**/
UINT8
GetTxtAliasCheckAndReset(
  VOID
  )
{
  UINT8 TxtACheck;
  TxtACheck =  CmosRead8(CMOS_TXT_REG);
  //
  // ACheck Request needs to be reset since we do not want system to run ACheck in the same boot sequence
  //
  CmosWrite8(CMOS_TXT_REG, (UINT8) (TxtACheck & TXT_ACHECK_REQ_CLEAR_MASK));
  return TxtACheck;
}

/**
  Save the TXT Alias Check Request.

  @param [in] ACheckRequest   Request of TXT Alias Check.
**/
VOID
SaveTxtAliasCheck(
  UINT8  ACheckRequest
  )
{
  CmosWrite8(CMOS_TXT_REG, ACheckRequest);
}

/**
  Get TXT Powerdown Request and Reset it.

  @retval TXT Powerdown Request
**/
UINT8
GetTxtPowerdownRequest(
  VOID
  )
{
  UINT8 TxtPowerDownReq;

  TxtPowerDownReq =  CmosRead8 (CMOS_TXT_REG);
  //
  // TXT Powerdown Request needs to be reset since we do not want system to go to Powerdown state once again
  //
  CmosWrite8(CMOS_TXT_REG, (UINT8)(TxtPowerDownReq & TXT_POWERDOWN_REQ_CLEAR_MASK));
  TxtPowerDownReq = (TxtPowerDownReq & 2) >> 1;

  return TxtPowerDownReq;
}

/**
  Save the TXT Memory Powerdown Request in Cmos.

  @param [in] PowerdownRequest   Request of TXT Memory Powerdown (PD_Request).
                          000b   No request
                          001b   MRC scrubbing error
                          010b   Set by BIOS Setup via CMOS
                          011b   MOR Interface
                          100b   Set by BIOS Setup via TPM
**/
VOID
SaveTxtPowerdown(
  UINT8  PowerdownRequest
  )
{
  UINT8 RegVal;
  UINT8 TxtPowerDownReq = 0;

  if ((PowerdownRequest == 1) || (PowerdownRequest == 2)) {
    // Cmos case
    RegVal = CmosRead8 (CMOS_TXT_POWERDOWN_REQUEST);
    RegVal |= (PowerdownRequest & 7);  // update bits[2:0] in CMOS
    CmosWrite8 (CMOS_TXT_POWERDOWN_REQUEST, RegVal);
  } else if (PowerdownRequest == 4 ) {
    // TPM case
    TxtPowerDownReq = 2;
    RegVal = CmosRead8 (CMOS_TXT_REG);
    RegVal |= TxtPowerDownReq;         // update Cmos 2nd Bit
    CmosWrite8 (CMOS_TXT_REG, RegVal);
  }

  return;
}

/**
  Save the CPU Ratio.

  @param [in] CpuRatio   The Cpu ratio value to be saved.
**/
VOID
SaveCpuRatio(
  UINT8  CpuRatio
)
{
  CmosWrite8(CMOS_CPU_RATIO_OFFSET, CpuRatio);
}

/**
  Set the BCLK Ramp Flag.

  @param [in] BclkRampFlag   The BCLK Ramp Flag to be set.
**/
VOID
SetBclkRampFlag(
  UINT8  BclkRampFlag
  )
{
  CmosWrite8(CMOS_OC_SEND_BCLK_RAMP_MSG, BclkRampFlag);
}
