/** @file
  HECI utilities common module for use in PEI, DXE, and SMM.

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

@par Specification Reference:
**/

#include <Uefi.h>
#include <IndustryStandard/Pci30.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/HobLib.h>
#include <Library/TimerLib.h>
#include <Library/MeFwStsLib.h>
#include <Library/MeInfoLib.h>
#include <Register/HeciRegs.h>
#include <Register/PchRegs.h>
#include <MeDefines.h>
#include <MeState.h>
#include <MkhiMsgs.h>
#include <MeBiosPayloadHob.h>

/**
  Get ME Firmware Sku from ME Firmware Status 3 on HECI1

  @param [out] MeSku              Pointer to returned ME Sku information

  @retval EFI_SUCCESS             Success, returned ME Sku information is valid
  @retval EFI_INVALID_PARAMETER   Invalid input pointer
  @retval EFI_DEVICE_ERROR        Can't read Sku information
**/
EFI_STATUS
MeGetFirmwareSku (
  OUT UINT8 *MeSku
  )
{
  HECI_FW_STS3_REGISTER   MeFirmwareStatus3;

  if (MeSku == NULL)  {
    return EFI_INVALID_PARAMETER;
  }

  MeFirmwareStatus3.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS_3);
  if (MeFirmwareStatus3.ul == 0xFFFFFFFF) {
    MeFirmwareStatus3.ul = GetMeStatusFromFwstsHob (R_ME_CFG_HFS_3);
    if (MeFirmwareStatus3.ul == 0xFFFFFFFF) {
      return EFI_DEVICE_ERROR;
    }
  }

  *MeSku = (UINT8) MeFirmwareStatus3.r.FwSku;
  return EFI_SUCCESS;
}

/**
  Return ME Mode

  @param [out] MeMode             Pointer for ME Mode report

  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
  @retval EFI_DEVICE_ERROR        Read device is not HECI
**/
EFI_STATUS
MeBiosGetMeMode (
  OUT UINT32                       *MeMode
  )
{
  HECI_FWS_REGISTER MeFirmwareStatus;

  if (MeMode == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *MeMode = ME_MODE_FAILED;

  if (PciSegmentRead16 (MeHeciPciCfgBase (HECI1) + PCI_VENDOR_ID_OFFSET) != V_PCH_INTEL_VENDOR_ID) {
    return EFI_DEVICE_ERROR;
  }

  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);
  if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
    return EFI_DEVICE_ERROR;
  }

  switch (MeFirmwareStatus.r.MeOperationMode) {
    case ME_OPERATION_MODE_NORMAL:
      *MeMode = ME_MODE_NORMAL;
      break;

    case ME_OPERATION_MODE_DEBUG:
      *MeMode = ME_MODE_DEBUG;
      break;

    case ME_OPERATION_MODE_SOFT_TEMP_DISABLE:
      *MeMode = ME_MODE_TEMP_DISABLED;
      break;

    case ME_OPERATION_MODE_SECOVR_JMPR:
    case ME_OPERATION_MODE_SECOVR_HECI_MSG:
      *MeMode = ME_MODE_SECOVER;
      break;

    default:
      *MeMode = ME_MODE_FAILED;
  }

  DEBUG ((DEBUG_INFO, "[ME] MeMode %d (HECI-1 HFS: %08X)\n", *MeMode, MeFirmwareStatus.ul));

  return EFI_SUCCESS;
}

/**
  Get an abstract Intel HECI1 State from Firmware Status Register.
  This is used to control BIOS flow for different Intel ME functions.

  @param[out] MeStatus            Pointer for status report

  @retval EFI_SUCCESS             MeStatus copied
  @retval EFI_INVALID_PARAMETER   Pointer to MeStatus is invalid
  @retval EFI_NOT_FOUND           HECI1 Device hidden
**/
EFI_STATUS
EFIAPI
MeBiosGetMeStatus (
  OUT UINT32        *MeStatus
  )
{
  HECI_FWS_REGISTER MeFirmwareStatus;
  UINT64            HeciBaseAddress;

  if (MeStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  if (PciSegmentRead16 (HeciBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    REPORT_STATUS_CODE (EFI_ERROR_CODE, SW_HECI_GET_STATUS_ERROR | EFI_STATUS_TO_STATUS_CODE_VALUE (EFI_NOT_FOUND));
    *MeStatus = ME_NOT_READY;
    return EFI_NOT_FOUND;
  }

  MeFirmwareStatus.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);

  if (MeFirmwareStatus.r.CurrentState == ME_STATE_NORMAL && MeFirmwareStatus.r.ErrorCode == ME_ERROR_CODE_NO_ERROR) {
    *MeStatus = ME_READY;
  } else if (MeFirmwareStatus.r.CurrentState == ME_STATE_RECOVERY) {
    *MeStatus = ME_IN_RECOVERY_MODE;
  } else if (MeFirmwareStatus.r.CurrentState == ME_STATE_INIT) {
    *MeStatus = ME_INITIALIZING;
  } else if (MeFirmwareStatus.r.CurrentState == ME_STATE_DISABLE_WAIT) {
    *MeStatus = ME_DISABLE_WAIT;
  } else if (MeFirmwareStatus.r.CurrentState == ME_STATE_TRANSITION) {
    *MeStatus = ME_TRANSITION;
  } else {
    *MeStatus = ME_NOT_READY;
  }

  if (MeFirmwareStatus.r.FwUpdateInprogress) {
    *MeStatus |= ME_FW_UPDATES_IN_PROGRESS;
  }

  if (MeFirmwareStatus.r.FwInitComplete == ME_FIRMWARE_COMPLETED) {
    *MeStatus |= ME_FW_INIT_COMPLETE;
  }

  if (MeFirmwareStatus.r.MeBootOptionsPresent == ME_BOOT_OPTIONS_PRESENT) {
    *MeStatus |= ME_FW_BOOT_OPTIONS_PRESENT;
  }

  DEBUG ((DEBUG_INFO, "[HECI1] MeStatus 0x%x (Hfsts1: %08x)\n", *MeStatus, MeFirmwareStatus.ul));
  return EFI_SUCCESS;
}

/**
  Checks if Me FW is Client and it is ready

  @retval FALSE           Not ME Client or not ready
  @retval TRUE            Ready Client ME detected
**/
BOOLEAN
MeClientIsReady (
  VOID
  )
{
  HECI_FWS_REGISTER MeFirmwareStatus;

  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);

  if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
    return FALSE;
  }

  return (BOOLEAN) (MeFirmwareStatus.r.FwInitComplete != 0);
}

/**
  Checks if Me FW Sku indicated in Firmware Status 3 is Client

  @retval FALSE           Not Client ME SKU
  @retval TRUE            Client ME SKU detected
**/
BOOLEAN
MeSkuIsClient (
  VOID
  )
{
  UINT8 MeSku;

  if (!EFI_ERROR (MeGetFirmwareSku (&MeSku))) {
    if ((MeSku == ME_FWSTS3_FW_SKU_CONSUMER) ||
        (MeSku == ME_FWSTS3_FW_SKU_LITE) ||
        (MeSku == ME_FWSTS3_FW_SKU_CORPORATE)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Checks if Me FW is Client Corporate

  @retval FALSE           Not ME Client corporate
  @retval TRUE            Corporate Client ME detected
**/
BOOLEAN
MeClientIsCorporate (
  VOID
  )
{
  HECI_FWS_REGISTER       MeFirmwareStatus;
  HECI_FW_STS3_REGISTER   MeFirmwareStatus3;

  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);
  if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
    return FALSE;
  }

  MeFirmwareStatus3.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS_3);
  if (MeFirmwareStatus3.r.FwSku == ME_FWSTS3_FW_SKU_CORPORATE) {
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if Me FW is Client in recovery

  @retval FALSE           Client is not in recovery
  @retval TRUE            Client is in recovery
-**/
BOOLEAN
MeClientInRecovery (
  VOID
  )
{
  HECI_FWS_REGISTER MeFirmwareStatus;

  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);

  if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
    return TRUE;
  }

  if (MeFirmwareStatus.r.MeOperationMode == ME_OPERATION_MODE_SECOVR_JMPR) {
    return TRUE;
  }

  return FALSE;
}

/**
  Wait until Firmware Init Complete is set in Firmware Status Register.

  @retval EFI_SUCCESS             Firmware Init Complete bit is set
  @retval EFI_TIMEOUT             Firmware Init Complete bit is not set in 100 seconds
**/
EFI_STATUS
WaitFirmwareInitComplete (
  VOID
  )
{
  HECI_FWS_REGISTER      MeFirmwareStatus;
  UINT32                 Timeout;

  Timeout = FW_INIT_DONE_TIMEOUT * 1000;
  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);
  while (MeFirmwareStatus.r.FwInitComplete != 1) {
    if (Timeout == 0) {
      return EFI_TIMEOUT;
    }

    MicroSecondDelay (1000);
    Timeout--;
    MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);
  }

  return EFI_SUCCESS;
}
