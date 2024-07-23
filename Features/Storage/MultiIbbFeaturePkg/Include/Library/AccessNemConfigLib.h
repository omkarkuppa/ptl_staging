/** @file
  Provide APIs for accessing BaseNemConfigLib not in Feature Pkg.

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

#ifndef  _ACCESS_NEM_CONFIG_LIB_H_
#define  _ACCESS_NEM_CONFIG_LIB_H_

//
// Include files
//
#include <Uefi.h>
#include <PiPei.h>

typedef struct _NEM_DATA_INPUT_STRUCT {
  UINT32        FunctionName;
  UINTN        *Data1;
  UINTN        *Data2;
  UINTN        *Data3;
  UINTN        *Data4;

} NEM_DATA_INPUT_STRUCT;

typedef enum {
  MtrrInitMask = 0,
  MtrrRangeChange,
  NemSizeCalculation
} NEM_CONFIG_LIB;

/**
  Ppi setup for accessing libraries not in Feature Pkg.

  @param[in]  FunctionName       Input the name of the calling function
  @param[in]  Ptr1               Pointer of the input parameter 1 consumed by indicating function
  @param[in]  Ptr2               Pointer of the input parameter 2 consumed by indicating function
  @param[in]  Ptr3               Pointer of the input parameter 3 consumed by indicating function
  @param[in]  Ptr4               Pointer of the input parameter 4 consumed by indicating function

  @retval EFI_SUCCESS            Ppi setup successfully
  @retval Others                 Ppi setup failed
**/
EFI_STATUS
BridgeTo1sLibPpiSetup (
  IN UINT32  FunctionName,
  IN UINTN  *Ptr1,
  IN UINTN  *Ptr2,
  IN UINTN  *Ptr3,
  IN UINTN  *Ptr4
  );

#endif // _ACCESS_NEM_CONFIG_LIB_H_
