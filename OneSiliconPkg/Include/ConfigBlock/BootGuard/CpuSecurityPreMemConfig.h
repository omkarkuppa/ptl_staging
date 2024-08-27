/** @file
  CPU Security PreMemory Config Block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef _CPU_SECURITY_PREMEM_CONFIG_H_
#define _CPU_SECURITY_PREMEM_CONFIG_H_

#define CPU_SECURITY_PREMEM_CONFIG_REVISION 2

extern EFI_GUID gCpuSecurityPreMemConfigGuid;

#pragma pack (push,1)

/**
  CPU Security PreMemory Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add TmeExcludeBase, TmeExcludeSize, GenerateNewTmeKey.
**/
typedef struct {
  CONFIG_BLOCK_HEADER Header;                      ///< Config Block Header
  UINT16              BiosSize;                    ///< Flash information for BIOS Guard: BIOS Size in KB.
  UINT8               Reserved[2];                 ///< Reserved for future use
/**
   Enable or Disable BIOS Guard; 0: Disable; <b>1: Enable</b>.
    - This is an optional feature and can be opted out.
    - If this policy is set to Disabled, the policies in the BIOS_GUARD_CONFIG will be ignored.
    - If PeiBiosGuardLibNull is used, this policy will have no effect.
**/
  UINT32              BiosGuard               :  1;
  UINT32              BiosGuardToolsInterface :  1; ///< BIOS Guard Tools Interface; <b>0: Disable</b>, 1:Enable
/**
   Enable or Disable Trusted Execution Technology; <b>0: Disable</b>; 1: Enable.
    - This is an optional feature and can be opted out.
    - If this policy is set to Disabled, the policies in the CPU_TXT_PREMEM_CONFIG will be ignored.
    - If PeiTxtLibNull is used, this policy will have no effect.
**/
  UINT32              Txt                     :  1;
  UINT32              SkipStopPbet            :  1; ///< <b>(Test)</b> Skip Stop PBET Timer; <b>0: Disable</b>; 1: Enable.
  UINT32              ResetAux                :  1; ///< <b>(Test)</b> Reset Auxiliary content, <b>0: Disabled</b>, 1: Enabled
  UINT32              TxtAcheckRequest        :  1; ///< <b>(Test)</b> AcheckRequest <b>0: Disabled</b>, 1: Enabled. When Enabled, it will call Acheck regardless of crashcode value

  /**
     Enable or Disable Total Storage Encryption (TSE) feature.
     - <b>0: Disable</b>
     -    1: Enable
  **/
  UINT32              TseEnable               : 1;
  /**
  Enable or Disable Trust Domain Extension (TDX)
    -    0: Disable (Default)
    -    1: Enable
  **/
  UINT32              TdxEnable               : 1;
  /**
     Generate new TME key regardless of the type of reset.
     - <b>0: do not generate</b>
     -    1: generate
  **/
  UINT32              GenerateNewTmeKey       : 1;
  UINT32              RsvdBits                : 23;

  //
  // There are 36 bytes in this structure above this line.
  // The TmeExcludeBase field requires an 8-byte alignment
  // therefore reserve 4 bytes here.
  //
  UINT8               Reserved2[3];
  UINT64              TmeExcludeBase;               ///< TME exclude base
  UINT64              TmeExcludeSize;               ///< TME exclude size

  UINTN              TdxActmModuleAddr;
  UINT32             TdxActmModuleSize;

  UINT8              TdxSeamldrSvn;
} CPU_SECURITY_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _CPU_SECURITY_CONFIG_H_
