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

#include <IndustryStandard/SmBus.h>
#include <Library/DebugLib.h>
#include <Library/SmbusLib.h>
#include <Library/PlatformVoltageControlLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciSegmentLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Register/PchRegs.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PcdLib.h>

///
/// uP1816P DAC
///
#define VCC1P8_CPU_SECONDARY_ADDRESS   0x2A
#define VCC1P8_DDR_SECONDARY_ADDRESS   0x2A
#define VCCDD2_SECONDARY_ADDRESS       0x28
#define VNNAON_SECONDARY_ADDRESS       0x26
#define VCCIO_SECONDARY_ADDRESS        0x2A
#define V0P82_PCH_SECONDARY_ADDRESS    0x24
#define V3P3_PCH_SECONDARY_ADDRESS     0x24
#define V1P8_PCH_SECONDARY_ADDRESS     0x20

///
/// Richtek 3638AE VR
///
#define RICHTEK_VR_SECONDARY_ADDRESS   0x40
#define RICHTEK_VFIX_LB_CORE_MASK      0x00FF
#define RICHTEK_VFIX_HB_CORE_MASK      0x0100
#define VCCIA_VFIX_LB_REG              0x94
#define VCCIA_VFIX_HB_REG              0x95
#define VCCGT_VFIX_LB_REG              0xAB
#define VCCGT_VFIX_HB_REG              0xAC
#define VCCSA_VFIX_LB_REG              0xBB
#define VCCSA_VFIX_HB_REG              0xBC
#define VFIX_MODE_REG                  0xAA

#define PCI_DEVICE_NUMBER_PCH_SMBUS    31
#define PCI_FUNCTION_NUMBER_PCH_SMBUS  4
#define R_SMBUS_CFG_64                 0x64
#define R_SMBUS_CFG_BASE               0x20
#define B_SMBUS_CFG_BASE_BAR           0x0000FFE0
#define R_SMBUS_CFG_HOSTC              0x40
#define B_SMBUS_CFG_HOSTC_HST_EN       BIT0
#define B_SMBUS_CFG_HOSTC_SMI_EN       BIT1
#define B_SMBUS_CFG_HOSTC_I2C_EN       BIT2
#define B_SMBUS_CFG_HOSTC_SSRESET      BIT3

/**
  Programs the overrides voltage which are controlled by uP1816P
  chip using SVID/PMBUS interface.

  @param[in] SlaveAddress    Device slave address
  @param[in] DeviceVref      Device command register
  @param[in] DefaultVoltage  Voltage target in mV
  @param[in] TargetVoltage   Voltage target in mV

  @retval EFI_SUCCESS     Successful to override voltage.
**/
EFI_STATUS
SetP1816Voltage (
  IN  UINT8                 SlaveAddress,
  IN  UINT8                 DeviceVref,
  IN  UINT16                DefaultVoltage,
  IN  UINT16                TargetVoltage
  )
{
  EFI_STATUS                  Status;
  EFI_SMBUS_DEVICE_ADDRESS    SlaveAddr;
  UINT8                       Data;
  UINT8                       Prefix;
  UINT8                       CurrentData;

  //
  // Target address
  //
  SlaveAddr.SmbusDeviceAddress = (SlaveAddress >> 1);

  //
  // Data = 0 is default voltage
  //
  Prefix = 0;
  if (TargetVoltage >= DefaultVoltage) {
    Data = (UINT8) (((TargetVoltage - DefaultVoltage) / 10) | (Prefix << 7));
  } else {
    Prefix = 1;
    Data = (UINT8) (((DefaultVoltage - TargetVoltage) / 10) | (Prefix << 7));
  }

  CurrentData = SmBusReadDataByte (
                  SMBUS_LIB_ADDRESS (SlaveAddr.SmbusDeviceAddress, DeviceVref, 0, FALSE),
                  &Status
                  );

  DEBUG ((DEBUG_INFO, "SMBUS transaction request : Addr=0x%08X Command=0x%08X CurrentData=0x%08X Data=0x%08X\n",
    SlaveAddr.SmbusDeviceAddress, DeviceVref, CurrentData, Data));

  SmBusWriteDataByte (
    SMBUS_LIB_ADDRESS (SlaveAddr.SmbusDeviceAddress, DeviceVref, 0, FALSE),
    Data,
    &Status
    );

  DEBUG ((DEBUG_INFO, "SMBUS transaction result : Status=0x%08X\n", Status));

  return Status;
}

/**
  Programs the overrides for VCC Ia/Gt/SA Fixed VID mode of Richtek RT3638AE VR (External VR).

  @param[in] VidModeEn      Enabled/ Disable VCC VID Mode

  @retval    EFI_SUCCESS   The function completes successfully
  @retval    EFI_STATUS    Fail status return from SmBusWriteDataWord ()
**/
EFI_STATUS
SetRichtekFixedVidMode (
  IN  UINT8                 VidModeEn
  )
{
  EFI_STATUS                  Status;
  EFI_SMBUS_DEVICE_ADDRESS    SecondaryAddr;
  UINT8                       CurrentData;

  //
  // Secondary address
  //
  SecondaryAddr.SmbusDeviceAddress = (RICHTEK_VR_SECONDARY_ADDRESS >> 1);
  DEBUG ((DEBUG_INFO, "VidModeEn = 0x%X\n", VidModeEn));

  //
  // Enable / Disable Fixed VID mode of IA/GT/SA Rail
  //
  CurrentData = SmBusReadDataByte (
                  SMBUS_LIB_ADDRESS (SecondaryAddr.SmbusDeviceAddress, VFIX_MODE_REG, 0, FALSE),
                  &Status
                  );

  DEBUG ((DEBUG_INFO, "SMBUS transaction Reading : Addr=0x%08X VID_Reg=0x%08X CurrentData=0x%08X\n",
    SecondaryAddr.SmbusDeviceAddress, VFIX_MODE_REG, CurrentData));

  SmBusWriteDataByte (
    SMBUS_LIB_ADDRESS (SecondaryAddr.SmbusDeviceAddress, VFIX_MODE_REG, 0, FALSE),
    VidModeEn,
    &Status
    );
  DEBUG ((DEBUG_INFO, "SMBUS setup Fixed VID mode : Status=0x%08X\n", Status));

  return Status;
}

/**
  Programs the overrides for VCC IA/GT/SA voltage which are controlled by Richtek RT3638AE VR (External VR).

  @param[in] VidModeEn      Enabled/ Disable VCC VID Mode
  @param[in] VfixLbReg      LB Register for VID voltage
  @param[in] VfixHbReg      HB Register for VID voltage
  @param[in] Voltage        Voltage target in mV

  @retval    EFI_SUCCESS   The function completes successfully
  @retval    EFI_STATUS    Fail status return from SmBusWriteDataWord ()
**/
EFI_STATUS
SetRichtekVoltage (
  IN  UINT8                 VidModeEn,
  IN  UINT8                 VfixLbReg,
  IN  UINT8                 VfixHbReg,
  IN  UINT16                Voltage
  )
{
  EFI_STATUS                  Status;
  EFI_SMBUS_DEVICE_ADDRESS    SecondaryAddr;
  UINT16                      InputVid;
  UINT8                       FixVidHb;
  UINT8                       FixVidLb;

  Status = EFI_SUCCESS;
  //
  // Secondary address
  //
  SecondaryAddr.SmbusDeviceAddress = (RICHTEK_VR_SECONDARY_ADDRESS >> 1);
  DEBUG ((DEBUG_INFO, "VCC overrides: VidModeEn = 0x%X, Voltage = %X\n", VidModeEn, Voltage));
  DEBUG ((DEBUG_INFO, "SecondaryAddr.SmbusDeviceAddress = %X\n", SecondaryAddr.SmbusDeviceAddress));

  if (VidModeEn) {
    //
    // Calculate request VID (voltage)
    //
    InputVid = (UINT16) ((Voltage - 245) / 5);
    FixVidHb = (InputVid & RICHTEK_VFIX_HB_CORE_MASK)? 0x01 : 0x00;
    FixVidLb = (UINT8) (InputVid & RICHTEK_VFIX_LB_CORE_MASK);

    DEBUG ((DEBUG_INFO, "SMBUS transaction request: Addr=0x%08X Voltage=0x%08X FVid_LB = 0x%08X FVid_HB = 0x%08X\n",
      SecondaryAddr.SmbusDeviceAddress, InputVid, FixVidLb, FixVidHb));

    SmBusWriteDataByte (
      SMBUS_LIB_ADDRESS (SecondaryAddr.SmbusDeviceAddress, VfixLbReg, 0, FALSE),
      FixVidLb,
      &Status
      );
    DEBUG ((DEBUG_INFO, "SMBUS transaction VFIX_LB Status=0x%08X\n", Status));

    SmBusWriteDataByte (
      SMBUS_LIB_ADDRESS (SecondaryAddr.SmbusDeviceAddress, VfixHbReg, 0, FALSE),
      FixVidHb,
      &Status
      );
    DEBUG ((DEBUG_INFO, "SMBUS transaction VFIX_HB Status=0x%08X\n", Status));
  }
  return Status;
}

/**
  Check and apply the override knobs for external voltage rails based on board design

  @param[in] OcSetup       Setup variables under OC Setup
**/
VOID
ProgramVoltageOverrides (
  IN  OC_SETUP             *OcSetup
  )
{
  EFI_STATUS Status;
  UINT8      VidMode;
  BOOLEAN    IsFixedVidUpdated;

  Status = EFI_SUCCESS;
  VidMode = 0;
  IsFixedVidUpdated = FALSE;

  //
  // Check for CPU Vcc1p8 voltage override
  //
  if (OcSetup->Vcc1p8CpuOverrideEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming CPU Vcc1p8 Override = %d\n", OcSetup->Vcc1p8CpuOverrideVoltage));
    Status = SetP1816Voltage (VCC1P8_CPU_SECONDARY_ADDRESS, 0x02, 1800, OcSetup->Vcc1p8CpuOverrideVoltage);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "CPU Vcc1p8 programming failed. Status = %r\n", Status));
    }
  }

  //
  // Check for DDR Vcc1p8 voltage override
  //
  if (OcSetup->Vcc1p8DdrOverrideEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming DDR Vcc1p8 Override = %d\n", OcSetup->Vcc1p8DdrOverrideVoltage));
    Status = SetP1816Voltage (VCC1P8_DDR_SECONDARY_ADDRESS, 0x01, 1800, OcSetup->Vcc1p8DdrOverrideVoltage);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "DDR Vcc1p8 programming failed. Status = %r\n", Status));
    }
  }

  //
  // Check for VccDD2 voltage override
  //
  if (OcSetup->VccDd2OverrideEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming VccDD2 Override = %d\n", OcSetup->VccDd2OverrideVoltage));
    Status = SetP1816Voltage (VCCDD2_SECONDARY_ADDRESS, 0x01, 1100, OcSetup->VccDd2OverrideVoltage);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "VccDD2 programming failed. Status = %r\n", Status));
    }
  }

  //
  // Check for VNNAON voltage override
  //
  if (OcSetup->VnnaonOverrideEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming VNNAON Override = %d\n", OcSetup->VnnaonOverrideVoltage));
    Status = SetP1816Voltage (VNNAON_SECONDARY_ADDRESS, 0x01, 770, OcSetup->VnnaonOverrideVoltage);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Vnnaon programming failed. Status = %r\n", Status));
    }
  }

  //
  // Check for VCCIO voltage override
  //
  if (OcSetup->VccIoOverrideEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming VCCIO Override = %d\n", OcSetup->VccIoOverrideVoltage));
    Status = SetP1816Voltage (VCCIO_SECONDARY_ADDRESS, 0x03, 1250, OcSetup->VccIoOverrideVoltage);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "VCCIO programming failed. Status = %r\n", Status));
    }
  }

  //
  // Check for PCH V0P82 voltage override
  //
  if (OcSetup->V0p82OverrideEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming PCH V0P82 Override = %d\n", OcSetup->V0p82OverrideVoltage));
    Status = SetP1816Voltage (V0P82_PCH_SECONDARY_ADDRESS, 0x01, 820, OcSetup->V0p82OverrideVoltage);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "PCH V0P82 programming failed. Status = %r\n", Status));
    }
  }

  //
  // Check for PCH V3P3 voltage override
  //
  if (OcSetup->V3p3OverrideEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming PCH V3P3 Override = %d\n", OcSetup->V3p3OverrideVoltage));
    Status = SetP1816Voltage (V3P3_PCH_SECONDARY_ADDRESS, 0x02, 3300, OcSetup->V3p3OverrideVoltage);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "PCH V3P3 programming failed. Status = %r\n", Status));
    }
  }

  //
  // Check for PCH V1P8 voltage override
  //
  if (OcSetup->V1p8OverrideEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming PCH V1P8 Override = %d\n", OcSetup->V1p8OverrideVoltage));
    Status = SetP1816Voltage (V1P8_PCH_SECONDARY_ADDRESS, 0x01, 1800, OcSetup->V1p8OverrideVoltage);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "PCH V1P8 programming failed. Status = %r\n", Status));
    }
  }

  //
  // Check for RichTek VccIA voltage override
  //
  if (OcSetup->RichtekVccIaEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming RichTek VccIA Override = %d\n", OcSetup->RichtekVccIaVoltage));
    Status = SetRichtekVoltage (OcSetup->RichtekVccIaFixedVidMode, VCCIA_VFIX_LB_REG, VCCIA_VFIX_HB_REG, OcSetup->RichtekVccIaVoltage);

    IsFixedVidUpdated = TRUE;
    VidMode |= (OcSetup->RichtekVccIaFixedVidMode << 2);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "RichTek VccIA programming failed. Status = %r\n", Status));
    }
  }


  //
  // Check for RichTek VccGT voltage override
  //
  if (OcSetup->RichtekVccGtEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming RichTek VccGT Override = %d\n", OcSetup->RichtekVccGtVoltage));
    Status = SetRichtekVoltage (OcSetup->RichtekVccGtFixedVidMode, VCCGT_VFIX_LB_REG, VCCGT_VFIX_HB_REG, OcSetup->RichtekVccGtVoltage);

    IsFixedVidUpdated = TRUE;
    VidMode |= (OcSetup->RichtekVccGtFixedVidMode << 1);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "RichTek VccGT programming failed. Status = %r\n", Status));
    }
  }

  //
  // Check for RichTek VccSA voltage override
  //
  if (OcSetup->RichtekVccSaEnable) {
    DEBUG ((DEBUG_INFO, "(OCVR) Programming RichTek VccSA Override = %d\n", OcSetup->RichtekVccSaVoltage));
    Status = SetRichtekVoltage (OcSetup->RichtekVccSaFixedVidMode, VCCSA_VFIX_LB_REG, VCCSA_VFIX_HB_REG, OcSetup->RichtekVccSaVoltage);

    IsFixedVidUpdated = TRUE;
    VidMode |= (OcSetup->RichtekVccSaFixedVidMode);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "RichTek VccSA programming failed. Status = %r\n", Status));
    }
  }

  if (IsFixedVidUpdated) {
    SetRichtekFixedVidMode (VidMode);
  }
}

/**
  Platform Voltage Init - Programs platform voltages over SMBUS.
  Run this function after Smbus2Ppi is installed.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
PlatformVoltageInit (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *ReadOnlyVariable;
  UINTN                           VariableSize;
  UINTN                           SmbusPciBase;
  UINT16                          SmbusIoBar;
  OC_SETUP                        OcSetup;

  DEBUG ((DEBUG_INFO, "(OC) Platform Voltage Init\n"));

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &ReadOnlyVariable
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (OC_SETUP);
  Status = ReadOnlyVariable->GetVariable (
                               ReadOnlyVariable,
                               L"OcSetup",
                               &gOcSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &OcSetup
                               );

  if (EFI_ERROR (Status)) {
    //
    // If GetVariable fails.
    //
    DEBUG ((DEBUG_INFO, "(OC) Failed to read Setup Data\n"));
    return EFI_SUCCESS;
  }

  if (OcSetup.OverclockingSupport == 0) {
    DEBUG ((DEBUG_INFO, "(OC) Skipping Platform Voltage Init, overclocking is not enabled in BIOS setup.\n"));
    return EFI_SUCCESS;
  }

  ///
  /// External Voltage rails control
  ///   - Check if we need to override the voltages of the different VRMs using the SVID or PMBUS interfaces
  ///
  if (OcSetup.OverclockingSupport != 0) {
    //
    // Initial SMBUS
    //
    SmbusIoBar = 0xEFA0;

    //
    // Temporary initialize SMBUS
    //
    SmbusPciBase = PCI_SEGMENT_LIB_ADDRESS (
                     DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SMBUS,
                     PCI_FUNCTION_NUMBER_PCH_SMBUS,
                     0
                     );

    DEBUG ((DEBUG_INFO, "SmbusPciBase = %x\n", SmbusPciBase));
    ///
    /// Set SMBus PCI 0x64 = 0x0A0A0000
    ///
    PciSegmentWrite32 (SmbusPciBase + R_SMBUS_CFG_64, 0x0A0A0000);

    //
    // Initialize I/O BAR
    //
    PciSegmentAndThenOr32 (SmbusPciBase + R_SMBUS_CFG_BASE, B_SMBUS_CFG_BASE_BAR, SmbusIoBar);

    //
    // Enable the Smbus I/O Enable
    //
    PciSegmentOr8 (SmbusPciBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_IO_SPACE);

    //
    // Enable the SMBUS Controller
    //
    PciSegmentOr8 (SmbusPciBase + R_SMBUS_CFG_HOSTC, B_SMBUS_CFG_HOSTC_SSRESET);

    PciSegmentAndThenOr8 (
     SmbusPciBase + R_SMBUS_CFG_HOSTC,
     (UINT8) (~(B_SMBUS_CFG_HOSTC_SMI_EN | B_SMBUS_CFG_HOSTC_I2C_EN)),
     B_SMBUS_CFG_HOSTC_HST_EN
     );
    ProgramVoltageOverrides (&OcSetup);
  }

  return Status;
}
