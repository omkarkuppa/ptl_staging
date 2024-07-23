/** @file
  This file defines the BIOS Guard Protocol which implements the
  Intel(R) BIOS Guard Host Controller Compatibility Interface.

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
#ifndef _BIOSGUARD_H_
#define _BIOSGUARD_H_

///
/// Extern the GUID for protocol users.
///
extern EFI_GUID gSmmBiosGuardProtocolGuid;

/**
  Forward reference of BIOS Guard Protocol for ANSI C compatibility
**/
typedef struct _BIOSGUARD_PROTOCOL BIOSGUARD_PROTOCOL;

/**
  This service will write BIOSGUARD_DIRECTORY MSR and invoke the BIOS Guard Module by writing to PLAT_FRMW_PROT_TRIGGER MSR for writing/erasing to flash.
  BIOS should invoke BIOSGUARD_PROTOCOL.Write() or BIOSGUARD_PROTOCOL.Erase() function prior to calling BIOSGUARD_PROTOCOL.Execute() for flash writes/erases (except for BiosUpdate).
  Write()/Erase() function will render BIOS Guard script during execution.
  Execute() function will implement the following steps:
  - 1. Update BIOS Guard directory with address of BGUP.
  - 2. All the AP's except the BSP thread are put to sleep.
  - 3. BIOS Guard module is invoked from BSP to execute desired operation.

  If BiosUpdate flag is set to true, BGUP (BGUP Header + BIOS Guard Script + Update data) is part of data that is passed to SMI Handler. SMI Handler invokes BIOS Guard module to process the update.
  This function would be called by runtime driver, please do not use any MMIO macro here.

  @param[in] This                Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in] BiosUpdate          Flag to indicate flash update is requested by the Tool

  @retval EFI_SUCCESS            Successfully completed flash operation.
  @retval EFI_INVALID_PARAMETER  The parameters specified are not valid.
  @retval EFI_UNSUPPORTED        The CPU or SPI memory is not supported.
  @retval EFI_DEVICE_ERROR       Device error, command aborts abnormally.
**/
typedef
EFI_STATUS
(EFIAPI *BIOSGUARD_EXECUTE)(
  IN BIOSGUARD_PROTOCOL *This,
  IN BOOLEAN            BiosUpdate
  );

/**
  This service fills BIOS Guard script buffer for flash writes.
  BIOS should invoke this function prior to calling BIOSGUARD_PROTOCOL.Execute() with all the relevant data required for flash write.
  This function will not invoke BIOS Guard Module, only create script required for writing to flash.
  @warning This function would be called by runtime driver, please do not use any MMIO macro here.

  @param[in] This             Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in] Address          This value specifies the offset from the start of the SPI Flash component where BIOS Image is located.
  @param[in] DataByteCount    Number of bytes in the data portion.
  @param[in] Buffer           Pointer to caller-allocated buffer containing the data sent.

  @retval EFI_SUCCESS         Successfully filled the BIOS Guard script buffer.
  @retval EFI_BAD_BUFFER_SIZE DataSectionSize in BGUP header exceeds the size of BIOS Guard script buffer
**/
typedef
EFI_STATUS
(EFIAPI *BIOSGUARD_WRITE)(
  IN BIOSGUARD_PROTOCOL *This,
  IN UINTN              Address,
  IN UINT32             DataByteCount,
  IN OUT UINT8          *Buffer
  );

/**
  This service fills BIOS Guard script buffer for erasing blocks in flash.
  BIOS should invoke this function prior to calling BIOSGUARD_PROTOCOL.Execute() with all the relevant data required for flash erase.
  This function will not invoke BIOS Guard module, only create script required for erasing each block in the flash.
  @warning This function would be called by runtime driver, please do not use any MMIO macro here.

  @param[in] This     Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in] Address  This value specifies the offset from the start of the SPI Flash component where BIOS Image is located.
**/
typedef
VOID
(EFIAPI *BIOSGUARD_ERASE)(
  IN BIOSGUARD_PROTOCOL *This,
  IN UINTN              Address
  );

/**
  This protocol provides all the services required for flash writes/erases via BIOS Guard
  BIOS Guard Module can only be launched from SMM
  @warning All flash writes & erases that BIOS needs to do must flow thru SMI Handler and so dependency on SMM_BASE_PROTOCOL
  for installing BIOS Guard Protocol. Prior to BIOS Guard SMM Protocol being installed there should be no writes/erases to flash.
**/
struct _BIOSGUARD_PROTOCOL {
  BIOSGUARD_WRITE   Write;   ///< Invoked to fill up BIOS Guard script buffer for flash writes
  BIOSGUARD_ERASE   Erase;   ///< Invoked to fill up BIOS Guard script buffer for flash erases
  BIOSGUARD_EXECUTE Execute; ///< Will trigger invocation of BIOS Guard module
};

#endif
