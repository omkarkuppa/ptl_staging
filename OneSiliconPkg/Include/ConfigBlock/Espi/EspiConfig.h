/** @file
  Espi policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _ESPI_CONFIG_H_
#define _ESPI_CONFIG_H_

#define PCH_ESPI_CONFIG_REVISION 2
extern EFI_GUID gEspiConfigGuid;

#pragma pack (push,1)

/**
  This structure contains the policies which are related to ESPI.

  <b>Revision 1</b>:
  - Initial revision
  <b>Revision 2</b>:
  - Added LockLinkConfiguration field to config block
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    LPC (eSPI) Memory Range Decode Enable. When TRUE, then the range
    specified in PCLGMR[31:16] is enabled for decoding to LPC (eSPI).
    <b>0: FALSE</b>, 1: TRUE
  **/
  UINT32    LgmrEnable            :  1;
  /**
    eSPI host and device BME settings.
    When TRUE, then the BME bit enabled in eSPI host and device.
    0: FALSE, <b>1: TRUE </b>
  **/
  UINT32    BmeHostDeviceEnabled :  1;
  /**
    Host HOST_C10 (Virtual Wire) to device Enable (VWHC10OE)
    <b>0b: Disable HOST_C10 reporting (HOST_C10 indication from PMC is ignored)</b>
    1b: Enable HOST_C10 reporting to device via eSPI Virtual Wire (upon receiving a HOST_C10 indication from PMC)
  **/
  UINT32    HostC10ReportEnable   :  1;
  /**
    eSPI Link Configuration Lock (SBLCL)
    If set to TRUE then communication through SET_CONFIG/GET_CONFIG
    to eSPI device addresses from range 0x0 - 0x7FF
    <b>1: TRUE</b>, 0: FALSE
  **/
  UINT32    LockLinkConfiguration :  1;
  /**
   Hardware Autonomous Enable (HAE)
   If set to TRUE, then the IP may request a PG whenever it is idle
  **/
  UINT32    EspiPmHAE             :  1;
  /**
    Disable/Enable Non-Fatal Error Reporting (PCNFEE)
    If set to TRUE, then NFE bit of PCERR_SLV0 and PCERR_SLV1 is cleared which disables non-fatal error reporting
  **/
  UINT32    HideNonFatalErrors    :  1;

  /**
    eSPI NMI Enable CS1
    Set this bit to enable eSPI NMI VW events to be processed by the SOC
  **/
  UINT32    NmiEnableCs1          :  1;

  UINT32    RsvdBits              : 25;     ///< Reserved bits
} PCH_ESPI_CONFIG;

#pragma pack (pop)

#endif // _ESPI_CONFIG_H_

