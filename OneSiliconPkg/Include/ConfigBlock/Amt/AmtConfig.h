/** @file
  AMT Config Block for PEI/DXE phase

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
#ifndef _AMT_CONFIG_H_
#define _AMT_CONFIG_H_

#include <Library/ConfigBlockLib.h>
#include <ConfigBlock.h>

#define AMT_PEI_CONFIG_REVISION 1
extern EFI_GUID gAmtPeiConfigGuid;

#define AMT_DXE_CONFIG_REVISION 1
extern EFI_GUID gAmtDxeConfigGuid;

#pragma pack (1)

/**
  AMT Pei Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                 ///< Config Block Header
  /**
    Enable or Disable Intel Active Management Technology feature.
    If disabled, all Intel AMT features, including Alert Standard Format features, will not be supported.
       0: Disable
    <b>1: Enable.</b>
  **/
  UINT32  AmtEnabled                      : 1;
  /**
    ME WatchDog timer feature. If disabled, below WatchDogTimerOs/WatchDogTimerBios will be irrelevant.
    See WatchDogTimerOs and WatchDogTimerBios description.
    <b>0: Disable</b>
       1: Enable ME WDT if corresponding timer value is not zero.
  **/
  UINT32  WatchDogEnabled                 : 1;
  UINT32  AmtSolEnabled                   : 1;  ///< Serial Over Lan retrieved from Mebx. The default value depends on CSME/AMT. 0: Disable, <b>1: Enable</b>
  UINT32  RsvdBits                        : 29; ///< Reserved for future use & Config block alignment
  /**
    OS WatchDog Timer
    <b>0: Disable</b> OS WDT won't be started after stopping BIOS WDT even if WatchDogEnabled is 1.
    Non zero value - OS WDT will be started after stopping BIOS WDT if WatchDogEnabled is 1. The timer is set
    according to the value.
  **/
  UINT16  WatchDogTimerOs;
  /**
    BIOS WatchDog Timer
    <b>0: Disable</b> BIOS WDT won't be started even if WatchDogEnabled is 1.
    Non zero value - The BIOS WDT is set according to the value and started if WatchDogEnabled is 1.
  **/
  UINT16  WatchDogTimerBios;
} AMT_PEI_CONFIG;


/**
  AMT Dxe Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                 ///< Config Block Header
  UINT32  CiraRequest                     : 1;  ///< Trigger CIRA boot. <b>0: No CIRA request</b>; 1: Trigger CIRA request
  UINT32  UnConfigureMe                   : 1;  ///< OEMFlag Bit 15: Unconfigure ME with resetting MEBx password to default. <b>0: No</b>; 1: Un-configure ME without password
  UINT32  UsbProvision                    : 1;  ///< Enable/Disable of AMT USB Provisioning. <b>0: Disable</b>; 1: Enable
  UINT32  RsvdBits                        : 29; ///< Reserved for future use & Config block alignment
} AMT_DXE_CONFIG;

#pragma pack ()

#endif // _AMT_CONFIG_H_
