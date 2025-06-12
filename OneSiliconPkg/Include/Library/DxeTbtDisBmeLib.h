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
#ifndef _TBT_SECURITY_LIB_H_
#define _TBT_SECURITY_LIB_H_

#include <Uefi.h>
#include <PcieRegs.h>
#include <PeiITbtGenericStructure.h>
#include <IndustryStandard/Pci.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/PcieHelperLib.h>

typedef struct {
  UINT32 Seg     : 8;
  UINT32 Bus     : 8;
  UINT32 Dev     : 5;
  UINT32 Func    : 3;
  UINT32 PcieCap : 8;
} SBDF;

typedef enum {
  DevTypePci,
  DevTypePcieEndpoint,
  DevTypePcieUpstream,
  DevTypePcieDownstream,
  DevTypeMax
} PCI_DEV_TYPE;

/**
  Converts device's segment:bus:device:function coordinates to flat address

  @param[in] Sbdf   device's segment:bus:device:function coordinates
  @retval    address of device's PCI cfg space
**/
UINT64
SbdfToBase (
  IN SBDF Sbdf
  );

/**
  Returns information about type of device.

  @param[IN] Sbdf            device's segment:bus:device:function coordinates
  @retval     one of: not a PCIe device (legacy PCI), PCIe endpoint, PCIe upstream port or PCIe downstream port (including rootport)
**/
PCI_DEV_TYPE
GetDeviceType (
  IN SBDF Sbdf
  );

/**
  Initializes Dev:Func numbers for use in FindNextPcieChild or FindNextLegalSbdf functions.

  @param[out] Sbdf            device's segment:bus:device:function coordinates
**/
VOID
InitChildFinder (
  OUT SBDF *Sbdf
  );

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
  );

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
BOOLEAN
IsPcieDevice (
  IN OUT SBDF *Sbdf
  );

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

  @param[in]     ParentDevType  type of bridge who's parent of this bus
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
  );

/**
  Finds next PCIe (not legacy PCI) device behind given device
  If device/function number == 0x1F/0x7, this function searches for children from scratch
  Any other device number means Dev:Func contain address of last found child device
  and this function should search for next one

  @param[in]     ParentDevType  type of bridge who's parent of this bus
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
  );

/**
  Recursive Tbt Hierarchy Configuration

  @param[in]  Sbdf       device's segment:bus:device:function coordinates
**/
VOID
RecursiveTbtHierarchyConfiguration (
  IN SBDF       Sbdf
  );

#endif
