/** @file
  Nvme drive Health implementation head file.

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

#ifndef _DXE_NVME_H_
#define _DXE_NVME_H_

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <IndustryStandard/Nvme.h>
#include <Protocol/NvmExpressPassthru.h>
#include <TelemetryCommon.h>
#include <Protocol/PciIo.h>
#include <Library/VmdInfoLib.h>
#include <Library/DevicePathLib.h>

#define NVME_ALL_NAMESPACES             0xFFFFFFFF
#define NVME_CONTROLLER_ID              0
#define NVME_LOG_MAX_SIZE               1024
#define NVME_GENERIC_TIMEOUT            EFI_TIMER_PERIOD_SECONDS (5)

#define NVME_SMART_HEALTH_LOG_MAX_SIZE            512
#define NVME_CRITICALL_WARNING                    0
#define NVME_ENDURANCE_GROUP_CRITICALL_WARNING    6
#define NVME_MEDIA_DATA_INTEGRITY_ERRORS_START    160
#define NVME_MEDIA_DATA_INTEGRITY_ERRORS_END      175

#define NVME_HEALTH_INSTANCE_DATA_SIGNATURE  SIGNATURE_32 ('N', 'V', 'M', 'H')

///
/// structure Revision (as defined in Telemetry Gen 2.0 spec.)
///
#define NVME_AIP_HEADER_REVISION  0x01
#define NVME_AIP_DATA_REVISION    0x01

typedef struct {
  UINT32                                      Signature;
  EFI_HANDLE                                  Handle;
  EFI_ADAPTER_INFORMATION_PROTOCOL            AdapterInfo;
  EFI_DEVICE_PATH_PROTOCOL                    *NvmeHealthDevicePath;
} NVME_HEALTH_DEV;

#define NVME_HEALTH_INSTANCE_FROM_THIS(a) \
  CR ( \
  (a), \
  NVME_HEALTH_DEV, \
  AdapterInfo, \
  NVME_HEALTH_INSTANCE_DATA_SIGNATURE \
  )

/**
  Implement 'Get Log Page' 02h(SMART / Health Information) Command.

  @param[in]   NvmeDevice   The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in]   NamespaceId  NamespaceId for an NVM Express namespace present on the
                            NVM Express controller
  @param[out]  Buffer       The buffer used to store the log data.

  @return EFI_SUCCESS           Successfully get the identify controller data.
  @return EFI_DEVICE_ERROR      Fail to get the identify controller data.
  @return EFI_OUT_OF_RESOURCES  Unable to allocate required resources.

**/
EFI_STATUS
NvmeGetLogPageSmartInfo (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmeDevice,
  IN  UINT32                                NamespaceId,
  OUT VOID                                  *Buffer
  );

#endif // #ifndef _DXE_NVME_H_

