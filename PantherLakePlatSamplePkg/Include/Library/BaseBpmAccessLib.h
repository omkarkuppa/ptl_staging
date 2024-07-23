/** @file
  This library contains functions for accessing the BPM/KM and other ACM functionality

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

#include <Protocol/Tcg2Protocol.h>

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

//
// BPM Policy:
//   FIT record type 12 points to Boot Policy Manifest.
//   FIT record type 11 points to Key Manifest.
//

#define RSA_KEY_SIZE_1K                     SIZE_1KB
#define RSA_KEY_SIZE_2K                     SIZE_2KB
#define RSA_KEY_SIZE_3K                     (SIZE_1KB + SIZE_2KB)
#define PKCS_1_5_RSA_SHA1_SIGNATURE_SIZE    (RSA_KEY_SIZE_1K / 8)
#define PKCS_1_5_RSA_SHA256_SIGNATURE_SIZE  (RSA_KEY_SIZE_2K / 8)
#define PKCS_1_5_RSA_SHA384_SIGNATURE_SIZE  (RSA_KEY_SIZE_3K / 8)
#define SHA1_DIGEST_SIZE                    20
#define SHA256_DIGEST_SIZE                  32
#define SHA384_DIGEST_SIZE                  48
#define SM3_256_DIGEST_SIZE                 32

#define MAX_SUPPORTED_SIGNATURE_SIZE        RSA_KEY_SIZE_3K/8

typedef union {
    UINT8   digest8[SHA1_DIGEST_SIZE];
    UINT32  digest32[SHA1_DIGEST_SIZE / 4];
} BtgSha1Digest_u;

typedef union {
    UINT8   digest8[SHA256_DIGEST_SIZE];
    UINT32  digest32[SHA256_DIGEST_SIZE / 4];
    UINT64  digest64[SHA256_DIGEST_SIZE / 8];
} BtgSha256Digest_u;

typedef union {
    UINT8   digest8[SHA384_DIGEST_SIZE];
    UINT32  digest32[SHA384_DIGEST_SIZE / 4];
    UINT64  digest64[SHA384_DIGEST_SIZE / 8];
} BtgSha384Digest_u;

//
// Hash structure
//
typedef struct {
  UINT16               HashAlg;
  UINT16               Size;
  BtgSha1Digest_u      HashBuffer;
} SHA1_HASH_STRUCTURE;

typedef struct {
  UINT16               HashAlg;
  UINT16               Size;
  BtgSha256Digest_u    HashBuffer;
} SHA256_HASH_STRUCTURE;

typedef struct {
  UINT16               HashAlg;
  UINT16               Size;
  BtgSha384Digest_u    HashBuffer;
} SHA384_HASH_STRUCTURE;

typedef struct {
  UINT16               HashAlg;
  UINT16               Size;
  UINT8                HashBuffer[];
} SHAX_HASH_STRUCTURE;

typedef struct {
  UINT16               Size;           //Total number of bytes of HASH_LIST structure
  UINT16               Count;          //Number of Digest elements
} HASH_LIST;

typedef struct {
  UINT16                 Size;         //Total number of bytes of HASH_LIST structure
  UINT16                 Count;        //Number of Digest elements
  SHA1_HASH_STRUCTURE    Sha1Digest;   //Array of digests  {AlgID, Size, HashValue; ...}
  SHA256_HASH_STRUCTURE  Sha256Digest; //Array of digests  {AlgID, Size, HashValue; ...}
  SHA256_HASH_STRUCTURE  ShaSm3Digest; //Array of digests  {AlgID, Size, HashValue; ...}
  SHA384_HASH_STRUCTURE  Sha384Digest; //Array of digests  {AlgID, Size, HashValue; ...}
} MAX_HASH_LIST;

typedef struct {
  UINT64               Usage;          // Bit mask of usages
  SHAX_HASH_STRUCTURE  Digest;         // Standard BtG hash structure primitive
} SHAX_KMHASH_STRUCT;


#define ALG_RSA             0x1
#define ALG_RSASSA          0x14

#define RSA_PUBLIC_KEY_STRUCT_VERSION_1_0  0x10
#define RSA_PUBLIC_KEY_STRUCT_KEY_EXPONENT_DEFAULT  0x11 // NOT 0x10001

typedef struct {
  UINT8    Version;                    // 0x10
  UINT16   KeySizeBits;                // 1024 or 2048 or 3072 bits
} KEY_STRUCT_HEADER;

typedef struct {
  UINT8    Version;                    // 0x10
  UINT16   KeySizeBits;                // Number of bits in the modulus.
  UINT32   Exponent;                   // The public exponent (must be RSA_KEY_EXPONENT_VALUE)
  UINT8    Modulus[RSA_KEY_SIZE_2K/8]; // The modulus in LSB format (256 bytes)
} RSA2K_PUBLIC_KEY_STRUCT;

typedef struct {
  UINT8    Version;                    // 0x10
  UINT16   KeySizeBits;                // Number of bits in the modulus.
  UINT32   Exponent;                   // The public exponent (must be RSA_KEY_EXPONENT_VALUE)
  UINT8    Modulus[RSA_KEY_SIZE_3K/8]; // The modulus in LSB format (384 bytes)
} RSA3K_PUBLIC_KEY_STRUCT;

typedef struct {
  UINT8    Version;                    // 0x10
  UINT16   KeySizeBits;                // 1024 or 2048 or 3072 bits
  UINT32   Exponent;                   // The public exponent
  UINT8    Modulus[1];                 // The modulus in LSB format
} RSA_PUBLIC_KEY_STRUCT;

#define ECC_PUBLIC_KEY_STRUCT_VERSION_1_0  0x10
#define ECC_PUBLIC_KEY_STRUCT_KEY_SIZE_DEFAULT  256
#define ECC_PUBLIC_KEY_STRUCT_KEY_LEN_DEFAULT   (ECC_PUBLIC_KEY_STRUCT_KEY_SIZE_DEFAULT/8)
typedef struct {
  UINT8  Version;                      // 0x10
  UINT16 KeySizeBits;                  // 256 - Number of bits in key. Fixed for SM2
  UINT8  Qx[ECC_PUBLIC_KEY_STRUCT_KEY_LEN_DEFAULT];  // X component. Fixed size for SM2
  UINT8  Qy[ECC_PUBLIC_KEY_STRUCT_KEY_LEN_DEFAULT];  // Y component. Fixed size for SM2
} ECC_PUBLIC_KEY_STRUCT;

#define RSASSA_SIGNATURE_STRUCT_VERSION_1_0  0x10

typedef struct {
  UINT8    Version;
  UINT16   SigSizeBits;                // 2048 or 3072 bits
} SIGNATURE_STRUCT_HEADER;

typedef struct {
  UINT8    Version;
  UINT16   KeySizeBits;                // 2048 or 3072 bits
  UINT16   HashAlg;
  UINT8    Signature[];
} RSASSA_SIGNATURE_STRUCT;

#define ECC_SIGNATURE_STRUCT_VERSION_1_0  0x10
typedef struct {
  UINT8    Version;
  UINT16   KeySizeBits;                // 256 or 384 bits
  UINT16   HashAlg;
} ECC_SIGNATURE_STRUCT;

#define KEY_SIGNATURE_STRUCT_VERSION_1_0  0x10
typedef struct {
  UINT8                      Version;
  UINT16                     KeyAlg;       // TPM_ALG_RSA=0x1 or TPM_ALG_ECC=0x23
} KEY_AND_SIGNATURE_STRUCT_HEADER;
typedef struct {
  UINT8                      Version;
  UINT16                     KeyAlg;       // TPM_ALG_RSA=0x1 or TPM_ALG_ECC=0x23
  union {                                  // Based on KeyAlg
    RSA_PUBLIC_KEY_STRUCT    RsaKey;
    ECC_PUBLIC_KEY_STRUCT    EccKey;
  } Key;
  UINT16                     SigScheme;    // TPM_ALG_RSASSA=0x14 or TPM_ALG_RSAPSS=0x15 or TPM_ALG_SM2=0x1B
  union {                                  // Based on KeyAlg
    RSASSA_SIGNATURE_STRUCT  SignatureRsa;
    ECC_SIGNATURE_STRUCT     SignatureEcc;
  } Sig;
} KEY_AND_SIGNATURE_STRUCT;

#define BP_KEY_TYPE_BOOT_POLICY_MANIFEST  0
#define BP_KEY_TYPE_KEY_MANIFEST          1

#define BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_ID  (*(UINT64 *)"__ACBP__")
#define BOOT_POLICY_MANIFEST_HEADER_VERSION_2_5          0x25
typedef struct {
  UINT8              StructureId[8];
  UINT8              StructVersion;        // 0x25
  UINT8              HdrStructVersion;     // 0x20
  UINT16             HdrSize;              // total number of bytes in Header (i.e., offset to first element)
  UINT16             KeySignatureOffset;   // Offset from start of Bpm to KeySignature field of Signature Element
  UINT8              BpmRevision;
  UINT8              BpmRevocation;
  UINT8              AcmRevocation;
  UINT8              Reserved;
  UINT16             NemPages;
} BOOT_POLICY_MANIFEST_HEADER;

#define IBB_SEGMENT_FLAG_IBB     0x0
#define IBB_SEGMENT_FLAG_NON_IBB 0x1
typedef struct {
  UINT16             Reserved;         // Alignment
  UINT16             Flags;            // Control flags
  UINT32             Base;             // Segment base
  UINT32             Size;             // Segment size
} REGION_SEGMENT;

#define BOOT_POLICY_MANIFEST_IBB_ELEMENT_STRUCTURE_ID  (*(UINT64 *)"__IBBS__")
#define BOOT_POLICY_MANIFEST_IBB_ELEMENT_DIGEST_ID     (*(UINT64 *)"__DIGE__")

#define BOOT_POLICY_MANIFEST_IBB_ELEMENT_VERSION_2_0       0x21
#define IBB_FLAG_INITIAL_MEASURE_LOC3  0x2
#define IBB_FLAG_AUTHORITY_MEASURE     0x4
//
// BIT7 will be used to control the stability of PCR[0] by masking out mutable policies from
// ACM_BIOS_POLICY_STS (0xfed30378) which is part of extend value into PCR[0]
//
#define IBB_FLAG_SRTM_AC               0x80 //Bit 7
#define SRTM_AC_MASK                   0x20FFF

typedef struct {
  UINT8               StructureId[8];
  UINT8               StructVersion;   // 0x20
  UINT8               Reserved0;
  UINT16              ElementSize;     // Total number of bytes in the element
  UINT8               Reserved1;
  UINT8               SetType;
  UINT8               Reserved;
  UINT8               PbetValue;
  UINT32              Flags;
  UINT64              IbbMchBar;
  UINT64              VtdBar;
  UINT32              DmaProtBase0;
  UINT32              DmaProtLimit0;
  UINT64              DmaProtBase1;
  UINT64              DmaProtLimit1;
  SHAX_HASH_STRUCTURE PostIbbHash;
} IBB_ELEMENT;

#define BOOT_POLICY_MANIFEST_TXT_ELEMENT_STRUCTURE_ID  (*(UINT64 *)"__TXTS__")
#define BOOT_POLICY_MANIFEST_TXT_ELEMENT_VERSION_2_0       0x22
typedef struct {
  UINT8               StructureId[8];
  UINT8               StructVersion;   // 0x20
  UINT8               Reserved0;
  UINT16              ElementSize;     // Total number of bytes in the element
  UINT8               Reserved1;
  UINT8               SetType;
  UINT16              Reserved;
  UINT32              Flags;
  UINT16              PwrDownInterval;
  UINT8               PttCmosOffset0;
  UINT8               PttCmosOffset1;
  UINT16              AcpiBaseOffset;
  UINT16              Reserved2;
  UINT32              PrwmBaseOffset;
  HASH_LIST           DigestList;
  UINT8               Reserved3[3];
  UINT8               SegmentCount;
  REGION_SEGMENT*     TxtSegment;      // TxtSegment[SegmentCount]
} TXT_ELEMENT;

typedef struct {
  UINT8               SetNumber;
  UINT8               Reserved[3];
  UINT8               Data[48];
} BIOS_SPECIFIC_SEGMENT_STRUCTURE;

#define BOOT_POLICY_MANIFEST_BIOS_SPECIFIC_IBB_STRUCTURE_STRUCTURE_ID  (*(UINT64 *)"__BSIS__")
#define BOOT_POLICY_MANIFEST_BIOS_SPECIFIC_IBB_STRUCTURE_VERSION_2_0       0x20
typedef struct {
  UINT8                             StructureId[8];
  UINT8                             StructVersion;    // 0x10
  UINT16                            SizeOfData;
  UINT8                             Reserved0;
  BIOS_SPECIFIC_SEGMENT_STRUCTURE*  Data;             // BSIS data. Could be an array of BSSS structures - BSSS []
} BIOS_SPECIFIC_IBB_STRUCTURE;

#define BPM_CNBS_ELEMENT_STRUCTURE_ID (*(UINT64 *)"__CNBS__")
#define BPM_CNBS_ELEMENT_VERSION 0x10
typedef struct {
  UINT8               StructureID[8];
  UINT8               StructVersion; // 0x10
  UINT16              SizeOfData;
  UINT8               Reserved;
  REGION_SEGMENT      BufferData;
} CNBS_SEGMENT;

#define BPM_SDBS_ELEMENT_STRUCTURE_ID (*(UINT64 *)"__SDBS__")

#define BOOT_POLICY_MANIFEST_PLATFORM_CONFIG_DATA_ELEMENT_STRUCTURE_ID  (*(UINT64 *)"__PCDS__")
#define BOOT_POLICY_MANIFEST_PLATFORM_CONFIG_DATA_ELEMENT_VERSION_3_0       0x30
typedef struct {
  UINT8                        StructureId[8];
  UINT8                        StructVersion;   // 0x30
  UINT8                        Reserved0;
  UINT16                       ElementSize;     // Total number of bytes in the element
  UINT16                       Reserved1;
  UINT16                       SizeOfData;
  UINT8                        PdrsData[20];    //Data[SizeofData]  // PDRS
  CNBS_SEGMENT                 Cnbs;
  BIOS_SPECIFIC_IBB_STRUCTURE* Data2;           //Data2[SizeOfData2]  // BSIS
} PLATFORM_CONFIG_DATA_ELEMENT;

#define BOOT_POLICY_MANIFEST_PLATFORM_MANUFACTURER_ELEMENT_STRUCTURE_ID  (*(UINT64 *)"__PMDA__")
#define BOOT_POLICY_MANIFEST_PLATFORM_MANUFACTURER_ELEMENT_VERSION_2_0       0x20
typedef struct {
  UINT8               StructureId[8];
  UINT8               StructVersion;       // 0x20
  UINT8               Reserved0;
  UINT16              ElementSize;         // Total number of bytes in the element
  UINT16              Reserved1;
  UINT16              PmDataSize;          // required to be 4 byte multiple
  UINT8*              PmData;              // PmData[PmDataSize]
} PLATFORM_MANUFACTURER_ELEMENT;
#define BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_STRUCTURE_ID  (*(UINT64 *)"__PMSG__")
#define BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_VERSION_1_0       0x10
typedef struct {
  UINT8               StructureId[8];
  UINT8               StructVersion;       // 0x20
  UINT8               Reserved[3];         // KeySignature must be DWORD aligned
  KEY_AND_SIGNATURE_STRUCT  KeySignature;  // this is now a variable Size
} BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT;

#define KEY_MANIFEST_STRUCTURE_ID  (*(UINT64 *)"__KEYM__")
#define KEY_MANIFEST_STRUCTURE_VERSION_2_1     0x21
typedef struct {
  UINT8               StructureId[8];
  UINT8               StructVersion;       // 0x21
  UINT8               reserved[3];         // 3 bytes to make KeySignatureOffset same offset as for BPM
  UINT16              KeySignatureOffset;  // Offset from start of KM to KeyManifestSignature
  UINT8               Reserved2[3];        // Alignment
  UINT8               KeyManifestRevision;
  UINT8               KmSvn;
  UINT8               KeyManifestId;
  UINT16              KmPubKeyHashAlg;
  UINT16              KeyCount;
} KEY_MANIFEST_STRUCTURE;

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
