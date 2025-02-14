/** @file
  This library contains functions for accessing the BPM/KM and other ACM functionality

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

@par Specification
**/

#include <Protocol/Tcg2Protocol.h>
#include <BpmDef.h>

//
// Data structure definitions
//
#pragma pack (1)

//
// ACM definition
//
#define ACM_MODULE_TYPE_CHIPSET_ACM                     2
#define ACM_MODULE_SUBTYPE_CAPABLE_OF_EXECUTE_AT_RESET  0x1
#define ACM_MODULE_SUBTYPE_ANC_MODULE                   0x2
#define ACM_HEADER_FLAG_DEBUG_SIGNED                    BIT15
#define ACM_NPW_SVN                                     0x2

typedef struct {
  UINT16     ModuleType;
  UINT16     ModuleSubType;
  UINT32     HeaderLen;
  UINT32     HeaderVersion;
  UINT16     ChipsetId;
  UINT16     Flags;
  UINT32     ModuleVendor;
  UINT32     Date;
  UINT32     Size;
  UINT16     AcmSvn;
  UINT16     SeAcmSvn;
  UINT32     CodeControl;
  UINT32     ErrorEntryPoint;
  UINT32     GdtLimit;
  UINT32     GdtBasePtr;
  UINT32     SegSel;
  UINT32     EntryPoint;
  UINT8      Rsvd2[64];
  UINT32     KeySize;            // 64 DWORDS in the Key
  UINT32     ScratchSize;
  UINT8      Rsa2048PubKey[256];
  UINT32     RsaPubExp;
  UINT8      Rsa2048Sig[256];
  UINT8      Scratch[572];       // 143 DWORDS = 572 BYTES Scratch Size
} ACM_HEADER;

typedef struct {
  UINT16     ModuleType;
  UINT16     ModuleSubType;
  UINT32     HeaderLen;
  UINT32     HeaderVersion;
  UINT16     ChipsetId;
  UINT16     Flags;
  UINT32     ModuleVendor;
  UINT32     Date;
  UINT32     Size;
  UINT16     AcmSvn;
  UINT16     SeAcmSvn;
  UINT32     CodeControl;
  UINT32     ErrorEntryPoint;
  UINT32     GdtLimit;
  UINT32     GdtBasePtr;
  UINT32     SegSel;
  UINT32     EntryPoint;
  UINT8      Rsvd2[64];
  UINT32     KeySize;            // 96 DWORDS in the Key
  UINT32     ScratchSize;
  UINT8      Rsa3072PubKey[384];
  UINT8      Rsa3072Sig[384];
  UINT8      Scratch[832];       // 208 DWORDS = 832 BYTES Scratch Size
} ACM_HEADER_3;

#define MMIO_ACM_POLICY_STATUS  (TXT_PUBLIC_BASE + R_CPU_ACM_POLICY_STATUS)  // 0xFED30378
#define SCRTM_VERSION_UTF16_LENGTH                      13

typedef struct {
  UINT16 ModuleType;
  UINT16 ModuleSubType;
  UINT32 HeaderLen;
  UINT32 HeaderVersion;
  UINT16 ModuleID;
  UINT16 ModuleFlags;
  UINT32 ModuleVendor;
  UINT32 Revision;
  UINT32 Sizeb;
  UINT16 AcmSvn;
  UINT16 SeSvn;
  UINT32 CodeControl;
  UINT32 ErrorEntryPoint;
  UINT32 GDTSize;
  UINT32 GDTBasePtr;
  UINT32 SegSel;
  UINT32 EntryPoint;
  UINT8  Reserved2[64];
  UINT32 KeySize;     // 96 DWORDS in the Key
  UINT32 ScratchSize; // 208 DWORDS = 832 BYTES Sractch Size
  UINT8  RSA3072PubKey[384];
  UINT8  RSA3072Sig[384];
  UINT8 XMSSPubKey[64];
  UINT8 XMSSSig[2692];
  UINT8 Reserved3[60];
  UINT8 scratch[3584];
} ACM_HEADER_4;

#define BPM_SDBS_ELEMENT_STRUCTURE_ID (*(UINT64 *)"__SDBS__")
#define BPM_PDRS_ELEMENT_STRUCTURE_ID (*(UINT64 *)"__PDRS__")
typedef struct {
  UINT8               StructureId[8];
  UINT8               StructVersion;   // 0x20
  UINT8               Reserved;
  UINT16              SizeOfData;
  UINT8*              Data;
} POWER_DOWN_REQUEST_STRUCTURE;

#define PDR_MEDIA_TPM_12      0
#define PDR_MEDIA_TPM_20      1
#define PDR_MEDIA_PTT         2
#define PDR_MEDIA_CMOS        3

typedef struct {
  UINT8               MediaType;
  UINT32              NvIndex;
  UINT8               BitfieldWidth;
  UINT8               BitfieldPos;
  UINT8               ByteOffset;
} TPM_NV_ADDRESS_STRUCTURE;

//
// ACM Policy data
//
typedef struct {
  UINT64 AcmPolicyStatus;
  UINT8  ManifestKeyHashes[];
} ACM_POLICY_DATA;

typedef struct {
  UINT64 AcmPolicyStatus;
  UINT8  KmKeyHash[MAX_SUPPORTED_SIGNATURE_SIZE];
  UINT8  BpmKeyHashFromKm[MAX_SUPPORTED_SIGNATURE_SIZE];
  UINT8  FbmKeyHash[MAX_SUPPORTED_SIGNATURE_SIZE];
} MAX_ACM_POLICY_DATA;

//
// Efi Startup Locality Event Data
//
typedef struct {
  UINT8   Signature[16];
  UINT8   StartupLocality;
} TCG_EFI_STARTUP_LOCALITY_EVENT;

typedef struct {
  EFI_GUID                  *EventGuid;
  EFI_TCG2_EVENT_LOG_FORMAT  LogFormat;
} EFI_TCG2_EVENT_INFO_STRUCT;

typedef union {
  struct {
    UINT64 KmId               : 4;      // 0-3   Key Manifest ID used for verified Key Manifest
    UINT64 MeasuredBoot       : 1;      // 4     perform measured boot
    UINT64 VerifiedBoot       : 1;      // 5     perform verified boot
    UINT64 HAP                : 1;      // 6     high assurance platform
    UINT64 TxtSupported       : 1;      // 7     txt supported
    UINT64 BootMedia          : 1;      // 8     Boot media
    UINT64 DCD                : 1;      // 9     disable CPU debug
    UINT64 DBI                : 1;      // 10    disable BSP init
    UINT64 PBE                : 1;      // 11    protect BIOS environment
    UINT64 BBP                : 1;      // 12    bypass boot policy - fast S3 resume
    UINT64 TpmType            : 2;      // 13-14 TPM Type
    UINT64 TpmSuccess         : 1;      // 15    TPM Success
    UINT64 Reserved1          : 1;      // 16
    UINT64 BootPolicies       : 1;      // 17    PFR supported
    UINT64 BackupActions      : 2;      // 18-19 Backup actions
    UINT64 TxtProfile         : 5;      // 20-24 TXT profile selection
    UINT64 MemScrubPolicy     : 2;      // 25-26 Memory scrubbing policy
    UINT64 Reserved2          : 2;      // 27-28
    UINT64 DmaProtection      : 1;      // 29    DMA Protection
    UINT64 Reserved3          : 2;      // 30-31
    UINT64 SCrtmStatus        : 3;      // 32-34 S-CRTM status
    UINT64 Cosign             : 1;      // 35    CPU co-signing
    UINT64 TpmStartupLocality : 1;      // 36    TPM startup locality.
    UINT64 Reserved           :27;      // 37-63
  } Bits;
  UINT64 Data;
} ACM_BIOS_POLICY;

#pragma pack ()

/**
  Find FIT Entry address data by type

  @param[in] Type   FIT Entry type

  @return FIT entry address data
**/
VOID *
FindFitEntryData (
  IN UINT8   Type
  );

/**
  Find ACM address

  @return ACM address
**/
VOID *
FindAcm (
  VOID
  );

/**
  Find BPM address

  @return BPM address
**/
VOID *
FindBpm (
  VOID
  );

/**
  Find FBM address

  @return FBM address

**/
VOID *
FindFbm (
  VOID
  );

/**
  Find KM address

  @return KM address
**/
VOID *
FindKm (
  VOID
  );

/**
  Find BPM element by structureID

  @param[in] Bpm           BPM address
  @param[in] StructureID   BPM element StructureID

  @return BPM element
**/
VOID *
FindBpmElement (
  IN BOOT_POLICY_MANIFEST_HEADER              *Bpm,
  IN UINT64                                   StructureId
  );

/**
  Find BPM IBB element

  @param[in] Bpm           BPM address

  @return BPM IBB element
**/
VOID *
FindBpmIbb (
  IN BOOT_POLICY_MANIFEST_HEADER              *Bpm
  );

/**
  Find BPM Pcd element

  @param[in] Bpm           BPM address

  @return BPM Pcd element
**/
VOID *
FindBpmPcde (
  IN BOOT_POLICY_MANIFEST_HEADER              *Bpm
  );

/**
  Check if this NPW ACM

  @retval TRUE  This is NPW ACM
  @retval FALSE This is NOT NPW ACM
**/
BOOLEAN
IsNpwAcm (
  VOID
  );

/**
  Check if this is VerifiedBoot

  @retval TRUE  This is VerifiedBoot
  @retval FALSE This is NOT VerifiedBoot
**/
BOOLEAN
IsVerifiedBoot (
  VOID
  );

/**
  Check if this is MeasuredBoot

  @retval TRUE  This is MeasuredBoot
  @retval FALSE This is NOT MeasuredBoot
**/
BOOLEAN
IsMeasuredBoot (
  VOID
  );

/**
  Get ACM Policy Status

  @return ACM Policy Status
**/
UINT64
GetAcmPolicySts (
  VOID
  );

/**
  Dump raw data with colume format.

  @param[in]  Data  raw data
  @param[in]  Size  raw data size

**/
VOID
InternalDumpHex (
  IN UINT8  *Data,
  IN UINTN  Size
  );

/**
  Calculate SHA1 Hash

  @param[in]  Data   data
  @param[in]  Size   data size
  @param[out] Digest SHA1 digest
**/
VOID
CreateSha1Hash (
  IN  UINT8     *Data,
  IN  UINTN     Size,
  OUT UINT8     *Digest
  );

/**
  Calculate SHA256 Hash

  @param[in]  Data   data
  @param[in]  Size   data size
  @param[out] Digest SHA256 digest
**/
VOID
CreateSha256Hash (
  IN  UINT8     *Data,
  IN  UINTN     Size,
  OUT UINT8     *Digest
  );

/**
  Calculate SHA384 Hash

  @param[in]  Data   data
  @param[in]  Size   data size
  @param[out] Digest SHA384 digest
**/
VOID
CreateSha384Hash (
  IN  UINT8     *Data,
  IN  UINTN     Size,
  OUT UINT8     *Digest
  );

/**
   Calculate SHA512 Hash

   @param[in]  Data   data
   @param[in]  Size   data size
   @param[out] Digest SHA384 digest
**/
VOID
CreateSha512Hash (
  IN  UINT8     *Data,
  IN  UINTN     Size,
  OUT UINT8     *Digest
  );


/**
  Calculate SM3 Hash

  @param[in]  Data   data
  @param[in]  Size   data size
  @param[out] Digest SM3 digest
**/
VOID
CreateSm3Hash (
  IN  UINT8     *Data,
  IN  UINTN     Size,
  OUT UINT8     *Digest
  );
