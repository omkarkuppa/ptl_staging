/** @file
  Core decode encode functions

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

#include "decoder.h"           // strcuts and functions
#include "decoder_tables.h"    // address decode tables
#include "decoder_registers.h" // register field offsets

// Local functions
VOID       get_config(registers*, config*);                                                                                              // Extract the config structure from the regsiters
BOOLEAN    check_config(config*);                                                                                                        // Check configuration
VOID       print_config(config*);                                                                                                        // Print the config structure in human readable format
VOID       get_ranges(CPU_FAMILY, tech, BOOLEAN, BOOLEAN, BOOLEAN, range*);                                                              // Get ranges for each field in the DRAM address table
BOOLEAN    decode(config*, UINT64, dram_address*);                                                                                       // Main function to perform DRAM address decode
BOOLEAN    encode(config*, dram_address*, UINT64*);                                                                                      // Main function to perform DRAM address encode
slice      get_zone_address(UINT64, UINT64, int, int, UINT64);                                                                           // Auxilary function for zone0/zone1 address decode calculation
UINT64     get_upper_level_address(UINT64, UINT64, UINT32, int, int, UINT64);                                                            // Auxilary function for zone0/zone1 address encode calculation
VOID       gen_registers(UINT32, UINT32, UINT32, UINT32, UINT32, UINT32, UINT32, UINT32, UINT64, UINT64, UINT64, UINT32, registers*);    // Group registers to struct

/*
   heper function to get supported CPU family
*/
CPU_FAMILY GetTargetPlatform(void)
{
  CPUID_VERSION_INFO_EAX  Eax;

  ///
  /// Read the CPUID & DID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, &Eax.Uint32, NULL, NULL, NULL);

  switch (Eax.Uint32 & CPUID_FULL_FAMILY) {
    case CPUID_FULL_FAMILY_PANTHERLAKE:
        DECODER_DEBUG("Detected Ptl CPU: 0x08%x\n", Eax.Uint32 & CPUID_FULL_FAMILY_MODEL);
        return EnumCpuPtl;
    default:
        DECODER_ERROR("Detected unknown CPU: 0x08%x\n", Eax.Uint32 & CPUID_FULL_FAMILY_MODEL);
        return EnumCpuUnknown;
  }
}

// decode function for shared object call
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
                   UINT32* column)
{
   registers    regs; // registers
   config       cfg; // config structure
   dram_address result; // DRAM address

   gen_registers(mad_inter_channel,
                 mad_intra_channel_0,
                 mad_intra_channel_1,
                 mad_mc_hash,
                 mad_channel_hash,
                 mad_sub_channel_hash,
                 mad_dram_hash_0,
                 mad_dram_hash_1,
                 0, 0, 0, 0, &regs);

   get_config(&regs, &cfg);

   if( !check_config(&cfg)) {
      return FALSE;
   }

   print_config(&cfg);

   if (!decode(&cfg, address, &result)) { // Note: Need to decode all address bits.
      return FALSE;
   }

   *ch     = result.ch;
   *sub_ch = result.sub_ch;
   *rank   = result.rank;
   *bg     = result.bg;
   *bank   = result.bank;
   *row    = result.row;
   *column = result.column;

   return TRUE;
}

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
                  UINT64* address)
{
   registers    regs; // registers
   config       cfg; // config structure
   dram_address result; // DRAM address

   gen_registers(mad_inter_channel,
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
                 &regs);

   get_config(&regs, &cfg);

   if (!check_config(&cfg)) {
      return FALSE;
   }

   print_config(&cfg);

   result.mc     = mc;
   result.ch     = ch;
   result.sub_ch = sub_ch;
   result.rank   = rank;
   result.bg     = bg;
   result.bank   = bank;
   result.row    = row;
   result.column = column;

   if (!encode(&cfg, &result, address)) {//Call encode function
      return FALSE;
   }

   return TRUE;
}

//Get system address from physical address
VOID pa_decoder_so( //inputs
                   UINT64 remap_base,
                   UINT64 remap_limit,
                   UINT64 tolud,
                   UINT32 memory_slice_hash,
                   UINT64 physical_address,
                   //outputs
                   UINT64* address,
                   UINT32* mc_id) {

   UINT32 hash_mask   = range32(memory_slice_hash,MEMORY_SLICE_HASH_HASH_MASK);
   UINT64 zone1_start = LShiftU64((UINT64)range32(memory_slice_hash, MEMORY_SLICE_HASH_ZONE1_START), ZONE1_START_UNIT);

   *address = physical_address;

   //REMAP
   if ((physical_address >= LShiftU64(range64(remap_base,REMAP_BASE), REMAP_UNIT)) &&
       (physical_address < LShiftU64(range64(remap_limit,REMAP_LIMIT), REMAP_UNIT))) {
        *address -= LShiftU64(range64(tolud,TOLUD), REMAP_UNIT);
   }

   if ((range32(memory_slice_hash, MEMORY_SLICE_HASH_HASH_ENABLED) == 0) || (physical_address >= zone1_start)) {
      *mc_id = range32(memory_slice_hash, MEMORY_SLICE_HASH_SLICE_L_ID); // address in zone1
   } else {
      *mc_id = xor(physical_address, hash_mask << HASH_OFFSET); //address in zone0
   }
}

//populate registers struct
VOID gen_registers( // inputs
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
                   // output
                   registers* regs) {
   regs->mad_inter_channel    = mad_inter_channel;
   regs->mad_intra_channel[0] = mad_intra_channel_0;
   regs->mad_intra_channel[1] = mad_intra_channel_1;
   regs->mad_mc_hash          = mad_mc_hash;
   regs->mad_channel_hash     = mad_channel_hash;
   regs->mad_sub_channel_hash = mad_sub_channel_hash;
   regs->mad_dram_hash[0]     = mad_dram_hash_0;
   regs->mad_dram_hash[1]     = mad_dram_hash_1;
   regs->remap_base           = remap_base;
   regs->remap_limit          = remap_limit;
   regs->tolud                = tolud;
   regs->memory_slice_hash    = memory_slice_hash;

}

// Extract the config structure from the regsiters
// mostly using range32 function to extract fields from registers
VOID get_config(registers* regs, config* cfg)
{
   int i;
   cfg->family           = GetTargetPlatform();
   cfg->ddr_type         = range32(regs->mad_inter_channel, MAD_INTER_CHANNEL_DDR_TYPE);
   cfg->ecc              = (BOOLEAN)range32(regs->mad_inter_channel, MAD_INTER_CHANNEL_ECC);
   cfg->ch_size[0]       = range32(regs->mad_inter_channel, MAD_INTER_CHANNEL_CH0_SIZE);
   cfg->ch_size[1]       = range32(regs->mad_inter_channel, MAD_INTER_CHANNEL_CH1_SIZE);
   cfg->mc_hash_enabled  = (BOOLEAN)range32(regs->mad_mc_hash, MAD_MC_HASH_HASH_ENABLED);
   cfg->mc_hash_lsb      = range32(regs->mad_mc_hash, MAD_MC_HASH_HASH_LSB);
   cfg->zone1_start      = range32(regs->mad_mc_hash, MAD_MC_HASH_ZONE1_START);
   cfg->ch_hash_mask     = range32(regs->mad_channel_hash, MAD_CHANNEL_HASH_HASH_MASK);
   cfg->ch_hash_lsb      = range32(regs->mad_channel_hash, MAD_CHANNEL_HASH_HASH_LSB);
   cfg->ch_hash_mask    |= 1<<cfg->ch_hash_lsb;
   if (cfg->ddr_type == ddr5) {
      cfg->sub_ch_hash_mask = 0;
      cfg->sub_ch_hash_lsb  = 17 - HASH_OFFSET;
   } else {
      cfg->sub_ch_hash_mask = range32(regs->mad_sub_channel_hash, MAD_SUB_CHANNEL_HASH_HASH_MASK);
      cfg->sub_ch_hash_lsb  = range32(regs->mad_sub_channel_hash, MAD_SUB_CHANNEL_HASH_HASH_LSB);
   }
   cfg->sub_ch_hash_mask|= 1<<cfg->sub_ch_hash_lsb;
   for (i = 0; i < NUM_CH; i++) { //loop over per channel registers
      cfg->device_density[i][0] = range32(regs->mad_intra_channel[i], MAD_INTRA_CHANNEL_SUBCH_0_DENSITY);
      cfg->device_density[i][1] = range32(regs->mad_intra_channel[i], MAD_INTRA_CHANNEL_SUBCH_1_DENSITY);
      cfg->device_width[i][0]   = (UINT8)range32(regs->mad_intra_channel[i], MAD_INTRA_CHANNEL_SUBCH_0_WIDTH);
      cfg->device_width[i][1]   = (UINT8)range32(regs->mad_intra_channel[i], MAD_INTRA_CHANNEL_SUBCH_1_WIDTH);
      cfg->ranks[i][0]          = (UINT8)range32(regs->mad_intra_channel[i], MAD_INTRA_CHANNEL_SUBCH_0_RANKS);
      cfg->ranks[i][1]          = (UINT8)range32(regs->mad_intra_channel[i], MAD_INTRA_CHANNEL_SUBCH_1_RANKS);
      cfg->eim[i]               = (BOOLEAN)range32(regs->mad_dram_hash[i], MAD_DRAM_HASH_EIM);
      cfg->ebh[i]               = (BOOLEAN)range32(regs->mad_dram_hash[i], MAD_DRAM_HASH_EBH);
      cfg->bg0_hash[i]          = range32(regs->mad_dram_hash[i], MAD_DRAM_HASH_BG0);
      cfg->bg1_hash[i]          = range32(regs->mad_dram_hash[i], MAD_DRAM_HASH_BG1);
      cfg->bg2_hash[i]          = range32(regs->mad_dram_hash[i], MAD_DRAM_HASH_BG2);
      cfg->onedpc[i]            = cfg->family == EnumCpuPtl && cfg->ddr_type == ddr5
                                  && cfg->device_density[i][1] == 0 && cfg->ranks[i][0];
   }

   //Additional optional registers (for encoding)
   cfg->remap_base       = range64(regs->remap_base, REMAP_BASE);
   cfg->remap_limit      = range64(regs->remap_limit, REMAP_LIMIT);
   cfg->tolud            = range64(regs->tolud, TOLUD);
   cfg->mc_hash_mask     = range32(regs->memory_slice_hash, MEMORY_SLICE_HASH_HASH_MASK);
   cfg->slice_l_id       = (UINT8)range32(regs->memory_slice_hash, MEMORY_SLICE_HASH_SLICE_L_ID);

   //fix ch_size in case it is zero to match mad_intra_channel configuration
   for (i = 0; i < NUM_CH; ++i) {
      if (cfg->ch_size[i] == 0) {
         cfg->ch_size[i] = (cfg->device_density[i][0] << (cfg->device_width[i][0] + cfg->ranks[i][0] + (int)cfg->ddr_type)) +
                           (cfg->device_density[i][1] << (cfg->device_width[i][1] + cfg->ranks[i][1] + (int)cfg->ddr_type));
      }
   }
}

// Check configuration
BOOLEAN check_config(config* cfg)
{
   UINT8 i,j;

   UINT32 sub_ch_size[NUM_CH][NUM_SUB_CH]; //holds sub channel sizes in 0.5GB

   //At least one channel needs to be occupied
   if ((cfg->ch_size[0] == 0) &&
       (cfg->ch_size[1] == 0)) {
      DECODER_ERROR("none of the channels are occupied\n");
      return FALSE;
   }

   //MAD_INTER_CHANNEL channel sizes match calculation from MAD_INTER_CHANNEL registers
   for (i = 0; i < NUM_CH; i++) {
      for (j = 0; j< NUM_SUB_CH; j++) {
         sub_ch_size[i][j] = cfg->device_density[i][j] << (cfg->device_width[i][j] + cfg->ranks[i][j] + (int)cfg->ddr_type);
      }
      if (cfg->ch_size[i] != (sub_ch_size[i][0] + sub_ch_size[i][1])) {
         DECODER_ERROR("mad_inter_channel.ch%d_size mismatch %d , %d %d\n",i, cfg->ch_size[i], sub_ch_size[i][0], sub_ch_size[i][1]);
         return FALSE;
     }
   }

   //Non supported device densities
   for (i = 0; i < NUM_CH; i++) {
      for (j = 0; j< NUM_SUB_CH; j++) {
         if ((cfg->device_density[i][j] == 1) ||
             (cfg->device_density[i][j] == 5) ||
             (cfg->device_density[i][j] == 7) ||
             (cfg->device_density[i][j] > 8)  ||
             ((cfg->ddr_type == ddr5) &&
              ((cfg->device_density[i][j] == 2) ||
               (cfg->device_density[i][j] == 3)))) {
               DECODER_ERROR("non supported mad_intra_channel[%d].subch_%d_density\n",i,j);
               return FALSE;
         }
      }
   }

   //Hash mask must contain hash LSB
   if (bit(cfg->ch_hash_mask, cfg->ch_hash_lsb) == 0) {
      DECODER_ERROR("mad_channel_hash.hash_mask doesn't have hash_lsb bit set\n");
      return FALSE;
   }
   if (cfg->ddr_type==lpddr5 &&
       bit(cfg->sub_ch_hash_mask,cfg->sub_ch_hash_lsb) == 0) {
      DECODER_ERROR("mad_sub_channel_hash.hash_mask doesn't have hash_lsb bit set\n");
      return FALSE;
   }

   //Hash mask can't contain bits lower than LSB
   if (((cfg->ch_hash_mask & ((1 << cfg->ch_hash_lsb) - 1)) != 0) &&
       (cfg->ch_hash_mask != 0)) {
      DECODER_ERROR("mad_channel_hash.hash_mask contains bits lower than hash_lsb\n");
      return FALSE;
   }
   if (((cfg->sub_ch_hash_mask & ((1 << cfg->sub_ch_hash_lsb) - 1)) != 0) &&
       (cfg->sub_ch_hash_mask != 0)) {
      DECODER_ERROR("mad_sub_channel_hash.hash_mask contains bits lower than hash_lsb\n");
      return FALSE;
   }

   //BG hash allowed ranges
   for (i = 0; i < NUM_CH; i++) {
      if (((cfg->ddr_type == lpddr5) &&
           (cfg->bg0_hash[i] == 6)) ||
          (cfg->bg0_hash[i] == 7)) {
         DECODER_ERROR("mad_dram_hash[%d].bg0_hash illegal configuraiton\n");
         return FALSE;
      }
      if (((cfg->ddr_type == lpddr5) &&
           (cfg->bg1_hash[i]==6)) ||
          (cfg->bg1_hash[i] == 7)) {
         DECODER_ERROR("mad_dram_hash[%d].bg0_hash illegal configuraiton\n");
         return FALSE;
      }
      if (((cfg->ddr_type == lpddr5) &&
           (cfg->bg2_hash[i] != 0)) ||
          (cfg->bg2_hash[i] == 7)) {
         DECODER_ERROR("mad_dram_hash[%d].bg2_hash illegal configuraiton\n");
         return FALSE;
      }
   }

   //EBH mismatched between channels
   if ((cfg->ebh[0] != cfg->ebh[1]) &&
       (cfg->ch_size[0] > 0) &&
       (cfg->ch_size[1] > 0)) {
      DECODER_ERROR("EBH not set the same for both channels\n");
      return FALSE;
   }

   //LPDDR5 symmetry rules
   // Due to FPGA support - channel 0 allows sub channel 0 only occupancy
   for (i = 0; i < NUM_CH; i++) {
      if ((cfg->ddr_type==lpddr5) &&
          (i == 1 || cfg->device_density[i][1] > 0) &&
          ((cfg->device_density[i][0] != cfg->device_density[i][1]) ||
           (cfg->device_width[i][0] != cfg->device_width[i][1]) ||
           (cfg->ranks[i][0] != cfg->ranks[i][1]))) {
         DECODER_ERROR("different devices between sub channels on channel %d\n",i);
         return FALSE;
      }
   }

/*
   //EBH requires all occupied sub channels to be the same
   BOOLEAN sub_ch_occupied[NUM_CH][NUM_SUB_CH];
   UINT32 density = 0; // fill with one of the occupied sub channels detrails (the rest need to match)
   UINT8 width   = 0;
   UINT8 ranks   = 0;
   for (i = 0; i < NUM_CH; i++) {
      for (j = 0; j < NUM_SUB_CH; j++) {
         sub_ch_occupied[i][j] = cfg->device_density[i][j] > 0;
         if (sub_ch_occupied[i][j]) {
            density = cfg->device_density[i][j];
            width   = cfg->device_width[i][j];
            ranks   = cfg->ranks[i][j];
            if ((cfg->ebh[i]) &&
                (cfg->device_density[i][j] > 0) &&
                ((cfg->device_density[i][j] != density) ||
                 (cfg->device_width[i][j] != width) ||
                 (cfg->ranks[i][j] != ranks))) {
               DECODER_ERROR("EBH enabled in non supported configuration\n");
               return FALSE;
            }
         }
      }
   }
*/

   //REMAP checks
   if (cfg->tolud > 0 &&
       cfg->remap_base > 0 &&
       cfg->remap_base >= cfg->remap_limit) {
      DECODER_ERROR("REMAP_LIMIT needs to be larger than REMAP_BASE\n");
      return FALSE;
   }
   if (cfg->remap_base > 0 &&
       cfg->tolud >= cfg->remap_base) {
      DECODER_ERROR("REMAP region needs to be above TOLUD\n");
      return FALSE;
   }
   if (cfg->remap_base > 0 &&
       (cfg->tolud + cfg->remap_limit - cfg->remap_base) > cfg->remap_base) {
      DECODER_ERROR("REMAP region source and destination overlap\n");
      return FALSE;
   }

   if (cfg->ddr_type == ddr5) {
      for (i = 0; i < NUM_CH; i++) {
         if ((cfg->device_density[i][0] > 0) &&
             (cfg->device_density[i][1] == 0) &&
             (cfg->ranks[i][0] == 1) &&
             (cfg->device_width[i][0] != cfg->device_width[i][1])) {
            DECODER_ERROR("In DDR5 1DPC device width of sub channel 1 needs to match device width of sub channel 0");
            return FALSE;
         }
      }
   }

   return TRUE;
}

// Print the config structure in human readable format
VOID print_config(config* cfg)
{
   int i,j;

   DECODER_DEBUG("--------------------------------------------------\n");
   DECODER_DEBUG("technology: %s\n", cfg->ddr_type ? L"ddr5" : L"lpddr5");
   DECODER_DEBUG("memory size: ch[0]=%d%sGB, ch[1]=%d%sGB\n", cfg->ch_size[0] >> 1, cfg->ch_size[0]&1 ? L".5" : L"", cfg->ch_size[1] >> 1, cfg->ch_size[1] & 1 ? L".5" : L"");
   DECODER_DEBUG("MC hash %s", cfg->mc_hash_enabled ? L"enabled" : L"disabled");
   if (cfg->mc_hash_enabled) {
      DECODER_DEBUG(", MC hash");
      if (cfg->mc_hash_mask > 0) {
         DECODER_DEBUG("Mask=0x%04x ", cfg->mc_hash_mask);
      }
      DECODER_DEBUG("LSB=%d ", cfg->mc_hash_lsb + HASH_OFFSET);
      DECODER_DEBUG("Zone1 start=%dGB\n", cfg->zone1_start);
   }
   DECODER_DEBUG("channel hash:     mask=0x%04x LSB=%d\n", cfg->ch_hash_mask, cfg->ch_hash_lsb + HASH_OFFSET);
   if (cfg->ddr_type == lpddr5) {
      DECODER_DEBUG("sub channel hash: mask=0x%04x LSB=%d\n", cfg->sub_ch_hash_mask, cfg->sub_ch_hash_lsb + HASH_OFFSET);
   }
   for (i = 0; i < NUM_CH; i++) {
      for (j = 0; j < NUM_SUB_CH; j++) {
         if (cfg->device_density[i][j] > 0) {
            DECODER_DEBUG("ch[%d] sub_ch[%d] population: %dGb %s %s\n", i, j, cfg->device_density[i][j] * 4,
                                                                 cfg->device_width[i][j] ? L"x16" : L"x8", cfg->ranks[i][j] ? L"2 ranks" : L"1 rank");
         }
      }
   }
   for (i = 0; i < NUM_CH; i++) {
      if (cfg->ch_size[i] > 0) {
         DECODER_DEBUG("ch[%d]: eim %s, ebh %s%s ", i, cfg->eim[i] ? L"enabled" : L"disabled", cfg->ebh[i] ? L"enabled" : L"disabled", cfg->onedpc[i] ? L", 1DPC mode " : L" ");
         if (cfg->bg0_hash[i])  {
            DECODER_DEBUG("BG0 hash: %d ", cfg->bg0_hash[i] + HASH_OFFSET - 1);
         }
         if (cfg->bg1_hash[i])  {
            DECODER_DEBUG("BG1 hash: %d ",cfg->bg1_hash[i] + HASH_OFFSET - 1);
         }
         if ((cfg->ddr_type == ddr5) && (cfg->bg2_hash[i] != 0)) {
            DECODER_DEBUG("BG2 hash: %d",cfg->bg2_hash[i] + HASH_OFFSET - 1);
         }
         DECODER_DEBUG("\n");
      }
   }
   DECODER_DEBUG("--------------------------------------------------\n");
}

//Get ranges for each field in the DRAM address table
VOID get_ranges(CPU_FAMILY family, tech ddr_type, BOOLEAN dual_rank, BOOLEAN ddr5_x8, BOOLEAN onedpc, range* ranges) {

   BOOLEAN ebh_dual_rank;

   //select according to technology ranges for DRAM decode, EIM bits and EBH masks
   ranges->range_rank    = ddr_type == lpddr5 ? RANGE_LPDDR5_RANK                                               : (ddr5_x8 ? RANGE_DDR5_X8_RANK : RANGE_DDR5_X16_RANK);
   ranges->range_bg0     = ddr_type == lpddr5 ? RANGE_LPDDR5_BG0                                                : RANGE_DDR5_BG0;
   ranges->range_bg1     = ddr_type == lpddr5 ? RANGE_LPDDR5_BG1                                                : RANGE_DDR5_BG1;
   ranges->range_bg2     = RANGE_DDR5_X8_BG2;
   ranges->range_ba0     = ddr_type == lpddr5 ? RANGE_LPDDR5_BA0                                                : (ddr5_x8 ? RANGE_DDR5_X8_BA0 : RANGE_DDR5_X16_BA0);
   ranges->range_ba1     = ddr_type == lpddr5 ? RANGE_LPDDR5_BA1                                                : (ddr5_x8 ? RANGE_DDR5_X8_BA1 : RANGE_DDR5_X16_BA1);
   ranges->range_row_msb = ddr_type == lpddr5 ? (dual_rank ? RANGE_LPDDR5_2R_ROW_MSB : RANGE_LPDDR5_1R_ROW_MSB) : (ddr5_x8 ? (dual_rank ? RANGE_DDR5_X8_2R_ROW_MSB : RANGE_DDR5_X8_1R_ROW_MSB) : (dual_rank ? RANGE_DDR5_X16_2R_ROW_MSB : RANGE_DDR5_X16_1R_ROW_MSB));
   ranges->range_row_lsb = ddr_type == lpddr5 ? (dual_rank ? RANGE_LPDDR5_2R_ROW_LSB : RANGE_LPDDR5_1R_ROW_LSB) : (ddr5_x8 ? (dual_rank ? RANGE_DDR5_X8_2R_ROW_LSB : RANGE_DDR5_X8_1R_ROW_LSB) : (dual_rank ? RANGE_DDR5_X16_2R_ROW_LSB : RANGE_DDR5_X16_1R_ROW_LSB));
   ranges->range_col_msb = ddr_type == lpddr5 ? RANGE_LPDDR5_COL_MSB                                            : RANGE_DDR5_COL_MSB;
   ranges->range_col_lsb = ddr_type == lpddr5 ? RANGE_LPDDR5_COL_LSB                                            : RANGE_DDR5_COL_LSB;

   ranges->eim_rank      = ddr_type == lpddr5 ? EIM_LPDDR5_RANK                                                 : (ddr5_x8 ? EIM_DDR5_X8_RANK : EIM_DDR5_X16_RANK);
   ranges->eim_ba0       = ddr_type == lpddr5 ? (dual_rank ? EIM_LPDDR5_2R_BA0 : EIM_LPDDR5_1R_BA0)             : (ddr5_x8 ? (dual_rank ? EIM_DDR5_X8_2R_BA0 : EIM_DDR5_X8_1R_BA0) : (dual_rank ? EIM_DDR5_X16_2R_BA0 : EIM_DDR5_X16_1R_BA0));
   ranges->eim_ba1       = ddr_type == lpddr5 ? (dual_rank ? EIM_LPDDR5_2R_BA1 : EIM_LPDDR5_1R_BA1)             : (ddr5_x8 ? (dual_rank ? EIM_DDR5_X8_2R_BA1 : EIM_DDR5_X8_1R_BA1) : (dual_rank ? EIM_DDR5_X16_2R_BA1 : EIM_DDR5_X16_1R_BA1));

   if (onedpc)
      ebh_dual_rank = FALSE; // HW uses 1R tables when DDR5-1DPC performance feature is enabled
   else
      ebh_dual_rank = dual_rank;

   ranges->ebh_bg0       = ddr_type == lpddr5 ? (ebh_dual_rank ? EBH_LPDDR5_2R_BG0 : EBH_LPDDR5_1R_BG0)
                                              : (ddr5_x8 ? (ebh_dual_rank ? EBH_DDR5_X8_2R_BG0 : EBH_DDR5_X8_1R_BG0)
                                                         : (ebh_dual_rank ? EBH_DDR5_X16_2R_BG0 : EBH_DDR5_X16_1R_BG0));
   ranges->ebh_bg1       = ddr_type == lpddr5 ? (ebh_dual_rank ? EBH_LPDDR5_2R_BG1 : EBH_LPDDR5_1R_BG1)
                                              : (ddr5_x8 ? (ebh_dual_rank ? EBH_DDR5_X8_2R_BG1 : EBH_DDR5_X8_1R_BG1)
                                                         : (ebh_dual_rank ? EBH_DDR5_X16_2R_BG1 : EBH_DDR5_X16_1R_BG1));
   ranges->ebh_bg2       = ebh_dual_rank ? EBH_DDR5_X8_2R_BG2 : EBH_DDR5_X8_1R_BG2;
   ranges->ebh_ba0       = ddr_type == lpddr5 ? (ebh_dual_rank ? EBH_LPDDR5_2R_BA0 : EBH_LPDDR5_1R_BA0)
                                              : (ddr5_x8 ? (ebh_dual_rank ? EBH_DDR5_X8_2R_BA0 : EBH_DDR5_X8_1R_BA0)
                                                         : (ebh_dual_rank ? EBH_DDR5_X16_2R_BA0 : EBH_DDR5_X16_1R_BA0));
   ranges->ebh_ba1       = ddr_type == lpddr5 ? (ebh_dual_rank ? EBH_LPDDR5_2R_BA1 : EBH_LPDDR5_1R_BA1)
                                              : (ddr5_x8 ? (ebh_dual_rank ? EBH_DDR5_X8_2R_BA1 : EBH_DDR5_X8_1R_BA1)
                                                         : (ebh_dual_rank ? EBH_DDR5_X16_2R_BA1 : EBH_DDR5_X16_1R_BA1));
}

BOOLEAN decode(config* cfg, UINT64 address, dram_address* result)
{
   slice channel_address; //channel address
   slice sub_channel_address; //sub channel address
   int sub_ch_size[NUM_SUB_CH]; //holds sub channel size in 0.5GB for the selected channel

   // Do physical to system address translation if additional registers were provided (remap_base, remap_limit, toud, memory_slice_hash)
   UINT64 system_address = address;
   result->mc = 0;
   if (cfg->tolud > 0 || cfg->mc_hash_mask > 0) {
      //REMAP
     if ((cfg->remap_base > 0) &&
         (address >= LShiftU64((UINT64)cfg->tolud, REMAP_UNIT)) &&
         (address < LShiftU64((UINT64)(cfg->tolud + cfg->remap_limit - cfg->remap_base), REMAP_UNIT))) {
         DECODER_ERROR("address inside remap region destination\n");
         return FALSE;
      }
      if ((cfg->remap_base > 0) &&
          (address >= LShiftU64((UINT64)cfg->remap_base, REMAP_UNIT)) &&
          (address < LShiftU64((UINT64)cfg->remap_limit, REMAP_UNIT))) {
          system_address -= LShiftU64((UINT64)cfg->remap_base, REMAP_UNIT);
          system_address += LShiftU64((UINT64)cfg->tolud, REMAP_UNIT);
      }

      //MC slice
      if ((cfg->mc_hash_enabled == 0) ||
          (system_address >= LShiftU64((UINT64)cfg->zone1_start, ZONE1_START_UNIT))) {
            result->mc = cfg->slice_l_id; //address in zone1
      } else {
         result->mc = xor(system_address, LShiftU64((UINT64)cfg->mc_hash_mask, HASH_OFFSET)); //address in zone0
      }
   }

   //get slice address (zone0/zone1)
   UINT64 slice_address = system_address;
   if (cfg->mc_hash_enabled == 1) {
      if (system_address >= LShiftU64((UINT64)cfg->zone1_start, ZONE1_START_UNIT)) {
         slice_address = system_address - LShiftU64((UINT64)cfg->zone1_start, (ZONE1_START_UNIT - 1)); //address in Zone1
      } else {
         slice_address = chop(system_address, cfg->mc_hash_lsb + HASH_OFFSET);  //address in Zone0
      }
   }

   if (slice_address >= (LShiftU64((UINT64)cfg->ch_size[0], CH_SIZE_UNIT) + LShiftU64((UINT64)cfg->ch_size[1], CH_SIZE_UNIT))) {
      DECODER_ERROR("address out of range\n");
      return FALSE;
   }

   //get channel address from slice address (based on channel sizes and channel hash)
   channel_address = get_zone_address(LShiftU64((UINT64)cfg->ch_size[0], CH_SIZE_UNIT),
                                      LShiftU64((UINT64)cfg->ch_size[1], CH_SIZE_UNIT),
                                      cfg->ch_hash_mask, cfg->ch_hash_lsb, slice_address);
   //XOR channel_address.id with CH_L id (in case address is in zone0)
   if ((cfg->ch_size[0] > 0) &&
       (cfg->ch_size[1] > 0) &&
       (slice_address < LShiftU64(MIN(LShiftU64((UINT64)cfg->ch_size[0], CH_SIZE_UNIT),
                                      LShiftU64((UINT64)cfg->ch_size[1], CH_SIZE_UNIT)), 1)) &&
       (cfg->ch_size[1] > cfg->ch_size[0])) {
         channel_address.id = 1 - channel_address.id;
   }

   result->ch = channel_address.id;

   //calculate sub channels sizes for selected channel
   sub_ch_size[0] = cfg->device_density[channel_address.id][0] << (cfg->device_width[channel_address.id][0] + cfg->ranks[channel_address.id][0] + (int)cfg->ddr_type);
   sub_ch_size[1] = cfg->device_density[channel_address.id][1] << (cfg->device_width[channel_address.id][1] + cfg->ranks[channel_address.id][1] + (int)cfg->ddr_type);

   //get sub channel address from channel address (based on sub channel sizes and sub channel hash)
   if (cfg->ddr_type == ddr5 &&
       cfg->eim[result->ch]) {
         cfg->sub_ch_hash_mask |= 1 << (23 - HASH_OFFSET); /// For DDR5 2DPC, if EIM is enabled DIMM select is bit 17^23
   }
   sub_channel_address = get_zone_address(LShiftU64((UINT64)sub_ch_size[0], CH_SIZE_UNIT),
                                          LShiftU64((UINT64)sub_ch_size[1], CH_SIZE_UNIT),
                                          cfg->sub_ch_hash_mask, cfg->sub_ch_hash_lsb, channel_address.address);
   if (cfg->ddr_type == ddr5 &&
       sub_ch_size[1] > sub_ch_size[0] &&
       channel_address.address < LShiftU64(MIN(LShiftU64((UINT64)sub_ch_size[0], CH_SIZE_UNIT), LShiftU64((UINT64)sub_ch_size[1], CH_SIZE_UNIT)), 1)) {
       sub_channel_address.id = 1 - sub_channel_address.id;  // For DDR5 2DPC XOR with DIMM_L id (in case address is in zone 0)
   }
   result->sub_ch = sub_channel_address.id;

   //selectors for addressing table
   BOOLEAN dual_rank = cfg->ranks[channel_address.id][sub_channel_address.id] == 1;                                          //sub channel number of ranks
   BOOLEAN ddr5_x8 = (cfg->ddr_type == ddr5) && (cfg->device_width[channel_address.id][sub_channel_address.id] == 1);        //sub channel has DDR5 X8 devices
   range ranges;

   //select according to technology ranges for DRAM decode, EIM bits and EBH masks
   get_ranges(cfg->family, cfg->ddr_type, dual_rank, ddr5_x8, cfg->onedpc[result->ch], &ranges);

   //DRAM decode
   result->rank   = dual_rank ? bit(sub_channel_address.address, ranges.range_rank) : 0;
   result->bg     = (bit(sub_channel_address.address, ranges.range_bg1) << 1) + bit(sub_channel_address.address, ranges.range_bg0);
   if (ddr5_x8) {
      result->bg += bit(sub_channel_address.address, ranges.range_bg2) << 2; //DDR5 X8 BG2
   }
   result->bank   = (bit(sub_channel_address.address, ranges.range_ba1) << 1) + bit(sub_channel_address.address, ranges.range_ba0);
   result->row    = (UINT32)range64(sub_channel_address.address, ranges.range_row_msb, ranges.range_row_lsb);
   result->column = (UINT32)range64(sub_channel_address.address, ranges.range_col_msb, ranges.range_col_lsb);

   //EIM
   if (cfg->eim[channel_address.id]) {
      result->rank ^= dual_rank ? bit(sub_channel_address.address, ranges.eim_rank) : 0;
      result->bank ^= (bit(sub_channel_address.address, ranges.eim_ba1) << 1) + bit(sub_channel_address.address, ranges.eim_ba0);
   }

   //EBH
   if (cfg->ebh[channel_address.id]) {
      //In DDR5-1DPC EBH XORs are calculated based on "sub channel address", after removal RS bit from channel address
      slice sub_channel_address_for_ebh = sub_channel_address;
      if (cfg->onedpc[result->ch]) {
         sub_channel_address_for_ebh = get_zone_address(LShiftU64((UINT64)sub_ch_size[0], (CH_SIZE_UNIT-1)),
                                                        LShiftU64((UINT64)sub_ch_size[0], (CH_SIZE_UNIT-1)),
                                                        1 << (ranges.range_rank - HASH_OFFSET),
                                                        ranges.range_rank - HASH_OFFSET,
                                                        channel_address.address);
      }

      result->bg ^= (xor(sub_channel_address_for_ebh.address, ranges.ebh_bg1) << 1) + xor(sub_channel_address_for_ebh.address, ranges.ebh_bg0);
      if (ddr5_x8) {
         result->bg ^= xor(sub_channel_address_for_ebh.address, ranges.ebh_bg2) << 2; //DDR5 X8 BG2
      }
      result->bank ^= (xor(sub_channel_address_for_ebh.address, ranges.ebh_ba1) << 1) + xor(sub_channel_address_for_ebh.address, ranges.ebh_ba0);
   }

   //BG hash
   if (cfg->bg0_hash[channel_address.id] != 0) {
      result->bg ^= bit(sub_channel_address.address, cfg->bg0_hash[channel_address.id] + HASH_OFFSET - 1);
   }
   if (cfg->bg1_hash[channel_address.id] != 0) {
      result->bg ^= bit(sub_channel_address.address, cfg->bg1_hash[channel_address.id] + HASH_OFFSET - 1) << 1;
   }
   if (ddr5_x8 && cfg->bg2_hash[channel_address.id] != 0) {
      result->bg ^= bit(sub_channel_address.address, cfg->bg2_hash[channel_address.id] + HASH_OFFSET - 1) << 2;
   }

   //Print addresses

   if (cfg->tolud > 0 || cfg->mc_hash_mask > 0) {
      DECODER_DEBUG("physical address    = 0x%010lx\n",                  address);
      DECODER_DEBUG("system address      = 0x%010lx mc          = %x\n", system_address,result->mc);
   } else {
      DECODER_DEBUG("system address      = 0x%010lx\n",                  address);
   }
   DECODER_DEBUG("slice address       = 0x%010lx\n",                     slice_address);
   DECODER_DEBUG("channel address     = 0x%010lx channel     = %x\n",    channel_address.address,     channel_address.id);
   DECODER_DEBUG("sub channel address = 0x%010lx sub channel = %x\n",    sub_channel_address.address, sub_channel_address.id);
   DECODER_DEBUG("--------------------------------------------------\n");

   return TRUE;
}

BOOLEAN encode(config* cfg, dram_address* result, UINT64* address)
{
   UINT64 sub_channel_address = 0;  //sub channel address
   UINT64 channel_address     = 0;  //channel address
   UINT64 slice_address       = 0;  //slice address
   UINT64 system_address      = 0;  //system address
   int sub_ch_size[NUM_SUB_CH]; //holds sub channel size in 0.5GB for the selected channel

   *address = 0;

   //check that address bits are in supported range
   if (result->mc > 1 ||
       result->ch > 1 ||
       result->sub_ch > 1 ||
       result->bank > 3 ||
       result->bg > 7 ||
       result->bank > 3 ||
       result->column > 1023) {
      DECODER_ERROR("address out of range\n");
      return FALSE;
   }

   //selectors for addressing table
   BOOLEAN dual_rank = cfg->ranks[result->ch][result->sub_ch] == 1;                                     //sub channel number of ranks
   BOOLEAN ddr5_x8   = (cfg->ddr_type == ddr5) && (cfg->device_width[result->ch][result->sub_ch] == 1); //sub channel has DDR5 X8 devices

   //check that BG2 isn't set for technology other than DDR5 X8
   if (!ddr5_x8 && result->bg > 3) {
      DECODER_ERROR("address out of range\n");
      return FALSE;
   }

   range ranges;

   //select according to technology ranges for DRAM decode, EIM bits and EBH masks
   get_ranges(cfg->family, cfg->ddr_type, dual_rank, ddr5_x8, cfg->onedpc[result->ch], &ranges);

   sub_channel_address += dual_rank ? LShiftU64((UINT64)result->rank, ranges.range_rank) : 0;
   sub_channel_address += ddr5_x8   ? LShiftU64((UINT64)(result->bg >= 4), ranges.range_bg2)  : 0; //DDR5 X8 BG2
   sub_channel_address += LShiftU64((UINT64)bit(result->bg,1), ranges.range_bg1);
   sub_channel_address += LShiftU64((UINT64)bit(result->bg,0), ranges.range_bg0);
   sub_channel_address += LShiftU64((UINT64)bit(result->bank,1),ranges.range_ba1);
   sub_channel_address += LShiftU64((UINT64)bit(result->bank,0), ranges.range_ba0);
   sub_channel_address += LShiftU64((UINT64)result->row,ranges.range_row_lsb);
   sub_channel_address += LShiftU64((UINT64)result->column, ranges.range_col_lsb);

   //EIM XORs
   if (cfg->eim[result->ch]) {
      if (dual_rank) {
         sub_channel_address ^= LShiftU64((UINT64)bit(sub_channel_address, ranges.eim_rank), ranges.range_rank);
      }
      sub_channel_address ^=  LShiftU64((UINT64)bit(sub_channel_address, ranges.eim_ba0), ranges.range_ba0);
      sub_channel_address ^=  LShiftU64((UINT64)bit(sub_channel_address, ranges.eim_ba1), ranges.range_ba1);
   }

   //EBH XORs
   if (cfg->ebh[result->ch]) {
      //In DDR5-1DPC EBH XORs are calculated based on sub channel address as if it was a single rank config
      // as only bits above RS are used, we can shift right the whole address for simplicity
      UINT64 sub_channel_address_for_ebh = RShiftU64(sub_channel_address, (cfg->onedpc[result->ch] ? 1 : 0));
      if (ddr5_x8) {
         sub_channel_address ^= LShiftU64((UINT64)xor(sub_channel_address_for_ebh, ranges.ebh_bg2), ranges.range_bg2); //DDR5 X8 BG2
      }
      sub_channel_address ^= LShiftU64((UINT64)xor(sub_channel_address_for_ebh, ranges.ebh_bg1), ranges.range_bg1);
      sub_channel_address ^= LShiftU64((UINT64)xor(sub_channel_address_for_ebh, ranges.ebh_bg0), ranges.range_bg0);
      sub_channel_address ^= LShiftU64((UINT64)xor(sub_channel_address_for_ebh, ranges.ebh_ba1), ranges.range_ba1);
      sub_channel_address ^= LShiftU64((UINT64)xor(sub_channel_address_for_ebh, ranges.ebh_ba0), ranges.range_ba0);
    }

   //BG hash XORs
   if (cfg->bg0_hash[result->ch] != 0) {
      sub_channel_address ^= LShiftU64((UINT64)bit(sub_channel_address, cfg->bg0_hash[result->ch] + HASH_OFFSET - 1), ranges.range_bg0);
   }
   if (cfg->bg1_hash[result->ch]!=0) {
      sub_channel_address ^= LShiftU64((UINT64)bit(sub_channel_address, cfg->bg1_hash[result->ch] + HASH_OFFSET - 1), ranges.range_bg1);
   }
   if (ddr5_x8 && cfg->bg2_hash[result->ch]!=0) {
      sub_channel_address ^= LShiftU64((UINT64)bit(sub_channel_address, cfg->bg2_hash[result->ch] + HASH_OFFSET - 1), ranges.range_bg2);
   }

   //sub channel to channel encode
   sub_ch_size[0] = cfg->device_density[result->ch][0] << (cfg->device_width[result->ch][0] + cfg->ranks[result->ch][0] + (int)cfg->ddr_type);
   sub_ch_size[1] = cfg->device_density[result->ch][1] << (cfg->device_width[result->ch][1] + cfg->ranks[result->ch][1] + (int)cfg->ddr_type);

   if (cfg->ddr_type == ddr5 &&
       cfg->eim[result->ch]) {
         cfg->sub_ch_hash_mask |= 1 << (23 - HASH_OFFSET);  // For DDR5 2DPC, if EIM is enabled DIMM select is bit 17^23
   }

   channel_address = get_upper_level_address(LShiftU64((UINT64)sub_ch_size[0], CH_SIZE_UNIT),
                                             LShiftU64((UINT64)sub_ch_size[1], CH_SIZE_UNIT),
                                             cfg->sub_ch_hash_mask, cfg->sub_ch_hash_lsb + HASH_OFFSET,
                                             result->sub_ch ^ (cfg->ddr_type == ddr5 && (sub_ch_size[1] > sub_ch_size[0]) ? 1 : 0),
                                             sub_channel_address);

   //channel to slice encode
   slice_address = get_upper_level_address(LShiftU64((UINT64)cfg->ch_size[0], CH_SIZE_UNIT),
                                           LShiftU64((UINT64)cfg->ch_size[1], CH_SIZE_UNIT),
                                           cfg->ch_hash_mask, cfg->ch_hash_lsb + HASH_OFFSET,
                                           result->ch ^ (cfg->ch_size[1] > cfg->ch_size[0]), channel_address);

   //slice to MC encode
   system_address = slice_address;  //1MC

   if (cfg->mc_hash_enabled) {  //2MCs
      if (slice_address >= LShiftU64((UINT64)cfg->zone1_start, (ZONE1_START_UNIT-1))) {
         system_address = slice_address + LShiftU64((UINT64)cfg->zone1_start, (ZONE1_START_UNIT-1)); //zone1 with 2MCs
      }else { //zone0
         system_address  = LShiftU64(range64(slice_address, 39, cfg->mc_hash_lsb + HASH_OFFSET), (cfg->mc_hash_lsb + HASH_OFFSET + 1));                             //High bits
         system_address += range64(slice_address, cfg->mc_hash_lsb + HASH_OFFSET - 1, 0);                                                                           //Low bits
         system_address += LShiftU64((UINT64)(xor(system_address, LShiftU64((UINT64)cfg->mc_hash_mask, HASH_OFFSET))^result->mc), cfg->mc_hash_lsb + HASH_OFFSET);  //Hash bit
         //NOTE - without MC id and hash mask theres not enaough information to calculate system address hash LSB bit
      }
   }

   //(UN)REMAP
   *address = system_address;
   if ((cfg->remap_base > 0) &&
       (*address >= LShiftU64((UINT64)cfg->tolud, REMAP_UNIT)) &&
       (*address < LShiftU64(((UINT64)cfg->tolud + cfg->remap_limit - cfg->remap_base), REMAP_UNIT))){
      *address -= LShiftU64((UINT64)cfg->tolud, REMAP_UNIT);
      *address += LShiftU64((UINT64)cfg->remap_base, REMAP_UNIT);
   }

   //Print addresses
   DECODER_DEBUG("mc=%x ch=%x sub_ch=%x rank=%x bg=%x bank=%x row=%x column=%x\n",result->mc, result->ch, result->sub_ch, result->rank, result->bg, result->bank, result->row, result->column);
   DECODER_DEBUG("sub channel address = 0x%010lx\n", sub_channel_address);
   DECODER_DEBUG("channel address     = 0x%010lx\n", channel_address);
   DECODER_DEBUG("slice address       = 0x%010lx\n", slice_address);
   if (cfg->tolud > 0 || cfg->mc_hash_mask > 0) {
      DECODER_DEBUG("system address      = 0x%010lx\n", system_address);
   }
   DECODER_DEBUG("--------------------------------------------------\n");

   return TRUE;
}

// Auxilary function for decode, generate address according to zone0/zone1
slice get_zone_address(UINT64 size_0, UINT64 size_1, int hash_mask, int hash_lsb, UINT64 address) {

   slice result;

   //single slice is populated, address is unchanged, id is according to occupied slice
   result.address = address;
   if (size_0 == 0) {
      result.id = 1;
      return result;
   }
   if (size_1 == 0) {
      result.id = 0;
      return result;
   }

   //slices are asymmetric and address is above 2*small slice size - zone1
   if ((size_0 != size_1) &&
       (address >= LShiftU64(MIN(size_0, size_1), 1))) {
      result.address -= MIN(size_0, size_1);    //reduce small slice size from address
      result.id = size_0 < size_1 ? 1 : 0;       //id is the large slice id
      return result;
   }

   //address is hashed between the two slices (zone0)
   result.address = chop(address,hash_lsb + HASH_OFFSET); //remove hash LSB bit from address
   result.id = xor(address, hash_mask << HASH_OFFSET);     //use hash mask to determine slice id
   return result;
}

// Auxilary function for encode, generate upper level address according to zone0/zone1
UINT64 get_upper_level_address(UINT64 size_0, UINT64 size_1, UINT32 hash_mask, int hash_lsb, int index, UINT64 address) {

   UINT64 result = 0;

   //single slice is populated, address is unchanged
   if (size_0 == 0 || size_1 == 0) {
      result = address;
      return result;
   }

   //zone 0
   if ((size_0 == size_1) ||
       (address < MIN(size_0, size_1))) {
      result  = LShiftU64(range64(address, 39, hash_lsb), (hash_lsb + 1));       //High bits
      result += range64(address, hash_lsb-1, 0);                                   //Low bits
      result += LShiftU64((UINT64)(xor(result, LShiftU64((UINT64)hash_mask, HASH_OFFSET)) ^ index), hash_lsb);                 //Hash LSB bit
      return result;
   }

   //zone 1
   result = address + MIN(size_0, size_1);
   return result;
}