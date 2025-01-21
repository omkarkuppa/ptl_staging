/** @file
  This file is SampleCode of the library for Intel Silicon PEI
  Platform Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include "PeiSiPolicyUpdate.h"
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/LpssUartDebugPropertyPcdLib.h>
#include <Library/SerialPortParameterLib.h>
#include <Library/SiPolicyLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/NemMapLib.h>
#include <BootStateLib.h>
#include <PolicyUpdateMacro.h>
#include <Setup.h>
#include <SetupVariable.h>
#include <SpiConfig.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#include <FspsUpd.h>
#endif
#include <Ppi/DebugEventHandler.h>

/**
  Check if BIOS Update is going to happen during this boot.

  This function checks:

    1. If BootMode is BOOT_ON_FLASH_UPDATE and there is a Capsule to update BIOS region detected.
    2. If TopSwap is get set, it is necessary for doing the BIOS resiliency restore.
    3. If BIOS update is in progress in the last boot.
  If one of above condition stands, set a GUID HOB as an indication to be consumed in DXE/BDS

  and return TRUE. Otherwise, return FALSE.

  @retval TRUE       BIOS Update is going to happen during this boot.
  @retval FALSE      No request of BIOS update is detected.

**/
BOOLEAN
IsBiosUpdateRequired (
  VOID
  );

/**
  This function performs Check First Boot and Rvp Support to see if Policy Default Check Required

  @retval BOOLEAN         Required TURE or FALSE
**/
BOOLEAN
EFIAPI
IsPolicyDefaultCheckRequired (
  )
{
  BOOLEAN         IsFirstBoot;
  BOOLEAN         IsRvpSupport;

  //
  // Check is first boot and is RVP supported.
  //
  if (IsBootStatePresent ()) {
    IsFirstBoot = FALSE;
  } else {
    IsFirstBoot = TRUE;
  }

  if ((PcdGet16 (PcdBoardId)) == (PcdGet16 (PcdDefaultBoardId))) {
    IsRvpSupport = TRUE;
  } else {
    IsRvpSupport = FALSE;
  }

  DEBUG ((DEBUG_INFO, "IsFirstBoot %x\n", IsFirstBoot));
  DEBUG ((DEBUG_INFO, "IsRvpSupport %x\n", IsRvpSupport));

  //
  // Policy Default Check only for First Boot and Rvp Support
  //
  return (IsFirstBoot && IsRvpSupport);
}

/**
  This function performs PCIE PEI Policy initialization.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPciePolicyPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             VariableSize;
  SI_SETUP                          SiSetup;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                              *FspmUpd;
#else
  SI_PREMEM_CONFIG                  *SiPreMemConfig;
  SPI_CONFIG                        *SpiConfig;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
  SiPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSiPreMemConfigGuid, (VOID *) &SiPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSpiConfigGuid, (VOID *) &SpiConfig);
  ASSERT_EFI_ERROR (Status);
#endif
  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SI_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"SiSetup",
                               &gSiSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SiSetup
                               );
  ASSERT_EFI_ERROR (Status);

  if (Status == EFI_SUCCESS) {
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PlatformDebugOption, SiPreMemConfig->PlatformDebugOption, SiSetup.PlatformDebugOption );
  }
  //
  // Policy to prevent MRC modify the boot mode while in flash update
  //
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SiSkipOverrideBootModeWhenFwUpdate, SiPreMemConfig->SkipOverrideBootModeWhenFwUpdate, (UINT8) FALSE);

  return EFI_SUCCESS;
}

/**
  This function performs Silicon PEI Policy initialization.

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI_NOT_FOUND           The FspmUpd is not found.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiSiPolicyPreMem (
  VOID
  )
{
#if FixedPcdGet8(PcdFspModeSelection) == 1
  EFI_STATUS                        Status;
  VOID                              *FspmUpd;
  EFI_PEI_HOB_POINTERS              Hob;
  DEBUG_CONFIG_DATA_HOB             *DebugConfigData;
  UINT8                             DebugInterfaces;
  DEBUG_EVENT_HANDLER_PPI           *DebugEventHandlerPpi;
  UINTN                             TempRamBase;
  UINT32                            TempRamSize;

  DEBUG ((DEBUG_INFO, "Update PeiSiPolicyUpdate Pre-Mem Start\n"));

  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
  if (FspmUpd == NULL) {
    return EFI_NOT_FOUND;
  }
  GetTempRamBaseAndSize (&TempRamBase, &TempRamSize);
  ((FSPM_UPD *)FspmUpd)->FspmArchUpd.StackBase = (EFI_PHYSICAL_ADDRESS) (TempRamBase + TempRamSize);
  ((FSPM_UPD *)FspmUpd)->FspmArchUpd.StackSize = PcdGet32 (PcdFspTemporaryRamSize);

  Status = PeiServicesGetBootMode (&(((FSPM_UPD *)FspmUpd)->FspmArchUpd.BootMode));
  if (EFI_ERROR (Status)) {
    ((FSPM_UPD *)FspmUpd)->FspmArchUpd.BootMode = BOOT_WITH_FULL_CONFIGURATION;
  }
  ((FSPM_UPD *)FspmUpd)->FspmArchUpd.BootLoaderTolumSize = 0x0;

  Status = PeiServicesLocatePpi (
           &gDebugEventHandlerPpiGuid,       // GUID
           0,                                // INSTANCE
           NULL,                             // EFI_PEI_PPI_DESCRIPTOR
           (VOID **) &DebugEventHandlerPpi   // PPI
           );
  if (!EFI_ERROR(Status)) {
    ((FSPM_UPD *) FspmUpd)->FspmArchUpd.FspEventHandler = (EFI_PHYSICAL_ADDRESS)(UINTN) DebugEventHandlerPpi->DebugEventHandler;
  }

  //
  // Initialize DebugConfigData
  //
  DebugInterfaces = 0;
  Hob.Guid = GetFirstGuidHob (&gDebugConfigHobGuid);
  if (Hob.Guid != NULL) {
    DebugConfigData = (DEBUG_CONFIG_DATA_HOB *) GET_GUID_HOB_DATA (Hob.Guid);
    if (DebugConfigData != NULL) {
      // Debug Interfaces
      if (DebugConfigData->RamDebugInterface)      { DebugInterfaces |= STATUS_CODE_USE_RAM; }
      if (DebugConfigData->UartDebugInterface)     { DebugInterfaces |= STATUS_CODE_USE_ISA_SERIAL; }
      if (DebugConfigData->Usb3DebugInterface)     { DebugInterfaces |= STATUS_CODE_USE_USB3; }
      if (DebugConfigData->TraceHubDebugInterface) { DebugInterfaces |= STATUS_CODE_USE_TRACEHUB; }
      ((FSPM_UPD *)FspmUpd)->FspmConfig.PcdDebugInterfaceFlags  = DebugInterfaces;
      // Serial debug message baud rate
      ((FSPM_UPD *)FspmUpd)->FspmConfig.PcdSerialDebugBaudRate  = DebugConfigData->SerialDebugBaudRate;
      // Serial debug message level
      ((FSPM_UPD *)FspmUpd)->FspmConfig.PcdSerialDebugLevel     = DebugConfigData->SerialDebug;
      // MRC Serial debug message level
      ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialDebugMrcLevel     = DebugConfigData->SerialDebugMrcLevel;
      if (DebugConfigData->SerialIoDebugInterface) {
        ((FSPM_UPD *)FspmUpd)->FspmConfig.PcdDebugInterfaceFlags            |= STATUS_CODE_USE_SERIALIO;
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugControllerNumber = DebugConfigData->SerialIoUartDebugControllerNumber;
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugBaudRate         = DebugConfigData->SerialIoUartDebugBaudRate;
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugStopBits         = DebugConfigData->SerialIoUartDebugStopBits;
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugParity           = DebugConfigData->SerialIoUartDebugParity;
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugAutoFlow         = DebugConfigData->SerialIoUartDebugFlowControl;
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugDataBits         = DebugConfigData->SerialIoUartDebugDataBits;
        // @todo - consider adding GPIO muxes to Setup option
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugRxPinMux         = 0x0;
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugTxPinMux         = 0x0;
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugRtsPinMux        = 0x0;
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugCtsPinMux        = 0x0;
        ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugMode             = 0x2; //SerialIoUartHidden
      }
    }
  }
  DEBUG ((DEBUG_INFO, "FspmUpd->FspmConfig.PcdDebugInterfaceFlags is 0x%X\n",             ((FSPM_UPD *)FspmUpd)->FspmConfig.PcdDebugInterfaceFlags));
  DEBUG ((DEBUG_INFO, "FspmUpd->FspmConfig.PcdSerialDebugBaudRate is 0x%X\n",             ((FSPM_UPD *)FspmUpd)->FspmConfig.PcdSerialDebugBaudRate));
  DEBUG ((DEBUG_INFO, "FspmUpd->FspmConfig.PcdSerialDebugLevel is 0x%X\n",                ((FSPM_UPD *)FspmUpd)->FspmConfig.PcdSerialDebugLevel));
  DEBUG ((DEBUG_INFO, "FspmUpd->FspmConfig.SerialDebugMrcLevel is 0x%X\n",                ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialDebugMrcLevel));
  DEBUG ((DEBUG_INFO, "FspmUpd->FspmConfig.SerialIoUartDebugControllerNumber is 0x%X\n",  ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugControllerNumber));
  DEBUG ((DEBUG_INFO, "FspmUpd->FspmConfig.SerialIoUartDebugBaudRate is %d\n",            ((FSPM_UPD *)FspmUpd)->FspmConfig.SerialIoUartDebugBaudRate));

  //
  // Update TcssXhciEn UPD based on Tcss Enable VPD PCD
  //
  ((FSPM_UPD *)FspmUpd)->FspmConfig.TcssXhciEn = PcdGet8 (VpdPcdTcssEnable);
  DEBUG ((DEBUG_INFO, "FspmUpd->FspmConfig.TcssXhciEn is 0x%X\n",((FSPM_UPD *)FspmUpd)->FspmConfig.TcssXhciEn));

#endif

  return EFI_SUCCESS;
}

STATIC SVID_SID_INIT_ENTRY mSsidTablePtr[SI_MAX_DEVICE_COUNT] = {0};

/**
  This provide sample code to update SSID table policy.

  @param[in] SiConfig  SICONFIG policy
**/
STATIC
VOID
UpdateSsidPolicy (
  SI_CONFIG  *SiConfig
  )
{
  UINT32                          EntryCount;
  EFI_STATUS                      Status;
  PCH_SETUP                       PchSetup;
  UINTN                           VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                               *FspsUpd;
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#endif

  EntryCount                    = 0;
  Status                        = EFI_SUCCESS;
  VariableServices              = NULL;
  VarSize                       = 0;

  ZeroMem (mSsidTablePtr, sizeof (mSsidTablePtr));

  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PeiServicesLocatePpi failed\n"));
    return;
  }

  VarSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &PchSetup
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetVariable (PchSetup) failed\n"));
    return;
  }

/*
  Below provides the sample code to update SSID table.
  This is valid when SkipSsidProgramming is FALSE;
  It doesn't need to contain entries for all Intel internal devices.
  It can only contains the SVID_SID_INIT_ENTRY entries for those Dev# Func# which needs
  to be override.
  In the enties, only Dev, Function, SubSystemVendorId, and SubSystemId are required.

  E.g. Platform only needs to override BDF 0:31:0 to AAAA:BBBB and BDF 0:31:1 to CCCC:DDDD,
  it can be done in platform like this

  For 0:31:3, update HdaPreMemConfig->SubSystemIds policy with custom SSID and SVID
  Setting mSsidTablePtr[EntryCount].SvidSidValue.SubSystemVendorId to 0xFFFE will program SSID with DID value
*/
/*

  EntryCount = 0;
  mSsidTablePtr[EntryCount].Address.Bits.Device   = P2SBSOCDeviceNumber (); //Example 2: mSsidTablePtr[EntryCount].Address.Bits.Device   = ThcDevNumber (0);
  mSsidTablePtr[EntryCount].Address.Bits.Function = P2SBSOCFunctionNumber (); // Example 2: mSsidTablePtr[EntryCount].Address.Bits.Function = ThcFuncNumber (0);
  mSsidTablePtr[EntryCount].SvidSidValue.SubSystemVendorId = 0xAAAA;
  mSsidTablePtr[EntryCount].SvidSidValue.SubSystemId       = 0xBBBB;
  EntryCount ++;
  mSsidTablePtr[EntryCount].Address.Bits.Device   = PMCSOCDevNumber (); //Example 2: mSsidTablePtr[EntryCount].Address.Bits.Device   = ThcDevNumber (1);
  mSsidTablePtr[EntryCount].Address.Bits.Function = PMCSOCFuncNumber (); // Example 2: mSsidTablePtr[EntryCount].Address.Bits.Function = ThcFuncNumber (1);
  mSsidTablePtr[EntryCount].SvidSidValue.SubSystemVendorId = 0xCCCC;
  mSsidTablePtr[EntryCount].SvidSidValue.SubSystemId       = 0xDDDD;
  EntryCount ++;
*/
  ASSERT (EntryCount < SI_MAX_DEVICE_COUNT);

  // WA: UPDATE_POLICY () func fails to update pointer , as SiSsidTablePtr is different type in configblock and UPD
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.SiSkipSsidProgramming, SiConfig->SkipSsidProgramming, (UINT8) FALSE);
#if FixedPcdGet8(PcdFspModeSelection) == 0
  SiConfig->SsidTablePtr = (UINT32 *)mSsidTablePtr;
#endif
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.SiNumberOfSsidTableEntry, SiConfig->NumberOfSsidTableEntry, (UINT16) EntryCount);

}

/**
  This function performs Silicon PEI Policy initialization.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
UpdatePeiSiPolicy (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  EFI_STATUS                         Status;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                               *FspsUpd;
  DEBUG_EVENT_HANDLER_PPI            *DebugEventHandlerPpi;
#endif
  SI_CONFIG                          *SiConfig;

  DEBUG ((DEBUG_INFO, "Update PeiSiPolicyUpdate Pos-Mem Start\n"));

  Status = EFI_SUCCESS;
  SiConfig        = NULL;
#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#else
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSiConfigGuid, (VOID *) &SiConfig);
  ASSERT_EFI_ERROR (Status);
#endif
  UpdateSsidPolicy (
    SiConfig
    );
#if FixedPcdGet8(PcdFspModeSelection) == 1
  Status = PeiServicesLocatePpi (
           &gDebugEventHandlerPpiGuid,       // GUID
           0,                                // INSTANCE
           NULL,                             // EFI_PEI_PPI_DESCRIPTOR
           (VOID **) &DebugEventHandlerPpi   // PPI
           );
  if (!EFI_ERROR(Status)) {
    ((FSPS_UPD *) FspsUpd)->FspsArchUpd.FspEventHandler = (EFI_PHYSICAL_ADDRESS)(UINTN) DebugEventHandlerPpi->DebugEventHandler;
  }
#endif

#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.SiSkipBiosDoneWhenFwUpdate, SiConfig->SkipBiosDoneWhenFwUpdate, (UINT8) TRUE);
#elif (FixedPcdGetBool (PcdTopSwapSmiSupport) == 0x0)
  if (IsBiosUpdateRequired ()) {
    DEBUG ((DEBUG_INFO, "Skip BiosDone MSR for BIOS Update Process\n"));
    UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.SiSkipBiosDoneWhenFwUpdate, SiConfig->SkipBiosDoneWhenFwUpdate, (UINT8) TRUE);
  }
#endif

  return EFI_SUCCESS;
}
