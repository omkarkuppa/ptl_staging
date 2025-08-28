/** @file
  Initializes THC Devices in PEI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiThcInitLib.h>
#include <Register/ThcRegs.h>
#include <Ppi/SiPolicy.h>
#include <IndustryStandard/Pci30.h>

typedef union {
  struct {
    UINT32 D0i2RxDmaPolicy     : 1;  // [0][RW] D0i2 RxDMA Policy
    UINT32 Rsvd0               : 7;  // [1:7][RO] Reserved
    /**
      [8:11][RW] D0i2 Entry Timer
      This timer defines how long the THC must be idle before power gating due to D0i2.
      This timer is NOT used for D3 or SW PG.
       4'b0000: 0us
       4'b0001: 100ns
       4'b0010: 1us
       4'b0011: 10us
       4'b0100: 100us
       4'b0101: 1ms
       4'b0110: 10ms
       4'b0111: 100ms
       4'b1000: 1s
       4'b1001: 10s
       Others: reserved
    **/
    UINT32 D0i2EntryTimer      : 4;
    UINT32 D0i2VnnRemovalTimer : 4;  // [12:15][RW] D0i2 Vnn Removal Timer
    UINT32 ThcLtrEn            : 1;  // [16][RW] LTR Enable
    UINT32 ThcTimeStampSrc     : 1;  // [17][RW] Timestamp source
    UINT32 ThcTsD0i2Mode       : 1;  // [18][RW] Timestamp Timer mode
    UINT32 Rsvd1               : 5;  // [19:23][RO] Reserved
    /**
      [24:30][RW] Ten Microsecond Counter Value
      BIOS programs this register based on the SoC pgcb source clock frequency for THC generate the 10 microsecond tick internally.
      For example, if the PGCB clock frequency is 2.5MHz, each clock cycle is 400ns,
      BIOS shall program this field to 25 to generate the 10us tick.
      This timer is not running in D0i2.
      This bit can be locked when the THC_BIOS_LOCK_EN bit is 1
    **/
    UINT32 ThcMicroSecCnt      : 7;
    UINT32 Rsvd2               : 1;  // [31][RO] Reserved
  } Fields;
  UINT32 Data32;
} THC_SB_PM_CTRL;
typedef union {
  struct {
    UINT32 SscClkGateEn        : 1;  // [0][RW] SSC Clock Gate Enable
    UINT32 Rsvd0               : 31;  // [31:1][RO] Reserved
  } Fields;
  UINT32 Data32;
} THC_SB_SSC_CLK_CFG;

/**
  Configures THC PM Ctrl D0i2 Entry Timer

  @param[in] ThcPcrAccess     THC PCR Register Access
  @param[in] D0i2EntryTimer   D0i2 Entry Timer value
**/
VOID
STATIC
ThcPmCtrlD0i2EntryTimer (
  IN REGISTER_ACCESS     *ThcPcrAccess,
  IN UINT32              D0i2EntryTimer
  )
{
  THC_SB_PM_CTRL ThcSbPmCtrl;

  if (ThcPcrAccess == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - ThcPcrAccess is null\n", __FUNCTION__));
    return;
  }

  ThcSbPmCtrl.Data32 = ThcPcrAccess->Read32 (ThcPcrAccess, R_THC_PCR_SB_PM_CTRL);
  ThcSbPmCtrl.Fields.D0i2EntryTimer = D0i2EntryTimer;
  ThcPcrAccess->Write32 (ThcPcrAccess, R_THC_PCR_SB_PM_CTRL, ThcSbPmCtrl.Data32);
}

/**
  Enables LTR feature for THC

  @param[in] ThcPcrAccess     THC PCR Register Access
**/
VOID
STATIC
ThcPmCtrlLtrEnable (
  IN REGISTER_ACCESS     *ThcPcrAccess
  )
{
  THC_SB_PM_CTRL ThcSbPmCtrl;

  if (ThcPcrAccess == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - ThcPcrAccess is null\n", __FUNCTION__));
    return;
  }

  //
  // BIOS shall set the DIS_QOS_DMD_RSP bit to 1 before setting THC_LTR_EN to 1, during THC initialization (host reset exit).
  //
  DEBUG ((DEBUG_INFO, "Disabling THC waiting for QOS_DMD response from SoC\n"));
  ThcPcrAccess->AndThenOr32 (ThcPcrAccess, R_THC_PCR_SLEEP_STATES_CTL, (UINT32) ~0, B_THC_PCR_SLEEP_STATES_CTL_DIS_QOS_DMD_RSP);
  DEBUG ((DEBUG_INFO, "THC_SLEEP_STATES_CTL = %08X\n", ThcPcrAccess->Read32 (ThcPcrAccess, R_THC_PCR_SLEEP_STATES_CTL) ));
  ThcSbPmCtrl.Data32 = ThcPcrAccess->Read32 (ThcPcrAccess, R_THC_PCR_SB_PM_CTRL);
  ThcSbPmCtrl.Fields.ThcLtrEn = 1;
  ThcPcrAccess->Write32 (ThcPcrAccess, R_THC_PCR_SB_PM_CTRL, ThcSbPmCtrl.Data32);
}

/**
  Sets Ten Microsecond Counter Value

  @param[in] ThcPcrAccess             THC PCR Register Access
  @param[in] PgcbSourceClockFrequency Unit required to calculate Ten Microsecond Counter Value
  @param[in] TimestampTimerMode       Timestamp timer behavior in D0i2
**/
VOID
STATIC
ThcSetMicroSecCntAndTimeStampMode (
  IN REGISTER_ACCESS     *ThcPcrAccess,
  IN UINT32              PgcbSourceClockFrequency,
  IN UINT8               TimestampTimerMode
  )
{
  THC_SB_PM_CTRL ThcSbPmCtrl;

  if (ThcPcrAccess == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - ThcPcrAccess is null\n", __FUNCTION__));
    return;
  }

  ThcSbPmCtrl.Data32 = ThcPcrAccess->Read32 (ThcPcrAccess, R_THC_PCR_SB_PM_CTRL);
  //
  // Clock Cycle in nanoseconds = 10^9/x
  // To calculate 10 micro seconds Divide 10 000 ns by the previous result
  // Final 10 us formula = 0.00001 * PgcbSourceClockFrequency [Hz]
  //
  ThcSbPmCtrl.Fields.ThcMicroSecCnt = 0x7f & (UINT8) DivU64x64Remainder (PgcbSourceClockFrequency, 100000, NULL);
  DEBUG ((DEBUG_INFO, "ThcSbPmCtrl.Fields.ThcMicroSecCnt %d\n", ThcSbPmCtrl.Fields.ThcMicroSecCnt));
  ThcSbPmCtrl.Fields.ThcTsD0i2Mode = TimestampTimerMode;
  ThcPcrAccess->Write32 (ThcPcrAccess, R_THC_PCR_SB_PM_CTRL, ThcSbPmCtrl.Data32);
}

/**
  This function does a read and write to 0x8.

  @param[in] ThcPcrAccess             THC PCR Register Access

**/
VOID
STATIC
ThcClkCfgReadAndWrite (
  IN REGISTER_ACCESS     *ThcPcrAccess
  )
{
  THC_SB_SSC_CLK_CFG ThcSbSscClkCfg;

  if (ThcPcrAccess == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - ThcClkCfgReadAndWrite ThcPcrAccess is null\n", __FUNCTION__));
    return;
  }

  ThcSbSscClkCfg.Data32 = ThcPcrAccess->Read32 (ThcPcrAccess, R_THC_SB_SSC_CLK_CFG);
  DEBUG ((DEBUG_INFO, "ThcSbSscClkCfg.Data32: 0x%x\n", ThcSbSscClkCfg.Data32));
  ThcPcrAccess->Write32 (ThcPcrAccess, R_THC_SB_SSC_CLK_CFG, ThcSbSscClkCfg.Data32);
}


/**
  Configures THC-SPI Interface Clock

  @param[in] ThcPcrAccess     THC PCR Register Access
  @param[in] Clock            Clock DFX or Functional
**/
VOID
STATIC
ThcSetPortClock (
  IN REGISTER_ACCESS     *ThcPcrAccess,
  IN THC_PORT_CLOCK      Clock
  )
{
  UINT32 DataOr32 = 0x0;

  if (ThcPcrAccess == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - ThcPcrAccess is null\n", __FUNCTION__));
    return;
  }

  if (Clock == ThcClockUseDfx) {
    DataOr32 |= B_THC_PCR_SB_SPI_CTRL_SPI_DFX_CLK_EN;
  }
  ThcPcrAccess->AndThenOr32 (ThcPcrAccess, R_THC_PCR_SB_SPI_CTRL, (UINT32) ~(B_THC_PCR_SB_SPI_CTRL_SPI_DFX_CLK_EN), DataOr32);
}

/**
  Configures THC Port

  @param[in] ThcPciCfgBaseAddr     Thc Pci Config Base Adress
  @param[in] ThcIndex              THC Index
  @param[in] ThcInterruptPin       Intrrupt Pin
  @param[in] ThcMmio               Temporary MMIO
**/
VOID
STATIC
ThcConfigurePort (
  IN UINT64         ThcPciCfgBaseAddr,
  IN UINT8          ThcIndex,
  IN UINT8          ThcInterruptPin,
  IN UINT64         ThcMmio
  )
{
  DEBUG ((DEBUG_INFO, "ThcConfigurePort %d Entry \n", ThcIndex));

  if (PciSegmentRead16 (ThcPciCfgBaseAddr + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "THC%d: PCI Device not found!\n", ThcIndex));
    return;
  }

  //
  // Configure Interrupts
  //
  PciSegmentWrite8 (ThcPciCfgBaseAddr + PCI_INT_PIN_OFFSET, ThcInterruptPin);

  //
  // Set the THC PCI Config Power Control Enable register D3HE bit
  //
  PciSegmentOr32 (ThcPciCfgBaseAddr + R_THC_CFG_PCE, B_THC_CFG_PCE_D3HE);

  //
  // Enable Memory decoding
  //
  PciSegmentWrite32 (ThcPciCfgBaseAddr + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) ThcMmio);
  if (PciSegmentRead32 (ThcPciCfgBaseAddr + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (ThcPciCfgBaseAddr + PCI_BASE_ADDRESSREG_OFFSET + 0x4, (UINT32) RShiftU64 (ThcMmio, 32));
  }
  PciSegmentOr16 (ThcPciCfgBaseAddr + PCI_COMMAND_OFFSET, (UINT16) EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Set Port Supported
  //
  MmioOr32 ((UINTN) ThcMmio + R_THC_MEM_PRT_CONTROL, B_THC_MEM_PRT_CONTROL_PORT_SUPPORTED);

  //
  // Lock Port Control Register
  //
  MmioOr32 ((UINTN) ThcMmio + R_THC_MEM_PRT_CONTROL, B_THC_MEM_PRT_CONTROL_THC_BIOS_LOCK_EN);

  //
  // Disable Memory decoding and put THC in D3
  //
  PciSegmentAnd16 (ThcPciCfgBaseAddr + PCI_COMMAND_OFFSET, (UINT16) (~EFI_PCI_COMMAND_MEMORY_SPACE));
  PciSegmentOr32 (ThcPciCfgBaseAddr + R_THC_CFG_PMD_PMCSRBSE_PMCSR, V_THC_CFG_PMD_PMCSRBSE_PMCSR_PWRST_D3);
  PciSegmentWrite32 (ThcPciCfgBaseAddr + PCI_BASE_ADDRESSREG_OFFSET, 0);
  if (PciSegmentRead32 (ThcPciCfgBaseAddr + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (ThcPciCfgBaseAddr + PCI_BASE_ADDRESSREG_OFFSET + 4, 0);
  }

  DEBUG ((DEBUG_INFO, "ThcConfigurePort Exit \n"));
}

/**
  Validates THC Port Configuration

  @param[in] ThcController     Pointer to THC Controller structure
  @param[in] ThcPort            Port Policy

  @retval TRUE                 Validation Passed
          FALSE                Validation Failed
**/
BOOLEAN
STATIC
ThcPolicyValidate (
  IN THC_CONTROLLER  *ThcController,
  IN THC_PORT        *ThcPort
  )
{
  if ((ThcController == NULL) || (ThcPort == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a - ThcController or ThcPort are NULL\n", __FUNCTION__));
    return FALSE;
  }

  if ((ThcController->ThcCtrlIndex == 0) && (ThcPort->Assignment >= ThcAssignmentThc1)) {
    goto THC_ERROR;
  }
  if ((ThcController->ThcCtrlIndex == 1) &&
      (ThcPort->Assignment == ThcAssignmentThc0 ||
      ThcPort->Assignment > ThcAssignmentThc1)) {
    goto THC_ERROR;
  }
  return TRUE;

THC_ERROR:
  DEBUG ((DEBUG_ERROR, "Wrong Thc Assignment settings in policy!\n"));
  DEBUG ((DEBUG_WARN, "ThcPort->Assignment: %d \n", ThcPort->Assignment));
  DEBUG ((DEBUG_WARN, "ThcController->ThcCtrlIndex: %d \n", ThcController->ThcCtrlIndex));
  ASSERT (FALSE);
  return FALSE;
}

/**
  Initialize the Intel Touch Host Controller

  @param[in] ThcHandle        The THC Handle instance
**/
VOID
ThcInit (
  IN THC_HANDLE  *ThcHandle
  )
{
  UINT8 ThcIndex = 0;

  DEBUG ((DEBUG_VERBOSE, "ThcInit() Start\n"));

  if (ThcHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a - ThcHandle cannot be NULL\n", __FUNCTION__));
    return;
  }

  //
  // Validate THC policy
  //
  for (ThcIndex = 0; ThcIndex < ThcHandle->ThcCount; ThcIndex++) {
    if (!ThcPolicyValidate (&ThcHandle->Controller[ThcIndex], &ThcHandle->ThcPortConfig[ThcIndex])) {
      if (ThcHandle->Callback->ThcControllerDisable != NULL) {
        ThcHandle->Callback->ThcControllerDisable (&ThcHandle->Controller[ThcIndex]);
      }
      DEBUG ((DEBUG_VERBOSE, "ThcInit() End\n"));
      return;
    }
  }

  for (ThcIndex = 0; ThcIndex < ThcHandle->ThcCount; ThcIndex++) {
    ThcSetPortClock (ThcHandle->Controller[ThcIndex].ThcPcrAccess, ThcHandle->ThcPortConfig[ThcIndex].Clock);
    //
    // Configuring the THC SB PM CTRL register's
    // Note: Power Management requires configuration regardless of THC disabled/enabled state
    //       Without this setting THC might block S0iX entery even when Function Disabled
    //
    ThcPmCtrlD0i2EntryTimer (ThcHandle->Controller[ThcIndex].ThcPcrAccess, ThcHandle->PrivateConfig.D0i2EntryTimer);

    if ((ThcHandle->PrivateConfig.ThcResourceDisabled == TRUE) &&
        (ThcHandle->Callback->ThcResourceControlDisable != NULL)) {
      ThcHandle->Callback->ThcResourceControlDisable (ThcHandle->Controller[ThcIndex].ThcPcrAccess);
    }

    //
    // Enable THC
    //
    if (ThcHandle->ThcPortConfig[ThcIndex].Assignment > ThcAssignmentNone) {
      //
      // Hid Over Spi/I2c Config
      //
      if ((ThcHandle->ThcPortConfig[ThcIndex].Mode == HidOverSpi) &&
          (ThcHandle->Callback->ThcHidOverSpiEnable != NULL)) {
        ThcHandle->Callback->ThcHidOverSpiEnable (ThcHandle, ThcIndex);
      } else if ((ThcHandle->ThcPortConfig[ThcIndex].Mode == HidOverI2c) &&
                (ThcHandle->Callback->ThcHidOverI2cEnable != NULL)) {
        ThcHandle->Callback->ThcHidOverI2cEnable (ThcHandle, ThcIndex);
      } else if ((ThcHandle->ThcPortConfig[ThcIndex].Mode == ThcModeNone) &&
                (ThcHandle->Callback->ThcHidOverI2cEnable != NULL)) {
        ThcHandle->Callback->ThcHidOverI2cEnable (ThcHandle, ThcIndex);
      }

      //
      // Configuring the THC SB PM CTRL registers
      // - Ltr Enable
      //
      ThcPmCtrlLtrEnable (ThcHandle->Controller[ThcIndex].ThcPcrAccess);

      if (ThcHandle->PrivateConfig.FrameCoalescingSupported) {
        ThcSetMicroSecCntAndTimeStampMode (
          ThcHandle->Controller[ThcIndex].ThcPcrAccess,
          ThcHandle->PrivateConfig.PgcbSourceClockFrequency,
          ThcHandle->ThcPortConfig[ThcIndex].TimestampTimerMode
          );
      }

      ThcClkCfgReadAndWrite (ThcHandle->Controller[ThcIndex].ThcPcrAccess);

      ThcConfigurePort (
        ThcHandle->Controller[ThcIndex].PciCfgBaseAddr,
        ThcHandle->Controller[ThcIndex].ThcCtrlIndex,
        ThcHandle->PrivateConfig.InterruptPin[ThcIndex],
        ThcHandle->Mmio
        );

      //
      // Configure Gpio
      //
      if ((ThcHandle->ThcPortConfig[ThcIndex].Mode == HidOverSpi) &&
          (ThcHandle->Callback->ThcGpioEnable != NULL)) {
        ThcHandle->Callback->ThcGpioEnable (ThcIndex, &ThcHandle->ThcPortConfig[ThcIndex]);
      } else if ((ThcHandle->ThcPortConfig[ThcIndex].Mode == HidOverI2c) &&
                (ThcHandle->Callback->ThcI2cGpioEnable != NULL)) {
        ThcHandle->Callback->ThcI2cGpioEnable (ThcIndex, &ThcHandle->ThcPortConfig[ThcIndex]);
      }
    } else {
      if (ThcHandle->Callback->ThcControllerDisable != NULL) {
        ThcHandle->Callback->ThcControllerDisable (&ThcHandle->Controller[ThcIndex]);
      }
    }
  }

  DEBUG ((DEBUG_VERBOSE, "ThcInit() End\n"));
}
