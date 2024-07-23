<!--
@file

  This file provides the details for DmarAcpiFeaturePkg.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
* **Feature Name:** DMAR ACPI Feature Package
* **PI Phase(s) Supported:** DXE
* **SMM Required?** NO

## Purpose
This package is responsible for creating and publishing DMAR ACPI table in DXE phase after PCI enumeration complete event. This implementation is compliant with the following document:
  - Intel(R) Virtualization Technology for Directed I/O (VT-D) Architecture
    Specification v4.0, Dated June 2022.<br>
    https://software.intel.com/content/dam/develop/external/us/en/documents/vt-directed-io-spec.pdf

Currently supported remapping structures:
* DRHD (DMA-remapping hardware unit definition)
* RMRR (Reserved Memory Region Reporting)
* ATSR (Root Port ATS Capability Reporting Structure)
* RHSA (Remapping Hardware Static Affinity)
* SATC (SoC Integrated Address Translation Cache)
* SIDP (SoC Integrated Device Property)

**NOTE:**
* No support for ANDD (ACPI Name-space Device Declaration) structure.
* No support for N-deep PCIe hierarchy.

## Protocols

This package requires **gVtdTopologyProtocolGuid** to be installed before PCI enumeration complete event (**gEfiPciEnumerationCompleteProtocolGuid**). OEM/Customer may also install **gOemDmarProtocolGuid**.

EFI Guid Name            | Guid Value
------------------------ | ----------
**gVtdTopologyProtocolGuid** | ```0x6D766693-9DDD-458D-B124-89C287662DFB```
**gOemDmarProtocolGuid** | ```0x2442FD82-1B8F-427C-BE4C378104EAC2FF```

### VT-d Topology Protocol

**gVtdTopologyProtocolGuid** **MUST** be installed by Silicon code before PCI enumeration complete event.

Function Name               | Parameters | Description
--------------------------- | ---------- | --------
```GetSystemInfo```          | **VTD_TOPOLOGY_PROTOCOL** *```*This```*</br>**SYSTEM_INFO** *```*SystemInfo```*</br> | Function returns information about a system.
```GetNumberOfVtdEngines```        | **VTD_TOPOLOGY_PROTOCOL** *```*This```*</br>**UINT32** *```*NumberOfVtdEngines```*</br> | Function returns a number of VT-d engines in the system.
```GetVtdEngineInfo```             | **VTD_TOPOLOGY_PROTOCOL** *```*This```*</br>**UINT32** *```VtdEngineId```* </br>**VTD_ENGINE_INFO** *```*Info```*</br> | Function returns information about an enabled specific VT-d engine.
```GetDevices```                | **VTD_TOPOLOGY_PROTOCOL** *```*This```*</br>**UINT32** *```VtdEngineId```* </br>**DEVICE_INFO** *```**DeviceList```*</br>**UINT32** *```*ListLength```*</br> | Function returns a list of devices which are under a scope of the specific VT-d engine.
```GetPciDevicesWithAtc``` | **VTD_TOPOLOGY_PROTOCOL** *```*This```*</br>**UINT32** *```VtdEngineId```* </br>**DEVICE_INFO** *```**DeviceList```*</br>**UINT32** *```*ListLength```*</br>   | Function returns a list of PCI SoC integrated devices having address translation cache (ATC) which are under a scope of the specific VT-d engine.
```GetDevicesWithProperties``` | **VTD_TOPOLOGY_PROTOCOL** *```*This```*</br>**UINT32** *```VtdEngineId```* </br>**DEVICE_INFO** *```**DeviceList```*</br>**UINT32** *```*ListLength```*</br>   | Function returns a list of SoC integrated devices that have special properties and are under a scope of the specific VT-d engine.
```GetNumberOfReservedMemoryRegions``` | **VTD_TOPOLOGY_PROTOCOL** *```*This```*</br>**UINT32** *```*NumberOfReservedMemoryRegions```* </br>   | Function returns the number of BIOS allocated reserved memory regions which may be DMA targets.
```GetReservedMemoryRegionInfo``` | **VTD_TOPOLOGY_PROTOCOL** *```*This```*</br>**UINT32** *```ReservedMemoryRegionId```*</br>**RESERVED_MEMORY_REGION_INFO** *```*Info```*</br>**DEVICE_INFO** *```**DeviceList```*</br>**UINT32** *```*ListLength```*</br>   | Function returns information about BIOS allocated reserved memory region along with a list of devices which may require access to this specified reserved memory region.
```GetRootPortsSupportingAts``` | **VTD_TOPOLOGY_PROTOCOL** *```*This```*</br>**UINT16** *```SegmentId```*</br>**UINT8** *```*Flags```*</br>**DEVICE_INFO** *```**DeviceList```*</br>**UINT32** *```*ListLength```*</br>   | Function returns a list of PCIe Root-Ports supporting Address Translation Services (ATS) transactions for a given segment.

### OEM DMAR Protocol

**gVtdTopologyProtocolGuid** may be installed by platform code before PCI enumeration complete event. It's needed only when OEM wants to have RMRR entries for its specific PCIe devices.

Function Name               | Parameters | Description
--------------------------- | ---------- | --------
```GetNumberOfReservedMemoryRegions``` | **OEM_DMAR_PROTOCOL** *```*This```*</br>**UINT32** *```*NumberOfReservedMemoryRegions```* </br>   | Function returns the number of BIOS allocated reserved memory regions (defined by OEMs) which may be DMA targets.
```GetReservedMemoryRegionInfo``` | **OEM_DMAR_PROTOCOL** *```*This```*</br>**UINT32** *```ReservedMemoryRegionId```*</br>**RESERVED_MEMORY_REGION_INFO** *```*Info```*</br>**DEVICE_INFO** *```**DeviceList```*</br>**UINT32** *```*ListLength```*</br>   | Function returns information about BIOS allocated reserved memory region (defined by OEMs) along with a list of devices which may require access to this specified reserved memory region.
## Configuration

Set ```gDmarAcpiFeaturePkgTokenSpaceGuid.PcdDmarFeatureEnable``` to ```TRUE``` to enable DMAR feature.</br>

### PcdsFixedAtBuild

Below PCDs are fixed at build, and theirs values must be known during build.

PCD Name | Default | Description |
-------- | ------- | -----------|
PcdDmarRevision | 1 | Revision of DMAR table |
PcdDmarRmrrSupport | FALSE | Support for RMRR structures |
PcdDmarSatcSupport | FALSE | Support for SATC structures |
PcdDmarSidpSupport | FALSE | Support for SIDP structures |
PcdDmarRhsaSupport | FALSE | Support for RHSA structures |
PcdDmarAtsrSupport | FALSE | Support for ATSR structures |

**NOTE**: Setting ```PcdDmarRmrrSupport```, ```PcdDmarSatcSupport```, ```PcdDmarSidpSupport```,```PcdDmarRhsaSupport```, ```PcdDmarAtsrSupport``` to ```FALSE``` will result in the generated binary not including the code supporting the specific structure.
### PcdsDynamic

Below PCDs may be changed in runtime.

PCD Name | Default | Description |
-------- | ------- | -----------|
PcdDmarFlags | 0x1 | This PCD reflects DMAR.Flags field.<BR>  **BIT0**: INTR_REMAP - If Clear, the platform does not support interrupt remapping. If Set, the platform supports interrupt remapping. <br> **BIT1**: X2APIC_OPT_OUT - For firmware compatibility reasons, platform firmware may Set this field to request system software to opt out of enabling Extended xAPIC (X2APIC) mode.<br> **BIT3**: DMA_CTRL_PLATFORM_OPT_IN_FLAG: Platform firmware is recommended to Set this field to report any platform initiated DMA is restricted to only reserved memory regions (reported in RMRR structures) when transferring control to system software such as on ExitBootServices().
PcdDmarRmrrEnable | TRUE | This PCD may be used for enabling/disabling RMRR structures in runtime. It only matters when ```PcdDmarRmrrSupport``` is set to ```TRUE```.
PcdDmarSatcEnable | TRUE | This PCD may be used for enabling/disabling SATC structures in runtime. It only matters when ```PcdDmarSatcSupport``` is set to ```TRUE```.
PcdDmarSidpEnable | TRUE | This PCD may be used for enabling/disabling SIDP structures in runtime. It only matters when ```PcdDmarSidpSupport``` is set to ```TRUE```.
PcdDmarRhsaEnable | FALSE | This PCD may be used for enabling/disabling RHSA structures in runtime. It only matters when ```PcdDmarRhsaSupport``` is set to ```TRUE```.
PcdDmarAtsrEnable | FALSE | This PCD may be used for enabling/disabling ATSR structures in runtime. It only matters when ```PcdDmarAtsrSupport``` is set to ```TRUE```.

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
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p DmarAcpiFeaturePkg -t NOOPT -a X64 TOOL_CHAIN_TAG=VS2019 --verbose
>     Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p DmarAcpiFeaturePkg -t NOOPT -a X64 TOOL_CHAIN_TAG=VS2019 --verbose
>     Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p DmarAcpiFeaturePkg -t NOOPT -a X64 TOOL_CHAIN_TAG=VS2019 -–verbose