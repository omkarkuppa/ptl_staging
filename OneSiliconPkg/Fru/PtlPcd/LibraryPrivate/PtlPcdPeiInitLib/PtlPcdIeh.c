/** @file
  The IEH initialization After Memory.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include "PtlPcdInitPei.h"

#include <IehConfig.h>
#include <IehHandle.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Register/ItssRegs.h>
#include <Register/PchRegs.h>
#include <Register/IehRegs.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PciSegmentLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <PcdSbPortIds.h>

/**
  Programs severity register bits based on ITSS UEV register

  @param[in] IehHandle    IEH Handle instance
  @param[in] Uev          ITSS UEV value
**/
VOID
PtlPcdIehProgramSeverityF2Segment (
  IN  IEH_HANDLE      *IehHandle,
  IN  UINT32          Uev
  )
{
  UINT64           Data64;

  Data64  = (UINT64) (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);

  //
  // UR severity bits
  //
  if (Uev & BIT20) {
    Data64 |= (UINT64) (BIT24 | BIT20 | BIT16 | BIT12 | BIT8);
  }
  //
  // UC severity bits
  //
  if (Uev & BIT16) {
    Data64 |= (UINT64) (BIT23 | BIT19 | BIT15 | BIT11 | BIT7);
  }
  //
  // Command parity severity bits
  //
  if (Uev & BIT22) {
    Data64 |= (UINT64) (BIT22 | BIT18 | BIT14 | BIT10 | BIT6);
  }

  PciSegmentWrite32 (IehHandle->PciCfgBase + R_IEH_CFG_LERRUNCSEV_N0, (UINT32) Data64);
  PciSegmentWrite32 (IehHandle->PciCfgBase + R_IEH_CFG_LERRUNCSEV_N0 + 4, (UINT32) RShiftU64 (Data64, 32));
}

/**
  Programs severity register bits based on ITSS UEV register

  @param[in] IehHandle    IEH Handle instance
  @param[in] Uev          ITSS UEV value
**/
VOID
PtlPcdIehProgramSeverityF3Segment (
  IN  IEH_HANDLE      *IehHandle,
  IN  UINT32          Uev
  )
{
  UINT64           Data64;

  Data64  = (UINT64) (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0);

  //
  // UR severity bits
  //
  if (Uev & BIT20) {
    Data64 |= (UINT64) (BIT8);
  }
  //
  // UC severity bits
  //
  if (Uev & BIT16) {
    Data64 |= (UINT64) (BIT7);
  }
  //
  // Command parity severity bits
  //
  if (Uev & BIT22) {
    Data64 |= (UINT64) (BIT6);
  }

  PciSegmentWrite32 (IehHandle->PciCfgBase + R_IEH_CFG_LERRUNCSEV_N0, (UINT32) Data64);
  PciSegmentWrite32 (IehHandle->PciCfgBase + R_IEH_CFG_LERRUNCSEV_N0 + 4, (UINT32) RShiftU64 (Data64, 32));
}

/**
  Programs IEH to Enable Mode

  @param[in] IehHandle    IEH Handle instance
**/
VOID
PtlIehEnableMode (
  IN  IEH_HANDLE      *IehHandle
  )
{
  #if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  EFI_STATUS                      Status;
  TRACE_HUB_PREMEM_CONFIG         *TraceHubPreMemConfig;
  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi;

  PciSegmentWrite32 (IehHandle->PciCfgBase + R_IEH_CFG_GCOERRMSK, 0xFFFFFFFF);
  PciSegmentWrite32 (IehHandle->PciCfgBase + R_IEH_CFG_GSYSEVTMAP, 0x0);
  PciSegmentWrite32 (IehHandle->PciCfgBase + R_IEH_CFG_LERRUNCMSK_N0, (UINT32) BIT20);
  PciSegmentWrite32 (IehHandle->PciCfgBase + R_IEH_CFG_LERRCORMSK_N0, (UINT32) BIT20);

  Status = PeiServicesLocatePpi (
            &gSiPreMemPolicyPpiGuid,
            0,
            NULL,
            (VOID **) &SiPreMemPolicyPpi
           );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "IEH: NPK.EnableMode = 0x%x\n",TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode));
  if (TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode == 0x1) {
    PciSegmentAnd32 (IehHandle->PciCfgBase + R_IEH_CFG_LERRUNCMSK_N0, (UINT32) ~BIT20);
    PciSegmentAnd32 (IehHandle->PciCfgBase + R_IEH_CFG_LERRCORMSK_N0, (UINT32) ~BIT20);
  }
#endif
  PciSegmentAndThenOr32 (IehHandle->PciCfgBase + R_IEH_CFG_GSYSEVTMAP, 0x7FFFFFC0, 0x3F);
  PciSegmentAnd32 (IehHandle->PciCfgBase + R_IEH_CFG_IEHTYPEVER, (UINT32)~(B_IEH_CFG_IEHTYPEVER_IEH_BUSNUM));
  PciSegmentAndThenOr32 (IehHandle->PciCfgBase + R_IEH_CFG_ERRPINCTRL, 0xFFFFFFC0, 0x2A);
  PciSegmentAnd32 (IehHandle->PciCfgBase + R_IEH_CFG_ERRPINDATA, 0xFFFFFFF8);
  PciSegmentAnd32 (IehHandle->PciCfgBase + R_IEH_CFG_BITMAP, 0xFFFFFFE0);

  PciSegmentWrite32 (IehHandle->PciCfgBase + R_IEH_CFG_LCORERRCNTSEL_N0, 0x0);
}

/**
  Creates IEH handle from provided parameters

  @param[in]      SiPolicy          SiPolicyPpi instance
  @param[in]      PciCfgBase        PCI CFG Base Address of IEH controller
  @param[in, out] IehHandle         IEH handle to initialize
  @param[in, out] IehPrivateConfig  Initialized IEH Private Config structure
**/
VOID
PtlCreateIehHandle (
  IN      SI_POLICY_PPI       *SiPolicy,
  IN      UINT64              PciCfgBase,
  IN OUT  IEH_HANDLE          *IehHandle,
  IN OUT  IEH_PRIVATE_CONFIG  *IehPrivateConfig
  )
{
  EFI_STATUS      Status;
  IEH_CONFIG      *IehConfig;

  //
  // PCIe Config Space Base Address
  //
  IehHandle->PciCfgBase = PciCfgBase;

  //
  // IEH Private Config
  //
  IehPrivateConfig->BusNum = DEFAULT_PCI_BUS_NUMBER_PCH;
  IehHandle->PrivateConfig = IehPrivateConfig;

  //
  // IEH public configuration
  //
  Status = GetConfigBlock ((VOID *) SiPolicy, &gIehConfigGuid, (VOID *) &IehConfig);
  ASSERT_EFI_ERROR (Status);
  IehHandle->Config = IehConfig;
}

/**
  Build a PTL SOC IEH handle

  @param[in]      SiPolicy          SiPolicyPpi instance
  @param[in, out] IehHandle         IEH handle to initialize
  @param[in, out] IehPrivateConfig  Initialized IEH Private Config structure
**/
VOID
PtlPcdIehGetHandle (
  IN      SI_POLICY_PPI       *SiPolicy,
  IN OUT  IEH_HANDLE          *IehHandle,
  IN OUT  IEH_PRIVATE_CONFIG  *IehPrivateConfig
  )
{
  UINT64    PciCfgBase;

  PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (
                 DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_IEH,
                 PCI_FUNCTION_NUMBER_PCH_IEH,
                 0
                 );

  PtlCreateIehHandle (SiPolicy, PciCfgBase, IehHandle, IehPrivateConfig);
}

/**
  Build a PTL SOC IEH handle for second instance

  @param[in]      SiPolicy          SiPolicyPpi instance
  @param[in, out] IehHandle         IEH handle to initialize
  @param[in, out] IehPrivateConfig  Initialized IEH Private Config structure
**/
VOID
PtlPcdIehGetHandle2 (
  IN      SI_POLICY_PPI       *SiPolicy,
  IN OUT  IEH_HANDLE          *IehHandle,
  IN OUT  IEH_PRIVATE_CONFIG  *IehPrivateConfig
  )
{
  UINT64    PciCfgBase;

  PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (
                 DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_IEH_2,
                 PCI_FUNCTION_NUMBER_PCH_IEH_2,
                 0
                 );

  PtlCreateIehHandle (SiPolicy, PciCfgBase, IehHandle, IehPrivateConfig);
}

/**
  Initialize SOC IEH controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
PtlPcdIehInit (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  IEH_HANDLE          IehHandle;
  IEH_HANDLE          IehHandle2;
  IEH_PRIVATE_CONFIG  IehPrivateConfig;
  UINT32              ItssUev;
  P2SB_PORT_16_ID     Pid;

  ZeroMem (&IehHandle, sizeof (IEH_HANDLE));
  ZeroMem (&IehHandle2, sizeof (IEH_HANDLE));
  ZeroMem (&IehPrivateConfig, sizeof (IEH_PRIVATE_CONFIG));
  Pid.Pid16bit = PTL_SID_F2_PID_ITSS;
  ItssUev = PchPcrRead32 (Pid.PortId.LocalPid, R_ITSS_PCR_UEV);


  PtlPcdIehGetHandle (SiPolicy, &IehHandle, &IehPrivateConfig);
  if (PciSegmentRead32 (IehHandle.PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_INFO, "IEH device not enabled. Skipping initialization.\n"));
  } else {
    PtlPcdIehProgramSeverityF2Segment (&IehHandle, ItssUev);
    IehInit (&IehHandle);

    if (IehHandle.Config->Mode == IEH_MODE_ENABLE) {
      PtlIehEnableMode (&IehHandle);
    }
  }

  PtlPcdIehGetHandle2 (SiPolicy, &IehHandle2, &IehPrivateConfig);
  if (PciSegmentRead32 (IehHandle2.PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_INFO, "IEH device not enabled. Skipping initialization.\n"));
  } else {
    PtlPcdIehProgramSeverityF3Segment (&IehHandle2, ItssUev);
    IehInit (&IehHandle2);

    if (IehHandle2.Config->Mode == IEH_MODE_ENABLE) {
      PtlIehEnableMode (&IehHandle2);
    }
  }
}
