/** @file
  Internal file to support Foxville CRC operation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleCommon.h"
#include "Defines/FoxvilleModule.h"
#include "Defines/FoxvilleNvmMap.h"
#include "Defines/FoxvilleRegs.h"
#include "Misc/FoxvillePciIoAccess.h"
#include "FoxvilleApi.h"
#include "FoxvilleCrc.h"

//
// Module Variables.
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8  mFoxvilleCrcTable[] = {
  0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
  0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
  0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
  0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
  0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
  0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
  0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
  0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
  0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
  0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
  0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
  0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
  0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
  0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
  0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
  0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};

/**
  Calculate the CRC for a given section of the FW.

  @param[in]   HwPtr                  Pointer to the HW instance.
  @param[in]   Pointer                Address to the FW section in flash.
  @param[out]  AdditionalCrc          Pointer to the calculated CRC value.
  @param[out]  AdditionalCrcLocation  Pointer to the address of CRC in the flash.

  @retval  EFI_SUCCESS            Succeed to calculate the CRC value.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to calculate the CRC value.

**/
EFI_STATUS
CalculateFwSectionCrc (
  IN     FOXVILLE_HW_INSTANCE  *HwPtr,
  IN     UINT32                Pointer,
     OUT UINT8                 *AdditionalCrc,
     OUT UINT16                *AdditionalCrcLocation
  )
{
  EFI_STATUS  Status;
  UINT32      FlashSize;
  UINT32      ImageSize;
  UINT32      CrcLocationValue;
  UINT16      Length;
  UINT16      StartPosition;
  UINT16      Idx;
  UINT8       Buffer[2];
  UINT8       Crc = 0;

  FlashSize        = HwPtr->FlashSize;
  ImageSize        = 0;
  CrcLocationValue = 0;
  Length           = 0;
  StartPosition    = 0;
  Idx              = 0;

  ZeroMem (Buffer, sizeof (Buffer));

  DEBUG ((DEBUG_INFO, "%a: Pointer = 0x%08X\n", __func__, Pointer));

  if ((AdditionalCrc == NULL) || (AdditionalCrcLocation == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read the location of the specified pointer.
  //
  if (Pointer >= SHADOW_RAM_SECTOR_SIZE) {
    //
    // Pointers to the Manageability sections are stored on the Flash
    //
    //  Firmware Extension Area data will not fall into this code block because Pointer
    //  will equal 0 - pointing at the beginning of the data buffer. We want to
    //  use pointers passed in the buffer for Firmware Extension Area data.
    //
    if (Pointer >= FlashSize) {
      DEBUG ((DEBUG_ERROR, "Pointer is higher than Size.\n"));
      return EFI_INVALID_PARAMETER;
    }

    Status = ReadFlashRegUint32 (HwPtr->PciIo, Pointer * 2, &CrcLocationValue);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    //
    // We need only the first word of CrcLocationValue.
    //
    *AdditionalCrcLocation = (UINT16)(CrcLocationValue & MAX_UINT16);

    //
    // Only some manageability sections can be located in the legacy EEPROM. Skip
    // manageability sections located on the Flash's Firmware Extension Area when we
    // passed-in only EEPROM data.
    //
    if (*AdditionalCrcLocation >= ImageSize) {
      *AdditionalCrcLocation = 0;
    }
  } else {
    Status = ReadNvmEepromUint16 (HwPtr->PciIo, (UINT16)Pointer, AdditionalCrcLocation);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Couldn't read offset 0x%x for addCRCLoc\n", Pointer));
      return EFI_DEVICE_ERROR;
    }
  }

  if ((*AdditionalCrcLocation == 0) || (*AdditionalCrcLocation == MAX_UINT16)) {
    return EFI_SUCCESS;
  }

  //
  // Read the header of the additional section.
  //
  Status = ReadNvmEepromUint16 (HwPtr->PciIo, *AdditionalCrcLocation, (UINT16 *)Buffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Couldn't read offset 0x%x for TradSec\n", *AdditionalCrcLocation));
    return EFI_DEVICE_ERROR;
  }

  //
  // Parse the first word in the section.
  //
  StartPosition = *AdditionalCrcLocation;
  Length        = Buffer[0] * sizeof (UINT16);

  for (Idx = StartPosition; Idx < (Length / 2) + StartPosition; Idx++) {
    Status = ReadNvmEepromUint16 (HwPtr->PciIo, Idx, (UINT16 *)Buffer);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Couldn't read offset 0x%x for TradSec\n", *AdditionalCrcLocation));
      return EFI_DEVICE_ERROR;
    }

    Crc = mFoxvilleCrcTable[Crc ^ Buffer[0]];

    //
    // We are supposed to presume that the top byte of first word holds 0.
    // This byte holds the current CRC we are updating.
    //
    if (Idx != StartPosition) {
      Crc = mFoxvilleCrcTable[Crc ^ Buffer[1]];
    } else {
      Crc = mFoxvilleCrcTable[Crc ^ 0];
    }
  }

  //
  // Return the Crc for this section.
  //
  *AdditionalCrc = Crc;

  return EFI_SUCCESS;
}

/**
  Calculate and update the CRC for a given section of the FW.

  @param[in]  HwPtr    Pointer to the HW instance.
  @param[in]  Pointer  Address to the FW section in flash.

  @retval  EFI_SUCCESS            Succeed to calculate and update the CRC value.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_CRC_ERROR          Failed to calculate the CRC value.
  @retval  Others                 Failed to calculate and update the CRC value.

**/
EFI_STATUS
EvaluateFwSectionCrc (
  IN FOXVILLE_HW_INSTANCE  *HwPtr,
  IN UINT32                Pointer
  )
{
  EFI_STATUS               Status;
  UINT16                   CrcAddress;
  UINT8                    CrcValue;
  FOXVILLE_SECTION_HEADER  SectionHeader;

  CrcAddress           = 0;
  CrcValue             = 0;
  SectionHeader.Uint16 = 0;

  Status = CalculateFwSectionCrc (HwPtr, Pointer, &CrcValue, &CrcAddress);
  if (EFI_ERROR (Status)) {
    return EFI_CRC_ERROR;
  }

  //
  // Section is not presented.
  //
  if (CrcAddress == 0x0000) {
    DEBUG ((DEBUG_INFO, "Section is not presented.\n"));
    return EFI_SUCCESS;
  }

  //
  // Illegal value.
  //
  if (CrcAddress == MAX_UINT16) {
    return EFI_DEVICE_ERROR;
  }

  Status = ReadNvmEepromUint16 (HwPtr->PciIo, CrcAddress, &(SectionHeader.Uint16));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to read the section header.\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a: Flash section header - 0x%04X.\n", __func__, SectionHeader.Uint16));

  //
  // Update the CRC field only.
  //
  SectionHeader.Bits.Crc = CrcValue;

  DEBUG ((DEBUG_INFO, "%a: Updated section header - 0x%04X.\n", __func__, SectionHeader.Uint16));

  Status = WriteNvmEepromUint16 (HwPtr->PciIo, CrcAddress, SectionHeader.Uint16);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to write the section header.\n"));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Updates all the MNG sections CRC value.

  @param[in]  HwPtr  Pointer to the HW instance.

  @retval  EFI_SUCCESS            Succeed to update all the MNG section CRC value.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to update all the MNG section CRC value.

**/
EFI_STATUS
EFIAPI
UpdateMngCrcs (
  IN FOXVILLE_HW_INSTANCE  *HwPtr
  )
{
  EFI_STATUS                   Status;
  EFI_STATUS                   ExitStatus;
  UINT16                       FWSectionPointer;
  UINT16                       ManagementMode;
  FOXVILLE_COMMON_FW_PARAMS_1  CommonFwParams1;

  ExitStatus             = EFI_SUCCESS;
  FWSectionPointer       = 0;
  ManagementMode         = 0;
  CommonFwParams1.Uint16 = 0;

  DEBUG ((DEBUG_INFO, "Enter UpdateMngCrcs\n"));

  if (HwPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the FW module configuration pointer from EEPROM.
  //
  Status = ReadNvmEepromUint16 (
             HwPtr->PciIo,
             WORD_OFFSET_OF (FOXVILLE_NVM_MAP, FwModuleCfgPtr),
             &FWSectionPointer
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get FW module configuration pointer from EEPROM.\n"));
    return EFI_DEVICE_ERROR;
  }

  //
  // If it is not presented, then there is nothing to do.
  //
  if (FWSectionPointer == MAX_UINT16) {
    return EFI_SUCCESS;
  }

  //
  // Update the Common FW parameters CRC.
  //
  Status = EvaluateFwSectionCrc (HwPtr, (FWSectionPointer + WORD_OFFSET_OF (FOXVILLE_FW_MODULE_CFG, CommonFwParamsPtr)));
  if (EFI_ERROR (Status)) {
    ExitStatus = EFI_DEVICE_ERROR;
  }

  //
  // Update the Flexible TCO Filters Config CRC.
  // (It is possible that module is disabled.)
  //
  Status = EvaluateFwSectionCrc (HwPtr, (FWSectionPointer + WORD_OFFSET_OF (FOXVILLE_FW_MODULE_CFG, FlexTcoFilterCfgPtr)));
  if ((EFI_ERROR (Status)) && (Status != EFI_DEVICE_ERROR)) {
    ExitStatus = EFI_DEVICE_ERROR;
  }

  //
  // Update the Traffic Type Params CRC
  //
  Status = EvaluateFwSectionCrc (HwPtr, (FWSectionPointer + WORD_OFFSET_OF (FOXVILLE_FW_MODULE_CFG, TrafficTypesParamsPtr)));
  if (EFI_ERROR (Status)) {
    ExitStatus = EFI_DEVICE_ERROR;
  }

  //
  // Get the Common MNG parameters section pointer from EEPROM.
  //
  Status = ReadNvmEepromUint16 (
             HwPtr->PciIo,
             (FWSectionPointer + WORD_OFFSET_OF (FOXVILLE_FW_MODULE_CFG, CommonFwParamsPtr)),
             &ManagementMode
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get Common FW parameters pointer from EEPROM.\n"));
    return EFI_DEVICE_ERROR;
  }

  Status = ReadNvmEepromUint16 (
             HwPtr->PciIo,
             (ManagementMode + WORD_OFFSET_OF (FOXVILLE_COMMON_FW_PARAMS_SECTION, CommonFwParams1)),
             &(CommonFwParams1.Uint16)
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get management mode from EEPROM.\n"));
    return EFI_DEVICE_ERROR;
  }

  //
  // Check if SPT unique sections presented.
  //
  if (CommonFwParams1.Bits.ManageabilityMode != FOXVILLE_MANAGEABILITY_MODE_ENABLED) {
    goto Exit;
  }

  //
  // Update the SideBand CRC.
  //
  Status = EvaluateFwSectionCrc (HwPtr, (FWSectionPointer + WORD_OFFSET_OF (FOXVILLE_FW_MODULE_CFG, SidebandCfgPtr)));
  if (EFI_ERROR (Status)) {
    ExitStatus = EFI_DEVICE_ERROR;
  }

  //
  // Update Port Configuration CRC.
  //
  Status = EvaluateFwSectionCrc (HwPtr, (FWSectionPointer + WORD_OFFSET_OF (FOXVILLE_FW_MODULE_CFG, PtLanCfgPtr)));
  if (EFI_ERROR (Status)) {
    ExitStatus = EFI_DEVICE_ERROR;
  }

Exit:

  return ExitStatus;
}
