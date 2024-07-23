/** @file
  PPAM platform SMM API

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/SerialPortLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/SmmReadyToLock.h>
#include <Protocol/SmmCpu.h>
#include <Protocol/SmMonitorInit.h>
#include <Library/PcdLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/Msr.h>
#include <Protocol/SmmResourceConfigProtocol.h>
#include <Protocol/SmmSpaLogOutputProtocol.h>
#include "SmmIoMsrAccess.h"
#include <Library/CpuExceptionHandlerLib.h>
#include <Library/DgrInfoLib.h>

//
// Define the maximum debug message length
//
#define MAX_DEBUG_MESSAGE_LENGTH  0x100

//
// Define PPAM Manifest Type
//
#define PPAM_MANIFEST_2K  0x2
#define PPAM_MANIFEST_3K  0x3

EFI_SM_MONITOR_INIT_PROTOCOL             *mSmMonitorInitProtocol;
SMM_RESOURCE_CONFIG_PROTOCOL             *mSmmResourceConfigProtocol;
EFI_HANDLE                               mSmmResourceProtectHandle = NULL;

#if FixedPcdGetBool (PcdSpaEnable) == 1
/**
  SPA Log Output Protocol Interface.
  This function send the input string data through serial port.

  @param[in]  This          The pointer to the instance of the EFI_SPA_LOG_OUTPUT_PROTOCOL protocol.
  @param[in]  SpaLogString  SPA log string
**/
EFI_STATUS
EFIAPI
SpaLogOutputString (
  IN  EFI_SPA_LOG_OUTPUT_PROTOCOL  *This,
  IN  CHAR16                       *SpaLogString
  );

//
// SPA Log output protocol Interfaces.
//
EFI_SPA_LOG_OUTPUT_PROTOCOL SpaLogOutputProtocol = {
  SpaLogOutputString
};
#endif

/**
  This function is the Hook for OEM to handle Exception in SMI handler.
  OEM can modify this function as per their requirement.

  Note: It is OEMs responsibility to handle this function.
        If this function returns with Success then system will continue execution.
        If return value is Not Successful then system will enter CPU Deadloop.

  @param[in] InterruptType          Defines which interrupt or exception to hook.
  @param[in] SystemContext          Pointer to EFI_SYSTEM_CONTEXT.

  @retval   EFI_UNSUPPORTED  Intel RVP BIOS returns Unsupported.
**/
EFI_STATUS
EFIAPI
OemHookExceptionHandler (
  IN CONST  EFI_EXCEPTION_TYPE  InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT  SystemContext
  );

/**
  Ring 3 Exception Handler Interface
**/
GLOBAL_REMOVE_IF_UNREFERENCED
SPS_RING3_EXCEPTION_HANDLER gSpsRing3ExceptionHandlerProtocol = {
  OemHookExceptionHandler
};

/**
  This function is the Hook for OEM to handle Exception in SMI handler.
  OEM can modify this function as per their requirement.

  Note: It is OEMs responsibility to handle this function.
        If this function returns with Success then system will continue execution.
        If return value is Not Successful then system will enter CPU Deadloop.

  @param[in] InterruptType          Defines which interrupt or exception to hook.
  @param[in] SystemContext          Pointer to EFI_SYSTEM_CONTEXT.

  @retval   EFI_UNSUPPORTED  Intel RVP BIOS returns Unsupported.
**/
EFI_STATUS
EFIAPI
OemHookExceptionHandler (
  IN CONST  EFI_EXCEPTION_TYPE  InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT  SystemContext
  )
{
  //
  // Place Holder for OEM to handle Exception in SMI handler
  //
  return EFI_UNSUPPORTED;
}

/**
  Load PPAM image.

  @retval EFI_SUCCESS           PPAM is loaded to Buffer
  @retval EFI_BUFFER_TOO_SMALL  MSEG is too small
  @retval EFI_UNSUPPORTED       MSEG is not enabled
**/
EFI_STATUS
LoadPpamImage (
  UINT8 NrPpamVersion
  )
{
  EFI_STATUS     Status;
  VOID           *PpamImageBuffer;
  UINTN          PpamImageSize;
  EFI_GUID       *PpamGuid;

  if (NrPpamVersion >= NR_PPAM_11_SUPPORT) {
    DEBUG ((DEBUG_INFO, "Get GUID Pointer for PPAM 1.1\n"));
    PpamGuid = PcdGetPtr (PcdPpam11BinFile);
  } else {
    DEBUG((DEBUG_INFO, "PPAM Version is not supported\n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Extract PPAM image from FV
  //
  PpamImageBuffer = NULL;
  PpamImageSize = 0;

  Status = GetSectionFromAnyFv (
             PpamGuid,
             EFI_SECTION_RAW,
             0,
             &PpamImageBuffer,
             &PpamImageSize
             );
  if (EFI_ERROR (Status) || (PpamImageSize == 0)) {
    DEBUG ((DEBUG_ERROR, "Failed to get PPAM Image from FV\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = mSmMonitorInitProtocol->LoadMonitor ((EFI_PHYSICAL_ADDRESS) (UINTN) PpamImageBuffer, PpamImageSize);
  DEBUG ((DEBUG_INFO, "mSmMonitorInitProtocol->LoadMonitor - %r\n", Status));
  ASSERT_EFI_ERROR (Status);

  gBS->FreePool ((VOID *) ((UINTN) PpamImageBuffer));

  return Status;
}

/**
  Load PPAM Manifest.

  @param[in] PpamManifestType Type of the PPAM Manifest file.

  @retval EFI_SUCCESS           STM is loaded to MSEG
  @retval EFI_BUFFER_TOO_SMALL  MSEG is too small
  @retval EFI_OUT_OF_RESOURCES  Can not allocate the memory 
**/
EFI_STATUS
LoadPpamManifest (
  UINT8 PpamManifestType
  )
{
  EFI_STATUS     Status;
  VOID           *PpamManifestBuffer;
  VOID           *PpamManifestRuntimeBuffer;
  UINTN          PpamManifestSize;
  EFI_GUID       *PpamManifestGuid;

  if (PpamManifestType == PPAM_MANIFEST_2K) {
    DEBUG ((DEBUG_INFO, "Get GUID Pointer for PPAM 1.1 manifest\n"));
    PpamManifestGuid = PcdGetPtr (PcdPpam11ManifestBinFile);
  } else {
    DEBUG ((DEBUG_INFO, "Get GUID Pointer for PPAM 1.1 manifest 3k\n"));
    PpamManifestGuid = PcdGetPtr (PcdPpam11Manifest3kBinFile);
  }

  //
  // Extract PPAM Manifest from FV
  //
  PpamManifestBuffer = NULL;
  PpamManifestSize = 0;

  Status = GetSectionFromAnyFv (
             PpamManifestGuid,
             EFI_SECTION_RAW,
             0,
             &PpamManifestBuffer,
             &PpamManifestSize
             );
  if (EFI_ERROR (Status) || (PpamManifestSize == 0)) {
    DEBUG ((DEBUG_ERROR, "Failed to get PPAM Manifest from FV\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gBS->AllocatePool (EfiRuntimeServicesData, PpamManifestSize, &PpamManifestRuntimeBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate memory for PPAM Manifest\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "PpamManifestRuntimeBuffer - 0x%x\n", PpamManifestRuntimeBuffer));
  CopyMem (PpamManifestRuntimeBuffer, PpamManifestBuffer, PpamManifestSize);

  if (PpamManifestType == PPAM_MANIFEST_2K) {
    Status = gBS->InstallConfigurationTable (&gPpamManifestGuid, PpamManifestRuntimeBuffer);
  } else {
    Status = gBS->InstallConfigurationTable (&gPpamManifest3kGuid, PpamManifestRuntimeBuffer);
  }
  ASSERT_EFI_ERROR (Status);

  gBS->FreePool ((VOID *)((UINTN)PpamManifestBuffer));

  return Status;
}

/**
  Allow access to required IO Port within SMM after End Of Dxe using
  bitmap file from FV.

  @retval EFI_SUCCESS           Both read and write access are allowed on specified IOs.
  @retval EFI_UNSUPPORTED       Resource config protocol is not available.
**/
EFI_STATUS
UpdatePolicyIoBitmapfile (
  VOID
  )
{
  EFI_STATUS              Status;
  VOID                    *IoBitmapPolicyBuffer;
  UINTN                   IoBitmapPolicyBufferSize;

  //
  // Extract IO Bitmap policy binary from FV
  //
  IoBitmapPolicyBuffer = NULL;
  IoBitmapPolicyBufferSize = 0;

  Status = GetSectionFromAnyFv (
             PcdGetPtr (PcdSpsIoPolicyBitmap),
             EFI_SECTION_RAW,
             0,
             &IoBitmapPolicyBuffer,
             &IoBitmapPolicyBufferSize
             );
  if (EFI_ERROR (Status) || (IoBitmapPolicyBufferSize == 0)) {
    DEBUG ((DEBUG_ERROR, "Failed to get Sps IO Bitmap Policy from FV\n"));
    return Status;
  }

  Status = mSmmResourceConfigProtocol->InstallIoBitmap (IoBitmapPolicyBuffer, (UINT16) IoBitmapPolicyBufferSize);
  ASSERT_EFI_ERROR (Status);
  gBS->FreePool ((VOID *) ((UINTN)IoBitmapPolicyBuffer));

  return Status;
}

/**
  Allow access to required MSR within SMM after End Of Dxe using
  bitmap file from FV.

  @retval EFI_SUCCESS           Both read and write access are allowed on specified MSRs.
  @retval EFI_UNSUPPORTED       Resource config protocol is not available.
**/
EFI_STATUS
UpdatePolicyMsrBitmapfile (
  VOID
  )
{
  EFI_STATUS              Status;
  VOID                    *MsrBitmapPolicyBuffer;
  UINTN                   MsrBitmapPolicyBufferSize;

  //
  // Extract MSR Bitmap policy binary from FV
  //
  MsrBitmapPolicyBuffer = NULL;
  MsrBitmapPolicyBufferSize = 0;

  Status = GetSectionFromAnyFv (
             PcdGetPtr (PcdSpsMsrPolicyBitmap),
             EFI_SECTION_RAW,
             0,
             &MsrBitmapPolicyBuffer,
             &MsrBitmapPolicyBufferSize
             );
  if (EFI_ERROR (Status) || (MsrBitmapPolicyBufferSize == 0)) {
    DEBUG ((DEBUG_ERROR, "Failed to get MSR Bitmap Policy from FV\n"));
    return Status;
  }

  Status = mSmmResourceConfigProtocol->InstallMsrBitmap (MsrBitmapPolicyBuffer, (UINT16) MsrBitmapPolicyBufferSize);
  ASSERT_EFI_ERROR (Status);
  gBS->FreePool ((VOID *) ((UINTN)MsrBitmapPolicyBuffer));

  return Status;
}

/**
  Allow access to required IO Port within SMM after End Of Dxe using
  allowed list table from SmmIoMsrAccess.h.
**/
VOID
AllowSmmIoAccess (
  VOID
  )
{
  UINT16                         Index;
  UINT16                         NoOfEntries;

  // Update bitmap with allowed list IOs to allow access
  NoOfEntries = sizeof (mAllowedListIo) / sizeof (DGR_IO_TABLE);
  for (Index = 0; Index < NoOfEntries; Index++) {
    mSmmResourceConfigProtocol->SmmConfigIoBitmap (
                                  mAllowedListIo[Index].PortBase,
                                  mAllowedListIo[Index].PortLength,
                                  FALSE
                                  );
  }
}

/**
  Allow access to required MSR within SMM after End Of Dxe using
  allowed list table from SmmIoMsrAccess.h.
**/
VOID
AllowSmmMsrAccess (
  VOID
  )
{
  UINT16                         Index;
  UINT16                         NoOfEntries;

  // Update bitmap with allowed list MSRs to allow access
  NoOfEntries = sizeof (mAllowedListMsr) / sizeof (DGR_MSR_TABLE);
  for (Index = 0; Index < NoOfEntries; Index++) {
    mSmmResourceConfigProtocol->SmmConfigMsrBitmap (
                                  mAllowedListMsr[Index].Msr,
                                  mAllowedListMsr[Index].DenyReadAccess,
                                  mAllowedListMsr[Index].DenyWriteAccess
                                  );
  }
}

#if FixedPcdGetBool (PcdSpaEnable) == 1
/**
  Function to convert string from unicode to ASCII.

  @param[in]  StrUnicode          Input unicode string.
  @param[out] StrAscii            Output ASCII string
  @param[out] StrSize             String size
**/
VOID
ConvertToAscii (
  IN  CHAR16 *StrUnicode,
  OUT CHAR8  *StrAscii,
  IN  UINTN  StrSize
  )
{
  UINTN Index;

  ASSERT (StrAscii != NULL);
  ASSERT (StrUnicode != NULL);
  Index = 0;

  while ((*StrUnicode != 0) && (Index < StrSize)) {
    *StrAscii = (CHAR8) ReadUnaligned16 (StrUnicode);
    ++StrAscii;
    ++StrUnicode;
    Index++;
  }

  *StrAscii = 0;
}

/**
  SPA Log Output Protocol Interface.
  This function send the input string data through serial port.

  @param[in]  This          The pointer to the instance of the EFI_SPA_LOG_OUTPUT_PROTOCOL protocol.
  @param[in]  SpaLogString  SPA log string

  @retval EFI_SUCCESS       The function completed successfully
**/
EFI_STATUS
EFIAPI
SpaLogOutputString (
  IN  EFI_SPA_LOG_OUTPUT_PROTOCOL  *This,
  IN  CHAR16                       *SpaLogString
  )
{
#ifdef MDEPKG_NDEBUG
  CHAR8    Buffer [MAX_DEBUG_MESSAGE_LENGTH];
#endif

#ifndef MDEPKG_NDEBUG
  DEBUG ((EFI_D_ERROR, "%s \n", SpaLogString));
#else
  // Print this log without debug MACRO to support Release Build
  ConvertToAscii (SpaLogString, Buffer, sizeof (Buffer));
  //
  // Send the print string to a Serial Port
  //
  SerialPortWrite ((UINT8 *) Buffer, AsciiStrLen (Buffer));
#endif

  return EFI_SUCCESS;
}

/**
  Install SPA Log Output Protocol
**/
VOID
InstallSpaLogOutputProtocol (
  VOID
  )
{
  EFI_STATUS                       Status;
  EFI_HANDLE                       Handle;

  Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gEfiSpaLogOutputProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &SpaLogOutputProtocol
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SpaLogOutputProtocol failed to install.\n"));
  }

  return;
}
#endif

/**

  PPAM platform SMM driver entry point function.

  @param ImageHandle   image handle for this driver image
  @param SystemTable   pointer to the EFI System Table

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
InstallPpamPlatformSmm (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS            Status;
  UINT8                 NrPpamSupportLevel;

  NrPpamSupportLevel = NiftyRockSupportLevel ();
  if (NrPpamSupportLevel == NO_NR_PPAM_SUPPORT) {
    return EFI_UNSUPPORTED;
  }

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmMonitorInitProtocolGuid,
                    NULL,
                    (VOID **) &mSmMonitorInitProtocol
                    );
  if (EFI_ERROR (Status) || (mSmMonitorInitProtocol == NULL)) {
    return EFI_UNSUPPORTED;
  }

  Status = LoadPpamImage (NrPpamSupportLevel);
  if (!EFI_ERROR (Status)) {
    Status = LoadPpamManifest (PPAM_MANIFEST_2K);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to load PPAM manifest\n"));
      ASSERT_EFI_ERROR (Status);
    }
    Status = LoadPpamManifest (PPAM_MANIFEST_3K);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to load PPAM manifest 3K\n"));
      ASSERT_EFI_ERROR (Status);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "Failed to load PPAM\n"));
    ASSERT_EFI_ERROR (Status);
  }

//
// Restricting IO, MSR access is controlled on PPAM 11 supported SKUs
//
  if (NrPpamSupportLevel >= NR_PPAM_11_SUPPORT) {
    Status = gSmst->SmmLocateProtocol (
                      &gSmmResourceConfigProtocolGuid,
                      NULL,
                      (VOID **) &mSmmResourceConfigProtocol
                      );
    if (EFI_ERROR (Status) || (mSmmResourceConfigProtocol == NULL)) {
      return EFI_UNSUPPORTED;
    }
    if (PcdGetBool(PcdDgrPolicyOverride)) {
      Status = UpdatePolicyIoBitmapfile ();
      if (EFI_ERROR (Status)) {
        // In case if override bitmap policy fails, update with Allowedlist bitmap policy.
        AllowSmmIoAccess ();
      }
      Status = UpdatePolicyMsrBitmapfile ();
      if (EFI_ERROR (Status)) {
        // In case if override bitmap policy fails, update with Allowedlist bitmap policy.
        AllowSmmMsrAccess ();
      }
    } else {
      AllowSmmIoAccess ();
      AllowSmmMsrAccess ();
    }
  }

  //
  // Install Protocol for OEM Ring-3 Exception Handling
  //
  Status = gSmst->SmmInstallProtocolInterface (
                    &mSmmResourceProtectHandle,
                    &gSpsRing3ExceptionHandlerProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &gSpsRing3ExceptionHandlerProtocol
                    );
  ASSERT_EFI_ERROR (Status);

#if FixedPcdGetBool (PcdSpaEnable) == 1
  InstallSpaLogOutputProtocol ();
#endif

  return EFI_SUCCESS;
}
