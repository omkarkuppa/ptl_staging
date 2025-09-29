/**@file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2003 Intel Corporation.

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

#include <SetupPrivate.h>
#include <Library/BaseMemoryLib.h>
#include <Register/PttPtpRegs.h>
#include <Protocol/Tcg2Protocol.h>

/**
   Initialize TCG Strings

   Populates the form's current TPM device strings based on the contents of the TCG2CONFIGURATION and
   TCG_SETUP variables populated by the TPM detection at runtime.

   @param[in] HiiHandle    HiiHandle Handle to HII database
   @param[in] Class        Indicates the setup class

**/
VOID
InitTcgStrings (
  EFI_HII_HANDLE HiiHandle,
  UINT16         Class
  )
{
  EFI_STATUS          Status;
  UINTN               VariableSize;
  UINT32              SetupVolVarAttr;
  UINT32              TpmStsFtif;
  EFI_TCG2_PROTOCOL   *Tcg2Protocol = NULL;

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));

  Status = gBS->LocateProtocol (
                 &gEfiTcg2ProtocolGuid,
                 NULL,
                 (VOID **)&Tcg2Protocol
           );

  //
  // Check if the Tcg2Protocol was installed
  //
  if ((Tcg2Protocol == NULL) ||
      (EFI_ERROR (Status))) {
    DEBUG ((DEBUG_ERROR, "Unable to locate TCG2 Protocol!\n"));
  }

  //
  // Check if gTpmErrorHobGuid was created.
  //
  if (GetFirstGuidHob (&gTpmErrorHobGuid) != NULL) {
    DEBUG ((DEBUG_ERROR, "gTpmErrorHobGuid exists!\n"));
  }

  //
  // Set initial value to error.
  //
  HiiSetString (HiiHandle, STRING_TOKEN (STR_TCG2_DEVICE_LOC_CONTENT), L"TPM Detection Error", NULL);

  //
  // Get SetupVolatileData Variable
  //
  VariableSize = sizeof (SETUP_VOLATILE_DATA);
  Status = gRT->GetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  &SetupVolVarAttr,
                  &VariableSize,
                  &mSetupVolatileData
                  );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return ;
  }

  //
  // Read TPM location from HW register
  //
  TpmStsFtif = MmioRead32 (R_PTT_TXT_STS_FTIF) & FTIF_FT_LOC_MASK;
  DEBUG ((DEBUG_INFO, "FTIF.LOC: 0x%x\n", TpmStsFtif));

  //
  // Store value into setup data structure for menu logic
  //
  mSetupVolatileData.TpmLocation = TpmStsFtif;

  //
  // Set SetupVolatileData Variable
  //
  Status = gRT->SetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  SetupVolVarAttr,
                  sizeof (mSetupVolatileData),
                  &mSetupVolatileData
                  );

  ASSERT_EFI_ERROR (Status);

  //
  // Populate the strings reported in the menu form based on TPM location reported by HW.
  //
  switch (TpmStsFtif) {
  case V_FTIF_SPI_DTPM_PRESENT:
    HiiSetString (
      HiiHandle,
      STRING_TOKEN (STR_TCG2_DEVICE_LOC_CONTENT),
      L"dTPM 2.0",
      NULL
      );
    break;

  case V_FTIF_FTPM_PRESENT:
    HiiSetString (
      HiiHandle,
      STRING_TOKEN (STR_TCG2_DEVICE_LOC_CONTENT),
      L"PTT",
      NULL
      );
    break;

  case V_FTIF_PSE_PRESENT:
    HiiSetString (
      HiiHandle,
      STRING_TOKEN (STR_TCG2_DEVICE_LOC_CONTENT),
      L"PSE",
      NULL
      );
    break;

  default:
    HiiSetString (
      HiiHandle,
      STRING_TOKEN (STR_TCG2_DEVICE_LOC_CONTENT),
      L"No TPM detected",
      NULL
      );
    break;
  }

  return;
}
