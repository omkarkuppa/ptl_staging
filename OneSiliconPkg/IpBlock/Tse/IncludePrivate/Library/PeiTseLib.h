/** @file
    Header file for PEI TSE Init Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _PEI_TSE_LIB_H_
#define _PEI_TSE_LIB_H_

/**
   Perform Total Storage Encryption initialization.

   This function needs to be called once the TseDataHob has been created and populated with the
   TSE policy and HW capabilities state.

   @param[in] CpuSecurityPreMemConfig Pointer to CPU_SECURITY_PREMEM_CONFIG instance

   @retval EFI_SUCCESS       The TSE initialization completed successfully.
   @retval EFI_NOT_FOUND     The TSE HOB was not found, initialization can not be performed.
**/
EFI_STATUS
EFIAPI
TseInit (
  IN CPU_SECURITY_PREMEM_CONFIG *CpuSecurityPreMemConfig
  );

#endif // _PEI_TSE_LIB_H_
