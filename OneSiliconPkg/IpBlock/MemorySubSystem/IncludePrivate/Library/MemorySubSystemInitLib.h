/** @file
  Header file for Memory Sub System Init Library

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

#ifndef _PEI_MEMORY_SUB_SYS_INIT_LIB_H_
#define _PEI_MEMORY_SUB_SYS_INIT_LIB_H_

#include <Base.h>
#include <CMrcInterface.h>
#include <Ppi/SiPolicy.h>

/**
 Initialization of Memory SubSystem
**/
VOID
EFIAPI
PeiMemorySubSystemInit (
  IN  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi,
  IN  MrcParameters           *CONST MrcData
  );

/**
  Initialization of IMR Exclusion Registers.

  @retval     EFI_SUCCES         Initialized successfully
**/
EFI_STATUS
EFIAPI
PeiImrExclusionInit (
  VOID
  );

/*
  This function is to poll Path_to_memory_available with BIOS Mailbox Command which indicates Path to memory available.

  @retval EFI_STATUS     - Successful if bit is set.
*/

EFI_STATUS
PollPathToMemAvailable (
  VOID
  );
#endif
