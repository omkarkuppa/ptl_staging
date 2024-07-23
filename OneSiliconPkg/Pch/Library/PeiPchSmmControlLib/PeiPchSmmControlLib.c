/** @file
  The PEI Library Implements SmmControl Support.

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

@par Specification
**/
#include "PchSmmControlLib.h"
#include <Register/EspiRegs.h>
#include <Register/PmcRegs.h>

STATIC PEI_SMM_CONTROL_PPI    mSmmControlPpi = {
  PeiActivate,
  PeiDeactivate
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMmControlPpiGuid,
  &mSmmControlPpi
};

/**
  This function install PEI SMM Control PPI

**/
EFI_STATUS
EFIAPI
PchSmmControlInit (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = PeiServicesInstallPpi (&mPpiList);
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  Trigger the software SMI

  @param[in] Command              The value to be set on the software SMI command port
  @param[in] Data                 The value to be set on the software SMI data port

  @retval EFI_SUCCESS             Function completes successfully
**/
EFI_STATUS
EFIAPI
SmmTrigger (
  IN UINT8   Command,
  IN UINT8   Data
  )
{
  UINT32  OutputData;
  UINT32  OutputPort;
  UINT16  PmBase;

  PmBase = PmcGetAcpiBase ();

  ///
  /// Enable the APMC SMI
  ///
  OutputPort  = PmBase + R_ACPI_IO_SMI_EN;
  OutputData  = IoRead32 ((UINTN) OutputPort);
  OutputData |= (B_ACPI_IO_SMI_EN_APMC_EN | B_ACPI_IO_SMI_EN_GBL_SMI_EN);
  DEBUG (
    (DEBUG_VERBOSE,
     "The SMI Control Port at address %x will be written to %x.\n",
     OutputPort,
     OutputData)
    );
  IoWrite32 (
    (UINTN) OutputPort,
    (UINT32) (OutputData)
    );

  OutputPort  = R_PCH_IO_APM_STS;
  OutputData  = Data;

  //
  // Write data to APM DATA PORT
  //
  IoWrite8 (
    (UINTN) OutputPort,
    (UINT8) (OutputData)
    );

  OutputPort  = R_PCH_IO_APM_CNT;
  OutputData  = Command;

  ///
  /// Generate the APMC SMI
  ///
  IoWrite8 (
    (UINTN) OutputPort,
    (UINT8) (OutputData)
    );

  return EFI_SUCCESS;
}

/**
  Clear the SMI status


  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_DEVICE_ERROR        Something error occurred
**/
EFI_STATUS
EFIAPI
SmmClear (
  VOID
  )
{
  UINT32      OutputData;
  UINT32      OutputPort;
  UINT16      PmBase;

  PmBase = PmcGetAcpiBase ();

  ///
  /// Clear the Power Button Override Status Bit, it gates EOS from being set.
  ///
  OutputPort  = PmBase + R_ACPI_IO_PM1_STS;
  OutputData  = B_ACPI_IO_PM1_STS_PRBTNOR;
  DEBUG (
    (DEBUG_VERBOSE,
     "The PM1 Status Port at address %x will be written to %x.\n",
     OutputPort,
     OutputData)
    );
  IoWrite16 (
    (UINTN) OutputPort,
    (UINT16) (OutputData)
    );

  ///
  /// Clear the APM SMI Status Bit
  ///
  OutputPort  = PmBase + R_ACPI_IO_SMI_STS;
  OutputData  = B_ACPI_IO_SMI_STS_APM_STS;
  DEBUG (
    (DEBUG_VERBOSE,
     "The SMI Status Port at address %x will be written to %x.\n",
     OutputPort,
     OutputData)
    );
  IoWrite32 (
    (UINTN) OutputPort,
    (UINT32) (OutputData)
    );

  ///
  /// Set the EOS Bit
  ///
  OutputPort  = PmBase + R_ACPI_IO_SMI_EN;
  OutputData  = IoRead32 ((UINTN) OutputPort);
  OutputData |= B_ACPI_IO_SMI_EN_EOS;
  DEBUG (
    (DEBUG_VERBOSE,
     "The SMI Control Port at address %x will be written to %x.\n",
     OutputPort,
     OutputData)
    );
  IoWrite32 (
    (UINTN) OutputPort,
    (UINT32) (OutputData)
    );

  ///
  /// If the EOS bit did not get set, then we've got a problem.
  ///
  DEBUG_CODE (
    OutputData = IoRead32 ((UINTN) OutputPort);
    if ((OutputData & B_ACPI_IO_SMI_EN_EOS) != B_ACPI_IO_SMI_EN_EOS) {
      DEBUG ((DEBUG_ERROR, "Bugger, EOS did not get set!\n"));
      return EFI_DEVICE_ERROR;
    }
    );

  return EFI_SUCCESS;
}

/**
  This routine generates an SMI

  @param[in] PeiServices                General purpose services available to every PEIM.
  @param[in] This                       The EFI SMM Control ppi instance
  @param[in, out] ArgumentBuffer        The buffer of argument
  @param[in, out] ArgumentBufferSize    The size of the argument buffer
  @param[in] Periodic                   Periodic or not
  @param[in] ActivationInterval         Interval of periodic SMI

  @retval EFI Status                    Describing the result of the operation
  @retval EFI_INVALID_PARAMETER         Some parameter value passed is not supported
**/
EFI_STATUS
EFIAPI
PeiActivate (
  IN      EFI_PEI_SERVICES           **PeiServices,
  IN       PEI_SMM_CONTROL_PPI       *This,
  IN OUT  INT8                       *ArgumentBuffer OPTIONAL,
  IN OUT  UINTN                      *ArgumentBufferSize OPTIONAL,
  IN      BOOLEAN                    Periodic OPTIONAL,
  IN      UINTN                      ActivationInterval OPTIONAL
  )
{
  EFI_STATUS  Status;
  UINT8       Command;

  if (Periodic) {
    DEBUG ((DEBUG_WARN, "Invalid parameter\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (ArgumentBuffer == NULL) {
    Command = 0xFF;
  } else {
    if (ArgumentBufferSize == NULL || *ArgumentBufferSize != 1) {
      return EFI_INVALID_PARAMETER;
    }

    Command = *ArgumentBuffer;
  }
  ///
  /// Clear any pending the APM SMI
  ///
  Status = SmmClear ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return SmmTrigger (Command, 0x00);
}

/**
  This routine clears an SMI

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] This                 The EFI SMM Control ppi instance
  @param[in] Periodic             Periodic or not

  @retval EFI Status              Describing the result of the operation
  @retval EFI_INVALID_PARAMETER   Some parameter value passed is not supported
**/
EFI_STATUS
EFIAPI
PeiDeactivate (
  IN      EFI_PEI_SERVICES        **PeiServices,
  IN PEI_SMM_CONTROL_PPI          *This,
  IN  BOOLEAN                     Periodic OPTIONAL
  )
{
  if (Periodic) {
    return EFI_INVALID_PARAMETER;
  }

  return SmmClear ();
}

