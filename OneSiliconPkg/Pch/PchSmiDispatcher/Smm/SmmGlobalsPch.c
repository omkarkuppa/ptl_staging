/** @file
  This driver is responsible for the registration of child drivers
  and the abstraction of the PCH SMI sources.

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
#include <Register/TcoRegs.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mTco1StsClear =
  (
    B_TCO_IO_TSTS1_CPUSERR_STS |
    B_TCO_IO_TSTS1_CPUSMI_STS |
    B_TCO_IO_TSTS1_CPUSCI_STS |
    B_TCO_IO_TSTS1_BIOSWR_STS |
    B_TCO_IO_TSTS1_NEWCENTURY_STS |
    B_TCO_IO_TSTS1_TIMEOUT |
    B_TCO_IO_TSTS1_TCO_INT_STS |
    B_TCO_IO_TSTS1_OS_TCO_SMI
    );
