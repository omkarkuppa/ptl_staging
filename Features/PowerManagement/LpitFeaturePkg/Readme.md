# Overview
* **Feature Name:** LpitFeaturePkg
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
ACPI 5.0 introduced the concept of a Low Power S0 Idle Capable platform through
setting FADT.Flags[21] bit. The setting of this bit infers the presence of one or more
Low Power Idle (LPI) states on such an LPI capable platform.
The inference of LPI state(s) is insufficient for a contemporary OS to monitor and
provide meaningful diagnostics of whether the LPI state was entered, for how long
and diagnose if the desired state was not achieved.
Intel is using a defined table to provide this information. The table has a reserved
signature ("LPIT") in the ACPI specification, and must be included in the set of ACPI
tables provided by Intel LPI capable platforms.

# High-Level Theory of Operation
This package update and install LPIT ACPI table to support LPIT feature.

## Firmware Volumes
* PostMemory

## Modules
* LpitDxe

### LpitDxe
* Create and install LPIT ACPI table.

## Library
* No library is created by this feature.

## Key Functions
* **FindLpitTable**: Find & Read LPIT ACPI table.
* **UpdateLpitTable**: Fill up LPIT ACPI table.
* **InstallTable**: Install LPIT ACPI table.

## Configuration
* **PcdLpitFeatureEnable**: Main feature PCD to support LPIT feature on system firmware, set TRUE/FALSE to enable/disable this feature.

## Data Flows
* Driver LpitDxe will read, update, and install LPIT ACPI table and it will be consumed by OS.

## Control Flows
N/A

## Build Flows
* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

1. Prepare build environment with VS2019 or CLANG11 as needed
2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features/PowerManagement
3. Edk2\edksetup.bat Rebuild
4. build -p LpitFeaturePkg/LpitFeaturePkg.dsc -a X64 -t VS2019 -b DEBUG
5. build -p LpitFeaturePkg/LpitFeaturePkg.dsc -a X64 -t VS2019 -b RELEASE
6. build -p LpitFeaturePkg/LpitFeaturePkg.dsc -a X64 -t CLANGPDB -b DEBUG
7. build -p LpitFeaturePkg/LpitFeaturePkg.dsc -a X64 -t CLANGPDB -b RELEASE

## Unit Test build flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

* Install the pip requirements.\
   `pip install -r Edk2/pip-requirements.txt`

### Build the package
1. Setup Edk2 environment.\
   `python Edk2/BaseTools/Edk2ToolsBuild.py -t VS2019`
2. Get the code dependencies.\
   `stuart_setup -c Intel/.pytool/CISettings.py -p LpitFeaturePkg -t DEBUG,RELEASE -a X64`
3. Update other dependencies.\
   `stuart_update -c Intel/.pytool/CISettings.py -p LpitFeaturePkg -t DEBUG,RELEASE -a X64`
4. Build unit test host.\
   `stuart_ci_build -c Intel/.pytool/CISettings.py -p LpitFeaturePkg -t DEBUG,RELEASE -a X64 TOOL_CHAIN_TAG=VS2019 --verbose`

## Test Point Results
* ACPI LPIT table has been installed correctly.

## Functional Exit Criteria
* Once the feature is enabled on the platform, LPIT ACPI table should present in ACPI tables which are dumped from OS.

## Feature Enabling Checklist
1. Include LpitFeature.dsc and PostMemory.fdf
2. Enable PcdLpitFeatureEnable.

## Performance Impact
* Minor impact

## Common Optimizations
N/A
