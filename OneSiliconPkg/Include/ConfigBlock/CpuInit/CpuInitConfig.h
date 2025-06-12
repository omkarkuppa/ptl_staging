/** @file
  CPU Initialization Config Block.

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
#ifndef _CPU_INIT_CONFIG_H_
#define _CPU_INIT_CONFIG_H_

extern EFI_GUID gCpuInitConfigGuid;

#pragma pack (push,1)

/**
  CPU Initialization Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/

#define CPU_INIT_CONFIG_REVISION 1

typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT32                MicrocodePatchRegionSize;
  EFI_PHYSICAL_ADDRESS  MicrocodePatchAddress;    ///< Pointer to microcode patch that is suitable for this processor.
  /**
    Enable or Disable Advanced Encryption Standard (AES) feature.
    For some countries, this should be disabled for legal reasons.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 AesEnable           : 1;
  /**
    Enable or Disable Trusted Execution Technology (TXT) feature.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 TxtEnable           : 1;
  /**
    Enable or Disable or Auto for PPIN Support to view Protected Processor Inventory Number.
    - <b>0: Disable</b>
    -    1: Enable
    -    2: Auto : Feature is based on End Of Manufacturing (EOM) flag. If EOM is set, it is disabled.
  **/
  UINT32 PpinSupport         : 2;
  /**
    Enable or Disable #AC machine check on split lock.
    - <b>0: Disable</b>
    -    1: Enable
  **/
  UINT32 AcSplitLock         : 1;
  /**
  Enable or Disable Avx.
  -      1: Disable
  -  <b> 0: Enable</b>
  **/
  UINT32 AvxDisable          : 1;
  /**
    Enable or Disable X2APIC Support.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 X2ApicEnable        : 1;
  /**
    Enable or Disable PMON to update memory address.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 PmonEnable          : 1;
  UINT32 RsvdBits            : 25;                ///< Reserved for future use
  UINT8  Reserved0[3];                            ///< Reserved for future use
  UINT64 PmonAddress;                             ///< PMON memory address
} CPU_INIT_CONFIG;

#pragma pack (pop)

#endif // _CPU_INIT_CONFIG_H_
