/** @file
  TCSS info helper services.

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

#include <Library/TcssInfoLib.h>
#include <TcssDataHob.h>
#include <Library/HobLib.h>
#include <Register/HostDmaRegs.h>
#include <Register/CpuUsbRegs.h>
#include <Register/ItbtPcieRegs.h>
#include <PcdSbPortIds.h>
#include <Library/P2SbSocLib.h>
#include <Library/P2sbPrivateLib.h>

/**
  GetTbtDmaBusNumber: Get TbtDma Bus Number

  @retval PCI bus number for TbtDma
**/
UINT64
EFIAPI
GetTbtDmaBusNumber (
  VOID
  )
{
  return (UINT64)HOST_DMA_BUS_NUM;
}

/**
  GetTbtDmaDevNumber: Get TbtDma Dev Number

  @retval PCI dev number for TbtDma
**/
UINT64
EFIAPI
GetTbtDmaDevNumber (
  VOID
  )
{
  return (UINT64)HOST_DMA_DEV_NUM;
}

/**
  GetTbtDma0FunNumber: Get TbtDma0 Fun Number

  @retval PCI fun number for TbtDma0
**/
UINT64
EFIAPI
GetTbtDma0FuncNumber (
  VOID
  )
{
  return (UINT64)HOST_DMA0_FUN_NUM;
}

/**
  GetTbtDma1FunNumber: Get TbtDma1 Fun Number

  @retval PCI fun number for TbtDma1
**/
UINT64
EFIAPI
GetTbtDma1FuncNumber (
  VOID
  )
{
  return (UINT64)HOST_DMA1_FUN_NUM;
}

/**
  GetTcssXhciBusNumber: Get TcssXhci Bus Number

  @retval PCI bus number for TcssXhci
**/
UINT64
EFIAPI
GetTcssXhciBusNumber (
  VOID
  )
{
  return (UINT64)XHCI_NORTH_BUS_NUM;
}

/**
  GetTcssXhciDevNumber: Get TcssXhci Dev Number

  @retval PCI dev number for TcssXhci
**/
UINT64
EFIAPI
GetTcssXhciDevNumber (
  VOID
  )
{
  return (UINT64)XHCI_NORTH_DEV_NUM;
}

/**
  GetTcssXhciFunNumber: Get TcssXhci Fun Number

  @retval PCI fun number for TcssXhci
**/
UINT64
EFIAPI
GetTcssXhciFuncNumber (
  VOID
  )
{
  return (UINT64)XHCI_NORTH_FUNC_NUM;
}

/**
  GetITbtPcieDevNumber: Get ITbt Dev Number

  @retval ITbt dev number for TcssXhci
**/
UINT64
EFIAPI
GetITbtPcieDevNumber (
  VOID
  )
{
  return (UINT64)ITBT_PCIE_DEV_NUM;
}

/**
  Checks if IOM_TYPEC_SW_CONFIGURATION_1 is locked

  @retval TRUE   IOM_TYPEC_SW_CONFIGURATION_1 is locked
  @retval FALSE  IOM_TYPEC_SW_CONFIGURATION_1 is not locked
**/
BOOLEAN
EFIAPI
IsTcssTypeCSwCfg1Locked (
  VOID
  )
{
  TCSS_DATA_HOB *TcssHob;

  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    return FALSE;
  }
  return !!(TcssHob->TcssData.SwConfiguration1Lock);
}

/**
  Checks if IOM_TYPEC_SW_CONFIGURATION_3 is locked

  @retval TRUE   IOM_TYPEC_SW_CONFIGURATION_3 is locked
  @retval FALSE  IOM_TYPEC_SW_CONFIGURATION_3 is not locked
**/
BOOLEAN
EFIAPI
IsTcssTypeCSwCfg3Locked (
  VOID
  )
{
  TCSS_DATA_HOB *TcssHob;

  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    return FALSE;
  }
  return !!(TcssHob->TcssData.SwConfiguration3Lock);
}

/**
  Checks if IOM_TYPEC_SW_CONFIGURATION_4 is locked

  @retval TRUE   IOM_TYPEC_SW_CONFIGURATION_4 is locked
  @retval FALSE  IOM_TYPEC_SW_CONFIGURATION_4 is not locked
**/
BOOLEAN
EFIAPI
IsTcssTypeCSwCfg4Locked (
  VOID
  )
{
  TCSS_DATA_HOB *TcssHob;

  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    return FALSE;
  }
  return !!(TcssHob->TcssData.SwConfiguration4Lock);
}

/**
  Checks if IOM_PCR_IOM_USB4HR_MISC_CONFIG_LOCK is locked

  @retval TRUE   IOM_PCR_IOM_USB4HR_MISC_CONFIG_LOCK is locked
  @retval FALSE  IOM_PCR_IOM_USB4HR_MISC_CONFIG_LOCK is not locked
**/
BOOLEAN
EFIAPI
IsTcssTypeCMiscCfgLocked (
  VOID
  )
{
  TCSS_DATA_HOB *TcssHob;

  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    return FALSE;
  }
  return !!(TcssHob->TcssData.MiscConfigurationLock);
}

/**
  The function returns TCSS IOM PCR MMIO base

  @retval IOM PCR MMIO base
**/
UINT64
EFIAPI
TcssGetIomPcrMmioBase (
  VOID
  )
{
  P2SB_PORT_16_ID P2sbPid;
  P2SB_CONTROLLER P2sbController;
  UINTN           IomPcrBase;

  // WCL_SID_F3_PID_TC_IOM is same as PTL_SID_F3_PID_TC_IOM(0xF380)
  P2sbPid.Pid16bit = PTL_SID_F3_PID_TC_IOM;
  PtlPcdGetP2SbController (&P2sbController, P2sbPid);
  IomPcrBase = P2sbGetMmioBase (&P2sbController, P2sbPid.PortId.LocalPid);

  return IomPcrBase;
}