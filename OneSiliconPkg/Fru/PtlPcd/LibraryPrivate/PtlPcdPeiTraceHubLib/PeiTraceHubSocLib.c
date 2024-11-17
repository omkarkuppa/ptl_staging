/** @file
  TraceHub SoC library.
  All functions in this library are available for PEI

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Register/PchRegs.h>
#include <Register/UsbHostRegs.h>
#include <Register/TraceHubRegs.h>
#include <Library/PchPciBdfLib.h>
#include <Library/ConfigBlockLib.h>
#include <Defines/PchReservedResources.h>
#include <Library/PmcLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PmcPrivateLib.h>
#include <IntelRcStatusCode.h>
#include <Library/DciPrivateLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/VtdInfoLib.h>
#include <TraceHubDataHob.h>
#include <Library/MtrrLib.h>
#include <Library/HobLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <TraceHubHandle.h>
#include <PcdSbPortIds.h>

/**
  Return if need to keep tracing which is activated by default

  @param[in]  TraceHubController      Pointer to TraceHub controller structure

  @retval TRUE    Keep Trace
  @retval FALSE   Stop Trace
**/
BOOLEAN
SocTraceHubKeepTrace (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  )
{
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   DciPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_EXI;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &DciPcrAccess);

  return IsKeepEarlyTrace ((REGISTER_ACCESS *) &DciPcrAccess);
}

/**
  Enable TraceHub FW_BAR.
  Since FW BAR is not conventional PCI BAR offset (0x70) that needs to be shadowed into
  independent PCI device in PSF -- TraceHub phantom device.
  BIOS programs TraceHub phantom device's bar 0 with FW Base and enable MSE
  to make FW BAR MMIO available

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
  @param[in]  FwBar                   FW Bar Address
**/
STATIC
VOID
SocTraceHubEnableFwBar (
  IN  TRACEHUB_CONTROLLER      *TraceHubController,
  IN  UINT64                   FwBar
  )
{
  //
  // Program FW_BAR to TraceHub phantom device BAR0 and BAR1 as FW_LBAR and FW_UBAR from PSF
  //
  PtlPcdPsfSetTraceHubAcpiBar (0, (UINT32) FwBar);
  PtlPcdPsfSetTraceHubAcpiBar (1, (UINT32) RShiftU64 (FwBar, 32));

  // Enable MSE of TraceHub phantom device from PSF
  //
  PtlPcdPsfEnableTraceHubAcpiMemSpace ();
  //
  // Hide TraceHub phantom Device
  //
  PtlPcdHideTraceHubAcpi ();
}

/**
  Set AET trace.
  AET is one of FW trace agents, need configure FW BAR for AET.

  @param[in]  FwBar                   FW Bar Address
**/
VOID
SocTraceHubSetAetTrace (
  IN  UINT64                   FwBar
  )
{
  CpuWriteTraceHubAcpiBase (FwBar);
}

/**
  Enable TraceHub

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
**/
STATIC
VOID
SocTraceHubEnable (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  )
{
  ///
  /// Un-power gate TraceHub
  ///
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREMEM_ENABLE_TRACEHUB);
  DEBUG ((DEBUG_INFO, "TraceHubEnable() un-power-gate Soc TraceHub device via PMC\n"));
  PmcEnableTraceHub (PmcGetPwrmBase ());
}

/**
  Disable TraceHub

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
**/
STATIC
VOID
SocTraceHubDisable (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  )
{
  ///
  /// Disable trace hub phantom device via PSF
  ///
  PtlPcdDisableTraceHubAcpi ();
  ///
  /// Disable trace hub via PSF
  ///
  DEBUG ((DEBUG_INFO, "SocTraceHubDisable() disable Trace Hub device via PSF\n"));
  PtlPcdPsfDisableTraceHub ();
  ///
  /// Power gate TraceHub
  ///
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREMEM_DISABLE_TRACEHUB);
  DEBUG ((DEBUG_INFO, "TraceHubDisable() Power gate Soc TraceHub device via PMC\n"));
  PmcDisableTraceHub (PmcGetPwrmBase ());
}

/**
  Enable ACRO clock for TraceHub

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
**/
STATIC
VOID
SocTraceHubEnClk (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  )
{
  // No Acro clock support for Soc TraceHub, implement null function
}

/**
  Disable ACRO clock for TraceHub

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
**/
STATIC
VOID
SocTraceHubDisClk (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  )
{
  // No Acro clock support for Soc TraceHub, implement null function
}

/**
  Get TraceHub controller instance for Soc

  @param[out]  TraceHubController     Pointer to TraceHub controller identification structure

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - TraceHubController is NULL
**/
EFI_STATUS
TraceHubGetSocController (
  OUT TRACEHUB_CONTROLLER         *TraceHubController
  )
{
  if (TraceHubController == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  TraceHubController->Segment           = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
  TraceHubController->Bus               = DEFAULT_PCI_BUS_NUMBER_PCH;
  TraceHubController->Device            = PchTraceHubDevNumber ();
  TraceHubController->Function          = PchTraceHubFuncNumber ();
  TraceHubController->PciCfgBaseAddr    = PchTraceHubPciCfgBase ();
  return EFI_SUCCESS;
}

/**
  Initialize Soc TraceHub handle structure
  This should be done according TraceHub IP integration in SoC

  @param[in] SiPreMemPolicyPpi       The SI PreMem Policy PPI instance
  @param[in out]  TraceHubHandle     TraceHub handle structure
  @param[in] TraceHubController      TRACEHUB_CONTROLLER structure
  @param[in] PrivateConfig           TRACEHUB_PRIVATE_CONFIG structure
  @param[in] P2SbController          P2SB controller pointer
  @param[in] Callback                TRACEHUB_CALLBACK structure
  @param[in] TraceHubSbAccessMmio    P2SB Register Access to TraceHub
**/
VOID
TraceHubInitSocHandle (
  IN SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi,
  IN OUT  TRACEHUB_HANDLE            *TraceHubHandle,
  IN TRACEHUB_CONTROLLER             *TraceHubController,
  IN TRACEHUB_PRIVATE_CONFIG         *PrivateConfig,
  IN TRACEHUB_CALLBACK               *Callback,
  IN P2SB_CONTROLLER                 *P2SbController,
  IN P2SB_SIDEBAND_REGISTER_ACCESS   *TraceHubSbAccessMmio
  )
{
  TRACE_HUB_PREMEM_CONFIG            *TraceHubPreMemConfig;
  EFI_STATUS                         Status;
  UINT16                             TraceHubFid;
  P2SB_PORT_16_ID                    P2SBPid;

  DEBUG ((DEBUG_INFO, "TraceHubInitSocHandle\n"));
  ZeroMem (TraceHubHandle, sizeof (TRACEHUB_HANDLE));
  ZeroMem (TraceHubController, sizeof (TRACEHUB_CONTROLLER));
  ZeroMem (PrivateConfig, sizeof (TRACEHUB_PRIVATE_CONFIG));
  ZeroMem (Callback, sizeof (TRACEHUB_CALLBACK));
  ZeroMem (TraceHubSbAccessMmio, sizeof (P2SB_SIDEBAND_REGISTER_ACCESS));
  //
  // Configure TraceHub controller
  //
  TraceHubGetSocController (TraceHubController);
  TraceHubHandle->Controller = TraceHubController;
  //
  // Configure TraceHub config block
  //
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  TraceHubHandle->TraceHubConfig = &TraceHubPreMemConfig->TraceHub[SocTraceHub];
  //
  // Configure TraceHub private config
  //
  PrivateConfig->MtbBar = PCH_TRACE_HUB_MTB_BASE_ADDRESS;
  PrivateConfig->SwBar = PCH_TRACE_HUB_SW_BASE_ADDRESS;
  PrivateConfig->RtitBar = PCH_TRACE_HUB_RTIT_BASE_ADDRESS;
  PrivateConfig->RtitSize = PCH_TRACE_HUB_RTIT_MMIO_SIZE;
  PrivateConfig->FwBar = PCH_TRACE_HUB_FW_BASE_ADDRESS;
  PrivateConfig->AcroClkSupported = FALSE;
  if (IsVtdEngineEnabled (IOP_VTD)) {
    PrivateConfig->RootSpace = 3;
  } else {
    PrivateConfig->RootSpace = 0;
  }
  TraceHubHandle->PrivateConfig = PrivateConfig;
  //
  // Configure TraceHub callbacks
  //
  Callback->TraceHubKeepTrace = SocTraceHubKeepTrace;
  Callback->TraceHubEnableFwBar = SocTraceHubEnableFwBar;
  Callback->TraceHubSetAetTrace = SocTraceHubSetAetTrace;
  Callback->TraceHubEnable = SocTraceHubEnable;
  Callback->TraceHubDisable = SocTraceHubDisable;
  Callback->TraceHubEnClk = SocTraceHubEnClk;
  Callback->TraceHubDisClk = SocTraceHubDisClk;
  TraceHubHandle->Callback = Callback;
  //
  // Configure TraceHub P2Sb access interface
  //
  P2SBPid.Pid16bit = PTL_SID_F2_PID_NPK;
  PtlPcdGetP2SbController (P2SbController, P2SBPid);

  TraceHubFid = (TraceHubHandle->Controller->Device << 3) | TraceHubHandle->Controller->Function;
  BuildP2SbSidebandAccess (P2SbController, P2SBPid.PortId.LocalPid, TraceHubFid, P2SbMemory, P2SbMsgAccess, FALSE, TraceHubSbAccessMmio);
  TraceHubHandle->TraceHubSbAccessMmio = (REGISTER_ACCESS *) TraceHubSbAccessMmio;
}

/**
  Read TraceHub MTB BAR register with Sideband Message Interface

  @param[in]  Offset                 Register offset
  @param[out] Data                   Register value

  @retval     EFI_SUCCESS            Completed successfully
              EFI_UNSUPPORTED        TraceHub does not exist
**/
EFI_STATUS
ReadMtbBar (
  IN  UINT32    Offset,
  OUT UINT32    *Data
  )
{
  TRACEHUB_CONTROLLER                TraceHubController;
  P2SB_CONTROLLER                    P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS      TraceHubSbAccessMmio;
  UINT16                             TraceHubFid;
  P2SB_PORT_16_ID                    P2SBPid;

  TraceHubGetSocController (&TraceHubController);
  if (PciSegmentRead32 (TraceHubController.PciCfgBaseAddr) == 0xffffffff) {
    return EFI_UNSUPPORTED;
  }
  P2SBPid.Pid16bit = PTL_SID_F2_PID_NPK;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  TraceHubFid = (TraceHubController.Device << 3) | TraceHubController.Function;
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, TraceHubFid, P2SbMemory, P2SbMsgAccess, FALSE, &TraceHubSbAccessMmio);
  *Data = TraceHubSbAccessMmio.Access.Read32 (&TraceHubSbAccessMmio.Access, Offset);
  return EFI_SUCCESS;
}

/**
  Write TraceHub MTB BAR register with Sideband Message Interface

  @param[in]  Offset                 Register offset
  @param[in]  *Data                  Register value

  @retval     EFI_SUCCESS            Completed successfully
              EFI_UNSUPPORTED        TraceHub does not exist
**/
EFI_STATUS
WriteMtbBar (
  IN  UINT32    Offset,
  IN  UINT32    *Data
  )
{
  TRACEHUB_CONTROLLER                TraceHubController;
  P2SB_CONTROLLER                    P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS      TraceHubSbAccessMmio;
  UINT16                             TraceHubFid;
  P2SB_PORT_16_ID                    P2SBPid;

  TraceHubGetSocController (&TraceHubController);
  if (PciSegmentRead32 (TraceHubController.PciCfgBaseAddr) == 0xffffffff) {
    return EFI_UNSUPPORTED;
  }
  P2SBPid.Pid16bit = PTL_SID_F2_PID_NPK;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  TraceHubFid = (TraceHubController.Device << 3) | TraceHubController.Function;
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, TraceHubFid, P2SbMemory, P2SbMsgAccess, FALSE, &TraceHubSbAccessMmio);
  TraceHubSbAccessMmio.Access.Write32 (&TraceHubSbAccessMmio.Access, Offset, *Data);
  return EFI_SUCCESS;
}

/**
  Determine if Soc TraceHub is in use by tool

  @retval     TRUE           - debugger is in use
              FALSE          - debugger is not in use
**/
BOOLEAN
DebuggerInUseSoc (
  VOID
  )
{
  EFI_STATUS                         Status;
  UINT32                             Scrpd0;

  Status = ReadMtbBar (R_TRACE_HUB_MEM_SCRPD0, &Scrpd0);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "SOC TraceHub SCRPD0 = 0x%08x\n", Scrpd0));

  if (Scrpd0 == 0xFFFFFFFF) {
    return FALSE;
  } else {
    return !!(Scrpd0 & B_TRACE_HUB_MEM_SCRPD0_DEBUGGERINUSE);
  }
}

/**
  Read TraceHub Scratchpad 1 register

  @param[out] Scrpd1                 Scrpd1 value

  @retval     EFI_SUCCESS            Completed successfully
              EFI_UNSUPPORTED        TraceHub does not exist
**/
EFI_STATUS
ReadTraceHubScrpd1 (
  OUT UINT32       *Scrpd1
  )
{
  return ReadMtbBar (R_TRACE_HUB_MEM_SCRPD1, Scrpd1);
}

/**
  Write TraceHub Scratchpad 1 register

  @param[in]  Scrpd1                 Scrpd1 value

  @retval     EFI_SUCCESS            Completed successfully
              EFI_UNSUPPORTED        TraceHub does not exist
**/
EFI_STATUS
WriteTraceHubScrpd1 (
  IN UINT32       *Scrpd1
  )
{
  return WriteMtbBar (R_TRACE_HUB_MEM_SCRPD1, Scrpd1);
}
