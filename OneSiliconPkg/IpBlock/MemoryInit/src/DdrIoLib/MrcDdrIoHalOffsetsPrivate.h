 /** @file
  This header exposes private members of MrcDdrIoHalOffsets such that they
  can be used in Unit Testing

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

#ifndef MRC_DDRIO_OFFSETS_PRIVATE_H_
#define MRC_DDRIO_OFFSETS_PRIVATE_H_

#include "MrcDdrIoApi.h"

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcDdrIoHalOffsetsPrivate)

// Generic constant values coming from a difference of any DDRDATAxCHy_Reg - DDRDATA_Base_Reg
// these constants are needed to get the correct DATAxCHy offset to access
// e.g. Where DATA0CH0_CR_DDRCRDATACONTROL0_REG is the DDRDATA_Base_Reg
//      DATA0CH1_CR_DDRCRDATACONTROL0_REG - DATA0CH0_CR_DDRCRDATACONTROL0_REG = 0x180
//      DATA1CH0_CR_DDRCRDATACONTROL0_REG - DATA0CH0_CR_DDRCRDATACONTROL0_REG = 0x300
#define INTRA_CH_OFFSET       0x180
#define INTRA_STRB_OFFSET     0x300

/**
  This function returns the offset to access specific Channel/Strobe of Data's DCCFSMSTATUS.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR.
**/
UINT32
DataDccFsmStatusOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
  );

/**
  This function returns the offset to access specific Channel/Strobe of DataRxEqTap.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR.
**/
UINT32
DataRxEqTapOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
  );

/**
  Function used to get the CR Offset for DATATCOCONTROL0/1.

  @param[in]  Channel - DDR Channel Number within the processor socket (0-based).
  @param[in]  Strobe  - DQS data group within the channel (0-based).
  @param[in]  Bit     - Bit within the strobe (0-based).

  @returns the offset of the CR.
**/
UINT32
GetDdrIoDataTcoControl(
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe,
  IN  UINT32  const Bit
  );

/**
  Function used to get the CR Offset for Data Read Groups.

  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Controller   - Memory controller in the processor socket (0-based).
  @param[in]  Channel      - DDR Channel Number within the processor socket (0-based).
  @param[in]  Rank         - Rank index in the channel. (0-based).
  @param[in]  Strobe       - Dqs data group within the rank (0-based).
  @param[in]  Bit          - Bit index within the data group (0-based).
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoDataReadOffsets (
  IN  MrcParameters        *MrcData,
  IN  GSM_GT               Group,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Rank,
  IN  UINT32               Strobe,
  IN  UINT32               Bit,
  OUT UINT64_STRUCT *const VolatileMask
  );

#endif // MRC_DDRIO_OFFSETS_PRIVATE_H_
