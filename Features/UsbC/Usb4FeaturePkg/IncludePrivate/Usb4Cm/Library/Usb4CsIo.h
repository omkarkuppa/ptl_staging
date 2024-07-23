/** @file
  Header file for USB4 configuration space access functions.

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

#ifndef _USB4_CS_IO_H_
#define _USB4_CS_IO_H_

#include <Uefi.h>
#include <Library/Usb4ConfigLayer.h>
#include <Library/Usb4ConfigSpace.h>

typedef struct _USB4_CS_IO   USB4_CS_IO;

//
// USB4_CS_IO functions
//

/**
  Reads register data with a given address from USB4 config space.
  Operation:
  1. Prepare the read request
  2. Swap bytes
  3. Calculate CRC and swap it too
  4. Read response and swap bytes of response data

  @param[in]  This        - Pointer to USB4_CS_IO function table.
  @param[in]  TopologyId  - Pointer to Topology ID of Router.
  @param[in]  ConfigSpace - Configuration space to be read.
  @param[in]  AdpNum      - Adapter number.
  @param[in]  Offset      - Data offset in the config space.
  @param[in]  Count       - DWORD count to be read from config space.
  @param[out] Data        - Data buffer for Read data.
  @param[out] RspAdpNum   - Pointer to the response adapter number (Required for TBT3 compatible operations).

  @retval EFI_SUCCESS           - USB4 config space register read success.
  @retval EFI_UNSUPPORTED       - Fail to read USB4 config space register.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
typedef
EFI_STATUS
(* USB4_CS_IO_READ) (
  IN  USB4_CS_IO           *This,
  IN  PTOPOLOGY_ID         TopologyId,
  IN  USB4_CONFIG_SPACE    ConfigSpace,
  IN  UINT8                AdpNum,
  IN  UINT16               Offset,
  IN  UINT8                Count,
  OUT UINT32               *Data,
  OUT UINT8                *RspAdpNum
  );

/**
  Write data to a given register in USB4 config space
  Operation:
  1. Prepare the write request
  2. Swap bytes
  3. Calculate CRC and swap it

  @param[in] This         - Pointer to USB4_CS_IO function table.
  @param[in] TopologyId   - Pointer to Topology ID of Router.
  @param[in] ConfigSpace  - Configuration space to be written.
  @param[in] AdpNum       - Adapter number
  @param[in] Offset       - Data offset in the config space.
  @param[in] Count        - DWORD count to be written to config space.
  @param[in] Data         - Data buffer for USB4 config space read.

  @retval EFI_SUCCESS           - Write data to USB4 register successfully.
  @retval EFI_UNSUPPORTED       - Fail to read USB4 config space register.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
typedef
EFI_STATUS
(* USB4_CS_IO_WRITE) (
  IN USB4_CS_IO           *This,
  IN PTOPOLOGY_ID         TopologyId,
  IN USB4_CONFIG_SPACE    ConfigSpace,
  IN UINT8                AdpNum,
  IN UINT16               Offset,
  IN UINT8                Count,
  IN UINT32               *Data
  );

/**
  Query Upstream Adapter number of Router.
  Used for TBT3 compatible operations.

  @param[in]  This         - Pointer to USB4_CS_IO instance.
  @param[in]  TopologyId   - Pointer to Topology ID of Router.
  @param[out] UpAdpNum     - Pointer to Upstream Adapter number.

  @retval EFI_SUCCESS      - Query Upstream Adapter number success.
  @retval other            - Query Upstream Adapter number failure.
**/
typedef
EFI_STATUS
(* USB4_CS_IO_QUERY_RT_UP_ADAPTER) (
  IN  USB4_CS_IO      *This,
  IN  PTOPOLOGY_ID    TopologyId,
  OUT UINT8           *UpAdpNum
  );

/**
  Increment reference count to host interface core.
  The host interface core can't be released if the reference count is non-zero.

  @param[in] This - Pointer to USB4_CS_IO function table.
**/
typedef
VOID
(* USB4_CS_IO_ADD_REF) (
  IN USB4_CS_IO    *This
  );

/**
  Decrement reference count to host interface core.
  The host interface core can't be released if the reference count is non-zero.

  @param[in] This - Pointer to USB4_CS_IO function table.
**/
typedef
VOID
(* USB4_CS_IO_RELEASE_REF) (
  IN USB4_CS_IO    *This
  );

//
// Function table for USB4 config space access
//
struct _USB4_CS_IO {
  USB4_CS_IO_READ                   CsRead;
  USB4_CS_IO_WRITE                  CsWrite;
  USB4_CS_IO_QUERY_RT_UP_ADAPTER    QueryRtUpAdp;
  USB4_CS_IO_ADD_REF                AddRef;
  USB4_CS_IO_RELEASE_REF            ReleaseRef;
};

#endif
