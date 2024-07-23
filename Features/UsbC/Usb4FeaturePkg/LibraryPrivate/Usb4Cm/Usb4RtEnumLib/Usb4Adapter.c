/** @file
  USB4 adapter specific implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4CsLib.h>
#include <Library/Tbt3SupportLib.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4ConfigLayer.h>
#include <Library/Usb4ConfigSpace.h>
#include <Library/Usb4Capabilities.h>
#include <Library/Usb4CsIo.h>
#include <Library/Usb4RouterDefs.h>
#include <Library/Tbt3ConfigSpace.h>
#include <Library/Usb4DebugLib.h>
#include "Usb4RtDump.h"

extern EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log;

//
// Valid adapter type defined in USB4 spec.
//
const USB4_ADAPTER_TYPE mUsb4ValidAdapterTypes[] =
{
  USB4_ADP_TYPE_UNSUPPORTED,
  USB4_ADP_TYPE_LANE,
  USB4_ADP_TYPE_HOST_INTERFACE,
  USB4_ADP_TYPE_PCIE_DS,
  USB4_ADP_TYPE_PCIE_US,
  USB4_ADP_TYPE_DP_IN,
  USB4_ADP_TYPE_DP_OUT,
  USB4_ADP_TYPE_USB3_DS,
  USB4_ADP_TYPE_USB3_US
};

/**
  Check if the adapter type is valid

  @param[in] AdapterType - Adapter type to be checked

  @retval TRUE  - Adapter type is valid
  @retval FALSE - Adapter type is invalid
**/
BOOLEAN
IsAdapterTypeValid (
  IN UINT32 AdapterType
  )
{
  UINT32 Index;

  for (Index = 0; Index < sizeof(mUsb4ValidAdapterTypes)/sizeof(mUsb4ValidAdapterTypes[0]); Index++) {
    if (AdapterType == (UINT32)mUsb4ValidAdapterTypes[Index]) {
        return TRUE;
    }
  }
  return FALSE;
}

/**
  Convert adapter type to adapter type name string

  @param[in] AdapterType - Adapter Type defined in USB 4

  @retval Adapter type name string
**/
CHAR16 *
AdapterTypeName (
  USB4_ADAPTER_TYPE    AdapterType
  )
{
  switch (AdapterType) {
    case USB4_ADP_TYPE_UNSUPPORTED:
      return L"Unsupported Adapter";
    case USB4_ADP_TYPE_LANE:
      return L"Lane Adapter";
    case USB4_ADP_TYPE_HOST_INTERFACE:
      return L"Host Interface Adapter";
    case USB4_ADP_TYPE_PCIE_DS:
      return L"PCI-E Downstream Adapter";
    case USB4_ADP_TYPE_PCIE_US:
      return L"PCI-E Upstream Adapter";
    case USB4_ADP_TYPE_DP_IN:
      return L"DP IN Adapter";
    case USB4_ADP_TYPE_DP_OUT:
      return L"DP OUT Adapter";
    case USB4_ADP_TYPE_USB3_DS:
      return L"USB Downstream Adapter";
    case USB4_ADP_TYPE_USB3_US:
      return L"USB Upstream Adapter";
    default:
      return L"Invalid Adapter Type";
  }
}

/**
  Query and save the required Lane Adapter TMU capability into Adapter instance.

  @param[in] Router         - Pointer to Router instance.
  @param[in] AdpNum         - Adapter number.
  @param[in] CapReg         - DWORD index of capability in configuration space.

  @retval EFI_SUCCESS           - Query and save the Lane adapter TMU capability successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Unsupported adapter type.
  @retval Errors in config space access.
**/
EFI_STATUS
QueryLaneAdapterTmuCap (
  IN PUSB4_ROUTER      Router,
  IN UINT8             AdpNum,
  IN UINT16            CapReg
  )
{
  EFI_STATUS          Status;
  USB4_ADP_CAP_TMU    TmuCap;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "QueryLaneAdapterTmuCap: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "QueryLaneAdapterTmuCap: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Router->Adapter[AdpNum].Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "QueryLaneAdapterTmuCap: Invalid adapter type 0x%08X\n", Router->Adapter[AdpNum].Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query Lane adapter TMU Capability
  //
  DEBUG ((CM_VERBOSE, "    (%016llX, %d) Query Lane TMU Cap at offset 0x%0x\n", Router->TopologyId, AdpNum, CapReg));
  Status = Usb4CsAdpRead (Router, AdpNum, CapReg, sizeofdw (USB4_ADP_CAP_TMU), (UINT32 *)&TmuCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "(%016llX, %d) Query Lane TMU Cap err at 0x%0x, %r\n", Router->TopologyId, AdpNum, CapReg, Status));
    goto Exit;
  }

  DumpLaneAdapterTmuCap (&TmuCap);
Exit:
  return Status;
}

/**
  Query and save the required USB4 Port capability into Adapter instance.

  @param[in] Router         - Pointer to Router instance.
  @param[in] AdpNum         - Adapter number.
  @param[in] CapReg         - DWORD index of capability in configuration space.

  @retval EFI_SUCCESS           - Query and save the USB4 Port capability success
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Unsupported adapter type.
  @retval Errors in config space access.
**/
EFI_STATUS
QueryUsb4PortCap (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN UINT16          CapReg
  )
{
  EFI_STATUS    Status;
  UINT32        Usb4PortCap[2];

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "QueryUsb4PortCap: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "QueryUsb4PortCap: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Router->Adapter[AdpNum].Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "QueryUsb4PortCap: Invalid adapter type 0x%08X\n", Router->Adapter[AdpNum].Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query DWORD 18/19 of USB4 Port Capability
  //
  DEBUG ((CM_VERBOSE, "    (%016llX, %d) Query USB4 Port Cap for adapter at offset 0x%0x\n", Router->TopologyId, AdpNum, CapReg));
  Status = Usb4CsAdpRead (Router, AdpNum, CapReg + USB4_CS_18, 2, Usb4PortCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "(%016llX, %d) Query USB4 Port Cap err at 0x%0x, %r\n", Router->TopologyId, AdpNum, CapReg, Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "        USB4 Port Cap CS18 = 0x%08X\n", Usb4PortCap[0]));
  DEBUG ((DEBUG_INFO, "        USB4 Port Cap CS19 = 0x%08X\n", Usb4PortCap[1]));
Exit:
  return Status;
}

/**
  Query and save the required Lane Adapter capability into Adapter instance.

  @param[in] Router         - Pointer to Router instance.
  @param[in] AdpNum         - Adapter number.
  @param[in] CapReg         - DWORD index of capability in configuration space.

  @retval EFI_SUCCESS           - Query and save the adapter capability success
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Unsupported adapter type.
  @retval Errors in config space access.
**/
EFI_STATUS
QueryLaneAdapterCap (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN UINT16          CapReg
  )
{
  EFI_STATUS           Status;
  PUSB4_ADAPTER        LaneAdp;
  USB4_ADP_CAP_LANE    LaneAdpCap;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "QueryLaneAdapterCap: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "QueryLaneAdapterCap: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "QueryLaneAdapterCap: Invalid adapter type 0x%08X\n", LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query Lane Adapter Configuration Capability
  //
  DEBUG ((CM_VERBOSE, "    (%016llX, %d) Query Lane Adp Cap  at offset 0x%0x\n", Router->TopologyId, AdpNum, CapReg));
  Status = Usb4CsAdpRead (Router, AdpNum, CapReg, sizeofdw (USB4_ADP_CAP_LANE), (UINT32 *)&LaneAdpCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "(%016llX, %d) Query Lane Adp Cap err at 0x%0x, %r\n", Router->TopologyId, AdpNum, CapReg, Status));
    goto Exit;
  }

  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_ADP_STATE, (UINT32) AdpNum, LaneAdpCap.CapCs1.Fields.AdapterState);

  LaneAdp->AdpDep.Lane.AdpCap.SupportedLinkSpeeds = LaneAdpCap.CapCs0.Fields.SupportedLinkSpeeds;
  LaneAdp->AdpDep.Lane.AdpCap.SupportedLinkWidths = LaneAdpCap.CapCs0.Fields.SupportedLinkWidths;
  LaneAdp->AdpDep.Lane.AdpCap.Cl0sSupport = LaneAdpCap.CapCs0.Fields.Cl0sSupport;
  LaneAdp->AdpDep.Lane.AdpCap.Cl1Support  = LaneAdpCap.CapCs0.Fields.Cl1Support;
  LaneAdp->AdpDep.Lane.AdpCap.Cl2Support  = LaneAdpCap.CapCs0.Fields.Cl2Support;

  DumpLaneCap (&LaneAdpCap);
Exit:
  return Status;
}

/**
  Query and save the required DP Adapter capability into Adapter instance.

  @param[in] Router         - Pointer to Router instance.
  @param[in] AdpNum         - Adapter number.
  @param[in] CapReg         - DWORD index of capability in configuration space.

  @retval EFI_SUCCESS   - Query and save the adapter capability success
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Unsupported adapter type.
  @retval Errors in config space access.
**/
EFI_STATUS
QueryAdpDpCap (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN UINT16          CapReg
  )
{
  EFI_STATUS            Status;
  PUSB4_ADAPTER         DpAdp;
  USB4_ADP_CAP_DP       DpAdpCap;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "QueryAdpDpCap: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "QueryAdpDpCap: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DpAdp = &(Router->Adapter[AdpNum]);
  if ((DpAdp->Type != USB4_ADP_TYPE_DP_IN) && (DpAdp->Type != USB4_ADP_TYPE_DP_OUT)) {
    DEBUG ((DEBUG_ERROR, "QueryAdpDpCap: Unsupported adapter type 0x%08X\n", DpAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query DP Adapter Capability
  //
  DEBUG ((CM_VERBOSE, "    (%016llX, %d) Query DP Adapter Cap at offset 0x%0x\n", Router->TopologyId, AdpNum, CapReg));
  Status = Usb4CsAdpRead (Router, AdpNum, CapReg, sizeofdw (USB4_ADP_CAP_DP), (UINT32 *)&DpAdpCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "(%016llX, %d) Query DP Adp Cap err at 0x%0x, %r\n", Router->TopologyId, AdpNum, CapReg, Status));
    goto Exit;
  }

  if (DpAdp->Type == USB4_ADP_TYPE_DP_IN) {
    DumpAdpDpInCap (&(DpAdpCap.DpIn));
  } else if (DpAdp->Type == USB4_ADP_TYPE_DP_OUT) {
    DumpAdpDpOutCap (&(DpAdpCap.DpOut));
  }

Exit:
  return Status;
}

/**
  Query and save the required USB3 Adapter capability into Adapter instance.

  @param[in] Router         - Pointer to Router instance.
  @param[in] AdpNum         - Adapter number.
  @param[in] CapReg         - DWORD index of capability in configuration space.

  @retval EFI_SUCCESS   - Query and save the adapter capability success
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Unsupported adapter type.
  @retval Errors in config space access.
**/
EFI_STATUS
QueryAdpUsb3Cap (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN UINT16          CapReg
  )
{
  EFI_STATUS           Status;
  PUSB4_ADAPTER        Usb3Adp;
  USB4_ADP_CAP_USB3    Usb3AdpCap;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "QueryAdpUsb3Cap: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "QueryAdpUsb3Cap: Invalid AdpNum %d\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Usb3Adp = &Router->Adapter[AdpNum];
  if ((Usb3Adp->Type != USB4_ADP_TYPE_USB3_US) && (Usb3Adp->Type != USB4_ADP_TYPE_USB3_DS)) {
    DEBUG ((DEBUG_ERROR, "QueryAdpUsb3Cap: Unsupported adapter type 0x%08X\n", Usb3Adp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query USB Adapter Capability
  //
  DEBUG ((CM_VERBOSE, "(0x%016llX, %d) Query USB3 Adp Cap at offset 0x%0x\n", Router->TopologyId, AdpNum, CapReg));
  Status = Usb4CsAdpRead (Router, AdpNum, CapReg, sizeofdw (USB4_ADP_CAP_USB3), (UINT32 *) &Usb3AdpCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "(0x%016llX, %d) Query USB3 Adp Cap err at 0x%0x, %r\n", Router->TopologyId, AdpNum, CapReg, Status));
    goto Exit;
  }

  DumpAdpUsb3Cap (&Usb3AdpCap);

Exit:
  return Status;
}

/**
  Query and save the required PCIe Adapter capability into Adapter instance.

  @param[in] Router         - Pointer to Router instance.
  @param[in] AdpNum         - Adapter number.
  @param[in] CapReg         - DWORD index of capability in configuration space.

  @retval EFI_SUCCESS           - Query and save the adapter capability success
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Unsupported adapter type.
  @retval Errors in config space access.
**/
EFI_STATUS
QueryAdpPcieCap (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN UINT16          CapReg
  )
{
  EFI_STATUS           Status;
  PUSB4_ADAPTER        PcieAdp;
  USB4_ADP_CAP_PCIE    PcieAdpCap;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "QueryAdpPcieCap: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "QueryAdpPcieCap: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  PcieAdp = &Router->Adapter[AdpNum];
  if ((PcieAdp->Type != USB4_ADP_TYPE_PCIE_US) && (PcieAdp->Type != USB4_ADP_TYPE_PCIE_DS)) {
    DEBUG ((DEBUG_ERROR, "QueryAdpPcieCap: Unsupported adapter type 0x%08X\n", PcieAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query PCIe Adapter Capability
  //
  DEBUG ((CM_VERBOSE, "Query PCIe Adapter Cap for Adapter %d at offset 0x%0x\n", AdpNum, CapReg));
  Status = Usb4CsAdpRead (Router, AdpNum, CapReg, sizeofdw (USB4_ADP_CAP_PCIE), (UINT32 *) &PcieAdpCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Query PCIe Adapter Cap failure: Adapter %d at 0x%0x, %r\n", AdpNum, CapReg, Status));
    goto Exit;
  }

  DumpAdpPcieCap (&PcieAdpCap);

Exit:
  return Status;
}

/**
  Query and save the required protocol Adapter capability into Adapter instance.

  @param[in] Router         - Pointer to Router instance.
  @param[in] AdpNum         - Adapter number.
  @param[in] CapReg         - DWORD index of capability in configuration space.

  @retval EFI_SUCCESS           - Query and save the adapter capability success
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Unsupported adapter type.
  @retval Errors in config space access.
**/
EFI_STATUS
QueryProtocolAdpCap (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN UINT16          CapReg
  )
{
  EFI_STATUS      Status;
  UINT32          AdapterType;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "QueryProtocolAdpCap: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "QueryProtocolAdpCap: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  AdapterType = Router->Adapter[AdpNum].Type;
  switch (AdapterType) {
    case USB4_ADP_TYPE_DP_OUT:
    case USB4_ADP_TYPE_DP_IN:
      Status = QueryAdpDpCap (Router, AdpNum, CapReg);
      break;
    case USB4_ADP_TYPE_USB3_US:
    case USB4_ADP_TYPE_USB3_DS:
      Status = QueryAdpUsb3Cap (Router, AdpNum, CapReg);
      break;
    case USB4_ADP_TYPE_PCIE_US:
    case USB4_ADP_TYPE_PCIE_DS:
      Status = QueryAdpPcieCap (Router, AdpNum, CapReg);
      break;
    default:
      DEBUG ((DEBUG_ERROR, "QueryProtocolAdpCap - Unsupported adapter type 0x%08X\n", AdapterType));
      Status = EFI_UNSUPPORTED;
      break;
  }

Exit:
  return Status;
}

/**
  Query and add the Adapter capability into Adapter instance.

  @param[in] Router   - Pointer to Router instance.
  @param[in] AdpNum   - Adapter number.

  @retval EFI_SUCCESS   - Query and add the adapter capability success
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Unsupported adapter type.
  @retval Errors in config space access.
**/
EFI_STATUS
Usb4AdpScanCapabilities (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS                Status;
  PUSB4_ADAPTER             Adapter;
  USB4_CAP_HEADER           AdpCapHeader;
  PUSB4_ADP_CAP_VSC         VscCapHeader;
  USB4_ADP_CAP_VSEC_CS_1    VsecCs1;
  UINT16                    CapReg;
  UINT8                     CapId;
  UINT16                    VsecNextCap;

  DEBUG ((DEBUG_INFO, "Usb4AdpScanCapabilities entry\n"));
  Status = EFI_SUCCESS;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4AdpScanCapabilities: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4AdpScanCapabilities: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Adapter = &Router->Adapter[AdpNum];
  CapReg  = (UINT16)Adapter->NextCap;
  while (CapReg != 0) {
    VsecNextCap = 0;
    DEBUG ((CAP_VERBOSE, "    Adapter %d - Read Adapter capability from DWORD 0x%0x\n", AdpNum, CapReg));
    //
    // Read the Capability ID
    //
    Status = Usb4CsAdpRead (Router, AdpNum, CapReg, sizeofdw (AdpCapHeader), (UINT32 *) &AdpCapHeader);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Adapter %d - Read capability ID at offset 0x%0x failure, %r\n", AdpNum, CapReg, Status));
      goto Exit;
    }

    CapId = (UINT8) AdpCapHeader.CapId;
    switch (CapId) {
      case USB4_ADP_CAP_ID_LANE:
        DEBUG ((DEBUG_INFO, "    Adapter %d, CapId = %d, LaneAdpCapReg = 0x%0x\n", AdpNum, CapId, CapReg));
        if (Adapter->Type == USB4_ADP_TYPE_LANE) {
          Adapter->AdpCapReg = (UINT8) CapReg;
          Status = QueryLaneAdapterCap (Router, AdpNum, CapReg);
        } else {
          DEBUG ((DEBUG_ERROR, "CapId %d is not supported for adp %d type 0x%08X\n", CapId, AdpNum, Adapter->Type));
        }
        break;
      case USB4_ADP_CAP_ID_USB4_PORT:
        DEBUG ((DEBUG_INFO, "    Adapter %d, CapId = %d, Usb4PortCapReg = 0x%0x\n", AdpNum, CapId, CapReg));
        if (Adapter->Type == USB4_ADP_TYPE_LANE) {
          Adapter->AdpDep.Lane.PgCapReg = (UINT8) CapReg;
          Status = QueryUsb4PortCap (Router, AdpNum, CapReg);
        } else {
          DEBUG ((DEBUG_ERROR, "CapId %d is not supported for adp %d type 0x%08X\n", CapId, AdpNum, Adapter->Type));
        }
        break;
      case USB4_ADP_CAP_ID_TMU:
        DEBUG ((DEBUG_INFO, "    Adapter %d, CapId = %d, TmuCapReg = 0x%0x\n", AdpNum, CapId, CapReg));
        Adapter->TmuCapReg = (UINT8) CapReg;
        if (Adapter->Type == USB4_ADP_TYPE_LANE) {
          Status = QueryLaneAdapterTmuCap (Router, AdpNum, CapReg);
        } else {
          DEBUG ((DEBUG_ERROR, "CapId %d is not supported for adp %d type 0x%08X\n", CapId, AdpNum, Adapter->Type));
        }
        break;
      case USB4_ADP_CAP_ID_PROTOCOL:
        DEBUG ((DEBUG_INFO, "    Adapter %d, CapId = %d, AdpCapReg = 0x%0x\n", AdpNum, CapId, CapReg));
        Adapter->AdpCapReg = (UINT8) CapReg;
        Status = QueryProtocolAdpCap (Router, AdpNum, CapReg);
        break;
      case USB4_ADP_CAP_ID_VENDOR:
        DEBUG ((DEBUG_INFO, "    Adapter %d, CapId = %d, VendorCapReg = 0x%0x\n", AdpNum, CapId, CapReg));
        //
        // VSC and VSEC cap use the same CAP ID.
        // Need to check bit [31:24] to identify VSC and VSEC.
        //
        VscCapHeader = (PUSB4_ADP_CAP_VSC) &AdpCapHeader;
        //
        // <TBT3>
        // Save adapter VSC0 and VSC1 offset for TBT3 support.
        //
        if (VscCapHeader->CapCs0.Fields.VscLength != 0) {
          if (VscCapHeader->CapCs0.Fields.VscId == 0) {
            Adapter->Vsc0Reg = CapReg;
          } else if (VscCapHeader->CapCs0.Fields.VscId == 1) {
            Adapter->Vsc1Reg = CapReg;
          }
          DEBUG ((DEBUG_INFO, "    VscId = 0x%0x, offset = 0x%0x, VscDws = 0x%0x\n",
                  VscCapHeader->CapCs0.Fields.VscId,
                  CapReg,
                  VscCapHeader->CapCs0.Fields.VscLength
                  ));
        } else {
          //
          // VSEC is identified, read CS1 to get the next capability offset
          //
          Status = Usb4CsAdpRead (Router, AdpNum, CapReg + 1, 1, (UINT32 *) &VsecCs1);
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "Read Adp %d CapReg 0x%0x VSEC length failure, %r\n", AdpNum, CapReg + 1, Status));
            break;
          }
          VsecNextCap = (UINT16) VsecCs1.Fields.NextCap;

          DEBUG ((DEBUG_INFO, "    VsecId = 0x%0x, offset = 0x%0x, VsecDws = 0x%0x\n",
                  ((PUSB4_ADP_CAP_VSEC_CS_0) &AdpCapHeader)->Fields.VsecId,
                  CapReg,
                  VsecCs1.Fields.VsecLength));
        }

        Status = EFI_SUCCESS;
        break;
      default:
        DEBUG ((DEBUG_ERROR, "    Adapter %d, CapId = %d (Unsupported), Reg = 0x%0x\n", AdpNum, AdpCapHeader.CapId, CapReg));
        break;
    }
    if (EFI_ERROR (Status)) {
      goto Exit;
    }

    //
    // VSEC Next Cap is at different location.
    //
    CapReg = (VsecNextCap) ? VsecNextCap : (UINT16) AdpCapHeader.NextCap;
  }

  //
  // Lane adapter capability should be present in Lane adapter.
  // DP/PCIe/USB3 adapter capability should be present in DP/PCIe/USB3 adapter.
  //
  if (Adapter->AdpCapReg == 0) {
    if ((Adapter->Type != USB4_ADP_TYPE_UNSUPPORTED) && (Adapter->Type != USB4_ADP_TYPE_HOST_INTERFACE)) {
      DEBUG ((DEBUG_ERROR, "No Adapter Capability for Rt 0x%016llX Adp %d\n", Router->TopologyId, AdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
  }
Exit:
  DEBUG ((DEBUG_INFO, "Usb4AdpScanCapabilities exit\n"));
  return Status;
}

/**
  Enumerate USB4 Adapter and its Capabilities.

  @param[in,out] Router - Pointer to USB4 Router instance.
  @param[in]     AdpNum - Adapter Number.
  @param[in]     Lane0  - Lane0 indicator. Enumerated adapter should be Lane0 in USB4 Port if it's Lane adapter.

  @retval EFI_SUCCESS           - Enumerate USB4 Adapter success.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access.
**/
EFI_STATUS
Usb4AdpEnum (
  IN OUT PUSB4_ROUTER    Router,
  IN  UINT8              AdpNum,
  IN  BOOLEAN            Lane0
  )
{
  EFI_STATUS                Status;
  PTOPOLOGY_ID              TopologyId;
  PUSB4_ADAPTER             Adapter;
  USB4_ADP_CS_INFO          AdpCsInfo;
  UINT32                    AdapterType;
  TBT3_ADP_CS_0             *Tbt3AdpCs0;
  USB4_ADP_CAP_USB3_CS_0    Usb3CapCs0;

  DEBUG ((DEBUG_INFO, "Usb4AdpEnum entry, Adapter number = %d\n", AdpNum));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4AdpEnum: Null Router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4AdpEnum: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  TopologyId = &(Router->TopologyId);
  Adapter    = &(Router->Adapter[AdpNum]);

  Adapter->Router = Router;

  //
  // Read the basic Adapter Configuration Space registers
  //
  DEBUG ((CM_VERBOSE, "Read Adapter config space - Rt 0x%016llX Adp %d\n", TopologyId->Value, AdpNum));
  Status = Usb4CsAdpRead (Router, AdpNum, 0, sizeofdw (AdpCsInfo), (UINT32 *) &AdpCsInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d: Read config space failure, %r\n", TopologyId->Value, AdpNum, Status));
    Adapter->Type = USB4_ADP_TYPE_UNSUPPORTED;
    //
    // EFI_NOT_FOUND is returned by Usb4CsAdpRead if ERR_ADDR notification is received.
    // That means the adapter is unused.
    // This is not a failure.
    //
    if (Status == EFI_NOT_FOUND) {
      DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d is unused.\n", TopologyId, AdpNum));
      Status = EFI_SUCCESS;
    }
    goto Exit;
  }

  //
  // <TBT3>
  // TBT3 adapter is identified as unsupported adapter if adapter CS_0 bis 31-16 is 0xFFFF.
  // Refer to TBT3 CM guide 3.2.2 Enumerating ports
  //
  if (Router->State.Tbt3) {
    Tbt3AdpCs0 = (PTBT3_ADP_CS_0) &(AdpCsInfo.AdpCs0);
    if (Tbt3AdpCs0->Fields.ProductId == 0xFFFF) {
      Adapter->Type = USB4_ADP_TYPE_UNSUPPORTED;
      Status = EFI_SUCCESS;
      goto Exit;
    }
  }

  AdapterType = AdpCsInfo.AdpCs2.Fields.AdapterType;
  if (IsAdapterTypeValid (AdapterType)) {
      Adapter->Type   = AdapterType;
  } else {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d: Invalid type 0x%06x!\n", TopologyId->Value, AdpNum, AdapterType));
      Adapter->Type = USB4_ADP_TYPE_UNSUPPORTED;
      //
      // Enumerate the remaining adapters even the unsupported adapter type is enumerated.
      //
      Status = EFI_SUCCESS;
      goto Exit;
  }
  Adapter->AdpNum         = AdpNum;
  Adapter->NextCap        = (UINT8) AdpCsInfo.AdpCs1.Fields.NextCapability;
  Adapter->MaxInputHopId  = (UINT16) AdpCsInfo.AdpCs5.Fields.MaxInputHopId;
  Adapter->MaxOutputHopId = (UINT16) AdpCsInfo.AdpCs5.Fields.MaxOutputHopId;
  DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d: Type = 0x%06x (%s), NextCap = 0x%0x, MaxInHop = %d, MaxOutHop = %d\n",
          TopologyId->Value,
          Adapter->AdpNum,
          Adapter->Type,
          AdapterTypeName (Adapter->Type),
          Adapter->NextCap,
          Adapter->MaxInputHopId,
          Adapter->MaxOutputHopId
          ));

  DEBUG ((DEBUG_INFO, "Total Buffers = %d, NFC Buffers = %d\n", AdpCsInfo.AdpCs4.Fields.TotalBuffers, AdpCsInfo.AdpCs4.Fields.NfcBuffers));

  if (AdapterType == USB4_ADP_TYPE_LANE) {
    Adapter->AdpDep.Lane.Status.Lane0 = (Lane0) ? 1 : 0;
    DEBUG ((DEBUG_INFO, "Adapter %d - Lane Adapter, Lane0 = %d\n", AdpNum, Adapter->AdpDep.Lane.Status.Lane0));
  }

  //
  // Scan the adapter capabilities.
  //
  if (AdapterType != USB4_ADP_TYPE_UNSUPPORTED) {
    Status = Usb4AdpScanCapabilities (Router, AdpNum);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d - Scan Adp Cap failure, %r\n", Router->TopologyId, AdpNum, Status));
      if (Adapter->AdpCapReg == 0) {
        DEBUG ((DEBUG_ERROR, "Usb4AdpEnum: Adp %d capability is not present.\n", AdpNum));
      }
      //
      // <TBT3>
      // Skip capability scan failure
      // Got ERR_ADDR failure during enumerate Tapex adapter 7 (Host Interface adapter) CapReg 0x52
      //
      Status = EFI_SUCCESS;
    }
  }

  //
  // Disconnect all USB3 downstream adapters before USB3 path setup.
  // USB3 path will be setup when the downstream router is connected and USB3 tunneling is enabled.
  //
  // USB4 CM guide section 3.2.2:
  // - If the Connection Manager does not setup a Path to a USB3 Adapter, it disconnects the Adapter as defined in Section 5.5.3.
  //
  // USB port reset timeout failure is observed in USB4 router hot plug if this is not implemented.
  //
  if (AdapterType == USB4_ADP_TYPE_USB3_DS) {
    DEBUG ((DEBUG_INFO, "Disconnect Rt 0x%016llX USB3 Adp %d before USB3 path setup\n", Router->TopologyId, AdpNum));
    Status = Usb4CsAdpRead (Router, AdpNum, Adapter->AdpCapReg, 1, (UINT32 *) &Usb3CapCs0);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4AdpEnum: Read USB3 cap cs_0 failure, %r\n", Status));
      goto Exit;
    }
    //
    // Disconnect USB3 downstream adapter.
    //
    // USB4 CM guide section 5.5.3:
    //   When the Connection Manager sets the ADP_USB3_CS_0.Path Enable bit to 0b and the ADP_USB3_CS_0.Valid bit to 1b,
    //   it initiates a USB3 Adapter Disconnect.
    //
    Usb3CapCs0.Fields.PathEnable = 0;
    Usb3CapCs0.Fields.Valid      = 1;
    Status = Usb4CsAdpWrite (Router, AdpNum, Adapter->AdpCapReg, 1, (UINT32 *) &Usb3CapCs0);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4AdpEnum: Write USB3 cap cs_0 failure, %r\n", Status));
      goto Exit;
    }
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4AdpEnum exit\n"));
  return Status;
}

/**
  Wait for Lane bonding complete

  @param[in] Router - Pointer to Router instance.
  @param[in] AdpNum - Downstream Adapter number for lane bonding.

  @retval EFI_SUCCESS           - Lane bonding complete.
  @retval EFI_TIMEOUT           - Wait for Lane bonding complete timeout.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access.
**/
EFI_STATUS
WaitForLaneBondingComplete (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS                Status;
  UINT32                    Index;
  PUSB4_ADAPTER             LaneAdp;
  USB4_ADP_CAP_LANE_CS_1    AdpCapCs1;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "WaitForLaneBondingComplete: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "WaitForLaneBondingComplete - Router 0x%016llX, Adapter %d\n", Router->TopologyId, AdpNum));

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "WaitForLaneBondingComplete: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX, Adp %d - Not Lane adapter, Adapter type = %d\n", Router->TopologyId, AdpNum, LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  for (Index = 0; Index < CM_STATUS_WAITS_MAX; Index++) {
    Status = Usb4CsAdpRead (Router, AdpNum, LaneAdp->AdpCapReg + USB4_CS_1, 1, (UINT32 *) &AdpCapCs1);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Query Lane Cap failure: Adapter %d at 0x%0x, %r\n", AdpNum, LaneAdp->AdpCapReg, Status));
      goto Exit;
    }

    if (AdpCapCs1.Fields.CurLinkWidth == LANE_LINK_WIDTH_x2) {
      DEBUG ((DEBUG_INFO, "Bonding complete, poll count = %d\n", Index));
      break;
    }

    Status = gBS->Stall (CM_STATUS_POLL_US);
  }

  DumpLaneCapCs1 (&AdpCapCs1);

  if (Index == CM_STATUS_WAITS_MAX) {
    DEBUG ((DEBUG_ERROR, "Lane Bonding timeout, poll count = %d\n", Index));
    Status = EFI_TIMEOUT;
    goto Exit;
  }
Exit:
  DEBUG ((DEBUG_INFO, "WaitForLaneBondingComplete exit - %r\n", Status));
  return Status;
}

/**
  Set target link width and set LaneBonding bit to enable lane bonding.

  @param[in] Router      - Pointer to Router instance.
  @param[in] Lane0AdpNum - Lane0 Adapter Number.
  @param[in] SetBonding  - 1: Set LaneBonding bit (Lane_ADP_CS1 bit 15) to 1.
                           0: Don't set LaneBonding bit.

  @retval EFI_SUCCESS           - Enable lane bonding on the given lane adapter successfully.
  @retval EFI_UNSUPPORTED       - Fail to enable lane bonding on the given lane adapter.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
EnableLaneBonding (
  IN PUSB4_ROUTER    Router,
  IN UINT8           Lane0AdpNum,
  IN BOOLEAN         SetBonding
  )
{
  EFI_STATUS                Status;
  PUSB4_ADAPTER             LaneAdp;
  USB4_ADP_CAP_LANE_CS_1    AdpCapCs1[2];
  INT8                      Index;
  UINT8                     AdpNum;

  DEBUG ((DEBUG_INFO, "EnableLaneBonding entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "EnableLaneBonding: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Lane0AdpNum should be less than MaxAdapter
  // Otherwise, its Lane1 adapter will be greater than MaxAdapter
  //
  if ((Lane0AdpNum == 0) || (Lane0AdpNum >= Router->MaxAdapter) || (Lane0AdpNum >= USB4_ADAPTER_SUPPORT_MAX - 1)) {
    DEBUG ((DEBUG_ERROR, "EnableLaneBonding: Invalid AdpNum %d!\n", Lane0AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Make sure both Lane0 and Lane1 adapters are in CL0 state
  //
  for (Index = 0; Index < 2; Index++) {
    AdpNum = Lane0AdpNum + (UINT8) Index;
    LaneAdp = &Router->Adapter[AdpNum];
    if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX, Adp %d - Not Lane adapter, Adapter type = %d\n", Router->TopologyId, AdpNum, LaneAdp->Type));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    if (LaneAdp->AdpCapReg == 0) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX, Adp %d - NULL lane adapter capability\n", Router->TopologyId, AdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    Status = Usb4CsAdpRead (Router, AdpNum, LaneAdp->AdpCapReg + USB4_CS_1, 1, (UINT32 *) &(AdpCapCs1[Index]));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Query Lane Cap failure: Adapter %d at 0x%0x, %r\n", AdpNum, LaneAdp->AdpCapReg, Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "Adp %d original link width = %d\n", AdpNum, AdpCapCs1[Index].Fields.CurLinkWidth));
    if (AdpCapCs1[Index].Fields.AdapterState != LANE_STATE_CL0) {
      DEBUG ((DEBUG_ERROR, "Lane adp %d is not in CL0 status, state = %d\n", AdpNum, AdpCapCs1[Index].Fields.AdapterState));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
  }

  //
  // <TBT3>
  // Check VSEC6 PORT_ATTR bonding enable bit for TBT3 lane bonding support
  //
  if (Router->State.Tbt3) {
    if (IsTbt3LaneBondingSupported (Router, Lane0AdpNum) == FALSE) {
      DEBUG ((DEBUG_ERROR, "Rt 0x %016llX Adp %d TBT3 lane bonding is not supported!\n", Router->TopologyId, Lane0AdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
  }

  //
  // The target link width of both lane 0 and lane1 should be set to x2.
  // LaneBonding bit is set only in lane 0 and is the last step for lane bonding.
  //
  for (Index = 1; Index >= 0; Index--) {
    AdpNum  = Lane0AdpNum + (UINT8) Index;
    LaneAdp = &Router->Adapter[AdpNum];
    //
    // Enable Lane Bonding only if the current Link width is not x2.
    // Otherwise, the lane bonding request will wait for no response.
    // Need to check with new FW to see if the situation is same.
    //
    if (AdpCapCs1[Index].Fields.CurLinkWidth != LANE_LINK_WIDTH_x2) {
      AdpCapCs1[Index].Fields.LinkWidthTarget = LANE_LINK_TARGET_WIDTH_x2;
      //
      // Set LaneBonding bit for Lane0 only
      //
      if (SetBonding && (Index == 0)) {
        DEBUG ((DEBUG_INFO, "Adp %d: Set LaneBonding to 1\n", AdpNum));
        AdpCapCs1[Index].Fields.LaneBonding = 1;
      }
      DEBUG ((DEBUG_INFO, "Write adp %d cap cs1\n", AdpNum));
      Status = Usb4CsAdpWrite (Router, AdpNum, LaneAdp->AdpCapReg + USB4_CS_1, 1, (UINT32 *) &(AdpCapCs1[Index]));
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Write Lane Cap failure: Adapter %d at 0x%0x, %r\n", AdpNum, LaneAdp->AdpCapReg, Status));
        goto Exit;
      }
    }
  }
Exit:
  DEBUG ((DEBUG_INFO, "EnableLaneBonding exit\n"));
  return Status;
}

/**
  Bonding dual lanes link in USB4 Port.

  @param[in,out] Router - Pointer to Router instance.
  @param[in]     AdpNum - Adapter Number.

  @retval EFI_SUCCESS           - Bonding dual lanes link success.
  @retval EFI_UNSUPPORTED       - Lane bonding is not supported.
  @retval EFI_TIMEOUT           - Lane bonding timeout
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
AdapterLaneBonding (
  IN OUT PUSB4_ROUTER    Router,
  IN     UINT8           AdpNum
  )
{
  EFI_STATUS             Status;
  UINT8                  DsRtUpAdpNum;
  PUSB4_ADAPTER          LaneAdp;
  PUSB4_ADAPTER          DsRtUpAdapter;
  PUSB4_ROUTER           DsRouter;
  USB4_ADP_CAP_LANE_CS_1 AdpCapCs1;

  DEBUG ((DEBUG_INFO, "AdapterLaneBonding entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "AdapterLaneBonding: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "AdapterLaneBonding: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "AdpNum %d is not Lane Adapter, Adapter type = %d\n",AdpNum, LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DsRouter = LaneAdp->AdpDep.Lane.DsRouter;
  if (DsRouter == NULL) {
    DEBUG ((DEBUG_ERROR, "No downstream Router at Router 0x%016llX Adapter %d\n", Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  DsRtUpAdpNum = DsRouter->LaneUpAdpNum;
  if ((DsRtUpAdpNum == 0) || (DsRtUpAdpNum > DsRouter->MaxAdapter) || (DsRtUpAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "AdapterLaneBonding: Invalid upstream AdpNum %d in DsRt!\n", DsRtUpAdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DsRtUpAdapter = &DsRouter->Adapter[DsRtUpAdpNum];
  if (DsRtUpAdapter->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "DsRtUpAdpNum %d is not Lane Adapter, Adapter type = %d\n",DsRtUpAdpNum, DsRtUpAdapter->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Both ends of Link need to support x2 width for lane bonding
  //
  if (!((LaneAdp->AdpDep.Lane.AdpCap.SupportedLinkWidths & LANE_LINK_WIDTH_x2) &&
        (DsRtUpAdapter->AdpDep.Lane.AdpCap.SupportedLinkWidths & LANE_LINK_WIDTH_x2))) {
    DEBUG ((DEBUG_ERROR, "Lane bonding failure - Not both adapters support dual lanes\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // When USB4 link is in Gen4 mode, bonding is done automatically by HW, hence CM is not required to do the bonding.
  //
  Status = Usb4CsAdpRead (Router, AdpNum, LaneAdp->AdpCapReg + USB4_CS_1, 1, (UINT32 *) &(AdpCapCs1));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Query Lane Cap failure: Adapter %d at 0x%0x, %r\n", AdpNum, LaneAdp->AdpCapReg, Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d - Current Link Speed = 0x%0x\n", Router->TopologyId, AdpNum, AdpCapCs1.Fields.CurLinkSpeed));

  if (AdpCapCs1.Fields.CurLinkSpeed == LANE_LINK_SPEED_GEN4) {
    DEBUG ((DEBUG_INFO, "Skip Lane Bonding for Gen4 link\n"));
    Status = EFI_SUCCESS;
    Router->Adapter[AdpNum].AdpDep.Lane.Status.LbEnabled          = TRUE;
    DsRouter->Adapter[DsRtUpAdpNum].AdpDep.Lane.Status.LbEnabled  = TRUE;
    goto Exit;
  }

  //
  // Set the target link width in upstream USB4 port of downstream Router.
  //
  Status = EnableLaneBonding (DsRouter, DsRtUpAdpNum, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d - Enable lane bonding bit failure, %r\n", DsRouter->TopologyId, DsRtUpAdpNum, Status));
    goto Exit;
  }

  //
  // Set the target link width in downstream Lane Adapter.
  // Enable LaneBonding bit in the last step.
  //
  Status = EnableLaneBonding (Router, AdpNum, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d - Enable lane bonding bit failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

  //
  // Wait for Lane Bonding complete
  //
  Status = WaitForLaneBondingComplete (Router, AdpNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to wait for lane bonding complete, %r\n", Status));
    goto Exit;
  }

  Router->Adapter[AdpNum].AdpDep.Lane.Status.LbEnabled          = TRUE;
  DsRouter->Adapter[DsRtUpAdpNum].AdpDep.Lane.Status.LbEnabled  = TRUE;

Exit:
  DEBUG ((DEBUG_INFO, "AdapterLaneBonding exit - %r\n", Status));
  return Status;
}

/**
  Query Lane Adapter State.

  @param[in]  Router       - Pointer to Router instance.
  @param[in]  AdpNum       - Downstream Adapter number.
  @param[out] AdapterState - Lane adapter state.

  @retval EFI_SUCCESS           - Query Lane adapter state successfully.
  @retval EFI_UNSUPPORTED       - Not a Lane adapter or Lane adapter capability is not present.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
QueryLaneAdapterState (
  IN  PUSB4_ROUTER          Router,
  IN  UINT8                 AdpNum,
  OUT LANE_ADAPTER_STATE    *AdapterState
  )
{
  EFI_STATUS                Status;
  PUSB4_ADAPTER             LaneAdp;
  USB4_ADP_CAP_LANE_CS_1    LaneAdpCapCs1;

  if ((Router == NULL) || (AdapterState == NULL)) {
    DEBUG ((DEBUG_ERROR, "QueryLaneAdapterState: Invalid parameter (%p,%p)\n", Router, AdapterState));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "QueryLaneAdapterState: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "AdpNum %d is not Lane Adapter, Adapter type = %d\n",AdpNum, LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  *AdapterState = LANE_STATE_INVALID;

  if (LaneAdp->AdpCapReg != 0) {
    //
    // Query Lane Adapter Capabiity CS1 for Adapter State
    //
    Status = Usb4CsAdpRead (Router, AdpNum, LaneAdp->AdpCapReg + USB4_CS_1, 1, (UINT32 *) &LaneAdpCapCs1);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Query Lane Adapter State failure, %016llX Adapter %d, %r\n", Router->TopologyId, AdpNum, Status));
      goto Exit;
    }

    if (LaneAdpCapCs1.Fields.AdapterState <= LANE_STATE_MAX) {
      *AdapterState = LaneAdpCapCs1.Fields.AdapterState;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "QueryLaneAdapterState: Lane adapter capability is not present!\n"));
    Status = EFI_UNSUPPORTED;
  }
Exit:
  return Status;
}

/**
  Unlock Router Downstream Adapter.

  @param[in] Router - Pointer to Router instance.
  @param[in] AdpNum - Adapter number to be unlocked.

  @retval EFI_SUCCESS           - Unlock Router Downstream Adapter success.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Unsupported adapter type.
  @retval Errors in config space access.
**/
EFI_STATUS
LaneAdapterUnlock (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS       Status;
  PUSB4_ADAPTER    LaneAdp;
  USB4_ADP_CS_4    AdpCs4;

  DEBUG ((DEBUG_INFO, "LaneAdapterUnlock entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "LaneAdapterUnlock: Null Router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // <TBT3>
  // TBT3 doesn't use adapter lock.
  //
  if (Router->State.Tbt3) {
    DEBUG ((DEBUG_INFO, "LaneAdapterUnlock: Adapter unlock action is not required for TBT3 router.\n"));
    Status = EFI_SUCCESS;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "LaneAdapterUnlock: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "AdpNum %d is not Lane Adapter, Adapter type = %d\n",AdpNum, LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsAdpRead (Router, AdpNum, USB4_CS_4, 1, (UINT32 *) &AdpCs4);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Query Rt 0x%016llX Adp %d CS4 err, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Original Rt 0x%016llX Adp %d lock = %d\n", Router->TopologyId, AdpNum, AdpCs4.Fields.LocK));

  //
  // Unlock the Downstream Adapter
  //
  AdpCs4.Fields.LocK = 0;
  Status = Usb4CsAdpWrite (Router, AdpNum, USB4_CS_4, 1, (UINT32 *) &AdpCs4);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write Rt 0x%016llX Adp %d CS4 failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "LaneAdapterUnlock exit\n"));
  return Status;
}

/**
  Configure link TMU mode.

  @param[in,out] Router - Pointer to Router instance.
  @param[in]     AdpNum - Adapter Number.
  @param[in]     Connect - TRUE  : Config link TMU for router connection
                           FALSE : Config link TMU for router disconnect

  @retval EFI_SUCCESS           - Config link TMU mode success.
  @retval EFI_UNSUPPORTED       - Config link TMU is not supported.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
LinkConfigTmuMode (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN BOOLEAN         Connect
  )
{
  EFI_STATUS               Status;
  PUSB4_ADAPTER            LaneDfp;
  PUSB4_ADAPTER            DsRtUfp;
  PUSB4_ROUTER             DsRouter;
  UINT8                    DsRtUfpNum;
  UINT32                   TmuDws[2];
  USB4_ADP_CAP_TMU_CS_8    *TmuCap8;
  USB4_ADP_CAP_TMU_CS_9    *TmuCap9;

  DEBUG ((DEBUG_INFO, "LinkConfigTmuMode entry - Connect = %d\n", Connect));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "LinkConfigTmuMode: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "LinkConfigTmuMode: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneDfp = &Router->Adapter[AdpNum];
  if (LaneDfp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "LinkConfigTmuMode: AdpNum %d is not Lane Adapter, Adp type = %d\n", AdpNum, LaneDfp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DsRouter = LaneDfp->AdpDep.Lane.DsRouter;
  if (DsRouter == NULL) {
    DEBUG ((DEBUG_ERROR, "LinkConfigTmuMode: No DS Router at Rt 0x%016llX Adp %d\n", Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  DsRtUfpNum = DsRouter->LaneUpAdpNum;
  if ((DsRtUfpNum == 0) || (DsRtUfpNum > DsRouter->MaxAdapter) || (DsRtUfpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "LinkConfigTmuMode: Invalid upstream AdpNum %d in DsRt!\n", DsRtUfpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DsRtUfp = &DsRouter->Adapter[DsRtUfpNum];
  if (DsRtUfp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "DsRtUfpNum %d is not Lane Adapter, Adapter type = %d\n", DsRtUfpNum, DsRtUfp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Configure TMU to Enhanced Uni-Directional mode if both routers of link supports USB4 Ver2
  //
  DEBUG ((DEBUG_INFO, "USB4 Version : DFP = 0x%0x, UFP = 0x%0x\n", Router->Usb4Version, DsRouter->Usb4Version));
  if ((Router->Usb4Version != RT_USB4_VERSION_2) || (DsRouter->Usb4Version != RT_USB4_VERSION_2)) {
    DEBUG ((DEBUG_INFO, "TMU Enhanced Uni-Directional mode is not configured!\n"));
    Status = EFI_SUCCESS;
    goto Exit;
  }

  TmuCap8 = (USB4_ADP_CAP_TMU_CS_8 *)&(TmuDws[0]);
  TmuCap9 = (USB4_ADP_CAP_TMU_CS_9 *)&(TmuDws[1]);

  if (Connect) {
    //
    // USB4 2.0 specification 7.3.1.3:
    // TMU should be set to Enhanced Uni-Directional mode if the Routers of both sides of link support USB4 Version 2.
    // Connection Manager Notes:
    // UFP TMU should be configured before DFP TMU is configured.
    //
    DEBUG ((DEBUG_INFO, "Rt 0x%016llx DFP %d - Set TMU Enhanced Uni-Directional Mode\n", Router->TopologyId, AdpNum));
    //
    // Configure UFP TMU for Enhanced Uni-Directional Mode
    //
    Status = Usb4CsAdpRead (DsRouter, DsRtUfpNum, DsRtUfp->TmuCapReg + 8, 2, (UINT32 *)(&TmuDws[0]));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Read Rt 0x%016llX UFP %d TMU Cap failure, %r\n", DsRouter->TopologyId, DsRtUfpNum, Status));
      goto Exit;
    }
    TmuCap9->Fields.DirSwitchN      = TMU_EUDM_DIRSWITCHN;
    TmuCap9->Fields.AdpSyncInterval = TMU_EUDM_ADP_TIME_SYNC_INTERVAL;
    TmuCap8->Fields.EudmEnable      = 1;
    Status = Usb4CsAdpWrite (DsRouter, DsRtUfpNum, DsRtUfp->TmuCapReg + 8, 2, (UINT32 *)(&TmuDws[0]));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Write Rt 0x%016llX UFP TMU Cap failure, %r\n", DsRouter->TopologyId, Status));
      goto Exit;
    }

    //
    // Configure DFP TMU for Enhanced Uni-Directional Mode
    //
    Status = Usb4CsAdpRead (Router, AdpNum, LaneDfp->TmuCapReg + 8, 2, (UINT32 *)(&TmuDws[0]));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Read Rt 0x%016llX DFP %d TMU Cap failure, %r\n", Router->TopologyId, AdpNum, Status));
      goto Exit;
    }
    TmuCap9->Fields.DirSwitchN      = TMU_EUDM_DIRSWITCHN;
    TmuCap9->Fields.AdpSyncInterval = TMU_EUDM_ADP_TIME_SYNC_INTERVAL;
    TmuCap8->Fields.EudmEnable      = 1;
    Status = Usb4CsAdpWrite (Router, AdpNum, LaneDfp->TmuCapReg + 8, 2, (UINT32 *)(&TmuDws[0]));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Write Rt 0x%016llX DFP %d TMU Cap failure, %r\n", Router->TopologyId, AdpNum, Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "Configure TMU Enhanced Uni-Directional Mode successfully\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Rt 0x%016llx DFP %d - Disable DFP TMU\n", Router->TopologyId, AdpNum));
    //
    // Disable DFP TMU if Enhanced Uni-Directional mode is enabled on connection.
    //
    Status = Usb4CsAdpRead (Router, AdpNum, LaneDfp->TmuCapReg + 8, 2, (UINT32 *)(&TmuDws[0]));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Read Rt 0x%016llX DFP %d TMU Cap failure, %r\n", Router->TopologyId, AdpNum, Status));
      goto Exit;
    }
    if (TmuCap8->Fields.EudmEnable) {
      DEBUG ((DEBUG_INFO, "Rt 0x%016llx DFP %d - Set TMU mode off from EUDM\n", Router->TopologyId, AdpNum));
      TmuCap9->Fields.DirSwitchN      = 0;
      TmuCap9->Fields.AdpSyncInterval = 0;
      TmuCap8->Fields.EudmEnable      = 0;
      Status = Usb4CsAdpWrite (Router, AdpNum, LaneDfp->TmuCapReg + 8, 2, (UINT32 *)(&TmuDws[0]));
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Write Rt 0x%016llX DFP %d TMU Cap failure, %r\n", Router->TopologyId, AdpNum, Status));
        goto Exit;
      }
    }
  }

Exit:
  DEBUG ((DEBUG_INFO, "LinkConfigTmuMode exit\n"));
  return Status;
}
