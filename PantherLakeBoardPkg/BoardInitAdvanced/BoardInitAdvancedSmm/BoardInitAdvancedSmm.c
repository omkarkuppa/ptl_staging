/** @file
  Board Advanced SMM Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include "BoardInitAdvancedSmm.h"
#include <OemSetup.h>
#include <CpuRegs.h>
#include <PchPolicyCommon.h>
#include "PlatformBoardId.h"
#include "AcpiCommon.h"

#include <Library/PmcLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdItssLib.h>
#include <Library/TimerLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Register/GpioV2PcdPins.h>
#include <Protocol/PchTcoSmiDispatch.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/SmmCpu.h>
#include <IntelRcStatusCode.h>
#include <Register/PchRegs.h>
#include <Register/EspiRegs.h>
#include <Register/PmcRegs.h>
#include <Register/RtcRegs.h>
#include <Library/PchPciBdfLib.h>
#include <Register/GenerationMsr.h>
#include <Library/EcSmiLib.h>
#include <Library/EcMiscLib.h>
#include <Library/MemoryAllocationLib.h>
#if FixedPcdGetBool(PcdCapsuleEnable) == 1
#include <Library/HobLib.h>
#include <Library/PcdRtcAccessLib.h>
#endif

//
// Global variables
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SMM_SYSTEM_TABLE2                     *mSmst;

GLOBAL_REMOVE_IF_UNREFERENCED UINT16                                    mAcpiBaseAddr;
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_NVS_AREA                         *mPlatformNvsAreaPtr = 0;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_SMM_DEV                          mAcpiSmm;
//
// Setup variables
//
GLOBAL_REMOVE_IF_UNREFERENCED SETUP_DATA                                mSystemConfiguration;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SETUP                                 mPchSetup;

//
// HMRFPO lock output parameters
//

GLOBAL_REMOVE_IF_UNREFERENCED UINT8                                     mPlatformFlavor;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                                     mBoardType;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                                     mEcPresent;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_SMM_CPU_PROTOCOL                      *mSmmCpu;

GLOBAL_REMOVE_IF_UNREFERENCED UINT32                                    mFpsWakeGpio = 0;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                                   mGpioTier2WakeSupport;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                                     Force15SecGuardBandWakeTimeEnable;
#if FixedPcdGetBool(PcdCapsuleEnable) == 1
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                                   mEndOfDxe = FALSE;
#endif

/**
  Converts Decimal to BCD
  Only for 2 digit BCD.

  @param[in] Dec       - Decimal value to be converted to BCD

  @retval BCD for Dec number
**/
UINT8
DecToBCD (
  UINT8 Dec
  )
{
  UINT8 FirstDigit;
  UINT8 SecondDigit;

  FirstDigit  = Dec % 10;
  SecondDigit = Dec / 10;

  return (SecondDigit << 4) + FirstDigit;
}

/**
  Converts BCD to Dec number
  Only for 2 digit BCD.

  @param[in] BCD       - BCD number which needs to be converted to Dec

  @retval Dec value for given BCD
**/
UINT8
BCDToDec (
  UINT8 BCD
  )
{
  UINT8 FirstDigit;
  UINT8 SecondDigit;
  FirstDigit  = BCD & 0xf;
  SecondDigit = BCD >> 4;

  return SecondDigit * 10 + FirstDigit;
}

/**
  Reads the RTC Index register

  @param[in] Index - Index register

  @retval Value in Index register
**/
UINT8
ReadRtcIndex (
  IN UINT8 Index
  )
{
  UINT8           Value;
  UINT8           Addr;

  //
  // Use port RTC alternative ports 74h/75h to prevent from breaking NMI setting
  //

  //
  // Check if Data Time is valid
  //
  if (Index <= 9) {
    do {
      Addr = 0x0A;
      mSmst->SmmIo.Io.Write (
                        &mSmst->SmmIo,
                        SMM_IO_UINT8,
                        R_RTC_IO_INDEX_ALT,
                        1,
                        &Addr
                        );
      mSmst->SmmIo.Io.Read (
                        &mSmst->SmmIo,
                        SMM_IO_UINT8,
                        R_RTC_IO_TARGET_ALT,
                        1,
                        &Value
                        );
    } while (Value & 0x80);
  }

  Addr = Index;
  mSmst->SmmIo.Io.Write (
                    &mSmst->SmmIo,
                    SMM_IO_UINT8,
                    R_RTC_IO_INDEX_ALT,
                    1,
                    &Addr
                    );
  //
  // Read register.
  //
  mSmst->SmmIo.Io.Read (
                    &mSmst->SmmIo,
                    SMM_IO_UINT8,
                    R_RTC_IO_TARGET_ALT,
                    1,
                    &Value
                    );
  if (Index <= 9) {
    Value = BCDToDec (Value);
  }

  return (UINT8) Value;
}

/**
  Writes to an RTC Index register

  @param[in] Index   - Index to be written
  @param[in] Value   - Value to be written to Index register
**/
VOID
WriteRtcIndex (
  IN UINT8 Index,
  IN UINT8 Value
  )
{
  UINT8           Addr;

  //
  // Use port RTC alternative ports 74h/75h to prevent from breaking NMI setting
  //

  Addr = Index;
  mSmst->SmmIo.Io.Write (
                    &mSmst->SmmIo,
                    SMM_IO_UINT8,
                    R_RTC_IO_INDEX_ALT,
                    1,
                    &Addr
                    );
  if (Index <= 9) {
    Value = DecToBCD (Value);
  }
  //
  // Write Register.
  //
  mSmst->SmmIo.Io.Write (
                    &mSmst->SmmIo,
                    SMM_IO_UINT8,
                    R_RTC_IO_TARGET_ALT,
                    1,
                    &Value
                    );
}

/**
  Clears RTC sts and Alarm Interrupt Enable bit
**/
VOID
ClearRtcSts (
  VOID
  )
{

  UINT8 Value;
  UINT16 Data16;

  //
  // Clear Alarm Flag (AF) by reading RTC Reg B
  //
  Value = ReadRtcIndex (RTC_REGISTER_B);
  if (Value & BIT5) {
    //
    // If Alarm Interrupt Enable (RTC Interrupt)bit is set then
    // Clear Alarm Interrupt Enable
    //
    Value &= ~BIT5;
    WriteRtcIndex (0xB, Value);
    //
    // Clear RTC PM1 status
    //
    Data16 =  B_ACPI_IO_PM1_STS_RTC;
    mSmst->SmmIo.Io.Write (
                        &mSmst->SmmIo,
                        SMM_IO_UINT16,
                        mAcpiBaseAddr + R_ACPI_IO_PM1_STS,
                        1,
                        &Data16
                        );
    //
    // Set Alarm Interrupt Enable
    //
    Value = ReadRtcIndex (RTC_REGISTER_B);
    Value |= BIT5;
    WriteRtcIndex (0xB, Value);
  } else {
    //
    // If Alarm Interrupt bit is not set then directly Clear RTC PM1 status
    //
    Data16 =  B_ACPI_IO_PM1_STS_RTC;
    mSmst->SmmIo.Io.Write (
                       &mSmst->SmmIo,
                       SMM_IO_UINT16,
                       mAcpiBaseAddr + R_ACPI_IO_PM1_STS,
                       1,
                       &Data16
                       );
  }
}

/**
  Enable wake capability for that wake delivered via tier2 GPIO

  @retval EFI_SUCCESS           - Operation successfully performed
**/
EFI_STATUS
EnablePchTier2GpioWakeSupport (
  VOID
  )
{

  UINT16                             Data16;

  if (!mGpioTier2WakeSupport) {
    return EFI_UNSUPPORTED;
  }

  //
  // Enable wake capability when device is present
  //
  mSmst->SmmIo.Io.Read (
                    &mSmst->SmmIo,
                    SMM_IO_UINT16,
                    mAcpiBaseAddr + R_ACPI_IO_GPE0_EN_127_96,
                    1,
                    &Data16
                    );
  Data16 = Data16 | B_ACPI_IO_GPE0_STS_127_96_GPIO_TIER2_SCI_STS;
  mSmst->SmmIo.Io.Write (
                    &mSmst->SmmIo,
                    SMM_IO_UINT16,
                    mAcpiBaseAddr + R_ACPI_IO_GPE0_EN_127_96,
                    1,
                    &Data16
                    );

  return EFI_SUCCESS;
}

/**
  If PCIe native mode is disabled through BIOS setup options,
  Windows ACPI driver initializes the PM1 enable bits with PCIEXP_WAKE_DIS bit set to 1 provided the flag PCI_EXP_WAK is set in FADT.
  If the FADT flag is not set then Windows will not set the PCIEXP_WAKE_DIS bit.

  In summary OS is setting PCIEXP_WAKE_DIS if native PCIe support is disabled.

  To mitigate the wake issue bios will clear this bit in case of Sx entry which will allow system to detect any wake event from Add in card."

  @param[in] VOID

  @retval    VOID
**/
VOID
EnablePchPcieWakeSupport (
  VOID
)
{

  UINT16  Data16;

  if (!(mSystemConfiguration.PciExpNative)) {
    mSmst->SmmIo.Io.Read (
                      &mSmst->SmmIo,
                      SMM_IO_UINT16,
                      mAcpiBaseAddr + R_ACPI_IO_PM1_EN,
                      1,
                      &Data16
                      );
    Data16 = Data16 & (UINT16)~B_ACPI_IO_PM1_EN_PCIEXP_WAKE;
    mSmst->SmmIo.Io.Write (
                      &mSmst->SmmIo,
                      SMM_IO_UINT16,
                      mAcpiBaseAddr + R_ACPI_IO_PM1_EN,
                      1,
                      &Data16
                      );
  }
}

#define FPS_FPC1021  06

/**
  Enable FInger Print Sensor Wake Support
  Default GPIO is configured as GPIO Driver Mode where FPS driver will take care in OS Environment.
  Here Enable Wake support by reconfiguring the GPIO with SCI Capability

  @param[in] VOID

  @retval    VOID
**/
#define DDR3_STANDBY_PWR_GPIO_PIN                      0                    // Platform specific @ TBD

/**
  Updates RTC Alarm registers with GuarbandWakeTime (Current Time + GuardBandTime)
**/
VOID
RtcForceSetGuardBandAlarmTime (
  VOID
  )
{

  UINT8         Value;
  UINT8         RtcHour;
  UINT8         RtcMinute;
  UINT8         RtcSecond;
  //
  // Get Current Time(Hour:min:sec)
  //
  RtcHour   = ReadRtcIndex (RTC_HOUR_INDEX);
  RtcMinute = ReadRtcIndex (RTC_MIN_INDEX);
  RtcSecond = ReadRtcIndex (RTC_SEC_INDEX);
  //
  // Set GuardBandWake Time
  //
  RtcSecond = RtcSecond + GUARD_BAND_TIME;
  RtcMinute = RtcMinute + (RtcSecond/60);
  RtcHour   = RtcHour + (RtcMinute/60);

  RtcMinute   = RtcMinute % 60;
  RtcSecond   = RtcSecond % 60;
  Value     = ReadRtcIndex (RTC_REGISTER_B);
  if (Value & BIT1) {
    if (RtcHour > 23) {
          RtcHour = RtcHour % 24;
      }
  } else {
    if (RtcHour > 11) {
      RtcHour = RtcHour  % 12;
    }
  }
  //
  //  Check if AIE bit is set.
  //
  if (Value & BIT5) {
    //
    // If Alarm Interrupt Enable (RTC Interrupt)bit is set then
    // Clear Alarm Interrupt Enable
    //
    Value &= ~BIT5;
    WriteRtcIndex (0xB, Value);
  }
  //
  // Write GuardBandWake Time to RtcAlarmIndices(Hour:Min:Sec)
  //
  WriteRtcIndex (RTC_HOUR_ALARM_INDEX,RtcHour);
  WriteRtcIndex (RTC_MIN_ALARM_INDEX ,RtcMinute);
  WriteRtcIndex (RTC_SEC_ALARM_INDEX ,RtcSecond);
  //
  // Run in a loop untill correct time gets updated.
  //
  while((RtcHour != ReadRtcIndex (RTC_HOUR_ALARM_INDEX)) || (RtcMinute != ReadRtcIndex (RTC_MIN_ALARM_INDEX)) || (RtcSecond != ReadRtcIndex (RTC_SEC_ALARM_INDEX))) {
    WriteRtcIndex (RTC_HOUR_ALARM_INDEX,RtcHour);
    WriteRtcIndex (RTC_MIN_ALARM_INDEX,RtcMinute);
    WriteRtcIndex (RTC_SEC_ALARM_INDEX,RtcSecond);
  }
  //
  // Set AIE bit
  //
  Value = ReadRtcIndex (RTC_REGISTER_B);
  Value |= 1 << 5;
  WriteRtcIndex (RTC_REGISTER_B, Value);
}


/**
  Clearing RTC status register and enabling RTC enable register after timer has been updated.
  @param[in]         None
**/
VOID
ConfigureGuardBandRtcReg (
  VOID
  )
{
  UINT16                 Data16;
  //
  // Check if AIE(Alarm Interrupt Bit) bit is set
  //
  if (ReadRtcIndex (RTC_REGISTER_B) & BIT5) {
    //
    // Clear RTC PM1 status
    //
    mSmst->SmmIo.Io.Read (
                      &mSmst->SmmIo,
                      SMM_IO_UINT16,
                      mAcpiBaseAddr + R_ACPI_IO_PM1_STS,
                      1,
                      &Data16
                      );
    if (Data16 & B_ACPI_IO_PM1_STS_RTC) {
      ClearRtcSts ();
    }
    //
    // Set GuardBandWakeTime
    //
    RtcForceSetGuardBandAlarmTime ();
    //
    // Set RTC_EN bit in PM1_EN to wake back the System from Sx state
    //
    mSmst->SmmIo.Io.Read (
                      &mSmst->SmmIo,
                      SMM_IO_UINT16,
                      mAcpiBaseAddr + R_ACPI_IO_PM1_EN,
                      1,
                      &Data16
                      );
    Data16 = Data16 | B_ACPI_IO_PM1_EN_RTC;
    mSmst->SmmIo.Io.Write (
                      &mSmst->SmmIo,
                      SMM_IO_UINT16,
                      mAcpiBaseAddr + R_ACPI_IO_PM1_EN,
                      1,
                      &Data16
                      );
  } else {
    return;
  }
}
/**
  Updates WADT Registers with GuardBandTime

  @param[in]         None
**/
VOID
WadtForceSetGuardBandAlarmTime (
  VOID
  )
{
  //
  // Add GuardBandTime to AC Timer
  //
  MmioWrite32 (PmcGetPwrmBase () + R_PMC_PWRM_WADT_AC,GUARD_BAND_TIME);
  //
  // Add GuardBandTime to DC Timer
  //
  MmioWrite32 (PmcGetPwrmBase () + R_PMC_PWRM_WADT_DC,GUARD_BAND_TIME);
}


/**
  Enables RTC Wake from S5 and fast startup flow.

  @param[in] VOID

  @retval    VOID
**/
VOID
WakeOnRTCS5 (
  VOID
  )
{
  UINT16    Data16;
  if ((mSystemConfiguration.WakeOnRTCS5 == 1) && (ReadRtcIndex (RTC_REGISTER_B) & BIT5)) {
    //
    // Clear RTC PM1 status
    //
    mSmst->SmmIo.Io.Read (
                  &mSmst->SmmIo,
                  SMM_IO_UINT16,
                  mAcpiBaseAddr + R_ACPI_IO_PM1_STS,
                  1,
                  &Data16
                  );
    if (Data16 & B_ACPI_IO_PM1_STS_RTC) {
      //
      // Clears RTC Timer
      //
      ClearRtcSts ();
    }
    //
    // Set RTC_EN bit in PM1_EN to wake back the System from Sx state
    //
    mSmst->SmmIo.Io.Read (
                      &mSmst->SmmIo,
                      SMM_IO_UINT16,
                      mAcpiBaseAddr + R_ACPI_IO_PM1_EN,
                      1,
                      &Data16
                      );
    Data16 = Data16 | B_ACPI_IO_PM1_EN_RTC;
    mSmst->SmmIo.Io.Write (
                      &mSmst->SmmIo,
                      SMM_IO_UINT16,
                      mAcpiBaseAddr + R_ACPI_IO_PM1_EN,
                      1,
                      &Data16
                      );
  }
}

/**
  S3 Sleep Entry Call Back.

  @param[in] DispatchHandle     - The handle of this callback, obtained when registering
  @param[in] DispatchContext    - The predefined context which contained sleep type and phase
  @param[in] CommBuffer         - A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in] CommBufferSize     - The size of the CommBuffer.

  @retval EFI_SUCCESS           - Operation successfully performed
**/
EFI_STATUS
EFIAPI
S3SleepEntryCallBack (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer  OPTIONAL,
  IN  UINTN                         *CommBufferSize  OPTIONAL
  )
{
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_BIOS_S3_ENTRY); // PostCode (BIOS_S3_ENTRY);

  //
  // Enable Wakesupport for PCIE Add-on Cards when Native support is disabled
  //
  EnablePchPcieWakeSupport ();

  if ((mPlatformFlavor == FlavorMobile) || (mPlatformFlavor == FlavorEmbedded)) {
    //
    // This GPIO operation is designed to take care of DDR3 Standby Power Reduction
    //  PullDown GPIO
    //
    //GpioSetPadElectricalConfig (DDR3_STANDBY_PWR_GPIO_PIN, GpioTermNone); //@todo, uncomment when DDR3_STANDBY_PWR_GPIO_PIN has been assigned correct GpioPad
  }

  //
  // Enable Tier2 GPIO Wake support
  //
  EnablePchTier2GpioWakeSupport ();
  //
  //
  // Check if GuardBandTimer is Enabled
  //
  if (Force15SecGuardBandWakeTimeEnable == 1) {
    //
    // Check if the RtcInterrrupt is enabled
    //
    if (mSystemConfiguration.SystemTimeAndAlarmSource == 1) {
      //
      // Configuring RTC
      //
      ConfigureGuardBandRtcReg ();
    } else {
      //
      // Set WADT GuardBandTime
      //
      WadtForceSetGuardBandAlarmTime ();
    }
  }
  //
  // WARNING!!! Please do not modify GP_LVL register after this point
  //
  return EFI_SUCCESS;
}

/**
  S4 Sleep Entry Call Back.

  @param[in] DispatchHandle     - The handle of this callback, obtained when registering
  @param[in] DispatchContext    - The predefined context which contained sleep type and phase
  @param[in] CommBuffer         - A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in] CommBufferSize     - The size of the CommBuffer.

  @retval EFI_SUCCESS           - Operation successfully performed
**/
EFI_STATUS
EFIAPI
S4SleepEntryCallBack (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer  OPTIONAL,
  IN  UINTN                         *CommBufferSize  OPTIONAL
  )
{
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_BIOS_S4_ENTRY); // PostCode (BIOS_S4_ENTRY);

  if (mEcPresent == 1) {
    //
    // Put EC in non-ACPI mode.  This ensures EC takes over FAN control from now on till actual shutdown.
    //
    EnableEcAcpiMode (FALSE);
  }
  //
  // Enable Wakesupport for PCIE Add-on Cards when Native support is disabled
  //
  EnablePchPcieWakeSupport ();
  //
  // WARNING!!! Please do not modify GP_LVL register after this point
  //

  //
  // Enable Tier2 GPIO Wake support
  //
  EnablePchTier2GpioWakeSupport ();
  //
  // Check if GuardBandTimer is Enabled
  //
  if (Force15SecGuardBandWakeTimeEnable == 1) {
    //
    // Check if the RtcInterrrupt is enabled
    //
    if (mSystemConfiguration.SystemTimeAndAlarmSource == 1) {
      //
      // RTC Enable
      //
      ConfigureGuardBandRtcReg ();
    } else {
      //
      // Set  WADT GuardBandTime
      //
      WadtForceSetGuardBandAlarmTime ();
    }
  } else {
    //
    // Enable RTC for fast start up flow
    //
    WakeOnRTCS5 ();
  }
  return EFI_SUCCESS;
}

/**
  S5 Sleep Entry Call Back.

  @param[in] DispatchHandle     - The handle of this callback, obtained when registering
  @param[in] DispatchContext    - The predefined context which contained sleep type and phase
  @param[in] CommBuffer         - A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in] CommBufferSize     - The size of the CommBuffer.

  @retval EFI_SUCCESS           - Operation successfully performed
**/
EFI_STATUS
EFIAPI
S5SleepEntryCallBack (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer  OPTIONAL,
  IN  UINTN                         *CommBufferSize  OPTIONAL
  )
{
  UINT32    Data32;

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_BIOS_S5_ENTRY); // PostCode (BIOS_S5_ENTRY);

  if (mEcPresent == 1) {
    //
    // Put EC in non-ACPI mode.  This ensures EC takes over FAN control from now on till actual shutdown.
    //
    EnableEcAcpiMode (FALSE);
  }

  //
  // Enable Wakesupport for PCIE Add-on Cards when Native support is disabled
  //
  EnablePchPcieWakeSupport ();
  //
  // Check if the Wake on RTC is enabled and RtcInterrrupt is enabled
  //
  if (mSystemConfiguration.WakeOnRTCS5 == 1) {
    //
    // Check if GuardBandTimer is enabled
    //
    if (Force15SecGuardBandWakeTimeEnable == 1) {
      //
      // RTC Enable
      //
      ConfigureGuardBandRtcReg ();
    } else {
      //
      // Enable RTC for S5 flow
      //
      WakeOnRTCS5 ();
    }
  }

  if (mSystemConfiguration.SystemTimeAndAlarmSource == 0) {
    Data32 =  B_ACPI_IO_GPE0_STS_127_96_WADT_STS;
    //
    // Clear WADT STS bit before enabling wadt_en bit.
    //
    mSmst->SmmIo.Io.Write (
           &mSmst->SmmIo,
           SMM_IO_UINT32,
           mAcpiBaseAddr + R_ACPI_IO_GPE0_STS_127_96,
           1,
           &Data32
           );
    //
    // Read GPE0_EN bit
    //
    mSmst->SmmIo.Io.Read (
           &mSmst->SmmIo,
           SMM_IO_UINT32,
           mAcpiBaseAddr + R_ACPI_IO_GPE0_EN_127_96,
           1,
           &Data32
           );
    //
    // Read GPE0 enable register and set WADT_EN enable (bit 18) to enable
    // WADT from S5 state.
    //
    Data32 |= B_ACPI_IO_GPE0_EN_127_96_WADT_EN;
    mSmst->SmmIo.Io.Write (
             &mSmst->SmmIo,
             SMM_IO_UINT32,
             mAcpiBaseAddr + R_ACPI_IO_GPE0_EN_127_96,
             1,
             &Data32
             );
  }
  //
  // WARNING!!! Please do not modify GP_LVL register after this point
  //

  return EFI_SUCCESS;
}

/**
  When a power button event happens, it shuts off the machine

  @param[in] DispatchHandle    - Handle of this dispatch function
  @param[in] DispatchContext   - Pointer to the dispatch function's context
  @param[in] CommBuffer        - A pointer to a collection of data in memory that will
                                 be conveyed from a non-SMM environment into an SMM environment.
  @param[in] CommBufferSize    - The size of the CommBuffer.

  @retval EFI_SUCCESS          - Operation successfully performed
**/
EFI_STATUS
EFIAPI
PowerButtonCallback (
  IN  EFI_HANDLE                 DispatchHandle,
  IN  CONST VOID                 *DispatchContext,
  IN  OUT VOID                   *CommBuffer  OPTIONAL,
  IN  UINTN                      *CommBufferSize  OPTIONAL
  )
{
  UINT32  Buffer;
  UINT16  Data16;

  //
  // Clear Power Button Status
  //
  Data16 = B_ACPI_IO_PM1_STS_PWRBTN;
  mSmst->SmmIo.Io.Write (
                    &mSmst->SmmIo,
                    SMM_IO_UINT16,
                    mAcpiBaseAddr + R_ACPI_IO_PM1_STS,
                    1,
                    &Data16
                    );

  //
  // Shut it off now
  //
  mSmst->SmmIo.Io.Read (
                    &mSmst->SmmIo,
                    SMM_IO_UINT32,
                    mAcpiBaseAddr + R_ACPI_IO_PM1_CNT,
                    1,
                    &Buffer
                    );
  Buffer &= ~(B_ACPI_IO_PM1_CNT_SLP_EN | B_ACPI_IO_PM1_CNT_SLP_TYP);

  Buffer |= V_ACPI_IO_PM1_CNT_S5;

  mSmst->SmmIo.Io.Write (
                    &mSmst->SmmIo,
                    SMM_IO_UINT32,
                    mAcpiBaseAddr + R_ACPI_IO_PM1_CNT,
                    1,
                    &Buffer
                    );

  Buffer |= B_ACPI_IO_PM1_CNT_SLP_EN;

  mSmst->SmmIo.Io.Write (
                    &mSmst->SmmIo,
                    SMM_IO_UINT32,
                    mAcpiBaseAddr + R_ACPI_IO_PM1_CNT,
                    1,
                    &Buffer
                    );

  return EFI_SUCCESS;
}

/**
  This hardware SMI handler will be run every time the BIOS Write Enable bit is set.

  @param[in] DispatchHandle       Not used

**/
VOID
EFIAPI
PchTcoTimeoutCallback (
  IN  EFI_HANDLE                              DispatchHandle
  )
{
  DEBUG((DEBUG_INFO, "PchTcoTimeoutCallback entry\n"));

  //
  // Stall for 3 seconds to trigger the second timeout.
  //
  MicroSecondDelay (3000 * 1000);

  //
  // A catastrophic crash happened (causing TCO timeout) when we reach here
  // Directly trigger a warm reboot here
  //
  IoWrite8 (R_PCH_IO_RST_CNT, V_PCH_IO_RST_CNT_HARDRESET);

  CpuDeadLoop ();

  return;
}

/**
  Register a SMI handler for Tco timeout event.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid pointer passed.
**/
EFI_STATUS
EFIAPI
RegisterTcoTimeoutHandler (
  VOID
  )
{
  EFI_STATUS                                Status;
  PCH_TCO_SMI_DISPATCH_PROTOCOL             *mPchTcoSmiDispatchProtocol;
  EFI_HANDLE                                Handle;

  Status = mSmst->SmmLocateProtocol (&gPchTcoSmiDispatchProtocolGuid, NULL, (VOID **) &mPchTcoSmiDispatchProtocol);

  Handle = NULL;
  Status = mPchTcoSmiDispatchProtocol->TcoTimeoutRegister (
                                         mPchTcoSmiDispatchProtocol,
                                         PchTcoTimeoutCallback,
                                         &Handle
                                         );
  DEBUG((DEBUG_INFO, "RegisterTcoTimeoutHandler status = %r\n", Status));

  return Status;
}

#if FixedPcdGetBool(PcdCapsuleEnable) == 1
#if (FixedPcdGet8 (PcdEmbeddedEnable) == 0x0 && FixedPcdGetBool (PcdTopSwapSmiSupport) == 0x1)
/**
  SMM END_OF_DXE protocol notification event handler.

  @param  Protocol   Points to the protocol's unique identifier
  @param  Interface  Points to the interface instance
  @param  Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS   SmmEndOfDxeCallback runs successfully

**/
EFI_STATUS
EFIAPI
SmmEndOfDxeCallback (
  IN CONST EFI_GUID                       *Protocol,
  IN VOID                                 *Interface,
  IN EFI_HANDLE                           Handle
  )
{
  DEBUG ((DEBUG_INFO, "[SmmPlatform]SMM_END_OF_DXE is signaled\n"));
  mEndOfDxe = TRUE;

  return EFI_SUCCESS;
}

/**
  Software SMI callback to set TopSwap bit. Only allowed to access TopSwap bit before EndOfDxe.

  @param[in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      DispatchContext Points to an optional handler context which was specified when the
                                  handler was registered.
  @param[in, out] CommBuffer      A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS             The interrupt was handled successfully.

**/
EFI_STATUS
EFIAPI
EnableTopSwapCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer  OPTIONAL,
  IN  OUT UINTN                     *CommBufferSize  OPTIONAL
  )
{
  if (!mEndOfDxe) {
    PtlPcdTopSwapSet (TRUE);
  }

  return EFI_SUCCESS;
}

/**
  Software SMI callback to clear TopSwap bit. Only allowed to access TopSwap bit before EndOfDxe.

  @param[in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      DispatchContext Points to an optional handler context which was specified when the
                                  handler was registered.
  @param[in, out] CommBuffer      A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS             The interrupt was handled successfully.

**/
EFI_STATUS
EFIAPI
DisableTopSwapCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer  OPTIONAL,
  IN  OUT UINTN                     *CommBufferSize  OPTIONAL
  )
{
  if (!mEndOfDxe) {
    PtlPcdTopSwapSet (FALSE);
  }

  return EFI_SUCCESS;
}

/**
  Register a SMI handler to set/clear TopSwap bit.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval Other                         SMI handler is not registered.
**/
EFI_STATUS
RegisterTopSwapSwSmiHandler (
  VOID
  )
{
  EFI_STATUS                                Status;
  VOID                                      *SmmEndOfDxeRegistration;
  EFI_SMM_SW_DISPATCH2_PROTOCOL             *SwDispatch;
  EFI_HANDLE                                SwHandle;
  EFI_SMM_SW_REGISTER_CONTEXT               SwContext;

  DEBUG((DEBUG_INFO, "RegisterTopSwapSwSmiHandler entry\n"));

  if (GetFirstGuidHob (&gSkipBiosLockForSysFwUpdateGuid) == NULL) {
    DEBUG ((DEBUG_INFO, "BIOS Update is not intended, skip to register TopSwap SMI callback\n"));
    return EFI_SUCCESS;
  }

  //
  // Register EFI_SMM_END_OF_DXE_PROTOCOL_GUID notify function.
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gEfiSmmEndOfDxeProtocolGuid,
                    SmmEndOfDxeCallback,
                    &SmmEndOfDxeRegistration
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_READY;
  }

  Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**)&SwDispatch);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_READY;
  }

  //
  // Register TopSwap enable/disable handler
  //
  SwContext.SwSmiInputValue = (UINTN) -1;
  Status = SwDispatch->Register (
                         SwDispatch,
                         EnableTopSwapCallback,
                         &SwContext,
                         &SwHandle
                         );
  ASSERT_EFI_ERROR (Status);
  if (Status == EFI_SUCCESS) {
    PcdSet8S (PcdTopSwapEnableSwSmi, (UINT8) SwContext.SwSmiInputValue);
  }

  SwContext.SwSmiInputValue = (UINTN) -1;;
  Status = SwDispatch->Register (
                         SwDispatch,
                         DisableTopSwapCallback,
                         &SwContext,
                         &SwHandle
                         );
  ASSERT_EFI_ERROR (Status);
  if (Status == EFI_SUCCESS) {
    PcdSet8S (PcdTopSwapDisableSwSmi, (UINT8) SwContext.SwSmiInputValue);
  }

  return EFI_SUCCESS;
}
#endif
#endif

/**
  Initializes the SMM Platform Driver

  @param[in] ImageHandle   - Pointer to the loaded image protocol for this driver
  @param[in] SystemTable   - Pointer to the EFI System Table

  @retval Status           - EFI_SUCCESS
  @retval Assert, otherwise.

**/
EFI_STATUS
EFIAPI
InitializePlatformSmm (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                Status;
  EFI_HANDLE                                Handle;
  EFI_HANDLE                                PowerButtonHandle;
  EFI_HANDLE                                S4SleepEntryHandle;
  EFI_HANDLE                                S5SleepEntryHandle;
#if FixedPcdGetBool (PcdS3Enable) == 1
  EFI_HANDLE                                S3SleepEntryHandle;
#endif

  EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL   *PowerButtonDispatch;
  EFI_SMM_SW_DISPATCH2_PROTOCOL             *SwDispatch;
  EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT     PowerButtonContext;
  EFI_SMM_SX_DISPATCH2_PROTOCOL             *SxDispatch;
  EFI_SMM_SX_REGISTER_CONTEXT               EntryDispatchContext;
  PLATFORM_NVS_AREA_PROTOCOL                *PlatformNvsAreaProtocol;
  UINTN                                     DataSize;

  PCH_ESPI_SMI_DISPATCH_PROTOCOL            *PchEspiSmiDispatchProtocol;
  UINTN                                      VarDataSize;


  Handle              = NULL;
  PowerButtonHandle   = NULL;

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_INIT_SMM_ENTRY); // PostCode (BOARD_INIT_SMM_ENTRY);

  //
  // Great!  We're now in SMM!
  //
  //
  // Locate Platform Info Protocol.
  //
  mPlatformFlavor = PcdGet8 (PcdPlatformFlavor);
#if FixedPcdGetBool (PcdEcEnable) == 1
  mEcPresent      = PcdGetBool (PcdEcPresent);
#else
  mEcPresent      = 0;
#endif
  mGpioTier2WakeSupport = PcdGetBool (PcdGpioTier2WakeEnable);

  //
  // Initialize global variables
  //
  mSmst = gSmst;

  //
  // Locate setup variable
  // We assert if it is not found because we have a dependency on AcpiPlatform which is
  // dependent on protocols the setup driver produces.
  //
  DataSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &DataSize,
                  &mSystemConfiguration
                  );
//  ASSERT_EFI_ERROR (Status);

  DataSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  L"PchSetup",
                  &gPchSetupVariableGuid,
                  NULL,
                  &DataSize,
                  &mPchSetup
                  );

  VarDataSize = sizeof (UINT8);
  Status = gRT->GetVariable (
                  L"Force15SecGuardBandWakeTimeEnable",
                  &gUefiIntelPlatformVariablesGuid,
                  NULL,
                  &VarDataSize,
                  &Force15SecGuardBandWakeTimeEnable
                  );


  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO,"GuardBandTimer is not enabled"));
  } else {
    DEBUG ((DEBUG_INFO,"GuardBandTimer is initialised=%x",Force15SecGuardBandWakeTimeEnable));
  }

  //
  // Get the ACPI Base Address
  //
  mAcpiBaseAddr = PmcGetAcpiBase ();

  //
  // Clears RTC Timer
  //
  ClearRtcSts ();

  //
  // Locate the ICH SMM Power Button dispatch protocol
  //
  Status = mSmst->SmmLocateProtocol (&gEfiSmmPowerButtonDispatch2ProtocolGuid, NULL, (VOID**)&PowerButtonDispatch);
  ASSERT_EFI_ERROR (Status);

  //
  // Register the power button SMM event
  //
  PowerButtonContext.Phase = EfiPowerButtonEntry;
  Status = PowerButtonDispatch->Register (
                                  PowerButtonDispatch,
                                  PowerButtonCallback,
                                  &PowerButtonContext,
                                  &PowerButtonHandle
                                  );
  ASSERT_EFI_ERROR (Status);

  //
  //  Locate the ICH SMM SW dispatch protocol
  //
  Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**)&SwDispatch);
  ASSERT_EFI_ERROR (Status);

  //
  // Locate our shared data area
  //
  Status = gBS->LocateProtocol (&gPlatformNvsAreaProtocolGuid, NULL, (VOID **) &PlatformNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);
  mPlatformNvsAreaPtr = PlatformNvsAreaProtocol->Area;

  //
  // Locate the SmmSxDispatch protocol
  //
  Status = mSmst->SmmLocateProtocol (&gEfiSmmSxDispatch2ProtocolGuid, NULL, (VOID**)&SxDispatch);
  if (EFI_ERROR (Status)) {
    return Status;
  }

#if FixedPcdGetBool (PcdS3Enable) == 1
  //
  // Register S3 entry phase call back function
  //
  EntryDispatchContext.Type  = SxS3;
  EntryDispatchContext.Phase = SxEntry;
  Status = SxDispatch->Register (
                         SxDispatch,
                         S3SleepEntryCallBack,
                         &EntryDispatchContext,
                         &S3SleepEntryHandle
                         );
#endif
  //
  // Register S4-Entry phase callback function
  //
  EntryDispatchContext.Type  = SxS4;
  EntryDispatchContext.Phase = SxEntry;
  Status = SxDispatch->Register (
                         SxDispatch,
                         S4SleepEntryCallBack,
                         &EntryDispatchContext,
                         &S4SleepEntryHandle
                         );
  //
  // Register S5 entry phase call back function
  //
  EntryDispatchContext.Type  = SxS5;
  EntryDispatchContext.Phase = SxEntry;
  Status = SxDispatch->Register (
                         SxDispatch,
                         S5SleepEntryCallBack,
                         &EntryDispatchContext,
                         &S5SleepEntryHandle
                         );

#if FixedPcdGetBool(PcdS3Enable) == 1
  //
  // Initialize RuntimeScriptTable base address
  //
  Status = InitializeSmmScriptLib (SystemTable, 2, &(mAcpiSmm.RuntimeScriptTableBase));
  DEBUG((DEBUG_INFO, "RuntimeScriptTableBase is 0x%x\n", mAcpiSmm.RuntimeScriptTableBase));
  ASSERT_EFI_ERROR (Status);
#endif

  //
  // Lock HMRFPO
  // Output parameters are stored in global variables that can be used in subsequent calls in SMM
  // call back functions, e.g., HMRFPO Enable
  //

  //
  //   Locate the PCH eSPI SMI dispatch protocol.
  //
  PchEspiSmiDispatchProtocol = NULL;
  Status = mSmst->SmmLocateProtocol (&gPchEspiSmiDispatchProtocolGuid, NULL, (VOID **) &PchEspiSmiDispatchProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Locate the PCH eSPI SMI dispatch protocol failure\n"));
  } else {

    //
    // Register an eSPI EC SMI callback function.
    //
    Handle = NULL;
    Status = PchEspiSmiDispatchProtocol->EspiPeripheralSmiRegister (
                                       PchEspiSmiDispatchProtocol,
                                       eSpiEcSmiCallback,
                                       &Handle
                                       );
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "Register eSPI EC SMI callback function failure\n"));
    }
  }

  //
  // Register TCO timeout SMI handler
  //
  Status = RegisterTcoTimeoutHandler ();

#if FixedPcdGetBool(PcdCapsuleEnable) == 1
  //
  // TopSwap Manipulation SMI handler
  //
#if (FixedPcdGet8 (PcdEmbeddedEnable) == 0x1 || FixedPcdGetBool (PcdTopSwapSmiSupport) == 0x0)
  DEBUG ((DEBUG_INFO, "Skip to register TopSwap SMI\n"));
#else
  Status = RegisterTopSwapSwSmiHandler ();
#endif
#endif

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_INIT_SMM_EXIT); // PostCode (BOARD_INIT_SMM_EXIT);

  return EFI_SUCCESS;
}
