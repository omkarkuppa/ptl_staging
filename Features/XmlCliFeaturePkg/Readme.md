# Overview

* **Feature Name:** XmlCliFeaturePkg
* **PI Phase(s) Supported:** PEI, DXE, SMM, RT-DXE/RT-SMM
* **SMM Required:** Optional (*Preferred as of now*)

## Purpose

XmlCli is an automation feature which is primarily targets bulk volume testing to accelerate the process of validation and testing. Basically the access to NVRAM APIs through which below features can be supported:

* Access/Modify bios setup option(s)
* Get/Set NVRAM Variable (`$NVAR`)

This features can be used on any valid interface such as Windows, Linux, ESXi, ITP, LTB, EFI

> Note: This feature is **not recommended** for the product bios for **end-products**. As the aim is only to allow Intel or OEMs to ease their tests and validation.



# High-Level Theory of Operation

Feature consists of a driver `XmlCliCommonDxe.inf` which is responsible for installing and publishing XmlCli Protocols and APIs. This feature can be enabled/disabled using the bios setup option `XmlCliSupport` set to `0x1` (Enabled) or `0x0` (Disabled).
Default value for this will be set to `0x0` (Disabled) for Client BIOS and `0x1` (Enabled) only for Intel Internal Server BIOS. This default option is toggled with Pcd `PcdXmlCliSupport`.

This feature enabled BIOS Driver to provide API for retrieving or modifying UEFI Variable (NVRAM Variable)

## Modules

* XmlCliCommonDxe.inf
* XmlCliCommonPei.inf
* XmlCliCommonSmm.inf
### XmlCliCommonDxe
This library Finds the FFS consist of `BiosKnobsData` binary file stitched in the BIOS image with the FFS GUID `615E6021-603D-4124-B7EA-C48A3737BACD`.
Once Found, it reserved memory for GBT xml (varies by platform as dependent on number of setup options), `2.5 MB` Dram Shared Mailbox, Cli buffers.

This reservation of memory on platform is handled using below Pcd Values:
```ini
[PcdsFixedAtBuild]
  gXmlCliFeaturePkgTokenSpaceGuid.PcdKnobsEntriesSize
  gXmlCliFeaturePkgTokenSpaceGuid.PcdKnobValueMapSize
  gXmlCliFeaturePkgTokenSpaceGuid.PcdKnobPatchDataBufferSize
  gXmlCliFeaturePkgTokenSpaceGuid.PcdGbtXmlReserved
```
More about the reserved values and condition can be found under section `[PcdsFixedAtBuild]` in file `Include/XmlCliFeature.dsc`

Once done with reserving memory the module registers Cli Api.

Shared Mailbox reserved by XmlCli is defined as:
```c
SHARED_MAILBOX_TYPE DramMailBoxTable[] = {
  { LEG_MAILBOX_SIG,     LEG_MAILBOX_OFFSET,    LEG_MAILBOX_SIZE,   SHARED_MEMORY_FLAG_MEMORY_TYPE },
  { CLI_REQ_SIG,         CLI_REQ_BUFFER_OFFSET, CLI_BUFFER_SIZE,    SHARED_MEMORY_FLAG_MEMORY_TYPE},
  { CLI_RES_SIG,         CLI_RES_BUFFER_OFFSET, CLI_BUFFER_SIZE,    SHARED_MEMORY_FLAG_MEMORY_TYPE},
  { SHARED_MB_LAST_SIG,  SHARED_MB_LAST_SIG,    SHARED_MB_LAST_SIG, SHARED_MB_LAST_SIG}
};

SHARED_MAILBOX_TYPE DramMailBoxTableLite[] = {
  { LEG_MAILBOX_SIG,     LEG_MAILBOX_OFFSET,         LEG_MAILBOX_SIZE,     SHARED_MEMORY_FLAG_MEMORY_TYPE },
  { CLI_REQ_SIG,         CLI_REQ_BUFFER_OFFSET,      CLI_LITE_BUFFER_SIZE, SHARED_MEMORY_FLAG_MEMORY_TYPE},
  { CLI_RES_SIG,         CLI_LITE_RES_BUFFER_OFFSET, CLI_LITE_BUFFER_SIZE, SHARED_MEMORY_FLAG_MEMORY_TYPE},
  { SHARED_MB_LAST_SIG,  SHARED_MB_LAST_SIG,         SHARED_MB_LAST_SIG,   SHARED_MB_LAST_SIG}
};
```
Address of this shared Mailbox is stored at CMOS values 0xF0 and 0xF1.

APIs for operation in DXE phase are published using `DxeCliApiProto` which are:
```c
const CLI_COMMAND_ENTRY gCliCommandTable[] = {
  { READ_BIOS_KNOBS_OPCODE,     CliReadBiosKnobs,      FALSE},
  { APPEND_BIOS_KNOBS_OPCODE,   CliAppendBiosKnobs,    FALSE},
  { UPDATE_BIOS_KNOBS_OPCODE,   CliUpdateBiosKnobs,    FALSE},
  { RESTORE_MODIFY_KNOBS_OPCODE, CliRestoreModifyKnobs, FALSE},
  { LOAD_DEFAULT_KNOBS_OPCODE,  CliLoadDefaultKnobs,   FALSE},
  { GET_SET_VARIABLE_OPCODE,    CliGetSetVariable,    TRUE },  // This function is supported for XmlCli Lite as well.
};
```

This driver is also responsible for publishing setup page, To add the Setup Form in Setup Browser below steps shall be performed:

   1. Include below Section in any setup form in `.vfr`/`.hfr`, where you want to display XmlCli Setup Form:
        ```
        // XmlCli Setup Form
        goto
          formsetguid = {0x8fd6d4e5, 0x138c, 0x4b5c, {0xef, 0x95, 0xdd, 0x00, 0xd2, 0x68, 0x9b, 0x8c}},  // XMLCLI_FORM_SET_GUID
          formid      = 1,  // the first Form id in XmlCliSetupVfr.vfr
          question    = 0,
          prompt      = STRING_TOKEN(STR_XMLCLI_FORM),
          help        = STRING_TOKEN(STR_XMLCLI_FORM_HELP);
        ```
   2. Define `STR_XMLCLI_FORM` and `STR_XMLCLI_FORM_HELP` in `uni` file of platform setup.

### XmlCliCommonPei
For Client Platform, restores the mailbox address in CMOS so that the tool can read it after S3 resume.

### XmlCliCommonSmm
Allocates reserved memory, install protocols and Register Cli Apis in SMM phase. All the operation API is authenticated.
Also Authenticated XmlCli Enable mechanism provided via this module.

Logically this driver does the similar functionality as XmlCliCommonDxe except that APIs are handled with SMM mode. i.e.
APIs are published using `SmmCliApiProto` instead of `DxeCliApiProto`

Typically during OS Runtime APIs are handled by this Smm driver.

## Configuration

### Setup Option Values

| Setup Option | Possible Value | Interpretation |
| - | - | - |
| XmlCliSupport | `0`-Disabled or `1`-Enabled | Determine whether the XmlCliSupport should be available for the target platform or not. Once this feature is enabled, Reboot required to take the change in effect. |
| PublishSetupPgPtr | `0`-Disabled or **`1`-Enabled** | Determines whether or not to publish setup page pointer to the setup knob in the xml or not `Enabled by default`|
| EnableXmlCliLite | **`0`-Disabled** or `1`-Enabled | To optimize the operation speed and save the reserved memory minimal lite feature to be enabled `Disabled by default`. Once this feature is enabled, Reboot required to take the change in effect. After the reboot user can perform faster knob operations, however some details (i.e. prompt name, setup page pointer, option values/min-max values) would not be available compared to non lite operation. Only Nvar name, Knob Variable Name, knob offset, default value and current value would be present. |

For easier flow, It is also possible to control default values of above setup options during build time using below Pcds:
```ini
[FixedPcd]
  gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliSupport
  gXmlCliFeaturePkgTokenSpaceGuid.PcdPublishSetupPgPtr
  gXmlCliFeaturePkgTokenSpaceGuid.PcdEnableXmlCliLite
```

## Build Flows
- Adding -DFSP_ARCH=IA32 or -DFSP_ARCH=X64 to build cmd is required for XmlCliFeaturePkg standalone build.
  - **-DFSP_ARCH=IA32** - Build PEI components in 32bit
  - **-DFSP_ARCH=X64** - Build PEI components in 64bit
- During Post Build Process, we execute **ParseSetup.py** file to parse the setup knob files `*.i` files for vfr output files and generate the binary file for setup knobs data.
- This binary then created as FFS with guid `615E6021-603D-4124-B7EA-C48A3737BACD` and the FFS is stitched to the FV using FMMT tool.
    - FV guid for Client Platform `A881D567-6CB0-4EEE-8435-2E72D33E45B5`
    - FV guid for Server Platform `CDBB7B35-6833-4ED6-9AB2-57D2ACDDF6F0`.

> Incremental build is not supported.

## Feature Enabling Checklist

1. Add `Include/XmlCliFeature.dsc` entry, check if all required packages and libraries are available.
2. Add `PreMemory.fdf` (Only for Client BIOS) and `PostMemory.fdf` to flash map file.
3. Set value of `gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliFeatureEnable` to `TRUE`. (In production BIOS this PCD shall be `FALSE`.)
4. Set value of `gXmlCliFeaturePkgTokenSpaceGuid.PcdPlatformXmlCli` to value based on platform to set some default values for platform. It could take any of listed below:
    ```
    0x0 - Don't care about platform
    0x1 - Client Platform
    0x2 - Server Platform
    0x3 - IOTG Platform
    0x4 - Other Platform
    ```
5. (Optional) To parse BoardID, FabID, KSC version and LanPhyversion without entering into setup below code segment should be added appropriately during Initialization of Platform Strings:
    ```c    
            Status = gBS->LocateProtocol(&XmlCliCommonGuid, NULL, (VOID **)&TempProtocolPtr);
            if ( EFI_ERROR (Status) ) { // XmlCliCommon not published.
              DEBUG ((EFI_D_INFO, "XML_CLI: XmlCli Support Status: Disabled. \n"));
            } else {
              DEBUG ((EFI_D_INFO, "XML_CLI: XmlCli Support Status: Enabled. \n"));
              PlatformSetupCallback (NULL, NULL);
              CallbackGetLanPhyRevision (NULL, NULL);
            }
    ```
6. (Optional) After Ensuring all the setup initialization is completed relevant to Setup Options, below method should be used and invoked appropriately:
    ```c
    VOID
    GenGbtXml (
      VOID
      )
    {
      EFI_STATUS     Status;
      XML_CLI_PROTO  *XmlCliProto;
      EFI_GUID       XmlCliProtocolGuid = {
        0xe3e49b8d, 0x1987, 0x48d0, { 0x9a, 0x1, 0xed, 0xa1, 0x79, 0xca, 0xb, 0xd6 }
      };

      Status = gBS->LocateProtocol ( &XmlCliProtocolGuid, NULL, (VOID **)&XmlCliProto );
      if (!EFI_ERROR (Status)) {
        XmlCliProto->GenerateGbtXml((VOID*)XmlCliProto);
      }
    }
    ```

## Unit Test build flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

> Step1. Prepare build environment with VS2019 as needed.
> Step2. Install the pip requirements.
         Sample : `pip install -r Edk2/pip-requirements.txt`
> Step3. Setup Edk2 environment.
         Sample : `Edk2/edksetup.bat Rebuild`
> Step4. Get the code dependencies.
         Sample : `stuart_setup -c Intel/.pytool/CISettings.py -p XmlCliFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`
> Step5. Update other dependencies.
         Sample : `stuart_update -c Intel/.pytool/CISettings.py -p XmlCliFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`
> Step6. Build unit test host.
         Sample : `stuart_ci_build -c Intel/.pytool/CISettings.py -p XmlCliFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`

## Feature Build Example for Windows Platform

1. Clone Repositories
    ```commandline
    git clone https://github.com/tianocore/edk2.git
    git clone https://github.com/tianocore/edk2-non-osi.git
    git clone https://github.com/tianocore/edk2-platforms.git
    ```

2. Set packages path

    ```commandline
    set workspace=%cd%
    set EDK_TOOLS_PATH=%workspace%\edk2\BaseTools
    set packages_path=%workspace%\edk2;%workspace%\edk2-non-osi;%workspace%\edk2-platforms\Platform\Intel;%workspace%\edk2-platforms\Silicon\Intel;%workspace%\edk2-platforms\Platform\Qemu;%workspace%\edk2-platforms\Silicon\Qemu;%workspace%\edk2-platforms\Features\Intel;%workspace%\edk2-platforms\Features\Intel\Debugging;%workspace%\edk2-platforms\Features\Intel\Network;%workspace%\edk2-platforms\Features\Intel\OutOfBandManagement;%workspace%\edk2-platforms\Features\Intel\PowerManagement;%workspace%\edk2-platforms\Features\Intel\SystemInformation;%workspace%\edk2-platforms\Features\Intel\UserInterface
    ```

3. Update edk2 submodules

    ```commandline
    cd %workspace%\edk2
    git submodule update --init
    ```

4. Build BaseTools packages

    ```commandline
    edksetup.bat Rebuild
    ```

5. Build feature package

    ```commandline
    build -a IA32 -a X64 -t CLANGPDB -b NOOPT -p XmlCliFeaturePkg\XmlCliFeaturePkg.dsc
    ```
   or
    ```commandline
   build -a IA32 -a X64 -t VS2019 -b DEBUG -p XmlCliFeaturePkg\XmlCliFeaturePkg.dsc
    ```
