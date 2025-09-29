/** @file
  This file contains the required header files for the HSTI Silicon DXE driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#ifndef _HSTI_IHV_DXE_H_
#define _HSTI_IHV_DXE_H_

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/MpService.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Guid/EventGroup.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/Tcg2Protocol.h>
#include <IndustryStandard/Hsti.h>
#include <Protocol/AdapterInformation.h>
#include <Library/HstiLib.h>
#include <HstiFeatureBit_1_1a.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <Library/HobLib.h>
#include <Library/VtdInfoLib.h>
#include <Register/GenerationMsr.h>

#ifndef TPM_BASE
#define TPM_BASE                  0
#endif
#include <Library/Tpm2CommandLib.h>

#define TPM_BASE_ADDRESS                 0xfed40000
#define DTPM_LOC                         (BIT18 | BIT16)
#define FTPM_LOC                         (BIT18 | BIT17 | BIT16)
#define TPM_VER_FTIF_FT_LOC_MASK         (BIT18 | BIT17 | BIT16)
#define PSE_LOC                          (BIT18)

//
// Silicon
//
#include <Protocol/SiPolicyProtocol.h>
#include <Protocol/Spi.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SmbusInfoLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/PchDmiAccessLib.h>
#include <Library/IocAccessLib.h>
#include <Library/PcdRtcAccessLib.h>
#include <Library/PmcLib.h>
#include <Library/ThermalAccessLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/MeUtilsLib.h>
#include <Include/PcieRegs.h>
#include <Register/Cpuid.h>
#include <Register/PchRegs.h>
#include <Register/PchDmiRegs.h>
#include <Register/PchPcieRpRegs.h>
#include <Library/IpuLib.h>
#include <Library/CpuPlatformLib.h>
#include <MemInfoHob.h>
#include <Library/HostBridgeInfoLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Register/GenerationMsr.h>

#include <TcssInfo.h>

#include <Library/SmbusInfoLib.h>
#include <Library/TmeInfoLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/BaseTraceHubInfoLib.h>
#include <Library/TseInfoLib.h>
#include <Library/BaseImrInfoLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/IGpuInfoLib.h>

#define  SIZE_4KB    0x00001000
#define  SIZE_16KB   0x00004000
#define  SIZE_32KB   0x00008000
#define  SIZE_1MB    0x00100000
#define  SIZE_2MB    0x00200000
#define  SIZE_4MB    0x00400000
#define  SIZE_8MB    0x00800000
#define  SIZE_16MB   0x01000000
#define  SIZE_32MB   0x02000000
#define  SIZE_128MB  0x08000000
#define  SIZE_4GB    0x0000000100000000ULL
#define  BASE_4GB    0x0000000100000000ULL

#define MAX_NEW_AUTHORIZATION_SIZE        SHA512_DIGEST_SIZE

#define HSTI_PLATFORM_NAME  L"Intel"
#define HSTI_BUILD_STRING_ERROR     L"Invalid Input Parameter <Null String>: Unable to build Status String\r\n"
#define HSTI_OUT_OF_RESOURCES_ERROR L"Out of Resources: Unable to Save Status String in HSTI Status Structure\r\n"

#define   B_PCI_VENDOR_ID_MASK     (0xFFFF)
#define   B_IGD_DID_MASK           (0xFFFF)

// Max debug message length for DEBUG call is char[128],
// Workaround for printing 128 characters to Serial Comm port
// instead of one by one character
#define MAX_DEBUG_HSTI_MESSAGE_LENGTH     128

#pragma pack(1)
typedef struct {
  UINT32  Version;
  UINT32  Role;
  CHAR16  ImplementationID[256];
  UINT32  SecurityFeaturesSize;
  UINT8   SecurityFeaturesRequired[HSTI_SECURITY_FEATURE_SIZE];
  UINT8   SecurityFeaturesImplemented[HSTI_SECURITY_FEATURE_SIZE];
  UINT8   SecurityFeaturesVerified[HSTI_SECURITY_FEATURE_SIZE];
  CHAR16  End;
} ADAPTER_INFO_PLATFORM_SECURITY_STRUCT;
#pragma pack()

extern UINT8  mFeatureImplemented[HSTI_SECURITY_FEATURE_SIZE];

/**
  Check HSTI Library Set was Successful
**/
VOID
CheckStatusForHstiLibSet (
  IN EFI_STATUS   Status
  );

/**
  Check HSTI Library Set was Successful
**/
VOID
CheckStatusForHstiLibAppend (
  IN EFI_STATUS   Status
  );

/**
  Concatenate Status string.

  @param[in] StatusCodeString     - Status Code
  @param[in] StatusString         - Status Text
  @param[out] StringSizeOut       - Return String size for memory deallocation

  @retval CHAR16 - Concatenated string.
  Note: This function will always return a string. In case of error while
         trying to create the Concatenated String, it will as well return
         a string for Invalid output Parameter or out of resources error.
**/
CHAR16 *
EFIAPI
BuildHstiStatusStringAndSize (
  IN  CHAR16                   *StatusCodeString,
  IN  CHAR16                   *StatusString,
  OUT UINTN                    *StringSizeOut
  );

/**
  Concatenate Status string and Append HSTI Status String if it hasn't been reported yet.
  The codes are tracked by externally provided bitmap.
  If CodeReportedBitmap[CodeIndexBit] is not set, append the string
  Otherwise do nothing

  @param[in] StatusCodeString     - Status Code
  @param[in] StatusString         - Status Text
  @param[in] CodeReportedBitmap   - Flags for which codes have been used
  @param[in] CodeIndexBit         - Flag index for reported code

  @retval EFI_SUCCESS            The Status String was successfully build and saved in HSTI Status Structure.
**/
EFI_STATUS
EFIAPI
BuildAndAppendHstiUniqueStatusString (
  IN      CHAR16  *StatusCodeString,
  IN      CHAR16  *StatusString,
  IN OUT  UINT64  *CodeReportedBitmap,
  IN      UINT64  CodeIndexBit
  );

/**
  Concatenate Status string and Append HSTI Status String.

  @param[in] StatusCodeString     - Status Code
  @param[in] StatusString         - Status Text

  @retval EFI_SUCCESS            The Status String was successfully build and saved in HSTI Status Structure.
**/
EFI_STATUS
EFIAPI
BuildAndAppendHstiStatusString (
  IN  CHAR16                   *StatusCodeString,
  IN  CHAR16                   *StatusString
  );

/**
  Run tests for HardwareRootedBootIntegrity bit
**/
VOID
CheckHardwareRootedBootIntegrity (
  VOID
  );

/**
  Run tests for BootFirmwareMediaProtection bit
**/
VOID
CheckBootFirmwareMediaProtection (
  VOID
  );

/**
  Run tests for SignedFirmwareUpdate bit
**/
VOID
CheckSignedFirmwareUpdate (
  VOID
  );

/**
  Run tests for MeasuredBootEnforcement bit
**/
VOID
CheckMeasuredBootEnforcement (
  VOID
  );

/**
  Run tests for IntegratedDeviceDMAProtection bit
**/
VOID
CheckIntegratedDeviceDmaProtection (
  VOID
  );

/**
  Run tests for SecureDebugInterfaceConfiguration
**/
VOID
CheckSecureDebugInterfaceConfiguration (
  VOID
  );

/**
  Run tests for SecureCPUConfiguration bit
**/
VOID
CheckSecureCpuConfiguration (
  VOID
  );

/**
  Run tests for SecureMemoryMapConfiguration bit
**/
VOID
CheckSecureMemoryMapConfiguration (
  VOID
  );

/**
  Run tests for SecureIntegratedGraphicsConfiguration bit
**/
VOID
CheckSecureIntegratedGraphicsConfiguration (
  VOID
  );

/**
  Run tests for SecurePCHConfiguration bit
**/
VOID
CheckSecurePchConfiguration (
  VOID
  );

//
// Help function
//

/**
  Initialize MP Helper
**/
VOID
InitMp (
  VOID
  );

/**
  Concatenate error string.

  @retval UINTN - CpuNumber.
**/
UINTN
GetCpuNumber (
  VOID
  );


/**
  Read each processor MSR value.

  @param[in] ProcessorNumber     - Processor ID
  @param[in] Index               - Index

  @retval UINT64 - Msr Value.
**/
UINT64
ProcessorReadMsr64 (
  IN UINTN   ProcessorNumber,
  IN UINT32  Index
  );

/**
  Write value to each processor MSR.

  @param[in] ProcessorNumber     - Processor ID
  @param[in] Index               - Index
  @param[in] Value               - Value

**/
VOID
ProcessorWriteMsr64 (
  IN UINTN   ProcessorNumber,
  IN UINT32  Index,
  IN UINTN   Value
  );

/**
  Concatenate error string.

  @param[in]  ProcessorNumber     - Processor ID
  @param[in]  Index               - Index
  @param[out] Eax                 - Eax
  @param[out] Ebx                 - Ebx
  @param[out] Ecx                 - Ecx
  @param[out] Edx                 - Edx
**/
VOID
ProcessorCpuid (
  IN  UINTN   ProcessorNumber,
  IN  UINT32  Index,
  OUT UINT32  *Eax,  OPTIONAL
  OUT UINT32  *Ebx,  OPTIONAL
  OUT UINT32  *Ecx,  OPTIONAL
  OUT UINT32  *Edx   OPTIONAL
  );

/**
  Concatenate error string.

  @param[in] Address     - Address

  @retval UINT64 - Value.
**/
UINT64
EFIAPI
PciRead64 (
  IN  UINTN                   Address
  );

#endif
