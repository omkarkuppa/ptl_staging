/** @file
  Header file for SATA SoC library.

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
#ifndef _SATA_SOC_LIB_H_
#define _SATA_SOC_LIB_H_

#include <SataCtrl.h>
#include <Base.h>
#include <Uefi/UefiBaseType.h>

#define SATA_1_CONTROLLER_INDEX             0
#define SATA_2_CONTROLLER_INDEX             1
#define SATA_3_CONTROLLER_INDEX             2

/**
  Get SATA controller instance

  @param[in]  SataCtrlIndex       SATA controller index
  @param[out] SataController      Pointer to SATA controller structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - SataController NULL or SataCtrlIndex out of the range
**/
EFI_STATUS
SataGetController (
  IN  UINT32              SataCtrlIndex,
  OUT SATA_CONTROLLER     *SataController
  );

/**
  Get Maximum Sata Controller Number

  @retval Maximum Sata Controller Number
**/
UINT8
MaxSataControllerNum (
  VOID
  );

/**
  Get Maximum Sata Port Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     Maximum Sata Port Number
**/
UINT8
MaxSataPortNum (
  IN UINT32      SataCtrlIndex
  );

/**
  Check if SATA controller supports RST remapping

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     TRUE                Controller supports remapping
  @retval     FALSE               Controller does not support remapping
**/
BOOLEAN
IsRemappingSupportedOnSata (
  IN UINT32  SataCtrlIndex
  );

/**
  Checks if SoC supports the SATA PGD power down on given
  SATA controller.

  @param[in] SataCtrlIndex  SATA controller index

  @retval TRUE   SATA PGD power down supported
  @retval FALSE  SATA PGD power down not supported
**/
BOOLEAN
IsSataPowerGatingSupported (
  IN UINT32 SataCtrlIndex
  );

#endif // _SATA_SOC_LIB_H_
