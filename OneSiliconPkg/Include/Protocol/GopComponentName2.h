/** @file
  Protocol to retrieve the GOP driver version

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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
#ifndef _GOP_COMPONENT_NAME2_H_
#define _GOP_COMPONENT_NAME2_H_


typedef struct _GOP_COMPONENT_NAME2_PROTOCOL  GOP_COMPONENT_NAME2_PROTOCOL;

///
/// GOP Component protocol for retrieving driver name
///
typedef
EFI_STATUS
(EFIAPI *GOP_COMPONENT_NAME2_GET_DRIVER_NAME) (
  IN  GOP_COMPONENT_NAME2_PROTOCOL * This,
  IN  CHAR8                           *Language,
  OUT CHAR16                          **DriverName
  );

///
/// GOP Component protocol for retrieving controller name
///
typedef
EFI_STATUS
(EFIAPI *GOP_COMPONENT_NAME2_GET_CONTROLLER_NAME) (
  IN  GOP_COMPONENT_NAME2_PROTOCOL          * This,
  IN  EFI_HANDLE                               ControllerHandle,
  IN  EFI_HANDLE                               ChildHandle OPTIONAL,
  IN  CHAR8                                    *Language,
  OUT CHAR16                                   **ControllerName
  );

///
/// GOP Component protocol for retrieving driver version
///
typedef
EFI_STATUS
(EFIAPI *GOP_COMPONENT_NAME2_GET_DRIVER_VERSION) (
  IN  GOP_COMPONENT_NAME2_PROTOCOL          * This,
  IN  CHAR8                                    *Language,
  OUT CHAR16                                   **DriverVersion
  );

/**
  GOP Component protocol\n
  This protocol will be installed by GOP driver and can be used to retrieve GOP information.
**/
struct _GOP_COMPONENT_NAME2_PROTOCOL {
  GOP_COMPONENT_NAME2_GET_DRIVER_NAME      GetDriverName;          ///< Protocol function to get driver name
  GOP_COMPONENT_NAME2_GET_DRIVER_VERSION   GetDriverVersion;       ///< Protocol function to get driver version
  GOP_COMPONENT_NAME2_GET_CONTROLLER_NAME  GetControllerName;      ///< Protocol function to get controller name
  CHAR8                                    *SupportedLanguages;    ///< Number of Supported languages.
};

extern EFI_GUID gGopComponentName2ProtocolGuid;

#endif
