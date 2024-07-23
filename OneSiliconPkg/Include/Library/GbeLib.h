/** @file
  Header file for GbeLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _GBE_LIB_H_
#define _GBE_LIB_H_

/**
  Check whether GbE region is valid
  Check SPI region directly since GbE might be disabled in SW.

  @retval TRUE                    Gbe Region is valid
  @retval FALSE                   Gbe Region is invalid
**/
BOOLEAN
IsGbeRegionValid (
  VOID
  );

/**
  Check whether GBE controller is enabled in the platform.

  @retval TRUE                    GbE is enabled
  @retval FALSE                   GbE is disabled
**/
BOOLEAN
IsGbePresent (
  IN UINTN  PmcPwrmBase
  );

/**
  Checks if Gbe is Enabled or Disabled

  @param [in]  GbePciCfgBase    GbE PCI Config Space Address

  @retval  BOOLEAN    TRUE if device is enabled, FALSE otherwise.
**/
BOOLEAN
IsGbeEnabled (
  IN UINT64 GbePciCfgBase
  );

/**
  Returns Gigabit Ethernet PCI Device Number

  @retval  GbE device number
**/
UINT8
GbeDevNumber (
  VOID
  );

/**
  Returns Gigabit Ethernet PCI Function Number

  @retval  GbE function number
**/
UINT8
GbeFuncNumber (
  VOID
  );

/**
  Gets Phy Revision and Model Number

  @param [in]  GbePciCfgBase    GbE PCI Config Space Address
  @param [out] LanPhyRevision   Return Value

  @return EFI_SUCCESS           Phy Revision was obtained
          EFI_NOT_FOUND         Gbe is disabled or not present
          EFI_DEVICE_ERROR      Gbe class code check failed
          EFI_INVALID_PARAMETER When GbeBar is incorrect
                                When Phy register or address is out of bounds
**/
EFI_STATUS
GbeGetLanPhyRevision (
  IN   UINT64  GbePciCfgBase,
  OUT  UINT16  *LanPhyRevision
  );

#endif // _GBE_LIB_H_
