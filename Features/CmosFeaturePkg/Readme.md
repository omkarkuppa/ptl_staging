<!--
@file

 This file provide the details for CMOS Feature.

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
* **Feature Name:** CmosFeaturePkg
* **PI Phase(s) Supported:** Agnostic
* **SMM Required?** NO

## Purpose
CMOS memory in a computer is a type of NVRAM that stores BIOS settings and date/time information. CMOS lib helps to read and update CMOS address

# High-Level Theory of Operation
This package provides advanced feature functionality for CMOS Feature support.

## Firmware Volumes


## <Library Name>
CmosAccessLibNull
PlatformCmosAccessLib
PlatformCmosAccessLibNull
PlatformNvRamHookLibCmos

## Key Functions
CmosInit
CmosRead8
CmosWrite8
CmosRead16
CmosWrite16
CmosRead32
CmosWrite32
PlatformCmosGetNmiState
PlatformCmosGetEntry

## Configuration
PcdPlatformCmosAccessSupport              Used to enable/Disable the Null instance of CMOS libraries
PcdCmosFastBootDefaultValue               default value for CMOS_FAST_BOOT_REG
PcdCmosDebugPrintErrorLevelDefaultValue   default value for CMOS_DEBUG_PRINT_LEVEL_REG masked with 0xff
PcdCmosStatusCodeFlags                        default value for CMOS_DEBUG_PRINT_ENDPOINTS

## Data Flows
Drivers will use above CMOS Libraries to read and update CMOS address

## Control Flows


## Build Flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

>     Step1. prepare build environment with VS2019 or CLANG11 as needed
>     Step2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features/
>     Step3. Edk2\edksetup.bat Rebuild
>     Step4. build -p CmosFeaturePkg/CmosFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b DEBUG
>     Step5. build -p CmosFeaturePkg/CmosFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b RELEASE
>     Step6. build -p CmosFeaturePkg/CmosFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b DEBUG
>     Step7. build -p CmosFeaturePkg/CmosFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b RELEASE

## Unit Test build flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

>     Step1. Prepare build environment with VS2019 as needed.
>     Step2. Install the pip requirements.
             Sample : pip install -r Edk2/pip-requirements.txt
>     Step3. Setup Edk2 environment.
             Sample : Edk2/edksetup.bat Rebuild
>     Step4. Get the code dependencies.
             Sample : stuart_setup -c Intel/.pytool/CISettings.py -p CmosFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
>     Step5. Update other dependencies.
             Sample : stuart_update -c Intel/.pytool/CISettings.py -p CmosFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
>     Step6. Build unit test host.
             Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p CmosFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose


