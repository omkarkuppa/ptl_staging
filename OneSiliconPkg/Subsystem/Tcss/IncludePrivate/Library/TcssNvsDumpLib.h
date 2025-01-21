/** @file
  TCSS ACPI NVS infomation dump function library.

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
#ifndef _TCSS_NVS_DUMP_LIB_H_
#define _TCSS_NVS_DUMP_LIB_H_


/**

  This function get ACPI TCSS variable from TCSS NVS protocol and print data.

  @retval EFI_SUCCESS               Print the TCSS NVS data successfully.
  @retval EFI_NOT_FOUND             Not found the statistics information.

**/
EFI_STATUS
EFIAPI
DumpTcssVariable (
  VOID
  );

#endif
