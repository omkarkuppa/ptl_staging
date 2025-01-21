/** @file
  Disable BME on TBT Hierarchy Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _TBT_SECURITY_LIB_H_
#define _TBT_SECURITY_LIB_H_

#include <Uefi.h>
#include <PcieRegs.h>
#include <PeiITbtGenericStructure.h>
#include <IndustryStandard/Pci.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/PcieHelperLib.h>

typedef struct {
  UINT32 Seg     : 8;
  UINT32 Bus     : 8;
  UINT32 Dev     : 5;
  UINT32 Func    : 3;
  UINT32 PcieCap : 8;
} SBDF;

typedef enum {
  DevTypePci,
  DevTypePcieEndpoint,
  DevTypePcieUpstream,
  DevTypePcieDownstream,
  DevTypeMax
} PCI_DEV_TYPE;

/**
  Recursive Tbt Hierarchy Configuration

  @param[in]  Sbdf       device's segment:bus:device:function coordinates
**/
VOID
RecursiveTbtHierarchyConfiguration (
  IN SBDF       Sbdf
  );

#endif
