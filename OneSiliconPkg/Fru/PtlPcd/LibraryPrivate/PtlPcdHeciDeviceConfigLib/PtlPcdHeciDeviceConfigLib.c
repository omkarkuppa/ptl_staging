/** @file
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
**/

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Defines/PchReservedResources.h>
#include <MeDefines.h>
#include <HeciDeviceConfig.h>

HECI_DEVICE_LOCATION mHeciDeviceLocation[] = {
  /// CSME: HECI1
  {
    Csme,
    HECI1
  },
  /// CSME: HECI2
  {
    Csme,
    HECI2
  }
};

/**
  Builds the HOBs with HECI devices configurations.
  These HOBs are required by the HECI driver for device initialization.
**/
VOID
PtlPcdBuildHeciDeviceConfigHobs (
  VOID
  )
{
  HECI_DEVICE_CONFIG      *HeciDeviceConfigHob;
  UINT64                  HeciPciCfgAddr;
  UINT32                  TempMemoryBar;
  UINT16                  DeviceId;
  UINT8                   HeciDeviceCount;
  UINT8                   Index;
  VOID                    *GuidHob;

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

  GuidHob = GetFirstGuidHob (&gHeciDeviceConfigHobGuid);
  if (GuidHob != NULL) {
    DEBUG ((DEBUG_INFO, "%a () - End. HECI Device Config already installed!\n", __FUNCTION__));
    return;
  }

  HeciDeviceCount = sizeof (mHeciDeviceLocation) / sizeof (HECI_DEVICE_LOCATION);

  for (Index = 0; Index < HeciDeviceCount; Index++) {
    HeciPciCfgAddr = PCI_SEGMENT_LIB_ADDRESS (
                       0,
                       0,
                       PCI_DEVICE_NUMBER_PCH_HECI1,
                       mHeciDeviceLocation[Index].HeciDevFunction,
                       0
                       );
    DeviceId = PciSegmentRead16 (HeciPciCfgAddr + PCI_DEVICE_ID_OFFSET);
    if (DeviceId == 0xFFFF) {
      DEBUG ((
        DEBUG_WARN,
        "HECI device at location: [%d:%d:%d:%d] is disabled!\n",
        0,
        0,
        PCI_DEVICE_NUMBER_PCH_HECI1,
        mHeciDeviceLocation[Index].HeciDevFunction
        ));
      continue;
    }

    HeciDeviceConfigHob = BuildGuidHob (&gHeciDeviceConfigHobGuid, sizeof (HECI_DEVICE_CONFIG));
    if (HeciDeviceConfigHob == NULL) {
      DEBUG ((DEBUG_ERROR, "HOB could not be allocated!\n"));
      return;
    }

    TempMemoryBar = PCH_HECI1_BASE_ADDRESS + Index * PCH_HECI1_MMIO_SIZE;

    HeciDeviceConfigHob->SecurityEngine    = mHeciDeviceLocation[Index].SecurityEngine;
    HeciDeviceConfigHob->PciSegment        = 0;
    HeciDeviceConfigHob->PciBus            = 0;
    HeciDeviceConfigHob->PciDevice         = PCI_DEVICE_NUMBER_PCH_HECI1;
    HeciDeviceConfigHob->PciFunction       = mHeciDeviceLocation[Index].HeciDevFunction;
    HeciDeviceConfigHob->HeciInterruptMode = 0;
    HeciDeviceConfigHob->TempMemoryBar     = TempMemoryBar;

    DEBUG ((DEBUG_INFO, "HECI%d Config\n", HECI_DEVICE_TO_NUMBER (mHeciDeviceLocation[Index].HeciDevFunction)));
    DEBUG ((
      DEBUG_INFO,
      "\t[%d:%d:%d:%d] DeviceId: 0x%04x\n",
      HeciDeviceConfigHob->PciSegment,
      HeciDeviceConfigHob->PciBus,
      HeciDeviceConfigHob->PciDevice,
      HeciDeviceConfigHob->PciFunction,
      DeviceId
      ));
    DEBUG ((DEBUG_INFO, "\tHeciInterruptMode: 0x%02x\n", HeciDeviceConfigHob->HeciInterruptMode));
    DEBUG ((DEBUG_INFO, "\tTempMemoryBar: 0x%02x\n", HeciDeviceConfigHob->TempMemoryBar));
  }


  DEBUG ((DEBUG_INFO, "%a () - End\n", __FUNCTION__));
}