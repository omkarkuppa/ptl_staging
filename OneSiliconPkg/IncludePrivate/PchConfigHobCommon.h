/** @file
  The definition for common config HOBs.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _PCH_CONFIG_HOB_COMMON_H_
#define _PCH_CONFIG_HOB_COMMON_H_

#include <Uefi/UefiBaseType.h>

/**
  The LOCK_DOWN_CONFIG_HOB describes the expected configuration of the PCH
  for security requirement.
**/
typedef struct {
  UINT32  GlobalSmi      :  1;
  /**
    <b>(Test)</b> Enable BIOS Interface Lock Down bit to prevent writes to the Backup Control Register
    Top Swap bit and the General Control and Status Registers Boot BIOS Straps. 0: Disable; <b>1: Enable</b>.
  **/
  UINT32  BiosInterface  :  1;
  /**
    Enable the BIOS Lock Enable (BLE) feature and set EISS bit (D31:F5:RegDCh[5])
    for the BIOS region protection. When it is enabled, the BIOS Region can only be
    modified from SMM after EndOfDxe protocol is installed.
    Note: When BiosLock is enabled, platform code also needs to update to take care
    of BIOS modification (including SetVariable) in DXE or runtime phase after
    EndOfDxe protocol is installed.
    Enable InSMM.STS (EISS) in SPI
    If this EISS bit is set, then WPD must be a '1' and InSMM.STS must be '1' also
    in order to write to BIOS regions of SPI Flash. If this EISS bit is clear,
    then the InSMM.STS is a don't care.
    The BIOS must set the EISS bit while BIOS Guard support is enabled.
    In recovery path, platform can temporary disable EISS for SPI programming in
    PEI phase or early DXE phase.
    0: Disable; <b>1: Enable.</b>
  **/
  UINT32  BiosLock        :  1;
  UINT32  SpiEiss         :  1;
  /*
    Set true to enable CF9h Global Reset Lockdown.
    <b>0: Disable</b>, 1: Enable
  */
  UINT32  Cf9LockEnable   :  1;
  UINT32  RsvdBits        : 27;
} LOCK_DOWN_CONFIG_HOB;


/**
  This structure describles PCH general config HOB.
**/
typedef struct {
  /**
    This member describes whether or not the Compatibility Revision ID (CRID) feature
    of PCH should be enabled. <b>0: Disable</b>; 1: Enable
  **/
  UINT32 Crid       :  1;
  /**
    This shall describe USB Audio Offload enable capability
  **/
  UINT32 UaolEnable :  1;
  /**
    Disable/Enable Resets triggered by the system. <b>0: Platform POR</b>; 1: Force Enable; 2: Force Disable.
  **/
  UINT32 DisableResets  :  2;
  UINT32 RsvdBits0      : 28; ///< Reserved bits
} PCH_GENERAL_CONFIG_HOB;

#endif
