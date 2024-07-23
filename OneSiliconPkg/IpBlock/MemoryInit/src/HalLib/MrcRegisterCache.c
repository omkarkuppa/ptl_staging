/** @file
  This file contains functions to access the register cacheing information.

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

#include "MrcRegisterCache.h" // for function prototypes and interface definitions
#include "MrcCommon.h"
#include "MrcHalRegisterAccess.h"
#include "CMrcStatsTracker.h" // for MrcStatsIncrementData()

typedef enum {
  RegCacheSuccess,
  RegCacheNextFree,
  RegCacheNextWrite,
  RegCacheFull,
  RegCacheMax
} RegCacheStatus;


/**
  Search the register cache for the specified MCHBAR register offset value.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[out] Record  - The index into the register cache that contains the location to read or store data.
  @param[in]  Offset  - MCHBAR register offset.

  @retval 0 = Found, 1 = Not found but Record contains index to next available free location, 2 = Not found but Record contains index to next available location that has read only data, >2 = cache full.
**/
static
RegCacheStatus
SeekCacheLocation (
  MrcParameters *const MrcData,
  UINT16        *const Record,
  const UINT32  Offset
  )
{
  MrcOutput                     *Outputs;
  MRC_REGISTER_CACHE            *RegisterCache;
  REGISTER_CACHE_OFFSET_STRUCT  *Cache;
  UINT16                        Index;
  UINT16                        FirstEmptyIndex;
  UINT16                        FirstNonPendingIndex;
  UINT16                        StartIndex;
  UINT16                        HashVal;
  UINT16                        MaxEntry;

  Outputs              = &MrcData->Outputs;
  RegisterCache        = &Outputs->RegisterCache;
  MaxEntry             = MAX_REGISTER_CACHE_ENTRIES;
  FirstEmptyIndex      = MaxEntry;
  FirstNonPendingIndex = MaxEntry;
  StartIndex           = (Offset >> 2 & MIN_CACHE_ENTRIES_MASK);

  // MRC_HAL_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_HAL, "DSeekCacheLocation: Offset = 0x%X\n", Offset);
  for (Index = 0; Index < MaxEntry; Index++) {
    HashVal = (StartIndex + Index) % MaxEntry;
    Cache   = &RegisterCache->Data[HashVal];
    if (Cache->Flags.Bits.IsValid) {
      if ((Cache->Offset == (UINT16) Offset) &&
          (Cache->Flags.Bits.OffsetMsb == (UINT8)((Offset >> 16) & 1))) {
        *Record = HashVal;
        return (RegCacheSuccess);
      } else if (!Cache->Flags.Bits.IsDirty && (FirstNonPendingIndex == MaxEntry)) {
        FirstNonPendingIndex = HashVal;
      }
    } else if (FirstEmptyIndex == MaxEntry) {
      FirstEmptyIndex = HashVal;
    }
    if (FirstEmptyIndex < MaxEntry) {
      RegisterCache->MaxValid = HashVal;
      break;
    }
  }

  if (FirstEmptyIndex < MaxEntry) {
    *Record = FirstEmptyIndex;
    return (RegCacheNextFree);
  } else if (FirstNonPendingIndex < MaxEntry) {
    *Record = FirstNonPendingIndex;
    return (RegCacheNextFree);
  }
  return (RegCacheFull);
}

/**
  Set the current cached 32-bit data value for the specified MCHBAR register.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Offset     - MCHBAR register offset.
  @param[in] Is64BitReg - TRUE if Offset points to a 64-bit register, FALSE if Offset points to a 32-bit register.
  @param[in] IsDirty    - TRUE = Data has not been written from cache location to hardware register.
  @param[in] Data       - The data.

  @retval The value written to the cache.
**/
static
UINT32
SetCache32 (
  MrcParameters *const MrcData,
  const UINT32  Offset,
  const BOOLEAN Is64BitReg,
  const BOOLEAN IsDirty,
  const BOOLEAN IsHighDWord,
  const UINT32  Data
  )
{
  MrcOutput                    *Outputs;
  MRC_REGISTER_CACHE           *RegisterCache;
  REGISTER_CACHE_OFFSET_STRUCT *Cache;
  UINT16                       Index;

  Outputs       = &MrcData->Outputs;
  RegisterCache = &Outputs->RegisterCache;
  if (SeekCacheLocation (MrcData, &Index, Offset) < RegCacheFull) {
    Cache           = &RegisterCache->Data[Index];
    Cache->Offset   = (UINT16) Offset;
    Cache->Data     = Data;
    Cache->Flags.Bits.Is64BitReg  = Is64BitReg;
    Cache->Flags.Bits.IsDirty     = IsDirty;
    Cache->Flags.Bits.IsValid     = TRUE;
    Cache->Flags.Bits.IsHighDWord = IsHighDWord;
    Cache->Flags.Bits.OffsetMsb   = (UINT8)(Offset >> 16) & 1;
  } else {
    RegisterCache->Overflow = TRUE;
  }
  return (Data);
}

/**
  Get the current cached 64-bit data value for the specified MCHBAR register.
  If it is not in the cache, read it from MMIO space.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Offset  - MCHBAR register offset.

  @retval The value read.
**/
static
UINT64
GetRegisterCachedData64 (
  MrcParameters *const MrcData,
  const UINT32  Offset
  )
{
  MRC_REGISTER_CACHE *RegisterCache;
  UINT32             Status;
  UINT32             Reg[(sizeof (UINT64) / sizeof (UINT32))];
  UINT16             Index[(sizeof (UINT64) / sizeof (UINT32))];
  UINT8              Pass;
  UINT8              Count;
  union {
    UINT64 Data;
    UINT32 Data32[2];
  } Value;

  RegisterCache = &MrcData->Outputs.RegisterCache;
  Value.Data    = 0;
  Count         = 0;
  Reg[0]        = Offset;
  Reg[1]        = Offset + sizeof (UINT32);

  for (Pass = 0; Pass < (sizeof (Index) / sizeof (Index[0])); Pass++) {
    Status = SeekCacheLocation (MrcData, &Index[Pass], Reg[Pass]);
    if (Status == RegCacheSuccess) {
      Count |= (1 << Pass);
    } else if (Status >= RegCacheFull) {
      RegisterCache->Overflow = TRUE;
    }
  }

  if (Count == 3) {
    for (Pass = 0; Pass < (sizeof (Index) / sizeof (Index[0])); Pass++) {
      Value.Data32[Pass] = RegisterCache->Data[Index[Pass]].Data;
    }
  } else {
    Value.Data = MrcReadCR64 (MrcData, Offset);
    for (Pass = 0; Pass < (sizeof (Index) / sizeof (Index[0])); Pass++) {
      SetCache32 (MrcData, Reg[Pass], TRUE, FALSE, (Pass == 1), Value.Data32[Pass]);
    }
  }

  return (Value.Data);
}

/**
  Set the current cached 64-bit data value for the specified MCHBAR register.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Offset  - MCHBAR register offset.
  @param[in]  IsDirty - TRUE = Data has not been written from cache location to hardware register.
  @param[out] Data    - The data.

  @retval The value written.
**/
static
UINT64
SetRegisterCachedData64 (
  MrcParameters *const MrcData,
  const UINT32  Offset,
  const BOOLEAN IsDirty,
  const UINT64  Data
  )
{
  UINT8                        Pass;
  union {
    UINT64 Data;
    UINT32 Data32[2];
  } Value;

  Value.Data = Data;
  for (Pass = 0; Pass < (sizeof (Value.Data32) / sizeof (Value.Data32[0])); Pass++) {
    SetCache32 (MrcData, Offset + (Pass * sizeof (Value.Data32[0])), TRUE, IsDirty, (Pass == 1), Value.Data32[Pass]);
  }

  return (Data);
}

/**
  This function updates the current register value in the register cache.

  @param[in] MrcData    - Pointer to global data.
  @param[in] Is64BitReg - TRUE if Offset points to a 64-bit register, FALSE if Offset points to a 32-bit register.
  @param[in] IsDirty    - TRUE = Data has not been written from cache location to hardware register.
  @param[in] Offset     - CR Offset to update in the cache.
  @param[in] Value      - New register value.

  @retval The current register value, either from the cache or from hardware if either cache reads are disabled or doesn't exist.
**/
static
VOID
MrcCrCacheUpdate (
  IN MrcParameters *const MrcData,
  IN const BOOLEAN Is64BitReg,
  IN const BOOLEAN IsDirty,
  IN const UINT32  Offset,
  IN const UINT64  Value
  )
{
  UINT64_STRUCT    RegValue;

  RegValue.Data = Value;

  if (Is64BitReg) {
    SetRegisterCachedData64 (MrcData, Offset, IsDirty, RegValue.Data);
  } else {
    SetCache32 (MrcData, Offset, FALSE, IsDirty, FALSE, RegValue.Data32.Low);
  }
}

#if 0 // This function is used for dynamic register save/restore from cache and multicast accesses.
/**
  This function updates the current register value in the register cache.

  @param[in] MrcData    - Pointer to global data.
  @param[in] Is64BitReg - TRUE if Offset points to a 64-bit register, FALSE if Offset points to a 32-bit register.
  @param[in] IsDirty    - TRUE = Data has not been written from cache location to hardware register.
  @param[in] Socket     - Processor socket in the system (0-based). Not used in Core MRC.
  @param[in] Group      - DDRIO group being accessed.
  @param[in] Channel    - DDR Channel Number within the processor socket (0-based).
  @param[in] Rank       - Rank index in the channel. (0-based).
  @param[in] Strobe     - Dqs data group within the rank (0-based).
  @param[in] Value      - New register value.
  @param[in] RegOffset  - Pointer to Multicast data for updating the cache.

  @retval The current register value, either from the cache or from hardware if either cache reads are disabled or doesn't exist.
**/
static
VOID
MrcCrCacheUpdate (
  IN MrcParameters *const MrcData,
  IN const BOOLEAN Is64BitReg,
  IN const BOOLEAN IsDirty,
  IN const GSM_GT  Group,
  IN const UINT32  Socket,
  IN const UINT32  Channel,
  IN const UINT32  Rank,
  IN const UINT32  Strobe,
  IN const UINT64  Value,
  IN const MRC_REG_OFFSETS *const RegOffset
  )
{
  MrcControllerOut *ControllerOut;
  MrcChannelOut    *ChannelOut;
  UINT64_STRUCT    RegValue;
  UINT32           Offset;
  UINT32           ChannelStart;
  UINT32           ChannelEnd;
  UINT32           ChannelLoop;
  UINT32           RankStart;
  UINT32           RankEnd;
  UINT32           RankLoop;
  UINT32           StrobeStart;
  UINT32           StrobeEnd;
  UINT32           StrobeLoop;

  RegValue.Data = Value;

  if (RegOffset->BCastType == MrcBcastChannelStrobe) {
    ChannelStart = 0;
    ChannelEnd = MAX_CHANNEL - 1;
    StrobeStart = 0;
    StrobeEnd = MAX_SDRAM_IN_DIMM - 1;
    // @todo: Add Strobe Brodacast support when designs need it.
//  } else if (RegOffset->BCastType == MrcBcastStrobe) {
//    MRC_DEBUG_ASSERT (1==0, &MrcData->Outputs.Debug, "Unsupported Broadcast. BCastType == %d", RegOffset->BCastType);
  } else if (RegOffset->BCastType == MrcBcastRank) {
    ChannelStart = Channel;
    ChannelEnd = Channel;
    StrobeStart = 0;
    StrobeEnd = MAX_SDRAM_IN_DIMM - 1;
  } else if (RegOffset->BCastType == MrcBcastChannel) {
    ChannelStart = Channel;
    ChannelEnd = Channel;
    // @todo: Currently we have not have Rank Segmentation in Register Layout.  If so, need to add rank logic.
    StrobeStart = 0;
    StrobeEnd = MAX_SDRAM_IN_DIMM - 1;
  } else if (RegOffset->BCastType == MrcBcastSystem) {
    ChannelStart = 0;
    ChannelEnd = MAX_CHANNEL - 1;
    StrobeStart = 0;
    StrobeEnd = MAX_SDRAM_IN_DIMM - 1;
  } else { // MrcBcastUnicast
    ChannelStart = Channel;
    ChannelEnd = Channel;
    StrobeStart = Strobe;
    StrobeEnd = Strobe;
  }
  RankStart = Rank;
  RankEnd = Rank;
  Offset = RegOffset->BOffset;

  //
  // If any loop address offset is zero, then we set Start==Stop
  //
  if (RegOffset->COffset == 0) {
    ChannelStart = Channel;
    ChannelEnd = Channel;
  }
  // Optimization.  Client products don't Multicast across ranks.
  //if (RegOffset->ROffset == 0) {
  //  RankStart = Rank;
  //  RankEnd = Rank;
  //}
  if (RegOffset->SOffset == 0) {
    StrobeStart = Strobe;
    StrobeEnd = Strobe;
  }

  ControllerOut = &MrcData->Outputs.Controller[0];
  for (ChannelLoop = ChannelStart; ChannelLoop <= ChannelEnd; ChannelLoop++) {
    ChannelOut = &ControllerOut->Channel[ChannelLoop];
    for (RankLoop = RankStart; RankLoop <= RankEnd; RankLoop++) {
      for (StrobeLoop = StrobeStart; StrobeLoop <= StrobeEnd; StrobeLoop++) {
        Offset = RegOffset->BOffset + (RegOffset->COffset * ChannelLoop) +
                  (RegOffset->ROffset * RankLoop) + (RegOffset->SOffset * StrobeLoop);
        if (Is64BitReg) {
          SetRegisterCachedData64 (MrcData, Offset, IsDirty, RegValue.Data);
        } else {
          SetCache32 (MrcData, Offset, FALSE, IsDirty, RegValue.Data32.Low);
        }
      }
    }
  }
}

/**
  Get the current cached 32-bit data value and MCHBAR register offset.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[out] Offset  - The MCHBAR register offset.
  @param[out] Data    - The data.

  @retval mrcSuccess if found in the cache, otherwise mrcFail.
**/
MrcStatus
GetNextCache32 (
  MrcParameters *const MrcData,
  UINT32        *const Offset,
  UINT32        *const Data
  )
{
  MrcOutput                    *Outputs;
  MRC_REGISTER_CACHE           *RegisterCache;
  REGISTER_CACHE_OFFSET_STRUCT *Cache;
  UINT16                       Index;

  Outputs       = &MrcData->Outputs;
  RegisterCache = &Outputs->RegisterCache;
  Cache         = &RegisterCache->Data[0];
  for (Index = 0; Index < MAX_REGISTER_CACHE_ENTRIES; Index++, Cache++) {
    if (Cache->Flags.Bits.IsValid) {
      if (RegisterCache->NextSequence == Cache->Sequence) {
        *Data   = Cache->Data;
        *Offset = (Cache->Flags.Bits.OffsetMsb << 16) | Cache->Offset;
        break;
      }
    }
  }
  RegisterCache->NextSequence++;
  return ((RegisterCache->NextSequence > RegisterCache->CurrentSequence) ? mrcFail : mrcSuccess);
}
#endif

/**
  Invalidate the specified offset in the register cache.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Offset  - MCHBAR register offset.

  @retval mrcSuccess if found in the cache, otherwise mrcFail.
**/
MrcStatus
InvalidateRegisterOffsetInCache (
  MrcParameters *const MrcData,
  UINT32  Offset
  )
{
  MrcOutput                    *Outputs;
  MRC_REGISTER_CACHE           *RegisterCache;
  REGISTER_CACHE_OFFSET_STRUCT *Cache;
  UINT16                       Index;

  if (SeekCacheLocation (MrcData, &Index, Offset) == RegCacheSuccess) {
    Outputs                       = &MrcData->Outputs;
    RegisterCache                 = &Outputs->RegisterCache;
    Cache                         = &RegisterCache->Data[Index];
    Cache->Flags.Bits.IsValid = FALSE;
    if (Cache->Flags.Bits.Is64BitReg) {
      if (Cache->Flags.Bits.IsHighDWord) {
        // The register entry found is the upper 32 bits. Need to find the lower 32 bits.
        Offset -= 4;
      }
      else {
        // The register entry found is the lower 32 bits. Need to find the upper 32 bits.
        Offset += 4;
      }

      // Find cache entry of the other half of the 64-bit register
      if (SeekCacheLocation(MrcData, &Index, Offset) == RegCacheSuccess) {
        Cache = &RegisterCache->Data[Index];
        Cache->Flags.Bits.IsValid = FALSE;
      }
    }

    RegisterCache->Overflow = FALSE;
    return (mrcSuccess);
  }

  return (mrcFail);
}

/**
  Initialize the register cache.

  @param[in] MrcData - Pointer to MRC global data.

  @retval mrcSuccess.
**/
MrcStatus
InitializeRegisterCache (
  MrcParameters *const MrcData
  )
{
  const MRC_FUNCTION *MrcCall;

  MrcCall = MrcData->Inputs.Call.Func;
  MrcCall->MrcSetMem ((UINT8 *) &MrcData->Outputs.RegisterCache, sizeof (MRC_REGISTER_CACHE), 0);
  return (mrcSuccess);
}

/**
  Invalidate the register cache data.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval mrcSuccess.
**/
MrcStatus
MrcInvalidateRegisterCachedData (
  MrcParameters *const MrcData
  )
{
  MrcOutput                    *Outputs;
  MRC_REGISTER_CACHE           *RegisterCache;
  REGISTER_CACHE_OFFSET_STRUCT *Cache;
  UINT16                       Index;

  Outputs       = &MrcData->Outputs;
  RegisterCache = &Outputs->RegisterCache;
  for (Index = 0; Index < MAX_REGISTER_CACHE_ENTRIES; Index++) {
    Cache = &RegisterCache->Data[Index];
    Cache->Flags.Data = 0;
  }
  RegisterCache->Overflow = FALSE;
  return (mrcSuccess);
}

/**
  Flush the register data cache entries marked pending to hardware.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval mrcSuccess.
**/
MrcStatus
MrcFlushRegisterCachedData (
  MrcParameters *const MrcData
  )
{
  MrcOutput                    *Outputs;
  MRC_REGISTER_CACHE           *RegisterCache;
  // The first cache entry found
  REGISTER_CACHE_OFFSET_STRUCT *Cache;

  // If the entry is 64-bit, this is where the second seek is stored
  REGISTER_CACHE_OFFSET_STRUCT *CacheOther;

  // Pointers used to help optimize code size by reducing identical
  // function calls
  REGISTER_CACHE_OFFSET_STRUCT **LowDWord;
  REGISTER_CACHE_OFFSET_STRUCT **HighDWord;

  UINT16                       Index;
  UINT16                       IndexOther;
  UINT32                       Offset;

  // Used for 64-bit registers, this stores the offset of the lower dword
  UINT32                       TrueOffset;
  UINT64_STRUCT                Value64;
  RegCacheStatus               State;

  Outputs       = &MrcData->Outputs;
  RegisterCache = &Outputs->RegisterCache;
  for (Index = 0; Index < MAX_REGISTER_CACHE_ENTRIES; Index++) {
    Cache = &RegisterCache->Data[Index];
    Offset = (Cache->Flags.Bits.OffsetMsb << 16) | Cache->Offset;
    if ((Cache->Flags.Bits.IsValid) && (Cache->Flags.Bits.IsDirty)) {
      // Flush a 64-bit register
      if (Cache->Flags.Bits.Is64BitReg) {
        if (Cache->Flags.Bits.IsHighDWord) {
          // The register entry found is the upper 32 bits. Need to find the lower 32 bits.
          Offset -= 4;
          TrueOffset = Offset;
          LowDWord = &CacheOther;
          HighDWord = &Cache;
        } else {
          // The register entry found is the lower 32 bits. Need to find the upper 32 bits.
          TrueOffset = Offset;
          Offset += 4;
          LowDWord = &Cache;
          HighDWord = &CacheOther;
        }

        // Find cache entry of the other half of the 64-bit register
        State = SeekCacheLocation (MrcData, &IndexOther, Offset);
        MRC_DEBUG_ASSERT((State == RegCacheSuccess),
                         &MrcData->Outputs.Debug,
                         "Only half of a 64-bit register is stored in the hash table");

        //Reconstruct the register and write it to hardware
        CacheOther = &RegisterCache->Data[IndexOther];
        Value64.Data32.Low = (*LowDWord)->Data;
        Value64.Data32.High = (*HighDWord)->Data;
        MrcWriteCR64 (MrcData, TrueOffset, Value64.Data);
        CacheOther->Flags.Bits.IsDirty = FALSE;
      } else { // Flush a 32-bit register entry
        MrcWriteCR (MrcData, Offset, Cache->Data);
      }
      Cache->Flags.Bits.IsDirty = FALSE;
    }
  }
  //Once Cache is flushed, SeekCacheLocation will be able to use any valid but not dirty location
  RegisterCache->Overflow = FALSE;
  return (mrcSuccess);
}
#if 0 // This code is used for register cache debug only.
#ifdef MRC_DEBUG_PRINT
/**
  Print the entire register data cache.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval mrcSuccess.
**/
MrcStatus
PrintRegisterCachedData (
  MrcParameters *const MrcData
  )
{
  MrcOutput                          *Outputs;
  const MRC_REGISTER_CACHE           *RegisterCache;
  const REGISTER_CACHE_OFFSET_STRUCT *Cache;
  MrcDebug                           *Debug;
  UINT16                             Index;
  UINT32                             FreeCount;

  Debug         = &MrcData->Outputs.Debug;
  Outputs       = &MrcData->Outputs;
  RegisterCache = &Outputs->RegisterCache;
  FreeCount     = 0;
  for (Index = 0; Index < MAX_REGISTER_CACHE_ENTRIES; Index++) {
    Cache = &RegisterCache->Data[Index];
    if (!Cache->Flags.Bits.IsValid) {
      FreeCount++;
    }
    MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        "%04X %04Xh = %08Xh %c%c%s%c\n",
        Index,
        (Cache->Flags.Bits.OffsetMsb << 16) | Cache->Offset,
        Cache->Data,
        (Cache->Flags.Bits.IsValid) ? 'v' : ' ',
        (Cache->Flags.Bits.IsDirty) ? 'd' : ' ',
        (Cache->Flags.Bits.Is64BitReg) ? "64" : "32",
        (Cache->Flags.Bits.IsHighDWord) ? 'H' : ' '
        );
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Register cache MaxValid = %u, FreeCount = %u\n", RegisterCache->MaxValid, FreeCount);
  if (RegisterCache->Overflow == TRUE) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Register cache overflow!\n");
  }
  return (mrcSuccess);
}
#endif
#endif

/**
  This function sets the current register value and updates corresponding cache location if update cache is enabled.

  @param[in] MrcData    - Pointer to global data.
  @param[in] Offset     - Register offset.
  @param[in] Is64BitReg - TRUE if Offset points to a 64-bit register, FALSE if Offset points to a 32-bit register.
  @param[in] Mode       - Bit-field flags controlling cache access.
  @param[in] Delay      - Delay in microseconds to invoke after hardware register write.
  @param[in] Value      - New register value.

  @retval mrcSuccess or mrcWrongInputParameter if cache update is enabled and there is no cache entry.
**/
MrcStatus
MrcCrCacheWrite (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset,
  IN const BOOLEAN Is64BitReg,
  IN const UINT32  Mode,
  IN const UINT32  Delay,
  IN const UINT64  Value
  )
{
  UINT64_STRUCT    Value64;
  BOOLEAN          IsDirty;
  MRC_REGISTER_CACHE *RegisterCache;

  MrcStatsIncrementData (MrcData, MRC_CACHED_WRITE_CR);

  Value64.Data = Value;
  IsDirty = ((Mode & GSM_FORCE_WRITE) != 0) || ((Mode & GSM_CACHE_ONLY) == 0) ? FALSE : TRUE;

  if ((Mode & GSM_UPDATE_CACHE) != 0) {
    MrcCrCacheUpdate (MrcData, Is64BitReg, IsDirty, Offset, Value);
  }

  RegisterCache = &MrcData->Outputs.RegisterCache;
  if (((Mode & GSM_FORCE_WRITE) != 0) || ((Mode & GSM_CACHE_ONLY) == 0) || (RegisterCache->Overflow == TRUE)) {
    if (Is64BitReg) {
      MrcWriteCR64 (MrcData, Offset, Value);
    } else {
      MrcWriteCR (MrcData, Offset, Value64.Data32.Low);
    }
    if (Delay != 0) {
      MrcWait (MrcData, Delay * MRC_TIMER_1US);
    }
  }
  return (mrcSuccess);
}

/**
  This function gets the current register value, either from the cache or from hardware if either cache reads are disabled or doesn't exist.

  @param[in] MrcData    - Pointer to global data.
  @param[in] Offset     - Register offset.
  @param[in] Is64BitReg - TRUE if Offset points to a 64-bit register, FALSE if Offset points to a 32-bit register.
  @param[in] Mode       - Bit-field flags controlling cache access.

  @retval The current register value, either from the cache or from hardware if either cache reads are disabled or doesn't exist.
**/
UINT64
MrcCrCacheRead (
  IN MrcParameters *const MrcData,
  IN const UINT32  Offset,
  IN const BOOLEAN Is64BitReg,
  IN const UINT32  Mode
  )
{
  MRC_REGISTER_CACHE *RegisterCache;
  RegCacheStatus     Status;
  UINT64_STRUCT      Value;
  UINT16             Index;
  BOOLEAN            Multicast;

  MrcStatsIncrementData (MrcData, MRC_CACHED_READ_CR);

  Multicast = FALSE;
  if ((Mode & GSM_READ_CSR) == 0) {
    if (Is64BitReg) {
      return GetRegisterCachedData64 (MrcData, Offset);
    } else {
      RegisterCache = &MrcData->Outputs.RegisterCache;
      Status        = SeekCacheLocation (MrcData, &Index, Offset);
      if (Status == RegCacheSuccess) {
        return (RegisterCache->Data[Index].Data);
      }
    }
  }

  Value.Data = Is64BitReg ? MrcReadCR64 (MrcData, Offset) : MrcReadCR (MrcData, Offset);
  if ((!Multicast && ((Mode & GSM_UPDATE_CACHE) != 0))) {
    SetCache32 (MrcData, Offset, Is64BitReg, FALSE, FALSE, Value.Data32.Low);
    if (Is64BitReg) {
      SetCache32 (MrcData, Offset + sizeof (UINT32), Is64BitReg, FALSE, TRUE, Value.Data32.High);
    }
  }
  return (Value.Data);
}
