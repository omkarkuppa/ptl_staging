/** @file
  USB3 IP initialization library

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

#include <IpCsi.h>
#include <IpWrapperBaseTypes.h>
#include <IpWrapper64Bit.h>

#include <IpUsb3.h>
#include <IpUsb3VscId.h>
#include <IpUsb3Defines.h>
#include "IpUsb3Version.h"
#include <IpUsb3Private.h>
#include <Register/UsbHostRegs.h>

#define USB_OC_MAX_PINS 8
#define USB_OC_SKIP     0xFF

/**
  Enables USB3 ports by enabling SuperSpeed terminations

  @param[in]  pInst             USB3 IP Instance
  @param[in]  EnablePortsMask   Bitmask of enabled USB3 ports

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3EnableUsb3Ports (
  IP_USB3_INST  *pInst,
  UINT32        EnablePortsMask
  )
{
  UINT32   Mask;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  Mask = ((0x1 << pInst->Usb3LanesCount) - 1);

  IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_SSPE_REG, ~(Mask), (EnablePortsMask & Mask));

  return IpCsiStsSuccess;
}

/**
  This goes through general USB3 controller initialization programming
  requirements described in USB3 Programming guide.

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3HostControllerInit (
  IP_USB3_INST *pInst
  )
{
  UINT32  EnabledPortsMask;
  UINT32  LaneIndex;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  if (pInst->Usb3LanesCount > 0) {
    IpUsb3SetControl (pInst, 0, IpUsb3FeatIdUsb3ProgrammingLock, 0);

    EnabledPortsMask = 0;
    for (LaneIndex = 0; LaneIndex < pInst->Usb3LanesCount; LaneIndex++) {
      if (pInst->Usb3PortConfig[LaneIndex].Enable == TRUE) {
        if (pInst->cIsUsbOwningLane != NULL) {
          if (pInst->cIsUsbOwningLane (pInst, LaneIndex) == FALSE) {
            continue;
          }
        }
        EnabledPortsMask |= (0x1 << LaneIndex);
      }
    }
    IpUsb3EnableUsb3Ports(pInst, EnabledPortsMask);
  }

  IpUsb3SetControl (pInst, 0, IpUsb3FeatIdResetPrepOverride, IpUsb3FeatValResetPrepOverrideEn);

  if ( (pInst->ExternalPowerControl == FALSE) && (pInst->SkipD3HE == FALSE))
  {
    IpUsb3SetControl (pInst, 0, IpUsb3FeatIdPowerControl, IpUsb3FeatValPowerControlEn);
  }

  if ((pInst->Integration != IpUsb3IntegrationUsb4ss) && (!pInst->IsDciEnabled)) {
    IpUsb3ChangeUsb2DbcPortsToFunctional (pInst);
  }

  IpUsb3SetControl (pInst, 0, IpUsb3FeatIdAccessControlLock, 0);
  return IpCsiStsSuccess;
}

/**
  Programs interrupt pin in xHCI controller

  @param[in]  pInst         USB3 IP Instance
  @param[in]  IntPin        Interrupt Pin

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorBadParam   IntPin value must be between 0 and 3
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3InterruptMapping (
  IP_USB3_INST  *pInst,
  UINT8         IntPin
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  if (IntPin > 3) {
    PRINT_ERROR ("Interrupt Pin accepted values range is between 0 and 3 (inclusive)\n");
    return IpCsiStsErrorBadParam;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  IP_WR_WRITE_8 (pInst->RegCntxtPci, R_XHCI_CFG_IPIN, IntPin);

  return IpCsiStsSuccess;
}

/**
  Programs Over Current registers based on data present in IP Instance

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3OvercurrentMapping (
  IP_USB3_INST  *pInst
  )
{
  UINT8    Index;
  UINT32   U2OcmBuf[USB_OC_MAX_PINS] = {0};
  UINT32   U3OcmBuf[USB_OC_MAX_PINS] = {0};
  UINT32   OcPin;
  UINT32   OcmRegIndex;
  UINT32   OcPinsUsedMask;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  if (pInst->OverCurrentEnable == FALSE) {
    for (Index = 0; Index < USB_OC_MAX_PINS; Index++) {
      IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_U2OCM1 + (Index * 4), 0);
      IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_U3OCM1 + (Index * 4), 0);
    }

    if (pInst->OverCurrentLock == TRUE) {
      IP_WR_OR_32 (pInst->RegCntxtPci, R_XHCI_CFG_XHCC2, B_XHCI_CFG_XHCC2_OCCFGDONE);
    }
    return IpCsiStsSuccess;
  }

  OcPinsUsedMask = 0;

  for (Index = 0; Index < pInst->Usb2PortCount; Index++) {
    if (pInst->Usb2PortConfig[Index].OverCurrentPin != USB_OC_SKIP) {
      OcPin = pInst->Usb2PortConfig[Index].OverCurrentPin;
      U2OcmBuf[OcPin] |= (UINT32) (0x1 << Index);
      OcPinsUsedMask |= (UINT32) (0x1 << OcPin);
    }
  }

  for (Index = 0; Index < pInst->Usb3LanesCount; Index++) {
    if (pInst->Usb3PortConfig[Index].OverCurrentPin != USB_OC_SKIP) {
      OcPin = pInst->Usb3PortConfig[Index].OverCurrentPin;
      OcPinsUsedMask |= (UINT32) (0x1 << OcPin);
      OcmRegIndex = OcPin;
      if (pInst->cCalculateOcPin != NULL) {
        OcmRegIndex = pInst->cCalculateOcPin (pInst, OcPin);
        PRINT_LEVEL1 (
          "%s - recalculated OverCurrent pin %d for USB3 port %d to OCM register index %d\n",
          __FUNCTION__, OcPin, Index, OcmRegIndex
          );
      }
      U3OcmBuf[OcmRegIndex] |= (UINT32) (BIT0 << Index);
    }
  }

  for (Index = 0; Index < USB_OC_MAX_PINS; Index++) {
    IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_U2OCM1 + (Index * 4), U2OcmBuf[Index]);
    IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_U3OCM1 + (Index * 4), U3OcmBuf[Index]);
  }

  if ((OcPinsUsedMask != 0)
   && (pInst->cEnableOvercurrentPin != NULL))
  {
    for (Index = 0; Index < USB_OC_MAX_PINS; Index++) {
      if ((OcPinsUsedMask >> Index) & 0x1) {
        pInst->cEnableOvercurrentPin (pInst, Index);
      }
    }
  }

  if (pInst->OverCurrentLock == TRUE) {
    IP_WR_OR_32 (pInst->RegCntxtPci, R_XHCI_CFG_XHCC2, B_XHCI_CFG_XHCC2_OCCFGDONE);
  }
  return IpCsiStsSuccess;
}

/**
  Programs latency related parameters in xHCI.
  Based on provided parameters will either enable NDE support with side band or PMREQ
  or program LTR related values based on data in IP instance.

  @param[in]  pInst         USB3 IP Instance
  @param[in]  LtrEnable     Flag whether to use LTR or NDE
  @param[in]  NdeSbEnable   Flag whether to use side band or PMREQ for NDE
  @param[in]  HsiiEnable    Flag whether to enable HS Interrupt IN alarm

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3LatencyTuning (
  IP_USB3_INST  *pInst,
  BOOLEAN       LtrEnable,
  BOOLEAN       NdeSbEnable,
  BOOLEAN       HsiiEnable
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  if (LtrEnable) {
    IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_XLTP_HITC, pInst->LtrHighIdleTime);
    IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_XLTP_MITC, pInst->LtrMedIdleTime);
    IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_XLTP_LITC, pInst->LtrLowIdleTime);

    IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_PMREQ_CTRL_REG, B_XHCI_MEM_PMREQ_CTRL_REG_PWRSCH_IT_IN_LTR);
  } else {
    if (NdeSbEnable) {
      IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_PWR_SCHED_CTRL2, B_XHCI_MEM_PWR_SCHED_CTRL2_NDE_SBMSG_EN);
    } else {
      IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_PWR_SCHED_CTRL2, ~(B_XHCI_MEM_PWR_SCHED_CTRL2_NDE_SBMSG_EN));
    }
  }

  IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_XLTP_LTV2, ~(B_XHCI_MEM_XLTP_LTV2_LTV_LMT), pInst->LtvLimit);

  if (HsiiEnable) {
    IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_PWR_SCHED_CTRL2, B_XHCI_MEM_PWR_SCHED_CTRL2_HS_INT_IN_ALRM);
  }

  return IpCsiStsSuccess;
}

/**
  Enables function disable functionality in xHCI

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3FunctionDisable (
  IP_USB3_INST  *pInst
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  IP_WR_OR_32 (pInst->RegCntxtPci, R_XHCI_CFG_XHCC3, B_XHCI_CFG_XHCC3_FXN_DISABLE);

  return IpCsiStsSuccess;
}

/**
  Disables USB2 ports using Port Disable Override feature

  @param[in]  pInst               USB3 IP Instance
  @param[in]  DisabledPortsMask   Bitmask of disabled ports
  @param[in]  PortCount           Number of USB2 port count

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsError           actual register values differ from expected ones
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3DisableUsb2Ports (
  IP_USB3_INST  *pInst,
  UINT32        DisabledPortsMask,
  UINT8         PortCount
  )
{
  UINT32   Data;
  UINT32   ReadBackValue;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  Data = (DisabledPortsMask & ((0x1 << PortCount) - 1));
  IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_USB2PDO, Data);

  ReadBackValue = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_USB2PDO);

  if (ReadBackValue != Data) {
    IpWrPrint (pInst->PrintWarning, "USB2 Port Disabling may not have worked. Unexpected value");
    return IpCsiStsError;
  }

  return IpCsiStsSuccess;
}

/**
  Disables USB3 ports using Port Disable Override feature

  @param[in]  pInst               USB3 IP Instance
  @param[in]  DisabledPortsMask   Bitmask of disabled ports
  @param[in]  PortCount           Number of USB3 port count

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsError           actual register values differ from expected ones
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3DisableUsb3Ports (
  IP_USB3_INST  *pInst,
  UINT32        DisabledPortsMask,
  UINT8         PortCount
  )
{
  UINT32   Data;
  UINT32   ReadBackValue;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  Data = (DisabledPortsMask & ((0x1 << PortCount) - 1));
  IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_USB3PDO, Data);

  ReadBackValue = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_USB3PDO);

  if (ReadBackValue != Data) {
    IpWrPrint (pInst->PrintWarning, "USB3 Port Disabling may not have worked. Unexpected value");
    return IpCsiStsError;
  }

  return IpCsiStsSuccess;
}

/**
  Shall go through SOC specific programming flows and requirements described in
  USB3 programming guide.

  @param[in]  pInst                           USB3 IP Instance
  @param[in]  RequestBoundaryCrossingControl  Crossing boundary control
  @param[in]  MaxPayloadSize                  Max payload size

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
  @retval     IpCsiStsErrorBadParam   unsupported payload size provided
**/
IP_CSI_STATUS
IpUsb3FabricConfiguration (
  IP_USB3_INST  *pInst,
  UINT8         RequestBoundaryCrossingControl,
  UINT8         MaxPayloadSize
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  if ((RequestBoundaryCrossingControl != 0) && (RequestBoundaryCrossingControl != 1)) {
    PRINT_ERROR ("Invalid boundary crossing control parameter provided to %s\n", __FUNCTION__);
    return IpCsiStsErrorBadParam;
  }

  if ((MaxPayloadSize != 0) && (MaxPayloadSize != 1) && (MaxPayloadSize != 2) && (MaxPayloadSize != 7)) {
    PRINT_ERROR ("Invalid payload size parameter provided to %s\n", __FUNCTION__);
    return IpCsiStsErrorBadParam;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  IP_WR_AND_THEN_OR_32 (pInst->RegCntxtPci,
                      R_XHCI_CFG_XHCC2,
                      ~(B_XHCI_CFG_XHCC2_DREQBCC),
                      ((RequestBoundaryCrossingControl << N_XHCI_CFG_XHCC2_DREQBCC) & B_XHCI_CFG_XHCC2_DREQBCC));

  IP_WR_AND_THEN_OR_32 (pInst->RegCntxtPci,
                      R_XHCI_CFG_XHCC2,
                      ~(B_XHCI_CFG_XHCC2_WRREQSZCTRL),
                      ((MaxPayloadSize << N_XHCI_CFG_XHCC2_WRREQSZCTRL) & B_XHCI_CFG_XHCC2_WRREQSZCTRL));

  IP_WR_AND_THEN_OR_32 (pInst->RegCntxtPci,
                      R_XHCI_CFG_XHCC2,
                      ~(B_XHCI_CFG_XHCC2_RDREQSZCTRL),
                      (MaxPayloadSize & B_XHCI_CFG_XHCC2_RDREQSZCTRL));

  if ((pInst->Integration != IpUsb3IntegrationUsb4ss) && (pInst->UsbAudioOffloadEnable)) {
    IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem,
                        R_XHCI_MEM_ADO_CONFIG_REG1,
                        ~(B_XHCI_MEM_ADO_CONFIG_REG1_ADO_MAX_PYLD_SIZE),
                        ((MaxPayloadSize << N_XHCI_MEM_ADO_CONFIG_REG1_ADO_MAX_PYLD_SIZE) & B_XHCI_MEM_ADO_CONFIG_REG1_ADO_MAX_PYLD_SIZE));
  }

  return IpCsiStsSuccess;
}

/**
  Shall go through SOC specific programming flows and requirements described in
  USB3 programming guide.

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
  @retval     IpCsiStsErrorBadParam   one of the configuration parameters in IP Instance was incorrect
  @retval     IpCsiStsError           actual register values differ from expected ones
**/
IP_CSI_STATUS
IpUsb3SocConfiguration (
  IP_USB3_INST *pInst
  )
{
  UINT8         Index;
  UINT32        PortResetMessagingMask;
  UINT32        DisabledPortsMask;
  IP_CSI_STATUS Status;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  Status = IpUsb3InterruptMapping (pInst, pInst->InterruptPin);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("Couldn't programm interrupt pin.\n");
    return Status;
  }

  IpUsb3OvercurrentMapping (pInst);

  Status = IpUsb3FabricConfiguration (pInst, pInst->RequestBoundaryCrossingControl, pInst->MaxPayloadSize);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("Error during fabric configuration");
    return Status;
  }

  if (pInst->Integration != IpUsb3IntegrationUsb4ss) {
    Status = IpUsb3SetControl (pInst, 0, IpUsb3FeatIdAudioOffload, pInst->UsbAudioOffloadEnable);
    if (Status != IpCsiStsSuccess) {
      PRINT_ERROR ("Couldn't enable USB Audio Offload.\n");\
      return Status;
    }
  }

  IpUsb3LatencyTuning (pInst, pInst->LtrEnable, pInst->NdeSbEnable, pInst->HsiiEnable);

  PortResetMessagingMask = 0;
  for (Index = 0; Index < pInst->Usb2PortCount; Index++) {
    if (pInst->Usb2PortConfig[Index].PortResetMessageEnable == TRUE) {
      PortResetMessagingMask |= (0x1 << Index);
    }
  }
  Status = IpUsb3SetControl (pInst, 0, IpUsb3FeatIdPortResetMessaging, PortResetMessagingMask);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("Couldn't program port reset messaging.\n");
    return Status;
  }

  if (pInst->DisableHostController == TRUE) {
    IpUsb3FunctionDisable (pInst);
  }

  if (pInst->PdoProgramming == TRUE) {
    DisabledPortsMask = 0;
    for (Index = 0; Index < pInst->Usb2PortCount; Index++) {
      if (pInst->Usb2PortConfig[Index].Enable == FALSE) {
        DisabledPortsMask |= (0x1 << Index);
      }
    }
    Status = IpUsb3DisableUsb2Ports (pInst, DisabledPortsMask, pInst->Usb2PortCount);
    if (Status != IpCsiStsSuccess) {
      PRINT_ERROR ("Error occured while disabling USB2 ports.\n");
      return Status;
    }

    DisabledPortsMask = 0;
    for (Index = 0; Index < pInst->Usb3LanesCount; Index++) {
      if (pInst->Usb3PortConfig[Index].Enable == FALSE) {
        DisabledPortsMask |= (0x1 << Index);
      }
    }
    Status = IpUsb3DisableUsb3Ports (pInst, DisabledPortsMask, pInst->Usb3LanesCount);
    if (Status != IpCsiStsSuccess) {
      PRINT_ERROR ("Error occured while disabling USB3 ports.\n");
      return Status;
    }
  }

  return IpCsiStsSuccess;
}

/**
  Shall go through platform specific programming flows and requirements described in
  USB3 programming guide.

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3PlatformConfiguration (
  IP_USB3_INST *pInst
  )
{
  UINT8    Index;
  UINT32   InboxDevicesMap;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  InboxDevicesMap = 0;
  for (Index = 0; Index < pInst->Usb2PortCount; Index++) {
    if (pInst->Usb2PortConfig[Index].HasInboxDevice == TRUE) {
      InboxDevicesMap |= (0x1 << Index);
    }
  }
  IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_PDDIS_REG, InboxDevicesMap);

  InboxDevicesMap = 0;
  for (Index = 0; Index < pInst->Usb3LanesCount; Index++) {
    if (pInst->Usb3PortConfig[Index].HasInboxDevice == TRUE) {
      InboxDevicesMap |= (0x1 << Index);
    }
  }
  IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_LFPS_PM_CTRL_REG, InboxDevicesMap);

  return IpCsiStsSuccess;
}

/**
  Enables MMIO access to xHCI registers by programming MBAR value in xHCI PCI
  config space and enabling Memory Space Enable bit in PCI.CMD

  @param[in]  pInst         USB3 IP Instance
  @param[in]  Address       MBAR address value

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsError           cannot program Memory Space Enable bit
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3MmioEnable (
  IP_USB3_INST  *pInst,
  UINT64        Address
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  IP_WR_AND_16 (pInst->RegCntxtPci, R_XHCI_CFG_CMD, ~(B_XHCI_CFG_CMD_MSE));

  if (IP_WR_READ_32 (pInst->RegCntxtPci, R_XHCI_CFG_MBAR_N0) & BIT2) {
    IP_WR_WRITE_32 (pInst->RegCntxtPci, R_XHCI_CFG_MBAR_N1, (UINT32)IpWrRShiftU64 (Address, 32));
  }
  IP_WR_WRITE_32 (pInst->RegCntxtPci, R_XHCI_CFG_MBAR_N0, (UINT32)Address);

  IP_WR_OR_16 (pInst->RegCntxtPci, R_XHCI_CFG_CMD, B_XHCI_CFG_CMD_MSE);

  if ((IP_WR_READ_16 (pInst->RegCntxtPci, R_XHCI_CFG_CMD) & B_XHCI_CFG_CMD_MSE) == 0) {
    IpWrPrint (pInst->PrintError, "Cannot program xHCI Memory Space Enable bit\n");
    IpWrError (pInst->ErrorCntxt, IpWrErrorMajorError, __LINE__);
    return IpCsiStsError;
  }

  return IpCsiStsSuccess;
}

/**
  Disables MMIO access to xHCI registers by clearing Memory Space Enable bit in
  PCI.CMD register and clearing MBAR register in xHCI PCI config space.

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3MmioDisable (
  IP_USB3_INST  *pInst
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  IP_WR_AND_16 (pInst->RegCntxtPci, R_XHCI_CFG_CMD, ~(B_XHCI_CFG_CMD_MSE));

  if (IP_WR_READ_32 (pInst->RegCntxtPci, R_XHCI_CFG_MBAR_N0) & BIT2) {
    IP_WR_WRITE_32 (pInst->RegCntxtPci, R_XHCI_CFG_MBAR_N1, 0);
  }
  IP_WR_WRITE_32 (pInst->RegCntxtPci, R_XHCI_CFG_MBAR_N0, 0);

  return IpCsiStsSuccess;
}

/**
  This interface will apply the feature values provided by using SetControl interface.

  @param[in]  pInst         USB3 IP Instance
  @param[in]  pFeatureList  List of Parameter Ids and Parameter Values to be applied.
  @param[in]  EntryCount    The number of feature list entries in the pFeatureList.

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
  @retval     IpCsiStsError           call to IpUsb3SetControl returned an error
  @retval     IpCsiStsErrorUnknownId  bad IP_USB3_FEATURE_LIST_ENTRY
**/
IP_CSI_STATUS
IpUsb3ApplyFeatureList (
  IP_USB3_INST                *pInst,
  IP_USB3_FEATURE_LIST_ENTRY  *pFeatureList,
  UINT32                      EntryCount
  )
{
  IP_CSI_STATUS   Status;
  UINT32          Index;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  if (pFeatureList == NULL) {
    PRINT_ERROR ("You must provide feature list to %s\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  for (Index = 0; Index < EntryCount; Index++) {
    Status = IpUsb3SetControl (pInst, 0, pFeatureList[Index].FeatId, pFeatureList[Index].FeatVal);
    if (Status != IpCsiStsSuccess) {
      PRINT_ERROR (
        "%s - error returned from IpUsb3SetControl with status: %x\n", __FUNCTION__, Status
      );
      return IpCsiStsError;
    }
  }

  return IpCsiStsSuccess;
}

/**
  Performs full Host Controller initialization based on USB3 IP Instance data

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsError           error occured during execution
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3HostControllerFullInit (
  IP_USB3_INST  *pInst
  )
{
  IP_CSI_STATUS Status;
  IP_CSI_STATUS ExitStatus;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  // Enable MMIO access before any register programming
  Status = IpUsb3MmioEnable (pInst, pInst->MmioBaseAddress);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("Cannot open memory space for xHCI controller initialization. Aborting!\n");
    goto exit_init;
  }

  // Calling IpInit() so we apply version specific programming first
  Status = IpUsb3IpInit (pInst);
  if (Status != IpCsiStsSuccess) {
    PRINT_WARNING ("Error during IpUsb3IpInit\n");
    goto exit_init;
  }

  // Applying tunable parameters based on tuning list
  Status = IpUsb3ApplyTuningList (pInst, pInst->TuningList, pInst->TuningListEntryCount);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("Error during IpUsb3ApplyTuningList\n");
    goto exit_init;
  }

  // SoC specific configuration
  Status = IpUsb3SocConfiguration (pInst);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("Error during IpUsb3SocConfiguration\n");
    goto exit_init;
  }

  // Platform specific configuration (inbox devices)
  Status = IpUsb3PlatformConfiguration (pInst);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("Error during IpUsb3PlatformConfiguration\n");
    goto exit_init;
  }

  Status = IpUsb3ApplyFeatureList (pInst, pInst->FeatureList, pInst->FeatureListEntryCount);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("Error during IpUsb3ApplyFeatureList\n");
    goto exit_init;
  }

  // Host Controller specific programming along with setting lock bits
  Status = IpUsb3HostControllerInit (pInst);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("Error during IpUsb3HostControllerInit\n");
    goto exit_init;
  }

exit_init:
  ExitStatus = Status;

  // Disable MMIO access since BIOS is done with controller init
  Status = IpUsb3MmioDisable (pInst);
  if (Status != IpCsiStsSuccess) {
    PRINT_ERROR ("Error during IpUsb3MmioDisable\n");
    ExitStatus = Status;
  }

  if (ExitStatus != IpCsiStsSuccess) {
    return IpCsiStsError;
  }
  return IpCsiStsSuccess;
}

/**
  Goes through all USB2 ports reported by controller and switches them to functional
  ports from DbC

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3ChangeUsb2DbcPortsToFunctional (
  IP_USB3_INST  *pInst
  )
{
  UINT32            ReportedHsPortCount;
  UINT32            Index;
  UINT32            Offset;
  UINT32            Data32;
  IP_CSI_STATUS     Status;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  ReportedHsPortCount = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_XECP_SUPP_USB2_2);
  ReportedHsPortCount &= B_XHCI_MEM_XECP_SUPP_USB2_2_CPC;
  ReportedHsPortCount >>= N_XHCI_MEM_XECP_SUPP_USB2_2_CPC;

  PRINT_LEVEL1 ("%s - Reported count of HS ports: %d\n", __FUNCTION__, ReportedHsPortCount);

  for (Index = 0; Index < ReportedHsPortCount; Index++) {
    Offset = R_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0 + (Index * S_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_SPACING);

    Data32 = IP_WR_READ_32 (pInst->RegCntxtSbMmio, Offset);

    // if SPR value = 0x180, then do 0x180(dbc) -> 0x20(disconnect) -> 0x00(host)
    if ((Data32 & B_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_CONNECTOR_EVENT) ==
        V_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_CONNECTOR_EVENT_DBC) {
      PRINT_LEVEL1 ("%s - port %d used for DbC. Will switch to Host Subscription\n", __FUNCTION__, Index);

      Data32 &= (UINT32) ~(B_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_CONNECTOR_EVENT
                   | B_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_SW_VBUS);
      Data32 |= (UINT32) V_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_CONNECTOR_EVENT_DISCONNECT;
      IP_WR_WRITE_32 (pInst->RegCntxtSbMmio, Offset, Data32);
      PRINT_LEVEL1 ("%s - SPR[%08X] = %08X\n", __FUNCTION__, Offset, Data32);

      IpWrRegPollUs (
                     pInst->RegCntxtSbMmio,
                     R_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0 + (Index * S_XHCI_PCR_DAP_ESS_PORT_CONTROL1_SPACING),
                     IpWrRegFlagSize32Bits,
                     B_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0_OPERATION_STATE,
                     V_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0_OPERATION_STATE_DISCON,
                     10,
                     pInst->TimeCntxt,
                     10,
                     &Status
                     );
      if (Status != IpCsiStsSuccess) {
        PRINT_ERROR ("%s - port %d. Couldn't disconnect DBC subscription.\n", __FUNCTION__, Index);
        continue;
      }

      IP_WR_AND_THEN_OR_32 (
        pInst->RegCntxtSbMmio,
        Offset,
        ((UINT32) ~(B_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_CONNECTOR_EVENT
                   | B_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_SW_VBUS)),
        V_XHCI_PCR_DAP_USB2_PORT_CONTROL_0_REG_0_CONNECTOR_EVENT_HOST
        );

      IpWrRegPollUs (
                     pInst->RegCntxtSbMmio,
                     R_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0 + (Index * S_XHCI_PCR_DAP_ESS_PORT_CONTROL1_SPACING),
                     IpWrRegFlagSize32Bits,
                     B_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0_OPERATION_STATE,
                     V_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0_OPERATION_STATE_HOST,
                     10,
                     pInst->TimeCntxt,
                     10,
                     &Status
                     );
      if (Status != IpCsiStsSuccess) {
        PRINT_ERROR ("%s - port %d. Couldn't switch to host subscription.\n", __FUNCTION__, Index);
        continue;
      }

      Data32 = IP_WR_READ_32 (
                pInst->RegCntxtSbMmio,
                R_XHCI_PCR_DAP_USB2_PORT_STATUS_REG_0 + (Index * S_XHCI_PCR_DAP_ESS_PORT_CONTROL1_SPACING)
                );
      PRINT_LEVEL1 ("%s - SPR USB2 Port %d Status0 = %08X\n", __FUNCTION__, Index, Data32);
    }
  }

  return IpCsiStsSuccess;
}

/**
  Goes through all USB2 ports reported by the controller and switches to device mode
  based on configuration in IP_USB3_USB2_PORT_CONFIG

  @param[in]  pInst                   USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3UsbSwDeviceModeConfig (
  IP_USB3_INST  *pInst
  )
{
  UINT32  ReportedHsPortCount;
  UINT32  Index;
  UINT32  PortControl0Offset;
  UINT32  PortControl1Offset;
  UINT32  PortControl0Val;
  UINT32  PortControl1Val;
  UINT16  ConnectorType;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  ReportedHsPortCount = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_XECP_SUPP_USB2_2);
  ReportedHsPortCount &= B_XHCI_MEM_XECP_SUPP_USB2_2_CPC;
  ReportedHsPortCount >>= N_XHCI_MEM_XECP_SUPP_USB2_2_CPC;

  PRINT_LEVEL1 ("%s - Reported count of HS ports: %d\n", __FUNCTION__, ReportedHsPortCount);

  for (Index = 0; Index < ReportedHsPortCount; Index++) {
    PortControl0Offset = R_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_REG_0_MMIO + (Index * S_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_SPACING);
    PortControl1Offset = R_XHCI_MEM_DAP_USB2_PORT_CONTROL_1_REG_0_MMIO + (Index * S_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_SPACING);

    PortControl0Val = IP_WR_READ_32 (pInst->RegCntxtMem, PortControl0Offset);
    PortControl1Val = IP_WR_READ_32 (pInst->RegCntxtMem, PortControl1Offset);
    PRINT_LEVEL1 ("%s - Port %d DAP Control0 = %08X, Control1 = %08X\n",
      __FUNCTION__, Index, PortControl0Val, PortControl1Val);

    ConnectorType = PortControl1Val & B_XHCI_MEM_DAP_USB2_PORT_CONTROL_1_REG_0_MMIO_CONNECTOR_TYPE;
    if ((ConnectorType == V_XHCI_MEM_DAP_USB2_PORT_CONTROL_1_REG_0_MMIO_CONNECTOR_TYPE_AB)
      || (ConnectorType == V_XHCI_MEM_DAP_USB2_PORT_CONTROL_1_REG_0_MMIO_CONNECTOR_TYPE_C)) {
      if (pInst->Usb2PortConfig[Index].SwDeviceModeEnable == TRUE) {
        PRINT_LEVEL1 ("%s - Port %d is Type AB/C and will be swtiched to device subscription\n", __FUNCTION__, Index);

        IP_WR_AND_THEN_OR_32 (
          pInst->RegCntxtMem,
          PortControl0Offset,
          ((UINT32) ~B_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_REG_0_MMIO_CONNECTOR_EVENT),
          (V_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_REG_0_MMIO_CONNECTOR_EVENT_DEV_SUBSCRIPTION |
           B_XHCI_MEM_DAP_USB2_PORT_CONTROL_0_REG_0_MMIO_SW_VBUS)
          );
      }

      PortControl0Val = IP_WR_READ_32 (pInst->RegCntxtMem, PortControl0Offset);
      PortControl1Val = IP_WR_READ_32 (pInst->RegCntxtMem, PortControl1Offset);
      PRINT_LEVEL1 ("%s - Port %d DAP Control0 = %08X, Control1 = %08X\n",
        __FUNCTION__, Index, PortControl0Val, PortControl1Val);
    }
  }

  return IpCsiStsSuccess;
}