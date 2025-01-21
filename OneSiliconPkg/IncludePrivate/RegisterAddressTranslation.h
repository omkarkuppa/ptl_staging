/** @file
  Common definitions for Register Address Translation

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

#ifndef __REGISTER_ADDRESS_TRANSLATION__
#define __REGISTER_ADDRESS_TRANSLATION__

#include <PiPei.h>

#ifndef MAX_SOCKET
#define MAX_SOCKET (FixedPcdGet32 (PcdMaxCpuSocketCount))
#endif

#define RAT_TOPOLOGY_VERSION_MAJOR 0x2
#define RAT_TOPOLOGY_VERSION_MINOR 0x1
#define RAT_TOPOLOGY_VERSION ((RAT_TOPOLOGY_VERSION_MAJOR << 8) | RAT_TOPOLOGY_VERSION_MINOR)

#define RAT_MAX_MAP_TABLE_SIZE    0x40000

#define RAT_INVALID_BASE(Value)                                   \
  (((Value) == (UINT64)-1) ||                                     \
   ((Value) & 0xFFFFFFFF00000000ull) == 0xFFFFFFFF00000000ull ||  \
   ((Value) & 0x00000000FFFFFFFFull) == 0x00000000FFFFFFFFull)

#pragma pack(1)

///
/// Register access type (read, write)
///
typedef enum {
  RegisterAccessRead = 0,
  RegisterAccessWrite,
  RegisterAccessInvalid
} REGISTER_ACCESS_RW;

///
/// Register width (byte, word, dword, qword)
///
typedef enum {
  RegisterWidthByte = 0,
  RegisterWidthWord,
  RegisterWidthDWord,
  RegisterWidthQWord
} REGISTER_WIDTH_BYTES;

///
/// Register space types (refer to IRS - Intel Register Specification)
///
typedef enum {
  RegisterSpaceMemFixed = 0,  // IRS: MEM (MMIO via fixed address)
  RegisterSpaceLtMem,         // IRS: LT_MEM (TBD, a kind of fixed MMIO)
  RegisterSpaceCfg,           // IRS: CFG (PCI configuration space)
  RegisterSpaceMemBar,        // IRS: MEM (MMIO via BAR register)
  RegisterSpaceIoBar,         // IRS: IO (PCI IO via BAR register)
  RegisterSpaceIoFixed,       // IRS: IO (PCI IO via fixed address)
  RegisterSpaceMsgCfg,        // IRS: MSG,Opcode=CFG-SB (PCI configuration space via IOSF_Sideband)
  RegisterSpaceMsgMem,        // IRS: MSG,Opcode=MEM-SB (MMIO via IOSF_Sideband)
  RegisterSpaceMsgIo,         // IRS: MSG,Opcode=IO-SB (PCI IO via IOSF_Sideband)
  RegisterSpaceMsgCr,         // IRS: MSG,Opcode=CR (Private CR via IOSF_Sideband)
  RegisterSpaceMaxType,
  RegisterSpaceAny
} REGISTER_SPACE_TYPE;

#define SPECIAL_BAR_TYPE_EXPPTM       0x8000000000000000ull
#define SPECIAL_BAR_TYPE_RCRB         0x4000000000000000ull
#define SPECIAL_BAR_TYPE_CXLRB        0x2000000000000000ull

typedef struct {
  UINT16  Type;
  UINT16  Instances;
} ACCESSOR_HEADER;

typedef struct {
  UINT32 Offset:12;
  UINT32 Function:3;
  UINT32 Device:5;
  UINT32 Bus:8;
  UINT32 Reserved:4;
} PCI_BDFO;

typedef union {
  PCI_BDFO  Bdfo;
  UINT32    Address;
} PCI_CFG_ACCESSOR;

typedef struct {
  UINT32 Address;
} FIXED_ADDRESS_ACCESSOR;

typedef struct {
  UINT32                Offset;
  UINT16                BarIndex;
} BAR_ACCESSOR;

typedef union {
  struct {
    UINT32  PortId:16;
    UINT32  SbFid:13;
    UINT32  SbBar:3;
  } Bits;
  UINT32    Address;
} SIDEBAND_ACCESSOR;

typedef union {
  PCI_CFG_ACCESSOR        PciCfg;
  FIXED_ADDRESS_ACCESSOR  Fixed;
  BAR_ACCESSOR            Bar;
  SIDEBAND_ACCESSOR       Sideband;
} ACCESSOR_DESCRIPTOR;

typedef struct {
  UINT64              Id;
  UINT16              AccessorTypes;
  UINT16              Reserved;
  ACCESSOR_HEADER     TypeInstances[1];
} ENDPOINT_DESCRIPTOR;

typedef struct {
  UINT16                Count;
  EFI_PHYSICAL_ADDRESS  Accessors;  // ACCESSOR_DESCRIPTOR*
} INSTANCE_INFO;

typedef struct {
  UINT64          Id;
  INSTANCE_INFO   Instances[RegisterSpaceMaxType];
} ENDPOINT_INDEX;

typedef struct {
  UINT16        BarIndex;
  UINT64        BarValue;
} BAR_CACHE;

typedef struct {
  struct {
    UINT8         Type;   // RegisterSpaceCfg or RegisterSpaceMsgCr
    UINT8         Shift;
    UINT64        Mask;
    UINT64        Register;
  }             Bar[2];
} COMBINED_BAR;

typedef struct {
  UINT64                PreviousId;
  EFI_PHYSICAL_ADDRESS  PreviousBase;
  UINT16                PreviousInstance;
  UINT16                PreviousIndex;
  UINT8                 PreviousAccessType;
} LOOKUP_CACHE;

#define TOTAL_MAPPABLE_BUSSES 256

typedef struct {
  EFI_PHYSICAL_ADDRESS  MmcfgBase;
  EFI_PHYSICAL_ADDRESS  MapTable;         // SOC_MAP_HEADER*
  EFI_PHYSICAL_ADDRESS  EndpointIndices;  // ENDPOINT_INDEX*
  EFI_PHYSICAL_ADDRESS  BarValueCache;
  LOOKUP_CACHE          LookupCache;
  BOOLEAN               PostedWrite;
  UINT8                 BusMap[TOTAL_MAPPABLE_BUSSES];
} PER_SOCKET_DATA;

typedef struct {
  UINT16                StructureVersion;
  UINT16                SocketCount;
  PER_SOCKET_DATA       PerSocketData[MAX_SOCKET];
  EFI_PHYSICAL_ADDRESS  DefaultMapTable;        // SOC_MAP_HEADER*
  EFI_PHYSICAL_ADDRESS  DefaultEndpointIndices; // ENDPOINT_INDEX*
  EFI_PHYSICAL_ADDRESS  Ppi;                    // For PEI code only
  EFI_PHYSICAL_ADDRESS  MapStringTable;         // CHAR8*[EpCount]
  UINT8                 StringTableUnit;
} SYSTEM_TOPOLOGY;

typedef struct {
  UINT64          HardwareId;   // SoC identification
  UINT32          Release;      // OS-XML release
  UINT32          Length;       // SoC Map Table length, including this header
  UINT32          Checksum;     // Checksum of whole table
  UINT16          Version;      // Version of SoC Register Address Map Table format
  UINT16          EpCount;      // Number of endpoints
  UINT16          BarCount;     // Number of BARs
  //
  // COMBINED_BAR * BarCount
  //
  // ENDPOINT_DESCRIPTOR * EpCount
  //
} SOC_MAP_HEADER;

#pragma pack()

/**
  Reads the value of the requested register within given endpoint instance.

  @param[in] Socket     Socket ID or index
  @param[in] Id         Endpoint ID within SoC
  @param[in] Instance   Instance of endpoint given by Id
  @param[in] Type       Register access space type
  @param[in] Register   Register to read from
  @param[in] Size       Byte width of register

  @retval             - Value read from register
**/
typedef
UINT64
(EFIAPI *RAT_REG_READ) (
  IN UINT8                Socket,
  IN UINT64               Id,
  IN UINT16               Instance,
  IN REGISTER_SPACE_TYPE  Type,
  IN UINT32               Register,
  IN REGISTER_WIDTH_BYTES Size
  );

/**
  Writes a value to the requested register within given endpoint instance.

  @param[in] Socket     Socket ID or index
  @param[in] Id         Endpoint ID within SoC
  @param[in] Instance   Instance of endpoint given by Id
  @param[in] Type       Register access space type
  @param[in] Register   Register to write to
  @param[in] Value      Value to be written
  @param[in] Size       Byte width of register

  @retval               None
**/
typedef
VOID
(EFIAPI *RAT_REG_WRITE) (
  IN UINT8                Socket,
  IN UINT64               Id,
  IN UINT16               Instance,
  IN REGISTER_SPACE_TYPE  Type,
  IN UINT32               Register,
  IN UINT64               Value,
  IN REGISTER_WIDTH_BYTES Size
  );

/**
  Returns the base address of the requested endpoint instance in given register
  access space.

  @param[in] Socket     Socket ID or index
  @param[in] Id         Endpoint ID within SoC
  @param[in] Instance   Instance of endpoint given by Id
  @param[in] Type       Type of Register access space

  @retval               NULL if register address could not be determined,
                        valid address otherwise
**/
typedef
VOID*
(EFIAPI *RAT_GET_ADDR) (
  IN UINT8                Socket,
  IN UINT64               Id,
  IN UINT16               Instance,
  IN REGISTER_SPACE_TYPE  Type
  );

///
/// This service abstracts the ability to read/write silicon register.
///
typedef struct _REG_ADDR_TRANS_INTERFACE {
  RAT_GET_ADDR            GetBaseAddr;
  RAT_REG_READ            RegRead;
  RAT_REG_WRITE           RegWrite;
} REG_ADDR_TRANS_INTERFACE;

#endif // __REGISTER_ADDRESS_TRANSLATION__

