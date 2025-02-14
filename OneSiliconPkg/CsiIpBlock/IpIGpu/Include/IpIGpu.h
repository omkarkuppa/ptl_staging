/** @file
  IP API for IGPU

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

#ifndef __IP_IGPU_H__
#define __IP_IGPU_H__

#include <IpCsi.h>
#include "IpIGpuConfig.h"
#include "IpIGpuDefines.h"

#if !defined (MDEPKG_NDEBUG)
#define IP_IGPU_PRINT_ALL
#endif

#define VERSION_IPFWINTFEXT_IGPU_GEN    (0x13)
#define VERSION_IPFWINTFEXT_IGPU_MAJOR  (0x00)
#define VERSION_IPFWINTFEXT_IGPU_MINOR  (0x0011)

#define VERSION_IPFWINTFEXT_IGPU  ((VERSION_IPFWINTFEXT_IGPU_MAJOR<<16) | (VERSION_IPFWINTFEXT_IGPU_MINOR<<0))   // FORMAT: 0x00MMmmmm
#define VERSION_IPFWINTF_IGPU     ((VERSION_IPFWINTFEXT_IGPU_GEN<<24) | (VERSION_IPFWINTFEXT_IGPU & 0x00FFFFFF)) // FORMAT: 0xGGMMmmmm

#define IP_IGPU_SIGNATURE  0X5F555047495F5049       // SIGNATURE_64 ('I','P','_','I','G','P','U','_')

#ifndef FORCE_ENABLE
#define FORCE_ENABLE  1
#endif

typedef struct {
  UINT32    Offset;
  UINT32    Value;
} BOOT_SCRIPT_REGISTER_IGPU_SETTING;

typedef enum {
  // IpIGpuXe*
  IpIGpuXe   = 1,
  IpIGpuXe2  = 2,
  IpIGpuXe3  = 3,
  IpIGpuXe3p = 4,
} IP_IGPU_XE_VERSION;

typedef struct _IP_IGPU_INST IP_IGPU_INST;
struct _IP_IGPU_INST {
  IP_CSI_INST_PREFIX           Prefix;

  // IP Wrapper Register Contexts
  IP_WR_REG_CNTXT              PcieCfgAccess;  // IGpu Configspace primary access
  IP_WR_REG_CNTXT              MmioAccess;     // IGpu Register access

  // IP Wrapper Error Contexts
  IP_WR_ERROR_CNTXT            ErrorCntxt;
  IP_WR_ERROR_CNTXT            ErrorCntxtWarning;
  IP_WR_ERROR_CNTXT            ErrorAssert;

  // IP Wrapper Print Contexts
  IP_WR_PRINT_CNTXT            PrintError;
  IP_WR_PRINT_CNTXT            PrintWarning;
  IP_WR_PRINT_CNTXT            PrintLevel1;

  // IP Wrapper Version Specific Contexts
  IP_WR_VSC_CNTXT              IpWrVscCntxt;

  // IP Wrapper Timing Contexts
  IP_WR_TIME_CNTXT             TimeCntxt;

  //
  // Signature to validate the pInst
  // it should be always 0X5F555047495F5049
  //
  UINT64                       Signature;
  //
  // IGPU Product Xe Version
  //
  IP_IGPU_XE_VERSION           XeVersion;

  //
  // These members describe the configuration for the Pre Mememory Phase
  // This can be used in PostMem as well.
  //
  IP_IGPU_PEI_PREMEM_CONFIG    IGpuPreMemConfig;

  //
  // These members describe the configuration for the Post Mememory Phase
  //
  IP_IGPU_PEI_CONFIG           IGpuConfig;

  //
  // These Members used to store the data during the initialization of IGPU
  //
  IGPU_PRIVATE_CONFIG          IGpuPrivateConfig;
};

typedef enum {
  // IpIGpuFeatId<CapName>
  IpIGpuFeatIdUnknown = 0,
} IP_IGPU_FEAT_ID;

typedef enum {
  IpIGpuFeatValDis = 0,

  // <=== NOTE: add new emums to end for backwards compatability
} IP_IGPU_FEAT_VAL;

typedef enum {
  GtRc6CtxLowMemReg = 1,
  MediaRc6CtxMemReg,
  DsmBaseReg,
  GsmBaseReg,
  GgcIGpuSgReg,
  MediaFlatCcsReg,
  DispGsmBaseReg,
  DispDsmBaseReg,
  DispGgcReg,
  MediaPavpcReg,
  DispPavpcReg,
  GtFlatCcsReg,
  GtFlatCcsGamReg,
  MediaFlatCcsGamReg,
} IP_IGPU_REGISTER;

typedef enum {
  GtRc6CtxLowMemBase = 1,
  GtRc6CtxLowMemLock,
} IP_IGPU_RC6CTX_LOW_GT_REGISTER_MODE;

typedef enum {
  MediaRc6CtxMemLock,
  MediaRc6CtxMemBase,
} IP_IGPU_RC6CTX_MEDIA_REGISTER_MODE;

typedef enum {
  DsmBaseAddr,
} IP_IGPU_DSMBASE_REGISTER_MODE;

typedef enum {
  GsmBaseAddr,
} IP_IGPU_GSMBASE_REGISTER_MODE;

typedef enum {
  GgcIGpuSgData,
} IP_IGPU_GGC_IGPU_REGISTER_MODE;

typedef enum {
  MediaFlatCcsBase,
} IP_IGPU_FLATCCSBASEANDRANGE_MEDIA_REGISTER_MODE;

typedef enum {
  DispGsmBaseAddr,
} IP_IGPU_GSMBASE_DISP_REGISTER_MODE;

typedef enum {
  DispDsmBaseAddr,
} IP_IGPU_DSMBASE_DISP_REGISTER_MODE;

typedef enum {
  DispGgcData,
} IP_IGPU_GGC_DISP_REGISTER_MODE;

typedef enum {
  MediaPavpcBase,
  MediaPavpcBaseMB,
  MediaPavpcData,
  MediaPavpcSize,
  MediaPavpcLock,
} IP_IGPU_PAVPC_MEDIA_REGISTER_MODE;

typedef enum {
  DispPavpcBase,
  DispPavpcLock,
} IP_IGPU_PAVPC_DISP_REGISTER_MODE;

typedef enum {
  GtFlatCcsBase,
} IP_IGPU_FLATCCSBASEANDRANGE_GT_REGISTER_MODE;

typedef enum {
  GtFlatCcsEnable,
} IP_IGPU_FLATCCS_GAM_GT_REGISTER_MODE;

typedef enum {
  MediaFlatCcsEnable,
} IP_IGPU_FLATCCS_GAM_MEDIA_REGISTER_MODE;

/**
InstInit for this version of IP

@param[in]  pInst  A pointer to the IP instance to be used.

@retval     IpCsiStsSuccess on success
@retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuInstInit (
  IP_IGPU_INST  *pInst
  );

/**
  Since InstInit is not allowed to access any registers, IpInit is used to write to any registers needed.
  * IpInit() shall be the first API function called after InstInit.
  * this is a good place to fix bad register defaults or work around bugs

  @param[in]  pInst  A pointer to the IP instance to be used.

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuIpInit (
  IP_IGPU_INST  *pInst
  );

/**
  Get version information

  @param[in]  pInst  The instance
  @param[in]  VerId  Version ID

  @retval            0 is returned upon any error
  @retval            return value define by VerId
**/
UINT32
IpIGpuGetVersion (
  IP_IGPU_INST   *pInst,
  IP_CSI_VER_ID  VerId
  );

/**
  Get capability

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP.
  @param[in]      FeatureId  Cap ID
  @param[in, out] pCsiSts    Optional and may be NULL.  If non-NULL return API status

  @retval                    defined by FeatureId
**/
UINT32
IpIGpuGetCapability (
  IP_IGPU_INST     *pInst,
  UINT32           IpTarget,
  IP_IGPU_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  );

/**
  Get status

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP.
  @param[in]      FeatureId  Status ID
  @param[in, out] pCsiSts    Optional and may be NULL.  If non-NULL return API status

  @retval         defined by FeatureId
**/
UINT32
IpIGpuGetStatus (
  IP_IGPU_INST     *pInst,
  UINT32           IpTarget,
  IP_IGPU_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  );

/**
  Get control

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP.
  @param[in]      FeatureId  Control ID
  @param[in, out] pCsiSts    Optional and may be NULL.  If non-NULL return API status

  @retval         defined by FeatureId
**/
UINT32
IpIGpuGetControl (
  IP_IGPU_INST     *pInst,
  UINT32           IpTarget,
  IP_IGPU_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  );

/**
  Set control

  @param[in]  pInst       The instance
  @param[in]  IpTarget    IP defined value for target within the IP.
  @param[in]  FeatureId   Feature ID
  @param[in]  FeatureVal  Value based on FeatureId

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuSetControl (
  IP_IGPU_INST     *pInst,
  UINT32           IpTarget,
  IP_IGPU_FEAT_ID  FeatureId,
  UINT32           FeatureVal
  );

/**
  This function will check if Bus Initiator and Memory access on 0:2:0 is enabled or not

  @retval TRUE          IGD Bus Initiator Access and Memory Space access is Enabled
  @retval FALSE         IGD Bus Initiator Access and Memory Space access is Disable
**/
BOOLEAN
IpIGpuCmdRegEnabled (
  IP_IGPU_INST  *pInst
  );

/**
  This function will check the pInst for IpIGpu is valid or not

  @param[in]  pInst       The instance

  @retval TRUE          PInst is valid
  @retval FALSE         PInst is not valid
**/
BOOLEAN
IpIGpuIsInstValid (
  IP_IGPU_INST  *pInst
  );

/**
  This function will check if Intel Graphics is Enabled or Supported

  @param[in]  pInst       The instance

  @retval TRUE          Graphics Enabled or Supported
  @retval FALSE         Graphics not Enabled or not Supported
**/
BOOLEAN
IpIGpuSupported (
  IP_IGPU_INST  *pInst
  );

/**
  Get GttMmAdr

  @param[in]      pInst      The instance

  @retval         GttMmAdr
**/
UINT64
IpIGpuGetGttMmAdr (
  IP_IGPU_INST  *pInst
  );

/**
  This function will program the GttMmAdr

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuProgramGttMmAdr (
  IP_IGPU_INST  *pInst
  );

/**
  This function will enable Bus Initiator and Memory access on 0:2:0

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuEnableCmdReg (
  IP_IGPU_INST  *pInst
  );

/**
  This function will enable IO Bar on 0:2:0

  @param[in] pInst   - A pointer to the IP instance to be used.
  @retval None
**/
VOID
IpIGpuEnableIoCmdReg (
  IP_IGPU_INST  *pInst
  );

/**
  This function will disable IO Bar on 0:2:0

  @param[in] pInst   - A pointer to the IP instance to be used.
  @retval None
**/
VOID
IpIGpuDisableIoCmdReg (
  IP_IGPU_INST  *pInst
  );

/**
  This function will return the IGPU Vendor ID

  @param[in]  pInst       The instance

  @retval VendorId        IGPU 0 2 0 Vendor ID
**/
UINT16
IpIGpuGetVendorId (
  IP_IGPU_INST  *pInst
  );

/**
  This function will return the IGPU Device ID

  @param[in]  pInst       The instance

  @retval DeviceId        IGPU 0 2 0 Device ID
**/
UINT16
IpIGpuGetDeviceId (
  IP_IGPU_INST  *pInst
  );

/**
  This function will return the IGPU Subsystem Vendor ID

  @param[in]  pInst          The instance

  @retval SubsystemVendorId  IGPU 0 2 0 Subsystem Vendor ID
**/
UINT16
IpIGpuGetSubsystemVendorId (
  IP_IGPU_INST  *pInst
  );

/**
  This function will return the IGPU Subsystem ID

  @param[in]  pInst          The instance

  @retval SubsystemId        IGPU 0 2 0 Subsystem ID
**/
UINT16
IpIGpuGetSubsystemId (
  IP_IGPU_INST  *pInst
  );

/**
  This function will return the IGPU Revision ID

  @param[in]  pInst         The instance

  @retval RevisionId        IGPU 0 2 0 Revision ID
**/
UINT8
IpIGpuGetRevisionId (
  IP_IGPU_INST  *pInst
  );

/**
  Get LMemBar
  @param[in]      pInst      The instance
  @retval         GttMmAdr
**/
UINT64
IpIGpuGetLMemBar (
  IP_IGPU_INST  *pInst
  );

/**
  This function will program the LMemBar

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuProgramLMemBar (
  IP_IGPU_INST  *pInst
  );

/**
  This function will return GttMmAdr Bar Length

  @param[in]  pInst       The instance

  @retval     GttMmAdr Size on success
  @retval     0 on failure
**/
UINT64
IpIGpuGetGttMmAdrSize (
  IP_IGPU_INST  *pInst
  );

/**
  This function will return LMem Bar Length

  @param[in]  pInst       The instance

  @retval     LMemBar Size on success
  @retval     0 on failure
**/
UINT64
IpIGpuGetLMemBarSize (
  IP_IGPU_INST  *pInst
  );

/**
  Program Flat CCS Base Address register in Display.

  @param[in]  pInst               A pointer to the IP instance to be used.

  @return     A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpIGpuProgramFlatCcsBaseForDisplay (
  IP_IGPU_INST  *pInst
  );

/**
  Initialize the Display Memory Subsystem Config registers.

  @param[in]  pInst     A pointer to the IP instance to be used.

  @return     A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpIGpuDisplayMemSsInit (
  IP_IGPU_INST  *pInst
  );

/**
  Check and Force Vdd On based on the H/W Status

  @param[in]  pInst               A pointer to the IP instance to be used.
  @param[in]  BootMode            Current BootMode

  @return         A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpIGpuCheckAndForceVddOn (
  IP_IGPU_INST  *pInst,
  UINT32        BootMode
  );

/**
  This function is to set IGPU Memory map.

  @param[in]  pInst       A pointer to the IP instance to be used.

  @retval                 IpCsiStsSuccess on success
  @retval                 !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuSetMemMap (
  IP_IGPU_INST  *pInst
  );

/**
   Initialize GT of SystemAgent

  @param[in]  pInst             A pointer to the IP instance to be used.

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuGtInit (
  IP_IGPU_INST  *pInst
  );

/**
  Initialize PAVP features.
  @param[in]  pInst       A pointer to the IP instance to be used.

  @retval IpCsiStsSuccess - PAVP initialization complete
  @retval EFI_UNSUPPORTED - IGPU is not present so PAVP not supported
**/
IP_CSI_STATUS
IpIGpuPavpInit (
  IP_IGPU_INST  *pInst
  );

/**
  Function to clear GMS, GGMS and disable IVD and Vemen

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure

**/
IP_CSI_STATUS
IpIGpuAdditionalStepToDisable (
  IP_IGPU_INST  *pInst
  );

/**
  Function to clear temporary Graphics BAR at end of PEI

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure

**/
IP_CSI_STATUS
IpIGpuClearTempBar (
  IP_IGPU_INST  *pInst
  );

/**
  Function to Store the GMD Status in Private Config

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure

**/
IP_CSI_STATUS
IpIGpuSaveGMDStatus (
  IP_IGPU_INST  *pInst
  );

/**
  Initialize Media of IGPU

  @param[in]  pInst       A pointer to the IP instance to be used.

  @return     A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpIGpuMediaInit (
  IP_IGPU_INST  *pInst
  );

/**
  This function is to read the register.
  @param[in] pInst         - A pointer to the IP instance to be used.
  @param[in] DataRequested - Instance of Mode of the Register
  @retval UINT64           - Register Details based on Mode
**/
UINT64
IpIGpuGetRegistersData (
  IP_IGPU_INST      *pInst,
  IP_IGPU_REGISTER  RegisterName,
  UINT16            DataRequested
  );

/**
  Print Graphics PCI space in Debug log.
  @param[in] pInst   - A pointer to the IP instance to be used.
  @retval None
**/
VOID
IpIGpuPrintConfigSpace (
  IP_IGPU_INST  *pInst
  );

/**
  This function will clear command register of 0:2:0

  @param[in] pInst   - A pointer to the IP instance to be used.
  @retval     cmd register
**/
IP_CSI_STATUS
IpIGpuClearCmdReg (
  IP_IGPU_INST  *pInst
  );

/**
  Get L3Bank register status
  @param[in]      pInst      The instance
  @retval         Status of L3BANK lock bit
**/
BOOLEAN
IpIGpuIsL3BankLocked (
  IP_IGPU_INST  *pInst
  );

/**
  Get L3Bank Media register status
  @param[in]      pInst      The instance

  @retval         Status of L3BANK MEDIA lock bit
**/
BOOLEAN
IpIGpuIsL3BankMediaLocked (
  IP_IGPU_INST  *pInst
  );

/**
  Enable Force Wake Up for GT.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuForceWakeupGt (
  IP_IGPU_INST  *pInst
  );

/**
  Enable Idle mode for GT.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuIdleGt (
  IP_IGPU_INST  *pInst
  );

/**
  Enable Force Wake Up for media.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuForceWakeupMedia (
  IP_IGPU_INST  *pInst
  );

/**
  Enable Idle mode for Media.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuIdleMedia (
  IP_IGPU_INST  *pInst
  );

/**
  Print IGPU Register Info.

  @param[in] pInst      - A pointer to the IP instance to be used.
  @param[in] String     - IGPU Register String name if provided.
  @param[in] Register   - IGPU Register offset.
  @param[in] Data       - IGPU Register Value which is written.
  @param[in] Value      - IGPU Register Data after write.
**/
VOID
IpIGpuPrintRegData (
  IP_IGPU_INST  *pInst,
  CHAR8         *String,
  UINT32        Register,
  UINT64        Value,
  UINT64        Data
  );

/**
  Allocate GSM2 Base Address and Size

  @param[in]  pInst  A pointer to the IP instance to be used.
  @param[in]  UINT64 Base Address
  @param[in]  UINT64 Size
**/
VOID
IpIGpuGsm2Allocation (
  IP_IGPU_INST  *pInst,
  UINT64        Gsm2BaseAddress,
  UINT8         Gsm2Size
  );

//
// PRINT_* Macros
//
// Easy PRINT_*() Macros
#if defined (IP_IGPU_PRINT_ALL) || defined (IP_IGPU_PRINT_ERROR)
#define PRINT_ERROR(...)                        \
  do {                                          \
    IpWrPrint (pInst->PrintError, __VA_ARGS__); \
    IpWrError (pInst->ErrorCntxt, IpWrErrorMajorAssert, __LINE__); \
  } while (0)
#define PRINT_ERROR_NO_CNTXT(...)                                                 \
  do {                                                                   \
    IpWrPrint ((IP_WR_PRINT_CNTXT) NULL, __VA_ARGS__);                   \
    IpWrError ((IP_WR_ERROR_CNTXT) NULL, IpWrErrorMajorError, __LINE__); \
  } while (0)
#else
#define PRINT_ERROR(...)                        \
  do {                                          \
    IpWrError (pInst->ErrorCntxt, IpWrErrorMajorAssert, __LINE__); \
  } while (0)
#define PRINT_ERROR_NO_CNTXT(...)                                                 \
  do {                                                                   \
    IpWrError ((IP_WR_ERROR_CNTXT) NULL, IpWrErrorMajorError, __LINE__); \
  } while (0)
#endif

#if defined (IP_IGPU_PRINT_ALL) || defined (IP_IGPU_PRINT_WARNING)
#define PRINT_WARNING(...)                             \
  do {                                                 \
    IpWrPrint (pInst->PrintWarning, __VA_ARGS__);      \
    IpWrError (pInst->ErrorCntxtWarning, 0, __LINE__); \
  } while (0)
#else
#define PRINT_WARNING(...)                             \
  do {                                                 \
    IpWrError (pInst->ErrorCntxtWarning, 0, __LINE__); \
  } while (0)
#endif

#if defined (IP_IGPU_PRINT_ALL) || defined (IP_IGPU_PRINT_LEVEL1)
#define PRINT_LEVEL1(...)  IpWrPrint (pInst->PrintLevel1, __VA_ARGS__)
#else
#define PRINT_LEVEL1(...) \
  do {                    \
  } while (0)
#endif

#endif // __IP_IGPU_H__
