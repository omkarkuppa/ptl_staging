/** @file
  This file contains definitions of WDT functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#ifndef _PEI_WDT_LIB_H_
#define _PEI_WDT_LIB_H_

/**
  This runtine clears status bits and disable watchdog, then lock the
  WDT registers.
  while WDT is designed to be disabled and locked by policy,
  bios should not enable WDT by WDT PPI. In such case, bios shows the
  warning message but not disable and lock WDT register to make sure
  WDT event trigger correctly.

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
WdtDisableAndLock (
  IN  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi
  );

/**
  Install PchResetCallback for WDT.
**/
VOID
WdtInstallResetCallback (
  VOID
  );

/**
  Disable WDT when S3 resume before OS.
**/
VOID
WdtDisableBeforeS3Resume (
  VOID
  );
#endif // _PEI_WDT_LIB_H_
