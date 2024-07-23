<!--
@file

 This file provide the details for Platform Service Record feature.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2022 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
-->
# Overview
* **Feature Name:** Platform Service Record
* **PI Phase(s) Supported:** DXE
* **SMM Required?** NO

## Purpose

Display Genesis Record, Platform Event Ledger, Platform Power State Historical Counter information in bios setup page and allow user to export the whole data blob into USB storage.

Currently supported function:
* Display Genesis Record
* Display Platform Event Ledger
* Display Platform Power State Historical Counter
* Export PSR Blob to USB storage
* Notify CSME various PSR events.

# High-Level Theory of Operation
1.PsrDxe driver will update PSR capability and install PlatformServiceRecordProtocol during post.
2.PsrSetup driver will check the PlatformServiceRecordProtocol is installed or not.
3.Will pubilish the Hii data if the protocol is been installed.

Feature package contains:

* Drivers
  * **PsrDxe**   (*PlatformServiceRecord.inf*)
    * This driver is responsible for installing the PlatformServiceRecordProtocol for usage.
  * **PsrSetup** (*PsrSetup.inf*)
    * This driver is responsible for publishing HII data and [Config Access Protocol].
## Modules

* PlatformServiceRecord.inf
* PsrSetup.inf

### PlatformServiceRecord.inf
  Update PSR capability based on VpdPcdPsrSupportedCapabilities.
  - NotifyPsrCapabilitySet
    - Inform CSME of updated PSR Capability list.

  Provide PLATFORM_SERVICE_RECORD_PROTOCOL service for usage.
  - GetPsrData
    - Get PSR from CSME via HECI cmds and extract the encrypted data blob. (CSME is not ready)
  - ExportPsrToUsbStorage
    - Export PSR to selected Usb Storage.
  - NotifyChassisIntrusionDetection
    - Inform CSME of Chassis Intrusion via HECI cmds.
  - NotifyCriticalTempShutdown
    - Inform CSME of Critical Temperature Shutdown event via HECI cmds only when PSR log state is started.
  - NotifyEventReport
    - Inform CSME of Firmware Recovery, Firmware Update, System Hang and Power Drop events via HECI interface only when PSR log state is started.

### PsrSetup.inf
  Publish setup form to display PSR related data.

typedef struct {
  UINT8            PsrLogState;
  PSR_VERSION      PsrVersion;
  UINT8            Psrid[16];
  EFI_UPID         Upid;
  UINT8            Capabilities[PSR_MAX_SUPPORTED_CAPABILITIES];
  UINT32           GenesisDate;
  CHAR16           OemInfo[32];
  CHAR16           OemMakeInfo[32];
  CHAR16           OemModelInfo[32];
  CHAR16           ManufCOE[32];
  CHAR16           OemDataStore[512];

  UINT32           S0RuntimeCtr;
  UINT32           WarmResetCtr;
  UINT32           S0toS5Ctr;
  UINT32           S0toS4Ctr;
  UINT32           S0toS3Ctr;
  UINT32           PrtcFailureCtr;
  UINT32           CsmeInvalidStateCtr;
  UINT32           CsmeDamCtr;
  UINT32           CsmeUnlockedCtr;
  UINT32           SvnIncreaseCtr;
  UINT32           ExcessiveShockCtr;
  UINT32           ExcessiveOperationalTempCtr;
  UINT32           IshConnectionCtr;
  UINT32           CsmeResetCtr;
  UINT32           FwRecoveryCtr;
  UINT32           FwUpdateCtr;
  UINT32           SysHangCtr;
  UINT32           PwrDropCtr;

  UINT32           TotalEventCount;
  PSR_EVENTS       Events[PSR_MAX_EVENT_COUNT];
  CSME_FW_VERSION  FwVersion;
  UINT8            Rsvd[32];
} PSR_CONFIGURATION;

## Library

* No library created by this feature.

## Key Functions
* Display Platfrom Service Record having system related information like Genesis Record, Platform Event Ledger, Platform Power State Historical Counter information in bios setup page and,
* Allow user to export the whole data blob to a selected USB storage.

## Configuration
* Set gPsrFeatureModuleTokenSpaceGuid.PcdPlatformServiceRecordSupport to TRUE to enable PSR feature
* Set gPsrFeatureModuleTokenSpaceGuid.VpdPcdPsrSupportedCapabilities to enable/disable specific PSR capability depends on design.
* Set gPsrFeatureModuleTokenSpaceGuid.PcdPsrSetupConfiguration to determine formset display location,
For **client platform**. The definition of each field is listed below:
  - Bit 31    --- Whether to show formset, 1 - show, 0 - hide
  - Bit 30:24 --- Reserved
  - Bit 23:16 --- Formset display order
  - Bit 15:0  --- The id of form to display formset under Advanced menu

## Data Flows
* Send HECI cmd to CSME to get an encrypted data blob, and do extract.
  Save the blob to PSR configuration, also can export it to selected USB storage.

## Control Flows
* N/A

## Build Flows

### Prerequisite
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

1. Install the pip requirements.\
   `pip install -r Edk2/pip-requirements.txt`

### Build the package
1. Setup Edk2 environment.\
   `python Edk2/BaseTools/Edk2ToolsBuild.py -t VS2019`
2. Get the code dependencies.\
   `stuart_setup -c Intel/.pytool/CISettings.py -p PsrFeaturePkg -t DEBUG,RELEASE -a IA32,X64`
3. Update other dependencies.\
   `stuart_update -c Intel/.pytool/CISettings.py -p PsrFeaturePkg -t DEBUG,RELEASE -a IA32,X64`
4. Build unit test host.\
   `stuart_ci_build -c Intel/.pytool/CISettings.py -p PsrFeaturePkg -t DEBUG,RELEASE -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`

## Test Point Results

* Display Blob Data
  Platform Service Record information will display sample data in setup.

* Detect Valid USB Storages and List
  Valid USB storages will list when enter Export Platform Service Record To USB submenu in bios setup.

* Export PSR Data
  Select a USB storage in Export Platform Service Record To USB submenu, it will pop up message box to show the action is execute successfully or not.
  If yes, can found a file with name as "PlatformServiceRecord_YYYY/MM/DD_HH:MM:SS.bin" in the selected USB storage device.

## Functional Exit Criteria

* Once the feature is enabled on the platform, Intel(R) Platform Service Record menu should be visible in the BIOS setup browser.

## Feature Enabling Checklist

1. Add PsrFeature entry and path to your build file and check if all required packages/libraries exists for this package.
2. Add PsrFeature.fdf entry and path to your flash map file.

## Performance Impact
* Minimal impact on performance, major impact in setup for sending HECI cmd to get PSR Blob.
* Add PlatformServiceRecord driver to install protocal service and PsrSetup to publish setup form during boot time.

## Common Optimizations
* To Prevent sending HECI cmd too many times in setup, set flag to check the first time that user enter PSR setup menu.
  Only send cmd once when user enter the PSR setup menu.

## Unit Test build flows

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

- Step1. Prepare build environment as needed.
   - Visual Studio VS2019
   - Python 3.10
   - NASM 2.15.05
   - OpenCppCoverage
   - virtualenv
- Step2. Perform a one-time setup of python virtual environments.
   - `py -m venv unit-testing`
- Step3. Activate the python virtual environment.
   - `./unit-testing/Scripts/activate`
- Step4. Install the pip requirements.
   - `pip install -r Edk2/pip-requirements.txt`
- Step5. Setup Edk2 environment.
   - `python Edk2/BaseTools/Edk2ToolsBuild.py -t VS2019`
- Step6. Get the code dependencies.
   - `stuart_setup -c Intel/.pytool/CISettings.py -p PsrFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`
- Step7. Update other dependencies.
   - `stuart_update -c Intel/.pytool/CISettings.py -p PsrFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`
- Step8. Build unit test host.
   - `stuart_ci_build -c Intel/.pytool/CISettings.py -p PsrFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`
