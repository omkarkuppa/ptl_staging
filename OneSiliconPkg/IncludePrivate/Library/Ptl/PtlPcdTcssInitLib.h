/** @file
  Header file for PTL TCSS Init Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef _PTL_SOC_TCSS_INIT_LIB_H_
#define _PTL_SOC_TCSS_INIT_LIB_H_

#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <Uefi/UefiBaseType.h>
#include <Ppi/SiPolicy.h>
#include <TcssPeiConfig.h>
#include <UsbHandle.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Register/HostBridgeRegs.h>

/**
  Check if this chipset Enable TCSS

  @retval BOOLEAN  TRUE if IP enable, FALSE disable
**/
BOOLEAN
EFIAPI
IsItcssEnabled (
  VOID
  );

EFI_STATUS
PtlTcssInitPreMem (
  IN TCSS_PEI_PREMEM_CONFIG      *TcssPeiPreMemConfig,
  IN HOST_BRIDGE_PREMEM_CONFIG   *HostBridgePreMemConfig
  );

EFI_STATUS
PtlTcssInitPostMem (
  SI_POLICY_PPI           *SiPolicy,
  TCSS_PEI_PREMEM_CONFIG  *TcssPeiPreMemConfig,
  TCSS_PEI_CONFIG         *TcssPeiConfig,
  PEI_ITBT_CONFIG         *PeiITbtConfig,
  HOST_BRIDGE_PREMEM_CONFIG *HostBridgePreMemConfig
  );

EFI_STATUS
PtlTcssInitEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

#endif // _PTL_SOC_TCSS_INIT_LIB_H_
