/** @file
  The driver is to create and publish the Connectivity UEFI variables.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <CnvUefiConfigVariables.h>

STATIC CONST UINT8  mSgomInitTable[338] = {
  /*  Horizontal Countries Letter
     AB    CD    EF    GH    IJ    KL    MN    OP    QR    ST    UV    WX    YZ     Vertical Countries Letter   */
  0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0x01, 0x20, // A
  0x12, 0x10, 0x02, 0x22, 0x00, 0x00, 0x01, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, // B
  0x20, 0x00, 0x00, 0x20, 0x02, 0x10, 0x00, 0x01, 0x10, 0x00, 0x00, 0x00, 0x22, // C
  0x00, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x20, // D
  0x00, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, // E
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, // F
  0x00, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, // G
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x20, 0x10, 0x02, 0x00, 0x00, // H
  0x00, 0x10, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, // I
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, // J
  0x00, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x02, 0x00, // K
  0x20, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x20, 0x22, 0x00, 0x00, // L
  0x02, 0x20, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x02, 0x12, 0x02, // M
  0x00, 0x00, 0x00, 0x02, 0x02, 0x20, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x20, // N
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // O
  0x01, 0x00, 0x01, 0x21, 0x00, 0x21, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x01, // P
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Q
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x00, // R
  0x02, 0x00, 0x00, 0x01, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // S
  0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x20, 0x10, 0x00, 0x01, 0x10, // T
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, // U
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // V
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // W
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // X
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Y
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00  // Z
};

STATIC CONST UINT8  mUatsInitTable[338] = {
  /* Vertical Countries Letter
    BA    DC    FE    HG    JI    LK    NM    PO    RQ    TS    VU    XW    ZY     Horizontal Countries Letter */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // A
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // B
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // C
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // D
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // E
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // F
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // G
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // H
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // I
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // J
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // K
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // L
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // M
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // N
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // O
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // P
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Q
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // R
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // S
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // T
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // U
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // V
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // W
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // X
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Y
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // Z
};
/**
  Install SPLC for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallSplc (
  )
{
  CNV_UEFI_VAR_SPLC  SplcVar;
  EFI_STATUS         Status;

  ZeroMem (&SplcVar, sizeof (CNV_UEFI_VAR_SPLC));
  SplcVar.Header.Revision   = CNV_UEFI_SPLC_REVISION_0;
  SplcVar.DefaultPowerLimit = 0xFFFFFFFF;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_SPLC),
             CNV_UEFI_SPLC_VAR_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&SplcVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install WRDD for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallWrdd (
  )
{
  CNV_UEFI_VAR_WRDD  WrddVar;
  EFI_STATUS         Status;

  ZeroMem (&WrddVar, sizeof (CNV_UEFI_VAR_WRDD));
  WrddVar.Header.Revision  = CNV_UEFI_WRDD_REVISION_0;
  WrddVar.RegulatoryDomain = 0x4150;  // "AP"

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_WRDD),
             CNV_UEFI_WRDD_VAR_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&WrddVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install WRDS for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallWrds (
  )
{
  CNV_UEFI_VAR_WRDS_WIFI_SAR  WrdsVar;
  EFI_STATUS                  Status;

  ZeroMem (&WrdsVar, sizeof (CNV_UEFI_VAR_WRDS_WIFI_SAR));
  WrdsVar.Header.Revision = CNV_UEFI_WRDS_REVISION_2;
  WrdsVar.Mode = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[0]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[1]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[2]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[3]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[4]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[5]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[6]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[7]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[8]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[9]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[10] = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[0]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[1]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[2]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[3]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[4]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[5]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[6]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[7]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[8]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[9]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[10] = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[0]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[1]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[2]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[3]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[4]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[5]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[6]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[7]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[8]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[9]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[10] = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[0]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[1]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[2]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[3]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[4]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[5]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[6]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[7]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[8]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[9]  = 0x00;
  WrdsVar.SarValues.SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[10] = 0x00;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_WRDS_WIFI_SAR),
             CNV_UEFI_WRDS_VAR_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&WrdsVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install EWRD for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallEwrd (
  )
{
  CNV_UEFI_VAR_EWRD  EwrdVar;
  EFI_STATUS         Status;

  ZeroMem (&EwrdVar, sizeof (CNV_UEFI_VAR_EWRD));
  EwrdVar.Header.Revision = CNV_UEFI_EWRD_REVISION_2;
  EwrdVar.WifiDynamicSarMode      = 0x00;
  EwrdVar.WifiDynamicSarRangeSets = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[0].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[1].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainATxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.ChainBTxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainATxPowerLimits[10] = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[0]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[1]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[2]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[3]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[4]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[5]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[6]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[7]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[8]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[9]  = 0x00;
  EwrdVar.WifiDynamicSarValuesExt[2].SAR_VALUES_UNION.SAR_VALUES_REV_2.CdbChainBTxPowerLimits[10] = 0x00;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_EWRD),
             CNV_UEFI_EWRD_VAR_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&EwrdVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install WGDS for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallWgds (
  )
{
  CNV_UEFI_VAR_WGDS  WgdsVar;
  EFI_STATUS         Status;

  ZeroMem (&WgdsVar, sizeof (CNV_UEFI_VAR_WGDS));
  WgdsVar.Header.Revision                                           = CNV_UEFI_WGDS_REVISION_3;
  WgdsVar.NumberOfGroups                                            = 3;
  WgdsVar.WifiSarPowerDelta[0].GeoSarOffset2400Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[0].GeoSarOffset5200Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[0].GeoSarOffset6000To7000Mhz.MaxAllowed = 0xFF;
  WgdsVar.WifiSarPowerDelta[0].GeoSarOffset2400Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[0].GeoSarOffset5200Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[0].GeoSarOffset6000To7000Mhz.ChainA     = 0x00;
  WgdsVar.WifiSarPowerDelta[0].GeoSarOffset2400Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[0].GeoSarOffset5200Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[0].GeoSarOffset6000To7000Mhz.ChainB     = 0x00;
  WgdsVar.WifiSarPowerDelta[1].GeoSarOffset2400Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[1].GeoSarOffset5200Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[1].GeoSarOffset6000To7000Mhz.MaxAllowed = 0xFF;
  WgdsVar.WifiSarPowerDelta[1].GeoSarOffset2400Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[1].GeoSarOffset5200Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[1].GeoSarOffset6000To7000Mhz.ChainA     = 0x00;
  WgdsVar.WifiSarPowerDelta[1].GeoSarOffset2400Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[1].GeoSarOffset5200Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[1].GeoSarOffset6000To7000Mhz.ChainB     = 0x00;
  WgdsVar.WifiSarPowerDelta[2].GeoSarOffset2400Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[2].GeoSarOffset5200Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[2].GeoSarOffset6000To7000Mhz.MaxAllowed = 0xFF;
  WgdsVar.WifiSarPowerDelta[2].GeoSarOffset2400Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[2].GeoSarOffset5200Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[2].GeoSarOffset6000To7000Mhz.ChainA     = 0x00;
  WgdsVar.WifiSarPowerDelta[2].GeoSarOffset2400Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[2].GeoSarOffset5200Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[2].GeoSarOffset6000To7000Mhz.ChainB     = 0x00;
  WgdsVar.WifiSarPowerDelta[3].GeoSarOffset2400Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[3].GeoSarOffset5200Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[3].GeoSarOffset6000To7000Mhz.MaxAllowed = 0xFF;
  WgdsVar.WifiSarPowerDelta[3].GeoSarOffset2400Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[3].GeoSarOffset5200Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[3].GeoSarOffset6000To7000Mhz.ChainA     = 0x00;
  WgdsVar.WifiSarPowerDelta[3].GeoSarOffset2400Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[3].GeoSarOffset5200Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[3].GeoSarOffset6000To7000Mhz.ChainB     = 0x00;
  WgdsVar.WifiSarPowerDelta[4].GeoSarOffset2400Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[4].GeoSarOffset5200Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[4].GeoSarOffset6000To7000Mhz.MaxAllowed = 0xFF;
  WgdsVar.WifiSarPowerDelta[4].GeoSarOffset2400Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[4].GeoSarOffset5200Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[4].GeoSarOffset6000To7000Mhz.ChainA     = 0x00;
  WgdsVar.WifiSarPowerDelta[4].GeoSarOffset2400Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[4].GeoSarOffset5200Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[4].GeoSarOffset6000To7000Mhz.ChainB     = 0x00;
  WgdsVar.WifiSarPowerDelta[5].GeoSarOffset2400Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[5].GeoSarOffset5200Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[5].GeoSarOffset6000To7000Mhz.MaxAllowed = 0xFF;
  WgdsVar.WifiSarPowerDelta[5].GeoSarOffset2400Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[5].GeoSarOffset5200Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[5].GeoSarOffset6000To7000Mhz.ChainA     = 0x00;
  WgdsVar.WifiSarPowerDelta[5].GeoSarOffset2400Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[5].GeoSarOffset5200Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[5].GeoSarOffset6000To7000Mhz.ChainB     = 0x00;
  WgdsVar.WifiSarPowerDelta[6].GeoSarOffset2400Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[6].GeoSarOffset5200Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[6].GeoSarOffset6000To7000Mhz.MaxAllowed = 0xFF;
  WgdsVar.WifiSarPowerDelta[6].GeoSarOffset2400Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[6].GeoSarOffset5200Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[6].GeoSarOffset6000To7000Mhz.ChainA     = 0x00;
  WgdsVar.WifiSarPowerDelta[6].GeoSarOffset2400Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[6].GeoSarOffset5200Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[6].GeoSarOffset6000To7000Mhz.ChainB     = 0x00;
  WgdsVar.WifiSarPowerDelta[7].GeoSarOffset2400Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[7].GeoSarOffset5200Mhz.MaxAllowed       = 0xFF;
  WgdsVar.WifiSarPowerDelta[7].GeoSarOffset6000To7000Mhz.MaxAllowed = 0xFF;
  WgdsVar.WifiSarPowerDelta[7].GeoSarOffset2400Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[7].GeoSarOffset5200Mhz.ChainA           = 0x00;
  WgdsVar.WifiSarPowerDelta[7].GeoSarOffset6000To7000Mhz.ChainA     = 0x00;
  WgdsVar.WifiSarPowerDelta[7].GeoSarOffset2400Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[7].GeoSarOffset5200Mhz.ChainB           = 0x00;
  WgdsVar.WifiSarPowerDelta[7].GeoSarOffset6000To7000Mhz.ChainB     = 0x00;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_WGDS),
             CNV_UEFI_WGDS_VAR_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&WgdsVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install PPAG for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallPpag (
  )
{
  CNV_UEFI_VAR_PPAG  PpagVar;
  EFI_STATUS         Status;

  ZeroMem (&PpagVar, sizeof (CNV_UEFI_VAR_PPAG));
  PpagVar.Header.Revision                             = CNV_UEFI_PPAG_REVISION;
  PpagVar.WifiAntGainMode.PpagModesU32                = 0;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_EU           = 0x00;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_CHINA        = 0x00;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_EU_LPI_UHB   = 0x00;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_EU_VLP_UHB   = 0x00;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_EU_SP_UHB    = 0x00;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_FCC_LPI_UHB  = 0x00;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_FCC_VLP_UHB  = 0x00;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_FCC_SP_UHB   = 0x00;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_ISED_LPI_UHB = 0x00;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_ISED_VLP_UHB = 0x00;
  PpagVar.WifiAntGainMode.BitsModes.PPAG_ISED_SP_UHB  = 0x00;
  PpagVar.AntennaGainChainA[0]                        = 0x18;
  PpagVar.AntennaGainChainA[1]                        = 0x28;
  PpagVar.AntennaGainChainA[2]                        = 0x28;
  PpagVar.AntennaGainChainA[3]                        = 0x28;
  PpagVar.AntennaGainChainA[4]                        = 0x28;
  PpagVar.AntennaGainChainA[5]                        = 0x28;
  PpagVar.AntennaGainChainA[6]                        = 0x28;
  PpagVar.AntennaGainChainA[7]                        = 0x28;
  PpagVar.AntennaGainChainA[8]                        = 0x28;
  PpagVar.AntennaGainChainA[9]                        = 0x28;
  PpagVar.AntennaGainChainA[10]                       = 0x28;
  PpagVar.AntennaGainChainB[0]                        = 0x18;
  PpagVar.AntennaGainChainB[1]                        = 0x28;
  PpagVar.AntennaGainChainB[2]                        = 0x28;
  PpagVar.AntennaGainChainB[3]                        = 0x28;
  PpagVar.AntennaGainChainB[4]                        = 0x28;
  PpagVar.AntennaGainChainB[5]                        = 0x28;
  PpagVar.AntennaGainChainB[6]                        = 0x28;
  PpagVar.AntennaGainChainB[7]                        = 0x28;
  PpagVar.AntennaGainChainB[8]                        = 0x28;
  PpagVar.AntennaGainChainB[9]                        = 0x28;
  PpagVar.AntennaGainChainB[10]                       = 0x28;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_PPAG),
             CNV_UEFI_PPAG_VAR_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&PpagVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install WTAS for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallWtas (
  )
{
  CNV_UEFI_VAR_WTAS  WtasVar;
  EFI_STATUS         Status;

  ZeroMem (&WtasVar, sizeof (CNV_UEFI_VAR_WTAS));
  WtasVar.Header.Revision = CNV_UEFI_WTAS_REVISION_1;
  WtasVar.TASSelection        = 0x00;
  WtasVar.NumOfBlockedEntries = 0x00;
  WtasVar.BlockedListEntry[0] = 0x00;
  WtasVar.BlockedListEntry[1] = 0x00;
  WtasVar.BlockedListEntry[2] = 0x00;
  WtasVar.BlockedListEntry[3] = 0x00;
  WtasVar.BlockedListEntry[4] = 0x00;
  WtasVar.BlockedListEntry[5] = 0x00;
  WtasVar.BlockedListEntry[6] = 0x00;
  WtasVar.BlockedListEntry[7] = 0x00;
  WtasVar.BlockedListEntry[8] = 0x00;
  WtasVar.BlockedListEntry[9] = 0x00;
  WtasVar.BlockedListEntry[10]= 0x00;
  WtasVar.BlockedListEntry[11]= 0x00;
  WtasVar.BlockedListEntry[12]= 0x00;
  WtasVar.BlockedListEntry[13]= 0x00;
  WtasVar.BlockedListEntry[14]= 0x00;
  WtasVar.BlockedListEntry[15]= 0x00;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_WTAS),
             CNV_UEFI_WTAS_VAR_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&WtasVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install WLAN General Config for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallWlanGeneralCfg (
  )
{
  STATIC WLAN_GENERAL_CONFIG_SET  WlanGeneralCfgVar = {
    {
      CNV_UEFI_WLAN_GENERAL_CFG_REVISION_4 // Revision
    },
    {
      0x00, // Function[0]
      0x00, // Function[1]
      0x00, // Function[2]
      0x00, // Function[3]
      0x00, // Function[4]
      0x00, // Function[5]
      0x00, // Function[6]
      0x00, // Function[7]
      0x00, // Function[8]
      0x00, // Function[9]
      0x00, // Function[10]
      0x00, // Function[11]
      0x00, // Function[12]
      0x00, // Function[13]
      0x00, // Function[14]
      0x00, // Function[15]
      0x00, // Function[16]
      0x00, // Function[17]
      0x00, // Function[18]
      0x00, // Function[19]
      0x00, // Function[20]
      0x00, // Function[21]
      0x00, // Function[22]
      0x00, // Function[23]
      0x00, // Function[24]
      0x00, // Function[25]
      0x00, // Function[26]
      0x00, // Function[27]
      0x00, // Function[28]
      0x00, // Function[29]
      0x00, // Function[30]
      0x00  // Function[31]
    }
  };
  EFI_STATUS                      Status;


  Status = UpdateVariables (
             sizeof (WLAN_GENERAL_CONFIG_SET),
             CNV_UEFI_WLAN_GENERAL_CFG_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&WlanGeneralCfgVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install WiFi OEM PNVM

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallWlanOemPnvm (
  )
{
  WLAN_OEM_PNVM  WlanOemPnvmVar;
  EFI_STATUS     Status;

  ZeroMem (&WlanOemPnvmVar, sizeof (WLAN_OEM_PNVM));
  WlanOemPnvmVar.Header.Revision = CNV_UEFI_WLAN_OEM_PNVM_REVISION_1;
  WlanOemPnvmVar.TotalPackagesSizeInBytes  = 0x00;
  WlanOemPnvmVar.NubmberOfSkusPackages     = 0x00;
  WlanOemPnvmVar.Reserved                  = 0x00;
  WlanOemPnvmVar.Reserved2                 = 0x00;
  WlanOemPnvmVar.IntelSignedPnvmSkuList[0] = 0x00;

  Status = UpdateVariables (
             sizeof (WLAN_OEM_PNVM),
             CNV_UEFI_WLAN_OEM_PNVM_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&WlanOemPnvmVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install Tx Power Reduction for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallWlanReducedPower (
  )
{
  WLAN_TX_REDUCED_POWER  WlanReducedPowerVar;
  EFI_STATUS             Status;

  ZeroMem (&WlanReducedPowerVar, sizeof (WLAN_TX_REDUCED_POWER));
  WlanReducedPowerVar.Header.Revision = CNV_UEFI_WLAN_REDUCED_POWER_REVISON_1;
  WlanReducedPowerVar.TotalPackagesSizeInBytes  = 0x00;
  WlanReducedPowerVar.NubmberOfSkusPackages     = 0x00;
  WlanReducedPowerVar.Reserved                  = 0x00;
  WlanReducedPowerVar.Reserved2                 = 0x00;
  WlanReducedPowerVar.ReducedPowerPayload[0]    = 0x00;

  Status = UpdateVariables (
             sizeof (WLAN_TX_REDUCED_POWER),
             CNV_UEFI_WLAN_REDUCED_POWER_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&WlanReducedPowerVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install Phy Filter Configuration for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallWpfc (
  )
{
  UEFI_CNV_VAR_WPFC  WpfcVar;
  EFI_STATUS         Status;

  ZeroMem (&WpfcVar, sizeof (UEFI_CNV_VAR_WPFC));
  WpfcVar.Header.Revision = UEFI_CNV_WPFC_REVISION_0;
  WpfcVar.Wpfc.WifiPhyFilterConfigA = 0x00;
  WpfcVar.Wpfc.WifiPhyFilterConfigB = 0x00;
  WpfcVar.Wpfc.WifiPhyFilterConfigC = 0x00;
  WpfcVar.Wpfc.WifiPhyFilterConfigD = 0x00;

  Status = UpdateVariables (
             sizeof (UEFI_CNV_VAR_WPFC),
             UEFI_CNV_WPFC_VAR_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&WpfcVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install SGOM for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallSgom (
  )
{
  CNV_UEFI_WLAN_SGOM_DATA  SgomVar;
  EFI_STATUS               Status;

  SgomVar.Header.Revision = CNV_UEFI_WLAN_SGOM_REVISION_1;
  CopyMem (SgomVar.CountriesSarOffsetMap.U8Raw, mSgomInitTable, HORIZONTAL_COUNTRIES_LETTER_MAX * VERTICAL_COUNTRIES_LETTER_MAX / 2);

  Status = UpdateVariables (
             sizeof (CNV_UEFI_WLAN_SGOM_DATA),
             CNV_UEFI_WLAN_SGOM_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&SgomVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install BRDS Revision 2 for BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallBrdsRev2 (
  )
{
  CNV_UEFI_VAR_BRDS     BrdsVarV2;
  EFI_STATUS            Status;

  ZeroMem (&BrdsVarV2, sizeof (CNV_UEFI_VAR_BRDS));
  BrdsVarV2.Header.Revision = CNV_UEFI_BRDS_REVISION_2;
  BrdsVarV2.Mode = 0x00;
  BrdsVarV2.IncreasedPowerMode                     = 0x00;
  BrdsVarV2.BtSarValuesChainA.subBand2G4           = 0x00;
  BrdsVarV2.BtSarValuesChainA.subBand5G2           = 0x00;
  BrdsVarV2.BtSarValuesChainA.subBand5G85G9        = 0x00;
  BrdsVarV2.BtSarValuesChainA.subBand6G1           = 0x00;
  BrdsVarV2.BtSarValuesChainA.subBand6G3           = 0x00;
  BrdsVarV2.BtSarValuesChainB.subBand2G4           = 0x00;
  BrdsVarV2.BtSarValuesChainB.subBand5G2           = 0x00;
  BrdsVarV2.BtSarValuesChainB.subBand5G85G9        = 0x00;
  BrdsVarV2.BtSarValuesChainB.subBand6G1           = 0x00;
  BrdsVarV2.BtSarValuesChainB.subBand6G3           = 0x00;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_BRDS),
             CNV_UEFI_BRDS_VAR_NAME,
             &gUefiIntelCnvBtVariablesGuid,
             (VOID *)&BrdsVarV2,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install BTLC for BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallBtlc (
  )
{
  CNV_UEFI_VAR_BTLC  BtlcVar;
  EFI_STATUS         Status;

  ZeroMem (&BtlcVar, sizeof (CNV_UEFI_VAR_BTLC));
  BtlcVar.Header.Revision = CNV_UEFI_BTLC_REVISION_1;
  BtlcVar.Mode             = 0x00;
  BtlcVar.S0               = 0x00;
  BtlcVar.S0ix             = 0x00;
  BtlcVar.S4               = 0x00;
  BtlcVar.S5               = 0x00;
  BtlcVar.LedConfig        = 0x00;
  BtlcVar.LedPulseDuration = 0x00;
  BtlcVar.LedPulseInterval = 0x00;
  BtlcVar.TileInAPM        = 0x00;
  BtlcVar.Reserved         = 0x00;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_BTLC),
             CNV_UEFI_BTLC_VAR_NAME,
             &gUefiIntelCnvBtVariablesGuid,
             (VOID *)&BtlcVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

#if (FixedPcdGet16 (PcdCnvUefiVarVersion) == 4)

/**
  Install AOLD for BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallAold (
  )
{
  CNV_UEFI_VAR_AOLD  AoldVar;
  EFI_STATUS         Status;

  AoldVar.Header.Revision       = CNV_UEFI_AOLD_REVISION_0;
  AoldVar.AudioOffloadSelection = 0;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_AOLD),
             CNV_UEFI_AOLD_VAR_NAME,
             &gUefiIntelCnvBtVariablesGuid,
             (VOID *)&AoldVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

#endif

/**
  Install WCSC for WiFi, BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallWcsc (
  )
{
  CNV_UEFI_VAR_WCSC  WcscVar;
  EFI_STATUS         Status;

  ZeroMem (&WcscVar, sizeof (CNV_UEFI_VAR_WCSC));
  WcscVar.Header.Revision = CNV_UEFI_WCSC_VAR_REVISION_1;
  WcscVar.StepCharacterization.StepMode       = 0x00;
  WcscVar.StepCharacterization.CNVi_EQChannel = 0x00;
  WcscVar.StepCharacterization.CNVr_EQChannel = 0x00;
  WcscVar.StepCharacterization.Radio_1        = 0x00;
  WcscVar.StepCharacterization.Radio_2        = 0x00;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_WCSC),
             CNV_UEFI_WCSC_VAR_NAME,
             &gUefiIntelCnvCommonVariablesGuid,
             (VOID *)&WcscVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install SADS for WiFi, BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallSads (
  )
{
  CNV_UEFI_VAR_SADS  SadsVar;
  EFI_STATUS         Status;

  SadsVar.Header.Revision = CNV_UEFI_SADS_REVISION_0;
  SadsVar.AntDiversitySel = 2;

  //
  // Update SADS for Common GUID
  //
  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_SADS),
             CNV_UEFI_SADS_VAR_NAME,
             &gUefiIntelCnvCommonVariablesGuid,
             (VOID *)&SadsVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install ECKV for WiFi, BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallEckv (
  )
{
  CNV_UEFI_VAR_ECKV  EckvVar;
  EFI_STATUS         Status;

  EckvVar.Header.Revision       = CNV_UEFI_ECKV_REVISION_0;
  EckvVar.CnvExternal32kHzClock = 0;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_ECKV),
             CNV_UEFI_ECKV_VAR_NAME,
             &gUefiIntelCnvCommonVariablesGuid,
             (VOID *)&EckvVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install WwanCoEx for WiFi, BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallWwanCoEx (
  )
{
  ICNV_WWAN_SKU_REV_1  WwanCoExVar;
  EFI_STATUS           Status;

  WwanCoExVar.Header.Revision       = CNV_UEFI_WWAN_CO_EX_REVISION_1;
  WwanCoExVar.WwanMode              = 0;
  WwanCoExVar.AntIsolation.LowBand  = 15;
  WwanCoExVar.AntIsolation.HighBand = 15;

  Status = UpdateVariables (
             sizeof (ICNV_WWAN_SKU_REV_1),
             CNV_UEFI_WWAN_CO_EX_VAR_NAME,
             &gUefiIntelCnvCommonVariablesGuid,
             (VOID *)&WwanCoExVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install MPCC for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallMpcc (
  )
{
  WLAN_MPCC_CONFIG_SET  MpccVar;
  EFI_STATUS            Status;

  MpccVar.Header.Revision            = CNV_UEFI_WLAN_MPCC_REV;
  MpccVar.M21aAssembled              = 0;
  MpccVar.WifiPlatCurrentConsumption = 2;

  Status = UpdateVariables (
             sizeof (WLAN_MPCC_CONFIG_SET),
             CNV_UEFI_WLAN_MPCC_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&MpccVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install BBFB (BT BiQuad Filter Bypass) for BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallBbfb (
  )
{
  ICNV_BT_BIQUAD_BYPASS_REV_1  BbfbVar;
  EFI_STATUS                   Status;

  ZeroMem (&BbfbVar, sizeof (ICNV_BT_BIQUAD_BYPASS_REV_1));
  BbfbVar.Header.Revision = UEFI_CNV_BBFB_VAR_REV;
  BbfbVar.BiQuadBypass = 0x00;

  Status = UpdateVariables (
             sizeof (ICNV_BT_BIQUAD_BYPASS_REV_1),
             UEFI_CNV_BBFB_VAR_NAME,
             &gUefiIntelCnvBtVariablesGuid,
             (VOID *)&BbfbVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install PPAG for BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallBtPpag (
  )
{
  BT_PPAG_CONFIG_SET  PpagVar;
  EFI_STATUS          Status;

  PpagVar.Header.Revision = CNV_UEFI_BT_PPAG_VAR_REVISION;
  PpagVar.ppagModesU32    = 0x00000000;

  Status = UpdateVariables (
             sizeof (BT_PPAG_CONFIG_SET),
             CNV_UEFI_BT_PPAG_VAR_NAME,
             &gUefiIntelCnvBtVariablesGuid,
             (VOID *)&PpagVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install EBRD (Extended BT Descriptor) for BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallEbrd (
  )
{
  CNV_UEFI_VAR_EBRD  EbrdVar;
  EFI_STATUS         Status;

  ZeroMem (&EbrdVar, sizeof (CNV_UEFI_VAR_EBRD));
  EbrdVar.Header.Revision = CNV_UEFI_EBRD_REVISION_1;
  EbrdVar.BtDynamicSarMode      = 0x00;
  EbrdVar.BtDynamicSarRangeSets = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[0].BtSarPowerAntA.subBand2G4       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[0].BtSarPowerAntA.subBand5G2       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[0].BtSarPowerAntA.subBand5G85G9    = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[0].BtSarPowerAntA.subBand6G1       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[0].BtSarPowerAntA.subBand6G3       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[0].BtSarPowerAntB.subBand2G4       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[0].BtSarPowerAntB.subBand5G2       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[0].BtSarPowerAntB.subBand5G85G9    = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[0].BtSarPowerAntB.subBand6G1       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[0].BtSarPowerAntB.subBand6G3       = 0x00;

  EbrdVar.BtDynamicSarValues2G4Ext[1].BtSarPowerAntA.subBand2G4       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[1].BtSarPowerAntA.subBand5G2       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[1].BtSarPowerAntA.subBand5G85G9    = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[1].BtSarPowerAntA.subBand6G1       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[1].BtSarPowerAntA.subBand6G3       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[1].BtSarPowerAntB.subBand2G4       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[1].BtSarPowerAntB.subBand5G2       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[1].BtSarPowerAntB.subBand5G85G9    = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[1].BtSarPowerAntB.subBand6G1       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[1].BtSarPowerAntB.subBand6G3       = 0x00;

  EbrdVar.BtDynamicSarValues2G4Ext[2].BtSarPowerAntA.subBand2G4       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[2].BtSarPowerAntA.subBand5G2       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[2].BtSarPowerAntA.subBand5G85G9    = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[2].BtSarPowerAntA.subBand6G1       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[2].BtSarPowerAntA.subBand6G3       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[2].BtSarPowerAntB.subBand2G4       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[2].BtSarPowerAntB.subBand5G2       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[2].BtSarPowerAntB.subBand5G85G9    = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[2].BtSarPowerAntB.subBand6G1       = 0x00;
  EbrdVar.BtDynamicSarValues2G4Ext[2].BtSarPowerAntB.subBand6G3       = 0x00;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_EBRD),
             CNV_UEFI_EBRD_VAR_NAME,
             &gUefiIntelCnvBtVariablesGuid,
             (VOID *)&EbrdVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install Bands Selection Mode for BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallBtBandsSelection (
  )
{
  ICNV_BT_BANDS_SELECTION_MODE_REV_1  BtBandsSelectionVar;
  EFI_STATUS                          Status;

  ZeroMem (&BtBandsSelectionVar, sizeof (ICNV_BT_BANDS_SELECTION_MODE_REV_1));
  BtBandsSelectionVar.Header.Revision = CNV_UEFI_BT_BANDS_SEL_REVISION_1;
  BtBandsSelectionVar.BandsSelection = 0x00;

  Status = UpdateVariables (
             sizeof (ICNV_BT_BANDS_SELECTION_MODE_REV_1),
             CNV_UEFI_BT_BANDS_SEL_VAR_NAME,
             &gUefiIntelCnvBtVariablesGuid,
             (VOID *)&BtBandsSelectionVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install UATS (UHB AP Type Support) for WiFi

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallUats (
  )
{
  CNV_UEFI_WLAN_UATS_DATA  UatsVar;
  EFI_STATUS               Status;

  ZeroMem (&UatsVar, sizeof (CNV_UEFI_WLAN_UATS_DATA));
  UatsVar.Header.Revision = CNV_UEFI_WLAN_UATS_REV_1;
  CopyMem (UatsVar.CountriesUhbApTypeSupportMap.U8Raw, mUatsInitTable, HORIZONTAL_COUNTRIES_LETTER_MAX * VERTICAL_COUNTRIES_LETTER_MAX / 2);

  Status = UpdateVariables (
             sizeof (CNV_UEFI_WLAN_UATS_DATA),
             CNV_UEFI_WLAN_UATS_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&UatsVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install UHB Country Select for BT

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallBtUhbCountrySelect (
  )
{
  ICNV_BT_UHB_COUNTRY_SELECT_REV_1  BtUhbCountrySelVar;
  EFI_STATUS                        Status;

  ZeroMem (&BtUhbCountrySelVar, sizeof (ICNV_BT_UHB_COUNTRY_SELECT_REV_1));
  BtUhbCountrySelVar.Header.Revision = CNV_UEFI_BT_UHB_COUNTRY_SEL_REV_1;
  BtUhbCountrySelVar.CountrySelectionBitmap = 0x00;

  Status = UpdateVariables (
             sizeof (ICNV_BT_UHB_COUNTRY_SELECT_REV_1),
             CNV_UEFI_BT_UHB_COUNTRY_SELECT_VAR_NAME,
             &gUefiIntelCnvBtVariablesGuid,
             (VOID *)&BtUhbCountrySelVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install CNVi Dual Chain Mode for BT

  @retval EFI_SUCCESS        The function completed successfully
  @retval others             Failed to install UEFI variable

**/
EFI_STATUS
InstallBtDualChainMode (
  )
{
  ICNV_BT_DUAL_CHAIN_MODE_REV_1  BtDcmVar;
  EFI_STATUS                     Status;

  ZeroMem (&BtDcmVar, sizeof (ICNV_BT_DUAL_CHAIN_MODE_REV_1));
  BtDcmVar.Header.Revision = CNV_UEFI_BT_DUAL_CHAIN_MODE_REVISION_1;
  BtDcmVar.DualChainMode = 0x00;

  Status = UpdateVariables (
             sizeof (ICNV_BT_DUAL_CHAIN_MODE_REV_1),
             CNV_UEFI_BT_DUAL_CHAIN_MODE_NAME,
             &gUefiIntelCnvBtVariablesGuid,
             (VOID *)&BtDcmVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install CNVi Dual MAC Mode for BT

  @retval EFI_SUCCESS        The function completed successfully
  @retval others             Failed to install UEFI variable

**/
EFI_STATUS
InstallBtDualMacMode (
  )
{
  ICNV_BT_DUAL_MAC_REV_1  BtDmmVar;
  EFI_STATUS              Status;

  ZeroMem (&BtDmmVar, sizeof (ICNV_BT_DUAL_MAC_REV_1));
  BtDmmVar.Header.Revision = ICNV_BT_DUAL_MAC_MODE_REVISION_1;
  BtDmmVar.DualMacMode = 0x00;

  Status = UpdateVariables (
             sizeof (ICNV_BT_DUAL_MAC_REV_1),
             CNV_UEFI_BT_DUAL_MAC_MODE_NAME,
             &gUefiIntelCnvBtVariablesGuid,
             (VOID *)&BtDmmVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install PUNC for WiFi

  @retval EFI_SUCCESS        The function completed successfully
  @retval others             Failed to install UEFI variable

**/
EFI_STATUS
InstallPunc (
  )
{
  CNV_UEFI_VAR_WIFI_PUNCTURING  PuncVar;
  EFI_STATUS                    Status;

  PuncVar.Header.Revision = UEFI_CNV_PUNC_REVISION;
  PuncVar.Puncturing      = 0;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_WIFI_PUNCTURING),
             UEFI_CNV_PUNC_VAR_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&PuncVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install Sensing for WiFi

  @retval EFI_SUCCESS        The function completed successfully
  @retval others             Failed to install UEFI variable

**/
EFI_STATUS
InstallWlanSensing (
  VOID
  )
{
  UEFI_CNV_VAR_WIFI_SENSING  SensingVar;
  EFI_STATUS                 Status;

  ZeroMem (&SensingVar, sizeof (UEFI_CNV_VAR_WIFI_SENSING));
  SensingVar.Header.Revision = UEFI_CNV_WLAN_SENSIN_REVISION;
  SensingVar.SensingSettings        = 0x00;
  SensingVar.SensingConfiguration   = 0x00;
  SensingVar.NumOfPreferredChannels = 0x00;
  SensingVar.PreferredChannels[0]   = 0x00;
  SensingVar.PreferredChannels[1]   = 0x00;
  SensingVar.PreferredChannels[2]   = 0x00;
  SensingVar.PreferredChannels[3]   = 0x00;
  SensingVar.PreferredChannels[4]   = 0x00;
  SensingVar.PreferredChannels[5]   = 0x00;
  SensingVar.PreferredChannels[6]   = 0x00;
  SensingVar.PreferredChannels[7]   = 0x00;
  SensingVar.PreferredChannels[8]   = 0x00;
  SensingVar.PreferredChannels[9]   = 0x00;
  SensingVar.PreferredChannels[10]  = 0x00;
  SensingVar.PreferredChannels[11]  = 0x00;
  SensingVar.PreferredChannels[12]  = 0x00;
  SensingVar.PreferredChannels[13]  = 0x00;
  SensingVar.PreferredChannels[14]  = 0x00;
  SensingVar.PreferredChannels[15]  = 0x00;

  Status = UpdateVariables (
             sizeof (UEFI_CNV_VAR_WIFI_SENSING),
             UEFI_CNV_WLAN_SENSING_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&SensingVar,
             FALSE,
             FALSE
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install WBEM (Wi-Fi 320MHz Bandwidth Enablement per MCC) for WiFi

  @retval EFI_SUCCESS        The function completed successfully
  @retval others             Failed to install UEFI variable

**/
EFI_STATUS
InstallWbem (
  )
{
  CNV_UEFI_VAR_WIFI_320MHz_PER_MCC     WbemVar;
  EFI_STATUS                           Status;

  ZeroMem (&WbemVar, sizeof (CNV_UEFI_VAR_WIFI_320MHz_PER_MCC));
  WbemVar.Header.Revision = CNV_UEFI_WLAN_WBEM_REV;
  WbemVar.WiFi320MHzPerMcc = 0x00000000;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_WIFI_320MHz_PER_MCC),
             CNV_UEFI_WLAN_WBEM_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *) &WbemVar,
             FALSE,
             FALSE
             );
  ASSERT (!EFI_ERROR (Status));

  return Status;
}

/**
  Install DSBR (Drive Strength BRI Rsp) for WiFi, BT

  @retval EFI_SUCCESS        The function completed successfully
  @retval others             Failed to install UEFI variable
**/
EFI_STATUS
InstallDsbr (
  )
{
  CNV_UEFI_VAR_DSBR                 DsbrVar;
  EFI_STATUS                        Status;

  ZeroMem (&DsbrVar, sizeof (CNV_UEFI_VAR_DSBR));
  DsbrVar.Header.Revision        = CNV_UEFI_DSBR_VAR_REVISION;
  DsbrVar.CnvDriveStrengthBriRsp = 0x000000F1;

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_DSBR),
             CNV_UEFI_DSBR_VAR_NAME,
             &gUefiIntelCnvCommonVariablesGuid,
             (VOID *) &DsbrVar,
             FALSE,
             FALSE
             );
  ASSERT (!EFI_ERROR (Status));

  return Status;
}

/**
  Connectivity Config Initialization

  @retval EFI_SUCCESS        The function completed successfully

**/
VOID
CnvConfigInit (
  IN UEFI_CNV_CONFIG  *CnvConfig
  )
{
}

/**
  Install CNV UEFI Variables

  @retval EFI_SUCCESS        The function completed successfully

**/
EFI_STATUS
InstallCnvUefiVariables (
  IN UEFI_CNV_CONFIG  *CnvConfig
  )
{
  EFI_STATUS  Status;

  Status = InstallSplc ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallWrdd ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallWrds ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallEwrd ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallWgds ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallPpag ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallWtas ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallWlanGeneralCfg ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallWlanOemPnvm ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallWpfc ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallSgom ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallWwanCoEx ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallBrdsRev2 ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallEbrd ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallBtBandsSelection ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallUats ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallBtUhbCountrySelect ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallBtDualChainMode ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallBtDualMacMode ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallBtlc ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallWcsc ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallSads ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallEckv ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallMpcc ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallBbfb ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallBtPpag ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status =  InstallWlanSensing ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallWbem ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallDsbr ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

 #if (FixedPcdGet16 (PcdCnvUefiVarVersion) == 4)
  Status =  InstallPunc ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InstallAold ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

 #endif

  return Status;
}
