/** @file
  Intel One Click Recovery HTTPS Certificate Functional Implementation.

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

@par Specification Reference:
**/

#include "OcrHttpsCertificate.h"
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/TlsAuthentication.h>
#include <Guid/ImageAuthentication.h>

// List of GUIDs for registering Certificates
STATIC EFI_GUID OcrTlsAuthConfigGuid[MAX_NUM_ROOT_CA_CERTIFICATE] = {
  { 0xf575f8b1, 0xc4a8, 0x4571, { 0xba, 0xb0, 0x13, 0x5d, 0xbb, 0x09, 0xdb, 0xff }},
  { 0x352791b0, 0x2931, 0x4e4e, { 0x91, 0xf8, 0x0b, 0xb4, 0xda, 0x6e, 0xc7, 0x6a }},
  { 0xf021cb20, 0xfe1b, 0x4fec, { 0x8a, 0x2e, 0x73, 0x77, 0x4d, 0x18, 0x0c, 0x38 }},
  { 0xb801e82a, 0xb794, 0x4f56, { 0xbc, 0x15, 0xb0, 0x78, 0x12, 0xbc, 0x57, 0x32 }}
  };

/**
  Get All Root CA Certificates

  @retval     EFI_SUCCESS  successful sync CA Certificates
**/
EFI_STATUS
GetRootCaCertificates (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT32                          Index;
  ROOT_CA_CERTIFICATE             Cert;

  // Get all Certificates and enroll them
  for (Index = 0; Index < MAX_NUM_ROOT_CA_CERTIFICATE; Index++) {
    Status = AsfGetRootCaCertificate (Index, &Cert);

    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Root CA Certificate\n Index: %d\n CertSize: %d\n", Index, Cert.CertSize));
      Status = EnrollOcrX509Certificate (Cert.Cert, Cert.CertSize, &OcrTlsAuthConfigGuid[Index]);
    }

    if (EFI_ERROR (Status) || Cert.LastCert) {
      break;
    }
  }

  return Status;
}

/**
  Function to Delete x509 certificate

  @param[in] OcrTlsGuid         Guid associated with certificate

  @retval EFI_SUCCESS           Deleted x509 Certificate
  @retval EFI_OUT_OF_RESOURCES  Out of resources
**/
EFI_STATUS
DeleteOcrX509Certificate (
  IN EFI_GUID  *OcrTlsGuid
  )
{
  EFI_STATUS                  Status;
  UINTN                       DataSize;
  UINT32                      Attr;
  UINT32                      Index;
  UINT8                       *OldCertListData;
  UINT8                       *NewCertListData;
  EFI_SIGNATURE_LIST          *NewCertList;
  EFI_SIGNATURE_LIST          *TempCertList;
  EFI_SIGNATURE_DATA          *CertData;
  UINTN                       CertCount;
  UINT32                      TempCertOffset;
  UINT32                      Block1Size;
  UINT32                      Block2Size;
  UINT32                      OldBlock2Offset;
  BOOLEAN                     IsItemFound;
  UINT32                      ItemDataSize;

  NewCertList     = NULL;
  Attr            = 0;
  Block1Size      = 0;
  Block2Size      = 0;

  if (OcrTlsGuid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get original signature list data.
  //
  DataSize = 0;
  Status = gRT->GetVariable (EFI_TLS_CA_CERTIFICATE_VARIABLE, &gEfiTlsCaCertificateGuid, NULL, &DataSize, NULL);
  if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
    return Status;
  }

  OldCertListData = (UINT8 *) AllocateZeroPool (DataSize);
  if (OldCertListData == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  Status = gRT->GetVariable (EFI_TLS_CA_CERTIFICATE_VARIABLE, &gEfiTlsCaCertificateGuid, &Attr, &DataSize, OldCertListData);
  if (EFI_ERROR(Status)) {
    FreePool (OldCertListData);
    return Status;
  }

  //
  // Allocate space for new variable.
  //
  NewCertListData = (UINT8 *) AllocateZeroPool (DataSize);
  if (NewCertListData == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    FreePool (OldCertListData);
    return Status;
  }

  //
  // Enumerate all data and erase the target item.
  //
  IsItemFound = FALSE;
  ItemDataSize = (UINT32) DataSize;
  TempCertOffset = 0;
  TempCertList = (EFI_SIGNATURE_LIST*) OldCertListData;

  while ((ItemDataSize > 0) && (ItemDataSize >= TempCertList->SignatureListSize) && !IsItemFound) {
    // Move TempCertList to current
    TempCertList = (EFI_SIGNATURE_LIST*) (OldCertListData + TempCertOffset);

    if (CompareGuid(&TempCertList->SignatureType, &gEfiCertX509Guid)) {
      // Calculate the certificate count in List
      CertData = (EFI_SIGNATURE_DATA *) ((UINT8 *) TempCertList + sizeof (EFI_SIGNATURE_LIST) + TempCertList->SignatureHeaderSize);
      CertCount  = (TempCertList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) - TempCertList->SignatureHeaderSize) / TempCertList->SignatureSize;

      // Search for the OCR TLS Certificate
      for (Index = 0; Index < CertCount; Index++) {
        if (CompareGuid(&CertData->SignatureOwner, OcrTlsGuid)) {
          IsItemFound = TRUE;
          // Calculate Block 1 size, old certificate list block 2 offset, and Block 2 size
          if (CertCount == 1) {
            // if this is only certificate in list, then delete whole list including header
            Block1Size = TempCertOffset;
            OldBlock2Offset = Block1Size + TempCertList->SignatureListSize;
            Block2Size = (UINT32) DataSize - OldBlock2Offset;

            CopyMem (NewCertListData, OldCertListData, Block1Size);
            CopyMem (NewCertListData + Block1Size, OldCertListData + OldBlock2Offset, Block2Size);
          } else {
            // Only delete the certificate
            Block1Size = TempCertOffset + sizeof (EFI_SIGNATURE_LIST) + TempCertList->SignatureHeaderSize + (Index * TempCertList->SignatureSize);
            OldBlock2Offset = Block1Size + TempCertList->SignatureSize;
            Block2Size = (UINT32) DataSize - OldBlock2Offset;

            CopyMem(NewCertListData, OldCertListData, Block1Size);
            CopyMem(NewCertListData + Block1Size, OldCertListData + OldBlock2Offset, Block2Size);

            TempCertList = (EFI_SIGNATURE_LIST*) (NewCertListData + TempCertOffset);
            NewCertList = (EFI_SIGNATURE_LIST*) NewCertListData;
            NewCertList->SignatureListSize -= TempCertList->SignatureSize;
          }
          break;
        }
      }
    }
    TempCertOffset += TempCertList->SignatureListSize;
    ItemDataSize -= TempCertList->SignatureListSize;
  }

  if (IsItemFound) {
    DataSize  = Block1Size + Block2Size;

    Status = gRT->SetVariable(
                EFI_TLS_CA_CERTIFICATE_VARIABLE,
                &gEfiTlsCaCertificateGuid,
                Attr,
                DataSize,
                NewCertListData
                );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to set variable, Status = %r\n", Status));
    }
  }

  if (OldCertListData != NULL) {
    FreePool (OldCertListData);
  }

  if (NewCertListData != NULL) {
    FreePool (NewCertListData);
  }

  return Status;

}

/**
  Function to enroll x509 certificate

  @param[in] X509Data            X509 Certificate Data
  @param[in] X509DataSize        X509 Certificate Data Size
  @param[in] OcrTlsGuid          Guid associated with certificate

  @retval EFI_SUCCESS            Successfully enrolled x509 Certificate
  @retval EFI_OUT_OF_RESOURCES   Out of resources
**/
EFI_STATUS
EnrollOcrX509Certificate (
  IN VOID      *X509Data,
  IN UINTN     X509DataSize,
  IN EFI_GUID  *OcrTlsGuid
  )
{
  EFI_STATUS                        Status;
  EFI_SIGNATURE_LIST                *CACert;
  EFI_SIGNATURE_DATA                *CACertData;
  UINTN                             DataSize;
  UINTN                             SigDataSize;
  UINT32                            Attr;

  if (X509Data == NULL || X509DataSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  SigDataSize = sizeof (EFI_SIGNATURE_LIST) + sizeof (EFI_SIGNATURE_DATA) - 1 + X509DataSize;

  CACert = (EFI_SIGNATURE_LIST*) AllocateZeroPool (SigDataSize);
  if (CACert == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CACert->SignatureListSize = (UINT32) SigDataSize;
  CACert->SignatureHeaderSize = 0;
  CACert->SignatureSize = (UINT32) (sizeof (EFI_SIGNATURE_DATA) - 1 + X509DataSize);
  CopyGuid (&CACert->SignatureType, &gEfiCertX509Guid);

  CACertData = (EFI_SIGNATURE_DATA*) ((UINT8* ) CACert + sizeof (EFI_SIGNATURE_LIST));
  CopyGuid (&CACertData->SignatureOwner, OcrTlsGuid);
  CopyMem ((UINT8* ) (CACertData->SignatureData), X509Data, X509DataSize);

  //
  // Check if signature database entry has been already existed.
  // If true, use EFI_VARIABLE_APPEND_WRITE attribute to append the
  // new signature data to original variable
  //
  Attr = (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS);

  Status = gRT->GetVariable(
                  EFI_TLS_CA_CERTIFICATE_VARIABLE,
                  &gEfiTlsCaCertificateGuid,
                  NULL,
                  &DataSize,
                  NULL
                  );

  if (Status == EFI_BUFFER_TOO_SMALL) {
    Attr |= EFI_VARIABLE_APPEND_WRITE;
  } else if (Status != EFI_NOT_FOUND) {
    if (CACert != NULL) {
      FreePool (CACert);
    }
    return Status;
  }

  Status = gRT->SetVariable(
                  EFI_TLS_CA_CERTIFICATE_VARIABLE,
                  &gEfiTlsCaCertificateGuid,
                  Attr,
                  SigDataSize,
                  (VOID *) CACert
                  );

  if (CACert != NULL) {
    FreePool (CACert);
  }

  return Status;
}

/**
  Function to Delete ALL certificates

  @retval EFI_SUCCESS           Deleted ALL certificates
  @retval other                 Failed to delete certificates
**/
EFI_STATUS
DeleteAllCertificates (
  VOID
 )
{
  EFI_STATUS    Status;
  UINT8         Index;
  //
  // Set Recursive call to delete all certificates
  //
  for (Index = 0; Index < MAX_NUM_ROOT_CA_CERTIFICATE; Index++) {
    Status = DeleteOcrX509Certificate (&OcrTlsAuthConfigGuid[Index]);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "[%a] error status %r", __FUNCTION__, Status));
      break;
    }
  }
  return Status;
}
