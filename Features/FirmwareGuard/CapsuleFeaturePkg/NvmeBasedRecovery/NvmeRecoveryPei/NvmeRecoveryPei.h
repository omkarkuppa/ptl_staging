/** @file
  This PEIM driver request Nvme recovery seed to csme, and save it to hob
  produce gCsmeSeedHobGuid instance for other driver.

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

#ifndef _NVME_RECOVERY_PEI_H_
#define _NVME_RECOVERY_PEI_H_

#include <PiPei.h>
#include <Ppi/EndOfPeiPhase.h>
#include <IndustryStandard/Pci.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiMeLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <BupMsgs.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Guid/NvmeRecoveryCommon.h>
#include <Library/CpuPlatformLib.h>
#include <Library/ResiliencySupportLib.h>

#define NVME_BASED_BRAND_IDENTITY_BIT  BIT3
/**
  Bios send seed request to CSME before MRC initialization.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_NOT_FOUND             Couldn't locate HeciPpi or debug token data
  @retval EFI_INVALID_PARAMETER     Parameter invalid
**/
EFI_STATUS
EFIAPI
CsmeSeedRequest (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  );

#endif  // _NVME_RECOVERY_PEI_H_
