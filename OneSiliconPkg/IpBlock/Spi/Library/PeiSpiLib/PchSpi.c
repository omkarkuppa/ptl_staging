/** @file
  PCH SPI PEI Library implements the SPI Host Controller Compatibility Interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/SpiInitLib.h>
#include <Library/SpiSocLib.h>

/**
  Clear EISS bit(Enable InSMM.STS)

**/
VOID
SpiClearEissBit (
  )
{
  SPI_HANDLE  SpiHandle;
  EFI_STATUS  Status;

  Status = GetSpiHandle (&SpiHandle, NULL);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
  }
  SpiDisableEiss (&SpiHandle);
}
