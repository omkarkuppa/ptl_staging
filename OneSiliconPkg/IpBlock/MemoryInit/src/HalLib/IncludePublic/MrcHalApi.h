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

#ifndef _MrcHalApi_h_
#define _MrcHalApi_h_

#include "MrcRegisterCache.h"
#include "CMrcTypes.h"

///
/// Mode Parameters for MrcGetSet...()
///
#define GSM_READ_CSR      MRC_BIT0  ///< Set to force the read from hardware. Clear to read from cache.
#define GSM_READ_ONLY     MRC_BIT1  ///< Set to skip the write flow.
#define GSM_WRITE_OFFSET  MRC_BIT2  ///< Set to use the "value" input as an offset. Set to use the "value" input as an absolute.
#define GSM_FORCE_WRITE   MRC_BIT3  ///< Set to force a write even if the data to be written matches the cached value.
#define GSM_UPDATE_CACHE  MRC_BIT4  ///< Set to update cached value with the read or write value
#define GSM_COMBINE_EDGES MRC_BIT5  ///< Set to use value1 as left/low side edge and value2 as right/high side edge
                                    ///   Clear to use value1 as center and don't use value2
#define GSM_CACHE_ONLY    MRC_BIT6  ///< Set to update cached value only, clear updates both cache and hardware.
#define GSM_PRINT_VAL     MRC_BIT7  ///< Set this mode to have the HAL print the final Value before returning.
                                    ///   Uses MRC_DEBUG_NOTE level
#define GSM_UPDATE_HOST   MRC_BIT8  ///< Set to update the MRC Host Data
#define GSM_WRITE_ALL     MRC_BIT9  ///< Set to update all instances, ignoring population checks

#define ReadUncached              (GSM_READ_CSR | GSM_READ_ONLY)        ///< Force Register Field Read.
#define ReadNoCache               (GSM_READ_CSR | GSM_READ_ONLY)        ///< Force Register Field Read. No Cache Space is allocated.
#define ReadCached                (ReadUncached | GSM_UPDATE_CACHE)     ///< Force Register Field Read. Cache is updated.
#define ReadFromCache             GSM_READ_ONLY                         ///< Read Cached Value only.
#define ReadCrWriteCached         (GSM_READ_CSR | GSM_UPDATE_CACHE)     ///< Force Register Field Read and then Write if field is different.  Cache is update.
#define ReadCrForceWriteCached    (ReadCrWriteCached | GSM_FORCE_WRITE) ///< Force Register Field Read and then force Write.  Cache is update.
#define ReadCrWriteToCache        (ReadCrWriteCached | GSM_CACHE_ONLY)  ///< Force Register Field Read and then Caches the Write if field is different.  Cache is update.
#define ForceWriteUncached        GSM_FORCE_WRITE                       ///< Force Register Field write. Cache is not updated.
#define ForceWriteNoCache         GSM_FORCE_WRITE                       ///< Force Register Field write. No Cache Space is allocated.
#define WriteCached               GSM_UPDATE_CACHE                      ///< Updates Cache value to Input value.  Assumes cache coherency. Register Field is updated if Cached value differs from Input value.
#define WriteToCache              (GSM_CACHE_ONLY | GSM_UPDATE_CACHE)   ///< Updates the Cache Value only.
#define WriteUncached             0                                     ///< See <*>CommonTypes.h for the list of cached registers
#define ForceWriteCached          (GSM_FORCE_WRITE | GSM_UPDATE_CACHE)  ///< Force Register Field write. Cache is updated.
#define WriteOffsetUncached       GSM_WRITE_OFFSET                      ///< Add Input value to Cache value. Cache is not updated. Register Field is updated if offset value is non-zero.
#define ForceWriteOffsetCached    (ForceWriteCached | GSM_WRITE_OFFSET) ///< Add Input value to Cache value then Force Register Field write. Cache is updated.
#define ForceWriteOffsetUncached  (GSM_WRITE_OFFSET | GSM_FORCE_WRITE)  ///< Add Input value to Cache value then Force Register Field write. Cache is not updated.
#define WriteOffsetCached         (GSM_WRITE_OFFSET | GSM_UPDATE_CACHE) ///< Add Input value to Cache value. Cache is updated. Register Field is updated if offset value is non-zero.
#define WriteOffsetToCache        (WriteOffsetCached | GSM_CACHE_ONLY)  ///< Add Input value to Cache value. Cache is updated. No write to register.
#define ReadForceWriteNoCache     (GSM_READ_CSR | GSM_FORCE_WRITE)      ///< Read Register Field value and rewrite to it.
#define WriteNoCache              GSM_READ_CSR                          ///< Forces Read From Register (to preserver other register fields), updates the specific field, and writes the value to the register if update value is different from the Cached Value.
#define PrintValue                GSM_PRINT_VAL                         ///< GetSet will print the value after the Read/Write flow to the MRC log.

///
/// Channel Index Adjust macro for MC GSM_GT
///
///
// For MrcGetSet, we should only pass Channel 0 & 2 for calls when LPDDR which map to the shared CR's.
// Mask of the LSb to achieve this.
#define GS_LP_IP_CH(IsLp, Ch) ((IsLp) ? ((Ch) & (~0x1)) : (Ch))

///
/// GetSet initializer type
///
typedef struct {
  GSM_GT  Knob;
  INT64   Value;
} MRC_GSM_INIT_TYPE;

typedef struct {
  GSM_GT  Knob;
  INT8    Value;
} MRC_GSM_INIT_TYPE_8;

/**
  Top level function used to interact with DDRIO parameters.
  This function ignores unused parameters in Core MRC to reduce code space.
    Socket Index, DIMM index, FreqIndex, and Level.
  Only one setting for the whole controller, so Channel, Rank, Strobe, and Bit index is dropped.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetMc (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with DDRIO parameters.
  This function ignores unused parameters in Core MRC to reduce code space.
  Only one setting so Controller, Channel, Rank, Strobe, and Bit index is dropped.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetFreqIndex (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   FreqIndex,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function is used to set parameters that do not have any specificity to them.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetNoScope (
  IN      MrcParameters *const  MrcData,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based)
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetMcCh (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank within the DDR Channel (0-based).
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetMcChRnk (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is used to access indexed Command/Control/Clock groups.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank within the DDR Channel (0-based).
  @param[in]      Index       - Group index to Get/Set.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetCcc (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Index,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is used to access indexed Command/Control/Clock groups using Lane.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank within the DDR Channel (0-based).
  @param[in]      Lane        - Lane index to GSM_GT Group.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetCccLane (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Lane,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is used to access Block set of registers related to DCC

  @param[in]      MrcData        - Pointer to global data structure.
  @param[in]      PartitionBlock - Which Block set of DCC registers (DATASHARED, CCCSHARED) to access
  @param[in]      BlockIndex     - Which Index of Block to access (Passed on as Lane parameter)
  @param[in]      FieldIndex     - Which Field to access (passed on as Dimm parameter)
  @param[in]      PhIndex        - Which Phase to access (Passed on as FreqIndex parameter)
  @param[in]      Group          - DDRIO group to access.
  @param[in]      Mode           - Bit-field flags controlling Get/Set.
  @param[in,out]  Value          - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetPartitionBlockPiCodeLUT (
  IN      MrcParameters *const  MrcData,
  IN      UINT32                PartitionBlock,
  IN      UINT32                BlockIndex,
  IN      UINT32                FieldIndex,
  IN      UINT32                PhIndex,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is used to access Block set of registers related to DCC, but intended to CCC partitions.

  @param[in]      MrcData        - Pointer to global data structure.
  @param[in]      BlockIndex     - Which Index of Block to access (Passed on as Lane parameter)
  @param[in]      FieldIndex     - Which Field to access (Passed on as Rank parameter)
  @param[in]      PhIndex        - Which Phase to access (Passed on as FreqIndex parameter)
  @param[in]      Group          - DDRIO group to access.
  @param[in]      Mode           - Bit-field flags controlling Get/Set.
  @param[in,out]  Value          - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetCccPartitionBlockPiCodeLUT (
  IN      MrcParameters *const  MrcData,
  IN      UINT32                BlockIndex,
  IN      UINT32                FieldIndex,
  IN      UINT32                PhIndex,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is used to access Block set of registers related to DCC

  @param[in]      MrcData        - Pointer to global data structure.
  @param[in]      PartitionBlock - Which Block set of DCC registers (PLL, PG, VCCCLK) to access
  @param[in]      BlockIndex     - Which Index of Block to access (Passed on as Lane parameter)
  @param[in]      Group          - DDRIO group to access.
  @param[in]      Mode           - Bit-field flags controlling Get/Set.
  @param[in,out]  Value          - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetPartitionBlock (
  IN      MrcParameters *const  MrcData,
  IN      UINT32                PartitionBlock,
  IN      UINT32                BlockIndex,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is used to access indexed register field instances of block set of registers.

  @param[in]      MrcData        - Pointer to global data structure.
  @param[in]      PartitionBlock - Which Block set of DCC registers (PLL, PG, VCCCLK) to access (Passed on as Strobe)
  @param[in]      BlockIndex     - Which Index of Block to access (Passed on as Lane parameter)
  @param[in]      Index          - Group index to Get/Set (Passed on as Rank).
  @param[in]      Group          - DDRIO group to access.
  @param[in]      Mode           - Bit-field flags controlling Get/Set.
  @param[in,out]  Value          - Pointer to value for Get/Set to operate on. Can be offset or absolute value based on mode.

  @return MrcStatus
**/
MrcStatus
MrcGetSetPartitionBlockIndex (
  IN      MrcParameters *const  MrcData,
  IN      UINT32                PartitionBlock,
  IN      UINT32                BlockIndex,
  IN      UINT32        const   Index,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is used to access indexed register field instances.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Index       - Group index to Get/Set (Passed on as Lane).
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetIndex (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Index,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is used to access indexed and SaGv register field instance.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Index       - Group index to Get/Set (Passed on as Lane).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetIndexSagv (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Index,
  IN      UINT32        const   FreqIndex,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
);

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is for those registers that are only per Strobe per Channel granularity.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Strobe      - Dqs data group within the rank (0-based).
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetChStrb (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Strobe,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based)
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - Dqs data group within the rank (0-based).
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetStrobe (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.

  @param[in]      MrcData   - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel   - DDR Channel Number within the processor socket (0-based)
  @param[in]      Rank      - Rank number within a channel (0-based).
  @param[in]      Strobe    - Dqs data group within the rank (0-based).
  @param[in]      Bit       - Bit index within the data group (0-based).
  @param[in]      Group     - DDRIO group to access.
  @param[in]      Mode      - Bit-field flags controlling Get/Set.
  @param[in,out]  Value     - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetBit (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Bit,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  Top level function used to interact with DIMM.
  This function ignores unused parameters in Core MRC to reduce code space.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based)
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcCkdGetSetCW (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

// TODO: Remove when GREEN/BLUE code is separated correctly. Satisfy compiler
#define GET_SET_MEM_SS_SKIP
/**
  Top level function used to interact with MemSS parameters.
  This function ignores unused parameters to reduce code space.
  It sets registers for MemSS so it doesn't accept the Mptu/Channel/Rank parameters

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Group       - MemSS group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSetMemSS (
  IN      MrcParameters* const  MrcData,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64* const  Value
);

/**
  Top level function used to perform Smbus read transaction.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Address     - Smbus device address.
  @param[out]     Status      - Return status.

  @retval The data byte from the Smbus device.
**/
UINT8
MrcSmbusRead (
  IN  MrcParameters *const  MrcData,
  IN  UINT32         const  Address,
  OUT UINT32        *const  Status
  );

/**
  Top level function used to perform Smbus write transaction.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Address     - Smbus device address.
  @param[in]      Value       - The data value to write.
  @param[out]     Status      - Return status.

**/
void
MrcSmbusWrite (
  IN  MrcParameters *const  MrcData,
  IN  UINT32         const  Address,
  IN  UINT8                 Value,
  OUT UINT32        *const  Status
  );

#endif // _MrcHalApi_h_
