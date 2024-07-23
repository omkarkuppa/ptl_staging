/** @file
  Header file for PeiAceInitLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _PEI_ACE_INIT_LIB_H_
#define _PEI_ACE_INIT_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/PeiHdaLib.h>

/**
  Program HDA and DSP bars for temporary MMIO access.

  @param[in, out] HdaController        Pointer to Hda device structure

  @retval VOID
**/
VOID
AceEnableHdaDspMmioAccess (
  IN OUT  HDA_CONTROLLER        *HdaController
  );

/**
  Clear HDA and DSP bars to disable MMIO access.

  @param[in] HdaController           Pointer to Hda device structure

  @retval VOID
**/
VOID
AceDisableHdaDspMmioAccess (
  IN  HDA_CONTROLLER        *HdaController
  );

/**
  Check if DSP is disbaled or not by fuses.

  @param[in] HdaController        Pointer to Hda Controller structure

  @retval TRUE                    Audio DSP is not present
  @retval FALSE                   Audio DSP is present
**/
BOOLEAN
AceIsDspFusedOut (
  HDA_CONTROLLER               *HdaController
  );

/**
  Initialize the Intel High Definition Audio Codec(s) present in the system.
  For each codec, a predefined codec verb table should be programmed.
  The list contains 32-bit verbs to be sent to the corresponding codec.
  If it is not programmed, the codec uses the default verb table, which may or may not
  correspond to the platform jack information.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_ABORTED             Controller in reset - unable to proceed with initialization
  @retval EFI_INVALID_PARAMETER   Provided VerbTableData is null
**/
EFI_STATUS
AceConfigureHdAudioCodec (
  IN HDA_HANDLE                *HdaHandle
  );

/**
  Triggers ACE initialization or disables ACE depending on HdaPreMem configuration.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure
  @param[in, out] XtalValue       Pointer to Xtal value
  @param[in] InterruptPin         Interrupt pin number

  @retval EFI_SUCCESS             Controller disabled successfully
**/
EFI_STATUS
AceConfigureHdAudio (
  IN HDA_HANDLE                *HdaHandle,
  IN OUT UINT32                *XtalValue,
  IN UINT8                     InterruptPin
  );

#endif
