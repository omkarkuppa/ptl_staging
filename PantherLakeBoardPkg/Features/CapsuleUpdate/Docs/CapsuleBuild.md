<!--
  @file
  This file provide the details for Capsule Toolkit.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

  @par Specification Reference:

-->
# Capsule Toolkit

> This document is aligned with ```v1.0.16``` Capsule Toolkit.

## Revision History

| Revision Number | Description     | Revision Date |
| --------------- | --------------- | ------------- |
| 1.0             | Initial Release | Jun. 17, 2025 |

## Scope

- This document describes the methodology of client capsule file generation for supported standalone and monolithic capsule using manual method.
- This document will also provide the information on the types of capsules, payloads, version represents, build required command, and their syntax.
- Please note to make the firmware update successfully, you need to apply the correct capsule on SUT or it would be failed.

## Prerequisite

- **```Mandatory``` Windows Operation System**
  - Toolkit had been tested on Windows 11

- **```Mandatory``` Python**
  - Install the [Python](https://www.python.org/) 3.8+ on your computer
  - Please install the ```toml``` package via ``` py -3 -m pip install toml```
    - This dependency would be removed while the minimum Python version of toolkit is updated
  - Follow the [PEP-680](https://peps.python.org/pep-0680/) ```tomllib``` added as part of native package from Python 3.11

- **```Mandatory``` UEFI Capsule Signing Keys**
  - Generate the 3-layers self-signed X.509 capsule signing keys
  - Refer the [document](./../../../../Features/FirmwareGuard/CapsuleFeaturePkg/SigningKeys/Readme.md) to configure and create the required capsule signing key
  - Generated signing keys naming should be matched the platform config file located in ```Intel/*BoardPkg/Features/CapsuleUpdate/Config```
    - Under ```Keystore``` section with ```CapTrustPublicCert```, ```CapOtherPublicCert```, and ```CapSignerPrivateCert``` keys
  - Signing keys should be placed in ```Intel/Features/FirmwareGuard/CapsuleFeaturePkg/SigningKeys/InternalOnly/Common```
  - During Capsule build process, it will be copied into the ```Intel/Features/FirmwareGuard/CapsuleFeaturePkg/Tools/SigningKeys```

- **```Mandatory``` BIOS Guard Signing Keys**
  - Use the platform BIOS Guard signing key on proper enabling platform
  - If you do not have this key please generating following the BIOS Guard enabling guideline
  - This document is not intended to include the scope of it
  - Used signing keys naming should be matched the platform config file located in ```Intel/*BoardPkg/Features/CapsuleUpdate/Config```
    - Under ```BiosGuard.Certificate``` section with ```PbkFile``` and ```PvkFile``` keys
  - Signing keys should be placed in ```Binaries/*BinPkg/Tools/ToolScripts/BiosGuard/SignBiosGuard```
  - During Capsule build process, it will be copied into the ```Intel/Features/FirmwareGuard/CapsuleFeaturePkg/Tools/BiosGuard/SignBiosGuard```

- **```Optional``` Windows Update Package Signing Keys**
  - Follow the Microsoft guideline to create the test signing key for signing Windows update package
  - This document is not intended to include the scope of it
  - Used signing keys naming should be matched the platform config file located in ```Intel/*BoardPkg/Features/CapsuleUpdate/Config```
    - Under ```Keystore``` section with ```WinDriverPfxCert``` keys
  - Signing keys should be placed in ```Binaries/*BinPkg/CapsuleUpdate/InternalOnly/SigningKeys```
  - During Capsule build process, it will be copied into the ```Intel/Features/FirmwareGuard/CapsuleFeaturePkg/Tools/SigningKeys```

- **```Mandatory``` BaseTools**
  - EDK-II BaseTools should be built as executable programs on running OS and installed on ```Edk2/BaseTools```
  - During Capsule build process, it will be copied into the ```Intel/Features/FirmwareGuard/CapsuleFeaturePkg/Tools/Tools/BaseTools```

- **```Mandatory``` BIOS Guard**
  - BIOS Guard tool provided the executable programs to compiled the BGSL and generate the BGUP
  - To generate the signed BGUP it is depend on the OpenSSL
  - Tool should be placed in ```Binaries/*BinPkg/Tools/ToolScripts/BiosGuard/UpdatePackage```
  - During Capsule build process, it will be copied into the ```Intel/Features/FirmwareGuard/CapsuleFeaturePkg/Tools/Tools/BiosGuard/UpdatePackage```

- **```Mandatory``` OpenSSL**
  - OpenSSL should be installed on ```Binaries/Tools/OpenSSL```
  - During Capsule build process, it will be copied into the ```Intel/Features/FirmwareGuard/CapsuleFeaturePkg/Tools/Tools/OpenSSL```

- **```Mandatory``` Microsoft Windows Software Development Kits (SDK) and Windows Driver Kits (WDK)**

  > Lack of installing the SDK and WDK would cause the failure to generate the Windows update package.

  - Windows SDK and WDK would be used for building Windows drive update package
  - Toolkit would require two programs ```signtool.exe``` and ```Inf2cat.exe``` to support generate the test-signed Windows driver update package
  - Toolkit would extend the path into below,
    - ```C:\Program Files (x86)\Windows Kits\10\bin\x64```
    - ```C:\Program Files (x86)\Windows Kits\8.1\bin\x64```
    - ```C:\Program Files (x86)\Windows Kits\10\bin\*\x64```
    - ```C:\Program Files (x86)\Windows Kits\10\bin\x86```
    - ```C:\Program Files (x86)\Windows Kits\8.1\bin\x86```
    - ```C:\Program Files (x86)\Windows Kits\10\bin\*\x86```
    - ```C:\bin\*\x64```
    - ```C:\bin\*\x86```
  - Following the [instructions](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk) provided from Microsoft to do the installation

## Workspace

> The path of toolkit must not have any special character or any space.

- The generic code flow for generating the capsule file had been moved into ```CapsuleFeaturePkg```
- Platform configuration file would be placed in each of platform ```*BoardPkg```
- Any platform specific configuration file would be placed in ```Intel/*BoardPkg/Features/CapsuleUpdate/Config```
- Each ```toml``` platform configuration file mapping into one type of BIOS building

```
Intel/BoardPkg
├── Features
│   └── CapsuleUpdate
│       └── Config           // Directory to place the platform configuration file.
└── CapsuleBuild.py          // Entry point for the platform to start build capsule file.

Intel/Features/FirmwareGuard/CapsulePkg
├── CapsuleCommon            // Python package to provide the common functionalities.
├── CapsuleGenerate          // Python package to provide the FMP capsule generate functionalities.
├── EnvChecker               // Python package to check the workspace and environment for toolkit
├── Output                   // Directory to place the output capsule file and test-signed Windows update package.
├── Scripts                  // Standalone Python script (Only depend on CapsuleCommon package.)
├── SigningKeys              // Directory to place the keys for signing.
├── Template                 // Directory to place the template file.
├── Temporary                // Directory to place the temporary file while building the capsule file.
├── Tools                    // Directory to place the stakeholders toolkit.
│   ├── BaseTools
│   ├── BiosGuard
│   └── OpenSSL
├── EnvCheck.py              // Script to execute the toolkit build environment checking.
├── FmpCapsuleGen.py         // Script to generate the FMP capsule files. (Base script.)
└── FmpCapsuleGenWrapper.py  // Script to provide the common interface for platform to wrap.
```

## FMP Instances

| No   | FW Name       | FW GUID                                    | FW Type      | Color (R,G, B)                     | Flash Interface        |
| ---- | ------------- | ------------------------------------------ | ------------ | ---------------------------------- | ---------------------- |
| 1    | Monolithic    | ```7FEB1D5D-33F4-48D3-BD11-C4B36B6D0E57``` | System (0x1) | ```(255, 255, 000)``` **[Yellow]** | BiosGuard/SPI/FWUpdate |
| 2    | BIOS          | ```6C8E136F-D3E6-4131-AC32-4687CB4ABD27``` | System (0x1) | ```(000, 000, 255)``` **[Blue]**   | BiosGuard/SPI          |
| 3    | Extended BIOS | ```FD0AADC7-9696-4795-8F34-53069F759FB0``` | System (0x1) | ```(000, 000, 255)``` **[Blue]**   | BiosGuard/SPI          |
| 4    | CSME          | ```0EAB05C1-766A-4805-A039-3081DE0210C7``` | Device (0x2) | ```(127, 000, 127)``` **[Purple]** | FWUpdate               |
| 5    | EC            | ```3DD84775-EC79-4ECB-8404-74DE030C3F77``` | Device (0x2) | ```(000, 183, 000)``` **[Green]**  | SPI                    |
| 6    | ISH PDT       | ```76CA0AD8-4A14-4389-B7E5-FD88791762AD``` | Device (0x2) | ```(128, 128, 255)``` **[Violet]** | FWUpdate               |
| 7    | uCode         | ```69585D92-B50A-4AD7-B265-2EB1AE066574``` | Device (0x2) | ```(255, 000, 000)``` **[Red]**    | BiosGuard/SPI          |
| 8    | BtGAcm        | ```4E88068B-41B2-4E05-893C-DB0B43F7D348``` | Device (0x2) | ```(000, 128, 255)``` **[Azure]**  | BiosGuard/SPI          |
| 9    | FSP           | ```5E8F2C7B-6D4F-4A9E-BF6D-8A7E9B0F3C2A``` | Device (0x2) | ```(000, 000, 255)``` **[Blue]**   | BiosGuard/SPI          |
| 10   | IFWI          | ```F9FA5980-95F5-491A-9583-136D7F26D63E``` | Device (0x2) | ```(255, 255, 255)``` **[White]**  | BiosGuard/SPI          |
| 11   | Foxville      | ```6E36078B-6B6F-42BC-981E-4C639AFA4D6D``` | Device (0x2) | ```(000, 000, 255)``` **[Blue]**   | PCIe Register          |

## Standalone Capsule

> Once Monolithic capsule update successfully, list of standalone FMP instances would be hidden automatically.

### BIOS FMP Standalone Capsule

> This FMP module would not be built into BIOS when BIOS extended region is enabled.

- Build the capsule for BIOS capsule update, BIOS payload is mandatory
- This capsule would be supported on BIOS Guard disabled and enabled platform
- Payloads:
  | No   | Item                       | FFS GUID                                   | Payload location   |
  | ---- | -------------------------- | ------------------------------------------ | -------------------|
  | 1    | BIOS Flash Config INI File | ```F73E8213-0A23-4FF6-BFB3-3995E312AAF7``` | Automated Generate |
  | 2    | BIOS ROM Image File        | ```5C245293-53B5-4455-BA02-2279EA55BB68``` | User Input         |
  | 3    | BIOS BGUP Image File       | ```C2E2039C-CFD8-4706-BE37-8F4DBB526005``` | Automated Generate |
- FwVersion format:
  ```
  Source Version : MMMM.mm
  Capsule Version: MMMMmm
    - MMMM: BIOS major version
    - mm  : BIOS minor version

  Example:
    2413.01 -> 241310 (Capsule DEC FwVersion) -> 3AE9E (Capsule HEX FwVersion)
  ```
- Command syntaxes:
  - To build the BIOS capsule, there have 2 ways to pass the BIOS image into toolkit
    - Input by user via ```-bios``` flag
    - Decompose from IFWI image via ```-ifwi``` with ```-fit``` flag
  ```
  py -3 CapsuleBuild.py bioscap [res]
                                [-bios <BIOS_IMAGE>] [-fit <FIT_TOOL>] [-ifwi <IFWI_IMAGE>]
                                [-biossvn <BIOS_SVN_VALUE>] [-bioslsv <BIOS_LSV_VALUE>]

    - (Optional) res     : If the BIOS to be built is Resiliency BIOS, set this flag is mandatory
                           Default: false

    - (Optional) -bios   : Path to the BIOS ROM image
                           Format : PATH
                           Default: ''

    - (Optional) -ifwi   : Path to the IFWI image (Should be used with "-fit" flag.)
                           Format : PATH
                           Default: ''

    - (Optional) -fit    : Path to the FIT tool (Should be used with "-ifwi" flag.)
                           Format : PATH
                           Default: ''

    - (Optional) -biossvn: BIOS SVN value for BIOS Guard update package (e.g. 3132)
                           Should be BIOS major version [4 digits]
                           Only take effect when using BGUP to update the flash region
                           Format : MMMM
                           Default: Major version of input BIOS

    - (Optional) -bioslsv: BIOS FMP lowest supported version value (e.g. 235201)
                           Format : MMMMmm
                           Default: 0
  ```
- Samples:
  ```
  [1] Build the BIOS capsule for Resiliency BIOS
      -> py -3 CapsuleBuild.py bioscap res -bios <BIOS_IMAGE>

  [2] Build the BIOS capsule for Native BIOS with BIOSSVN 0x31320000
      -> py -3 CapsuleBuild.py bioscap -bios <BIOS_IMAGE> -biossvn 3132

  [3] Build the BIOS capsule for Native BIOS with LSV 0x396C1
      -> py -3 CapsuleBuild.py bioscap -ifwi <IFWI_IMAGE> -fit <FIT_TOOL> -bioslsv 235201
  ```

### Extended BIOS FMP Standalone Capsule

> This FMP module would not be built into BIOS when BIOS extended region is disabled.

- Build the capsule for Extended BIOS capsule update, Extended BIOS payload is mandatory
- This capsule would be supported on BIOS Guard disabled and enabled platform
- Payloads:
  | No   | Item                                | FFS GUID                                   | Payload location   |
  | ---- | ----------------------------------- | ------------------------------------------ | -------------------|
  | 1    | Extended BIOS Flash Config INI File | ```BFEABAF1-0306-494B-9126-28CE91FB4EF6``` | Automated Generate |
  | 2    | Extended BIOS ROM Image File        | ```5C245293-53B5-4455-BA02-2279EA55BB68``` | User Input         |
  | 3    | Extended BIOS BGUP Image File       | ```C2E2039C-CFD8-4706-BE37-8F4DBB526005``` | Automated Generate |
- FwVersion format:
  ```
  Source Version : MMMM.mm
  Capsule Version: MMMMmm
    - MMMM: BIOS major version
    - mm  : BIOS minor version

  Example:
    2413.01 -> 241310 (Capsule DEC FwVersion) -> 3AE9E (Capsule HEX FwVersion)
  ```
- Command syntaxes:
  - To build the extended BIOS capsule, there have 2 ways to pass the Extended BIOS image into toolkit
    - Input by user via ```-bios``` flag
    - Decompose from IFWI image via ```-ifwi``` with ```-fit``` flag
  ```
  py -3 CapsuleBuild.py extbioscap [res]
                                   [-bios <EXT_BIOS_IMAGE>] [-fit <FIT_TOOL>] [-ifwi <IFWI_IMAGE>]
                                   [-biossvn <EXT_BIOS_SVN_VALUE>] [-bioslsv <EXT_BIOS_LSV_VALUE>]

    - (Optional) res     : If the Extended BIOS to be built is Resiliency Extended BIOS, set this flag is mandatory
                           Default: false

    - (Optional) -bios   : Path to the Extended BIOS ROM image
                           Format : PATH
                           Default: ''

    - (Optional) -ifwi   : Path to the IFWI image (Should be used with "-fit" flag.)
                           Format : PATH
                           Default: ''

    - (Optional) -fit    : Path to the FIT tool (Should be used with "-ifwi" flag.)
                           Format : PATH
                           Default: ''

    - (Optional) -biossvn: Extended BIOS SVN value for BIOS Guard update package (e.g. 3132)
                           Should be Extended BIOS major version [4 digits]
                           Only take effect when using BGUP to update the flash region
                           Format : MMMM
                           Default: Major version of input BIOS

    - (Optional) -bioslsv: Extended BIOS FMP lowest supported version value (e.g. 235201)
                           Format : MMMMmm
                           Default: 0
  ```
- Samples:
  ```
  [1] Build the Extended BIOS capsule for Resiliency Extended BIOS
      -> py -3 CapsuleBuild.py extbioscap res -bios <EXT_BIOS_IMAGE>

  [2] Build the Extended BIOS capsule for Native Extended BIOS with BIOSSVN 0x31320000
      -> py -3 CapsuleBuild.py extbioscap -bios <EXT_BIOS_IMAGE> -biossvn 3132

  [3] Build the Extended BIOS capsule for Native Extended BIOS with LSV 0x396C1
      -> py -3 CapsuleBuild.py extbioscap -ifwi <IFWI_IMAGE> -fit <FIT_TOOL> -bioslsv 235201
  ```

### CSME FMP Standalone Capsule

> Please follow the ME kit documentation to generate the FWUpdate image file properly.

- Build the capsule for CSME capsule update, ME FWUpdate payload is mandatory
- ME FWUpdate image is different than full CSE region (code + data), it would include several required ingredients
- FwVersion format:
  ```
  Source Version : MM.mm.HH.BBBB
  Capsule Version: mmHHBBBB
    [1] MM  : CSME major version
    [2] mm  : CSME minor version
    [3] HH  : CSME hotfix version
    [4] BBBB: CSME build version

  Example:
    Due to the DWORD limitation on FwVer field (Major version would not be used)
    21.0.15.1578 -> 000F062A (Capsule HEX FwVersion)
  ```
- Command syntaxes:
  - To build the CSME capsule, there have 2 ways to pass the FWUpdate image into toolkit
      - Input by user via ```-me``` flag
      - Decompose from IFWI image via ```-ifwi``` with ```-fit``` flag, ```-cse``` is required if it is Corporate IFWI
  ```
  py -3 CapsuleBuild.py mecap -mever <ME_VER_VALUE>
                              [-me <FWU_IMAGE>]
                              [-fit <FIT_TOOL>] [-ifwi <IFWI_IMAGE>] [-cse <CSE_IMAGE>]
                              [-mesku <ME_SKU_VALUE>] [-melsv <ME_LSV_VALUE>]

    - (Mandatory) -mever : CSME FMP FWUpdate payload version (e.g. 21.0.15.1662)
                           Format : MM.mm.HH.BBBB
                           Default: ''

    - (Optional)  -me    : Path to the ME FWUpdate image
                           Format : PATH
                           Default: ''

    - (Optional)  -ifwi  : Path to the IFWI image (Should be used with "-fit" flag.)
                           Format : PATH
                           Default: ''

    - (Optional)  -fit   : Path to the FIT tool (Should be used with "-ifwi" flag.)
                           Format : PATH
                           Default: ''

    - (Optional)  -cse   : Path to the CSE image (Should be used when generating from Corporate IFWI.)
                           Format : PATH
                           Default: ''

    - (Optional)  -mesku : SKU type for input FWUpdate image
                           To support the CSME reselling model, this value should be identical
                           Please refer the table below to input the supported value
                           Default: 'Default'

    - (Optional)  -melsv : CSME FMP lowest supported version value (e.g. 21.0.15.1437)
                           Format : MM.mm.HH.BBBB
                           Default: 0.0.0.0
  ```
- Samples:
  ```
  [1] Build the ME capsule for 21.0.0.1662 version with default GUID from Corporate IFWI
      -> py -3 CapsuleBuild.py mecap -ifwi <IFWI_IMAGE> -fit <FIT_TOOL> -cse <CSE_IMAGE> -mever 21.0.15.1662 -mesku default

  [2] Build the ME capsule for version 21.0.0.1662 and LSV 21.0.0.1434 with default GUID
      -> py -3 CapsuleBuild.py mecap -me <FWU_IMAGE> -mever 21.0.15.1662 -mesku default -melsv 21.0.0.1434
  ```
- Different Image Type GUID for CSME Reselling Model
  > Please note all the valid type mapping GUID would be unique between different generation.<br/>
  > Build the capsule with wrong GUID would cause SUT not trigger the capsule update.

  | No   | Type          | FW GUID                                    | Description                                     |
  | ---- | ------------- | ------------------------------------------ | ----------------------------------------------- |
  | 1    | ```default``` | ```0EAB05C1-766A-4805-A039-3081DE0210C7``` | Build for the generic CSME capsule for default. |
  | 2    | ```Cons_21``` | ```9DED4D29-DAD7-4297-AC3A-924D4E7DA042``` | For Gen-21 Consumer CSME. (PTL Mobile SKU)      |
  | 3    | ```Corp_21``` | ```F77BD6C9-FA4F-493F-9370-F3A501AB8189``` | For Gen-21 Corporate CSME. (PTL Mobile SKU)     |

### EC FMP Standalone Capsule

- Build the capsule for EC capsule update, EC payload is mandatory
- FwVersion format:
  ```
  Source Version : MM.mm.PP.BB
  Capsule Version: MMmmPPBB
    [1] MM: EC major version
    [2] mm: EC minor version
    [3] PP: EC platform ID
    [4] BB: EC build version

  Example:
  2.2.0.1 -> 02020001 (Capsule HEX FwVersion)
  ```
- Command syntaxes:
  - To build the EC capsule, there have 2 ways to pass the EC image into toolkit
      - Input by user via ```-ec``` flag
      - Decompose from IFWI image via ```-ifwi``` with ```-fit``` flag
  ```
  py -3 CapsuleBuild.py eccap [-ec <EC_IMAGE>]
                              [-fit <FIT_TOOL>] [-ifwi <IFWI_IMAGE>]
                              [-eclsv <EC_LSV_VALUE>]

    - (Optional) -ec   : Path to the EC image
                         Format : PATH
                         Default: ''

    - (Optional) -ifwi : Path to the IFWI image (Should be used with "-fit" flag.)
                         Format : PATH
                         Default: ''

    - (Optional) -fit  : Path to the FIT tool (Should be used with "-ifwi" flag.)
                         Format : PATH
                         Default: ''

    - (Optional) -eclsv: EC FMP lowest supported version value (e.g. 2.2.0.1)
                         Format : MM.mm.PP.BB
                         Default: 0.0.0.0
  ```
- Samples:
  ```
  [1] Build the EC capsule
      -> py -3 CapsuleBuild.py eccap -ec <EC_IMAGE>

  [2] Build the EC capsule for LSV 2.2.0.1
      -> py -3 CapsuleBuild.py eccap -ec <EC_IMAGE> -eclsv 2.2.0.1
  ```

### ISH PDT FMP Standalone Capsule

> The update interface for the ISH PDT leveraged the ME FWUpdate library provided API.<br/>
> This FMP instance is supported for ISH PDT (Platform Description Table) only, it is **NOT** indicated for ISH firmware.

- Build the capsule for ISH PDT capsule update, ISH PDT payload is mandatory
- ISH PDT FMP version is using virtual versioning due to it would report as ```1``` always
- FwVersion format:
  ```
  ISH PDT FMP instance is using virtual PDT version
  User could input the version as needed
  Value should be UINT32

  Capsule Version: UINT32 value

  Example:
  20 (DEC) -> 14 (Capsule HEX FwVersion)
  ```
- Command syntaxes:
  - To build the ISH PDT capsule, there have 2 ways to pass the ISH PDT image into toolkit
      - Input by user via ```-pdt``` flag
      - Decompose from IFWI image via ```-ifwi``` with ```-fit``` flag
  ```
  py -3 CapsuleBuild.py pdtcap -pdtver <ISH_PDT_VER_VALUE>
                               [-pdt <ISH_PDT_IMAGE>]
                               [-fit <FIT_TOOL>] [-ifwi <IFWI_IMAGE>]
                               [-pdtlsv <ISH_PDT_LSV_VALUE>]

    - (Mandatory) -pdtver: ISH PDT FMP version value (e.g. 6)
                           Format : UINT32 value
                           Default: ''

    - (Optional)  -pdt   : Path to the ISH PDT image
                           Format : PATH
                           Default: ''

    - (Optional)  -ifwi  : Path to the IFWI image (Should be used with "-fit" flag.)
                           Format : PATH
                           Default: ''

    - (Optional)  -fit   : Path to the FIT tool (Should be used with "-ifwi" flag.)
                           Format : PATH
                           Default: ''

    - (Optional)  -pdtlsv: ISH PDT FMP lowest supported version value (e.g. 3)
                           Format : UINT32 value
                           Default: 0
  ```
- Samples:
  ```
  [1] Build the ISH PDT capsule for version 20
      -> py -3 CapsuleBuild.py pdtcap -pdt <ISH_PDT_IMAGE> -pdtver 20

  [2] Build the ISH PDT capsule for version 20 and LSV 6
      -> py -3 CapsuleBuild.py pdtcap -pdt <ISH_PDT_IMAGE> -pdtver 20 -pdtlsv 6
  ```

### BootGuard ACM FMP Standalone Capsule

> Slot size conception is used for handling the multiple BtGACM image deliver in one BIOS image.<br/>
> Current platform assigned the slot size as ```0x48000 (288K)```.

- Build the capsule for BtGACM capsule update, BtGACM payload is mandatory
- This capsule would be supported on BIOS Guard disabled and enabled platform
- With the build flow, toolkit would do the padding on the input BtGACM image into assigned slot size ```Firmware.Btgacm.SlotSize```
- Payloads:
  | No   | Item                         | FFS GUID                                   | Payload location   |
  | ---- | ---------------------------- | ------------------------------------------ | -------------------|
  | 1    | BtGACM Flash Config INI File | ```23C9D7A5-FEEB-4AF3-A38E-6E066F271EF3``` | Automated Generate |
  | 2    | BtGACM Image File            | ```1F725288-145F-4129-9F59-03F64FE1A10C``` | User Input         |
  | 3    | BtGACM BGUP Image File       | ```64B015D5-B088-44AB-9375-751FE94A351C``` | Automated Generate |
- FwVersion format:
  ```
  Source Version : MM.mm.RR
  Capsule Version: MMmmRR
  [1] MM  : BtGAcm major version
  [2] mm  : BtGAcm minor version
  [3] RR  : BtGAcm revision version

  Example:
  1.19.17 -> 011311 (Capsule HEX FwVersion)
  ```
- Command syntaxes:
  ```
  py -3 CapsuleBuild.py btgacmcap -btgacm <BTG_ACM_IMAGE>
                                  -build <DEBUG|RELEASE> -biossvn <BIOS_SVN_VALUE>
                                  [res]
                                  [-btgacmlsv <BTG_ACM_LSV_VALUE>]

    - (Mandatory) -btgacm   : Path to the BtGACM image
                              Format : PATH
                              Default: ''

    - (Mandatory) build     : Indicate the target BIOS build type
                              Value of the flag should be "DEBUG" or "RELEASE"
                              Default: ''

    - (Mandatory) -biossvn  : BIOS SVN value for BIOS Guard update package (e.g. 3132)
                              Should be BIOS major version [4 digits]
                              Only take effect when using BGUP to update the flash region
                              Format : MMMM
                              Default: Major version of input BIOS

    - (Optional)  res       : If the target is Resiliency BIOS, set this flag is mandatory
                              Default: false

    - (Optional)  -btgacmlsv: BtGACM FMP lowest supported version value (e.g. 235201)
                              Format : MM.mm.BB
                              Default: 0.0.0
  ```
- Samples:
  ```
  [1] Build the BtGAcm capsule for Resiliency debug BIOS with BIOSSVN 0x31320000
      -> py -3 CapsuleBuild.py btgacmcap res -btgacm <BTG_ACM_IMAGE> -build debug -biossvn 3132

  [2] Build the BtGAcm capsule for Native release BIOS with BIOSSVN 0x31320000
      -> py -3 CapsuleBuild.py btgacmcap -btgacm <BTG_ACM_IMAGE> -build release -biossvn 3132

  [3] Build the BtGAcm capsule for Native release BIOS and LSV 1.19.11 with BIOSSVN 0x31320000
      -> py -3 CapsuleBuild.py btgacmcap -btgacm <BTG_ACM_IMAGE> -build release -biossvn 3132 -btgacm 1.19.11
  ```

### Microcode FMP Standalone Capsule

> Slot size conception is used for handling the multiple microcode patches deliver in one BIOS image.<br/>
> There would have ```4K``` range for placing the microcode array version information.<br/>
> Current platform assigned the slot size as 0x37000 (220K).

- Build the capsule for microcode capsule update, microcode patch is mandatory
- With the build flow, toolkit would do the padding on the input microcode image into assigned slot size ```Firmware.Ucode.SlotSize```
- Microcode FMP version is using virtual versioning due to multiple microcode patches with different version in one BIOS
- Supported microcode capsule update mode:
  - ```ucodebgup```: Build microcode capsule with BGUP (Supported on BIOS Guard disabled and enabled platform)
    - Include full microcode FV image and correspond BGUP file
  - ```ucodefull```: Build microcode capsule without BGUP (Supported on BIOS Guard disabled platform)
    - Include full microcode FV image
  - ```ucodeslot```: Build microcode capsule with single slot (Supported on BIOS Guard disabled platform)
    - Include single microcode image and virtual version
    - Only allowed the microcode image version is **larger** than current platform
- Layout of the microcode capsule file is described as external data representation(XDR) with ```4``` sections below,
  - While building the microcode capsule with ```ucodeslot``` mode, mark the size of first 2 sections as ```0```
- Payloads:
  | Section Name        | Included                        | Description                               |
  | ------------------- | ------------------------------- | ----------------------------------------- |
  | uCode Full FV Image | ```ucodebgup``` ```ucodefull``` | Full microcode FV image                   |
  | uCode Full FV BGUP  | ```ucodefull```                 | BGUP to flash the full microcode FV image |
  | uCode Version       | ```ucodeslot```                 | Microcode version file                    |
  | uCode Array         | ```ucodeslot```                 | Microcode array file (Only one uCode)     |
- Microcode FV (Applicable on ```ucodebgup``` and ```ucodefull```):
  | No   | Item                   | FFS GUID                                   | Payload location   |
  | ---- | ---------------------- | ------------------------------------------ | ------------------ |
  | 1    | Microcode Version File | ```E0562501-B41B-4566-AC0F-7EA8EE817F20``` | Automated Generate |
  | 2    | Microcode Array File   | ```197DB236-F856-4924-90F8-CDF12FB875F3``` | Automated Generate |
- FwVersion format:
  ```
  Microcode array could deliver different microcode patches.
  Correct microcode patch would be loaded depend on the hardware of platform.
  User need to input their expected version manually.

  Value should be UINT32

  Capsule Version: UINT32 value

  Example:
  15 (DEC) -> F (Capsule HEX FwVersion)
  ```
- Command syntaxes:
  ```
  py -3 CapsuleBuild.py ucodecap -ucode <UCODE_IMAGE>[-ucode <UCODE_IMAGE> ...]
                                 -ucodemode <ucodebgup|ucodefull|ucodeslot>
                                 -build <DEBUG|RELEASE>
                                 [res]
                                 [-ucodever <UCODE_VER_VALUE>] [-ucodelsv <UCODE_LSV_VALUE>]
                                 [-ucodeverstr <UCODE_VER_STR>]
                                 [-biossvn <BIOS_SVN_VALUE>]

    - (Mandatory) -ucode      : Path to the microcode patch
                                Re-use this flag to input multiple microcode patch in order
                                Format : PATH
                                Default: ''

    - (Mandatory) -ucodemode  : Path to the microcode patch
                                Value of the flag should be "ucodebgup" or "ucodefull" or "ucodeslot"
                                Default: ''

    - (Mandatory) build       : Indicate the target BIOS build type
                                Value of the flag should be "DEBUG" or "RELEASE"
                                Default: ''

    - (Optional)  res         : If the target is Resiliency BIOS, set this flag is mandatory
                                It is mandatory for "BGUP" and "FULL" mode for Resiliency BIOS
                                Default: false

    - (Optional)  -ucodever   : Microcode FMP version value (e.g. 0xF)
                                Format : UINT32 value
                                Default: 1

    - (Optional)  -ucodelsv   : Microcode FMP lowest supported version value (e.g. 0xA)
                                Format : UINT32 value
                                Default: 1

    - (Optional)  -ucodeverstr: Microcode FMP version string
                                Format : ASC-II value
                                Default: Version 0.0.0.1

    - (Optional)  -biossvn    : BIOS SVN value for BIOS Guard update package (e.g. 3132)
                                Should be BIOS major version [4 digits]
                                Only take effect when using BGUP to update the flash region
                                It is mandatory for "BGUP" mode
                                Format : MMMM
                                Default: Major version of input BIOS
  ```
- Samples:
  ```
  [1] Build the BGUP mode uCode capsule for Resiliency debug BIOS and version 0xF with BIOSSVN 0x31320000
      -> py -3 CapsuleBuild.py ucodecap res -ucode <UCODE_IMAGE> -ucodemode ucodebgup -ucodever 0xF -build debug -biossvn 3132

  [2] Build the FULL mode uCode capsule for Native debug BIOS version 0xF with LSV 0xA
      -> py -3 CapsuleBuild.py ucodecap -ucode <UCODE_IMAGE_1> -ucode <UCODE_IMAGE_2> -ucodemode ucodefull -ucodever 0xF -ucodelsv 0xA -build debug

  [3] Build the SLOT mode uCode capsule for Native release BIOS version 0xF and LSV 0xA with version string "Version 0.0.0.F"
      -> py -3 CapsuleBuild.py ucodecap -ucode <UCODE_IMAGE> -ucodemode ucodeslot -ucodever 0xF -ucodelsv 0xA -ucodeverstr "Version 0.0.0.F" -build release
  ```

### FSP FMP Standalone Capsule

> Only signed-FSP with FBM is supported this FMP capsule update.

- Build the capsule for FSP capsule update, FSP payload and correspond FBM is mandatory
- This capsule would be supported on BIOS Guard disabled and enabled platform
- Payloads:
  | No   | Item                       | FFS GUID                                   | Payload location     |
  | ---- | -------------------------- | ------------------------------------------ | -------------------- |
  | 1    | FSP Flash Config INI File  | ```8F9E0C7B-9D2F-4C8A-9F0D-1E6F5B7B0D3C``` | Automated Generate   |
  | 2    | FSP Image File wit FBM     | ```3A6E0F9C-5B7E-4F8D-8E1C-9B0A0F6B8D2A``` | User Input (FSP+FBM) |
  | 3    | FSP BGUP Image File        | ```7D0E8C2F-1F6D-4F9B-AE0B-6A9C8B3A3E7C``` | Automated Generate   |
- FwVersion format:
  ```
  Source Version : WW.XX.YYYY.ZZZZ
  Capsule Version: YYYYZZZZZ
    [1] WW  : FSP major version
    [2] XX  : FSP minor version
    [3] YYYY: FSP revision version
    [4] ZZZZ: FSP build version

  Example:
    Due to the DWORD limitation on FwVer field (Major version and minor version would not be used)
    02.11.0025.2321 -> 00252321 (Capsule HEX FwVersion)
  ```
- Command syntaxes:
  ```
  py -3 CapsuleBuild.py fspcap -fsp <FSP_IMAGE> -fbm <FBM_IMAGE>
                               -biossvn <BIOS_SVN_VALUE>
                               [res]
                               [-fsplsv <FSP_LSV_VALUE>]

    - (Mandatory) -fsp    : Path to the FSP image
                            Format : PATH
                            Default: ''

    - (Mandatory) -fbm    : Path to the FBM image
                            Format : PATH
                            Default: ''

    - (Mandatory) -biossvn: BIOS SVN value for BIOS Guard update package (e.g. 3132)
                            Should be BIOS major version [4 digits]
                            Only take effect when using BGUP to update the flash region
                            Format : MMMM
                            Default: Major version of input BIOS

    - (Optional)  res     : If the BIOS to be built is Resiliency BIOS, set this flag is mandatory
                            Default: false

    - (Optional)  -fsplsv : FSP FMP lowest supported version value (e.g. 02.10.0024.2231)
                            Format : MM.mm.RRRR.BBBB
                            Default: 0.0.0.0
  ```
- Samples:
  ```
  [1] Build the FSP capsule for Resiliency BIOS with BIOSSVN 0x31320000
      -> py -3 CapsuleBuild.py fspcap res -fsp <FSP_IMAGE> -fbm <FBM_IMAGE> -biossvn 3132

  [2] Build the FSP capsule for Native BIOS with BIOSSVN 0x31320000
      -> py -3 CapsuleBuild.py fspcap -fsp <FSP_IMAGE> -fbm <FBM_IMAGE> -biossvn 3132

  [3] Build the FSP capsule for Native BIOS with BIOSSVN 0x31320000 and LSV 0x00242231
      -> py -3 CapsuleBuild.py fspcap -fsp <FSP_IMAGE> -fbm <FBM_IMAGE> -biossvn 3132 -fsplsv 02.10.0024.2231
  ```

### Foxville FMP Standalone Capsule

> Size of Foxville image could be ```1MB``` and ```2MB```, please ensure apply into matched size of image on device flash.

- Build the capsule for Foxville capsule update, Foxville payload is mandatory
- Ensure the supported Foxville add-in card inserted properly before doing the testing
- FwVersion format:
  ```
  Source Version : M.mmm
  Capsule Version: Mmmm
    [1] M  : Foxville major version
    [2] mmm: Foxville minor version

  Example:
    2.37 -> 2037 (Capsule HEX FwVersion)
  ```
- Command syntaxes:
  ```
  py -3 CapsuleBuild.py foxvillecap -foxville <FOXVILLE_IMAGE>
                                    [-foxvillelsv <FOXVILLE_LSV_VALUE>]

    - (Mandatory) -foxville   : Path to the Foxville image
                                Format : PATH
                                Default: ''

    - (Optional)  -foxvillelsv: Foxville FMP lowest supported version value (e.g. 2.32)
                                Format : M.mmm
                                Default: 0.0
  ```
- Samples:
  ```
  [1] Build the Foxville capsule
      -> py -3 CapsuleBuild.py foxvillecap -foxville <FOXVILLE_IMAGE>

  [2] Build the Foxville capsule with LSV 2.32
      -> py -3 CapsuleBuild.py foxvillecap -foxville <FOXVILLE_IMAGE> -foxvillelsv 2.032
  ```

## Monolithic Capsule

> Once Standalone capsule update successfully, Monolithic FMP instance would be hidden automatically.

- Build the capsule for Monolithic capsule update, BIOS payload is mandatory
- Monolithic capsule would update multiple components in one time and the supported components as below,
  - Mandatory component: ```BIOS```
  - Optional components: ```ME```, ```EC```, ```GbE NVM```, and ```ISH PDT```
- Dummy file would be included inside the section of component when correspond ingredient is not set (Correspond component update would be skipped)
- Payloads:
  | No   | Item                       | FFS GUID                                   | Payload location   |
  | ---- | -------------------------- | ------------------------------------------ | ------------------ |
  | 1    | BIOS Flash Config INI File | ```F73E8213-0A23-4FF6-BFB3-3995E312AAF7``` | Automated Generate |
  | 2    | BIOS ROM Image File        | ```5C245293-53B5-4455-BA02-2279EA55BB68``` | User input         |
  | 3    | BIOS BGUP Image File       | ```C2E2039C-CFD8-4706-BE37-8F4DBB526005``` | Automated Generate |
  | 4    | CSME FWUpdate Image File   | ```BA18C3BB-FD85-4D24-9730-DEB55D39562F``` | User input         |
  | 5    | EC Binary Image File       | ```E0139CE3-81BE-421C-B882-FDDEE1DE1C41``` | User input         |
  | 6    | ISH PDT Binary Image File  | ```CDD975BA-DE5D-4820-BE44-51B11B602324``` | User input         |
  | 7    | GbE NVM Binary Image File  | ```B7016578-FA9A-4758-8F67-8746D839E083``` | User input         |
- FwVersion format:
  - Monolithic capsule contain multiple payloads which have their own FW versions that cannot presented in ESRT one time
  - Version leveraged the BIOS version to stand for the FwVersion field (DWORD), please refer the BIOS FMP Standalone Capsule section above
- Command syntaxes:
  - To build the Monolithic capsule, there have 2 ways to pass the each ingredient into toolkit
    - Input by user via ```-bios```, ```-me```, ```-ec```, ```-pdt```, and ```-gbe```
    - Decompose from IFWI image via ```-ifwi``` with ```-fit``` flag, ```-cse``` is required if it is Corporate IFWI
    - If any ingredient is not expected to contain inside the capsule, use the keyword ```IGNORE```
  ```
  py -3 CapsuleBuild.py monocap [res]
                                [-bios <BIOS_IMAGE>] [-me <FWU_IMAGE>] [-ec <EC_IMAGE>] [-pdt <ISH_PDT_IMAGE>] [-gbe <GBE_NVM_IMAGE>]
                                [-fit <FIT_TOOL>] [-ifwi <IFWI_IMAGE>] [-cse <CSE_IMAGE>]
                                [-biossvn <BIOS_SVN_VALUE>] [-bioslsv <BIOS_LSV_VALUE>]

    - (Optional) res     : If the Extended BIOS to be built is Resiliency Extended BIOS, set this flag is mandatory
                           Default: false

    - (Optional) -bios   : Path to the BIOS ROM image
                           Format : PATH
                           Default: ''

    - (Optional) -me     : Path to the ME FWUpdate image
                           Input the "IGNORE``` the component not plan for delivering update image
                           Format : PATH
                           Default: ''

    - (Optional) -ec     : Path to the EC image
                           Input the "IGNORE``` the component not plan for delivering update image
                           Format : PATH
                           Default: ''

    - (Optional) -pdt    : Path to the ISH PDT image
                           Input the "IGNORE``` the component not plan for delivering update image
                           Format : PATH
                           Default: ''

    - (Optional) -gbe    : Path to the GbE NVM image
                           Input the "IGNORE``` the component not plan for delivering update image
                           Format : PATH
                           Default: ''

    - (Optional) -ifwi   : Path to the IFWI image (Should be used with "-fit" flag.)
                           Format : PATH
                           Default: ''

    - (Optional) -fit    : Path to the FIT tool (Should be used with "-ifwi" flag.)
                           Format : PATH
                           Default: ''

    - (Optional) -cse    : Path to the CSE image (Should be used when generating from Corporate IFWI.)
                           Format : PATH
                           Default: ''

    - (Optional) -biossvn: BIOS SVN value for BIOS Guard update package (e.g. 3132)
                           Should be BIOS major version [4 digits]
                           Only take effect when using BGUP to update the flash region
                           Format : MMMM
                           Default: Major version of input BIOS

    - (Optional) -bioslsv: BIOS FMP lowest supported version value (e.g. 235201)
                           Format : MMMMmm
                           Default: 0
  ```
- Samples:
  ```
  [1] Build the Monolithic capsule for Resiliency BIOS from Corporate IFWI
  py -3 CapsuleBuild.py monocap res -ifwi <IFWI_IMAGE> -fit <FIT_TOOL> -cse <CSE_IMAGE>

  [2] Build the Monolithic capsule for Native BIOS with BIOS ingredient only with BIOSSVN 0x31320000
  py -3 CapsuleBuild.py monocap -bios <BIOS_IMAGE> -ec IGNORE -me IGNORE -pdt IGNORE -gbe IGNORE -biossvn 3132

  [3] Build the Monolithic capsule for Native BIOS with LSV 0x396C1 from Consumer IFWI
  py -3 CapsuleBuild.py monocap -ifwi <IFWI_IMAGE> -fit <FIT_TOOL> -bioslsv 235201
  ```

## IFWI FMP Capsule

> This solution is purely for development only, should **NOT** be included in product.<br/>
> IFWI capsule would flash the whole SPI device without supporting seamless recovery update, please do not have any power interruption.<br/>
> **Note: ESRT table would not leave any record of the update attempt after a successfully upgrade.**

- Build the capsule for IFWI capsule update, IFWI with BIOS region is mandatory
- There have a assumption that size of IFWI file should be multiple of ```8MB```
  - To reduce the time of flash the whole IFWI image, the size of erase with BIOS Guard enabled is ```64K```
- Build the capsule for IFWI capsule update, this capsule would support on BiosGuard enable/disable environment
- Payloads:
  | No   | Item                       | FFS GUID                                   | Payload location   |
  | ---- | -------------------------- | ------------------------------------------ | -------------------|
  | 1    | IFWI Flash Config INI File | ```B2C92F59-9AA9-4C0A-9850-35FE2F84C276``` | Automated Generate |
  | 2    | IFWI Binary Image File     | ```60247F48-5DAC-4140-ABCD-8C54A7FD49B0``` | User Input         |
  | 3    | IFWI BGUP Image File       | ```D4892E64-8710-4541-82E5-50092A4DC9D9``` | Automated Generate |
- FwVersion format:
  - Version leveraged the BIOS version to stand for the FwVersion field (DWORD), please refer the BIOS FMP Standalone Capsule section above
- Command syntaxes:
  ```
  py -3 CapsuleBuild.py ifwicap -ifwi <IFWI_IMAGE>
                                [-biossvn <BIOS_SVN_VALUE>]

    - (Optional) -ifwi   : Path to the IFWI image
                           Format : PATH
                           Default: ''

    - (Optional) -biossvn: BIOS SVN value for BIOS Guard update package (e.g. 3132)
                           Should be BIOS major version [4 digits]
                           Only take effect when using BGUP to update the flash region
                           Format : MMMM
                           Default: Major version of input BIOS
  ```
- Samples:
  ```
  [1] Build the IFWI capsule
    -> py -3 CapsuleBuild.py ifwicap -ifwi <IFWI_IMAGE>

  [2] Build the IFWI capsule with BIOSSVN 0x31320000
    -> py -3 CapsuleBuild.py ifwicap -ifwi <IFWI_IMAGE> -biossvn 3132
  ```

## Capsule Output

- Once capsule build command had been executed successfully, you could get the output directory under ```Intel/Features/FirmwareGuard/CapsulePkg/Tools/Output```
- If Windows SDK and WDK had been installed properly and signing keys is placed correctly, test-signed Windows update package could be found in ```Intel/Features/FirmwareGuard/CapsulePkg/Tools/Output/Windows```
- Output files
  - ```*.cap```: FMP capsule file. (Please refer the UEFI specification for detail structure and information.)
  - ```*.inf```: Setup information file. (Provide the information for this Windows driver update package.)
  - ```*.cat```: Catalog file to record the hash values of ```*.cap``` and ```*.inf``` files. (Need to be signed.)

## Capsule Testing

### 1. Apply Capsule from UEFI Shell

> It is mandatory to disable secure boot if your EFI application is not signed with valid certificate.

- Pre-requisites
  - Build the ```CapsuleApp.efi``` application from EDK-II
    - Build the sample capsule update application ```Edk2/MdeModulePkg/Application/CapsuleApp```
    - ```Optional``` Disable the secure boot in BIOS setup option
- Install the capsule file from UEFI shell
  - Put the ```CapsuleApp.efi``` and target capsule file into ```FAT32``` formatted USB drive
  - Boot the platform to UEFI shell and switch the file system to the USB drive (Input ```fs0:``` if it is mounted in ```fs0:```)
  - Apply the capsule file via the command ```fs0:> CapsuleApp.efi <CAPSULE_FILE>```
  - System should reboot automatically with warm reset
  - Progress bar with Intel logo should be observed while entering capsule update flow (Only internal graphic connected could observe.)
  - Wait for the firmware update complete and boot back to UEFI shell
- Verify the capsule update result
  - Check the ESRT table via the command ```CapsuleApp.efi -E```
  - Dump the FMP protocol via the command ```CapsuleApp.efi -P```

### 2. Apply Windows Update Package from Windows OS

> Check the [signing type of Windows](https://techcommunity.microsoft.com/blog/hardwaredevcenter/introducing-preproduction-signing-level/3483336) installed to see if the secure boot disable and set test signing enable is supported and required or not.

- Pre-requisites
  - Check the system date and time is correct
  - ```Optional``` Disable the secure boot in BIOS setup option
  - ```Optional``` Set the ```TESTSIGNING``` via ```bcdedit``` command
    - Open administrator command prompt
    - ```bcdedit /set testsigning on```
    - Reboot the SUT
    - Confirm the changes via ```bcdedit``` command
    - You should observe the ```testsigning                       Yes``` in output message
- Install the Windows update package
  - Copy the whole Windows driver update package into the SUT (Should have ```*.cap```, ```*.inf```, and ```*.cat``` files.)
  - ```Optional``` Install the test certificate into the SUT (It is not required if the certificate is valid naturally.)
    - Double click on the security catalog file ```*.cat``` inside the package
    - Click on **"View Signature"**
    - Click on **"View Certificate"** then click on **"Install Certificate..."**
    - Choose **"Local Machine"** as store location. Click **"Next"** button
    - Browse the store location manually, select the **"Trusted Root Certification Authorities"**
    - Click **"OK"** button and **"Next"** button to install the test certificate on the system for one time
    - If import succeeds, you should observe the pop-out message **"The Import was Successful"**
  - Open administrator command prompt and use command ```pnputil -i -a <INF_FILE>``` to apply the update package
  - Reboot the SUT to make the Windows boot loader aware that a firmware update is required and hand off the capsule file to UEFI FW
  - During the FW upgrade, you should observe the additional message **"Please wait while we install a system update"**
  - SUT would reset automatically, then start entering the flash update mode
  - Progress bar, Intel logo, and **"Please wait while we install a system update"** should be observed while entering the capsule update flow (Only internal graphics connection can display this.)
  - Wait for the firmware update complete and boot back to Windows OS
- Verify the capsule update result
  - There are 3 locations to check the result of the upgrade,
    - Open the **"Device Manager"** go to **"Firmware"** section to find the corresponding firmware device
    - Open the **Registry Editor** and check the ESRT table located in ```HKEY_LOCAL_MACHINE\HARDWARE\UEFI\ESRT```
    - Open the **Registry Editor** and check the ```HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FirmwareResources```

## Remarks

- Certificates used for capsule file and Windows update package CAT file with sample test signing should **NOT** be used for product.
- ESRT entry hidden rules
  - Only happens when list of monolithic/standalone capsules got applied successfully
  - The standalone firmware update include ```BIOS```, ```ME```, ```EC```, and ```ISH PDT```
  - Any other device firmware will not be impacted
- Apply update on Resiliency BIOS
  - ESP partition is mandatory, firmware update would be aborted if UEFI FW cannot find valid one
  - Need to check that the free capacity is larger than ```64MB```
  - This rule affects the ```Monolithic```, ```BIOS```, ```ME```, ```BtGACM```, ```FSP``` and ```uCode```
- To support the BIOS seamless recovery, need to check that the flash map layout is configured properly
  - Top Swap (Boot Block) Size should be set as ```4MB``` in IFWI soft-strap
  - Ensure the BIOS image IBB size is set correctly
  - Native debug BIOS is not supported due to the IBB region size limitation

## Appendix A - BIOS Guard Update Package (BGUP)

> Please refer to the BIOS Guard enablement guideline to enable it on platform properly.<br/>
> Update interface of BGUP is only applicable when BIOS Guard is enabled on platform.

- In BIOS Guard enabled platform, BIOS region on SPI is protected by BIOS Guard
  - It is not allowed to program BIOS region via SPI protocol directly
- BIOS security version number (SVN) is built in BIOS image to ensure the input BGUP is acceptable to be used
  - BIOS SVN is reported from BIOS located in ```Intel/*PlatSamplePkg/Library/PeiPolicyUpdateLib/PeiCpuPolicyUpdatePreMem.c```
  - If the applied BGUP has an equal or higher SVN number, the capsule update should succeed
  - If the applied BGUP has a lower SVN number, the capsule update should be rejected by BIOS Guard
