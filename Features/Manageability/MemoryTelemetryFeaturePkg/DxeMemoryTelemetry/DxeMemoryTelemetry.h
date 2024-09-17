/** @file
  Intel Dxe Memory Telemetry functional definitions.

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

#ifndef _DXE_MEMORY_TELEMETRY_H_
#define _DXE_MEMORY_TELEMETRY_H_

#include <PiDxe.h>
#include <MemInfoHob.h>

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
#define DDR_DTYPE_LPDDR3        0xF1  ///< LPDDR3 memory type
#define DDR_DTYPE_JEDEC_LPDDR3  0x0F  ///< LPDDR3 memory type, JEDEC SPD Specification

/**
  Function to create/clear the AmtPprEnable variable
**/
VOID
MemoryTelemetryAmtPprMain (
  VOID
  );

/**
  Entry point of the Dxe Memory Telemetry
  Checks for support, and begins the PHAT table information update.

  @param[in] ImageHandle        Handle for this drivers loaded image protocol.
  @param[in] SystemTable        EFI system table.

  @retval     EFI_SUCCESS       The protocol has installed successfully
  @retval     EFI_UNSUPPORTED   The protocol is not supported
  @retval     Others            The protocol did not install successfully
**/
EFI_STATUS
EFIAPI
DxeMemoryTelemetryEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#endif // _DXE_MEMORY_TELEMETRY_H_
