/** @file
  Declares the interface for TbtNvmDrvHr class.
  This class represents TBT HR abstraction.

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
#ifndef TBT_NVM_DRV_HR_PROTOCOL_
#define TBT_NVM_DRV_HR_PROTOCOL_

#include <Protocol/PciIo.h>

#include <Library/TbtNvmDrvDma.h>
#include <TbtNvmRetimer.h>

typedef struct _TBT_HOST_ROUTER TBT_HOST_ROUTER;

typedef struct _TBT_HR_IMPL TBT_HR_IMPL;

typedef
TBT_STATUS
(*WRITE_CIO_DEV_REG) (
  IN TBT_HOST_ROUTER   *This,
  IN UINT8             ConfigurationSpace,
  IN UINT8             Adapter,
  IN UINT16            RegNum,
  IN UINT8             Length,
  IN UINT32            *DataPtr
  );

typedef
TBT_STATUS
(*READ_CIO_DEV_REG) (
  IN TBT_HOST_ROUTER   *This,
  IN UINT8             ConfigurationSpace,
  IN UINT8             Adapter,
  IN UINT16            RegNum,
  OUT UINT32           *DataPtr
  );

typedef
VOID
(*TBT_HR_DTOR) (
  IN TBT_HOST_ROUTER   *This
  );

typedef
TBT_STATUS
(*FORCE_PWR_HR) (
  IN  EFI_PCI_IO_PROTOCOL *PciIoProto,
  IN  BOOLEAN             Enable,
  OUT BOOLEAN             *TBTControllerWasPowered
  );

struct _TBT_HOST_ROUTER {
  TBT_HR_IMPL       *Impl;
  PCIE_RP_CONFIG    PcieRpConfig;
  UINT32            RefCount;
  UINT8             FirmwareType;
  PCIE_BDF          TbtDmaPcieBdf;
  FORCE_PWR_HR      ForcePwrFunc;
  WRITE_CIO_DEV_REG WriteCioDevReg;
  READ_CIO_DEV_REG  ReadCioDevReg;
  TBT_HR_DTOR       Dtor;
};

struct _TBT_HR_IMPL {
    EFI_PCI_IO_PROTOCOL *pPciIoProto;
    TBT_DMA             *pDma;
    FORCE_PWR_HR        ForcePwrFunc;
};

#endif /* TBT_NVM_DRV_HR_PROTOCOL_ */
