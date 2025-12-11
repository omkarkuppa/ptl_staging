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
#include <IpSim.h>
#include <IpSimLib.h>
#include <IpSimRegMap.h>
#include <IpWrapper.h>
#endif  //IPSIM_ENABLE

#include <Library/PmcLib.h>
#include <Library/PmcPrivateLib.h>
#ifdef IPSIM_ENABLE
extern EFI_GUID gTcssHobGuid;
#endif

static EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) PtlTcssInitEndOfPei
};

EFI_PEI_PPI_DESCRIPTOR mTcssPeiInitDonePpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gTcssPeiInitDonePpiGuid,
  NULL
};

/**
  Installs TCSS data HOB
  The HOB stores TCSS data required for later use in DXE phase.

  @retval EFI_SUCCESS           The HOB was successfully created.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space for HOB creation.
**/
EFI_STATUS
InstallTcssDataHob (
  VOID
  )
{

  EFI_STATUS      Status;
  TCSS_DATA_HOB   *TcssHob;

  ///
  /// Create HOB for TCSS Data
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (TCSS_DATA_HOB),
             (VOID **) &TcssHob
             );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Initialize the HOB header data
  ///
  TcssHob->EfiHobGuidType.Name = gTcssHobGuid;
  DEBUG ((DEBUG_INFO, "TcssHob->EfiHobGuidType.Name: %g\n", &TcssHob->EfiHobGuidType.Name));
  ZeroMem (&(TcssHob->TcssData), sizeof (TCSS_DATA));

  DEBUG ((DEBUG_INFO, "TcssHob @ %X\n", TcssHob));
  DEBUG ((DEBUG_INFO, "&(TcssHob->TcssData) @ %X\n", &(TcssHob->TcssData)));
  DEBUG ((DEBUG_INFO, "TcssHob - HobHeader: %X\n", sizeof (TCSS_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "TcssData: %X\n", sizeof (TCSS_DATA)));

  return Status;
}

EFI_STATUS
PtlTcssInitPostMem (
  SI_POLICY_PPI   *SiPolicy,
  TCSS_PEI_PREMEM_CONFIG  *TcssPeiPreMemConfig,
  TCSS_PEI_CONFIG         *TcssPeiConfig,
  PEI_ITBT_CONFIG         *PeiITbtConfig,
  HOST_BRIDGE_PREMEM_CONFIG *HostBridgePreMemConfig
  )
{
  EFI_STATUS        Status;

  P2SB_CONTROLLER   P2SbController;

  TCSS_INST         TcssInst;
  TCSS_CALLBACKS    TcssCallbacks;
  TCSS_CONFIG       TcssOptions;
  TCSS_INFO         TcssInfo;

  IP_USB3_INST      Usb3Inst;
  IP_WR_REG_INFO    Usb3RegInfoPciPrim;
  IP_WR_REG_INFO    Usb3RegInfoMmio;
  IP_WR_REG_INFO    Usb3RegInfoPcr;
  IP_WR_REG_INFO    Usb3RegInfoDci;

  IP_USB4_INST      Usb4Inst;
  IP_USB4_DMA       Usb4Dma[MAX_HOST_ITBT_DMA_NUMBER];
  IP_WR_REG_INFO    Usb4DmaRegInfoPciPrim[MAX_HOST_ITBT_DMA_NUMBER];

  IP_IOM_INST       IomInst;
  IP_WR_REG_INFO    IomRegInfoPcr;

  IP_TC_PCIE_RP_INST PcieRpInst;
  IP_PCIE_ROOT_PORT  PcieRpPorts[MAX_ITBT_PCIE_PORT];
  IP_WR_REG_INFO     PcieRpRegInfoPciPrim[MAX_ITBT_PCIE_PORT];
  IP_WR_REG_INFO     PcieRpRegInfoPcr[MAX_ITBT_PCIE_PORT];
  IP_PCIE_CALLBACKS  PcieCallbacks;

  IOM_DEVEN_CONFIG   DevEn;

  UINT32                        Index;
  TCSS_DATA_HOB                 *TcssHob;
  TCSS_USBTC_PEI_PERMEM_CONFIG  *UsbTcPeiPreMemConfig;

  Status = EFI_SUCCESS;
  UsbTcPeiPreMemConfig = &(TcssPeiPreMemConfig->UsbTcConfig);

  // Tables below only needed in PostMem since that's when USB3 will be initialized
  IP_CSI_TUNING_PARAM_ENTRY  PtlUsb3TuningList[] = {
    { IpCsiTuningParamIdReleaseInfo,          0x20230124 },
    { IpUsb3TuningParamIdCp13Deemphasis,      0x00000A86 },
    { IpUsb3TuningParamIdCp14Deemphasis,      0x00007A40 },
    { IpUsb3TuningParamIdCp15Deemphasis,      0x00007944 },
    { IpUsb3TuningParamIdCp16Deemphasis,      0x00000C00 },
    { IpUsb3TuningParamIdLinkPollingMinTime,  0x2 },
    { IpUsb3TuningParamIdU2ExitLatency,       0x3E8 },
    { IpUsb3TuningParamIdHbufWaterMark,       0x2 },
    { IpUsb3TuningParamIdIdleHysteresisScale, 0x1},
    { IpUsb3TuningParamIdMinIdleSpan,         0x1E},
  };

  IP_USB3_FEATURE_LIST_ENTRY  PtlUsb3FeatureList[] = {
    { IpUsb3FeatIdHhhFlushFix,                    IpUsb3FeatValHhhFlushFixDis },
    { IpUsb3FeatIdB2bRetryFix,                    IpUsb3FeatValB2bRetryFixEn },
    { IpUsb3FeatIdReadAfterWriteDelay,            IpUsb3FeatValReadAfterWriteDelayEn },
    { IpUsb3FeatIdHSAsyncActiveExposure,          IpUsb3FeatValHSAsyncActiveExposureDis },
    { IpUsb3FeatIdDbcTraceInEp,                   IpUsb3FeatValDbcTraceInEpEn },
    { IpUsb3FeatIdBandwidthCheck,                 IpUsb3FeatValBandwidthCheckEn },
  };

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
                            TcssPeiConfig,
                            PeiITbtConfig,
                            HostBridgePreMemConfig
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR: PTL TCSS Instance Structure initialization fail!\n"));
    return Status;
  }

  Usb3Inst.TuningList = PtlUsb3TuningList;
  Usb3Inst.TuningListEntryCount = sizeof (PtlUsb3TuningList) / sizeof (PtlUsb3TuningList[0]);
  Usb3Inst.FeatureList = PtlUsb3FeatureList;
  Usb3Inst.FeatureListEntryCount = sizeof (PtlUsb3FeatureList) / sizeof (PtlUsb3FeatureList[0]);

  //
  // TCSS initialization procedure for Post Memory
  //
  Status = SsTcssInitPostMemory (&TcssInst);
  if (Status) {
    DEBUG ((DEBUG_ERROR, "ERROR: PTL TCSS Post Memory initialization fail!\n"));
    if (Status == IpCsiStsErrorNotAllowed) {
      DEBUG ((DEBUG_ERROR, "ERROR: IOM is corrupted- Disable TCSS devices!\n"));
    }
    DevEn.TcssDevEn = 0;
    HostBridgeSetTcssDeven (DevEn.TcssDevEn);
    TcssInst.IomInst->IomConfig.DevEn.TcssDevEnBit.TcssPcieRpEn = 0;
    TcssInst.IomInst->IomConfig.DevEn.TcssDevEnBit.Usb4HrEn     = 0;
    TcssInst.IomInst->IomConfig.DevEn.TcssDevEnBit.TcssXhciEn   = 0;
  }

  //
  // Update TCSS HOB data for future use by other modules
  //
  InstallTcssDataHob ();

  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Cannot find TCSS HOB!!!\n"));
    return EFI_NOT_FOUND;
  }

  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    TcssHob->TcssData.ItbtPcieRpEn[Index]       = (TcssInst.IomInst->IomConfig.DevEn.TcssDevEnBit.TcssPcieRpEn >> Index) & 1;
  }
  TcssHob->TcssData.ItbtXhciEn                  = (UINT8)(TcssInst.IomInst->IomConfig.DevEn.TcssDevEnBit.TcssXhciEn);
  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index++) {
    TcssHob->TcssData.ItbtDmaEn[Index]          = (TcssInst.IomInst->IomConfig.DevEn.TcssDevEnBit.Usb4HrEn >> Index) & 1;
  }

  for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
    if (IS_TC_PORT_USB_SUPPORTED (UsbTcPeiPreMemConfig->PortIndex.CapPolicy[Index])) {
      TcssHob->TcssData.TcssUsbEnMap |= (UINT8)(BIT0 << Index);
    } else {
      TcssHob->TcssData.TcssUsbEnMap &= (UINT8)~(BIT0 << Index);
    }
  }

  CopyMem (&TcssHob->TcssPeiPreMemCfgBlk, TcssPeiPreMemConfig, sizeof (TCSS_PEI_PREMEM_CONFIG));
  DEBUG ((DEBUG_INFO, "TcssHob Update TcssPeiPreMemConfig Done\n"));

  CopyMem (&TcssHob->TcssPeiCfgBlk, TcssPeiConfig, sizeof (TCSS_PEI_CONFIG));
  DEBUG ((DEBUG_INFO, "TcssHob Update TcssPeiConfig Done\n"));

  //
  // Performing TcssInitEndOfPei after EndOfPei PPI produced
  //
  PeiServicesNotifyPpi (&mEndOfPeiNotifyList);

  PeiServicesInstallPpi (&mTcssPeiInitDonePpi);

  return Status;
}