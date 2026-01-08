/**@file
  Header file for the MeSmbios Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#ifndef _ME_SMBIOS_H_
#define _ME_SMBIOS_H_

#include <Library/BaseLib.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
#include <MkhiMsgs.h>

#define MESMBIOS_TYPE_130                           130
#define MESMBIOS_TYPE_131                           131

///
/// ExtendedData
///
#define EXTENDED_DATA_RESERVED_BITMASK              BIT0 | BIT2 | BIT5 | BIT7

///
/// OEMCapabilities1
///
#define OEM_CAPS_1_RESERVED_BITMASK                 BIT0 | BIT1 | BIT5 | BIT6 | BIT7

#define OEM_CAPS_1_BIOS_REFLASH_BIT_OFFSET          2
#define OEM_CAPS_1_BIOS_SETUP_BIT_OFFSET            3
#define OEM_CAPS_1_BIOS_PAUSE_BIT_OFFSET            4

///
/// OEMCapabilities2
///
#define OEM_CAPS_2_TERMINAL_EMULATION_VTUTF8        0x03
#define OEM_CAPS_2_TERMINAL_EMULATION_PCANSI        0x04

///
/// OEMCapabilities3
///
#define OEM_CAPS_3_SECURE_ERASE_BIT_OFFSET          6
#define OEM_CAPS_3_BIOS_SECURE_BOOT_BIT_OFFSET      7

///
/// OEMCapabilities4
///
#define OEM_CAPS_4_VPRO_TBT_DOCK_BIT_OFFSET         0
#define OEM_CAPS_4_OCR_HTTPS_BOOT_BIT_OFFSET        1
#define OEM_CAPS_4_OCR_PBA_BOOT_BIT_OFFSET          2
#define OEM_CAPS_4_OCR_WINRE_BOOT_BIT_OFFSET        3
#define OEM_CAPS_4_OCR_AMT_DIS_SEC_BOOT_BIT_OFFSET  4
#define OEM_CAPS_4_OCR_WIFI_PROFILE_BIT_OFFSET      5

#pragma pack(1)
///
/// This is definition for SMBIOS Oem data type 130
///
typedef struct {
  SMBIOS_STRUCTURE  Header;
  UINT8             AmtSignature[4];  ///< Table signature
  UINT8             AmtSupported;     ///< Any manageability supported, includes full or standard manageability
  UINT8             AmtEnabled;       ///< Any manageability enabled, includes full or standard manageability
  UINT8             SrouEnabled;      ///< Storage Redirection Over USB-R feature enablement
  UINT8             SolEnabled;       ///< Serial Over Lan feature enablement
  UINT8             NetworkEnabled;   ///< Network interface enablement
  UINT8             ExtendedData;     ///< Kept for backward compatibility
  UINT8             OemCapabilities1; ///< 1st Byte of OEM Capabilities
  UINT8             OemCapabilities2; ///< 2nd Byte of OEM Capabilities
  UINT8             OemCapabilities3; ///< 3rd Byte of OEM Capabilities
  UINT8             OemCapabilities4; ///< 4th Byte of OEM Capabilities
  UINT8             KvmEnabled;       ///< Kvm redirection enablement
  UINT8             Reserved;         ///< Reserved
  UINT32            RpeCapabilities;  ///< Remote Platform Erase Capbilities, keeps it at the end and 4 byte alignment
} SMBIOS_TABLE_TYPE130;

typedef struct {
  UINT32  VMXState          : 1;    ///< [0]     VMX enabled state (On/Off) (This bit is a duplicate of Bit 5 and maintained for backward compatibility)
  UINT32  SMXState          : 1;    ///< [1]     SMX enabled state (On/Off) (This bit is a duplicate of Bit 3 and maintained for backward compatibility)
  UINT32  LtTxtCap          : 1;    ///< [2]     LT/TXT capability
  UINT32  LtTxtEnabled      : 1;    ///< [3]     LT/TXT Enabled state (Optional for vPro verification)
  UINT32  VTxCap            : 1;    ///< [4]     VT-x capability
  UINT32  VTxEnabled        : 1;    ///< [5]     VT-x Enabled state (Optional for vPro verification)
  UINT32  VTrpSupport       : 1;    ///< [6]     VT-rp capability support
  UINT32  BiosGuardEnabled  : 1;    ///< [7]     BIOS Guard Enabled state
  UINT32  Reserved          : 24;   ///< [31:8]  Reserved, set to 0
} CPU_CAP;

typedef struct {
  UINT32  FunctionNumber : 3;  ///< [2:0]     PCI Device Function Number
  UINT32  DeviceNumber   : 5;  ///< [7:3]     PCI Device Device Number
  UINT32  BusNumber      : 8;  ///< [15:8]    PCI Device Bus Number
  UINT32  DeviceID       : 16; ///< [31:16]   Device Identification Number (DID): This field is the identifier of PCH PCI device. DID will be set to 0xFF if not found.
  UINT32  Reserved       : 32; ///< [63:32]   Reserved, must set to 0
} PCH_CAP;

///
/// ME Platform Capabilities
///
typedef struct {
  UINT32  MeEnabled          : 1;  ///< [0]     ME enabled/Disabled
  UINT32  Reserved           : 2;  ///< [2:1]   Reserved, must set to 0
  UINT32  IntelAmtFw         : 1;  ///< [3]     Intel AMT FW support
  UINT32  IntelAmtFwStandard : 1;  ///< [4]     Intel AMT Standard FW support
  UINT32  IntelAmtEnabled    : 1;  ///< [5]     Intel AMT Enabled
  UINT32  Reserved1          : 7;  ///< [12:6]  Reserved
  UINT32  IntelKVMEnabled    : 1;  ///< [13]    Intel KVM enabled
  UINT32  IntelKVM           : 1;  ///< [14]    Intel KVM supported
  UINT32  LocalWakeupTimer   : 1;  ///< [15]    Local Wakeup Timer support
  UINT32  Reserved2          : 16; ///< [31:16] Reserved, must set to 0
  UINT32  MeMinorVer         : 16; ///< [47:32] ME FW Minor Version.
  UINT32  MeMajorVer         : 16; ///< [63:48] ME FW Major Version.
  UINT32  MeBuildNo          : 16; ///< [79:64] ME FW Build Number.
  UINT32  MeHotFixNo         : 16; ///< [95:80] ME FW Hotfix Number
} ME_CAP;

typedef struct {
  UINT32  FunctionNumber : 3;  ///< [2:0]   PCI Device Function Number of Wired LAN
  UINT32  DeviceNumber   : 5;  ///< [7:3]   PCI Device Device Number of Wired LAN
  UINT32  BusNumber      : 8;  ///< [15:8]  PCI Device Bus Number of Wired LAN
  UINT32  DeviceID       : 16; ///< [31:16] PCI Device Identifier (DID) of wired NIC. DID will be set to 0xFF if not found.
  UINT32  Reserved1      : 16; ///< [47:32] Reserved for Wired NIC.
  UINT32  Reserved2      : 16; ///< [63:48] Reserved, must set to 0 (Wireless)
  UINT32  Reserved3      : 32; ///< [95:64] Reserved, must set to 0 (Wireless)
} NETWORK_DEV;

typedef struct {
  UINT32  Reserved1           : 1;    ///< [0]    Reserved, must be set to 0
  UINT32  VTdSupport          : 1;    ///< [1]    BIOS supports VT-d in BIOS setup screen (can be editable)
  UINT32  TxtSupport          : 1;    ///< [2]    BIOS supports TXT in BIOS setup screen (can be editable)
  UINT32  Reserved2           : 1;    ///< [3]    Reserved, must be set to 0
  UINT32  Reserved3           : 1;    ///< [4]    Reserved, must be set to 0
  UINT32  VTxSupport          : 1;    ///< [5]    BIOS supports VT-x in BIOS setup screen (can be editable)
  UINT32  IsrdIssrCapability  : 1;    ///< [6]    BIOS supports ISRD & ISSR
  UINT32  Reserved4           : 1;    ///< [7]    Reserved, must be set to 0
  UINT32  NvmeRecoveryEnabled : 1;    ///< [8]    BIOS supports NVMe_Based Resiliency
  UINT32  Reserved5           : 23;   ///< [31:9] Reserved, must be set to 0
} BIOS_CAP;

///
/// This is definition for SMBIOS Oem data type 131
///
typedef struct {
  SMBIOS_STRUCTURE  Header;
  CPU_CAP           CpuCapabilities;
  UINT8             Reserved[8];
  PCH_CAP           PchCapabilities;
  ME_CAP            MeCapabilities;
  MEFWCAPS_SKU      MeFwConfigured;
  NETWORK_DEV       NetworkDevice;
  BIOS_CAP          BiosCapabilities;
  UINT8             StructureIdentifier[4];
  UINT32            Reserved1;
} SMBIOS_TABLE_TYPE131;
#pragma pack()
#endif
