/** @file
  This file contains all the MRC general API to the MRC wrapper.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#include "MrcGeneral.h"
#include "MrcMaintenance.h"
#include "MrcMemoryApi.h"
#include "MrcLpddr5.h"
#include "MrcDdr5.h"
#include "CMrcStartMemoryConfiguration.h"
#include "MrcLpddr5Registers.h"
#include "MrcRegisterCache.h"

const MrcVersionWithSignature cVersion = {
  0x5F52455643524D5FULL,    // SIGNATURE_64 ('_','M','R','C','V','E','R','_');
  #include "MrcVersion.h"
};

// Number of "b" in y=mx+b linear calculation equation for VF curve, "m" is scaled to 100 due to decimal numbers usage
#define VDDQ_VF_CURVE_LINEAR_POINT_700 ((VDD_0_70)-(((((VDD_0_70 - VDD_0_65) * 100) / (f4267 - f2667)) * f4267) / 100))
#define VDDQ_VF_CURVE_LINEAR_POINT_800 ((VDD_0_80)-(((((VDD_0_80 - VDD_0_70) * 100) / (f5400 - f4267)) * f5400) / 100))

typedef enum {
  LowBwSaGv,         ///< Low Bandwidth
  MidBwSaGv,         ///< Medium Bandwidth
  HighBwSaGv,        ///< High Bandwidth
  LowLatencySaGv,    ///< Low Latency
  RfiHighBwSaGv,     ///< High Bandwidth RFI Mitigation
  RfiLowLatencySaGv, ///< Low Latency RFI Mitigation
  SaGvPointTypeMax,
} SaGvTypes;

/**
  Determine if SaGv is enabled from SaGv Input.

  @param[in] MrcData - include all the MRC general data.

  @retval TRUE  - If SaGv is enabled.
          FALSE - If SaGv is disabled.
**/
BOOLEAN
MrcIsSaGvEnabled (
  IN  MrcParameters *const  MrcData
)
{
  const MrcInput  *Inputs;

  Inputs  = &MrcData->Inputs;

  if (Inputs->ExtInputs.Ptr->SaGv == MRC_SAGV_ENABLED) {
    return TRUE;
  }

  return FALSE;
}

#ifdef MRC_DEBUG_PRINT
/**
  Print the specified memory to the serial message debug port.

  @param[in] Debug - Serial message debug structure.
  @param[in] Start - The starting address to dump.
  @param[in] Size  - The amount of data in bytes to dump.

  @retval Nothing.
**/
void
MrcPrintMemory (
  IN MrcDebug *const    Debug,
  IN const UINT8 *const Start,
  IN const UINT32       Size
  )
{
  const UINT8  *Address;
  const UINT8  *End;
  UINT32       Line;
  UINT32       Offset;
  union {
    UINT64     Data64[2];
    UINT32     Data32[4];
    UINT16     Data16[8];
    UINT8      Data8[16];
  } Buffer;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "          ");
  for (Offset = 0; Offset < 16; Offset++) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%02" PRIXMAX " ", ((UINTN) Start + Offset) % 16);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
  End = Start + Size;
  for (Line = 0; Line < ((Size / 16) + 1); Line++) {
    Address = Start + (Line * 16);
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%p: ", Address);
    for (Offset = 0; Offset < 16; Offset++) {
      Buffer.Data8[Offset] = ((Address + Offset) < End) ? Address[Offset] : 0;
    }
    for (Offset = 0; Offset < 16; Offset++) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, ((Address + Offset) < End) ? "%02X " : "   ", Buffer.Data8[Offset]);
    }
    for (Offset = 0; (Offset < 16) && ((Address + Offset) < End); Offset++) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%c", MRC_ISPRINT (Buffer.Data8[Offset]) ? Buffer.Data8[Offset] : '.');
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
  }
}
#endif

/**
  This function return the MRC version.

  @param[in] MrcData - Include all MRC global data.
  @param[out] Version - Location to store the MRC version.

  @retval Nothing.
**/
void
MrcVersionGet (
  IN  const MrcParameters* const MrcData,
  OUT MrcVersion* const Version
  )
{
  const MrcInput* Inputs;
  const MRC_FUNCTION* MrcCall;

  if (Version != NULL) {
    Inputs = &MrcData->Inputs;
    MrcCall = Inputs->Call.Func;
    MrcCall->MrcCopyMem ((UINT8*)Version, (UINT8*)&cVersion.Version, sizeof (MrcVersion));
  }
}

/**
  Preforms an unsigned rounded division operation of size UINT64.
  Follows this formula: (a, b)  (((a) + (b) / 2) / (b))
  @param[in] MrcData     - The Mrc Host data structure
  @param[in] Var1        - Numerator
  @param[in] Var2        - Denominator

  @retval Rounded value from UINT64 division
**/
UINT64
UnsignedDivideRoundx64 (
  IN MrcParameters   *MrcData,
  IN UINT64          Var1,
  IN UINT64          Var2
  )
{
  const MrcInput *Inputs;
  const MRC_FUNCTION *MrcCall;
  UINT64 Temp;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;

  Temp = MrcCall->MrcDivU64x64 (Var2, 2, NULL);
  return MrcCall->MrcDivU64x64 (Temp + Var1, Var2, NULL);
}
