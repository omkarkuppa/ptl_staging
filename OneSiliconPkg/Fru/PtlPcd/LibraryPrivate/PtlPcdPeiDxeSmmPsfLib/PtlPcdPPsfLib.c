/** @file
  This file contains internal PSF routines for PCH PSF SoC lib usage

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
#include <Library/BaseMemoryLib.h>
#include <Register/PchRegsPsf.h>
#include <Register/PsfRegsPtlPcdWrapper.h>
#include <Register/Ptl/PsfTcssRegsPtlPcd.h>
#include <Library/PsfLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Ppi/SiPolicy.h>
#include <PsfConfig.h>
#include <PcdSbPortIds.h>
#include <Library/Ptl/PtlPcdP2SbSocLib.h>

/**
  OSXML name: psf_ptlp_23Q1_auto_PTL-PCD-P_osxml_a1_eco1
Item: PSF_psf_ptlp_23Q1
Name: PSF
Release: psf_ptlp_23Q1
**/
#define PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE 8

extern EFI_GUID gPchP2SbDevGuid;

#ifndef P2SB_PID_INVALID
#define P2SB_PID_INVALID  0xFF
#endif

/**
  Return P2SB_PORT_16_ID corresponding to given PsfId

  @param[in]  PsfId   PSF number

  @retval P2SB_PORT_16_ID  P2SB PID corresponding to PSF number
**/
STATIC
P2SB_PORT_16_ID
PsfIdToP2SbPid (
  IN  UINT32  PsfId
  )
{
  P2SB_PORT_16_ID Pid;

  switch (PsfId) {
    case 0:
      Pid.Pid16bit = PTL_SID_F3_PID_PSF0;
      break;
    case 4:
      Pid.Pid16bit = PTL_SID_F2_PID_PSF4;
      break;
    case 6:
      Pid.Pid16bit = PTL_SID_F2_PID_PSF6;
      break;
    case 8:
      Pid.Pid16bit = PTL_SID_F2_PID_PSF8;
      break;
    case 11:
      Pid.Pid16bit = PTL_SID_F3_PID_TC_PSFTOP;
      break;
    case 12:
      Pid.Pid16bit = PTL_SID_F3_PID_TC_PSF;
      break;
    case 14:
      Pid.Pid16bit = PTL_SID_F2_PID_PSF14;
      break;
    case 15:
      Pid.Pid16bit = PTL_SID_F2_PID_PSF15;
      break;
    default:
      Pid.Pid16bit = P2SB_PID_INVALID;
      break;
  }
  return Pid;
}

STATIC
VOID
BuildPsfP2SbAccess (
  IN   UINT32                        PsfId,
  IN   P2SB_CONTROLLER               *P2SbController,
  OUT  P2SB_SIDEBAND_REGISTER_ACCESS *PsfP2SbAccess
  )
{
  P2SB_PORT_16_ID  PsfPid;
  EFI_STATUS       Status;

  PsfPid = PsfIdToP2SbPid (PsfId);
  ASSERT (PsfPid.Pid16bit != P2SB_PID_INVALID);

  Status = PtlPcdGetP2SbController (P2SbController, PsfPid);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  BuildP2SbSidebandAccess (
    P2SbController,
    PsfPid.PortId.LocalPid,
    0,
    P2SbPrivateConfig,
    P2SbMmioAccess,
    FALSE,
    PsfP2SbAccess
    );
}

/**
  Prepare table of supported PSF segments for PTL PCD-P

  @param[in     ] NumEntries          Number entries in PSF_DEV_TABLE
  @param[in, out] PsfTable            Pointer to PSF PSF_DEV_TABLE
  @param[in, out] PsfDevArray         Pointer to PSF_DEV array
  @param[in, out] P2SbController      Pointer to P2SB controllers array
  @param[in, out] P2SbAccess          Pointer to P2SB access array
**/
STATIC
VOID
PtlPcdPPsfPreparePsfTable (
  IN     UINT32                         NumEntries,
  IN OUT PSF_DEV_TABLE                  *PsfTable,
  IN OUT PSF_DEV                        *PsfDevArray,
  IN OUT P2SB_CONTROLLER                *P2SbController,
  IN OUT P2SB_SIDEBAND_REGISTER_ACCESS  *P2SbAccess
  )
{
  UINT32            Index;
  UINT32            PsfIdArray[] = { 0, 4, 6, 8, 11, 12, 14, 15 };
  ASSERT (ARRAY_SIZE (PsfIdArray) == NumEntries);

  for (Index = 0; Index < NumEntries; Index++) {
    BuildPsfP2SbAccess (PsfIdArray[Index], &(P2SbController[Index]), &(P2SbAccess[Index]));

    PsfDevArray[Index].Id = PsfIdArray[Index];
    PsfDevArray[Index].Access = (REGISTER_ACCESS*) &P2SbAccess[Index];
  }
  PsfTable->Size = NumEntries;
  PsfTable->Data = PsfDevArray;
}

/**
  Get PSF_PORT structure for I2C

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in]       I2cNum         I2C Controller number
  @param[in, out]  I2cPort        Pointer to PSF Port for current I2C device.

**/
STATIC
VOID
PtlPcdPPsfGetLpssI2cPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  UINT32                        I2cNum,
  IN  PSF_PORT                      *I2cPort
  )
{
  PSF_REG_BASE mPtlPcdPLpssI2cPsfRegBase[] =
  {
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F0_OFFSET3 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F1_OFFSET4 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F2_OFFSET5 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D21_F3_OFFSET6 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D25_F0_OFFSET7 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D25_F1_OFFSET8 }
  };

  if (I2cNum >= ARRAY_SIZE (mPtlPcdPLpssI2cPsfRegBase)) {
    DEBUG ((DEBUG_ERROR, "Wrong I2C index, max: %d, given: %d\n", ARRAY_SIZE (mPtlPcdPLpssI2cPsfRegBase) - 1, I2cNum));
    return;
  }

  BuildPsfP2SbAccess (
    mPtlPcdPLpssI2cPsfRegBase[I2cNum].PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPLpssI2cPsfRegBase[I2cNum].PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  I2cPort->PsfDev = PsfDev;
  I2cPort->RegBase = mPtlPcdPLpssI2cPsfRegBase[I2cNum].RegBase;
}

/**
  Disable LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPPsfDisableLpssI2c (
  IN UINT32  I2cNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT I2cPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssI2cPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    I2cNum,
    &I2cPort
  );

  PsfDisableDevice (&I2cPort);
}

/**
  Hide LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPPsfHideLpssI2c (
  IN UINT32  I2cNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT I2cPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssI2cPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    I2cNum,
    &I2cPort
  );

  PsfHideDevice (&I2cPort);
}

/**
  Unhide LPSS I2C at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPPsfUnhideLpssI2c (
  IN UINT32  I2cNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT I2cPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssI2cPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    I2cNum,
    &I2cPort
  );

  PsfUnhideDevice (&I2cPort);
}

/**
  Enable LPSS I2C BAR1 at PSF level

  @param[in] I2cNum  LPSS I2C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPPsfDisableLpssI2cBar1 (
  IN UINT32  I2cNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT I2cPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssI2cPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    I2cNum,
    &I2cPort
  );

  PsfDisableDeviceBar (&I2cPort, (BIT3 | BIT2));
}

/**
  Get PSF_PORT structure for I3C

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in]       I3cNum         I3C Controller number
  @param[in, out]  I3cPort        Pointer to PSF Port for current I3C device.
**/
STATIC
VOID
PtlPcdPPsfGetLpssI3cPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  UINT32                        I3cNum,
  IN  PSF_PORT                      *I3cPort
  )
{
  PSF_REG_BASE mPtlPcdPLpssI3cPsfPort[] =
  {
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F0_OFFSET9 },
    { 0, 0 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D17_F1_OFFSET10 }
  };

  if (I3cNum >= ARRAY_SIZE (mPtlPcdPLpssI3cPsfPort)|| (I3cNum == 1)) {
    DEBUG ((DEBUG_ERROR, "Wrong I3C index, max: %d, excluding 1, given: %d\n", ARRAY_SIZE (mPtlPcdPLpssI3cPsfPort) - 1, I3cNum));
    return;
  }

  BuildPsfP2SbAccess (
    mPtlPcdPLpssI3cPsfPort[I3cNum].PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPLpssI3cPsfPort[I3cNum].PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  I3cPort->PsfDev = PsfDev;
  I3cPort->RegBase = mPtlPcdPLpssI3cPsfPort[I3cNum].RegBase;
}

/**
  Disable LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPPsfDisableLpssI3c (
  IN UINT32  I3cNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT I3cPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssI3cPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    I3cNum,
    &I3cPort
  );
  PsfDisableDevice (&I3cPort);
}

/**
  Hide LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPPsfHideLpssI3c (
  IN UINT32  I3cNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT I3cPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssI3cPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    I3cNum,
    &I3cPort
  );
  PsfHideDevice (&I3cPort);
}

/**
  Unhide LPSS I3C at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPPsfUnhideLpssI3c (
  IN UINT32  I3cNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT I3cPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssI3cPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    I3cNum,
    &I3cPort
  );
  PsfUnhideDevice (&I3cPort);
}

/**
  Enable LPSS I3C BAR1 at PSF level

  @param[in] I2cNum  LPSS I3C device (I2C0, I2C1, ....)
**/
VOID
PtlPcdPPsfDisableLpssI3cBar1 (
  IN UINT32  I3cNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT I3cPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssI3cPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    I3cNum,
    &I3cPort
  );

  PsfEnableDeviceBar (&I3cPort, BIT3 | BIT2);
}

/**
  Get PSF_PORT structure for SPI

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in]       SpiNum         SPI Controller number
  @param[in, out]  SpiPort        Pointer to PSF Port for current SPI device.
**/
VOID
PtlPcdPPsfGetLpssSpiPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  UINT32                        SpiNum,
  IN  PSF_PORT                      *SpiPort
  )
{
  PSF_REG_BASE mPtlPcdPLpssSpiPsfRegBase[] =
  {
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F2_OFFSET11 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F3_OFFSET12 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D18_F6_OFFSET13 }
  };

  if (SpiNum >= ARRAY_SIZE (mPtlPcdPLpssSpiPsfRegBase)) {
    DEBUG ((DEBUG_ERROR, "Wrong SPI index, max: %d, given: %d\n", ARRAY_SIZE (mPtlPcdPLpssSpiPsfRegBase) - 1, SpiNum));
    return;
  }

  BuildPsfP2SbAccess (
    mPtlPcdPLpssSpiPsfRegBase[SpiNum].PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPLpssSpiPsfRegBase[SpiNum].PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  SpiPort->PsfDev = PsfDev;
  SpiPort->RegBase = mPtlPcdPLpssSpiPsfRegBase[SpiNum].RegBase;
}

/**
  Disable LPSS SPI at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdPPsfDisableLpssSpi (
  IN UINT32  SpiNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT SpiPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssSpiPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    SpiNum,
    &SpiPort
  );

  PsfDisableDevice (&SpiPort);
}

/**
  Hide LPSS SPI at PSF level

  @param[in] SpiNum  LPSS SPI device (SPI0, SPI1, ....)
**/
VOID
PtlPcdPPsfHideLpssSpi (
  IN UINT32  SpiNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT SpiPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssSpiPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    SpiNum,
    &SpiPort
  );

  PsfHideDevice (&SpiPort);
}

/**
  Unhide LPSS SPI at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdPPsfUnhideLpssSpi (
  IN UINT32  SpiNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT SpiPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssSpiPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    SpiNum,
    &SpiPort
  );

  PsfUnhideDevice (&SpiPort);
}

/**
  Disable LPSS SPI BAR1 at PSF level

  @param[in] SpiNum  SPI number (0 based)
**/
VOID
PtlPcdPPsfDisableLpssSpiBar1 (
  IN UINT32  SpiNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT SpiPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssSpiPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    SpiNum,
    &SpiPort
  );

  PsfDisableDeviceBar (&SpiPort, (BIT3 | BIT2));
}

/**
  Get PSF_PORT structure for UART

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in]       UartNum        UART Controller number
  @param[in, out]  UartPort       Pointer to PSF Port for current UART device.
**/
VOID
PtlPcdPPsfGetLpssUartPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  UINT32                        UartNum,
  IN  PSF_PORT                      *UartPort
  )
{
  PSF_REG_BASE mPtlPcdPLpssUartPsfRegBase[] =
  {
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F0_OFFSET14 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D30_F1_OFFSET15 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_LPSS_RS0_D25_F2_OFFSET16 }
  };

  if (UartNum >= ARRAY_SIZE (mPtlPcdPLpssUartPsfRegBase)) {
    DEBUG ((DEBUG_ERROR, "Wrong UART index, max: %d, given: %d\n", ARRAY_SIZE (mPtlPcdPLpssUartPsfRegBase) - 1, UartNum));
    return;
  }

  BuildPsfP2SbAccess (
    mPtlPcdPLpssUartPsfRegBase[UartNum].PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPLpssUartPsfRegBase[UartNum].PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  UartPort->PsfDev = PsfDev;
  UartPort->RegBase = mPtlPcdPLpssUartPsfRegBase[UartNum].RegBase;
}

/**
  Disable LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPPsfDisableLpssUart (
  IN UINT32  UartNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT UartPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssUartPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    UartNum,
    &UartPort
  );

  PsfDisableDevice (&UartPort);
}

/**
  Enable LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPPsfEnableLpssUart (
  IN UINT32  UartNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT UartPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssUartPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    UartNum,
    &UartPort
  );

  PsfEnableDevice (&UartPort);
}

/**
  Unhide LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPPsfUnhideLpssUart (
  IN UINT32  UartNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT UartPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssUartPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    UartNum,
    &UartPort
  );

  PsfUnhideDevice (&UartPort);
}

/**
  Hide LPSS UART at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPPsfHideLpssUart (
  IN UINT32  UartNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT UartPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssUartPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    UartNum,
    &UartPort
  );

  PsfHideDevice (&UartPort);
}

/**
  Disable LPSS UART BAR1 at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPPsfDisableLpssUartBar1 (
  IN UINT32  UartNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT UartPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssUartPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    UartNum,
    &UartPort
  );

  PsfDisableDeviceBar (&UartPort, (BIT3 | BIT2));
}

/**
  Enable LPSS UART BAR1 at PSF level

  @param[in] UartNum  UART number (0 based)
**/
VOID
PtlPcdPPsfEnableLpssUartBar1 (
  IN UINT32  UartNum
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT UartPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetLpssUartPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    UartNum,
    &UartPort
  );

  PsfEnableDeviceBar (&UartPort, (BIT3 | BIT2));
}

/**
  Get PSF_PORT structure for TraceHub

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  TraceHubPort   Pointer to PSF Port for current TraceHub device.
**/
VOID
PtlPcdPPsfGetTraceHubPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *TraceHubPort
  )
{
  PSF_REG_BASE mPtlPcdPTraceHubPsfPort = { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_NPK_RS0_D31_F7_OFFSET5 };

  BuildPsfP2SbAccess (
    mPtlPcdPTraceHubPsfPort.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPTraceHubPsfPort.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  TraceHubPort->PsfDev = PsfDev;
  TraceHubPort->RegBase = mPtlPcdPTraceHubPsfPort.RegBase;
}

/**
  Disable TraceHub at PSF level
**/
VOID
PtlPcdPPsfDisableTraceHub (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT TraceHubPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetTraceHubPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &TraceHubPort
  );

  PsfDisableDevice (&TraceHubPort);
}

/**
  Get PSF_PORT structure for TraceHub ACPI

  @param[in, out]  RegisterAccess    REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev            Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  TraceHubAcpiPort  Pointer to PSF Port for current TraceHub ACPI device.
**/
STATIC
VOID
PtlPcdPPsfGetTraceHubAcpiPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *TraceHubAcpiPort
  )
{
  PSF_REG_BASE mPtlPcdPTraceHubAcpiPsfPort = { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_NPK_RS0_D17_F2_OFFSET4 };

  BuildPsfP2SbAccess (
    mPtlPcdPTraceHubAcpiPsfPort.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPTraceHubAcpiPsfPort.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  TraceHubAcpiPort->PsfDev = PsfDev;
  TraceHubAcpiPort->RegBase = mPtlPcdPTraceHubAcpiPsfPort.RegBase;
}

/**
  Set TraceHub ACPI BARx address at PSF level

  @param[in] BarNum      BAR Number (0:BAR0, 1:BAR1, ...)
  @param[in] BarValue    32bit BAR value
**/
VOID
PtlPcdPPsfSetTraceHubAcpiBar (
  IN UINT8     BarNum,
  IN UINT32    BarValue
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT TraceHubAcpiPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetTraceHubAcpiPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &TraceHubAcpiPort
  );

  PsfSetDeviceBarValue (&TraceHubAcpiPort, BarNum, BarValue);
}

/**
  Enable TraceHub ACPI Memory Space at PSF level
**/
VOID
PtlPcdPPsfEnableTraceHubAcpiMemSpace (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT TraceHubAcpiPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetTraceHubAcpiPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &TraceHubAcpiPort
  );

  PsfEnableDeviceMemSpace (&TraceHubAcpiPort);
}

/**
  Hide TraceHub ACPI at PSF level
**/
VOID
PtlPcdPPsfHideTraceHubAcpi (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT TraceHubAcpiPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetTraceHubAcpiPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &TraceHubAcpiPort
  );

  PsfHideDevice (&TraceHubAcpiPort);
}

/**
  Disable TraceHub ACPI at PSF level
**/
VOID
PtlPcdPPsfDisableTraceHubAcpi (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT TraceHubAcpiPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetTraceHubAcpiPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &TraceHubAcpiPort
  );

  PsfDisableDevice (&TraceHubAcpiPort);
}

/**
  Get PSF_PORT structure for HECI

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in]       HeciNum        HECI Controller number
  @param[in, out]  HeciPort       Pointer to PSF Port for current HECI device.
**/
STATIC
VOID
PtlPcdPPsfGetHeciPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  UINT32                        HeciNum,
  IN  PSF_PORT                      *HeciPort
  )
{
  PSF_REG_BASE mPtlPcdPHeciPsfRegBase[] =
  {
    { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F0_OFFSET9 },
    { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F1_OFFSET10 },
    { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F4_OFFSET11 },
    { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_CSE_RS0_D22_F5_OFFSET12 }
  };

  if (HeciNum >= ARRAY_SIZE (mPtlPcdPHeciPsfRegBase)) {
    DEBUG ((DEBUG_ERROR, "Wrong HECI index, max: %d, given: %d\n", ARRAY_SIZE (mPtlPcdPHeciPsfRegBase) - 1, HeciNum));
    return;
  }

  BuildPsfP2SbAccess (
    mPtlPcdPHeciPsfRegBase[HeciNum].PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPHeciPsfRegBase[HeciNum].PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  HeciPort->PsfDev = PsfDev;
  HeciPort->RegBase = mPtlPcdPHeciPsfRegBase[HeciNum].RegBase;
}

/**
  Enable HECI at PSF level

  @param[in] HeciNumber  HECI Number (1-4)
**/
VOID
PtlPcdPPsfEnableHeci (
  IN  UINT8     HeciNumber
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT HeciPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetHeciPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    HeciNumber,
    &HeciPort
  );

  PsfEnableDevice (&HeciPort);
}

/**
  Disable HECI at PSF level

  @param[in] HeciNumber  HECI Number (1-4)
**/
VOID
PtlPcdPPsfDisableHeci (
  IN  UINT8     HeciNumber
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT HeciPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetHeciPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    HeciNumber,
    &HeciPort
  );

  PsfDisableDevice (&HeciPort);
}

/**
  Get PSF_PORT structure for SSE

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in]       SseNum         SSE Controller number
  @param[in, out]  SsePort        Pointer to PSF Port for current SSE device.
**/
STATIC
VOID
PtlPcdPPsfGetSsePort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  UINT32                        SseNum,
  IN  PSF_PORT                      *SsePort
  )
{
  PSF_REG_BASE mPtlPcdPSsePsfRegBase[] =
  {
    { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ESE_RS0_D24_F0_OFFSET16 },
    { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ESE_RS0_D24_F1_OFFSET17 },
    { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ESE_RS0_D24_F2_OFFSET18 }
  };

  if (SseNum >= ARRAY_SIZE (mPtlPcdPSsePsfRegBase)) {
    DEBUG ((DEBUG_ERROR, "Wrong SSE index, max: %d, given: %d\n", ARRAY_SIZE (mPtlPcdPSsePsfRegBase) - 1, SseNum));
    return;
  }

  BuildPsfP2SbAccess (
    mPtlPcdPSsePsfRegBase[SseNum].PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPSsePsfRegBase[SseNum].PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  SsePort->PsfDev = PsfDev;
  SsePort->RegBase = mPtlPcdPSsePsfRegBase[SseNum].RegBase;
}

/**
  Disable SSE at PSF level

  @param[in] SseNumber     SSE Number (1-3)
**/
VOID
PtlPcdPPsfDisableSse (
  IN UINT8      SseNumber
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT SsePort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetSsePort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    SseNumber,
    &SsePort
  );

  PsfDisableDevice (&SsePort);
}

/**
  Get PSF_PORT structure for SOL

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  SolPort        Pointer to PSF Port for current SOL device.
**/
STATIC
VOID
PtlPcdPPsfGetSolPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *SolPort
  )
{
  PSF_REG_BASE mPtlPcdPSolPsfRegBase = {
    6,
    R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_PTIO_RS0_D22_F3_OFFSET27
  };

  BuildPsfP2SbAccess (
    mPtlPcdPSolPsfRegBase.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPSolPsfRegBase.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  SolPort->PsfDev = PsfDev;
  SolPort->RegBase = mPtlPcdPSolPsfRegBase.RegBase;
}

/**
  Enable SOL at PSF level
**/
VOID
PtlPcdPPsfEnableSol (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT SolPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetSolPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &SolPort
  );

  PsfEnableDevice (&SolPort);
}

/**
  Disable SOL at PSF level
**/
VOID
PtlPcdPPsfDisableSol (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT SolPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetSolPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &SolPort
  );

  PsfDisableDevice (&SolPort);
}

/**
  Get PSF_PORT structure for IDER

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  IderPort       Pointer to PSF Port for current IDER device.
**/
STATIC
VOID
PtlPcdPPsfGetIderPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *IderPort
  )
{
  PSF_REG_BASE mPtlPcdPIderPsfRegBase = { 6, R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_PTIO_RS0_D22_F2_OFFSET26 };

  BuildPsfP2SbAccess (
    mPtlPcdPIderPsfRegBase.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPIderPsfRegBase.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  IderPort->PsfDev = PsfDev;
  IderPort->RegBase = mPtlPcdPIderPsfRegBase.RegBase;
}

/**
  Disable IDER device at PSF level
**/
VOID
PtlPcdPPsfDisableIder (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT IderPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetIderPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &IderPort
  );

  PsfDisableDevice (&IderPort);
}

/**
  Get PSF_PORT structure for ACE

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  AcePort        Pointer to PSF Port for current Ace device.
**/
STATIC
VOID
PtlPcdPPsfGetAcePort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *AcePort
  )
{
  PSF_REG_BASE mPtlPcdPAcePsfRegBase;

  mPtlPcdPAcePsfRegBase.PsfNumber = 6;
  mPtlPcdPAcePsfRegBase.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_ACE_RS0_D31_F3_OFFSET28;

  BuildPsfP2SbAccess (
    mPtlPcdPAcePsfRegBase.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPAcePsfRegBase.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  AcePort->PsfDev = PsfDev;
  AcePort->RegBase = mPtlPcdPAcePsfRegBase.RegBase;
}

/**
  Disable HDAudio device at PSF level
**/
VOID
PtlPcdPPsfDisableAce (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT AcePort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetAcePort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &AcePort
  );

  PsfDisableDevice (&AcePort);
}

/**
  Disable DSP Bar (HdAudio BAR4 and BAR5) at PSF level.
**/
VOID
PtlPcdPPsfDisableDspBar (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT AcePort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetAcePort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &AcePort
  );

  PsfDisableDeviceBar (&AcePort, (BIT5 | BIT4));
}

/**
  Get PSF_PORT structure for THC

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in]       ThcNum         THC Controller number
  @param[in, out]  ThcPort        Pointer to PSF Port for current THC device.
**/
STATIC
VOID
PtlPcdPPsfGetThcPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  UINT32                        ThcNum,
  IN  PSF_PORT                      *ThcPort
  )
{
  PSF_REG_BASE mPtlPcdPThcPsfRegBase[] =
  {
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_THC0_RS0_D16_F0_OFFSET29 },
    { 8, R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_THC1_RS0_D16_F1_OFFSET30 }
  };

  if (ThcNum >= ARRAY_SIZE (mPtlPcdPThcPsfRegBase)) {
    DEBUG ((
      DEBUG_WARN,
      "Wrong THC number, max: %d, given: %d\n",
      ARRAY_SIZE (mPtlPcdPThcPsfRegBase) - 1,
      ThcNum
      ));
    ASSERT (FALSE);
    return;
  }

  BuildPsfP2SbAccess (
    mPtlPcdPThcPsfRegBase[ThcNum].PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPThcPsfRegBase[ThcNum].PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  ThcPort->PsfDev = PsfDev;
  ThcPort->RegBase = mPtlPcdPThcPsfRegBase[ThcNum].RegBase;
}

/**
  Disable THC device at PSF level

  @param[in]  ThcNumber                Touch Host Controller Number THC0 or THC1
**/
VOID
PtlPcdPPsfDisableThc (
  IN  UINT32        ThcNumber
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT ThcPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetThcPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    ThcNumber,
    &ThcPort
  );
  PsfDisableDevice (&ThcPort);
}

/**
  Get PSF_PORT structure for XDCI

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  XdciPort       Pointer to PSF Port for current XDCI device.
**/
STATIC
VOID
PtlPcdPPsfGetXdciPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *XdciPort
  )
{
  PSF_REG_BASE mPtlPcdPXdciPsfRegBase = {
    8,
    R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_XDCI_RS0_D20_F1_OFFSET31
    };

  BuildPsfP2SbAccess (
    mPtlPcdPXdciPsfRegBase.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPXdciPsfRegBase.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  XdciPort->PsfDev = PsfDev;
  XdciPort->RegBase = mPtlPcdPXdciPsfRegBase.RegBase;
}

/**
  Disable xDCI device at PSF level
**/
VOID
PtlPcdPPsfDisableXdci (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT XdciPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetXdciPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &XdciPort
  );

  PsfDisableDevice (&XdciPort);
}

/**
  Get PSF_PORT structure for ISH

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  IshPort        Pointer to PSF Port for current ISH device.
**/
VOID
PtlPcdPPsfGetIshPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *IshPort
  )
{
  PSF_REG_BASE mPtlPcdPIshPsfRegBase;

  mPtlPcdPIshPsfRegBase.PsfNumber = 0;
  mPtlPcdPIshPsfRegBase.RegBase = R_PTL_PCD_P_H_PSF_0_AGNT_T0_SHDW_BAR0_ISH_RS0_D18_F0_OFFSET2;

  BuildPsfP2SbAccess (
    mPtlPcdPIshPsfRegBase.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPIshPsfRegBase.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  IshPort->PsfDev = PsfDev;
  IshPort->RegBase = mPtlPcdPIshPsfRegBase.RegBase;
}

/**
  Disable ISH at PSF level
**/
VOID
PtlPcdPPsfDisableIsh (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT IshPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetIshPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &IshPort
  );

  PsfDisableDevice (&IshPort);
}

/**
  Disable ISH BAR1 at PSF level
**/
VOID
PtlPcdPPsfDisableIshBar1 (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT IshPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetIshPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &IshPort
  );

  PsfDisableDeviceBar (&IshPort, (BIT3 | BIT2));
}

/**
  Get PSF_PORT structure for GBE

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  GbePort        Pointer to PSF Port for current ISH device.
**/
VOID
PtlPcdPPsfGetGbePort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *GbePort
  )
{
  PSF_REG_BASE mPtlPcdPGbePsfRegBase;

  mPtlPcdPGbePsfRegBase.PsfNumber = 6;
  mPtlPcdPGbePsfRegBase.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_GBE_RS0_D31_F6_OFFSET32;

  BuildPsfP2SbAccess (
    mPtlPcdPGbePsfRegBase.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPGbePsfRegBase.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  GbePort->PsfDev = PsfDev;
  GbePort->RegBase = mPtlPcdPGbePsfRegBase.RegBase;
}

/**
  Disable GbE device at PSF level
**/
VOID
PtlPcdPPsfDisableGbe (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT GbePort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetGbePort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &GbePort
  );

  PsfDisableDevice (&GbePort);
}

/**
  Get PSF_PORT structure for SMBUS

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  SmbusPort      Pointer to PSF Port for current SMBUS device.
**/
STATIC
VOID
PtlPcdPPsfGetSmbusPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *SmbusPort
  )
{
  PSF_REG_BASE mPtlPcdPSmbusPsfRegBase = {
    8,
    R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_SMB_RS0_D31_F4_OFFSET24
    };

  BuildPsfP2SbAccess (
    mPtlPcdPSmbusPsfRegBase.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPSmbusPsfRegBase.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  SmbusPort->PsfDev = PsfDev;
  SmbusPort->RegBase = mPtlPcdPSmbusPsfRegBase.RegBase;
}

/**
  Disable SMBUS device at PSF level
**/
VOID
PtlPcdPPsfDisableSmbus (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT SmbusPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetSmbusPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &SmbusPort
  );

  PsfDisableDevice (&SmbusPort);
}

/**
  Get PSF_PORT structure for SCS UFS

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in]       ScsUfsNumber   Number of SCS UFS device
  @param[in, out]  ScsUfsPort     Pointer to PSF Port for current SCS UFS device.
**/
STATIC
VOID
PtlPcdPPsfGetScsUfsPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  UINT32                        ScsUfsNumber,
  IN  PSF_PORT                      *ScsUfsPort
  )
{
  PSF_REG_BASE mPtlPcdPScsUfsPsfRegBase[] =
  {
    { 0, R_PTL_PCD_P_PSF_0_AGNT_T0_SHDW_BAR0_UFS_RS0_D23_F0_OFFSET4 }
  };

  BuildPsfP2SbAccess (
    mPtlPcdPScsUfsPsfRegBase[ScsUfsNumber].PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPScsUfsPsfRegBase[ScsUfsNumber].PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  ScsUfsPort->PsfDev = PsfDev;
  ScsUfsPort->RegBase = mPtlPcdPScsUfsPsfRegBase[ScsUfsNumber].RegBase;
}

/**
  Disable SCS UFS at PSF level

  @param[in] UfsNumber   UFS number
**/
VOID
PtlPcdPPsfDisableScsUfs (
  IN UINT32  UfsNumber
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT ScsUfsPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetScsUfsPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    UfsNumber,
    &ScsUfsPort
  );
  PsfDisableDevice (&ScsUfsPort);
}

/**
  Disable SCS UFS BAR1 at PSF level

  @param[in] UfsNumber   UFS number
**/
VOID
PtlPcdPPsfDisableScsUfsBar1 (
  IN UINT32  UfsNumber
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT ScsUfsPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetScsUfsPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    UfsNumber,
    &ScsUfsPort
  );
  PsfDisableDeviceBar (&ScsUfsPort, (BIT3 | BIT2));
}

//
// Max number of PCIe Root Ports PSF controlled
//
#define PTL_PCD_P_PSF_MAX_PSF_DECODED_PCIE_ROOT_PORTS 8

/**
  Return max number of PCIe controllers decoded by PSF
**/
UINT32
PtlPcdPPsfGetMaxPsfDecodedPciePortNum (
  VOID
  )
{
  return PTL_PCD_P_PSF_MAX_PSF_DECODED_PCIE_ROOT_PORTS;
}

/**
  Return PSF_PCIE_PORT_DATA for given RpIndex

  @param[in]       RpIndex         PCIe Root Port Index (0 based)
  @param[in][out]  PciePortData    PCIE PORT Data
**/
STATIC
VOID
PtlPcdPPsfGetPciePortData (
  IN UINT32              RpIndex,
  IN PSF_PCIE_PORT_DATA  *PsfPcieRpPortData
  )
{
  STATIC PSF_PCIE_PORT_DATA mPtlPcdPPsfPciePortData[] = {
    { 6, 0, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F0_OFFSET64, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F0_OFFSET68, R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F0_OFFSET64, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F0_OFFSET68 },
    { 6, 0, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F1_OFFSET65, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F1_OFFSET69, R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F1_OFFSET65, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F1_OFFSET69 },
    { 6, 0, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F2_OFFSET66, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F2_OFFSET70, R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F2_OFFSET66, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F2_OFFSET70 },
    { 6, 0, R_PTL_PCD_P_H_PSF_6_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F3_OFFSET67, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPA_RS0_D28_F3_OFFSET71, R_PTL_PCD_P_H_PSF_6_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F3_OFFSET67, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPA_RS0_D28_F3_OFFSET71 },
    { 4, 0, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F4_OFFSET64, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F4_OFFSET64, R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F4_OFFSET64, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F4_OFFSET64 },
    { 4, 0, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F5_OFFSET65, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F5_OFFSET65, R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F5_OFFSET65, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F5_OFFSET65 },
    { 4, 0, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F6_OFFSET66, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F6_OFFSET66, R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F6_OFFSET66, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F6_OFFSET66 },
    { 4, 0, R_PTL_PCD_P_H_PSF_4_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F7_OFFSET67, R_PTL_PCD_P_H_PSF_0_AGNT_T1_SHDW_BAR0_PXPB_RS0_D28_F7_OFFSET67, R_PTL_PCD_P_H_PSF_4_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F7_OFFSET67, R_PTL_PCD_P_H_PSF_0_T1_AGENT_FUNCTION_CONFIG_PXPB_RS0_D28_F7_OFFSET67 }
  };

  ASSERT (PTL_PCD_P_PSF_MAX_PSF_DECODED_PCIE_ROOT_PORTS == ARRAY_SIZE (mPtlPcdPPsfPciePortData));
  if (RpIndex >= ARRAY_SIZE (mPtlPcdPPsfPciePortData)) {
    DEBUG ((DEBUG_ERROR, "%a - RpIndex out of range, given: %d, max: %d\n", __FUNCTION__, RpIndex, ARRAY_SIZE (mPtlPcdPPsfPciePortData) - 1));
    return;
  }
  CopyMem (
    PsfPcieRpPortData,
    &mPtlPcdPPsfPciePortData[RpIndex],
    sizeof (PSF_PCIE_PORT_DATA)
    );
}

PSF_MCAST_REG_DATA mPtlPcdPEoiReg[] = {
  { 0, R_PTL_PCD_P_H_PSF_0_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI, R_PTL_PCD_P_H_PSF_0_PSF_MC_CONTROL_MCAST0_RS0_EOI, 4 },
  { 4, R_PTL_PCD_P_H_PSF_4_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI, R_PTL_PCD_P_H_PSF_4_PSF_MC_CONTROL_MCAST0_RS0_EOI, 4 },
  { 6, R_PTL_PCD_P_H_PSF_6_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI, R_PTL_PCD_P_H_PSF_6_PSF_MC_CONTROL_MCAST0_RS0_EOI, 9 },
  { 8, R_PTL_PCD_P_H_PSF_8_PSF_MC_AGENT_MCAST0_RS0_TGT0_EOI, R_PTL_PCD_P_H_PSF_8_PSF_MC_CONTROL_MCAST0_RS0_EOI, 1 }
};

PSF_MCAST_REG_DATA_TABLE mPtlPcdPEoiRegData = {
  .Size = ARRAY_SIZE (mPtlPcdPEoiReg),
  .Data = mPtlPcdPEoiReg
};

/**
  Get EOI register data table for all PSFs

  @return PsfEoiRegDataTable   Pointer to PSF_EOI_REG_DATA_TABLE
**/
STATIC
PSF_MCAST_REG_DATA_TABLE*
PtlPcdPPsfGetEoiRegDataTable (
  VOID
  )
{
  return &mPtlPcdPEoiRegData;
}

/**
  PCIe PSF port destination ID (psf_id:port_group_id:port_id:channel_id)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval Destination ID
**/
PSF_PORT_DEST_ID
PtlPcdPPsfPcieDestinationId (
  IN UINT32  RpIndex
  )
{
  PSF_PORT_DEST_ID mPtlPcdPRpDestId[] =
  {
    { 0x68000 }, { 0x68002 }, { 0x68004 }, { 0x68006 },  // PXPA: PSF6, PortGroupId = 1, PortId = 0
    { 0x48100 }, { 0x48102 }, { 0x48104 }, { 0x48106 },  // PXPB: PSF4, PortGroupId = 1, PortId = 1
    { 0x48000 }, { 0x48002 }
  };

  if (RpIndex < ARRAY_SIZE (mPtlPcdPRpDestId)) {
    return mPtlPcdPRpDestId[RpIndex];
  }

  return (PSF_PORT_DEST_ID){0};
};


PSF_MCAST_REG_DATA mPtlPcdPMctpRegData[] = {
  { 0, R_PTL_PCD_P_H_PSF_0_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1, R_PTL_PCD_P_H_PSF_0_PSF_MC_CONTROL_MCAST1_RS0_MCTP1, 4, 0, TRUE, 6 },
  { 4, R_PTL_PCD_P_H_PSF_4_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1, R_PTL_PCD_P_H_PSF_4_PSF_MC_CONTROL_MCAST1_RS0_MCTP1, 4, 0, TRUE, 0 },
  { 6, R_PTL_PCD_P_H_PSF_6_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1, R_PTL_PCD_P_H_PSF_6_PSF_MC_CONTROL_MCAST1_RS0_MCTP1, 9, 0, FALSE, 0 }
};

PSF_MCAST_REG_DATA_TABLE mPtlPcdPMctpRegDataTable = {
  .Size = ARRAY_SIZE (mPtlPcdPMctpRegData),
  .Data = mPtlPcdPMctpRegData
};

/**
  Get MCTP register data table for all PSFs

  @return PsfEoiRegDataTable   Pointer to PSF_EOI_REG_DATA_TABLE
**/
STATIC
PSF_MCAST_REG_DATA_TABLE*
PtlPcdPPsfGetMctpRegDataTable (
  VOID
  )
{
  return &mPtlPcdPMctpRegDataTable;
}

/**
  Check if PCIe Root Port is enabled at PSF level

  @param[in] RpIndex      PCIe Root Port Index

  @retval    TRUE         Root Port is enabled on PSF
  @retval    FALSE        Root Port is disabled on PSF
**/
BOOLEAN
PtlPcdPPsfIsPcieRootPortEnabled (
  IN  UINT32  RpIndex
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;
  PSF_PCIE_PORT_DATA             PsfPcieRpPortData;

  if (RpIndex >= PtlPcdPPsfGetMaxPsfDecodedPciePortNum ()) {
    DEBUG ((DEBUG_WARN, "%a - wrong RpIndex, given: %d, max: %d\n", __FUNCTION__, RpIndex, PTL_PCD_P_PSF_MAX_PSF_DECODED_PCIE_ROOT_PORTS));
    return FALSE;
  }

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );
  ZeroMem (&PsfPcieRpPortData, sizeof (PSF_PCIE_PORT_DATA));
  PtlPcdPPsfGetPciePortData (RpIndex, &PsfPcieRpPortData);

  return PsfIsPcieRootPortEnabled (&PsfTable, &PsfPcieRpPortData);
}

/**
  Fill MCTP Targets Table

  @param[out] TargetIdTable    MCTP Targets table
  @param[in]  MaxTableSize     TargetIdTable real size

  @retval Number of targets, resulting size of the table
**/
STATIC
UINT32
PtlPcdPPsfMctpTargetsTable (
  OUT PSF_PORT_DEST_ID  *TargetIdTable,
  IN  UINT32            MaxTableSize
  )
{
  UINT32       TargetNum;
  UINT32       RpIndex;
  UINT32       MaxPsfDecodedPciePortCount;

  TargetNum = 0;
  ZeroMem (TargetIdTable, sizeof(PSF_PORT_DEST_ID) * MaxTableSize);

  MaxPsfDecodedPciePortCount = PtlPcdPPsfGetMaxPsfDecodedPciePortNum ();

  if (MaxPsfDecodedPciePortCount + 1 > MaxTableSize) {
    DEBUG ((DEBUG_ERROR, "Cannot create MctpTargetsTable - table size is too small!\n"));
    return 0;
  }

  for (RpIndex = 0; RpIndex < MaxPsfDecodedPciePortCount; RpIndex++) {
    if (PtlPcdPPsfIsPcieRootPortEnabled (RpIndex)) {
      TargetIdTable[TargetNum] = PtlPcdPPsfPcieDestinationId (RpIndex);
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

/**
  Get PSF_PORT structure for CNVI

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  CnviPort       Pointer to PSF Port for CNVI device.
**/
STATIC
VOID
PtlPcdPPsfGetCnviPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *CnviPort
  )
{
  PSF_REG_BASE mPtlPcdPCnviRegBase;

  mPtlPcdPCnviRegBase.PsfNumber = 6;
  mPtlPcdPCnviRegBase.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_WIFI_RS0_D20_F3_OFFSET34;

  BuildPsfP2SbAccess (
    mPtlPcdPCnviRegBase.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPCnviRegBase.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  CnviPort->PsfDev = PsfDev;
  CnviPort->RegBase = mPtlPcdPCnviRegBase.RegBase;
}

/**
  Disable CNVi at PSF level
**/
VOID
PtlPcdPPsfDisableCnvi (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT CnviPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetCnviPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &CnviPort
    );

  PsfDisableDevice (&CnviPort);
}

/**
  Get PSF_PORT structure for CNVI BT

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  CnviBtPort     Pointer to PSF Port for CNVI BT device.
**/
STATIC
VOID
PtlPcdPPsfGetCnviBtPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *CnviBtPort
  )
{
  PSF_REG_BASE mPtlPcdPCnviBtRegBase;

  mPtlPcdPCnviBtRegBase.PsfNumber = 6;
  mPtlPcdPCnviBtRegBase.RegBase = R_PTL_PCD_P_H_PSF_6_AGNT_T0_SHDW_BAR0_BT_RS0_D20_F7_OFFSET30;

  BuildPsfP2SbAccess (
    mPtlPcdPCnviBtRegBase.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPCnviBtRegBase.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  CnviBtPort->PsfDev = PsfDev;
  CnviBtPort->RegBase = mPtlPcdPCnviBtRegBase.RegBase;
}

/**
  Disable CNVi Bluetooth(R) at PSF level
**/
VOID
PtlPcdPPsfDisableCnviBt (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT CnviBtPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetCnviBtPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &CnviBtPort
  );

  PsfDisableDevice (&CnviBtPort);
}

/**
  Get PSF_PORT structure for PMC

  @param[in, out]  RegisterAccess REGISTER_ACCESS interface to PSF
  @param[in, out]  PsfDev         Pointer to PSF_DEV structure
  @param[in, out]  P2SbController Pointer to P2SB controller
  @param[in, out]  PmcPort        Pointer to PSF Port for current PMC device.
**/
STATIC
VOID
PtlPcdPPsfGetPmcPort (
  IN  P2SB_SIDEBAND_REGISTER_ACCESS *RegisterAccess,
  IN  PSF_DEV                       *PsfDev,
  IN  P2SB_CONTROLLER               *P2SbController,
  IN  PSF_PORT                      *PmcPort
  )
{
  PSF_REG_BASE mPtlPcdPPmcRegBase = {
    8,
    R_PTL_PCD_P_H_PSF_8_AGNT_T0_SHDW_BAR0_PMC_RS0_D31_F2_OFFSET21
  };

  BuildPsfP2SbAccess (
    mPtlPcdPPmcRegBase.PsfNumber,
    P2SbController,
    RegisterAccess
    );

  PsfDev->Id = mPtlPcdPPmcRegBase.PsfNumber;
  PsfDev->Access = &RegisterAccess->Access;

  PmcPort->PsfDev = PsfDev;
  PmcPort->RegBase = mPtlPcdPPmcRegBase.RegBase;
}

/**
  Set PMC ABASE at PSF level

  @param[in] Abase  PMC ACPI base address to be set
**/
VOID
PtlPcdPPsfSetPmcAbase (
  IN  UINT16  Abase
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT PmcPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetPmcPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &PmcPort
  );

  PsfSetPmcAbase (&PmcPort, Abase);
}

/**
  Hide PMC at PSF level
**/
VOID
PtlPcdPPsfHidePmc (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT PmcPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetPmcPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &PmcPort
  );

  PsfHideDevice (&PmcPort);
}

/**
  Get PMC PWRM BAR0 from PSF

  @retval Address  Address for PWRM BAR0
**/
UINTN
PtlPcdPPsfGetPmcPwrmBar0 (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS RegisterAccess;
  PSF_DEV PsfDev;
  PSF_PORT PmcPort;
  P2SB_CONTROLLER P2SbController;

  PtlPcdPPsfGetPmcPort (
    &RegisterAccess,
    &PsfDev,
    &P2SbController,
    &PmcPort
  );

  return PsfGetPmcPwrmBase (&PmcPort);
}

/**
  Enable PCIe Relaxed Ordering in PSF
**/
VOID
PtlPcdPPsfEnablePcieRelaxedOrder (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;

  STATIC PSF_REG_BASE PtlPcdPPsfPortConfigsEgressFroRegs[] =
  {
    { 0, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT0 },
    { 0, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT1 },
    { 0, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT2 },
    { 0, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT3 },
    { 0, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG1_PORT4 },
    { 4, R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG1_PORT0 },
    { 4, R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG1_PORT1 },
    { 4, R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG1_PORT2 },
    { 6, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT0 },
    { 6, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT1 },
    { 6, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT2 },
    { 6, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT3 },
    { 6, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT4 },
    { 6, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT5 },
    { 6, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT6 },
    { 6, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT7 },
    { 6, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG1_PORT8 }
  };

  STATIC PSF_REG_BASE PtlPcdPPsfPortConfigsIngressFroRegs[] =
  {
    { 0, R_PTL_PCD_P_H_PSF_0_PSF_PORT_CONFIG_PG0_PORT0 },
    { 4, R_PTL_PCD_P_H_PSF_4_PSF_PORT_CONFIG_PG0_PORT0 },
    { 6, R_PTL_PCD_P_H_PSF_6_PSF_PORT_CONFIG_PG0_PORT0 },
    { 8, R_PTL_PCD_P_H_PSF_8_PSF_PORT_CONFIG_PG0_PORT0 },
    { 14, R_PTL_PCD_P_H_PSF_14_PSF_PORT_CONFIG_PG0_PORT0 },
    { 15, R_PTL_PCD_P_H_PSF_15_PSF_PORT_CONFIG_PG0_PORT0 }
  };

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  PsfSetEgressFro (
    &PsfTable,
    PtlPcdPPsfPortConfigsEgressFroRegs,
    ARRAY_SIZE (PtlPcdPPsfPortConfigsEgressFroRegs)
    );

  PsfSetIngressFro (
    &PsfTable,
    PtlPcdPPsfPortConfigsIngressFroRegs,
    ARRAY_SIZE (PtlPcdPPsfPortConfigsIngressFroRegs)
    );
}

/**
  Return RC_OWNER value to program

  @retval RC_OWNER
**/
STATIC
UINT32
PtlPcdPPsfGetRcOwner (
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
#define PTL_PCD_P_MAX_MCTP_TARGET_TABLE_ENTRIES (PTL_PCD_P_PSF_MAX_PSF_DECODED_PCIE_ROOT_PORTS + 1)
/**
  Configure MCTP
**/
VOID
PtlPcdPPsfMctpConfigure (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;
  PSF_PORT_DEST_ID               PcdMctpTargetIdTable[PTL_PCD_P_MAX_MCTP_TARGET_TABLE_ENTRIES];
  UINT32                         PcdMctpTargetIdTableSize;

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  PcdMctpTargetIdTableSize =
  PtlPcdPPsfMctpTargetsTable (
    PcdMctpTargetIdTable,
    PTL_PCD_P_MAX_MCTP_TARGET_TABLE_ENTRIES
    );

  PsfConfigureMctpCycle (
    &PsfTable,
    PtlPcdPPsfGetMctpRegDataTable (),
    PcdMctpTargetIdTable,
    PcdMctpTargetIdTableSize,
    PtlPcdPPsfGetRcOwner ()
    );
}

STATIC PSF_REG_BASE mPtlPcdPPsfRootspaceConfigs[] = {
  { 0, R_PTL_PCD_P_H_PSF_0_ROOTSPACE_CONFIG_RS0 },
  { 0, R_PTL_PCD_P_H_PSF_0_ROOTSPACE_CONFIG_RS3 },
  { 4, R_PTL_PCD_P_H_PSF_4_ROOTSPACE_CONFIG_RS0 },
  { 4, R_PTL_PCD_P_H_PSF_4_ROOTSPACE_CONFIG_RS3 },
  { 6, R_PTL_PCD_P_H_PSF_6_ROOTSPACE_CONFIG_RS0 },
  { 6, R_PTL_PCD_P_H_PSF_6_ROOTSPACE_CONFIG_RS3 },
  { 8, R_PTL_PCD_P_H_PSF_8_ROOTSPACE_CONFIG_RS3 },
  { 8, R_PTL_PCD_P_H_PSF_8_ROOTSPACE_CONFIG_RS0 },
  { 11, R_PTL_PCD_P_H_TCSS_PSF_11_ROOTSPACE_CONFIG_RS0 },
  { 11, R_PTL_PCD_P_H_TCSS_PSF_11_ROOTSPACE_CONFIG_RS3 },
  { 12, R_PTL_PCD_P_H_TCSS_PSF_12_ROOTSPACE_CONFIG_RS0 },
  { 12, R_PTL_PCD_P_H_TCSS_PSF_12_ROOTSPACE_CONFIG_RS3 },
  { 14, R_PTL_PCD_P_H_PSF_14_ROOTSPACE_CONFIG_RS3 },
  { 14, R_PTL_PCD_P_H_PSF_14_ROOTSPACE_CONFIG_RS0 },
  { 15, R_PTL_PCD_P_H_PSF_15_ROOTSPACE_CONFIG_RS3 },
  { 15, R_PTL_PCD_P_H_PSF_15_ROOTSPACE_CONFIG_RS0 }
};

/**
  Enable VTd support in PSF.
**/
VOID
PtlPcdPPsfEnableVtd (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  PsfLibEnableVtd (
    &PsfTable,
    mPtlPcdPPsfRootspaceConfigs,
    ARRAY_SIZE (mPtlPcdPPsfRootspaceConfigs)
    );
}

/**
  Disable PSF address-based peer-to-peer decoding.
**/
VOID
PtlPcdPPsfDisableP2pDecoding (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  PsfDisableP2pDecoding (
    &PsfTable,
    mPtlPcdPPsfRootspaceConfigs,
    ARRAY_SIZE (mPtlPcdPPsfRootspaceConfigs)
    );
}

/**
  Reset R_PCH_PSF_PCR_ROOTSPACE_CONFIG_RS0 and
  R_PCH_PSF_PCR_ROOTSPACE_CONFIG_RS3 registers for all PSFs
  to HW default.
  These registers are resets to defaults only after global reset,
  after any other reset type these needs to be reset by BIOS.
**/
STATIC
VOID
PtlPcdPPsfResetRootspaceConfig (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  PsfResetRootspaceConfig (
    &PsfTable,
    mPtlPcdPPsfRootspaceConfigs,
    ARRAY_SIZE (mPtlPcdPPsfRootspaceConfigs)
    );
}

/**
  Program Deferred Write Buffer

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
PtlPcdPPsfProgramDWB (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                    Status;
  PSF_CONFIG                    *PsfConfig;
  PSF_DEV                       PsfDev;
  PSF_PORT                      PsfPort;
  P2SB_SIDEBAND_REGISTER_ACCESS RegAccess;
  P2SB_CONTROLLER               P2SbController;
  PSF_REG_BASE                  PsfRegBase;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPsfConfigGuid, (VOID *) &PsfConfig);
  ASSERT_EFI_ERROR (Status);

  PsfRegBase.PsfNumber = 6;
  PsfRegBase.RegBase = R_PTL_PCD_P_H_PSF_6_DWB_CONFIG_PG0_PORT0_CHANNEL0;

  BuildPsfP2SbAccess (PsfRegBase.PsfNumber, &P2SbController, &RegAccess);
  PsfDev.Id = PsfRegBase.PsfNumber;
  PsfDev.Access = (REGISTER_ACCESS *)&RegAccess;

  PsfPort.PsfDev = &PsfDev;
  PsfPort.RegBase = PsfRegBase.RegBase;

  PsfProgramDWB (
    &PsfPort,
    PsfConfig->DwbConfigDwbFlushThreshold,
    PsfConfig->DwbConfigNonxHCIEn,
    PsfConfig->DwbConfigOBFFEn,
    PsfConfig->DwbConfigDWBEn
    );
}

/**
  Convert Bifurcation, number of Root Ports and Lane numbers to Grant Count array

  @param[in]      Bifurcation   PCIe controller bifurcation encoded as in RPCFG register - number 0-7
  @param[in]      RpCount       Number of Root Ports on PCIe controller
  @param[in]      LaneNum       Number of lanes on PCIe controller
  @param[in, out] GcArray       Grant Count value array for given Bifurcation, RpNum and LaneNum
**/
STATIC
VOID
BifurcationToGrantCount (
  UINT8  Bifurcation,
  UINT32 RpCount,
  UINT32 LaneNum,
  UINT8  *GcArray
  )
{
  UINT32 Index;
  STATIC BIFURCATION_TO_GRANT_COUNT BifurcationToGrantCountArray[] = {
    { .Bifurcation = 0, .RpCount = 4, .LaneNum =  4, .GcArray = {  1, 1, 1, 1 } },
    { .Bifurcation = 1, .RpCount = 4, .LaneNum =  4, .GcArray = {  2, 0, 1, 1 } },
    { .Bifurcation = 2, .RpCount = 4, .LaneNum =  4, .GcArray = {  2, 0, 2, 0 } },
    { .Bifurcation = 3, .RpCount = 4, .LaneNum =  4, .GcArray = {  4, 0, 0, 0 } },

    { .Bifurcation = 2, .RpCount = 2, .LaneNum =  4, .GcArray = {  2, 2, 0, 0 } },
    { .Bifurcation = 3, .RpCount = 2, .LaneNum =  4, .GcArray = {  4, 0, 0, 0 } },

    { .Bifurcation = 3, .RpCount = 2, .LaneNum =  8, .GcArray = {  4, 4, 0, 0 } },
    { .Bifurcation = 5, .RpCount = 2, .LaneNum =  8, .GcArray = {  8, 0, 0, 0 } }
  };

  for (Index = 0; Index < ARRAY_SIZE (BifurcationToGrantCountArray); Index++) {
    if (BifurcationToGrantCountArray[Index].Bifurcation == Bifurcation &&
        BifurcationToGrantCountArray[Index].RpCount == RpCount &&
        BifurcationToGrantCountArray[Index].LaneNum == LaneNum)
    {
      CopyMem (GcArray, BifurcationToGrantCountArray[Index].GcArray, RpCount * sizeof (UINT8));
      return;
    }
  }
  DEBUG ((DEBUG_ERROR, "%a - Unsupported controller configuration: Bifurcation: 0x%X, RpCount: %d, LaneNum: %d\n",
    __FUNCTION__,
    Bifurcation,
    RpCount,
    LaneNum
    ));
}

/**
  Program Grant Counts for PCIE controllers on PTL-PCD-S

  @param[in] PcieCtrlBifurcationArray        Array of PCIe controllers bifurcation config
  @param[in] PcieCtrlNumOfLanesArray         Array of lanes per PCIe controller
  @param[in] PcieCtrlNumOfRootPortsArray     Array of root ports per PCIe controller
  @param[in] ArraySize                       Number of PCIe contollers
  @param[in] PcieRpEnableArray               Array of Root Port enable state
  @param[in] PcieRpArraySize                 Number of Root Ports and size of PcieRpEnableArray
**/
VOID
PtlPcdPPsfGrantCountProgramming (
  UINT8                  *PcieCtrlBifurcationArray,
  UINT8                  *PcieCtrlNumOfLanesArray,
  UINT8                  *PcieCtrlNumOfRootPortsArray,
  UINT32                 ArraySize,
  BOOLEAN                *PcieRpEnableArray,
  UINT32                 PcieRpArraySize
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;
  UINT32                         Index;
  UINT32                         RpNum = 0;

  //
  // REG_WITH_VALUEs below represent PSF_REG_BASE and value pair of Grant Count registers.
  // Since Grant Count registers could be shared among PCIe Root Ports to be filled with
  // Grant Count values, and those GC values will be added if more than one Root Port
  // has linked to the same register, those REG_WITH_VALUEs will be referenced in
  // GC_REGS_FOR_RP array below. F.ex. if first and second Root Port shared the same
  // REG_WITH_VALUE register pair for Device Grant Count, then PsfSetGrantCountForRegs
  // function below will add GC value for this REG_WITH_VALUE twice.
  //
  // Code needs to add GC values to REG_WITH_VALUE virtually before actual register programming,
  // because:
  // 1. GC values registers have default value of 4. Adding GC values to that register will
  //    result with incorrect programming
  // 2. Writing "0" to these registers first is forbidden since 0 is restricted value for them.
  //
  REG_WITH_VALUE Psf_6_Dgcr_20 = {{6, R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR20}, 0};
  REG_WITH_VALUE Psf_6_Dgcr_18 = {{6, R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR18}, 0};
  REG_WITH_VALUE Psf_6_Dgcr_16 = {{6, R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR16}, 0};
  REG_WITH_VALUE Psf_6_Dgcr_14 = {{6, R_PTL_PCD_P_H_PSF_6_DEV_GNTCNT_RELOAD_DGCR14}, 0};
  REG_WITH_VALUE Psf_4_Dgcr_9 = {{4, R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR9}, 0};
  REG_WITH_VALUE Psf_4_Dgcr_7 = {{4, R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR7}, 0};
  REG_WITH_VALUE Psf_4_Dgcr_5 = {{4, R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR5}, 0};
  REG_WITH_VALUE Psf_4_Dgcr_3 = {{4, R_PTL_PCD_P_H_PSF_4_DEV_GNTCNT_RELOAD_DGCR3}, 0};
  REG_WITH_VALUE Psf_6_Pg_1_Tgt_0 = {{6, R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT0}, 0};
  REG_WITH_VALUE Psf_6_Pg_1_Tgt_2 = {{6, R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT2}, 0};
  REG_WITH_VALUE Psf_6_Pg_1_Tgt_4 = {{6, R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT4}, 0};
  REG_WITH_VALUE Psf_6_Pg_1_Tgt_6 = {{6, R_PTL_PCD_P_H_PSF_6_TARGET_GNTCNT_RELOAD_PG1_TGT6}, 0};
  REG_WITH_VALUE Psf_4_Pg_1_Tgt_8 = {{4, R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT8}, 0};
  REG_WITH_VALUE Psf_4_Pg_1_Tgt_10 = {{4, R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT10}, 0};
  REG_WITH_VALUE Psf_4_Pg_1_Tgt_12 = {{4, R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT12}, 0};
  REG_WITH_VALUE Psf_4_Pg_1_Tgt_14 = {{4, R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT14}, 0};
  REG_WITH_VALUE Psf_4_Pg_0_Tgt_1 = {{4, R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG0_TGT1}, 0};
  REG_WITH_VALUE Psf_4_Pg_1_Tgt_0 = {{4, R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT0}, 0};
  REG_WITH_VALUE Psf_4_Pg_0_Tgt_2 = {{4, R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG0_TGT2}, 0};
  REG_WITH_VALUE Psf_4_Pg_1_Tgt_2 = {{4, R_PTL_PCD_P_H_PSF_4_TARGET_GNTCNT_RELOAD_PG1_TGT2}, 0};
  REG_WITH_VALUE Psf_0_Pg_0_Tgt_1 = {{0, R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG0_TGT1}, 0};
  REG_WITH_VALUE Psf_0_Pg_1_Tgt_7 = {{0, R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT7}, 0};
  REG_WITH_VALUE Psf_0_Pg_0_Tgt_2 = {{0, R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG0_TGT2}, 0};
  REG_WITH_VALUE Psf_0_Pg_1_Tgt_8 = {{0, R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT8}, 0};
  REG_WITH_VALUE Psf_0_Pg_0_Tgt_3 = {{0, R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG0_TGT3}, 0};
  REG_WITH_VALUE Psf_0_Pg_1_Tgt_9 = {{0, R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT9}, 0};
  REG_WITH_VALUE Psf_0_Pg_1_Tgt_22 = {{0, R_PTL_PCD_P_H_PSF_0_TARGET_GNTCNT_RELOAD_PG1_TGT22}, 0};

  //
  // RegWithValueArray is the array of REG_WITH_VALUE for function PsfProgramGrantCountRegisters
  // that writes to register proper values (if value is not equal 0). That should be called after
  // GcArray is set with proper GC values that comes from bifurcation, number of root ports
  // and lane numbers, and after PsfSetGrantCountForRegs function
  // (that sets GC for proper register for every Root Port) is called
  //
  REG_WITH_VALUE *RegWithValueArray[] = {
    &Psf_6_Dgcr_20,
    &Psf_6_Dgcr_18,
    &Psf_6_Dgcr_16,
    &Psf_6_Dgcr_14,
    &Psf_4_Dgcr_9,
    &Psf_4_Dgcr_7,
    &Psf_4_Dgcr_5,
    &Psf_4_Dgcr_3,
    &Psf_6_Pg_1_Tgt_0,
    &Psf_6_Pg_1_Tgt_2,
    &Psf_6_Pg_1_Tgt_4,
    &Psf_6_Pg_1_Tgt_6,
    &Psf_4_Pg_1_Tgt_8,
    &Psf_4_Pg_1_Tgt_10,
    &Psf_4_Pg_1_Tgt_12,
    &Psf_4_Pg_1_Tgt_14,
    &Psf_4_Pg_0_Tgt_1,
    &Psf_4_Pg_1_Tgt_0,
    &Psf_4_Pg_0_Tgt_2,
    &Psf_4_Pg_1_Tgt_2,
    &Psf_0_Pg_0_Tgt_1,
    &Psf_0_Pg_1_Tgt_7,
    &Psf_0_Pg_0_Tgt_2,
    &Psf_0_Pg_1_Tgt_8,
    &Psf_0_Pg_0_Tgt_3,
    &Psf_0_Pg_1_Tgt_9,
    &Psf_0_Pg_1_Tgt_22
  };

  //
  // GcRegsForRp is array of structures that contains REG_WITH_VALUE pointers that corresponds to
  // proper Device, Target Upstream and Targed Downstream Grant Count resiter for every Root Port.
  // If there is no proper Grant Count register of particular type for given Root Port, pointer
  // will be NULL, and function PsfSetGrantCountForRegs will omit that register
  //
  GC_REGS_FOR_RP GcRegsForRp[] = {
    // Target:Channel PXPA:0
    {
      .GcRegs1stLvl = { &Psf_6_Dgcr_20, NULL, &Psf_6_Pg_1_Tgt_0 },
      .GcRegs2ndLvl = { NULL, &Psf_0_Pg_0_Tgt_1, &Psf_0_Pg_1_Tgt_22 }
    },
    // Target:Channel PXPA:2
    {
      .GcRegs1stLvl = { &Psf_6_Dgcr_18, NULL, &Psf_6_Pg_1_Tgt_2 },
      .GcRegs2ndLvl = { NULL, &Psf_0_Pg_0_Tgt_1, &Psf_0_Pg_1_Tgt_22 }
    },
    // Target:Channel PXPA:4
    {
      .GcRegs1stLvl = { &Psf_6_Dgcr_16, NULL, &Psf_6_Pg_1_Tgt_4 },
      .GcRegs2ndLvl = { NULL, &Psf_0_Pg_0_Tgt_1, &Psf_0_Pg_1_Tgt_22 }
    },
    // Target:Channel PXPA:6
    {
      .GcRegs1stLvl = { &Psf_6_Dgcr_14, NULL, &Psf_6_Pg_1_Tgt_6 },
      .GcRegs2ndLvl = { NULL, &Psf_0_Pg_0_Tgt_1, &Psf_0_Pg_1_Tgt_22 }
    },
    // Target:Channel PXPB:0
    {
      .GcRegs1stLvl = { &Psf_4_Dgcr_9, NULL, &Psf_4_Pg_1_Tgt_8 },
      .GcRegs2ndLvl = { NULL, &Psf_0_Pg_0_Tgt_1, &Psf_0_Pg_1_Tgt_7 }
    },
    // Target:Channel PXPB:2
    {
      .GcRegs1stLvl = { &Psf_4_Dgcr_7, NULL, &Psf_4_Pg_1_Tgt_10 },
      .GcRegs2ndLvl = { NULL, &Psf_0_Pg_0_Tgt_1, &Psf_0_Pg_1_Tgt_7 }
    },
    // Target:Channel PXPB:4
    {
      .GcRegs1stLvl = { &Psf_4_Dgcr_5, NULL, &Psf_4_Pg_1_Tgt_12 },
      .GcRegs2ndLvl = { NULL, &Psf_0_Pg_0_Tgt_1, &Psf_0_Pg_1_Tgt_7 }
    },
    // Target:Channel PXPB:6
    {
      .GcRegs1stLvl = { &Psf_4_Dgcr_3, NULL, &Psf_4_Pg_1_Tgt_14 },
      .GcRegs2ndLvl = { NULL, &Psf_0_Pg_0_Tgt_1, &Psf_0_Pg_1_Tgt_7 }
    },
    // Target:Channel PXPC:0
    {
      .GcRegs1stLvl = { NULL, &Psf_4_Pg_0_Tgt_1, &Psf_4_Pg_1_Tgt_0 },
      .GcRegs2ndLvl = { NULL, &Psf_0_Pg_0_Tgt_2, &Psf_0_Pg_1_Tgt_8 }
    },
    // Target:Channel PXPC:2
    {
      .GcRegs1stLvl = { NULL, &Psf_4_Pg_0_Tgt_2, &Psf_4_Pg_1_Tgt_2 },
      .GcRegs2ndLvl = { NULL, &Psf_0_Pg_0_Tgt_3, &Psf_0_Pg_1_Tgt_9 }
    }
  };

  UINT8 GcArray[10];

  ZeroMem (GcArray, ARRAY_SIZE (GcArray));

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  //
  // Fill GcArray with proper GC values according to
  // bifurcation, RP number and lane number
  //
  RpNum = 0;
  for (Index = 0; Index < ArraySize; Index++) {
    BifurcationToGrantCount (
      PcieCtrlBifurcationArray[Index],
      PcieCtrlNumOfRootPortsArray[Index],
      PcieCtrlNumOfLanesArray[Index],
      &GcArray[RpNum]
      );
    RpNum += PcieCtrlNumOfRootPortsArray[Index];
  }

  ASSERT (ARRAY_SIZE(GcRegsForRp) == ARRAY_SIZE (GcArray));
  ASSERT (ARRAY_SIZE (GcArray) == PcieRpArraySize);

  //
  // If Root Port is disabled, do not set Grant Count for it
  //
  for (Index = 0; Index < PcieRpArraySize; Index++) {
    if (!PcieRpEnableArray[Index]) {
      GcArray[Index] = 0;
    }
  }

  PsfSetGrantCountForRegs (
    (GC_REGS_FOR_RP *)GcRegsForRp,
    GcArray,
    ARRAY_SIZE(GcRegsForRp)
  );

  //
  // PsfSetGrantCountForRegs sets Grant Count values only virtually - proper
  // register programming is done by PsfProgramGrantCountRegisters
  //
  PsfProgramGrantCountRegisters (
    &PsfTable,
    (REG_WITH_VALUE **)RegWithValueArray,
    ARRAY_SIZE (RegWithValueArray)
    );
}

/**
  Program P2SB as EOI target
**/
VOID
PtlPcdPPsfEnableP2SbEoiTarget (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  PSF_PORT_DEST_ID P2sbTarget = {
    .Fields = {
      .ChannelId = 0,
      .PortId = 11,
      .PortGroupId = 1,
      .PsfId = 8
    }
  };
  PsfEnableEoiTarget (
    &PsfTable,
    PtlPcdPPsfGetEoiRegDataTable (),
    P2sbTarget
    );
}

/**
  Set PCIe Root Port function in PSF

  @param[in] RpIndex       Root Port index (0 based)
  @param[in] NewFunction   New PCIe Root Port function
**/
VOID
PtlPcdPPsfSetPcieRpFunction (
  IN UINT32  RpIndex,
  IN UINT32  NewFunction
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;
  PSF_PCIE_PORT_DATA             PsfPcieRpPortData;

  ZeroMem (&PsfPcieRpPortData, sizeof (PSF_PCIE_PORT_DATA));
  PtlPcdPPsfGetPciePortData (RpIndex, &PsfPcieRpPortData);

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  PsfSetPcieFunction (&PsfTable, &PsfPcieRpPortData, NewFunction);
}

/**
  Reset registers that are not reset by themselves in every type of reset
  and reset Root Port functions

  @param[in] PcieRpFuncNumArray        PCIE RP Function number array
  @param[in] PcieRpFuncNumArraySize    Size of PcieRpFuncNumArray
**/
VOID
PtlPcdPPsfEarlyInit (
  IN  UINT8                     *PcieRpFuncNumArray,
  IN  UINT32                    PcieRpFuncNumArraySize
  )
{
  UINT32                         Index;
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  for (Index = 0; Index < PcieRpFuncNumArraySize && Index < PtlPcdPPsfGetMaxPsfDecodedPciePortNum (); Index++) {
    PtlPcdPPsfSetPcieRpFunction (Index, PcieRpFuncNumArray[Index]);
  }

  PsfResetEoiTargets (
    &PsfTable,
    PtlPcdPPsfGetEoiRegDataTable ()
    );

  PsfResetMctpTargets (
    &PsfTable,
    PtlPcdPPsfGetMctpRegDataTable ()
    );

  PtlPcdPPsfResetRootspaceConfig ();
}

/**
  Disable PCIe Root Port at PSF level

  @param[in]  RpIndex         PCIe Root Port Index (0 based)
**/
VOID
PtlPcdPPsfDisablePcieRootPort (
  IN UINT32  RpIndex
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;
  PSF_PCIE_PORT_DATA             PsfPcieRpPortData;

  ZeroMem (&PsfPcieRpPortData, sizeof (PSF_PCIE_PORT_DATA));
  PtlPcdPPsfGetPciePortData (RpIndex, &PsfPcieRpPortData);

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  PsfDisablePcieRootPort (&PsfTable, &PsfPcieRpPortData);
}

/**
  Enable PCIe Root Port at PSF level

  @param[in]  RpIndex         PCIe Root Port Index (0 based)
**/
VOID
PtlPcdPPsfEnablePcieRootPort (
  IN UINT32  RpIndex
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;
  PSF_PCIE_PORT_DATA             PsfPcieRpPortData;

  ZeroMem (&PsfPcieRpPortData, sizeof (PSF_PCIE_PORT_DATA));
  PtlPcdPPsfGetPciePortData (RpIndex, &PsfPcieRpPortData);

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  PsfEnablePcieRootPort (&PsfTable, &PsfPcieRpPortData);
}

/**
  Program Root Port as PSF EOI target
  @param[in] RpIndex   Root Port number
**/
VOID
PtlPcdPPsfEnableRpEoiTarget (
  UINT32  RpIndex
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  PsfEnableEoiTarget (
    &PsfTable,
    PtlPcdPPsfGetEoiRegDataTable (),
    PtlPcdPPsfPcieDestinationId (RpIndex)
    );
}

STATIC
VOID
PtlPcdPPsfProgramPsfGlobalConfig (
  IN UINT32    AndData32,
  IN UINT32    OrData32
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;
  UINT32                         PsfTableIndex;
  PSF_DEV                        *PsfDev;

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  for (PsfTableIndex = 0; PsfTableIndex < PsfTable.Size; PsfTableIndex++) {
    PsfDev = &PsfTable.Data[PsfTableIndex];
    PsfDev->Access->AndThenOr32 (PsfDev->Access, R_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG, AndData32, OrData32);
  }
}

/**
  Disable Trunk Clock Gating, Local Clock Gating and Partition Clock Gating
  for all PSFs in PSF topology
**/
VOID
PtlPcdPPsfDisableClockGating (
  VOID
  )
{
  UINT32                         AndData32;
  UINT32                         OrData32;

  AndData32 = (UINT32)~(
    B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENPCG |
    B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENTCG  |
    B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENLCG
    );

  OrData32 = 0;
  PtlPcdPPsfProgramPsfGlobalConfig (AndData32, OrData32);
}

/**
  Enable Trunk Clock Gating, Local Clock Gating and Partition Clock Gating
  for all PSFs in PSF topology
**/
VOID
PtlPcdPPsfEnableClockGating (
  VOID
  )
{
  UINT32                         AndData32;
  UINT32                         OrData32;

  AndData32 = (UINT32)~(0);

  OrData32 = (UINT32)(
    B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENPCG |
    B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENTCG  |
    B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENLCG
    );
  PtlPcdPPsfProgramPsfGlobalConfig (AndData32, OrData32);
}

/**
  Configure PSF power management.
  Must be called after all PSF configuration is completed.
**/
VOID
PtlPcdPPsfConfigurePowerManagement (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  SbAccessSegment[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  P2SB_CONTROLLER                P2SbController[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV                        PsfDevArray[PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE];
  PSF_DEV_TABLE                  PsfTable;
  UINT32                         PsfTableIndex;
  PSF_DEV                        *PsfDev;
  UINT32                         DataAnd32;
  UINT32                         DataOr32;

  DataAnd32 = ~0u;
  DataOr32 = (B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENLCG | B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENPCG);

  PtlPcdPPsfPreparePsfTable (
    PTL_PCD_P_NUM_ENTRIES_PSF_DEV_TABLE,
    &PsfTable,
    PsfDevArray,
    P2SbController,
    SbAccessSegment
    );

  //
  // There in an ordering requirement to program enTCG before enLCG.
  //
  for (PsfTableIndex = 0; PsfTableIndex < PsfTable.Size; PsfTableIndex++) {
    PsfDev = &PsfTable.Data[PsfTableIndex];
    PsfDev->Access->AndThenOr32 (PsfDev->Access, R_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG, ~0u, B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENTCG);
    PsfDev->Access->AndThenOr32 (PsfDev->Access, R_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG, DataAnd32, DataOr32);
  }
}