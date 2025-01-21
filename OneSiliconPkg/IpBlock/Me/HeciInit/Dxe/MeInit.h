/** @file
  Definitions of ME Initialize implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#ifndef _ME_INIT_H_
#define _ME_INIT_H_

/**
  Signal a event to save Me relevant registers and check if EOP is sent inside FSP Dispatch.
  This event must be run before ExitPmAuth.

  @param[in] Event                The event that triggered this notification function
  @param[in] Context              Pointer to the notification functions context

**/
VOID
EFIAPI
MeEndOfDxeEvent (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

/**
  Signal Me Ready To Boot Event
  Perform tasks below in the following order:
  1. Signal AmtReadyToBoot events for AMT platform hooks.
  2. Signal MePlatformReadyToBoot event for platform hooks.
  3. Report FWSTS and save to SMBIOS.
  4. Send EOP message.
  5. Lock config for manufacturing mode.
  6. Disable ME devices if particular ME-BIOS boot path requires this
     and this has not been done in PEI phase.

  @param[in] Event            The event that triggered this notification function
  @param[in] Context          Pointer to the notification functions context

**/
VOID
EFIAPI
MeReadyToBootEvent (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

/**
  To perform Me ReadyToBoot event function.
**/
VOID
MeReadyToBoot (
  VOID
  );

#endif
