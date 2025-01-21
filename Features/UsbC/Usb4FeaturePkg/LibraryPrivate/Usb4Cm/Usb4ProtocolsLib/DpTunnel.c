/** @file
  DisplayPort tunneling implementation.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4CsLib.h>
#include <Library/Usb4DomainLib.h>
#include <Library/Usb4RouterLib.h>
#include <Library/Tbt3SupportLib.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4RouterDefs.h>
#include <Library/Usb4Capabilities.h>
#include <Library/Usb4DebugLib.h>
#include "ProtocolConfig.h"
#include "DpPath.h"

extern EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log;

/**
  Execute DP Tunneling Router Operation

  @param[in]  Router    - Pointer to Router instance.
  @param[in]  OpCode    - Router operation OpCode.
  @param[in]  DpNum     - DisplayPort number.
  @param[out] OpStatus  - Pointer to Router operation returned status.

  @retval EFI_SUCCESS           - Router operation execution success.
  @retval EFI_TIMEOUT           - Timeout failure of router operation execution.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DpRtOpExec (
  IN PUSB4_ROUTER   Router,
  IN UINT32         OpCode,
  IN UINT32         DpNum,
  OUT UINT32        *OpStatus
  )
{
  EFI_STATUS    Status;
  USB4_RT_OP    RtOp;
  UINT32        Index;

  if ((Router == NULL) || (OpStatus == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DpRtOpExec: Invalid parameter (%p, %p)\n", Router, OpStatus));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  RtOp.MetaData = DpNum;
  RtOp.Op.Value = 0;
  RtOp.Op.Fields.OpCode = OpCode;
  RtOp.Op.Fields.OpValid = 1;

  Status = Usb4CsRtWrite (Router, USB4_CS_25, sizeofdw (USB4_RT_OP), (UINT32 *)&RtOp);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4DpRtOpExec: Write Router 0x%016llX CS25 failure, %r\n", Router->TopologyId, Status));
    goto Exit;
  }

  for (Index = 0; Index < CM_STATUS_WAITS_MAX; Index++) {
    Status = Usb4CsRtRead (Router, USB4_CS_26, 1, (UINT32 *)&RtOp.Op);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4DpRtOpExec: Read Router 0x%016llX CS25 failure, %r\n", Router->TopologyId, Status));
      goto Exit;
    }

    if (RtOp.Op.Fields.OpValid == 0) {
      if (RtOp.Op.Fields.OpNotSupported == 0) {
        *OpStatus = RtOp.Op.Fields.Status;
        DEBUG ((DEBUG_INFO, "Router CS26 = 0x%08x\n", RtOp.Op));
        DEBUG ((DEBUG_INFO, "DP RT Op 0x%0x DpNum %d - Status = 0x%0x, retry = %d\n", OpCode, DpNum, *OpStatus, Index));
      } else {
        DEBUG ((DEBUG_INFO, "DP RT Op 0x%0x DpNum %d - Router Operation is not supported!\n", OpCode, DpNum));
        Status = EFI_UNSUPPORTED;
      }
      break;
    }

    gBS->Stall (CM_STATUS_POLL_US);
  }

  if (Index == CM_STATUS_WAITS_MAX) {
    DEBUG ((DEBUG_ERROR, "Router 0x%016llX DpNum %d - DP RT Op timeout!\n", Router->TopologyId, DpNum));
    Status = EFI_TIMEOUT;
    goto Exit;
  }

Exit:
  return Status;
}

/**
  Set Router TMU to enable Time Sync for DP.

  @param[in] DpSrcRt  - Pointer to the Router instance connecting DP Source.
  @param[in] DpSinkRt - Pointer to the Router instance connecting DP Sink.

  @retval EFI_SUCCESS           - Set Router TMU for DP success.
  @retval EFI_UNSUPPORTED       - Set Router TMU for DP failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
EnableTimeSyncForDp (
  IN PUSB4_ROUTER    DpSrcRt,
  IN PUSB4_ROUTER    DpSinkRt
  )
{
  EFI_STATUS              Status;
  PUSB4_ROUTER            CurRt;
  PUSB4_ADAPTER           LaneDs;
  UINT8                   Depth;
  UINT8                   DepthMax;
  UINT8                   AdpNum;
  USB4_RT_CAP_TMU_CS_0    TmuCapCs0;
  USB4_RT_CAP_TMU_CS_3    TmuCapCs3;

  DEBUG ((DEBUG_INFO, "EnableTimeSyncForDp entry\n"));

  Status = EFI_SUCCESS;

  if ((DpSrcRt == NULL) || (DpSinkRt == NULL)) {
    DEBUG ((DEBUG_ERROR, "EnableTimeSyncForDp: Invalid parameter (%p, %p)\n", DpSrcRt, DpSinkRt));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Depth in for loop traverses from Router connecting DP source to Router connecting DP sink
  // Depth should be in valid range for TopologyId.Adapter array access
  //
  DepthMax = MIN (DpSinkRt->Depth, mUsb4TopologyDepth);
  DepthMax = MIN (DepthMax, TOPOLOGY_ID_ADAPTER_SIZE - 1);

  //
  // Set TMU sync from DP source router to DP sink router.
  //
  for (CurRt = DpSrcRt, Depth = DpSrcRt->Depth; Depth <= DepthMax; Depth++) {
    //
    // Skip TMU setting for Host Router according to reference codes provided by TBT team.
    //
    if (CurRt->TopologyId.Value != 0) {
      DEBUG ((DEBUG_INFO, "Configure Rt 0x%016llX TMU\n", CurRt->TopologyId));

      //
      // Set TimeDisruption to 1 before change TS Packet Interval value
      //
      if (CurRt != DpSinkRt) {
        if (CurRt->TmuCapReg == 0) {
          DEBUG ((DEBUG_ERROR, "TMU capability is not present in Rt 0x%016llX\n", CurRt->TopologyId));
          Status = EFI_UNSUPPORTED;
          goto Exit;
        }
        Status = Usb4CsRtRead (CurRt, CurRt->TmuCapReg + USB4_CS_0, 1, (UINT32 *) &TmuCapCs0);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Rt 0x%016llX read RT TMU at 0x%x failure, %r\n", CurRt->TopologyId, CurRt->TmuCapReg, Status));
          goto Exit;
        }
        DEBUG ((DEBUG_INFO, "Set TimDisruption to 1. Original TimeDisruption = %d\n", TmuCapCs0.Fields.TimeDisruption));
        TmuCapCs0.Fields.TimeDisruption = 1;
        Status = Usb4CsRtWrite (CurRt, CurRt->TmuCapReg + USB4_CS_0, 1, (UINT32 *) &TmuCapCs0);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Rt 0x%016llX write RT TMU at 0x%x failure, %r\n", CurRt->TopologyId, CurRt->TmuCapReg, Status));
          goto Exit;
        }
      }

      //
      // Set TS Packet Interval value
      //
      Status = Usb4CsRtRead (CurRt, CurRt->TmuCapReg + USB4_CS_3, 1, (UINT32 *) &TmuCapCs3);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX read RT TMU at 0x%x failure, %r\n", CurRt->TopologyId, CurRt->TmuCapReg + USB4_CS_3, Status));
        goto Exit;
      }
      DEBUG ((DEBUG_INFO, "Set TS Packet Interval to %d us. Original TS Packet Interval = %d us\n", TSPKT_INTERVAL_HIFI, TmuCapCs3.Fields.TsPktInterval));
      TmuCapCs3.Fields.TsPktInterval = TSPKT_INTERVAL_HIFI;
      Status = Usb4CsRtWrite (CurRt, CurRt->TmuCapReg + USB4_CS_3, 1, (UINT32 *) &TmuCapCs3);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Router 0x%016llX write RT TMU at 0x%x failure, %r\n", CurRt->TopologyId, CurRt->TmuCapReg + USB4_CS_3, Status));
        goto Exit;
      }

      //
      // Read back the value for checking - For initial development phase only
      //
      Status = Usb4CsRtRead (CurRt, CurRt->TmuCapReg + USB4_CS_3, 1, (UINT32 *)&TmuCapCs3);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX read RT TMU at 0x%x failure, %r\n", CurRt->TopologyId, CurRt->TmuCapReg + USB4_CS_3, Status));
        goto Exit;
      }
      DEBUG ((CM_VERBOSE, "Updated TS Packet Interval = %d us\n", TmuCapCs3.Fields.TsPktInterval));

      //
      // Set TimeDisruption to 0 after change TS Packet Interval value
      //
      if (CurRt != DpSinkRt) {
        Status = Usb4CsRtRead (CurRt, CurRt->TmuCapReg + USB4_CS_0, 1, (UINT32 *) &TmuCapCs0);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Rt 0x%016llX read RT TMU at 0x%x failure, %r\n", CurRt->TopologyId, CurRt->TmuCapReg, Status));
          goto Exit;
        }
        DEBUG ((DEBUG_INFO, "Set TimDisruption to 0. Original TimeDisruption = %d\n", TmuCapCs0.Fields.TimeDisruption));
        TmuCapCs0.Fields.TimeDisruption = 0;
        Status = Usb4CsRtWrite (CurRt, CurRt->TmuCapReg + USB4_CS_0, 1, (UINT32 *) &TmuCapCs0);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Rt 0x%016llX write RT TMU at 0x%x failure, %r\n", CurRt->TopologyId, CurRt->TmuCapReg, Status));
          goto Exit;
        }
      }

      if (CurRt == DpSinkRt) {
        break;
      }
    }

    AdpNum = DpSinkRt->TopologyId.Adapter[Depth];
    if ((AdpNum == 0) || (AdpNum > CurRt->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
      DEBUG ((DEBUG_ERROR, "Invalid adp %d at depth %d in topology id 0x%016llX\n", AdpNum, Depth, DpSinkRt->TopologyId));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }

    LaneDs = &CurRt->Adapter[AdpNum];
    if (LaneDs->Type != USB4_ADP_TYPE_LANE) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d is not Lane adapter\n", CurRt->TopologyId, AdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    CurRt = LaneDs->AdpDep.Lane.DsRouter;
    if (CurRt == NULL) {
      DEBUG ((DEBUG_ERROR, "Null downstream Router at depth %d\n", Depth));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
  }
Exit:
  DEBUG ((DEBUG_INFO, "EnableTimeSyncForDp exit\n"));
  return Status;
}

/**
  Exchange DP local capabilities of DP-IN and DP-OUT

  @param[in] DpSrcRt     - Pointer to DP source Router.
  @param[in] DpIn       - DP source adapter number.
  @param[in] DpSinkRt    - Pointer to DP sink Router.
  @param[in] DpOut     - DP sink adapter number.

  @retval EFI_SUCCESS   - DP local capabilities exchanges success.
  @retval other         - DP local capabilities exchanges failure.
**/
EFI_STATUS
ExchangeDpCapabilities (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpIn,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpOut
  )
{
  EFI_STATUS                  Status;
  PUSB4_ADAPTER               DpInAdp;
  PUSB4_ADAPTER               DpOutAdp;
  USB4_ADP_CAP_DP_CS_4_5      DpLocalCap;
  USB4_ADP_CAP_DP_OUT_CS_6    DpCapCs6;
  UINT32                      Retry;
  UINT8                       NvmVer;

  DEBUG ((DEBUG_INFO, "ExchangeDpCapabilities entry\n"));

  if ((DpSrcRt == NULL) || (DpSinkRt == NULL)) {
    DEBUG ((DEBUG_ERROR, "ExchangeDpCapabilities: Invalid parameter (%p, %p)\n", DpSrcRt, DpSinkRt));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((DpIn == 0) || (DpIn > DpSrcRt->MaxAdapter) || (DpIn >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "ExchangeDpCapabilities: Invalid DpIn %d\n", DpIn));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((DpOut == 0) || (DpOut > DpSinkRt->MaxAdapter) || (DpOut >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "ExchangeDpCapabilities: Invalid DpOut %d\n", DpOut));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "DP-IN  : (0x%016llX, %d)\n", DpSrcRt->TopologyId, DpIn));
  DEBUG ((DEBUG_INFO, "DP-OUT : (0x%016llX, %d)\n", DpSinkRt->TopologyId, DpOut));

  DpInAdp = &DpSrcRt->Adapter[DpIn];
  DpOutAdp = &DpSinkRt->Adapter[DpOut];

  //
  // <TBT3>
  // Set DP IP register for TitanRidge
  // Refer to CM TBT3 Device Support 5.2.1.2
  //
  if (DpSinkRt->RouterType == USB4_ROUTER_TITAN_RIDGE) {
    Status = Tbt3TrDpCmhsInit (DpSinkRt);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Fail to initialize DP IP register before CMHS, %r\n", Status));
      goto Exit;
    }
  }

  //
  // <TBT3>
  // AlpineRidge doesn't support CMHS
  // Skip CMHS step for AlpineRidge
  //
  if (DpSinkRt->RouterType != USB4_ROUTER_ALPINE_RIDGE) {
    DEBUG ((CM_DEBUG, "Write 1 to CMHS bit\n"));
    //
    // Initiate the handshake by writing 1 to CMHS bit
    //
    Status = Usb4CsAdpRead (DpSinkRt, DpOut, DpOutAdp->AdpCapReg + DP_STATUS_CTRL, 1, (UINT32 *) &DpCapCs6);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d read DP Cap CS6 failure, %r\n", DpSinkRt->TopologyId, DpOut, Status));
      goto Exit;
    }
    DpCapCs6.Fields.DpInUsb4Flag = 1;
    DpCapCs6.Fields.CmHandshake  = 1;
    Status = Usb4CsAdpWrite (DpSinkRt, DpOut, DpOutAdp->AdpCapReg + DP_STATUS_CTRL, 1, (UINT32 *)&DpCapCs6);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d write DP Cap CS6 failure, %r\n", DpSinkRt->TopologyId, DpOut, Status));
      goto Exit;
    }

    //
    // Polling the CMHS bit until it is reset to 0 by DP OUT adapter
    //
    for (Retry = 0; Retry < CM_STATUS_WAITS_MAX; Retry++) {
      Status = Usb4CsAdpRead (DpSinkRt, DpOut, DpOutAdp->AdpCapReg + DP_STATUS_CTRL, 1, (UINT32 *)&DpCapCs6);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d read DP cap cs6 failure, %r\n", DpSinkRt->TopologyId, DpOut, Status));
        goto Exit;
      }
      if (DpCapCs6.Fields.CmHandshake == 0) {
        DEBUG ((CM_DEBUG, "DP CMHS cleared, Lane cnt = %d, Link Rate = %d, Retry = %d\n",
                DpCapCs6.Fields.LaneCnt,
                DpCapCs6.Fields.LinkRate,
                Retry));
        break;
      }
      Status = gBS->Stall (CM_STATUS_POLL_US);
    }

    if (Retry == CM_STATUS_WAITS_MAX) {
      Status = EFI_TIMEOUT;
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d wait CMHS bit clear timeout! CS6=0x%08X\n", DpSinkRt->TopologyId, DpOut, DpCapCs6));
      goto Exit;
    }
  }

  //
  // Write DP-IN's Local Capabilities to DP-OUT's Remote Capabilities
  //
  Status = Usb4CsAdpRead (DpSrcRt, DpIn, DpInAdp->AdpCapReg + DP_LOCAL_CAP, sizeofdw (DpLocalCap), (UINT32 *) &DpLocalCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d read DP Cap CS4 failure, %r\n", DpSrcRt->TopologyId, DpIn, Status));
    goto Exit;
  }
  DEBUG ((CM_DEBUG, "DP-IN's CS4 = 0x%08X after CMHS cleared\n", DpLocalCap));
  Status = Usb4CsAdpWrite (DpSinkRt, DpOut, DpOutAdp->AdpCapReg + DP_REMOTE_CAP, sizeofdw (DpLocalCap), (UINT32 *) &DpLocalCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d write DP Cap CS5 failure, %r\n", DpSinkRt->TopologyId, DpOut, Status));
    goto Exit;
  }

  //
  // Write DP-OUT's Local Capabilities to DP-IN's Remote Capabilities
  //
  Status = Usb4CsAdpRead (DpSinkRt, DpOut, DpOutAdp->AdpCapReg + DP_LOCAL_CAP, sizeofdw (DpLocalCap), (UINT32 *)&DpLocalCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d read DP Cap CS4 failure, %r\n", DpSinkRt->TopologyId, DpOut, Status));
    goto Exit;
  }
  DEBUG ((CM_DEBUG, "DP-OUT's CS4 = 0x%08X after CMHS cleared\n", DpLocalCap));

  //
  // <TBT3>
  // Set LttprNotSupport bit to 1 if TitanRidge NVM version is lower than 62
  // (Refer to CM TBT3 Device support 5.2.1.2.2)
  //
  if (DpSinkRt->RouterType == USB4_ROUTER_TITAN_RIDGE) {
    Status = Tbt3TrReadNvmVersion (DpSinkRt, &NvmVer);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Fail to read TitanRidge NVM version, %r\n", Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "TitanRidge NVM version = %d\n", NvmVer));
    if (NvmVer < TR_LTTPR_SUPPORT_REV_MIN) {
      DEBUG ((DEBUG_INFO, "Original LttprNotSupport bit = %d\n", DpLocalCap.Fields.LttprNotSupport));
      DpLocalCap.Fields.LttprNotSupport = 1;
    }
  }

  Status = Usb4CsAdpWrite (DpSrcRt, DpIn, DpInAdp->AdpCapReg + DP_REMOTE_CAP, sizeofdw (DpLocalCap), (UINT32 *)&DpLocalCap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d write DP Cap CS5 failure, %r\n", DpSrcRt->TopologyId, DpIn, Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "ExchangeDpCapabilities exit\n"));
  return Status;
}

/**
  Configure DP NFC buffers

  @param[in] DpSrcRt     - Pointer to DP source Router.
  @param[in] DpSinkRt    - Pointer to DP sink Router.

  @retval EFI_SUCCESS   - Configure DP NFC buffers success.
  @retval other         - Configure DP NFC buffers failure.
**/
EFI_STATUS
DpConfigNfcBuffers (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpIn,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpOut
  )
{
  EFI_STATUS       Status;
  PUSB4_ROUTER     CurRt;
  PUSB4_ADAPTER    DsLane;
  UINT8            AdpNum;
  UINT8            Depth;
  UINT8            DepthMax;
  USB4_ADP_CS_4    AdpCs4;

  DEBUG ((DEBUG_INFO, "DpConfigNfcBuffers entry\n"));
  if ((DpSrcRt == NULL) || (DpSinkRt == NULL)) {
    DEBUG ((DEBUG_ERROR, "DpConfigNfcBuffers: Invalid parameter (%p, %p)\n", DpSrcRt, DpSinkRt));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Configure NFC buffers of DP-IN adapter
  //
  Status = Usb4CsAdpRead (DpSrcRt, DpIn, USB4_CS_4, 1, (UINT32 *) &AdpCs4);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RT 0x%016llX adp %d read Adapter CS4 failure, %r\n", DpSrcRt->TopologyId, DpIn, Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Rt 0x%016llX DP-IN %d - NFC buffers = %d\n", DpSrcRt->TopologyId, DpIn, AdpCs4.Fields.NfcBuffers));

  //
  // Depth in for loop traverses from Router connecting DP source to the parent Router of Router connecting DP sink
  // Depth should be in valid range for TopologyId.Adapter array access
  //
  DepthMax = MIN (DpSinkRt->Depth - 1, mUsb4TopologyDepth - 1);
  DepthMax = MIN (DepthMax, TOPOLOGY_ID_ADAPTER_SIZE - 1);
  for (CurRt = DpSrcRt, Depth = DpSrcRt->Depth; Depth <= DepthMax; Depth++) {
    AdpNum = DpSinkRt->TopologyId.Adapter[Depth];
    if ((AdpNum == 0) || (AdpNum > CurRt->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
      DEBUG ((DEBUG_ERROR, "Invalid adp %d at depth %d in topology id 0x%016llX\n", AdpNum, Depth, DpSinkRt->TopologyId));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    DsLane = &CurRt->Adapter[AdpNum];
    if (DsLane->Type != USB4_ADP_TYPE_LANE) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d is not Lane adapter\n", CurRt->TopologyId, AdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    CurRt = DsLane->AdpDep.Lane.DsRouter;
    if (CurRt == NULL) {
      DEBUG ((DEBUG_ERROR, "Null downstream Router at depth %d\n", Depth));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }

    //
    // Configure NFC buffers of upstream Lane adapter on DP traverse list
    //
    Status = Usb4CsAdpRead (CurRt, CurRt->LaneUpAdpNum, USB4_CS_4, 1, (UINT32 *) &AdpCs4);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "RT 0x%016llX adp %d read Adapter CS4 failure, %r\n", CurRt->TopologyId, CurRt->LaneUpAdpNum, Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX Lane Up %d - Original NFC buffers = %d\n", CurRt->TopologyId, CurRt->LaneUpAdpNum, AdpCs4.Fields.NfcBuffers));
    AdpCs4.Fields.NfcBuffers = (CurRt == DpSinkRt) ? DP_OUT_NFC_BUFFERS : DP_MID_NFC_BUFFERS;
    Status = Usb4CsAdpWrite (CurRt, CurRt->LaneUpAdpNum, USB4_CS_4, 1, (UINT32 *) &AdpCs4);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "RT 0x%016llX adp %d write Adapter CS4 failure, %r\n", CurRt->TopologyId, CurRt->LaneUpAdpNum, Status));
      goto Exit;
    }
    //
    // Read back the NFC buffers for checking - For initial development phase only
    //
    Status = Usb4CsAdpRead (CurRt, CurRt->LaneUpAdpNum, USB4_CS_4, 1, (UINT32 *) &AdpCs4);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "RT 0x%016llX adp %d read Adapter CS4 failure, %r\n", CurRt->TopologyId, CurRt->LaneUpAdpNum, Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX Lane Up %d - Updated NFC buffers = %d\n", CurRt->TopologyId, CurRt->LaneUpAdpNum, AdpCs4.Fields.NfcBuffers));
  }
Exit:
  DEBUG ((DEBUG_INFO, "DpConfigNfcBuffers exit\n"));
  return Status;
}

/**
  Enable DP Tunnel between DP Source and DP Sink.

  @param[in] DpSrcRt      - Pointer to router instance which contains DP-IN adapter connecting DP source.
  @param[in] DpSrcAdpNum  - Adapter number of DP-IN adapter connecting DP source.
  @param[in] DpSinkRt     - Pointer to router instance which contains DP-OUT adapter connecting DP sink.
  @param[in] DpSinkAdpNum - Adapter number of DP-OUT adapter connecting DP sink.
  @param[in] StreamId     - DP stream ID for identifying DP path.

  @retval EFI_SUCCESS           - Enable DP Tunnel success.
  @retval EFI_UNSUPPORTED       - Fail to enable DP Tunnel.
  @retval EFI_OUT_OF_RESOURCES  - Fail to get DP resources to enable DP tunnel.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4EnableDpTunnel (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum,
  IN UINT32          StreamId
  )
{
  EFI_STATUS    Status;
  UINT32        OpStatus;

  DEBUG ((DEBUG_INFO, "Usb4EnableDpTunnel entry - StreamId = %d\n", StreamId));

  if ((DpSrcRt == NULL) || (DpSinkRt == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4EnableDpTunnel: Invalid parameter (%p, %p)\n", DpSrcRt, DpSinkRt));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Usb4EnableDpTunnel: Rt 0x%016llX <-> 0x%016llX\n", DpSrcRt->TopologyId, DpSinkRt->TopologyId));

  //
  // Allocate DP resources in DP-IN adapter of DP source.
  //
  DEBUG ((DEBUG_INFO, "Usb4DpRtOpExec alloc DP resources\n"));
  Status = Usb4DpRtOpExec (DpSrcRt, RT_OP_ALLOC_DP_RESOURCES, DpSrcAdpNum, &OpStatus);
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPRES_ALLOC, (UINT32) Status, OpStatus);
  if ((EFI_ERROR (Status)) || RT_OP_STATUS_ERROR (OpStatus)) {
    DEBUG ((DEBUG_ERROR, "Router 0x%016llX allocates DP IN resources failure, %r\n", DpSrcRt->TopologyId, Status));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  //
  // Set TMU to enable Time Sync for DP.
  //
  DEBUG ((DEBUG_INFO, "Enable Time Sync\n"));
  Status = EnableTimeSyncForDp (DpSrcRt, DpSinkRt);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4EnableDpTunnel: Enable DP Time sync failure, %r\n", Status));
    goto Exit;
  }

  //
  // Exchange DP capabilities of DP-IN adapter and DP-OUT adapter.
  //
  Status = ExchangeDpCapabilities (DpSrcRt, DpSrcAdpNum, DpSinkRt, DpSinkAdpNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4EnableDpTunnel: Exchange DP capabilities failure!, %r\n", Status));
    goto Exit;
  }

  //
  // Configure NFC buffers
  //
  Status = DpConfigNfcBuffers (DpSrcRt, DpSrcAdpNum, DpSinkRt, DpSinkAdpNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4EnableDpTunnel: Configure NFC buffers failure, %r\n", Status));
    goto Exit;
  }

  //
  // Configure DP path between DP source and DP sink
  //
  Status = DpPathSetup (DpSrcRt, DpSrcAdpNum, DpSinkRt, DpSinkAdpNum, StreamId);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4EnableDpTunnel: Configure DP path failure, %r\n", Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4EnableDpTunnel exit, %r\n", Status));
  return Status;
}

/**
  Disable DP Tunnel between DP Source and DP Sink for DP adapter removal or router removal.
  - Disable DP path of DP-IN adapter and DP-OUT adapter.
  - Tear down DP path between DP-IN and DP-OUT (until upper level router of the removed router if any).
  - De-allocate DP resources

  @param[in] DpSrcRt      - Pointer to router instance which contains DP-IN adapter connecting DP source.
  @param[in] DpSrcAdpNum  - Adapter number of DP-IN adapter connecting DP source.
  @param[in] DpSinkRt     - Pointer to router instance which contains DP-OUT adapter connecting DP sink.
  @param[in] DpSinkAdpNum - Adapter number of DP-OUT adapter connecting DP sink.
  @param[in] StreamId     - DP stream ID for identifying DP path.
  @param[in] RemovedRt    - Removed router on DP path.
                            This is specified if DP sink router is under the removed router tree.
                            USB4 config space access is not available from the removed router.

  @retval EFI_SUCCESS           - Disable DP Tunnel success.
  @retval EFI_UNSUPPORTED       - Fail to disable DP Tunnel.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DisableDpTunnel (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum,
  IN UINT32          StreamId,
  IN PUSB4_ROUTER    RemovedRt
  )
{
  EFI_STATUS    Status;
  UINT32        OpStatus;

  DEBUG ((DEBUG_INFO, "Usb4DisableDpTunnel entry - StreamId = %d\n", StreamId));

  if ((DpSrcRt == NULL) || (DpSinkRt == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DisableDpTunnel: Null router (%p, %p)\n", DpSrcRt, DpSinkRt));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (RemovedRt != NULL) {
    //
    // The DP source router is in the removed router tree.
    // USB4 config space access is not available.
    //
    if (RemovedRt->Depth <= DpSrcRt->Depth) {
      DEBUG ((DEBUG_ERROR, "Usb4DisableDpTunnel: DP source router has been disconnected\n"));
      Status = EFI_SUCCESS;
      goto Exit;
    }

    if (Usb4RtInRtTree (DpSinkRt, RemovedRt) == FALSE) {
      DEBUG ((DEBUG_ERROR, "DP sink Rt 0x%016llX is not under removed Rt 0x%016llX\n", DpSinkRt->TopologyId, RemovedRt->TopologyId));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
  }
  //
  // Disable DP path of DP-IN adapter
  //
  DEBUG ((DEBUG_INFO, "Disable DP path of DP-IN - Rt 0x%016llX Adp %d\n", DpSrcRt->TopologyId, DpSrcAdpNum));
  Status = DpAdpEnablePath (DpSrcRt, DpSrcAdpNum, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Disable DP path failure Rt 0x%016llX DP-IN %d, %r\n", DpSrcRt->TopologyId, DpSrcAdpNum, Status));
  }

  //
  // Disable DP path of DP-OUT adapter if DP disable action is not triggered by router removal.
  //
  if (RemovedRt == NULL) {
    DEBUG ((DEBUG_INFO, "Disable DP path of DP-OUT - Rt 0x%016llX Adp %d\n", DpSinkRt->TopologyId, DpSinkAdpNum));
    Status = DpAdpEnablePath (DpSinkRt, DpSinkAdpNum, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Disable DP path failure Rt 0x%016llX DP-IN %d, %r\n", DpSinkRt->TopologyId, DpSinkAdpNum, Status));
    }
  }

  //
  // Tear down DP path from DP Source to DP Sink.
  //
  Status = DpPathTearDown (DpSrcRt, DpSrcAdpNum, DpSinkRt, DpSinkAdpNum, StreamId, RemovedRt);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Tear down DP path failure, %r\n", Status));
  }

  //
  // De-allocate DP Resources
  //
  Status = Usb4DpRtOpExec (DpSrcRt, RT_OP_DEALLOC_DP_RESOURCES, DpSrcAdpNum, &OpStatus);
  DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d De-alloc DP Res, status = 0x%0x\n", DpSrcRt->TopologyId, DpSrcAdpNum, OpStatus));
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DPRES_DEALLOC, (UINT32) Status, OpStatus);

Exit:
  DEBUG ((DEBUG_INFO, "Usb4DisableDpTunnel exit\n"));
  return Status;
}

/**
  Remove DP tunnel if the downstream router is in DP tunnel path.

  @param[in] Router    - Pointer to Router instance.
  @param[in] AdpNum    - Adapter number.

  @retval EFI_SUCCESS           - Remove PCIe Tunneling successfully.
  @retval EFI_UNSUPPORTED       - Remove PCIe Tunneling failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4RtRemoveDsDp (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS       Status;
  PUSB4_ROUTER     DsRouter;
  PUSB4_ADAPTER    LaneAdp;

  DEBUG ((DEBUG_INFO, "Usb4RtRemoveDsDp entry - Router %p, AdpNum %d\n", Router, AdpNum));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsDp: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsDp: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsDp: Adp %d is not Lane Adapter, Adapter type = %d\n", AdpNum, LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DsRouter = LaneAdp->AdpDep.Lane.DsRouter;
  if (DsRouter == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsDp: Rt 0x%016llX Adp %d null downstream router\n", Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Remove DP from domain for router removal.
  //
  Status = Usb4DomainDpDisableForRtRemoval (Router->DomainContext, DsRouter);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsDp: Remove DP from domain failure, %r\n", Status));
    goto Exit;
  }
Exit:
  return Status;
}