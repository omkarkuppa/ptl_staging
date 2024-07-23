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
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <PsfConfig.h>
#include <PcdSbPortIds.h>

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_CONTROLLER P2SbDev0 = {
  PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           PCI_DEVICE_NUMBER_PCH_P2SB,
           PCI_FUNCTION_NUMBER_PCH_P2SB,
           0
           ),
  PCH_PCR_BASE_ADDRESS,
  TRUE
  };

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_CONTROLLER P2SbDev1 = {
  PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           PCI_DEVICE_NUMBER_SECOND_P2SB,
           PCI_FUNCTION_NUMBER_SECOND_P2SB,
           0
           ),
  PCH_PCR_SECOND_BASE_ADDRESS,
  TRUE
  };

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_SIDEBAND_REGISTER_ACCESS Psf0Access = {
  P2SB_REGISTER_ACCESS_INIT,
  P2SbMmioAccess,
  GET_P2SB_LOCAL_PID(PTL_SID_F3_PID_PSF0),
  0,
  0,
  P2SbPrivateConfig,
  FALSE,
  &P2SbDev1
  };

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_SIDEBAND_REGISTER_ACCESS Psf4Access = {
  P2SB_REGISTER_ACCESS_INIT,
  P2SbMmioAccess,
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_PSF4),
  0,
  0,
  P2SbPrivateConfig,
  FALSE,
  &P2SbDev0
  };

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_SIDEBAND_REGISTER_ACCESS Psf5Access = {
  P2SB_REGISTER_ACCESS_INIT,
  P2SbMmioAccess,
  GET_P2SB_LOCAL_PID(PTL_H_SID_F2_PID_PSF5),
  0,
  0,
  P2SbPrivateConfig,
  FALSE,
  &P2SbDev0
  };

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_SIDEBAND_REGISTER_ACCESS Psf6Access = {
  P2SB_REGISTER_ACCESS_INIT,
  P2SbMmioAccess,
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_PSF6),
  0,
  0,
  P2SbPrivateConfig,
  FALSE,
  &P2SbDev0
  };

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_SIDEBAND_REGISTER_ACCESS Psf8Access = {
  P2SB_REGISTER_ACCESS_INIT,
  P2SbMmioAccess,
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_PSF8),
  0,
  0,
  P2SbPrivateConfig,
  FALSE,
  &P2SbDev0
  };

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_SIDEBAND_REGISTER_ACCESS Psf14Access = {
  P2SB_REGISTER_ACCESS_INIT,
  P2SbMmioAccess,
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_PSF14),
  0,
  0,
  P2SbPrivateConfig,
  FALSE,
  &P2SbDev0
  };

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_SIDEBAND_REGISTER_ACCESS Psf15Access = {
  P2SB_REGISTER_ACCESS_INIT,
  P2SbMmioAccess,
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_PSF15),
  0,
  0,
  P2SbPrivateConfig,
  FALSE,
  &P2SbDev0
  };

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_SIDEBAND_REGISTER_ACCESS Psf11Access = {
  P2SB_REGISTER_ACCESS_INIT,
  P2SbMmioAccess,
  GET_P2SB_LOCAL_PID(PTL_SID_F3_PID_TC_PSFTOP),
  0,
  0,
  P2SbPrivateConfig,
  FALSE,
  &P2SbDev1
  };

GLOBAL_REMOVE_IF_UNREFERENCED P2SB_SIDEBAND_REGISTER_ACCESS Psf12Access = {
  P2SB_REGISTER_ACCESS_INIT,
  P2SbMmioAccess,
  GET_P2SB_LOCAL_PID(PTL_SID_F3_PID_TC_PSF),
  0,
  0,
  P2SbPrivateConfig,
  FALSE,
  &P2SbDev1
  };

GLOBAL_REMOVE_IF_UNREFERENCED PSF_DEV  Psf0Dev = {
  &Psf0Access.Access,
  FALSE
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_DEV  Psf4Dev = {
  &Psf4Access.Access,
  FALSE
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_DEV  Psf5Dev = {
  &Psf5Access.Access,
  FALSE
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_DEV  Psf6Dev = {
  &Psf6Access.Access,
  FALSE
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_DEV  Psf8Dev = {
  &Psf8Access.Access,
  FALSE
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_DEV  Psf14Dev = {
  &Psf14Access.Access,
  TRUE
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_DEV  Psf15Dev = {
  &Psf15Access.Access,
  FALSE
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_DEV  Psf11TcssDev = {
  &Psf11Access.Access,
  FALSE
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_DEV  Psf12TcssDev = {
  &Psf12Access.Access,
  FALSE
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT mPchSerialIoI2cPsfPort[] =
{
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F0_OFFSET3 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F1_OFFSET4 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F2_OFFSET5 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F3_OFFSET6 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D25_F0_OFFSET7 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D25_F1_OFFSET8 }
};

/**
  Disable LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPsfDisableLpssI2c (
  IN UINT32  I2cNum
  )
{
  if (I2cNum >= ARRAY_SIZE (mPchSerialIoI2cPsfPort)) {
    return;
  }
  PsfDisableDevice (mPchSerialIoI2cPsfPort[I2cNum]);
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
  if (I2cNum >= ARRAY_SIZE (mPchSerialIoI2cPsfPort)) {
    return;
  }
  PsfHideDevice (mPchSerialIoI2cPsfPort[I2cNum]);
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
  if (I2cNum >= ARRAY_SIZE (mPchSerialIoI2cPsfPort)) {
    return;
  }
  PsfUnhideDevice (mPchSerialIoI2cPsfPort[I2cNum]);
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
  if (I2cNum >= ARRAY_SIZE (mPchSerialIoI2cPsfPort)) {
    return;
  }
  PsfDisableDeviceBar (mPchSerialIoI2cPsfPort[I2cNum], (BIT3 | BIT2));
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
  PSF_PORT I3cPsfPort;

  if ((I3cNum > 2) || (I3cNum == 1)) {
    return;
  }
  I3cPsfPort.PsfDev = &Psf8Dev;
  if (I3cNum == 0) {
    I3cPsfPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F0_OFFSET9;
  } else if (I3cNum == 2) {
    I3cPsfPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F1_OFFSET10;
  }
  PsfDisableDevice (I3cPsfPort);
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
  PSF_PORT I3cPsfPort;

  if ((I3cNum > 2) || (I3cNum == 1)) {
    return;
  }
  I3cPsfPort.PsfDev = &Psf8Dev;
  if (I3cNum == 0) {
    I3cPsfPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F0_OFFSET9;
  } else if (I3cNum == 2) {
    I3cPsfPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F1_OFFSET10;
  }
  PsfHideDevice (I3cPsfPort);
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
  PSF_PORT I3cPsfPort;

  if ((I3cNum > 2) || (I3cNum == 1)) {
    return;
  }
  I3cPsfPort.PsfDev = &Psf8Dev;
  if (I3cNum == 0) {
    I3cPsfPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F0_OFFSET9;
  } else if (I3cNum == 2) {
    I3cPsfPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F1_OFFSET10;
  }
  PsfUnhideDevice (I3cPsfPort);
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
  PSF_PORT I3cPsfPort;

  if ((I3cNum > 2) || (I3cNum == 1)) {
    return;
  }
  I3cPsfPort.PsfDev = &Psf8Dev;
  if (I3cNum == 0) {
    I3cPsfPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F0_OFFSET9;
  } else if (I3cNum == 2) {
    I3cPsfPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F1_OFFSET10;
  }
  PsfDisableDeviceBar (I3cPsfPort, (BIT3 | BIT2));
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT mPchSerialIoSpiPsfPort[] =
{
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F2_OFFSET11 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F3_OFFSET12 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D18_F6_OFFSET13 }
};

/**
  Disable LPSS SPI at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdPsfDisableLpssSpi (
  IN UINT32  SpiNum
  )
{
  if (SpiNum >= ARRAY_SIZE (mPchSerialIoSpiPsfPort)) {
    DEBUG ((DEBUG_WARN, "%a - wrong SPI number, bust be lesser than %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPchSerialIoSpiPsfPort), SpiNum));
    return;
  }

  PsfDisableDevice (mPchSerialIoSpiPsfPort[SpiNum]);
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
  if (SpiNum >= ARRAY_SIZE (mPchSerialIoSpiPsfPort)) {
    return;
  }
  PsfHideDevice (mPchSerialIoSpiPsfPort[SpiNum]);
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
  if (SpiNum >= ARRAY_SIZE (mPchSerialIoSpiPsfPort)) {
    DEBUG ((DEBUG_WARN, "%a - wrong SPI number, bust be lesser than %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPchSerialIoSpiPsfPort), SpiNum));
    return;
  }

  PsfUnhideDevice (mPchSerialIoSpiPsfPort[SpiNum]);
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
  if (SpiNum >= ARRAY_SIZE (mPchSerialIoSpiPsfPort)) {
    DEBUG ((DEBUG_WARN, "%a - wrong SPI number, bust be lesser than %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPchSerialIoSpiPsfPort), SpiNum));
    return;
  }

  PsfDisableDeviceBar (mPchSerialIoSpiPsfPort[SpiNum], BIT3 | BIT2);
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT mPchSerialIoUartPsfPort[] =
{
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F0_OFFSET14 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F1_OFFSET15 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D25_F2_OFFSET16 }
};

/**
  Disable LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPsfDisableLpssUart (
  IN UINT32  UartNum
  )
{
  if (UartNum >= ARRAY_SIZE (mPchSerialIoUartPsfPort)) {
    DEBUG ((DEBUG_WARN, "%a - wrong UART number, bust be lesser than %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPchSerialIoUartPsfPort), UartNum));
    return;
  }

  PsfDisableDevice (mPchSerialIoUartPsfPort[UartNum]);
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
  if (UartNum >= ARRAY_SIZE (mPchSerialIoUartPsfPort)) {
    DEBUG ((DEBUG_WARN, "%a - wrong UART number, bust be lesser than %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPchSerialIoUartPsfPort), UartNum));
    return;
  }

  PsfEnableDevice (mPchSerialIoUartPsfPort[UartNum]);
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
  if (UartNum >= ARRAY_SIZE (mPchSerialIoUartPsfPort)) {
    DEBUG ((DEBUG_WARN, "%a - wrong UART number, bust be lesser than %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPchSerialIoUartPsfPort), UartNum));
    return;
  }

  PsfUnhideDevice (mPchSerialIoUartPsfPort[UartNum]);
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
  if (UartNum >= ARRAY_SIZE (mPchSerialIoUartPsfPort)) {
    DEBUG ((DEBUG_WARN, "%a - wrong UART number, bust be lesser than %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPchSerialIoUartPsfPort), UartNum));
    return;
  }

  PsfHideDevice (mPchSerialIoUartPsfPort[UartNum]);
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
  if (UartNum >= ARRAY_SIZE (mPchSerialIoUartPsfPort)) {
    DEBUG ((DEBUG_WARN, "%a - wrong UART number, bust be lesser than %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPchSerialIoUartPsfPort), UartNum));
    return;
  }

  PsfDisableDeviceBar (mPchSerialIoUartPsfPort[UartNum], BIT3 | BIT2);
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
  if (UartNum >= ARRAY_SIZE (mPchSerialIoUartPsfPort)) {
    DEBUG ((DEBUG_WARN, "%a - wrong UART number, bust be lesser than %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPchSerialIoUartPsfPort), UartNum));
    return;
  }

  PsfEnableDeviceBar (mPchSerialIoUartPsfPort[UartNum], BIT3 | BIT2);
}

/**
  Disable TraceHub at PSF level
**/
VOID
PtlPcdPsfDisableTraceHub (
  VOID
  )
{
  PSF_PORT TraceHubPort;

  TraceHubPort.PsfDev = &Psf6Dev;
  TraceHubPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_NPK_RS0_D31_F7_OFFSET5;

  PsfDisableDevice (TraceHubPort);
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
  PSF_PORT TraceHubAcpiPort;

  TraceHubAcpiPort.PsfDev = &Psf6Dev;
  TraceHubAcpiPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_NPK_RS0_D17_F2_OFFSET4;

  PsfSetDeviceBarValue (TraceHubAcpiPort, BarNum, BarValue);
}

/**
  Enable TraceHub ACPI Memory Space at PSF level
**/
VOID
PtlPcdPsfEnableTraceHubAcpiMemSpace (
  VOID
  )
{
  PSF_PORT TraceHubAcpiPort;

  TraceHubAcpiPort.PsfDev = &Psf6Dev;
  TraceHubAcpiPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_NPK_RS0_D17_F2_OFFSET4;

  PsfEnableDeviceMemSpace (TraceHubAcpiPort);
}

/**
  Hide TraceHub ACPI at PSF level
**/
VOID
PtlPcdHideTraceHubAcpi (
  VOID
  )
{
  PSF_PORT TraceHubAcpiPort;

  TraceHubAcpiPort.PsfDev = &Psf6Dev;
  TraceHubAcpiPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_NPK_RS0_D17_F2_OFFSET4;

  PsfHideDevice (TraceHubAcpiPort);
}

/**
  Disable TraceHub ACPI at PSF level
**/
VOID
PtlPcdDisableTraceHubAcpi (
  VOID
  )
{
  PSF_PORT TraceHubAcpiPort;

  TraceHubAcpiPort.PsfDev = &Psf6Dev;
  TraceHubAcpiPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_NPK_RS0_D17_F2_OFFSET4;

  PsfDisableDevice (TraceHubAcpiPort);
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchHeciPsfRegBase[] =
{
  R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F0_OFFSET9,
  R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F1_OFFSET10,
  R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F4_OFFSET11,
  R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F5_OFFSET12
};

/**
  Enable HECI at PSF level

  @param[in] HeciNumber  HECI Number (1-4)
**/
VOID
PtlPcdPsfEnableHeci (
  IN  UINT8     HeciNumber
  )
{
  PSF_PORT HeciPort;

  if ((HeciNumber == 0) || (HeciNumber > 4)) {
    DEBUG ((DEBUG_INFO, "%a - wrong HeciNumber: should be 1-4, is %d\n", __FUNCTION__, HeciNumber));
    return;
  }
  HeciPort.PsfDev = &Psf6Dev;
  HeciPort.RegBase = mPchHeciPsfRegBase[HeciNumber - 1];

  PsfEnableDevice (HeciPort);
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
  PSF_PORT HeciPort;

  if ((HeciNumber == 0) || (HeciNumber > 4)) {
    DEBUG ((DEBUG_INFO, "%a - wrong HeciNumber: should be 1-4, is %d\n", __FUNCTION__, HeciNumber));
    return;
  }
  HeciPort.PsfDev = &Psf6Dev;
  HeciPort.RegBase = mPchHeciPsfRegBase[HeciNumber - 1];

  PsfDisableDevice (HeciPort);
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
  PSF_PORT SsePort;
  UINT16 mPchSseHeciPsfRegBase[] =
  {
    R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ESE_RS0_D24_F0_OFFSET16,
    R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ESE_RS0_D24_F1_OFFSET17,
    R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ESE_RS0_D24_F2_OFFSET18
  };

  if ((SseNumber == 0) || (SseNumber > ARRAY_SIZE (mPchSseHeciPsfRegBase))) {
    DEBUG ((DEBUG_INFO, "%a - wrong SseNumber: should be 1-%d, is %d\n", __FUNCTION__, ARRAY_SIZE (mPchSseHeciPsfRegBase), SseNumber));
    return;
  }

  SsePort.PsfDev = &Psf6Dev;
  SsePort.RegBase = mPchSseHeciPsfRegBase[SseNumber - 1];

  PsfDisableDevice (SsePort);
}

/**
  Enable SOL at PSF level
**/
VOID
PtlPcdPsfEnableSol (
  VOID
  )
{
  PSF_PORT SolPort;

  SolPort.PsfDev = &Psf6Dev;
  SolPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_PTIO_RS0_D22_F3_OFFSET27;

  PsfEnableDevice (SolPort);
}

/**
  Disable SOL at PSF level
**/
VOID
PtlPcdPsfDisableSol (
  VOID
  )
{
  PSF_PORT SolPort;

  SolPort.PsfDev = &Psf6Dev;
  SolPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_PTIO_RS0_D22_F3_OFFSET27;

  PsfDisableDevice (SolPort);
}

/**
  Disable IDER device at PSF level
**/
VOID
PtlPcdPsfDisableIder (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfDev = &Psf6Dev;
  PsfPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_PTIO_RS0_D22_F2_OFFSET26;
  PsfDisableDevice (PsfPort);
}

/**
  Disable HDAudio device at PSF level
**/
VOID
PtlPcdPsfDisableAce (
  VOID
  )
{
  PSF_PORT PsfPort;
    PsfPort.PsfDev = &Psf6Dev;
    PsfPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ACE_RS0_D31_F3_OFFSET28;
  PsfDisableDevice (PsfPort);
}

/**
  Disable DSP Bar (HdAudio BAR4 and BAR5) at PSF level.
**/
VOID
PtlPcdPsfDisableDspBar (
  VOID
  )
{
  PSF_PORT PsfPort;
    PsfPort.PsfDev = &Psf6Dev;
    PsfPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ACE_RS0_D31_F3_OFFSET28;

  PsfDisableDeviceBar (PsfPort, (BIT5 | BIT4));
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchThcPsfRegBase[] =
{
  R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_THC0_RS0_D16_F0_OFFSET29,
  R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_THC1_RS0_D16_F1_OFFSET30
};

/**
  Disable THC device at PSF level

  @param[in]  ThcNumber                Touch Host Controller Number THC0 or THC1
**/
VOID
PtlPcdPsfDisableThc (
  IN  UINT32        ThcNumber
  )
{
  PSF_PORT ThcPort;

  if (ThcNumber >= ARRAY_SIZE (mPchThcPsfRegBase)) {
    DEBUG ((DEBUG_WARN, "Wrong THC number!\n"));
    ASSERT (FALSE);
    return;
  }

  ThcPort.PsfDev = &Psf8Dev;
  ThcPort.RegBase = mPchThcPsfRegBase[ThcNumber];
  PsfDisableDevice (ThcPort);
}

/**
  Disable xDCI device at PSF level
**/
VOID
PtlPcdPsfDisableXdci (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfDev = &Psf8Dev;
  PsfPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_XDCI_RS0_D20_F1_OFFSET31;
  PsfDisableDevice (PsfPort);
}

/**
  Disable ISH at PSF level
**/
VOID
PtlPcdPsfDisableIsh (
  VOID
  )
{
  PSF_PORT IshPort;
    IshPort.PsfDev = &Psf0Dev;
    IshPort.RegBase = R_PTL_PCD_P_H_PSF_0_AGNT_T0_SHDW_BAR0_ISH_RS0_D18_F0_OFFSET2;

  PsfDisableDevice (IshPort);
}

/**
  Disable ISH BAR1 at PSF level
**/
VOID
PtlPcdPsfDisableIshBar1 (
  VOID
  )
{
  PSF_PORT IshPort;
    IshPort.PsfDev = &Psf0Dev;
    IshPort.RegBase = R_PTL_PCD_P_H_PSF_0_AGNT_T0_SHDW_BAR0_ISH_RS0_D18_F0_OFFSET2;

  PsfDisableDeviceBar (IshPort, (BIT3 | BIT2));
}

/**
  Disable GbE device at PSF level
**/
VOID
PtlPcdPsfDisableGbe (
  VOID
  )
{
  PSF_PORT GbePort;
    GbePort.PsfDev = &Psf6Dev;
    GbePort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_GBE_RS0_D31_F6_OFFSET32;

  PsfDisableDevice (GbePort);
}

/**
  Disable SMBUS device at PSF level
**/
VOID
PtlPcdPsfDisableSmbus (
  VOID
  )
{
  PSF_PORT SmbusPort;

  SmbusPort.PsfDev = &Psf8Dev;
  SmbusPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_SMB_RS0_D31_F4_OFFSET24;

  PsfDisableDevice (SmbusPort);
}

UINT16 mPtlPcdPsfScsUfsRegs[] =
{
  R_PTL_PCD_P_PSF_0_AGNT_T0_SHDW_BAR0_UFS_RS0_D23_F0_OFFSET4
};


/**
  Disable SCS UFS at PSF level

  @param[in] UfsNumber   UFS number
**/
VOID
PtlPcdPsfDisableScsUfs (
  IN UINT32  UfsNumber
  )
{
  PSF_PORT UfsPort;
    if (UfsNumber >= ARRAY_SIZE (mPtlPcdPsfScsUfsRegs)) {
      DEBUG ((DEBUG_WARN, "%a - wrong UfsNumber: max %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPtlPcdPsfScsUfsRegs) - 1, UfsNumber));
      return;
    }
    UfsPort.PsfDev = &Psf0Dev;
    UfsPort.RegBase = mPtlPcdPsfScsUfsRegs[UfsNumber];

  PsfDisableDevice (UfsPort);
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
  PSF_PORT UfsPort;
    if (UfsNumber >= ARRAY_SIZE (mPtlPcdPsfScsUfsRegs)) {
      DEBUG ((DEBUG_WARN, "%a - wrong UfsNumber: max %d, given %d\n", __FUNCTION__, ARRAY_SIZE (mPtlPcdPsfScsUfsRegs) - 1, UfsNumber));
      return;
    }
    UfsPort.PsfDev = &Psf0Dev;
    UfsPort.RegBase = mPtlPcdPsfScsUfsRegs[UfsNumber];
  PsfDisableDeviceBar (UfsPort, (BIT3 | BIT2));
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_REG_BASE mPtlPcdPHPPsfPcieRegBase[] =
{
  { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F0_OFFSET64 },
  { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F1_OFFSET65 },
  { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F2_OFFSET66 },
  { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F3_OFFSET67 },
  { 4, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F4_OFFSET64 },
  { 4, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F5_OFFSET65 },
  { 4, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F6_OFFSET66 },
  { 4, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F7_OFFSET67 }
};


PSF_PCIE_PORT_DATA_TABLE mPtlPchPHPsfPciePortData = PSF_PCIE_PORT_DATA_TABLE_INIT (
//{ PsfNumber, SecondLevelPsfNumber, RootPciePort, RootRs3Port, SecondLevelPort, RootFunctionConfigPort, RootRs3FunctionConfigPort, RootPortFunctionConfig2ndLvlPort}
  { 6, 0, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F0_OFFSET64, 0, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F0_OFFSET68, R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F0_OFFSET64, 0, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F0_OFFSET68 },
  { 6, 0, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F1_OFFSET65, 0, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F1_OFFSET69, R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F1_OFFSET65, 0, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F1_OFFSET69 },
  { 6, 0, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F2_OFFSET66, 0, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F2_OFFSET70, R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F2_OFFSET66, 0, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F2_OFFSET70 },
  { 6, 0, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F3_OFFSET67, 0, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F3_OFFSET71, R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F3_OFFSET67, 0, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F3_OFFSET71 },
  { 4, 0, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F4_OFFSET64, 0, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F4_OFFSET64, R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F4_OFFSET64, 0, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F4_OFFSET64 },
  { 4, 0, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F5_OFFSET65, 0, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F5_OFFSET65, R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F5_OFFSET65, 0, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F5_OFFSET65 },
  { 4, 0, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F6_OFFSET66, 0, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F6_OFFSET66, R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F6_OFFSET66, 0, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F6_OFFSET66 },
  { 4, 0, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F7_OFFSET67, 0, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F7_OFFSET67, R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F7_OFFSET67, 0, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F7_OFFSET67 }
);


/**
  Return second level PSF_PORT to which PCIE Root Port device is connected (directly)

  @param[in]  RpIndex         PCIe Root Port Index (0 based)
  @retval     PciePortData    PCIE PORT Data
**/
STATIC
PSF_PCIE_PORT_DATA*
PtlPcdPsfGetPciePortData (
  IN UINT32  RpIndex
  )
{
  if (RpIndex < mPtlPchPHPsfPciePortData.Size) {
    return &mPtlPchPHPsfPciePortData.Data[RpIndex];
  }
  return NULL;
}


PSF_REG_DATA_TABLE mPtlPcdPHEoiRegData = PSF_REG_DATA_TABLE_INIT (
  { 0, R_PTL_PCD_P_H_PSF_0_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI, R_PTL_PCD_P_H_PSF_0_PSF_MC_CONTROL_MCAST0_RS0_EOI, 4 },
  { 4, R_PTL_PCD_P_H_PSF_4_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI, R_PTL_PCD_P_H_PSF_4_PSF_MC_CONTROL_MCAST0_RS0_EOI, 4 },
  { 6, R_PTL_PCD_P_H_PSF_6_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI, R_PTL_PCD_P_H_PSF_6_PSF_MC_CONTROL_MCAST0_RS0_EOI, 9 },
  { 8, R_PTL_PCD_P_H_PSF_8_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI, R_PTL_PCD_P_H_PSF_8_PSF_MC_CONTROL_MCAST0_RS0_EOI, 1 }
);


/**
  Get EOI register data table for all PSFs

  @return PsfEoiRegDataTable   Pointer to PSF_EOI_REG_DATA_TABLE
**/
STATIC
PSF_REG_DATA_TABLE*
PtlPcdPsfGetEoiRegDataTable (
  VOID
  )
{
    return &mPtlPcdPHEoiRegData;
}

PSF_REG_DATA_TABLE mPtlPcdPHMctpRegDataTable = PSF_REG_DATA_TABLE_INIT (
  { 0, R_PTL_PCD_P_H_PSF_0_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1, R_PTL_PCD_P_H_PSF_0_PSF_MC_CONTROL_MCAST1_RS0_MCTP1, 4, 0, TRUE, 6 },
  { 4, R_PTL_PCD_P_H_PSF_4_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1, R_PTL_PCD_P_H_PSF_4_PSF_MC_CONTROL_MCAST1_RS0_MCTP1, 4, 0, TRUE, 0 },
  { 6, R_PTL_PCD_P_H_PSF_6_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1, R_PTL_PCD_P_H_PSF_6_PSF_MC_CONTROL_MCAST1_RS0_MCTP1, 9, 0, FALSE, 0 }
);


/**
  Get MCTP register data table for all PSFs

  @return PsfEoiRegDataTable   Pointer to PSF_EOI_REG_DATA_TABLE
**/
STATIC
PSF_REG_DATA_TABLE*
PtlPcdPsfGetMctpRegDataTable (
  VOID
  )
{
    return &mPtlPcdPHMctpRegDataTable;
}

/**
  Fill MCTP Targets Table

  @param[out] TargetIdTable    MCTP Targets table
  @param[in]  MaxTableSize     TargetIdTable real size
  @param[in]  PsfTable         PSF Segment Table
  @param[in]  PcieRegBaseTable Pcie Reg Base Table

  @retval Number of targets, resulting size of the table
**/
STATIC
UINT32
PtlPcdMctpTargetsTable (
  OUT PSF_PORT_DEST_ID  *TargetIdTable,
  IN  UINT32            MaxTableSize,
  IN  PSF_SEGMENT_TABLE *PsfTable,
  IN  PSF_REG_BASE      *PcieRegBaseTable
  )
{
  UINT32       TargetNum;
  UINT32       RpIndex;
  PSF_PORT     PciePsfPort;
  UINT32       MaxPciePortNum;

  if (PsfTable == NULL || PcieRegBaseTable == NULL) {
    return 0;
  }

  TargetNum = 0;
  ZeroMem (TargetIdTable, sizeof(PSF_PORT_DEST_ID) * MaxTableSize);

  if ((UINT32)(GetPchMaxPciePortNum ()) > MaxTableSize) {
    DEBUG ((DEBUG_ERROR, "Cannot create PtlPcdMctpTargetsTable - table size is too small!\n"));
    return 0;
  }
    MaxPciePortNum = ARRAY_SIZE (mPtlPcdPHPPsfPcieRegBase);

  for (RpIndex = 0; RpIndex < MaxPciePortNum; RpIndex++) {
    PciePsfPort.RegBase = PcieRegBaseTable[RpIndex].RegBase;
    PciePsfPort.PsfDev = PsfGetDev (PsfTable, PcieRegBaseTable[RpIndex].PsfNumber);
    if (PsfIsBridgeEnabled (PciePsfPort)) {
      TargetIdTable[TargetNum] = PtlPcdPsfPcieDestinationId (RpIndex);
      TargetNum++;
    }
  }
  //
  // Add IOC Destination Id
  //
  TargetIdTable[TargetNum] = (PSF_PORT_DEST_ID){ 0x0001 };
  TargetNum++;

  return TargetNum;
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT_DEST_ID mPtlPcdPHRpDestId[] =
{
  {0x68000}, {0x68002}, {0x68004}, {0x68006}, // PXPA: PSF6, PortID = 0, PortGroupID = 1
  {0x48100}, {0x48102}, {0x48104}, {0x48106}, // PXPB: PSF4, PortID = 1, PortGroupID = 1
  {0x48000}, {0x48002}                        // PXPC: PSF4, PortID = 0, PortGroupID = 1
};


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
    if (RpIndex < ARRAY_SIZE (mPtlPcdPHRpDestId)) {
      return mPtlPcdPHRpDestId[RpIndex];
    }

  ASSERT (FALSE);
  return (PSF_PORT_DEST_ID){0};
}

/**
  Disable CNVi at PSF level
**/
VOID
PtlPcdPsfDisableCnvi (
  VOID
  )
{
  PSF_PORT CnviPort;

  CnviPort.PsfDev = &Psf6Dev;
    CnviPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_WIFI_RS0_D20_F3_OFFSET34;

  PsfDisableDevice (CnviPort);
}

/**
  Disable CNVi Bluetooth(R) at PSF level
**/
VOID
PtlPcdPsfDisableCnviBt (
  VOID
  )
{
  PSF_PORT BtPort;

  BtPort.PsfDev = &Psf6Dev;
    BtPort.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_BT_RS0_D20_F7_OFFSET30;

  PsfDisableDevice (BtPort);
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
  PSF_PORT PmcPort;

  PmcPort.PsfDev = &Psf8Dev;
  PmcPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_PMC_RS0_D31_F2_OFFSET21;

  PsfSetPmcAbase (PmcPort, Abase);
}

/**
  Hide PMC at PSF level
**/
VOID
PtlPcdPsfHidePmc (
  VOID
  )
{
  PSF_PORT PmcPort;

  PmcPort.PsfDev = &Psf8Dev;
  PmcPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_PMC_RS0_D31_F2_OFFSET21;

  PsfHideDevice (PmcPort);
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
  PSF_PORT PmcPort;

  PmcPort.PsfDev = &Psf8Dev;
  PmcPort.RegBase = R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_PMC_RS0_D31_F2_OFFSET21;

  return PsfGetPmcPwrmBase (PmcPort);
}

PSF_SEGMENT_TABLE mPtlPcdPSegmentTable = PSF_SEGMENT_TABLE_INIT (
  {  0, &Psf0Dev  },
  {  4, &Psf4Dev  },
  {  6, &Psf6Dev  },
  {  8, &Psf8Dev  },
  { 14, &Psf14Dev },
  { 15, &Psf15Dev }
);

PSF_SEGMENT_TABLE mPtlPcdHSegmentTable = PSF_SEGMENT_TABLE_INIT (
  {  0, &Psf0Dev  },
  {  4, &Psf4Dev  },
  {  5, &Psf5Dev  },
  {  6, &Psf6Dev  },
  {  8, &Psf8Dev  },
  { 14, &Psf14Dev },
  { 15, &Psf15Dev }
);

/**
  Get table of supported PSF segments

  @return  PsfSegmentTable   Table of supported PSF segments
**/
PSF_SEGMENT_TABLE*
PtlPcdPsfGetSegmentTable (
  VOID
  )
{
  if (PtlIsPcdP ()) {
    return &mPtlPcdPSegmentTable;
  } else if (PtlIsPcdH ()) {
    return &mPtlPcdHSegmentTable;
  }
  return NULL;
}

PSF_PORT_RELAXED_ORDERING_CONFIG_REG  mPtlPcdPPsfPortRelaxedOrderingConfigRegs[] =
{
  { &Psf0Dev, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf0Dev, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT1, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf0Dev, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT2, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf0Dev, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT3, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf0Dev, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT4, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },

  { &Psf4Dev, R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG1_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf4Dev, R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG1_PORT1, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf4Dev, R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG1_PORT2, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },

  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT1, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT2, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT3, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT4, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT5, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT6, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT7, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT8, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },

  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG0_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO },
  { &Psf14Dev, R_PTL_PCD_P_H_PSF_14_PSF_PORT_CONFIG_PG0_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO },
  { &Psf15Dev, R_PTL_PCD_P_H_PSF_15_PSF_PORT_CONFIG_PG0_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_PSF_PORT_CONFIG_PG0_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO },
  { &Psf4Dev, R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG0_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO },
  { &Psf0Dev, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG0_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO }
};


PSF_PORT_RELAXED_ORDERING_CONFIG_REG  mPtlPcdHPsfPortRelaxedOrderingConfigRegs[] =
{
  { &Psf5Dev, R_PTL_PCD_H_PSF_5_PSF_PORT_CONFIG_PG1_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },
  { &Psf5Dev, R_PTL_PCD_H_PSF_5_PSF_PORT_CONFIG_PG1_PORT1, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO },

  { &Psf5Dev, R_PTL_PCD_H_PSF_5_PSF_PORT_CONFIG_PG0_PORT0, B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO }
};

PSF_RELAXED_ORDER_REGS mPsfPortRelaxedOrderingConfigRegsStruct;

/**
  Enable PCIe Relaxed Ordering in PSF
**/
VOID
PtlPcdPsfEnablePcieRelaxedOrder (
  VOID
  )
{
    mPsfPortRelaxedOrderingConfigRegsStruct.RegsTable = mPtlPcdPPsfPortRelaxedOrderingConfigRegs;
    mPsfPortRelaxedOrderingConfigRegsStruct.RegsTableSize = ARRAY_SIZE (mPtlPcdPPsfPortRelaxedOrderingConfigRegs);
    if (PtlIsPcdH ()) {
      mPsfPortRelaxedOrderingConfigRegsStruct.RegsPchTypeSpecific = mPtlPcdHPsfPortRelaxedOrderingConfigRegs;
      mPsfPortRelaxedOrderingConfigRegsStruct.RegsPchTypeSpecificTableSize = ARRAY_SIZE (mPtlPcdHPsfPortRelaxedOrderingConfigRegs);
    } else {
      mPsfPortRelaxedOrderingConfigRegsStruct.RegsPchTypeSpecific = NULL;
      mPsfPortRelaxedOrderingConfigRegsStruct.RegsPchTypeSpecificTableSize = 0;
    }

  PsfEnablePcieRelaxedOrder (&mPsfPortRelaxedOrderingConfigRegsStruct);
}

/**
  Return RC_OWNER value to program

  @retval RC_OWNER
**/
STATIC
UINT32
PtlPcdPsfGetRcOwner (
  )
{
  PSF_PORT_DEST_ID Pmt = {
    .Fields = {
      .ChannelId = 1,
      .PortId = 2,
      .PortGroupId = 0,
      .PsfId = 15
    }
  };
  return Pmt.RegVal;
}

//
// This constant should be at least equal to GetPchMaxPciePortNum ()
//
#define PTL_PCD_MAX_MCTP_TARGET_TABLE_ENTRIES 12
/**
  Configure MCTP
**/
VOID
PtlPcdMctpConfigure (
  VOID
  )
{
  PSF_PORT_DEST_ID    PcdMctpTargetIdTable[PTL_PCD_MAX_MCTP_TARGET_TABLE_ENTRIES];
  UINT32              PcdMctpTargetIdTableSize;
    PcdMctpTargetIdTableSize = PtlPcdMctpTargetsTable (
                               PcdMctpTargetIdTable,
                               PTL_PCD_MAX_MCTP_TARGET_TABLE_ENTRIES,
                               PtlPcdPsfGetSegmentTable (),
                               mPtlPcdPHPPsfPcieRegBase
                               );
  PsfConfigureMctpCycle (
    PcdMctpTargetIdTable,
    PcdMctpTargetIdTableSize,
    PtlPcdPsfGetSegmentTable (),
    PtlPcdPsfGetMctpRegDataTable (),
    PtlPcdPsfGetRcOwner ()
    );
}

PSF_PORT mPtlPcdPPsfRootspaceConfigs[] = {
  { &Psf0Dev, R_PTL_PCD_P_H_PSF_0_ROOTSPACE_CONFIG_RS0 },
  { &Psf0Dev, R_PTL_PCD_P_H_PSF_0_ROOTSPACE_CONFIG_RS3 },
  { &Psf4Dev, R_PTL_PCD_P_H_PSF_4_ROOTSPACE_CONFIG_RS0 },
  { &Psf4Dev, R_PTL_PCD_P_H_PSF_4_ROOTSPACE_CONFIG_RS3 },
  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_ROOTSPACE_CONFIG_RS0 },
  { &Psf6Dev, R_PTL_PCD_P_H_PSF_6_ROOTSPACE_CONFIG_RS3 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_ROOTSPACE_CONFIG_RS3 },
  { &Psf8Dev, R_PTL_PCD_P_H_PSF_8_ROOTSPACE_CONFIG_RS0 },
  { &Psf14Dev, R_PTL_PCD_P_H_PSF_14_ROOTSPACE_CONFIG_RS3 },
  { &Psf14Dev, R_PTL_PCD_P_H_PSF_14_ROOTSPACE_CONFIG_RS0 },
  { &Psf15Dev, R_PTL_PCD_P_H_PSF_15_ROOTSPACE_CONFIG_RS3 },
  { &Psf15Dev, R_PTL_PCD_P_H_PSF_15_ROOTSPACE_CONFIG_RS0 }
};

PSF_PORT mPtlPcdHPsfRootspaceConfigs[] = {
  { &Psf5Dev, R_PTL_PCD_H_PSF_5_ROOTSPACE_CONFIG_RS0 },
  { &Psf5Dev, R_PTL_PCD_H_PSF_5_ROOTSPACE_CONFIG_RS3 }
};


PSF_PORT mPtlPcdTcssPsfRootspaceConfigs[] = {
  { &Psf11TcssDev, R_PTL_PCD_P_H_TCSS_PSF_11_ROOTSPACE_CONFIG_RS0 },
  { &Psf11TcssDev, R_PTL_PCD_P_H_TCSS_PSF_11_ROOTSPACE_CONFIG_RS3 },
  { &Psf12TcssDev, R_PTL_PCD_P_H_TCSS_PSF_12_ROOTSPACE_CONFIG_RS0 },
  { &Psf12TcssDev, R_PTL_PCD_P_H_TCSS_PSF_12_ROOTSPACE_CONFIG_RS3 }
};
/**
  Enable VTd support in PSF.
**/
VOID
PtlPcdPsfEnableVtd (
  VOID
  )
{
  UINT32 Index;

  for (Index = 0; Index < ARRAY_SIZE (mPtlPcdPPsfRootspaceConfigs); Index++) {
    mPtlPcdPPsfRootspaceConfigs[Index].PsfDev->Access->AndThenOr32 (
      mPtlPcdPPsfRootspaceConfigs[Index].PsfDev->Access,
      mPtlPcdPPsfRootspaceConfigs[Index].RegBase,
      ~0u,
      B_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_VTDEN
      );
  }
  if (PtlIsPcdH ()) {
    for (Index = 0; Index < ARRAY_SIZE (mPtlPcdHPsfRootspaceConfigs); Index++) {
      mPtlPcdHPsfRootspaceConfigs[Index].PsfDev->Access->AndThenOr32 (
        mPtlPcdHPsfRootspaceConfigs[Index].PsfDev->Access,
        mPtlPcdHPsfRootspaceConfigs[Index].RegBase,
        ~0u,
        B_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_VTDEN
        );
    }
  }
 for (Index = 0; Index < ARRAY_SIZE (mPtlPcdTcssPsfRootspaceConfigs); Index++) {
    mPtlPcdTcssPsfRootspaceConfigs[Index].PsfDev->Access->AndThenOr32 (
      mPtlPcdTcssPsfRootspaceConfigs[Index].PsfDev->Access,
      mPtlPcdTcssPsfRootspaceConfigs[Index].RegBase,
      ~0u,
      B_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_VTDEN
      );
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
  UINT32 Index;

  for (Index = 0; Index < ARRAY_SIZE (mPtlPcdPPsfRootspaceConfigs); Index++) {
    mPtlPcdPPsfRootspaceConfigs[Index].PsfDev->Access->AndThenOr32 (
      mPtlPcdPPsfRootspaceConfigs[Index].PsfDev->Access,
      mPtlPcdPPsfRootspaceConfigs[Index].RegBase,
      (UINT32)~B_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_ENADDRP2P,
      0
      );
  }
  if (PtlIsPcdH ()) {
    for (Index = 0; Index < ARRAY_SIZE (mPtlPcdHPsfRootspaceConfigs); Index++) {
      mPtlPcdHPsfRootspaceConfigs[Index].PsfDev->Access->AndThenOr32 (
        mPtlPcdHPsfRootspaceConfigs[Index].PsfDev->Access,
        mPtlPcdHPsfRootspaceConfigs[Index].RegBase,
        (UINT32)~B_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_ENADDRP2P,
        0
        );
    }
  }
 for (Index = 0; Index < ARRAY_SIZE (mPtlPcdTcssPsfRootspaceConfigs); Index++) {
    mPtlPcdTcssPsfRootspaceConfigs[Index].PsfDev->Access->AndThenOr32 (
      mPtlPcdTcssPsfRootspaceConfigs[Index].PsfDev->Access,
      mPtlPcdTcssPsfRootspaceConfigs[Index].RegBase,
      (UINT32)~B_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_ENADDRP2P,
      0
      );
  }
}

/**
  Reset R_PCH_PSF_PCR_ROOTSPACE_CONFIG_RS0 and
  R_PCH_PSF_PCR_ROOTSPACE_CONFIG_RS3 registers for all PSFs
  to HW default.
  These registers are resets to defaults only after global reset,
  after any other reset type these needs to be reset by BIOS.
**/
VOID
PtlPcdPsfResetRootspaceConfig (
  VOID
  )
{
  UINT32 Index;

  for (Index = 0; Index < ARRAY_SIZE (mPtlPcdPPsfRootspaceConfigs); Index++) {
    mPtlPcdPPsfRootspaceConfigs[Index].PsfDev->Access->Write32 (
      mPtlPcdPPsfRootspaceConfigs[Index].PsfDev->Access,
      mPtlPcdPPsfRootspaceConfigs[Index].RegBase,
      (UINT32)((0 << N_PTL_PCD_P_H_PSF_PCR_ROOTSPACE_CONFIG_RSX_ENADDRP2P) | (0 << N_PTL_PCD_P_H_PSF_PCR_ROOTSPACE_CONFIG_RSX_VTDEN))
      );
  }
  if (PtlIsPcdH ()) {
    for (Index = 0; Index < ARRAY_SIZE (mPtlPcdHPsfRootspaceConfigs); Index++) {
      mPtlPcdHPsfRootspaceConfigs[Index].PsfDev->Access->Write32 (
        mPtlPcdHPsfRootspaceConfigs[Index].PsfDev->Access,
        mPtlPcdHPsfRootspaceConfigs[Index].RegBase,
        (UINT32)((0 << N_PTL_PCD_P_H_PSF_PCR_ROOTSPACE_CONFIG_RSX_ENADDRP2P) | (0 << N_PTL_PCD_P_H_PSF_PCR_ROOTSPACE_CONFIG_RSX_VTDEN))
        );
    }
  }
 for (Index = 0; Index < ARRAY_SIZE (mPtlPcdTcssPsfRootspaceConfigs); Index++) {
    mPtlPcdTcssPsfRootspaceConfigs[Index].PsfDev->Access->Write32 (
      mPtlPcdTcssPsfRootspaceConfigs[Index].PsfDev->Access,
      mPtlPcdTcssPsfRootspaceConfigs[Index].RegBase,
      (UINT32)((0 << N_PTL_PCD_P_H_PSF_PCR_ROOTSPACE_CONFIG_RSX_ENADDRP2P) | (0 << N_PTL_PCD_P_H_PSF_PCR_ROOTSPACE_CONFIG_RSX_VTDEN))
      );
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
  EFI_STATUS  Status;
  PSF_CONFIG  *PsfConfig;
  UINT32      DwbFlushThreshold;
  UINT32      NonxHCIEn;
  UINT32      OBFFEn;
  UINT32      DWBEn;
  UINT32      DwbConfigRegOffset;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPsfConfigGuid, (VOID *) &PsfConfig);
  ASSERT_EFI_ERROR (Status);
  DwbFlushThreshold = PsfConfig->DwbConfigDwbFlushThreshold;
  NonxHCIEn = PsfConfig->DwbConfigNonxHCIEn;
  OBFFEn = PsfConfig->DwbConfigOBFFEn;
  DWBEn = PsfConfig->DwbConfigDWBEn;
    DwbConfigRegOffset = R_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0;

  Psf6Dev.Access->AndThenOr32 (
    Psf6Dev.Access,
    DwbConfigRegOffset,
    (UINT32)~(B_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_FLUSHTHRESHHOLD | B_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_NONXHCIEN | B_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_OBFFEN | B_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_DWBEN),
    ((DwbFlushThreshold << N_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_FLUSHTHRESHHOLD) | (NonxHCIEn << N_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_NONXHCIEN) | (OBFFEn << N_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_OBFFEN) | (DWBEn << N_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0_DWBEN))
    );
}

//
// Grant count programming
//
#define GRANT_COUNT_REG_N_A       0xFFFF

EFI_STATUS
PtlPcdSetGrantCount (
  REGISTER_ACCESS  *Access,
  UINT32           DevGntCntUpstreamReg,
  UINT32           TgtGntCntUpstreamReg,
  UINT32           TgtGntCntDownstreamReg,
  UINT32           GrantCount
  )
{
  if (Access == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (DevGntCntUpstreamReg != GRANT_COUNT_REG_N_A) {
    Access->Write32 (
      Access,
      DevGntCntUpstreamReg,
      GrantCount
    );
  }

  if (TgtGntCntUpstreamReg != GRANT_COUNT_REG_N_A) {
    Access->Write32 (
      Access,
      TgtGntCntUpstreamReg,
      GrantCount
    );
  }

  if (TgtGntCntDownstreamReg != GRANT_COUNT_REG_N_A) {
    Access->Write32 (
      Access,
      TgtGntCntDownstreamReg,
      GrantCount
    );
  }

  return EFI_SUCCESS;
}

#define DEFAULT_PCIE_GRANT_COUNT 0xFF
#define PSF_PCIE_CONTROLLER_MAX_CHANNELS 4
typedef struct {
  UINT32 Channels; // number of channels, cannot be greater than PSF_PCIE_CONTROLLER_MAX_CHANNELS
  UINT16 DevGntCntUpstreamReg[PSF_PCIE_CONTROLLER_MAX_CHANNELS];
  UINT16 TgtGntCntUpstreamReg[PSF_PCIE_CONTROLLER_MAX_CHANNELS];
  UINT16 TgtGntCntDownstreamReg[PSF_PCIE_CONTROLLER_MAX_CHANNELS];
} PTL_PCD_GRANT_COUNT_REG_DATA;

typedef struct _PTL_PCD_PSF_TOPOLOGY {
  REGISTER_ACCESS               *Access;
  UINT32                        PsfId;
  PSF_TOPO_PORT_TYPE            PortType;
  UINT32                        PcieCtrlIndex;
  struct _PTL_PCD_PSF_TOPOLOGY  *Child;
  PTL_PCD_GRANT_COUNT_REG_DATA  *GrantCountData;
  UINT32                        PcieRegs[PSF_PCIE_CONTROLLER_MAX_CHANNELS];
} PTL_PCD_PSF_TOPOLOGY;

UINT32
PtlPcdSetPcieControllerGrantCount (
  REGISTER_ACCESS              *Access,
  PTL_PCD_GRANT_COUNT_REG_DATA *GntCntData,
  UINT32                       ChannelGrant[PSF_PCIE_CONTROLLER_MAX_CHANNELS],
  BOOLEAN                      PsfPortEnabled[PSF_PCIE_CONTROLLER_MAX_CHANNELS]
  )
{
  UINT32 Channel;
  UINT32 ControllerGrantCount;

  ControllerGrantCount = 0;

  for (Channel = 0; Channel < GntCntData->Channels && Channel < PSF_PCIE_CONTROLLER_MAX_CHANNELS; Channel++) {
    if ((ChannelGrant[Channel] == DEFAULT_PCIE_GRANT_COUNT) || !PsfPortEnabled[Channel]) {
      //
      // If HW default or Root Port is disabled move to next channel
      continue;
    }

    PtlPcdSetGrantCount (Access, GntCntData->DevGntCntUpstreamReg[Channel], GntCntData->TgtGntCntUpstreamReg[Channel], GntCntData->TgtGntCntDownstreamReg[Channel], ChannelGrant[Channel]);
    ControllerGrantCount += ChannelGrant[Channel];
  }
  return ControllerGrantCount;
}

UINT32
PtlPcdSetSegmentGrantCount (
  REGISTER_ACCESS              *Access,
  PTL_PCD_GRANT_COUNT_REG_DATA *GntCntData,
  UINT32                       GrantCount
  )
{
  PtlPcdSetGrantCount (Access, GntCntData->DevGntCntUpstreamReg[0], GntCntData->TgtGntCntUpstreamReg[0], GntCntData->TgtGntCntDownstreamReg[0], GrantCount);

  return GrantCount;
}

BOOLEAN
PtlPcdPsfIsPcieEnabled (
  REGISTER_ACCESS *Access,
  UINT32          Register
  )
{
  return ((Access->Read32 (Access, Register + R_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN)
           & B_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN_FUNDIS) == 0);
}

VOID
PtlPcdPsfPcieChannelGrantCounts (
  IN  PSF_PCIE_CTRL_CONFIG  PsfPcieCtrlConfig,
  OUT UINT32                *ChannelGrant
  )
{
  switch (PsfPcieCtrlConfig) {
    case PsfPcieCtrl4xn:
      ChannelGrant[0] = 1;
      ChannelGrant[1] = 1;
      ChannelGrant[2] = 1;
      ChannelGrant[3] = 1;
      break;
    case PsfPcieCtrl1x2n_2xn:
      ChannelGrant[0] = 2;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = 1;
      ChannelGrant[3] = 1;
      break;
    case PsfPcieCtrl2xn_1x2n:
      ChannelGrant[0] = 1;
      ChannelGrant[1] = 1;
      ChannelGrant[2] = 2;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
    case PsfPcieCtrl2x2n:
      ChannelGrant[0] = 2;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = 2;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
    case PsfPcieCtrl1x4n:
      ChannelGrant[0] = 4;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
    default:
      ChannelGrant[0] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
  }
}

UINT32
PtlPcdTopologyConfigurePcieGrantCounts (
  PSF_PCIE_CTRL_CONFIG   *PcieCtrlConfigsArray,
  UINT32                 PcieCtrlConfigsArraySize,
  PTL_PCD_PSF_TOPOLOGY   *PsfSegment
  )
{
  UINT32               GrantCount;
  PTL_PCD_PSF_TOPOLOGY *ChildSegment;
  UINT32               ChannelGrant[PSF_PCIE_CONTROLLER_MAX_CHANNELS];
  BOOLEAN              PciePortEnabled[PSF_PCIE_CONTROLLER_MAX_CHANNELS];
  UINT32               Index;

  GrantCount = 0;

  if (PsfSegment->PortType == PsfPcieCtrlPort && PsfSegment->PcieCtrlIndex < PcieCtrlConfigsArraySize) {
    PtlPcdPsfPcieChannelGrantCounts (PcieCtrlConfigsArray[PsfSegment->PcieCtrlIndex], &ChannelGrant[0]);

    for (Index = 0; Index < PSF_PCIE_CONTROLLER_MAX_CHANNELS; Index++) {
      PciePortEnabled[Index] = PtlPcdPsfIsPcieEnabled (
                                 PsfSegment->Access,
                                 PsfSegment->PcieRegs[Index]
                               );
    }

    GrantCount = PtlPcdSetPcieControllerGrantCount (
                   PsfSegment->Access,
                   PsfSegment->GrantCountData,
                   ChannelGrant,
                   PciePortEnabled
                 );

  } else if (PsfSegment->PortType == PsfToPsfPort) {
    ChildSegment = PsfSegment->Child;

    while (ChildSegment->PortType != PsfNullPort) {
      GrantCount += PtlPcdTopologyConfigurePcieGrantCounts (
                      PcieCtrlConfigsArray,
                      PcieCtrlConfigsArraySize,
                      ChildSegment
                    );
      ChildSegment++;
    }

    if (GrantCount > 0) {
      GrantCount = PtlPcdSetSegmentGrantCount (
                     PsfSegment->Access,
                     PsfSegment->GrantCountData,
                     GrantCount
                     );
    }
  }
  return GrantCount;
}

PTL_PCD_GRANT_COUNT_REG_DATA mPxpaGrantCountRegs = {
  .Channels = 4,
  .DevGntCntUpstreamReg = {
    R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR20,
    R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR18,
    R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR16,
    R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR14
  },
  .TgtGntCntUpstreamReg = {
    GRANT_COUNT_REG_N_A,
    GRANT_COUNT_REG_N_A,
    GRANT_COUNT_REG_N_A,
    GRANT_COUNT_REG_N_A
  },
  .TgtGntCntDownstreamReg = {
    R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT0,
    R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT2,
    R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT4,
    R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT6
  }
};


PTL_PCD_GRANT_COUNT_REG_DATA mPxpbGrantCountRegs = {
  .Channels = 4,
  .DevGntCntUpstreamReg = {
    R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR9,
    R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR7,
    R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR5,
    R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR3
  },
  .TgtGntCntUpstreamReg = {
    GRANT_COUNT_REG_N_A,
    GRANT_COUNT_REG_N_A,
    GRANT_COUNT_REG_N_A,
    GRANT_COUNT_REG_N_A
  },
  .TgtGntCntDownstreamReg = {
    R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT8,
    R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT10,
    R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT12,
    R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT14
  }
};

PTL_PCD_GRANT_COUNT_REG_DATA mPsf0ToPsf6GrantCountRegs = {
  .Channels = 1,
  .DevGntCntUpstreamReg = {
    GRANT_COUNT_REG_N_A
  },
  .TgtGntCntUpstreamReg = {
    R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG0_TGT1
  },
  .TgtGntCntDownstreamReg = {
    R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT22
  }
};

PTL_PCD_GRANT_COUNT_REG_DATA mPsf0ToPsf4GrantCountRegs = {
  .Channels = 1,
  .DevGntCntUpstreamReg = {
    GRANT_COUNT_REG_N_A
  },
  .TgtGntCntUpstreamReg = {
    R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG0_TGT1
  },
  .TgtGntCntDownstreamReg = {
    R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT7
  }
};


#define PTL_PCD_PSF_TOPOLOGY_END { NULL, 0, PsfNullPort, 0, NULL, NULL }

PTL_PCD_PSF_TOPOLOGY PtllPcdPsf6[] = {
  {
    .Access = &Psf6Access.Access,
    .PsfId = 6,
    .PortType = PsfPcieCtrlPort,
    .PcieCtrlIndex = 0,
    .Child = NULL,
    .GrantCountData = &mPxpaGrantCountRegs,
    .PcieRegs = {
      R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F0_OFFSET64,
      R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F1_OFFSET65,
      R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F2_OFFSET66,
      R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F3_OFFSET67
    }
  },
  PTL_PCD_PSF_TOPOLOGY_END
};

PTL_PCD_PSF_TOPOLOGY PtllPcdPsf4[] = {
  {
    .Access = &Psf4Access.Access,
    .PsfId = 4,
    .PortType = PsfPcieCtrlPort,
    .PcieCtrlIndex = 1,
    .Child = NULL,
    .GrantCountData = &mPxpbGrantCountRegs,
    .PcieRegs = {
      R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F4_OFFSET64,
      R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F5_OFFSET65,
      R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F6_OFFSET66,
      R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F7_OFFSET67
    }
  },
  PTL_PCD_PSF_TOPOLOGY_END
};


PTL_PCD_PSF_TOPOLOGY PtlPcdPsf0[] = {
  {
    .Access = &Psf0Access.Access,
    .PsfId = 0,
    .PortType = PsfToPsfPort,
    .PcieCtrlIndex = 0, // this value is ignored if PortType == PsfToPsfPort
    .Child = PtllPcdPsf6,
    .GrantCountData = &mPsf0ToPsf6GrantCountRegs
  },
  {
    .Access = &Psf0Access.Access,
    .PsfId = 0,
    .PortType = PsfToPsfPort,
    .PcieCtrlIndex = 0, // this value is ignored if PortType == PsfToPsfPort
    .Child = PtllPcdPsf4,
    .GrantCountData = &mPsf0ToPsf4GrantCountRegs
  },
  PTL_PCD_PSF_TOPOLOGY_END
};


/**
  Program Grant Counts for PCIE controllers on PTL-PCD

  @param PcieCtrlConfigsArray        Array of PCIe controllers bifurcation config
  @param PcieCtrlConfigsArraySize    Size of the array
**/
VOID
PtlPcdGrantCountProgramming (
  PSF_PCIE_CTRL_CONFIG   *PcieCtrlConfigsArray,
  UINT32                 PcieCtrlConfigsArraySize
  )
{
  PTL_PCD_PSF_TOPOLOGY *PsfTopology;
    PsfTopology = PtlPcdPsf0;
  PtlPcdTopologyConfigurePcieGrantCounts (PcieCtrlConfigsArray, PcieCtrlConfigsArraySize, PsfTopology);
}
/**
  Program P2SB as EOI target
**/
VOID
PtlPcdEnableP2SbEoiTarget (
  VOID
  )
{
  PSF_PORT_DEST_ID P2sbTarget = {
    .Fields = {
      .ChannelId = 0,
      .PortId = 11,
      .PortGroupId = 1,
      .PsfId = 8
    }
  };
  PsfEnableEoiTarget (P2sbTarget, PtlPcdPsfGetSegmentTable (), PtlPcdPsfGetEoiRegDataTable ());
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
  PSF_PCIE_PORT_DATA *PortData;
    PortData = PtlPcdPsfGetPciePortData (RpIndex);
  PsfSetPcieFunction (PortData, NewFunction, PtlPcdPsfGetSegmentTable ());
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
  PSF_EARLY_INIT_DATA   EarlyInitData;
    EarlyInitData.PciePortDataTable = &mPtlPchPHPsfPciePortData;
    EarlyInitData.EoiRegDataTable = &mPtlPcdPHEoiRegData;
    EarlyInitData.MctpRegDataTable = &mPtlPcdPHMctpRegDataTable;
    EarlyInitData.MctpSupported = TRUE;

  PsfEarlyInit (&EarlyInitData, PtlPcdPsfGetSegmentTable (), PcieRpFuncNumArray, PcieRpFuncNumArraySize);
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
  PSF_PCIE_PORT_DATA *PortData;
    PortData = PtlPcdPsfGetPciePortData (RpIndex);
  PsfDisablePcieRootPort (PortData, PtlPcdPsfGetSegmentTable ());
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
  PSF_PCIE_PORT_DATA *PortData;
    PortData = PtlPcdPsfGetPciePortData (RpIndex);
  PsfEnablePcieRootPort (PortData, PtlPcdPsfGetSegmentTable ());

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
  PsfEnableEoiTarget (PtlPcdPsfPcieDestinationId (RpIndex), PtlPcdPsfGetSegmentTable(), PtlPcdPsfGetEoiRegDataTable ());
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
  PSF_SEGMENT_TABLE *PsfTable;
  UINT32            PsfTableIndex;
  PSF_DEV           *PsfDev;
  UINT32            DataAnd32;
  UINT32            DataOr32;

  DataAnd32 = ~0u;
  DataOr32 = (B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENLCG | B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENPCG);

  PsfTable = PtlPcdPsfGetSegmentTable ();
  //
  // There in an ordering requirement to program enTCG before enLCG.
  //
  for (PsfTableIndex = 0; PsfTableIndex < PsfTable->Size; PsfTableIndex++) {
    PsfDev = PsfTable->Data[PsfTableIndex].PsfDev;
    PsfDev->Access->AndThenOr32 (PsfDev->Access, R_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG, ~0u, B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENTCG);
    PsfDev->Access->AndThenOr32 (PsfDev->Access, R_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG, DataAnd32, DataOr32);
  }
}