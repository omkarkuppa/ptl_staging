/** @file
  The TCSS Init Library

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

#include <IpCsi.h>
#include <IpWrapper.h>
#include <IpWrapperBitDef.h>

#include <TcssLib.h>
#include <IpIom.h>
#include <IpUsb4.h>
#include "TcssInitPrivate.h"
#include <Library/TcssPmcLib.h>
#include <TcssPeiPreMemConfig.h>

/**
  Validates if USB4 Host Router is enabled and ready for initialization

  @param[in]  pInst             TCSS Instance

  @retval  BOOLEAN
    - TRUE:   USB4 Host Router is enabled
    - FALSE:  USB4 Host Router is disabled or not ready
**/
BOOLEAN
IsUsb4HrEnabled (
  TCSS_INST   *pInst
  )
{
  IP_CSI_STATUS      Status;
  IOM_MG_IMR_STATUS  Imr;
  BOOLEAN            Usb4Exist;
  TCSS_DEVEN_CONFIG  Deven;
  UINT32             Index;

  Usb4Exist  = FALSE;

  Status = IpIomGetImrMgStatus (pInst->IomInst, &Imr.RegValue);

  if (Status != IpCsiStsSuccess) {
    if (Status == IpCsiStsNotReady) {
      PRINT_ERROR ("%a: ERROR: MG firmware not loaded to IMR! \n\tAbort TCSS initialization!\n", __FUNCTION__);
      // MG firmware not loaded to IMR.
      // All TCSS IPs in the SOC DEVEN register should be disabled
    }
  }

  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    if ((pInst->PcieInst->DevEn.DevEnBit.TcssPcieRpEn >> Index) & 1) {
      Usb4Exist = TRUE;
      break;
    }
  }

  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index++) {
    if ((pInst->Usb4Inst->Usb4Config.DevEn.DevEnBits.Usb4HrEn >> Index) & 1) {
      Usb4Exist = TRUE;
      break;
    }
  }

  Status = IpIomGetImrTbtStatus (pInst->IomInst, &Imr.RegValue);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: ERROR: TBT firmware not loaded to IMR! \n\tAbort TCSS initialization!\n", __FUNCTION__);
    // TBT firmware not loaded to IMR.
    if (Usb4Exist == TRUE) {
      PRINT_ERROR ("%a: TBT fatal error!\n", __FUNCTION__);
      return FALSE;
    }
    // All TCSS IPs in the SOC DEVEN register should be disabled
    Deven.TcssDevEn = pInst->Callbacks->GetTcssDeven();
    Deven.TcssDevEnBit.TcssPcieRpEn = 0;
    Deven.TcssDevEnBit.Usb4HrEn     = 0;
    pInst->IomInst->Callback.SetTcssDevenRegisterMchbar(Deven.TcssDevEn);

    return FALSE;
  }

  return TRUE;
}

/**
  TCSS Instance Initialization
  Initializes all Sub-IPs Instance structures

  @param[in]  pInst  The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
TcssInstInit (
  TCSS_INST   *pInst
  )
{
  IP_CSI_STATUS Status;

  TCSS_API_ENTRY ();

  Status = IpTcPcieRpInstInit (pInst->PcieInst);
  TCSS_RETURN_IF_ERROR (Status);

  Status = IpIomInstInit (pInst->IomInst);
  TCSS_RETURN_IF_ERROR (Status);

  Status = IpUsb3InstInit (pInst->Usb3Inst);
  TCSS_RETURN_IF_ERROR (Status);

  Status = IpUsb4InstInit (pInst->Usb4Inst);
  TCSS_RETURN_IF_ERROR (Status);

  return Status;
}

/**
  TCSS initialization procedure entry point

  @param[in]  pInst instance

  @retval  IP_CSI_STATUS
    - IpCsiStsSuccess:  programming completed without error
    - IpCsiStsErrorNullPtr:  null pointer to IP Instance provided
**/
IP_CSI_STATUS
SsTcssInit (
  TCSS_INST   *pInst
  )
{
  IP_CSI_STATUS Status;

  TCSS_API_ENTRY ();

  Status = SsTcssInitPreMemory (pInst);
  TCSS_RETURN_IF_ERROR (Status);

  Status = SsTcssInitPostMemory (pInst);
  TCSS_RETURN_IF_ERROR (Status);

  Status = SsTcssInitEndOfPei (pInst);
  TCSS_RETURN_IF_ERROR (Status);

  Status = SsTcssGetInfo (pInst);
  TCSS_RETURN_IF_ERROR (Status);

  return IpCsiStsSuccess;
}

/**
Returns version of Type-C Subsystem related components

@param[in]      pInst the instance
@param[in]      VerId version ID

@retval         0 is returned upon any error
@retval         return value define by VerId
**/

UINT32
TcssGetVersion (
  TCSS_INST       *pInst,
  IP_CSI_VER_ID   VerId
  )
{
  IP_CSI_VER_ID RetValue;

  TCSS_API_ENTRY ();

  RetValue = VERSION_UNKNOWN;

  switch (VerId) {
    case (IpCsiVerIdHwIp):
      RetValue = TCSS_VERSION_HW;
      break;
    case (IpCsiVerIdHwIpCustom):
      RetValue = TCSS_VERSION_HW_CUSTOM;
      break;
    case (IpCsiVerIdIpFwIntf):
      RetValue = TCSS_VERSION_IPFWINTF;
      break;
    case (IpCsiVerIdIpFwIntfExt):
      RetValue = TCSS_VERSION_IPFWINTFEXT;
      break;
    case (IpCsiVerIdIpFw):
      RetValue = TCSS_VERSION_FW;
      break;
    case (IpCsiVerIdCsi):
      RetValue = VERSION_CSI;
      break;
    default:
      RetValue = MAX_UINT32;
      break;
  }

  return RetValue;
}

/**
  TCSS initialization procedure executed before memory is discovered.

  @param[in]  pInst             TCSS Instance

  @retval  IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         actual register values differ from expected ones
    - IpCsiStsErrorNullPtr:  null pointer to IP Instance provided
**/
IP_CSI_STATUS
SsTcssInitPreMemory (
  TCSS_INST   *pInst
  )
{
  IP_CSI_STATUS   Status;
  UINT8           Index;
  UINT32          Reg32;

  Status      = IpCsiStsSuccess;
  Index       = 0;

  TCSS_API_ENTRY ();

  if (pInst->IomInst->TcssVersion != TcssOld) {
    IpIomEnableEdp (pInst->IomInst);
    Status = IpIomGetImrTbtStatus (pInst->IomInst, &Reg32);
    if (Status != IpCsiStsSuccess) {
      PRINT_WARNING ("%a: ERROR: TBT firmware not loaded to IMR in premem init new flow\n", __FUNCTION__);
    }
    IpIomSetUsb4Id (pInst->IomInst);
  }

  ///
  /// Configuring AUX Hsl orientation in IOM
  ///
  IpIomAuxHslOriInit (pInst->IomInst);

  ///
  /// Configure Fixed Connection for TCSS ports
  ///
  IpIomFixedConnectionPortInit(pInst->IomInst);

  PRINT_INFO ("Configure TCSS_PORT_CONFIG in IOM before Deven register update\n");
  if (pInst->IomInst->TcssVersion != TcssOld) {
    IpIomTcssSettings (pInst->IomInst);
    IpIomSetUsb4Rtd3 (pInst->IomInst);
  }

  ///
  ///   BIOS fills TCSS DEVEN value to IOP based on BIOS policy accordingly,
  ///   BIOS can only disables/enables supported TCSS DEV IP by SKU/Fuse accordingly.
  ///
  PRINT_INFO ("Configure TCSS_DEVEN in IOM\n");
  pInst->IomInst->Callback.SetTcssDevenRegisterMchbar (pInst->Config->Deven.TcssDevEn);

  ///
  ///   BIOS reads TCSS DEVEN register back into local stack,
  ///   the value reflects the exact TCSS DEV IP which can be enabled currently based on fuse info and BIOS policy.
  ///   (Failsafe to ensure IOM can get the exactly DEV map used by Punit)
  ///
  pInst->Config->Deven.TcssDevEn            = pInst->Callbacks->GetTcssDeven();
  pInst->PcieInst->DevEn.DevEn              = pInst->Config->Deven.TcssDevEn;
  pInst->Usb4Inst->Usb4Config.DevEn.DevEn   = pInst->Config->Deven.TcssDevEn;
  pInst->IomInst->IomConfig.DevEn.TcssDevEn = pInst->Config->Deven.TcssDevEn;

  PRINT_INFO ("SsTcssInitPreMemory: TCSS DEVEN = 0x%08X\n", pInst->Config->Deven.TcssDevEn);

  //
  // Writes the TCSS DEVEN data to IOM mailbox
  //
  Status = IpIomDevEnInit (pInst->IomInst);
  TCSS_RETURN_IF_ERROR (Status);

  //
  // Configure LSX pins in SOC GPIO controller
  //
  if (pInst->Callbacks->LsxPinsConfigure != NULL) {
    for (Index = 0; Index < pInst->Config->MaxTcssUsb3Ports; Index++) {
      if (IS_TC_PORT_TUNNELING_SUPPORTED (pInst->Config->PortEnData.CapPolicy[Index])) {
        pInst->Callbacks->LsxPinsConfigure (Index);
      }
    }
  }

  //
  // Enable Compatibility Revision ID in IOM
  //
  if (pInst->Config->CridEnable == TRUE) {
    Status = IpIomCrIdProgram (pInst->IomInst);
    TCSS_RETURN_IF_ERROR (Status);
  }

  return IpCsiStsSuccess;
}

/**
  TCSS initialization procedure executed after memory is discovered.

  @param[in]  pInst             TCSS Instance

  @retval  IP_CSI_STATUS
    - IpCsiStsSuccess:          programming completed without error
    - IpCsiStsError:            an error during the programming
    - IpCsiStsErrorNotAllowed:  IOM is corrupted
    - IpCsiStsErrorNullPtr:     null pointer to IP Instance provided
**/
IP_CSI_STATUS
SsTcssInitPostMemory (
  TCSS_INST   *pInst
  )
{
  UINT32                        PmcReplay;
  UINT32                        PmcTcssInitStatus;
  IP_CSI_STATUS                 Status;
  UINT8                         MaxUsb3;
  UINT8                         Index;
  DEV_INIT_DONE                 Done;

  TCSS_API_ENTRY ();

  Done.Value  = 0;

  //
  //  IOM Post Memory initialization
  //
  Status = IpIomInitPostMemory (pInst->IomInst);
  if (Status != IpCsiStsSuccess) {
    if (Status == IpCsiStsErrorNotAllowed) {
      //
      // IOM is corrupted- abort TCSS initialization
      // All TCSS IPs in the SOC DEVEN register should be disabled
      //
      PRINT_ERROR ("%a: ERROR: IOM is corrupted! \n\tAbort TCSS initialization!\n", __FUNCTION__);
    }
    return Status;
  }

  //
  // Configure TCSS LSx_OE pins in SOC GPIO controller
  //
  if (pInst->Callbacks->LsxOePinsConfigure != NULL) {
    for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
      if (IS_TC_PORT_USB4_SUPPORTED (pInst->Config->PortEnData.CapPolicy[Index])) {
        //
        // The TcssLsxOe is bit map to per TCSS port. 1: USB4 20G retimerless port.
        // Enable corresponding LSx_OE GPIO to native function for USB4 20G retimerless port
        //
        if ((pInst->Config->LsxOe >> Index && BIT0) == 1) {
          pInst->Callbacks->LsxOePinsConfigure (Index);
        }
      }
    }
  }

  //
  //  PCIe Root Port - Function mapping programming
  //
  Status = IpTcPcieRpSetFunctionMap (pInst->PcieInst);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: [TCSS] Failed to program PCIe function map. Status: %r\n", __FUNCTION__, Status);
  }

  //
  //  PCIe Root Port - Function mapping programming in IOC
  //
  if (pInst->Callbacks->PcieSetFunctionMapInSoc != NULL) {
    pInst->Callbacks->PcieSetFunctionMapInSoc (pInst->PcieInst->PcieFuncMap);
  }

  //
  //  PCIe Root Port initialization
  //
  Status = IpTcPcieRpIpInit (pInst->PcieInst);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: [TCSS] Failed to initialize TCSS PCIe root ports. Status: %r\n", __FUNCTION__, Status);
  } else {
    if (pInst->IomInst->IomConfig.DevEn.TcssDevEnBit.TcssPcieRpEn) {
      for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
        if (((pInst->PcieInst->DevEn.DevEn >> Index) & BIT0)) {
          Done.Dev.PcieRp = TRUE;
          break;
        }
      }
    }
  }

  //
  // Clear VGA and VGA16 decode for TC PCIe Root Ports
  //
  Status = IpTcPcieRpClearVgaRegisters (pInst->PcieInst);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: [TCSS] Failed to clear VGA registers. Status: %r\n", __FUNCTION__, Status);
  }

  //
  //  Lock PCIe RP configuration
  //
  Status = IpTcPcieRpLockConfiguration (pInst->PcieInst);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: [TCSS] Failed to lock PCIe RP configuration. Status: %r\n", __FUNCTION__, Status);
  }


  //
  //  Program SubVendor and SubSystem IDs for TCSS PCIE devices
  //
  if (pInst->Callbacks->SetSvidSsid != NULL) {
    pInst->Callbacks->SetSvidSsid ();
  }

  //
  //  USB3 xHCI initialization
  //
  Status = IpUsb3HostControllerFullInit (pInst->Usb3Inst);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: [TCSS] Failed to initialize USB3 xHCI. Status: %r\n", __FUNCTION__, Status);
  } else {
    if (pInst->IomInst->IomConfig.DevEn.TcssDevEnBit.TcssXhciEn) {
      Done.Dev.Xhci = TRUE;
    }
  }

  //
  //  USB4 initialization
  //
  if (IsUsb4HrEnabled (pInst) == TRUE) {
    if (pInst->Callbacks->EnablePciePmeEvents) {
      pInst->Callbacks->EnablePciePmeEvents ();
    }

    Status = IpUsb4IpInit (pInst->Usb4Inst);
    if (Status != IpCsiStsSuccess) {
      PRINT_ERROR ("%a: [TCSS] Failed to initialize USB4. Status: %r\n", __FUNCTION__, Status);
    } else {
      if (pInst->IomInst->IomConfig.DevEn.TcssDevEnBit.Usb4HrEn) {
        Done.Dev.Usb4 = TRUE;
      }
    }
  }

  //
  //  Set initialization complete indication for each controller
  //
  Status = IpIomSetDevInitDone (pInst->IomInst, Done);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: [TCSS] Failed to set initialization complete indication. Status: %r\n", __FUNCTION__, Status);
  }

  //
  // BIOS <-> PMC Handshake
  //
  if (pInst->Config->PmcPdEnable) {
    PmcReplay = TCSS_PMC_REPLAY_EN;
    if (pInst->Config->BootMode == BOOT_ON_S3_RESUME) {
      PmcReplay = 0;
    }
    if ((pInst->Config->BootMode == BOOT_ON_S4_RESUME) &&
      (pInst->Config->VccSt != 0)) {
        PmcReplay = 0;
    }

    PERF_INMODULE_BEGIN ("PmcTcssBiosInitDone");
    if (pInst->Callbacks->PmcTcssBiosInitDone != NULL) {
      PmcTcssInitStatus = (UINT32) pInst->Callbacks->PmcTcssBiosInitDone (pInst->Info->IOMReady, PmcReplay);
      PRINT_INFO ("%a: [TCSS] PmcTcssInitStatus = %r\n", __FUNCTION__, PmcTcssInitStatus);
    }
    PERF_INMODULE_END ("PmcTcssBiosInitDone");

    (void) PmcTcssInitStatus;  // to avoid GCC warning
  }

  //
  //  PMC configuration - USB Type C to A conversion
  //
  if (pInst->Config->MaxTcssUsb3Ports > MAX_TCSS_USB3_PORTS) {
    MaxUsb3 = MAX_TCSS_USB3_PORTS;
    PRINT_ERROR ("%a: [TCSS] Wrong USB3 ports quantity\n", __FUNCTION__);
  } else {
    MaxUsb3 = pInst->Config->MaxTcssUsb3Ports;
  }
  for (Index = 0; Index < MaxUsb3; Index++) {
    if ((pInst->Config->TcssConvUsbA[Index].Field.Enable == 0) ||
        (pInst->Config->TcssConvUsbA[Index].Field.MappingPchXhciUsb2 == 0)) {
      continue;
    }
    if (pInst->Callbacks->ConvertUsbCToUsbA != NULL) {
      pInst->Callbacks->ConvertUsbCToUsbA (
        Index + 1,
        pInst->Config->TcssConvUsbA[Index].Field.MappingPchXhciUsb2,
        pInst->Config->TcssConvUsbA[Index].Field.Orientation
        );
    }
  }
  return IpCsiStsSuccess;
}

/**
  TCSS initialization procedure executed at the end of PEI phase.

  @param[in]  pInst             TCSS Instance

  @retval  IP_CSI_STATUS
    - IpCsiStsSuccess:        programming completed without error
    - IpCsiStsError:          an error during the programming
    - IpCsiStsNotReady:       TBT or MG firmware not loaded to IMR
    - IpCsiStsErrorNullPtr:   null pointer to IP Instance provided
**/
IP_CSI_STATUS
SsTcssInitEndOfPei (
  TCSS_INST   *pInst
  )
{
  IP_CSI_STATUS      Status;
  IOM_MG_IMR_STATUS  Imr;

  TCSS_API_ENTRY ();

  Status = IpIomGetImrMgStatus (pInst->IomInst, &Imr.RegValue);
  if (Status != IpCsiStsSuccess) {
    if (Status == IpCsiStsNotReady) {
      PRINT_ERROR ("%a: ERROR: MG firmvare not loaded to IMR! \n\tAbort TCSS initialization!\n", __FUNCTION__);
    }
    IpIomConfigurationLock (pInst->IomInst);
    return Status;
  }
  pInst->Info->MgImrStatus.RegValue = Imr.RegValue;

  Status = IpIomGetImrTbtStatus (pInst->IomInst, &Imr.RegValue);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: ERROR: TBT firmware not loaded to IMR! \n\tAbort TCSS initialization!\n", __FUNCTION__);
    IpIomConfigurationLock (pInst->IomInst);
    return Status;
  }



  Status = IpIomConfigurationLock (pInst->IomInst);
  TCSS_RETURN_IF_ERROR (Status);

  return IpCsiStsSuccess;
}

/**
  Returns TCSS informations

  @param[in]  pInst             TCSS Instance

  @retval  IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed without error
    - IpCsiStsError:         Returned data can de invalid or not complete
    - IpCsiStsErrorNullPtr:  null pointer to IP Instance or Info provided
**/
IP_CSI_STATUS
SsTcssGetInfo (
  TCSS_INST   *pInst
  )
{
  IP_CSI_STATUS       Status;
  UINT32              Data32;
  IP_IOM_CONFIG_LOCK  IomConfigLockData;
  UINT8               SkuId;
  UINT8               BaseSteppingId;
  UINT8               DashSteppingId;
  TCSS_API_ENTRY ();

  if (pInst->Info == NULL) {
    PRINT_ERROR ("%a: ERROR: TCSS Info NULL parameter!\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  Status = IpIomIsReady (pInst->IomInst, &Data32);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: IpIomIsReady error, Status = %r\n", __FUNCTION__, Status);
  }
  pInst->Info->IOMReady = Data32;

  Status = IpIomGetTcssProductInfo (pInst->IomInst, &SkuId, &BaseSteppingId, &DashSteppingId);
  if (Status == IpCsiStsSuccess) {
    PRINT_INFO ("%a: TCSS SoC SkuId = 0x%02X, BaseSteppingId = 0x%02X, DashSteppingId = 0x%02X\n", __FUNCTION__, SkuId, BaseSteppingId, DashSteppingId);
  } else {
    PRINT_INFO ("%a: Cannot get the TCSS SoC product information, Status = %r\n", __FUNCTION__, Status);
  }

  Status = IpIomGetFwVersion (pInst->IomInst, &Data32);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: IpIomGetFwVersion error, Status = %r\n", __FUNCTION__, Status);
  }
  pInst->Info->IomFwVersion = Data32;

  Status = IpIomGetFwEngrVersion (pInst->IomInst, &Data32);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: IpIomGetFwEngrVersion error, Status = %r\n", __FUNCTION__, Status);
  }
  pInst->Info->IomFwEngrVersion = Data32;

  Status = IpIomGetImrTbtStatus (pInst->IomInst, &Data32);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: IpIomGetImrTbtStatus error, Status = %r\n", __FUNCTION__, Status);
  }
  pInst->Info->TbtImrStatus.RegValue = Data32;

  Status = IpTcPcieRpGetFunctionMap (pInst->PcieInst, &Data32);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: IpTcPcieRpGetFunctionMap error, Status = %r\n", __FUNCTION__, Status);
  }
  pInst->Info->ItbtPcieFuncMap = Data32;

  Status = IpIomGetImrMgStatus (pInst->IomInst, &Data32);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: IpIomGetImrMgStatus error, Status = %r\n", __FUNCTION__, Status);
  }
  pInst->Info->MgImrStatus.RegValue = Data32;

  Status = IpIomGetSwConfigs (pInst->IomInst, &IomConfigLockData);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: IpIomGetSwConfigs error, Status = %r\n", __FUNCTION__, Status);
  }
  pInst->Info->SwConfiguration1Lock = IomConfigLockData.Lock1;
  pInst->Info->SwConfiguration3Lock = IomConfigLockData.Lock3;
  pInst->Info->SwConfiguration4Lock = IomConfigLockData.Lock4;
  pInst->Info->MiscConfugurationLock = IomConfigLockData.Lock5;

  Status = IpIomGetDeepStTcState (pInst->IomInst, &Data32);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("%a: IpIomGetDeepStTcState error, Status = %r\n", __FUNCTION__, Status);
  }
  pInst->Info->DeepStTcState = Data32;

  return IpCsiStsSuccess;
}
