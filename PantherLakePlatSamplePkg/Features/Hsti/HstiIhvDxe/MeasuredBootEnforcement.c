/** @file
  This file contains the tests for the MeasuredBootEnforcement BIT

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

#include "HstiIhvDxe.h"
#include <Register/FlashRegs.h>
#include <Library/PcdLib.h>
#include <IndustryStandard/TpmPtp.h>
#include <Txt.h>

#define PCR_7_SELECTION_MASK 0x80
#define PCR_0_SELECTION_MASK 0x01

TPM2B_AUTH             mNewPlatformAuth;

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
  Return the Hash Algorithm Authorization size

  @param[in] PcrHash  PCR Hash Digest value

  @return HashSize    Return Hash Digest Authorization Size found in the Active PCR
  @return SHA512      Return default Hash Authorization size
**/
UINT16
GetPcrHashSize (
  IN UINT16 Hash
  )
{
  UINT16 HashSize;
  HashSize = GetHashSizeFromAlgo (Hash);

  switch (Hash) {
    case TPM_ALG_SHA1:
      DEBUG ((DEBUG_INFO, "       Algorithm size = SHA1\n"));
      break;
    case TPM_ALG_SHA256:
      DEBUG ((DEBUG_INFO, "       Algorithm size = SHA256\n"));
      break;
    case TPM_ALG_SHA384:
      DEBUG ((DEBUG_INFO, "       Algorithm size = SHA384\n"));
      break;
    case TPM_ALG_SHA512:
      DEBUG ((DEBUG_INFO, "       Algorithm size = SHA512\n"));
      break;
    case TPM_ALG_SM3_256:
      DEBUG ((DEBUG_INFO, "       Algorithm size = SM3_256\n"));
      break;
    default:
      HashSize = MAX_NEW_AUTHORIZATION_SIZE;
      DEBUG ((DEBUG_INFO, "       Algorithm not supported by BIOS for this Pcr\n"));
      DEBUG ((DEBUG_INFO, "       Hash Algorithm = 0, setting default to SHA512_DIGEST_SIZE\n"));
  }

  return HashSize;
}

/**
   TPM 2.0 support test case

   @retval TRUE     Test case was execution steps completed and output result is valid.
   @retval FALSE    Error occurred during test case execution.
**/
BOOLEAN
CheckTpmChipsetSupport (
  VOID
  )
{
  UINTN   TpmBaseAddress;
  UINT32  TpmVerFtifFtLocMask;
  UINT8   ValueAtTpmBaseAddress;
  PTP_CRB_INTERFACE_IDENTIFIER   TpmInterfaceId;
  PTP_FIFO_INTERFACE_CAPABILITY  TpmIntfCapability;

  TpmBaseAddress = (UINTN)PcdGet64 (PcdTpmBaseAddress);
  ValueAtTpmBaseAddress = *(UINT8 *) TpmBaseAddress;

  //
  // Check if TPM is in error state
  //
  if (GetFirstGuidHob (&gTpmErrorHobGuid) != NULL) {
    DEBUG ((DEBUG_INFO, "        Tpm is in error state"));
    return FALSE;
  }

  TpmVerFtifFtLocMask =  MmioRead32 (TXT_PUBLIC_BASE + 0x800) & TPM_VER_FTIF_FT_LOC_MASK;
  DEBUG ((DEBUG_INFO, "        TPM Location configured (expected values: dTPM = 0x5 or fTPM = 0x7 or PSE = 0x4) = 0x%x\n", (TpmVerFtifFtLocMask >> 16)));
  DEBUG ((DEBUG_INFO, "        Value at TPM Base Address (0x%x) = 0x%x\n", TpmBaseAddress, ValueAtTpmBaseAddress));

  if ((((TpmVerFtifFtLocMask & DTPM_LOC) != DTPM_LOC) &&
      ((TpmVerFtifFtLocMask & FTPM_LOC)  != FTPM_LOC) &&
      ((TpmVerFtifFtLocMask & PSE_LOC)   != PSE_LOC)) ||
      (ValueAtTpmBaseAddress == 0xFF)) {
    return FALSE;
  }

  TpmInterfaceId.Uint32    = MmioRead32 ((UINTN)&((PTP_CRB_REGISTERS *)TpmBaseAddress)->InterfaceId);
  TpmIntfCapability.Uint32 = MmioRead32 ((UINTN)&((PTP_FIFO_REGISTERS *)TpmBaseAddress)->InterfaceCapability);
  DEBUG ((DEBUG_INFO, "         TPM Interface Identifier (0x%x) = 0x%x\n", &((PTP_CRB_REGISTERS *)TpmBaseAddress)->InterfaceId, TpmInterfaceId));
  DEBUG ((DEBUG_INFO, "         TPM Interface Capability (0x%x) = 0x%x\n", &((PTP_FIFO_REGISTERS *)TpmBaseAddress)->InterfaceCapability, TpmIntfCapability));

  if ((TpmInterfaceId.Bits.InterfaceType != PTP_INTERFACE_IDENTIFIER_INTERFACE_TYPE_FIFO) &&
      (TpmInterfaceId.Bits.InterfaceType != PTP_INTERFACE_IDENTIFIER_INTERFACE_TYPE_CRB) &&
      (TpmInterfaceId.Bits.InterfaceType != PTP_INTERFACE_IDENTIFIER_INTERFACE_TYPE_TIS)) {
    DEBUG ((DEBUG_INFO, "        TPM Interface Identifier Interface Type invalid\n"));
    return FALSE;
  }


  if ((TpmInterfaceId.Bits.InterfaceType == PTP_INTERFACE_IDENTIFIER_INTERFACE_TYPE_FIFO) &&
      (TpmInterfaceId.Bits.InterfaceVersion == PTP_INTERFACE_IDENTIFIER_INTERFACE_VERSION_FIFO) &&
      (TpmInterfaceId.Bits.CapFIFO != 0) &&
      (TpmIntfCapability.Bits.InterfaceVersion == INTERFACE_CAPABILITY_INTERFACE_VERSION_PTP)) {

    DEBUG ((DEBUG_INFO, "        Supported TPM Interface is FIFO\n"));
  } else if ((TpmInterfaceId.Bits.InterfaceType == PTP_INTERFACE_IDENTIFIER_INTERFACE_TYPE_CRB) &&
      (TpmInterfaceId.Bits.InterfaceVersion == PTP_INTERFACE_IDENTIFIER_INTERFACE_VERSION_CRB) &&
      (TpmInterfaceId.Bits.CapCRB != 0)) {

    DEBUG ((DEBUG_INFO, "        Supported TPM Interface is CRB\n"));
  }  else if (TpmInterfaceId.Bits.InterfaceType == PTP_INTERFACE_IDENTIFIER_INTERFACE_TYPE_TIS) {
    DEBUG ((DEBUG_INFO, "        Supported TPM Interface is FIFO as defined in TIS1.3\n"));
  } else {
    DEBUG ((DEBUG_INFO, "        TPM Interface Id invalid\n"));
    return FALSE;
  }
  return TRUE;
}
/**
   TPM 2.0 PCR Initialization test case

   This function will read the set of supported and active PCRs and return whether
   these PCRs have been initialized.

   @param[in]  PcrNumberMask       Index mask of the PCR to be tested
   @param[out] PcrInitialized  Pointer to the boolean variable to store the test result

   @retval EFI_SUCCESS     Test case was execution steps completed and output result is valid.
   @retval other           Some error occurs during test case execution.
**/
EFI_STATUS
CheckPcrInitialization (
  BYTE                               PcrNumberMask,
  BOOLEAN                           *PcrInitialized
  )
{
  EFI_STATUS                        Status;
  TPML_PCR_SELECTION                Pcrs;
  TPML_PCR_SELECTION                PcrSelectionIn;
  UINT32                            PcrUpdateCounter;
  TPML_PCR_SELECTION                PcrSelectionOut;
  TPML_DIGEST                       PcrValues;

  EFI_TCG2_EVENT_ALGORITHM_BITMAP   TpmHashAlgorithmBitmap;
  TPMI_ALG_HASH                     CurrentPcrBankHash;

  UINT32                            Index;
  UINT32                            ActivePcrBanks;
  UINT32                            HashAlg;
  BOOLEAN                           CurrentPcrBankInitialized;

  *PcrInitialized           = TRUE;
  CurrentPcrBankInitialized = TRUE;
  PcrUpdateCounter          = 0;

  ZeroMem (&PcrSelectionIn, sizeof (PcrSelectionIn));
  ZeroMem (&PcrSelectionOut, sizeof (PcrSelectionOut));
  ZeroMem (&PcrValues, sizeof (PcrValues));
  ZeroMem (&Pcrs, sizeof (TPML_PCR_SELECTION));

  //
  // Read TPM capabilities
  //
  Status = Tpm2GetCapabilityPcrs (&Pcrs);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to read TPM capabilities\n"));
    return Status;
  }

  DEBUG ((DEBUG_ERROR, "TPM supported PCR count = 0x%x\n", Pcrs.count));

  //
  // Get Active Pcrs
  //
  Status = Tpm2GetCapabilitySupportedAndActivePcrs (&TpmHashAlgorithmBitmap, &ActivePcrBanks);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to read TPM capabilities and active PCRs\n"));
    return Status;
  }

  //
  // Select from Active PCRs
  //
  for (Index = 0; Index < Pcrs.count; Index++) {
    CurrentPcrBankHash = Pcrs.pcrSelections[Index].hash;
    HashAlg = GetHashMaskFromAlgo (CurrentPcrBankHash);

    //
    // Skip inactive PCRs
    //
    if ((HashAlg & ActivePcrBanks) == 0) {
      continue;
    }

    //
    // Select PCR 0-7 from current active bank
    //
    PcrSelectionIn.pcrSelections[PcrSelectionIn.count].hash = Pcrs.pcrSelections[Index].hash;
    PcrSelectionIn.pcrSelections[PcrSelectionIn.count].sizeofSelect = PCR_SELECT_MAX;
    PcrSelectionIn.pcrSelections[PcrSelectionIn.count].pcrSelect[0] = PcrNumberMask;
    PcrSelectionIn.pcrSelections[PcrSelectionIn.count].pcrSelect[1] = 0x00;
    PcrSelectionIn.pcrSelections[PcrSelectionIn.count].pcrSelect[2] = 0x00;
    PcrSelectionIn.count++;
  }

  //
  // Read PCRs
  //
  Status = Tpm2PcrRead (&PcrSelectionIn, &PcrUpdateCounter, &PcrSelectionOut, &PcrValues);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to read PCRs\n"));
    return Status;
  }

  //
  // Parse through read PCRs
  //
  for (Index = 0; Index < PcrSelectionOut.count; Index++) {
    CurrentPcrBankHash = PcrSelectionOut.pcrSelections[Index].hash;
    HashAlg = GetHashMaskFromAlgo (CurrentPcrBankHash);

    //
    // Skip inactive PCRs
    //
    if ((HashAlg & ActivePcrBanks) == 0) {
      continue;
    }

    CurrentPcrBankInitialized = !IsZeroBufferLocal (PcrValues.digests[Index].buffer, PcrValues.digests[Index].size);

    if (CurrentPcrBankInitialized == FALSE) {
      DEBUG ((DEBUG_INFO, "        PcrRead from PcrSelectionOut.pcrSelections[%d].hash = 0x%x\n is not initialized!", Index, CurrentPcrBankHash));
      *PcrInitialized = FALSE;
    }
  }

  return EFI_SUCCESS;
}

/**
   TPM 2.0 Hierarchy Change Auth *negative* test case

   This function will attempt to change the Platform Authentication using NULL auth.

   @param[out] HierarchyChangeSuccess Pointer to the boolean variable to store the test result

   @retval EFI_SUCCESS            Test case was execution steps completed and output result is valid.
   @retval EFI_INVALID_PARAMETER  Authentication size was 0 (BIOS does not supported the Hash).
   @retval other                  Some error occurs during test case execution.
**/
EFI_STATUS
CheckTpm2HierarchyChangeAuth (
  BOOLEAN *HierarchyChangeSuccess
  )
{
  EFI_STATUS                        Status;
  UINT32                            Index;
  TPML_PCR_SELECTION                Pcrs;
  TPMS_AUTH_COMMAND                 LocalAuthSession;
  UINT16                            AuthSize;

  AuthSize = MAX_NEW_AUTHORIZATION_SIZE;
  ZeroMem (&Pcrs, sizeof (TPML_PCR_SELECTION));
  Status = Tpm2GetCapabilityPcrs (&Pcrs);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to read TPM capabilities\n"));
    return Status;
  }

  for (Index = 0; Index < Pcrs.count; Index++) {
    AuthSize = GetPcrHashSize (Pcrs.pcrSelections[Index].hash);
  }

  if (AuthSize != 0) {
    ZeroMem (mNewPlatformAuth.buffer, AuthSize);
    ZeroMem (&LocalAuthSession, sizeof (LocalAuthSession));
    LocalAuthSession.sessionHandle = TPM_RS_PW;
    LocalAuthSession.hmac.size = AuthSize;
    CopyMem (LocalAuthSession.hmac.buffer, mNewPlatformAuth.buffer, AuthSize);

    //
    // Negative Test (Expected Status: Device Error)
    //
    Status = Tpm2HierarchyChangeAuth (TPM_RH_PLATFORM, NULL, &mNewPlatformAuth);
    DEBUG ((DEBUG_INFO, "        Tpm2HierarchyChangeAuth with NULL auth Result: - %r\n", Status));

    if (EFI_ERROR (Status)) {
      *HierarchyChangeSuccess = FALSE;
    } else {
      *HierarchyChangeSuccess = TRUE;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "Unable to run Change Auth *negative* test case"));
    *HierarchyChangeSuccess = FALSE;
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/**
   TPM 2.0 Event Log test case

   This function will attempt to retrieve the TPM event log from the Tcg2Protocol and return its location

   @param[out] EventLogLocation Pointer to the event log

   @retval EFI_SUCCESS     Test case was execution steps completed and output result is valid.
   @retval other           Some error occurs during test case execution.
**/
EFI_STATUS
CheckTpmEventLog (
  EFI_PHYSICAL_ADDRESS *EventLogLocation
  )
{

  EFI_STATUS                        Status;
  EFI_TCG2_PROTOCOL                 *Tcg2Protocol = NULL;
  EFI_PHYSICAL_ADDRESS              EventLogLastEntry;
  BOOLEAN                           EventLogTruncated;
  UINT32                            ActivePcrBanks;

  Status = gBS->LocateProtocol (
                  &gEfiTcg2ProtocolGuid,
                  NULL,
                  (VOID **)&Tcg2Protocol
                  );

  if (Tcg2Protocol == NULL) {
    DEBUG ((DEBUG_ERROR, "TCG2 Protocol is NULL!\n"));
    return EFI_OUT_OF_RESOURCES;;
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to locate TCG2 Protocol!\n"));
    return Status;
  }

  Status = Tcg2Protocol->GetActivePcrBanks (Tcg2Protocol, &ActivePcrBanks);

  //
  // Report SHA's follows TCG 2 event log format
  //
  Status = Tcg2Protocol->GetEventLog (
                           Tcg2Protocol,
                           EFI_TCG2_EVENT_LOG_FORMAT_TCG_2,
                           EventLogLocation,
                           &EventLogLastEntry,
                           &EventLogTruncated
                           );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to retrieve TCG2 event log!\n"));
      return Status;
    }

  DEBUG ((DEBUG_INFO, "        EventLogLocation - EventLogLocation - %x\n", *EventLogLocation));
  return EFI_SUCCESS;
}

/**
   TPM 2.0 active locality test case

   @retval TRUE     Test case was execution steps completed and output result is valid.
   @retval FALSE    Error occurred during test case execution.
**/
BOOLEAN
CheckTpmActiveLocality (
  VOID
  )
{
  UINTN   TpmBaseAddress;
  UINT8   TpmFifoAccess;
  UINT32  TpmCrbLocalityState;
  PTP_CRB_INTERFACE_IDENTIFIER TpmInterfaceId;

  TpmBaseAddress = (UINTN)PcdGet64 (PcdTpmBaseAddress);

  TpmInterfaceId.Uint32    = MmioRead32 ((UINTN)&((PTP_CRB_REGISTERS *)TpmBaseAddress)->InterfaceId);

  if (TpmInterfaceId.Bits.InterfaceType == PTP_INTERFACE_IDENTIFIER_INTERFACE_TYPE_FIFO) {
    //
    // supported interface is FIFO
    //
    TpmFifoAccess = MmioRead8 (TpmBaseAddress + OFFSET_OF(PTP_FIFO_REGISTERS, Access));
    DEBUG ((DEBUG_INFO, "         TPM FIFO access (0x%x) = 0x%x\n", TpmBaseAddress + OFFSET_OF(PTP_FIFO_REGISTERS, Access), TpmFifoAccess));
    if (((TpmFifoAccess & PTP_FIFO_VALID) == 0) ||
        ((TpmFifoAccess & PTP_FIFO_ACC_ACTIVE) == 0) ||
        ((TpmFifoAccess & PTP_FIFO_ACC_SEIZED) != 0)) {
      // Tpm present but locality is not properly configured
      DEBUG ((DEBUG_INFO, "         TPM FIFO access invalid"));
      return FALSE;
    }
  } else if (TpmInterfaceId.Bits.InterfaceType == PTP_INTERFACE_IDENTIFIER_INTERFACE_TYPE_CRB) {

    //
    // supported interface is CRB
    //
    TpmCrbLocalityState = MmioRead32 (TpmBaseAddress + OFFSET_OF(PTP_CRB_REGISTERS, LocalityState));
    DEBUG ((DEBUG_INFO, "         TPM CRB locality state (0x%x) = 0x%x\n", TpmBaseAddress + OFFSET_OF(PTP_CRB_REGISTERS, LocalityState), TpmCrbLocalityState));
    if (((TpmCrbLocalityState & PTP_CRB_LOCALITY_STATE_TPM_REG_VALID_STATUS) == 0) ||
        ((TpmCrbLocalityState & PTP_CRB_LOCALITY_STATE_LOCALITY_ASSIGNED) == 0) ||
        ((TpmCrbLocalityState & PTP_CRB_LOCALITY_STATE_ACTIVE_LOCALITY_MASK) != 0) ) {
      // Tpm present but locality is not properly configured
      DEBUG ((DEBUG_INFO, "         TPM CRB locality state is invalid"));
      return FALSE;
    }
  } else {

    //
    // TpmIntfCapability is invalid
    //
    DEBUG ((DEBUG_INFO, "         TPM interface capability is invalid"));
    return FALSE;
  }

  return TRUE;
}

/**
   TPM 2.0 establishment test case

   @retval TRUE     Test case was execution steps completed and output result is valid.
   @retval FALSE    Error occurred during test case execution.
**/
BOOLEAN
CheckTpmEstablishment (
  VOID
  )
{
  UINTN   TpmBaseAddress;
  UINT8   TpmFifoAccess;
  UINT32  TpmCrbLocalityState;
  PTP_CRB_INTERFACE_IDENTIFIER   TpmInterfaceId;

  TpmBaseAddress = (UINTN)PcdGet64 (PcdTpmBaseAddress);

  TpmInterfaceId.Uint32 = MmioRead32 ((UINTN)&((PTP_CRB_REGISTERS *)TpmBaseAddress)->InterfaceId);

  if (TpmInterfaceId.Bits.InterfaceType == PTP_INTERFACE_IDENTIFIER_INTERFACE_TYPE_FIFO) {
    //
    // supported interface is FIFO
    //
    TpmFifoAccess = MmioRead8 (TpmBaseAddress + OFFSET_OF(PTP_FIFO_REGISTERS, Access));
    if ((TpmFifoAccess & PTP_FIFO_ACC_ESTABLISH) == 0) {
        // tpm has been established
        DEBUG ((DEBUG_INFO, "         TPM has been established"));
        return FALSE;
    }
  } else if (TpmInterfaceId.Bits.InterfaceType == PTP_INTERFACE_IDENTIFIER_INTERFACE_TYPE_CRB) {
    //
    // supported interface is CRB
    //
    TpmCrbLocalityState = MmioRead32 (TpmBaseAddress + OFFSET_OF(PTP_CRB_REGISTERS, LocalityState));
    if ((TpmCrbLocalityState & PTP_CRB_LOCALITY_STATE_TPM_ESTABLISHED) == 0) {
      // tpm has been established
      DEBUG ((DEBUG_INFO, "         TPM has been established"));
      return FALSE;
    }
  } else {

    //
    // TpmIntfCapability is invalid
    //
    DEBUG ((DEBUG_INFO, "         TPM interface capability is invalid"));
    return FALSE;
  }

  return TRUE;
}
/**
  Run tests for MeasuredBootEnforcement bit
**/
VOID
CheckMeasuredBootEnforcement (
  VOID
  )
{
  EFI_STATUS                        Status;
  BOOLEAN                           Result;
  BOOLEAN                           TpmPresent;
  BOOLEAN                           PcrInitialized;
  BOOLEAN                           HierarchyChangeSuccess;
  EFI_PHYSICAL_ADDRESS              EventLogLocation;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT) == 0) {
    return;
  }

  //
  // TPM Presence
  //
  Result     = TRUE;
  TpmPresent = TRUE;
  Status     = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "      Chipset supports FW TPM or dTPM Test\n"));
  DEBUG ((DEBUG_INFO, "        If chipset supports FW TPM it must be enabled or the platform must have a dTPM\n"));

  if (CheckTpmChipsetSupport () == FALSE) {

    DEBUG ((DEBUG_INFO, "        Unexpected Status: FW TPM not configured or no dTPM present on system\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_1,
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_1
      );

    Result = FALSE;
    TpmPresent = FALSE;
  }

  //
  // PCR Initialization
  //
  DEBUG ((DEBUG_INFO, "      PCR[7] Initialization Check Test\n"));
  PcrInitialized = FALSE;

  if (TpmPresent == TRUE) {
    Status = CheckPcrInitialization (PCR_7_SELECTION_MASK, &PcrInitialized);
  }

  if ((TpmPresent == FALSE) || (PcrInitialized == FALSE) || EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, " Unexpected Status: Pcr[7] is not initialized and it is Active\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_2,
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_2
      );

    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      PCR[0] Initialization Check Test\n"));
  PcrInitialized = FALSE;

  if (TpmPresent == TRUE) {
    Status = CheckPcrInitialization (PCR_0_SELECTION_MASK, &PcrInitialized);
  }

  if ((TpmPresent == FALSE) || (PcrInitialized == FALSE) || EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Pcr[0] is not initialized and it is Active\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_9,
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_9
      );

    Result = FALSE;
  }

  //
  // TPM Event Log
  //
  DEBUG ((DEBUG_INFO, "      Event log published Test\n"));
  EventLogLocation = (EFI_PHYSICAL_ADDRESS) NULL;

  if (TpmPresent == TRUE) {
    Status = CheckTpmEventLog (&EventLogLocation);
  }

  if ((TpmPresent == FALSE)                             ||
      (EventLogLocation == (EFI_PHYSICAL_ADDRESS) NULL) ||
      EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Event log is not published and must be\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_3,
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_3
      );

    Result = FALSE;
  }

  //
  // TPM Platform Auth Value
  //
  DEBUG ((DEBUG_INFO, "      Platform Auth Value All Zero Test\n"));
  HierarchyChangeSuccess = TRUE;

  if (TpmPresent == TRUE) {
    Status = CheckTpm2HierarchyChangeAuth (&HierarchyChangeSuccess);
  }

  if ((TpmPresent == FALSE)            ||
      (HierarchyChangeSuccess == TRUE) ||
      EFI_ERROR (Status)) {

    if (Status == EFI_INVALID_PARAMETER) {
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_5,
        HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_5
        );
    } else {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: Platform Auth Value is Zero\n"));

      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_4,
        HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_4
        );
    }
    Result = FALSE;
  }

  //
  // TPM Active locality
  //
  DEBUG ((DEBUG_INFO, "      Active locality test\n"));
  if ((TpmPresent == FALSE) || CheckTpmActiveLocality () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TPM locality configuration is invalid\n"));
    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_7,
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_7
      );
  }

  //
  // TPM Established
  //
  DEBUG ((DEBUG_INFO, "      Establishment test\n"));
  if ((TpmPresent == FALSE) || (CheckTpmEstablishment () == FALSE)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TPM establishment bit is set\n"));
    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_CODE_8,
      HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT_UNEXP_STATUS_STRING_8
      );
  }
  //
  // ALL PASS
  //
  if (Result) {

    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               0,
               HSTI_BYTE0_MEASURED_BOOT_ENFORCEMENT
               );

    CheckStatusForHstiLibSet (Status);
  }

  return;
}
