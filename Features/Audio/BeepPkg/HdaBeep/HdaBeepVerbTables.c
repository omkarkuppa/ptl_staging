/** @file
  HD-Audio verb tables for intend of beeping on HDA interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <HdaBeepVerbTables.h>

HDAUDIO_VERB_TABLE Alc256InitVerbs = HDAUDIO_VERB_TABLE_INIT (
  //
  //  VerbTable: (Realtek ALC256)
  //  Revision ID = 0xff
  //  Codec Verb Table
  //  Codec Address: CAd value (0/1/2)
  //  Codec Vendor: 0x10EC0256
  //
  0x10EC, 0x0256,
  0xFF, 0xFF,
  //===================================================================================================
  //
  //                               Realtek Semiconductor Corp.
  //
  //===================================================================================================

  //Realtek High Definition Audio Configuration - Version : 5.0.3.1
  //Realtek HD Audio Codec : ALC256
  //PCI PnP ID : PCI\VEN_8086&DEV_2668&SUBSYS_72708086
  //HDA Codec PnP ID : INTELAUDIO\FUNC_01&VEN_10EC&DEV_0256&SUBSYS_10EC12CC
  //The number of verb command block : 12

  //    NID 0x12 : 0x411111F0
  //    NID 0x13 : 0x411111F0
  //    NID 0x14 : 0x90170110
  //    NID 0x18 : 0x411111F0
  //    NID 0x19 : 0x04A11040
  //    NID 0x1A : 0x411111F0
  //    NID 0x1B : 0x411111F0
  //    NID 0x1D : 0x40610041
  //    NID 0x1E : 0x411111F0
  //    NID 0x21 : 0x04211020

  //===== HDA Codec Subsystem ID Verb-table =====
  //HDA Codec Subsystem ID  : 0x10EC12CC
  0x001720CC,
  0x00172112,
  0x001722EC,
  0x00172310,

  //===== Pin Widget Verb-table =====
  //Widget node 0x01 :
  0x0017FF00,
  0x0017FF00,
  0x0017FF00,
  0x0017FF00,
  //Pin widget 0x12 - DMIC
  0x01271CF0,
  0x01271D11,
  0x01271E11,
  0x01271F41,
  //Pin widget 0x13 - DMIC
  0x01371CF0,
  0x01371D11,
  0x01371E11,
  0x01371F41,
  //Pin widget 0x14 - Front (Port-D)
  0x01471C10,
  0x01471D01,
  0x01471E17,
  0x01471F90,
  //Pin widget 0x18 - NPC
  0x01871CF0,
  0x01871D11,
  0x01871E11,
  0x01871F41,
  //Pin widget 0x19 - MIC2 (Port-F)
  0x01971C40,
  0x01971D10,
  0x01971EA1,
  0x01971F04,
  //Pin widget 0x1A - LINE1 (Port-C)
  0x01A71CF0,
  0x01A71D11,
  0x01A71E11,
  0x01A71F41,
  //Pin widget 0x1B - NPC
  0x01B71CF0,
  0x01B71D11,
  0x01B71E11,
  0x01B71F41,
  //Pin widget 0x1D - BEEP-IN
  0x01D71C41,
  0x01D71D00,
  0x01D71E61,
  0x01D71F40,
  //Pin widget 0x1E - NPC
  0x01E71CF0,
  0x01E71D11,
  0x01E71E11,
  0x01E71F41,
  //Pin widget 0x21 - HP1-OUT (Port-I)
  0x02171C20,
  0x02171D10,
  0x02171E21,
  0x02171F04,
  //Widget node 0x20 - 1 ://Codec hidden reset and speaker power 2W/4ohm
  0x0205001A,
  0x0204C003,
  0x02050038,
  0x02047901,
  //Widget node 0x20 - 2 ://Class D power on Reset
  0x0205003C,
  0x02040354,
  0x0205003C,
  0x02040314,
  //Widget node 0x20 - 3 :// Disable AGC and set AGC limit to -1.5dB
  0x02050016,
  0x02040C50,
  0x02050012,
  0x0204EBC1,
  //Widget node 0x20 - 4 :// Set AGC Post gain +1.5dB then Enable AGC
  0x02050013,
  0x02044023,
  0x02050016,
  0x02040E50,
  //Widget node 0x20 - 5 ://Silence detector enableing + Set EAPD to verb control
  0x02050037,
  0x0204FE15,
  0x02050010,
  0x02040020,
  //Widget node 0x20 - 6 ://Silence data mode Threshold (-90dB)
  0x02050030,
  0x0204A000,
  0x0205001B,
  0x02040A4B,
  //Widget node 0x20 - 7 ://Default setting-1
  0x05750003,
  0x05740DA3,
  0x02050046,
  0x02040004,
  //Widget node 0x20 - 8 : support 1 pin detect two port
  0x02050009,
  0x0204E003,
  0x0205000A,
  0x02047770,
  //Widget node 0x20 - 9 : To set LDO1/LDO2 as default
  0x02050008,
  0x02046A0C,
  0x02050008,
  0x02046A0C,
  // Enable PCBeep pass-through to 0x14 FRONT widget (Class D amplifier)
  0x02050036,
  0x02047717,
  // Set EADP at high to enable Class D amplifier
  0x02050010,
  0x02040320,
  // Enable 0x14 output buffer and unmute it
  0x01470740,
  0x0143B000
);

HDAUDIO_VERB_TABLE Alc256BeepOnVerbs = HDAUDIO_VERB_TABLE_INIT (
  //
  //  VerbTable: (Realtek ALC256)
  //  Revision ID = 0xff
  //  Codec Verb Table
  //  Codec Address: CAd value (0/1/2)
  //  Codec Vendor: 0x10EC0256
  //
  0x10EC, 0x0256,
  0xFF, 0xFF,

  // Enable sine tone generator
  0x00170A0C
);

HDAUDIO_VERB_TABLE Alc256BeepOffVerbs = HDAUDIO_VERB_TABLE_INIT (
  //
  //  VerbTable: (Realtek ALC256)
  //  Revision ID = 0xff
  //  Codec Verb Table
  //  Codec Address: CAd value (0/1/2)
  //  Codec Vendor: 0x10EC0256
  //
  0x10EC, 0x0256,
  0xFF, 0xFF,

  // Sine gen disable
  0x00170A00
);


HDA_BEEP_VERB_TABLES HdaBeepVerbTablesAlc256 = HDA_BEEP_VERB_TABLES_INIT (
  &Alc256InitVerbs,
  &Alc256BeepOnVerbs,
  &Alc256BeepOffVerbs
);
