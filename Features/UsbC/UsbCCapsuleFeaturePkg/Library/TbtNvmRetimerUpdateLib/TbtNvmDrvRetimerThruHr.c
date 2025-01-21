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

  DEBUG ((DEBUG_ERROR, "WriteIecs Length 0x%x\n", Length));

  if (Length > TBT_IECS_MAILBOX_LEGTH_IN_DW || Length == 0 || This == NULL) {
    ASSERT (Length <= TBT_IECS_MAILBOX_LEGTH_IN_DW && Length > 0);
    ASSERT (This != NULL);
    return TBT_STATUS_INVALID_PARAM;
  }

  DEBUG ((DEBUG_VERBOSE, "RetimerThruHr::WriteIecs: IECS reg num - 0x%x, Length - 0x%x\n", RegNum, Length));

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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::WriteIecs: ERROR when writing the data, TbtStatus - %d. \
                          Exiting...\n", TbtStatus));
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

    DEBUG ((DEBUG_INFO, "TbtNvmDrvRetimerThruHrWriteIecs: Writing USB 4.0 reg, USB4CapRegWrite = 0x%08x\n",
                          USB4CapRegWrite));
    TbtStatus = RetimerPtr->Hr->WriteCioDevReg (
                                  RetimerPtr->Hr,
                                  ADAPTER_CONFIG_SPACE,
                                  RetimerPtr->TbtPort,
                                  TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1,
                                  1,
                                  &USB4CapRegWrite
                                  );
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHr::WriteIecs: ERROR when writing the data, TbtStatus - %d. \
                            Exiting...\n", TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    TbtStatus = WaitForMsgOutTxDone (RetimerPtr, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, 0);
    if (TbtStatus == TBT_STATUS_NON_RECOVERABLE_ERROR || TbtStatus == TBT_STATUS_SUCCESS) {
       if (TbtStatus == TBT_STATUS_NON_RECOVERABLE_ERROR)
       DEBUG ((DEBUG_ERROR, "RetimerThruHr::WriteIecs WaitForMsgOutTxDone::failed: \
                             RegNum=%x, Data[0]=%x, Length=%d, TbtStatus - %d. \
                             Exiting...\n", RegNum, *DataPtr, Length, TbtStatus));
      return TbtStatus;
    } else if (TbtStatus == TBT_STATUS_RETRY) {
        DEBUG ((DEBUG_ERROR, "RetimerThruHr::WriteIecs TBT_STATUS_RETRY WaitForMsgOutTxDone::failed: \
                              RegNum=%x, Data[0]=%x, Length=%d, TbtStatus - %d. \
                              Exiting...\n", RegNum, *DataPtr, Length, TbtStatus));
    }
    DEBUG ((DEBUG_ERROR, "RetimerThruHr::WriteIecs: RetryCnt=%d, Off=%x, Cmd=%x\n",
           RetryCnt, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, RegNum));
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

  DEBUG ((DEBUG_VERBOSE, "RetimerThruHr::ReadIecs: IECS reg num - 0x%x\n", RegNum));

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
      DEBUG ((DEBUG_ERROR,
              "RetimerThruHr::ReadIecs: ERROR when reading from IECS rdata, TbtStatus - %d. Exiting...\n",
              TbtStatus));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    TbtStatus = WaitForMsgOutTxDone (RetimerPtr, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, 1);
    if (TbtStatus == TBT_STATUS_NON_RECOVERABLE_ERROR) {
      return TbtStatus;
    } else if (TbtStatus == TBT_STATUS_SUCCESS) {
      break;
    }
    DEBUG ((DEBUG_ERROR, "RetimerThruHr::ReadIecs: RetryCnt=%d, Off=%x, RegNum=%x\n",
            RetryCnt, TBT_USB4_PORT_CAPABILITY_OFFSET + PORT_CS_1, RegNum));
  }

  if (TbtStatus != TBT_STATUS_SUCCESS) {
    DEBUG ((DEBUG_ERROR,
            "RetimerThruHr::ReadIecs: Read IECS operation failed after several retries, TbtStatus - %d. Exiting...\n",
            TbtStatus));
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
    DEBUG ((DEBUG_ERROR,
            "RetimerThruHr::ReadIecs: ERROR when reading from IECS rdata, TbtStatus - %d. Exiting...\n",
            TbtStatus));
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

  DEBUG ((DEBUG_INFO, "RetimerThruHr::CapabilityParsing.\n"));
  if (This == NULL) {
    ASSERT (This != NULL);
    DEBUG ((DEBUG_ERROR, "RetimerThruHr::CapabilityParsing, NULL device provided.\n"));
    return TBT_STATUS_INVALID_PARAM;
  }

  RetimerPtr   = (RETIMER_THRU_HR *)This->Impl;
  NoOfAttempts = 10;
  TimeDelay    = PcdGet32 (PcdUsbCCapabilityParsingTimeDelay);
  //WA for handling TBT controller responding after first read register attempt
  while (NoOfAttempts--) {
    RetimerPtr->Hr->ReadCioDevReg (RetimerPtr->Hr, DEVICE_CONFIG_SPACE, 0, 0x0, &Data);
    DEBUG ((DEBUG_INFO, "RetimerThruHr::CapabilityParsing: Data read 0x%08x.\n", Data));
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
    DEBUG ((DEBUG_ERROR,
            "RetimerThruHr::CapabilityParsing: ERROR when reading from Adapter config space, TbtStatus - %d. \
             Exiting...\n", TbtStatus ));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  NextCapability = 0x0;
  CapabilityID = 0x0;
  while (1) {
    NextCapability = ((UINT8)Data) & MASK (7, 0);
    DEBUG ((DEBUG_INFO, "RetimerThruHr::CapabilityParsing: Read NextCapability = %x\n", NextCapability));

    if (NextCapability == 0x0) {
      DEBUG ((DEBUG_ERROR, "RetimerThruHr::CapabilityParsing: ERROR, USB4 Port Capability was not found.\n"));
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
      DEBUG ((DEBUG_ERROR,
            "RetimerThruHr::CapabilityParsing: ERROR when reading from Adapter config space, TbtStatus - %d. \
             Exiting...\n", TbtStatus ));
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }

    CapabilityID = (UINT8)(Data >> 8);
    DEBUG ((DEBUG_INFO, "RetimerThruHr::CapabilityParsing: Capability ID = 0x%x.\n", CapabilityID));

    if (CapabilityID == 0x6) {
      DEBUG ((DEBUG_INFO, "RetimerThruHr::CapabilityParsing: USB4 Port Capability found at offset 0x%x.\n", NextCapability ));
      TBT_USB4_PORT_CAPABILITY_OFFSET = (UINT16)NextCapability;
      return TBT_STATUS_SUCCESS;
    }
  }
  DEBUG ((DEBUG_ERROR, "RetimerThruHr::CapabilityParsing, failed to parse capabilities.\n"));
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
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_ENUM_CMD, 0, 0);
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_ENUM_CMD);
  Status = SendEnumCmd (RetimerPtr);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_ENUM_CMD_FAIL, (UINT32) Status, 0);
    DEBUG ((DEBUG_ERROR, "InitRetimerHW: The Retimer could not perform ENUM, Status %d\n", Status));
    return Status;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_LSUP_CMD, 0, 0);
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_LSUP_CMD);
  Status = SendLsupCmdEn (RetimerPtr);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_LSUP_CMD_FAIL, (UINT32) Status, 0);
    DEBUG ((DEBUG_ERROR, "InitRetimerHW: The Retimer could not perform LSUP, Status %d\n", Status));
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
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_USUP_CMD, 0, 0);
  UsbCProgressCodeProtocol->ShowProgressCode (USBC_DEBUG_PROGRESS_CODE_FEATURES_RETIMER_CAPSULE_USUP_CMD);
  Status = SendLsupCmdDis (RetimerPtr);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_TBTCMD_SEND_USUP_CMD_FAIL, (UINT32) Status, 0);
    DEBUG ((DEBUG_ERROR, "TerminateHW - Dtor Failed to send USUP to retimer.\n"));
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

  DEBUG ((DEBUG_INFO, "TbtNvmDrvRetimerThruHr::Dtor was called.\n"));

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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHrCtor: Failed to allocate memory for RETIMER_THRU_HR\n"));
    return NULL;
  }
  CommunicationPtr = TbtNvmDrvAllocateMem (sizeof (TBT_RETIMER));
  if (CommunicationPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHrCtor: Failed to allocate memory for DEVICE_COMMUNICATION_INTERFACE\n"));
    goto free_retimer;
  }
  HrPtr = TbtNvmDrvHrCtor (FirmwareType, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  if (HrPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHrCtor: Failed to create DMA\n"));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvRetimerThruHrCtor: The retimer could not perform Capability Parsing, \
                          Status %d. Exiting...\n", Status));
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
