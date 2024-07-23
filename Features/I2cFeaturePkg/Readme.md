<!--
@file

 This file provide the details for Platform I2C feature.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2021 Intel Corporation.

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
* **Feature Name:** I2C feature
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
This feature package implements I2C related platform features.

# High-Level Theory of Operation
This feature provides I2C related features:

1. I2cPlatformDxe.inf Implements the EFI driver binding driver to produce I2cEnumerateProtocol and
  I2cBusConfigurationManagementProtocol which is later consumed by I2cBus driver
  to enumerate and configure I2C device.Platform varying information are passed from below PCDs,
  these PCDs are initialized in the I2cPlatformDxeHookLib Library constructor
    PcdI2cFrequencyDataMap
    PcdSerialIoI2cDevId
    PcdI2cControllerDevList
2. I2cTouchPanelDxe.inf Implements the EFI driver binding driver to produce AbsolutePointerProtocol and
  initialize touch panel hardware in platform-specific way
3. I2cPssDxe.inf Implements the EFI driver binding driver to produce
  I2cPssProtocol which provides the board serial number from PSS chip.
  Platform could decide the S/N source from PSS chip or not through PcdI2cPssEnable.
## Firmware Volumes
* FvAdvancedPostMemory

## Modules
* I2cPlatformDxe
* I2cTouchPanelDxe
* DxeHidI2cPlatformSupportLibNull
* I2cPssMonzaDxe

## I2cPlatformDxe
The driver produce I2cEnumerateProtocol and I2cBusConfigurationManagementProtocol

## I2cTouchPanelDxe
The driver produce AbsolutePointerProtocol and initialize touch panel hardware in platform-specific way

## Key Functions
* `SetConfiguration`

  Since our platforms support only a single configuration per i2c bus,
  this function configures i2c central for correct bus speed

## DxeHidI2cPlatformSupportLibNull
A NULL implementation of library DxeHidI2cPlatformSupportLib, Platform maybe implemented its own DxeHidI2cPlatformSupportLib.

## I2cPssMonzaDxe
The driver produce I2cPssProtocol

## Configuration
* PcdI2cFeatureEnable - Enables this feature.
* PcdI2cFrequencyDataMap - Serial I2C Controller frequency Information table
* PcdSerialIoI2cDevId - Serial Io I2c DeviceID List
* PcdI2cControllerDevList - Serial Io I2c Device List for all controllers
* There is no special configuration for I2cTouchPanelDxe.
  the platform maybe implemented its own DxeHidI2cPlatformSupportLib.
* PcdI2cPssEnable - Get the Board S/N from PSS chip.
## Data Flows
Consumes below PCD to produces I2cEnumerateProtocol and I2cBusConfigurationManagementProtocol with other settings
  PcdI2cFrequencyDataMap
  PcdSerialIoI2cDevId
  PcdI2cControllerDevList

## Control Flows
This feature is only controlled by PcdI2cFeatureEnable

## Build Flows
* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

>     Step1. prepare build environment with VS2019 or CLANG11 as needed
>     Step2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features;%cd%/Edk2Platforms/Features/Intel/UserInterface;
>     Step3. Edk2\edksetup.bat Rebuild
>     Step4. build -p I2cFeaturePkg/I2cFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b DEBUG
>     Step5. build -p I2cFeaturePkg/I2cFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b RELEASE
>     Step6. build -p I2cFeaturePkg/I2cFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b DEBUG
>     Step7. build -p I2cFeaturePkg/I2cFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b RELEASE

## Test Point Results
* Soft-keyboard verification
  verify I2C touch panel works via virtual keyboard in BIOS setup, Expecting I2cTouchPanelDxe driver to be enabled

* Dump protocol handles for below protocol's
  Command : Dh -p I2cEnumerateProtocol
            Dh -p I2cBusConfigurationManagementProtocol
            Dh -p AbsolutePointerProtocol
            Dh -p I2cPssProtocol

## Functional Exit Criteria
Dump for below protocol's in BIOS shell to Displays protocol information associated to handles
  Command : Dh -p I2cEnumerateProtocol
            Dh -p I2cBusConfigurationManagementProtocol
            Dh -p AbsolutePointerProtocol
            Dh -p I2cPssProtocol

## Feature Enabling Checklist
1. Add I2cFeaturePkg entry and path to your build file and check if all required packages/libraries exists for this package.
2. Add I2cFeaturePkg PostMemory.fdf entry and path to your flash map file.
3. Ensure other required PCDs are updated in platform code.

## Performance Impact
None or Minimal impact

## Common Optimizations

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
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p I2cFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p I2cFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p I2cFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose