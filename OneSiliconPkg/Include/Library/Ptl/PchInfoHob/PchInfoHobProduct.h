/** @file
  This file contains definitions of PCH Info HOB.

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
#ifndef _PCH_INFO_HOB_PRODUCT_H_
#define _PCH_INFO_HOB_PRODUCT_H_

extern EFI_GUID gPchInfoHobGuid;
extern EFI_GUID gPchPcieInfoHobGuid;

#include <Library/PchLimits.h>

#define PCH_INFO_HOB_REVISION  8

#pragma pack (push,1)
/**
  This structure is used to provide the information of PCH controller.

  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add CridSupport, CridOrgRid, and CridNewRid.
  <b>Revision 3</b>:
  - Add Thc0Strap.
  <b>Revision 4</b>
  - Removed GbePciePortNumber
  <b>Revision 5</b>
  - Add ThcSupport, GbeSupport, TsnSupport, PSOnSupport.
  <b>Revision 6</b>
  - Add XhciSupport, HdaLinkSupport, DmicLinkSupport, SspLinkSupport, SndwLinkSupport.
  <b>Revision 7</b>
  - Add SndwMultilaneSupport
  <b>Revision 8</b>
  - Add OsDebugPort
  <b>Revision 9</b>
  - Add IshAvailable
**/
typedef struct {
  /**
    This member specifies the revision of the PCH Info HOB. This field is used
    to indicate backwards compatible changes to the protocol. Platform code that
    consumes this protocol must read the correct revision value to correctly interpret
    the content of the protocol fields.
  **/
  UINT8        Revision;
  UINT8        PcieControllerBifurcation[PCH_MAX_PCIE_CONTROLLERS];
  /**
    THC strap disable/enable status
  **/
  UINT8        Thc0Strap;
  UINT32       PciePortFuses;
  /**
    Bit map for PCIe Root Port Lane setting. If bit is set it means that
    corresponding Root Port has its lane enabled.
    BIT0 - RP0, BIT1 - RP1, ...
    This information needs to be passed through HOB as FIA registers
    are not accessible with POSTBOOT_SAI
  **/
  UINT32       PciePortLaneEnabled;
  /**
    Publish Hpet BDF and IoApic BDF information for VTD.
  **/
  UINT32       HpetBusNum       :  8;
  UINT32       HpetDevNum       :  5;
  UINT32       HpetFuncNum      :  3;
  UINT32       IoApicBusNum     :  8;
  UINT32       IoApicDevNum     :  5;
  UINT32       IoApicFuncNum    :  3;
  /**
    Publish the CRID information.
  **/
  UINT32       CridOrgRid       :  8;
  UINT32       CridNewRid       :  8;
  UINT32       CridSupport      :  1;
  /**
    Publish the HDA information.
  **/
  UINT32       AudioDspFusedOut :  1;
  /**
    Publish PCH features support.
  **/
  UINT32       ThcSupport       :  1;
  UINT32       GbeSupport       :  1;
  UINT32       TsnSupport       :  1;
  UINT32       PSOnSupport      :  1;
  UINT32       HdaLinkSupport   :  1;
  UINT32       XhciSupport      :  1;
  UINT32       IshAvailable     :  1;
  UINT32       LpssDebugUartPg  :  1;
  UINT32       Rsvdbits         :  6;
  UINT8        DmicLinkSupport[PCH_MAX_HDA_DMIC_LINK_NUM];
  UINT8        SspLinkSupport[PCH_MAX_HDA_SSP_LINK_NUM];
  UINT8        SndwLinkSupport[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8        SndwMultilaneSupport[PCH_MAX_HDA_SNDW_MULTILANE_NUM];
  UINT8        OsDebugPort;
} PCH_INFO_HOB;

#pragma pack (pop)

#endif

