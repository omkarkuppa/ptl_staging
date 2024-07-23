/** @file
  Header file for Silicon Init PEIM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _SI_INIT_MODULE_H_
#define _SI_INIT_MODULE_H_

#include <SiFvi.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/SiPolicy.h>
#include <Library/MeInitLib.h>
#include <Library/CpuPowerOnConfigLib.h>
#include <Library/TxtPeiLib.h>
#include <Library/SmbiosCpuLib.h>
#include <Library/PchDmiLib.h>
#include <SiConfigHob.h>
#include <Library/PsmiInitLib.h>

/**
  PPI function to print Silicon Policy config block settings

  @retval EFI_STATUS       - Locate SiPolicy PPI successfully.
  @retval others           - Locate SiPolicy PPI fail.
 **/
EFI_STATUS
EFIAPI
PeiSiPolicyPrint (
  VOID
  );

/**
  Silicon Init End of PEI callback function. This is the last change before entering DXE and OS when S3 resume.

  @param[in] PeiServices   - Pointer to PEI Services Table.
  @param[in] NotifyDesc    - Pointer to the descriptor for the Notification event that
                             caused this function to execute.
  @param[in] Ppi           - Pointer to the PPI data associated with this function.

  @retval EFI_STATUS       - Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
SiInitOnEndOfPei (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDesc,
  IN VOID                               *Ppi
  );

/**
  Slicon Initializes after PostMem phase Policy PPI produced,
  All required polices must be installed before the callback

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  SiPolicy PPI.

  @retval EFI_SUCCESS             Succeeds.
**/
EFI_STATUS
EFIAPI
SiInitPostMemOnPolicy (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  );

/**
  This function gets registered as a callback to Silicon Init on PCI enumeration complete.

  @param[in]  PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in]  Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SiOnPciEnumerationComplete (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

/**
  This function gets registered as a callback to Silicon Init on end of firmware.

  @param[in]  PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in]  Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SiOnEndOfFirmware (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );
#endif // _SI_INIT_MODULE_H_
