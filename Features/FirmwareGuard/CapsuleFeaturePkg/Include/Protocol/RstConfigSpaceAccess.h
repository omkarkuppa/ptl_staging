/**@file
  RST config space access protocol for bios.

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

#ifndef _RST_CONFIG_SPACE_ACCESS_H
#define _RST_CONFIG_SPACE_ACCESS_H

/// Version of EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL
#define EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL_VERSION    0x0001

///
/// Global ID for the EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL
///
#define EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL_GUID                                \
  {                                                                              \
    0x32370e89, 0x5f4a, 0x4ef0, 0xa5, 0x58, 0xce, 0x76, 0xfe, 0xff, 0x2b, 0xbe } \
  }

typedef struct _EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL  EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL;

/**
  Retrieves the address of PCIe Config Space of device (specified by NamespaceId value) managed by VMD controller.

  @param  This                  A pointer to the EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL instance.
  @param  NamespaceId           A NamespaceId value returned by GetNextNamespaced function of NVMe Pass Thru protocol.
  @param  ConfigSpaceAddress    An address of PCIe Config Space of device managed by VMD controller.


  @retval EFI_SUCCESS           The PCIe Config Space address of the device has been retrieved with success.
  @retval EFI_NOT_FOUND         The device specified by NamespaceId couldn't be found.
  @retval EFI_INVALID_PARAMETER ConfigSpaceAddress buffer is NULL or invalid NamespaceId requested.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_RST_CONFIG_SPACE_ACCESS_GET_ADDRESS) (
  IN  EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL  *This,
  IN  UINT32                                NamespaceId,
  OUT EFI_PHYSICAL_ADDRESS                  *ConfigSpaceAddress
  );

/**
  Retrieves the PCI bus/device/function number of the root port before device managed by VMD controller.

  @param  This                  A pointer to the EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL instance.
  @param  NamespaceId           A NamespaceId value returned by GetNextNamespaced function of NVMe Pass Thru protocol.
  @param  BusNumber             PCI bus number of root port
  @param  DeviceNumber          PCI device number of root port
  @param  FunctionNumber        PCI function number of root port

  @retval EFI_SUCCESS           The BusNumber, DeviceNumber and FunctionNumber of the device has been retrieved with success.
  @retval EFI_NOT_FOUND         The device specified by NamespaceId couldn't be found.
  @retval EFI_INVALID_PARAMETER BusNumber, DeviceNumber or FunctionNumber is NULL or invalid NamespaceId requested.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_RST_CONFIG_SPACE_ACCESS_GET_BDF) (
  IN  EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL  *This,
  IN  UINT32                                NamespaceId,
  OUT UINTN                                 *BusNumber,
  OUT UINTN                                 *DeviceNumber,
  OUT UINTN                                 *FunctionNumber
  );


struct _EFI_RST_CONFIG_SPACE_ACCESS_PROTOCOL {
  UINT8                                    Version;
  EFI_RST_CONFIG_SPACE_ACCESS_GET_ADDRESS  GetAddress;
  EFI_RST_CONFIG_SPACE_ACCESS_GET_BDF      GetBDF;
  };

extern EFI_GUID gRSTConfigSpaceAccessGuid;

#endif // _RST_CONFIG_SPACE_ACCESS_H
