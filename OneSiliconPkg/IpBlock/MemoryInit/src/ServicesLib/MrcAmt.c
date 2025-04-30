/** @file
  This file contains functions to detect and record DRAM Failing addresses via
  Advanced Memory Test.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "MrcAmt.h"
#include "MrcPpr.h"
#include "MrcAmtPprInterface.h"
#include "MrcCommon.h"
#include "MrcMemoryApi.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcSpdProcessingDefs.h"
#include "TestDataEngine.h"
#include "MrcIoTest.h"


/**
  Get a pointer to Row fail range entry at the given list index

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number
  @param[in] FailIndex      - index into the Row fail list

  @retval Pointer to Row fail range entry
**/
ROW_FAIL_RANGE *
AmtGetRowFailRange (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel,
  UINT32                  FailIndex
  )
{
  MRC_DEBUG_ASSERT (FailIndex < MAX_FAIL_RANGE, &MrcData->Outputs.Debug, "AmtGetRowFailRange Fatal Error: FailIndex > MAX_FAIL_RANGE\n");

  return &(MrcData->Outputs.FailRange[Controller][Channel][FailIndex]);
} //AmtGetRowFailRange

/**
  Sets values for the fail range entry at the given index

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number
  @param[in] FailIndex      - index into the Row fail list
  @param[in] NewAddr        - ROW_ADDR to set
  @param[in] NewBgMask      - new BankGroupMask
  @param[in] Overflow       - Whether too many errors were found on this bank
  @param[in] RowOverride    - if non-negative, value gets written to RowAddr after NewAddr is written

  @retval None
**/
VOID
AmtSetRowFailRange (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel,
  UINT32                  FailIndex,
  ROW_ADDR                NewAddr,
  UINT32                  NewBgMask,
  BOOLEAN                 Overflow,
  INT32                   RowOverride
  )
{
  ROW_FAIL_RANGE      *FailRangePtr;

  MRC_DEBUG_ASSERT (FailIndex < MAX_FAIL_RANGE, &MrcData->Outputs.Debug, "AmtSetRowFailRange Fatal Error: FailIndex > MAX_FAIL_RANGE\n");

  FailRangePtr = AmtGetRowFailRange(MrcData, Controller, Channel, FailIndex);
  FailRangePtr->Addr = NewAddr;
  if (RowOverride >= 0) {
    FailRangePtr->Addr.Bits.Row = (UINT32)RowOverride;
  }
  FailRangePtr->BankGroupMask = NewBgMask;
  FailRangePtr->Overflow = Overflow;
} //AmtSetRowFailRange

/**
  Sets values for the fail range entry at the given index with separate parameters for each ROW_ADDR field

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number
  @param[in] FailIndex      - index into the Row fail list
  @param[in] Row            - Row address of the fail
  @param[in] BankPair       - Bank address of the fail
  @param[in] Rank           - Rank address of the fail
  @param[in] UpperBgMask    - Not used for DDR5
  @param[in] Valid          - Whether this entry is a valid failure (should be 1)
  @param[in] NewBgMask      - new BankGroupMask
  @param[in] Overflow       - Whether too many errors were found on this bank

  @retval None
**/
VOID
AmtSetRowFailRangeAddr (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel,
  UINT32                  FailIndex,
  UINT32                  Row,
  UINT32                  BankPair,
  UINT32                  Rank,
  UINT32                  UpperBgMask,
  UINT32                  Valid,
  UINT32                  NewBgMask,
  BOOLEAN                 Overflow
  )
{
  ROW_FAIL_RANGE      *FailRangePtr;

  MRC_DEBUG_ASSERT (FailIndex < MAX_FAIL_RANGE, &MrcData->Outputs.Debug, "AmtSetRowFailRangeAddr Fatal Error: FailIndex > MAX_FAIL_RANGE\n");

  FailRangePtr = AmtGetRowFailRange(MrcData, Controller, Channel, FailIndex);
  FailRangePtr->Addr.Data = 0;
  FailRangePtr->Addr.Bits.Row = Row;
  FailRangePtr->Addr.Bits.BankPair = BankPair;
  FailRangePtr->Addr.Bits.Rank = Rank;
  FailRangePtr->Addr.Bits.UpperBgMask = UpperBgMask;
  FailRangePtr->Addr.Bits.Valid = Valid;
  FailRangePtr->BankGroupMask = NewBgMask;
  FailRangePtr->Overflow = Overflow;
}

/**
  Get the number of entries in the Row fail list for given controller/channel

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number

  @retval Number of entries in the Row fail list
**/
UINT32
AmtGetRowFailListSize (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel
  )
{
  return MrcData->Outputs.FailMax[Controller][Channel];
} //AmtGetRowFailListSize

/**
  Get the current index in the Row fail list for given controller/channel

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number

  @retval Current row fail range index
**/
UINT32
AmtGetRowFailIndex (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel
  )
{
  return MrcData->Outputs.FailIndex[Controller][Channel];
} //AmtGetRowFailIndex


/**
  Set the current index in the Row fail list for given controller/channel

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number
  @param[in] FailIndex      - index into the Row fail list

  @retval   None
**/
VOID
AmtSetRowFailIndex (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel,
  UINT32                  FailIndex
  )
{
  MrcData->Outputs.FailIndex[Controller][Channel] = FailIndex;
} //AmtSetRowFailIndex

/**
  Set the number of entries in the Row fail list for given controller/channel

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel
  @param[in] FailMax        - Number of entries in Row fail list

  @retval   None
**/
VOID
AmtSetRowFailListSize (
  MrcParameters* const    MrcData,
  UINT8                   Controller,
  UINT8                   Channel,
  UINT32                  FailMax
  )
{
  MrcData->Outputs.FailMax[Controller][Channel] = FailMax;
} //AmtSetRowFailListSize

/**
  Creates a full DQ failure per technology.

  @param[in] MrcData        - Global MRC data structure
  @param[in,out] DqMask     - Bitmask of DQ errors

**/
VOID
AmtCreateDqFailure (
  IN     MrcParameters* const   MrcData,
  IN OUT UINT32                 DqMask[DQ_MASK_INDEX_MAX]
  )
{
  DqMask[0] = (MrcData->Outputs.IsDdr5) ? 0xFFFFFFFF : 0xFFFF;
  DqMask[1] = (MrcData->Outputs.EccSupport) ? 0xFF : 0;
}

/**
  Reads and returns approximate DIMM temperature via MR4 Refresh Multiplier.

  @param[in] MrcData      - Global MRC data structure
  @param[in] Controller   - Target Controller
  @param[in] Channel      - Target Channel
  @param[in] Rank         - Target Rank

  @retval   DeviceTemp    - MR4 Refresh Multiplier value.

**/
UINT8
MrcReadDeviceTemperature (
  IN MrcParameters* const   MrcData,
  IN UINT32                 Controller,
  IN UINT32                 Channel,
  IN UINT32                 Rank
  )
{
  MrcOutput     *Outputs;
  UINT8         MrrResult[MRC_MRR_ARRAY_SIZE];
  UINT8         MaxDeviceTemp;
  UINT8         CurDeviceTemp;
  MrcChannelOut *ChannelOut;
  MrcDimmOut    *DimmOut;
  BOOLEAN       Width16;
  UINT32        Device;
  UINT32        MaxDevices;

  Outputs       = &MrcData->Outputs;
  ChannelOut    = &Outputs->Controller[Controller].Channel[Channel];
  DimmOut       = &ChannelOut->Dimm[RANK_TO_DIMM_NUMBER (Rank)];
  Width16       = (DimmOut->SdramWidth == 16);
  MaxDevices    = Outputs->IsDdr5 ? Outputs->SdramCount : ((Outputs->LpByteMode) ? 2 : 1);


  MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR4, MrrResult); // DDR5 and LP5 both read MR4 for temperature
  MaxDeviceTemp = 0;
  for (Device = 0; Device < MaxDevices; Device++) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "MR4_Device[%d] value: 0x%x\n", Device, MrrResult[Device]);
    if (Outputs->IsLpddr) {
      // Only read bits 3:0 because 0b11111 applies when high temp operating limit exceeded
      // We do not need to distinguish between 0b01111 and 0b11111
      CurDeviceTemp = MrrResult[Device] & (MRC_BIT3 | MRC_BIT2 | MRC_BIT1 | MRC_BIT0); // Bits 4:0, but only read 3:0
    } else {
      if (Outputs->IsDdr5 && (Width16 && (Device % 2 == 1))) {
        continue; // Only even index is valid for x16 dram device
      }
      CurDeviceTemp = ((MrrResult[Device] & MRC_BIT5) >> 2) | (MrrResult[Device] & (MRC_BIT2 | MRC_BIT1 | MRC_BIT0)); // Bits 2:0, plus bit 5 shifted to bit 3 for extended temperature range
    }
    if (CurDeviceTemp > MaxDeviceTemp) {
      MaxDeviceTemp = CurDeviceTemp;
    }
  }

  return MaxDeviceTemp;
}

/**
  Prints an approximate DIMM temperature based on MR4 Refresh Multplier value.

  @param[in] MrcData      - Global MRC data structure
  @param[in] DeviceTemp   - MR4 Refresh Multiplier value.

**/
VOID
MrcPrintDeviceTemperature (
  IN MrcParameters* const   MrcData,
  IN UINT8                  DeviceTemp
  )
{
  MrcOutput     *Outputs;
  const CHAR8   *DeviceTempValue;

  Outputs = &MrcData->Outputs;

  if (Outputs->IsLpddr) {
    if (DeviceTemp <= 0x9) {
      DeviceTempValue = "below 85C";
    } else {
      DeviceTempValue = "above 85C";
    }
  } else {
    switch (DeviceTemp) {
      case 1:
        DeviceTempValue = "below 80C";
        break;
      case 2:
        DeviceTempValue = "80-85C";
        break;
      case 3:
        DeviceTempValue = "85-90C";
        break;
      case 4:
        DeviceTempValue = "90-95C";
        break;
      case 5:
        DeviceTempValue = "above 95C";
        break;
      case 8:
        DeviceTempValue = "below 75C";  // 8 or greater means Wide range supported
        break;
      case 9:
        DeviceTempValue = "75-80C";
        break;
      case 10:
        DeviceTempValue = "80-85C";
        break;
      case 11:
        DeviceTempValue = "85-90C";
        break;
      case 12:
        DeviceTempValue = "90-95C";
        break;
      case 13:
        DeviceTempValue = "95-100C";
        break;
      case 14:
        DeviceTempValue = "above 100C";
        break;
      default:
        DeviceTempValue = "undefined";
        break;
    }
  }

  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "  DeviceTemp: %s\n", DeviceTempValue);
}

/**
  Compare most significant channel address bits excluding row bits

  @param CurAddr          - First address to compare
  @param RowAddr          - Second address to compare

  @retval   TRUE for match, otherwise FALSE
**/
BOOLEAN
AmtCurrentAddrMatch (
  ROW_ADDR CurAddr,
  ROW_ADDR RowAddr
  )
{
  // Mask fields that we don't care to match
  RowAddr.Bits.Row = 0;
  CurAddr.Bits.Row = 0;
  RowAddr.Bits.UpperBgMask = 0;
  CurAddr.Bits.UpperBgMask = 0;

  if (CurAddr.Data == RowAddr.Data) {
    return TRUE;
  } else {
    return FALSE;
  }
} //AmtCurrentAddrMatch

/**
  Display the row failure list

  @param[in] MrcData            - Global MRC data structure
  @param[in] Controller         - Controller number
  @param[in] Channel            - Channel number

  @retval   None
**/
#if AMT_PRINT_ROW_FAIL_LIST
VOID
AmtDisplayRowFailureList (
  IN MrcParameters   *const   MrcData,
  IN UINT8                    Controller,
  IN UINT8                    Channel
  )
{
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  ROW_FAIL_RANGE      *FailRangePtr;
  ROW_ADDR            RowAddr;
  UINT32              FailIndex;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  for (FailIndex = 0; FailIndex < AmtGetRowFailListSize (MrcData, Controller, Channel); FailIndex++) {
    FailRangePtr = AmtGetRowFailRange (MrcData, Controller, Channel, FailIndex);
    RowAddr = FailRangePtr->Addr;

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RowFailIndex:%d\n  ROW_ADDR: Valid:%d Rank:%d BankPair:%d UpperBgMask:0x%X BankGroupMask:0x%X Row:0x%X Size:0x%X\n",
      FailIndex, RowAddr.Bits.Valid, RowAddr.Bits.Rank, RowAddr.Bits.BankPair,
      RowAddr.Bits.UpperBgMask, FailRangePtr->BankGroupMask, RowAddr.Bits.Row, FailRangePtr->Overflow);

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
  }
}
#endif

/**
  Shifts row fail list to right by number of entries starting at given index

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to compare
  @param[in] Channel        - Channel number to compare
  @param[in] Index          - List index to begin the shift operation
  @param[in] NumEntries     - Number of entries to shift

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtShiftFailRangeListRight (
  MrcParameters *const MrcData,
  UINT8                Controller,
  UINT8                Channel,
  UINT32               Index,
  UINT32               NumEntries
  )
{
  MRC_FUNCTION    *MrcCall;
  INT32           CurEntry;
  UINT32          FailMax;
  MrcStatus       Status = mrcSuccess;

  MrcCall = MrcData->Inputs.Call.Func;
  FailMax = AmtGetRowFailListSize (MrcData, Controller, Channel);

  //For last entry in list down to index, move entry up by NumEntries amount
  if (FailMax + NumEntries < MAX_FAIL_RANGE) {
    if (FailMax != 0) {
      for (CurEntry = FailMax - 1; CurEntry >= (INT32) Index; CurEntry--) {
        MrcCall->MrcCopyMem ((UINT8*) AmtGetRowFailRange (MrcData, Controller, Channel, CurEntry + NumEntries), (UINT8*) AmtGetRowFailRange (MrcData, Controller, Channel, CurEntry), sizeof(ROW_FAIL_RANGE));
        //*AmtGetRowFailRange (MrcData, Controller, Channel, CurEntry + NumEntries) = *AmtGetRowFailRange (MrcData, Controller, Channel, CurEntry);
      }
    }
    AmtSetRowFailListSize (MrcData, Controller, Channel, FailMax + NumEntries);
  } else {
    Status = mrcFail;
  }
  return Status;
} //AmtShiftFailRangeListRight

/**
  Shifts row fail list left by number of entries starting at given index

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to compare
  @param[in] Channel        - Channel number to compare
  @param[in] Index          - List index to begin the shift operation
  @param[in] NumEntries     - Number of entries to shift

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtShiftFailRangeListLeft (
  MrcParameters *const MrcData,
  UINT8                Controller,
  UINT8                Channel,
  UINT32               Index,
  UINT32               NumEntries
  )
{
  MRC_FUNCTION    *MrcCall;
  UINT32          CurEntry;
  UINT32          FailMax;
  MrcStatus       Status = mrcSuccess;

  MrcCall = MrcData->Inputs.Call.Func;
  FailMax = AmtGetRowFailListSize(MrcData, Controller, Channel);

  if (Index >= NumEntries) {
    //For index up to last entry in list, move entry down by NumEntries amount
    for (CurEntry = Index; CurEntry < FailMax; CurEntry++) {
      MrcCall->MrcCopyMem ((UINT8*) AmtGetRowFailRange (MrcData, Controller, Channel, CurEntry - NumEntries), (UINT8*) AmtGetRowFailRange (MrcData, Controller, Channel, CurEntry), sizeof(ROW_FAIL_RANGE));
      //*AmtGetRowFailRange (MrcData, Controller, Channel, CurEntry - NumEntries) = *AmtGetRowFailRange (MrcData, Controller, Channel, CurEntry);
    }
    AmtSetRowFailListSize(MrcData, Controller, Channel, FailMax - NumEntries);
  } else {
    Status = mrcFail;
  }
  return Status;
} // AmtShiftFailRangeListLeft

/**
  Remove an entry from the Row Fail Range list

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to update
  @param[in] Channel        - Channel number to update
  @param[in] FailIndex      - List index to be removed from the list

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtRemoveRowFailRange (
  MrcParameters *const MrcData,
  UINT8                Controller,
  UINT8                Channel,
  UINT32               FailIndex
)
{
  MrcStatus       Status = mrcSuccess;
  ROW_FAIL_RANGE  *FailRangePtr;

  // If removing the last fail in row range, it will not get overwritten
  // Zero the entry to avoid problems
  FailRangePtr = AmtGetRowFailRange(MrcData, Controller, Channel, FailIndex);
  FailRangePtr->Addr.Data = 0;
  FailRangePtr->BankGroupMask = 0;
  FailRangePtr->Overflow = 0;

  // Shift list left by 1 at next entry
  AmtShiftFailRangeListLeft (MrcData, Controller, Channel, FailIndex + 1, 1);
  AmtSetRowFailIndex (MrcData, Controller, Channel, FailIndex);

  return Status;
} // AmtRemoveRowFailRange

/**
  Sets values for the fail range entry at the given index

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number
  @param[in] Channel        - Channel number
  @param[in] FailIndex      - index into the Row fail list
  @param[in] NewFail        - FailRange for the new fail to add
  @param[in] SetIndex       - Whether to update the MrcOutputs->FailIndex pointer to the newly created failure
  @param[in,out] Status     - mrcSuccess / mrcFail

  @retval Whether the operation was successful
**/
UINT8
AmtInsertEntryIntoRowFailRange (
  IN     MrcParameters* const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT32               FailIndex,
  IN     ROW_FAIL_RANGE       *NewFail,
  IN     BOOLEAN              SetIndex,
  IN OUT MrcStatus            *Status
)
{
  UINT8           Done;

  MRC_DEBUG_ASSERT (FailIndex < MAX_FAIL_RANGE, &MrcData->Outputs.Debug, "AmtSetRowFailRangeAndCopyDqMask Fatal Error: FailIndex > MAX_FAIL_RANGE\n");

  if (AmtShiftFailRangeListRight (MrcData, Controller, Channel, FailIndex, 1) == mrcSuccess) {
    AmtSetRowFailRange (MrcData, Controller, Channel, FailIndex, NewFail->Addr, NewFail->BankGroupMask, NewFail->Overflow, -1);
    if (SetIndex) {
      AmtSetRowFailIndex (MrcData, Controller, Channel, FailIndex);
    }
    Done = 1;
  } else {
    Done = 0;
    *Status = mrcFail;
  }

  return Done;
}

/**
  Updates Row failure list with last failure, coalescing failure ranges where possible

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to update
  @param[in] Channel        - Channel number to update
  @param[in] NewFail        - New failure information

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtUpdateRowFailList (
  MrcParameters *const MrcData,
  UINT8                Controller,
  UINT8                Channel,
  ROW_FAIL_RANGE       NewFail
  )
{
  UINT32          index;
  MrcStatus       Status = mrcSuccess;
  ROW_FAIL_RANGE  *FailRangePtr;
  UINT8           done;

  // Search FailRange list for position to insert new failure data
  index = 0;
  done = 0;
  while (!done && (index < AmtGetRowFailListSize (MrcData, Controller, Channel) && (index + 2) < MAX_FAIL_RANGE) && (Status == mrcSuccess)) {

    FailRangePtr = AmtGetRowFailRange (MrcData, Controller, Channel, index);

    // Check search criteria to move to next entry
    // 1. Non-matching bank, subrank, rank
    // 2. New failure row is greater than current entry row
    if (!AmtCurrentAddrMatch (FailRangePtr->Addr, NewFail.Addr) || (NewFail.Addr.Bits.Row > FailRangePtr->Addr.Bits.Row)) {
      index++;
      continue;
    }

    // Current/Search entry:
    // 1. matching bank, subrank and rank;
    // 2. New failure row <= current row

    // Case:
    // Search fail row range:     x
    // New failure row range:     x
    //
    // Action:
    // Combine bank group masks.
    // We expect to see the same error on subsequent patterns until it is repaired.
    if (NewFail.Addr.Bits.Row == FailRangePtr->Addr.Bits.Row) {
      AmtSetRowFailIndex (MrcData, Controller, Channel, index);
      // clear the repair mask of new fail bankgroup mask.
      FailRangePtr->BankGroupRepairedMask &= ~NewFail.BankGroupMask;
      FailRangePtr->BankGroupMask |= NewFail.BankGroupMask;
      done = 1;
    }
    // Case:
    // Search fail row range:      x
    // New failure row range:     x
    // OR
    // Search fail row range:      x
    // New failure row range:   x
    //
    // Action:
    // Add new entry for lower row address
    else {  // NewFail.Addr.Bits.Row < FailRangePtr->Addr.Bits.Row
      done = AmtInsertEntryIntoRowFailRange (MrcData, Controller, Channel, index, &NewFail, TRUE, &Status);
    }
  } //while

    // Append entry to end of list
    // Shift list right by 1 at end of list
  if (!done) {
    done = AmtInsertEntryIntoRowFailRange (MrcData, Controller, Channel, index, &NewFail, TRUE, &Status);
  } // if !done

  return Status;
}  //AmtUpdateRowFailList

/**
  Updates Row failure list with last failure, coalescing failure ranges where possible

  @param[in]  MrcData                 - Global MRC data structure
  @param[in]  PprAmtData              - PPR and AMT data structure
  @param[in]  Controller              - Controller number of failure
  @param[in]  Channel                 - Channel number of failure
  @param[in]  ErrorStatus             - the failure information
  @param[in]  DeviceTemp              - Dimm Temperature on initial failure

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtUpdateRowFailures (
  IN  MrcParameters *const          MrcData,
  IN  PPR_AMT_PARAMETER_DATA *const PprAmtData,
  IN  UINT8                         Controller,
  IN  UINT8                         Channel,
  IN  ERROR_STATUS_AMT              ErrorStatus,
  IN  UINT8                         DeviceTemp[MAX_CONTROLLER][MAX_CHANNEL]
)
{
  MRC_FUNCTION       *MrcCall;
  MrcOutput          *Outputs;
  UINT32             ErrorStatusLo;
  UINT32             ErrorStatusEcc;
  MrcStatus          Status = mrcSuccess;
  ROW_FAIL_RANGE     NewFail;
  UINT32             ErrBank;

  MrcCall = MrcData->Inputs.Call.Func;

  MrcCall->MrcSetMem ((UINT8 *) &NewFail, sizeof (ROW_FAIL_RANGE), 0);

  MrcCall->MrcSetMem ((UINT8 *)&NewFail, sizeof (NewFail), 0);
  ErrorStatusLo = 0;
  ErrorStatusEcc = 0;

  Outputs = &MrcData->Outputs;
  NewFail.Addr.Data = 0;     // Initialize to Invalid state

  ErrorStatusLo = ErrorStatus.errData;
  ErrorStatusEcc = (Outputs->EccSupport) ? (ErrorStatus.errEcc & MRC_UINT8_MAX) : 0;

  Outputs->PprNumDetectedErrors++;
  Outputs->PprNumDetectedErrors = MIN(PPR_MAX_DETECTED_ERRORS, Outputs->PprNumDetectedErrors);

  if (ErrorStatusLo | ErrorStatusEcc) {
    // Create new fail address
    ErrBank = ErrorStatus.errBank & MRC_UINT16_MAX;
    NewFail.Addr.Bits.Row = ErrorStatus.errRow;
    NewFail.Addr.Bits.BankPair = ErrBank & ((1 << PprAmtData->BaseBits) - 1);
    NewFail.Addr.Bits.Rank = ErrorStatus.errRank;
    NewFail.Addr.Bits.Valid = 1;

    NewFail.BankGroupMask = (1 << (ErrBank >> PprAmtData->BaseBits));
    NewFail.DeviceTemp = DeviceTemp[Controller][Channel];
    NewFail.Device = ErrorStatus.device;
    NewFail.DqMask[0] = ErrorStatusLo;
    NewFail.DqMask[1] = ErrorStatusEcc;

    if (AmtUpdateRowFailList (MrcData, Controller, Channel, NewFail) != mrcSuccess) {
      Status = mrcFail;
    }
  } // DeviceTemp, ErrorStatusLo, ErrorStatusECC
  if (Status == mrcFail) {
    MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "UpdateRowFailures: FailRange list is Full!\n");
  }
  return Status;
}  //AmtUpdateRowFailures

/**
  Manually add entries into the row failure list for debugging purposes

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval none
**/
#if POISON_ROW_FAIL_LIST
VOID
AmtPoisonRowFailList (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData
)
{
  MRC_FUNCTION        *MrcCall;
  MrcOutput           *Outputs;
  UINT8               Controller;
  UINT8               Channel;
  UINT8               Index;
  ROW_FAIL_RANGE      FailEntry[25];
  UINT8               FailEntryCount = 3;
  UINT32              CurrRowFailMax;
  MrcCall = MrcData->Inputs.Call.Func;
  Outputs = &MrcData->Outputs;

  // If row fail list has already been poisoned during this memory test, do not poison again
  if (PprAmtData->HasRowFailListBeenPoisonedOnThisTest) {
    return;
  }

  PprAmtData->HasRowFailListBeenPoisonedOnThisTest = TRUE;

  MrcCall->MrcSetMem ((UINT8 *) &FailEntry, sizeof (FailEntry), 0);
  //
  // initialize FailEntries
  // 3 distinct errors, arbitrary banks and BG
  //

  // Entry 0
  FailEntry[0].Addr.Bits.Row = 0x6A;
  FailEntry[0].Addr.Bits.BankPair = 0x1;
  FailEntry[0].Addr.Bits.Rank = 0x0;
  FailEntry[0].Addr.Bits.Valid = 1;
  if (Outputs->IsDdr5) {
    FailEntry[0].BankGroupMask = 0x02; // BG index 1
  } else {
    FailEntry[0].BankGroupMask = 0x01; // BG index 0
  }
  //FailEntry[0].Addr.Data = 0x80C0003; // random

  // Entry 1
  FailEntry[1].Addr.Bits.Row = 0x1B3;
  FailEntry[1].Addr.Bits.BankPair = 0x0;
  FailEntry[1].Addr.Bits.Rank = 0x0;
  FailEntry[1].Addr.Bits.Valid = 1;
  if (Outputs->IsDdr5) {
    FailEntry[1].BankGroupMask = 0x40; // BG index 6
  } else {
    FailEntry[1].BankGroupMask = 0x01; // BG index 0
  }
  //FailEntry[1].Addr.Data = 0x80C0077;

  // Entry 2
  FailEntry[2].Addr.Bits.Row = 0x1CF;
  FailEntry[2].Addr.Bits.BankPair = 0x1;
  FailEntry[2].Addr.Bits.Rank = 0x0;
  FailEntry[2].Addr.Bits.Valid = 1;
  if (Outputs->IsDdr5) {
    FailEntry[2].BankGroupMask = 0x10; // BG index 4
  } else {
    FailEntry[2].BankGroupMask = 0x01; // BG index 0
  }
  //FailEntry[2].Addr.Data = 0x80C7D08;



  FailEntryCount = 3;
  // Update RowFailMax for Mc0,Ch0
  AmtSetRowFailListSize(MrcData, Outputs->FirstPopController, 0, FailEntryCount);

  // copy FailEntries to RowFailRange
  for (Index = 0; Index < FailEntryCount; Index++) {
    // assume all errors on Mc0,Ch0 unless Mc0 not populated
    MrcCall->MrcCopyMem ((UINT8 *) &MrcData->Outputs.FailRange[Outputs->FirstPopController][0][Index], (UINT8 *) &FailEntry[Index], sizeof (FailEntry[Index]));
    //MrcData->Outputs.FailRange[0][0][Index] = FailEntry[Index];
  }

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Poisoned FailEntry Array with following the following %d entries\n", FailEntryCount);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      CurrRowFailMax = AmtGetRowFailListSize(MrcData, Controller, Channel);
      if (CurrRowFailMax > 0) { // Check if exist row failures for controller/channel, if so loop through all fails for it
        for (Index = 0; Index < CurrRowFailMax; Index++) {
          MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "FailRange[%d][%d][%d].Addr.Bits.Row: 0x%x\n", Controller, Channel, Index, MrcData->Outputs.FailRange[Controller][Channel][Index].Addr.Bits.Row);
          MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "FailRange[%d][%d][%d].Addr.Bits.BankPair: 0x%x\n", Controller, Channel, Index, MrcData->Outputs.FailRange[Controller][Channel][Index].Addr.Bits.BankPair);
          MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "FailRange[%d][%d][%d].Addr.Bits.Rank: 0x%x\n", Controller, Channel, Index, MrcData->Outputs.FailRange[Controller][Channel][Index].Addr.Bits.Rank);
          MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "FailRange[%d][%d][%d].Addr.Data: 0x%x\n", Controller, Channel, Index, MrcData->Outputs.FailRange[Controller][Channel][Index].Addr.Data);
          MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "FailRange[%d][%d][%d].BankGroupMask: 0x%x\n", Controller, Channel, Index, MrcData->Outputs.FailRange[Controller][Channel][Index].BankGroupMask);
        }
      }
    } // Channel
  } // Controller
}

/**
  Loop through all controllers and channels to find and print the total row failures per controller, channel

  @param[in]  MrcData      - Pointer to global MRC data.

**/
VOID
AmtPrintTotalRowFailures (
  MrcParameters *const    MrcData
  )
{
  MrcOutput           *Outputs;
  UINT8               Channel;
  UINT8               MaxChDdr;
  UINT8               Controller;
  UINT8               RowFailureFound = 0;
  UINT32              TotalRowFailures = 0;
  UINT32              Index;
  ROW_FAIL_RANGE      *FailRangePtr;

  Outputs = &MrcData->Outputs;
  MaxChDdr = Outputs->MaxChannels;

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Summary of row failures:\n");
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if (MC_CH_MASK_CHECK (Outputs->McChBitMask, Controller, Channel, MaxChDdr) == 0 || AmtGetRowFailListSize(MrcData, Controller, Channel) == 0) {
        continue;
      }
      RowFailureFound = 1;
      TotalRowFailures = 0;
      for (Index = 0; Index < AmtGetRowFailListSize (MrcData, Controller, Channel); Index++) {
        FailRangePtr = AmtGetRowFailRange (MrcData, Controller, Channel, Index);
        if (FailRangePtr->Addr.Bits.Valid) {
          MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "  Row = 0x%x, FailRange[%d] = Data: 0x%x BgMask: 0x%x\n", FailRangePtr->Addr.Bits.Row, Index, FailRangePtr->Addr.Data, FailRangePtr->BankGroupMask);
          TotalRowFailures++;
        } else {
          MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "  Index %d not valid\n", Index);
        }
      }
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "  Mc%d.C%d: has %d row errors\n", Controller, Channel, TotalRowFailures);
    }
  }
  if (!RowFailureFound) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "  No row errors found!\n");
  }
}
#endif // POISON_ROW_FAIL_LIST

/**
  This implements the Advanced Memory test algorithm, to run over the specified Controller/Channels.

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure
**/
VOID
MrcAdvancedMemTest (
  IN MrcParameters            *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA   *const  PprAmtData
)
{
  MrcOutput         *Outputs;
  MRC_FUNCTION      *MrcCall;
  MrcDimmOut        *DimmOut;
  UINT8             LocalMcChBitMask;
  UINT8             Rank;
  UINT8             Channel;
  UINT8             Controller;
  UINT8             IpChannel;
  BOOLEAN           Is24GbDDR5[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8             RankMask;
  UINT32            MaxRow[MAX_CONTROLLER][MAX_IP_CHANNEL];
  UINT32            CurrentRow[MAX_CONTROLLER][MAX_IP_CHANNEL];

  PprAmtData->TestStatus = mrcSuccess;
  PprAmtData->FromRowTestPpr = FALSE;
  PprAmtData->Bank = 0;
  Outputs = &MrcData->Outputs;
  MrcCall = MrcData->Inputs.Call.Func;

  if (PprAmtData->UseStopOnError) {
    // Only support up direction
    PprAmtData->Direction = AMT_ADDR_DIR_UP;
  }

  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "MrcAdvancedMemTest: CmdPat=%d, IsUseInvtPat=%d, NumCacheLines=%d, Direction=%d\n",
    PprAmtData->CmdPat, PprAmtData->IsUseInvtPat, PprAmtData->NumCL, PprAmtData->Direction);

  MrcCall->MrcSetMem((UINT8 *)Is24GbDDR5, sizeof(Is24GbDDR5), 0);

  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    LocalMcChBitMask = 0; // Reset bitmask for new Rank
    if ((Outputs->ValidRankMask & (1 << Rank)) != 0) {
      MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "///// Testing Rank = %d /////\n", Rank);
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[RANK_TO_DIMM_NUMBER (Rank)];
        // Initialize per-Rank Column Row and Bank sizes
        // The MrcLog2 function returns +1 so we subtract 1
        PprAmtData->ColumnBits[Controller][Channel] = MrcLog2 (MrcData, DimmOut->ColumnSize) - 1;
        PprAmtData->RowBits[Controller][Channel] = MrcLog2 (MrcData, DimmOut->RowSize) - 1;
        PprAmtData->BankBits[Controller][Channel] = (MrcLog2 (MrcData, DimmOut->Banks) - 1) + (MrcLog2 (MrcData, DimmOut->BankGroups) - 1);

        if ((Outputs->IsDdr5) && (DimmOut->DensityIndex == MrcDensity24Gb)) {
          Is24GbDDR5[Controller][Channel] = TRUE;
        }

        RankMask = (1 << Rank);
        LocalMcChBitMask |= SelectReutRanks (MrcData, Controller, Channel, RankMask, FALSE);
      }
    }
    // Skip to next rank if no ranks enabled on any channel
    if (LocalMcChBitMask == 0) {
      continue;
    }

    PprAmtData->Rank = Rank;
    SetupIOTestRetention (MrcData, PprAmtData, LocalMcChBitMask, 1); // LC = 1

    // Set initial settings for starting test location
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (MC_CH_MASK_CHECK (LocalMcChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
          continue;
        }
        IpChannel = LP_IP_CH (Outputs->IsLpddr, Channel);
        CurrentRow[Controller][IpChannel] = 0;
        // Get max row bits
        MaxRow[Controller][IpChannel] = 0;
        // If controller/channel is not valid for this rank, leave MaxRow as 0
        if (PprAmtData->RowBits[Controller][Channel]) {
          MaxRow[Controller][IpChannel] = 1 << PprAmtData->RowBits[Controller][Channel];
        }
        if (Is24GbDDR5[Controller][Channel]) {
          // Row bits R[16:15] of 11b is invalid
          MaxRow[Controller][IpChannel] = MaxRow[Controller][IpChannel] / 4 * 3;
        }
      } //ch
    } // controller

    // TestSize = MaxRow because starting row is 0
    PprAmtData->TestStatus = AmtExecuteRowRangeTest (MrcData, PprAmtData, LocalMcChBitMask,
      CurrentRow, MaxRow);

    // Failure means that row fail range list is full, exit out of testing
    if (PprAmtData->TestStatus == mrcFail) {
      MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "AmtExecuteRowRangeTest returned failure - FailRange List Full!\n");
      return;
    }
#if AMT_PRINT_ROW_FAIL_LIST
    AmtDisplayRowFailureList (MrcData, Controller, Channel);
#endif
  } // rank

  return;
}

/**
  Determines the max row and column for this channel for stop-on-error tests

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure
  @param[in] Controller     - Controller number to update
  @param[in] Channel        - Channel number to update
**/
VOID
AmtGetMaxRowCol (
  MrcParameters *const              MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT32                         Controller,
  IN UINT32                         Channel,
  IN UINT32                         *MaxRow,
  IN UINT32                         *MaxCol
  )
{
  UINT32            IpChannel;

  IpChannel = LP_IP_CH(MrcData->Outputs.IsLpddr, Channel);

  if (MaxRow == NULL || MaxCol == NULL) {
    return;
  }

  *MaxRow = PprAmtData->TestRowAddr[Controller][IpChannel] + PprAmtData->TestRowSize[Controller][IpChannel];
  *MaxCol = TestDataEngineGetNumColumns (MrcData, Controller, Channel, PprAmtData->Rank);
}

/**
  Determines the row and column range of the next test if using stop-on-error.

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure
  @param[in] Controller     - Controller number to update
  @param[in] Channel        - Channel number to update
**/
VOID
AmtDetermineNextTestRange (
  MrcParameters* const              MrcData,
  IN PPR_AMT_PARAMETER_DATA* const  PprAmtData,
  IN UINT32                         Controller,
  IN UINT32                         Channel
  )
{
  MrcOutput         *Outputs;
  UINT32            IpChannel;
  BOOLEAN           IsYMarchAddrIncOrder;
  UINT32            NumReadsToFailure;
  UINT32            MaxRow;
  UINT32            MaxCol;
  UINT32            StoppedRow;

  Outputs = &MrcData->Outputs;
  IpChannel = LP_IP_CH(Outputs->IsLpddr, Channel);
  IsYMarchAddrIncOrder = (PprAmtData->TestType == PprTestTypeYMarchShort || PprAmtData->TestType == PprTestTypeYMarchLong);
  NumReadsToFailure = 0;
  AmtGetMaxRowCol (MrcData, PprAmtData, Controller, Channel, &MaxRow, &MaxCol);

  // Stop on error (MPTU)
  switch (PprAmtData->AmtTestState[Controller]) {
    case AmtInitialTest:
      PprAmtData->RowStart[Controller][IpChannel] = PprAmtData->TestRowAddr[Controller][IpChannel];
      PprAmtData->RowEnd[Controller][IpChannel] = MaxRow - 1;
      if (PprAmtData->ErrInjMask) {
        // Inject error on col 1
        PprAmtData->ColStart[Controller][IpChannel] = 1;
        PprAmtData->ColEnd[Controller][IpChannel] = PprAmtData->ColStart[Controller][IpChannel];
      } else {
        PprAmtData->ColStart[Controller][IpChannel] = 0;
        PprAmtData->ColEnd[Controller][IpChannel] = MaxCol - 1;
      }
      break;
    case AmtRemBankTest:
      if (IsYMarchAddrIncOrder) {
        // Finish the banks on the failing row
        if (PprAmtData->TestType == PprTestTypeYMarchLong) {
          NumReadsToFailure = PprAmtData->LRA[Controller].errBank + 1;
          if (PprAmtData->LRA[Controller].errRow - PprAmtData->TestRowAddr[Controller][IpChannel] < 2) {
            // First even or odd row
            PprAmtData->RowStart[Controller][IpChannel] = PprAmtData->LRA[Controller].errRow;
          } else {
            PprAmtData->RowStart[Controller][IpChannel] = PprAmtData->LRA[Controller].errRow - 2;
            NumReadsToFailure += (1 << PprAmtData->BankBits[Controller][Channel]);
          }
        } else {
          NumReadsToFailure = (PprAmtData->LRA[Controller].errRow - PprAmtData->TestRowAddr[Controller][IpChannel])
            * (1 << PprAmtData->BankBits[Controller][Channel]) + PprAmtData->LRA[Controller].errBank + 1;
          if (NumReadsToFailure <= MRC_UINT8_MAX) {
            PprAmtData->RowStart[Controller][IpChannel] = PprAmtData->TestRowAddr[Controller][IpChannel];
          } else {
            PprAmtData->RowStart[Controller][IpChannel] = PprAmtData->LRA[Controller].errRow - 7;
            NumReadsToFailure = 7 * (1 << PprAmtData->BankBits[Controller][Channel]) + PprAmtData->LRA[Controller].errBank + 1;
          }
        }
        // Single col
        PprAmtData->ColStart[Controller][IpChannel] = PprAmtData->LRA[Controller].errCol;
      } else {
        // Finish the banks on the failing column
        NumReadsToFailure = PprAmtData->LRA[Controller].errCol * (1 << PprAmtData->BankBits[Controller][Channel]) + PprAmtData->LRA[Controller].errBank + 1;
        if (NumReadsToFailure <= MRC_UINT8_MAX) {
          PprAmtData->ColStart[Controller][IpChannel] = 0;
        } else {
          PprAmtData->ColStart[Controller][IpChannel] = PprAmtData->LRA[Controller].errCol - 7;
          NumReadsToFailure = 7 * (1 << PprAmtData->BankBits[Controller][Channel]) + PprAmtData->LRA[Controller].errBank + 1;
        }
        // Single row
        PprAmtData->RowStart[Controller][IpChannel] = PprAmtData->LRA[Controller].errRow;
      }
      PprAmtData->RowEnd[Controller][IpChannel] = PprAmtData->LRA[Controller].errRow;
      PprAmtData->ColEnd[Controller][IpChannel] = PprAmtData->LRA[Controller].errCol;
      PprAmtData->NumIgnoreReads[Controller][Channel] = NumReadsToFailure;
      break;
    case AmtRem2ndOrderTest:
      if (IsYMarchAddrIncOrder) {
        // Finish the rows on the failing column
        PprAmtData->ColStart[Controller][IpChannel] = PprAmtData->LRA[Controller].errCol;
        PprAmtData->ColEnd[Controller][IpChannel] = PprAmtData->ColStart[Controller][IpChannel];
        PprAmtData->RowStart[Controller][IpChannel] = (INT64) PprAmtData->LRA[Controller].errRow + 1;
        PprAmtData->RowEnd[Controller][IpChannel] = MaxRow - 1;
      } else {
        // Finish the columns on the failing row
        PprAmtData->RowStart[Controller][IpChannel] = PprAmtData->LRA[Controller].errRow;
        PprAmtData->RowEnd[Controller][IpChannel] = PprAmtData->RowStart[Controller][IpChannel];
        PprAmtData->ColStart[Controller][IpChannel] = (INT64) PprAmtData->LRA[Controller].errCol + 1;
        PprAmtData->ColEnd[Controller][IpChannel] = MaxCol - 1;
      }
      PprAmtData->NumIgnoreReads[Controller][Channel] = 0;
      break;
    case AmtRem3rdOrderTest:
      if (IsYMarchAddrIncOrder) {
        // Finish columns
        PprAmtData->RowStart[Controller][IpChannel] = PprAmtData->TestRowAddr[Controller][IpChannel];
        PprAmtData->RowEnd[Controller][IpChannel] = MaxRow - 1;
        PprAmtData->ColStart[Controller][IpChannel] = (INT64) PprAmtData->LRA[Controller].errCol + 1;
        PprAmtData->ColEnd[Controller][IpChannel] = MaxCol - 1;
      } else {
        // Finish rows
        PprAmtData->ColStart[Controller][IpChannel] = 0;
        PprAmtData->ColEnd[Controller][IpChannel] = MaxCol - 1;
        PprAmtData->RowStart[Controller][IpChannel] = (INT64) PprAmtData->LRA[Controller].errRow + 1;
        PprAmtData->RowEnd[Controller][IpChannel] = MaxRow - 1;
      }
      PprAmtData->NumIgnoreReads[Controller][Channel] = 0;
      break;
    case AmtTestComplete:
      // Disable the channel in TempRankChEnMap by setting size to 0
      PprAmtData->TestRowSize[Controller][IpChannel] = 0;
      return;
    case AmtYMarchLongRemEvenRows:
      PprAmtData->ColStart[Controller][IpChannel] = PprAmtData->LRA[Controller].errCol;
      PprAmtData->ColEnd[Controller][IpChannel] = PprAmtData->ColStart[Controller][IpChannel];
      PprAmtData->RowStart[Controller][IpChannel] = (INT64) PprAmtData->LRA[Controller].errRow + 2;
      PprAmtData->RowEnd[Controller][IpChannel] = MaxRow - 2;
      PprAmtData->NumIgnoreReads[Controller][Channel] = 0;
      break;
    case AmtYMarchLongRemOddRows:
      if (PprAmtData->ErrorFound[Controller]) {
        StoppedRow = PprAmtData->LRA[Controller].errRow;
      } else {
        StoppedRow = (UINT32) PprAmtData->RowEnd[Controller][IpChannel];
      }
      PprAmtData->ColStart[Controller][IpChannel] = PprAmtData->LRA[Controller].errCol;
      PprAmtData->ColEnd[Controller][IpChannel] = PprAmtData->ColStart[Controller][IpChannel];
      if (StoppedRow == (UINT32) (MaxRow - 2)) {
        PprAmtData->RowStart[Controller][IpChannel] = (INT64) PprAmtData->TestRowAddr[Controller][IpChannel] + 1;
      } else {
        PprAmtData->RowStart[Controller][IpChannel] = (INT64) PprAmtData->LRA[Controller].errRow + 2;
      }
      PprAmtData->RowEnd[Controller][IpChannel] = MaxRow - 1;
      PprAmtData->NumIgnoreReads[Controller][Channel] = 0;
      break;
  }
  PprAmtData->ColumnBits[Controller][Channel] = MrcLog2(MrcData, (UINT32) (PprAmtData->ColEnd[Controller][IpChannel] -
    PprAmtData->ColStart[Controller][IpChannel] + 1)) - 1;
  PprAmtData->RowBits[Controller][Channel] = MrcLog2(MrcData, (UINT32) (PprAmtData->RowEnd[Controller][IpChannel] -
    PprAmtData->RowStart[Controller][IpChannel] + 1)) - 1;

  MRC_DEBUG_MSG (
    &Outputs->Debug,
    MSG_LEVEL_NOTE,
    "Mc%d.C%d: RowStart = 0x%08llx, RowEnd = 0x%08llx, ColStart = 0x%08llx, ColEnd = 0x%08llx, RowBits = %d, ColBits = %d\n",
    Controller,
    Channel,
    (UINT64) PprAmtData->RowStart[Controller][IpChannel],
    (UINT64) PprAmtData->RowEnd[Controller][IpChannel],
    (UINT64) PprAmtData->ColStart[Controller][IpChannel],
    (UINT64) PprAmtData->ColEnd[Controller][IpChannel],
    PprAmtData->RowBits[Controller][Channel],
    PprAmtData->ColumnBits[Controller][Channel]
  );
}

/**
  Determines the next AmtTestState for the current channel

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure
  @param[in] Controller     - Controller number to update
  @param[in] Channel        - Channel number to update

  @retval Whether this channel is done with its memory test
**/
BOOLEAN
AmtDetermineNextTestState (
  MrcParameters *const              MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT32                         Controller,
  IN UINT32                         Channel
  )
{
  MrcOutput       *Outputs;
  UINT32          IpChannel;
  BOOLEAN         IsLastBank;
  BOOLEAN         IsLastCol;
  BOOLEAN         IsLastRow;
  BOOLEAN         IsLast2ndOrder;
  BOOLEAN         IsLast3rdOrder;
  BOOLEAN         done;
  UINT32          MaxRow;
  UINT32          MaxCol;
  UINT32          StoppedRow; // Which row the last test stopped on
  BOOLEAN         IsLastEvenRow;  // For YMarchLong, if test stopped on the last even numbered row

  Outputs = &MrcData->Outputs;
  IpChannel = LP_IP_CH(Outputs->IsLpddr, Channel);
  done = 0;
  AmtGetMaxRowCol (MrcData, PprAmtData, Controller, Channel, &MaxRow, &MaxCol);

  if (PprAmtData->AmtTestState[Controller] == AmtTestComplete) {
    // Nothing to do for this controller
    return 1;
  }
  if (PprAmtData->ErrorFound[Controller]) {
    IsLastBank = (PprAmtData->LRA[Controller].errBank == (UINT32) ((1 << PprAmtData->BankBits[Controller][Channel]) - 1));
    IsLastCol = PprAmtData->LRA[Controller].errCol == (UINT32) (MaxCol - 1);
    IsLastRow = PprAmtData->LRA[Controller].errRow == (UINT32) (MaxRow - 1);
    StoppedRow = PprAmtData->LRA[Controller].errRow;
  } else {
    // No error found, either move to next range or mark test as complete for this controller
    if (PprAmtData->AmtTestState[Controller] == AmtInitialTest) {
      PprAmtData->AmtTestState[Controller] = AmtTestComplete;
      return 1;
    }
    IsLastBank = TRUE;
    IsLastCol = PprAmtData->ColEnd[Controller][IpChannel] == (UINT32) (MaxCol - 1);
    IsLastRow = PprAmtData->RowEnd[Controller][IpChannel] == (UINT32) (MaxRow - 1);
    StoppedRow = (UINT32) PprAmtData->RowEnd[Controller][IpChannel];
  }
  if (PprAmtData->TestType == PprTestTypeYMarchShort || PprAmtData->TestType == PprTestTypeYMarchLong) {
    IsLast2ndOrder = IsLastRow;
    IsLast3rdOrder = IsLastCol;
  } else {
    IsLast2ndOrder = IsLastCol;
    IsLast3rdOrder = IsLastRow;
  }
  if (!IsLastBank) {
    PprAmtData->AmtTestState[Controller] = AmtRemBankTest;
  } else if (!IsLast2ndOrder) {
    if (PprAmtData->TestType == PprTestTypeYMarchLong) {
      // If test stopped on last even row, then even rows are finished and test should move to odd rows
      IsLastEvenRow = (StoppedRow == (UINT32) (MaxRow - 2));
      PprAmtData->AmtTestState[Controller] = (IsLastEvenRow || ((StoppedRow - PprAmtData->TestRowAddr[Controller][IpChannel]) & MRC_BIT0)) ? AmtYMarchLongRemOddRows : AmtYMarchLongRemEvenRows;
    } else {
      PprAmtData->AmtTestState[Controller] = AmtRem2ndOrderTest;
    }
  } else if (!IsLast3rdOrder) {
    PprAmtData->AmtTestState[Controller] = AmtRem3rdOrderTest;
  } else {
    PprAmtData->AmtTestState[Controller] = AmtTestComplete;
    done = 1;
  }

  return done;
}

/**
  Executes a step of Advanced Memory test on given row address and size, and logs results.

  @param[in] MrcData                - Global MRC data structure
  @param[in] PprAmtData             - PPR and AMT data structure
  @param[in] McChBitMask            - Memory Controller Channel Bit mask to read results for.
  @param[in] BaseRow                - Row address to start test
  @param[in] RangeSize              - Row range size to test

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtExecuteRowRangeTest (
  MrcParameters   *const        MrcData,
  IN PPR_AMT_PARAMETER_DATA     *const  PprAmtData,
  UINT8                         McChBitMask,
  UINT32                        BaseRow[MAX_CONTROLLER][MAX_IP_CHANNEL],
  UINT32                        RangeSize[MAX_CONTROLLER][MAX_IP_CHANNEL]
  )
{
  MrcOutput       *Outputs;
  MRC_FUNCTION    *MrcCall;
  MrcStatus       Status;
  UINT8           Channel;
  UINT8           Controller;
  UINT8           TempRankChEnMap;
  UINT8           MaxChannels;
  UINT8           IpChannel;
  BOOLEAN         IsLpddr;
  UINT32          RemSize[MAX_CONTROLLER][MAX_IP_CHANNEL];
  INT8            i;
  UINT8           done;
  UINT8           LoopIterationTimeout;

  Outputs = &MrcData->Outputs;
  MrcCall = MrcData->Inputs.Call.Func;
  MaxChannels = Outputs->MaxChannels;
  TempRankChEnMap = McChBitMask;
  IsLpddr = Outputs->IsLpddr;
  Status = mrcSuccess;
  MrcCall->MrcSetMem ((UINT8 *)PprAmtData->TestRowSize, sizeof (PprAmtData->TestRowSize), 0);
  MrcCall->MrcSetMem ((UINT8 *)PprAmtData->TestRowAddr, sizeof (PprAmtData->TestRowAddr), 0);
  MrcCall->MrcSetMem ((UINT8 *)RemSize, sizeof (RemSize), 0);
  LoopIterationTimeout = 0;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    PprAmtData->AmtTestState[Controller] = AmtInitialTest;
    for (Channel = 0; Channel < MaxChannels; Channel++) {
      if ((MC_CH_MASK_CHECK(McChBitMask, Controller, Channel, MaxChannels) == 0) || (IS_MC_SUB_CH(IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH(IsLpddr, Channel);
      if (PprAmtData->Direction == AMT_ADDR_DIR_UP) {
        PprAmtData->TestRowAddr[Controller][IpChannel] = BaseRow[Controller][IpChannel];
      } else {
        PprAmtData->TestRowAddr[Controller][IpChannel] = BaseRow[Controller][IpChannel] + RangeSize[Controller][IpChannel];
      }
      RemSize[Controller][IpChannel] = RangeSize[Controller][IpChannel];
      if (RangeSize[Controller][IpChannel] == 0) {
        if (IsLpddr) {
          TempRankChEnMap &= ~(3 << ((Controller * MaxChannels) + 2 * IpChannel));
        } else {
          TempRankChEnMap &= ~(1 << ((Controller * MaxChannels) + Channel));
        }
      }
    }
  }

  do {
    LoopIterationTimeout++;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      PprAmtData->ErrorFound[Controller] = FALSE;
      for (Channel = 0; Channel < MaxChannels; Channel++) {
        if ((MC_CH_MASK_CHECK(TempRankChEnMap, Controller, Channel, MaxChannels) == 0)) {
          continue;
        }
        IpChannel = LP_IP_CH(IsLpddr, Channel);

        if (!PprAmtData->UseStopOnError) {
          PprAmtData->TestRowSize[Controller][IpChannel] = 0;
          PprAmtData->RowBits[Controller][Channel] = 0;

          for (i = 31; i >= 0; i--) {
            if (RemSize[Controller][IpChannel] & (1 << i)) {
              PprAmtData->TestRowSize[Controller][IpChannel] = (1 << i);
              PprAmtData->RowBits[Controller][Channel] = (UINT8)i;
              break;
            }
          }
        } else {
          PprAmtData->TestRowSize[Controller][IpChannel] = RangeSize[Controller][IpChannel];
          AmtDetermineNextTestRange (MrcData, PprAmtData, Controller, Channel);
        }

        if (PprAmtData->TestRowSize[Controller][IpChannel] == 0) {
          TempRankChEnMap &= ~(1 << ((Controller * MaxChannels) + Channel));  // Mask channel if nothing to test
        }
        if (!IS_MC_SUB_CH(IsLpddr, Channel) && PprAmtData->Direction == AMT_ADDR_DIR_DOWN) {
          PprAmtData->TestRowAddr[Controller][IpChannel] -= PprAmtData->TestRowSize[Controller][IpChannel];
        }
      }
    }

    MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "CmdPat = %d\n", PprAmtData->CmdPat);
    TempRankChEnMap = AdvancedMemTestRankSetupMATSRowRange(MrcData, PprAmtData, TempRankChEnMap);

    RunIOTest(MrcData, (UINT8)TempRankChEnMap, MRC_IGNORE_ARG_8, StaticPattern, 1, FALSE);

    // For READ test must check for failures
    if (PprAmtData->CmdPat == PatRdWr || PprAmtData->CmdPat == PatRd) {
      // Advanced Memtest error handling path
      Status = AmtCheckTestResults(MrcData, PprAmtData, TempRankChEnMap);
    } // if READ

    done = 1;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannels; Channel++) {
        if ((MC_CH_MASK_CHECK(McChBitMask, Controller, Channel, MaxChannels) == 0) || (IS_MC_SUB_CH(IsLpddr, Channel))) {
          continue;
        }
        IpChannel = LP_IP_CH(IsLpddr, Channel);
        if (!PprAmtData->UseStopOnError) {
          if (RemSize[Controller][IpChannel] > 0) {
            // Move to next 2^n range until done
            if (PprAmtData->Direction == AMT_ADDR_DIR_UP) {
              PprAmtData->TestRowAddr[Controller][IpChannel] += PprAmtData->TestRowSize[Controller][IpChannel];
              if (PprAmtData->TestRowAddr[Controller][IpChannel] < BaseRow[Controller][IpChannel] + RangeSize[Controller][IpChannel]) {
                done = 0;
              }
            } else {
              if (PprAmtData->TestRowAddr[Controller][IpChannel] > BaseRow[Controller][IpChannel]) {
                done = 0;
              }
            }
            RemSize[Controller][IpChannel] &= ~PprAmtData->TestRowSize[Controller][IpChannel];
          }
        } else {
          // Break out of the channel loop so this function only runs once per controller
          done = done && AmtDetermineNextTestState (MrcData, PprAmtData, Controller, Channel);
          break;
        }
      } // channel
    } // controller
    // LoopIterationTimeout keeps the test from infinitely looping
  } while (!done && LoopIterationTimeout < AMT_ROW_RANGE_TEST_MAX_LOOPS);

  if (LoopIterationTimeout >= AMT_ROW_RANGE_TEST_MAX_LOOPS) {
    MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "AmtExecuteRowRangeTest: Error - Loop exceeded maximum count %d\n", AMT_ROW_RANGE_TEST_MAX_LOOPS);
  }

  // If not FromRowTestPpr, failure means that row fail range list is full, exit out of testing
  return Status;
} //AmtExecuteRowRangeTest

/**
  Run write/read test on a row.

  @param[in] MrcData              - Global MRC data structure
  @param[in] PprAmtData           - PPR and AMT data structure
  @param[in] McChBitMask          - Memory Controller Channel Bit mask to read results for.
  @param[in] Row                  - Row address to run test

  @retval status - mrcSuccess if no row failures found, mrcFail if row failure found

**/
MrcStatus
RowTestPprWorker (
  MrcParameters *const            MrcData,
  PPR_AMT_PARAMETER_DATA  *const  PprAmtData,
  UINT8                           McChBitMask,
  UINT32                          Row
  )
{
  MrcStatus                   Status;
  MrcOutput                   *Outputs;
  MrcDimmOut                  *DimmOut;
  MRC_FUNCTION                *MrcCall;
  UINT32                      Channel;
  UINT32                      Controller;
  UINT8                       IpChannel;
  PprTestTypeOffset           TestTypeSave;
  UINT32                      BaseRow[MAX_CONTROLLER][MAX_IP_CHANNEL];
  UINT32                      RangeSize[MAX_CONTROLLER][MAX_IP_CHANNEL];
  Status = mrcSuccess;
  Outputs = &MrcData->Outputs;
  MrcCall = MrcData->Inputs.Call.Func;
  TestTypeSave = PprAmtData->TestType;
  PprAmtData->TestType = PprTestTypeWcMats8;

  MrcCall->MrcSetMem((UINT8 *) BaseRow, sizeof (BaseRow), 0);
  MrcCall->MrcSetMem((UINT8 *) RangeSize, sizeof (RangeSize), 0);
  MrcCall->MrcSetMem((UINT8 *) PprAmtData->RowBits, sizeof (PprAmtData->RowBits), 0); // Only 1 row in test
  MrcCall->MrcSetMem((UINT8 *) PprAmtData->BankBits, sizeof (PprAmtData->BankBits), 0); // Only 1 bank in test

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[RANK_TO_DIMM_NUMBER(PprAmtData->Rank)];
      IpChannel = (UINT8) LP_IP_CH (Outputs->IsLpddr, Channel);
      if (MC_CH_MASK_CHECK(McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
        continue;
      } else {
        BaseRow[Controller][IpChannel] = Row;
        RangeSize[Controller][IpChannel] = 1;
        // Initialize per-Rank Column Row and Bank sizes
        // The MrcLog2 function returns +1 so we subtract 1
        PprAmtData->ColumnBits[Controller][Channel] = MrcLog2 (MrcData, DimmOut->ColumnSize) - 1;
      }
    }
  }
  PprAmtData->CmdPat = PatWr;
  SetupIOTestRetention (MrcData, PprAmtData, McChBitMask, 1); // LC = 1

  AmtExecuteRowRangeTest (MrcData, PprAmtData, McChBitMask, BaseRow, RangeSize);

  PprAmtData->CmdPat = PatRd;
  Status = AmtExecuteRowRangeTest (MrcData, PprAmtData, McChBitMask, BaseRow, RangeSize);

  PprAmtData->TestType = TestTypeSave;

  return Status;
}

/**
  Executes a single-row write/read memory test with a simple data pattern.

  @param[in] MrcData              - Global MRC data structure
  @param[in] PprAmtData           - PPR and AMT data structure
  @param[in] McChBitMask          - Memory Controller Channel Bit mask to read results for.
  @param[in] Bank                 - Current bank address
  @param[in] Row                  - Row address to run test

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
RowTestPpr (
  IN MrcParameters  *const          MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT8                          McChBitMask,
  IN UINT32                         Bank,
  IN UINT32                         Row
  )
{
  MrcOutput                   *Outputs;
  MrcInput                    *Inputs;
  MrcStatus                   Status;

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  Status = mrcSuccess;
  PprAmtData->FromRowTestPpr = TRUE;
  PprAmtData->Bank = Bank;
  PprAmtData->DataPatternDepth = 1;
  PprAmtData->UiShl = 0;
  PprAmtData->Direction = AMT_ADDR_DIR_UP;
  PprAmtData->IsUseInvtPat = FALSE;
  PprAmtData->SubSeqDataInv[0] = 0;
  PprAmtData->SubSeqDataInv[1] = 1;
  // Some register setup logic checks if PatternNumber is 1 before running
  // We want all setup logic to run since disposition flow or PprCleanup might override some values
  PprAmtData->PatternNumber = 1;

  if ((McChBitMask == 0) ||
      (Outputs->IsLpddr && (Inputs->PprRepairType == sPPR)) ||
      (Inputs->PprRepairType == NoRepair)) {
    return mrcSuccess;
  }

  MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "\nPPR Row Test------------------------ START\n");

  // Test pattern all 1s
  PprAmtData->DataPattern[0] = 0xFFFFFFFFFFFFFFFF;
  Status = RowTestPprWorker(MrcData, PprAmtData, McChBitMask, Row);
  if (Status != mrcSuccess) {
    return Status;
  }

  MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "\n");

  // Test pattern all 0s
  PprAmtData->DataPattern[0] = 0x0000000000000000;
  Status = RowTestPprWorker(MrcData, PprAmtData, McChBitMask, Row);

  MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "PPR Row Test------------------------ END\n\n");

  return Status;
}
