/** @file
  Oc Mailbox Library.

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
#include <Uefi.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/OcMailboxLib.h>
#include <Library/TimerLib.h>

/**
  Poll the run/busy bit of the mailbox until available or timeout expires.

  @param[in]  IsBsp        TRUE indicates the code is running from BSP.

  @retval EFI_STATUS
          - EFI_SUCCESS           Command succeeded.
          - EFI_TIMEOUT           Command timeout.
**/
EFI_STATUS
OcPollMailboxReady (
  IN BOOLEAN  IsBsp
  );

/**
  Check to see if the executing thread is BSP

  @retval TRUE   Executing thread is BSP
  @retval FALSE  Executing thread is AP
**/
BOOLEAN
OcMailboxLibIsBsp (
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
OcMailboxWrite (
  IN UINT32  MailboxCommand,
  IN UINT32  MailboxData,
  OUT UINT32 *MailboxStatus
  )
{
  EFI_STATUS         Status;
  UINT64             MsrData;
  OC_MAILBOX_FULL    OcMailboxFull;
  BOOLEAN            IsBspInt;

  IsBspInt = OcMailboxLibIsBsp ();

  ///
  ///  Poll the run/busy to ensure the interface is available
  ///
  Status = OcPollMailboxReady (IsBspInt);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsBspInt) {
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Command = %Xh\n", MailboxCommand));
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Write Data    = %Xh\n", MailboxData));
  }

  ///
  /// Set the Run/Busy bit to signal mailbox data is ready to process
  ///
  OcMailboxFull.Interface.InterfaceData = MailboxCommand;
  OcMailboxFull.Data = MailboxData;
  OcMailboxFull.Interface.Fields.RunBusy = 1;
  CopyMem (&MsrData, &OcMailboxFull, sizeof (MsrData));

  ///
  /// Write mailbox command to OC mailbox
  ///
  AsmWriteMsr64 (MSR_OC_MAILBOX, MsrData);

  ///
  /// Poll run/busy to indicate the completion of write request
  ///
  Status = OcPollMailboxReady (IsBspInt);
  if (EFI_ERROR (Status)) {
    return Status;
  }


  ///
  /// Read the mailbox command from OC mailbox
  ///
  MsrData = AsmReadMsr64 (MSR_OC_MAILBOX);
  CopyMem (&OcMailboxFull, &MsrData, sizeof (OcMailboxFull));

  ///
  ///  Copy Overclocking mailbox completion code
  ///
  *MailboxStatus = OcMailboxFull.Interface.Fields.CommandCompletion;

  if (IsBspInt) {
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Status = %2Xh\n", *MailboxStatus));
  }

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
OcMailboxRead (
  IN UINT32  MailboxCommand,
  OUT UINT32 *MailboxDataPtr,
  OUT UINT32 *MailboxStatus
  )
{
  EFI_STATUS         Status;
  UINT64             MsrData;
  OC_MAILBOX_FULL    OcMailboxFull;
  BOOLEAN            IsBspInt;

  IsBspInt = OcMailboxLibIsBsp ();

  ///
  ///  Poll the run/busy to ensure the interface is available
  ///
  Status = OcPollMailboxReady (IsBspInt);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsBspInt) {
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Read Command = %Xh\n", MailboxCommand));
  }

  ///
  /// Set the Run/Busy bit to signal mailbox data is ready to process
  ///
  OcMailboxFull.Interface.InterfaceData = MailboxCommand;
  OcMailboxFull.Data = *MailboxDataPtr;
  OcMailboxFull.Interface.Fields.RunBusy = 1;
  CopyMem (&MsrData, &OcMailboxFull, sizeof (MsrData));

  ///
  /// Write mailbox command to OC mailbox
  ///
  AsmWriteMsr64 (MSR_OC_MAILBOX, MsrData);

  ///
  /// Poll run/busy to indicate the completion of write request
  ///
  OcPollMailboxReady (IsBspInt);

  ///
  /// Read the OC mailbox
  ///
  MsrData = AsmReadMsr64 (MSR_OC_MAILBOX);
  CopyMem (&OcMailboxFull, &MsrData, sizeof (OcMailboxFull));

  ///
  ///  Copy Overclocking mailbox completion code and read results
  ///
  *MailboxStatus = OcMailboxFull.Interface.Fields.CommandCompletion;
  CopyMem (MailboxDataPtr, &OcMailboxFull.Data, sizeof (UINT32));

  if (IsBspInt) {
    DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Status = %2Xh\n", *MailboxStatus));
  }

  return Status;
}

/**
  Poll the run/busy bit of the mailbox until available or timeout expires.

  @param[in]  IsBsp        TRUE indicates the code is running from BSP.

  @retval EFI_STATUS
          - EFI_SUCCESS           Command succeeded.
          - EFI_TIMEOUT           Command timeout.
**/
EFI_STATUS
OcPollMailboxReady (
  IN BOOLEAN  IsBsp
  )
{
  EFI_STATUS              Status;
  UINT16                  StallCount;
  UINT8                   RunBusyBit;
  UINT64                  MsrData;
  OC_MAILBOX_FULL         OcMailboxFull;

  Status = EFI_SUCCESS;
  StallCount = 0;
  RunBusyBit = 1;

  do {
    ///
    /// Read the OC mailbox run/busy state
    ///
    MsrData = AsmReadMsr64 (MSR_OC_MAILBOX);
    CopyMem (&OcMailboxFull, &MsrData, sizeof (OcMailboxFull));
    RunBusyBit = (UINT8) OcMailboxFull.Interface.Fields.RunBusy;

    //
    // Wait for 1us
    //
    MicroSecondDelay (MAILBOX_WAIT_STALL);
    StallCount++;
  }
  while ((RunBusyBit == 1) && (StallCount < MAILBOX_WAIT_TIMEOUT));

  if ((RunBusyBit == 1) && (StallCount == MAILBOX_WAIT_TIMEOUT)) {
    if (IsBsp) {
      DEBUG ((DEBUG_ERROR, "(MAILBOX) Mailbox interface timed out.\n"));
    }
    Status = EFI_TIMEOUT;
  }
  return Status;
}
