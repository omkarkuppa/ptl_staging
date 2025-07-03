/** @file
  The PCH Initialization Dispatcher After Memory.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include "PtlPcdInitPei.h"
#include <IntelRcStatusCode.h>
#include <Library/PchDmiLib.h>
#include <Library/PeiGbeInitLib.h>
#include <Library/PeiP2sbPrivateLib.h>
#include <Library/PeiRtcLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PeiSataLib.h>
#include <Library/SpiInitLib.h>
#include <Library/PeiSerialIoInitLib.h>
#include <Library/PeiSmbusLib.h>
#include <Library/PeiFusaLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/ChipsetInitLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/ThcSocLib.h>
#include <Library/SerialIoUartSocLib.h>
#include <Library/PcdPeiLpssI2cSocInitLib.h>
#include <Library/PcdPeiLpssI3cSocInitLib.h>
#include <Library/SerialIoSpiSocLib.h>
#include <Library/PeiLpcLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <RegisterAccess.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <Register/GpioAcpiDefines.h>
#include <Ppi/FspmArchConfigPpi.h>
#include <PcdSbPortIds.h>
#include <Library/PerformanceLib.h>
#include <Library/Ptl/PcdMinimalGpioNativeLib/PtlPcdMinimalGpioNativeLib.h>

#define PCD_PCIE_NO_SUCH_CLOCK  0xFF
#define PCD_MAX_PCIE_CLOCK_7    7
#define PCD_PCIE_CLOCK_8        7
#define PCD_PCIE_CLOCK_9        8

/*
  Performs PCH ISH Controller initialization

  @param[in] SiPolicy     The Silicon Policy PPI instance
*/
VOID
PchIshConfigure (
  IN    SI_POLICY_PPI     *SiPolicy
  );

/**
  The function issues reset based on SI_SCHEDULE_RESET_HOB
**/
STATIC
VOID
PchPeiReset (
  VOID
  )
{
  EFI_STATUS                Status;
  BOOLEAN                   ResetStatus;
  VOID                      *S3DataPtr;
  FSPM_ARCH_CONFIG_PPI      *FspmArchConfigPpi;

  if (!SiScheduleResetIsRequired ()) {
    return;
  }
  //
  // Locate FSP-M Arch Config PPI for Memory Configuration Save Data.
  //
  FspmArchConfigPpi = NULL;
  S3DataPtr         = NULL;
  Status = PeiServicesLocatePpi (
             &gFspmArchConfigPpiGuid,
             0,
             NULL,
             (VOID **) &FspmArchConfigPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (FspmArchConfigPpi == NULL) {
    return;
  }
  if (!EFI_ERROR (Status)) {
    S3DataPtr = FspmArchConfigPpi->NvsBufferPtr;
  }

  if (S3DataPtr == NULL) {
    DEBUG ((DEBUG_INFO, "MRC Data not valid. Postpone reset to DXE\n"));
    return;
  }
  ResetStatus = SiScheduleResetPerformReset ();
  ASSERT (!ResetStatus);
}

/**
  Pch init after memory PEI module

  @param[in] SiPolicy     The Silicon Policy PPI instance

  @retval None
**/
VOID
EFIAPI
PtlPcdInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   LpcPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_LPC;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMsgAccess, FALSE, &LpcPcrAccess);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_START);
  DEBUG ((DEBUG_INFO, "PchInit - Start\n"));

  //
  // Build and Update PCH configuration HOB. This should be done before device init starts.
  //
  BuildPchConfigHobs (SiPolicy);

  PERF_INMODULE_BEGIN ("PcdPhyConfigure");
  PtlPcdPhyConfigure (SiPolicy);
  PERF_INMODULE_END ("PcdPhyConfigure");

  PchConfigureClkreqMapping ();

  if (IsPchGbeSupported ()) {
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_GBE);
    PtlPcdGbeInit (SiPolicy);
  }

#if FixedPcdGet8(PcdTsnSupport) == 0x1
  //
  // Configure Time Sensitive Networking devices
  //
  PtlPcdTsnInit (SiPolicy);
#endif

  //
  // PchInfo HOB must be created before PCIe root port initialization, because
  // afterwards it cannot be determined which ports were fused off
  //
  BuildPchInfoHob ();

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_PCIE_RP);
  PERF_INMODULE_BEGIN ("PcdPcieRpInit");
  PtlPcdPcieRpInit (SiPolicy);
  PERF_INMODULE_END ("PcdPcieRpInit");
  //
  // Handle PCH PSF Disable
  //
  PtlPcdPsfDisableP2pDecoding ();

  //
  // Set Deferred Write Buffer in PSF
  //
  PtlPcdPsfProgramDWB (SiPolicy);

  PtlPcdUsbInit (SiPolicy);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_P2SB);
  PtlPcdP2SbInit (SiPolicy);
  if (!PtlPcdIsSecondP2SbHidden ()) {
    PtlPcdSecondP2SbInit (SiPolicy);
  }

  //
  // Configure PSTH
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_PSTH);
  PchPsthConfigure (SiPolicy);

  //
  // Set PSF Multicast EOI forwarding for ITSS (with IOAPIC controller)
  // P2SB needs to be set as EOI Target because interrupt controller (ITSS)
  // is a sideband device located behind P2SB
  //
  PtlPcdPsfEnableP2SbEoiTarget ();
  //
  // Configure IOAPIC and interrupts
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_ITSS);
  PtlPcdItssInit (SiPolicy);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_CNVI);
  PtlPcdCnviPostMemInit (SiPolicy);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_THC);
  ThcSocInit (SiPolicy);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_SCS);
  PtlPcdScsInit (SiPolicy);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_ISH);
  PchIshConfigure (SiPolicy);

  //
  // Configure GPIO PM and interrupt settings
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_GPIO);
  PtlPcdMinimalGpioConfigurePm ();
  PtlPcdMinimalGpioSetIrq (ItssGetGpioDevIntConfig (SiPolicy));
  //
  // Configure PSF PM settings
  //
  PtlPcdPsfConfigurePowerManagement ();

  //
  // Configure RTC
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_RTC);
  PtlPcdRtcInit (SiPolicy);

  //
  // Configure SMBUS
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_SMBUS);
  SmbusConfigure (SiPolicy);

  //
  // Configure LPC PM
  //
  LpcConfigurePm ((REGISTER_ACCESS *) &LpcPcrAccess);

  //
  // Configure LPSS
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_LPSS);
  PtlPcdLpssInit (SiPolicy);

  //
  // Configure eSPI after memory
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_ESPI);
  PtlPcdEspiInit (SiPolicy);

  //
  // Configure IEH
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_IEH);
  PtlPcdIehInit (SiPolicy);

  //
  // ChipsetInit
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_CHIPSETINIT);
  ChipsetInitSync ();

  //
  // Configure Serial IO Function 0
  // This has to happen late here after all other PCH devices (non serial) are configured because non serial devices
  // may share device number with func0 serial Io
  //
  PtlPcdLpssFunction0Disable (SiPolicy);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_PMC);
  PtlPcdPmcInit (SiPolicy);

  //
  // Move Bluetooth configuration from CnviPostMemInit () to after PmcInit ()
  // where utmiclk is readily available to detect connection.
  //
  PtlPcdCnviConfigureBt (SiPolicy);

  //
  // Configure SSE
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_SSE);
  PtlPcdSseInit (SiPolicy);

  //
  // Disable the clock 8 and 9 for PTL PCD-P
  //
  if (PtlIsPcdP () && (GetPchMaxPcieClockNum () == PCD_MAX_PCIE_CLOCK_7)) {
    DisableClockBufferProgramming (PCH_PCIE_NO_SUCH_CLOCK, PCD_PCIE_CLOCK_8);
    DisableClockBufferProgramming (PCH_PCIE_NO_SUCH_CLOCK, PCD_PCIE_CLOCK_9);
  }

  //
  // Issue Reset based on SiScheduleResetHob
  //
  PchPeiReset ();

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_POSTMEM_INIT_DONE);
  DEBUG ((DEBUG_INFO, "PchInit - End\n"));
}
