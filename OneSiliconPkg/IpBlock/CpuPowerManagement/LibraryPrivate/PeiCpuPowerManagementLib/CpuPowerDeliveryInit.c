/** @file
  CPU Power Delivery initializations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include "PowerMgmtInit.h"
#include <Library/BaseLib.h>
#include <Library/CpuCommonLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/BaseMemoryLib.h>
#include <IntelRcStatusCode.h>
#include <Library/CpuPowerManagementLib.h>
#include <Register/B2pMailbox.h>

/**
  Program Tcc Activation Offset and Running Average Temperature Limit (RATL).

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/
VOID
InitRatl (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  MSR_TEMPERATURE_TARGET_REGISTER  TempMsr;
  MSR_PLATFORM_INFO_REGISTER       PlatformInfoMsr;
  BOOLEAN                          IsTccActivationOffsetProgrammable;

  DEBUG ((DEBUG_INFO, "InitRatl Start\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_INITRATL_START); //PostCode (0xC31)

  ///
  /// Check Tcc Activation Offset Programmable Setting from Platform Info MSR Bits[30]
  ///
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  IsTccActivationOffsetProgrammable = PlatformInfoMsr.Bits.PrgTjOffsetEn != 0;
  DEBUG ((DEBUG_INFO,"TCC activation programmable = %x\n", IsTccActivationOffsetProgrammable));

  ///
  /// First check if TCC Activation Offset is programmable based on PLATFORM INFO MSR [30]
  /// If TCC activation offset is not programmable, skip programming TEMPERATURE_TARGET MSR.
  /// If TCC Activation Offset is programable, program the TCC Activation offset value
  /// from Policy, and the Tcc activation offset programming should be dependent on RESET_CPL done.
  ///
  if (IsTccActivationOffsetProgrammable) {
    ///
    /// Read Temperature Target MSR
    ///
    TempMsr.Uint64 = AsmReadMsr64 (MSR_TEMPERATURE_TARGET);

    ///
    /// If TccActivationOffset is greater than max supported value, set it to max value.
    ///
    if (CpuPowerDeliveryConfig->TccActivationOffset > TempMsr.Bits.RefTemp) {
      TempMsr.Bits.TjMaxTccOffset = (UINT8) TempMsr.Bits.RefTemp;
    } else {
      TempMsr.Bits.TjMaxTccOffset = CpuPowerDeliveryConfig->TccActivationOffset;
    }

    ///
    /// Lock the MSR by setting BIT31 of MSR 1A2h.
    ///
    TempMsr.Bits.Locked = 0;
    if (CpuPowerDeliveryConfig->TccOffsetLock) {
      TempMsr.Bits.Locked = 1;
    }

    ///
    /// Write Temperature Target MSR
    ///
    AsmWriteMsr64 (MSR_TEMPERATURE_TARGET, TempMsr.Uint64);
  }

  DEBUG ((DEBUG_INFO, "InitRatl Done\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_INITRATL_DONE); //PostCode (0xC32)

  return;
}

/**
  Configure PL4 Boost through PCODE mailbox.

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/
VOID
ConfigurePl4Boost (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  PCODE_MAILBOX_INTERFACE       MailboxCommand;
  MAILBOX_DATA_PL4_BOOST_POWER  MailboxData;
  UINT32                        MailboxStatus;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO,"Configure PL4 Boost:\n"));

  ///
  /// Skip configuring PL4_BOOST if required value is equal to default value(0) of mailbox.
  ///
  if (CpuPowerDeliveryConfig->PowerLimit4Boost != 0) {
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.Command = MAILBOX_BIOS_CMD_PLATFORM_CONFIG;
    MailboxCommand.Fields.Param1 = MAILBOX_BIOS_SUBCMD_WRITE_PL4_BOOST_POWER;
    MailboxData.Data32 = 0;
    MailboxData.Fields.Pl4Boost = CpuPowerDeliveryConfig->PowerLimit4Boost;
    Status = MailboxWrite (MailboxCommand.InterfaceData, MailboxData.Data32, &MailboxStatus);
    if (Status != EFI_SUCCESS || MailboxStatus != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Write PL4 Boost config failed. MailboxStatus = %x, Mailbox command return status: %r\n", MailboxStatus, Status));
    }
  }
}

/**
  Initialize the processor power management based on hardware capabilities
  and user configuration settings. Include below features:
  1. Initialize Config TDP
  2. Program Miscellaneous Power Management
  3. Initialize CPU Ctdp data in Hob
  4. Initialize thermal features

  @param[in] CpuPowerDeliveryConfig       Pointer to CPU_POWER_DELIVERY_CONFIG

  @retval EFI_SUCCESS - on success
  @retval Appropiate failure code on error
**/
VOID
InitEarlyPpm (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  UINT16                    PowerLimit1;
  UINT16                    PowerLimit2;
  UINT8                     PowerLimitWindow;
  UINT8                     Tar;
  UINT8                     ConfigTdpBootModeIndex;
  BOOLEAN                   ApplyConfigTdp;

  DEBUG ((DEBUG_INFO, "InitEarlyPpm Start\n"));

  ///
  /// Initialize thermal features
  ///
  EarlyThermalInitOnBsp (CpuPowerDeliveryConfig);

  ///
  /// Initialize Config TDP
  ///
  ApplyConfigTdp = CheckCtdpApply (CpuPowerDeliveryConfig);
  if (ApplyConfigTdp){
    InitConfigurableTdpSettings (&PowerLimit1, &PowerLimit2, &PowerLimitWindow, &Tar, &ConfigTdpBootModeIndex, CpuPowerDeliveryConfig);
  } else {
    InitNonConfigurableTdpSettings (&PowerLimit1, &PowerLimit2, &PowerLimitWindow, &Tar, CpuPowerDeliveryConfig);
    ConfigTdpBootModeIndex = 0;
  }

  ConfigurePowerValue (ApplyConfigTdp, PowerLimit1, PowerLimit2, PowerLimitWindow, Tar, ConfigTdpBootModeIndex, CpuPowerDeliveryConfig);

  ///
  /// Initialise Miscellaneous features
  ///
  InitMiscFeatures (CpuPowerDeliveryConfig);

  ///
  /// Initialize CPU Ctdp data in Hob.
  ///
  InitCpuCtdpNvsDataHob (ApplyConfigTdp, PowerLimit1, PowerLimit2, PowerLimitWindow, Tar, ConfigTdpBootModeIndex, CpuPowerDeliveryConfig);

  DEBUG ((DEBUG_INFO, "InitEarlyPpm Done\n"));

  return;
}

/**
  Initialize Cpu Power Data hob and processor power management based on hardware capabilities
  and user configuration settings. Include below features:
  1. Create CPU power management private Data Hob
  2. Program RATL
  3. Configure PL4 Boost through PCODE mailbox
  4. Initialize Ctdp and transfer related config to hob

  @param[in] CpuPowerDeliveryConfig       Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
EFIAPI
CpuPowerDeliveryInit (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  )
{
  DEBUG ((DEBUG_INFO, "CpuPowerDeliveryInit Start \n"));

  ///
  /// Create CPU power management private Data Hob
  ///
  InitializeCpuPowerMgmtPrivateDataHob ();

  ///
  /// Program Tcc Activation Offset and Running Average Temperature Limit (RATL)
  ///
  InitRatl (CpuPowerDeliveryConfig);

  ///
  /// Configure PL4 Boost
  ///
  ConfigurePl4Boost (CpuPowerDeliveryConfig);

  ///
  /// Initialize CPU Power Management related features at early Phase (determine HW and configured state, configure hardware and software accordingly)
  ///
  InitEarlyPpm (CpuPowerDeliveryConfig);

  DEBUG ((DEBUG_INFO, "CpuPowerDeliveryInit Done \n"));

  return;
}
