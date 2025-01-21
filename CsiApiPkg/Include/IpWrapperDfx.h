/** @file
  IP Wrapper (IPWR) Dfx functions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IP_WRAPPER_DFX_H_
#define IP_WRAPPER_DFX_H_

  #include "IpCsi.h"

// IMPORTANT:
// - all CSI and Wrapper types must be defined in IpCsi.h.  Function declarations will continue
//   to be defined in the appropriate H files
// - please see comments in IpCsi.h for further explanation

/**
  Set default requester SAI value

  @param[in]  pCntxt    IP Wrapper defined DFx group context. Opaque to CSI FW.
  @param[in]  Sai       IP_WR_DFX_SAI 8 bit SAI value used in future reg access.
                        It is is NOT the 5 bit SAI index, and is not supported by all IP Wrappers.

  @return     A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpWrDfxSetSai (
  IP_WR_DFX_CNTXT  pCntxt,
  IP_WR_DFX_SAI    Sai
  );

/**
  Get default requester SAI value

  @param[in]  pCntxt    IP Wrapper defined DFx group context. Opaque to CSI FW.

  @return     IP_WR_DFX_SAI 8 bit SAI value used in future reg access.
                        It is is NOT the 5 bit SAI index, and is not supported by all IP Wrappers.
**/
IP_WR_DFX_SAI
IpWrDfxGetSai (
  IP_WR_DFX_CNTXT  pCntxt
  );

/**
  Trigger IP Wrapper specific debug capture  (@todo: better description)

  @param[in]  pCntxt    IP Wrapper defined DFx group context. Opaque to CSI FW.

  @return     A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpWrDfxDebugCapture (
  IP_WR_DFX_CNTXT  pCntxt
  );

/**
  Trigger IP Wrapper specific pause execution  (@todo: better description)

  @param[in]  pCntxt    IP Wrapper defined DFx group context. Opaque to CSI FW.

  @return     A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpWrDfxPauseExec (
  IP_WR_DFX_CNTXT  pCntxt
  );

/**
  A verbose Register read, that will print additional DFX information during the register access.

  @param[in]  pCntxt      IP Wrapper defined register group context. Opaque to CSI FW.
  @param[in]  Offset      Byte offset of the register.
  @param[in]  RegFlags    OR'ed values of IP_WR_REG_FLAGS. At least "IpWrRegFlagSize*" is required for size
  @param[in]  RegName     OPTIONAL register string name (may be NULL).

  @return     The data read from the register.
**/
UINT64
IpWrDfxRegRead (
  IP_WR_REG_CNTXT  pCntxt,
  UINT32           Offset,
  UINT32           RegFlags,
  const char       *RegName
  );

/**
  A verbose Register write, that will print additional DFX information during the register access.

  @param[in]  pCntxt      IP Wrapper defined register group context. Opaque to CSI FW.
  @param[in]  offset      Byte offset of the register.
  @param[in]  Data        Data to be written
  @param[in]  RegFlags    OR'ed values of IP_WR_REG_FLAGS. at least "IpWrRegFlagSize*" is required for size
  @param[in]  RegName     OPTIONAL register string name (may be NULL).

  @return     The data that was written to the register.
**/
UINT64
IpWrDfxRegWrite (
  IP_WR_REG_CNTXT  pCntxt,
  UINT32           Offset,
  UINT64           Data,
  UINT32           RegFlags,
  const char       *RegName
  );

/**
  A verbose Register field access (@todo: better description)

  @param[in]  FldName     OPTIONAL field string name (may be NULL)
  @param[in]  Data        Data in Field

  @return     Data
**/
UINT64
IpWrDfxFldData (
  const char  *FldName,
  UINT64      Data
  );

#endif //IP_WRAPPER_DFX_H_

#ifdef __cplusplus
}
#endif
