/** @file
  Prototype information for private/internal functions used in MrcChipApi.c


  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

*/


#ifndef MRC_CHIP_API_PRIVATE_H_
#define MRC_CHIP_API_PRIVATE_H_

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcChipApiPrivate)

/**
  Use MRS Burst FSM to program all relevant MR registers in one quick operation.
  Can only be used on ULT/ULX (up to 2 ranks, no ECC).
  Runs on all populated controllers, channels, and ranks.

  @param[in]  MrcData - Include all MRC global data.

  @retval mrcSuccess    - MRS FSM was executed successfully
  @retval mrcDeviceBusy - timed out waiting for MRH or MRS FSM
**/
MrcStatus
WrappedMrcProgramMrsFsm (
  IN MrcParameters *const MrcData
  );


#endif //  MRC_CHIP_API_PRIVATE_H_

