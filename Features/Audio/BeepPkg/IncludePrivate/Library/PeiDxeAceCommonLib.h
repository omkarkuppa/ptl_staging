/** @file
  Header file for Ace Common Library

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

@par Specification Reference:
**/

#ifndef _PEI_DXE_SNDW_COMMON_LIB_H_
#define _PEI_DXE_SNDW_COMMON_LIB_H_

#include <IndustryStandard/Pci22.h>

/**
  Check if passed instance of PciIo is HDA device.

  @param[in] EFI_PCI_IO_PROTOCOL      Instance of PciIo of a PCI device

  @retval TRUE                        HDA PCI Device supported
  @retval FALSE                       HDA PCI Device not suppoorted
**/
BOOLEAN
IsHdaDeviceSupported (
  IN EFI_PCI_IO_PROTOCOL              *PciIo
  );

/**
  Polling the Status bit.

  @param[in] StatusReg            The register address to read the status
  @param[in] PollingBitMap        The bit mapping for polling
  @param[in] PollingData          The Data for polling

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Polling the bit map time out
**/
EFI_STATUS
StatusPolling (
  IN UINTN     StatusReg,
  IN UINT32    PollingBitMap,
  IN UINT32    PollingData
  );

/**
  Polling the Status bit.
  Maximum polling time (us) equals HDA_MAX_LOOP_TIME * HDA_WAIT_PERIOD.

  @param[in] StatusReg            The register address to read the status
  @param[in] PollingBitMap        The bit mapping for polling
  @param[in] PollingData          The Data for polling

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Polling the bit map time out
**/
EFI_STATUS
StatusPolling16 (
  IN UINTN     StatusReg,
  IN UINT16    PollingBitMap,
  IN UINT16    PollingData
  );

/**
  Checks HD Audio Controller reset state.

  @param[in] HdaBar             Memory Space Base Address

  @return BOOLEAN               TRUE if controller is in reset, FALSE otherwise
**/
BOOLEAN
IsHdaControllerReset (
  IN  UINTN    HdaBar
  );

/**
  Function sets HDA BAR (if not set) and enables MMIO access

  @param[in, out] HdaBar            Pointer to HDA base address
  @param[out]     IsTemporaryBar    Pointer to variable that keeps the information whether temporary BAR was set

  @retval EFI_SUCCESS               MMIO access enabled successfully
  @retval EFI_UNSUPPORTED           HDA device is not available
**/
EFI_STATUS
EnableHdaMmioAccess (
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN OUT UINTN                *HdaBar,
  OUT    BOOLEAN              *IsTemporaryBar
  );

/**
  Sets HD Audio Controller Reset state.
  In reset all state machines, memory mapped registers and links are driven to reset values.

  @param[in] HdaHandle              Pointer to Hda controller configuration structure
  @param[in] AssertResetSignal      TRUE to enter (assert) reset, FALSE to exit (de-assert) reset

  @retval EFI_SUCCESS               Controller changed reset state successfully
  @retval EFI_TIMEOUT               Polling RESET# bit timed out
**/
EFI_STATUS
AceSetControllerReset (
  IN  UINTN    HdaBar,
  IN  BOOLEAN  AssertResetSignal
  );

/**
  Disables MMIO access and clears BARs of HD-Audio device

  @param[out] IsTemporaryBar        Variable that keeps the information whether temporary BAR was set

  @retval VOID
**/
VOID
DisableHdaDspMmioAccess (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN BOOLEAN              IsTemporaryBar
  );

#endif
