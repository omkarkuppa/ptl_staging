/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#ifndef _BOOTLOADER_PLATFORM_DATA_H
#define _BOOTLOADER_PLATFORM_DATA_H

//
// @note
// All lines between tag EXTERNAL_BOOTLOADER_STRUCT_BEGIN and
// EXTERNAL_BOOTLOADER_STRUCT_END will be exported to the generated
// FspUpd.h, FsptUpd.h, FspmUpd.h and FspsUpd.h files by the tool.
//

#pragma pack(push, 1)


/* !EXPORT FSPT EXTERNAL_BOOTLOADER_STRUCT_BEGIN */
/* !EXPORT FSPT EXTERNAL_BOOTLOADER_STRUCT_END   */


/* !EXPORT FSPM EXTERNAL_BOOTLOADER_STRUCT_BEGIN */

#include <MemInfoHob.h>

///
/// The ChipsetInit Info structure provides the information of ME ChipsetInit CRC and BIOS ChipsetInit CRC.
///
typedef struct {
  UINT8             Revision;         ///< Chipset Init Info Revision
  UINT8             Rsvd[3];          ///< Reserved
  UINT16            MeChipInitCrc;    ///< 16 bit CRC value of MeChipInit Table
  UINT16            BiosChipInitCrc;  ///< 16 bit CRC value of PchChipInit Table
} CHIPSET_INIT_INFO;

/* !EXPORT FSPM EXTERNAL_BOOTLOADER_STRUCT_END   */


/* !EXPORT FSPS EXTERNAL_BOOTLOADER_STRUCT_BEGIN */

///
/// Azalia Header structure
///
typedef struct {
  UINT16 VendorId;           ///< Codec Vendor ID
  UINT16 DeviceId;           ///< Codec Device ID
  UINT8  RevisionId;         ///< Revision ID of the codec. 0xFF matches any revision.
  UINT8  SdiNum;             ///< SDI number, 0xFF matches any SDI.
  UINT16 DataDwords;         ///< Number of data DWORDs pointed by the codec data buffer.
  UINT32 Reserved;           ///< Reserved for future use. Must be set to 0.
} AZALIA_HEADER;

///
/// Audio Azalia Verb Table structure
///
typedef struct {
  AZALIA_HEADER Header;      ///< AZALIA PCH header
  UINT32        *Data;       ///< Pointer to the data buffer. Its length is specified in the header
} AUDIO_AZALIA_VERB_TABLE;

///
/// Refer to the definition of PCH_INT_PIN
///
typedef enum {
  SiPchNoInt,        ///< No Interrupt Pin
  SiPchIntA,
  SiPchIntB,
  SiPchIntC,
  SiPchIntD
} SI_PCH_INT_PIN;
///
/// The PCH_DEVICE_INTERRUPT_CONFIG block describes interrupt pin, IRQ and interrupt mode for PCH device.
///
typedef struct {
  UINT8        Device;                  ///< Device number
  UINT8        Function;                ///< Device function
  UINT8        IntX;                    ///< Interrupt pin: INTA-INTD (see SI_PCH_INT_PIN)
  UINT8        Irq;                     ///< IRQ to be set for device.
} SI_PCH_DEVICE_INTERRUPT_CONFIG;

#define SI_PCH_MAX_DEVICE_INTERRUPT_CONFIG  64       ///< Number of all PCH devices

/* !EXPORT FSPS EXTERNAL_BOOTLOADER_STRUCT_END */

#pragma pack(pop)

#endif

