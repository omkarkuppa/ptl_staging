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

#include <Library/BaseBpmAccessLib.h>

#include <PiPei.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BootGuardLib.h>

/**
  Find FIT Entry address data by type

  @param[in] Type   FIT Entry type

  @return FIT entry address data
**/
VOID *
FindFitEntryData (
  IN UINT8   Type
  )
{
  FIRMWARE_INTERFACE_TABLE_ENTRY  *FitEntry;
  UINT32                          EntryNum;
  UINT64                          FitTableOffset;
  UINT32                          Index;

  FitTableOffset = *(UINT64 *)(UINTN)(BASE_4GB - 0x40);
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)(UINTN)FitTableOffset;
  if ((FitEntry == NULL) || (FitEntry[0].Address != *(UINT64 *)"_FIT_   ")) {
    return NULL;
  }
  if (FitEntry[0].Type != FIT_TYPE_00_HEADER) {
    return NULL;
  }
  EntryNum = *(UINT32 *)(&FitEntry[0].Size[0]) & 0xFFFFFF;
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == Type) {
      return (VOID *)(UINTN)FitEntry[Index].Address;
    }
  }

  return NULL;
}

/**
  Find ACM address

  @return ACM address
**/
VOID *
FindAcm (
  VOID
  )
{
  return FindFitEntryData (FIT_TYPE_02_STARTUP_ACM);
}

/**
  Find BPM address

  @return BPM address
**/
VOID *
FindBpm (
  VOID
  )
{
  return FindFitEntryData (FIT_TYPE_0C_BOOT_POLICY_MANIFEST);
}

/**
  Find KM address

  @return KM address
**/
VOID *
FindKm (
  VOID
  )
{
  return FindFitEntryData (FIT_TYPE_0B_KEY_MANIFEST);
}

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
  )
{
  BOOT_POLICY_MANIFEST_HEADER             *BpmHeader;
  IBB_ELEMENT                             *IbbElement;
  TXT_ELEMENT                             *TxtElement;
  PLATFORM_CONFIG_DATA_ELEMENT            *PcdsElement;
  PLATFORM_MANUFACTURER_ELEMENT           *PmElement;
  BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT  *BpmSignatureElement;
  UINT8                                   *Buffer;

  Buffer = (UINT8 *)Bpm;

  BpmHeader = (BOOT_POLICY_MANIFEST_HEADER *)Buffer;
  if (*(UINT64 *)BpmHeader->StructureId != BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_ID) {
    return NULL;
  }
  if (StructureId == BOOT_POLICY_MANIFEST_HEADER_STRUCTURE_ID) {
    return Buffer;
  }
  Buffer += sizeof(BOOT_POLICY_MANIFEST_HEADER);
  IbbElement = (IBB_ELEMENT *)Buffer;
  do {
    if (*(UINT64 *)IbbElement->StructureId != BOOT_POLICY_MANIFEST_IBB_ELEMENT_STRUCTURE_ID) {
      return NULL;
    }
    if (StructureId == BOOT_POLICY_MANIFEST_IBB_ELEMENT_STRUCTURE_ID) {
      return Buffer;
    }
    if (StructureId == BOOT_POLICY_MANIFEST_IBB_ELEMENT_DIGEST_ID) {
      // Advance to end of IBB_ELEMENT structure
      Buffer = (UINT8*) &(IbbElement->PostIbbHash);
      Buffer += sizeof(UINT32) + IbbElement->PostIbbHash.Size;
      Buffer += sizeof(UINT32); //IbbEntryPoint
      return Buffer;
    } else {
      Buffer += IbbElement->ElementSize; // Go to the end of the IBB Element structure
    }
    IbbElement = (IBB_ELEMENT *)Buffer;
  } while (*(UINT64 *)IbbElement->StructureId == BOOT_POLICY_MANIFEST_IBB_ELEMENT_STRUCTURE_ID); // to support multiple IBB Elements

  // Do we have TXT element in BPM?
  // If so, advance to end of TXT_ELEMENT structure
  TxtElement = (TXT_ELEMENT *)Buffer;
  if (*(UINT64 *)TxtElement->StructureId == BOOT_POLICY_MANIFEST_TXT_ELEMENT_STRUCTURE_ID)
  {
    Buffer += TxtElement->ElementSize;
  }

  // Do we have Platform Config Data element in BPM?
  // If so, advance to end of PLATFORM_CONFIG_DATA_ELEMENT structure
  PcdsElement = (PLATFORM_CONFIG_DATA_ELEMENT *)Buffer;
  if (*(UINT64 *)PcdsElement->StructureId == BOOT_POLICY_MANIFEST_PLATFORM_CONFIG_DATA_ELEMENT_STRUCTURE_ID)
  {
    if (StructureId == BOOT_POLICY_MANIFEST_PLATFORM_CONFIG_DATA_ELEMENT_STRUCTURE_ID) {
      return Buffer;
    }
    if (StructureId == BPM_CNBS_ELEMENT_STRUCTURE_ID) {
      Buffer = (UINT8 *) &PcdsElement->Cnbs;
      return Buffer;
    }
    Buffer += PcdsElement->ElementSize;
  }

  // Do we have Platform Manufacturer element in BPM?
  // If so, advance to end of PLATFORM_MANUFACTURER_ELEMENT structure
  PmElement = (PLATFORM_MANUFACTURER_ELEMENT *)Buffer;
  while (*(UINT64 *)PmElement->StructureId == BOOT_POLICY_MANIFEST_PLATFORM_MANUFACTURER_ELEMENT_STRUCTURE_ID) {
    if (StructureId == BOOT_POLICY_MANIFEST_PLATFORM_MANUFACTURER_ELEMENT_STRUCTURE_ID) {
      return Buffer;
    }
    Buffer += sizeof(PLATFORM_MANUFACTURER_ELEMENT) + PmElement->PmDataSize;
    PmElement = (PLATFORM_MANUFACTURER_ELEMENT *)Buffer;
  }

  BpmSignatureElement = (BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT *)Buffer;
  if (*(UINT64 *)BpmSignatureElement->StructureId != BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_STRUCTURE_ID) {
    return NULL;
  }
  if (StructureId == BOOT_POLICY_MANIFEST_SIGNATURE_ELEMENT_STRUCTURE_ID) {
    return Buffer;
  }
  return NULL;
}

/**
  Find BPM IBB element

  @param[in] Bpm           BPM address

  @return BPM IBB element
**/
VOID *
FindBpmIbb (
  IN BOOT_POLICY_MANIFEST_HEADER              *Bpm
  )
{
  return FindBpmElement (Bpm, BOOT_POLICY_MANIFEST_IBB_ELEMENT_STRUCTURE_ID);
}

/**
  Find BPM Pcd element
  @param[in] Bpm           BPM address
  @return BPM Pcd element
**/
VOID *
FindBpmPcde (
  IN BOOT_POLICY_MANIFEST_HEADER              *Bpm
  )
{
  return FindBpmElement (Bpm, BOOT_POLICY_MANIFEST_PLATFORM_CONFIG_DATA_ELEMENT_STRUCTURE_ID);
}

/**
  Check if this NPW ACM

  @retval TRUE  This is NPW ACM
  @retval FALSE This is NOT NPW ACM
**/
BOOLEAN
IsNpwAcm (
  VOID
  )
{
  ACM_HEADER  *Acm;

  Acm = FindAcm ();
  ASSERT (Acm != NULL);
  if (Acm == NULL) {
    return FALSE;
  }

  if (((Acm->Flags & ACM_HEADER_FLAG_DEBUG_SIGNED) == 0) && (Acm->AcmSvn < ACM_NPW_SVN)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Check if this is VerifiedBoot

  @retval TRUE  This is VerifiedBoot
  @retval FALSE This is NOT VerifiedBoot
**/
BOOLEAN
IsVerifiedBoot (
  VOID
  )
{
  if ((AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Check if this is MeasuredBoot

  @retval TRUE  This is MeasuredBoot
  @retval FALSE This is NOT MeasuredBoot
**/
BOOLEAN
IsMeasuredBoot (
  VOID
  )
{
  if ((AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Get ACM Policy Status

  @return ACM Policy Status
**/
UINT64
GetAcmPolicySts (
  VOID
  )
{
  UINT64 AcmPolicyStatus = MmioRead64 (MMIO_ACM_POLICY_STATUS);

  return AcmPolicyStatus;
}

/**
  Dump raw data.

  @param[in]  Data  raw data
  @param[in]  Size  raw data size

**/
VOID
InternalDumpData (
  IN UINT8  *Data,
  IN UINTN  Size
  )
{
  UINTN  Index;
  for (Index = 0; Index < Size; Index++) {
    DEBUG ((DEBUG_INFO, "%02x", (UINTN)Data[Index]));
  }
}

/**
  Dump raw data with colume format.

  @param[in]  Data  raw data
  @param[in]  Size  raw data size

**/
VOID
InternalDumpHex (
  IN UINT8  *Data,
  IN UINTN  Size
  )
{
  UINTN   Index;
  UINTN   Count;
  UINTN   Left;

#define COLUME_SIZE  (16 * 2)

  Count = Size / COLUME_SIZE;
  Left  = Size % COLUME_SIZE;
  for (Index = 0; Index < Count; Index++) {
    DEBUG ((DEBUG_INFO, "%04x: ", Index * COLUME_SIZE));
    InternalDumpData (Data + Index * COLUME_SIZE, COLUME_SIZE);
    DEBUG ((DEBUG_INFO, "\n"));
  }

  if (Left != 0) {
    DEBUG ((DEBUG_INFO, "%04x: ", Index * COLUME_SIZE));
    InternalDumpData (Data + Index * COLUME_SIZE, Left);
    DEBUG ((DEBUG_INFO, "\n"));
  }
}

#ifndef DISABLE_SHA1_DEPRECATED_INTERFACES
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
  )
{
  UINTN       CtxSize;
  VOID        *HashCtx;

  CtxSize = Sha1GetContextSize ();
  HashCtx = AllocatePool (CtxSize);
  ASSERT (HashCtx != NULL);

  if (HashCtx != NULL){
    Sha1Init (HashCtx);
    Sha1Update (HashCtx, Data, Size);
    Sha1Final (HashCtx, Digest);
    FreePool (HashCtx);

    DEBUG((DEBUG_INFO, "Sha1 Hash  - \n"));
    InternalDumpHex((UINT8 *)(Digest), SHA1_DIGEST_SIZE);
  }

  return ;
}
#endif

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
  )
{
  UINTN       CtxSize;
  VOID        *HashCtx;

  CtxSize = Sha256GetContextSize ();
  HashCtx = AllocatePool (CtxSize);
  ASSERT (HashCtx != NULL);

  if (HashCtx != NULL){
    Sha256Init (HashCtx);
    Sha256Update (HashCtx, Data, Size);
    Sha256Final (HashCtx, Digest);
    FreePool (HashCtx);

    DEBUG((DEBUG_INFO, "Sha256 Hash  - \n"));
    InternalDumpHex((UINT8 *)(Digest), SHA256_DIGEST_SIZE);
  }
  return ;
}

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
  )
{
  UINTN       CtxSize;
  VOID        *HashCtx;

  CtxSize = Sha384GetContextSize ();
  HashCtx = AllocatePool (CtxSize);
  ASSERT (HashCtx != NULL);

  if (HashCtx != NULL){
    Sha384Init (HashCtx);
    Sha384Update (HashCtx, Data, Size);
    Sha384Final (HashCtx, Digest);
    FreePool (HashCtx);

    DEBUG((DEBUG_INFO, "Sha384 Hash  - \n"));
    InternalDumpHex((UINT8 *)(Digest), SHA384_DIGEST_SIZE);
  }
  return ;
}

/**
   Calculate SHA512 Hash

   @param[in]  Data   data
   @param[in]  Size   data size
   @param[out] Digest SHA512 digest
**/
VOID
CreateSha512Hash (
  IN  UINT8     *Data,
  IN  UINTN     Size,
  OUT UINT8     *Digest
  )
{
  UINTN       CtxSize;
  VOID        *HashCtx;

  CtxSize = Sha512GetContextSize ();
  HashCtx = AllocatePool (CtxSize);
  ASSERT (HashCtx != NULL);

  if (HashCtx != NULL){
    Sha512Init (HashCtx);
    Sha512Update (HashCtx, Data, Size);
    Sha512Final (HashCtx, Digest);
    FreePool (HashCtx);

    DEBUG((DEBUG_INFO, "Sha512 Hash  - \n"));
    InternalDumpHex((UINT8 *)(Digest), SHA512_DIGEST_SIZE);
  }
  return ;
}

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
  )
{
  UINTN       CtxSize;
  VOID        *HashCtx;

  CtxSize = Sm3GetContextSize ();
  HashCtx = AllocatePool (CtxSize);
  ASSERT (HashCtx != NULL);

  if (HashCtx != NULL){
    Sm3Init (HashCtx);
    Sm3Update (HashCtx, Data, Size);
    Sm3Final (HashCtx, Digest);
    FreePool (HashCtx);

    DEBUG((DEBUG_INFO, "Sm3 Hash  - \n"));
    InternalDumpHex((UINT8 *)(Digest), SM3_256_DIGEST_SIZE);
  }
  return ;
}
