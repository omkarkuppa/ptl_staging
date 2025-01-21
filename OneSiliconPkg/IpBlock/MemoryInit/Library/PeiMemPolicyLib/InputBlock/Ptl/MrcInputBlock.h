/** @file
  This file includes MRC input block related defines, structures and functions.

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

#ifndef MRC_INPUT_BLOCK_H_
#define MRC_INPUT_BLOCK_H_

#include "CMrcInterface.h"  // for MrcParameters
#include "Ptl/CMrcExtInputs.h"

#pragma pack(push, 1)
typedef struct {
  UINT64 Sig0;
  UINT64 Sig1;
} MRC_16_BYTE_SIGNATURE_TYPE;

#ifdef MRC_MINIBIOS_BUILD
typedef struct {
  BOOLEAN             LpFreqSwitch;
  UINT32              ErrorCountForFail;
  UINT8               BER;
  BOOLEAN             TxtClean;
  BOOLEAN             EnablePda;
  BOOLEAN             Lp58BankMode;
  BOOLEAN             ReadMprVA;
  BOOLEAN             IsPmaMemConfigDoneSkip;
  MRC_CCE_CONFIG      PmaCceConfig;
  BOOLEAN             IsCs2NRequested;
  UINT8               MinAllowedNModeOvrd;
  BOOLEAN             SenseAtRxDll;
  BOOLEAN             LockUiDiv6Flow;
  BOOLEAN             MptuPropagationErrorFlow;
  UINT16              RloadTarget;
  BOOLEAN             DiscardLvrAutoTrimResults;
  BOOLEAN             PhClkSkipPhCorrection;
#ifdef HVM_MODE
  UINT8               PhClkCheckPhError;
  UINT8               PhClkCheckDcError;
#endif // HVM_MODE
} MRC_INT_INPUTS_TYPE;
#endif

typedef struct {
  /// Used as ASCII String Identifier to locate the whole
  /// MRC_INPUT_BINARY_BLOCK_TYPE when compiled as binary.
  MRC_16_BYTE_SIGNATURE_TYPE Signature;

  /// Contains the structure of Inputs that are exposed to
  /// BIOS Setup / FSPM UPD and must follow binary backwards
  /// compatibility model defined by BIOS.
  MRC_EXT_INPUTS_TYPE        ExternalInputs;

#ifdef MRC_MINIBIOS_BUILD
  /// Contains members defined in MrcInput that are not exposed
  /// to BIOS but also critical to expose for MiniBios validation.
  MRC_INT_INPUTS_TYPE        InternalInputs;
#endif

  /// Expected to be initialized with 0xDEADBEEF as the end of struct
  /// so that code can detect incomplete initialization of the struct
  /// when using original K&R structure initialization.
  UINT32                     EndMarker;
} MRC_INPUT_BINARY_BLOCK_TYPE;
#pragma pack(pop)

#define MRC_INPUT_BINARY_BLOCK_END                     (0xDEADBEEF)

extern MRC_INPUT_BINARY_BLOCK_TYPE MainInputs;

#ifdef MRC_MINIBIOS_BUILD
/**
  Copy over static data of structure MRC_INPUT_BINARY_BLOCK_TYPE(MainInputs) to the pointer passed in the address of the MrcInput data on the stack

  @param[out] Inputs - the address of the MrcInput data on the stack.
**/
VOID
MrcInputsDefaultInit (
  OUT MrcInput *Inputs
  );

#endif // MRC_MINIBIOS_BUILD

/**
  Copy over static data structure MainInputs to the pointer passed in the address of the ExternalInputs

  @param[out] ExternalInputsPtr - the address of the ExternalInputs.

**/
VOID
InitExternalInputsDefault (
  OUT MRC_EXT_INPUTS_TYPE *ExternalInputsPtr
  );

#ifdef MRC_MINIBIOS_BUILD
/**
 * Print External Inputs as a binary dump for verification.
 * 
 * @param[in] ExtInputs - the address of the External Inputs
 * @param[in] Debug     - Debug print
 */
VOID
ExtInputBinaryDump (
  IN MRC_EXT_INPUTS_TYPE *ExtInputs,
  IN MrcDebug            *Debug
  );

#endif // MRC_MINIBIOS_BUILD

#endif // MRC_INPUT_BLOCK_H_
