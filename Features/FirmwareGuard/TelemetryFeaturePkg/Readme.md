<!--
@file

 This file provides the details for FirmwareGuard Telemetry feature.

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
* **Feature Name:** FirmwareGuard - Telemetry
* **PI Phase(s) Supported:** PEI, DXE
* **SMM Required?** NO

## Purpose
This feature package generates the Acpi table 'PHAT', which displayes Fvi(Firmware Version Information)
and the health status of key hareware components to user.

# High-Level Theory of Operation
* The Pei/Dxe drivers under <FirmwareVersion> are used to collect all firmware information.
* The Dxe drivers under <HealthDriver> focus on recording the health status of Csme/MotherBoard/Mrc/Nvme.
* Library TelemetryFviLib provides Aip protocol service for Dxe driver, or build Hob for Pei modules.

## Modules
  <FirmwareVersion>
 * PlatformFirmwareVersionDxe
 * SiFirmwareVersionDxe
  <HealthDriver>
 * CsmeHealthDxe
 * MotherBoardHealthDxe
 * MrcHealthDxe
 * NvmeHealthDxe
  <DxeTelemetryAcpi>
 * DxeTelemetryAcpi
 * DxeTelemetryHwErrRec
  <ResetReasonDxe>
 * ResetReasonDxe
  <Fdf Files>
 * TelemetryDxe.fdf includes 2 FviDxe drives and 4 Health Dxe drivers

## Library
* TelemetryFviLib

## Configuration
[PCD]
* Set PcdTelemetryFeatureEnable to turn on/off this feature
* Set PcdBootTimeOverPercentage to indicate the percentage for mother board driver health.
* Set PcdPlatformFviReadyProtocolGuid to indicate when Platform Fvidata in SMBIOS with INTEL_FVI_SMBIOS_TYPE is ready to be retrieved.
* Set PcdPlatformFviEcStr to indicate EC Fvistring reported in SMBIOS with INTEL_FVI_SMBIOS_TYPE.
* Set PcdPlatformFviLanPhyStr to indicate LAN PHY Fvistring reported in SMBIOS with INTEL_FVI_SMBIOS_TYPE.

## Control Flows
* N/A

## Build Flows
* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

> Step1. prepare build environment with VS2019 or CLANG11 as needed
> Step2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features/FirmwareGuard;%cd%/Edk2Platforms/Silicon/Intel;%cd%/Edk2Platforms/Platform/Intel
> Step3. Edk2\edksetup.bat Rebuild
> Step4. build -p TelemetryFeaturePkg/TelemetryFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b DEBUG
> Step5. build -p TelemetryFeaturePkg/TelemetryFeaturePkg.dsc -a IA32 -a X64 -t VS2019 -b RELEASE
> Step6. build -p TelemetryFeaturePkg/TelemetryFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b DEBUG
> Step7. build -p TelemetryFeaturePkg/TelemetryFeaturePkg.dsc -a IA32 -a X64 -t CLANGPDB -b RELEASE

## Performance Impact
* Minor impact

## Common Optimizations
* N/A

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
             Sample : stuart_setup -c Intel/.pytool/CISettings.py -p TelemetryFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
>     Step5. Update other dependencies.
             Sample : stuart_update -c Intel/.pytool/CISettings.py -p TelemetryFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
>     Step6. Build unit test host.
             Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p TelemetryFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose