# Overview
* **Feature Name:** UFSProvisioningTool

## Version
   - Format : YYYY.MM.DD.XX
   - Current version : 2022.06.02.01

## Purpose
UFS provisioing tool will provision the UFS device with the golden configuration. Below features can be supported:
* Provision UFS device
* Override the existing configuration
* Lock the UFS device
This features can be used on EFI shell interface

## Modules
* UFSProvisioningTool.inf

# High-Level Theory of Operation
The tool will be using the following BIOS APIs for reading and writing the UFS device descriptors, flags, and attributes:
///
/// UFS Device Config Protocol structure.
///
struct _EFI_UFS_DEVICE_CONFIG_PROTOCOL {
    EFI_UFS_DEVICE_CONFIG_RW_DESCRIPTOR RwUfsDescriptor;
    EFI_UFS_DEVICE_CONFIG_RW_FLAG       RwUfsFlag;
    EFI_UFS_DEVICE_CONFIG_RW_ATTRIBUTE  RwUfsAttribute;
};

## Configuration
  Based on device version following are the configuration:

  bLength              0x90(For UFS 2.1 or 3.0)
  bLength              0xE6(For UFS 2.2 or 3.1 or 4.0)
  bDescriptorIDN       0x01
  bConfDescContinue    0x00
  bBootEnable          0x01
  bDescrAccessEn       0x01
  bInitPowerMode       0x01
  bHighPriorityLUN     0x7F
  bSecureRemovalType   0x00
  bInitActiveICCLevel  0x0F
  wPeriodicRTCUpdate   0x00
  bRPMBRegionEnable    0x00
  bRPMBRegion1Size     0x00
  bRPMBRegion2Size     0x00
  bRPMBRegion3Size     0x00
  ## When UFS device is 2.2 or 3.1 and above
  bWriteBoosterBufferPreserveUserSpaceEn  0x01
  bWriteBoosterBufferType                 0x01
  dNumSharedWriteBoosterBufferAllocUnits  Calculate (minimum of gMaxWriteBooster and 16GB in allocation units)

  ## Fill the first 3 Unit Descriptors configurable parameters as following:
  Configurable params                 LU 0 Values                                                     LU 1 Values                             LU 2 Values
  bLUEnable                           0x01                                                            0x01                                    0x01
  bBootLunID                          0x00                                                            0x01                                    0x02
  bLUWriteProtect                     0x00                                                            0x00                                    0x00
  bMemoryType                         0x00(Normal)                                                    0x03(Enhanced)                          0x03(Enhanced)
  dNumAllocUnits                      device capacity minus LU1 and LU2 sizes with adjustment factor  Calculate (32MB with adjustment factor) Calculate (32MB with adjustment factor)
  bDataReliability                    0x01                                                            0x01                                    0x01
  bLogicalBlockSize                   0x0C                                                            0x0C                                    0x0C
  bProvisioningType                   0x02                                                            0x02                                    0x02
  wContextCapabilities                0x0000                                                          0x0000                                  0x0000
  ## When UFS device is 2.2 or 3.1 and above
  dLUNumWriteBoosterBufferAllocUnits   0x00000000  0x00000000  0x00000000
  (Note: LU1 and LU2 are 32MB boot partitions that are currnetly not used by Windows)

## Build Environment
   1. As part of BIOS build UFSProvisioningTool module will be built by default and the output UFSProvisioningTool.efi will be available in the below path,
      "\Build\AlderLakeBoardPkg\DEBUG_VS2019\X64\UfsFeaturePkg\UfsProvisioningTool\UFSProvisioningTool\OUTPUT"
   
   2. UFSProvisioningTool supports stand alone build and following are the steps,
        - Open command promt from "\Intel\AlderlakeBoardPkg"
        - Before standalone build prepare the build command using prep command
            Example : \Intel\AlderlakeBoardPkg> prep 
        - Run build command
            Example : \Intel\AlderlakeBoardPkg> build -p \UfsFeaturePkg\UfsFeaturePkg.dsc
        - The output UFSProvisioningTool.efi will be available in the below path,
          "\Build\UfsFeaturePkg\DEBUG_VS2019\X64"  

## BKM for EFI Application
1. Copy the EFI application "UFSProvisioningTool.efi" to the USB storage device.

2. Connect the USB storage device to the ADL-N SUT and boot it to the UEFI shell.

3. Go to the USB filesystem partition where the EFI application was copied as shown below for instance,

    Shell > FS0: <press Enter>
    FS0 :\> UFSProvisioningTool.efi

4. To provision the UFS device run the below command,
   Run: UFSProvisioningTool.efi

   |---------------------------------------------------------------------------------------------------------------------- |
   |    FS0 :\> UFSProvisioningTool.efi                                                                                    |
   |   Welcome to UFS Provisioning Tool EFI Utility, Version 0.1                                                           |
   |       Usage:ersion = 0x0                                                                                              |
   |            'UFSProvisioningTool.efi F' to force the reprovisioning                                                    |
   |            'UFSProvisioningTool.efi L' to permanently lock the configuration!!!                                       |
   |            'UFSProvisioningTool.efi' to check the blank device and then provision                                     |
   |    [UFS Provisioning Tool ] Read Device descriptor status : Success DeviceDescriptorSize=0x40                         |
   |    [UFS Provisioning Tool ] Read Device descriptor status : Success DeviceDescriptorSize=0x59 OriginalSize=0x59       |
   |    [UFS Provisioning Tool ] Printing Device Descriptor                                                                |
   |    bLength : 59x0                                                                                                     |
   |    bDescriptorIDN : 0                                                                                                 |
   |    bDevice : 00x0                                                                                                     |
   |    bDeviceClass : 0                                                                                                   |
   |    bDeviceSubClass : 0                                                                                                |
   |    bProtocol : 00                                                                                                     |
   |    bNumberLU : 30                                                                                                     |
   |    bNumberWLU : 4                                                                                                     |
   |    bBootEnable : 1                                                                                                    |
   |    bDescrAccessEn : 1                                                                                                 |
   |    bInitPowerMode : 1                                                                                                 |
   |    bHighPriorityLUN : 7F                                                                                              |
   |    bSecureRemovalType : 0                                                                                             |
   |    bSecurityLU : 1                                                                                                    |
   |    bBackgroundOpsTermLat : 4                                                                                          |
   |    bInitActiveICCLevel : F                                                                                            |
   |    wSpecVersion : 310                                                                                                 |
   |    wManufactureDate : 1021                                                                                            |
   |    iManufacturerName : 0                                                                                              |
   |    iProductName : 1                                                                                                   |
   |    iSerialNumber : 2                                                                                                  |
   |    iOemID : 3 0x0                                                                                                     |
   |    wManufacturerID : 1CE                                                                                              |
   |    bUD0BaseOffset : 16                                                                                                |
   |    bUDConfigPLength : 1A                                                                                              |
   |    bDeviceRTTCap : 2                                                                                                  |
   |    wPeriodicRTCUpdate : 0                                                                                             |
   |    bUFSFeaturesSupport : 0x83                                                                                         |
   |-----------------------------------------------------------------------------------------------------------------------|

5. As a result of step 4 if user gets output as UFS device is already provisioned like below,

    [UFS Provisioning Tool] UFS spec Version=0x310
    [UFS Provisioning Tool] already provisioned
     Use 'UFSProvisioningTool.efi F' to force the reprovisioning

    then use below command to forcefully override the existing configuration,
    Run: UFSProvisioningTool.efi F

    |----------------------------------------------------------------------------------------------------------------------|
    |   FS0 :\> UFSProvisioningTool.efi F                                                                                  |
    |   Welcome to UFS Provisioning Tool EFI Utility, Version 0.1                                                          |
    |       Usage:ersion = 0x0                                                                                             |
    |           'UFSProvisioningTool.efi F' to force the reprovisioning                                                    |
    |           'UFSProvisioningTool.efi L' to permanently lock the configuration!!!                                       |
    |           'UFSProvisioningTool.efi' to check the blank card and then provision                                       |
    |   [UFS Provisioning Tool ] Read Device descriptor status : Success DeviceDescriptorSize=0x40                         |
    |   [UFS Provisioning Tool ] Read Device descriptor status : Success DeviceDescriptorSize=0x59 OriginalSize=0x59       |
    |   [UFS Provisioning Tool ] Printing Device Descriptor                                                                |
    |   bLength : 59x0eBoosterBufferAllocUnits = 0x1000                                                                    |
    |   bDescriptorIDN : 0----------------------- END OF DEVICE DESCRIPTOR -------------------------------------------     |
    |   bDevice : 00x0ing Tool ] UFS spec Version=0x310                                                                    |
    |   bDeviceClass : 0g Tool ] already provisioned                                                                       |
    |   bDeviceSubClass : 0ngTool.efi F' to force the reprovisioning                                                       |
    |   bProtocol : 00                                                                                                     |
    |   bNumberLU : 30                                                                                                     |
    |   bNumberWLU : 4                                                                                                     |
    |   bBootEnable : 1                                                                                                    |
    |   bDescrAccessEn : 1                                                                                                 |
    |   bInitPowerMode : 1                                                                                                 |
    |   bHighPriorityLUN : 7F                                                                                              |
    |   bSecureRemovalType : 0                                                                                             |
    |   bSecurityLU : 1                                                                                                    |
    |   bBackgroundOpsTermLat : 4                                                                                          |
    |   bInitActiveICCLevel : F                                                                                            |
    |   wSpecVersion : 310                                                                                                 |
    |   wManufactureDate : 1021                                                                                            |
    |   iManufacturerName : 0                                                                                              |
    |   iProductName : 1                                                                                                   |
    |   iSerialNumber : 2                                                                                                  |
    |   iOemID : 3 0x0                                                                                                     |
    |   wManufacturerID : 1CE                                                                                              |
    |   bUD0BaseOffset : 16                                                                                                |
    |   bUDConfigPLength : 1A                                                                                              |
    |   bDeviceRTTCap : 2                                                                                                  |
    |   wPeriodicRTCUpdate : 0                                                                                             |
    |   bUFSFeaturesSupport = 0x83                                                                                         |
    |   bFFUTimeout = 0x1                                                                                                  |
    |   bQueueDepth = 0x20                                                                                                 |
    |   wDeviceVersion = 0x0                                                                                               |
    |   bNumSecureWPArea = 0x20                                                                                            |
    |   dPSAMaxDataSize = 0x9EB555                                                                                         |
    |   bPSAStateTimeout = 0x0                                                                                             |
    |----------------------------------------------------------------------------------------------------------------------|

6. If user wants to lock the UFS device configuration, run the below command (Note: once device is locked, the configuration can not be changed and there is no way to unlock the device),
     Run: UFSProvisioningTool.efi L
