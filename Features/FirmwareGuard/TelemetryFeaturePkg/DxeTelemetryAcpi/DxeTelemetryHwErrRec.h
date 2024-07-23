/** @file
  Dxe Telemetry HwErrRec Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _DXE_TELEMETRY_HWERRREC_H_
#define _DXE_TELEMETRY_HWERRREC_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <TelemetryCommon.h>
#include <Protocol/FirmwareManagement.h>
#include <Library/PrintLib.h>
#include <Guid/Cper.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>


#define TELEMETRY_HW_ERR_REC_VAR_NAME       L"HwErrRec"
#define TELEMETRY_HW_ERR_REC_VAR_NAME_LEN   16      // Buffer length covers at least "HwErrRec####\0"

#define ERROR_RECORD_HEADER_SIGNATURE_START  SIGNATURE_32('C', 'P', 'E', 'R')
#define ERROR_RECORD_HEADER_SIGNATURE_END    0xFFFFFFFF
#define ERROR_RECORD_HEADER_REVISION         0x0101
#define ERROR_RECORD_HEADER_SIGNATURE_COUNT  0x01
#define ERROR_RECORD_HEADER_ERRORSEVERITY    0x03
#define ERROR_RECORD_HEADER_VALIDATIONBITS   0x01
#define ERROR_RECORD_HEADER_FLAGS            0x00

#define ERROR_RECORD_SECTION_DESCRIPTOR_REVISION      0x0100
#define ERROR_RECORD_SECTION_DESCRIPTOR_SECVALIDMASK  0x00
#define ERROR_RECORD_SECTION_DESCRIPTOR_SECTIONFLAGS  0x01
#define ERROR_RECORD_SECTION_DESCRIPTOR_SEVERITY      0x03


extern GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN  mIsFirstCall;
extern GLOBAL_REMOVE_IF_UNREFERENCED UINT8    *mMotherBoardHealthRecBuffer;

#pragma pack(1)

typedef struct {
  EFI_COMMON_ERROR_RECORD_HEADER                   Header;            // Common error record header
  EFI_ERROR_SECTION_DESCRIPTOR                     SectionDescriptor; // Error Section Descriptor
} EFI_DRIVERHEALTH_HARDWARE_ERROR_RECORD;

typedef struct {
  UINT16            RecordLength;             // The length of ther version data record.
  UINT8             Revision;                 // Revision of this Firmware Version Data Record.  For spec 0.3d, the value is fixed to 1.
  UINT32            Reserved;
  UINT8             AmHealthy;                // is the device healthy?
                                              // 0 = Errors found
                                              // 1 = No errors found
                                              // 2 = Unknown (treated as healthy)
                                              // 3 = Advisory - additional device-specific data exposed
  EFI_GUID          DeviceSignature;          // The GUIDed record type that follows
  UINT32            DeviceSpecificDataOffset; // Offset to the Device-specific Data from the start of this Data Record.
//EFI_DEVICE_PATH   DevicePath;               // Device Path of the component
} EFI_TELEMETRY_HEALTH_RECORD;

#pragma pack()

/**

  This function set DriverHealth HwErrRec Variable.
  If the current variable size is greater than the MaximumVariableSize
  current HwErrRec Variable will not be created.

  @param[IN]      AcpiPhatTablePointer  A pointer to the EFI_ACPI_DESCRIPTION_HEADER instance.

  @retval EFI_SUCCESS            The firmware has successfully stored the variable and its data as
                                 defined by the Attributes.
  @retval EFI_INVALID_PARAMETER  An invalid combination of attribute bits, name, and GUID was supplied, or the
                                 DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER  VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED    The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION The variable could not be written due to EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set,
                                 but the AuthInfo does NOT pass the validation check carried out by the firmware.
  @retval EFI_NOT_FOUND          The variable trying to be updated or deleted was not found.

**/
EFI_STATUS
EFIAPI
SetDriverHealthHwErrRecVariable (
  IN     EFI_ACPI_DESCRIPTION_HEADER     *AcpiPhatTablePointer
  );

/**

Clear all the Telemetry HwErrRec entries on flash.

  @retval EFI_SUCCESS            The firmware has successfully stored the variable and its data as
                                 defined by the Attributes.
  @retval EFI_INVALID_PARAMETER  An invalid combination of attribute bits, name, and GUID was supplied, or the
                                 DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER  VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED    The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION The variable could not be written due to EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set,
                                 but the AuthInfo does NOT pass the validation check carried out by the firmware.
  @retval EFI_NOT_FOUND          The variable trying to be updated or deleted was not found.

**/
EFI_STATUS
EFIAPI
ClearAllTelemetryHwErrRec (
  VOID
  );

#endif
