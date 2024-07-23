/** @file
  This file is BasePreSiliconEnvDetectLib library is used to detect Simics environment.

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PreSiliconEnvDetectLib.h>

/**
  Check if currently run in Simics environment or not.

  @retval TRUE                  Run in Simics environment
  @retval FALSE                 Not run in Simics environment
**/
BOOLEAN
IsSimicsEnvironment (
  VOID
  )
{
  UINT8 PlatformId;

  PlatformId = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_SIM_RECOGNITION_OFFSET));

  return (PlatformId == V_SA_MC_SIM_RECOGNITION_PLATFORM_SIMICS);
}

/**
  Check if currently run in HFPGA environment or not.

  @retval TRUE                  Run in HFPGA environment
  @retval FALSE                 Not run in HFPGA environment
**/
BOOLEAN
IsHfpgaEnvironment (
  VOID
  )
{
  UINT8 PlatformId;

  PlatformId = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_SIM_RECOGNITION_OFFSET));

  return (PlatformId == V_SA_MC_SIM_RECOGNITION_PLATFORM_HFPGA);
}

/**
  Check if currently run in HSLE environment or not.

  @retval TRUE                  Run in HSLE environment
  @retval FALSE                 Not run in HSLE environment
**/
BOOLEAN
IsHSLEEnvironment (
  VOID
  )
{
  UINT8 PlatformId;

  PlatformId = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_SIM_RECOGNITION_OFFSET));

  return (PlatformId == V_SA_MC_SIM_RECOGNITION_PLATFORM_HSLE);
}

/**
  Check if currently run in HSLE UPF environment or not.

  @retval TRUE                  Run in HSLE UPF environment
  @retval FALSE                 Not run in HSLE UPF environment
**/
BOOLEAN
IsHSLEEnvironment_UPF (
  VOID
  )
{
  UINT8 PlatformId;

  PlatformId = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_SIM_RECOGNITION_OFFSET));

  return (PlatformId == V_SA_MC_SIM_RECOGNITION_PLATFORM_HSLE_UPF);
}

/**
  Check if currently run in FastEMU environment or not.

  @retval TRUE                  Run in FastEMU environment
  @retval FALSE                 Not run in FastEMU environment
**/
BOOLEAN
IsFastEMUEnvironment (
  VOID
  )
{
#ifdef V_SA_MC_SIM_RECOGNITION_PLATFORM_FASTEMU
  UINT8 PlatformId;

  PlatformId = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_SIM_RECOGNITION_OFFSET));

  return (PlatformId == V_SA_MC_SIM_RECOGNITION_PLATFORM_FASTEMU);
#else
  return FALSE;
#endif
}

/**
  Check if currently run in PSS environment or not.

  @retval TRUE                  Run in PSS environment
  @retval FALSE                 Not run in PSS environment
**/
BOOLEAN
IsPreSiEnvironment (
  VOID
  )
{
  UINT8 PlatformId;

  PlatformId = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_SIM_RECOGNITION_OFFSET));

  return (PlatformId == V_SA_MC_SIM_RECOGNITION_PLATFORM_HSLE)
          || (PlatformId == V_SA_MC_SIM_RECOGNITION_PLATFORM_HFPGA)
          || (PlatformId == V_SA_MC_SIM_RECOGNITION_PLATFORM_SIMICS)
#ifdef V_SA_MC_SIM_RECOGNITION_PLATFORM_FASTEMU
          || (PlatformId == V_SA_MC_SIM_RECOGNITION_PLATFORM_FASTEMU)
#endif
          || (PlatformId == V_SA_MC_SIM_RECOGNITION_PLATFORM_HSLE_UPF);
}
