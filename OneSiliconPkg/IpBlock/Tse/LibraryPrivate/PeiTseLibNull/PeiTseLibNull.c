/** @file
  Initilize TSE in PEI

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

@par Specification Reference:
**/

/**
   Perform Total Storage Encryption initialization.

   Null library empty implementation

   @retval EFI_SUCCESS       The TSE initialization completed successfully.
   @retval EFI_UNSUPPORTED   TSE is not supported
**/
EFI_STATUS
TseInit (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}
