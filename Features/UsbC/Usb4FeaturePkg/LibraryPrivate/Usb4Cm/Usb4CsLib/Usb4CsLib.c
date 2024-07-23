/** @file
  USB4 configuration space access implementation.

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
#include <Library/DebugLib.h>
#include <Library/Usb4ConfigLayer.h>
#include <Library/Usb4ConfigSpace.h>
#include <Library/Usb4CsIo.h>
#include <Library/Usb4RouterDefs.h>
#include <Library/Usb4DomainDefs.h>

/**
  Read USB4 router config space data.

  @param[in]  Router  - Pointer to Router instance.
  @param[in]  Offset  - DWORD offset in Router Configuration Space.
  @param[in]  Count   - Data count in DWORD.
  @param[out] Data    - Data buffer for the returned data.

  @retval EFI_SUCCESS           - Read USB4 router config space data successfully.
  @retval EFI_UNSUPPORTED       - Fail to read USB4 router config space data.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CsRtRead (
  IN  PUSB4_ROUTER    Router,
  IN  UINT16          Offset,
  IN  UINT8           Count,
  OUT UINT32          *Data
  )
{
  EFI_STATUS     Status;
  USB4_DOMAIN    *Domain;
  USB4_CS_IO     *Usb4CsIo;

  if ((Router == NULL) || (Data == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CsRtRead: Invalid parameter (%p,%p)\n", Router, Data));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsRtRead: Null domain\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Usb4CsIo = Domain->Usb4CsIo;
  if (Usb4CsIo == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsRtRead: Null Usb4CsIo\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsIo->CsRead (Usb4CsIo, &(Router->TopologyId), USB4_CS_ROUTER, 0, Offset, Count, Data, NULL);
Exit:
  return Status;
}

/**
  Write USB4 router config space data.

  @param[in]  Router  - Pointer to Router instance.
  @param[in]  Offset  - DWORD offset in Router Configuration Space.
  @param[in]  Count   - Data count in DWORD.
  @param[out] Data    - Data buffer for the written data.

  @retval EFI_SUCCESS           - Write USB4 router config space data successfully.
  @retval EFI_UNSUPPORTED       - Fail to write USB4 router config space data.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CsRtWrite (
  IN  PUSB4_ROUTER    Router,
  IN  UINT16          Offset,
  IN  UINT8           Count,
  OUT UINT32          *Data
  )
{
  EFI_STATUS     Status;
  USB4_DOMAIN    *Domain;
  USB4_CS_IO     *Usb4CsIo;

  if ((Router == NULL) || (Data == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CsRtWrite: Invalid parameter (%p,%p)\n", Router, Data));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsRtWrite: Null domain\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Usb4CsIo = Domain->Usb4CsIo;
  if (Usb4CsIo == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsRtWrite: Null Usb4CsIo\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsIo->CsWrite (Usb4CsIo, &(Router->TopologyId), USB4_CS_ROUTER, 0, Offset, Count, Data);
Exit:
  return Status;
}

/**
  Read USB4 adapter config space data.

  @param[in]  Router  - Pointer to Router instance.
  @param[in]  AdpNum  - Adapter number.
  @param[in]  Offset  - DWORD offset in Adapter Configuration Space.
  @param[in]  Count   - Data count in DWORD.
  @param[out] Data    - Data buffer for the returned data.

  @retval EFI_SUCCESS           - Read USB4 adapter config space data successfully.
  @retval EFI_UNSUPPORTED       - Fail to read USB4 adapter config space data.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CsAdpRead (
  IN  PUSB4_ROUTER    Router,
  IN  UINT8           AdpNum,
  IN  UINT16          Offset,
  IN  UINT8           Count,
  OUT UINT32          *Data
  )
{
  EFI_STATUS     Status;
  USB4_DOMAIN    *Domain;
  USB4_CS_IO     *Usb4CsIo;

  if ((Router == NULL) || (Data == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CsAdpRead: Invalid parameter (%p,%p)\n", Router, Data));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsAdpRead: Null domain\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Usb4CsIo = Domain->Usb4CsIo;
  if (Usb4CsIo == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsAdpRead: Null Usb4CsIo\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (AdpNum > Router->MaxAdapter) {
    DEBUG ((DEBUG_ERROR, "Usb4CsAdpRead: Adapter number %d is out of range!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = Usb4CsIo->CsRead (Usb4CsIo, &(Router->TopologyId), USB4_CS_ADAPTER, AdpNum, Offset, Count, Data, NULL);
Exit:
  return Status;
}

/**
  Write USB4 adapter config space data.

  @param[in]  Router  - Pointer to Router instance.
  @param[in]  AdpNum  - Adapter number.
  @param[in]  Offset  - DWORD offset in Adapter Configuration Space.
  @param[in]  Count   - Data count in DWORD.
  @param[in]  Data    - Data buffer for the written data.

  @retval EFI_SUCCESS           - Write USB4 adapter config space data successfully.
  @retval EFI_UNSUPPORTED       - Fail to write USB4 adapter config space data.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CsAdpWrite (
  IN  PUSB4_ROUTER    Router,
  IN  UINT8           AdpNum,
  IN  UINT16          Offset,
  IN  UINT8           Count,
  OUT UINT32          *Data
  )
{
  EFI_STATUS     Status;
  USB4_DOMAIN    *Domain;
  USB4_CS_IO     *Usb4CsIo;

  if ((Router == NULL) || (Data == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CsAdpWrite: Invalid parameter (%p,%p)\n", Router, Data));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsAdpWrite: Null domain\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Usb4CsIo = Domain->Usb4CsIo;
  if (Usb4CsIo == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsAdpWrite: Null Usb4CsIo\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (AdpNum > Router->MaxAdapter) {
    DEBUG ((DEBUG_ERROR, "Usb4CsAdpWrite: Adapter number %d is out of range!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = Usb4CsIo->CsWrite (Usb4CsIo, &(Router->TopologyId), USB4_CS_ADAPTER, AdpNum, Offset, Count, Data);
Exit:
  return Status;
}

/**
  Read Path entry in Path Configuration Space.

  @param[in]  Router    - Pointer to Router instance.
  @param[in]  AdpNum    - Adapter number.
  @param[in]  HopId     - Path HopId.
  @param[out] PathEntry - Data buffer for the returned data.

  @retval EFI_SUCCESS           - Read USB4 path entry successfully.
  @retval EFI_UNSUPPORTED       - Fail to read USB4 path entry.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CsPathRead (
  IN  PUSB4_ROUTER           Router,
  IN  UINT8                  AdpNum,
  IN  UINT16                 HopId,
  OUT PUSB4_PATH_CS_ENTRY    PathEntry
  )
{
  EFI_STATUS     Status;
  USB4_DOMAIN    *Domain;
  USB4_CS_IO     *Usb4CsIo;

  if ((Router == NULL) || (PathEntry == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathRead: Invalid parameter (%p,%p)\n", Router, PathEntry));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathRead: Null domain\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Usb4CsIo = Domain->Usb4CsIo;
  if (Usb4CsIo == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathRead: Null Usb4CsIo\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (AdpNum > Router->MaxAdapter) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathRead: Adapter number %d is out of range!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Read path entry from Path config space.
  //
  Status = Usb4CsIo->CsRead (
             Usb4CsIo,
             &(Router->TopologyId),
             USB4_CS_PATH,
             AdpNum,
             HopId * PATH_ENTRY_DW_SIZE,
             PATH_ENTRY_DW_SIZE,
             (UINT32 *) PathEntry,
             NULL
             );
Exit:
  return Status;
}

/**
  Write Path entry in Path Configuration Space.

  @param[in]  Router    - Pointer to Router instance.
  @param[in]  AdpNum    - Adapter number.
  @param[in]  HopId     - Path HopId.
  @param[out] PathEntry - Path entry data to be written.

  @retval EFI_SUCCESS           - Write USB4 path entry successfully.
  @retval EFI_UNSUPPORTED       - Fail to write USB4 path entry.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CsPathWrite (
  IN  PUSB4_ROUTER           Router,
  IN  UINT8                  AdpNum,
  IN  UINT16                 HopId,
  OUT PUSB4_PATH_CS_ENTRY    PathEntry
  )
{
  EFI_STATUS    Status;
  USB4_DOMAIN   *Domain;
  USB4_CS_IO    *Usb4CsIo;

  if ((Router == NULL) || (PathEntry == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathWrite: Invalid parameter (%p,%p)\n", Router, PathEntry));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathWrite: Null domain\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Usb4CsIo = Domain->Usb4CsIo;
  if (Usb4CsIo == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathWrite: Null Usb4CsIo\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (AdpNum > Router->MaxAdapter) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathWrite: Adapter number %d is out of range!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Write path entry to Path config space.
  //
  Status = Usb4CsIo->CsWrite (
             Usb4CsIo,
             &(Router->TopologyId),
             USB4_CS_PATH,
             AdpNum,
             HopId * PATH_ENTRY_DW_SIZE,
             PATH_ENTRY_DW_SIZE,
             (UINT32 *) PathEntry
             );
Exit:
  return Status;
}

/**
  Enable/Disable Path entry in Path Configuration Space.

  @param[in] Router   - Pointer to Router instance.
  @param[in] AdpNum   - Adapter number.
  @param[in] HopId    - Path HopId.
  @param[in] Enable   - 1: Enable path, 0: Disable path.

  @retval EFI_SUCCESS           - Enable/Disable Path entry success.
  @retval EFI_UNSUPPORTED       - Fail to enable/disable path entry.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CsPathEnable (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN UINT16          HopId,
  IN BOOLEAN         Enable
  )
{
  EFI_STATUS            Status;
  USB4_DOMAIN           *Domain;
  USB4_CS_IO            *Usb4CsIo;
  USB4_PATH_CS_ENTRY    PathEntry;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathEnable: Null router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathEnable: Null domain\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Usb4CsIo = Domain->Usb4CsIo;
  if (Usb4CsIo == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathEnable: Null Usb4CsIo\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (AdpNum > Router->MaxAdapter) {
    DEBUG ((DEBUG_ERROR, "Usb4CsPathEnable: Adapter number %d is out of range!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = Usb4CsIo->CsRead (
             Usb4CsIo,
             &(Router->TopologyId),
             USB4_CS_PATH,
             AdpNum,
             HopId * PATH_ENTRY_DW_SIZE,
             PATH_ENTRY_DW_SIZE,
             (UINT32 *)&PathEntry,
             NULL
             );
  PathEntry.PathCs0.Fields.Valid = (Enable) ? 1 : 0;
  Status = Usb4CsIo->CsWrite (
             Usb4CsIo,
             &Router->TopologyId,
             USB4_CS_PATH,
             AdpNum,
             HopId * PATH_ENTRY_DW_SIZE,
             PATH_ENTRY_DW_SIZE,
             (UINT32 *)&PathEntry
             );
Exit:
  return Status;
}

/**
  Query Upstream Adapter number of Router.
  Used for TBT3 compatible operations.

  @param[in]  Router       - Pointer to Router instance.
  @param[out] UpAdpNum     - Pointer to Upstream Adapter number.

  @retval EFI_SUCCESS           - Query upstream adapter number successfully.
  @retval EFI_UNSUPPORTED       - Fail to query upstream adapter number.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CsRtQueryUpAdapter (
  IN PUSB4_ROUTER    Router,
  OUT UINT8          *UpAdpNum
  )
{
  EFI_STATUS     Status;
  USB4_DOMAIN    *Domain;
  USB4_CS_IO     *Usb4CsIo;

  DEBUG ((DEBUG_INFO, "Usb4CsRtQueryUpAdapter entry\n"));

  if ((Router == NULL) || (UpAdpNum == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CsRtQueryUpAdapter: Invalid parameter (%p,%p)\n", Router, UpAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsRtQueryUpAdapter: Null domain\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Usb4CsIo = Domain->Usb4CsIo;
  if (Usb4CsIo == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CsRtQueryUpAdapter: Null Usb4CsIo\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsIo->QueryRtUpAdp (Usb4CsIo, &(Router->TopologyId), UpAdpNum);
Exit:
  DEBUG ((DEBUG_INFO, "Usb4CsRtQueryUpAdapter exit\n"));
  return Status;
}