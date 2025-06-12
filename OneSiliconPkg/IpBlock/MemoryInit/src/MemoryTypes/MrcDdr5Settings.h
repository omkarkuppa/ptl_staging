/** @file
  This API provides an interface to abstract out memory protocol differences.

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
#ifndef MRC_DDR5_SETTINGS_H_
#define MRC_DDR5_SETTINGS_H_

#include "CMrcApi.h"  // Indirectly provides "IN" and "OUT"

// AUTO-GENERATED CARD ENUM AND TABLE DECLARATIONS START
// AUTO-GENERATED CODE
typedef enum {
    Card_207G = 0,
    Card_240C = 1,
    Card_241G = 2,
    Card_246C = 3,
    Card_256B = 4,
    Card_269C = 5,
    Card_279C = 6,
    Card_default = 7,
    Card_Max
} CardEnum;

typedef enum {
    Card_default_1R_6400 = 0,
    Card_default_1R_7200 = 1,
    Card_default_2R_6400 = 2,
    Card_default_2R_7200 = 3,
    Card_240C_1R_6400 = 4,
    Card_246C_1R_6400 = 5,
    Card_246C_1R_7200 = 6,
    Card_269C_1R_6400 = 7,
    Card_279C_1R_6400 = 8,
    Card_279C_1R_7200 = 9,
    Card_207G_2R_6400 = 10,
    Card_241G_2R_6400 = 11,
    Card_256B_2R_6400 = 12,
    Card_256B_2R_7200 = 13,
} Ddr5ParamIndex;

extern const char* CardPnArray[];

typedef struct {
    INT8 ddr5_dfe_tap1;
    INT8 ddr5_dfe_tap2;
    INT8 ddr5_rtt_wr;
    INT8 ddr5_rtt_nom_wr;
    INT8 ddr5_rtt_park_rx;
    INT8 ddr5_ron_up;
    INT8 ddr5_ron_dn;
} Ddr5InitailSettings;

typedef struct {
    INT8 cpu_ron;
    INT8 dq_tco;
    INT8 dq_odt;
    INT8 rxtap0;
    INT8 rxtap1;
    INT8 ctle_r;
    INT8 ctle_c;
    INT8 ctle_rcmn;
    INT8 ctle_dccmn;
    INT8 ctle_eq;
    INT8 ctle_tailctl;
} Ddr5PhyInitailSettings;

extern const Ddr5InitailSettings Ddr5InitailSettingsParams[];

extern const Ddr5PhyInitailSettings Ddr5PhyParams[];

Ddr5ParamIndex GetDdr5ParamIndex (
    MrcFrequency Frequency,
    UINT32 NumOfRanks,
    CardEnum Card
);

// AUTO-GENERATED CARD ENUM AND TABLE DECLARATIONS END

/**
  This function returns index in ODT table according to to DIMM/rank population.

  @param[in] MrcData         - Include all the MRC general data.
  @param[in] Controller      - selected Controller.
  @param[in] Channel         - selected Channel.

  @retval TOdtIndex - Index in ODT table according to to DIMM/rank population.
**/
TOdtIndex
GetOdtIndexDdr5 (
  IN MrcParameters* const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  );

/**
  This function selects the DQ ODT table according to DIMM/rank population.

  @param[in] MrcData         - Include all the MRC general data.
  @param[in] Profile         - Memory Profile.
  @param[in] Dimm            - selected DIMM.
  @param[in] OdtIndex        - selected ODT index.

  @retval TOdtValueDqDdr5 * - Pointer to the relevant table or NULL if the table was not found.
**/
TOdtValueDqDdr5 *
SelectTableDdr5 (
  IN MrcParameters *const MrcData,
  IN const MrcProfile     Profile,
  IN const UINT32         Dimm,
  IN const TOdtIndex      OdtIndex
  );

/**
  This function selects the CCC ODT table according to the Rank population.

  @param[in] MrcData         - Include all the MRC general data.
  @param[in] Profile         - Memory Profile.
  @param[in] Controller      - selected Controller.
  @param[in] Channel         - selected Channel.
  @param[in] Dimm            - selected Dimm.

  @retval TOdtValueCccDdr5 * - Pointer to the relevant table or NULL if the table was not found.
**/
TOdtValueCccDdr5 *
SelectCccTableDdr5 (
  IN MrcParameters *const MrcData,
  IN const MrcProfile     Profile,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Dimm
  );

/**
  This function selects the DFE table according to DIMM/Rank population.

  @param[in] MrcData         - Include all the MRC general data.
  @param[in] Profile         - Memory Profile.
  @param[in] Controller      - selected Controller.
  @param[in] Channel         - selected Channel.
  @param[in] Dimm            - selected Dimm.

  @return Pointer to the relevant table or NULL if the table was not found.
**/
TDFEValueDdr5 *
SelectDfeTableDdr5 (
  IN MrcParameters *const MrcData,
  IN const MrcProfile     Profile,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Dimm
  );

#endif // MRC_DDR5_SETTINGS_H_
