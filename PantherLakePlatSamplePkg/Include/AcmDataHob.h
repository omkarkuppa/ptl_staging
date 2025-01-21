/**@file

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

#ifndef __ACM_DATA_HOB_H__
#define __ACM_DATA_HOB_H__

#include <IndustryStandard/Tpm20.h>

#pragma pack (1)

#define ACM_NEM_MTRR_MSR_BASE_9       0x212       //MSR_IA32_MTRR_PHYSBASE9
#define ACM_NEM_MTRR_MSR_MASK_9       0x213       //MSR_IA32_MTRR_PHYSMASK9

#define ACM_NEM_IBB_DIGESTS_UUID1     0xA57AF70D
#define ACM_NEM_IBB_DIGESTS_UUID2     0x7FA0420d
#define ACM_NEM_IBB_DIGESTS_UUID3     0x93BA32C1
#define ACM_NEM_IBB_DIGESTS_UUID4     0x24243F75

#define ACM_NEM_ACM_DIGESTS_UUID1     0x6C9A90D6
#define ACM_NEM_ACM_DIGESTS_UUID2     0x649A41cc
#define ACM_NEM_ACM_DIGESTS_UUID3     0x8BD7A772
#define ACM_NEM_ACM_DIGESTS_UUID4     0x49524471

#define ACM_NEM_ACM_VER_UUID1         0xB1C553DE
#define ACM_NEM_ACM_VER_UUID2         0x4AACC6C8
#define ACM_NEM_ACM_VER_UUID3         0x9FB57D91
#define ACM_NEM_ACM_VER_UUID4         0x54FE45D3

#define ACM_NEM_ACM_BENCHMARK_UUID1   0x06A9C77B
#define ACM_NEM_ACM_BENCHMARK_UUID2   0x850B476B
#define ACM_NEM_ACM_BENCHMARK_UUID3   0xB3D0C9B4
#define ACM_NEM_ACM_BENCHMARK_UUID4   0x7BCB8A85

typedef struct _IBB_DIGEST {
  TPML_DIGEST_VALUES      DigestValues;
} IBB_DIGEST;

typedef struct _ACM_DIGEST {
  TPM2B_DIGEST            DigestValues;
} ACM_DIGEST;

typedef struct _ACM_VERSION {
  TPML_DIGEST_VALUES      DigestValues;
} ACM_VERSION;

typedef struct _RECORD {
  UINT16                  TaskNumber; // ACM Task Number
  UINT16                  Tag;        // Tagged benchmark
  UINT64                  TimeStamp;  // Time stamp in TSC clocks
} RECORD;

typedef struct _STARTUP_ACM_BENCHMARK {
  UINT32                  Count;
  RECORD                  *Record;
} STARTUP_ACM_BENCHMARK;

typedef struct _ACM_DATA_HOB {
  IBB_DIGEST              IBBDigests;          // IBB Digest
  ACM_DIGEST              ACMDigests;          // ACM Digest
  ACM_VERSION             ACMVersion;          // ACM SCRTM version
  STARTUP_ACM_BENCHMARK   StartupACMBenchmark; // Startup ACM Benchmark
} ACM_DATA_HOB;

#pragma pack ()

#endif // __ACM_DATA_HOB_H__
