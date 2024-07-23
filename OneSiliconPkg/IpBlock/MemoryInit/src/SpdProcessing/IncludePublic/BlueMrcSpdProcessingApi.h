/** @file
  SPD processing header file.

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

@par Specification
**/
#ifndef BlueMrcSpdProcessing_h_
#define BlueMrcSpdProcessing_h_

#include "CMrcInterface.h"  // for MrcParameters

/**
    Calculate the memory clock value from the current memory frequency.

    @param[in, out] MrcData     - Pointer to MrcData data structure.
    @param[in]      Frequency   - Memory frequency to convert.

    @retval Returns the tCK value in [fs] for the given frequency.
**/
extern
UINT32
ConvertFreq2Clock (
  IN OUT MrcParameters *const MrcData,
  IN     const MrcFrequency   Frequency
);

/**
  Calculate the CRC16 of the provided SPD data. CRC16 formula is the same
  one that is used for calculating the CRC16 stored at SPD bytes 126-127.
  This can be used to detect DIMM change.

  @param[in]  Buffer - Pointer to the start of the data.
  @param[in]  Size   - Amount of data in the buffer, in bytes.
  @param[out] Crc    - Pointer to location to write the calculated CRC16 value.

  @retval Returns TRUE.
**/
extern
BOOLEAN
GetDimmCrc (
  IN  const UINT8 *const Buffer,
  IN  const UINT32       Size,
  OUT UINT16      *const Crc
  );

/**
  Determine the starting address and size of the SPD area to generate a CRC.

    @param[in, out] MrcData    - The MRC "global data".
    @param[in]      Controller - Controller index.
    @param[in]      Channel    - Channel index.
    @param[in]      Dimm       - Dimm index.
    @param[out]     CrcSize    - Location to write CRC block size.

    @retval The starting address of the CRC block.
**/
const UINT8 *
MrcSpdCrcArea (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT8                Dimm,
  OUT    UINT32        *const CrcSize
  );

/**
  Find the vendor-specific swizzling configuration

  @param[in]  MrcData   - Pointer to Spd data
  @param[in]  Controller     - Current Controller
  @param[in]  Channel        - Current Channel
  @param[in]  Dimm           - Current Dimm
  @param[in]  DimmMap        - Dimm Bit Map of the dimm
  @param[in][out] MA1Swizzle - MA1_Swizzling
  @param[in][out] MA2Swizzle - MA2_Swizzling
**/
VOID
MrcGetVendorSwizzling (
  IN MrcParameters *const MrcData,
  IN UINT8             Controller,
  IN UINT8             Channel,
  IN UINT8             Dimm,
  IN UINT8             DimmMap,
  IN OUT UINT32        *MA1Swizzle,
  IN OUT UINT32        *MA2Swizzle
  );

/**
  Get the DDR Type for the first populated channel and populate the type
  specific parameters in the global data structure.

  @param[in, out] MrcData - The MRC "global data".

  @retval mrcSuccess
**/
MrcStatus
MrcGetSpdDdrTypeParams (
  IN OUT MrcParameters *const MrcData
  );

/**
  Process the SPD information for all DIMMs on all channels.

  @param[in, out] MrcData - The MRC "global data".

  @retval mrcSuccess on success, mrcDimmNotExist if no DIMMs found.
**/
extern
MrcStatus
MrcSpdProcessingStatic (
  IN OUT MrcParameters *const MrcData
  );

/**
  Process the SPD information for all DIMMs on all channels.

  @param[in, out] MrcData - The MRC "global data".

  @retval mrcSuccess on success, mrcDimmNotExist if no DIMMs found.
**/
extern
MrcStatus
MrcSpdProcessingCalc (
  IN OUT MrcParameters *const MrcData
  );

/**
   Determine if MemoryProfile is XMP and if DIMM doesn't support XMP

   @param[in] MrcData - Pointer to MrcData data structure.
   @param[in] Profile

   @retval TRUE on XMP PROFILE 1-5 and DIMM doesn't support the corresponding XMP profile.
   @retval FALSE XMP profile is not 1-5 or DIMM does support the corresponding XMP profile.
**/
BOOLEAN
NeedIgnoreXmp (
  IN MrcParameters *const MrcData,
  IN MrcProfile           Profile
  );
#endif // BlueMrcSpdProcessing_h_
