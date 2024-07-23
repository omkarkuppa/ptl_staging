/** @file
  USB4 host interface core libary header.

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

#ifndef _USB4_HI_CORE_LIB_H_
#define _USB4_HI_CORE_LIB_H_

#include <Uefi.h>
#include <Library/Usb4ConfigLayer.h>
#include <Library/Usb4HiCoreDefs.h>
#include <Library/Usb4CsIo.h>
#include <Library/Usb4HrInst.h>

/**
  Create and initialize USB4 Host Interface for Tx/Rx Ring access.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Initialize USB4 host interface successfully.
  @retval EFI_UNSUPPORTED       - Fail to initialize USB4 host interface.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HiCoreCreate (
  IN  USB4_HR_INSTANCE    *Usb4Hr
  );

/**
  Destroy USB4 host interface

  @param[in] HiCore - Pointer to USB4 host interface core instance.
**/
VOID
Usb4HiCoreDestroy (
  IN USB4_HI_CORE    *HiCore
  );

/**
  Start USB4 host interface core.
  - Create Ring buffer and enable Tx/Rx Ring0 and release.

  @param[in] HiCore - Pointer to USB4 host interface core instance.
**/
EFI_STATUS
Usb4HiCoreStart (
  IN USB4_HI_CORE    *HiCore
  );

/**
  Query USB4_CS_IO associated with USB4 host interface core.

  @param[in]  HiCore   - Pointer to USB4 host interface core instance.
  @param[out] Usb4CsIo - Pointer of pointer to USB4_CS_IO.

  @retval EFI_SUCCESS           - Query USB4_CS_IO successfully.
  @retval EFI_NOT_FOUND         - USB4_CS_IO is not available.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HiQueryCsIo (
  IN  USB4_HI_CORE    *HiCore,
  OUT USB4_CS_IO      **Usb4CsIo
  );

/**
  Query Rx Ring for Rx event.

  @param[in]  HiCore     - Pointer to USB4 host interface core instance.
  @param[out] RxEvent    - Pointer to the received Rx Event.

  @retval EFI_SUCCESS           - A Rx event is received.
  @retval EFI_NOT_FOUND         - No Rx event is received.
  @retval EFI_UNSUPPORTED       - Fail to query Rx event for unexpected error.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HiQueryRxEvent (
  IN  USB4_HI_CORE    *HiCore,
  OUT RX_EVENT        *RxEvent
  );

#endif
