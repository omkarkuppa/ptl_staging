/** @file
  Header file for PCH Initialization Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _PCH_INIT_DXE_H_
#define _PCH_INIT_DXE_H_

#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <SiConfigHob.h>
#include <Protocol/PchNvsArea.h>
#include <ItssConfigHob.h>
#include <LpssUartConfigHob.h>
#include <PmcConfigHob.h>
#include <LpssI2cConfigHob.h>
#include <LpssSpiConfigHob.h>
#include <PchConfigHobCommon.h>
#include <HdaConfigHob.h>
#include <PchPcieRpConfigHob.h>
#include <EspiConfigHob.h>

//
// Data definitions
//
extern EFI_HANDLE               mImageHandle;

//
// Pch NVS area definition
//
extern PCH_NVS_AREA_PROTOCOL    mPchNvsAreaProtocol;

extern ITSS_CONFIG_HOB          *mPchItssConfigHob;
extern LPSS_UART_CONFIG_HOB     *mPchLpssUartConfigHob;
extern LPSS_SPI_CONFIG_HOB      *mPchLpssSpiConfigHob;
extern PMC_CONFIG_HOB           *mPchPmcConfigHob;
extern LOCK_DOWN_CONFIG_HOB     *mPchLockDownConfigHob;
extern PCIE_RP_CONFIG_HOB       *mPchPcieRpConfigHob;
extern HDAUDIO_CONFIG_HOB       *mPchHdaConfigHob;

//
// Function Prototype
//

//
// Local function prototypes
//
/**
  Common PchInit Module Entry Point
**/
VOID
PchInitEntryPointCommon (
  VOID
  );

/**
  Common PCH initialization on PCI enumeration complete.
**/
VOID
PchOnPciEnumCompleteCommon (
  VOID
  );

/**
  Calls Boot Os Hand off routine for each Serial IO Controller
**/
VOID
ConfigureSerialIoAtBoot (
  VOID
  );

/**
  Puts all SerialIo controllers (except UARTs in debug mode) in D3.
  Clears MemoryEnable for all PCI-mode controllers on S3 resume
**/
VOID
ConfigureSerialIoAtS3Resume (
  VOID
  );

/**
  Initialize Intel High Definition Audio ACPI Tables

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_LOAD_ERROR          ACPI table cannot be installed
  @retval EFI_UNSUPPORTED         ACPI table not set because DSP is disabled
**/
EFI_STATUS
PchHdAudioAcpiInit (
  VOID
  );

/**
  Update ASL data for CNVI Device.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
UpdateCnviAcpiData (
   VOID
   );

/**
  Initialize Pch acpi
  @param[in] ImageHandle          Handle for the image of this driver

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchAcpiInit (
  IN EFI_HANDLE         ImageHandle
  );

/**
  Update ASL object before Boot

  @retval EFI_STATUS
  @retval EFI_NOT_READY         The Acpi protocols are not ready.
**/
EFI_STATUS
PchUpdateNvsArea (
  VOID
  );

/**
  Initialize PCH Nvs Area opeartion region.

  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
PatchPchNvsAreaAddress (
  VOID
  );

/**
  Perform the remaining configuration on USB.
  Set the XHCC2.WRREQSZCTRL and XHCC2.RDREQSZCTRL based on the DMI DEVCTL MPS configured value

  @retval None
**/
VOID
ConfigureUsbOnEndOfDxe (
  VOID
  );

#endif // _PCH_INIT_DXE_H_
