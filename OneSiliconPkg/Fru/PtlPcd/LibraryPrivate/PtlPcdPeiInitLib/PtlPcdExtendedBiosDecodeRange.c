/** @file
  The Extended BIOS decode range enabling.

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

#include "PtlPcdInitPei.h"

#include <ConfigBlock.h>
#include <SpiConfig.h>
#include <Register/SpiRegs.h>
#include <Library/PciSegmentLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiPchDmiLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/SpiInitLib.h>
#include <Library/IocLib.h>
#include <IndustryStandard/Pci30.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdSpiSocLib.h>
#include <SpiHandle.h>
#include <Library/PeiServicesLib.h>
#include <PiPei.h>

/**
  PPI indicates Extended BIOS Decode initialization is done
  and Extended BIOS Region is successfully mapped to memory.
**/
static EFI_PEI_PPI_DESCRIPTOR mExtendedBiosDecodeReadyPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gExtendedBiosDecodeReadyPpiGuid,
  NULL
};

#define SPI_SOC_M_TARGET    0x5140
#define SPI_SOC_S_TARGET    0x5100

/**
  Performs Extended BIOS decode range configuration

  @param[in] SiPolicy     The Silicon Policy PPI instance
**/
VOID
PtlPcdExtendedBiosDecodeRangeInit (
  IN  SI_POLICY_PPI               *SiPolicy
  )
{
  EFI_STATUS      Status;
  UINT16          DecodeTarget;
  SPI_HANDLE      SpiHandle;

  Status = PtlPcdGetSpiHandle (&SpiHandle, SiPolicy);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a - Cannot create SPI handle, exiting, Status %r\n",
      __FUNCTION__, Status
      ));
    return;
  }

  if (SpiHandle.SpiConfig == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "%a - No SpiConfig instance in SPI handle\n",
      __FUNCTION__
      ));
    return;
  }

  if (SpiHandle.SpiConfig->ExtendedBiosDecodeRangeEnable == FALSE) {
    DEBUG ((
      DEBUG_INFO,
      "%a - Extended BIOS decode range is disabled. Skipping initialization.\n",
       __FUNCTION__
       ));
    return;
  }

  //
  // Configure PCH SPI extended BIOS range decoding
  //
  Status = SpiEnableExtendedBiosRangeSupport (&SpiHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a - Error while configuring Extended BIOS range in SPI. Status = %r\n",
      __FUNCTION__, Status
      ));
    return;
  }

  //
  // Configure DMI Source Decode to make Extended BIOS region direct read access
  //
  DecodeTarget = SPI_SOC_M_TARGET;

  Status = IocSetGeneralPurposeMemoryRange (
             0,
             SpiHandle.SpiConfig->ExtendedBiosDecodeRangeBase,
             SpiHandle.SpiConfig->ExtendedBiosDecodeRangeLimit,
             DecodeTarget
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a - Error while configuring Extended BIOS range in IOC. Status = %r\n",
      __FUNCTION__, Status
      ));
  }
  Status = PeiServicesInstallPpi (&mExtendedBiosDecodeReadyPpi);
  ASSERT_EFI_ERROR (Status);
}

/**
  Once main memory has been initialized, initilaize extended BIOS based on policy.

  @param PeiServices        General purpose services available to every PEIM.
  @param NotifyDescriptor   Information about the notify event.
  @param Ppi                Context

  @retval   EFI_SUCCESS   Extended BIOS decode range initialize done.
  @retval   Other         Extended BIOS decode range initialize unsuccesful.
**/
EFI_STATUS
EFIAPI
PtlPcdExtendedBiosDecodeRangeInitializeCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS             Status;
  SI_PREMEM_POLICY_PPI   *SiPolicyPreMemPpi;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPolicyPreMemPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a - Error locating gSiPreMemPolicyPpiGuid. Status = %r\n",
      __FUNCTION__, Status
      ));
    return Status;
  }

  PtlPcdExtendedBiosDecodeRangeInit ((SI_POLICY_PPI *) SiPolicyPreMemPpi);

  return EFI_SUCCESS;
}

// Notify Descriptor used to register a callback
static EFI_PEI_NOTIFY_DESCRIPTOR mInitializeExtendedBiosDecodePpiPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  PtlPcdExtendedBiosDecodeRangeInitializeCallback
  };

/**
  Registers an extended BIOS range decode initialization callback when memory has been discovered.

  @retval   EFI_SUCCESS   Notify descriptor registration done.
  @retval   Other         Notify descriptor registration unsuccesful.
**/
EFI_STATUS
PtlPcdExtendedBiosDecodeRangeRegistration (
  VOID
  )
{
  EFI_STATUS             Status;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  ///
  /// Register a callback  to initialize BIOS decode range
  ///
  Status = PeiServicesNotifyPpi (&mInitializeExtendedBiosDecodePpiPeiNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error registering extended BIOS decode callback.\n"));
  }

  return Status;
}
