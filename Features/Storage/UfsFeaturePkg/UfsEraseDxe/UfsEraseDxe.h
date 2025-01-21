/**@file
  Ufs Erase Driver.

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

#ifndef _UFS_ERASE_H_
#define _UFS_ERASE_H_

#include <PiDxe.h>
#include <IndustryStandard/Scsi.h>

#include <Protocol/LoadedImage.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/BlockIo.h>
#include <Protocol/ScsiPassThruExt.h>
#include <Protocol/UfsDeviceConfig.h>
#include <Protocol/EraseBlock.h>
#include <Protocol/UfsEraseBlock.h>
#include <Protocol/ScsiIo.h>

#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

extern EFI_DRIVER_BINDING_PROTOCOL   gUfsEraseDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL   gUfsEraseComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL  gUfsEraseComponentName2;

#define UFS_ERASE_SIGNATURE SIGNATURE_32 ('u', 's', 'e', 's')

#pragma pack(1)
//
// UFS Descriptors
//
typedef enum {
  UfsDeviceDesc     = 0x00,
  UfsConfigDesc     = 0x01,
  UfsUnitDesc       = 0x02,
  UfsInterConnDesc  = 0x04,
  UfsStringDesc     = 0x05,
  UfsGeometryDesc   = 0x07,
  UfsPowerDesc      = 0x08
} UFS_DESC_IDN;

//
// UFS 2.0 Spec Section 14.1.6.2 - Device Descriptor
//
typedef struct {
  UINT8  Length;
  UINT8  DescType;
  UINT8  Device;
  UINT8  DevClass;
  UINT8  DevSubClass;
  UINT8  Protocol;
  UINT8  NumLun;
  UINT8  NumWLun;
  UINT8  BootEn;
  UINT8  DescAccessEn;
  UINT8  InitPowerMode;
  UINT8  HighPriorityLun;
  UINT8  SecureRemovalType;
  UINT8  SecurityLun;
  UINT8  BgOpsTermLat;
  UINT8  InitActiveIccLevel;
  UINT16 SpecVersion;
  UINT16 ManufactureDate;
  UINT8  ManufacturerName;
  UINT8  ProductName;
  UINT8  SerialName;
  UINT8  OemId;
  UINT16 ManufacturerId;
  UINT8  Ud0BaseOffset;
  UINT8  Ud0ConfParamLen;
  UINT8  DevRttCap;
  UINT16 PeriodicRtcUpdate;
  UINT8  Rsvd1[17];
  UINT8  Rsvd2[16];
} UFS_DEV_DESC;

//
// UFS 2.0 Spec Section 14.1.6.9 - 14.1.6.12 - String Descriptor
//
typedef struct {
  UINT8  Length;
  UINT8  DescType;
  CHAR16 Unicode[126];
} UFS_STRING_DESC;
#pragma pack()

typedef struct {
  UINT32                          Signature;
  EFI_HANDLE                      Controller;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_EXT_SCSI_PASS_THRU_PROTOCOL *ScsiInterfaceExt;
  EFI_UFS_DEVICE_CONFIG_PROTOCOL  *UfsDevConfig;
  EFI_UNICODE_STRING_TABLE        *ControllerNameTable;

  UFS_ERASE_INFO                  *Info;
  UFS_ERASE_BLOCK_PROTOCOL        EraseBlock;
  UINT64                          EstimateTime[UFS_SANITIZE_COUNT];
} UFS_ERASE_PRIVATE_DATA;

#define PRIVATE_FROM_ERASEBLOCK(a)  CR (a, UFS_ERASE_PRIVATE_DATA, EraseBlock, UFS_ERASE_SIGNATURE)

/**
  Retrieves a Unicode string that is the user readable name of the driver.

  This function retrieves the user readable name of a driver in the form of a
  Unicode string. If the driver specified by This has a user readable name in
  the language specified by Language, then a pointer to the driver name is
  returned in DriverName, and EFI_SUCCESS is returned. If the driver specified
  by This does not support the language specified by Language,
  then EFI_UNSUPPORTED is returned.

  @param  This[in]              A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param  Language[in]          A pointer to a Null-terminated ASCII string
                                array indicating the language. This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified
                                in RFC 4646 or ISO 639-2 language code format.

  @param  DriverName[out]       A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                driver specified by This in the language
                                specified by Language.

  @retval EFI_SUCCESS           The Unicode string for the Driver specified by
                                This and the language specified by Language was
                                returned in DriverName.

  @retval EFI_INVALID_PARAMETER Language is NULL.

  @retval EFI_INVALID_PARAMETER DriverName is NULL.

  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
EfiDriverComponentNameGetDriverName(
  EFI_COMPONENT_NAME_PROTOCOL*    This,
  CHAR8*                          Language,
  CHAR16**                        DriverName
  );

/**
  Retrieves a Unicode string that is the user readable name of the controller
  that is being managed by a driver.

  This function retrieves the user readable name of the controller specified by
  ControllerHandle and ChildHandle in the form of a Unicode string. If the
  driver specified by This has a user readable name in the language specified by
  Language, then a pointer to the controller name is returned in ControllerName,
  and EFI_SUCCESS is returned.  If the driver specified by This is not currently
  managing the controller specified by ControllerHandle and ChildHandle,
  then EFI_UNSUPPORTED is returned.  If the driver specified by This does not
  support the language specified by Language, then EFI_UNSUPPORTED is returned.

  @param  This[in]              A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or
                                EFI_COMPONENT_NAME_PROTOCOL instance.

  @param  ControllerHandle[in]  The handle of a controller that the driver
                                specified by This is managing.  This handle
                                specifies the controller whose name is to be
                                returned.

  @param  ChildHandle[in]       The handle of the child controller to retrieve
                                the name of.  This is an optional parameter that
                                may be NULL.  It will be NULL for device
                                drivers.  It will also be NULL for a bus drivers
                                that wish to retrieve the name of the bus
                                controller.  It will not be NULL for a bus
                                driver that wishes to retrieve the name of a
                                child controller.

  @param  Language[in]          A pointer to a Null-terminated ASCII string
                                array indicating the language.  This is the
                                language of the driver name that the caller is
                                requesting, and it must match one of the
                                languages specified in SupportedLanguages. The
                                number of languages supported by a driver is up
                                to the driver writer. Language is specified in
                                RFC 4646 or ISO 639-2 language code format.

  @param  ControllerName[out]   A pointer to the Unicode string to return.
                                This Unicode string is the name of the
                                controller specified by ControllerHandle and
                                ChildHandle in the language specified by
                                Language from the point of view of the driver
                                specified by This.

  @retval EFI_SUCCESS           The Unicode string for the user readable name in
                                the language specified by Language for the
                                driver specified by This was returned in
                                DriverName.

  @retval EFI_INVALID_PARAMETER ControllerHandle is NULL.

  @retval EFI_INVALID_PARAMETER ChildHandle is not NULL and it is not a valid
                                EFI_HANDLE.

  @retval EFI_INVALID_PARAMETER Language is NULL.

  @retval EFI_INVALID_PARAMETER ControllerName is NULL.

  @retval EFI_UNSUPPORTED       The driver specified by This is not currently
                                managing the controller specified by
                                ControllerHandle and ChildHandle.

  @retval EFI_UNSUPPORTED       The driver specified by This does not support
                                the language specified by Language.

**/
EFI_STATUS
EFIAPI
EfiDriverComponentNameGetControllerName(
  EFI_COMPONENT_NAME_PROTOCOL*    This,
  EFI_HANDLE                      ControllerHandle,
  EFI_HANDLE                      ChildHandle,
  CHAR8*                          Language,
  CHAR16**                        ControllerName
  );

/**
  Check if EFI_EXT_SCSI_PASS_THRU_PROTOCOL or EFI_SCSI_PASS_THRU_PROTOCOL exist on the parent handle.

  @param  ChildHandle  Child Handle to retrieve Parent information.

  @retval  TRUE    Should produce Block Io & Block Io2.
  @retval  FALSE   Should not produce Block Io & Block Io2.

**/
BOOLEAN
IsUfsDevice (
  IN  EFI_HANDLE      ChildHandle
  );

#endif //_UFS_ERASE_H_
