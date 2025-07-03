/** @file
  Header file for Fusa Private Lib Internal functions that
  perform Fusa HW access and CTC generic routines.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _FUSA_PRIVATE_LIB_INTERNAL_H_
#define _FUSA_PRIVATE_LIB_INTERNAL_H_

#include "PeiFusaResultReportingLib.h"

typedef enum {
  McaClear,
  McaParity,
  McaCorrectable,
  McaUncorrectable
} MrcMcaMode;

typedef enum {
  TrafficRead,
  TrafficWrite,
  TrafficReadWrite,
  TrafficWriteRead
} MrcGenTrafficMode;

///MCA Bank numbers
#define MCA_FE      0
#define MCA_DCU     1
#define MCA_TLB     2
#define MCA_MLC     3
#define MCA_PUNIT   4
#define MCA_SNCU    5
#define MCA_IOP     (MCA_SNCU)
#define MCA_ICELAND 6
#define MCA_HBO0    8
#define MCA_HBO1    9
#define MCA_CBO0    10
#define MCA_CBO1    11
#define MCA_CBO2    12
#define MCA_CBO3    13
#define MCA_CBO4    14
#define MCA_CBO5    15

///IP index for the FuSa MSR MSR_PERRINJ_AT_IP
typedef enum {
  CORE = 1,
  IOP_CS0_IOSF_UP = 2,
  IOP_CS1_CMI = 3,
  IOP_CS2_CII = 4,
  IOP_CS4_RCH = 6,
  IOP_CS5_IOSF_DIP = 7,
  IOP_CS6_IOTRK  = 8,
  IOP_CS7_CMI_UP = 9,
  IOP_CS8_P2PD = 10,
  IOP_CS9_MSG_FIFO = 11,
  IOP_CS10_P2PA = 12,
  IOP_CS11_UP_COMPLETION = 13,
  DIP_IOSF_MCMD = 18,
  DIP_UP_CMD_ADDRESS = 19,
  DIP_CMI_CPL_DATA = 21,
  CCF_NCURACU_CR_NCU_IDP_PERRINJ_CTRL = 22,
  CCF_SBO_CR_WRC_PERR_INJ_CTRL = 23,
  CBO_Slice0_Egress_eccerrinj_ctrl = 26,
  CBO_Slice0_Egress_llc_perrinj_ctrl = 27,
  CBO_Slice1_Egress_eccerrinj_ctrl = 30,
  CBO_Slice1_Egress_llc_perrinj_ctrl = 31,
  CBO_Slice2_Egress_eccerrinj_ctrl = 34,
  CBO_Slice2_Egress_llc_perrinj_ctrl = 35,
  CBO_Slice3_Egress_eccerrinj_ctrl = 38,
  CBO_Slice3_Egress_llc_perrinj_ctrl = 39,
  CBO_Slice4_Egress_eccerrinj_ctrl = 42,
  CBO_Slice4_Egress_llc_perrinj_ctrl = 43,
  CBO_Slice5_Egress_eccerrinj_ctrl = 46,
  CBO_Slice5_Egress_llc_perrinj_ctrl = 47,
  CBO_Slice6_Egress_eccerrinj_ctrl = 50,
  CBO_Slice6_Egress_llc_perrinj_ctrl = 51,
  CBO_Slice7_Egress_eccerrinj_ctrl = 54,
  CBO_Slice7_Egress_llc_perrinj_ctrl = 55,
  CBO_Slice8_Egress_eccerrinj_ctrl = 58,
  CBO_Slice8_Egress_llc_perrinj_ctrl = 59,
  CBO_Slice9_Egress_eccerrinj_ctrl = 62,
  CBO_Slice9_Egress_llc_perrinj_ctrl = 63,
  CBO0_INGRESS = 84,
  CBO1_INGRESS,
  CBO2_INGRESS,
  CBO3_INGRESS,
  CBO4_INGRESS,
  CBO5_INGRESS,
  CBO6_INGRESS,
  CBO7_INGRESS,
  CBO_Slice10_Egress_eccerrinj_ctrl = 106,
  CBO_Slice10_Egress_llc_perrinj_ctrl = 107,
  CBO_Slice11_Egress_eccerrinj_ctrl = 110,
  CBO_Slice11_Egress_llc_perrinj_ctrl = 111,
} FUSA_IP_INDEX_NUMBER;

#define FUSA_MAX_CONTROLLER              2
#define FUSA_MAX_CHANNEL                 4

#define MSR_PERRINJ_AT_IP                (0x00000107)
#define MSR_PERRINJ_CTRL                 (0x00000108)

#define MSCOD_MASK 0xFFFF0000ULL
#define MCACOD_MASK 0xFFFFULL
#define MCASTATUS_VALID BIT63
#define R_SA_MCHBAR (0x48)
#define R_SA_DMIBAR  (0x68)
///<"Index" - Tells uCode which CR should be targeted in the uncore. Thread-Scope
//#define MSR_PERRINJ_AT_IP 0x107U
/**
  "Payload" - Data to be written (in the case of WRMSR instruction) or read (in the case of RDMSR instruction)
  to/from uncore CR.  Core-Scope
  */
//#define MSR_PERRINJ_CTRL  0x108U

///Common register definition used by CTC with MCA_IOP usage
#define R_SA_MCHBAR_FUSA_MCA_REPORTING_EN_0_0_0_MCHBAR_IMPH                      0x6F30U
#define B_SA_MCHBAR_FUSA_MCA_REPORTING_EN_0_0_0_MCHBAR_IMPH_MCA_REPORTING_EN     BIT0

#define MAX_BIG_CORE_COUNT   4

/**
  Check if it is a supported CPU for this library

  @note Not being used yet

  @retval TRUE - Supported
  @retval FALSE - Not Supported
**/
BOOLEAN
IsCpuFusaSupported (
  VOID
  );

/**
  Enable MCA on the targetted bank index for error reporting
  purpose and clear the rest. if TargetMcaBankNum is 0xFF
  (outside of the supported MCA bank count), then effectively
  disable all MCA reporting

  @param[in] TargetMcaBankNum - Target MCA bank number
**/
VOID
McaReportingEnable(
  IN UINT32 TargetMcaBankNum
  );

/**
  Read the related MCA bank status and compare against the
  expected McaStatus values (auto check for valid bit)

  @param[in] McaBankNum - MCA bank number
  @param[in] McaStatusMask - Mask value to be applied on
        McaStatus before comparison
  @param[in] ExpectedMcaStatus - Expected McaStatus value after
        masked.

  @retval  FUSA_TEST_PASS - if matches expected values
  @retval  FUSA_TEST_FAIL - if not matches expected values
**/

UINT8
McaBankStatusCheck(
  IN  UINT8  McaBankNum,
  IN  UINT64 McaStatusMask,
  IN  UINT64 ExpectedMcaStatus,
  OUT UINT64 *McaRusultOut
  );

/**
  Clear all MCA bank status
**/
VOID
McaBankStatusClearAll(
  VOID
  );

/**
  Dump all MCA bank status in debug build
**/
VOID
McaBankStatusDumpAll(
  VOID
  );

/**
  Clear the related MCA bank status, address and misc details

  @param[in] McaBankNum - MCA bank number
**/
VOID
McaBankInfoClear(
  IN UINT8 McaBankNum
  );

/**
  Dump the related MCA bank status, address and misc details in
  debug build

  @param[in] McaBankNum - MCA bank number
**/
VOID
McaBankInfoDump(
  IN UINT8 McaBankNum
  );

/**
  Write the parity error injection control payload into the Fusa
  IP indexed by the IpIndex

  @param[in] IpIndex - IP Index for the Fusa MSR for the error
        injection
  @param[in] InjectionControlPayload - Injection control payload
        for the error injection

  @note Once the Fusa MSR is used, the ucode will disable the
        prefetcher for preventing from messing with the CTC
        mechanism. This step will not be undone because it is
        not expected the system to boot to OS without a reset
        for the CTC path.

**/
VOID
FuSaMsrWrite (
  IN UINT32 IpIndex,
  IN UINT32 InjectionControlPayload
  );

/**
  Read the parity error injection control payload status from
  the Fusa IP indexed by the IpIndex

  @param[in] IpIndex - IP Index for the Fusa MSR for the error
        injection

  @retval  Parity error injection control payload status
**/
UINT32
FuSaMsrRead (
  IN UINT32 IpIndex
  );

/**
  Get MCH Base address

  @param  MchBaseAddress - MCH Base address

  @retval  fusaDeviceError - for unexpected address value
  @retval  fusaNoError - no error otherwise
**/
FUSA_LIB_STATUS
MchBarAddressGet (
  OUT UINTN *MchBaseAddress
  );

/**
  Get DMI Bar address

  @param  DmiBaseAddress - DMI Base address

  @retval  fusaDeviceError - for unexpected address value
  @retval  fusaNoError - no error otherwise
**/
FUSA_LIB_STATUS
DmiBarAddressGet (
  OUT UINTN *DmiBaseAddress
  );

/**
  Get GTTMM Base address

  @param  GttMmBaseAdr - GTTMM Base address

  @retval  fusaDeviceError - for unexpected address value
  @retval  fusaNoError - no error otherwise
**/
FUSA_LIB_STATUS
GttMmBarAddressGet (
  OUT UINTN *GttMmBaseAdr
  );

/**
  Get GM Base address

  @param  GmBaseAdr - GM Base address

  @retval  fusaDeviceError - for unexpected address value
  @retval  fusaNoError - no error otherwise
**/
FUSA_LIB_STATUS
GmBarAddressGet (
  OUT UINTN *GmBaseAdr
  );

/**
  Get PCH Device D31 F0 Base address

  @param  PchD31F0BaseAddress - PCH Device D31 F0 Base address

  @note   VOID return because no error possible
**/
VOID
PchD31F0BaseAddressGet (
  OUT UINTN *PchD31F0BaseAddress
  );

/**
 * Function pointer for the generic injection test traffic
 * routine
 */
//typedef VOID (* const FUSA_TRAFFIC_FUNC)( VOID );

/**
 * Function pointer for the generic injection test traffic
 * routine
 */
typedef UINT32 (* const FUSA_INJECTION_TRAFFIC_FUNC)( UINT32 IpIndex, UINT32 InjectionPayload);

/**
  Generic error injection flow using Fusa MSR

  @param[in] IpIndexInput - IP Index for the Fusa MSR for the
        error injection
  @param[in] InjectionPayload - Injection payload for the error
      injection
  @param[in] InjectionAndTraffic - function pointer for
        performing injection and related traffic
  @param[in] McaBankNum - The MCA Bank that reflect the test
        status
  @param[in] McaStatusMask - MCA status mask to be applied
        before comparing for the ExpectedMcaStatus
  @param[in] ExpectedMcaStatus - Expected MCA status value
  @param[in] CheckNum - check number assigned to this check
  @param[in,out] pFusaTestResult - pointer to the test result
        buffer for result to be updated
  @param[in] DebugFlag - to control the debug print. Main
        purpose is to allow AP to avoid the debug print that
        would cause hang.

  @retval  lib routine calling status from UpdateResults

  @note - combine injection and traffic in one routine so that
        there is not done through function calling and return in
        which the error itself may mess up with function return
        or calling.
**/
FUSA_LIB_STATUS
FusaMsrCtcTestGeneric (
  IN UINT32                       IpIndexInput,
  IN UINT32                       InjectionPayload,
  IN FUSA_INJECTION_TRAFFIC_FUNC  InjectionAndTraffic,
  IN UINT8                        McaBankNum,
  IN UINT64                       McaStatusMask,
  IN UINT64                       ExpectedMcaStatus,
  IN UINT8                        CheckNum,
  IN OUT FUSA_TEST_RESULT         *pFusaTestResult,
  IN BOOLEAN                      DebugFlag
  );

#endif // _FUSA_PRIVATE_LIB_INTERNAL_H_
