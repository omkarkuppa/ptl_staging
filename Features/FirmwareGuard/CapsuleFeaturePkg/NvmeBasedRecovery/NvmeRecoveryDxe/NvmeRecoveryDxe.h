/**@file
  NvmeRecovery Dxe driver, main entry of the Nvme recovery feature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _NVME_RECOVERY_H_
#define _NVME_RECOVERY_H_

#include <Uefi.h>
#include <IndustryStandard/Pci.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "NvmeDevice.h"
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PchPcieRpLib.h>
#include <Protocol/Spi.h>
#include <Guid/SysFwUpdateProgress.h>
#include <Library/VmdInfoLib.h>
#include <Protocol/RstConfigSpaceAccess.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/SeamlessRecoverySupportLib.h>
#include <Library/HobLib.h>
#include <BupMsgs.h>
#include <Protocol/PciIo.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Register/PchRegs.h>
#include <Library/PlatformBootStateHookLib.h>
#include <Library/PlatformWdtLib.h>
#include <Library/ResiliencySupportLib.h>
#include <Guid/NvmeRecoveryCommon.h>
#include <Library/MeInfoLib.h>
#include <Register/HeciRegs.h>
#include <Library/MeFwStsLib.h>
#include <MeSmbios.h>

#define NVME_RECOVERY_DESCRIPTOR_HEADER_SIGNATURE     SIGNATURE_32 ('N','R','D','H')
#define NVME_RECOVERY_DESCRIPTOR_ELEMENT_SIGNATURE    SIGNATURE_32 ('R','E','C','V')
#define NVME_NORECOVERY_DESCRIPTOR_ELEMENT_SIGNATURE  SIGNATURE_32 ('N','O','N','E')

//
// NVMe Recovery componnets list
//
typedef enum {
  RECV_HEAD = 0,
  RECV_BIOS,
  RECV_CSME,
} NVME_RECOVERY_ELEMENT_INDEX;

typedef enum {
  BPPEnable    = 0,
  BPLock,
  NWPAuthCtrl
} NVME_DCBD_STRUCT;

//
// Hash algorithm support list
//
typedef enum {
  RECV_RSVD   = 0,
  RECV_SHA256 = 1,
  RECV_SHA384 = 2,
  RECV_SHA512 = 3
} NVME_SUPPORTED_HASH_TYPE;

typedef enum {
  RESTORE_ELEMENT_ACTION_NONE,
  RESTORE_ELEMENT_ACTION_COPY,
  RESTORE_ELEMENT_ACTION_DELETE,
  RESTORE_ELEMENT_ACTION_MAX
} RESTORE_ELEMENT_ACTION;

typedef struct{
  RESTORE_ELEMENT_ACTION  RestoreAction;
  UINT32                  ElementType;
  UINT32                  OffsetRecvImage;
  UINT32                  OffsetSpi;
  UINT32                  ElementSize;
} NVME_RESTORE_ELEMENT;

typedef struct{
  UINT32                Type;
  UINT32                StructVersion;
  UINT32                IpType;
  UINT32                TotalSize;
  UINT32                NumberOfElements;
  UINT32                Reserved;
  NVME_RESTORE_ELEMENT  RestoreElement;
} NVME_IMAGE_STRUCT;

typedef struct{
  UINT32                Type;
  UINT32                StructVersion;
  UINT32                IpType;
  UINT32                TotalSize;
  UINT32                NumberOfElements;
  UINT32                Reserved;
}  NVME_IMAGE_HEADER_STRUCT;

typedef struct{
  NVME_IMAGE_HEADER_STRUCT    NvmeImageHeaderStruct;
  UINT32                      HashType;   // Required 32-bit, should be one of NVME_SUPPORTED_HASH_TYPE.
  UINT8                       Hash[128];  // Hash filed is 1024-bit.
} NVME_IMAGE_HEADER;

#define NVME_RECOVERY_NUMOFIMAGES             2                           // Bios and CSME
#define NVME_RECOVERY_NUMOFELEMENTS           3                           // Bios, CSME Code and CSME Data
#define NVME_ALL_NAMESPACEID                  0XFFFFFFFF

#endif
