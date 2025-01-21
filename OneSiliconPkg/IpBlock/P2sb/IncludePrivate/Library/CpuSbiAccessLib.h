/** @file
  Header file for CpuSbiAccessLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _CPU_SBI_ACCESS_LIB_H_
#define _CPU_SBI_ACCESS_LIB_H_

#include <Library/CpuRegbarAccessLib.h>
#include <Library/P2SbSidebandAccessLib.h>

/**
  CPU SBI opcode definitions
**/
typedef P2SB_SBI_OPCODE  CPU_SBI_OPCODE;

/**
  CPU SBI response status definitions
**/
typedef P2SB_SBI_RESPONSE  CPU_SBI_RESPONSE;

/**
  Execute CPU SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the CpuSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal IOM and hide IOM if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the IOM before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
  @retval EFI_TIMEOUT                   Timeout while waiting for response
**/
EFI_STATUS
EFIAPI
CpuSbiExecution (
  IN     CPU_SB_DEVICE_PID              Pid,
  IN     UINT64                         Offset,
  IN     CPU_SBI_OPCODE                 Opcode,
  IN     BOOLEAN                        Posted,
  IN     UINT16                         Fid,
  IN OUT UINT32                         *Data32,
  OUT    UINT8                          *Response
  );

/**
  Full function for executing CPU SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the CpuSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal IOM and hide IOM if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the IOM before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in] Fbe                        First byte enable
  @param[in] Bar                        Bar
  @param[in] Fid                        Function ID
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
  @retval EFI_TIMEOUT                   Timeout while waiting for response
**/
EFI_STATUS
EFIAPI
CpuSbiExecutionEx (
  IN     CPU_SB_DEVICE_PID              Pid,
  IN     UINT64                         Offset,
  IN     CPU_SBI_OPCODE                 Opcode,
  IN     BOOLEAN                        Posted,
  IN     UINT16                         Fbe,
  IN     UINT16                         Bar,
  IN     UINT16                         Fid,
  IN OUT UINT32                         *Data32,
  OUT    UINT8                          *Response
  );

/**
  Builds CPU SBI access.

  @param[in]  Pid                           P2SB PID
  @param[in]  Fid                           P2SB FID
  @param[in]  RegisterSpace                 Register space to access
  @param[in]  AccessMethod                  Access method
  @param[out] CpuSbiSidebandRegisterAccess  Structure to initialize

  @retval TRUE   Initialized successfully
  @retval FALSE  Failed to initialize
**/
BOOLEAN
BuildCpuSbiAccess (
  IN  P2SB_PID                       Pid,
  IN  UINT16                         Fid,
  IN  P2SB_REGISTER_SPACE            RegisterSpace,
  IN  P2SB_SIDEBAND_ACCESS_METHOD    AccessMethod,
  OUT P2SB_SIDEBAND_REGISTER_ACCESS  *CpuSbiSidebandRegisterAccess
  );

#endif // _CPU_SBI_ACCESS_LIB_H_
