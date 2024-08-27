/** @file
  PEIM to initialize Ipu FRU.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <IpuPreMemConfig.h>
#include <Library/IpuInitLib.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/DebugLib.h>
#include <Register/GpioAcpiDefines.h>
#include <GpioV2Services.h>
#include <Library/PcdLib.h>

/**
  Configure IMGUCLK

  @param[in] IpuPreMemPolicy    The IPU PreMem Policy instance
**/
VOID
ConfigureImguClkOutGpio (
  IN    IPU_PREMEM_CONFIG    *IpuPreMemPolicy
  )
{
  UINT8            Index;
  GPIOV2_SERVICES  *GpioServices;
  EFI_STATUS       Status;

  Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);

  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    DEBUG ((DEBUG_ERROR, "Failed to locate GPIO Services %r\n", Status));
  }

  for (Index = 0; Index < GPIO_IMGUCLK_NUMBER_OF_PINS; Index++) {
    if (IpuPreMemPolicy->ImguClkOutEn[Index] == TRUE) {
      PtlPcdGpioEnableImguClkOut (GpioServices, Index);
    }
  }
}