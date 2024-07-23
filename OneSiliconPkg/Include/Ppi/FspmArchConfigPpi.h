/** @file
  Header file for FSP-M Arch Config PPI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _FSPM_ARCH_CONFIG_PPI_H_
#define _FSPM_ARCH_CONFIG_PPI_H_

///
/// Global ID for the FSPM_ARCH_CONFIG_PPI.
///
#define FSPM_ARCH_CONFIG_GUID \
  { \
    0x824d5a3a, 0xaf92, 0x4c0c, { 0x9f, 0x19, 0x19, 0x52, 0x6d, 0xca, 0x4a, 0xbb } \
  }

///
/// This PPI provides FSP-M Arch Config PPI.
///
typedef struct {
  UINT8         Revision;
  UINT8         Reserved[3];
  VOID          *NvsBufferPtr;
  UINT32        BootLoaderTolumSize;
  UINT8         Reserved1[4];
} FSPM_ARCH_CONFIG_PPI;

extern EFI_GUID gFspmArchConfigPpiGuid;

#endif // _FSPM_ARCH_CONFIG_PPI_H_
