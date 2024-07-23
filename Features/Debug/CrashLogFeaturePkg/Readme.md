<!--
@file

 This file provide the details for CrashLogFeaturePkg.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
* **Feature Name:** CrashLogFeaturePkg
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
To install ACPI BERT table with crashlog record

# High-Level Theory of Operation
This package install ACPI BERT table with crashlog record.

## Firmware Volumes
* PostMemory

## Configuration
* Set gCrashLogFeaturePkgTokenSpaceGuid.PcdCrashLogFeatureEnable to TRUE to enable CrashLog feature.

## Key Functions
* CrashLogDxeEntry

## CrashLogDxe
Once each crashlog agent gets crashlog successfully, it will store the data to a HOB structure during PEI phase.
CrashLogDxe driver will locate each HOB data and copy the data into ACPI BERT table.
The HOB structure detail can be found in CrashLogHob.h

## Build Flows

* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

> Step1. prepare build environment with VS2019 or CLANG11 as needed
> Step2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features/Debug/
> Step3. Edk2\edksetup.bat
> Step4. build -p CrashLogFeaturePkg/CrashLogFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b DEBUG
> Step5. build -p CrashLogFeaturePkg/CrashLogFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b RELEASE
> Step6. build -p CrashLogFeaturePkg/CrashLogFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b DEBUG
> Step7. build -p CrashLogFeaturePkg/CrashLogFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b RELEASE

## Test Point Results
Verified by bios debug log.

## Functional Exit Criteria
Check if ACPI BERT table is installed on OS.

## Feature Enabling Checklist
Include CrashLogFeature.dsc

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
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p CrashLogFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p CrashLogFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p CrashLogFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose