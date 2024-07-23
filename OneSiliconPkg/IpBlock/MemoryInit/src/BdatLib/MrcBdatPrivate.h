/** @file
  Prototype information for private/internal function used for
  Copying the memory related timing and configuration information into the
  Compatible BIOS data (BDAT) table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#ifndef MRC_BDAT_PRIVATE_H_
#define MRC_BDAT_PRIVATE_H_

#include "CMrcInterface.h" // For MrcParameters
#include "CMrcRmtData.h"  // for RMT_RESULT_COLUMNS

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcBdatPrivate)

/**
  Print out the RMT column data.

  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      RmtRows       - The RMT column result.
  @param[in]      ElementNumber - The index of the results.
*/
VOID
PrintBdatResult (
  IN MrcParameters  *const MrcData,
  IN RMT_RESULT_COLUMNS *RmtRows,
  IN UINT32             ElementNumber
);


/**
  Determine the number of rows for the RMT Results

  @param[in] MrcData - Include all MRC global data.

  @return The number of RMT Result Rows
**/
UINT32
GetNumberRowsBdatResult (
  IN MrcParameters *const MrcData
);


/**
  Return the size of the BDAT Schema for the specified BDAT Test Type.

  @param[in] MrcData - Include all MRC global data.

  @return The size of the BDAT Schema.
**/
UINT32
GetBdatSchemaSize (
  IN MrcParameters *const MrcData
);


/**
Fill the RMT schema 6 which is defined in the BDAT spec.
The schema includes meta data and columns data sections.

  @param[in, out] MrcData - Include all MRC global data.
  @param[out]     RmtDataBuffer - Pointer to data block to fill up the
                  RMT structure.
  @param[in]      RmtDataBufferSize - Size of the RMT data block.
  @param[in]      MsgPrint- Show debug prints
**/
VOID
FillRmtBdatStructure (
  IN OUT MrcParameters *const MrcData,
  OUT   VOID           *RmtDataBuffer,
  IN    UINT32          RmtDataBufferSize,
  IN    BOOLEAN         MsgPrint
);


/**
Fill the Margin2D.
The schema includes meta data and columns data sections.

  @param[in, out] MrcData - Include all MRC global data.
  @param[out]     DataBuffer - Pointer to data block to fill up the Margin2D
                  result structure.
  @param[in]      DataBufferSize - Size of the Margin2D results data block.
  @param[in]      MsgPrint - Show debug prints
**/
VOID
FillMargin2DBdatStructure (
  IN OUT MrcParameters *const MrcData,
  OUT   VOID           *DataBuffer,
  IN    UINT32          DataBufferSize,
  IN    BOOLEAN         MsgPrint
);


#endif // MRC_BDAT_PRIVATE_H_
