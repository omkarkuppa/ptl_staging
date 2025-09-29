/**@file
  Platform Sanitization Dxe driver to perform TPM clear.

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

@par Specification Reference:
**/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/Tpm2CommandLib.h>
#include <Protocol/Tcg2Protocol.h>
#include <Register/PttPtpRegs.h>
#include <Library/IoLib.h>

/**
  Function to perform TPM clear operation.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.
**/
EFI_STATUS
TpmClearOperation (
  VOID
  )
{
  EFI_STATUS                Status;
  TPMS_AUTH_COMMAND         *AuthSession;

  AuthSession = NULL; // NULL indicates no platform auth change needed.

  DEBUG ((DEBUG_INFO, "PS: Tpm2ClearControl ... \n"));
  Status = Tpm2ClearControl (TPM_RH_PLATFORM, AuthSession, NO);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PS: Tpm2ClearControl Failed.\n"));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "PS: Tpm2Clear ... \n"));
  Status = Tpm2Clear (TPM_RH_PLATFORM, AuthSession);
  DEBUG ((DEBUG_INFO, "PS: Tpm2Clear Done, Status - %r\n", Status));

  return Status;
}

/**
  Function to trigger TPM Clear operation by invoking Tpm2ClearControl and Tpm2Clear when TPM is present.
  TPM Clear affects Endorsement keys, NV Indexes and objects stored in the TPM's Owner or
  Endorsement hierarchies (like bitlocker key / virtual smart card / fingerprint data etc...)
  TPM clear does not affect TPM measurements.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_UNSUPPORTED       If TPM is not present/enabled.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.
**/
EFI_STATUS
TriggerTpmClear (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_TCG2_PROTOCOL         *Tcg2;
  UINT32                    TpmStsFtif;

  //
  // Read TPM location from HW register
  //
  TpmStsFtif = MmioRead32 (R_PTT_TXT_STS_FTIF) & FTIF_FT_LOC_MASK;

  if (TpmStsFtif == V_FTIF_SPI_DTPM_PRESENT) {
    DEBUG ((DEBUG_INFO, "PS: dTPM 2.0 Present\n"));
  } else if (TpmStsFtif == V_FTIF_FTPM_PRESENT) {
    DEBUG ((DEBUG_INFO, "PS: PTT Present\n"));
  } else {
    DEBUG ((DEBUG_INFO, "PS: No TPM detected\n"));
    return EFI_DEVICE_ERROR;
  }

  // Locate gEfiTcg2ProtocolGuid. If installed then continue to perform TPM Clear.
  // If not not installed then return Not Ready.
  Status = gBS->LocateProtocol (&gEfiTcg2ProtocolGuid, NULL, (VOID **) &Tcg2);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PS: Tcg2 Protocol Not Found. Status - %r\n", Status));
    return EFI_NOT_READY;
  }

  Status = TpmClearOperation ();

  return Status;
}
