/** @file
  IP Common Services Interface (CSI)

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IP_CSI_H_
#define IP_CSI_H_

// NOTE:IpWrapperBaseTypes.h is provided by user of CSI to match their environment
  #include <IpWrapperBaseTypes.h>

// IpUpiGetVersion(IpCsiVerIdCsi)

#define _IP_CSI_VERSION_MAJOR  0x0001
#define _IP_CSI_VERSION_MINOR  0x0029

#define IP_CSI_VERSION  ((_IP_CSI_VERSION_MAJOR<<16) | _IP_CSI_VERSION_MINOR)// 0xMMMMmmmm, CSI-SemVer=MMMM.mmmm.0

  #ifndef IP_CSI_ATTR_VA_ARG
// For VA_ARGS, some compilers/environments may need special "__attribute__" before function
// Example:
//  #define IP_CSI_ATTR_VA_ARG __attribute__((ms_abi))
//  void IP_CSI_ATTR_VA_ARG IpWrPrint (IP_WR_PRINT_CNTXT  pCntxt, const char *Format, ...);
#define IP_CSI_ATTR_VA_ARG // NOOP unless defined otherwise
  #endif

// IMPORTANT:
// - all CSI and Wrapper types must be defined in IpCsi.h.  Function declarations will continue
//   to be defined in the appropriate H files
// - inclusion of ALL types in this H file allows the IP's to use any CSI/Wrapper type without
//   breaking the BIOS build procedure

/// IP CSI Status
typedef enum {
  IpCsiStsMin     = 0,   ///< must be first in the range "IpCsiSts*"
  IpCsiStsSuccess = 0,   ///< Success

  IpCsiStsErrorMin,                   ///< must be first in the range "IpCsiStsError*"
  IpCsiStsError = IpCsiStsErrorMin,   ///< Error (see SDAS/API for more details)
  IpCsiStsErrorBadParam,              ///< Error bad parameter (see SDAS/API for more details)
  IpCsiStsErrorUnknownId,             ///< Error unknown ID (see SDAS/API for more details)
  IpCsiStsErrorTimeout,               ///< Error timeout (see SDAS/API for more details)
  IpCsiStsErrorNullPtr,               ///< Error NULL pointer (see SDAS/API for more details)
  IpCsiStsErrorNotAllowed,            ///< Error not allowed (see SDAS/API for more details)
  IpCsiStsErrorInsufficientResources, ///< Error Insufficient Resources (see SDAS/API for more details)
  IpCsiStsInProgress,                 ///< Operation in progress (see SDAS/API for more details)
  IpCsiStsNotReady,                   ///< Operation not ready (see SDAS/API for more details)
  IpCsiStsErrorFips,                  ///< FIPS Error (see SDAS/API for more details)

  // <=== add new ones here for compatibility

  IpCsiStsErrorIpCustomMin = 0x10000000,  ///< must be first in the range  "IpCsiStsErrorIpCustom*"
  // <=== IpIpname.h may IP Specific errors to this range to IP_IPNAME_CSI_ERROR_STATUS
  IpCsiStsErrorIpCustomMax = 0x1FFFFFFF,  ///< must be last in the range "IpCsiStsErrorIpCustom*"

  IpCsiStsErrorMax = IpCsiStsErrorIpCustomMax, ///< must be last in the range "IpCsiStsError*"
  IpCsiStsMax      = IpCsiStsErrorIpCustomMax, ///< must be last in the range "IpCsiSts*"
} IP_CSI_STATUS;

/// IP CSI Version ID
typedef enum {
  IpCsiVerIdMin  = 0,       ///< must be first
  IpCsiVerIdHwIp = 0,       ///< HW IP
  IpCsiVerIdIpFwIntf,       ///< IPSD FW interface
  IpCsiVerIdIpFw,           ///< IPSD FW
  IpCsiVerIdCsi,            ///< CSI
  IpCsiVerIdHwIpCustom,     ///< HW IP Custom
  IpCsiVerIdIpFwIntfExt,    ///< IPSD FW Interface Extension
  IpCsiVerIdS3dFwIntf,      ///< S3D FW interface
  IpCsiVerIdS3dFw,          ///< S3D IP FW
  IpCsiVerIdS3dFwIntfExt,   ///< S3D FW Interface Extension
  IpCsiVerIdSdlibFw,        ///< SDLib Firmware Implementation
  IpCsiVerIdSdlibFwIntfExt, ///< SDLib Firmware Interface SemVer

  // <=== add new ones here for compatibility
  IpCsiVerIdMax ///< this must be last
} IP_CSI_VER_ID;

/// IP CSI TUNING PARAMETER ENTRY
typedef struct {
  UINT32    ParamId;         ///< use IP_CSI_TUNING_PARAM_ID or IP_<IpName>_TUNING_PARAM_ID
  UINT32    ParamData;       ///< format defined by ParamId
} IP_CSI_TUNING_PARAM_ENTRY;

/// IP CSI TUNING PARAMETER ID
typedef enum {
  IpCsiTuningParamIdIpMin = 0,
  // <=== IpIpname.h defined new IP_<IpName>_TUNING_PARAMID in this range
  IpCsiTuningParamIdIpMax = 0x3FFFFFFF,

  // standard CSI enums start here
  IpCsiTuningParamIdCommonMin = 0x40000000, ///< start of common CSI ParamID. Not used & Treated as NOOP
  IpCsiTuningParamIdNoop,                   ///< do nothing
  IpCsiTuningParamIdReleaseInfo,            ///< should be first >=1 entries in list. Contains tuning team release info. Printed upon execution
  IpCsiTuningParamIdAscii,                  ///< Print 4 bytes of ASCII in ParamData upon execution.
                                            ///<  - (ParamData & 0xFF) printed first
                                            ///<  - ((ParamData>>24) & 0xFF) printed last
                                            ///<  - ASCII=0 is not printed.
  // <=== add new ones here for compatibility
  IpCsiTuningParamIdCommonMax,

  // DEPRECATED and kept for compatibility
  IpCsiTuningParamIdIpStart     = IpCsiTuningParamIdIpMin,     // DEPRECATED: use new name
  IpCsiTuningParamIdCommonStart = IpCsiTuningParamIdCommonMin, // DEPRECATED: use new name
} IP_CSI_TUNING_PARAM_ID;

// These Context's are declared here so that IPFW can compile, but need
// to be defined by the IpWrapper code (IPWR) as they are Opaque to the IPFW.
  #ifndef IP_WR_OPAQUE_DATA
#define IP_WR_OPAQUE_DATA  void*// meaning Opaque to CSI FW
  #endif
typedef IP_WR_OPAQUE_DATA  IP_WR_REG_CNTXT;
typedef IP_WR_OPAQUE_DATA  IP_WR_PRINT_CNTXT;
typedef IP_WR_OPAQUE_DATA  IP_WR_ERROR_CNTXT;
typedef IP_WR_OPAQUE_DATA  IP_WR_MEM_CNTXT;
typedef IP_WR_OPAQUE_DATA  IP_WR_TIME_CNTXT;
typedef IP_WR_OPAQUE_DATA  IP_WR_VSC_CNTXT;
typedef IP_WR_OPAQUE_DATA  IP_WR_DFX_CNTXT;

  #ifndef IP_WR_TIME_VALUE
#define IP_WR_TIME_VALUE  UINT32// units/meaning Opaque to CSI FW
  #endif

/// IP WR REGISTER ACCESS
typedef enum {
  IpWrRegAccessMin     = 0, ///< first in range
  IpWrRegAccessDefault = 0, ///< Default
  IpWrRegAccessCfg,         ///< Config
  IpWrRegAccessSb,          ///< Side band
  IpWrRegAccessMem,         ///< MMIO

  // <=== add new ones here for compatibility
  IpWrRegAccessMax, ///< last in range
} IP_WR_REG_ACCESS;

/// IP WR REGISTER FLAGS
typedef enum {
  IpWrRegFlagSize8Bits      = 8,         ///< 8 bits
  IpWrRegFlagSize16Bits     = 16,        ///< 16 bits
  IpWrRegFlagSize32Bits     = 32,        ///< 32 bits
  IpWrRegFlagSize64Bits     = 64,        ///< 64 bits
  IpWrRegFlagSizeMask       = 0xFF,      ///< size mask IpWrRegFlagSize*
  IpWrRegFlagForceCached    = (1 << 8),  ///< Bits[8]=1 force cached
  IpWrRegFlagForceNonCached = (1 << 9),  ///< Bits[9]=1 force non-cached
  IpWrRegFlagForcePosted    = (1 << 10), ///< Bits[10]=1 force posted
  IpWrRegFlagForceNonPosted = (1 << 11), ///< Bits[11]=1 force non-posted
  IpWrRegFlagOrderMask      = (3 << 12), ///< Bits[13:12] order mask IpWrRegFlagOrder*
  IpWrRegFlagOrderHighToLow = (1 << 12), ///< Bits[13:12]=01 order hight-to-low
  IpWrRegFlagOrderLowToHigh = (2 << 12), ///< Bits[13:12]=10 order low-to-high
  IpWrRegFlagOrderAny       = IpWrRegFlagOrderLowToHigh,
  IpWrRegFlagOrderUndefined = (0 << 12)  ///< Bits[13:12]=00 order undefined

                              // <=== add new ones here for compatibility
} IP_WR_REG_FLAGS;

/// IP WR ERROR MAJOR
typedef enum {
  IpWrErrorMajorIpMin = 0,     ///< IPs start IP specific major errors at 0 in IP_IPNAME_ERROR_MAJOR
  // <== IpIpname.h defined more in IP_IPNAME_ERROR_MAJOR
  IpWrErrorMajorIpMax = 9999,

  IpWrErrorMajorWrMin = 10000, ///< starting range for common CSI errors
  IpWrErrorMajorError,         ///< generic Error. Minor = line number
  IpWrErrorMajorAssert,        ///< assert failure. Minor = line number
  IpWrErrorMajorNullPtr,       ///< Null pointer. Minor = line number
  IpWrErrorMajorBadValue,      ///< Bad value. Minor = line number
  IpWrErrorMajorTimeout,       ///< Timeout. Minor = timer (see IP_IPNAME_ERROR_MINOR)
  IpWrErrorMajorInfoLine,      ///< Minor = previous error line number (not required, but nice to have)
  IpWrErrorMajorInfoData,      ///< Minor = previous error data (not required, but nice to have)
  IpWrErrorMajorInfoFile,      ///< Minor = previous error file ID defined in IP_IPNAME_ERROR_MINOR (not required, but nice to have)
  IpWrErrorMajorInfoPhase,     ///< Minor = previous error phase defined in IP_IPNAME_ERROR_MINOR (not required, but nice to have)
  IpWrErrorMajorCsiStatus,     ///< Minor = IP_CSI_STATUS

  // <=== add new ones here for compatibility

  IpWrErrorMajorWrMax,

  // DEPRECATED
  IpWrErrorMajorIpStart = IpWrErrorMajorIpMin, // DEPRECATED: use new name
  IpWrErrorMajorWrStart = IpWrErrorMajorWrMin, // DEPRECATED: use new name
} IP_WR_ERROR_MAJOR;

/// this is the 8 bit SAI value used in future reg access. This is NOT the 5 bit SAI index.
typedef enum {
  IpWrDfxSaiInvalid = 0, ///< invalid SAI value
  IpWrDfxSaiOS      = 1, ///< 5Bit index 0
  IpWrDfxSaiUCode   = 3, ///< 5Bit index 1
  IpWrDfxSaiSmm     = 5, ///< 5Bit index 2
  IpWrDfxSaiSunPass = 7, ///< 5Bit index 3
  IpWrDfxSaiBoot    = 9  ///< 5Bit index 4
} IP_WR_DFX_SAI;

/// IP WR ERROR MINOR
typedef enum {
  IpWrErrorMinorNa = 0  ///< minor is NA

                     // <=== add new ones here for compatibility
} IP_WR_ERROR_MINOR;

/// IP CSI INSTANCE PREFIX
typedef struct {
  UINT64               _RegMacroTemp64; ///< used by REG_* macros inIpCsiReg.h
                                        ///< do not move or re-order members. This forces next pName pointer to be 64bit aligned
  char                 *pName;          ///< Optional string name
                                        ///< do not move or re-order members. BIOS wants this to take up 64bits in 64/32bit mode
  IP_WR_OPAQUE_DATA    pWrapperFwUsage; ///< optional IP Wrapper usage
                                        ///< do not move or re-order members. This forces previous pName pointer to be padded to 64bits
} IP_CSI_INST_PREFIX;

typedef struct {
  UINT32    InstId;       // Optional ID# opaque to S3D
} S3D_CSI_INST_PREFIX;

// DEPRECATED ITEMS kept for compatibility
#define VERSION_CSI  IP_CSI_VERSION// DEPRECATED: use IP_CSI_VERSION instead of VERSION_CSI

#endif //IP_CSI_H_

#ifdef __cplusplus
}
#endif
