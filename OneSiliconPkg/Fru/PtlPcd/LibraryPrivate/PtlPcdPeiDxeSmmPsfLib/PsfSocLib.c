/** @file
  This file contains internal PSF routines for PCH PSF SoC lib usage

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataSocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/PchRegsPsf.h>
#include <Register/PsfRegsPtlPcdWrapper.h>
#include <Library/PsfLib.h>
#include <Library/PciSegmentLib.h>
#include <PchPcieRpInfo.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Defines/PchReservedResources.h>
#include <Register/PchRegs.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/PcdInfoLib.h>
#include <PsfConfig.h>
#include <PcdSbPortIds.h>
#include <Library/Ptl/PtlPcdP2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPPsfLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdHPsfLib.h>

/**
  Disable LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfDisableLpssI2c (
  IN UINT32  I2cNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableLpssI2c (I2cNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableLpssI2c (I2cNum);
  }
}

/**
  Hide LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfHideLpssI2c (
  IN UINT32  I2cNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfHideLpssI2c (I2cNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfHideLpssI2c (I2cNum);
  }
}

/**
  Unhide LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfUnhideLpssI2c (
  IN UINT32  I2cNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfUnhideLpssI2c (I2cNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfUnhideLpssI2c (I2cNum);
  }
}

/**
  Enable LPSS I2C BAR1 at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfDisableLpssI2cBar1 (
  IN UINT32  I2cNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableLpssI2cBar1 (I2cNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableLpssI2cBar1 (I2cNum);
  }
}

/**
  Disable LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfDisableLpssI3c (
  IN UINT32  I3cNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableLpssI3c (I3cNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableLpssI3c (I3cNum);
  }
}

/**
  Hide LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfHideLpssI3c (
  IN UINT32  I3cNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfHideLpssI3c (I3cNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfHideLpssI3c (I3cNum);
  }
}

/**
  Unhide LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfUnhideLpssI3c (
  IN UINT32  I3cNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfUnhideLpssI3c (I3cNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfUnhideLpssI3c (I3cNum);
  }
}

/**
  Enable LPSS I3C BAR1 at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfDisableLpssI3cBar1 (
  IN UINT32  I3cNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableLpssI3cBar1 (I3cNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableLpssI3cBar1 (I3cNum);
  }
}

/**
  Disable LPSS SPI at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdPsfDisableLpssSpi (
  IN UINT32  SpiNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableLpssSpi (SpiNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableLpssSpi (SpiNum);
  }
}

/**
  Hide LPSS SPI at PSF level

  @param[in] SpiNum  LPSS SPI device (SPI0, SPI1, ....)
**/
VOID
PtlPcdPsfHideLpssSpi (
  IN UINT32  SpiNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfHideLpssSpi (SpiNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfHideLpssSpi (SpiNum);
  }
}

/**
  Unhide LPSS SPI at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdPsfUnhideLpssSpi (
  IN UINT32  SpiNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfUnhideLpssSpi (SpiNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfUnhideLpssSpi (SpiNum);
  }
}

/**
  Disable LPSS SPI BAR1 at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdPsfDisableLpssSpiBar1 (
  IN UINT32  SpiNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableLpssSpiBar1 (SpiNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableLpssSpiBar1 (SpiNum);
  }
}

/**
  Disable LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfDisableLpssUart (
  IN UINT32  UartNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableLpssUart (UartNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableLpssUart (UartNum);
  }
}

/**
  Enable LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfEnableLpssUart (
  IN UINT32  UartNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnableLpssUart (UartNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnableLpssUart (UartNum);
  }
}

/**
  Unhide LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfUnhideLpssUart (
  IN UINT32  UartNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfUnhideLpssUart (UartNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfUnhideLpssUart (UartNum);
  }
}

/**
  Hide LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfHideLpssUart (
  IN UINT32  UartNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfHideLpssUart (UartNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfHideLpssUart (UartNum);
  }
}

/**
  Disable LPSS UART BAR1 at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfDisableLpssUartBar1 (
  IN UINT32  UartNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableLpssUartBar1 (UartNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableLpssUartBar1 (UartNum);
  }
}

/**
  Enable LPSS UART BAR1 at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfEnableLpssUartBar1 (
  IN UINT32  UartNum
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnableLpssUartBar1 (UartNum);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnableLpssUartBar1 (UartNum);
  }
}

/**
  Disable TraceHub at PSF level
**/
VOID
PtlPcdPsfDisableTraceHub (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableTraceHub ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableTraceHub ();
  }
}

/**
  Set TraceHub ACPI BARx address at PSF level

  @param[in] BarNum      BAR Number (0:BAR0, 1:BAR1, ...)
  @param[in] BarValue    32bit BAR value
**/
VOID
PtlPcdPsfSetTraceHubAcpiBar (
  IN UINT8     BarNum,
  IN UINT32    BarValue
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfSetTraceHubAcpiBar (BarNum, BarValue);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfSetTraceHubAcpiBar (BarNum, BarValue);
  }
}

/**
  Enable TraceHub ACPI Memory Space at PSF level
**/
VOID
PtlPcdPsfEnableTraceHubAcpiMemSpace (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnableTraceHubAcpiMemSpace ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnableTraceHubAcpiMemSpace ();
  }
}

/**
  Hide TraceHub ACPI at PSF level
**/
VOID
PtlPcdPsfHideTraceHubAcpi (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfHideTraceHubAcpi ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfHideTraceHubAcpi ();
  }
}

/**
  Disable TraceHub ACPI at PSF level
**/
VOID
PtlPcdPsfDisableTraceHubAcpi (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableTraceHubAcpi ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableTraceHubAcpi ();
  }
}

/**
  Enable HECI at PSF level

  @param[in] HeciNumber  HECI Number (1-4)
**/
VOID
PtlPcdPsfEnableHeci (
  IN  UINT8     HeciNumber
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnableHeci (HeciNumber);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnableHeci (HeciNumber);
  }
}

/**
  Disable HECI at PSF level

  @param[in] HeciNumber  HECI Number (1-4)
**/
VOID
PtlPcdPsfDisableHeci (
  IN  UINT8     HeciNumber
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableHeci (HeciNumber);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableHeci (HeciNumber);
  }
}


/**
  Disable SSE at PSF level

  @param[in] SseNumber     SSE Number (1-3)
**/
VOID
PtlPcdPsfDisableSse (
  IN UINT8      SseNumber
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableSse (SseNumber);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableSse (SseNumber);
  }
}

/**
  Enable SOL at PSF level
**/
VOID
PtlPcdPsfEnableSol (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnableSol ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnableSol ();
  }
}

/**
  Disable SOL at PSF level
**/
VOID
PtlPcdPsfDisableSol (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableSol ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableSol ();
  }
}

/**
  Disable IDER device at PSF level
**/
VOID
PtlPcdPsfDisableIder (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableIder ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableIder ();
  }
}

/**
  Disable HDAudio device at PSF level
**/
VOID
PtlPcdPsfDisableAce (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableAce ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableAce ();
  }
}

/**
  Disable DSP Bar (HdAudio BAR4 and BAR5) at PSF level.
**/
VOID
PtlPcdPsfDisableDspBar (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableDspBar ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableDspBar ();
  }
}

/**
  Disable THC device at PSF level

  @param[in]  ThcNumber                Touch Host Controller Number THC0 or THC1
**/
VOID
PtlPcdPsfDisableThc (
  IN  UINT32        ThcNumber
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableThc (ThcNumber);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableThc (ThcNumber);
  }
}

/**
  Disable xDCI device at PSF level
**/
VOID
PtlPcdPsfDisableXdci (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableXdci ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableXdci ();
  }
}

/**
  Disable ISH at PSF level
**/
VOID
PtlPcdPsfDisableIsh (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableIsh ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableIsh ();
  }
}

/**
  Disable ISH BAR1 at PSF level
**/
VOID
PtlPcdPsfDisableIshBar1 (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableIshBar1 ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableIshBar1 ();
  }
}

/**
  Disable GbE device at PSF level
**/
VOID
PtlPcdPsfDisableGbe (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableGbe ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableGbe ();
  }
}

/**
  Disable SMBUS device at PSF level
**/
VOID
PtlPcdPsfDisableSmbus (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableSmbus ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableSmbus ();
  }
}

/**
  Disable SCS UFS at PSF level

  @param[in] UfsNumber   UFS number
**/
VOID
PtlPcdPsfDisableScsUfs (
  IN UINT32  UfsNumber
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableScsUfs (UfsNumber);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableScsUfs (UfsNumber);
  }
}

/**
  Disable SCS UFS BAR1 at PSF level

  @param[in] UfsNumber   UFS number
**/
VOID
PtlPcdPsfDisableScsUfsBar1 (
  IN UINT32  UfsNumber
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableScsUfsBar1 (UfsNumber);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableScsUfsBar1 (UfsNumber);
  }
}

/**
  Return max number of PCIe controllers decoded by PSF
**/
UINT32
PtlPcdPsfGetMaxPsfDecodedPciePortNum (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    return PtlPcdPPsfGetMaxPsfDecodedPciePortNum ();
  } else if (PtlIsPcdH ()) {
    return PtlPcdHPsfGetMaxPsfDecodedPciePortNum ();
  }
  return 0;
}

/**
  PCIe PSF port destination ID (psf_id:port_group_id:port_id:channel_id)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval Destination ID
**/
PSF_PORT_DEST_ID
PtlPcdPsfPcieDestinationId (
  IN UINT32  RpIndex
  )
{
  if (PtlIsPcdP ()) {
    return PtlPcdPPsfPcieDestinationId (RpIndex);
  } else if (PtlIsPcdH ()) {
    return PtlPcdHPsfPcieDestinationId (RpIndex);
  }
  return (PSF_PORT_DEST_ID){0};
};

/**
  Check if PCIe Root Port is enabled at PSF level

  @param[in] RpIndex      PCIe Root Port Index

  @retval    TRUE         Root Port is enabled on PSF
  @retval    FALSE        Root Port is disabled on PSF
**/
BOOLEAN
PtlPcdPsfIsPcieRootPortEnabled (
  IN  UINT32  RpIndex
  )
{
  if (PtlIsPcdP ()) {
    return PtlPcdPPsfIsPcieRootPortEnabled (RpIndex);
  } else if (PtlIsPcdH ()) {
    return PtlPcdHPsfIsPcieRootPortEnabled (RpIndex);
  }
  return FALSE;
}

/**
  Disable CNVi at PSF level
**/
VOID
PtlPcdPsfDisableCnvi (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableCnvi ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableCnvi ();
  }
}

/**
  Disable CNVi Bluetooth(R) at PSF level
**/
VOID
PtlPcdPsfDisableCnviBt (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableCnviBt ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableCnviBt ();
  }
}

/**
  Set PMC ABASE at PSF level

  @param[in] Abase  PMC ACPI base address to be set
**/
VOID
PtlPcdPsfSetPmcAbase (
  IN  UINT16  Abase
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfSetPmcAbase (Abase);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfSetPmcAbase (Abase);
  }
}

/**
  Hide PMC at PSF level
**/
VOID
PtlPcdPsfHidePmc (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfHidePmc ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfHidePmc ();
  }
}

/**
  Get PMC PWRM BAR0 from PSF

  @retval Address  Address for PWRM BAR0
**/
UINTN
PtlPcdPsfGetPmcPwrmBar0 (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    return PtlPcdPPsfGetPmcPwrmBar0 ();
  } else if (PtlIsPcdH ()) {
    return PtlPcdHPsfGetPmcPwrmBar0 ();
  }
  return ~(UINT32)0;
}

/**
  Enable PCIe Relaxed Ordering in PSF
**/
VOID
PtlPcdPsfEnablePcieRelaxedOrder (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnablePcieRelaxedOrder ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnablePcieRelaxedOrder ();
  }
}

/**
  Configure MCTP
**/
VOID
PtlPcdPsfMctpConfigure (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfMctpConfigure ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfMctpConfigure ();
  }
}

/**
  Enable VTd support in PSF.
**/
VOID
PtlPcdPsfEnableVtd (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnableVtd ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnableVtd ();
  }
}

/**
  Disable PSF address-based peer-to-peer decoding.
**/
VOID
PtlPcdPsfDisableP2pDecoding (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableP2pDecoding ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableP2pDecoding ();
  }
}

/**
  Program Deferred Write Buffer

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
PtlPcdPsfProgramDWB (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfProgramDWB (SiPreMemPolicyPpi);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfProgramDWB (SiPreMemPolicyPpi);
  }
}

/**
  Program Grant Counts for PCIE controllers

  @param[in] PcieCtrlBifurcationArray        Array of PCIe controllers bifurcation config
  @param[in] PcieCtrlNumOfLanesArray         Array of lanes per PCIe controller
  @param[in] PcieCtrlNumOfRootPortsArray     Array of root ports per PCIe controller
  @param[in] ArraySize                       Number of PCIe contollers
  @param[in] PcieRpEnableArray               Array of Root Port enable state
  @param[in] PcieRpArraySize                 Number of Root Ports and size of PcieRpEnableArray
**/
VOID
PtlPcdPsfGrantCountProgramming (
  UINT8                  *PcieCtrlBifurcationArray,
  UINT8                  *PcieCtrlNumOfLanesArray,
  UINT8                  *PcieCtrlNumOfRootPortsArray,
  UINT32                 ArraySize,
  BOOLEAN                *PcieRpEnableArray,
  UINT32                 PcieRpArraySize
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfGrantCountProgramming (
      PcieCtrlBifurcationArray,
      PcieCtrlNumOfLanesArray,
      PcieCtrlNumOfRootPortsArray,
      ArraySize,
      PcieRpEnableArray,
      PcieRpArraySize
      );
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfGrantCountProgramming (
      PcieCtrlBifurcationArray,
      PcieCtrlNumOfLanesArray,
      PcieCtrlNumOfRootPortsArray,
      ArraySize,
      PcieRpEnableArray,
      PcieRpArraySize
      );
  }
}

/**
  Program P2SB as EOI target
**/
VOID
PtlPcdPsfEnableP2SbEoiTarget (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnableP2SbEoiTarget ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnableP2SbEoiTarget ();
  }
}

/**
  Set PCIe Root Port function in PSF

  @param[in] RpIndex       Root Port index (0 based)
  @param[in] NewFunction   New PCIe Root Port function
**/
VOID
PtlPcdPsfSetPcieRpFunction (
  IN UINT32  RpIndex,
  IN UINT32  NewFunction
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfSetPcieRpFunction (RpIndex, NewFunction);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfSetPcieRpFunction (RpIndex, NewFunction);
  }
}

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
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEarlyInit (PcieRpFuncNumArray, PcieRpFuncNumArraySize);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEarlyInit (PcieRpFuncNumArray, PcieRpFuncNumArraySize);
  }
}

/**
  Disable PCIe Root Port at PSF level

  @param[in]  RpIndex         PCIe Root Port Index (0 based)
**/
VOID
PtlPcdPsfDisablePcieRootPort (
  IN UINT32  RpIndex
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisablePcieRootPort (RpIndex);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisablePcieRootPort (RpIndex);
  }
}

/**
  Enable PCIe Root Port at PSF level

  @param[in]  RpIndex         PCIe Root Port Index (0 based)
**/
VOID
PtlPcdPsfEnablePcieRootPort (
  IN UINT32  RpIndex
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnablePcieRootPort (RpIndex);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnablePcieRootPort (RpIndex);
  }
}

/**
  Program Root Port as PSF EOI target
  @param[in] RpIndex   Root Port number
**/
VOID
PtlPcdPsfEnableRpEoiTarget (
  UINT32  RpIndex
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnableRpEoiTarget (RpIndex);
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnableRpEoiTarget (RpIndex);
  }
}

/**
  Disable Trunk Clock Gating, Local Clock Gating and Partition Clock Gating
  for all PSFs in PSF topology
**/
VOID
PtlPcdPsfDisableClockGating (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfDisableClockGating ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfDisableClockGating ();
  }
}

/**
  Enable Trunk Clock Gating, Local Clock Gating and Partition Clock Gating
  for all PSFs in PSF topology
**/
VOID
PtlPcdPsfEnableClockGating (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfEnableClockGating ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfEnableClockGating ();
  }
}

/**
  Configure PSF power management.
  Must be called after all PSF configuration is completed.
**/
VOID
PtlPcdPsfConfigurePowerManagement (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    PtlPcdPPsfConfigurePowerManagement ();
  } else if (PtlIsPcdH ()) {
    PtlPcdHPsfConfigurePowerManagement ();
  }
}