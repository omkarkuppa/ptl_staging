/** @file
  SV board HSIO configuration

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

@par Specification
**/


#ifndef _HSIO_BOARD_CONFIG_H_
#define _HSIO_BOARD_CONFIG_H_

#include <GpioV2Config.h>


#define HSIO_MAX_SLOTS                      6
#define MAX_NAME_LENGTH                     10
#define MAX_BUFFER_LENGTH                   256

typedef struct {
  UINT32              SlotNumber;
  UINT32              SlotType;
  GPIOV2_INIT_CONFIG  GpioTable;
} HSIO_GPIOV2_INIT_CONFIG;

typedef enum {
  Peg_M2        = (0x1 << 9),  ///< Set PEG M.2 Module
  Mem_2Dpc      = (0x1 << 8),  ///< Set Memory module 2DPC
  Mem_1Dpc      = (0x1 << 7),  ///< Set Memory module 1DPC
  PegBc2x8_1x4  = (0x1 << 6),  ///< Set PEG 2x8,1x4
  PegBc1x16_1x4 = (0x1 << 5),  ///< Set PEG 1x16,1x4
  HsioBc1x4     = (0x1 << 4),  ///< Set HSIO BC 1x4
  HsioBc4x1     = (0x1 << 3),  ///< Set HSIO BC 4x1
  HsioBc2x1_1x2 = (0x1 << 2),  ///< Set HSIO BC 2x1_1x2
  HsioBc2x2     = (0x1 << 1),  ///< Set HSIO BC 2x2
  HsioBcSATA    = 0x1          /// <set SATA
} HSIO_MODULE_TYPE;

typedef struct {
  UINT32 SrcClock;
  UINT32 RootPort;
} Clock;

typedef struct {
  Clock SlotClk1;
  Clock SlotClk2;
  Clock SlotClk3;
  Clock SlotClk4;
} RootPortClk;

typedef struct {
  UINT32 Size;
  UINT32 Slot[0];
} HSIO_RVP_CONFIG;

/**
  Reads data using SMbus from FRU chip

  @param[in]  FruDeviceAddress
  @param[out] Buffer
  @param[in]  BufSize

  @return TRUE if the reading was successful, FALSE otherwise.
**/
BOOLEAN
ReadFruData (
  IN  UINT8   FruDeviceAddress,
  OUT UINT8   *Buffer,
  IN  INT32   BufSize
  );

/**
  Reads the type of Add On Board (AOB) from FRU unit using SMBus and saves the type in PcdHsioBoardConfig

**/
EFI_STATUS
EFIAPI
HsioConfigDetection (
  VOID
  );

/**
  Extracts the module type which is programmed on the FRU unit located on the add-on card.
  The module type is programmed as a string on the FRU unit in the Board Product Name section and the method returns
  the corresponding enum (HSIO_MODULE_TYPE) value.

  @param[in]  SlotFruAddress  SMBus address of the HSIO module
  @param[out] ModuleType      HSIO_MODULE_TYPE enum value

  @retval EFI_DEVICE_ERROR  Failed to read from SMBUS
  @retval EFI_CRC_ERROR     FRU data CRC error
  @retval EFI_NOT_FOUND     Module type was not found in the Board Product Name section.
  @retval EFI_SUCCESS       HSIO module type detected
**/
EFI_STATUS
EFIAPI
GetHsioModuleType (
  IN  UINT8     SlotFruAddress,
  OUT UINT32   *ModuleType
  );

/**
  Sets the GpioTable output parameter base on the static and dynamic GPIO configuration.
  @param[OUT]     GpioTable             Pointer to GPIO config table.
  @param[in]      GpioLookupTable       Pointer to the dynamic GPIO configuration lookup table
  @param[in]      StaticTable           Pointer to static GPIO config table
  @param[in]      RvpConfig             Holds the RVP module types
**/
EFI_STATUS
EFIAPI
SetHsioRvpGpioTable (
  OUT GPIOV2_INIT_CONFIG       *GpioTable,
  IN  HSIO_GPIOV2_INIT_CONFIG  *GpioLookupTable,
  IN  GPIOV2_INIT_CONFIG       *StaticTable,
  IN  HSIO_RVP_CONFIG          *RvpConfig
  );

#endif // _HSIO_BOARD_CONFIG_H_
