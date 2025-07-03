/** @file
  This file contains functions that initialize PCI Express clock sources in PCH.

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

@par Specification Reference:
**/

#include <Ppi/SiPolicy.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Register/PchRegsClk.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/TimerLib.h>
#include <GpioV2Signals.h>
#include <Library/GpioV2AccessLib.h>
#include <Register/GpioAcpiDefines.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/GpioHelpersLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>
#include <Library/Ptl/PcdInfoLib/PtlPcdInfoLib.h>


/**
  Checks if the IPC Command to manage PCIE CLKREQ Override is supported.

  @retval TRUE        It's supported
  @retval FALSE       It's not supported
**/
BOOLEAN
IsClkreqIpcCmdSupported (
  VOID
  )
{
  return FALSE;
}

/**
  Disables one PCIe clock.

  @param[in] ClockUsage     Type and number of PCIe port for which Clock should be disabled
  @param[in] ClockNumber    The clock which needs to be disabled.
**/
VOID
DisableClockBufferProgramming (
  PCH_PCIE_CLOCK_USAGE ClockUsage,
  UINT32               ClockNumber
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  IsClkAccess;
  P2SB_PORT_16_ID                P2SBPid;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_ICLK;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (
    &P2SbController,
    P2SBPid.PortId.LocalPid,
    0,
    P2SbPrivateConfig,
    P2SbMmioAccess,
    FALSE,
    &IsClkAccess
    );
  IsClkAccess.Access.AndThenOr32 (&IsClkAccess.Access, R_ISCLK_PCR_BIOS_BUFFEN_H, ~(BIT0 << ClockNumber), 0);
}

/**
  Checks current state of ClkReq GPIO pad that was assigned to given port.
  This function returns error when port has no Clock or ClkReq assigned
  or if GPIO pad is not owned by BIOS. If there's no error, pad's current input value is returned.

  @param[in] ClockUsage     PCIe port for which ClkReq must be checked
  @retval EFI_UNSUPPORTED   no ClkReq assigned or input value can't be read
  @retval EFI_DEVICE_ERROR  wrong pad state, high
  @retval EFI_SUCCESS       ClkReq is in correct state, low
**/
EFI_STATUS
CheckClkReq (
  PCH_PCIE_CLOCK_USAGE ClockUsage
  )
{
  EFI_STATUS         Status;
  UINT32             ClkReqNumber;
  GPIOV2_NATIVE_PAD  ClkReqPad;
  GPIOV2_CONFIG      PadConfig;
  GPIOV2_PAD_STATE   GpioValue;
  UINT32             GpioPinMuxValue;
  GPIOV2_SERVICES    *GpioServices;

  //
  // If GPIO Override is enabled then dont change CLKREQ GPIO to GPIO mode.
  // As Native function programming will not be done later.
  //
  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  ClkReqNumber = FindClkReqForUsage (ClockUsage);
  if (ClkReqNumber == PCH_PCIE_NO_SUCH_CLOCK || ClkReqNumber >= GetPchMaxPcieClockReqNum ()) {
    return EFI_UNSUPPORTED;
  }

  //
  //  Get Gpio Pin Mux Value
  //
  GpioPinMuxValue = FindGpioPinMuxForClkReq (ClockUsage);

  ZeroMem (&PadConfig, sizeof(PadConfig));

  ClkReqPad = PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_PCIE_SRCCLKREQ (ClkReqNumber), GpioPinMuxValue);
  PadConfig.PadMode      = GpioV2PadModeGpio;
  PadConfig.Direction    = GpioV2DirIn;
  GpioServices->ConfigurePad (GpioServices, ClkReqPad, &PadConfig);
  DEBUG ((DEBUG_INFO, "Enabling CLKREQ%d ClkReqPad %0x\n", ClkReqNumber, ClkReqPad));

  //
  // Wait some time to make sure GPIO pad was reconfigured and its input value propagated to internal logic
  //
  MicroSecondDelay (5);

  Status = GpioServices->GetRx (GpioServices, ClkReqPad, &GpioValue);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
  if (GpioValue != GpioV2StateLow) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Enables CLK REQ for given PCIe port
  If given port has a clock source assigned and that clock source has a clock request signal,
  then the corresponding GPIO pad is configured into ClkReq mode

  @param[in]  ClockUsage      type and number of PCIe port
  @retval     EFI_SUCCESS     Clock Request was successfully enabled
  @retval     EFI_UNSUPPORTED there's no ClkReq pad corresponding to given Port
**/
EFI_STATUS
EnableClkReq (
  PCH_PCIE_CLOCK_USAGE ClockUsage
  )
{
  EFI_STATUS      Status;
  UINT32          ClkNumber;
  UINT32          ClkReqNumber;
  UINT32          GpioPinMuxValue;
  GPIOV2_PAD      ClkReqPad;
  GPIOV2_CONFIG   PadConfig;
  GPIOV2_SERVICES *GpioServices;

  if (GpioOverrideLevel1Enabled ()) {
    DEBUG ((DEBUG_INFO, "%a () - End. Gpio Override Enabled, skipped GPIO configuration.\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  ClkNumber = ClockUsageToClockNumber (GetClocksPolicy (), ClockUsage);
  if (ClkNumber == PCH_PCIE_NO_SUCH_CLOCK) {
    return EFI_UNSUPPORTED;
  }
  ClkReqNumber = ClockNumberToClkReqNumber (GetClocksPolicy (), ClkNumber);
  if (ClkReqNumber == PCH_PCIE_NO_SUCH_CLOCK || ClkReqNumber >= GetPchMaxPcieClockReqNum ()) {
    return EFI_UNSUPPORTED;
  }

  //
  // Get Gpio Pin Mux Value
  //
  GpioPinMuxValue = ClockNumberToClkReqGpioPinMux (GetClocksPolicy (), ClkNumber);

  ZeroMem (&PadConfig, sizeof (PadConfig));
  //
  // Configure GPIO pins for ClkReq
  //
  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_PCIE_SRCCLKREQ (ClkReqNumber), GpioPinMuxValue);
  if (Status == EFI_SUCCESS) {
    ClkReqPad = PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_PCIE_SRCCLKREQ (ClkReqNumber), GpioPinMuxValue);
    PadConfig.Direction   = GpioV2DirNone;
    PadConfig.ResetConfig = GpioV2ResetHost;
    GpioServices->ConfigurePad (GpioServices, ClkReqPad, &PadConfig);
    DEBUG ((DEBUG_INFO, "Enabling CLKREQ%d ClkReqPad %0x\n", ClkReqNumber, ClkReqPad));
  }

  return EFI_SUCCESS;
}

/**
  Programs PadRstCfg for Gpio CLK REQ not assigned pin for given PCIe port
  Corresponding GPIO pad PadRstMode is configured into GpioV2ResetHost mode
  @param[in]  ClockUsage      type and number of PCIe port
  @retval     EFI_SUCCESS     PadRstMode was successfully enabled
  @retval     EFI_UNSUPPORTED PadRstMode was not set to given Port
**/
EFI_STATUS
ChangePadRstCfg (
  PCH_PCIE_CLOCK_USAGE ClockUsage
  )
{
  EFI_STATUS      Status;
  UINT32          ClkNumber;
  UINT32          ClkReqNumber;
  UINT32          GpioPinMuxValue;
  GPIOV2_PAD      ClkReqPad;
  GPIOV2_CONFIG   PadConfig;
  GPIOV2_SERVICES *GpioServices;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);


  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices(%a, %d) failed (Status: %d)\n", __FUNCTION__, GPIO_HID_PTL_PCD_P, 0, Status));
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  ClkNumber = ClockUsageToClockNumber (GetClocksPolicy (), ClockUsage);
  if (ClkNumber == PCH_PCIE_NO_SUCH_CLOCK) {
    return EFI_UNSUPPORTED;
  }
  ClkReqNumber = ClockNumberToClkReqNumber (GetClocksPolicy (), ClkNumber);
  if (ClkReqNumber == PCH_PCIE_NO_SUCH_CLOCK || ClkReqNumber >= GetPchMaxPcieClockReqNum ()) {
    return EFI_UNSUPPORTED;
  }
  //
  // Get Gpio Pin Mux Value
  //
  GpioPinMuxValue = ClockNumberToClkReqGpioPinMux (GetClocksPolicy (), ClkNumber);

  ZeroMem (&PadConfig, sizeof (PadConfig));

  ClkReqPad = PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_PCIE_SRCCLKREQ (ClkReqNumber), GpioPinMuxValue);
  PadConfig.ResetConfig = GpioV2ResetHost;
  GpioServices->ConfigurePad (GpioServices, ClkReqPad, &PadConfig);
  DEBUG ((DEBUG_INFO, "Configure PadRstCfg on CLKREQ%d ClkReqPad %0x\n", ClkReqNumber, ClkReqPad));

  return EFI_SUCCESS;
}
