/** @file
  Board Initialization in PEI post memory phase.


  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <PiPei.h>
#include <IndustryStandard/Acpi.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

#include <Register/PttPtpRegs.h>
#include <Library/SiliconInitLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/BoardConfigLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Library/PeiHdaVerbTables.h>
#include <Library/GpioV2WrapperLib.h>
#include <PlatformBoardId.h>
#include <PlatformBoardConfig.h>
#include <Library/ResiliencySupportLib.h>
#include <TbtNvmRetimer.h>
#include <Library/EcMiscLib.h>
#if FixedPcdGetBool (PcdSetupEnable) == 1
  #include <Ppi/ReadOnlyVariable2.h>
  #include <SetupVariable.h>
  extern EFI_GUID gSetupVariableGuid;
#endif
#include <Library/GbeLib.h>
#include <Library/HobLib.h>
#include <MeBiosPayloadHob.h>
#include <Library/BoardConfigLib.h>
#include <ThcConfig.h>

#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiDxeSmmReserveMmio64SizeLib.h>
#include <MeBiosPayloadHob.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/CnviLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED SETUP_DATA                              mSetupData;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SETUP                               mPchSetup;
/**
  Configure PciHostBridge related PCDs

  @param[in] PeiServices    Pointer to PEI Services Table.
  @param[in] NotifyDesc     Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in] Ppi            Pointer to the PPI data associated with this function.

  @retval EFI_SUCCESS             ConfigurePciHostBridgePcdsEndOfPei  Succesful
**/
EFI_STATUS
EFIAPI
ConfigurePciHostBridgePcdsEndOfPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) ConfigurePciHostBridgePcdsEndOfPei
};

/**
  Boards configuration init function for PEI post memory phase.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
PtlGpioTableInit (
  VOID
  )
{
  GPIOV2_INIT_CONFIG  *GpioTable;

  //
  // GPIO Table Init
  //
  GpioTable = NULL;

  //
  // GPIO Table Init, Update PostMem GPIO table to PcdBoardGpioTable
  //
  GpioTable = (GPIOV2_INIT_CONFIG *)PcdGetPtr (VpdPcdBoardGpioTable);
  ConfigureGpioTabletoPCD (GpioTable, POST_MEM);
  return EFI_SUCCESS;
}

/**
  Configures GPIO

  @param[in]  GpioDefinition  Point to Platform Gpio table
  @param[in]  GpioTableCount  Number of Gpio table entries

**/
VOID
ConfigureGpio (
  IN GPIOV2_INIT_CONFIG               *GpioDefinition,
  IN UINT16                           GpioTableCount
  )
{
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "ConfigureGpio() Start\n"));

  Status = GpioV2ConfigurePads (GpioDefinition, GpioTableCount);
  ASSERT_EFI_ERROR (Status);

  Status = GpioV2SetConflictCheckData (GpioDefinition, GpioTableCount);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "ConfigureGpio() End\n"));
}

/**
  Touch panel GPIO init function for PEI post memory phase.
**/
VOID
TouchPanelGpioInit (
  VOID
  )
{
  GPIOV2_INIT_CONFIG              *VpdPcdGpioTable;
  VPD_GPIO_PAD                    *GpioVpd;
  UINT16                          Size;
  UINT16                          BoardId;

  DEBUG ((DEBUG_INFO, "TouchPanelGpioInit\n"));

  GpioVpd = NULL;
  VpdPcdGpioTable = NULL;
  Size = 0;
  GpioVpd = PcdGetPtr (VpdPcdTouchpanel0IrqGpio);
  PcdSet32S (PcdTouchpanelIrqGpio, GpioVpd->GpioPad);

  GpioVpd = PcdGetPtr (VpdPcdTouchpanel1IrqGpio);
  PcdSet32S (PcdTouchpanel1IrqGpio, GpioVpd->GpioPad);
  BoardId = PcdGet16 (PcdBoardId);

  switch (BoardId) {
    case BoardIdPtlSimics:
    case BoardIdPtlUHHmpRvp:
    case BoardIdPtlUHLp5Rvp1:
    case BoardIdPtlUHLp5Rvp3:
    case BoardIdPtlUHDdr5Rvp4:
    case BoardIdPtlUHCammDTbTRvp2:
    case BoardIdPtlUHCammBep:
    case BoardIdPtlHLp5Gcs1:
    case BoardIdPtlHLp5Gcs2:
    case BoardIdPtlUHLp5Aep:
    case BoardIdPtlUHLp5MemSktmRvp:
    case BoardIdPtlUHLp5Adk1:
    case BoardIdPtlUHLp5Adk2:
      //
      // Verify if THC0 or THC1 panels are enabled before changing GPIO configuration
      //
      if ((mPchSetup.ThcAssignment[0] == ThcAssignmentNone) && (mSetupData.PchI2cTouchPanelType != 0)) {
        DEBUG ((DEBUG_INFO, "THC0 Disabled. Configuring GPIO Touch Panel 1 set for other controller use\n"));
        VpdPcdGpioTable = PcdGetPtr (VpdPcdBoardGpioTableTouchPanel0);
        GetGpioTableSize (VpdPcdGpioTable, &Size);
        ConfigureGpio (
          VpdPcdGpioTable,
          Size
          );
      }

      if ((mPchSetup.ThcAssignment[1] == ThcAssignmentNone) && (mSetupData.PchI2cTouchPanelType != 0)) {
        DEBUG ((DEBUG_INFO, "THC1 Disabled. Configuring GPIO Touch Panel 2 set for other controller use\n"));
        VpdPcdGpioTable = PcdGetPtr (VpdPcdBoardGpioTableTouchPanel1);
        GetGpioTableSize (VpdPcdGpioTable, &Size);
        ConfigureGpio (
          VpdPcdGpioTable,
          Size
          );
      }
      break;
  }
}

/**
  Lch GPIO init function for Configure GPIO.
**/
VOID
LchGpioInit (
  VOID
  )
{
  GPIOV2_INIT_CONFIG              *VpdPcdGpioTable;
  UINT16                          Size;

  DEBUG ((DEBUG_INFO, "LchGpioInit Start\n"));

  VpdPcdGpioTable = NULL;
  Size = 0;

  VpdPcdGpioTable = PcdGetPtr (VpdPcdBoardGpioTableLch);
  PcdSet64S (PcdBoardGpioTableLch, (UINTN) VpdPcdGpioTable);
  GetGpioTableSize(VpdPcdGpioTable, &Size);
  PcdSet16S (PcdBoardGpioTableLchSize, Size);

  // Configure Lch GPIO only when its enabled in setup.
  //
  if (mSetupData.LchSupport) {
    DEBUG ((DEBUG_INFO, "Configure Lch gpio\n"));
    ConfigureGpio ((VOID *) (UINTN) PcdGet64 (PcdBoardGpioTableLch), (UINTN) PcdGet16 (PcdBoardGpioTableLchSize));
  }

  DEBUG ((DEBUG_INFO, "LchGpioInit End\n"));
}

/**
  HDA VerbTable init function for PEI post memory phase.
**/
VOID
PtlHdaVerbTableInit (
  VOID
  )
{
  HDAUDIO_VERB_TABLE                *VerbTableToUpdate;
  UINT8                             Index;

  if (mPchSetup.PchHdaAlc245DmicConfiguration == 1) {
    VerbTableToUpdate = &HdaVerbTableAlc245X4;
  } else {
    VerbTableToUpdate = &HdaVerbTableAlc245NoDmic;
  }

  for (Index = 0; Index < HdaVerbTableDataBase.Size; Index++) {
    if (HdaVerbTableDataBase.HdaVerbTable[Index]->Header.DeviceId == VerbTableToUpdate->Header.DeviceId) {
      HdaVerbTableDataBase.HdaVerbTable[Index] = VerbTableToUpdate;
      break;
    }
  }

  PcdSet64S (PcdHdaVerbTableDatabase, (UINTN) &HdaVerbTableDataBase);
}

/**
  Misc. init function for PEI post memory phase.
**/
VOID
PtlBoardMiscInit (
  VOID
  )
{
  EFI_STATUS    Status;
  VPD_GPIO_PAD *GpioVpd;

  GpioVpd = NULL;
  PcdSetBoolS (PcdVrAlertEnable, PcdGetBool (VpdPcdVrAlertEnable));
  PcdSetBoolS (PcdPchThermalHotEnable, PcdGetBool (VpdPcdPchThermalHotEnable));
  PcdSetBoolS (PcdDashGEnable, PcdGetBool (VpdPcdDashGEnable));

  // PCIE SLOT #1 RTD3
  GpioVpd = PcdGetPtr (VpdPcdPcieSlot1PwrEnableGpioNo);
  PcdSet32S (PcdPcieSlot1PwrEnableGpioNo, GpioVpd->GpioPad);
  PcdSetBoolS (PcdPcieSlot1PwrEnableGpioPolarity, PcdGetBool (VpdPcdPcieSlot1PwrEnableGpioPolarity));
  GpioVpd = PcdGetPtr (VpdPcdPcieSlot1HoldRstGpioNo);
  PcdSet32S (PcdPcieSlot1HoldRstGpioNo, GpioVpd->GpioPad);
  PcdSetBoolS (PcdPcieSlot1HoldRstGpioPolarity, PcdGetBool (VpdPcdPcieSlot1HoldRstGpioPolarity));
  PcdSet8S (PcdPcieSlot1RootPort, PcdGet8 (VpdPcdPcieSlot1RootPort));

  // M.2 SSD #1 RTD3
  GpioVpd = PcdGetPtr(VpdPcdM2Ssd1PowerEnableGpio);
  PcdSet32S (PcdPchSsd1PwrEnableGpioNo, GpioVpd->GpioPad);                                                // PCH M.2 SSD power enable gpio pin
  PcdSetBoolS (PcdPchSsd1PwrEnableGpioPolarity, PcdGetBool (VpdPcdM2Ssd1PowerEnableGpioPolarity));        // PCH M.2 SSD power enable gpio pin polarity
  GpioVpd = PcdGetPtr (VpdPcdM2Ssd1RstGpio);
  PcdSet32S (PcdPchSsd1RstGpioNo, GpioVpd->GpioPad);                                                      // PCH M.2 SSD reset gpio pin
  PcdSetBoolS (PcdPchSsd1RstGpioPolarity, PcdGetBool (VpdPcdM2Ssd1RstGpioPolarity));                      // PCH M.2 SSD reset gpio pin polarity

  // GEN5 M.2 SSD #2 RTD3
  GpioVpd = PcdGetPtr(VpdPcdM2Ssd2PowerEnableGpio);
  PcdSet32S (PcdM2Ssd2PowerEnableGpio, GpioVpd->GpioPad);                                                 // Gen5 M.2 SSD2 power enable gpio pin
  PcdSetBoolS (PcdM2Ssd2PowerEnableGpioPolarity, PcdGetBool (VpdPcdM2Ssd2PowerEnableGpioPolarity));       // Gen5 M.2 SSD2 power enable gpio pin polarity
  GpioVpd = PcdGetPtr (VpdPcdM2Ssd2RstGpio);
  PcdSet32S (PcdM2Ssd2RstGpio, GpioVpd->GpioPad);                                                         // Gen5 M.2 SSD2 reset gpio pin
  PcdSetBoolS (PcdM2Ssd2RstGpioPolarity, PcdGetBool (VpdPcdM2Ssd2RstGpioPolarity));                       // Gen5 M.2 SSD2 reset gpio pin polarity

 // PCIE SLOT #2 RTD3
  GpioVpd = PcdGetPtr (VpdPcdPcieSlot2PwrEnableGpioNo);
  PcdSet32S (PcdPcieSlot2PwrEnableGpioNo, GpioVpd->GpioPad);
  PcdSetBoolS (PcdPcieSlot2PwrEnableGpioPolarity, PcdGetBool (VpdPcdPcieSlot2PwrEnableGpioPolarity));
  GpioVpd = PcdGetPtr (VpdPcdPcieSlot2HoldRstGpioNo);
  PcdSet32S (PcdPcieSlot2HoldRstGpioNo, GpioVpd->GpioPad);
  PcdSetBoolS (PcdPcieSlot2HoldRstGpioPolarity, PcdGetBool (VpdPcdPcieSlot2HoldRstGpioPolarity));
  PcdSet8S (PcdPcieSlot2RootPort, PcdGet8 (VpdPcdPcieSlot2RootPort));

  // PCIE SLOT #3 RTD3
  GpioVpd = PcdGetPtr (VpdPcdPcieSlot3PwrEnableGpioNo);
  PcdSet32S (PcdPcieSlot3PwrEnableGpioNo, GpioVpd->GpioPad);
  PcdSetBoolS (PcdPcieSlot3PwrEnableGpioPolarity, PcdGetBool (VpdPcdPcieSlot3PwrEnableGpioPolarity));
  GpioVpd = PcdGetPtr (VpdPcdPcieSlot3HoldRstGpioNo);
  PcdSet32S (PcdPcieSlot3HoldRstGpioNo, GpioVpd->GpioPad);
  PcdSetBoolS (PcdPcieSlot3HoldRstGpioPolarity, PcdGetBool (VpdPcdPcieSlot3HoldRstGpioPolarity));
  PcdSet8S (PcdPcieSlot3RootPort, PcdGet8 (VpdPcdPcieSlot3RootPort));
  Status = PeiServicesNotifyPpi (&mEndOfPeiNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR , "ConfigurePciHostBridgePcdsEndOfPei  Notify PPI Failed - %r\n", Status));
  }

  return;
}

/**
  Configure PciHostBridge related PCDs

  @param[in] PeiServices    Pointer to PEI Services Table.
  @param[in] NotifyDesc     Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in] Ppi            Pointer to the PPI data associated with this function.

  @retval EFI_SUCCESS             ConfigurePciHostBridgePcdsEndOfPei  Succesful
**/
EFI_STATUS
EFIAPI
ConfigurePciHostBridgePcdsEndOfPei  (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                       Status;
  EFI_PHYSICAL_ADDRESS             PciBaseAddress;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  UINT64                           Length;
  UINTN                            ResMemLimit1;
  UINT64                           PhysicalAddressLimit;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,  // GUID
             0,                                 // INSTANCE
             NULL,                              // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices        // PPI
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ConfigurePciHostBridgePcds: PeiServicesLocatePpi failed\n"));
    return Status;
  }

  //
  // Allocate 56 KB of I/O space [0x2000..0xFFFF]
  //
  DEBUG ((DEBUG_INFO, " Assign IO resource for PCI_ROOT_BRIDGE from 0x%X to 0x%X\n", PcdGet16 (PcdPciReservedIobase) , PcdGet16 (PcdPciReservedIoLimit)));

  //
  // Read memory map registers
  //
  PciBaseAddress         = (UINT32) GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase);
  PhysicalAddressLimit   = 0;

  ResMemLimit1 = PcdGet32 (PcdPciReservedMemLimit);
  if (ResMemLimit1 == 0) {
    ResMemLimit1 = (UINTN) PcdGet64 (PcdPciExpressBaseAddress);
  }

  Length = ResMemLimit1 - PciBaseAddress;

  if (Length != 0) {
    PcdSet32S (PcdPciReservedMemBase, (UINT32) PciBaseAddress);
    PcdSet32S (PcdPciReservedMemLimit, (UINT32) (PciBaseAddress + Length - 1));
    DEBUG ((DEBUG_INFO, " Assign Memory Resource for PCI_ROOT_BRIDGE from 0x%X", PcdGet32 (PcdPciReservedMemBase)));
    DEBUG ((DEBUG_INFO, " to 0x%X\n", PcdGet32 (PcdPciReservedMemLimit)));
  }

  //
  // Check Enable Above 4GB MMIO or not
  //
  PhysicalAddressLimit = LShiftU64 (1, GetMaxPhysicalAddressSize ());
  //
  // Provide 256GB to Max addressable memory range available above 4GB MMIO resource
  // limited to use single variable MTRR to cover this above 4GB MMIO region.
  //
  PcdSet64S (PcdPciReservedMemAbove4GBBase, PcdGet64 (PcdMmio64Base));
  if ((PhysicalAddressLimit % SIZE_256GB) == 0) { // Checking Memory Size multiples of 256GB
    PcdSet64S (PcdPciReservedMemAbove4GBLimit, (PhysicalAddressLimit - ReserveMmio64Size () - 1));
  } else {
    DEBUG ((DEBUG_INFO, "Memory Limit Size is not multiple of 256 GB, Limiting the size to 256GB\n"));
    PcdSet64S (PcdPciReservedMemAbove4GBLimit, PcdGet64 (PcdPciReservedMemAbove4GBBase) + PcdGet64 (PcdMmio64Length) - 1);
  }
  if (PcdGet64 (PcdPciReservedMemAbove4GBBase) < PcdGet64 (PcdPciReservedMemAbove4GBLimit)) {
    DEBUG ((DEBUG_INFO, " PCI space that above 4GB MMIO is from 0x%lX", PcdGet64 (PcdPciReservedMemAbove4GBBase)));
    DEBUG ((DEBUG_INFO, " to 0x%lX\n", PcdGet64 (PcdPciReservedMemAbove4GBLimit)));
  } else {
    DEBUG ((DEBUG_INFO, "Pci Reserved MemAbove 4GB Base = 0x%lX\n", PcdGet64 (PcdPciReservedMemAbove4GBBase)));
    DEBUG ((DEBUG_INFO, "Pci Reserved MemAbove 4GB Limit = 0x%lX\n", PcdGet64 (PcdPciReservedMemAbove4GBLimit)));
  }
  return EFI_SUCCESS;
}

/**
  Tbt init function for PEI post memory phase.
**/
VOID
PtlBoardTbtInit (
  VOID
  )
{

  USBC_RETIMER_FLASH_BOARD_CONFIG *RetimerFlashConfig;

  //
  // PD/Retimer information
  //
  PcdSetBoolS (PcdUsbCEcSupportPdInfoPresent, PcdGetBool (VpdPcdUsbCEcSupportPdInfoPresent));

  PcdSetBoolS (PcdUsbCEcSupportRetimerInfoPresent, PcdGetBool (VpdPcdUsbCEcSupportRetimerInfoPresent));
  PcdSet8S (PcdUsbCCapsuleDebugLevel, mSetupData.UsbcCapsuleDebugLevel);

  //
  // 2 seconds GOP HPD polling timer + 1 seconds to ensure all pending timer callbacks to be serviced.
  //
  PcdSet32S (PcdTcssWaitRetimerDeviceReady, 3000000);
  //
  // Initialize Retimer configuration table and update to PcdUsbCRetimerConfigTable
  //
  RetimerFlashConfig = (USBC_RETIMER_FLASH_BOARD_CONFIG *)PcdGetPtr (VpdPcdUsbCRetimerFlash);
  SetRetimerFlashNumber (RetimerFlashConfig);
  SetRetimerCfgTablePcd (RetimerFlashConfig);

  UpdateUcxxDriverTypePcd();
}

/**
   Security GPIO init function for PEI post memory phase.
**/
VOID
PtlBoardSecurityInit (
  VOID
  )
{
  GPIOV2_PAD    GpioPad;
  UINT32        GpioIrqNumber;
  UINT32        VerFtif;
  VPD_GPIO_PAD *GpioVpd;

  GpioVpd = NULL;
  if (FeaturePcdGet (PcdTpm2Enable) == TRUE) {
    //
    // If dTPM is connected to the SPI I/F, configure interrupt
    //
    VerFtif = MmioRead32 (R_PTT_TXT_STS_FTIF);
    GpioVpd = PcdGetPtr (VpdPcdTpm2IntGpio);
    GpioPad = GpioVpd->GpioPad;
    if (GpioPad != 0) {
      if ((VerFtif & FTIF_FT_LOC_MASK) == V_FTIF_SPI_DTPM_PRESENT) {
        GpioIrqNumber = 0;
        GpioV2GetPadIoApicIrqNum (GpioPad, &GpioIrqNumber);
        DEBUG ((DEBUG_INFO, "TPM Interrupt Number: %d\n", GpioIrqNumber));
        PcdSet32S (PcdTpm2CurrentIrqNum, GpioIrqNumber);
      } else {
        PcdSet32S (PcdTpm2CurrentIrqNum, 0);
      }
    }
  }
}

/**
  Capsule Update init function for PEI post memory phase.
**/
VOID
PtlBoardCapsuleUpdateInit (
  VOID
  )
{
  if (GetBiosResiliencyType () == SUPPORT_NVME_BASED_RECOVERY) {
    DEBUG ((DEBUG_INFO, "Set slot port number for nvme %d \n", PcdGet32 (VpdPcdNvmeRecoveryPrimarySlotPortNumber)));
    PcdSet32S (PcdNvmeRecoveryPrimarySlotPortNumber, PcdGet32 (VpdPcdNvmeRecoveryPrimarySlotPortNumber));
  }
}

/**
  Board Specific Init for PEI post memory phase.
**/
VOID
PeiPtlBoardSpecificInitPostMemNull (
  VOID
  )
{
}

/**
  Board's PCD function hook init function for PEI post memory phase.
**/
VOID
PtlBoardFunctionInit (
  VOID
)
{
  PcdSet64S (PcdFuncPeiBoardSpecificInitPostMem, (UINTN) PeiPtlBoardSpecificInitPostMemNull);
}

/**
  Configure GPIO, TouchPanel, HDA, PMC, TBT etc.

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
PtlBoardInitBeforeSiliconInit (
  VOID
  )
{

  EFI_STATUS                      Status;
  UINTN                           VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;

  VariableServices = NULL;
  Status = PeiServicesLocatePpi (
           &gEfiPeiReadOnlyVariable2PpiGuid,
           0,
           NULL,
           (VOID **) &VariableServices
           );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }

  VarSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                             VariableServices,
                             L"PchSetup",
                             &gPchSetupVariableGuid,
                             NULL,
                             &VarSize,
                             &mPchSetup
                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Pch setup data failed\n"));
  }
  VarSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                             VariableServices,
                             L"Setup",
                             &gSetupVariableGuid,
                             NULL,
                             &VarSize,
                             &mSetupData
                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Setup Data failed\n"));
  }

  PtlGpioTableInit ();
  TouchPanelGpioInit ();
  PtlHdaVerbTableInit ();
  PtlBoardMiscInit ();
  InstallPeiGfxPlatformPpi();
  PtlBoardTbtInit ();
  PtlBoardCapsuleUpdateInit ();
  PtlBoardFunctionInit();
  PtlBoardSecurityInit();
  GpioInit (PcdGetPtr (PcdBoardGpioTable));
  LchGpioInit();
  PlatformDebugStatePcdInit();
  ///
  /// Do Late PCH init
  ///
  LateSiliconInit ();


  return EFI_SUCCESS;
}
VOID
PtlBoardSpecificGpioInitPostMem (
  VOID
  )
{
  VPD_GPIO_PAD *GpioVpd;
  GpioVpd = NULL;
  //
  // Assign FingerPrint, Gnss, Bluetooth & Touch Pad relative GPIO.
  //

  GpioVpd = PcdGetPtr (VpdPcdLchIrqGpioPin);
  PcdSet32S (PcdLchIrqGpioPin, GpioVpd->GpioPad);
  GpioVpd = PcdGetPtr (VpdPcdLchResetGpioPin);
  PcdSet32S (PcdLchResetGpioPin, GpioVpd->GpioPad);

  GpioVpd = PcdGetPtr (VpdPcdTouchpadIrqGpio);
  PcdSet32S (PcdTouchpadIrqGpio, GpioVpd->GpioPad);                     // Touch Pad Interrupt Pin

  PcdSetBoolS (PcdTouchpadIrqGpioPolarity, PcdGetBool (VpdPcdTouchpadIrqGpioPolarity));                   // Touch Pad Interrupt pin polarity

  GpioVpd = PcdGetPtr (VpdPcdHdaI2sCodecIrqGpio);
  PcdSet32S(PcdHdaI2sCodecIrqGpio, GpioVpd->GpioPad);
  PcdSet8S(PcdHdaI2sCodecI2cBusNumber, PcdGet8 (VpdPcdHdaI2sCodecI2cBusNumber));// Audio I2S Codec conntected to I2C0

  //
  // Update OEM table ID
  //
  PcdSet64S (PcdBoardRtd3TableSignature, PcdGet64 (VpdPcdBoardRtd3TableSignature));

  //
  //Modify Preferred_PM_Profile field based on Board SKU's. Default is set to Mobile
  //
  PcdSet8S (PcdFadtPreferredPmProfile, EFI_ACPI_6_5_PM_PROFILE_MOBILE);
  if (PcdGet8 (PcdPlatformFlavor) == FlavorUpServer) {
    PcdSet8S (PcdFadtPreferredPmProfile, EFI_ACPI_6_5_PM_PROFILE_ENTERPRISE_SERVER);
  }
}

VOID
PtlInitCommonPlatformPcdPostMem (
  VOID
  )
{
  VPD_GPIO_PAD *GpioVpd;

  GpioVpd = NULL;
  //
  // SMC Runtime Sci Pin
  // EC will use eSpi interface to generate SCI
  //
  PcdSet32S (PcdSmcRuntimeSciPin, PcdGet32 (VpdPcdSmcRuntimeSciPin));
  //
  // Pm Power Button Gpio Pin
  //
  GpioVpd = PcdGetPtr (VpdPcdPmPowerButtonGpioPin);
  PcdSet32S (PcdPmPowerButtonGpioPin, GpioVpd->GpioPad);

  PcdSetBoolS (PcdSpdAddressOverride, PcdGetBool (VpdPcdSpdAddressOverride));
  //
  // Battery Support
  // Real & Virtual battery is need to supported in all except Desktop
  //
  PcdSet8S (PcdBatterySupport, PcdGet8 (VpdPcdBatterySupport));
  //
  // Sky Camera Sensor
  //
  PcdSetBoolS (PcdMipiCamSensor, PcdGetBool (VpdPcdMipiCamSensor));
  //
  // MIPI CSI Camera
  //
  PcdSetBoolS (PcdMipiCamGpioEnable, PcdGetBool (VpdPcdMipiCamGpioEnable));
  //
  // EC UART
  //
  PcdSetBoolS (PcdEcUart, PcdGetBool (VpdPcdEcUart));
  //
  // ZPODD
  //
  PcdSet8S (PcdZPoddConfig, PcdGet8 (VpdPcdZPoddConfig));

  //
  // Convertable Dock Support
  // Not supported only for S & H SKU's
  PcdSetBoolS (PcdConvertableDockSupport, PcdGetBool (VpdPcdConvertableDockSupport));
  //
  // EC Hotkey F3, F4, F5, F6, F7 and F8 Support
  //
  PcdSet8S (PcdEcHotKeyF3Support, PcdGet8 (VpdPcdEcHotKeyF3Support));
  PcdSet8S (PcdEcHotKeyF4Support, PcdGet8 (VpdPcdEcHotKeyF4Support));
  PcdSet8S (PcdEcHotKeyF5Support, PcdGet8 (VpdPcdEcHotKeyF5Support));
  PcdSet8S (PcdEcHotKeyF6Support, PcdGet8 (VpdPcdEcHotKeyF6Support));
  PcdSet8S (PcdEcHotKeyF7Support, PcdGet8 (VpdPcdEcHotKeyF7Support));
  PcdSet8S (PcdEcHotKeyF8Support, PcdGet8 (VpdPcdEcHotKeyF8Support));

  //
  // Virtual Button Volume Up & Done Support
  // Virtual Button Home Button Support
  // Virtual Button Rotation Lock Support
  //
  PcdSetBoolS (PcdVirtualButtonVolumeUpSupport, PcdGetBool (VpdPcdVirtualButtonVolumeUpSupport));
  PcdSetBoolS (PcdVirtualButtonVolumeDownSupport, PcdGetBool (VpdPcdVirtualButtonVolumeDownSupport));
  PcdSetBoolS (PcdVirtualButtonHomeButtonSupport, PcdGetBool (VpdPcdVirtualButtonHomeButtonSupport));
  PcdSetBoolS (PcdVirtualButtonRotationLockSupport, PcdGetBool (VpdPcdVirtualButtonRotationLockSupport));
  //
  // Slate Mode Switch Support
  //
  PcdSetBoolS (PcdSlateModeSwitchSupport, PcdGetBool (VpdPcdSlateModeSwitchSupport));
  //
  // Virtual Gpio Button Support
  //
  PcdSetBoolS (PcdVirtualGpioButtonSupport, PcdGetBool (VpdPcdVirtualGpioButtonSupport));
  //
  // Ac Dc Auto Switch Support
  //
  PcdSetBoolS (PcdAcDcAutoSwitchSupport, PcdGetBool (VpdPcdAcDcAutoSwitchSupport));

  //
  // Acpi Enable All Button Support
  //
  PcdSetBoolS (PcdAcpiEnableAllButtonSupport, PcdGetBool (VpdPcdAcpiEnableAllButtonSupport));
  //
  // Acpi Hid Driver Button Support
  //
  PcdSetBoolS (PcdAcpiHidDriverButtonSupport, PcdGetBool (VpdPcdAcpiHidDriverButtonSupport));
  //
  // CNVi Wi-Fi
  //
  PcdSetBoolS (PcdCnviPresence, IsCnviWifiEnabled ());
  //
  // MRC Dimm Slot Number
  //
  PcdSet64S (PcdSlotMapAddress, (UINT64) PcdGetPtr (VpdPcdMrcNoOfDimmSlot));
}

/**
  Board init for PEI after Silicon initialized

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
PtlBoardInitAfterSiliconInit (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "PtlBoardInitAfterSiliconInit \n"));
  PtlBoardSpecificGpioInitPostMem ();
  PtlInitCommonPlatformPcdPostMem ();

  return EFI_SUCCESS;
}
