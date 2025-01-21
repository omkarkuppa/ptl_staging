/** @file
  Implementation for SpcrDeviceLib with AMT support.

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

#include <Uefi.h>
#include <Protocol/DevicePath.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/SpcrDeviceLib.h>
#if FixedPcdGetBool(PcdAmtEnable) == 1
#include <Library/DxeAsfLib.h>
#include <Library/DxeAmtSupportLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#endif

/**
  Get a Serial Port device used for SPCR.
  The caller should call FreePool() to free the memory if return value is not NULL.

  This function will try to read the EFI variable OutOfBand,
  if empty, then try to get the Serial Over Lan device of AMT,
  and save it to OutOfBand again if get it.

  @retval NULL          Can not get device.
  @retval Others        A serial port device path.

**/
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
GetSpcrDevice (
  VOID
  )
{
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;

  DevicePath = NULL;
  GetVariable2 (L"OutOfBand", &gOutOfBandGuid, (VOID **) &DevicePath, NULL);
  if (DevicePath != NULL) {
    return DevicePath;
  }

#if FixedPcdGetBool(PcdAmtEnable) == 1
  if (AsfIsSolEnabled ()) {
    return NULL;
  }

  DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AmtGetSolDevicePath ();
  if (DevicePath == NULL) {
    return NULL;
  }

  gRT->SetVariable (
          L"OutOfBand",
          &gOutOfBandGuid,
          EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
          GetDevicePathSize (DevicePath),
          DevicePath
          );
  // The memory returned can't be FreePool (), so duplicate it.
  DevicePath = DuplicateDevicePath (DevicePath);

#endif // FixedPcdGetBool(PcdAmtEnable) == 1
  return DevicePath;
}
