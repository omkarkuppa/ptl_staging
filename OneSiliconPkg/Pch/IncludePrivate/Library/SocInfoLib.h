/** @file
  Header file for SocInfoLib.

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
#ifndef _PCH_PRIVATE_INFO_LIB_H_
#define _PCH_PRIVATE_INFO_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <Hda.h>

/**
  Check if this is Server PCH

  @retval TRUE                It's a Server PCH
  @retval FALSE               It's not a Server PCH
**/
BOOLEAN
IsPchServer (
  VOID
  );

/**
  Get Pch Maximum Pcie ClockReq Number

  @retval Pch Maximum Pcie ClockReq Number
**/
UINT8
GetPchMaxPcieClockReqNum (
  VOID
  );

/**
  Get Pch Maximum Serial IO SPI Chip Selects count

  @retval Pch Maximum Serial IO SPI Chip Selects nu,ber
**/
UINT8
GetPchMaxSerialIoSpiChipSelectsNum (
  VOID
  );

/**
  Get Pch Maximum ISH SPI Controller Cs pins number

  @retval Pch Maximum ISH SPI controller Cs pins number
**/
UINT8
GetPchMaxIshSpiControllerCsPinsNum (
  VOID
  );

/**
  Get Pch Maximum Type C Port Number

  @retval Pch Maximum Type C Port Number
**/
UINT8
GetPchMaxTypeCPortNum (
  VOID
  );

/**
  Check if this chipset supports eMMC controller

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchEmmcSupported (
  VOID
  );

/**
  Check if this chipset supports SD controller

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchSdCardSupported (
  VOID
  );

/**
  Check if this chipset supports HSIO BIOS Sync

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsChipsetInitSyncSupported (
  VOID
  );

/**
  Check whether ISH is supported.

  @retval TRUE                    ISH is supported in PCH
  @retval FALSE                   ISH is not supported by PCH
**/
BOOLEAN
PchIsIshSupported (
  VOID
  );

/**
  Check if link between PCH and CPU is an P-DMI

  @retval    TRUE           P-DMI link
  @retval    FALSE          Not an P-DMI link
**/
BOOLEAN
IsPchWithPdmi (
  VOID
  );

/**
  Check if link between PCH and CPU is an OP-DMI

  @retval    TRUE           OP-DMI link
  @retval    FALSE          Not an OP-DMI link
**/
BOOLEAN
IsPchWithOpdmi (
  VOID
  );

/**
  Check if link between PCH and CPU is an F-DMI

  @retval    TRUE           F-DMI link
  @retval    FALSE          Not an F-DMI link
**/
BOOLEAN
IsPchWithFdmi (
  VOID
  );

/**
  Get Pch Maximum THC count

  @retval Pch Maximum THC count number
**/
UINT8
GetPchMaxThcCount (
  VOID
  );

/**
  Return support status for P2SB PCR 20-bit addressing

  @retval    TRUE
  @retval    FALSE
**/
BOOLEAN
IsP2sb20bPcrSupported (
  VOID
  );

/**
  Check if this chipset supports THC controller

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchThcSupported (
  VOID
  );

/**
  Check whether integrated LAN controller is supported.

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchGbeSupported (
  VOID
  );

/**
  Check whether integrated TSN is supported.

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchTsnSupported (
  VOID
  );

/**
  Check whether ATX Shutdown (PS_ON) is supported.

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchPSOnSupported (
  VOID
  );

/**
  Check whether XHCI Controller is supported.

  @retval BOOLEAN  TRUE if supported, FALSE otherwise
**/
BOOLEAN
IsPchXhciSupported (
  VOID
  );

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
  );

/**
  Initialize PCH related info at latter stages of the boot.
**/
VOID
PchInfoLateInit (
  VOID
  );
#endif // _PCH_PRIVATE_INFO_LIB_H_
