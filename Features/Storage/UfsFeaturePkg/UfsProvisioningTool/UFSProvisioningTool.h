/** @file
  UFS Provisioning Application header file.

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

#ifndef __UFS_PROVISIONING_TOOL_H__
#define __UFS_PROVISIONING_TOOL_H__

#define UFS_MAX_LUNS                        0x8
#define UFS_2_1_3_0_DEVICE_DESC_SIZE        0x40
#define UFS_2_1_3_0_GEOMETRY_DESC_SIZE      0x48
#define CONFIG_DESC_WRITE_DELAY             100000

#pragma pack(1)
//
// UFS 3.1 Spec Section 14.1.4.2 - Device Descriptor
//
typedef struct {
  UINT8     bLength;
  UINT8     bDescriptorIDN;
  UINT8     bDevice;
  UINT8     bDeviceClass;
  UINT8     bDeviceSubClass;
  UINT8     bProtocol;
  UINT8     bNumberLU;
  UINT8     bNumberWLU;
  UINT8     bBootEnable;
  UINT8     bDescrAccessEn;
  UINT8     bInitPowerMode;
  UINT8     bHighPriorityLUN;
  UINT8     bSecureRemovalType;
  UINT8     bSecurityLU;
  UINT8     bBackgroundOpsTermLat;
  UINT8     bInitActiveICCLevel;
  UINT16    wSpecVersion;
  UINT16    wManufactureDate;
  UINT8     iManufacturerName;
  UINT8     iProductName;
  UINT8     iSerialNumber;
  UINT8     iOemID;
  UINT16    wManufacturerID;
  UINT8     bUD0BaseOffset;
  UINT8     bUDConfigPLength;
  UINT8     bDeviceRTTCap;
  UINT16    wPeriodicRTCUpdate;
  UINT8     bUFSFeaturesSupport;
  UINT8     bFFUTimeout;
  UINT8     bQueueDepth;
  UINT16    wDeviceVersion;
  UINT8     bNumSecureWPArea;
  UINT32    dPSAMaxDataSize;
  UINT8     bPSAStateTimeout;
  UINT8     iProductRevisionLevel;
  UINT8     Rsvd1[5];
  UINT8     Rsvd2[16];
  UINT8     Rsvd3[3];
  UINT8     Rsvd4[12];
  UINT32    dExtendedUFSFeaturesSupport;
  UINT8     bWriteBoosterBufferPreserveUserSpaceEn;
  UINT8     bWriteBoosterBufferType;
  UINT32    dNumSharedWriteBoosterBufferAllocUnits;
} UFS_DEV_DESC;

//
// UFS 3.1 Spec Section 14.1.4.4 - Geometry Descriptor
//
typedef struct {
  UINT8     bLength;
  UINT8     bDescriptorIDN;
  UINT8     bMediaTechnology;
  UINT8     Reserved;
  UINT64    qTotalRawDeviceCapacity;
  UINT8     bMaxNumberLU;
  UINT32    dSegmentSize;
  UINT8     bAllocationUnitSize;
  UINT8     bMinAddrBlockSize;
  UINT8     bOptimalReadBlockSize;
  UINT8     bOptimalWriteBlockSize;
  UINT8     bMaxInBufferSize;
  UINT8     bMaxOutBufferSize;
  UINT8     bRPMB_ReadWriteSize;
  UINT8     bDynamicCapacityResourcePolicy;
  UINT8     bDataOrdering;
  UINT8     bMaxContexIDNumber;
  UINT8     bSysDataTagUnitSize;
  UINT8     bSysDataTagResSize;
  UINT8     bSupportedSecRTypes;
  UINT16    wSupportedMemoryTypes;
  UINT32    dSystemCodeMaxNAllocU;
  UINT16    wSystemCodeCapAdjFac;
  UINT32    dNonPersistMaxNAllocU;
  UINT16    wNonPersistCapAdjFac;
  UINT32    dEnhanced1MaxNAllocU;
  UINT16    wEnhanced1CapAdjFac;
  UINT32    dEnhanced2MaxNAllocU;
  UINT16    wEnhanced2CapAdjFac;
  UINT32    dEnhanced3MaxNAllocU;
  UINT16    wEnhanced3CapAdjFac;
  UINT32    dEnhanced4MaxNAllocU;
  UINT16    wEnhanced4CapAdjFac;
  UINT32    dOptimalLogicalBlockSize;
  UINT8     Res[7];
  UINT32    dWriteBoosterBufferMaxNAllocUnits;
  UINT8     bDeviceMaxWriteBoosterLUs;
  UINT8     bWriteBoosterBufferCapAdjFac;
  UINT8     bSupportedWriteBoosterBufferUserSpaceReductionTypes;
  UINT8     bSupportedWriteBoosterBufferTypes;
} UFS_GEOMETRY_DESC;

//
// UFS 2.1 Spec Section 14.1.4.3 - Configuration Descriptor
//
typedef struct {
  UINT8     bLength;
  UINT8     bDescriptorIDN;
  UINT8     bConfDescContinue;
  UINT8     bBootEnable;
  UINT8     bDescrAccessEn;
  UINT8     bInitPowerMode;
  UINT8     bHighPriorityLUN;
  UINT8     bSecureRemovalType;
  UINT8     bInitActiveICCLevel;
  UINT16    wPeriodicRTCUpdate;
  UINT8     Rsvd;
  UINT8     bRPMBRegionEnable;
  UINT8     bRPMBRegion1Size;
  UINT8     bRPMBRegion2Size;
  UINT8     bRPMBRegion3Size;
} UFS_CONFIG_DESC_GEN_HEADER_2P1_3P0;

typedef struct {
  UINT8     bLUEnable;
  UINT8     bBootLunID;
  UINT8     bLUWriteProtect;
  UINT8     bMemoryType;
  UINT32    dNumAllocUnits;
  UINT8     bDataReliability;
  UINT8     bLogicalBlockSize;
  UINT8     bProvisioningType;
  UINT16    wContextCapabilities;
  UINT8     Rsvd1[3];
} UFS_UNIT_DESC_CONFIG_PARAMS_2P1_3P0;

//
// UFS 2.1 Spec Section 14.1.4.3 - Configuration Descriptor
//
typedef struct {
  UFS_CONFIG_DESC_GEN_HEADER_2P1_3P0     Header;
  UFS_UNIT_DESC_CONFIG_PARAMS_2P1_3P0    UnitDescConfParams[UFS_MAX_LUNS];
} UFS_CONFIG_DESC_2P1_3P0;

//
// UFS 3.1 Spec Section 14.1.4.3 - Configuration Descriptor
//
typedef struct {
  UINT8     bLength;
  UINT8     bDescriptorIDN;
  UINT8     bConfDescContinue;
  UINT8     bBootEnable;
  UINT8     bDescrAccessEn;
  UINT8     bInitPowerMode;
  UINT8     bHighPriorityLUN;
  UINT8     bSecureRemovalType;
  UINT8     bInitActiveICCLevel;
  UINT16    wPeriodicRTCUpdate;
  UINT8     Rsvd1;
  UINT8     bRPMBRegionEnable;
  UINT8     bRPMBRegion1Size;
  UINT8     bRPMBRegion2Size;
  UINT8     bRPMBRegion3Size;
  UINT8     bWriteBoosterBufferPreserveUserSpaceEn;
  UINT8     bWriteBoosterBufferType;
  UINT32    dNumSharedWriteBoosterBufferAllocUnits;
} UFS_CONFIG_DESC_GEN_HEADER;

typedef struct {
  UINT8     bLUEnable;
  UINT8     bBootLunID;
  UINT8     bLUWriteProtect;
  UINT8     bMemoryType;
  UINT32    dNumAllocUnits;
  UINT8     bDataReliability;
  UINT8     bLogicalBlockSize;
  UINT8     bProvisioningType;
  UINT16    wContextCapabilities;
  UINT8     Rsvd1[3];
  UINT8     Rsvd2[6];
  UINT32    dLUNumWriteBoosterBufferAllocUnits;
} UFS_UNIT_DESC_CONFIG_PARAMS;

//
// UFS 3.1 Spec Section 14.1.4.3 - Configuration Descriptor
//
typedef struct {
  UFS_CONFIG_DESC_GEN_HEADER     Header;
  UFS_UNIT_DESC_CONFIG_PARAMS    UnitDescConfParams[UFS_MAX_LUNS];
} UFS_CONFIG_DESC;

typedef enum {
  Ufs_2_1  = 0x0210,
  Ufs_2_2  = 0x0220,
  Ufs_3_0  = 0x0300,
  Ufs_3_1  = 0x0310,
  Ufs_4_0  = 0x0400,
  Ufs_Max  = 0xFFFF
} UFS_SPEC_VERSION;

//
// Section 14.3 - Attributes
//
typedef enum {
  UfsAttrBootLunEn        = 0x00,
  UfsAttrCurPowerMode     = 0x02,
  UfsAttrActiveIccLevel   = 0x03,
  UfsAttrOutOfOrderDataEn = 0x04,
  UfsAttrBgOpStatus       = 0x05,
  UfsAttrPurgeStatus      = 0x06,
  UfsAttrMaxDataInSize    = 0x07,
  UfsAttrMaxDataOutSize   = 0x08,
  UfsAttrDynCapNeeded     = 0x09,
  UfsAttrRefClkFreq       = 0x0a,
  UfsAttrConfigDescLock   = 0x0b,
  UfsAttrMaxNumOfRtt      = 0x0c,
  UfsAttrExceptionEvtCtrl = 0x0d,
  UfsAttrExceptionEvtSts  = 0x0e,
  UfsAttrSecondsPassed    = 0x0f,
  UfsAttrContextConf      = 0x10,
  UfsAttrCorrPrgBlkNum    = 0x11
} UFS_ATTR_IDN;

//
// UFS Descriptors
//
typedef enum {
  UfsDeviceDesc    = 0x00,
  UfsConfigDesc    = 0x01,
  UfsUnitDesc      = 0x02,
  UfsInterConnDesc = 0x04,
  UfsStringDesc    = 0x05,
  UfsGeometryDesc  = 0x07,
  UfsPowerDesc     = 0x08
} UFS_DESC_IDN;

typedef enum {
  UfsConfigDescrNotLocked = 0x0,
  UfsConfigDescrLocked    = 0x1,
  UfsConfigDescrReserved  = 0xFF
} UFS_CONFIG_DESC_LOCK_ATTRIBUTE;

typedef enum {
  UfsBootDisabled      = 0x0,
  UfsBootLuAEnabled    = 0x1,
  UfsBootLuBEnabled    = 0x2,
  UfsBootLuReserved    = 0xFF
} UFS_BOOT_LUN_EN_ATTRIBUTE;

typedef enum {
  UfsMemoryTypeNormal    = 0x0,
  UfsMemoryTypeEnhanced1 = 0x3,
  UfsMemoryTypeReserved  = 0xFF
} UFS_LU_MEMORY_TYPE;

typedef struct {
  UINT8     NumberOfLUs;                   //Number of LUs to be enabled.
  UINT64    SharedWriteBoosterBufferSize;  //Shared Write Booster Buffer Size in MB/GB, And is available only in UFS spec 2.2, 3.1 and 4.0
} UFS_DEVICE_CONFIGURABLE_PARAMS;

typedef struct {
  BOOLEAN             Enable;              // Number of enabled LUs Should match to NumberOfLUs
  UINT8               WriteProtect;
  UFS_LU_MEMORY_TYPE  MemoryType;          //Check the supported Memory Types of the Tool and Ufs Specification
  UINT64              Size;                //Size in MB/GB, Should be multiples of (bAllocationUnitSize * dSegmentSize) of the Card.
} UFS_LU_CONFIGURABLE_PARAMS;

typedef struct {
  UFS_DEVICE_CONFIGURABLE_PARAMS     DevConfParams;
  UFS_LU_CONFIGURABLE_PARAMS         LuConfParams[UFS_MAX_LUNS];
} UFS_PROVISIONING_CONFIGURATION;

#pragma pack()

#endif
