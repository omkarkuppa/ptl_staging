/** @file
  This file provides services for Gpio policy function

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
#include <Uefi.h>
#include <Library/ConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Protocol/PchPolicy.h>
#include <ConfigBlock/Gpio/GpioDevConfig.h>

/**
  Print GPIO_DXE_CONFIG and serial out.

  @param[in] PchPolicy            Pointer to a PCH_POLICY_PROTOCOL
**/
VOID
GpioDxePrintConfig (
  IN PCH_POLICY_PROTOCOL    *PchPolicy
  )
{
  EFI_STATUS        Status;
  GPIO_DXE_CONFIG   *GpioDxeConfig;

  Status = GetConfigBlock ((VOID *) PchPolicy, &gGpioDxeConfigGuid, (VOID *) &GpioDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ GPIO DXE Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " HideGpioAcpiDevice : %d\n", GpioDxeConfig->HideGpioAcpiDevice));
}

/**
  Load DXE Config block default for GPIO

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
GpioDxeLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  GPIO_DXE_CONFIG  *GpioDxeConfig;
  GpioDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "GpioDxeConfig->Header.GuidHob.Name = %g\n", &GpioDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "GpioDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", GpioDxeConfig->Header.GuidHob.Header.HobLength));

  GpioDxeConfig->HideGpioAcpiDevice = 0;
}

STATIC COMPONENT_BLOCK_ENTRY  mGpioBlocks = {
  &gGpioDxeConfigGuid,
  sizeof (GPIO_DXE_CONFIG),
  GPIO_DXE_CONFIG_REVISION,
  GpioDxeLoadConfigDefault
};

/**
  Get Gpio config block table size.

  @retval      Size of config block
**/
UINT16
GpioDxeGetConfigBlockTotalSize (
  VOID
  )
{
  return mGpioBlocks.Size;
}

/**
  Add Gpio ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
GpioDxeAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mGpioBlocks, 1);
}
