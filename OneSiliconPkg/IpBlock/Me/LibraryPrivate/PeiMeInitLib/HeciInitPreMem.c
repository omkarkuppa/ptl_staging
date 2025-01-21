/** @file
  Framework PEIM to HECI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2008 Intel Corporation.

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

#include <Ppi/HeciControlPpi.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PmcLib.h>
#include <Library/IoLib.h>
#include <Library/MeInitLib.h>
#include <Library/SpiLib.h>
#include <Library/SpiAccessLib.h>
#include <Register/PmcRegs.h>

/**
  Initialize early ME HOBs

  @retval EFI_SUCCESS             HECI1 initialized successfully
  @retval others                  Error occurred
**/
EFI_STATUS
InstallMeHobs (
  VOID
  )
{
  EFI_STATUS           Status;
  VOID                 *PmconHobPtr;
  VOID                 *FdoHobPtr;
  UINT32               RegVal32;
  UINT8                RegVal8;
  ME_HANDLE            *MeHandle;

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

  RegVal8  = 0;
  RegVal32 = 0;

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - Me Handle init failed!\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }

  PmconHobPtr = GetFirstGuidHob (&gMeSavedPmconHobGuid);
  if (PmconHobPtr == NULL) {
    RegVal32 = MmioRead32 (MeHandle->PmcPwrmBase + R_PMC_PWRM_GEN_PMCON_A);
    PmconHobPtr = BuildGuidDataHob (&gMeSavedPmconHobGuid, &RegVal32, sizeof (RegVal32));
    ASSERT (PmconHobPtr != NULL);
  }

  FdoHobPtr = GetFirstGuidHob (&gMeSavedFdoHobGuid);
  if (FdoHobPtr == NULL) {
    SpiServiceInit ();
    RegVal8 = SpiIsFlashDescriptorOverrideEnabled ();
    FdoHobPtr = BuildGuidDataHob (&gMeSavedFdoHobGuid, &RegVal8, sizeof (RegVal8));
    ASSERT (FdoHobPtr != NULL);
  }

  DEBUG ((DEBUG_INFO, "%a () - End\n", __FUNCTION__));

  return Status;
}

/**
  Internal function performing PM register initialization for Me
**/
VOID
MePmInit (
  VOID
  )
{
  ///
  /// Before system memory initialization, BIOS should check the wake status to determine if Intel Management Engine
  /// has reset the system while the host was in a sleep state. If platform was not in a sleep state
  /// BIOS should ensure a non-sleep exit path is taken. One way to accomplish this is by forcing
  /// an S5 exit path by the BIOS.
  ///
  if (PmcGetSleepTypeAfterWake () == PmcNotASleepState) {
    PmcSetSleepState (PmcS5SleepState);
    DEBUG ((DEBUG_INFO, "MePmInit () - Force an S5 exit path.\n"));
  }
}
