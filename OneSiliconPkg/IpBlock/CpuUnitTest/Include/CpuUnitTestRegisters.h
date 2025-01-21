/** @file

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

#ifndef CPU_UNIT_TEST_REGISTERS_H_
#define CPU_UNIT_TEST_REGISTERS_H_

#define MAILBOX_TYPE_PCODE                    0x00000001
#define MAILBOX_TYPE_OC                       0x00000002
typedef struct {
  UINT64    Value;
  UINTN     WrittenCount;
} MSR_ENTRY;

typedef struct {
  UINT32    Leaf;
  UINT32    SubLeaf;
  UINT32    Eax;
  UINT32    Ebx;
  UINT32    Ecx;
  UINT32    Edx;
} CPU_ID_ENTRY;

//
// The MSR[Index] BITs (set in ReadonlyBitMask) is readonly
//   if MSR[LockIndex] & LockBitMask == LockBitValue
// Note: When LockBitMask = LockBitValue = 0, if-condition is TRUE always. Then MSR[Index] is readonly always.
//
typedef struct {
  UINT32    Index;
  UINT64    ReadonlyBitMask; // 0xFFFFFFFFFFFFFFFF protects all 64bits, 0xF0 protects BIT[7:4]
  UINT32    LockIndex;       // if -1, readonly always, otherwise readonly when MSR[LockIndex] is set.
  UINT64    LockBitMask;
  UINT64    LockBitValue;
} CPU_READ_ONLY_MSR_ENTRY;

typedef struct {
  UINT32    MailboxType;   // PCODE or OC mailbox
  UINT32    ReadCommand;   // -1 indicates no associated read command
  UINT32    WriteCommand;  // -1 indicates no associated write command
  UINT32    InputData;     // Only valid when using ReadMailbox2. Otherwise, it is ignored
  UINT32    Setting;       // A PCODE_SETTING_ENTRY entry could only provide the mapping of read/write command without specifying the default setting.
  UINTN     WrittenCount;  // 0 indicates Setting is not initialized by CpuUnitTest setup code or production code.
} PCODE_SETTING_ENTRY;

typedef struct {
  UINT64    Address;
  UINT8     Data;
} MMIO_ENTRY;

typedef struct {
  CPU_ID_ENTRY               *CpuId;
  MSR_ENTRY                  *Msr[2];        // 0: 0 ~ 0x1000, 1: 0x80000000 ~ 0x80001000
  //
  // TRUE indicating that readonly MSRs cannot be modified.
  // It's set to TRUE after the CPU registers are settle down for testing.
  //
  BOOLEAN                    ProtectReadonlyMsr;
  CPU_READ_ONLY_MSR_ENTRY    *ReadonlyMsr;

  PCODE_SETTING_ENTRY        *PcodeSetting;
  UINTN                      PcodeSettingCount;
  UINTN                      PcodeSettingCapability;

  MMIO_ENTRY                 *Mmio;
  UINTN                      MmioCount;
  UINTN                      MmioCapability;
} CPU_UNIT_TEST_REGISTERS;

#endif
