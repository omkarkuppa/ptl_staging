/** @file
  Intel One Click Recovery HTTPS Certificate functional definitions.

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

#ifndef _OCR_CERTIFICATE_SUPPORT_H_
#define _OCR_CERTIFICATE_SUPPORT_H_

#include <PiDxe.h>
#include <Protocol/AsfProtocol.h>
#include <Guid/TlsAuthentication.h>
#include <Guid/ImageAuthentication.h>

/**
  Get All Root CA Certificates

  @retval     EFI_SUCCESS  successful sync CA Certificates
**/
EFI_STATUS
GetRootCaCertificates (
  VOID
  );

/**
  Function to Delete ALL x509 certificates

  @param[in] OcrTlsGuid         Guid associated with certificate

  @retval EFI_SUCCESS           Deleted x509 Certificate
  @retval EFI_OUT_OF_RESOURCES  Out of resources
**/
EFI_STATUS
DeleteAllCertificates (
  VOID
  );

/**
  Function to Delete x509 certificate

  @param[in] OcrTlsGuid         Guid associated with certificate

  @retval EFI_SUCCESS           Deleted x509 Certificate
  @retval EFI_OUT_OF_RESOURCES  Out of resources
**/
EFI_STATUS
DeleteOcrX509Certificate (
  IN EFI_GUID  *OcrTlsGuid
  );

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
  );

#endif // _OCR_CERTIFICATE_SUPPORT_H_
