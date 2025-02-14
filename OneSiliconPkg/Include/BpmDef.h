/** @file
  This file contains BPM Structures, defined in Boot Guard Spec.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef __BPM_DEF_H__
#define __BPM_DEF_H__

#include <IndustryStandard/FirmwareInterfaceTable.h>

#pragma pack (1)

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
#define SHA512_DIGEST_SIZE                  64

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

typedef union {
    UINT8   digest8[SHA512_DIGEST_SIZE];
    UINT32  digest32[SHA512_DIGEST_SIZE / 4];
    UINT64  digest64[SHA512_DIGEST_SIZE / 8];
} BtgSha512Digest_u;

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
  BtgSha512Digest_u    HashBuffer;
} SHA512_HASH_STRUCTURE;

typedef struct {
  UINT16               HashAlg;
  UINT16               Size;
  UINT8                HashBuffer[];
} SHAX_HASH_STRUCTURE;

typedef struct {
  UINT16               Size;           // Total number of bytes of HASH_LIST structure
  UINT16               Count;          // Number of Digest elements
} HASH_LIST;

typedef struct {
  UINT16                 Size;         // Total number of bytes of HASH_LIST structure
  UINT16                 Count;        // Number of Digest elements
  SHA384_HASH_STRUCTURE  Sha384Digest; // Array of digests  {AlgID, Size, HashValue; ...}
  SHA1_HASH_STRUCTURE    Sha1Digest;   // Array of digests  {AlgID, Size, HashValue; ...}
  SHA256_HASH_STRUCTURE  Sha256Digest; // Array of digests  {AlgID, Size, HashValue; ...}
  SHA512_HASH_STRUCTURE  Sha512Digest; // Array of digests  {AlgID, Size, HashValue; ...}
  SHA256_HASH_STRUCTURE  ShaSm3Digest; // Array of digests  {AlgID, Size, HashValue; ...}
} MAX_HASH_LIST;

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

#define BPM_CNBS_ELEMENT_STRUCTURE_ID (*(UINT64 *)"__CNBS__")
#define BPM_PDRS_ELEMENT_STRUCTURE_ID (*(UINT64 *)"__PDRS__")
#define BPM_BSIS_ELEMENT_STRUCTURE_ID (*(UINT64 *)"__BSIS__")
#define BPM_BSIS_ELEMENT_VERSION 0x10
typedef struct {
  UINT8               StructureID[8];
  UINT8               StructVersion; // 0x20
  UINT8               Reserved;
  UINT16              SizeOfData;
  UINT8               Data[];
} BSIS_SEGMENT;

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
  UINT64               Usage;          // Bit mask of usages
  SHAX_HASH_STRUCTURE  Digest;         // Standard BtG hash structure primitive
} SHAX_KMHASH_STRUCT;

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

#pragma pack ()

#endif
