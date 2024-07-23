/** @file
  Provide SEC board init data and functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <PiDxe.h>
#include <Library/BoardInitLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SerialPortLib.h>
#include <Library/GetFsptApiParameter.h>
#include <FspUpd.h>
#include <FsptUpd.h>

EFI_PEI_PPI_DESCRIPTOR  mFsptUpdLocationPpi[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gFsptUpdLocationPpiGuid,
    NULL // To be patched later.
  }
};

VOID
BootloaderDebugHandler (
  IN UINT8  *Buffer,
  IN UINTN  NumberOfBytes
  );

#pragma pack(1)

#if defined (MDE_CPU_IA32)
///
/// FSPT_ARCH2_IA32_UPD Configuration.
///
typedef struct {
  ///
  /// Revision of the structure is 2 for this version of the specification.
  ///
  UINT8                Revision;
  UINT8                Reserved[3];
  ///
  /// Length of the structure in bytes. The current value for this field is 64.
  ///
  UINT32               Length;
  ///
  /// FspDebugHandler Optional debug handler for the bootloader to receive debug messages
  /// occurring during FSP execution.
  ///
  UINT32               FspDebugHandlerLow32;
  UINT32               FspDebugHandlerHigh32;
  UINT8                Reserved1[16];
} FSPT_ARCH2_IA32_UPD;

typedef struct {
  FSP_UPD_HEADER              FspUpdHeader;
  FSPT_ARCH2_IA32_UPD         FsptArchUpd;
  FSPT_CORE_UPD               FsptCoreUpd;
  FSP_T_CONFIG                FsptConfig;
  UINT8                       UnusedUpdSpace4[6];
  UINT16                      UpdTerminator;
} FSPT_IA32_UPD;

GLOBAL_REMOVE_IF_UNREFERENCED CONST FSPT_IA32_UPD FsptUpdDataPtr = {
#else
GLOBAL_REMOVE_IF_UNREFERENCED CONST FSPT_UPD FsptUpdDataPtr = {
#endif
  {
    FSPT_UPD_SIGNATURE,
    0x02,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00
    }
  },
  {
    0x03,
    {
      0x00, 0x00, 0x00
    },
    0x00000020,
    (UINTN) BootloaderDebugHandler,
    (UINT32) 0x000000000,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
  },
  {
    FixedPcdGet32 (PcdFlashFvMicrocodeBase) + FixedPcdGet32 (PcdMicrocodeOffsetInFv),
#if (FixedPcdGetBool (PcdLoadMicroCodePatch) == 1)
    FixedPcdGet32 (PcdFlashFvMicrocodeSize) - FixedPcdGet32 (PcdMicrocodeOffsetInFv),
#else
    0,
#endif
    0,          // Set CodeRegionBase as 0, so that caching will be 4GB-(CodeRegionSize > LLCSize ? LLCSize : CodeRegionSize) will be used.
    FixedPcdGet32 (PcdFlashCodeCacheSize)
  },
  {
    FixedPcdGet8 (PcdSerialIoUartDebugEnable),
    FixedPcdGet8 (PcdSerialIoUartNumber),
    FixedPcdGet8 (PcdSerialIoUartMode),
    0,
    FixedPcdGet32 (PcdSerialIoUartBaudRate),
    FixedPcdGet64 (PcdPciExpressBaseAddress),
    FixedPcdGet32 (PcdTemporaryPciExpressRegionLength),
    FixedPcdGet8 (PcdSerialIoUartParity),
    FixedPcdGet8 (PcdSerialIoUartDataBits),
    FixedPcdGet8 (PcdSerialIoUartStopBits),
    FixedPcdGet8 (PcdSerialIoUartAutoFlow),
    FixedPcdGet32 (PcdSerialIoUartRxPinMux),
    FixedPcdGet32 (PcdSerialIoUartTxPinMux),
    FixedPcdGet32 (PcdSerialIoUartRtsPinMux),
    FixedPcdGet32 (PcdSerialIoUartCtsPinMux),
    FixedPcdGet32 (PcdSerialIoUartDebugMmioBase),
    FixedPcdGet32 (PcdSerialIoUartDebugPciCfgBase),
    FixedPcdGet8 (PcdLpcUartDebugEnable),
    FixedPcdGet8 (PcdDebugInterfaceFlags),
    FixedPcdGet8 (PcdSerialDebugLevel),
    FixedPcdGet8 (PcdIsaSerialUartBase),
    FixedPcdGet8 (PcdSerialIo2ndUartEnable),
    FixedPcdGet8 (PcdSerialIo2ndUartNumber),
    FixedPcdGet8 (PcdSerialIo2ndUartMode),
    0, //Needed for UnusedUpdSpace alignment
    FixedPcdGet32 (PcdSerialIo2ndUartBaudRate),
    FixedPcdGet8 (PcdSerialIo2ndUartParity),
    FixedPcdGet8 (PcdSerialIo2ndUartDataBits),
    FixedPcdGet8 (PcdSerialIo2ndUartStopBits),
    FixedPcdGet8 (PcdSerialIo2ndUartAutoFlow),
    FixedPcdGet32 (PcdSerialIo2ndUartRxPinMux),
    FixedPcdGet32 (PcdSerialIo2ndUartTxPinMux),
    FixedPcdGet32 (PcdSerialIo2ndUartRtsPinMux),
    FixedPcdGet32 (PcdSerialIo2ndUartCtsPinMux),
    FixedPcdGet32 (PcdSerialIo2ndUartMmioBase),
    FixedPcdGet32 (PcdSerialIo2ndUartPciCfgBase),
    FixedPcdGet32 (PcdTopMemoryCacheSize),
    {FixedPcdGet8 (PcdSerialIoSpiCs0Polarity), FixedPcdGet8 (PcdSerialIoSpiCs1Polarity)},      // PcdSerialIoSpiCsPolarity
    {FixedPcdGet8 (PcdSerialIoSpiCs0Enable), FixedPcdGet8 (PcdSerialIoSpiCs1Enable)},          // PcdSerialIoSpiCsEnable
    FixedPcdGet8 (PcdSerialIoSpiMode),                                                         // PcdSerialIoSpiMode
    FixedPcdGet8 (PcdSerialIoSpiDefaultCsOutput),                                              // PcdSerialIoSpiDefaultCsOutput
    FixedPcdGet8 (PcdSerialIoSpiCsMode),                                                       // PcdSerialIoSpiCsMode
    FixedPcdGet8 (PcdSerialIoSpiCsState),                                                      // PcdSerialIoSpiCsState
    FixedPcdGet8 (PcdSerialIoSpiNumber),                                                       // PcdSerialIoSpiNumber
    {0, 0, 0},                                                                                 // Rsvd0030
    FixedPcdGet32 (PcdSerialIoSpiMmioBase),                                                    // PcdSerialIoSpiMmioBase
    {FixedPcdGet32 (PcdSerialIoSpiCs0PinMux), FixedPcdGet32 (PcdSerialIoSpiCs0PinMux)},        // PcdSerialIoSpiCsPinMux
    FixedPcdGet32 (PcdSerialIoSpiClkPinMux),                                                   // PcdSerialIoSpiClkPinMux
    FixedPcdGet32 (PcdSerialIoSpiMisoPinMux),                                                  // PcdSerialIoSpiMisoPinMux
    FixedPcdGet32 (PcdSerialIoSpiMosiPinMux),                                                  // PcdSerialIoSpiMosiPinMux
    FixedPcdGet32 (PcdSerialIoI2cMmioBase),                                                    // PcdSerialIoI2cMmioBase
    FixedPcdGet32 (PcdSerialIoI2cSdaPin),                                                      // PcdSerialIoI2cSdaPin
    FixedPcdGet32 (PcdSerialIoI2cSclPin),                                                      // PcdSerialIoI2cSclPin
    FixedPcdGet8 (PcdSerialIoI2cPadsTerm),                                                     // PcdSerialIoI2cPadsTerm
    FixedPcdGet8 (PcdSerialIoI2cNumber),                                                       // PcdSerialIoI2cNumber
    FixedPcdGet8 (PcdEnableFspmCompression),                                                   // SecondaryDataStackSet
    {
      0x00
    }
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  0x55AA
};

#pragma pack()

EFI_STATUS
EFIAPI
BoardAfterTempRamInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

UINT8 *
EFIAPI
AppendAdditionalPlatformPpi (
  IN     UINT8  *CopyDestinationPointer,
  IN OUT UINT8  *PpiIndex
  )
{
  FSPT_UPD  *FsptUpd;

  FsptUpd = (FSPT_UPD *)((UINTN) (SecGetFsptApiParameter ()));
  if (FsptUpd->FspUpdHeader.Signature == FSPT_UPD_SIGNATURE) {
    CopyMem (CopyDestinationPointer, mFsptUpdLocationPpi, sizeof (mFsptUpdLocationPpi));
    ((EFI_PEI_PPI_DESCRIPTOR *) CopyDestinationPointer)->Ppi = (VOID *) FsptUpd;
    *PpiIndex += 1;
    return (CopyDestinationPointer += sizeof (mFsptUpdLocationPpi));
  } else {
    return CopyDestinationPointer;
  }
}

VOID
BootloaderDebugHandler (
  IN UINT8  *Buffer,
  IN UINTN  NumberOfBytes
  )
{
  SerialPortWrite ((UINT8 *)Buffer, NumberOfBytes);
}