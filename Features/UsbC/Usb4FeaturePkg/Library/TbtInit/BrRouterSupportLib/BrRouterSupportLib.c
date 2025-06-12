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
#include <Library/BrRouterSupportLib.h>
#include <Library/Usb4PlatformHob.h>
#include <Library/ItbtPcieRpLib.h>

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

  DEBUG ((
    DEBUG_INFO,
    "==================== Recursive start from %02x/%02x/%02x/%02x ====================\n",
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

  DEBUG ((
    DEBUG_INFO,
    "==================== Recursive End for %02x/%02x/%02x/%02x ====================\n",
    Sbdf.Seg,
    Sbdf.Bus,
    Sbdf.Dev,
    Sbdf.Func
    ));
}

/**
  EnableBrBmeCallBack

  Enable BME on BR Host and Device Router tree at EfiPciHostBridgeSetResources
  to allow MMIO propagation for loading OPROM from MMIO.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
EnableBrBmeCallBack (
  IN OUT VOID
  )
{
  UINT8               HrIndex;
  UINT8               Usb4HrCount;
  SBDF                Sbdf;
  VOID                *HobPtr;
  USB4_PLATFORM_HOB   *Usb4PlatformHob;
  USB4_PLATFORM_INFO  *Usb4PlatformInfo;
  USB4_HR_INFO        *Usb4HrInfo;
  EFI_STATUS          Status;
  UINTN               RpDev;
  UINTN               RpFunc;
  UINTN               RpSegment;
  UINTN               RpBus;

  DEBUG ((DEBUG_INFO, "%a() - Start\n", __FUNCTION__));

  HobPtr          = NULL;
  HobPtr          = GetFirstGuidHob (&gUsb4PlatformHobGuid);
  Usb4PlatformHob = NULL;
  Status          = EFI_SUCCESS;
  RpDev           = 0;
  RpFunc          = 0;
  RpSegment       = 0;
  RpBus           = 0;

  ZeroMem (&Sbdf, sizeof (Sbdf));

  if (HobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Unable to find USB4 platform hob!\n"));
    goto Exit;
  }

  //
  // Get USB4 platform info HOB
  //
  Usb4PlatformHob = GET_GUID_HOB_DATA (HobPtr);
  Usb4PlatformInfo = &(Usb4PlatformHob->Usb4PlatformInfo);
  Usb4HrCount = Usb4PlatformInfo->Usb4HrCount;

  if (Usb4HrCount > USB4_HR_SUPPORT_MAX) {
    DEBUG ((DEBUG_ERROR, "USB4 HR count invalid\n"));
    goto Exit;
  }

  for (HrIndex = 0; HrIndex < MAX_ITBT_PCIE_PORT; HrIndex++) {
    Status = GetItbtPcieRpInfo ((UINTN) HrIndex, &RpSegment, &RpBus, &RpDev, &RpFunc);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to get port[%d] info, Status: %r\n", HrIndex, Status));
      continue;
    }
    Sbdf.Seg  = (UINT32) RpSegment;
    Sbdf.Bus  = (UINT32) RpBus;
    Sbdf.Dev  = (UINT32) RpDev;
    Sbdf.Func = (UINT32) RpFunc;

    DEBUG ((
      DEBUG_INFO,
      "HR[%d] USB4 ITBT Root Port (SBDF) = (%02x/%02x/%02x/%02x)\n",
      HrIndex,
      Sbdf.Seg,
      Sbdf.Bus,
      Sbdf.Dev,
      Sbdf.Func
      ));
    FindBrHierarchyConfiguration (Sbdf);
  }

  for (HrIndex = 0; HrIndex < Usb4HrCount; HrIndex++) {
    Usb4HrInfo = &(Usb4PlatformInfo->Usb4Hr[HrIndex]);

    if (Usb4HrInfo->IntegratedHr) {
      continue;
    } else {
      Sbdf.Seg  = (UINT32) 0;
      Sbdf.Bus  = (UINT32) Usb4HrInfo->Rp.Bdf.Bus;
      Sbdf.Dev  = (UINT32) Usb4HrInfo->Rp.Bdf.Dev;
      Sbdf.Func = (UINT32) Usb4HrInfo->Rp.Bdf.Func;

      DEBUG ((
        DEBUG_INFO,
        "HR[%d] USB4 DTBT Root Port (SBDF) = (%02x/%02x/%02x/%02x)\n",
        HrIndex,
        Sbdf.Seg,
        Sbdf.Bus,
        Sbdf.Dev,
        Sbdf.Func
        ));

      FindBrHierarchyConfiguration (Sbdf);
    }
  }

Exit:
  DEBUG((DEBUG_INFO, "%a() - end\n", __FUNCTION__));
}