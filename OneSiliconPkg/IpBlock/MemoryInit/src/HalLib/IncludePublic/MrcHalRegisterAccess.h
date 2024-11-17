/** @file
  Header file associated with the memory controller hardware abstraction layer.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#ifndef _MrcHalRegisterAccess_h_
#define _MrcHalRegisterAccess_h_
#pragma pack (push, 1)

#include "MrcDebugPrint.h"
#include "MrcRegisterCache.h"

#define MAX_CHUNK_SIZE (8)

// MrcCheckComplexOrSideEffect() defines
#define MRC_IS_COMPLEX (TRUE)
#define MRC_IS_SIDE_EFFECT (FALSE)

/// Hash arrays
#define ROUNDTRIP_DELAY_MAX (8)

typedef struct {
  UINT8 Channel;
  UINT8 Lane;
} CccPinBlock;

///
/// Hash structure used by GetSet
///
#ifndef MRC_HASH_STRUCT
#define MRC_HASH_STRUCT
typedef union {
  struct {
    UINT32 Offset     :  17; // Bits 16:0
    UINT32 BfOffset   :   6; // Bits 22:17
    UINT32 IsVolatile :   1; // Bits 23:23
    UINT32 BfWidth    :   6; // Bits 29:24
    UINT32 RegSize    :   1; // Bits 30:30
    UINT32 BfSign     :   1; // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MRC_REGISTER_HASH_STRUCT;

#define MRC_REGISTER_HASH_Offset_OFF                                 ( 0)
#define MRC_REGISTER_HASH_Offset_WID                                 (17)
#define MRC_REGISTER_HASH_Offset_MSK                                 (0x00001FFFF)
#define MRC_REGISTER_HASH_Offset_MIN                                 (0)
#define MRC_REGISTER_HASH_Offset_MAX                                 (131071) // 0x0001FFFF
#define MRC_REGISTER_HASH_Offset_DEF                                 (0x00000000)

#define MRC_REGISTER_HASH_BfOffset_OFF                               (17)
#define MRC_REGISTER_HASH_BfOffset_WID                               ( 6)
#define MRC_REGISTER_HASH_BfOffset_MSK                               (0x007E0000)
#define MRC_REGISTER_HASH_BfOffset_MIN                               (0)
#define MRC_REGISTER_HASH_BfOffset_MAX                               (63) // 0x0000003F
#define MRC_REGISTER_HASH_BfOffset_DEF                               (0x00000000)

#define MRC_REGISTER_HASH_NoAdj_OFF                                  (23)
#define MRC_REGISTER_HASH_NoAdj_WID                                  ( 1)
#define MRC_REGISTER_HASH_NoAdj_MSK                                  (0x00800000)
#define MRC_REGISTER_HASH_NoAdj_MIN                                  (0)
#define MRC_REGISTER_HASH_NoAdj_MAX                                  (1) // 0x00000001
#define MRC_REGISTER_HASH_NoAdj_DEF                                  (0x00000000)

#define MRC_REGISTER_HASH_BfWidth_OFF                                (24)
#define MRC_REGISTER_HASH_BfWidth_WID                                ( 6)
#define MRC_REGISTER_HASH_BfWidth_MSK                                (0x3F000000)
#define MRC_REGISTER_HASH_BfWidth_MIN                                (0)
#define MRC_REGISTER_HASH_BfWidth_MAX                                (63) // 0x0000003F
#define MRC_REGISTER_HASH_BfWidth_DEF                                (0x00000000)

#define MRC_REGISTER_HASH_RegSize_OFF                                (30)
#define MRC_REGISTER_HASH_RegSize_WID                                ( 1)
#define MRC_REGISTER_HASH_RegSize_MSK                                (0x40000000)
#define MRC_REGISTER_HASH_RegSize_MIN                                (0)
#define MRC_REGISTER_HASH_RegSize_MAX                                (1) // 0x00000001
#define MRC_REGISTER_HASH_RegSize_DEF                                (0x00000000)

#define MRC_REGISTER_HASH_BfSign_OFF                                 (31)
#define MRC_REGISTER_HASH_BfSign_WID                                 ( 1)
#define MRC_REGISTER_HASH_BfSign_MSK                                 (0x80000000)
#define MRC_REGISTER_HASH_BfSign_MIN                                 (0)
#define MRC_REGISTER_HASH_BfSign_MAX                                 (1) // 0x00000001
#define MRC_REGISTER_HASH_BfSign_DEF                                 (0x00000000)
#endif // MRC_HASH_STRUCT

#define GSM_SAFE_STR_ACC(index)  MAX ((index), (sizeof (GsmGtDebugStrings) / sizeof (GsmGtDebugStrings[0]))

#define MRC_CLK_GRP_MAX         (4)
#define MRC_CTL_GRP_MAX         (4)
#define MRC_CKE_GRP_MAX         (4)
#define MRC_U_Y_CCC_GRP_MAX     (2)
#define MRC_H_S_CCC_GRP_MAX     (4)
#define MRC_DDR5_CMD_GRP_MAX    (2)

// DCC defines
#define MAX_PHDCC                  (4)

///
/// Shared Constants
///
extern const CHAR8* GsmGtDebugStrings[GsmGtMax + 1];

/**
  Top level function that is System Level Specificity.
  The flow is as follows:
    Check Multicast/Unicast, translate from system to IP register architecture, side effect and update other PHY logic.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Dimm        - DIMM Number within the DDR Channel (0-based). Ignored as Rank is rank number in the channel.
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - Dqs data group within the rank (0-based).
  @param[in]      Lane        - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Level       - DDRIO level to access.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSet (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Dimm,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Lane,
  IN      UINT32        const   FreqIndex,
  IN      GSM_LT        const   Level,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
);

/**
This function is used to interact with SOC.
The flow is as follows:
  Get the GSM_GT bit shift instruction, CR offset, and Multicasting offsets.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Dimm        - DIMM Number within the DDR Channel (0-based). Ignored as Rank is rank number in the channel.
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      Bit         - Bit index within the data group (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Level       - DDRIO level to access.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
GetSet (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Dimm,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Bit,
  IN      UINT32        const   FreqIndex,
  IN      GSM_LT        const   Level,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
);


/**
  This function is used to interact with SOC.
  The flow is as follows:
    Get the GSM_GT bit shift instruction, CR offset, and Multicasting offsets.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      Lane        - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.
  @retval MrcStatus
**/
MrcStatus
MrcCkdGetSet (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Lane,
  IN      UINT32        const   FreqIndex,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
);


/**
  This function is the interface for the core of the MRC to get the limits of a
  specific GSM_GT parameter.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[in]  Rank      - Rank number within a channel (0-based).
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetSetLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  IN  UINT32  const   Rank,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  );

/**
  This function is the interface for the core of the MRC to get the limits of RxVref or RxDqVref.

  @param[in]  MrcData   - Pointer to global data.
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetSetRxVrefLimits (
  IN  MrcParameters *const MrcData,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  );

/**
  This function generates the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller    - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel       - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Dqs data group within the rank (0-based).
  @param[in]      Lane          - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex     - Index supporting multiple operating frequencies.
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Pointer to return the Register volatile mask.

  @retval MrcStatus
**/
MrcStatus
MrcGetMcTimingHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  );

/**
  This function returns the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller    - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel       - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Dqs data group within the rank (0-based).
  @param[in]      Lane          - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex     - Index supporting multiple operating frequencies.
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Pointer to return the Register volatile mask.

  @retval MrcStatus
**/
MrcStatus
MrcGetMcConfigHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  );

/**
  This function returns the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller    - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel       - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Dqs data group within the rank (0-based).
  @param[in]      Lane          - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex     - Index supporting multiple operating frequencies.
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Pointer to return the Register volatile mask.

  @retval MrcStatus
**/
MrcStatus
MrcGetPmaHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  );


/**
  This function returns the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]      Controller    - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]      Channel       - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]      Lane          - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]      FreqIndex     - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Unused here, but needed so we can use a pointer to this function or a different function

  @retval MrcStatus
**/
MrcStatus
MrcGetCkdHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  );


/**
  Update MRC Host Data Structure when GSM_UPDATE_HOST is set
  Currently only used for RxDqBitDelay and TxDqBitDelay

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      Bit         - Bit index within the data group (0-based).
  @param[in]      Group       - DDRIO group to access.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

**/
void
MrcGetSetUpdateHost (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Bit,
  IN      GSM_GT        const   Group,
  IN OUT  INT64         *const  Value
  );

/**
  Converting CCC Pin to Group and Lane

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Pin         - CCC Pin Number to check.
  @param[in,out]  TransLane   - Lane index to GSM_GT Group.
  @param[in,out]  TransRank   - Rank within the DDR Channel (0-based).
  @param[in,out]  Group       - DDRIO group to access.

  @retval MrcStatus
**/
MrcStatus
MrcGetCccGroupLane (
  IN      MrcParameters *const  MrcData,
  IN      UINT8         const   Channel,
  IN      UINT8         const   Pin,
  IN OUT  UINT32        *const  TransLane,
  IN OUT  UINT32        *const  TransRank,
  IN OUT  GSM_GT        *const  Group
  );

/**
  This function programs WCK DCC registers to work in a cross-platform manner

  @param[in] MrcData     - Include all MRC global data.
  @param[in] GetSetGroup - WCK DCC GetSet Group
  @param[in] GetSetVal   - WCK DCC Value
**/
VOID
MrcSetWckDccCrossProj (
  IN MrcParameters* const MrcData,
  IN GSM_GT               GetSetGroup,
  IN INT64                *GetSetVal
  );

/**
  Check if the given GSM_GT group key is skipable for reasons of writing to
  channels 1/3.

  @param[in]  Group  - DDRIO group to access.

  @retval IsSkipGroup - TRUE, if within a GSM_GT skipable range
                      - FALSE, otherwise
**/
BOOLEAN
MrcGsmGtSkip (
  IN  GSM_GT  const  Group
  );

/**
  Routing function to retrieve the correct Hash and Volatile Mask for a GSM_GT group instance.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller    - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel       - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Dqs data group within the rank (0-based).
  @param[in]      Bit           - Bit index within the GSM_GT group (0-based).
  @param[in]      FreqIndex     - Index supporting multiple operating frequencies.
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Pointer to return the Register volatile mask.

  @retval MrcStatus - mrcSuccess, the hash was sucessfully retreived
                    - mrcInstanceDoesNotExist, otherwise
**/
MrcStatus
MrcGetHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Bit,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  );

#ifdef MRC_DEBUG_PRINT
/**
  This debug only function checks to ensure the group is supported by GetSet.

  @param[in]  MrcData - Global MRC Data.
  @param[in]  Group   - The group to be accessed.

  @retval mrcSuccess if supported, otherwise mrcWrongInputParameter
**/
MrcStatus
MrcCheckGroupSupported (
  IN  MrcParameters *const  MrcData,
  IN  GSM_GT        const   Group
  );
#endif // MRC_DEBUG_PRINT

/**
  Check whether the group is supported by current project

  @param[in]  MrcData - Global MRC Data.
  @param[in]  Group   - The group to be accessed.
  @param[in]  Strobe  - The Strobe to be accessed.

  @retval mrcSuccess if supported, otherwise mrcWrongInputParameter
**/
BOOLEAN
MrcCheckGroupUnSupported (
  IN  MrcParameters *const  MrcData,
  IN  GSM_GT        const   Group,
  IN  UINT32        const   Strobe
  );

/**
 Helper function to get max partition number.

 @param[in] Partition - Partition type.
 @param[in] UlxUlt    - Whether CPU SKU is ULX / ULT (PHY).

**/
UINT32
GetPartitionMax (
  IN PARTITION_TYPE   Partition,
  IN BOOLEAN          UlxUlt
  );

/**
  Variable Max Limits for parameters.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - The group to be checked.
  @param[in]  Index     - The index of the group to be checked.
  @param[out] RankMax   - Maximum value allow for Rank parameter.
  @param[out] StrobeMax - Maximum value allow for Strobe parameter.
  @param[out] LaneMax   - Maximum value allow for Lane parameter.
**/
VOID
MrcGetSetParamMaxAdjust (
  IN  MrcParameters *const  MrcData,
  IN  GSM_GT        const   Group,
  IN  UINT32                Index,
  OUT UINT32                *RankMax,
  OUT UINT32                *StrobeMax,
  OUT UINT32                *LaneMax
  );

/**
  This function is used to determine if the GSM_GT passed in is a complex parameter.

  @param[in]  MrcData - Pointer to global data structure.
  @param[in]  Group   - GetSet group to check.
  @param[in]  IsComplex - The requested check is to see if the parameter is complex.  Otherwise is there a side effect.

  @retval TRUE if complex or side effect, otherwise FALSE.
**/
BOOLEAN
MrcCheckComplexOrSideEffect (
  IN  MrcParameters *const  MrcData,
  IN  GSM_GT        const   Group,
  IN  BOOLEAN       const   IsComplex
  );

/**
  This function handles accessing and updating complex parameter values.
  This function will adjust the signal for that scope.  It will Not adjust other relative signals.
    Any shared parameter will need to be accounted for outside this function.
  The way each complex parameter is composed is project specific.
  The units of each parameter is specified in the parameter declaration in GSM_GT.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      SubChannel  - DDR SubChannel number within a Channel (0-Based).
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus: mrcComplexParamDone - no more programming is needed, otherwise continue with GetSet flow.
**/
MrcStatus
MrcGetSetComplexParam (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   SubChannel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   FreqIndex,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Converting controller, channel, rank, strobe number from System to IP register architecture.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - Dqs data group within the rank or the command group (0-based).
  @param[in]      Lane        - Lane index within the GSM_GT group (0-based).
  @param[in]      Group       - DDRIO group to access.

  @retval MrcStatus
**/
MrcStatus
MrcTranslateSystemToIp (
  IN      MrcParameters *const  MrcData,
  IN OUT  UINT32        *const  Controller,
  IN OUT  UINT32        *const  Channel,
  IN OUT  UINT32        *const  Rank,
  IN OUT  UINT32        *const  Strobe,
  IN OUT  UINT32        *const  Lane,
  IN      GSM_GT        const   Group
  );

/*
  This function is used to update other Phy register in the channel based on DDR type.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Dimm        - DIMM Number within the DDR Channel (0-based). Ignored as Rank is rank number in the channel.
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      Bit         - Bit index within the data group (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Level       - DDRIO level to access.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval Nothing.
**/
void
MrcGetSetUpdatePhyChannels (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Dimm,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Bit,
  IN      UINT32        const   FreqIndex,
  IN      GSM_LT        const   Level,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN      INT64         *const  Value
  );

/*
  This function perform any side effect action required from the Group that was just set.
  It will inherit the same Mode parameter so it passed it on to the actions executed.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      SubChannel  - DDR SubChannel number within a Channel (0-Based).
  @param[in]      Dimm        - DIMM Number within the DDR Channel (0-based). Ignored as Rank is rank number in the channel.
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      Bit         - Bit index within the data group (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Level       - DDRIO level to access.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval Nothing.
**/
void
MrcGetSetSideEffect (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   SubChannel,
  IN      UINT32        const   Dimm,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Bit,
  IN      UINT32        const   FreqIndex,
  IN      GSM_LT        const   Level,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN      INT64         *const  Value
  );

/**
  This function checks if the Group's value need do Divide by codes instead of
  by hardware.
  @param[in]  MrcData       - Pointer to global data.
  @param[in]  Group         - Group to access.

  @return BOOLEAN
**/
BOOLEAN
MrcGetSetNeedDivide (
  IN   MrcParameters *const  MrcData,
  IN   GSM_GT        const   Group
  );

/**
  This function calculates values by dividing 2 if needed.
  @param[in]  MrcData       - Pointer to global data.
  @param[in]  Value         - Value

  @return Result.
**/
INT64
MrcGetDividedValueTck (
  IN    MrcParameters *const  MrcData,
  IN    INT64                 Value
  );

/**
  Check if CR Cache is enabled or not in GetSet HAL

  @retval TRUE  - CR cache is enabled
  @retval FALSE - CR cache is disabled
**/
BOOLEAN
MrcHalCrCacheEnabled (
  VOID
  );

#pragma pack (pop)
#endif
