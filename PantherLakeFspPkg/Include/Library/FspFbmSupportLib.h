/** @file
  This file contains functions to support FBM/BPM operations in FSP.

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

#ifndef __FSP_FBM_SUPPORT_LIB_H__
#define __FSP_FBM_SUPPORT_LIB_H__

#include <Uefi.h>
#include <BspmDef.h>
#include <FbmDef.h>

/**
  Find FIT entry address for BPM

  @return FIT entry address for BPM

**/
VOID *
EFIAPI
FindBpmFitEntry (
  VOID
  );

/**
  Find BPM entry address from FIT

  @return BPM address

**/
VOID *
EFIAPI
FindBpm (
  VOID
  );

/**
  Find BPM element by structureID

  @param[in] Bpm           BPM address
  @param[in] StructureID   BPM element StructureID

  @return BPM element
**/
VOID *
EFIAPI
FindBpmElement (
  IN BOOT_POLICY_MANIFEST_HEADER         *Bpm,
  IN UINT64                              StructureId
  );

/**
 Locate FBM from FIT.

  @retval FBM structure location or NULL if not found.

**/
FSP_BOOT_MANIFEST_STRUCTURE *
EFIAPI
LocateFbm (
  VOID
  );

/**
 Locate BSPM structure from BPM. BSPM is one kind of BSSS format defined for FSP use.
 BPM layout is shown as below:

  BPM
    - BPM Header
    - IBBS
    - TXTS
    - PMDA
    - PCDS
      - PDRS
      - CNBS
      - BSIS
        - BSIS Header
        - BSSS Array
          - BSPM
          - Other BSSS format (Not Defined yet)
      - Other PCDE Elements
    - PMSG

  @retval Pointer for BSPM structure in BPM, or NULL if not found.
**/
BSPM_ELEMENT *
EFIAPI
LocateBspm (
  VOID
  );

/**
 Locate BpmIBB Element

  @retval BpmIbb structure location or NULL if not found.

**/
IBB_ELEMENT *
EFIAPI
LocateBpmIbbElement (
    VOID
    );

#endif
