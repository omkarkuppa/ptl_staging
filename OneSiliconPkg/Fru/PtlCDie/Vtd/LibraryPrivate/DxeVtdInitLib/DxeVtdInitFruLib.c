/** @file
  DXE Fru Library to initialize Vtd

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
#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/VtdInfoLib.h>
#include <Library/TmeInfoLib.h>
#include <Protocol/VtdTopologyProtocol.h>
#include <Library/CpuPlatformLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PchInfoHob.h>
#include <IGpuDataHob.h>

#define MAX_PHYSICAL_ADDRESS_SIZE_WITH_MKTME  46

#define R_IO_APIC_MEM_INDEX_OFFSET            0x00000000U
#define R_IO_APIC_MEM_DATA_OFFSET             0x00000010U

typedef struct {
  VTD_TOPOLOGY_PROTOCOL    Public;
  struct {
    struct {
      VTD_ENGINE_INFO    Info;
      DEVICE_INFO        *DeviceList;
      UINT32             DeviceCount;
    } VtdEngine[3];
  } Private;
} VTD_TOPOLOGY_PROTOCOL_INTERNAL;

/**
  Get IOAPIC ID from LPC

  @retval APIC ID
**/
UINT8
GetIoApicId1 (
  VOID
  )
{
  UINT32  IoApicAddress;
  UINT32  IoApicId;

  IoApicAddress = PcdGet32 (PcdSiIoApicBaseAddress);
  ///
  /// Get current IO APIC ID
  ///
  MmioWrite8 ((UINTN)IoApicAddress + R_IO_APIC_MEM_INDEX_OFFSET, 0);
  IoApicId = MmioRead32 ((UINTN)IoApicAddress + R_IO_APIC_MEM_DATA_OFFSET) >> 24;

  return (UINT8)IoApicId;
}


/**
  This function returns information about a system.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[out] SystemInfo          The information about a system.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
**/
EFI_STATUS
GetSystemInfo (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  OUT SYSTEM_INFO            *SystemInfo
  )
{
  if ((This == NULL) || (SystemInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (IsTmeActivated ()) {
    SystemInfo->HostAddressWidth = MAX_PHYSICAL_ADDRESS_SIZE_WITH_MKTME - 1;
  } else {
    SystemInfo->HostAddressWidth = GetMaxPhysicalAddressSize () - 1;
  }
  SystemInfo->NumaSupported = FALSE;
  SystemInfo->TotalSegments = 1;

  return EFI_SUCCESS;
}

/**
  This function returns the number of VT-d engines present in the platform.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[out] HostAddressWidth    The number of VT-d engines present in the system.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
**/
EFI_STATUS
GetNumberOfVtdEngines (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  OUT UINT32                 *NumberOfVtdEngines
  )
{
  UINT32  Index;
  UINT32  NumberOfEnableVtdEngines;

  if ((This == NULL) || (NumberOfVtdEngines == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  NumberOfEnableVtdEngines = 0;

  for (Index = 0; Index < VTD_ENGINE_NUMBER; Index++) {
    if (IsVtdEngineEnabled (Index)) {
      NumberOfEnableVtdEngines++;
    }
  }

  *NumberOfVtdEngines = NumberOfEnableVtdEngines;

  return EFI_SUCCESS;
}

/**
  This function returns information about a specific VT-d engine.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[in]  VtdEngineId         VT-d engine number.
  @param[out] Info                The information about a specific VT-d engine.
                                    @see VTD_ENGINE_INFO

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
**/
EFI_STATUS
GetVtdEngineInfo1 (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT32                 VtdEngineId,
  OUT VTD_ENGINE_INFO        *Info
  )
{
  UINT32                          Index;
  UINT32                          EnabledVtdEngineId;
  VTD_TOPOLOGY_PROTOCOL_INTERNAL  *VtdTopologyProtocol;

  if ((This == NULL) || (Info == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  VtdTopologyProtocol = (VTD_TOPOLOGY_PROTOCOL_INTERNAL *)This;

  EnabledVtdEngineId = 0;

  for (Index = 0; Index < VTD_ENGINE_NUMBER; Index++) {
    if (IsVtdEngineEnabled (Index)) {
      if (VtdEngineId == EnabledVtdEngineId) {
        break;
      }

      EnabledVtdEngineId++;
    }
  }

  if (Index >= VTD_ENGINE_NUMBER) {
    return EFI_UNSUPPORTED;
  }

  CopyMem (Info, &(VtdTopologyProtocol->Private.VtdEngine[Index].Info), sizeof (VTD_ENGINE_INFO));

  return EFI_SUCCESS;
}

/**
  This function returns a list of devices which are under a scope of the specific VT-d engine.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[in]  VtdEngineId         VT-d engine number.
  @param[out] DeviceList          The list of DEVICE_INFO entries. @see DEVICE_INFO
  @param[out] ListLength          The number of elements in the DeviceList.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
  @retval EFI_UNSUPPORTED         This function is unsupported.
**/
EFI_STATUS
GetPciDevices (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT32                 VtdEngineId,
  OUT DEVICE_INFO            **DeviceList  OPTIONAL,
  OUT UINT32                 *ListLength
  )
{
  UINT32                          Index;
  UINT32                          EnabledVtdEngineId;
  DEVICE_INFO                     *LocalDeviceList;
  DEVICE_INFO                     *ReturnDeviceList;
  UINT32                          DeviceCount;
  UINT32                          DeviceIndex;
  UINT16                          VendorId;
  UINT32                          NumberOfEnabledDevices;
  VTD_TOPOLOGY_PROTOCOL_INTERNAL  *VtdTopologyProtocol;
  BOOLEAN                         IncludePciAll;

  if ((This == NULL) || (ListLength == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  VtdTopologyProtocol = (VTD_TOPOLOGY_PROTOCOL_INTERNAL *)This;

  EnabledVtdEngineId = 0;

  for (Index = 0; Index < VTD_ENGINE_NUMBER; Index++) {
    if (IsVtdEngineEnabled (Index)) {
      if (VtdEngineId == EnabledVtdEngineId) {
        break;
      }

      EnabledVtdEngineId++;
    }
  }

  if (Index >= VTD_ENGINE_NUMBER) {
    return EFI_UNSUPPORTED;
  }

  LocalDeviceList = VtdTopologyProtocol->Private.VtdEngine[Index].DeviceList;
  DeviceCount     = VtdTopologyProtocol->Private.VtdEngine[Index].DeviceCount;
  IncludePciAll   = VtdTopologyProtocol->Private.VtdEngine[Index].Info.IncludePciAll;

  NumberOfEnabledDevices = 0;
  ReturnDeviceList       = NULL;

  if (LocalDeviceList != NULL) {
    for (DeviceIndex = 0; DeviceIndex < DeviceCount; DeviceIndex++) {
      if ((LocalDeviceList[DeviceIndex].Type == PciEndpointDevice) ||
          (LocalDeviceList[DeviceIndex].Type == PciBridge))
      {
        //
        // In case of VT-d engine instance with INCLUDE_PCI_ALL set, don't add devices of type 01h and 02h
        //
        if (IncludePciAll) {
          continue;
        }

        VendorId = PciSegmentRead16 (
                     PCI_SEGMENT_LIB_ADDRESS (
                       0,
                       LocalDeviceList[DeviceIndex].Bdf.Bus,
                       LocalDeviceList[DeviceIndex].Bdf.Device,
                       LocalDeviceList[DeviceIndex].Bdf.Function,
                       PCI_VENDOR_ID_OFFSET
                       )
                     );

        if (VendorId == 0xFFFF) {
          continue;
        }
      }

      if (ReturnDeviceList == NULL) {
        ReturnDeviceList = AllocatePool (sizeof (DEVICE_INFO));
      } else {
        ReturnDeviceList = ReallocatePool (
                             NumberOfEnabledDevices * sizeof (DEVICE_INFO),
                             (NumberOfEnabledDevices + 1) * sizeof (DEVICE_INFO),
                             ReturnDeviceList
                             );
      }

      if (ReturnDeviceList == NULL) {
      return EFI_OUT_OF_RESOURCES;
      }

      CopyMem (&ReturnDeviceList[NumberOfEnabledDevices], &LocalDeviceList[DeviceIndex], sizeof (DEVICE_INFO));
      NumberOfEnabledDevices++;
    }
  }

  *ListLength = NumberOfEnabledDevices;
  if (DeviceList != NULL) {
    *DeviceList = ReturnDeviceList;
  }

  return EFI_SUCCESS;
}

/**
  This function returns a list of SoC integrated devices having address translation
  cache (ATC) which are under a scope of the specific VT-d engine.

  @attention Bits 1-7 in DeviceList[].Flags field are reserved and MUST be 0, only
             bit 0 may be set.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[in]  VtdEngineId         VT-d engine number.
  @param[out] DeviceList          The list of DEVICE_INFO entries.
                                    @see DEVICE_INFO definition
  @param[out] ListLength          The number of elements in the DeviceList.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
  @retval EFI_UNSUPPORTED         This function is unsupported.
**/
EFI_STATUS
GetPciDevicesWithAtc (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT32                 VtdEngineId,
  OUT DEVICE_INFO            **DeviceList,
  OUT UINT32                 *ListLength
  )
{
  UINT32                          Index;
  UINT32                          EnabledVtdEngineId;
  DEVICE_INFO                     *LocalDeviceList;
  DEVICE_INFO                     *ReturnDeviceList;
  UINT32                          DeviceCount;
  UINT32                          DeviceIndex;
  UINT16                          VendorId;
  UINT32                          NumberOfEnabledDevices;
  VTD_TOPOLOGY_PROTOCOL_INTERNAL  *VtdTopologyProtocol;

  if ((This == NULL) || (ListLength == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  VtdTopologyProtocol = (VTD_TOPOLOGY_PROTOCOL_INTERNAL *)This;

  EnabledVtdEngineId = 0;

  for (Index = 0; Index < VTD_ENGINE_NUMBER; Index++) {
    if (IsVtdEngineEnabled (Index)) {
      if (VtdEngineId == EnabledVtdEngineId) {
        break;
      }

      EnabledVtdEngineId++;
    }
  }

  if (Index >= VTD_ENGINE_NUMBER) {
    return EFI_UNSUPPORTED;
  }

  LocalDeviceList = VtdTopologyProtocol->Private.VtdEngine[Index].DeviceList;
  DeviceCount     = VtdTopologyProtocol->Private.VtdEngine[Index].DeviceCount;

  NumberOfEnabledDevices = 0;
  ReturnDeviceList       = NULL;

  if (LocalDeviceList != NULL) {
    for (DeviceIndex = 0; DeviceIndex < DeviceCount; DeviceIndex++) {
      if (LocalDeviceList[DeviceIndex].Type == PciEndpointDevice) {

        // skip Camarillo device
        if ((LocalDeviceList[DeviceIndex].Bdf.Bus == 0) &&
            (LocalDeviceList[DeviceIndex].Bdf.Device == 0x04) &&
            (LocalDeviceList[DeviceIndex].Bdf.Function == 0)) {
          continue;
        }

        // skip Telemetry device
        if ((LocalDeviceList[DeviceIndex].Bdf.Bus == 0) &&
            (LocalDeviceList[DeviceIndex].Bdf.Device == 0x0A) &&
            (LocalDeviceList[DeviceIndex].Bdf.Function == 0)) {
          continue;
        }

        VendorId = PciSegmentRead16 (
                     PCI_SEGMENT_LIB_ADDRESS (
                       0,
                       LocalDeviceList[DeviceIndex].Bdf.Bus,
                       LocalDeviceList[DeviceIndex].Bdf.Device,
                       LocalDeviceList[DeviceIndex].Bdf.Function,
                       PCI_VENDOR_ID_OFFSET
                       )
                     );

        if (VendorId == 0xFFFF) {
          continue;
        }

        if (ReturnDeviceList == NULL) {
          ReturnDeviceList = AllocatePool (sizeof (DEVICE_INFO));
        } else {
          ReturnDeviceList = ReallocatePool (
                               NumberOfEnabledDevices * sizeof (DEVICE_INFO),
                               (NumberOfEnabledDevices + 1) * sizeof (DEVICE_INFO),
                               ReturnDeviceList
                               );
        }

       if (ReturnDeviceList == NULL) {
       return EFI_OUT_OF_RESOURCES;
       }

        CopyMem (&ReturnDeviceList[NumberOfEnabledDevices], &LocalDeviceList[DeviceIndex], sizeof (DEVICE_INFO));
        // check whether BIT4 (ATC_REQUIRED) in Flags is set, if so, then set BIT0 for SATC
        ReturnDeviceList[NumberOfEnabledDevices].Flags =
          ((ReturnDeviceList[NumberOfEnabledDevices].Flags & BIT4) != 0) ? BIT0 : 0;
        NumberOfEnabledDevices++;
      }
    }
  }

  *ListLength = NumberOfEnabledDevices;
  if (DeviceList != NULL) {
    *DeviceList = ReturnDeviceList;
  }

  return EFI_SUCCESS;
}

/**
  This function returns a list of SoC integrated devices that have special properties
  and are under a scope of the specific VT-d engine.

  @attention Bits 5-7 in DeviceList[].Flags field are reserved and MUST be 0, only
             bits 0-4 may be set.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[in]  VtdEngineId         VT-d engine number.
  @param[out] DeviceList          The list of DEVICE_INFO entries.
                                    @see DEVICE_INFO definition
  @param[out] ListLength          The number of elements in the DeviceList.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
  @retval EFI_UNSUPPORTED         This function is unsupported.
**/
EFI_STATUS
GetDevicesWithProperties (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT32                 VtdEngineId,
  OUT DEVICE_INFO            **DeviceList,
  OUT UINT32                 *ListLength
  )
{
  UINT32                          Index;
  UINT32                          EnabledVtdEngineId;
  DEVICE_INFO                     *LocalDeviceList;
  DEVICE_INFO                     *ReturnDeviceList;
  UINT32                          DeviceCount;
  UINT32                          DeviceIndex;
  UINT16                          VendorId;
  UINT32                          NumberOfEnabledDevices;
  VTD_TOPOLOGY_PROTOCOL_INTERNAL  *VtdTopologyProtocol;

  if ((This == NULL) || (ListLength == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  VtdTopologyProtocol = (VTD_TOPOLOGY_PROTOCOL_INTERNAL *)This;

  EnabledVtdEngineId = 0;

  for (Index = 0; Index < VTD_ENGINE_NUMBER; Index++) {
    if (IsVtdEngineEnabled (Index)) {
      if (VtdEngineId == EnabledVtdEngineId) {
        break;
      }

      EnabledVtdEngineId++;
    }
  }

  if (Index >= VTD_ENGINE_NUMBER) {
    return EFI_UNSUPPORTED;
  }

  LocalDeviceList = VtdTopologyProtocol->Private.VtdEngine[Index].DeviceList;
  DeviceCount     = VtdTopologyProtocol->Private.VtdEngine[Index].DeviceCount;

  NumberOfEnabledDevices = 0;
  ReturnDeviceList       = NULL;

  if (LocalDeviceList != NULL) {
    for (DeviceIndex = 0; DeviceIndex < DeviceCount; DeviceIndex++) {
      if (LocalDeviceList[DeviceIndex].Type == PciEndpointDevice) {

        // skip Camarillo device
        if ((LocalDeviceList[DeviceIndex].Bdf.Bus == 0) &&
            (LocalDeviceList[DeviceIndex].Bdf.Device == 0x04) &&
            (LocalDeviceList[DeviceIndex].Bdf.Function == 0)) {
          continue;
        }

        // skip Telemetry device
        if ((LocalDeviceList[DeviceIndex].Bdf.Bus == 0) &&
            (LocalDeviceList[DeviceIndex].Bdf.Device == 0x0A) &&
            (LocalDeviceList[DeviceIndex].Bdf.Function == 0)) {
          continue;
        }

        VendorId = PciSegmentRead16 (
                     PCI_SEGMENT_LIB_ADDRESS (
                       0,
                       LocalDeviceList[DeviceIndex].Bdf.Bus,
                       LocalDeviceList[DeviceIndex].Bdf.Device,
                       LocalDeviceList[DeviceIndex].Bdf.Function,
                       PCI_VENDOR_ID_OFFSET
                       )
                     );

        if (VendorId == 0xFFFF) {
          continue;
        }

        if (ReturnDeviceList == NULL) {
          ReturnDeviceList = AllocatePool (sizeof (DEVICE_INFO));
        } else {
          ReturnDeviceList = ReallocatePool (
                               NumberOfEnabledDevices * sizeof (DEVICE_INFO),
                               (NumberOfEnabledDevices + 1) * sizeof (DEVICE_INFO),
                               ReturnDeviceList
                               );
        }

       if (ReturnDeviceList == NULL) {
       return EFI_OUT_OF_RESOURCES;
       }

        CopyMem (&ReturnDeviceList[NumberOfEnabledDevices], &LocalDeviceList[DeviceIndex], sizeof (DEVICE_INFO));
        NumberOfEnabledDevices++;
      }
    }
  }

  *ListLength = NumberOfEnabledDevices;
  if (DeviceList != NULL) {
    *DeviceList = ReturnDeviceList;
  }

  return EFI_SUCCESS;
}

/**
  This function returns the number of BIOS allocated reserved memory regions which may be DMA targets.

  @param[in]  This                           Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[out] NumberOfReservedMemoryRegions  The number of BIOS allocated reserved memory regions which may be DMA
                                             targets.

  @retval EFI_SUCCESS                        Command succeeded.
  @retval EFI_INVALID_PARAMETER              The input parameters are invalid.
**/
EFI_STATUS
GetNumberOfReservedMemoryRegions (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  OUT UINT32                 *NumberOfReservedMemoryRegions
  )
{
  IGPU_DATA_HOB                   *IGpuDataHob;

  IGpuDataHob = NULL;
  *NumberOfReservedMemoryRegions = 0;

  IGpuDataHob = (IGPU_DATA_HOB *) GetFirstGuidHob (&gIGpuDataHobGuid);

  if (IGpuDataHob != NULL) {
    if ((IGpuDataHob->Gsm2BaseAddress > 0) && (IGpuDataHob->Gsm2Size > 0)) {
        *NumberOfReservedMemoryRegions += 1;
    }
  }

  DEBUG ((DEBUG_INFO, "NumberOfReservedMemoryRegions = %d\n", *NumberOfReservedMemoryRegions));

  return EFI_SUCCESS;
}

/**
  This function returns a info about BIOS allocated reserved memory region along with a list of devices which may
  require access to this specified reserved memory region.

  @attention  In DeviceList MUST be only devices which are enabled.
  @attention  A caller is responsible to free DeviceList after calling this function.

  @param[in]  This                    Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[in]  ReservedMemoryRegionId  Reserved memory region id.
  @param[out] DeviceList              The list of DEVICE_INFO entries. @see DEVICE_INFO
                                      Fields Bdf, Type in DeviceList MUST be filled.
  @param[out] ListLength              The number of elements in the DeviceList.

  @retval EFI_SUCCESS                 Command succeeded.
  @retval EFI_INVALID_PARAMETER       The input parameters are invalid.
  @retval EFI_UNSUPPORTED             This function is unsupported.
**/
EFI_STATUS
GetReservedMemoryRegionInfo (
  IN  VTD_TOPOLOGY_PROTOCOL        *This,
  IN  UINT32                       ReservedMemoryRegionId,
  OUT RESERVED_MEMORY_REGION_INFO  *Info,
  OUT DEVICE_INFO                  **DeviceList   OPTIONAL,
  OUT UINT32                       *ListLength
  )
{
  DEVICE_INFO                     *LocalDeviceList;
  DEVICE_INFO                     *ReturnDeviceList;
  UINT32                          DeviceCount;
  UINT32                          DeviceIndex;
  UINT16                          VendorId;
  UINT32                          NumberOfEnabledDevices;
  VTD_TOPOLOGY_PROTOCOL_INTERNAL  *VtdTopologyProtocol;
  IGPU_DATA_HOB                   *IGpuDataHob;

  IGpuDataHob = NULL;

  DEBUG ((DEBUG_INFO, "GetReservedMemoryRegionInfo Start\n"));

  if ((This == NULL) || (ListLength == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (ReservedMemoryRegionId == GFX_VTD) {
    IGpuDataHob = (IGPU_DATA_HOB *) GetFirstGuidHob (&gIGpuDataHobGuid);

    VtdTopologyProtocol = (VTD_TOPOLOGY_PROTOCOL_INTERNAL *)This;

    LocalDeviceList = VtdTopologyProtocol->Private.VtdEngine[GFX_VTD].DeviceList;
    DeviceCount     = VtdTopologyProtocol->Private.VtdEngine[GFX_VTD].DeviceCount;
    DEBUG ((DEBUG_INFO, "DeviceCount = %x\n", DeviceCount));

    NumberOfEnabledDevices = 0;
    ReturnDeviceList       = NULL;

    if (LocalDeviceList != NULL) {
      for (DeviceIndex = 0; DeviceIndex < DeviceCount; DeviceIndex++) {
        if (LocalDeviceList[DeviceIndex].Type == PciEndpointDevice) {

          VendorId = PciSegmentRead16 (
                      PCI_SEGMENT_LIB_ADDRESS (
                        0,
                        LocalDeviceList[DeviceIndex].Bdf.Bus,
                        LocalDeviceList[DeviceIndex].Bdf.Device,
                        LocalDeviceList[DeviceIndex].Bdf.Function,
                        PCI_VENDOR_ID_OFFSET
                        )
                      );

          if (VendorId == 0xFFFF) {
            continue;
          }

          if (ReturnDeviceList == NULL) {
            ReturnDeviceList = AllocatePool (sizeof (DEVICE_INFO));
          } else {
            ReturnDeviceList = ReallocatePool (
                                NumberOfEnabledDevices * sizeof (DEVICE_INFO),
                                (NumberOfEnabledDevices + 1) * sizeof (DEVICE_INFO),
                                ReturnDeviceList
                                );
          }

          if (ReturnDeviceList == NULL) {
            return EFI_OUT_OF_RESOURCES;
          }

          CopyMem (&ReturnDeviceList[NumberOfEnabledDevices], &LocalDeviceList[DeviceIndex], sizeof (DEVICE_INFO));
          NumberOfEnabledDevices++;
        }
      }
    }

    *ListLength = NumberOfEnabledDevices;
    if (DeviceList != NULL) {
      *DeviceList = ReturnDeviceList;
    }

    if (IGpuDataHob != NULL) {
      if ((IGpuDataHob->Gsm2BaseAddress > 0) && (IGpuDataHob->Gsm2Size > 0)) {
        Info->BaseAddress  = IGpuDataHob->Gsm2BaseAddress;
        Info->LimitAddress = (Info->BaseAddress) + IGpuDataHob->Gsm2Size - 1;
        DEBUG ((DEBUG_INFO, "RMRR Base  address IGD GSM2 %016lX\n", Info->BaseAddress));
        DEBUG ((DEBUG_INFO, "RMRR Limit address IGD GSM2 %016lX\n", Info->LimitAddress));
      }
    }
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

/**
  This function returns a list of PCIe Root-Ports supporting Address Translation Services (ATS) transactions for a given
  segment.

  @attention  In DeviceList MUST be only devices which are enabled.
  @attention  A caller is responsible to free DeviceList after calling this function.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[in]  SegmentId           Segment Id.
  @param[out] Flags               Flags for ATSR entry in a given segment.
  @param[out] DeviceList          The list of DEVICE_INFO entries. @see DEVICE_INFO
                                  In DeviceList should be only devices of PciBridge type.
                                  Fields Bdf, Type in DeviceList MUST be filled.
  @param[out] ListLength          The number of elements in the DeviceList.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
  @retval EFI_UNSUPPORTED         This function is unsupported.
**/
EFI_STATUS
GetRootPortsSupportingAts (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT16                 SegmentId,
  OUT UINT8                  *Flags         OPTIONAL,
  OUT DEVICE_INFO            **DeviceList   OPTIONAL,
  OUT UINT32                 *ListLength
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Initializes a private part of VTD_TOPOLOGY_PROTOCOL_INTERNAL structure.

  @param[in, out] VtdTopologyProtocol   Pointer to VT-d topology protocol

  @retval EFI_SUCCESS                   The function succeeded.
  @retval EFI_INVALID_PARAMETER         VtdTopologyProtocol is NULL
  @retval EFI_NOT_FOUND                 gPchInfoHobGuid not found
**/
EFI_STATUS
InternalVtdTopologyProtocolConstructor (
  IN OUT VTD_TOPOLOGY_PROTOCOL_INTERNAL  *VtdTopologyProtocol
  )
{
  UINT32        Index;
  DEVICE_INFO   *DeviceList;
  VOID          *HobPtr;
  PCH_INFO_HOB  *PchInfoHob;

  DEVICE_INFO  GfxVtdEngineDevices[] = {
    {{ 0, 0x02, 0 }, PciEndpointDevice, 0, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4)}  // GFX
  };

  DEVICE_INFO  NonGfxVtdEngineDevices[] = {
    {{ 0, 0x04, 0 }, PciEndpointDevice, 0, 0},                                  // Camarillo
    {{ 0, 0x05, 0 }, PciEndpointDevice, 0, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4)}, // IPU
    {{ 0, 0x0A, 0 }, PciEndpointDevice, 0, 0},                                  // Telemetry
    {{ 0, 0x0B, 0 }, PciEndpointDevice, 0, (BIT2 | BIT3 | BIT4)},               // NPU
    {{ 0, 0x0C, 0 }, PciEndpointDevice, 0, (BIT3 | BIT4)},                      // IAA
  };

  DEVICE_INFO  GlobalVtdEngineDevices[] = {
    {{ 0, 0, 0 },    IoApicDevice,      0, 0 },                                 // IOAPIC
    {{ 0, 0, 0 },    HpetDevice,        0, 0 },                                 // HPET
  };

  if (VtdTopologyProtocol == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  if (HobPtr == NULL) {
    ASSERT (HobPtr != NULL);
    return EFI_NOT_FOUND;
  }

  PchInfoHob = (PCH_INFO_HOB *)GET_GUID_HOB_DATA (HobPtr);

  //
  // Initialize info about all VT-d engines
  //
  for (Index = 0; Index < VTD_ENGINE_NUMBER; Index++) {
    VtdTopologyProtocol->Private.VtdEngine[Index].Info.Segment         = 0;
    VtdTopologyProtocol->Private.VtdEngine[Index].Info.BarSize         = GetVtdBarSize (Index);
    VtdTopologyProtocol->Private.VtdEngine[Index].Info.BaseAddress     = GetVtdBaseAddress (Index);
    VtdTopologyProtocol->Private.VtdEngine[Index].Info.ProximityDomain = 0;
  }

  VtdTopologyProtocol->Private.VtdEngine[GLOBAL_VTD].Info.IncludePciAll = 1;

  //
  // Create a device list for GFX VT-d engine
  //
  DeviceList = (DEVICE_INFO *)AllocateZeroPool (sizeof (GfxVtdEngineDevices));
  if (DeviceList == NULL) {
    DEBUG ((DEBUG_ERROR, "GfxVtdEngineDevices: Ran out of memory resource.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (DeviceList, &GfxVtdEngineDevices[0], sizeof (GfxVtdEngineDevices));
  VtdTopologyProtocol->Private.VtdEngine[GFX_VTD].DeviceList  = DeviceList;
  VtdTopologyProtocol->Private.VtdEngine[GFX_VTD].DeviceCount = ARRAY_SIZE (GfxVtdEngineDevices);

  //
  // Create a device list for non-GFX VT-d engine
  //
  DeviceList = (DEVICE_INFO *)AllocateZeroPool (sizeof (NonGfxVtdEngineDevices));
  if (DeviceList == NULL) {
    DEBUG ((DEBUG_ERROR, "NonGfxVtdEngineDevices: Ran out of memory resource.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (DeviceList, &NonGfxVtdEngineDevices[0], sizeof (NonGfxVtdEngineDevices));
  VtdTopologyProtocol->Private.VtdEngine[NON_GFX_VTD].DeviceList  = DeviceList;
  VtdTopologyProtocol->Private.VtdEngine[NON_GFX_VTD].DeviceCount = ARRAY_SIZE (NonGfxVtdEngineDevices);

  //
  // Create a device list for global VT-d engine
  //
  for (Index = 0; Index < ARRAY_SIZE (GlobalVtdEngineDevices); Index++) {
    if (GlobalVtdEngineDevices[Index].Type == IoApicDevice) {
      //
      // Update BDF and EnumerationId for IOAPIC
      //
      GlobalVtdEngineDevices[Index].Bdf.Bus       = (UINT8)PchInfoHob->IoApicBusNum;
      GlobalVtdEngineDevices[Index].Bdf.Device    = (UINT8)PchInfoHob->IoApicDevNum;
      GlobalVtdEngineDevices[Index].Bdf.Function  = (UINT8)PchInfoHob->IoApicFuncNum;
      GlobalVtdEngineDevices[Index].EnumarationId = GetIoApicId1 ();
    } else if (GlobalVtdEngineDevices[Index].Type == HpetDevice) {
      //
      // Update BDF for HPET
      //
      GlobalVtdEngineDevices[Index].Bdf.Bus      = (UINT8)PchInfoHob->HpetBusNum;
      GlobalVtdEngineDevices[Index].Bdf.Device   = (UINT8)PchInfoHob->HpetDevNum;
      GlobalVtdEngineDevices[Index].Bdf.Function = (UINT8)PchInfoHob->HpetFuncNum;
    }
  }

  DeviceList = (DEVICE_INFO *)AllocateZeroPool (sizeof (GlobalVtdEngineDevices));
  if (DeviceList == NULL) {
    DEBUG ((DEBUG_ERROR, "GlobalVtdEngineDevices: Ran out of memory resource.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (DeviceList, &GlobalVtdEngineDevices[0], sizeof (GlobalVtdEngineDevices));
  VtdTopologyProtocol->Private.VtdEngine[GLOBAL_VTD].DeviceList  = DeviceList;
  VtdTopologyProtocol->Private.VtdEngine[GLOBAL_VTD].DeviceCount = ARRAY_SIZE (GlobalVtdEngineDevices);

  return EFI_SUCCESS;
}

/**
  This functions installs VT-d topology protocol.
    @see OneSiliconPkg\Include\Protocol\VtdTopologyProtocol.h
**/
VOID
InstallVtdTopologyProtocol (
  VOID
  )
{
  EFI_STATUS                      Status;
  VTD_TOPOLOGY_PROTOCOL_INTERNAL  *VtdTopologyProtocol;
  EFI_HANDLE                      Handle;

  VtdTopologyProtocol = (VTD_TOPOLOGY_PROTOCOL_INTERNAL *)AllocateZeroPool (sizeof (VTD_TOPOLOGY_PROTOCOL_INTERNAL));

  if (VtdTopologyProtocol == NULL) {
    ASSERT (FALSE);
    return;
  }

  Handle = NULL;

  //
  // Initialize a private part of VTD_TOPOLOGY_PROTOCOL_INTERNAL
  //
  Status = InternalVtdTopologyProtocolConstructor (VtdTopologyProtocol);

  if (!EFI_ERROR (Status)) {
    //
    // Initialize a public part of VTD_TOPOLOGY_PROTOCOL_INTERNAL
    //
    VtdTopologyProtocol->Public.Revision                         = VTD_TOPOLOGY_PROTOCOL_REVISION;
    VtdTopologyProtocol->Public.GetSystemInfo                    = GetSystemInfo;
    VtdTopologyProtocol->Public.GetNumberOfVtdEngines            = GetNumberOfVtdEngines;
    VtdTopologyProtocol->Public.GetVtdEngineInfo                 = GetVtdEngineInfo1;
    VtdTopologyProtocol->Public.GetDevices                       = GetPciDevices;
    VtdTopologyProtocol->Public.GetPciDevicesWithAtc             = GetPciDevicesWithAtc;
    VtdTopologyProtocol->Public.GetDevicesWithProperties         = GetDevicesWithProperties;
    VtdTopologyProtocol->Public.GetNumberOfReservedMemoryRegions = GetNumberOfReservedMemoryRegions;
    VtdTopologyProtocol->Public.GetReservedMemoryRegionInfo      = GetReservedMemoryRegionInfo;
    VtdTopologyProtocol->Public.GetRootPortsSupportingAts        = GetRootPortsSupportingAts;

    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gVtdTopologyProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    VtdTopologyProtocol
                    );
  }

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (FALSE);

    if (VtdTopologyProtocol->Private.VtdEngine[GFX_VTD].DeviceList != NULL) {
      gBS->FreePool(VtdTopologyProtocol->Private.VtdEngine[GFX_VTD].DeviceList);
    }

    if (VtdTopologyProtocol->Private.VtdEngine[NON_GFX_VTD].DeviceList != NULL) {
      gBS->FreePool(VtdTopologyProtocol->Private.VtdEngine[NON_GFX_VTD].DeviceList);
    }

    if (VtdTopologyProtocol->Private.VtdEngine[GLOBAL_VTD].DeviceList != NULL) {
      gBS->FreePool(VtdTopologyProtocol->Private.VtdEngine[GLOBAL_VTD].DeviceList);
    }

    gBS->FreePool (VtdTopologyProtocol);
    return;
  }
}
