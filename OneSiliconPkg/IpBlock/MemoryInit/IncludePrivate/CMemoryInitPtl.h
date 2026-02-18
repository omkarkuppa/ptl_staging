/** @file
  Memory Initialization PEIM PTL.

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

@par Specification Reference:
**/

#include <CMrcInterface.h>
#include <Ppi/SiPolicy.h>
#include <Library/CpuInitPreMem.h>

/**
  Determines whether or not the platform has executed a TXT launch by
  examining the TPM Establishment bit.
  @param[in] VOID
  @retval TRUE        - If the TPM establishment bit is asserted.
  @retval FALSE       - If the TPM establishment bit is unasserted.
**/
extern
BOOLEAN
IsEstablishmentBitAsserted (
  VOID
  );

/**
  Unlock memory when security is set and TxT is not enabled.
  @param[in] MrcData     - Mrc global data.
  @retval Nothing
**/
extern
void
UnlockMemory (
  IN CONST MrcParameters    *CONST  MrcData
  );

/**
  Determine whether a cold reset of the platform is required.
  Note that the memory configuration saved data must be valid.

  @param[in] MrcData                 - The MRC "global data" area.
  @param[in] MemConfig               - MEMORY_CONFIGURATION structure.
  @param[in] CpuMemoryInitConfig     - The Cpu Memory Init config.
  @param[in] MrcBootMode             - Current MRC boot mode.
  @param[out] IsEfiResetColdRequired - TRUE if a power cycle is required.

  @retval TRUE if cold reset is required, otherwise returns FALSE.
**/
extern
BOOLEAN
ColdBootRequired (
  IN MrcParameters                *CONST  MrcData,
  IN MEMORY_CONFIGURATION         *MemConfig,
  IN CPU_MEMORY_INIT_CONFIG       *CpuMemoryInitConfig,
  IN MRC_BOOT_MODE                MrcBootMode,
  OUT BOOLEAN                     *IsEfiResetColdRequired
  );

/**
  Update the progress bar showing the MRC completion status.

  @param[in out]  MrcData - All the MRC global data.
**/
VOID
MrcUpdateProgressBar (
  IN OUT MrcParameters *CONST MrcData
  );

/**
  Show an error message upon encountering an MRC failure.

  @param[in]       IGpuPreMemConfig   - IGPU_PEI_PREMEM_CONFIG to access the IGpuConfig related information.
  @param[in]       PostCode           - Current post code.
**/
VOID
MrcShowProgressBarMessage (
  IN IGPU_PEI_PREMEM_CONFIG       *IGpuPreMemConfig,
  IN UINT16                        PostCode
  );
