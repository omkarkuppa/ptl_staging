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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrSendDrvReady: Failed to generate CRC from the data. Status - %d. Exiting...\n", Status));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  Message.crc = SwapBytes32 (Crc32);
  // Send the message
  TbtStatus = DmaPtr->TxCfgPkt (DmaPtr, PDF_SW_TO_FW_COMMAND, sizeof(Message), (UINT8 *)&Message);
  if (TBT_STATUS_ERR (TbtStatus)) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrSendDrvReady: The driver ready command failed. TbtStatus - %d. Exiting...\n", TbtStatus));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  // Make sure response is received
  TbtStatus = DmaPtr->RxCfgPkt (DmaPtr, PDF_FW_TO_SW_RESPONSE, NULL, NULL);
  if (TBT_STATUS_ERR (TbtStatus)) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrSendDrvReady: Didn't receive response for driver ready command. TbtStatus - %d. Exiting...\n", TbtStatus));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "TbtNvmDrvHrSendDrvReady: Sent successfully\n"));
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

  DEBUG ((DEBUG_VERBOSE, "TbtNvmDrvHrWriteCioReg: ConfigurationSpace - %d, Adapter - %d, RegNum - 0x%x\n", ConfigurationSpace, Adapter, RegNum));
  DEBUG ((DEBUG_VERBOSE, "TbtNvmDrvHrWriteCioReg: Length - 0x%x, Data - ", Length));
  for (DataPtrIndex = 0; DataPtrIndex < Length; DataPtrIndex++) {
    DEBUG ((DEBUG_VERBOSE, "%x ", *(DataPtr + DataPtrIndex)));
  }
  DEBUG ((DEBUG_VERBOSE, "\n"));

  DEBUG_CODE_END ();

  ASSERT (Length > 0 && Length <= 16);

  if (mWriteDevReq == NULL) {  // If not allocated yet
    mWriteDevReq = TbtNvmDrvAllocateMem (sizeof(WRITE_CONFIGURATION_REGISTERS_REQUEST));
    if (mWriteDevReq == NULL) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrWriteCioReg: Couldn't allocate memory for write request struct. Exiting...\n"));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrWriteCioReg: Avoid buffer overflow.\n"));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  TbtNvmDrvSwapEndianess ((UINT32 *)mWriteDevReq, Len / 4, (UINT32 *)&mWriteReqSwapped);
  // Calculate CRC
  Status = CalCrc32c ((UINT8 *)&mWriteReqSwapped, Len, &Crc32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,"TbtNvmDrvHrWriteCioReg: Failed to generate CRC from the data. Status - %d. Exiting...\n", Status));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrWriteCioReg: Couldn't transmit DMA frame to write to reg - 0x%x, length - 0x%x. Exiting...\n", RegNum, Length));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrReadCioDevReg: TbtNvmDrvHrWriteCioReg: Couldn't receive response while writing to reg - 0x%x, length - 0x%x. Exiting...\n", RegNum, Length));
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

  DEBUG ((DEBUG_VERBOSE, "TbtNvmDrvHrReadCioDevReg: ConfigurationSpace - %d, Adapter - %d, RegNum - 0x%x\n", ConfigurationSpace, Adapter, RegNum));

  if (mReadDevReq == NULL) {  // If not allocated yet
    mReadDevReq = TbtNvmDrvAllocateMem (sizeof(READ_CONFIGURATION_REGISTERS_REQUEST));
    if (mReadDevReq == NULL) {
      DEBUG ((DEBUG_ERROR, "Couldn't allocate memory for read request struct\n"));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrWriteCioReg: Avoid buffer overflow.\n"));
    return TBT_STATUS_NON_RECOVERABLE_ERROR;
  }
  TbtNvmDrvSwapEndianess ((UINT32 *)mReadDevReq, Len / 4, (UINT32 *)&mReadReqSwapped);
  // Calculate CRC
  Status = CalCrc32c ((UINT8 *)&mReadReqSwapped, Len, &Crc32);
  if (TBT_STATUS_ERR (Status)) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrWriteCioReg: Failed to generate CRC from the data. Status - %d. Exiting...\n", Status));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrReadCioDevReg: Couldn't transmit DMA frame to read from reg - 0x%x. Exiting...\n", RegNum));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrReadCioDevReg: Couldn't receive response while reading from reg - 0x%x. Exiting...\n", RegNum));
    goto error_while_reading;
  }

  // Prepare the read data for return
  ReadData = mReadReqResponse.ReadData;
  ReadData = SwapBytes32 (ReadData);
  *DataPtr = ReadData;
  DEBUG ((DEBUG_VERBOSE, "Read data is - 0x%x\n", ReadData));

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
STATIC
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

    DEBUG ((DEBUG_INFO, "GetTbtHostRouterList: FirmwareType=%d ForcePwrFunc=%p, B%d D%d F%d PcieRpType=%d PcieRootPort=%d ret=%p RefCount=%d\n",
    FirmwareType, ForcePwrFunc, TbtHostInfo->TbtDmaPcieBdf.Bus, TbtHostInfo->TbtDmaPcieBdf.Device, TbtHostInfo->TbtDmaPcieBdf.Function,
    TbtHostInfo->PcieRpConfig.PcieRpType, TbtHostInfo->PcieRpConfig.PcieRootPort, TbtHostInfo, TbtHostInfo->RefCount));

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
STATIC
VOID
AddTbtHostRouterList (
  IN TBT_HOST_ROUTER  *TbtHostRouter,
  IN UINT8            FirmwareType,
  IN PCIE_RP_CONFIG   *PcieRpConfig,
  IN PCIE_BDF         *TbtDmaPcieBdf, OPTIONAL
  IN FORCE_PWR_HR     ForcePwrFunc OPTIONAL
  )
{
  if (TbtHostRoutersCount >= MAX_HOST_ROUTERS) {
    DEBUG ((DEBUG_ERROR, "AddTbtHostRouterList ERROR: increase MAX_HOST_ROUTERS\n"));
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
  DEBUG ((DEBUG_INFO, "AddTbtHostRouterList FirmwareType=%d ForcePwrFunc=%p, B%d D%d F%d PcieRpType=%d PcieRootPort=%d ret=%p RefCount=%d\n",
  FirmwareType, ForcePwrFunc, TbtHostRouter->TbtDmaPcieBdf.Bus, TbtHostRouter->TbtDmaPcieBdf.Device, TbtHostRouter->TbtDmaPcieBdf.Function,
  TbtHostRouter->PcieRpConfig.PcieRpType, TbtHostRouter->PcieRpConfig.PcieRootPort, TbtHostRouter, TbtHostRouter->RefCount));
}

/**
  Remove the information list which in the TbtHostRouters.
  @param[in]  TbtHostRouter      Pointer to the TBT_HOST_ROUTER structure on which the method is invoked.
**/
STATIC
VOID
RemoveTbtHostRouterList (
  IN TBT_HOST_ROUTER  *TbtHostRouter
  )
{
  UINT32            Index;
  TBT_HOST_ROUTER*  TbtHostInfo;

  if (TbtHostRouter == NULL) {
    return;
  }

  for (Index = 0; Index < TbtHostRoutersCount; ++Index) {
    TbtHostInfo = TbtHostRouters[Index];
    if (TbtHostInfo != TbtHostRouter) {
      continue;
    }

    DEBUG ((DEBUG_INFO, "RemoveTbtHostRouterList: FirmwareType=%d ForcePwrFunc=%p, B%d D%d F%d PcieRpType=%d PcieRootPort=%d ret=%p RefCount=%d\n",
    TbtHostInfo->FirmwareType, TbtHostInfo->ForcePwrFunc, TbtHostInfo->TbtDmaPcieBdf.Bus, TbtHostInfo->TbtDmaPcieBdf.Device,
    TbtHostInfo->TbtDmaPcieBdf.Function, TbtHostInfo->PcieRpConfig.PcieRpType, TbtHostInfo->PcieRpConfig.PcieRootPort,
    TbtHostInfo, TbtHostInfo->RefCount));

    TbtHostRouters[Index] = NULL;
    return;
  }
  DEBUG ((DEBUG_ERROR, "RemoveTbtHostRouterList ERROR: TbtHostRouter=%p\n", TbtHostRouter));
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
  DEBUG ((DEBUG_VERBOSE, "TbtNvmDrvHrDtor is called RefCount=%d.\n", This->RefCount));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: LocateHandleBuffer of gEfiPciIoProtocolGuid failed, Status = %r\n", Status));
    return NULL;
  }

  HrImplPtr = TbtNvmDrvAllocateMem (sizeof(TBT_HR_IMPL));
  if (!HrImplPtr) {
    gBS->FreePool (Handles);
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: TbtNvmDrvAllocateMem failed, Status = %r\n", EFI_OUT_OF_RESOURCES));
    return NULL;
  }

  if ( (FirmwareType == DISCRETE_TBT_RETIMER)
    || (FirmwareType == DISCRETE_TBT))
  {
    RootPortIndex           = PcieRpConfig->PcieRootPort;  //TBD
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
        DEBUG ((DEBUG_INFO, "TbtNvmDrvHrCtor: Type:PCH FirmwareType %x PchRpNumber %x PchBusNumber %x\n", FirmwareType, PchRpNumber, PchBusNumber));
        break;
      case (PCIE_RP_TYPE_CPU):
        DEBUG ((DEBUG_INFO, "TbtNvmDrvHrCtor: Type:CPU FirmwareType %x CpuRpNumber %x CpuBusNumber %x\n", FirmwareType, CpuRpNumber, CpuBusNumber));
        break;
      default:
        DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: Failed at unsupported type value %x in PCIE_RP_TYPE.\n", PcieRpConfig->PcieRpType));
        goto free_hr_impl;
    }
  }

  // Search for the device with a given BDF or root port index.
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (Handles[Index], &gEfiPciIoProtocolGuid, (VOID**) &HrImplPtr->pPciIoProto);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: HandleProtocol of gEfiPciIoProtocolGuid failed, Status = %r\n", Status));
      continue;
    }
    Status = HrImplPtr->pPciIoProto->Pci.Read (
                                           HrImplPtr->pPciIoProto,
                                           EfiPciIoWidthUint32,
                                           0,
                                           sizeof (PciId) / sizeof (UINT32),
                                           &PciId
                                           );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: Pci.Read failed, Status = %r\n", Status));
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
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: check for USB class failed, Status = %r\n", Status));
      continue;
    }

    ProgInterface = PciData.Hdr.ClassCode[0];
    SubClassCode  = PciData.Hdr.ClassCode[1];
    BaseCode      = PciData.Hdr.ClassCode[2];
    DEBUG ((DEBUG_INFO, "PCI Device, class code = (%0xh, %0xh, %0xh)\n", BaseCode, SubClassCode, ProgInterface));

    if ((ProgInterface == PCI_IF_USB4) && (SubClassCode == PCI_CLASS_SERIAL_USB) && (BaseCode == PCI_CLASS_SERIAL)) {
      DEBUG ((DEBUG_INFO, "TbtNvmDrvHrCtor: USB4 PCI device\n"));

      Status = HrImplPtr->pPciIoProto->GetLocation (
                                         HrImplPtr->pPciIoProto,
                                         &SegmentNumber,
                                         &BusNumber,
                                         &DeviceNumber,
                                         &FunctionNumber
                                         );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: GetLocation failed, Status = %r\n", Status));
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
          DEBUG ((DEBUG_ERROR,"The TBT Controller %x: %04x:%02x:%02x.%02x is not the target.\n",
            PciId,
            SegmentNumber,
            BusNumber,
            DeviceNumber,
            FunctionNumber
            ));
          Status = EFI_NOT_FOUND;
          continue;
        }
      }

      DEBUG ((DEBUG_INFO,"TbtNvmDrvHrCtor: Found Thunderbolt Controller %x: %04x:%02x:%02x.%02x\n",
        PciId,
        SegmentNumber,
        BusNumber,
        DeviceNumber,
        FunctionNumber
        ));
      Status = EFI_SUCCESS;

      break;
    } else {
      Status = EFI_NOT_FOUND;
      continue;
    }
  } // end of search for the HR

  if (EFI_ERROR (Status)) {
    if (FirmwareType == INTEGRATED_TBT_RETIMER) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: PCI device with bdf = 0x%x,0x%x,0x%x is not found!, Status = %r\n",
        TbtDmaPcieBdf->Bus,
        TbtDmaPcieBdf->Device,
        TbtDmaPcieBdf->Function,
        Status
        ));
    } else {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: PCI device is not found with root port = %d!, Status = %r\n",
        PcieRpConfig->PcieRootPort,
        Status
        ));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: Attributes get is failed, Status = %r\n", Status));
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
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: Attributes set failed, Status = %r\n", Status));
    goto free_hr_impl;
  }

  if (ForcePwrFunc != NULL) {
    // Force power the ITBT
    TBTControllerWasPowered = FALSE;

    DEBUG ((DEBUG_INFO, "TbtNvmDrvHrCtor: Force power the ITBT\n"));
    TbtStatus = ForcePwrFunc (HrImplPtr->pPciIoProto, TRUE, &TBTControllerWasPowered);
    if (TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: Could not perform device %x force pwr, Status = %d\n", PciId, Status));
      goto free_hr_impl;
    }
    DEBUG ((DEBUG_INFO, "TbtNvmDrvHrCtor: Force powered device %x\n", PciId));
  }
  HrImplPtr->ForcePwrFunc = ForcePwrFunc;

  Crc32cInit ();
  // Initiate DMA and config it
  HrImplPtr->pDma = TbtNvmDrvDmaCtor (HrImplPtr->pPciIoProto, &mTBTControllerWasPowered, FunctionNumber);
  if (!HrImplPtr->pDma) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: Could not initialize DMA for device \n"));
    goto free_hr_impl;
  }

  // Allocate the interface and assign it
  HrPtr = TbtNvmDrvAllocateMem (sizeof(TBT_HOST_ROUTER));
  if (!HrPtr) {
    DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: Could not allocate memory for TBT_HOST_ROUTER\n"));
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
    // Check if ITBT exists
    //
    ITbtInfoHob = NULL;
    ITbtInfoHob = GetFirstGuidHob (&gITbtInfoHobGuid);
    if (ITbtInfoHob == NULL) {
      DEBUG ((DEBUG_INFO, "No integrated USB4 host router found!\n"));
      TbtNvmDrvDeAllocateMem (HrPtr);
      goto free_dma;
    }
    CmMode = (ITbtInfoHob->Usb4CmMode & USB4_CM_MODE_IN_PRE_BOOT);
  } else {
    //
    // DTBT USB4 CM mode default only support FW CM.
    //
    CmMode = USB4_CM_MODE_FW_CM;
  }

  DEBUG ((DEBUG_INFO, "TbtNvmDrvHrCtor: CM Mode = %x\n", CmMode));

  // Send driver ready (needed when FW CM mode is detected)
  if (CmMode == USB4_CM_MODE_FW_CM) {
    mTBTControllerWasPowered = FALSE;
    *HrImplPtr->pDma->TBTControllerWasPowered = mTBTControllerWasPowered;
    Status = TbtNvmDrvHrSendDrvReady (HrImplPtr->pDma);
    if (TBT_STATUS_ERR (Status)) {
      DEBUG ((DEBUG_ERROR, "TbtNvmDrvHrCtor: Failed to send driver ready, Status=%d\n", Status));
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
  DEBUG ((DEBUG_INFO, "TbtNvmDrvHrCtor: Completed\n"));
  return HrPtr;

free_dma:
  HrImplPtr->pDma->Dtor (HrImplPtr->pDma);
free_hr_impl:
  TbtNvmDrvDeAllocateMem (HrImplPtr);
  gBS->FreePool (Handles);
  return NULL;
}
