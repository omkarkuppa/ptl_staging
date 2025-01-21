/** @file
Null Instance of Fru file for VMD feature initialization.

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

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <Uefi/UefiBaseType.h>
#include <PcieRegs.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiVmdInitLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/PcieSipRegs.h>
#include <Library/VmdInfoLib.h>
#include <Register/VmdRegs.h>
#include <Library/PeiVmdInitFruLib.h>
#include <Library/PeiImrInitLib.h>
#include <Register/CpuGenInfoRegs.h>
#include <Library/CpuLib.h>

#define VMD_DEVICE_ID        (0xB06F)

/**
  This function enumerate all downstream bridge.

  @param[in] BusNum  - Primary bus number of current bridge.

  @retval BusNum: return current bus number if current bus is an endpoint device.
  @retval SubBus: return subordinate bus number if current bus is a bridge.
**/
UINT8
VmdEnumerateDownstream (
  IN UINT8  BusNum
  )
{
  return 0;
}


/**
This function detects the mass storage devices attached to PEG Slots and PCH PCIe Root ports and the Sata controller.

@param[in]      *VmdInfoHob     pointer to VMD info Hob to store the information of detected devices.

@retval EFI_SUCCESS            Successfully initialized Pre-Mem configurations.
**/

EFI_STATUS
VmdDetectPcieStorageDevices (
    VMD_INFO_HOB               *VmdInfoHob
)
{
  return EFI_SUCCESS;
}

/**
  This function detects whether Root Bus1 is supported and if so, programs the additional set of registers for Root Bus 1.
  @param[in]      *VmdInfoHob  pointer to VMD info Hob to read the information of detected devices.
  @param[in, out] *VmConfig    pointer to VmConfig register structure.
**/

VOID
VmdInitFruApi (
  IN      VMD_INFO_HOB *VmdInfoHob,
  IN OUT  VMCONFIG_IOC_VMD_STRUCT *VmConfig
  )
{
  return;
}

/**
  Reserve VMD Stolen Memory.

  @param[in] TopUseableMemAddr          - Moving address pointer
  @param[in] Touud                      - Size leftover pointer
  @param[in] ResourceAttributeTested    - Resource attribute flag
**/

VOID
EFIAPI
VmdMemoryAllocation (
  IN OUT EFI_PHYSICAL_ADDRESS     *TopUseableMemAddr,
  IN OUT UINT64                   *Touud,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested
  )
{

}

/**
  Check if Vmd Memory reservation is required.

  @retval TRUE    If Memory reservation is required..
  @retval FALSE   If Memory reservation is not required..
**/

BOOLEAN
IsVmdMemoryAllocationRequired (
VOID
  )
{
  return FALSE;
}