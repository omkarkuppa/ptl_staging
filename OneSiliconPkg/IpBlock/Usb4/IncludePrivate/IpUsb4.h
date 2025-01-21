/** @file
  IP API for USB4

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

#ifndef __IP_USB4_H__
#define __IP_USB4_H__

#include <IpCsi.h>

#ifndef MAX_HOST_ITBT_DMA_NUMBER
#define MAX_HOST_ITBT_DMA_NUMBER    2
#endif  //MAX_HOST_ITBT_DMA_NUMBER

#define USB4_CM_MODE_FW_CM           0x00
#define USB4_CM_MODE_SW_CM           0x01

#define VERSION_HW_USB4              0x00020001
#define VERSION_HW_CUSTOM_USB4       0x00000001
#define VERSION_FW_USB4              0x00000002
#define VERSION_IPFWINTF_USB4        0x05230102
#define VERSION_IPFWINTFEXT_USB4     0x00000002
#define VERSION_UNKNOWN_USB4         0

typedef struct _IP_USB4_INST IP_USB4_INST;

#ifndef _TCSS_VER
#define _TCSS_VER
typedef enum {
  TcssOld   = 0,
  TcssNew   = 1
} TCSS_VER;
#endif // _TCSS_VER

typedef enum {
  IP_NULL,                                      ///< All policy flags turned off.
  IP_NORMAL   = (1 << 0),                       ///< Execute USB4 function on cold reset.
  IP_S3       = (1 << 1),                       ///< Execute USB4 function on S3 exit.
  IP_S4       = (1 << 2),                       ///< Execute USB4 function on S4 exit.
  IP_ALL      = 0xFFFFFFFF                      ///< Execute USB4 function always (MAX_UINTN for 32 bits).
} IP_USB4_BOOT_MODE;

///
/// The USB4_DEVEN_CONFIG describes Device Enable settings.
///
typedef union {
  UINT32      DevEn;            ///< Maps to bits in TCSS DEVEN register
  struct {
    UINT32    TcssPcieRpEn : 8;
    UINT32    TcssXhciEn   : 1;
    UINT32    Rsvd         : 1;
    UINT32    Usb4HrEn     : 4;
    UINT32 : 18;
  } DevEnBits;
} USB4_DEVEN_CONFIG;

typedef struct {
  USB4_DEVEN_CONFIG       DevEn;          // Device Enables for USB4
  UINT32                  ITbtToPcieReg;  // Handshake register value driven to DMA controller PCIE vendor specific configuration register from FW (LC/CM to host)
  UINT32                  PcieToITbtReg;  // Handshake register value driven from DMA controller PCIE vendor specific configuration register to FW (HOST to LC/CM)
  /**
    Timeout value for forcing power iTBT controller on every boot/reboot/Sx exit as a precondition for execution of following
    mailbox communication.
    After applying Force Power Thunderbolt BIOS shall poll for iTBT readiness for mailbox communication
    If TBT cable is disconnected, iTBT microcontrollers are in lower power state.
    To ensure successful mailbox execution, independently on presence of TBT cable, TBT BIOS shall bring iTBT
    microcontrollers up by applying Force Power. iTBT microcontrollers will wake up either due to TBT cable presence or
    Force Power event.
    <b>(Test)</b>
    <b> default is 500 ms </b>
  **/
  UINT16                  ITbtForcePowerOnTimeoutInMs;
  /**
    Timeout value while sending connect topology mailbox command in order to bring all connected TBT devices are available
    on PCIe before BIOS will enumerate them in BDS
    <b>(Test)</b>
    <b> default is 5000 ms </b>
  **/
  UINT16                  ITbtConnectTopologyTimeoutInMs;

  UINT8                   ITbtSecurityLevel;               ///< iTbt Security Level <b>Deprecated</b>
  UINT8                   ITbtPcieTunnelingForUsb4;        ///< Disable/Enable PCIe tunneling for USB4. <b>default is enable</b>
  UINT8                   Usb4CmMode;                      ///< USB4 CM mode
  UINT16                  ITbtDmaLtr[MAX_HOST_ITBT_DMA_NUMBER];     ///< iTbt Host controller DMA LTR value
} IP_USB4_CONFIG;

typedef struct _IP_USB4_DMA IP_USB4_DMA;

struct _IP_USB4_DMA {
  IP_WR_REG_CNTXT   PrimCfgAccess;
  IP_USB4_DMA       *Next;
};

struct _IP_USB4_INST {
  IP_CSI_INST_PREFIX    Prefix;

  // IP Wrapper Register Contexts
  IP_USB4_DMA           *DmaList;

  // IP Wrapper Print Contexts
  IP_WR_PRINT_CNTXT     PrintError;
  IP_WR_PRINT_CNTXT     PrintWarning;
  IP_WR_PRINT_CNTXT     PrintLevel1;

// IP Wrapper Version Specific Contexts
  IP_WR_VSC_CNTXT       IpWrVscCntxt;

  // IP Wrapper Timing Contexts
  IP_WR_TIME_CNTXT      TimeCntxt;

  // USB4 IP Public Variables
  IP_USB4_CONFIG        Usb4Config;

  // Current Boot Mode
  IP_USB4_BOOT_MODE     BootMode;

  // Random Number (For UUID)
  UINT64                RandomNum;

  // to handle differences between new and old flow
  TCSS_VER              TcssVersion;
};

/**
  Instance structure initialization for this version of IP
  For current version of USB4 IP there is no requirements for this function.
  All data in Instance Structure is initialized by the caller.
  The function just returns success status.

  @param[in] pInst  The instance structure pointer

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpUsb4InstInit (
  IP_USB4_INST      *pInst
  );

/**
  USB4 IP Initialization procedure.
  The function shall be the first API function called after IpIomInstInit.
  This is a good place to fix bad register defaults or work around bugs.

  @param[in]  pInst The instance structure pointer

  @retval         IpCsiStsSuccess on success
  @retval         !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpUsb4IpInit (
  IP_USB4_INST     *pInst
  );

/**
  This interface will return a version value that corresponds to the VerId provided and
  is formatted per the CSI specification.
  - CSI Defined Interface

  @param[in]      pInst      A pointer to the IP instance to be used.
  @param[in]      VerId      An enum value defined in the CSI specification of type IP_CSI_VER_ID.

  @retval         0                       The VerId value is not recognized and a version could not be determined.
  @retval         IpCsiStsErrorNullPtr    pInst == NULL
  @retval         Non-Zero                A version value corresponding to the VerId provided, and formatted per the CSI.
**/
UINT32
IpUsb4GetVersion (
  IP_USB4_INST      *pInst,
  IP_CSI_VER_ID     VerId
  );

#endif //__IP_USB4_H__
