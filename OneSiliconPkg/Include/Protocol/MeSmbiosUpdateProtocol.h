/** @file
  MeSmbiosTablesUpdateProtocol definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _ME_SMBIOS_UPDATE_PROTOCOL_H_
#define _ME_SMBIOS_UPDATE_PROTOCOL_H_

///
/// ME SMBIOS Update Protocol
///

/**
  This function is used to update ME SMBIOS table.

  @return Standard EFI_STATUS is returned.
**/
typedef
EFI_STATUS
(EFIAPI *ME_UPDATE_SMBIOS) (
  VOID
  );

/**
  ME SMBIOS Tables update protocol
  The interface functions are to update SMBIOS type 130 (in case of AMT) and 131.
  It is platform vendor's responsibility to implement the function instance.
**/
typedef struct {
  ME_UPDATE_SMBIOS                UpdateSmbios130;
  ME_UPDATE_SMBIOS                UpdateSmbios131;
} ME_SMBIOS_TABLES_UPDATE_PROTOCOL;

extern EFI_GUID gMeSmbiosTablesUpdateProtocolGuid;

#endif // _ME_SMBIOS_UPDATE_PROTOCOL_H_
