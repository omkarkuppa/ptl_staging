/** @file
  Function prototype of BoardConfigLib.

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

@par Specification
**/

#ifndef _BOARD_CONFIG_LIB_H_
#define _BOARD_CONFIG_LIB_H_

#include <GpioV2Config.h>
#include <Library/SetupInitLib.h>
#include <HdAudioConfig.h>
#include <PlatformBoardConfig.h>
#include <TbtNvmRetimer.h>
#include <Ppi/IGpuPlatformPolicyPpi.h>

#define SIZE_OF_FIELD(TYPE, Field) (sizeof (((TYPE *)0)->Field))

#define SIZE_OF_TABLE(TABLE, TYPE) (sizeof (TABLE) / sizeof (TYPE))

//
// @todo Define BOARD_CONFIG
//
#define BOARD_CONFIG PLATFORM_INFO

#define PRE_MEM        0
#define POST_MEM       1
#define EARLY_PRE_MEM  2
#define POST_MEM_PCH   3

/**
  Procedure to detect current board HW configuration.

**/
VOID
EFIAPI
GetBoardConfig (
  VOID
  );

/**
  Count the number of GPIO settings in the Table.

  @param[in]  GpioTable   The pointer of GPIO config table
  @param[out] GpioCount   The number of GPIO config entries
**/
VOID
GetGpioTableSize (
  GPIOV2_INIT_CONFIG   *GpioTable,
  OUT UINT16            *GpioCount
  );

/**
  Configure GPIO Table setting to PcdBoardGpioTablePreMem && PcdBoardGpioTable

  @param[in]  GpioTable   The pointer of GPIO config table
  @param[in]  IsPostMem   Is call from PostMem/PreMem
                          True - PostMem, False - PreMem
**/
VOID
ConfigureGpioTabletoPCD(
  IN GPIOV2_INIT_CONFIG   *GpioTable,
  IN UINT16            IsPostMem
  );

/**
  Configure GPIO pads in PEI phase.

  @param[in]  GpioTable  Pointer to Gpio table
**/
VOID
GpioInit (
  IN GPIOV2_INIT_CONFIG *GpioTable
  );

/**
  Configure GPIO group GPE tier.

**/
VOID
GpioGroupTierInit (
  VOID
  );

/**
  Check if given rootport has device connected and enable wake capability

  @param[in]  RpNum           An unsigned integer represent the root port number.

  @retval                     TRUE if endpoint was connected
  @retval                     FALSE if no endpoint was detected
**/
BOOLEAN
IsPcieEndPointPresent (
  IN UINT8 RpNum
  );

/**
  Update Pch Type in BIOS ID string based on PchSeries.

**/
VOID
UpdatePchTypeInBoardId (
  VOID
  );

extern HDAUDIO_VERB_TABLE HdaVerbTableDisplayAudio;
extern HDAUDIO_VERB_TABLE HdaVerbTableAlc286S;
extern HDAUDIO_VERB_TABLE HdaVerbTableAlc298;
extern HDAUDIO_VERB_TABLE HdaVerbTableAlc700;
extern HDAUDIO_VERB_TABLE HdaVerbTableAlc701;
extern HDAUDIO_VERB_TABLE HdaVerbTableAlc274;
extern HDAUDIO_VERB_TABLE HdaVerbTableAlc711;

// LPDDR4 200b 8Gb die, DDP, 2 channels per die, x16
// Samsung K4F6E304Hx-MGCJ
// or Hynix H9HCNNNBUUxLHR-NMx
// or Micron MT53B512M32D2NP-053
// 3733, 32-34-34-79
// 2 ranks per channel, 2 SDRAMs per channel, 2x2x8Gb = 4GB total per channel
extern const UINT8 mLpddr4Ddp8Gb200bSpd;

// LPDDR4X 432b 16Gb die, QDP 2x16
// Samsung K3UH7H70MM-JGCJ
// 3733, 32-34-34-79
// 2 ranks per channel, 1 SDRAMs per rank, 4x16Gb = 8GB total per channel
extern const UINT8 mLpddr4xQdp16Gb432bSpd;

// LPDDR4X 556b 8Gb die, ODP, 1 channel per die, x16
// Micron MT53D1G64D8SQ-046
// 4266
// 2 ranks per channel, 2 SDRAMs per channel, 2x4x8Gb = 8GB total per channel
extern const UINT8 mLpddr4xOdp8Gb556bSpd;

// LPDDR4X 556b 4Gb die, QDP, 1 channel per die, x16
// Micron MT53D512M64D4RQ-046 WT:E
// 4266
// 2 ranks per channel, 2 SDRAMs per channel, 2x4x8Gb = 4GB total per channel
extern const UINT8 mLpddr4xQdp4Gb556bSpd;

extern const UINT8 mLpddr4Ddp16Gb200b1rSpd;

// LPDDR5 496b 12Gb die, QDP 1x16
// Samsung K3LK2K20BM-BGCN
// 5500, ??-??-??-??
// 16 Banks, 4 bank groups, 12Gb SDRAM density
// 16 Row bits, 10 Column bits
// Non-Monolithic DRAM Device, 4 dies, 4 Channels per die,
extern const UINT8 mLpddr5xQdp12Gb496bSpd;

// LPDDR4X 200b 16Gb die, DDP, 2 channel per die, 2x8
// Samsung  K4U6E3S4AA-MGCL
// 4267, 36-39-39-90
// 1 rank per channel, 2 SDRAMs per channel, 2x8GB = 16GB total per channel
extern const UINT8 mLpddr4xDdp16Gb200bSpd;

// LPDDR4X 200b 16Gb die, 2 channel per die, 2x8
// Micron MT53E1G32D4NQ-046
// 4266
extern const UINT8 mLpddr4xQdp16Gb200bSpd;

extern const UINT8 mDdr5Spd_1Rx8[];
extern const UINT8 mDdr5Spd_2Rx8[];

/**
  Setting the number of retimer from the configuration table.

  @param[in]  RetimerFlashConfig   The pointer of retimer flash configuration table
**/
VOID
SetRetimerFlashNumber (
  IN USBC_RETIMER_FLASH_BOARD_CONFIG     *RetimerFlashConfig
  );

/**
  Setting PcdUsbCRetimerConfigTable from UsbC connector/Retimer configuration table

  @param[in]  RetimerFlashConfig         The pointer of reitmer flash configuration table
**/
VOID
SetRetimerCfgTablePcd (
  IN USBC_RETIMER_FLASH_BOARD_CONFIG   *RetimerFlashConfig
  );


/**
  Setting UcsiRevision To Ec

**/
VOID
SetUcsiRevisionToEc (
  VOID
  );

/**
  Setting PcdUcsiRevision

**/
VOID
SetUcsiRevisionPcd (
  VOID
  );

/**
  Setting PcdUsbCPdSupportBitmap from UsbC connector configuration table
  The lower 4 bits (BIT0 - BIT3) of PcdUsbCPdSupportBitmap represent
  whether TCP 0 through 3 can support PD.

**/
VOID
SetPcdUsbCPdSupportBitmap (
  VOID
  );


/**
  Set TCSS Port Capability PCD

**/
VOID
EFIAPI
SetTcssPortCapPcd (
  VOID
  );

/**
  Update UcxxDriverType PCD based on TcssUcmDevice setup menu and
  PortUcxxDriverSupport PCD based on
  TestDisableTcssPortUcxxSupport[x] setup menu.

**/
VOID
EFIAPI
UpdateUcxxDriverTypePcd (
  VOID
  );

VOID
EFIAPI
CnvPreMemPcdInitBeforeSiliconInit (
  VOID
  );

EFI_STATUS
EFIAPI
PlatformDebugStateChecksCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Register a notify event to check debug state and update
  PcdFirmwareDebuggerInitialized.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
PlatformDebugStatePcdInit (
  VOID
  );

/**
  This function will return Lid Status in PEI phase.

  @param[out] CurrentLidStatus

  @retval     EFI_SUCCESS
  @retval     EFI_UNSUPPORTED
**/

EFI_STATUS
EFIAPI
GetPeiPlatformLidStatus (
  OUT LID_STATUS  *CurrentLidStatus
  );

/**
 * @brief  This function returns VBT address and size in the PEI Phase.
  @param[out] VbtAddress
  @param[out] VbtSize

  @retval     EFI_SUCCESS
  @retval     EFI_NOT_FOUND
**/
EFI_STATUS
EFIAPI
GetVbtData (
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
  );

/**
 Install mPeiGraphicsPlatformPpi.
**/
VOID
InstallPeiGfxPlatformPpi (
  VOID
  );

#endif // _BOARD_CONFIG_LIB_H_
