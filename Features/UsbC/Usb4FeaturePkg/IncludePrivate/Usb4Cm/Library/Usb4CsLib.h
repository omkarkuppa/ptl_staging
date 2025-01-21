/** @file
  USB4 configuration space access library header.

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

#ifndef _USB4_CS_LIB_H_
#define _USB4_CS_LIB_H_

#include <Uefi.h>
#include <Library/Usb4RouterDefs.h>

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );
#endif
