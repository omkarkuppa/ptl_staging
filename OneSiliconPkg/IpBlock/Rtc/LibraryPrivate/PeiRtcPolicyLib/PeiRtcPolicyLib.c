/** @file
  This file provides services for RTC PEI policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <RtcConfig.h>

/**
  Print RTC_CONFIG settings.

  @param[in] SiPolicyPpi    Instance of SI_POLICY_PPI
**/
VOID
RtcPrintConfig (
  IN  SI_POLICY_PPI         *SiPolicyPpi
  )
{
  EFI_STATUS                Status;
  RTC_CONFIG                *RtcConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gRtcConfigGuid, (VOID *) &RtcConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ Rtc Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " BiosInterfaceLock = %x\n", RtcConfig->BiosInterfaceLock));
  DEBUG ((DEBUG_INFO, " MemoryLock = %x\n", RtcConfig->MemoryLock));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
RtcLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  RTC_CONFIG  *RtcConfig;
  RtcConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "RtcConfig->Header.GuidHob.Name = %g\n", &RtcConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "RtcConfig->Header.GuidHob.Header.HobLength = 0x%x\n", RtcConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Rtc configuration.
  ********************************/
  RtcConfig->BiosInterfaceLock  = TRUE;
  RtcConfig->MemoryLock         = TRUE;
}

STATIC COMPONENT_BLOCK_ENTRY  mRtcBlocks = {
  &gRtcConfigGuid,
  sizeof (RTC_CONFIG),
  RTC_CONFIG_REVISION,
  RtcLoadConfigDefault
};

/**
  Get Rtc config block table size.

  @retval      Size of config block
**/
UINT16
RtcGetConfigBlockTotalSize (
  VOID
  )
{
  return mRtcBlocks.Size;
}

/**
  Add Rtc ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
RtcAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mRtcBlocks, 1);
  return Status;
}
