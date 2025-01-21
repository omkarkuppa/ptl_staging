/** @file
  USB4 domain management for DP arrival/removal.

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
#include <Library/DebugLib.h>
#include <Library/Usb4CsLib.h>
#include <Library/Usb4RouterLib.h>
#include <Library/Usb4ProtocolsLib.h>
#include <Library/Usb4Capabilities.h>
#include <Library/Usb4DebugLib.h>
#include <Library/Usb4ConfigSpace.h>
#include <Library/Usb4DomainDefs.h>

extern EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log;

/**
  Look for new DP stream with DP source and sink available to enable.
  Only 1 DP stream is supported in pre-boot for USB4 Ver2.
  Enable new DP stream only if no DP stream is enabled in Domain.

  @param[in] Domain             - Pointer to USB4 domain

  @retval EFI_SUCCESS           - Enable new DP Tunnel success.
  @retval EFI_UNSUPPORTED       - Fail to enable new DP Tunnel.
  @retval EFI_OUT_OF_RESOURCES  - Fail to get DP resources to enable DP tunnel.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_NOT_FOUND         - No other DP stream meeting the requirements can be found.
**/
EFI_STATUS
Usb4EnableNewDpStream (
  IN PUSB4_DOMAIN           Domain
  )
{
  EFI_STATUS         Status;
  PUSB4_DP_STREAM    NewDpStream;
  UINT32             NewIndex;

  DEBUG ((DEBUG_INFO, "Usb4EnableNewDpStream entry\n"));

  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4EnableNewDpStream: Invalid Domain %p\n", Domain));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Only one DP stream should be enabled in pre-boot with USB4 Ver2
  //
  if (Domain->DpStreamCount != 0) {
    DEBUG ((DEBUG_ERROR, "Usb4EnableNewDpStream: DP stream has been enabled, skip enabling new DP stream\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = EFI_NOT_FOUND;
  //
  // Look for DP stream that both DP source and sink are available but is not enabled.
  //
  for (NewIndex = 0; NewIndex < USB4_DP_STREAM_MAX; NewIndex++) {
    NewDpStream = &(Domain->DpStream[NewIndex]);
    if ((NewDpStream->DpSrcRouter != NULL) && (NewDpStream->DpSinkRouter != NULL) && !NewDpStream->StreamReady) {
      DEBUG ((DEBUG_INFO, "New DP stream found to be enabled - Rt 0x%016llX Adp %d <-> Rt 0x%016llX Adp %d\n",
              NewDpStream->DpSrcRouter->TopologyId,
              NewDpStream->DpSrcAdpNum,
              NewDpStream->DpSinkRouter->TopologyId,
              NewDpStream->DpSinkAdpNum
            ));
      //
      // Enable new DP stream
      //
      Status = Usb4EnableDpTunnel (
                NewDpStream->DpSrcRouter,
                NewDpStream->DpSrcAdpNum,
                NewDpStream->DpSinkRouter,
                NewDpStream->DpSinkAdpNum,
                NewIndex);
      if (EFI_ERROR (Status)) {
        //
        // Fail to enable DP tunneling of new DP stream
        // Keep search if another new DP stream can be enabled
        //
        DEBUG ((DEBUG_ERROR, "Usb4EnableNewDpStream: Enable new DP stream %d failure, %r\n", NewIndex, Status));
      } else {
        NewDpStream->StreamReady = TRUE;
        Domain->DpStreamCount++;
        Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSTREAM_ENABLED, NewIndex, 0);
        DEBUG ((DEBUG_INFO, "Enable DP stream %d. DP stream count = %d\n", NewIndex, Domain->DpStreamCount));
        break;
      }
    }
  } // for (NewIndex = 0; NewIndex < USB4_DP_STREAM_MAX; NewIndex++)

Exit:
  DEBUG ((DEBUG_INFO, "Usb4EnableNewDpStream exit, %r\n", Status));
  return Status;
}

/**
  Add a DP source to USB4 domain.

  @param[in] DomainContext - USB4 domain context.
  @param[in] Router        - Pointer to router instance.
  @param[in] AdpNum        - DP adapter number.

  @retval EFI_SUCCESS           - Add a DP source to USB4 domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to add a DP source to USB4 domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainDpSrcAdpAdd (
  IN USB4_DOMAIN_CONTEXT    DomainContext,
  IN PUSB4_ROUTER           Router,
  IN UINT8                  AdpNum
  )
{
  EFI_STATUS         Status;
  PUSB4_DOMAIN       Domain;
  PUSB4_ADAPTER      DpAdp;
  PUSB4_DP_STREAM    DpStream;
  UINT32             OpStatus;
  UINT32             Index;
  UINT32             StreamId;

  DEBUG ((DEBUG_INFO, "Usb4DomainDpSrcAdpAdd entry\n"));

  Status = EFI_SUCCESS;
  Domain = (PUSB4_DOMAIN) DomainContext;

  if ((Domain == NULL) || (Router == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSrcAdpAdd: Invalid parameter (%p, %p)\n", Domain, Router));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Domain->Signature != USB4_DOMAIN_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSrcAdpAdd: Invalid USB4 domain signature 0x%08x\n", Domain->Signature));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Router->TopologyId.Value != 0) {
    DEBUG ((DEBUG_ERROR, "DP source from the device router is not supported!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSrcAdpAdd: Invalid adpnum %d\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DpAdp = &(Router->Adapter[AdpNum]);
  if (DpAdp->Type != USB4_ADP_TYPE_DP_IN) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSrcAdpAdd: Rt 0x%016llX Adp %d is not DP IN\n", Router->TopologyId, AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Check if it's a duplicated request
  //
  for (Index = 0; Index < USB4_DP_STREAM_MAX; Index++) {
    if (Domain->DpStream[Index].DpSrcRouter != NULL) {
      if ((Domain->DpStream[Index].DpSrcRouter->TopologyId.Value == Router->TopologyId.Value) &&
          (Domain->DpStream[Index].DpSrcAdpNum == AdpNum)) {
        DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d DP source has been added, ignore request!\n", Router->TopologyId, AdpNum));
        goto Exit;
      }
    }
  }

  //
  // Find a DP stream that DP source is not present.
  //
  StreamId = 0xFFFFFFFF;
  for (Index = 0; Index < USB4_DP_STREAM_MAX; Index++) {
    //
    // The first priority is to search DP stream that no DP source available but DP sink available
    // We can enable DP tunneling immediately
    //
    if (Domain->DpStream[Index].DpSrcRouter == NULL) {
      if (Domain->DpStream[Index].DpSinkRouter != NULL) {
        StreamId = Index;
        break;
      } else if (StreamId == 0xFFFFFFFF) {
        //
        // The first available DP stream will be selected if no DP sink in other DP stream can be paired.
        //
        StreamId = Index;
      }
    }
  }

  if (StreamId == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "<U4CmErr> Not able to add more DP source into domain!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query DP resources to see if it can be added to domain as DP source.
  //
  OpStatus = RT_OP_STATUS_SUCCESS;
  Status = Usb4DpRtOpExec (Router, RT_OP_QUERY_DP_RESOURCES, AdpNum, &OpStatus);
  DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d Query DP Resources, OpStatus = 0x%0x\n", Router->TopologyId, AdpNum, OpStatus));
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPRES_QUERY, (UINT32) Status, OpStatus);
  if (EFI_ERROR (Status) || RT_OP_STATUS_ERROR (OpStatus)) {
    DEBUG ((DEBUG_ERROR, "Query DP resources failure, status = %r, opstatus = 0x%0x\n", Status, OpStatus));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Add Rt 0x%016llX Adp %d to DP stream %d as DP source\n", Router->TopologyId, AdpNum, StreamId));
  DpStream = &(Domain->DpStream[StreamId]);
  DpStream->DpSrcRouter = Router;
  DpStream->DpSrcAdpNum = AdpNum;
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSRC_ADD, (UINT32) AdpNum, 0);

  //
  // Enable DP tunnel if the DP sink is available.
  //
  if (DpStream->DpSinkRouter != NULL) {
    //
    // Only one DP stream should be enabled in pre-boot with USB4 Ver2
    //
    if (Domain->DpStreamCount == 0) {
      Status = Usb4EnableDpTunnel (Router, AdpNum, DpStream->DpSinkRouter, DpStream->DpSinkAdpNum, StreamId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "<U4CmErr> Usb4DomainDpSrcAdpAdd: Enable DP tunneling failure, %r\n", Status));
        goto Exit;
      }
      Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSTREAM_ENABLED, StreamId, 0);
      DpStream->StreamReady = TRUE;
      Domain->DpStreamCount++;
      DEBUG ((DEBUG_INFO, "Enable new DP stream %d. DP stream count = %d\n", StreamId, Domain->DpStreamCount));
    } else {
      DEBUG ((DEBUG_INFO, "DP stream %d is not enabled though DP source and sink are available.\n", StreamId));
    }
  } // if (DpStream->DpSinkRouter != NULL)

Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainDpSrcAdpAdd exit\n"));
  return Status;
}

/**
  Remove a DP source from USB4 domain.

  @param[in] DomainContext - USB4 domain context.
  @param[in] Router        - Pointer to router instance.
  @param[in] AdpNum        - DP adapter number.

  @retval EFI_SUCCESS           - Add a DP source to USB4 domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to add a DP source to USB4 domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainDpSrcAdpRemove (
  IN USB4_DOMAIN_CONTEXT    DomainContext,
  IN PUSB4_ROUTER           Router,
  IN UINT8                  AdpNum
  )
{
  EFI_STATUS         Status;
  PUSB4_DOMAIN       Domain;
  PUSB4_ADAPTER      DpAdp;
  PUSB4_DP_STREAM    DpStream;
  PUSB4_DP_STREAM    NewDpStream;
  UINT32             Index;
  UINT32             NewIndex;

  DEBUG ((DEBUG_INFO, "Usb4DomainDpSrcAdpRemove entry\n"));

  Status = EFI_SUCCESS;

  Domain = (PUSB4_DOMAIN) DomainContext;

  if ((Domain == NULL) || (Router == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSrcAdpRemove: Invalid parameter (%p, %p)\n", Domain, Router));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Domain->Signature != USB4_DOMAIN_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSrcAdpRemove: Invalid USB4 domain signature 0x%08x\n", Domain->Signature));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Router->TopologyId.Value != 0) {
    DEBUG ((DEBUG_ERROR, "DP source from the device router is not supported!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSrcAdpRemove: Invalid adpnum %d\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DpAdp = &(Router->Adapter[AdpNum]);
  if (DpAdp->Type != USB4_ADP_TYPE_DP_IN) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSrcAdpRemove: Rt 0x%016llX Adp %d is not DP IN\n", Router->TopologyId, AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Check if the given router/adapter is DP source in the domain.
  //
  for (Index = 0; Index < USB4_DP_STREAM_MAX; Index++) {
    DpStream = &(Domain->DpStream[Index]);
    if ((DpStream->DpSrcRouter == Router) && (DpStream->DpSrcAdpNum == AdpNum)) {
      break;
    }
  }

  //
  // Do nothing if the given router/adapter is not a DP source in the domain.
  //
  if (Index == USB4_DP_STREAM_MAX) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d is not found in DP source of DP stream, ignore request!\n", Router->TopologyId, AdpNum));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Remove DP source Rt 0x%016llX Adp %d from DP stream %d\n", Router->TopologyId, AdpNum, Index));
  DpStream->DpSrcRouter = NULL;
  DpStream->DpSrcAdpNum = 0;
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSRC_REMOVE, (UINT32) AdpNum, 0);

  //
  // Disable DP tunnel if it has been enabled.
  //
  if (DpStream->StreamReady) {
    Status = Usb4DisableDpTunnel (Router, AdpNum, DpStream->DpSinkRouter, DpStream->DpSinkAdpNum, Index, NULL);
    Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSTREAM_DISABLED, Index, 0);
    DpStream->StreamReady = FALSE;
    if (Domain->DpStreamCount > 0) {
      Domain->DpStreamCount--;
      DEBUG ((DEBUG_INFO, "DP stream %d is disabled. DP stream count = %d\n", Index, Domain->DpStreamCount));
    } else {
      //
      // Something wrong in DP stream count handling, but doesn't have critical impact to the remaining actions
      // It's not necessary to return failure directly
      // This condition is not expected to happen.
      // Use ASSERT to highlight the abnormal condition.
      //
      DEBUG ((DEBUG_ERROR, "DpStreamCount == 0 before decrement is not expected!\n"));
      ASSERT (FALSE);
    }
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4DomainDpSrcAdpRemove: Disable DP tunnel failure, %r\n", Status));
      goto Exit;
    }
    //
    // Check if another DP source is available to pair the DP sink of original DP stream
    //
    for (NewIndex = 0; NewIndex < USB4_DP_STREAM_MAX; NewIndex++) {
      if (NewIndex == Index) {
        continue;
      }
      NewDpStream = &(Domain->DpStream[NewIndex]);
      if ((NewDpStream->DpSrcRouter != NULL) && (NewDpStream->DpSinkRouter == NULL)) {
        DEBUG ((DEBUG_INFO, "New DP source found to enable DP stream - Rt 0x%016llX Adp %d\n",
                NewDpStream->DpSrcRouter->TopologyId,
                NewDpStream->DpSrcAdpNum
              ));
        //
        // Move the DP sink of the original DP stream to new DP stream and enable DP tunneling
        //
        NewDpStream->DpSinkRouter = DpStream->DpSinkRouter;
        NewDpStream->DpSinkAdpNum = DpStream->DpSinkAdpNum;
        Status = Usb4EnableDpTunnel (
                   NewDpStream->DpSrcRouter,
                   NewDpStream->DpSrcAdpNum,
                   NewDpStream->DpSinkRouter,
                   NewDpStream->DpSinkAdpNum,
                   NewIndex);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Usb4DomainDpSrcAdpRemove: Enable new DP stream %d failure, %r\n", NewIndex, Status));
          //
          // Clear DP sink information of new DP stream and keep searching for pairing DP stream.
          //
          NewDpStream->DpSinkRouter = NULL;
          NewDpStream->DpSinkAdpNum = 0;
        } else {
          //
          // Clear DP sink information of original DP stream and set new DP stream ready
          //
          DpStream->DpSinkRouter = NULL;
          DpStream->DpSinkAdpNum = 0;
          NewDpStream->StreamReady = TRUE;
          Domain->DpStreamCount++;
          Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSTREAM_ENABLED, NewIndex, 0);
          DEBUG ((DEBUG_INFO, "DP stream %d is enabled. DP stream count = %d\n", NewIndex, Domain->DpStreamCount));
          break;
        }
      }
    } // for (NewIndex = 0; NewIndex < USB4_DP_STREAM_MAX; NewIndex++)
    //
    // Only 1 DP stream is supported in pre-boot for USB4 Ver2.
    // It's possible that another DP stream with DP source and sink available but is not enabled.
    // Look for such DP stream to enable if no new DP stream is enabled of the original DP stream.
    //
    if (NewIndex >= USB4_DP_STREAM_MAX) {
      Status = Usb4EnableNewDpStream (Domain);
      if (Status == EFI_SUCCESS) {
        DEBUG ((DEBUG_INFO, "New DP stream is enabled after DP Source removal\n"));
      }
    }
  } // if (DpStream->StreamReady)

Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainDpSrcAdpRemove exit\n"));
  return Status;
}

/**
  Add a DP sink to USB4 domain.

  @param[in] DomainContext - USB4 domain context.
  @param[in] Router        - Pointer to router instance.
  @param[in] AdpNum        - DP adapter number.

  @retval EFI_SUCCESS           - Add a DP sink to USB4 domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to add a DP sink to USB4 domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainDpSinkAdpAdd (
  IN USB4_DOMAIN_CONTEXT    DomainContext,
  IN PUSB4_ROUTER           Router,
  IN UINT8                  AdpNum
  )
{
  EFI_STATUS                  Status;
  PUSB4_DOMAIN                Domain;
  PUSB4_ADAPTER               DpAdp;
  PUSB4_DP_STREAM             DpStream;
  UINT32                      Index;
  UINT32                      StreamId;
  USB4_ADP_CAP_DP_OUT_CS_2    CapCs2;

  DEBUG ((DEBUG_INFO, "Usb4DomainDpSinkAdpAdd entry\n"));

  Status = EFI_SUCCESS;
  Domain = (PUSB4_DOMAIN) DomainContext;

  if ((Domain == NULL) || (Router == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSinkAdpAdd: Invalid parameter (%p, %p)\n", Domain, Router));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Domain->Signature != USB4_DOMAIN_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSinkAdpAdd: Invalid USB4 domain signature 0x%08x\n", Domain->Signature));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSinkAdpAdd: Invalid adpnum %d\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DpAdp = &(Router->Adapter[AdpNum]);
  if (DpAdp->Type != USB4_ADP_TYPE_DP_OUT) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSinkAdpAdd: Rt 0x%016llX Adp %d is not DP OUT\n", Router->TopologyId, AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Check if it's a duplicated request
  //
  for (Index = 0; Index < USB4_DP_STREAM_MAX; Index++) {
    if (Domain->DpStream[Index].DpSinkRouter != NULL) {
      if ((Domain->DpStream[Index].DpSinkRouter->TopologyId.Value == Router->TopologyId.Value) &&
          (Domain->DpStream[Index].DpSinkAdpNum == AdpNum)) {
        DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d DP sink has been added, ignore request!\n", Router->TopologyId, AdpNum));
        goto Exit;
      }
    }
  }

  //
  // Find a DP stream that DP sink is not present.
  //
  StreamId = 0xFFFFFFFF;
  for (Index = 0; Index < USB4_DP_STREAM_MAX; Index++) {
    //
    // The first priority is to search DP stream that no DP sink available but DP source available
    // We can enable DP tunneling immediately
    //
    if (Domain->DpStream[Index].DpSinkRouter == NULL) {
      if (Domain->DpStream[Index].DpSrcRouter != NULL) {
        StreamId = Index;
        break;
      } else if (StreamId == 0xFFFFFFFF) {
        //
        // The first available DP stream will be selected if no DP source in other DP stream can be paired.
        //
        StreamId = Index;
      }
    }
  }

  if (StreamId == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "<U4CmErr> Not able to add more DP sink into domain!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Check if HPD bit is set
  //
  Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->AdpCapReg + USB4_CS_2, 1, (UINT32 *) &CapCs2);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read DP capability failure, %r\n", Status));
    goto Exit;
  }
  if (CapCs2.Fields.Hpd == 0) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d HPD bit is not set!",Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Add Rt 0x%016llX Adp %d to DP stream %d as DP sink\n", Router->TopologyId, AdpNum, StreamId));
  DpStream = &(Domain->DpStream[StreamId]);
  DpStream->DpSinkRouter = Router;
  DpStream->DpSinkAdpNum = AdpNum;
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSINK_ADD, (UINT32) AdpNum, 0);

  //
  // Enable DP tunnel if the DP source is available.
  //
  if (DpStream->DpSrcRouter != NULL) {
    //
    // Only one DP stream should be enabled in pre-boot with USB4 Ver2
    //
    if (Domain->DpStreamCount == 0) {
      Status = Usb4EnableDpTunnel (DpStream->DpSrcRouter, DpStream->DpSrcAdpNum, Router, AdpNum, StreamId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "<U4CmErr> Usb4DomainDpSinkAdpAdd: Enable DP tunneling failure, %r\n", Status));
        goto Exit;
      }
      Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSTREAM_ENABLED, StreamId, 0);
      DpStream->StreamReady = TRUE;
      Domain->DpStreamCount++;
      DEBUG ((DEBUG_INFO, "Enable new DP stream %d. DP stream count = %d\n", StreamId, Domain->DpStreamCount));
    } else {
      DEBUG ((DEBUG_INFO, "DP stream %d is not enabled though DP source and sink are available.\n", StreamId));
    }
  } // if (DpStream->DpSrcRouter != NULL)

Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainDpSinkAdpAdd exit\n"));
  return Status;
}

/**
  Remove a DP sink from USB4 domain.

  @param[in] DomainContext - USB4 domain context.
  @param[in] Router        - Pointer to router instance.
  @param[in] AdpNum        - DP adapter number.

  @retval EFI_SUCCESS           - Remove a DP sink from USB4 domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to remove a DP sink from USB4 domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainDpSinkAdpRemove (
  IN USB4_DOMAIN_CONTEXT    DomainContext,
  IN PUSB4_ROUTER           Router,
  IN UINT8                  AdpNum
  )
{
  EFI_STATUS         Status;
  PUSB4_DOMAIN       Domain;
  PUSB4_ADAPTER      DpAdp;
  PUSB4_DP_STREAM    DpStream;
  PUSB4_DP_STREAM    NewDpStream;
  UINT32             Index;
  UINT32             NewIndex;

  DEBUG ((DEBUG_INFO, "Usb4DomainDpSinkAdpRemove entry\n"));

  Status = EFI_SUCCESS;
  Domain = (PUSB4_DOMAIN) DomainContext;

  if ((Domain == NULL) || (Router == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSinkAdpRemove: Invalid parameter (%p, %p)\n", Domain, Router));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Domain->Signature != USB4_DOMAIN_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSinkAdpRemove: Invalid USB4 domain signature 0x%08x\n", Domain->Signature));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSinkAdpRemove: Invalid adpnum %d\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DpAdp = &(Router->Adapter[AdpNum]);
  if (DpAdp->Type != USB4_ADP_TYPE_DP_OUT) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpSinkAdpRemove: Rt 0x%016llX Adp %d is not DP OUT\n", Router->TopologyId, AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Check if the given router/adapter is DP sink in the domain.
  //
  for (Index = 0; Index < USB4_DP_STREAM_MAX; Index++) {
    DpStream = &(Domain->DpStream[Index]);
    if ((DpStream->DpSinkRouter == Router) && (DpStream->DpSinkAdpNum == AdpNum)) {
      break;
    }
  }

  //
  // Do nothing if the given router/adapter is not a DP source in the domain.
  //
  if (Index == USB4_DP_STREAM_MAX) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d is not found in DP sink of DP stream, ignore request!\n", Router->TopologyId, AdpNum));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Remove DP sink Rt 0x%016llX Adp %d from DP stream %d\n", Router->TopologyId, AdpNum, Index));
  DpStream->DpSinkRouter = NULL;
  DpStream->DpSinkAdpNum = 0;
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSINK_REMOVE, (UINT32) AdpNum, 0);

  //
  // Disable DP tunnel if it has been enabled.
  //
  if (DpStream->StreamReady) {
    Status = Usb4DisableDpTunnel (DpStream->DpSrcRouter, DpStream->DpSrcAdpNum, Router, AdpNum, Index, NULL);
    DpStream->StreamReady = FALSE;
    Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSTREAM_DISABLED, Index, 0);
    if (Domain->DpStreamCount > 0) {
      Domain->DpStreamCount--;
      DEBUG ((DEBUG_INFO, "DP stream %d is disabled. DP stream count = %d\n", Index, Domain->DpStreamCount));
    } else {
      //
      // Something wrong in DP stream count handling, but doesn't have critical impact to the remaining actions
      // It's not necessary to return failure directly
      // This condition is not expected to happen.
      // Use ASSERT to highlight the abnormal condition.
      //
      DEBUG ((DEBUG_ERROR, "DpStreamCount == 0 before decrement is not expected!\n"));
      ASSERT (FALSE);
    }
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4DomainDpSinkAdpRemove: Disable DP tunnel failure, %r\n", Status));
      goto Exit;
    }
    //
    // Check if another DP sink is available to pair the DP source of original DP stream
    //
    for (NewIndex = 0; NewIndex < USB4_DP_STREAM_MAX; NewIndex++) {
      if (NewIndex == Index) {
        continue;
      }
      NewDpStream = &(Domain->DpStream[NewIndex]);
      if ((NewDpStream->DpSinkRouter != NULL) && (NewDpStream->DpSrcRouter == NULL)) {
        DEBUG ((DEBUG_INFO, "New DP sink found to enable DP stream - Rt 0x%016llX Adp %d\n",
                NewDpStream->DpSinkRouter->TopologyId,
                NewDpStream->DpSinkAdpNum
              ));
        //
        // Move the DP source of the original DP stream to new DP stream and enable DP tunneling
        //
        NewDpStream->DpSrcRouter = DpStream->DpSrcRouter;
        NewDpStream->DpSrcAdpNum = DpStream->DpSrcAdpNum;
        Status = Usb4EnableDpTunnel (
                   NewDpStream->DpSrcRouter,
                   NewDpStream->DpSrcAdpNum,
                   NewDpStream->DpSinkRouter,
                   NewDpStream->DpSinkAdpNum,
                   NewIndex);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Usb4DomainDpSinkAdpRemove: Enable new DP stream %d failure, %r\n", NewIndex, Status));
          //
          // Clear DP source information of new DP stream and keep searching for pairing DP stream.
          //
          NewDpStream->DpSrcRouter = NULL;
          NewDpStream->DpSrcAdpNum = 0;
        } else {
          //
          // Clear DP source information of original DP stream and set new DP stream ready
          //
          DpStream->DpSrcRouter = NULL;
          DpStream->DpSrcAdpNum = 0;
          NewDpStream->StreamReady = TRUE;
          Domain->DpStreamCount++;
          Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSTREAM_ENABLED, NewIndex, 0);
          DEBUG ((DEBUG_INFO, "DP stream %d is enabled. DP stream count = %d\n", NewIndex, Domain->DpStreamCount));
          break;
        }
      }
    } // for (NewIndex = 0; NewIndex < USB4_DP_STREAM_MAX; NewIndex++)

    //
    // Only 1 DP stream is supported in pre-boot for USB4 Ver2.
    // It's possible that another DP stream with DP source and sink available but is not enabled.
    // Look for such DP stream to enable if no new DP stream is enabled of the original DP stream.
    //
    if (NewIndex >= USB4_DP_STREAM_MAX) {
      Status = Usb4EnableNewDpStream (Domain);
      if (Status == EFI_SUCCESS) {
        DEBUG ((DEBUG_INFO, "New DP stream is enabled after DP Sink removal\n"));
      }
    }
  } // if (DpStream->StreamReady)

Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainDpSinkAdpRemove exit\n"));
  return Status;
}

/**
  Remove DP source or sink from domain for router removal.

  @param[in] DomainContext - USB4 domain context.
  @param[in] Router        - Removed router.

  @retval EFI_SUCCESS           - Remove DP source or sink from domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to remove DP source or sink from domain for router removal.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainDpDisableForRtRemoval (
  IN USB4_DOMAIN_CONTEXT    DomainContext,
  IN PUSB4_ROUTER           Router
  )
{
  EFI_STATUS         Status;
  PUSB4_DOMAIN       Domain;
  PUSB4_DP_STREAM    DpStream;
  PUSB4_ROUTER       DpSrcRt;
  PUSB4_ROUTER       DpSinkRt;
  UINT8              DpSrcAdpNum;
  UINT8              DpSinkAdpNum;
  UINT32             Index;
  BOOLEAN            DpRemoval;

  DEBUG ((DEBUG_INFO, "Usb4DomainDpDisableForRtRemoval entry\n"));

  Status = EFI_SUCCESS;

  Domain = (PUSB4_DOMAIN) DomainContext;

  if ((Domain == NULL) || (Router == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpDisableForRtRemoval: Invalid parameter (%p, %p)\n", Domain, Router));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Domain->Signature != USB4_DOMAIN_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDpDisableForRtRemoval: Invalid USB4 domain signature 0x%08x\n", Domain->Signature));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  for (Index = 0; Index < USB4_DP_STREAM_MAX; Index++) {
    DpStream = &(Domain->DpStream[Index]);
    DpRemoval = FALSE;
    if (DpStream->StreamReady) {
      DpSrcRt = DpStream->DpSrcRouter;
      DpSrcAdpNum = DpStream->DpSrcAdpNum;
      DpSinkRt = DpStream->DpSinkRouter;
      DpSinkAdpNum = DpStream->DpSinkAdpNum;
    }
    //
    // Check if DP source router is under the removed router.
    //
    if ((DpStream->DpSrcRouter != NULL) && Usb4RtInRtTree (DpStream->DpSrcRouter, Router)) {
      DpStream->DpSrcRouter = NULL;
      DpStream->DpSrcAdpNum = 0;
      DpRemoval = TRUE;
    }
    //
    // Check if DP sink router is under the removed router.
    //
    if ((DpStream->DpSinkRouter != NULL) && Usb4RtInRtTree (DpStream->DpSinkRouter, Router)) {
      DpStream->DpSinkRouter = NULL;
      DpStream->DpSinkAdpNum = 0;
      DpRemoval = TRUE;
    }

    if (DpStream->StreamReady && DpRemoval) {
      //
      // Disable and tear down DP tunnel path if DP tunnel has been enabled.
      //
      Status = Usb4DisableDpTunnel (DpSrcRt, DpSrcAdpNum, DpSinkRt, DpSinkAdpNum, Index, Router);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Usb4DomainDpDisableForRtRemoval: Dp tunnel disable failure, %r\n", Status));
      }
      DpStream->StreamReady = FALSE;
      Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPSTREAM_DISABLED, Index, 0);
      if (Domain->DpStreamCount > 0) {
        Domain->DpStreamCount--;
        DEBUG ((DEBUG_INFO, "DP stream %d is disabled. DP stream count = %d\n", Index, Domain->DpStreamCount));
      } else {
        //
        // Something wrong in DP stream count handling, but doesn't have critical impact to the remaining actions
        // It's not necessary to return failure directly
        // This condition is not expected to happen.
        // Use ASSERT to highlight the abnormal condition.
        //
        DEBUG ((DEBUG_ERROR, "DpStreamCount == 0 before decrement is not expected!\n"));
        ASSERT (FALSE);
      }
    }
  }
Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainDpDisableForRtRemoval exit\n"));
  return Status;
}