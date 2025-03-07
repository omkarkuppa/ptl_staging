/** @file
  Interface definition details for platform hook support to ME module during DXE phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef _PLATFORM_ME_HOOK_H_
#define _PLATFORM_ME_HOOK_H_

///
/// ME Hook provided by platform for DXE phase
/// This protocol provides an interface to hook reference code by OEM.
///
/// Revision 1: Added PLATFORM_ME_HOOK_PRE_GLOBAL_RESET
/// Revision 2: Added PLATFORM_ME_HOOK_PRE_DATA_CLEAR and PLATFORM_ME_HOOK_POST_DATA_CLEAR
///

#define PLATFORM_ME_HOOK_PROTOCOL_REVISION  2

/**
  Platform hook before BIOS sends Global Reset Heci Message to ME

  @retval EFI Status Code

**/
typedef
EFI_STATUS
(EFIAPI *PLATFORM_ME_HOOK_PRE_GLOBAL_RESET) (
  VOID
  );

/**
  Platform hook before BIOS sends Data Clear Heci Message to ME

  @param[out]  ProceedDataClear  Pointer to consent sending Data Clear HECI message.

  @retval  EFI_SUCCESS  Succeed to execute the function.
           Others       Fail to execute the function.

**/
typedef
EFI_STATUS
(EFIAPI *PLATFORM_ME_HOOK_PRE_DATA_CLEAR) (
  OUT BOOLEAN  *ProceedDataClear
  );

/**
  Platform hook after BIOS sends Data Clear Heci Message to ME

  @param[in]  DataClearStatus  Indicate the status of sending Data Clear HECI message.

  @retval  EFI_SUCCESS  Succeed to execute the function.
           Others       Fail to execute the function.

**/
typedef
EFI_STATUS
(EFIAPI *PLATFORM_ME_HOOK_POST_DATA_CLEAR) (
  IN EFI_STATUS  DataClearStatus
  );

///
/// Platform ME Hook Protocol
/// This protocol provides an interface to hook reference code by OEM
///
typedef struct _PLATFORM_ME_HOOK_PROTOCOL {
  ///
  /// Function pointer for the hook called before BIOS sends Global Reset Heci Message to ME
  ///
  PLATFORM_ME_HOOK_PRE_GLOBAL_RESET  PreGlobalReset;
  ///
  /// Function pointers for the hook called before and after BIOS sends Data Clear Heci Message to ME
  ///
  PLATFORM_ME_HOOK_PRE_DATA_CLEAR    PreDataClear;
  PLATFORM_ME_HOOK_POST_DATA_CLEAR   PostDataClear;
} PLATFORM_ME_HOOK_PROTOCOL;

extern EFI_GUID gPlatformMeHookProtocolGuid;

#endif
