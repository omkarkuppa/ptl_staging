# Overview
* **Feature Name:** Alert Standard Format (ASF) Table
* **PI Phase(s) Supported:** DXE
* **SMM Required?** NO

## Purpose
A managed client’s ASF configuration and capabilities are reported by the system firmware (or BIOS) via ACPI description tables and control methods and, optionally, as static information stored within an SEEPROM.
OS-present software uses this information to customize the system’s ASF-aware alert-sending device(s).

# High-Level Theory of Operation
This feature will install Alert Standard Format (ASF) ACPI Table when ReadyToBoot event is triggered.

## Firmware Volumes
* PostMemory

## Modules
* AsfTableDxe

### AsfTableDxe
* Create and install ASF ACPI table.

## Library
* No library is created by this feature.

## Key Functions
* **BuildAsfTable**: Create, fill up, and install ACPI table.

## Configuration
* Set **gAsfTableFeaturePkgTokenSpaceGuid.PcdAsfTableFeatureEnable** to TRUE/FALSE to enable/disable this feature.

## Data Flows
* Driver AsfTableDxe will install ASF ACPI table and it will be consumed by OS.

## Control Flows
N/A

## Build Flows
* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

1. Prepare build environment with VS2019 or CLANG11 as needed
2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features/Manageability;%cd%/Edk2Platforms/Silicon/Intel;%cd%/Edk2Platforms/Platform/Intel
3. Edk2\edksetup.bat Rebuild
4. build -p AsfTableFeaturePkg/AsfTableFeaturePkg.dsc -a X64 -t VS2019 -b DEBUG
5. build -p AsfTableFeaturePkg/AsfTableFeaturePkg.dsc -a X64 -t VS2019 -b RELEASE
6. build -p AsfTableFeaturePkg/AsfTableFeaturePkg.dsc -a X64 -t CLANGPDB -b DEBUG
7. build -p AsfTableFeaturePkg/AsfTableFeaturePkg.dsc -a X64 -t CLANGPDB -b RELEASE

## Unit Test build flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

1. Install the pip requirements.\
   `pip install -r Edk2/pip-requirements.txt`

### Build the package
1. Setup Edk2 environment.\
   `python Edk2/BaseTools/Edk2ToolsBuild.py -t VS2019`
2. Get the code dependencies.\
   `stuart_setup -c Intel/.pytool/CISettings.py -p AsfTableFeaturePkg -t DEBUG,RELEASE -a X64`
3. Update other dependencies.\
   `stuart_update -c Intel/.pytool/CISettings.py -p AsfTableFeaturePkg -t DEBUG,RELEASE -a X64`
4. Build unit test host.\
   `stuart_ci_build -c Intel/.pytool/CISettings.py -p AsfTableFeaturePkg -t DEBUG,RELEASE -a X64 TOOL_CHAIN_TAG=VS2019 --verbose`

## Test Point Results
* ASF ACPI table has been installed correctly.

## Functional Exit Criteria
* Check if ASF ACPI table is installed on OS or not.

## Feature Enabling Checklist
1. Include AsfTableFeature.dsc and PostMemory.fdf
2. Enable PcdAsfTableFeatureEnable.

## Performance Impact
* Minimal impact on performance.

## Common Optimizations
N/A
