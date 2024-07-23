/** @file

  Telemetry Private Library

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

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Register/TelemetryRegs.h>
#include <Library/TelemetryLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/TelemetryPrivateLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PeiTelemetryFruLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Defines/HostBridgeDefines.h>

/**
  Poll the run/busy bit of the mailbox until available or timeout expires.

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Mailbox command failed with an error
  @retval EFI_TIMEOUT             Mailbox command did not complete
**/
EFI_STATUS
CpuCrashLogPollMailboxReady (
  IN UINT32                  CrashLogMailboxAddress
  )
{
  CPU_CRASHLOG_MAILBOX       CrashLogMailboxInterface;
  UINT16                     StallCount;

  StallCount = 0;

  do {
    CrashLogMailboxInterface.InterfaceData = MmioRead32 (CrashLogMailboxAddress);

    MicroSecondDelay (CPU_CRASHLOG_MAILBOX_WAIT_STALL);
    StallCount++;
  } while ((CrashLogMailboxInterface.Fields.Busy == 1) && (StallCount < CPU_CRASHLOG_MAILBOX_WAIT_TIMEOUT));

  if ((CrashLogMailboxInterface.Fields.Busy == 1) && (StallCount >= CPU_CRASHLOG_MAILBOX_WAIT_TIMEOUT)) {
    DEBUG ((DEBUG_ERROR, "CPU CrashLog Mailbox timed out.\n"));
    return EFI_TIMEOUT;
  }
  return EFI_SUCCESS;
}

/**
  Send CPU CrashLog Mailbox command

  @param[in]  Command             Command request for CrashLog management.
  @param[in]  Param               Parameter associated with the requested command.

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Mailbox command failed with an error
  @retval EFI_TIMEOUT             Mailbox command did not complete
**/
EFI_STATUS
CpuCrashLogMailboxCommand (
  IN  UINT8                Command,
  IN  UINT8                Param
  )
{
  EFI_STATUS               Status;
  CPU_CRASHLOG_MAILBOX     CrashLogMailboxInterface;
  UINT32                   CrashLogBaseAddress;
  TEL_CRASHLOG_DEVSC_CAP   CrashLogDevscCap;

  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "CpuCrashLogMailboxCommand(): COMMAND=0x%02X, PARAM=0x%02X\n", Command, Param));

  ZeroMem (&CrashLogDevscCap, sizeof (TEL_CRASHLOG_DEVSC_CAP));
  Status = GetCpuCrashLogCapability (&CrashLogDevscCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CPU CrashLog feature is not supported\n"));
    return Status;
  }
  CrashLogBaseAddress = GetCpuCrashLogBarAddress (&CrashLogDevscCap);
  DEBUG ((DEBUG_INFO, "CrashLogBaseAddress = 0x%X\n", CrashLogBaseAddress));
  CrashLogMailboxInterface.Fields.Command = Command;
  CrashLogMailboxInterface.Fields.Param = Param;
  CrashLogMailboxInterface.Fields.Busy = 1;

  MmioWrite32 (CrashLogBaseAddress + R_CRASHLOG_MAILBOX_INTERFACE_ADDRESS, CrashLogMailboxInterface.InterfaceData);

  CpuCrashLogPollMailboxReady (CrashLogBaseAddress + R_CRASHLOG_MAILBOX_INTERFACE_ADDRESS);
  return Status;
}

/**
  Get CPU CrashLog Main pointer data

  @param[in]    BaseAddr          Pci configuration space MMIO address of CrashLog BAR
  @param[out]   *CrashDataBuff    Pointer to CrashLog Main data buffer
**/
EFI_STATUS
CpuCrashLogMainDiscovery (
  IN   UINT32                 BaseAddr,
  OUT  CPU_CRASHLOG_BUFFER    *CrashDataBuff
  )
{
  UINT32   CrashlogData;

  CrashDataBuff->Data = ((UINT64) MmioRead32 (BaseAddr + R_CRASHLOG_MAIN_POINTER_OFFSET) + \
                          LShiftU64((UINT64) MmioRead32 (BaseAddr + R_CRASHLOG_MAIN_POINTER_OFFSET + 4), 32));
  DEBUG ((DEBUG_INFO, "MainLog CrashDataBuff->Data = 0x%X\n", CrashDataBuff->Data));
  //
  // Read Data from buffer
  //
  CrashlogData = MmioRead32 (BaseAddr + CrashDataBuff->Fields.DataBuffAddress);
  DEBUG ((DEBUG_INFO, "MainLog Byte 0 = 0x%X\n", CrashlogData));
  if (CrashlogData == 0) {
    DEBUG ((DEBUG_INFO, "CPU CrashLog: No Main CrashLog\n"));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Get CPU CrashLog Telemetry pointer data

  @param[in]    BaseAddr          Pci configuration space MMIO address of CrashLog BAR
  @param[out]   *CrashDataBuff    Pointer to CrashLog Telemetry data buffer
**/
EFI_STATUS
CpuCrashLogTelemetryDiscovery (
  IN   UINT32                 BaseAddr,
  OUT  CPU_CRASHLOG_BUFFER    *CrashDataBuff
  )
{
  UINT32   CrashlogData;

  CrashDataBuff->Data = ((UINT64) MmioRead32 (BaseAddr + R_CRASHLOG_TELEMETRY_POINTER_OFFSET) + \
                          LShiftU64((UINT64) MmioRead32 (BaseAddr + R_CRASHLOG_TELEMETRY_POINTER_OFFSET + 4), 32));
  DEBUG ((DEBUG_INFO, "Telemetry CrashDataBuff->Data = 0x%X\n", CrashDataBuff->Data));
  //
  // Read Data from buffer
  //
  CrashlogData = MmioRead32 (BaseAddr + CrashDataBuff->Fields.DataBuffAddress);
  DEBUG ((DEBUG_INFO, "Telemetry Byte 0 = 0x%X\n", CrashlogData));
  if ( CrashlogData == 0x0 ) {
    DEBUG ((DEBUG_INFO, "CPU CrashLog: No Telemetry CrashLog\n"));
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
  Get CPU CrashLog Trace pointer data

  @param[in]    BaseAddr          Pci configuration space MMIO address of CrashLog BAR
  @param[out]   *CrashDataBuff    Pointer to CrashLog Trace data buffer
**/
EFI_STATUS
CpuCrashLogTraceDiscovery (
  IN   UINT32                 BaseAddr,
  OUT  CPU_CRASHLOG_BUFFER    *CrashDataBuff
  )
{
  UINT32      CrashlogData;

  CrashDataBuff->Data = ((UINT64) MmioRead32 (BaseAddr + R_CRASHLOG_TRACE_POINTER_OFFSET) + \
                          LShiftU64((UINT64) MmioRead32 (BaseAddr + R_CRASHLOG_TRACE_POINTER_OFFSET + 4), 32));
  DEBUG ((DEBUG_INFO, "Trace CrashDataBuff->Data = 0x%X\n", CrashDataBuff->Data));
  //
  // Read Data from buffer
  //
  CrashlogData = MmioRead32 (BaseAddr + CrashDataBuff->Fields.DataBuffAddress);
  DEBUG ((DEBUG_INFO, "Trace Byte 0 = 0x%X\n", CrashlogData));

  if ( CrashlogData == 0x0) {
    DEBUG ((DEBUG_INFO, "CPU CrashLog: No Trace CrashLog\n"));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  This function disables cpu crash log

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Mailbox command failed with an error
  @retval EFI_TIMEOUT             Mailbox command did not complete
**/
EFI_STATUS
CpuCrashLogDisable (
  VOID
  )
{
  TEL_CRASHLOG_DEVSC_CAP  CrashLogDevscCap;
  UINT32                  TempBarAddr;
  UINT32                  TelemetryBaseAddress;
  EFI_STATUS              Status;

  DEBUG ((DEBUG_INFO, "Disable CPU crashLog\n"));

  TelemetryBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, TEL_BUS_NUM, TEL_DEV_NUM, TEL_FUN_NUM, 0);
  if (PciSegmentRead16 (TelemetryBaseAddress + PCI_VENDOR_ID_OFFSET) == MAX_UINT16) {
    DEBUG ((DEBUG_ERROR, "Telemetry PCI device is disabled\n"));
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&CrashLogDevscCap, sizeof (TEL_CRASHLOG_DEVSC_CAP));
  Status = GetCpuCrashLogCapability (&CrashLogDevscCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "CPU CrashLog feature is not supported\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Program BAR address and enable command register memory space decoding
  //
  TempBarAddr = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
  if (CrashLogDevscCap.DiscoveryData.Fields.TBir == V_TEL_DVSEC_TBIR_BAR0) {
    PciSegmentWrite32 (TelemetryBaseAddress + R_TEL_CFG_BAR0, TempBarAddr);
  } else if (CrashLogDevscCap.DiscoveryData.Fields.TBir == V_TEL_DVSEC_TBIR_BAR1) {
    PciSegmentWrite32 (TelemetryBaseAddress + R_TEL_CFG_BAR1, TempBarAddr);
  }
  PciSegmentOr16 (TelemetryBaseAddress + PCI_COMMAND_OFFSET, (UINT16) (EFI_PCI_COMMAND_MEMORY_SPACE));

  Status = CpuCrashLogMailboxCommand (V_CPU_CRASHLOG_CMD_DISABLE, 0);

  //
  // Disable MSE/ISE and clear BARs after sending mailbox command to disable Crashlog
  //
  PciSegmentAnd16 (TelemetryBaseAddress + PCI_COMMAND_OFFSET, (UINT16) ~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_IO_SPACE));
  if (CrashLogDevscCap.DiscoveryData.Fields.TBir == V_TEL_DVSEC_TBIR_BAR0) {
    PciSegmentWrite32 (TelemetryBaseAddress + R_TEL_CFG_BAR0, 0);
  } else if (CrashLogDevscCap.DiscoveryData.Fields.TBir == V_TEL_DVSEC_TBIR_BAR1) {
    PciSegmentWrite32 (TelemetryBaseAddress + R_TEL_CFG_BAR1, 0);
  }
  DEBUG ((DEBUG_INFO, "CpuCrashLogCollectDataFromTelemetrySRAM - end\n"));

  return Status;
}

/**
  Save Cpu CrashLog BAR for S3

**/
VOID
SaveCpuCrashLogBarForS3 (
  VOID
  )
{
  UINT64                   DeviceBase;
  UINT32                   BaseAddr;
  TEL_CRASHLOG_DEVSC_CAP   CrashLogDevscCap;
  EFI_STATUS               Status;

  BaseAddr = 0;
  Status = GetCpuCrashLogCapability (&CrashLogDevscCap);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "CPU CrashLog feature is not supported\n"));
    return;
  }
  DeviceBase = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, TEL_BUS_NUM, TEL_DEV_NUM, TEL_FUN_NUM, 0);

  if (CrashLogDevscCap.DiscoveryData.Fields.TBir == V_TEL_DVSEC_TBIR_BAR0) {
    BaseAddr = (PciSegmentRead32 (DeviceBase + R_TEL_CFG_BAR0) & 0xFFFFFFF0);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      PcdGet64 (PcdSiPciExpressBaseAddress) + PCI_SEGMENT_LIB_ADDRESS (
                                              SA_SEG_NUM,
                                              TEL_BUS_NUM,
                                              TEL_DEV_NUM,
                                              TEL_FUN_NUM,
                                              R_TEL_CFG_BAR0),
      1,
      &BaseAddr
    );
  } else if (CrashLogDevscCap.DiscoveryData.Fields.TBir == V_TEL_DVSEC_TBIR_BAR1) {
    BaseAddr = (PciSegmentRead32 (DeviceBase + R_TEL_CFG_BAR1) & 0xFFFFFFF0);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      PcdGet64 (PcdSiPciExpressBaseAddress) + PCI_SEGMENT_LIB_ADDRESS (
                                              SA_SEG_NUM,
                                              TEL_BUS_NUM,
                                              TEL_DEV_NUM,
                                              TEL_FUN_NUM,
                                              R_TEL_CFG_BAR1),
      1,
      &BaseAddr
    );
  }
}
