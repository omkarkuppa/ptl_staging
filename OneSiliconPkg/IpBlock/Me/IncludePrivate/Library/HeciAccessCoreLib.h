/** @file
  HECI Access driver core operations for use in PEI, DXE and SMM.

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

@par Specification
**/

#ifndef _HECI_ACCESS_CORE_LIB_H_
#define _HECI_ACCESS_CORE_LIB_H_

#include <Protocol/HeciAccessProtocol.h>
#include <HeciDeviceConfig.h>

/**
  HECI Access data instance definition.

  This protocol defines operations for HECI (Host Embedded Controller Interface)
  hardware access.
**/
typedef struct {
  HECI_ACCESS                  HeciAccess;         ///< HeciAccess Protocol/Ppi
  VOID                         *HeciAccessRoot;    ///< HeciAccess Protocol root
  VOID                         *HeciAccessNext;    ///< HeciAccess next Protocol
  HECI_DEVICE_CONFIG           DeviceConfig;       ///< HECI device configuration (SBDF, temporary BAR, interrupt config)
} HECI_ACCESS_PRIVATE;

#pragma pack (push, 1)
/**
  HECI MBAR Type definition
 **/
typedef union {
  UINT64   QWord;
  struct {
    UINT32 DWordL;
    UINT32 DWordH;
  } Bits;
} HECI_MBAR;
#pragma pack (pop)

/**
  Returns HECI BAR.

  @param[in] This       HECI Access device context

  @retval BAR read from HECI PCI Config Space
**/
UINTN
GetHeciBar (
  IN HECI_ACCESS_PRIVATE    *This
  );

/**
  Function sets HECI Access device PPI functions

  @param[in,out]  NewHeciDevice           Pointer to HECI Access device to be set
**/
VOID
SetHeciAccessFunctions (
  IN OUT HECI_ACCESS_PRIVATE     *NewHeciDevice
  );

/**
  Function sets new HECI Access device based on configuration record

  @param[in,out]  NewHeciDevice           HECI Access device to be set pointer
  @param[in]      HeciDeviceInitInfo      HECI Configuration record

  @retval EFI_SUCCESS                     Device info was set correctly
  @retval EFI_ABORTED                     HECI device is disabled or wrong config was passed
**/
EFI_STATUS
HeciSetNewDevice (
  IN OUT   HECI_ACCESS_PRIVATE    *NewHeciDevice,
  IN CONST HECI_DEVICE_CONFIG     *HeciDeviceInitInfo
  );

/**
  Get next HECI Access

  @param[in] This     Pointer to HECI Access protocol instance from which the next instance should be returned.

  @retval Pointer to the next HeciAccess protocol instance
**/
HECI_ACCESS *
EFIAPI
GetNextHeciAccess (
  IN HECI_ACCESS    *This
  );

#endif // _HECI_ACCESS_CORE_LIB_H_
