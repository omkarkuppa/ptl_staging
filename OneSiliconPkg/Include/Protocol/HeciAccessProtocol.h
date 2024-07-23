/** @file
  ME HECI Access Protocol - allows ME-UEFI FW communication over HECI in DXE and SMM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef __HECI_ACCESS_PROTOCOL_H__
#define __HECI_ACCESS_PROTOCOL_H__

#include <Uefi.h>
#include <Protocol/PciIo.h>

/**
  ME HECI Protocol GUID
  This Protocol provides an interface to communicate with Intel ME in DXE/SMM phase.
**/
extern EFI_GUID gHeciAccessProtocolGuid;
extern EFI_GUID gHeciAccessSmmProtocolGuid;

typedef struct _HECI_ACCESS_ HECI_ACCESS;

/**
  Get HECI Access Parameter

  @param[in]     This            Pointer to HECI Access device context structure
  @param[in,out] HeciAccessParam Pointer to HECI Access parameter

  @retval Operation EFI_STATUS
**/
typedef
EFI_STATUS
(EFIAPI *HECI_ACCESS_GET_PARAMETER) (
  IN      HECI_ACCESS   *This,
  IN OUT  UINTN         *HeciAccessParam
  );

/**
  Get next HECI Access

  @param[in] This     Pointer to HECI Access protocol instance from which the next instance should be returned.

  @retval Pointer to the next HeciAccess protocol instance
**/
typedef
HECI_ACCESS *
(EFIAPI *HECI_ACCESS_GET_NEXT_HECI) (
  IN HECI_ACCESS    *This
  );

/**
  PPI/PROTOCOL for ME-UEFI FW HECI Access.

  This protocol defines operations for HECI (Host Embedded Controller Interface)
  hardware access.
**/
typedef struct _HECI_ACCESS_ {
  EFI_PCI_IO_PROTOCOL               PciIo;                  /// < PciIo R/W HECI PCIe device
  HECI_ACCESS_GET_NEXT_HECI         GetNextHeciAccess;      /// < HECI access protocol walker
  HECI_ACCESS_GET_PARAMETER         GetHeciInterruptMode;   /// < Default HECI Interrupt mode to be used
} HECI_ACCESS;

#endif // __HECI_ACCESS_PROTOCOL_H__
