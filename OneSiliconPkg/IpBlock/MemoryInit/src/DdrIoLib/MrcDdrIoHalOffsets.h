 /** @file
  This header exposes members of MrcDdrIoHalOffsets

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef MRC_DDRIO_OFFSETS_H_
#define MRC_DDRIO_OFFSETS_H_

/**
  This function returns the offset to access specific Channel/Strobe of DdrCrCmdBusTrain.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
CmdBusTrainOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
  );

/**
  This function returns the offset to access specific Channel/Strobe of Data's DCCCALCCONTROL.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataDccCalcCtlOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
  );

/**
  This function returns the offset to access specific Channel/Strobe of DataTrainFeedback.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataTrainFeedbackOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe,
  OUT UINT64_STRUCT *const VolatileMask
  );

/**
  This function returns the offset to access specific Channel/Rank/Strobe of DataDqs.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Rank    - 0-based index of Rank to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataDqsOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Rank,
  IN  UINT32  const Strobe,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  This function returns the offset to access specific Channel/Strobe of DllControl0.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DllControl0Offset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe,
  OUT UINT64_STRUCT *const VolatileMask
  );

/**
  This function returns the offset to access specific Channel/Strobe of DllStatus

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DllStatusOffset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  This function returns the offset to access specific Channel/Strobe of WRRETRAINDELTARANK.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
WrReTrainDeltaRankOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
  );

/**
  Function used to get the CR Offset for Retrain offset Groups.

  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Channel      - DDR Channel Number within the processor socket (0-based).
  @param[in]  Strobe       - Dqs data group within the rank (0-based).
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoReTrainOffsets (
  IN  GSM_GT               Group,
  IN  UINT32               Channel,
  IN  UINT32               Strobe,
  OUT UINT64_STRUCT *const VolatileMask
  );

#endif //
