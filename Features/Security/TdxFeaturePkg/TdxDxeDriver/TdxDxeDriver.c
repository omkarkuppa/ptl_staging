/** @file
  TDX Dxe driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuPlatformLib.h>
#include <TdxDataHob.h>

/**
  Function that uses DEBUG() macros to display the contents of Buffer.

  @param[in]  Buffer      A pointer to the Buffer.
  @param[in]  BufferSize  buffer size.

**/
VOID
PrintByteBuffer (
  IN UINT8  *Buffer,
  IN UINTN  BufferSize
  )
{
  UINTN     Index;
  UINT8     *Data8;

  Data8 = (UINT8 *) Buffer;
  for (Index = 0; Index < BufferSize; Index++) {
    DEBUG ((DEBUG_INFO, " %02x ", *Data8++));
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

/**
  TDX DXE Module Entry Point\n

  @param[in] ImageHandle          Image handle for this driver image
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             TDX DXE driver initialization completed successfully.
  @retval EFI_UNSUPPORTED         if TDX is disabled or not supported.
  @retval EFI_NOT_FOUND           if TDX data HOB was not found.
  @retval others                  Error conditions
**/
EFI_STATUS
EFIAPI
TdxDxeEntryPoint (
  IN  EFI_HANDLE       ImageHandle,
  IN  EFI_SYSTEM_TABLE *SystemTable
  )
{
  TDX_DATA_HOB        *TdxDataHobPtr = NULL;
  EFI_STATUS          Status;
  UINT8               *ActmMac = NULL;
  UINT32              MacOffset;
  UINT8               *ManifestAsBytes = NULL;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "[TDX] %a()\n", __FUNCTION__));

  if (IsSeamModeSupported () == FALSE) {
    DEBUG ((DEBUG_ERROR, "[TDX] Tdx is not supported.\n"));
    Status = EFI_UNSUPPORTED;
    return Status;
  }

  TdxDataHobPtr = (TDX_DATA_HOB *) GetFirstGuidHob (&gTdxDataHobGuid);
  if (TdxDataHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "[TDX] ERROR: TdxDataHob not found!\n"));
    Status = EFI_NOT_FOUND;
    return Status;
  }

  if (TdxDataHobPtr->TdxEnabled == FALSE) {
    DEBUG ((DEBUG_ERROR, "[TDX] TDX is disabled. Skipping TDX Dxe initialization...\n"));
    Status = EFI_UNSUPPORTED;
    return Status;
  }

  ActmMac = AllocateZeroPool (SIZE_OF_MANIFEST_MAC);
  if (ActmMac == NULL) {
    DEBUG ((DEBUG_ERROR, "[TDX] ERROR: AllocateZeroPool failure!\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  MacOffset = sizeof (TdxDataHobPtr->ActmDimmManifest.Header) + TdxDataHobPtr->ActmDimmManifest.Header.Size - SIZE_OF_MANIFEST_MAC;
  ManifestAsBytes = (UINT8 *) &TdxDataHobPtr->ActmDimmManifest;
  if ((MacOffset + SIZE_OF_MANIFEST_MAC) > sizeof (TdxDataHobPtr->ActmDimmManifest)) {
    FreePool (ActmMac);
    return EFI_BUFFER_TOO_SMALL;
  }
  CopyMem (ActmMac, &ManifestAsBytes[MacOffset], SIZE_OF_MANIFEST_MAC);

  DEBUG ((DEBUG_INFO, "[TDX] ACTM MAC = "));
  PrintByteBuffer (ActmMac, SIZE_OF_MANIFEST_MAC);

  Status = gRT->SetVariable (
    L"TdxActmMacNvVar",
    &gTdxActmMacNvVarGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
    (UINTN)SIZE_OF_MANIFEST_MAC,
    ActmMac
  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TDX] Set variable Status = %r\n", Status));
  }

  FreePool (ActmMac);
  return Status;
}
