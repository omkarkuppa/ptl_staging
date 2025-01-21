/** @file
  This file implements functions for setting up the Pattern
  Generators for CPGC 2.0.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#include "CMcAddress.h"
#include "MrcCpgcOffsets.h"
#include "MrcCommon.h"
#include "MrcDebugPrint.h"
#include "MrcCpgcApi.h"
#include "Cpgc20.h"
#include "Cpgc20Patterns.h"

/**
  This function configures the rotation rate for the PG (per Outputs->McChBitMask).
  If the RotateRate is 0, the hardware rotation is disabled.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  RotateRate    - Log2 count of how many transactions (SubSequence or Cacheline) before we rotate the PG.

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0..
**/
MrcStatus
Cpgc20ConfigPgRotation (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                RotateRate
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Offset;
  UINT8     MaxChannel;
  UINT8     McChMask;
  MC0_CH0_CR_CPGC_DPAT_CFG_STRUCT CpgcDPatLaneRotate;

  Outputs    = &MrcData->Outputs;
  McChMask   = Outputs->McChBitMask;
  MaxChannel = Outputs->MaxChannels;

  if (RotateRate > MC0_CH0_CR_CPGC_DPAT_CFG_LANE_ROTATE_RATE_MAX) {
    MRC_CPGC_DEBUG_MSG (
      &Outputs->Debug,
      MSG_LEVEL_WARNING,
      "%sRotateRate %d exceeds max count %d\n",
      gWarnString,
      RotateRate,
      MC0_CH0_CR_CPGC_DPAT_CFG_LANE_ROTATE_RATE_MAX
      );
    RotateRate = MC0_CH0_CR_CPGC_DPAT_CFG_LANE_ROTATE_RATE_MAX;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      Offset = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_DPAT_CFG_REG,
                MC1_CH0_CR_CPGC_DPAT_CFG_REG, Controller,
                MC0_CH1_CR_CPGC_DPAT_CFG_REG, IpChannel);
      CpgcDPatLaneRotate.Data = MrcReadCR (MrcData, Offset);
      if (CpgcDPatLaneRotate.Bits.LANE_ROTATE_RATE != RotateRate) {
        CpgcDPatLaneRotate.Bits.LANE_ROTATE_RATE = RotateRate;
        MrcWriteCR (MrcData, Offset, CpgcDPatLaneRotate.Data);
      }
    }
  }
  return (McChMask == 0) ? mrcFail : mrcSuccess;
}



/**
  This function sets up the CPGC data pattern configuration per Outputs->McChBitMask.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  PatCtlPtr   - Structure that stores start, Stop, IncRate and Dqpat for pattern.
  @param[in]  Index       - Which USQ to program pattern to

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20DPatUsqCfg (
  IN  MrcParameters *const   MrcData,
  IN  MRC_PATTERN_CTL *const PatCtlPtr,
  IN  UINT8                  Index
  )
{
  MrcOutput                     *Outputs;
  UINT8                         McChMask;
  UINT8                         MaxChannel;
  UINT8                         Controller;
  UINT8                         IpChannel;
  UINT8                         Channel;
  UINT32                        Offset;
  MRC_PG_UNISEQ_TYPE            UniseqMode;

  MC0_CH0_CR_CPGC_DPAT_USQ_CFG_0_STRUCT     CpgcDpatUsqCfg;
  MC0_CH0_CR_CPGC_DPAT_UNISEQ_POLY_0_STRUCT CpgcDpatUniSeqPoly; //Galois Poly

  Outputs    = &MrcData->Outputs;
  McChMask   = Outputs->McChBitMask;
  MaxChannel = Outputs->MaxChannels;

  if (PatCtlPtr->DQPat == StaticPattern) {
    UniseqMode = MrcPgMuxPatBuffer;
  } else if (PatCtlPtr->DQPat == LMNVa) {
    UniseqMode = MrcPgMuxLmn;
  } else if (PatCtlPtr->DQPat == GaloisMprVa) {
    UniseqMode = MrcPgMuxLfsrGalois;
  } else {
    UniseqMode = MrcPgMuxLfsr;
  }
  CpgcDpatUsqCfg.Data = 0;
  CpgcDpatUsqCfg.Bits.UNISEQ_MODE = UniseqMode;
  CpgcDpatUniSeqPoly.Data = 0;

  if (PatCtlPtr->DQPat == GaloisMprVa) {
    CpgcDpatUniSeqPoly.Bits.GALOIS_POLY = 0xB8; // Galois Polynomial x^8+x^6+x^5+x^4+1
    CpgcDpatUsqCfg.Bits.UNISEQ_LFSR_POLYNOMIAL_SIZE = 0; // When in GaloisMprVa mode, LFSR Polynomial won't be used.
  } else {
    CpgcDpatUsqCfg.Bits.UNISEQ_LFSR_POLYNOMIAL_SIZE = MrcLfsr16;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      Offset = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_DPAT_USQ_CFG_0_REG,
                MC1_CH0_CR_CPGC_DPAT_USQ_CFG_0_REG, Controller,
                MC0_CH1_CR_CPGC_DPAT_USQ_CFG_0_REG, IpChannel);
      Offset += (MC0_CH0_CR_CPGC_DPAT_USQ_CFG_1_REG - MC0_CH0_CR_CPGC_DPAT_USQ_CFG_0_REG) * Index;
      MrcWriteCR8 (MrcData, Offset, CpgcDpatUsqCfg.Data);

      if (PatCtlPtr->DQPat == GaloisMprVa) {
        Offset = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_DPAT_UNISEQ_POLY_0_REG,
                MC1_CH0_CR_CPGC_DPAT_UNISEQ_POLY_0_REG, Controller,
                MC0_CH1_CR_CPGC_DPAT_UNISEQ_POLY_0_REG, IpChannel);
        Offset += (MC0_CH0_CR_CPGC_DPAT_UNISEQ_POLY_1_REG - MC0_CH0_CR_CPGC_DPAT_UNISEQ_POLY_0_REG) * Index;
        MrcWriteCR (MrcData, Offset, CpgcDpatUniSeqPoly.Data);
      }
    }
  }
  return (McChMask == 0) ? mrcFail : mrcSuccess;
}

/**
  This function sets up the CPGC LFSR configuration per Outputs->McChBitMask.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  LaneRotate  - Lane Rotate Rate, should be log2(CPGC2_BASE_REPEATS.Base_Repeats+1) for BasicVA

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20LfsrCfg (
  IN  MrcParameters *const   MrcData,
  IN  UINT8                  LaneRotate
  )
{
  MrcOutput                               *Outputs;
  UINT8                                   Controller;
  UINT8                                   Channel;
  UINT8                                   IpChannel;
  UINT8                                   MaxChannel;
  UINT8                                   McChMask;
  UINT32                                  Offset;
  MC0_CH0_CR_CPGC_DPAT_CFG_STRUCT         CpgcDpatCfg;

  Outputs          = &MrcData->Outputs;
  MaxChannel       = Outputs->MaxChannels;
  McChMask         = Outputs->McChBitMask;
  CpgcDpatCfg.Data = 0;
  CpgcDpatCfg.Bits.LANE_ROTATE_RATE = LaneRotate;

  if (!Outputs->EccSupport) {
    CpgcDpatCfg.Bits.ECC_DISABLE = 1;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      Offset = OFFSET_CALC_MC_CH (
        MC0_CH0_CR_CPGC_DPAT_CFG_REG,
        MC1_CH0_CR_CPGC_DPAT_CFG_REG, Controller,
        MC0_CH1_CR_CPGC_DPAT_CFG_REG, IpChannel);
      MrcWriteCR (MrcData, Offset, CpgcDpatCfg.Data);
    }
  }

  return (McChMask == 0) ? mrcFail : mrcSuccess;
}

/**
  This function program the CPGC PGs Mux Seeds by setting up the CPGC LFSR configuration per Outputs->McChBitMask.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  Seeds        - Seed to program

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20LfsrSeed (
  IN  MrcParameters *const MrcData,
  IN  const UINT32  *const Seeds
  )
{
  MrcOutput                             *Outputs;
  UINT32                                Offset;
  UINT8                                 McChMask;
  UINT8                                 Index;
  UINT8                                 MaxChannel;
  UINT8                                 Controller;
  UINT8                                 Channel;
  UINT8                                 IpChannel;
  BOOLEAN                               IsDdr5;
  MC0_CH0_CR_CPGC_DPAT_UNISEQ_0_STRUCT  CpgcLfsrSeed;

  Outputs           = &MrcData->Outputs;
  McChMask          = Outputs->McChBitMask;
  MaxChannel        = Outputs->MaxChannels;
  IsDdr5            = Outputs->IsDdr5;

  for (Index = 0; Index < MRC_NUM_MUX_SEEDS; Index++) {
    CpgcLfsrSeed.Data = Seeds[Index];
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
          continue;
        }
        IpChannel = DDR5_IP_CH (IsDdr5, Channel);
        Offset = OFFSET_CALC_MC_CH (
          MC0_CH0_CR_CPGC_DPAT_UNISEQ_0_REG,
          MC1_CH0_CR_CPGC_DPAT_UNISEQ_0_REG, Controller,
          MC0_CH1_CR_CPGC_DPAT_UNISEQ_0_REG, IpChannel);
        Offset += (MC0_CH0_CR_CPGC_DPAT_UNISEQ_1_REG - MC0_CH0_CR_CPGC_DPAT_UNISEQ_0_REG) * Index;
        MrcWriteCR (MrcData, Offset, CpgcLfsrSeed.Data);
      }
    }
  }
  return (McChMask == 0) ? mrcFail : mrcSuccess;
}

/**
  This function sets the Data and ECC Lane Mask Enables (per Outputs->McChBitMask).
  If all bits in DataLaneMask and EccLaneMask are 0, then the DC/Invert feature is disabled.

  @param[in]  MrcData       - Pointer to global MRC data.
  @param[in]  DataLaneMask  - Bit-Mask of lanes to enable in this PG.
  @param[in]  EccLaneMask   - Bit-Mask of lanes to enable for ECC in this PG.

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20SetPgInvDcEn (
  IN  MrcParameters *const  MrcData,
  IN  UINT64                DataLaneMask,
  IN  UINT8                 EccLaneMask
  )
{
  MrcOutput     *Outputs;
  UINT32        Controller;
  UINT32        Channel;
  UINT32        IpChannel;
  UINT8         MaxChannel;
  UINT8         McChMask;
  UINT32        Offset;
  UINT64_STRUCT DataLaneMsk;
  MC0_CH0_CR_CPGC_DPAT_INVDC_CFG_STRUCT       CpgcDPatInvDcCfg;
  MC0_CH0_CR_CPGC_DPAT_INV_DC_MASK_LO_STRUCT  CpgcDpatInvMaskLowDWord;
  MC0_CH0_CR_CPGC_DPAT_INV_DC_MASK_HI_STRUCT  CpgcDpatInvMaskHighDWord;

  Outputs          = &MrcData->Outputs;
  McChMask         = Outputs->McChBitMask;
  MaxChannel       = Outputs->MaxChannels;
  DataLaneMsk.Data = DataLaneMask;
  CpgcDpatInvMaskLowDWord.Data = DataLaneMsk.Data32.Low;
  CpgcDpatInvMaskHighDWord.Data = DataLaneMsk.Data32.High;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      Offset = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_DPAT_INVDC_CFG_REG,
                MC1_CH0_CR_CPGC_DPAT_INVDC_CFG_REG, Controller,
                MC0_CH1_CR_CPGC_DPAT_INVDC_CFG_REG, IpChannel);
      CpgcDPatInvDcCfg.Data = MrcReadCR (MrcData, Offset);
      if (CpgcDPatInvDcCfg.Bits.ECC_INV_DC_MASK != EccLaneMask) {
        CpgcDPatInvDcCfg.Bits.ECC_INV_DC_MASK = EccLaneMask;
        MrcWriteCR (MrcData, Offset, CpgcDPatInvDcCfg.Data);
      }

      Offset = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_DPAT_INV_DC_MASK_LO_REG,
                MC1_CH0_CR_CPGC_DPAT_INV_DC_MASK_LO_REG, Controller,
                MC0_CH1_CR_CPGC_DPAT_INV_DC_MASK_LO_REG, IpChannel);
      MrcWriteCR (MrcData, Offset, CpgcDpatInvMaskLowDWord.Data);

      Offset = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_DPAT_INV_DC_MASK_HI_REG,
                MC1_CH0_CR_CPGC_DPAT_INV_DC_MASK_HI_REG, Controller,
                MC0_CH1_CR_CPGC_DPAT_INV_DC_MASK_HI_REG, IpChannel);
      MrcWriteCR (MrcData, Offset, CpgcDpatInvMaskHighDWord.Data);
    }
  }
  return (McChMask == 0) ? mrcFail : mrcSuccess;
}

/**
  This function sets the Pattern Generators' Invert/DC configuration (per Outputs->McChBitMask).
  The invert mode inverts the selected lanes' values.  The DC mode sets the
  selected lanes to a DC value.

  When rotation is enabled, once the rotation rate is met, the LaneMask is rotated from
  LSb to MSb.  Bit 63 of data is rotated into bit 0 of ECC, and bit 7 of ECC is rotated
  to Bit 0 of data.

  @param[in]  MrcData             - Pointer to global MRC data.
  @param[in]  Mode                - Selects Invert (0) or DC mode (1).
  @param[in]  DcPolarity          - 0 = DC low. 1 = DC high.
  @param[in]  EnableRotate        - Enables the rotation feature for the Bit Enable Masks of ECC and Data.
  @param[in]  RotateRateExponent  - Programs the counter to rotate every 2^(RotateRateExponenet).
  @param[in]  BytegroupMapping    - Selects between 2 bytes of DPAT pattern generator to be applied to each DQ byte group.  MSB is the ECC byte group.

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20SetPgInvDcCfg (
  IN  MrcParameters *const  MrcData,
  IN  CPGC20_INVERT_DC_MODE Mode,
  IN  UINT8                 DcPolarity,
  IN  BOOLEAN               EnableRotate,
  IN  UINT8                 RotateRateExponent,
  IN  UINT16                BytegroupMapping
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Offset;
  UINT8     MaxChannel;
  UINT8     McChMask;
  BOOLEAN   IsDdr5;
  MC0_CH0_CR_CPGC_DPAT_INVDC_CFG_STRUCT CpgcDPatInvDcCfg;
  MC0_CH0_CR_CPGC_DPAT_INVDC_CFG_STRUCT CpgcDPatInvDcCfgOrig;

  Outputs               = &MrcData->Outputs;
  McChMask              = Outputs->McChBitMask;
  MaxChannel            = Outputs->MaxChannels;
  IsDdr5                = Outputs->IsDdr5;
  CpgcDPatInvDcCfg.Data = 0;
  CpgcDPatInvDcCfg.Bits.DC_OR_INV         = Mode;
  CpgcDPatInvDcCfg.Bits.DC_POL            = DcPolarity;
  CpgcDPatInvDcCfg.Bits.MASK_ROTATE_EN    = EnableRotate;
  CpgcDPatInvDcCfg.Bits.MASK_ROTATE_RATE  = RotateRateExponent;
  CpgcDPatInvDcCfg.Bits.BYTEGROUP_MAPPING = MIN (BytegroupMapping, MC0_CH0_CR_CPGC_DPAT_INVDC_CFG_BYTEGROUP_MAPPING_MAX);
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (IsDdr5, Channel);
      Offset = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_DPAT_INVDC_CFG_REG,
                MC1_CH0_CR_CPGC_DPAT_INVDC_CFG_REG, Controller,
                MC0_CH1_CR_CPGC_DPAT_INVDC_CFG_REG, IpChannel);
      CpgcDPatInvDcCfgOrig.Data = MrcReadCR (MrcData, Offset);
      CpgcDPatInvDcCfg.Bits.ECC_INV_DC_MASK = CpgcDPatInvDcCfgOrig.Bits.ECC_INV_DC_MASK;

      if (CpgcDPatInvDcCfg.Data != CpgcDPatInvDcCfgOrig.Data) {
        MrcWriteCR (MrcData, Offset, CpgcDPatInvDcCfg.Data);
      }
    }
  }
  return (McChMask == 0) ? mrcFail : mrcSuccess;
}

/**
  This function sets the Pattern Generators' Alternate Dpat Buffer Control. Useful for static pattern setup.

  @param[in]  MrcData             - Pointer to global MRC data.
  @param[in]  IncScale            - Exponential/Linear
  @param[in]  IncRate             - Pattern Generator's "lines" advancement rate
  @param[in]  Start               - Lane to start from
  @param[in]  Stop                - Lane to wrap ("stop") on

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20SetDpatAltBufCtl (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                IncScale,
  IN  UINT16                IncRate,
  IN  UINT8                 Start,
  IN  UINT8                 Stop
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Offset;
  UINT8     McChBitMask;
  UINT8     MaxChannel;
  MC0_CH0_CR_CPGC_DPAT_ALT_BUF_CFG_STRUCT CpgcDpatAltBufCfg;

  Outputs     = &MrcData->Outputs;
  McChBitMask = Outputs->McChBitMask;
  MaxChannel  = Outputs->MaxChannels;
  CpgcDpatAltBufCfg.Data = 0;
  CpgcDpatAltBufCfg.Bits.BUF_PNTR_INC_SCALE = IncScale;
  CpgcDpatAltBufCfg.Bits.BUF_PNTR_INC_RATE  = IncRate;
  CpgcDpatAltBufCfg.Bits.BUF_STRT_PNTR      = Start;
  CpgcDpatAltBufCfg.Bits.BUF_END_PNTR       = Stop;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannel) == 0)) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      Offset = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_DPAT_ALT_BUF_CFG_REG,
                MC1_CH0_CR_CPGC_DPAT_ALT_BUF_CFG_REG, Controller,
                MC0_CH1_CR_CPGC_DPAT_ALT_BUF_CFG_REG, IpChannel);
      MrcWriteCR (MrcData, Offset, CpgcDpatAltBufCfg.Data);
    }
  }
  return mrcSuccess;
}

/**
  This function sets the Pattern Generators' Dpat Buffer Control. Useful for static pattern setup.

  @param[in]  MrcData             - Pointer to global MRC data.
  @param[in]  IncScale            - Exponential/Linear
  @param[in]  IncRate             - Pattern Generator's "lines" advancement rate
  @param[in]  Start               - Lane to start from
  @param[in]  Stop                - Lane to wrap ("stop") on

  @retval - MrcStatus: mrcSuccess if McChBitMask are non-zero, mrcFail if McChBitMask = 0.
**/
MrcStatus
Cpgc20SetDpatBufCtl (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                IncScale,
  IN  UINT16                IncRate,
  IN  UINT8                 Start,
  IN  UINT8                 Stop
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Offset;
  UINT8     McChBitMask;
  UINT8     MaxChannel;
  MC0_CH0_CR_CPGC_DPAT_ALT_BUF_CFG_STRUCT CpgcDpatAltBufCfg;

  Outputs     = &MrcData->Outputs;
  McChBitMask = Outputs->McChBitMask;
  MaxChannel  = Outputs->MaxChannels;
  CpgcDpatAltBufCfg.Data = 0;
  CpgcDpatAltBufCfg.Bits.BUF_PNTR_INC_SCALE = IncScale;
  CpgcDpatAltBufCfg.Bits.BUF_PNTR_INC_RATE  = IncRate;
  CpgcDpatAltBufCfg.Bits.BUF_STRT_PNTR      = Start;
  CpgcDpatAltBufCfg.Bits.BUF_END_PNTR       = Stop;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannel) == 0)) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
      Offset = OFFSET_CALC_MC_CH (
                MC0_CH0_CR_CPGC_DPAT_ALT_BUF_CFG_REG,
                MC1_CH0_CR_CPGC_DPAT_ALT_BUF_CFG_REG, Controller,
                MC0_CH1_CR_CPGC_DPAT_ALT_BUF_CFG_REG, IpChannel);
      MrcWriteCR (MrcData, Offset, CpgcDpatAltBufCfg.Data);
    }
  }
  return (McChBitMask == 0) ? mrcFail : mrcSuccess;
}

/**
  This routine fills an array with the CPGC pattern generators EXT_BUF values,
  the values depends on whether its driving victim LFSR or aggressor LFSR.
  This routine performs the following steps:
  Step 0: Iterate through all CPGC DPAT External Buffers
  Step 1: Use the vmask and amask parameters to determine victim and agressor bytes
  Step 2: Create a compressed vector for a given 32 byte cacheline
          Victim bytes have a value of AA, and Aggressor bytes have a value of CC
          If both are slected bytes have a value of C0, and if neither the value of C0

  @param[in] vmask       - 32 bit victim mask.  1 indicates this bit should use LFSR0. Mask length is 16 bits (CPGC_20_NUM_DPAT_EXTBUF)
  @param[in] amask       - 32 bit aggressor mask. 0/1 indicates this bit should use LFSR1/2. Mask length is 16 bits (CPGC_20_NUM_DPAT_EXTBUF)
  @param[out] Pattern    - 32 bit pattern array to be programed to CPGC_20_NUM_DPAT_EXTBUF

  @retval Nothing
**/
VOID
Cpgc20FillVAPattern (
  IN UINT32               vmask,
  IN UINT32               amask,
  OUT UINT32              Pattern[CPGC_20_NUM_DPAT_EXTBUF]
  )
{
  static const UINT32 VaPatVector[4] = { 0xAAAAAAAA, 0xC0C0C0C0, 0xCCCCCCCC, 0xF0F0F0F0 };
  UINT32             Bit;
  UINT32             Vic;
  UINT32             Agg2;
  UINT32             BitMask;
  UINT32             Index;

  // AA = Victim (LFSR0), 0C = Agg1(LFSR1), 0xCC = Agg2 (LFSR2), 0xF0 = Agg3 (LFSR3 - Temporarily used for LP5)
  for (Bit = 0; Bit < CPGC_20_NUM_DPAT_EXTBUF; Bit++) {
    BitMask = MRC_BIT0 << Bit;
    Vic = (vmask & BitMask);
    Agg2 = (amask & BitMask);
    // Select the vector for this lane
    if (Vic && Agg2) {
      Index = 1;
    } else if (Vic && !Agg2) {
      Index = 0;
    } else if (!Vic && !Agg2) {
      Index = 3;
    } else {
      Index = 2;
    }
    Pattern[Bit] = VaPatVector[Index];
  }
}

/**
  Program CPGC_DPAT_EXTBUF in a specified index, with a specified pattern.

  @param[in] MrcData     - Include all MRC global data.
  @param[in] McChBitMask - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in] Pattern  - Array of a given pattern.

  @retval Nothing
**/
VOID
Cpgc20SetDqAssignemnt (
  IN MrcParameters *const  MrcData,
  IN UINT8                 McChMask,
  IN UINT32                Pattern[CPGC_20_NUM_DPAT_EXTBUF]
  )
{
  MrcOutput *Outputs;
  UINT32    Offset;
  UINT32    IpCh;
  UINT32    Channel;
  UINT32    Controller;
  UINT32    Index;
  BOOLEAN   IsLpddr;

  Outputs = &MrcData->Outputs;
  IsLpddr = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, Outputs->MaxChannels) == 0) ||
        (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpCh = LP_IP_CH (IsLpddr, Channel);
      for (Index = 0; Index < CPGC_20_NUM_DPAT_EXTBUF; Index++) {
        // Program ExtBuf
        Offset = OFFSET_CALC3 (
          MC0_BUF0_CR_CPGC_DPAT_EXTBUF_0_REG,
          MC1_BUF0_CR_CPGC_DPAT_EXTBUF_0_REG, Controller,
          MC0_BUF1_CR_CPGC_DPAT_EXTBUF_0_REG, IpCh,
          MC0_BUF0_CR_CPGC_DPAT_EXTBUF_1_REG, Index);

        MrcWriteCR (MrcData, Offset, Pattern[Index]);
      }
    }
  }
}

/**
  Fill the test engine with the given fixed pattern.
  In Enhanced Channel Mode only entry [0] of each chunk will be used (low 32 bit).

  @param[in] MrcData  - Include all MRC global data.
  @param[in] Pattern  - Array of chunks (64 bit), broken into two 32-bit entries (second dimension).
  Length (first dimension) can be up to NUM_DPAT_EXTBUF
  @retval Nothing
**/
VOID
Cpgc20WriteMATSFixedPattern (
  IN MrcParameters *const  MrcData,
  IN UINT32                Pattern[]
  )
{
  UINT32    *ExtBufPtr;
  MrcOutput *Outputs;
  UINT32    Offset;
  UINT32    IpCh;
  UINT32    Channel;
  UINT32    Controller;
  UINT32    Index;
  UINT8     McChMask;
  BOOLEAN   Lpddr;

  Outputs   = &MrcData->Outputs;
  McChMask  = Outputs->McChBitMask;
  Lpddr     = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, Outputs->MaxChannels) == 0) ||
        (IS_MC_SUB_CH (Lpddr, Channel))) {
        continue;
      }
      IpCh = LP_IP_CH (Lpddr, Channel);
      // Program ExtBuf
      Offset = OFFSET_CALC_MC_CH (
        MC0_BUF0_CR_CPGC_DPAT_EXTBUF_0_REG,
        MC1_BUF0_CR_CPGC_DPAT_EXTBUF_0_REG, Controller,
        MC0_BUF1_CR_CPGC_DPAT_EXTBUF_0_REG, IpCh);
      for (Index = 0; Index < CPGC_20_NUM_DPAT_EXTBUF; Index++) {
        ExtBufPtr = &Pattern[Index];
        MrcWriteCR (MrcData, Offset, *ExtBufPtr);
        //Adjust Offset for next EXTBUF
        Offset += (MC0_BUF0_CR_CPGC_DPAT_EXTBUF_1_REG - MC0_BUF0_CR_CPGC_DPAT_EXTBUF_0_REG);
      }
    }
  }
  return;
}

