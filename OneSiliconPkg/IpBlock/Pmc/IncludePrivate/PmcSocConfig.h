/** @file
  PMC SoC configuration

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _PMC_SOC_CONFIGURATION_H_
#define _PMC_SOC_CONFIGURATION_H_

typedef enum {
  AdrSinglePhase = 0,
  AdrDualPhase
} ADR_PHASE_TYPE;

typedef enum {
  AdrGpioB = 0,
  AdrGpioC
} ADR_GPIO;

typedef enum {
  AdrOverPmSync = 0,
  AdrOverDmi
} ADR_MSG_INTERFACE;

typedef struct {
  BOOLEAN            Supported;
  ADR_PHASE_TYPE     AdrPhaseType;
  ADR_GPIO           AdrGpio;
  ADR_MSG_INTERFACE  AdrMsgInterface;
  //
  // On some designs ADR_GEN_CFG has been moved in the HW.
  // Set this to if ADR_GEN_CFG is located at 0x1908
  //
  BOOLEAN            AdrGenCfgMoved;
} PMC_ADR_SOC_CONFIG;

typedef struct {
  BOOLEAN             CppmCgInterfaceVersion;
  BOOLEAN             LpmSupported;
  UINT8               LpmInterfaceVersion;
  BOOLEAN             OsIdleSupported;
  BOOLEAN             TimedGpioSupported;
  UINT32              CpuIovrRampTime;
  BOOLEAN             PsOnSupported;
  BOOLEAN             ModPhySusPgSupported;
  UINT8               SciIrq;
  BOOLEAN             FabricPowerGatingCppmQualificationEnable;
  BOOLEAN             UsbDbcConnected;
  UINT32              Usb3LanesConnectedBitmask;
  BOOLEAN             DisableIosfSbClockGating;
  BOOLEAN             SkipModPhyGatingPolicy;
  PMC_ADR_SOC_CONFIG  AdrSocConfig;
  BOOLEAN             AllSbrIdleQualifierEnable;
  UINT32              LpmPriVal;                            ///< Low Power Mode Priority
  UINT8               PmSyncMiscCfgInterfaceVersion;
  BOOLEAN             LockFivr;
  BOOLEAN             LockCnviLdoCnf;
  BOOLEAN             AllPsfIdleOverride;

  BOOLEAN             FabricPowerGatingConfigurable;
  BOOLEAN             LpmPriorityConfigurable;
  BOOLEAN             ObffConfigurable;
  BOOLEAN             BreakCxConfigurable;
  BOOLEAN             PwrCycDurLegacyOffset;
  BOOLEAN             PmcWdtCmdSupported;

  BOOLEAN             PowerOptimizerEnable;
  UINT8               ClientObffEn;
  UINT8               CxObffEntryDelay;
  UINT8               MemCloseStateEn;
  UINT8               InternalObffEn;
  UINT8               ExternalObffEn;
  BOOLEAN             SleEnabled;

} PMC_SOC_CONFIG;

typedef struct {
  BOOLEAN  OverrideFetRampTime;
  UINT8    FetRampTime;
  UINT8    IsFetRampTime;
  UINT16   FuseDownloadDelayUs;
} PMC_FIVR_SOC_CONFIG;

#endif
