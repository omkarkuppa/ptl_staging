/** @file
  This file provides functions used to initialize address map.

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

#include <PiDxe.h>
#include <Uefi.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/SystemTopologyLib.h>
#include <Library/RegisterAddressMapLib.h>

STATIC SYSTEM_TOPOLOGY  *mTopology = NULL;
STATIC EFI_EVENT        mRuntimeVirtualAddressChangeEvent = NULL;

/**
  Get pointer to SYSTEM_TOPOLOGY stored in HOB or global variable.

  @return The address of SYSTEM_TOPOLOGY structure stored globally
  @return NULL if error occurred
**/
SYSTEM_TOPOLOGY*
EFIAPI
AddressMapGetTopology (
  VOID
  )
{
  EFI_HOB_GUID_TYPE *Hob;

  if (mTopology == NULL) {
    Hob = GetFirstGuidHob (&gSystemTopologyGuid);
    if (Hob == NULL) {
      return NULL;
    }

    mTopology = GET_GUID_HOB_DATA (Hob);
    if (mTopology->StructureVersion != RAT_TOPOLOGY_VERSION) {
      mTopology = NULL;
    }
  }

  return mTopology;
}

/**
  Set or update the address of SYSTEM_TOPOLOGY data structure in global variable.

  @param[in] Topology   (New) address of system topology data

  @return none
**/
VOID
EFIAPI
AddressMapSetTopology (
  IN SYSTEM_TOPOLOGY    *Topology
  )
{
  mTopology = Topology;
}

/**
  This is a notification function registered on EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
  It converts pointer to new virtual address.

  @param  Event        Event whose notification function is being invoked.
  @param  Context      Pointer to the notification function's context.

  @retval VOID

**/
VOID
EFIAPI
RuntimeVirtualAddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  SystemTopologyConvertPointers (EfiConvertPointer);
  EfiConvertPointer (0, (VOID **)&mTopology);
}

/**
  Make sure the register address map table workable in DXE and Runtime.

  Usually, this function should prepare initialization data and pass them to
  SystemTopologyInstallAddrMapData().

  @return EFI_UNSUPPORTED if this function is not supported or implemented
  @return EFI_SUCCESS     if the initialization is done successfully
**/
EFI_STATUS
EFIAPI
AddressMapTableInit (
  VOID
  )
{
  EFI_STATUS            Status;
  EFI_HOB_GUID_TYPE     *Hob;

  Hob = GetFirstGuidHob (&gSystemTopologyGuid);
  if (Hob == NULL) {
    //
    // No topology data installed.
    //
    return EFI_UNSUPPORTED;
  }

  //
  // Migrate the system topology data (replicate the HOB copy).
  //
  Status = SystemTopologyMigrateData (TRUE);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  RuntimeVirtualAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mRuntimeVirtualAddressChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

