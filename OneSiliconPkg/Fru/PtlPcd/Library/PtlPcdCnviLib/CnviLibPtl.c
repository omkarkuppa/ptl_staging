/** @file
  CNVi Library.
  All functions from this library are available in PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

@par Specification Reference:
**/

#include <Base.h>
#include <Uefi.h>
#include <Pi/PiMultiPhase.h>
#include <IndustryStandard/Pci30.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcdInfoLib.h>
#include <Register/PchRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Register/GpioAcpiDefines.h>
#include <Library/Ptl/PcdMinimalGpioNativeLib/PtlPcdMinimalGpioNativeLib.h>

/**
  This function checks if CNVi CRF module is present

  @retval TRUE:  CNVi CRF module is present
          FALSE: CNVi CRF module is NOT present
**/
BOOLEAN
CnviCrfModuleIsPresent (
  VOID
  )
{
  return PtlPcdMinimalGpioGetCnviCrfStrapPinStatus ();
}

/**
  Check if CNVi Wifi is Enabled.

  @retval TRUE:  CNVi Wifi is enabled
          FALSE: CNVi Wifi is disabled
**/
BOOLEAN
IsCnviWifiEnabled (
  VOID
  )
{
  if (PciSegmentRead16 (CnviWifiPciCfgBase () + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return FALSE;
  }
  return TRUE;
}

/**
  Check if CNVi BT is Enabled.

  @retval TRUE:  CNVi BT is enabled
          FALSE: CNVi BT is disabled
**/
BOOLEAN
IsCnviBtEnabled (
  VOID
  )
{
  if (PciSegmentRead16 (CnviBtPciCfgBase () + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return FALSE;
  }
  return TRUE;
}
