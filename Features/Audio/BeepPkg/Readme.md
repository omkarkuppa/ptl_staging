# Overview

* Feature Name: BeepPkg - Package that allows to beep on a desired audio codec by configuring the beep/tone generator located on a codec.
* PI Phase(s) Supported: PEI, DXE
* SMM Required: No

## Purpose

This feature allows to beep on the desired audio codec by using one of the implemented interfaces:
* SoundWire Beep - This interface allows to initialize built-in SoundWire controllers on Intel platforms and enumerate/control connected codecs for each SoundWire controller over the SoundWire interface. For more information about the SoundWire interface, see [SoundWire specification](https://www.mipi.org/specifications/soundwire).
The SoundWire (Sndw) Manager IP is a soft controller IP with configurable parameters that can be tailored to support audio streaming applications over the SoundWire interface on Intel platforms.
* HD-Audio Beep - This interface allows to enumerate, intialize and beep on codecs aligned with the HD-Audio specification, see [High Definition Audio Specification](https://www.intel.com/content/www/us/en/standards/high-definition-audio-specification.html).

### SoundWire Interface
Currently supported use cases:

* Driver strength selection
* Multiple codecs support, up to 11 codecs on each SoundWire link (single Manager interface handles up to 11 codecs according to SoundWire specification)
* Installing SndwAccess PPI (PEI) and/or protocol (DXE) which allows communication with all connected codecs
* Installing Beep PPI (PEI) and/or protocol (DXE) which allows generate a short tones on supported codecs

Currently supported codecs:

* Realtek ALC1308

### HD-Audio Interface

Currently supported use cases:

* Driver strength selection
* Standard enumeration of HD-A codecs on enabled SDI lines
* Installing Beep PPI (PEI) and/or protocol (DXE) which allows controlling sine generator located on a HD-A codec to generate a beep sound

Currently supported codecs:

* Realtek ALC256

## High-Level Theory of Operation

Feature package contains:

* PPIs/Protocols
  * **SndwAccess**
    * This PPI/protocol contains all functions needed to operate on attached SoundWire codecs.
    Enable/disable functions allow to enable and disable access to SoundWire codecs. The user before any operation on codecs should use the **Enable** function. In other cases access will not be ready. After the user is done using codecs, the **Disable** function should be called. In other cases, the SoundWire OS driver may work incorrectly.
  * **Beep**
    * This PPI/protocol contains functions that allow to beep on desired, pre-configured at build-time codec. The same PPI/Protocol is produced by both interfaces, so a use case of the package assumes using only one interface at the moment.

* Drivers
  * **SndwInitPei** (*SndwInitPei.inf*) and **SndwInitDxe**  (*SndwInitDxe.inf*)
    * This driver is responsible for installing the **SndwAccess** PPI/protocol on *SndwInitPei*/*SndwInitDxe* entry point. *SndwAccess* PPI/protocol allows to prepare SoundWire controllers for codecs enumeration process. During enumeration the driver checks and initializes all of the connected codecs, reads all identification information from them and creates a codec list. The driver entry points have to be executed before drivers start binding to the hardware. It is the board's responsibility to place them in flash in a way that meets this requirement.

  * **SndwBeepPei** (*SndwBeepPei.inf*) and **SndwBeepDxe**  (*SndwBeepDxe.inf*)
    * This driver is responsible for installing the **Beep** PPI/protocol. It implements all of the *Beep* PPI/protocol functions for the pre-configured SoundWire Codec. This driver requires *SndwAccess* PPI/protocol to work.

  * **HdaBeepPei** (*HdaBeepPei.inf*) and **HdaBeepDxe**  (*HdaBeepDxe.inf*)
    * This driver is responsible for installing the **Beep** PPI/protocol. It implements all of the *Beep* PPI/protocol functions for the pre-configured HD-A Codec.

  * **BeepExamplePei** (*BeepExamplePei.inf*) and **BeepExampleDxe**  (*BeepExampleDxe.inf*)
    * This driver shows an example of using **Beep** PPI/protocol in the PEI/DXE boot phase. It can be used independently of an interface that is installed on *Beep* PPI/protocol.

### SoundWire Access PPI/Protocol

EFI Guid Name           | Guid Value
----------------------- | ----------
gSndwAccessProtocolGuid | ```0x0DA23D05-425B-4A21-83EE-D3546E2CD4C3```
gSndwAccessPpiGuid      | ```0x9D42AD70-2D57-425A-8318-E3509009538F```

Function Name | Parameters | Comments
------------- | ---------- | --------
```Send```          | **CONST SNDW_ACCESS** *```*This```*</br>**SNDW_CODEC_INFO** *```SndwCodecInfo```*</br>**SNDW_COMMAND** *```*TxCommand```*</br>**UINTN** *```TxSize```* | Function operating on Sndw Fifo buffer for sending messages to codecs.
```SendWithAck```   | **CONST SNDW_ACCESS** *```*This```*</br>**SNDW_CODEC_INFO** *```SndwCodecInfo```*</br>**SNDW_COMMAND** *```TxCommand```*</br>**SNDW_COMMAND** *```RxCommand```* (OPTIONAL) | Function sends one message through the Sndw interface and waits for the corresponding ACK message.
```Receive```       | **CONST SNDW_ACCESS** *```*This```*</br>**SNDW_CODEC_INFO** *```SndwCodecInfo```*</br>**SNDW_COMMAND** *```**RxCommand```*</br>**UINTN** *```RxSize```* | Function operating on Sndw Fifo for receiving messages from codecs.
```GetFirstCodec``` | **CONST SNDW_ACCESS** *```*This```*</br>**SNDW_CODEC_INFO** *```**SndwCodecInfo```* | Function return first codec saved in codecs list. If it does not exist, function sets *CodecInfo to ```NULL```.
```GetNextCodec```  | **CONST SNDW_ACCESS** *```*This```*</br>**SNDW_CODEC_INFO** *```*SndwCodecInfo```*</br>**SNDW_CODEC_INFO** *```**NextCodecInfo```* | Function retrieves the next codec saved in codecs list that follows the given one. If it does not exist, function sets *NextCodecInfo to ```NULL```.
```Enable```        | **CONST SNDW_ACCESS** *```*This```* | Function enables Soundwire interface and enumerates attached codecs.
```Disable```       | **CONST SNDW_ACCESS** *```*This```* | Function disables Soundwire interface.

### SoundWire CodecInfo structure (SNDW_CODEC_INFO)

Field Name    | Comments
------------- | --------
```SndwLinkIndex``` | Number of SoundWire link from 1 to max SoundWire link number (Defined by silicon). Check board design.
```CodecId```       | Structure specifying the address of SoundWire codec. This address should be unique on link, where codec is connected.

### SoundWire CodecId structure

Register Address | Field Name     | Comments
---------------- | -------------- | --------
```0x50```             | Version        | Indicates compliance with a specific SoundWire specification version.</br>b```0011``` => SoundWire v1.2</br>b```0010``` => SoundWire v1.1</br>b```0001``` => SoundWire v1.0
```0x51```, ```0x52```       | ManufacturerID | MIPI standard manufacturer code, see [http://mid.mipi.org](http://mid.mipi.org/).
```0x53```, ```0x54```       | PartID         | Fields implemented and defined by manufacturer.
```0x55```             | Class          | Field reserved for MIPI-defined class encoding.

### SoundWire MIPI Tx/Rx Commands structure

Bits  | Field Name    | Read                  | Write                | Comments
----- | ------------- | --------------------- | -------------------- | --------
```31```    | SspTag        | SSPOINT_TAG           | SSPOINT_TAG          | This bit is used to inform hardware that this command should be delayed until the next SSP occurs.
```30:28``` | Opcode        | SndwCmdRead           | SndwCmdWrite         | Type of the command.
```27:24``` | DeviceAddress | Device Address (0-11) | DeviceAddress (0-11) | Do not edit this field. It is overriden by driver based on enumeration process.
```23:8```  | RegAddress    | Register Address      | Register Address     | It is an address of the register in codec memory. Check codec specification.
```7:0```   | RegData       | Reserved              | Register Data        | New value of register

### SoundWire MIPI Rx Responses structure

Bits  | Field Name | Read        | Write         | Comments
----- | ---------- | ----------- | ------------- | --------
```31:16``` | Reserved2  | Reserved    | Reserved      | Reserved.
```15:8```  | Data       | RegData     | Response Data | Response from Read Tx Command.
```7```     | Reserved1  | Reserved    | Reserved      | Reserved.
```6:4```   | Opcode     | SndwCmdRead | SndwCmdWrite  | Type of command.
```3:2```   | Reserved0  | Reserved    | Reserved      | Reserved.
```1```     | Nak        | NAK         | NAK           | If this value is equal to b```1```, command is aborted.
```0```     | Ack        | ACK         | ACK           | Informs whether command has been accepted. If this value is equal to b```1``` then command has been accepted.

### Opcode

Value in Opcode field | Command Name
--------------------- | ------------
b```000```            | Ping
b```010```            | Read
b```011```            | Write

### Beep PPI/Protocol

EFI Guid Name           | Guid Value
----------------------- | ----------
gBeepProtocolGuid | ```0x5D2A7CCF-33BE-444D-BAA6-D6B597F5668B```
gBeepPpiGuid      | ```0x7F6C03D8-AD3B-406E-BF36-0467A84009E5```

Function Name | Parameters | Comments
------------- | ---------- | --------
```BeepOff``` | **CONST BEEP** *```*This```* | Function send command which disables sine tone generator.
```BeepOn```  | **CONST BEEP** *```*This```*</br>**UINTN** *```Frequency```*</br>**INTN** *```Amplitude```* | Function sends command which enables sine tone generator.
```Beep```    | **CONST BEEP** *```*This```*</br>**UINTN** *```Frequency```*</br>**INTN** *```Amplitude```*</br>**UINTN** *```NumOfBeeps```*</br>**UINTN** *```BeepDuration```*</br>**UINTN** *```PauseDuration```* | Function send command which enables sine tone generator. It allows to customize parameters like duration, number of beeps and pause between beeps.
```Enable```  | **CONST BEEP** *```*This```* | Function calls **Enable** function from SoundWire Access protocol.
```Disable``` | **CONST BEEP** *```*This```* | Function calls **Disable** function from SoundWire Access protocol.

## Modules

### PEI phase:
* SndwInitPei.inf
* SndwBeepPei.inf
* BeepExamplePei.inf
* HdaBeepPei.inf

### DXE phase:
* SndwInitDxe.inf
* SndwBeepDxe.inf
* BeepExampleDxe.inf
* HdaBeepDxe.inf

## Configuration

The behavior of the package is controlled by following PCDs:
* ```gBeepModuleTokenSpaceGuid.PcdPeiBeepEnable``` - set to ```TRUE``` enables all drivers for PEI phase. Located and could be changed in ```BoardPkgPcdInit.dsc``` for active project.
* ```gBeepModuleTokenSpaceGuid.PcdDxeBeepEnable``` - set to ```TRUE``` enables all drivers for DXE phase. Located and could be changed in ```BoardPkgPcdInit.dsc``` for active project.
* ```gBeepModuleTokenSpaceGuid.PcdSndwInterfaceEnable``` - set to ```TRUE``` enables drivers needed for Beep on Soundwire interface. Located and could be changed in ```Beep.dsc```.
* ```gBeepModuleTokenSpaceGuid.PcdHdaInterfaceEnable``` - set to ```TRUE``` enables drivers needed for Beep on HD-A interface. Located and could be changed in ```Beep.dsc```.

### SoundWire Interface
In case of using the Beep on a audio codec from the supported list, only the PCD needs to be configured:
* ```PcdSndwBeepCodecId``` - contains information about codec IDs, used to match supported configuration to the enumerated codec</br>
  Setting it to {0x20, 0x02, 0x5D, 0x13, 0x08, 0x00} means: </br>
  * 0x20 - version number,
  * 0x025D - manufacturer ID,
  * 0x1308 - part ID,
  * 0x00 - class.

If you wish to add a new SoundWire codec to the supported list, you need to:
1. Create a source file with function definitions, similarly as for Alc1308, refer to ```Features/Audio/BeepPkg/SndwBeep/SndwBeepRltkAlc1308.c``` file.
1. Expose these function declarations in ```Features/Audio/BeepPkg/SndwBeep/SndwBeepCodecs.h```.
1. Add ```PartId``` of your codec to the ```SNDW_BEEP_CODEC_PART_ID``` structure in ```Features/Audio/BeepPkg/SndwBeep/SndwBeepPrivateData.h```.
1. Set your newly functions as callbacks in the ```BeepApi``` in either ```SndwBeepDxe.c``` or ```SndwBeepPei.c```. </br>
Example:
```
switch (SwapBytes16 (*(UINT16 *) SndwBeepContext->SndwCodecInfo.CodecId.Encoding.PartId)) {
  case RltkAlc1308PartId:
    SndwBeepContext->BeepApi.BeepOn  = SndwBeepOnAlc1308;
    SndwBeepContext->BeepApi.BeepOff = SndwBeepOffAlc1308;
    SndwBeepContext->BeepApi.Beep    = SndwBeepAlc1308;
    break;
}
```

Note:
The required sndw commands must be requested from the codec vendor.

### HDA Interface
By default, after enabling ```gBeepModuleTokenSpaceGuid.PcdHdaInterfaceEnable``` the interface for Alc256 will be used for Beep.</br>
If you wish to add a new HDA codec to the supported list, you need to refer to ```Features/Audio/BeepPkg/HdaBeep/HdaBeepVerbTables.c``` file and create a similar structure for the supported operations as for Alc256:
```
HDA_BEEP_VERB_TABLES HdaBeepVerbTablesAlc256 = HDA_BEEP_VERB_TABLES_INIT (
  &Alc256InitVerbs,
  &Alc256BeepOnVerbs,
  &Alc256BeepOffVerbs
);
```
After that modify the line:
```  HdaBeepContext->HdaCodecVerbTables               = &HdaBeepVerbTablesAlc256;``` </br>
In either ```Features/Audio/BeepPkg/HdaBeep/HdaBeepPei.c``` or ```Features/Audio/BeepPkg/HdaBeep/HdaBeepDxe.c``` with the name of the created structure.

Note:
The required verb tables must be requested from the codec vendor.

### Settings example

#### Sndw Beep in DXE
gBeepModuleTokenSpaceGuid.PcdDxeBeepEnable|TRUE</br>
gBeepModuleTokenSpaceGuid.PcdSndwInterfaceEnable|TRUE</br>
gBeepModuleTokenSpaceGuid.PcdSndwBeepCodecId|{```0x20```, ```0x02```, ```0x5D```, ```0x13```, ```0x08```, ```0x00```}</br>

#### HD-A Beep in PEI
gBeepModuleTokenSpaceGuid.PcdPeiBeepEnable|TRUE</br>
gBeepModuleTokenSpaceGuid.PcdHdaInterfaceEnable|TRUE</br>

### GPIO configuration
GPIO pin programming of audio links is not supported by this package. </br>
Before starting any enumeration, initialization or beeping on a codec, related GPIO pins must be properly programmed. </br>
This step is required in case of using the Beep in the pre-mem phase, in any other case it's optional as long as the package is used after the code in the post-mem phase of ACE IP when GPIO programming is done based on the selected setup options.

GPIO pins that needs to be programmed in case of using HDA Beep interface:
* HDA_BCLK - bit clock
* HDA_RSTB - reset signal
* HDA_SYNC - frame sync
* HDA_SDO - serial data out
* HDA_SDI0 - serial data in #0, optional, depends on where the codec intended to beep is connected
* HDA_SDI1 - serial data in #1, optional, depends on where the codec intended to beep is connected

GPIO pins that needs to be programmed in case of using SoundWire Beep interface:
* SNDWx_CLK - clock for a sndw link, where 'x' denotes sndw link number.
* SNDWx_DATA - data for a sndw link, where 'x' denotes sndw link number.

Note:
The GPIO pin programming may differ depending on the used platform, so information about the required native functionality of the above pins is not included.

## Data flows

### SoundWire Access PPI/Protocol

The SndwAccess Protocol can be used in all DXE phases after protocol installation. For the PEI phase, the PPI is installed very early in the prememory init environment  (before MRC).
Before calling any function for communication with codecs, the user should use the ```Enable``` function.
After that user can call any function provided by the SndwAccess Protocol. After all operations, the user should use the ```Disable``` function.
This step is important for further SoundWire components, like OS drivers.

### Example of using installed PPI and/or protocol:

```C
Status = PeiServicesLocatePpi (&gSndwAccessPpiGuid, 0, NULL, (VOID **) &SndwAccessApi);
if (EFI_ERROR (Status)) {
  DEBUG ((DEBUG_ERROR, "%a(): Failed to locate PPI - %r\n", __FUNCTION__, Status));
  return EFI_NOT_FOUND;
}
```


```C
Status = gBS->LocateProtocol (
                &gSndwAccessProtocolGuid,
                NULL,
                (VOID **) SndwAccessApi
                );
if (EFI_ERROR (Status)) {
  DEBUG ((DEBUG_ERROR, "%a(): Failed to locate protocol - %r\n", __FUNCTION__, Status));
  return;
}
```
After that, it is possible to use the interface like this:
```C
Status = SndwAccess->Enable (SndwAccess);
...
Status = SndwAccess->Disable (SndwAccess);
```

After the ```SndwAccess->Enable``` call, the user can send and receive MCP messages from codecs.
Function ```Send``` is able to send multiple messages. Parameters for the ```Send``` function are a pointer to the SndwAccess PPI/Protocol, codec information (SNDW_CODEC_INFO), pointer to array of MCP messages (SNDW_COMMAND) and the number of messages.
Refer to SoundWire [CodecId structure](#soundWire-CodecId-structure), SoundWire MIPI [Tx](#soundWire-MIPI-Tx/Rx-Commands-structure)/[Rx](#soundWire-MIPI-Rx-Responses-structure) Commands structure and [Opcode](#opcode).

Example:

```C
SNDW_CODEC_INFO         CodecInfo;
SNDW_COMMAND            Command[1];

CodecInfo.CodecId.Encoding.Version = 0x20;
CodecInfo.CodecId.Encoding.ManufacturerID[0] = 0x02;
CodecInfo.CodecId.Encoding.ManufacturerID[1] = 0x5D;
CodecInfo.CodecId.Encoding.PartId[0] = 0x13;
CodecInfo.CodecId.Encoding.PartId[1] = 0x08;
CodecInfo.CodecId.Encoding.Class = 0x00;

Command[0].TxWrite.OpCode  = SndwCmdWrite;  // write data to codec
Command[0].TxWrite.RegAddr = 0x42;          // address of register in codec memory
Command[0].TxWrite.RegData = 0x21;          // new register value

Status = SndwAccess->Send (SndwAccess, CodecInfo, &Command, 1);
```

All messages sent to codecs have responses. Response contains data from codec if OpCode of command was SndwRead, Ack and Nak. Refer to [SoundWire MIPI Rx Responses structure](#soundWire-MIPI-Rx-Responses-structure). Function for getting responses is ```Receive```. Parameters for ```Receive``` function are pointer to *SndwAccess* protocol, codec information structure (*SNDW_CODEC_INFO*), pointer to received MCP messages (*SNDW_COMMAND*) and number of received messages. ```Receive``` function allocates memory and user should free up this memory with ```FreePool```;

Example:

```C
SNDW_CODEC_INFO         CodecInfo;
SNDW_COMMAND            *CommandResponse;
UINTN                   ResponseSize;

CodecInfo.CodecId.Encoding.Version = 0x20;
CodecInfo.CodecId.Encoding.ManufacturerID[0] = 0x02;
CodecInfo.CodecId.Encoding.ManufacturerID[1] = 0x5D;
CodecInfo.CodecId.Encoding.PartId[0] = 0x13;
CodecInfo.CodecId.Encoding.PartId[1] = 0x08;
CodecInfo.CodecId.Encoding.Class = 0x00;

Status = SndwAccess->Receive (SndwAccess, CodecInfo, &CommandResponse, &ResponseSize);
if (!EFI_ERROR (Status) && CommandResponse[0].Rx.Ack == 1) {
  // command success
}
FreePool (CommandResponse);
```

SndwAccess PPI/Protocol allows to send and receive one MCP message at a time by ```SendWithAck``` function. Parameters are a pointer to *SndwAccess* PPI/Protocol, codec information structure (*SNDW_CODEC_INFO*), MCP messages to send (*SNDW_COMMAND*) and a pointer to received MCP message.

Example:

```C
SNDW_CODEC_INFO         CodecInfo;
SNDW_COMMAND            Command;
SNDW_COMMAND            *CommandRespons;

CodecInfo.CodecId.Encoding.Version = 0x20;
CodecInfo.CodecId.Encoding.ManufacturerID[0] = 0x02;
CodecInfo.CodecId.Encoding.ManufacturerID[1] = 0x5D;
CodecInfo.CodecId.Encoding.PartId[0] = 0x13;
CodecInfo.CodecId.Encoding.PartId[1] = 0x08;
CodecInfo.CodecId.Encoding.Class = 0x00;

Command.TxWrite.OpCode  = SndwCmdWrite;  // write data to codec
Command.TxWrite.RegAddr = 0x42;          // address of regioster in codec
Command.TxWrite.RegData = 0x21;          // new register value

SndwAccess->SendWithAck (SndwAccess, CodecInfo, Command, CommandRespons);
if (!EFI_ERROR (Status) && CommandRespons->Rx.Ack == 1) {
  // command success
}
```

During the codecs enumeration process codecs list is created. From this list user can retrieve information about any found codec. ```GetFirstCodec``` function allows to get information about first codec discovered during codecs enumeration. Parameters for this function are pointer to the *SndwAccess* PPI/Protocol and pointer to pointer to codec information structure (```SNDW_CODEC_INFO **CodecInfo```). ```GetFirstCodec``` allocates memory for codec information structure and user should free up this memory with ```FreePool```.</br>
```GetNextCodec``` retrieves codec information about codec that follows the given one. Parameters for this function are are pointer to *SndwAccess* PPI/protocol, pointer to codec information structure (```SNDW_CODEC_INFO *CodecInfo```) preceding the one that user wants to get and pointer to pointer to codec information structure (```SNDW_CODEC_INFO **NextCodecInfo```). ```GetNextCodec``` allocates memory for codec information structure and user should free up this memory with ```FreePool```. If there is no codec in codecs list following the given one, value pointed out by ```NextCodecInfo``` will be set to ```NULL```.

Example:

```C
SNDW_CODEC_INFO         *CodecInfo;
SNDW_CODEC_INFO         *NextCodecInfo;

SndwAccess->GetFirstCodec (SndwAccess, &NextCodecInfo);

while (NextCodecInfo != NULL) {
  DEBUG ((DEBUG_INFO, "------- SNDW CODEC -------\n"));

  DEBUG ((DEBUG_INFO, "SndwLinkIndex:        %d\n", NextCodecInfo->SndwLinkIndex));
  DEBUG ((DEBUG_INFO, "Codec ID:\n"));
  DEBUG ((DEBUG_INFO, "  Version:            0x%02x\n", NextCodecInfo->CodecId.Encoding.Version));
  DEBUG ((DEBUG_INFO, "  ManufacturerID[0]:  0x%02x\n", NextCodecInfo->CodecId.Encoding.ManufacturerID[0]));
  DEBUG ((DEBUG_INFO, "  ManufacturerID[1]:  0x%02x\n", NextCodecInfo->CodecId.Encoding.ManufacturerID[1]));
  DEBUG ((DEBUG_INFO, "  PartId[0]:          0x%02x\n", NextCodecInfo->CodecId.Encoding.PartId[0]));
  DEBUG ((DEBUG_INFO, "  PartId[1]:          0x%02x\n", NextCodecInfo->CodecId.Encoding.PartId[1]));
  DEBUG ((DEBUG_INFO, "  Class:              0x%02x\n", NextCodecInfo->CodecId.Encoding.Class));

  DEBUG ((DEBUG_INFO, "----------------------\n"));

  CodecInfo = NextCodecInfo;
  NextCodecInfo = NULL;
  SndwAccess->GetNextCodec (SndwAccess, CodecInfo, &NextCodecInfo);
  FreePool (CodecInfo);
}
```

### Beep PPI/Protocol

The Beep PPI/Protocol is installed differently depending on the interface that is being used:
* HD-Audio interface - the PPI/Protocol is installed after all driver's dependencies are available.
* SoundWire interface - the PPI/Protocol is installed as soon as possible after ```SndwAccess``` PPI/Protocol installation.

Before using functions for generating beep sound user should use ```Enable``` function, after that user can use ```Beep``` function.
Parameters for ```Beep``` are pointer to Beep PPI/Protocol, frequency in Hz, amplitude in dB, number of beep sound, duration of beep in milliseconds and pause between beeps in milliseconds. ```Beep``` function is blocking function.
It means that user cannot do anything before end of this function. Beep PPI/Protocol contains also function for non blocking beep sounds. ```BeepOn``` function alows to turn on tone generator.
Parameters for ```BeepOn``` are pointer to Beep PPI/Protocol, frequency in Hz, amplitude in dB. ```BeepOff``` allow to turn off tone generator. Parameter for ```BeepOff``` is pointer to Beep PPI/Protocol.
After all operations user should use ```Disable``` function. This step is important for further SoundWire components like OS drivers.

Example of generating beep:

Locating PPI/Protocol:
```C
  Status = gBS->LocateProtocol (
                  &gBeepProtocolGuid,
                  NULL,
                  (VOID **) &BeepProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to locate Protocol - %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }
```
```C
Status = PeiServicesLocatePpi (&gBeepPpiGuid, 0, NULL, (VOID **) &BeepPpi);
if (EFI_ERROR (Status)) {
  DEBUG ((DEBUG_ERROR, "%a(): Failed to locate PPI - %r\n", __FUNCTION__, Status));
  return EFI_NOT_FOUND;
}
```
Using the interface of Beep PPI/Protocol:
```C
Status = BeepProtocol->Enable (BeepProtocol);
Status = BeepProtocol->Beep (BeepProtocol, 250, 0, 2, 1000, 500);
Status = BeepProtocol->Disable (BeepProtocol);
```

## Build flows

No special tools are required to build this feature.

## Feature Enabling Checklist

1. Add *BeepPkg.dsc* entry, check if all required packages and libraries are available.
1. Add *BeepPei.fdf* (pre- or post-memory init phase) and *BeepDxe.fdf* to flash map file.
1. In case of using this package in the PEI phase - add required GPIO pin programming to the platform code and make sure that GPIOs are programmed before calling ```Enable``` from the ```Beep``` PPI/Protocol interface. This can be achieved by either adding the GPIO programming to the ```VpdPcdBoardGpioTable``` or using GpioV2WrapperLib. Refer to the [GPIO configuration section](#gpio-configuration) for the required pins configuration.
1. Locate the Beep PPI/Protocol and use its interface as shown in *BeepExample.c*.

## Implementation note

1.  In the case of Beep on a HD-Audio codec, some of the parameters of functions from the interface are not used, thus some operations are not fully supported. </br>
This mainly applies to the ```Amplitude``` and ```Frequency``` parameters of the ```Beep``` and ```BeepOn``` functions. </br>
In this situation, in order to change the parameters of the beep generator on a HDA codec, one shall make appropriate changes directly in the verb tables in the ```HdaBeepVerbTables.c``` file accordingly to the codec specification.
1. In a rare case, when both interfaces are used and two the same Beep PPIs/protocols are produced, there is no way of differentiating them during the consumption of the PPI/protocol.
