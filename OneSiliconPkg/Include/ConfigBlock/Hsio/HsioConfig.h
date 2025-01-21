/** @file
  HSIO policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _HSIO_CONFIG_H_
#define _HSIO_CONFIG_H_

#define HSIO_CONFIG_REVISION 1

extern EFI_GUID gHsioConfigGuid;

#pragma pack (push,1)

/**
  The HSIO_CONFIG block provides HSIO message related settings.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;   ///< Config Block Header
  VOID      *ChipsetInitBinPtr;   ///< Policy used to point to the Base (+ OEM) ChipsetInit binary used to sync between BIOS and CSME
  UINT32    ChipsetInitBinLen;    ///< Policy used to indicate the size of the Base (+ OEM) ChipsetInit binary used to sync between BIOS and CSME
  VOID      *NphyBinPtr;          ///< Policy used to point to the OEM Nphy binary used to sync between BIOS and CSME
  UINT32    NphyBinLen;           ///< Policy used to indicate the size of the OEM Nphy binary used to sync between BIOS and CSME
  VOID      *SynpsPhyBinPtr;      ///< Policy used to point to the OEM Synophsys Phy binary used to sync between BIOS and CSME
  UINT32    SynpsPhyBinLen;       ///< Policy used to indicate the size of the OEM Synophsys Phy binary used to sync between BIOS and CSME
} HSIO_CONFIG;

#pragma pack (pop)

#endif // _HSIO_CONFIG_H_
