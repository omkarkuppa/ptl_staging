# Overview

* Feature Name: MipiCamFeaturePkg
* PI Phase(s) Supported: DXE
* SMM Required: No

## Purpose

SetupVariable transmit Nvs, and provide ACPI table to met IPU driver requirement.

Currently supported use cases:
* Install Mipi Camera ACPI table for use by driver, Can configure participants for each supported platform independently(code-time exclusion using PCDs)

# High-Level Theory of Operation
MIPICAM_CONFIG_VARIABLE_NAME

Feature consists of a single driver(MipiCam.inf) which is responsible for installing the Mipi Camera tables under a separate MipiCam SSDT Acpi table.
Based on selected options in EFI variable MIPICAM\_CONFIG\_VARIABLE the Mipi Camera module is configured, after publication of Mipi Camera driver(MipiCam.inf).

The MipiCamDxe.inf entry point has to be executed before drivers start binding to the hardware. It is board responsibility to place
them in flash in a way that meets this requirement.

## Modules
* MipiCamDxe.inf - Transmit the setup variable and Hob data to Nvs, then install the ACPI table.

## Libraries
* MipiCamSensors.asl - Provide Controllogic, Sensor, and Flash's SSDT table for driver use.


## Key Functions
N/A

## Configuration
* Set gMipiCamFeaturePkgTokenSpaceGuid.PcdMipiCamFeatureEnable to TRUE to enable MIPI Camera UEFI Config feature.
* Set gMipiCamFeaturePkgTokenSpaceGuid.PcdIpuDriverPlatformId to provide platform Id for IPU driver identify.

| Type                                                | Variable name                             | Variable GUID                       |
|-----------------------------------------------------|-------------------------------------------|-------------------------------------|
| MIPICAM\_CONFIGURATION\_VARIABLE                    | mMipiCamSetupData                         | 295C6B57-75B9-4525-9F04EF9DB3505668 |


## Data Flows
Setup option  ==> NVS ========> ACPITable
_______________________________________________________________________
SaSetup.hfr   ===> MipiCamNvs.aht=> MipiCamSensor.asl

## Control Flows
N/A

## Build flows
* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

* Step1. prepare build environment with VS2019 or CLANG11 as needed
* Step2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Edk2Platforms/Platform/Intel;%cd%/Intel;%cd%/Intel/Features/Camera
* Step3. Edk2\edksetup.bat
* Step4. build -p MipiCamFeaturePkg/MipiCamFeaturePkg.dsc -t VS2019 -a IA32 -a X64 -b DEBUG
* Step5. build -p MipiCamFeaturePkg/MipiCamFeaturePkg.dsc -t VS2019 -a IA32 -a X64 -b RELEASE

## Test Point Results
Booted to BIOS menu and MIPI settings can be configured via the the knobs in MIPICamera setup page.
Check the GPIO and I2C correction by board and Camera HW connfiguration
Dumped ACPI tables and checked all MIPICamera ACPI tables are present in DSDT and SSDT as expected.

## Functional Exit Criteria
* Once the feature is enabled on the platform, MIPI Camera setup page shall present in SystemAgent if PcdMipiCamFeatureEnable is TRUE.
* Once the feature is enabled on the platform, MIPI Camera SSDT, methods and devices shall present in ACPI tables dumped from OS.

## Feature Enabling Checklist

1. Add MipiCamFeature.dsc entry and path to your build file and check if all required packages/libraries exists for this package.
2. Add MipiCamFeaturePostMemory.fdf entry and path to your flash map file.
3. Install MipiCamConfigurationVariable and select proper configuration in BoardPkg.
4. Set gMipiCamFeaturePkgTokenSpaceGuid.PcdMipiCamFeatureEnable to true.

## Unit Test build flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

* Step1. Prepare build environment with VS2019 as needed.
* Step2. Install the pip requirements.
         Sample : pip install -r Edk2/pip-requirements.txt
* Step3. Setup Edk2 environment.
         Sample : Edk2/edksetup.bat Rebuild
* Step4. Get the code dependencies.
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p MipiCamFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
* Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p MipiCamFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
* Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p MipiCamFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose