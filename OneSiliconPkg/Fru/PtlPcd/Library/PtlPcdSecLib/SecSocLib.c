/** @file
  SEC SOC library in C.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PcdPeiLpssI2cSocInitLib.h>
#include <Library/PeiLpssI2cInitLib.h>
#include <Library/PmcLib.h>
#include <Library/PrintLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PchInfoLib.h>
#include <Library/DebugLib.h>

#include <Register/P2sbRegs.h>
#include <Register/SmbusRegs.h>
#include <Register/RtcRegs.h>
#include <Register/TraceHubRegs.h>
#include <IndustryStandard/Pci.h>
#include <LpssI2cConfig.h>
#include <LpssI2cHandle.h>
#include <Library/SecPchLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <IncludePrivate/PtlPcdSbPortIds.h>
#include <Library/SecLpssUartInitLib.h>
#include <Library/LpssUartLib.h>
#include <Register/TcoRegs.h>

/**
  Serial Io Uart Debug Configuration Wrapper

  @param[out] UartDeviceConfig           A pointer to the SERIAL_IO_UART_CONFIG.
  @param[out] LpssUartDebugEnable    Serial Io Uart Debug Enable/Disable.
  @param[out] LpssUartNumber         The Number of Serial Io Uart.
  @param[out] LpssUartPciMmioBase    MMIO Base Address by default in PCI Mode
**/
VOID
LpssUartDebugConfigurationWrapper (
  OUT LPSS_UART_DEVICE_CONFIG  *UartDeviceConfig,
  OUT UINT8                    *LpssUartDebugEnable,
  OUT UINT8                    *LpssUartNumber,
  OUT UINT32                   *LpssUartPciMmioBase
  );

/**
  Serial Io Additional Uart Configuration Wrapper
  Allows to configure another UART in FSPT

  @param[out] UartEnable                 Serial Io Additional Uart Enable/Disable.
  @param[out] UartDeviceConfig           A pointer to the SERIAL_IO_UART_CONFIG.
  @param[out] LpssUartNumber         The Number of Serial Io Uart.
  @param[out] LpssUartPciMmioBase    MMIO Base Address by default in PCI Mode
**/
VOID
Lpss2ndUartConfigurationWrapper (
  OUT UINT8                    *UartEnable,
  OUT LPSS_UART_DEVICE_CONFIG  *UartDeviceConfig,
  OUT UINT8                    *LpssUartNumber,
  OUT UINT32                   *LpssUartPciMmioBase
  );


/**
 Lpss I2C Configuration Wrapper

  @param[out] I2cDeviceConfig           A pointer to the LPSS_I2C_CONTROLLER_CONFIG.
  @param[out] LpssI2cNumber             The Number of Lpss I2c.
  @param[out] LpssI2cNumber             MMIO Base Address by default in PCI Mode
**/
VOID
LpssI2cConfigurationWrapper (
  OUT LPSS_I2C_CONTROLLER_CONFIG *I2cDeviceConfig,
  OUT UINT8                      *LpssI2cNumber,
  OUT UINT64                     *LpssI2cMmioBase
  );

/**
  This is helper function to initialize SerialIoSpi in early init.
**/
VOID
EFIAPI
SerialIoSpiEarlyinitalization (
  VOID
  )
{

}

/**
  This is helper function to initialize SerialIoI2c in early init.
**/
VOID
EFIAPI
SerialIoI2cEarlyinitalization (
  VOID
  )
{
  EFI_STATUS                  Status;
  LPSS_I2C_HANDLE             I2cHandle;
  LPSS_I2C_CONTROLLER_CONFIG  I2cDeviceConfig;
  UINT8                       LpssI2cNumber;
  UINT64                      LpssI2cMmioBase;
  REGISTER_ACCESS             *PciCfgAccess;

  LpssI2cConfigurationWrapper (&I2cDeviceConfig, &LpssI2cNumber, &LpssI2cMmioBase);

  Status = PtlPcdLpssI2cBuildHandle (NULL, &I2cHandle, &I2cDeviceConfig, LpssI2cNumber);
  if (Status == EFI_UNSUPPORTED) {
    return;
  }

  if (I2cDeviceConfig.Mode == 0) {
    return;
  }

  PciCfgAccess = I2cHandle.Controller->PcieCfgAccess;

  PciCfgAccess->Write32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET, (UINT32)LpssI2cMmioBase);
  if (PciCfgAccess->Read32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciCfgAccess->Write32 (PciCfgAccess, PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (LpssI2cMmioBase, 32));
  }

  LpssI2cInit (&I2cHandle);
}

/**
  This is helper function to initialize SerialIoUart in early init.
**/
VOID
EFIAPI
LpssUartDebugConfiguration (
  VOID
  )
{
  LPSS_UART_DEVICE_CONFIG  UartDeviceConfig;
  UINT8                    LpssUartDebugEnable;
  UINT8                    LpssUartNumber;
  UINT32                   LpssUartPciMmioBase;
  CHAR8                    CarInitBuffer[32];

  AsciiSPrint (CarInitBuffer, sizeof (CarInitBuffer), "FSP-T: CAR Init\n");
  LpssUartDebugConfigurationWrapper (&UartDeviceConfig, &LpssUartDebugEnable, &LpssUartNumber, &LpssUartPciMmioBase);
  UartDeviceConfig.DBG2      = FALSE;
  UartDeviceConfig.DmaEnable = FALSE;

  //
  // Initialize LpssUart UART for debug message
  //
  if (LpssUartDebugEnable == 1) {
    SecLpssUartConfiguration (LpssUartNumber, &UartDeviceConfig);
    LpssUartWrite (
        GetLpssUartFixedMmioAddress (LpssUartNumber),
        (UINT8 *)CarInitBuffer, 
        AsciiStrLen (CarInitBuffer));
  }
}

/**
  This function do the PCH cycle decoding initialization.
**/
VOID
EFIAPI
EarlyCycleDecoding (
  VOID
  )
{
  UINT64           P2sbBase;
  UINT64           P2sbMmioBase;
  UINT64           SmbusBase;
  UINT64           TraceHubPciBase;
  P2SB_PORT_16_ID  P2SBPid;

  //
  // Enable PCR base address in PCH
  //
  P2sbMmioBase = PCH_PCR_BASE_ADDRESS;
  P2sbBase = P2sbPciCfgBase ();
  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_SBREG_BAR, (UINT32) P2sbMmioBase);
  if (PciSegmentRead32 (P2sbBase + R_P2SB_CFG_SBREG_BAR) & BIT2) {
    PciSegmentWrite32 (
      P2sbBase + R_P2SB_CFG_SBREG_BAR + 4,
      (UINT32) RShiftU64 (P2sbMmioBase, 32)
      );
  };
  //
  // Enable P2SB MSE
  //
  PciSegmentOr8 (P2sbBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Enable PCR base address in PCH - second P2SB instance
  //
  P2sbMmioBase = PCH_PCR_SECOND_BASE_ADDRESS;
  P2sbBase = P2sb16bPciCfgBase ();
  PciSegmentWrite32 (P2sbBase + R_P2SB_CFG_SBREG_BAR, (UINT32) P2sbMmioBase);
  if (PciSegmentRead32 (P2sbBase + R_P2SB_CFG_SBREG_BAR) & BIT2) {
    PciSegmentWrite32 (
      P2sbBase + R_P2SB_CFG_SBREG_BAR + 4,
      (UINT32) RShiftU64 (P2sbMmioBase, 32)
      );
  };
  //
  // Enable P2SB MSE
  //
  PciSegmentOr8 (P2sbBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Program PWRM BASE
  //
  PmcSetPwrmBase (PmcPciCfgBase (), PCH_PWRM_BASE_ADDRESS);

  //
  // Program ACPI Base.
  //
  PtlPcdPsfSetPmcAbase (PcdGet16 (PcdAcpiBaseAddress));

  //
  // Program and Enable TCO Base
  //
  PchTcoBaseSet (PcdGet16 (PcdTcoBaseAddress));

  ///
  /// Halt the TCO timer as early as possible
  ///
  IoWrite16 (PcdGet16 (PcdTcoBaseAddress) + R_TCO_IO_TCTL1, B_TCO_IO_TCTL1_TCO_TMR_HALT);

  SmbusBase = SmbusPciCfgBase ();
  //
  // Set SMBus PCI 0x64 = 0x0A0A0000
  //
  PciSegmentWrite32 (SmbusBase + R_SMBUS_CFG_HTIM, 0x0A0A0000);
  //
  // Initialize SMBUS IO BAR
  //
  PciSegmentWrite16 (SmbusBase + R_SMBUS_CFG_SBA, PcdGet16 (PcdSmbusBaseAddress));
  //
  // Enable the Smbus I/O Enable
  //
  PciSegmentOr8 (SmbusBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_IO_SPACE);
  //
  // Enable the SMBUS Controller
  //
  PciSegmentOr8 (SmbusBase + R_SMBUS_CFG_HCFG, B_SMBUS_CFG_HCFG_HSTEN);

  //
  // Enable the upper 128-byte bank of RTC RAM.
  // PCR [RTC] + 0x3400 [2] = 1
  P2SBPid.Pid16bit = PTL_SID_F2_PID_RTC_HOST;
  PchPcrAndThenOr32 (P2SBPid.PortId.LocalPid, R_RTC_PCR_RC, (UINT32) ~0, B_RTC_PCR_RC_UE);

  TraceHubPciBase = PchTraceHubPciCfgBase ();
  //
  // Program FW_LBAR (PCI offset 0x70)
  //
  PciSegmentWrite32 (TraceHubPciBase + R_TRACE_HUB_CFG_FW_LBAR, (UINT32) PCH_TRACE_HUB_FW_BASE_ADDRESS);
  //
  // Program FW_UBAR (PCI offset 0x74)
  //
  PciSegmentWrite32 (TraceHubPciBase + R_TRACE_HUB_CFG_FW_UBAR, (UINT32) RShiftU64 (PCH_TRACE_HUB_FW_BASE_ADDRESS, 32));
  //
  // Program FW_BAR to TraceHub phantom device BAR0 and BAR1 as FW_LBAR and FW_UBAR from PSF
  //
  PtlPcdPsfSetTraceHubAcpiBar (0, (UINT32) PCH_TRACE_HUB_FW_BASE_ADDRESS);
  PtlPcdPsfSetTraceHubAcpiBar (1, (UINT32) RShiftU64 (PCH_TRACE_HUB_FW_BASE_ADDRESS, 32));
  //
  // Enable MSE of TraceHub phantom device from PSF
  //
  PtlPcdPsfEnableTraceHubAcpiMemSpace ();
}
