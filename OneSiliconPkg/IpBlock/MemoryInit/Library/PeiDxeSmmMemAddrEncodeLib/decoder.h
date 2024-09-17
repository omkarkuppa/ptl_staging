/** @file
  decode encode data types

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/


#ifndef DECODER_H
#define DECODER_H
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Register/Cpuid.h>

#undef DECODER_HAS_DEBUG

#ifdef DECODER_HAS_DEBUG
#define DECODER_DEBUG(...) DEBUG((DEBUG_INFO, __VA_ARGS__))
#else
#define DECODER_DEBUG(...)
#endif

#define DECODER_ERROR(...) DEBUG((DEBUG_ERROR, __VA_ARGS__))

#define CPUID_FULL_FAMILY                    0x0FFF0000
#define CPUID_FULL_FAMILY_MODEL              0x0FFF0FF0
#define CPUID_FULL_FAMILY_PANTHERLAKE        0x000C0000

typedef enum {
  EnumCpuPtl        = 0,
  EnumCpuUnknown
} CPU_FAMILY;

#define HASH_OFFSET 6        //Hash masks and LSB bit start at address bit 6
#define ZONE1_START_UNIT 30  //Zone1_Start field is in GB
#define CH_SIZE_UNIT     29  //ch_size fields are in 0.5GB
#define REMAP_UNIT       20  //remap and tolud regsiters are in MB

//Number of channels and sub channels
#define NUM_CH      2
#define NUM_SUB_CH  2

// Configuration registers
typedef struct registers {
   UINT32 mad_inter_channel;               // MAD_INTER_CHANNEL_0_0_0_MCHBAR
   UINT32 mad_intra_channel[NUM_CH];       // MAD_INTRA_CHANNEL_0_0_0_MCHBAR[1:0] (per channel)
   UINT32 mad_mc_hash;                     // MAD_MC_HASH_0_0_0_MCHBAR
   UINT32 mad_channel_hash;                // MAD_CHANNEL_HASH_0_0_0_MCHBAR
   UINT32 mad_sub_channel_hash;            // MAD_SUB_CHANNEL_HASH_0_0_0_MCHBAR
   UINT32 mad_dram_hash[NUM_CH];           // MAD_DRAM_HASH_0_0_0_MCHBAR[1:0] (per channel)

   UINT64 remap_base;                      // REMAP_BASE
   UINT64 remap_limit;                     // REMAP_LIMIT
   UINT64 tolud;                           // TOLUD
   UINT32 memory_slice_hash;               // MEMORY_SLICE_HASH
} registers;

//Encoding of MAD_INTER_CHANNEL_0_0_0_MCHBAR.DDR_TYPE
typedef enum {lpddr5, ddr5} tech;

// Memory configuration structure

typedef struct config {
   CPU_FAMILY family;
   tech ddr_type;                              // DRAM technology (0=LPDDR5 1=DDR5)
   BOOLEAN ecc;                                // UNUSED - part of MAD_INTER_CHANNEL register
   UINT32  ch_size[NUM_CH];                    // Channel size in 0.5GB multiples
   UINT32  device_density[NUM_CH][NUM_SUB_CH]; // Device densities in 0.5GB multiples
   UINT8   device_width[NUM_CH][NUM_SUB_CH];   // Device widths (0=x8 1=x16)
   UINT8   ranks[NUM_CH][NUM_SUB_CH];          // Number of ranks (0=1R 1=2R)
   BOOLEAN mc_hash_enabled;                    // Hash between two MCs based on mc_hash_lsb
   UINT32  mc_hash_lsb;                        // address bit to remove in case of address in Zone0 0=bit6, 1=bit7,...
   UINT32  zone1_start;                        // Zone1 start address in GBs (for single MC or asymmetric populations between MCs)
   UINT32  ch_hash_mask;                       // Channel hash mask (bits [19:6] of slice address)
   UINT32  ch_hash_lsb;                        // Channel hash LSB bit 0=bit6, 1=bit7,...
   UINT32  sub_ch_hash_mask;                   // Sub channel hash mask (bits [19:6] of channel address)
   UINT32  sub_ch_hash_lsb;                    // Sub channel hash LSB bit 0-bit6, 1=bit7,...
   BOOLEAN eim[NUM_CH];                        // EIM (enhanced interleave mode)
   BOOLEAN ebh[NUM_CH];                        // EBH (enhanced bank interleave)
   UINT32  bg0_hash[NUM_CH];                   // BG0 hash bit 0=no, 1=bit6, 2=bit7,...
   UINT32  bg1_hash[NUM_CH];                   // BG1 hash bit 0=no, 1=bit6, 2=bit7,...
   UINT32  bg2_hash[NUM_CH];                   // BG2 hash bit 0=no, 1=bit6, 2=bit7,...
   UINT64  remap_base;                         // remap base (in MB)
   UINT64  tolud;                              // tolud (in MB)
   UINT64  remap_limit;                        // remap limit (in MB)
   UINT32  mc_hash_mask;                       // memory_slice_hash.hash_mask
   UINT8   slice_l_id;                         // memory_slice_hash.slice_l_id
   BOOLEAN onedpc[NUM_CH];
} config;

// DRAM address structure
typedef struct dram_address {
   UINT32 mc;                                  // MC (only used if doing physical to system translation)
   UINT32 ch;                                  // Channel
   UINT32 sub_ch;                              // Sub channel
   UINT32 rank;                                // Rank
   UINT32 bg;                                  // Bank group
   UINT32 bank;                                // Bank
   UINT32 row;                                // Row
   UINT32 column;                             // Column
} dram_address;

//Struct used for holding stages of decoded address
typedef struct slice {
   UINT32 id;                              // slice ID
   UINT64 address;                         // slice address
} slice;

//Struct used for holding DRAM address field ranges
typedef struct range {
   UINT32 range_rank;                          // Location of rank bit in DRAM decode table
   UINT32 range_bg0;                           // Location of BG0 bit in DRAM decode table
   UINT32 range_bg1;                           // Location of BG1 bit in DRAM decode table
   UINT32 range_bg2;                           // Location of BG2 bit in DRAM decode table
   UINT32 range_ba0;                           // Location of BA0 bit in DRAM decode table
   UINT32 range_ba1;                           // Location of BA1 bit in DRAM decode table
   UINT32 range_row_msb;                       // Location of row MSB bit in DRAM decode table
   UINT32 range_row_lsb;                       // Location of row LSB bin in DRAM decode table
   UINT32 range_col_msb;                       // Location of column MSB bit in DRAM decode table
   UINT32 range_col_lsb;                       // Location of column LSB bit in DRAM decode table
   UINT32 eim_rank;                            // Location of EIM hash for rank bit
   UINT32 eim_ba0;                             // Location of EIM hash for BA0 bit
   UINT32 eim_ba1;                             // Location of EIM hash for BA1 bit
   UINT64 ebh_bg0;                             // EBH mask for BG0 bit
   UINT64 ebh_bg1;                             // EBH mask for BG1 bit
   UINT64 ebh_bg2;                             // EBH mask for BG2 bit
   UINT64 ebh_ba0;                             // EBH mask for BA0 bit
   UINT64 ebh_ba1;                             // EBH mask for BA1 bit
} range;

//return a bit from a 64bit value: value[bit]
static inline UINT32 bit(UINT64 value, UINT32 bit) {
   return RShiftU64(value, bit) & 1;
}

//return subset of bits from 64b value: value[msb:lsb] (maximum mask width supported 32bit)
static inline UINT64 range64(UINT64 value, UINT32 msb, UINT32 lsb) {
   return RShiftU64(value, lsb) & (LShiftU64(1, msb - lsb + 1) - 1);
}

//return subset of bits from 32b value: value[msb:lsb]
static inline UINT32 range32(UINT32 value, UINT32 msb, UINT32 lsb) {
   return (value >> lsb) & ((1 << (msb - lsb + 1)) -1);
}

//remove bit from 64b value
static inline UINT64 chop(UINT64 address, UINT32 bit) {
          // MSB[64:bit+1]                                LSB[bit-1: 0]
   return LShiftU64(RShiftU64(address, (bit + 1)) ,bit) | (address & (LShiftU64(1, bit) - 1));
}

//return |(A^B)
static inline UINT32 xor(UINT64 a, UINT64 b) {
   UINT8 parity = 0;
   UINT64 temp = a & b;
   while(temp) {
      if (temp & 1) {
         parity = 1 - parity;
      }
      temp = RShiftU64(temp, 1);
   }
   return parity;
}

#endif //DECODER_H

