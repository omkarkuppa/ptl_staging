/** @file
  Describes the functions visible to the rest of the BIOS Guard.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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
#ifndef _BIOSGUARD_DEFINITIONS_H_
#define _BIOSGUARD_DEFINITIONS_H_

#ifndef ALIGN_256KB
#define ALIGN_256KB    0x00040000      ///< 256KB alignment
#endif
#ifndef ALIGN_512KB
#define ALIGN_512KB    0x00080000      ///< 512KB alignment
#endif
#ifndef EFI_PAGE_SIZE
#define EFI_PAGE_SIZE  0x00001000      ///< 4KB page size
#endif

///
/// BIOS Guard Module Commands
///
#define BIOSGUARD_COMMAND_NOP              0x00      ///< NO OP.
#define BIOSGUARD_COMMAND_BEGIN            0x01      ///< BIOS Guard Begin.
#define BIOSGUARD_COMMAND_WRITE_INDEX      0x10      ///< BIOS Guard Write Index.
#define BIOSGUARD_COMMAND_WRITE_IMM        0x11      ///< BIOS Guard Write Immediate.
#define BIOSGUARD_COMMAND_READ_INDEX       0x12      ///< BIOS Guard Read Index.
#define BIOSGUARD_COMMAND_READ_IMM         0x13      ///< BIOS Guard Read Immediate.
#define BIOSGUARD_COMMAND_ERASE_BLK        0x14      ///< BIOS Guard Erase Block.
#define BIOSGUARD_COMMAND_EC_CMD_WR_INDEX  0x20      ///< EC_CMD Write Index.
#define BIOSGUARD_COMMAND_EC_CMD_WR_IMM    0x21      ///< EC_CMD Write Immediate.
#define BIOSGUARD_COMMAND_EC_STS_RD        0x22      ///< EC_CMD Read Status.
#define BIOSGUARD_COMMAND_EC_DATA_WR_INDEX 0x23      ///< EC_DATA Write Index.
#define BIOSGUARD_COMMAND_EC_DATA_WR_IMM   0x24      ///< EC_DATA Write Immediate.
#define BIOSGUARD_COMMAND_EC_DATA_RD       0x25      ///< EC_DATA Read.
#define BIOSGUARD_COMMAND_SET_BUFFER_INDEX 0x53      ///< BIOS Guard Set Buffer Index.
#define BIOSGUARD_COMMAND_SET_FLASH_INDEX  0x55      ///< BIOS Guard Set Flash Index.
#define BIOSGUARD_COMMAND_RD_STS           0xB0      ///< BIOS Guard Read Status.
#define BIOSGUARD_COMMAND_END              0xFF      ///< BIOS Guard End.

///
/// BIOS Guard Module Error Codes
///
#define BIOSGUARD_ERR_OK                                0x0000      ///< Operation completed without error.
#define BIOSGUARD_ERR_UNSUPPORTED_CPU                   0x0001      ///< BIOS Guard module detected an incompatibility with the installed CPU.
#define BIOSGUARD_ERR_BAD_DIRECTORY                     0x0002      ///< BIOSGUARD_DIRECTORY check failed.
#define BIOSGUARD_ERR_BAD_BGPDT                         0x0003      ///< A pre-execution check of the BGPDT failed.
#define BIOSGUARD_ERR_BAD_BGUP                          0x0004      ///< An inconsistency was found in the update package.
#define BIOSGUARD_ERR_SCRIPT_SYNTAX                     0x0005      ///< Unknown operator or name, or invalid syntax found in script.
#define BIOSGUARD_ERR_UNDEFINED_FLASH_OBJECT            0x0006      ///< An unimplemented flash object was referenced.
#define BIOSGUARD_ERR_INVALID_LINE                      0x0007      ///< A JMP, JE, JNE, JG, JGE, JL, or JLE operator has a target that is not within the script buffer (between BEGIN and END inclusive).
#define BIOSGUARD_ERR_BAD_BGUPC                         0x0008      ///< BGUPC inconsistency found.
#define BIOSGUARD_ERR_BAD_SVN                           0x0009      ///< BIOS Guard module SVN is lower than required by BGPDT.
#define BIOSGUARD_ERR_UNEXPECTED_OPCODE                 0x000A      ///< An EC related opcode found in a script when the BGPDT indicates there is no EC in the system.
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG     0x000B      ///< An implementation specific memory or IO configuration check failed.
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR              0x000C      ///< An implementation specific general configuration check failed.
#define BIOSGUARD_ERR_EC_OPERATION_FAILED               0x000D      ///< An EC related opcode was found in the script, but the EC hardware was not ready or is behaving in an unexpected way
#define BIOSGUARD_ERR_UNSIGNED_B0_STORE                 0x000E      ///< An attempt to modify B0 contents occurred in an unsigned script.
#define BIOSGUARD_ERR_FT_UPDATE_NOT_ALLOWED             0x000F      ///< Update package indicates Fault Tolerance update, but platform policy in BGPDT precludes it.
#define BIOSGUARD_ERR_RANGE_VIOLATION                   0x8001      ///< Buffer or flash operation exceeded object bounds.
#define BIOSGUARD_ERR_SFAM_VIOLATION                    0x8002      ///< An unsigned script attempted to write or erase a bock of flash that overlaps with the SFAM.
#define BIOSGUARD_ERR_OVERFLOW                          0x8003      ///< An integer overflow occurred.
#define BIOSGUARD_ERR_EXEC_LIMIT                        0x8004      ///< Total number of script opcodes retired exceeds either platform limit, or global limit.
#define BIOSGUARD_ERR_INTERNAL_ERROR                    0x8005      ///< An internal consistency check failed within the BIOS Guard module.
#define BIOSGUARD_ERR_LAUNCH_FAIL                       0xFFFF      ///< CPU detected an error and did not execute the BIOS Guard module.

#define BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0001         0x0001      ///< CPU is not in the supported CPU list. Note that the supported CPU list is only checked in some early versions of the BIOS Guard module.
#define BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0002         0x0002      ///< No valid number of CPU BIOS Guard module header
#define BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0003         0x0003      ///< RdRand is not supported
#define BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0004         0x0004      ///< Unable to produce random number using RdRand opcode.
#define BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0005         0x0005      ///< CPU brand string error
#define BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0006         0x0006      ///< Debug module in production system     

#define BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0001       0x0001      ///< Directory overlaps AC_RAM
#define BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0002       0x0002      ///< Directory is not in addressable DRAM
#define BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0003       0x0003      ///< Unknown type declared in directory
#define BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0004       0x0004      ///< Incorrect number of entries for type. Expected exactly one entry.
#define BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0005       0x0005      ///< Incorrect number of entries for type. Expected zero or one entry.
#define BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0006       0x0006      ///< Directory address field is not zero
#define BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0007       0x0007      ///< Directory address is not aligned properly
#define BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0008       0x0008      ///< Directory contains more than 64 entries
#define BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0009       0x0009      ///< Directory is not in DMA protected ranged

#define BIOSGUARD_ERR_BAD_BGPDT_CODE_0001               0x0001      ///< BGPDT overlaps AC_RAM
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_0002               0x0002      ///< BGPDT is not in addressable DRAM
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_0003               0x0003      ///< BGPDT overlaps another declared range
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_0004               0x0004      ///< Incorrect BGPDT hash
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_0005               0x0005      ///< Incorrect version
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_0006               0x0006      ///< Incorrect BGPDT contents - non-zero EC specific BGPDT fields, but no EC exists
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_0007               0x0007      ///< Incorrect BGPDT contents - At least one EC opcode is non-zero, but EC is not protected
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_0008               0x0008      ///< Incorrect BGPDT contents - EC is protected, but at least one EC opcode is zero
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_0009               0x0009      ///< Incorrect BGPDT contents - Illegal value for EC present field (reserved)
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_000A               0x000A      ///< Non-zero reserved field in BGPDT
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_000B               0x000B      ///< Reserved - not used
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_000C               0x000C      ///< Bad SFAM entry
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_000D               0x000D      ///< Reserved - not used
#define BIOSGUARD_ERR_BAD_BGPDT_CODE_000E               0x000E      ///< Incorrect BGPDT size

#define BIOSGUARD_ERR_BAD_BGUP_CODE_0001                0x0001      ///< BGUP overlaps AC_RAM
#define BIOSGUARD_ERR_BAD_BGUP_CODE_0002                0x0002      ///< BGUP is not in addressable DRAM
#define BIOSGUARD_ERR_BAD_BGUP_CODE_0003                0x0003      ///< BGUP overlaps another declared range
#define BIOSGUARD_ERR_BAD_BGUP_CODE_0004                0x0004      ///< Incorrect version
#define BIOSGUARD_ERR_BAD_BGUP_CODE_0005                0x0005      ///< Incorrect platform ID
#define BIOSGUARD_ERR_BAD_BGUP_CODE_0006                0x0006      ///< Missing BGUPC
#define BIOSGUARD_ERR_BAD_BGUP_CODE_0007                0x0007      ///< EC is not protected
#define BIOSGUARD_ERR_BAD_BGUP_CODE_0008                0x0008      ///< Unsupported BGSL version
#define BIOSGUARD_ERR_BAD_BGUP_CODE_0009                0x0009      ///< Bad script size
#define BIOSGUARD_ERR_BAD_BGUP_CODE_000A                0x000A      ///< Non-zero reserved field in BGUP
#define BIOSGUARD_ERR_BAD_BGUP_CODE_000B                0x000B      ///< BGUP overlaps MSEG
#define BIOSGUARD_ERR_BAD_BGUP_CODE_000C                0x000C      ///< BGUP is not in DMA protected range
#define BIOSGUARD_ERR_BAD_BGUP_CODE_000D                0x000D      ///< BGUP spans SMRR boundary
#define BIOSGUARD_ERR_BAD_BGUP_CODE_000E                0x000E      ///< Unsigned script contains erase operation when flash wear-out mitigation is being enforced 

#define BIOSGUARD_ERR_BAD_BGUPC_CODE_0001               0x0001      ///< BGUPC overlaps AC_RAM
#define BIOSGUARD_ERR_BAD_BGUPC_CODE_0002               0x0002      ///< BGUPC is not in addressable DRAM
#define BIOSGUARD_ERR_BAD_BGUPC_CODE_0003               0x0003      ///< BGUPC overlaps another declared range
#define BIOSGUARD_ERR_BAD_BGUPC_CODE_0004               0x0004      ///< Incorrect version
#define BIOSGUARD_ERR_BAD_BGUPC_CODE_0005               0x0005      ///< Non-zero reserved field in BGUPC
#define BIOSGUARD_ERR_BAD_BGUPC_CODE_0006               0x0006      ///< Unsupported crypto algorithm
#define BIOSGUARD_ERR_BAD_BGUPC_CODE_0007               0x0007      ///< BGUPC is not in DMA protected range
#define BIOSGUARD_ERR_BAD_BGUPC_CODE_0008               0x0008      ///< BGUP attributes are all zero
#define BIOSGUARD_ERR_BAD_BGUPC_CODE_0009               0x0009      ///< Bad key hash
#define BIOSGUARD_ERR_BAD_BGUPC_CODE_000A               0x000A      ///< Bad signature

#define BIOSGUARD_ERR_BAD_SVN_CODE_0001                 0x0001      ///< BIOS Guard module SVN < BGPDT requirement
#define BIOSGUARD_ERR_BAD_SVN_CODE_0002                 0x0002      ///< BIOS SVN in BGUP < BGPDT requirement
#define BIOSGUARD_ERR_BAD_SVN_CODE_0003                 0x0003      ///< EC SVN in BGUP < EC requirement

#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0001        0x0001      ///< Reserved - not used
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0002        0x0002      ///< SPIBAR overlaps AC_RAM
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0003        0x0003      ///< SPIBAR overlaps DRAM
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0004        0x0004      ///< SPIBAR overlaps reserved area
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0005        0x0005      ///< SPIBAR overlaps APIC
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0006        0x0006      ///< SPIBAR overlaps ME
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0007        0x0007      ///< SPIBAR overlaps PEG1X
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0008        0x0008      ///< SPIBAR overlaps PEG1X
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0009        0x0009      ///< SPIBAR overlaps GTTMMADR
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000A        0x000A      ///< SPI controller configuration is invalid
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000B        0x000B      ///< EC IO ports overlapped with MWAIT_IO_REDIRECTION
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000C        0x000C      ///< SPIBAR overlaps MMIO
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000D        0x000D      ///< GT overlaps DRAM
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000E        0x000E      ///< GT overlaps AC-RAM
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000F        0x000F      ///< GT overlaps reserved area
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0010        0x0010      ///< GT overlaps APIC
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0011        0x0011      ///< GT overlaps GMADR
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0012        0x0012      ///< SPIBAR overlaps a PCIE device
#define BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0013        0x0013      ///< SPIBAR overlaps MMCFG range

#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0001    0x0001      ///< PCH is not supported (device ID)
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0002    0x0002      ///< PCH is not in descriptor mode
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0003    0x0003      ///< PCH is not ready (error bits are set prior to BIOS Guard script execution).
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0004    0x0004      ///< PCH size is not supported
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0005    0x0005      ///< PCH erase block size is not supported
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0006    0x0006      ///< PCH device error (failed in the PCH initialization communication)
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0007    0x0007      ///< GT device did not become idle before 1 second timeout
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0008    0x0008      ///< Overclock Watchdog Timer enabled and locked
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0009    0x0009      ///< GT misconfigured
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_000A    0x000A      ///< MMCFG length is configured to 11b which is unsupported
#define BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_000B    0x000B      ///< Bus #0 is configured to a non-zero value

#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0001          0x0001      ///< Error from crypto function in BGPDT SHA calc 1. Please report to Intel.
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0002          0x0002      ///< Error from crypto function BGUPC SHA calc 1. Please report to Intel
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0003          0x0003      ///< Error from crypto function BGUPC RSA SHA verify. Please report to Intel.
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0004          0x0004      ///< Corrupted internal table that used in the first pass (only if the module was corrupted in AC_RAM). Please report to Intel.
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0005          0x0005      ///< Corrupted internal table that used in the first pass (only if the module was corrupted in AC_RAM). Please report to Intel.
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0006          0x0006      ///< Internal script line is out from the script range (only if have corruption in AC_RAM). Please report to Intel.
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0007          0x0007      ///< Bad parameter to internal function op_load (only if have corruption in AC_RAM). Please report to Intel.
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0008          0x0008      ///< Bad parameter to internal function op_store (only if have corruption in AC_RAM). Please report to Intel.
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0009          0x0009      ///< Bad parameter to internal function mem_cpy_s (only if have corruption in AC_RAM). Please report to Intel.
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_000A          0x000A      ///< Bad range parameter to internal function memcpy (only if have corruption in AC_RAM). Please report to Intel.
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_000B          0x000B      ///< Error from crypto function BGUPC SHA calc 2. Please report to Intel.
#define BIOSGUARD_ERR_INTERNAL_ERROR_CODE_000C          0x000C      ///< Error from crypto function BGUPC RSA verify. Please report to Intel.

#define BIOSGUARD_ERR_EC_OPERATION_FAILED_CODE_0001     0x0001      ///< EC buffer is not empty when expected
#define BIOSGUARD_ERR_EC_OPERATION_FAILED_CODE_0002     0x0002      ///< EC timeout occurred - indicates protocol failure or hardware failure
#define BIOSGUARD_ERR_EC_OPERATION_FAILED_CODE_0003     0x0003      ///< Port Test failed

#define BIOSGUARD_F0_INDEX             0x0         ///< PCH flash object index
#define BIOSGUARD_B0_INDEX             0x0         ///< Buffer object index
#define BIOSGUARD_IF_INDEX             0xF         ///< Integer variable index

#define BIOSGUARD_MEMORY_PAGES         128          ///< BIOS Guard Module needs 512KB of reserved memory (128 pages of 4KB each one).
#define BIOSGUARD_ALIGNMENT_IN_PAGES   128          ///< BIOS Guard requires 512KB alignment (128 pages of 4KB each one).

#define BIOSGUARD_MODULE_SIZE_IN_PAGES                      16   ///< Used to allocate 64K memory (16 pages of 4KB each) for reading BIOS Guard Module.
#define BIOSGUARD_INTERNAL_HEADER_MODULE_ID_OFFSET          140  ///< Offset of Module ID. Size of BIOS HEADER (128 Bytes) + Offset of Internal Header Module ID (12 Bytes).
#define BIOSGUARD_INTERNAL_HEADER_SVN_OFFSET                144  ///< Offset of Module SVN. Size of BIOS HEADER (128 Bytes) + Offset of Internal Header Major Revision ID (16 Bytes).
#define BIOSGUARD_INTERNAL_HEADER_SE_SVN_OFFSET             200  ///< Offset of Module SE SVN. Size of BIOS HEADER (128 Bytes) + Offset of Internal Header Major Revision ID (72 Bytes).
#define BIOSGUARD_MODULE_SIZE_OFFSET                        16   ///< Offset of Size in BIOS Guard Module header.
#define BIOSGUARD_REVISION_ID_MAJOR_OFFSET                  6    ///< Offset of Revision ID (Major).
#define BIOSGUARD_REVISION_ID_MINOR_OFFSET                  4    ///< Offset of Revision ID (Minor).
#define BIOSGUARD_PRODUCT_ID_OFFSET                         56   ///< Offset of Product ID.
#define BIOSGUARD_BUILD_NUMBER_OFFSET                       58   ///< Offset of Build Number.

#define BGPDT_MAJOR_VERSION         2           ///< BGPDT Major Version
#define BGPDT_MINOR_VERSION         2           ///< BGPDT Minor Version
#define BGUP_HDR_VERSION            2           ///< BGUP Header Version
#define PSL_MAJOR_VERSION           2           ///< PSL Major Version
#define PSL_MINOR_VERSION           0           ///< PSL Major Version

#define BIOSGUARD_LOG_VERSION            1      ///< BIOS Guard log version
#define BIOSGUARD_LOG_OPT_STEP_TRACE     BIT0   ///< BIOS Guard logging option - Step trace
#define BIOSGUARD_LOG_OPT_BRANCH_TRACE   BIT1   ///< BIOS Guard logging option - Branch trace
#define BIOSGUARD_LOG_OPT_FLASH_WRITE    BIT2   ///< BIOS Guard logging option - Flash write
#define BIOSGUARD_LOG_OPT_FLASH_ERASE    BIT3   ///< BIOS Guard logging option - Flash erase
#define BIOSGUARD_LOG_OPT_FLASH_ERROR    BIT4   ///< BIOS Guard logging option - Flash error
#define BIOSGUARD_LOG_OPT_DEBUG          BIT5   ///< BIOS Guard logging option - Debug opcode execution

///
/// BIOS Guard attribute definition
///
typedef enum {
  EnumEcPresent = BIT1,
  EnumEcBiosGuardProtected = BIT2,
  EnumDescriptorOverridePolicy = BIT3,
  EnumFlashwearoutProtection = BIT4,
  EnumFtuEnable = BIT5
} BIOSGUARD_ATTRIBUTES;

///
/// SPI component size selection
///
typedef enum {
  EnumSpiCompSize512KB  = 0,
  EnumSpiCompSize1MB,
  EnumSpiCompSize2MB,
  EnumSpiCompSize4MB,
  EnumSpiCompSize8MB,
  EnumSpiCompSize16MB,
  EnumSpiCompSize32MB,
  EnumSpiCompSize64MB,
  EnumSpiCompSize128MB
} SPI_COMPONENT_SIZE;

#define MIN_SFAM_COUNT  1       ///< Minimum SFAM Count
#define MAX_SFAM_COUNT  64      ///< Maximum SFAM Count

///
/// Signed Flash map descriptor definition.
///
typedef struct {
  UINT32 FirstByte; ///< Linear flash address of the first byte of the signed range, must be aligned to be first byte in the block. Ordering is little-endian.
  UINT32 LastByte;  ///< Linear flash address of the last byte of the signed range, must be aligned to be last byte in the block. Ordering is little-endian.
} SFAM_DATA;

///
/// BIOS Guard Platform Data Table  (BGPDT)
/// Provides platform specific data required by BIOS Guard Module
///
typedef struct {
  UINT32    BgpdtSize;                 ///< Size in bytes of BGPDT including SFAM.
  UINT16    BgpdtMajVer;               ///< Indicates major version of BGPDT.
  UINT16    BgpdtMinVer;               ///< Indicates minor version of BGPDT.
  UINT8     PlatId[16];                ///< PLAT_ID used to be compared against the one found in the BGUP Header to prevent cross platform flashing.
  UINT8     PkeySlot0[48];             ///< SHA256 hash for BGUP verification key 0.
  UINT8     PkeySlot1[48];             ///< SHA256 hash for BGUP verification key 1.
  UINT8     PkeySlot2[48];             ///< SHA256 hash for BGUP verification key 2.
  UINT32    BgModSvn;                  ///< BIOS Guard Module SVN.
  UINT32    BiosSvn;                   ///< BIOS_SVN to prevent back-flashing.
  UINT32    ExecLim;                   ///< Limit the number of opcodes that can be executed on any invocation of BIOS Guard.
  /**
  Bitmap of Policy attributes
   - BIT[0]: Reserved. Must be 0
   - BIT[2:1]: EC_PRESENT
      - 00b = EC does not exist in the system.
      - 01b = There exists an EC in the system, BIOS Guard does not extend any protection to the EC.
      - 11b = There exist an EC in the system, BIOS Guard extends protection to the EC.
      - 10b = Reserved.  Must not be used.
   - BIT[3]: DESCRIPTOR_OVERRIDE_POLICY
       - 0b = Do not override BIOS Guard security policy.
       - 1b = Override BIOS Guard security policy.
   - BIT[4]: FLASH_WEAROUT_POLICY
       - 0b = Legacy BIOS Guard behavior.
       - 1b = Enable Flash Wear-Out Protection mitigation.
   - BIT[5]: FTU_ENABLE
       - 0b = Legacy BIOS Guard behavior.
       - 1b = Enable fault tolerant update.
   - BIT[23:6]: Reserved, must be 0
   - BIT[31:24]: Reserved for platform specific definition. Must be 0 if not defined.
  **/
  UINT32    BiosGuardAttr;
  /**
  Read/Write command sent to EC
   - BIT[9:0]: 8 bit IO port used for sending EC commands (writes), and reading EC status (reads).
      - This field must be populated if PLAT_ATTR.EC_PRESENT != 0.
      - This field must be zero if PLAT_ATTR.EC_PRESENT == 0.
   - BIT[31:10]: Reserved.  Must be 0.
  **/
  UINT32    EcCmd;
  /**
  Data read or written to EC
   - BIT[9:0]: 8 bit IO port used for reading and writing data to the EC based on a command issued to EC_CMD.
      - This field must be populated if PLAT_ATTR.EC_PRESENT != 0.
      - This field must be zero if PLAT_ATTR.EC_PRESENT == 0.
   - BIT[31:10]: Reserved.  Must be 0
  **/
  UINT32    EcData;
  /**
  EC command indicating a read of the current EC firmware SVN.
   - BIT[7:0]: EC command.
      - This field must be populated if PLAT_ATTR.EC_PRESENT != 0.
      - This field must be zero if PLAT_ATTR.EC_PRESENT == 0.
   - BIT[31:8]: Reserved.  Must be 0.
  **/
  UINT32    EcCmdGetSvn;
  /**
  EC command indicating begin of flash update session.
   - BIT[7:0]:  EC command.
      - This field must be populated if PLAT_ATTR.EC_PRESENT != 0.
      - This field must be zero if PLAT_ATTR.EC_PRESENT == 0.
   - BIT[31:8]: Reserved.  Must be 0.
  **/
  UINT32    EcCmdOpen;
  /**
  EC command indicating the termination of BIOS Guard protected session.
   - BIT[7:0]:  EC command indicating the termination of BIOS Guard protected session.
      - This field must be populated if PLAT_ATTR.EC_PRESENT != 0.
      - This field must be zero if PLAT_ATTR.EC_PRESENT == 0.
   - BIT[31:8]: Reserved.  Must be 0.
  **/
  UINT32    EcCmdClose;
  /**
  EC command used to verify connectivity between BIOS Guard and EC.
   - BIT[7:0]:  EC command.
      - This field must be populated if PLAT_ATTR.EC_PRESENT != 0.
      - This field must be zero if PLAT_ATTR.EC_PRESENT == 0.
   - BIT[31:8]: Reserved.  Must be 0.
  **/
  UINT32    EcCmdPortTest;
  UINT8     Reserved1[4];              ///< Reserved bits.
  /**
  Defines number of elements in SFAM array
   - BIT[5..0]: Index of the last SFAM element
   - BIT[7..6]: Reserved for future use.  Must be 0
  **/
  UINT8     LastSfam;
  UINT8     Reserved2[3];              ///< Reserved 3 bits.
  SFAM_DATA SfamData[MAX_SFAM_COUNT];  ///< Array of flash address map descriptors.  sizeof (SFAM_DATA) == 8
} BGPDT;

///
/// BIOS Guard update Package Header
///
typedef struct {
  UINT16 Version;           ///< Version of the update package header.  Must be 0x0002.
  UINT8  Reserved3[2];      ///< Reserved bits.
  UINT8  PlatId[16];        ///< PLAT_ID used to be compared against the one found in the BGPDT to prevent cross platform flashing.
  /**
  If any bit set in this field then BGUP must be signed and valid BGUPC must be provided for BGUP to be processed.
  - BIT[0]: Indicates write/erase operations will be executed on protected flash area indicated in the BGPDT SFAM.
  - BIT[1]: Indicates protected EC operations included.
  **/
  UINT16 PkgAttributes;
  UINT8  Reserved4[2];      ///< Reserved bits.
  UINT16 PslMajorVer;       ///< Indicates the PSL major version. Must be 2.
  UINT16 PslMinorVer;       ///< Indicates the PSL minor version. Must be 0.
  UINT32 ScriptSectionSize; ///< Size in bytes of the script.
  UINT32 DataSectionSize;   ///< Size of the data region in bytes.
  UINT32 BiosSvn;           ///< BIOS SVN.
  UINT32 EcSvn;             ///< EC SVN.
  UINT32 VendorSpecific;    ///< Vendor specific data.
} BGUP_HEADER;

///
/// Memory location for BGUPC and BIOS Guard LOG inside BIOS Guard DPR allocated memory for Tool interface
///
#define BGUPC_MEMORY_SIZE            0x00008000                                          ///< 32KB
#define BIOSGUARD_LOG_MEMORY_SIZE    0x00020000                                          ///< 128KB
#define BGUPC_MEMORY_OFFSET          (BGUPC_MEMORY_SIZE + BIOSGUARD_LOG_MEMORY_SIZE)     ///< BiosGuardMemAddress + BiosGuardMemSize - BIOSGUARD_LOG_MEMORY_SIZE - 32KB
#define BIOSGUARD_LOG_MEMORY_OFFSET  BIOSGUARD_LOG_MEMORY_SIZE                           ///< BiosGuardMemAddress + BiosGuardMemSize - 128KB
#define MAX_BIOSGUARD_LOG_PAGE       ((BIOSGUARD_LOG_MEMORY_SIZE / EFI_PAGE_SIZE) - 2)   ///< 30 4KB Pages

///
/// Memory Size for BIOS Guard Update Package when in TSEG
///
#define BGUP_TSEG_BUFFER_SIZE        0x00014000  ///< 16KB Script + 64KB Flash Block.

///
/// BIOS Guard update package definition for BIOS SMM Initiated runtime calls
///
typedef struct {
  BGUP_HEADER BgupHeader;                            ///< BIOS Guard update package header.
  UINT64      BgupBuffer[BGUP_TSEG_BUFFER_SIZE / 8]; ///< BIOS Guard update buffer - Designed to contain the BIOS Guard Script followed immediately by the Update Data
} BGUP;

///
/// BIOS Guard Log
/// The logging facility is used to communicate detailed information regarding the execution of a BIOS Guard script
/// from the SMI handler which invoked the BIOS Guard module itself.
///
typedef struct {
  UINT16 Version;           ///< Indicates the version of the log.  Must be 0x0001.
  UINT16 LastPage;          ///< Last valid page index for the log.
  /**
  Bitmap indicating what events to log
  - BIT[0]:    Step trace. This indicates a full execution trace.  Each line is entered into the log with an EXECUTION_TRACE entry.
  - BIT[1]:    Branch trace. All taken jumps are logged with a BRANCH_TRACE entry.
  - BIT[2]:    Flash write  All flash write operations are logged with a FLASH_WRITE entry.
  - BIT[3]:    Flash erase  All flash erase operations are logged with a FLASH_ERASE entry.
  - BIT[4]:    Flash error. All error conditions from flash operations are logged with FLASH_ERROR entry.
  - BIT[5]:    Debug. Log Debug opcode execution.
  - BIT[6]:    BIOS Guard module debug message. Log implementation specific debug messages from debug module.
  - BIT[31:7]: Reserved. Must be 0. If any reserved bits are set in Header.LoggingOptions, the BIOS Guard module must disable the logging feature.
  **/
  UINT32 LoggingOptions;
  UINT8  Reserved5[8];      ///< Reserved bits.
  UINT32 BgModSvn;          ///< Indicates a version number of the BIOS Guard module.
  UINT32 NumOfEntriesInLog; ///< Total number of log entries that have been written to the log.
} BIOSGUARD_LOG;

///
/// HOB used to pass data through every phase of BIOS Guard Bios.
/// BIOS Guard BIOS code is executed in PEI, DXE and SMM and HOB is the only method to properly pass data between every phase.
///
typedef struct {
  EFI_HOB_GUID_TYPE     EfiHobGuidType;              ///< GUID Hob type header.
  BGPDT                 Bgpdt;                       ///< BIOS Guard Platform Data Table.
  BGUP_HEADER           BgupHeader;                  ///< BIOS Guard update package header, this header will be appended to all flash updates along with PSL script.
  BIOSGUARD_LOG         BiosGuardLog;                ///< Header for BIOS Guard Log Buffer.
  EFI_PHYSICAL_ADDRESS  BiosGuardModulePtr;          ///< Pointer to the BIOS Guard Module.
  UINT64                BiosGuardMemAddress;         ///< BIOS Guard Memory Address for Tool Interface
  UINT8                 BiosGuardMemSize;            ///< BIOS Guard Memory Size for Tool Interface
  UINT8                 BiosGuardSwSmi;              ///< BIOS Guard SW SMI value for Tool Interface
  UINT32                BiosGuardModuleSize;         ///< BIOS Guard Module Size.
} BIOSGUARD_HOB;

#endif
