/** @file
  Mailbox Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include <Uefi.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>


/**
  Poll the run/busy bit of the mailbox until available or timeout expires.

  @retval EFI_STATUS
          - EFI_SUCCESS           Command succeeded.
          - EFI_TIMEOUT           Command timeout.
**/
EFI_STATUS
PollMailboxReady (
  VOID
  );

/**
  Check to see if the executing thread is BSP

  @retval TRUE   Executing thread is BSP
  @retval FALSE  Executing thread is AP
**/
BOOLEAN
MailboxLibIsBsp (
  VOID
  )
{
  MSR_IA32_APIC_BASE_REGISTER Msr;

  Msr.Uint64  = AsmReadMsr64 (MSR_IA32_APIC_BASE);
  return (BOOLEAN) (Msr.Bits.BSP == 1);
}

/**
  Generic Mailbox function for mailbox write commands. This function will
  poll the mailbox interface for control, issue the write request, poll
  for completion, and verify the write was succussful.

  @param[in]  MailboxCommand
  @param[in]  MailboxData
  @param[out] *MailboxStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
MailboxWrite (
  IN UINT32  MailboxCommand,
  IN UINT32  MailboxData,
  OUT UINT32 *MailboxStatus
  )
{
  EFI_STATUS         Status;
  UINT64             MchBar;
  PCODE_MAILBOX_FULL PcodeMailboxFull;

  //
  // Below assertion catches the invalid case:
  //   Issue PCODE or VR mailbox commands from AP.
  //
  ASSERT (MailboxLibIsBsp ());

  ///
  ///  Poll the run/busy to ensure the interface is available
  ///
  Status = PollMailboxReady ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = %Xh\n", MailboxCommand));
  DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Data    = %Xh\n", MailboxData));

  ///
  /// Copy in Mailbox data and write the PCODE mailbox DATA field
  ///
  PcodeMailboxFull.Interface.InterfaceData = MailboxCommand;
  PcodeMailboxFull.Data = MailboxData;
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  MmioWrite32 ((MchBar + PCODE_MAILBOX_DATA_OFFSET), PcodeMailboxFull.Data);

  ///
  /// Set the Run/Busy bit to signal mailbox data is ready to process
  ///
  PcodeMailboxFull.Interface.Fields.RunBusy = 1;
  MmioWrite32 ((MchBar + PCODE_MAILBOX_INTERFACE_OFFSET), PcodeMailboxFull.Interface.InterfaceData);

  ///
  /// Poll run/busy to indicate the completion of write request
  ///
  Status = PollMailboxReady ();
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ///
  /// Read the BIOS PCODE mailbox
  ///
  PcodeMailboxFull.Interface.InterfaceData = MmioRead32 (MchBar + PCODE_MAILBOX_INTERFACE_OFFSET);

  ///
  ///  Copy PCODE mailbox completion code
  ///
  *MailboxStatus = (UINT32) PcodeMailboxFull.Interface.Fields.Command;

  DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Status = %2Xh\n", *MailboxStatus));

  return Status;
}

/**
  Internal common Mailbox Read funtion

  @param[in]       MailboxCommand
  @param[in, out]  *MailboxDataPtr
  @param[out]      *MailboxStatus
  @param[in]       WithInput

  @retval EFI_STATUS
**/
EFI_STATUS
MailboxReadCommon (
  IN     UINT32  MailboxCommand,
  IN OUT UINT32  *MailboxDataPtr,
     OUT UINT32  *MailboxStatus,
  IN     BOOLEAN WithInput
  )
{
  EFI_STATUS         Status;
  UINT64             MchBar;
  PCODE_MAILBOX_FULL PcodeMailboxFull;

  //
  // Below assertion catches the invalid case:
  //   Issue PCODE or VR mailbox commands from AP.
  //
  ASSERT (MailboxLibIsBsp ());

  ///
  ///  Poll the run/busy to ensure the interface is available
  ///
  Status = PollMailboxReady ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (WithInput) {
    DEBUG ((DEBUG_INFO, "(MAILBOX) MailboxReadWithInput Command = %Xh\n", MailboxCommand));
  } else {
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Read Command = %Xh\n", MailboxCommand));
  }

  ///
  /// Write the PCODE mailbox read request.
  /// Read requests only require a write to the PCODE interface mailbox.
  /// The read results will be updated in the data mailbox.
  ///
  PcodeMailboxFull.Interface.InterfaceData = MailboxCommand;
  PcodeMailboxFull.Interface.Fields.RunBusy = 1;
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  if (WithInput) {
    //
    // Only write data if it is called by MailboxReadWithInput
    //
    DEBUG ((DEBUG_INFO, "(MAILBOX) MailboxReadWithInput input data = %Xh\n", *MailboxDataPtr));
    PcodeMailboxFull.Data = *MailboxDataPtr;
    MmioWrite32 ((MchBar + PCODE_MAILBOX_DATA_OFFSET), PcodeMailboxFull.Data);
  }

  MmioWrite32 ((MchBar + PCODE_MAILBOX_INTERFACE_OFFSET), PcodeMailboxFull.Interface.InterfaceData);

  ///
  /// Poll run/busy to indicate the completion of read request
  ///
  Status = PollMailboxReady ();
  if (EFI_ERROR(Status)) {
    return Status;
  }

  ///
  /// Read the BIOS PCODE mailbox
  ///
  PcodeMailboxFull.Interface.InterfaceData = MmioRead32 (MchBar + PCODE_MAILBOX_INTERFACE_OFFSET);
  PcodeMailboxFull.Data = MmioRead32 (MchBar + PCODE_MAILBOX_DATA_OFFSET);

  ///
  ///  Copy PCODE mailbox completion code and read results
  ///
  *MailboxStatus = (UINT32) PcodeMailboxFull.Interface.Fields.Command;
  CopyMem (MailboxDataPtr, &PcodeMailboxFull.Data, sizeof (UINT32));
  DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Status = %2Xh\n", *MailboxStatus));

  return Status;
}

/**
  Generic Mailbox function for mailbox read commands. This function will write
  the read request, and populate the read results in the output data.

  @param[in]  MailboxCommand
  @param[out] *MailboxDataPtr
  @param[out] *MailboxStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
MailboxRead (
  IN  UINT32  MailboxCommand,
  OUT UINT32 *MailboxDataPtr,
  OUT UINT32 *MailboxStatus
  )
{
  return MailboxReadCommon(MailboxCommand, MailboxDataPtr, MailboxStatus, FALSE);
}

/**
  Generic Mailbox function for mailbox read commands. This function will write
  the read request, and populate the read results in the output data.
  Different with MailboxRead, the param MailboxDataPtr is IN&OUT type, which will
  be writen into mailbox at first, then be replaced by the read result.

  @param[in]       MailboxCommand
  @param[in, out]  *MailboxDataPtr
  @param[out]      *MailboxStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
MailboxReadWithInput (
  IN     UINT32  MailboxCommand,
  IN OUT UINT32  *MailboxDataPtr,
     OUT UINT32  *MailboxStatus
  )
{
  return   MailboxReadCommon(MailboxCommand, MailboxDataPtr, MailboxStatus, TRUE);
}

/**
  Poll the run/busy bit of the mailbox until available or timeout expires.

  @retval EFI_STATUS
          - EFI_SUCCESS           Command succeeded.
          - EFI_TIMEOUT           Command timeout.
**/
EFI_STATUS
PollMailboxReady (
  VOID
  )
{
  EFI_STATUS              Status;
  UINT16                  StallCount;
  UINT8                   RunBusyBit;
  UINT64                  MchBar;
  PCODE_MAILBOX_INTERFACE PcodeMailboxInterface;

  Status = EFI_SUCCESS;
  StallCount = 0;
  RunBusyBit = 1;

  do {
    ///
    /// Read the MMIO run/busy state
    ///
    MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
    PcodeMailboxInterface.InterfaceData = MmioRead32 (MchBar + PCODE_MAILBOX_INTERFACE_OFFSET);
    RunBusyBit = (UINT8) PcodeMailboxInterface.Fields.RunBusy;
    //
    // Wait for 1us
    //
    MicroSecondDelay (MAILBOX_WAIT_STALL);
    StallCount++;
  }
  while ((RunBusyBit == 1) && (StallCount < MAILBOX_WAIT_TIMEOUT));

  if ((RunBusyBit == 1) && (StallCount == MAILBOX_WAIT_TIMEOUT)) {
    DEBUG ((DEBUG_WARN, "(MAILBOX) Mailbox interface timed out.\n"));
    Status = EFI_TIMEOUT;
  }
  return Status;
}
