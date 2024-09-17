/** @file
  PEIM Private Library to program IMRs.

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

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiImrInitLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Register/HostBridgeRegs.h>
#include <Library/IGpuInfoLib.h>
#include <Library/NguInfoLib.h>
#include <Library/PeiCmiInitLib.h>
#include <DprInfoHob.h>

STATIC IMR_ENTRY mImrTable[IMR_MAX] = {
  //
  // IMR_ENTRY should be aligned and match with IMR_ID numbers
  // IMR ID (IMR_ID), RAC, WAC, BaseAddressRegOffset, CP Offset
  //
  { IMR_NPK,          (B_NPK_SAI | B_RCIOMMU_BYPASS_SAI), (B_NPK_SAI | B_RCIOMMU_BYPASS_SAI), GET_NOC_EFFECTIVE_ADDRESS(IMR15BASE_IMPH_IOC_MCHBAR_REG),        GET_NOC_EFFECTIVE_ADDRESS(IMR15CP_IMPH_IOC_MCHBAR_REG)}, //NPK is IMR15 register
  { IMR_PRMRR,        V_SAI_NONE,                         V_SAI_NONE,                         GET_NOC_EFFECTIVE_ADDRESS(IMR17BASE_IMPH_IOC_MCHBAR_REG),        GET_NOC_EFFECTIVE_ADDRESS(IMR17CP_IMPH_IOC_MCHBAR_REG)}, //PRMRR is IMR17 register
  { IMR_TSE,          V_SAI_NONE,                         V_SAI_NONE,                         GET_NOC_EFFECTIVE_ADDRESS(IMR21BASE_IMPH_IOC_MCHBAR_REG),        KEEP_SI_DEFAULT_VALUE}, //TSE is IMR21 register
  { IMR_STREAM_TRACER,V_SAI_NONE,                         V_SAI_NONE,                         GET_NOC_EFFECTIVE_ADDRESS(IMR25BASE_IMPH_IOC_MCHBAR_REG),        GET_NOC_EFFECTIVE_ADDRESS(IMR25CP_IMPH_IOC_MCHBAR_REG)}, //STREAM_TRACER is IMR25 register
  { IMR_SEAMRR,       V_SAI_NONE,                         V_SAI_NONE,                         GET_NOC_EFFECTIVE_ADDRESS(IMR26BASE_IMPH_IOC_MCHBAR_REG),        KEEP_SI_DEFAULT_VALUE}, //SEAMRR is IMR26 register
  { IMR_1M_GSM,       V_SAI_NONE,                         V_SAI_NONE,                         GET_NOC_EFFECTIVE_ADDRESS(IMR1M2BASE_IMPH_IOC_MCHBAR_REG),       KEEP_SI_DEFAULT_VALUE}, //GSM is IMR1M2 register
  { IMR_1M_DSM,       V_SAI_NONE,                         V_SAI_NONE,                         GET_NOC_EFFECTIVE_ADDRESS(IMR1M3BASE_IMPH_IOC_MCHBAR_REG),       KEEP_SI_DEFAULT_VALUE}, //DSM is IMR1M3 register
  { IMR_1M_CCS,       B_GT_SAI | B_DISPLAY_SAI,           B_GT_SAI | B_DISPLAY_SAI,           GET_NOC_EFFECTIVE_ADDRESS(IMR1M6BASE_IMPH_IOC_MCHBAR_REG),       KEEP_SI_DEFAULT_VALUE}, //CCS is IMR1M6 register
  { IMR_1M_DPR,       V_SAI_NONE,                         V_SAI_NONE,                         GET_NOC_EFFECTIVE_ADDRESS(IMR1M9BASE_IMPH_IOC_MCHBAR_REG),       INVALID_CP_OFFSET},     //DPR is IMR1M9 register
  { IMR_1M_TSEG,      V_SAI_NONE,                         V_SAI_NONE,                         GET_NOC_EFFECTIVE_ADDRESS(IMR1M10BASE_IMPH_IOC_MCHBAR_REG),      INVALID_CP_OFFSET},     //TSEG is IMR1M10 register
  { IMR_1M_IMR11,     V_SAI_NONE,                         V_SAI_NONE,                         GET_NOC_EFFECTIVE_ADDRESS(IMR1M11BASE_IMPH_IOC_MCHBAR_REG),      INVALID_CP_OFFSET}      //IMR1M11 register
};

/**
  Return TRUE when the Operand is exactly power of 2.

  @retval TRUE  Operand is exactly power of 2.
  @retval FALSE Operand is not power of 2.
**/
STATIC
BOOLEAN
IsPowerOfTwo (
  IN  UINT64        Operand
)
{
  ASSERT (Operand != 0);
  return (BOOLEAN) ((Operand & (Operand - 1)) == 0);
}

/**
  Program IMR registers.

  @param[in] ImrId     - IMR ID number
  @param[in] Base      - Byte aligned Base Address value to be programmed in IMR
  @param[in] Size      - Size of the region

  @retval EFI_SUCCESS            - IMR Programming completed successfully.
  @retval EFI_INVALID_PARAMETER  - The input parameter is invalid
  @retval EFI_UNSUPPORTED          IMR is not supported for BIOS programming.
**/
EFI_STATUS
EFIAPI
SetImr (
  IN   IMR_ID      ImrId,
  IN   UINT64      Base,
  IN   UINT64      Size
)
{
  UINT64             MchBar;
  UINT64             MaskOrLimit;
  UINT64             SizeAlignment;
  BOOLEAN            IsFullImr;
  UINT64             CpDefault;
  IMRnBASE_0_0_0_MCHBAR_IMPH     ImrnBase;
  IMR1MnBASE_0_0_0_MCHBAR_IMPH   Imr1mBase;
  IMRnMASK_0_0_0_MCHBAR_IMPH     ImrnMask;
  IMR1MnLimit_0_0_0_MCHBAR_IMPH  Imr1mLimit;


  DEBUG ((DEBUG_INFO, "IMR Programming for %d\n", ImrId));

  //
  // Return if invalid values are passed for Base, Size and Imr Id.
  //
  if ((Base == 0x0) || (Size == 0x0) || (ImrId >= IMR_MAX) || (ImrId < 0)) {
    DEBUG ((DEBUG_WARN, "IMR Programming exiting due to invalid Base Or Size Or IMR ID \n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Return if IMR is not BIOS programmable.
  //
  if (mImrTable[ImrId].BaseAddrRegOffset == IMR_NOT_BIOS_PROGRAMMABLE) {
    DEBUG ((DEBUG_WARN, "IMR is not BIOS programmable hence exiting from SetImr\n"));
    return EFI_UNSUPPORTED;
  }
  //
  // Check if the IMR to be programmed is Full or Simplified.
  //
  IsFullImr = FALSE;
  if (mImrTable[ImrId].Cp != INVALID_CP_OFFSET) {
    DEBUG ((DEBUG_INFO, "IMR to be programmed is Full IMR Type\n"));
    IsFullImr = TRUE;
  } else {
    DEBUG ((DEBUG_INFO, "IMR to be programmed is Simplified IMR Type\n"));
  }

  //
  // Check if zero values are passed for RAC/WAC in case of simplified IMR.
  //
  if ((IsFullImr == FALSE) && ((mImrTable[ImrId].Rac != 0x0) || (mImrTable[ImrId].Wac != 0x0))) {
    DEBUG ((DEBUG_WARN, "IMR Programming exiting due to non-zero RAC Or WAC value for Simplified IMR.\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Decide the Size alignment based on IMR type.
  // As per IO Exclude Range Alignment Requirement, BIOS shall ensure IO Exclude Range (aka NoC/SAF Exclude range) is aligned at 4KB boundary
  // Note - This is additional check since in most cases regular IMR size' is bigger than 4KB , so naturally it might be aligned at bigger 2 power N address , which is fine.
  // IMR_1M are 1MB aligned.
  //
  if ((ImrId >= IMR_NPK) && (ImrId < IMR_1M_GSM)) {
    SizeAlignment = SIZE_4KB;
  } else {
    SizeAlignment = SIZE_1MB;
  }

  //
  // Check if Base address is aligned to 4KB for IMR and 1MB for IMR_1M.
  //
  if ((Base & (SizeAlignment - 1)) != 0) {
    DEBUG ((DEBUG_WARN, "IMR Programming exiting due to Base is not aligned.\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if IMR Size is power of two and equal or bigger than 4KB.
  //
  if ((ImrId >= IMR_NPK) && (ImrId < IMR_1M_GSM)) {
    if ((FALSE == IsPowerOfTwo (Size)) || (Size < SizeAlignment)) {
      DEBUG ((DEBUG_WARN, "IMR Programming exiting due to Size is not power of two Or lesser than minimum.\n"));
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Get the MCHBAR value to program IMRs.
  //
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  //
  // Check IMRn vs IMR1M and program its Mask/Limit
  //
  if ((ImrId >= IMR_NPK) && (ImrId < IMR_1M_GSM)) {
    //
    // Calculate and program Mask value for IMRn.
    //
    MaskOrLimit = ~(Size - 1);
    ImrnMask.Data = MmioRead64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset + RELATIVE_MASK_OFFSET);
    ImrnMask.Bits.IMRn_MASK_RW = RShiftU64 (MaskOrLimit, N_IMR_BASE_LIMIT_ADDRESS_OFFSET);
    MmioWrite64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset + RELATIVE_MASK_OFFSET, ImrnMask.Data);
  } else if ((ImrId >= IMR_1M_GSM) && (ImrId < IMR_MAX)) {
    //
    // Calculate and program Limit value which ANDed with incoming address to determine
    // if result matches the IMR Base value and address falls within the IMR.
    //
    MaskOrLimit = Base + Size;
    Imr1mLimit.Data = MmioRead64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset + RELATIVE_MASK_OFFSET);
    Imr1mLimit.Bits.IMR1Mn_Limit_RW = RShiftU64 (MaskOrLimit, N_IMR_1M_BASE_LIMIT_ADDRESS_OFFSET);
    MmioWrite64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset + RELATIVE_MASK_OFFSET, Imr1mLimit.Data);
  }

  DEBUG ((DEBUG_INFO, "Mask/Limit programmed in IMR %d = %lx \n", ImrId, MmioRead64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset + RELATIVE_MASK_OFFSET)));

  //
  // Check if it is full IMR and if HOSTIA_BOOT_SAI is set, then program RAC/WAC/CP values.
  // Skip RAC/WAC programming for IMR1M and keep the silicon default values.
  //
  if (IsFullImr && (mImrTable[ImrId].Cp != KEEP_SI_DEFAULT_VALUE)) {
    CpDefault = MmioRead64 ((UINTN)MchBar + mImrTable[ImrId].Cp);
    DEBUG ((DEBUG_INFO, "Default CP value for IMR %d = %lx \n", ImrId, CpDefault));
    if ((CpDefault & B_HOSTIA_BOOT_SAI) == B_HOSTIA_BOOT_SAI) {
      MmioOr64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset + RELATIVE_RAC_OFFSET, (UINT64) mImrTable[ImrId].Rac);  // RAC
      MmioOr64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset + RELATIVE_WAC_OFFSET, (UINT64) mImrTable[ImrId].Wac);  // WAC

      DEBUG ((DEBUG_INFO, "RAC programmed in IMR %d = %lx \n", ImrId, MmioRead64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset + RELATIVE_RAC_OFFSET)));
      DEBUG ((DEBUG_INFO, "WAC programmed in IMR %d = %lx \n", ImrId, MmioRead64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset + RELATIVE_WAC_OFFSET)));
    }
  }

  //
  // Check IMRn vs IMR1M and program its Base.
  //
  if ((ImrId >= IMR_NPK) && (ImrId < IMR_1M_GSM)) {
    //
    // Enable Address Range and program IMRn Base address
    //
    ImrnBase.Data = MmioRead64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset);
    ImrnBase.Bits.Disable = 0x0;
    ImrnBase.Bits.IMRn_BASE_RW = RShiftU64 (Base, N_IMR_BASE_LIMIT_ADDRESS_OFFSET);
    MmioWrite64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset, ImrnBase.Data);
  } else if ((ImrId >= IMR_1M_GSM) && (ImrId < IMR_MAX)) {
    //
    // Enable Address Range and program IMR1M Base address
    //
    Imr1mBase.Data = MmioRead64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset);
    Imr1mBase.Bits.Disable = 0x0;
    Imr1mBase.Bits.IMR1Mn_BASE_RW = RShiftU64 (Base, N_IMR_1M_BASE_LIMIT_ADDRESS_OFFSET);
    MmioWrite64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset, Imr1mBase.Data);
  }

  DEBUG ((DEBUG_INFO, "Base programmed in IMR %d = %lx \n", ImrId, MmioRead64 ((UINTN) MchBar + mImrTable[ImrId].BaseAddrRegOffset)));

  return EFI_SUCCESS;
}

/**
  Program IMR and IMR_1M for all IPs.

  @param[in] TsegSize      - Size of TSEG
**/
VOID
ImrInit (
  IN UINT32   TsegSize
)
{
  EFI_STATUS                   Status;
  UINT8                        ImrIndex;
  UINT64                       Base;
  UINT64                       Size;
  UINT8                        TotalDprSizeMB;

  Base = 0;
  Size = 0;

  CalculateTotalDprMemorySize (&TotalDprSizeMB);

  for (ImrIndex = 0; ImrIndex < IMR_MAX; ++ImrIndex) {
    switch (mImrTable[ImrIndex].ImrId) {
      case IMR_1M_DPR:
        Base = (UINT64) (GetHostBridgeRegisterData (HostBridgeTsegMb, HostBridgeTsegMbFullData) - ((UINT32) TotalDprSizeMB << 20));
        Size = LShiftU64 ((UINT32) TotalDprSizeMB, 20);
        DEBUG ((DEBUG_INFO, "DprBase = %lx \n", Base));
        DEBUG ((DEBUG_INFO, "DprSize = %lx \n", Size));
        break;
      case IMR_1M_GSM:
        if (FALSE != IGpuIsSupported()) {
          Base = GetHostBridgeRegisterData (HostBridgeBgsm, HostBridgeBgsmFullData);
          Size = (UINT64) IGpuGetGsmSizeInBytes ();
          DEBUG ((DEBUG_INFO, "GsmBase = %lx \n", Base));
          DEBUG ((DEBUG_INFO, "GsmSize = %lx \n", Size));
        } else {
          DEBUG ((DEBUG_INFO, "Skip ProgramGsmImr because IGPU is not supported.\n"));
        }
        break;
      case IMR_1M_DSM:
        if (FALSE != IGpuIsSupported()) {
          Base = GetHostBridgeRegisterData (HostBridgeBdsm, HostBridgeBdsmFullData);
          Size = (UINT64) IGpuGetDsmSizeInBytes ();
          DEBUG ((DEBUG_INFO, "DsmBase = %lx \n", Base));
          DEBUG ((DEBUG_INFO, "DsmSize = %lx \n", Size));
        } else {
          DEBUG ((DEBUG_INFO, "Skip ProgramDsmImr because IGPU is not supported.\n"));
        }
        break;
      case IMR_1M_TSEG:
        Base = GetHostBridgeRegisterData (HostBridgeTsegMb, HostBridgeTsegMbFullData);
        Size = LShiftU64 (TsegSize, 20);
        DEBUG ((DEBUG_INFO, "TsegBase = %lx \n", Base));
        DEBUG ((DEBUG_INFO, "Tsegsize = %lx \n", Size));
        break;
      default:
        Base = 0x0;
        Size = 0x0;
        break;
    }
    if (Base != 0 && Size != 0) {
      Status = SetImr (mImrTable[ImrIndex].ImrId, Base, Size);
      if (Status != EFI_SUCCESS) {
        DEBUG ((DEBUG_WARN, "Fail to program IMR\n"));
        return;
      }
    }
  }
}
