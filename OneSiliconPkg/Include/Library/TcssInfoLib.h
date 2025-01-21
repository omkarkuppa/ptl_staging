/** @file
  Header file for SmmTcss Private Info Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _SMM_TCSS_INFO_LIB_H_
#define _SMM_TCSS_INFO_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>

/**
  GetTbtDmaBusNumber: Get TbtDma Bus Number

  @retval PCI bus number for TbtDma
**/
UINT64
GetTbtDmaBusNumber (
  VOID
  );

/**
  GetTbtDmaDevNumber: Get TbtDma Dev Number

  @retval PCI dev number for TbtDma
**/
UINT64
GetTbtDmaDevNumber (
  VOID
  );

/**
  GetTbtDma0FuncNumber: Get TbtDma0 Fun Number

  @retval PCI fun number for TbtDma0
**/
UINT64
GetTbtDma0FuncNumber (
  VOID
  );

/**
  GetTbtDma1FuncNumber: Get TbtDma1 Fun Number

  @retval PCI fun number for TbtDma1
**/
UINT64
GetTbtDma1FuncNumber (
  VOID
  );

/**
  GetTcssXhciBusNumber: Get CpuXhci Bus Number

  @retval PCI bus number for CpuXhci
**/
UINT64
GetTcssXhciBusNumber (
  VOID
  );

/**
  GetTcssXhciDevNumber: Get CpuXhci Dev Number

  @retval PCI dev number for CpuXhci
**/
UINT64
GetTcssXhciDevNumber (
  VOID
  );

/**
  GetTcssXhciFunNumber: Get CpuXhci Fun Number

  @retval PCI fun number for CpuXhci
**/
UINT64
GetTcssXhciFuncNumber (
  VOID
  );

/**
  GetITbtPcieDevNumber: Get ITbt PCIe Dev Number

  @retval ITbt PCIe dev number for ITbt PCIe
**/
UINT64
GetITbtPcieDevNumber (
  VOID
  );

/**
  Checks if IOM_TYPEC_SW_CONFIGURATION_1 is locked

  @retval TRUE   IOM_TYPEC_SW_CONFIGURATION_1 is locked
  @retval FALSE  IOM_TYPEC_SW_CONFIGURATION_1 is not locked
**/
BOOLEAN
IsTcssTypeCSwCfg1Locked (
  VOID
  );

/**
  Checks if IOM_TYPEC_SW_CONFIGURATION_3 is locked

  @retval TRUE   IOM_TYPEC_SW_CONFIGURATION_3 is locked
  @retval FALSE  IOM_TYPEC_SW_CONFIGURATION_3 is not locked
**/
BOOLEAN
IsTcssTypeCSwCfg3Locked (
  VOID
  );

/**
  Checks if IOM_TYPEC_SW_CONFIGURATION_4 is locked

  @retval TRUE   IOM_TYPEC_SW_CONFIGURATION_4 is locked
  @retval FALSE  IOM_TYPEC_SW_CONFIGURATION_4 is not locked
**/
BOOLEAN
IsTcssTypeCSwCfg4Locked (
  VOID
  );

/**
  Checks if IOM_PCR_IOM_USB4HR_MISC_CONFIG_LOCK is locked

  @retval TRUE   IOM_PCR_IOM_USB4HR_MISC_CONFIG_LOCK is locked
  @retval FALSE  IOM_PCR_IOM_USB4HR_MISC_CONFIG_LOCK is not locked
**/
BOOLEAN
IsTcssTypeCMiscCfgLocked (
  VOID
  );

/**
  GetTcssXhciBusNumber: Get TcssXhci Bus Number

  @retval PCI bus number for TcssXhci
**/
UINT64
GetTcssXhciBusNumber (
  VOID
  );

/**
  GetTcssXhciDevNumber: Get TcssXhci Dev Number

  @retval PCI dev number for TcssXhci
**/
UINT64
GetTcssXhciDevNumber (
  VOID
  );

/**
  GetTcssXhciFunNumber: Get TcssXhci Fun Number

  @retval PCI fun number for TcssXhci
**/
UINT64
GetTcssXhciFuncNumber (
  VOID
  );

#endif
