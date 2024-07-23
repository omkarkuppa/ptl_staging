/** @file
  PEI DCI Init library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Register/DciRegs.h>
#include <Library/PeiDciInitLib.h>
#include <Library/HobLib.h>

/**
  Perform DCI configuration.

  @param[in]  DciHandle           Pointer to DCI handle structure
**/
VOID
DciConfiguration (
  IN  DCI_HANDLE                  *DciHandle
  )
{
  REGISTER_ACCESS                 *DciPcrAccess;
  PCH_DCI_PREMEM_CONFIG           *DciPreMemConfig;
  DCI_PRIVATE_CONFIG              *PrivateConfig;
  EFI_RESET_TYPE                  ResetType;
  UINT32                          Data32And;
  UINT32                          Data32Or;
  UINT32                          EctrlOrg;
  UINT32                          EctrlNew;

  Data32And = ~0u;
  Data32Or = 0;
  DciPcrAccess = DciHandle->DciPcrAccess;
  DciPreMemConfig = DciHandle->DciPreMemConfig;
  PrivateConfig = DciHandle->PrivateConfig;

  EctrlOrg = DciPcrAccess->Read32 (DciPcrAccess, R_DCI_PCR_ECTRL);
  ///
  /// Configure low power mode
  ///
  DciHandle->Callback->DciLpmConfig (DciPreMemConfig, DciPcrAccess);
  ///
  /// Configure Keep Early Trace
  ///
  switch (DciPreMemConfig->KeepEarlyTrace) {
    case KetDisabled:
      Data32And &= (UINT32)~B_DCI_PCR_ECTRL_KET;
      break;

    case KetEnabled:
      Data32Or |= B_DCI_PCR_ECTRL_KET;
      break;

    case KetNoChange:
      //
      // When KeepEarlyTrace is 2, which implies probe is connected, and KeepEarlyTrace is set by ITH tool, BIOS does not touch it
      //
      Data32And &= (UINT32)~B_DCI_PCR_ECTRL_KET;
      Data32Or |= (EctrlOrg & B_DCI_PCR_ECTRL_KET);
      break;

    default:
      break;
  }
  ///
  /// Configure USB DBC mode according to debug probe type
  ///
  switch (DciPreMemConfig->DciDbcMode) {
    case DciDbcDisabled:
      //
      // Disable USB DbC
      //
      if (EctrlOrg & B_DCI_PCR_ECTRL_DBGENABLE) {
        Data32And &= (UINT32)~(B_DCI_PCR_ECTRL_USB2DBCEN | B_DCI_PCR_ECTRL_USB3DBCEN);
      } else {
        //
        // if Debug_En = 0, don't clear USB3DBCEN which is set to 1 by HW default.
        //
        Data32And &= (UINT32)~(B_DCI_PCR_ECTRL_USB2DBCEN);
      }
      //
      // Disable USB3 Dbc disconnection flow on Sx entry.
      //
      if (DciHandle->Callback->ConnectUsb3dbcInSx != NULL) {
        DciHandle->Callback->ConnectUsb3dbcInSx ();
      }
      break;

    case DciDbcUsb2:
      //
      // Enable USB2 DbC only
      //
      if (EctrlOrg & B_DCI_PCR_ECTRL_DBGENABLE) {
        //
        // Clear USB3DBCEN only when Debug_EN = 1.
        // if Debug_EN = 0, don't touch USB3DBCEN.
        //
        Data32And &= (UINT32)~(B_DCI_PCR_ECTRL_USB3DBCEN);
      }
      Data32Or |= B_DCI_PCR_ECTRL_USB2DBCEN;
      //
      // Disable USB3 Dbc disconnection flow on Sx entry.
      //
      if (DciHandle->Callback->ConnectUsb3dbcInSx != NULL) {
        DciHandle->Callback->ConnectUsb3dbcInSx ();
      }
      break;

    case DciDbcUsb3:
      //
      // Enable USB3 DbC only
      //
      Data32And &= (UINT32)~(B_DCI_PCR_ECTRL_USB2DBCEN);
      Data32Or |= B_DCI_PCR_ECTRL_USB3DBCEN;
      //
      // Enable USB3 Dbc disconnection flow on Sx entry.
      //
      if (DciHandle->Callback->DisconnectUsb3dbcInSx != NULL) {
        DciHandle->Callback->DisconnectUsb3dbcInSx ();
      }
      break;

    case DciDbcBoth:
      //
      // Enable both USB2/3DBCEN
      //
      Data32Or |= (B_DCI_PCR_ECTRL_USB2DBCEN | B_DCI_PCR_ECTRL_USB3DBCEN);
      //
      // Enable USB3 Dbc disconnection flow on Sx entry.
      //
      if (DciHandle->Callback->DisconnectUsb3dbcInSx != NULL) {
        DciHandle->Callback->DisconnectUsb3dbcInSx ();
      }
      break;

    case DciDbcNoChange:
      //
      // Comply with HW value, keep it unchanged
      //
      Data32And &= (UINT32)~(B_DCI_PCR_ECTRL_USB2DBCEN | B_DCI_PCR_ECTRL_USB3DBCEN);
      Data32Or |= (EctrlOrg & (B_DCI_PCR_ECTRL_USB2DBCEN | B_DCI_PCR_ECTRL_USB3DBCEN));
      break;

    default:
      break;
  }

  ///
  /// Configure PLATFORM_DEBUG_EN_USER, set DCI_EN for user opt-in debug enabled
  ///
  if ((EctrlOrg & B_DCI_PCR_ECTRL_TOOLEN)) {
    if (GetBootModeHob () == BOOT_WITH_DEFAULT_SETTINGS) {
      //
      // Clean ECTRL[18] and HEEN
      //
      Data32And &= (UINT32)~B_DCI_PCR_ECTRL_TOOLEN;
      Data32And &= (UINT32)~B_DCI_PCR_ECTRL_HEEN;
    } else {
      //
      // Preserve HEEN
      //
      Data32Or &= (UINT32)~B_DCI_PCR_ECTRL_HEEN;
    }
  } else {
    if (DciPreMemConfig->DciEn) {
      Data32Or |= B_DCI_PCR_ECTRL_HEEN;
    } else {
      Data32And &= (UINT32)~B_DCI_PCR_ECTRL_HEEN;
    }
  }
  DciPcrAccess->AndThenOr32 (DciPcrAccess, R_DCI_PCR_ECTRL, Data32And, Data32Or);
  ///
  /// Set DCI EN Lock when Authorized Debug Enable is false
  ///
  if (PrivateConfig->AuthDebugEn == FALSE) {
    DciPcrAccess->Or32 (DciPcrAccess, R_DCI_PCR_ECTRL, B_DCI_PCR_ECTRL_HOST_EXI_EN_LOCK);
    DEBUG ((DEBUG_INFO, "Set DCI Enable Lock\n"));
  }
  EctrlNew = DciPcrAccess->Read32 (DciPcrAccess, R_DCI_PCR_ECTRL);
  DEBUG ((DEBUG_INFO, "ECTRL Old = 0x%08x\nECTRL New = 0x%08x\n", EctrlOrg, EctrlNew));

  if ((EctrlNew & R_DCI_PCR_ECTRL) == 0) {
    DciPcrAccess->And32 (DciPcrAccess, R_DCI_PCR_ECKPWRCTL, (UINT32)~B_DCI_PCR_ECKPWRCTL_RO_FAST_CLKGATE_DIS);
  }

  // Program Bus Number Setup register when PCH bus number non zero for USB2DBC
  if(EctrlNew & (B_DCI_PCR_ECTRL_DBGENABLE | B_DCI_PCR_ECTRL_USB2DBCEN)) {
    if (DciHandle->Callback->SetDbcBusNumber != NULL) {
      DciHandle->Callback->SetDbcBusNumber ();
      DEBUG ((DEBUG_INFO, "Set DBC.DBC PCH Bus Number\n"));
    }
  }

  if (PrivateConfig->ResetHandle == TRUE) {
    if ((EctrlOrg & B_DCI_PCR_ECTRL_DBGENABLE) != (EctrlNew & B_DCI_PCR_ECTRL_DBGENABLE)) {
      // if DEBUG_EN bit is set differently from previous boot, trigger global reset to ensure PMC update at the same time.
      ResetType = EfiResetPlatformSpecific;
      DciHandle->Callback->DciResetHandling (ResetType);
    } else if (EctrlNew & B_DCI_PCR_ECTRL_DBGENABLE) {
      // if DEBUG_EN bit is set and USB2DBCEN / USB3DBCEN bits are set differently from previous boot, trigger global reset to ensure PMC update at the same time.
      if ((EctrlOrg & (B_DCI_PCR_ECTRL_USB2DBCEN | B_DCI_PCR_ECTRL_USB3DBCEN)) !=
          (EctrlNew & (B_DCI_PCR_ECTRL_USB2DBCEN | B_DCI_PCR_ECTRL_USB3DBCEN))) {
        ResetType = EfiResetPlatformSpecific;
        DciHandle->Callback->DciResetHandling (ResetType);
      }
    }
  }
}
