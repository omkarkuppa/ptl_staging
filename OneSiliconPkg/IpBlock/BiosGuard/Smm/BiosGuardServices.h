/** @file
  Header file for BIOS Guard Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#ifndef _BIOSGUARD_SERVICES_H_
#define _BIOSGUARD_SERVICES_H_

#include <DprInfoHob.h>
#include <BiosGuard.h>
#include <Protocol/BiosGuard.h>
#include <Protocol/BiosGuardNvsArea.h>
#include <Protocol/CpuNvsArea.h>
#include <Protocol/SmmIoTrapDispatch2.h>

UINT8                mBiosGuardUpdateData[BGUP_TSEG_BUFFER_SIZE];
static UINT16        mBiosGuardUpdateCounter;
BGUP                 *mBiosGuardUpdatePackagePtr;
EFI_PHYSICAL_ADDRESS mBgupCertificate;
BIOSGUARD_LOG        *mBiosGuardLogPtr;
BIOSGUARD_LOG        mBiosGuardLogTemp;
BGUP_HEADER          mBiosGuardBgupHeaderTemp;
EFI_PHYSICAL_ADDRESS mBiosGuardMemAddress;
UINT32               mBiosGuardMemSize;
UINT64               mBiosGuardFullStatus;
UINT8                mBiosGuardUpdatePackageInTseg;

///
/// Private data structure definitions for the driver
///
#define BIOSGUARD_SIGNATURE  SIGNATURE_32 ('B', 'G', 'R', 'D')

#define BIOSGUARD_DIRECTORY_MAX_SIZE               6
#define BIOSGUARD_DIRECTORY_BIOSGUARD_MODULE_ENTRY 0x00
#define BIOSGUARD_DIRECTORY_BGPDT_ENTRY            0x01
#define BIOSGUARD_DIRECTORY_BGUP_ENTRY             0x02
#define BIOSGUARD_DIRECTORY_BGUP_CERTIFICATE_ENTRY 0x03
#define BIOSGUARD_DIRECTORY_BIOSGUARD_LOG_ENTRY    0x04
#define BIOSGUARD_DIRECTORY_UNDEFINED_ENTRY        0xFE
#define BIOSGUARD_DIRECTORY_END_MARKER             0xFF

#define BIOSGUARD_STATUS_STRINGS_NUMBER            21

///
/// Stall period in microseconds
///
#define BIOSGUARD_WAIT_PERIOD                      1
#define BIOSGUARD_AP_SAFE_RETRY_LIMIT              10000

#define BIOSGUARD_MODULE_EXECUTE_WAIT              0
#define BIOSGUARD_MODULE_EXECUTE_TRIGGER           1
#define BIOSGUARD_MODULE_EXECUTE_ABORT             2

#define N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_STATUS_OFFSET                 0
#define V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_STATUS_MASK                   0x000000000000FFFF
#define N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_DATA_OFFSET                   16
#define V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_DATA_MASK                     0x000000000000FFFF
#define N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_TERMINAL_OFFSET               32
#define V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_TERMINAL_MASK                 0x000000000000FFFF
#define B_MSR_PLAT_FRMW_PROT_TRIG_PARAM_SE                            BIT62
#define N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_SE_OFFSET                     62

typedef enum {
  EnumBiosGuardModule        = 0,
  EnumBgpdt,
  EnumBgup,
  EnumBgupCertificate,
  EnumBiosGuardLog,
  EnumBiosGuardDirectoryEnd
} BIOSGUARD_DIRECTORY;

typedef struct {
  UINTN                Signature;
  EFI_HANDLE           Handle;
  BIOSGUARD_PROTOCOL   BiosGuardProtocol;
  EFI_PHYSICAL_ADDRESS BiosGuardDirectory[BIOSGUARD_DIRECTORY_MAX_SIZE];
  UINT64               MsrValue;
} BIOSGUARD_INSTANCE;

typedef struct{
  UINT16  BiosGuardStatus;
  UINT16  BiosGuardAdditionalData;
  UINT16  BiosGuardTerminalLine;
  UINT8   BiosGuardSE;
} BIOSGUARD_INVOCATION_RESULT;

#define BIOSGUARD_INSTANCE_FROM_BIOSGUARDPROTOCOL(a)  CR (a, BIOSGUARD_INSTANCE, BiosGuardProtocol, BIOSGUARD_SIGNATURE)
#define BIOSGUARD_BUFFER_SIZE_WITH_NEW_RECORDS(a)             ((BGUP_TSEG_BUFFER_SIZE / 8) - a - 1)

/**
  Display condition associated with  error code returned by the module

  @param[in] BiosGuardStatus  Error code returned by the module
  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchStringToStatusCode (
  IN UINT16 BiosGuardStatus,
  IN UINT16 BiosGuardAdditionalData
  );

/**
  Print Bios Guard invocation result from dedicated structure

  @param[in] BiosGuardInvocationResult  Pointer to the BIOSGUARD_INVOCATION_RESULT structure.
**/
VOID
EFIAPI
BiosGuardPrintInvocationResult (
  IN BIOSGUARD_INVOCATION_RESULT *BiosGuardInvocationResult
  );

/**
  Parse Bios Guard invocation result from mailbox MSR 0x115 to dedicated structure

  @param[in] BiosGuardInstance               Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in, out] BiosGuardInvocationResult  Pointer to the BIOSGUARD_INVOCATION_RESULT structure.
**/
VOID
EFIAPI
BiosGuardParseInvocationResult (
  IN BIOSGUARD_INSTANCE *BiosGuardInstance,
  IN BIOSGUARD_INVOCATION_RESULT *BiosGuardInvocationResult
  );

/**
  Match EFI status to error code returned by the module

  @param[in] BiosGuardStatus  Error code returned by the module

  @retval EFI_SUCCESS            Successfully completed flash operation.
  @retval EFI_INVALID_PARAMETER  The parameters specified are not valid.
  @retval EFI_UNSUPPORTED        The CPU or SPI memory is not supported.
  @retval EFI_DEVICE_ERROR       Device error, command aborts abnormally.
**/

EFI_STATUS
EFIAPI
BiosGuardEvaluateStatusCode (
  IN UINT16 BiosGuardStatus
  );

/**
  Initialize BIOS Guard protocol instance.

  @param[in] BiosGuardInstance  Pointer to BiosGuardInstance to initialize

  @retval EFI_SUCCESS           The protocol instance was properly initialized
  @retval EFI_NOT_FOUND         BIOS Guard Binary module was not found.
  @retval EFI_OUT_OF_RESOURCES  Allocated memory could not be freed.
  @retval EFI_INVALID_PARAMETER Invalid BIOS Guard Binary module buffer size
**/
EFI_STATUS
BiosGuardProtocolConstructor (
  BIOSGUARD_INSTANCE *BiosGuardInstance
  );

/**
  This service invokes the BIOS Guard Binary.
  All AP threads are present and released by BSP to trigger MSR 0x116.

  Set MSR 0x115 with BIOS Guard DIRECTORY Address.
  Trigger MSR 0x116 to invoke BIOS Guard Binary.
  Read MSR 0x115 to get BIOS Guard Binary Status.

  @param[in] BiosGuardInstance              Pointer to BiosGuardInstance to initialize
**/
VOID
EFIAPI
BiosGuardModuleExecuteBsp (
  IN VOID          *BiosGuardInstance
  );

/**
  This service invokes the BIOS Guard Binary.
  All AP threads are present and released by BSP to trigger MSR 0x116.

  Set MSR 0x115 with BIOS Guard DIRECTORY Address.
  Trigger MSR 0x116 to invoke BIOS Guard Binary.
  Read MSR 0x115 to get BIOS Guard Binary Status.

  @param[in] BiosGuardInstance              Pointer to BiosGuardInstance to initialize
**/
VOID
EFIAPI
BiosGuardModuleExecuteAp (
  IN VOID          *BiosGuardInstance
  );

/**
  This service will write BIOSGUARD_DIRECTORY MSR and invoke the BIOS Guard Module by writing to PLAT_FRMW_PROT_TRIGGER MSR for writing/erasing to flash.
  BIOS should invoke BIOSGUARD_PROTOCOL.Write() or BIOSGUARD_PROTOCOL.Erase() function prior to calling BIOSGUARD_PROTOCOL.Execute() for flash writes/erases (except for BiosUpdate).
  Write()/Erase() function will render BIOS Guard script during execution.
  Execute() function will implement the following steps:
  1. Update BIOS Guard directory with address of BGUP.
  2. All the AP's except the BSP thread are put to sleep.
  3. BIOS Guard module is invoked from BSP to execute desired operation.
  If BiosUpdate flag is set to true, BGUP (BGUP Header + BIOS Guard Script + Update data) is part of data that is passed to SMI Handler. SMI Handler invokes BIOS Guard module to process the update.
  This function would be called by runtime driver, please do not use any MMIO macro here.

  @param[in] This                Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in] BiosUpdate          Flag to indicate flash update is requested by the Tool

  @retval EFI_SUCCESS            Successfully completed flash operation.
  @retval EFI_INVALID_PARAMETER  The parameters specified are not valid.
  @retval EFI_UNSUPPORTED        The CPU or SPI memory is not supported.
  @retval EFI_DEVICE_ERROR       Device error, command aborts abnormally.
**/
EFI_STATUS
EFIAPI
BiosGuardProtocolExecute (
  IN BIOSGUARD_PROTOCOL *This,
  IN BOOLEAN            BiosUpdate
  );

/**
  This service fills BIOS Guard script buffer for flash writes.
  BIOS should invoke this function prior to calling BIOSGUARD_PROTOCOL.Execute() with all the relevant data required for flash write.
  This function will not invoke BIOS Guard Module, only create script required for writing to flash.
  This function would be called by runtime driver, please do not use any MMIO macro here.

  @param[in] This             Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in] Offset           This value specifies the offset from the start of the SPI Flash component.
  @param[in] DataByteCount    Number of bytes in the data portion.
  @param[in] Buffer           Pointer to caller-allocated buffer containing the data sent.

  @retval EFI_SUCCESS         Successfully filled the BIOS Guard script buffer.
  @retval EFI_BAD_BUFFER_SIZE DataSectionSize in BGUP header exceeds the size of BIOS Guard script buffer
**/
EFI_STATUS
EFIAPI
BiosGuardProtocolWrite (
  IN BIOSGUARD_PROTOCOL *This,
  IN UINTN              Offset,
  IN UINT32             DataByteCount,
  IN OUT UINT8          *Buffer
  );

/**
  This service fills BIOS Guard script buffer for erasing blocks in flash.
  BIOS should invoke this function prior to calling BIOSGUARD_PROTOCOL.Execute() with all the relevant data required for flash erase.
  This function will not invoke BIOS Guard module, only create script required for erasing each block in the flash.
  This function would be called by runtime driver, please do not use any MMIO macro here.

  @param[in] This     Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in] Offset   This value specifies the offset from the start of the SPI Flash component.
**/
VOID
EFIAPI
BiosGuardProtocolBlockErase (
  IN BIOSGUARD_PROTOCOL *This,
  IN UINTN              Offset
  );

#endif
