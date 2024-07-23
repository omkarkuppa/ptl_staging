/** @file
  HECI Control driver core operations for use in PEI, DXE and SMM.

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

#ifndef _HECI_CONTROL_CORE_LIB_H_
#define _HECI_CONTROL_CORE_LIB_H_

#include <Protocol/HeciControlProtocol.h>
#include <Protocol/HeciTransportProtocol.h>

/**
  HECI Control data instance definition.

  This protocol defines operations for HECI (Host Embedded Controller Interface)
  control access.
**/
typedef struct _HECI_CONTROL_PRIVATE_ {
  HECI_CONTROL        HeciControl;              ///< HeciControl Protocol/Ppi
} HECI_CONTROL_PRIVATE;

/**
  Initialize defined HECI communication interface (excluding ICH interface).

  @param[in]     This           HeciControl instance
  @param[in]     HeciTransport  HeciTransport instance

  @retval EFI_SUCCESS           Initialization succeed
  @retval EFI_INVALID_PARAMETER At least one of input parameter is NULL
**/
EFI_STATUS
EFIAPI
HeciControlInitalizeCommunicationDevice (
  IN      HECI_CONTROL_PRIVATE *This,
  IN      HECI_TRANSPORT       *HeciTransport
  );

/**
  Initialize all defined HECI communication interfaces (excluding ICH interfaces).

  @param[in]     This           HeciControl instance
**/
VOID
HeciControlInitalizeAllCommunicationDevices (
  IN      HECI_CONTROL_PRIVATE *This
  );

/**
  Initialize Specific HECI communication interface.

  @param[in]     This           HeciControl instance
  @param[in]     HeciNum        Heci Number
**/
VOID
HeciControlInitalizeSpecificCommunicationDevice (
  IN  HECI_CONTROL_PRIVATE *This,
  IN  UINT8                HeciNum
  );

/**
  Initialize HECI_CONTROL protocol functions

  @param[out]     HeciControl HECI control pointer to be initialized
**/
VOID
SetHeciControlFunctions (
  OUT HECI_CONTROL_PRIVATE *HeciControl
  );

#endif // _HECI_CONTROL_CORE_LIB_H_
