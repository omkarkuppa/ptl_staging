/** @file
  Policy definition of Memory Config Block
  Used by projects: PantherLake

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
#ifndef _MEMORY_CONFIG_H_
#define _MEMORY_CONFIG_H_

#include <ConfigBlock.h>
#include "Ptl/CMrcExtInputs.h"


#pragma pack(push, 1)

// MEMORY_CONFIGURATION_REVISION:
// Keep in sync with MRC_EXT_INPUTS_REVISION in MrcExtInputs.h
#define MEMORY_CONFIGURATION_REVISION 1
#define MEMORY_CONFIG_NO_CRC_REVISION 1

///
/// Memory SubSystem Definitions
///
#define MEM_CFG_MAX_CONTROLLERS          MAX_CONTROLLER
#define MEM_CFG_MAX_CHANNELS             MAX_CHANNEL
#define MEM_CFG_MAX_CHANNEL_SHARE_REGS   2
#define MEM_CFG_MAX_DIMMS                EXT_MAX_DIMMS_IN_CHANNEL
#define MEM_CFG_MAX_RANKS_PER_DIMM       MAX_RANK_IN_DIMM
#define MEM_CFG_NUM_BYTES_MAPPED         2
#define MEM_CFG_MAX_SPD_SIZE             1024
#define MEM_CFG_MAX_SOCKETS              (MEM_CFG_MAX_CONTROLLERS * MEM_CFG_MAX_CHANNELS * MEM_CFG_MAX_DIMMS)
#define MEM_CFG_MAX_ROWS                 (MEM_CFG_MAX_RANKS_PER_DIMM * MEM_CFG_MAX_SOCKETS)
#ifndef MEM_MAX_SAGV_POINTS
#define MEM_MAX_SAGV_POINTS              MAX_SAGV_POINTS
#endif
#define MEM_MAX_IBECC_REGIONS            8
///
/// SMRAM Memory Range
///
#define PEI_MR_SMRAM_ABSEG_MASK     0x01
#define PEI_MR_SMRAM_HSEG_MASK      0x02

///
/// SA SPD profile selections.
///
typedef enum {
  Default,             ///< 0, Default SPD
  UserDefined,         ///< 1, User Defined profile
  XMPProfile1,         ///< 2, XMP Profile 1
  XMPProfile2,         ///< 3, XMP Profile 2
  XMPProfileMax = 0xFF ///< Ensures SA_SPD is UINT8
} SA_SPD;

///
/// Define the boot modes used by the SPD read function.
///
typedef enum {
  SpdCold,       ///< Cold boot
  SpdWarm,       ///< Warm boot
  SpdS3,         ///< S3 resume
  SpdFast,       ///< Fast boot
  SpdBootModeMax ///< Delimiter
} SPD_BOOT_MODE;

/**
  SPD Data Buffer
**/
typedef struct {
  UINT8 SpdData[MEM_CFG_MAX_CONTROLLERS][MEM_CFG_MAX_CHANNELS][MEM_CFG_MAX_DIMMS][MEM_CFG_MAX_SPD_SIZE];  ///< SpdData
//Next Field Offset 2048
} SPD_DATA_BUFFER;

/**
  DqDqs Mapping
**/
typedef struct {
  UINT8 DqsMapCpu2Dram[MEM_CFG_MAX_CONTROLLERS][MEM_CFG_MAX_CHANNELS][MEM_CFG_NUM_BYTES_MAPPED];  ///< DqsMapCpu2Dram
  UINT8 DqMapCpu2Dram[MEM_CFG_MAX_CONTROLLERS][MEM_CFG_MAX_CHANNELS][MEM_CFG_NUM_BYTES_MAPPED][8];  ///< DqMapCpu2Dram
//Next Field Offset 16
} MEMORY_DQDQS_MAPPING;

/**
  Rcomp Policies
**/
typedef struct {
  UINT16  RcompResistor;                      ///< Offset 0: Reference RCOMP resistors on motherboard ~ 100 ohms
  UINT16  RcompTarget[MAX_RCOMP_TARGETS];     ///< Offset 1: RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
//Next Field Offset 16
} MEMORY_RCOMP;

/**
  SPD Offset Table
**/
typedef struct {
  UINT16 Start;           ///< Offset 0
  UINT16 End;             ///< Offset 2
  UINT8  BootMode;        ///< Offset 4
  UINT8  Reserved3[3];    ///< Offset 5 Reserved for future use
} SPD_OFFSET_TABLE;

///
/// SA memory address decode.
///
typedef struct
{
  UINT8  Controller; ///< Offset 0 Zero based Controller number
  UINT8  Channel;    ///< Offset 1 Zero based Channel number
  UINT8  Dimm;       ///< Offset 2 Zero based DIMM number
  UINT8  Rank;       ///< Offset 3 Zero based Rank number
  UINT8  BankGroup;  ///< Offset 4 Zero based Bank Group number
  UINT8  Bank;       ///< Offset 5 Zero based Bank number
  UINT16 Cas;        ///< Offset 6 Zero based CAS number
  UINT32 Ras;        ///< Offset 8 Zero based RAS number
} SA_ADDRESS_DECODE;

typedef UINT8      (EFIAPI * SA_IO_READ_8)               (UINTN IoAddress);                                                                                                                                                                               ///< CPU I/O port 8-bit read.
typedef UINT16     (EFIAPI * SA_IO_READ_16)              (UINTN IoAddress);                                                                                                                                                                               ///< CPU I/O port 16-bit read.
typedef UINT32     (EFIAPI * SA_IO_READ_32)              (UINTN IoAddress);
typedef UINT8      (EFIAPI * SA_IO_WRITE_8)              (UINTN IoAddress, UINT8 Value);                                                                                                                                                                  ///< CPU I/O port 8-bit write.
typedef UINT16     (EFIAPI * SA_IO_WRITE_16)             (UINTN IoAddress, UINT16 Value);                                                                                                                                                                 ///< CPU I/O port 16-bit write.
typedef UINT32     (EFIAPI * SA_IO_WRITE_32)             (UINTN IoAddress, UINT32 Value);                                                                                                                                                                 ///< CPU I/O port 32-bit write.
typedef UINT8      (EFIAPI * SA_MMIO_READ_8)             (UINTN Address);                                                                                                                                                                                 ///< Memory Mapped I/O port 8-bit read.
typedef UINT16     (EFIAPI * SA_MMIO_READ_16)            (UINTN Address);                                                                                                                                                                                 ///< Memory Mapped I/O port 16-bit read.
typedef UINT32     (EFIAPI * SA_MMIO_READ_32)            (UINTN Address);                                                                                                                                                                                 ///< Memory Mapped I/O port 32-bit read.
typedef UINT64     (EFIAPI * SA_MMIO_READ_64)            (UINTN Address);                                                                                                                                                                                 ///< Memory Mapped I/O port 64-bit read.
typedef UINT8      (EFIAPI * SA_MMIO_WRITE_8)            (UINTN Address, UINT8 Value);                                                                                                                                                                    ///< Memory Mapped I/O port 8-bit write.
typedef UINT16     (EFIAPI * SA_MMIO_WRITE_16)           (UINTN Address, UINT16 Value);                                                                                                                                                                   ///< Memory Mapped I/O port 16-bit write.
typedef UINT32     (EFIAPI * SA_MMIO_WRITE_32)           (UINTN Address, UINT32 Value);                                                                                                                                                                   ///< Memory Mapped I/O port 32-bit write.
typedef UINT64     (EFIAPI * SA_MMIO_WRITE_64)           (UINTN Address, UINT64 Value);                                                                                                                                                                   ///< Memory Mapped I/O port 64-bit write.
typedef UINT8      (EFIAPI * SA_SMBUS_READ_8)            (UINTN Address, RETURN_STATUS *Status);                                                                                                                                                          ///< Smbus 8-bit read.
typedef UINT16     (EFIAPI * SA_SMBUS_READ_16)           (UINTN Address, RETURN_STATUS *Status);                                                                                                                                                          ///< Smbus 16-bit read.
typedef UINT8      (EFIAPI * SA_SMBUS_WRITE_8)           (UINTN Address, UINT8 Value, RETURN_STATUS *Status);                                                                                                                                             ///< Smbus 8-bit write.
typedef UINT16     (EFIAPI * SA_SMBUS_WRITE_16)          (UINTN Address, UINT16 Value, RETURN_STATUS *Status);                                                                                                                                            ///< Smbus 16-bit write.
typedef UINT32     (EFIAPI * SA_GET_PCI_DEVICE_ADDRESS)  (UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 Offset);                                                                                                                                         ///< Get PCI device address.
typedef UINT32     (EFIAPI * SA_GET_PCIE_DEVICE_ADDRESS) (UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 Offset);                                                                                                                                         ///< Get PCI express device address.
typedef VOID       (EFIAPI * SA_GET_RTC_TIME)            (UINT8 *Second, UINT8 *Minute, UINT8 *Hour, UINT8 *Day, UINT8 *Month, UINT16 *Year);                                                                                                             ///< Get the current time value.
typedef UINT64     (EFIAPI * SA_GET_CPU_TIME)            (VOID);                                                                                                                                                                                          ///< The current CPU time in milliseconds.
typedef UINT32     (EFIAPI * SA_GET_CPU_TIME_US)         (void *MrcData);
typedef VOID *     (EFIAPI * SA_MEMORY_COPY)             (VOID *Destination, CONST VOID *Source, UINTN NumBytes);                                                                                                                                         ///< Perform byte copy operation.
typedef VOID *     (EFIAPI * SA_MEMORY_SET_BYTE)         (VOID *Buffer, UINTN NumBytes, UINT8 Value);                                                                                                                                                     ///< Perform byte initialization operation.
typedef VOID *     (EFIAPI * SA_MEMORY_SET_WORD)         (VOID *Buffer, UINTN NumWords, UINT16 Value);                                                                                                                                                    ///< Perform word initialization operation.
typedef VOID *     (EFIAPI * SA_MEMORY_SET_DWORD)        (VOID *Buffer, UINTN NumDwords, UINT32 Value);                                                                                                                                                   ///< Perform dword initialization operation.
typedef UINT64     (EFIAPI * SA_LEFT_SHIFT_64)           (UINT64 Data, UINTN NumBits);                                                                                                                                                                    ///< Left shift the 64-bit data value by specified number of bits.
typedef UINT64     (EFIAPI * SA_RIGHT_SHIFT_64)          (UINT64 Data, UINTN NumBits);                                                                                                                                                                    ///< Right shift the 64-bit data value by specified number of bits.
typedef UINT64     (EFIAPI * SA_MULT_U64_U32)            (UINT64 Multiplicand, UINT32 Multiplier);                                                                                                                                                        ///< Multiply a 64-bit data value by a 32-bit data value.
typedef UINT64     (EFIAPI * SA_DIV_U64_U64)             (UINT64 Dividend, UINT64 Divisor, UINT64 *Remainder);                                                                                                                                            ///< Divide a 64-bit data value by a 64-bit data value.
typedef BOOLEAN    (EFIAPI * SA_GET_SPD_DATA)            (SPD_BOOT_MODE BootMode, UINT8 SpdAddress, UINT8 *Buffer, UINT8 *Ddr5Table, UINT32 Ddr5TableSize, UINT8 *LpddrTable, UINT32 LpddrTableSize);                                                     ///< Read the SPD data over the SMBus, at the given SmBus SPD address and copy the data to the data structure.
typedef UINT8      (EFIAPI * SA_GET_MC_ADDRESS_DECODE)   (UINT64 Address, SA_ADDRESS_DECODE *DramAddress);
typedef UINT8      (EFIAPI * SA_GET_MC_ADDRESS_ENCODE)   (SA_ADDRESS_DECODE *DramAddress, UINT64 Address);
typedef BOOLEAN    (EFIAPI * SA_GET_RANDOM_NUMBER)       (UINT32 *Rand);                                                                                                                                                                                  ///< Get the next random 32-bit number.
typedef EFI_STATUS (EFIAPI * SA_CPU_MAILBOX_READ)        (UINT32 Command, UINT32 *Value, UINT32 *Status);                                                                                                                                    ///< Perform a CPU mailbox read.
typedef EFI_STATUS (EFIAPI * SA_CPU_MAILBOX_WRITE)       (UINT32 Command, UINT32 Value, UINT32 *Status);                                                                                                                                     ///< Perform a CPU mailbox write.
typedef UINT32     (EFIAPI * SA_GET_MEMORY_VDD)          (VOID *GlobalData, UINT32 DefaultVdd);                                                                                                                                                           ///< Get the current memory voltage (VDD).
typedef UINT32     (EFIAPI * SA_SET_MEMORY_VDD)          (VOID *GlobalData, UINT32 DefaultVdd, UINT32 Value);                                                                                                                                             ///< Set the memory voltage (VDD) to the given value.
typedef UINT32     (EFIAPI * SA_CHECKPOINT)              (VOID *GlobalData, UINT32 CheckPoint, VOID *Scratch);                                                                                                                                            ///< Check point that is called at various points in the MRC.
typedef VOID       (EFIAPI * SA_DEBUG_HOOK)              (VOID *GlobalData, UINT16 DisplayDebugNumber);                                                                                                                                                   ///< Typically used to display to the I/O port 80h.
typedef UINT8      (EFIAPI * SA_CHANNEL_EXIST)           (VOID *Outputs, UINT8 Channel);                                                                                                                                                                  ///< Returns whether Channel is or is not present.
typedef INT32      (EFIAPI * SA_PRINTF)                  (VOID *Debug, UINT32 Level, char *Format, ...);                                                                                                                                                  ///< Print to output stream/device.
typedef VOID       (EFIAPI * SA_DEBUG_PRINT)             (VOID *String);                                                                                                                                                                                  ///< Output a string to the debug stream/device.
typedef UINT32     (EFIAPI * SA_CHANGE_MARGIN)           (VOID *GlobalData, UINT8 Param, INT32 Value0, INT32 Value1, UINT8 EnMultiCast, UINT8 Channel, UINT8 RankIn, UINT8 Byte, UINT8 BitIn, UINT8 UpdateMrcData, UINT8 SkipWait, UINT32 RegFileParam);  ///< Change the margin.
typedef UINT8      (EFIAPI * SA_SIGN_EXTEND)             (UINT8 Value, UINT8 OldMsb, UINT8 NewMsb);                                                                                                                                                       ///< Sign extends OldMSB to NewMSB Bits (Eg: Bit 6 to Bit 7).
typedef VOID       (EFIAPI * SA_SHIFT_PI_COMMAND_TRAIN)  (VOID *GlobalData, UINT8 Channel, UINT8 Iteration, UINT8 RankMask, UINT8 GroupMask, INT32 NewValue, UINT8 UpdateHost);                                                                           ///< Move CMD/CTL/CLK/CKE PIs during training.
typedef VOID       (EFIAPI * SA_UPDATE_VREF)             (VOID *GlobalData, UINT8 Channel, UINT8 RankMask, UINT16 DeviceMask, UINT8 VrefType, INT32 Offset, BOOLEAN UpdateMrcData, BOOLEAN PDAmode, BOOLEAN SkipWait);                                    ///< Update the Vref value and wait until it is stable.
typedef UINT8      (EFIAPI * SA_GET_RTC_CMOS)            (UINT8 Location);                                                                                                                                                                                ///< Get the current value of the specified RTC CMOS location.
typedef UINT64     (EFIAPI * SA_MSR_READ_64)             (UINT32 Location);                                                                                                                                                                               ///< Get the current value of the specified MSR location.
typedef UINT64     (EFIAPI * SA_MSR_WRITE_64)            (UINT32 Location, UINT64 Data);                                                                                                                                                                  ///< Set the current value of the specified MSR location.
typedef VOID       (EFIAPI * SA_MRC_RETURN_FROM_SMC)     (VOID *GlobalData, UINT32 MrcStatus);                                                                                                                                                            ///< Hook function after returning from MrcStartMemoryConfiguration()
typedef VOID       (EFIAPI * SA_MRC_DRAM_RESET)          (UINT32 PciEBaseAddress, UINT32 ResetValue);                                                                                                                                                     ///< Assert or deassert DRAM_RESET# pin; this is used in JEDEC Reset.
typedef VOID       (EFIAPI * SA_DELAY_NS)                (VOID *GlobalData, UINT32 DelayNs);                                                                                                                                                              ///< Delay (stall) for the given amount of nanoseconds.
typedef VOID       (EFIAPI * SA_SET_LOCK_PRMRR)          (UINT64 PrmrrBaseAddress, UINT32 PrmrrSize);


///
/// Function calls into the SA.
///
typedef struct {
  SA_IO_READ_8               IoRead8;               ///< Offset 0:   - CPU I/O port 8-bit read.
  SA_IO_READ_16              IoRead16;              ///< Offset 4:   - CPU I/O port 16-bit read.
  SA_IO_READ_32              IoRead32;              ///< Offset 8:   - CPU I/O port 32-bit read.
  SA_IO_WRITE_8              IoWrite8;              ///< Offset 12:  - CPU I/O port 8-bit write.
  SA_IO_WRITE_16             IoWrite16;             ///< Offset 16:  - CPU I/O port 16-bit write.
  SA_IO_WRITE_32             IoWrite32;             ///< Offset 20:  - CPU I/O port 32-bit write.
  SA_MMIO_READ_8             MmioRead8;             ///< Offset 24:  - Memory Mapped I/O port 8-bit read.
  SA_MMIO_READ_16            MmioRead16;            ///< Offset 28:  - Memory Mapped I/O port 16-bit read.
  SA_MMIO_READ_32            MmioRead32;            ///< Offset 32:  - Memory Mapped I/O port 32-bit read.
  SA_MMIO_READ_64            MmioRead64;            ///< Offset 36:  - Memory Mapped I/O port 64-bit read.
  SA_MMIO_WRITE_8            MmioWrite8;            ///< Offset 40:  - Memory Mapped I/O port 8-bit write.
  SA_MMIO_WRITE_16           MmioWrite16;           ///< Offset 44:  - Memory Mapped I/O port 16-bit write.
  SA_MMIO_WRITE_32           MmioWrite32;           ///< Offset 48:  - Memory Mapped I/O port 32-bit write.
  SA_MMIO_WRITE_64           MmioWrite64;           ///< Offset 52:  - Memory Mapped I/O port 64-bit write.
  SA_SMBUS_READ_8            SmbusRead8;            ///< Offset 56:  - Smbus 8-bit read.
  SA_SMBUS_READ_16           SmbusRead16;           ///< Offset 60:  - Smbus 16-bit read.
  SA_SMBUS_WRITE_8           SmbusWrite8;           ///< Offset 64:  - Smbus 8-bit write.
  SA_SMBUS_WRITE_16          SmbusWrite16;          ///< Offset 68:  - Smbus 16-bit write.
  SA_GET_PCI_DEVICE_ADDRESS  GetPciDeviceAddress;   ///< Offset 72:  - Get PCI device address.
  SA_GET_PCIE_DEVICE_ADDRESS GetPcieDeviceAddress;  ///< Offset 76:  - Get PCI express device address.
  SA_GET_RTC_TIME            GetRtcTime;            ///< Offset 80:  - Get the current time value.
  SA_GET_CPU_TIME            GetCpuTime;            ///< Offset 84:  - The current CPU time in milliseconds.
  SA_GET_CPU_TIME_US         MrcGetCpuTimeUs;
  SA_MEMORY_COPY             CopyMem;               ///< Offset 88:  - Perform byte copy operation.
  SA_MEMORY_SET_BYTE         SetMem;                ///< Offset 92:  - Perform byte initialization operation.
  SA_MEMORY_SET_WORD         SetMemWord;            ///< Offset 96:  - Perform word initialization operation.
  SA_MEMORY_SET_DWORD        SetMemDword;           ///< Offset 100: - Perform dword initialization operation.
  SA_LEFT_SHIFT_64           LeftShift64;           ///< Offset 104: - Left shift the 64-bit data value by specified number of bits.
  SA_RIGHT_SHIFT_64          RightShift64;          ///< Offset 108: - Right shift the 64-bit data value by specified number of bits.
  SA_MULT_U64_U32            MultU64x32;            ///< Offset 112: - Multiply a 64-bit data value by a 32-bit data value.
  SA_DIV_U64_U64             DivU64x64;             ///< Offset 116: - Divide a 64-bit data value by a 64-bit data value.
  SA_GET_SPD_DATA            GetSpdData;            ///< Offset 120: - Read the SPD data over the SMBus, at the given SmBus SPD address and copy the data to the data structure.
  SA_GET_RANDOM_NUMBER       GetRandomNumber;       ///< Offset 124: - Get the next random 32-bit number.
  SA_CPU_MAILBOX_READ        CpuMailboxRead;        ///< Offset 128: - Perform a CPU mailbox read.
  SA_CPU_MAILBOX_WRITE       CpuMailboxWrite;       ///< Offset 132: - Perform a CPU mailbox write.
  SA_CHECKPOINT              CheckPoint;            ///< Offset 136: - Check point that is called at various points in the MRC.
  SA_DEBUG_HOOK              DebugHook;             ///< Offset 140: - Typically used to display to the I/O port 80h.
  SA_DEBUG_PRINT             DebugPrint;            ///< Offset 144: - Output a string to the debug stream/device.
  SA_GET_RTC_CMOS            GetRtcCmos;            ///< Offset 148: - Get the current value of the specified RTC CMOS location.
  SA_MSR_READ_64             ReadMsr64;             ///< Offset 152: - Get the current value of the specified MSR location.
  SA_MSR_WRITE_64            WriteMsr64;            ///< Offset 156  - Set the current value of the specified MSR location.
  SA_MRC_RETURN_FROM_SMC     MrcReturnFromSmc;      ///< Offset 160  - Hook function after returning from MrcStartMemoryConfiguration()
  SA_DELAY_NS                MrcDelayNs;            ///< Offset 164  - Delay (stall) for the given amount of nanoseconds.
} SA_FUNCTION_CALLS;

///
/// Function calls into the MRC.
///
typedef struct {
  SA_CHANNEL_EXIST           MrcChannelExist;       ///< Offset 0:  - Returns whether Channel is or is not present.
  SA_PRINTF                  MrcPrintf;             ///< Offset 4:  - Print to output stream/device.
  SA_CHANGE_MARGIN           MrcChangeMargin;       ///< Offset 8:  - Change the margin.
  SA_SIGN_EXTEND             MrcSignExtend;         ///< Offset 12: - Sign extends OldMSB to NewMSB Bits (Eg: Bit 6 to Bit 7).
  SA_SHIFT_PI_COMMAND_TRAIN  ShiftPiCommandTrain;   ///< Offset 16: - Move CMD/CTL/CLK/CKE PIs during training.
  SA_UPDATE_VREF             MrcUpdateVref;         ///< Offset 20: - Update the Vref value and wait until it is stable.
} SA_MEMORY_FUNCTIONS;

/**
 Memory Configuration
 The contents of this structure are CRC'd by the MRC for option change detection.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;    ///< Offset 0-27 Config Block Header
  MRC_EXT_INPUTS_TYPE  ExternalInputs;
} MEMORY_CONFIGURATION;

/// Memory Configuration
/// The contents of this structure are not CRC'd by the MRC for option change detection.
/// <b>Revision 1</b>:  - Initial version.
typedef struct {
  CONFIG_BLOCK_HEADER      Header;              ///< Offset 0-23 Config Block Header
  SA_FUNCTION_CALLS        SaCall;              ///< Offset 24   Function calls into the SA.
  SA_MEMORY_FUNCTIONS      MrcCall;             ///< Offset 204  Function calls into the MRC.
  SPD_DATA_BUFFER          *SpdData;            ///< Offset 240  Memory SPD data, will be used by the MRC when SPD SmBus address is zero.
  UINT32                   Reserved0;
  MEMORY_DQDQS_MAPPING     *DqDqsMap;           ///< Offset 244  LPDDR DQ bit and DQS byte swizzling between CPU and DRAM.
  MEMORY_RCOMP             *RcompData;          ///< Offset 248  DDR RCOMP resistors and target values.
  UINT64                   PlatformMemorySize;  ///< Offset 252  The minimum platform memory size required to pass control into DXE
  UINT32                   CleanMemory:1;       ///< Offset 256  Ask MRC to clear memory content: <b>FALSE=Do not Clear Memory</b>; TRUE=Clear Memory
  UINT32                   ReservedBits5:31;
  /**
   Sets the serial debug message level\n
     0x00 = Disabled\n
     0x01 = Errors only\n
     0x02 = Errors and Warnings\n
     <b>0x03 = Errors, Warnings, and Info</b>\n
     0x04 = Errors, Warnings, Info, and Events\n
     0x05 = Displays Memory Init Execution Time Summary only\n
  **/
  UINT8   SerialDebugLevel;                     ///< Offset 260
  UINT8   MemTestOnWarmBoot;                    ///< Offset 261  Run Base Memory Test On WarmBoot:  0=Disabled, <b>1=Enabled</b>
  UINT8   DisableMrcRetraining;                 ///< Offset 262 Enable/Disable DisableMrcRetraining.
  /**
    Offset 263 Memory DIMMs' SPD address for reading SPD data.
    TGL Mapping
      0 - Controller 0 Channel 0 Dimm 0 - DDR4 - DDR5 - LPDDR4 - LPDDR5
      1 - Controller 0 Channel 0 Dimm 1 - DDR4
      2 - Controller 0 Channel 1 Dimm 0 -------- DDR5 - LPDDR4 - LPDDR5
      3 - Controller 0 Channel 1 Dimm 1 -------- DDR5 2DPC
      4 - Controller 0 Channel 2 Dimm 0 --------------- LPDDR4 - LPDDR5
      6 - Controller 0 Channel 3 Dimm 0 --------------- LPDDR4 - LPDDR5
      8 - Controller 1 Channel 0 Dimm 0 - DDR4 - DDR5 - LPDDR4 - LPDDR5
      9 - Controller 1 Channel 0 Dimm 1 - DDR4
     10 - Controller 1 Channel 1 Dimm 0 -------- DDR5 - LPDDR4 - LPDDR5
     11 - Controller 1 Channel 1 Dimm 1 -------- DDR5 2DPC
     12 - Controller 1 Channel 2 Dimm 0 --------------- LPDDR4 - LPDDR5
     14 - Controller 1 Channel 3 Dimm 0 --------------- LPDDR4 - LPDDR5
  **/
  UINT8   SpdAddressTable[MEM_CFG_MAX_SOCKETS];
  UINT8   MrcBdatEnable;                     ///< Offset 279 This field enables the generation of the BIOS DATA ACPI Tables: <b>0=FALSE</b>, 1=TRUE.
  UINT8   MrcBdatTestType;                   ///< Offset 280 When BdatEnable is set to TRUE, this option selects the type of data which will be populated in the BIOS Data ACPI Tables: <b>0=RMT</b>, 1=RMT Per Bit, 2=Margin 2D.

  UINT8   SafeLoadingBiosEnableState;        ///< Offset 281  Safe Loading Bios Enable State
  UINT8   PprRecoveryStatusEnable;           ///< Offset 282  Ppr Recovery Status Enable
  UINT8   TsegMemoryTestStatus;              ///< Offset 283  Tseg Memory Test Status
  UINT8   MrcPprStatus;                      ///< Offset 284  Mrc Ppr Status
  UINT8   RetryCount;                        ///< Offset 285  TSEG RetryCount
  UINT8   Rsvd[2];                           ///< Offset 286 Reserved for future use.
} MEMORY_CONFIG_NO_CRC;
#pragma pack(pop)

#endif // _MEMORY_CONFIG_H_
