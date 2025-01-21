/** @file
  Implementation of DxeBoardConfigLib.

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
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/BoardConfigLib.h>
#include <Library/PchPciBdfLib.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/PchRegs.h>
#include <PcieRegs.h>
#include <Library/PciSegmentLib.h>
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
#include <DptfConfig.h>
#endif
#if FixedPcdGetBool (PcdMipiCamFeatureEnable) == 1
#include <MipiCamConfig.h>
#endif

/**
  Check if given rootport has device connected and enable wake capability

  @param[in]  RpNum           An unsigned integer represent the root port number.

  @retval                     TRUE if endpoint was connected
  @retval                     FALSE if no endpoint was detected
**/
BOOLEAN
IsPcieEndPointPresent (
  IN UINT8 RpNum
  )
{
  UINT64        RpBaseAddress;

  RpBaseAddress = PchPcieRpPciCfgBase (RpNum);

    if ((PciSegmentRead16 (RpBaseAddress) != 0xFFFF) &&
        (PciSegmentRead16 (RpBaseAddress + R_PCIE_SLSTS_OFFSET) & B_PCIE_SLSTS_PDS)) {
      return TRUE;
    }

  return FALSE;
}
