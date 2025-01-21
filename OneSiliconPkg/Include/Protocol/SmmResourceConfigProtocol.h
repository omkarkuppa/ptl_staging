/** @file
  Protocol used for Configuring SMM Resources (IO / MSR)

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

@par Specification
**/
#ifndef _SMM_RESOURCE_CONFIG_PROTOCOL_H_
#define _SMM_RESOURCE_CONFIG_PROTOCOL_H_

extern EFI_GUID gSmmResourceConfigProtocolGuid;

/**
  This function configures IO port access through bitmap for all CPUs within SMM.
  ie Using this API, IO port can be configured as read write access or restricted access
  within SMM.
  For example: To deny access to IO port 0xCF8 to 0xCFB, invoke this API with
  PortBase = 0xCF8, PortLength = 4 and DenyRwAccess = TRUE

  @param[in] PortBase       IO Port base address.
  @param[in] PortLength     Length of IO Port bitmap that needs access configuration.
  @param[in] DenyRwAccess   0 = Allow RW Access to port; 1 = Deny RW Access to port.

  @retval      EFI_SUCCESS  Successfully enabled protection for specified IO range.
**/
typedef
EFI_STATUS
(EFIAPI *SMM_CONFIG_IO_BITMAP) (
  IN UINT16  PortBase,
  IN UINT16  PortLength,
  IN BOOLEAN DenyRwAccess
  );

/**
  This function configures MSR access through bitmap for all CPUs within SMM.
  Using this API, any MSR can be configured as Read only, Write only, Read/Write
  or no access with in SMM.

  For example:
    To configure MSR 0xE2, Invoke this API with MSR = 0xE2 and

      Case1: To revoke both read and write access
        DenyReadAccess = TRUE, DenyWriteAccess = TRUE.

      Case2: To revoke read access and allow Write access
        DenyReadAccess = TRUE, DenyWriteAccess = FALSE.

      Case3: To allow read access and revoke write access
        DenyReadAccess = FALSE, DenyWriteAccess = TRUE.

      Case4: To allow both read and Write access
        DenyReadAccess = FALSE, DenyWriteAccess = FALSE.

  DGR reference code allows access for only specified MSRs and denies
  access to rest of all MSRs. Refer mAllowedListMsr table structure at
  Intel\<Platform Sample Pkg>\Features\NiftyRock\PpamPlatformSmm\SmmIoMsrAccess.h for
  list of MSRs allowed for R/W access.

  OEM platform SMM driver can use this API to modify access to any MSR before
  SMM End of Dxe event (gEfiSmmEndOfDxeProtocolGuid).

  @param[in]  Msr          MSR base address.
  @param[in]  DenyReadAccess   FALSE - Allow read access, TRUE - Deny read access to MSR.
  @param[in]  DenyWriteAccess  FALSE - Allow write access, TRUE - Deny write access to MSR.

  @retval  EFI_SUCCESS            MSR Bitmap has been successfully configured.
  @retval  EFI_INVALID_PARAMETER  If MSR is not in the range of 0 to 0x1FFF and
                                  0xC0000000 to 0xC0001FFF.
  @retval  EFI_ACCESS_DENIED      If this protocol is invoked after SMM End of Dxe event.
**/
typedef
EFI_STATUS
(EFIAPI *SMM_CONFIG_MSR_BITMAP) (
  IN UINT32  Msr,
  IN BOOLEAN DenyReadAccess,
  IN BOOLEAN DenyWriteAccess
  );

/**
  This function allows a caller to install new IO Bitmap on top of the
  default IO Bitmap.

  @param[in] Buffer          Buffer address of IO Bitmap.
  @param[in] BufferLength    Length of the buffer. This should be 0x2000.

  @retval  EFI_SUCCESS            IO bitmap has been successfully installed.
  @retval  EFI_INVALID_PARAMETER  If Buffer length is not equal to 0x2000 (8KB).
  @retval  EFI_ACCESS_DENIED      If this protocol is invoked after SMM End of Dxe event.
**/
typedef
EFI_STATUS
(EFIAPI *SMM_INSTALL_IO_BITMAP) (
  IN UINTN   *Buffer,
  IN UINT16  BufferLength
  );

/**
  This function allows a caller to install new MSR Bitmap on top of the
  default MSR Bitmap.

  @param[in] Buffer          Buffer address of IO Bitmap.
  @param[in] BufferLength    Length of the buffer. This should be 0x1000.

  @retval  EFI_SUCCESS            MSR bitmap has been successfully installed.
  @retval  EFI_INVALID_PARAMETER  If Buffer length is not equal to 0x1000 (4KB).
  @retval  EFI_ACCESS_DENIED      If this protocol is invoked after SMM End of Dxe event.
**/
typedef
EFI_STATUS
(EFIAPI *SMM_INSTALL_MSR_BITMAP) (
  IN UINTN   *Buffer,
  IN UINT16  BufferLength
  );

/**
  The protocol allows the platform code to publish a set of configuration information that the
  CPU drivers will use to configure IO ports & MSR registers access with in SMM.
**/
typedef struct {
  SMM_CONFIG_IO_BITMAP           SmmConfigIoBitmap;
  SMM_CONFIG_MSR_BITMAP          SmmConfigMsrBitmap;
  SMM_INSTALL_IO_BITMAP          InstallIoBitmap;
  SMM_INSTALL_MSR_BITMAP         InstallMsrBitmap;
} SMM_RESOURCE_CONFIG_PROTOCOL;


/**
  Function for OEM Ring-3 Exception Handler

  @param[in] InterruptType          Defines which interrupt or exception to hook.
  @param[in] SystemContext          Pointer to EFI_SYSTEM_CONTEXT.

  @retval      EFI_SUCCESS  Ring-3 Exception Handling is successful.
**/
typedef
EFI_STATUS
(EFIAPI *EXCEPTION_HANDLER) (
  IN CONST  EFI_EXCEPTION_TYPE  InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT  SystemContext
  );

/**
  The protocol allows to handle OEM Ring-3 Exceptions.
**/
typedef struct {
  EXCEPTION_HANDLER           SpsRing3ExceptionHandler;
} SPS_RING3_EXCEPTION_HANDLER;

#endif
