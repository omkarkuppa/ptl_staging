/** @file
  Power state and boot mode save and restore data functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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
#ifndef _MrcSaveRestore_h_
#define _MrcSaveRestore_h_

#include "CMrcTypes.h"
#include "CMrcApi.h"

typedef enum {
  PtNone,
  PtMc,
  PtCh,
  PtByte,
  PtCCC,
  PtGenFsm,
  PtDATAShared,
  PtDATASbmem,
  PtVccClk,
  PtCCCShared,
  PtCCCSbmem,
  PtPG,
  PtMax
} PartitionScope;

typedef enum {
  StCommon,
  StSaGv,
  StMax
} SaveRestoreScope;

typedef enum {
  R32Bit,
  R64Bit
} RegSize;

#pragma pack(push, 1)
typedef struct {
  UINT16 StartMchbarOffset;
  UINT16 EndMchbarOffset;
} SaveDataControl;

typedef struct {
  const SaveDataControl *SaveDataControlData;
  UINT32                DataSize;
  PartitionScope        PScope;
  SaveRestoreScope      SRScope;
  RegSize               RegSize;
#ifdef MRC_DEBUG_PRINT
  char                  *ArrayName;
#endif // MRC_DEBUG_PRINT
} SaveDataLoopControl;
#pragma pack(pop)

/**
  This function verifies that neither CPU fuses or DIMMs have changed.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess if fast boot is allowed, otherwise mrcColdBootRequired.
**/
extern
MrcStatus
MrcFastBootPermitted (
  IN     MrcParameters *const MrcData
  );

/**
  This function saves any values that need to be used during non-cold boots.

  @param[in, out] MrcData - Include all the MRC global data.

  @retval mrcSuccess if the save occurred with no errors, otherwise returns an error code.
**/
extern
MrcStatus
MrcSaveMCValues (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function saves any values that need to be used during non-cold boots which may have changed

  @param[in, out] MrcData - Include all the MRC global data.

  @retval mrcSuccess if the save occurred with no errors, otherwise returns an error code.
**/
extern
MrcStatus
MrcUpdateSavedMCValues (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function saves any remaining values that need to be used during non-cold boots.

  @param[in, out] MrcData - Include all the MRC global data.

  @retval mrcSuccess if the save occurred with no errors, otherwise returns an error code.
**/
extern
MrcStatus
MrcSaveMCValuesFinal (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function restores host structure data for CmdPiCode/CtlPiCode and update cache for RcvEn, TxDqs, TxDq
  This is needed to do CmdT and CmdV margining in RMT on Fast Flow and for LP5 since we are using Low/POR frequency switch duting JEDEC reset.

  @param[in] MrcData - Include all the MRC global data.

  @retval mrcSuccess if the memory controller write back completed with no errors, otherwise returns an error code.
**/
MrcStatus
MrcRestoreCCCHostStructure (
  IN     MrcParameters *const MrcData
  );

/**
  This function copies the non-training information that needs to be restored
  from the 'save' data structure to the 'Output' data structure.

  @param[in, out] MrcData - include all the MRC global data.

  @retval mrcSuccess if the copy completed with no errors, otherwise returns an error code.
**/
extern
MrcStatus
MrcRestoreNonTrainingValues (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function writes the previously determined training values back to the memory controller.

  @param[in] MrcData - Include all the MRC global data.

  @retval mrcSuccess if the memory controller write back completed with no errors, otherwise returns an error code.
**/
extern
MrcStatus
MrcRestoreTrainingValues (
  IN     MrcParameters *const MrcData
  );

/**
  Populates MRC host data for registers used in
  MrcMcRegisterOffset() using values from the actual register
  values.

  @param[in, out] MrcData  - The MRC "global data" area.

  @retval Nothing.
**/
void
MrcPopulateHostData (
  IN OUT MrcParameters *MrcData
  );


#endif
