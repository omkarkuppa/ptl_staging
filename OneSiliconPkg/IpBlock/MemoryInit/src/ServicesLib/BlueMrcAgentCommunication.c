/** @file
  This file is used as a driver to all inter-agent communications from Blue MRC.
  It includes all the functions that the Blue MRC needs to send/receive data.

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

#include "MrcCommon.h"
#include "BlueMrcAgentCommunication.h"
#include "MrcUcPayloadData.h"
#include "MrcDdrIo.h"

#ifndef FULL_HEADLESS

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
  )
{
  UINT32 UcssSramBase;
  UINT32 XtensaSramBase;
  UINT32 UcssAddress;

  // Translate SRAM address from Xtensa address space to UCSS address space:
  // UcssAddress = <UCSS SRAM region base>             +  <offset inside this region>
  // UcssAddress = <DRAM/IRAM/FESRAM HW start address> + (<SRAM address in Xtensa space> - <DRAM/IRAM/FESRAM start address in Xtensa space>)

  if (XtensaAddr >= XTENSA_FESRAM_BASE) {
    XtensaSramBase = XTENSA_FESRAM_BASE;
    UcssSramBase   = MRC_UCSS_FESRAM_START;
  } else if (XtensaAddr >= XTENSA_IRAM_BASE) {
    XtensaSramBase = XTENSA_IRAM_BASE;
    UcssSramBase = MRC_UCSS_IRAM_START;
  } else { // DRAM
    XtensaSramBase = XTENSA_DRAM_BASE;
    UcssSramBase   = MRC_UCSS_DRAM_START;
  }

  UcssAddress = UcssSramBase + XtensaAddr - XtensaSramBase;
  return UcssAddress;
}

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
  )
{
  MrcDebug  *Debug;
  UINT32    BytesCopied;
  UINT32    UcssAddress;
  BOOLEAN   DotsPrinted;

  Debug = &MrcData->Outputs.Debug;

  UcssAddress = XtensaToUcssAddrTranslation (Destination);

  MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_ADDR_REG, UcssAddress);
  BytesCopied = 0;
  DotsPrinted = FALSE;
  while (BytesCopied < Size) {
    MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_DATA_REG, *((UINT32 *) ((UINTN) Source + BytesCopied)));
    BytesCopied += 4;
    if ((BytesCopied % 8192) == 0) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "."); // Print a dot every 8KB
      DotsPrinted = TRUE;
    }
  }
  if (DotsPrinted) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
  }

  return mrcSuccess;
}

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
  )
{
  UINT32 BytesCopied;
  UINT32 UcssAddress;
  UINT32 Data;
  UINT32 ZeroCount;

  UcssAddress = XtensaToUcssAddrTranslation (Destination);
  Data = 0;

  MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_ADDR_REG, UcssAddress);
  BytesCopied = 0;
  while (BytesCopied < Size) {
    ZeroCount = 0;
    while (BytesCopied < Size) {
      Data = *((UINT32 *) ((UINTN) Source + BytesCopied));
      if (Data != 0) {
        break;
      }
      ZeroCount   += 4;
      BytesCopied += 4;
    }
    if (BytesCopied >= Size) {    // Last section of the block is all zeroes
      break;
    }
    if (ZeroCount != 0) {
      // Skip one or more zero DWORDs
      UcssAddress += ZeroCount;
      MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_ADDR_REG, UcssAddress);
    }
    MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_DATA_REG, Data);
    BytesCopied += 4;
    UcssAddress += 4;
  }

  return mrcSuccess;
}

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
  )
{
  UINT32 BytesCopied;
  UINT32 UcssAddress;

  UcssAddress = XtensaToUcssAddrTranslation (Source);
  // No need to halt the UC when reading from SRAM
  MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_ADDR_REG, UcssAddress);
  BytesCopied = 0;
  while (BytesCopied < Size) {
    *((UINT32 *) ((UINTN) Destination + BytesCopied)) = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_SRAM_DATA_REG);
    BytesCopied += 4;
  }

  return mrcSuccess;
}

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
  )
{
  return BlueMrcReadUcData (MrcData, gUcCommBufferAddress, (UINT32 *) CommBuffer, sizeof (MrcBlueGreenCommunication));
}

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
  )
{
  return BlueMrcWriteUcData (MrcData, (UINT32 *) CommBuffer, gUcCommBufferAddress, sizeof (MrcBlueGreenCommunication));
}

/**
  Write External Inputs in UC SRAM.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] ExtInputs  - Address to copy external inputs data from.

  @retval passes through BlueMrcWriteUcData's return value.
**/
MrcStatus
BlueMrcWriteUcExtInputs (
  IN    MrcParameters       *const MrcData,
  IN    MRC_EXT_INPUTS_TYPE *ExtInputs
  )
{
  return BlueMrcWriteUcData (MrcData, (UINT32 *) ExtInputs, gUcExtInputsAddress, sizeof (MRC_EXT_INPUTS_TYPE));
}

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
  )
{
  MrcStatus Status;
  MrcBlueGreenCommunication CommBuffer;

  Status = BlueMrcReadUcCommBuffer (MrcData, &CommBuffer);
  if (Status != mrcSuccess) {
    return Status;
  }

  CommBuffer.Misc.Bits.Halt = Halt;
  return BlueMrcWriteUcCommBuffer (MrcData, &CommBuffer);
}

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
  IN BOOLEAN              Halt
  )
{
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_STRUCT   Ctrl;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_STATUS_STRUCT Status;
  UINT64 Timeout;

  //
  // @todo: These CR accesses should get absorbed into the HAL
  //

  Ctrl.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_REG);
  Ctrl.Bits.xt_runstall = Halt;
  MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_REG, Ctrl.Data);

  // Wait until Halt/unhalt takes effect
  Timeout = MrcData->Inputs.Call.Func->MrcGetCpuTime() + MRC_WAIT_TIMEOUT;
  do {
    Status.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_STATUS_REG);
  } while ((Status.Bits.xt_halt != Halt) && (MrcData->Inputs.Call.Func->MrcGetCpuTime () < Timeout));

  if (Status.Bits.xt_halt != Halt) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s Xtensa %sHalt Timeout\n", gErrString, (Halt ? "" : "Un"));
    return mrcTimeout;
  }
  return mrcSuccess;
}

/**
  Set soft reset bit in UC control register.

  @param[in] MrcData - Include all MRC global data.

  @retval returns mrcSuccess.
**/
MrcStatus
BlueMrcXtensaSoftReset (
  IN MrcParameters *const MrcData
  )
{
  UINT64 Timeout;
  BOOLEAN Busy;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_STRUCT   Ctrl;

  //
  // @todo: These CR accesses should get absorbed into the HAL
  //

  Ctrl.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_REG);
  Ctrl.Bits.xt_soft_reset = MRC_XTENSA_SOFT_RESET;
  MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_REG, Ctrl.Data);

  // Poll until reset bit is cleared
  Timeout = MrcData->Inputs.Call.Func->MrcGetCpuTime() + MRC_WAIT_TIMEOUT;
  do {
    Ctrl.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_REG);
    Busy = (BOOLEAN)Ctrl.Bits.xt_soft_reset; // Poll until bit is cleared

    // COMMENT THIS BLOCK FOR HSLE BUILD
    if (MrcData->Inputs.ExtInputs.Ptr->SimicsFlag) {
      Ctrl.Bits.xt_soft_reset = 0; // Manually clear reset bit
      MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_REG, Ctrl.Data);
      Busy = FALSE;
    }
  } while (Busy && (MrcData->Inputs.Call.Func->MrcGetCpuTime () < Timeout));

  if (Busy) {
    return mrcTimeout;
  }

  return mrcSuccess;
}

/**
  Set Xtensa's FW download done bit.

  @param[in] MrcData - Include all MRC global data.

  @retval returns mrcSuccess.
**/
MrcStatus
BlueMrcSetXtensaFwDownloadDone (
  IN MrcParameters *const MrcData
  )
{
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_FW_DNLD_CTRL_STRUCT DownloadCtrl;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_STRUCT      Ctrl;

  Ctrl.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_REG);
  Ctrl.Bits.xt_debug_en = TRUE;
  MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_CTRL_REG, Ctrl.Data);

  //
  // @todo: These CR accesses should get absorbed into the HAL
  //
  DownloadCtrl.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_FW_DNLD_CTRL_REG);
  DownloadCtrl.Bits.fw_dnld_done = MRC_XTENSA_FW_DOWNLOAD_COMPLETE;

  DownloadCtrl.Bits.sram_active_arb_en = TRUE;


  MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_FW_DNLD_CTRL_REG, DownloadCtrl.Data);

  return mrcSuccess;
}

/**
  Sets Xtensa's near and far SRAMs to a known value.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess - SRAM was preloaded
  @retval mrcTimeout - Timed out during SRAM preload
**/
MrcStatus
BlueMrcClearXtensaSram (
  IN MrcParameters *const MrcData
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcInput            *Inputs;
  UINT64              Timeout;
  BOOLEAN             Busy;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_START_ADDR_STRUCT  StartAddr;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_STOP_ADDR_STRUCT   StopAddr;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;

  // Zero out the top 32 KB of DRAM0 manually: [64..96KB]
  BlueMrcClearXtensaDramTop (MrcData);

  // Preload data is all zeroes
  MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_DATA_REG, 0);

  // FE SRAM - start from address 0
  StartAddr.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_START_ADDR_REG);
  StartAddr.Bits.direct_load_start_addr = 0;
  MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_START_ADDR_REG, StartAddr.Data);

  // Preload FE SRAM - stop at the last address: 52 slices x 4KB DWORDs
  // Preload NE SRAM - HW will clear the full range of near-end SRAMs
  // Both SRAM's are preloaded by HW in parallel
  StopAddr.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_STOP_ADDR_REG);
  StopAddr.Bits.direct_load_stop_addr = (52 * 4096) - 1;  // 0x33FFF
  StopAddr.Bits.direct_load_decrement = 0;                // Incremental order
  StopAddr.Bits.start_direct_load     = 1;                // Start FE SRAM preload
  StopAddr.Bits.ne_start_direct_load  = 1;                // Start NE SRAM preload
  MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_STOP_ADDR_REG, StopAddr.Data);

  // Poll until FE/NE SRAM preload is done
  Timeout = MrcCall->MrcGetCpuTime() + MRC_WAIT_TIMEOUT;
  do {
    StopAddr.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_DRCT_LOAD_STOP_ADDR_REG);
    Busy = (StopAddr.Bits.start_direct_load == 1) || (StopAddr.Bits.ne_start_direct_load == 1); // Poll until both are done
    if (Inputs->ExtInputs.Ptr->SimicsFlag) {
      Busy = FALSE;
    }
  } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));
  if (Busy) {
    return mrcTimeout;
  }

  return mrcSuccess;
}

/**
  Check xtensa status registers, print any error conditions.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess - if no fatal error occurs, otherwise return mrcFail.
**/
MrcStatus
BlueMrcCheckXtStatusRegs (
  IN MrcParameters *const MrcData
  )
{
  MrcDebug *Debug;
  MrcStatus Status;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_STATUS_STRUCT       XtStatus;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_PFAULTINFO0_STRUCT  Pfault0;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_PFAULTINFO1_STRUCT  Pfault1;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_NE_ECC_STATUS_STRUCT   NeEccStatus;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_FE_ECC_STATUS_STRUCT   FeEccStatus;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_ECC_CTRL_STRUCT        UcssEccCtrl;

  Debug = &MrcData->Outputs.Debug;
  Status = mrcSuccess;

  XtStatus.Data    = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_STATUS_REG);
  NeEccStatus.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_NE_ECC_STATUS_REG);
  FeEccStatus.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_FE_ECC_STATUS_REG);

  if (XtStatus.Bits.xt_pfatalerror == 1) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", gErrString, "XT_STATUS: xt_pfatalerror");
    Status = mrcFail;
  }
  if (XtStatus.Bits.xt_double_exception_error == 1) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", gErrString, "XT_STATUS: xt_double_exception_error");
    Status = mrcFail;
  }
  // uncorrectable error on dram0
  if (NeEccStatus.Bits.ne_dram0_ecc_uncorr_err) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s %s\n", gErrString , "DRAM0", "uncorrectable ECC error");
    Status = mrcFail;
  }
  // uncorrectable error on iram0
  if (NeEccStatus.Bits.ne_iram0_ecc_uncorr_err) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s %s\n", gErrString, "IRAM0", "uncorrectable ECC error");
    Status = mrcFail;
  }
  // uncorrectable error on iram1
  if (NeEccStatus.Bits.ne_iram1_ecc_uncorr_err) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s %s\n", gErrString, "IRAM1", "uncorrectable ECC error");
    Status = mrcFail;
  }
  // uncorrectable error on FE SRAM
  if (FeEccStatus.Bits.fe_ecc_uncorr_err) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s %s\n", gErrString, "FE SRAM", "uncorrectable ECC error");
    Status = mrcFail;
  }

  Pfault0.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_PFAULTINFO0_REG);
  if (Pfault0.Bits.pfaultinfo0 != 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s XT_PFAULTIFO%u: 0x%x\n", gErrString, 0, Pfault0.Data);
    // Fault severity level (bits 8:15)
    // 0000: Fault Severity is not reset. The system can recover without resetting.
    // 0001: Fault Severity is core reset. Minimally,reset the Xtensa processor to recover from the fault.
    // 0010: Fault Severity is system reset. The whole system may need to be reset to recover from the fault.
    if (Pfault0.Data & MRC_BIT8) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Severity is %s Reset\n", "Core");
      Status = mrcFail;
    }
    if (Pfault0.Data & MRC_BIT9) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Severity is %s Reset\n", "System");
      Status = mrcFail;
    }

    Pfault1.Data = MrcReadCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_XT_PFAULTINFO1_REG);
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "XT_PFAULTIFO%u: 0x%x\n", 1, Pfault0.Data);
    // Pfault Bits (32:63)
    // Bit 47 (map to bit 15) PrefetchRAM uncorrectable error
    // Bit 43 (map to bit 11) InstCache uncorrectable error
    // Bit 39 (map to bit 7)  DataCache uncorrectable error
    // Bit 35 (map to bit 3) InstRAM uncorrectable error
    // Bit 33 (map to bit 1) DataRAM uncorrectable error
    if (Pfault1.Data & MRC_BIT1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", "DataRAM", "uncorrectable error");
      Status = mrcFail;
    }
    if (Pfault1.Data & MRC_BIT3) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", "InstRAM", "uncorrectable error");
      Status = mrcFail;
    }
    if (Pfault1.Data & MRC_BIT7) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", "DataCache", "uncorrectable errorr");
      Status = mrcFail;
    }
    if (Pfault1.Data & MRC_BIT11) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", "InstCache", "uncorrectable error");
      Status = mrcFail;
    }
    if (Pfault1.Data & MRC_BIT15) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", "PrefetchRAM", "uncorrectable error");
      Status = mrcFail;
    }
  }

  // ECC (NE,FE) Correctable Errors will be treated as Warnings

  if (NeEccStatus.Bits.ne_iram0_ecc_corr_count != 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "%s %s ECC CORR ERR COUNT: %u\n", gWarnString, "IRAM0", NeEccStatus.Bits.ne_iram0_ecc_corr_count);
  }
  if (NeEccStatus.Bits.ne_iram1_ecc_corr_count != 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "%s %s ECC CORR ERR COUNT: %u\n", gWarnString, "IRAM1", NeEccStatus.Bits.ne_iram1_ecc_corr_count);
  }
  if (NeEccStatus.Bits.ne_dram0_ecc_corr_count != 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "%s %s ECC CORR ERR COUNT: %u\n", gWarnString, "DRAM0", NeEccStatus.Bits.ne_dram0_ecc_corr_count);
  }
  // if we have any NE corr ecc err - clear it.
  if ((NeEccStatus.Bits.ne_iram0_ecc_corr_count != 0) ||
      (NeEccStatus.Bits.ne_iram1_ecc_corr_count != 0) ||
      (NeEccStatus.Bits.ne_dram0_ecc_corr_count != 0)
     ) {
    // clear NE ECC correctable err count
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "XT %s Clear ECC CORR ERR COUNT\n", "NE");
    UcssEccCtrl.Data = MrcReadCR (MrcData,  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_ECC_CTRL_REG);
    UcssEccCtrl.Bits.ecc_ne_corr_err_count_clr = 1;
    MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_ECC_CTRL_REG, UcssEccCtrl.Data);
  }

  if (FeEccStatus.Bits.fe_ecc_corr_count != 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "%s %s ECC CORR ERR COUNT: %u\n", gWarnString, "FE SRAM", FeEccStatus.Bits.fe_ecc_corr_count);
    // clear FE ECC correctable err count
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "XT %s Clear ECC CORR ERR COUNT\n", "FE");
    UcssEccCtrl.Data = MrcReadCR (MrcData,  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_ECC_CTRL_REG);
    UcssEccCtrl.Bits.ecc_fe_corr_err_count_clr = 1;
    MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_ECC_CTRL_REG, UcssEccCtrl.Data);
  }

  return Status;
}

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
  )
{
#ifdef MRC_DEBUG_PRINT
  MrcDebug    *Debug;
  MrcDebugMsgLevel Lvl;
  UINT8       LocalBuffer[MRC_GREEN_LOG_SIZE + 1];
  UINT32      NewLogSize;
  UINT32      i;
  MrcPrintBuf PrintBuf;

  Debug = &MrcData->Outputs.Debug;
  Lvl = Debug->Level & ~MSG_LEVEL_CSV & ~MSG_LEVEL_MMIO; //Clear CSV and MMIO bits.

  if (LogSize > MRC_GREEN_LOG_SIZE) {
    LogSize = MRC_GREEN_LOG_SIZE;
  }


  if (MrcData->Inputs.IsCrBasedCommunication) {
    i = 0;
    PrintBuf.Data = MrcReadCR (MrcData, MRC_PRINT_BUF_CR_REG);
    // If this function was called, it means we have at least one character in the buffer
    LocalBuffer[i++] = (UINT8) PrintBuf.Bits.Byte0;

    if (PrintBuf.Bits.Size > 1) {
      LocalBuffer[i++] = (UINT8) PrintBuf.Bits.Byte1;
    }
    if (PrintBuf.Bits.Size > 2) {
      LocalBuffer[i++] = (UINT8) PrintBuf.Bits.Byte2;
    }
    LocalBuffer[i++] = 0; // EOL char
    // Indicate to Green that we have consumed the buffer
    MrcWriteCR (MrcData, MRC_PRINT_BUF_CR_REG, 0);
  } else {
    //
    // Buffer is ours until we clear the LogSize.
    // Read the log into a local buffer, then release it to unblock the UC.
    // Then print from the local buffer. String formatting already happened.
    //
    // Fetch only the populated portion of the LogBuffer
    BlueMrcReadUcData (MrcData, gUcCommBufferAddress + MRC_BLUE_GREEN_COMM_CONTROLS_SIZE, (UINT32 *) LocalBuffer, LogSize);
    LocalBuffer[LogSize] = 0;     // Add the NULL terminator
    NewLogSize = 0;

    if (MrcData->Inputs.ExtInputs.Ptr->SimicsFlag) {
      // Simics needs a halt before Blue MRC writes to SRAM while Xtensa is running
      BlueMrcSetXtensaHardwareHaltState(MrcData, MRC_XTENSA_HW_HALT);
    }
    // This will allow Green MRC to send the next log portion
    BlueMrcWriteUcData (MrcData, &NewLogSize, gUcCommBufferAddress + MRC_BLUE_GREEN_COMM_LOG_SIZE_OFFSET, MRC_BLUE_GREEN_COMM_LOG_SIZE_SIZE);
    if (MrcData->Inputs.ExtInputs.Ptr->SimicsFlag) {
      BlueMrcSetXtensaHardwareHaltState(MrcData, MRC_XTENSA_HW_UNHALT);
    }
  }
  // need to print all Green logs
  MRC_DEBUG_MSG(Debug, Lvl, "%s", LocalBuffer);

  return mrcSuccess;
#else
    return mrcSuccess;
#endif // MRC_DEBUG_PRINT
  }
#endif // FULL_HEADLESS
