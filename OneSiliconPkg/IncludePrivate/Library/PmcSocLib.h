/** @file
  Header file for private PMC SOC Lib.

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

#ifndef _PEI_PMC_SOC_LIB_H_
#define _PEI_PMC_SOC_LIB_H_

#include <PmcSocConfig.h>

/**
  Checks if the Intel Touch Host Controller is Enabled

  @param[in] ThcNumber            THC0/THC1 0 or 1

  @retval TRUE                    THC_DIS strap is 0
  @retval FALSE                   THC_DIS strap is 1
**/
BOOLEAN
PmcIsThcEnabled (
  IN UINT8  ThcNumber
  );

/**
  This function checks if ISH is function disabled
  by static power gating

  @retval ISH device state
**/
BOOLEAN
PmcIsIshFunctionDisabled (
  VOID
  );

/**
  This function checks if ISH device is supported (not disabled by fuse)

  @retval ISH support state
**/
BOOLEAN
PmcIsIshSupported (
  VOID
  );

/**
  This function disables ISH device by static power gating.
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.

  @retval TRUE   Disabled ISH in PMC
  @retval FALSE  Failed to disable ISH in PMC
**/
BOOLEAN
PmcStaticDisableIsh (
  VOID
  );

/**
  This function enables ISH device by disabling static power gating
**/
VOID
PmcEnableIsh (
  VOID
  );

/**
  This function checks if GbE is function disabled
  by static power gating

  @retval GbE device state
**/
BOOLEAN
PmcIsGbeFunctionDisabled (
  VOID
  );

/**
  This function disables GbE device by static power gating
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableGbe (
  VOID
  );

/**
  This function enables GbE device by disabling static power gating
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableGbe (
  VOID
  );

/**
  This function checks if GbE device is supported (not disabled by fuse)

  @param[in] PmcMmio       PMC MMIO

  @retval GbE support state
**/
BOOLEAN
PmcIsGbeSupported (
  IN UINTN  PmcMmio
  );

/**
  This function enables all SerailIo devices
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcEnableSerialIo (
  IN UINTN  PmcMmio
  );

/**
  This function disables (static power gating) all SerailIo devices.
  For SerialIo controllers they can be power gated only if all of them are to be disabled.
  They cannot be statically power gated separately.
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcStaticDisableSerialIo (
  IN UINTN  PmcMmio
  );

/**
  This function checks if all SerialIo devices are statically disabled (static power gating)

  @param[in] PmcMmio        PMC MMIO

  @retval SerialIo disable state
**/
BOOLEAN
PmcIsSerialIoStaticallyDisabled (
  IN UINTN  PmcMmio
  );

/**
  This function checks if SerialIo device is supported (not disabled by fuse)

  @param[in] PmcMmio        PMC MMIO

  @retval SerialIo support state
**/
BOOLEAN
PmcIsSerialIoSupported (
  IN UINTN  PmcMmio
  );

/**
  This function checks if Cnvi device is supported (not disabled by fuse)

  @retval Cnvi support state
**/
BOOLEAN
PmcIsCnviSupported (
  VOID
  );

/**
  This function checks if CNVi is function disabled
  by static power gating

  @retval GbE device state
**/
BOOLEAN
PmcIsCnviFunctionDisabled (
  VOID
  );

/**
  This function enables CNVi device by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableCnvi (
  VOID
  );

/**
  This function disables CNVi device by static power gating
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableCnvi (
  VOID
  );

/**
  This function disables (non-static power gating) PCIe Root Port

  @param[in] RpIndex        PCIe Root Port Index (0 based)
**/
VOID
PmcDisablePcieRootPort (
  IN UINT32  RpIndex
  );

/**
  This function checks if SATA device is supported (not disabled by fuse)

  @param[in] SataCtrlIndex SATA controller index

  @retval SATA support state
**/
BOOLEAN
PmcIsSataSupported (
  IN UINT32  SataCtrlIndex
  );

/**
  This function checks if XDCI device is supported (not disabled by fuse)

  @retval XDCI support state
**/
BOOLEAN
PmcIsXdciSupported (
  VOID
  );

/**
  This function checks if SCS UFS device is supported (not disabled by fuse)

  @param[in] UfsNum     SCS UFS Device

  @retval SCS device support state
**/
BOOLEAN
PmcIsScsUfsSupported (
  IN UINT32   UfsNum
  );

/**
  This function checks RTC SCRATCH GCR register if SCS UFS is function disabled

  @retval SCS UFS disable state
**/
BOOLEAN
PmcIsScsUfsFunctionDisabled (
  VOID
  );

/**
  Check if PMC Extended straps are supported

  @retval  Status of Extended Straps support
**/
BOOLEAN
PmcIsExtendedStrapsSupported (
  VOID
  );

/**
  Check if PMC IPC1 command for CPU straps is supported.

  @retval  Status of CPU Straps support
**/
BOOLEAN
PmcIsCpuStrapsIpcCommandSupported (
  VOID
  );

/**
  This function gets Group to GPE0 configuration, based on Power Managment Controller (PMC) PWRM address.
  Input address gives opportiunity to configure PMC located inside SOC and PCH as well.
  @param[in]  PwrmAddress       PMC PWRM address
  @param[out] GpeDw0Value       GPIO Group to GPE_DW0 assignment
  @param[out] GpeDw1Value       GPIO Group to GPE_DW1 assignment
  @param[out] GpeDw2Value       GPIO Group to GPE_DW2 assignment
**/
VOID
PwrmPmcGetGpioGpe (
  IN  UINTN     PwrmAddress,
  OUT UINT32    *GpeDw0Value,
  OUT UINT32    *GpeDw1Value,
  OUT UINT32    *GpeDw2Value
  );
#endif
