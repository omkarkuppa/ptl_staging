/** @file
  TBT3 Router specific implementation

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/Tbt3ConfigSpace.h>
#include <Library/Usb4RouterDefs.h>
#include <Library/Usb4CsLib.h>

/**
  Set DP TMU stable bit of TitanRidge DP OUT adapter.
  (TitanRidge only, refer to CM TBT3 Device support 5.1 TMU configuration)
  @param[in] Router - Pointer to router instance.
  @param[in] AdpNum - Adapter number.

  @retval EFI_SUCCESS           - Set DP TMU stable bit successfully.
  @retval EFI_UNSUPPORTED       - Fail to set DP TMU stable bit.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3TrSetTmuStable (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS           Status;
  PUSB4_ADAPTER        Adapter;
  TBT3_TMU_DP_CS_13    DpTmuCs13;

  DEBUG ((DEBUG_INFO, "Tbt3TrSetTmuStable entry - AdpNum = %d\n", AdpNum));
  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrSetTmuStable: invalid parameter.\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrSetTmuStable: Invalid AdpNum %d\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Adapter = &(Router->Adapter[AdpNum]);
  if ((Adapter->Type != USB4_ADP_TYPE_DP_OUT) || (Adapter->TmuCapReg == 0)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d is not DP OUT or null DP TMU cap\n", Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsAdpRead (Router, AdpNum, Adapter->TmuCapReg + TBT3_CS_13, 1, (UINT32 *) &DpTmuCs13);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d read DP TMU CS13 failure, Status = %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d DP TMU CS13.Stable = 0x%0X\n", Router->TopologyId, AdpNum, DpTmuCs13.Fields.TmuStable));
  DpTmuCs13.Fields.TmuStable = 0x80;
  Status = Usb4CsAdpWrite (Router, AdpNum, Adapter->TmuCapReg + TBT3_CS_13, 1, (UINT32 *) &DpTmuCs13);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d write DP TMU CS13 failure, Status = %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Tbt3TrSetTmuStable exit\n"));
  return Status;
}

/**
  Query TBT3 router VSEC6 capability

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Query TBT3 router VSEC6 capability successfully.
  @retval EFI_UNSUPPORTED       - Fail to query TBT3 router VSEC6 capability.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3QueryRtVsec6 (
  IN PUSB4_ROUTER    Router
  )
{
  EFI_STATUS                   Status;
  TBT3_RT_VSEC_6_CAP_STRUCT    Vsec6Cs2;
  UINT32                       Index;
  UINT16                       Vsec6Reg;
  UINT16                       LcBase;
  TBT3_RT_VSEC_6_CS_42         Vsec6Cs42;
  TBT3_RT_VSEC_6_PORT_ATTR     PortAttr;
  TBT3_RT_VSEC_6_LC_SX_CTRL    LcSxCtrl;

  DEBUG ((DEBUG_INFO, "Tbt3QueryRtVsec6 entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Tbt3QueryRtVsec6: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Vsec6Reg = Router->Vsec6Reg;
  if (Vsec6Reg == 0) {
    DEBUG ((DEBUG_ERROR, "Tbt3QueryRtVsec6: Null VSEC6 capability\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsRtRead (Router, Vsec6Reg + TBT3_RT_VSEC_6_CAP_STRUCT_OFFSET, 1, (UINT32 *) &Vsec6Cs2);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tbt3QueryRtVsec6: Read VSEC6 CS 2 failure, %r\n", Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "VSEC6: USB4 ports = %d, common region length = %d, port region length = %d\n",
          Vsec6Cs2.Fields.Usb4Ports,
          Vsec6Cs2.Fields.CommonRegionLen,
          Vsec6Cs2.Fields.Usb4PortRegionLen
          ));
  Router->Tbt3Vsec6.Usb4Ports       = (UINT8) Vsec6Cs2.Fields.Usb4Ports;
  Router->Tbt3Vsec6.CommonRegionLen = (UINT8) Vsec6Cs2.Fields.CommonRegionLen;
  Router->Tbt3Vsec6.PortRegionLen   = (UINT16) Vsec6Cs2.Fields.Usb4PortRegionLen;

  for (Index = 0; Index < Vsec6Cs2.Fields.Usb4Ports; Index++) {
    LcBase = Vsec6Reg + (UINT16) Vsec6Cs2.Fields.CommonRegionLen + (UINT16) (Vsec6Cs2.Fields.Usb4PortRegionLen * Index);
    Status = Usb4CsRtRead (Router, LcBase + TBT3_RT_VSEC_6_CS_42_OFFSET, 1, (UINT32 *) &Vsec6Cs42);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Tbt3QueryRtVsec6: Read VSEC6 CS 42 failure, %r\n", Status));
      goto Exit;
    }
    Status = Usb4CsRtRead (Router, LcBase + TBT3_RT_VSEC_6_PORT_ATTR_OFFSET, 1, (UINT32 *) &PortAttr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Tbt3QueryRtVsec6: Read VSEC6 PORT_ATTR (CS_141) failure, %r\n", Status));
      goto Exit;
    }
    Status = Usb4CsRtRead (Router, LcBase + TBT3_RT_VSEC_6_LC_SX_CTRL_OFFSET, 1, (UINT32 *) &LcSxCtrl);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Tbt3QueryRtVsec6: Read VSEC6 LC_SX_CTRL (CS_150) failure, %r\n", Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "VSEC6 port region %d :\n", Index));
    DEBUG ((DEBUG_INFO, "    CS_42              - UsbPlug = %d\n", Vsec6Cs42.Fields.UsbPlug));
    DEBUG ((DEBUG_INFO, "    PortAttr (CS_141)  - BondingEnable = %d, Rtd3Support = %d\n", PortAttr.Fields.BondingEnabled, PortAttr.Fields.Rtd3Support));
    DEBUG ((DEBUG_INFO, "    LC_SX_CTRL (CS_150)- WakeEvents=0x%0x, L0C=%d, L0D=%d, L1C=%d, L1D=%d, SLI=%d, SLP=%d\n",
            LcSxCtrl.Fields.EnableWakeEvts,
            LcSxCtrl.Fields.Ln0Configured,
            LcSxCtrl.Fields.Ln0InterDomain,
            LcSxCtrl.Fields.Ln1Configured,
            LcSxCtrl.Fields.Ln1InterDomain,
            LcSxCtrl.Fields.StartLinkInit,
            LcSxCtrl.Fields.EnterSleep
            ));
  }
Exit:
DEBUG ((DEBUG_INFO, "Tbt3QueryRtVsec6 exit\n"));
  return Status;
}

/**
  Set link configured bit of TBT3 LC_SX_CTRL.
  This function may be called before adapter enumeration.

  @param[in] Router - Pointer to router instance.
  @param[in] AdpNum - Adapter number.

  @retval EFI_SUCCESS           - Set link configured bit successful.
  @retval EFI_UNSUPPORTED       - Fail to set link configured bit.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3SetLinkConfigured (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS                   Status;
  UINT16                       LcBase;
  TBT3_RT_VSEC_6_LC_SX_CTRL    LcSxCtrl;

  DEBUG ((DEBUG_INFO, "Tbt3SetLinkConfigured entry - AdpNum = %d\n", AdpNum));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Tbt3SetLinkConfigured: invalid parameter.\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if ((Router->Vsec6Reg == 0) || (AdpNum == 0) || (AdpNum > Router->MaxAdapter)) {
    DEBUG ((DEBUG_ERROR, "Tbt3SetLinkConfigured: Null VSEC6 cap or invalid AdpNum %d\n", AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  LcBase = Router->Vsec6Reg + Router->Tbt3Vsec6.CommonRegionLen + Router->Tbt3Vsec6.PortRegionLen * ((AdpNum - 1) >> 1);
  Status = Usb4CsRtRead (Router, LcBase + TBT3_RT_VSEC_6_LC_SX_CTRL_OFFSET, 1, (UINT32 *) &LcSxCtrl);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tbt3SetLinkConfigured: Read LC_SX_CTRL failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "LC_SX_CTRL: Original L0C = %d, L1C = %d, Up = %d\n",
          LcSxCtrl.Fields.Ln0Configured,
          LcSxCtrl.Fields.Ln1Configured,
          LcSxCtrl.Fields.PortIsUpStream
          ));
  if (AdpNum & 1) {
    LcSxCtrl.Fields.Ln0Configured = 1;
  } else {
    LcSxCtrl.Fields.Ln1Configured = 1;
  }

  //
  // Set upstream port bit if AdpNum is upstream port.
  // DP out hot plug is not detected if this bit is not set.
  //
  if (AdpNum == Router->LaneUpAdpNum) {
    LcSxCtrl.Fields.PortIsUpStream = 1;
  }

  DEBUG ((DEBUG_INFO, "LC_SX_CTRL: Updated L0C = %d, L1C = %d, Up = %d\n",
          LcSxCtrl.Fields.Ln0Configured,
          LcSxCtrl.Fields.Ln1Configured,
          LcSxCtrl.Fields.PortIsUpStream
          ));
  Status = Usb4CsRtWrite (Router, LcBase + TBT3_RT_VSEC_6_LC_SX_CTRL_OFFSET, 1, (UINT32 *) &LcSxCtrl);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tbt3SetLinkConfigured: Write LC_SX_CTRL failure, %r\n", Status));
    goto Exit;
  }
Exit:
  DEBUG ((DEBUG_INFO, "Tbt3SetLinkConfigured exit\n"));
  return Status;
}

/**
  Set link configured bit of TBT3 VSEC6 LC_SX_CTRL for TBT3 adapter.
  (Refer to TBT3 CM guide 3.2.2)

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Set link configured successfully.
  @retval EFI_UNSUPPORTED       - Fail to set link configured.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3RtLinkConfigured (
  IN PUSB4_ROUTER    Router
  )
{
  EFI_STATUS      Status;
  PUSB4_ROUTER    ParentRt;
  UINT8           ParentAdpNum;

  Status = EFI_UNSUPPORTED;

  if (Router == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  ParentRt     = Router->ParentRt;
  ParentAdpNum = Router->ParentAdpNum;

  //
  // Set link configured is required only between router and parent router.
  //
  if (ParentRt == NULL) {
    goto Exit;
  }

  if (ParentRt->State.Tbt3 == 1) {
    DEBUG ((DEBUG_INFO, "Tbt3RtLinkConfigured: Parent Rt 0x%016llX Adp %d\n", ParentRt->TopologyId, ParentAdpNum));
    Status = Tbt3SetLinkConfigured (ParentRt, ParentAdpNum);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Fail to set Rt 0x%016llX Adp %d link configured, %r\n", ParentRt->TopologyId, ParentAdpNum, Status));
      goto Exit;
    }
  }

  if (Router->State.Tbt3 == 1) {
    DEBUG ((DEBUG_INFO, "Tbt3RtLinkConfigured: Rt 0x%016llX Adp %d\n", Router->TopologyId, Router->LaneUpAdpNum));
    Status = Tbt3SetLinkConfigured (Router, Router->LaneUpAdpNum);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Fail to set Rt 0x%016llX Adp %d link configured, %r\n", Router->TopologyId, Router->LaneUpAdpNum, Status));
      goto Exit;
    }
  }

Exit:
  return Status;
}

/**
  Set TBT3 VSEC6 CM bit and SLI bit for TBT3 router.
  (Refer to TBT3 CM guide 3.2.2)

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Set TBT3 CM bit and SLI bit successfully.
  @retval EFI_UNSUPPORTED       - Fail to set TBT3 CM bit and SLI bit.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3SetCmSli (
  IN PUSB4_ROUTER    Router
  )
{
  EFI_STATUS               Status;
  UINT8                    Index;
  UINT16                   LcBase;
  TBT3_RT_VSEC_6_LC_SX_CTRL    LcSxCtrl;
  TBT3_VSEC_6              *Vsec6;

  DEBUG ((DEBUG_INFO, "Tbt3SetCmSli entry\n"));

  Status = EFI_UNSUPPORTED;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Tbt3SetCmSli: Null router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (Router->Vsec6Reg == 0) {
    DEBUG ((DEBUG_ERROR, "Tbt3SetCmSli: Null VSEC6 capability\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Set CM bit and SLI bit of all USB4 port regions
  //
  for (Index = 0; Index < Router->Tbt3Vsec6.Usb4Ports; Index++) {
    Vsec6  = &(Router->Tbt3Vsec6);
    LcBase = Router->Vsec6Reg + Vsec6->CommonRegionLen + Vsec6->PortRegionLen * Index;
    Status = Usb4CsRtRead (Router, LcBase + TBT3_RT_VSEC_6_LC_SX_CTRL_OFFSET, 1, (UINT32 *) &LcSxCtrl);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Tbt3SetCmSli: Read VSEC6 LC_SX_CTRL (CS_150) failure, %r\n", Status));
      goto Exit;
    }

    LcSxCtrl.Fields.AlpineRidgeCm = 1;
    LcSxCtrl.Fields.IntelCm       = 1;
    LcSxCtrl.Fields.StartLinkInit = 1;

    Status = Usb4CsRtWrite (Router, LcBase + TBT3_RT_VSEC_6_LC_SX_CTRL_OFFSET, 1, (UINT32 *) &LcSxCtrl);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Tbt3SetCmSli: Read VSEC6 LC_SX_CTRL (CS_150) failure, %r\n", Status));
      goto Exit;
    }
  }

Exit:
  DEBUG ((DEBUG_INFO, "Tbt3SetCmSli exit\n"));
  return Status;
}

/**
  Enable AlpineRidge USB hot plug events
  (Refer to CM TBT3 Device Support 4.1.2 xHC enablement)

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Enable AlpineRidge USB hot plug events successfully.
  @retval EFI_UNSUPPORTED       - Fail to enable AlpineRidge USB hot plug events.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
*/
EFI_STATUS
Tbt3ArEnableUsbPlugEvents (
  IN PUSB4_ROUTER    Router
  )
{
  EFI_STATUS            Status;
  TBT3_RT_VSC_1_CS_1    Vsc1Cs1;

  DEBUG ((DEBUG_INFO, "Tbt3ArEnableUsbPlugEvents entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Tbt3ArEnableUsbPlugEvents: Null router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((Router->RouterType != USB4_ROUTER_ALPINE_RIDGE) || (Router->Vsc1Reg == 0)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX is not AlpineRidge or null VSC1 capability\n", Router->TopologyId));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsRtRead (Router, Router->Vsc1Reg + 1, 1, (UINT32 *) &Vsc1Cs1);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tbt3ArEnableUsbPlugEvents: Read Rt VSC1 CS_1 failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Original DisableUsbHp bit = %d\n", Vsc1Cs1.Fields.DisableUsbHp));
  Vsc1Cs1.Fields.DisableUsbHp = 0;
  Status = Usb4CsRtWrite (Router, Router->Vsc1Reg + 1, 1, (UINT32 *) &Vsc1Cs1);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tbt3ArEnableUsbPlugEvents: Write Rt VSC1 CS_1 failure, %r\n", Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Tbt3ArEnableUsbPlugEvents exit\n"));
  return Status;
}

/**
  Set xHC connect bit of all router VSEC6 port region for TitanRidge router.
  (Refer to CM TBT3 Device Support 4.1.2 xHC enablement)

  @param[in] Router  - Pointer to router instance.
  @param[in] Connect - 1: Connect xHC, 0: Disconnect xHC.

  @retval EFI_SUCCESS           - Set xHC connect bit successfully.
  @retval EFI_UNSUPPORTED       - Fail to set xHC connect bit.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
*/
EFI_STATUS
Tbt3TrConnectXhc (
  IN PUSB4_ROUTER    Router,
  IN BOOLEAN         Connect
  )
{
  EFI_STATUS                 Status;
  UINT8                      Index;
  UINT16                     LcBase;
  TBT3_RT_VSEC_6_LINK_REQ    LinkReq;
  TBT3_VSEC_6                *Vsec6;

  DEBUG ((DEBUG_INFO, "Tbt3TrConnectXhc entry - Connect = %d\n", Connect));

  Status = EFI_UNSUPPORTED;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrConnectXhc: Null router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if ((Router->RouterType != USB4_ROUTER_TITAN_RIDGE) || (Router->Vsec6Reg == 0)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX is not TitanRidge or null VSEC6 capability\n", Router->TopologyId));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Set xHC connect bit of all USB4 port regions
  //
  for (Index = 0; Index < Router->Tbt3Vsec6.Usb4Ports; Index++) {
    Vsec6  = &(Router->Tbt3Vsec6);
    LcBase = Router->Vsec6Reg + Vsec6->CommonRegionLen + Vsec6->PortRegionLen * Index;
    Status = Usb4CsRtRead (Router, LcBase + TBT3_RT_VSEC_6_LINK_REQ_OFFSET, 1, (UINT32 *) &LinkReq);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Tbt3TrConnectXhc: Read VSEC6 LINK_REQ (CS_173) failure, %r\n", Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "Port %d : Original xHC connect bit = %d\n", Index, LinkReq.Fields.ConnectXhc));
    LinkReq.Fields.ConnectXhc = (Connect) ? 1 : 0;
    Status = Usb4CsRtWrite (Router, LcBase + TBT3_RT_VSEC_6_LINK_REQ_OFFSET, 1, (UINT32 *) &LinkReq);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Tbt3TrConnectXhc: Write VSEC6 LINK_REQ (CS_173) failure, %r\n", Status));
      goto Exit;
    }
  }
Exit:
DEBUG ((DEBUG_INFO, "Tbt3TrConnectXhc exit\n"));
  return Status;
}

/**
  Set xHC connect bit based on VSEC6 USB plug bit fo all router VSEC6 port region of AlpineRidge router.
  (Refer to CM TBT3 Device Support 4.1.2 xHC enablement)

  @param[in] Router  - Pointer to router instance.
  @param[in] Connect - 1: Connect xHC, 0: Disconnect xHC.

  @retval EFI_SUCCESS           - Set xHC connect bit successfully.
  @retval EFI_UNSUPPORTED       - Fail to set xHC connect bit.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
*/
EFI_STATUS
Tbt3ArConnectXhc (
  IN PUSB4_ROUTER    Router,
  IN BOOLEAN         Connect
  )
{
  EFI_STATUS                 Status;
  UINT8                      Index;
  UINT16                     LcBase;
  TBT3_RT_VSEC_6_LINK_REQ    LinkReq;
  TBT3_RT_VSEC_6_CS_42       Vsec6Cs42;
  TBT3_VSEC_6                *Vsec6;

  DEBUG ((DEBUG_INFO, "Tbt3ArConnectXhc entry - Connect = %d\n", Connect));

  Status = EFI_UNSUPPORTED;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Tbt3ArConnectXhc: Null router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if ((Router->RouterType != USB4_ROUTER_ALPINE_RIDGE) || (Router->Vsec6Reg == 0)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX is not AlpineRidge or null VSEC6 capability\n", Router->TopologyId));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Set xHC connect bit of all USB4 port regions
  //
  for (Index = 0; Index < Router->Tbt3Vsec6.Usb4Ports; Index++) {
    Vsec6  = &(Router->Tbt3Vsec6);
    LcBase = Router->Vsec6Reg + Vsec6->CommonRegionLen + Vsec6->PortRegionLen * Index;
    Status = Usb4CsRtRead (Router, LcBase + TBT3_RT_VSEC_6_LINK_REQ_OFFSET, 1, (UINT32 *) &LinkReq);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Tbt3ArConnectXhc: Read VSEC6 LINK_REQ (CS_173) failure, %r\n", Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "Port %d : Original xHC connect bit = %d\n", Index, LinkReq.Fields.ConnectXhc));

    if (Connect) {
      Status = Usb4CsRtRead (Router, LcBase + TBT3_RT_VSEC_6_CS_42_OFFSET, 1, (UINT32 *) &Vsec6Cs42);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Tbt3ArConnectXhc: Read VSEC6 CS_42 failure, %r\n", Status));
        goto Exit;
      }
      if ((Vsec6Cs42.Fields.UsbPlug == 1) && (LinkReq.Fields.ConnectXhc == 0)) {
        LinkReq.Fields.ConnectXhc = 1;
        Status = Usb4CsRtWrite (Router, LcBase + TBT3_RT_VSEC_6_LINK_REQ_OFFSET, 1, (UINT32 *) &LinkReq);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Tbt3ArConnectXhc: Write VSEC6 LINK_REQ (CS_173) failure, %r\n", Status));
          goto Exit;
        }
      }
    } else {
      LinkReq.Fields.ConnectXhc = 0;
      Status = Usb4CsRtWrite (Router, LcBase + TBT3_RT_VSEC_6_LINK_REQ_OFFSET, 1, (UINT32 *) &LinkReq);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Tbt3ArConnectXhc: Write VSEC6 LINK_REQ (CS_173) failure, %r\n", Status));
        goto Exit;
      }
    }
  }

Exit:
  DEBUG ((DEBUG_INFO, "Tbt3ArConnectXhc exit\n"));
  return Status;
}

/**
  Check if lane bonding can be supported in TBT3 router.
  (Refer to CM TBT3 Device Support 6.1 Lane Bonding).

  @param[in] Router - Pointer to router instance.
  @param[in] AdpNum - Adapter number.

  @retval TRUE  - TBT3 lane bonding is supported.
  @retval FALSE - TBT3 lane bonding is not supported.
**/
BOOLEAN
IsTbt3LaneBondingSupported (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS                   Status;
  TBT3_VSEC_6                  *Vsec6;
  UINT8                        PortNum;
  UINT16                       Vsec6Reg;
  UINT16                       LcBase;
  TBT3_RT_VSEC_6_PORT_ATTR     PortAttr;
  BOOLEAN                      BondingEnabled;

  DEBUG ((DEBUG_INFO, "IsTbt3LaneBondingSupported entry\n"));

  BondingEnabled = FALSE;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "IsTbt3LaneBondingSupported: Null router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter)) {
    DEBUG ((DEBUG_ERROR, "IsTbt3LaneBondingSupported: Invalid adapter number %d\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Vsec6Reg = Router->Vsec6Reg;
  if (Vsec6Reg == 0) {
    DEBUG ((DEBUG_ERROR, "IsTbt3LaneBondingSupported: Null VSEC6 capability\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  Vsec6 = &(Router->Tbt3Vsec6);
  PortNum = (AdpNum - 1) >> 1;
  LcBase = Vsec6Reg + Vsec6->CommonRegionLen + Vsec6->PortRegionLen * PortNum;
  Status = Usb4CsRtRead (Router, LcBase + TBT3_RT_VSEC_6_PORT_ATTR_OFFSET, 1, (UINT32 *) &PortAttr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "IsTbt3LaneBondingSupported: Read VSEC6 PORT_ATTR (CS_141) failure, %r\n", Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "TBT3 PORT_ATTR bonding enabled bit = %d\n", PortAttr.Fields.BondingEnabled));
  if (PortAttr.Fields.BondingEnabled) {
    BondingEnabled = TRUE;
  }

Exit:
  DEBUG ((DEBUG_INFO, "IsTbt3LaneBondingSupported exit\n"));
  return BondingEnabled;
}

/**
  Read TBT3 DP IP register data for TitanRidge

  @param[in] Router  - Pointer to router instance.
  @param[in] AdpNum  - Adapter number.
  @param[in] Address - DP IP register address.
  @param[in] Data    - Pointer to the returned register data.

  @retval EFI_SUCCESS           - Read DP IP register data successfully.
  @retval EFI_UNSUPPORTED       - Fail to read DP IP register data.
  @retval EFI_TIMEOUT           - Fail to read DP IP register data within timeout period.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
TrDpIpRegRead (
  IN  PUSB4_ROUTER    Router,
  IN  UINT8           AdpNum,
  IN  UINT16          Address,
  OUT UINT32          *Data
  )
{
  EFI_STATUS              Status;
  PUSB4_ADAPTER           DpAdp;
  TBT3_DP_VSC_0_IP_CMD    IpCmd;
  TBT3_DP_VSC_0_IP_ADDR   IpAddr;
  UINT32                  Index;
  UINT32                  RegData;

  //
  // Check required parameters.
  //
  DEBUG ((DEBUG_INFO, "TrDpIpRegRead entry\n"));
  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Null router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Invalid AdpNum %d\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (Router->RouterType != USB4_ROUTER_TITAN_RIDGE) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Rt 0x%016llX is not TitanRidge!\n", Router->TopologyId));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  DpAdp = &(Router->Adapter[AdpNum]);
  if ((DpAdp->Type != USB4_ADP_TYPE_DP_IN) && (DpAdp->Type != USB4_ADP_TYPE_DP_OUT)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Rt 0x%016llX Adp %d is not DP adapter\n", Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  if (DpAdp->Vsc0Reg == 0) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Null VSC0 capability!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Set ID_ADDR.Address
  //
  Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_IP_ADDR_OFFSET, 1, (UINT32 *) &IpAddr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Read VSC0 IP_ADDR failure, %r\n", Status));
    goto Exit;
  }
  IpAddr.Fields.Addr = Address;
  Status = Usb4CsAdpWrite (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_IP_ADDR_OFFSET, 1, (UINT32 *) &IpAddr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Write VSC0 IP_ADDR failure, %r\n", Status));
    goto Exit;
  }

  //
  // Set IP_CMD.Cmd and IP_CMD.Valid
  //
  Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_IP_CMD_OFFSET, 1, (UINT32 *) &IpCmd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Read VSC0 IP_CMD failure, %r\n", Status));
    goto Exit;
  }
  IpCmd.Fields.Cmd   = TBT3_DP_IP_CMD_READ;
  IpCmd.Fields.Valid = 1;
  Status = Usb4CsAdpWrite (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_IP_CMD_OFFSET, 1, (UINT32 *) &IpCmd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Write VSC0 IP_CMD failure, %r\n", Status));
    goto Exit;
  }

  //
  // Poll until IP_CMD.Valid bit is cleared.
  //
  for (Index = 0; Index < TBT3_DP_IP_POLL_MAX; Index++) {
    Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_IP_CMD_OFFSET, 1, (UINT32 *) &IpCmd);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Read VSC0 IP_CMD failure, %r\n", Status));
      goto Exit;
    }
    if (IpCmd.Fields.Valid == 0) {
      break;
    }
    gBS->Stall (TBT3_DP_IP_POLL_US);
  }
  if (Index == TBT3_DP_IP_POLL_MAX) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Wait cmd valid bit cleared timeout!\n"));
    Status = EFI_TIMEOUT;
    goto Exit;
  }

  //
  // Read IP_RDATA.
  //
  Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_RDATA_OFFSET, 1, &RegData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegRead: Read VSC0 IP_RDATA failure, %r\n", Status));
    goto Exit;
  }

  *Data = RegData;

Exit:
  DEBUG ((DEBUG_INFO, "TrDpIpRegRead exit\n"));
  return Status;
}

/**
  Write TBT3 DP IP register data for TitanRidge.

  @param[in] Router  - Pointer to router instance.
  @param[in] AdpNum  - Adapter number.
  @param[in] Address - DP IP register address.
  @param[in] Data    - Data to be written to DP IP register.

  @retval EFI_SUCCESS           - Write DP IP register data successfully.
  @retval EFI_UNSUPPORTED       - Fail to write DP IP register data.
  @retval EFI_TIMEOUT           - Fail to write DP IP register data within timeout period.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
TrDpIpRegWrite (
  IN  PUSB4_ROUTER    Router,
  IN  UINT8           AdpNum,
  IN  UINT16          Address,
  IN  UINT32          Data
  )
{
  EFI_STATUS              Status;
  PUSB4_ADAPTER           DpAdp;
  TBT3_DP_VSC_0_IP_CMD    IpCmd;
  TBT3_DP_VSC_0_IP_ADDR   IpAddr;
  UINT32                  Index;

  DEBUG ((DEBUG_INFO, "TrDpIpRegWrite entry\n"));

  //
  // Check required parameters.
  //
  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Null router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Invalid AdpNum %d\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (Router->RouterType != USB4_ROUTER_TITAN_RIDGE) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Rt 0x%016llX is not TitanRidge!\n", Router->TopologyId));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  DpAdp = &(Router->Adapter[AdpNum]);
  if ((DpAdp->Type != USB4_ADP_TYPE_DP_IN) && (DpAdp->Type != USB4_ADP_TYPE_DP_OUT)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Rt 0x%016llX Adp %d is not DP adapter\n", Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  if (DpAdp->Vsc0Reg == 0) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Null VSC0 capability!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Set ID_ADDR.Address
  //
  Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_IP_ADDR_OFFSET, 1, (UINT32 *) &IpAddr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Read VSC0 IP_ADDR failure, %r\n", Status));
    goto Exit;
  }
  IpAddr.Fields.Addr = Address;
  Status = Usb4CsAdpWrite (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_IP_ADDR_OFFSET, 1, (UINT32 *) &IpAddr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Write VSC0 IP_ADDR failure, %r\n", Status));
    goto Exit;
  }

  //
  // Write IP_WDATA.
  //
  Status = Usb4CsAdpWrite (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_WDATA_OFFSET, 1, &Data);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Write VSC0 IP_WDATA failure, %r\n", Status));
    goto Exit;
  }

  //
  // Set IP_CMD.Cmd and IP_CMD.Valid.
  //
  Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_IP_CMD_OFFSET, 1, (UINT32 *) &IpCmd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Read VSC0 IP_CMD failure, %r\n", Status));
    goto Exit;
  }
  IpCmd.Fields.Cmd   = TBT3_DP_IP_CMD_WRITE;
  IpCmd.Fields.Valid = 1;
  Status = Usb4CsAdpWrite (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_IP_CMD_OFFSET, 1, (UINT32 *) &IpCmd);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Write VSC0 IP_CMD failure, %r\n", Status));
    goto Exit;
  }

  //
  // Poll until IP_CMD.Valid bit is cleared.
  //
  for (Index = 0; Index < TBT3_DP_IP_POLL_MAX; Index++) {
    Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->Vsc0Reg + TBT3_DP_IP_CMD_OFFSET, 1, (UINT32 *) &IpCmd);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Read VSC0 IP_CMD failure, %r\n", Status));
      goto Exit;
    }
    if (IpCmd.Fields.Valid == 0) {
      break;
    }
    gBS->Stall (TBT3_DP_IP_POLL_US);
  }
  if (Index == TBT3_DP_IP_POLL_MAX) {
    DEBUG ((DEBUG_ERROR, "TrDpIpRegWrite: Wait cmd valid bit cleared timeout!\n"));
    Status = EFI_TIMEOUT;
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "TrDpIpRegWrite exit\n"));
  return Status;
}

/**
  Initialize DP IP register data befor CMHS
  (Refer to CM TBT3 Device Support 5.2.1.2)

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Initialize DP IP register successfully.
  @retval EFI_UNSUPPORTED       - Fail to initialize DP IP register.
  @retval EFI_TIMEOUT           - Fail to initialize DP IP register within timeout period.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3TrDpCmhsInit (
  IN PUSB4_ROUTER    Router
  )
{
  EFI_STATUS      Status;
  UINT8           AdpNum;
  PUSB4_ADAPTER   Adapter;
  UINT32          RegData;

  DEBUG ((DEBUG_INFO, "Tbt3TrDpCmhsInit entry\n"));

  Status = EFI_SUCCESS;

  //
  // Check required parameters.
  //
  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrDpCmhsInit: Null router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (Router->RouterType != USB4_ROUTER_TITAN_RIDGE) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrDpCmhsInit: Rt 0x%016llX is not TitanRidge!\n", Router->TopologyId));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  for (AdpNum = 0; (AdpNum <= Router->MaxAdapter) && (AdpNum < USB4_ADAPTER_SUPPORT_MAX); AdpNum++) {
    Adapter = &(Router->Adapter[AdpNum]);
    if ((Adapter->Type != USB4_ADP_TYPE_DP_IN) && (Adapter->Type != USB4_ADP_TYPE_DP_OUT)) {
      continue;
    }
    if (Adapter->Vsc0Reg == 0) {
      DEBUG ((DEBUG_ERROR, "Null VSC0 capability in Rt 0x%016llX DP Adp %d!\n", Router->TopologyId, AdpNum));
      continue;
    }

    //
    // Set DP IP register 0xD282 bit 3:0 t0 0.
    //
    Status = TrDpIpRegRead (Router, AdpNum, TR_DP_IP_CMHS_INIT_ADDR, &RegData);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "DP IP read failure for CMHS init, %r\n", Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "Original Rt 0x%016llX Adp %d DP IP RegData = 0x%08X\n", Router->TopologyId, AdpNum, RegData));

    RegData &= 0xFFFFFFF0;
    Status = TrDpIpRegWrite (Router, AdpNum, TR_DP_IP_CMHS_INIT_ADDR, RegData);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "DP IP write failure for CMHS init, %r\n", Status));
      goto Exit;
    }
  }
Exit:
  DEBUG ((DEBUG_INFO, "Tbt3TrDpCmhsInit exit\n"));
  return Status;
}

/**
  Read NVM version for TitanRidge router.

  @param[in]  Router     - Pointer to router instance.
  @param[out] NvmVersion - Pointer to the returned NVM version.

  @retval EFI_SUCCESS           - Read NVM version successfully.
  @retval EFI_UNSUPPORTED       - Fail to read NVM version.
  @retval EFI_TIMEOUT           - Fail to get response for NVM command within timeout period.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Tbt3TrReadNvmVersion (
  IN PUSB4_ROUTER    Router,
  OUT UINT8          *NvmVersion
  )
{
  EFI_STATUS           Status;
  PUSB4_ADAPTER        HiAdp;
  UINT8                HiAdpNum;
  TBT3_NVM_CMD         NvmReq;
  TBT3_NVM_CMD_RESP    NvmResp;
  UINT32               NvmData;
  UINT32               Index;

  DEBUG ((DEBUG_INFO, "Tbt3TrReadNvmVersion entry\n"));

  //
  // Check required parameters.
  //
  if ((Router == NULL) || (NvmVersion == NULL)) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrReadNvmVersion: Invalid parameter (%p, %p)!\n", Router, NvmVersion));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (Router->RouterType != USB4_ROUTER_TITAN_RIDGE) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrReadNvmVersion: Rt 0x%016llX is not TitanRidge!\n", Router->TopologyId));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  HiAdpNum = Router->HiAdpNum;
  if ((HiAdpNum == 0) || (HiAdpNum > Router->MaxAdapter) || (HiAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrReadNvmVersion: Host interface adapter not available\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  HiAdp = &(Router->Adapter[HiAdpNum]);
  if (HiAdp->Vsc1Reg == 0) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrReadNvmVersion: Null VSC1 capability in host interface Adp %d!\n", HiAdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // NVM read request.
  // Send NVM read request to read a DWORD at offset 8. (Refer to CM TBT3 Device Support 5.2.1.2.2)
  // NVM version is at NVM offset 0x0A, the third byte of DWORD at offset 8.
  //
  NvmReq.Value            = 0;
  NvmReq.Fields.Cmd       = TBT3_NVM_CMD_READ;
  NvmReq.Fields.NvmOffset = 8;
  NvmReq.Fields.DwCount   = 1;
  NvmReq.Fields.OpReq     = 1;
  Status = Usb4CsAdpWrite (Router, HiAdpNum, HiAdp->Vsc1Reg + TBT3_HI_MAIL_IN_OFFSET, 1, (UINT32 *) &NvmReq);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write TBT3 mail in command (NVM Read) failure, %r\n", Status));
    goto Exit;
  }

  //
  // Poll Operation Request bit until it is cleared.
  //
  for (Index = 0; Index < TBT3_NVM_READ_POLL_MAX; Index++) {
    Status = Usb4CsAdpRead (Router, HiAdpNum, HiAdp->Vsc1Reg + TBT3_HI_MAIL_IN_OFFSET, 1, (UINT32 *) &NvmReq);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Read TBT3 mail in command (NVM Read) failure, %r\n", Status));
      goto Exit;
    }
    if (NvmReq.Fields.OpReq == 0) {
      DEBUG ((DEBUG_INFO, "Operation request bit cleared, retry = %d\n", Index));
      break;
    }
    gBS->Stall (TBT3_NVM_POLL_US);
  }
  if (Index == TBT3_NVM_READ_POLL_MAX) {
    DEBUG ((DEBUG_ERROR, "Poll Operation request bit cleared timeout!\n"));
    Status = EFI_TIMEOUT;
    goto Exit;
  }

  //
  // NVM read response.
  //
  Status = Usb4CsAdpRead (Router, HiAdpNum, HiAdp->Vsc1Reg + TBT3_HI_MAIL_OUT_OFFSET, 1, (UINT32 *) &NvmResp);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrReadNvmVersion: Read TBT3 NVM command response failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "NVM command response = 0x%08X, cmd = 0x%x\n", NvmResp.Value, NvmResp.Fields.Cmd));
  if (NvmResp.Fields.Response == 0) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrReadNvmVersion: NVM response bit is not set!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Read NVM data
  //
  Status = Usb4CsAdpRead (Router, HiAdpNum, HiAdp->Vsc1Reg + TBT3_HI_DATA_1_OFFSET, 1, (UINT32 *) &NvmData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tbt3TrReadNvmVersion: Read NVM Data1 failure, %r\n", Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Tbt3TrReadNvmVersion: NVM Data1 = 0x%08X\n", NvmData));

  //
  // NVM version is at NVM offset 0x0A, the third byte of DWORD at offset 8.
  //
  *NvmVersion = (UINT8)((NvmData >> 16) & 0x000000FF);

Exit:
  DEBUG ((DEBUG_INFO, "Tbt3TrReadNvmVersion exit\n"));
  return Status;
}