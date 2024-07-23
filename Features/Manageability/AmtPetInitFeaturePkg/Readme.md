# Overview

* Feature Name: AmtPetInitFeaturePkg
* PI Phase(s) Supported: DXE
* SMM Required: No

## Purpose

This package implements support for reporting ASF status code to AMT. 

# High-Level Theory of Operation

If Intel AMT is enabled in the system, BIOS shall provide an interface that a software module can call to report the ASF status code, [Amt Pet Init Force Push Hob](#Amt-Pet-Init-Force-Push-Hob), when BIOS PET Progress is enabled.

</br>
## Modules

* AmtPetInitDxe.inf

## Configuration
Set ```gAmtPetInitFeaturePkgTokenSpaceGuid.PcdAmtPetInitFeatureEnable``` to ```TRUE``` to enable AMT Pet Init feature.
</br>

## Data flows

Before determining which, AMT Pet init message shall be issued, BIOS shall compare the current feature settings with the one kept within [Amt Pet Init Configuration](#AMT-Pet-Init-Configuration)

### Amt Pet Init Configuration

Field Name       | Description
---------------- | -----------
```Revision```   | AMT MAC PassThrough Configuration Revision
```FwProgress``` | PET Events Progress to receive PET Events. 0: Disable; <b>1: Enable</b>

</br>
### Amt Pet Init Force Push Hob

Field                | Description
-------------------- | -----------
```EfiHobGuidType``` | GUID Hob type header.
```MessageType```    | ASF message type.

## Build flows

### Prerequisite
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

1. Install the pip requirements.\
   `pip install -r Edk2/pip-requirements.txt`

### Build the package
1. Setup Edk2 environment.\
   `python Edk2/BaseTools/Edk2ToolsBuild.py -t VS2019`
2. Get the code dependencies.\
   `stuart_setup -c Intel/.pytool/CISettings.py -p AmtPetInitFeaturePkg -t DEBUG,RELEASE -a IA32,X64`
3. Update other dependencies.\
   `stuart_update -c Intel/.pytool/CISettings.py -p AmtPetInitFeaturePkg -t DEBUG,RELEASE -a IA32,X64`
4. Build unit test host.\
   `stuart_ci_build -c Intel/.pytool/CISettings.py -p AmtPetInitFeaturePkg -t DEBUG,RELEASE -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose`

## Functional Exit Criteria

Once the feature is enabled on the platform, AMT Pet init shall be reporting ASF status code.

## Feature Enabling Checklist

1. Add AmtPetInitFeaturePkg.dsc entry and path to your build file and check if all required packages/libraries exists for this package.
2. Add PostMem.fdf entry and path to your flash map file.
