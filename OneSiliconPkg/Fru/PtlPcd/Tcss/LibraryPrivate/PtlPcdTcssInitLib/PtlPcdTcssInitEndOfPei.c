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
#endif

#include <ITbtInfoHob.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>

/**
  Get IOM Reg Bar Base

  @retval          UINT64        IOM Reg Bar Base
**/
UINT64
GetIomRegBase (
  VOID
  )
{
  P2SB_PORT_16_ID                 P2SBPid;
  P2SB_CONTROLLER                 P2SbController;

  P2SBPid.Pid16bit = PTL_SID_F3_PID_TC_IOM;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);

  return (P2SbController.Mmio | (P2SBPid.PortId.LocalPid << 16));
}

VOID
PtlTcssCreateTbtHob (
  TCSS_INST       *TcssInst
  )
{
  SI_POLICY_PPI         *SiPolicyPpi;
  PEI_ITBT_CONFIG       *PeiITbtConfig;
  ITBT_INFO_HOB         *ITbtInfoHob;
  TCSS_DATA_HOB         *TcssHob;
  UINT8                 EnableDma;
  EFI_STATUS            Status;
  UINT32                Index;

  DEBUG ((DEBUG_INFO, "%a\n",__FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPeiITbtConfigGuid, (VOID *) &PeiITbtConfig);
  ASSERT_EFI_ERROR (Status);

  EnableDma = 0;
  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index++) {
    if (((TcssInst->Config->Deven.TcssDevEnBit.Usb4HrEn >> Index) & 1)) {
      EnableDma = 1;
      break;
    }
  }

  if (EnableDma == 0) {
    DEBUG ((DEBUG_INFO, "All DMAs and RPs disabled - ITbtInfoHob is not created!\n"));
  } else {

    ITbtInfoHob = GetFirstGuidHob (&gITbtInfoHobGuid);
    if (ITbtInfoHob == NULL) {
      //
      // Create HOB for ITBT Data
      //
      Status = PeiServicesCreateHob (
                EFI_HOB_TYPE_GUID_EXTENSION,
                sizeof (ITBT_INFO_HOB),
                (VOID **) &ITbtInfoHob
                );
      DEBUG ((DEBUG_INFO, "ITbtInfoHob Created \n"));
    }

    if (ITbtInfoHob != NULL) {
      //
      // Initialize the ITBT INFO HOB data.
      //
      ITbtInfoHob->EfiHobGuidType.Name = gITbtInfoHobGuid;

      TcssHob = NULL;
      ///
      /// Locate HOB for TCSS Data
      ///
      TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);

      //
      // Update ITBT Policy
      //
      for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
        if (TcssHob == NULL) {
          ITbtInfoHob->ITbtRootPortConfig[Index].ITbtPcieRootPortEn = 0;
          DEBUG ((DEBUG_INFO, "TcssHob not found\n"));
        } else {
          ITbtInfoHob->ITbtRootPortConfig[Index].ITbtPcieRootPortEn = TcssHob->TcssData.ItbtPcieRpEn[Index];
        }
      }
      ITbtInfoHob->ITbtForcePowerOnTimeoutInMs    = PeiITbtConfig->ITbtGenericConfig.ITbtForcePowerOnTimeoutInMs;
      ITbtInfoHob->ITbtConnectTopologyTimeoutInMs = PeiITbtConfig->ITbtGenericConfig.ITbtConnectTopologyTimeoutInMs;
      ITbtInfoHob->ITbtPcieTunnelingForUsb4       = PeiITbtConfig->ITbtGenericConfig.ITbtPcieTunnelingForUsb4;
      ITbtInfoHob->Usb4CmMode                     = PeiITbtConfig->ITbtGenericConfig.Usb4CmMode;
    }
  }
}

EFI_STATUS
PtlTcssInitEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS        Status;
  IP_CSI_STATUS     CsiStatus;

  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi;
  SI_POLICY_PPI           *SiPolicyPpi;
  PEI_ITBT_CONFIG         *PeiITbtConfig;
  TCSS_PEI_PREMEM_CONFIG  *TcssPeiPreMemConfig;
  TCSS_PEI_CONFIG         *TcssPeiConfig;
  HOST_BRIDGE_PREMEM_CONFIG *HostBridgePreMemConfig;

  TCSS_INST         TcssInst;
  TCSS_CALLBACKS    TcssCallbacks;
  TCSS_CONFIG       TcssOptions;
  TCSS_INFO         TcssInfo;

  P2SB_CONTROLLER   P2SbController;

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

  IOM_DEVEN_CONFIG   DevEn;
  IP_PCIE_CALLBACKS  PcieCallbacks;

  TCSS_DATA_SSID_HOB *TcssSsidHob;
  TCSS_DATA_HOB      *TcssHob;
  UINT32             Index;
  UINT16             RetimerMap;

  DEBUG ((DEBUG_INFO, "%a\n",__FUNCTION__));

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesLocatePpi (&gSiPolicyPpiGuid, 0, NULL, (VOID **) &SiPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gTcssPeiConfigGuid, (VOID *) &TcssPeiConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPeiITbtConfigGuid, (VOID *) &PeiITbtConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);
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

  //
  // Create TBT HOB
  //
  PtlTcssCreateTbtHob (&TcssInst);

  //
  // TCSS initialization procedure for End of PEI
  //
  CsiStatus = SsTcssInitEndOfPei (&TcssInst);
  if (CsiStatus) {
    DEBUG ((DEBUG_ERROR, "ERROR: PTL TCSS End of PEI initialization fail!\n"));
    if (CsiStatus == IpCsiStsNotReady) {
      DEBUG ((DEBUG_ERROR, "ERROR: TBT or MG FW not loaded to IMR - Disable TCSS devices!\n"));
      DevEn.TcssDevEn = 0;
      HostBridgeSetTcssDeven (DevEn.TcssDevEn);
    }
    Status = EFI_DEVICE_ERROR;
  }

  //
  // Read TCSS info data. The data will be used to update TCSS HOB
  //
  CsiStatus = SsTcssGetInfo (&TcssInst);
  if (CsiStatus) {
    DEBUG ((DEBUG_ERROR, "ERROR: PTL TCSS - Cannot read info data completely!\n"));
    Status = EFI_DEVICE_ERROR;
  }

  //
  // Update TCSS HOB data for future use by other modules
  //
  TcssHob = (TCSS_DATA_HOB*) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Cannot find TCSS HOB!!!\n"));
    return EFI_NOT_FOUND;
  }
  TcssHob->TcssData.IOMReady                    = TcssInst.Info->IOMReady;
  TcssHob->TcssData.PcieMultipleSegmentEnabled  = FALSE;
  TcssHob->TcssData.SwConfiguration1Lock        = TcssInst.Info->SwConfiguration1Lock;
  TcssHob->TcssData.SwConfiguration3Lock        = TcssInst.Info->SwConfiguration3Lock;
  TcssHob->TcssData.SwConfiguration4Lock        = TcssInst.Info->SwConfiguration4Lock;
  TcssHob->TcssData.IomFwVersion                = TcssInst.Info->IomFwVersion;
  TcssHob->TcssData.IomFwEngrVersion            = TcssInst.Info->IomFwEngrVersion;
  TcssHob->TcssData.TbtImrStatus.RegValue       = TcssInst.Info->TbtImrStatus.RegValue;
  TcssHob->TcssData.ItbtPcieFuncMap             = TcssInst.Info->ItbtPcieFuncMap;
  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    TcssHob->TcssData.ItbtPcieRpEn[Index]       = (TcssInst.IomInst->IomConfig.DevEn.TcssDevEn >> Index) & 1;
  }
  TcssHob->TcssData.ItbtXhciEn                  = (UINT8)TcssInst.Config->Deven.TcssDevEnBit.TcssXhciEn;
  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index++) {
    TcssHob->TcssData.ItbtDmaEn[Index]          = (TcssInst.IomInst->IomConfig.DevEn.TcssDevEnBit.Usb4HrEn >> Index) & 1;
  }
  TcssHob->TcssData.TcssRtd3                    = (UINT8)TcssInst.IomInst->IomConfig.D3ColdEnable;
  TcssHob->TcssData.MgImrStatus.RegValue        = TcssInst.Info->MgImrStatus.RegValue;
  TcssHob->TcssData.TcssIomVccSt                = (UINT8)TcssInst.IomInst->IomConfig.VccSt;
  TcssHob->TcssData.DeepstTcState               = (UINT8)TcssInst.Info->DeepStTcState;
  TcssHob->TcssData.IomBase                     = GetIomRegBase ();

  RetimerMap = TcssInst.IomInst->IomConfig.AuxOriOverride;
  for (Index = 0; Index < MAX_IOM_AUX_BIAS_COUNT; Index++) {
    //
    // Check IOM_TYPEC_SW_CONFIGURATION_3_PORTx_RETIMER_DISABLED bit. If the retimer disabled bit is 0,
    // it means there is retimer attached to the port.
    //
    if ((RetimerMap & (BIT0 << (Index * 2))) == 0) {
      TcssHob->TcssData.RetimerMap |= BIT0 << Index;
    }
  }

  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    TcssHob->PcieItbtLtrData[Index].LtrEnable                         = PcieRpPorts[Index].PcieRpConfig.LtrEnable;
    TcssHob->PcieItbtLtrData[Index].LtrMaxSnoopLatency                = (UINT16)PcieRpPorts[Index].PcieRpConfig.LtrMaxSnoopLatency;
    TcssHob->PcieItbtLtrData[Index].LtrMaxNoSnoopLatency              = (UINT16)PcieRpPorts[Index].PcieRpConfig.LtrMaxNoSnoopLatency;
    TcssHob->PcieItbtLtrData[Index].SnoopLatencyOverrideMode          = (UINT8)PcieRpPorts[Index].PcieRpConfig.SnoopLatencyOverrideMode;
    TcssHob->PcieItbtLtrData[Index].SnoopLatencyOverrideMultiplier    = (UINT8)PcieRpPorts[Index].PcieRpConfig.SnoopLatencyOverrideMultiplier;
    TcssHob->PcieItbtLtrData[Index].SnoopLatencyOverrideValue         = (UINT16)PcieRpPorts[Index].PcieRpConfig.SnoopLatencyOverrideValue;
    TcssHob->PcieItbtLtrData[Index].NonSnoopLatencyOverrideMode       = (UINT8)PcieRpPorts[Index].PcieRpConfig.NonSnoopLatencyOverrideMode;
    TcssHob->PcieItbtLtrData[Index].NonSnoopLatencyOverrideMultiplier = (UINT8)PcieRpPorts[Index].PcieRpConfig.NonSnoopLatencyOverrideMultiplier;
    TcssHob->PcieItbtLtrData[Index].NonSnoopLatencyOverrideValue      = (UINT16)PcieRpPorts[Index].PcieRpConfig.NonSnoopLatencyOverrideValue;
    TcssHob->PcieItbtLtrData[Index].ForceLtrOverride                  = (UINT8)PcieRpPorts[Index].PcieRpConfig.ForceLtrOverride;
    TcssHob->PcieItbtLtrData[Index].LtrConfigLock                     = (UINT8)PcieRpPorts[Index].PcieRpConfig.LtrConfigLock;
  }

  //
  // the TcssSsidHob only use PEI phase, so claer value before entry DXE phase.
  //
  TcssSsidHob = NULL;
  TcssSsidHob = (TCSS_DATA_SSID_HOB *) GetFirstGuidHob (&gTcssSsidHobGuid);
  if (TcssSsidHob != NULL) {
    TcssSsidHob->TcssSsidTable = NULL;
    TcssSsidHob->NumberOfTcssSsidTable = 0;
  }

  DEBUG ((DEBUG_INFO, "%a END\n",__FUNCTION__));
  return Status;
}