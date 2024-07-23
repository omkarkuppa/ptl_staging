/** @file
  Gigabit Ethernet policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _GBE_CONFIG_H_
#define _GBE_CONFIG_H_

#define GBE_CONFIG_REVISION 1
extern EFI_GUID gGbeConfigGuid;

#pragma pack (push,1)

/**
  PCH intergrated GBE controller configuration settings.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Config Block Header
  /**
    Determines if enable PCH internal GBE, 0: Disable; <b>1: Enable</b>.
    When Enable is changed (from disabled to enabled or from enabled to disabled),
    it needs to set LAN Disable regsiter, which might be locked by FDSWL register.
    So it's recommendated to issue a global reset when changing the status for PCH Internal LAN.
  **/
  UINT32  Enable                :  1;
  UINT32  LtrEnable             :  1;  ///< <b>0: Disable</b>; 1: Enable LTR capabilty of PCH internal LAN.
  UINT32  PchWOLFastSupport     :  1;  ///< Enables bit B_PCH_ACPI_GPE0_EN_127_96_PME_B0 during PchLanSxCallback in PchLanSxSmm
  UINT32  RsvdBits0             : 29;  ///< Reserved bits
} GBE_CONFIG;

#pragma pack (pop)

#endif // _GBE_CONFIG_H_
