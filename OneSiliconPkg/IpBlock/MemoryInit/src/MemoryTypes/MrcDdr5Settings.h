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
#include "MrcDdrCommon.h"
#include "MrcDdr5.h"

// AUTO-GENERATED DDR5 TABLE DECLARATIONS START
// AUTO-GENERATED CODE
#define DDR5_WRONG_OPT_VAL (-1000)
#define DDR5_NUM_OF_OPTS (7)

typedef enum {
    Card_230C = 0,
    Card_234B = 1,
    Card_235A = 2,
    Card_240C = 3,
    Card_256B = 4,
    Card_269C = 5,
    Card_270B = 6,
    Card_276C = 7,
    Card_298C = 8,
    Card_311B = 9,
    Card_324B = 10,
    Card_75B = 11,
    Card_Hynix1R = 12,
    Card_Hynix2R = 13,
    Card_Samsung2R = 14,
    Card_default = 15,
    Card_Max
} CardEnum;

typedef enum {
  Card_240C_1R_6400 = 0,
  Card_256B_2R_6400 = 1,
  Card_256B_2R_7200 = 2,
  Card_269C_1R_6400 = 3,
  Card_276C_1R_6400 = 4,
  Card_276C_1R_7200 = 5,
  Card_298C_1R_6400 = 6,
  Card_298C_1R_7200 = 7,
  Card_311B_2R_7200 = 8,
  Card_311B_2R_6400 = 9,
  Card_324B_2R_6400 = 10,
  Card_324B_2R_7200 = 11,
  Card_230C_1R_6400 = 12,
  Card_234B_2R_7200 = 13,
  Card_235A_1R_6400 = 14,
  Card_75B_2R_6400 = 15,
  Card_270B_2R_6400 = 16,
  Card_Samsung2R_2R_6400 = 17,
  Card_Samsung2R_2R_7200 = 18,
  Card_Hynix1R_1R_7200 = 19,
  Card_Hynix2R_2R_7200 = 20,
  Card_default_1R_6400 = 21,
  Card_default_1R_7200 = 22,
  Card_default_2R_6400 = 23,
  Card_default_2R_7200 = 24,
  Card_NotFound
} Ddr5ParamIndex;

typedef struct {
  INT8    DfeTap1;
  INT8    DfeTap2;
  UINT16  RttWr;
  UINT16  RttNomWr;
  UINT16  RttNomRd;
  UINT16  RonUp;
  UINT16  RonDn;
} NnFlexDdr5Params;

extern const char* CardPartNumber[];
extern const char* Ddr5OptParamNames[];
extern const char* Ddr5CardToName[];
extern const NnFlexDdr5Params NnFlexInitialSettingsDdr5[];
/**
  This function returns index NN Flex csv according to to Dimm/Rank/Freq.
  @param[in] Frequency   - Frequency config.
  @param[in] NumOfRanks  - Rank config - 1/2.
  @param[in] Card        - Dimm module card id.
  @returns corresponding card row index in NN Flex csv to struct table. default if not exists.
**/
Ddr5ParamIndex
GetDdr5ParamIndex (
  IN MrcFrequency Frequency,
  IN UINT32 NumOfRanks,
  IN CardEnum Card
  );
// AUTO-GENERATED DDR5 TABLE DECLARATIONS END

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

/**
  This function is used to get the corresponding card for a given dram part info.

  @param[in]  ModulePartNumber - Dram module part number from SPD.
  @param[in]  NumOfRanks       - Rank config - 1/2.
  @param[in]  ManufactorIdCode - Dram manufacture id code from SPD.

  @returns - The corresponding card index.
**/
CardEnum
Ddr5GetCardEnum (
  IN const CHAR8* ModulePartNumber,
  IN const UINT32 NumOfRanks,
  IN const UINT8  ManufactorIdCode
  );

/**
  Remove whitespaces from a module part number string.

  @param[in]      ModulePartNumber      - Source module part number string from SPD.
  @param[in, out] PartNumberNoSpaces    - Destination buffer for part number without spaces (allocated by caller)
**/
VOID
RemoveWhitespacesFromPartNumber (
  IN     const CHAR8* ModulePartNumber,
  IN OUT       CHAR8* PartNumberNoSpaces
  );

/**
  This function checks if a substring matches the beginning of a module part number.

  @param[in]  ModulePartNumber - DIMM module part number from SPD.
  @param[in]  Prefix           - String to check if it's a prefix of the module part number.

  @retval TRUE  - Prefix matches the beginning of ModulePartNumber.
  @retval FALSE - Prefix does not match or invalid parameters.
**/
BOOLEAN
Ddr5IsModulePartPrefix (
  IN const CHAR8* ModulePartNumber,
  IN const CHAR8* Prefix
  );
#endif // MRC_DDR5_SETTINGS_H_
