<!--
@file

 This file provide the details for PmaxFeaturePkg.

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
* **Feature Name:** PmaxFeaturePkg
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
To prvide SSDT with ACPI PMAX device.

# High-Level Theory of Operation
This package update and install SSDT with ACPI PMAX device for PMAX feature support.

## Firmware Volumes
PmaxDxe

## Key Functions
FindPmaxTable
UpdatePmaxTable
InstallPmaxTable
InstallTable

## Configuration
VpdPcdPmaxDevices


## Data Flows
Drivers will use VpdPcdPmaxDevices to update SSDT.

## Control Flows
PcdPmaxFeatureEnable to enable this feature.

## Build Flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

>     Step1. prepare build environment with VS2019 or CLANG11 as needed
>     Step2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features/PowerManagement
>     Step3. Edk2\edksetup.bat Rebuild
>     Step4. build -p PmaxFeaturePkg/PmaxFeaturePkg.dsc -a X64 -t VS2019 -b DEBUG
>     Step5. build -p PmaxFeaturePkg/PmaxFeaturePkg.dsc -a X64 -t VS2019 -b RELEASE
>     Step6. build -p PmaxFeaturePkg/PmaxFeaturePkg.dsc -a X64 -t CLANGPDB -b DEBUG
>     Step7. build -p PmaxFeaturePkg/PmaxFeaturePkg.dsc -a X64 -t CLANGPDB -b RELEASE

## Test Point Results
ACPI PMAX device has updated and installed correct.

## Functional Exit Criteria
1. FindPmaxTable - Find PMAX SSDT Table and return EFI_SUCCESS.
2. UpdatePmaxTable - Get PCD and update the values and return EFI_SUCCESS.
3. InstallPmaxTable - PMAX table installed and return EFI_SUCCESS.

## Feature Enabling Checklist
PcdPmaxFeatureEnable enabled.


## Performance Impact

## Common Optimizations

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
             Sample : stuart_setup -c Intel/.pytool/CISettings.py -p PmaxFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
>     Step5. Update other dependencies.
             Sample : stuart_update -c Intel/.pytool/CISettings.py -p PmaxFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
>     Step6. Build unit test host.
             Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p PmaxFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose