/** @file

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

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#define EFI_NON_DEVICE_CLASS              0x00
#define EFI_DISK_DEVICE_CLASS             0x01
#define EFI_VIDEO_DEVICE_CLASS            0x02
#define EFI_NETWORK_DEVICE_CLASS          0x04
#define EFI_INPUT_DEVICE_CLASS            0x08
#define EFI_ON_BOARD_DEVICE_CLASS         0x10
#define EFI_OTHER_DEVICE_CLASS            0x20

#define CONFIGURATION_DISABLED            0x0
#define CONFIGURATION_ENABLED             0x1
#define CONFIGURATION_AUTO                0x1

#define SETUP_HD_INTERFACE_IDE            0x0
#define SETUP_HD_INTERFACE_AHCI           0x1
#define SETUP_HD_INTERFACE_RAID           0x2

#define IGD_DISPLAY_DEVICE_IGD            0x0
#define IGD_DISPLAY_DEVICE_PEG            0x1
#define IGD_DISPLAY_DEVICE_PCI            0x2

//
// B7 = LFP2 B6 = EFP2 B5 = TV2 B4 = CRT2 B3 = LFP B2 = EFP B1 = TV B0 = CRT
//
#define IGD_BT_CRT                        0x01
#define IGD_BT_TV                         0x02
#define IGD_BT_EFP                        0x04
#define IGD_BT_LFP                        0x08
#define IGD_BT_CRT2                       0x10
#define IGD_BT_TV2                        0x20
#define IGD_BT_EFP2                       0x40
#define IGD_BT_LFP2                       0x80

#define IGD_BT_CRT_LFP                    0x09
#define IGD_BT_VBIOS_DEFAULT              0
#define IGD_BT_LFPSDVO                    IGD_BT_LFP2
#define IGD_BT_TVSDVO                     IGD_BT_TV2
#define IGD_BT_CRT_LFPSDVO                0x81
#define IGD_BT_CRT_EFP                    0x05
#define IGD_BT_CRT_TV_LFP                 0x0B

#define IGD_GMS_FIXED                     0x0
#define IGD_GMS_DVMT                      0x1

#define STOLEN_MEMORY_1M                  0x0
#define STOLEN_MEMORY_8M                  0x1
//
// Memory labels
//
#define MEMORY_SLOT1_SPEED          0x0200
#define MEMORY_SLOT2_SPEED          0x0201
#define MEMORY_SLOT3_SPEED          0x0202
#define MEMORY_SLOT4_SPEED          0x0203
#define END_MEMORY_SLOT_SPEED       0x020F
#define MEMORY_TCL_DDR2             0x0300
#define MEMORY_TCL_DDR              0x0301
#define MEMORY_TCL_END              0x030F

//
// DVMT5.0 Graphic Memory labels
//
#define DVMT50_MODE                   0x0301

#define DVMT50_PRE_ALLOC              0x0302



//
// GMCH SKUs
//
#define GMCH_CHIPSETTYPE_945GM      0
#define GMCH_CHIPSETTYPE_945PM      1
#define GMCH_CHIPSETTYPE_945GMS     2
#define GMCH_CHIPSETTYPE_945GML     3
#define GMCH_CHIPSETTYPE_945GT      4

#define OPR_SCI                        0x00
#define OPR_SMI                        0x01

#endif
