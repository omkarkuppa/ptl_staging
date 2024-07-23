/** @file
  Definition of Alert Standard Format (ASF) 2.0

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2005 Intel Corporation.

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
#ifndef _ASF_PROTOCOL_H_
#define _ASF_PROTOCOL_H_

#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/AlertStandardFormatTable.h>
#include <AsfMsgs.h>

typedef enum {
  Byte1,
  Byte2
} OPTION_INDEX;


///
/// Sub-Command Attributes
///
#define ASF_BOOT_OPTIONS_PRESENT      0x16
#define ASF_BOOT_OPTIONS_NOT_PRESENT  0x17

///
/// ASF Internet Assigned Numbers Authority Manufacturer ID
/// (The firmware sends 0XBE110000 for decimal value 4542)
///
#define ASF_INDUSTRY_IANA             0x000011BE
#define ASF_INDUSTRY_CONVERTED_IANA   INTEL_IANA_SWAP32 (ASF_INDUSTRY_IANA)  ///< 0XBE110000, received from ME FW

///
/// Intel specific definitions start
///

///
/// Intel Internet Assigned Numbers Authority Manufacturer ID
/// (The firmware sends 0x57010000 for decimal value 343)
///
#define INTEL_IANA_SWAP32(x)      (UINT32)((((x) & 0xff) << 24) | (((x) & 0xff00) << 8) | \
                                    (((x) & 0xff0000) >> 8) | (((x) & 0xff000000) >> 24))
#define ASF_INTEL_IANA            0x00000157
#define ASF_INTEL_CONVERTED_IANA  INTEL_IANA_SWAP32 (ASF_INTEL_IANA)  ///< 0X57010000, received from ME

///
/// Intel OEM Special Command
///
#define ASF_INTEL_OEM_CMD                  0xC1
#define ASF_INTEL_OEM_FORCE_HTTPS_BOOT_CMD 0xC2
#define ASF_INTEL_OEM_FORCE_PBA_BOOT_CMD   0xC3

///
/// Intel OEM Parameters Byte1 values
///
#define USE_SOL                            BIT0 ///< 0000 0001 - bit 0, use SOL on the next boot

///
/// Intel Special Command Parameter Low byte values
///
#define USE_STORAGE_REDIR                  BIT0  ///< 0000 0001 - bit 0 High Byte will be used to indicate the channel
#define ENFORCE_SECURE_BOOT                BIT1  ///< 0000 0010 - bit 1 Enforce secure boot over Storage Redirection
#define REFLASH_BIOS                       BIT2  ///< 0000 0100 - bit 2
#define BIOS_SETUP                         BIT3  ///< 0000 1000 - bit 3
#define BIOS_PAUSE                         BIT4  ///< 0001 0000 - bit 4
#define USE_KVM                            BIT5  ///< 0010 0000 - bit 5
#define ENABLE_SECURE_ERASE                BIT6  ///< 0100 0000 - bit 6 @Deprecated.
#define ENABLE_REMOTE_PLATFORM_ERASE       BIT7  ///< 1000 0000 - bit 7

///
/// Intel Special Command Parameter High byte values
///
/**
  00: SRoU is connected to CD/DVD device
  01: SRoU is connected to floppy device
**/
#define STORAGE_REDIRECTION_BOOT_DEVICE_MASK   (BIT1 | BIT0)

///
/// Intel specific definitions end
///

///
/// Alert Standard Format Protocol
/// This protocol provides interfaces to get ASF Boot Options
/// HECI protocol is consumed and used to send ASF messages and receive ASF Boot Options
///
typedef struct {
  ASF_BOOT_OPTIONS        AsfBootOptions;
} ALERT_STANDARD_FORMAT_PROTOCOL;

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gAlertStandardFormatProtocolGuid;

#endif
