/** @file
  SPD tables for PTL boards.

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

// LPDDR4 200b
// Samsung K4F6E304Hx-MGCJ
// or Hynix H9HCNNNBUUxLHR-NMx
// or Micron MT53B512M32D2NP-053
// 3733, 32-34-34-79
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mLpddr4Ddp8Gb200bSpd[] = {
  0x23,                                 ///< 0   384 SPD bytes used, 512 total
  0x11,                                 ///< 1   SPD Revision 1.1
  0x10,                                 ///< 2   DRAM Type: LPDDR4 SDRAM
  0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
  0x15,                                 ///< 4   8 Banks, no bank groups, 8 Gb SDRAM density
  0x19,                                 ///< 5   15 Rows, 10 Columns
  0x91,                                 ///< 6   Non-Monolithic DRAM Device, 2 die, 1 Channels per package, Signal Loading Matrix 1
  0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
  0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
  0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
  0x00,                                 ///< 10  Reserved
  0x00,                                 ///< 11  Module Nominal Voltage: Reserved
  0x0A,                                 ///< 12  Module Organization: 2 Ranks, x16 Device Width per Channel
  0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
  0x00,                                 ///< 14  Module Thermal Sensor: none
  0x00,                                 ///< 15  Extended Module Type: Reserved
  0x00,                                 ///< 16  Signal Loading: not specified
  0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
  0x05,                                 ///< 18  tCKAVGmin = 0.536 ns (LPDDR4-3733)
  0x0F,                                 ///< 19  tCKAVGmax = 1.875 ns
  0x92,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
  0x54,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20
  0x01,                                 ///< 22  CAS Latencies supported (Third Byte) : 32
  0x00,                                 ///< 23  CAS Latencies supported (Fourth Byte):
  0x8A,                                 ///< 24  Minimum CAS Latency (tAAmin) = 17.14 ns
  0x00,                                 ///< 25  Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
  0x90,                                 ///< 26  Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xA8,                                 ///< 27  Row precharge time for all banks (tRPab) = 21 ns
  0x90,                                 ///< 28  Minimum row precharge time (tRPmin) = 18 ns
  0xA0,                                 ///< 29  tRFCab = 180 ns (8 Gb dual-channel die)
  0x05,                                 ///< 30  tRFCab MSB
  0xD0,                                 ///< 31  tRFCpb = 90 ns (8 Gb dual-channel die)
  0x02,                                 ///< 32  tRFCpb MSB
  0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
  0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
  0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
  0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0x92,                                 ///< 123 FTB for tAAmin = 17.14 ns
  0x00,                                 ///< 124 FTB for tCKAVGmax = 1.875 ns
  0xA7,                                 ///< 125 FTB for tCKAVGmin = 0.536 ns (LPDDR4-3733)
  0x00,                                 ///< 126 CRC A
  0x00,                                 ///< 127 CRC B
  0, 0,                                 ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
  0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
  0x00,                                 ///< 322 Module Manufacturing Location
  0x00,                                 ///< 323 Module Manufacturing Date Year
  0x00,                                 ///< 324 Module Manufacturing Date Week
  0x20,                                 ///< 325 Module Serial Number A
  0x00,                                 ///< 326 Module Serial Number B
  0x00,                                 ///< 327 Module Serial Number C
  0x00,                                 ///< 328 Module Serial Number D
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
  0x00,                                 ///< 349 Module Revision Code
  0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
  0x00                                  ///< 352 DRAM Stepping
                                        ///< 353 - 511 are zeroes, no need to keep them, reduce BIOS image size
};

// LPDDR4X 556b
// Micron MT53D512M64D4RQ-046 WT:E
// 4266
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mLpddr4xQdp4Gb556bSpd[] = {
  0x23,                                 ///< 0   384 SPD bytes used, 512 total
  0x11,                                 ///< 1   SPD Revision 1.1
  0x11,                                 ///< 2   DRAM Type: LPDDR4x SDRAM
  0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
  0x15,                                 ///< 4   8 Banks, no bank groups, 8 Gb SDRAM density
  0x21,                                 ///< 5   16 Row bits, 10 Column bits
  0xB9,                                 ///< 6   Non-Monolithic DRAM Device, 4 dies, 2 Channels per die, Signal Loading not specified
  0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
  0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
  0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
  0x00,                                 ///< 10  Reserved
  0x00,                                 ///< 11  Module Nominal Voltage: Reserved
  0x02,                                 ///< 12  Module Organization: 1 Ranks, x16 Device Width per Channel
  0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
  0x00,                                 ///< 14  Module Thermal Sensor: none
  0x00,                                 ///< 15  Extended Module Type: Reserved
  0x00,                                 ///< 16  Signal Loading: Not Specific
  0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
  0x04,                                 ///< 18  tCKAVGmin = 0.536 ns (LPDDR4-3733)
  0x0F,                                 ///< 19  tCKAVGmax = 1.875 ns
  0x92,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
  0x54,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20
  0x05,                                 ///< 22  CAS Latencies supported (Third Byte) : 32
  0x00,                                 ///< 23  CAS Latencies supported (Fourth Byte):
  0x87,                                 ///< 24  Minimum CAS Latency (tAAmin) = 17.14 ns
  0x00,                                 ///< 25  Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
  0x90,                                 ///< 26  Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xA8,                                 ///< 27  Row precharge time for all banks (tRPab) = 21 ns
  0x90,                                 ///< 28  Minimum row precharge time (tRPmin) = 18 ns
  0xC0,                                 ///< 29  tRFCab = 280 ns (8 Gb single-channel die)
  0x08,                                 ///< 30  tRFCab MSB
  0x60,                                 ///< 31  tRFCpb = 140 ns (8 Gb single-channel die)
  0x04,                                 ///< 32  tRFCpb MSB
  0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
  0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
  0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
  0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xE5,                                 ///< 123 FTB for tAAmin = 17.14 ns (FTB = -0.11 ns)
  0x00,                                 ///< 124 FTB for tCKAVGmax = 1.875 ns (FTB = 0 ns)
  0xE1,                                 ///< 125 FTB for tCKAVGmin = 0.536 ns (FTB = -.089 ns) (LPDDR4-3733)
  0x85,                                 ///< 126 CRC A
  0x85,                                 ///< 127 CRC B
  0, 0,                                 ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
  0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
  0x00,                                 ///< 322 Module Manufacturing Location
  0x00,                                 ///< 323 Module Manufacturing Date Year
  0x00,                                 ///< 324 Module Manufacturing Date Week
  0x56,                                 ///< 325 Module Serial Number A
  0x00,                                 ///< 326 Module Serial Number B
  0x00,                                 ///< 327 Module Serial Number C
  0x00,                                 ///< 328 Module Serial Number D
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
  0x00,                                 ///< 349 Module Revision Code
  0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
  0x00                                  ///< 352 DRAM Stepping
                                        ///< 353 - 511 are zeroes, no need to keep them, reduce BIOS image size
};

// LPDDR4X 432b
// Samsung K3UH7H70MM-JGCJ
// 3733, 32-34-34-79
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mLpddr4xQdp16Gb432bSpd[] = {
  0x23,                                 ///< 0   384 SPD bytes used, 512 total
  0x11,                                 ///< 1   SPD Revision 1.1
  0x11,                                 ///< 2   DRAM Type: LPDDR4x SDRAM
  0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
  0x16,                                 ///< 4   8 Banks, no bank groups, 16 Gb SDRAM density
  0x21,                                 ///< 5   16 Row bits, 10 Column bits
  0xB4,                                 ///< 6   Non-Monolithic DRAM Device, 4 dies, 2 Channels per die, Signal Loading not specified
  0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
  0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
  0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
  0x00,                                 ///< 10  Reserved
  0x00,                                 ///< 11  Module Nominal Voltage: Reserved
  0x0A,                                 ///< 12  Module Organization: 2 Ranks, x16 Device Width per Channel
  0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
  0x00,                                 ///< 14  Module Thermal Sensor: none
  0x00,                                 ///< 15  Extended Module Type: Reserved
  0x48,                                 ///< 16  Signal Loading: Data/Strobe/Mask: 2 loads, CAC: 2 loads, CS: 1 load
  0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
  0x05,                                 ///< 18  tCKAVGmin = 0.536 ns (LPDDR4-3733)
  0x0F,                                 ///< 19  tCKAVGmax = 1.875 ns
  0x92,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
  0x54,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20
  0x01,                                 ///< 22  CAS Latencies supported (Third Byte) : 32
  0x00,                                 ///< 23  CAS Latencies supported (Fourth Byte):
  0x8A,                                 ///< 24  Minimum CAS Latency (tAAmin) = 17.14 ns
  0x00,                                 ///< 25  Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
  0x90,                                 ///< 26  Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xA8,                                 ///< 27  Row precharge time for all banks (tRPab) = 21 ns
  0x90,                                 ///< 28  Minimum row precharge time (tRPmin) = 18 ns
  0xC0,                                 ///< 29  tRFCab = 280 ns (16 Gb dual-channel die)
  0x08,                                 ///< 30  tRFCab MSB
  0x60,                                 ///< 31  tRFCpb = 140 ns (16 Gb dual-channel die)
  0x04,                                 ///< 32  tRFCpb MSB
  0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
  0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
  0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
  0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0x92,                                 ///< 123 FTB for tAAmin = 17.14 ns (FTB = -0.11 ns)
  0x00,                                 ///< 124 FTB for tCKAVGmax = 1.875 ns (FTB = 0 ns)
  0xA7,                                 ///< 125 FTB for tCKAVGmin = 0.536 ns (FTB = -.089 ns) (LPDDR4-3733)
  0x00,                                 ///< 126 CRC A
  0x00,                                 ///< 127 CRC B
  0, 0,                                 ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
  0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
  0x00,                                 ///< 322 Module Manufacturing Location
  0x00,                                 ///< 323 Module Manufacturing Date Year
  0x00,                                 ///< 324 Module Manufacturing Date Week
  0x43,                                 ///< 325 Module Serial Number A
  0x00,                                 ///< 326 Module Serial Number B
  0x00,                                 ///< 327 Module Serial Number C
  0x00,                                 ///< 328 Module Serial Number D
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
  0x00,                                 ///< 349 Module Revision Code
  0x80,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
  0xCE,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
  0x00                                  ///< 352 DRAM Stepping
                                        ///< 353 - 511 are zeroes, no need to keep them, reduce BIOS image size
};

// LPDDR4X 556b
// Micron MT53D1G64D8SQ-046
// 4266
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mLpddr4xOdp8Gb556bSpd[] = {
  0x23,                                 ///< 0   384 SPD bytes used, 512 total
  0x11,                                 ///< 1   SPD Revision 1.1
  0x11,                                 ///< 2   DRAM Type: LPDDR4x SDRAM
  0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
  0x1B,                                 ///< 4   8 Banks, no bank groups, 6 Gb SDRAM density
  0x21,                                 ///< 5   15 Rows, 10 Columns
  0xF8,                                 ///< 6   Non-Monolithic DRAM Device, 8 die, 4 Channels per package, Signal Loading Matrix 1
  0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
  0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
  0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
  0x00,                                 ///< 10  Reserved
  0x00,                                 ///< 11  Module Nominal Voltage: Reserved
  0x0A,                                 ///< 12  Module Organization: 2 Ranks, x16 Device Width per Channel
  0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
  0x00,                                 ///< 14  Module Thermal Sensor: none
  0x00,                                 ///< 15  Extended Module Type: Reserved
  0x00,                                 ///< 16  Signal Loading: Not Specific
  0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
  0x05,                                 ///< 18  tCKAVGmin = 0.536 ns (LPDDR4-3733)
  0x0F,                                 ///< 19  tCKAVGmax = 1.875 ns
  0x92,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
  0x54,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20
  0x01,                                 ///< 22  CAS Latencies supported (Third Byte) : 32
  0x00,                                 ///< 23  CAS Latencies supported (Fourth Byte):
  0x8A,                                 ///< 24  Minimum CAS Latency (tAAmin) = 17.14 ns
  0x00,                                 ///< 25  Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
  0x90,                                 ///< 26  Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xA8,                                 ///< 27  Row precharge time for all banks (tRPab) = 21 ns
  0x90,                                 ///< 28  Minimum row precharge time (tRPmin) = 18 ns
  0xC0,                                 ///< 29  tRFCab = 280 ns (8 Gb single-channel die)
  0x08,                                 ///< 30  tRFCab MSB
  0x60,                                 ///< 31  tRFCpb = 140 ns (8 Gb single-channel die)
  0x04,                                 ///< 32  tRFCpb MSB
  0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
  0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
  0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
  0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0x92,                                 ///< 123 FTB for tAAmin = 17.14 ns (FTB = -0.11 ns)
  0x00,                                 ///< 124 FTB for tCKAVGmax = 1.875 ns (FTB = 0 ns)
  0xA7,                                 ///< 125 FTB for tCKAVGmin = 0.536 ns (FTB = -.089 ns) (LPDDR4-3733)
  0x00,                                 ///< 126 CRC A
  0x00,                                 ///< 127 CRC B
  0, 0,                                 ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
  0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
  0x00,                                 ///< 322 Module Manufacturing Location
  0x00,                                 ///< 323 Module Manufacturing Date Year
  0x00,                                 ///< 324 Module Manufacturing Date Week
  0x56,                                 ///< 325 Module Serial Number A
  0x00,                                 ///< 326 Module Serial Number B
  0x00,                                 ///< 327 Module Serial Number C
  0x00,                                 ///< 328 Module Serial Number D
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
  0x00,                                 ///< 349 Module Revision Code
  0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
  0x00                                  ///< 352 DRAM Stepping
                                        ///< 353 - 511 are zeroes, no need to keep them, reduce BIOS image size
};

GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mLpddr4Ddp16Gb200b1rSpd[] = {
  0x23,                                 ///< 0 384 SPD bytes used, 512 total
  0x10,                                 ///< 1 SPD Revision 1.0
  0x11,                                 ///< 2 DRAM Type: LPDDR4X SDRAM
  0x0E,                                 ///< 3 Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
  0x15,                                 ///< 4 8 Banks, no bank groups, 8 Gb SDRAM density
  0x19,                                 ///< 5 15 Rows, 10 Columns
  0x95,                                 ///< 6 Non-Monolithic DRAM Device, 2 die, 2 Channels per package, Signal Loading Matrix 1
  0x08,                                 ///< 7 SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
  0x00,                                 ///< 8 SDRAM Thermal / Refresh options: Reserved
  0x40,                                 ///< 9 Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
  0x00,                                 ///< 10 Reserved
  0x00,                                 ///< 11 Module Nominal Voltage: Reserved
  0x02,                                 ///< 12 Module Organization: 1 Ranks, x16 Device Width per Channel
  0x01,                                 ///< 13 Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
  0x00,                                 ///< 14 Module Thermal Sensor: none
  0x00,                                 ///< 15 Extended Module Type: Reserved
  0x48,                                 ///< 16 Signal Loading: Data/Strobe/Mask: 2 loads, CAC: 2 loads, CS: 1 load
  0x00,                                 ///< 17 MTB = 0.125ns, FTB = 1 ps
  0x04,                                 ///< 18  tCKAVGmin = 0.469 ns (LPDDR4-4267)
  0xFF,                                 ///< 19  tCKAVGmax = 32.002 ns
  0x92,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
  0x55,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20, 16
  0x00,                                 ///< 22  CAS Latencies supported (Third Byte) :
  0x00,                                 ///< 23 CAS Latencies supported (Fourth Byte):
  0x8C,                                 ///< 24  Minimum CAS Latency (tAAmin) = 17.5 ns
  0x00,                                 ///< 25 Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
  0x90,                                 ///< 26 Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xA8,                                 ///< 27 Row precharge time for all banks (tRPab) = 21 ns
  0x90,                                 ///< 28 Minimum row precharge time (tRPmin) = 18 ns
  0x90,                                 ///< 29 tRFCab = 210 ns (16 Gb dual-channel die)
  0x06,                                 ///< 30 tRFCab MSB
  0xD0,                                 ///< 31 tRFCpb = 90 ns (16 Gb dual-channel die)
  0x02,                                 ///< 32 tRFCpb MSB
  0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
  0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
  0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
  0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0x00,                                 ///< 123 FTB for tAAmin = 17.5 ns
  0x7F,                                 ///< 124 FTB for tCKAVGmax = 32.002 ns
  0xE1,                                 ///< 125 FTB for tCKAVGmin = 0.469 ns (LPDDR4-4267)
  0x00,                                 ///< 126 CRC A
  0x00,                                 ///< 127 CRC B
  0, 0,                                 ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
  0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
  0x00,                                 ///< 322 Module Manufacturing Location
  0x00,                                 ///< 323 Module Manufacturing Date Year
  0x00,                                 ///< 324 Module Manufacturing Date Week
  0x55,                                 ///< 325 Module Serial Number A
  0x00,                                 ///< 326 Module Serial Number B
  0x00,                                 ///< 327 Module Serial Number C
  0x00,                                 ///< 328 Module Serial Number D
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
  0x00,                                 ///< 349 Module Revision Code
  0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
  0x00                                  ///< 352 DRAM Stepping
                                        ///< 353 - 511 are zeroes, no need to keep them, reduce BIOS image size
};

// LPDDR5 496b 12Gb die, QDP 1x16
// Samsung K3LK2K20BM-BGCN
// 5500, ??-??-??-??
// 16 Banks, 4 bank groups, 12Gb SDRAM density
// 16 Row bits, 10 Column bits
// Non-Monolithic DRAM Device, 4 dies, 4 Channels per die,
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mLpddr5xQdp12Gb496bSpd[] = {
  // ************************************
  // Placeholder need correct SPD for LP5
  // ************************************
  0x23,                                 ///< 0   384 SPD bytes used, 512 total
  0x10,                                 ///< 1   SPD Revision 1.0
  0x13,                                 ///< 2   DRAM Type: LPDDR5 SDRAM
  0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
  0x88,                                 ///< 4   8 Banks, no bank groups, 12 Gb SDRAM density
  0x29,                                 ///< 5   17 Rows, 10 Columns
  0xB8,                                 ///< 6   Non-Monolithic DRAM Device, 4 die, 4 Channels per package, Signal Loading Matrix 1
  0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
  0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
  0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
  0x00,                                 ///< 10  Reserved
  0x00,                                 ///< 11  Module Nominal Voltage: Reserved
  0x02,                                 ///< 12  Module Organization: 1 Ranks, x16 Device Width per Channel
  0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
  0x00,                                 ///< 14  Module Thermal Sensor: none
  0x00,                                 ///< 15  Extended Module Type: Reserved
  0x48,                                 ///< 16  Signal Loading: Data/Strobe/Mask: 2 loads, CAC: 2 loads, CS: 1 load
  0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
  0x05,                                 ///< 18  tCKAVGmin = 0.625 ns (LPDDR4-3200)
  0xFF,                                 ///< 19  tCKAVGmax = 32.002 ns
  0x92,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
  0x55,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20, 16
  0x00,                                 ///< 22  CAS Latencies supported (Third Byte) :
  0x00,                                 ///< 23  CAS Latencies supported (Fourth Byte):
  0x8C,                                 ///< 24  Minimum CAS Latency (tAAmin) = 17.5 ns
  0x00,                                 ///< 25  Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
  0x90,                                 ///< 26  Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xA8,                                 ///< 27  Row precharge time for all banks (tRPab) = 21 ns
  0x90,                                 ///< 28  Minimum row precharge time (tRPmin) = 18 ns
  0x90,                                 ///< 29  tRFCab = 210 ns (16 Gb dual-channel die)
  0x06,                                 ///< 30  tRFCab MSB
  0xD0,                                 ///< 31  tRFCpb = 90 ns (16 Gb dual-channel die)
  0x02,                                 ///< 32  tRFCpb MSB
  0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
  0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
  0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
  0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0x00,                                 ///< 123 FTB for tAAmin = 17.5 ns
  0x7F,                                 ///< 124 FTB for tCKAVGmax = 32.002 ns
  0x00,                                 ///< 125 FTB for tCKAVGmin = 0.469 ns (LPDDR4-4267)
  0x00,                                 ///< 126 CRC A
  0x00,                                 ///< 127 CRC B
  0, 0,                                 ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
  0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
  0x00,                                 ///< 322 Module Manufacturing Location
  0x00,                                 ///< 323 Module Manufacturing Date Year
  0x00,                                 ///< 324 Module Manufacturing Date Week
  0x20,                                 ///< 325 Module ID: Module Serial Number
  0x00,                                 ///< 326 Module Serial Number B
  0x00,                                 ///< 327 Module Serial Number C
  0x00,                                 ///< 328 Module Serial Number D
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
  0x00,                                 ///< 349 Module Revision Code
  0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
  0x00                                  ///< 352 DRAM Stepping
                                        ///< 353 - 511 are zeroes, no need to keep them, reduce BIOS image size
};

// LPDDR4X 200b 16Gb die, DDP, 2 channel per die, 2x8
// Samsung  K4U6E3S4AA-MGCL
// 4267, 36-39-39-90
// 1 rank per channel, 2 SDRAMs per channel, 2x8GB = 16GB total per channel
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mLpddr4xDdp16Gb200bSpd[] = {
  0x23,                                 ///< 0   384 SPD bytes used, 512 total
  0x10,                                 ///< 1   SPD Revision 1.0
  0x11,                                 ///< 2   DRAM Type: LPDDR4x SDRAM
  0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
  0x15,                                 ///< 4   8 Banks, no bank groups, 16 Gb SDRAM density
  0x19,                                 ///< 5   5 15 Rows, 10 Columns
  0x94,                                 ///< 6   Non-Monolithic DRAM Device, 2 die, 1 Channels per package, Signal Loading Matrix 1
  0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
  0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
  0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
  0x00,                                 ///< 10  Reserved
  0x00,                                 ///< 11  Module Nominal Voltage: Reserved
  0x02,                                 ///< 12  Module Organization: 1 Ranks, x16 Device Width per Channel
  0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
  0x00,                                 ///< 14  Module Thermal Sensor: none
  0x00,                                 ///< 15  Extended Module Type: Reserved
  0x48,                                 ///< 16  Signal Loading: Data/Strobe/Mask: 2 loads, CAC: 2 loads, CS: 1 load
  0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
  0x05,                                 ///< 18  tCKAVGmin = 0.625 ns (LPDDR4-3200)
  0xFF,                                 ///< 19  tCKAVGmax = 32.002 ns
  0x92,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
  0x55,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20, 16
  0x00,                                 ///< 22  CAS Latencies supported (Third Byte) :
  0x00,                                 ///< 23  CAS Latencies supported (Fourth Byte):
  0x8C,                                 ///< 24  Minimum CAS Latency (tAAmin) = 17.5 ns
  0x00,                                 ///< 25  Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
  0x90,                                 ///< 26  Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xA8,                                 ///< 27  Row precharge time for all banks (tRPab) = 21 ns
  0x90,                                 ///< 28  Minimum row precharge time (tRPmin) = 18 ns
  0x90,                                 ///< 29  tRFCab = 210 ns (16 Gb dual-channel die)
  0x06,                                 ///< 30  tRFCab MSB
  0xD0,                                 ///< 31  tRFCpb = 90 ns (16 Gb dual-channel die)
  0x02,                                 ///< 32  tRFCpb MSB
  0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
  0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
  0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
  0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0x00,                                 ///< 123 FTB for tAAmin = 17.5 ns
  0x7F,                                 ///< 124 FTB for tCKAVGmax = 32.002 ns
  0x00,                                 ///< 125 FTB for tCKAVGmin = 0.469 ns (LPDDR4-4267)
  0x00,                                 ///< 126 CRC A
  0x00,                                 ///< 127 CRC B
  0, 0,                                 ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
  0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
  0x00,                                 ///< 322 Module Manufacturing Location
  0x00,                                 ///< 323 Module Manufacturing Date Year
  0x00,                                 ///< 324 Module Manufacturing Date Week
  0x20,                                 ///< 325 Module ID: Module Serial Number
  0x00,                                 ///< 326 Module Serial Number B
  0x00,                                 ///< 327 Module Serial Number C
  0x00,                                 ///< 328 Module Serial Number D
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
  0x00,                                 ///< 349 Module Revision Code
  0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
  0x00                                  ///< 352 DRAM Stepping
                                        ///< 353 - 511 are zeroes, no need to keep them, reduce BIOS image size
};

// LPDDR4X 200b 16Gb die, 2 channel per die, 2x8
// Micron MT53E1G32D4NQ-046
// 4266
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mLpddr4xQdp16Gb200bSpd[] = {
  0x23,                                 ///< 0   384 SPD bytes used, 512 total
  0x12,                                 ///< 1   SPD Revision 1.0
  0x11,                                 ///< 2   DRAM Type: LPDDR4x SDRAM
  0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
  0x15,                                 ///< 4   8 Banks, no bank groups, 16 Gb SDRAM density
  0x21,                                 ///< 5   5 15 Rows, 10 Columns
  0xB1,                                 ///< 6   Non-Monolithic DRAM Device, 2 die, 1 Channels per package, Signal Loading Matrix 1
  0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
  0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
  0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
  0x00,                                 ///< 10  Reserved
  0x00,                                 ///< 11  Module Nominal Voltage: Reserved
  0x0A,                                 ///< 12  Module Organization: 1 Ranks, x16 Device Width per Channel
  0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
  0x00,                                 ///< 14  Module Thermal Sensor: none
  0x00,                                 ///< 15  Extended Module Type: Reserved
  0x48,                                 ///< 16  Signal Loading: Data/Strobe/Mask: 2 loads, CAC: 2 loads, CS: 1 load
  0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
  0x04,                                 ///< 18  tCKAVGmin = 0.625 ns (LPDDR4-3200)
  0x00,                                 ///< 19  tCKAVGmax = 32.002 ns
  0xD2,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
  0x54,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20, 16
  0x01,                                 ///< 22  CAS Latencies supported (Third Byte) :
  0x00,                                 ///< 23  CAS Latencies supported (Fourth Byte):
  0x87,                                 ///< 24  Minimum CAS Latency (tAAmin) = 17.5 ns
  0x40,                                 ///< 25  Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
  0x90,                                 ///< 26  Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xA8,                                 ///< 27  Row precharge time for all banks (tRPab) = 21 ns
  0x90,                                 ///< 28  Minimum row precharge time (tRPmin) = 18 ns
  0xC0,                                 ///< 29  tRFCab = 210 ns (16 Gb dual-channel die)
  0x08,                                 ///< 30  tRFCab MSB
  0x60,                                 ///< 31  tRFCpb = 90 ns (16 Gb dual-channel die)
  0x04,                                 ///< 32  tRFCpb MSB
  0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
  0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
  0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
  0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
  0xE4,                                 ///< 123 FTB for tAAmin = 17.5 ns
  0x00,                                 ///< 124 FTB for tCKAVGmax = 32.002 ns
  0x60,                                 ///< 125 FTB for tCKAVGmin = 0.469 ns (LPDDR4-4267)
  0xA1,                                 ///< 126 CRC A
  0xAC,                                 ///< 127 CRC B
  0, 0,                                 ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
  0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
  0x00,                                 ///< 322 Module Manufacturing Location
  0x00,                                 ///< 323 Module Manufacturing Date Year
  0x00,                                 ///< 324 Module Manufacturing Date Week
  0x20,                                 ///< 325 Module ID: Module Serial Number
  0x00,                                 ///< 326 Module Serial Number B
  0x00,                                 ///< 327 Module Serial Number C
  0x00,                                 ///< 328 Module Serial Number D
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
  0x00,                                 ///< 349 Module Revision Code
  0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
  0x00                                  ///< 352 DRAM Stepping
                                        ///< 353 - 511 are zeroes, no need to keep them, reduce BIOS image size
};

GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mDdr5Spd_1Rx8[] = {
  0x30,///< 0   1024 SPD bytes total
  0x08,///< 1   SPD Revision 0.8
  0x12,///< 2   DRAM Type: DDR5 SDRAM
  0x03,///< 3   Module Type: Not Hybrid (DRAM only) / SO-DIMM Solution
  0x04,///< 4   Monolithic SDRAM, 16 Gb SDRAM density
  0x00,///< 5   16 Rows, 10 Columns
  0x20,///< 6   x8 SDRAM I/O Width
  0x62,///< 7   8 Bank Groups, 4 Banks per Bank Group
  0x00,///< 8   Secondary SDRAM Density and Package
  0x00,///< 9   Secondary SDRAM Addressing
  0x00,///< 10  Secondary SDRAM I/O Width
  0x00,///< 11  Secondary BankGroups and Banks per Bank Group
  0x60,///< 12  PPR Supported, One row per bank group, Soft PPR Supported
  0x00,///< 13  Commercial Temperature Grade, 0 to 85 C
  0x00,///< 14  Reserved
  0x00,///< 15  Reserved
  0x00,///< 16  SDRAM Nominal Voltage VDD:  1.1V
  0x00,///< 17  SDRAM Nominal Voltage VDDQ: 1.1V
  0x00,///< 18  SDRAM Nominal Voltage VPP:  1.8V
  0x00,///< 19  Reserved
  0xA1,///< 20  tCKAVGmin LSB
  0x01,///< 21  tCKAVGmin MSB
  0xE8,///< 22  tCKAVGmax LSB
  0x03,///< 23  tCKAVGmax MSB
  0x72,///< 24  CAS Latencies supported (First Byte) : 32, 30, 28, 22
  0x15,///< 25  CAS Latencies supported (Second Byte): 44, 40, 36
  0x00,///< 26  CAS Latencies supported (Third Byte) :
  0x00,///< 27  CAS Latencies supported (Fourth Byte):
  0x00,///< 28  CAS Latencies supported (Fifth Byte) :
  0x00,///< 29  Reserved
  0x1E,///< 30  Minimum CAS Latency (tAAmin) LSB
  0x41,///< 31  Minimum CAS Latency (tAAmin) MSB
  0x1E,///< 32  Minimum RAS-to-CAS delay (tRCDmin) LSB
  0x41,///< 33  Minimum RAS-to-CAS delay (tRCDmin) MSB
  0x1E,///< 34  Minimum Row Precharge delay (tRPmin) LSB
  0x41,///< 35  Minimum Row Precharge delay (tRPmin) MSB
  0x00,///< 36  Minimum Active to Precharge delay (tRASmin) LSB
  0x7D,///< 37  Minimum Active to Precharge delay (tRASmin) MSB
  0x1E,///< 38  Minimum Active to Active/Refresh delay (tRCmin) LSB
  0xBE,///< 39  Minimum Active to Active/Refresh delay (tRCmin) MSB
  0x30,///< 40  Minimum Write Recovery time (tWRmin) LSB
  0x75,///< 41  Minimum Write Recovery time (tWRmin) MSB
  0x27,///< 42  Refresh Recovery Delay (tRFC1min) LSB
  0x01,///< 43  Refresh Recovery Delay (tRFC1min) MSB
  0xA0,///< 44  Refresh Recovery Delay (tRFC2min) MSB
  0x00,///< 45  Refresh Recovery Delay (tRFC2min) MSB
  0x82,///< 46  Refresh Recovery Delay (tRFCsbmin) MSB
  0x00,///< 47  Refresh Recovery Delay (tRFCsbmin) MSB
  0,  0, ///< 48 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 110 - 119
  0, 0, 0, 0, 0, 0, ///< 120 - 125
  0x47, ///< 126 CRC Bytes 0 - 127 LSB
  0xAE, ///< 127 CRC Bytes 0 - 127 MSB
  0, 0, ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 180 - 189
  0, 0, ///< 190 - 191
  0x08, ///< 192 SPD Revision for Module Information: 0.8
  0x00, ///< 193 Reserved
  0xC2, ///< 194 SPD Manufacturer ID First Byte
  0xC4, ///< 195 SPD Manufacturer ID Second Byte
  0x80, ///< 196 SPD Device Type
  0x00, ///< 197 SPD Device Revision
  0x80, ///< 198 PMIC0 Manufacturer ID First Byte
  0xB3, ///< 199 PMIC0 Manufacturer ID Second Byte
  0x80, ///< 200 PMIC0 Device Type
  0x11, ///< 201 PMIC0 Device Revision
  0, 0, 0, 0, ///< 202 - 205 PMIC1
  0, 0, 0, 0, ///< 206 - 209 PMIC2
  0x80, ///< 210 Thermal Sensors Manufacturer ID First Byte
  0xB3, ///< 211 Thermal Sensors Manufacturer ID First Byte
  0x80, ///< 212 Thermal Sensors Device Type
  0x11, ///< 213 Thermal Sensors Device Revision
  0, 0, 0, 0, 0, 0, ///< 214 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 220 - 229
  0x0F, ///< 230 Module Nominal Height
  0x10, ///< 231 Module Nominal Thickness
  0x00, ///< 232 Reference Raw Card Used
  0x01, ///< 233 1 Row of DRAM on Module
  0x01, ///< 234 1 Rank, 8 bits SDRAM data width per channel
  0x22, ///< 235 2 Channels per DIMM, 32 bits per Channel
  0, 0, 0, 0, ///< 236 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 310 - 319
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 320 - 329
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 330 - 339
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 340 - 349
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 350 - 359
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 360 - 369
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 370 - 379
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 380 - 389
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 390 - 399
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 400 - 409
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 410 - 419
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 420 - 429
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 430 - 439
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00, ///< 440 - 445
  0x9C, ///< 446 CRC for Bytes 128 - 253 LSB
  0xAD, ///< 447 CRC for Bytes 128 - 253 MSB
  0, 0, ///< 448 - 449
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 450 - 459
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 460 - 469
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 470 - 479
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 480 - 489
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 490 - 499
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 500 - 509
  0, 0, ///< 510 - 511
  0x80, ///< 512 Module Manufacturer ID First Byte
  0xAD, ///< 513 Module Manufacturer ID Second Byte
  0x00, ///< 514 Module Manufacturing Location
  0, 0, ///< 515 - 516 Module Manufacturing Date
  0, 0, 0, 0, ///< 517 - 520 Module Serial Number
  0x54,  0x4D,  0x43,  0x41,  0x32,  0x47,  0x53,  0x36,  0x41,  0x4A, ///< 521 - 530
  0x52,  0x38,  0x43,  0x2D,  0x45,  0x42,  0x20,  0x20,  0x20,  0x20, ///< 531 - 540
  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20, ///< 541 - 550
  0x00, ///< 551 Module Revision Code
  0x80, ///< 552 DRAM Manufacturer ID First Byte
  0xAD  ///< 553 DRAM Manufacturer ID Second Byte
        ///< 554 - 1023 are zeroes, no need to keep them, save PEI FV size
};

GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mDdr5Spd_2Rx8[] = {
  0x30,///< 0   1024 SPD bytes total
  0x08,///< 1   SPD Revision 0.8
  0x12,///< 2   DRAM Type: DDR5 SDRAM
  0x03,///< 3   Module Type: Not Hybrid (DRAM only) / SO-DIMM Solution
  0x04,///< 4   Monolithic SDRAM, 16 Gb SDRAM density
  0x00,///< 5   16 Rows, 10 Columns
  0x20,///< 6   x8 SDRAM I/O Width
  0x62,///< 7   8 Bank Groups, 4 Banks per Bank Group
  0x00,///< 8   Secondary SDRAM Density and Package
  0x00,///< 9   Secondary SDRAM Addressing
  0x00,///< 10  Secondary SDRAM I/O Width
  0x00,///< 11  Secondary BankGroups and Banks per Bank Group
  0x60,///< 12  PPR Supported, One row per bank group, Soft PPR Supported
  0x00,///< 13  Commercial Temperature Grade, 0 to 85 C
  0x00,///< 14  Reserved
  0x00,///< 15  Reserved
  0x00,///< 16  SDRAM Nominal Voltage VDD:  1.1V
  0x00,///< 17  SDRAM Nominal Voltage VDDQ: 1.1V
  0x00,///< 18  SDRAM Nominal Voltage VPP:  1.8V
  0x00,///< 19  Reserved
  0xA1,///< 20  tCKAVGmin LSB
  0x01,///< 21  tCKAVGmin MSB
  0xE8,///< 22  tCKAVGmax LSB
  0x03,///< 23  tCKAVGmax MSB
  0x72,///< 24  CAS Latencies supported (First Byte) : 32, 30, 28, 22
  0x15,///< 25  CAS Latencies supported (Second Byte): 44, 40, 36
  0x00,///< 26  CAS Latencies supported (Third Byte) :
  0x00,///< 27  CAS Latencies supported (Fourth Byte):
  0x00,///< 28  CAS Latencies supported (Fifth Byte) :
  0x00,///< 29  Reserved
  0x1E,///< 30  Minimum CAS Latency (tAAmin) LSB
  0x41,///< 31  Minimum CAS Latency (tAAmin) MSB
  0x1E,///< 32  Minimum RAS-to-CAS delay (tRCDmin) LSB
  0x41,///< 33  Minimum RAS-to-CAS delay (tRCDmin) MSB
  0x1E,///< 34  Minimum Row Precharge delay (tRPmin) LSB
  0x41,///< 35  Minimum Row Precharge delay (tRPmin) MSB
  0x00,///< 36  Minimum Active to Precharge delay (tRASmin) LSB
  0x7D,///< 37  Minimum Active to Precharge delay (tRASmin) MSB
  0x1E,///< 38  Minimum Active to Active/Refresh delay (tRCmin) LSB
  0xBE,///< 39  Minimum Active to Active/Refresh delay (tRCmin) MSB
  0x30,///< 40  Minimum Write Recovery time (tWRmin) LSB
  0x75,///< 41  Minimum Write Recovery time (tWRmin) MSB
  0x27,///< 42  Refresh Recovery Delay (tRFC1min) LSB
  0x01,///< 43  Refresh Recovery Delay (tRFC1min) MSB
  0xA0,///< 44  Refresh Recovery Delay (tRFC2min) MSB
  0x00,///< 45  Refresh Recovery Delay (tRFC2min) MSB
  0x82,///< 46  Refresh Recovery Delay (tRFCsbmin) MSB
  0x00,///< 47  Refresh Recovery Delay (tRFCsbmin) MSB
  0,  0, ///< 48 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 60 - 69
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 70 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 110 - 119
  0, 0, 0, 0, 0, 0, ///< 120 - 125
  0x47, ///< 126 CRC Bytes 0 - 127 LSB
  0xAE, ///< 127 CRC Bytes 0 - 127 MSB
  0, 0, ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 180 - 189
  0, 0, ///< 190 - 191
  0x08, ///< 192 SPD Revision for Module Information: 0.8
  0x00, ///< 193 Reserved
  0xC2, ///< 194 SPD Manufacturer ID First Byte
  0xC4, ///< 195 SPD Manufacturer ID Second Byte
  0x80, ///< 196 SPD Device Type
  0x00, ///< 197 SPD Device Revision
  0x80, ///< 198 PMIC0 Manufacturer ID First Byte
  0xB3, ///< 199 PMIC0 Manufacturer ID Second Byte
  0x80, ///< 200 PMIC0 Device Type
  0x11, ///< 201 PMIC0 Device Revision
  0, 0, 0, 0, ///< 202 - 205 PMIC1
  0, 0, 0, 0, ///< 206 - 209 PMIC2
  0x80, ///< 210 Thermal Sensors Manufacturer ID First Byte
  0xB3, ///< 211 Thermal Sensors Manufacturer ID First Byte
  0x80, ///< 212 Thermal Sensors Device Type
  0x11, ///< 213 Thermal Sensors Device Revision
  0, 0, 0, 0, 0, 0, ///< 214 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 220 - 229
  0x0F, ///< 230 Module Nominal Height
  0x10, ///< 231 Module Nominal Thickness
  0x00, ///< 232 Reference Raw Card Used
  0x01, ///< 233 1 Row of DRAM on Module
  0x08, ///< 234 Module Organization: 2 Package Ranks; symmetrical rank mix
  0x22, ///< 235 2 Channels per DIMM, 32 bits per Channel
  0, 0, 0, 0, ///< 236 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 310 - 319
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 320 - 329
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 330 - 339
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 340 - 349
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 350 - 359
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 360 - 369
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 370 - 379
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 380 - 389
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 390 - 399
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 400 - 409
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 410 - 419
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 420 - 429
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 430 - 439
  0x00,  0x00,  0x00,  0x00,  0x00,  0x00, ///< 440 - 445
  0x9C, ///< 446 CRC for Bytes 128 - 253 LSB
  0xAD, ///< 447 CRC for Bytes 128 - 253 MSB
  0, 0, ///< 448 - 449
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 450 - 459
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 460 - 469
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 470 - 479
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 480 - 489
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 490 - 499
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ///< 500 - 509
  0, 0, ///< 510 - 511
  0x80, ///< 512 Module Manufacturer ID First Byte
  0xAD, ///< 513 Module Manufacturer ID Second Byte
  0x00, ///< 514 Module Manufacturing Location
  0, 0, ///< 515 - 516 Module Manufacturing Date
  0, 0, 0, 0, ///< 517 - 520 Module Serial Number
  0x54,  0x4D,  0x43,  0x41,  0x32,  0x47,  0x53,  0x36,  0x41,  0x4A, ///< 521 - 530
  0x52,  0x38,  0x43,  0x2D,  0x45,  0x42,  0x20,  0x20,  0x20,  0x20, ///< 531 - 540
  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20, ///< 541 - 550
  0x00, ///< 551 Module Revision Code
  0x80, ///< 552 DRAM Manufacturer ID First Byte
  0xAD  ///< 553 DRAM Manufacturer ID Second Byte
        ///< 554 - 1023 are zeroes, no need to keep them, save PEI FV size
};
