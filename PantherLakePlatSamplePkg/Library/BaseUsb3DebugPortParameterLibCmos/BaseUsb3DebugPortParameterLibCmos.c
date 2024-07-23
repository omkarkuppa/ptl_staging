/** @file
  USB debug Port Parameter library instance based on PCD.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/CmosAccessLib.h>
#include <Library/PcdLib.h>
#include <Library/Usb3DebugPortParameterLib.h>

/**
  Returns the USB3 debug port controller.
  bit:  0~ 7: Function
  bit:  8~15: Device
  bit: 16~24: Bus

  @return  Controller information of USB debug port.

**/
UINT32
EFIAPI
GetUsb3DebugPortController (
  VOID
  )
{
  USB3_DEBUG_PORT_CONTROLLER       UsbDebugPort;

  UsbDebugPort.Controller = 0;
  UsbDebugPort.PciAddress.Bus = CmosRead8 (PcdGet8 (PcdUsb3DebugPortBusIndex));
  UsbDebugPort.PciAddress.Device = CmosRead8 (PcdGet8 (PcdUsb3DebugPortDeviceIndex));
  UsbDebugPort.PciAddress.Function = CmosRead8 (PcdGet8 (PcdUsb3DebugPortFunctionIndex));
  if (UsbDebugPort.Controller == 0) {
    // Just in case CMOS cleared
    UsbDebugPort.PciAddress.Bus = PcdGet8(PcdUsbSerialXhciBus);
    UsbDebugPort.PciAddress.Device = PcdGet8(PcdUsbSerialXhciDev);
    UsbDebugPort.PciAddress.Function = PcdGet8(PcdUsbSerialXhciFunc);
    CmosWrite8 (PcdGet8 (PcdUsb3DebugPortBusIndex), UsbDebugPort.PciAddress.Bus);
    CmosWrite8 (PcdGet8 (PcdUsb3DebugPortDeviceIndex), UsbDebugPort.PciAddress.Device);
    CmosWrite8 (PcdGet8 (PcdUsb3DebugPortFunctionIndex), UsbDebugPort.PciAddress.Function);
  }

  return UsbDebugPort.Controller;
}

/**
  Sets the USB3 debug port controller.
  bit:  0~ 7: Function
  bit:  8~15: Device
  bit: 16~24: Bus

  @param[in]    Controller information of USB debug port value to be set.

  @retval  TRUE           The controller of USB debug port was sucessfully set.
  @retval  FALSE          The controller of USB debug port could not be set.

**/
BOOLEAN
EFIAPI
SetUsb3DebugPortController (
  UINT32        Controller
  )
{
  USB3_DEBUG_PORT_CONTROLLER       UsbDebugPort;

  UsbDebugPort.Controller = Controller;
  CmosWrite8 (PcdGet8 (PcdUsb3DebugPortBusIndex), UsbDebugPort.PciAddress.Bus);
  CmosWrite8 (PcdGet8 (PcdUsb3DebugPortDeviceIndex), UsbDebugPort.PciAddress.Device);
  CmosWrite8 (PcdGet8 (PcdUsb3DebugPortFunctionIndex), UsbDebugPort.PciAddress.Function);
  return TRUE;
}
