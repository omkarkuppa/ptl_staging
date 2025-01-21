/** @file
  Intel Platform Opal protocol

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

#ifndef _PLATFORM_OPAL_PROTOCOL_H_
#define _PLATFORM_OPAL_PROTOCOL_H_

typedef struct _PLATFORM_OPAL_PROTOCOL  PLATFORM_OPAL_PROTOCOL;

#define PLATFORM_OPAL_PROTOCOL_REVISION  1

/**
  Provide interface to perform Opal revert action on certain device

  @param[in] This        Pointer of PLATFORM_OPAL_PROTOCOL.
  @param[in] Handle      SSCP handle of the selected device.
  @param[in] Password    Password from host.
  @param[in] DeviceName  Device name.

  @retval EFI_SUCCESS  Perform opal revert successfully.
  @retval Others       Failed due to some error.

**/
typedef
EFI_STATUS
(EFIAPI *PERFORM_OPAL_REVERT) (
  IN PLATFORM_OPAL_PROTOCOL *This,
  IN EFI_HANDLE             Handle,
  IN CHAR8                  *Password OPTIONAL,
  IN CHAR16                 *DeviceName OPTIONAL
);

/**
  Provide interface to get the block sid state on certain device

  @param[in] This        Pointer of PLATFORM_OPAL_PROTOCOL.
  @param[in] Handle      SSCP handle of the selected device.
  @param[out] BlockState Blocked state.

  @retval EFI_SUCCESS  support block SID and return state.
  @retval Others       Failed due to some error.

**/
typedef
EFI_STATUS
(EFIAPI *GET_BLOCKSID_STATE) (
  IN PLATFORM_OPAL_PROTOCOL  *This,
  IN EFI_HANDLE              Handle,
  OUT UINT8                  *BlockState
  );

/**
  Provide interface to get the lock state on certain device

  @param[in]  This      Pointer of PLATFORM_OPAL_PROTOCOL.
  @param[in]  Handle    SSCP handle of the selected device.
  @param[out] IsEnabled TRUE/FALSE, TRUE: opal feature is enabled.
  @param[out] IsLocked  TRUE/FALSE, TRUE: device is locked.

  @retval EFI_SUCCESS  support Lock feature and return state.
  @retval Others       Failed due to some error.
**/
typedef
EFI_STATUS
(EFIAPI *GET_STATE) (
  IN  PLATFORM_OPAL_PROTOCOL *This,
  IN  EFI_HANDLE             Handle,
  OUT BOOLEAN                *IsEnabled OPTIONAL,
  OUT BOOLEAN                *IsLocked OPTIONAL
  );

///
/// The Platform Opal Protocol provides interface to perform
/// Opal revert action on certain device.
///
typedef struct _PLATFORM_OPAL_PROTOCOL {
  UINT8                    Revision;
  PERFORM_OPAL_REVERT      PerformOpalRevert;
  GET_BLOCKSID_STATE       GetBlockSidState;
  GET_STATE                GetState;
} PLATFORM_OPAL_PROTOCOL;

//
// Extern the GUID for protocol users.
//
extern EFI_GUID  gPlatformOpalProtocolGuid;

#endif
