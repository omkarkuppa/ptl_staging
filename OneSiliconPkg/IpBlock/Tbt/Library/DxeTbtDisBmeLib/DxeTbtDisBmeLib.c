/** @file
  Disable BME on TBT Hierarchy Lib.

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
#include <Library/DxeTbtDisBmeLib.h>

/**
  Converts device's segment:bus:device:function coordinates to flat address

  @param[in] Sbdf   device's segment:bus:device:function coordinates
  @retval    address of device's PCI cfg space
**/
UINT64
SbdfToBase (
  IN SBDF Sbdf
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func, 0);
}

/**
  Returns information about type of device.

  @param[IN] Sbdf            device's segment:bus:device:function coordinates
  @retval     one of: not a PCIe device (legacy PCI), PCIe endpoint, PCIe upstream port or PCIe downstream port (including rootport)
**/
PCI_DEV_TYPE
GetDeviceType (
  IN SBDF Sbdf
  )
{
  UINT8 DeviceType;

  DeviceType = 0;

  if (Sbdf.PcieCap == 0) {
    return DevTypePci;
  }
  DeviceType = (UINT8) ((PciSegmentRead16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_XCAP_OFFSET) & B_PCIE_XCAP_DT) >> N_PCIE_XCAP_DT);
  if (DeviceType == PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT) {
    return DevTypePcieUpstream;
  } else if (DeviceType == PCIE_DEVICE_PORT_TYPE_DOWNSTREAM_PORT || DeviceType == PCIE_DEVICE_PORT_TYPE_ROOT_PORT) {
    return DevTypePcieDownstream;
  } else {
    return DevTypePcieEndpoint;
  }
}

/**
  Initializes Dev:Func numbers for use in FindNextPcieChild or FindNextLegalSbdf functions.

  @param[out] Sbdf            device's segment:bus:device:function coordinates
**/
VOID
InitChildFinder (
  OUT SBDF *Sbdf
  )
{
  //
  // Initialize Dev/Func to maximum values, so that when FindNextLegalSbdf ()
  // is called on those input parameters, it will return 1st legal address (Dev 0 Func 0).
  //
  Sbdf->Dev = PCI_MAX_DEVICE;
  Sbdf->Func = PCI_MAX_FUNC;
}

/**
  Checks the device is a bridge and has non-zero secondary bus number assigned.
  If so, it returns TRUE and initializes ChildSbdf with such values that
  allow searching for devices on the secondary bus.
  ChildSbdf will be mangled even if this function returns FALSE.

  Legal bus assignment is assumed. This function doesn't check subordinate bus numbers of
  the the device it was called on or any bridges between it and root complex

  @param[in]  Sbdf       device's segment:bus:device:function coordinates
  @param[out] ChildSbdf  SBDF initialized in such way that calling FindNextPcieChild( ) on it will find all children devices

  @retval TRUE if device is a bridge and has a bus behind it; FALSE otherwise
**/
BOOLEAN
HasChildBus (
  IN  SBDF   Sbdf,
  OUT SBDF   *ChildSbdf
  )
{
  UINT32 Data32;
  UINT64 Base;
  UINT8  SecondaryBus;

  Data32        = 0;
  Base          = 0;
  SecondaryBus  = 0;

  ChildSbdf->Seg = Sbdf.Seg;
  InitChildFinder (ChildSbdf);

  Base = SbdfToBase (Sbdf);

  if (PciSegmentRead8 (Base + R_PCI_BCC_OFFSET) != PCI_CLASS_BRIDGE) {
    DEBUG ((DEBUG_INFO, "HasChildBus %02:%02:%02:%02: no\n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return FALSE;
  }
  Data32 = PciSegmentRead32 (Base + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET);
  SecondaryBus = (UINT8)((Data32 & B_PCI_BRIDGE_BNUM_SCBN) >> 8);
  ChildSbdf->Bus = SecondaryBus;
  if (SecondaryBus == 0) {
    DEBUG ((DEBUG_INFO, "HasChildBus %02x:%02x:%02x:%02x: no\n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return FALSE;
  } else {
    DEBUG ((DEBUG_INFO, "HasChildBus %02x:%02x:%02x:%02x: yes, Secondary Bus number:0x%x\n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func, SecondaryBus));
    return TRUE;
  }
}


/**
  Checks if device at given address exists and is a PCI Express device.
  PCI express devices are distinguished from PCI by having Capability ID 0x10
  If the device is PCI express then its SDBF structure gets updated with pointer to
  the PCIe Capability. This is an optimization feature. It greatly decreases the number
  of bus accesses, since most features configured by this library depend on registers
  whose location is relative to PCIe capability.

  @param[in,out] Sbdf   on entry, segment:bus:device:function coordinates
                        on exit, PcieCap offset is updated
  @retval               TRUE when PCIe device exists; FALSE if it's not PCIe or there's no device at all
**/
STATIC
BOOLEAN
IsPcieDevice (
  IN OUT SBDF *Sbdf
  )
{
  UINT8 PcieCapOffset;
  UINT64 Base;

  PcieCapOffset = 0;
  Base = 0;

  Base = SbdfToBase (*Sbdf);

  if (PciSegmentRead16 (Base) == 0xFFFF) {
    return FALSE;
  }

  PcieCapOffset = PcieBaseFindCapId (Base, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PcieCapOffset == 0) {
    DEBUG ((DEBUG_INFO, "IsPcieDevice %02x:%02x:%02x:%02x - legacy\n", Sbdf->Seg, Sbdf->Bus, Sbdf->Dev, Sbdf->Func));
    return FALSE;
  } else {
    Sbdf->PcieCap = PcieCapOffset;
    DEBUG ((DEBUG_INFO, "IsPcieDevice %02x:%02x:%02x:%02x - yes\n", Sbdf->Seg, Sbdf->Bus, Sbdf->Dev, Sbdf->Func));
    return TRUE;
  }
}

/**
  Returns TRUE and Dev:Func numbers where a PCIe device could legally be located, or FALSE if there
  no such coordinates left.

  Segment and Bus fields of SBDF structure are input only and determine which bus will be scanned.
  This function should be called in a while() loop. It replaces the less efficient method of
  using nested FOR loops that iterate over all device and function numbers. It is optimized for
  the amount of bus access. If function0 doesn't exist or doesn't have Multifunction bit set,
  then higher function numbers are skipped. If parent of this bus is a downstream port, then
  Device numbers 1-31 get skipped too (there can be only Dev0 behind downstream ports)
  If device/function number == 0x1F/0x7, this function returns first possible address, that is 0:0
  Any other device number means Dev:Func contain address of last found child device
  and this function should search for next one

  @param[in]     ParentDevType  type of bridge who's partent of this bus
  @param[in,out] Sbdf           On entry: location returned previously from this function
                                          Dev:Func value of 1F:07 means search should start from the beginning
                                On exit:  if legal Dev:Func combination was found, that Dev:Func is returned
                                          otherwise, Dev:Func are initialized to 1F:07 for convenience
  @retval TRUE when next legal Dev:Func address was found; FALSE otherwise
**/
BOOLEAN
FindNextLegalSbdf (
  IN     PCI_DEV_TYPE ParentDevType,
  IN OUT SBDF         *Sbdf
  )
{
  UINT8  MaxDev;
  UINT64 Func0Base;

  MaxDev = 0;
  Func0Base = 0;

  if (ParentDevType == DevTypePcieEndpoint) {
    return FALSE;
  }
  if (ParentDevType == DevTypePcieUpstream) {
    MaxDev = PCI_MAX_DEVICE;
  } else {
    MaxDev = 0;
  }
  Func0Base = PCI_SEGMENT_LIB_ADDRESS (Sbdf->Seg, Sbdf->Bus, Sbdf->Dev, 0, 0);
  if ((Sbdf->Dev == PCI_MAX_DEVICE) && Sbdf->Func == PCI_MAX_FUNC) {
    Sbdf->Dev = 0;
    Sbdf->Func = 0;
    return TRUE;
  } else if ((Sbdf->Func == PCI_MAX_FUNC) || (Sbdf->Func == 0 && !IsMultifunctionDevice (Func0Base))) {
    //
    // if it's the last function of a device, then return Func0 of new device or FALSE in case there are no more devices
    //
    if (Sbdf->Dev == MaxDev) {
      InitChildFinder (Sbdf);
      return FALSE;
    }
    (Sbdf->Dev)++;
    Sbdf->Func = 0;
    return TRUE;
  } else {
    (Sbdf->Func)++;
    return TRUE;
  }
}

/**
  Finds next PCIe (not legacy PCI) device behind given device
  If device/function number == 0x1F/0x7, this function searches for children from scratch
  Any other device number means Dev:Func contain address of last found child device
  and this function should search for next one

  @param[in]     ParentDevType  type of bridge who's partent of this bus
  @param[in,out] Sbdf           On entry: location returned previously from this function
                                          Dev:Func value of 0x1F:0x07 means search should start from the beginning
                                On exit:  if PCIe device was found, its SBDF coordinates are returned
                                          otherwise, Dev:Func are initialized to 0x1F:0x07 for convenience
  @retval TRUE when next PCIe device was found; FALSE otherwise
**/
BOOLEAN
FindNextPcieChild (
  IN      PCI_DEV_TYPE  ParentDevType,
  IN OUT  SBDF          *Sbdf
  )
{
  while ( FindNextLegalSbdf (ParentDevType, Sbdf)) {
    if (IsPcieDevice (Sbdf)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Disable PCI device Bus Host bit

  @param[in]  Sbdf       device's segment:bus:device:function coordinates
**/
VOID
DisableBME (
  IN SBDF       Sbdf
  )
{
  UINT16  Command;
  UINT64  DeviceCmdRegAddress;

  DeviceCmdRegAddress = PCI_SEGMENT_LIB_ADDRESS (Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func, PCI_COMMAND_OFFSET);

  Command = PciSegmentRead16 (DeviceCmdRegAddress);
  DEBUG ((DEBUG_INFO, "Before disable BME (0x%x:0x%x:0x%x:0x%x): Command: 0x%x \n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func, Command));

  if ((Command & EFI_PCI_COMMAND_BUS_MASTER) == EFI_PCI_COMMAND_BUS_MASTER) {
    PciSegmentAnd16 (DeviceCmdRegAddress, (UINT16)~EFI_PCI_COMMAND_BUS_MASTER);
  }

  DEBUG ((DEBUG_INFO, "After disable BME (0x%x:0x%x:0x%x:0x%x): Command: 0x%x \n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func, PciSegmentRead16 (DeviceCmdRegAddress)));
}

/**
  Recursive Tbt Hierarchy Configuration

  @param[in]  Sbdf       device's segment:bus:device:function coordinates
**/
VOID
RecursiveTbtHierarchyConfiguration (
  IN SBDF       Sbdf
  )
{
  SBDF          ChildSbdf = {0,0,0,0,0};
  PCI_DEV_TYPE  DevType;
  UINT32        Data32;

  DEBUG ((DEBUG_INFO, "Recursive start from %02x:%02x:%02x:%02x\n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  DevType   = DevTypeMax;
  Data32    = PciSegmentRead32 (SbdfToBase (Sbdf));

  if ((Data32 == 0) || (Data32 == 0xFF)) {
    DEBUG ((DEBUG_INFO, "Controller doesn't exist or it's not supported, value: %d\n", Data32));
    return;
  }

  DevType = GetDeviceType (Sbdf);
  DEBUG ((DEBUG_INFO, "DevType: %d\n", DevType));

  if (DevType == DevTypePcieEndpoint) {
    DisableBME (Sbdf);
  }

  if (HasChildBus (Sbdf, &ChildSbdf) == TRUE) {
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      RecursiveTbtHierarchyConfiguration (ChildSbdf);
    }
  }

  if ((DevType != DevTypePcieEndpoint)) {
    DisableBME (Sbdf);
  }

  DEBUG ((DEBUG_INFO, "Recursive end for %02x:%02x:%02x:%02x\n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
}