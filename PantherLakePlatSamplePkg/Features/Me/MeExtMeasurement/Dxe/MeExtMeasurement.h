/** @file
  Header file for CSME Extended Measurement library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _DXE_ME_EXT_MEASUREMENT_H_
#define _DXE_ME_EXT_MEASUREMENT_H_

#include <Uefi.h>
#include <IndustryStandard/UefiTcgPlatform.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciLib.h>
#include <Library/PciSegmentLib.h>
#include <Protocol/Tcg2Protocol.h>
#include <Library/HobLib.h>
#include <ConfigBlock.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/TpmMeasurementLib.h>
#include <Library/DxeMeLib.h>
#include <Library/MeInfoLib.h>
#include <MkhiMsgs.h>
#include <Register/PchRegs.h>
#include <Register/HeciRegs.h>
#include <MeBiosPayloadData.h>
#include <MeBiosPayloadHob.h>
#include <AmtConfig.h>
#include <Library/IoLib.h>
#include <PcieRegs.h>
#include <Library/SpiAccessLib.h>
#include <Library/PchInfoLib.h>
#include <PchResetPlatformSpecific.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/TimerLib.h>
#include <SetupVariable.h>

#define MAX_EVENTLOG_SIZE              4096
#define SIG_LENGTH                     20
#define HER_REG_COUNT_SHA384           12   // HER1-HER12
#define HER_REG_COUNT_SHA256           8    // HER1-HER8
#define CSME_INFO_VERSION              0x00000001
#define B_ME_DSP                       (BIT16 | BIT17 | BIT18 | BIT19)   // Bits16:19 DFX Secure bus policy value (DSP)
#define ERC_POLL_TIMEOUT               100

///
/// CSME Measurement Event in TPM Log
///
typedef struct {
  UINT8                    Signature[SIG_LENGTH];
  UINT32                   ERHashAlgorithm;
  UINT8                    Events[MAX_EVENTLOG_SIZE];
} CSME_MEASUREMENT_EVENT;

typedef struct {
  UINT32      HwRot                   : 1;  //< Bit 0 - HW_ROT
  UINT32      InvalidState            : 1;  //< Bit 1 - Invalid_State
  UINT32      UntrustedMeasurement    : 1;  //< Bit 2 - Untrusted_Measurement
  UINT32      InvalidMeasurement      : 1;  //< BIT 3 - Invalid_Measurement
  UINT32      LogUnavailable          : 1;  //< BIT 4 - Log_Unavailable
  UINT32      FdoInvalidMeasurement   : 1;  //< Bit 5 - FDO_Invalid_Measurement
  UINT32      Reserved                : 26; //< Bit 6-31 - Reserved
} CSME_INFO_FLAGS;

///
/// CSME Info Measurement Event in TPM Log
///
typedef struct {
  UINT8                    Signature[SIG_LENGTH];
  UINT32                   Version;
  UINT16                   VendorID;
  UINT16                   DeviceID;
  CSME_INFO_FLAGS          Flags;
} CSME_INFO_MEASUREMENT;

/**
  Ready To Boot Event call back function performs getting AMT Config data from CSME and extending to TPM-PCR[1]

  @param[in] Event         The event that triggered this notification function
  @param[in] Context       Pointer to the notification functions context
**/
VOID
EFIAPI
AmtConfigReadyToBootEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

#endif // _DXE_ME_EXT_MEASUREMENT_H_
