<!--
@file

 This file provide the details for CapsuleFeature.

@copyright
  INTEL CONFIDENTIAL

  Copyright (C) 2022 Intel Corporation

  This software and the related documents are Intel copyrighted materials, and your
  use of them is governed by the express license under which they were provided to
  you ("License"). Unless the License provides otherwise, you may not use, modify,
  copy, publish, distribute, disclose or transmit this software or the related
  documents without Intel's prior written permission.

  This software and the related documents are provided as is, with no express or
  implied warranties, other than those that are expressly stated in the License.

@par Specification Reference:
-->
# Overview
* **Feature Name:** CapsuleFeature
* **PI Phase(s) Supported:** DXE, PEI
* **SMM Required?** Yes

## Purpose
To provide Capsule update and Recovery function of system firmwares (e.g., BIOS, ME, and EC) on client platforms.

# High-Level Theory of Operation
This package depends on FmpDevicePkg and produce different FmpDeviceLib library instances for different firmware components.
On top of those library instances, adding recovery support for power interruption (Seamless Recovery) or BIOS/ME corruption (Resiliency)
P.S. Currently only support SPI boot media.

## Firmware Volumes
FvAdvanced
FvPreMemory
FvBspPreMemory

P.S. If support FSP@reset -> FvBsp; if no support FSP@reset FspFvPreMem

## Modules

   BiosResiliencyDxe       - A driver to backup current BIOS for BIOS corruption case - Copy IBB to IBBR (TopSwap region), copy OBB to external storage
   MeResiliencyDxe         - A driver to detect ME recovery request and trigger ME capsule update for recovering.
   PlatformInitRecoveryPei - A driver to detect recovery requests and change the boot flow accordingly.

## Libraries

   FmpDeviceLibBios       - Library instance to support BIOS update
   FmpDeviceLibBtGAcm     - Library instance to support BootGuard ACM update
   FmpDeviceLibEc         - Library instance to support EC update (Currently only shared ROM EC is supported.)
   FmpDeviceLibIshPdt     - Library instance to support ISH PDT data update
   FmpDeviceLibMe         - Library instance to support ME update
   FmpDeviceLibMonolithic - Library instance to support multiple components update includes BIOS, ME, EC and PDT.
   FmpDeviceLibuCode      - Library instance to support Microcode update

   ComponentUpdateLib
   - Update different components (e.g., BIOS, ME, EC) on IFWI.

   PlatformFlashAccessLib
   - lower level library to program firmware device. Currently only support SPI access.

   FwUpdateLib
   - CSME FWU library for ME update

   PlatformDisplayUpdateProgressLib
   - Library to redirect progress value from GOP to 80h port. This is used for the platform without IGD.

   BiosUpdateHelpersLib
   - To update BIOS according to configuration profile in capsule image. Consumed by FmpDeviceLibBios and FmpDeviceLibMonolithic.

   PlatformBiosUpdateHookLib
   - To get BIOS version and related information from platform and reported in FMP.
     Provide a hook before/after BIOS update for platform specific actions. Consumed by FmpDeviceLibBios and FmpDeviceLibMonolithic.

   PlatformEcUpdateHookLib
   - To get EC version and related information from platform and reported in FMP.
   - Provide a hook before/after EC update for platform specific actions. Consumed by FmpDeviceLibEc and FmpDeviceLibMonolithic.

   PlatformMeUpdateHookLib
   - To get ME type id and related information from platform and reported in FMP.
     Provide a hook before/after ME update for platform specific actions. Consumed by FmpDeviceLibMe and FmpDeviceLibMonolithic.

   PlatformEcResetLib
   - Trigger a EC reset after EC firmware update. Consumed by FmpDeviceLibEc and FmpDeviceLibMonolithic.

   CapsuleUpdateResetLib
   - It's invoked after a successful firmware update:
     1. Changing reset type after certain firmware update type.
     2. Adding callback for required actions before system reset.

   PeiPlatformRecoveryLib
   - To get Capsule update and OBB backup files from external storage.
   - Dispatch FVs in OBB in order to boot to BDS phase for continuing the interrupted update or restoring BIOS/ME firmware.

   PlatformWdtLib
   - To stop or start watchdog timer on platform for control update/recovery process.

   SeamlessRecoverySupportLib
   - To backup required image for an interruption update.

## Key Functions
1. ME FMP          - FwuFullUpdateFromBuffer and FwuCheckUpdateProgress for ME FWU.
2. BIOS FMP        - PlatformFmpGetBiosVersion() and PlatformFmpGetBiosVersionString() to report BIOS version on platform.
3. EC FMP          - PlatformFmpGetEcVersion() and PlatformFmpGetEcVersionString() to report EC version on platform.
                     PlatformEcReset() to enforce EC to reload its firmware after update.
4. BIOS Resiliency - ArmPlatformWdt() and DisarmPlatformWdt() to start monitoring BIOS boot healthiness.

## Configuration

In order to enable BIOS Recovery support, platform BIOS need to figure out its flashmap layout that:

1. Have IBB region resides in the top of BIOS memory space and the size should be smaller than configured TopSwap size.
   The IBB must contain FvBspPreMemory that includes recovery module and the storage modules (NVMe, SATA) able to read files in PEI phase.
2. Have OBB region that contains required module to boot to EndOfDxe phase and continuate capsule update process.

Below PCDs should be configured according to platform flashmap layout:
  PcdFlashIbbBase,  PcdFlashIbbOffset,  PcdFlashIbbSize
    - Initial boot block region
  PcdFlashIbbRBase, PcdFlashIbbROffset, PcdFlashIbbRSize (BIOS Resiliency only)
    - Recovery initial boot block region - Size should be identical to IBB and address should be next to it
  PcdFlashObbBase,  PcdFlashObbOffset,  PcdFlashObbSize
    - Boot blocks contains required module to boot to EndOfDxe phase and continuate capsule update process. (e.g., storage driver, FMP drivers, etc.)
  PcdObbRecoveryRequiredFvGuid
    - FVs in OBB that need to be dispatched in recovery path.
  PcdNonFitPayloadRecoveryRequiredFvGuid
    - FVs in NonFitPayload that need to be dispatched in recovery path.

  PcdTopSwapEnableSwSmi, PcdTopSwapDisableSwSmi
    - SMIs to trigger TopSwap

## Data Flows

1. The data structure to keep firmware component update progress for recovery/resiliency usage.

  typedef enum {
    NotStarted              = 0,
    UpdatingMe              = 1,
    UpdatingIshPdt          = 2,
    UpdatingBios            = 3,
    UpdatingEc              = 4,
    UpdatingGbE             = 5,
    UpdatingBtGAcm          = 7,
    UpdatinguCode           = 8,
    UpdatingResiliency      = 9, // Post-BIOS update phase. Indicates BIOS backup requirement.
    UpdatingMeResiliency    = 10,
    UpdatingTypeMax
  } SYSTEM_FIRMWARE_COMPONENT;

  typedef enum {
    BiosIbbR     = 0,
    BiosIbb,
    BiosObb,
    BiosMax
  } BIOS_UPDATE_PROGRESS;

  typedef enum {
    EcMainImage        = 0,
    EcRecoveryImage    = 1,
    EcMax
  } EC_UPDATE_PROGRESS;

  typedef struct {
    SYSTEM_FIRMWARE_COMPONENT  Component;
    UINT32                     Progress;
  } SYSTEM_FIRMWARE_UPDATE_PROGRESS;

2. To support Microcode update, must contain below data structure in Microcode FV:

  typedef struct {
    UINT32  Version;
    UINT32  LowestSupportedVersion;
    CHAR16  VersionString[0];
  } INTEL_MICROCODE_VERSION_FFS_DATA;

## Control Flows
1. When there is capsule detected by platform changes boot mode is BOOT_ON_FLASH_UPDATE,
   FmpDeviceSetImage in provided library instance will be invoked to program firmware device according ImageTypeId in capsule image.
2. When TopSwap is enabled or SysFwUpdateProgress variable exists, it means there is an interrupted update or a recovery request.
   Boot mode will be changed to BOOT_ON_FLASH_UPDATE again in order to disable BIOS protection and, either continuing the update or restoring BIOS.

## Build Flows
- Adding -DFSP_ARCH=IA32 or -DFSP_ARCH=X64 to build cmd is required for CapsuleFeaturePkg standalone build.
  - **-DFSP_ARCH=IA32** - Build PEI components in 32bit
  - **-DFSP_ARCH=X64** - Build PEI components in 64bit

## Test Point Results
The FMP devices with below GUID will be installed and observed in ESRT table:
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_MONO          = 7FEB1D5D-33F4-48D3-BD11-C4B36B6D0E57 # gFmpDeviceMonolithicDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_BIOS          = 6C8E136F-D3E6-4131-AC32-4687CB4ABD27 # gFmpDeviceBiosDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_EXTENDED_BIOS = FD0AADC7-9696-4795-8F34-53069F759FB0 # gFmpDeviceExtendedBiosDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_BTGACM        = 4E88068B-41B2-4E05-893C-DB0B43F7D348 # gFmpDeviceBtGAcmDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_UCODE         = 69585D92-B50A-4AD7-B265-2EB1AE066574 # gFmpDeviceuCodeDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_FSP           = 5E8F2C7B-6D4F-4A9E-BF6D-8A7E9B0F3C2A # gFmpDeviceFspDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_ME            = 0EAB05C1-766A-4805-A039-3081DE0210C7 # gFmpDeviceMeDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_EC            = 3DD84775-EC79-4ECB-8404-74DE030C3F77 # gFmpDeviceEcDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_ISH_PDT       = 76CA0AD8-4A14-4389-B7E5-FD88791762AD # gFmpDeviceIshPdtDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_FOXVILLE             = 6E36078B-6B6F-42BC-981E-4C639AFA4D6D # gFmpDeviceFoxvilleDefaultGuid

## Functional Exit Criteria
1. Capsule update    - Capsule update is triggered successfully and the version and status is correctly reported in ESRT.
2. Seamless Recovery - After a power interruption during update, update can proceed after re-applying power.
3. Resiliency        - A BIOS/ME corruption lead to BIOS/ME FW restore.

## Feature Enabling Checklist
1. For BIOS update, make sure below library instances are implemented in board:
   PlatformBiosUpdateHookLib

2. For EC update, make sure below library instances are implemented in board:
   PlatformEcUpdateHookLib
   PlatformEcResetLib

3. For ME update, make sure below library instances are implemented in board:
   PlatformMeUpdateHookLib
   FwUpdateLib

4. For BIOS Seamless Recovery / Resiliency support, make sure flashmap is configured according to TopSwap setting.
   See ## Configuration section above

## Performance Impact
Minor impact

## Common Optimizations
Common size or performance tuning options for this feature.

This section is recommended but not required. If not used, the contents should be left empty.

## Unit Test build flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

> Step1. Prepare build environment with VS2019 as needed.
> Step2. Install the pip requirements.
         Sample : pip install -r Edk2/pip-requirements.txt
> Step3. Setup Edk2 environment.
         Sample : Edk2/edksetup.bat Rebuild
> Step4. Get the code dependencies.
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p CapsuleFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p CapsuleFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p CapsuleFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
