/**@file
  Header file for the MeSmbios Driver.

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

#ifndef _ME_SMBIOS_INTERNAL_H_
#define _ME_SMBIOS_INTERNAL_H_

#include <Base.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Protocol/Smbios.h>
#include <MeSmbios.h>

EFI_STATUS
AddSmbiosRecord (
  IN VOID   *SmbiosTable,
  IN UINT32 SmbiosTableSize
  );

/**
  Create the FWSTS info to the SMBIOS table using the SMBIOS protocol
  Invoke this routine to add the table entry when all the FWSTS data is finalized.

  @param[in]  Event               - A pointer to the Event that triggered the callback.
  @param[in]  Context             - A pointer to private data registered with the callback

  @retval EFI_SUCCESS             - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES    - if not able to get resources.
  @retval EFI_UNSUPPORTED         - if required DataHub or SMBIOS protocol is not available.
**/
VOID
EFIAPI
AddFwStsSmbiosEntry (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  );

EFI_STATUS
CreateSmbiosTableType130 (
  VOID
  );

EFI_STATUS
CreateSmbiosTableType131 (
  VOID
  );

#endif // _ME_SMBIOS_INTERNAL_H_
