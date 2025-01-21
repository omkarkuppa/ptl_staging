<!--
@file

 This file provides the details for Dptf feature.

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

* Feature Name: DPTF Platform Driver. This Driver installs ACPI table containing support for DPTF OS driver.
* PI Phase(s) Supported: DXE
* SMM Required: No

## Purpose

This feature installs support for DPTF/DTT Thermal management solution on Intel platforms.

Currently supported use cases:
* Install DPTF ACPI table for use by ESIF & DPTF UI driver
* Can configure participants for each supported platform independently(code-time exclusion using PCDs)
* Platform Data module will provide platform/board specific data to the Dptf module
* Can also disable participants at run-time based on Dptf Configuration Variable


# High-Level Theory of Operation
DPTF_CONFIG_VARIABLE_NAME
Feature consists of a single driver(Dptf.inf) which is responsible for installing the DPTF tables under a separate DPTF SSDT Acpi table.
Based on selected options in EFI variable DPTF\_CONFIG\_VARIABLE the DPTF module is configured, after publication of DPTF driver(Dptf.inf).

The Dptf.inf entry point has to be executed before drivers start binding to the hardware. It is board responsibility to place
them in flash in a way that meets this requirement.

## Modules

* Dptf.inf
* DptfAcpiTables.inf

## Configuration

Set gDptfFeaturePkgTokenSpaceGuid.PcdDptfFeatureEnable to enable to enable the DPTF Feature.

The following PCDs can be used to include/exclude the DPTF Participants for a specific platform
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttFan1Participant to false to exclude the FAN1 participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttFan2Participant to false to exclude the FAN2 participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttFan3Participant to false to exclude the FAN3 participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttSen1Participant to false to exclude the Sensor1 participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttSen2Participant to false to exclude the Sensor2 participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttSen3Participant to false to exclude the Sensor3 participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttSen4Participant to false to exclude the Sensor4 participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttSen5Participant to false to exclude the Sensor5 participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttPchpParticipant to false to exclude the Pch participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttChargerParticipant to false to exclude the Charger participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttBatteryParticipant to false to exclude the Battery participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttPowerParticipant to false to exclude the Power participant.
Set gDptfFeaturePkgTokenSpaceGuid.PcdDttGpupParticipant to false to exclude the Gpu Sensor participant.

Required APIs by DPTF are under \_SB.DPTF container device. Platform expected to provide these APIs
ECAV -> Check if EC driver has been installed
SCHG -> Set Charge Rate in Amp
CMPP -> The battery discharge current capability for at least 1ms, in 1mA unit
VBNL -> The battery instantaneous no-load voltage that can be obtained from battery fuel gauge. The unit is 1mV.
RBHF -> The RBHF The high frequency impedance value that can be obtained from battery fuel gauge. The unit is 1mOhm.
PBSS -> Max sustained power for battery in milliwatts
CTYP -> The CTYP object provides charger type.  0x01 = Traditional 0x02 = Hybrid 0x03 = NVDC
BMAX -> Battery A Maximum value
FCHG -> Checks if Fast Charging is enabled
FNSL -> Set Fan Speed Level 0-100 by Fan Index
GFNS -> Read Current Fan Speed in RPM by Fan Index
B1FC -> Battery 1 Full Charge Capacity
PWRT -> Current power resource type PwrSrcType Bit[0] = 0 <DC>, Bit[0] = 1 <AC>, Bit[1] = 1 <USB-PD>, Bit[2] = 1
        <Wireless Charging> Bit[7:4]  = Power Delivery State Change Sequence Number
ARTG -> Rating of the adapter connected in mW
PROP -> Worst case platform power in Watts
PBOK -> Used by DTT to inform EC about power delivery state change. Read DTT BIOS Writer's guide for more info
SNRD -> Read a temperature value of EC Sensor by Sensor Index
STRP -> Set trip point for a specific EC sensor by Sensor Index

Other _Qxx methods might be required to notify OSPM of changes in above values for interrupt based mechanism.
Check BIOS Writer's Guide for more information about above APIs

Customers are expected to modify platform related settings in PlatformData module
under PlatSamplePkg\Features\PlatformData\Acpi\PlatformData.asl

PlatformData ACPI module will also need Platform to provide the following platform/board specific Dptf files.
Modify the following files accordingly

Art.asl  -> Active Relationship Table
Psvt.asl -> Passive Relationship Table
Trt.asl  -> Thermal Relationship Table
BiosDataVault.asl -> Bios Data Vault
Fps.asl  -> Fan Perfomance State Table
HID.h    -> HID values for DTT Participants

Board can pass the DPTF configuration to the feature using below EFI variable:

| Type                                                | Variable name                             | Variable GUID                       |
|-----------------------------------------------------|-------------------------------------------|-------------------------------------|
| DPTF\_CONFIGURATION\_VARIABLE                       | mDptfSetupData                            | AC5CF0C1-A682-478F-8AA8F285091B2A85 |
In our current case, PlatformSetup.hfr in each controls the DPTF configuration per platform.

## Build flows
EDK II Build Environment
No special tools are required to build this feature.

## Feature Enabling Checklist

1. Add DptfFeature.dsc entry and path to your build file and check if all required packages/libraries exists for this package.
2. Add PostMemory.fdf entry and path to your flash map file.
3. Install DptfConfigurationVariable and select proper configuration in BoardPkg.
4. Set gDptfFeaturePkgTokenSpaceGuid.PcdDptfFeatureEnable to true.
5. Provide the required APIs under \_SB.DPTF container device
6. Make platform specific changes under PlatformData ACPI module(Art,Trt, Psvt, HID) under PlatSamplePkg folder
7. Include Ipf.asl to add the IETM ACPI device

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
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p DptfFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p DptfFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p DptfFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose