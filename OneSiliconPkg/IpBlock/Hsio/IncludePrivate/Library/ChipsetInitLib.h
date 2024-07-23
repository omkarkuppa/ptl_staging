/** @file
  HSIO ChipsetInit library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#ifndef _CHIPSET_INIT_LIB_H_
#define _CHIPSET_INIT_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiHsioLib.h>
#include <PcrDefine.h>

#define HSIO_OEM_TABLE_VERSION_OFFSET         0x4

#define HSIO_CHIPSETINIT_TBL_MAX_SIZE         0x2000
#define HSIO_SPHY_TBL_MAX_SIZE                0x1000
#define HSIO_NPHY_TBL_MAX_SIZE                0x800

typedef enum {
  OemTableChipsetInit,
  OemTableSphy,
  OemTableNphy
} OEM_TABLE_TYPE;

#pragma pack(1)

/**
  PCH HSIO ChipsetInit Version Information
**/
typedef struct {
  UINT16 BaseCrc;
  UINT16 SusCrc;
  UINT16 OemCrc;
  UINT8  Version;
  UINT8  Product;
  UINT8  MetalLayer     : 4;
  UINT8  BaseLayer      : 4;
  UINT8  OemVersion;
  UINT16 DebugMode      : 1;
  UINT16 OemCrcValid    : 1;
  UINT16 SusCrcValid    : 1;
  UINT16 BaseCrcValid   : 1;
  UINT16 Reserved       : 12;
} PCH_HSIO_VER_INFO;

/**
  BIOS OEM table version information
**/
typedef struct {
  UINT16   Major;
  UINT16   Minor;
  UINT16   Hotfix;
  UINT16   Build;
} OEM_TABLE_VER_INFO;
#pragma pack()

/**
  Writes PHY Tuning values to the ChipsetInit SUS Table

  @param[in] Pid         Sideband ID
  @param[in] Offset      PCR offset
  @param[in] Data32      DWORD Value
  @param[in] CrcAndMask  AND Mask to be used for CRC calculation
  @param[in] CrcOrMask   OR Mask to be used for CRC calculation

  @retval EFI_SUCCESS             Copy to SUS Table was successful
  @retval EFI_DEVICE_ERROR        Unable to save writes to SUS Table
**/
EFI_STATUS
HsioChipsetInitSusWrite32 (
  IN P2SB_PID     Pid,
  IN UINT32       Offset,
  IN UINT32       Data32,
  IN UINT32       CrcAndMask,
  IN UINT32       CrcOrMask
  );

/**
  This function is used to ensure CSME has the latest OEM tables.

  A BIOS<->CSME ChipsetInit sync is performed if there is a mismatch.

  @retval EFI_SUCCESS             BIOS and CSME ChipsetInit settings are in sync
  @retval EFI_UNSUPPORTED         OEM table sync not supported
  @retval EFI_OUT_OF_RESOURCES    BIOS is not able to allocate memory
**/

EFI_STATUS
ChipsetInitSync (
  VOID
  );

#endif // _CHIPSET_INIT_LIB_H_
