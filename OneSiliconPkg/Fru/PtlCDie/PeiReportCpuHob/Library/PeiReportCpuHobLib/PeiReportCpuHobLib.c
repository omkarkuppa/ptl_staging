/** @file

  Source code file for Report CPU HOB library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/HobLib.h>
#include <Library/CpuPlatformLib.h>

/**
  Platform specific implementation to build the CPU HOB.
**/
VOID
EFIAPI
ReportCpuHob (
  VOID
  )
{
  //
  // Create a CPU hand-off information
  //
  BuildCpuHob (GetMaxPhysicalAddressSize (), 16);
}
