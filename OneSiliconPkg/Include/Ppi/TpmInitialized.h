/** @file
  Tag GUID that must be installed by the TPM PEIM after the TPM hardware is
  initialized.  PEIMs that must execute after TPM hardware initialization
  may use this GUID in their dependency expressions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2008 Intel Corporation.

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

#ifndef _PEI_TPM_INITIALIZED_PPI_H_
#define _PEI_TPM_INITIALIZED_PPI_H_

///
/// Global ID for the PEI_TPM_INITIALIZED_PPI which always uses a NULL interface.
///
#define PEI_TPM_INITIALIZED_PPI_GUID \
  { \
    0xe9db0d58, 0xd48d, 0x47f6, 0x9c, 0x6e, 0x6f, 0x40, 0xe8, 0x6c, 0x7b, 0x41 \
  }

extern EFI_GUID gPeiTpmInitializedPpiGuid;

///
/// Global ID for the PEI_TPM_INITIALIZATION_DONE_PPI which always uses a NULL interface.
///
#define PEI_TPM_INITIALIZATION_DONE_PPI_GUID \
  { \
    0xa030d115, 0x54dd, 0x447b, { 0x90, 0x64, 0xf2, 0x6, 0x88, 0x3d, 0x7c, 0xcc \
  }

extern EFI_GUID gPeiTpmInitializationDonePpiGuid;

#endif
