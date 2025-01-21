/** @file
  PCH private PMC Library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PmcLib.h>
#include <Register/PmcRegs.h>

/**
  Lock PMC features in DXE and setup boot script to do the same
  on S3 resume.

  @attention This function will lock down all remaining PMC features
  that couldn't be locked on EndOfPei event. Currently we are locking down
  debug mode configuration and host read access to PMC XRAM. This must be done
  before EndOfDxe and on all boot modes.

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcLockWithS3BootScript (
  IN UINTN  PchPwrmBase
  )
{
  //
  // Lock PMC debug mode configuration and host read access to PMC XRAM.
  //
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_PM_CFG, B_PMC_PWRM_PM_CFG_DBG_MODE_LOCK | B_PMC_PWRM_PM_CFG_PMC_READ_DISABLE);

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) PchPwrmBase + R_PMC_PWRM_PM_CFG,
    1,
    (VOID *) ((UINTN) PchPwrmBase + R_PMC_PWRM_PM_CFG)
    );
}

/**
  This S3 BootScript only function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
**/
VOID
PmcDisableCf9GlobalResetInS3BootScript (
  VOID
  )
{
  UINT32                          Data;

  UINTN                           PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  Data = MmioRead32 (PchPwrmBase + R_PMC_PWRM_ETR3);

  Data &= (UINT32) ~B_PMC_PWRM_ETR3_CF9GR;

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PchPwrmBase +
    R_PMC_PWRM_ETR3,
    1,
    &Data
    );
}

/**
  This S3 BootScript only function disables triggering Global Reset of both
  the Host and the ME partitions after CF9h write of 6h or Eh.
  Global Reset configuration is locked after programming
**/
VOID
PmcDisableCf9GlobalResetWithLockInS3BootScript (
  VOID
  )
{
  UINT32                          Data;

  UINTN                           PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  Data = MmioRead32 (PchPwrmBase + R_PMC_PWRM_ETR3);

  Data &= (UINT32) ~B_PMC_PWRM_ETR3_CF9GR;
  Data |= (UINT32) B_PMC_PWRM_ETR3_CF9LOCK;

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PchPwrmBase +
    R_PMC_PWRM_ETR3,
    1,
    &Data
    );
}

/**
  This function sets SLP_SX Stretching Policy and adds
  lock setting to S3 Boot Script

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcLockSlpSxStretchingPolicyWithS3BootScript (
  IN UINTN  PchPwrmBase
  )
{
  MmioOr8 (
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B + 2,
    (UINT8) ((B_PMC_PWRM_GEN_PMCON_B_SLPSX_STR_POL_LOCK) >> 16)
    );

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B + 2,
    1,
    (VOID *) (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B + 2)
    );
}

/**
  This function sets SMI Lock with S3 Boot Script programming

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcLockSmiWithS3BootScript (
  IN UINTN  PchPwrmBase
  )
{
  MmioOr8 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B, B_PMC_PWRM_GEN_PMCON_B_SMI_LOCK);

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B,
    1,
    (VOID *) (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B)
    );
}

/**
  This function locks PMC Set Strap Message interface with S3 Boot Script programming
**/
VOID
PmcLockSetStrapMsgInterfaceWithS3BootScript (
  VOID
  )
{
  UINTN  PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  MmioOr32 ((UINTN) PchPwrmBase + R_PMC_PWRM_SSML, B_PMC_PWRM_SSML_SSL);

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint8,
    PchPwrmBase + R_PMC_PWRM_SSML,
    1,
    (VOID *) (PchPwrmBase + R_PMC_PWRM_SSML)
    );
}

#define PMC_CRID_UIP_TIMEOUT  0x1000000
/**
  Wait for CRID update ready.
**/
STATIC
VOID
PmcWaitForCridUip (
  VOID
  )
{
  UINTN     PchPwrmBase;
  UINT32    Timeout;

  PchPwrmBase = PmcGetPwrmBase ();
  Timeout     = 0;
  while (MmioRead32 (PchPwrmBase + R_PMC_PWRM_CRID_UIP) & B_PMC_PWRM_CRID_UIP_CRID_UIP) {
    if (Timeout++ > PMC_CRID_UIP_TIMEOUT) {
      ASSERT (FALSE);
      break;
    }
  }
}

/**
  Select CRID0 mode.
  The silicon Revision ID will be changed. Refer to silicon spec for RID value of CRID0.
**/
VOID
PmcSetCrid0WithS3BootScript (
  IN UINTN   PchPwrmBase
  )
{
  UINT32  Crid;
  UINT32  Data32And;
  UINT32  Data32;

  Crid = MmioRead32 (PchPwrmBase + R_PMC_PWRM_CRID);
  //
  // If Crid is locked or RID_SEL is changed then skip the detection.
  //
  if ((Crid & B_PMC_PWRM_CRID_CRID_LK) != 0) {
    ASSERT (FALSE);
  }

  PmcWaitForCridUip ();
  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_CRID, V_PMC_PWRM_CRID_RID_SEL_CRID0);
  PmcWaitForCridUip ();

  Data32And = B_PMC_PWRM_CRID_UIP_CRID_UIP;
  Data32    = 0;
  S3BootScriptSaveMemPoll (
    S3BootScriptWidthUint32,
    PchPwrmBase + R_PMC_PWRM_CRID_UIP,
    &Data32And,
    &Data32,
    1,
    PMC_CRID_UIP_TIMEOUT
    );

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PchPwrmBase + R_PMC_PWRM_CRID,
    1,
    (VOID *) (PchPwrmBase + R_PMC_PWRM_CRID)
    );

  S3BootScriptSaveMemPoll (
    S3BootScriptWidthUint32,
    PchPwrmBase + R_PMC_PWRM_CRID_UIP,
    &Data32And,
    &Data32,
    1,
    PMC_CRID_UIP_TIMEOUT
    );
}

/**
  Lock CRID.

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcLockCridWithS3BootScript (
  IN UINTN  PchPwrmBase
  )
{
  MmioOr32 (
    PchPwrmBase + R_PMC_PWRM_CRID,
    B_PMC_PWRM_CRID_CRID_LK
    );

  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PchPwrmBase + R_PMC_PWRM_CRID,
    1,
    (VOID *) (PchPwrmBase + R_PMC_PWRM_CRID)
    );
}

/**
  This function sets eSPI SMI Lock
  @attention This function must be called after eSPI SMI generation has been enabled.
    This setting is required in all boot modes and before EndOfDxe.
    If set value will be restored upon S3 resume by bootscript.
**/
VOID
PmcLockEspiSmiWithS3BootScript (
  VOID
  )
{
  UINT8   Data8Or;
  UINT8   Data8And;

  Data8Or  = (UINT8) (B_PMC_PWRM_GEN_PMCON_A_ESPI_SMI_LOCK >> 8);
  Data8And = (UINT8)~((B_PMC_PWRM_GEN_PMCON_A_PF | B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS) >> 8);

  MmioAndThenOr8 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 1, Data8And, Data8Or);
  S3BootScriptSaveMemReadWrite (
    S3BootScriptWidthUint8,
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 1,
    &Data8Or,
    &Data8And
    );
}
