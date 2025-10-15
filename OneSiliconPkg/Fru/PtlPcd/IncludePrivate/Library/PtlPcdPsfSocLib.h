/** @file
  This file contains internal header for PSF lib usage

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _PTLPCD_PSF_SOC_LIB_H_
#define _PTLPCD_PSF_SOC_LIB_H_

#include <Library/PsfLib.h>
#include <Ppi/SiPolicy.h>

/**
  Disable LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfDisableLpssI2c (
  IN UINT32  I2cNum
  );

/**
  Hide LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfHideLpssI2c (
  IN UINT32  I2cNum
  );

/**
  Unhide LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfUnhideLpssI2c (
  IN UINT32  I2cNum
  );

/**
  Enable LPSS I2C BAR1 at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfDisableLpssI2cBar1 (
  IN UINT32  I2cNum
  );

/**
  Disable LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfDisableLpssI3c (
  IN UINT32  I3cNum
  );

/**
  Hide LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfHideLpssI3c (
  IN UINT32  I3cNum
  );

/**
  Unhide LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfUnhideLpssI3c (
  IN UINT32  I3cNum
  );

/**
  Enable LPSS I3C BAR1 at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfDisableLpssI3cBar1 (
  IN UINT32  I3cNum
  );

/**
  Disable LPSS SPI at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdPsfDisableLpssSpi (
  IN UINT32  SpiNum
  );

/**
  Hide LPSS SPI at PSF level

  @param[in] SpiNum  LPSS SPI device (SPI0, SPI1, ....)
**/
VOID
PtlPcdPsfHideLpssSpi (
  IN UINT32  SpiNum
  );

/**
  Unhide LPSS SPI at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdPsfUnhideLpssSpi (
  IN UINT32  SpiNum
  );

/**
  Disable LPSS SPI BAR1 at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdPsfDisableLpssSpiBar1 (
  IN UINT32  SpiNum
  );

/**
  Disable LPSS UART BAR1 at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfDisableLpssUartBar1 (
  IN UINT32  UartNum
  );

/**
  Enable LPSS UART BAR1 at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfEnableLpssUartBar1 (
  IN UINT32  UartNum
  );
/**
  Enable LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfEnableLpssUart (
  IN UINT32  UartNum
  );

/**
  Disable LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfDisableLpssUart (
  IN UINT32  UartNum
  );

/**
  Unhide LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfUnhideLpssUart (
  IN UINT32  UartNum
  );

/**
  Hide LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfHideLpssUart (
  IN UINT32  UartNum
  );

/**
  Disable TraceHub at PSF level
**/
VOID
PtlPcdPsfDisableTraceHub (
  VOID
  );

/**
  Set TraceHub ACPI BARx address at PSF level

  @param[in] BarNum      BAR Number (0:BAR0, 1:BAR1, ...)
  @param[in] BarValue    32bit BAR value
**/
VOID
PtlPcdPsfSetTraceHubAcpiBar (
  IN UINT8     BarNum,
  IN UINT32    BarValue
  );

/**
  Enable TraceHub ACPI Memory Space at PSF level
**/
VOID
PtlPcdPsfEnableTraceHubAcpiMemSpace (
  VOID
  );

/**
  Hide TraceHub ACPI at PSF level
**/
VOID
PtlPcdPsfHideTraceHubAcpi (
  VOID
  );

/**
  Disable TraceHub ACPI at PSF level
**/
VOID
PtlPcdPsfDisableTraceHubAcpi (
  VOID
  );

/**
  Enable HECI at PSF level

  @param[in] HeciNumber  HECI Number (1-4)
**/
VOID
PtlPcdPsfEnableHeci (
  IN  UINT8     HeciNumber
  );

/**
  Disable HECI at PSF level

  @param[in] HeciNumber  HECI Number (1-4)
**/
VOID
PtlPcdPsfDisableHeci (
  IN  UINT8     HeciNumber
  );

/**
  Enable SOL at PSF level
**/
VOID
PtlPcdPsfEnableSol (
  VOID
  );

/**
  Disable SOL at PSF level
**/
VOID
PtlPcdPsfDisableSol (
  VOID
  );

/**
  Disable IDER device at PSF level
**/
VOID
PtlPcdPsfDisableIder (
  VOID
  );

/**
  Disable ISH at PSF level
**/
VOID
PtlPcdPsfDisableIsh (
  VOID
  );

/**
  Disable ISH BAR1 at PSF level
**/
VOID
PtlPcdPsfDisableIshBar1 (
  VOID
  );

/**
  Disable CNVi at PSF level
**/
VOID
PtlPcdPsfDisableCnvi (
  VOID
  );

/**
  Disable CNVi Bluetooth(R) at PSF level
**/
VOID
PtlPcdPsfDisableCnviBt (
  VOID
  );

/**
  Disable THC device at PSF level

  @param[in]  ThcNumber                Touch Host Controller Number THC0 or THC1
**/
VOID
PtlPcdPsfDisableThc (
  IN  UINT32        ThcNumber
  );

/**
  Disable HDAudio device at PSF level
**/
VOID
PtlPcdPsfDisableAce (
  VOID
  );

/**
  Disable Dsp bar at PSF level
**/
VOID
PtlPcdPsfDisableDspBar (
  VOID
  );

/**
  Disable xDCI device at PSF level
**/
VOID
PtlPcdPsfDisableXdci (
  VOID
  );

/**
  Set PMC ABASE at PSF level

  @param[in] Abase  PMC ACPI base address to be set
**/
VOID
PtlPcdPsfSetPmcAbase (
  IN  UINT16  Abase
  );

/**
  Hide PMC at PSF level
**/
VOID
PtlPcdPsfHidePmc (
  VOID
  );

/**
  Get PMC PWRM BAR0 from PSF

  @retval Address  Address for PWRM BAR0
**/
UINTN
PtlPcdPsfGetPmcPwrmBar0 (
  VOID
  );

/**
  Disable GbE device at PSF level
**/
VOID
PtlPcdPsfDisableGbe (
  VOID
  );

/**
  Disable SMBUS device at PSF level
**/
VOID
PtlPcdPsfDisableSmbus (
  VOID
  );

/**
  Disable SCS UFS at PSF level

  @param[in] UfsNumber   UFS number
**/
VOID
PtlPcdPsfDisableScsUfs (
  IN UINT32  UfsNumber
  );

/**
  Disable SCS UFS BAR1 at PSF level

  @param[in] UfsNumber   UFS number
**/
VOID
PtlPcdPsfDisableScsUfsBar1 (
  IN UINT32  UfsNumber
  );

/**
  Return max number of PCIe controllers decoded by PSF
**/
UINT32
PtlPcdPsfGetMaxPsfDecodedPciePortNum (
  VOID
  );

/**
  Disable SSE at PSF level

  @param[in] SseNumber     SSE Number (1-3)
**/
VOID
PtlPcdPsfDisableSse (
  IN UINT8      SseNumber
  );

/**
  PCIe PSF port destination ID (psf_id:port_group_id:port_id:channel_id)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval Destination ID
**/
PSF_PORT_DEST_ID
PtlPcdPsfPcieDestinationId (
  IN UINT32  RpIndex
  );

/**
  Check if PCIe Root Port is enabled at PSF level

  @param[in] RpIndex      PCIe Root Port Index

  @retval    TRUE         Root Port is enabled on PSF
  @retval    FALSE        Root Port is disabled on PSF
**/
BOOLEAN
PtlPcdPsfIsPcieRootPortEnabled (
  IN  UINT32  RpIndex
  );

/**
  Enable PCIe Relaxed Ordering in PSF
**/
VOID
PtlPcdPsfEnablePcieRelaxedOrder (
  VOID
  );

/**
  Configure MCTP
**/
VOID
PtlPcdPsfMctpConfigure (
  VOID
  );

/**
  Enable VTd support in PSF.
**/
VOID
PtlPcdPsfEnableVtd (
  VOID
  );

/**
  Disable PSF address-based peer-to-peer decoding.
**/
VOID
PtlPcdPsfDisableP2pDecoding (
  VOID
  );

/**
  Program Deferred Write Buffer

  @param[in] SiPolicyPpi            Pointer to SI_POLICY_PPI
**/
VOID
PtlPcdPsfProgramDWB (
  IN SI_POLICY_PPI     *SiPolicyPpi
  );

/**
  Program Grant Counts for PCIE controllers on WCL-PCD

  @param[in] PcieCtrlBifurcationArray        Array of PCIe controllers bifurcation config
  @param[in] PcieCtrlNumOfLanesArray         Array of lanes per PCIe controller
  @param[in] PcieCtrlNumOfRootPortsArray     Array of root ports per PCIe controller
  @param[in] ArraySize                       Number of PCIe contollers
  @param[in] PcieRpEnableArray               Array of Root Port enable state
  @param[in] PcieRpArraySize                 Number of Root Ports and size of PcieRpEnableArray
  @param[in] Forcel                          Force Link Width
**/
VOID
PtlPcdPsfGrantCountProgramming (
  UINT8                  *PcieCtrlBifurcationArray,
  UINT8                  *PcieCtrlNumOfLanesArray,
  UINT8                  *PcieCtrlNumOfRootPortsArray,
  UINT32                 ArraySize,
  BOOLEAN                *PcieRpEnableArray,
  UINT32                 PcieRpArraySize,
  UINT32                 Forcel
  );

/**
  Program P2SB as EOI target
**/
VOID
PtlPcdPsfEnableP2SbEoiTarget (
  VOID
  );

/**
  Program Root Port as PSF EOI target
  @param[in] RpIndex   Root Port number
**/
VOID
PtlPcdPsfEnableRpEoiTarget (
  UINT32  RpIndex
  );

/**
  Reset registers that are not reset by themselves in every type of reset
  and reset Root Port functions

  @param[in] PcieRpFuncNumArray        PCIE RP Function number array
  @param[in] PcieRpFuncNumArraySize    Size of PcieRpFuncNumArray
**/
VOID
PtlPcdPsfEarlyInit (
  IN  UINT8                     *PcieRpFuncNumArray,
  IN  UINT32                    PcieRpFuncNumArraySize
  );

/**
  Disable PCIe Root Port at PSF level

  @param[in]  RpIndex         PCIe Root Port Index (0 based)
**/
VOID
PtlPcdPsfDisablePcieRootPort (
  IN UINT32  RpIndex
  );

/**
  Enable PCIe Root Port at PSF level

  @param[in]  RpIndex         PCIe Root Port Index (0 based)
**/
VOID
PtlPcdPsfEnablePcieRootPort (
  IN UINT32  RpIndex
  );

/**
  Set PCIe Root Port function in PSF

  @param[in] RpIndex       Root Port index (0 based)
  @param[in] NewFunction   New PCIe Root Port function
**/
VOID
PtlPcdPsfSetPcieRpFunction (
  IN UINT32  RpIndex,
  IN UINT32  NewFunction
  );

/**
  Disable Trunk Clock Gating, Local Clock Gating and Partition Clock Gating
  for all PSFs in PSF topology
**/
VOID
PtlPcdPsfDisableClockGating (
  VOID
  );

/**
  Enable Trunk Clock Gating, Local Clock Gating and Partition Clock Gating
  for all PSFs in PSF topology
**/
VOID
PtlPcdPsfEnableClockGating (
  VOID
  );

/**
  Configure PSF power management.
  Must be called after all PSF configuration is completed.
**/
VOID
PtlPcdPsfConfigurePowerManagement (
  VOID
  );

/**
  Program hysteresis value for trunk clock gating on PSF6.
  Value should be set to 0x9c4.
**/
VOID
PtlPcdPsfConfigureClkreqHys (
  VOID
  );

/**
  Disable CAN device at PSF level

  @param[in]  CanNumber                CAN Controller Number (0 based)
**/
VOID
PtlPcdPsfDisableCan (
  IN  UINT32        CanNumber
  );
#endif
