# **EDK II Minimized firmware for Intel(R) PantherLake platforms**

##Features
This package provides the minimal feature of the PantherLake BIOS.

## **Windows Build Instructions**

### Pre-requisites

* Microsoft Visual Studio.
  - Visual Studio 2019 recommended and is used in the examples below.
* ASL compiler: Available from http://www.acpica.org
  - Install into `C:\ASL` to match default `tools_def.txt` configuration;
  - or update `tools_def.txt` to match ASL compiler location.
* NASM assembler:  Available from: http://www.nasm.us/
  - Install into `C:\NASM` to match default `tools_def.txt` configuration;
  - or update `tools_def.txt` to match NASM assembler location.
* Intel Binaries.
  - Please download essential binaries from Intel Website, and place into `PantherLakeOpenBoardPkg\Binaries`; or
    Contact your Intel window.

### Build
1. Type `cd Intel\PantherLakeBoardPkg`
2. Type 'prep' for debug build, 'prep r' for release build
3. Type 'bld' to build the PantherLake BIOS image. Alternatively, the build output can be logged to file with 's' option.
        - Supply 's' argument to prep
        - Supply '/s' argument bld
         e.g. command stream would be
            prep r s && bld /s
4. BIOS is generated in 'Build\PantherLakeBoardPkg\DEBUG_VS2019x86\FV\CLIENTBIOS.fd'
5. Create the IFWI using the BIOS ROM from Step 4 and the latest IFWI from the VP Simics PPR

## Capsule Building

- Refer the [document](./Features/CapsuleUpdate/Docs/CapsuleBuild.md) located in ```Features/CapsuleUpdate/Docs/CapsuleBuild.md```
