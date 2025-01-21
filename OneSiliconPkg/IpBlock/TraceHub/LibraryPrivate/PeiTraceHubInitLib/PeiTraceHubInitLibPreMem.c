/** @file
  TraceHub Init PreMem Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiTraceHubInitLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/TraceHubRegs.h>
#include <InterruptConfig.h>

/**
  Wait trace source input buffer empty

  @param[in]  TraceHubSbAccessMmio    Pointer to TraceHub mmio sideband interface
  @param[in]  BitCheck                Specify bit check

  @retval EFI_SUCCESS                 Successfully completed.
  @retval EFI_TIMEOUT                 timeout while waiting for input buffer empty
**/
STATIC
EFI_STATUS
WaitEmpty (
  IN REGISTER_ACCESS                 *TraceHubSbAccessMmio,
  IN UINT32                           BitCheck
  )
{
  UINTN                            Timeout;

  Timeout = 100;
  while (Timeout > 0) {
    if ((TraceHubSbAccessMmio->Read32 (TraceHubSbAccessMmio, R_TRACE_HUB_MEM_GTHSTAT) & BitCheck) == 0) {
      break;
    } else {
      Timeout--;
    }
  }
  if (Timeout > 0) {
    return EFI_SUCCESS;
  }
  DEBUG ((DEBUG_ERROR, "Wait empty timeout 0x08%x for bit 0x%08x\n", TraceHubSbAccessMmio->Read32 (TraceHubSbAccessMmio, R_TRACE_HUB_MEM_GTHSTAT), BitCheck));
  return EFI_TIMEOUT;
}

/**
  Disable early trace

  Trace is activated by HW default.
  Stopping trace allows system entering s0ix or keep tracing but blocking s0ix

  @param[in]  TraceHubSbAccessMmio    Pointer to TraceHub mmio sideband interface
  @param[in]  TraceHubPciBase         TraceHub Pci base
**/
STATIC
VOID
DisableTrace (
  IN REGISTER_ACCESS                 *TraceHubSbAccessMmio,
  IN UINT64                          TraceHubPciBase
  )
{
  // Clear wait-empty
  TraceHubSbAccessMmio->And32 (TraceHubSbAccessMmio, R_TRACE_HUB_MEM_NDB_CTRL, (UINT32)~(B_TRACE_HUB_MEM_NDB_CTRL_NDB_WAITEMPTY));
  // Stop STH trace storage
  TraceHubSbAccessMmio->And32 (TraceHubSbAccessMmio, R_TRACE_HUB_MEM_SCR, (UINT32)~(B_TRACE_HUB_MEM_SCR_STOREENOVRD4));
  // Wait for trace source input buffer empty
  WaitEmpty (TraceHubSbAccessMmio, (UINT32) B_TRACE_HUB_MEM_GTHSTAT_IBE4);
  // Stop DTF trace storage
  TraceHubSbAccessMmio->And32 (TraceHubSbAccessMmio, R_TRACE_HUB_MEM_SCR, (UINT32)~(B_TRACE_HUB_MEM_SCR_STOREENOVRD5));
  // Wait for trace source buffer empty
  WaitEmpty (TraceHubSbAccessMmio, (UINT32) B_TRACE_HUB_MEM_GTHSTAT_IBE5);

  PciSegmentOr8 (TraceHubPciBase + R_TRACE_HUB_CFG_NPKDSC, B_TRACE_HUB_CFG_NPKDSC_FLR);

  DEBUG ((DEBUG_INFO, "Disable early trace complete\n"));
}

/**
  This function performs basic initialization for TraceHub, and return if trace hub needs to be power gated.

  Although controller allows access to a 64bit address resource, PEI phase is a 32-bit environment,
  addresses greater than 4G is not allowed by CPU address space.
  So, the addresses must be limited to below 4G and UBARs should be set to 0.
  If scratchpad0 bit24 is set, which indicates trace hub is used by tool that implies probe is connected and capturing traces.
  With such assumption, BIOS may not power gate trace hub but initialize trace hub as host debugger mode despite what trace hub policy it is.

  @param[in]  TraceHubHandle         Pointer to TraceHub handle structure
  @param[in, out]  TraceHubDisable   Determine if TraceHub needs to be power-gated.
                                     TRUE : Power gate TraceHub
                                     FALSE : Do not power-gate TraceHub
**/
VOID
TraceHubInitialize (
  IN TRACEHUB_HANDLE                 *TraceHubHandle,
  IN OUT  BOOLEAN                    *TraceHubDisable
  )
{
  UINT32                             Scrpd0;
  UINT8                              TraceVerbosity;
  UINT64                             TraceHubPciBase;
  TRACE_HUB_CONFIG                   *TraceHubConfig;
  TRACEHUB_PRIVATE_CONFIG            *PrivateConfig;
  REGISTER_ACCESS                    *TraceHubSbAccessMmio;

  TraceHubPciBase = 0;
  TraceVerbosity = 0;
  TraceHubPciBase = TraceHubHandle->Controller->PciCfgBaseAddr;
  TraceHubConfig = TraceHubHandle->TraceHubConfig;
  PrivateConfig = TraceHubHandle->PrivateConfig;
  TraceHubSbAccessMmio = TraceHubHandle->TraceHubSbAccessMmio;

  DEBUG ((DEBUG_INFO, "TraceHubInitialize() B%d:D%d:F%d start\n", TraceHubHandle->Controller->Bus, TraceHubHandle->Controller->Device, TraceHubHandle->Controller->Function));

  if (PciSegmentRead16 (TraceHubPciBase) == 0xFFFF) {
    if (TraceHubConfig->EnableMode == TraceHubModeDisabled) {
      ///
      /// Clear verbosity to disable BIOS trace
      ///
      if (TraceHubConfig->BiosTraceSink) {
        UpdateTraceHubDataHob (0, (UINT32)PrivateConfig->FwBar);
      }
      DEBUG ((DEBUG_INFO, "TraceHubInitialize() - End. TraceHub device is not present due to TH mode is disabled\n"));
      *TraceHubDisable = FALSE;
      return;
    }
    //
    // We are here assuming TH is in PG state, but user opt-in TH enabled, so we un-gate trace hub via PMC IPC1 command.
    // Noted: Trace hub PG state preserves until G3 / global reset.
    // No need to check tool ownership via SCRPD0[24], for it can only be set when TH is active (un-gated).
    //
    DEBUG ((DEBUG_INFO, "TraceHubInitialize - un-gate trace hub due to user opt-in TraceHub mode from disabled to enabled.\n"));
    TraceHubHandle->Callback->TraceHubEnable (TraceHubHandle->Controller);
  }

  Scrpd0 = TraceHubSbAccessMmio->Read32 (TraceHubSbAccessMmio, R_TRACE_HUB_MEM_SCRPD0);
  DEBUG ((DEBUG_INFO, "Scrpd0 = 0x%08x\n", Scrpd0));

  ///
  /// Stop trace if need not keep tracing
  ///
  if (!(TraceHubHandle->Callback->TraceHubKeepTrace (TraceHubHandle->Controller)) && !(Scrpd0 & B_TRACE_HUB_MEM_SCRPD0_DEBUGGERINUSE)) {
    DisableTrace (TraceHubSbAccessMmio, TraceHubPciBase);
  }

  ///
  /// Check if STT is disconnected and if user requested disabling of Trace Hub
  ///
  if ((TraceHubConfig->EnableMode == TraceHubModeDisabled) && !(Scrpd0 & B_TRACE_HUB_MEM_SCRPD0_DEBUGGERINUSE)) {
    ///
    /// Clear MSE
    ///
    PciSegmentWrite8 (TraceHubPciBase + PCI_COMMAND_OFFSET, 0);
    DEBUG ((DEBUG_INFO, "TraceHubInitialize() - End. STT disconnected and Trace Hub requested to be disable\n"));
    *TraceHubDisable = TRUE;
    ///
    /// Clear verbosity to disable BIOS trace
    ///
    if (TraceHubConfig->BiosTraceSink) {
      UpdateTraceHubDataHob (0, (UINT32)PrivateConfig->FwBar);
    }
    return;
  }
  ///
  /// Cache BIOS trace verbosity into HOB
  ///
  if (TraceHubConfig->BiosTraceSink) {
    TraceVerbosity = TraceHubSbAccessMmio->Read8 (TraceHubSbAccessMmio, R_TRACE_HUB_MEM_SCRPD2);
    UpdateTraceHubDataHob (TraceVerbosity, (UINT32)PrivateConfig->FwBar);
  }
  ///
  /// Clear LPMEN
  ///
  DEBUG ((DEBUG_INFO, "LPP_CTL old = 0x%08x\n", TraceHubSbAccessMmio->Read32 (TraceHubSbAccessMmio, R_TRACE_HUB_MEM_LPP_CTL)));
  TraceHubSbAccessMmio->And32 (TraceHubSbAccessMmio, R_TRACE_HUB_MEM_LPP_CTL, (UINT32)~B_TRACE_HUB_MEM_LPP_CTL_LPMEN);
  DEBUG ((DEBUG_INFO, "LPP_CTL new = 0x%08x\n", TraceHubSbAccessMmio->Read32 (TraceHubSbAccessMmio, R_TRACE_HUB_MEM_LPP_CTL)));
  ///
  /// Enable TraceHub debug for Sx
  ///
  if (PrivateConfig->AcroClkSupported) {
    TraceHubHandle->Callback->TraceHubEnClk (TraceHubHandle->Controller);
  }
  ///
  /// Program the MTB BAR
  ///
  //
  // Set MTB_LBAR (PCI offset 0x10)
  //
  PciSegmentWrite32 (TraceHubPciBase + R_TRACE_HUB_CFG_MTB_LBAR, (UINT32) PrivateConfig->MtbBar);
  //
  // Set MTB_UBAR (PCI offset 0x14)
  //
  PciSegmentWrite32 (TraceHubPciBase + R_TRACE_HUB_CFG_MTB_UBAR, (UINT32) RShiftU64 (PrivateConfig->MtbBar, 32));
  ///
  /// Program the SW BAR
  ///
  //
  // SW_LBAR (PCI offset 0x18)
  //
  PciSegmentWrite32 (TraceHubPciBase + R_TRACE_HUB_CFG_SW_LBAR, (UINT32) PrivateConfig->SwBar);
  //
  // SW_UBAR (PCI offset 0x1C)
  //
  PciSegmentWrite32 (TraceHubPciBase + R_TRACE_HUB_CFG_SW_UBAR, (UINT32) RShiftU64 (PrivateConfig->SwBar, 32));

  ///
  /// Program RTIT BAR if needed (Not all TraceHub support RTIT)
  ///
  if (PrivateConfig->RtitBar > 0) {
    //
    // RTIT_LBAR (PCI offset 0x20)
    //
    PciSegmentWrite32 (TraceHubPciBase + R_TRACE_HUB_CFG_RTIT_LBAR, (UINT32) PrivateConfig->RtitBar);
    //
    // RTIT_UBAR (PCI offset 0x24)
    //
    PciSegmentWrite32 (TraceHubPciBase + R_TRACE_HUB_CFG_RTIT_UBAR, (UINT32) RShiftU64 (PrivateConfig->RtitBar, 32));
  }
  ///
  /// Program the FW BAR
  ///
  //
  // FW_LBAR (PCI offset 0x70)
  //
  PciSegmentWrite32 (TraceHubPciBase + R_TRACE_HUB_CFG_FW_LBAR, (UINT32) PrivateConfig->FwBar);
  //
  // FW_UBAR (PCI offset 0x74)
  //
  PciSegmentWrite32 (TraceHubPciBase + R_TRACE_HUB_CFG_FW_UBAR, (UINT32) RShiftU64 (PrivateConfig->FwBar, 32));
  //
  // Enable FW BAR : shadow FW BAR into phantom device
  //
  TraceHubHandle->Callback->TraceHubEnableFwBar (TraceHubHandle->Controller, PrivateConfig->FwBar);
  ///
  /// Set AET trace
  ///
  if (TraceHubConfig->AetEnabled) {
    TraceHubHandle->Callback->TraceHubSetAetTrace (PrivateConfig->FwBar);
  }
  ///
  /// Set INT Pin A which is fixed for TraceHub
  ///
  PciSegmentWrite8 (TraceHubPciBase + PCI_INT_PIN_OFFSET, PchIntA);

  ///
  /// Enable MSE and BME
  ///
  if ((Scrpd0 & B_TRACE_HUB_MEM_SCRPD0_DEBUGGERINUSE) && (Scrpd0 & B_TRACE_HUB_MEM_SCRPD0_MEMISPRIMDEST)) {
    // defer setting MSE and BME until post mem when trace to memory is activated
  } else {
    PciSegmentOr8 (TraceHubPciBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  }
  DEBUG ((DEBUG_INFO, "TraceHubInitialize () Assigned BARs:\n"));
  DEBUG ((DEBUG_INFO, "TraceHubInitialize () MTB_LBAR = 0x%08x\n", PciSegmentRead32 (TraceHubPciBase + R_TRACE_HUB_CFG_MTB_LBAR) & B_TRACE_HUB_CFG_MTB_LBAR_ADDR));
  DEBUG ((DEBUG_INFO, "TraceHubInitialize () SW_LBAR  = 0x%08x\n", PciSegmentRead32 (TraceHubPciBase + R_TRACE_HUB_CFG_SW_LBAR) & B_TRACE_HUB_CFG_SW_LBAR_ADDR));
  if (PrivateConfig->RtitBar > 0) {
    DEBUG ((DEBUG_INFO, "TraceHubInitialize () RTIT_LBAR  = 0x%08x\n", PciSegmentRead32 (TraceHubPciBase + R_TRACE_HUB_CFG_RTIT_LBAR) & B_TRACE_HUB_CFG_RTIT_LBAR_ADDR));
  }
  DEBUG ((DEBUG_INFO, "TraceHubInitialize () FW_LBAR  = 0x%08x\n", PciSegmentRead32 (TraceHubPciBase + R_TRACE_HUB_CFG_FW_LBAR) & B_TRACE_HUB_CFG_FW_LBAR_ADDR));
  DEBUG ((DEBUG_INFO, "TraceHubInitialize () - End\n"));
  *TraceHubDisable = FALSE;
}

/**
  Disable and power gate TraceHub

  @param[in]  TraceHubHandle       Pointer to TraceHub handle structure

  @retval     EFI_SUCCESS       The function completed successfully.
  @retval     EFI_UNSUPPORTED   The device is not present
**/
EFI_STATUS
TraceHubDisable (
  IN TRACEHUB_HANDLE                 *TraceHubHandle
  )
{
  DEBUG ((DEBUG_INFO, "TraceHubDisable() B%d:D%d:F%d\n", TraceHubHandle->Controller->Bus, TraceHubHandle->Controller->Device, TraceHubHandle->Controller->Function));

  if (PciSegmentRead16 (TraceHubHandle->Controller->PciCfgBaseAddr) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "TraceHub device is not present\n"));
    return EFI_UNSUPPORTED;
  }
  ///
  /// Remove force ACRO_ON
  ///
  if (TraceHubHandle->PrivateConfig->AcroClkSupported) {
    TraceHubHandle->Callback->TraceHubDisClk (TraceHubHandle->Controller);
  }
  ///
  /// Disable TraceHub
  ///
  TraceHubHandle->Callback->TraceHubDisable (TraceHubHandle->Controller);

  return EFI_SUCCESS;
}
