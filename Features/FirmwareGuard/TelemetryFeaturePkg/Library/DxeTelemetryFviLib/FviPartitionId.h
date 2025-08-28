/** @file
  FVI Partition ID definitions.

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
**/

#ifndef _FVI_PARTITION_ID_H_
#define _FVI_PARTITION_ID_H_

//
// CSE Firmware Partition Table (FPT) Partition IDs
// These definitions are based on the CSE Layout and FW update specification
//
#define FPT_PARTITION_NAME_PMCP         0x50434D50    // PMC - Power Management Controller Partition
#define FPT_PARTITION_NAME_RBEP         0x50454252    // RBEP - Recovery Boot Engine Partition
#define FPT_PARTITION_NAME_OEMP         0x504D454F    // OEMP - OEM Partition
#define FPT_PARTITION_NAME_PCHC         0x43484350    // PCHC - PCH Controller
#define FPT_PARTITION_NAME_PCOD         0x444F4350    // PCOD - P-Unit Code
#define FPT_PARTITION_NAME_ACOD         0x444F4341    // ACOD - Audio Code
#define FPT_PARTITION_NAME_NPHY         0x5948504E    // NPHY - Network PHY
#define FPT_PARTITION_NAME_SPHY         0x59485053    // SPHY - SATA PHY
#define FPT_PARTITION_NAME_CNVI         0x49564E43    // CNVi - Connectivity (WiFi)
#define FPT_PARTITION_NAME_NFTP         0x5054464E    // NFTP - Network File Transfer Protocol
#define FPT_PARTITION_NAME_ISHC         0x43485349    // ISHC - Integrated Sensor Hub Controller
#define FPT_PARTITION_NAME_WCOD         0x444F4357    // WCOD - WiFi Code
#define FPT_PARTITION_NAME_LOCL         0x4C434F4C    // LOCL - Local partition
#define FPT_PARTITION_NAME_ADSP         0x50534441    // ADSP - Audio DSP for wireless (WLAN)
#define FPT_PARTITION_NAME_IUNP         0x504E5549    // IUNP - Intel Unified Network Protocol
#define FPT_PARTITION_NAME_EFWP         0x50574645    // EFWP - ME SSE Firmware Partition

//
// Structure to map partition IDs to their component GUIDs
//
typedef struct {
  UINT32    PartitionId;
  EFI_GUID  *ComponentGuid;
} FVI_PARTITION_MAPPING;

//
// Partition ID to Component GUID mapping table
//
#define FVI_PARTITION_MAPPING_TABLE { \
  { FPT_PARTITION_NAME_PMCP, &gPmcPartitionComponentId },   \
  { FPT_PARTITION_NAME_RBEP, &gRbepPartitionComponentId },  \
  { FPT_PARTITION_NAME_OEMP, &gOempPartitionComponentId },  \
  { FPT_PARTITION_NAME_PCHC, &gPchcPartitionComponentId },  \
  { FPT_PARTITION_NAME_PCOD, &gPcodPartitionComponentId },  \
  { FPT_PARTITION_NAME_ACOD, &gAcodPartitionComponentId },  \
  { FPT_PARTITION_NAME_NPHY, &gNphyPartitionComponentId },  \
  { FPT_PARTITION_NAME_SPHY, &gSphyPartitionComponentId },  \
  { FPT_PARTITION_NAME_CNVI, &gCnviPartitionComponentId },  \
  { FPT_PARTITION_NAME_NFTP, &gNftpPartitionComponentId },  \
  { FPT_PARTITION_NAME_ISHC, &gIshcPartitionComponentId },  \
  { FPT_PARTITION_NAME_WCOD, &gWcodPartitionComponentId },  \
  { FPT_PARTITION_NAME_LOCL, &gLoclPartitionComponentId },  \
  { FPT_PARTITION_NAME_ADSP, &gAdspPartitionComponentId },  \
  { FPT_PARTITION_NAME_IUNP, &gIunpPartitionComponentId },  \
  { FPT_PARTITION_NAME_EFWP, &gMeSseVersionComponentId }    \
}

//
// Number of entries in the partition mapping table
//
#define FVI_PARTITION_MAPPING_COUNT (sizeof((FVI_PARTITION_MAPPING[])FVI_PARTITION_MAPPING_TABLE) / sizeof(FVI_PARTITION_MAPPING))

#endif // _FVI_PARTITION_ID_H_
