/** @file
  HSIO SOC layer Library for PTL-PCD

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

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/PchRegsHsio.h>
#include <Register/PchRegsHsioVer5.h>
#include <Library/PeiHsioLib.h>
#include <Library/PchInfoLib.h>
#include <P2SbController.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/MeInitLib.h>
#include <Library/PcdInfoLib.h>
#include <PcrDefine.h>
#include <PchPcieRpConfig.h>
#include <Pi/PiPeiCis.h>
#include <Library/PcieConfigAccessLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdHsioSocLib.h>
#include <Ppi/SiPolicy.h>
#include <PcdSbPortIds.h>

/**
  Enable dynamic power gating of the PHY

  @param[in out]  P2SbController Pointer to P2SB controller identification structure
  @param[in    ]  SbAccess       Pointer to Sideband access interface structure

**/
VOID
PtlPcdPhyPwrGatingProg (
  IN      P2SB_SIDEBAND_REGISTER_ACCESS  *SbAccess
  )
{
  UINT8             Index;
  REGISTER_ACCESS   *SbRegisterAccess;
  P2SB_CONTROLLER   P2SbController;
  EFI_STATUS        Status;
  PCH_PCIE_CONFIG   *PchPcieConfig;
  SI_POLICY_PPI     *SiPolicy;
  SiPolicy          = NULL;
  P2SB_PORT_16_ID   *PidsMpPhy;
  UINT8             PidsMpPhySize;
  P2SB_PORT_16_ID   PidsAicSocP [] = {
                      { .Pid16bit = PTL_SID_F2_PID_MPFPW1A },
                      { .Pid16bit = PTL_SID_F2_PID_MPFPW2A },
                      { .Pid16bit = PTL_P_H_SID_F2_PID_G5FPW1A }
                    };
  P2SB_PORT_16_ID   PidsAicSocH [] = {
                      { .Pid16bit = PTL_SID_F2_PID_MPFPW1A },
                      { .Pid16bit = PTL_SID_F2_PID_MPFPW2A },
                      { .Pid16bit = PTL_P_H_SID_F2_PID_G5FPW1A },
                      { .Pid16bit = PTL_H_SID_F2_PID_G5FPW2A }
                       };



  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicy
             );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID *) &PchPcieConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  PidsMpPhy = NULL;

  if (PtlIsPcdH ()) {
    PidsMpPhy = PidsAicSocH;
    PidsMpPhySize = (sizeof (PidsAicSocH) / sizeof (P2SB_PORT_16_ID));
  } else {
    PidsMpPhy = PidsAicSocP;
    PidsMpPhySize = (sizeof (PidsAicSocP) / sizeof (P2SB_PORT_16_ID));
  }

  for (Index = 0; Index < PidsMpPhySize; Index++) {
    PtlPcdGetP2SbController (&P2SbController, PidsMpPhy[Index]);
    BuildP2SbSidebandAccess (
     &P2SbController,
     PidsMpPhy[Index].PortId.LocalPid,
     0,
     P2SbPrivateConfig,
     P2SbMsgAccess,
     TRUE,
     SbAccess
    );

    SbRegisterAccess = (REGISTER_ACCESS *) SbAccess;
    //
    // Set BIT31 of PHYCOMMONREG12 register (access width 8)
    //
    if (PchPcieConfig->PcieCommonConfig.PhyAutoPowerGating) {
      SbRegisterAccess->Or8 (SbRegisterAccess, R_HSIO_VER5_PHYCOMMONREG12 + 3, BIT7);
    } else {
      SbRegisterAccess->And8 (SbRegisterAccess, R_HSIO_VER5_PHYCOMMONREG12 + 3, (UINT8)~(BIT7));
    }
  }
}

/**
  Enable the IOMT tool using the EOM bit that is reflected on CSE FW Status register
**/
VOID
PtlPcdPhyBeforeSaiPostBoot (
  IN      P2SB_SIDEBAND_REGISTER_ACCESS  *SbAccess
  )
{
  EFI_STATUS         Status;
  UINT8              PidIndex;
  UINT8              SaiIndex;
  REGISTER_ACCESS    *SbRegisterAccess;
  P2SB_CONTROLLER    P2SbController;
  P2SB_PORT_16_ID    *PidsMpPhy;
  UINT8              PidsMpPhySize;

  P2SB_PORT_16_ID    PidsAicSocP [] = {
                       { .Pid16bit = PTL_SID_F2_PID_MPFPW1A },
                       { .Pid16bit = PTL_SID_F2_PID_MPFPW2A },
                       { .Pid16bit = PTL_P_H_SID_F2_PID_G5FPW1A },
                       { .Pid16bit = PTL_SID_F2_PID_U3FPW1A },
                       { .Pid16bit = PTL_SID_F3_PID_TC_PGA0A },
                       { .Pid16bit = PTL_SID_F3_PID_TC_PGA1A },
                       { .Pid16bit = PTL_SID_F3_PID_TC_PGA2A },
                       { .Pid16bit = PTL_SID_F3_PID_TC_PGA3A }
                     };
  P2SB_PORT_16_ID    PidsAicSocH [] = {
                       { .Pid16bit = PTL_SID_F2_PID_MPFPW1A },
                       { .Pid16bit = PTL_SID_F2_PID_MPFPW2A },
                       { .Pid16bit = PTL_P_H_SID_F2_PID_G5FPW1A },
                       { .Pid16bit = PTL_H_SID_F2_PID_G5FPW2A },
                       { .Pid16bit = PTL_SID_F2_PID_U3FPW1A },
                       { .Pid16bit = PTL_SID_F3_PID_TC_PGA0A },
                       { .Pid16bit = PTL_SID_F3_PID_TC_PGA1A },
                       { .Pid16bit = PTL_SID_F3_PID_TC_PGA2A },
                       { .Pid16bit = PTL_SID_F3_PID_TC_PGA3A }
                     };



  UINT32 SaiPolicyReg[] = {
           R_HSIO_BTRS_SAI_WRITE_POLICY_LO,
           R_HSIO_BTRS_SAI_READ_POLICY_LO,
           R_HSIO_REGION0_SAI_WRITE_POLICY_LO,
           R_HSIO_REGION0_SAI_READ_POLICY_LO,
           R_HSIO_REGION1_SAI_WRITE_POLICY_LO,
           R_HSIO_REGION1_SAI_READ_POLICY_LO,
           R_HSIO_REGION2_SAI_WRITE_POLICY_LO,
           R_HSIO_REGION2_SAI_READ_POLICY_LO
         };

  PidsMpPhy = NULL;

  if (PtlIsPcdH ()) {
    PidsMpPhy = PidsAicSocH;
    PidsMpPhySize = (sizeof (PidsAicSocH) / sizeof (P2SB_PORT_16_ID));
  } else {
    PidsMpPhy = PidsAicSocP;
    PidsMpPhySize = (sizeof (PidsAicSocP) / sizeof (P2SB_PORT_16_ID));

  }

  if(!IsEom ()) {

    for (PidIndex = 0; PidIndex < PidsMpPhySize; PidIndex ++) {
      Status = PtlPcdGetP2SbController (&P2SbController, PidsMpPhy[PidIndex]);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        return;
      }
      for (SaiIndex = 0; SaiIndex < (sizeof (SaiPolicyReg) / sizeof (UINT32)); SaiIndex ++) {
        BuildP2SbSidebandAccess (
          &P2SbController,
          PidsMpPhy[PidIndex].PortId.LocalPid,
          0,
          P2SbPrivateConfig,
          P2SbMsgAccess,
          TRUE,
          SbAccess
          );
        SbRegisterAccess = (REGISTER_ACCESS *) SbAccess;
        //
        // WAC (WriteAccess Policy)  BIT0 = 1.
        //
        //
        SbRegisterAccess->Or32 (SbRegisterAccess, SaiPolicyReg[SaiIndex], BIT0);
      }
    }

  }
}