/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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


#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PciLib.h>
#include <Library/MeInfoLib.h>
#include <Register/HeciRegs.h>
#include <Protocol/LifeCycleState.h>
#include <Library/PreSiliconEnvDetectLib.h>

LIFE_CYCLE_STATE mLifeCycleState;

LIFE_CYCLE_STATE_PROTOCOL mLifeCycleStateProtocol = {
  LIFE_CYCLE_STATE_PROTOCOL_REVISION1,
  GetLifeCycleState
};

/**
  Retrieves life cycle state

  @param[in]   This              Protocol instance pointer.
  @param[out]  LifeCycleState    The life cycle state read from ME FW status.

  @retval      EFI_SUCCESS       The state was successfully retrived.

**/
EFI_STATUS
GetLifeCycleState (
  IN LIFE_CYCLE_STATE_PROTOCOL *This,
  OUT LIFE_CYCLE_STATE *LifeCycleState
  )
{
  *LifeCycleState = mLifeCycleState;
  return EFI_SUCCESS;
}

/**
  The life cycle state driver entry point.

  @param[in]  ImageHandle       Image handle of this driver
  @param[in]  SystemTable       A pointer to the EFI System Table.

  @return     EFI_SUCCESS       The protocol is installed successfully.
  @return     Others            Failed to initialize the driver.

**/
EFI_STATUS
LifeCycleStateDriverEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_HANDLE            LifeCycleStatHandle;
  HECI_FW_STS6_REGISTER MeFirmwareStatus;

  LifeCycleStatHandle = NULL;

  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS_6);

  if (IsSimicsEnvironment ()) {
    if (PciSegmentRead16 (MeHeciPciCfgBase (HECI1) + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
      DEBUG ((DEBUG_INFO, "HeciBaseAddress Not found! \n"));
      DEBUG ((DEBUG_INFO, "Temp set MeMode in Simics as normal mode! \n"));
      MeFirmwareStatus.r.FpfSocConfigLock = 0;
    }
  }

  mLifeCycleState = (MeFirmwareStatus.r.FpfSocConfigLock == 1) ? StateEndUser : StateManufacturing;

  //
  // Install on a new handle
  //
  Status = gBS->InstallProtocolInterface (
                  &LifeCycleStatHandle,
                  &gLifeCycleStateProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mLifeCycleStateProtocol
                  );

  DEBUG ((DEBUG_INFO, "LifeCycleState Protocol Installation Status = %r\n", Status));

  return Status;
}
