/** @file
  Header file for the PCH Init PEIM

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
#ifndef _PTL_SOC_INIT_PEI_H_
#define _PTL_SOC_INIT_PEI_H_

#include <Uefi.h> // Required by Reset Notify
#include <Ppi/PlatformSpecificResetNotification.h> // Required by Reset Notify
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/TimerLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/SataSocLib.h>
#include <Library/PeiTraceHubInitLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/PcdPeiLpssI2cSocInitLib.h>
#include <Library/PcdPeiLpssI3cSocInitLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PmcLib.h>
#include <IndustryStandard/Pci30.h>
#include <PchResetPlatformSpecific.h>
#include <Library/PchInfoHob.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PchLimits.h>
#include <Library/PeiThermalLib.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/PeiP2sbPrivateLib.h>
#include <Library/P2sbPrivateLib.h>
#include <Library/PeiPchDmiLib.h>
#include <Library/PeiItssLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/PeiScsLib.h>
#include <Library/PeiIehInitLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <P2SbHandle.h>
#include <LpssI3cConfig.h>
#include <LpssI2cConfig.h>
#include <Library/PeiTraceHubInitLib.h>
#include <PchConfigHobCommon.h>

extern EFI_GUID gPchDeviceTableHobGuid;

/**
  The function program PHY registers.

  @param[in] SiPolicyPpi               The SI Policy PPI instance

**/
VOID
PtlPcdPhyConfigure (
  IN  SI_POLICY_PPI    *SiPolicyPpi
  );

/**
  Perform Initialization of the Downstream Root Ports.

  @param[in] SiPolicy             The SI Policy PPI
**/
VOID
PtlPcdPcieRpInit (
  IN SI_POLICY_PPI     *SiPolicy
  );

/**
  Perform pre-mem initialization of the root port.

  @param[in] SiPreMemPolicyPpi  Pointer to pre-mem SI policy.
**/
VOID
PtlPcdPcieRpPreMemInit (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  );

/**
  Perform pre-policy initialization of a root port.
**/
VOID
PtlPcdPciePrePolicyInit (
  VOID
  );

/**
  Update PCIe portion of the info hob.

  @param[in, out] PchInfoHob  PCH info HOB to fill in.
**/
VOID
BuildPcieInfoHob (
  IN OUT PCH_INFO_HOB *PchInfoHob
  );

/**
  Initialize PMC controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
PtlPcdPmcInit (
  IN SI_POLICY_PPI  *SiPolicy
  );

/**
  Init PMC at end of PEI.

  @param[in] SiPolicy  Pointer to SiPolicy
**/
VOID
PtlPcdPmcInitEndOfPei (
  IN SI_POLICY_PPI  *SiPolicy
  );

/**
  Initialize P2SB structures - PCH Legacy
  This should be done according P2SB IP integration in SoC

  @param[in]      SiPolicy        Pointer to Silicon Policy
  @param[in out]  P2SbHandle      Pointer to P2SB handle structure
  @param[in out]  P2SbPrivate     Pointer to P2SB private configuration structure
  @param[in out]  P2SbController  Pointer to P2SB controller identification structure
  @param[in out]  P2SbCallback    Pointer to P2SB callback structure
  @param[in out]  P2SbPcieAccess  Pointer to P2SB PCIe Register Access
**/
VOID
PtlPcdP2SbHandleInit (
  IN     SI_POLICY_PPI           *SiPolicy,
  IN OUT P2SB_HANDLE             *P2SbHandle,
  IN OUT P2SB_PRIVATE_CONFIG     *P2SbPrivate,
  IN OUT P2SB_CONTROLLER         *P2SbController,
  IN OUT P2SB_CALLBACK           *P2SbCallback,
  IN OUT PCI_CONFIG_SPACE_ACCESS *P2SbPcieAccess
  );

/**
  The function performs P2SB initialization for SOC.

  @param[in] SiPolicy         The SI Policy PPI instance
**/
VOID
PtlPcdP2SbInit (
  IN  SI_POLICY_PPI           *SiPolicy
  );

/**
  The function performs P2SB initialization for second P2SB instance.

  @param[in]      SiPolicy       Pointer to Silicon Policy
**/
VOID
PtlPcdSecondP2SbInit (
  IN  SI_POLICY_PPI           *SiPolicy
  );

/**
  The function performs P2SB lock programming for SOC.

  @param[in] SiPolicy         The SI Policy PPI instance
**/
VOID
PtlPcdP2sbLock (
  IN  SI_POLICY_PPI           *SiPolicy
  );

/**
  The function performs P2SB lock programming for second P2SB instance.

  @param[in] SiPolicy         The SI Policy PPI instance
**/
VOID
PtlPcdSecondP2sbLock (
  IN  SI_POLICY_PPI           *SiPolicy
  );

/**
  Early init P2SB configuration
**/
VOID
PtlPcdP2sbEarlyConfig (
  VOID
  );

/**
  Initialize LAN device on PCH

  @param[in]      SiPolicy       Pointer to Silicon Policy
**/
VOID
PtlPcdGbeInit (
  IN  SI_POLICY_PPI  *SiPolicy
  );

/**
  Initialize the TSN devices

  @param[in] SiPolicy   The Silicon Policy PPI instance
**/
VOID
PtlPcdTsnInit (
  IN  SI_POLICY_PPI  *SiPolicy
  );

/**
  Performs USB Controllers initialization

  @param[in] SiPolicy   The Silicon Policy PPI instance
**/
VOID
PtlPcdUsbInit (
  IN  SI_POLICY_PPI  *SiPolicy
  );


/**
  Intialize SCS subsystem.

  @param[in] SiPolicyPpi  SI policy PPI.
**/
VOID
PtlPcdScsInit (
  IN SI_POLICY_PPI  *SiPolicy
  );

/**
  Configure PCIe Grant Counts
**/
VOID
PchConfigurePcieGrantCounts (
  VOID
  );

/**
  Configure BiosLock according to policy setting.

  @param[in] LockDownConfig       LockDownConfig policy
**/
VOID
ConfigureBiosLock (
  IN  SI_POLICY_PPI  *SiPolicy
  );

/**
  The function performs PSTH specific programming.

  @param[in] SiPolicy          The SI Policy instance
**/
VOID
PchPsthConfigure (
  IN  SI_POLICY_PPI           *SiPolicy
  );

/**
  The function update pch config hob in the end of PchInit.

  @param[in]      SiPolicy               The SI Policy PPI instance
**/
VOID
BuildPchConfigHobs (
  IN     SI_POLICY_PPI          *SiPolicy
  );

/**
  The function update pch config hob after Pre Mem policy is installed.

  @param[in]  SiPreMemPolicy    The SI Pre Meme Policy PPI instance
**/
VOID
BuildPchGeneralConfigHobPreMem (
  IN     SI_PREMEM_POLICY_PPI         *SiPreMemPolicy
  );

/**
  The function update pch info hob in the end of PchInit.
**/
VOID
BuildPchInfoHob (
  VOID
  );

/**
  Initialize the Smbus PPI and program the Smbus BAR

  @param[in]  SiPreMemPolicyPpi   The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database
**/
EFI_STATUS
SmbusInit (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi
  );

/**
  The function performs SMBUS specific programming.

  @param[in] SiPolicyPpi       The SI Policy PPI instance

**/
VOID
SmbusConfigure (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  );

/**
  Initialize P2SB structures - second P2SB instance
  This should be done according P2SB IP integration

  @param[in]      SiPolicy        Pointer to Silicon Policy
  @param[in out]  P2SbHandle      Pointer to P2SB handle structure
  @param[in out]  P2SbPrivate     Pointer to P2SB private configuration structure
  @param[in out]  P2SbController  Pointer to P2SB controller identification structure
  @param[in out]  P2SbCallback    Pointer to P2SB callback structure
  @param[in out]  P2SbPcieAccess  Pointer to P2SB PCIe Register Access
**/
VOID
PtlPcdSecondP2SbHandleInit (
  IN     SI_POLICY_PPI           *SiPolicy,
  IN OUT P2SB_HANDLE             *P2SbHandle,
  IN OUT P2SB_PRIVATE_CONFIG     *P2SbPrivate,
  IN OUT P2SB_CONTROLLER         *P2SbController,
  IN OUT P2SB_CALLBACK           *P2SbCallback,
  IN OUT PCI_CONFIG_SPACE_ACCESS *P2SbPcieAccess
  );

/**
  Initialize ESPI controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
PtlPcdEspiInit (
  IN SI_POLICY_PPI  *SiPolicy
  );

/**
  Initialize SOC IEH controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
PtlPcdIehInit (
  IN SI_POLICY_PPI  *SiPolicy
  );

/**
  Registers an extended BIOS range decode initialization callback.

  @retval   EFI_SUCCESS   Notify descriptor registration done.
  @retval   Other         Notify descriptor registration unsuccesful.
**/
EFI_STATUS
PtlPcdExtendedBiosDecodeRangeRegistration (
  VOID
  );

/**
  Initialize Soc TraceHub in PreMem

  @param[in] SiPreMemPolicyPpi        The SI PreMem Policy PPI instance

**/
VOID
PtlPcdTraceHubInitPreMem (
  IN  SI_PREMEM_POLICY_PPI           *SiPreMemPolicyPpi
  );

/**
  Notify function to disable and power gate TraceHub

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
  @retval     EFI_UNSUPPORTED   The device is not supported
**/
EFI_STATUS
TraceHubPowerGateNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Initialize DCI

  @param[in]  SiPreMemPolicyPpi   The SI PreMem Policy PPI instance
**/
VOID
PtlPcdDciInit (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi
  );

/**
  Perform Thermal Management Support initialization

  @param[in] SiPolicy                   SI policy PPI instance.
  @param[in] PwrmBase                   Address for PWRM base.

**/
VOID
ThermalInit (
  IN  SI_POLICY_PPI                     *SiPolicy,
  IN  UINTN                             PwrmBase
  );

/**
  Initializes PTL SoC SSE/SSE++ subsystem.
  All SSE/SSE++ devices should be hidden and disabled.

  @param[in] SiPolicy                   SI policy PPI instance.
**/
VOID
PtlPcdSseInit (
  IN  SI_POLICY_PPI                     *SiPolicy
  );

/**
  Initialize CAN Subsystem for PTL SOC

  @param[in] SiPolicy     Pointer to SI_POLICY_PPI

  @retval    EFI_SUCCESS  The function completed successfully.
**/
EFI_STATUS
PtlPcdCanInit (
  IN  SI_POLICY_PPI *SiPolicyPpi
  );

/**
  Initialize the CNVi controller on Pre-Mem phase.

  @param[in] SiPreMemPolicyPpi         The SI PreMem Policy PPI instance
**/
VOID
PtlPcdCnviPreMemInit (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  );

/**
  Initialize the CNVi controller on Post-Mem phase.

  @param[in] SiPolicy     The Silicon Policy PPI instance

**/
VOID
PtlPcdCnviPostMemInit (
  IN  SI_POLICY_PPI  *SiPolicy
  );

/**
  Configure CNVi Bluetooth on Post-Mem phase.

  @param[in] SiPolicy     The Silicon Policy PPI instance
**/
VOID
PtlPcdCnviConfigureBt (
  IN SI_POLICY_PPI *SiPolicy
  );

/**
  Initialize SOC RTC

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
**/
VOID
PtlPcdRtcInit (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  );

/**
  The function performs SOC RTC lock setting.

  @param[in] SiPolicy  Pointer to SI_POLICY_PPI
**/
VOID
PtlPcdRtcLock (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  );

/**
  Configures LPSS Controllers

  @param[in] SiPolicy
**/
VOID
PtlPcdLpssInit (
  IN     SI_POLICY_PPI           *SiPolicy
  );

/**
  Configures LPSS Function 0 Disabled Controllers

  @param[in] SiPolicy              The SiPolicy instance
**/
VOID
PtlPcdLpssFunction0Disable (
  IN SI_POLICY_PPI                 *SiPolicy
  );

/**
  Initialize the I2c controller

  @param[in]      SiPolicy         The SI Policy PPI instance
**/
VOID
PtlPcdLpssI2cInit (
  IN     SI_POLICY_PPI          *SiPolicy
  );

/**
  Initialize the ACE (Audio & Context Subsystem) in Pre-mem phase

  @param[in] SiPreMemPolicyPpi       Pre-mem Policy

**/
VOID
PtlPcdAcePreMemInit (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  );

/**
  Initialize the ACE (Audio & Context Subsystem). Program FNCFG register before P2SB Lock.

  @param[in] SiPreMemPolicyPpi       Pre-mem Policy

**/
VOID
PtlPcdAcePostMemInit (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  );

/**
Before any interrupt is enabled, set controller message enable.
**/
VOID
PtlPcdItssSetMasterMessageEnable (
  VOID
 );

/**
  Initialize Itss controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
PtlPcdItssInit (
  IN SI_POLICY_PPI  *SiPolicy
  );

/**
  Build Memory Mapped IO Resource which is used to build E820 Table in LegacyBios,
  the resource range should be preserved in ACPI as well.
**/
VOID
PchPreservedMmioResource (
  VOID
  );

/**
 * Disable Spi Dma Workaround
 */
VOID
PtlPcdSpiDmaWaEnd (
  VOID
  );


#endif
