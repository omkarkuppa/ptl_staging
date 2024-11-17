/** @file
  This file contains functions to get DDR IO Data Offsets
  used memory training.

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

#include "MrcDdrIoApi.h"
#include "CMcAddress.h"
#include "MrcHalRegisterAccess.h"
#include "MrcCommon.h"
#include "MrcDdrIoDefines.h"

/// Defines
// @todo: Write unit test to catch if these defines ever change from the expected values
// Generic constant values coming from a difference of any DDRDATAxCHy_Reg - DDRDATA_Base_Reg
// these constants are needed to get the correct DATAxCHy offset to access
// e.g. Where DATA0CH0_CR_DDRCRDATACONTROL0_REG is the DDRDATA_Base_Reg
//      DATA0CH1_CR_DDRCRDATACONTROL0_REG - DATA0CH0_CR_DDRCRDATACONTROL0_REG = 0x180
//      DATA1CH0_CR_DDRCRDATACONTROL0_REG - DATA0CH0_CR_DDRCRDATACONTROL0_REG = 0x300
#define INTRA_CH_OFFSET       0x180
#define INTRA_STRB_OFFSET     0x300

/// Structs
typedef struct {
  UINT8 Data;
  UINT8 Channel;
} MRC_DATA_CR_MAP;

/// Constants
// Sequential order of LPDDR Data[0:9] pairs of CR bytes.
static const MRC_DATA_CR_MAP DataLpCrMap[8][2] = {
//  {{Data_N, Channel_N}, {Data_N, Channel_N+1}}
    {{0, 0}, {0, 1}},  // Data0
    {{1, 0}, {1, 1}},  // Data1
    {{2, 0}, {2, 1}},  // Data2
    {{3, 0}, {3, 1}},  // Data3
    {{4, 0}, {4, 1}},  // Data4
    {{5, 0}, {5, 1}},  // Data5
    {{6, 0}, {6, 1}},  // Data6
    {{7, 0}, {7, 1}},  // Data7
};

// Sequential order of DDR Data[0:9] pairs of CR bytes.
static const MRC_DATA_CR_MAP DataDdrCrMap[4][5] = {
//  {{Data_N, Channel_N}, {Data_N, Channel_N+1}, {Data_N+1, Channel_N}, {Data_N+1, Channel_N+1}}
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {8, 0}},   //  Data0, Data1, Data8 - ECC
    {{2, 0}, {2, 1}, {3, 0}, {3, 1}, {8, 1}},   //  Data2, Data3, Data8 - ECC
    {{4, 0}, {4, 1}, {5, 0}, {5, 1}, {9, 0}},   //  Data4, Data5, Data9 - ECC
    {{6, 0}, {6, 1}, {7, 0}, {7, 1}, {9, 1}},   //  Data6, Data7, Data9 - ECC
};

///
/// Local Functions
///

/**
  This function returns the offset to access specific Channel/Strobe of DdrCrCmdBusTrain.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
CmdBusTrainOffset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
  )
{
  UINT32 Offset;

  Offset  = DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG;
  Offset += (DATA0CH1_CR_DDRCRCMDBUSTRAIN_REG - DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG) * Channel +
            (DATA1CH0_CR_DDRCRCMDBUSTRAIN_REG - DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG) * Strobe;

  return Offset;
}

/**
  This function returns the offset to access specific Channel/Strobe of DataTrainFeedback.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataTrainFeedbackOffset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32 Offset;

  Offset  = DATA0CH0_CR_DATATRAINFEEDBACK_REG;
  Offset += (DATA0CH1_CR_DATATRAINFEEDBACK_REG - DATA0CH0_CR_DATATRAINFEEDBACK_REG) * Channel +
            (DATA1CH0_CR_DATATRAINFEEDBACK_REG - DATA0CH0_CR_DATATRAINFEEDBACK_REG) * Strobe;
  VolatileMask->Data = DATA0CH0_CR_DATATRAINFEEDBACK_VOLATILE_BITFIELDS_MSK;

  return Offset;
}

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
  )
{
  UINT32 Offset;

  Offset = OFFSET_CALC_MC_CH (
    DATA0CH0_CR_DCCCALCCONTROL_REG, DATA0CH1_CR_DCCCALCCONTROL_REG, Channel,
    DATA1CH0_CR_DCCCALCCONTROL_REG, Strobe
  );

  return Offset;
}

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
  )
{
  UINT32 Offset;

  Offset = OFFSET_CALC_MC_CH (
    DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG, DATA0CH1_CR_DDRCRWRRETRAINDELTARANK_REG, Channel,
    DATA1CH0_CR_DDRCRWRRETRAINDELTARANK_REG, Strobe
  );

  return Offset;
}

/**
  This function returns the offset to access specific Channel/Strobe of Data's DCCFSMCONTROL.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataDccFsmCtlOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
  )
{
  UINT32 Offset;

  Offset = OFFSET_CALC_MC_CH (
    DATA0CH0_CR_DCCFSMCONTROL_REG, DATA0CH1_CR_DCCFSMCONTROL_REG, Channel,
    DATA1CH0_CR_DCCFSMCONTROL_REG, Strobe
  );

  return Offset;
}

/**
  This function returns the offset for the specific
  Channel/Strobe of any register in the DATA partition.

  @param[in]  RegOffset  - 0-based RegOffset base address.
  @param[in]  Controller - 0-based index of Controller access.
  @param[in]  Channel    - 0-based index of Channel to access.
  @param[in]  Strobe     - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
MrcGetDataOffset (
  IN  MrcParameters   *MrcData,
  IN  UINT32  const   RegOffset,
  IN  UINT32  const   Controller,
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
  )
{
  const MRC_DATA_CR_MAP *PartitionInst;
  UINT32    Offset;
  UINT32    TransChannel;
  MrcOutput *Outputs;

  Outputs  = &MrcData->Outputs;

  if ((Controller != MRC_IGNORE_ARG) && (Controller < MAX_CONTROLLER)) {
    TransChannel = ((Controller * Outputs->MaxChannels) + Channel);
    PartitionInst = Outputs->IsDdr5 ? &DataDdrCrMap[TransChannel][Strobe] : &DataLpCrMap[TransChannel][Strobe];
    Offset = RegOffset + (PartitionInst->Channel * INTRA_CH_OFFSET) + (PartitionInst->Data * INTRA_STRB_OFFSET);
  } else {
    Offset = RegOffset + (Channel * INTRA_CH_OFFSET) + (Strobe * INTRA_STRB_OFFSET);
  }

  return Offset;
}

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
  )
{
  UINT32 Offset;

  Offset  = DATA0CH0_CR_DDRCRDLLSTATUS_REG;
  Offset += (DATA0CH1_CR_DDRCRDLLSTATUS_REG - DATA0CH0_CR_DDRCRDLLSTATUS_REG) * Channel +
            (DATA1CH0_CR_DDRCRDLLSTATUS_REG - DATA0CH0_CR_DDRCRDLLSTATUS_REG) * Strobe;
  VolatileMask->Data = DATA0CH0_CR_DDRCRDLLSTATUS_VOLATILE_BITFIELDS_MSK;

  return Offset;
}

/**
  This function returns the offset to access specific Channel/Strobe of DllControl0.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DllControl0Offset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32 Offset;

  Offset  = DATA0CH0_CR_DDRCRDLLCONTROL0_REG;
  Offset += (DATA0CH1_CR_DDRCRDLLCONTROL0_REG - DATA0CH0_CR_DDRCRDLLCONTROL0_REG) * Channel +
            (DATA1CH0_CR_DDRCRDLLCONTROL0_REG - DATA0CH0_CR_DDRCRDLLCONTROL0_REG) * Strobe;
  VolatileMask->Data = DATA0CH0_CR_DDRCRDLLCONTROL0_VOLATILE_BITFIELDS_MSK;

  return Offset;
}

/**
  This function returns the offset to access specific Channel/Strobe of DataControl0.

  @param[in]  Channel     - 0-based index of Channel to access.
  @param[in]  Strobe      - 0-based index of Strobe to access.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataControl0Offset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32 Offset;

  Offset  = DATA0CH0_CR_DDRCRDATACONTROL0_REG;
  Offset += (DATA0CH1_CR_DDRCRDATACONTROL0_REG - DATA0CH0_CR_DDRCRDATACONTROL0_REG) * Channel +
            (DATA1CH0_CR_DDRCRDATACONTROL0_REG - DATA0CH0_CR_DDRCRDATACONTROL0_REG) * Strobe;
  VolatileMask->Data = DATA0CH0_CR_DDRCRDATACONTROL0_VOLATILE_BITFIELDS_MSK;

  return Offset;
}

/**
  This function returns the offset to access specific Channel/Strobe of DataControl1.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataControl1Offset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
  )
{
  UINT32 Offset;

  Offset  = DATA0CH0_CR_DDRCRDATACONTROL1_REG;
  Offset += (DATA0CH1_CR_DDRCRDATACONTROL1_REG - DATA0CH0_CR_DDRCRDATACONTROL1_REG) * Channel +
            (DATA1CH0_CR_DDRCRDATACONTROL1_REG - DATA0CH0_CR_DDRCRDATACONTROL1_REG) * Strobe;

  return Offset;
}

/**
  This function returns the offset to access specific Channel/Strobe of DataControl2.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataControl2Offset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
  )
{
  UINT32 Offset;

  Offset  = DATA0CH0_CR_DDRCRDATACONTROL2_REG;
  Offset += (DATA0CH1_CR_DDRCRDATACONTROL2_REG - DATA0CH0_CR_DDRCRDATACONTROL2_REG) * Channel +
            (DATA1CH0_CR_DDRCRDATACONTROL2_REG - DATA0CH0_CR_DDRCRDATACONTROL2_REG) * Strobe;

  return Offset;
}

/**
  This function returns the offset to access specific Channel/Strobe of DataControl5.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataControl5Offset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
  )
{
  UINT32 Offset;

  Offset  = DATA0CH0_CR_DDRCRDATACONTROL5_REG;
  Offset += (DATA0CH1_CR_DDRCRDATACONTROL5_REG - DATA0CH0_CR_DDRCRDATACONTROL5_REG) * Channel +
            (DATA1CH0_CR_DDRCRDATACONTROL5_REG - DATA0CH0_CR_DDRCRDATACONTROL5_REG) * Strobe;

  return Offset;
}

/**
  This function returns the offset to access specific DATA_SHAREDx of DataControl6.

  @param[in]  Index - 0-based index of DATA_SHAREDx to access.

  @returns the offset of the CR
**/
UINT32
DataControl6Offset (
  IN  UINT32  const   Index
  )
{
  UINT32 Offset;

  Offset = DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_REG;
  Offset += (DDRDATA_SHARED1_CR_DDRCRDATACONTROL6_REG - DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_REG) * Index;

  return Offset;
}

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
  )
{
  UINT32 Offset;

  Offset  = DATA0CH0_CR_DDRDATADQSRANK0_REG;
  Offset += (DATA0CH1_CR_DDRDATADQSRANK0_REG - DATA0CH0_CR_DDRDATADQSRANK0_REG) * Channel +
            (DATA0CH0_CR_DDRDATADQSRANK1_REG - DATA0CH0_CR_DDRDATADQSRANK0_REG) * Rank +
            (DATA1CH0_CR_DDRDATADQSRANK0_REG - DATA0CH0_CR_DDRDATADQSRANK0_REG) * Strobe;

  VolatileMask->Data = DATA0CH0_CR_DDRDATADQSRANK0_VOLATILE_BITFIELDS_MSK;

  return Offset;
}

/**
  This function returns the offset to access specific Channel/Rank/Strobe/Bit of DataDqRankXLaneY.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Rank    - 0-based index of Rank to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.
  @param[in]  Bit     - 0-based index of Bit to access.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register.

  @return UINT32 - CR offset
**/
UINT32
DataDqRankXLaneYOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Rank,
  IN  UINT32  const Strobe,
  IN  UINT32  const Bit,
  OUT UINT64_STRUCT *const VolatileMask
  )
{
  UINT32 Offset;

  if (Channel >= MAX_CHANNEL) {
    // Lane 8 (DBI) offset is not consecutive with lane [0..7], so it needs to be handled separately
    if (Bit == DBI_BIT) {
      Offset = DATA_CR_DDRDATADQRANK0LANE8_REG;
      Offset += (DATA_CR_DDRDATADQRANK1LANE8_REG - DATA_CR_DDRDATADQRANK0LANE8_REG) * Rank;
    } else {
    // Overall Broadcast
      Offset = DATA_CR_DDRDATADQRANK0LANE0_REG;
      Offset += (DATA_CR_DDRDATADQRANK1LANE0_REG - DATA_CR_DDRDATADQRANK0LANE0_REG) * Rank +
                (DATA_CR_DDRDATADQRANK0LANE1_REG - DATA_CR_DDRDATADQRANK0LANE0_REG) * Bit;
    }
  } else {
    // Lane 8 (DBI) offset is not consecutive with lane [0..7], so it needs to be handled separately
    if (Bit == DBI_BIT) {
      Offset = DATA0CH0_CR_DDRDATADQRANK0LANE8_REG;
      Offset += (DATA0CH1_CR_DDRDATADQRANK0LANE8_REG - DATA0CH0_CR_DDRDATADQRANK0LANE8_REG) * Channel +
                (DATA0CH0_CR_DDRDATADQRANK1LANE8_REG - DATA0CH0_CR_DDRDATADQRANK0LANE8_REG) * Rank +
                (DATA1CH0_CR_DDRDATADQRANK0LANE8_REG - DATA0CH0_CR_DDRDATADQRANK0LANE8_REG) * Strobe;
    } else {
      Offset = DATA0CH0_CR_DDRDATADQRANK0LANE0_REG;
      Offset += (DATA0CH1_CR_DDRDATADQRANK0LANE0_REG - DATA0CH0_CR_DDRDATADQRANK0LANE0_REG) * Channel +
        (DATA0CH0_CR_DDRDATADQRANK1LANE0_REG - DATA0CH0_CR_DDRDATADQRANK0LANE0_REG) * Rank +
        (DATA1CH0_CR_DDRDATADQRANK0LANE0_REG - DATA0CH0_CR_DDRDATADQRANK0LANE0_REG) * Strobe +
        (DATA0CH0_CR_DDRDATADQRANK0LANE1_REG - DATA0CH0_CR_DDRDATADQRANK0LANE0_REG) * Bit;
    }
  }
  VolatileMask->Data = DATA0CH0_CR_DDRDATADQRANK0LANE0_VOLATILE_BITFIELDS_MSK;

  return Offset;
}

/**
  Function used to get the CR Offset for registers listed under PGx/VCCCLKx/PLL/DATA_SHAREDx/CCC_SHAREDx partitions.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Set          - Set index being targeted
  @param[in]  SubGroup     - SubGroup being targeted
  @param[in]  Lane         - Lane index within the data group (0-based).
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoPartitionOffsets (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Set,
  IN  UINT32                SubGroup,
  IN  UINT32                Lane,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32  Offset;

  Offset = MRC_UINT32_MAX;

  switch (SubGroup) {
    case PartitionPll:
      Offset = GetDdrIoPllOffsets (Group, Set, VolatileMask);
      break;

    case PartitionPg:
      Offset = GetDdrIoPgOffsets (MrcData, Group, Lane, VolatileMask);
      break;

    case PartitionComp:
      Offset = GetDdrIoCompOffsets (MrcData, Group, Lane, FreqIndex, VolatileMask);
      break;

    case PartitionDataShared:
      Offset = GetDdrIoDataOffsets (MrcData, Group, Set, Lane, FreqIndex, VolatileMask);
      break;

    case PartitionCccShared:
      Offset = GetDdrIoCccOffsets (MrcData, Group, Set, Lane, FreqIndex, VolatileMask);
      break;

    default:
      break;
  }

  return Offset;
}

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
  IN  GSM_GT                Group,
  IN  UINT32                Channel,
  IN  UINT32                Strobe,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32 Offset;

  Offset = MRC_UINT32_MAX;

  switch (Group) {
    case WrRetrainDeltaPiCode:
      Offset = DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG +
        ((DATA0CH1_CR_DDRCRWRRETRAINDELTARANK_REG - DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG) * Channel) +
        ((DATA1CH0_CR_DDRCRWRRETRAINDELTARANK_REG - DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG) * Strobe);
      break;

    case RdRetrainDeltaPiCode:
      Offset = DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG +
        ((DATA0CH1_CR_DDRCRRDRETRAINDELTARANK_REG - DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG) * Channel) +
        ((DATA1CH0_CR_DDRCRRDRETRAINDELTARANK_REG - DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG) * Strobe);
      break;

    default:
      break;
  }
  VolatileMask->Data = DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_VOLATILE_BITFIELDS_MSK;

  return Offset;
}

// These functions are called in DdrioInit()
/**
  This function returns the offset to access specific Channel/Strobe of Cbt.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
CbtOffset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
  )
{
  UINT32 Offset;

  if (Channel >= MAX_CHANNEL) {
    // Overall Broadcast
    Offset = DATA_CR_DDRCRCMDBUSTRAIN_REG;
  } else {
    Offset = DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG;
    Offset += (DATA0CH1_CR_DDRCRCMDBUSTRAIN_REG - DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG) * Channel +
      (DATA1CH0_CR_DDRCRCMDBUSTRAIN_REG - DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG) * Strobe;
  }
  return Offset;
}

/**
  This function returns the offset to access specific Channel/Strobe of WRRETRAINSWIZZLECONTROL.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
WrRetrainSwizzleControlOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
  )
{
  UINT32 Offset;

  Offset = DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG +
         ((DATA0CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG - DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG) * Channel) +
         ((DATA1CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG - DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG) * Strobe);

  return Offset;
}

/**
  This function returns the offset to access specific Channel/Strobe of WrRetrainControlStatus.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
WrRetrainControlStatusOffset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
  )
{
  UINT32 Offset;

  Offset = DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG;
  Offset += (DATA0CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_REG - DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG) * Channel +
         (DATA1CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG - DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG) * Strobe;

  return Offset;
}

/**
  This function returns the offset to access specific Index and SaGv Point for DDRCRVCCDDQCONTROL_Target

  @param[in]  Index - DDRDATA DDRCRVCCDDQCONTROL SBMEM Index
  @param[in]  FreqIndex  - SaGvIndex
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataVccDdqTargetOffset (
  IN  UINT32                Index,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  )
{
   return 0; //Not VccDdq for DATA partition
}

/**
  This function returns the offset to access specific Index for DDRCRLVRAutoTrim

  @param[in]  Index - DATA DDRCRLVRAutoTrim SBMEM Index
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataLvrAutoTrimOffset (
  IN  UINT32                Index,
  OUT UINT64_STRUCT  *const VolatileMask
  )
{
  UINT32 Offset;

  Offset = DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG;
  Offset += (DDRDATA_SBMEM1_CR_DDRCRLVRAUTOTRIM_REG - DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG) * Index;
  VolatileMask->Data = DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_VOLATILE_BITFIELDS_MSK;
  return Offset;
}

/**
  This function returns the offset to access specific Index and SaGv Point for DDRCRVCCIOGCONTROL_Target

  @param[in]  Index - DDRDATA DDRCRVCCIOGCONTROL SBMEM Index
  @param[in]  FreqIndex  - SaGvIndex
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataVccIogTargetOffset(
  IN  UINT32                Index,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  )
{
   return 0; //No DDRCRVCCIOGCONTROL in Data partition
}

/**
  This function returns the offset to access specific Index and SaGv Point for DDRCRVCCDDQCONTROL_Target

  @param[in]  Index - CCC DDRCRVCCDDQCONTROL SBMEM Index
  @param[in]  FreqIndex  - SaGvIndex
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
CccVccDdqTargetOffset (
  IN  UINT32                Index,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  )
{
    return 0; //No DDRCRVCCIOGCONTROL in CCC partition
}

/**
  This function returns the offset to access specific Index for DDRCRLvrAutoTrim

  @param[in]  Index - CCC DDRCRLVRAutoTrim SBMEM Index
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
CccLvrAutoTrimOffset (
  IN  UINT32                Index,
  OUT UINT64_STRUCT  *const VolatileMask
  )
{
  UINT32 Offset;

  Offset = DDRCCC_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG;
  Offset += (DDRCCC_SBMEM1_CR_DDRCRLVRAUTOTRIM_REG - DDRCCC_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG) * Index;
  VolatileMask->Data = DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_VOLATILE_BITFIELDS_MSK;
  return Offset;
}

/**
  This function returns the offset to access specific Index and SaGv Point for DDRCRVCCIOGCONTROL_Target

  @param[in]  Index - CCC DDRCRVCCIOGCONTROL SBMEM Index
  @param[in]  FreqIndex  - SaGvIndex
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
CccVccIogTargetOffset (
  IN  UINT32                Index,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  )
{
   return 0; //No DDRCRVCCIOGCONTROL in CCC partition
}

/**
  This function returns the offset to access specific Channel/Lane of CCC Lane.

  @param[in] Channel      - 0-based index of Channel to access.
  @param[in]  Lane         - Lane index
  @param[out] VolatileMask - Volatile Mask

  @returns the offset of the CR.
**/
UINT32
GetDdrIoCccLaneOffsets (
  IN  UINT32                  Channel,
  IN  UINT32                  Lane,
  OUT UINT64_STRUCT  *const   VolatileMask
  )
{
  UINT32 Offset;

  Offset = OFFSET_CALC_CH (CH0CCC_CR_DDRCRCCCLANE0_REG, CH1CCC_CR_DDRCRCCCLANE0_REG, Channel);
  Offset += (CH0CCC_CR_DDRCRCCCLANE1_REG - CH0CCC_CR_DDRCRCCCLANE0_REG) * Lane;
  VolatileMask->Data = CH0CCC_CR_DDRCRCCCLANE0_VOLATILE_BITFIELDS_MSK;

  return Offset;
}

/**
  This function returns the offset to access specific Channel/Lane of PGVTT CCC Lane.

  @param[in] Channel      - 0-based index of Channel to access.
  @param[out] VolatileMask - Volatile Mask

  @returns the offset of the CR.
**/
UINT32
GetDdrIoVttLaneOffsets (
  IN  UINT32                  Channel,
  OUT UINT64_STRUCT  *const   VolatileMask
  )
{
  UINT32 Offset;

  Offset = OFFSET_CALC_CH (DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_REG, DDRDATA_SHARED1_CR_DDRCRCCCCH0LANE0_REG, Channel);
  //Offset += (DDRDATA_SHARED0_CR_DDRCRCCCCH1LANE0_REG - DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_REG) * (Channel % 2);
  VolatileMask->Data = DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_VOLATILE_BITFIELDS_MSK;

  return Offset;
}
