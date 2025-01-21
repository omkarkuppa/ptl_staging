/** @file
  Header file for PchInfoLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _PCH_INFO_LIB_H_
#define _PCH_INFO_LIB_H_

#include <Hda.h>
#include <Uefi/UefiBaseType.h>
#include "PchInfoDefs.h"

typedef UINT8 PCH_STEPPING;
typedef UINT8 PCH_SERIES;
typedef UINT8 PCH_GENERATION;

/**
  Return LPC Device Id

  @retval PCH_LPC_DEVICE_ID         PCH Lpc Device ID
**/
UINT16
PchGetLpcDid (
  VOID
  );

/**
  Return Pch stepping type

  @retval PCH_STEPPING            Pch stepping type
**/
PCH_STEPPING
PchStepping (
  VOID
  );

/**
  Determine if PCH is supported

  @retval TRUE                    PCH is supported
  @retval FALSE                   PCH is not supported
**/
BOOLEAN
IsPchSupported (
  VOID
  );

/**
  Return Pch Series

  @retval PCH_SERIES                Pch Series
**/
PCH_SERIES
PchSeries (
  VOID
  );

/**
  Check if this is PCH LP series

  @retval TRUE                It's PCH LP series
  @retval FALSE               It's not PCH LP series
**/
BOOLEAN
IsPchLp (
  VOID
  );

/**
  Check if this is PCH H series

  @retval TRUE                It's PCH H series
  @retval FALSE               It's not PCH H series
**/
BOOLEAN
IsPchH (
  VOID
  );

/**
  Check if this is PCH N series

  @retval TRUE                It's PCH N series
  @retval FALSE               It's not PCH N series
**/
BOOLEAN
IsPchN (
  VOID
  );

/**
  Check if this is PCH P series

  @retval TRUE                It's PCH P series
  @retval FALSE               It's not PCH P series
**/
BOOLEAN
IsPchP (
  VOID
  );

/**
  Check if this is PCH M series

  @retval TRUE                It's PCH M series
  @retval FALSE               It's not PCH M series
**/
BOOLEAN
IsPchM (
  VOID
  );

/**
  Check if this is PCH S series

  @retval TRUE                It's PCH S series
  @retval FALSE               It's not PCH S series
**/
BOOLEAN
IsPchS (
  VOID
  );

/**
  Return Pch Generation

  @retval PCH_GENERATION            Pch Generation
**/
PCH_GENERATION
PchGeneration (
  VOID
  );

/**
  Check if this is ADL PCH generation

  @retval TRUE                It's ADL PCH
  @retval FALSE               It's not ADL PCH
**/
BOOLEAN
IsAdlPch (
  VOID
  );

/**
  Check if this is MTL SOC generation

  @retval TRUE                It's MTL SOC
  @retval FALSE               It's not MTL SOC
**/
BOOLEAN
IsMtlSoc (
  VOID
  );

/**
  Check if this is PTL SOC generation

  @retval TRUE                It's PTL SOC
  @retval FALSE               It's not PTL SOC
**/
BOOLEAN
IsPtlPcd (
  );

/**
  Check if this is MTL SOC generation

  @retval TRUE                It's MTL SOC
  @retval FALSE               It's not MTL SOC
**/
BOOLEAN
IsLnlSoc (
  VOID
  );

/**
  Get Pch Maximum Pcie Root Port Number

  @retval PcieMaxRootPort         Pch Maximum Pcie Root Port Number
**/
UINT8
GetPchMaxPciePortNum (
  VOID
  );

/**
  Translate RpIndex to Controller index.

  @param[in] RpIndex  Root port index

  @return ControllerIndex
  @retval 0xFF Failed to lookup controller index.
**/
UINT32
RpIndexToControllerIndex (
  IN UINTN  RpIndex
  );

/**
  Get Pch Maximum Pcie Controller Number

  @retval Pch Maximum Pcie Controller Number
**/
UINT8
GetPchMaxPcieControllerNum (
  VOID
  );

/**
  Get Pch Maximum Pcie Clock Number

  @retval Pch Maximum Pcie Clock Number
**/
UINT8
GetPchMaxPcieClockNum (
  VOID
  );

/**
  Get Pch Usb2 Maximum Physical Port Number

  @retval Pch Usb2 Maximum Physical Port Number
**/
UINT8
GetPchUsb2MaxPhysicalPortNum (
  VOID
  );

/**
  Get Pch Maximum Usb2 Port Number of XHCI Controller

  @retval Pch Maximum Usb2 Port Number of XHCI Controller
**/
UINT8
GetPchXhciMaxUsb2PortNum (
  VOID
  );

/**
  Get Pch Maximum Usb3 Port Number of XHCI Controller

  @retval Pch Maximum Usb3 Port Number of XHCI Controller
**/
UINT8
GetPchXhciMaxUsb3PortNum (
  VOID
  );

/**
  Get Pch Maximum LPSS I2C controllers number

  @retval Pch Maximum LPSS I2C controllers number
**/
UINT8
GetMaxI2cInterfacesNum (
  VOID
  );

/**
  Get Maximum LPSS I3C interfaces number

  @retval Pch Maximum LPSS I3C interfaces number
**/
UINT8
GetMaxI3cInterfacesNum (
  VOID
  );

/**
  Get Pch Maximum LPSS SPI controllers number

  @retval Pch Maximum LPSS SPI controllers number
**/
UINT8
GetMaxSpiInterfacesNum (
  VOID
  );

/**
  Get Pch Maximum LPSS UART controllers number

  @retval Pch Maximum LPSS UART controllers number
**/
UINT8
GetMaxUartInterfacesNum (
  VOID
  );

/**
  Get Pch Maximum ISH UART Controller number

  @retval Pch Maximum ISH UART controllers number
**/
UINT8
GetPchMaxIshUartControllersNum (
  VOID
  );

/**
  Get Pch Maximum ISH I2C Controller number

  @retval Pch Maximum ISH I2C controllers number
**/
UINT8
GetPchMaxIshI2cControllersNum (
  VOID
  );

/**
  Get Pch Maximum ISH I3C Controller number

  @retval Pch Maximum ISH I3C controllers number
**/
UINT8
GetPchMaxIshI3cControllersNum (
  VOID
  );

/**
  Get Pch Maximum ISH SPI Controller number

  @retval Pch Maximum ISH SPI controllers number
**/
UINT8
GetPchMaxIshSpiControllersNum (
  VOID
  );

/**
  Get Pch Maximum ISH GP number

  @retval Pch Maximum ISH GP number
**/
UINT8
GetPchMaxIshGpNum (
  VOID
  );

#define PCH_STEPPING_STR_LENGTH_MAX 3

/**
  Get PCH stepping ASCII string.
  Function determines major and minor stepping versions and writes them into a buffer.
  The return string is zero terminated

  @param [out]     Buffer               Output buffer of string
  @param [in]      BufferSize           Buffer size.
                                        Must not be less then PCH_STEPPING_STR_LENGTH_MAX

  @retval EFI_SUCCESS                   String copied successfully
  @retval EFI_INVALID_PARAMETER         The stepping is not supported, or parameters are NULL
  @retval EFI_BUFFER_TOO_SMALL          Input buffer size is too small
**/
EFI_STATUS
PchGetSteppingStr (
  OUT    CHAR8                          *Buffer,
  IN     UINT32                         BufferSize
  );

/**
  Get PCH series ASCII string.
  The return string is zero terminated.

  @retval Static ASCII string of PCH Series
**/
CHAR8*
PchGetSeriesStr (
  );

/**
  Get PCH Sku ASCII string
  The return string is zero terminated.

  @retval Static ASCII string of PCH Sku
**/
CHAR8*
PchGetSkuStr (
  VOID
  );

/**
  Gets the maximum number of UFS controller supported by this chipset.

  @return Number of supported UFS controllers
**/
UINT8
PchGetMaxUfsNum (
  VOID
  );

/**
  Get Pch Maximum Hda Sndw Link

  @retval Pch Maximum Hda Sndw Link
**/
UINT8
GetPchHdaMaxSndwLinkNum (
  VOID
  );

/**
  Get Pch Maximum Hda Sndw Multilane

  @retval Pch Maximum Hda Sndw Multilane Numer
**/
UINT8
GetPchHdaMaxSndwMultilaneNum (
  VOID
  );

/**
  Get Pch Maximum Hda Ssp Link

  @retval Pch Maximum Hda Ssp Link
**/
UINT8
GetPchHdaMaxSspLinkNum (
  VOID
  );

/**
  Get Pch Maximum Hda Dmic Link

  @retval Pch Maximum Hda Dmic Link
**/
UINT8
GetPchHdaMaxDmicLinkNum (
  VOID
  );

/**
  Check whether integrated TSN is supported.

  @retval TRUE                    TSN is supported in current PCH
  @retval FALSE                   TSN is not supported on current PCH
**/
BOOLEAN
PchIsTsnSupported (
  VOID
  );

/**
  Get Pch Maximum THC controllers number
  @retval Pch Maximum THC controllers number
**/
UINT8
GetPchMaxThcControllersNum (
  VOID
);

#endif // _PCH_INFO_LIB_H_
