/** @file
  ME config block for PEI phase

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
#ifndef _ME_PEI_CONFIG_H_
#define _ME_PEI_CONFIG_H_

#include <Uefi.h>
#include <ConfigBlock.h>

#define ME_PEI_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gMePeiPreMemConfigGuid;

#pragma pack (push,1)

/**
  ME Pei Pre-Memory Configuration Structure.

  <b>Revision 1:</b>
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                 ///< Config Block Header
  UINT32 HeciTimeouts                     : 1;  ///< 0: Disable; <b>1: Enable</b> - HECI Send/Receive Timeouts.
  /**
    <b>(Test)</b>
    <b>0: Disabled</b>
       1: ME DID init stat 0 - Success
       2: ME DID init stat 1 - No Memory in Channels
       3: ME DID init stat 2 - Memory Init Error
  **/
  UINT32 DidInitStat                      : 2;
  /**
    <b>(Test)</b>
    <b>0: Set to 0 to enable polling for CPU replacement</b>
       1: Set to 1 will disable polling for CPU replacement
  **/
  UINT32 DisableCpuReplacedPolling        : 1;
  /**
    <b>(Test)</b>
    <b>0: ME BIOS will check each messages before sending</b>
       1: ME BIOS always sends messages without checking
  **/
  UINT32 DisableMessageCheck              : 1;
  /**
    <b>(Test)</b>
    The SkipMbpHob policy determines whether ME BIOS Payload data will be requested during boot.
    If set to 1, BIOS will not send Get MBP message and not create MBP Hob.
    <b>0: ME BIOS will send Get MBP message and create HOB for MBP data</b>
       1: ME BIOS will not send Get MBP message
  **/
  UINT32 SkipMbpHob                       : 1;
  UINT32 HeciCommunication                : 2;  ///< <b>(Test)</b> <b>0: POR</b>; 1: Enable; 2: Disable - Enable/Disable HECI1.
  UINT32 HeciCommunication2               : 2;  ///< <b>(Test)</b> <b>0: POR</b>; 1: Enable; 2: Disable - Enable/Disable HECI2.
  UINT32 HeciCommunication3               : 2;  ///< <b>(Test)</b> <b>0: POR</b>; 1: Enable; 2: Disable - Enable/Disable HECI3.
  UINT32 KtDeviceEnable                   : 2;  ///< <b>(Test)</b> <b>0: POR</b>; 1: Enable; 2: Disable - Enable/Disable KT Device.
  UINT32 SkipCpuReplacementCheck          : 1;  ///< <b>(Test)</b> <b>0: Disable</b>; 1: Enable - Enable/Disable to skip CPU replacement check.
  UINT32 RsvdBits                         :17;  ///< Reserved for future use & Config block alignment
} ME_PEI_PREMEM_CONFIG;
#pragma pack (pop)


#define ME_PEI_CONFIG_REVISION 1
extern EFI_GUID gMePeiConfigGuid;

#pragma pack (push,1)

/**
  ME Pei Post-Memory Configuration Structure.

  <b>Revision 1:</b>
  - Initial version.
**/

typedef struct {
  CONFIG_BLOCK_HEADER   Header;                 ///< Config Block Header

  UINT32 EndOfPostMessage                 : 2;  ///< 0: Disabled; 1: Send in PEI; <b>2: Send in DXE</b> - Send EOP at specific phase.
  UINT32 DisableD0I3SettingForHeci        : 1;  ///< <b>(Test)</b> <b>0: Disable</b>; 1: Enable - Enable/Disable D0i3 for HECI.
  /**
    Enable/Disable Me Unconfig On Rtc Clear. If enabled, BIOS will send MeUnconfigOnRtcClearDisable Msg with parameter 0.
    It will cause ME to unconfig if RTC is cleared.
    -    0: Disable
    - <b>1: Enable</b>
    -    2: Cmos is clear, status unkonwn
    -    3: Reserved
  **/
  UINT32 MeUnconfigOnRtcClear             : 2;
  UINT32 MctpBroadcastCycle               : 1;   ///< <b>(Test)</b> <b>0: Disable</b>; 1: Enable - Program registers for MCTP Cycle.
  /**
    Enable/Disable CSME Data Resilience BIOS support.
    It will force loading ME data default.
    -    0: Disable
    - <b>1: Enable</b> BIOS will send Data Clear HECI command in PEI if MFS failure is detected.
    -    2: Enable and defer - BIOS will defer Data Clear HECI command to DXE if MFS failure is detected.
  **/
  UINT32 CseDataResilience                : 2;                                                 ///< but defer the data clear command to later boot phase
  UINT32 SseCommunication                 : 2;   ///< (Test) 0: POR; 1: Enable; 2: Disable - Enable/Disable SSE/SSE++ Devices.
  UINT32 PseEomFlowEnable                 : 1;   ///< <b>0: Disable</b>; 1: Enable - PSE EOM flow enable
  UINT32 RsvdBits                         : 21;  ///< Reserved for future use & Config block alignment
} ME_PEI_CONFIG;

#pragma pack (pop)

#endif // _ME_PEI_CONFIG_H_
