/** @file
  File to contain all the hardware specific stuff for the Smm Power Button dispatch protocol.

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
#include <PchSmmHelpers.h>
#include <Library/PmcPrivateLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC mPowerButtonSourceDesc = {
  PCH_SMM_SCI_EN_DEPENDENT,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_PM1_EN}
      },
      S_ACPI_IO_PM1_EN,
      N_ACPI_IO_PM1_EN_PWRBTN
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_ACPI_IO_PM1_STS}
      },
      S_ACPI_IO_PM1_STS,
      N_ACPI_IO_PM1_STS_PWRBTN
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_PM1_STS_REG
  }
};

/**
  Get the power button status.

  @param[in] Record               The pointer to the DATABASE_RECORD.
  @param[out] Context             Calling context from the hardware, will be updated with the current power button status.

**/
VOID
EFIAPI
PowerButtonGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  )
{
  if (PmcGetPwrBtnLevel ()) {
    Context->PowerButton.Phase = EfiPowerButtonExit;
  } else {
    Context->PowerButton.Phase = EfiPowerButtonEntry;
  }
}

/**
  Check whether Power Button status of two contexts match

  @param[in] Context1             Context 1 that includes Power Button status 1
  @param[in] Context2             Context 2 that includes Power Button status 2

  @retval FALSE                   Power Button status match
  @retval TRUE                    Power Button status don't match
**/
BOOLEAN
EFIAPI
PowerButtonCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  )
{
  return (BOOLEAN) (Context1->PowerButton.Phase == Context2->PowerButton.Phase);
}
