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
#include "MrcHalInternal.h" // for function prototypes and interface definitions

#define MUTLICAST_NOT_SUPPORTED (0)

static const UINT8 BlockPartitionMaxRange[MaxPartition] = {
  [PartitionDataShared] = MRC_DATA_SHARED_NUM,
  [PartitionData]       = MRC_DATA_MOBILE_NUM * MRC_DATA_CH_NUM,
  [PartitionCcc]        = MRC_CCC_NUM,
  [PartitionCccShared]  = MRC_CCC_SHARED_NUM,
  [PartitionPg]         = MRC_PG_NUM,
};

/**
  This function gets block partition index range for the specified partition type and index.
  If index is multicast then the range is for all instances of the partition.

  @param[in] PartitionType - The partition type.
  @param[in] Index         - The partition index.

  @returns The partition index range.
**/
MRC_RANGE
MrcGetPartitionIndexRange (
  IN const PARTITION_TYPE PartitionType,
  IN const UINT32         Index
  )
{
  MRC_RANGE Range = {.Start = MRC_IGNORE_ARG_8, .End = MRC_IGNORE_ARG_8};
  BOOLEAN IsPartitionSupported = (BlockPartitionMaxRange[PartitionType] != MUTLICAST_NOT_SUPPORTED);

  if (IsPartitionSupported) {
    if (BlockPartitionMaxRange[PartitionType] == Index) {
      // This is multicast
      Range.Start = 0;
      Range.End = BlockPartitionMaxRange[PartitionType];
    } else {
      Range.Start = Index;
      Range.End   = Index + 1;
    }
  }

  return Range;
}

/**
  This function sets the specified register bit field.

  @param[in] HashIn        - The bit field hash value.
  @param[in] BitFieldValue - The bit field value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
UINT32
MrcHalSetBitField32 (
  IN const UINT32 HashIn,
  IN const UINT32 BitfieldValue,
  IN const UINT32 RegisterValue
  )
{
  UINT32 Mask;
  UINT32 BfOffset;
  UINT32 BfWidth;
  MRC_REGISTER_HASH_STRUCT Hash;

  Hash.Data = HashIn;
  BfOffset  = Hash.Bits.BfOffset;
  BfWidth   = Hash.Bits.BfWidth;
  Mask      = ((BfWidth >= 32) ? (0xFFFFFFFFUL) : ((1UL << BfWidth) - 1)) << BfOffset;
  return ((RegisterValue & ~Mask) | ((BitfieldValue << BfOffset) & Mask));
}

/**
  This function sets the specified register bit field.

  @param[in] HashIn        - The bit field hash value.
  @param[in] BitFieldValue - The bit field value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
UINT32
MrcHalSetBitFieldS32 (
  IN const UINT32 HashIn,
  IN const INT32  BitfieldValue,
  IN const UINT32 RegisterValue
  )
{
  return (MrcHalSetBitField32 (HashIn, BitfieldValue, RegisterValue));
}

/**
  This function will determine if the group access is signed or unsigned and take care of sign extension.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] HashIn        - The bit field hash value.
  @param[in] RegisterValue - The register value to modify.

  @retval The register bit field value.
**/
INT64
MrcHalGsmGetBitField (
  IN MrcParameters *const MrcData,
  IN const MRC_REGISTER_HASH_STRUCT HashIn,
  IN const UINT64 RegisterValue
  )
{
  UINT64_STRUCT Value;

  Value.Data = RegisterValue;
  if (HashIn.Bits.RegSize == 0) {
    if (HashIn.Bits.BfSign == 0) {
      return (MrcHalGetBitField32 (HashIn.Data, Value.Data32.Low));
    } else {
      return (MrcHalGetBitFieldS32 (HashIn.Data, Value.Data32.Low));
    }
  } else {
    if (HashIn.Bits.BfSign == 0) {
      return (MrcHalGetBitField64 (MrcData, HashIn.Data, Value.Data));
    } else {
      return (MrcHalGetBitFieldS64 (MrcData, HashIn.Data, Value.Data));
    }
  }
}

/**
  This function will determine if the group access is signed or unsigned and take care of sign extension.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] HashIn        - The bit field hash value.
  @param[in] BitFieldValue - The bit field value to modify.
  @param[in] RegisterValue - The register value to modify.

  @retval The updated register.
**/
UINT64
MrcHalGsmSetBitField (
  IN MrcParameters *const MrcData,
  IN const MRC_REGISTER_HASH_STRUCT HashIn,
  IN const INT64  BitFieldValue,
  IN const UINT64 RegisterValue
  )
{
  IN INT64_STRUCT BfValue;
  IN UINT64_STRUCT Value;

  BfValue.Data = BitFieldValue;
  Value.Data = RegisterValue;
  if (HashIn.Bits.RegSize == 0) {
    if (HashIn.Bits.BfSign == 0) {
      return (MrcHalSetBitField32 (HashIn.Data, BfValue.Data32.Low, Value.Data32.Low));
    } else {
      return (MrcHalSetBitFieldS32 (HashIn.Data, BfValue.Data32.Low, Value.Data32.Low));
    }
  } else {
    if (HashIn.Bits.BfSign == 0) {
      return (MrcHalSetBitField64 (MrcData, HashIn.Data, BfValue.Data, Value.Data));
    } else {
      return (MrcHalSetBitFieldS64 (MrcData, HashIn.Data, BfValue.Data, Value.Data));
    }
  }
}

/**
  This function gets the specified register bit field.

  @param[in] HashIn        - The bit field hash value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
UINT32
MrcHalGetBitField32 (
  IN const UINT32 HashIn,
  IN const UINT32 RegisterValue
  )
{
  UINT32 BfWidth;
  MRC_REGISTER_HASH_STRUCT Hash;

  Hash.Data = HashIn;
  BfWidth   = Hash.Bits.BfWidth;
  return ((RegisterValue >> Hash.Bits.BfOffset) & ((BfWidth >= 32) ? (0xFFFFFFFFUL) : ((1UL << BfWidth) - 1)));
}

/**
  This function gets the specified register bit field.

  @param[in] HashIn        - The bit field hash value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
INT32
MrcHalGetBitFieldS32 (
  IN const UINT32 HashIn,
  IN const UINT32 RegisterValue
  )
{
  UINT32 Value;
  UINT32 BfWidth;
  MRC_REGISTER_HASH_STRUCT Hash;

  Hash.Data = HashIn;
  Value     = MrcHalGetBitField32 (HashIn, RegisterValue);
  BfWidth   = Hash.Bits.BfWidth;
  if (BfWidth < 32) {
    if (Value & (1 << (BfWidth - 1))) {
      Value |= ~((1 << BfWidth) - 1);
    }
  }
  return (Value);
}

/**
  This function sets the specified register bit field.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] HashIn        - The bit field hash value.
  @param[in] BitFieldValue - The bit field value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
UINT64
MrcHalSetBitField64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  HashIn,
  IN const UINT64  BitfieldValue,
  IN const UINT64  RegisterValue
  )
{
  MRC_FUNCTION *Func;
  UINT64 Mask;
  UINT32 BfOffset;
  UINT32 BfWidth;
  MRC_REGISTER_HASH_STRUCT Hash;

  Hash.Data = HashIn;
  BfWidth   = Hash.Bits.BfWidth;
  if (BfWidth >= 64) {
    return (BitfieldValue);
  }
  BfOffset = Hash.Bits.BfOffset;
  Func     = MrcData->Inputs.Call.Func;
  Mask     = Func->MrcLeftShift64 ((Func->MrcLeftShift64 (1ULL, BfWidth) - 1), BfOffset);
  return ((RegisterValue & ~Mask) | (Func->MrcLeftShift64 (BitfieldValue, BfOffset) & Mask));
}

/**
  This function sets the specified register bit field.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] HashIn        - The bit field hash value.
  @param[in] BitFieldValue - The bit field value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
UINT64
MrcHalSetBitFieldS64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  HashIn,
  IN const INT64   BitfieldValue,
  IN const UINT64  RegisterValue
  )
{
  return (MrcHalSetBitField64 (MrcData, HashIn, BitfieldValue, RegisterValue));
}

/**
  This function gets the specified register bit field.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] HashIn        - The bit field hash value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
UINT64
MrcHalGetBitField64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  HashIn,
  IN const UINT64  RegisterValue
  )
{
  MRC_FUNCTION *Func;
  UINT32 BfWidth;
  MRC_REGISTER_HASH_STRUCT Hash;

  Hash.Data = HashIn;
  BfWidth   = Hash.Bits.BfWidth;
  if (BfWidth >= 64) {
    return (RegisterValue);
  }
  Func = MrcData->Inputs.Call.Func;
  return ((Func->MrcRightShift64 (RegisterValue, Hash.Bits.BfOffset)) & (Func->MrcLeftShift64 (1ULL, BfWidth) - 1));
}

/**
  This function gets the specified register bit field.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] HashIn        - The bit field hash value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
INT64
MrcHalGetBitFieldS64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  HashIn,
  IN const UINT64  RegisterValue
  )
{
  MRC_FUNCTION *Func;
  UINT64 Value;
  UINT64 Sign;
  UINT32 BfWidth;
  MRC_REGISTER_HASH_STRUCT Hash;

  Value     = MrcHalGetBitField64 (MrcData, HashIn, RegisterValue);
  Hash.Data = HashIn;
  BfWidth   = Hash.Bits.BfWidth;
  if (BfWidth < 64) {
    Func = MrcData->Inputs.Call.Func;
    Sign = Func->MrcLeftShift64 (1ULL, BfWidth - 1);
    if (Value & Sign) {
      Value |= ~(Func->MrcLeftShift64 (1ULL, BfWidth) - 1);
    }
  }
  return (Value);
}
