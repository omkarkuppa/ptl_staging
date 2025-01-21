/** @file
  This file provides functions used to initialize/install address map.

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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include "SpecialRegisterAccess.h"

/**
  Get pointer to SYSTEM_TOPOLOGY stored in HOB or SYSTEM_INFO_VAR.

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
  SYSTEM_TOPOLOGY   *Topology;

  Topology = (SYSTEM_TOPOLOGY *)(UINTN)GetSystemInfoVar ()->SocAddrMapData;
  if (Topology == NULL) {
    Hob = GetFirstGuidHob (&gSystemTopologyGuid);
    if (Hob == NULL) {
      return NULL;
    }

    Topology = GET_GUID_HOB_DATA (Hob);
  }

  if (Topology->StructureVersion != RAT_TOPOLOGY_VERSION) {
    return NULL;
  }

  return Topology;
}

/**
  Set or update the address of SYSTEM_TOPOLOGY data structure in SYSTEM_INFO_VAR.

  @param[in] Topology   (New) address of system topology data

  @return none
**/
VOID
EFIAPI
AddressMapSetTopology (
  IN SYSTEM_TOPOLOGY    *Topology
  )
{
  GetSystemInfoVar ()->SocAddrMapData = (EFI_PHYSICAL_ADDRESS)(UINTN)Topology;
}

/**
  This function initializes SYSTEM_INFO_VAR Sideband access.

  @param[out]   P2SbController        Pointer to P2SB controller identification structure
  @param[out]   P2SbSidebandAccess    Pointer to Sideband access interface structure

  @return none
**/
STATIC
VOID
InitP2SbSidebandAccess (
  OUT P2SB_SIDEBAND_REGISTER_ACCESS   *P2SbSidebandAccess,
  OUT P2SB_CONTROLLER                 *P2SbController
  )
{
  EFI_STATUS      Status;

  ASSERT (P2SbSidebandAccess != NULL);
  ASSERT (P2SbController != NULL);

  Status = GetP2SbController (P2SbController);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  BuildP2SbSidebandAccess (
    P2SbController,
    0,
    0,
    0,
    0,
    FALSE,
    P2SbSidebandAccess
    );
}

/**
  This function builds SYSTEM_INFO_VAR Sideband access.

  @param[in]  SysInfoVar              The pointer to SYSTEM_INFO_VAR.

  @retval None
**/
STATIC
VOID
InitSbiAccessInfo (
  SYSTEM_INFO_VAR         *SysInfoVar
  )
{
  SysInfoVar->SbiAccess     = (EFI_PHYSICAL_ADDRESS)(UINTN)(SysInfoVar + 1);
  SysInfoVar->PchSbiAccess  = SysInfoVar->SbiAccess + sizeof(P2SB_SIDEBAND_REGISTER_ACCESS);
  SysInfoVar->CpuSbiAccess  = SysInfoVar->PchSbiAccess + sizeof(P2SB_SIDEBAND_REGISTER_ACCESS);
  SysInfoVar->P2SbCtrl      = SysInfoVar->CpuSbiAccess + sizeof(P2SB_SIDEBAND_REGISTER_ACCESS);

  InitP2SbSidebandAccess (
    (P2SB_SIDEBAND_REGISTER_ACCESS *)(UINTN)SysInfoVar->SbiAccess,
    (P2SB_CONTROLLER *)(UINTN)SysInfoVar->P2SbCtrl
    );

  InitP2SbSidebandAccess (
    (P2SB_SIDEBAND_REGISTER_ACCESS *)(UINTN)SysInfoVar->PchSbiAccess,
    (P2SB_CONTROLLER *)(UINTN)SysInfoVar->P2SbCtrl
    );

  BuildCpuSbiAccess (
    0,
    0,
    0,
    0,
    (P2SB_SIDEBAND_REGISTER_ACCESS *)(UINTN)SysInfoVar->CpuSbiAccess
    );
}

/**
  Fix up static pointers once PEI core has migrated NEM data to permanent memory.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
VOID
FixupSystemInfoVarPointers (
  VOID
  )
{
  SYSTEM_INFO_VAR     *SysInfoVar;

  SysInfoVar = GetSysInfoVarPointer();
  if (SysInfoVar->SbiAccess != (UINTN)(SysInfoVar + 1)) {
    InitSbiAccessInfo (SysInfoVar);
  }
}

/**
  Call back at Post-Memory to fix pointers in system topology data structure.

  @param[in] PeiServices        Describes the list of possible PEI Services
  @param[in] NotifyDescriptor   Pointer to notify descriptor
  @param[in] Ppi                Pointer to PPI bound with the call back

  @retval EFI_SUCCESS           System topology pointers updated
  @retval EFI_NOT_FOUND         Could not find topology data
**/
EFI_STATUS
EFIAPI
FixupMapPointers (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  FixupSystemInfoVarPointers ();
  return SystemTopologyFixupDataPointers (NULL);
}

CONST EFI_PEI_NOTIFY_DESCRIPTOR mAddrMapNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMemoryDiscoveredPpiGuid,
    FixupMapPointers
  }
};

/**
  Initialize register address map table.

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
  EFI_STATUS              Status;
  EFI_PHYSICAL_ADDRESS    MmcfgBases[MAX_SOCKET];
  UINT8                   Index;
  UINT8                   *StringTable;
  UINT8                   StringTableUnit;
  VOID                    *HobPtr;
  SYSTEM_INFO_VAR         *SysInfoVar;
  UINTN                   Size;

  //
  // Do not create another table if one has already been created
  //
  HobPtr = GetFirstGuidHob (&gSystemInfoVarHobGuid);
  if (HobPtr == NULL) {
    Size = sizeof(SYSTEM_INFO_VAR)
           + sizeof(P2SB_SIDEBAND_REGISTER_ACCESS) * 3
           + sizeof(P2SB_CONTROLLER);
    SysInfoVar = (SYSTEM_INFO_VAR *)BuildGuidHob (
                                      &gSystemInfoVarHobGuid,
                                      Size
                                      );
    if (SysInfoVar == NULL) {
      ASSERT (SysInfoVar != NULL);
      return EFI_OUT_OF_RESOURCES;
    }

    ZeroMem (SysInfoVar, Size);
    InitSbiAccessInfo (SysInfoVar);
  }

  SetSysInfoVarPointer (SysInfoVar);

  StringTable     = NULL;
  StringTableUnit = 0;

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    MmcfgBases[Index] = PcdGet64 (PcdPciExpressBaseAddress);
  }

//#if (RAT_TRACE_ENABLED && RAT_TRACE_WITH_STRING_TABLE)
  if (gSocAddressMapStringTable != NULL && gSocAddressMapStringTable[0] != NULL) {
    StringTable     = (UINT8 *)gSocAddressMapStringTable;
    StringTableUnit = STRING_TABLE_UNIT;
  }
//#endif

  Status = SystemTopologyInstallAddrMapData (
              gSocAddressMapTable,
              ((SOC_MAP_HEADER *)gSocAddressMapTable)->Length,
              StringTable,
              StringTableUnit,
              MmcfgBases,
              MAX_SOCKET
              );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  return PeiServicesNotifyPpi (mAddrMapNotifyList);
}

