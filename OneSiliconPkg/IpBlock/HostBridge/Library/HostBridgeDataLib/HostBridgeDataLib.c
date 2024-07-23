/** @file
  The GUID definition for Host Bridge Data Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Register/SncuRegs.h>
#include <Library/PciSegmentLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/BaseLib.h>
#include <Ppi/SiPolicy.h>
#include <HostBridgeDataHob.h>
#include <Library/HobLib.h>
#include <IndustryStandard/Pci22.h>

/**
  This function will return the 16-bit Device Id value of Device 0.

  @return the 16-bit value of DID.
**/
UINT16
EFIAPI
GetHostBridgeDeviceId (
  VOID
  )
{
  return (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS(SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, PCI_DEVICE_ID_OFFSET)));
}

/**
  Print HostBridege PCI space in Debug log.

**/
VOID
HostBridgePciPrint (
  VOID
  )
{
  UINT64 PciBase;
  UINT8  i;
  UINT8  j;

  PciBase = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, 0);
  if (PciSegmentRead16 (PciBase + PCI_VENDOR_ID_OFFSET) != 0xFFFF) {
    DEBUG ((DEBUG_INFO, "\nPrinting PCI space for device 0\n  "));
    for (i = 0; i <= 0xF; i++) {
      DEBUG ((DEBUG_INFO, "  %2X", i));
    }
    for (i = 0; i <= 0xF; i++) {
      DEBUG ((DEBUG_INFO, "\n%2X", (i * 0x10)));
      for (j = 0; j <= 0xF; j++) {
        DEBUG ((DEBUG_INFO, "  %2X", PciSegmentRead8 (PciBase + (i * 0x10) + j)));
      }
    }
  }

  DEBUG ((DEBUG_INFO, "\n"));
}
