/** @file
  UFS Provisioning Application for Intel platforms.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/ShellLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Protocol/UfsDeviceConfig.h>
#include <UFSProvisioningTool.h>

EFI_UFS_DEVICE_CONFIG_PROTOCOL *gEfiUfsDeviceConfigProtocol;

UFS_PROVISIONING_CONFIGURATION  gUfsProvisioningConfiguration = {
  {                                 // DevConfParams
    3,                              // NumberOfLUs
    SIZE_16GB                       // Shared Write Booster Buffer Size in MB/GB, And is available only in UFS spec 2.2, 3.1 and 4.0
  },
  {
    {                               // LuConfParams[0]
      TRUE,                         // Cumulative count of enabled LUs should match to NumberOfLUs
      0x0,                          // LU is not write protected
      UfsMemoryTypeNormal,          // Check bLUWriteProtect in Ufs Specification, currently Tool supports UfsMemoryTypeNormal/UfsMemoryTypeEnhanced1
      0x0                           // Size in MB/GB, Should be multiples of (bAllocationUnitSize * dSegmentSize) of the Card. 0 means no preference(allocate remaining size), applicable only LU0.
    },
    {                               // LuConfParams[1]
      TRUE,                         // Cumulative count of enabled LUs should match to NumberOfLUs
      0x0,                          // LU is not write protected
      UfsMemoryTypeEnhanced1,       // Check bLUWriteProtect in Ufs Specification, currently Tool supports UfsMemoryTypeNormal/UfsMemoryTypeEnhanced1
      SIZE_32MB                     // Size in MB/GB, Should be multiples of (bAllocationUnitSize * dSegmentSize) of the Card. 0 means no preference(allocate remaining size), applicable only LU0.
    },
    {                               // LuConfParams[2]
      TRUE,                         // Cumulative count of enabled LUs should match to NumberOfLUs
      0x0,                          // LU is not write protected
      UfsMemoryTypeEnhanced1,       // Check bLUWriteProtect in Ufs Specification, currently Tool supports UfsMemoryTypeNormal/UfsMemoryTypeEnhanced1
      SIZE_32MB                     // Size in MB/GB, Should be multiples of (bAllocationUnitSize * dSegmentSize) of the Card. 0 means no preference(allocate remaining size), applicable only LU0.
    },
    {                               // LuConfParams[3]
      FALSE,                        // Cumulative count of enabled LUs should match to NumberOfLUs
      0x0,                          // LU is not write protected
      UfsMemoryTypeNormal,          // Check bLUWriteProtect in Ufs Specification, currently Tool supports UfsMemoryTypeNormal/UfsMemoryTypeEnhanced1
      0x0                           // Size in MB/GB, Should be multiples of (bAllocationUnitSize * dSegmentSize) of the Card. 0 means no preference(allocate remaining size), applicable only LU0.
    },
    {                               // LuConfParams[4]
      FALSE,                        // Cumulative count of enabled LUs should match to NumberOfLUs
      0x0,                          // LU is not write protected
      UfsMemoryTypeNormal,          // Check bLUWriteProtect in Ufs Specification, currently Tool supports UfsMemoryTypeNormal/UfsMemoryTypeEnhanced1
      0x0                           // Size in MB/GB, Should be multiples of (bAllocationUnitSize * dSegmentSize) of the Card. 0 means no preference(allocate remaining size), applicable only LU0.
    },
    {                               // LuConfParams[5]
      FALSE,                        // Cumulative count of enabled LUs should match to NumberOfLUs
      0x0,                          // LU is not write protected
      UfsMemoryTypeNormal,          // Check bLUWriteProtect in Ufs Specification, currently Tool supports UfsMemoryTypeNormal/UfsMemoryTypeEnhanced1
      0x0                           // Size in MB/GB, Should be multiples of (bAllocationUnitSize * dSegmentSize) of the Card. 0 means no preference(allocate remaining size), applicable only LU0.
    },
    {                               // LuConfParams[6]
      FALSE,                        // Cumulative count of enabled LUs should match to NumberOfLUs
      0x0,                          // LU is not write protected
      UfsMemoryTypeNormal,          // Check bLUWriteProtect in Ufs Specification, currently Tool supports UfsMemoryTypeNormal/UfsMemoryTypeEnhanced1
      0x0                           // Size in MB/GB, Should be multiples of (bAllocationUnitSize * dSegmentSize) of the Card. 0 means no preference(allocate remaining size), applicable only LU0.
    },
    {                               // LuConfParams[7]
      FALSE,                        // Cumulative count of enabled LUs should match to NumberOfLUs
      0x0,                          // LU is not write protected
      UfsMemoryTypeNormal,          // Check bLUWriteProtect in Ufs Specification, currently Tool supports UfsMemoryTypeNormal/UfsMemoryTypeEnhanced1
      0x0                           // Size in MB/GB, Should be multiples of (bAllocationUnitSize * dSegmentSize) of the Card. 0 means no preference(allocate remaining size), applicable only LU0.
    }
  }
};

UINT16  gVersion = Ufs_2_1;
UINT8   gLUNS = 0;
UINT8   gForceOverride = FALSE;
UINT8   gConfigurationLock = FALSE;
UINT64  gAvailableAllocUnits = 0;
UINT64  gWriteBoosterSize = SIZE_16GB;
UINT32  gMaxWriteBoosterAllocUnits = 0;
UINT32  gWriteBoosterBufferAllocUnits = 0;

/**
  Swaps the Endiannness of the Data buffer given.

  @param[in, out] Buffer      The data buffer to swap.
  @param[in]      BufferSize  The length of converted data.

**/
VOID
SwapEndianness (
  IN OUT UINT8   *Buffer,
  IN     UINT32  BufferSize
  )
{
  UINT32  Index;
  UINT8   Temp;
  UINT32  SwapCount;

  SwapCount = BufferSize / 2;
  for (Index = 0; Index < SwapCount; Index++) {
    Temp                           = Buffer[Index];
    Buffer[Index]                  = Buffer[BufferSize - 1 - Index];
    Buffer[BufferSize - 1 - Index] = Temp;
  }
}

/**
  Print the Device descriptor values

  @param[in] DeviceDescriptor  Points to the device descriptor structure.
**/
VOID
PrintDeviceDescriptor (
  IN UFS_DEV_DESC      *DeviceDescriptor,
  IN UFS_SPEC_VERSION  SpecVersion
  )
{
  UINT8  Index = 0;

  Print (L"[UFS Provisioning Tool] Printing Device Descriptor\n");
  Print (L"bLength : %x\n", DeviceDescriptor->bLength);
  Print (L"bDescriptorIDN : %x\n", DeviceDescriptor->bDescriptorIDN);
  Print (L"bDevice : %x\n", DeviceDescriptor->bDevice);
  Print (L"bDeviceClass : %x\n", DeviceDescriptor->bDeviceClass);
  Print (L"bDeviceSubClass : %x\n", DeviceDescriptor->bDeviceSubClass);
  Print (L"bProtocol : %x\n", DeviceDescriptor->bProtocol);
  Print (L"bNumberLU : %x\n", DeviceDescriptor->bNumberLU);
  Print (L"bNumberWLU : %x\n", DeviceDescriptor->bNumberWLU);
  Print (L"bBootEnable : %x\n", DeviceDescriptor->bBootEnable);
  Print (L"bDescrAccessEn : %x\n", DeviceDescriptor->bDescrAccessEn);
  Print (L"bInitPowerMode : %x\n", DeviceDescriptor->bInitPowerMode);
  Print (L"bHighPriorityLUN : %x\n", DeviceDescriptor->bHighPriorityLUN);
  Print (L"bSecureRemovalType : %x\n", DeviceDescriptor->bSecureRemovalType);
  Print (L"bSecurityLU : %x\n", DeviceDescriptor->bSecurityLU);
  Print (L"bBackgroundOpsTermLat : %x\n", DeviceDescriptor->bBackgroundOpsTermLat);
  Print (L"bInitActiveICCLevel : %x\n", DeviceDescriptor->bInitActiveICCLevel);
  Print (L"wSpecVersion : %x\n", DeviceDescriptor->wSpecVersion);
  Print (L"wManufactureDate : %x\n", DeviceDescriptor->wManufactureDate);
  Print (L"iManufacturerName : %x\n", DeviceDescriptor->iManufacturerName);
  Print (L"iProductName : %x\n", DeviceDescriptor->iProductName);
  Print (L"iSerialNumber : %x\n", DeviceDescriptor->iSerialNumber);
  Print (L"iOemID : %x\n", DeviceDescriptor->iOemID);
  Print (L"wManufacturerID : %x\n", DeviceDescriptor->wManufacturerID);
  Print (L"bUD0BaseOffset : %x\n", DeviceDescriptor->bUD0BaseOffset);
  Print (L"bUDConfigPLength : %x\n", DeviceDescriptor->bUDConfigPLength);
  Print (L"bDeviceRTTCap : %x\n", DeviceDescriptor->bDeviceRTTCap);
  Print (L"wPeriodicRTCUpdate : %x\n", DeviceDescriptor->wPeriodicRTCUpdate);
  Print (L"bUFSFeaturesSupport = 0x%x\n", DeviceDescriptor->bUFSFeaturesSupport);
  Print (L"bFFUTimeout = 0x%x\n", DeviceDescriptor->bFFUTimeout);
  Print (L"bQueueDepth = 0x%x\n", DeviceDescriptor->bQueueDepth);
  Print (L"wDeviceVersion = 0x%x\n", DeviceDescriptor->wDeviceVersion);
  Print (L"bNumSecureWPArea = 0x%x\n", DeviceDescriptor->bNumSecureWPArea);
  Print (L"dPSAMaxDataSize = 0x%x\n", DeviceDescriptor->dPSAMaxDataSize);
  Print (L"bPSAStateTimeout = 0x%x\n", DeviceDescriptor->bPSAStateTimeout);
  Print (L"iProductRevisionLevel = 0x%x\n", DeviceDescriptor->iProductRevisionLevel);
  for (Index = 0; Index < 5; Index++) {
    Print (L"Rsvd1[%d] = 0x%x\n", Index, DeviceDescriptor->Rsvd1[Index]);
  }
  for (Index = 0; Index < 16; Index++) {
    Print (L"Rsvd2[%d] = 0x%x\n", Index, DeviceDescriptor->Rsvd2[Index]);
  }

  if ((SpecVersion == Ufs_2_2) || (SpecVersion >= Ufs_3_1)) {
    for (Index = 0; Index < 3; Index++) {
      Print (L"Rsvd3[%d] = 0x%x\n", Index, DeviceDescriptor->Rsvd3[Index]);
    }
    for (Index = 0; Index < 12; Index++) {
      Print (L"Rsvd4[%d] = 0x%x\n", Index, DeviceDescriptor->Rsvd4[Index]);
    }
    Print (L"dExtendedUFSFeaturesSupport : %x\n", DeviceDescriptor->dExtendedUFSFeaturesSupport);
    Print (L"bWriteBoosterBufferPreserveUserSpaceEn = 0x%x\n", DeviceDescriptor->bWriteBoosterBufferPreserveUserSpaceEn);
    Print (L"bWriteBoosterBufferType = 0x%x\n", DeviceDescriptor->bWriteBoosterBufferType);
    Print (L"dNumSharedWriteBoosterBufferAllocUnits = 0x%x\n", DeviceDescriptor->dNumSharedWriteBoosterBufferAllocUnits);
  }
  Print (L"-------------------------------------END OF DEVICE DESCRIPTOR------------------------------------------\n");
}

/**
  Print the Geometry descriptor values

  @param[in] GeometryDeviceDescriptor  Points to the Geometry descriptor structure.
**/
VOID
PrintGeometryDescriptor (
  IN UFS_GEOMETRY_DESC  *GeometryDeviceDescriptor,
  IN UFS_SPEC_VERSION    SpecVersion
  )
{
  UINT8  Index = 0;

  Print (L"[UFS Provisioning Tool] Printing Geometry descriptor\n");
  Print (L"bLength : %x\n", GeometryDeviceDescriptor->bLength);
  Print (L"bDescriptorIDN : %x\n", GeometryDeviceDescriptor->bDescriptorIDN);
  Print (L"bMediaTechnology : %x\n", GeometryDeviceDescriptor->bMediaTechnology);
  Print (L"Reserved : %x\n", GeometryDeviceDescriptor->Reserved);
  Print (L"qTotalRawDeviceCapacity : %x\n", GeometryDeviceDescriptor->qTotalRawDeviceCapacity);
  Print (L"bMaxNumberLU : %x\n", GeometryDeviceDescriptor->bMaxNumberLU);
  Print (L"dSegmentSize : %x\n", GeometryDeviceDescriptor->dSegmentSize);
  Print (L"bAllocationUnitSize : %x\n", GeometryDeviceDescriptor->bAllocationUnitSize);
  Print (L"bMinAddrBlockSize : %x\n", GeometryDeviceDescriptor->bMinAddrBlockSize);
  Print (L"bOptimalReadBlockSize : %x\n", GeometryDeviceDescriptor->bOptimalReadBlockSize);
  Print (L"bOptimalWriteBlockSize : %x\n", GeometryDeviceDescriptor->bOptimalWriteBlockSize);
  Print (L"bMaxInBufferSize : %x\n", GeometryDeviceDescriptor->bMaxInBufferSize);
  Print (L"bMaxOutBufferSize : %x\n", GeometryDeviceDescriptor->bMaxOutBufferSize);
  Print (L"bRPMB_ReadWriteSize : %x\n", GeometryDeviceDescriptor->bRPMB_ReadWriteSize);
  Print (L"bDynamicCapacityResourcePolicy : %x\n", GeometryDeviceDescriptor->bDynamicCapacityResourcePolicy);
  Print (L"bDataOrdering : %x\n", GeometryDeviceDescriptor->bDataOrdering);
  Print (L"bMaxContexIDNumber : %x\n", GeometryDeviceDescriptor->bMaxContexIDNumber);
  Print (L"bSysDataTagUnitSize : %x\n", GeometryDeviceDescriptor->bSysDataTagUnitSize);
  Print (L"bSysDataTagResSize : %x\n", GeometryDeviceDescriptor->bSysDataTagResSize);
  Print (L"bSupportedSecRTypes : %x\n", GeometryDeviceDescriptor->bSupportedSecRTypes);
  Print (L"wSupportedMemoryTypes : %x\n", GeometryDeviceDescriptor->wSupportedMemoryTypes);
  Print (L"dSystemCodeMaxNAllocU : %x\n", GeometryDeviceDescriptor->dSystemCodeMaxNAllocU);
  Print (L"wSystemCodeCapAdjFac : %x\n", GeometryDeviceDescriptor->wSystemCodeCapAdjFac);
  Print (L"dNonPersistMaxNAllocU : %x\n", GeometryDeviceDescriptor->dNonPersistMaxNAllocU);
  Print (L"wNonPersistCapAdjFac : %x\n", GeometryDeviceDescriptor->wNonPersistCapAdjFac);
  Print (L"dEnhanced1MaxNAllocU : %x\n", GeometryDeviceDescriptor->dEnhanced1MaxNAllocU);
  Print (L"wEnhanced1CapAdjFac : %x\n", GeometryDeviceDescriptor->wEnhanced1CapAdjFac);
  Print (L"dEnhanced2MaxNAllocU : %x\n", GeometryDeviceDescriptor->dEnhanced2MaxNAllocU);
  Print (L"wEnhanced2CapAdjFac : %x\n", GeometryDeviceDescriptor->wEnhanced2CapAdjFac);
  Print (L"dEnhanced3MaxNAllocU : %x\n", GeometryDeviceDescriptor->dEnhanced3MaxNAllocU);
  Print (L"wEnhanced3CapAdjFac : %x\n", GeometryDeviceDescriptor->wEnhanced3CapAdjFac);
  Print (L"dEnhanced4MaxNAllocU : %x\n", GeometryDeviceDescriptor->dEnhanced4MaxNAllocU);
  Print (L"wEnhanced4CapAdjFac : %x\n", GeometryDeviceDescriptor->wEnhanced4CapAdjFac);
  Print (L"dOptimalLogicalBlockSize : %x\n", GeometryDeviceDescriptor->dOptimalLogicalBlockSize);
  if ((SpecVersion == Ufs_2_2) || (SpecVersion >= Ufs_3_1)) {
    for (Index = 0; Index < 7; Index++) {
      Print (L"Res[%d] = 0x%x\n", Index, GeometryDeviceDescriptor->Res[Index]);
    }
    Print (L"dWriteBoosterBufferMaxNAllocUnits : %x\n", GeometryDeviceDescriptor->dWriteBoosterBufferMaxNAllocUnits);
    Print (L"bDeviceMaxWriteBoosterLUs : %x\n", GeometryDeviceDescriptor->bDeviceMaxWriteBoosterLUs);
    Print (L"bWriteBoosterBufferCapAdjFac : %x\n", GeometryDeviceDescriptor->bWriteBoosterBufferCapAdjFac);
    Print (L"bSupportedWriteBoosterBufferUserSpaceReductionTypes : %x\n", GeometryDeviceDescriptor->bSupportedWriteBoosterBufferUserSpaceReductionTypes);
    Print (L"bSupportedWriteBoosterBufferTypes : %x\n", GeometryDeviceDescriptor->bSupportedWriteBoosterBufferTypes);
  }
  Print (L"-------------------------------------END OF GEOMETRY DESCRIPTOR------------------------------------------\n");
}

/**
  Print the Configuration descriptor values

  @param[in] Config  Points to the Configuration descriptor structure.
**/
VOID
PrintConfigurationDescriptor (
  IN UFS_CONFIG_DESC  *Config
  )
{
  UINT8 Index;

  Print (L"[UFS Provisioning Tool] Printing Configuration descriptor \n");
  Print (L"bLength : %x\n", Config->Header.bLength);
  Print (L"bDescriptorIDN : %x\n", Config->Header.bDescriptorIDN);
  Print (L"bConfDescContinue : %x\n", Config->Header.bConfDescContinue);
  Print (L"bBootEnable : %x\n", Config->Header.bBootEnable);
  Print (L"bDescrAccessEn : %x\n", Config->Header.bDescrAccessEn);
  Print (L"bInitPowerMode : %x\n", Config->Header.bInitPowerMode);
  Print (L"bHighPriorityLUN : %x\n", Config->Header.bHighPriorityLUN);
  Print (L"bSecureRemovalType : %x\n", Config->Header.bSecureRemovalType);
  Print (L"bInitActiveICCLevel : %x\n", Config->Header.bInitActiveICCLevel);
  Print (L"wPeriodicRTCUpdate : %x\n", Config->Header.wPeriodicRTCUpdate);
  Print (L"Rsvd1 : %x\n", Config->Header.Rsvd1);
  Print (L"bRPMBRegionEnable : %x\n", Config->Header.bRPMBRegionEnable);
  Print (L"bRPMBRegion1Size : %x\n", Config->Header.bRPMBRegion1Size);
  Print (L"bRPMBRegion2Size : %x\n", Config->Header.bRPMBRegion2Size);
  Print (L"bRPMBRegion3Size : %x\n", Config->Header.bRPMBRegion3Size);
  Print (L"bWriteBoosterBufferPreserveUserSpaceEn : %x\n", Config->Header.bWriteBoosterBufferPreserveUserSpaceEn);
  Print (L"bWriteBoosterBufferType : %x\n", Config->Header.bWriteBoosterBufferType);
  Print (L"dNumSharedWriteBoosterBufferAllocUnits : %x\n", Config->Header.dNumSharedWriteBoosterBufferAllocUnits);
  for (Index = 0; Index < UFS_MAX_LUNS; Index++) {
    Print (L"For LU%d \n", Index);
    Print (L"bLUEnable : %x\n", Config->UnitDescConfParams[Index].bLUEnable);
    Print (L"bBootLunID : %x\n", Config->UnitDescConfParams[Index].bBootLunID);
    Print (L"bLUWriteProtect : %x\n", Config->UnitDescConfParams[Index].bLUWriteProtect);
    Print (L"bMemoryType : %x\n", Config->UnitDescConfParams[Index].bMemoryType);
    Print (L"dNumAllocUnits : %x\n", Config->UnitDescConfParams[Index].dNumAllocUnits);
    Print (L"bDataReliability : %x\n", Config->UnitDescConfParams[Index].bDataReliability);
    Print (L"bLogicalBlockSize : %x\n", Config->UnitDescConfParams[Index].bLogicalBlockSize);
    Print (L"bProvisioningType : %x\n", Config->UnitDescConfParams[Index].bProvisioningType);
    Print (L"wContextCapabilities : %x\n", Config->UnitDescConfParams[Index].wContextCapabilities);
    Print (L"Rsvd1 : %x\n", Config->UnitDescConfParams[Index].Rsvd1[0]);
    Print (L"Rsvd1 : %x\n", Config->UnitDescConfParams[Index].Rsvd1[1]);
    Print (L"Rsvd1 : %x\n", Config->UnitDescConfParams[Index].Rsvd1[2]);
    Print (L"Rsvd1 : %x\n", Config->UnitDescConfParams[Index].Rsvd2[0]);
    Print (L"Rsvd1 : %x\n", Config->UnitDescConfParams[Index].Rsvd2[1]);
    Print (L"Rsvd1 : %x\n", Config->UnitDescConfParams[Index].Rsvd2[2]);
    Print (L"Rsvd1 : %x\n", Config->UnitDescConfParams[Index].Rsvd2[3]);
    Print (L"Rsvd1 : %x\n", Config->UnitDescConfParams[Index].Rsvd2[4]);
    Print (L"Rsvd1 : %x\n", Config->UnitDescConfParams[Index].Rsvd2[5]);
    Print (L"dLUNumWriteBoosterBufferAllocUnits : %x\n", Config->UnitDescConfParams[Index].dLUNumWriteBoosterBufferAllocUnits);
  }
  Print (L"----------------------------END OF CONFIGURATION DESCRIPTOR------------------------------------\n");
}

/**
  Print the Configuration descriptor values for Ufs spec 2.1 and 3.0

  @param[in] Config  Points to the Configuration descriptor structure.
**/
VOID
PrintConfigurationDescriptor2p1And3p0 (
  IN UFS_CONFIG_DESC_2P1_3P0  *Config
  )
{
  UINT8 Index;

  Print (L"[UFS Provisioning Tool] Printing Configuration descriptor \n");
  Print (L"bLength : %x\n", Config->Header.bLength);
  Print (L"bDescriptorIDN : %x\n", Config->Header.bDescriptorIDN);
  Print (L"bConfDescContinue : %x\n", Config->Header.bConfDescContinue);
  Print (L"bBootEnable : %x\n", Config->Header.bBootEnable);
  Print (L"bDescrAccessEn : %x\n", Config->Header.bDescrAccessEn);
  Print (L"bInitPowerMode : %x\n", Config->Header.bInitPowerMode);
  Print (L"bHighPriorityLUN : %x\n", Config->Header.bHighPriorityLUN);
  Print (L"bSecureRemovalType : %x\n", Config->Header.bSecureRemovalType);
  Print (L"bInitActiveICCLevel : %x\n", Config->Header.bInitActiveICCLevel);
  Print (L"wPeriodicRTCUpdate : %x\n", Config->Header.wPeriodicRTCUpdate);
  if (gVersion == Ufs_3_0) {
    Print (L"Rsvd : %x\n", Config->Header.Rsvd);
    Print (L"bRPMBRegionEnable : %x\n", Config->Header.bRPMBRegionEnable);
    Print (L"bRPMBRegion1Size : %x\n", Config->Header.bRPMBRegion1Size);
    Print (L"bRPMBRegion2Size : %x\n", Config->Header.bRPMBRegion2Size);
    Print (L"bRPMBRegion3Size : %x\n", Config->Header.bRPMBRegion3Size);
  }
  for (Index = 0; Index < UFS_MAX_LUNS; Index++) {
    Print (L"For LU%d \n", Index);
    Print (L"bLUEnable : %x\n", Config->UnitDescConfParams[Index].bLUEnable);
    Print (L"bBootLunID : %x\n", Config->UnitDescConfParams[Index].bBootLunID);
    Print (L"bLUWriteProtect : %x\n", Config->UnitDescConfParams[Index].bLUWriteProtect);
    Print (L"bMemoryType : %x\n", Config->UnitDescConfParams[Index].bMemoryType);
    Print (L"dNumAllocUnits : %x\n", Config->UnitDescConfParams[Index].dNumAllocUnits);
    Print (L"bDataReliability : %x\n", Config->UnitDescConfParams[Index].bDataReliability);
    Print (L"bLogicalBlockSize : %x\n", Config->UnitDescConfParams[Index].bLogicalBlockSize);
    Print (L"bProvisioningType : %x\n", Config->UnitDescConfParams[Index].bProvisioningType);
    Print (L"wContextCapabilities : %x\n", Config->UnitDescConfParams[Index].wContextCapabilities);
    Print (L"Rsvd1[0] : %x\n", Config->UnitDescConfParams[Index].Rsvd1[0]);
    Print (L"Rsvd1[1] : %x\n", Config->UnitDescConfParams[Index].Rsvd1[1]);
    Print (L"Rsvd1[2] : %x\n", Config->UnitDescConfParams[Index].Rsvd1[2]);
  }
  Print (L"----------------------------END OF CONFIGURATION DESCRIPTOR------------------------------------\n");
}

/**
  Swaps the Endiannness of the Device descriptor.

  @param[in] Config  Points to the Configuration descriptor structure.
**/
VOID
ConvertDeviceDescriptorEndianness (
  IN UFS_DEV_DESC *DeviceDescriptor
  )
{
  SwapEndianness ((UINT8 *)&DeviceDescriptor->wSpecVersion, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&DeviceDescriptor->wManufactureDate, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&DeviceDescriptor->wManufacturerID, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&DeviceDescriptor->wPeriodicRTCUpdate, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&DeviceDescriptor->wDeviceVersion, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&DeviceDescriptor->dPSAMaxDataSize, sizeof (UINT32));
  if ((DeviceDescriptor->wSpecVersion == Ufs_2_2) || (DeviceDescriptor->wSpecVersion >= Ufs_3_1)) {
    SwapEndianness ((UINT8 *)&DeviceDescriptor->dExtendedUFSFeaturesSupport, sizeof (UINT32));
    SwapEndianness ((UINT8 *)&DeviceDescriptor->dNumSharedWriteBoosterBufferAllocUnits, sizeof (UINT32));
  }
}

/**
  Swaps the Endiannness of the Geometry descriptor.

  @param[in] Config  Points to the Configuration descriptor structure.
**/
VOID
ConvertGeometryDescriptorEndianness (
  IN UFS_GEOMETRY_DESC *GeometryDeviceDescriptor
  )
{
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->qTotalRawDeviceCapacity, sizeof (UINT64));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->dSegmentSize, sizeof (UINT32));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->wSupportedMemoryTypes, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->dSystemCodeMaxNAllocU, sizeof (UINT32));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->wSystemCodeCapAdjFac, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->dNonPersistMaxNAllocU, sizeof (UINT32));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->wNonPersistCapAdjFac, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->dEnhanced1MaxNAllocU, sizeof (UINT32));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->wEnhanced1CapAdjFac, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->dEnhanced2MaxNAllocU, sizeof (UINT32));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->wEnhanced2CapAdjFac, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->dEnhanced3MaxNAllocU, sizeof (UINT32));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->wEnhanced3CapAdjFac, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->dEnhanced4MaxNAllocU, sizeof (UINT32));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->wEnhanced4CapAdjFac, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->dOptimalLogicalBlockSize, sizeof (UINT32));
  if ((gVersion == Ufs_2_2) || (gVersion >= Ufs_3_1)) {
    SwapEndianness ((UINT8 *)&GeometryDeviceDescriptor->dWriteBoosterBufferMaxNAllocUnits, sizeof (UINT32));
  }
}

/**
  Swaps the Endiannness of the Configuration descriptor.

  @param[in] Config  Points to the Configuration descriptor structure.
**/
VOID
ConvertConfigDescriptorEndianness (
  IN UFS_CONFIG_DESC  *Config
  )
{
  UINT8 Index;

  SwapEndianness ((UINT8 *)&Config->Header.wPeriodicRTCUpdate, sizeof (UINT16));
  SwapEndianness ((UINT8 *)&Config->Header.dNumSharedWriteBoosterBufferAllocUnits, sizeof (UINT32));
  for (Index = 0; Index < UFS_MAX_LUNS ; Index++) {
    SwapEndianness ((UINT8 *)&Config->UnitDescConfParams[Index].dNumAllocUnits, sizeof (UINT32));
    SwapEndianness ((UINT8 *)&Config->UnitDescConfParams[Index].wContextCapabilities, sizeof (UINT16));
    SwapEndianness ((UINT8 *)&Config->UnitDescConfParams[Index].dLUNumWriteBoosterBufferAllocUnits, sizeof (UINT32));
  }
}

/**
  Swaps the Endiannness of the Configuration descriptor for 2.1 and 3.0 versions.

  @param[in] Config  Points to the Configuration descriptor structure.
**/
VOID
ConvertConfigDescriptor2p1And3p0Endianness (
  IN UFS_CONFIG_DESC_2P1_3P0  *Config
  )
{
  UINT8 Index;

  SwapEndianness ((UINT8 *)&Config->Header.wPeriodicRTCUpdate, sizeof (UINT16));
  for (Index = 0; Index < UFS_MAX_LUNS ; Index++) {
    SwapEndianness ((UINT8 *)&Config->UnitDescConfParams[Index].dNumAllocUnits, sizeof (UINT32));
    SwapEndianness ((UINT8 *)&Config->UnitDescConfParams[Index].wContextCapabilities, sizeof (UINT16));
  }
}

/***
  Print a welcoming message.
  Establishes the main structure of the application.
  @retval  0         The application exited normally.
  @retval  Other     An error occurred.
***/
INTN EFIAPI ShellAppMain (IN UINTN Argc, IN CHAR16 **Argv)
{
  EFI_STATUS               Status;
  CHAR16                   *Operation;
  UFS_DEV_DESC             DeviceDescriptor;
  UINT32                   DeviceDescriptorSize;
  UFS_GEOMETRY_DESC        GeometryDeviceDescriptor;
  UINT32                   GeometryDeviceDescriptorSize;
  UINT32                   Attributes;
  UINT32                   AttributesSize = sizeof(UINT32);
  UFS_CONFIG_DESC_2P1_3P0  Config2p13p0;
  UINT32                   ConfigSize2p13p0;
  UFS_CONFIG_DESC          Config;
  UINT32                   ConfigSize;
  INT8                     Index = 0;

  Print (L"Welcome to UFS Provisioning Tool EFI Utility, Version 0.1\n");
  Print (L"  Usage:\n");
  Print (L"    'UFSProvisioningTool.efi F' to force the reprovisioning\n");
  Print (L"    'UFSProvisioningTool.efi L' to permanently lock the configuration!!!\n");
  Print (L"    'UFSProvisioningTool.efi' to check the blank card and then provision\n");

  if (Argc > 1) {
    Operation = (CHAR16*)Argv[1];
    // Check command line parameters
    if ((Operation[0] == L'F') ) { // Operation == Force override ??
      gForceOverride = TRUE;       // Force override
    } else if (Operation[0] == L'L') {
      gConfigurationLock = TRUE;   // Lock the config after provisioning
    }
  }

  Status = gBS->LocateProtocol (&gEfiUfsDeviceConfigProtocolGuid, NULL, (VOID**) &gEfiUfsDeviceConfigProtocol);
  if (EFI_ERROR (Status)) {
    Print (L"[UFS Prov Tool - ERROR] Platform must implement this protocol. Please ensure your UFS storage device is present and detected on the system before run the tool %r\n", Status);
    return Status;
  }

  Attributes = UfsConfigDescrReserved;
  Status = gEfiUfsDeviceConfigProtocol->RwUfsAttribute (gEfiUfsDeviceConfigProtocol, TRUE, UfsAttrConfigDescLock, 0, 0, (UINT8 *)&Attributes, &AttributesSize);
  if (EFI_ERROR (Status)) {
    Print (L"[UFS Provisioning Tool - ERROR] Read Attribute failed Status : %r\n", Status);
    return Status;
  }

  if (Attributes == UfsConfigDescrLocked) {
    Print (L"[UFS Provisioning Tool] UFS configuration is locked \n");
    return 0;
  }

  DeviceDescriptorSize = sizeof(UFS_DEV_DESC);  //Read all bytes
  ZeroMem ((VOID*)&DeviceDescriptor, DeviceDescriptorSize);  //Clear data
  Status = gEfiUfsDeviceConfigProtocol->RwUfsDescriptor (gEfiUfsDeviceConfigProtocol, TRUE, UfsDeviceDesc, 0, 0, (UINT8*)&DeviceDescriptor, &DeviceDescriptorSize);
  if (EFI_ERROR (Status)) {
    Print (L"[UFS Provisioning Tool - ERROR] Read Device descriptor failed with error status : %r\n", Status);
    return Status;
  }
  Print (L"[UFS Provisioning Tool ] Read Device descriptor status : %r DeviceDescriptorSize=0x%x OriginalSize=0x%x\n", Status, DeviceDescriptorSize, sizeof(UFS_DEV_DESC));
  ConvertDeviceDescriptorEndianness (&DeviceDescriptor);
  gVersion = DeviceDescriptor.wSpecVersion;
  PrintDeviceDescriptor (&DeviceDescriptor, gVersion);
  Print (L"[UFS Provisioning Tool] UFS spec Version=0x%x \n", gVersion);

  // Check the device condition
  gLUNS = DeviceDescriptor.bNumberLU; // number of configured logical units
  if ((gLUNS > 0) && (!gForceOverride)) {
    Print (L"[UFS Provisioning Tool] already provisioned\n");
    Print (L" Use 'UFSProvisioningTool.efi F' to force the reprovisioning\n");
    return 0;
  }

  if ((gVersion == Ufs_2_2) || (gVersion >= Ufs_3_1)) {
    GeometryDeviceDescriptorSize = sizeof (UFS_GEOMETRY_DESC);
  } else {
    GeometryDeviceDescriptorSize = UFS_2_1_3_0_GEOMETRY_DESC_SIZE;
  }

  ZeroMem((VOID*)&GeometryDeviceDescriptor, sizeof (UFS_GEOMETRY_DESC));  //Clear data
  // Read Geometry Descriptor to fetch the Total Raw Device Capacity
  Status = gEfiUfsDeviceConfigProtocol->RwUfsDescriptor (gEfiUfsDeviceConfigProtocol, TRUE, UfsGeometryDesc, 0, 0, (UINT8*)&GeometryDeviceDescriptor, &GeometryDeviceDescriptorSize);
  if (EFI_ERROR (Status)) {
    Print (L"[UFS Provisioning Tool - ERROR] Geometry descriptor Read failed Status : %r\n", Status);
    return Status;
  }

  Print (L"[UFS Provisioning Tool ] Read Geometry descriptor status : %r GeometryDeviceDescriptorSize=0x%x\n", Status, GeometryDeviceDescriptorSize);
  ConvertGeometryDescriptorEndianness (&GeometryDeviceDescriptor);
  PrintGeometryDescriptor (&GeometryDeviceDescriptor, gVersion);

  gAvailableAllocUnits = GeometryDeviceDescriptor.qTotalRawDeviceCapacity/(GeometryDeviceDescriptor.dSegmentSize * GeometryDeviceDescriptor.bAllocationUnitSize);

  //
  // Get Ufs Device's Lun Info by reading Configuration Descriptor.
  //
  if ((gVersion == Ufs_2_2) || (gVersion >= Ufs_3_1)) {
    ConfigSize = sizeof(UFS_CONFIG_DESC);
    ZeroMem((VOID*)&Config, ConfigSize);  //Clear data
    Status = gEfiUfsDeviceConfigProtocol->RwUfsDescriptor (gEfiUfsDeviceConfigProtocol, TRUE, UfsConfigDesc, 0, 0, (UINT8*)&Config, &ConfigSize);
    if (EFI_ERROR (Status)) {
      Print (L"[UFS Provisioning Tool - ERROR] Read Configuration descriptor failed with error status : %r\n", Status);
      return Status;
    }

    Print (L"[UFS Provisioning Tool ] Read Configuration descriptor status : %r ConfigSize=0x%x OriginalSize=0x%x\n", Status, ConfigSize, sizeof(UFS_CONFIG_DESC));
    ConvertConfigDescriptorEndianness(&Config);
    PrintConfigurationDescriptor(&Config);

    // Length
    Config.Header.bLength = sizeof(UFS_CONFIG_DESC);

    //Config.Header.
    Config.Header.bDescriptorIDN = UfsConfigDesc;
    Config.Header.bConfDescContinue = 0x00;
    Config.Header.bBootEnable = 0x01;
    Config.Header.bDescrAccessEn = 0x01;
    Config.Header.bInitPowerMode = 0x01;
    Config.Header.bHighPriorityLUN = 0x7F;
    Config.Header.bSecureRemovalType = 0x00;
    Config.Header.bInitActiveICCLevel = 0x0F;
    Config.Header.wPeriodicRTCUpdate = 0x00;

    if (gVersion >= Ufs_3_1) {
      // No RPMB Regions
      Config.Header.bRPMBRegionEnable = 0x00;
      Config.Header.bRPMBRegion1Size = 0x00;
      Config.Header.bRPMBRegion2Size = 0x00;
      Config.Header.bRPMBRegion3Size = 0x00;
    }

    if ((DeviceDescriptor.dExtendedUFSFeaturesSupport & BIT8) &&
        ((GeometryDeviceDescriptor.bSupportedWriteBoosterBufferUserSpaceReductionTypes == 0x1) ||
          (GeometryDeviceDescriptor.bSupportedWriteBoosterBufferUserSpaceReductionTypes == 0x2)) &&
        ((GeometryDeviceDescriptor.bSupportedWriteBoosterBufferTypes == 0x1) ||
          (GeometryDeviceDescriptor.bSupportedWriteBoosterBufferTypes == 0x2))) {
      gWriteBoosterSize = gUfsProvisioningConfiguration.DevConfParams.SharedWriteBoosterBufferSize;
      gMaxWriteBoosterAllocUnits = GeometryDeviceDescriptor.dWriteBoosterBufferMaxNAllocUnits;
      if (gMaxWriteBoosterAllocUnits < (gWriteBoosterSize/(GeometryDeviceDescriptor.dSegmentSize * 512 * GeometryDeviceDescriptor.bAllocationUnitSize))) {
        gWriteBoosterBufferAllocUnits = gMaxWriteBoosterAllocUnits;
      } else {
        gWriteBoosterBufferAllocUnits = (gWriteBoosterSize/(GeometryDeviceDescriptor.dSegmentSize * 512 * GeometryDeviceDescriptor.bAllocationUnitSize)) & ((UINT32)~(0));
      }
      Print (L"[UFS Provisioning Tool]  gMaxWriteBoosterAllocUnits: %x gWriteBoosterBufferAllocUnits: %x\n", gMaxWriteBoosterAllocUnits, gWriteBoosterBufferAllocUnits);

      // Enable WriteBooster
      Config.Header.bWriteBoosterBufferPreserveUserSpaceEn = 0x01;
      Config.Header.bWriteBoosterBufferType = 0x01;
      Config.Header.dNumSharedWriteBoosterBufferAllocUnits = gWriteBoosterBufferAllocUnits;
    }

    for (Index = (UFS_MAX_LUNS-1); Index >= 0 ; Index--) {
      if ((gUfsProvisioningConfiguration.LuConfParams[Index].Enable == FALSE) ||
         ((gUfsProvisioningConfiguration.LuConfParams[Index].Size == 0) && (Index != 0))) {
        ZeroMem((VOID*)& Config.UnitDescConfParams[Index], sizeof(UFS_UNIT_DESC_CONFIG_PARAMS));  //Clear this unit descriptor data
        continue; //Skip this LU
      }
      Config.UnitDescConfParams[Index].bLUEnable = 0x01;
      if ((Index == 1) || (Index == 2)) {
        Config.UnitDescConfParams[Index].bBootLunID = Index;
      } else {
        Config.UnitDescConfParams[Index].bBootLunID = 0x00;
      }
      Config.UnitDescConfParams[Index].bLUWriteProtect = gUfsProvisioningConfiguration.LuConfParams[Index].WriteProtect;
      if (Index != 0) {
        if (gUfsProvisioningConfiguration.LuConfParams[Index].MemoryType == UfsMemoryTypeEnhanced1) {
          Config.UnitDescConfParams[Index].bMemoryType = UfsMemoryTypeEnhanced1;
          Config.UnitDescConfParams[Index].dNumAllocUnits = (gUfsProvisioningConfiguration.LuConfParams[Index].Size/(GeometryDeviceDescriptor.dSegmentSize * 512 * GeometryDeviceDescriptor.bAllocationUnitSize)) & ((UINT32)~(0));
          Config.UnitDescConfParams[Index].dNumAllocUnits *= ((GeometryDeviceDescriptor.wEnhanced1CapAdjFac/0x100));
        } else {
          Config.UnitDescConfParams[Index].bMemoryType = UfsMemoryTypeNormal;
          Config.UnitDescConfParams[Index].dNumAllocUnits = (gUfsProvisioningConfiguration.LuConfParams[Index].Size/(GeometryDeviceDescriptor.dSegmentSize * 512 * GeometryDeviceDescriptor.bAllocationUnitSize)) & ((UINT32)~(0));
        }
        if ((gAvailableAllocUnits - Config.UnitDescConfParams[Index].dNumAllocUnits) > 0) {
          gAvailableAllocUnits -= Config.UnitDescConfParams[Index].dNumAllocUnits;
        } else {
          //abort
          Print (L"[UFS Provisioning Tool - ERROR] Storage not enough for LU[%d]: gAvailableAllocUnits=0x%x AllocUnitsRequired 0x%x\n", Index, gAvailableAllocUnits, Config.UnitDescConfParams[Index].dNumAllocUnits);
          return EFI_UNSUPPORTED;
        }
      } else {
        Config.UnitDescConfParams[Index].bMemoryType = gUfsProvisioningConfiguration.LuConfParams[Index].MemoryType;
        Config.UnitDescConfParams[Index].dNumAllocUnits = gAvailableAllocUnits & ((UINT32)~(0));
      }

      Config.UnitDescConfParams[Index].bDataReliability = 0x01;
      Config.UnitDescConfParams[Index].bLogicalBlockSize = 0x0c;
      Config.UnitDescConfParams[Index].bProvisioningType = 0x02;
      Config.UnitDescConfParams[Index].wContextCapabilities = 0x0;
      if ((gVersion == Ufs_2_2) || (gVersion >= Ufs_3_1)) {
        Config.UnitDescConfParams[Index].dLUNumWriteBoosterBufferAllocUnits = 0x0;
      }
    }
    Print (L"[UFS Provisioning Tool] Printing Before Configuration descriptor write \n");
    ConvertConfigDescriptorEndianness(&Config);
    PrintConfigurationDescriptor(&Config);
    Print (L"-----------------------------------------------------------------------\n");
    ConfigSize = sizeof (UFS_CONFIG_DESC);
    Status = gEfiUfsDeviceConfigProtocol->RwUfsDescriptor (gEfiUfsDeviceConfigProtocol, FALSE, UfsConfigDesc, 0, 0, (UINT8*)&Config, &ConfigSize);
    MicroSecondDelay (CONFIG_DESC_WRITE_DELAY); //100ms wait after cofiguration descriptor write to make sure buffer was flushed.
    if (EFI_ERROR (Status)) {
      Print (L"[UFS Provisioning Tool - ERROR] Write Configuration descriptor failed with error status : %r\n", Status);
      return Status;
    }
    Print (L"[UFS Provisioning Tool ] Write Configuration descriptor status : %r ConfigSize=0x%x OriginalSize=0x%x\n", Status, ConfigSize, sizeof (UFS_CONFIG_DESC));
    ConfigSize = sizeof(UFS_CONFIG_DESC);
    ZeroMem((VOID*)&Config, ConfigSize);  //Clear data
    Status = gEfiUfsDeviceConfigProtocol->RwUfsDescriptor (gEfiUfsDeviceConfigProtocol, TRUE, UfsConfigDesc, 0, 0, (UINT8*)&Config, &ConfigSize);
    if (EFI_ERROR (Status)) {
      Print (L"[UFS Provisioning Tool - ERROR] Read Configuration descriptor failed with error status : %r\n", Status);
      return Status;
    }
    Print (L"[UFS Provisioning Tool ] Read Configuration descriptor status : %r ConfigSize=0x%x OriginalSize=0x%x\n", Status, ConfigSize, sizeof (UFS_CONFIG_DESC));
    ConvertConfigDescriptorEndianness(&Config);
    PrintConfigurationDescriptor(&Config);
  } else {
    ConfigSize2p13p0 = sizeof(UFS_CONFIG_DESC_2P1_3P0);
    ZeroMem((VOID*)&Config2p13p0, sizeof(ConfigSize2p13p0));  //Clear data
    Status = gEfiUfsDeviceConfigProtocol->RwUfsDescriptor (gEfiUfsDeviceConfigProtocol, TRUE, UfsConfigDesc, 0, 0, (UINT8*)&Config2p13p0, &ConfigSize2p13p0);
    if (EFI_ERROR (Status)) {
      Print (L"[UFS Provisioning Tool - ERROR] Configuration descriptor read failed Status : %r\n", Status);
      return Status;
    }

    Print (L"[UFS Provisioning Tool ] Read Configuration descriptor status : %r ConfigSize2p13p0=0x%x OriginalSize=0x%x\n", Status, ConfigSize2p13p0, sizeof(UFS_CONFIG_DESC_2P1_3P0));
    ConvertConfigDescriptor2p1And3p0Endianness(&Config2p13p0);
    PrintConfigurationDescriptor2p1And3p0(&Config2p13p0);
    // Length
    Config2p13p0.Header.bLength = sizeof(UFS_CONFIG_DESC_2P1_3P0);
    //Config.Header.
    Config2p13p0.Header.bDescriptorIDN = UfsConfigDesc;
    Config2p13p0.Header.bConfDescContinue = 0x00;
    Config2p13p0.Header.bBootEnable = 0x01;
    Config2p13p0.Header.bDescrAccessEn = 0x01;
    Config2p13p0.Header.bInitPowerMode = 0x01;
    Config2p13p0.Header.bHighPriorityLUN = 0x7F;
    Config2p13p0.Header.bSecureRemovalType = 0x00;
    Config2p13p0.Header.bInitActiveICCLevel = 0x0F;
    Config2p13p0.Header.wPeriodicRTCUpdate = 0x00;

    if (gVersion == Ufs_3_0) {
      // No RPMB Regions
      Config2p13p0.Header.bRPMBRegionEnable = 0x00;
      Config2p13p0.Header.bRPMBRegion1Size = 0x00;
      Config2p13p0.Header.bRPMBRegion2Size = 0x00;
      Config2p13p0.Header.bRPMBRegion3Size = 0x00;
    }

    for (Index = (UFS_MAX_LUNS-1); Index >= 0 ; Index--) {
      if ((gUfsProvisioningConfiguration.LuConfParams[Index].Enable == FALSE) ||
          ((gUfsProvisioningConfiguration.LuConfParams[Index].Size == 0) && (Index != 0))) {
        ZeroMem((VOID*)& Config2p13p0.UnitDescConfParams[Index], sizeof(UFS_UNIT_DESC_CONFIG_PARAMS_2P1_3P0));  //Clear this unit descriptor data
        continue; //Skip this LU
      }
      Config2p13p0.UnitDescConfParams[Index].bLUEnable = 0x01;
      if ((Index == 1) || (Index == 2)) {
        Config2p13p0.UnitDescConfParams[Index].bBootLunID = Index;
      } else {
        Config2p13p0.UnitDescConfParams[Index].bBootLunID = 0x00;
      }
      Config2p13p0.UnitDescConfParams[Index].bLUWriteProtect = gUfsProvisioningConfiguration.LuConfParams[Index].WriteProtect;
      if (Index != 0) {
        if (gUfsProvisioningConfiguration.LuConfParams[Index].MemoryType == UfsMemoryTypeEnhanced1) {
          Config2p13p0.UnitDescConfParams[Index].bMemoryType = UfsMemoryTypeEnhanced1;
          Config2p13p0.UnitDescConfParams[Index].dNumAllocUnits = (gUfsProvisioningConfiguration.LuConfParams[Index].Size /(GeometryDeviceDescriptor.dSegmentSize * 512 * GeometryDeviceDescriptor.bAllocationUnitSize)) & ((UINT32)~(0));
          Config2p13p0.UnitDescConfParams[Index].dNumAllocUnits *= ((GeometryDeviceDescriptor.wEnhanced1CapAdjFac/0x100));
        } else {
          Config2p13p0.UnitDescConfParams[Index].bMemoryType = UfsMemoryTypeNormal;
          Config2p13p0.UnitDescConfParams[Index].dNumAllocUnits = (gUfsProvisioningConfiguration.LuConfParams[Index].Size /(GeometryDeviceDescriptor.dSegmentSize * 512 * GeometryDeviceDescriptor.bAllocationUnitSize)) & ((UINT32)~(0));
        }
        if ((gAvailableAllocUnits - Config2p13p0.UnitDescConfParams[Index].dNumAllocUnits) > 0) {
          gAvailableAllocUnits -= Config2p13p0.UnitDescConfParams[Index].dNumAllocUnits;
        } else {
          Print (L"[UFS Provisioning Tool - ERROR] Storage not enough for LU[%d]: gAvailableAllocUnits=0x%x AllocUnitsRequired 0x%x\n", Index, gAvailableAllocUnits, Config2p13p0.UnitDescConfParams[Index].dNumAllocUnits);
          return EFI_UNSUPPORTED;
        }
      } else {
        Config2p13p0.UnitDescConfParams[Index].bMemoryType = gUfsProvisioningConfiguration.LuConfParams[Index].MemoryType;
        Config2p13p0.UnitDescConfParams[Index].dNumAllocUnits = gAvailableAllocUnits & ((UINT32)~(0));
      }

      Config2p13p0.UnitDescConfParams[Index].bDataReliability = 0x1;
      Config2p13p0.UnitDescConfParams[Index].bLogicalBlockSize = 0xc;
      Config2p13p0.UnitDescConfParams[Index].bProvisioningType = 0x2;
      Config2p13p0.UnitDescConfParams[Index].wContextCapabilities = 0x0;
    }
    Print (L"[UFS Provisioning Tool] Printing Before Configuration descriptor write \n");
    ConvertConfigDescriptor2p1And3p0Endianness(&Config2p13p0);
    PrintConfigurationDescriptor2p1And3p0 (&Config2p13p0);
    Print (L"-----------------------------------------------------------------------\n");

    ConfigSize2p13p0 = sizeof (UFS_CONFIG_DESC_2P1_3P0);
    Status = gEfiUfsDeviceConfigProtocol->RwUfsDescriptor (gEfiUfsDeviceConfigProtocol, FALSE, UfsConfigDesc, 0, 0, (UINT8*)&Config2p13p0, &ConfigSize2p13p0);
    MicroSecondDelay (CONFIG_DESC_WRITE_DELAY);  //100ms wait after cofiguration descriptor write to make sure buffer was flushed.
    if (EFI_ERROR (Status)) {
      Print (L"[UFS Provisioning Tool - ERROR] Write Configuration descriptor failed with error status : %r\n", Status);
      return Status;
    }
    Print (L"[UFS Provisioning Tool ] Write Configuration descriptor status : %r ConfigSize2p13p0=0x%x OriginalSize=0x%x\n", Status, ConfigSize2p13p0, sizeof (UFS_CONFIG_DESC_2P1_3P0));
    ConfigSize2p13p0 = sizeof(UFS_CONFIG_DESC_2P1_3P0);
    ZeroMem((VOID*)&Config2p13p0, ConfigSize2p13p0);  //Clear data
    Status = gEfiUfsDeviceConfigProtocol->RwUfsDescriptor (gEfiUfsDeviceConfigProtocol, TRUE, UfsConfigDesc, 0, 0, (UINT8*)&Config2p13p0, &ConfigSize2p13p0);
    if (EFI_ERROR (Status)) {
      Print (L"[UFS Provisioning Tool - ERROR] Read Configuration descriptor failed with error status : %r\n", Status);
      return Status;
    }
    Print (L"[UFS Provisioning Tool ] Read Configuration descriptor status : %r ConfigSize2p13p0=0x%x OriginalSize=0x%x\n", Status, ConfigSize2p13p0, sizeof (UFS_CONFIG_DESC_2P1_3P0));
    ConvertConfigDescriptor2p1And3p0Endianness (&Config2p13p0);
    PrintConfigurationDescriptor2p1And3p0 (&Config2p13p0);
  }

  // Set bBootLunEn
  Attributes = UfsBootLuAEnabled;
  Status = gEfiUfsDeviceConfigProtocol->RwUfsAttribute (gEfiUfsDeviceConfigProtocol, FALSE, UfsAttrBootLunEn, 0, 0, (UINT8 *)&Attributes, &AttributesSize);
  if (EFI_ERROR (Status)) {
    Print (L"[UFS Provisioning Tool - ERROR] Setting bBootLunEn Attribute failed Status : %r\n", Status);
    return Status;
  }
  Print (L"[UFS Provisioning Tool] Enabled boot from Boot LU A Status : %r\n", Status);

  if (gConfigurationLock == TRUE) {
    Print (L"[UFS Provisioning Tool] Locking configuration \n");
    Attributes = UfsConfigDescrLocked;
    Status = gEfiUfsDeviceConfigProtocol->RwUfsAttribute (gEfiUfsDeviceConfigProtocol, FALSE, UfsAttrConfigDescLock, 0, 0, (UINT8 *)&Attributes, &AttributesSize);
    if (EFI_ERROR (Status)) {
      Print (L"[UFS Provisioning Tool - ERROR] Write Attribute for Lock failed %r\n", Status);
      return Status;
    } else {
      Print (L"[UFS Provisioning Tool] Lock configuration success \n");
    }
  }
  Print (L"UFS Provisioning Tool Exit \n");
  return (0);
}