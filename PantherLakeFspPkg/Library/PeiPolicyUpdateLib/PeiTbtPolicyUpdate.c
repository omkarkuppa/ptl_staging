/** @file
  This file is SampleCode of the library for Intel TBT PEI
  Platform Policy Update.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/FspCommonLib.h>
#include <FspsUpd.h>
#include <Library/ConfigBlockLib.h>
#if FixedPcdGetBool(PcdTcssSupport) == 1
#include <TcssDataHob.h>
#include <PeiITbtConfig.h>
#endif
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <TcssInfo.h>
#include <TcssPeiConfig.h>
/**
  This function performs TBT PEI Policy update.

  @param[in] SiPolicy       The SI Policy PPI instance
  @param[in] FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiTbtPolicy (
  IN OUT SI_POLICY_PPI    *SiPolicy,
  IN     FSPS_UPD         *FspsUpd
  )
{
#if FixedPcdGetBool(PcdTcssSupport) == 1
  EFI_STATUS                      Status;
  UINT8                           Index;
  PEI_ITBT_CONFIG                 *PeiITbtConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP FspUpdatePeiTbtPolicy\n"));

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPeiITbtConfigGuid, (VOID *) &PeiITbtConfig);
  ASSERT_EFI_ERROR (Status);

  PeiITbtConfig->ITbtGenericConfig.ITbtForcePowerOnTimeoutInMs = FspsUpd->FspsConfig.ITbtForcePowerOnTimeoutInMs;
  PeiITbtConfig->ITbtGenericConfig.Usb4CmMode                  = FspsUpd->FspsConfig.Usb4CmMode;
  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index ++) {
    PeiITbtConfig->ITbtDmaLtr[Index] = FspsUpd->FspsConfig.ITbtDmaLtr[Index];
  }

  PeiITbtConfig->ITbtGenericConfig.ITbtConnectTopologyTimeoutInMs = FspsUpd->FspsConfig.ITbtConnectTopologyTimeoutInMs;
  PeiITbtConfig->ITbtGenericConfig.ITbtPcieTunnelingForUsb4 = FspsUpd->FspsConfig.ITbtPcieTunnelingForUsb4;
#endif
  return EFI_SUCCESS;
}
