/** @file
  Header file for PchPcrLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _PCH_PCR_LIB_H_
#define _PCH_PCR_LIB_H_

#include <PcrDefine.h>

/**
  Read PCR register.
  It returns PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID

  @retval UINT32       PCR register value.
**/
UINT32
PchPcrRead32 (
  IN  P2SB_PID                          Pid,
  IN  UINT32                            Offset
  );

/**
  Read PCR register.
  It returns PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID

  @retval UINT16       PCR register value.
**/
UINT16
PchPcrRead16 (
  IN  P2SB_PID                          Pid,
  IN  UINT32                            Offset
  );

/**
  Read PCR register.
  It returns PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID

  @retval UINT8        PCR register value
**/
UINT8
PchPcrRead8 (
  IN  P2SB_PID                          Pid,
  IN  UINT32                            Offset
  );

/**
  Write PCR register.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  Data     Input Data. Must be the same size as Size parameter.

  @retval UINT32       Value written to register
**/
UINT32
PchPcrWrite32 (
  IN  P2SB_PID                          Pid,
  IN  UINT32                            Offset,
  IN  UINT32                            InData
  );

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  Data     Input Data. Must be the same size as Size parameter.

  @retval  UINT16      Value written to register
**/
UINT16
PchPcrWrite16 (
  IN  P2SB_PID                          Pid,
  IN  UINT32                            Offset,
  IN  UINT16                            InData
  );

/**
  Write PCR register.
  It programs PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  Data     Input Data. Must be the same size as Size parameter.

  @retval  UINT8       Value written to register
**/
UINT8
PchPcrWrite8 (
  IN  P2SB_PID                          Pid,
  IN  UINT32                            Offset,
  IN  UINT8                             InData
  );

/**
  Write PCR register.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval  UINT32      Value written to register

**/
UINT32
PchPcrAndThenOr32 (
  IN  P2SB_PID                          Pid,
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  );

/**
  Write PCR register and read back.
  The read back ensures the PCR cycle is completed before next operation.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval  UINT32      Value read back from the register
**/
UINT32
PchPcrAndThenOr32WithReadback (
  IN  P2SB_PID                          Pid,
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  );

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval UINT16       Value written to register

**/
UINT16
PchPcrAndThenOr16 (
  IN  P2SB_PID                          Pid,
  IN  UINT32                            Offset,
  IN  UINT16                            AndData,
  IN  UINT16                            OrData
  );

/**
  Write PCR register.
  It programs PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval  UINT8       Value written to register

**/
UINT8
PchPcrAndThenOr8 (
  IN  P2SB_PID                          Pid,
  IN  UINT32                            Offset,
  IN  UINT8                             AndData,
  IN  UINT8                             OrData
  );

#endif // _PCH_PCR_LIB_H_
