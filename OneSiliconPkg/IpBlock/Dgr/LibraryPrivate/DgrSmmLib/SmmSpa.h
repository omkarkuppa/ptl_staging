/** @file
  The DGR specific Header file for SMM SPA.

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

@par Specification Reference:
**/

#ifndef _SMM_SPA_H_
#define _SMM_SPA_H_

#include <Protocol/LoadedImage.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>

/**
  SMM Policy Analyzer (SPA) callback function at SMM Ready to Lock.
  Initializes the SPA Context structure with reserved memory for SPA.

  @param[in] Protocol   Points to the protocol's unique identifier
  @param[in] Interface  Points to the interface instance
  @param[in] Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS   Notification handler runs successfully.
**/
EFI_STATUS
EFIAPI
SpaSmmReadyToLockCallback (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  );

//
// Definition help catch error at build time.
//

#pragma pack (push)
#pragma pack (1)

#define C_ASSERT(e)  typedef char ___C_ASSERT___[e?1:-1]

#define NAME_STRING_LENGTH         35
#define SPA_PAGE_TABLE_POOL_SIZE   0x100000     // 1 MB
#define MAX_PROTOCOL_ENTRIES       2            // Define maximum of only 2 protocol entries

typedef struct {
  EFI_GUID FileGuid;
  UINTN    EntryPoint;
  UINTN    ImageBase;
  UINTN    ImageSize;
  UINTN    LoadedImageBase;
  CHAR16   NameString [NAME_STRING_LENGTH + 1];
} IMAGE_STRUCT;

typedef struct _IMAGE_HANDLE_DB {
  UINTN        ImageStructCountMax;
  UINTN        ImageStructCount;
  IMAGE_STRUCT ImageHandles [1];
} IMAGE_HANDLE_DB;

typedef struct _SPA_CTXT {
  UINT8               Valid;
  UINT8               RSVD1;
  UINT16              RSVD2;
  UINTN               ImageHandleDb;                  // Address to Image Handle Db Memory.
  UINTN               UsermodePage;                   // Address to user mode page for logging.
  UINTN               SpaPageTablePool;               // Address of page pool for SPA page fault handler.
  UINT32              SpaPageTablePoolSize;
  UINTN               SpaLogOutputCallback;           // SPA_LOG_OUTPUT_CB function pointers.
} SPA_CTXT;

// Checking structure size with actual number of bytes
C_ASSERT (sizeof (SPA_CTXT) == 40);


#pragma pack (pop)
#endif
