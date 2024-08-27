/** @file

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

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Pci30.h>
#include <Ppi/SiPolicy.h>
#include <LpssUartHandle.h>
#include <Register/LpssUartRegs.h>
#include <Library/LpssUartLib.h>

/**
  Enable MMIO access to LPSS Uart

  @param[in]   LpssUartController    Pointer to Lpss Uart Controller
**/
STATIC
VOID
LpssUartEnableMmio (
  LPSS_UART_CONTROLLER       *LpssUartController
  )
{
  DEBUG ((DEBUG_INFO, "\t\t%a\n", __FUNCTION__));
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR, (UINT32) LpssUartController->FixedMmio);
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR_HIGH, 0x0);
  LpssUartController->PcieCfgAccess->Or16 (LpssUartController->PcieCfgAccess, PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Disable MMIO access to LPSS Uart

  @param[in]   LpssUartController    Pointer to Lpss Uart Controller
  @param[in]   RemoveBar             Remove Bar
**/
STATIC
VOID
LpssUartDisableMmio (
  LPSS_UART_CONTROLLER       *LpssUartController,
  IN BOOLEAN                 RemoveBar
  )
{
  DEBUG ((DEBUG_INFO, "\t\t%a\n", __FUNCTION__));
  if (RemoveBar) {
    LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR, 0x0);
    LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR_HIGH, 0x0);
  }

  LpssUartController->PcieCfgAccess->And16 (LpssUartController->PcieCfgAccess, PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Gets UART out of reset

  @param[in]  MmioBaseAddress    Base address of UART MMIO space
**/
STATIC
VOID
LpssUartGetOutOfReset (
  IN UINTN            MmioBaseAddress
  )
{
  UINT16 ResetOffset;

  DEBUG ((DEBUG_INFO, "\t\t%a\n", __FUNCTION__));

  ResetOffset = R_LPSS_UART_MEM_RESETS;

  DEBUG ((DEBUG_INFO, "UART CTR: %x, %x\n", MmioRead32 (MmioBaseAddress + R_LPSS_UART_MEM_CTR), UART_COMPONENT_IDENTIFICATION_CODE));

  if (MmioRead32 (MmioBaseAddress + R_LPSS_UART_MEM_CTR) != UART_COMPONENT_IDENTIFICATION_CODE) {
    ResetOffset = R_LPSS_UART_MEM_RESETS/4; // Requires 8 bit offset
  }

  MmioOr8 (MmioBaseAddress + ResetOffset, B_LPSS_UART_MEM_RESETS_RESET_IP | B_LPSS_UART_MEM_RESETS_RESET_DMA);
}

/**
  Configures LPSS UART to work on fixed address assignments
  Disables Pci Enumaration for given device in PSF

  @param[in] LpssUartController        Pointer to Lpss Uart Controller
**/
STATIC
VOID
LpssUartSetFixedMmio (
  IN LPSS_UART_CONTROLLER   *LpssUartController
  )
{
  UINT32  Data32;

  DEBUG ((DEBUG_INFO, "\t\t%a\n", __FUNCTION__));
  //
  // Places Lpss device in D3
  //
  LpssUartController->PcieCfgAccess->Or32 (LpssUartController->PcieCfgAccess,
                                        R_LPSS_UART_CFG_PMECTRLSTATUS,
                                        B_LPSS_UART_CFG_PMECTRLSTATUS_POWERSTATE);

  //
  // Disable memory
  //
  LpssUartDisableMmio (LpssUartController, TRUE);

  //
  // Assign BAR0 and BAR1 (access to Pci Config Space)
  //
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR, (UINT32) LpssUartController->FixedMmio);
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR_HIGH, 0x0);
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR1, (UINT32) LpssUartController->FixedMmioPciCfgBaseAddr);
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR1_HIGH, 0x0);
  Data32 = LpssUartController->PcieCfgAccess->Read32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR);
  DEBUG ((DEBUG_INFO, "%a: PCI BAR 0= 0x%08X\n", __FUNCTION__, Data32));
  Data32 = LpssUartController->PcieCfgAccess->Read32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR1);
  DEBUG ((DEBUG_INFO, "%a: PCI BAR 1= 0x%08X\n", __FUNCTION__, Data32));

  //
  // Set D0 before placing device in Hidden Mode, otherwise memory will not map
  //
  LpssUartController->PcieCfgAccess->And32 (LpssUartController->PcieCfgAccess,
                                          R_LPSS_UART_CFG_PMECTRLSTATUS,
                                          (UINT32)~(B_LPSS_UART_CFG_PMECTRLSTATUS_POWERSTATE));
  //
  // Enable MSE
  //
  LpssUartController->PcieCfgAccess->Or16 (LpssUartController->PcieCfgAccess, PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Get controller out of reset
  //
  MmioOr8 (LpssUartController->FixedMmio + R_LPSS_UART_MEM_RESETS, B_LPSS_UART_MEM_RESETS_RESET_IP | B_LPSS_UART_MEM_RESETS_RESET_DMA);
}

/**
  Enables Lpss Uart Power Gating

  @param[in] LpssUartController        Pointer to Lpss Uart Controller
**/
STATIC
VOID
LpssEnablePowerGating (
  IN LPSS_UART_CONTROLLER   *LpssUartController
  )
{
  DEBUG ((DEBUG_INFO, "\t\t%a\n", __FUNCTION__));
  LpssUartController->PcieCfgAccess->Or32 (LpssUartController->PcieCfgAccess,
                                          R_LPSS_UART_CFG_D0I3_MAX_POW_LAT_PG_CONFIG,
                                          B_LPSS_UART_PCR_D0I3_MAX_POW_LAT_PG_CONFIG_PMCRE |
                                          B_LPSS_UART_PCR_D0I3_MAX_POW_LAT_PG_CONFIG_DEVIDLEN |
                                          B_LPSS_UART_PCR_D0I3_MAX_POW_LAT_PG_CONFIG_D3HEN);
}

/**
  Configures Lpss Uart to work on fixed address assignment only through SEC/PEI phase.
  In DXE MMIO will be reassigned.

  Note: Fixed memory is not allocated in OS as MotherBoard/Device resource

  @param[in] LpssUartController        Pointer to Lpss Uart Controller
**/
STATIC
VOID
LpssUartPciSetFixedMmio (
  IN LPSS_UART_CONTROLLER   *LpssUartController
  )
{
  DEBUG ((DEBUG_INFO, "\t\t%a\n", __FUNCTION__));
  //
  // Places Lpss Uart device in D3
  //
  LpssUartController->PcieCfgAccess->Or32 (LpssUartController->PcieCfgAccess,
                                        R_LPSS_UART_CFG_PMECTRLSTATUS,
                                        B_LPSS_UART_CFG_PMECTRLSTATUS_POWERSTATE);

  LpssUartController->PcieCfgAccess->And16 (LpssUartController->PcieCfgAccess, PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);

  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR, (UINT32) LpssUartController->FixedMmio);
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR_HIGH, 0x0);
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR1,  0x0);
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR1_HIGH, 0x0);

  LpssUartController->PcieCfgAccess->And32 (LpssUartController->PcieCfgAccess,
                                          R_LPSS_UART_CFG_PMECTRLSTATUS,
                                          ~(UINT32)B_LPSS_UART_CFG_PMECTRLSTATUS_POWERSTATE);

  LpssUartController->PcieCfgAccess->Or16 (LpssUartController->PcieCfgAccess, PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Get controller out of reset
  //
  MmioOr8 (LpssUartController->FixedMmio + R_LPSS_UART_MEM_RESETS, B_LPSS_UART_MEM_RESETS_RESET_IP | B_LPSS_UART_MEM_RESETS_RESET_DMA);
}

/**
  Configures Lpss Uart to work in Hidden/Com mode.

  Note: Fixed memory is not allocated in OS as MotherBoard/Device resource

  @param[in] LpssUartController        Pointer to Lpss Uart Controller
  @param[in] LpssUartDeviceConfig            Pointer to Lpss Uart Config
  @param[in] LpssUartPrivateConfig     Pointer to Lpss Uart Private Config
**/
STATIC
VOID
LpssUartHiddenComModeInit (
  LPSS_UART_CONTROLLER       *LpssUartController,
  LPSS_UART_DEVICE_CONFIG    *LpssUartDeviceConfig,
  LPSS_UART_PRIVATE_CONFIG   *LpssUartPrivateConfig
  )
{
  DEBUG ((DEBUG_INFO, "\t%a\n", __FUNCTION__));

  LpssUartSetFixedMmio (LpssUartController);

  LpssUartGetOutOfReset (LpssUartController->FixedMmio);

  LpssUartSetAttributes (
    LpssUartController->FixedMmio,
    LpssUartDeviceConfig->Attributes.BaudRate,
    LpssUartDeviceConfig->Attributes.Parity,
    LpssUartDeviceConfig->Attributes.DataBits,
    LpssUartDeviceConfig->Attributes.StopBits,
    LpssUartDeviceConfig->Attributes.AutoFlow
  );
}

/**
  Disable Lpss Uart Device

  Note: Fixed memory is not allocated in OS as MotherBoard/Device resource

  @param[in] LpssUartController        Pointer to Lpss Uart Controller
  @param[in] LpssUartDeviceConfig            Pointer to Lpss Uart Config
  @param[in] LpssUartPrivateConfig     Pointer to Lpss Uart Private Config
**/
STATIC
VOID
LpssUartDisabledDevice (
  IN LPSS_UART_CONTROLLER       *LpssUartController,
  IN LPSS_UART_DEVICE_CONFIG    *LpssUartDeviceConfig,
  IN LPSS_UART_PRIVATE_CONFIG   *LpssUartPrivateConfig
  )
{
  DEBUG ((DEBUG_INFO, "\t%a\n", __FUNCTION__));

  LpssUartEnableMmio (LpssUartController);

  //
  // Places Lpss device in D3
  //
  LpssUartController->PcieCfgAccess->Or32 (LpssUartController->PcieCfgAccess,
                                        R_LPSS_UART_CFG_PMECTRLSTATUS,
                                        B_LPSS_UART_CFG_PMECTRLSTATUS_POWERSTATE);
  //
  // Disable MMIO access
  //
  LpssUartDisableMmio (LpssUartController, LpssUartController->TempBar);
}

/**
  Configures Lpss Uart to work in Pci mode.

  Note: Fixed memory is not allocated in OS as MotherBoard/Device resource

  @param[in] LpssUartController        Pointer to Lpss Uart Controller
  @param[in] LpssUartDeviceConfig            Pointer to Lpss Uart Config
  @param[in] LpssUartPrivateConfig     Pointer to Lpss Uart Private Config
**/
STATIC
VOID
LpssUartPciModeInit (
  LPSS_UART_CONTROLLER       *LpssUartController,
  LPSS_UART_DEVICE_CONFIG    *LpssUartDeviceConfig,
  LPSS_UART_PRIVATE_CONFIG   *LpssUartPrivateConfig
  )
{
  DEBUG ((DEBUG_INFO, "\t%a\n", __FUNCTION__));

  //
  // Clear Bar1
  //
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR1, 0x0);
  LpssUartController->PcieCfgAccess->Write32 (LpssUartController->PcieCfgAccess, R_LPSS_UART_CFG_BAR1_HIGH, 0x0);

  LpssUartPciSetFixedMmio (LpssUartController);
  //
  // Uart in Pci mode does not have any resources assigned through SEC/PEI, except tremporary MMIO BAR.
  // To enable it in that stage set PowerGating to Enabled.
  // It will assign UART fixed MMIO and leave it in D0 after initial configuration.
  //
  if (LpssUartDeviceConfig->PowerGating == LpssUartPgEnabled) {
    LpssUartController->TempBar = FALSE;
  } else {
    LpssUartController->TempBar = TRUE;
  }

  LpssUartGetOutOfReset (LpssUartController->FixedMmio);

  LpssUartSetAttributes (
    LpssUartController->FixedMmio,
    LpssUartDeviceConfig->Attributes.BaudRate,
    LpssUartDeviceConfig->Attributes.Parity,
    LpssUartDeviceConfig->Attributes.DataBits,
    LpssUartDeviceConfig->Attributes.StopBits,
    LpssUartDeviceConfig->Attributes.AutoFlow
  );

  if (LpssUartDeviceConfig->PowerGating != LpssUartPgEnabled) {
    //
    // Set D3
    //
    LpssUartController->PcieCfgAccess->Or32 (LpssUartController->PcieCfgAccess,
                                          R_LPSS_UART_CFG_PMECTRLSTATUS,
                                          B_LPSS_UART_CFG_PMECTRLSTATUS_POWERSTATE);

    LpssUartDisableMmio (LpssUartController, LpssUartController->TempBar);
  }
}

/**
  Configures LPSS Uart Controller

  @param[in] LpssUartHandle        The Lpss UART Handle instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       Usupported UART controller mode
              EFI_ABORTED           Device not present or unsupported Uart Mode
              EFI_INVALID_PARAMETER Invalid parameter
**/
EFI_STATUS
LpssUartInit (
  IN LPSS_UART_HANDLE   *LpssUartHandle
  )
{
  LPSS_UART_CONTROLLER       *LpssUartController;
  LPSS_UART_DEVICE_CONFIG    *LpssUartDeviceConfig;
  LPSS_UART_PRIVATE_CONFIG   *LpssUartPrivateConfig;

  if (LpssUartHandle == NULL) {
    DEBUG ((DEBUG_INFO, "LpssUartHandle == NULL. %a () - End.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a (Ctrl #%d)\n", __FUNCTION__, LpssUartHandle->ControllerId));

  LpssUartController         = LpssUartHandle->Controller;
  LpssUartDeviceConfig       = LpssUartHandle->UartConfig;
  LpssUartPrivateConfig      = &LpssUartHandle->PrivateConfig;

  if (LpssUartController->PcieCfgAccess->Read16 (LpssUartController->PcieCfgAccess, 0) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "LPSS UART not present - initialization aborted!\n"));
    return EFI_ABORTED;
  }

  if (LpssUartDeviceConfig->Mode == LpssUartSkipInit) {
    DEBUG ((DEBUG_INFO, "Lpss Uart initialization skipped. %a - End.\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  LpssUartController->TempBar = FALSE;

  DEBUG ((DEBUG_INFO, "LPSS Uart VID: 0x%X, DID: 0x%x.\n", LpssUartController->PcieCfgAccess->Read16 (LpssUartController->PcieCfgAccess, 0), LpssUartController->PcieCfgAccess->Read16 (LpssUartController->PcieCfgAccess, PCI_DEVICE_ID_OFFSET)));

  LpssEnablePowerGating (LpssUartController);

  switch (LpssUartDeviceConfig->Mode) {
    case LpssUartDisabled:
      LpssUartDisabledDevice (LpssUartController, LpssUartDeviceConfig, LpssUartPrivateConfig);
      break;
    case LpssUartHidden:
    case LpssUartCom:
      LpssUartHiddenComModeInit (LpssUartController, LpssUartDeviceConfig, LpssUartPrivateConfig);
      break;
    case LpssUartPci:
      LpssUartPciModeInit (LpssUartController, LpssUartDeviceConfig, LpssUartPrivateConfig);
      break;
    default:
      DEBUG ((DEBUG_INFO, "LPSS Uart mode: %d not supported!\n%a () - End.\n", LpssUartDeviceConfig->Mode, __FUNCTION__));
      return EFI_ABORTED;
  }
  return EFI_SUCCESS;
}
