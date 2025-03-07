/** @file
  This file contains code related to initializing and configuring the DDRIO.

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

#include "MrcCommon.h"
#include "MrcDdrIoApi.h"

/**
  This function programs the current post code to PHYPMSTATUS1 register field.

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  PostCode - MRC post code to be programmed

**/
VOID
MrcProgramPostCode (
  IN MrcParameters* const MrcData,
  IN INT64          PostCode
  )
{
}

/**
  This function sets DDRIO IP Version, Derivative, Segment, Stepping based on DDRPHY_MISC_SAUG_CR_IPVERSION_REG.

  @param[in]  MrcData     - Include all MRC global data.

  @retval Nothing.
**/
MrcStatus
MrcSetupDdrIoIpInfo (
  OUT MrcParameters *const MrcData
  )
{
  MrcInput             *Inputs;
  MrcDebug             *Debug;
  MrcStatus            Status;
  MrcDdrIoIpVersion    *IpVersion;
  DDRPHY_MISC_SAUG_CR_IPVERSION_STRUCT   DdrMiscIpVersion;
  const char           *StepStr;

  Inputs          = &MrcData->Inputs;
  Debug           = &MrcData->Outputs.Debug;
  IpVersion       = &Inputs->DdrIoIpVersion;
  Status          = mrcSuccess;
  StepStr         = NULL;

  DdrMiscIpVersion.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_IPVERSION_REG);
  IpVersion->Bits.Version    = (UINT8)DdrMiscIpVersion.Bits.Version;
  IpVersion->Bits.Derivative = (UINT8)DdrMiscIpVersion.Bits.Derivative;
  IpVersion->Bits.Segment    = (UINT8)DdrMiscIpVersion.Bits.Segment;
  IpVersion->Bits.Stepping   = (UINT8)DdrMiscIpVersion.Bits.Subversion;

  if (IpVersion->Bits.Stepping > ipStepLast) {
    IpVersion->Bits.Stepping = ipStepLast;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s DDRIO IP: %s = %d\n", gWarnString, gMrcIpSteppingStr, ipStepLast);
  }

  if (IpVersion->Bits.Version != ipVerDdrIoLnlPtl) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s DDRIO IP: %s\n", gWarnString, gMrcIpVersionStr);
      Status  = mrcFail;
  }

  if (Inputs->ExtInputs.Ptr->SimicsFlag && IpVersion->Bits.Derivative != ipDerivativeWcl) {
    StepStr = "A0";
    Inputs->IsDdrIoMbA0 = TRUE;
    IpVersion->Bits.Derivative = ipDerivativePtl;
  } else {
    switch (IpVersion->Bits.Segment) {
      case IpSegmentMobile:
        Inputs->IsDdrIoUlxUlt = TRUE;
        switch (IpVersion->Bits.Stepping) {
          case ipStepB0:
            StepStr = "B0";
            Inputs->IsDdrIoB0 = TRUE;
            break;

          default:
          case ipStepA0:
            StepStr = "A0";
            Inputs->IsDdrIoMbA0 = TRUE;
            break;
        }
        break;

      default:
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s DDRIO IP: %s\n", gWarnString, gMrcIpSegmentStr);
        Status  = mrcFail;
        break;
    }
  }

  if (IpVersion->Bits.Derivative == ipDerivativeWcl) {
    Inputs->IsDdrphyx64 = TRUE;
  }

  if (Status == mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s\n", Inputs->IsDdrphyx64 ? "PantherLake" : "PantherLake" );
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "\nPHY IP Version: %d.%d.%d %s\n",
      IpVersion->Bits.Derivative,
      IpVersion->Bits.Segment,
      IpVersion->Bits.Version,
      (StepStr == NULL) ? "Unknown" : StepStr
    );
  }

  return Status;
}