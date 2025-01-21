/** @file
  Touch Host Controller Quick Spi Absolute Pointer API

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

#include "QuickSpiPrivate.h"

/**
  AbsPtr protocol's WaitForInput (called when a consumer of AbsPtr protocol waits for new data)

  @param[in]  Event              Pointer to event that triggered this callback
  @param[in]  Context            Device context
**/
VOID
EFIAPI
AbsPtrWaitForInput (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

/**
  Reset touch panel
  Function required by AbsPtr protocol.

  @param[in]  This
  @param[in]  ExtendedVerification

  @retval EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
AbsPtrReset (
  IN EFI_ABSOLUTE_POINTER_PROTOCOL *This,
  IN BOOLEAN                       ExtendedVerification
  );

/**
  If not-yet-consumed touch data is available, GetState() returns it and marks it unavailable
  If such data is not available, error will be returned. User should wait until WaitForInput event gets signalled

  @param[in]      This
  @param[in/out]  State

  @retval EFI_SUCCESS     Data consumed
  @retval EFI_NOT_READY   Data not available
**/
EFI_STATUS
EFIAPI
AbsPtrGetState (
  IN      EFI_ABSOLUTE_POINTER_PROTOCOL  *This,
  IN OUT  EFI_ABSOLUTE_POINTER_STATE     *State
  );

/**
  Installs AbsolutePointerProtocol
  Assigns TouchPanel Guid for Virtual Keyboard support

  @param[in]  QuickSpiDev            Context of QuickSpi device

  @retval EFI_SUCCESS           Absolute Pointer Protocol Installed
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
QuickSpiInstallAbsPtr (
  IN QUICK_SPI_DEV                *QuickSpiDev
  );

/**
  Closes and uninstalls Absolute Pointer Protocol

  @param[in]  QuickSpiDev            Context of QuickSpi device

**/
VOID
EFIAPI
QuickSpiStopAbsPtr (
  IN QUICK_SPI_DEV                *QuickSpiDev
  );

