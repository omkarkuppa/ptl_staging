/** @file
  Header file for SMM Control PEI Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _PCH_SMM_CONTROL_LIB_H_
#define _PCH_SMM_CONTROL_LIB_H_

/**
  This function install PEI SMM Control PPI

  @retval EFI_STATUS  Results of the installation of the SMM Control PPI
**/
EFI_STATUS
EFIAPI
PchSmmControlInit (
  VOID
  );

#endif
