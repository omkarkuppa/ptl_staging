## @file
#  Platform description.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
# @par Specification
##

  ################################################################################
  #
  # UPDs consumed in FspSiliconInit Api
  #
  ################################################################################
  # !BSF FIND:{PTLUPD_S}
  # !HDR COMMENT:{FSP_UPD_HEADER:FSP UPD Header}
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:START}
  # FspsUpdSignature: {PTLUPD_S}
  gPlatformFspPkgTokenSpaceGuid.Signature                   | * | 0x08 | 0x535F4450554C5450
  # !BSF NAME:{FspsUpdRevision}  TYPE:{None}
  gPlatformFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x02
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:END}
  gPlatformFspPkgTokenSpaceGuid.Reserved                    | * | 0x17 | {0x00}

  # !HDR COMMENT:{FSPS_ARCH2_UPD:FSPS_ARCH_UPD}
  # !HDR EMBED:{FSPS_ARCH2_UPD:FspsArchUpd:START}
  gPlatformFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x02
  gPlatformFspPkgTokenSpaceGuid.Reserved                    | * | 0x03 | {0x00}
  gPlatformFspPkgTokenSpaceGuid.Length                      | * | 0x04 | 0x00000020
  gPlatformFspPkgTokenSpaceGuid.FspEventHandler             | * | 0x08 | 0x00000000

  # !HDR EMBED:{FSPS_ARCH2_UPD:FspsArchUpd:END}
  gPlatformFspPkgTokenSpaceGuid.Reserved1                   | * | 0x10 | {0x00}

  # !HDR COMMENT:{FSP_S_CONFIG:FSP S Configuration}
  # !HDR EMBED:{FSP_S_CONFIG:FspsConfig:START}

  #
  # CPU Post-Mem Block Start
  #
  # !BSF PAGE:{CPU2}

  # !BSF NAME:{CpuBistData} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Pointer CPU BIST Data}
  gPlatformFspPkgTokenSpaceGuid.CpuBistData                 | * | 0x8 | 0

  # !BSF NAME:{CpuMpPpi} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{<b>Optional</b> pointer to the boot loader's implementation of EFI_PEI_MP_SERVICES_PPI. If not NULL, FSP will use the boot loader's implementation of multiprocessing. See section 5.1.4 of the FSP Integration Guide for more details.}
  gPlatformFspPkgTokenSpaceGuid.CpuMpPpi                     | * | 0x8 | 0

    # !BSF NAME:{PMC WDT enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable PMC WDT configuration}
  gPlatformFspPkgTokenSpaceGuid.PmcWdtTimerEn               | * | 0x1  | 0x01

  # !HDR EMBED:{FSP_S_CONFIG:FspsConfig:END}
  gPlatformFspPkgTokenSpaceGuid.ReservedFspsUpd             | * | 0x1 | {0x00}

  # Note please keep "UpdTerminator" at the end of each UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure.

  gPlatformFspPkgTokenSpaceGuid.UpdTerminator               | * | 0x02 | 0x55AA