/** @file
  This file contains functions for creating TPM Event Log

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

@par Specification
**/

#include <PiPei.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <IndustryStandard/Tpm20.h>
#include <IndustryStandard/UefiTcgPlatform.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/PrintLib.h>

#include <Library/Tpm2CommandLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/TpmMeasurementLib.h>

#include <Guid/TcgEventHob.h>
#include <Protocol/Tcg2Protocol.h>
#include <Ppi/FirmwareVolumeInfoMeasurementExcluded.h>

#include <Register/Cpuid.h>
#include <Register/GenerationMsr.h>
#include <CpuRegs.h>
#include <Txt.h>

#include <Library/BootGuardLib.h>
#include <Library/PeiBootGuardEventLogLib.h>
#include <Library/BaseBpmAccessLib.h>

#if FixedPcdGetBool(PcdBootGuardPerformanceEnable) == 1
#include <Library/PerformanceLib.h>
#include <Library/PrintLib.h>
#include <AcmDataHob.h>
#endif

STATIC TPMI_ALG_HASH SortedAlgIds[] = {
  TPM_ALG_SHA1,
  TPM_ALG_SHA256,
  TPM_ALG_SHA384,
  TPM_ALG_SHA512,
  TPM_ALG_SM3_256
};

STATIC EFI_TCG2_EVENT_INFO_STRUCT mTcg2EventInfo[] = {
  {&gTcgEventEntryHobGuid,  EFI_TCG2_EVENT_LOG_FORMAT_TCG_1_2},
  {&gTcgEvent2EntryHobGuid, EFI_TCG2_EVENT_LOG_FORMAT_TCG_2},
};

EFI_PEI_PPI_DESCRIPTOR  mMeasurementExcludeFvPlatformPpiList = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPeiFvMeasurementExcludedPlatformPpiGuid,
  NULL
};

#if FixedPcdGetBool(PcdBootGuardPerformanceEnable) == 1
/**
This function extracts BootGuard S-ACM performance data from MTRR MSR 0x210 and publishes into
FPDT table via Edk2 Performance library API's.
**/

VOID
RecordBootGuardPerfData (
  VOID
  )
{
  UINT32  Idx, PerfElemCnt;
  UINT32  Tag;
  EFI_HOB_GUID_TYPE  *GuidHob;
  ACM_DATA_HOB *AcmDataHob;
  RECORD *PerfRecords, *CurrentRecord;

  GuidHob = GetFirstGuidHob (&gAcmDataHobGuid);

  if (GuidHob != NULL) {
    DEBUG ((DEBUG_INFO, "Found ACM Performance data!! \n"));

    AcmDataHob = GET_GUID_HOB_DATA (GuidHob);

    PerfElemCnt = AcmDataHob->StartupACMBenchmark.Count;
    PerfRecords = AcmDataHob->StartupACMBenchmark.Record;

    if (PerfElemCnt <= BTG_PERF_MAX_RECORDS) {

      for (Idx = 0; Idx < PerfElemCnt; Idx++) {
        CurrentRecord = PerfRecords + Idx;

        if (CurrentRecord->Tag){
          Tag = CurrentRecord->Tag & ~BIT32;
          if (CurrentRecord->Tag%2 == 0){ //Use last bit of tag to determine Start vs End
                      PERF_START_EX (&gPerfAcmGuid, NULL, NULL, CurrentRecord->TimeStamp, Tag);
          }
          else {
            Tag = (UINT32) ((Tag & (UINT16) ~BIT0)) | 1;
            PERF_END_EX (&gPerfAcmGuid, NULL, NULL, CurrentRecord->TimeStamp, Tag);
          }
        }
      }
    }
    FreePool(PerfRecords);
  } else {
    DEBUG ((DEBUG_INFO, "ACM Performance data not found\n"));
    }

}
#endif

/**
  Check if buffer is all zero.

  @param[in] Buffer      Buffer to be checked.
  @param[in] BufferSize  Size of buffer to be checked.

  @retval TRUE  Buffer is all zero.
  @retval FALSE Buffer is not all zero.
**/
BOOLEAN
IsZeroBufferLocal (
  IN VOID  *Buffer,
  IN UINTN BufferSize
  )
{
  UINT8 *BufferData;
  UINTN Index;

  BufferData = Buffer;
  for (Index = 0; Index < BufferSize; Index++) {
    if (BufferData[Index] != 0) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
  Get digest from digest list.

  @param[in] HashAlg    digest algorithm
  @param[in] DigestList digest list
  @param[in] Digest     digest

  @retval EFI_SUCCESS   Sha1Digest is found and returned.
  @retval EFI_NOT_FOUND Sha1Digest is not found.
  @retval EFI_BUFFER_TOO_SMALL If DigestList's count exceeds the maximum supported digest count.
**/
EFI_STATUS
Tpm2GetDigestFromDigestList (
  IN TPMI_ALG_HASH      HashAlg,
  IN TPML_DIGEST_VALUES *DigestList,
  IN VOID               *Digest
  )
{
  UINT32  Index;
  UINT16  DigestSize;

  DigestSize = GetHashSizeFromAlgo (HashAlg);
  for (Index = 0; Index < DigestList->count; Index++) {
    //
    // Sanity check that index is within structure bounds before accessing.
    //
    if (Index >= HASH_COUNT) {
      return EFI_BUFFER_TOO_SMALL;
    }

    //
    // Copy digest on matching hashing algorithm
    //
    if (DigestList->digests[Index].hashAlg == HashAlg) {
      CopyMem (
        Digest,
        &DigestList->digests[Index].digest,
        DigestSize
        );
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Hash and log ACM extended event entry into the TCG Event Log.

  @param[in] DigestList    Pointer to a TPML_DIGEST_VALUES structure.
  @param[in] NewEventHdr   Pointer to a TCG_PCR_EVENT_HDR data structure.
  @param[in] NewEventData  Pointer to the new event data.

  @retval EFI_SUCCESS           The new event log entry was added.
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.
**/
EFI_STATUS
LogAcmPcrExtendedEvent (
  IN      TPML_DIGEST_VALUES        *DigestList,
  IN      TCG_PCR_EVENT_HDR         *NewEventHdr,
  IN      UINT8                     *NewEventData
  )
{
  VOID                              *HobData;
  EFI_STATUS                        Status;
  UINTN                             Index;
  EFI_STATUS                        RetStatus;
  TCG_PCR_EVENT2                    *TcgPcrEvent2;
  UINT8                             *DigestBuffer;
  UINT32                            SupportedEventLogs;

  SupportedEventLogs = EFI_TCG2_EVENT_LOG_FORMAT_TCG_1_2 | EFI_TCG2_EVENT_LOG_FORMAT_TCG_2;
  DEBUG ((DEBUG_INFO, "SupportedEventLogs - 0x%08x\n", SupportedEventLogs));

  RetStatus = EFI_SUCCESS;
  for (Index = 0; Index < sizeof (mTcg2EventInfo) / sizeof (mTcg2EventInfo[0]); Index++) {
    if ((SupportedEventLogs & mTcg2EventInfo[Index].LogFormat) != 0) {
      DEBUG((DEBUG_INFO, "  LogFormat - 0x%08x\n", mTcg2EventInfo[Index].LogFormat));
      switch (mTcg2EventInfo[Index].LogFormat) {
      case EFI_TCG2_EVENT_LOG_FORMAT_TCG_1_2:
        Status = Tpm2GetDigestFromDigestList (TPM_ALG_SHA1, DigestList, &NewEventHdr->Digest);
        if (!EFI_ERROR (Status)) {
          HobData = BuildGuidHob (
                      &gTcgEventEntryHobGuid,
                      sizeof (*NewEventHdr) + NewEventHdr->EventSize
                      );
          if (HobData == NULL) {
            RetStatus = EFI_OUT_OF_RESOURCES;
            break;
          }

          CopyMem (HobData, NewEventHdr, sizeof (*NewEventHdr));
          HobData = (VOID *) ((UINT8*)HobData + sizeof (*NewEventHdr));
          CopyMem (HobData, NewEventData, NewEventHdr->EventSize);
        }
        break;

      case EFI_TCG2_EVENT_LOG_FORMAT_TCG_2:
        HobData = BuildGuidHob (
                    &gTcgEvent2EntryHobGuid,
                    (sizeof (TcgPcrEvent2->PCRIndex) +
                     sizeof (TcgPcrEvent2->EventType) +
                     GetDigestListSize (DigestList) +
                     sizeof (TcgPcrEvent2->EventSize) + NewEventHdr->EventSize)
                    );

        if (HobData == NULL) {
          RetStatus = EFI_OUT_OF_RESOURCES;
          break;
        }

        TcgPcrEvent2 = HobData;
        TcgPcrEvent2->PCRIndex = NewEventHdr->PCRIndex;
        TcgPcrEvent2->EventType = NewEventHdr->EventType;
        DigestBuffer = (UINT8 *)&TcgPcrEvent2->Digest;
        DigestBuffer = CopyDigestListToBuffer (DigestBuffer, DigestList, PcdGet32 (PcdTpm2HashMask));
        CopyMem (DigestBuffer, &NewEventHdr->EventSize, sizeof (TcgPcrEvent2->EventSize));
        DigestBuffer = DigestBuffer + sizeof (TcgPcrEvent2->EventSize);
        CopyMem (DigestBuffer, NewEventData, NewEventHdr->EventSize);
        break;
      }
    }
  }

  return RetStatus;
}

/**
  Check FIT content to find ACM base.

  @retval BiosAcmBase - Null value if matching ACM was not found, else matching ACM address.

**/
VOID *
FindBiosAcmFromFit (
  )
{
  EFI_STATUS                             Status;
  EFI_PHYSICAL_ADDRESS                   BiosAcmBase;
  UINT64                                 FitPointer;
  UINT64                                 FitEnding;
  FIRMWARE_INTERFACE_TABLE_ENTRY         *FitTable;
  UINT32                                 EntryNum;
  CPUID_VERSION_INFO_EAX                 CpuSignature;
  UINT32                                 Index;
  FIT_TYPE2_VER_200_ENTRY                *MultiAcmEntry;

  DEBUG ((DEBUG_INFO, "%a - start\n", __FUNCTION__));
  Status = EFI_SUCCESS;
  FitPointer = *(UINT64 *) (UINTN) FIT_POINTER_ADDRESS;
  CpuSignature.Uint32 = 0;
  BiosAcmBase = 0x0;

  //
  // Check FIT header.
  //
  FitTable = (FIRMWARE_INTERFACE_TABLE_ENTRY *) (UINTN) FitPointer;
  if ((FitTable[0].Type != FIT_TYPE_00_HEADER) ||
      (FitTable[0].Address != FIT_TYPE_00_SIGNATURE)) {
    DEBUG ((DEBUG_ERROR, "Error: Invalid FIT header.\n"));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Check FIT ending address in valid range.
  //
  EntryNum = (((UINT32)(FitTable[0].Size[0])) + (((UINT32)(FitTable[0].Size[1])) << 8) + (((UINT32)(FitTable[0].Size[2])) << 16));
  FitEnding = FitPointer + sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY) * EntryNum;
  if (FitEnding  > (SIZE_4GB - 0x40)) {
    DEBUG ((DEBUG_ERROR, "Table exceeds valid range.\n"));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  AsmCpuid (CPUID_VERSION_INFO, &CpuSignature.Uint32, NULL, NULL, NULL);
  DEBUG ((DEBUG_INFO, "CpuSignature: 0x%x\n", CpuSignature.Uint32));
  DEBUG ((DEBUG_INFO, "  Model         : 0x%x\n", CpuSignature.Bits.Model));
  DEBUG ((DEBUG_INFO, "  Family        : 0x%x\n", CpuSignature.Bits.FamilyId));
  DEBUG ((DEBUG_INFO, "  ProcessorType : 0x%x\n", CpuSignature.Bits.ProcessorType));
  DEBUG ((DEBUG_INFO, "  ExtModel      : 0x%x\n", CpuSignature.Bits.ExtendedModelId));
  DEBUG ((DEBUG_INFO, "  ExtFamily     : 0x%x\n", CpuSignature.Bits.ExtendedFamilyId));

  //
  // Check Type 02 entry for ACM base address
  //
  for (Index = 0; Index < EntryNum; Index++) {

    if (FitTable[Index].Type != FIT_TYPE_02_STARTUP_ACM) {
      continue;
    }

    //
    // Type 02 entry found
    //
    if (FitTable[Index].Version == FIT_TYPE_VERSION) {
      //
      // Make this a default base address
      //
      BiosAcmBase = (EFI_PHYSICAL_ADDRESS)(UINTN) FitTable[Index].Address;
      DEBUG ((DEBUG_INFO, "Type2 0x100 entry (%d):\n", Index));
      DEBUG ((DEBUG_ERROR, "Found ACM base 0x16%llx in Type2 Ver 0x100 entry.\n", BiosAcmBase));
    } else if (FitTable[Index].Version == 0x200) {
      //
      // Check for multi ACM FIT entry (Type 2, verion 0x200)
      //
      MultiAcmEntry = (FIT_TYPE2_VER_200_ENTRY*) &FitTable[Index];
      DEBUG ((DEBUG_INFO, "Type2 0x200 entry (%d):\n", Index));
      DEBUG ((DEBUG_INFO, "  Address       : 0x%x\n", MultiAcmEntry->Address));

      DEBUG ((DEBUG_INFO, "  Model         : 0x%x\n", MultiAcmEntry->Model));
      DEBUG ((DEBUG_INFO, "  Family        : 0x%x\n", MultiAcmEntry->Family));
      DEBUG ((DEBUG_INFO, "  ProcessorType : 0x%x\n", MultiAcmEntry->ProcessorType));
      DEBUG ((DEBUG_INFO, "  ExtModel      : 0x%x\n", MultiAcmEntry->ExtModel));
      DEBUG ((DEBUG_INFO, "  ExtFamily     : 0x%x\n", MultiAcmEntry->ExtFamily));

      DEBUG ((DEBUG_INFO, "  ModelMask     : 0x%x\n", MultiAcmEntry->ModelMask));
      DEBUG ((DEBUG_INFO, "  FamilyMask    : 0x%x\n", MultiAcmEntry->FamilyMask));
      DEBUG ((DEBUG_INFO, "  TypeMask      : 0x%x\n", MultiAcmEntry->TypeMask));
      DEBUG ((DEBUG_INFO, "  ExtModelMask  : 0x%x\n", MultiAcmEntry->ExtModelMask));
      DEBUG ((DEBUG_INFO, "  ExtFamilyMask : 0x%x\n", MultiAcmEntry->ExtFamilyMask));

      if (((CpuSignature.Bits.Model            & MultiAcmEntry->ModelMask)     == MultiAcmEntry->Model) && \
          ((CpuSignature.Bits.FamilyId         & MultiAcmEntry->FamilyMask)    == MultiAcmEntry->Family) && \
          ((CpuSignature.Bits.ProcessorType    & MultiAcmEntry->TypeMask)      == MultiAcmEntry->ProcessorType) && \
          ((CpuSignature.Bits.ExtendedModelId  & MultiAcmEntry->ExtModelMask)  == MultiAcmEntry->ExtModel) && \
          ((CpuSignature.Bits.ExtendedFamilyId & MultiAcmEntry->ExtFamilyMask) == MultiAcmEntry->ExtFamily)) {
        BiosAcmBase = (EFI_PHYSICAL_ADDRESS)(UINTN) FitTable[Index].Address;
        DEBUG ((DEBUG_ERROR, "Found ACM base 0x%16llx in Type2 Ver 0x200 entry.\n", BiosAcmBase));
        break;
      }

    } else {
      DEBUG ((DEBUG_ERROR, "Unrecognized Type 2 version\n"));
      ASSERT (FALSE);
    }
  }

Exit:
  ///
  /// If BIOS ACM is not found - don't hang system. Assume that TXT
  /// must be disabled.
  ///
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to find BIOS ACM from FIT\n"));
    return NULL;
  }

  DEBUG ((DEBUG_INFO, "ACM Module Location: 0x%x\n", BiosAcmBase));

  // Found Startup ACM; check for 256KB alignment
  if ((((EFI_PHYSICAL_ADDRESS)(BiosAcmBase)) & (EFI_PHYSICAL_ADDRESS) 0x3FFFF) != 0) {
    DEBUG ((DEBUG_ERROR, "ACM not 256KB aligned!\n"));
    ASSERT (((((EFI_PHYSICAL_ADDRESS)(BiosAcmBase)) & (EFI_PHYSICAL_ADDRESS) 0x3FFFF) == 0) ? TRUE : FALSE);
  }

  return (VOID *) BiosAcmBase;
}

/**
   Compute hash of data for the selected PCR, strictly following
   the ascending order of the TCG algorithm IDs:
   SHA1 -> SHA256 -> SHA384 -> SHA512 -> SM3

   @param[in]  Data   data
   @param[in]  Size   data size
   @param[in]  SelectedPcrs Mask of selected PCRs
   @param[out] DigestList   Digest list to store the computed digest values.
**/
VOID
HashDataForSelectedPcrs (
  IN  UINT8              *Data,
  IN  UINTN              Size,
  IN  UINT32             SelectedPcrs,
  OUT TPML_DIGEST_VALUES *DigestList
  )
{
#if FixedPcdGetBool(PcdDeprecatedCryptoRemove) == 0
  UINT8  Sha1[SHA1_DIGEST_SIZE];
#endif
  UINT8  Sha256[SHA256_DIGEST_SIZE];
  UINT8  Sha384[SHA384_DIGEST_SIZE];
  UINT8  Sha512[SHA512_DIGEST_SIZE];
  UINT8  Sm3[SM3_256_DIGEST_SIZE];

  ZeroMem (DigestList, sizeof (TPML_DIGEST_VALUES));

  if (((SelectedPcrs & EFI_TCG2_BOOT_HASH_ALG_SHA1) != 0) &&
      (DigestList->count < HASH_COUNT)) {
#if FixedPcdGetBool(PcdDeprecatedCryptoRemove) == 0
    CreateSha1Hash ((UINT8*)Data, Size, (UINT8 *)Sha1);
    DigestList->digests[DigestList->count].hashAlg = TPM_ALG_SHA1;
    CopyMem (DigestList->digests[DigestList->count].digest.sha1, Sha1, SHA1_DIGEST_SIZE);
    DigestList->count ++;
#else
    DEBUG ((DEBUG_ERROR, "Unsupported hashing algorithm: SHA1\n"));
#endif
  }

  if (((SelectedPcrs & EFI_TCG2_BOOT_HASH_ALG_SHA256) != 0) &&
      (DigestList->count < HASH_COUNT)) {
    CreateSha256Hash ((UINT8*)Data, Size, (UINT8 *)Sha256);
    DigestList->digests[DigestList->count].hashAlg = TPM_ALG_SHA256;
    CopyMem (DigestList->digests[DigestList->count].digest.sha256, Sha256, SHA256_DIGEST_SIZE);
    DigestList->count ++;
  }

  if (((SelectedPcrs & EFI_TCG2_BOOT_HASH_ALG_SHA384) != 0) &&
      (DigestList->count < HASH_COUNT)) {
    CreateSha384Hash ((UINT8*)Data, Size, (UINT8 *)Sha384);
    DigestList->digests[DigestList->count].hashAlg = TPM_ALG_SHA384;
    CopyMem (DigestList->digests[DigestList->count].digest.sha384, Sha384, SHA384_DIGEST_SIZE);
    DigestList->count ++;
  }

  if (((SelectedPcrs & EFI_TCG2_BOOT_HASH_ALG_SHA512) != 0) &&
      (DigestList->count < HASH_COUNT)) {
    CreateSha512Hash ((UINT8*)Data, Size, (UINT8 *)Sha512);
    DigestList->digests[DigestList->count].hashAlg = TPM_ALG_SHA512;
    CopyMem (DigestList->digests[DigestList->count].digest.sha512, Sha512, SHA512_DIGEST_SIZE);
    DigestList->count ++;
  }

  if (((SelectedPcrs & EFI_TCG2_BOOT_HASH_ALG_SM3_256) != 0) &&
      (DigestList->count < HASH_COUNT)) {
    CreateSm3Hash ((UINT8*)Data, Size, (UINT8 *)Sm3);
    DigestList->digests[DigestList->count].hashAlg = TPM_ALG_SM3_256;
    CopyMem (DigestList->digests[DigestList->count].digest.sm3_256, Sm3, SM3_256_DIGEST_SIZE);
    DigestList->count ++;
  }
}

/**
   Print DigestList

   @param[in]  DigestList        Input Digest list to print
**/
VOID
PrintDigestList (
  IN  TPML_DIGEST_VALUES *DigestList
  )
{
  UINT32 InputIterator = 0;
  UINT32 DigestListSize = 0;
  UINT8 DigestSize = 0;
  TPMI_ALG_HASH CurrDigestAlg;

  if (DigestList == NULL) {
    return;
  }

  DigestListSize = DigestList->count;

  //
  // Check that DigestListSize never exceeds the maximum supported digest count
  //
  if (DigestListSize > HASH_COUNT) {
    DEBUG ((DEBUG_ERROR, "DigestListSize exceeds maximum digest count supported!\n"));
    ASSERT (DigestListSize <= HASH_COUNT);
    return;
  }

  for (InputIterator = 0; InputIterator < DigestListSize; InputIterator++) {

    CurrDigestAlg = DigestList->digests[InputIterator].hashAlg;
    DEBUG ((DEBUG_INFO, "Hash Alg  = 0x%04x\n", CurrDigestAlg));

    switch (CurrDigestAlg) {
    case TPM_ALG_SHA1:
      DigestSize = SHA1_DIGEST_SIZE;
      break;
    case TPM_ALG_SHA256:
      DigestSize = SHA256_DIGEST_SIZE;
      break;
    case TPM_ALG_SHA384:
      DigestSize = SHA384_DIGEST_SIZE;
      break;
    case TPM_ALG_SHA512:
      DigestSize = SHA512_DIGEST_SIZE;
      break;
    case TPM_ALG_SM3_256:
      DigestSize = SM3_256_DIGEST_SIZE;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "ERROR! Unsupported hashing algorithm (0x%04x)!\n", CurrDigestAlg));
      continue;
      break;
    }
    InternalDumpHex ((UINT8 *) &(DigestList->digests[InputIterator].digest), DigestSize);
  }
}

/**
   Sort the input DigestList into the output SortedDigestList

   To disambiguate any extension performed by ACM and allow BIOS
   to reconstruct the event, order of entries in TPML_DIGEST_VALUES
   structure must be strictly specified in the ascending order of the
   TCG algorithm IDs: SHA1 -> SHA256 -> SHA384 -> SHA512 -> SM3

   @param[in]  DigestList        Input Digest list to sort
   @param[out] SortedDigestList  Output sorted Digest list
**/
VOID
SortDigestList (
  IN  TPML_DIGEST_VALUES *DigestList,
  OUT TPML_DIGEST_VALUES *SortedDigestList
  )
{
  UINT8 SortedAlgIdsSize = sizeof (SortedAlgIds) / sizeof (TPMI_ALG_HASH);
  UINT8 AlgIterator = 0;
  UINT8 SortedDigestCurrentPosition = 0;
  UINT32 InputIterator = 0;
  UINT8 MatchCount = 0;
  UINT32 DigestListSize = 0;

  DEBUG ((DEBUG_INFO, "%a() entry...\n", __FUNCTION__));

  if ((DigestList == NULL) || (SortedDigestList == NULL)) {
    return;
  }

  ZeroMem (SortedDigestList, sizeof (TPML_DIGEST_VALUES));

  DigestListSize = SortedDigestList->count = DigestList->count;

  //
  // Check that DigestListSize never exceeds the maximum supported digest count
  //
  if (DigestListSize > HASH_COUNT) {
    DEBUG ((DEBUG_ERROR, "DigestListSize exceeds maximum digest count supported!\n"));
    ASSERT (DigestListSize <= HASH_COUNT);
    return;
  }

  //
  // Iterate over TCG algorithms in specified ascending order.
  //
  for (AlgIterator = 0; ((AlgIterator < SortedAlgIdsSize) && (MatchCount < DigestListSize)); AlgIterator++) {
    //
    // Iterate over input digest list searching for a matching algorithm entry.
    //
    for (InputIterator = 0; InputIterator < DigestListSize; InputIterator++) {

      if (DigestList->digests[InputIterator].hashAlg != SortedAlgIds[AlgIterator]) {
        continue;
      }

      SortedDigestList->digests[SortedDigestCurrentPosition].hashAlg = DigestList->digests[InputIterator].hashAlg;
      CopyMem (
        &(SortedDigestList->digests[SortedDigestCurrentPosition].digest),
        &(DigestList->digests[InputIterator].digest),
        sizeof (TPMU_HA));

      SortedDigestCurrentPosition++;
      MatchCount++;
      break;
    }
  }
}

/**
   BIOS SHALL measure KM, BPM, and ACM_POLICY_STATUS register from locality 0.

   @param[in] ActivePcrBanks Active PCR Banks
   @param[in] AcmPolicyStatus ACM Policy status data
   @param[in] Km Pointer to the Key Manifest
   @param[in] Bpm Pointer to the Boot Policy Manifest
   @param[in] IbbHashPtr Pointer to the IBB hash list

   @retval     EFI_SUCCESS     The function completes successfully
   @retval     EFI_INVALID_PARAMETER Input pointers are invalid
   @retval     EFI_UNSUPPORTED Unable to parse unsupported hashing algorithm or signing scheme
**/
EFI_STATUS
CreatePolicyDataMeasurementEvent (
  IN UINT32  ActivePcrBanks,
  IN UINT64  AcmPolicyStatus,
  IN KEY_MANIFEST_STRUCTURE           *Km,
  IN BOOT_POLICY_MANIFEST_HEADER      *Bpm,
  IN IBB_ELEMENT                      *BpmIbb
  )
{

  EFI_STATUS                       Status = EFI_SUCCESS;
  UINT8                            *CurrPos;
  UINT32                           BpmSigSize = 0;
  UINT32                           KmSigSize = 0;
  UINT16                           KeyModulusSize;
  UINT16                           KeyStructSize;
  UINT32                           AcmPolicyDataSize = 0;
  MAX_ACM_POLICY_DATA              MaxAcmPolicyData;
  UINT8                            *AcmPolicyDataPtr;
  STATIC CONST UINT16              PolicyDataMeasurementEventDataString[] = L"BIOS Measured Boot Guard Policy\0";

  DEBUG ((DEBUG_INFO, "%a() entry...\n", __FUNCTION__));

  ASSERT (Km != NULL);
  if (Km == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Bpm != NULL);
  if (Bpm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (BpmIbb != NULL);
  if (BpmIbb == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&MaxAcmPolicyData, sizeof (MAX_ACM_POLICY_DATA));
  AcmPolicyDataPtr = (UINT8*) &MaxAcmPolicyData;

  ((ACM_POLICY_DATA *) AcmPolicyDataPtr)->AcmPolicyStatus = AcmPolicyStatus;
  DEBUG ((DEBUG_INFO, "AcmPolicySts  - 0x%04lx\n", ((ACM_POLICY_DATA *) AcmPolicyDataPtr)->AcmPolicyStatus));

  //
  // Mask out mutable policies from ACM_POLICY_STS
  //
  if ((BpmIbb->Flags & IBB_FLAG_SRTM_AC ) != 0) {
    ((ACM_POLICY_DATA *) AcmPolicyDataPtr)->AcmPolicyStatus &= SRTM_AC_MASK;
    DEBUG ((DEBUG_WARN, "ACM Policy Data Measurement: SRTM_AC_MASK is applied for AcmPolicySts.\n"));
    DEBUG ((DEBUG_WARN, "AcmPolicySts (MASKED)  - 0x%04lx\n", ((ACM_POLICY_DATA *) AcmPolicyDataPtr)->AcmPolicyStatus));
  }

  // Advance data pointers
  AcmPolicyDataSize = sizeof (((ACM_POLICY_DATA *) AcmPolicyDataPtr)->AcmPolicyStatus);
  AcmPolicyDataPtr += AcmPolicyDataSize;

  CurrPos = ((UINT8*) Km + Km->KeySignatureOffset);
  DEBUG ((DEBUG_INFO, "KmSignature offset:0x%04x\n", Km->KeySignatureOffset));

  if (((KEY_AND_SIGNATURE_STRUCT*) CurrPos)->KeyAlg == TPM_ALG_RSA) {
    // Advance the pointer to version (1 byte) and Key_alg (2 byte)
    CurrPos = (UINT8 *) (CurrPos + sizeof (UINT8) + sizeof (UINT16));
    KeyModulusSize = (((KEY_STRUCT_HEADER *) (UINT8*) CurrPos)->KeySizeBits)/8; // Modulus size in bytes
    KeyStructSize = sizeof (UINT8) + sizeof (UINT16) + sizeof (UINT32) + KeyModulusSize;
    CurrPos = CurrPos + KeyStructSize;

    // Here we have reached up to SigScheme
    if((*(UINT16*) CurrPos == TPM_ALG_RSASSA) || (*(UINT16*)CurrPos == TPM_ALG_RSAPSS)) {
      CurrPos += sizeof (UINT16);
      KmSigSize = (((SIGNATURE_STRUCT_HEADER*)CurrPos)->SigSizeBits)/8;
      CurrPos += sizeof (UINT8) + sizeof (UINT16) + sizeof (UINT16); // Sig version (1 byte) + sigsize (2 bytes) + hash_alg (2 bytes)

      // We have reached here to the signature data
      DEBUG ((DEBUG_INFO, "KmSignatureSize: 0x%x\n", KmSigSize));
      DEBUG ((DEBUG_INFO, "KmSignature:\n"));
      InternalDumpHex (CurrPos, KmSigSize);

      if ((AcmPolicyDataSize + KmSigSize) > (sizeof(AcmPolicyStatus) + MAX_SUPPORTED_SIGNATURE_SIZE)) {
        DEBUG ((DEBUG_ERROR, "KM signature measurement: Error! Buffer Too Small.\n"));
        Status = EFI_BUFFER_TOO_SMALL;
        ASSERT_EFI_ERROR (Status);
        return Status;
      }

      // Copy Key manifest signature
      CopyMem (
        AcmPolicyDataPtr,
        CurrPos,
        KmSigSize);
    }

  } else {
    DEBUG ((DEBUG_ERROR, "Unsupported KM KeyAlg\n"));
    Status = EFI_UNSUPPORTED;
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  AcmPolicyDataSize += KmSigSize;
  AcmPolicyDataPtr += KmSigSize;

  CurrPos = ((UINT8*) Bpm + Bpm->KeySignatureOffset);
  DEBUG ((DEBUG_INFO, "BpmSignature offset:0x%04x\n", Bpm->KeySignatureOffset));

  if (((KEY_AND_SIGNATURE_STRUCT*) CurrPos)->KeyAlg == TPM_ALG_RSA) {

    // advance the pointer to version (1 byte) and Key_alg (2 byte)
    CurrPos = (UINT8 *) (CurrPos + sizeof (UINT8) + sizeof (UINT16));
    KeyModulusSize = (((KEY_STRUCT_HEADER *) (UINT8*) CurrPos)-> KeySizeBits)/8; //modulus size in bytes
    KeyStructSize = sizeof (UINT8) + sizeof (UINT16) + sizeof (UINT32) + KeyModulusSize;
    CurrPos = CurrPos + KeyStructSize;

    // Here we have reached up to SigScheme
    if ((*(UINT16 *) CurrPos == TPM_ALG_RSASSA) || (*(UINT16 *) CurrPos == TPM_ALG_RSAPSS)) {
      CurrPos += sizeof (UINT16);
      BpmSigSize = (((SIGNATURE_STRUCT_HEADER *) CurrPos)->SigSizeBits)/8;
      CurrPos += sizeof (UINT8) + sizeof (UINT16) + sizeof (UINT16) ; // Sig version (1 byte) + sigsize (2 bytes) + hash_alg (2 bytes)

      // We have reached here to the signature data
      DEBUG ((DEBUG_INFO, "BpmSignatureSize: 0x%x\n", BpmSigSize));
      DEBUG ((DEBUG_INFO, "BpmSignature:\n"));
      InternalDumpHex (CurrPos, BpmSigSize);

      if ((AcmPolicyDataSize + BpmSigSize) > sizeof (MAX_ACM_POLICY_DATA)) {
        DEBUG ((DEBUG_ERROR, "Bpm signature measurement: Error! Buffer Too Small.\n"));
        Status = EFI_BUFFER_TOO_SMALL;
        ASSERT_EFI_ERROR (Status);
        return Status;
      }

      CopyMem (
        AcmPolicyDataPtr,
        CurrPos,
        BpmSigSize);
    }

  } else {
    DEBUG ((DEBUG_ERROR, "Unsupported BPM KeyAlg\n"));
    Status = EFI_UNSUPPORTED;
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  AcmPolicyDataSize += BpmSigSize;

  //
  // Dump ACM Policy Data to be measured
  //
  DEBUG ((DEBUG_INFO, "ACM Policy Data blob:\n"));
  InternalDumpHex ((UINT8 *) &MaxAcmPolicyData, AcmPolicyDataSize);

  //
  // Measure to PCR[0] with event EV_POST_CODE
  //
  Status = TpmMeasureAndLogData (
             0,
             EV_POST_CODE,
             (UINT8 *) &PolicyDataMeasurementEventDataString,
             sizeof (PolicyDataMeasurementEventDataString),
             &MaxAcmPolicyData,
             AcmPolicyDataSize
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Measuring Post Code Event with Error - %r!\n", Status));
    return Status;
  }

  return Status;
}

/**
  Create IBB Measurement event log

  All digests must be retrieved from BPM and placed into TPML_DIGEST_VALUES using the ascending
  order of the TCG algorithm IDs: SHA1 -> SHA256 -> SHA384 -> SHA512 -> SM3

  @param[in] ActivePcrBanks Active PCR Banks
  @param[in] Bpm Pointer to the Boot Policy Manifest
  @param[in] IbbHashPtr Pointer to the IBB hash list

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_INVALID_PARAMETER Input pointers are invalid
**/
EFI_STATUS
CreateIbbMeasurementEvent (
  IN UINT32                       ActivePcrBanks,
  IN BOOT_POLICY_MANIFEST_HEADER  *Bpm,
  IN HASH_LIST                    *IbbHashPtr
  )
{
  EFI_STATUS                   Status = EFI_SUCCESS;
  UINT16                       Idx;
  UINT8                        *CurrPos;
  UINT16                       CurrHashAlg;
  UINT32                       CurrPcrBank;
  TPMI_ALG_HASH                CurrDigestAlg;
  TCG_PCR_EVENT_HDR            NewEventHdr;
  UINT16                       IbbDigestSize;
  UINT16                       IbbDigestCount;
  TPML_DIGEST_VALUES           DigestList;
  TPML_DIGEST_VALUES           SortedDigestList;
  STATIC CONST CHAR16          IbbMeasurementEventDataString[] = L"Boot Guard Measured IBB\0";

  CurrPcrBank = 0;
  CurrHashAlg =  0;
  ZeroMem (&DigestList, sizeof (TPML_DIGEST_VALUES));
  ZeroMem (&SortedDigestList, sizeof (TPML_DIGEST_VALUES));

  DEBUG ((DEBUG_INFO, "%a() entry...\n", __FUNCTION__));

  ASSERT (Bpm != NULL);
  if (Bpm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (IbbHashPtr != NULL);
  if (IbbHashPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CurrPos = (UINT8 *)IbbHashPtr;
  IbbDigestCount = ((HASH_LIST*)CurrPos)->Count;
  DEBUG ((DEBUG_INFO, "IbbDigestCount = 0x%04x\n", IbbDigestCount));

  //
  // Advance past size and count field
  //
  CurrPos += sizeof (UINT16) + sizeof (UINT16);

  DEBUG ((DEBUG_INFO, "ActivePcrBanks = 0x%x\n", ActivePcrBanks));

  for (Idx = 0; Idx < IbbDigestCount ; Idx++) {
    CurrDigestAlg = ((SHAX_HASH_STRUCTURE *)CurrPos)->HashAlg;
    IbbDigestSize = ((SHAX_HASH_STRUCTURE *)CurrPos)->Size;

    DEBUG ((DEBUG_INFO, "Idx       = 0x%04x\n", Idx));
    DEBUG ((DEBUG_INFO, "Hash Alg  = 0x%04x\n", CurrDigestAlg));
    DEBUG ((DEBUG_INFO, "Hash Size = 0x%04x\n", IbbDigestSize));

    //
    // Advance to digest
    //
    CurrPos += sizeof (UINT16) + sizeof (UINT16);

    switch (CurrDigestAlg) {
#if FixedPcdGetBool(PcdDeprecatedCryptoRemove) == 0
    case TPM_ALG_SHA1:
      CurrPcrBank = EFI_TCG2_BOOT_HASH_ALG_SHA1;
      CurrHashAlg = TPM_ALG_SHA1;
      break;
#endif
    case TPM_ALG_SHA256:
      CurrPcrBank = EFI_TCG2_BOOT_HASH_ALG_SHA256;
      CurrHashAlg = TPM_ALG_SHA256;
      break;
    case TPM_ALG_SHA384:
      CurrPcrBank = EFI_TCG2_BOOT_HASH_ALG_SHA384;
      CurrHashAlg = TPM_ALG_SHA384;
      break;
    case TPM_ALG_SHA512:
      CurrPcrBank = EFI_TCG2_BOOT_HASH_ALG_SHA512;
      CurrHashAlg = TPM_ALG_SHA512;
      break;
    case TPM_ALG_SM3_256:
      CurrPcrBank = EFI_TCG2_BOOT_HASH_ALG_SM3_256;
      CurrHashAlg = TPM_ALG_SM3_256;
      break;
    default:
      DEBUG ((DEBUG_WARN, "WARNING! Unsupported hashing algorithm (0x%04x) found in the BPM digests.\n", CurrDigestAlg));
      break;
    }

    if ((DigestList.count < HASH_COUNT) && (ActivePcrBanks & CurrPcrBank) != 0) {
      DEBUG ((DEBUG_INFO, "  Copying digest for matching active PCR bank: 0x%08x.\n", CurrPcrBank));
      DigestList.digests[DigestList.count].hashAlg = CurrHashAlg;

      CopyMem (
        (UINT8 *)&(DigestList.digests[DigestList.count].digest),
        CurrPos,
        IbbDigestSize);

      InternalDumpHex (CurrPos, IbbDigestSize);
      DigestList.count++;
    }
    CurrPos += IbbDigestSize;
  }

  //
  // Sort Digest list to ensure ascending order of the TCG algorithm IDs
  //
  SortDigestList (
    &DigestList,
    &SortedDigestList
    );

  //
  // Create log event
  //
  NewEventHdr.PCRIndex  = 0;
  NewEventHdr.EventType = EV_POST_CODE;
  NewEventHdr.EventSize = sizeof (IbbMeasurementEventDataString);

  Status = LogAcmPcrExtendedEvent (&SortedDigestList, &NewEventHdr, (UINT8 *) IbbMeasurementEventDataString);
  return Status;
}

/**
   Recreate SCRTM Version extend event according to the requirements for EV_S_CRTM_VERSION event per
   PC Client PFP specification.

   S-ACM will perform a PCR0 extend operation from locality 3 containing the the ACM version.
   To disambiguate this extend and allow BIOS to reconstruct it, the order of entries in
   TPML_DIGEST_VALUES structure must be strictly specified in the ascending order of the TCG algorithm IDs:
   TCG algorithm IDs: SHA1 -> SHA256 -> SHA384 -> SHA512 -> SM3

   ACM version number:  NULL terminated UTF16 bit string. ACM revision (offset 20, 4 bytes) concatenated
   with ACM_SVN (offset 28, 2 bytes) converted to the UTF16 format

   @param[in] ActivePcrBanks Active PCR Banks
   @param[in] AcmHdr Pointer to the ACM header

   @retval     EFI_SUCCESS  The function completes successfully
   @retval     EFI_INVALID_PARAMETER Input pointers are invalid
**/
EFI_STATUS
CreateScrtmVersionEvent (
  IN UINT32                       ActivePcrBanks,
  IN ACM_HEADER_4                 *AcmHdr
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  UINT8   ByteCounter;
  UINT8   UnicodeCounter;

  UINT8   AcmHeaderDataBuffer[6] = {0};
  CHAR16  ConvertBuffer[3] = {'\0'};

  CHAR16  ScrtmUtf16String[SCRTM_VERSION_UTF16_LENGTH] = {'\0'};
  CHAR16  *UnicodeCharIterator = ScrtmUtf16String;

  TPML_DIGEST_VALUES  DigestList;
  TCG_PCR_EVENT_HDR   NewEventHdr;

  DEBUG ((DEBUG_INFO, "%a() entry...\n", __FUNCTION__));

  ASSERT (AcmHdr != NULL);
  if (AcmHdr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read ACM revision (build date) - 4 bytes
  //
  CopyMem (
    &AcmHeaderDataBuffer[0],
    &((ACM_HEADER_4 *) AcmHdr)->Revision,
    4
    );

  //
  // Read ACM SVN - 2 bytes
  //
  CopyMem (
    &AcmHeaderDataBuffer[4],
    &((ACM_HEADER_4 *) AcmHdr)->AcmSvn,
    2
    );

  InternalDumpHex((UINT8 *)(AcmHeaderDataBuffer), 6);

  for (ByteCounter = 0; ByteCounter < 6; ByteCounter++) {
    //
    // Fill convertion buffer with hex strings (with leading zeroes) per byte input.
    //
    UnicodeSPrint (
      ConvertBuffer,
      sizeof (CHAR16) * 3,
      L"%02X",
      AcmHeaderDataBuffer[ByteCounter]
      );

    if (UnicodeCharIterator >= (ScrtmUtf16String + SCRTM_VERSION_UTF16_LENGTH)) {
      DEBUG ((DEBUG_ERROR, "ERROR parsing ACM version string! %s\n", ScrtmUtf16String));
      ASSERT (UnicodeCharIterator < (ScrtmUtf16String + SCRTM_VERSION_UTF16_LENGTH));
      break;
    }
    //
    //  Keep 2 UTF-16 characters and trim NULL character from resulting hex strings
    //
    for (UnicodeCounter = 0; UnicodeCounter < 2; UnicodeCounter++) {
      *UnicodeCharIterator = (CHAR16) ConvertBuffer[UnicodeCounter];
      UnicodeCharIterator++;
    }
  }
  DEBUG ((DEBUG_INFO, "SCRTM version UTF-16 string: %s\n", ScrtmUtf16String));

  //
  // Hash digest value
  //
  ZeroMem (&DigestList, sizeof (TPML_DIGEST_VALUES));

  HashDataForSelectedPcrs (
    (UINT8 *) &ScrtmUtf16String,
    sizeof (UINT16) * SCRTM_VERSION_UTF16_LENGTH,
    ActivePcrBanks,
    &DigestList);

  //
  // Create log event
  //
  NewEventHdr.PCRIndex  = 0;
  NewEventHdr.EventType = EV_S_CRTM_VERSION;
  NewEventHdr.EventSize = sizeof (UINT16) * SCRTM_VERSION_UTF16_LENGTH;
  Status = LogAcmPcrExtendedEvent (&DigestList, &NewEventHdr, (UINT8 *) ScrtmUtf16String);
  return Status;
}

/**
   Create Locality Startup event entry

   The Startup Locality event should be placed in the log before any event which extends
   PCR[0]. This allows software which needs to parse the TCG Event Log to initialize its
   internal PCR[0] state correctly.

   @param[in] StartupLocality TPM Startup Locality
   @param[in] ActivePcrBanks Active PCR Banks

   @retval     EFI_SUCCESS  The function completes successfully
   @retval     EFI_INVALID_PARAMETER Input pointers are invalid
**/
EFI_STATUS
CreateLocalityStartupEvent (
  IN UINT8              StartupLocality,
  IN UINT32             ActivePcrBanks
  )
{

  EFI_STATUS                       Status = EFI_SUCCESS;
  TCG_PCR_EVENT_HDR                NewEventHdr;
  TPML_DIGEST_VALUES               DigestList;
  TCG_EFI_STARTUP_LOCALITY_EVENT   LocalityEventData;
  static CONST CHAR8               LocalityString[] = "StartupLocality\0";

  DEBUG ((DEBUG_INFO, "%a() entry...\n", __FUNCTION__));

  ZeroMem (&DigestList, sizeof (TPML_DIGEST_VALUES));

  if (((ActivePcrBanks & EFI_TCG2_BOOT_HASH_ALG_SHA1) != 0) &&
      (DigestList.count < HASH_COUNT)) {
    DigestList.digests[DigestList.count].hashAlg = TPM_ALG_SHA1;
    DigestList.count ++;
  }

  if (((ActivePcrBanks & EFI_TCG2_BOOT_HASH_ALG_SHA256) != 0) &&
      (DigestList.count < HASH_COUNT)) {
    DigestList.digests[DigestList.count].hashAlg = TPM_ALG_SHA256;
    DigestList.count ++;
  }

  if (((ActivePcrBanks & EFI_TCG2_BOOT_HASH_ALG_SHA384) != 0) &&
      (DigestList.count < HASH_COUNT)) {
    DigestList.digests[DigestList.count].hashAlg = TPM_ALG_SHA384;
    DigestList.count ++;
  }

  if (((ActivePcrBanks & EFI_TCG2_BOOT_HASH_ALG_SHA512) != 0) &&
      (DigestList.count < HASH_COUNT)) {
    DigestList.digests[DigestList.count].hashAlg = TPM_ALG_SHA512;
    DigestList.count ++;
  }

  if (((ActivePcrBanks & EFI_TCG2_BOOT_HASH_ALG_SM3_256) != 0) &&
      (DigestList.count < HASH_COUNT)) {
    DigestList.digests[DigestList.count].hashAlg = TPM_ALG_SM3_256;
    DigestList.count ++;
  }

  NewEventHdr.PCRIndex  = 0;
  NewEventHdr.EventType = EV_NO_ACTION;

  NewEventHdr.EventSize = sizeof (TCG_EFI_STARTUP_LOCALITY_EVENT);

  CopyMem (LocalityEventData.Signature, LocalityString, AsciiStrSize (LocalityString));
  LocalityEventData.StartupLocality = StartupLocality;

  Status = LogAcmPcrExtendedEvent (&DigestList, &NewEventHdr,(UINT8 *) &LocalityEventData);
  return Status;
}

/**
  If Boot Guard already measured IBB, we do not need let TPM measure it again.
  So we let BiosInfo PEIM register the Measurement Exclude PPI
  and TPM driver knows this Fv should be excluded.
**/
VOID
ExcludeIbbFv (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "Excluding IBB FVs as Boot Guard did Measurement of PEI FV\n"));
  PeiServicesInstallPpi (&mMeasurementExcludeFvPlatformPpiList);
}

/**
  This is the library's callback entry point that will handle the TCG event logging
  depending on the Boot Guard configuration.

  It will call the functions required to recreate the ACM event measurements into
  the TCG log and for BIOS to extend the ACM policy data measurement to PCR[0].

  If the ACM policy data can't be retrieved for measurement, this function will
  attempt to measure an EV_SEPARATOR event to PCR[0].

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED Bootguard not supported
  @retval     EFI_NOT_FOUND If any of the required structures could not be located.
  @retval     EFI_DEVICE_ERROR TPM not present or in error state.
  @retval     Others
**/
EFI_STATUS
EFIAPI
CreateBootguardEventLogEntriesCallback (
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR     *NotifyDesc,
  IN  VOID                          *InvokePpi
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  BOOT_GUARD_INFO         BootGuardInfo;
  ACM_BIOS_POLICY         AcmPolicySts;
  UINT32                  TpmActivePcrBanks      = 0;
  UINT32                  TpmHashAlgorithmBitmap = 0;
  UINT8                   TpmStartupLocality     = 0;

  ACM_HEADER_4                 *AcmHdr     = NULL;
  KEY_MANIFEST_STRUCTURE       *Km         = NULL;
  BOOT_POLICY_MANIFEST_HEADER  *Bpm        = NULL;
  IBB_ELEMENT                  *BpmIbb     = NULL;
  HASH_LIST                    *IbbHashPtr = NULL;

  DEBUG ((DEBUG_INFO, "%a() entry...\n", __FUNCTION__));

  ZeroMem (&BootGuardInfo, sizeof (BOOT_GUARD_INFO));
  GetBootGuardInfo (&BootGuardInfo);

  if (BootGuardInfo.BootGuardCapability == FALSE) {
    return EFI_UNSUPPORTED;
  }

  if (BootGuardInfo.ByPassTpmEventLog == TRUE) {
    return EFI_SUCCESS;
  }

  AcmPolicySts.Data = MmioRead64 (MMIO_ACM_POLICY_STATUS);

  //
  // If BootGuard ACM is the S-CRTM, check for the TPM Startup locality used.
  //   b'001--> BTG / b'010 (2) --> TXT / b'100 (4) --> PFR
  //

  if (AcmPolicySts.Bits.SCrtmStatus == 0) {
    return EFI_SUCCESS;
  }

  //
  // Initialize TPM Startup locality to 0
  //
  TpmStartupLocality = LOCALITY_0_INDICATOR;
  //
  // If BootGuard ACM is the S-CRTM,
  // Create event logs from previous PCR extensions on behalf of ACM
  //

  //
  // Update TPM startup locality based on the ACM Policy Status TpmStartupLocality field:
  //   0x0 : Startup Locality = 3
  //   0x1 : Startup Locality = 0
  //
  if (AcmPolicySts.Bits.TpmStartupLocality == 0) {
    TpmStartupLocality = LOCALITY_3_INDICATOR;
  }

  //
  // Create TPM event log if not Sx Resume Type - S3/Deep-S3/FFS Resume
  // Note: S4->S0 is similar to S5->S0
  //
  DEBUG ((DEBUG_INFO, "BIOS is Creating Event Log for ACM measurements\n"));

  AcmHdr = FindBiosAcmFromFit ();
  if (AcmHdr == NULL) {
    DEBUG ((DEBUG_ERROR, "ACM header not found!\n"));
    ASSERT (AcmHdr != NULL);
    return EFI_NOT_FOUND;
  }

  Km = FindKm ();
  if (Km == NULL) {
    DEBUG ((DEBUG_ERROR, "Key Manifest not found!\n"));
    ASSERT (Km != NULL);
    return EFI_NOT_FOUND;
  }

  Bpm = FindBpm ();
  if (Bpm == NULL) {
    DEBUG ((DEBUG_ERROR, "Boot Policy Manifest not found!\n"));
    ASSERT (Bpm != NULL);
    return EFI_NOT_FOUND;
  }

  BpmIbb = FindBpmIbb (Bpm);
  if (BpmIbb == NULL) {
    DEBUG ((DEBUG_ERROR, "IBB Element not found!\n"));
    ASSERT (BpmIbb != NULL);
    return EFI_NOT_FOUND;
  }

  IbbHashPtr = (HASH_LIST*) FindBpmElement (Bpm, BOOT_POLICY_MANIFEST_IBB_ELEMENT_DIGEST_ID);
  if (IbbHashPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "IBB Hash not found!\n"));
    ASSERT (IbbHashPtr != NULL);
    return EFI_NOT_FOUND;
  }

  //
  // Request locality 0 on TPM device before issuing TPM commands.
  //
  Status = Tpm2RequestUseTpm ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tpm2RequestUseTpm() error status: %r\n", Status));
    return Status;
  }

  Status = Tpm2GetCapabilitySupportedAndActivePcrs (
             &TpmHashAlgorithmBitmap,
             &TpmActivePcrBanks);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tpm2GetCapabilitySupportedAndActivePcrs() error status: %r\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "TpmStartupLocality = 0x%x\n", TpmStartupLocality));
  Status = CreateLocalityStartupEvent (TpmStartupLocality, TpmActivePcrBanks);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateLocalityStartupEvent() error status: %r\n", Status));
    return Status;
  }

  Status = CreateScrtmVersionEvent (TpmActivePcrBanks, AcmHdr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateScrtmVersionEvent() error status: %r\n", Status));
    return Status;
  }
  //
  // Disable BIOS TCG drivers measurement of CRTM
  // and log ACM's extention of the S-CRTM version
  //
  PcdSet8S (PcdTpm2ScrtmPolicy, 0);

  //
  // Log ACM's extention of the IBB
  //
  Status = CreateIbbMeasurementEvent (TpmActivePcrBanks, Bpm, IbbHashPtr);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateIbbMeasurementEvent() error status: %r\n", Status));
    return Status;
  }
  //
  // If Boot Guard already measured IBB, we do not need let TPM measure it again.
  //
  if (IsMeasuredBoot ()) {
    ExcludeIbbFv ();
  }

  //
  // BIOS extends Policy data measurement from Locality 0
  //
  Status = CreatePolicyDataMeasurementEvent (
             TpmActivePcrBanks,
             AcmPolicySts.Data,
             Km,
             Bpm,
             BpmIbb);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreatePolicyDataMeasurementEvent() error status: %r\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Successfully completed %a()\n", __FUNCTION__));
  return Status;
}

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEdkiiTcgPpiGuid,
  CreateBootguardEventLogEntriesCallback
};

/**
   The constructor registers a callback to recreate the ACM event measurements
   into the TCG log and for BIOS to extend the ACM policy data measurement once
   the gEdkiiTcgPpiGuid has been produced by the TCG PEIM module.

  @param[in]  FileHandle   The handle of FFS header the loaded driver.
  @param[in]  PeiServices  The pointer to the PEI services.

  @retval EFI_SUCCESS      The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
PeiBootGuardEventLogConstructor (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  DEBUG ((DEBUG_INFO, "%a()\n", __FUNCTION__));
  PeiServicesNotifyPpi (&mNotifyList);

#if FixedPcdGetBool(PcdBootGuardPerformanceEnable) == 1
  RecordBootGuardPerfData();
#endif

  return EFI_SUCCESS;
}
