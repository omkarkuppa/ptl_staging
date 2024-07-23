/** @file
  Header file for SMBIOS CPU library.

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

@par Specification Reference:
  System Management BIOS (SMBIOS) Reference Specification v3.0.0
  dated 2015-Feb-12 (DSP0134)
  http://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.0.0.pdf
**/

#ifndef _SMBIOS_CPU_LIB_H_
#define _SMBIOS_CPU_LIB_H_

/**
  This function produces SMBIOS Cache Info HOBs, and an SMBIOS Processor Info HOB.
**/
VOID
EFIAPI
InitializeSmbiosCpuHobs (
  VOID
  );

#endif
