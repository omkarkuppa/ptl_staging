/** @file
  HECI Transport driver core operations for use in PEI, DXE and SMM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _HECI_TRANSPORT_CORE_H_
#define _HECI_TRANSPORT_CORE_H_

#include <Register/HeciRegs.h>

#define GET_INT_ENABLE(Csr32)               ((Csr32 & B_ME_CSR_INT_ENABLE_MASK) >> N_ME_CSR_INT_ENABLE_SHIFT)
#define SET_INT_ENABLE(Csr32, Value)        Csr32 = (((Value << N_ME_CSR_INT_ENABLE_SHIFT) & B_ME_CSR_INT_ENABLE_MASK) | (Csr32 & ~B_ME_CSR_INT_ENABLE_MASK))
#define GET_INT_STATUS(Csr32)               ((Csr32 & B_ME_CSR_INT_STATUS_MASK) >> N_ME_CSR_INT_STATUS_SHIFT)
#define SET_INT_STATUS(Csr32, Value)        Csr32 = (((Value << N_ME_CSR_INT_STATUS_SHIFT) & B_ME_CSR_INT_STATUS_MASK) | (Csr32 & ~B_ME_CSR_INT_STATUS_MASK))
#define GET_INT_GENERATE(Csr32)             ((Csr32 & B_ME_CSR_INT_GENERATE_MASK) >> N_ME_CSR_INT_GENERATE_SHIFT)
#define SET_INT_GENERATE(Csr32, Value)      Csr32 = (((Value << N_ME_CSR_INT_GENERATE_SHIFT) & B_ME_CSR_INT_GENERATE_MASK) | (Csr32 & ~B_ME_CSR_INT_GENERATE_MASK))
#define GET_READY(Csr32)                    ((Csr32 & B_ME_CSR_READY_MASK) >> N_ME_CSR_READY_SHIFT)
#define SET_READY(Csr32, Value)             Csr32 = (((Value << N_ME_CSR_READY_SHIFT) & B_ME_CSR_READY_MASK) | (Csr32 & ~B_ME_CSR_READY_MASK))
#define GET_RESET(Csr32)                    ((Csr32 & B_ME_CSR_RESET_MASK) >> N_ME_CSR_RESET_SHIFT)
#define SET_RESET(Csr32, Value)             Csr32 = (((Value << N_ME_CSR_RESET_SHIFT) & B_ME_CSR_RESET_MASK) | (Csr32 & ~B_ME_CSR_RESET_MASK))
#define GET_CB_READ_POINTER(Csr32)          ((Csr32 & B_ME_CSR_READ_POINTER_MASK) >> N_ME_CSR_READ_POINTER_SHIFT)
#define GET_CB_WRITE_POINTER(Csr32)         ((Csr32 & B_ME_CSR_WRITE_POINTER_MASK) >> N_ME_CSR_WRITE_POINTER_SHIFT)
#define GET_CB_DEPTH(Csr32)                 ((Csr32 & B_ME_CSR_DEPTH_MASK) >> N_ME_CSR_DEPTH_SHIFT)

#define MAX_CB_DEPTH_SIMICS                 32

#endif // _HECI_TRANSPORT_CORE_H_
