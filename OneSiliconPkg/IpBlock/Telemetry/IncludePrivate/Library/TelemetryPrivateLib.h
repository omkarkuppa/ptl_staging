/** @file
  Header file for TelemetryPeiPrivateLib.

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
#ifndef TELEMETRY_PEI_PRIVATE_LIB_H_
#define TELEMETRY_PEI_PRIVATE_LIB_H_

#include <Library/TelemetryLib.h>
#include <TelemetryPeiConfig.h>

//
// Structure of Cpu CrashLog Mailbox Interface
//
typedef union {
  struct {
    UINT32  Command         :8;
    UINT32  Param           :8;
    UINT32  Reserved        :15;
    UINT32  Busy            :1;
  } Fields;
  UINT32 InterfaceData;
} CPU_CRASHLOG_MAILBOX;

  /**
  Send CPU CrashLog Mailbox command

  @param[in]  Command             Command request for crashLog management.
  @param[in]  Param               Parameter associated with the requested command.

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Mailbox command failed with an error
  @retval EFI_TIMEOUT             Mailbox command did not complete
**/
EFI_STATUS
CpuCrashLogMailboxCommand (
  IN  UINT8                Command,
  IN  UINT8                Param
  );

/**
  Get CPU CrashLog Main pointer data

  @param[in]    BaseAddr          Pci configuration space MMIO address of CrashLog BAR
  @param[out]   *CrashDataBuff    Pointer to CrashLog Main data buffer
**/
EFI_STATUS
CpuCrashLogMainDiscovery (
  IN   UINT32                 BaseAddr,
  OUT  CPU_CRASHLOG_BUFFER    *CrashDataBuff
  );

/**
  Get CPU CrashLog Telemetry pointer data

  @param[in]    BaseAddr          Pci configuration space MMIO address of CrashLog BAR
  @param[out]   *CrashDataBuff    Pointer to CrashLog Telemetry data buffer
**/
EFI_STATUS
CpuCrashLogTelemetryDiscovery (
  IN   UINT32                 BaseAddr,
  OUT  CPU_CRASHLOG_BUFFER    *CrashDataBuff
  );

/**
  Get CPU CrashLog Trace pointer data

  @param[in]    BaseAddr          Pci configuration space MMIO address of CrashLog BAR
  @param[out]   *CrashDataBuff    Pointer to CrashLog Trace data buffer
**/
EFI_STATUS
CpuCrashLogTraceDiscovery (
  IN   UINT32                 BaseAddr,
  OUT  CPU_CRASHLOG_BUFFER    *CrashDataBuff
  );

/**
  This function disables cpu crash log

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Mailbox command failed with an error
  @retval EFI_TIMEOUT             Mailbox command did not complete
**/
EFI_STATUS
CpuCrashLogDisable (
  VOID
  );

/**
  Save CPU CrashLog BAR for S3

**/

VOID
SaveCpuCrashLogBarForS3 (
  VOID
);
#endif
