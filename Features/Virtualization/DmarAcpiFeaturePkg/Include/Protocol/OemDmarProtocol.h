/** @file
  OemDmarProtocol definitions

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

#ifndef _OEM_DMAR_PROTOCOL_H_
#define _OEM_DMAR_PROTOCOL_H_

#include <Protocol/VtdTopologyProtocol.h>

///
/// OEM DMAR Protocol
/// This protocol is intended for OEMs which want to add some additional remapping structures into DMAR ACPI table.
///

#define OEM_DMAR_PROTOCOL_REVISION  1
typedef struct _OEM_DMAR_PROTOCOL_ OEM_DMAR_PROTOCOL;

/**
  This function returns the number of BIOS allocated reserved memory regions (defined by OEMs) which may be DMA targets.

  @param[in]  This                           Pointer to the OEM_DMAR_PROTOCOL instance.
  @param[out] NumberOfReservedMemoryRegions  The number of BIOS allocated reserved memory regions (defined by OEMs)
                                             which may be DMA targets.

  @retval EFI_SUCCESS                        Command succeeded.
  @retval EFI_INVALID_PARAMETER              The input parameters are invalid.
**/
typedef
EFI_STATUS
(*OEM_GET_NUMBER_OF_RESERVED_MEMORY_REGIONS) (
  IN  OEM_DMAR_PROTOCOL  *This,
  OUT UINT32             *NumberOfReservedMemoryRegions
  );

/**
  This function returns information about BIOS allocated reserved memory region (defined by OEMs) along with a list of
  devices which may require access to this specified reserved memory region.

  @attention  In DeviceList MUST be only devices which are enabled.
  @attention  A caller is responsible to free DeviceList after calling this function.

  @param[in]  This                    Pointer to the OEM_DMAR_PROTOCOL instance.
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
(*OEM_GET_RESERVERD_MEMORY_REGION_INFO) (
  IN  OEM_DMAR_PROTOCOL            *This,
  IN  UINT32                       ReservedMemoryRegionId,
  OUT RESERVED_MEMORY_REGION_INFO  *Info,
  OUT DEVICE_INFO                  **DeviceList   OPTIONAL,
  OUT UINT32                       *ListLength
  );

/**
  This protocol provides interfaces to get information about additional remapping structures defined by OEMs.

  It is OEM's responsibility to implement these functions.

  <b>Revision 1:</b>
  - Initial version.
**/
struct _OEM_DMAR_PROTOCOL_ {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT32                                       Revision;
  OEM_GET_NUMBER_OF_RESERVED_MEMORY_REGIONS    GetNumberOfReservedMemoryRegions;
  OEM_GET_RESERVERD_MEMORY_REGION_INFO         GetReservedMemoryRegionInfo;
};

extern EFI_GUID  gOemDmarProtocolGuid;

#endif // _OEM_DMAR_PROTOCOL_H_
