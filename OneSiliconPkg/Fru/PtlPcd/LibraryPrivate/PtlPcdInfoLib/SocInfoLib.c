/** @file
  SoC private information library.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Library/SocInfoLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchLimits.h>
#include <Defines/PchReservedResources.h>

/**
  Check if this is Server PCH

  @retval TRUE                It's a Server PCH
  @retval FALSE               It's not a Server PCH
**/
BOOLEAN
IsPchServer (
  VOID
  )
{
  return FALSE;
}

/**
  Get Pch Maximum Pcie ClockReq Number

  @retval Pch Maximum Pcie ClockReq Number
**/
UINT8
GetPchMaxPcieClockReqNum (
  VOID
  )
{
  return GetPchMaxPcieClockNum ();
}

/**
  Get Pch Maximum Serial IO SPI Chip Selects count

  @retval Pch Maximum Serial IO SPI Chip Selects number
**/
UINT8
GetPchMaxSerialIoSpiChipSelectsNum (
  VOID
  )
{
  return PCH_MAX_SERIALIO_SPI_CHIP_SELECTS;
}

/**
  Get Pch Maximum ISH SPI Controller Cs pins number

  @retval Pch Maximum ISH SPI controller Cs pins number
**/
UINT8
GetPchMaxIshSpiControllerCsPinsNum (
  VOID
  )
{
  return PCH_MAX_ISH_SPI_CS_PINS;
}

/**
  Get Pch Maximum Type C Port Number

  @retval Pch Maximum Type C Port Number
**/
UINT8
GetPchMaxTypeCPortNum (
  VOID
  )
{
  return 4;
}

/**
  Check if this chipset supports eMMC controller

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchEmmcSupported (
  VOID
  )
{
  return FALSE;
}

/**
  Check if this chipset supports SD controller

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchSdCardSupported (
  VOID
  )
{
  return FALSE;
}

/**
  Check if this chipset supports HSIO BIOS Sync

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsChipsetInitSyncSupported (
  VOID
  )
{
  return TRUE;
}

/**
  Check whether ISH is supported by PCH Series.

  @retval TRUE                    ISH is supported in current PCH
  @retval FALSE                   ISH is not supported on current PCH
**/
BOOLEAN
PchIsIshSupported (
  VOID
  )
{
  return TRUE;
}

/**
  Check if link between PCH and CPU is an P-DMI

  @retval    TRUE           P-DMI link
  @retval    FALSE          Not an P-DMI link
**/
BOOLEAN
IsPchWithPdmi (
  VOID
  )
{
  return FALSE;
}

/**
  Check if link between PCH and CPU is an OP-DMI

  @retval    TRUE           OP-DMI link
  @retval    FALSE          Not an OP-DMI link
**/
BOOLEAN
IsPchWithOpdmi (
  VOID
  )
{
  return FALSE;
}

/**
  Check if link between PCH and CPU is an F-DMI

  @retval    TRUE           F-DMI link
  @retval    FALSE          Not an F-DMI link
**/
BOOLEAN
IsPchWithFdmi (
  VOID
  )
{
  return FALSE;
}

/**
  Get Pch Maximum THC count

  @retval Pch Maximum THC count number
**/
UINT8
GetPchMaxThcCount (
  VOID
  )
{
  return PCH_MAX_THC_CONTROLLERS;
}

/**
  return support status for P2SB PCR 20-bit addressing

  @retval    TRUE
  @retval    FALSE
**/
BOOLEAN
IsP2sb20bPcrSupported (
  VOID
  )
{
  return TRUE;
}

/**
  Check if this chipset supports THC controller

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchThcSupported (
  VOID
  )
{
  return TRUE;
}

/**
  Check whether integrated LAN controller is supported.

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchGbeSupported (
  VOID
  )
{
  return TRUE;
}

/**
  Check whether integrated TSN is supported.

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchTsnSupported (
  VOID
  )
{
  return FALSE;
}

/**
  Check whether ATX Shutdown (PS_ON) is supported.

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchPSOnSupported (
  VOID
  )
{
  return FALSE;
}

/**
  Check whether XHCI Controller is supported.

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchXhciSupported (
  VOID
  )
{
  return TRUE;
}

/**
  Check if given Audio Interface is supported

  @param[in] AudioLinkType   Link type support to be checked
  @param[in] AudioLinkIndex  Link number

  @retval    TRUE           Link supported
  @retval    FALSE          Link not supported
**/
BOOLEAN
IsAudioInterfaceSupported (
  IN HDAUDIO_LINK_TYPE     AudioLinkType,
  IN UINT32                AudioLinkIndex
  )
{
  //
  // Interfaces supported:
  // 1. HDA Link (SDI0/SDI1)
  // 2. Display Audio Link (SDI2)
  // 3. SSP[0-5]
  // 4. SNDW[1-4]
  // 5. SNDW Multilane[0-1]
  switch (AudioLinkType) {
    case HdaLink:
    case HdaIDispLink:
      return TRUE;
    case HdaDmic:
      if (AudioLinkIndex < PCH_MAX_HDA_DMIC_LINK_NUM) {
        return TRUE;
      } else {
        return FALSE;
      }
    case HdaSsp:
      if (AudioLinkIndex < PCH_MAX_HDA_SSP_LINK_NUM - PCH_MAX_HDA_SSP_EXTENDED_LINK_NUM) {
        return TRUE;
      } else {
        return FALSE;
      }
    case HdaSndw:
      if (AudioLinkIndex < PCH_MAX_HDA_SNDW_LINK_NUM) {
        return TRUE;
      } else {
        return FALSE;
      }
    case HdaSndwMultilane:
      if (AudioLinkIndex < PCH_MAX_HDA_SNDW_MULTILANE_NUM) {
        return TRUE;
      } else {
        return FALSE;
      }
    default:
      return FALSE;
  }
}

/**
  Initialize PCH-related info at latter stages of the boot.
**/
VOID
PchInfoLateInit (
  VOID
  )
{

}

/**
  Returns reserved Serial IO MMIO

  @retval    Mmio
**/
UINTN
GetReservedSerialIoMmio (
  VOID
  )
{
  return PCH_SERIAL_IO_BASE_ADDRESS;
}
