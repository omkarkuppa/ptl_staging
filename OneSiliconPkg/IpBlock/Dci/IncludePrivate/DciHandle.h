/** @file
  DCI Handle structure

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

@par Specification
**/
#ifndef _DCI_HANDLE_H_
#define _DCI_HANDLE_H_

#include <RegisterAccess.h>
#include <DciConfig.h>
#include <Ppi/SiPolicy.h>

/**
  Internal DCI policy options
**/
typedef struct {
  BOOLEAN              AuthDebugEn;     ///< Determine if authorized debug is enabled by fuse
  BOOLEAN              ResetHandle;     ///< Determine if reset is required when DCI configuration bits changed
} DCI_PRIVATE_CONFIG;

/**
  DCI low power mode configuration

  @param[in]  *DciPreMemConfig    Instance of PCH_DCI_PREMEM_CONFIG
  @param[in]  *DciPcrAccess       Instance of DCI sideband access interface
**/
typedef
VOID
(*DCI_LPM_CONFIG) (
  IN  PCH_DCI_PREMEM_CONFIG      *DciPreMemConfig,
  IN  REGISTER_ACCESS            *DciPcrAccess
  );

/**
  DCI reset handling.

  @param[in]  ResetType           UEFI defined reset type
**/
typedef
VOID
(*DCI_RESET_HANDLING) (
  IN  EFI_RESET_TYPE              ResetType
  );

/**
  Set DBC bus number.
**/
typedef
VOID
(*SET_DBC_BUS_NUM) (
  VOID
  );

/**
  Enable USB3DBC disconnect in Sx
**/
typedef
VOID
(*EN_USB3DBC_DCNT) (
  VOID
  );

/**
  Disable USB3DBC disconnect in Sx
**/
typedef
VOID
(*DIS_USB3DBC_DCNT) (
  VOID
  );

/**
  DCI callbacks
  List of function pointers can be passed to IP Block driver
**/
typedef struct {
  DCI_RESET_HANDLING        DciResetHandling;
  DCI_LPM_CONFIG            DciLpmConfig;
  SET_DBC_BUS_NUM           SetDbcBusNumber;
  EN_USB3DBC_DCNT           DisconnectUsb3dbcInSx;
  DIS_USB3DBC_DCNT          ConnectUsb3dbcInSx;
} DCI_CALLBACK;

/**
  DCI handle structure
  Stores all data necessary to initialize DCI
**/
typedef struct {
  PCH_DCI_PREMEM_CONFIG      *DciPreMemConfig;
  REGISTER_ACCESS            *DciPcrAccess;  ///< DCI controller Private Config Register (PCR) space access
  DCI_CALLBACK               *Callback;
  DCI_PRIVATE_CONFIG         *PrivateConfig;
} DCI_HANDLE;

#endif // _DCI_HANDLE_H_
