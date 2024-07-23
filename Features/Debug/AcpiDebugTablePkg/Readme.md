# Overview
* **Feature Name:** Acpi Debug Table
* **PI Phase(s) Supported:** DXE
* **SMM Required?** NO

## Purpose
The purpose of this AFP is to develop an ACPI feature driver installation function. This driver aims to facilitate the installation and management of ACPI tables, specifically focusing on the debug port ACPI table. By installing the debug port ACPI table, this driver enhances system debugging capabilities, allowing developers to efficiently diagnose and troubleshoot system-level issues.

# High-Level Theory of Operation
This feature will install ACPI Debug Table when entry point InstallDebugAcpiTable is triggered.

## Firmware Volumes
* PostMemory

## Modules
* AcpiDebugTableDxe

### AcpiDebugTableDxe
* Create and install ACPI debug table.

## Library
* No library is created by this feature.

## Key Functions
* **BuildAcpiDebugTable**: Create, fill up, and install ACPI debug table.

## Configuration
* Set **gAcpiDebugTablePkgTokenSpaceGuid.PcdAcpiDebugTableEnable** to TRUE/FALSE to enable/disable this feature.

## Data Flows
* Driver AcpiDebugTableDxe will install ACPI debug table and it will be consumed by OS.

## Control Flows
N/A

## Build Flows
* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG11  |

1. Prepare build environment with VS2019 or CLANG11 as needed
2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features/Debug;%cd%/Edk2Platforms/Silicon/Intel;%cd%/Edk2Platforms/Platform/Intel
3. Edk2\edksetup.bat Rebuild
4. build -p AcpiDebugTablePkg/AcpiDebugTablePkg.dsc -a X64 -t VS2019 -b DEBUG
5. build -p AcpiDebugTablePkg/AcpiDebugTablePkg.dsc -a X64 -t VS2019 -b RELEASE
6. build -p AcpiDebugTablePkg/AcpiDebugTablePkg.dsc -a X64 -t CLANGPDB -b DEBUG
7. build -p AcpiDebugTablePkg/AcpiDebugTablePkg.dsc -a X64 -t CLANGPDB -b RELEASE

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
   `stuart_setup -c Intel/.pytool/CISettings.py -p AcpiDebugTablePkg -t DEBUG,RELEASE -a X64`
3. Update other dependencies.\
   `stuart_update -c Intel/.pytool/CISettings.py -p AcpiDebugTablePkg -t DEBUG,RELEASE -a X64`
4. Build unit test host.\
   `stuart_ci_build -c Intel/.pytool/CISettings.py -p AcpiDebugTablePkg -t DEBUG,RELEASE -a X64 TOOL_CHAIN_TAG=VS2019 --verbose`

## Test Point Results
* ACPI debug table has been installed correctly.

## Functional Exit Criteria
* Check if ACPI debug table is installed on OS or not.

## Feature Enabling Checklist
1. Include AcpiDebugTable.dsc and PostMemory.fdf
2. Enable PcdAcpiDebugTableEnable.

## Performance Impact
* Minimal impact on performance.

## Common Optimizations
N/A
