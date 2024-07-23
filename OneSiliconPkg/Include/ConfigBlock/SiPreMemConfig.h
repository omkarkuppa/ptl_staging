/** @file
  Si Config Block PreMem

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _SI_PREMEM_CONFIG_H_
#define _SI_PREMEM_CONFIG_H_

#define SI_PREMEM_CONFIG_REVISION  1

extern EFI_GUID gSiPreMemConfigGuid;

typedef enum {
  DisableDebug          = 0x00,
  EnabledTraceActive    = 0x02,
  EnabledTraceReady     = 0x04,
  EnabledTracePowerOff  = 0x06,
  Manual                = 0x07
} PLATFORM_DEBUG_OPTION;

#pragma pack (push,1)
/**
  The Silicon PreMem Policy allows the platform code to publish a set of configuration
  information that the RC drivers will use to configure the silicon hardware.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Offset 0 - 27 Config Block Header
  /**
    Platform Debug Option
    As a main switch to enable platform debug capability and relevant settings.
    Manual: Do not use Platform Debug Option to override other debug-relevant policies, but the user must set each debug option manually, aimed at advanced users.

    PlatformDebugOption related policies please refer to PLATFORM_DEBUG_OPTION
    <b>0:Disabled</b>; 2:Enabled TraceHub active; 4:Enabled TraceHub ready; 6:Enabled TraceHub power off; 7:Manual
  **/
  UINT32    PlatformDebugOption   :  4;       /// offset 28 - 31
  UINT32    RsvdBits              : 28;
  /**
    This is used to skip override boot mode during firmware update boot mode.
    When set to TRUE and boot mode is BOOT_ON_FLASH_UPDATE,
    skip setting boot mode to BOOT_WITH_FULL_CONFIGURATION in PEI memory init.
    <b>0: FALSE</b>, 1: TRUE
  **/
  UINT8     SkipOverrideBootModeWhenFwUpdate; /// offset 32
  UINT8     RsvdBytes[3];                     /// offset 33 - 35
} SI_PREMEM_CONFIG;

#pragma pack (pop)
#endif // _SI_PREMEM_CONFIG_H_
