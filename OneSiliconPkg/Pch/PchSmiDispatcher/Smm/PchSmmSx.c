/** @file
  File to contain all the hardware specific stuff for the Smm Sx dispatch protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#include "PchSmmHelpers.h"
#include <Register/PmcRegs.h>
#include "PchSmiHelper.h"

extern BOOLEAN               mS3SusStart;
#define PROGRESS_CODE_S3_SUSPEND_END  PcdGet32 (PcdProgressCodeS3SuspendEnd)

GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mSxSourceDesc = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_EN}
      },
      S_ACPI_IO_SMI_EN,
      N_ACPI_IO_SMI_EN_SMI_ON_SLP_EN
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_SMI_STS}
      },
      S_ACPI_IO_SMI_STS,
      N_ACPI_IO_SMI_STS_SMI_ON_SLP_EN_STS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_SMI_ON_SLP_EN_STS
  }
};

/**
  Get the Sleep type

  @param[in] Record               No use
  @param[out] Context             The context that includes SLP_TYP bits to be filled

**/
VOID
EFIAPI
SxGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  )
{
  UINT32  Pm1Cnt;

  Pm1Cnt = IoRead32 ((UINTN) mAcpiBaseAddr + R_ACPI_IO_PM1_CNT);

  ///
  /// By design, the context phase will always be ENTRY
  ///
  Context->Sx.Phase = SxEntry;

  ///
  /// Map the PM1_CNT register's SLP_TYP bits to the context type
  ///
  switch (Pm1Cnt & B_ACPI_IO_PM1_CNT_SLP_TYP) {
    case V_ACPI_IO_PM1_CNT_S0:
      Context->Sx.Type = SxS0;
      break;

    case V_ACPI_IO_PM1_CNT_S1:
      Context->Sx.Type = SxS1;
      break;

    case V_ACPI_IO_PM1_CNT_S3:
      Context->Sx.Type = SxS3;
      break;

    case V_ACPI_IO_PM1_CNT_S4:
      Context->Sx.Type = SxS4;
      break;

    case V_ACPI_IO_PM1_CNT_S5:
      Context->Sx.Type = SxS5;
      break;

    default:
      ASSERT (FALSE);
      break;
  }
}

/**
  Check whether sleep type of two contexts match

  @param[in] Context1             Context 1 that includes sleep type 1
  @param[in] Context2             Context 2 that includes sleep type 2

  @retval FALSE                   Sleep types match
  @retval TRUE                    Sleep types don't match
**/
BOOLEAN
EFIAPI
SxCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  )
{
  return (BOOLEAN) (Context1->Sx.Type == Context2->Sx.Type);
}

/**
  When we get an SMI that indicates that we are transitioning to a sleep state,
  we need to actually transition to that state.  We do this by disabling the
  "SMI on sleep enable" feature, which generates an SMI when the operating system
  tries to put the system to sleep, and then physically putting the system to sleep.

**/
VOID
PchSmmSxGoToSleep (
  VOID
  )
{
  UINT32      Pm1Cnt;
  EFI_HANDLE  Handle = NULL;
  EFI_STATUS  Status;

  ClearPcieSci ();

  ///
  /// Disable SMIs
  ///
  PchSmmClearSource (&mSxSourceDesc);
  PchSmmDisableSource (&mSxSourceDesc);

  ///
  /// Get Power Management 1 Control Register Value
  ///
  Pm1Cnt = IoRead32 ((UINTN) mAcpiBaseAddr + R_ACPI_IO_PM1_CNT);

  ///
  /// Record S3 suspend performance data
  ///
  if ((Pm1Cnt & B_ACPI_IO_PM1_CNT_SLP_TYP) == V_ACPI_IO_PM1_CNT_S3) {
    ///
    /// Report status code before goto S3 sleep
    ///
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PROGRESS_CODE_S3_SUSPEND_END);
    mS3SusStart = FALSE;
    ///
    /// Write back cache into memory and invalidate cache before going to sleep.
    ///
    AsmWbinvd ();
  }

  Status = gSmst->SmmInstallProtocolInterface (
                  &Handle,
                  &gReadyToSleepProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gReadyToSleepProtocolGuid install failed\n"));
    ASSERT_EFI_ERROR (Status);
  }

  ///
  /// Now that SMIs are disabled, write to the SLP_EN bit again to trigger the sleep
  ///
  Pm1Cnt |= B_ACPI_IO_PM1_CNT_SLP_EN;

  IoWrite32 ((UINTN) mAcpiBaseAddr + R_ACPI_IO_PM1_CNT, Pm1Cnt);

  ///
  /// Should only proceed if wake event is generated.
  ///
  if ((Pm1Cnt & B_ACPI_IO_PM1_CNT_SLP_TYP) == V_ACPI_IO_PM1_CNT_S1) {
    while (((IoRead16 ((UINTN) mAcpiBaseAddr + R_ACPI_IO_PM1_STS)) & B_ACPI_IO_PM1_STS_WAK) == 0x0);
  } else {
    CpuDeadLoop ();
  }
  ///
  /// The system just went to sleep. If the sleep state was S1, then code execution will resume
  /// here when the system wakes up.
  ///
  Pm1Cnt = IoRead32 ((UINTN) mAcpiBaseAddr + R_ACPI_IO_PM1_CNT);

  if ((Pm1Cnt & B_ACPI_IO_PM1_CNT_SCI_EN) == 0) {
    ///
    /// An ACPI OS isn't present, clear the sleep information
    ///
    Pm1Cnt &= ~B_ACPI_IO_PM1_CNT_SLP_TYP;
    Pm1Cnt |= V_ACPI_IO_PM1_CNT_S0;

    IoWrite32 ((UINTN) mAcpiBaseAddr + R_ACPI_IO_PM1_CNT, Pm1Cnt);
  }

  PchSmmClearSource (&mSxSourceDesc);
  PchSmmEnableSource (&mSxSourceDesc);
}
