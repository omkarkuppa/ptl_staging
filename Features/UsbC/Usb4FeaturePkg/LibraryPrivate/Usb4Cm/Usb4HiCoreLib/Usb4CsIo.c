/** @file
  Function implementation of USB4 configuration space access.

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

@par Specification
**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4HiCoreDefs.h>
#include <Library/Usb4CsIo.h>
#include <Library/CmUtilsLib.h>
#include "Crc32c.h"
#include "Usb4CtrlPkt.h"

/**
  Reads register data with a given address from USB4 config space.
  Operation:
  1. Prepare the read request
  2. Swap bytes
  3. Calculate CRC and swap it too
  4. Read response and swap bytes of response data

  @param[in]  This        - Pointer to USB4_CS_IO function table.
  @param[in]  TopologyId  - Pointer to Topology ID of Router.
  @param[in]  ConfigSpace - Configuration space to be read.
  @param[in]  AdpNum      - Adapter number.
  @param[in]  Offset      - Data offset in the config space.
  @param[in]  Count       - DWORD count to be read from config space.
  @param[out] Data        - Data buffer for Read data.
  @param[out] RspAdpNum   - Pointer to the response adapter number (Required for TBT3 compatible operations).

  @retval EFI_SUCCESS           - USB4 config space register read success.
  @retval EFI_UNSUPPORTED       - The request is not supported.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES  - No Tx buffer to send the control packet.
  @retval EFI_DEVICE_ERROR      - Error event is received from Rx ring.
  @retval EFI_NOT_FOUND         - ERR_ADDR notification event is received from Rx ring.
  @retval EFI_TIMEOUT           - The expected PDF packet is not received after timeout period.
  @retval EFI_NOT_READY         - The adapter is disconnected or disabled.
**/
EFI_STATUS
Usb4CsIoRead (
  IN  USB4_CS_IO           *This,
  IN  PTOPOLOGY_ID         TopologyId,
  IN  USB4_CONFIG_SPACE    ConfigSpace,
  IN  UINT8                AdpNum,
  IN  UINT16               Offset,
  IN  UINT8                Count,
  OUT UINT32               *Data,
  OUT UINT8                *RspAdpNum
  )
{
  EFI_STATUS                   Status;
  UINT32                       Len;
  UINT32                       Crc32c;
  UINT16                       ExpectedLength;
  UINT32                       Index;
  PUSB4_HI_CORE                HiCore;
  PUSB4_CONTROL_READ_REQUEST   CtrlReadReq;
  PUSB4_CONTROL_READ_RESPONSE  CtrlReadRsp;

  if ((This == NULL) || (TopologyId == NULL) || (Data == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CsIoRead: Invalid parameter (%p,%p,%p)\n", This, TopologyId, Data));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((CM_VERBOSE, "      Usb4CsIoRead: TopologyId = %016llX, cs = %d, AdpNum = %d, Reg = 0x%x, DWs = 0x%x\n",
          TopologyId->Value, ConfigSpace, AdpNum, Offset, Count));

  if (Count > USB4_CONTROL_DATA_DW_MAX) {
    DEBUG ((DEBUG_ERROR, "Usb4CsIoRead: Length(%d) > Maximum supported frame size\n", Count));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Get host interface from USB4_CS_IO pointer
  //
  HiCore = USB4_HI_CORE_FROM_CSIO (This);
  CtrlReadReq = &(HiCore->CtrlReadReq);
  CtrlReadRsp = &(HiCore->CtrlReadRsp);

  //
  // Initialize constant values for read request
  //
  CtrlReadReq->Header.RouteString.Str.High = TopologyId->Id.High;
  CtrlReadReq->Header.RouteString.Str.Low  = TopologyId->Id.Low;
  CtrlReadReq->Header.SequenceNum          = 0;
  CtrlReadReq->Header.ConfigSpace          = ConfigSpace;
  CtrlReadReq->Header.AdpNum               = AdpNum;
  CtrlReadReq->Header.Count                = Count;
  CtrlReadReq->Header.Offset               = Offset;

  //
  // Swap byte endianess of each DWORD of the control read request data excluding CRC before transmit
  //
  Len = sizeof (USB4_CONTROL_READ_REQUEST) - 4;
  CmSwapEndianness ((UINT32 *) CtrlReadReq, Len / 4, (UINT32 *) CtrlReadReq);

  CalCrc32c ((UINT8 *) CtrlReadReq, Len, &Crc32c);
  CtrlReadReq->Crc = SwapBytes32 (Crc32c);

  DEBUG ((CM_VERBOSE, "      Usb4CsIoRead: Transmit control read packet...\n"));

  // Transmit the control read request
  Status = Usb4CtrlPktSend (HiCore, (UINT8 *) CtrlReadReq, PDF_READ_REQUEST, sizeof (USB4_CONTROL_READ_REQUEST));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Ctrl read failure, %r, TopologyId=%016llX, cs=%d, AdpNum=%d, Offset=0x%x, Count=0x%x\n",
          Status, TopologyId->Value, ConfigSpace, AdpNum, Offset, Count));
    goto Exit;
  }

  //
  // Expected length of the received response packet depends on the requested read data length
  //
  ExpectedLength = (UINT16) sizeof (USB4_CONTROL_PACKET_HEADER) + Count * 4 + 4;

  //
  // Receive the read response packet
  //
  DEBUG ((CM_VERBOSE, "      Usb4CsIoRead: Receive control read response...\n"));
  Status = Usb4CtrlPktRcv (HiCore, (UINT8 *) CtrlReadRsp, PDF_READ_REQUEST, ExpectedLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Ctrl read response failure, %r, TopologyId=%016llX, cs=%d, AdpNum=%d, Reg=0x%x, DWs=0x%x\n",
          Status, TopologyId->Value, ConfigSpace, AdpNum, Offset, Count));
    goto Exit;
  }

  //
  // Swap byte endianess of each DWORD of the control read response data excluding CRC
  //
  CmSwapEndianness ((UINT32 *) CtrlReadRsp, (ExpectedLength - 4) / 4, (UINT32 *) CtrlReadRsp);

  //
  // Update the response adapter number if request
  // Upstream adapter field of Router CS1 needs to be updated for TBT3 Router enumeration
  // This is not required for USB4 compliant Router
  //
  if (RspAdpNum != NULL) {
    *RspAdpNum = (UINT8) CtrlReadRsp->Header.AdpNum;
  }

  DEBUG ((CM_VERBOSE, "      Usb4CsIoRead: Response received, cs = %d, addr = 0x%x, size = %d DWs, AdpNum = %d\n",
          CtrlReadRsp->Header.ConfigSpace,
          CtrlReadRsp->Header.Offset,
          CtrlReadRsp->Header.Count,
          CtrlReadRsp->Header.AdpNum
          ));

  DEBUG ((CM_VERBOSE, "      Read response data (Hex): "));
  // Prepare the read data for return
  for (Index = 0; Index < Count; Index++) {
    Data[Index] = CtrlReadRsp->Data[Index];
    DEBUG ((CM_VERBOSE, "%08x ", Data[Index]));
  }
  DEBUG ((CM_VERBOSE, "\n"));

Exit:
  DEBUG ((CM_VERBOSE, "      Usb4CsIoRead exit\n"));
  return Status;
}

/**
  Write data to a given register in USB4 config space
  Operation:
  1. Prepare the write request
  2. Swap bytes
  3. Calculate CRC and swap it

  @param[in] This         - Pointer to USB4_CS_IO function table.
  @param[in] TopologyId   - Pointer to Topology ID of Router.
  @param[in] ConfigSpace  - Configuration space to be written.
  @param[in] AdpNum       - Adapter number
  @param[in] Offset       - Data offset in the config space.
  @param[in] Count        - DWORD count to be written to config space.
  @param[in] Data         - Data buffer for USB4 config space read.

  @retval EFI_SUCCESS           - Write data to USB4 register successfully.
  @retval EFI_UNSUPPORTED       - The request is not supported.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES  - No Tx buffer to send the control packet.
  @retval EFI_DEVICE_ERROR      - Error event is received from Rx ring.
  @retval EFI_NOT_FOUND         - ERR_ADDR notification event is received from Rx ring.
  @retval EFI_TIMEOUT           - The expected PDF packet is not received after timeout period.
  @retval EFI_NOT_READY         - The adapter is disconnected or disabled.
**/
EFI_STATUS
Usb4CsIoWrite (
  IN USB4_CS_IO           *This,
  IN PTOPOLOGY_ID         TopologyId,
  IN USB4_CONFIG_SPACE    ConfigSpace,
  IN UINT8                AdpNum,
  IN UINT16               Offset,
  IN UINT8                Count,
  IN UINT32               *Data
  )
{
  EFI_STATUS                      Status;
  UINT16                          Len;
  UINT32                          Crc32c;
  UINT16                          Index;
  UINT16                          ExpectedLength;
  PUSB4_HI_CORE                   HiCore;
  PUSB4_CONTROL_WRITE_REQUEST     CtrlWriteReq;
  PUSB4_CONTROL_WRITE_RESPONSE    CtrlWriteRsp;

  if ((This == NULL) || (TopologyId == NULL) || (Data == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CsIoWrite: Invalid parameter (%p,%p,%p)\n", This, TopologyId, Data));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((CM_VERBOSE, "      Usb4CsIoWrite: TopologyId = %016llX, cs = %d, AdpNum = %d, Reg = 0x%x, DWs = 0x%x\n",
          TopologyId->Value, ConfigSpace, AdpNum, Offset, Count));

  if(Count > USB4_CONTROL_DATA_DW_MAX) {
    DEBUG ((DEBUG_ERROR, "Usb4CsIoWrite: Length(%d) > Maximum supported frame size. Exiting...\n", Count));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  HiCore = USB4_HI_CORE_FROM_CSIO (This);
  CtrlWriteReq = &(HiCore->CtrlWriteReq);
  CtrlWriteRsp = &(HiCore->CtrlWriteRsp);

  //
  // Setup control write request header
  //
  CtrlWriteReq->Header.RouteString.Str.High = TopologyId->Id.High;
  CtrlWriteReq->Header.RouteString.Str.Low  = TopologyId->Id.Low;
  CtrlWriteReq->Header.SequenceNum          = 0;
  CtrlWriteReq->Header.ConfigSpace          = ConfigSpace;
  CtrlWriteReq->Header.AdpNum               = AdpNum;
  CtrlWriteReq->Header.Offset               = Offset;
  CtrlWriteReq->Header.Count                = Count;

  for (Index = 0; Index < Count; Index++) {
    CtrlWriteReq->Data[Index] = Data[Index];
  }

  //
  // Swap byte endianess of each DWORD of the control write request data excluding CRC
  //
  Len = sizeof (USB4_CONTROL_PACKET_HEADER) + (Count * 4);
  CmSwapEndianness ((UINT32 *) CtrlWriteReq, Len / 4, (UINT32 *) CtrlWriteReq);

  //
  // Calculate CRC and swap byte endianess of CRC before insert to data
  //
  CalCrc32c ((UINT8 *) CtrlWriteReq, Len, &Crc32c);
  CtrlWriteReq->Data[Count] = SwapBytes32 (Crc32c);

  //
  // Transmit the control write request
  //
  Status = Usb4CtrlPktSend (HiCore, (UINT8 *) CtrlWriteReq, PDF_WRITE_REQUEST, Len + 4);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Ctrl write failure, %r, TopologyId = %016llX, cs = %d, AdpNum = %d, reg = 0x%x, DWs = %d\n",
            Status, TopologyId->Value, ConfigSpace, AdpNum, Offset, Count));
    goto Exit;
  }

  ExpectedLength = (UINT16) sizeof (USB4_CONTROL_WRITE_RESPONSE);

  //
  // Receive the write response packet
  //
  DEBUG ((CM_VERBOSE, "      Usb4CsIoWrite: Receive control write response...\n"));
  Status = Usb4CtrlPktRcv (HiCore, (UINT8 *) CtrlWriteRsp, PDF_WRITE_REQUEST, ExpectedLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Ctrl Write response failure, %r, TopologyId=%016llX, cs=%d, AdpNum=%d, reg=0x%x, DWs=%d\n",
            Status, TopologyId->Value, ConfigSpace, AdpNum, Offset, Count));
    goto Exit;
  }

  //
  // Swap byte endianess of each DWORD of the control write response data excluding CRC
  //
  CmSwapEndianness ((UINT32 *) CtrlWriteRsp, sizeof (USB4_CONTROL_PACKET_HEADER) / 4, (UINT32 *) CtrlWriteRsp);
  DEBUG ((CM_VERBOSE, "      Usb4CsIoWrite: Write response received, cs = %d, addr = 0x%x, write size = %d DWs\n",
          CtrlWriteRsp->Header.ConfigSpace, CtrlWriteRsp->Header.Offset, CtrlWriteRsp->Header.Count));

Exit:
  DEBUG ((CM_VERBOSE, "      Usb4CsIoWrite exit\n"));
  return Status;
}

/**
  Query Upstream Adapter number of Router.
  Used for TBT3 compatible operations.

  @param[in]  This       - Pointer to USB4_CS_IO function table.
  @param[in]  TopologyId - Pointer to Topology ID of Router.
  @param[out] UpAdpNum   - Pointer to Upstream Adapter number.

  @retval EFI_SUCCESS           - Query upstream adapter number success.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Query upstream adapter number failure.
**/
EFI_STATUS
Usb4CsIoQueryRtUpAdp (
  IN  USB4_CS_IO      *This,
  IN  PTOPOLOGY_ID    TopologyId,
  OUT UINT8           *UpAdpNum
  )
{
  EFI_STATUS       Status;
  UINT32           RtCs0;

  if ((This == NULL) || (UpAdpNum == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CsIoQueryRtUpAdp: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // For a Response to a Request that targets Router Configuration Space, Adapter Number field shall contain
  // the Adapter Number on which the associated Read Request arrived.
  // Default Upstream Adapter of TBT3 Router CS1 doesn't indicate real Upstream Adapter number.
  // For TBT3 Router, query the real Upstream Adapter number by checking the Adapter number of Router CS read response.
  //
  Status = Usb4CsIoRead (This, TopologyId, USB4_CS_ROUTER, 0, USB4_CS_0, 1, &RtCs0, UpAdpNum);
  if (EFI_ERROR (Status)) {
    Status = EFI_UNSUPPORTED;
  }
Exit:
  return Status;
}

/**
  Increment reference count to host interface core.
  The host interface core can't be released if the reference count is non-zero.

  @param[in] This - Pointer to USB4_CS_IO function table.
**/
VOID
Usb4CsIoAddRef (
  IN USB4_CS_IO    *This
  )
{
  PUSB4_HI_CORE    HiCore;

  if (This != NULL) {
    HiCore = USB4_HI_CORE_FROM_CSIO (This);
    HiCore->Usb4CsIoRef++;
    DEBUG ((DEBUG_INFO, "Usb4CsIoAddRef: Ref = %d\n", HiCore->Usb4CsIoRef));
  }
}

/**
  Decrement reference count to host interface core.
  The host interface core can't be released if the reference count is non-zero.

  @param[in] This - Pointer to USB4_CS_IO function table.
**/
VOID
Usb4CsIoReleaseRef (
  IN USB4_CS_IO    *This
  )
{
  PUSB4_HI_CORE    HiCore;

  if (This != NULL) {
    HiCore = USB4_HI_CORE_FROM_CSIO (This);
    if (HiCore->Usb4CsIoRef > 0) {
      HiCore->Usb4CsIoRef--;
      DEBUG ((DEBUG_INFO, "Usb4CsIoReleaseRef: Ref = %d\n", HiCore->Usb4CsIoRef));
    } else {
      DEBUG ((DEBUG_ERROR, "Invalid USB4_CS_IO reference release!\n"));
    }
  }
}

/**
  Initialize USB4_CS_IO function table

  @param[out] This - Pointer to USB4_CS_IO function table.
**/
EFI_STATUS
Usb4CsIoInit (
  OUT USB4_CS_IO    *This
  )
{
  EFI_STATUS    Status;

  DEBUG ((DEBUG_INFO, "Usb4CsIoInit entry\n"));

  if (This != NULL) {
    This->CsRead       = Usb4CsIoRead;
    This->CsWrite      = Usb4CsIoWrite;
    This->AddRef       = Usb4CsIoAddRef;
    This->ReleaseRef   = Usb4CsIoReleaseRef;
    This->QueryRtUpAdp = Usb4CsIoQueryRtUpAdp;
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "Usb4CsIoInit exit\n"));
  return Status;
}