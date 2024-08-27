/** @file
  This file defines:
  * CpuPmData GUID for HOB.
  * CpuPmData GUID HOB data structure.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef __CPU_PM_DATA_H__
#define __CPU_PM_DATA_H__

extern GUID  gCpuPmDataGuid;

#pragma pack(1)
typedef struct {
  UINT16    IccMax;
  UINT16    VoltageLimit;
  UINT16    AcLoadLine;
  UINT16    DcLoadLine;
  UINT16    TdcLimit;
  UINT32    TdcTimeLimit;
  UINT16    IccMaxFvm;
  UINT16    IccItripFvm;
  UINT16    TdcLimitIrms;
  UINT32    TdcTimeLimitIrms;
} VOLTAGE_REGULATOR_PARAM;

typedef struct {
  UINT32    MsrPowerLimit1;
  UINT32    MsrPowerLimit2;
  UINT32    CtdpUpPowerLimit2;
  UINT32    CtdpNominalPowerLimit2;
  UINT32    CtdpDownPowerLimit2;
  UINT32    MsrPowerLimit4;
  UINT32    MsrPowerLimit4DisableFvm;
  UINT8     MsrPowerLimit1TimeWindow;
  UINT8     IsysCurrentL1Tau;
  UINT16    IsysCurrentLimitL1;
  UINT16    IsysCurrentLimitL2;
  UINT16    VsysMax;
} POWER_MANAGEMENT_PARAM;

typedef struct {
  UINT32                     Identifier;
  UINT32                     FamilyModel;
  UINT16                     SaDid0;
  UINT16                     PCoreCount;
  UINT16                     ECoreCount;
  UINT16                     LpECoreCount;
  UINT16                     SocECoreCount;
  UINT16                     Tdp;
  //
  // Type for Cpu, same with enum type CPU_SKU
  //
  UINT32                     Type;
  //
  // Below info is controled by input identifier
  //
  POWER_MANAGEMENT_PARAM     PmData;
  UINT32                     VrDataCount;
  //
  // May have multiple elements for VrData, count is stored in VrDataCount.
  //
  VOLTAGE_REGULATOR_PARAM    VrData[0];
} CPU_PM_DATA;

#pragma pack()
#endif
