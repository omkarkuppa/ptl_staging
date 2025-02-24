/** @file
  Information library for PantherLake SoC.

  This library is available for PEI, DXE and SMM.
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
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/PrintLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/EspiRegs.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Protocol/IntelDieInfo.h>
#include <PcdEspiDeviceIds.h>
#define PTL_SOC_UNKNOWN_SKU  0xFFFF

typedef struct {
  UINT32  Id;
  CHAR8   *Str;
} PTL_INFO_STR_TABLE;

GLOBAL_REMOVE_IF_UNREFERENCED PTL_INFO_STR_TABLE  mPtlPcdSeries[] = {
  {PTL_PCD_P, "PTL PCD P"},
  {PTL_PCD_H, "PTL PCD H"},
  {PTL_SOC_UNKNOWN_SERIES, "Unknown"}
};

//
// PTL PCD P and H Mobile LPC Device IDs
//
GLOBAL_REMOVE_IF_UNREFERENCED PTL_INFO_STR_TABLE mPtlPcdSkuStrs[] = {
  {V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_0,   "PTL P Unfused"},
  {V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_1,   "P Super SKU (SSKU)"},
  {V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_2,   "P Premium"},
  {V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_3,   "P Base/Mainstream"},
  {V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_0,   "PTL H Unfused"},
  {PTL_SOC_UNKNOWN_SKU,                  "Unknown"}
};

EFI_STATUS
PtlPcdInstallDieInfoInternal (
  IN  EFI_GUID                               Type,
  IN  UINT32                                 DieIndex,
  IN  UINT64                                 DieId,
  IN  UINT32                                 SteppingId,
  IN  EDKII_INTEL_DIE_INFO_GET_DIE_NAME_STR  GetNameStr,
  IN  EDKII_INTEL_DIE_INFO_GET_STEPPING_STR  GetSteppingStr,
  IN  EDKII_INTEL_DIE_INFO_GET_SKU_STR       GetSkuStr
  );

/**
  Return eSPI Device Id.

  @retval  PTL SoC eSPI Device ID
**/
UINT16
PtlPcdGetEspiDid (
  VOID
  )
{
  UINT64  LpcBaseAddress;

  LpcBaseAddress = LpcPciCfgBase ();

  return PciSegmentRead16 (LpcBaseAddress + PCI_DEVICE_ID_OFFSET);
}

/**
  Return SoC Series

  @retval PTL_SOC_SERIES     SoC Series
**/
PTL_SOC_SERIES
PtlPcdGetSeries (
  VOID
  )
{
  switch (PtlPcdGetEspiDid () & B_ESPI_CFG_DID) {
    case V_ESPI_CFG_DID_PTL_PCD_P:
      return PTL_PCD_P;
    case V_ESPI_CFG_DID_PTL_PCD_H:
      return PTL_PCD_H;
    default:
      return PTL_SOC_UNKNOWN_SERIES;
  }
}

/**
  Checks if SoC is supported.

  @return TRUE if SOC is supported by BIOS.
**/
BOOLEAN
PtlPcdIsSupported (
  VOID
  )
{
  UINT16         LpcDeviceId;
  UINT16         LpcVendorId;
  UINT64         LpcBaseAddress;

  LpcBaseAddress = LpcPciCfgBase ();

  LpcDeviceId = PciSegmentRead16 (LpcBaseAddress + PCI_DEVICE_ID_OFFSET);
  LpcVendorId = PciSegmentRead16 (LpcBaseAddress + PCI_VENDOR_ID_OFFSET);

  ///
  /// Verify that this is a supported chipset
  ///
  if ((LpcVendorId == V_PCH_INTEL_VENDOR_ID) && (PtlPcdGetSeries () != PTL_SOC_UNKNOWN_SERIES)) {
    return TRUE;
  } else {
    DEBUG ((DEBUG_WARN, "BIOS doesn't support the LpcDeviceId: 0x%04x!\n", LpcDeviceId));
    return FALSE;
  }
}

/**
  Get PTL SoC series string.

  @return Pointer to NULL terminated string with PTL SoC series name.
**/
CHAR8*
PtlPcdGetSeriesStr (
  VOID
  )
{
  UINTN           Index;
  PTL_SOC_SERIES  SocSeries;

  SocSeries = PtlPcdGetSeries ();
  for (Index = 0; Index < ARRAY_SIZE (mPtlPcdSeries); Index++) {
    if (SocSeries == mPtlPcdSeries[Index].Id) {
      return mPtlPcdSeries[Index].Str;
    }
  }

  return "Unknown";
}

/**
  Get PTL SoC SKU str.

  @retval Static ASCII string of PTL SoC SKU
**/
CHAR8*
PtlPcdGetSkuStr (
  VOID
  )
{
  UINTN          Index;
  UINT16         LpcDid;

  LpcDid = PtlPcdGetEspiDid ();

  for (Index = 0; Index < ARRAY_SIZE (mPtlPcdSkuStrs); Index++) {
    if (LpcDid == mPtlPcdSkuStrs[Index].Id) {
      return mPtlPcdSkuStrs[Index].Str;
    }
  }

  return "Undefined SKU";
}

/**
  Checks if PTL PCD is of P series.

  @return TRUE if PTL SoC is of P series.
**/
BOOLEAN
PtlIsPcdP (
  VOID
  )
{
  return (PtlPcdGetSeries () == PTL_PCD_P);
}

/**
  Checks if PTL PCD is of H series.

  @return TRUE if PTL SoC is of H series.
**/
BOOLEAN
PtlIsPcdH (
  VOID
  )
{
  return (PtlPcdGetSeries () == PTL_PCD_H);
}

/**
  Checks if PCD is of WCL PCD series.

  @return TRUE if SoC is of WCL PCD series.
**/
BOOLEAN
IsWclPcd (
  VOID
  )
{
  return (PtlPcdGetSeries () == WCL_PCD);
}


/**
  Return SoC stepping type

  @retval PCH_STEPPING   SoC stepping type
**/
PTL_SOC_STEPPING
PtlPcdGetStepping (
  VOID
  )
{
  return PciSegmentRead8 (LpcPciCfgBase () + PCI_REVISION_ID_OFFSET);
}

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
  )
{
  PTL_SOC_STEPPING  SocStepping;

  if ((Buffer == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }
  if (BufferSize < PTL_SOC_INFO_STEPPING_STR_MIN_LENGTH) {
    return EFI_BUFFER_TOO_SMALL;
  }

  SocStepping = PtlPcdGetStepping ();
  AsciiSPrint (Buffer, BufferSize, "%c%c", 'A' + (SocStepping >> 4), '0' + (SocStepping & 0xF));
  return EFI_SUCCESS;
}

/**
  Get number of PCIe root ports on PTL SOC

  @retval Number of root ports on the SoC
**/
UINT8
GetMaxPciePortNum (
  VOID
  )
{
  UINT8  NumOfRootPorts;

  if (PtlIsPcdP ()) {
    NumOfRootPorts = 10;
  } else {
    NumOfRootPorts = 12;
  }
  return NumOfRootPorts;
}

/**
  Get number of PCIe controller on PTL SOC.

  @return Number of controllers on the SoC.
**/
UINT8
GetMaxPcieControllerNum (
  VOID
  )
{
  UINT8  NumOfControllers;

  if (PtlIsPcdP ()) {
    NumOfControllers = 3;
  } else {
    NumOfControllers = 5;
  }
  return NumOfControllers;
}

/**
  This function returns pointer to the const SoC name string.

  @retval Pointer to name string
**/
CONST
CHAR8*
EFIAPI
PtlPcdDieGetNameStr (
  IN EDKII_INTEL_DIE_INFO_PROTOCOL  *This
  )
{
  if (This->DieIndex == 1) {
    return PtlPcdGetSeriesStr ();
  } else {
    return NULL;
  }
}

/**
  This function returns pointer to the const SoC stepping string.

  @retval Pointer to name string
**/
CONST
CHAR8*
EFIAPI
PtlPcdDieGetSteppingStr (
  IN EDKII_INTEL_DIE_INFO_PROTOCOL  *This
  )
{
  if (This->DieIndex == 1) {
    return "A0";
  } else {
    return NULL;
  }
}

/**
  This function returns pointer to the const SoC SKU string.

  @retval Pointer to name string
**/
CONST
CHAR8*
EFIAPI
PtlPcdDieGetSkuStr (
  IN EDKII_INTEL_DIE_INFO_PROTOCOL  *This
  )
{
  if (This->DieIndex == 1) {
    return PtlPcdGetSkuStr ();
  } else {
    return NULL;
  }
}

/**
  This function installs DieInfo instance for SoC die.

  @retval EFI_SUCCESS if instance has been installed successfully.
**/
EFI_STATUS
PtlPcdInstallDieInfo (
  VOID
  )
{
  EFI_GUID  Type;

  CopyGuid (&Type, &gIntelDieInfoSocGuid);

  return PtlPcdInstallDieInfoInternal (
           Type,
           1,
           PtlPcdGetEspiDid (),
           PtlPcdGetStepping (),
           PtlPcdDieGetNameStr,
           PtlPcdDieGetSteppingStr,
           PtlPcdDieGetSkuStr
           );
}

/**
  Get Usb Maximum Physical Port Number

  @retval Usb Maximum Physical Port Number
**/
UINT8
PtlPcdGetUsb2MaxPhysicalPortNum (
  VOID
  )
{
  switch (PtlPcdGetSeries ()) {
    case PTL_PCD_P:
    case PTL_PCD_H:
      return 8;
    default:
      return 0;
  }
}

/**
  Get Maximum Usb2 Port Number of XHCI Controller

  @retval Maximum Usb2 Port Number of XHCI Controller
**/
UINT8
PtlPcdGetXhciMaxUsb2PortNum (
  VOID
  )
{
  return 10;
}

/**
  Get Maximum Usb3 Port Number of XHCI Controller

  @retval Maximum Usb3 Port Number of XHCI Controller
**/
UINT8
PtlPcdGetXhciMaxUsb3PortNum (
  VOID
  )
{
  return 2;
}

/**
  Get USB-R Port Number for KVM session

  @retval USB-R Port Number for KVM session
**/
UINT8
PtlPcdGetUsbrKvmPortNum (
  VOID
  )
{
  return PtlPcdGetUsb2MaxPhysicalPortNum ();
}

/**
  Get USB-R Port Number for Storage session

  @retval USB-R Port Number for Storage session
**/
UINT8
PtlPcdGetUsbrStoragePortNum (
  VOID
  )
{
  return PtlPcdGetUsb2MaxPhysicalPortNum () + 1;
}

/**
  Get CAN controller count

  @retval Pch CAN count
**/
UINT8
PtlPcdGetCanCtrlNum (
  VOID
  )
{
  return 0;
}

/**
  Get maximum TSN controllers number

  @retval Maximum TSN controllers number
**/
UINT8
PtlPcdGetTsnMaxCtrlNum (
  VOID
  )
{
  return 0;
}
