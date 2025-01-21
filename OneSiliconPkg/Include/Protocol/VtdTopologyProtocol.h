/** @file
  VtdTopologyProtocol definitions

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

#ifndef _VTD_TOPOLOGY_PROTOCOL_H_
#define _VTD_TOPOLOGY_PROTOCOL_H_

///
/// VT-d Topology Protocol
/// This protocol provides APIs which give information about VT-d topology in the system.
///

#define VTD_TOPOLOGY_PROTOCOL_REVISION  1
typedef struct _VTD_TOPOLOGY_PROTOCOL_ VTD_TOPOLOGY_PROTOCOL;

typedef struct {
  UINT8   HostAddressWidth;
  BOOLEAN NumaSupported;
  UINT16  TotalSegments; ///< Number of segments in system
} SYSTEM_INFO;

typedef struct {
  UINT64     BaseAddress;
  UINT32     BarSize;
  UINT16     Segment;
  UINT32     ProximityDomain;
  BOOLEAN    IncludePciAll;
} VTD_ENGINE_INFO;

typedef struct {
  UINT16    PciSegment;
  UINT64    BaseAddress;
  UINT64    LimitAddress;
} RESERVED_MEMORY_REGION_INFO;
typedef struct {
  UINT8    Bus;
  UINT8    Device   : 5;
  UINT8    Function : 3;
} PCI_BDF;

/**
  Device type
**/
typedef enum {
  PciEndpointDevice = 0x1,
  PciBridge,
  IoApicDevice,
  HpetDevice,
  AcpiNamespaceDevice,
  VtdInfoDeviceTypeMaximum
} DEVICE_TYPE;

typedef struct {
  PCI_BDF        Bdf;
  DEVICE_TYPE    Type;
  UINT8          EnumarationId;
  UINT8          Flags;
} DEVICE_INFO;

/**
  This function returns information about a system.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[out] SystemInfo          The information about a system.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
**/
typedef
EFI_STATUS
(*GET_SYSTEM_INFO) (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  OUT SYSTEM_INFO            *SystemInfo
  );

/**
  This function returns the number of enabled VT-d engines in the platform.

  @attention This function MUST return a number of ENABLED VT-d engines which will exposed in DMAR ACPI table.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[out] NumberOfVtdEngines  The number of VT-d engines present in the system.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
**/
typedef
EFI_STATUS
(*GET_NUMBER_OF_VTD_ENGINES) (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  OUT UINT32                 *NumberOfVtdEngines
  );

/**
  This function returns information about an enabled specific VT-d engine.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[in]  VtdEngineId         VT-d engine number.
  @param[out] Info                The information about a specific VT-d engine.
                                    @see VTD_ENGINE_INFO

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
**/
typedef
EFI_STATUS
(*GET_VTD_ENGINE_INFO) (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT32                 VtdEngineId,
  OUT VTD_ENGINE_INFO        *Info
  );

/**
  This function returns a list of devices which are under a scope of the specific VT-d engine.

  @attention  In DeviceList MUST be only devices which are enabled.
  @attention  A caller is responsible to free DeviceList after calling this function.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[in]  VtdEngineId         VT-d engine number.
  @param[out] DeviceList          The list of PCI DEVICE_INFO. @see DEVICE_INFO
                                  Fields Bdf and Type in DeviceList MUST be filled for each type of device.
                                  For IoApicDevice, HpetDevice and AcpiNamespaceDevice devices, EnumarationId MUST be
                                  also filled with a correct value.
  @param[out] ListLength          The number of elements in the DeviceList.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
  @retval EFI_UNSUPPORTED         This function is unsupported.
**/
typedef
EFI_STATUS
(*GET_DEVICES) (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT32                 VtdEngineId,
  OUT DEVICE_INFO            **DeviceList   OPTIONAL,
  OUT UINT32                 *ListLength
  );

/**
  This function returns a list of SoC integrated devices having address translation cache (ATC)
  which are under a scope of the specific VT-d engine.

  @attention  In DeviceList MUST be only devices which are enabled.
  @attention  A caller is responsible to free DeviceList after calling this function.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[in]  VtdEngineId         VT-d engine number.
  @param[out] DeviceList          The list of DEVICE_INFO entries. @see DEVICE_INFO
                                  In DeviceList should be only devices of PciEndpointDevice type.
                                  Fields Bdf, Type and Flags in DeviceList MUST be filled.
                                  Bits 1-7 in Flags field are reserved and MUST be 0, only bit 0 may be set.
  @param[out] ListLength          The number of elements in the DeviceList.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
  @retval EFI_UNSUPPORTED         This function is unsupported.
**/
typedef
EFI_STATUS
(*GET_PCI_DEVICES_WITH_ATC) (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT32                 VtdEngineId,
  OUT DEVICE_INFO            **DeviceList   OPTIONAL,
  OUT UINT32                 *ListLength
  );

/**
  This function returns a list of SoC integrated devices that have special properties and are under a scope of the
  specific VT-d engine.

  @attention  In DeviceList MUST be only devices which are enabled.
  @attention  A caller is responsible to free DeviceList after calling this function.

  @param[in]  This                Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[in]  VtdEngineId         VT-d engine number.
  @param[out] DeviceList          The list of DEVICE_INFO entries. @see DEVICE_INFO
                                  In DeviceList should be only devices of PciEndpointDevice or AcpiNamespaceDevice type.
                                  Fields Bdf, Type and Flags in DeviceList MUST be filled.
                                  Bits 5-7 in Flags field are reserved and MUST be 0, only bits 0-4 may be set.
  @param[out] ListLength          The number of elements in the DeviceList.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_INVALID_PARAMETER   The input parameters are invalid.
  @retval EFI_UNSUPPORTED         This function is unsupported.
**/
typedef
EFI_STATUS
(*GET_DEVICES_WITH_PROPERTIES) (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT32                 VtdEngineId,
  OUT DEVICE_INFO            **DeviceList   OPTIONAL,
  OUT UINT32                 *ListLength
  );

/**
  This function returns the number of BIOS allocated reserved memory regions which may be DMA targets.

  @param[in]  This                           Pointer to the VTD_TOPOLOGY_PROTOCOL instance.
  @param[out] NumberOfReservedMemoryRegions  The number of BIOS allocated reserved memory regions which may be DMA
                                             targets.

  @retval EFI_SUCCESS                        Command succeeded.
  @retval EFI_INVALID_PARAMETER              The input parameters are invalid.
**/
typedef
EFI_STATUS
(*GET_NUMBER_OF_RESERVED_MEMORY_REGIONS) (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  OUT UINT32                 *NumberOfReservedMemoryRegions
  );

/**
  This function returns information about BIOS allocated reserved memory region along with a list of devices which may
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
typedef
EFI_STATUS
(*GET_RESERVERD_MEMORY_REGION_INFO) (
  IN  VTD_TOPOLOGY_PROTOCOL        *This,
  IN  UINT32                       ReservedMemoryRegionId,
  OUT RESERVED_MEMORY_REGION_INFO  *Info,
  OUT DEVICE_INFO                  **DeviceList   OPTIONAL,
  OUT UINT32                       *ListLength
  );

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
typedef
EFI_STATUS
(*GET_ROOT_PORTS_SUPPORTING_ATS) (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT16                 SegmentId,
  OUT UINT8                  *Flags         OPTIONAL,
  OUT DEVICE_INFO            **DeviceList   OPTIONAL,
  OUT UINT32                 *ListLength
  );

/**
  This protocol provides interfaces to get information about VT-d topology in system (which are needed for creating DMAR
  ACPI table).

  It is silicon's responsibility to implement these functions.

  <b>Revision 1:</b>
  - Initial version.
**/
struct _VTD_TOPOLOGY_PROTOCOL_ {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT32                                   Revision;
  GET_SYSTEM_INFO                          GetSystemInfo;
  GET_NUMBER_OF_VTD_ENGINES                GetNumberOfVtdEngines;
  GET_VTD_ENGINE_INFO                      GetVtdEngineInfo;
  GET_DEVICES                              GetDevices;
  GET_PCI_DEVICES_WITH_ATC                 GetPciDevicesWithAtc;
  GET_DEVICES_WITH_PROPERTIES              GetDevicesWithProperties;
  GET_NUMBER_OF_RESERVED_MEMORY_REGIONS    GetNumberOfReservedMemoryRegions;
  GET_RESERVERD_MEMORY_REGION_INFO         GetReservedMemoryRegionInfo;
  GET_ROOT_PORTS_SUPPORTING_ATS            GetRootPortsSupportingAts;
};

extern EFI_GUID  gVtdTopologyProtocolGuid;

#endif // _VTD_TOPOLOGY_PROTOCOL_H_
