/** @file
  BR router support Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#include <Library/DxeTbtDisBmeLib.h>
#include <Library/BrRouterSupportLib.h>

/**
  Enable PCI device Bus Master bit

  @param[in]  Sbdf       device's segment:bus:device:function coordinates
**/
VOID
EnableBme (
  IN SBDF       Sbdf
  )
{
  UINT16  Command;
  UINT64  DeviceCmdRegAddress;

  DeviceCmdRegAddress = PCI_SEGMENT_LIB_ADDRESS (Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func, PCI_COMMAND_OFFSET);

  Command = PciSegmentRead16 (DeviceCmdRegAddress);
  DEBUG ((DEBUG_INFO, " > Before enable BME %02x/%02x/%02x/%02x: Command: 0x%x \n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func, Command));

  if ((Command & EFI_PCI_COMMAND_BUS_MASTER) != EFI_PCI_COMMAND_BUS_MASTER) {
    DEBUG ((DEBUG_INFO, "  >> Enabling BME bit"));
    PciSegmentOr16 (DeviceCmdRegAddress, (UINT16)EFI_PCI_COMMAND_BUS_MASTER);
  }
  if ((Command & EFI_PCI_COMMAND_MEMORY_SPACE) != EFI_PCI_COMMAND_MEMORY_SPACE) {
    DEBUG ((DEBUG_INFO, "  >> Enabling MSE bit"));
    PciSegmentOr16 (DeviceCmdRegAddress, (UINT16)EFI_PCI_COMMAND_MEMORY_SPACE);
  }

  DEBUG ((DEBUG_INFO, " > After enable BME %02x/%02x/%02x/%02x: Command: 0x%x \n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func, PciSegmentRead16 (DeviceCmdRegAddress)));
}

/**
  Checks if device at given address is BR device router.

  @param[in,out] Sbdf   device's segment:bus:device:function coordinates

  @retval               TRUE when it's BR DR; FALSE if it's not BR DR
**/
BOOLEAN
IsBrDeviceRouter (
  IN SBDF      *Sbdf
  )
{
  UINT32    DidVid;
  UINT16    VendorId;
  UINT16    DeviceId;

  DidVid   = PciSegmentRead32 (SbdfToBase (*Sbdf));
  VendorId = DidVid & 0x0000FFFF;
  DeviceId = DidVid >> 16;

  if (VendorId == PCI_VID_INTEL) {
    switch (DeviceId) {
      case PCI_DID_BR_HUB_80:
      case PCI_DID_BR_HUB_40:
        DEBUG ((DEBUG_INFO, " Found BR DR - DeviceId: 0x%X\n", DeviceId));
        return TRUE;
    }
  }

  return FALSE;
}

/**
  Recursive BR Hierarchy Configuration

  @param[in]  Sbdf       device's segment:bus:device:function coordinates
**/
VOID
FindBrHierarchyConfiguration  (
  IN SBDF           Sbdf
  )
{
  SBDF          ChildSbdf = {0,0,0,0,0};
  PCI_DEV_TYPE  DevType;
  UINT32        VidDid;

  DEBUG ((DEBUG_INFO, "==================== Recursive start from %02x/%02x/%02x/%02x ====================\n",
          Sbdf.Seg,
          Sbdf.Bus,
          Sbdf.Dev,
          Sbdf.Func
        ));

  DevType   = DevTypeMax;
  VidDid    = PciSegmentRead32 (SbdfToBase (Sbdf));

  if ((VidDid == 0) || (VidDid == 0xFFFFFFFF)) {
    DEBUG ((DEBUG_ERROR, " Controller doesn't exist or it's not supported, value: %d\n", VidDid));
    return;
  }

  DevType = GetDeviceType (Sbdf);
  DEBUG ((DEBUG_INFO, " DevType: %d - (0:RP / 1:EndPoint / 2:US / 3:DS / 4:Init)\n", DevType));

  if (HasChildBus (Sbdf, &ChildSbdf) == TRUE) {
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      FindBrHierarchyConfiguration (ChildSbdf);
    }
  }

  if (IsBrDeviceRouter (&Sbdf)) {
    if (DevType != DevTypePcieEndpoint) {
      EnableBme (Sbdf);
    }
  }

  DEBUG ((DEBUG_INFO, "==================== Recursive End for %02x/%02x/%02x/%02x ====================\n",
          Sbdf.Seg,
          Sbdf.Bus,
          Sbdf.Dev,
          Sbdf.Func
        ));
}