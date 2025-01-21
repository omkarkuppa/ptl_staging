/** @file
  PCH private PMC Library for all PCH generations.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Library/PmcPrivateLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PmcSocLib.h>

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
  )
{
  EFI_STATUS             Status;
  UINT32                 IpcSts;
  UINTN                  Timeout;

  //This function among other is used during reading debug log level.
  //When command to access NMI control register command is detected all debug logging MUST be skipped to avoid dead loop here.
  if (WriteBufPtr->Buf0 != V_PMC_PWRM_IPC_CMD_WBUF0_PROXY_NMI) {
    DEBUG ((DEBUG_INFO, "PmcSendCommand(): IPC_COMMAND=0x%02X, IPC_SUB_CMD = 0x%02X, IPC_SIZE=0x%02X \n", Command, SubCmdId, CmdSize));
    DEBUG ((DEBUG_INFO, "WBUF0=0x%08X, WBUF1=0x%08X, WBUF2=0x%08X, WBUF3=0x%08X \n", WriteBufPtr->Buf0, WriteBufPtr->Buf1, WriteBufPtr->Buf2, WriteBufPtr->Buf3));
  }
  if (CmdSize > 16) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Program the Write Buffer 0 with the Data that needs to be written to PMC
  //
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_IPC_WBUF0), WriteBufPtr->Buf0);
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_IPC_WBUF1), WriteBufPtr->Buf1);
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_IPC_WBUF2), WriteBufPtr->Buf2);
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_IPC_WBUF3), WriteBufPtr->Buf3);
  //
  // Program the command register with command and size
  //
  MmioWrite32 (
    PchPwrmBase + R_PMC_PWRM_IPC_CMD,
    (UINT32) ((CmdSize << N_PMC_PWRM_IPC_CMD_SIZE) |
      (SubCmdId << N_PMC_PWRM_IPC_CMD_CMD_ID) |
      (Command << N_PMC_PWRM_IPC_CMD_COMMAND))
    );

  //
  // Read the IPC_STS register to get BUSY or Error status
  //
  Timeout = 0;
  Status = EFI_SUCCESS;
  while (TRUE) {
    IpcSts = MmioRead32 (PchPwrmBase + R_PMC_PWRM_IPC_STS);
    if ((IpcSts & B_PMC_PWRM_IPC_STS_BUSY) == 0) {
      Status = EFI_SUCCESS;
      break;
    }

    if (Timeout > (1000 * 100)) {
      Status = EFI_TIMEOUT;
      break;
    }
    MicroSecondDelay (10);
    Timeout++;
  }

  if ((IpcSts & B_PMC_PWRM_IPC_STS_ERROR) != 0) {
    Status = EFI_DEVICE_ERROR;
  }

  if (EFI_ERROR (Status)) {
    if (WriteBufPtr->Buf0 != V_PMC_PWRM_IPC_CMD_WBUF0_PROXY_NMI) {
      DEBUG ((DEBUG_ERROR, "PmcSendCommand() Error: IPC_STS=0x%08X\n", IpcSts));
    }
    return Status;
  }

  if (ReadBufPtr != NULL) {
    //
    // Fill the  ReadBuffer contents with the Data that needs to be read from PMC
    //
    ReadBufPtr->Buf0 = MmioRead32(PchPwrmBase + R_PMC_PWRM_IPC_RBUF0);
    ReadBufPtr->Buf1 = MmioRead32(PchPwrmBase + R_PMC_PWRM_IPC_RBUF1);
    ReadBufPtr->Buf2 = MmioRead32(PchPwrmBase + R_PMC_PWRM_IPC_RBUF2);
    ReadBufPtr->Buf3 = MmioRead32(PchPwrmBase + R_PMC_PWRM_IPC_RBUF3);
    if (WriteBufPtr->Buf0 != V_PMC_PWRM_IPC_CMD_WBUF0_PROXY_NMI) {
      DEBUG ((DEBUG_INFO, "RBUF0=0x%08X, RBUF1=0x%08X, RBUF2=0x%08X, RBUF3=0x%08X \n", ReadBufPtr->Buf0, ReadBufPtr->Buf1, ReadBufPtr->Buf2, ReadBufPtr->Buf3));
    }
  }

  return Status;
}

/**
  This function checks if SCI interrupt is enabled

  @retval SCI Enable state
**/
BOOLEAN
PmcIsSciEnabled (
  VOID
  )
{
  return ((IoRead8 (PmcGetAcpiBase () + R_ACPI_IO_PM1_CNT) & B_ACPI_IO_PM1_CNT_SCI_EN) != 0);
}

/**
  This function triggers Software GPE
**/
VOID
PmcTriggerSwGpe (
  VOID
  )
{
  IoOr32 (PmcGetAcpiBase () + R_ACPI_IO_GPE_CTRL, B_ACPI_IO_GPE_CTRL_SWGPE_CTRL);
}

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
  )
{
  if (PciSegmentRead16 (PmcPciCfgBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Enable IO space and Bus Master in PMC device
  //
  PciSegmentOr16 (
    PmcPciCfgBase + PCI_COMMAND_OFFSET,
    (UINT16) (EFI_PCI_COMMAND_IO_SPACE | EFI_PCI_COMMAND_BUS_MASTER)
    );
  return EFI_SUCCESS;
}

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
  )
{
  if (Address != PcdGet16 (PcdAcpiBaseAddress)) {
    DEBUG ((DEBUG_ERROR, "PmcSetAcpiBase Error. Invalid Address: %x.\n", Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (PciSegmentRead16 (PmcPciCfgBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Disable IO BAR in PMC Device first before changing the ACPI base address.
  //
  PciSegmentAnd16 (
    PmcPciCfgBase + PCI_COMMAND_OFFSET,
    (UINT16) ~EFI_PCI_COMMAND_IO_SPACE
    );

  //
  // Program PMC BAR1 Device- ACPI base address
  //
  PciSegmentAndThenOr32 (
    PmcPciCfgBase + (PCI_BASE_ADDRESSREG_OFFSET * PCI_BAR_IDX2),
    (UINT32)(~0xFFFF),
    Address
    );


  //
  // Enable IO BAR in PMC Device
  //
  PciSegmentOr16 (
    PmcPciCfgBase + PCI_COMMAND_OFFSET,
    EFI_PCI_COMMAND_IO_SPACE
    );

  return EFI_SUCCESS;
}

/**
  Set PCH PWRM base address.

  @param[in] PmcBase                    PMC PCI base address
  @param[in] Address                    Address for PWRM base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Wrong address passed.
**/
EFI_STATUS
PmcSetPwrmBase (
  IN  UINT64                            PmcBase,
  IN  UINT64                            Address
  )
{
  if (Address == 0) {
    DEBUG ((DEBUG_ERROR, "PmcSetPwrmBase Error. Null Address.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (PciSegmentRead16 (PmcBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Disable PWRMBASE in PMC Device first before changing PWRM base address.
  //
  PciSegmentAnd16 (
    PmcBase + PCI_COMMAND_OFFSET,
    (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE
    );

  //
  // Program PWRMBASE in PMC Device
  //
  PciSegmentAndThenOr32 (
    PmcBase + R_PMC_CFG_BAR,
    (UINT32) (~B_PMC_CFG_PWRM_BASE_MASK),
    (UINT32) Address
    );

  //
  // Program upper 32 bits of PWRMBASE in PMC Device
  //
  if (PciSegmentRead32 (PmcBase + R_PMC_CFG_BAR) & BIT2) {
    PciSegmentWrite32 (
      PmcBase + R_PMC_CFG_BAR + 4,
      (UINT32) RShiftU64 (Address, 32)
      );
  }

  //
  // Enable PWRMBASE in PMC Device
  //
  PciSegmentOr16 (
    PmcBase + PCI_COMMAND_OFFSET,
    EFI_PCI_COMMAND_MEMORY_SPACE
    );
  return EFI_SUCCESS;
}

/**
  Get Virtual GPIO (vGPIO) base address.

  @param[out] VgpioBar                  Vitrual GPIO base address

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Wrong address passed.
**/
EFI_STATUS
PmcGetVgpioBar (
  OUT  UINTN   *VgpioBar
  )
{
  UINT32    Offset;

  Offset = MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_VGPIO_BAR);

  // vGPIO BAR (PWRM offset 0x2000) stores the vGPIO offset in PWRM space
  *VgpioBar = PmcGetPwrmBase () + Offset;
  if (*VgpioBar == 0) {
    return EFI_NOT_READY;
  } else {
    return EFI_SUCCESS;
  }
}

/**
  Enable IBL's Virtual GPIO (vGPIO) feature by
  configuring eSPI interface mode to "eSPI GPIO expander Virtual Wire"

**/
VOID
PmcEnableVgpioEspiVirtualWire (
  VOID
  )
{
  MmioOr8 ((PmcGetPwrmBase () + R_PMC_PWRM_VGPIO_CFG), B_PMC_PWRM_VGPIO_CFG_VGPIO_eSPI_MODE);
}

/**
  This function locks HOST SW power gating control

  @param[in] PmcMmio       PMC MMIO
**/
VOID
PmcLockHostSwPgCtrl (
  UINTN PmcMmio
  )
{
  MmioOr32 (PmcMmio + R_PMC_PWRM_HSWPGCR1, B_PMC_PWRM_HSWPGCR1_SW_PG_CTRL_LOCK);
}

/**
  This function checks if HOST SW Power Gating Control is locked

  @retval lock state
**/
BOOLEAN
PmcIsHostSwPgCtrlLocked (
  VOID
  )
{
  //
  // Get lock info from PWRMBASE + HSWPGCR1
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_HSWPGCR1) & B_PMC_PWRM_HSWPGCR1_SW_PG_CTRL_LOCK) != 0);
}

/**
  This function checks if LAN wake from DeepSx is enabled

  @retval Lan Wake state
**/
BOOLEAN
PmcIsLanDeepSxWakeEnabled (
  VOID
  )
{
  //
  // Get wake info from PWRMBASE + DSX_CFG
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_DSX_CFG) & (UINT32) B_PMC_PWRM_DSX_CFG_LANWAKE_PIN_DSX_EN) != 0);
}

/**
  Disables USB2 Core PHY PowerGating during power on for Chipsetinit table update

  @param[in] Address                    Address for PWRM base address.
**/
VOID
PmcUsb2CorePhyPowerGatingDisable (
  IN UINTN  PmcMmio
  )
{
  MmioAnd32 (PmcMmio + R_PMC_PWRM_PM_CFG, (UINT32) ~B_PMC_PWRM_PM_CFG_ALLOW_USB2_CORE_PG);
}

/**
  This function reads CPU Early Power-on Configuration (EPOC)

  @retval CPU EPOC value
**/
UINT32
PmcGetCpuEpoc (
  VOID
  )
{
  return MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_CPU_EPOC);
}

/**
  This function sets CPU Early Power-on Configuration (EPOC)

  @param[in] CpuEpocValue      CPU EPOC value
**/
VOID
PmcSetCpuEpoc (
  IN UINT32     CpuEpocValue
  )
{
  MmioWrite32 (PmcGetPwrmBase () + R_PMC_PWRM_CPU_EPOC, CpuEpocValue);
}

/**
  This function reads CPU Early Power-on Configuration 2 (EPOC2)

  @retval CPU EPOC2 value
**/
UINT32
PmcGetCpuEpoc2 (
  VOID
  )
{
  return MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_CPU_EPOC2);
}

/**
  This function sets CPU Early Power-on Configuration 2 (EPOC2)

  @param[in] CpuEpoc2Value      CPU EPOC2 value
**/
VOID
PmcSetCpuEpoc2 (
  IN UINT32     CpuEpoc2Value
  )
{
  MmioWrite32 (PmcGetPwrmBase () + R_PMC_PWRM_CPU_EPOC2, CpuEpoc2Value);
}

/**
  This function sets DRAM_RESET# Control Pin value

  @param[in] DramResetVal      0: Pin output is low
                               1: Pin output is tri-stated
**/
VOID
PmcSetDramResetCtlState (
  IN UINT32     DramResetVal
  )
{
  ASSERT (DramResetVal < 2);

  MmioAndThenOr32 (
    PmcGetPwrmBase () + R_PMC_PWRM_PM_CFG2,
    (UINT32)~B_PMC_PWRM_PM_CFG2_DRAM_RESET_CTL,
    DramResetVal << N_PMC_PWRM_PM_CFG2_DRAM_RESET_CTL
    );
}

/**
  This function enables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh
**/
VOID
PmcEnableCf9GlobalReset (
  VOID
  )
{
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_ETR3, (UINT32) B_PMC_PWRM_ETR3_CF9GR);
}

/**
  This function enables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh
  Global Reset configuration is locked after programming
**/
VOID
PmcEnableCf9GlobalResetWithLock (
  VOID
  )
{
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_ETR3, (UINT32) (B_PMC_PWRM_ETR3_CF9GR | B_PMC_PWRM_ETR3_CF9LOCK));
}

/**
  This function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.

  @param[in]         PchPwrmBase  Address of PWRM base address
**/
VOID
PmcDisableCf9GlobalReset (
  IN UINTN          PchPwrmBase
  )
{
  ASSERT (PchPwrmBase != 0);
  MmioAnd32 (PchPwrmBase + R_PMC_PWRM_ETR3, (UINT32) ~B_PMC_PWRM_ETR3_CF9GR);
}

/**
  This function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
  Global Reset configuration is locked after programming
**/
VOID
PmcDisableCf9GlobalResetWithLock (
  IN UINTN          PchPwrmBase
  )
{
  ASSERT (PchPwrmBase != 0);
  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_ETR3,
    (UINT32) ~B_PMC_PWRM_ETR3_CF9GR,
    (UINT32) B_PMC_PWRM_ETR3_CF9LOCK
    );
}

/**
  This function disables CF9 reset without Resume Well reset.
  Cf9 0x6/0xE reset will also reset resume well logic.
**/
VOID
PmcDisableCf9ResetWithoutResumeWell (
  VOID
  )
{

  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ETR3, (UINT32) ~B_PMC_PWRM_ETR3_CWORWRE);
}

/**
  This function checks PMC Set Strap Message interface

  @retval State of PMC Set Strap Message Interface
**/
BOOLEAN
PmcIsSetStrapMsgInterfaceEnabled (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_SSMC) & B_PMC_PWRM_SSMC_SSMS) != 0);
}

/**
  This function sets 16-bit PMC Set Strap Message interface data.

  @param[in] StrapData          Strap Set Data
**/
VOID
PmcSetStrapMsgData (
  IN UINT16           StrapData
  )
{
  UINTN  PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_SSMD, (UINT32)StrapData);
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_SSMC, B_PMC_PWRM_SSMC_SSMS);
}

/**
  This function read 16-bit PMC Set Strap Message interface data.

  @retval 16-bit Strap Message Data
**/
UINT16
PmcGetStrapMsgData (
  VOID
  )
{
  return MmioRead16 (PmcGetPwrmBase () + R_PMC_PWRM_SSMD);
}

/**
  This function locks PMC Set Strap Message interface
**/
VOID
PmcLockSetStrapMsgInterface (
  VOID
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  if (PmcIsCpuStrapsIpcCommandSupported ()) {
    Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_CPU_STRAP, V_PMC_PWRM_IPC_CMD_CMD_ID_CPU_STRAP_LOCK, 4, &Wbuf, NULL);
    ASSERT_EFI_ERROR (Status);
  } else {
    MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_SSML, B_PMC_PWRM_SSML_SSL);
  }
}

/**
  This function clears RTC Power Failure status (RTC_PWR_FLR)
**/
VOID
PmcClearRtcPowerFailureStatus (
  VOID
  )
{
  //
  // Set B_PMC_PWRM_GEN_PMCON_B_RPS to 0 to clear it.
  //
  MmioAnd8 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B, (UINT8) (~B_PMC_PWRM_GEN_PMCON_B_RPS));
}

/**
  This function will clear the DISB - DRAM Initialization Scratchpad Bit.
**/
VOID
PmcClearDramInitScratchpad (
  VOID
  )
{
  //
  // Clear B_PMC_PWRM_GEN_PMCON_A_DISB.
  // NOTE: Byte access to not clear BIT18 and BIT16
  //
  MmioAnd8 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 2, (UINT8) ~((B_PMC_PWRM_GEN_PMCON_A_DISB | B_PMC_PWRM_GEN_PMCON_A_MS4V | B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR) >> 16));
}

/**
  This function enables PCI Express* PME events

  @param[in] PmcMmio       PMC MMIO

**/
VOID
PmcEnablePciExpressPmeEvents (
  IN UINTN  PmcMmio
  )
{
  MmioOr32 (PmcMmio + R_PMC_PWRM_GEN_PMCON_B, B_PMC_PWRM_GEN_PMCON_B_BIOS_PCI_EXP_EN);
}

/**
  This function checks if eSPI SMI Lock is set

  @retval eSPI SMI Lock state
**/
BOOLEAN
PmcIsEspiSmiLockSet (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A) & B_PMC_PWRM_GEN_PMCON_A_ESPI_SMI_LOCK) != 0);
}

/**
  This function sets SW SMI Rate.

  @param[in] SwSmiRate        Refer to PMC_SWSMI_RATE for possible values
**/
VOID
PmcSetSwSmiRate (
  IN PMC_SWSMI_RATE          SwSmiRate
  )
{
  UINTN         PchPwrmBase;
  STATIC UINT8  SwSmiRateRegVal[4] = {
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_1_5MS,
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_16MS,
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_32MS,
    V_PMC_PWRM_GEN_PMCON_A_SWSMI_RTSL_64MS
  };

  ASSERT (SwSmiRate <= PmcSwSmiRate64ms);

  PchPwrmBase = PmcGetPwrmBase ();

  //
  // SWSMI_RATE_SEL BIT (PWRMBASE offset 1020h[7:6]) bits are in RTC well
  //
  MmioAndThenOr8 (
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A,
    (UINT8)~B_PMC_PWRM_GEN_PMCON_A_SWSMI_RATESEL,
    SwSmiRateRegVal[SwSmiRate]
    );
}

/**
  This function sets Periodic SMI Rate.

  @param[in] PeriodicSmiRate        Refer to PMC_PERIODIC_SMI_RATE for possible values
**/
VOID
PmcSetPeriodicSmiRate (
  IN PMC_PERIODIC_SMI_RATE    PeriodicSmiRate
  )
{
  UINTN         PchPwrmBase;
  STATIC UINT8  PeriodicSmiRateRegVal[4] = {
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_8S,
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_16S,
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_32S,
    V_PMC_PWRM_GEN_PMCON_A_PER_SMI_64S
  };

  ASSERT (PeriodicSmiRate <= PmcPeriodicSmiRate64s);

  PchPwrmBase = PmcGetPwrmBase ();

  MmioAndThenOr8 (
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A,
    (UINT8)~B_PMC_PWRM_GEN_PMCON_A_PER_SMI_SEL,
    PeriodicSmiRateRegVal[PeriodicSmiRate]
    );
}

/**
  This function reads Power Button Level

  @retval State of PWRBTN# signal (0: Low, 1: High)
**/
UINT8
PmcGetPwrBtnLevel (
  VOID
  )
{
  if (MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B) & B_PMC_PWRM_GEN_PMCON_B_PWRBTN_LVL) {
    return 1;
  } else {
    return 0;
  }
}

/**
  This function checks if memory is placed in self-refresh

  @retval Memory Placed In Self-Refresh state
**/
BOOLEAN
PmcIsMemoryPlacedInSelfRefreshState (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A) & B_PMC_PWRM_GEN_PMCON_A_MEM_SR) != 0);
}

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
  )
{
  UINT32 Data32;

  Data32 = MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GPIO_CFG);

  *GpeDw0Value = ((Data32 & B_PMC_PWRM_GPIO_CFG_GPE0_DW0) >> N_PMC_PWRM_GPIO_CFG_GPE0_DW0);
  *GpeDw1Value = ((Data32 & B_PMC_PWRM_GPIO_CFG_GPE0_DW1) >> N_PMC_PWRM_GPIO_CFG_GPE0_DW1);
  *GpeDw2Value = ((Data32 & B_PMC_PWRM_GPIO_CFG_GPE0_DW2) >> N_PMC_PWRM_GPIO_CFG_GPE0_DW2);
}

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
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = ModifyClocksMask;
  Wbuf.Buf1 = EnabledClocksMask;
  Wbuf.Buf2 = ModifyPortClkreqMask;
  Wbuf.Buf3 = EnabledPortClkreqMask;

  Status = PmcSendCommand (PchPwrmBase, V_PMC_PWRM_IPC_SRC_CLK_PORT_MAPPING_CMD, 0, 8, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}

#define PMC_CRID_UIP_TIMEOUT  0x1000000
/**
  Wait for CRID update ready.
**/
STATIC
VOID
PmcWaitForCridUip (
  IN UINTN   PchPwrmBase
  )
{
  volatile UINT32  Timeout;

  Timeout     = 0;
  while (MmioRead32 (PchPwrmBase + R_PMC_PWRM_CRID_UIP) & B_PMC_PWRM_CRID_UIP_CRID_UIP) {
    if (Timeout++ > PMC_CRID_UIP_TIMEOUT) {
      DEBUG ((DEBUG_WARN, "%a : ERROR PMC Wait for CRID update Timeout. PMC PWRM CRID - %x\n",__FUNCTION__,MmioRead32 (PchPwrmBase + R_PMC_PWRM_CRID_UIP)));
      break;
    }
  }
}

/**
  This function disables SA SSC
**/
VOID
PmcDisableSaSsc(
  VOID
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem(&Wbuf, sizeof(PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = BIT0;
  Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_COMMAND_SSC_STATE, 0, 4, &Wbuf, NULL);
  ASSERT_EFI_ERROR(Status);
}

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
  )
{
  UINT32  Crid;

  if (PciSegmentRead16 (PmcPciCfgBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Crid = MmioRead32 (PchPwrmBase + R_PMC_PWRM_CRID);
  //
  // If Crid is locked or RID_SEL is changed then skip the detection.
  //
  if (((Crid & B_PMC_PWRM_CRID_CRID_LK) != 0) ||
      ((Crid & B_PMC_PWRM_CRID_RID_SEL) != 0))
  {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  *OrgRid = PciSegmentRead8 (PmcPciCfgBase + PCI_REVISION_ID_OFFSET);
  PmcWaitForCridUip (PchPwrmBase);
  MmioWrite8 (PchPwrmBase + R_PMC_PWRM_CRID, V_PMC_PWRM_CRID_RID_SEL_CRID0);
  PmcWaitForCridUip (PchPwrmBase);
  *NewRid = PciSegmentRead8 (PmcPciCfgBase + PCI_REVISION_ID_OFFSET);
  MmioWrite8 (PchPwrmBase + R_PMC_PWRM_CRID, V_PMC_PWRM_CRID_RID_SEL_REVISIONID);
  PmcWaitForCridUip (PchPwrmBase);

  *CridSupported = (*OrgRid != *NewRid);
  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Status = PmcSendCommand (PmcMmio, V_PMC_PWRM_IPC_CMD_COMMAND_ME_DISABLE, DisableMe, 0, &Wbuf, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR PmcSendCommand Status = %r\n", __FUNCTION__, Status));
  }
}

/**
  This function disables TraceHub by enabling power gating

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcDisableTraceHub (
  IN UINTN  PmcMmio
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = BIT0;
  Status = PmcSendCommand (PmcMmio, V_PMC_PWRM_IPC_CMD_COMMAND_DFX_PWR_STATE, 0, 4, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function enables TraceHub by disabling power gating

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcEnableTraceHub (
  IN UINTN  PmcMmio
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = BIT1;
  Status = PmcSendCommand (PmcMmio, V_PMC_PWRM_IPC_CMD_COMMAND_DFX_PWR_STATE, 0, 4, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function forces ACRO on during Sx that allows TraceHub to request for clock and use it for Sx debug

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcEnableTraceHubDebugForSx (
  IN UINTN  PmcMmio
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = BIT2;
  Status = PmcSendCommand (PmcMmio, V_PMC_PWRM_IPC_CMD_COMMAND_DFX_PWR_STATE, 0, 4, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function removes force ACRO on during Sx that disallows TraceHub to request for clock and use it for Sx debug

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcDisableTraceHubDebugForSx (
  IN UINTN  PmcMmio
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = BIT3;
  Status = PmcSendCommand (PmcMmio, V_PMC_PWRM_IPC_CMD_COMMAND_DFX_PWR_STATE, 0, 4, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function enables USB3DBC disconnect in Sx

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcEnableUsb3dbcDisconnectForSx (
  IN UINTN  PmcMmio
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = BIT11;
  Status = PmcSendCommand (PmcMmio, V_PMC_PWRM_IPC_CMD_COMMAND_DFX_PWR_STATE, 0, 4, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function disables USB3DBC disconnect in Sx

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcDisableUsb3dbcDisconnectForSx (
  IN UINTN  PmcMmio
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = BIT12;
  Status = PmcSendCommand (PmcMmio, V_PMC_PWRM_IPC_CMD_COMMAND_DFX_PWR_STATE, 0, 4, &Wbuf, NULL);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function prints global reset reason.
**/
VOID
PmcPrintResetReason (
  VOID
  )
{
  UINTN PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();
  DEBUG ((DEBUG_INFO, "GBLRST_CAUSE0: 0x%X\n", MmioRead32 (PchPwrmBase + R_PMC_PWRM_GBLRST_CAUSE0)));
  DEBUG ((DEBUG_INFO, "GBLRST_CAUSE1: 0x%X\n", MmioRead32 (PchPwrmBase + R_PMC_PWRM_GBLRST_CAUSE1)));
  DEBUG ((DEBUG_INFO, "HPR_CAUSE0   : 0x%X\n", MmioRead32 (PchPwrmBase + R_PMC_PWRM_HPR_CAUSE0)));
}

/**
  This function sends PMC command to enable I/O Margining tool support

  @param[in] PchPwrmBase  PWRM base address.
**/
VOID
PmcEnableIoMargining (
  IN UINTN PchPwrmBase
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Status = PmcSendCommand (
    PchPwrmBase,
    V_PMC_PWRM_IPC_CMD_COMMAND_GENERIC_WORKAROUND,
    V_PMC_PWRM_IPC_CMD_SUBCMD_IO_MARGINING_ENABLE,
    4,
    &Wbuf,
    NULL);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR: PMC did not support enabling I/O Margining tool, Status: %r \n", Status));
  }
}


/**
  Send IPC Memory Calibration Done notification

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcSendBiosResetCompletionMemCalDone (
  IN UINTN PwrmBase
  )
{
  PMC_IPC_COMMAND_BUFFER  Wbuf;
  EFI_STATUS              Status;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = V_PMC_PWRM_IPC_CMD_CMD_BIOS_RST_CMPL_STS_MEM_CAL;
  Status = PmcSendCommand (PwrmBase, V_PMC_PWRM_IPC_CMD_CMD_BIOS_RST_CMPL_STS, 0, 1, &Wbuf, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR PmcCommand Status = %r\n", __FUNCTION__, Status));
  }
  return Status;
}

/**
  Send IPC Pcie Enumeration done notification

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcSendBiosResetPciEnumDone (
  IN UINTN PwrmBase
  )
{
  PMC_IPC_COMMAND_BUFFER  Wbuf;
  EFI_STATUS              Status;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = V_PMC_PWRM_IPC_CMD_CMD_BIOS_RST_CMPL_STS_PCI_ENUM;
  Status = PmcSendCommand (PwrmBase, V_PMC_PWRM_IPC_CMD_CMD_BIOS_RST_CMPL_STS, 0, 1, &Wbuf, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR PmcCommand Status = %r\n", __FUNCTION__, Status));
  }
  return Status;
}

/**
  Send IPC BIOS POST Done notification to trigger PMC SBX WA.
  BIOS has completed POST and sets the BIOS_RST_CPL_STS_POST_DONE flag.]

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcSendBiosPostDoneSbxWa (
  VOID
  )
{
  PMC_IPC_COMMAND_BUFFER  Wbuf;
  EFI_STATUS              Status;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = 0b100;
  Status = PmcSendCommand (PmcGetPwrmBase (), V_PMC_PWRM_IPC_CMD_CMD_BIOS_RST_CMPL_STS, 0, 1, &Wbuf, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR PmcSendCommand BIOS POST Done Status =%r\n", __FUNCTION__, Status));
  }
  DEBUG ((DEBUG_INFO, "%a: PmcSendCommand sent BIOS POST Done\n", __FUNCTION__));

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
  return Status;
}

/**
  Send IPC Start Time Sync notification

  @retval Result of the IPC command send
**/
EFI_STATUS
PmcSendStartTimeSync (
  IN UINTN PwrmBase
  )
{
  PMC_IPC_COMMAND_BUFFER  Wbuf;
  EFI_STATUS              Status;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = V_PMC_PWRM_IPC_CMD_CMD_GENERIC_WORKAROUND_TIME_SYNC_CONTROL;
  Wbuf.Buf1 = V_PMC_PWRM_IPC_CMD_CMD_GENERIC_WORKAROUND_TIME_SYNC_ENABLE;
  Status = PmcSendCommand (PwrmBase, V_PMC_PWRM_IPC_CMD_COMMAND_GENERIC_WORKAROUND, 0, 1, &Wbuf, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR PmcCommand Status = %r\n", __FUNCTION__, Status));
  }
  return Status;
}

/**
  This function checks if cpu epoc is locked

  @retval cpu epoc locked state
**/
BOOLEAN
PmcIsCpuEpocLocked (
  VOID
  )
{
  //
  // Check if R_PMC_PWRM_GEN_PMCON_B[19] cpu epoc bit is locked or not
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B) & B_PMC_PWRM_GEN_PMCON_B_CPU_EPOC_LOCK) != 0);
}

/**
  PMC scratch pad register (offset = 0x1954) is written by pCode before system reset.
  This function reads SaGv and FspOp self-refresh entry values from PMC scratch pad register.

  @param[out] GvPoint  Self-Refresh Entry value from PMC register on Warm / S3 flow for SAGV

  @retval Returns none
**/
VOID
PmcReadSaGvPointBeforeReset (
  OUT UINT8 *GvPoint
  )
{
  DSW_SPARE_GCR_0_STRUCT DswSpareGcr0;

  DswSpareGcr0.Data = MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_DSW_SPARE_GCR_0);
  *GvPoint = (UINT8) DswSpareGcr0.Bits.QclkGvLastPoint;
}

/**
  This function disables PMC Watch dog timer
  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcDisableWdt (
  IN UINTN  PmcMmio
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = 0;
  Wbuf.Buf1 = BIT0;
  Status = PmcSendCommand (PmcMmio, V_PMC_PWRM_IPC_CMD_COMMAND_DFX_OVERRIDES, 0, 4, &Wbuf, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "%a : PmcSendCommand() Status = %r \n",  __FUNCTION__, Status));
  }
}

/**
  This function enables PMC Watch dog timer
  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcEnableWdt (
  IN UINTN  PmcMmio
  )
{
  EFI_STATUS                Status;
  PMC_IPC_COMMAND_BUFFER    Wbuf;

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  Wbuf.Buf0 = BIT0;
  Wbuf.Buf1 = BIT0;
  Status = PmcSendCommand (PmcMmio, V_PMC_PWRM_IPC_CMD_COMMAND_DFX_OVERRIDES, 0, 4, &Wbuf, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "%a : PmcSendCommand() Status = %r \n",  __FUNCTION__, Status));
  }
}