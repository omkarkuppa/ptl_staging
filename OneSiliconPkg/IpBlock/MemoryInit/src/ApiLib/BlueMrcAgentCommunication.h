/** @file
  This file includes all definitions needed for inter-agent communications from Blue MRC.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _BlueMrcAgentCommunication_h_
#define _BlueMrcAgentCommunication_h_

#include "CBlueMrcCommonTypes.h"
#include "CMrcInterface.h"  // For MrcParameters
#include "MrcBlueGreenCommunication.h"

/**
  Translate Address from
  Xtensa MemSpace (Used by Xtensa Compiler) to
  UCSS   MemSpace (Used by indirect Mem Access UCSS Register)

  @param[in] XtensaAddr - Xtensa MemSpace Address.

  @retval returns UCSS MemSpace Address.
**/
UINT32
XtensaToUcssAddrTranslation (
  IN UINT32 XtensaAddr
  );

/**
  Write data into the UC SRAM.

  @param[in] MrcData     - Include all MRC global data.
  @param[in] Source      - x86 address to read data from.
  @param[in] Destination - UC address to copy data to.
  @param[in] Size        - Payload size in bytes. If size is not a multiple of 4, then an extra [1..3] bytes will be written as UCSS works in DWORD resolution.

  @retval returns mrcSuccess.
**/
MrcStatus
BlueMrcWriteUcData (
  IN MrcParameters *const MrcData,
  IN UINT32               *Source,
  IN UINT32               Destination,
  IN UINT32               Size
  );

/**
  Write data into the UC SRAM.
  This routine assumes that the SRAM is preloaded with zeroes and will skip writing zero data.
  This speeds up writing blocks of data that have a lot of zeroes, such as MrcData.

  @param[in] MrcData     - Include all MRC global data.
  @param[in] Source      - x86 address to read data from.
  @param[in] Destination - UC address to copy data to.
  @param[in] Size        - Payload size in bytes. If size is not a multiple of 4, then an extra [1..3] bytes will be written as UCSS works in DWORD resolution.

  @retval returns mrcSuccess.
**/
MrcStatus
BlueMrcWriteUcDataSkipZeroes (
  IN MrcParameters *const MrcData,
  IN UINT32               *Source,
  IN UINT32               Destination,
  IN UINT32               Size
  );

/**
  Read data back from the UC SRAM.

  @param[in] MrcData     - Include all MRC global data.
  @param[in] Source      - UC address to read data from.
  @param[in] Destination - x86 address to copy data to.
  @param[in] Size        - Data size in bytes. If size is not a multiple of 4, then an extra [1..3] bytes will be read as UCSS works in DWORD resolution.

  @retval returns mrcSuccess.
**/
MrcStatus
BlueMrcReadUcData (
  IN     MrcParameters *const MrcData,
  IN     UINT32               Source,
  IN OUT UINT32               *Destination,
  IN     UINT32               Size
  );

/**
  Read the dedicated communication buffer in UC SRAM.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] CommBuffer - Address to copy comm buffer data to.

  @retval passes through BlueMrcReadUcData's return value.
**/
MrcStatus
BlueMrcReadUcCommBuffer (
  IN     MrcParameters      *const MrcData,
  IN OUT MrcBlueGreenCommunication *CommBuffer
  );

/**
  Write the dedicated communication buffer in UC SRAM.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] CommBuffer - Address to copy comm buffer data from.

  @retval passes through BlueMrcWriteUcData's return value.
**/
MrcStatus
BlueMrcWriteUcCommBuffer (
  IN    MrcParameters      *const MrcData,
  IN    MrcBlueGreenCommunication *CommBuffer
  );

/**
  Write External Inputs in UC SRAM.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] ExtInputs  - Address to copy external inputs data from.

  @retval passes through BlueMrcWriteUcData's return value.
**/
MrcStatus
BlueMrcWriteUcExtInputs (
  IN  MrcParameters       *const MrcData,
  IN  MRC_EXT_INPUTS_TYPE *ExtInputs
  );

/**
  Set or clear MRC software halt flag for UC.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Halt    - TRUE to halt, FALSE to un-halt.

  @retval passes through BlueMrcRead/WriteUcCommBuffer's return value.
**/
MrcStatus
BlueMrcSetXtensaSoftwareHaltState (
  IN MrcParameters *const MrcData,
  IN BOOLEAN Halt
  );

/**
  Set or clear halt bit in UC control register.
  Poll until Halt/Unhalt takes effect, with a timeout.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Halt    - TRUE to halt, FALSE to un-halt.

  @retval mrcSuccess Operation completed successfully
  @retval mrcTimeout Timed out waiting on xt_halt bit to match xt_runstall
**/
MrcStatus
BlueMrcSetXtensaHardwareHaltState (
  IN MrcParameters *const MrcData,
  IN BOOLEAN Halt
  );

/**
  Set soft reset bit in UC control register.

  @param[in] MrcData - Include all MRC global data.

  @retval returns mrcSuccess.
**/
MrcStatus
BlueMrcXtensaSoftReset (
  IN MrcParameters *const MrcData
  );

/**
  Set Xtensa's FW download done bit.

  @param[in] MrcData - Include all MRC global data.

  @retval returns mrcSuccess.
**/
MrcStatus
BlueMrcSetXtensaFwDownloadDone (
  IN MrcParameters *const MrcData
  );

/**
  Sets Xtensa's near and far SRAMs to a known value.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess - SRAM was preloaded
  @retval mrcTimeout - Timed out during SRAM preload
**/
MrcStatus
BlueMrcClearXtensaSram (
  IN MrcParameters *const MrcData
  );

/**
  Check xtensa status registers, print any error conditions.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess - if no fatal error occurs, otherwise return mrcFail.
**/
MrcStatus
BlueMrcCheckXtStatusRegs (
  IN MrcParameters *const MrcData
  );

/**
  Check the Green MRC print log and print any contents.

  @param[in] MrcData - Include all MRC global data.
  @param[in] LogSize - Green log size (without NULL terminator)

  @retval returns mrcSuccess.
**/
MrcStatus
BlueMrcPrintGreenLog (
  IN MrcParameters *const MrcData,
  IN UINT32               LogSize
  );

#endif // _BlueMrcAgentCommunication_h_
