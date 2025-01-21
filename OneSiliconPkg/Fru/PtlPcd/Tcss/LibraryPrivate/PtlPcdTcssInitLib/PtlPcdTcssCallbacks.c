/** @file
  TCSS Callbacks implementation for PTL FRU/SOC.

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

@par Specification
**/

#include "PtlPcdTcssInitPrivate.h"
#include <Library/PeiSiSsidLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Defines/TcssOcDefinesPcd.h>

/**
  Get Device Enables form SOC IO Cache service

  @param[out]  DevEnCfg  Pointer to Device Enables

  @retval     Status
**/
UINT32
PtlIpIocTcssGetDevEn (
  VOID
  )
{
#ifdef IPSIM_ENABLE
  IP_WR_REG_CNTXT Cntxt;
#endif

  DEBUG ((DEBUG_INFO, "INFO: %a: - Entry\n", __FUNCTION__));

#ifdef IPSIM_ENABLE
  Cntxt = IpSimGetRegMap (MEM_IOC, 0, 0);
  return (UINT32)IpWrRegRead (Cntxt, 0x000073a8, IpWrRegFlagSize32Bits);
#else
  return HostBridgeGetTcssDeven ();
#endif
}

/**
  PCIe Rp downstream port configuration

  @param[in]  FuncMap PCIe RP function mapping
**/
VOID
PtlTcPcieRpDownStreamConfig (
  UINT8 Index
  )
{
  UINT32                             SaMchBar;
  UINT8                              RootPortFn;
  UINT8                              TempRootPortBusNumMin;
  UINT8                              TempRootPortBusNumMax;
  PCIE_FEATURE_CONFIGURATION         FeatureConfiguration;
  IOC_PMC_DEVEN_C_IOC_MCHBAR_STRUCT  SaTcssDevenCfg;
  UINT32                             BdfAllocRegOffset;
  BDF_ALLOC_C_0_IOC_MCHBAR_STRUCT  BdfAllocCfg;

  SaMchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
  SaTcssDevenCfg.Data = MmioRead32 (SaMchBar + IOC_PMC_DEVEN_C_IOC_MCHBAR_REG);

  DEBUG ((DEBUG_INFO, "[TCSS] %a SaTcssDeven = 0x%x\n", __FUNCTION__, SaTcssDevenCfg.Data));
  FeatureConfiguration.ConfigureMrrs = FALSE;

  TempRootPortBusNumMin = PcdGet8 (PcdSiliconInitTempPciBusMin);
  TempRootPortBusNumMax = PcdGet8 (PcdSiliconInitTempPciBusMax);

  ///
  /// BdfAllocRegOffset = BDF_ALLOC_C_0_IOC_MCHBAR_REG + (PMC_DEVEN_C[i] * 4),
  /// PMC_DEVEN_C[i] = Index
  ///
  BdfAllocRegOffset = BDF_ALLOC_C_0_IOC_MCHBAR_REG + (Index * 4);
  BdfAllocCfg.Data = MmioRead32 (SaMchBar + BdfAllocRegOffset);
  RootPortFn = (UINT8)(BdfAllocCfg.Bits.func << BDF_ALLOC_C_0_IOC_MCHBAR_FUNC_LSB);
  DEBUG ((DEBUG_INFO, "[TCSS] %a RootPortFn[%d] = 0x%x\n", __FUNCTION__, Index, RootPortFn));

  RootportDownstreamConfiguration (
    SA_SEG_NUM,
    SA_MC_BUS,
    (UINT8)ITBT_PCIE_DEV_NUM,
    (UINT8)RootPortFn,
    TempRootPortBusNumMin,
    TempRootPortBusNumMax,
    EnumiTbtPcie,
    &FeatureConfiguration
    );
}

STATIC SVID_SID_SKIP_TABLE mTcssSsidTable[] = {{ITBT_PCIE_BUS_NUM,  ITBT_PCIE_DEV_NUM,  ITBT_PCIE_0_DEFAULT_FUN_NUM},
                                               {ITBT_PCIE_BUS_NUM,  ITBT_PCIE_DEV_NUM,  ITBT_PCIE_1_DEFAULT_FUN_NUM},
                                               {ITBT_PCIE_BUS_NUM,  ITBT_PCIE_DEV_NUM,  ITBT_PCIE_2_DEFAULT_FUN_NUM},
                                               {ITBT_PCIE_BUS_NUM,  ITBT_PCIE_DEV_NUM,  ITBT_PCIE_3_DEFAULT_FUN_NUM},
                                               {XHCI_NORTH_BUS_NUM, XHCI_NORTH_DEV_NUM, XHCI_NORTH_FUNC_NUM},
                                               {HOST_DMA_BUS_NUM,   HOST_DMA_DEV_NUM,   HOST_DMA0_FUN_NUM},
                                               {HOST_DMA_BUS_NUM,   HOST_DMA_DEV_NUM,   HOST_DMA1_FUN_NUM}
                                              };

VOID
PtlSetSvidSsid (
  VOID
  )
{
  EFI_STATUS    Status;
  UINT16        NumberOfTcssTable;
  SI_POLICY_PPI *SiPolicyPpi;

  DEBUG ((DEBUG_INFO, "%a\n",__FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR getting SiPolicyPpi \n", __FUNCTION__));
  }

  NumberOfTcssTable = (UINT16) sizeof (mTcssSsidTable) / sizeof (mTcssSsidTable[0]);
  // TCSS SSID SVID setting has to program after IOM ready.
  SiTcssProgramSsid (SiPolicyPpi, mTcssSsidTable, NumberOfTcssTable);
}

/**
  Enables PCIe PME Events for TCSS Root Ports

**/
VOID
PtlEnablePciePmeEvents (
  VOID
  )
{
  PmcEnablePciExpressPmeEvents (PmcGetPwrmBase ());
}

/**
  Calculates Overcurrent pin number to match USB3 OCM register bit map
  @param[in]  pInst             IP_USB3_INST structure pointer
  @param[in]  OcPin             Index of lane to check
**/
UINT32
CalculateOcPin (
  IP_USB3_INST  *pInst,
  UINT32        OcPin
  )
{
  //
  // OcPin == OCM Index 0, 1, 2, 3...
  //
  return TCSS_USB3_OCM_INDEX(OcPin);
}
