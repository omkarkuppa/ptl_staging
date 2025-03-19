/** @file
  Memory definitions for form factor and module type.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef _MEMORY_DEFINITIONS_H_
#define _MEMORY_DEFINITIONS_H_

///
/// Memory module type definition in DDR3 SPD Data
///
#define DDR_MTYPE_SPD_OFFSET  3     ///< Module type, offset 3, bits (3:0)
#define DDR_MTYPE_SPD_MASK    0x0F  ///< Module Type mask
#define DDR_MTYPE_RDIMM       0x01  ///< Registered DIMM Memory
#define DDR_MTYPE_UDIMM       0x02  ///< Unbuffered DIMM Memory
#define DDR_MTYPE_SODIMM      0x03  ///< Small Outline DIMM Memory
#define DDR_MTYPE_LR_DIMM     0x04  ///< LR-DIMM Memory
#define DDR_MTYPE_CUDIMM      0x05  ///< CU DIMM Memory
#define DDR_MTYPE_CSODIMM     0x06  ///< CSO DIMM Memory
#define DDR_MTYPE_MRDIMM      0x07  ///< MR DIMM Memory
#define DDR_MTYPE_CAMM        0x08  ///< CAMM Memory
#define DDR_MTYPE_DDIMM       0x0A  ///< Small Outline Unbuffered DIMM Memory, 72-bit data bus
#define DDR_MTYPE_MEM_DOWN    0x0B  ///< Soldered Down Memory

///
/// Memory device type definition in DDR3 SPD Data
///
#define DDR_DTYPE_SPD_OFFSET    2     ///< Device type, offset 2, bits (7:0)
#define DDR_DTYPE_DDR3          11    ///< DDR3 memory type
#define DDR_DTYPE_DDR4          12    ///< DDR4 memory type
#define DDR_DTYPE_DDR5          18    ///< DDR5 memory type
#define DDR_DTYPE_LPDDR5        19    ///< LPDDR5 memory type
#define DDR_DTYPE_LPDDR5X       21    ///< LPDDR5X memory type
#define DDR_DTYPE_LPDDR3        0xF1  ///< LPDDR3 memory type
#define DDR_DTYPE_JEDEC_LPDDR3  0x0F  ///< LPDDR3 memory type, JEDEC SPD Specification

///
/// Memory module type definition in LP4/LP5 SPD Spec
///
#define DDR_MTYPE_LPDIMM_LP5      0x07  ///< LP DIMM Memory
#define DDR_MTYPE_MEM_DOWN_LP5    0x0E  ///< Soldered Down Memory

///
/// Standin for missing enum definition in SMBIOS type 17 MEMORY_FORM_FACTOR
/// TODO: Update concurrently with SMBIOS Type 17 Spec update of MEMORY_FORM_FACTOR
///
#define MEMORY_FORM_FACTOR_CAM    0x11

///
/// AMT PPR Ran last boot definitions
///
#define RAN_LAST_BOOT   1

///
/// Controller device presence definitions
///
#ifndef CONTROLLER_NOT_PRESENT
#define CONTROLLER_NOT_PRESENT  0
#endif

///
/// Error Info Hob current max returned
///
#ifndef MAX_RESULTS_HOB_COUNT
#define MAX_RESULTS_HOB_COUNT   1
#endif

#endif // _MEMORY_DEFINITIONS_H_
