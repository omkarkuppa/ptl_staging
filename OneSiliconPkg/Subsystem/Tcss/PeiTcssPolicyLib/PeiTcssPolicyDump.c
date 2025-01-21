/** @file
  This file provides services for TCSS policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <TcssPeiConfig.h>
#include <TcssInfo.h>

/**
  Print TCSS_PEI_CONFIG and serial out.

  @param[in] TcssPeiConfig            Pointer to a TCSS_PEI_CONFIG
**/
VOID
TcssConfigDump (
  IN TCSS_PEI_CONFIG     *TcssPeiConfig
  )
{
  TCSS_PCIE_PORT_POLICY  *SaPeiPciCommonPortConfig;
  UINTN                  Index;

  if (TcssPeiConfig == NULL) {
    DEBUG ((DEBUG_INFO, "The TcssPeiConfig is NULL!!"));
    return;
  }

  DEBUG ((DEBUG_INFO, "------------------------ TCSS_PEI_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, "[TCSS] POST_MEM Config Block Revision : %d\n", TcssPeiConfig->Header.Revision));

  DEBUG ((DEBUG_INFO, "TcssPeiConfig->UsbConfig.OverCurrentEnable or PCH OvercurrentEnable 0x%02x\n", TcssPeiConfig->UsbConfig.OverCurrentEnable));
  DEBUG ((DEBUG_INFO, "TcssPeiConfig->UsbConfig.PdoProgramming 0x%02x\n", TcssPeiConfig->UsbConfig.PdoProgramming));
  DEBUG ((DEBUG_INFO, "TcssPeiConfig->UsbConfig.XhciOcLock 0x%02x\n", TcssPeiConfig->UsbConfig.XhciOcLock));

  for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
    DEBUG ((DEBUG_INFO, "TcssPeiConfig->UsbConfig.PortUsb30[%x].Enable 0x%02x\n", Index, TcssPeiConfig->UsbConfig.PortUsb30[Index].Enable));
    DEBUG ((DEBUG_INFO, "TcssPeiConfig->UsbConfig.PortUsb30[%x].OverCurrentPin 0x%02x\n", Index, TcssPeiConfig->UsbConfig.PortUsb30[Index].OverCurrentPin));
  }

  for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
    DEBUG ((DEBUG_INFO, "TcssPeiConfig->MiscConfig.TcssConvUsbA[%x].Enable 0x%02x\n", Index, TcssPeiConfig->MiscConfig.TcssConvUsbA[Index].Field.Enable));
    DEBUG ((DEBUG_INFO, "TcssPeiConfig->MiscConfig.TcssConvUsbA[%x].MappingPchXhciUsb2 0x%02x\n", Index, TcssPeiConfig->MiscConfig.TcssConvUsbA[Index].Field.MappingPchXhciUsb2));
    DEBUG ((DEBUG_INFO, "TcssPeiConfig->MiscConfig.TcssConvUsbA[%x].Orientational 0x%02x\n", Index, TcssPeiConfig->MiscConfig.TcssConvUsbA[Index].Field.Orientational));
  }

  DEBUG ((DEBUG_INFO, "TcssPeiConfig->MiscConfig.LsxOe %d\n", TcssPeiConfig->MiscConfig.LsxOe));

  DEBUG ((DEBUG_INFO, "TcssPeiConfig->IomConfig.IomInterface.VccSt %d\n", TcssPeiConfig->IomConfig.IomInterface.VccSt));
  DEBUG ((DEBUG_INFO, "TcssPeiConfig->IomConfig.IomInterface.D3ColdEnable %d\n", TcssPeiConfig->IomConfig.IomInterface.D3ColdEnable));
  DEBUG ((DEBUG_INFO, "TcssPeiConfig->IomConfig.TcStateLimit %d\n", TcssPeiConfig->IomConfig.TcStateLimit));
  DEBUG ((DEBUG_INFO, "TcssPeiConfig->IomConfig.TcNotifyIgd %d\n", TcssPeiConfig->IomConfig.TcNotifyIgd));

  DEBUG ((DEBUG_INFO, "TcssPeiConfig->IomConfig.PmcInterface.PmcPdEnable %d\n", TcssPeiConfig->IomConfig.PmcInterface.PmcPdEnable));

  DEBUG((DEBUG_INFO, "------------------------ TCSS SA_PEI_PCIE_COMMON_CONFIG -----------------\n"));

  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    SaPeiPciCommonPortConfig = &(TcssPeiConfig->PciePolicy.PciePortPolicy[Index]);
    DEBUG((DEBUG_INFO, " Itbt Root Port %x Config :\n", Index));
    DEBUG((DEBUG_INFO, " AcsEnabled : %d\n", SaPeiPciCommonPortConfig->AcsEnabled));
    DEBUG((DEBUG_INFO, " LtrEnable : %d\n", SaPeiPciCommonPortConfig->LtrEnable));
    DEBUG((DEBUG_INFO, " ItbtPcieRootPortPTM : %d\n", SaPeiPciCommonPortConfig->PtmEnabled));
    DEBUG((DEBUG_INFO, " Aspm : %d\n", SaPeiPciCommonPortConfig->Aspm));
    DEBUG((DEBUG_INFO, " SlotNumber : %d\n", SaPeiPciCommonPortConfig->SlotNumber));
    DEBUG((DEBUG_INFO, " SlotPowerLimitScale : %d\n", SaPeiPciCommonPortConfig->SlotPowerLimitScale));
    DEBUG((DEBUG_INFO, " SlotPowerLimitValue : %d\n", SaPeiPciCommonPortConfig->SlotPowerLimitValue));
    DEBUG((DEBUG_INFO, " AdvancedErrorReporting : %d\n", SaPeiPciCommonPortConfig->AdvancedErrorReporting));
    DEBUG((DEBUG_INFO, " UnsupportedRequestReport : %d\n", SaPeiPciCommonPortConfig->UnsupportedRequestReport));
    DEBUG((DEBUG_INFO, " FatalErrorReport : %d\n", SaPeiPciCommonPortConfig->FatalErrorReport));
    DEBUG((DEBUG_INFO, " NoFatalErrorReport : %d\n", SaPeiPciCommonPortConfig->NoFatalErrorReport));
    DEBUG((DEBUG_INFO, " SystemErrorOnFatalError : %d\n", SaPeiPciCommonPortConfig->SystemErrorOnFatalError));
    DEBUG((DEBUG_INFO, " SystemErrorOnNonFatalError : %d\n", SaPeiPciCommonPortConfig->SystemErrorOnNonFatalError));
    DEBUG((DEBUG_INFO, " SystemErrorOnCorrectableError : %d\n", SaPeiPciCommonPortConfig->SystemErrorOnCorrectableError));

    DEBUG((DEBUG_INFO, " LtrMaxSnoopLatency   : %d\n", SaPeiPciCommonPortConfig->LtrMaxSnoopLatency));
    DEBUG((DEBUG_INFO, " LtrMaxNoSnoopLatency : %d\n", SaPeiPciCommonPortConfig->LtrMaxNoSnoopLatency));
    DEBUG((DEBUG_INFO, " SnoopLatencyOverrideMode          : %d\n", SaPeiPciCommonPortConfig->SnoopLatencyOverrideMode));
    DEBUG((DEBUG_INFO, " SnoopLatencyOverrideMultiplier    : %d\n", SaPeiPciCommonPortConfig->SnoopLatencyOverrideMultiplier));
    DEBUG((DEBUG_INFO, " SnoopLatencyOverrideValue         : %d\n", SaPeiPciCommonPortConfig->SnoopLatencyOverrideValue));
    DEBUG((DEBUG_INFO, " NonSnoopLatencyOverrideMode       : %d\n", SaPeiPciCommonPortConfig->NonSnoopLatencyOverrideMode));
    DEBUG((DEBUG_INFO, " NonSnoopLatencyOverrideMultiplier : %d\n", SaPeiPciCommonPortConfig->NonSnoopLatencyOverrideMultiplier));
    DEBUG((DEBUG_INFO, " NonSnoopLatencyOverrideValue      : %d\n", SaPeiPciCommonPortConfig->NonSnoopLatencyOverrideValue));
    DEBUG((DEBUG_INFO, " ForceLtrOverride : %d\n", SaPeiPciCommonPortConfig->ForceLtrOverride));
    DEBUG((DEBUG_INFO, " LtrConfigLock    : %d\n", SaPeiPciCommonPortConfig->LtrConfigLock));
  }
}