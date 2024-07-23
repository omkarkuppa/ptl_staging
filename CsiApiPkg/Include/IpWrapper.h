/** @file
  IP Wrapper Intefaces defined by Common Services Interface (CSI) Spec

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

#ifndef IP_WRAPPER_H_
#define IP_WRAPPER_H_

  #include "IpCsi.h"
  #include "IpWrapperOptional.h"
  #include "IpWrapperDeprecated.h"

// IMPORTANT:
// - all CSI and Wrapper types must be defined in IpCsi.h.  Function declarations will continue
//   to be defined in the appropriate H files
// - please see comments in IpCsi.h for further explanation

/**
  CSI definition of Error

  @param[in]  pCntxt    IP Wrapper defined error context. Opaque to CSI FW.
  @param[in]  Major     Major error type uses IP_WR_ERROR_MAJOR or IP_IPNAME_ERROR_MAJOR
  @param[in]  Minor     Minor defined by Major. May be IP_WR_ERROR_MINOR, IP_IPNAME_ERROR_MINOR, or UINT32
**/
void
IpWrError (
  IP_WR_ERROR_CNTXT  pCntxt,
  UINT32             Major,
  UINT32             Minor
  );

/**
  CSI definition of Print

  @param[in]  pCntxt    IP Wrapper defined print context. Opaque to CSI FW.
  @param[in]  format    Formated string compatible with C99
  @param[in]  ...       variable length params. NOTE: this will use vargs best practices!
**/
void
IP_CSI_ATTR_VA_ARG
IpWrPrint (
  IP_WR_PRINT_CNTXT  pCntxt,
  const char         *Format,
  ...
  );

/**
  Basic Csi register write.

  @param[in]  pCntxt      IP Wrapper defined register group context. Opaque to CSI FW.
  @param[in]  offset      Byte offset of reg
  @param[in]  data        Data to be written
  @param[in]  RegFlags    OR'ed values of IP_WR_REG_FLAGS. at least "IpWrRegFlagSize*" is required for size

  @return     The data written
**/
UINT64
IpWrRegWrite (
  IP_WR_REG_CNTXT  pCntxt,
  UINT32           Offset,
  UINT64           Data,
  UINT32           RegFlags
  );

/**
  Basic CSI register read.

  @param[in]  pCntxt      IP Wrapper defined register group context. Opaque to CSI FW.
  @param[in]  offset      Byte offset of reg
  @param[in]  RegFlags    OR'ed values of IP_WR_REG_FLAGS. at least "IpWrRegFlagSize*" is required for size

  @return     The data read from the Offset of the provided Context.
**/
UINT64
IpWrRegRead (
  IP_WR_REG_CNTXT  pCntxt,
  UINT32           Offset,
  UINT32           RegFlags
  );

/**
  Polling Register read. This function will continue to read the specified register until
  one of the following criteria is met:
    - The Mask and Match criteria have been met
    - The number of retries is exceeded
  Time Accuracy is limited by the environment.

  @param[in]      pRegCntxt      Used for reg read (see IpWrRegRead)
  @param[in]      Offset         Used for reg read (see IpWrRegRead)
  @param[in]      RegFlags       Flags used for reg read (see IpWrRegRead)
  @param[in]      Mask           Exit polling when ((regRead & Mask) == Match)
  @param[in]      Match          Exit polling when ((regRead & Mask) == Match)
  @param[in]      MaxRetry       # of retry before timeout.  0 loops forever
  @param[in]      pTimeCntxt     Time context for delay
  @param[in]      DelayMicroSec  minimum uSecond delay per retry. Can be 0.
  @param[out]     pApiSts        Optional (ie may be NULL)
                                 on Match, return IpCsiStsSuccess
                                 on timeout, return IpCsiStsErrorTimeout
**/
void
IpWrRegPollUs (
  IP_WR_REG_CNTXT   pRegCntxt,
  UINT32            Offset,
  UINT32            RegFlags,
  UINT64            Mask,
  UINT64            Match,
  UINT32            MaxRetry,
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            DelayMicroSec,
  IP_CSI_STATUS     *pApiSts
  );

/**
  Polling Register read. This function will continue to read the specified register until
  one of the following criteria is met:
    - ((regRead & Mask) == Match1)
    - ((regRead & Mask) == Match2)
    - The number of retries is exceeded
  Time Accuracy is limited by the environment.

  @param[in]      pRegCntxt      Used for reg read (see IpWrRegRead)
  @param[in]      Offset         Used for reg read (see IpWrRegRead)
  @param[in]      RegFlags       Flags used for reg read (see IpWrRegRead)
  @param[in]      Mask           Exit polling when ((regRead & Mask) == Match1) || ((regRead & Mask) == Match2)
  @param[in]      Match1         Exit polling when ((regRead & Mask) == Match1)
  @param[in]      Match2         Exit polling when ((regRead & Mask) == Match2)
  @param[in]      MaxRetry       # of retry before timeout.  0 loops forever
  @param[in]      pTimeCntxt     Time context for delay
  @param[in]      DelayMicroSec  minimum uSecond delay per retry. Can be 0.
  @param[out]     pApiSts        Optional (ie may be NULL)
                                 on Match, return IpCsiStsSuccess
                                 on timeout, return IpCsiStsErrorTimeout
**/
void
IpWrRegPoll2ValUs (
  IP_WR_REG_CNTXT   pRegCntxt,
  UINT32            Offset,
  UINT32            RegFlags,
  UINT64            Mask,
  UINT64            Match1,
  UINT64            Match2,
  UINT32            MaxRetry,
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            DelayMicroSec,
  IP_CSI_STATUS     *pApiSts
  );

/**
  Polling Register read. This function will continue to read the specified register until
  one of the following criteria is met:
    - The Mask and Match criteria have been met
    - The number of retries is exceeded
  Time Accuracy is limited by the environment.

  @param[in]      pRegCntxt      Used for reg read (see IpWrRegRead)
  @param[in]      Offset         Used for reg read (see IpWrRegRead)
  @param[in]      RegFlags       Flags used for reg read (see IpWrRegRead)
  @param[in]      Mask           Exit polling when ((regRead & Mask) == Match)
  @param[in]      Match          Exit polling when ((regRead & Mask) == Match)
  @param[in]      MaxRetry       # of retry before timeout.  0 loops forever
  @param[in]      pTimeCntxt     Time context for Delay
  @param[in]      DelayNanoSec   minimum nanosecond delay per retry. Can be 0.
  @param[out]     pApiSts        Optional (ie may be NULL)
                                 on Match, return IpCsiStsSuccess
                                 on timeout, return IpCsiStsErrorTimeout
**/
void
IpWrRegPollNs (
  IP_WR_REG_CNTXT   pRegCntxt,
  UINT32            Offset,
  UINT32            RegFlags,
  UINT64            Mask,
  UINT64            Match,
  UINT32            MaxRetry,
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            DelayNanoSec,
  IP_CSI_STATUS     *pApiSts
  );

/**
  Polling Register read. This function will continue to read the specified register until
  one of the following criteria is met:
    - ((regRead & Mask) == Match1)
    - ((regRead & Mask) == Match2)
    - The number of retries is exceeded
  Time Accuracy is limited by the environment.

  @param[in]      pRegCntxt      Used for reg read (see IpWrRegRead)
  @param[in]      Offset         Used for reg read (see IpWrRegRead)
  @param[in]      RegFlags       Flags used for reg read (see IpWrRegRead)
  @param[in]      Mask           Exit polling when ((regRead & Mask) == Match1) || ((regRead & Mask) == Match2)
  @param[in]      Match1         Exit polling when ((regRead & Mask) == Match1)
  @param[in]      Match2         Exit polling when ((regRead & Mask) == Match2)
  @param[in]      MaxRetry       # of retry before timeout.  0 loops forever
  @param[in]      pTimeCntxt     Time context for Delay
  @param[in]      DelayNanoSec   minimum nanosecond delay per retry. Can be 0.
  @param[out]     pApiSts        Optional (ie may be NULL)
                                 on Match, return IpCsiStsSuccess
                                 on timeout, return IpCsiStsErrorTimeout
**/
void
IpWrRegPoll2ValNs (
  IP_WR_REG_CNTXT   pRegCntxt,
  UINT32            Offset,
  UINT32            RegFlags,
  UINT64            Mask,
  UINT64            Match1,
  UINT64            Match2,
  UINT32            MaxRetry,
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            DelayNanoSec,
  IP_CSI_STATUS     *pApiSts
  );

/**
  Register access change  (@todo: this needs a better description)

  @param[in]  pCntxt        IP Wrapper defined register group context. Opaque to CSI FW.
  @param[in]  AccessType    Requested access type

  @return     A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpWrSetRegAccess (
  IP_WR_REG_CNTXT   pCntxt,
  IP_WR_REG_ACCESS  AccessType
  );

/**
  Return after a microsecond delay that is at least MinMicroSec in duration.
  May "sleep" if supported
  Time Accuracy is limited by the environment.

  @param[in]  pTimeCntxt     Time context (Opaque to CSI FW)
  @param[in]  MinMicroSec    The minimum number of micro seconds to delay for.
**/
void
IpWrDelayUs (
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            MinMicroSec
  );

/**
  Return after a nanosecond delay that is at least MinNanoSec in duration.
  May "sleep" if supported
  Time Accuracy is limited by the environment.

  @param[in]  pTimeCntxt     Time context (Opaque to CSI FW)
  @param[in]  MinNanoSec     The minimum number of nano seconds to delay for.
**/
void
IpWrDelayNs (
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            MinNanoSec
  );

/**
  Mark/Note/Capture current time and return its opaque value.
  Return value used with IpWrDelayFromTime*(), etc.

  @param[in]      pTimeCntxt     Time context (Opaque to CSI FW)

  @return     current opaque time value (IP_WR_TIME_VALUE)
              units of this value are Opaque to CSI FW
**/
IP_WR_TIME_VALUE
IpWrMarkTime (
  IP_WR_TIME_CNTXT  pTimeCntxt
  );

/**
  Waits for at least NumUs microseconds after Time and then returns. May "sleep" if supported
  Time Accuracy is limited by the environment.

  @param[in]      pTimeCntxt     Time context (Opaque to CSI FW)
  @param[in]      Time           time to start the wait from
  @param[in]      NumUs          minimum # of microseconds to wait after Time
**/
void
IpWrDelayFromTimeUs (
  IP_WR_TIME_CNTXT  pTimeCntxt,
  IP_WR_TIME_VALUE  Time,
  UINT32            NumUs
  );

/**
  Waits for at least NumNs nanoseconds after Time and then returns. May "sleep" if supported
  Time Accuracy is limited by the environment.

  @param[in]      pTimeCntxt     Time context (Opaque to CSI FW)
  @param[in]      Time           time to start the wait from
  @param[in]      NumNs          minimum # of nanoseconds to wait after Time
**/
void
IpWrDelayFromTimeNs (
  IP_WR_TIME_CNTXT  pTimeCntxt,
  IP_WR_TIME_VALUE  Time,
  UINT32            NumNs
  );

/**
  CSI definition of Memset

  @param[in]  pCntxt      IP Wrapper defined memory context. Opaque to CSI FW.
  @param[in]  pMem        The address of the memory to be set.
  @param[in]  Value       The byte value to use.
  @param[in]  NumBytes    Number of bytes to set.

  @return     The address of the memory that was set.
**/
void *
IpWrMemset (
  IP_WR_MEM_CNTXT  pCntxt,
  void             *pMem,
  UINT8            Value,
  UINT32           NumBytes
  );

/**
  CSI definition of Memcpy

  @param[in]  pCntxt      IP Wrapper defined memory context. Opaque to CSI FW.
  @param[in]  pDest       Destination address in memory to copy to.
  @param[in]  pSrc        Source address in memory to copy from.
  @param[in]  NumBytes    Number of bytes to copy.

  @return     The address in memory that NumBytes of pSrc was copied to.
**/
void *
IpWrMemcpy (
  IP_WR_MEM_CNTXT  pCntxt,
  void             *pDest,
  const void       *pSrc,
  UINT32           NumBytes
  );

/**
  CSI definition of Malloc

  @param[in]  pCntxt    IP Wrapper defined memory context. Opaque to CSI FW.
  @param[in]  Size      Number of bytes to allocate.

  @retval     NULL      An error occurred.
  @retval     Non-Zero  Memory was successfully allocated.
**/
void *
IpWrMalloc (
  IP_WR_MEM_CNTXT  pCntxt,
  UINT32           Size
  );

/**
  CSI definition of Free

  @param[in]  pCntxt    IP Wrapper defined memory context. Opaque to CSI FW.
  @param[in]  Ptr       Address in memory to free

**/
void
IpWrFree (
  IP_WR_MEM_CNTXT  pCntxt,
  void             *Ptr
  );

/**
  IpWrapper Version Specific Configuration(VSC) Enabled
  (@todo: add better description of how this vsc check is intended to work.)

  @param[in]  pCntxt    IP Wrapper defined VSC context. Opaque to CSI FW.
  @param[in]  VscId     An Id value that the IP will use to check if vsc is required.

  @retval     TRUE       The caller should apply the vsc for the given VscId.
  @retval     FALSE      The caller should NOT apply the vsc for the given VscId.
**/
BOOLEAN
IpWrVersionSpecificConfigurationEnabled (
  IP_WR_VSC_CNTXT  pCntxt,
  UINT32           VscId
  );

#endif //IP_WRAPPER_H_

#ifdef __cplusplus
}
#endif
