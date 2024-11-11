/** @file
  Header file for IGPU Includes

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

#ifndef _IP_IGPU_DEFINES_H_
#define _IP_IGPU_DEFINES_H_

#define IGPU_GTTMMADR_ALING(x) (x & 0xFFFFFFFFFF000000)
#define IGPU_GTTMMADR_ADDRESS(x) (x & 0xFFFFFFFFFFFFFFF0)

#define GCD_WOPCM_SIZE_2_MB       2
#define WOPCM_TOTAL_SIZE_4_MB     4
#define WOPCM_TOTAL_SIZE_8_MB     8

//
// GMD ID
//
typedef union {
  struct {
    UINT32 GMDRevId        : 6;  // Bits 5:0
    UINT32 Rsvd            : 8;  // Bits 13:6
    UINT32 GMDRelease      : 8;  // Bits 21:14
    UINT32 GMDArchitecture : 10; // Bits 31:22
  } Bits;
  UINT32 Data;
} GMD_ID_STRUCT;

#define GMD_ID_REVID_SIZE        0x0006
#define GMD_ID_RELEASE_SIZE      0x0008
#define GMD_ID_ARCHITECTURE_SIZE 0x000A

#define CLEAR_GMD_REVID_MASK     0xFFFFFFC0 // Mask to clear bits 0-5 (RevId) in a 32-bit integer

//
// GMD Arch IDs
//
// GMD_<IP>_Arch_Release
//
#define GMD_GT_30_0              0x07800000
#define GMD_MEDIA_30_0           0x07800000
#define GMD_GT_ARCH_35           35
#define GMD_MEDIA_ARCH_35        35

//
// GMD RevIDs
//
#define GMD_MEDIA_REV_30_0_0_A0  0x0
#define GMD_MEDIA_REV_30_0_0_B0  0x4


///
/// GT MMIO Registers for MemSS Global Configuration
///
#define MEM_SS_INFO_GLOBAL_DDRTYPE_DDR4   0
#define MEM_SS_INFO_GLOBAL_DDRTYPE_DDR5   1
#define MEM_SS_INFO_GLOBAL_DDRTYPE_LPDDR5 2
#define MEM_SS_INFO_GLOBAL_DDRTYPE_LPDDR4 3
#define MEM_SS_INFO_GLOBAL_DDRTYPE_DDR3   4
#define MEM_SS_INFO_GLOBAL_DDRTYPE_LPDDR3 5


#ifndef FORCE_DISABLE
#define FORCE_DISABLE 2
#endif
#ifndef FORCE_ENABLE
#define FORCE_ENABLE  1
#endif
#ifndef PLATFORM_POR
#define PLATFORM_POR  0
#endif


//
// General
//
#ifndef STATIC
#define STATIC  static
#endif

#ifndef VOID
#define VOID  void
#endif

#ifndef CHAR8
typedef char CHAR8;
#endif

//
// Edk2\MdePkg\Include\Base.h
//
#define  BIT0   0x00000001
#define  BIT1   0x00000002
#define  BIT2   0x00000004
#define  BIT3   0x00000008
#define  BIT4   0x00000010
#define  BIT5   0x00000020
#define  BIT6   0x00000040
#define  BIT7   0x00000080
#define  BIT8   0x00000100
#define  BIT9   0x00000200
#define  BIT10  0x00000400
#define  BIT11  0x00000800
#define  BIT12  0x00001000
#define  BIT13  0x00002000
#define  BIT14  0x00004000
#define  BIT15  0x00008000
#define  BIT16  0x00010000
#define  BIT17  0x00020000
#define  BIT18  0x00040000
#define  BIT19  0x00080000
#define  BIT20  0x00100000
#define  BIT21  0x00200000
#define  BIT22  0x00400000
#define  BIT23  0x00800000
#define  BIT24  0x01000000
#define  BIT25  0x02000000
#define  BIT26  0x04000000
#define  BIT27  0x08000000
#define  BIT28  0x10000000
#define  BIT29  0x20000000
#define  BIT30  0x40000000
#define  BIT31  0x80000000
#define  BIT32  0x0000000100000000ULL
#define  BIT33  0x0000000200000000ULL
#define  BIT34  0x0000000400000000ULL
#define  BIT35  0x0000000800000000ULL
#define  BIT36  0x0000001000000000ULL
#define  BIT37  0x0000002000000000ULL
#define  BIT38  0x0000004000000000ULL
#define  BIT39  0x0000008000000000ULL
#define  BIT40  0x0000010000000000ULL
#define  BIT41  0x0000020000000000ULL
#define  BIT42  0x0000040000000000ULL
#define  BIT43  0x0000080000000000ULL
#define  BIT44  0x0000100000000000ULL
#define  BIT45  0x0000200000000000ULL
#define  BIT46  0x0000400000000000ULL
#define  BIT47  0x0000800000000000ULL
#define  BIT48  0x0001000000000000ULL
#define  BIT49  0x0002000000000000ULL
#define  BIT50  0x0004000000000000ULL
#define  BIT51  0x0008000000000000ULL
#define  BIT52  0x0010000000000000ULL
#define  BIT53  0x0020000000000000ULL
#define  BIT54  0x0040000000000000ULL
#define  BIT55  0x0080000000000000ULL
#define  BIT56  0x0100000000000000ULL
#define  BIT57  0x0200000000000000ULL
#define  BIT58  0x0400000000000000ULL
#define  BIT59  0x0800000000000000ULL
#define  BIT60  0x1000000000000000ULL
#define  BIT61  0x2000000000000000ULL
#define  BIT62  0x4000000000000000ULL
#define  BIT63  0x8000000000000000ULL

#define  SIZE_64KB   0x00010000
#define  SIZE_1MB    0x00100000

#ifndef MAX_UINT64
#define MAX_UINT64  ((UINT64)0xFFFFFFFFFFFFFFFFULL)
#endif

#ifndef MAX_UINT32
#define MAX_UINT32  ((UINT32)0xFFFFFFFF)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(Array)  (sizeof (Array) / sizeof ((Array)[0]))
#endif

#define  PCI_BASE_ADDRESS_MEM_TYPE_32        0x00        /* 32 bit address */
#define  PCI_BASE_ADDRESS_MEM_TYPE_64        0x04        /* 64 bit address */

//
// MdePkg\Include\Pi\PiBootMode.h
//
#ifndef BOOT_ASSUMING_NO_CONFIGURATION_CHANGES
#define BOOT_ASSUMING_NO_CONFIGURATION_CHANGES         0x02
#endif

#ifndef BOOT_ON_S3_RESUME
#define BOOT_ON_S3_RESUME                              0x11
#endif

///
/// Edk2\MdePkg\Include\IndustryStandard\SmBios.h
/// Memory Device - Type
///
typedef enum {
  MemoryTypeOther                    = 0x01,
  MemoryTypeUnknown                  = 0x02,
  MemoryTypeDram                     = 0x03,
  MemoryTypeEdram                    = 0x04,
  MemoryTypeVram                     = 0x05,
  MemoryTypeSram                     = 0x06,
  MemoryTypeRam                      = 0x07,
  MemoryTypeRom                      = 0x08,
  MemoryTypeFlash                    = 0x09,
  MemoryTypeEeprom                   = 0x0A,
  MemoryTypeFeprom                   = 0x0B,
  MemoryTypeEprom                    = 0x0C,
  MemoryTypeCdram                    = 0x0D,
  MemoryType3Dram                    = 0x0E,
  MemoryTypeSdram                    = 0x0F,
  MemoryTypeSgram                    = 0x10,
  MemoryTypeRdram                    = 0x11,
  MemoryTypeDdr                      = 0x12,
  MemoryTypeDdr2                     = 0x13,
  MemoryTypeDdr2FbDimm               = 0x14,
  MemoryTypeDdr3                     = 0x18,
  MemoryTypeFbd2                     = 0x19,
  MemoryTypeDdr4                     = 0x1A,
  MemoryTypeLpddr                    = 0x1B,
  MemoryTypeLpddr2                   = 0x1C,
  MemoryTypeLpddr3                   = 0x1D,
  MemoryTypeLpddr4                   = 0x1E,
  MemoryTypeLogicalNonVolatileDevice = 0x1F,
  MemoryTypeHBM                      = 0x20,
  MemoryTypeHBM2                     = 0x21,
  MemoryTypeDdr5                     = 0x22,
  MemoryTypeLpddr5                   = 0x23,
  MemoryTypeHBM3                     = 0x24
} MEMORY_DEVICE_TYPE;

#endif // _IP_IGPU_DEFINES_H_
