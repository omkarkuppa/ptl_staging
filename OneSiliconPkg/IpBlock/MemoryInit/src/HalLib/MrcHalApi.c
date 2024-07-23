/** @file
  Implementation of the memory controller hardware abstraction layer.

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

// Include files
#include "MrcHalApi.h" // for function prototypes and interface definitions
#include "CMcAddress.h"
#include "MrcHalInternal.h"

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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          Controller,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          FreqIndex,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          Controller,
          Channel,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          Controller,
          Channel,
          MRC_IGNORE_ARG,
          Rank,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          Controller,
          Channel,
          MRC_IGNORE_ARG,
          Rank,
          Index,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          Controller,
          Channel,
          MRC_IGNORE_ARG,
          Rank,
          MRC_IGNORE_ARG,
          Lane,
          MRC_IGNORE_ARG,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is used to access Block set of registers related to DCC

  @param[in]      MrcData        - Pointer to global data structure.
  @param[in]      PartitionBlock - Which Block set of DCC registers (DATASHARED, CCCSHARED) to access
  @param[in]      BlockIndex     - Which Index of Block to access (Passed on as Lane parameter)
  @param[in]      FieldIndex     - Which Field to access (Passed on as Rank parameter)
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
  )
{
  return MrcGetSet (
    MrcData,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    FieldIndex,
    PartitionBlock,
    BlockIndex,
    PhIndex,
    DdrLevel,
    Group,
    Mode,
    Value
  );
}

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
  )
{
  return MrcGetSet (
    MrcData,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    PartitionBlock,
    (PartitionBlock == PartitionPll) ? MRC_IGNORE_ARG : BlockIndex,
    MRC_IGNORE_ARG,
    DdrLevel,
    Group,
    Mode,
    Value
  );
}

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
  )
{
  return MrcGetSet (
    MrcData,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    Index,
    PartitionBlock,
    (PartitionBlock == PartitionPll) ? MRC_IGNORE_ARG : BlockIndex,
    MRC_IGNORE_ARG,
    DdrLevel,
    Group,
    Mode,
    Value
  );
}

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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          Index,
          MRC_IGNORE_ARG,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

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
)
{
  return MrcGetSet (
    MrcData,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    Index,
    FreqIndex,
    DdrLevel,
    Group,
    Mode,
    Value
  );
}

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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          Controller,
          Channel,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          Strobe,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

/**
  Top level function used to interact with SOC.
  This function ignores unused parameters in Core MRC to reduce code space.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          Controller,
          Channel,
          MRC_IGNORE_ARG,
          Rank,
          Strobe,
          MRC_IGNORE_ARG,
          0,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

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
  )
{
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          Controller,
          Channel,
          MRC_IGNORE_ARG,
          Rank,
          Strobe,
          Bit,
          MRC_IGNORE_ARG,
          DdrLevel,
          Group,
          Mode,
          Value
          );
}

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
)
{
  return MrcCkdGetSet (
    MrcData,
    MRC_IGNORE_ARG,
    Controller,
    Channel,
    Rank,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    Group,
    Mode,
    Value
  );
}

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
)
{
  const MRC_FUNCTION  *MrcCall;

  MrcCall = MrcData->Inputs.Call.Func;
  return MrcCall->MrcSmbusRead8 (Address, Status);
}

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
)
{
  const MRC_FUNCTION  *MrcCall;

  MrcCall = MrcData->Inputs.Call.Func;
  MrcCall->MrcSmbusWrite8 (Address, Value, Status);
}

// TODO: Remove when GREEN/BLUE code is separated correctly. Satisfy compiler
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
  IN      MrcParameters *const  MrcData,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  )
{
#ifdef GET_SET_MEM_SS_SKIP
  return mrcSuccess;
#else
  return MrcGetSet (
          MrcData,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          MRC_IGNORE_ARG,
          DdrLevel,
          Group,
          Mode,
          Value
          );
#endif
}
