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

#ifndef _MrcHalCore_h_
#define _MrcHalCore_h_

#include "MrcHalRegisterAccess.h"
#include "MrcCommon.h"

/**
  This function gets block partition index range for the specified partition type and index.
  If index is multicast then the range is for all instances of the partition.

  @param[in] PartitionType - The partition type.
  @param[in] Index         - The partition index.

  @returns The partition index range.
**/
MRC_RANGE
MrcGetPartitionIndexRange (
  IN MrcParameters *const MrcData,
  IN const PARTITION_TYPE PartitionType,
  IN const UINT32         Index
  );

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
  );

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
  );

/**
  This function sets the specified register bit field.

  @param[in] HashIn        - The bit field hash value.
  @param[in] BitFieldValue - The bit field value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
extern
UINT32
MrcHalSetBitField32 (
  IN const UINT32 HashIn,
  IN const UINT32 BitfieldValue,
  IN const UINT32 RegisterValue
  );

/**
  This function sets the specified register bit field.

  @param[in] HashIn        - The bit field hash value.
  @param[in] BitFieldValue - The bit field value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
extern
UINT32
MrcHalSetBitFieldS32 (
  IN const UINT32 HashIn,
  IN const INT32  BitfieldValue,
  IN const UINT32 RegisterValue
  );

/**
  This function gets the specified register bit field.

  @param[in] Group         - DDRIO group to access.
  @param[in] HashIn        - The bit field hash value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
extern
INT32
MrcHalGsmGetBitField32 (
  IN const GSM_GT Group,
  IN const UINT32 HashIn,
  IN const UINT32 RegisterValue
);

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
  );

/**
  This function gets the specified register bit field.

  @param[in] HashIn        - The bit field hash value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
extern
INT32
MrcHalGetBitFieldS32 (
  IN const UINT32 HashIn,
  IN const UINT32 RegisterValue
  );

/**
  This function sets the specified register bit field.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] HashIn        - The bit field hash value.
  @param[in] BitFieldValue - The bit field value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
extern
UINT64
MrcHalSetBitField64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  HashIn,
  IN const UINT64  BitfieldValue,
  IN const UINT64  RegisterValue
  );

/**
  This function sets the specified register bit field.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] HashIn        - The bit field hash value.
  @param[in] BitFieldValue - The bit field value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
extern
UINT64
MrcHalSetBitFieldS64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  HashIn,
  IN const INT64   BitfieldValue,
  IN const UINT64  RegisterValue
  );

/**
  This function gets the specified register bit field.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] HashIn        - The bit field hash value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
extern
UINT64
MrcHalGetBitField64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  HashIn,
  IN const UINT64  RegisterValue
  );

/**
  This function gets the specified register bit field.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] HashIn        - The bit field hash value.
  @param[in] RegisterValue - The register value to write.

  @retval The value written to the register.
**/
extern
INT64
MrcHalGetBitFieldS64 (
  IN MrcParameters *const MrcData,
  IN const UINT32  HashIn,
  IN const UINT64  RegisterValue
  );


#endif // _MrcHalCore_h_
