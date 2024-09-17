/** @file
  The file contains SA related setup options

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

#include <SetupPrivate.h>
#include "OemSetup.h"
#include "SaSetup.h"
#include "PlatformBoardId.h"
#include "CpuRegs.h"
#include "DimmInfo.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/CpuPlatformLib.h>
#include <Protocol/IGpuPolicy.h>
#include <Protocol/SmbusHc.h>
#include <Protocol/MemInfo.h>
#include <Protocol/GopPolicy.h>
#include <VtioSetup.h>
#if FixedPcdGetBool(PcdTcssSupport) == 1
#include "TcssDataHob.h"
#endif
#include <IpuDataHob.h>
#include <Register/SaPcieDmiRegs.h>
#include <Library/PchPciBdfLib.h>
#include <Library/NpuInfoLib.h>
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
#include <DptfConfig.h>
#endif
#include <Library/HostBridgeInfoLib.h>
#include <Library/IaxInfoLib.h>
#include <Library/IGpuInfoLib.h>
#include <Defines/HostBridgeDefines.h>

static EFI_HII_HANDLE     gHiiHandle;
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
GLOBAL_REMOVE_IF_UNREFERENCED DPTF_CONFIG          mDptfSetupData;
#endif
#include <VmdInfoHob.h>
#include <Library/VmdInfoLib.h>
#include <Library/PcdInfoLib.h>
#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 1
#include <MipiCamConfig.h>
#endif

EFI_STATUS
EFIAPI
UpdateVmdInfo (
  EFI_HII_HANDLE HiiHandle
  );

#define NB_MIN(a, b)  (((a) < (b)) ? (a) : (b))

EFI_STRING_ID  DimmSizeString[SLOT_NUM] = {
  STRING_TOKEN (STR_DIMM_SIZE_MC0_CH0_SLT0_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC0_CH0_SLT1_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC0_CH1_SLT0_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC0_CH1_SLT1_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC0_CH2_SLT0_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC0_CH2_SLT1_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC0_CH3_SLT0_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC0_CH3_SLT1_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC1_CH0_SLT0_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC1_CH0_SLT1_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC1_CH1_SLT0_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC1_CH1_SLT1_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC1_CH2_SLT0_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC1_CH2_SLT1_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC1_CH3_SLT0_VALUE),
  STRING_TOKEN (STR_DIMM_SIZE_MC1_CH3_SLT1_VALUE)
};

EFI_STRING_ID  RankInDimmString[SLOT_NUM] = {
  STRING_TOKEN (STR_DIMM_MC0_CH0_SLT0_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH0_SLT1_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH1_SLT0_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH1_SLT1_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH2_SLT0_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH2_SLT1_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH3_SLT0_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH3_SLT1_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH0_SLT0_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH0_SLT1_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH1_SLT0_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH1_SLT1_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH2_SLT0_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH2_SLT1_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH3_SLT0_RANK_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH3_SLT1_RANK_VALUE)
};

EFI_STRING_ID  DimmMfgString[SLOT_NUM] = {
  STRING_TOKEN (STR_DIMM_MC0_CH0_SLT0_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH0_SLT1_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH1_SLT0_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH1_SLT1_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH2_SLT0_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH2_SLT1_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH3_SLT0_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC0_CH3_SLT1_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH0_SLT0_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH0_SLT1_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH1_SLT0_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH1_SLT1_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH2_SLT0_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH2_SLT1_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH3_SLT0_MFG_VALUE),
  STRING_TOKEN (STR_DIMM_MC1_CH3_SLT1_MFG_VALUE)
};

EFI_STRING_ID  DimmStatusString[SLOT_NUM] = {
  STRING_TOKEN (STR_DIMM_MC0_CH0_SLT0_STATUS),
  STRING_TOKEN (STR_DIMM_MC0_CH0_SLT1_STATUS),
  STRING_TOKEN (STR_DIMM_MC0_CH1_SLT0_STATUS),
  STRING_TOKEN (STR_DIMM_MC0_CH1_SLT1_STATUS),
  STRING_TOKEN (STR_DIMM_MC0_CH2_SLT0_STATUS),
  STRING_TOKEN (STR_DIMM_MC0_CH2_SLT1_STATUS),
  STRING_TOKEN (STR_DIMM_MC0_CH3_SLT0_STATUS),
  STRING_TOKEN (STR_DIMM_MC0_CH3_SLT1_STATUS),
  STRING_TOKEN (STR_DIMM_MC1_CH0_SLT0_STATUS),
  STRING_TOKEN (STR_DIMM_MC1_CH0_SLT1_STATUS),
  STRING_TOKEN (STR_DIMM_MC1_CH1_SLT0_STATUS),
  STRING_TOKEN (STR_DIMM_MC1_CH1_SLT1_STATUS),
  STRING_TOKEN (STR_DIMM_MC1_CH2_SLT0_STATUS),
  STRING_TOKEN (STR_DIMM_MC1_CH2_SLT1_STATUS),
  STRING_TOKEN (STR_DIMM_MC1_CH3_SLT0_STATUS),
  STRING_TOKEN (STR_DIMM_MC1_CH3_SLT1_STATUS)
};

EFI_STRING_ID  IbeccSizeString[IBECC_SIZE_NUM] = {
  STRING_TOKEN (STR_32MB_IBECC_STRING),
  STRING_TOKEN (STR_64MB_IBECC_STRING),
  STRING_TOKEN (STR_128MB_IBECC_STRING),
  STRING_TOKEN (STR_256MB_IBECC_STRING),
  STRING_TOKEN (STR_512MB_IBECC_STRING),
  STRING_TOKEN (STR_1GB_IBECC_STRING),
  STRING_TOKEN (STR_2GB_IBECC_STRING),
  STRING_TOKEN (STR_4GB_IBECC_STRING),
  STRING_TOKEN (STR_8GB_IBECC_STRING),
  STRING_TOKEN (STR_16GB_IBECC_STRING),
  STRING_TOKEN (STR_32GB_IBECC_STRING),
  STRING_TOKEN (STR_64GB_IBECC_STRING),
  STRING_TOKEN (STR_128GB_IBECC_STRING),
  STRING_TOKEN (STR_256GB_IBECC_STRING)
};

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 1
EFI_STATUS
UpdateIpuSetupConfig (
  IN UINT8  IsIpuSupported
  )
{
  UINTN                         VarSize;
  UINT32                        VariableAttr;
  MIPICAM_CONFIG                MipiSetupData;
  EFI_STATUS                    Status;

  Status = EFI_SUCCESS;
  //
  // When IPU is supported, keep the setup value as-is.
  //
  if (IsIpuSupported == 1) {
    return Status;
  }
  DEBUG ((DEBUG_INFO, "%a(%d):%a -> <IPU is not Support, disable Camera>\n", __FILE__, __LINE__, __FUNCTION__));
  //
  // Get Setup Variable
  //
  VarSize = sizeof (MIPICAM_CONFIG);
  Status = gRT->GetVariable (
                  MIPICAM_CONFIG_VARIABLE_NAME,
                  &gMipiCamConfigVariableGuid,
                  &VariableAttr,
                  &VarSize,
                  &MipiSetupData
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  MipiSetupData.MipiCam_ControlLogic0 = 0;
  MipiSetupData.MipiCam_ControlLogic1 = 0;
  MipiSetupData.MipiCam_ControlLogic2 = 0;
  MipiSetupData.MipiCam_ControlLogic3 = 0;
  MipiSetupData.MipiCam_Link0 = 0;
  MipiSetupData.MipiCam_Link1 = 0;
  MipiSetupData.MipiCam_Link2 = 0;
  MipiSetupData.MipiCam_Link3 = 0;
  MipiSetupData.MipiCam_Link0_FlashDriverSelection = 0;
  MipiSetupData.MipiCam_Link1_FlashDriverSelection = 0;
  MipiSetupData.MipiCam_Link2_FlashDriverSelection = 0;
  //
  // Set Setup Variable
  //
  Status = gRT->SetVariable (
                  MIPICAM_CONFIG_VARIABLE_NAME,
                  &gMipiCamConfigVariableGuid,
                  VariableAttr,
                  VarSize,
                  &MipiSetupData
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;

}
#endif

VOID
InitSaStrings (
  EFI_HII_HANDLE HiiHandle,
  UINT16         Class
  )
{
  UINT8                           MemoryType;
  UINT8                           Profile;
  UINT64                          MemorySize;
  UINT16                          DdrFrequency;
  UINT16                          VendorId;
  EFI_STATUS                      Status;
  UINTN                           SaSetupVariableSize;
  UINTN                           SetupVolVariableSize;
  UINT32                          Value32;
  UINTN                           Slot;
  UINT8                           w;
  UINT8                           x;
  UINT8                           y;
  UINT8                           z;
  UINT16                          Data16;
  UINT8                           NodeIndex;
  UINT8                           ChannelIndex;
  UINT8                           DimmIndex;
  UINT8                           MfgIdIndex;
  UINT8                           SkipStringUpdate;
  SA_SETUP                        *pSaSetup;
  MEM_INFO_PROTOCOL               *MemInfoProtocol;
  MEMORY_INFO_DATA                *MemInfoData;
  MEMORY_TIMING                   *Timing;
  IGPU_POLICY_PROTOCOL            *IGpuPolicy;
  CHAR8                           *DimmStatusStr;
  UINT8                           Index;
  UINT32                          SaVariableAttributes;
  UINT32                          SetupVolAttributes;
  IGPU_DXE_CONFIG                 *IGpuDxeConfig;
  IPU_DATA_HOB                    *IpuDataHob;
  VMD_INFO_HOB                    *VmdInfoHob;
#if FixedPcdGetBool(PcdTcssSupport) == 1
  TCSS_DATA_HOB         *TcssHob;
#endif
  CHAR8                           *IbeccSizeStr;
  UINT32                          IbeccSize;
  UINT8                           SizeIndex;
  UINT8                           Mc0Enabled;
  UINT8                           Mc1Enabled;
  UINT8                           i;
  UINTN                           Size;
  UINT32                          VariableAttributes;



  VmdInfoHob = NULL;
  Index = 0;

  if ((Class != MAIN_FORM_SET_CLASS) && (Class != ADVANCED_FORM_SET_CLASS)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "<InitSaStrings>"));

  Status = gBS->LocateProtocol (&gIGpuPolicyProtocolGuid, NULL, (VOID **) &IGpuPolicy);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)IGpuPolicy, &gIGpuDxeConfigGuid, (VOID *)&IGpuDxeConfig);
  ASSERT_EFI_ERROR (Status);

  SaSetupVariableSize = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  &SaVariableAttributes,
                  &SaSetupVariableSize,
                  &mSaSetup
                  );
  DEBUG((DEBUG_INFO, "GetVariable 'SaSetup' Status = %r\n", Status));
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Get the Memory Info HOB Protocol if it exists.
  //
  Status = gBS->LocateProtocol (&gMemInfoProtocolGuid, NULL, (VOID **) &MemInfoProtocol);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  MemInfoData = &MemInfoProtocol->MemInfoData;

  MemoryType    = MemInfoData->DdrType;
  DdrFrequency  = MemInfoData->ddrFreq;
  Profile       = MemInfoData->Profile;
  Timing        = &MemInfoData->Timing[Profile];

  mSaSetup.XmpProfileEnable = MemInfoData->XmpProfileEnable;

  if (mSaSetup.PPR != 0) {
    if ((mSaSetup.PprRunOnce != 0) && (mSaSetup.PprRunAtFastboot == 0)) {
      mSaSetup.PPR = 0;
    }
    if (mSaSetup.PprRepairPhysicalAddress != 0) {
      mSaSetup.PprRepairPhysicalAddress = 0;
    }
  }

  Mc0Enabled = 0;
  Mc1Enabled = 0;

  Status = gRT->SetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  SaVariableAttributes,
                  SaSetupVariableSize,
                  &mSaSetup
                  );
  //
  // Update the memory size string for main page and chipset page
  //
  if ((Class == MAIN_FORM_SET_CLASS) || (Class == ADVANCED_FORM_SET_CLASS)) {
    MemorySize = MemInfoData->memSize;
    InitString (HiiHandle, STRING_TOKEN (STR_MEMORY_SIZE_VALUE), L"%5ld MB", MemorySize);
    InitString (HiiHandle, STRING_TOKEN (STR_MEMORY_FREQ_VALUE), L"%5ld MHz", DdrFrequency);

    InitString (HiiHandle, STRING_TOKEN (STR_PPR_STAT_VALUE),  L"Detected bad rows %d times; Repair failed %d times;"
      L" Force Repair %s"
      , MemInfoData->PprDetectedErrors, MemInfoData->PprRepairFails
      , (mSaSetup.PprForceRepair == 0) ? L"disabled" : (MemInfoData->PprForceRepairStatus != 0 ? L"failed" : L"succeeded")
    );

    //
    // MEMSS_PMA_CR_MRC_VERSION_REG - MRC version
    //
    Value32 = MmioRead32 ((UINTN) PcdGet64 (PcdMchBaseAddress) + 0x13D40);
    x       = (UINT8) ((Value32 & 0xFF000000) >> 24);
    y       = (UINT8) ((Value32 & 0xFF0000) >> 16);
    z       = (UINT8) ((Value32 & 0xFF00) >> 8);
    w       = (UINT8) (Value32 & 0xFF);

    InitString (HiiHandle, STRING_TOKEN (STR_MRC_REV_VALUE), L"%d.%d.%d.%d", x, y, z, w);
  }

  if (IGpuGetDeviceId () != 0xFFFF) {
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_CHIP_IGFX_GOP_REV_VALUE),
      L"%s",
      &IGpuDxeConfig->GopVersion
    );
  }

  if (Class == MAIN_FORM_SET_CLASS) {
    gHiiHandle = HiiHandle;
    Data16 = IGpuGetDeviceId ();
    if (Data16 != 0xFFFF) {
      InitString (gHiiHandle, STRING_TOKEN (STR_PROCESSOR_GT_VALUE), L"0x%X", Data16);
    }

  }

  if (Class == ADVANCED_FORM_SET_CLASS) {
    SetupVolVariableSize = sizeof (SETUP_VOLATILE_DATA);
    Status = gRT->GetVariable (
                    L"SetupVolatileData",
                    &gSetupVariableGuid,
                    &SetupVolAttributes,
                    &SetupVolVariableSize,
                    &mSetupVolatileData
                    );
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status)) {
      Slot = 0;
      SkipStringUpdate = FALSE;
      mSetupVolatileData.MemoryType = MemoryType;
      DEBUG((EFI_D_INFO, "MemoryType : %X\n", MemoryType));
      for (NodeIndex = 0; NodeIndex < NODE_NUM; NodeIndex++) {
        for (ChannelIndex = 0; ChannelIndex < CH_NUM; ChannelIndex++) {
          //
          // Skip the strings init for DDR5 when Channel is equal or above one
          //
          if (((ChannelIndex >= 1) && (mSetupVolatileData.MemoryType == MRC_DDR_TYPE_DDR5))) {
            SkipStringUpdate = TRUE;
          }
          for (DimmIndex = 0; DimmIndex < DIMM_NUM; DimmIndex++,Slot++) {
            //
            // Skip the stings init for LPDDR5 when Dimm is above zero
            //
            if ((DimmIndex > 0)&& (mSetupVolatileData.MemoryType == MRC_DDR_TYPE_LPDDR5)) {
              SkipStringUpdate = TRUE;
            }
            if (!SkipStringUpdate) {
              if (MemInfoData->DimmStatus[Slot] != DIMM_DISABLED) {
                InitString (
                  HiiHandle,
                  DimmSizeString[Slot],
                  L"%4ld MB (%a)",
                  MemInfoData->dimmSize[Slot],
                  (mSetupVolatileData.MemoryType < MRC_DDR_TYPE_UNKNOWN) ? MemoryTypeStr[mSetupVolatileData.MemoryType] : "Unknown"
                );
              }
              if (MemInfoData->DimmStatus[Slot] < DIMM_NOT_PRESENT) {
                mSetupVolatileData.DimmPresent[Slot] = 1;
                InitString (
                  HiiHandle,
                  RankInDimmString[Slot],
                  L"%1d",
                  MemInfoData->RankInDimm[Slot]
                  );
                DimmStatusStr = (MemInfoData->DimmStatus[Slot] == DIMM_DISABLED) ? "Populated & Disabled" : "Populated & Enabled";
                if (MemInfoData->DimmStatus[Slot] != DIMM_DISABLED) {
                  if (Slot < 8) {
                    Mc0Enabled = 1;
                  } else {
                    Mc1Enabled = 1;
                  }
                }
                /**
                  Get the Memory Module Vendor JEDEC ID
                  Byte 117-118 for DDR3/LPDDR3 and byte 320-321 for DDR4
                  It's from first byte of SPD buffer.
                **/
                VendorId = *(UINT16 *) (UINTN) (MemInfoData->DimmsSpdData[Slot]);
                VendorId &= ~(BIT7);  // Clear the parity bit
                for (MfgIdIndex = 0; MfgIdIndex < MemoryModuleManufacturerListSize; MfgIdIndex++) {
                  if (VendorId == MemoryModuleManufacturerList[MfgIdIndex].MfgId) {
                    InitString (
                      HiiHandle,
                      DimmMfgString[Slot],
                      L"%a",
                      MemoryModuleManufacturerList[MfgIdIndex].String
                     );
                    break;
                  }
                }
              } else {
                mSetupVolatileData.DimmPresent[Slot] = 0;
                DimmStatusStr = "Not Populated / Disabled";
              }

              InitString (
                HiiHandle,
                DimmStatusString[Slot],
                L"%a",
                DimmStatusStr
                );
            } else {
              SkipStringUpdate = FALSE;
            }
          }
        }
      }

      mSetupVolatileData.McCount = Mc0Enabled + Mc1Enabled;
      // Update Ibecc Protected Base Display based on number of IBECC enabled
      for (i = 0; i < IBECC_REGIONS_MAX; i++) {
        mSaSetup.IbeccProtectedRegionOverallBase[i] = mSaSetup.IbeccProtectedRegionBase[i] << (mSetupVolatileData.McCount - 1);
      }
      // Update Ibecc Protected Size Display based on number of IBECC enabled
      for (SizeIndex = 0; SizeIndex < IBECC_SIZE_NUM; SizeIndex++) {
        IbeccSize = 32 << SizeIndex;
        // Display of size is based on total IBECC enabled, so multiple the size by 2 if both IBECC enabled
        if (mSetupVolatileData.McCount > 1) {
          IbeccSize = IbeccSize << 1;
        }
        if (IbeccSize >= 1024) {
          IbeccSize = IbeccSize >> 10;
          IbeccSizeStr = "GB";
        } else {
          IbeccSizeStr = "MB";
        }
        InitString (
          HiiHandle,
          IbeccSizeString[SizeIndex],
          L"%d %a",
          IbeccSize,
          IbeccSizeStr
          );
      }
      // Update MemorySize per IBECC instance to use for hiding protected size option
      mSetupVolatileData.MemorySizeIn32MB = (MemInfoData->memSize >> 5) >> (mSetupVolatileData.McCount - 1);
      DEBUG((DEBUG_INFO, "mSetupVolatileData.MemorySizeIn32MB (per MC) = %d\n", mSetupVolatileData.MemorySizeIn32MB));

      Status = gRT->SetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  SaVariableAttributes,
                  SaSetupVariableSize,
                  &mSaSetup
                  );

      //
      // VT-d status report
      //
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_SA_VTD_VALUE),
        mSetupVolatileData.VTdAvailable ? L"Supported" : L"Unsupported"
        );

      //
      // IPU BIOS Setup option display
      //
      IpuDataHob = (IPU_DATA_HOB *) GetFirstGuidHob (&gIpuDataHobGuid);
      if (IpuDataHob != NULL) {
        mSetupVolatileData.IpuSupport = 1;
      } else {
        mSetupVolatileData.IpuSupport = 0;
      }

#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 1
      UpdateIpuSetupConfig (mSetupVolatileData.IpuSupport);
#endif
      //
      // NPU BIOS Setup option display
      //
      if (IsNpuSupported ()) {
        mSetupVolatileData.NpuSupport = 1;
      } else {
        mSetupVolatileData.NpuSupport = 0;
      }
      //
      // IAX BIOS Setup option display
      //
      if (IsIaxSupported ()) {
        mSetupVolatileData.IaxSupport = 1;
      } else {
        mSetupVolatileData.IaxSupport = 0;
      }

      //
      // Graphics BIOS Setup option display
      //
      if (IGpuIsDisplayPresent ()) {
        mSetupVolatileData.IsDisplayPresent = 1;
      } else {
        mSetupVolatileData.IsDisplayPresent = 0;
      }

      if (IGpuIsGtPresent ()) {
        mSetupVolatileData.IsGtPresent = 1;
      } else {
        mSetupVolatileData.IsGtPresent = 0;
      }

      if (IGpuIsMediaPresent ()) {
        mSetupVolatileData.IsMediaPresent = 1;
      } else {
        mSetupVolatileData.IsMediaPresent = 0;
      }

      InitString (
        HiiHandle,
        STRING_TOKEN (STR_IGPU_DISPLAY_VALUE),
        mSetupVolatileData.IsDisplayPresent ? L"Supported" : L"Unsupported"
        );

      InitString (
        HiiHandle,
        STRING_TOKEN (STR_IGPU_MEDIA_VALUE),
        mSetupVolatileData.IsMediaPresent ? L"Supported" : L"Unsupported"
        );

      InitString (
        HiiHandle,
        STRING_TOKEN (STR_IGPU_GT_VALUE),
        mSetupVolatileData.IsGtPresent ? L"Supported" : L"Unsupported"
        );
      Status = gRT->SetVariable (
                      L"SetupVolatileData",
                      &gSetupVariableGuid,
                      SetupVolAttributes,
                      SetupVolVariableSize,
                      &mSetupVolatileData
                      );
      ASSERT_EFI_ERROR (Status);
    } // if SetupVolatileData found
    //
    // Update the tCL, tRCD, tRP and tRAS string with data obtained from MemInfo protocol
    //
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_MEMORY_VDD_VALUE),
      L"%d",
      MemInfoData->VddVoltage[Profile]
      );

    InitString (
      HiiHandle,
      STRING_TOKEN (STR_MEMORY_TIMINGS_VALUE),
      L"%d-%d-%d-%d",
      Timing->tCL,
      Timing->tRCDtRP,
      Timing->tRCDtRP,
      Timing->tRAS
      );
  } // if ADVANCED_FORM_SET_CLASS


//
//  IOM, TBT firmware version and TBT IMR status
//
#if FixedPcdGetBool(PcdTcssSupport) == 1
  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob != NULL) {
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_TCSS_TBT_STATUS_STRING),
      L"TBT FW IMR Status: %08X",
      TcssHob->TcssData.TbtImrStatus.RegValue
      );

    InitString (
      HiiHandle,
      STRING_TOKEN (STR_TCSS_PHY_VERSION_STRING),
      L"PHY FW version: %04X",
      TcssHob->TcssData.MgImrStatus.Bits.MgFwVersion
      );

    if (TcssHob->TcssData.TbtImrStatus.Bits.Done) {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_TCSS_TBT_VERSION_STRING),
        L"TBT FW version: %04d",
        TcssHob->TcssData.TbtImrStatus.Bits.TbtFwVersion
        );
    }

    if (TcssHob->TcssData.IOMReady) {
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_TCSS_IOM_VERSION_STRING),
        L"IOM FW version: %08X",
        TcssHob->TcssData.IomFwVersion
        );

      InitString (
        HiiHandle,
        STRING_TOKEN (STR_TCSS_IOM_ENGR_VERSION_STRING),
        L"IOM FW version: %08X",
        TcssHob->TcssData.IomFwEngrVersion
        );

      InitString (
        HiiHandle,
        STRING_TOKEN (STR_TC_CSTATE_STATUS),
        L"Deepest TC state: %04X",
        TcssHob->TcssData.DeepstTcState
        );
    }
  } else {
    DEBUG ((DEBUG_ERROR, "TcssHob is NULL, Update TCSS Setup Menu Failed!\n"));
  }
#endif

  //
  // Update SaSetup variables
  //
  Size = sizeof (mSaSetup);
  Status = gRT->GetVariable(L"SaSetup", &gSaSetupVariableGuid, &VariableAttributes, &Size, &mSaSetup);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
  pSaSetup = (SA_SETUP *) &mSaSetup;
  // VMD: Check if VMD is supported
  if (IsVmdSupported() == TRUE) {
    // VMD is supported
    pSaSetup->VmdSupported = 1;
    //
    // Update VMD Volatile variables
    //
        VmdInfoHob = (VMD_INFO_HOB *) GetFirstGuidHob (&gVmdInfoHobGuid);
    if (VmdInfoHob != NULL) {
      for (Index=0; Index < VMD_MAX_DEVICES; ++Index) {
        pSaSetup->VmdPortPresent[Index] = VmdInfoHob->VmdPortInfo.PortInfo[Index].DeviceDetected;
        pSaSetup->VmdPort[Index]        = VmdInfoHob->VmdPortInfo.PortInfo[Index].PortEn;
        // Also update the device and func
        pSaSetup->VmdPortDev[Index]     = VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev;
        pSaSetup->VmdPortFunc[Index]    = VmdInfoHob->VmdPortInfo.PortInfo[Index].RpFunc;
      }
      DEBUG ((EFI_D_INFO, "Vmd: Printing updated setup info\n"));
      for (Index=0; Index < VMD_MAX_DEVICES; ++Index) {
        if (pSaSetup->VmdPortPresent[Index]) { //print only if device is discovered
          DEBUG ((EFI_D_INFO, "Vmd For Rp %d VmdPortPresent is %d  VmdPort-En is %d  VmdPortDev is %d  VmdPortFunc is %d\n",
            Index, pSaSetup->VmdPortPresent[Index], pSaSetup->VmdPort[Index], pSaSetup->VmdPortDev[Index], pSaSetup->VmdPortFunc[Index] ));
        }
      }
    } else {
      DEBUG ((EFI_D_INFO, "Vmd Info Hob not found\n"));
    }
  }
  gRT->SetVariable (
        L"SaSetup",
        &gSaSetupVariableGuid,
        VariableAttributes,
        sizeof (mSaSetup),
        &mSaSetup
        );
  }

  // update VMD info if VMD is supported.
  if (mSaSetup.VmdSupported) {
    UpdateVmdInfo(HiiHandle);
  }
}

EFI_STATUS
EFIAPI
UpdateVmdInfo (
  EFI_HII_HANDLE HiiHandle
  )
{
  EFI_STRING_ID   StrRef;
  VMD_INFO_HOB    *VmdInfoHob;
  UINT8           Index;

  DEBUG ((EFI_D_INFO, "UpdateVmdSetupInfo()\n"));
  VmdInfoHob = (VMD_INFO_HOB *) GetFirstGuidHob (&gVmdInfoHobGuid);
  if (VmdInfoHob == NULL) {
    DEBUG ((EFI_D_INFO, "Vmd Info Hob not found\n"));
    return EFI_NOT_FOUND;
  }
  for ( Index = 0; Index < VMD_MAX_DEVICES; ++Index ) {
    switch(Index) {
      case 0:
        StrRef = STRING_TOKEN (STR_RP1_BDF_VALUE);
        break;
      case 1:
        StrRef = STRING_TOKEN (STR_RP2_BDF_VALUE);
        break;
      case 2:
        StrRef = STRING_TOKEN (STR_RP3_BDF_VALUE);
        break;
      case 3:
        StrRef = STRING_TOKEN (STR_RP4_BDF_VALUE);
        break;
      case 4:
        StrRef = STRING_TOKEN (STR_RP5_BDF_VALUE);
        break;
      case 5:
        StrRef = STRING_TOKEN (STR_RP6_BDF_VALUE);
        break;
      case 6:
        StrRef = STRING_TOKEN (STR_RP7_BDF_VALUE);
        break;
      case 7:
        StrRef = STRING_TOKEN (STR_RP8_BDF_VALUE);
        break;
      case 8:
        StrRef = STRING_TOKEN (STR_RP9_BDF_VALUE);
        break;
      case 9:
        StrRef = STRING_TOKEN (STR_RP10_BDF_VALUE);
        break;
      case 10:
        StrRef = STRING_TOKEN (STR_RP11_BDF_VALUE);
        break;
      case 11:
        StrRef = STRING_TOKEN (STR_RP12_BDF_VALUE);
        break;
      case 12:
        StrRef = STRING_TOKEN (STR_RP13_BDF_VALUE);
        break;
      case 13:
        StrRef = STRING_TOKEN (STR_RP14_BDF_VALUE);
        break;
      case 14:
        StrRef = STRING_TOKEN (STR_RP15_BDF_VALUE);
        break;
      case 15:
        StrRef = STRING_TOKEN (STR_RP16_BDF_VALUE);
        break;
      case 16:
        StrRef = STRING_TOKEN (STR_RP17_BDF_VALUE);
        break;
      case 17:
        StrRef = STRING_TOKEN (STR_RP18_BDF_VALUE);
        break;
      case 18:
        StrRef = STRING_TOKEN (STR_RP19_BDF_VALUE);
        break;
      case 19:
        StrRef = STRING_TOKEN (STR_RP20_BDF_VALUE);
        break;
      case 20:
        StrRef = STRING_TOKEN (STR_RP21_BDF_VALUE);
        break;
      case 21:
        StrRef = STRING_TOKEN (STR_RP22_BDF_VALUE);
        break;
      case 22:
        StrRef = STRING_TOKEN (STR_RP23_BDF_VALUE);
        break;
      case 23:
        StrRef = STRING_TOKEN (STR_RP24_BDF_VALUE);
        break;
      case 24:
        StrRef = STRING_TOKEN (STR_RP25_BDF_VALUE);
        break;
      case 25:
        StrRef = STRING_TOKEN (STR_RP26_BDF_VALUE);
        break;
      case 26:
        StrRef = STRING_TOKEN (STR_RP27_BDF_VALUE);
        break;
      case 27:
        StrRef = STRING_TOKEN (STR_RP28_BDF_VALUE);
        break;
      case 28:
        StrRef = STRING_TOKEN (STR_RP29_BDF_VALUE);
        break;
      case 29:
        StrRef = STRING_TOKEN (STR_RP30_BDF_VALUE);
        break;
      case 30:
        StrRef = STRING_TOKEN (STR_RP31_BDF_VALUE);
        break;
    }

    InitString (
      HiiHandle,
      StrRef,
      L"Map RP BDF %d/%d/%d Under VMD",
      VmdInfoHob->VmdPortInfo.PortInfo[Index].RpBus,
      VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev,
      VmdInfoHob->VmdPortInfo.PortInfo[Index].RpFunc
      );
  }
  return EFI_SUCCESS;
}

/**
  This function performs mapping of all the detected storage devices under
  VMD if Global Mapping is enabled.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  KeyValue               A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.
**/
EFI_STATUS
EFIAPI
VmdCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  SA_SETUP        *SaSetup;
  UINTN           VarSize;
  EFI_STATUS      Status;
  EFI_STRING      RequestString;
  VMD_INFO_HOB    *VmdInfoHob;
  int             i;

  Status = EFI_SUCCESS;
  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    DEBUG ((DEBUG_INFO, "Exited without updating VMD Setup menu\n"));
    return EFI_UNSUPPORTED;
  }

  ASSERT (KeyValue == KEY_SA_VMD_GLOBAL_MAPPING);
  DEBUG ((DEBUG_INFO, "VmdCallback()\n"));

  VarSize = sizeof (SA_SETUP);
  SaSetup = AllocatePool (VarSize);
  ASSERT (SaSetup != NULL);
  if (SaSetup == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (!HiiGetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VarSize, (UINT8 *) SaSetup)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status) && (SaSetup->VmdGlobalMapping == 1)) {
    VmdInfoHob = (VMD_INFO_HOB *) GetFirstGuidHob (&gVmdInfoHobGuid);
    if (VmdInfoHob == NULL) {
      DEBUG ((DEBUG_INFO, "Vmd Info Hob not found\n"));
    } else {
      DEBUG ((DEBUG_INFO, "Vmd Global Mapping is enabled\n"));
      RequestString = NULL;
      for (i = 0; i < VMD_MAX_DEVICES; ++i) {
        if (VmdInfoHob->VmdPortInfo.PortInfo[i].DeviceDetected) {
          switch(i) {
            case 0:
              SaSetup->VmdPort[0] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[0]), sizeof (SaSetup->VmdPort[0]));
              break;
            case 1:
              SaSetup->VmdPort[1] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[1]), sizeof (SaSetup->VmdPort[1]));
              break;
            case 2:
              SaSetup->VmdPort[2] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[2]), sizeof (SaSetup->VmdPort[2]));
              break;
            case 3:
              SaSetup->VmdPort[3] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[3]), sizeof (SaSetup->VmdPort[3]));
              break;
            case 4:
              SaSetup->VmdPort[4] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[4]), sizeof (SaSetup->VmdPort[4]));
              break;
            case 5:
              SaSetup->VmdPort[5] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[5]), sizeof (SaSetup->VmdPort[5]));
              break;
            case 6:
              SaSetup->VmdPort[6] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[6]), sizeof (SaSetup->VmdPort[6]));
              break;
            case 7:
              SaSetup->VmdPort[7] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[7]), sizeof (SaSetup->VmdPort[7]));
              break;
            case 8:
              SaSetup->VmdPort[8] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[8]), sizeof (SaSetup->VmdPort[8]));
              break;
            case 9:
              SaSetup->VmdPort[9] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[9]), sizeof (SaSetup->VmdPort[9]));
              break;
            case 10:
              SaSetup->VmdPort[10] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[10]), sizeof (SaSetup->VmdPort[10]));
              break;
            case 11:
              SaSetup->VmdPort[11] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[11]), sizeof (SaSetup->VmdPort[11]));
              break;
            case 12:
              SaSetup->VmdPort[12] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[12]), sizeof (SaSetup->VmdPort[12]));
              break;
            case 13:
              SaSetup->VmdPort[13] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[13]), sizeof (SaSetup->VmdPort[13]));
              break;
            case 14:
              SaSetup->VmdPort[14] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[14]), sizeof (SaSetup->VmdPort[14]));
              break;
            case 15:
              SaSetup->VmdPort[15] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[15]), sizeof (SaSetup->VmdPort[15]));
              break;
            case 16:
              SaSetup->VmdPort[16] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[16]), sizeof (SaSetup->VmdPort[16]));
              break;
            case 17:
              SaSetup->VmdPort[17] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[17]), sizeof (SaSetup->VmdPort[17]));
              break;
            case 18:
              SaSetup->VmdPort[18] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[18]), sizeof (SaSetup->VmdPort[18]));
              break;
            case 19:
              SaSetup->VmdPort[19] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[19]), sizeof (SaSetup->VmdPort[19]));
              break;
            case 20:
              SaSetup->VmdPort[20] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[20]), sizeof (SaSetup->VmdPort[20]));
              break;
            case 21:
              SaSetup->VmdPort[21] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[21]), sizeof (SaSetup->VmdPort[21]));
              break;
            case 22:
              SaSetup->VmdPort[22] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[22]), sizeof (SaSetup->VmdPort[22]));
              break;
            case 23:
              SaSetup->VmdPort[23] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[23]), sizeof (SaSetup->VmdPort[23]));
              break;
            case 24:
              SaSetup->VmdPort[24] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[24]), sizeof (SaSetup->VmdPort[24]));
              break;
            case 25:
              SaSetup->VmdPort[25] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[25]), sizeof (SaSetup->VmdPort[25]));
              break;
            case 26:
              SaSetup->VmdPort[26] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[26]), sizeof (SaSetup->VmdPort[26]));
              break;
            case 27:
              SaSetup->VmdPort[27] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[27]), sizeof (SaSetup->VmdPort[27]));
              break;
            case 28:
              SaSetup->VmdPort[28] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[28]), sizeof (SaSetup->VmdPort[28]));
              break;
            case 29:
              SaSetup->VmdPort[29] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[29]), sizeof (SaSetup->VmdPort[29]));
              break;
            case 30:
              SaSetup->VmdPort[30] = 1;
              RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, VmdPort[30]), sizeof (SaSetup->VmdPort[30]));
              break;
          } // Switch
        } // if (devicedetected)
      } // for
      if (RequestString != NULL) {
        if (HiiSetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VarSize, (UINT8 *) SaSetup, RequestString)) {
          FreePool (RequestString);
        } else {
          Status = EFI_NOT_FOUND;
          DEBUG ((DEBUG_INFO, "Vmd browser is not set\n"));
        }
        ASSERT_EFI_ERROR (Status);
      }
    } // else
  } // if (VmdGlobalMapping)
  FreePool (SaSetup);

  return Status;
} // VmdCallback

/**
  This function updates TCSS setup variables when there is change in TCSS xHCI.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  KeyValue               A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.
**/
EFI_STATUS
EFIAPI
TcssXhciCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  SA_SETUP                 *SaSetup;
  SETUP_DATA               *SetupData;
  UINT8                    Index;
  UINTN                    VarSize;
  EFI_STATUS               Status;
  EFI_STRING               RequestString;
  UINT8                    *TcssPortCap;
  UINT32                   TcssPortCapMap;

  DEBUG ((EFI_D_INFO, "TcssXhciCallback()\n"));

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  Status = EFI_SUCCESS;
  Index  = 0;
  TcssPortCapMap = PcdGet32 (PcdTcssPortCapMap);
  TcssPortCap    = (UINT8 *) &TcssPortCapMap;

  VarSize = sizeof (SA_SETUP);
  SaSetup = AllocatePool (VarSize);
  ASSERT (SaSetup != NULL);
  if (SaSetup == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (!HiiGetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VarSize, (UINT8 *) SaSetup)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
      SaSetup->TcssPortConf[Index] = (Value->u8 == 0) ? DISABLE : TcssPortCap[Index];
    }
    RequestString = NULL;
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, TcssPortConf[0]),            sizeof (SaSetup->TcssPortConf[0]));
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, TcssPortConf[1]),            sizeof (SaSetup->TcssPortConf[1]));
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, TcssPortConf[2]),            sizeof (SaSetup->TcssPortConf[2]));
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, TcssPortConf[3]),            sizeof (SaSetup->TcssPortConf[3]));
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, TcssVccstStatus),            sizeof (SaSetup->TcssVccstStatus));
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, TcssD3ColdEnable),           sizeof (SaSetup->TcssD3ColdEnable));

    if (RequestString != NULL) {
      if (!HiiSetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VarSize, (UINT8 *) SaSetup, RequestString)) {
        Status = EFI_NOT_FOUND;
      }
      ASSERT_EFI_ERROR (Status);
      FreePool (RequestString);
    }
  }
  FreePool (SaSetup);

  VarSize = sizeof (SETUP_DATA);
  SetupData = AllocatePool (VarSize);
  ASSERT (SetupData != NULL);
  if (SetupData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (!HiiGetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) SetupData)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    if (Value->u8 == 0) {
      SetupData->UsbcBiosTcssHandshake = 0;
    } else {
      SetupData->UsbcBiosTcssHandshake = 1;
    }
    RequestString = NULL;
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, UsbcBiosTcssHandshake), sizeof (SetupData->UsbcBiosTcssHandshake));

    if (RequestString != NULL) {
      if (!HiiSetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) SetupData, RequestString)) {
        Status = EFI_NOT_FOUND;
      }
      ASSERT_EFI_ERROR (Status);
      FreePool (RequestString);
    }
  }
  FreePool (SetupData);

  return Status;
}

/**
  This function checks if protected range start address
  is aligned with protected size, and make sure the
  start address of a range does not overlap with the
  size needed for another range.

  @param[in] EFI_HII_CONFIG_ACCESS_PROTOCOL   *This
  @param[in] EFI_BROWSER_ACTION               Action
  @param[in] EFI_QUESTION_ID                  KeyValue
  @param[in] UINT8                            Type
  @param[in] EFI_IFR_TYPE_VALUE               *Value
  @param[in] OUT EFI_BROWSER_ACTION_REQUEST   *ActionRequest

  @retval EFI_SUCCESS        Call Back Function executed successfully
**/
EFI_STATUS
EFIAPI
IbeccRangeCheckCallBack (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  EFI_STATUS  Status;
  SA_SETUP    *SaSetup;
  EFI_STRING  RequestString;
  UINT32      SetupVolAttributes;
  UINT32      SaVariableAttributes;
  UINTN       SetupVolVariableSize;
  UINTN       SaSetupVariableSize;
  UINTN       VarSize;
  UINT8       RequestedRegion;
  UINT8       i, j;
  UINT8       CurrentRegion;
  UINT8       DisabledRegionCount;
  UINT16      RequestedSize;
  UINT16      RequestedBase;
  UINT32      RequestedRegionStart;
  UINT32      RequestedRegionEnd;
  UINT32      OtherRegionStart;
  UINT32      OtherRegionEnd;
  BOOLEAN     IsRangeCheck;
  CHAR16      RequestedRegionStr[50];
  CHAR16      OtherRegionStr[50];
  CHAR16      *ErrorString;
  CHAR16      *StringBuffer[MAX_IBECC_REGION_OVERLAP];
  CHAR16      **CurrentStringPtr = &StringBuffer[0];

  if (Action != EFI_BROWSER_ACTION_CHANGED && Action != EFI_BROWSER_ACTION_SUBMITTED) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a () enter, Action = 0x%x, KeyValue = 0x%x\n", __FUNCTION__, Action, KeyValue));

  Status = EFI_SUCCESS;

  VarSize = sizeof (SA_SETUP);
  SaSetup = AllocatePool (VarSize);
  ASSERT (SaSetup != NULL);
  if (SaSetup == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (!HiiGetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VarSize, (UINT8 *) SaSetup)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  SetupVolVariableSize = sizeof (SETUP_VOLATILE_DATA);
  Status = gRT->GetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  &SetupVolAttributes,
                  &SetupVolVariableSize,
                  &mSetupVolatileData
                  );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR(Status);
    return Status;
  }

  SaSetupVariableSize = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  &SaVariableAttributes,
                  &SaSetupVariableSize,
                  &mSaSetup
                  );
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR(Status);
    return Status;
  }

  ErrorString = AllocateZeroPool (0x100);
  ASSERT (ErrorString != NULL);
  for (i = 0; i < MAX_IBECC_REGION_OVERLAP; i++) {
    StringBuffer[i] = AllocateZeroPool (0x100 * sizeof (CHAR16));
    ASSERT (StringBuffer[i] != NULL);
    ZeroMem (StringBuffer[i], 0x100 * sizeof (CHAR16));
  }

  if (!EFI_ERROR (Status)) {
    if (Action == EFI_BROWSER_ACTION_CHANGED) {
      switch (KeyValue) {
        case KEY_IBECC_BASE0_CHECK:
        case KEY_IBECC_MASK0_CHECK:
          RequestedRegion = 0;
          break;

        case KEY_IBECC_BASE1_CHECK:
        case KEY_IBECC_MASK1_CHECK:
          RequestedRegion = 1;
          break;

        case KEY_IBECC_BASE2_CHECK:
        case KEY_IBECC_MASK2_CHECK:
          RequestedRegion = 2;
          break;

        case KEY_IBECC_BASE3_CHECK:
        case KEY_IBECC_MASK3_CHECK:
          RequestedRegion = 3;
          break;

        case KEY_IBECC_BASE4_CHECK:
        case KEY_IBECC_MASK4_CHECK:
          RequestedRegion = 4;
          break;

        case KEY_IBECC_BASE5_CHECK:
        case KEY_IBECC_MASK5_CHECK:
          RequestedRegion = 5;
          break;

        case KEY_IBECC_BASE6_CHECK:
        case KEY_IBECC_MASK6_CHECK:
          RequestedRegion = 6;
          break;

        case KEY_IBECC_BASE7_CHECK:
        case KEY_IBECC_MASK7_CHECK:
          RequestedRegion = 7;
          break;

        default:
          return EFI_UNSUPPORTED;
      }

      // Check if base is multiple of size. Reset base to 0 if base is not multiple of size to prevent user save invalid base.
      RequestedSize = (0x4000 - SaSetup->IbeccProtectedRegionMask[RequestedRegion]) * mSetupVolatileData.McCount;
      RequestedBase = SaSetup->IbeccProtectedRegionOverallBase[RequestedRegion];
      if ((RequestedBase % RequestedSize) > 0) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          NULL,
          L"",
          L"Invalid base entered",
          L"Base must be multiple of size",
          L"Resetting base to 0",
          L"Please change the region base/size",
          L"",
          NULL
          );
        SaSetup->IbeccProtectedRegionOverallBase[RequestedRegion] = 0;
      } else {
        RequestedRegionStart = RequestedBase;
        RequestedRegionEnd   = RequestedRegionStart + RequestedSize;
        CurrentRegion = 0xFF;

        // Compare with other region to make sure does not overlap. Prompt message to inform user to change base/size if there is overlap.
        for (i = 0; i < IBECC_REGIONS_MAX; i++) {
          if (i == RequestedRegion) {
            continue;
          }
          if (SaSetup->IbeccProtectedRegionEnable[i] == 1) {
            OtherRegionStart = SaSetup->IbeccProtectedRegionOverallBase[i];
            OtherRegionEnd   = OtherRegionStart + ((0x4000 - SaSetup->IbeccProtectedRegionMask[i]) * mSetupVolatileData.McCount);
            if (((RequestedRegionStart >= OtherRegionStart) && (RequestedRegionStart < OtherRegionEnd)) ||
                ((OtherRegionStart >= RequestedRegionStart) && (OtherRegionStart < RequestedRegionEnd))) {
              UnicodeValueToStringS (RequestedRegionStr, sizeof(RequestedRegionStr), 0, RequestedRegion, 0);
              UnicodeValueToStringS (OtherRegionStr, sizeof(OtherRegionStr), 0, i, 0);
              if (CurrentRegion != RequestedRegion) {
                CurrentRegion = RequestedRegion;
                StrCatS (ErrorString, 0x100 / sizeof (CHAR16), L"Region ");
                StrCatS (ErrorString, 0x100 / sizeof (CHAR16), RequestedRegionStr);
                StrCatS (ErrorString, 0x100 / sizeof (CHAR16), L" overlap with region ");
              } else {
                StrCatS (ErrorString, 0x100 / sizeof (CHAR16), L", ");
              }
              StrCatS (ErrorString, 0x100 / sizeof (CHAR16), OtherRegionStr);
            }
          }
        }
        if (CurrentRegion != 0xFF) {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            NULL,
            L"",
            ErrorString,
            L"Please change the region base/size",
            L"",
            NULL
            );
        }
      }
    } else {
      // Check all regions before saving. Disable the region that's overlapped.
      DisabledRegionCount = 0;
      CurrentRegion = 0xFF;
      for (i = 0; i < IBECC_REGIONS_MAX; i++) {
      RequestedRegionStart = SaSetup->IbeccProtectedRegionOverallBase[i];
      RequestedRegionEnd   = RequestedRegionStart + ((0x4000 - SaSetup->IbeccProtectedRegionMask[i]) * mSetupVolatileData.McCount);
      IsRangeCheck = FALSE;
        if (SaSetup->IbeccProtectedRegionEnable[i] == 1) {
          for (j = 0; j < IBECC_REGIONS_MAX; j++) {
            if (i == j) {
              continue;
            }
            if (SaSetup->IbeccProtectedRegionEnable[j] == 1) {
              OtherRegionStart = SaSetup->IbeccProtectedRegionOverallBase[j];
              OtherRegionEnd   = OtherRegionStart + ((0x4000 - SaSetup->IbeccProtectedRegionMask[j]) * mSetupVolatileData.McCount);
              if (((RequestedRegionStart >= OtherRegionStart) && (RequestedRegionStart < OtherRegionEnd)) ||
                  ((OtherRegionStart >= RequestedRegionStart) && (OtherRegionStart < RequestedRegionEnd))) {
                UnicodeValueToStringS (RequestedRegionStr, sizeof(RequestedRegionStr), 0, i, 0);
                UnicodeValueToStringS (OtherRegionStr, sizeof(OtherRegionStr), 0, j, 0);
                SaSetup->IbeccProtectedRegionEnable[j] = 0;
                IsRangeCheck = TRUE;
                if (CurrentRegion != i) {
                  CurrentRegion = i;
                  StrCatS (*CurrentStringPtr, 0x100 / sizeof (CHAR16), L"Region ");
                  StrCatS (*CurrentStringPtr, 0x100 / sizeof (CHAR16), RequestedRegionStr);
                  StrCatS (*CurrentStringPtr, 0x100 / sizeof (CHAR16), L" overlap with region ");
                } else {
                  StrCatS (*CurrentStringPtr, 0x100 / sizeof (CHAR16), L", ");
                }
                StrCatS (*CurrentStringPtr, 0x100 / sizeof (CHAR16), OtherRegionStr);
                if (DisabledRegionCount == 0) {
                  StrCatS (ErrorString, 0x100 / sizeof (CHAR16), L"Disabling region ");
                } else {
                  StrCatS (ErrorString, 0x100 / sizeof (CHAR16), L", ");
                }
                StrCatS (ErrorString, 0x100 / sizeof (CHAR16), OtherRegionStr);
                DisabledRegionCount++;
              }
            }
          }
          if (IsRangeCheck) {
            CurrentStringPtr++;
          }
        }
      }

      if (DisabledRegionCount > 0) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          NULL,
          L"",
          StringBuffer[0],
          StringBuffer[1],
          StringBuffer[2],
          StringBuffer[3],
          ErrorString,
          L"",
          NULL
          );
      }

      for (i = 0; i < IBECC_REGIONS_MAX; i++) {
        mSaSetup.IbeccProtectedRegionOverallBase[i] = SaSetup->IbeccProtectedRegionOverallBase[i];
        mSaSetup.IbeccProtectedRegionBase[i] = SaSetup->IbeccProtectedRegionOverallBase[i] / mSetupVolatileData.McCount;
        mSaSetup.IbeccProtectedRegionEnable[i] = SaSetup->IbeccProtectedRegionEnable[i];
      }
      Status = gRT->SetVariable (
                      L"SaSetup",
                      &gSaSetupVariableGuid,
                      SaVariableAttributes,
                      SaSetupVariableSize,
                      &mSaSetup
                      );
      ASSERT_EFI_ERROR (Status);
    }

    FreePool (ErrorString);
    for (i = 0; i < MAX_IBECC_REGION_OVERLAP; i++) {
      FreePool (StringBuffer[i]);
    }

    RequestString = NULL;
    for (i = 0; i < IBECC_REGIONS_MAX; i++) {
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, IbeccProtectedRegionOverallBase[i]),     sizeof (SaSetup->IbeccProtectedRegionOverallBase[i]));
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, IbeccProtectedRegionEnable[i]),          sizeof (SaSetup->IbeccProtectedRegionEnable[i]));
    }
    if (RequestString != NULL) {
      if (!HiiSetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VarSize, (UINT8 *) SaSetup, RequestString)) {
        Status = EFI_NOT_FOUND;
      }
      ASSERT_EFI_ERROR (Status);
      FreePool (RequestString);
    }

    FreePool (SaSetup);
  }

  return Status;
}

/**
  This function updates any configuration parameter affected by changing VT-d
  setting. At the moment, the CpuSetup.TmeEnable needs to be set to "DISABLE"
  if VT-d is disabled.

  @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action           Specifies the type of action taken by the browser.
  @param[in] KeyValue         A unique value which is sent to the original
                              exporting driver so that it can identify the type
                              of data to expect.
  @param[in] Type             The type of value for the question.
  @param[in] Value            A pointer to the data being sent to the original
                              exporting driver.
  @param[out] ActionRequest   On return, points to the action requested by the
                              callback function.

  @retval EFI_SUCCESS - The function completed successfully
  @retval EFI_UNSUPPORTED - The Setup Menu was not updated
  @retval EFI_NOT_FOUND - There is an error in Locating protocols or retrieving
                          browser data
**/
EFI_STATUS
EFIAPI
VtdCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  EFI_STATUS              Status;
  SA_SETUP                *SaSetup;
  CPU_SETUP               *CpuSetup;
  VTIO_SETUP_DATA_STRUCT  *VtioSetup;
  UINT32                  VariableAttr;
  UINTN                   SaSize;
  UINTN                   VarSize;
  EFI_STRING              RequestString;

///
///

  Status = EFI_SUCCESS;
  RequestString = NULL;

  //
  // Only process SA_VTD_QUESTION_ID callback
  //
  if (KeyValue != SA_VTD_QUESTION_ID) {
    return EFI_UNSUPPORTED;
  }

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  SaSize = sizeof (SA_SETUP);
  SaSetup = AllocatePool (SaSize);
  ASSERT (SaSetup != NULL);
  if (SaSetup == NULL) {
    DEBUG ((DEBUG_ERROR, "VtdCallBackFunction: Out of resources allocating SaSetup!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  //  Get Browser data by VarStore name (SaSetup)
  //
  if (!HiiGetBrowserData (&gSaSetupVariableGuid, L"SaSetup", SaSize, (UINT8 *) SaSetup)) {
    FreePool (SaSetup);
    DEBUG ((DEBUG_ERROR, "VtdCallBackFunction: SaSetup not found!\n"));
    return EFI_NOT_FOUND;
  }

  //
  //  The only time we change CpuSetup.TmeEnable is when
  //  The VT-d setting is being set to "DISABLE"
  //
  if (Value->u8 == 0) {
    VarSize   = sizeof (CPU_SETUP);
    CpuSetup  = AllocatePool (VarSize);
    ASSERT (CpuSetup != NULL);

    if (CpuSetup == NULL) {
      FreePool (SaSetup);
      DEBUG ((DEBUG_ERROR, "VtdCallBackFunction: Out of resources allocating CpuSetup!\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // GetBrowserData by VarStore Name (CpuSetup)
    //
    if (!HiiGetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup)) {
      FreePool (CpuSetup);
      FreePool (SaSetup);
      DEBUG ((DEBUG_ERROR, "VtdCallBackFunction: CpuSetup not found!\n"));
      return EFI_NOT_FOUND;
    }

    // check to see if TmeEnable is 1, if it is then set it to 0
    if (CpuSetup->TmeEnable == 1) {
      CpuSetup->TmeEnable = 0;
    }

    // check for TdxEnable
    if (CpuSetup->TdxEnable == 1) {
      CpuSetup->TdxEnable = 0;
    }

    //
    //  Construct the Browser string for CpuSetup.TmeEnable and CpuSetup.TdxEnable and then set it.
    //
    RequestString = HiiConstructRequestString (
                      RequestString,
                      OFFSET_OF (CPU_SETUP, TmeEnable),
                      sizeof (CpuSetup->TmeEnable)
                      );

    RequestString = HiiConstructRequestString (
                      RequestString,
                      OFFSET_OF (CPU_SETUP, TdxEnable),
                      sizeof (CpuSetup->TdxEnable)
                      );

    if (RequestString != NULL) {
      if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup, RequestString)) {
        DEBUG ((DEBUG_ERROR, "VtdCallBackFunction: Setting Hii string ERROR!\n"));
        Status = EFI_NOT_FOUND;
      }

      FreePool (RequestString);
    }

    FreePool (CpuSetup);

    // Disable VTIO when VTd is Disabled
    VarSize   = sizeof (VTIO_SETUP_DATA_STRUCT);
    VtioSetup  = AllocatePool (VarSize);
    ASSERT (VtioSetup != NULL);

    if (VtioSetup == NULL) {
      DEBUG ((DEBUG_ERROR, "VtdCallBackFunction: Out of resources allocating VtioSetup!\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    Status  = gRT->GetVariable (
                L"VtioCfg",
                &gVtioSetupGuid,
                &VariableAttr,
                &VarSize,
                VtioSetup
                );

    if (EFI_ERROR(Status)) {
      FreePool (VtioSetup);
      DEBUG ((DEBUG_ERROR, "VtdCallBackFunction: Unable to get Vtio variable!\n"));
      return EFI_NOT_FOUND;
    }

    // check to see if VtioSupport is 1, if it is then set it to 0
    if (VtioSetup->VtioSupport == 1) {
      VtioSetup->VtioSupport = 0;
    }

    Status = gRT->SetVariable (
                L"VtioCfg",
                &gVtioSetupGuid,
                VariableAttr,
                VarSize,
                VtioSetup
                );

    if (EFI_ERROR(Status)) {
      FreePool (VtioSetup);
      DEBUG ((DEBUG_ERROR, "VtdCallBackFunction: Unable to set Vtio variable!\n"));
      return EFI_NOT_FOUND;
    }

    FreePool(VtioSetup);

///
///
  }

  FreePool (SaSetup);

  return Status;
}
