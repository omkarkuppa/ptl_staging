# BtgAcmMisc Tool

## Revision History

| Revision Number | Description                                       | Revision Date |
| --------------- | ------------------------------------------------- | ------------- |
| 1.0             | Initial Release                                   | Mar. 27, 2024 |
| 1.1             | Support the different size of BtGACM inside BIOS. | Mar. 28, 2024 |
| 1.2             | Add module to support padding BtGACM binary.      | Mar. 28, 2024 |
| 1.3             | Introduce slot size and change size alignment.    | Sep. 30, 2024 |

## Introduction

- ```BtgAcmMisc``` tool provides a series of operation for supporting BIOS building.
- Check the slot size to meet the alignment requirement.
- Support to decode the different size of the BtGACM binary resident in BIOS.
  - Parse the FFS based on the platform assigned GUID value.
  - Decode the ACM header from the possible offset (Based on slot size).
- There have one major operations provided by this tool.
  1. Padding the BtGACM binary image into assigned slot size.
  2. Generate the parameter file for FIT type-2 entry to pass into ```FitGen``` tool.

## Modules

- ```Base```: Provide the generic definition/wrapper/function to share across different sub-modules within this tool.

  - ```_ByteBufferWrapper.py```: Wrapper object to provide the binary file operation.
  - ```_StructWrapper.py```: Wrapper object to provide the C-struct operation.

- ```BtgAcm```: Provide the definition and struct wrapper to decode the BtGACM binary.

  - ```_AcmHdrParser.py```:  Wrapper object to decode the ACM header from buffer.
  - ```_AcmInfoTblParser.py```: Wrapper object to decode the ACM information table from buffer.
  - ```_AcmProcessorIdListParser.py```: Wrapper object to decode the ACM processor ID list from buffer.

- ```Edk2```: Provide the definition and struct wrapper for EDK-II related.

  - ```_Edk2FfsParser.py```: Wrapper object to decode the EDK-II FFS header.

- ```_BtgAcmFfsParser.py```: API to provide the parser to decode BtGACM FFS.

  ```python
  from BtgAcmMisc._BtgAcmFfsParser import *

  #
  # Decode the BtGACM FFS buffer.
  # FFS GUID: { 26FDAA3D-B7ED-4714-8509-EECF1593800D }
  #
  FfsParser = BtgAcmFfsParser (Buffer = BiosBuffer)

  #
  # Get the FFS data buffer.
  #
  FfsDataBuffer = FfsParser.DataBuffer
  ```

- ```_BtgAcmBinParser.py```: API to provide the parser to decode each BtGACM binary from BtGACM FFS.

  ```python
  from BtgAcmMisc._BtgAcmBinParser import *

  #
  # Decode the BtGACM binaries inside the BtGACM FFS.
  #
  BinParser = BtgAcmBinParser (
                DataBuffer = FfsParser.DataBuffer,
                SlotSize   = SlotSize,
                )

  #
  # Get each of the BtGACM binary information.
  #
  BinInfoList = BinParser.BinInfoList
  ```

- ```_BtgAcmFitEntryGenerator.py```: API to provide the generator to export the parameter file for FitGen tool.

  ```python
  from BtgAcmMisc._BtgAcmFitEntryGenerator import *

  #
  # Generate the command list for FitGen tool.
  #
  FitEntryGen = BtgAcmFitEntryGenerator (
                  Address   = Address,
                  BinInfo   = BinInfo,
                  SlotSize  = SlotSize,
                  )

  #
  # Retrieve the FitGen command list.
  #
  CmdList = FitEntryGen.CmdList
  ```

- ```_BtgAcmPadding.py```: API to provide the service to padding the BtGAcm binary to assigned slot size.

  ```python
  from BtgAcmMisc._BtgAcmPadding import *

  #
  # Initial the padding object.
  #
  Padding = BtgAcmPadding (
              BtgAcmBasePath = BtgAcmBasePath,
              SlotSize       = SlotSize,
              )

  #
  # Execute the padding operation and export to assigned path.
  #
  Padding.Execute (OutputDir)
  ```

## Usage - FitEntry

### Standalone Usage

- Use the below command to call the standalone script.

  ```shell
  python BtgAcmMiscScript.py FitEntry -I <PATH> -S <SLOT_SIZE> -O <PATH>
  ```

  | Command                                 | Description                                                    | Mandatory |
  | --------------------------------------- | -------------------------------------------------------------- | --------- |
  | ```-i``` / ```-I``` / ```--input```     | Path to the BIOS image which contained the BtGACM FFS.         | V         |
  | ```-s``` / ```-S``` / ```--slotsize```  | Integer number to support the slot size of each BtGACM binary. | V         |
  | ```-o``` / ```-O``` / ```--output```    | Path to the file to export input params passed to FitGen tool. | V         |

### Import Usage

- Add the package in Python import path.

  ```python
  from BtgAcmMisc import Type2FitEntryGenerator

  ReturnCode = Type2FitEntryGenerator (
                 BiosFilePath = BIOS_IMG_PATH,
                 OutputPath   = OUTPUT_FILE_PATH,
                 SlotSize     = BTG_ACM_SLOT_SIZE,
                 )
  ```

## Usage - Padding

### Standalone Usage

- Use the below command to call the standalone script.

  ```shell
  python BtgAcmMiscScript.py Padding -I <PATH> -S <SLOT_SIZE> -O <PATH>
  ```

  | Command                                 | Description                                                     | Mandatory |
  | --------------------------------------- | --------------------------------------------------------------- | --------- |
  | ```-i``` / ```-I``` / ```--input```     | Path to the base path placed the BtGACM binary files.           | V         |
  | ```-s``` / ```-S``` / ```--slotsize```  | Integer number to support the slot size of each BtGACM binary.  | V         |
  | ```-o``` / ```-O``` / ```--output```    | Path to export the padded BtGACM binary files.                  | V         |

### Import Usage

- Add the package in Python import path.

  ```python
  from BtgAcmMisc import BtgAcmPaddingWrapper

  ReturnCode = BtgAcmPaddingWrapper (
                 BtgAcmBasePath = BaseBtgAcmBinDir,
                 SlotSize       = BtgAcmSlotSize,
                 OutputDir      = OutputDir,
                 )
  ```
