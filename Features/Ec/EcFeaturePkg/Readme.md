<!--
@file

 This file provides the details for Embedded Controller feature.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2021 Intel Corporation.

  The source code contained or described herein, and all documents related to the
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
* **Feature Name:** Embedded Controller
* **PI Phase(s) Supported:** PEI, DXE
* **SMM Required?** NO

## Purpose
This feature package implements for Embedded Controller of Intel RVP related platform features only.
Currently supported functions:
* Power Plane Management
* Thermal Management
* Battery Charging Management
* Keyboard Inputs
* Button Array Support
* Peripheral Devices

# High-Level Theory of Operation
* The interface between EC and OS communicates, BIOS receive the data from EC and also send command/data to EC through IO port 0x66/0x62.
* Libraries from EcFeaturePkg provide APIs to platform code usage to communicate with EC.
* Platform code shall initialize EC via lib in EcFeaturePkg according platform setting / setup.

Feature package contains:

* Drivers
 * EcAcpiTables
 * EcDxe

## EcAcpiTables.inf
  - Data raw include Ecdt and EcSsdt ACPI tables

## EcDxe.inf
  - Get the Ecdt and EcSsdt from FV and install them
  - Update the EcSsdt EcNvs data region base on gEcNvsAreaProtocolGuid
  - Update the Ecdt GPE bit base on gEcNvsAreaProtocolGuid

## <Library Name>
* EcLib
* EcPrivateLib
* EcMiscLib
* EcSmiLib
* EcTcssLib
* EcSmiLib

## Key Functions
C code:
* SendEcCommand
* SendEcData
* ReceiveEcData

ACPI code:
* EcDevices under H_EC scope:
  - Ambient Light Sensor, Batterys, Button Indicator, Convertible Indicator, Docking Indicator,
    DPTF, LID switch and WDT
* EcDevices under _SB scope:
  - AC and Power Button
* ECRD, ECWT and ECMD: base methods to implement all EC functions

## Configuration
* Set gEcFeaturePkgTokenSpaceGuid.PcdEcEnable to TRUE will include EC feature libraries
* Set gEcFeaturePkgTokenSpaceGuid.PcdEcEnable to FALSE will include EC feature with Null libraries

## Data Flows
* Send EC command through Port 0x66, and data through 0x62.

## ACPI Flows
* The gEcNvsAreaProtocolGuid would be installed in AdvancedAcpiDxe driver
  and it would be consumed by EcDxe to update the data in Ecdt and EcSsdt
* EcDxe would install the Ecdt and EcSsdt ACPI tables
* The functions inside EcSsdt would use base methods ECRD, ECWT and ECMD
* All external usage APIs is defined in EcExternalMethod.asl

## Control Flows
* N/A

## Build Flows

* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

> Step1. prepare build environment with VS2019 or CLANG11 as needed
> Step2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features/Ec/
> Step3. Edk2\edksetup.bat Rebuild
> Step4. build -p EcFeaturePkg/EcFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b DEBUG
> Step5. build -p EcFeaturePkg/EcFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b RELEASE
> Step6. build -p EcFeaturePkg/EcFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b DEBUG
> Step7. build -p EcFeaturePkg/EcFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b RELEASE

## Test Point Results

* Detect Board ID
  EC will detect board ID of Intel RVP configuration and start required initialization. The board ID will be shown in the log "Fields.BoardId from EC is 0x%x\n"

* Send EC command
  Send EC command through IO port 0x66, and the command could be found in the log as "Sending EC Command: %02X\n"]

@todo capture more test result here, such as what kind of EC functionalities is verified with this package

## Functional Exit Criteria
* Succeeding to send command to EC without any timeout, and the status can be found after "SendEcCommand status %x\n"

## Feature Enabling Checklist
  Make sure platform code perform EC initialization, for example:
   - Perform EC initialization via lib in this package according to platform setting
   - Detect DTT Participants from EC
   - Show or suppress fan participants in DTT setup based on the above detection
   - Enable reporting memory temperatures to the processor via PECI based on SA setup option
   - Enable EC low power mode based on setup option
   - Update device power based on Pseudo G3 State
   - Enable EC Turbo Control Mode based on CPU setup option
  Add EcFeatureLib.dsc to your build file and check if all required packages/libraries exist for this package.

  AdvancedAcpiDxe.inf
  - Detect DTT Participants from EC
  - Show or suppress fan participants in DTT setup based on the above detection
  - Enable reporting memory temperatures to the processor via PECI based on SA setup option
  - Enable EC low power mode based on setup option
  - Update device power based on Pseudo G3 State
  - Enable EC Turbo Control Mode based on CPU setup option

## Remain to improve
* ACPI part
  - There are many external datas and methods defined on platform side used in EC ACPI tables
    consider to remove the dependency.
  - EcNVS is installed on platform side (AdvancedAcpiDxe.inf) because the EcNVS should be updated
    base on some of the Setup variables. Consider to move all EC related options to EC independent
    setup driver and install the EcNVS.

## Performance Impact
* If EC is not responding while receiving commands, there would be an EC_TIMEOUT (200 ms.)

@todo What's the post time impact if EC is enabled? code size impact ?

## Common Optimizations
* N/A
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
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p EcFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p EcFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p EcFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose