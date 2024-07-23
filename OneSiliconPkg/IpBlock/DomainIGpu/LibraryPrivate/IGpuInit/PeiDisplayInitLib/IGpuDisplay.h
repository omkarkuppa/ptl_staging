/** @file
  The GUID definition for Graphics Info Lib

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef __DOMAIN_IGPU_DISPLAY_LIB_H__
#define __DOMAIN_IGPU_DISPLAY_LIB_H__

#include <Uefi/UefiBaseType.h>
#include <Pi/PiPeiCis.h>
#include <IpWrapperCntxtInfoClient.h>
#include <Library/DebugLib.h>
#include <IpIGpu.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiServicesLib.h>
#include <IGpuConfig.h>
#include <IGpuDataHob.h>
#include <IGpuInit.h>
#include <Guid/GraphicsInfoHob.h>
#include <Library/DomainIGpuInit.h>
#include <Library/ReportStatusCodeLib.h>
#include <IntelRcStatusCode.h>
#include <Library/IGpuInfoLib.h>
#include <Library/MtrrLib.h>
#include <Ppi/Graphics.h>
#include <Library/FspErrorInfoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <GpioV2Functionalities.h>
#include <GpioV2Signals.h>

/**
  Configure Display GPIO's

  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioV2Signals
  @param[in]  PinMux              GPIO Native pin mux platform config.
**/
EFI_STATUS
EFIAPI
ConfigureDisplayGpio (
  IN  UINT32           PadFunction,
  IN  UINT32           PinMux
  );

/**
  IGpuPeiNotifyCallback to call the GOP and fill the display

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
IGpuPeiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  IGpuCallPpiAndFillFrameBuffer: Call GraphicsInitPeim PeiGraphicsPpi to initalize display and get Mode info.
  Publish GraphicsInfoHob and call FillFrameBufferAndShowLogo

  @param[in]  pInst       The instance

  @retval    EFI_OUT_OF_RESOURCES - no enough reosurce for completing this task.
  @retval    EFI_STATUS - status code from calling several sub functions.
**/
EFI_STATUS
EFIAPI
IGpuCallPpiAndFillFrameBuffer (
  IP_IGPU_INST  *pInst
  );

/**
  IGpuFillFrameBufferAndShowLogo: Fill frame buffer with the image

  @param[in]  pInst       The instance

  @retval    EFI_STATUS
**/
EFI_STATUS
IGpuFillFrameBufferAndShowLogo (
  IP_IGPU_INST                       *pInst,
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE  *mMode
  );

#endif // __DOMAIN_IGPU_DISPLAY_LIB_H__
