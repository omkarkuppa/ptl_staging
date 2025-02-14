/** @file
  Implements the interface for TbtNvmDrvRetimerThruHr class.
  This class is in charge of providing the way to access the retimer
  through TBT integrated HR.

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
#include <Protocol/TbtNvmDrvDef.h>
#include <Protocol/UsbCProgressCodeProtocol.h>
#include <Library/TbtNvmDrvRetimerThruHr.h>
#include <Library/TbtNvmRetimerDrvHelpers.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>

UINT16 TBT_USB4_PORT_CAPABILITY_OFFSET;

/**
  Write Retimer's IECS register
  Operation:
    Write the data to write to MSG_OUT_WDATA corresponding to the port.
    Write the command to the MSG_OUT_CMD corresponding to the port.
    Wait for command to complete, return the Status.

  @param[in] This      Pointer to the generic device interface - TBT_RETIMER
  @param[in] RegNum    Retimer's IECS register number to write to
  @param[in] DataPtr   Data pointer to write
  @param[in] Length    How many DWs to write

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS - indicate operation success
**/
TBT_STATUS
WriteIecs (
  IN TBT_RETIMER      *This,
  IN UINT8            RegNum,
  IN UINT32           *DataPtr,
  IN UINT8            Length
  )
{
  TBT_STATUS        TbtStatus;
  RETIMER_THRU_HR   *RetimerPtr;
  UINT32            USB4CapRegWrite;
  UINTN             RetryCnt;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_WRITE_IECS_LENGTH, (UINT32) Length, 0);

  if (Length > TBT_IECS_MAILBOX_LEGTH_IN_DW || Length == 0 || This == NULL) {
    ASSERT (Length <= TBT_IECS_MAILBOX_LEGTH_IN_DW && Length > 0);
    ASSERT (This != NULL);
    return TBT_STATUS_INVALID_PARAM;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBTCMD_WRITE_IECS_REG_NUM_LENGTH, (UINT32) RegNum, (UINT32) Length);

  RetimerPtr = (RETIMER_THRU_HR *)This->Impl;

  TbtStatus = RetimerPtr->Hr->WriteCioDevReg (
                                RetimerPtr->Hr,
                                ADAPTER_CONFIG_SPACE,
                                RetimerPtr->TbtPort,
                                TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
                                Length,
                                DataPtr
                                );
  if (TBT_STATUS_ERR (TbtStatus)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WRITE_IECS_DATA_ERROR, (UINT32) TbtStatus, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  USB4CapRegWrite = 0;
  for (RetryCnt = 0; RetryCnt < TBT_TOTAL_RETRIES_ON_MSG_OUT; RetryCnt++) {
    USB4CapRegWrite = (1 << TBT_USB4_PORT_CAPABILITY_PND_OFFSET)
                    | (1 << TBT_USB4_PORT_CAPABILITY_WnR_OFFSET)
                    | (RetimerPtr->CascadedRetimerIndex << TBT_USB4_PORT_CAPABILITY_RETIMER_INDEX_OFFSET)
                    | (2 << TBT_USB4_PORT_CAPABILITY_TARGET_OFFSET)
                    | ((Length << 2) << TBT_USB4_PORT_CAPABILITY_LENGTH_OFFSET)
                    | RegNum;

    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_WRITE_IECS_USB4_REG, USB4CapRegWrite, 0);
    TbtStatus = RetimerPtr->Hr->WriteCioDevReg (
                                  RetimerPtr->Hr,
                                  ADAPTER_CONFIG_SPACE,
                                  RetimerPtr->TbtPort,
                                  TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
                                  1,
                                  &USB4CapRegWrite
                                  );
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WRITE_IECS_DATA_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    TbtStatus = WaitForMsgOutTxDone (RetimerPtr, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, 0);
    if (TbtStatus == TBT_STATUS_NON_RECOVERABLE_ERROR || TbtStatus == TBT_STATUS_SUCCESS) {
      if (TbtStatus == TBT_STATUS_NON_RECOVERABLE_ERROR) {
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WRITE_IECS_FAIL, 0, 0);
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WRITE_IECS_REGNUM_DATA, (UINT32) RegNum, *DataPtr);
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WRITE_IECS_LENGTH_STATUS, (UINT32) Length, (UINT32) TbtStatus);
      }
      return TbtStatus;
    } else if (TbtStatus == TBT_STATUS_RETRY) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WRITE_IECS_RETRY_FAIL, 0, 0);
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WRITE_IECS_REGNUM_DATA, (UINT32) RegNum, *DataPtr);
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WRITE_IECS_LENGTH_STATUS, (UINT32) Length, (UINT32) TbtStatus);
    }
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WRITE_IECS_RETRY_REG, (UINT32) RetryCnt, 0);
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_WRITE_IECS_OFFSET_CMD, (UINT32) (TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1), (UINT32) RegNum);
  }

  return TbtStatus;
}

/**
  Read Retimer's IECS register
  Operation:
    Write the command to the MSG_OUT_CMD corresponding to the port.
    Wait for command to complete
    Read MSG_OUT_RDDATA and assign it to buffer pointed by Data
    Return Status

  @param[in]  This        Pointer to the generic device interface - TBT_RETIMER
  @param[in]  RegNum      Retimer's IECS register number to read from
  @param[in]  DataLength  How many DWs to read
  @param[out] DataPtr     Data pointer to a buffer where the read data will be assigned

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS - indicate operation success

  Limitation: read support only one DW
**/
TBT_STATUS
ReadIecs (
  IN  TBT_RETIMER    *This,
  IN  UINT8          RegNum,   // TODO: RegNum type should be enum
  IN  UINT32         DataLength,
  OUT UINT32         *DataPtr
  )
{
  TBT_STATUS        TbtStatus;
  RETIMER_THRU_HR   *RetimerPtr;
  UINT32            USB4CapRegWrite;
  UINTN             RetryCnt;

  if ((This == NULL) || (DataPtr == NULL)) {
    ASSERT (This != NULL);
    ASSERT (DataPtr != NULL);
    return TBT_STATUS_INVALID_PARAM;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBTCMD_READ_IECS_REG, (UINT32) RegNum, 0);
  RetimerPtr = (RETIMER_THRU_HR *)This->Impl;

  USB4CapRegWrite = 0;
  for (RetryCnt = 0; RetryCnt < TBT_TOTAL_RETRIES_ON_MSG_OUT; RetryCnt++) {
    USB4CapRegWrite = (1 << TBT_USB4_PORT_CAPABILITY_PND_OFFSET)
                    | (RetimerPtr->CascadedRetimerIndex << TBT_USB4_PORT_CAPABILITY_RETIMER_INDEX_OFFSET)
                    | (2 << TBT_USB4_PORT_CAPABILITY_TARGET_OFFSET)
                    | (DataLength << 2) << TBT_USB4_PORT_CAPABILITY_LENGTH_OFFSET
                    | RegNum;

    TbtStatus = RetimerPtr->Hr->WriteCioDevReg (
                                  RetimerPtr->Hr,
                                  ADAPTER_CONFIG_SPACE,
                                  RetimerPtr->TbtPort,
                                  TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
                                  1,
                                  &USB4CapRegWrite
                                  );
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_READ_IECS_DATA_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    TbtStatus = WaitForMsgOutTxDone (RetimerPtr, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, 1);
    if (TbtStatus == TBT_STATUS_NON_RECOVERABLE_ERROR) {
      return TbtStatus;
    } else if (TbtStatus == TBT_STATUS_SUCCESS) {
      break;
    }
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_READ_IECS_RETRYCNT, (UINT32) RetryCnt, 0);
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_READ_IECS_OFFSET_REG, (UINT32) (TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1), (UINT32) RegNum);
  }

  if (TbtStatus != TBT_STATUS_SUCCESS) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_READ_IECS_OPERATION_FAIL, (UINT32) TbtStatus, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  TbtStatus = RetimerPtr->Hr->ReadCioDevReg (
                                RetimerPtr->Hr,
                                ADAPTER_CONFIG_SPACE,
                                RetimerPtr->TbtPort,
                                TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_2,
                                DataPtr
                                );
  if (TBT_STATUS_ERR (TbtStatus)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_READ_IECS_DATA_ERROR, (UINT32) TbtStatus, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  return TBT_STATUS_SUCCESS;
}

/** Capability Parser.
  Check for offset of USB4 Capability.

  @param[in] This pointer to the generic device interface - TBT_RETIMER

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS - indicate operation success
**/
TBT_STATUS
CapabilityParsing (
  IN TBT_RETIMER *This
  )
{
  TBT_STATUS        TbtStatus;
  RETIMER_THRU_HR   *RetimerPtr;
  UINT32            Data;
  UINT8             NextCapability;
  UINT8             CapabilityID;
  UINT8             NoOfAttempts;
  UINT32            TimeDelay;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_CAPABILITY_PARSING, 0, 0);

  if (This == NULL) {
    ASSERT (This != NULL);
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_CAPABILITY_PARSING_DEV_NULL, 0, 0);
    return TBT_STATUS_INVALID_PARAM;
  }

  RetimerPtr   = (RETIMER_THRU_HR *)This->Impl;
  NoOfAttempts = 10;
  TimeDelay    = PcdGet32 (PcdUsbCCapabilityParsingTimeDelay);
  //WA for handling TBT controller responding after first read register attempt
  while (NoOfAttempts--) {
    RetimerPtr->Hr->ReadCioDevReg (RetimerPtr->Hr, DEVICE_CONFIG_SPACE, 0, 0x0, &Data);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_CAPABILITY_PARSING_READ_DATA, Data, 0);
      Data = Data & 0xFFFF;
      if ((Data == 0x8086) || (Data == 0x8087)) {
        break;
      }
    gBS->Stall (TimeDelay);
  }

  TbtStatus = RetimerPtr->Hr->ReadCioDevReg (
                                RetimerPtr->Hr,
                                ADAPTER_CONFIG_SPACE,
                                RetimerPtr->TbtPort,
                                0x1,
                                &Data
                                );
  if (TBT_STATUS_ERR (TbtStatus)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_CAPABILITY_PARSING_READ_ERROR, (UINT32) TbtStatus, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  NextCapability = 0x0;
  CapabilityID = 0x0;
  while (1) {
    NextCapability = ((UINT8)Data) & MASK (7, 0);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_CAPABILITY_PARSING_READ_NEXT, (UINT32) NextCapability, 0);

    if (NextCapability == 0x0) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_CAPABILITY_USB4_FOUND_ERROR, 0, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    TbtStatus = RetimerPtr->Hr->ReadCioDevReg (
                                  RetimerPtr->Hr,
                                  ADAPTER_CONFIG_SPACE,
                                  RetimerPtr->TbtPort,
                                  NextCapability,
                                  &Data
                                  );
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_CAPABILITY_PARSING_READ_ERROR, (UINT32) TbtStatus, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    CapabilityID = (UINT8)(Data >> 8);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_CAPABILITY_ID, (UINT32) CapabilityID, 0);

    if (CapabilityID == 0x6) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_CAPABILITY_USB4_OFFSET, (UINT32) NextCapability, 0);
      TBT_USB4_PORT_CAPABILITY_OFFSET = (UINT16)NextCapability;
      return TBT_STATUS_SUCCESS;
    }
  }
  CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_CAPABILITY_PARSING_FAIL, 0, 0);
  return TBT_STATUS_NON_RECOVERABLE_ERROR;
}

/**
  Indicate to the device about the flow state

  @param[in] This The pointer to the interface, where this function is defined.
  @param[in] State Indicate to the device about flow state, as it might be required to perform some steps.

**/
VOID
StateFromDriver (
  IN TBT_RETIMER      *This,
  IN DRIVER_STATE     State
  )
{
  if (State == AFTER_AUTH) {
    // Need to send ENUM again
    SendEnumCmd ((RETIMER_THRU_HR *)This->Impl);
  }
}

/**
  Initialize the required HW through SendEnumCmd and SendLsupCmdEn

  @param[in] This      Pointer to the generic device interface - TBT_RETIMER

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS               - indicate operation success
**/
TBT_STATUS
InitHW (
  IN TBT_RETIMER      *This
  )
{
  EFI_STATUS                   Status;
  RETIMER_THRU_HR              *RetimerPtr;
  USBC_PROGRESS_CODE_PROTOCOL  *UsbCProgressCodeProtocol;

  //
  // Locate UsbC Capsule Debug Progress Code Protocol
  //
  Status = gBS->LocateProtocol (&gUsbCCapsuleDebugProgressCodeProtocolGuid, NULL, (VOID**) &UsbCProgressCodeProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InitHW - Failed to locate UsbCProgressCodeProtocol (%r).\n", Status));
    return Status;
  }

  RetimerPtr = (RETIMER_THRU_HR *)This->Impl;

  // Enumerate retimer on a path to the target retimer
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBTCMD_SEND_ENUM_CMD, 0, 0);
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_ENUM_CMD);
  Status = SendEnumCmd (RetimerPtr);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_SEND_ENUM_CMD_FAIL, (UINT32) Status, 0);
    return Status;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBTCMD_SEND_LSUP_CMD, 0, 0);
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_LSUP_CMD);
  Status = SendLsupCmdEn (RetimerPtr);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_SEND_LSUP_CMD_FAIL, (UINT32) Status, 0);
    return Status;
  }

  return TBT_STATUS_SUCCESS;
}

/**
  Terminate the required HW through SendLsupCmdDis

  @param[in] This      Pointer to the generic device interface - TBT_RETIMER

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS               - indicate operation success
**/
TBT_STATUS
TerminateHW (
  IN TBT_RETIMER      *This
  )
{
  TBT_STATUS                   Status;
  RETIMER_THRU_HR              *RetimerPtr;
  USBC_PROGRESS_CODE_PROTOCOL  *UsbCProgressCodeProtocol;

  //
  // Locate UsbC Capsule Debug Progress Code Protocol
  //
  Status = gBS->LocateProtocol (&gUsbCCapsuleDebugProgressCodeProtocolGuid, NULL, (VOID**) &UsbCProgressCodeProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TerminateHW - Failed to locate UsbCProgressCodeProtocol (%r).\n", Status));
    return Status;
  }

  RetimerPtr = (RETIMER_THRU_HR *)This->Impl;
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBTCMD_SEND_USUP_CMD, 0, 0);
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_USUP_CMD);
  Status = SendLsupCmdDis (RetimerPtr);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_SEND_USUP_CMD_FAIL, 0, 0);
    return Status;
  }
  return TBT_STATUS_SUCCESS;
}

/**
  Destroy the given connection
  This module destructor.

  @param[in] This      Pointer to the generic device interface - TBT_RETIMER
**/
VOID
Dtor (
  IN TBT_RETIMER *This
  )
{
  RETIMER_THRU_HR   *RetimerPtr;

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBTCMD_DTOR_CALL, 0, 0);

  RetimerPtr = (RETIMER_THRU_HR *)This->Impl;

  // Destroy all resources
  RetimerPtr->Hr->Dtor (RetimerPtr->Hr);
  TbtNvmDrvDeAllocateMem (RetimerPtr);
  TbtNvmDrvDeAllocateMem (This);
}

/**
  Constructs the Retimer thru HR module.
  Initializes all the internal data structures and initialize the required HW.

  @param[in] FirmwareType         The type of update firmware.
  @param[in] TbtPcieRootPort      The parent pcie root port of discrete TBT device.
  @param[in] TbtPort              The port connected to the target retimer.
  @param[in] CascadedRetimerIndex The index of the retimer in the path from HR to the target retimer.
  @param[in] PcieRpConfig         PCIe root port config.
  @param[in] ForcePwrFunc         HR Force power function, applied if supplied.

  @retval A pointer to the device interface.

**/
TBT_RETIMER *
TbtNvmDrvRetimerThruHrCtor (
  IN UINT8          FirmwareType,
  IN PCIE_BDF       *TbtDmaPcieBdf,
  IN TBT_PORT       TbtPort,
  IN UINT32         CascadedRetimerIndex,
  IN PCIE_RP_CONFIG *PcieRpConfig,
  IN FORCE_PWR_HR   ForcePwrFunc OPTIONAL
  )
{
  EFI_STATUS        Status;
  RETIMER_THRU_HR   *RetimerPtr;
  TBT_RETIMER       *CommunicationPtr;
  TBT_HOST_ROUTER   *HrPtr;

  // Create all the resources
  RetimerPtr = TbtNvmDrvAllocateMem (sizeof (RETIMER_THRU_HR));
  if (RetimerPtr == NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_THRU_ALLOCATE_MEM_FAIL, 0, 0);
    return NULL;
  }
  CommunicationPtr = TbtNvmDrvAllocateMem (sizeof (TBT_RETIMER));
  if (CommunicationPtr == NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_DEV_ALLOCATE_MEM_FAIL, 0, 0);
    goto free_retimer;
  }
  HrPtr = TbtNvmDrvHrCtor (FirmwareType, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  if (HrPtr == NULL) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_CREATE_DMA_FAIL, 0, 0);
    goto free_communication;
  }

  // Assign communication interface implementation
  CommunicationPtr->Impl            = RetimerPtr;
  CommunicationPtr->WriteIecsReg    = WriteIecs;
  CommunicationPtr->ReadIecsReg     = ReadIecs;
  CommunicationPtr->Destroy         = Dtor;
  CommunicationPtr->IndicateState   = StateFromDriver;
  CommunicationPtr->InitHW          = InitHW;
  CommunicationPtr->TerminateHW     = TerminateHW;

  RetimerPtr->Comm = CommunicationPtr;

  if (TbtPort == PORT_A) {
    RetimerPtr->TbtPort = FIRST_MASTER_LANE;
  } else {
    RetimerPtr->TbtPort = SECOND_MASTER_LANE;
  }
  RetimerPtr->CascadedRetimerIndex = CascadedRetimerIndex;
  RetimerPtr->Hr = HrPtr;

  Status = CapabilityParsing (CommunicationPtr);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBTCMD_RT_CAPABILITY_PARSING_FAIL, (UINT32) Status, 0);
    goto free_hr;
  }

  return CommunicationPtr;

free_hr:
  HrPtr->Dtor (HrPtr);
free_communication:
  TbtNvmDrvDeAllocateMem (CommunicationPtr);
free_retimer:
  TbtNvmDrvDeAllocateMem (RetimerPtr);
  return NULL;
}
