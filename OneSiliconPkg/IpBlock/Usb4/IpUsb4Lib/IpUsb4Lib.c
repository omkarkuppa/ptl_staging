/** @file
  The TCSS USB4 Init Library

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
#include <IpCsi.h>
#include <IpUsb4.h>
#include <IpWrapper.h>
#include <IpWrapper64Bit.h>

#include <Register/Usb4Regs.h>
#include <Usb4MailBoxCmdDefinition.h>
#include "IpUsb4Private.h"

/**
  Check if USB4 Host Router controller is enabled

  @param[in]  pInst             The instance structure
  @param[in]  ControllerIndex   The index of the DMA controller

  @retval  TRUE            The specific USB4 controller is enabled
  @retval  FALSE           The specific USB4 controller is disabled
**/
BOOLEAN
IpUsb4IsControllerEnabled (
  IP_USB4_INST  *pInst,
  UINT32        ControllerIndex
  )
{

  if (ControllerIndex >= MAX_HOST_ITBT_DMA_NUMBER) {
    return FALSE;
  }

  return !!(pInst->Usb4Config.DevEn.DevEnBits.Usb4HrEn & (1 << ControllerIndex));
}

/**
  Internal function to Wait for Tbt2PcieDone Bit to set or clear

  @param[in]  pInst             The instance structure
  @param[in]  Usb4AccessCntxt   DMA register access context
  @param[in]  Tbt2PcieRegOffset Tbt2Pcie Register Offset
  @param[in]  TimeOut           The number of 1 ms units to delay
  @param[in]  Tbt2PcieDone      Wait condition (wait for Bit to Clear/Set)
  @param[out] Tbt2PcieValue     Function register value

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorTimeout   ITBT FW communication timeout
  @retval  IpCsiStsErrorNullPtr   NULL pointer passed as param
  @retval  IpCsiStsError          Device not present
**/
IP_CSI_STATUS
IpUsb4WaitforCommandCompletion (
  IP_USB4_INST      *pInst,
  IP_WR_REG_CNTXT   Usb4AccessCntxt,
  UINT32            Tbt2PcieRegOffset,
  UINT32            TimeOut,
  BOOLEAN           Tbt2PcieDone,
  UINT32            *Tbt2PcieValue
  )
{
  IP_CSI_STATUS     Status;
  UINT32            MaskToCheck;
  UINT32            ValueToCheck;
  UINT32            RegValue;

  USB4_PRINT_INFO ("[TBT] Waiting for TBT controller to finish command\n");

  USB4_API_ENTRY ();

  if (Tbt2PcieValue == NULL) {
    USB4_PRINT_ERROR ("%a: ERROR: Tbt2PcieValue is NULL!\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  RegValue = (UINT32)IpWrRegRead (Usb4AccessCntxt, Tbt2PcieRegOffset, IpWrRegFlagSize32Bits);
  if (RegValue == 0xFFFFFFFF) {
    USB4_PRINT_ERROR ("%a: ERROR: Device not present!", __FUNCTION__);
    return IpCsiStsError;
  }

  MaskToCheck  =  TBT2PCIE_DON_R;
  if (Tbt2PcieDone) {
    ValueToCheck =  TBT2PCIE_DON_R;
  } else {
    ValueToCheck = 0;
  }

  IpWrRegPollUs (Usb4AccessCntxt,
                  Tbt2PcieRegOffset,
                  IpWrRegFlagSize32Bits,
                  MaskToCheck,
                  ValueToCheck,
                  TimeOut,
                  pInst->TimeCntxt,
                  TBT_MAILBOX_DELAY,
                  &Status
                );

  *Tbt2PcieValue = (UINT32)IpWrRegRead (Usb4AccessCntxt, Tbt2PcieRegOffset, IpWrRegFlagSize32Bits);

  return Status;
}

/**
  Execute TBT Mailbox Command without data

  1.Load PCIE2TBT register with command and set VALID bit of PCIE2TBT register to indicate a new command sent
  2.Wait for DONE bit set by LC acknowledging the command sent from the host before timeout
  3.Clear PCIE2TBT register valid bit which should be cleared by host SW after command done
  4.Wait for DONE bit cleared by LC if it is iTBT controller

  @param[in]  pInst           The instance structure
  @param[in]  Usb4AccessCntxt DMA register access context
  @param[in]  Command         TBT Command
  @param[in]  Tbt2PcieOffset  TBT2PCIE Register Offset
  @param[in]  Pcie2TbtOffset  PCIE2TBT Register Offset
  @param[in]  Timeout         Time out with 1 ms granularity

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorTimeout   ITBT FW communication timeout
  @retval  IpCsiStsErrorNullPtr   NULL pointer passed as param
**/
IP_CSI_STATUS
IpUsb4SetPcie2TbtCommand (
  IP_USB4_INST      *pInst,
  IP_WR_REG_CNTXT   Usb4AccessCntxt,
  UINT8             Command,
  UINT32            Tbt2PcieOffset,
  UINT32            Pcie2TbtOffset,
  UINT32            Timeout
  )
{
  IP_CSI_STATUS Status;
  UINT32        RegisterValue;

  USB4_API_ENTRY ();

  USB4_PRINT_INFO ("[TBT] Send command %x to controller \n", Command);

  ///
  /// Load PCIE2TBT register with command and set VALID bit of PCIE2TBT register to indicate a new command sent, generates
  /// interrupt to LC and CM.
  ///
  IpWrRegWrite (Usb4AccessCntxt, Pcie2TbtOffset, Command | PCIE2TBT_VLD_B, IpWrRegFlagSize32Bits);

  ///
  /// Wait for DONE bit set by LC acknowledging the command sent from the host before timeout
  ///
  Status = IpUsb4WaitforCommandCompletion (pInst, Usb4AccessCntxt, Tbt2PcieOffset, Timeout, TRUE, &RegisterValue);

  ///
  /// Clear PCIE2TBT register valid bit which should be cleared by host SW after command done
  ///
  IpWrRegWrite (Usb4AccessCntxt, Pcie2TbtOffset, 0, IpWrRegFlagSize32Bits);

  //
  /// Wait for DONE bit cleared by LC if it is iTBT controller
  ///
  if (Tbt2PcieOffset == pInst->Usb4Config.ITbtToPcieReg) {
    Status = IpUsb4WaitforCommandCompletion (pInst, Usb4AccessCntxt, Tbt2PcieOffset, Timeout, FALSE, &RegisterValue);
  }

  return Status;
}

/**
  Program iTBT FP Register in order to bring iTBT micro controller up by applying Force Power.

  @param[in] pInst  The instance structure

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorTimeout   ITBT FW communication timeout
  @retval  IpCsiStsErrorNullPtr   NULL pointer passed as param
**/
IP_CSI_STATUS
IpUsb4SetForcePower (
  IP_USB4_INST     *pInst
  )
{
  IP_CSI_STATUS   Status;
  IP_USB4_DMA     *DmaDevice;
  UINT32          RegValue;
  UINT8           DmaIndex;

  USB4_API_ENTRY ();

  if (pInst->DmaList == NULL) {
    USB4_PRINT_WARNING ("WARNING: NO DMA controller passed to %a!\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  Status = IpCsiStsSuccess;
  DmaDevice = pInst->DmaList;
  for (DmaIndex = 0; DmaDevice != NULL; DmaIndex++) {
    if (IpUsb4IsControllerEnabled (pInst, DmaIndex) == FALSE) {
      USB4_PRINT_INFO ("DMA%d is disabled. Skip..\n", DmaIndex);
      DmaDevice = DmaDevice->Next;
      continue;
    }

    RegValue = (UINT32)IpWrRegRead (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_22, IpWrRegFlagSize32Bits);
    RegValue &= (UINT32) ~ (B_USB4_CFG_TBT_DMA_CFG_VS_CAP_22_DMA_ACTIVE_DELAY);
    RegValue |= B_USB4_CFG_TBT_DMA_CFG_VS_CAP_22_FORCE_POWER | (V_HOST_DMA_DMA_ACTIVE_DELAY_VALUE << N_USB4_CFG_TBT_DMA_CFG_VS_CAP_22_DMA_ACTIVE_DELAY);
    IpWrRegWrite (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_22, RegValue, IpWrRegFlagSize32Bits);

    //
    //  Polling for NVM FW readiness
    //
    IpWrRegPollUs (DmaDevice->PrimCfgAccess,
                    R_USB4_CFG_TBT_DMA_CFG_VS_CAP_9,
                    IpWrRegFlagSize32Bits,
                    B_USB4_CFG_TBT_DMA_CFG_VS_CAP_9_FW_READY,
                    B_USB4_CFG_TBT_DMA_CFG_VS_CAP_9_FW_READY,
                    pInst->Usb4Config.ITbtForcePowerOnTimeoutInMs,
                    pInst->TimeCntxt,
                    1000,
                    &Status
                  );
    if (Status != IpCsiStsSuccess) {
      if (Status == IpCsiStsErrorTimeout) {
        USB4_PRINT_ERROR ("ERROR: DMA Controller %d is NOT ready! Timeout!\n", DmaIndex);
      } else {
        USB4_PRINT_ERROR ("ERROR: DMA Controller %d NOT powered!\n", DmaIndex);
      }
      return Status;
    } else {
      USB4_PRINT_INFO ("DMA Controller %d is powered\n", DmaIndex);
    }

    DmaDevice = DmaDevice->Next;
  }

  return Status;
}

/**
  Send Usb4 Connect Topology Command and CM mode command

  @param[in] pInst  The instance structure

  @retval  IpCsiStsSuccess     The function completes successfully
  @retval  IpCsiStsError       Connect Topology / CM mode command NOT executed successfully.
**/
IP_CSI_STATUS
IpUsb4SetCmMode (
  IP_USB4_INST     *pInst
  )
{
  IP_CSI_STATUS   Status;
  IP_USB4_DMA     *DmaDevice;
  UINT8           DmaIndex;
  UINT8           Usb4CmMode;
  UINT8           CommandValue;

  USB4_API_ENTRY ();

  Usb4CmMode = pInst->Usb4Config.Usb4CmMode;
  if (Usb4CmMode == USB4_CM_MODE_FW_CM) {
    CommandValue = PCIE2TBT_FIRMWARE_CM_MODE;
  } else {
    CommandValue = PCIE2TBT_PASS_THROUGH_MODE;
  }
  USB4_PRINT_INFO ("[TBT] IpUsb4SetCmMode CommandValue = 0x%X\n", CommandValue);

  //
  // Each Controller will have 2 Root Ports + HIA
  // Based on the root ports enabled, execute Mail Box Communications on corresponding HIA
  //
  Status = IpCsiStsSuccess;
  DmaDevice = pInst->DmaList;
  for (DmaIndex = 0; DmaDevice != NULL; DmaIndex++) {
    if (IpUsb4IsControllerEnabled (pInst, DmaIndex) == FALSE) {
      USB4_PRINT_INFO ("DMA%d is disabled. Skip..\n", DmaIndex);
      DmaDevice = DmaDevice->Next;
      continue;
    }

    //
    // Send FW / SW CM mode command
    //
    Status = IpUsb4SetPcie2TbtCommand (pInst,
                                        DmaDevice->PrimCfgAccess,
                                        CommandValue,
                                        pInst->Usb4Config.ITbtToPcieReg,
                                        pInst->Usb4Config.PcieToITbtReg,
                                        3
                                      );
    if (Status != IpCsiStsSuccess) {
      USB4_PRINT_ERROR ("ERROR: Send FW / SW CM mode command return Error Status: %d (DMA Index = %d)!\n", Status, DmaIndex);
      return Status;
    }

    if (Usb4CmMode != USB4_CM_MODE_FW_CM) {
      USB4_PRINT_INFO ("SW CM mode, skip sending CONNECT_TOPOLOGY command. DMA Index = %d\n", DmaIndex);
      DmaDevice = DmaDevice->Next;
      continue;
    }

    //
    // 1. Send "CONNECT_TOPOLOGY" command to CM with 5 second timeout in Post Mem. CONNECT_TOPOLOGY command is targeted to 0x1F.
    // 2. Dessert iTBT FP bit (0xFC) on each enabled iTBT instance (controller)
    //
    Status = IpUsb4SetPcie2TbtCommand (pInst,
                                        DmaDevice->PrimCfgAccess,
                                        PCIE2TBT_CONNECT_TOPOLOGY_COMMAND,
                                        pInst->Usb4Config.ITbtToPcieReg,
                                        pInst->Usb4Config.PcieToITbtReg,
                                        pInst->Usb4Config.ITbtConnectTopologyTimeoutInMs);
    if (Status != IpCsiStsSuccess) {
      USB4_PRINT_ERROR ("ERROR: Send CONNECT_TOPOLOGY command return Error Status: %d (DMA Index = %d)!\n", Status, DmaIndex);
      return Status;
    }

    DmaDevice = DmaDevice->Next;
  }

  return Status;
}

/**
  Program LTR Registers for Integrated Thunderbolt.

  @param[in] pInst  The instance structure

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  EFI_UNSUPPORTED        ITBT is not supported.
**/
IP_CSI_STATUS
IpUsb4SetDmaLtr (
  IP_USB4_INST     *pInst
  )
{
  UINT16       RegData;
  UINT8        DmaIndex;
  IP_USB4_DMA  *DmaDevice;

  USB4_API_ENTRY ();

  DmaDevice = pInst->DmaList;
  for (DmaIndex = 0; DmaDevice != NULL; DmaIndex++) {
    if (IpUsb4IsControllerEnabled (pInst, DmaIndex) == FALSE) {
      USB4_PRINT_INFO ("DMA%d is disabled. Skip..\n", DmaIndex);
      DmaDevice = DmaDevice->Next;
      continue;
    }
    //
    // Program Usb4 LTR to maximum snoop/non-snoop latency values
    //
    RegData = pInst->Usb4Config.ITbtDmaLtr[DmaIndex] & ~BIT15;
    if (RegData != 0) {
      RegData |= BIT15;
    }

    IpWrRegWrite (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_16, RegData, IpWrRegFlagSize16Bits);
    USB4_PRINT_INFO ("[USB4] Write 0x%08X to Offset 0x%08X\n", RegData, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_16);

    USB4_PRINT_INFO ("[USB4] Usb4SetDmaLtr Read Offset 0x%08X = 0x%08X\n", R_USB4_CFG_TBT_DMA_CFG_VS_CAP_16, ((UINT16) IpWrRegRead (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_16, IpWrRegFlagSize16Bits)));

    DmaDevice = DmaDevice->Next;
  }

  return IpCsiStsSuccess;
}

/**
  This function Update UUID number to USB4 Host controller.

  @param[in] pInst  The instance structure

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  EFI_INVALID_PARAMETER  PeiITbtConfig is invalid
**/
IP_CSI_STATUS
IpUsb4SetDmaUuid (
  IP_USB4_INST     *pInst
  )
{
  UINT64       RegData;
  UINT16       VendorId;
  UINT8        DmaIndex;
  IP_USB4_DMA  *DmaDevice;

  USB4_API_ENTRY ();

  // Top 48 bits of the random number are the bottom 48 bits of the value
  // It's ORed with the VendorId for the top 16
  // Then ANDed so that the bottom 4 bits = 0
  // And the Index is ORed in at the bottom
  VendorId = 0x8087;

  DmaDevice = pInst->DmaList;
  for (DmaIndex = 0; DmaDevice != NULL; DmaIndex++) {
    if (IpUsb4IsControllerEnabled (pInst, DmaIndex) == FALSE) {
      USB4_PRINT_INFO ("DMA%d is disabled. Skip..\n", DmaIndex);
      DmaDevice = DmaDevice->Next;
      continue;
    }
    RegData = (UINT64) (((UINT64) IpWrRShiftU64 (pInst->RandomNum, 16)) | (UINT64) IpWrLShiftU64 (VendorId, 48)); // Program VendorID
    RegData = (UINT64) (RegData & 0xFFFFFFFFFFFFFFF0);
    RegData = (UINT64) (RegData | (UINT64) DmaIndex); // set instance

    // Write the UUID
    // UUID[63:32]
    IpWrRegWrite (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_11, (UINT32) IpWrRShiftU64 (RegData, 32), IpWrRegFlagSize32Bits);
    // UUID[31:0]
    IpWrRegWrite (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_10, (UINT32)RegData, IpWrRegFlagSize32Bits);

    USB4_PRINT_INFO ("[USB4] Write 0x%08X%08X to Offset 0x%08X\n", (UINT32) IpWrRShiftU64 (RegData, 32), (UINT32)RegData, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_10);

    // Read the UUID back
    // UUID[63:32]
    RegData = IpWrLShiftU64 ((UINT64) IpWrRegRead (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_11, IpWrRegFlagSize32Bits), 32);
    // UUID[31:0]
    RegData += IpWrRegRead (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_10, IpWrRegFlagSize32Bits);

    USB4_PRINT_INFO ("[USB4] Usb4SetDmaUUID Read Offset 0x%08X = 0x%08X%08X\n", R_USB4_CFG_TBT_DMA_CFG_VS_CAP_10, (UINT32) IpWrRShiftU64 (RegData, 32), (UINT32)RegData);

    DmaDevice = DmaDevice->Next;
  }

  return IpCsiStsSuccess;
}

/**
  Send USB4 command to ask USB4 controller to enable or disable PCIe tunneling for USB4

  @param[in] pInst  The instance structure

  @retval  IpCsiStsSuccess     The function completes successfully
  @retval  IpCsiStsError       USB4 Command is not executed successfully
**/
IP_CSI_STATUS
IpUsb4PcieTunneling (
  IP_USB4_INST     *pInst
  )
{
  UINT32       RegData;
  UINT8        DmaIndex;
  IP_USB4_DMA  *DmaDevice;

  USB4_API_ENTRY ();

  DmaDevice = pInst->DmaList;
  for (DmaIndex = 0; DmaDevice != NULL; DmaIndex++) {
    if (IpUsb4IsControllerEnabled (pInst, DmaIndex) == FALSE) {
      USB4_PRINT_INFO ("DMA%d is disabled. Skip..\n", DmaIndex);
      DmaDevice = DmaDevice->Next;
      continue;
    }
    //
    // Execute Mail Box Communications on corresponding HIA
    //
    // 1. BIOS send command to FW CM with 5 second timeout in Post Mem.
    // 2. BIOS dessert Usb4 FP bit (0xFC) on each enabled Usb4 instance (controller)
    //
    //
    // Enable PCIe Tunnel over USB4 -> set security level to 0
    // It means no security management via TBT3/USB4 Controller
    //
    if (pInst->Usb4Config.ITbtPcieTunnelingForUsb4 == 1) {
      RegData = V_HOST_DMA_ITBT_SECURITY_LEVEL0;
    } else {
      RegData = V_HOST_DMA_ITBT_SECURITY_LEVEL5;
    }

    IpWrRegWrite (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_17, RegData, IpWrRegFlagSize32Bits);

    USB4_PRINT_INFO ("[USB4] Write 0x%08X to Offset 0x%08X\n", RegData, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_17);

    USB4_PRINT_INFO ("[USB4] %a Read Offset 0x%08X = 0x%08X\n",  __FUNCTION__, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_17, ((UINT32) IpWrRegRead (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_17, IpWrRegFlagSize32Bits)));

    DmaDevice = DmaDevice->Next;
  }

  return IpCsiStsSuccess;
}

/**
  Remove Force Power for USB4 controllers

  @param[in] pInst  The instance structure

  @retval  IpCsiStsSuccess     The function completes successfully
  @retval  IpCsiStsError       USB4 Command is not executed successfully.
**/
IP_CSI_STATUS
IpUsb4UnsetForcePower (
  IP_USB4_INST     *pInst
  )
{
  UINT32       RegData;
  UINT8        DmaIndex;
  IP_USB4_DMA  *DmaDevice;

  USB4_API_ENTRY ();

  DmaDevice = pInst->DmaList;
  for (DmaIndex = 0; DmaDevice != NULL; DmaIndex++) {
    if (IpUsb4IsControllerEnabled (pInst, DmaIndex) == FALSE) {
      USB4_PRINT_INFO ("DMA%d is disabled. Skip..\n", DmaIndex);
      DmaDevice = DmaDevice->Next;
      continue;
    }
    RegData = (UINT8) IpWrRegRead (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_22, IpWrRegFlagSize8Bits);
    RegData &= ~B_USB4_CFG_TBT_DMA_CFG_VS_CAP_22_FORCE_POWER;
    IpWrRegWrite (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_22, RegData, IpWrRegFlagSize8Bits);

    USB4_PRINT_INFO ("[USB4] Write 0x%08X to Offset 0x%08X\n", RegData, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_22);

    RegData = (UINT8) IpWrRegRead (DmaDevice->PrimCfgAccess, R_USB4_CFG_TBT_DMA_CFG_VS_CAP_22, IpWrRegFlagSize8Bits);

    USB4_PRINT_INFO ("[USB4] Usb4UnsetForcePower Read Offset 0x%08X = 0x%08X\n", R_USB4_CFG_TBT_DMA_CFG_VS_CAP_22, RegData);

    DmaDevice = DmaDevice->Next;
  }

  return IpCsiStsSuccess;
}

/**
  This interface will return a version value that corresponds to the VerId provided and
  is formatted per the CSI specification.
  - CSI Defined Interface

  @param[in]      pInst      A pointer to the IP instance to be used.
  @param[in]      VerId      An enum value defined in the CSI specification of type IP_CSI_VER_ID.

  @retval         0                       The VerId value is not recognized and a version could not be determined.
  @retval         IpCsiStsErrorNullPtr    pInst == NULL
  @retval         Non-Zero                A version value corresponding to the VerId provided, and formatted per the CSI.
**/
UINT32
IpUsb4GetVersion (
  IP_USB4_INST      *pInst,
  IP_CSI_VER_ID     VerId
  )
{
  IP_CSI_VER_ID RetValue;

  USB4_API_ENTRY ();

  switch (VerId) {
    case (IpCsiVerIdHwIp):
      RetValue = VERSION_HW_USB4;
      break;
    case (IpCsiVerIdHwIpCustom):
      RetValue = VERSION_HW_CUSTOM_USB4;
      break;
    case (IpCsiVerIdIpFwIntf):
      RetValue = VERSION_IPFWINTF_USB4;
      break;
    case (IpCsiVerIdIpFwIntfExt):
      RetValue = VERSION_IPFWINTFEXT_USB4;
      break;
    case (IpCsiVerIdIpFw):
      RetValue = VERSION_FW_USB4;
      break;
    case (IpCsiVerIdCsi):
      RetValue = VERSION_CSI;
      break;
    default:
      RetValue = VERSION_UNKNOWN_USB4;
      break;
  }

  return RetValue;
}

/**
  Instance structure initialization for this version of IP
  For current version of USB4 IP there is no requirements for this function.
  All data in Instance Structure is initialized by the caller.
  The function just returns success status.

  @param[in] pInst  The instance structure pointer

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpUsb4InstInit (
  IP_USB4_INST      *pInst
  )
{
  USB4_API_ENTRY ();

  return IpCsiStsSuccess;
}

/**
  USB4 IP Initialization procedure.
  The function shall be the first API function called after IpIomInstInit.
  This is a good place to fix bad register defaults or work around bugs.

  @param[in]  pInst The instance structure pointer

  @retval         IpCsiStsSuccess on success
  @retval         !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpUsb4IpInit (
  IP_USB4_INST     *pInst
  )
{
  IP_CSI_STATUS Status;

  USB4_API_ENTRY ();

  Status = IpUsb4SetForcePower (pInst);
  USB4_RETURN_IF_ERROR (Status)

  Status = IpUsb4PcieTunneling (pInst);
  USB4_RETURN_IF_ERROR (Status)

  if (pInst->TcssVersion == TcssOld) {
    Status = IpUsb4SetDmaUuid (pInst);
    USB4_RETURN_IF_ERROR (Status)
  }

  Status = IpUsb4SetDmaLtr (pInst);
  USB4_RETURN_IF_ERROR (Status)

  Status = IpUsb4SetCmMode (pInst);
  USB4_RETURN_IF_ERROR (Status)

  Status = IpUsb4UnsetForcePower (pInst);
  USB4_RETURN_IF_ERROR (Status)

  return IpCsiStsSuccess;
}
