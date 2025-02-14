/** @file
  Touch Host Controller QuickI2c THC Protocol API

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include "QuickI2cLibPrivate.h"
#include "QuickI2cHid.h"
#include "QuickI2cSubIP.h"
#include "QuickI2cDriver.h"

/**
  Send Power on request to SubIP
  @param[in]  QuickI2cDev      QuickI2c Dev
  @param[in]  PowerState       PowerState
  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cSendSetPowerRequest (
  IN QUICK_I2C_DEV    *QuickI2cDev,
  IN UINT8            PowerState
  )
{
  EFI_STATUS                      Status;
  QUICK_I2C_SET_POWER_REQUEST     QuickI2cSetPowerReq;
  QUICK_I2C_OUTPUT_REPORT_HEADER  OutputReportHeader;

  THC_LOCAL_DEBUG (L"QuickI2cSendSetPowerRequest Entry() Instance: %d, PowerState: %d \n", QuickI2cDev->InstanceId, PowerState);

  Status = EFI_SUCCESS;
  SetMem (&QuickI2cSetPowerReq, sizeof(QUICK_I2C_SET_POWER_REQUEST), 0);
  SetMem (&OutputReportHeader, sizeof(QUICK_I2C_OUTPUT_REPORT_HEADER), 0);

  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = TRUE;

  //
  // As per spec only ContentLength and Command Register will be passed to THC HW PRD
  //
  OutputReportHeader.Fields.ContentLength = sizeof (QUICK_I2C_SET_POWER_REQUEST);

  //
  // First add the command register LSB + MSB to the TXDMA buffer
  //
  if (QuickI2cDev->DeviceDescriptor.wCommandRegister) {
    OutputReportHeader.Fields.CommandRegister = QuickI2cDev->DeviceDescriptor.wCommandRegister;
  }

  //
  // First add the command register LSB+MSB to the TXDMA buffer
  //
  QuickI2cSetPowerReq.Fields.High.Opcode    = (UINT8) QuickI2cSubIpOpcodeSetPower;
  QuickI2cSetPowerReq.Fields.High.Reserved  = 0b0000;
  QuickI2cSetPowerReq.Fields.Low.PowerState = (UINT8) PowerState;
  QuickI2cSetPowerReq.Fields.Low.Reserved   = 0b000000;

  //
  // For Power On request thier is no response expected
  //
  Status = QuickI2cDmaWriteTx (QuickI2cDev, OutputReportHeader, (UINT8 *) &QuickI2cSetPowerReq);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cSendSetPowerRequest: QuickI2cDmaWriteTx Error Status %r\n", Status)
    DEBUG ((DEBUG_ERROR, "QuickI2cSendSetPowerRequest: QuickI2cDmaWriteTx Error Status %r\n", Status));
    // continue
  }

  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = FALSE;

  return Status;
}

/**
  Send Reset request to SubIP
  @param[in]  QuickI2cDev      QuickI2c Dev
  @param[in]  Timeout          Timeout
  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cSendResetRequest (
  IN QUICK_I2C_DEV      *QuickI2cDev,
  IN UINTN              Timeout
  )
{
  EFI_STATUS                      Status;
  QUICK_I2C_COMMAND_REQUEST       QuickI2cCommandRequest;
  QUICK_I2C_OUTPUT_REPORT_HEADER  OutputReportHeader;
  THC_M_PRT_INT_STATUS            ThcIntStatus;
  RESET_RESPONSE_DATA             ResetResponseData;

  THC_LOCAL_DEBUG (L"QuickI2cSendResetRequest Entry() Instance: %d\n", QuickI2cDev->InstanceId);

  SetMem (&QuickI2cCommandRequest, sizeof (QUICK_I2C_COMMAND_REQUEST), 0);
  SetMem (&OutputReportHeader, sizeof (QUICK_I2C_OUTPUT_REPORT_HEADER), 0);
  SetMem (&ResetResponseData, sizeof (RESET_RESPONSE_DATA), 0);

  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = TRUE;

  OutputReportHeader.Fields.ContentLength = sizeof (QUICK_I2C_COMMAND_REQUEST);
  //
  // First add the command register LSB + MSB to the TXDMA buffer
  //
  if (QuickI2cDev->DeviceDescriptor.wCommandRegister) {
    OutputReportHeader.Fields.CommandRegister = QuickI2cDev->DeviceDescriptor.wCommandRegister;
  }
  //
  // Send Reset Request
  //
  QuickI2cCommandRequest.Fields.High.Opcode     = (UINT8) QuickI2cSubIpOpcodeReset;
  QuickI2cCommandRequest.Fields.High.Reserved   = 0b0000;
  QuickI2cCommandRequest.Fields.Low.ReportID    = QUICK_I2C_RESET_REQUEST_REPORT_ID;
  QuickI2cCommandRequest.Fields.Low.ReportType  = (UINT8) QuickI2cReportTypeReset;
  QuickI2cCommandRequest.Fields.Low.Reserved    = 0b00;

  // Set other registers to clear INT status
  ThcIntStatus.Data32 = MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS);
  ThcIntStatus.Fields.DevRawIntSts = 1;
  MmioWrite32 ((UINTN)(QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS), ThcIntStatus.Data32);
  THC_LOCAL_DEBUG (L"Instance %d: DEV_RAW_INT_STS = 0x%X\n", QuickI2cDev->InstanceId, MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS))
  DEBUG ((DEBUG_INFO, "Instance %d: DEV_RAW_INT_STS = 0x%X\n", QuickI2cDev->InstanceId, MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS)));

  Status = QuickI2cDmaWriteTx (QuickI2cDev, OutputReportHeader, (UINT8 *) &QuickI2cCommandRequest);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "QuickI2cSendResetRequest: QuickI2cDmaWriteTx Status %r\n", Status));
    THC_LOCAL_DEBUG (L"QuickI2cSendResetRequest: QuickI2cDmaWriteTx Status %r\n", Status)
    Status = EFI_SUCCESS; // override to make sure its optional response
    goto End;
  }

  //
  // Wait for Response
  //
  if (Timeout > 0) {
    THC_LOCAL_DEBUG (L"QuickI2cSendResetRequest Waiting for Response DEV_RAW_INT_STS = 0x%X \n", MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS))
    DEBUG ((DEBUG_INFO, "QuickI2cSendResetRequest Waiting for Response DEV_RAW_INT_STS = 0x%X\n", MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS)));
    do {
      ThcIntStatus.Data32 = MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS);
      if (ThcIntStatus.Fields.DevRawIntSts) {
        Status = QuickI2cLibPerformPioRead (
                  QuickI2cDev->PciBar0,
                  QuickI2cDev->DeviceDescriptor.wInputRegister,
                  sizeof (RESET_RESPONSE_DATA),
                  &ResetResponseData.Data32
                );
        if (!EFI_ERROR (Status)) {
            DEBUG ((DEBUG_INFO, "QuickI2cLibPerformPioRead: ResetResponseData 0x%X\n", ResetResponseData.Data32));
          //
          // Clear int status bit
          //
          ThcIntStatus.Data32 = MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS);
          ThcIntStatus.Fields.DevRawIntSts = 1;
          MmioWrite32 ((UINTN)(QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS), ThcIntStatus.Data32);
          Status = EFI_SUCCESS;
          break;
        }
      }
      MicroSecondDelay (1000);
      Timeout--;
    } while (Timeout > 0);
    if (Timeout == 0) {
      // Output reponse is optional
      THC_LOCAL_DEBUG (L"QuickI2cSendResetRequest Response Timeout might be expected\n")
      DEBUG ((DEBUG_WARN, "QuickI2cSendResetRequest Response Timeout might be expected\n"));
      Status = EFI_SUCCESS; // override to make sure its optional response
    }
  }

End:
  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = FALSE;

  return Status;
}

/**
  THC Hid Get Feature
  @param[in]  QuickI2cDev      Context of QuickI2c device
  @param[in]  HidPacket        THC HID packet
  @param[in]  ReportType       Report Type
  @param[in]  Timeout          Timeout
  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device did not respond to request for report descriptro
  @retval EFI_TIMEOUT          Timeout while waiting for response from the device
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickI2c HID support
**/
EFI_STATUS
QuickI2cGetFeature (
  IN     QUICK_I2C_DEV        *QuickI2cDev,
  IN OUT HID_PACKET           *HidPacket,
  IN     UINT8                ReportType,
  IN     UINTN                Timeout
  )
{
  EFI_STATUS                  Status;
  UINT32                      RxDlenEn;
  UINT32                      Wbc;
  UINT32                      WriteDataHeader;
  UINT32                      Data;
  UINT8                       rId;
  QUICK_I2C_COMMAND_REQUEST   Quick2cGetFeatureRequest;

  THC_LOCAL_DEBUG (L"QuickI2cGetFeature: Start() Instance: %d, ReportType = %d, Timeout = %d\n", QuickI2cDev->InstanceId, ReportType, Timeout);
  THC_LOCAL_DEBUG (L"QuickI2cGetFeature: HidPacket->ReportId = %d, HidPacket->ReportBufferLen = %d\n", HidPacket->ReportId, HidPacket->ReportBufferLen);

  rId                             = 0;
  QuickI2cDev->HidDataAvailable   = FALSE;
  QuickI2cDev->HidActive          = TRUE;
  QuickI2cDev->SwDmaMessageLength = 0;

  ZeroMem (QuickI2cDev->HidBuffer, THC_MAX_HID_BUFFER_SIZE);
  ZeroMem (&Quick2cGetFeatureRequest, sizeof (QUICK_I2C_COMMAND_REQUEST));

  //
  // Send Get Feature Request
  //
  Quick2cGetFeatureRequest.Fields.High.Opcode     = QuickI2cSubIpOpcodeGet;
  Quick2cGetFeatureRequest.Fields.High.Reserved   = 0b0000;
  Quick2cGetFeatureRequest.Fields.Low.ReportType  = ReportType;
  Quick2cGetFeatureRequest.Fields.Low.Reserved    = 0b00;

  // need to check reportID value to determine if the third optional byte is needed and also determine the write-byte-size
  if (QuickI2cDev->ReportPacket.ReportId < QUICKI_I2C_THC_SWDMA_WRITE_BYTE_SIZE_LIMIT) {
    Quick2cGetFeatureRequest.Fields.Low.ReportID  = HidPacket->ReportId;
    RxDlenEn                                      = QUICK_I2C_THC_SWDMA_I2C_RX_DLEN_EN_1;
    Wbc                                           = sizeof (QuickI2cDev->DeviceDescriptor.wCommandRegister) +
                                                    sizeof (Quick2cGetFeatureRequest) +
                                                    sizeof (QuickI2cDev->DeviceDescriptor.wDataRegister);
    WriteDataHeader                               = QuickI2cDev->DeviceDescriptor.wCommandRegister | (Quick2cGetFeatureRequest.Data << 16);
    Data                                          = QuickI2cDev->DeviceDescriptor.wDataRegister;
  } else {
    Quick2cGetFeatureRequest.Fields.Low.ReportID = 0b1111;
    rId                                          = HidPacket->ReportId;
    RxDlenEn                                     = QUICK_I2C_THC_SWDMA_I2C_RX_DLEN_EN_1;
    Wbc                                          = sizeof (QuickI2cDev->DeviceDescriptor.wCommandRegister) +
                                                   sizeof (Quick2cGetFeatureRequest) +
                                                   sizeof (rId) +
                                                   sizeof (QuickI2cDev->DeviceDescriptor.wDataRegister);
    WriteDataHeader                              = QuickI2cDev->DeviceDescriptor.wCommandRegister | (Quick2cGetFeatureRequest.Data << 16);
    Data                                         = (QuickI2cDev->DeviceDescriptor.wDataRegister << 8) | rId;
  }

  THC_LOCAL_DEBUG(L"QuickI2cGetFeature: RxDlenEn = %d, Wbc = %d, WriteDataHeader= 0x%x, Data= 0x%x\n", RxDlenEn, Wbc, WriteDataHeader, Data)
  THC_LOCAL_DEBUG(L"QuickI2cGetFeature: QuickI2c SwDma Active !\n")
  QuickI2cDev->SwDmaActive = TRUE;
  Status = QuickI2cSwDma (QuickI2cDev, &Data, RxDlenEn, Wbc, WriteDataHeader);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cGetFeature: QuickI2cSwDma return Status: %r\n", Status));
    goto End;
  }

  //
  // Wait for Response
  //
  if (Timeout > 0) {
    DEBUG ((DEBUG_INFO, "QuickI2cGetFeature: Waiting for Response \n"));
    do {
      if (QuickI2cDev->HidDataAvailable == TRUE && QuickI2cDev->ReadDone == TRUE) {
        THC_LOCAL_DEBUG (L"QuickI2cGetFeature: Response received\n")
        DEBUG ((DEBUG_INFO, "QuickI2cGetFeature: Response received\n"));
        Status = EFI_SUCCESS;
        break;
      }
      MicroSecondDelay (1000);
      Timeout--;
    } while (Timeout > 0);

    if (Timeout == 0) {
      THC_LOCAL_DEBUG (L"QuickI2cGetFeature: Response Timeout might be optional\n")
      DEBUG ((DEBUG_ERROR, "QuickI2cGetFeature: Response Timeout might be optional\n"));
      Status = EFI_TIMEOUT;
      goto End;
    }
  }

  //
  // fill return data
  //
  if (QuickI2cDev->SwDmaMessageLength > 0) {
    THC_LOCAL_DEBUG (L"QuickI2cGetFeature: received SwDmaMessageLength = %d \n", QuickI2cDev->SwDmaMessageLength);
    HidPacket->ReportBufferLen = QuickI2cDev->SwDmaMessageLength;
    HidPacket->ReportBuffer = AllocateZeroPool (QuickI2cDev->SwDmaMessageLength);
    if (HidPacket->ReportBuffer != NULL) {
      CopyMem (HidPacket->ReportBuffer, QuickI2cDev->HidBuffer, QuickI2cDev->SwDmaMessageLength);
      //THC_LOCAL_DEBUG (L"QuickI2cGetFeature: GetFeatureDump <%d> Bytes :\n", HidPacket->ReportBufferLen);  //uncomment for debug purpose
      //ShowBuffer (HidPacket->ReportBuffer, QuickI2cDev->SwDmaMessageLength);
    }
  }

End:
  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = FALSE;

  //
  // complete the process
  //
  QuickI2cCompleteSwdma (QuickI2cDev);

  QuickI2cDmaFillSinglePrdTable (&QuickI2cDev->SwDmaRead, 0);
  QuickI2cDev->SwDmaActive = FALSE;

  DEBUG ((DEBUG_INFO, "QuickI2cGetFeature: SwDmaActive %r End\n", Status));
  return Status;
}

/**
  THC Hid Set Feature
  @param[in]  QuickI2cDev      Context of QuickI2c device
  @param[in]  HidPacket        THC HID packet
  @param[in]  ReportType       Report Type
  @param[in]  Timeout          Timeout
  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device did not respond to request for report descriptor
  @retval EFI_TIMEOUT          Timeout while waiting for response from the device
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickI2c HID support
**/
EFI_STATUS
QuickI2cSetFeature (
  IN  QUICK_I2C_DEV              *QuickI2cDev,
  IN  HID_PACKET                 *HidPacket,
  IN  UINT8                      ReportType,
  IN  UINTN                      Timeout
  )
{
  EFI_STATUS                      Status;
  UINT8                           *WriteBuffer;
  UINT8                           rId;
  UINT16                          DataFieldLength;
  BOOLEAN                         IncludeLengthField;
  UINT8                           TotalLength;
  UINT32                          ContentLength;
  QUICK_I2C_COMMAND_REQUEST       Quick2cSetFeatureRequest;
  QUICK_I2C_OUTPUT_REPORT_HEADER  OutputReportHeader;

  THC_LOCAL_DEBUG (L"QuickI2cSetFeature: Start() Instance: %d, ReportType = %d, Timeout = %d\n", QuickI2cDev->InstanceId, ReportType, Timeout);
  THC_LOCAL_DEBUG (L"QuickI2cSetFeature: HidPacket->ReportId = %d, HidPacket->ReportBufferLen = %d\n", HidPacket->ReportId, HidPacket->ReportBufferLen);
  //ShowBuffer (HidPacket->ReportBuffer, HidPacket->ReportBufferLen); //uncomment for debug purpose

  ZeroMem (&Quick2cSetFeatureRequest, sizeof (QUICK_I2C_COMMAND_REQUEST));
  ZeroMem (&OutputReportHeader, sizeof (QUICK_I2C_OUTPUT_REPORT_HEADER));
  ZeroMem (QuickI2cDev->HidBuffer, THC_MAX_HID_BUFFER_SIZE);

  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = TRUE;
  ContentLength                 = 0;
  TotalLength                   = 0;
  rId                           = 0;

  //
  // Send command request
  //
  Quick2cSetFeatureRequest.Fields.High.Opcode     = (UINT8) QuickI2cSubIpOpcodeSet;
  Quick2cSetFeatureRequest.Fields.High.Reserved   = 0b0000;
  Quick2cSetFeatureRequest.Fields.Low.ReportType  = (UINT8) ReportType;
  Quick2cSetFeatureRequest.Fields.Low.Reserved    = 0b00;

  // need to check reportID value to determine if the third optional byte is needed and also determine the write-byte-size
  if (HidPacket->ReportId < QUICKI_I2C_THC_SWDMA_WRITE_BYTE_SIZE_LIMIT) {
    Quick2cSetFeatureRequest.Fields.Low.ReportID  = HidPacket->ReportId;;
  } else {
    Quick2cSetFeatureRequest.Fields.Low.ReportID = 0b1111;
    rId                                          = HidPacket->ReportId;
  }

  // Determine if the length field should be included
  IncludeLengthField = (HidPacket->ReportBuffer != NULL && HidPacket->ReportBufferLen > 0) ? TRUE : FALSE;

  // First add the command register LSB + MSB to the TXDMA buffer
  if (QuickI2cDev->DeviceDescriptor.wCommandRegister) {
    OutputReportHeader.Fields.CommandRegister = QuickI2cDev->DeviceDescriptor.wCommandRegister;
  }

  // For optional third byte, add report ID, add the data register LSB + MSB to the TXDMA buffer
  ContentLength = sizeof (QUICK_I2C_COMMAND_REQUEST) +
                  ((rId > 0) ? sizeof (rId) : 0) +
                  ((IncludeLengthField) ? sizeof (QuickI2cDev->DeviceDescriptor.wDataRegister) : 0) +
                  ((IncludeLengthField) ? sizeof (DataFieldLength) : 0) +
                  ((IncludeLengthField) ? HidPacket->ReportBufferLen : 0);

  if (ContentLength > 0xFFFF) {
    DEBUG ((DEBUG_ERROR, "ContentLength exceeds UINT16 maximum value\n"));
    return EFI_BAD_BUFFER_SIZE;
  }

  OutputReportHeader.Fields.ContentLength = (UINT16) ContentLength;
  THC_LOCAL_DEBUG (L"OutputReportHeader.Fields.ContentLength: %d\n", OutputReportHeader.Fields.ContentLength)

  WriteBuffer = AllocateZeroPool (OutputReportHeader.Fields.ContentLength);
  if (WriteBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (WriteBuffer, (VOID*)&(Quick2cSetFeatureRequest), sizeof (QUICK_I2C_COMMAND_REQUEST));
  TotalLength += sizeof (QUICK_I2C_COMMAND_REQUEST);

  if (rId > 0) {
    CopyMem (WriteBuffer + TotalLength, (VOID*)&(rId), sizeof (rId));
    TotalLength += sizeof (rId);
  }

  if (IncludeLengthField > 0) {
    //
    // Add the data register LSB + MSB to the TXDMA buffer if there is a data to send
    //
    if (QuickI2cDev->DeviceDescriptor.wDataRegister) {
      CopyMem (WriteBuffer + TotalLength, (VOID*)&(QuickI2cDev->DeviceDescriptor.wDataRegister), sizeof (QuickI2cDev->DeviceDescriptor.wDataRegister));
      TotalLength += sizeof (QuickI2cDev->DeviceDescriptor.wDataRegister);
    }

    //
    // Copy Length of data field and Content itself to TxDma write buffer
    //
    DataFieldLength = (UINT16)(sizeof (DataFieldLength) + HidPacket->ReportBufferLen);
    CopyMem (WriteBuffer + TotalLength, (VOID*)&DataFieldLength, sizeof (DataFieldLength));
    TotalLength += sizeof (DataFieldLength);
    CopyMem (WriteBuffer + TotalLength, (VOID*)HidPacket->ReportBuffer, HidPacket->ReportBufferLen);
  }

  //THC_LOCAL_DEBUG (L"QuickI2cSetFeature: WriteBuffer Content: "); //uncomment for debug purpose
  //ShowBuffer (WriteBuffer, OutputReportHeader.Fields.ContentLength);
  //THC_LOCAL_DEBUG (L"QuickI2cSetFeature: OutputReportHeader Content: ");
  //ShowBuffer ((UINT8 *)&OutputReportHeader, sizeof (QUICK_I2C_OUTPUT_REPORT_HEADER));

  //
  // No response is needed for QuickI2cOutputReportSetFeature, WriteDmaPollComplete is enough
  //
  Status = QuickI2cDmaWriteTx (QuickI2cDev, OutputReportHeader, WriteBuffer);
  THC_LOCAL_DEBUG (L"QuickI2cSetFeature QuickI2cDmaWriteTx: %r\n", Status)
  if (EFI_ERROR (Status)) {
   DEBUG ((DEBUG_INFO, "QuickI2cSetFeature QuickI2cDmaWriteTx: %r, might be expected\n", Status));
   // continue
  }

  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = FALSE;

  FreePool (WriteBuffer);
  return Status;
}