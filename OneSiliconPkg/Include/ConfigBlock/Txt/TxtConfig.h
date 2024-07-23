/** @file
  TXT PreMemory Config Block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _TXT_CONFIG_H_
#define _TXT_CONFIG_H_

#define TXT_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gTxtPreMemConfigGuid;

#pragma pack (push,1)

/**
  CPU TXT PreMemory Configuration Structure.
  @note <b>Optional.</b> These policies will be ignored if CPU_SECURITY_PREMEM_CONFIG -> Txt is disabled, or PeiTxtLibNull is used.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT32                SinitMemorySize;          ///< Size of SINIT module if installed in flash part. Zero otherwise.
  UINT32                TxtHeapMemorySize;        ///< Size of memory reserved for TXT Heap. This memory is used by MLE.
  UINT32                TxtDprMemorySize;         ///< Size of DPR protected memory reserved for Intel TXT component.
  EFI_PHYSICAL_ADDRESS  TxtDprMemoryBase;         ///< Base address of DPR protected memory reserved for Intel TXT component.
  UINT64                TxtLcpPdSize;             ///< Size of Platform Default Launch Control Policy data if installed in flash part. Zero otherwise.
  EFI_PHYSICAL_ADDRESS  TxtLcpPdBase;             ///< Base address of Platform Default Launch Control Policy data if installed in flash part. Zero otherwise.
  EFI_PHYSICAL_ADDRESS  ApStartupBase;            ///< Base address of TXT AP Startup code
  EFI_PHYSICAL_ADDRESS  BiosAcmBase;              ///< Base address of BIOS ACM in flash part
  UINT32                BiosAcmSize;              ///< Size of ACM Binary.
  UINT32                TgaSize;                  ///< Size of Trusted Graphics Aperture if supported by chipset.
  UINT32                IsTPMPresence;
  UINT8                 TxtImplemented;           ///< This field currently is used to tell MRC if it should run after TXT initializatoin completed: <b>0=Run without waiting for TXT</b>, 1=Run after TXT initialization by callback
  UINT8                 Reserved[3];              ///< Reserved bytes
} TXT_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _TXT_CONFIG_H_
