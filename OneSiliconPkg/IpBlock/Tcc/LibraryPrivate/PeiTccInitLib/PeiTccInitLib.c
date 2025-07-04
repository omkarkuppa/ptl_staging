/** @file
  PEI TCC Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MsrFruLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/MemoryAllocationLib.h>
#include <TccDataHob.h>
#include <TccConfig.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include <Ppi/MpServices.h>
#include <Library/CpuCommonLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/TimerLib.h>
#include <Library/PchPcrLib.h>
#include <Library/CpuRegbarAccessLib.h>
#include <Ppi/MpServices2.h>
#include <Library/CpuInfoFruLib.h>
#include <Register/Cpuid.h>
#include <CpuPowerMgmtBasicConfig.h>

typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_STRUCT;

GLOBAL_REMOVE_IF_UNREFERENCED TCC_CONFIG               *mTccConfig       = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EDKII_PEI_MP_SERVICES2_PPI  *gMpServices2Ppi   = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED TCC_INFO_HOB             *mTccInfoHob      = NULL;

/**
  Initialize TCC configuration for TCC performance

  @param[in] PeiServices          Pointer to PEI Services Table.
  @param[in] SiPolicyPpi          The SI Policy PPI instance
**/
VOID
EFIAPI
TccInit (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN  SI_POLICY_PPI           *SiPolicyPpi
  )
{
  EFI_STATUS                  Status;

  DEBUG ((DEBUG_INFO, "[TCC] TccInit() - Start\n"));

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTccConfigGuid, (VOID *) &mTccConfig);

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "Couldnt locate TCC Config\n"));
    return;
  }

  Status = PeiServicesLocatePpi (
            &gEdkiiPeiMpServices2PpiGuid,
            0,
            NULL,
            (VOID **) &gMpServices2Ppi);

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "Couldnt locate gEfiPeiMpServicesPpiGuid\n"));
    return;
  }

  DEBUG ((DEBUG_INFO, "[TCC] TccInit() - End\n"));
}
