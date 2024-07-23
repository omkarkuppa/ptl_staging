/** @file
  Header file for TelemetryLib.

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
#ifndef _TELEMETRY_LIB_H_
#define _TELEMETRY_LIB_H_

//
// Structure of DEVSC registers
//
typedef union {
  struct {
    UINT32  PcieCapId             : 16;       ///< PCI Express Capability ID
    UINT32  CapVer                : 4;        ///< Capability Version
    UINT32  NextCapOffset         : 12;       ///< Next Capability Offset
  } Fields;
  UINT32 Data;
} CAP_DATA;

typedef union {
  struct {
    UINT32  DevscVenId            : 16;       ///< DVSEC_VENID
    UINT32  DevscVer              : 4;        ///< DVSEC_VER
    UINT32  DevscLen              : 12;       ///< DVSEC_LEN
    UINT32  DevscId               : 16;       ///< DVSEC_ID
    UINT32  NumEntries            : 8;        ///< Number of entries, describes the number of telemetry aggregators that would exist in this capability lookup table.
    UINT32  EntrySize             : 8;        ///< Entry Size in DWORDS
  } Fields;
  UINT64 Data64;
  UINT32 Data32[2];
} DEVSC_DATA;

typedef union {
  struct {
    UINT32  TBir                  : 3;         ///< tBIR, The BAR to be used
    UINT32  DiscoveryTableOffset  : 29;        ///< Discovery Table Offset
  } Fields;
  UINT32 Data;
} DISCOVERY_DATA;

typedef struct {
  CAP_DATA        CapData;
  DEVSC_DATA      DevscData;
  DISCOVERY_DATA  DiscoveryData;
} TEL_CRASHLOG_DEVSC_CAP;

//
// Structure of Cpu CrashLog Header
//
typedef union {
  struct {
    UINT32  AccessType                          : 4;             ///< Access Type
    UINT32  CrashType                           : 4;             ///< Crash Type
    UINT32  Count                               : 8;
    UINT32  Reserved                            : 16;
  } Fields;
  UINT32 HeaderData;
} CPU_CRASHLOG_HEADER_DATA;

typedef union {
  struct {
    UINT32  CrashlogSupported                               : 1;
    UINT32  DisableSupported                                : 1;
    UINT32  DiscoveryMechanism                              : 2;
    UINT32  ManualTriggerSupported                          : 1;
    UINT32  ClearSupported                                  : 1;
    UINT32  TriggerOnAllResetSupported                      : 1;
    UINT32  ReArmSupported                                  : 1;
    UINT32  Reserved                                        : 20;
    UINT32  CrashlogRequestorFlowInProgressStatus           : 1;
    UINT32  ReArmStatus                                     : 1;
    UINT32  TriggerOnAllResetStatus                         : 1;
    UINT32  DisableStatus                                   : 1;
  } Fields;
  UINT32 Guid;            ///< GLOBALLY_UNIQUE_ID
} CPU_CRASHLOG_CAPABILITY;

typedef struct {
  CPU_CRASHLOG_HEADER_DATA     CrashlogHeaderData;
  CPU_CRASHLOG_CAPABILITY      CrashlogCapability;
} CPU_CRASHLOG_HEADER;

//
// Structure of Cpu CrashLog Buffer
//
typedef union {
  struct {
    UINT32  DataBuffAddress;    ///< Base address within this BAR for the crashLog data buffer.
    UINT32  DataBuffSize;       ///< Number of dwords of data contained in the crashLog buffer.
  } Fields;
  UINT64 Data;
} CPU_CRASHLOG_BUFFER;

//
// Structure of Cpu CrashLog Struct
//
typedef struct {
  CPU_CRASHLOG_BUFFER  MainBuffer;
  CPU_CRASHLOG_BUFFER  TelemetryBuffer;
  CPU_CRASHLOG_BUFFER  TraceBuffer;
} CPU_CRASHLOG_STRUCT;

/**
  Find the address of Cpu CrashLog ID

  @param[out] CrashLogDevscCap     Pointer to structure of CrashLog DEVSC Data

  @retval EFI_UNSUPPORTED          Cpu CrashLog ID not found
  @retval EFI_SUCCESS              Cpu CrashLog ID found
**/
EFI_STATUS
GetCpuCrashLogCapability (
  OUT TEL_CRASHLOG_DEVSC_CAP  *CrashLogDevscCap
  );

/**
  This function returns the value of CPU CrashLog BAR.

  @param[in] CrashLogDevscCap     Structure of CrashLog DEVSC Data

  @retval BaseAddr                MMIO address of CrashLog BAR
**/
UINT32
GetCpuCrashLogBarAddress (
  IN TEL_CRASHLOG_DEVSC_CAP  *CrashLogDevscCap
  );

/**
  This function clears cpu crash log

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Mailbox command failed with an error
  @retval EFI_TIMEOUT             Mailbox command did not complete
**/
EFI_STATUS
CpuCrashLogClear (
  VOID
  );

/**
  This function send re-arm command to rearm the trigger

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_DEVICE_ERROR        Mailbox command failed with an error
  @retval EFI_TIMEOUT             Mailbox command did not complete
**/
EFI_STATUS
CpuCrashLogReArm (
  VOID
  );

/**
  Get CPU CrashLog Header data

  @param[in]    BaseAddr              Pci configuration space MMIO address of CrashLog BAR
  @param[out]   *CrashLogHeaderBuff   Pointer to CrashLog Discovery Data
**/
VOID
GetCrashLogHeader (
  IN  UINT32                BaseAddr,
  OUT CPU_CRASHLOG_HEADER   *CrashLogHeaderBuff
  );

/**
  This function discovers CPU crashLog and gets the size of each

  @param[out] CrashLogStruct      Pointer to CrashLog Discovery Data

  @retval EFI_UNSUPPORTED         Cpu CrashLog is not supported
  @retval EFI_SUCCESS             Cpu CrashLog is supported
**/
EFI_STATUS
CpuCrashLogDiscovery (
  OUT  CPU_CRASHLOG_STRUCT    *CrashLogStruct
  );
#endif // _TELEMETRY_LIB_H_
