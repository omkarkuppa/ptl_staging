<!--
@file

 This file provide the details for Platform NVMe feature.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2021 - 2022 Intel Corporation.

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
* **Feature Name:** platform NVMe feature
* **PI Phase(s) Supported:** PEI, DXE
* **SMM Required?** No

## Purpose
This feature package implements NVMe related platform features.

# High-Level Theory of Operation
This feature provides NVMe related platform features:

1. NvmePciHcPei.inf will call a library function to get the PCI bridge information, and scan PCI devices on PCI bus. Platform can implement its own library.
2. PowerLossNotifyDxe.inf will get platform policy and perform platform configuration via POWER_LOSS_NOTIFY_SETTING_PROTOCOL.

## Firmware Volumes
* PostMemory

## Modules
* NvmePciHcPei
* PowerLossNotifyDxe
* NvmePcieRpLibNull

## NvmePciHcPei
The driver scans NVMe devices on PCI bus and provides NVMe controller information via EDKII_NVM_EXPRESS_HOST_CONTROLLER_PPI.

## PowerLossNotifyDxe
The driver send command to control PLN feature on NVMe devices, and perfrom platform configuration via POWER_LOSS_NOTIFY_SETTING_PROTOCOL.

## NvmeEraseDxe
The driver detect the nvme EFI_BLOCK_IO_PROTOCOL installed and install the NVME_ERASE_BLOCK_PROTOCOL to the controller for nvme sanitization functionality.

## NvmePcieRpLibNull
A NULL implemention of library NvmePcieRpLib, it return NULL for function GetNextRootPortComplex().
Platform maybe implemente its own NvmePcieRpLib.

## Configuration
There is not special configuration.
But the platform maybe implement its own NvmePcieRpLib.
Set PcdNvmeFeatureEnable to TRUE to enable Nvme feature support.
Set PcdNvmePciScanEnable to TRUE to enable nvme pci scan and programming
Set PcdNvmePlnEnable to TRUE to enable nvme power lost notification
Set PcdNvmeEraseEnable to TRUE to enable nvme sanitization

## Data Flows
Architecturally defined data structures and flows for the feature.

### Data flows of NvmePciHcPei
The driver install EDKII_NVM_EXPRESS_HOST_CONTROLLER_PPI which is consumed by EDK2 NVMe drivers.

### Data flows of PowerLossNotifyDxe
The driver consumed POWER_LOSS_NOTIFY_SETTING_PROTOCOL which is installed by platform.

### Data flows of NvmeEraseDxe
The driver install NVME_ERASE_BLOCK_PROTOCOL which is consumed by platform.

## Control Flows
Consumer gets NVMe controller configuration via EDKII_NVM_EXPRESS_HOST_CONTROLLER_PPI.

## Build Flows
No any special build flows is needed.

## Test Point Results
Verified by bios debug log.

## Functional Exit Criteria
Check by bios debug log to see if EDKII_NVM_EXPRESS_HOST_CONTROLLER_PPI is installed successfully.

## Feature Enabling Checklist
Include NvmeFeature.dsc

## Common Optimizations

## Build flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

> Step1. Prepare build environment with VS2019 as needed.
> Step2. Install the pip requirements.
         Sample : pip install -r Edk2/pip-requirements.txt
> Step3. Setup Edk2 environment.
         Sample : Edk2/edksetup.bat Rebuild
> Step4. Get the code dependencies.
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p NvmeFeaturePkg -t DEBUG,RELEASE -a IA32,X64 TOOL_CHAIN_TAG=VS2019
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p NvmeFeaturePkg -t DEBUG,RELEASE -a IA32,X64 TOOL_CHAIN_TAG=VS2019
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p NvmeFeaturePkg -t DEBUG,RELEASE -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose

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
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p NvmeFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py-p NvmeFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p NvmeFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
