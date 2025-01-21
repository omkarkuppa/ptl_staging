/** @file
  Base IMR Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <Library/IoLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/NguInfoLib.h>
#include <Library/BaseImrInfoLib.h>
#include <Register/HostBridgeRegs.h>
#include <Register/ImrRegs.h>
#include <Library/PeiImrInitLib.h>

/**
  This function will check if IMR range is enabled or not

  @param[in] ImrBaseReg       IMR base address
  @param[in] ImrMaskReg       IMR mask value

  @retval TRUE                IMR range is enabled
  @retval FALSE               IMR range is disabled
**/
BOOLEAN
IsImrBaseRangeEnabled (
  IN UINT32                   ImrBaseReg,
  IN UINT32                   ImrMaskReg
  )
{
  IMR0BASE_IMPH_IOC_MCHBAR_STRUCT   ImrBaseStruct;
  IMR0MASK_IMPH_IOC_MCHBAR_STRUCT   ImrMaskStruct;

  ImrBaseStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(ImrBaseReg))));
  ImrMaskStruct.Data = (MmioRead64 ((UINTN) (GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase) + (UINT32) GET_NOC_EFFECTIVE_ADDRESS(ImrMaskReg))));

  if ((ImrBaseStruct.Bits.disable0 == 0) && (ImrBaseStruct.Bits.imr_base_rw != 0) && (ImrMaskStruct.Bits.imr_mask_rw != 0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  This function will check if TraceHub range is enabled or not

  @retval TRUE                TraceHub range is enabled
  @retval FALSE               TraceHub range is disabled
**/
BOOLEAN
IsImrTraceHubBaseRangeEnabled (
  VOID
  )
{
  UINT32  ImrBaseReg;
  UINT32  ImrMaskReg;

  ImrBaseReg = IMR_BASE_2_TYPE_BASE_OFFSET(TraceHubImr);
  ImrMaskReg = IMR_BASE_2_TYPE_MASK_OFFSET(TraceHubImr);

  return IsImrBaseRangeEnabled (ImrBaseReg, ImrMaskReg);
}

/**
  Determine if Imr Prmrr Base Range is Enabled.

  @retval TRUE      Imr Prmrr Base Range is enabled.
  @retval FALSE     Imr Prmrr Base Range is disabled.

**/
BOOLEAN
EFIAPI
IsImrPrmrrBaseRangeEnabled (
  VOID
  )
{
  UINT64                         MchBar;
  IMRnBASE_0_0_0_MCHBAR_IMPH     ImrnBase;
  IMRnMASK_0_0_0_MCHBAR_IMPH     ImrnMask;

  //
  // Get the MCHBAR value to program IMRs.
  //
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  //
  // Read IMR PRMRR Base
  //
  ImrnBase.Data = MmioRead64 ((UINTN) MchBar + GET_NOC_EFFECTIVE_ADDRESS(IMR17BASE_IMPH_IOC_MCHBAR_REG));

  //
  // Read IMR PRMRR Mask
  //
  ImrnMask.Data = MmioRead64 ((UINTN) MchBar + GET_NOC_EFFECTIVE_ADDRESS(IMR17BASE_IMPH_IOC_MCHBAR_REG) + RELATIVE_MASK_OFFSET);

  //
  // Return TRUE If IMR_PRMRR_BASE Enable Range bit[4] is set to 0, 
  // IMR_PRMRR_BASE bits[41:10] are not equal to zero and IMR_PRMRR_MASK bits[41:10] are not equal to zero.
  //
  if ((ImrnBase.Bits.Disable == 0) && (ImrnBase.Bits.IMRn_BASE_RW != 0) && (ImrnMask.Bits.IMRn_MASK_RW != 0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Determine if TSE Imr Base Range is Enabled.
  @retval TRUE      Imr Tse Base Range is enabled.
  @retval FALSE     Imr Tse Base Range is disabled.
**/
BOOLEAN
EFIAPI
IsImrTseBaseRangeEnabled (
  VOID
  )
{
  UINT64                         MchBar;
  IMRnBASE_0_0_0_MCHBAR_IMPH     ImrnBase;
  IMRnMASK_0_0_0_MCHBAR_IMPH     ImrnMask;

  //
  // Get the MCHBAR value to program IMRs.
  //
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  //
  // Read IMR TSE Base
  //
  ImrnBase.Data = MmioRead64 ((UINTN) MchBar + GET_NOC_EFFECTIVE_ADDRESS(IMR21BASE_IMPH_IOC_MCHBAR_REG));

  //
  // Read IMR TSE Mask
  //
  ImrnMask.Data = MmioRead64 ((UINTN) MchBar + GET_NOC_EFFECTIVE_ADDRESS(IMR21BASE_IMPH_IOC_MCHBAR_REG) + RELATIVE_MASK_OFFSET);

  //
  // Return TRUE If IMR_IMR_BASE Enable Range bit[4] is set to 0,
  // IMR_TSE_BASE bits[41:10] are not equal to zero and IMR_TSE_MASK bits[41:10] are not equal to zero.
  //
  if ((ImrnBase.Bits.Disable == 0) && (ImrnBase.Bits.IMRn_BASE_RW != 0) && (ImrnMask.Bits.IMRn_MASK_RW != 0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

