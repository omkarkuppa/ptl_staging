/** @file
  Acpi Gnvs Init Library header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include <Base.h>


/**
@brief
  Global NVS initialize.

  @param[in] PlatformNvsAreaPtr    - Pointer of global NVS area

  @retval EFI_SUCCESS              - Allocate Global NVS completed.
  @retval EFI_OUT_OF_RESOURCES     - Failed to allocate required page for PlatformNvsAreaProtocol.
**/
EFI_STATUS
EFIAPI
AcpiGnvsInit (
  IN OUT VOID               **PlatformNvsAreaPtr
  );

