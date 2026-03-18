/** @file
  This file contains functions for PCH DMI configuration for SIP15

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

#include <IndustryStandard/PciExpress21.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PeiPchDmiLib.h>
#include <Library/PchDmiLib.h>
#include <Library/PeiPcieSipInitLib.h>
#include <Register/PchDmiRegs.h>
#include <Register/PchDmi15Regs.h>
#include <Register/PcieSipRegs.h>
#include <Register/DmiSipRegs.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <PchDmiPreMemInfoHob.h>
#include <PchPcieRpConfig.h>
#include <ThermalConfig.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <Library/PeiDmiSipInitLib.h>
#include <Library/ConfigBlockLib.h>
#include <PchDmiHob.h>

static CONST UINT16 DmiLocTranCoefOverReg[] = {
    R_PCIE_CFG_LTCO1,
    R_PCIE_CFG_LTCO2,
    R_PCIE_CFG_LTCO3,
    R_PCIE_CFG_LTCO4
  };


/**
  Create PCH DMI HOB
**/
VOID
CreatePchDmiHob (
  VOID
  )
{
  EFI_STATUS            Status;
  PCH_DMI_HOB           *PchDmiHob;

  Status = PeiServicesCreateHob (
            EFI_HOB_TYPE_GUID_EXTENSION,
            sizeof (PCH_DMI_HOB),
            (VOID **) &PchDmiHob
            );
  DEBUG ((DEBUG_INFO, "PchDmiHob Created \n"));
  ASSERT_EFI_ERROR (Status);

  if (PchDmiHob != NULL) {
    //
    // Initialize PCH DMI HOB.
    //
    PchDmiHob->EfiHobGuidType.Name = gPchDmiHobGuid;
    DEBUG ((DEBUG_INFO, "PchDmiHob->EfiHobGuidType.Name: %g\n", &PchDmiHob->EfiHobGuidType.Name));
    DEBUG((DEBUG_INFO, "PchDmiHob @ %X\n", PchDmiHob));
    DEBUG ((DEBUG_INFO, "PchDmiHob - HobHeader: %X\n", sizeof (PCH_DMI_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  }
}

/**
  This function configures PCH DMI3 Capabilities.
  It should be called by DmiSipConfigureMiscConfiguration
  as part of Capability Initialization.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureCapability (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32                        Data32;
  UINT16                        Data16;
  UINT8                         Data8;
  UINT32                        NextCap;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 5.1.2
  // Build Capability linked list
  Data8 = RpDev->PciPcrAccess->Read8 (RpDev->PciPcrAccess, PCI_CAPBILITY_POINTER_OFFSET);
  RpDev->PciPcrAccess->Write8 (
                           RpDev->PciPcrAccess,
                           PCI_CAPBILITY_POINTER_OFFSET,
                           Data8
                           );
  Data16 = RpDev->PciPcrAccess->Read16 (RpDev->PciPcrAccess, R_PCIE_CFG_CLIST);
  RpDev->PciPcrAccess->Write16 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_CLIST,
                            Data16
                            );
  Data8 = RpDev->PciPcrAccess->Read8 (RpDev->PciPcrAccess, R_PCIE_CFG_XCAP);
  RpDev->PciPcrAccess->Write8 (
                           RpDev->PciPcrAccess,
                           R_PCIE_CFG_XCAP,
                           Data8
                           );
  RpDev->PciPcrAccess->Write16 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_MID,
                            0x9805
                            );
  Data16 = RpDev->PciPcrAccess->Read16 (RpDev->PciPcrAccess, R_PCIE_CFG_SVCAP);
  RpDev->PciPcrAccess->Write16 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_SVCAP,
                            Data16
                            );
  Data16 = RpDev->PciPcrAccess->Read16 (RpDev->PciPcrAccess, R_PCIE_CFG_PMCAP);
  RpDev->PciPcrAccess->Write16 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_PMCAP,
                            Data16
                            );

  // Build Extended Capability linked list
  NextCap     = V_PCIE_EXCAP_NCO_LISTEND;

  Data32 = (0x1 << N_PCIE_EXCAP_CV) | V_PCIE_EX_PL16MECH_CID;
  Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
  NextCap = R_PCIE_CFG_PL16MECH;
  RpDev->PciPcrAccess->Write32 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_PL16MECH,
                            Data32
                            );
  Data32 = (0x1 << N_PCIE_EXCAP_CV) | V_PCIE_EX_PL16GECH_CID;
  Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
  NextCap = R_PCIE_CFG_PL16GECH;
  RpDev->PciPcrAccess->Write32 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_PL16GECH,
                            Data32
                            );
  Data32 = (0x1 << N_PCIE_EXCAP_CV) | V_PCIE_EX_DLFECH_CID;
  Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
  NextCap = R_PCIE_CFG_DLFECH;
  RpDev->PciPcrAccess->Write32 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_DLFECH,
                            Data32
                            );
  Data32 = (0x1 << N_PCIE_EXCAP_CV) | V_PCIE_EX_SPE_CID;
  Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
  NextCap = R_PCIE_CFG_SPEECH;
  RpDev->PciPcrAccess->Write32 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_SPEECH,
                            Data32
                            );
  Data32 = 0;
  // No downstream port containment
  RpDev->PciPcrAccess->Write32 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_DPCECH,
                            Data32
                            );
  Data32 = (0x1 << N_PCIE_EXCAP_CV) | 0x2;
  Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
  NextCap = R_PCIE_CFG_VCCH;
  RpDev->PciPcrAccess->Write32 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_VCCH,
                            Data32
                            );
  Data32 = (0x1 << N_PCIE_EXCAP_CV) | V_PCIE_EX_PTM_CID;
  Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
  NextCap = R_PCIE_CFG_PTMECH;
  RpDev->PciPcrAccess->Write32 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_PTMECH,
                            Data32
                            );
  Data32 = (0x1 << N_PCIE_EXCAP_CV) | 0x1E;
  Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
  NextCap = R_PCIE_CFG_L1SECH;
  RpDev->PciPcrAccess->Write32 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_L1SECH,
                            Data32
                            );
  Data32 = (0x1 << N_PCIE_EXCAP_CV) | V_PCIE_EX_ACS_CID;
  Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
  NextCap = R_PCIE_CFG_ACSECH;
  RpDev->PciPcrAccess->Write32 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_ACSECH,
                            Data32
                            );
  Data32 = (0x1 << N_PCIE_EXCAP_CV) | V_PCIE_EX_AEC_CID;
  Data32 |= (NextCap << N_PCIE_EXCAP_NCO);
  RpDev->PciPcrAccess->Write32 (
                            RpDev->PciPcrAccess,
                            R_PCIE_CFG_AECH,
                            Data32
                            );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures DPC extensions support for DMI3.
  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureDpcExtensionsSupport (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  // BWG chapter 6.36
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_CFG_DPCCAPR, (UINT32) ~B_PCIE_CFG_DPCCAPR_RPEFDPC, (UINT32) 0u);

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures LTR mechanism support for DMI3.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureLtrSupport (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 5.5.7
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_CFG_DCAP2, (UINT32) ~0u, (UINT32) (B_PCIE_CFG_DCAP2_EFFS | B_PCIE_CFG_DCAP2_LTRMS));

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures LTR override support for DMI3.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureLtrOverride (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 5.5.7
  RpDev->PciPcrAccess->Write32 (RpDev->PciPcrAccess, R_PCIE_CFG_LTROVR2, 0x0);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_CFG_LTROVR, ~0u, 0x880F880F);

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures TpowerOn.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureTpowerOn (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 5.5.7
  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_PCIE_CFG_L1SCTL2,
                                (UINT32) ~(B_PCIE_CFG_L1SCTL2_POWT | B_PCIE_CFG_L1SCTL2_TPOS),
                                (UINT32) ((0x5 << N_PCIE_CFG_L1SCTL2_POWT) | (V_DMI_L1S_TPOS << N_PCIE_CFG_L1SCTL2_TPOS))
                                );

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures TpowerOff.
  It cannot be executed prior to DMI3.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureTpowerOff (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 5.5.7
  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_PCIE_CFG_PCIEPMECTL,
                                (UINT32) ~B_PCIE_CFG_PCIEPMECTL_POFFWT,
                                (UINT32) (0x3 << N_PCIE_CFG_PCIEPMECTL_POFFWT)
                                );

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures implementation specific programming.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipImplementationSpecificProgramming (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 5.5.7.13
  RpDev->PciPcrAccess->And32 (RpDev->PciPcrAccess, R_PCIE_CFG_MPC2, (UINT32) ~B_PCIE_CFG_MPC2_DISPLLEWL1SE);
  RpDev->PciPcrAccess->And32 (RpDev->PciPcrAccess, R_PCIE_CFG_PCIEPMECTL, (UINT32) ~B_PCIE_CFG_PCIEPMECTL_L1OCREWD);
  if (RpDev->Integration != UpDmi3) {
    RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_CFG_PCIEALC, (UINT32) ~0u, (UINT32) B_PCIE_CFG_PCIEALC_ONPRASPML1P);
  }

  RpDev->PciPcrAccess->And32 (RpDev->PciPcrAccess, R_DMI_PCR_IPCAPOVR1, ~(UINT32) (B_DMI_PCR_IPCAPOVR1_PCMRTOVRV | B_DMI_PCR_IPCAPOVR1_PCMRTS));

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures Dynamic Link Throttling.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureDynamicLinkThrottling (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 5.5.12
  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_PCIE_CFG_TNPT,
                                ~(B_PCIE_CFG_TNPT_TP | B_PCIE_CFG_TNPT_TT | B_PCIE_CFG_TNPT_DRXLTE | B_PCIE_CFG_TNPT_DTXLTE),
                                0x4 << N_PCIE_CFG_TNPT_TP
                                );

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures FCP Timeout.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureFcpTimeout (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 5.9.3.6
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_CFG_AECR1G3, ~0u, B_PCIE_CFG_AECR1G3_FCPTODIS);

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures Completion Timeout.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureCompletionTimeout (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 5.9.5.8
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_CFG_RCTL, ~0u, B_PCIE_CFG_RCTL_CRSSVE);
  RpDev->PciPcrAccess->AndThenOr16 (RpDev->PciPcrAccess, R_DMI_PCR_ROOTCAP, (UINT16)~0, B_DMI_PCR_ROOTCAP_CRSSV);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_TXCRSTOCTL, ~0u, B_PCIE_MEM_TXCRSTOCTL_TXNPCTODIS);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_CFG_DCTL2, ~0u, B_PCIE_CFG_DCTL2_CTD);
  RpDev->PciPcrAccess->And32 (RpDev->PciPcrAccess, R_PCIE_MEM_FCTL, (UINT32) ~B_PCIE_MEM_FCTL_CRSPSEL);

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures Preallocation and Peer Disable.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigurePreallocationAndPeerDisable (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 5.9.5.9 and 6.1
  RpDev->PciPcrAccess->And32 (
                          RpDev->PciPcrAccess,
                          R_PCIE_MEM_FCTL2,
                          (UINT32) ~B_PCIE_MEM_FCTL2_RXCPPREALLOCEN
                          );
  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_PCIE_CFG_CCFG,
                                (UINT32) ~(B_PCIE_CFG_CCFG_UMRPD | B_PCIE_CFG_CCFG_UPMWPD),
                                (UINT32) B_PCIE_CFG_CCFG_NPAP
                                );

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures Forced Relaxed Ordering.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureForcedRelaxedOrdering (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 6.8
  RpDev->PciPcrAccess->And32 (
                          RpDev->PciPcrAccess,
                          R_PCIE_CFG_COCTL,
                          (UINT32) ~B_PCIE_CFG_COCTL_ROAOP
                          );
  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_DMI_PCR_IPSCE,
                                (UINT32) ~(B_DMI_PCR_IPSCE_RXROM | B_DMI_PCR_IPSCE_TXROM),
                                (UINT32) (V_DMI_PCR_IPSCE_RXROM << N_DMI_PCR_IPSCE_RXROM)
                                );

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures Precision Time Measurement on DMI3.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigurePtm (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 6.17
  // program capabilities
  RpDev->PciPcrAccess->AndThenOr32 (
                          RpDev->PciPcrAccess,
                          R_PCIE_CFG_PTMCAPR,
                          (UINT32) ~(B_PCIE_CFG_PTMCAPR_PTMPDAC | B_PCIE_CFG_PTMCAPR_PTMRC | B_PCIE_CFG_PTMCAPR_PTMRSPC),
                          (UINT32) (BIT3 | B_PCIE_CFG_PTMCAPR_PTMREQC)
                          );

  // program control
  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_PCIE_CFG_PTMCTLR,
                                (UINT32) ~B_PCIE_CFG_PTMCTLR_RS,
                                (UINT32) B_PCIE_CFG_PTMCTLR_PTME
                                );

  RpDev->PciPcrAccess->And32 (
                          RpDev->PciPcrAccess,
                          R_PCIE_MEM_DECCTL,
                          (UINT32) ~B_PCIE_MEM_DECCTL_LCRXPTMREQ
                          );

  // program global, local, and IOSF sideband TSC
  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_PCIE_CFG_PTMECFG,
                                (UINT32) ~(B_PCIE_CFG_PTMECFG_PLTLFF | B_PCIE_CFG_PTMECFG_PGTSCFE | B_PCIE_CFG_PTMECFG_IOSFMADP),
                                (UINT32) ((0x1 << N_PCIE_CFG_PTMECFG_PLTLFF) | (0x2 << N_PCIE_CFG_PTMECFG_IOSFMADP))
                                );

  // program local TSC

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures Pipe Stage Delay.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigurePipeStageDelay (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 6.17
  // This is awaiting an update from BWG owner
  //
  // Implementation specific programming
  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_PCIE_CFG_PTMECFG,
                                (UINT32) ~B_PCIE_CFG_PTMECFG_PTMRNOPAD,
                                (UINT32) (B_PCIE_CFG_PTMECFG_PTMRPAE | (0x1 << N_PCIE_CFG_PTMECFG_PTMRNOPAD))
                                );

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures MCTP on DMI3.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
Dmi3SipConfigureMctp (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  // BWG chapter 6.32
  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_PCIE_CFG_MPC,
                                (UINT32) ~(B_PCIE_CFG_MPC_MMBNCE | B_PCIE_CFG_MPC_BMERCE),
                                (UINT32) (B_PCIE_CFG_MPC_MCTPSE)
                                );

  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_PCIE_CFG_CCFG,
                                (UINT32) ~0u,
                                (UINT32) (B_PCIE_CFG_CCFG_CRE | B_PCIE_CFG_CCFG_IORE)
                                );
  RpDev->PciPcrAccess->AndThenOr32 (
                                RpDev->PciPcrAccess,
                                R_DMI_PCR_DECCTL1,
                                (UINT32) ~0u,
                                (UINT32) B_DMI_PCR_DECCTL1_EMSGRXFP
                                );

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures PCH DMI MultiVC support for RS3.
  Applicable from SIP17.

  @param[in]  RpDev                Pointer to the root port device
  @param[in]  Vc                   The virtual channel number for programing
**/
VOID
Dmi3SipRs3TcVcConfiguration (
  IN  PCIE_ROOT_PORT_DEV                *RpDev,
  UINT32                                *Data32Or
  )
{
  RpDev->PciPcrAccess->Or32 (
    RpDev->PciPcrAccess,
    R_DMI_PCR_RSPVCC,
    B_DMI_PCR_RSPVCC_RS3VCCP
    );
  RpDev->PciPcrAccess->And32 (
      RpDev->PciPcrAccess,
      R_DMI_PCR_RS3V0CTL,
      (UINT32) ~B_DMI_PCR_RS3V0CTL_TVM
    );
  RpDev->PciPcrAccess->AndThenOr32 (
      RpDev->PciPcrAccess,
      R_DMI_PCR_RS3V1CTL,
      (UINT32) ~B_DMI_PCR_RS3V1CTL_TVM,
      (UINT32) (B_DMI_PCR_RS3V1CTL_EN | (V_DMI_PCR_RS3V1CTL_TVM << N_DMI_PCR_RS3V1CTL_TVM))
    );
  *Data32Or = (UINT32) (B_PCIE_MEM_FCUCTL_FC_CP_FCM_VC1 | B_PCIE_MEM_FCUCTL_FC_CP_FCM);
}

/**
  This function configures PCH DMI MultiVC support.
  Applicable from SIP17.

  @param[in]  RpDev                Pointer to the root port device
  @param[in]  Vc                   The virtual channel number for programing
**/
VOID
DmiSipConfigurePchMultiVC (
  IN  PCIE_ROOT_PORT_DEV                *RpDev,
  IN  PCH_DMI_VC_TYPE                   Vc
  )
{
  UINT32                        Data32Or;
  UINT32                        Data32And;

  Data32And = ~0u;
  Data32Or = 0u;

  DEBUG ((DEBUG_INFO, "%a() Start on VC %d\n", __FUNCTION__, Vc));
  switch (Vc) {
    case PchDmiVcTypeVc0:
      Data32And = (UINT32)~B_PCIE_CFG_PVCCR1_EVCC;
      if (RpDev->Integration == UpDmi3) {
        // Set 2 VCs
        Data32Or = (UINT32)(0x1 << N_PCIE_CFG_PVCCR1_EVCC);
      } else {
        // Set 3 VCs
        Data32Or = (UINT32)(0x2 << N_PCIE_CFG_PVCCR1_EVCC);
      }
      RpDev->PciPcrAccess->AndThenOr32 (
        RpDev->PciPcrAccess,
        R_PCIE_CFG_PVCCR1,
        Data32And,
        Data32Or
        );
      Data32And = (UINT32)~(B_PCIE_CFG_VCCH_NCO | B_PCIE_CFG_VCCH_CV | B_PCIE_CFG_VCCH_CID);
      // Set to 3 VCs
      Data32Or = (UINT32)((0xA30 << N_PCIE_CFG_VCCH_NCO) |
                          (0x1 << N_PCIE_CFG_VCCH_CV) |
                          (0x2 << N_PCIE_CFG_VCCH_CID));
      RpDev->PciCfgAccess->AndThenOr32 (
        RpDev->PciCfgAccess,
        R_PCIE_CFG_VCCH,
        Data32And,
        Data32Or
        );
      Data32And = (RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_V0VCRC) &
               (UINT32)~B_PCIE_CFG_V0VCRC_MTS);
      RpDev->PciCfgAccess->Write32 (
        RpDev->PciCfgAccess,
        R_PCIE_CFG_V0VCRC,
        Data32And
        );
      break;

    case PchDmiVcTypeVc1:
      Data32And = (RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_V1VCRC) &
               (UINT32)~B_PCIE_CFG_V1VCRC_MTS);
      RpDev->PciCfgAccess->Write32 (
        RpDev->PciCfgAccess,
        R_PCIE_CFG_V1VCRC,
        Data32And
        );
      break;

    case PchDmiVcTypeVcm:
      break;

    case PchDmiVcTypeMax:
      ASSERT (FALSE);
  }

  if (RpDev->Integration == UpDmi3 || RpDev->Integration == DpDmi) {
    Dmi3SipRs3TcVcConfiguration (RpDev, &Data32Or);
  } else {
    Data32Or = B_PCIE_MEM_FCUCTL_FC_CP_FCM_VCM | B_PCIE_MEM_FCUCTL_FC_CP_FCM_VC1 | B_PCIE_MEM_FCUCTL_FC_CP_FCM;
  }

  RpDev->PciPcrAccess->AndThenOr32 (
    RpDev->PciPcrAccess,
    R_PCIE_MEM_FCUCTL,
    ~0u,
    Data32Or
    );
  DEBUG ((DEBUG_INFO, "%a() End on VC %d\n", __FUNCTION__, Vc));
}

/**
  Get PCH DMI SIP17 Virtual Channel Control and Status registers

  @param[in]  RpDev                   Pointer to the RpDev
  @param[in]  Vc                      The virtual channel number for programing
  @param[out] PchDmiVcCtlAddress      DMI Virtual Channel Control register address
  @param[out] PchDmiVcStsAddress      DMI Virtual Channel Status register address
**/
VOID
SipDmiVcRegs (
  IN   PCIE_ROOT_PORT_DEV  *RpDev,
  IN   PCH_DMI_VC_TYPE     Vc,
  OUT  UINT16              *PchDmiVcCtlAddress,
  OUT  UINT16              *PchDmiVcStsAddress
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  if (RpDev->SipVersion <= PcieSip15) {
    switch (Vc) {
      // Use SIP15 offsetts
      case PchDmiVcTypeVc0:
        *PchDmiVcCtlAddress = 0x284;
        *PchDmiVcStsAddress = 0x28A;
        break;

      case PchDmiVcTypeVc1:
        *PchDmiVcCtlAddress = 0x290;
        *PchDmiVcStsAddress = 0x296;
        break;

      case PchDmiVcTypeVcm:
        *PchDmiVcCtlAddress = 0x2B0;
        *PchDmiVcStsAddress = 0x2B6;
        break;

      case PchDmiVcTypeMax:
        *PchDmiVcCtlAddress = 0;
        *PchDmiVcStsAddress = 0;
        ASSERT (FALSE);
    }
  } else {
    switch (Vc) {
      case PchDmiVcTypeVc0:
        *PchDmiVcCtlAddress = R_PCIE_CFG_V0CTL;
        *PchDmiVcStsAddress = R_PCIE_CFG_V0STS;
        break;

      case PchDmiVcTypeVc1:
        *PchDmiVcCtlAddress = R_PCIE_CFG_V1CTL;
        *PchDmiVcStsAddress = R_PCIE_CFG_V1STS;
        break;

      case PchDmiVcTypeVcm:
        *PchDmiVcCtlAddress = 0x2B0;  //VMCTL
        *PchDmiVcStsAddress = 0x2B6;  //VMSTS
        break;

      case PchDmiVcTypeMax:
        *PchDmiVcCtlAddress = 0;
        *PchDmiVcStsAddress = 0;
        ASSERT (FALSE);
    }
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Programing transaction classes of the corresponding virtual channel and Enable it

  @param[in]  Vc                   The virtual channel number for programming
  @param[in]  VcId                 The Identifier to be used for this virtual channel
  @param[in]  VcMap                The transaction classes are mapped to this virtual channel.
                                   When a bit is set, this transaction class is mapped to the virtual channel
  @param[in]  RpDev                Pointer to the root port device

  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_SUCCESS              The function completed successfully
**/
STATIC
VOID
DmiSipSetTcVcMapping (
  IN   PCH_DMI_VC_TYPE            Vc,
  IN   UINT8                      VcId,
  IN   UINT8                      VcMap,
  IN   IN  PCIE_ROOT_PORT_DEV     *RpDev
  )
{
  UINT32                          VxCtlAnd = 0;
  UINT32                          VxCtlOr = 0;
  UINT16                          VcCtlAddress = 0;
  UINT16                          VcStsAddress = 0;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  SipDmiVcRegs (RpDev, Vc, &VcCtlAddress, &VcStsAddress);
  VxCtlAnd  = (UINT32) (~(B_DMI_PCR_V0CTL_ID | V_DMI_PCR_VXCTL_TVM_MASK));
  VxCtlOr   = ((UINT32) VcId << N_DMI_PCR_V0CTL_ID) & B_DMI_PCR_V0CTL_ID;
  VxCtlOr |= (UINT32) VcMap;
  VxCtlOr |= B_DMI_PCR_V0CTL_EN;

  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         VcCtlAddress,
                         VxCtlAnd,
                         VxCtlOr
                         );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Polling negotiation status of the corresponding virtual channel

  @param[in]  Vc                   The virtual channel number for programming
  @param[in]  RpDev                Pointer to the root port device

  @retval EFI_INVALID_PARAMETER    Invalid parameter.
  @retval EFI_SUCCESS              The function completed successfully
**/
STATIC
VOID
SipDmiPollVcStatus (
  IN PCH_DMI_VC_TYPE     Vc,
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  UINT16         VcCtlAddress;
  UINT16         VcStsAddress;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  SipDmiVcRegs (RpDev, Vc, &VcCtlAddress, &VcStsAddress);

  //
  // Wait for negotiation to complete
  //
  while ((RpDev->PciPcrAccess->Read16 (RpDev->PciPcrAccess, VcStsAddress) & B_PCH_DMI_PCR_V1STS_NP) != 0) {
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel
  until negotiation completion.

  @param[in]  PchDmiTcVcMap              Buffer for PCH_DMI_TC_VC_MAP instance.
  @param[in]  RpDev                      Pointer to the root port device

  @retval EFI_SUCCESS                   The function completed successfully
  @retval Others                        All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
DmiSipTcVcProgPoll (
  IN PCH_DMI_TC_VC_MAP   *PchDmiTcVcMap,
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  UINT8                   Index;
  UINT8                   VcMap[PchDmiVcTypeMax];

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  ZeroMem (VcMap, PchDmiVcTypeMax);

  //
  // Set the TC/VC mappings
  //
  for (Index = 0; Index < PchDmiTcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "TC:%0x VC:%0x!\n", PchDmiTcVcMap->DmiTc[Index].TcId, PchDmiTcVcMap->DmiTc[Index].Vc));
    VcMap[PchDmiTcVcMap->DmiTc[Index].Vc] |= (BIT0 << PchDmiTcVcMap->DmiTc[Index].TcId);
  }

  for (Index = 0; Index < PchDmiVcTypeMax; Index++) {
    DEBUG ((DEBUG_INFO, "VC:%0x VCID:%0x Enable:%0x!\n",Index, PchDmiTcVcMap->DmiVc[Index].VcId, PchDmiTcVcMap->DmiVc[Index].Enable));
    if (PchDmiTcVcMap->DmiVc[Index].Enable == TRUE) {
      DmiSipSetTcVcMapping (
        Index,
        PchDmiTcVcMap->DmiVc[Index].VcId,
        VcMap[Index],
        RpDev
        );
      DmiSipConfigurePchMultiVC (RpDev, Index);
    }
  }

  //
  // After both above and downstream DMI TC/VC mapping are programmed,
  // poll VC negotiation pending status until is zero:
  //
  if (RpDev->Integration != DpDmi) {
    for (Index = 0; Index < PchDmiVcTypeMax; Index++) {
      if (PchDmiTcVcMap->DmiVc[Index].Enable == TRUE) {
        SipDmiPollVcStatus (Index, RpDev);
      }
    }
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  This function checks if DMI SIP15 Secured Register Lock (SRL) is set

  @param[in]  RpDev                Pointer to the root port device
  @retval SRL state
**/
BOOLEAN
DmiSipIsPchDmiLocked (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() called\n", __FUNCTION__));
  return ((RpDev->PciPcrAccess->Read32 (RpDev->PciPcrAccess, R_PCIE_CFG_MPC) & BIT23) != 0);
}

/**
  Enable PCIe Relaxed Order for DMI SIP15

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipEnablePcieRelaxedOrder (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  //
  // Enable Forced Relaxed Ordering to always allow downstream completions to pass posted writes.
  // Set Completion Relaxed Ordering Attribute Override Value
  // and Completion Relaxed Ordering Attribute Override Enable
  //
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_CFG_PCIECFG2, ~0u, (B_PCIE_CFG_PCIECFG2_CROAOV | B_PCIE_CFG_PCIECFG2_CROAOE));
  if (RpDev->Integration == UpDmi3) {
    Dmi3SipConfigureForcedRelaxedOrdering (RpDev);
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function will switch SAI value to be driven to IOSF Primary Fabric
  for cycles with Core BDF from HOSTIA_BOOT_SAI to HOSTIA_POSTBOOT_SAI.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipEnablePostBootSai (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32 Data32Or;

  //
  // Lock SIP17 Registers
  //
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  Data32Or = (UINT32) (B_PCIE_CFG_LPCR_DIDOVR_LOCK | B_PCIE_CFG_LPCR_SERL);
  if (RpDev->Integration == UpDmi3) {
    Data32Or |= (UINT32) B_PCIE_CFG_LPCR_SRL;
  }
  if (RpDev->SipVersion >= PcieSip17) {
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_LPCR,
      (UINT32)~0u,
      Data32Or
      );
    RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
      RpDev->PciSbiMsgCfgAccess,
      R_PCIE_PCR_SRL,
      (UINT32)~0u,
      (UINT32)(B_PCIE_PCR_SRL_SRL)
      );
    DEBUG ((DEBUG_INFO, "PchDmi15EnablePostBootSai: Sip17lock active\n"));
  }
  if (RpDev->Integration == UpDmi3) {
    RpDev->PciPcrAccess->AndThenOr32 (
      RpDev->PciPcrAccess,
      R_PCIE_CFG_LPCRE,
      (UINT32)~0u,
      (UINT32)B_PCIE_CFG_LPCRE_IPVCCAPL
      );
  }
  RpDev->PciPcrAccess->AndThenOr32 (
    RpDev->PciPcrAccess,
    R_PCIE_CFG_CTRL1,
    ~(UINT32) (B_PCIE_CFG_CTRL1_L1RC | B_PCIE_CFG_CTRL1_L1RCEP | B_PCIE_CFG_CTRL1_L1PL),
    (BIT3 | V_PCIE_CFG_CTRL1_L1PL << N_PCIE_CFG_CTRL1_L1PL)
    );
  //
  // Read back to ensure that the programming through sideband completes
  // before any new transaction is generated by subsequent code.
  // Ordering between primary and sideband is not guaranteed and
  // primary transaction triggered afterwards may be completed
  // before IPCS_PSS programming is finished.
  //
  RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_CTRL1);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs basic DMI initialization.

  @param[in]  RpDev                Pointer to the root port device
**/
STATIC
VOID
DmiSipBasicInit (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT8   Data8Or;

  Data8Or = 0u;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  //
  // Configure Uncorrectable Error Mask
  // Set the Completer Abort Mask, Poisoned TLP and Unsupported Request Error Mask
  //
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_UEM,
    ~0u,
    (B_PCIE_CFG_UEM_URE | B_PCIE_CFG_UEM_CM | B_PCIE_CFG_UEM_PT)
    );

  //
  // Configure Transmit Datapath Flush Timer
  //  00b - Wait for 4 clocks prior to initiating rate or powerstate change.
  // Configure Transmit Configuration Change Wait Time
  //  00b - 128ns for GEN1 and GEN2 mode. 256ns for GEN3 mode.
  //
  RpDev->PciCfgAccess->AndThenOr8 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_PHYCTL2,
    (UINT8) ~(B_PCIE_CFG_PHYCTL2_TDFT | B_PCIE_CFG_PHYCTL2_TXCFGCHGWAIT),
    0
    );

  //
  // Configure Squelch Direction settings
  //
  if (RpDev->PrivateConfig.SquelchDirectionOverrideDisable) {
    RpDev->PciCfgAccess->AndThenOr8 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_PHYCTL3,
      (UINT8) ~(B_PCIE_CFG_PHYCTL3_SQDIROVREN | B_PCIE_CFG_PHYCTL3_SQDIRCTRL),
      (UINT8) 0
      );
  } else {
    RpDev->PciCfgAccess->AndThenOr8 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_PHYCTL3,
      (UINT8) ~0,
      (B_PCIE_CFG_PHYCTL3_SQDIROVREN | B_PCIE_CFG_PHYCTL3_SQDIRCTRL)
      );
  }

  //
  // Set Recovery Upconfiguration Disable
  //
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_MPC2,
    ~0u,
    B_PCIE_CFG_MPC2_RUD
    );

  //
  // Set De-skew Buffer Unload Pointer Increment
  //
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_PWRCTL,
    ~0u,
    B_PCIE_CFG_PWRCTL_DBUPI
    );

  //
  // Set FC to send update immediately after the CREDITS ALLOCATED register has been updated
  //
  if (RpDev->Integration == UpDmi3) {
    Data8Or = (UINT8) (0x4 << N_PCIE_CFG_MPC_FCP);
  }
  RpDev->PciCfgAccess->AndThenOr8 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_MPC,
    (UINT8) ~B_PCIE_CFG_MPC_FCP,
    Data8Or
    );

  if (RpDev->PrivateConfig.DoNotHangOnLinkDown) {
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_CTRL1,
      ~0u,
      BIT4
      );
  } else {
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_CTRL1,
      (UINT32)~BIT8,
      (UINT32) 0u
      );
  }
  if (RpDev->Integration == UpDmi3) {
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_IOSFC3TC,
      (UINT32)~B_PCIE_CFG_IOSFC3TC_C0CPDC,
      (UINT32) (0x4 << N_PCIE_CFG_IOSFC3TC_C0CPDC)
      );
    RpDev->PciPcrAccess->AndThenOr16 (RpDev->PciPcrAccess, R_DMI_PCR_ROOTCAP, (UINT16) ~0, B_DMI_PCR_ROOTCAP_CRSSV);
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Configure DMI SIP17 aggresive clock gating.
  This function must be called before ASPM is enabled

  @param[in]  RpDev                Pointer to the root port device
**/
STATIC
VOID
DmiSipConfigureAggresiveClockGating (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32   Data32Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  RpDev->PciPcrAccess->AndThenOr32 (
    RpDev->PciPcrAccess,
    R_PCIE_MEM_IPCLKCTR,
    ~0u,
    B_PCIE_MEM_IPCLKCTR_MDPCCEN | B_PCIE_MEM_IPCLKCTR_PXDCGE
    );

  //
  // Configure Hardware Save and Restore
  //
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_PGCTRL,
    (UINT32)~B_PCIE_CFG_PGCTRL_PMREQBLKRSPT,
    (UINT32)(0x2 << N_PCIE_CFG_PGCTRL_PMREQBLKRSPT)
    );

  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_PGCBCTL1,
    (UINT32)~B_PCIE_CFG_PGCBCTL1_TACCRSTUP,
    (UINT32)(0x3 << N_PCIE_CFG_PGCBCTL1_TACCRSTUP)
    );

  //
  // Configure Aggresive Clock Gating
  //
  Data32Or = (UINT32)(B_PCIE_MEM_DCGM1_PXTTSULDCGM | B_PCIE_MEM_DCGM1_PXTRSULDCGM |
                      B_PCIE_MEM_DCGM1_PXTRULDCGM | B_PCIE_MEM_DCGM1_PXLSULDCGM |
                      B_PCIE_MEM_DCGM1_PXLIULDCGM | B_PCIE_MEM_DCGM1_PXLTULDCGM |
                      B_PCIE_MEM_DCGM1_PXLRULDCGM | B_PCIE_MEM_DCGM1_PXCULDCGM |
                      B_PCIE_MEM_DCGM1_PXKGULDCGM);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_DCGM1, ~0u, Data32Or);

  Data32Or = (UINT32)(BIT17 | BIT16 |
                      BIT15 | B_PCIE_MEM_DCGM2_PXFRULDCGM |
                      B_PCIE_MEM_DCGM2_PXFTULDCGM | B_PCIE_MEM_DCGM2_PXPBULDCGM |
                      B_PCIE_MEM_DCGM2_PXPSULDCGM | B_PCIE_MEM_DCGM2_PXPIULDCGM);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_DCGM2, ~0u, Data32Or);

  Data32Or = (UINT32)(B_PCIE_MEM_DCGM3_PXTTSUPDCGM | B_PCIE_MEM_DCGM3_PXTTUPDCGM |
                      B_PCIE_MEM_DCGM3_PXTOUPDCGM | B_PCIE_MEM_DCGM3_PXTRSUPDCGM |
                      B_PCIE_MEM_DCGM3_PXTRUPDCGM | B_PCIE_MEM_DCGM3_PXLIUPDCGM |
                      B_PCIE_MEM_DCGM3_PXLTUPDCGM | B_PCIE_MEM_DCGM3_PXLRUPDCGM |
                      B_PCIE_MEM_DCGM3_PXSRUSSNRDCGM | B_PCIE_MEM_DCGM3_PXCUPSNRDCGM |
                      B_PCIE_MEM_DCGM3_PXCUPSRCDCGM | B_PCIE_MEM_DCGM3_PXBUPDCGM |
                      B_PCIE_MEM_DCGM3_PXEUPDCGM);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_DCGM3, ~0u, Data32Or);

  //
  // Enable Aggresive Clock Gating
  //
  Data32Or = (UINT32)(B_PCIE_MEM_DCGEN1_PXTTSULDCGEN | B_PCIE_MEM_DCGEN1_PXTRSULDCGEN |
                      B_PCIE_MEM_DCGEN1_PXTRULDCGEN | B_PCIE_MEM_DCGEN1_PXLSULDCGEN |
                      B_PCIE_MEM_DCGEN1_PXLIULDCGEN | B_PCIE_MEM_DCGEN1_PXLRULDCGEN |
                      B_PCIE_MEM_DCGEN1_PXCULDCGEN | B_PCIE_MEM_DCGEN1_PXKGULDCGEN);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_DCGEN1, ~0u, Data32Or);

  Data32Or = (UINT32)(BIT17 | BIT16 |
                      BIT15 | B_PCIE_MEM_DCGEN2_PXFRULDCGEN |
                      B_PCIE_MEM_DCGEN2_PXFTULDCGEN | B_PCIE_MEM_DCGEN2_PXPBULDCGEN |
                      B_PCIE_MEM_DCGEN2_PXPSULDCGEN |B_PCIE_MEM_DCGEN2_PXPIULDCGEN);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_DCGEN2, ~0u, Data32Or);

  Data32Or = (UINT32)(B_PCIE_MEM_DCGEN3_PXTTSUPDCGEN | B_PCIE_MEM_DCGEN3_PXTTUPDCGEN |
                      B_PCIE_MEM_DCGEN3_PXTOUPDCGEN | B_PCIE_MEM_DCGEN3_PXTRSUPDCGEN |
                      B_PCIE_MEM_DCGEN3_PXTRUPDCGEN | B_PCIE_MEM_DCGEN3_PXLIUPDCGEN |
                      B_PCIE_MEM_DCGEN3_PXLRUPDCGEN | B_PCIE_MEM_DCGEN3_PXSRUSSNRDCGEN |
                      B_PCIE_MEM_DCGEN3_PXBUPDCGEN | B_PCIE_MEM_DCGEN3_PXEUPDCGEN);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_DCGEN3, ~0u, Data32Or);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Configure DMI clock gating.
  This function must be called before ASPM is enabled

  @param[in]  RpDev                Pointer to the root port device
**/
STATIC
VOID
DmiSipConfigureClockGating (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  //
  // Enable Dynamic Clock Gating on ISM Active
  //
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_CCFG,
    ~0u,
    B_PCIE_CFG_CCFG_DCGEISMA
    );

  //
  // Enable below clock gating settings:
  // - Partition/Trunk Oscillator Clock Gate Enable
  // - Link CLKREQ Enable
  // - Backbone CLKREQ Enable
  // - Shared Resource Dynamic Backbone Clock Gate Enable
  // - Root Port Dynamic Link Clock Gate Enable
  // - Root Port Dynamic Backbone Clock Gate Enable
  //
  RpDev->PciCfgAccess->AndThenOr8 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_RPDCGEN,
    (UINT8)~0,
    (B_PCIE_CFG_RPDCGEN_PTOCGE | B_PCIE_CFG_RPDCGEN_LCLKREQEN | B_PCIE_CFG_RPDCGEN_BBCLKREQEN |
    B_PCIE_CFG_RPDCGEN_SRDBCGEN | B_PCIE_CFG_RPDCGEN_RPDLCGEN | B_PCIE_CFG_RPDCGEN_RPDBCGEN)
    );

  //
  // Enable Side Clock Partition/Trunk Clock Gating
  // Set IOSF Sideband Interface Idle Counter to 00b - Wait for
  // 32 idle clocks before allowing trunk clock gating.
  //
  RpDev->PciCfgAccess->AndThenOr8 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_IOSFSBCS,
    (UINT8)~B_PCIE_CFG_IOSFSBCS_SIID,
    B_PCIE_CFG_IOSFSBCS_SCPTCGE
    );

  //
  // Enable Sideband Endpoint Oscillator/Side Clock Gating
  //
  if (RpDev->SipVersion >= PcieSip17) {
    RpDev->PciCfgAccess->AndThenOr8 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_RPPGEN,
      (UINT8) ~(B_PCIE_CFG_RPPGEN_L23R2DT),
      (B_PCIE_CFG_RPPGEN_SEOSCGE | BIT6)
      );
    //
    // Program OSC Clock Gate Hysteresis
    //
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_PCIEPMECTL3,
      (UINT32)~(B_PCIE_CFG_PCIEPMECTL3_L1PGAUTOPGEN | B_PCIE_CFG_PCIEPMECTL3_OSCCGH | B_PCIE_CFG_PCIEPMECTL3_PMREQCPGEXH),
      (UINT32) (0x1 << N_PCIE_CFG_PCIEPMECTL3_OSCCGH)
      );
    DmiSipConfigureAggresiveClockGating (RpDev);
  } else {
    RpDev->PciCfgAccess->AndThenOr8 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_RPPGEN,
      (UINT8)~0,
      B_PCIE_CFG_RPPGEN_SEOSCGE
      );
  }
  if (RpDev->Integration == UpDmi3) {
    // BWG chapter 5.5.3
    // Configure clock gating on IOSF Primary
    RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_IPCLKCTR, ~0u, B_PCIE_MEM_IPCLKCTR_MDPCCEN);
    // Configure Unit Level clock gating
    RpDev->PciPcrAccess->And32 (RpDev->PciPcrAccess, R_PCIE_MEM_DCGM1, (UINT32) ~B_PCIE_MEM_DCGM1_PXTTULDCGM);
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Configure DMI link power management.
  This function must be called before ASPM is enabled

  @param[in]  PchPcieConfig        Pointer to the PcieRpConfig
  @param[in]  RpDev                Pointer to the root port device
**/
STATIC
VOID
DmiSipConfigureLinkPowerManagement (
  IN  PCH_PCIE_CONFIG              *PchPcieConfig,
  IN  PCIE_ROOT_PORT_DEV           *RpDev
  )
{
  UINT8                 Data8And;
  UINT8                 Data8Or;
  UINT32                Data32And;
  UINT32                Data32Or;

  Data32And = ~0u;
  Data32Or = 0u;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  if (RpDev->SipVersion >= PcieSip17) {
    //
    // Programming DMI L1 preparation latency
    // to 4us for PCH A0 stepping,, and to 2us for other
    //
    if (PchStepping () > PCH_A0) {
      Data8Or  = (0x2 << 9) >> 8;
    } else {
      Data8Or  = (0x3 << 9) >> 8;
    }
  } else {
    Data8Or  = (0x3 << 9) >> 8;
  }
  Data8And = (UINT8)~((BIT11 | BIT10 | BIT9) >> 8);
  /**
   Allow DMI enter L1 when all root ports are in L1, L0s or link down. Disabled by default.
   When set to TRUE turns on:
     - L1 State Controller Power Gating
     - L1 State PHY Data Lane Power Gating
     - PHY Common Lane Power Gating
     - Hardware Autonomous Enable
     - PMC Request Enable and Sleep Enable
  **/
  if (PchPcieConfig->DmiPort.PcieRpCommonConfig.ClockGating == FALSE) {
    Data8And &= (UINT8)~(BIT12 >> 8);
  }
  //
  // Configure DMI L1 Preparation Latency to 4us - this is the time link layer has to
  // indicate IDLE before the link initialization and control logic enters Active State L1.
  // Be careful not to touch first byte of this register as CTRL1.PSS is RWO.
  //

  RpDev->PciCfgAccess->AndThenOr8 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_CTRL1 + 1,
    Data8And,
    Data8Or
    );

  //
  // Configure Gen1 and Gen2 Active State L0s Preparation Latency - time that link layer has to
  // indicate IDLE before the link initialization and control logic enters L0s
  // Set it to 0x14 clocks
  //
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_PCIEL0SC,
    (UINT32)~(B_PCIE_CFG_PCIEL0SC_G2ASL0SPL | B_PCIE_CFG_PCIEL0SC_G1ASL0SPL),
    ((0x14 << N_PCIE_CFG_PCIEL0SC_G2ASL0SPL) |
    (0x14 << N_PCIE_CFG_PCIEL0SC_G1ASL0SPL))
    );

  //
  // Configure Gen1 and Gen2 Common Clock N_FTS
  //
  if (RpDev->SipVersion >= PcieSip17) {
    Data32And = (UINT32)~(B_PCIE_CFG_PCIENFTS_G2UCNFTS | B_PCIE_CFG_PCIENFTS_G2CCNFTS |
                          B_PCIE_CFG_PCIENFTS_G1UCNFTS | B_PCIE_CFG_PCIENFTS_G1CCNFTS);
    if (RpDev->Integration == UpDmi3) {
      Data32Or = (UINT32)((0x7E << N_PCIE_CFG_PCIENFTS_G2UCNFTS) |
                          (0x5B << N_PCIE_CFG_PCIENFTS_G2CCNFTS) |
                          (0x3F << N_PCIE_CFG_PCIENFTS_G1UCNFTS) |
                          (0x2C << N_PCIE_CFG_PCIENFTS_G1CCNFTS));
    } else {
      Data32Or = (UINT32)((0x7E << N_PCIE_CFG_PCIENFTS_G2UCNFTS) |
                          (0xC8 << N_PCIE_CFG_PCIENFTS_G2CCNFTS) |
                          (0x3F << N_PCIE_CFG_PCIENFTS_G1UCNFTS) |
                          (0xBC << N_PCIE_CFG_PCIENFTS_G1CCNFTS));
    }
    RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_PCIENFTS, Data32And, Data32Or);

    Data32And = (UINT32)~(B_PCIE_CFG_G3L0SCTL_G3ASL0SPL | B_PCIE_CFG_G3L0SCTL_G3UCNFTS | B_PCIE_CFG_G3L0SCTL_G3CCNFTS);
    if (RpDev->Integration == UpDmi3) {
      Data32Or = (UINT32)((0x28 << N_PCIE_CFG_G3L0SCTL_G3ASL0SPL) |
                          (0x40 << N_PCIE_CFG_G3L0SCTL_G3UCNFTS) |
                          (0x2C << N_PCIE_CFG_G3L0SCTL_G3CCNFTS));
    } else {
      Data32Or = (UINT32)((0x28 << N_PCIE_CFG_G3L0SCTL_G3ASL0SPL) |
                          (0x40 << N_PCIE_CFG_G3L0SCTL_G3UCNFTS) |
                          (0x4B << N_PCIE_CFG_G3L0SCTL_G3CCNFTS));
    }
    RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_G3L0SCTL, Data32And, Data32Or);

    //
    // Configure Gen3 Common Clock N_FTS to 0x70
    //
    Data32And = (UINT32)~(B_PCIE_CFG_G4L0SCTL_G4ASL0SPL | B_PCIE_CFG_G4L0SCTL_G4L0SIC |
                          B_PCIE_CFG_G4L0SCTL_G4UCNFTS | B_PCIE_CFG_G4L0SCTL_G4CCNFTS);
    if (RpDev->Integration == UpDmi3) {
      Data32Or = (UINT32) ((0x28 << N_PCIE_CFG_G4L0SCTL_G4ASL0SPL) |
                           (0x3 << N_PCIE_CFG_G4L0SCTL_G4L0SIC) |
                           (0x80 << N_PCIE_CFG_G4L0SCTL_G4UCNFTS) |
                           (0x5B << N_PCIE_CFG_G4L0SCTL_G4CCNFTS));
    } else {
      Data32Or = (UINT32) ((0x28 << N_PCIE_CFG_G4L0SCTL_G4ASL0SPL) |
                           (0x3 << N_PCIE_CFG_G4L0SCTL_G4L0SIC) |
                           (0x80 << N_PCIE_CFG_G4L0SCTL_G4UCNFTS) |
                           (0x5E << N_PCIE_CFG_G4L0SCTL_G4CCNFTS));
    }
    RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_G4L0SCTL, Data32And, Data32Or);

    Data32And = (UINT32)~(B_PCIE_MEM_G5L0SCTL_G5UCNFTS | B_PCIE_MEM_G5L0SCTL_G5CCNFTS);
    Data32Or = (UINT32)0u;
    RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_MEM_G5L0SCTL, Data32And, Data32Or);
  } else {
    //
    // Configure Gen3 Active State L0s Preparation Latency - time that link layer has to
    // indicate IDLE before the link initialization and control logic enters L0s
    // Set it to 0x28 clocks
    // Set Gen3 Common Clock N_FTS to 0x38
    //
    Data32And = (UINT32)~(B_PCIE_CFG_PCIENFTS_G2CCNFTS | B_PCIE_CFG_PCIENFTS_G1CCNFTS);
    Data32Or = (UINT32)((0x70 << N_PCIE_CFG_PCIENFTS_G2CCNFTS) | (0x38 << N_PCIE_CFG_PCIENFTS_G1CCNFTS));
    RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_PCIENFTS, Data32And, Data32Or);

    Data32And = (UINT32)~(B_PCIE_CFG_G3L0SCTL_G3ASL0SPL | B_PCIE_CFG_G3L0SCTL_G3CCNFTS);
    Data32Or = (UINT32)(((0x28 << N_PCIE_CFG_G3L0SCTL_G3ASL0SPL) | (0x2c << N_PCIE_CFG_G3L0SCTL_G3CCNFTS)));
    RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_G3L0SCTL, Data32And, Data32Or);
  }

  //
  // Configure L1.2, L1.1 and L1 Standard
  //
  Data32And = (UINT32)~0u;
  if (RpDev->Integration == UpDmi3) {
    Data32And &= (UINT32) ~(B_PCIE_CFG_L1SCAP_PTV | B_PCIE_CFG_L1SCAP_PTPOS |
                            B_PCIE_CFG_L1SCAP_AL11S | B_PCIE_CFG_L1SCAP_PPL11S |
                            B_PCIE_CFG_L1SCAP_PPL12S);
    Data32Or = (UINT32)((0x5 << N_PCIE_CFG_L1SCAP_PTV) | (V_DMI_L1S_TPOS << N_PCIE_CFG_L1SCAP_PTPOS) |
                        B_PCIE_CFG_L1SCAP_L1PSS | B_PCIE_CFG_L1SCAP_AL12S);
  } else {
    Data32Or = (UINT32)(B_PCIE_CFG_L1SCAP_L1PSS | B_PCIE_CFG_L1SCAP_AL11S |
                        B_PCIE_CFG_L1SCAP_AL12S | B_PCIE_CFG_L1SCAP_PPL11S |
                        B_PCIE_CFG_L1SCAP_PPL12S);
  }
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_CFG_L1SCAP, Data32And, Data32Or);
  if (RpDev->Integration == UpDmi3) {
    // BWG chapter 5.5.4
    // Configure L1.LOW clock request early wake
    RpDev->PciPcrAccess->And32 (RpDev->PciPcrAccess, R_PCIE_CFG_PCIEPMECTL, (UINT32) ~B_PCIE_CFG_PCIEPMECTL_L1SNZCREWD);
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Configure DMI Squelch Power Management.

  @param[in]  RpDev                Pointer to the root port device
**/
STATIC
VOID
DmiSipConfigureSquelchPowerManagement (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  if (!(RpDev->SipVersion >= PcieSip17)) {
    //
    // Enable Low Bandwidth Squelch Settling Timer
    // Set this before enabling any of the squelch power management
    //
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_PCIECFG2,
      ~0u,
      BIT30
      );
    //
    // Enable Squelch propagation control
    //
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_PCIEDBG,
      ~0u,
      B_PCIE_CFG_PCIEDBG_SPCE
    );
  }

  //
  // Configure Root Port Squelch Exit Wait Latency to 120ns
  //
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_PWRCTL,
    (UINT32) ~B_PCIE_CFG_PWRCTL_RPSEWL,
    0x1 << N_PCIE_CFG_PWRCTL_RPSEWL
    );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Configure DMI PLL Shutdown.

  @param[in]  RpDev                Pointer to the root port device
**/
STATIC
VOID
DmiSipConfigurePllShutdown (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;
  UINT8    Data8And;
  UINT8    Data8Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  //
  // Configure PLL Wait to 0us.This is amount of time to wait from gating the link clock
  // before turning off the PLL.
  // Enable Gen3 PLL coupling
  // This needs to be done before enabling PLL shutdown
  //
  Data32And = (UINT32) ~B_PCIE_CFG_MPC2_PLLWAIT;
  Data32Or = (UINT32) (B_PCIE_CFG_MPC2_GEN2PLLC | B_PCIE_CFG_MPC2_GEN3PLLC);
  RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_MPC2, Data32And, Data32Or);

  //
  // Enable PCI Express GEN2 and GEN3 PLL Off
  // If enabled and no devices require the use of the PXP PLL clock outputs,
  // the PXP PLL can be shutdown.
  // This programming should be done before enabling ASPM
  //
  Data8And = (UINT8) ~0;
  Data8Or = (UINT8) (B_PCIE_CFG_PHYCTL2_PXPG3PLLOFFEN | B_PCIE_CFG_PHYCTL2_PXPG2PLLOFFEN);
  RpDev->PciCfgAccess->AndThenOr8 (RpDev->PciCfgAccess, R_PCIE_CFG_PHYCTL2, Data8And, Data8Or);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Configure DMI Power Gating.
  This needs to done before enabling ASPM L1

  @param[in]  PchPcieConfig        The PCH Policy PPI instance
  @param[in]  RpDev                Pointer to the root port device
**/
STATIC
VOID
DmiSipConfigurePowerGating (
  IN  PCH_PCIE_CONFIG              *PchPcieConfig,
  IN  PCIE_ROOT_PORT_DEV           *RpDev
  )
{
  UINT32            AndData;
  UINT32            OrData;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  //
  // Disable the squelch circuitry for all lanes
  //
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_PHYCTL4,
    ~0u,
    B_PCIE_CFG_PHYCTL4_SQDIS
    );

  if (RpDev->Integration == UpDmi3) {
    // Configure LTSSM Timer
    // BWG chapter 5.9.3.7
    // Configure L1 Power Gating
    // BWG chapter 5.5.7
    RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_CFG_CTRL2, (UINT32) ~B_PCIE_CFG_CTRL2_CLWSWPT, (UINT32) (0x3 << N_PCIE_CFG_CTRL2_CLWSWPT));
    RpDev->PciPcrAccess->And32 (RpDev->PciPcrAccess, R_PCIE_CFG_PGTHRES, (UINT32) ~B_PCIE_CFG_PGTHRES_L1PGLTREN);
  }
  //
  // Disable IP-Accessible Context Propagation
  // Enable PHY Lane Power Gating when link resides in LTSSM Disabled, Detect or L23_Rdy state
  // Enable disabling squelch when the link resides in LTSSM Disabled, L23_Rdy state or if the lane is unconfigured
  // Enable IP-Inaccessible Entry Policy
  //
  if (RpDev->SipVersion >= PcieSip17) {
    AndData = (UINT32) ~B_PCIE_CFG_PWRCTL_DARECE;
    OrData = (UINT32) (B_PCIE_CFG_PWRCTL_LIFECF | B_PCIE_CFG_PWRCTL_TXSWING);
    if (RpDev->Integration != UpDmi3) {
      OrData |= B_PCIE_CFG_PWRCTL_WPDMPGEP;
    }
    RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_PWRCTL, AndData, OrData);
    AndData = (UINT32)~(B_PCIE_CFG_PCIEPMECTL_IPACPE | B_PCIE_CFG_PCIEPMECTL_DLSULDLSD);
    OrData = (UINT32) (B_PCIE_CFG_PCIEPMECTL_FDPPGE | B_PCIE_CFG_PCIEPMECTL_DLSULPPGE | B_PCIE_CFG_PCIEPMECTL_IPIEP);
  } else {
    AndData = (UINT32)~B_PCIE_CFG_PCIEPMECTL_IPACPE;
    OrData = (UINT32) (B_PCIE_CFG_PCIEPMECTL_DLSULPPGE | B_PCIE_CFG_PCIEPMECTL_DLSULDLSD | B_PCIE_CFG_PCIEPMECTL_IPIEP);
  }
  RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_PCIEPMECTL, AndData, OrData);

  if (PchPcieConfig->DmiPowerReduction) {
    //
    // Enable L1 State Controller Power Gating
    // Enable L1 State PHY Data Lane Power Gating
    // Enable PHY Common Lane Power Gating
    //
    if (RpDev->SipVersion >= PcieSip17) {
      //
      // Configure PHY Power Gating
      //
      RpDev->PciCfgAccess->AndThenOr32 (
        RpDev->PciCfgAccess,
        R_PCIE_MEM_PHYPG,
        ~0u,
        (B_PCIE_MEM_PHYPG_DLPPGP | B_PCIE_MEM_PHYPG_DUCFGPHYPGE |
         B_PCIE_MEM_PHYPG_L23PHYPGE | B_PCIE_MEM_PHYPG_DISPHYPGE |
         B_PCIE_MEM_PHYPG_DETPHYPGE)
        );
      AndData = (UINT32)~(B_PCIE_CFG_PCIEPMECTL2_CPGEXH | B_PCIE_CFG_PCIEPMECTL2_CPGENH);
      OrData = (UINT32)(B_PCIE_CFG_PCIEPMECTL2_PHYCLPGE | B_PCIE_CFG_PCIEPMECTL2_L1SPHYDLPGE | B_PCIE_CFG_PCIEPMECTL2_FDCPGE |
                        B_PCIE_CFG_PCIEPMECTL2_DETSCPGE | B_PCIE_CFG_PCIEPMECTL2_L23RDYSCPGE | B_PCIE_CFG_PCIEPMECTL2_DISSCPGE |
                        B_PCIE_CFG_PCIEPMECTL2_L1SCPGE | B_PCIE_CFG_PCIEPMECTL2_CPMCSRE);
      if (RpDev->Integration == UpDmi3) {
        RpDev->PciCfgAccess->AndThenOr32 (
          RpDev->PciCfgAccess,
          R_PCIE_CFG_PCIEPMECTL,
          (UINT32)~(B_PCIE_CFG_PCIEPMECTL_DLSULDLSD | B_PCIE_CFG_PCIEPMECTL_L1FSOE |
                    B_PCIE_CFG_PCIEPMECTL_L1LTRTLSV | B_PCIE_CFG_PCIEPMECTL_L1LTRTLV),
          (UINT32)(B_PCIE_CFG_PCIEPMECTL_DLSULPPGE |
                  (0x2 << N_PCIE_CFG_PCIEPMECTL_L1LTRTLSV) |
                  (0x41 << N_PCIE_CFG_PCIEPMECTL_L1LTRTLV))
          );
      } else {
        RpDev->PciCfgAccess->AndThenOr32 (
          RpDev->PciCfgAccess,
          R_PCIE_CFG_PCIEPMECTL,
          (UINT32)~(B_PCIE_CFG_PCIEPMECTL_DLSULDLSD | B_PCIE_CFG_PCIEPMECTL_L1FSOE |
                    B_PCIE_CFG_PCIEPMECTL_L1LTRTLSV | B_PCIE_CFG_PCIEPMECTL_L1LTRTLV),
          (UINT32)(B_PCIE_CFG_PCIEPMECTL_DLSULPPGE |
                  (0x2 << N_PCIE_CFG_PCIEPMECTL_L1LTRTLSV) |
                  (0x32 << N_PCIE_CFG_PCIEPMECTL_L1LTRTLV))
          );
      }
    } else {
      AndData = (UINT32)~0u;
      OrData = (UINT32)(B_PCIE_CFG_PCIEPMECTL2_PHYCLPGE | B_PCIE_CFG_PCIEPMECTL2_L1SPHYDLPGE | B_PCIE_CFG_PCIEPMECTL2_L1SCPGE);
    }
  } else if (RpDev->SipVersion >= PcieSip17) {
    AndData = (UINT32)~0u;
    OrData = (UINT32)(B_PCIE_CFG_PCIEPMECTL2_L1SPHYDLPGE);
  }
  if (RpDev->PrivateConfig.DisableDmiPg) {
    AndData = (UINT32)~0u;
    OrData = (UINT32)(B_PCIE_CFG_PCIEPMECTL2_CPMCSRE | B_PCIE_CFG_PCIEPMECTL2_PHYCLPGE);
  }
  RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_PCIEPMECTL2, AndData, OrData);

  if (PchPcieConfig->DmiPowerReduction) {
    //
    // Set Hardware Autonomous Enable,
    // PMC Request Enable and Sleep Enable
    //
    OrData = B_PCIE_CFG_PCE_SE;
    if (!RpDev->PrivateConfig.DisableDmiPg) {
      OrData |= B_PCIE_CFG_PCE_HAE;
    }
    AndData = (UINT32)~(B_PCIE_CFG_PCE_HAE | B_PCIE_CFG_PCE_PMCRE);
  } else {
    //
    // Clear Hardware Autonomous Enable and Sleep Enable
    //
    AndData = (UINT32)~(B_PCIE_CFG_PCE_HAE | B_PCIE_CFG_PCE_PMCRE | B_PCIE_CFG_PCE_SE);
    if (RpDev->SipVersion >= PcieSip17) {
      OrData = B_PCIE_CFG_PCE_SE;
    } else {
      OrData = 0;
    }
  }
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_PCE,
    AndData,
    OrData
    );

  if (!(RpDev->SipVersion >= PcieSip17)) {
    //
    // CPG Exit Link Clock Wake Disable
    //
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_CTRL2,
      ~0u,
      BIT5
      );
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Sets ASPM setting in PCH DMI PreMem Info HOB.
  In case HOB doesn't exist it creates it, and fills in with value being passed to function.
  HOB carries to another phase information about PCH DMI ASPM that's been set.

  @param[in]  PchDmiAspm        PCH DMI ASPM setting to be set in the HOB

  @retval EFI_SUCCESS           The PCH DMI PreMem Info HOB has been created
  @retval EFI_OUT_OF_RESOURCES  Insufficient resources for HOB creation
**/
EFI_STATUS
DmiSipSetAspmInPchDmiPreMemInfoHob (
  IN UINT8 PchDmiAspm
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  PCH_DMI_PREMEM_INFO   PchDmiPreMemInfo;
  EFI_HOB_GUID_TYPE     *GuidHobPointer;
  PCH_DMI_PREMEM_INFO   *PchDmiPreMemInfoPtr;

  PchDmiPreMemInfoPtr = NULL;
  GuidHobPointer = GetFirstGuidHob (&gPchDmiPreMemInfoHobGuid);
  if (GuidHobPointer == NULL) {
    // If HOB doesn't exist we need to create one.
    PchDmiPreMemInfo.PchDmiAspm = PchDmiAspm;
    BuildGuidDataHob (&gPchDmiPreMemInfoHobGuid, &PchDmiPreMemInfo, sizeof (PCH_DMI_PREMEM_INFO));
    GuidHobPointer = GetFirstGuidHob (&gPchDmiPreMemInfoHobGuid);
  }
  if (GuidHobPointer != NULL) {
    // If HOB exists already, we need to update it's data with policy setting.
    PchDmiPreMemInfoPtr = GET_GUID_HOB_DATA (GuidHobPointer);
    PchDmiPreMemInfoPtr->PchDmiAspm = PchDmiAspm;
    Status = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO, "%a: PCH DMI ASPM in the HOB: %d\n", __FUNCTION__, PchDmiPreMemInfoPtr->PchDmiAspm));
  } else {
    Status = EFI_OUT_OF_RESOURCES;
  }
  return Status;
}

/**
  This function prepares pre ASPM programming on DMI

  @param[in]  DmiAspmCtrl State of PCH DMI ASPM
  @param[out] Data16And   Pointer to data to be ANDed to LCTL
  @param[out] Data16Or    Pointer to data to be ORed to LCTL
**/
VOID
DmiSipGetPreAspmSettings (
  IN  PCH_PCIE_ASPM_CONTROL    DmiAspmCtrl,
  OUT UINT16                  *Data16And,
  OUT UINT16                  *Data16Or
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  *Data16And = (UINT16) ~B_PCIE_CFG_LCTL_ASPM;

  if (DmiAspmCtrl == PchPcieAspmL0sL1) {
    //
    // Enable L0s/L1 on DMI
    //
    *Data16Or = (UINT16) 0x3;
  } else if (DmiAspmCtrl == PchPcieAspmL0s) {
    //
    // Enable L0s Entry only
    //
    *Data16Or = (UINT16) 0x1;
  } else if ((DmiAspmCtrl == PchPcieAspmAutoConfig) || (DmiAspmCtrl == PchPcieAspmL1)) {
    //
    // Enable L1 Entry only
    //
    *Data16Or = (UINT16) 0x2;
  } else {
    //
    // ASPM Disabled
    //
    *Data16Or  = (UINT16) 0x0;
  }

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
  return;
}

/**
  This function configures ASPM on DMI

  @param[in]  PchPcieConfig        Pointer to PCH PCIe Config block
  @param[in]  PchGeneralConfig     Pointer to PCH General Config block
  @param[in]  RpDev                Pointer to the root port device
**/

VOID
DmiSipConfigureAspm (
  IN  PCH_PCIE_CONFIG              *PchPcieConfig,
  IN  PCH_GENERAL_CONFIG           *PchGeneralConfig,
  IN  PCIE_ROOT_PORT_DEV           *RpDev
  )
{
  UINT16                           Data16And;
  UINT16                           Data16Or;
  UINT32                           Data32And;
  UINT32                           Data32Or;
  PCH_PCIE_ASPM_CONTROL            DmiAspmCtrl;

  Data16And = (UINT16) 0;
  Data16Or = (UINT16) 0;
  Data32And = ~0u;
  Data32Or = 0u;
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  //
  // Enable DMI ASPM
  //
  if (PchGeneralConfig->LegacyIoLowLatency) {
    DmiAspmCtrl = PchPcieAspmDisabled;
  } else {
    DmiAspmCtrl = PchPcieConfig->DmiPort.PcieRpCommonConfig.Aspm;
  }

  DmiSipGetPreAspmSettings (DmiAspmCtrl, &Data16And, &Data16Or);

  if ((RpDev->SipVersion >= PcieSip17) || RpDev->Integration == UpDmi3) {
    if (DmiAspmCtrl == PchPcieAspmL0sL1) {
      //
      // Enable L0s/L1 on DMI
      //
      Data32Or = (UINT32) (0x3 << N_PCIE_CFG_LCAP_APMS);
    } else if (DmiAspmCtrl == PchPcieAspmL0s) {
      //
      // Enable L0s Entry only
      //
      Data32Or = (UINT32) (0x1 << N_PCIE_CFG_LCAP_APMS);
    } else if ((DmiAspmCtrl == PchPcieAspmAutoConfig) || (DmiAspmCtrl == PchPcieAspmL1)) {
      //
      // Enable L1 Entry only
      //
      Data32Or = (UINT32) (0x2 << N_PCIE_CFG_LCAP_APMS);
    } else {
      //
      // ASPM Disabled
      //
      Data32Or = (UINT32) (0x0 << N_PCIE_CFG_LCAP_APMS);
    }
    Data32And = (UINT32)~(B_PCIE_CFG_LCAP_EL1 | B_PCIE_CFG_LCAP_EL0 | B_PCIE_CFG_LCAP_APMS);
    Data32Or |= (UINT32) ((0x6 << N_PCIE_CFG_LCAP_EL1) |
                          (0x4 << N_PCIE_CFG_LCAP_EL0) | B_PCIE_CFG_LCAP_ASPMOC);
  } else {
    Data32And = (UINT32) ~(B_PCIE_CFG_LCAP_EL1 | B_PCIE_CFG_LCAP_EL0);
    Data32Or |= (UINT32)((0x4 << N_PCIE_CFG_LCAP_EL1) |  ///< L1 Exit Latency = 8us to less than 16us
                         (0x3 << N_PCIE_CFG_LCAP_EL0));  ///< L0 Exit Latency = 256ns to less than 512ns
  }

  //
  // Configue DMI ASPM
  //
  RpDev->PciCfgAccess->AndThenOr16 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_LCTL,
    Data16And,
    Data16Or
    );
  RpDev->PciSbiMsgMemAccess->AndThenOr32 (
    RpDev->PciSbiMsgMemAccess,
    R_PCIE_MEM_LLPC,
    (UINT32) ~0u,
    (UINT32) B_PCIE_MEM_LLPC_AL1NTP
    );

  if ((DmiAspmCtrl != PchPcieAspmDisabled) &&
    (PchPcieConfig->DmiPort.ExtSync == FORCE_ENABLE)) {
    //
    // Enable Extended Synch
    //
    Data16And = (UINT16)~0;
    Data16Or  = BIT7;
    RpDev->PciCfgAccess->AndThenOr16 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_LCTL,
      Data16And,
      Data16Or
      );
  }

  //
  // Configure L0s Exit Latency to 0x3 (128ns - 256ns)
  // Configure L1s Exit Latency to 0x4 (8us - 16us)
  //
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_LCAP,
    Data32And,
    Data32Or
    );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function reads Supported ASPM stats from Link Capabilities register.

  @param[in]  RpDev        Pointer to the root port device
  @param[out] PchDmiAspm   PCH DMI ASPM Supported
**/
VOID
DmiSipGetAspmSupported (
  IN  PCIE_ROOT_PORT_DEV          *RpDev,
  OUT UINT8                       *PchDmiAspm
  )
{
  PCI_REG_PCIE_LINK_CAPABILITY    LinkCap;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  LinkCap.Uint32 = RpDev->PciCfgAccess->Read32 (
                     RpDev->PciCfgAccess,
                     R_PCIE_CFG_LCAP
                     );
  *PchDmiAspm = (UINT8) LinkCap.Bits.Aspm;
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function enables ASPM states in PCH DMI Link Control register.

  @param[in]  PchDmiAspm   Bitmap with ASPM states to enable
  @param[in]  RpDev        Pointer to the root port device
**/
VOID
DmiSipSetAspm (
  IN  UINT8                       PchDmiAspm,
  IN  PCIE_ROOT_PORT_DEV          *RpDev
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  //
  // We are storing information about PCH DMI ASPM setting in HOB
  // for use in PostMem phase.
  Status = DmiSipSetAspmInPchDmiPreMemInfoHob (PchDmiAspm);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Set DMI thermal throttling to recommended configuration.
  It's intended for PCH-H DMI SIP15 and above.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipSetRecommendedThermalThrottling (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32  Data32And;
  UINT32  Data32Or;
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  Data32And = (UINT32)~(B_DMI_PCR_UPHWAWC_TS3TW | B_DMI_PCR_UPHWAWC_TS2TW |
                        B_DMI_PCR_UPHWAWC_TS1TW | B_DMI_PCR_UPHWAWC_TS0TW);
  if (RpDev->SipVersion >= PcieSip17) {
    Data32Or  = (0 << N_DMI_PCR_UPHWAWC_TS3TW) |
              (0 << N_DMI_PCR_UPHWAWC_TS2TW) |
              (0 << N_DMI_PCR_UPHWAWC_TS1TW) |
              (3 << N_DMI_PCR_UPHWAWC_TS0TW) |
              B_DMI_PCR_UPHWAWC_TSAWEN;
  } else {
    Data32Or  = (0 << N_DMI_PCR_UPHWAWC_TS3TW) |
              (1 << N_DMI_PCR_UPHWAWC_TS2TW) |
              (2 << N_DMI_PCR_UPHWAWC_TS1TW) |
              (3 << N_DMI_PCR_UPHWAWC_TS0TW) |
              B_DMI_PCR_UPHWAWC_TSAWEN;
  }

  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_DMI_PCR_UPHWAWC, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Set DMI thermal throttling to custom configuration.
  This function will configure Thermal Sensor 0/1/2/3 TargetWidth and set
  DMI Thermal Sensor Autonomous Width Enable.
  It's intended only for PCH-H DMI SIP15.

  @param[in]  PchDmiThermalThrottling       DMI Thermal Throttling structure
  @param[in]  RpDev                         Pointer to the root port device
**/
VOID
DmiSipSetCustomThermalThrottling (
  IN  PCH_DMI_THERMAL_THROTTLING        PchDmiThermalThrottling,
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32  Data32And;
  UINT32  Data32Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  ///
  /// DMI Throttling action
  ///
  Data32And = (UINT32)~(B_DMI_PCR_UPHWAWC_TS3TW | B_DMI_PCR_UPHWAWC_TS2TW |
                        B_DMI_PCR_UPHWAWC_TS1TW | B_DMI_PCR_UPHWAWC_TS0TW);
  Data32Or  = (PchDmiThermalThrottling.ThermalSensor3TargetWidth << N_DMI_PCR_UPHWAWC_TS3TW) |
              (PchDmiThermalThrottling.ThermalSensor2TargetWidth << N_DMI_PCR_UPHWAWC_TS2TW) |
              (PchDmiThermalThrottling.ThermalSensor1TargetWidth << N_DMI_PCR_UPHWAWC_TS1TW) |
              (PchDmiThermalThrottling.ThermalSensor0TargetWidth << N_DMI_PCR_UPHWAWC_TS0TW) |
              B_DMI_PCR_UPHWAWC_TSAWEN;

  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_DMI_PCR_UPHWAWC, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}


/**
  This function performs PCH-H DMI Thermal Throttling init.

  @param[in]  ThermalConfig        Pointer to the PCH Thermal Config block
  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureThermalThrottling (
  IN  THERMAL_CONFIG              *ThermalConfig,
  IN  PCIE_ROOT_PORT_DEV          *RpDev
  )
{
  PCH_DMI_THERMAL_THROTTLING    DmiThermalThrottling;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  if (IsPchH () || (RpDev->SipVersion >= PcieSip17)) {
    if (ThermalConfig->DmiHaAWC.SuggestedSetting) {
      DmiSipSetRecommendedThermalThrottling (RpDev);
    } else if (ThermalConfig->DmiHaAWC.DmiTsawEn) {
      DmiThermalThrottling.ThermalSensor0TargetWidth = ThermalConfig->DmiHaAWC.TS0TW;
      DmiThermalThrottling.ThermalSensor1TargetWidth = ThermalConfig->DmiHaAWC.TS1TW;
      DmiThermalThrottling.ThermalSensor2TargetWidth = ThermalConfig->DmiHaAWC.TS2TW;
      DmiThermalThrottling.ThermalSensor3TargetWidth = ThermalConfig->DmiHaAWC.TS3TW;
      DmiSipSetCustomThermalThrottling (DmiThermalThrottling, RpDev);
    }
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Configure PCH DMI 10 Bit Tag and Scaled Flow Control.
  Applicable from SIP 17.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigure10BitTagAndScaledFlowControl (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  // Configure 10 bit tag for UPDMI
  PcieSipConfigure10BitTag (RpDev, FALSE, FALSE);

  // Configure Scaled Flow Control
  RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_DLFCAP, ~0u, (UINT32) B_PCIE_CFG_DLFCAP_LSFCS);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs programing of registers for Elastic Buffer.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
EFIAPI
DmiSipConfigureElasticBuffer (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32  Data32Or;
  UINT32  Data32And;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  Data32And = (UINT32)~B_PCIE_CFG_ACGR3S2_LSTPTLS;
  Data32Or = (UINT32)(BIT19 | B_PCIE_CFG_ACGR3S2_G4EBM | B_PCIE_CFG_ACGR3S2_G3EBM |
                      B_PCIE_CFG_ACGR3S2_G2EBM | B_PCIE_CFG_ACGR3S2_G1EBM | B_PCIE_CFG_ACGR3S2_SRT |
                      (0x1 << N_PCIE_CFG_ACGR3S2_LSTPTLS));

  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_ACGR3S2,
    Data32And,
    Data32Or
    );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures DMI Server Error Reporting Mode
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureSerm (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32     Data32And;
  UINT32     Data32Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  //
  // Configure SERM and PCIe Interrupt Pin
  //
  Data32And = (RpDev->PciCfgAccess->Read32 (
                 RpDev->PciCfgAccess,
                 R_PCIE_CFG_STRPFUSECFG) & (UINT32) ~B_PCIE_CFG_STRPFUSECFG_SERM) | B_PCIE_CFG_STRPFUSECFG_PXIP;
  Data32Or = 0x0 << N_PCIE_CFG_STRPFUSECFG_PXIP;
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_STRPFUSECFG,
    Data32And,
    Data32Or
    );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function sets PCH DMI Local and Remote Transmitter
  Link Equalization Preset/Coefficient Evaluation Bypass,
  and performs Precursor and Postcursor Preset/Coefficient
  mapping initialization.

  @param[in]  TestEnable           Is Test Environment in use
  @param[in]  RpDev                Pointer to the root port device
**/
VOID
SipDmiConfigurePresetCoefficientInitialization (
  IN  BOOLEAN                           TestEnable,
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32     Data32And;
  UINT32     Data32Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  Data32And = (UINT32)~(0);
  Data32Or = (UINT32)(0);
  if (RpDev->SipVersion >= PcieSip17) {
    Data32And = (UINT32)~(B_PCIE_CFG_RTPCL1_PCM |
                B_PCIE_CFG_RTPCL1_RTPOSTCL1PL3 |
                B_PCIE_CFG_RTPCL1_RTPRECL1PL2 |
                B_PCIE_CFG_RTPCL1_RTPOSTCL0PL1 |
                B_PCIE_CFG_RTPCL1_RTPRECL0PL0
                );

    Data32Or = (UINT32) ((9 << N_PCIE_CFG_RTPCL1_RTPOSTCL1PL3) |
                (8 << N_PCIE_CFG_RTPCL1_RTPRECL1PL2) |
                (5 << N_PCIE_CFG_RTPCL1_RTPOSTCL0PL1) |
                (4 << N_PCIE_CFG_RTPCL1_RTPRECL0PL0)
                );

    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_RTPCL1,
      Data32And,
      Data32Or
      );

    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_HAEQ,
      (UINT32)~(B_PCIE_CFG_HAEQ_HAPCCPI | B_PCIE_CFG_HAEQ_MACFOMC | B_PCIE_CFG_HAEQ_DL),
      (UINT32)(0xE << N_PCIE_CFG_HAEQ_DL)
      );

    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_EQCFG2,
      (UINT32)~(B_PCIE_CFG_EQCFG2_PCET | B_PCIE_CFG_EQCFG2_HAPCSB | B_PCIE_CFG_EQCFG2_REWMET),
      (UINT32)((0x2 << N_PCIE_CFG_EQCFG2_PCET) |
       (0x3 << N_PCIE_CFG_EQCFG2_HAPCSB) |
       (0x10 << N_PCIE_CFG_EQCFG2_REWMET))
      );

    Data32Or = (UINT32)((0x3 << N_PCIE_CFG_EQCFG4_PX16GRWTNEVE) |
                        (0x2 << N_PCIE_CFG_EQCFG4_PX16GHAPCCPI));
    if (RpDev->Integration == UpDmi3) {
      Data32Or |= B_PCIE_CFG_EQCFG4_PX16GEQTS2IRRC;
    }
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_EQCFG4,
      (UINT32)~(B_PCIE_CFG_EQCFG4_PX16GRTLEPCEB | B_PCIE_CFG_EQCFG4_PX16GRTPCOE |
                B_PCIE_CFG_EQCFG4_PX16GRWTNEVE | B_PCIE_CFG_EQCFG4_PX16GHAPCCPI),
      Data32Or
      );

    Data32And = (UINT32)~(B_PCIE_CFG_PX16GRTPCL1_RTPRECL1PL2 |
                B_PCIE_CFG_PX16GRTPCL1_RTPOSTCL0PL1 |
                B_PCIE_CFG_PX16GRTPCL1_RTPRECL0PL0
                );

    Data32Or = (UINT32) ((6 << N_PCIE_CFG_PX16GRTPCL1_RTPRECL1PL2) |
                         (5 << N_PCIE_CFG_PX16GRTPCL1_RTPOSTCL0PL1) |
                         (4 << N_PCIE_CFG_PX16GRTPCL1_RTPRECL0PL0)
                         );

    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_PX16GRTPCL1,
      Data32And,
      Data32Or
      );

    Data32And = (UINT32)~(B_PCIE_CFG_PX16GRTPCL4_RTPOSTCL7);

    Data32Or = (UINT32) (3 << N_PCIE_CFG_PX16GRTPCL4_RTPOSTCL7);

    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_PX16GRTPCL4,
      Data32And,
      Data32Or
      );

    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_EQCFG5,
      (UINT32)~(B_PCIE_CFG_EQCFG5_PCET | B_PCIE_CFG_EQCFG5_HAPCSB),
      (UINT32)((0x2 << N_PCIE_CFG_EQCFG5_PCET) | (0x2 << N_PCIE_CFG_EQCFG5_HAPCSB))
      );
    //
    // Set PCR[DMI] + Offset 2454h
    //
    Data32And = (UINT32) ~(B_PCIE_CFG_EQCFG1_TUPP | B_PCIE_CFG_EQCFG1_LERSMIE |
                           B_PCIE_CFG_EQCFG1_RWTNEVE | B_PCIE_CFG_EQCFG1_HAPCCPIE |
                           B_PCIE_CFG_EQCFG1_RTLEPCEB | B_PCIE_CFG_EQCFG1_RTPCOE);
    Data32Or = (UINT32) ((0x1 << N_PCIE_CFG_EQCFG1_RWTNEVE));
  }

  if (RpDev->Integration == UpDmi3) {
    Data32Or |= B_PCIE_CFG_EQCFG1_EQTS2IRRC;
  }

  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_EQCFG1,
    Data32And,
    Data32Or
    );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of miscellaneous SIP 17 DMI registers

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureMiscConfiguration (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32    Data32And;
  UINT32    Data32Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  Data32And = (UINT32) ~(B_PCIE_CFG_PCIECFG2_RRCP | B_PCIE_CFG_PCIECFG2_PMET);
  Data32Or = (UINT32) (B_PCIE_CFG_PCIECFG2_CROAOV | B_PCIE_CFG_PCIECFG2_CROAOE);
  if (RpDev->Integration != UpDmi3) {
    Data32Or |= B_PCIE_CFG_PCIECFG2_CRSREN;
  }
  RpDev->PciPcrAccess->AndThenOr32 (
                         RpDev->PciPcrAccess,
                         R_PCIE_CFG_PCIECFG2,
                         Data32And,
                         Data32Or
                         );

  //
  // Configure Rx Delay
  //
  Data32And = (UINT32) ~(B_PCIE_CFG_CCFG_UPMWPD | B_PCIE_CFG_CCFG_UNRD);
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_CCFG,
                         Data32And,
                         0u
                         );

  //
  // Configure Completion Ordering Mode
  //
  Data32And = (UINT32) ~(B_PCIE_CFG_DC_DCT1C |B_PCIE_CFG_DC_DCT0C | B_PCIE_CFG_DC_COM);
  RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_DC, Data32And, 0u);

  Data32And = (UINT32) ~(B_PCIE_CFG_PCIEALC_ITLRCLD | B_PCIE_CFG_PCIEALC_ILLRCLD |
                         B_PCIE_CFG_PCIEALC_SSRLD | B_PCIE_CFG_PCIEALC_SSRRS |
                         BIT20);
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_PCIEALC,
                         Data32And,
                         0u
                         );

  //
  // Configure Peer Disable
  //
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_PCIEDBG,
                         (UINT32)~B_PCIE_CFG_PCIEDBG_LDSWQRP,
                         (UINT32)B_PCIE_CFG_PCIEDBG_CTONFAE
                         );
  if (RpDev->Integration == UpDmi3) {
    Dmi3SipConfigureCompletionTimeout (RpDev);
    Dmi3SipConfigurePreallocationAndPeerDisable (RpDev);
    Data32And = (UINT32) ~(BIT21 | B_PCIE_CFG_ADVMCTRL_EIOSMASKRX |
                           B_PCIE_CFG_ADVMCTRL_RRLLCL | B_PCIE_CFG_ADVMCTRL_RLLG12R |
                           B_PCIE_CFG_ADVMCTRL_PMREQCWC | B_PCIE_CFG_ADVMCTRL_PMREQBLKPGRSPT);
    Data32Or = (UINT32)(B_PCIE_CFG_ADVMCTRL_INRXL0CTRL | B_PCIE_CFG_ADVMCTRL_EIOSDISDS |
                        B_PCIE_CFG_ADVMCTRL_RXL0DC | B_PCIE_CFG_ADVMCTRL_G3STFER |
                        (0x6 << N_PCIE_CFG_ADVMCTRL_PMREQCWC) |
                        (0x2 << N_PCIE_CFG_ADVMCTRL_PMREQBLKPGRSPT));
  } else {
    Data32And = (UINT32) ~(BIT21 | B_PCIE_CFG_ADVMCTRL_EIOSMASKRX |
                           B_PCIE_CFG_ADVMCTRL_RRLLCL | B_PCIE_CFG_ADVMCTRL_RLLG12R | B_PCIE_CFG_ADVMCTRL_PMREQCWC);
    Data32Or = (UINT32)(B_PCIE_CFG_ADVMCTRL_INRXL0CTRL | B_PCIE_CFG_ADVMCTRL_EIOSDISDS |
                        B_PCIE_CFG_ADVMCTRL_RXL0DC | B_PCIE_CFG_ADVMCTRL_G3STFER | (0x6 << N_PCIE_CFG_ADVMCTRL_PMREQCWC));
  }

  RpDev->PciPcrAccess->AndThenOr32 (
                         RpDev->PciPcrAccess,
                         R_PCIE_CFG_ADVMCTRL,
                         Data32And,
                         Data32Or
                         );

  //
  // Configure Error Injection
  //
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_EINJCTL,
                         ~0u,
                         B_PCIE_CFG_EINJCTL_EINJDIS
                         );

  Data32And = (UINT32)~(B_PCIE_CFG_MPC_CCEL | B_PCIE_CFG_MPC_IRRCE);
  Data32Or = (UINT32) ((0x4 << N_PCIE_CFG_MPC_CCEL) | B_PCIE_CFG_MPC_FCDL1E | B_PCIE_CFG_MPC_BT);
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_MPC,
                         Data32And,
                         Data32Or
                         );

  //
  // Configure Capabilities
  //
  Data32And = (UINT32)~(B_PCIE_CFG_AECH_NCO | B_PCIE_CFG_AECH_CV | B_PCIE_CFG_AECH_CID);
  Data32Or = (UINT32) ((0x280 << N_PCIE_CFG_AECH_NCO) |
                       (0x1 << N_PCIE_CFG_AECH_CV) |
                       (0x1 << N_PCIE_CFG_AECH_CID));
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_AECH,
                         Data32And,
                         Data32Or
                         );

  Data32And = (UINT32)~(B_PCIE_CFG_SPEECH_NCO | B_PCIE_CFG_SPEECH_CV | B_PCIE_CFG_SPEECH_PCIEECID);
  Data32Or = (UINT32) ((0xA90 << N_PCIE_CFG_SPEECH_NCO) |
                       (0x1 << N_PCIE_CFG_SPEECH_CV) |
                       (0x19 << N_PCIE_CFG_SPEECH_PCIEECID));
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_SPEECH,
                         Data32And,
                         Data32Or
                         );

  Data32And = (UINT32)~(B_PCIE_CFG_DLFECH_NCO | B_PCIE_CFG_DLFECH_CV | B_PCIE_CFG_DLFECH_PCIECID);
  Data32Or = (UINT32) ((0xA9C << N_PCIE_CFG_DLFECH_NCO) |
                       (0x1 << N_PCIE_CFG_DLFECH_CV) |
                       (0x25 << N_PCIE_CFG_DLFECH_PCIECID));
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_DLFECH,
                         Data32And,
                         Data32Or
                         );

  Data32And = (UINT32)~(B_PCIE_CFG_PL16GECH_NCO | B_PCIE_CFG_PL16GECH_CV | B_PCIE_CFG_PL16GECH_PCIECID);
  Data32Or = (UINT32) ((0xEDC << N_PCIE_CFG_PL16GECH_NCO) |
                       (0x1 << N_PCIE_CFG_PL16GECH_CV) |
                       (0x26 << N_PCIE_CFG_PL16GECH_PCIECID));
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_PL16GECH,
                         Data32And,
                         Data32Or
                         );

  Data32And = (UINT32)~(B_PCIE_CFG_PL16MECH_NCO | B_PCIE_CFG_PL16MECH_CV | B_PCIE_CFG_PL16MECH_PCIECID);
  Data32Or = (UINT32) ((0xEDC << N_PCIE_CFG_PL16MECH_NCO) |
                       (0x1 << N_PCIE_CFG_PL16MECH_CV) |
                       (0x26 << N_PCIE_CFG_PL16MECH_PCIECID));
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_PL16MECH,
                         Data32And,
                         Data32Or
                         );

  if (RpDev->Integration == UpDmi3) {
    Data32And = (UINT32) ~B_PCIE_MEM_PIDECCTL_CPLBNCHK;
    Data32Or = (UINT32) 0u;
  } else {
    Data32And = (UINT32) ~0u;
    Data32Or = (UINT32) B_PCIE_MEM_PIDECCTL_CPLBNCHK;
  }
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_MEM_PIDECCTL,
                         Data32And,
                         Data32Or
                         );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of DMI Rx Margining.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureRxMargining (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  Data32And = (UINT32)~(B_PCIE_CFG_RXMC1_MSRVS | B_PCIE_CFG_RXMC1_MSRTS |
                        B_PCIE_CFG_RXMC1_TMSLOP | B_PCIE_CFG_RXMC1_VMSLOP |
                        B_PCIE_CFG_RXMC1_MMNOLS);
  Data32Or = (UINT32) ((0x1F << N_PCIE_CFG_RXMC1_MSRVS) |
                       (0x3F << N_PCIE_CFG_RXMC1_MSRTS) |
                       (0x1 << N_PCIE_CFG_RXMC1_TMSLOP) |
                       (0x1 << N_PCIE_CFG_RXMC1_VMSLOP) |
                       (0x1 << N_PCIE_CFG_RXMC1_MMNOLS));
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_RXMC1,
                         Data32And,
                         Data32Or
                         );

  Data32And = (UINT32)~(B_PCIE_CFG_RXMC2_MNOTSS | B_PCIE_CFG_RXMC2_MMTOS |
                        B_PCIE_CFG_RXMC2_MNOVSS);
  if (RpDev->Integration == UpDmi3) {
    Data32Or = (UINT32) ((0x20 << N_PCIE_CFG_RXMC2_MNOTSS) |
                         (0x28 << N_PCIE_CFG_RXMC2_MMTOS) |
                         (0x28 << N_PCIE_CFG_RXMC2_MNOVSS));
  } else {
    Data32Or = (UINT32) ((0x9 << N_PCIE_CFG_RXMC2_MNOTSS) |
                         (0x14 << N_PCIE_CFG_RXMC2_MMTOS) |
                         (0x20 << N_PCIE_CFG_RXMC2_MNOVSS));
  }
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_RXMC2,
                         Data32And,
                         Data32Or
                         );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of DMI Phy.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigurePhy (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_CTRL2,
                         (UINT32) ~BIT19,
                         BIT18
                         );

  Data32And = (UINT32)~(0u);
  Data32Or = (UINT32) (B_PCIE_MEM_LPHYCP4_DPMDNDBFE | B_PCIE_MEM_LPHYCP4_PLLBUSDRC);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_LPHYCP4, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of DMI Phy Recalibration.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigurePhyRecal (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  Data32And = (UINT32)~(B_PCIE_MEM_IORCCP1_G5ORRRXECC | B_PCIE_MEM_IORCCP1_G4ORRRXECC |
                        B_PCIE_MEM_IORCCP1_G3ORRRXECC | B_PCIE_MEM_IORCCP1_G2ORRRXECC |
                        B_PCIE_MEM_IORCCP1_G1ORRRXECC);
  Data32Or = (UINT32) ((0x1 << N_PCIE_MEM_IORCCP1_G5ORRRXECC) |
                       (0x1 << N_PCIE_MEM_IORCCP1_G4ORRRXECC) |
                       (0x1 << N_PCIE_MEM_IORCCP1_G3ORRRXECC) |
                       (0x1 << N_PCIE_MEM_IORCCP1_G2ORRRXECC) |
                       (0x1 << N_PCIE_MEM_IORCCP1_G1ORRRXECC) |
                        B_PCIE_MEM_IORCCP1_DISORCRODI | B_PCIE_MEM_IORCCP1_DRCORRP | B_PCIE_MEM_IORCCP1_DISORCL12REC);
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_IORCCP1, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of DMI L1 Support.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureL1Support (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32   Data32Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  Data32Or = (UINT32) (B_PCIE_MEM_UPDLPHYCP_TXCMMDISLNCTL | B_PCIE_MEM_UPDLPHYCP_UPDLTCDC);
  if (RpDev->Integration == UpDmi3) {
    Data32Or |= B_PCIE_MEM_UPDLPHYCP_TXCMMDISLNCTL;
  }
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_UPDLPHYCP, ~0u, Data32Or);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of DMI D3 Vnn removal.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureD3VnnRemCtl (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32   Data32And;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  Data32And = (UINT32)~(B_PCIE_CFG_VNNREMCTL_ISPLFVNNRE | B_PCIE_CFG_VNNREMCTL_LRSLFVNNRE);
  RpDev->PciCfgAccess->And32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_VNNREMCTL,
                         Data32And
                         );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of Host Cycle Decoding.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureRxHostCycleDecoding (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  PcieSipRxPrimaryCycleDecode (RpDev);
  Data32And = (UINT32) ~0u;
  Data32Or = (UINT32) 0u;
  if (RpDev->Integration == UpDmi3) {
    Data32Or = (UINT32) (B_PCIE_MEM_DECCTL_LCRXINT | B_PCIE_MEM_DECCTL_MTRXLTC);
    //
    // Configure legacy Msg
    //
    Data32And = (UINT32) ~B_PCIE_MEM_DECCTL_LCRXINT;
  }
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_DECCTL, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function  Sets BIOS Lock-Down (BILD)

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipSetBiosLockDown (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32 Data32And;
  UINT32 Data32Or;
  Data32And = 0xFFFFFFFF;
  Data32Or = B_PCH_DMI_PCR_BILD;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_DMI_PCR_GCS, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of DMI Max Payload Size.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureMPS (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  Data32And = B_PCIE_CFG_DCAP_MPS;
  Data32Or = 0x1 << N_PCIE_CFG_DCAP_MPS;
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_DCAP,
                         Data32And,
                         Data32Or
                         );

  Data32And = B_PCIE_CFG_DCTL_MPS;
  Data32Or = (UINT32)((0x1 << N_PCIE_CFG_DCTL_MPS) |
                      (B_PCIE_CFG_DCTL_URE | B_PCIE_CFG_DCTL_FEE | B_PCIE_CFG_DCTL_NFE | B_PCIE_CFG_DCTL_CEE));
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_DCTL,
                         Data32And,
                         Data32Or
                         );

  Data32And = (UINT32) ~B_PCIE_CFG_IPCS_IMPS;
  Data32Or = (0x1 << N_PCIE_CFG_IPCS_IMPS);
  if (RpDev->Integration == UpDmi3) {
    // Set IPCS to 256B
    Data32And &= (UINT32) ~B_PCIE_CFG_IPCS_IMRS;
    Data32Or |= (UINT32) (0x1 << N_PCIE_CFG_IPCS_IMRS);
    // Set UPRS to 64B
    RpDev->PciPcrAccess->AndThenOr32 (
      RpDev->PciPcrAccess,
      R_PCIE_CFG_CCFG,
      (UINT32) ~B_PCIE_CFG_CCFG_UPRS,
      (UINT32) (B_PCIE_CFG_CCFG_UPSD | B_PCIE_CFG_CCFG_UNSD | (0x7 << N_PCIE_CFG_CCFG_UPRS))
      );
  }
  RpDev->PciPcrAccess->AndThenOr32 (
                         RpDev->PciPcrAccess,
                         R_PCIE_CFG_IPCS,
                         Data32And,
                         Data32Or
                         );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of DMI Coalescing.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureCoalescing (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_ACRG3,
    (UINT32) ~(B_PCIE_CFG_ACRG3_RRXDME | B_PCIE_CFG_ACRG3_CBGM | B_PCIE_CFG_ACRG3_ADESKEW_DIS),
    0u
    );
  if (RpDev->Integration == UpDmi3) {
    RpDev->PciPcrAccess->And32 (
                                RpDev->PciPcrAccess,
                                R_PCIE_CFG_COCTL,
                                (UINT32) ~(B_PCIE_CFG_COCTL_DDCE | B_PCIE_CFG_COCTL_PWCE)
                                );
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of DMI SERR# Enable.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureSERREnable (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  RpDev->PciCfgAccess->AndThenOr16 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_CMD,
    (UINT16)~0,
    (UINT16)(R_PCIE_CFG_CMD)
    );

  RpDev->PciCfgAccess->AndThenOr8 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_BCTRL,
    (UINT8)~0,
    (UINT8)(B_PCIE_CFG_BCTRL_SE)
    );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs configuration of DMI VC Rx flow control.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureVcRxFlowControl (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_RXQC,
    (UINT32)~(B_PCIE_CFG_RXQC_VMRQNPCOC | B_PCIE_CFG_RXQC_VMRQPCOC | B_PCIE_CFG_RXQC_V1RQNPCOC |
              B_PCIE_CFG_RXQC_V1RQPCOC | B_PCIE_CFG_RXQC_V0RQNPCOC | B_PCIE_CFG_RXQC_V0RQPCOC),
    (UINT32)((0x1 << N_PCIE_CFG_RXQC_VMRQNPCOC) |
             (0x1 << N_PCIE_CFG_RXQC_VMRQPCOC) |
             (0x1 << N_PCIE_CFG_RXQC_V1RQNPCOC) |
             (0x1 << N_PCIE_CFG_RXQC_V1RQPCOC) |
             (0x1 << N_PCIE_CFG_RXQC_V0RQNPCOC) |
             (0x1 << N_PCIE_CFG_RXQC_V0RQPCOC))
    );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs enabling of Thermal Throttling.
  From SIP 17 it should be performed afrer link training to max speed.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipEnableThermalThrottling (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_DMI_PCR_UPHWAWC, ~0u, B_DMI_PCR_UPHWAWC_TSAWEN);
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function performs early PCH-H DMI SIP15 init.
  It must be called before any upstream bus host transactions are enabled

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
PchDmiEarlyInit (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  //
  // Enable Transaction Layer Packet Fast Transmit Mode and
  // DMI Credit Allocated Update Mode
  //
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_MPC2,
    ~0u,
    B_PCIE_CFG_MPC2_TLPF | B_PCIE_CFG_MPC2_CAM
    );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function enables generating SERR interrupt by DMI whenever Stop and Scream is
  triggered due to Parity Error detected on the TLP Header.

  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipConfigureParityCheck (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  RpDev->PciPcrAccess->AndThenOr32 (
    RpDev->PciPcrAccess,
    R_DMI_CFG_PCR_EEPPES,
    ~0u,
    B_DMI_CFG_PCR_EEPPES_PSSSGE
  );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function prepares supported link speed vector for DMI.
  Applicable from SIP 17

  @param[in]  RpDev                Pointer to the root port device
  @return     A bitmap of supported link speeds (gens) is returned.
**/
UINT32
DmiSipGetSupportedLinkSpeedsVector (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32      Data32;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  Data32 = RpDev->PciCfgAccess->Read32 (
                                  RpDev->PciCfgAccess,
                                  R_PCIE_CFG_LCAP2);

  DEBUG ((DEBUG_INFO, "%a() End with vector at %d\n", __FUNCTION__, Data32));
  return Data32;
}

/**
  This function configures DMI SIP

  @param[in]  PchPcieConfig        Pointer to PCH PCIe config block
  @param[in]  ThermalConfig        Pointer to Thermal Throttling config block
  @param[in]  PchGeneralConfig     Pointer to PCH General config block
  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipInit (
  IN  PCH_PCIE_CONFIG             *PchPcieConfig,
  IN  THERMAL_CONFIG              *ThermalConfig,
  IN  PCH_GENERAL_CONFIG          *PchGeneralConfig,
  IN  PCIE_ROOT_PORT_DEV          *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  DmiSipBasicInit (RpDev);
  DmiSipConfigureClockGating (RpDev);
  DmiSipConfigureLinkPowerManagement (PchPcieConfig, RpDev);
  if (RpDev->SipVersion < PcieSip17) {
    DmiSipConfigureThermalThrottling (ThermalConfig, RpDev);
  }
  DmiSipConfigureSquelchPowerManagement (RpDev);
  DmiSipConfigurePllShutdown (RpDev);
  DmiSipConfigurePowerGating (PchPcieConfig, RpDev);
  if (RpDev->PrivateConfig.ConfigureAspmAtDpDmiInit) {
    DmiSipConfigureAspm (PchPcieConfig, PchGeneralConfig, RpDev);
  }
  if (RpDev->SipVersion >= PcieSip17) {
    DmiSipConfigureVcRxFlowControl (RpDev);
    DmiSipConfigureSerm (RpDev);
    DmiSipConfigureSERREnable (RpDev);
    DmiSipConfigureElasticBuffer (RpDev);
    DmiSipConfigureL1Support (RpDev);
    DmiSipConfigureD3VnnRemCtl (RpDev);
    DmiSipConfigureMPS (RpDev);
    DmiSipConfigureCoalescing (RpDev);
    DmiSipConfigureRxMargining (RpDev);
    PcieSipConfigureResetPrep (RpDev);
    PcieSipConfigurePowerDownMapping (RpDev);
    PcieSipConfigurePtm (RpDev, &RpDev->PrivateConfig.PtmConfig);
    DmiSipConfigurePhy (RpDev);
    DmiSipConfigurePhyRecal (RpDev);
    DmiSipConfigureMiscConfiguration (RpDev);
    DmiSipConfigure10BitTagAndScaledFlowControl (RpDev);
    DmiSipConfigureRxHostCycleDecoding (RpDev);
    if (RpDev->Integration == UpDmi3) {
      Dmi3SipConfigureCapability (RpDev);
      Dmi3SipConfigureDynamicLinkThrottling (RpDev);
      Dmi3SipConfigureLtrSupport (RpDev);
      Dmi3SipConfigureDpcExtensionsSupport (RpDev);
      Dmi3SipConfigureLtrOverride (RpDev);
      Dmi3SipConfigureFcpTimeout (RpDev);
      Dmi3SipImplementationSpecificProgramming (RpDev);
      Dmi3SipConfigureTpowerOn (RpDev);
      Dmi3SipConfigureTpowerOff (RpDev);
      Dmi3SipConfigurePtm (RpDev);
      Dmi3SipConfigurePipeStageDelay (RpDev);
      Dmi3SipConfigureMctp (RpDev);
      DmiSipEnablePcieRelaxedOrder (RpDev);
      DmiSipSetBiosLockDown (RpDev);
    }
    DmiSipSetBiosLockDown (RpDev);
  }
  if (RpDev->PrivateConfig.ParityCheckEnable) {
    DmiSipConfigureParityCheck (RpDev);
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function configures late DMI SIP
  @param[in]  RpDev                Pointer to the root port device
**/
VOID
DmiSipLateInit (
  IN  PCIE_ROOT_PORT_DEV          *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  RpDev->PciCfgAccess->AndThenOr32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_MEM_PIDECCTL,
                         (UINT32) ~B_PCIE_MEM_PIDECCTL_CPLBNCHK,
                         (UINT32) 0u
                         );

  RpDev->PciSbiMsgMemAccess->AndThenOr32 (
                                RpDev->PciSbiMsgMemAccess,
                                R_PCIE_MEM_IPSCE,
                                (UINT32) ~(B_PCIE_MEM_IPSCE_RXROM | B_PCIE_MEM_IPSCE_TXROM),
                                (UINT32) (V_PCIE_MEM_IPSCE_RXROM << N_PCIE_MEM_IPSCE_RXROM)
                                );

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function will perform necessary programming before changing PCH DMI link speed to Gen3 or higher.

  @param[in]  DmiSpeedGen          PCH DMI speed Gen (1-Gen1, 2-Gen2, 3-Gen3, 4-Gen4) to which link is going to be trained
  @param[in]  RpDev                Pointer to the root port device
**/
STATIC
VOID
DmiSipPrepareEq (
  IN  UINT8                             DmiSpeedGen,
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32                          Data32Or;
  UINT32                          PresetIndex;
  UINT32                          LaneIndex;
  UINT32                          PcmLaneIndex;
  BOOLEAN                         TestEnable;
  PCIE_LINK_EQ_PLATFORM_SETTINGS  *DmiGen3PlatformSettings;
  PCIE_LINK_EQ_PLATFORM_SETTINGS  *DmiGen4PlatformSettings;
  PCIE_LINK_EQ_PROJECT_SETTINGS   *DmiGen3ProjectSettings;
  PCIE_LINK_EQ_PROJECT_SETTINGS   *DmiGen4ProjectSettings;
  PCIE_LINK_EQ_COEFFICIENTS       *Gen3LaneCoefficients;
  PCIE_LINK_EQ_COEFFICIENTS       *Gen4LaneCoefficients;

  EFI_STATUS                      Status;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicy;
  PCH_PCIE_RP_PREMEM_CONFIG       *PchPcieRpPreMemConfig;
  UINT32                          Data32And;
  ///
  /// Get DMI settings through the PchPolicy PPI
  ///
  Status = PeiServicesLocatePpi (
             &gPchPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (EFI_ERROR (Status)) {
    Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  }
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PchPcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  //
  // Starting from SIP 15 onwards, there is space for
  // only one lane PCM configuration.
  // This configuration gets propagated to all lanes.
  //
  PcmLaneIndex = 1;
  Data32Or = 0u;
  DmiGen3PlatformSettings = &RpDev->PcieRootPortConfig->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings;
  DmiGen4PlatformSettings = &RpDev->PcieRootPortConfig->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings;
  DmiGen3ProjectSettings = &RpDev->PrivateConfig.Gen3EqSettings.ProjectSettings;
  DmiGen4ProjectSettings = &RpDev->PrivateConfig.Gen4EqSettings.ProjectSettings;
  Gen3LaneCoefficients = RpDev->PcieRootPortConfig->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList;
  Gen4LaneCoefficients = RpDev->PcieRootPortConfig->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList;
  if (RpDev->SipVersion >= PcieSip17) {
    //
    // Configure Phy.
    // This programming has to be performed before equalization.
    //
    Data32Or = (UINT32) (B_PCIE_MEM_LPHYCP1_RXEQFNEVC | B_PCIE_MEM_LPHYCP1_RXADPHM);
    RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_LPHYCP1, ~0u, Data32Or);
  }

  TestEnable = FALSE;

  if (DmiSpeedGen > 2) {
    //
    // Configure DMI Local Transmitter Coefficient Override
    //
    if (DmiGen3PlatformSettings->LocalTxOverrideEn) {
      DEBUG ((DEBUG_INFO, "LocalTxOverrideEn = %x \n", DmiGen3PlatformSettings->LocalTxOverrideEn));
      for (LaneIndex = 0; LaneIndex < PcmLaneIndex; LaneIndex++) {
        GetSipGen3PresetToCoefficientMapping (
          RpDev,
          LaneIndex,
          DmiGen3PlatformSettings->Ph2LocalTxOverridePreset,
          Gen3LaneCoefficients
          );
        GetSipGen4PresetToCoefficientMapping (
          RpDev,
          LaneIndex,
          DmiGen4PlatformSettings->Ph2LocalTxOverridePreset,
          Gen4LaneCoefficients
          );
      }
    }

    for (LaneIndex = 0; LaneIndex < PCH_MAX_DMI_LANES; LaneIndex++) {
      PcieSipSetGen3TransmitterLanePreset (
        RpDev,
        LaneIndex,
        DmiGen3PlatformSettings->Ph1DpTxPreset,
        DmiGen3PlatformSettings->Ph1UpTxPreset
        );
      PcieSipSetGen4TransmitterLanePreset (
        RpDev,
        LaneIndex,
        DmiGen4PlatformSettings->Ph1DpTxPreset,
        DmiGen4PlatformSettings->Ph1UpTxPreset
        );
      PcieSipSetGen3LocalTransmitterOverrideCoefficients (
        RpDev,
        LaneIndex,
        Gen3LaneCoefficients
        );
      PcieSipSetGen4LocalTransmitterOverrideCoefficients (
        RpDev,
        LaneIndex,
        Gen4LaneCoefficients
        );
    }

    if (DmiGen3PlatformSettings->HapcsSearchEnable != FORCE_DISABLE) {
      TestEnable = TRUE;
    }

    SipDmiConfigurePresetCoefficientInitialization (TestEnable, RpDev);

    //
    // Before training to GEN3 Configure Lane0 P0-P10 Preset Coefficient mapping
    // based on recommendation for PCH DMI
    //
    for (LaneIndex = 0; LaneIndex < PcmLaneIndex; LaneIndex++) {
      for (PresetIndex = 0; PresetIndex < ARRAY_SIZE (DmiGen3ProjectSettings->PresetToCoefficient); PresetIndex++) {
        if (RpDev->SipVersion >= PcieSip17) {
          SetSipGen3PresetToCoefficientMapping (
            RpDev,
            LaneIndex,
            PresetIndex,
            DmiGen3ProjectSettings->LowFrequencyValue,
            DmiGen3ProjectSettings->FullSwingValue,
            DmiGen3ProjectSettings->PresetToCoefficient[PresetIndex]
            );
          SetSipGen4PresetToCoefficientMapping (
            RpDev,
            PresetIndex,
            DmiGen4ProjectSettings->LowFrequencyValue,
            DmiGen4ProjectSettings->FullSwingValue,
            DmiGen4ProjectSettings->PresetToCoefficient[PresetIndex]
            );
        } else {
          SetSipGen3PresetToCoefficientMapping (
            RpDev,
            LaneIndex,
            PresetIndex,
            DmiGen3ProjectSettings->LowFrequencyValue,
            DmiGen3ProjectSettings->FullSwingValue,
            DmiGen3ProjectSettings->PresetToCoefficient[PresetIndex]
            );
        }
      }
    }
  }

  ///
  /// Set  DMI Link Data Rate Sustain Policy (DLDRSP)
  /// When set, DMI will always train to the highest supported link speed common to both sides of the link.
  /// DLDRSP should be set to 1 when the Target Link Speed is still indicating GEN1
  ///
  if (RpDev->SipVersion >= PcieSip17) {
    Data32Or = B_DMI_PCR_CTRL2_DLDRSP | B_PCIE_CFG_CTRL2_PMETOFD;
    //
    // Program Link Speed Training Policy
    // and initial Link Speed requirement
    // it has to be done before programming of TLS
    //
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_AECR1G3,
      (UINT32)~B_PCIE_CFG_AECR1G3_DCDCCTDT,
      (UINT32)(B_PCIE_CFG_AECR1G3_DTCGCM | B_PCIE_CFG_AECR1G3_TPSE)
      );
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_MPC2,
      (UINT32)~B_PCIE_CFG_MPC2_ORCE,
      (UINT32)(B_PCIE_CFG_MPC2_PTNFAE | B_PCIE_CFG_MPC2_LSTP | B_PCIE_CFG_MPC2_EOIFD)
      );
  } else {
    Data32Or = B_DMI_PCR_CTRL2_DLDRSP;
  }
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_CTRL2,
    ~0u,
    Data32Or
    );
  //
  // Configure DMI Local Transmitter Coefficient Override
  //
  for (LaneIndex = 0; LaneIndex < PCH_MAX_DMI_LANES; LaneIndex +=2) {
    //
    // Enable Transmitter Coefficient Override
    // Set Transmitter Post-Cursor and Pre-Cursor Coefficient Override value to 0
    //
    Data32And = (UINT32)~(B_PCIE_CFG_LTCO1_L0TPOSTCO | B_PCIE_CFG_LTCO1_L0TPRECO | B_PCIE_CFG_LTCO1_L1TPOSTCO | B_PCIE_CFG_LTCO1_L1TPRECO);
    Data32Or  = 0;

    if (PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex].DmiTranCoOverEn == FORCE_ENABLE) {
      //
      // Enable Transmitter Coefficient Override
      //
      Data32Or |= B_PCIE_CFG_LTCO1_L0TCOE;
      //
      // Get Pre and Post Coefficient Override value from policy
      //
      Data32Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex].DmiTranCoOverPostCur << N_PCIE_CFG_LTCO1_L0TPOSTCO;
      Data32Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex].DmiTranCoOverPreCur << N_PCIE_CFG_LTCO1_L0TPRECO;
    }

    if (PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex + 1].DmiTranCoOverEn == FORCE_ENABLE) {
      //
      //Enable Transmitter Coefficient Override
      //
      Data32Or |= B_PCIE_CFG_LTCO1_L1TCOE;
      //
      // Get Pre and Post Coefficient Override value from policy
      //
      Data32Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex + 1].DmiTranCoOverPostCur << N_PCIE_CFG_LTCO1_L1TPOSTCO;
      Data32Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex + 1].DmiTranCoOverPreCur << N_PCIE_CFG_LTCO1_L1TPRECO;
    }
    if (Data32Or != 0) {
      //
      // Program Local Transmitter Coefficient Override register
      //
      RpDev->PciCfgAccess->AndThenOr32 (
                              RpDev->PciCfgAccess,
                              DmiLocTranCoefOverReg[LaneIndex/2],
                              Data32And,
                              Data32Or
                              );
    } else {
      RpDev->PciCfgAccess->AndThenOr32 (
                              RpDev->PciCfgAccess,
                              DmiLocTranCoefOverReg[LaneIndex/2],
                              0u,
                              0u
                              );
    }
  }
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  This function will perform necessary programming before changing PCH DMI link speed to Gen2 or Gen3.

  @param[in]  DmiSpeedGen          PCH DMI speed Gen (1-Gen1, 2-Gen2, 3-Gen3) to which link is going to be trained
  @param[in]  RpDev                Pointer to the root port device
**/
STATIC
VOID
DmiSipBeforeLinkSpeedChange (
  IN  UINT8                             DmiSpeedGen,
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32            Data32Or;
  UINT32            Data32And;
  UINT16            Data16Or;
  UINT16            Data16And;
  UINTN             LaneIndex;
  BOOLEAN           TestEnable;

  static CONST UINT8  DmiUpPortTranPreset[] = {
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7
  };
  static CONST UINT16 DmiLaneEqualCntrReg[] = {
    R_PCIE_CFG_L01EC,
    R_PCIE_CFG_L23EC,
    R_PCIE_CFG_L45EC,
    R_PCIE_CFG_L67EC
  };
  static CONST UINT8  DmiGen4UpPortTranPreset[] = {
    7,
    7,
    7,
    7,
    7,
    7,
    7,
    7
  };
  static CONST UINT16 DmiGen4LaneEqualCntrReg[] = {
    R_PCIE_CFG_PL16L01EC,
    R_PCIE_CFG_PL16L23EC,
    R_PCIE_CFG_PL16L45EC,
    R_PCIE_CFG_PL16L67EC
  };

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  EFI_STATUS                 Status;
  SI_PREMEM_POLICY_PPI       *SiPreMemPolicy;
  PCH_PCIE_RP_PREMEM_CONFIG  *PchPcieRpPreMemConfig;

  ///
  /// Get DMI settings through the PchPolicy PPI
  ///
  Status = PeiServicesLocatePpi (
             &gPchPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (EFI_ERROR (Status)) {
    Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  }
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPcieRpPreMemConfigGuid, (VOID *) &PchPcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  if (RpDev->SipVersion >= PcieSip17) {
    //
    // Configure Phy.
    // This programming has to be executed before equalization.
    //
    Data32Or = (UINT32) (B_PCIE_MEM_LPHYCP1_RXEQFNEVC | B_PCIE_MEM_LPHYCP1_RXADPHM);
    RpDev->PciPcrAccess->AndThenOr32 (RpDev->PciPcrAccess, R_PCIE_MEM_LPHYCP1, ~0u, Data32Or);
  }

  TestEnable = FALSE;
  if (DmiSpeedGen > 2) {
    for (LaneIndex = 0; LaneIndex < PCH_MAX_DMI_LANES; LaneIndex += 2) {
      Data32And = ~(UINT32) (B_DMI_PCR_L01EC_UPL0TP | B_DMI_PCR_L01EC_UPL1TP);
      Data32Or = (((UINT32)DmiUpPortTranPreset[LaneIndex] << N_DMI_PCR_L01EC_UPL0TP) |
                  ((UINT32)DmiUpPortTranPreset[LaneIndex + 1] << N_DMI_PCR_L01EC_UPL1TP));

      if (PchPcieRpPreMemConfig->PchDmiPrememConfig.DmiUpPortTranPresetEn == FORCE_ENABLE) {
        Data32Or = 0;
        Data32Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex].DmiUpPortTranPreset << N_DMI_PCR_L01EC_UPL0TP;
        Data32Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex + 1].DmiUpPortTranPreset << N_DMI_PCR_L01EC_UPL1TP;
      } else if (PchPcieRpPreMemConfig->PchDmiPrememConfig.DmiUpPortTranPresetEn == FORCE_DISABLE) {
        Data32Or = 0;
        Data32And = ~0u;
      }

      RpDev->PciPcrAccess->AndThenOr32 (
                             RpDev->PciPcrAccess,
                             DmiLaneEqualCntrReg[LaneIndex/2],
                             Data32And,
                             Data32Or
                             );

      if (RpDev->SipVersion >= PcieSip17) {
        Data16And = (UINT16) ~(B_DMI_PCR_PL16L01EC_UP16L0TP | B_DMI_PCR_PL16L01EC_UP16L1TP);
        Data16Or = (((UINT16)DmiGen4UpPortTranPreset[LaneIndex] << N_DMI_PCR_PL16L01EC_UP16L0TP) |
                    ((UINT16)DmiGen4UpPortTranPreset[LaneIndex + 1] << N_DMI_PCR_PL16L01EC_UP16L1TP));

        if (PchPcieRpPreMemConfig->PchDmiPrememConfig.DmiUpPortTranPresetEn == FORCE_ENABLE) {
          Data16Or = 0;
          Data16Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex].DmiUpPortTranPreset << N_DMI_PCR_PL16L01EC_UP16L0TP;
          Data16Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex + 1].DmiUpPortTranPreset << N_DMI_PCR_PL16L01EC_UP16L1TP;
        } else if (PchPcieRpPreMemConfig->PchDmiPrememConfig.DmiUpPortTranPresetEn == FORCE_DISABLE) {
          Data16Or = 0;
          Data16And = (UINT16)~0;
        } else {
          Data16Or = 0;
          Data16And = (UINT16) 0;
        }

        RpDev->PciPcrAccess->AndThenOr16 (
                               RpDev->PciPcrAccess,
                               DmiGen4LaneEqualCntrReg[LaneIndex/2],
                               Data16And,
                               Data16Or
                               );
      }
    }

    //
    // Configure DMI Local Transmitter Coefficient Override
    //
    for (LaneIndex = 0; LaneIndex < PCH_MAX_DMI_LANES; LaneIndex +=2) {
      //
      // Enable Transmitter Coefficient Override
      // Set Transmitter Post-Cursor and Pre-Cursor Coefficient Override value to 0
      //
      Data32And = (UINT32)~(B_PCIE_CFG_LTCO1_L0TPOSTCO | B_PCIE_CFG_LTCO1_L0TPRECO | B_PCIE_CFG_LTCO1_L1TPOSTCO | B_PCIE_CFG_LTCO1_L1TPRECO);
      Data32Or  = 0;

      if (PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex].DmiTranCoOverEn == FORCE_ENABLE) {
        //
        // Enable Transmitter Coefficient Override
        //
        Data32Or |= B_PCIE_CFG_LTCO1_L0TCOE;
        //
        // Get Pre and Post Coefficient Override value from policy
        //
        Data32Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex].DmiTranCoOverPostCur << N_PCIE_CFG_LTCO1_L0TPOSTCO;
        Data32Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex].DmiTranCoOverPreCur << N_PCIE_CFG_LTCO1_L0TPRECO;
      }

      if (PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex + 1].DmiTranCoOverEn == FORCE_ENABLE) {
        //
        //Enable Transmitter Coefficient Override
        //
        Data32Or |= B_PCIE_CFG_LTCO1_L1TCOE;
        //
        // Get Pre and Post Coefficient Override value from policy
        //
        Data32Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex + 1].DmiTranCoOverPostCur << N_PCIE_CFG_LTCO1_L1TPOSTCO;
        Data32Or |= PchPcieRpPreMemConfig->PchDmiPrememConfig.Lane[LaneIndex + 1].DmiTranCoOverPreCur << N_PCIE_CFG_LTCO1_L1TPRECO;
      }
      if (Data32Or != 0) {
        //
        // Program Local Transmitter Coefficient Override register
        //
        RpDev->PciCfgAccess->AndThenOr32 (
                               RpDev->PciCfgAccess,
                               DmiLocTranCoefOverReg[LaneIndex/2],
                               Data32And,
                               Data32Or
                               );
      } else {
        RpDev->PciCfgAccess->AndThenOr32 (
                               RpDev->PciCfgAccess,
                               DmiLocTranCoefOverReg[LaneIndex/2],
                               0u,
                               0u
                               );
      }
    }
    if (PchPcieRpPreMemConfig->PchDmiPrememConfig.DmiRtlepceb != FORCE_DISABLE) {
    TestEnable = TRUE;
    }

    SipDmiConfigurePresetCoefficientInitialization (TestEnable, RpDev);
    if (RpDev->SipVersion >= PcieSip17) {
      //
      // ADP-S PCH
      //
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P0P1PCM,     0x0084A01E);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P1P2P3PCM,   0x23200807);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P3P4PCM,     0x00028140);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P5P6PCM,     0x058C0124);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P6P7P8PCM,   0x1E204700);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P8P9PCM,     0x001E1145);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P10PCM,      0x0000D01B);
      //
      // Configure Lane0 Local PHY Full Swing and Low Frequency Value
      //
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0LFFS, 0x3010280e);

      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_PX16GP0P1PCM,   0x0084A01E);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_PX16GP1P2P3PCM, 0x23200807);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_PX16GP3P4PCM,   0x00028140);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_PX16GP5P6PCM,   0x058C0124);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_PX16GP6P7P8PCM, 0x1E204700);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_PX16GP8P9PCM,   0x001E1145);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_PX16GP10PCM,    0x0000D01B);
      //
      // Configure Lane0 Local PHY Full Swing and Low Frequency Value
      //
      RpDev->PciPcrAccess->Write32 (RpDev->PciPcrAccess, R_PCIE_CFG_PX16GLFFS, 0x0000280E);
    } else {
      //
      // other PCH
      //
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P0P1PCM,     0x006C8018);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P1P2P3PCM,   0x1C180685);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P3P4PCM,     0x00020100);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P5P6PCM,     0x047000DD);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P6P7P8PCM,   0x181835C0);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P8P9PCM,     0x0015B104);
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0P10PCM,      0x0000A016);
      //
      // Configure Lane0 Local PHY Full Swing and Low Frequency Value
      //
      RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_L0LFFS, 0x0000200A);
    }
  }

  ///
  /// Set  DMI Link Data Rate Sustain Policy (DLDRSP)
  /// When set, DMI will always train to the highest supported link speed common to both sides of the link.
  /// DLDRSP should be set to 1 when the Target Link Speed is still indicating GEN1
  ///
  if (RpDev->SipVersion >= PcieSip17) {
    Data32Or = B_DMI_PCR_CTRL2_DLDRSP | B_PCIE_CFG_CTRL2_PMETOFD;
    //
    // Program Link Speed Training Policy
    // and initial Link Speed requirement
    // it has to be done before programming of TLS
    //
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_AECR1G3,
      ~0u,
      (UINT32)(B_PCIE_CFG_AECR1G3_DTCGCM | B_PCIE_CFG_AECR1G3_TPSE)
      );
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_MPC2,
      (UINT32)~B_PCIE_CFG_MPC2_ORCE,
      (UINT32)(B_PCIE_CFG_MPC2_PTNFAE | B_PCIE_CFG_MPC2_LSTP | B_PCIE_CFG_MPC2_EOIFD)
      );
  } else {
    Data32Or = B_DMI_PCR_CTRL2_DLDRSP;
  }
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_CTRL2,
    ~0u,
    Data32Or
    );
  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  The function provides Max DMI Link Speed.

  @param[in]  RpDev                 Pointer to the root port device
  @return                           DMI Max Link speed is returned.
**/
UINT8
DmiSipGetMaxLinkSpeed (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  switch (DmiSipGetSupportedLinkSpeedsVector (RpDev)) {
    case (BIT4 | BIT3 | BIT2 | BIT1):
      return 4;
    case (BIT3 | BIT2 | BIT1):
      return 3;
    case (BIT2 | BIT1):
      return 2;
    case (BIT1):
      return 1;
    default:
      DEBUG ((DEBUG_INFO, "Invalid Gen Speed\n"));
      ASSERT (FALSE);
      return 0;
  }
}

/**
  The function set the Target Link Speed in PCH-H to DMI GEN 3.

  @param[in]  TargetLinkSpeed       Target Link Speed
                                    2: GEN2
                                    3: GEN3
  @param[in]  RpDev                 Pointer to the root port device
**/
VOID
DmiSipSetTargetLinkSpeed (
  IN  UINT8                             TargetLinkSpeed,
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32   MaxLinkSpeed;

  if (RpDev->Integration == UpDmi3) {
    MaxLinkSpeed = PcieSipGetMaxLinkSpeed (RpDev);
  } else {
    MaxLinkSpeed = DmiSipGetMaxLinkSpeed (RpDev);
  }

  DEBUG ((DEBUG_INFO, "%a with Tls (%d), and Mls (%d)\n", __FUNCTION__, TargetLinkSpeed, MaxLinkSpeed));

  if (RpDev->Integration == UpDmi3) {
    DmiSipPrepareEq (TargetLinkSpeed, RpDev);
  } else {
    DmiSipBeforeLinkSpeedChange (TargetLinkSpeed, RpDev);
  }

  ///
  /// PCH BIOS Spec, Section 7
  /// Configure DMI Link Speed (for PCH With DMI ONLY)
  /// Please refer to the System Agent BIOS Writer's Guide on Supported Link Speed
  /// field in Link Capabilities register in CPU complex. (Done in SA code)
  /// If target link speed is GEN2, it can be configured as early as possible.
  /// Else if the target link speed is GEN3, refer to PCH BIOS spec for further programming requirement
  ///
  /// Link speed can be set to GEN2/3 if both LCAP in CPU complex (Done in SA code)
  /// and on PCH side indicates such capability.
  /// DMI Target Link Speed (TLS) is configurable on PCH side in Link Control 2 register (LCTL2)
  ///
  if (MaxLinkSpeed >= (UINT32) TargetLinkSpeed) {
    ///
    /// Set Target Link Speed (TLS)
    ///
    RpDev->PciCfgAccess->AndThenOr8 (
                           RpDev->PciCfgAccess,
                           R_PCIE_CFG_LCTL2,
                           (UINT8)~B_PCIE_CFG_LCTL2_TLS,
                           TargetLinkSpeed
                           );
    ///
    /// Please refer to the System Agent BIOS Writer's Guide to perform DMI Link Retrain after
    /// configures new DMI Link Speed. (Done in SA code)
    ///
  }
  if (RpDev->PrivateConfig.TriggerDmiTraining) {
    DEBUG ((DEBUG_INFO, "DmiSipSetTargetLinkSpeed programming link retrain"));
    RpDev->PciCfgAccess->AndThenOr16 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_LCTL,
      (UINT16)~(B_PCIE_CFG_LCTL_CCC),
      B_PCIE_CFG_LCTL_CCC
    );
    RpDev->PciCfgAccess->AndThenOr16 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_LCTL,
      (UINT16)~(B_PCIE_CFG_LCTL_RL),
      B_PCIE_CFG_LCTL_RL
    );
 }

  DEBUG ((DEBUG_INFO, "%a(%d) End\n", __FUNCTION__, TargetLinkSpeed));
}

/**
  The function configure the L1ss for DMI.

  @param[in]  RpDev                 Pointer to the root port device
**/
VOID
DmiConfigureL1ss (
  IN  PCIE_ROOT_PORT_DEV                *RpDev
  )
{
  UINT32       Data32;
  UINT32       Data32Or;
  UINT32       Data32And;

  Data32And = (UINT32)~(B_PCIE_CFG_L1SCTL1_L12LTRTLSV | B_PCIE_CFG_L1SCTL1_L12LTRTLV_MASK | B_PCIE_CFG_L1SCTL1_AL11E |
                        B_PCIE_CFG_L1SCTL1_PPL11E | B_PCIE_CFG_L1SCTL1_AL12E | B_PCIE_CFG_L1SCTL1_PPL12E);
  Data32Or  = (V_PCIE_CFG_L1SCTL1_L12LTRTLSV << N_PCIE_CFG_L1SCTL1_L12LTRTLSV) | (V_PCIE_CFG_L1SCTL1_L12LTRTLV << N_PCIE_CFG_L1SCTL1_L12LTRTLV);

  Data32 = RpDev->PciPcrAccess->Read32 (RpDev->PciPcrAccess, R_PCIE_CFG_L1SCAP);
  if (Data32 & B_PCIE_CFG_L1SCAP_AL12S) {
    Data32Or |= B_PCIE_CFG_L1SCTL1_AL12E | B_PCIE_CFG_L1SCTL1_PPL12E;
  }
  RpDev->PciCfgAccess->AndThenOr32 (
    RpDev->PciCfgAccess,
    R_PCIE_CFG_L1SCTL1,
    Data32And,
    Data32Or
  );
  DEBUG ((DEBUG_INFO, "Dmi R_PCIE_CFG_L1SCTL1 = %x \n", RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_L1SCTL1)));

  RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_DCTL2, B_PCIE_CFG_DCTL2_LTREN);
}

/**
  This function configures Port 80 decode settings.

  @param[in]  RpDev                Pointer to the root port device
  @param[in]  IocRpDestinationId   IOC Root Port destination ID
**/
VOID
DmiSipConfigurePort80Decode (
  IN  PCIE_ROOT_PORT_DEV     *RpDev,
  IN  IOC_ROOT_PORT_DEST_ID  IocRpDestinationId
  )
{
  UINT16 RpDestinationId;
  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));
  RpDestinationId = (UINT16)((0x2 << 12) |
                                  (IocRpDestinationId.PsfId << 8) |
                                  (IocRpDestinationId.PortGroupId << 7) |
                                  (IocRpDestinationId.PortId << 3) |
                                   IocRpDestinationId.ChannelId);
  //
  // Set GCS.RPR (bit 11) to forward Port8x cycles to given rootport.
  // Use byte access in order to not modify GCS.BILD.
  //
  RpDev->PciPcrAccess->Or8 (RpDev->PciPcrAccess, R_DMI_PCR_GCS + 1, BIT3);
  RpDev->PciPcrAccess->Write16 (RpDev->PciPcrAccess, R_DMI_PCR_GCS + 2, RpDestinationId);
}

/**
  Reads out internal signal state from the SIP IP hardware.
  This is accomplished with monitor mux register in RP.

  @param[in] DmiBar           Dmi base address
  @param[in] MonitorCategory  Monitor Category
  @param[in] MonitorSegment   Monitor Segment (Lane number)
  @param[in] MonitorCategory  Monitor Signal (To Indentify For Which Gen Speed Signal State Is Required)

  @return Value of the signal state.
**/
STATIC
UINT32
DmiSipGetInternalSignalState (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                MonitorCategory,
  IN UINT32                MonitorSegment,
  IN UINT32                MonitorSignal
  )
{
    UINT32      SignalState;

    SignalState = 0;

  if (MonitorCategory > 0xF ||
      MonitorSegment >= 0xF ||
      MonitorSignal > 0xFF) {
    DEBUG ((DEBUG_ERROR, "Failed to read monitor signal, index out of range C:%X S:%X S:%X\n",
                           MonitorCategory, MonitorSegment, MonitorSignal));
    return 0xFFFFFFFF;
  }

  RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_MM, MonitorSignal);

  RpDev->PciCfgAccess->Write32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_CDM,
                         MonitorCategory << N_PCIE_CFG_CDM_MCS | MonitorSegment << N_PCIE_CFG_CDM_MSS);
  SignalState = RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_MM) >> N_PCIE_CFG_MM_MSST;

  SignalState = (SignalState >> 12) & 0xF;
  return SignalState;
}

/**
  Dumps best EQ coefficients values that were selected during EQ.

  @param[in]  RpDev        Pointer to the root port device
  @param[in]  RpLaneIndex  Lane index
  @param[in]  Speed        Speed for which best Coefficients are required
**/

VOID
DmiSipDumpBestEqCoefficients (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                RpLaneIndex,
  IN UINT8                 Speed
  )
{
  UINT32      SignalState;
  PCH_DMI_HOB            *PchDmiHob;


  PchDmiHob = NULL;
  PchDmiHob = (PCH_DMI_HOB *)GetFirstGuidHob(&gPchDmiHobGuid);

  if (PchDmiHob == NULL) {
    DEBUG ((DEBUG_INFO, "PchDmiHob is NULL !\n"));
    ASSERT (FALSE);
    return;
  }

  DEBUG ((DEBUG_INFO, "Lane %d:", RpLaneIndex));

  if (Speed == PcieGen3) {
    SignalState = DmiSipGetInternalSignalState (
                    RpDev,
                    R_PCIE_SIP_MONITOR_CAT2,
                    RpLaneIndex,
                    R_PCIE_SIP_MONITOR_CAT2_BEST4COEFF_GEN3);
    DEBUG ((DEBUG_INFO, "GEN3: %X\n", SignalState));
    PchDmiHob->CmGen3[RpLaneIndex] = (UINT8) SignalState;
    PchDmiHob->CpGen3[RpLaneIndex] = (UINT8) SignalState;
  }

  if (Speed == PcieGen4) {
    SignalState = DmiSipGetInternalSignalState (
                    RpDev,
                    R_PCIE_SIP_MONITOR_CAT2,
                    RpLaneIndex,
                    R_PCIE_SIP_MONITOR_CAT2_BEST4COEFF_GEN4);
    DEBUG ((DEBUG_INFO, "GEN4: %X\n", SignalState));
    PchDmiHob->CmGen4[RpLaneIndex] = (UINT8) SignalState;
    PchDmiHob->CpGen4[RpLaneIndex] = (UINT8) SignalState;
  }

  if (Speed >= PcieGen5) {
    SignalState = DmiSipGetInternalSignalState (
                    RpDev,
                    R_PCIE_SIP_MONITOR_CAT2,
                    RpLaneIndex,
                    R_PCIE_SIP_MONITOR_CAT2_BEST4COEFF_GEN5);
    DEBUG ((DEBUG_INFO, "GEN5: %X\n", SignalState));
  }
}