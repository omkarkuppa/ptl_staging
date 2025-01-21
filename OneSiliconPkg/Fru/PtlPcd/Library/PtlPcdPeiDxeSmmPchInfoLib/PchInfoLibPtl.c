/** @file
  Soc information library for PTL.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <PchPcieRpInfo.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PchInfoLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PrintLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PcdInfoLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPcieRpLib.h>
#include <Register/PchRegs.h>
#include <Register/EspiRegs.h>
#include <IndustryStandard/Pci30.h>
#include "PchInfoLibPrivate.h"

/**
  Return LPC Device Id

  @retval PCH_LPC_DEVICE_ID         PCH Lpc Device ID
**/
UINT16
PchGetLpcDid (
  VOID
  )
{
  return PtlPcdGetEspiDid ();
}

/**
  Return Pch Series

  @retval PCH_SERIES            Pch Series
**/
PCH_SERIES
PchSeries (
  VOID
  )
{
  PTL_SOC_SERIES  SocSeries;

  SocSeries = PtlPcdGetSeries ();
  if (SocSeries == PTL_PCD_P) {
    return PCH_P;
  } else if (SocSeries == PTL_PCD_H) {
    return PCH_H;
  }

  return PCH_UNKNOWN_SERIES;
}

/**
  Return Pch stepping type

  @retval PCH_STEPPING            Pch stepping type
**/
PCH_STEPPING
PchStepping (
  VOID
  )
{
  return (PCH_STEPPING) PtlPcdGetStepping ();
}

/**
  Determine if PCH is supported

  @retval TRUE                    PCH is supported
  @retval FALSE                   PCH is not supported
**/
BOOLEAN
IsPchSupported (
  VOID
  )
{
  return PtlPcdIsSupported ();
}

/**
  Check if this is PCH P series

  @retval TRUE                It's PCH P series
  @retval FALSE               It's not PCH P series
**/
BOOLEAN
IsPchP (
  VOID
  )
{
  return (PchSeries () == PCH_P);
}

/**
  Check if this is PCH M series

  @retval TRUE                It's PCH M series
  @retval FALSE               It's not PCH M series
**/
BOOLEAN
IsPchM (
  VOID
  )
{
  return FALSE;
}

/**
  Check if this is PCH S series

  @retval TRUE                It's PCH S series
  @retval FALSE               It's not PCH S series
**/
BOOLEAN
IsPchS (
  VOID
  )
{
  return (PchSeries () == PCH_S);
}

/**
  Check if this is ADL PCH generation

  @retval TRUE                It's ADL PCH
  @retval FALSE               It's not ADL PCH
**/
BOOLEAN
IsAdlPch (
  VOID
  )
{
  return (PchGeneration () == ADL_PCH);
}

/**
  Check if this is RPL PCH generation

  @retval TRUE                It's RPL PCH
  @retval FALSE               It's not RPL PCH
**/
BOOLEAN
IsRplPch (
  VOID
  )
{
  return FALSE;
}

/**
  Check if this is PTL SOC generation

  @retval TRUE                It's PTL SOC
  @retval FALSE               It's not PTL SOC
**/
BOOLEAN
IsPtlPcd (
  VOID
  )
{
  return (PchGeneration () == PTL_SOC);
}

/**
  Check if this is MTL SOC generation

  @retval TRUE                It's MTL SOC
  @retval FALSE               It's not MTL SOC
**/
BOOLEAN
IsMtlSoc (
  VOID
  )
{
  return (PchGeneration () == MTL_SOC);
}

/**
  Check if this is S3m2.0 IBL generation

  @retval TRUE                It's S3m 2.0 IBL
  @retval FALSE               It's not S3m 2.0 IBL
**/
BOOLEAN
IsS3m2Ibl (
  VOID
  )
{
  return (PchGeneration () == S3M2_IBL);
}

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
  )
{
  return PtlPcdGetSteppingStr (Buffer, BufferSize);
}

/**
  Get PCH Sku ASCII string
  The return string is zero terminated.

  @retval Static ASCII string of PCH Sku
**/
CHAR8*
PchGetSkuStr (
  VOID
  )
{
  return PtlPcdGetSkuStr ();
}

/**
  Get Pch Maximum Pcie Controller Number

  @retval Pch Maximum Pcie Root Port Number
**/
UINT8
GetPchMaxPcieControllerNum (
  VOID
  )
{
  return GetMaxPcieControllerNum ();
}

/**
  Return Pch Generation

  @retval PCH_GENERATION      Pch Generation
**/
PCH_GENERATION
PchGeneration (
  VOID
  )
{
  return PTL_SOC;
}

/**
  Check if this is PCH H series

  @retval TRUE                It's PCH H series
  @retval FALSE               It's not PCH H series
**/
BOOLEAN
IsPchH (
  VOID
  )
{
  return (PchSeries () == PCH_H);
}

/**
  Check if this is PCH N series

  @retval TRUE                It's PCH N series
  @retval FALSE               It's not PCH N series
**/
BOOLEAN
IsPchN (
  VOID
  )
{
  return (PchSeries () == PCH_N);
}

/**
  Check if this is PCH LP series

  @retval TRUE     It's PCH LP series
  @retval FALSE    It's not PCH LP series
**/
BOOLEAN
IsPchLp (
  VOID
  )
{
  return FALSE;
}

/**
  Get PCH series ASCII string.

  @retval PCH Series string
**/
CHAR8*
PchGetSeriesStr (
  VOID
  )
{
  return PtlPcdGetSeriesStr ();
}

/**
  Get Pch Maximum Pcie Clock Number

  @retval Pch Maximum Pcie Clock Number
**/
UINT8
GetPchMaxPcieClockNum (
  VOID
  )
{
  if (PtlIsPcdP()) {
    return 7;
  } else {
    return 9;
  }
}

/**
  Get Pch Maximum Pcie Root Port Number

  @retval Pch Maximum Pcie Root Port Number
**/
UINT8
GetPchMaxPciePortNum (
  VOID
  )
{
  return GetMaxPciePortNum ();
}

/**
  Translate RpIndex to Controller index.
  @param[in] RpIndex  Root port index
  @return ControllerIndex
  @retval 0xFF Failed to lookup controller index.
**/
UINT32
RpIndexToControllerIndex (
  IN UINTN  RpIndex
  )
{
  return PtlPcdRpIndexToControllerIndex (RpIndex);
}

/**
  Get Pch Maximum Hda Dmic Link

  @retval Pch Maximum Hda Dmic Link
**/
UINT8
GetPchHdaMaxDmicLinkNum (
  VOID
  )
{
  return PCH_MAX_HDA_DMIC_LINK_NUM;
}

/**
  Get Pch Maximum Hda Sndw Link

  @retval Pch Maximum Hda Sndw Link
**/
UINT8
GetPchHdaMaxSndwLinkNum (
  VOID
  )
{
  return PCH_MAX_HDA_SNDW_LINK_NUM;
}

/**
  Get Pch Maximum Hda Sndw Multilane

  @retval Pch Maximum Hda Sndw Multilane Number
**/
UINT8
GetPchHdaMaxSndwMultilaneNum (
  VOID
  )
{
  return PCH_MAX_HDA_SNDW_MULTILANE_NUM;
}

/**
  Get Pch Maximum Hda Ssp Link

  @retval Pch Maximum Hda Ssp Link
**/
UINT8
GetPchHdaMaxSspLinkNum (
  VOID
  )
{
  return PCH_MAX_HDA_SSP_LINK_NUM;
}

/**
  Get Pch Usb2 Maximum Physical Port Number

  @retval Pch Usb2 Maximum Physical Port Number
**/
UINT8
GetPchUsb2MaxPhysicalPortNum (
  VOID
  )
{
  return PtlPcdGetUsb2MaxPhysicalPortNum ();
}

/**
  Get Pch Maximum Usb2 Port Number of XHCI Controller

  @retval Pch Maximum Usb2 Port Number of XHCI Controller
**/
UINT8
GetPchXhciMaxUsb2PortNum (
  VOID
  )
{
  return PtlPcdGetXhciMaxUsb2PortNum ();
}

/**
  Get Pch Maximum Usb3 Port Number of XHCI Controller

  @retval Pch Maximum Usb3 Port Number of XHCI Controller
**/
UINT8
GetPchXhciMaxUsb3PortNum (
  VOID
  )
{
  return PtlPcdGetXhciMaxUsb3PortNum ();
}

/**
  Gets the maximum number of UFS controller supported by this chipset.

  @return Number of supported UFS controllers
**/
UINT8
PchGetMaxUfsNum (
  VOID
  )
{
  return 1;
}

/**
  Get Pch Maximum ISH UART Controller number

  @retval Pch Maximum ISH UART controllers number
**/
UINT8
GetPchMaxIshUartControllersNum (
  VOID
  )
{
  return PCH_MAX_ISH_UART_CONTROLLERS;
}

/**
  Get Pch Maximum ISH I2C Controller number

  @retval Pch Maximum ISH I2C controllers number
**/
UINT8
GetPchMaxIshI2cControllersNum (
  VOID
  )
{
  return PCH_MAX_ISH_I2C_CONTROLLERS;
}

/**
  Get Pch Maximum ISH I3C Controller number

  @retval Pch Maximum ISH I3C controllers number
**/
UINT8
GetPchMaxIshI3cControllersNum (
  VOID
  )
{
  return PCH_MAX_ISH_I3C_CONTROLLERS;
}

/**
  Get Maximum LPSS I3C interfaces number

  @retval Pch Maximum LPSS I3C interfaces number
**/
UINT8
GetMaxI3cInterfacesNum (
  VOID
  )
{
  return PCH_MAX_SERIALIO_I3C_CONTROLLERS;
}

/**
  Get Pch Maximum ISH SPI Controller number

  @retval Pch Maximum ISH SPI controllers number
**/
UINT8
GetPchMaxIshSpiControllersNum (
  VOID
  )
{
  return PCH_MAX_ISH_SPI_CONTROLLERS;
}

/**
  Get Pch Maximum ISH GP number

  @retval Pch Maximum ISH GP number
**/
UINT8
GetPchMaxIshGpNum (
  VOID
  )
{
  return PCH_MAX_ISH_GP_PINS;
}

/**
  Get Pch Maximum LPSS I2C controllers number

  @retval Pch Maximum LPSS I2C controllers number
**/
UINT8
GetMaxI2cInterfacesNum (
  VOID
  )
{
  return 6;
}

/**
  Get Pch Maximum LPSS SPI controllers number

  @retval Pch Maximum LPSS SPI controllers number
**/
UINT8
GetMaxSpiInterfacesNum (
  VOID
  )
{
  return 3;
}

/**
  Get Pch Maximum LPSS UART controllers number

  @retval Pch Maximum LPSS UART controllers number
**/
UINT8
GetMaxUartInterfacesNum (
  VOID
  )
{
  return 3;
}

/**
  Get Pch Maximum THC controllers number
  @retval Pch Maximum THC controllers number
**/
UINT8
GetPchMaxThcControllersNum (
  VOID
  )
{
  return 2;
}