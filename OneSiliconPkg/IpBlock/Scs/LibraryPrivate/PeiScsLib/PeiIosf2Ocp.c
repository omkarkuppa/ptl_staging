/** @file
  Iosf2Ocp bridge helper functions.

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

#include <Register/ScsIosf2OcpBridgeRegs.h>
#include "PeiScsInitInternal.h"
#include <RegisterAccess.h>

/**
  Converts Iosf2Ocp controller index into PCI_CFG_CTRL register offset.

  @param[in] ControllerIndex  Index of the controller on the bridge.

  @return  UINT32  Offset of the PCI_CFG_CTRL register corresponding to Controller index
**/
STATIC
UINT32
Iosf2OcpGetPciCfgCtrlOffset (
  IN UINT8  ControllerIndex
  )
{
  return R_SCS_PCR_PCICFGCTR_BASE + (4 * ControllerIndex);
}

/**
  This function configures power and clock gating policy in Iosf2Ocp bridge
  based on value passed by the caller.

  @param[in] BridgeAccess              Bridge Register Access handle
  @param[in] PowerAndClockGatingValue  Value to be written to power and clock gating register
**/
VOID
Iosf2OcpConfigurePowerAndClockGating (
  IN REGISTER_ACCESS *BridgeAccess,
  IN UINT32          PowerAndClockGatingValue
  )
{
  BridgeAccess->Write32 (
    BridgeAccess,
    R_SCS_PCR_PMCTL,
    (BIT5| BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
    );

  //
  // Be careful not to overwrite the UFS enable and
  // UFS dual lane enable bits while updating power
  // and clock gating registers.
  //
  BridgeAccess->AndThenOr32 (
    BridgeAccess,
    R_SCS_PCR_GPPRVRW1,
    (UINT32) ~B_SCS_PCR_GPPRVRW1_PG_CG,
    PowerAndClockGatingValue & B_SCS_PCR_GPPRVRW1_PG_CG
    );
}

/**
  Enables Iosf2Ocp bridge to send snooped transactions on
  IOSF fabric.

  @param[in] BridgeAccess              Bridge Register Access handle
**/
VOID
Iosf2OcpEnableSnoopedTransactions (
  IN REGISTER_ACCESS *BridgeAccess
  )
{
  BridgeAccess->AndThenOr32 (
    BridgeAccess,
    R_SCS_PCR_IOSFCTL,
    (UINT32) ~0,
    B_SCS_PCR_IOSFCTL_NSNPDIS
    );
}

/**
  This function enables MPHY PLL to Iosf2Ocp bridge.

  @param[in] BridgeAccess              Bridge Register Access handle
**/
STATIC
VOID
Iosf2OcpEnablePll (
  IN REGISTER_ACCESS *BridgeAccess
  )
{
  //
  // Set GPPRVRW6 31b PLL En to 1
  //
  BridgeAccess->AndThenOr32 (
    BridgeAccess,
    R_SCS_PCR_GPPRVRW6,
    (UINT32) ~0,
    BIT31
    );
}

/**
  Disable OCP timeout on a IOSF2OCP bridge.

  @param[in] Iosf2OcpPort  Pointer to given controller described by IOSF2OCP_PORT structure
**/
VOID
Iosf2OcpDisableTimeout (
  IN IOSF2OCP_PORT  *Iosf2OcpPort
  )
{
  if (Iosf2OcpPort == NULL) {
    ASSERT (FALSE);
    return;
  }

  Iosf2OcpPort->BridgeAccess->AndThenOr32 (
    Iosf2OcpPort->BridgeAccess,
    R_SCS_PCR_I0_IA_AGENT_CONTROL,
    (UINT32) 0xFFF8FFFF,
    0x00000000
    );

  Iosf2OcpPort->BridgeAccess->AndThenOr32 (
    Iosf2OcpPort->BridgeAccess,
    R_SCS_PCR_SCC2MAIN_T0_TA_AGENT_CONTROL,
    (UINT32) 0xFFFFF8FF,
    0x00000000
    );
}

/**
  Configures interrupts in the Iosf2Ocp bridge.

  @param[in] Iosf2OcpPort  Pointer to given controller described by IOSF2OCP_PORT structure
  @param[in] InterruptPin  Interrupt pin number
  @param[in] Irq           IRQ number
**/
VOID
Iosf2OcpConfigureInterrupts (
  IN IOSF2OCP_PORT  *Iosf2OcpPort,
  IN UINT8          InterruptPin,
  IN UINT8          Irq
  )
{
  UINT32  Data32Or;
  UINT32  Data32And;

  if (Iosf2OcpPort == NULL) {
    ASSERT (FALSE);
    return;
  }

  Data32Or = (UINT32) ((InterruptPin << N_SCS_PCR_PCICFGCTR_IPIN1) |
                       (Irq << N_SCS_PCR_PCICFGCTR_PCI_IRQ));
  Data32And = ~(UINT32) (B_SCS_PCR_PCICFGCTR_PCI_IRQ | B_SCS_PCR_PCICFGCTR_ACPI_IRQ | B_SCS_PCR_PCICFGCTR_IPIN1);

  Iosf2OcpPort->BridgeAccess->AndThenOr32 (
    Iosf2OcpPort->BridgeAccess,
    Iosf2OcpGetPciCfgCtrlOffset (Iosf2OcpPort->ControllerIndex),
    Data32And,
    Data32Or
    );
}

/**
  This function disables BAR1 for a given controller on Iosf2Ocp brige.

  @param[in]  Iosf2OcpPort  Pointer to given controller described by IOSF2OCP_PORT structure
**/
VOID
Iosf2OcpDisableBar1 (
  IN IOSF2OCP_PORT  *Iosf2OcpPort
  )
{
  if (Iosf2OcpPort == NULL) {
    ASSERT (FALSE);
    return;
  }

  Iosf2OcpPort->BridgeAccess->AndThenOr32 (
    Iosf2OcpPort->BridgeAccess,
    Iosf2OcpGetPciCfgCtrlOffset (Iosf2OcpPort->ControllerIndex),
    ~0u,
    B_SCS_PCR_PCICFGCTR_BAR1DIS
    );
  //
  // Read back is needed to enforce the sideband and primary ordering.
  //
  Iosf2OcpPort->BridgeAccess->Read32 (
    Iosf2OcpPort->BridgeAccess,
    Iosf2OcpGetPciCfgCtrlOffset (Iosf2OcpPort->ControllerIndex)
    );
}

/**
  Disables access to PCI config space for a given controller on
  Iosf2Ocp bridge.

  @param[in] Iosf2OcpPort  Pointer to given controller described by IOSF2OCP_PORT structure
**/
STATIC
VOID
Iosf2OcpDisablePciCfgSpace (
  IN IOSF2OCP_PORT  *Iosf2OcpPort
  )
{
  if (Iosf2OcpPort == NULL) {
    ASSERT (FALSE);
    return;
  }

  Iosf2OcpPort->BridgeAccess->AndThenOr32 (
    Iosf2OcpPort->BridgeAccess,
    Iosf2OcpGetPciCfgCtrlOffset (Iosf2OcpPort->ControllerIndex),
    ~0u,
    B_SCS_PCR_PCICFGCTR_PCI_CFG_DIS
    );
  //
  // Read back is needed to enforce the sideband and primary ordering.
  //
  Iosf2OcpPort->BridgeAccess->Read32 (
    Iosf2OcpPort->BridgeAccess,
    Iosf2OcpGetPciCfgCtrlOffset (Iosf2OcpPort->ControllerIndex)
    );
}

/**
  This function enables UFS on Iosf2Ocp bridge.
  It supports up to 2 UFS HC on a single bridge.

  @param[in] UfsPort       Pointer to IOSF2OCP_PORT structure describing UFS controller
  @param[in] UsedLanesNum  Number of lanes used by UFS controller
**/
VOID
Iosf2OcpEnableUfs (
  IN IOSF2OCP_PORT  *UfsPort,
  IN UINT32         UsedLanesNum
  )
{
  UINT32  Data32Or;

  if ((UfsPort == NULL) || (UsedLanesNum == 0) || (UsedLanesNum > 2)) {
    ASSERT (FALSE);
    return;
  }

  Data32Or = 0;
  switch (UfsPort->ControllerIndex) {
    case 0:
      Data32Or |= B_SCS_PCR_GPPRVRW1_UFS0_ENABLE;
      if (UsedLanesNum == 2) {
        Data32Or |= B_SCS_PCR_GPPRVRW1_UFS0_DUAL_LANE;
      }
      break;
    case 1:
      Data32Or |= B_SCS_PCR_GPPRVRW1_UFS1_ENABLE;
      if (UsedLanesNum == 2) {
        Data32Or |= B_SCS_PCR_GPPRVRW1_UFS1_DUAL_LANE;
      }
      break;
    default:
      ASSERT (FALSE);
      return;
  }

  //
  // Hardware default for dual lane is '0' which indicates that a single lane should be used.
  //
  UfsPort->BridgeAccess->AndThenOr32 (
    UfsPort->BridgeAccess,
    R_SCS_PCR_GPPRVRW1,
    (UINT32) ~0,
    Data32Or
    );

  //
  // PLL enable is a setting global to Iosf2Ocp bridge instance.
  // Since this has to be enabled if at least one of the UFS controllers
  // is enabled there should be no harm in calling it multiple times.
  //
  Iosf2OcpEnablePll (UfsPort->BridgeAccess);
}

/**
  Unlocks fields in eMMC DLL registers.

  @param[in] EmmcPort  Pointer to IOSF2OCP_PORT structure describing eMMC controller
**/
VOID
Iosf2OcpUnlockEmmcDll (
  IN IOSF2OCP_PORT  *EmmcPort
  )
{
  if (EmmcPort == NULL) {
    ASSERT (FALSE);
    return;
  }

  EmmcPort->BridgeAccess->AndThenOr32 (
    EmmcPort->BridgeAccess,
    R_SCS_PCR_GPPRVRW6,
    (UINT32) ~0,
    BIT30
    );
}

/**
  Disables eMMC controller on Iosf2Ocp bridge.

  @param[in] EmmcPort  Structure describing eMMC controller location on Iosf2Ocp bridge
**/
VOID
Iosf2OcpDisableEmmc (
  IN IOSF2OCP_PORT  *EmmcPort
  )
{
  if (EmmcPort == NULL) {
    ASSERT (FALSE);
    return;
  }
  Iosf2OcpDisablePciCfgSpace (EmmcPort);
  EmmcPort->BridgeAccess->AndThenOr32 (
    EmmcPort->BridgeAccess,
    R_SCS_PCR_GPPRVRW2,
    (UINT32) ~0,
    BIT1
    );
}

/**
  Configures 1p8 voltage related registers in Iosf2Ocp bridge.

  @param[in] SdCardPort             Pointer to IOSf2OCP_PORT structure describing SdCard controller
  @param[in] PowerEnableActiveHigh  Flag to indicate if POWER_ENABLE# should be active high
**/
VOID
Iosf2OcpSdCard1p8Enable (
  IN IOSF2OCP_PORT  *SdCardPort,
  IN BOOLEAN        PowerEnableActiveHigh
  )
{
  if (SdCardPort == NULL) {
    ASSERT (FALSE);
    return;
  }
  //
  // Set Rcomp delay from 1p8mode to 10ms
  // Set offset 614h bit[7:0] = 0x7F
  //
  SdCardPort->BridgeAccess->AndThenOr32 (
    SdCardPort->BridgeAccess,
    R_SCS_PCR_GPPRVRW6,
    (UINT32) ~B_SCS_PCR_GPPRVRW6_1P8_SEL_DELAY,
    V_SCS_PCR_GPPRVRW6_1P8_SEL_DELAY
    );

  if (PowerEnableActiveHigh) {
    //
    // Set VDD1_Invert to 1 to make SD Card VDD1_POWER_ENABLE active HIGH.
    // Default is active LOW.
    // offset 604h [16] = 1b
    //
    SdCardPort->BridgeAccess->AndThenOr32 (
      SdCardPort->BridgeAccess,
      R_SCS_PCR_GPPRVRW2,
      (UINT32) ~0,
      B_SCS_PCR_GPPRVRW2_VDD1_INVERT
      );
  }
}

/**
  Disables SdCard controller on Iosf2Ocp bridge.

  @param[in] SdCardPort  Structure describing SdCard location on Iosf2Ocp bridge
**/
VOID
Iosf2OcpDisableSdCard (
  IN IOSF2OCP_PORT  *SdCardPort
  )
{
  if (SdCardPort == NULL) {
    ASSERT (FALSE);
    return;
  }
  Iosf2OcpDisablePciCfgSpace (SdCardPort);
  SdCardPort->BridgeAccess->AndThenOr32 (
    SdCardPort->BridgeAccess,
    R_SCS_PCR_GPPRVRW2,
    (UINT32) ~0,
    BIT3
    );
}

/**
  Disables UFS controller on Iosf2Ocp bridge.

  @param[in] UfsPort  Structure describing UFS controller location on Iosf2Ocp bridge.
**/
VOID
Iosf2OcpDisableUfs (
  IN IOSF2OCP_PORT  *UfsPort
  )
{
  if (UfsPort == NULL) {
    ASSERT (FALSE);
    return;
  }

  switch (UfsPort->ControllerIndex) {
    case 0:
      Iosf2OcpDisablePciCfgSpace (UfsPort);
      UfsPort->BridgeAccess->AndThenOr32 (
        UfsPort->BridgeAccess,
        R_SCS_PCR_GPPRVRW2,
        (UINT32) ~0,
        BIT0
        );
      break;
    case 1:
      Iosf2OcpDisablePciCfgSpace (UfsPort);
      UfsPort->BridgeAccess->AndThenOr32 (
        UfsPort->BridgeAccess,
        R_SCS_PCR_GPPRVRW2,
        (UINT32) ~0,
        BIT1
        );
      break;
    default:
      ASSERT (FALSE);
  }
}

/**
  Enables UFS inline encryption feature for UFS controller.

  @param[in] UfsPort  Pointer to IOSF2OCP_PORT structure describing UFS controller
**/
VOID
Iosf2OcpEnableUfsInlineEncryption (
  IN IOSF2OCP_PORT *UfsPort
  )
{
  UINT32 Data32Or;

  switch (UfsPort->ControllerIndex) {
    case 0:
      Data32Or = B_SCS_PCR_GPPRVRW1_UFS0_IES;
      break;
    case 1:
      Data32Or = B_SCS_PCR_GPPRVRW1_UFS1_IES;
      break;
    default:
      ASSERT (FALSE);
      return;
  }

  UfsPort->BridgeAccess->AndThenOr32 (
    UfsPort->BridgeAccess,
    R_SCS_PCR_GPPRVRW1,
    (UINT32) ~0,
    Data32Or
    );
}