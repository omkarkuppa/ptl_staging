/**@file
  PCH Setup Routines.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#include <Protocol/DevicePath.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/IdeControllerInit.h>
#include <SetupPrivate.h>
#include "PlatformBoardId.h"
#include <Library/SiFviLib.h>
#include "OemSetup.h"
#include <Library/GbeLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/PchPciBdfLib.h>
#include <Protocol/Spi.h>
#include <Protocol/Smbios.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchInfoHob.h>
#include <Library/CnviLib.h>
#include <CnvVfrSetupMenuHii.h>
#include <ChipsetInfoHob.h>
#include <PcieRegs.h>
#include <Register/PchRegs.h>
#include <Register/PchPcieRpRegs.h>
#include <PchPcieRpInfo.h>
#include <Register/FlashRegs.h>
#include <Register/FlashRegsVer2.h>
#include <Library/PmcLib.h>
#include <Library/PcdInfoLib.h>
#include <Register/PmcRegs.h>
//
// Print primitives
//
#define LEFT_JUSTIFY  0x01
#define PREFIX_SIGN   0x02
#define PREFIX_BLANK  0x04
#define COMMA_TYPE    0x08
#define LONG_TYPE     0x10
#define PREFIX_ZERO   0x20

#define DXE_DEVICE_DISABLED                     0
#define DXE_DEVICE_ENABLED                      1

//
// Length of temp string buffer to store value string.
//
#define CHARACTER_NUMBER_FOR_VALUE              30
#define _48_BIT_ADDRESS_FEATURE_SET_SUPPORTED   0x0400
#define ATAPI_DEVICE                            0x8000

VOID SwapEntries (
  IN CHAR8    *Data,
  IN UINT16   Size
  )
{
  UINT16  Index;
  CHAR8   Temp8;

  for (Index = 0; (Index+1) < Size; Index+=2) {
    Temp8           = Data[Index];
    Data[Index]     = Data[Index + 1];
    Data[Index + 1] = Temp8;
  }
}

VOID
DisplaySpiInformation (
  EFI_HII_HANDLE HiiHandle
  )
{
  UINT32      Flcomp;
  UINT32      FlashComponents;
  UINT32      Signature;
  UINT32      Data32;
  EFI_STATUS  Status;
  UINTN       VariableSize;
  SETUP_DATA  SetupData;
  UINT32      SetupAttr;

  SetupAttr = 0;
  VariableSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  &SetupAttr,
                  &VariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    SetupAttr = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
  }
  ASSERT_EFI_ERROR (Status);

  // Read Descriptor offset 0x10 - To get Descriptor Signature
  Signature = SpiGetDescriptorSignature ();
  DEBUG ((DEBUG_INFO, "\nSignature = 0x%.8x\n", Signature));

  // Read Descriptor offset 0x30 - To get supported features and R/W frequencies
  Flcomp = SpiGetFlashComponentDescription ();
  DEBUG ((DEBUG_INFO, "Flcomp = 0x%.8x\n", Flcomp));

  // Read Descriptor offset 0x14 - To get number of components
  FlashComponents = SpiGetFlashComponentsNumber ();
  DEBUG ((DEBUG_INFO, "FlashComponents = 0x%2x\n", FlashComponents));

  //
  // Dual Output Fast Read support
  //
  if (Flcomp & B_FLASH_FLCOMP_FR_SUP) {
    InitString (HiiHandle, STRING_TOKEN (STR_SPI_DUAL_OUTPUT_FAST_READ_SUPPORT_VALUE), L"%a", "Supported");
  } else {
    InitString (HiiHandle, STRING_TOKEN (STR_SPI_DUAL_OUTPUT_FAST_READ_SUPPORT_VALUE), L"%a", "Not supported");
  }

  //
  // Read ID and Read Status Clock Frequency [27:29]
  //
  Data32 = ((Flcomp & B_FLASH_FLCOMP_RIDS_FREQ) >> N_FLASH_FLCOMP_RIDS_FREQ);

  switch (Data32) {
    case V_FLASH_FLCOMP_FREQ_100MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_READ_ID_STATUS_CLOCK_FREQUENCY_VALUE), L"%a", "100 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_50MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_READ_ID_STATUS_CLOCK_FREQUENCY_VALUE), L"%a", "50 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_40MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_READ_ID_STATUS_CLOCK_FREQUENCY_VALUE), L"%a", "40 MHz (Not recommended)");
      break;
    case V_FLASH_FLCOMP_FREQ_33MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_READ_ID_STATUS_CLOCK_FREQUENCY_VALUE), L"%a", "33 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_25MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_READ_ID_STATUS_CLOCK_FREQUENCY_VALUE), L"%a", "25 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_14MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_READ_ID_STATUS_CLOCK_FREQUENCY_VALUE), L"%a", "14 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_76_8MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_READ_ID_STATUS_CLOCK_FREQUENCY_VALUE), L"%a", "76.8 MHz");
      break;
    default:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_READ_ID_STATUS_CLOCK_FREQUENCY_VALUE), L"%a", "Invalid Setting");
      break;
  }

  //
  // Write and Erase Clock Frequency [24:26]
  //
  Data32 = ((Flcomp & B_FLASH_FLCOMP_WE_FREQ) >> N_FLASH_FLCOMP_WE_FREQ);

  switch (Data32) {
    case V_FLASH_FLCOMP_FREQ_100MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_WRITE_ERASE_CLOCK_FREQUENCY_VALUE), L"%a", "100 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_50MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_WRITE_ERASE_CLOCK_FREQUENCY_VALUE), L"%a", "50 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_40MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_WRITE_ERASE_CLOCK_FREQUENCY_VALUE), L"%a", "40 MHz (Not recommended)");
      break;
    case V_FLASH_FLCOMP_FREQ_33MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_WRITE_ERASE_CLOCK_FREQUENCY_VALUE), L"%a", "33 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_25MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_WRITE_ERASE_CLOCK_FREQUENCY_VALUE), L"%a", "25 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_14MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_WRITE_ERASE_CLOCK_FREQUENCY_VALUE), L"%a", "14 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_76_8MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_WRITE_ERASE_CLOCK_FREQUENCY_VALUE), L"%a", "76.8 MHz");
      break;
    default:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_WRITE_ERASE_CLOCK_FREQUENCY_VALUE), L"%a", "Invalid Setting");
      break;
  }

  //
  // Fast Read Clock Frequency [21:23]
  //
  Data32 = ((Flcomp & B_FLASH_FLCOMP_FRCF_FREQ) >> N_FLASH_FLCOMP_FRCF_FREQ);

  switch (Data32) {
    case V_FLASH_FLCOMP_FREQ_100MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_FAST_READ_CLOCK_FREQUENCY_VALUE), L"%a", "100 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_50MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_FAST_READ_CLOCK_FREQUENCY_VALUE), L"%a", "50 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_40MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_FAST_READ_CLOCK_FREQUENCY_VALUE), L"%a", "40 MHz (Not recommended)");
      break;
    case V_FLASH_FLCOMP_FREQ_33MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_FAST_READ_CLOCK_FREQUENCY_VALUE), L"%a", "33 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_25MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_FAST_READ_CLOCK_FREQUENCY_VALUE), L"%a", "25 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_14MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_FAST_READ_CLOCK_FREQUENCY_VALUE), L"%a", "14 MHz");
      break;
    case V_FLASH_FLCOMP_FREQ_76_8MHZ:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_FAST_READ_CLOCK_FREQUENCY_VALUE), L"%a", "76.8 MHz");
      break;
    default:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_FAST_READ_CLOCK_FREQUENCY_VALUE), L"%a", "Invalid Setting");
      break;
  }

  //
  // Fast Read support [20]
  //
  if (Flcomp & B_FLASH_FLCOMP_FR_SUP) {
    InitString (HiiHandle, STRING_TOKEN (STR_SPI_FAST_READ_SUPPORT_VALUE), L"%a", "Supported");
  } else {
    InitString (HiiHandle, STRING_TOKEN (STR_SPI_FAST_READ_SUPPORT_VALUE), L"%a", "Not supported");
  }

  //
  // Number of components
  //
  switch (FlashComponents) {
    case 0:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_NUMBER_OF_COMPONENTS_VALUE), L"%a", "1 Component");
      SetupData.TwoComponents = 0;
      break;
    case 1:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_NUMBER_OF_COMPONENTS_VALUE), L"%a", "2 Components");
      SetupData.TwoComponents = 1;
      break;
    default:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI_NUMBER_OF_COMPONENTS_VALUE), L"%a", "Reserved");
      break;
  }
  Status = gRT->SetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  SetupAttr,
                  sizeof (SETUP_DATA),
                  &SetupData
                  );
  ASSERT_EFI_ERROR (Status);
  //
  // Display SPI Component 1 Density
  //

  // Execute if there are 2 components
  if (FlashComponents == 1){
    switch (Flcomp & 0xF0) {
      case 0:
        InitString (HiiHandle, STRING_TOKEN (STR_SPI1_DENSITY_VALUE), L"%a", "512 KB");
        break;
      case 1:
        InitString (HiiHandle, STRING_TOKEN (STR_SPI1_DENSITY_VALUE), L"%a", "1 MB");
        break;
      case 2:
        InitString (HiiHandle, STRING_TOKEN (STR_SPI1_DENSITY_VALUE), L"%a", "2 MB");
        break;
      case 3:
        InitString (HiiHandle, STRING_TOKEN (STR_SPI1_DENSITY_VALUE), L"%a", "4 MB");
        break;
      case 4:
        InitString (HiiHandle, STRING_TOKEN (STR_SPI1_DENSITY_VALUE), L"%a", "8 MB");
        break;
      case 5:
        InitString (HiiHandle, STRING_TOKEN (STR_SPI1_DENSITY_VALUE), L"%a", "16 MB");
        break;
      case 6:
        InitString (HiiHandle, STRING_TOKEN (STR_SPI1_DENSITY_VALUE), L"%a", "32 MB");
        break;
      case 7:
        InitString (HiiHandle, STRING_TOKEN (STR_SPI1_DENSITY_VALUE), L"%a", "64 MB");
        break;
      case 8:
      case 9:
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
        InitString (HiiHandle, STRING_TOKEN (STR_SPI1_DENSITY_VALUE), L"%a", "Reserved");
        break;
      default:
        InitString (HiiHandle, STRING_TOKEN (STR_SPI1_DENSITY_VALUE), L"%a", "Not present");
        break;
    }
  }
  //
  // Display SPI Component 0 Density
  //
  switch (Flcomp & 0x0F) {
    case 0:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI0_DENSITY_VALUE), L"%a", "512 KB");
      break;
    case 1:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI0_DENSITY_VALUE), L"%a", "1 MB");
      break;
    case 2:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI0_DENSITY_VALUE), L"%a", "2 MB");
      break;
    case 3:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI0_DENSITY_VALUE), L"%a", "4 MB");
      break;
    case 4:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI0_DENSITY_VALUE), L"%a", "8 MB");
      break;
    case 5:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI0_DENSITY_VALUE), L"%a", "16 MB");
      break;
    case 6:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI0_DENSITY_VALUE), L"%a", "32 MB");
      break;
    case 7:
      InitString (HiiHandle, STRING_TOKEN (STR_SPI0_DENSITY_VALUE), L"%a", "64 MB");
      break;
    default:
      break;
  }
}

VOID
BiosIshDataPass (
  VOID
  )
{
  static CONST CHAR8      FileName[] = "bios2ish";

  DEBUG ((DEBUG_INFO, "Sending PDT Unlock Message\n"));
  HeciPdtUnlockMsg (FileName);

}

BOOLEAN
IsPciePortShadowed (
  UINT32       Index,
  PCH_INFO_HOB *PchInfoHob
  )
{
  UINT32 Controller;
  UINT32 Port;

  Controller = Index / PCH_PCIE_CONTROLLER_PORTS;
  Port = Index % PCH_PCIE_CONTROLLER_PORTS;

  if (Index <= 9) {
    if ((Index == 9) && (PchInfoHob->PcieControllerBifurcation[Controller] == V_PCH_PCIE_CFG_STRPFUSECFG_RPC_4)) {
      return TRUE;
    } else if ((Port == 1 && Index != 9) && (PchInfoHob->PcieControllerBifurcation[Controller] != V_PCH_PCIE_CFG_STRPFUSECFG_RPC_1_1_1_1)) {
      return TRUE;
    } else if (Port == 2 && ((PchInfoHob->PcieControllerBifurcation[Controller] == V_PCH_PCIE_CFG_STRPFUSECFG_RPC_4))) {
      return TRUE;
    } else if ((Port == 3) && ((PchInfoHob->PcieControllerBifurcation[Controller] == V_PCH_PCIE_CFG_STRPFUSECFG_RPC_4) ||
                              (PchInfoHob->PcieControllerBifurcation[Controller] == V_PCH_PCIE_CFG_STRPFUSECFG_RPC_2_2))) {
      return TRUE;
    }
  } else {
    if (Index == 11 && PchInfoHob->FiaLos == 0x1) {
      return TRUE;
    }
  }
  return FALSE;
}

STATIC
BOOLEAN
PchSetupHdAudio (
  OUT SETUP_VOLATILE_DATA*  SetupVolatileData,
  OUT PCH_SETUP             *PchSetup
  )
{
  UINT32 Index;
  VOID                          *HobPtr;
  PCH_INFO_HOB                  *PchInfoHob;
  BOOLEAN                       UpdatePchSetup;
  CNV_VFR_CONFIG_SETUP          CnvSetup;
  UINTN                         VarDataSize;
  EFI_STATUS                    Status;

  UpdatePchSetup = FALSE;

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  if (HobPtr == NULL) {
    ASSERT (FALSE);
    return FALSE;
  }
  PchInfoHob = (PCH_INFO_HOB*) GET_GUID_HOB_DATA (HobPtr);

  SetupVolatileData->AudioDspFusedOut = (UINT8) PchInfoHob->AudioDspFusedOut;

  SetupVolatileData->AudioHdaLinkSupported = (UINT8) PchInfoHob->HdaLinkSupport;

  for (Index = 0; Index < GetPchHdaMaxDmicLinkNum (); Index++) {
    SetupVolatileData->AudioDmicLinkSupported[Index] = PchInfoHob->DmicLinkSupport[Index];
  }

  for (Index = 0; Index < GetPchHdaMaxSspLinkNum (); Index++) {
    SetupVolatileData->AudioSspLinkSupported[Index] = PchInfoHob->SspLinkSupport[Index];
  }

  for (Index = 0; Index < GetPchHdaMaxSndwLinkNum (); Index++) {
    SetupVolatileData->AudioSndwLinkSupported[Index] = PchInfoHob->SndwLinkSupport[Index];
  }

  for (Index = 0; Index < GetPchHdaMaxSndwMultilaneNum (); Index++) {
    SetupVolatileData->AudioSndwMultilaneSupported[Index] = PchInfoHob->SndwMultilaneSupport[Index];
  }
  VarDataSize = sizeof (CNV_VFR_CONFIG_SETUP);
  ZeroMem (&CnvSetup, VarDataSize);
#if FixedPcdGetBool (PcdCnvFeatureEnable) == 1
  Status = gRT->GetVariable (
                  L"CnvSetup",
                  &gCnvFeatureSetupGuid,
                  NULL,
                  &VarDataSize,
                  &CnvSetup
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
#endif

  // if BT is not present or disabled, disable DSP BT related knobs
  if ((!CnviCrfModuleIsPresent () || CnvSetup.CnviMode == 0) && !PchSetup->HdaDiscBtOffEnabled) {
    // BT Intel HFP SCO
    PchSetup->PchHdAudioFeature[5] = FALSE;
    // BT Intel A2DP
    PchSetup->PchHdAudioFeature[6] = FALSE;
    // BT Intel LE
    PchSetup->PchHdAudioFeature[9] = FALSE;
    UpdatePchSetup = TRUE;
  }

  return UpdatePchSetup;
}

STATIC
VOID
PchSetupCrid (
  OUT SETUP_VOLATILE_DATA*  SetupVolatileData
  )
{
  VOID          *HobPtr;
  PCH_INFO_HOB  *PchInfoHob;

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  if (HobPtr == NULL) {
    ASSERT (FALSE);
    return;
  }
  PchInfoHob = (PCH_INFO_HOB*) GET_GUID_HOB_DATA (HobPtr);
  SetupVolatileData->PchCridSupport = (UINT8) PchInfoHob->CridSupport;
}

STATIC
VOID
PchSetupScs (
  OUT SETUP_VOLATILE_DATA*  SetupVolatileData
  )
{
  UINT8  UfsIndex;

  for (UfsIndex = 0; UfsIndex < PchGetMaxUfsNum (); UfsIndex++) {
    SetupVolatileData->UfsSupported[UfsIndex] = PmcIsScsUfsSupported (UfsIndex);
  }
}

STATIC
VOID
PchSetupClock (
  OUT SETUP_VOLATILE_DATA *SetupVolatileData
)
{
  SetupVolatileData->MaxPcieClockNumber = GetPchMaxPcieClockNum ();
}

STATIC
VOID
PchSetupPcie (
  IN  EFI_HII_HANDLE        HiiHandle,
  OUT SETUP_VOLATILE_DATA*  SetupVolatileData
  )
{
  UINT32        Index;
  VOID          *HobPtr;
  PCH_INFO_HOB  *PchInfoHob;

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  ASSERT (HobPtr != NULL);
  if (HobPtr == NULL) {
    return;
  }
  PchInfoHob = (PCH_INFO_HOB*) GET_GUID_HOB_DATA (HobPtr);
  ASSERT (PchInfoHob != NULL);

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index ++) {
    if (PchInfoHob->PciePortFuses & (BIT0 << Index)) {
      SetupVolatileData->PciePortCfg[Index] = PCH_RP_FUSED_OFF;
    } else if ((PchInfoHob->PciePortLaneEnabled & (BIT0 << Index)) == 0) {
      SetupVolatileData->PciePortCfg[Index] = PCH_RP_DIFFERENT_BUS;
    } else if (IsPciePortShadowed (Index, PchInfoHob)) {
      SetupVolatileData->PciePortCfg[Index] = PCH_RP_MERGED;
    } else {
      SetupVolatileData->PciePortCfg[Index] = PCH_RP_AVAILABLE;
    }
  }

  for (Index = GetPchMaxPciePortNum (); Index < ARRAY_SIZE (SetupVolatileData->PciePortCfg); Index ++) {
    SetupVolatileData->PciePortCfg[Index] = PCH_RP_NOT_IMPLEMENTED;
  }

  for (Index = 0; Index < ARRAY_SIZE (SetupVolatileData->PciePortCfg); Index ++) {
    DEBUG ((DEBUG_INFO, "VolatileData.PciePortCfg[%d] = %d\n", Index, SetupVolatileData->PciePortCfg[Index]));
  }

}

/*
  Update SETUP_VOLATILE_DATA PCH USB related data
*/
STATIC
VOID
PchSetupUsb (
  OUT SETUP_VOLATILE_DATA  *SetupVolatileData
  )
{
  //
  // USB Ports Count update
  //
  SetupVolatileData->PchUsb2PortCount = PtlPcdGetUsb2MaxPhysicalPortNum ();
  SetupVolatileData->PchUsb3PortCount = PtlPcdGetXhciMaxUsb3PortNum ();
}

STATIC
VOID
PchSetupGbe (
  OUT SETUP_VOLATILE_DATA  *SetupVolatileData
  )
{
  VOID          *HobPtr;
  PCH_INFO_HOB  *PchInfoHob;

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  ASSERT (HobPtr != NULL);
  if (HobPtr == NULL) {
    return;
  }
  PchInfoHob = (PCH_INFO_HOB*) GET_GUID_HOB_DATA (HobPtr);
  ASSERT (PchInfoHob != NULL);

  SetupVolatileData->GbeSupportByPch = (UINT8) PchInfoHob->GbeSupport;
  SetupVolatileData->GbeAvailable = IsGbePresent (PmcGetPwrmBase ());
}

#if FixedPcdGet8(PcdTsnSupport) == 0x1
STATIC
VOID
PchSetupTsn (
  OUT SETUP_VOLATILE_DATA  *SetupVolatileData
  )
{
  SetupVolatileData->TsnSupportByPch = FALSE;
  SetupVolatileData->TsnAvailable = FALSE;
}
#endif

STATIC
VOID
PchSetupThc (
  OUT SETUP_VOLATILE_DATA  *SetupVolatileData
  )
{
  VOID          *HobPtr;
  PCH_INFO_HOB  *PchInfoHob;

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  ASSERT (HobPtr != NULL);
  if (HobPtr == NULL) {
    return;
  }
  PchInfoHob = (PCH_INFO_HOB*) GET_GUID_HOB_DATA (HobPtr);
  ASSERT (PchInfoHob != NULL);
  SetupVolatileData->ThcSupported = (UINT8) PchInfoHob->ThcSupport;
  SetupVolatileData->ThcAvailable = PchInfoHob->Thc0Strap;
}

STATIC
VOID
PchSetupIsh (
  OUT SETUP_VOLATILE_DATA  *SetupVolatileData
  )
{
  VOID          *HobPtr;
  PCH_INFO_HOB  *PchInfoHob;

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  ASSERT (HobPtr != NULL);
  if (HobPtr == NULL) {
    return;
  }
  PchInfoHob = (PCH_INFO_HOB*) GET_GUID_HOB_DATA (HobPtr);
  ASSERT (PchInfoHob != NULL);

  SetupVolatileData->IshAvailable = (UINT8)PchInfoHob->IshAvailable;
}

STATIC
VOID
PchSetupPSOn (
  OUT SETUP_VOLATILE_DATA  *SetupVolatileData
  )
{
  VOID          *HobPtr;
  PCH_INFO_HOB  *PchInfoHob;

  HobPtr = GetFirstGuidHob (&gPchInfoHobGuid);
  ASSERT (HobPtr != NULL);
  if (HobPtr == NULL) {
    return;
  }
  PchInfoHob = (PCH_INFO_HOB*) GET_GUID_HOB_DATA (HobPtr);
  ASSERT (PchInfoHob != NULL);

  SetupVolatileData->PSOnSupported = (UINT8) PchInfoHob->PSOnSupport;
}

STATIC
VOID
UpdateChipsetInitInfo (
  IN EFI_HII_HANDLE  HiiHandle
  )
{
  UINT64             PmcSsramBar0;
  BOOLEAN            IsBarInit;
  UINT16             SavedStsCmd;

  IsBarInit   = FALSE;
  SavedStsCmd = 0;
  if (IsPmcSsramDeviceEnabled ()) {
    //
    // Read Chipset init version from PMC Shared SRAM
    //
    IsBarInit = InitPmcSsramMmioBar (&SavedStsCmd);
    PmcSsramBar0 = GetPmcSsramMmioBar ();

    InitString (
      HiiHandle,
      STRING_TOKEN (STR_CHIP_HSIO_INTEL_VER_VALUE),
      L"%d.%d.%x.%d",
      MmioRead16 (PmcSsramBar0 + R_PMC_MEM_SSRAM_INTEL_HSIO_VERSION_MAJOR),
      MmioRead16 (PmcSsramBar0 + R_PMC_MEM_SSRAM_INTEL_HSIO_VERSION_MINOR),
      MmioRead16 (PmcSsramBar0 + R_PMC_MEM_SSRAM_INTEL_HSIO_VERSION_HOT_FIX),
      MmioRead16 (PmcSsramBar0 + R_PMC_MEM_SSRAM_INTEL_HSIO_VERSION_BUILD)
      );
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_CHIP_HSIO_OEM_VER_VALUE),
      L"%d.%d.%x.%d",
      MmioRead16 (PmcSsramBar0 + R_PMC_MEM_SSRAM_OEM_HSIO_VERSION_MAJOR),
      MmioRead16 (PmcSsramBar0 + R_PMC_MEM_SSRAM_OEM_HSIO_VERSION_MINOR),
      MmioRead16 (PmcSsramBar0 + R_PMC_MEM_SSRAM_OEM_HSIO_VERSION_HOT_FIX),
      MmioRead16 (PmcSsramBar0 + R_PMC_MEM_SSRAM_OEM_HSIO_VERSION_BUILD)
      );

    ClearPmcSsramMmioBar (SavedStsCmd, IsBarInit);
  }
}

STATIC
VOID
UpdateChipsetInfo (
  IN EFI_HII_HANDLE  HiiHandle
  )
{
  CHAR8   Buffer[PCH_STEPPING_STR_LENGTH_MAX];

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_CHIP_SB_VALUE),
    L"%a",
    PchGetSeriesStr ()
    );

  PchGetSteppingStr (Buffer, sizeof (Buffer));
  InitString (
    HiiHandle,
    STRING_TOKEN(STR_CHIP_SB_REV_VALUE),
    L"%a",
    Buffer
    );

  InitString (
    HiiHandle,
    STRING_TOKEN(STR_CHIP_SB_SKU_VALUE),
    L"%a",
    PchGetSkuStr ()
    );
}

VOID
InitSBStrings (
  EFI_HII_HANDLE HiiHandle,
  UINT16         Class
  )
{
  EFI_STATUS           Status;
  UINTN                VariableSize;
  UINT32               PchSetupAttr;
  PCH_SETUP            PchSetup;
  SETUP_VOLATILE_DATA  VolatileData;
  UINT32               SetupVolVarAttr;
  BOOLEAN              UpdatePchSetup;

  if (Class == ADVANCED_FORM_SET_CLASS) {
    DEBUG ((DEBUG_INFO, "<InitSBStrings>"));

    UpdatePchSetup = FALSE;

    VariableSize = sizeof (SETUP_VOLATILE_DATA);
    Status = gRT->GetVariable (
                    L"SetupVolatileData",
                    &gSetupVariableGuid,
                    &SetupVolVarAttr,
                    &VariableSize,
                    &VolatileData
                    );
    ASSERT_EFI_ERROR (Status);

    VariableSize = sizeof (PCH_SETUP);
    Status = gRT->GetVariable (
                    L"PchSetup",
                    &gPchSetupVariableGuid,
                    &PchSetupAttr,
                    &VariableSize,
                    &PchSetup
                    );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return;
    }

    //
    // Update SetupVolatileData and PchSetup
    //
    PchSetupPcie (HiiHandle, &VolatileData);
    PchSetupUsb (&VolatileData);
    PchSetupGbe (&VolatileData);
#if FixedPcdGet8(PcdTsnSupport) == 0x1
    PchSetupTsn (&VolatileData);
#endif
    PchSetupScs (&VolatileData);
    UpdatePchSetup |= PchSetupHdAudio (&VolatileData, &PchSetup);
    PchSetupThc (&VolatileData);
    PchSetupIsh (&VolatileData);
    PchSetupPSOn (&VolatileData);
    PchSetupCrid (&VolatileData);
    PchSetupClock (&VolatileData);
    VolatileData.PchSeries = PchSeries ();

    // Send PDT Unlock Message to ISH
    if (PchSetup.PchIshPdtUnlock == 1) {
      BiosIshDataPass ();
      //Set the value back to 0 so the user needs to manually enable the option to send the next PDT unlock message
      PchSetup.PchIshPdtUnlock = 0;
      UpdatePchSetup = TRUE;
    }

    //
    // Update PchSetup if needed
    //
    if (UpdatePchSetup == TRUE) {
      Status = gRT->SetVariable (
                      L"PchSetup",
                      &gPchSetupVariableGuid,
                      PchSetupAttr,
                      sizeof (PCH_SETUP),
                      &PchSetup
                      );
      ASSERT_EFI_ERROR (Status);
    }

    Status = gRT->SetVariable (
                    L"SetupVolatileData",
                    &gSetupVariableGuid,
                    SetupVolVarAttr,
                    sizeof (SETUP_VOLATILE_DATA),
                    &VolatileData
                    );
    ASSERT_EFI_ERROR (Status);

  } // ADVANCED_FORM_SET_CLASS

  if (Class == MAIN_FORM_SET_CLASS) {
    DEBUG ((DEBUG_INFO, "<InitSBStrings>"));

    UpdateChipsetInfo (HiiHandle);
    UpdateChipsetInitInfo (HiiHandle);

    InitString (
      HiiHandle,
      STRING_TOKEN (STR_CHIP_SB_PACKAGE_VALUE),
      L"%a",
      "Not Implemented Yet"
      );

    //
    // SPI controller information
    //
    DisplaySpiInformation(HiiHandle);
  } // MAIN_FORM_SET_CLASS
}
