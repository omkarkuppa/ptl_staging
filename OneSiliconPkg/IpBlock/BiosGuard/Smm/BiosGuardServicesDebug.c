/** @file
  BIOS Guard Driver implements the BIOS Guard Host Controller Compatibility Interface.

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

@par Specification Reference:
**/

#include "BiosGuardServicesDebug.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

/**
  Display condition associated with additional data returned by the module
  with error code Unsupported Cpu

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToUnsupportedCpu (
  IN UINT16 BiosGuardAdditionalData
)
{
  switch (BiosGuardAdditionalData)
  {
  case BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0001:
    DEBUG ((DEBUG_ERROR, " CPU is not in the supported CPU list\n"));
    break;
  case BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0002:
    DEBUG ((DEBUG_ERROR, " No valid number of CPU BIOS Guard module header\n"));
    break;
  case BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0003:
    DEBUG ((DEBUG_ERROR, " RdRand is not supported\n"));
    break;
  case BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0004:
    DEBUG ((DEBUG_ERROR, " Unable to produce random number using RdRand opcode\n"));
    break;
  case BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0005:
    DEBUG ((DEBUG_ERROR, " CPU brand string error\n"));
    break;
  case BIOSGUARD_ERR_UNSUPPORTED_CPU_CODE_0006:
    DEBUG ((DEBUG_ERROR, " Debug module in production system\n"));
    break;
  default:
    DEBUG ((DEBUG_ERROR, " details unknown\n"));
    break;
  }
}

/**
  Display condition associated with additional data returned by the module
  with error code Bad Directory

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToBadDirectory (
  IN UINT16 BiosGuardAdditionalData
)
{
  switch (BiosGuardAdditionalData)
  {
  case BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0001:
    DEBUG ((DEBUG_ERROR, " Directory overlaps AC_RAM\n"));
    break;
  case BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0002:
    DEBUG ((DEBUG_ERROR, " Directory is not in addressable DRAM\n"));
    break;
  case BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0003:
    DEBUG ((DEBUG_ERROR, " Unknown type declared in directory\n"));
    break;
  case BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0004:
    DEBUG ((DEBUG_ERROR, " Incorrect number of entries for type. Expected exactly one entry\n"));
    break;
  case BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0005:
    DEBUG ((DEBUG_ERROR, " Incorrect number of entries for type. Expected zero or one entry.\n"));
    break;
  case BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0006:
    DEBUG ((DEBUG_ERROR, " Directory address field is not zero\n"));
    break;
  case BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0007:
    DEBUG ((DEBUG_ERROR, " Directory address is not aligned properly\n"));
    break;
  case BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0008:
    DEBUG ((DEBUG_ERROR, " Directory contains more than 64 entries\n"));
    break;
  case BIOSGUARD_ERR_BAD_DIRECTORY_CPU_CODE_0009:
    DEBUG ((DEBUG_ERROR, " Directory is not in DMA protected ranged\n"));
    break;
  default:
    DEBUG ((DEBUG_ERROR, " details unknown\n"));
    break;
  }
}

/**
  Display condition associated with additional data returned by the module
  with error code Bad BGPDT

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuarMatchDetailsStringToBadBgpdt (
  IN UINT16 BiosGuardAdditionalData
)
{
  switch (BiosGuardAdditionalData)
  {
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_0001 :
    DEBUG ((DEBUG_ERROR, " BGPDT overlaps AC_RAM\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_0002 :
    DEBUG ((DEBUG_ERROR, " BGPDT is not in addressable DRAM\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_0003 :
    DEBUG ((DEBUG_ERROR, " BGPDT overlaps another declared range\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_0004 :
    DEBUG ((DEBUG_ERROR, " Incorrect BGPDT hash\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_0005 :
    DEBUG ((DEBUG_ERROR, " Incorrect version\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_0006 :
    DEBUG ((DEBUG_ERROR, " Incorrect BGPDT contents - non-zero EC specific BGPDT fields, but no EC exists\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_0007 :
    DEBUG ((DEBUG_ERROR, " Incorrect BGPDT contents - At least one EC opcode is non-zero, but EC is not protected\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_0008 :
    DEBUG ((DEBUG_ERROR, " Incorrect BGPDT contents - EC is protected, but at least one EC opcode is zero\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_0009 :
    DEBUG ((DEBUG_ERROR, " Incorrect BGPDT contents - Illegal value for EC present field (reserved)\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_000A :
    DEBUG ((DEBUG_ERROR, " Non-zero reserved field in BGPDT\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_000B :
    DEBUG ((DEBUG_ERROR, " Reserved - not used\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_000C :
    DEBUG ((DEBUG_ERROR, " Bad SFAM entry\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_000D :
    DEBUG ((DEBUG_ERROR, " Reserved - not used\n"));
    break;
  case BIOSGUARD_ERR_BAD_BGPDT_CODE_000E :
    DEBUG ((DEBUG_ERROR, " Incorrect number of entries for type. Expected zero or one entry\n"));
    break;
  default:
    DEBUG ((DEBUG_ERROR, " details unknown\n"));
    break;
  }
}

/**
  Display condition associated with additional data returned by the module
  with error code Bad BGUP

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToBadBgup (
  IN UINT16 BiosGuardAdditionalData
)
{
  switch (BiosGuardAdditionalData)
  {
    case BIOSGUARD_ERR_BAD_BGUP_CODE_0001:
      DEBUG ((DEBUG_ERROR, " BGUP overlaps AC_RAM\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_0002:
      DEBUG ((DEBUG_ERROR, " BGUP is not in addressable DRAM\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_0003 :
      DEBUG ((DEBUG_ERROR, " BGUP overlaps another declared range\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_0004:
      DEBUG ((DEBUG_ERROR, " Incorrect version\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_0005:
      DEBUG ((DEBUG_ERROR, " Incorrect platform ID\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_0006:
      DEBUG ((DEBUG_ERROR, " Missing BGUPC\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_0007 :
      DEBUG ((DEBUG_ERROR, " EC is not protected\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_0008 :
      DEBUG ((DEBUG_ERROR, " Unsupported BGSL version\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_0009:
      DEBUG ((DEBUG_ERROR, " Bad script size\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_000A:
      DEBUG ((DEBUG_ERROR, " Non-zero reserved field in BGUP\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_000B:
      DEBUG ((DEBUG_ERROR, " BGUP overlaps MSEG\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_000C:
      DEBUG ((DEBUG_ERROR, " BGUP is not in DMA protected range\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_000D:
      DEBUG ((DEBUG_ERROR, " BGUP spans SMRR boundary\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUP_CODE_000E:
      DEBUG ((DEBUG_ERROR, " Unsigned script contains erase operation when flash wear-out mitigation is being enforced \n"));
    break;
    default:
      DEBUG ((DEBUG_ERROR, " details unknown\n"));
      break;
  }
}

/**
  Display condition associated with additional data returned by the module
  with error code Bad BGUPC

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToBadBgupc (
  IN UINT16 BiosGuardAdditionalData
)
{
  switch (BiosGuardAdditionalData)
  {
    case BIOSGUARD_ERR_BAD_BGUPC_CODE_0001:
      DEBUG ((DEBUG_ERROR, " BGUPC overlaps AC_RAM\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUPC_CODE_0002:
      DEBUG ((DEBUG_ERROR, " BGUPC is not in addressable DRAM\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUPC_CODE_0003:
      DEBUG ((DEBUG_ERROR, " BGUPC overlaps another declared range\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUPC_CODE_0004:
      DEBUG ((DEBUG_ERROR, " Incorrect version\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUPC_CODE_0005:
      DEBUG ((DEBUG_ERROR, " Non-zero reserved field in BGUPC\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUPC_CODE_0006:
      DEBUG ((DEBUG_ERROR, " Unsupported crypto algorithm\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUPC_CODE_0007:
      DEBUG ((DEBUG_ERROR, " BGUPC is not in DMA protected rang\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUPC_CODE_0008:
      DEBUG ((DEBUG_ERROR, " BGUP attributes are all zero\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUPC_CODE_0009:
      DEBUG ((DEBUG_ERROR, " Bad key hash\n"));
      break;
    case BIOSGUARD_ERR_BAD_BGUPC_CODE_000A:
      DEBUG ((DEBUG_ERROR, " Bad signature\n"));
      break;
    default:
      DEBUG ((DEBUG_ERROR, " details unknown\n"));
      break;
  }
}

/**
  Display condition associated with additional data returned by the module
  with error code Bad SVN

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToBadSvn (
  IN UINT16 BiosGuardAdditionalData
)
{
  switch (BiosGuardAdditionalData)
  {
    case BIOSGUARD_ERR_BAD_SVN_CODE_0001:
      DEBUG ((DEBUG_ERROR, " BIOS Guard module SVN < BGPDT requirement\n"));
      break;
    case BIOSGUARD_ERR_BAD_SVN_CODE_0002:
      DEBUG ((DEBUG_ERROR, " BIOS SVN in BGUP < BGPDT requirement\n"));
      break;
    case BIOSGUARD_ERR_BAD_SVN_CODE_0003:
      DEBUG ((DEBUG_ERROR, " EC SVN in BGUP < EC requirement\n"));
      break;
    default:
      DEBUG ((DEBUG_ERROR, " details unknown\n"));
      break;
  }
}

/**
  Display condition associated with additional data returned by the module
  with error code Invalid Address Range Configuration

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToInvalidAddrRangeConf (
  IN UINT16 BiosGuardAdditionalData
)
{
  switch (BiosGuardAdditionalData)
  {
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0001:
      DEBUG ((DEBUG_ERROR, " Reserved - not used\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0002:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps AC_RAM\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0003:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps DRAM\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0004:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps reserved area\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0005:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps APIC\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0006:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps ME\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0007:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps PEG1X\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0008:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps PEG1X\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0009:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps GTTMMADR\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000A:
      DEBUG ((DEBUG_ERROR, " SPI controller configuration is invalid\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000B:
      DEBUG ((DEBUG_ERROR, " EC IO ports overlapped with MWAIT_IO_REDIRECTION\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000C:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps MMIO\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000D:
      DEBUG ((DEBUG_ERROR, " GT overlaps DRAM\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000E:
      DEBUG ((DEBUG_ERROR, " GT overlaps AC-RAM\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_000F:
      DEBUG ((DEBUG_ERROR, " GT overlaps reserved area\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0010:
      DEBUG ((DEBUG_ERROR, " GT overlaps APIC\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0011:
      DEBUG ((DEBUG_ERROR, " GT overlaps GMADR\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0012:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps a PCIE device\n"));
      break;
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG_CODE_0013:
      DEBUG ((DEBUG_ERROR, " SPIBAR overlaps MMCFG range\n"));
      break;
    default:
      DEBUG ((DEBUG_ERROR, " details unknown\n"));
      break;
  }
}

/**
  Display condition associated with additional data returned by the module
  with error code General Config Error

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToGeneralConfigError (
  IN UINT16 BiosGuardAdditionalData
)
{
  switch (BiosGuardAdditionalData)
  {
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0001:
      DEBUG ((DEBUG_ERROR, " PCH is not supported (device ID)\n"));
      break;
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0002:
      DEBUG ((DEBUG_ERROR, " PCH is not in descriptor mode\n"));
      break;
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0003:
      DEBUG ((DEBUG_ERROR, " PCH is not ready (error bits are set prior to BIOS Guard script execution).\n"));
      break;
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0004:
      DEBUG ((DEBUG_ERROR, " PCH size is not supported\n"));
      break;
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0005:
      DEBUG ((DEBUG_ERROR, " PCH erase block size is not supported\n"));
      break;
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0006:
      DEBUG ((DEBUG_ERROR, " PCH device error (failed in the PCH initialization communication)\n"));
      break;
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0007:
      DEBUG ((DEBUG_ERROR, " GT device did not become idle before 1 second timeout\n"));
      break;
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0008:
      DEBUG ((DEBUG_ERROR, " Overclock Watchdog Timer enabled and locked\n"));
      break;
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_0009:
      DEBUG ((DEBUG_ERROR, " GT misconfigured\n"));
      break;
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_000A:
      DEBUG ((DEBUG_ERROR, " MMCFG length is configured to 11b which is unsupported\n"));
      break;
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR_CODE_000B:
      DEBUG ((DEBUG_ERROR, " Bus #0 is configured to a non-zero value\n"));
      break;
    default:
      DEBUG ((DEBUG_ERROR, " details unknown\n"));
      break;
    }
}

/**
  Display condition associated with additional data returned by the module
  with error code Internal Error

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToInternalError (
  IN UINT16 BiosGuardAdditionalData
)
{
  switch (BiosGuardAdditionalData)
  {
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0001      :
      DEBUG ((DEBUG_ERROR, " Error from crypto function in BGPDT SHA calc 1.\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0002      :
      DEBUG ((DEBUG_ERROR, " Error from crypto function BGUPC SHA calc 1.\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0003      :
      DEBUG ((DEBUG_ERROR, " Error from crypto function BGUPC RSA SHA verify.\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0004      :
      DEBUG ((DEBUG_ERROR, " Corrupted internal table that used in the first pass (only if the module was corrupted in AC_RAM).\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0005      :
      DEBUG ((DEBUG_ERROR, " Corrupted internal table that used in the first pass (only if the module was corrupted in AC_RAM).\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0006      :
      DEBUG ((DEBUG_ERROR, " Internal script line is out from the script range (only if have corruption in AC_RAM).\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0007      :
      DEBUG ((DEBUG_ERROR, " Bad parameter to internal function op_load (only if have corruption in AC_RAM).\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0008      :
      DEBUG ((DEBUG_ERROR, " Bad parameter to internal function op_store (only if have corruption in AC_RAM).\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_0009      :
      DEBUG ((DEBUG_ERROR, " Bad parameter to internal function mem_cpy_s (only if have corruption in AC_RAM).\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_000A      :
      DEBUG ((DEBUG_ERROR, " Bad range parameter to internal function memcpy (only if have corruption in AC_RAM).\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_000B      :
      DEBUG ((DEBUG_ERROR, " Error from crypto function BGUPC SHA calc 2.\n"));
      break;
    case BIOSGUARD_ERR_INTERNAL_ERROR_CODE_000C      :
      DEBUG ((DEBUG_ERROR, " Error from crypto function BGUPC RSA verify.\n"));
      break;
    default:
      DEBUG ((DEBUG_ERROR, " details unknown\n"));
      break;
    }
}

/**
  Display condition associated with additional data returned by the module
  with error code EC Operation Failed

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToEcOperaionFailed (
  IN UINT16 BiosGuardAdditionalData
)
{
  switch (BiosGuardAdditionalData)
  {
    case BIOSGUARD_ERR_EC_OPERATION_FAILED_CODE_0001 :
      DEBUG ((DEBUG_ERROR, " EC buffer is not empty when expected\n"));
      break;
    case BIOSGUARD_ERR_EC_OPERATION_FAILED_CODE_0002 :
      DEBUG ((DEBUG_ERROR, " EC timeout occurred - indicates protocol failure or hardware failure\n"));
      break;
    case BIOSGUARD_ERR_EC_OPERATION_FAILED_CODE_0003 :
      DEBUG ((DEBUG_ERROR, " Port Test failed\n"));
      break;
    default:
      DEBUG ((DEBUG_ERROR, " details unknown\n"));
      break;
  }
}