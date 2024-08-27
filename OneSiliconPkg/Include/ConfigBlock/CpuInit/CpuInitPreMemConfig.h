/** @file
  CPU Initialization PreMemory Config Block.

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
#ifndef _CPU_INIT_PREMEM_CONFIG_H_
#define _CPU_INIT_PREMEM_CONFIG_H_

#define CPU_INIT_PREMEM_CONFIG_REVISION 1

extern EFI_GUID gCpuInitPreMemConfigGuid;

#pragma pack (push,1)

/**
  CPU Config Library PreMemory Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER Header;            ///< Config Block Header
  /**
  Boot Maximum Frequency
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 BootMaxFrequency           : 1;

  UINT32 BistOnReset                : 1; ///< <b>(Test)</b> Enable or Disable BIST on Reset; <b>0: Disable</b>; 1: Enable.
  /**
    Enable or Disable Virtual Machine Extensions (VMX) feature.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 VmxEnable                  : 1;
  /**
  Processor Early Power On Configuration FCLK setting.
   - <b>0: 800 MHz (ULT/ULX)</b>.
   - <b>1: 1 GHz (DT/Halo)</b>. Not supported on ULT/ULX.
   - 2: 400 MHz.
   - 3: Reserved.
  **/
  UINT32 FClkFrequency              : 2;
  /**
  Enable or Disable CrashLog feature
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 CrashLogEnable             : 1;

  /**
  Enable or Disable Total Memory Encryption (TME) feature.
    -    0: Disable
    - <b>1: Enable</b>
  **/
  UINT32 TmeEnable                  : 1;

  UINT32 DebugInterfaceEnable       : 2; ///< Enable or Disable processor debug features; 0: Disable; 1: Enable; <b>2: No Change</b>.
  UINT32 DebugInterfaceLockEnable   : 1; ///< Lock or Unlock debug interface features; 0: Disable; <b>1: Enable</b>.

  /**
  Enables a mailbox command to resolve rare PECI related Sx issues.
  @note This should only be used on systems that observe PECI Sx issues.
  - <b>0: Disable</b>
  -    1: Enable
  **/
  UINT32 PeciSxReset                : 1;

  /**
  Enable or Disable CrashLog GPRs dump
    - <b>0: Disable</b>
    -    1: Gprs Enabled, Smm Gprs Enabled
         2: Gprs Enabled, Smm Gprs Disabled
  **/
  UINT32 CrashLogGprs               : 2;

  /**
  Set OC lock bit in MSR 0x194[20] will lock the OC mailbox for OC configration settings and Turbo Ratio Limit.
    - 0: Disable
    - <b>1: Enable (Lock)</b>
  **/
  UINT32 OcLock                     : 1;
  UINT32 RsvdBit                    : 18;

  /**
    CpuRatio - Max non-turbo ratio (Flexible Ratio Boot) is set to CpuRatio. <b>0: Disabled</b> If disabled, doesn't override max-non turbo ratio.
  **/
  UINT8  CpuRatio;


  /**
    Number of big cores in processor to enable.
    -     0: Disable all cores for Hybrid CPU; Active all cores for Non-Hybrid CPU.
    -     1: 1 core
    -     2: 2 cores
    -     3: 3 cores
  **/
  UINT8 ActiveCoreCount;

  /**
    Number of small cores in processor to enable.
    -     0: Disable all cores for Hybrid CPU; Active all cores for Non-Hybrid CPU.
    -     1: 1 core
    -     2: 2 cores
    -     3: 3 cores
  **/
  UINT8 ActiveSmallCoreCount;

  /**
    Number of enabled LP Atom cores.
    Default is set to supported LP Atom core number.
    -    0      - all core disabled
    -    1      - enable 1 core
    -    2      - enable 2 cores
    -    3      - enable 3 cores
    -    4      - enable 4 cores
    -    0xFF   - enable all cores
    -    others - reserved for future use
  **/
  UINT8  ActiveLpAtomCoreCount;

  /**
    Determine whether to enable or disable DFD. DFD is enabled from cold boot.
      - <b>0   : DFD disabled</b>
      -    1   : DFD enabled</b>
  **/
  UINT8  DfdEnable;
  UINT8  Rsvd1[3];
  /**
  PRMRR Size.
    - <b>Software Control : 0x0</b>
    -                2MB : 0x200000
    -                4MB : 0x400000
    -              16 MB : 0x1000000
  **/
  UINT32 PrmrrSize;

  /**
  Enable or Disable Total Memory Encryption (TME) Bypass Mode
  If TME encryption bypass is enabled, then transactions with KeyID0 Bypass the encryption in CCE.
    -    0: Disable (Default)
    -    1: Enable
  **/
  UINT8 TmeBypassCapability;

  /**
    Size of TSEG in bytes. (Must be power of 2)
    <b>0x400000</b>: 4MB for Release build
    0x1000000      : 16MB for Debug build
  **/
  UINT32  TsegSize;

  /**
    Enable or disable SmmRelocation. For Coreboot customer it must be disabled.
      -    0: Disable
      -    1: Enable (Default)
  **/
  UINT8 SmmRelocationEnable;

  UINT8 Rsvd2[2];
} CPU_INIT_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _CPU_INIT_PREMEM_CONFIG_H_
