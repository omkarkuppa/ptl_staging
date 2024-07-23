/** @file
  IP API for IOM

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

#ifndef __IP_IOM_H__
#define __IP_IOM_H__

#include <IpCsi.h>

#define VERSION_HW_IOM              0x00020001
#define VERSION_HW_CUSTOM_IOM       0x00000001
#define VERSION_FW_IOM              0x00000003
#define VERSION_IPFWINTF_IOM        0x05230103
#define VERSION_IPFWINTFEXT_IOM     0x00000003
#define VERSION_UNKNOWN_IOM         0

#ifndef MAX_IOM_AUX_BIAS_COUNT
#define MAX_IOM_AUX_BIAS_COUNT      4
#endif  //MAX_IOM_AUX_BIAS_COUNT

#define IOM_DP                      0x1
#define IOM_HDMI                    0x2
#define IOM_EDP                     0x3

#ifndef MAX_TYPEC_PORT_NUM
#define MAX_TYPEC_PORT_NUM    2
#endif  //MAX_TYPEC_PORT_NUM

#ifndef _TCSS_VER
#define _TCSS_VER
typedef enum {
  TcssOld   = 0,
  TcssNew   = 1
} TCSS_VER;
#endif // _TCSS_VER

#ifndef ENVIRONMENT_DEFINED
#define ENVIRONMENT_DEFINED
typedef enum {
  SiliconEnv = 0,
  SimicsEnv  = 1,
  HfpgaEnv   = 2
} ENVIRONMENT;
#endif //ENVIRONMENT_DEFINED

#define FIX_CONNECTION_PORTS_MAX    6

#define B_IOM_BIOS_MAIL_BOX_IOM_EN  BIT16

typedef struct {
  UINT32  Lock1;
  UINT32  Lock3;
  UINT32  Lock4;
  UINT32  Lock5;  // IOM USB4.HR MISC.CONFIG LOCK
} IP_IOM_CONFIG_LOCK;

typedef enum {
  TypeCPortConf    = 0,  // 0000    : Type-C connector
  DPPortConf       = 1,  // 0001    : DisplayPort
  HDMIPortConf     = 2,  // 0010    : HDMI
  EDPPortConf      = 3,  // 0011    : eDP
  TypeAPortConf    = 4,  // 0100    : Type-A connector
  DisabledPortConf = 15  // 1111    : Port is disabled
} TCSS_PORT_CONF;

typedef enum {
  Retimerless   = 0,   ///< No retimer or redriver present
  Retimer       = 1,   ///< Retimer present
  Redriver      = 2,   ///< Redriver present
  Dicom         = 3    ///< Dicom present
} TCSS_PLATFORM_CONF;

typedef union {
  UINT32 RegValue;
  struct {
    UINT32 MgFwVersion :16;  ///< Bits 15:0
    UINT32 Reserved    :6;   ///< Bits 21:16
    UINT32 ErrorCode   :8;   ///< Bits 29:22
    UINT32 Valid       :1;   ///< Bits 30    - 00 = untrusted FW, 01 = successful authentication
    UINT32 Done        :1;   ///< Bits 31    - 01 = FW download to IMR is done
  } Bits;
} IOM_MG_IMR_STATUS;

///
/// The IOM_AUX_ORI_PAD_CFG describes IOM TypeC port map GPIO pin.
/// Those GPIO setting for DP Aux Orientation Bias Control when the TypeC port didn't have re-timer.
/// IOM needs know Pull-Up and Pull-Down pin for Bias control
///
typedef struct {
  UINT32     ReceptacleSbu1BiasCtrl; ///< GPIO Aux Bias Ctrl Pull Up Pin number that is for IOM indicate the pull up pin from TypeC port.
  UINT32     ReceptacleSbu2BiasCtrl; ///< GPIO Aux Bias Ctrl Pull Down Pin number that is for IOM indicate the pull down pin from TypeC port.
  UINT32     AuxIsoCtrl;     ///< GPIO ISO control pin GPIO number that is IOM indicate the AUX active from SOC.
} IOM_AUX_ORI_PAD_CFG;

///
/// The IOM_DEVEN_CONFIG describes Device Enable settings for TCSS.
///
typedef union {
  UINT32      TcssDevEn;        ///< Maps to bits in TCSS DEVEN register
  struct {
    UINT32    TcssPcieRpEn : 8;
    UINT32    TcssXhciEn   : 1;  /// XHCI GENX
    UINT32    Rsvd         : 1;
    UINT32    Usb4HrEn     : 4;
    UINT32    Rsrvd1       : 1;
    UINT32    XhciGenTEn   : 1;  /// GENT
    UINT32 : 16;
  } TcssDevEnBit;
} IOM_DEVEN_CONFIG;

  ///
  /// TCSS device initialization done indicators
  ///
  typedef union {
    UINT32  Value;
    struct {
      UINT32   PcieRp : 1;
      UINT32   Xhci   : 1;
      UINT32   Usb4   : 1;
      UINT32          : 29;
    } Dev;
  } DEV_INIT_DONE;

/**
  This function performs basic initialization IOM for AUX Layout.

  For those GPIO pins used for DP Aux orientation control and enabled, BIOS is responsible to -
  1. Configure Pad Mode (PMode) to function# associated with IOM_GPP*_*
     The Pad Mode here could be various per PCH design.
  2. BIOS Provide the following information for the DP Aux orientation bias control for provide the GPIO VwIndex,
     Bit Position, and VW Index information to IOM FW.
     I. GPIO endpoint IOSF-SB port ID (Pch Community port ID)
     II. VW index and data bit position

  @param[in]  GpioPadNum  - GPIO Pad number
  @param[out] PortId      - GPIO Pad mapping GPCOM PortID number
  @param[out] Bits        - GPIO Pad mapping Bit position
  @param[out] VwIndex     - GPIO Pad mapping Virtual Wire Index number
 **/
typedef
void
(*GET_AUX_ORI_GPIO) (
  UINT32   GpioPadNum,
  UINT16   *PortId,
  UINT8    *Bits,
  UINT8    *VwIndex
  );

typedef
void
(*SET_DEVEN_MCHBAR) (
  UINT32   Deven
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


typedef struct {
  GET_AUX_ORI_GPIO        GetAuxOriGpioSettings;
  SET_DEVEN_MCHBAR        SetTcssDevenRegisterMchbar;
  PMC_TCSS_BIOS_INIT_DONE PmcTcssBiosInitDone;
} IP_IOM_CALLBACKS;

typedef struct {
  UINT64   RandomNum;        // Random Number for UUID in USB4HR
  UINT16   Usb4hrSvid;       // Subvendor ID for USB4HR
  UINT16   Usb4hrSsid;       // Subsystem ID for USB4HR
  UINT8    Usb4Rtd3Enabled;  // boolean value indicating if RTD3 on USBHR should be enabled
} IP_IOM_USB4HR_CONFIG;

typedef struct {
  IOM_DEVEN_CONFIG        DevEn;
  UINT32                  D3ColdEnable;
  UINT32                  VccSt;
  UINT32                  TcStateLimit;
  UINT32                  AuxBiasCount;
  IOM_AUX_ORI_PAD_CFG     IomAuxPortPad[MAX_IOM_AUX_BIAS_COUNT];
  UINT16                  AuxOriOverride;
  UINT16                  HslOriOverride;
  UINT8                   FixConPort[FIX_CONNECTION_PORTS_MAX];
  ENVIRONMENT             Environment;
  UINT8                   TcNotifyIgd;
  TCSS_PORT_CONF          TypeCPortConf[MAX_TYPEC_PORT_NUM];
  UINT8                   IsHtiEn;
  TCSS_PLATFORM_CONF      TypeCPlatConf[MAX_TYPEC_PORT_NUM];
  UINT8                   EdpEnabled;
  UINT8                   IsUsb4V2CapabilityEn;
} IP_IOM_CONFIG;

typedef struct _IP_IOM_INST IP_IOM_INST;
struct _IP_IOM_INST {
  IP_CSI_INST_PREFIX    Prefix;

  // IP Wrapper Register Contexts
  IP_WR_REG_CNTXT       RegCntxtIomSb;

  // IP Wrapper Print Contexts
  IP_WR_PRINT_CNTXT     PrintError;
  IP_WR_PRINT_CNTXT     PrintWarning;
  IP_WR_PRINT_CNTXT     PrintLevel1;

  // IP Wrapper Timing Contexts
  IP_WR_TIME_CNTXT      TimeCntxt;

  // IOM IP Public Variables
  IP_IOM_CONFIG         IomConfig;

  // IOM IP Callbacks
  IP_IOM_CALLBACKS      Callback;

  IP_IOM_USB4HR_CONFIG  HostRouterSettings;

  // to handle differences between new and old flow
  TCSS_VER              TcssVersion;
};

/**
  InstInit for this version of IP

  @param[in] pInst  The instance, already allocated by the FSP

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIomInstInit (
  IP_IOM_INST      *pInst
  );

/**
  Since InstInit is not allowed to access any registers, IpIomInit is used to write
  to any registers needed.
  * IpIomInit () shall be the first API function called after IpIomInstInit.
  * this is a good place to fix bad register defaults or work around bugs

  @param[in]  pInst instance

  @retval         IpCsiStsSuccess on success
  @retval         !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIomIpInit (
  IP_IOM_INST     *pInst
  );

/**
  This interface will return a version value that corresponds to the VerId provided and
  is formatted per the CSI specification.
  - CSI Defined Interface

  @param[in]      pInst      A pointer to the IP instance to be used.
  @param[in]      VerId      An enum value defined in the CSI specification of type IP_CSI_VER_ID.

  @retval         0          The VerId value is not recognized and a version could not be determined.
  @retval         Non-Zero   A version value corresponding to the VerId provided, and formatted per the CSI.
**/
UINT32
IpIomGetVersion (
  IP_IOM_INST      *pInst,
  IP_CSI_VER_ID     VerId
  );

/**
  This function initializes Aux and HSL Orientation registers in IOM.

  @param[in] pInst    IOM IP instance structure

  @retval     IpCsiStsSuccess - AuxHslOri set correctly
  @retval     !IpCsiStsSuccess - Error case
**/
IP_CSI_STATUS
IpIomAuxHslOriInit (
  IP_IOM_INST   *pInst
  );

/**
  This function programs fixed connection for TCSS ports.

  @param[in]    pInst       IOM instance structure pointer

  @retval   IP_CSI_STATUS
  - IpCsiStsSuccess:       programming completed without error
  - IpCsiStsError:         IOM programming error
**/
IP_CSI_STATUS
IpIomFixedConnectionPortInit (
  IP_IOM_INST        *pInst
  );

/**
  Initializes TCSS Device Enables in IOM

  @param[in]    IomInst     IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         IOM programming error
**/
IP_CSI_STATUS
IpIomDevEnInit (
  IP_IOM_INST     *IomInst
  );

/**
  Enables Compatibility Revision ID in IOM

  @param[in]    IomInst     IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         IOM programming error
**/
IP_CSI_STATUS
IpIomCrIdProgram (
  IP_IOM_INST     *IomInst
  );

/**
  Initializas IOM at Post-Mem phase
  - Program VccSt
  - Program D3ColdEnable
  - Program C State Limit
  - SA XDCI IRQ Programming
  - Program Aux and HSL Orientation

  @param[in]    IomInst     IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         IOM programming error
**/
IP_CSI_STATUS
IpIomInitPostMemory (
  IP_IOM_INST   *IomInst
  );

/**
  IOM configuration lock procedure

  @param[in]    IomInst     IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         IOM programming error
**/
IP_CSI_STATUS
IpIomConfigurationLock (
  IP_IOM_INST   *IomInst
  );

/**
  Returns IOM Deep St TypeC State

  @param[in]    pInst       IOM instance structure
  @param[out]   State       Pointer to state variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfuly
    - IpCsiStsError:         Error during IOM reading
    - IpCsiStsErrorNullPtr:  Status Pointer not set
**/
IP_CSI_STATUS
IpIomGetDeepStTcState (
  IP_IOM_INST   *pInst,
  UINT32        *State
  );

/**
  This function gets TCSS SoC product information.

  @param[in]    pInst             IOM instance structure
  @param[out]   SkuId             TCSS SoC Sku ID pointer
  @param[out]   BaseSteppingId    TCSS SoC Base stepping ID pointer
  @param[out]   DashSteppingId    TCSS SoC Dash stepping ID pointer

  @retval       IP_CSI_STATUS
                - IpCsiStsSuccess:       Executed successfully
                - IpCsiStsErrorNullPtr:  SkuId Pointer not set
                - IpCsiStsErrorNullPtr:  BaseSteppingId Pointer not set
                - IpCsiStsErrorNullPtr:  DashSteppingId Pointer not set

**/
IP_CSI_STATUS
IpIomGetTcssProductInfo (
  IP_IOM_INST     *pInst,
  UINT8           *SkuId,
  UINT8           *BaseSteppingId,
  UINT8           *DashSteppingId
  );

/**
  Checks if IOM controller is ready to use

  @param[in]    IomInst     IOM instance structure
  @param[out]   Ready       Pointer to ready variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsError:         Error during IOM reading
    - IpCsiStsErrorNullPtr:  Ready Pointer not set
**/
IP_CSI_STATUS
IpIomIsReady (
  IP_IOM_INST   *IomInst,
  UINT32        *Ready
  );

/**
  Returns IOM firmware version

  @param[in]    IomInst     IOM instance structure
  @param[out]   Version     Pointer to version variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsError:         Error during IOM reading
    - IpCsiStsErrorNullPtr:  Version Pointer not set
**/
IP_CSI_STATUS
IpIomGetFwVersion (
  IP_IOM_INST   *IomInst,
  UINT32        *Version
  );

/**
  Returns IOM Engineering firmware version

  @param[in]    IomInst     IOM instance structure
  @param[out]   Version     Pointer to version variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsError:         Error during IOM reading
    - IpCsiStsErrorNullPtr:  Version Pointer not set
**/
IP_CSI_STATUS
IpIomGetFwEngrVersion (
  IP_IOM_INST   *IomInst,
  UINT32        *Version
  );

/**
  Returns IOM Isolated Memory Region status

  @param[in]    IomInst     IOM instance structure
  @param[out]   Status      Pointer to status variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsError:         Error during IOM reading
    - IpCsiStsErrorNullPtr:  Status Pointer not set
**/
IP_CSI_STATUS
IpIomGetImrMgStatus (
  IP_IOM_INST   *IomInst,
  UINT32        *Status
  );

/**
  Returns IOM Isolated Memory Region TBT status

  @param[in]    pInst       IOM instance structure
  @param[out]   Status      Pointer to status variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:                    Executed successfully
    - IpCsiStsNotReady:                   TBT FW not downloaded to IMR
    - IpCsiStsErrorInsufficientResources: Untrusted TBT FW detected
    - IpCsiStsErrorNullPtr:               Status Pointer not set
**/
IP_CSI_STATUS
IpIomGetImrTbtStatus (
  IP_IOM_INST   *pInst,
  UINT32        *Status
  );

/**
  Returns IOM configuration lock status

  @param[in]    pInst       IOM instance structure
  @param[out]   ConfigLock  Pointer to config structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsError:         Error during IOM reading
    - IpCsiStsErrorNullPtr:  ConfigLock Pointer not set
**/
IP_CSI_STATUS
IpIomGetSwConfigs (
  IP_IOM_INST         *pInst,
  IP_IOM_CONFIG_LOCK  *ConfigLock
  );

/**
  Programs TCSS Devices Initialization Done

  @param[in]    pInst       IOM instance structure
  @param[out]   Done        TCSS Devices Init Done

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsError:         Error during IOM programming
**/
IP_CSI_STATUS
IpIomSetDevInitDone (
  IP_IOM_INST   *pInst,
  DEV_INIT_DONE Done
  );


/**
  Sets SVID, SSID and UUID in USB4HR

  @param[in]    pInst    IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
**/
IP_CSI_STATUS
IpIomSetUsb4Id (
  IP_IOM_INST   *pInst
  );

/**
  Could enable RTD3 in USB4HR
  Lock IOM_USB4HR_MISC_CONFIG, IOM_USB4HR_SSID_SVID
  and IOM_USB4HR_UUID registers

  @param[in]    pInst        IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
**/
IP_CSI_STATUS
IpIomSetUsb4Rtd3 (
  IP_IOM_INST   *pInst
  );

/**
  Enable and disable HTI in TCSS_DEBUG_CONFIG register
  Set TCSS_PORT_CONTROL registers according to TypeCPortCap.
  Set IOM_USB4HR_MISC_CONFIG.ENABLE80G_BIOS_PREFERENCE and TCSS_PORT_CONFIG according to IOM_FUSE_REG1

  @param[in]    pInst        IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
**/
IP_CSI_STATUS
IpIomTcssSettings (
  IP_IOM_INST   *pInst
  );

/**
  Enable eDP on TypeC port 0

  @param[in]    pInst        IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
**/
IP_CSI_STATUS
IpIomEnableEdp (
  IP_IOM_INST   *pInst
  );

#endif //__IP_IOM_H__
