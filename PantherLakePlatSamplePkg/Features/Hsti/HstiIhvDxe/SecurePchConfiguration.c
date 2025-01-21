/** @file
  This file contains the tests for the SecuePchConfiguration bit

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

@par Specification
**/

#include "HstiIhvDxe.h"
#include <Library/TcoLib.h>
#include <Library/ThermalAccessLib.h>
#include <Library/PcdLib.h>
#include <Library/VmdInfoLib.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include <Protocol/DevicePath.h>
#include <Register/PmcRegs.h>
#include <HiddenDevices.h>

UINT64  CodeReportedBitmap;

// TODO:remove begin
// temporary debug solution for lack of library (offsets are defined privately)
#define R_PMC_PWRM_THERMAL_CTEN                               0x150C  ///< Catastrophic Trip Point Enable
#define B_PMC_PWRM_THERMAL_CTEN_CTENLOCK                      BIT31   ///< Policy Lock-Down Bit
#define B_PMC_PWRM_THERMAL_CTEN_CPDEN                         BIT0    ///< Catastrophic Power-Down Enable

#define R_PMC_PWRM_THERMAL_ECRPTEN                            0x1510  ///< EC Thermal Sensor Reporting Enable
#define B_PMC_PWRM_THERMAL_ECRPTEN_ECRPTENLOCK                BIT31   ///< Lock-Down Bit
#define B_PMC_PWRM_THERMAL_ECRPTEN_EN_PMC_TO_EC_TEMP_RPT      BIT0    ///< Enable PMC to EC Temperature Reporting

#define R_PMC_PWRM_THERMAL_TL                                 0x1520  ///< Throttle Levels
#define B_PMC_PWRM_THERMAL_TL_TLLOCK                          BIT31   ///< TL LOCK
#define B_PMC_PWRM_THERMAL_TL_TTEN                            BIT29   ///< TT Enable

#define R_PMC_PWRM_THERMAL_TLEN                               0x1528  ///< Throttle Levels Enable
#define B_PMC_PWRM_THERMAL_TLEN_TLENLOCK                      BIT31   ///< TLENLOCK

#define R_PMC_PWRM_THERMAL_PHLC                               0x1540  ///< PCH Hot Level Control
#define B_PMC_PWRM_THERMAL_PHLC_PHLCLOCK                      BIT31   ///< PHL Lock
#define B_PMC_PWRM_THERMAL_PHLC_PHLE                          BIT15   ///< PHL Enable

#define R_PTL_PCD_P_PMC_PWRM_TSS0                             0x1560
#define B_PTL_PCD_P_PMC_PWRM_TSS0_TSS0LOCK                    BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS1                             0x1564
#define B_PTL_PCD_P_PMC_PWRM_TSS1_TSS1LOCK                    BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS2                             0x1568
#define B_PTL_PCD_P_PMC_PWRM_TSS2_TSS2LOCK                    BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS3                             0x156C
#define B_PTL_PCD_P_PMC_PWRM_TSS3_TSS3LOCK                    BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS4                             0x1570
#define B_PTL_PCD_P_PMC_PWRM_TSS4_TSS4LOCK                    BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS5                             0x1574
#define B_PTL_PCD_P_PMC_PWRM_TSS5_TSS5LOCK                    BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS6                             0x1578
#define B_PTL_PCD_P_PMC_PWRM_TSS6_TSS6LOCK                    BIT31
#define R_PTL_PCD_P_PMC_PWRM_TSS7                             0x157C
#define B_PTL_PCD_P_PMC_PWRM_TSS7_TSS7LOCK                    BIT31

#define R_PMC_PWRM_DTS_S0IX_CONFIG                            0x1580      ///< DTS-SIP operation in S0ix
#define B_PMC_PWRM_DTS_S0IX_CONFIG_DISABLE_DTS_IN_S0IX        BIT0        ///< DISABLE_DTS_IN_S0IX
#define B_PMC_PWRM_DTS_S0IX_CONFIG_ENABLE_DTS_THRESHOLD_XING_CHECK BIT1   ///< ENABLE DTS THRESHOLD XING CHECK
#define B_PMC_PWRM_DTS_S0IX_CONFIG_S0IXLOCK                   BIT31       ///< Policy Lock-Down Bit

#define R_PMC_PWRM_SOCIFTTC                                   0x15A8   ///< SoC Internal Fabric Thermal Throttling Configuration
#define B_PMC_PWRM_SOCIFTTC_SOCIFTTML                         BIT1     ///< SoC Internal Fabric Thermal Throttling Mechanism Lock

#define R_PTL_PCD_P_PMC_PMSYNC_TPR_CFG                        0x18D4
#define B_PTL_PCD_P_PMC_PMSYNC_TPR_CFG_PCH2CPU_TPR_CFG_LOCK   BIT31

#define R_PMC_PWRM_THERMAL_DOWN_PBC                           0x18E4   ///< PM_DOWN PCH Budget Configuration
#define B_PMC_PWRM_THERMAL_DOWN_PBC_CFGLOCK                   BIT0

#define R_PMC_PWRM_THERMAL_THROTTLING                         0x1BE0   ///< PMC Thermal Throttling
#define B_PMC_PWRM_THERMAL_THROTTLING_VRALERT_EN              BIT0     ///< VRALERT Enable
#define B_PMC_PWRM_THERMAL_THROTTLING_PMC_THROT_LOCK          BIT15    /// PMC Throttle

#define R_PTL_PCD_P_PMC_PWRM_PMCR                             0x1DB0
#define B_PTL_PCD_P_PMC_PWRM_PMCR_PGD_LOCK                    BIT0
//TODO: remove end


/**
  Check Secure PCH Acs Ech Configuration
  @param[in] Result - Secure Pch Configuration Tests Result
  @retval EFI_SUCCESS Error Not Detected.
  @retval EFI_ERROR   Error Detected.
**/
EFI_STATUS
SecurePchAcsEchPortsConfiguration (
  EFI_PCI_IO_PROTOCOL *PciIo,
  PCI_TYPE00          *PciConfig
  )
{
  EFI_STATUS                Status;
  UINT32                    AcsEchPortRegValue  = 0;

  // For each port Verify ACS ECH Value is Correctly Configured
  DEBUG ((DEBUG_INFO, "      Root Port(s) ACS ECH Capability Enabled and Correct Configuration Test\n"));

  //
  // If the PCI device ID equals to VMD_DUMMY_DEVICE_ID, that means this port is a dummy device.
  // once VMD is enabled and if PCIe root port (on function0) is m apped under VMD, then the
  // root port config space will be hidden behind VMD and user cannot read the original root port
  // config space. In that case, dummy device will appear, just for the enumeration purpose.
  // That's why we should skip this check.
  //
  if (PciConfig->Hdr.VendorId == 0x8086 && PciConfig->Hdr.DeviceId == VMD_DUMMY_DEVICE_ID) {
    DEBUG ((DEBUG_INFO, "      Root Port is a dummy device\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Intel PCI root port
  //

  DEBUG ((DEBUG_INFO, "     Checking ACS on root port\n"));
  Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint32, R_PCH_PCIE_CFG_EX_ACSECH, sizeof(AcsEchPortRegValue), &AcsEchPortRegValue);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "     Failed to read AcsEchPortRegValue on root port\n"));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "     AcsEchPortRegValue = 0x%x\n", AcsEchPortRegValue));
  if (!((AcsEchPortRegValue & (V_PCH_PCIE_CFG_EX_ACS_CV << N_PCIE_EXCAP_CV)) &&
  ((AcsEchPortRegValue & V_PCIE_EX_ACS_CID) == V_PCIE_EX_ACS_CID))) {
    DEBUG ((DEBUG_INFO, "          Unexpected Status: ACS feature is not Enabled or ACS ECH is incorrectly Configured = 0x%x\n", AcsEchPortRegValue));
    BuildAndAppendHstiUniqueStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_5,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_5,
      &CodeReportedBitmap,
      BIT5
      );
    return EFI_UNSUPPORTED;
  }


  return EFI_SUCCESS;
}


BOOLEAN
SecurePchPmcPwrmTests (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT32               Uid
  )
{
  UINT32          Value = 0;
  EFI_STATUS      Status;
  BOOLEAN         Result;

  DEBUG ((DEBUG_INFO, "      Thermal Throttling Locks Test\n"));

  Result = TRUE;

  DEBUG ((DEBUG_INFO, "      Thermal TTLOCK Bit Test\n"));
  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_THERMAL_TL, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PMC_PWRM_THERMAL_TL_TLLOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Thermal TTLOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_7,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_7
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Thermal ECRPTENLOCK Bit Test\n"));
  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_THERMAL_ECRPTEN, 1, &Value);
  if (!EFI_ERROR (Status) && !(Value & B_PMC_PWRM_THERMAL_ECRPTEN_ECRPTENLOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Thermal ECRPTENLOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_8,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_8
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Thermal PHLCLOCK Bit Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_THERMAL_PHLC, 1, &Value);
  if (!EFI_ERROR (Status) && !(Value & B_PMC_PWRM_THERMAL_PHLC_PHLCLOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Thermal PHLCLOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_9,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_9
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Thermal TLENLOCK Bit Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_THERMAL_TLEN, 1, &Value);
  if (!EFI_ERROR (Status) && !(Value & B_PMC_PWRM_THERMAL_TLEN_TLENLOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Thermal TLENLOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_A,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_A
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Thermal SOCIFTTML Bit Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_SOCIFTTC, 1, &Value);
  if (!EFI_ERROR (Status) && !(Value & B_PMC_PWRM_SOCIFTTC_SOCIFTTML)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Thermal SOCIFTTML Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_C,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_C
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Thermal PMC_THROT Bit Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_THERMAL_THROTTLING, 1, &Value);
  if (!EFI_ERROR (Status) && !(Value & B_PMC_PWRM_THERMAL_THROTTLING_PMC_THROT_LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Thermal SOCIFTTML Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_D,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_D
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Thermal PPB_CFG Bit Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_THERMAL_DOWN_PBC, 1, &Value);
  if (!EFI_ERROR (Status) && !(Value & B_PMC_PWRM_THERMAL_DOWN_PBC_CFGLOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Thermal SOCIFTTML Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_E,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_E
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Set Strap Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_SSML, 1, &Value);
  if (!EFI_ERROR (Status) && !(Value & B_PMC_PWRM_SSML_SSL)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Strap Lock set to 0\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_2,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_2
      );
    Result = FALSE;
  }


  DEBUG ((DEBUG_INFO, "      PM CFG Debug Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_PM_CFG, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PMC_PWRM_PM_CFG_DBG_MODE_LOCK)) {
    DEBUG((DEBUG_INFO, "        Unexpected Status: Debug Mode set to 0\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_3,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_3
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      SMI Enable Test\n"));

  Status = PciIo->Io.Read(PciIo, EfiPciIoWidthUint32, 2, R_ACPI_IO_SMI_EN, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_ACPI_IO_SMI_EN_GBL_SMI_EN)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Global SMI is Disabled\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_C,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_C
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      SMI Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_GEN_PMCON_B, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PMC_PWRM_GEN_PMCON_B_SMI_LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Global SMI is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_D,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_D
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      VR CONFIG Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PMC_PWRM_GEN_PMCON_B, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PMC_PWRM_GEN_PMCON_B_VR_CONFIG_LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: VR CONFIG Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_6,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_6
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Power Gating Disable Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PTL_PCD_P_PMC_PWRM_PMCR, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PTL_PCD_P_PMC_PWRM_PMCR_PGD_LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: PGD_LOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_10,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_10
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      PCH/SOC Reporting of Thermal and Power Status to the CPU Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PTL_PCD_P_PMC_PMSYNC_TPR_CFG, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PTL_PCD_P_PMC_PMSYNC_TPR_CFG_PCH2CPU_TPR_CFG_LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: PCH2CPU_TPR_CFG_LOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_11,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_11
      );
    Result = FALSE;
  }

  //
  // Temperature Sensor and Status Locks Tests
  //
  DEBUG ((DEBUG_INFO, "      Temperature Sensor and Status 0 Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PTL_PCD_P_PMC_PWRM_TSS0, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PTL_PCD_P_PMC_PWRM_TSS0_TSS0LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TSS0LOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_13,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_13
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Temperature Sensor and Status 1 Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PTL_PCD_P_PMC_PWRM_TSS1, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PTL_PCD_P_PMC_PWRM_TSS1_TSS1LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TSS1LOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_14,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_14
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Temperature Sensor and Status 2 Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PTL_PCD_P_PMC_PWRM_TSS2, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PTL_PCD_P_PMC_PWRM_TSS2_TSS2LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TSS2LOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_15,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_15
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Temperature Sensor and Status 3 Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PTL_PCD_P_PMC_PWRM_TSS3, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PTL_PCD_P_PMC_PWRM_TSS3_TSS3LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TSS3LOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_16,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_16
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Temperature Sensor and Status 4 Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PTL_PCD_P_PMC_PWRM_TSS4, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PTL_PCD_P_PMC_PWRM_TSS4_TSS4LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TSS4LOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_17,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_17
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Temperature Sensor and Status 5 Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PTL_PCD_P_PMC_PWRM_TSS5, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PTL_PCD_P_PMC_PWRM_TSS5_TSS5LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TSS5LOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_18,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_18
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Temperature Sensor and Status 6 Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PTL_PCD_P_PMC_PWRM_TSS6, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PTL_PCD_P_PMC_PWRM_TSS6_TSS6LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TSS56LOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_19,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_19
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Temperature Sensor and Status 7 Lock Test\n"));

  Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, R_PTL_PCD_P_PMC_PWRM_TSS7, 1, &Value);
  if (!EFI_ERROR(Status) && !(Value & B_PTL_PCD_P_PMC_PWRM_TSS7_TSS7LOCK)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TSS57LOCK Bit is Unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_1A,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_1A
      );
    Result = FALSE;
  }

  return Result;
}

/**
  Run tests for SecurePCHConfiguration bit
**/
VOID
CheckSecurePchConfiguration (
  VOID
  )
{

  EFI_STATUS                Status;
  BOOLEAN                   Result;
  UINTN                     HandleCount;
  EFI_HANDLE                *HandleBuffer;
  static UINT32             Index;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  PCI_TYPE00                PciConfig;
  EFI_DEVICE_PATH_PROTOCOL  *DevPath;
  INTC_ACPI_EXTENDED_DEVICE_PATH *Acpi;

  CodeReportedBitmap = 0;

  if ((mFeatureImplemented[1] & HSTI_BYTE1_SECURE_PCH_CONFIGURATION) == 0) {
    return;
  }

  Result = TRUE;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol(HandleBuffer[Index], &gEfiPciIoProtocolGuid, (VOID*) &PciIo);
    if (EFI_ERROR(Status)) {
      continue;
    }

    Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint8,
                        0,
                        sizeof (PciConfig),
                        &PciConfig
                        );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "      Unable to read Pci Config\n"));
      Result = FALSE;
    } else {

      // PCH Acs Ech Ports Test
      if (PciConfig.Hdr.VendorId == 0x8086 && (PciConfig.Hdr.HeaderType & 0xF) == 0x1) {
        Status = SecurePchAcsEchPortsConfiguration (PciIo, &PciConfig);
        if (EFI_ERROR (Status)) {
          Result = FALSE;
        }
      }
    }

    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiDevicePathProtocolGuid, (VOID*) &DevPath);
    if (EFI_ERROR (Status)) {
      continue;
    }

    if ((DevPath->Type == ACPI_DEVICE_PATH) &&
        (DevPath->SubType == ACPI_EXTENDED_DP) &&
        (DevPath->Length[0] == sizeof (INTC_ACPI_EXTENDED_DEVICE_PATH))) {
      Acpi = (INTC_ACPI_EXTENDED_DEVICE_PATH*) DevPath;
      if (!AsciiStrCmp (Acpi->HidStr, PMC_HID_STR)) {
        DEBUG ((DEBUG_INFO, "Found PMC device UID = %d\n", Acpi->Acpi.UID));

        if (!SecurePchPmcPwrmTests (PciIo, Acpi->Acpi.UID)) {
          Result = FALSE;
        }
      }
    }
  }

  // TCO tests
  DEBUG ((DEBUG_INFO, "      TCO SMI Lock Test\n"));
  if (TcoIsSmiLock (PcdGet16 (PcdTcoBaseAddress)) == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TCO SMI is not Locked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_E,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_E
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      SPD Configuration and Write Protection Test\n"));

  if (SmbusIsHostLocked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: SPD is not Write Protected\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_CODE_4,
      HSTI_BYTE1_SECURE_PCH_CONFIGURATION_UNEXP_STATUS_STRING_4
      );
    Result = FALSE;
  }

  // RTC tests
  DEBUG ((DEBUG_INFO, "      RTC Bios Interface Lock Test\n"));
  if (PtlPcdRtcIsBiosInterfaceLock () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: RTC BILD is not Locked\n"));

    BuildAndAppendHstiUniqueStatusString (
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_CODE_4,
      HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_UNEXP_STATUS_STRING_4,
      &CodeReportedBitmap,
      BIT4
      );
  }

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               1,
               HSTI_BYTE1_SECURE_PCH_CONFIGURATION
               );
    CheckStatusForHstiLibSet (Status);
  }

  FreePool (HandleBuffer);
  return;
}
