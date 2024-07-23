/**@file
  Definition of UFS erase block protocol

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

#ifndef __UFS_ERASE_BLOCK_PROTOCOL_H__
#define __UFS_ERASE_BLOCK_PROTOCOL_H__

#define UFS_ERASE_BLOCK_PROTOCOL_GUID \
  { \
    0x44a955ad, 0xd12b, 0x40bf, { 0x96, 0x63, 0x60, 0xab, 0x45, 0xbc, 0x46, 0x47 } \
  }

typedef struct _UFS_ERASE_BLOCK_PROTOCOL UFS_ERASE_BLOCK_PROTOCOL;

#define UFS_ERASE_BLOCK_PROTOCOL_REVISION ((2<<16) | (60))

///
/// UFS_ERASE_BLOCK_TOKEN
///
typedef struct {
  //
  // If Event is NULL, then blocking I/O is performed. If Event is not NULL and
  // non-blocking I/O is supported, then non-blocking I/O is performed, and
  // Event will be signaled when the erase request is completed.
  //
  EFI_EVENT     Event;
  //
  // Defines whether the signaled event encountered an error.
  //
  EFI_STATUS    TransactionStatus;
} UFS_ERASE_BLOCK_TOKEN;

//
// Erase type
//
typedef enum {
  UfsClear          = 0x1,
  UfsOverwrite,
  UfsBlockErase,
  UfsCryptoErase
} UFS_ERASE_TYPE;

//
// Clear type, after clear, set the buffer to zero/one/random
//
typedef enum {
  UfsClearZero     = 0x0,
  UfsClearOne,
  UfsClearRandom,
} UFS_CLEAR_TYPE;

#define UFS_MANUFACURER_MAX_LEN 20
#define UFS_SN_MAX_LEN          20
#define UFS_MN_MAX_LEN          40
#define UFS_SANITIZE_COUNT      4

//
// Data structure used to store
//
typedef struct {
  UINT32                  VidDid;
  CHAR8                   Manufacturer[UFS_MANUFACURER_MAX_LEN + 1];
  CHAR8                   ModelName[UFS_MN_MAX_LEN + 1];
  CHAR8                   SerialNumber[UFS_SN_MAX_LEN + 1];
  UFS_ERASE_TYPE          Method;
  EFI_STATUS              Result;
} UFS_ERASE_INFO;

/**
  A function used by the driver to report the progress of operation

  @param[in] Completion  A value between 1 and 100 indicating the current
                         completion progress of the erase operation.
                         Completion progress is reported as from 1 to 100
                         percent. A value of 0 is used by the driver to
                         indicate that progress reporting is not supported.

  @retval EFI_SUCCESS            The erase request was queued if Event is not
                                 NULL. The data was erased correctly to the
                                 device if the Event is NULL.to the device.
  @retval EFI_INVALID_PARAMETER  The erase request contains LBAs that are not
                                 valid.


**/
typedef
EFI_STATUS
(EFIAPI *UFS_ERASE_BLOCK_PROGRESS) (
IN UINTN Completion
);

/**
  Erase a specified number of device blocks.

  @param[in] This        Indicates a pointer to the calling context.
  @param[in] MediaId     The media ID that the erase request is for.
  @param[in] LBA         The starting logical block address to be
                         erased. The caller is responsible for erasing
                         only legitimate locations.
  @param[in,out] Token   A pointer to the token associated with the
                         transaction.
  @param[in] Size        The size in bytes to be erased. This must be
                         a multiple of the physical block size of the
                         device.
  @param[in] EraseType   Erase type will be used to erase device block.
  @param[in] Progress    Function pointer used by the driver to report
                         the progress of operation.

  @retval EFI_SUCCESS             The erase request was queued if Event is not
                                  NULL. The data was erased correctly to the
                                  device if the Event is NULL.to the device.
  @retval EFI_WRITE_PROTECTED     The device cannot be erased due to write
                                  protection.
  @retval EFI_DEVICE_ERROR        The device reported an error while attempting
                                  to perform the erase operation.
  @retval EFI_INVALID_PARAMETER   The erase request contains LBAs that are not
                                  valid.
  @retval EFI_NO_MEDIA            There is no media in the device.
  @retval EFI_MEDIA_CHANGED       The MediaId is not for the current media.

**/
typedef
EFI_STATUS
(EFIAPI *UFS_BLOCK_ERASE) (
  IN     UFS_ERASE_BLOCK_PROTOCOL        *This,
  IN     UINT32                           MediaId,
  IN     EFI_LBA                          LBA,
  IN OUT UFS_ERASE_BLOCK_TOKEN           *Token,
  IN     UINTN                            Size,
  IN     UFS_ERASE_TYPE                  EraseType,
  IN     UFS_ERASE_BLOCK_PROGRESS        Progress OPTIONAL
  );

/**
  Return the result of erase operation on the device.

  @param[in]  This         Indicates a pointer to the calling context.
  @param[out] EraseInfo    A pointer to the data of erasing device information.

  @retval EFI_SUCCESS             Get result of erase operation successfully.
  @retval EFI_NOT_FOUND           There is no result can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
typedef
EFI_STATUS
(EFIAPI *GET_UFS_ERASE_INFO) (
  IN  UFS_ERASE_BLOCK_PROTOCOL    *This,
  OUT VOID                        **EraseInfo
  );

/**
  Get the time.

  @param[in]  This          Indicates a pointer to the calling context.
  @param[in]  EraseType     Erase type will be used to erase device block.

  @retval UINT64            Time to erase
**/
typedef
UINT64
(EFIAPI *GET_UFS_ESTIMATE_TIME) (
  IN  UFS_ERASE_BLOCK_PROTOCOL      *This,
  IN  UFS_ERASE_TYPE                EraseType
  );

///
/// The Platform Erase Block Protocol provides the ability for a device to expose
/// erase functionality. This optional protocol is installed on the same handle
/// as the EFI_BLOCK_IO_PROTOCOL or EFI_BLOCK_IO2_PROTOCOL.
///
struct _UFS_ERASE_BLOCK_PROTOCOL {
  //
  // The revision to which the EFI_ERASE_BLOCK_PROTOCOL adheres. All future
  // revisions must be backwards compatible. If a future version is not
  // backwards compatible, it is not the same GUID.
  //
  UINT64                      Revision;
  //
  // Returns the erase length granularity as a number of logical blocks. A
  // value of 1 means the erase granularity is one logical block.
  //
  UINT32                      EraseLengthGranularity;
  UFS_BLOCK_ERASE             EraseBlocks;
  GET_UFS_ERASE_INFO          GetEraseInfo;
  GET_UFS_ESTIMATE_TIME       GetEstimateTime;
};

extern EFI_GUID gUfsEraseBlockProtocolGuid;

#endif
