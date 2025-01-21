/** @file
  HybridGraphics Dxe driver.
  This DXE driver loads HybridGraphics acpi tables
  for the platform.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#include "HybridGraphicsInfo.h"
#include <Library/DxeHybridGraphicsInitLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/HybridGraphicsInfoFruLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/AslUpdateLib.h>
#include <Protocol/IGpuNvsArea.h>

#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>

#include <Defines/HostBridgeDefines.h>
#include <Library/IGpuInfoLib.h>

///
/// Global Variables
///
GLOBAL_REMOVE_IF_UNREFERENCED VOID                  *VbiosAddress = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                VbiosSize;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                 EndpointBus;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                 mRootBaseAddress;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                 RootPortDev;
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                 RootPortFun;

/**
  Initialize the HybridGraphics support (DXE).

  @retval EFI_SUCCESS          - HybridGraphics initialization complete
  @retval EFI_OUT_OF_RESOURCES - Unable to allocated memory
  @retval EFI_NOT_FOUND        - HG InfoHob not found
  @retval EFI_DEVICE_ERROR     - Error Accessing HG GPIO
**/
EFI_STATUS
DxeHybridGraphicsInit (
  VOID
  )
{
  UINT64        EpBase;
  UINT16        HgSubSystemId;
  HgSubSystemId = PcdGet16 (PcdHgSubSystemId);

  if (HgSubSystemId == 0x2212){
    DEBUG ((DEBUG_INFO, "HG setup option is not selected, exiting the flow\n"));
    return EFI_SUCCESS;
  } else {
    GetHybridGraphicsRpDevFun (&RootPortDev, &RootPortFun);
    DEBUG ((DEBUG_INFO, "dGPU Rootport info[B/D/F] : [0x00/0x%x/0x%x]\n", RootPortDev, RootPortFun));

    ///
    /// Get the PCIe Port Base Address to find Secondary bus number & sub-ordinate bus number
    ///
    mRootBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, RootPortDev, RootPortFun, 0);
    DEBUG ((DEBUG_INFO, "mRootBaseAddress %x\n", mRootBaseAddress));
    ///
    /// Check if Pcie device is present or not.
    ///
    if (PciSegmentRead16 (mRootBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
      DEBUG ((DEBUG_INFO, "Card is not detected\n"));
      return EFI_UNSUPPORTED;
    }

    ///
    /// Get the Endpoint PCIe Port Bus number.
    ///
    EndpointBus = PcdGet8 (PcdSiliconInitTempPciBusMin);
    DEBUG ((DEBUG_INFO, "EndpointBus %d\n", EndpointBus));

    //
    // Assign temporary bus numbers to the root port
    //
    PciSegmentAndThenOr32 (
      mRootBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET,
      (UINT32)~B_PCI_BRIDGE_BNUM_SBBN_SCBN,
      ((UINT32)(EndpointBus << 8)) | ((UINT32)(EndpointBus << 16))
      );

    //
    // A config write is required in order for the device to re-capture the Bus number,
    // according to PCI Express Base Specification, 2.2.6.2
    // Write to a read-only register VendorID to not cause any side effects.
    //
    EpBase = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, EndpointBus, SA_TEMP_DGPU_DEV, SA_TEMP_DGPU_FUN, 0);
    PciSegmentWrite16 (EpBase + PCI_VENDOR_ID_OFFSET, 0x8086);

    if (PciSegmentRead16 (EpBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
      DEBUG ((DEBUG_ERROR, "HG:: 0x00/0x%x/0x%x Rootport's Endpoint Bus Not found\n", RootPortDev, RootPortFun));
      return EFI_UNSUPPORTED;
    }

    if ((PciSegmentRead16(EpBase + PCI_VENDOR_ID_OFFSET)) == AMD_VID) {
      DEBUG ((DEBUG_INFO, "HG:: Found a AMD PCIE graphics card\n"));
    } else if ((PciSegmentRead16(EpBase + PCI_VENDOR_ID_OFFSET)) == NVIDIA_VID) {
      DEBUG ((DEBUG_INFO, "HG:: Found a NVIDIA PCIE graphics card\n"));
    } else {
      ///
      /// either means the Device ID is not on the list of devices we know
      ///
      DEBUG ((DEBUG_INFO, "HG:: Found a PCIE graphics card [not Nvidia/not AMD]\n"));
    }

    //
    // Clear temporary bus number
    //
    PciSegmentAnd32 (mRootBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, (UINT32) ~B_PCI_BRIDGE_BNUM_SBBN_SCBN);
    return EFI_SUCCESS;
  }
}

