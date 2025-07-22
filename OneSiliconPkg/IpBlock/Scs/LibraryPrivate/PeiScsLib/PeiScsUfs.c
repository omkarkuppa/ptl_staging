/** @file
  Initializes UFS host controller located on SCS Iosf2Ocp bridge.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Register/ScsUfsRegs.h>
#include "PeiScsInitInternal.h"

/**
  Fill UIC Command associated fields.

  @param[in] UicOpcode        The opcode of the UIC command.
  @param[in] Arg1Mib          The value for MIB Attribute of UIC command 1st argument.
  @param[in] Arg1GenSel       The value for Gen Selector Index of UIC command 1st argument.
  @param[in] Arg2             The value for UIC command 2nd argument.
  @param[in] Arg3             The value for UIC command 3rd argument.
**/
VOID
UfsFillUicCommand (
  IN     UINT8             UicOpcode,
  IN     UINT16            Arg1Mib,
  IN     UINT16            Arg1GenSel,
  IN     UINT32            Arg2,
  IN     UINT32            Arg3,
  IN OUT EDKII_UIC_COMMAND *Cmd
  )
{
  Cmd->UicOpcode = UicOpcode;
  Cmd->Arg1   = UFS_UIC_ARG_MIB_SEL (Arg1Mib, Arg1GenSel);
  Cmd->Arg2   = Arg2;
  Cmd->Arg3   = Arg3;
}

/**
  Waits until test condition is satisfied. This function guarantees that
  timeout will be at least as long as the one specified.

  @param[in] RegisterAddress  Address of the register to test
  @param[in] RegisterMask     Value of the mask to apply to the register
  @param[in] TestValue        Test value against which a register is tested
  @param[in] TimeoutUs        Value of the timeout in us granularity

  @retval TRUE if register assumed test value before timeout, FALSE otherwise
**/
STATIC
BOOLEAN
WaitUntil (
  IN UINTN   RegisterAddress,
  IN UINT32  RegisterMask,
  IN UINT32  TestValue,
  IN UINT32  TimeoutUs
  )
{
  while (TimeoutUs > 0) {
    if ((MmioRead32 (RegisterAddress) & RegisterMask) == TestValue) {
      return TRUE;
    }
    MicroSecondDelay (20);
    TimeoutUs -= 10;
  }
  return FALSE;
}

/**
  Performs UIC Command programming.
  This function assumes that UFS HC has already been enabled.

  @param[in] MmioBase  Address of the MMIO base
**/
STATIC
EFI_STATUS
UfsExecUicCommand (
  IN  UINTN                       MmioBase,
  IN OUT EDKII_UIC_COMMAND        *UicCommand
  )
{
  BOOLEAN ControllerReady;
  BOOLEAN IsCommandComplete;

  //
  // Clear IS.UCCS status
  //
  MmioAnd32 (MmioBase + R_SCS_MEM_UFS_IS, ~(UINT32)B_SCS_MEM_UFS_IS_UCCS);

  ControllerReady = WaitUntil (
                      MmioBase + R_SCS_MEM_UFS_HC_STATUS,
                      B_SCS_MEM_UFS_HCS_UCRDY,
                      B_SCS_MEM_UFS_HCS_UCRDY,
                      UIC_CMD_TIMEOUT);
  if (ControllerReady) {
    //
    // Set Attribute
    //
    MmioWrite32 (MmioBase + R_SCS_MEM_UFS_UCMD_ARG1, UicCommand->Arg1);
    MmioWrite32 (MmioBase + R_SCS_MEM_UFS_UCMD_ARG2, UicCommand->Arg2);
    MmioWrite32 (MmioBase + R_SCS_MEM_UFS_UCMD_ARG3, UicCommand->Arg3);
    MmioWrite32 (MmioBase + R_SCS_MEM_UFS_UIC_CMD, UicCommand->UicOpcode);
    // Check for IS.Bit10 UIC Command Completion Status (UCCS)
    // This bit is set to '1' by the host controller upon completion of a UIC command.
    IsCommandComplete = WaitUntil (
                          MmioBase + R_SCS_MEM_UFS_IS,
                          B_SCS_MEM_UFS_IS_UCCS,
                          B_SCS_MEM_UFS_IS_UCCS,
                          UIC_CMD_TIMEOUT);
    if (IsCommandComplete == FALSE) {
      DEBUG ((DEBUG_ERROR, "UIC Command Didnot complete \n"));
      return EFI_NOT_READY;
    }

    UicCommand->Arg2 = MmioRead32 (MmioBase + R_SCS_MEM_UFS_UCMD_ARG2);
    UicCommand->Arg3 = MmioRead32 (MmioBase + R_SCS_MEM_UFS_UCMD_ARG3);
    return EFI_SUCCESS;
  } else {
    DEBUG ((DEBUG_ERROR, "UFS not ready for UIC programming\n"));
    return EFI_NOT_READY;
  }
}

/**
  Scs UFS MPhy ULP.

  @param[in] ScsUfsHandle  UFS handle
**/
EFI_STATUS
ScsUfsMPhyUlp (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  EFI_STATUS         Status;
  EDKII_UIC_COMMAND  UicCommand;

  ScsControllerEnableMmio (ScsUfsHandle->Controller.PciCfgBase, (UINTN) ScsUfsHandle->Controller.MmioBase);

  UfsFillUicCommand (UfsUicDmeSet, ULP_REG, 0, 0, 0x5, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet ULP_REG = %r\n", Status));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeSet, VS_MPHYCFG_UPDT, 0, 0, 0x1, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet VS_MPHYCFG_UPDT = %r\n", Status));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeGet, ULP_REG, 0, 0, 0, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UfsUicDmeGet ULP_REG = %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "ScsUfsMPhyUlp ULP_REG = %X\n", UicCommand.Arg3));

  ScsControllerDisableMmio (ScsUfsHandle->Controller.PciCfgBase);

  return EFI_SUCCESS;
}

/**
  Scs UFS MPhy Enable.

  @param[in] ScsUfsHandle  UFS handle
**/
EFI_STATUS
ScsUfsMPhyEnable (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  EFI_STATUS         Status;
  EDKII_UIC_COMMAND  UicCommand;

  ScsControllerEnableMmio (ScsUfsHandle->Controller.PciCfgBase, (UINTN) ScsUfsHandle->Controller.MmioBase);

  UfsFillUicCommand (UfsUicDmeSet, MPHY_DISABLE, 0, 0, 0x0, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet MPHY_DISABLE = %r\n", Status));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeGet, MPHY_DISABLE, 0, 0, 0, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UfsUicDmeGet MPHY_DISABLE = %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "ScsUfsMPhyEnable MPHY_DISABLE = %X\n", UicCommand.Arg3));

  ScsControllerDisableMmio (ScsUfsHandle->Controller.PciCfgBase);

  return EFI_SUCCESS;
}

/**
  Scs UFS MPhy F/W Trigger.

  @param[in] ScsUfsHandle  UFS handle
**/
EFI_STATUS
ScsUfsFwTrigger (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  EFI_STATUS         Status;
  EDKII_UIC_COMMAND  UicCommand;

  ScsControllerEnableMmio (ScsUfsHandle->Controller.PciCfgBase, (UINTN) ScsUfsHandle->Controller.MmioBase);

  UfsFillUicCommand (UfsUicDmeSet, SRAM_CFG_8116, 0, 0, 0x38, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet SRAM_CFG_8116 = %r\n", Status));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeSet, SRAM_CFG_8117, 0, 0, 0x20, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet SRAM_CFG_8117 %r\n", Status));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeSet, SRAM_CFG_8118, 0, 0, 0xC, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet SRAM_CFG_8118 %r\n", Status));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeSet, SRAM_CFG_8119, 0, 0, 0x0, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet SRAM_CFG_8119 %r\n", Status));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeSet, SRAM_CFG_811C, 0, 0, 0x1, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet SRAM_CFG_811C %r\n", Status));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeSet, VS_MPHYCFG_UPDT, 0, 0, 0x1, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet VS_MPHYCFG_UPDT %r\n", Status));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeGet, SRAM_CFG_8116, 0, 0, 0, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UfsUicDmeGet SRAM_CFG_8116 = %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "ScsUfsFwTrigger SRAM_CFG_8116 = %X\n", UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, SRAM_CFG_8117, 0, 0, 0, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UfsUicDmeGet SRAM_CFG_8117 = %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "ScsUfsFwTrigger SRAM_CFG_8117 = %X\n", UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, SRAM_CFG_8118, 0, 0, 0, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UfsUicDmeGet SRAM_CFG_8118 = %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "ScsUfsFwTrigger SRAM_CFG_8118 = %X \n", UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, SRAM_CFG_8119, 0, 0, 0, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UfsUicDmeGet SRAM_CFG_8119 = %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "ScsUfsFwTrigger SRAM_CFG_8119 = %X\n", UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, SRAM_CFG_811C, 0, 0, 0, &UicCommand);
  Status = UfsExecUicCommand (ScsUfsHandle->Controller.MmioBase, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UfsUicDmeGet SRAM_CFG_811C = %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "ScsUfsFwTrigger SRAM_CFG_811C = %X\n", UicCommand.Arg3));

  ScsControllerDisableMmio (ScsUfsHandle->Controller.PciCfgBase);

  return EFI_SUCCESS;
}

/**
  Scs UFS Controller Ahit Programming.

  @param[in] ScsUfsHandle  UFS handle
**/
EFI_STATUS
ScsUfsProgramAhit (
  IN SCS_UFS_HANDLE  *ScsUfsHandle,
  IN UINT32           AhitValue
  )
{
  ScsControllerEnableMmio (ScsUfsHandle->Controller.PciCfgBase, (UINTN) ScsUfsHandle->Controller.MmioBase);
  //
  // Write Ahit_Value to the AHIT register.
  //
  MmioWrite32 (ScsUfsHandle->Controller.MmioBase + UFS_HC_AHIT_OFFSET, AhitValue);

  ScsControllerDisableMmio (ScsUfsHandle->Controller.PciCfgBase);

  return EFI_SUCCESS;
}

/**
  Configure Ufs controller

  @param[in] PciBaseAddress  PCI config base address of the controller
  @param[in] MmioBase        MMIO base address
**/
STATIC
VOID
ScsUfsInitMmioRegisters (
  IN UINT64  PciBaseAddress,
  IN UINTN   MmioBase
  )
{
  ScsControllerEnableMmio (PciBaseAddress, MmioBase);
  //
  // Configure UFS REF_CLK
  //
  MmioWrite32 (MmioBase + R_UFS_MEM_REF_CLK, V_UFS_MEM_REF_CLK);
  ScsControllerDisableMmio (PciBaseAddress);
}

/**
  Configure Ufs MMIO Register AUTO_LTR_VALUE (Offset 0x808)

  @param[in] PciBaseAddress  PCI config base address of the controller
  @param[in] MmioBase        MMIO base address
**/
STATIC
VOID
ScsUfsSetMmioAutoLtrRegister (
  IN UINT64  PciBaseAddress,
  IN UINTN   MmioBase
  )
{
  ScsControllerEnableMmio (PciBaseAddress, MmioBase);
  //
  // Configure UFS AUTO_LTR_VALUE (Offset 0x808) register with the below values -
  //  auto_ltr_val.reserved0=0x0
  //  auto_ltr_val.snoop_requirment=0x1
  //  auto_ltr_val.reserved_low_2=0x0
  //  auto_ltr_val.snoop_latency_scale=0x2
  //  auto_ltr_val.snoop_value=0x28A (650usec)
  //
  MmioWrite32 (MmioBase + R_SCS_MEM_UFS_AUTO_LTR_VALUE, 0x8A8A);
  ScsControllerDisableMmio (PciBaseAddress);
}

/**
  Enables UFS controller.

  @param[in] ScsUfsHandle  Handle.
**/
STATIC
VOID
ScsUfsEnable (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  DEBUG ((DEBUG_INFO, "Enabling UFS\n"));

  Iosf2OcpDisableBar1 (ScsUfsHandle->Controller.Iosf2OcpPort);
  ScsUfsHandle->Callbacks.Bar1Disable (ScsUfsHandle);
  if (ScsUfsHandle->Controller.Mmp) {
    MmpInit (ScsUfsHandle->Controller.Mmp);
  }
  if (!ScsUfsHandle->SocConfig.IsBootMedium) {
    Iosf2OcpEnableUfs (ScsUfsHandle->Controller.Iosf2OcpPort, ScsUfsHandle->SocConfig.NumOfLanes);
    ScsUfsInitMmioRegisters (ScsUfsHandle->Controller.PciCfgBase, (UINTN) ScsUfsHandle->Controller.MmioBase);
  }
  if (ScsUfsHandle->Config->InlineEncryption) {
    Iosf2OcpEnableUfsInlineEncryption (ScsUfsHandle->Controller.Iosf2OcpPort);
  }
  ScsUfsSetMmioAutoLtrRegister (ScsUfsHandle->Controller.PciCfgBase, (UINTN) ScsUfsHandle->Controller.MmioBase);
  Iosf2OcpConfigureInterrupts (
    ScsUfsHandle->Controller.Iosf2OcpPort,
    ScsUfsHandle->SocConfig.IntPin,
    ScsUfsHandle->SocConfig.Irq
    );
}

/**
  Disable UFS controller.

  @param[in] ScsUfsHandle  Handle.
**/
STATIC
VOID
ScsUfsDisable (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  DEBUG ((DEBUG_INFO, "Disabling UFS\n"));

  Iosf2OcpDisableBar1 (ScsUfsHandle->Controller.Iosf2OcpPort);
  ScsUfsHandle->Callbacks.Bar1Disable (ScsUfsHandle);
  PciSegmentOr16 (
    ScsUfsHandle->Controller.PciCfgBase + R_SCS_CFG_PG_CONFIG,
    (B_SCS_CFG_PG_CONFIG_HAE | B_SCS_CFG_PG_CONFIG_SE| B_SCS_CFG_PG_CONFIG_PGE | B_SCS_CFG_PG_CONFIG_I3E)
    );
  ScsControllerPutToD3 (ScsUfsHandle->Controller.PciCfgBase);
  if (ScsUfsHandle->Controller.Mmp) {
    MmpDisable (ScsUfsHandle->Controller.Mmp);
  }
  Iosf2OcpDisableUfs (ScsUfsHandle->Controller.Iosf2OcpPort);
  ScsUfsHandle->Callbacks.Disable (ScsUfsHandle);
}

/**
  Prints UFS SoC config into debug port.

  @param[in] ScsUfsHandle  UFS handle
**/
VOID
ScsUfsPrintSocConfig (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  DEBUG ((DEBUG_INFO, "Printing UFS SoC config\n"));
  DEBUG ((DEBUG_INFO, "IsBootMedium: %d\n", ScsUfsHandle->SocConfig.IsBootMedium));
  DEBUG ((DEBUG_INFO, "IsFuseDisabled: %d\n", ScsUfsHandle->SocConfig.IsFuseDisabled));
  DEBUG ((DEBUG_INFO, "IsMPhyUlpUpdateRequired: %d\n", ScsUfsHandle->SocConfig.IsMPhyUlpUpdateRequired));
  DEBUG ((DEBUG_INFO, "NumOfLanes: %d\n", ScsUfsHandle->SocConfig.NumOfLanes));
  DEBUG ((DEBUG_INFO, "IntPin: %d\n", ScsUfsHandle->SocConfig.IntPin));
  DEBUG ((DEBUG_INFO, "Irq: %d\n", ScsUfsHandle->SocConfig.Irq));
}

/**
  Configure UFS controller in SCS.

  @param[in] ScsUfsHandle  Handle.
**/
VOID
ScsUfsInit (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  EFI_STATUS       Status;

  DEBUG ((DEBUG_INFO, "UFS init start\n"));

  ScsUfsPrintSocConfig (ScsUfsHandle);

  if (ScsUfsHandle->SocConfig.NumOfLanes == 0) {
    ScsUfsHandle->UfsInfo->Supported = FALSE;
  } else {
    ScsUfsHandle->UfsInfo->Supported = TRUE;
  }
  ScsUfsHandle->UfsInfo->IsFuseDisabled = ScsUfsHandle->SocConfig.IsFuseDisabled;

  if ((ScsUfsHandle->Config->Enable || ScsUfsHandle->SocConfig.IsBootMedium) && !(ScsUfsHandle->SocConfig.IsFuseDisabled) &&
      (ScsUfsHandle->SocConfig.NumOfLanes != 0) && ScsUfsHandle->Config->UfsDeviceConnected) {
    if (ScsUfsHandle->SocConfig.IsMPhyUlpUpdateRequired) {
      Status = ScsUfsFwTrigger (ScsUfsHandle);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ScsUfsFwTrigger: Error, Status = %r\n", Status));
      }
      Status = ScsUfsMPhyEnable (ScsUfsHandle);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ScsUfsMPhyEnable: Error, Status = %r\n", Status));
      }
    }
    ScsUfsEnable (ScsUfsHandle);
  } else {
    if (ScsUfsHandle->SocConfig.IsMPhyUlpUpdateRequired) {
      Status = ScsUfsMPhyUlp (ScsUfsHandle);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ScsUfsMPhyUlp: Error, Status = %r\n", Status));
      }
      Status = ScsUfsFwTrigger (ScsUfsHandle);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ScsUfsFwTrigger: Error, Status = %r\n", Status));
      }
      Status = ScsUfsMPhyEnable (ScsUfsHandle);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ScsUfsMPhyEnable: Error, Status = %r\n", Status));
      }
      ScsUfsProgramAhit (ScsUfsHandle, V_UFS_MEM_AHIT_VALUE);
    }
    ScsUfsDisable (ScsUfsHandle);
  }
  DEBUG ((DEBUG_INFO, "UFS init finished\n"));
}

