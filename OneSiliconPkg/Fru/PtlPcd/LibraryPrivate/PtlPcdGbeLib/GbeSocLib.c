/** @file
  GBE SoC library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcdGbeSocLib.h>
#include <Register/PchRegs.h>

/**
  Get GBE controller instance for PCH

  @param[out]  GbeController          Pointer to GBE controller identification structure

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - GbeController is NULL
**/
EFI_STATUS
GbeGetController (
  OUT GBE_CONTROLLER         *GbeController
  )
{
  if (GbeController == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GbeController->Segment           = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
  GbeController->Bus               = DEFAULT_PCI_BUS_NUMBER_PCH;
  GbeController->Device            = GbeDevNumber ();
  GbeController->Function          = GbeFuncNumber ();
  GbeController->PciCfgBaseAddr    = GbePciCfgBase ();

  return EFI_SUCCESS;
}
