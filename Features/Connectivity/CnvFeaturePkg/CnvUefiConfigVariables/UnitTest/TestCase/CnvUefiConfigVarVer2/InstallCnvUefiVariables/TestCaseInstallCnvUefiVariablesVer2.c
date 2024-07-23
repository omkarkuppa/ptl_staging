/**@file
  Unit test case of the CnvUefiConfigVariablesVer2 driver.

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

@par Specification
**/

#include <CnvUefiConfigVariables.h>
#include "TestCaseInstallCnvUefiVariablesVer2.h"
#include <BootServicesMock.h>
#include <RunTimeServicesMock.h>
#include <PrivateMock.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

extern EFI_RUNTIME_SERVICES  *gRT;

/**
  Install CNV UEFI Variables

  @retval EFI_SUCCESS        The function completed successfully

**/
extern
EFI_STATUS
InstallCnvUefiVariables (
  IN UEFI_CNV_CONFIG  *CnvConfig
  );

/**
  Unit-Test Function for install CNV UEFI Variables.

  @param[in]  Context                            An optional parameter for Unit Test.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInstallCnvUefiVariablesVer2 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                          Status;
  UEFI_CNV_CONFIG                     *CnvConfig;
  CNV_UEFI_VAR_SPLC                   *GetSplcVar;
  CNV_UEFI_VAR_WRDD                   *GetWrddVar;
  CNV_UEFI_VAR_WRDS_WIFI_SAR          *GetWrdsVar;
  CNV_UEFI_VAR_EWRD                   *GetEwrdVar;
  CNV_UEFI_VAR_WGDS                   *GetWgdsVar;
  CNV_UEFI_VAR_PPAG                   *GetPpagVar;
  CNV_UEFI_VAR_WTAS                   *GetWtasVar;
  WLAN_GENERAL_CONFIG_SET             *GetWlanGeneralCfgVar;
  WLAN_OEM_PNVM                       *GetWlanOemPnvmVar;
  UEFI_CNV_VAR_WPFC                   *GetWpfcVar;
  CNV_UEFI_WLAN_SGOM_DATA             *GetSgomVar;
  ICNV_WWAN_SKU_REV_1                 *GetCnvWwanCoExVar;
  CNV_UEFI_VAR_BTLC                   *GetBtlcVar;
  CNV_UEFI_VAR_WCSC                   *GetWcscVar;
  CNV_UEFI_VAR_SADS                   *GetSadsVar;
  CNV_UEFI_VAR_ECKV                   *GetEckvVar;
  WLAN_MPCC_CONFIG_SET                *GetMpccVar;
  ICNV_BT_BIQUAD_BYPASS_REV_1         *GetBbfbVar;
  BT_PPAG_CONFIG_SET                  *GetBtPpagVar;
  UEFI_CNV_VAR_WIFI_SENSING           *GetWlanSensing;
  CNV_UEFI_VAR_BRDS_R2                *GetBrdsVarV2;
  CNV_UEFI_VAR_EBRD                   *GetEbrdVar;
  ICNV_BT_BANDS_SELECTION_MODE_REV_1  *GetBtBandsSelectionVar;
  CNV_UEFI_WLAN_UATS_DATA             *GetUatsVar;
  ICNV_BT_UHB_COUNTRY_SELECT_REV_1    *GetBtUhbCountrySelVar;
  ICNV_BT_DUAL_CHAIN_MODE_REV_1       *GetBtDcmVar;
  ICNV_BT_DUAL_MAC_REV_1              *GetBtDmmVar;

 #if (FixedPcdGet16 (PcdCnvUefiVarVersion) == 4)
  CNV_UEFI_VAR_WIFI_PUNCTURING  *GetPuncVar;
 #endif

  CnvConfig        = AllocateZeroPool (sizeof (UEFI_CNV_CONFIG));
  gRT              = AllocateZeroPool (sizeof (EFI_RUNTIME_SERVICES));
  gRT->GetVariable = MockGetVariable;
  //
  // InstallSplc ()
  //
  GetSplcVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_SPLC));
  expect_any_always (MockGetVariable, VariableName);
  expect_any_always (MockGetVariable, VendorGuid);
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_SPLC));
  will_return (MockGetVariable, GetSplcVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallWrdd ()
  //
  GetWrddVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_WRDD));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_WRDD));
  will_return (MockGetVariable, GetWrddVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallWrds ()
  //
  GetWrdsVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_WRDS_WIFI_SAR));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_WRDS_WIFI_SAR));
  will_return (MockGetVariable, GetWrdsVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallEwrd ()
  //
  GetEwrdVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_EWRD));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_EWRD));
  will_return (MockGetVariable, GetEwrdVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallWgds ()
  //
  GetWgdsVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_WGDS));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_WGDS));
  will_return (MockGetVariable, GetWgdsVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallPpag ()
  //
  GetPpagVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_PPAG));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_PPAG));
  will_return (MockGetVariable, GetPpagVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallWtas ()
  //
  GetWtasVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_WTAS));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_WTAS));
  will_return (MockGetVariable, GetWtasVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallWlanGeneralCfg ()
  //
  GetWlanGeneralCfgVar = AllocateZeroPool (sizeof (WLAN_GENERAL_CONFIG_SET));
  will_return (MockGetVariable, sizeof (WLAN_GENERAL_CONFIG_SET));
  will_return (MockGetVariable, GetWlanGeneralCfgVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallWlanOemPnvm ()
  //
  GetWlanOemPnvmVar = AllocateZeroPool (sizeof (WLAN_OEM_PNVM));
  will_return (MockGetVariable, sizeof (WLAN_OEM_PNVM));
  will_return (MockGetVariable, GetWlanOemPnvmVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallWpfc ()
  //
  GetWpfcVar = AllocateZeroPool (sizeof (UEFI_CNV_VAR_WPFC));
  will_return (MockGetVariable, sizeof (UEFI_CNV_VAR_WPFC));
  will_return (MockGetVariable, GetWpfcVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallSgom ()
  //
  GetSgomVar = AllocateZeroPool (sizeof (CNV_UEFI_WLAN_SGOM_DATA));
  will_return (MockGetVariable, sizeof (CNV_UEFI_WLAN_SGOM_DATA));
  will_return (MockGetVariable, GetSgomVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallWwanCoEx ()
  //
  GetCnvWwanCoExVar = AllocateZeroPool (sizeof (ICNV_WWAN_SKU_REV_1));
  will_return (MockGetVariable, sizeof (ICNV_WWAN_SKU_REV_1));
  will_return (MockGetVariable, GetCnvWwanCoExVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);

  //
  // InstallBrdsRev2 ()
  //
  GetBrdsVarV2 = AllocateZeroPool (sizeof (CNV_UEFI_VAR_BRDS_R2));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_BRDS_R2));
  will_return (MockGetVariable, GetBrdsVarV2);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallEbrd ()
  //
  GetEbrdVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_EBRD));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_EBRD));
  will_return (MockGetVariable, GetEbrdVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallBtBandsSelection ()
  //
  GetBtBandsSelectionVar = AllocateZeroPool (sizeof (ICNV_BT_BANDS_SELECTION_MODE_REV_1));
  will_return (MockGetVariable, sizeof (ICNV_BT_BANDS_SELECTION_MODE_REV_1));
  will_return (MockGetVariable, GetBtBandsSelectionVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallUats ()
  //
  GetUatsVar = AllocateZeroPool (sizeof (CNV_UEFI_WLAN_UATS_DATA));
  will_return (MockGetVariable, sizeof (CNV_UEFI_WLAN_UATS_DATA));
  will_return (MockGetVariable, GetUatsVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallBtUhbCountrySelect ()
  //
  GetBtUhbCountrySelVar = AllocateZeroPool (sizeof (ICNV_BT_UHB_COUNTRY_SELECT_REV_1));
  will_return (MockGetVariable, sizeof (ICNV_BT_UHB_COUNTRY_SELECT_REV_1));
  will_return (MockGetVariable, GetBtUhbCountrySelVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallBtDualChainMode ()
  //
  GetBtDcmVar = AllocateZeroPool (sizeof (ICNV_BT_DUAL_CHAIN_MODE_REV_1));
  will_return (MockGetVariable, sizeof (ICNV_BT_DUAL_CHAIN_MODE_REV_1));
  will_return (MockGetVariable, GetBtDcmVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallBtDualMacMode ()
  //
  GetBtDmmVar = AllocateZeroPool (sizeof (ICNV_BT_DUAL_MAC_REV_1));
  will_return (MockGetVariable, sizeof (ICNV_BT_DUAL_MAC_REV_1));
  will_return (MockGetVariable, GetBtDmmVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);

 #if (FixedPcdGet16 (PcdCnvUefiVarVersion) == 4)
  //
  // InstallPunc ()
  //
  GetPuncVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_WIFI_PUNCTURING));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_WIFI_PUNCTURING));
  will_return (MockGetVariable, GetPuncVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
 #endif
  //
  // InstallBtlc ()
  //
  GetBtlcVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_BTLC));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_BTLC));
  will_return (MockGetVariable, GetBtlcVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallWcsc ()
  //
  GetWcscVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_WCSC));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_WCSC));
  will_return (MockGetVariable, GetWcscVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallSads ()
  //
  GetSadsVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_SADS));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_SADS));
  will_return (MockGetVariable, GetSadsVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallEckv ()
  //
  GetEckvVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_ECKV));
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_ECKV));
  will_return (MockGetVariable, GetEckvVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallMpcc ()
  //
  GetMpccVar = AllocateZeroPool (sizeof (WLAN_MPCC_CONFIG_SET));
  will_return (MockGetVariable, sizeof (WLAN_MPCC_CONFIG_SET));
  will_return (MockGetVariable, GetMpccVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallBbfb ()
  //
  GetBbfbVar = AllocateZeroPool (sizeof (ICNV_BT_BIQUAD_BYPASS_REV_1));
  will_return (MockGetVariable, sizeof (ICNV_BT_BIQUAD_BYPASS_REV_1));
  will_return (MockGetVariable, GetBbfbVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallBtPpag ()
  //
  GetBtPpagVar = AllocateZeroPool (sizeof (BT_PPAG_CONFIG_SET));
  will_return (MockGetVariable, sizeof (BT_PPAG_CONFIG_SET));
  will_return (MockGetVariable, GetBtPpagVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);
  //
  // InstallWlanSensing ()
  //
  GetWlanSensing = AllocateZeroPool (sizeof (BT_PPAG_CONFIG_SET));
  will_return (MockGetVariable, sizeof (BT_PPAG_CONFIG_SET));
  will_return (MockGetVariable, GetWlanSensing);
  will_return (MockGetVariable, EFI_SUCCESS);

  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);

  //
  // Execute test function with above mock environment
  //
  Status = InstallCnvUefiVariables (CnvConfig);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  FreePool (CnvConfig);
  FreePool (gRT);
  FreePool (GetSplcVar);
  FreePool (GetWrddVar);
  FreePool (GetWrdsVar);
  FreePool (GetEwrdVar);
  FreePool (GetWgdsVar);
  FreePool (GetPpagVar);
  FreePool (GetWtasVar);
  FreePool (GetWlanGeneralCfgVar);
  FreePool (GetWlanOemPnvmVar);
  FreePool (GetWpfcVar);
  FreePool (GetSgomVar);
  FreePool (GetCnvWwanCoExVar);
  FreePool (GetBtlcVar);
  FreePool (GetWcscVar);
  FreePool (GetSadsVar);
  FreePool (GetEckvVar);
  FreePool (GetMpccVar);
  FreePool (GetBbfbVar);
  FreePool (GetBtPpagVar);
  FreePool (GetWlanSensing);
  FreePool (GetBrdsVarV2);
  FreePool (GetEbrdVar);
  FreePool (GetBtBandsSelectionVar);
  FreePool (GetUatsVar);
  FreePool (GetBtUhbCountrySelVar);
  FreePool (GetBtDcmVar);
  FreePool (GetBtDmmVar);
 #if (FixedPcdGet16 (PcdCnvUefiVarVersion) == 4)
  FreePool (GetPuncVar);
 #endif
  return UNIT_TEST_PASSED;
}
