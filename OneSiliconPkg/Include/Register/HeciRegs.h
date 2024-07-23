/** @file
  Register Definitions for HECI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef _HECI_REGS_H_
#define _HECI_REGS_H_

///
/// HECI PCI Config register definition
///
#define R_ME_CFG_HFS                           0x40
#define R_ME_CFG_HFS_2                         0x48
#define R_ME_CFG_PMCSR                         0x54
#define V_ME_CFG_PMCSR_PS                      (BIT0 | BIT1)
#define R_ME_CFG_HFS_3                         0x60
#define R_ME_CFG_HFS_4                         0x64
#define R_ME_CFG_HFS_5                         0x68
#define R_ME_CFG_HFS_6                         0x6C

#define R_ME_CFG_H_GS                          0x4C
#define R_ME_CFG_H_GS2                         0x70
#define B_ME_CFG_H_GS2_PROXY_PRESENT           BIT8
#define B_ME_CFG_H_GS2_STORAGE_OP_IN_PROGRESS  BIT9
#define B_ME_CFG_H_GS2_IBB_DATA_READY          BIT10
#define R_ME_CFG_HIDM                          0xA0
#define V_ME_CFG_HIDM_MSI                      0
#define V_ME_CFG_HIDM_SCI                      BIT0
#define V_ME_CFG_HIDM_SMI                      BIT1
#define B_ME_CFG_HIDM_MODE                     (BIT0 | BIT1)
#define B_ME_CFG_HIDM_L                        BIT2
#define R_ME_CFG_HERS                          0xBC
#define R_ME_CFG_HER1                          0xC0
#define R_ME_CFG_HER2                          0xC4
#define R_ME_CFG_HER3                          0xC8
#define R_ME_CFG_HER4                          0xCC
#define R_ME_CFG_HER5                          0xD0
#define R_ME_CFG_HER6                          0xD4
#define R_ME_CFG_HER7                          0xD8
#define R_ME_CFG_HER8                          0xDC

///
/// HECI MBAR register definition
///
#define R_ME_MEM_H_CB_WW                       0x00  ///< Circular Buffer Write Window
#define R_ME_MEM_H_CSR                         0x04  ///< Host Control and Status Register
#define R_ME_MEM_CSE_CB_RW                     0x08  ///< Circular Buffer Read Window
#define R_ME_MEM_CSE_CSR_HA                    0x0C  ///< ME Control and Status Register (read only)
//
// R_ME_MEM_H_CSR / R_ME_MEM_CSE_CSR_HA Bit definition
//
#define B_ME_CSR_INT_ENABLE_MASK               BIT0
#define N_ME_CSR_INT_ENABLE_SHIFT              0
#define B_ME_CSR_INT_STATUS_MASK               BIT1
#define N_ME_CSR_INT_STATUS_SHIFT              1
#define B_ME_CSR_INT_GENERATE_MASK             BIT2
#define N_ME_CSR_INT_GENERATE_SHIFT            2
#define B_ME_CSR_READY_MASK                    BIT3
#define N_ME_CSR_READY_SHIFT                   3
#define B_ME_CSR_RESET_MASK                    BIT4
#define N_ME_CSR_RESET_SHIFT                   4
#define B_ME_CSR_READ_POINTER_MASK             0x0000FF00
#define N_ME_CSR_READ_POINTER_SHIFT            8
#define B_ME_CSR_WRITE_POINTER_MASK            0x00FF0000
#define N_ME_CSR_WRITE_POINTER_SHIFT           16
#define B_ME_CSR_DEPTH_MASK                    0xFF000000
#define N_ME_CSR_DEPTH_SHIFT                   24

#define R_ME_MEM_DEVIDLEC                      0x800 ///< Host to configure the power mode
#define B_ME_MEM_DEVIDLEC_CIP                  BIT0  ///< Command In Progress
#define B_ME_MEM_DEVIDLEC_DEVIDLE              BIT2  ///< Dev Idle state
#define R_ME_MEM_HER_BASE                      0xC00 ///< Host side access CSME Extended Register Base Address to get SHA384 Data (48 bytes) from DWord registers HECI1_HER1 (0xCC0) to HECI1_HER12 (0xCEC)
#define R_ME_MEM_HERS                          0xCBC ///< Host side access CSME Extended Register Status
//
// R_ME_CFG_HERS / R_ME_MEM_HERS Bit definition
//
#define B_ME_EXTEND_REG_ALGORITHM              (BIT3 | BIT2 | BIT1 | BIT0)
#define B_ME_EXTEND_COMPLETE                   BIT29
#define B_ME_EXTEND_FEAT_PRESENT               BIT30
#define B_ME_EXTEND_REG_VALID                  BIT31
#define V_ME_SHA_1                             0x00
#define V_ME_SHA_256                           0x02
#define V_ME_SHA_384                           0x04

///
/// HECI1 Host Private CR registers
///
#define R_HOST_HECI1_SB_PCR_IPIN               0x04

///
/// KT Host Private CR registers
///
#define R_HOST_KT_PCR_KTHIPINR                 0x00

#pragma pack(1)
///
/// Register definitions
///

///
/// R_ME_MEM_H_CSR      - Host Control Status Register, offset 04h
/// R_ME_MEM_CSE_CSR_HA - ME Control Status Register (Host Access), offset 0Ch
///
typedef union {
  UINT32 Data;
  struct {
    UINT32  IntEnable      : 1;  ///< 0 - Host/ME Interrupt Enable
    UINT32  IntStatus      : 1;  ///< 1 - Host/ME Interrupt Status
    UINT32  IntGenerate    : 1;  ///< 2 - Host/ME Interrupt Generate
    UINT32  Ready          : 1;  ///< 3 - Host/ME Ready
    UINT32  Reset          : 1;  ///< 4 - Host/ME Reset
    UINT32  Reserved       : 3;  ///< 7:5 - Reserved
    UINT32  CBReadPointer  : 8;  ///< 15:8 - Host/ME Circular Buffer Read Pointer
    UINT32  CBWritePointer : 8;  ///< 23:16 - Host/ME Circular Buffer Write Pointer
    UINT32  CBDepth        : 8;  ///< 31:24 - Host/ME Circular Buffer Depth
  } Fields;
} HECI_CONTROL_STATUS_REGISTER;

///
/// HFSTS1, offset 40h
///
typedef union {
  UINT32  ul;
  struct {
    UINT32  CurrentState         : 4;  ///< 0:3 - Current State
    UINT32  SpiProtectionMode    : 1;  ///< 4 - SPI Protection Mode, 0 - Protected; 1 - UnProtected
    UINT32  FptBad               : 1;  ///< 5 - FPT (Flash Partition Table) Bad
    UINT32  MeOperationState     : 3;  ///< 6:8 - ME Operation State
    UINT32  FwInitComplete       : 1;  ///< 9 - This bit is set when firmware has fully entered a stable state
    UINT32  FtBupLdFlr           : 1;  ///< 10 - This bit is set when firmware is not able to load BRINGUP from the fault tolerant (FT) code
    UINT32  FwUpdateInprogress   : 1;  ///< 11 - This bit is set when data migration is required during ME FW update process
    UINT32  ErrorCode            : 4;  ///< 12:15 - Error Code
    UINT32  MeOperationMode      : 4;  ///< 16:19 - Management Engine Current Operation Mode
    UINT32  MeRstCnt             : 4;  ///< 20:23 - Number of ME Resets
    UINT32  MeBootOptionsPresent : 1;  ///< 24 - If this bit is set, an Boot Options is present
    UINT32  InvokeEDebugMode     : 1;  ///< 25 - @deprecated - Invoke EDebug Mode
    UINT32  BistTestState        : 1;  ///< 26 - This bit informs if BIST is starting or BIST is finished
    UINT32  BistResetRequest     : 1;  ///< 27 - This bit informs if BIST Reset Request
    UINT32  CurrentPowerSource   : 2;  ///< 28:29 - Current power source selection mode
    UINT32  D3SupportValid       : 1;  ///< 30 - D3 support
    UINT32  D0i3SupportValid     : 1;  ///< 31 - D0i3 support
  } r;
} HECI_FWS_REGISTER;

///
/// HFSTS2, offset 48h
///
typedef union {
  UINT32  ul;
  struct {
    UINT32  NftpLoadFailure  : 1;  ///< 0 - NFTP Load Failure Detection Field
    UINT32  IccProgSts       : 2;  ///< 1:2 - ICC Prog STS
    UINT32  InvokeMEBx       : 1;  ///< 3 - Invoke MEBX
    UINT32  CpuReplacedSts   : 1;  ///< 4 - CPU Replaced STS
    UINT32  Reserved         : 1;  ///< 5 - Reserved
    UINT32  MfsFailure       : 1;  ///< 6 - MFS Failure
    UINT32  WarmRstReqForDF  : 1;  ///< 7 - Warm Reset Required for Dynamic Fusing
    UINT32  CpuReplacedValid : 1;  ///< 8 - CPU Replaced Valid
    UINT32  LowPowerState    : 1;  ///< 9 - Low Power State
    UINT32  MePowerGating    : 1;  ///< 10 - ME Power Gating Indicator
    UINT32  FwUpdIpu         : 1;  ///< 11 - FW Update IPU Needed
    UINT32  FwUpdForcedSB    : 1;  ///< 12 - FW Update Forced Safe Boot
    UINT32  Reserved2        : 1;  ///< 13 - Reserved
    UINT32  CseToDisabled    : 1;  ///< 14 - CSE is on the way to be disabled
    UINT32  HeciListCh       : 1;  ///< 15 - HECI Listner Change
    UINT32  ExtStatCode1     : 8;  ///< 16:23 - EXT Status Code 1
    UINT32  ExtStatCode2     : 4;  ///< 24:27 - EXT Status Code 2
    UINT32  InfPhaseCode     : 4;  ///< 31:28 - Infra. Phase code
  } r;
} HECI_GS_SHDW_REGISTER;

///
/// HFSTS3, offset 60h
///
typedef union {
  UINT32  ul;
  struct {
    UINT32 Reserved                : 4; ///< 3:0 - Reserved
    UINT32 FwSku                   : 3; ///< 6:4 - FW Sku
    UINT32 TransactionalState      : 1; ///< 7 - Transactional State
    UINT32 StorageProxyPresent     : 1; ///< 8 - Storage Proxy Present
    UINT32 Reserved2               : 2; ///< 10:9 - Reserved
    UINT32 CseRpmcStatus           : 4; ///< 14:11 - CSE RPMC Status
    UINT32 CseRpmcDeviceExtStatus  : 3; ///< 17:15 - CSE RPMC Device Extended Status
    UINT32 BiosRpmcStatus          : 4; ///< 21:18 - BIOS RPMC Status
    UINT32 BiosRpmcDeviceExtStatus : 3; ///< 24:22 - BIOS RPMC Device Extended Status
    UINT32 Reserved3               : 7; ///< 31:25 - Reserved
  } r;
} HECI_FW_STS3_REGISTER;

///
/// HFSTS4, offset 64h
///
typedef union {
  UINT32  ul;
  struct {
    UINT32 Reserved         : 2;  ///< 1:0 - Reserved
    UINT32 FlashLogExist    : 1;  ///< 2 - Flash Log Exist
    UINT32 Reserved2        :29;  ///< 31:3 - Reserved
  } r;
} HECI_FW_STS4_REGISTER;

///
/// HFSTS5, offset 68h
///
typedef union {
  UINT32  ul;
  struct {
    UINT32 AcmActived       : 1;  ///< 0 - Boot Guard ACM Active STS, 0 - Unknown Boot Guard ACM status; 1 - Boot Guard ACM is active
    UINT32 VLD              : 1;  ///< 1 - Valid Bit, 0 - HFSTS#5 Bits [7:2] are invalid, 1 - Valid.
    UINT32 RCS              : 1;  ///< 2 - Result Code Source, 0 - Boot Guard ACM; 1 - CPU Microcode
    UINT32 ESC              : 5;  ///< 7:3 - Error Status Code
    UINT32 AcmDone          : 1;  ///< 8 - Boot Guard ACM DONE STS, 0 - Boot Guard ACM is not done; 1 - Boot Guard ACM is done
    UINT32 Smtc             : 7;  ///< 15:9 - Startup Module Timeout Count
    UINT32 ScrtmIndicator   : 1;  ///< 16 - S-CRTM Indicator
    UINT32 TxtSupport       : 1;  ///< 17 - TXT Support
    UINT32 BtgProfile       : 3;  ///< 20:18 - Boot Guard Profile
    UINT32 CpuDebugDisabled : 1;  ///< 21 - CPU Debug Disabled
    UINT32 BspInitDisabled  : 1;  ///< 22 - BSP Initialization Disabled
    UINT32 ExeStatus        : 1;  ///< 23 - BSP Boot Policy Manifest Execution Status
    UINT32 BtgTokenApplied  : 1;  ///< 24 - Indicates if CSE Boot Guard behavior has been modified because of Secure Token consumption
    UINT32 BtgStatus        : 4;  ///< 28:25 - Boot Guard Status
    UINT32 Reserved         : 2;  ///< 30:29 - Reserved
    UINT32 StartEnforcement : 1;  ///< 31 - Start Enforcement, 0 - no action; 1 - PCH starts the error enforcement logic
  } r;
} HECI_FW_STS5_REGISTER;

///
/// HFSTS6, offset 6Ch
///
typedef union {
  UINT32  ul;
  struct {
    UINT32 Reserved           :21;  ///< 20:0 - Reserved
    UINT32 ManufacturingLock  : 1;  ///< 21 - Manufacturing variables lock state, 0 - Not locked; 1 - Locked
    UINT32 Reserved2          : 8;  ///< 29:22 - Reserved
    UINT32 FpfSocConfigLock   : 1;  ///< 30 - Field Programmable Fuses (FPF) SOC Configuration Lock
    UINT32 SxResumeType       : 1;  ///< 31 - Sx Resume Type, 0- S4, S5 or G3; 1 S3, deep S3 for Intel Rapid Start Tech.
  } r;
} HECI_FW_STS6_REGISTER;

//
// ME Current State Values
//
#define ME_STATE_RESET        0x00
#define ME_STATE_INIT         0x01
#define ME_STATE_RECOVERY     0x02
#define ME_STATE_DISABLED     0x04
#define ME_STATE_NORMAL       0x05
#define ME_STATE_DISABLE_WAIT 0x06
#define ME_STATE_TRANSITION   0x07
#define ME_STATE_INVALID_CPU  0x08

//
// ME Firmware FwInitComplete
//
#define ME_FIRMWARE_COMPLETED   0x01
#define ME_FIRMWARE_INCOMPLETED 0x00

//
// ME Boot Options Present
//
#define ME_BOOT_OPTIONS_PRESENT     0x01
#define ME_BOOT_OPTIONS_NOT_PRESENT 0x00

//
// ME Operation State Values
//
#define ME_OPERATION_STATE_PREBOOT  0x00
#define ME_OPERATION_STATE_M0_UMA   0x01
#define ME_OPERATION_STATE_M3       0x04
#define ME_OPERATION_STATE_M0       0x05
#define ME_OPERATION_STATE_BRINGUP  0x06
#define ME_OPERATION_STATE_M0_ERROR 0x07

//
// ME Error Code Values
//
#define ME_ERROR_CODE_NO_ERROR      0x00
#define ME_ERROR_CODE_UNKNOWN       0x01
#define ME_ERROR_CODE_DISABLED      0x02
#define ME_ERROR_CODE_IMAGE_FAILURE 0x03
#define ME_ERROR_CODE_DEBUG_FAILURE 0x04

//
// ME Current Operation Mode
//
#define ME_OPERATION_MODE_NORMAL            0x00
#define ME_OPERATION_MODE_DEBUG             0x02
#define ME_OPERATION_MODE_SOFT_TEMP_DISABLE 0x03
#define ME_OPERATION_MODE_SECOVR_JMPR       0x04
#define ME_OPERATION_MODE_SECOVR_HECI_MSG   0x05

//
// ME Firmware SKU Values
//
#define ME_FWSTS3_FW_SKU_CONSUMER     0x02
#define ME_FWSTS3_FW_SKU_CORPORATE    0x03
#define ME_FWSTS3_FW_SKU_LITE         0x05

//
// ME Timeout Values
//
#define ME_STATE_STALL_1_SECOND  1000000
#define ME_MODE_MAX_WAIT_TIMEOUT 30
#define MSG_MAX_WAIT_TIMEOUT     5
#define FW_INIT_DONE_TIMEOUT     15  // 15 seconds for firmware init done timeout value
#define ME_FORMAT_FILE_TIMEOUT   180 // 180 seconds for formating file system and restoring data timeout value
#define PSE_MAX_WAIT_TIMEOUT     15  // 15 seconds for PSE polling register change timeout value

#pragma pack()

#endif // _HECI_REGS_H_
