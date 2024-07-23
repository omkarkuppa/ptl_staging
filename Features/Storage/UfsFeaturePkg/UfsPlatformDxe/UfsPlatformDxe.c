/** @file
  UFS platform driver for Intel platforms.

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
#include "UfsPlatformHci.h"
#include <Uefi.h>
#include <Uefi/UefiMultiPhase.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include <Protocol/UfsHostControllerPlatform.h>

#define UFS_PLATFORM_INTEL_VENDOR_ID       0x8086
#define UFS_PLATFORM_INTEL_MTLM_DEV_ID     0x7E47
#define UFS_PLATFORM_INTEL_ARLM_DEV_ID     0x7747
#define UFS_PLATFORM_INTEL_LNLM_DEV_ID     0xA847
#define UFS_PLATFORM_INTEL_PTLPCDP_DEV_ID  0xE447

#define UFS_UIC_TIMEOUT                 5000

/**
  Fill UIC Command associated fields.

  @param[in] UicOpcode        The opcode of the UIC command.
  @param[in] Arg1Mib          The value for MIB Attribute of UIC command 1st argument.
  @param[in] Arg1GenSel       The value for Gen Selector Index of UIC command 1st argument.
  @param[in] Arg2             The value for UIC command 2nd argument.
  @param[in] Arg3             The value for UIC command 3rd argument.
**/
VOID
UfsFillUicCommand (
  IN     UINT8             UicOpcode,
  IN     UINT16            Arg1Mib,
  IN     UINT16            Arg1GenSel,
  IN     UINT32            Arg2,
  IN     UINT32            Arg3,
  IN OUT EDKII_UIC_COMMAND *Cmd
  )
{
  Cmd->Opcode = UicOpcode;
  Cmd->Arg1   = UFS_UIC_ARG_MIB_SEL (Arg1Mib, Arg1GenSel);
  Cmd->Arg2   = Arg2;
  Cmd->Arg3   = Arg3;
}

/**
  Read 32bits data from specified UFS MMIO register.

  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver
  @param[in]  Offset        The offset within the UFS Host Controller MMIO space to start
                            the memory operation.
  @param[out] Value         The data buffer to store.

  @retval EFI_TIMEOUT       The operation is time out.
  @retval EFI_SUCCESS       The operation succeeds.
  @retval Others            The operation fails.

**/
EFI_STATUS
UfsMmioRead32 (
  IN EDKII_UFS_HC_DRIVER_INTERFACE      *UfsHcDriverInterface,
  IN UINTN                               Offset,
    OUT UINT32                           *Value
  )
{
  EFI_STATUS                          Status;
  EDKII_UFS_HOST_CONTROLLER_PROTOCOL  *UfsHc;

  UfsHc = UfsHcDriverInterface->UfsHcProtocol;

  Status = UfsHc->Read (UfsHc, EfiUfsHcWidthUint32, Offset, 1, Value);

  return Status;
}

/**
  Write 32bits data to specified UFS MMIO register.

  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver
  @param[in] Offset         The offset within the UFS Host Controller MMIO space to start
                            the memory operation.
  @param[in] Value          The data to write.

  @retval EFI_TIMEOUT       The operation is time out.
  @retval EFI_SUCCESS       The operation succeeds.
  @retval Others            The operation fails.

**/
EFI_STATUS
UfsMmioWrite32 (
  IN  EDKII_UFS_HC_DRIVER_INTERFACE      *UfsHcDriverInterface,
  IN  UINTN                              Offset,
  IN  UINT32                             Value
  )
{
  EFI_STATUS                          Status;
  EDKII_UFS_HOST_CONTROLLER_PROTOCOL  *UfsHc;

  UfsHc = UfsHcDriverInterface->UfsHcProtocol;

  Status = UfsHc->Write (UfsHc, EfiUfsHcWidthUint32, Offset, 1, &Value);

  return Status;
}

/**
  Wait for the value of the specified system memory set to the test value.

  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver
  @param[in]  Offset        The offset within the UFS Host Controller MMIO space to start
                            the memory operation.
  @param[in]  MaskValue     The mask value of memory.
  @param[in]  TestValue     The test value of memory.
  @param[in]  Timeout       The time out value for wait memory set, uses 100ns as a unit.

  @retval EFI_TIMEOUT       The system memory setting is time out.
  @retval EFI_SUCCESS       The system memory is correct set.
  @retval Others            The operation fails.

**/
EFI_STATUS
UfsWaitMemSet (
  IN  EDKII_UFS_HC_DRIVER_INTERFACE      *UfsHcDriverInterface,
  IN  UINTN                              Offset,
  IN  UINT32                             MaskValue,
  IN  UINT32                             TestValue,
  IN  UINT64                             Timeout
  )
{
  UINT32     Value;
  UINT64     Delay;
  BOOLEAN    InfiniteWait;
  EFI_STATUS Status;

  if (Timeout == 0) {
    InfiniteWait = TRUE;
  } else {
    InfiniteWait = FALSE;
  }

  Delay = DivU64x32 (Timeout, 10) + 1;

  do {
    //
    // Access PCI MMIO space to see if the value is the tested one.
    //
    Status = UfsMmioRead32 (UfsHcDriverInterface, Offset, &Value);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Value &= MaskValue;

    if (Value == TestValue) {
      return EFI_SUCCESS;
    }

    //
    // Stall for 1 microseconds.
    //
    MicroSecondDelay (1);

    Delay--;

  } while (InfiniteWait || (Delay > 0));

  return EFI_TIMEOUT;
}

/**
  Starts UIC Command sequence for Attribute Access (B)
  This is a proprietary sequence where each MPHY CR register access requires six UIC commands.
  The MPHY CR Register address and the write data needs to be embedded with RMMI Attributes
  that are accessed using these UIC commands.

  @param[in] UfsHcDriverInterface    Interface to the functionalities provided by core driver
  @param[in] RegisterAddress         MPHY CR Register Addr
  @param[in] Data                    MPHY CR Register Write/Read Data
  @param[in] WriteSelection          '1' CR Write, '0' CR Read

**/
EFI_STATUS
UfsUicAttributeAccessB (
  IN EDKII_UFS_HC_DRIVER_INTERFACE *UfsHcDriverInterface,
  IN UINT16                        RegisterAddress,
  IN OUT UINT16                    *Data16,
  IN UINT8                         WriteSelection
  )
{
  EFI_STATUS         Status;
  EDKII_UIC_COMMAND  UicCommand;

  UfsFillUicCommand (UfsUicDmeSet, CBCREGADDRLSB, 0, 0, RegisterAddress & 0x00FF, &UicCommand);
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UfsUicAttributeAccessB Failed to Set UfsUicDmeSet CBCREGADDRLSB(0x%X) Reg.Addr LSB (0x%X)\n", CBCREGADDRLSB, RegisterAddress & 0x00FF));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeSet, CBCREGADDRMSB, 0, 0, (RegisterAddress & 0xFF00) >> 8, &UicCommand);
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UfsUicAttributeAccessB Failed to Set UfsUicDmeSet CBCREGADDRMSB(0x%X) Reg.Addr MSB (0x%X)\n", CBCREGADDRMSB, RegisterAddress & 0xFF00));
    return Status;
  }

  if (WriteSelection == 1) {
    //
    // Write Operation
    //
    UfsFillUicCommand (UfsUicDmeSet, CBCREGWRLSB, 0, 0, *Data16 & 0x00FF, &UicCommand);
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UfsUicAttributeAccessB Failed to Set UfsUicDmeSet CBCREGWRLSB(0x%x) Data16 LSB: 0x%X\n", CBCREGWRLSB, *Data16 & 0x00FF));
      return Status;
    }
    UfsFillUicCommand (UfsUicDmeSet, CBCREGWRMSB, 0, 0, (*Data16 & 0xFF00) >> 8, &UicCommand);
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UfsUicAttributeAccessB Failed to Set UfsUicDmeSet CBCREGWRMSB(0x%x) Data16 MSB: 0x%X\n", CBCREGWRMSB, (*Data16 & 0xFF00) >> 8));
      return Status;
    }
    UfsFillUicCommand (UfsUicDmeSet, CBCREGRDWRSEL, 0, 0, 1, &UicCommand);
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UfsUicAttributeAccessB Failed to Set UfsUicDmeSet CBCREGRDWRSEL(0x%x) for Write\n", CBCREGRDWRSEL));
      return Status;
    }

    UfsFillUicCommand (UfsUicDmeSet, VS_MPHY_CFG_UPDT, 0, 0, 1, &UicCommand); // MPHY_CFG_UPDT_CMD = 0x1
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UfsUicAttributeAccessB Failed to Set UfsUicDmeSet VS_MPHY_CFG_UPDT(0x%x) MPHY_CFG_UPDT_CMD: 0x1\n", VS_MPHY_CFG_UPDT));
      return Status;
    }
  } else {
    //
    // Read Operation
    //
    UfsFillUicCommand (UfsUicDmeSet, CBCREGRDWRSEL, 0, 0, 0, &UicCommand);
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UfsUicAttributeAccessB Failed to Set UfsUicDmeSet CBCREGRDWRSEL(0x%x) for Read\n", CBCREGRDWRSEL));
      return Status;
    }
    UfsFillUicCommand (UfsUicDmeGet, CBCREGRDLSB, 0, 0, 0, &UicCommand);
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UfsUicAttributeAccessB Failed to Set UfsUicDmeGet CBCREGRDLSB(0x%x) UicCommand LSB: 0x%X\n", CBCREGRDLSB, UicCommand.Arg3));
      return Status;
    }
    *Data16 = (UINT16) UicCommand.Arg3 & 0x00FF;
    UfsFillUicCommand (UfsUicDmeGet, CBCREGRDMSB, 0, 0, 0, &UicCommand);
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "UfsUicAttributeAccessB Failed to Set UfsUicDmeGet CBCREGRDMSB(0x%x) UicCommand MSB: 0x%X\n", CBCREGRDMSB, UicCommand.Arg3));
      return Status;
    }
    *Data16 = *Data16 | (((UINT8) UicCommand.Arg3) << 8);
  }

  return EFI_SUCCESS;
}

/**
  Detects the connected Ufs Lanes and Programs the Active Lanes Attribute.

  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver

  @retval EFI_SUCCESS  Successfully Programmed the lanes
  @retval others       Failed to program the lanes
**/
EFI_STATUS
UfsActivateAllLanes (
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  )
{
  EFI_STATUS         Status;
  UINT32             ConnectedLanes = 0;
  UINT32             ActiveLanes = 0;
  EDKII_UIC_COMMAND  UicCommand;

  //Step 1: Query connected and Active Rx lanes and make all conntected Rx lanes Active.
  UfsFillUicCommand (UfsUicDmeGet, PA_ConnectedRxDataLanes, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_ConnectedRxDataLanes(%x) \n", PA_ConnectedRxDataLanes));
    return Status;
  }
  ConnectedLanes = UicCommand.Arg3;

  UfsFillUicCommand (UfsUicDmeGet, PA_ActiveRxDataLanes, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_ActiveRxDataLanes(%x) \n", PA_ActiveRxDataLanes));
    return Status;
  }
  ActiveLanes = UicCommand.Arg3;

  if (ActiveLanes < ConnectedLanes) {
    DEBUG ((DEBUG_INFO, "UfsActivateAllLanes : PA_ActiveRxDataLanes = %x  PA_ConnectedRxDataLanes = %x\n", ActiveLanes, ConnectedLanes));
    UfsFillUicCommand (UfsUicDmeSet, PA_ActiveRxDataLanes, 0, 0, ConnectedLanes, &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    DEBUG ((DEBUG_INFO, "Ufs UfsUicDmeSet Status=%r PA_ActiveRxDataLanes(0x%X) Value: 0x%X \n", Status, PA_ActiveRxDataLanes, ConnectedLanes));
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //Step 2: Query connected and Active Tx lanes and make all conntected Tx lanes Active.
  UfsFillUicCommand (UfsUicDmeGet, PA_ConnectedTxDataLanes, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_ConnectedTxDataLanes(%x)\n", PA_ConnectedTxDataLanes));
    return Status;
  }
  ConnectedLanes = UicCommand.Arg3;

  UfsFillUicCommand (UfsUicDmeGet, PA_ActiveTxDataLanes, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_ActiveTxDataLanes(%x) \n", PA_ActiveTxDataLanes));
    return Status;
  }
  ActiveLanes = UicCommand.Arg3;

  if (ActiveLanes < ConnectedLanes) {
    DEBUG ((DEBUG_INFO, "UfsActivateAllLanes : PA_ActiveTxDataLanes = %x  PA_ConnectedTxDataLanes = %x\n", ActiveLanes, ConnectedLanes));
    UfsFillUicCommand (UfsUicDmeSet, PA_ActiveTxDataLanes, 0, 0, ConnectedLanes, &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    DEBUG ((DEBUG_INFO, "Ufs UfsUicDmeSet Status=%r PA_ActiveTxDataLanes(0x%X) Value: 0x%X \n", Status, PA_ActiveTxDataLanes, ConnectedLanes));
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return Status;
}

/**
  Dumps the link Configuration.
  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver

  @retval EFI_SUCCESS  Successfully switched to fast mode
  @retval others       Failed to switch to fast mode
**/
EFI_STATUS
UfsDumpLinkConfig (
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  )
{
  EFI_STATUS         Status;
  EDKII_UIC_COMMAND  UicCommand;

  DEBUG ((DEBUG_INFO, "UfsDumpLinkConfig Entry \n"));

  UfsFillUicCommand (UfsUicDmeGet, PA_Local_TX_LCC_Enable, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_Local_TX_LCC_Enable(%x)\n", PA_Local_TX_LCC_Enable));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_Local_TX_LCC_Enable(%x) = %x \n", PA_Local_TX_LCC_Enable, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_MaxRxHSGear, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_MaxRxHSGear(%x)\n", PA_MaxRxHSGear));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_MaxRxHSGear(%x) = %x \n", PA_MaxRxHSGear, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmePeerGet, PA_MaxRxHSGear, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmePeerGet PA_MaxRxHSGear(%x)\n", PA_MaxRxHSGear));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmePeerGet PA_MaxRxHSGear(%x) = %x \n", PA_MaxRxHSGear, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_MaxRxPWMGear, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_MaxRxPWMGear(%x)\n", PA_MaxRxPWMGear));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_MaxRxPWMGear(%x) = %x \n", PA_MaxRxPWMGear, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmePeerGet, PA_MaxRxPWMGear, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmePeerGet PA_MaxRxPWMGear(%x)\n", PA_MaxRxPWMGear));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmePeerGet PA_MaxRxPWMGear(%x) = %x \n", PA_MaxRxPWMGear, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_AvailTxDataLanes, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_AvailTxDataLanes(%x)\n", PA_AvailTxDataLanes));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_AvailTxDataLanes(%x) = %x \n", PA_AvailTxDataLanes, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_AvailRxDataLanes, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_AvailRxDataLanes(%x)\n", PA_AvailRxDataLanes));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_AvailRxDataLanes(%x) = %x \n", PA_AvailRxDataLanes, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_ConnectedTxDataLanes, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_ConnectedTxDataLanes(%x)\n", PA_ConnectedTxDataLanes));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_ConnectedTxDataLanes(%x) = %x \n", PA_ConnectedTxDataLanes, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_ConnectedRxDataLanes, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_ConnectedRxDataLanes(%x)\n", PA_ConnectedRxDataLanes));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_ConnectedRxDataLanes(%x) = %x \n", PA_ConnectedRxDataLanes, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_ActiveTxDataLanes, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_ActiveTxDataLanes(%x)\n", PA_ActiveTxDataLanes));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_ActiveTxDataLanes(%x) = %x \n", PA_ActiveTxDataLanes, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_ActiveRxDataLanes, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_ActiveRxDataLanes(%x)\n", PA_ActiveRxDataLanes));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_ActiveRxDataLanes(%x) = %x \n", PA_ActiveRxDataLanes, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_TxPWRStatus, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_TxPWRStatus(%x)\n", PA_TxPWRStatus));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_TxPWRStatus(%x) = %x \n", PA_TxPWRStatus, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_RxPWRStatus, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_RxPWRStatus(%x)\n", PA_RxPWRStatus));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_RxPWRStatus(%x) = %x \n", PA_RxPWRStatus, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_TXGear, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_TXGear(%x)\n", PA_TXGear));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_TXGear(%x) = %x \n", PA_TXGear, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_TxTermination, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_TxTermination(%x)\n", PA_TxTermination));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_TxTermination(%x) = %x \n", PA_TxTermination, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_HSSeries, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_HSSeries(%x)\n", PA_HSSeries));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_HSSeries(%x) = %x \n", PA_HSSeries, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_RXGear, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_RXGear(%x)\n", PA_RXGear));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_RXGear(%x) = %x \n", PA_RXGear, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_RxTermination, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_RxTermination(%x)\n", PA_RxTermination));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_RxTermination(%x) = %x \n", PA_RxTermination, UicCommand.Arg3));

  UfsFillUicCommand (UfsUicDmeGet, PA_HSSeries, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_HSSeries(%x)\n", PA_HSSeries));
  }
  DEBUG ((DEBUG_INFO, "UfsUicDmeGet PA_HSSeries(%x) = %x \n", PA_HSSeries, UicCommand.Arg3));

  DEBUG ((DEBUG_INFO, "UfsDumpLinkConfig Exit \n"));
  return EFI_SUCCESS;
}

/**
  Checks if given UFS controller is an Intel design.

  @param[in] PciIo  PciIo protocol handle for the controller

  @retval TRUE   Device is Intel design
  @retval FALSE  Device is not Intel design
**/
BOOLEAN
IsIntelUfsDevice (
  IN EFI_PCI_IO_PROTOCOL  *PciIo
  )
{
  EFI_STATUS  Status;
  UINT16      VendorId;

  Status = PciIo->Pci.Read (
    PciIo,
    EfiPciIoWidthUint16,
    PCI_VENDOR_ID_OFFSET,
    1,
    &VendorId
    );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if (VendorId == UFS_PLATFORM_INTEL_VENDOR_ID) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Allows platform protocol to override host controller information

  @param[in]      ControllerHandle  Handle of the UFS controller.
  @param[in, out] HcInfo            Pointer EDKII_UFS_HC_INFO associated with host controller.

  @retval EFI_SUCCESS            Function completed successfully.
  @retval EFI_INVALID_PARAMETER  HcInfo is NULL.
  @retval Others                 Function failed to complete.
**/
EFI_STATUS
EFIAPI
UfsHcPlatformOverrideHcInfo (
  IN     EFI_HANDLE         ControllerHandle,
  IN OUT EDKII_UFS_HC_INFO  *HcInfo
  )
{
  return EFI_SUCCESS;
}

/**
  Applies platform specific programming after the driver
  has enabled the host controller.

  @param[in] ControllerHandle      Handle of the UFS controller
  @param[in] PciIo                 Associated PciIo protocol
  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver
**/
VOID
UfsHcPlatformPostHce (
  IN EFI_HANDLE                     ControllerHandle,
  IN EFI_PCI_IO_PROTOCOL            *PciIo,
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  )
{
  EDKII_UIC_COMMAND  LccDisableCommand;
  EFI_STATUS         Status;

  if (UfsHcDriverInterface == NULL) {
    return;
  }

  //
  // For all supported integrated controllers we have to disable the
  // line configuration check(LCC).
  //
  UfsFillUicCommand (UfsUicDmeSet, PA_Local_TX_LCC_Enable, 0, 0, 0, &LccDisableCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &LccDisableCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UfsHcPlatformPostHce: Failed to Set UfsUicDmeSet PA_Local_TX_LCC_Enable(%x) %r \n", PA_Local_TX_LCC_Enable, Status));
  }
}

/**
  Programs the recipe required for High speed link Unipro C-Port connection programming

  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver

  @retval EFI_SUCCESS  Successfully programmed the recipe
  @retval others       Failed to program the recipe
**/
EFI_STATUS
UfsHsUniproCPortRecipieProgramming (
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  )
{
  EFI_STATUS         Status;
  EDKII_UIC_COMMAND  UicCommand;
  UINT32             Data = 0;
  UINT32             Timeout = UFS_UIC_TIMEOUT;

  while ((Data != 1) && Timeout != 0) {
    UfsFillUicCommand (UfsUicDmeSet, T_ConnectionState, 0, 0, 0x0, &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "T_ConnectionState = 0 failed\n"));
      return Status;
    }
    UfsFillUicCommand (UfsUicDmeSet, T_CPortFlags, 0, 0, 0x6, &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "T_CPortFlags = 6 failed\n"));
      return Status;
    }
    UfsFillUicCommand (UfsUicDmeSet, T_ConnectionState, 0, 0, 0x1, &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "T_ConnectionState = 1 failed\n"));
      return Status;
    }
    UfsFillUicCommand (UfsUicDmeGet, T_ConnectionState, 0, 0, 0x0, &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "DmeGet for T_ConnectionState failed\n"));
      return Status;
    }
    Data = UicCommand.Arg3;
    MicroSecondDelay (1);
    Timeout--;
  }
  DEBUG((DEBUG_INFO, "UfsHsUniproCPortRecipieProgramming, Status = %r Data=%d Timeout=%d\n", Status, Data, Timeout));
  return Status;
}

/**
  Programs the PHY recipe required for High speed link

  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver

  @retval EFI_SUCCESS  Successfully programmed the recipe
  @retval others       Failed to program the recipe
**/
EFI_STATUS
UfsHsPhyRecipieProgramming (
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  )
{
  EFI_STATUS                               Status;
  EDKII_UIC_COMMAND                        UicCommand;
  UINT16                                   Data16;
  UINT16                                   UfsUicTimeout;
  UFS_MPHY_RAWLANEN_DIG_PCS_XF_RX_PCS_OUT  DigPcsXfRxPcsOut;

  DEBUG((DEBUG_INFO, "UfsHsPhyRecipieProgramming \n"));

  //
  // Step 1: Override RMMI (RMMI Reference M-PHY MODULE Interface) CBRATESEL with the desired rate (Rate B)
  // UIC Attribute Access (A): (Attribute Name: CBRATESEL, AttrID: 0x8114, RxLaneSelect: 0, Value: 0x01) //Rate B
  //
  DEBUG ((DEBUG_INFO, "Override RMMI (RMMI Reference M-PHY MODULE Interface) CBRATESEL with the desired rate (Rate B)\n"));
  UfsFillUicCommand (UfsUicDmeSet, CBRATESEL, 0, 0, 0x1, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set CBRATESEL(%x) \n", CBRATESEL));
    return Status;
  }

  //
  // Step 2: Set TX_CFGUPDT_0 to 1'b1 for one TX_CFGCLK_0 cycle.
  // UIC Attribute Access (A): (Attribute Name: VS_MphyCfgUpdt, AttrID: 0xD085, RxLaneSelect: 0x0, Value 0x1)
  // UFSHC sets tx_cfgupt_*/rx_cfgupt_* for one pulse
  //
  DEBUG ((DEBUG_INFO, "Set TX_CFGUPDT_0 to 1'b1 for one TX_CFGCLK_0 cycle.\n"));
  UfsFillUicCommand (UfsUicDmeSet, VS_MPHY_CFG_UPDT, 0, 0, 0x1, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set VS_MPHY_CFG_UPDT(%x) \n", VS_MPHY_CFG_UPDT));
    return Status;
  }

  //
  // Step 3: Program the MPHY register to override lane 0 phy rx_req to 1.
  // UIC Attribute Access (B) (Attribute Name: RAWLANEN_DIG_PCS_XF_RX_OVRD_IN_1, AttrID: 0x3006 RxLaneSelect: 0x0, Value: 0x0C)
  //
  DEBUG ((DEBUG_INFO, "Ufs Program the MPHY register to override lane 0 phy rx_req to 1.\n"));
  Data16 = 0xC;
  UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN0_DIG_PCS_XF_RX_OVRD_IN_1, &Data16, UFS_CR_WR);

  //
  // Step 4: Poll the MPHY rx_ack register until the value becomes 1.
  // UIC Attribute Read Access (B) (Attribute Name: RAWLANEN_DIG_PCS_XF_RX_PCS_OUT, AttrID: 0x300F, RxLaneSelect: 0x0, Expected Data: 8h01)
  //
  DEBUG ((DEBUG_INFO, "Ufs Poll the MPHY rx_ack register until the value becomes 1.\n"));
  UfsUicTimeout = UFS_UIC_TIMEOUT;
  DigPcsXfRxPcsOut.Data16 = 0x0;
  UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN0_DIG_PCS_XF_RX_PCS_OUT, &DigPcsXfRxPcsOut.Data16, UFS_CR_RD);
  while (DigPcsXfRxPcsOut.Fields.RxAck != 0x1) {
    UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN0_DIG_PCS_XF_RX_PCS_OUT, &DigPcsXfRxPcsOut.Data16, UFS_CR_RD);
    UfsUicTimeout--;
    MicroSecondDelay (1);
    if (UfsUicTimeout == 0) {
      DEBUG ((DEBUG_ERROR, "Failed to confirm rx_ack in RAWLANEN0_DIG_PCS_XF_RX_PCS_OUT(%x) Data16: 0x%X\n", RAWLANEN0_DIG_PCS_XF_RX_PCS_OUT, DigPcsXfRxPcsOut.Data16));
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // Step 5: Program the MPHY register to override lane 1 phy rx_req to 1.
  // UIC Attribute Access (B) (Attribute Name: RAWLANEN_DIG_PCS_XF_RX_OVRD_IN_1, AttrID: 0x3106, RxLaneSelect: 0x0, Value: 0x0C)
  //
  DEBUG ((DEBUG_INFO, "Ufs Program the MPHY register to override lane 1 phy rx_req to 1.\n"));
  Data16 = 0xC;
  UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN1_DIG_PCS_XF_RX_OVRD_IN_1, &Data16, UFS_CR_WR);

  //
  // Step 6: Poll the MPHY rx_ack register until the value becomes 1.
  // UIC Attribute Read Access (B) (Attribute Name: RAWLANEN_DIG_PCS_XF_RX_PCS_OUT, AttrID: 0x310F, RxLaneSelect: 0x0, Expected Data: 8h01)
  //
  DEBUG ((DEBUG_INFO, "Ufs Poll the MPHY rx_ack register until the value becomes 1.\n"));
  UfsUicTimeout = UFS_UIC_TIMEOUT;
  DigPcsXfRxPcsOut.Data16 = 0x0;
  UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN1_DIG_PCS_XF_RX_PCS_OUT, &DigPcsXfRxPcsOut.Data16, UFS_CR_RD);
  while (DigPcsXfRxPcsOut.Fields.RxAck != 0x1) {
    UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN1_DIG_PCS_XF_RX_PCS_OUT, &DigPcsXfRxPcsOut.Data16, UFS_CR_RD);
    UfsUicTimeout--;
    MicroSecondDelay (1);
    if (UfsUicTimeout == 0) {
      DEBUG ((DEBUG_ERROR, "Failed to confirm rx_ack in RAWLANEN1_DIG_PCS_XF_RX_PCS_OUT(%x) Data16: 0x%X\n", RAWLANEN1_DIG_PCS_XF_RX_PCS_OUT, DigPcsXfRxPcsOut.Data16));
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // Step 7: Program the MPHY register to override lane 0 phy rx_req to 0.
  // UIC Attribute Access (B) (Attribute Name: RAWLANEN_DIG_PCS_XF_RX_OVRD_IN_1, AttrID: 0x3006 RxLaneSelect: 0x0, Value: 0x08)
  //
  DEBUG ((DEBUG_INFO, "Ufs Program the MPHY register to override lane 0 phy rx_req to 0.\n"));
  Data16 = 0x8;
  UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN0_DIG_PCS_XF_RX_OVRD_IN_1, &Data16, UFS_CR_WR);

  //
  // Step 8: Poll the MPHY rx_ack register until the value becomes 0..
  // UIC Attribute Read Access (B) (Attribute Name: RAWLANEN_DIG_PCS_XF_RX_PCS_OUT, AttrID: 0x300F, RxLaneSelect: 0x0, Expected Data: 8h00)
  //
  DEBUG ((DEBUG_INFO, "Ufs Poll the MPHY rx_ack register until the value becomes 0..\n"));
  UfsUicTimeout = UFS_UIC_TIMEOUT;
  DigPcsXfRxPcsOut.Data16 = 0xFFFF;
  UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN0_DIG_PCS_XF_RX_PCS_OUT, &DigPcsXfRxPcsOut.Data16, UFS_CR_RD);
  while (DigPcsXfRxPcsOut.Fields.RxAck != 0x0) {
    UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN0_DIG_PCS_XF_RX_PCS_OUT, &DigPcsXfRxPcsOut.Data16, UFS_CR_RD);
    UfsUicTimeout--;
    MicroSecondDelay (1);
    if (UfsUicTimeout == 0) {
      DEBUG ((DEBUG_ERROR, "Failed to confirm rx_ack in RAWLANEN0_DIG_PCS_XF_RX_PCS_OUT(%x) Data16: 0x%X Expected Data: 0x00\n", RAWLANEN0_DIG_PCS_XF_RX_PCS_OUT, DigPcsXfRxPcsOut.Data16));
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // Step 9: Program the MPHY register to override lane 1 phy rx_req to 0.
  // UIC Attribute Access (B) (Attribute Name: RAWLANEN_DIG_PCS_XF_RX_OVRD_IN_1, AttrID: 0x3106, RxLaneSelect: 0x0, Value: 0x08)
  //
  DEBUG ((DEBUG_INFO, "Ufs Program the MPHY register to override lane 1 phy rx_req to 0.\n"));
  Data16 = 0x8;
  UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN1_DIG_PCS_XF_RX_OVRD_IN_1, &Data16, UFS_CR_WR);

  //
  // Step 10: Poll the MPHY rx_ack register until the value becomes 0.
  // UIC Attribute Read Access (B) (Attribute Name: RAWLANEN_DIG_PCS_XF_RX_PCS_OUT, AttrID: 0x310F, RxLaneSelect: 0x0, Expected Data: 8h00)
  //
  DEBUG ((DEBUG_INFO, "Ufs Poll the MPHY rx_ack register until the value becomes 0.\n"));
  UfsUicTimeout = UFS_UIC_TIMEOUT;
  DigPcsXfRxPcsOut.Data16 = 0xFFFF;
  UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN1_DIG_PCS_XF_RX_PCS_OUT, &DigPcsXfRxPcsOut.Data16, UFS_CR_RD);
  while (DigPcsXfRxPcsOut.Fields.RxAck != 0x0) {
    UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN1_DIG_PCS_XF_RX_PCS_OUT, &DigPcsXfRxPcsOut.Data16, UFS_CR_RD);
    UfsUicTimeout--;
    MicroSecondDelay (1);
    if (UfsUicTimeout == 0) {
      DEBUG ((DEBUG_ERROR, "Failed to confirn rx_ack in RAWLANEN1_DIG_PCS_XF_RX_PCS_OUT(%x) Data16: 0x%X Expected Data: 0x00\n", RAWLANEN1_DIG_PCS_XF_RX_PCS_OUT, DigPcsXfRxPcsOut.Data16));
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // Step 11: Program the MPHY register to remove the phy rx_req override (lane 0).
  // UIC Attribute Access (B) (Attribute Name: RAWLANEN_DIG_PCS_XF_RX_OVRD_IN_1, AttrID: 0x3006 RxLaneSelect: 0x0, Value: 0x00)
  //
  DEBUG ((DEBUG_INFO, "Ufs Program the MPHY register to remove the phy rx_req override (lane 0).\n"));
  Data16 = 0x0;
  UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN0_DIG_PCS_XF_RX_OVRD_IN_1, &Data16, UFS_CR_WR);

  //
  // Step 12: Program the MPHY register to remove the phy_rx_req override (lane 1).
  // UIC Attribute Access (B) (Attribute Name: RAWLANEN_DIG_PCS_XF_RX_OVRD_IN_1, AttrID: 0x3106 RxLaneSelect: 0x0, Value: 0x00)
  //
  DEBUG ((DEBUG_INFO, "Ufs Program the MPHY register to remove the phy_rx_req override (lane 1).\n"));
  Data16 = 0x0;
  UfsUicAttributeAccessB (UfsHcDriverInterface, RAWLANEN1_DIG_PCS_XF_RX_OVRD_IN_1, &Data16, UFS_CR_WR);

  return Status;
}

/**
  Programs the recipe required for High speed link Power Mode and Gear switch

  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver

  @retval EFI_SUCCESS  Successfully programmed the recipe
  @retval others       Failed to program the recipe
**/
EFI_STATUS
UfsHsRecipieProgramming (
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  )
{
  EFI_STATUS         Status;
  EDKII_UIC_COMMAND  UicCommand;

  UfsFillUicCommand (UfsUicDmeSet, PA_HSSeries, 0, 0, UFS_PA_HS_MODE_B, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Ufs UfsUicDmeSet Status=%r PA_HSSeries(0x%X) Value: 0x%X \n", Status, PA_HSSeries, UFS_PA_HS_MODE_B));
    return Status;
  }
  UfsFillUicCommand (UfsUicDmeSet, PA_RxTermination, 0, 0, 0x1, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to enable Rx termination\n"));
    return Status;
  }
  UfsFillUicCommand (UfsUicDmeSet, PA_TxTermination, 0, 0, 0x1, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to enable Tx termination\n"));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeSet, PA_Scrambling, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set PA_Scrambling(0x%X)\n", PA_Scrambling));
    return Status;
  }

  UfsFillUicCommand (UfsUicDmeSet, DME_FC0ProtectionTimeOutVal, 0, 0, 0x13ec, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set DME_FC0ProtectionTimeOutVal(0x%X)\n", DME_FC0ProtectionTimeOutVal));
    return Status;
  }
  UfsFillUicCommand (UfsUicDmeSet, DME_TC0ReplayTimeOutVal, 0, 0, 0x13ec, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set DME_TC0ReplayTimeOutVal(0x%X)\n", DME_TC0ReplayTimeOutVal));
    return Status;
  }
  UfsFillUicCommand (UfsUicDmeSet, DME_AFC0ReqTimeOutVal, 0, 0, 0xaf0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set DME_AFC0ReqTimeOutVal(0x%X)\n", DME_AFC0ReqTimeOutVal));
    return Status;
  }
  UfsFillUicCommand (UfsUicDmeSet, PA_PWRModeUserData0, 0, 0, 0x13ec, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set PA_PWRModeUserData0(0x%X)\n", PA_PWRModeUserData0));
    return Status;
  }
  UfsFillUicCommand (UfsUicDmeSet, PA_PWRModeUserData1, 0, 0, 0x13ec, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set PA_PWRModeUserData1(0x%X)\n", PA_PWRModeUserData1));
    return Status;
  }
  UfsFillUicCommand (UfsUicDmeSet, PA_PWRModeUserData2, 0, 0, 0xaf0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set PA_PWRModeUserData2(0x%X)\n", PA_PWRModeUserData2));
    return Status;
  }
  return Status;
}

/**
  Switches the link Power Mode and Gear.

  @param[in] ControllerHandle      Handle of the UFS controller
  @param[in] PciIo                 Associated PciIo protocol
  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver

  @retval EFI_SUCCESS  Successfully switched the Power Mode and Gear
  @retval others       Failed to switch the Power Mode and Gear
**/
EFI_STATUS
UfsPowerModeAndGearSwitch (
  IN EFI_HANDLE                     ControllerHandle,
  IN EFI_PCI_IO_PROTOCOL            *PciIo,
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  )
{
  EFI_STATUS         Status;
  UINT32             PowerMode[] = {Fast_Mode, Fast_Mode};
  UINT32             MaxHsGear[] = {HS_G1, HS_G1};
  UINT32             CurrentHsGear[] = {NO_HS, NO_HS};
  UINT32             MaxPwmGear[] = {PWM_G1, PWM_G1};
  EDKII_UIC_COMMAND  UicCommand;

  DEBUG ((DEBUG_INFO, "UfsPowerModeAndGearSwitch Entry: \n" ));

  //
  //Program the Unipro C-Port connection
  //
  Status = UfsHsUniproCPortRecipieProgramming (UfsHcDriverInterface);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "UfsHsUniproCPortRecipieProgramming Fails, Status = %r\n", Status));
  }

  //
  //Program the platform specific PHY recipe
  //
  Status = UfsHsPhyRecipieProgramming (UfsHcDriverInterface);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "UfsHsPhyRecipieProgramming Fails, Status = %r\n", Status));
  }

  //
  //Program Lanes
  //
  Status = UfsActivateAllLanes (UfsHcDriverInterface);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Program UFS Lanes \n"));
    return Status;
  }

  //
  //Query Max HS Gear and Max PWM Gears and if no HS gear, set PowerMode to SlowAuto
  //
  UfsFillUicCommand (UfsUicDmeGet, PA_MaxRxPWMGear, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_MaxRxPWMGear(%x)\n", PA_MaxRxPWMGear));
    return Status;
  }
  MaxPwmGear[UfsRxLane] = UicCommand.Arg3;

  UfsFillUicCommand (UfsUicDmeGet, PA_MaxRxHSGear, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_MaxRxHSGear(%x) \n", PA_MaxRxHSGear));
    return Status;
  }
  MaxHsGear[UfsRxLane] = UicCommand.Arg3;

  UfsFillUicCommand (UfsUicDmePeerGet, PA_MaxRxPWMGear, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmePeerGet PA_MaxRxPWMGear(%x) \n",PA_MaxRxPWMGear));
    return Status;
  }
  MaxPwmGear[UfsTxLane] = UicCommand.Arg3;

  UfsFillUicCommand (UfsUicDmePeerGet, PA_MaxRxHSGear, 0, 0, 0, &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmePeerGet PA_MaxRxHSGear(%x) \n", PA_MaxRxHSGear));
    return Status;
  }
  MaxHsGear[UfsTxLane] = UicCommand.Arg3;

  //
  //Mark the Power Mode as SlowAuto_Mode
  //
  if ((NO_HS == MaxHsGear[UfsRxLane]) || (NO_HS == MaxHsGear[UfsTxLane])) {
    PowerMode[UfsRxLane] = SlowAuto_Mode;
    PowerMode[UfsTxLane] = SlowAuto_Mode;
  }

  DEBUG ((DEBUG_INFO, "PowerMode[UfsRxLane] = %x \n", PowerMode[UfsRxLane]));
  DEBUG ((DEBUG_INFO, "PowerMode[UfsTxLane] = %x \n", PowerMode[UfsTxLane]));
  DEBUG ((DEBUG_INFO, "MaxHsGear[UfsRxLane] = %x \n", MaxHsGear[UfsRxLane]));
  DEBUG ((DEBUG_INFO, "MaxHsGear[UfsTxLane] = %x \n", MaxHsGear[UfsTxLane]));
  DEBUG ((DEBUG_INFO, "MaxPwmGear[UfsRxLane] = %x \n", MaxPwmGear[UfsRxLane]));
  DEBUG ((DEBUG_INFO, "MaxPwmGear[UfsTxLane] = %x \n", MaxPwmGear[UfsTxLane]));

  //
  //Set Rx gear
  //
  if (PowerMode[UfsRxLane] == SlowAuto_Mode) {
    UfsFillUicCommand (UfsUicDmeSet, PA_RXGear, 0, 0, MaxPwmGear[UfsRxLane], &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet PA_RXGear \n"));
      return Status;
    }
  } else {
    if (MaxHsGear[UfsRxLane] > HS_G4) {
      MaxHsGear[UfsRxLane] = HS_G4;
      DEBUG ((DEBUG_INFO, "Limited MaxHsGear to %x \n", MaxHsGear[UfsRxLane]));
    }

    UfsFillUicCommand (UfsUicDmeGet, PA_RXGear, 0, 0, 0, &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet PA_RXGear \n"));
      return Status;
    }
    CurrentHsGear[UfsRxLane] = UicCommand.Arg3;

    if (CurrentHsGear[UfsRxLane] < MaxHsGear[UfsRxLane]) {
      DEBUG ((DEBUG_INFO, "CurrentHsGear[UfsRxLane] = %x switching to MaxHsGear[UfsRxLane]=%x \n", CurrentHsGear[UfsRxLane], MaxHsGear[UfsRxLane]));
      UfsFillUicCommand (UfsUicDmeSet, PA_RXGear, 0, 0, MaxHsGear[UfsRxLane], &UicCommand );
      Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet PA_RXGear \n"));
        return Status;
      }
    }
  }

  //
  //Set Tx gear
  //
  if (PowerMode[UfsTxLane] == SlowAuto_Mode) {
    UfsFillUicCommand (UfsUicDmeSet, PA_TXGear, 0, 0, MaxPwmGear[UfsTxLane], &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet PA_TXGear \n"));
      return Status;
    }
  } else {
    if (MaxHsGear[UfsTxLane] > HS_G4) {
      MaxHsGear[UfsTxLane] = HS_G4;
      DEBUG ((DEBUG_INFO, "Limited MaxHsGear to %x \n", MaxHsGear[UfsTxLane]));
    }
    UfsFillUicCommand (UfsUicDmeGet, PA_TXGear, 0, 0, 0, &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to Get UfsUicDmeGet PA_TXGear \n"));
      return Status;
    }
    CurrentHsGear[UfsTxLane] = UicCommand.Arg3;
    if (CurrentHsGear[UfsTxLane] < MaxHsGear[UfsRxLane]) {
      DEBUG ((DEBUG_INFO, "CurrentHsGear[UfsTxLane] = %x switching to MaxHsGear[UfsTxLane]=%x \n", CurrentHsGear[UfsTxLane], MaxHsGear[UfsTxLane]));
      UfsFillUicCommand (UfsUicDmeSet, PA_TXGear, 0, 0, MaxHsGear[UfsTxLane], &UicCommand );
      Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Failed to set UfsUicDmeSet PA_TXGear \n"));
        return Status;
      }
    }
  }

  //
  //Set Adapt, if the target is HS Gear 4
  //
  if ((HS_G4 == MaxHsGear[UfsRxLane]) && (HS_G4 == MaxHsGear[UfsTxLane])) {
    //Refresh=0; Initial=1; No Adapt=3 (Only applicable to G4)
    UfsFillUicCommand (UfsUicDmeSet, PA_TxHsAdaptType, 0, 0, 0x1, &UicCommand );
    Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to set PA_TxHsAdaptType(%x) \n", PA_TxHsAdaptType));
      return Status;
    }
  }

  //
  //Program the Intel platform specific recipe required for High speed link Power Mode and Gear switch
  //
  Status = UfsHsRecipieProgramming (UfsHcDriverInterface);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "UfsHsRecipieProgramming Fails, Status = %r\n", Status));
  }

  //
  //Change Power Mode of both directions
  //
  UfsFillUicCommand (UfsUicDmeSet, PA_PWRMode, 0, 0, (((PowerMode[UfsRxLane] & 0xF) << 4) | (PowerMode[UfsTxLane] & 0xF)), &UicCommand );
  Status = UfsHcDriverInterface->UfsExecUicCommand (UfsHcDriverInterface, &UicCommand);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set PA_PWRMode(%x) \n", PA_PWRMode));
    return Status;
  }

  Status = UfsWaitMemSet (UfsHcDriverInterface, UFS_HC_IS_OFFSET, UFS_HC_IS_UPMS, UFS_HC_IS_UPMS, UFS_TIMEOUT);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to switch power mode\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "Dump LinkConfig After UfsPowerModeAndGearSwitch\n"));
  Status = UfsDumpLinkConfig (UfsHcDriverInterface);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Dump LinkConfig\n"));
  }

  DEBUG ((DEBUG_INFO, "UfsPowerModeAndGearSwitch Exit\n" ));
  return EFI_SUCCESS;
}

/**
  Switches the link Power Mode and Gear.

  @param[in] ControllerHandle      Handle of the UFS controller
  @param[in] PciIo                 Associated PciIo protocol
  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver
**/
VOID
UfsHcPlatformPostHceSwitchGear (
  IN EFI_HANDLE                     ControllerHandle,
  IN EFI_PCI_IO_PROTOCOL            *PciIo,
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  )
{
  EFI_STATUS                         Status;

  DEBUG ((DEBUG_INFO, "UfsHcPlatformPostHceSwitchGear Entry\n" ));
  if (UfsHcDriverInterface == NULL) {
    DEBUG((DEBUG_ERROR, "UfsHcPlatformPostHceSwitchGear UfsHcDriverInterface is NULL\n"));
    return;
  }
  Status = UfsPowerModeAndGearSwitch(ControllerHandle, PciIo, UfsHcDriverInterface);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "UfsPowerModeAndGearSwitch Fails, Status = %r\n", Status));
  }
  DEBUG ((DEBUG_INFO, "UfsHcPlatformPostHceSwitchGear Exit\n" ));
}

/**
  Applies platform specific programming before the driver
  has enabled the host controller.

  @param[in] ControllerHandle      Handle of the UFS controller
  @param[in] PciIo                 Associated PciIo protocol
  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver
**/
VOID
UfsHcPlatformPreHce (
  IN EFI_HANDLE                     ControllerHandle,
  IN EFI_PCI_IO_PROTOCOL            *PciIo,
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  );

/**
  Callback function for platform driver.

  @param[in]      ControllerHandle  Handle of the UFS controller.
  @param[in]      CallbackPhase     Specifies when the platform protocol is called
  @param[in, out] CallbackData      Data specific to the callback phase.
                                    For PreHce and PostHce - EDKII_UFS_HC_DRIVER_INTERFACE.
                                    For PreLinkStartup and PostLinkStartup - EDKII_UFS_HC_DRIVER_INTERFACE.

  @retval EFI_SUCCESS            Override function completed successfully.
  @retval EFI_INVALID_PARAMETER  CallbackPhase is invalid or CallbackData is NULL when phase expects valid data.
  @retval Others                 Function failed to complete.
**/
EFI_STATUS
EFIAPI
UfsHcPlatformCallback (
  IN     EFI_HANDLE                            ControllerHandle,
  IN     EDKII_UFS_HC_PLATFORM_CALLBACK_PHASE  CallbackPhase,
  IN OUT VOID                                  *CallbackData
  )
{
  EFI_STATUS           Status;
  EFI_PCI_IO_PROTOCOL  *PciIo;

  Status = gBS->HandleProtocol (
                  ControllerHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID *) &PciIo
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UfsHcPlatformCallback PciIoProtocol is not available, Status %r\n", Status));
    return Status;
  }

  if (!IsIntelUfsDevice (PciIo)) {
    return EFI_SUCCESS;
  }

  if (CallbackPhase == EdkiiUfsHcPreHce) {
    UfsHcPlatformPreHce (
      ControllerHandle,
      PciIo,
      (EDKII_UFS_HC_DRIVER_INTERFACE*) CallbackData
      );
  }

  if (CallbackPhase == EdkiiUfsHcPostHce) {
    UfsHcPlatformPostHce (
      ControllerHandle,
      PciIo,
      (EDKII_UFS_HC_DRIVER_INTERFACE*) CallbackData
      );
  }

  if (CallbackPhase == EdkiiUfsHcPostLinkStartup) {
    UfsHcPlatformPostHceSwitchGear (
      ControllerHandle,
      PciIo,
      (EDKII_UFS_HC_DRIVER_INTERFACE*) CallbackData
      );
  }

  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED EDKII_UFS_HC_PLATFORM_PROTOCOL  gEdkiiUfsHcPlatformProtocol = {
  EDKII_UFS_HC_PLATFORM_PROTOCOL_VERSION,
  UfsHcPlatformOverrideHcInfo,
  UfsHcPlatformCallback,
  EdkiiUfsCardRefClkFreq19p2Mhz
};

/**
  Applies platform specific programming before the driver
  has enabled the host controller.

  @param[in] ControllerHandle      Handle of the UFS controller
  @param[in] PciIo                 Associated PciIo protocol
  @param[in] UfsHcDriverInterface  Interface to the functionalities provided by core driver
**/
VOID
UfsHcPlatformPreHce (
  IN EFI_HANDLE                     ControllerHandle,
  IN EFI_PCI_IO_PROTOCOL            *PciIo,
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  )
{
  EFI_STATUS           Status;
  PCI_TYPE00           PciData;

  if (UfsHcDriverInterface == NULL) {
    return;
  }
  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, 0, sizeof (PciData), &PciData);
  //
  // Examine UFS Host Controller PCI Configuration table fields
  //
  if ((!EFI_ERROR (Status)) && (PciData.Hdr.ClassCode[2] == PCI_CLASS_MASS_STORAGE) && (PciData.Hdr.ClassCode[1] == 0x09)) {
    DEBUG ((DEBUG_INFO, "UFS controller found, DeviceId=%x\n", PciData.Hdr.DeviceId));
  } else {
    DEBUG ((DEBUG_ERROR, "PciData Read, Status=%r VendorId=0x%x DeviceId=%x\n", Status, PciData.Hdr.VendorId, PciData.Hdr.DeviceId));
    DEBUG ((DEBUG_ERROR, "PciData Read, BaseClass=0x%x SubClass=%x\n", PciData.Hdr.ClassCode[2], PciData.Hdr.ClassCode[1]));
  }

  if (((PciData.Hdr.DeviceId == UFS_PLATFORM_INTEL_MTLM_DEV_ID) ||
       (PciData.Hdr.DeviceId == UFS_PLATFORM_INTEL_ARLM_DEV_ID) ||
       (PciData.Hdr.DeviceId == UFS_PLATFORM_INTEL_LNLM_DEV_ID) ||
       (PciData.Hdr.DeviceId == UFS_PLATFORM_INTEL_PTLPCDP_DEV_ID)))
  {
    gEdkiiUfsHcPlatformProtocol.RefClkFreq = EdkiiUfsCardRefClkFreq38p4Mhz;
  }
  DEBUG ((DEBUG_INFO, "RefClkFreq=%x\n  0 -> 19.2 Mhz\n  1 -> 26 Mhz\n  2 -> 38.4 Mhz\n  3 -> Obsolete \n", gEdkiiUfsHcPlatformProtocol.RefClkFreq));
}

/**
  Entry point for UFS platform driver for Intel platforms

  @param[in] ImageHandle  This image handle
  @param[in] SystemTable  System table pointer
**/
EFI_STATUS
EFIAPI
UfsPlatformDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE           ProtocolHandle;
  EFI_STATUS           Status;

  ProtocolHandle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &ProtocolHandle,
                  &gEdkiiUfsHcPlatformProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &gEdkiiUfsHcPlatformProtocol
                  );
  DEBUG ((DEBUG_INFO, "UfsPlatformDriverEntryPoint, Status=%r\n", Status));
  return Status;
}

