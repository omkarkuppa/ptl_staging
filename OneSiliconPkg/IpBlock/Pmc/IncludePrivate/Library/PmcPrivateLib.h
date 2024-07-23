/** @file
  Header file for private PmcLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _PMC_PRIVATE_LIB_H_
#define _PMC_PRIVATE_LIB_H_

#include <Library/PmcLib.h>
#include "Register/PmcRegs.h"

/**
  Send PMC IPC1 Normal Read/Write command

  @param[in]  PchPwrmBase       PWRM base address
  @param[in]  Command           Command to be issued to PMC IPC 1 interface
  @param[in]  SubCmdId          SUB_CMD_ID for provided Command
  @param[in]  CmdSize           Total size in byte to be sent via PMC IPC 1 interface
  @param[in]  WriteBufPtr       Pointer to Structure of 4 DWORDs to be issued to PMC IPC 1 interface
  @param[out] ReadBufPtr        Pointer to Structure of 4 DWORDs to be filled by PMC IPC 1 interface

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_INVALID_PARAMETER   Invalid command size
  @retval EFI_DEVICE_ERROR        IPC command failed with an error
  @retval EFI_TIMEOUT             IPC command did not complete after 1s
**/
EFI_STATUS
PmcSendCommand (
  IN  UINTN                    PchPwrmBase,
  IN  UINT8                    Command,
  IN  UINT8                    SubCmdId,
  IN  UINT8                    CmdSize,
  IN  PMC_IPC_COMMAND_BUFFER   *WriteBufPtr,
  OUT PMC_IPC_COMMAND_BUFFER   *ReadBufPtr
  );

/**
  Set ACPI base address.
  The function shall be used for cases where
  ACPI base must be programmed in PMC BAR1 (offset 0x20)
  The Address should not be 0 and should be 256 bytes aligned. It is IO space, so must not exceed 0xFFFF.
  Only address matching PcdAcpiBaseAddress is the acceptable value for ACPI IO Base.
  Function enables ACPI BAR via ACPI Enable bit in ACPI Control register.

  @param[in] PmcPciCfgBase              PMC PCI config base address
  @param[in] Address                    Address for ACPI base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               PMC device not visible
**/
EFI_STATUS
PmcSetAcpiBasePmcBar1 (
  IN  UINT64                            PmcPciCfgBase,
  IN  UINT16                            Address
  );

/**
  Restore Bus Master Enable (BME) bit in PMC device (clear the bit) after PCIe enumaration.

  Read from PMC Command register always returns 0 on I/O enable bit (bit 0).
  Use read/modify/write procedure to PMC Command register causes erasing I/O space enable bit that disables ACPI I/O space.
  PCIe enumerators can use such method and can override I/O enable bit.
  To prevent I/O space disabling by enumerators Bus Master Enable (BME) bit should be set.
  In this case enumerator skips the device during enumeration.

   @param[in] PmcPciCfgBase              PMC PCI config base address

   @retval EFI_SUCCESS                   Successfully completed.
   @retval EFI_UNSUPPORTED               Cannot access PMC device.
 **/
EFI_STATUS
PmcEnableIOSpaceAndBusMaster (
  UINT64           PmcPciCfgBase
  );

/**
  Set PCH PWRM base address.

  @param[in] PmcBase                    PMC PCI base address
  @param[in] Address                    Address for PWRM base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PmcSetPwrmBase (
  IN  UINT64                            PmcBase,
  IN  UINT64                            Address
  );

/**
  Get Virtual GPIO (vGPIO) base address.

  @param[out] VgpioBar                  Vitrual GPIO base address

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Wrong address passed.
**/
EFI_STATUS
PmcGetVgpioBar (
  OUT  UINTN   *VgpioBar
  );

/**
  Enable IBL's Virtual GPIO (vGPIO) feature by
  configuring eSPI interface mode to "eSPI GPIO expander Virtual Wire"

**/
VOID
PmcEnableVgpioEspiVirtualWire (
  VOID
  );

/**
  This function locks static power gating configuration with S3 Boot Script programming
**/
VOID
PmcLockFunctionDisableConfigWithS3BootScript (
  VOID
  );

/**
  This function enables and disables PCIe reference clocks

  @param[in] PchPwrmBase            PWRM base address
  @param[in] ModifyClocksMask       mask of Clock Enables to be modified in this call
  @param[in] EnabledClocksMask      mask of Clock Enables to be set
  @param[in] ModifyPortClkreqMask   mask of PCIe port CLKREQ enable to be set
  @param[in] EnabledPortClkreqMask  mask of PCIe port CLKREQ to enable
**/
VOID
PmcSetPcieClockEnableMask (
  UINTN  PchPwrmBase,
  UINT32 ModifyClocksMask,
  UINT32 EnabledClocksMask,
  UINT32 ModifyPortClkreqMask,
  UINT32 EnabledPortClkreqMask
  );

/**
  This function locks HOST SW power gating control

  @param[in] PmcMmio       PMC MMIO
**/
VOID
PmcLockHostSwPgCtrl (
  UINTN PmcMmio
  );

/**
  This function checks if HOST SW Power Gating Control is locked

  @retval lock state
**/
BOOLEAN
PmcIsHostSwPgCtrlLocked (
  VOID
  );

/**
  This function checks if LAN wake from DeepSx is enabled

  @retval Lan Wake state
**/
BOOLEAN
PmcIsLanDeepSxWakeEnabled (
  VOID
  );

/**
  This function locks down PMC (DebugModeLock)

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcLockWithS3BootScript (
  IN UINTN  PchPwrmBase
  );

/**
  Disables USB2 Core PHY PowerGating during power on for Chipsetinit table update

  @param[in] Address                    Address for PWRM base address.
**/
VOID
PmcUsb2CorePhyPowerGatingDisable (
  IN UINTN  PmcMmio
  );

/**
  Checks if conditions for proper USB2 PHY AFE programming are met

  @retval TRUE    Conditions are met and programming can be done
  @retval FALSE   Conditions are not met and programming will cannot be done
**/
BOOLEAN
PmcIsAfeProgrammingAllowed (
  VOID
  );

/**
  This function reads CPU Early Power-on Configuration (EPOC)

  @retval CPU EPOC value
**/
UINT32
PmcGetCpuEpoc (
  VOID
  );

/**
  This function sets CPU Early Power-on Configuration (EPOC)

  @param[in] CpuEpocValue      CPU EPOC value
**/
VOID
PmcSetCpuEpoc (
  IN UINT32     CpuEpocValue
  );

/**
  This function reads CPU Early Power-on Configuration 2 (EPOC2)

  @retval CPU EPOC2 value
**/
UINT32
PmcGetCpuEpoc2 (
  VOID
  );

/**
  This function sets CPU Early Power-on Configuration 2 (EPOC2)

  @param[in] CpuEpoc2Value      CPU EPOC2 value
**/
VOID
PmcSetCpuEpoc2 (
  IN UINT32     CpuEpoc2Value
  );

/**
  This function sets DRAM_RESET# Control Pin value

  @param[in] DramResetVal      0: Pin output is low
                               1: Pin output is tri-stated
**/
VOID
PmcSetDramResetCtlState (
  IN UINT32     DramResetVal
  );

/**
  This function enables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh
**/
VOID
PmcEnableCf9GlobalReset (
  VOID
  );

/**
  This function enables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh
  Global Reset configuration is locked after programming
**/
VOID
PmcEnableCf9GlobalResetWithLock (
  VOID
  );

/**
  This function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.

  IN UINT32          PchPwrmBase
**/
VOID
PmcDisableCf9GlobalReset (
  IN UINTN          PchPwrmBase
  );

/**
  This function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
  Global Reset configuration is locked after programming

  IN UINT32          PchPwrmBase
**/
VOID
PmcDisableCf9GlobalResetWithLock (
  IN UINTN          PchPwrmBase
  );

/**
  This S3 BootScript only function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
**/
VOID
PmcDisableCf9GlobalResetInS3BootScript (
  VOID
  );

/**
  This S3 BootScript only function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
  Global Reset configuration is locked after programming
**/
VOID
PmcDisableCf9GlobalResetWithLockInS3BootScript (
  VOID
  );

/**
  This function disables CF9 reset without Resume Well reset.
  Cf9 0x6/0xE reset will also reset resume well logic.
**/
VOID
PmcDisableCf9ResetWithoutResumeWell (
  VOID
  );

/**
  This function locks PMC Set Strap Message interface with S3 Boot Script programming
**/
VOID
PmcLockSetStrapMsgInterfaceWithS3BootScript (
  VOID
  );

/**
  This function checks PMC Set Strap Message interface

  @retval State of PMC Set Strap Message Interface
**/
BOOLEAN
PmcIsSetStrapMsgInterfaceEnabled (
  VOID
  );

/**
  This function sets 16-bit PMC Set Strap Message interface data.

  @param[in] StrapData          Strap Set Data
**/
VOID
PmcSetStrapMsgData (
  IN UINT16           StrapData
  );

/**
  This function read 16-bit PMC Set Strap Message interface data.

  @retval 16-bit Strap Message Data
**/
UINT16
PmcGetStrapMsgData (
  VOID
  );

/**
  This function locks PMC Set Strap Message interface
**/
VOID
PmcLockSetStrapMsgInterface (
  VOID
  );

/**
  This function clears RTC Power Failure status (RTC_PWR_FLR)
**/
VOID
PmcClearRtcPowerFailureStatus (
  VOID
  );

/**
  This function will clear the DISB - DRAM Initialization Scratchpad Bit.
**/
VOID
PmcClearDramInitScratchpad (
  VOID
  );

/**
  This function enables PCI Express* PME events

  @param[in] PmcMmio       PMC MMIO

**/
VOID
PmcEnablePciExpressPmeEvents (
  IN UINTN  PmcMmio
  );

/**
  This function sets SLP_SX Stretching Policy and adds
  lock setting to S3 Boot Script

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcLockSlpSxStretchingPolicyWithS3BootScript (
  IN UINTN  PchPwrmBase
  );

/**
  This function sets SMI Lock with S3 Boot Script programming

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcLockSmiWithS3BootScript (
  IN UINTN  PchPwrmBase
  );

/**
  This function sets eSPI SMI Lock
  @attention This function must be called after eSPI SMI generation has been enabled.
    This setting is required in all boot modes and before EndOfDxe.
    If set value will be restored upon S3 resume by bootscript.
**/
VOID
PmcLockEspiSmiWithS3BootScript (
  VOID
  );

/**
  This function checks if eSPI SMI Lock is set

  @retval eSPI SMI Lock state
**/
BOOLEAN
PmcIsEspiSmiLockSet (
  VOID
  );

typedef enum {
  PmcSwSmiRate1p5ms = 0,
  PmcSwSmiRate16ms,
  PmcSwSmiRate32ms,
  PmcSwSmiRate64ms
} PMC_SWSMI_RATE;

/**
  This function sets SW SMI Rate.

  @param[in] SwSmiRate        Refer to PMC_SWSMI_RATE for possible values
**/
VOID
PmcSetSwSmiRate (
  IN PMC_SWSMI_RATE          SwSmiRate
  );

typedef enum {
  PmcPeriodicSmiRate8s = 0,
  PmcPeriodicSmiRate16s,
  PmcPeriodicSmiRate32s,
  PmcPeriodicSmiRate64s
} PMC_PERIODIC_SMI_RATE;

/**
  This function sets Periodic SMI Rate.

  @param[in] PeriodicSmiRate        Refer to PMC_PERIODIC_SMI_RATE for possible values
**/
VOID
PmcSetPeriodicSmiRate (
  IN PMC_PERIODIC_SMI_RATE    PeriodicSmiRate
  );

/**
  This function reads Power Button Level

  @retval State of PWRBTN# signal (0: Low, 1: High)
**/
UINT8
PmcGetPwrBtnLevel (
  VOID
  );

/**
  This function checks if memory is placed in self-refresh

  @retval Memory Placed In Self-Refresh state
**/
BOOLEAN
PmcIsMemoryPlacedInSelfRefreshState (
  VOID
  );

/**
  This function gets Group to GPE0 configuration

  @param[out] GpeDw0Value       GPIO Group to GPE_DW0 assignment
  @param[out] GpeDw1Value       GPIO Group to GPE_DW1 assignment
  @param[out] GpeDw2Value       GPIO Group to GPE_DW2 assignment
**/
VOID
PmcGetGpioGpe (
  OUT UINT32    *GpeDw0Value,
  OUT UINT32    *GpeDw1Value,
  OUT UINT32    *GpeDw2Value
  );

/**
  This function checks if SCI interrupt is enabled

  @retval SCI Enable state
**/
BOOLEAN
PmcIsSciEnabled (
  VOID
  );

/**
  This function triggers Software GPE
**/
VOID
PmcTriggerSwGpe (
  VOID
  );

//
// Possible SoundWire RCOMP configuration
// Below values can be OR'ed
//
typedef UINT32 PMC_SNDW_RCOMP_BUFFER_CONFIG;
#define PMC_SNDW_RCOMP_SNDW1_ENABLE  BIT0
#define PMC_SNDW_RCOMP_SNDW2_ENABLE  BIT1
#define PMC_SNDW_RCOMP_ACT_TOPOLOGY  BIT2

/**
  Disable ME devices. This function can be called only once during
  boot. Calling this function with DisableMe = FALSE will only tell PMC
  that we wish to keep ME enabled and to disallow any further attempts at disabling ME.

  @param[in] PmcMmio        PMC MMIO
  @param[in] DisableMe      TRUE if this function should disable ME.
**/
VOID
PmcDisableMeDevices (
  IN UINTN    PmcMmio,
  IN BOOLEAN  DisableMe
  );

/**
  Check if CRID mode is supported.

  @param[in] PmcPciCfgBase    PMC PCI config base address
  @param[in]  PchPwrmBase     PMC base address
  @param[out] CridSupported   Return TRUE if Crid mode is supported.
  @param[out] OrgRid          Return original rid.
  @param[out] NewRid          Return new rid.

  @retval  EFI_SUCCESS        System can detect the CRID mode.
  @retval  EFI_UNSUPPORTED    System cannot detect the CRID mode.
**/
EFI_STATUS
PmcDetectCrid0 (
  UINT64  PmcPciCfgBase,
  UINTN   PchPwrmBase,
  BOOLEAN *CridSupported,
  UINT8   *OrgRid,
  UINT8   *NewRid
  );

/**
  Select CRID0 mode.
  The silicon RID will be changed. Refer to silicon spec for RID value of CRID0.

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcSetCrid0WithS3BootScript (
  IN UINTN  PchPwrmBase
  );

/**
  Lock CRID.

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcLockCridWithS3BootScript (
  IN UINTN  PchPwrmBase
  );

/**
  This function disables SA SSC
**/
VOID
PmcDisableSaSsc(
  VOID
  );

/**
  This function disables TraceHub by enabling power gating

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcDisableTraceHub (
  IN UINTN  PmcMmio
  );

/**
  This function enables TraceHub by disabling power gating

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcEnableTraceHub (
  IN UINTN  PmcMmio
  );

/**
  This function forces ACRO on during Sx that allows TraceHub to request for clock and use it for Sx debug

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcEnableTraceHubDebugForSx (
  IN UINTN  PmcMmio
  );

/**
  This function removes force ACRO on during Sx that disallows TraceHub to request for clock and use it for Sx debug

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcDisableTraceHubDebugForSx (
  IN UINTN  PmcMmio
  );

/**
  This function enables USB3DBC disconnect in Sx

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcEnableUsb3dbcDisconnectForSx (
  IN UINTN  PmcMmio
  );

/**
  This function disables USB3DBC disconnect in Sx

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcDisableUsb3dbcDisconnectForSx (
  IN UINTN  PmcMmio
  );

/**
  This function prints global reset reason.
**/
VOID
PmcPrintResetReason (
  VOID
  );

/**
  This function sends PMC command to enable I/O Margining tool support

  @param[in] PchPwrmBase  PWRM base address.
**/
VOID
PmcEnableIoMargining (
  IN UINTN PchPwrmBase
  );

/**
  Send IPC Memory Calibration Done notification

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcSendBiosResetCompletionMemCalDone (
  IN UINTN PwrmBase
  );

/**
  Send IPC Pcie Enumeration done notification

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcSendBiosResetPciEnumDone (
  IN UINTN PwrmBase
  );

/**
  Send IPC BIOS POST Done notification to trigger PMC SBX WA.
  BIOS has completed POST and sets the BIOS_RST_CPL_STS_POST_DONE flag.

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcSendBiosPostDoneSbxWa (
  VOID
  );

/**
  Send IPC Start Time Sync notification

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcSendStartTimeSync (
  IN UINTN PwrmBase
  );

/**
  This function checks if cpu epoc is locked

  @retval cpu epoc locked state
**/
BOOLEAN
PmcIsCpuEpocLocked (
  VOID
  );

/**
  PMC scratch pad register (offset = 0x1954) is written by pCode before system reset.
  This function reads SaGv and FspOp self-refresh entry values from PMC scratch pad register.

  @param[out] GvPoint  Self-Refresh Entry value from PMC register on Warm / S3 flow for SAGV

  @retval Returns none
**/
VOID
PmcReadSaGvPointBeforeReset (
  OUT UINT8 *GvPoint
  );
/**
  This function disables PMC Watch dog timer
  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcDisableWdt (
  IN UINTN  PmcMmio
  );

/**
  This function enables PMC Watch dog timer
  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcEnableWdt (
  IN UINTN  PmcMmio
  );

#endif // _PMC_PRIVATE_LIB_H_
