<!--
@file

 This file provide the details for ClientSmbiosFeature.

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
* **Feature Name:** ClientSmbiosFeaturePkg
* **PI Phase(s) Supported:** DXE
* **SMM Required?** NO

## Purpose
This produces a reasonably rich set of SMBIOS tables for a typical Intel client system.

# High-Level Theory of Operation
This package provides advanced feature functionality for Smbios Feature support.

## Firmware Volumes
FvAdvanced.

## Modules
SmbiosMiscDxe.

## <SmbiosMiscDxe>
SmbiosMisc driver parses the SmbiosPlatformInfo structure and reports any generated data using SMBIOS protocol.

## <Library Name>
No library in this package.

## Key Functions
No key function while entry point of that Dxe driver will be executed once it's loaded.

## Configuration
PcdSmbiosPlatformInfo       Point address of Smbios Platform Info Table
PcdSmbiosPlatformInfoSize   Size of Smbios Platform Info Table
PcdPasswordStatusAdminName  AdminName for Smbios Type 14
PcdPasswordStatusUserName   UserName for Smbios Type 14

## Data Flows
Driver will use above Pcds to generate misc SMBIOS table and update some platform information.

## Control Flows
Driver will check PcdSmbiosPlatformInfo is zero or not, if it is no value yet, we create a callback to check again.
Create some callback function to update platform information with above Pcds.

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
   `stuart_setup -c Intel/.pytool/CISettings.py -p ClientSmbiosFeaturePkg -t DEBUG,RELEASE -a IA32,X64`
3. Update other dependencies.\
   `stuart_update -c Intel/.pytool/CISettings.py -p ClientSmbiosFeaturePkg -t DEBUG,RELEASE -a IA32,X64`
4. Build unit test host.\
   `stuart_ci_build -c Intel/.pytool/CISettings.py -p ClientSmbiosFeaturePkg -t DEBUG,RELEASE -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`

## Test Point Results
Verified by SMBIOS view log.

## Functional Exit Criteria
Check SMBIOS table dump from SMBIOS view or other SMBIOS tool.

## Feature Enabling Checklist
Include ClientSmbiosFeature.dsc and enable PcdSmbiosFeatureEnable.

## Performance Impact
Minimal impact as data parsing and table update via SMBIOS protocol

## Common Optimizations

## Unit Test build flows

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
   `stuart_setup -c Intel/.pytool/CISettings.py -p ClientSmbiosFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`
3. Update other dependencies.\
   `stuart_update -c Intel/.pytool/CISettings.py -p ClientSmbiosFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`
4. Build unit test host.\
   `stuart_ci_build -c Intel/.pytool/CISettings.py -p ClientSmbiosFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`
