/** @file
  File to support address decoding and encoding

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
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/NguInfoLib.h>    // GET_HBO_EFFECTIVE_ADDRESS
#include <Protocol/MemAddrDecodeEncode.h>


#define MAX_SUBCH            (2)

// MC0
#define MC0_MAD_INTER_CHANNEL_REG                                      (0x0000D800)
#define MC0_MAD_INTRA_CH0_REG                                          (0x0000D804)
#define MC0_MAD_INTRA_CH1_REG                                          (0x0000D808)
#define MC0_MAD_MC_HASH_REG                                            (0x0000D9B8)
#define MC0_MAD_CHANNEL_HASH_REG                                       (0x0000D824)
#define MC0_MAD_SUB_CHANNEL_HASH_REG                                   (0x0000D828)
#define MC0_MAD_DRAM_HASH_0_REG                                        (0x0000D80C)
#define MC0_MAD_DRAM_HASH_1_REG                                        (0x0000D810)

// MC1
#define MC1_MAD_INTER_CHANNEL_REG                                      (0x0001D800)
#define MC1_MAD_INTRA_CH0_REG                                          (0x0001D804)
#define MC1_MAD_INTRA_CH1_REG                                          (0x0001D808)
#define MC1_MAD_MC_HASH_REG                                            (0x0001D9B8)
#define MC1_MAD_CHANNEL_HASH_REG                                       (0x0001D824)
#define MC1_MAD_SUB_CHANNEL_HASH_REG                                   (0x0001D828)
#define MC1_MAD_DRAM_HASH_0_REG                                        (0x0001D80C)
#define MC1_MAD_DRAM_HASH_1_REG                                        (0x0001D810)
#define REMAP_BASE_HBO_MEM_REG                                         (0x00000208U)
#define REMAP_LIMIT_HBO_MEM_REG                                        (0x00000210U)
#define MEMORY_SLICE_HASH_HBO_MEM_REG                                  (0x00000200U)

VOID pa_decoder_so( //inputs
                   UINT64 remap_base,
                   UINT64 remap_limit,
                   UINT64 tolud,
                   UINT32 memory_slice_hash,
                   UINT64 physical_address,
                   //outputs
                   UINT64* address,
                   UINT32* mc_id);

BOOLEAN decoder_so( // inputs
                   UINT32 mad_inter_channel,
                   UINT32 mad_intra_channel_0,
                   UINT32 mad_intra_channel_1,
                   UINT32 mad_mc_hash,
                   UINT32 mad_channel_hash,
                   UINT32 mad_sub_channel_hash,
                   UINT32 mad_dram_hash_0,
                   UINT32 mad_dram_hash_1,
                   UINT64 address,
                   // outputs
                   UINT32* ch,
                   UINT32* sub_ch,
                   UINT32* rank,
                   UINT32* bg,
                   UINT32* bank,
                   UINT32* row,
                   UINT32* column);

// encoder function for shared object call
BOOLEAN encoder_so( // inputs
                  UINT32 mad_inter_channel,
                  UINT32 mad_intra_channel_0,
                  UINT32 mad_intra_channel_1,
                  UINT32 mad_mc_hash,
                  UINT32 mad_channel_hash,
                  UINT32 mad_sub_channel_hash,
                  UINT32 mad_dram_hash_0,
                  UINT32 mad_dram_hash_1,
                  UINT64 remap_base,
                  UINT64 remap_limit,
                  UINT64 tolud,
                  UINT32 memory_slice_hash,
                  UINT32 mc,
                  UINT32 ch,
                  UINT32 sub_ch,
                  UINT32 rank,
                  UINT32 bg,
                  UINT32 bank,
                  UINT32 row,
                  UINT32 column,
                  //output
                  UINT64* address);

VOID MrcGetCommonConfigRegisters(
                     UINT64 *remap_base,
                     UINT64 *remap_limit,
                     UINT64 *tolud,
                     UINT32 *memory_slice_hash)
{
  UINT64 MchBarBaseAddress;

  MchBarBaseAddress  = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  *tolud             = GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase);

  *remap_base        = MmioRead64(MchBarBaseAddress + GET_HBO_EFFECTIVE_ADDRESS (REMAP_BASE_HBO_MEM_REG));
  *remap_limit       = MmioRead64(MchBarBaseAddress + GET_HBO_EFFECTIVE_ADDRESS (REMAP_LIMIT_HBO_MEM_REG));

  *memory_slice_hash = MmioRead32 (MchBarBaseAddress + GET_HBO_EFFECTIVE_ADDRESS(MEMORY_SLICE_HASH_HBO_MEM_REG));
}

VOID MrcGetConfigRegisters(
                     UINT32 mc_id,
                     UINT32 *mad_inter_channel,
                     UINT32 *mad_intra_channel_0,
                     UINT32 *mad_intra_channel_1,
                     UINT32 *mad_mc_hash,
                     UINT32 *mad_channel_hash,
                     UINT32 *mad_sub_channel_hash,
                     UINT32 *mad_dram_hash_0,
                     UINT32 *mad_dram_hash_1)

{
  UINT64 MchBarBaseAddress;

  MchBarBaseAddress  = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  if (mc_id ==0) {
    *mad_inter_channel      = MmioRead32 (MchBarBaseAddress + MC0_MAD_INTER_CHANNEL_REG);
    *mad_intra_channel_0    = MmioRead32 (MchBarBaseAddress + MC0_MAD_INTRA_CH0_REG);
    *mad_intra_channel_1    = MmioRead32 (MchBarBaseAddress + MC0_MAD_INTRA_CH1_REG);
    *mad_mc_hash            = MmioRead32 (MchBarBaseAddress + MC0_MAD_MC_HASH_REG);
    *mad_channel_hash       = MmioRead32 (MchBarBaseAddress + MC0_MAD_CHANNEL_HASH_REG);
    *mad_sub_channel_hash   = MmioRead32 (MchBarBaseAddress + MC0_MAD_SUB_CHANNEL_HASH_REG);
    *mad_dram_hash_0        = MmioRead32 (MchBarBaseAddress + MC0_MAD_DRAM_HASH_0_REG);
    *mad_dram_hash_1        = MmioRead32 (MchBarBaseAddress + MC0_MAD_DRAM_HASH_1_REG);
  } else {
    *mad_inter_channel      = MmioRead32 (MchBarBaseAddress + MC1_MAD_INTER_CHANNEL_REG);
    *mad_intra_channel_0    = MmioRead32 (MchBarBaseAddress + MC1_MAD_INTRA_CH0_REG);
    *mad_intra_channel_1    = MmioRead32 (MchBarBaseAddress + MC1_MAD_INTRA_CH1_REG);
    *mad_mc_hash            = MmioRead32 (MchBarBaseAddress + MC1_MAD_MC_HASH_REG);
    *mad_channel_hash       = MmioRead32 (MchBarBaseAddress + MC1_MAD_CHANNEL_HASH_REG);
    *mad_sub_channel_hash   = MmioRead32 (MchBarBaseAddress + MC1_MAD_SUB_CHANNEL_HASH_REG);
    *mad_dram_hash_0        = MmioRead32 (MchBarBaseAddress + MC1_MAD_DRAM_HASH_0_REG);
    *mad_dram_hash_1        = MmioRead32 (MchBarBaseAddress + MC1_MAD_DRAM_HASH_1_REG);
  }
}

/**
  Address decode function
  Converts memory address to DRAM address

  @param[in]      MemoryAddress  - The 39-bit memory address to decode.
  @param[out]     DramAddress    - The DRAM address struct that the memory address decodes to.

  @retval Returns EFI_SUCCESS if successful, EFI_UNSUPPORTED otherwise.
 **/
EFI_STATUS
EFIAPI MrcMemoryAddressDecode (
    IN  UINT64          MemoryAddress,
    OUT DRAM_ADDRESS    *DramAddress
    )
{
  UINT32 mad_inter_channel;
  UINT32 mad_intra_channel_0;
  UINT32 mad_intra_channel_1;
  UINT32 mad_mc_hash;
  UINT32 mad_channel_hash;
  UINT32 mad_sub_channel_hash;
  UINT32 mad_dram_hash_0;
  UINT32 mad_dram_hash_1;
  UINT64 remap_base;
  UINT64 remap_limit;
  UINT64 tolud;
  UINT32 memory_slice_hash;

  UINT32 mc;
  UINT32 ch;
  UINT32 sub_ch;
  UINT32 rank;
  UINT32 bg;
  UINT32 bank;
  UINT32 row;
  UINT32 column;

  UINT64 temp_address;

  BOOLEAN Status;

  UINT32 Ddr_Type;

  MrcGetCommonConfigRegisters(
                   &remap_base,
                   &remap_limit,
                   &tolud,
                   &memory_slice_hash);

  pa_decoder_so(
               remap_base,
               remap_limit,
               tolud,
               memory_slice_hash,
               MemoryAddress,
               &temp_address,
               &mc);

  MrcGetConfigRegisters(
                   mc,
                   &mad_inter_channel,
                   &mad_intra_channel_0,
                   &mad_intra_channel_1,
                   &mad_mc_hash,
                   &mad_channel_hash,
                   &mad_sub_channel_hash,
                   &mad_dram_hash_0,
                   &mad_dram_hash_1);

  Status = decoder_so(
                   mad_inter_channel,
                   mad_intra_channel_0,
                   mad_intra_channel_1,
                   mad_mc_hash,
                   mad_channel_hash,
                   mad_sub_channel_hash,
                   mad_dram_hash_0,
                   mad_dram_hash_1,
                   temp_address,
                   // outputs
                   &ch,
                   &sub_ch,
                   &rank,
                   &bg,
                   &bank,
                   &row,
                   &column);

  Ddr_Type         = mad_inter_channel & 0x3; // bit0,1 is dram type, 0 is Lp5, 1 is DDR5

  if (Status) {
    DramAddress->Socket        = 0;
    DramAddress->Controller    = mc;
    if (Ddr_Type == 0) {
      DramAddress->ChannelNumber = ch * MAX_SUBCH + sub_ch; //LP5 channel is ch *2 + sub_ch.
      DramAddress->DimmNumber    = 0;                            //Dimm always 0
    } else {
      DramAddress->ChannelNumber = ch;                           //Ddr5 channel is ch.
      DramAddress->DimmNumber    = sub_ch;                       //Dimm is sub_ch
    }
    DramAddress->Rank          = rank;
    DramAddress->BankGroup     = bg;
    DramAddress->Bank          = bank;
    DramAddress->Ras           = row;
    DramAddress->Cas           = column;
  }

  return Status ? EFI_SUCCESS : EFI_UNSUPPORTED;
}

/**
  Address encode function (reverse address decode)
  DRAM address to memory address conversion

  @param[in]      DramAddress    - The DRAM address that is to be encoded.
  @param[out]     MemoryAddress  - The 39-bit memory address to convert to.

  @retval Returns EFI_SUCCESS if successful, EFI_UNSUPPORTED otherwise.
 **/
EFI_STATUS
EFIAPI MrcMemoryAddressEncode (
    IN  DRAM_ADDRESS *DramAddress,
    OUT UINT64       *MemoryAddress
    )
{
  UINT32 mad_inter_channel;
  UINT32 mad_intra_channel_0;
  UINT32 mad_intra_channel_1;
  UINT32 mad_mc_hash;
  UINT32 mad_channel_hash;
  UINT32 mad_sub_channel_hash;
  UINT32 mad_dram_hash_0;
  UINT32 mad_dram_hash_1;
  UINT64 remap_base;
  UINT64 remap_limit;
  UINT64 tolud;
  UINT32 memory_slice_hash;

  BOOLEAN Status;

  UINT32 Ddr_Type;

  UINT32 mc;
  UINT32 ch;
  UINT32 sub_ch;
  UINT32 rank;
  UINT32 bg;
  UINT32 bank;
  UINT32 row;
  UINT32 column;

  mc   = DramAddress->Controller;
  rank = DramAddress->Rank;
  bg   = DramAddress->BankGroup;
  bank = DramAddress->Bank;
  row  = DramAddress->Ras;
  column  = DramAddress->Cas;

  MrcGetCommonConfigRegisters(
                   &remap_base,
                   &remap_limit,
                   &tolud,
                   &memory_slice_hash);

  MrcGetConfigRegisters(
                   mc,
                   &mad_inter_channel,
                   &mad_intra_channel_0,
                   &mad_intra_channel_1,
                   &mad_mc_hash,
                   &mad_channel_hash,
                   &mad_sub_channel_hash,
                   &mad_dram_hash_0,
                   &mad_dram_hash_1);

  Ddr_Type         = mad_inter_channel & 0x3; // bit0,1 is dram type, 0 is Lp5, 1 is DDR5

  if (Ddr_Type == 0) {  //LP5
    ch = DramAddress->ChannelNumber / MAX_SUBCH;
    sub_ch = DramAddress->ChannelNumber % MAX_SUBCH;
  }else {  //DDR5
    ch = DramAddress->ChannelNumber;
    sub_ch = DramAddress->DimmNumber;
  }

  Status = encoder_so(// Inputs
                   mad_inter_channel,
                   mad_intra_channel_0,
                   mad_intra_channel_1,
                   mad_mc_hash,
                   mad_channel_hash,
                   mad_sub_channel_hash,
                   mad_dram_hash_0,
                   mad_dram_hash_1,
                   remap_base,
                   remap_limit,
                   tolud,
                   memory_slice_hash,
                   mc,
                   ch,
                   sub_ch,
                   rank,
                   bg,
                   bank,
                   row,
                   column,
                   //output
                   MemoryAddress);


  return Status ? EFI_SUCCESS : EFI_UNSUPPORTED;
}