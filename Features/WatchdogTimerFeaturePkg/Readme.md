<!--
@file

 This file provide the details for WatchdogTimerFeature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials, and your
  use of them is governed by the express license under which they were provided to
  you ("License"). Unless the License provides otherwise, you may not use, modify,
  copy, publish, distribute, disclose or transmit this software or the related
  documents without Intel's prior written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  This software and the related documents are provided as is, with no express or
  implied warranties, other than those that are expressly stated in the License.
-->
# Overview
* **Feature Name:** WatchdogTimerFeaturePkg
* **PI Phase(s) Supported:** DXE
* **SMM Required?** NO

## Purpose
This provides Watchdog Timer protocol access for application

# High-Level Theory of Operation
This package provides advanced feature functionality for Watchdog Timer Feature support.

## Firmware Volumes
* PreMemory.
* PostMemory.

## Modules
WdtAppPei.
WdtAppDxe.

## <WdtAppPei>
Register a Notify Callback when WDT PPI had been installed

## <WdtAppDxe>
WdtAppDxe driver provides Feedwatchdog and RequestWdt.

## <WdtAppPeiLib>
Turns on WDT during PEI phase according to requests made by
OS overclocking application (through WDT status) and BIOS modules (through flash variable)

## Key Functions


## Configuration
PcdWatchdogTimerFeatureEnable  WDT feature Enable/Disable switch
PcdWdtTimeoutValue             WDT timeout value
PcdWdtReloadTimer              Time Periodic for Feedwatchdog
PcdWdtTimeoutBetweenPeiDxe     WDT timeout value between PEI and DXE

## Data Flows
Driver will use above Pcds to controll Watchdog Timer

## Control Flows
Driver will check WDT PPI is installed or not, if it is installed, we turn on WDT according to requests made by
OS overclocking application (through WDT status) and BIOS modules (through flash variable).

## Build Flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

>     Step1. prepare build environment with VS2019 or CLANG11 as needed
>     Step2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features/
>     Step3. Edk2\edksetup.bat Rebuild
>     Step4. build -p WatchdogTimerFeaturePkg/WatchdogTimerFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b DEBUG
>     Step5. build -p WatchdogTimerFeaturePkg/WatchdogTimerFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b RELEASE
>     Step6. build -p WatchdogTimerFeaturePkg/WatchdogTimerFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b DEBUG
>     Step7. build -p WatchdogTimerFeaturePkg/WatchdogTimerFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b RELEASE

## Test Point Results
Verified by Intel XTU tool.

## Functional Exit Criteria
Intel XTU tool

## Feature Enabling Checklist
Include WatchdogTimerFeature.dsc and PcdWatchdogTimerFeatureEnable

## Performance Impact
Minimal impact as data parsing and install via WDT protocol

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
         Sample : stuart_setup -c Intel/Features/WatchdogTimerFeaturePkg/CISettings.py -p WatchdogTimerFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/Features/WatchdogTimerFeaturePkg/CISettings.py -p WatchdogTimerFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/Features/WatchdogTimerFeaturePkg/CISettings.py -p WatchdogTimerFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
