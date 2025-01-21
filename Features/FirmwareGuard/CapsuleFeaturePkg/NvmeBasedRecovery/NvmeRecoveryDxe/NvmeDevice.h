/**@file
  The file includes major RPMB access and Boot partion read/write commands
  based on NVMe1.4 spec.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Protocol/StorageSecurityCommand.h>
#include <Protocol/NvmExpressPassthru.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/BlockIo2.h>
#include <Protocol/DiskInfo.h>
#include <Library/TimerLib.h>
#include <Library/RngLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <IndustryStandard/Nvme.h>
#include "NvmeSecure.h"

#define SECURITY_PROTOCOL_INFO      0x00
#define SECURITY_PROTOCOL_NVME      0xEA

#define NVME_BAR                 0

#define PCI_CLASS_MASS_STORAGE_NVM                0x08  // mass storage sub-class non-volatile memory.
#define PCI_IF_NVMHCI                             0x02  // mass storage programming interface NVMHCI.

#define NVME_ASQ_SIZE                             1     // Number of admin submission queue entries, which is 0-based
#define NVME_ACQ_SIZE                             1     // Number of admin completion queue entries, which is 0-based

#define NVME_CSQ_SIZE                             1     // Number of I/O submission queue entries, which is 0-based
#define NVME_CCQ_SIZE                             1     // Number of I/O completion queue entries, which is 0-based

#define CFG_SPACE_BAR0_OFFSET                     0x10
#define CFG_SPACE_BAR1_OFFSET                     0x14

#define REG_BPINFO_SIZE                           sizeof (NVME_BPINFO)
#define REG_BPMBL_SIZE                            sizeof (NVME_BPMBL)
#define REG_BPRSEL_SIZE                           sizeof (NVME_BPRSEL)
//
// Number of asynchronous I/O submission queue entries, which is 0-based.
// The asynchronous I/O submission queue size is 4kB in total.
//
#define NVME_ASYNC_CSQ_SIZE                       63
//
// Number of asynchronous I/O completion queue entries, which is 0-based.
// The asynchronous I/O completion queue size is 4kB in total.
//
#define NVME_ASYNC_CCQ_SIZE                       255

#define NVME_MAX_QUEUES                           3     // Number of queues supported by the driver

#define NVME_CONTROLLER_ID                        0

//
// Time out value for Nvme transaction execution
//
#define NVME_GENERIC_TIMEOUT                  EFI_TIMER_PERIOD_SECONDS (5)

#define NVME_ALL_NAMESPACES                   0xFFFFFFFF

#define NVME_IFWI_IMAGE_SIZE                  0x2000000

#define NVME_128K_PORTION_WRITE_UNIT          0x20000
#define NVME_32K_PORTION_WRITE_UNIT           0x8000
#define NVME_16K_PORTION_WRITE_UNIT           0x4000
#define NVME_8K_PORTION_WRITE_UNIT            0x2000
#define NVME_4K_PORTION_WRITE_UNIT            0x1000
#define NVME_MINIMUM_PORTION_WRITE_UNIT       NVME_4K_PORTION_WRITE_UNIT

#define NVME_2MB_PORTION_READ_UNIT            0x200000
#define NVME_32K_PORTION_READ_UNIT            0x8000
#define NVME_16K_PORTION_READ_UNIT            0x4000
#define NVME_8K_PORTION_READ_UNIT             0x2000
#define NVME_4K_PORTION_READ_UNIT             0x1000

#define NVME_RPMB_DATA_FRAME_SIZE             sizeof (NVME_RPMB_DATA_FRAME)
#define NVME_RPMB_CONTENTS_SIZE               512
#define NVME_RPMB_TARGET_MIN_UNIT             512
#define NVME_RPMB_DATA_TOTAL_SIZE             NVME_RPMB_DATA_FRAME_SIZE + NVME_RPMB_CONTENTS_SIZE

#define WDC_SN530_PCIE_TIMEOUT                20000    // WDC SN530, FW version: 405FEE1F_GO

#define NVME_RPMB_ACCESS_SIZE_MASK            0xFF000000
#define NVME_RPMB_ACCESS_SIZE_SHIFT           24
//
// Time out value for Nvme Security Send / Receive
//
#define NVME_SSP_TIMEOUT                      EFI_TIMER_PERIOD_SECONDS (10)

//
// NVMe Security Specific Field RPMB Target value
// ref. Nvm Express 1.4 Figure 336
//
#define NSSF_RPMB_TARGET                      0x0001

#define DWORD(size) ((size) >> 2)

//
// 0's based Number of Dwords value
//
#define NUMD(size) (DWORD (size) - 1)

typedef enum {
  BPPcieRead    = 0,
  BPGetLogPage
} NVME_BOOT_PARTITION_READ_MODE;

//
// Get Log Page ? Boot Partition Information Log
// (ref. spec. v1.4++ Figure ??).
//
typedef struct {
  //
  // Display the content of Boot Partition.
  //
  UINT8                BPData[NVME_16K_PORTION_READ_UNIT];
} NVME_BOOT_PARTITION_INFO_LOG;

/**
  Nvme map one DMA buffer.

  @param PciIo          Pointer to PciIo for Nvme device.
  @param Address        DMA buffer address to be mapped.
  @param NumberOfBytes  Number of bytes to be mapped.

**/
VOID
NvmeMapOneDmaBuffer (
  IN EFI_PCI_IO_PROTOCOL        *PciIo,
  IN EFI_PHYSICAL_ADDRESS       Address,
  IN UINTN                      NumberOfBytes
  );

/**
  Allocates pages that are suitable for an OperationBusMasterCommonBuffer or
  OperationBusMasterCommonBuffer64 mapping.

  @param PciIo                  Pointer to PciIo for Nvme device.
  @param Pages                  The number of pages to allocate.
  @param Address                A pointer to store the base system memory address of the
                                allocated range.

  @retval EFI_SUCCESS           The requested memory pages were allocated.
  @retval EFI_UNSUPPORTED       Attributes is unsupported. The only legal attribute bits are
                                MEMORY_WRITE_COMBINE and MEMORY_CACHED.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The memory pages could not be allocated.

**/
EFI_STATUS
NvmeAllocateDmaBuffer (
  IN EFI_PCI_IO_PROTOCOL    *PciIo,
  IN UINTN                  BufferLength,
  OUT VOID                  **Address
  );
/**
  Return the result of RPMB Data Frame based on Figure 465: RPMB Operation Result in Nvme 1.4.

  @param  RpmbDataFrame          The pointer to the GetNVMeRPMBOperationResult data structure.

  @retval EFI_SUCCESS      Successfully read/write the RPMB Configuration Block.
  @retval EFI_DEVICE_ERROR Fail to read/write the RPMB Configuration Block.

**/
EFI_STATUS
GetNvmeRpmbOperationResult (
  IN NVME_RPMB_DATA_FRAME                     *RpmbDataFrame
  );

 /**
  The Firmware Image Download command is used to download all or a portion of an image for a future
  update to the controller.

  @param  Passthru      The pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL protocol.
  @param  NamespaceId   The NamespaceId for current device.
  @param  Buffer        The buffer used to store value read from an external image.
  @param  Length        The size of image.
  @param  DataOffset    Current dataoffset of the buffer in a loop.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
EFIAPI
NvmeStorageFirmwareDownload (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL       *Passthru,
  IN UINT32                                   NamespaceId,
  IN VOID                                     *Buffer,
  IN UINTN                                    Length,
  IN UINTN                                    DataOffset
  );

/**
  The Firmware Image Download command is used to download all or a portion of an image for a future
  update to the controller.

  @param  Passthru      The pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL protocol.
  @param  NamespaceId   The NamespaceId for current device.
  @param  CommitAction  This field specifies the action that is taken on the image downloaded with the Firmware Image
                        Download command or on a previously downloaded and placed image. (refer to Nvme1.4, Figure 175)

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
EFIAPI
NvmeStorageFirmwareCommit (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL     *Passthru,
  IN UINT32                                 NamespaceId,
  IN UINT32                                 CommitAction
  );

/**
  Read data from Nvm Express Boot Partition and save it to local memory.

  @param  PciIo             The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  BPRSEL            The configuration value of BPRSEL register.
  @param  CfgSpaceAddress   Current device's config space address.
  @param  IsVmd             VMD enabled flag.
  @param  HostMemoryBuffer  The buffer which is allocated by Host used to store value read from controller.

  @retval EFI_SUCCESS      Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR Fail to read the controller capability register.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex. Or BarIndex not valid for
                                this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_TIMEOUT           The operation is time out.

**/
EFI_STATUS
EFIAPI
NvmeBootPartitionReadToHostByPcie (
  IN EFI_PCI_IO_PROTOCOL                  *PciIo,
  IN NVME_BPRSEL                          BPRSEL,
  IN EFI_PHYSICAL_ADDRESS                 CfgSpaceAddress,
  IN BOOLEAN                              IsVmd,
  IN OUT VOID                             *HostMemoryBuffer
  );

/**
  Read Nvm Express Boot Partition data by Get Log Page command.

  @param  Passthru          The pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL data structure.
  @param  NamespaceId       The NamespaceId for current device.
  @param  HostMemoryBuffer  The buffer which is allocated by Host used to store value read from controller.
  @param  DataByWords       The data number for each read.
  @param  DataOffset        The current data offset for whole block read.

  @retval EFI_SUCCESS      Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR Fail to read the controller capability register.

**/
EFI_STATUS
EFIAPI
NvmeBootPartitionReadToHostByGetLogPage (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL   *Passthru,
  IN UINT32                               NamespaceId,
  IN OUT VOID                             *HostMemoryBuffer,
  IN UINTN                                DataByWords,
  IN UINTN                                DataOffset
  );

/**
  Authentication Key programming is initiated by a Security Send command to program the Authentication
  Key to the specified RPMB target, followed by a subsequent Security Send command to request the result,
  and lastly, the host issues a Security Receive command to retrieve the result.

  @param  Ssp        The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId    Id of the media, changes every time the media is replaced.
  @param  Key        The Authentication key should be updated to the specified RPMB target.
  @param  KeySize    Size of the key.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeAuthenticatedKeyDataProgramming (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  IN  CONST VOID                              *Key,
  IN  UINTN                                   KeySize
  );

/**
  The Read Write Counter Value sequence is initiated by a Security Send command to request the Write
  Counter value, followed by a Security Receive command to retrieve the Write Counter result.

  @param  Ssp            The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId        Id of the media, changes every time the media is replaced.
  @param  RpmbTarget     Rpmb target to access.
  @param  ReceiveBuffer  The buffer used to store RPMB data frame value read from controller.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NVMeRPMBReadWriteCounterValue (
  IN     EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN     UINT32                                  MediaId,
  IN     UINT8                                   RpmbTarget,
  IN OUT NVME_RPMB_DATA_FRAME                    *ReceiveBuffer
  );

/**
  The Authenticated Data Read implementation initiated by a Security Send command.

  @param  Ssp                The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId            Id of the media, changes every time the media is replaced.
  @param  RpmbTarget         Rpmb target to access.
  @param  ReceiveBuffer      The pointer to the The data Number of 512B blocks.
  @param  ReceiveBufferSize  The size of RPMB configuration block data.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeAuthenticatedDataRead (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  IN  UINT8                                   RpmbTarget,
  OUT VOID                                    *ReceiveBuffer,
  IN  UINTN                                   ReceiveBufferSize
  );

/**
  The Authenticated Data Write implementation initiated by a Security Send command.

  @param  Ssp                          The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId                      Id of the media, changes every time the media is replaced.
  @param  RpmbTarget                   Rpmb target to access.
  @param  RpmbTargetData               The pointer to the The data Number of 512B blocks.
  @param  ReceiveBufferSize            The size of RPMB configuration block data.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeAuthenticatedDataWrite (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  IN  UINT8                                   RpmbTarget,
  IN  VOID                                    *RpmbTargetData,
  IN  UINT32                                  ReceiveBufferSize
  );

/**
  The Authenticated Device Configuration Block Read implementation initiated by a Security Send command.

  @param  Ssp                The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId            Id of the media, changes every time the media is replaced.
  @param  ReceiveBuffer      The buffer used to store RPMB configuration block data read from controller.
  @param  ReceiveBufferSize  The size of RPMB configuration block data.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeAuthenticatedDeviceConfigurationBlockRead (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  OUT VOID                                    *ReceiveBuffer,
  IN  UINT32                                  ReceiveBufferSize
  );

/**
  The Authenticated Device Configuration Block Write implementation initiated by a Security Send command.

  @param  Ssp                          The pointer to the EFI_STORAGE_SECURITY_COMMAND_PROTOCOL protocol.
  @param  MediaId                      Id of the media, changes every time the media is replaced.
  @param  RPMBDeviceConfigurationData  The buffer used to store RPMB configuration block data is ready to write
                                       to the configuration register of controller.
  @param  ReceiveBufferSize            The size of RPMB configuration block data.

  @retval EFI_SUCCESS                  Successfully read the controller capability register content.
  @retval EFI_OUT_OF_RESOURCES         There is not enough memory resource.
  @retval EFI_INVALID_PARAMETER        The size of input key exceeded the scope of definition in NVMe1.4 spec,
                                       or the PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize
                                       is non-zero.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
EFI_STATUS
NvmeAuthenticatedDeviceConfigurationBlockWrite (
  IN  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *Ssp,
  IN  UINT32                                  MediaId,
  IN  VOID                                    *RPMBDeviceConfigurationData,
  IN  UINT32                                  ReceiveBufferSize
  );

/**
  Read Nvm Express Controller Registers data with 32-bit width.

  @param  PciIo            The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  NVMeReg          The buffer used to store value read from according the offset of registers.
  @param  RegOffset        The controller register offset.

  @retval EFI_SUCCESS           Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR      Fail to read the controller capability register.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex. Or BarIndex not valid for
                                this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/

EFI_STATUS
ReadNvmeController32 (
  IN EFI_PCI_IO_PROTOCOL                  *PciIo,
  IN OUT VOID                             *NVMeReg,
  IN UINT32                               RegOffset
  );

/**
  Write Nvm Express Controller Registers data with 32-bit width.

  @param  PciIo            The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  NVMeReg          The buffer used to store value read from according the offset of registers.
  @param  RegOffset        The controller register offset.

  @retval EFI_SUCCESS           Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR      Fail to read the controller capability register.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex. Or BarIndex not valid for
                                this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/
EFI_STATUS
WriteNvmeController32(
  IN EFI_PCI_IO_PROTOCOL                  *PciIo,
  IN VOID                                 *NVMeReg,
  IN UINT32                               RegOffset
  );

/**
  Read Nvm Express Controller Registers data with 64-bit width.

  @param  PciIo            The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  NVMeReg          The buffer used to store value read from according the offset of registers.
  @param  RegOffset        The controller register offset.

  @retval EFI_SUCCESS           Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR      Fail to read the controller capability register.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex. Or BarIndex not valid for
                                this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/
EFI_STATUS
ReadNvmeController64 (
  IN EFI_PCI_IO_PROTOCOL                  *PciIo,
  IN OUT VOID                             *NVMeReg,
  IN UINT32                               RegOffset
  );

 /**
  Write Nvm Express Controller Registers data with 64-bit width.

  @param  PciIo            The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  NVMeReg          The buffer used to store value read from according the offset of registers.
  @param  RegOffset        The controller register offset.

  @retval EFI_SUCCESS           Successfully read the controller capability register content.
  @retval EFI_DEVICE_ERROR      Fail to read the controller capability register.
  @retval EFI_UNSUPPORTED       The address range specified by Offset, Width, and Count is not
                                valid for the PCI BAR specified by BarIndex. Or BarIndex not valid for
                                this PCI controller.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.

**/
EFI_STATUS
WriteNvmeController64(
  IN EFI_PCI_IO_PROTOCOL                  *PciIo,
  IN VOID                                 *NVMeReg,
  IN UINT32                               RegOffset
  );

/**
  Get identify controller data.

  @param  Passthru         The pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL data structure.
  @param  NamespaceId       Current device NameSpaceId.
  @param  Buffer           The buffer used to store the identify controller data.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeIdentifyController2 (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *Passthru,
  IN UINT32                             NamespaceId,
  IN VOID                               *Buffer
  );

/**
  Dump NVMe identify information.

  @param  Passthru          The pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL data structure.
  @param  NamespaceId       Current device NameSpaceId.
  @param  ControllerData    output for current device's controller data.

  @retval EFI_SUCCESS      Successfully get the identify controller data.
  @retval EFI_NOT_FOUND    Fail to find NVMe controller.

**/
EFI_STATUS
DumpNvmeIdentifyControllerData (
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *Passthru,
  IN UINT32                              NamespaceId,
  OUT NVME_ADMIN_CONTROLLER_DATA         *ControllerData
  );


/**
  Write data to Boot partition after partition is unlocked.

  @param  Passthru                   A pointer to EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
  @param  NamespaceId                Current device NameSpaceId.
  @param  FirmwareImageBuffer        A pointer to the data should be saved to BP.
  @param  FileSize                   The size of image in memory.
  @param  WriteLength                Size of per chunk.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeBootPartitionWrite (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL      *Passthru,
  IN  UINT32                                  NamespaceId,
  IN  UINT8                                   *FirmwareImageBuffer,
  IN  UINTN                                   FileSize,
  IN  UINTN                                   WriteLength
  );

/**
  Read data from Boot partition thru Get Log Page command, caller should free the memory.

  @param  Passthru              A pointer to EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
  @param  NamespaceId           Current device NameSpaceId.
  @param  HostMemoryBuffer      A pointer to memory can be saved the data read from BP.
  @param  NvmeRecBPSize         The size of image in memory.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeBootPartitionGetLogPageRead (
  IN     EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL      *Passthru,
  IN     UINT32                                  NamespaceId,
  IN OUT VOID                                    **HostMemoryBuffer,
  IN OUT UINTN                                   NvmeRecBPSize
  );

/**
  Read data from Boot partition thru PCIE interface, caller should free the memory.

  @param  PciIo             The pointer to the EFI_PCI_IO_PROTOCOL data structure.
  @param  CfgSpaceAddress   Current device's config space address.
  @param  IsVmd             VMD enabled flag.
  @param  HostMemoryBuffer  A pointer to memory can be saved the data read from BP.
  @param  NvmeRecBPSize     The size of image in memory.

  @retval EFI_SUCCESS                The NVM Express Command Packet was sent by the host. TransferLength bytes were transferred
                                     to, or from DataBuffer.
  @retval EFI_BAD_BUFFER_SIZE        The NVM Express Command Packet was not executed. The number of bytes that could be transferred
                                     is returned in TransferLength.
  @retval EFI_NOT_READY              The NVM Express Command Packet could not be sent because the controller is not ready. The caller
                                     may retry again later.
  @retval EFI_DEVICE_ERROR           A device error occurred while attempting to send the NVM Express Command Packet.
  @retval EFI_INVALID_PARAMETER      NamespaceId or the contents of EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET are invalid. The NVM
                                     Express Command Packet was not sent, so no additional status information is available.
  @retval EFI_UNSUPPORTED            The command described by the NVM Express Command Packet is not supported by the NVM Express
                                     controller. The NVM Express Command Packet was not sent so no additional status information
                                     is available.
  @retval EFI_TIMEOUT                A timeout occurred while waiting for the NVM Express Command Packet to execute.

**/
EFI_STATUS
NvmeBootPartitionPcieRead (
  IN     EFI_PCI_IO_PROTOCOL                       *PciIo,
  IN     EFI_PHYSICAL_ADDRESS                      CfgSpaceAddress,
  IN     BOOLEAN                                   IsVmd,
  IN OUT VOID                                      **HostMemoryBuffer,
  IN OUT UINTN                                     NvmeRecBPSize
  );
