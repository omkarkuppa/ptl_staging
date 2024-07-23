# Overview

* Feature Name: AmtPetAlertFeaturePkg
* PI Phase(s) Supported: DXE
* SMM Required: No

## Purpose

This package implements support for sending information about the boot device to AMT prior to OS boot.

For more information please refer to [Alert Standard Format specification](https://www.dmtf.org/standards/asf) and **Intel&reg; Converged Security and Management Guide BIOS Specification** (chapter *Boot Audit Entry (BAE) PET Alert*).

# High-Level Theory of Operation

If Intel AMT is enabled in the system, BIOS shall send a Platform Event Trap Alert (PET Alert) detailing the boot device to AMT before passing control to the OS.
This step is required on normal boot process and S4/S5 resume path.

### Boot without a boot device change

If the boot source is a primary ATA hard disk, the disk manufacturer's ID and serial number are compared with the previous saved boot data. Standard BIOS process per ASF definitions:

Field                  | Value      | Description
---------------------- | ---------- | --------
```ASF Command```      | ```0x04``` | Message
```ASF Message Size``` | ```0x10``` | Number of bytes in ASF message beyond this byte
```ASF Subcommand```   | ```0x16``` | Push with no retransmit
```ASF Version```      | ```0x10``` | Version number
```Event Sensor Type```| ```0x0F``` | System firmware error or progress
```Event Type```       | ```0x6F``` | Sensor specific
```Event Offset```     | ```0x02``` | System firmware progress
```Event Source Type```| ```0x68``` | ASF 1.0
```Event Severity```   | ```0x08``` | Non-critical
```Sensor Device```    | ```0xFF``` | Unspecified
```Sensor Number```    | ```0xFF``` | Unspecified
```Entity```           | ```0x22``` | BIOS supplying this information
```Entity Instance```  | ```0x00``` | Unspecified
```Event Data 1```     | ```0x40``` | Next byte conforms to ASF specification
```Event Data 2```     | ```0x13``` | Starting operating system boot process
```Event Data 3```     | ```0x00``` | Reserved
```Event Data 4```     | ```0x00``` | Reserved
```Event Data 5```     | ```0x00``` | Reserved
</br>
### Boot with a boot device change

If the boot source is mismatched or it is either "network" or "removable device", such as diskette, CDROM, or any USB attached boot source, an entry is placed in a persistent first-in-first-out 3 entry queue, which is called "Boot Audit Entry" (BAE). An expanded message, which contains the BAE, is sent to Intel AMT whenever the BAE is updated. Thus this "unusual event" is reliably forwarded and uniquely identified to the network management agent:

Field                  | Value      | Description
---------------------- | ---------- | -----------
```ASF Command```      | ```0x04``` | Message
```ASF Message Size``` | ```0x10``` | Number of bytes in ASF message beyond this byte
```ASF Subcommand```   | ```0x15``` | Push with retransmit
```ASF Version```      | ```0x10``` | Version number
```Event Sensor Type```| ```0x0F``` | System firmware error or progress
```Event Type```       | ```0x6F``` | Sensor specific
```Event Offset```     | ```0x02``` | System firmware progress
```Event Source Type```| ```0x68``` | ASF 1.0
```Event Severity```   | ```0x10``` | Critical - abnormal boot device used
```Sensor Device```    | ```0xFF``` | Unspecified
```Sensor Number```    | ```0xFF``` | Unspecified
```Entity```           | ```0x22``` | BIOS supplying this information
```Entity Instance```  | ```0x00``` | Unspecified
```Event Data 1```     | ```0xAA``` | Next 4 bytes are OEM Specific
```Event Data 2```     | ```0x13``` | Starting operating system boot process
```Event Data 3```     | ```N/A```  | Most recent BAE queue entry
```Event Data 4```     | ```N/A```  | Second most recent BAE queue entry
```Event Data 5```     | ```N/A```  | Oldest BAE queue entry
</br>
## Modules

* AmtPetAlertDxe.inf

## Configuration
Set ```gAmtPetAlertFeaturePkgTokenSpaceGuid.PcdAmtPetAlertFeatureEnable``` to ```TRUE``` to enable AMT Pet Alert feature.
</br>

## Data flows

Before determining which, AMT Pet alert message shall be issued, BIOS shall compare the current boot device with the one kept within [Pet Alert Configuration EFI variable](#AMT-Pet-Alert-Configuration-Variable)

### Amt Pet Alert Configuration Variable

Field Name                 | Description
-------------------------- | -----------
```BootQueue[3]```         | Boot audit entry queue. [Possible entries](#BAE-Queue-Values)
```BootHarddriveTag[64]``` | Last boot device tag

### BAE Queue Values

Field Name                 | Value      | Description
-------------------------- | ---------- | -----------
```Empty Queue```          | ```0x7F``` | Last Entry in the queue
```Network device```       | ```0x01``` | Attempted to boot to the network
```New hard drive```       | ```0x02``` | Booted to a new hard disk file
```Removable device```     | ```0x03``` | Booted to a removable device

## Build flows

No special tools are required to build this feature.

## Functional Exit Criteria

Once the feature is enabled on the platform, AMT Pet alert shall be sent prior to OS boot.

## Feature Enabling Checklist

1. Add AmtPetAlertFeaturePkg.dsc entry and path to your build file and check if all required packages/libraries exists for this package.
2. Add AmtPetAlertFeaturePkg.fdf entry and path to your flash map file.
