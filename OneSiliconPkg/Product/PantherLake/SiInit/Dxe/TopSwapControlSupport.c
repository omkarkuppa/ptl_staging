/** @file
  Install TopSwapControlSupport protocol to provide Set/Get TopSwap functions.

  It is caller's responsiblity to ensure the accessibility of TopSwap in DXE phase.
  This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <PiDxe.h>
#include <Protocol/TopSwapControlProtocol.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdRtcAccessLib.h>

/**
  Get TopSwap Status

  @param[in]  This   A pointer to the TOP_SWAP_CONTROL_PROTOCOL instance.

  @retval     TRUE   TopSwap is set
  @retval     FALSE  TopSwap is clear

  @note  It is caller's responsiblity to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)
**/
BOOLEAN
EFIAPI
GetTopSwapStatus (
  IN TOP_SWAP_CONTROL_PROTOCOL  *This
  );

/**
  Set TopSwap Status

  @param[in]  This              A pointer to the TOP_SWAP_CONTROL_PROTOCOL instance.

  @param[in]  TopSwapEnable     Enable TopSwap or Disable it.

  @retval     EFI_SUCCESS       TopSwap set successfully
  @retval     EFI_DEVICE_ERROR  Command error

  @note  It is caller's responsiblity to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)
**/
EFI_STATUS
EFIAPI
SetTopSwapStatus (
  IN TOP_SWAP_CONTROL_PROTOCOL  *This,
  IN BOOLEAN                    TopSwapEnable
  );

EFI_HANDLE                 mTopSwapControlHandle  = NULL;

TOP_SWAP_CONTROL_PROTOCOL  mTopSwapControlSupport = {
  GetTopSwapStatus,
  SetTopSwapStatus
};

/**
  Install TopSwapControlSupport protocol to provide Set/Get TopSwap functions.

  It is caller's responsiblity to ensure the accessibility of TopSwap in DXE phase.
  This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)

  @param ImageHandle             A handle for the image that is initializing this driver.
  @param SystemTable             A pointer to the EFI system table.

  @retval EFI_SUCCESS            Installed TopSwap control Protocol successfully.
  @retval others                 Failed to install protocol.

**/
EFI_STATUS
EFIAPI
TopSwapControlSupport (
  VOID
  )
{
  EFI_STATUS  Status;

  //
  // Install TopSwap Control Protocol on a new handle
  //
  Status = gBS->InstallProtocolInterface (
                  &mTopSwapControlHandle,
                  &gTopSwapControlProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mTopSwapControlSupport
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Get TopSwap Status

  @param[in]  This   A pointer to the TOP_SWAP_CONTROL_PROTOCOL instance.

  @retval     TRUE   TopSwap is set
  @retval     FALSE  TopSwap is clear

  @note  It is caller's responsiblity to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)
**/
BOOLEAN
EFIAPI
GetTopSwapStatus (
  IN TOP_SWAP_CONTROL_PROTOCOL  *This
  )
{
  return PtlPcdTopSwapStatus ();
}

/**
  Set TopSwap Status

  @param[in]  This              A pointer to the TOP_SWAP_CONTROL_PROTOCOL instance.

  @param[in]  TopSwapEnable     Enable TopSwap or Disable it.

  @retval     EFI_SUCCESS       TopSwap set successfully
  @retval     EFI_DEVICE_ERROR  Command error

  @note  It is caller's responsiblity to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)
**/
EFI_STATUS
EFIAPI
SetTopSwapStatus (
  IN TOP_SWAP_CONTROL_PROTOCOL  *This,
  IN BOOLEAN                    TopSwapEnable
  )
{
  return PtlPcdTopSwapSet (TopSwapEnable);
}
