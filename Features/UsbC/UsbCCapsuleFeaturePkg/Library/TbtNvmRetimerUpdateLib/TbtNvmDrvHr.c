/** @file
  Implements the interface for TbtNvmDrvHr class.
  This class represents TBT HR abstraction.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/HobLib.h>
#include <ITbtInfoHob.h>
#include <DTbtInfoHob.h>
#include <TbtNvmRetimer.h>
#include <Library/TbtNvmDrvHr.h>
#include <Library/Crc32c.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>

BOOLEAN   mTBTControllerWasPowered; // Check TBT controller power Status in CM mode

/**
  Send driver ready using DMA.

  @param[in] DmaPtr DMA instance to work with

  @retval TbtStatus:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS - indicate operation success
**/
TBT_STATUS
TbtNvmDrvHrSendDrvReady (
  TBT_DMA *DmaPtr
  )
{
  EFI_STATUS    Status;
  TBT_STATUS    TbtStatus;
  DRV_RDY_CMD   Message;
  UINT32        Crc32;

  if (DmaPtr == NULL) {
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  // Prepare the driver ready message to be send
  Message.reqCode = SwapBytes32 (CC_DRV_READY);
  Status = CalCrc32c ((UINT8 *)&Message, OFFSET_OF (DRV_RDY_CMD, crc), &Crc32);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_GEN_CRC_FAIL, (UINT32) Status, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  Message.crc = SwapBytes32 (Crc32);
  // Send the message
  TbtStatus = DmaPtr->TxCfgPkt (DmaPtr, PDF_SW_TO_FW_COMMAND, sizeof(Message), (UINT8 *)&Message);
  if (TBT_STATUS_ERR (TbtStatus)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_RDY_CMD_SEND_FAIL, (UINT32) TbtStatus, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  // Make sure response is received
  TbtStatus = DmaPtr->RxCfgPkt (DmaPtr, PDF_FW_TO_SW_RESPONSE, NULL, NULL);
  if (TBT_STATUS_ERR (TbtStatus)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_RDY_CMD_RECEIVE_FAIL, (UINT32) TbtStatus, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CMD_SUCCESS, 0, 0);
  return TBT_STATUS_SUCCESS;
}

/* Write request pointer to reuse in TbtNvmDrvHrWriteCioReg only */
STATIC WRITE_CONFIGURATION_REGISTERS_REQUEST *mWriteDevReq = NULL;
/* Buffer to be used in TbtNvmDrvHrWriteCioReg. Predefined as optimization */
STATIC WRITE_CONFIGURATION_REGISTERS_REQUEST mWriteReqSwapped;

/**
  Write register to TBT CIO config space

  @param[in]  This               Pointer to the TBT_HOST_ROUTER structure on which the method is invoked
  @param[in]  ConfigurationSpace TBT device config space.
  @param[in]  Adapter            Adapter to be accessed.
  @param[in]  RegNum             Register offset in TBT device config space.
  @param[in]  Length             How many byte to write
  @param[in]  DataPtr            Data pointer to write

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS - indicate operation success
**/
TBT_STATUS
TbtNvmDrvHrWriteCioReg (
  IN TBT_HOST_ROUTER *This,
  IN UINT8           ConfigurationSpace,
  IN UINT8           Adapter,
  IN UINT16          RegNum,
  IN UINT8           Length,
  IN UINT32          *DataPtr
  )
{
  EFI_STATUS   Status;
  TBT_STATUS   TbtStatus;
  UINT16       DataPtrIndex;
  UINT32       Len;
  UINT32       Crc32;
  UINT16       ExpectedLength;

  if ( (DataPtr == NULL)
    || (This == NULL)
    || (Length <= 0)
    || (Length > 16))
  {
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  DEBUG_CODE_BEGIN ();

  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBT_DRV_WRITE_CIO_REG, (UINT32) ConfigurationSpace, 0);
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBT_DRV_WRITE_CIO_CONFIG, (UINT32) Adapter, (UINT32) RegNum);
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBT_DRV_WRITE_CIO_DATA, (UINT32) Length, *DataPtr);

  for (DataPtrIndex = 0; DataPtrIndex < Length; DataPtrIndex++) {
    DEBUG ((DEBUG_VERBOSE, "%x ", *(DataPtr + DataPtrIndex)));
  }
  DEBUG ((DEBUG_VERBOSE, "\n"));

  DEBUG_CODE_END ();

  ASSERT (Length > 0 && Length <= 16);

  if (mWriteDevReq == NULL) {  // If not allocated yet
    mWriteDevReq = TbtNvmDrvAllocateMem (sizeof(WRITE_CONFIGURATION_REGISTERS_REQUEST));
    if (mWriteDevReq == NULL) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_WRITE_ALLOCATE_MEM_ERROR, 0, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Initialize constant values for read request
    mWriteDevReq->RouteString = 0x0;    // HR
    mWriteDevReq->SequenceNumber = 0;   // Always use the same one, no two outstanding requests anyway;
  }
  mWriteDevReq->ConfigurationSpace = ConfigurationSpace;
  mWriteDevReq->Adapter = Adapter;
  mWriteDevReq->DwIndex = RegNum;
  mWriteDevReq->Length  = Length;
  for (DataPtrIndex = 0; DataPtrIndex < Length; DataPtrIndex++) {
    mWriteDevReq->WrData[DataPtrIndex] = *(DataPtr + DataPtrIndex);
  }
  // DataPtrIndex - points after the data

  // Swap the data
  Len = OFFSET_OF (WRITE_CONFIGURATION_REGISTERS_REQUEST, WrData) + DataPtrIndex * 4; // Length in Byte without CRC
  if (Len > sizeof (WRITE_CONFIGURATION_REGISTERS_REQUEST)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_AVOID_BUFFER, 0, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  TbtNvmDrvSwapEndianess ((UINT32 *)mWriteDevReq, Len / 4, (UINT32 *)&mWriteReqSwapped);
  // Calculate CRC
  Status = CalCrc32c ((UINT8 *)&mWriteReqSwapped, Len, &Crc32);
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_WRITE_CIO_GEN_CRC_FAIL, (UINT32) Status, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  mWriteReqSwapped.WrData[DataPtrIndex] = SwapBytes32 (Crc32);    // Insert CRC
  // Transmit the request
  TbtStatus = This->Impl->pDma->TxCfgPkt (
                                  This->Impl->pDma,
                                  PDF_WRITE_CONFIGURATION_REGISTERS,
                                  OFFSET_OF (WRITE_CONFIGURATION_REGISTERS_REQUEST, WrData) + Length * 4 + 4,  // RS + CMD + WRDATA + CRC
                                  (UINT8 *)&mWriteReqSwapped
                                  );
  if (TBT_STATUS_ERR (TbtStatus)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_TRANSMIT_DMA_ERROR, (UINT32) RegNum, (UINT32) Length);
    goto error_while_writing;
  }
  ExpectedLength = (UINT16)sizeof(WRITE_CONFIGURATION_REGISTERS_RESPONSE);
  // Receive the response
  TbtStatus = This->Impl->pDma->RxCfgPkt (
                                  This->Impl->pDma,
                                  PDF_WRITE_CONFIGURATION_REGISTERS,
                                  &ExpectedLength,
                                  NULL                                              // No data in the response
                                  );
  if (TBT_STATUS_ERR (TbtStatus)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_RECEIVE_ERROR, (UINT32) RegNum, (UINT32) Length);
    goto error_while_writing;
  }

  return TBT_STATUS_SUCCESS;

error_while_writing:
  This->Impl->pDma->DbgPrint (This->Impl->pDma);
  return TBT_STATUS_NON_RECOVERABLE_ERROR;
}

/* Read request pointer to reuse in TbtNvmDrvHrReadCioDevReg only */
STATIC READ_CONFIGURATION_REGISTERS_REQUEST *mReadDevReq = NULL;

/* Buffer to be used in TbtNvmDrvHrReadCioDevReg. Predefined as optimization */
STATIC READ_CONFIGURATION_REGISTERS_REQUEST mReadReqSwapped;

/* Buffer to be used in TbtNvmDrvHrReadCioDevReg. Predefined as optimization */
STATIC READ_CONFIGURATION_REGISTERS_RESPONSE mReadReqResponse;

/**
  Reads the a register with a given address from TBT device config space.
  Operation:
    Prepare the read request
    Swap bytes
    Calculate CRC and swap it too

  @param[in]  This               Pointer to the TBT_HOST_ROUTER structure on which the method is invoked
  @param[in]  ConfigurationSpace TBT device config space.
  @param[in]  Adapter            Adapter to be accessed.
  @param[in]  RegNum             Register offset in TBT device config space.
  @param[out] DataPtr            Read data, one DW.

**/
TBT_STATUS
TbtNvmDrvHrReadCioDevReg (
  IN  TBT_HOST_ROUTER *This,
  IN  UINT8           ConfigurationSpace,
  IN  UINT8           Adapter,
  IN  UINT16          RegNum,
  OUT UINT32          *DataPtr
  )
{
  EFI_STATUS Status;
  UINT32     Len;
  UINT32     Crc32;
  UINT32     ReadData;
  UINT16     ExpectedLength;

  if ( (DataPtr == NULL)
    || (This == NULL) )
  {
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBT_DRV_READ_CIO_CONFIG, (UINT32) ConfigurationSpace, 0);
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBT_DRV_READ_CIO_CONFIG2, (UINT32) Adapter, (UINT32) RegNum);

  if (mReadDevReq == NULL) {  // If not allocated yet
    mReadDevReq = TbtNvmDrvAllocateMem (sizeof(READ_CONFIGURATION_REGISTERS_REQUEST));
    if (mReadDevReq == NULL) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_READ_CIO_ALLOCATE_MEM_ERROR, 0, 0);
      return TBT_STATUS_NON_RECOVERABLE_ERROR;
    }
    // Initialize constant values for read request
    mReadDevReq->RouteString = 0x0;    // HR
    mReadDevReq->SequenceNumber = 0;   // Always use the same one, no two outstanding requests anyway;
    mReadDevReq->Length = 1;          // Always read only one register
  }
  mReadDevReq->ConfigurationSpace = ConfigurationSpace;
  mReadDevReq->Adapter = Adapter;
  mReadDevReq->DwIndex = RegNum;
  // Swap the data
  Len = (UINT32) ((UINTN) &(mReadReqSwapped.Crc) - (UINTN) &mReadReqSwapped);
  if (Len > sizeof(READ_CONFIGURATION_REGISTERS_REQUEST)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_READ_AVOID_BUFFER, 0, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  TbtNvmDrvSwapEndianess ((UINT32 *)mReadDevReq, Len / 4, (UINT32 *)&mReadReqSwapped);
  // Calculate CRC
  Status = CalCrc32c ((UINT8 *)&mReadReqSwapped, Len, &Crc32);
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_READ_CIO_GEN_CRC_FAIL, (UINT32) Status, 0);
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  mReadReqSwapped.Crc = SwapBytes32 (Crc32);
  // Transmit the request
  Status = This->Impl->pDma->TxCfgPkt (
                               This->Impl->pDma,
                               PDF_READ_CONFIGURATION_REGISTERS,
                               (UINT16)(Len + 4),                                // Length + CRC
                               (UINT8 *)&mReadReqSwapped
                               );
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_READ_CIO_TRANSMIT_DMA_FAIL, (UINT32) RegNum, 0);
    goto error_while_reading;
  }
  ExpectedLength = (UINT16)OFFSET_OF (READ_CONFIGURATION_REGISTERS_RESPONSE, Crc) + 4;
  // Receive the response
  Status = This->Impl->pDma->RxCfgPkt (
                               This->Impl->pDma,
                               PDF_READ_CONFIGURATION_REGISTERS,
                               &ExpectedLength,
                               (UINT8 *)&mReadReqResponse
                               );
  if (TBT_STATUS_ERR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_READ_CIO_RECEIVE_REG_FAIL, (UINT32) RegNum, 0);
    goto error_while_reading;
  }

  // Prepare the read data for return
  ReadData = mReadReqResponse.ReadData;
  ReadData = SwapBytes32 (ReadData);
  *DataPtr = ReadData;
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBT_DRV_READ_CIO_READ_DATA, ReadData, 0);

  return TBT_STATUS_SUCCESS;

error_while_reading:
  This->Impl->pDma->DbgPrint (This->Impl->pDma);
  return TBT_STATUS_NON_RECOVERABLE_ERROR;
}

/* TBT host router pointer to store the device information */
STATIC TBT_HOST_ROUTER *TbtHostRouters[MAX_HOST_ROUTERS];
/* TBT host router numbers */
STATIC UINT32 TbtHostRoutersCount = 0;

/**
  Get the information list from the TbtDmaPcieBdf.
  @param[in]  TbtHostRouter     Pointer to the TBT_HOST_ROUTER structure on which the method is invoked.
  @param[in]  PcieRpConfig      PCIe root port config.
  @param[in]  TbtDmaPcieBdf     PCIe BDF identifier.
  @param[in]  ForcePwrFunc      Force power function, will be performed on a found TBT device.
                                If NULL -> no Force Powered.
**/
TBT_HOST_ROUTER*
GetTbtHostRouterList (
  IN UINT8            FirmwareType,
  IN PCIE_RP_CONFIG   *PcieRpConfig,
  IN PCIE_BDF         *TbtDmaPcieBdf, OPTIONAL
  IN FORCE_PWR_HR     ForcePwrFunc OPTIONAL
  )
{
  UINT32            Index;
  TBT_HOST_ROUTER*  TbtHostInfo;
  UINT32            PtrAddr;

  for (Index = 0; Index < TbtHostRoutersCount; ++Index) {
    TbtHostInfo = TbtHostRouters[Index];

    if (TbtHostInfo == NULL) {
      continue;
    }
    if (TbtHostInfo->FirmwareType != FirmwareType) {
      continue;
    }
    if (TbtHostInfo->ForcePwrFunc != ForcePwrFunc) {
      continue;
    }
    if (TbtDmaPcieBdf != NULL &&
      (TbtHostInfo->TbtDmaPcieBdf.Bus != TbtDmaPcieBdf->Bus ||
       TbtHostInfo->TbtDmaPcieBdf.Device != TbtDmaPcieBdf->Device ||
       TbtHostInfo->TbtDmaPcieBdf.Function != TbtDmaPcieBdf->Function)) {
        continue;
      }
    if (PcieRpConfig != NULL && (
      PcieRpConfig->PcieRpType != TbtHostInfo->PcieRpConfig.PcieRpType ||
      PcieRpConfig->PcieRootPort != TbtHostInfo->PcieRpConfig.PcieRootPort)) {
        continue;
      }

    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_GET_HOST_ROUTER_LIST, 0, 0);
    PtrAddr = (UINT32) (UINTN) ForcePwrFunc;
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO, (UINT32) FirmwareType, PtrAddr);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO2, (UINT32) TbtHostInfo->TbtDmaPcieBdf.Bus, (UINT32) TbtHostInfo->TbtDmaPcieBdf.Device);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO3, (UINT32) TbtHostInfo->TbtDmaPcieBdf.Function, (UINT32) TbtHostInfo->PcieRpConfig.PcieRpType);
    PtrAddr = (UINT32) (UINTN) TbtHostInfo;
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO4, (UINT32) TbtHostInfo->PcieRpConfig.PcieRootPort, PtrAddr);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO5, TbtHostInfo->RefCount, 0);

    TbtHostInfo->RefCount++;
    return TbtHostInfo;
  }

  return NULL;
}

/**
  Add the information list into the TbtHostRouter.
  @param[in]  TbtHostRouter     Pointer to the TBT_HOST_ROUTER structure on which the method is invoked.
  @param[in]  FirmwareType      The firmware type of target.
  @param[in]  PcieRpConfig      PCIe root port config.
  @param[in]  TbtDmaPcieBdf     PCIe BDF identifier.
  @param[in]  ForcePwrFunc      Force power function, will be performed on a found TBT device.
                                If NULL -> no Force Powered.
**/
VOID
AddTbtHostRouterList (
  IN TBT_HOST_ROUTER  *TbtHostRouter,
  IN UINT8            FirmwareType,
  IN PCIE_RP_CONFIG   *PcieRpConfig,
  IN PCIE_BDF         *TbtDmaPcieBdf, OPTIONAL
  IN FORCE_PWR_HR     ForcePwrFunc OPTIONAL
  )
{
  UINT32            PtrAddr;

  if (TbtHostRoutersCount >= MAX_HOST_ROUTERS) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_ADD_ROUTER_ERROR, 0, 0);
    return;
  }
  TbtHostRouters[TbtHostRoutersCount] = TbtHostRouter;
  TbtHostRoutersCount++;

  TbtHostRouter->FirmwareType = FirmwareType;
  TbtHostRouter->ForcePwrFunc = ForcePwrFunc;
  if (TbtDmaPcieBdf != NULL) {
    TbtHostRouter->TbtDmaPcieBdf.Bus = TbtDmaPcieBdf->Bus;
    TbtHostRouter->TbtDmaPcieBdf.Device = TbtDmaPcieBdf->Device;
    TbtHostRouter->TbtDmaPcieBdf.Function = TbtDmaPcieBdf->Function;
  } else {
    TbtHostRouter->TbtDmaPcieBdf.Bus = 0;
    TbtHostRouter->TbtDmaPcieBdf.Device = 0;
    TbtHostRouter->TbtDmaPcieBdf.Function = 0;
  }

  if (PcieRpConfig != NULL) {
    TbtHostRouter->PcieRpConfig.PcieRpType = PcieRpConfig->PcieRpType;
    TbtHostRouter->PcieRpConfig.PcieRootPort = PcieRpConfig->PcieRootPort;
  } else {
    TbtHostRouter->PcieRpConfig.PcieRpType = 0;
    TbtHostRouter->PcieRpConfig.PcieRootPort = 0;
  }

  TbtHostRouter->RefCount++;
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_ADD_ROUTER_LIST, 0, 0);
  PtrAddr = (UINT32) (UINTN) ForcePwrFunc;
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO, (UINT32) FirmwareType, PtrAddr);
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO2, (UINT32) TbtHostRouter->TbtDmaPcieBdf.Bus, (UINT32) TbtHostRouter->TbtDmaPcieBdf.Device);
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO3, (UINT32) TbtHostRouter->TbtDmaPcieBdf.Function, (UINT32) TbtHostRouter->PcieRpConfig.PcieRpType);
  PtrAddr = (UINT32) (UINTN) TbtHostRouter;
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO4, (UINT32) TbtHostRouter->PcieRpConfig.PcieRootPort, PtrAddr);
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO5, TbtHostRouter->RefCount, 0);
}

/**
  Remove the information list which in the TbtHostRouters.
  @param[in]  TbtHostRouter      Pointer to the TBT_HOST_ROUTER structure on which the method is invoked.
**/
VOID
RemoveTbtHostRouterList (
  IN TBT_HOST_ROUTER  *TbtHostRouter
  )
{
  UINT32            Index;
  TBT_HOST_ROUTER*  TbtHostInfo;
  UINT32            PtrAddr;

  if (TbtHostRouter == NULL) {
    return;
  }

  for (Index = 0; Index < TbtHostRoutersCount; ++Index) {
    TbtHostInfo = TbtHostRouters[Index];
    if (TbtHostInfo != TbtHostRouter) {
      continue;
    }

    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_REMOVE_ROUTER_LIST, 0, 0);
    PtrAddr = (UINT32) (UINTN) TbtHostInfo->ForcePwrFunc;
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO, (UINT32) TbtHostInfo->FirmwareType, PtrAddr);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO2, (UINT32) TbtHostInfo->TbtDmaPcieBdf.Bus, (UINT32) TbtHostInfo->TbtDmaPcieBdf.Device);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO3, (UINT32) TbtHostInfo->TbtDmaPcieBdf.Function, (UINT32) TbtHostInfo->PcieRpConfig.PcieRpType);
    PtrAddr = (UINT32) (UINTN) TbtHostInfo;
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO4, (UINT32) TbtHostInfo->PcieRpConfig.PcieRootPort, PtrAddr);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HOST_ROUTER_INFO5, TbtHostInfo->RefCount, 0);
    TbtHostRouters[Index] = NULL;
    return;
  }
  PtrAddr = (UINT32) (UINTN) TbtHostRouter;
  CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_REMOVE_ROUTER_ERROR, PtrAddr, 0);
}

/**
  TBT HR destroy.

  @param[in]  This        Pointer to the TBT_HOST_ROUTER structure on which the method is invoked

**/
VOID
TbtNvmDrvHrDtor (
  TBT_HOST_ROUTER *This
  )
{
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, EVT_CODE_TBT_DRV_DTOR, This->RefCount, 0);
  This->RefCount--;
  if (This->RefCount > 0) {
    return;
  }

  RemoveTbtHostRouterList (This);

  if (This->Impl->ForcePwrFunc != NULL) {
    This->Impl->ForcePwrFunc (This->Impl->pPciIoProto, FALSE, This->Impl->pDma->TBTControllerWasPowered);   // Disable the force power to HR
  }
  This->Impl->pDma->Dtor (This->Impl->pDma);
  TbtNvmDrvDeAllocateMem (This->Impl);
  TbtNvmDrvDeAllocateMem (This);
}

/**
  TBT HR constructor.

  @param[in] FirmwareType    The firmware type of target.
  @param[in] PcieRpConfig    PCIe root port config.
  @param[in] PcieBdf         PCIe BDF identifier
  @param[in] ForcePwrFunc    Force power function, will be performed on a found TBT device.
                             If NULL -> no force pwr.

  @retval The TBT_HR object pointer

**/
TBT_HOST_ROUTER*
TbtNvmDrvHrCtor (
  IN UINT8           FirmwareType,
  IN PCIE_RP_CONFIG  *PcieRpConfig,
  IN PCIE_BDF        *TbtDmaPcieBdf, OPTIONAL
  IN FORCE_PWR_HR    ForcePwrFunc OPTIONAL
  )
{
  EFI_STATUS         Status;
  TBT_STATUS         TbtStatus;
  UINTN              HandleCount;
  EFI_HANDLE         *Handles;
  UINT32             Index;
  TBT_HR_IMPL        *HrImplPtr;
  UINTN              SegmentNumber;
  UINTN              BusNumber;
  UINTN              DeviceNumber;
  UINTN              FunctionNumber;
  UINT64             Attributes;
  TBT_HOST_ROUTER    *HrPtr;
  BOOLEAN            TBTControllerWasPowered;
  PCI_ID             PciId;
  PCI_TYPE00         PciData;
  UINT8              RootPortIndex;
  UINT8              CpuRpNumber;
  UINT8              CpuBusNumber;
  UINT8              PchRpNumber;
  UINT8              PchBusNumber;
  UINT8              MaxDTbtControllerNumber;
  UINT8              CmMode;
  UINT8              ProgInterface;
  UINT8              SubClassCode;
  UINT8              BaseCode;
  BOOLEAN            IsDeviceAvailable;
  ITBT_INFO_HOB      *ITbtInfoHob;
  DTBT_INFO_HOB      *DTbtInfoHob;
  UINT32             CombinedPciId;

  PchRpNumber  = 0;
  PchBusNumber = 0;
  CpuRpNumber  = 0;
  CpuBusNumber = 0;

  HrPtr = GetTbtHostRouterList (FirmwareType, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  if (HrPtr != NULL) {
    return HrPtr;
  }

  // Initialize UEFI PCI protocol
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );

  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_PROTOCOL_FAIL, (UINT32) Status, 0);
    return NULL;
  }

  HrImplPtr = TbtNvmDrvAllocateMem (sizeof (TBT_HR_IMPL));
  if (!HrImplPtr) {
    gBS->FreePool (Handles);
    Status = EFI_OUT_OF_RESOURCES;
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_ALLOCATE_MEM_FAIL, (UINT32) Status, 0);
    return NULL;
  }

  if ( (FirmwareType == DISCRETE_TBT_RETIMER)
    || (FirmwareType == DISCRETE_TBT))
  {
    RootPortIndex           = PcieRpConfig->PcieRootPort;
    DTbtInfoHob             = GetFirstGuidHob (&gDTbtInfoHobGuid);
    MaxDTbtControllerNumber = PcdGet8 (PcdBoardDTbtControllerNumber);

    for (Index = 0; Index < MaxDTbtControllerNumber; Index++) {
      if (DTbtInfoHob->DTbtControllerConfig[Index].RpType == PCIE_RP_TYPE_PCH) {
        PchRpNumber  = DTbtInfoHob->DTbtControllerConfig[Index].PcieRpNumber;
        PchBusNumber = DTbtInfoHob->DTbtControllerConfig[Index].PcieRpBus;
      } else if (DTbtInfoHob->DTbtControllerConfig[Index].RpType == PCIE_RP_TYPE_CPU) {
        CpuRpNumber  = DTbtInfoHob->DTbtControllerConfig[Index].PcieRpNumber;
        CpuBusNumber = DTbtInfoHob->DTbtControllerConfig[Index].PcieRpBus;
      }
    }

    switch (PcieRpConfig->PcieRpType) {
      case (PCIE_RP_TYPE_PCH):
        CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_TYPE_PCH, (UINT32) FirmwareType, 0);
        CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_TYPE_PCH1, (UINT32) PchRpNumber, (UINT32) PchBusNumber);
        break;
      case (PCIE_RP_TYPE_CPU):
        CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_TYPE_CPU, (UINT32) FirmwareType, 0);
        CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_TYPE_CPU1, (UINT32) CpuRpNumber, (UINT32) CpuBusNumber);
        break;
      default:
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_INVALID_RP_TYPE, (UINT32) PcieRpConfig->PcieRpType, 0);
        goto free_hr_impl;
    }
  }

  // Search for the device with a given BDF or root port index.
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (Handles[Index], &gEfiPciIoProtocolGuid, (VOID**) &HrImplPtr->pPciIoProto);
    if (EFI_ERROR(Status)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_HANDLE_PROTOCOL_FAIL, (UINT32) Status, 0);
      continue;
    }
    Status = HrImplPtr->pPciIoProto->Pci.Read (
                                           HrImplPtr->pPciIoProto,
                                           EfiPciIoWidthUint32,
                                           0,
                                           sizeof (PciId) / sizeof (UINT32),
                                           &PciId
                                           );
    CombinedPciId = ((UINT32)PciId.VendorId << 16) | PciId.DeviceId;

    if (EFI_ERROR (Status)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_PCI_READ_FAIL, (UINT32) Status, 0);
      continue;
    }
    if ( (PciId.VendorId != USB_VENDOR_ID_1)
      && (PciId.VendorId != USB_VENDOR_ID_2) )
    {
      Status = EFI_NOT_FOUND;
      continue;
    }
    //check for USB class
    Status = HrImplPtr->pPciIoProto->Pci.Read (
                                           HrImplPtr->pPciIoProto,
                                           EfiPciIoWidthUint8,
                                           PCI_CLASSCODE_OFFSET,
                                           sizeof (PciData.Hdr.ClassCode),
                                           PciData.Hdr.ClassCode
                                           );
    if (EFI_ERROR (Status)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_CHECK_USB_FAIL, (UINT32) Status, 0);
      continue;
    }

    ProgInterface = PciData.Hdr.ClassCode[0];
    SubClassCode  = PciData.Hdr.ClassCode[1];
    BaseCode      = PciData.Hdr.ClassCode[2];
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_PCI_DEV_CLASS, (UINT32) BaseCode, 0);
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_PCI_DEV_CLASS2, (UINT32) SubClassCode, (UINT32) ProgInterface);

    if ((ProgInterface == PCI_IF_USB4) && (SubClassCode == PCI_CLASS_SERIAL_USB) && (BaseCode == PCI_CLASS_SERIAL)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_USB4_PCI, 0, 0);
      Status = HrImplPtr->pPciIoProto->GetLocation (
                                         HrImplPtr->pPciIoProto,
                                         &SegmentNumber,
                                         &BusNumber,
                                         &DeviceNumber,
                                         &FunctionNumber
                                         );
      if (EFI_ERROR (Status)) {
        CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_LOCATION_GET_FAIL, (UINT32) Status, 0);
        continue;
      }
      if (FirmwareType == INTEGRATED_TBT_RETIMER) {
        if ( (BusNumber != TbtDmaPcieBdf->Bus)
          || (DeviceNumber != TbtDmaPcieBdf->Device)
          || (FunctionNumber != TbtDmaPcieBdf->Function))
        {
          Status = EFI_NOT_FOUND;
          continue;
        }
      }
      if ( (FirmwareType == DISCRETE_TBT_RETIMER)
        || (FirmwareType == DISCRETE_TBT))
      {
        IsDeviceAvailable = TRUE;
        if (PcieRpConfig->PcieRpType == PCIE_RP_TYPE_PCH) {
          if ((RootPortIndex != PchRpNumber) || (BusNumber <= PchBusNumber)) {
            IsDeviceAvailable = FALSE;
          }
        } else if (PcieRpConfig->PcieRpType == PCIE_RP_TYPE_CPU) {
          if ( (RootPortIndex != CpuRpNumber)
            || (BusNumber <= CpuBusNumber)
            || ((PchBusNumber != 0) && (BusNumber >= PchBusNumber)))
          {
            IsDeviceAvailable = FALSE;
          }
        }

        if (!IsDeviceAvailable) {
          CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_CONTROLLER, CombinedPciId, 0);
          CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_PCI_INFO, (UINT32) SegmentNumber, (UINT32) BusNumber);
          CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_PCI_INFO2, (UINT32) DeviceNumber, (UINT32) FunctionNumber);
          Status = EFI_NOT_FOUND;
          continue;
        }
      }
      CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_CONTROLLER_FOUND, CombinedPciId, 0);
      CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_PCI_INFO, (UINT32) SegmentNumber, (UINT32) BusNumber);
      CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_PCI_INFO3, (UINT32) DeviceNumber, (UINT32) FunctionNumber);
      Status = EFI_SUCCESS;

      break;
    } else {
      Status = EFI_NOT_FOUND;
      continue;
    }
  } // end of search for the HR

  if (EFI_ERROR (Status)) {
    if (FirmwareType == INTEGRATED_TBT_RETIMER) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER, (UINT32) TbtDmaPcieBdf->Bus, (UINT32) TbtDmaPcieBdf->Device);
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_NOT_FOUND_FOR_ITBT_RETIMER2, (UINT32) TbtDmaPcieBdf->Function, (UINT32) Status);
    } else {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_NOT_FOUND_FOR_DTBT_RETIMER, (UINT32) (PcieRpConfig->PcieRootPort), (UINT32) Status);
    }
    goto free_hr_impl;
  }

  // Get the PCI Command options that are supported by this controller.
  Status = HrImplPtr->pPciIoProto->Attributes (
                                     HrImplPtr->pPciIoProto,
                                     EfiPciIoAttributeOperationSupported,
                                     0,
                                     &Attributes
                                     );
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_ATTRIBUTES_GET_FAIL, (UINT32) Status, 0);
    goto free_hr_impl;
  }

  // Set the PCI Command options to enable device memory mapped IO,
  // port IO, and bus mastering.
  Status = HrImplPtr->pPciIoProto->Attributes (
                                     HrImplPtr->pPciIoProto,
                                     EfiPciIoAttributeOperationEnable,
                                     Attributes | (EFI_PCI_DEVICE_ENABLE | EFI_PCI_IO_ATTRIBUTE_DUAL_ADDRESS_CYCLE),
                                     NULL
                                     );
  if (EFI_ERROR (Status)) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_ATTRIBUTES_SET_FAIL, (UINT32) Status, 0);
    goto free_hr_impl;
  }

  if (ForcePwrFunc != NULL) {
    // Force power the iTBT
    TBTControllerWasPowered = FALSE;

    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_FORCE_PWR_ITBT, 0, 0);
    TbtStatus = ForcePwrFunc (HrImplPtr->pPciIoProto, TRUE, &TBTControllerWasPowered);
    if (TBT_STATUS_ERR (TbtStatus)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_PERFORM_PWR_ERROR, CombinedPciId, (UINT32) TbtStatus);
      goto free_hr_impl;
    }
    CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CTOR_FORCE_PWR_DEV, CombinedPciId, 0);
  }
  HrImplPtr->ForcePwrFunc = ForcePwrFunc;

  Crc32cInit ();
  // Initiate DMA and config it
  HrImplPtr->pDma = TbtNvmDrvDmaCtor (HrImplPtr->pPciIoProto, &mTBTControllerWasPowered, FunctionNumber);
  if (!HrImplPtr->pDma) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_CTOR_INIT_DMA_FAIL, 0, 0);
    goto free_hr_impl;
  }

  // Allocate the interface and assign it
  HrPtr = TbtNvmDrvAllocateMem (sizeof(TBT_HOST_ROUTER));
  if (!HrPtr) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_HOST_ALLOCATE_MEM_FAIL, 0, 0);
    goto free_dma;
  }
  HrPtr->Impl = HrImplPtr;
  HrPtr->ReadCioDevReg  = TbtNvmDrvHrReadCioDevReg;
  HrPtr->WriteCioDevReg = TbtNvmDrvHrWriteCioReg;
  HrPtr->Dtor           = TbtNvmDrvHrDtor;

  //
  // Get CM mode.
  //
  if (FirmwareType == INTEGRATED_TBT_RETIMER) {
    //
    // Check if iTBT exists
    //
    ITbtInfoHob = NULL;
    ITbtInfoHob = GetFirstGuidHob (&gITbtInfoHobGuid);
    if (ITbtInfoHob == NULL) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_ITBT_INFO_HOB_NOT_FOUND, 0, 0);
      TbtNvmDrvDeAllocateMem (HrPtr);
      goto free_dma;
    }
    CmMode = (ITbtInfoHob->Usb4CmMode & USB4_CM_MODE_IN_PRE_BOOT);
  } else {
    //
    // Get dTBT CM mode.
    //
    CmMode = (DTbtInfoHob->DTbtGenericConfig.Usb4CmMode & 0x07);
  }

  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_CM_MODE, (UINT32) CmMode, 0);

  // Send driver ready (needed when FW CM mode is detected)
  if (CmMode == USB4_CM_MODE_FW_CM) {
    mTBTControllerWasPowered = FALSE;
    *HrImplPtr->pDma->TBTControllerWasPowered = mTBTControllerWasPowered;
    Status = TbtNvmDrvHrSendDrvReady (HrImplPtr->pDma);
    if (TBT_STATUS_ERR (Status)) {
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, EVT_CODE_TBT_DRV_SEND_DRIVER_RDY_FAIL, (UINT32) Status, 0);
      HrImplPtr->pDma->DbgPrint (HrImplPtr->pDma);
      TbtNvmDrvDeAllocateMem (HrPtr);
      goto free_dma;
    }
  } else {
    mTBTControllerWasPowered = TRUE;
    *HrImplPtr->pDma->TBTControllerWasPowered = mTBTControllerWasPowered;
  }

  gBS->FreePool (Handles);
  AddTbtHostRouterList (HrPtr, FirmwareType, PcieRpConfig, TbtDmaPcieBdf, ForcePwrFunc);
  CapsuleLogWrite (USBC_CAPSULE_DBG_INFO, EVT_CODE_TBT_DRV_HR_CTOR_COMPLETE, 0, 0);
  return HrPtr;

free_dma:
  HrImplPtr->pDma->Dtor (HrImplPtr->pDma);
free_hr_impl:
  TbtNvmDrvDeAllocateMem (HrImplPtr);
  gBS->FreePool (Handles);
  return NULL;
}
