/** @file
  Copies the memory related timing and configuration information into the
  Compatible BIOS data (BDAT) table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef MRC_BDAT_H_
#define MRC_BDAT_H_


#include "CMrcInterface.h"  // for MrcParameters

/**
  Fill the compatible data structure BDAT with the information provided by
  the memory initialization code.

  @param[in, out] MrcData - Constant pointer to the Mrc data structure which contains the BDAT structure to fill.

  @retval mrcSuccess if the Hob is created successfully, or Bdat
          not requested (Inputs->BdatEnable is FALSE)
  @retval mrcFail if something went wrong
**/
MrcStatus
MrcFillBdatStructure (
  IN OUT MrcParameters *const MrcData
  );

#endif // MRC_BDAT_H_
