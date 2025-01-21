/** @file
  The definition for the VTd Enable DMA Buffer PPI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef __VTD_ENABLE_DMA_BUFFER_PPI_H__
#define __VTD_ENABLE_DMA_BUFFER_PPI_H__

#define VTD_ENABLE_DMA_BUFFER_PPI_GUID \
    { \
      0x2355a0a7, 0x4c52, 0x4b5b, { 0x96, 0xac 0xe5, 0x8e, 0x5e, 0xec, 0x79, 0xc8 } \
    }

//
// Forward declaration for the VTd Enable DMA Buffer PPI.
//

/*
  Enable DMA buffer in VTd

  @param[in] VtdEngineIndex         VTd engine index.

  @retval EFI_SUCCESS               Enable successfully.
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid.
  @retval EFI_UNSUPPORTED           VTd base is zero.
  @retval EFI_OUT_OF_RESOURCES      There is no additional space in the PPI database.
*/
typedef
EFI_STATUS
(EFIAPI *VTD_ENABLE_DMA_BUFFER) (
  IN VTD_ENGINE VtdEngineIndex
  );

extern EFI_GUID gVTdEnableDmaBufferPpiGuid;

#endif

