/** @file
  TCSS Pre Memory Initialization for PTL FRU/SOC.

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

#ifdef IPSIM_ENABLE
#include <stdio.h>
#include <string.h>
#include <IpSim.h>
#include <IpSimLib.h>
#include <IpSimRegMap.h>
#include <IpWrapper.h>
#endif

#include <TcssDataHob.h>

#include <Library/PcdGpioNativeLib.h>

STATIC SVID_SID_SKIP_TABLE mTcssSsidTable[] = {{ITBT_PCIE_BUS_NUM,  ITBT_PCIE_DEV_NUM,  ITBT_PCIE_0_DEFAULT_FUN_NUM},
                                               {ITBT_PCIE_BUS_NUM,  ITBT_PCIE_DEV_NUM,  ITBT_PCIE_1_DEFAULT_FUN_NUM},
                                               {ITBT_PCIE_BUS_NUM,  ITBT_PCIE_DEV_NUM,  ITBT_PCIE_2_DEFAULT_FUN_NUM},
                                               {ITBT_PCIE_BUS_NUM,  ITBT_PCIE_DEV_NUM,  ITBT_PCIE_3_DEFAULT_FUN_NUM},
                                               {XHCI_NORTH_BUS_NUM, XHCI_NORTH_DEV_NUM, XHCI_NORTH_FUNC_NUM},
                                               {HOST_DMA_BUS_NUM,   HOST_DMA_DEV_NUM,   HOST_DMA0_FUN_NUM},
                                               {HOST_DMA_BUS_NUM,   HOST_DMA_DEV_NUM,   HOST_DMA1_FUN_NUM}
                                              };


#ifdef IPSIM_ENABLE
EFI_GUID gTcssSsidHobGuid = { 0x8903d47a, 0x8f82, 0x4063, { 0xa8, 0x40, 0x31, 0x68, 0x9c, 0x9e, 0x78, 0x20} };
EFI_GUID gTcssHobGuid = { 0x455702ce, 0x4adb, 0x45d9, { 0x8b, 0x27, 0xf7, 0xb0, 0xd9, 0x79, 0x8a, 0xe0} };
EFI_GUID gSiPolicyPpiGuid = { 0xaebffa01, 0x7edc, 0x49ff, {0x8d, 0x88, 0xcb, 0x84, 0x8c, 0x5e, 0x86, 0x70} };
EFI_GUID gPeiITbtConfigGuid = { 0xd7e7e1e6, 0xcbec, 0x4f5f, {0xae, 0xd3, 0xfd, 0xc0, 0xa8, 0xb0, 0x7e, 0x25} };
#endif

/**
  Update TcssSsidTable adress from cache to permanent memory.

**/
VOID
UpdateTcssSsidDataHobafterMemoryDiscovery (
  VOID
  )
{
  TCSS_DATA_SSID_HOB    *TcssSsidHob;

  TcssSsidHob = GetFirstGuidHob (&gTcssSsidHobGuid);

  ///
  /// Update table address
  ///
  TcssSsidHob->TcssSsidTable = mTcssSsidTable;
}

EFI_PEI_NOTIFY_DESCRIPTOR  mUpdateTcssSsidDataHob = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiMemoryDiscoveredPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) UpdateTcssSsidDataHobafterMemoryDiscovery
};

/**
  Installs HOB with TCSS PCI devices Bus, Device, Function data.
  The HOB is consumend by Sub System and Sub Vendor IDS (SSID, SVID)
  programing procedure (for entire system).
  The HOB data is used to skip TCSS PCI functions SSID, SVID programming
  at early boot phase where TCSS is not initialized yet.
  TCSS SSID, SVID programming procedure is invoked later from TCSS code
  in a moment where the PCI devices are properly initialized.

  @retval EFI_SUCCESS           The HOB was successfully created.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space for HOB creation.
**/
EFI_STATUS
InstallTcssSsidDataHob (
  VOID
  )
{

  EFI_STATUS            Status;
  TCSS_DATA_SSID_HOB    *TcssSsidHob;

  ///
  /// Create HOB for TCSS SSID Data
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (TCSS_DATA_SSID_HOB),
             (VOID **) &TcssSsidHob
             );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Initialize the HOB header data
  ///
  TcssSsidHob->EfiHobGuidType.Name = gTcssSsidHobGuid;
  TcssSsidHob->TcssSsidTable = mTcssSsidTable;
  TcssSsidHob->NumberOfTcssSsidTable = (UINT16) sizeof (mTcssSsidTable) / sizeof (mTcssSsidTable[0]);

  return Status;
}

EFI_STATUS
PtlTcssInitPreMem (
  IN TCSS_PEI_PREMEM_CONFIG      *TcssPeiPreMemConfig,
  IN HOST_BRIDGE_PREMEM_CONFIG   *HostBridgePreMemConfig
  )
{
  EFI_STATUS         Status;

  TCSS_INST          TcssInst;
  TCSS_CALLBACKS     TcssCallbacks;
  TCSS_CONFIG        TcssOptions;
  TCSS_INFO          TcssInfo;

  P2SB_CONTROLLER    P2SbController;

  IP_USB3_INST       Usb3Inst;
  IP_WR_REG_INFO     Usb3RegInfoPciPrim;
  IP_WR_REG_INFO     Usb3RegInfoMmio;
  IP_WR_REG_INFO     Usb3RegInfoPcr;
  IP_WR_REG_INFO     Usb3RegInfoDci;

  IP_USB4_INST       Usb4Inst;
  IP_USB4_DMA        Usb4Dma[MAX_HOST_ITBT_DMA_NUMBER];
  IP_WR_REG_INFO     Usb4DmaRegInfoPciPrim[MAX_HOST_ITBT_DMA_NUMBER];

  IP_IOM_INST        IomInst;
  IP_WR_REG_INFO     IomRegInfoPcr;

  IP_TC_PCIE_RP_INST PcieRpInst;
  IP_PCIE_ROOT_PORT  PcieRpPorts[MAX_ITBT_PCIE_PORT];
  IP_WR_REG_INFO     PcieRpRegInfoPciPrim[MAX_ITBT_PCIE_PORT];
  IP_WR_REG_INFO     PcieRpRegInfoPcr[MAX_ITBT_PCIE_PORT];
  IP_PCIE_CALLBACKS  PcieCallbacks;

  Status = EFI_SUCCESS;

  //
  // Initialize PTL TCSS Instance Structure
  //
  // Note:
  // Memory is allocated on stack to prevent memory leaks in PEI phase
  //
  Status = PtlTcssInstInit (&TcssInst,
                            &TcssCallbacks,
                            &TcssOptions,
                            &TcssInfo,
                            &Usb3Inst,
                            &Usb3RegInfoPciPrim,
                            &Usb3RegInfoMmio,
                            &Usb3RegInfoPcr,
                            &Usb3RegInfoDci,
                            &P2SbController,
                            &Usb4Inst,
                            Usb4Dma,
                            Usb4DmaRegInfoPciPrim,
                            &IomInst,
                            &IomRegInfoPcr,
                            &PcieRpInst,
                            PcieRpPorts,
                            PcieRpRegInfoPciPrim,
                            PcieRpRegInfoPcr,
                            &PcieCallbacks,
                            TcssPeiPreMemConfig,
                            NULL, // TCSS Post-Mem config not available at Pre-Mem
                            NULL, // TBT config not available at Pre-Mem
                            HostBridgePreMemConfig
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR: PTL TCSS Instance Structure initialization fail!\n"));
    return Status;
  }

  //
  // TCSS initialization procedure for Pre Memory
  //
  Status = SsTcssInitPreMemory (&TcssInst);
  if (Status) {
    DEBUG ((DEBUG_ERROR, "ERROR: PTL TCSS Pre Memory initialization fail!\n"));
    return Status;
  }

  //
  // Install SSID/SVID HOB for TCSS
  //
  InstallTcssSsidDataHob ();
  Status = PeiServicesNotifyPpi (&mUpdateTcssSsidDataHob);
  ASSERT_EFI_ERROR (Status);

  return Status;
}