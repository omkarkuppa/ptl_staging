/** @file
  Header file for PtlPcdInfoLib.

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
#ifndef _PTL_SOC_INFO_LIB_H_
#define _PTL_SOC_INFO_LIB_H_

#include <Uefi/UefiBaseType.h>

typedef UINT8 PTL_SOC_SERIES;
typedef UINT8 PTL_SOC_STEPPING;

#define PTL_PCD_P  0x0
#define PTL_PCD_H  0x1
#define PTL_SOC_UNKNOWN_SERIES  0xFF

#define PTL_SOC_A0  0x0
#define PTL_SOC_UNKOWN_STEPPING  0xFF

#define PTL_SOC_INFO_STEPPING_STR_MIN_LENGTH  3

/**
  Checks if SoC is supported.

  @return TRUE if SOC is supported by BIOS.
**/
BOOLEAN
PtlPcdIsSupported (
  VOID
  );

/**
  Return eSPI Device Id.

  @retval  PTL SoC eSPI Device ID
**/
UINT16
PtlPcdGetEspiDid (
  VOID
  );

/**
  Return SoC Series

  @retval PCH_SERIES     SoC Series
**/
PTL_SOC_SERIES
PtlPcdGetSeries (
  VOID
  );

/**
  Get PTL SoC series string.

  @return Pointer to NULL terminated string with PTL SoC series name.
**/
CHAR8*
PtlPcdGetSeriesStr (
  VOID
  );

/**
  Get PTL SoC SKU str.

  @retval Static ASCII string of PTL SoC SKU
**/
CHAR8*
PtlPcdGetSkuStr (
  VOID
  );

/**
  Checks if PTL PCD is of P series.

  @return TRUE if PTL SoC is of P series.
**/
BOOLEAN
PtlIsPcdP (
  VOID
  );



/**
  Checks if PTL PCD is of H series.

  @return TRUE if PTL SoC is of H series.
**/
BOOLEAN
PtlIsPcdH (
  VOID
  );

/**
  Return SoC stepping type

  @retval PCH_STEPPING   SoC stepping type
**/
PTL_SOC_STEPPING
PtlPcdGetStepping (
  VOID
  );

/**
  Print PTL SoC stepping string into buffer.

  @param [out]     Buffer               Output buffer of string
  @param [in]      BufferSize           Buffer size.
                                        Must not be less than PTL_SOC_INFO_STEPPING_STR_MIN_LENGTH

  @retval EFI_SUCCESS                   String copied successfully
  @retval EFI_INVALID_PARAMETER         The stepping is not supported, or parameters are NULL
  @retval EFI_BUFFER_TOO_SMALL          Input buffer size is too small
**/
EFI_STATUS
PtlPcdGetSteppingStr (
  OUT CHAR8  *Buffer,
  IN UINT32  BufferSize
  );

/**
  Get number of PCIe root ports on PTL SOC

  @retval Number of root ports on the SoC
**/
UINT8
GetMaxPciePortNum (
  VOID
  );

/**
  Get number of PCIe controller on PTL SOC.

  @return Number of controllers on the SoC.
**/
UINT8
GetMaxPcieControllerNum (
  VOID
  );

/**
  This function installs DieInfo instance for SoC die.

  @retval EFI_SUCCESS if instance has been installed successfully.
**/
EFI_STATUS
PtlPcdInstallDieInfo (
  VOID
  );

/**
  Get Usb Maximum Physical Port Number

  @retval Usb Maximum Physical Port Number
**/
UINT8
PtlPcdGetUsb2MaxPhysicalPortNum (
  VOID
  );

/**
  Get Maximum Usb2 Port Number of XHCI Controller

  @retval Maximum Usb2 Port Number of XHCI Controller
**/
UINT8
PtlPcdGetXhciMaxUsb2PortNum (
  VOID
  );

/**
  Get Maximum Usb3 Port Number of XHCI Controller

  @retval Maximum Usb3 Port Number of XHCI Controller
**/
UINT8
PtlPcdGetXhciMaxUsb3PortNum (
  VOID
  );

/**
  Get USB-R Port Number for KVM session

  @retval USB-R Port Number for KVM session
**/
UINT8
PtlPcdGetUsbrKvmPortNum (
  VOID
  );

/**
  Get USB-R Port Number for Storage session

  @retval USB-R Port Number for Storage session
**/
UINT8
PtlPcdGetUsbrStoragePortNum (
  VOID
  );

/**
  Get CAN controller count

  @retval Pch CAN count
**/
UINT8
PtlPcdGetCanCtrlNum (
  VOID
  );

/**
  Get Maximum TSN controllers number

  @retval Maximum TSN controllers number
**/
UINT8
PtlPcdGetTsnMaxCtrlNum (
  VOID
  );

#endif
