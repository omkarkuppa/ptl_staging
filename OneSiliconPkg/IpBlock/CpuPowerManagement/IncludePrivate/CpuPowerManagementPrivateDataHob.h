/** @file
  Struct and GUID definitions for processor power management related data Hob.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#ifndef _CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB_H_
#define _CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB_H_

#include <Ppi/SiPolicy.h>
#include <PowerMgmtNvsStruct.h>
#define TDP_MAX_LEVEL 3

extern EFI_GUID gCpuPowerManagementPrivateDataHobGuid;

///
/// This HOB is used to pass only the required information from PEI for DXE consumption.
///
typedef struct {
  UINT32                 PpmFlags;                                /// PPM Flags Values.
  UINT8                  CtdpLevelsSupported;                     /// ConfigTdp Number Of Levels.
  UINT8                  ApplyConfigTdp;                          /// Enable Configurable TDP
  UINT8                  ConfigurablePpc;                         /// Boot Mode vlues for _PPC
  UINT8                  CtdpTar;                                 /// CTDP active Level TAR
  UINT8                  ConfigTdpBootModeIndex;                  /// CTDP Boot Mode Index
  UINT16                 CtdpPowerLimit1;                         /// CTDP active Level Power Limit1
  UINT16                 CtdpPowerLimit2;                         /// CTDP active Level Power Limit2
  UINT8                  CtdpPowerLimitWindow;                    /// CTDP active Level Power Limit1 Time Window
  UINT8                  EnableAllThermalFunctions;
  BOOLEAN                ThETAIbattEnable;                        /// ThETA Ibatt support to enabled
  BOOLEAN                IsysLimitL1Enable;                       /// Isys Limit L1 support to enabled
  BOOLEAN                IsysLimitL2Enable;                       /// Isys Limit L2 support to enabled
  FVID_TABLE             FvidTable[TPSS_FVID_MAX_STATES+1];       /// FVID Table
} CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB;
#endif
