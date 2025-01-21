/** @file
  Implementation of SPI IP init library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Pci30.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/SpiSocLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/IoLib.h>
#include <Library/SpiCommonLib.h>
#include <Library/TimerLib.h>
#include <Library/SpiInitLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/S3PciSegmentLib.h>
#include <Defines/PchReservedResources.h>
#include <Register/SpiRegs.h>
#include <Register/PchRegs.h>
#include <Ppi/SiPolicy.h>
#include <SpiConfig.h>
#include <SpiHandle.h>
#include <SpiInfoHob.h>


/**
  Lock SPI register before boot

  @param[in]  SpiHandle           SPI controller handle
**/
VOID
LockSpiConfiguration (
  IN  SPI_HANDLE        *SpiHandle
  )
{
  UINTN             Index;
  UINT16            Data16;
  UINT16            Data16And;
  UINT16            Data16Or;
  UINT32            Data32;
  UINT32            DlockValue;
  UINT32            Timer;

  if (SpiHandle == NULL) {
    DEBUG ((
      DEBUG_WARN,
      "%a - SPI_HANDLE was not provided, aborting\n",
      __FUNCTION__
      ));
    return;
  }

  //
  // Check for SPI controller presence before programming
  //
  if (PciSegmentRead16 (SpiHandle->PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }

  //
  // Make sure SPI BAR0 has fixed address before writing to boot script.
  // The same base address is set in PEI and will be used during resume.
  //
  PciSegmentAnd8 (SpiHandle->PciCfgBase + PCI_COMMAND_OFFSET, (UINT8) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentWrite32 (SpiHandle->PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) SpiHandle->MmioBase);
  if (PciSegmentRead32 (SpiHandle->PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (SpiHandle->PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (SpiHandle->MmioBase, 32));
  }
  PciSegmentOr8 (SpiHandle->PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Before setting FLOCKDN lock Extended BIOS Range configuration
  // All configuration of this feature shall be done already at this moment
  //
  PciSegmentOr32 (SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BC, B_SPI_CFG_BIOS_SPI_BC_EXT_BIOS_LOCK);
  Data32 = PciSegmentRead32 (SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BC);
  S3BootScriptSavePciCfgWrite (
    S3BootScriptWidthUint32,
    (UINTN) SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BC,
    1,
    (VOID *) &Data32
    );

  //
  // Program the Flash Protection Range Register based on policy
  //
  DlockValue = MmioRead32 (SpiHandle->MmioBase + R_SPI_MEM_BIOS_DLOCK);
  ASSERT (SpiHandle->SpiConfigHob != NULL);
  for (Index = 0; Index < PCH_FLASH_PROTECTED_RANGES; ++Index) {
    if ((SpiHandle->SpiConfigHob->ProtectRange[Index].WriteProtectionEnable ||
         SpiHandle->SpiConfigHob->ProtectRange[Index].ReadProtectionEnable) != TRUE) {
      continue;
    }

    //
    // Proceed to program the register after ensure it is enabled
    //
    Data32 = 0;
    Data32 |= (SpiHandle->SpiConfigHob->ProtectRange[Index].WriteProtectionEnable == TRUE) ? B_SPI_MEM_BIOS_FPR0_WPE : 0;
    Data32 |= (SpiHandle->SpiConfigHob->ProtectRange[Index].ReadProtectionEnable == TRUE) ? B_SPI_MEM_BIOS_FPR0_RPE : 0;
    Data32 |= ((UINT32) SpiHandle->SpiConfigHob->ProtectRange[Index].ProtectedRangeLimit << N_SPI_MEM_BIOS_FPR0_PRL) & B_SPI_MEM_BIOS_FPR0_PRL;
    Data32 |= SpiHandle->SpiConfigHob->ProtectRange[Index].ProtectedRangeBase & B_SPI_MEM_BIOS_FPR0_PRB;
    DEBUG ((DEBUG_INFO, "Protected range %d: 0x%08x \n", Index, Data32));

    DlockValue |= (UINT32) (B_SPI_MEM_BIOS_DLOCK_PR0LOCKDN << Index);
    MmioWrite32 ((UINTN) (SpiHandle->MmioBase + (R_SPI_MEM_BIOS_FPR0 + (Index * S_SPI_MEM_BIOS_FPRX))), Data32);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (SpiHandle->MmioBase + (R_SPI_MEM_BIOS_FPR0 + (Index * S_SPI_MEM_BIOS_FPRX))),
      1,
      (VOID *) (UINTN) (SpiHandle->MmioBase + (R_SPI_MEM_BIOS_FPR0 + (Index * S_SPI_MEM_BIOS_FPRX)))
      );
  }
  //
  // Program DLOCK register
  //
  MmioWrite32 ((UINTN) SpiHandle->MmioBase + R_SPI_MEM_BIOS_DLOCK, DlockValue);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) SpiHandle->MmioBase + R_SPI_MEM_BIOS_DLOCK,
    1,
    (VOID *) ((UINTN) SpiHandle->MmioBase + R_SPI_MEM_BIOS_DLOCK)
    );

  ///
  /// PCH BIOS Spec Section 3.6 Flash Security Recommendation
  /// In PCH SPI controller the BIOS should set the Flash Configuration Lock-Down bit
  /// (SPI_BAR0 + 04[15]) at end of post.  When set to 1, those Flash Program Registers
  /// that are locked down by this FLOCKDN bit cannot be written.
  /// Please refer to the EDS for which program registers are impacted.
  /// Additionally BIOS must program SPI_BAR0 + 0x04 BIT11 (WRSDIS) to disable Write Status in HW sequencing
  ///

  //
  // Ensure there is no pending SPI trasaction before setting lock bits
  //
  Timer = 0;
  while (MmioRead16 (SpiHandle->MmioBase + R_SPI_MEM_BIOS_HSFSTS_CTL) & B_SPI_MEM_BIOS_HSFSTS_CTL_H_SCIP) {
    if (Timer > SPI_WAIT_TIME) {
      //
      // SPI transaction is pending too long at this point, exit with error.
      //
      DEBUG ((DEBUG_WARN, "SPI Cycle timeout\n"));
      ASSERT (FALSE);
      break;
    }
    MicroSecondDelay (SPI_WAIT_PERIOD);
    Timer += SPI_WAIT_PERIOD;
  }

  Data16And = B_SPI_MEM_BIOS_HSFSTS_CTL_H_SCIP;
  Data16    = 0;
  S3BootScriptSaveMemPoll (
    S3BootScriptWidthUint16,
    SpiHandle->MmioBase + R_SPI_MEM_BIOS_HSFSTS_CTL,
    &Data16And,
    &Data16,
    SPI_WAIT_PERIOD,
    SPI_WAIT_TIME / SPI_WAIT_PERIOD
    );

  //
  // Clear any outstanding status
  //
  Data16Or  = B_SPI_MEM_BIOS_HSFSTS_CTL_H_SAF_DLE
            | B_SPI_MEM_BIOS_HSFSTS_CTL_H_SAF_ERROR
            | B_SPI_MEM_BIOS_HSFSTS_CTL_H_AEL
            | B_SPI_MEM_BIOS_HSFSTS_CTL_FCERR
            | B_SPI_MEM_BIOS_HSFSTS_CTL_FDONE;
  Data16And = 0xFFFF;
  MmioAndThenOr16 (SpiHandle->MmioBase + R_SPI_MEM_BIOS_HSFSTS_CTL, Data16And, Data16Or);
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint16,
    SpiHandle->MmioBase + R_SPI_MEM_BIOS_HSFSTS_CTL,
    &Data16Or,
    &Data16And
    );

  //
  // Set WRSDIS
  //
  Data16Or  = B_SPI_MEM_BIOS_HSFSTS_CTL_WRSDIS;
  Data16And = 0xFFFF;
  MmioAndThenOr16 (SpiHandle->MmioBase + R_SPI_MEM_BIOS_HSFSTS_CTL, Data16And, Data16Or);
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint16,
    SpiHandle->MmioBase + R_SPI_MEM_BIOS_HSFSTS_CTL,
    &Data16Or,
    &Data16And
    );

  //
  // Set FLOCKDN
  //
  Data16Or  = B_SPI_MEM_BIOS_HSFSTS_CTL_FLOCKDN;
  Data16And = 0xFFFF;
  MmioAndThenOr16 (SpiHandle->MmioBase + R_SPI_MEM_BIOS_HSFSTS_CTL, Data16And, Data16Or);
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint16,
    SpiHandle->MmioBase + R_SPI_MEM_BIOS_HSFSTS_CTL,
    &Data16Or,
    &Data16And
    );

  ///
  /// SPI Flash Programming Guide Section 5.5.2 Vendor Component Lock
  /// It is strongly recommended that BIOS sets the Vendor Component Lock (VCL) bits. VCL applies
  /// the lock to both VSCC0 and VSCC1 even if VSCC0 is not used. Without the VCL bits set, it is
  /// possible to make Host/GbE VSCC register(s) changes in that can cause undesired host and
  /// integrated GbE Serial Flash functionality.
  ///
  MmioOr32 ((UINTN) SpiHandle->MmioBase + R_SPI_MEM_BIOS_SFDP0_VSCC0, B_SPI_MEM_BIOS_SFDP0_VSCC0_VCL);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) SpiHandle->MmioBase + R_SPI_MEM_BIOS_SFDP0_VSCC0,
    1,
    (VOID *) ((UINTN) SpiHandle->MmioBase + R_SPI_MEM_BIOS_SFDP0_VSCC0)
    );
}

/**
  Enables Extended BIOS Range decoding in SPI controller by programming SPI
  BAR1 and enable bits

  @param[in] SpiHandle       SPI controller handle

  @retval   EFI_SUCCESS               Range was programmed and enabled
  @retval   EFI_INVALID_PARAMETER     One of arguments is incorrect
**/
EFI_STATUS
SpiEnableExtendedBiosRangeSupport (
  SPI_HANDLE  *SpiHandle
  )
{
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  //
  // Check for SPI controller presence before programming
  //
  if (PciSegmentRead16 (SpiHandle->PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return EFI_UNSUPPORTED;
  }

  //
  // Disable MSE
  //
  PciSegmentAnd16 (
    SpiHandle->PciCfgBase + PCI_COMMAND_OFFSET,
    (UINT16)~(EFI_PCI_COMMAND_MEMORY_SPACE)
    );

  //
  // Program BAR1 with decode range base address
  //
  PciSegmentWrite32 (
    SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BAR1,
    (SpiHandle->SpiConfig->ExtendedBiosDecodeRangeBase & B_SPI_CFG_BIOS_SPI_BAR0_MEMBAR)
    );

  //
  // Program EXT_BIOS_LIMIT_OFFSET to 16MB
  //
  PciSegmentAndThenOr32 (
    SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BC,
    (UINT32)~(B_SPI_CFG_BIOS_SPI_BC_EXT_BIOS_LIMIT_OFFSET),
    (UINT32) (1 << 24)
    );

  //
  // Program EXT_BIOS_EN
  //
  PciSegmentOr32 (
    SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BC,
    (UINT32) (B_SPI_CFG_BIOS_SPI_BC_EXT_BIOS_EN)
    );

  //
  // Enable MSE
  //
  PciSegmentOr16 (
    SpiHandle->PciCfgBase + PCI_COMMAND_OFFSET,
    (UINT16) EFI_PCI_COMMAND_MEMORY_SPACE
    );

  return EFI_SUCCESS;
}

/**
  Disable EISS (Enable InSMM.STS)

  @param[in]  SpiHandle           Instance of SPI_HANDLE describing SPI Flash Controller
**/
VOID
SpiDisableEiss (
  IN SPI_HANDLE       *SpiHandle
  )
{
  if (SpiHandle == NULL) {
    DEBUG ((DEBUG_WARN, "%a - invalid SPI handle provided.\n", __FUNCTION__));
    return;
  }

  PciSegmentAnd8 (SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BC, (UINT8) ~(B_SPI_CFG_BIOS_SPI_BC_EISS));
}

/**
  Disable Unsupport Request Reporting Enable

  @param[in]  SpiHandle           Instance of SPI_HANDLE describing SPI Flash Controller
**/
VOID
SpiDisableUrre (
  IN SPI_HANDLE       *SpiHandle
  )
{
  if (SpiHandle == NULL) {
    DEBUG ((DEBUG_WARN, "%a - invalid SPI handle provided.\n", __FUNCTION__));
    return;
  }

  PciSegmentAnd8 (SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_UR_STS_CTL, (UINT8) ~(B_SPI_CFG_BIOS_SPI_UR_STS_CTL_URRE));
}

/**
  Detect eDAF mode

  @param[in]  SpiHandle           Instance of SPI_HANDLE describing SPI Flash Controller
**/
BOOLEAN
SpiDetectEdafMode (
  IN SPI_HANDLE       *SpiHandle
  )
{
  if (SpiHandle == NULL) {
    DEBUG ((DEBUG_WARN, "%a - invalid SPI handle provided.\n", __FUNCTION__));
    return FALSE;
  }

  return !!(MmioRead32 (SpiHandle->MmioBase + R_SPI_MEM_BIOS_HSFSTS_CTL) & B_SPI_MEM_BIOS_HSFSTS_CTL_H_SAF_MODE_ACTIVE);
}

/**
  Configure BiosLockEnable bit and BiosInterfaceLock bit according to policy setting.

  @param[in] SpiHandle          Instance of SPI_HANDLE describing SPI Flash Controller
  @param[in] BiosLockEnable     Policy for BiosLockEnable bit programming
  @param[in] BiosInterfaceLock  Policy for BiosInterfaceLock bit programming
  @param[in] EissEnable         Policy for EISS bit programming

**/
VOID
SpiBiosLockEnableAndBiosInterfaceLockWithS3BootScript (
  IN SPI_HANDLE   *SpiHandle,
  IN BOOLEAN      BiosLockEnable,
  IN BOOLEAN      BiosInterfaceLock,
  IN BOOLEAN      EissEnable
  )
{
  UINT8                    SpiData8;

  if (!BiosLockEnable && !BiosInterfaceLock) {
    return;
  }

  ///
  /// PCH BIOS Spec Flash Security Recommendation
  ///
  /// BIOS needs to enable the BIOS Lock Enable (BLE) feature of the PCH by setting
  /// SPI/eSPI/LPC PCI offset DCh[1] = 1b.
  /// When this bit is set, attempts to write the Write Protect Disable (WPD) bit
  /// in PCH will cause a SMI which will allow the BIOS to verify that the write is
  /// from a valid source.
  /// Remember that BIOS needs to set SPI/LPC/eSPI PCI Offset DC [0] = 0b to enable
  /// BIOS region protection before exiting the SMI handler.
  /// Also, TCO_EN bit needs to be set (SMI_EN Register, ABASE + 30h[13] = 1b) to keep
  /// BLE feature enabled after booting to the OS.
  /// Intel requires that BIOS enables the Lock Enable (LE) feature of the PCH to
  /// ensure SMM protection of flash.
  /// RC installs a default SMI handler that clears WPD.
  /// There could be additional SMI handler to log such attempt if desired.
  ///
  /// BIOS needs to enable the "Enable in SMM.STS" (EISS) feature of the PCH by setting
  /// SPI PCI offset DCh[5] = 1b for SPI or setting eSPI PCI offset DCh[5] = 1b for eSPI.
  /// When this bit is set, the BIOS region is not writable until SMM sets the InSMM.STS bit,
  /// to ensure BIOS can only be modified from SMM. Please refer to CPU BWG for more details
  /// on InSMM.STS bit.
  /// Intel requires that BIOS enables the Lock Enable (LE) feature of the PCH to ensure
  /// SMM protection of flash.
  /// SPI PCI offset DCh[1] = 1b for SPI or setting eSPI PCI offset DCh[1] = 1b for eSPI.
  /// When this bit is set, EISS is locked down.
  ///
  SpiData8 = 0;
  if (BiosLockEnable) {
    SpiData8 |= B_SPI_CFG_BIOS_SPI_BC_LE;
    if (EissEnable) {
      SpiData8 |= B_SPI_CFG_BIOS_SPI_BC_EISS;
    }
  }
  ///
  /// BIOS also needs to set the BIOS Interface Lock Down bit in multiple locations
  /// (PCR[DMI] + 274Ch[0], LPC/eSPI PCI offset DCh[7] and SPI PCI offset DCh[7]).
  /// Setting these bits will prevent writes to the Top Swap bit (under their respective locations)
  /// and the Boot BIOS Straps. Enabling this bit will mitigate malicious software
  /// attempts to replace the system BIOS option ROM with its own code.
  ///
  if (BiosInterfaceLock) {
    SpiData8 |= B_SPI_CFG_BIOS_SPI_BC_BILD;
  }

  S3PciSegmentOr8 (SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BC, SpiData8);
  //
  // Reads back for posted write to take effect
  //
  SpiData8 = S3PciSegmentRead8 (SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BC);
}


/**
  Lock SPI BIOS decode enable range from change. Saving lock configuration with use of S3BootScript.

  @param[in] SpiHandle          Instance of SPI_HANDLE describing SPI Flash Controller

**/
VOID
SpiBiosDecodeEnableLockWithS3BootScript (
  IN SPI_HANDLE   *SpiHandle
  )
{
  if (SpiHandle == NULL) {
    return;
  }

  S3PciSegmentOr32 (SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BDE, B_SPI_CFG_BIOS_SPI_BDE_BLE);
}

/**
  Clears BIOS Write Protect Disable bit

  @param[in]  SpiHandle           Instance of SPI_HANDLE describing SPI Flash Controller
**/
VOID
SpiClearBiosWriteProtectDisable (
  IN SPI_HANDLE       *SpiHandle
  )
{
  //
  // Disable BIOSWE bit to protect BIOS
  //
  PciSegmentAnd8 (
    SpiHandle->PciCfgBase + R_SPI_CFG_BIOS_SPI_BC,
    (UINT8) ~B_SPI_CFG_BIOS_SPI_BC_WPD
    );
}

/**
  Install the SPI Info Hob
**/
VOID
InstallSpiInfoHob (
  VOID
  )
{
  SPI_INFO_HOB       *SpiInfoHob;

  DEBUG ((DEBUG_INFO, "Install SPI Info HOB\n"));

  SpiInfoHob = BuildGuidHob (&gSpiInfoHobGuid, sizeof (SPI_INFO_HOB));
  ASSERT (SpiInfoHob != 0);
  if (SpiInfoHob == NULL) {
    return;
  }

  CopyGuid (&SpiInfoHob->EfiHobGuidType.Name, &gSpiInfoHobGuid);
  //
  // Set Extended BIOS Support as FALSE by default.
  //
  SpiInfoHob->ExtendedBiosDecodeRangeEnable = FALSE;

  DEBUG ((DEBUG_INFO, "SpiInfoHob->EfiHobGuidType.Name: %g\n", &SpiInfoHob->EfiHobGuidType.Name));
}