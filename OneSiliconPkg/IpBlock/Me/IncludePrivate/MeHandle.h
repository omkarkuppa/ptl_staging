/** @file
  Header file for ME handle structure definition

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
#ifndef _ME_HANDLE_H_
#define _ME_HANDLE_H_

#include <Ppi/SiPolicy.h>
#include <MeDefines.h>
#include <PciSbdf.h>
#include <Library/P2SbSidebandAccessLib.h>

typedef struct _ME_HANDLE          ME_HANDLE;

/**
  CSME HECI Device Control Function

  @param[in] This                 The ME_HANDLE instance.
  @param[in] WhichDevice          Select of Me device
  @param[in] DeviceFuncCtrl       Function control
**/
typedef
VOID
(EFIAPI *SET_CSME_HECI_DEVICE_STATE) (
  IN ME_HANDLE                    *This,
  IN HECI_DEVICE                  WhichDevice,
  IN HECI_DEVICE_FUNC_CTRL        DeviceFuncCtrl
  );

/**
  Get CSME HECI devices sideband registers access

  @param[in]      This                    The ME_HANDLE instance.
  @param[in]      DeviceSel               Device to be configured
  @param[in, out] P2SbController          P2SB controller pointer
  @param[in, out] HeciPcrAccess           P2SB register access to ME devices
**/
typedef
VOID
(EFIAPI *GET_CSME_HECI_SIDEBAND_ACCESS) (
  IN     ME_HANDLE                     *This,
  IN     HECI_DEVICE                   DeviceSel,
  IN OUT P2SB_CONTROLLER               *P2SbController,
  IN OUT P2SB_SIDEBAND_REGISTER_ACCESS *HeciPcrAccess
  );

/**
  Get CSME HECI controller address that can be passed to the PCI Segment Library functions.

  @param[in] HeciFunc              HECI device function to be accessed.

  @retval HECI controller address in PCI Segment Library representation
**/
typedef
UINT64
(EFIAPI *GET_CSME_HECI_PCI_CFG_BASE) (
  IN HECI_DEVICE   HeciFunc
  );

/**
  Configure device interrupts

  @param[in] This                 The ME_HANDLE instance.
  @param[in] SiPolicy             Pointer to Si policy
  @param[in] HeciSbdf             HECI device SBDF

  @retval EFI_SUCCESS             The operation was successful
  @retval EFI_INVALID_PARAMETER   Wrong DeviceSel parameter
  @retval EFI_ABORTED             Interrupt pin programming not required
  @retval EFI_DEVICE_ERROR        Not able to write data
**/
typedef
EFI_STATUS
(EFIAPI *SET_CSME_HECI_INTERRUPT_PIN) (
  IN       ME_HANDLE     *This,
  IN CONST SI_POLICY_PPI *SiPolicy,
  IN       PCI_SBDF      HeciSbdf
  );

/**
  Get PSE HECI controller address that can be passed to the PCI Segment Library functions.

  @param[in] HeciFunc              HECI device function to be accessed.

  @retval HECI controller address in PCI Segment Library representation
**/
typedef
UINT64
(EFIAPI *GET_PSE_HECI_PCI_CFG_BASE) (
  IN PSE_HECI_DEVICE   HeciFunc
  );

typedef struct {
  GET_CSME_HECI_SIDEBAND_ACCESS  GetCsmeHeciSbAccess;      ///< Get CSME HECI sideband access
  GET_CSME_HECI_PCI_CFG_BASE     GetCsmeHeciPciCfgBase;    ///< Get CSME HECI PCI Configuration Base
  SET_CSME_HECI_DEVICE_STATE     SetCsmeHeciDeviceState;   ///< Set CSME HECI Device State (Enabled/Disabled)
  SET_CSME_HECI_INTERRUPT_PIN    SetCsmeHeciInterruptPin;  ///< Set CSME HECI Interrupt Pin
  GET_PSE_HECI_PCI_CFG_BASE      GetPseHeciPciCfgBase;     ///< Get PSE HECI PCI Configuration Base
} ME_CALLBACKS;

/**
  ME handle. Stores all data needed for ME IP initialization
**/
struct _ME_HANDLE {
  UINT32                  HeciTempMmio;                   ///< HECI Temporary BAR
  UINTN                   PmcPwrmBase;                    ///< PMC Base address
  UINT64                  XhciPciCfgBase;                 ///< XHCI controller address
  UINT8                   UsbrKvmPortNum;                 ///< USBr KVM port number
  UINT8                   UsbrStoragePortNum;             ///< USBr Storage redirection port number
  ME_CALLBACKS            *Callbacks;                     ///< Set of ME Callbacks
};

#endif// _ME_HANDLE_H_
