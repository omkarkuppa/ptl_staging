/** @file
  This file defines the Silicon Temp Ram Exit PPI which implements the MTRR values initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _FSP_TEMP_RAM_EXIT_PPI_H_
#define _FSP_TEMP_RAM_EXIT_PPI_H_

///
/// Global ID for the FSP_TEMP_RAM_EXIT_PPI.
///
#define FSP_TEMP_RAM_EXIT_GUID \
  { \
    0xbc1cfbdb, 0x7e50, 0x42be, { 0xb4, 0x87, 0x22, 0xe0, 0xa9, 0x0c, 0xb0, 0x52 } \
  }

//
// Forward declaration for the FSP_TEMP_RAM_EXIT_PPI.
//
typedef struct _FSP_TEMP_RAM_EXIT_PPI FSP_TEMP_RAM_EXIT_PPI;

/**
  Program MTRR values and print MTRRs.
**/
typedef
EFI_STATUS
(EFIAPI *FSP_TEMP_RAM_EXIT) (
  IN  VOID    *TempRamExitParamPtr
  );

///
/// This PPI provides function to program MTRR values
///
struct _FSP_TEMP_RAM_EXIT_PPI {
  FSP_TEMP_RAM_EXIT   TempRamExit;
};

extern EFI_GUID gFspTempRamExitPpiGuid;

#endif // _FSP_TEMP_RAM_EXIT_PPI_H_
