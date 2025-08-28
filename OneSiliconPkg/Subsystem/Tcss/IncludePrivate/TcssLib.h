/** @file
  Header file for TCSS Init Lib.

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
#ifndef _TCSS_LIB_H_
#define _TCSS_LIB_H_

#include <IpCsi.h>
#include <IpUsb3.h>
#include <IpUsb4.h>
#include <IpIom.h>
#include <IpTcPcieRp.h>
#include <TcssPeiPreMemConfig.h>

#define TCSS_VERSION_HW           0x00020005
#define TCSS_VERSION_HW_CUSTOM    0x00000001
#define TCSS_VERSION_FW           0x00020002
#define TCSS_VERSION_IPFWINTF     0x00020002
#define TCSS_VERSION_IPFWINTFEXT  0x00020002
#define VERSION_UNKNOWN             0

#ifndef BOOT_ON_S3_RESUME
#define BOOT_ON_S3_RESUME           0x11
#endif

#ifndef BOOT_ON_S4_RESUME
#define BOOT_ON_S4_RESUME           0x05
#endif

#ifndef PERF_INMODULE_BEGIN
#define PERF_INMODULE_BEGIN(s)
#endif

#ifndef PERF_INMODULE_END
#define PERF_INMODULE_END(s)
#endif

#ifndef MAX_TCSS_USB3_PORTS
#define MAX_TCSS_USB3_PORTS            4
#endif

typedef struct _TCSS_INST TCSS_INST;

///
/// Structure for Tbt firmware status from IMR.
///
typedef union {
  UINT32 RegValue;
  struct {
    UINT32 TbtFwVersion:16; ///< Bits 15:0
    UINT32 Reserved:6;      ///< Bits 21:16
    UINT32 ErrorCode:8;     ///< Bits 29:22
    UINT32 Valid:1;         ///< Bits 30    - 00 = untrusted FW, 01 = successful authentication
    UINT32 Done:1;          ///< Bits 31    - 01 = FW download to IMR is done
  } Bits;
} TC_TBT_IMR_STATUS;

///
/// Structure for MG image status from IMR.
///
typedef union {
  UINT32 RegValue;
  struct {
    UINT32 MgFwVersion:16; ///< Bits 15:0
    UINT32 Reserved:6;     ///< Bits 21:16
    UINT32 ErrorCode:8;    ///< Bits 29:22
    UINT32 Valid:1;        ///< Bits 30    - 00 = untrusted FW, 01 = successful authentication
    UINT32 Done:1;         ///< Bits 31    - 01 = FW download to IMR is done
  } Bits;
} TC_MG_IMR_STATUS;

///
/// The IOM_DEVEN_CONFIG describes Device Enable settings for TCSS.
///
typedef union {
  UINT32      TcssDevEn;        ///< Maps to bits in TCSS DEVEN register
  struct {
    UINT32    TcssPcieRpEn : 8;
    UINT32    TcssXhciEn   : 1;
    UINT32    Rsvd         : 1;
    UINT32    Usb4HrEn     : 4;
    UINT32 : 18;
  } TcssDevEnBit;
} TCSS_DEVEN_CONFIG;

//
// TCSS Port Convert to TypeA.
// The Data is avtive when Enable field is '1'.
//
typedef union {
  struct {
    UINT8  MappingPchXhciUsb2 :4;  ///< PCH xhci port number for Type C Port x mapping
    UINT8  Rsvd               :2;  ///< Reserved bytes
    UINT8  Orientation        :1;  ///< Enable / Disable Orientation bit. <b>0: Disable</b>; 1: Enable.
    UINT8  Enable             :1;  ///< Enable / Disable Type C Port X Convert to TypeA.
  } Field;
  UINT8 Data8;
} TCSS_TYPEC_CONV_USB_A;

///
/// IOM settings for TCSS.
///
typedef union {
  UINT32  PortEnData32;
  UINT8   CapPolicy[MAX_ITBT_PCIE_PORT];
} TCSS_USBTC_CONFIG;

/**
 TCSS configuration options.
**/
typedef struct {
  UINT8                  CridEnable;                      ///< Compatibility Revision ID
  UINT8                  MaxTcssUsb3Ports;                ///< Number of USB3 ports implemented in TCSS hardware
  UINT8                  LsxOe;                           ///< Bits defining value for LSx OE Orientation
  UINT8                  Rsvd[1];                         ///< Reserved bytes
  TCSS_USBTC_CONFIG      PortEnData;                      ///< TCSS USBC Ports Configuration
  UINT32                 PmcPdEnable;                     ///< PMC PD Solution Enable
  UINT32                 BootMode;                        ///< Boot Mode
  UINT32                 VccSt;                           ///< IOM VCCST request enable
  TCSS_DEVEN_CONFIG      Deven;                           ///< TCSS Device Enables
  TCSS_TYPEC_CONV_USB_A TcssConvUsbA[MAX_TCSS_USB3_PORTS];///< USB Type C to A conversion
} TCSS_CONFIG;

//
// TCSS information
//
typedef struct {
  UINT32            IOMReady : 1;           ///< Indicate the IOM ready.
  UINT32            Rsvd : 30;              ///< Reserved for future use.
  UINT32            IomFwVersion;           ///< IOM FW version.
  UINT32            IomFwEngrVersion;       ///< IOM FW Engineer version. Non-zero value used to identify engineering versions / hotfixes. Shall be zero for official releases.
  UINT32            ItbtPcieFuncMap;        ///< iTBT PCIe Root Port function mapping
  TC_TBT_IMR_STATUS TbtImrStatus;           ///< TBT FW IMR Status.
  TC_MG_IMR_STATUS  MgImrStatus;            ///< MG IMR Status
  UINT32            SwConfiguration1Lock;   ///< Configuration Lock mask 1
  UINT32            SwConfiguration3Lock;   ///< Configuration Lock mask 2
  UINT32            SwConfiguration4Lock;   ///< Configuration Lock mask 3
  UINT32            MiscConfugurationLock;  ///< USB4HR Misc Configuration Lock
  UINT32            DeepStTcState;          ///< TCSS Deep ST State
} TCSS_INFO;

/**
  TCSS PCIe Root Ports Function programing in SOC IO Cache service

  @param[in]  FuncMap PCIe RP function mapping
**/
typedef
void
(*PCIE_RP_SET_SOC_PCIE_FUN_MAP) (
  UINT32    FuncMap
  );

/**
  Set SubVendor and SubSystem IDs for TCSS PCIe devices:
  PCIe Root Ports, USB3, Host DMAs
**/
typedef
void
(*SET_SVID_SSID) (
  );

/**
  Enables PCIe PME Events for TCSS Root Ports

**/
typedef
void
(*ENABLE_PCIE_PME_EVENTS) (
  void
  );

/**
  Configure TC LSX pins in SOC GPIO service

  @param[in] Index   TCSS USBC Port Index
**/
typedef
void
(*LSX_PINS_CONFIGURE) (
  UINT8            Index
  );

/**
  Configure TCSS LSx_OE pins in SOC GPIO service

  @param[in] Index   TCSS USBC Port Index
**/
typedef
void
(*LSX_OE_PINS_CONFIGURE) (
  UINT8            Index
  );

/**
  Converts USB Type C to Type A

  @param[in]  TcssUsbcPort    Tcss USB Type C port number
  @param[in]  PchXhciUsbPort  USB3 port number
  @param[in]  OriHslEnable    ORI HSL Enable

  @retval   UINTN             Status
**/
typedef
UINTN
(*USB_C_TO_A_CONVERT) (
  UINT8                   TcssUsbcPort,
  UINT8                   PchXhciUsbPort,
  UINT8                   OriHslEnable
  );

/**
  Handshake with PMC if PMC handshaking support is enabled BIOS sends TCSS_BIOS_INIT Done (aka.USBC connection replay)
  message along with USBC connctions replay setting to PMC. All TCSS initialization is expected to be done before PMC
  do any port negotiation. The indicator sent from BIOS to PMC should be treated as TCSS BIOS initialization done
  semaphore.

  @param[in]       IomReadyStatus       Status of IomReady bit read from TCSS Hob
  @param[in]       TcssPmcBitMask       TCSS PMC status, BIT1: Replay status based on VccSt and BootMode

  @retval          EFI_SUCCESS          Command success
  @retval          EFI_DEVICE_ERROR     Command error
**/
typedef
UINTN
(*PMC_TCSS_BIOS_INIT_DONE) (
  IN  UINT32               IomReadyStatus,
  IN  UINT32               TcssPmcBitMask
  );

/**
  Reads the TCSS Device Enables (DEVEN) register from SOC IOC service

  @retval UINT32  TCSS DEVEN Register value

**/
typedef
UINT32
(*GET_TCSS_DEVEN) (
  void
);

/**
  List of upper layer function pointers (callbacks)
  that can be passed to Subsystem initialization library.
  Used for external IP and SoC related communication.
**/
typedef struct {
  // PCIe Rp Segment, Bus, Device a Function should be programmed in SOC IO Cache service
  // The programming MUST be synchronized between IOM and external IOC.
  // The programming must be executed in proper order and moment.
  PCIE_RP_SET_SOC_PCIE_FUN_MAP   PcieSetFunctionMapInSoc;

  // Set SubVendor and SubSystem IDs for TCSS PCIe devices:
  // PCIe Root Ports, USB3, Host DMAs
  // The programing must be executed after all PCIe devices are initialized
  // But before xHCI controller configuration is locked
  SET_SVID_SSID       SetSvidSsid;

  // TC LSX Pins configuration
  // Required to configure physical pins during TCSS programming sequence
  // Needs to be synchronized with internal IOM IP programming
  LSX_PINS_CONFIGURE  LsxPinsConfigure;

  // TCSS LSx OE pins configuration
  // Required to configure physical pins during TCSS programming sequence
  // Needs to be synchronized with internal TCSS IP programming
  LSX_OE_PINS_CONFIGURE  LsxOePinsConfigure;

  // USB Type C to Type A conversion
  // Power Management Controller (PMC) programming depends
  // on result of xHCI controller initialization
  USB_C_TO_A_CONVERT  ConvertUsbCToUsbA;

  // Handshake with Power Management Controller (PMC)
  // BIOS sends TCSS_BIOS_INIT Done message along USBC connections
  // to replay setting to PMC.
  // All TCSS initialization is expected to be done before PMC TCSS_BIOS_INIT Done
  PMC_TCSS_BIOS_INIT_DONE PmcTcssBiosInitDone;

  // Reads the TCSS Device Enables register from SOC IOC device
  GET_TCSS_DEVEN GetTcssDeven;

  // Enables PCIe PME Events for TCSS Root Ports
  // before USBr Host Router is initialized.
  ENABLE_PCIE_PME_EVENTS  EnablePciePmeEvents;
} TCSS_CALLBACKS;

struct _TCSS_INST {
  IP_CSI_INST_PREFIX  Prefix;

  //IP Wrapper Print Contexts
  IP_WR_PRINT_CNTXT   PrintError;
  IP_WR_PRINT_CNTXT   PrintWarning;
  IP_WR_PRINT_CNTXT   PrintLevel1;

  // Type-C Subsystem Sub-IPs Instance Structures
  IP_USB3_INST        *Usb3Inst;
  IP_USB4_INST        *Usb4Inst;
  IP_TC_PCIE_RP_INST  *PcieInst;
  IP_IOM_INST         *IomInst;

  // TCSS Configuration Options
  TCSS_CONFIG         *Config;

  // TCSS Information Data
  TCSS_INFO           *Info;

  // Upper layer function pointers
  TCSS_CALLBACKS      *Callbacks;
};

/**
  TCSS Instance Initialization

  @param[in]  pInst  The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
TcssInstInit (
  TCSS_INST   *pInst
  );

/**
  TCSS initialization procedure entry point

  @param[in]  pInst instance

  @retval  IP_CSI_STATUS
    - IpCsiStsSuccess:  programming completed without error
    - IpCsiStsErrorNullPtr:  null pointer to IP Instance provided
**/
IP_CSI_STATUS
SsTcssInit (
  TCSS_INST   *pInst
  );

/**
Returns version of Type-C Subsystem related components

@param[in]      pInst the instance
@param[in]      VerId version ID

@retval         0 is returned upon any error
@retval         return value define by VerId

**/
UINT32
TcssGetVersion (
  TCSS_INST     *pInst,
  IP_CSI_VER_ID VerId
  );

/**
  TCSS initialization procedure executed before memory is discovered.

  @param[in]  pInst             TCSS Instance

  @retval  IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         actual register values differ from expected ones
    - IpCsiStsErrorNullPtr:  null pointer to IP Instance provided
**/
IP_CSI_STATUS
SsTcssInitPreMemory (
  TCSS_INST   *pInst
  );

/**
  TCSS initialization procedure executed after memory is discovered.

  @param[in]  pInst             TCSS Instance

  @retval  IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:            an error during the programming
    - IpCsiStsErrorNotAllowed:  IOM is corrupted
    - IpCsiStsErrorNullPtr:  null pointer to IP Instance provided
**/
IP_CSI_STATUS
SsTcssInitPostMemory (
  TCSS_INST   *pInst
  );

/**
  TCSS initialization procedure executed at the end of PEI phase.

  @param[in] pInst  TCSS Instance

  @retval IP_CSI_STATUS
    - IpCsiStsSuccess: Programming completed without error
    - IpCsiStsError:   Error during the programming
**/
IP_CSI_STATUS
SsTcssInitEndOfPei (
  TCSS_INST *pInst
  );

/**
  Returns TCSS informations

  @param[in]  pInst             TCSS Instance
  @param[out] Info              TCSS Information

  @retval  IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed without error
    - IpCsiStsError:         Returned data can de invalid or not complete
    - IpCsiStsErrorNullPtr:  null pointer to IP Instance or Info provided
**/
IP_CSI_STATUS
SsTcssGetInfo (
  TCSS_INST   *pInst
  );

#endif
