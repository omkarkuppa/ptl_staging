/** @file

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

#ifndef  _MM_RUNTIME_UPDATE_LIB_H_
#define  _MM_RUNTIME_UPDATE_LIB_H_

#define MAX_CONTEXT_INDEX       0x20
#define MAX_CONTEXT_DATA_NAME   0x20

#ifndef EFI_MM_BOOT_MODE_INIT
#define EFI_MM_BOOT_MODE_INIT           0x00000001
#endif

#ifndef EFI_MM_BOOT_MODE_RUNTIME_UPDATE
#define EFI_MM_BOOT_MODE_RUNTIME_UPDATE 0x00000002
#endif

/**
  MM Update Context Suspend callback function.
  This function is called right before the Context Data is suspended.

  @param[in] Data            The context data.
  @param[in] DataSize        The Size of the context data.
  @param[in] Guid            The Guid of the context data.
  @param[in] Name            The Name of the context data.

  @return  EFI_SUCCESS       Callback function executes successfully.

**/
typedef
EFI_STATUS
(EFIAPI *MM_UPDATE_CONTEXT_SUSPEND_CALLBACK)(
  IN GUID                *Guid,
  IN CHAR16              *Name,
  IN UINTN               Size,
  IN VOID                *Data
  );

/**
  MM Update Context Resume callback function.
  This function is called right after the Context Data is resumed.

  @param[in] Data            The context data.
  @param[in] DataSize        The Size of the context data.
  @param[in] Guid            The Guid of the context data.
  @param[in] Name            The Name of the context data.

  @return  EFI_SUCCESS       Callback function executes successfully.

**/
typedef
EFI_STATUS
(EFIAPI *MM_UPDATE_CONTEXT_RESUME_CALLBACK)(
  IN GUID                *Guid,
  IN CHAR16              *Name,
  IN UINTN               Size,
  IN VOID                *Data
  );

/**
  Register the context data.

  The context data will be saved to the context area which is preserved across
  the MM runtime update.

  Context save is performed when gEfiMmSuspendForRuntimeUpdateProtocolGuid is
  notified before MM Update.
  Context resume is performed when gEfiMmResumeFromRuntimeUpdateProtocolGuid
  is notified after MM Update.

  Note:
  1. This function must be called in init phase.
  2. Saving Context with duplicated Guid and Name is not allowed. This function
  could only refuse duplicated context inside one driver. Developers should
  ensure the uniqueness of Guid and Name among all the updatable drivers.

  @param[in] Data              The context data.
  @param[in] DataSize          The Size of the context data.
  @param[in] Guid              The Guid of the context data.
  @param[in] Name              The Name of the context data.
  @param[in] SuspendCallback   Function to call before context data is suspended.
  @param[in] ResumeCallback    Function to call after context data is resumed.

  @return  EFI_SUCCESS            Succeed to register context data.
  @return  EFI_INVALID_PARAMETER  Data is NULL; DataSize is 0; Guid is NULL;
                                  Name is NULL; Context Data Count is over
                                  MAX_CONTEXT_INDEX; Guid and Name are duplicated.

**/
EFI_STATUS
EFIAPI
MmRuntimeUpdateRegisterContext (
  IN VOID                                *Data,
  IN UINTN                               DataSize,
  IN GUID                                *Guid,
  IN CHAR16                              *Name,
  IN MM_UPDATE_CONTEXT_SUSPEND_CALLBACK  SuspendCallback,  OPTIONAL
  IN MM_UPDATE_CONTEXT_RESUME_CALLBACK   ResumeCallback    OPTIONAL
  );

/**

  Get the context data from the context area which is preserved cross
  the MM runtime update.

  Note:
  1. This function must be called in init phase.
  2. Saving Context with duplicated Guid and Name is not allowed. This function
  could only refuse duplicated context inside one driver. Developers should
  ensure the uniqueness of Guid and Name among all the updatable drivers.

  @param[in] Name              The Name of the context data.
  @param[in] Data              The context data.
  @param[in] DataSize          The Size of the context data.
  @param[in] Guid              The Guid of the context data.

  @return  EFI_SUCCESS            Succeed to register context data.
  @return  EFI_INVALID_PARAMETER  Data is NULL; DataSize is 0; Guid is NULL;
                                  Name is NULL; Context Data Count is over
                                  MAX_CONTEXT_INDEX; Guid and Name are duplicated.

**/
EFI_STATUS
EFIAPI
MmRuntimeUpdateGetContext (
  IN CHAR16                          *Name,
  IN EFI_GUID                        *Guid,
  IN OUT UINTN                       *DataSize,
  OUT VOID                           *Data
  );


/**
  Set the context data to the context area which is preserved cross
  the MM runtime update.

  This function sets the context data to the context area. It is called
  by MM driver to save data in MM_SUSPEND_FOR_RUNTIME_UPDATE callback.


  @param[in] Name              The Name of the context data.
  @param[in] Data              The context data.
  @param[in] DataSize          The Size of the context data.
  @param[in] Guid              The Guid of the context data.

  @return  EFI_SUCCESS            Succeed to register context data.
  @return  EFI_INVALID_PARAMETER  Data is NULL; DataSize is 0; Guid is NULL;
                                  Name is NULL; Context Data Count is over
                                  MAX_CONTEXT_INDEX; Guid and Name are duplicated.

**/
EFI_STATUS
EFIAPI
MmRuntimeUpdateSetContext (
  IN CHAR16                          *Name,
  IN EFI_GUID                        *Guid,
  IN UINTN                           DataSize,
  IN VOID                            *Data
  );

/**
  Get the MM Boot Mode.

  @return  EFI_MM_BOOT_MODE_INIT             BIOS initialization phase.
  @return  EFI_MM_BOOT_MODE_RUNTIME_UPDATE   Runtime update phase.

**/
UINT32
EFIAPI
MmRuntimeUpdateGetBootMode (
  VOID
  );

/**
  Get the BIOS Version.

  @param[out] BiosVersion       The Bios Version out of the conversion.

**/
CHAR16 *
EFIAPI
DumpBiosVersion (VOID);

#endif
