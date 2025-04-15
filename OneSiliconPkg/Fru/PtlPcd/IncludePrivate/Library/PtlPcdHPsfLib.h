/** @file
  This file contains internal header for PTL PCD-H PSF lib usage

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

#ifndef _PTL_PCD_H_PSF_SOC_LIB_H_
#define _PTL_PCD_H_PSF_SOC_LIB_H_

#include <Library/PsfLib.h>
#include <Ppi/SiPolicy.h>

/**
  Disable LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdHPsfDisableLpssI2c (
  IN UINT32  I2cNum
  );

/**
  Hide LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdHPsfHideLpssI2c (
  IN UINT32  I2cNum
  );

/**
  Unhide LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdHPsfUnhideLpssI2c (
  IN UINT32  I2cNum
  );

/**
  Enable LPSS I2C BAR1 at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdHPsfDisableLpssI2cBar1 (
  IN UINT32  I2cNum
  );

/**
  Disable LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdHPsfDisableLpssI3c (
  IN UINT32  I3cNum
  );

/**
  Hide LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdHPsfHideLpssI3c (
  IN UINT32  I3cNum
  );

/**
  Unhide LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdHPsfUnhideLpssI3c (
  IN UINT32  I3cNum
  );

/**
  Enable LPSS I3C BAR1 at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdHPsfDisableLpssI3cBar1 (
  IN UINT32  I3cNum
  );

/**
  Disable LPSS SPI at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdHPsfDisableLpssSpi (
  IN UINT32  SpiNum
  );

/**
  Hide LPSS SPI at PSF level

  @param[in] SpiNum  LPSS SPI device (SPI0, SPI1, ....)
**/
VOID
PtlPcdHPsfHideLpssSpi (
  IN UINT32  SpiNum
  );

/**
  Unhide LPSS SPI at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdHPsfUnhideLpssSpi (
  IN UINT32  SpiNum
  );

/**
  Disable LPSS SPI BAR1 at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdHPsfDisableLpssSpiBar1 (
  IN UINT32  SpiNum
  );

/**
  Disable LPSS UART BAR1 at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdHPsfDisableLpssUartBar1 (
  IN UINT32  UartNum
  );

/**
  Enable LPSS UART BAR1 at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdHPsfEnableLpssUartBar1 (
  IN UINT32  UartNum
  );
/**
  Enable LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdHPsfEnableLpssUart (
  IN UINT32  UartNum
  );

/**
  Disable LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdHPsfDisableLpssUart (
  IN UINT32  UartNum
  );

/**
  Unhide LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdHPsfUnhideLpssUart (
  IN UINT32  UartNum
  );

/**
  Hide LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdHPsfHideLpssUart (
  IN UINT32  UartNum
  );

/**
  Disable TraceHub at PSF level
**/
VOID
PtlPcdHPsfDisableTraceHub (
  VOID
  );

/**
  Set TraceHub ACPI BARx address at PSF level

  @param[in] BarNum      BAR Number (0:BAR0, 1:BAR1, ...)
  @param[in] BarValue    32bit BAR value
**/
VOID
PtlPcdHPsfSetTraceHubAcpiBar (
  IN UINT8     BarNum,
  IN UINT32    BarValue
  );

/**
  Enable TraceHub ACPI Memory Space at PSF level
**/
VOID
PtlPcdHPsfEnableTraceHubAcpiMemSpace (
  VOID
  );

/**
  Hide TraceHub ACPI at PSF level
**/
VOID
PtlPcdHPsfHideTraceHubAcpi (
  VOID
  );

/**
  Disable TraceHub ACPI at PSF level
**/
VOID
PtlPcdHPsfDisableTraceHubAcpi (
  VOID
  );

/**
  Enable HECI at PSF level

  @param[in] HeciNumber  HECI Number (1-4)
**/
VOID
PtlPcdHPsfEnableHeci (
  IN  UINT8     HeciNumber
  );

/**
  Disable HECI at PSF level

  @param[in] HeciNumber  HECI Number (1-4)
**/
VOID
PtlPcdHPsfDisableHeci (
  IN  UINT8     HeciNumber
  );

/**
  Enable SOL at PSF level
**/
VOID
PtlPcdHPsfEnableSol (
  VOID
  );

/**
  Disable SOL at PSF level
**/
VOID
PtlPcdHPsfDisableSol (
  VOID
  );

/**
  Disable IDER device at PSF level
**/
VOID
PtlPcdHPsfDisableIder (
  VOID
  );

/**
  Disable ISH at PSF level
**/
VOID
PtlPcdHPsfDisableIsh (
  VOID
  );

/**
  Disable ISH BAR1 at PSF level
**/
VOID
PtlPcdHPsfDisableIshBar1 (
  VOID
  );

/**
  Check if PCIe Root Port is enabled at PSF level

  @param[in] RpIndex      PCIe Root Port Index

  @retval    TRUE         Root Port is enabled on PSF
  @retval    FALSE        Root Port is disabled on PSF
**/
BOOLEAN
PtlPcdHPsfIsPcieRootPortEnabled (
  IN  UINT32  RpIndex
  );

/**
  Disable CNVi at PSF level
**/
VOID
PtlPcdHPsfDisableCnvi (
  VOID
  );

/**
  Disable CNVi Bluetooth(R) at PSF level
**/
VOID
PtlPcdHPsfDisableCnviBt (
  VOID
  );

/**
  Disable THC device at PSF level

  @param[in]  ThcNumber                Touch Host Controller Number THC0 or THC1
**/
VOID
PtlPcdHPsfDisableThc (
  IN  UINT32        ThcNumber
  );

/**
  Disable HDAudio device at PSF level
**/
VOID
PtlPcdHPsfDisableAce (
  VOID
  );

/**
  Disable Dsp bar at PSF level
**/
VOID
PtlPcdHPsfDisableDspBar (
  VOID
  );

/**
  Disable xDCI device at PSF level
**/
VOID
PtlPcdHPsfDisableXdci (
  VOID
  );

/**
  Set PMC ABASE at PSF level

  @param[in] Abase  PMC ACPI base address to be set
**/
VOID
PtlPcdHPsfSetPmcAbase (
  IN  UINT16  Abase
  );

/**
  Hide PMC at PSF level
**/
VOID
PtlPcdHPsfHidePmc (
  VOID
  );

/**
  Get PMC PWRM BAR0 from PSF

  @retval Address  Address for PWRM BAR0
**/
UINTN
PtlPcdHPsfGetPmcPwrmBar0 (
  VOID
  );

/**
  Disable GbE device at PSF level
**/
VOID
PtlPcdHPsfDisableGbe (
  VOID
  );

/**
  Disable SMBUS device at PSF level
**/
VOID
PtlPcdHPsfDisableSmbus (
  VOID
  );

/**
  Disable SCS UFS at PSF level

  @param[in] UfsNumber   UFS number
**/
VOID
PtlPcdHPsfDisableScsUfs (
  IN UINT32  UfsNumber
  );

/**
  Disable SCS UFS BAR1 at PSF level

  @param[in] UfsNumber   UFS number
**/
VOID
PtlPcdHPsfDisableScsUfsBar1 (
  IN UINT32  UfsNumber
  );

/**
  Return max number of PCIe controllers decoded by PSF
**/
UINT32
PtlPcdHPsfGetMaxPsfDecodedPciePortNum (
  VOID
  );

/**
  Disable SSE at PSF level

  @param[in] SseNumber     SSE Number (1-3)
**/
VOID
PtlPcdHPsfDisableSse (
  IN UINT8      SseNumber
  );

/**
  PCIe PSF port destination ID (psf_id:port_group_id:port_id:channel_id)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval Destination ID
**/
PSF_PORT_DEST_ID
PtlPcdHPsfPcieDestinationId (
  IN UINT32  RpIndex
  );

/**
  Enable PCIe Relaxed Ordering in PSF
**/
VOID
PtlPcdHPsfEnablePcieRelaxedOrder (
  VOID
  );

/**
  Configure MCTP
**/
VOID
PtlPcdHPsfMctpConfigure (
  VOID
  );

/**
  Enable VTd support in PSF.
**/
VOID
PtlPcdHPsfEnableVtd (
  VOID
  );

/**
  Disable PSF address-based peer-to-peer decoding.
**/
VOID
PtlPcdHPsfDisableP2pDecoding (
  VOID
  );

/**
  Program Deferred Write Buffer

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
PtlPcdHPsfProgramDWB (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

/**
  Program Grant Counts for PCIE controllers on PTL-PCD-H

  @param[in] PcieCtrlBifurcationArray        Array of PCIe controllers bifurcation config
  @param[in] PcieCtrlNumOfLanesArray         Array of lanes per PCIe controller
  @param[in] PcieCtrlNumOfRootPortsArray     Array of root ports per PCIe controller
  @param[in] ArraySize                       Number of PCIe contollers
  @param[in] PcieRpEnableArray               Array of Root Port enable state
  @param[in] PcieRpArraySize                 Number of Root Ports and size of PcieRpEnableArray
**/
VOID
PtlPcdHPsfGrantCountProgramming (
  UINT8                  *PcieCtrlBifurcationArray,
  UINT8                  *PcieCtrlNumOfLanesArray,
  UINT8                  *PcieCtrlNumOfRootPortsArray,
  UINT32                 ArraySize,
  BOOLEAN                *PcieRpEnableArray,
  UINT32                 PcieRpArraySize
  );

/**
  Program P2SB as EOI target
**/
VOID
PtlPcdHPsfEnableP2SbEoiTarget (
  VOID
  );

/**
  Program Root Port as PSF EOI target
  @param[in] RpIndex   Root Port number
**/
VOID
PtlPcdHPsfEnableRpEoiTarget (
  UINT32  RpIndex
  );

/**
  Reset registers that are not reset by themselves in every type of reset
  and reset Root Port functions

  @param[in] PcieRpFuncNumArray        PCIE RP Function number array
  @param[in] PcieRpFuncNumArraySize    Size of PcieRpFuncNumArray
**/
VOID
PtlPcdHPsfEarlyInit (
  IN  UINT8                     *PcieRpFuncNumArray,
  IN  UINT32                    PcieRpFuncNumArraySize
  );

/**
  Disable PCIe Root Port at PSF level

  @param[in]  RpIndex         PCIe Root Port Index (0 based)
**/
VOID
PtlPcdHPsfDisablePcieRootPort (
  IN UINT32  RpIndex
  );

/**
  Enable PCIe Root Port at PSF level

  @param[in]  RpIndex         PCIe Root Port Index (0 based)
**/
VOID
PtlPcdHPsfEnablePcieRootPort (
  IN UINT32  RpIndex
  );

/**
  Set PCIe Root Port function in PSF

  @param[in] RpIndex       Root Port index (0 based)
  @param[in] NewFunction   New PCIe Root Port function
**/
VOID
PtlPcdHPsfSetPcieRpFunction (
  IN UINT32  RpIndex,
  IN UINT32  NewFunction
  );

/**
  Disable Trunk Clock Gating, Local Clock Gating and Partition Clock Gating
  for all PSFs in PSF topology
**/
VOID
PtlPcdHPsfDisableClockGating (
  VOID
  );

/**
  Enable Trunk Clock Gating, Local Clock Gating and Partition Clock Gating
  for all PSFs in PSF topology
**/
VOID
PtlPcdHPsfEnableClockGating (
  VOID
  );

/**
  Configure PSF power management.
  Must be called after all PSF configuration is completed.
**/
VOID
PtlPcdHPsfConfigurePowerManagement (
  VOID
  );

/**
  Program hysteresis value for trunk clock gating on PSF6.
  Value should be set to 0x9c4.
**/
VOID
PtlPcdHPsfConfigureClkreqHys (
  VOID
  );
#endif
