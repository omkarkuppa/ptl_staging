/** @file
  PCH General policy

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
#ifndef _PCH_GENERAL_CONFIG_H_
#define _PCH_GENERAL_CONFIG_H_

#define PCH_GENERAL_CONFIG_REVISION 2
#define PCH_GENERAL_PREMEM_CONFIG_REVISION 4

extern EFI_GUID gPchGeneralConfigGuid;
extern EFI_GUID gPchGeneralPreMemConfigGuid;

#pragma pack (push,1)

enum PCH_RESERVED_PAGE_ROUTE {
  PchReservedPageToLpc,                   ///< Port 80h cycles are sent to LPC.
  PchReservedPageToPcie                   ///< Port 80h cycles are sent to PCIe.
};

/**
  PCH General Configuration
  <b>Revision 2</b>:  - Add VtdEnabled field.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    This member describes whether or not the Compatibility Revision ID (CRID) feature
    of PCH should be enabled. <b>0: Disable</b>; 1: Enable
  **/
  UINT32    Crid            :  1;
  /**
    Set to enable low latency of legacy IO.
    Some systems require lower IO latency irrespective of power.
    This is a tradeoff between power and IO latency.
    @note: Once this is enabled, DmiAspm, Pcie DmiAspm in SystemAgent
    and ITSS Clock Gating are forced to disabled.
    <b>0: Disable</b>, 1: Enable
  **/
  UINT32    LegacyIoLowLatency  :  1;
  /**
    This member describes whether or not the VTD feature
    should be enabled in PCH. <b>0: Disable</b>; 1: Enable
  **/
  UINT32    VtdEnabled          :  1;
  UINT32    RsvdBits0           : 29;       ///< Reserved bits
} PCH_GENERAL_CONFIG;

/**
  PCH General Pre-Memory Configuration
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Added GpioOverride.
  <b>Revision 3</b>:  - Added IoeDebugEn, PmodeClkEn
  <b>Revision 4</b>:  - Added PmcPrivacyConsent
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    Control where the Port 80h cycles are sent, <b>0: LPC</b>; 1: PCI.
  **/
  UINT32    Port80Route     :  1;
  /**
    Gpio override Level
    -- <b>0: Disable</b>;
    -  1: Override Level 1 - Skips GPIO configuration in PEI/FSPM/FSPT phase
    -  2: Override Level 2 - Reserved for future use
  **/
  UINT32    GpioOverride    :  3;
  /**
    Enable/Disable IOE Debug. When enabled, IOE D2D Dfx link and clock will keep up for debug
    <b>0: Disable</b>; 1: Enable
  **/
  UINT32    IoeDebugEn      :  1;
  /**
    Enable/Disable PMODE clock. When enabled, Pmode clock will toggle for XDP use
    <b>0: Disable</b>; 1: Enable
  **/
  UINT32    PmodeClkEn      :  1;
  /**
    Set/Clear PMC Privacy Consent. When enabled, PMC will collect unique ID in CrashLog
    <b>0: Disable</b>; 1: Enable
  **/
  UINT32    PmcPrivacyConsent : 1;
  /**
    Disable/Enable Resets triggered by the system. <b>0: Platform POR</b>; 1: Force Enable; 2: Force Disable.
  **/
  UINT32    DisableResets     :  2;
  UINT32    RsvdBits0         : 23;       ///< Reserved bits
} PCH_GENERAL_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _PCH_GENERAL_CONFIG_H_
