/** @file
  This is the Common driver that initializes the Intel PCH.

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
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>

#include "PchInit.h"
#include <PchPolicyCommon.h>
#include <Library/SpiCommonLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PchDmiLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcrLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/SpiInitLib.h>
#include <Register/PchRegs.h>
#include <Register/EspiRegs.h>
#include <Register/SpiRegs.h>
#include <Register/PchRegsPsth.h>
#include <Register/TcoRegs.h>
#include <Register/HdaRegs.h>
#include <Register/PmcRegs.h>
#include <Fru/PtlPcd/IncludePrivate/Register/PmcRegsPtlPcd.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchInfoLib.h>
#include <Library/DciPrivateLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdSpiSocLib.h>
#include <Library/SpiInitLib.h>
#include <Library/DxeEspiPrivateLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>
#include <Register/GpioAcpiDefines.h>
#include <Library/P2sbPrivateLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <Defines/PcdPchBdfAssignment.h>

//
// Module variables
//
GLOBAL_REMOVE_IF_UNREFERENCED ITSS_CONFIG_HOB          *mPchItssConfigHob;
GLOBAL_REMOVE_IF_UNREFERENCED LPSS_UART_CONFIG_HOB     *mPchLpssUartConfigHob;
GLOBAL_REMOVE_IF_UNREFERENCED PMC_CONFIG_HOB           *mPchPmcConfigHob;
GLOBAL_REMOVE_IF_UNREFERENCED LPSS_SPI_CONFIG_HOB      *mPchLpssSpiConfigHob;
GLOBAL_REMOVE_IF_UNREFERENCED LOCK_DOWN_CONFIG_HOB     *mPchLockDownConfigHob;
GLOBAL_REMOVE_IF_UNREFERENCED PCIE_RP_CONFIG_HOB       *mPchPcieRpConfigHob;
GLOBAL_REMOVE_IF_UNREFERENCED HDAUDIO_CONFIG_HOB       *mPchHdaConfigHob;

//
// EFI_EVENT
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_EVENT mHeciEvent;

/**
  Set the XHCI controller's PCI CMD.MSE and CMD.BME bits.

**/
VOID
XhciMmioEnable (
  VOID
  )
{
  UINT32  XhciBar;
  UINT32  Command;
  UINT64  XhciBaseAddress;

  //
  // Step 1. Make sure the XHCI BAR is initialized.
  //         Check if lower 32 bits of 64-bit BAR are configured.
  //
  XhciBaseAddress = PchXhciPciCfgBase ();
  XhciBar = PciSegmentRead32 (XhciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET) & ~(0xF);

  if (XhciBar == 0xFFFFFFF0) {
    return;
  }
  if ((XhciBar & 0xFFFF0000) == 0) {
    //
    // If lower 32 bits are not configured, check upper 32 bits.
    //
    XhciBar = PciSegmentRead32 (XhciBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + 4);
    if (XhciBar == 0) {
      return;
    }
  }

  //
  // Step 2. If XHCI's MSE (Memory Space Enable) or BME (Bus Master Enable) bits are cleared, set them.
  //
  Command = PciSegmentRead32 (XhciBaseAddress + PCI_COMMAND_OFFSET);
  if ((Command & (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) != (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)) {
    PciSegmentOr32 (XhciBaseAddress + PCI_COMMAND_OFFSET, (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER));
  }
}

/**
  Common PchInit Module Entry Point
**/
VOID
PchInitEntryPointCommon (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS  HobPtr;

  DEBUG ((DEBUG_INFO, "PchInitEntryPointCommon() Start\n"));

  //
  // Get LOCK DOWN HOB
  //
  HobPtr.Guid   = GetFirstGuidHob (&gPchLockDownConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  mPchLockDownConfigHob = (LOCK_DOWN_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  //
  // Get PMC HOB
  //
  HobPtr.Guid   = GetFirstGuidHob (&gPchPmcConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  mPchPmcConfigHob = (PMC_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  //
  // Get LPSS UART HOB
  //
  HobPtr.Guid   = GetFirstGuidHob (&gPchLpssUartConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  mPchLpssUartConfigHob = (LPSS_UART_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  //
  // Get LPSS SPI HOB
  //
  HobPtr.Guid   = GetFirstGuidHob (&gPchLpssSpiConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  mPchLpssSpiConfigHob = (LPSS_SPI_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  //
  // Get ITSS HOB
  //
  HobPtr.Guid   = GetFirstGuidHob (&gPchItssConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  mPchItssConfigHob = (ITSS_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  //
  // Get HDAUDIO HOB
  //
  HobPtr.Guid   = GetFirstGuidHob (&gPchHdaConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  mPchHdaConfigHob = (HDAUDIO_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  //
  // Get PCIE RP HOB
  //
  HobPtr.Guid   = GetFirstGuidHob (&gPchPcieRpConfigHobGuid);
  ASSERT (HobPtr.Guid != NULL);
  mPchPcieRpConfigHob = (PCIE_RP_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr.Guid);

  DEBUG ((DEBUG_INFO, "PchInitEntryPointCommon() End\n"));

  return;
}

/**
  This function locks static power gating configuration with S3 Boot Script programming
**/
VOID
PmcLockFunctionDisableConfigWithS3BootScript (
  VOID
  )
{
  UINTN   PchPwrmBase;
  UINT32  DataAnd32;
  UINT32  DataOr32;

  PchPwrmBase = PmcGetPwrmBase ();
  DataOr32 = (UINT32)B_PTL_PCD_P_PMC_PWRM_GEN_PMCON_B_ST_FDIS_LK;
  DataAnd32 = ~(UINT32)B_PTL_PCD_P_PMC_PWRM_GEN_PMCON_B_ST_FDIS_LK;

  MmioOr32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B, DataOr32);

  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint32,
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B,
    &DataOr32,
    &DataAnd32
  );
}

/**
  Process all the lock downs
**/
VOID
ProcessAllLocks (
  VOID
  )
{
  UINT16        Data16And;
  UINT16        Data16Or;
  UINT16        TcoBase;
  SPI_HANDLE    SpiHandle;
  EFI_STATUS    Status;
  UINT64        EspiPciBase;

  PchTcoBaseGet (&TcoBase);

  //
  // Lock function disable (ST and NST PG) register fields.
  //
  PmcLockFunctionDisableConfigWithS3BootScript ();

  Status = PtlPcdGetSpiHandle (&SpiHandle, NULL);
  if (!EFI_ERROR (Status)) {
    //
    // Lock SPI register before boot.
    //
    LockSpiConfiguration (&SpiHandle);
    //
    // Set BiosLock amd BIOS interface lock.
    //
    SpiBiosLockEnableAndBiosInterfaceLockWithS3BootScript (
      &SpiHandle,
      (BOOLEAN) mPchLockDownConfigHob->BiosLock,
      (BOOLEAN) mPchLockDownConfigHob->BiosInterface,
      (BOOLEAN) mPchLockDownConfigHob->SpiEiss
      );
  }
  //
  // Set BiosLock amd BIOS interface lock
  //
  EspiPciBase = EspiPciCfgBase ();
  EspiBiosLockEnableAndBiosInterfaceLockWithS3BootScript (
    EspiPciBase,
    (BOOLEAN) mPchLockDownConfigHob->BiosLock,
    (BOOLEAN) mPchLockDownConfigHob->BiosInterface,
    (BOOLEAN) mPchLockDownConfigHob->SpiEiss
    );

  ///
  /// Additional Power Management Programming
  /// Step 3
  /// Lock configuration after stretch and ACPI base programming completed.
  ///
  PmcLockSlpSxStretchingPolicyWithS3BootScript (PmcGetPwrmBase());

  //
  // Lock Down TCO
  //
  Data16And = 0xFFFF;
  Data16Or  = B_TCO_IO_TCTL1_TCO_LOCK;
  IoOr16 (TcoBase + R_TCO_IO_TCTL1, Data16Or);
  S3BootScriptSaveIoReadWrite (
    S3BootScriptWidthUint16,
    (UINTN) TcoBase + R_TCO_IO_TCTL1,
    &Data16Or,  // Data to be ORed
    &Data16And  // Data to be ANDed
    );

  ///
  /// PCH BIOS Spec Section 5.15.1 Additional Chipset Initialization
  /// Step 1
  /// Lock PMC Set Strap Message Interface
  ///
  PmcLockSetStrapMsgInterfaceWithS3BootScript ();
  //
  // Lock Down PMC
  //
  PmcLockWithS3BootScript (PmcGetPwrmBase());
}

/**
  Set HD Audio PME bit
**/
VOID
ConfigureHdAudioPme (
  VOID
  )
{
  UINT64 HdaPciBase;

  HdaPciBase = HdaPciCfgBase ();

  if (PciSegmentRead16 (HdaPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }

  ///
  /// PME Enable for Audio controller
  ///
  if (mPchHdaConfigHob->Pme == TRUE) {
    PciSegmentOr32 (HdaPciBase + R_HDA_CFG_PCS, (UINT32) B_HDA_CFG_PCS_PMEE);
  }
}

/**
  Set eSPI BME bit
**/
VOID
ConfigureEspiBme (
  VOID
  )
{
  UINT64               EspiPciBase;
  VOID                 *HobPtr;
  ESPI_CONFIG_HOB      *EspiConfigHob;

  EspiPciBase = EspiPciCfgBase ();

  if (PciSegmentRead16 (EspiPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }
  if ((PciSegmentRead32 (EspiPciBase + R_ESPI_CFG_ESPI_BC) & B_ESPI_CFG_ESPI_BC_ESPI) == 0) {
    return;
  }

  //
  // Refer to PCH BWG.
  // To enable eSPI bus mastering BIOS must enable BME in eSPI controller
  // and also set BME bit in the respective target devices through Configuration
  // and Capabilities register of each target using Get_Configuration and Set_Configuration functionality.
  //
  // NOTE: The setting is also done in PEI, but might be cleared by PCI bus during PCI enumeration.
  //       Therefore, reeable it after PCI enumeration done.
  //
  HobPtr = GetFirstGuidHob (&gPchEspiConfigHobGuid);
  if (HobPtr == NULL) {
    ASSERT (FALSE);
    return;
  }
  EspiConfigHob = (ESPI_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr);

  if (EspiConfigHob->BmeHostDeviceEnabled == TRUE) {
    PciSegmentOr8 (EspiPciBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_BUS_MASTER);
  }
}

/**
  Enable TraceHub BME & MSE which is cleared by pci bus enumeration.
  Without enabling it back, trace to memory will be interrupted
**/
VOID
ConfigureTraceHub (
  VOID
  )
{
  UINT64 TraceHubPciBase;

  TraceHubPciBase = PchTraceHubPciCfgBase ();

  if (PciSegmentRead16 (TraceHubPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }
  PciSegmentOr8 (TraceHubPciBase + PCI_COMMAND_OFFSET, (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));
}

/**
  Common PCH initialization before Boot Sript Table is closed

**/
VOID
PchOnPciEnumCompleteCommon (
  VOID
  )
{
  UINT32                          Data32Or;
  UINT32                          Data32And;
  BOOLEAN                         ResetStatus;
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   DciPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;
#ifndef MDEPKG_NDEBUG
  EFI_STATUS                      Status;
  GPIOV2_SERVICES                 *GpioServices;
#endif

  P2SBPid.Pid16bit = PTL_SID_F2_PID_EXI;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &DciPcrAccess);

  DEBUG ((DEBUG_INFO, "PchOnPciEnumCompleteCommon() Start\n"));

  if (SiScheduleResetIsRequired ()) {
    ResetStatus = SiScheduleResetPerformReset ();
    ASSERT (!ResetStatus);
  }

  ProcessAllLocks ();

  //
  // PSTHCTL (0xD00h[2]) = 1, PSTH IOSF Primary Trunk Clock Gating Enable (PSTHIOSFPTCGE)
  Data32And = 0xFFFFFFFF;
  Data32Or =  B_PSTH_PCR_PSTHCTL_PSTHIOSFPTCGE;
  P2SBPid.Pid16bit = PTL_SID_F2_PID_PSTH;
  PchPcrAndThenOr32 (P2SBPid.PortId.LocalPid, R_PSTH_PCR_PSTHCTL, Data32And, Data32Or);
  PCH_PCR_BOOT_SCRIPT_READ_WRITE (
    S3BootScriptWidthUint32,
    P2SBPid.PortId.LocalPid, R_PSTH_PCR_PSTHCTL,
    &Data32Or,
    &Data32And
    );

  //
  // Set eSPI BME after PCI enumeration
  //
  ConfigureEspiBme ();

  //
  // Set HD Audio PME after PCI Enumeration since bit gets cleared by PCI bus driver
  //
  ConfigureHdAudioPme ();
  //
  // Enable TraceHub MSE & BME
  //
  ConfigureTraceHub ();

  ///
  /// Clear Global Reset Status, Power Failure and Host Reset Status bits
  ///
  PmcClearGlobalResetStatus ();
  PmcClearPowerFailureStatus ();
  PmcClearHostResetStatus ();

#ifndef MDEPKG_NDEBUG
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][%a] ERROR getting GpioServices\n", __FUNCTION__));
    return;
  }
  GpioServices->ConflictCheck (GpioServices);
#endif // MDEPKG_NDEBUG

  if (IsDciDebugEnabled ((REGISTER_ACCESS *) &DciPcrAccess)) {
    //
    // Enable workaround for Windows kernel debugger connected through Xhci
    //
    XhciMmioEnable ();
  }


  //
  // Send IPC PCI Enum Done Notification to PMC
  //
  PmcSendBiosResetPciEnumDone (PmcGetPwrmBase ());

  DEBUG ((DEBUG_INFO, "PchOnPciEnumCompleteCommon() End\n"));
}
