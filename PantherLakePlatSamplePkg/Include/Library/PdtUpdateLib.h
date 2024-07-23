/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _PDT_UPDATE_LIB_H_
#define _PDT_UPDATE_LIB_H_

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeMeLib.h>

/**
  API to Update new PDT file by sending it to ME

  @param[in] SensorBomId    SensorBomId to be matched to in MultiPDT package
  @param[in] FileBuffer     The pointer to MultiPdt file
  @param[in] MultiPdtSize   MultiPDT file size

  @retval EFI_SUCCESS       Bios2Ish file send complete.

**/

EFI_STATUS
PdtUpdate (
  IN  UINT32  SensorBomId,
  IN  UINT8   *FileBuffer,
  IN  UINT32  MultiPdtSize
  );

#endif
