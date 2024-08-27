/** @file
  PeiTbtInit library implementation with empty functions.

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

@par Specification
**/

#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PchPcieRpLib.h>
#include <Library/DTbtCommonLib.h>
#include <Library/TbtCommonLib.h>
#include <Library/PciSegmentLibSegmentInfo/PciSegmentLibCommon.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <DTbtDefinition.h>

/**
  BIOS initiates the "SetMaxPayloadSize" mailbox command with the required Meta Data

  @param[in]  Data                      Mps Data. 0 map to 128b and 1 map to 256b
  @param[in]  Bus                       Bus number for Host Router (DTBT)
  @param[in]  Device                    Device number for Host Router (DTBT)
  @param[in]  Function                  Function number for Host Router (DTBT)
  @param[in]  Timeout                   Time out with 100 ms granularity
**/
BOOLEAN
SetMaxPayloadSize (
  IN    UINT8                           Data,
  IN    UINT8                           Bus,
  IN    UINT8                           Device,
  IN    UINT8                           Function,
  IN    UINT32                          Timeout
  )
{
  UINT64                                Pcie2Tbt;
  UINT64                                Tbt2Pcie;
  UINT32                                RegisterValue;
  BOOLEAN                               ReturnFlagSet;
  BOOLEAN                               ReturnFlagClear;

  ReturnFlagSet   = FALSE;
  ReturnFlagClear = FALSE;

  DEBUG ((DEBUG_INFO, "[TBT] SetMaxPayloadSize to %d\n", Data));

  Tbt2Pcie = PCI_SEGMENT_LIB_ADDRESS(0, Bus, Device, Function, FixedPcdGet16 (PcdDTbtToPcieRegister));
  Pcie2Tbt = PCI_SEGMENT_LIB_ADDRESS(0, Bus, Device, Function, FixedPcdGet16 (PcdPcieToDTbtRegister));

  //
  // Write register offset with command
  //
  PciSegmentWrite32 (Pcie2Tbt, ((UINT32) Data << 8) | PCIE2TBT_MAX_PAYLOAD_SIZE_CMD | PCIE2TBT_VLD_B);

  //
  // Check if Tbt2PcieDone bit is been set
  //
  ReturnFlagSet = InternalWaitforCommandCompletion (Tbt2Pcie, Timeout, TRUE, &RegisterValue);
  if (!ReturnFlagSet) {
    DEBUG ((DEBUG_ERROR, "[TBT] SetMaxPayloadSize fail done bit is not set, RegisterValue = %x\n", RegisterValue));
  }

  //
  // Clean register offset value
  //
  PciSegmentWrite32 (Pcie2Tbt, 0);

  //
  // Check if Tbt2PcieDone bit is been clear
  //
  ReturnFlagClear = InternalWaitforCommandCompletion (Tbt2Pcie, Timeout, FALSE, &RegisterValue);
  if (!ReturnFlagClear) {
    DEBUG ((DEBUG_ERROR, "[TBT] SetMaxPayloadSize fail done bit is not clear, RegisterValue = %x\n", RegisterValue));
  }

  return ReturnFlagSet & ReturnFlagClear;
}

/**
  Check to skip unexpected device.
  @param[in]  Hdr                 Pointer to PCI config space.
  @param[out] IsPresence          Pointer to Boolean indicating PCI device presence

  @retval     TRUE                Skip this device because of unexpected device
  @retval     FALSE               Do not skip this device
**/
BOOLEAN
EFIAPI
IsSkipCheckDevice (
  IN  PCI_DEVICE_INDEPENDENT_REGION  *Hdr,
  OUT BOOLEAN                        *IsPresence
  )
{
  if ((Hdr == NULL) || (IsPresence == NULL)) {
    DEBUG ((DEBUG_ERROR, "IsSkipCheckDevice: Invalid parameter (%p, %p)\n", Hdr, IsPresence));
    return TRUE;
  }

  *IsPresence = TRUE;

  if (Hdr->VendorId == MAX_UINT16) {
    //
    // Device not exists
    //
    *IsPresence = FALSE;
    return TRUE;
  }

  if (!IS_PCI_A_BRIDGE (Hdr) || !IS_PCI_SUPPORT_CAPABILITY (Hdr)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Find PCI-E Capability with PCI bus/dev/function number.
  @param[in]      Bus    PCI bus number.
  @param[in]      Dev    PCI device number.
  @param[in]      Func   PCI function number.
  @param[in, out] Reg    [in]  Pointer to Offset of first PCIE capability to look up
                         [out] Pointer to Offset of PCI-E Capability with PCIEXP Capability ID.

  @retval EFI_SUCCESS           Offset of PCI-E Capability is retrieved
  @retval EFI_INVALID_PARAMETER NULL Reg pointer
  @retval EFI_NOT_FOUND         Not found PCI-E Capability in current PCI device
**/
EFI_STATUS
EFIAPI
FindPcieCap (
  IN     UINT8   Bus,
  IN     UINT8   Dev,
  IN     UINT8   Func,
  IN OUT UINT16  *Reg
  )
{
  UINT64                PcieAddress;
  UINT16                RegVal16;

  if (Reg == NULL) {
    DEBUG ((DEBUG_ERROR, "FindPcieCap: NULL Reg pointer\n"));
    return EFI_INVALID_PARAMETER;
  }

  do {
    PcieAddress = PCI_SEGMENT_LIB_ADDRESS (0, Bus, Dev, Func, *Reg);
    RegVal16 = PciSegmentRead16 (PcieAddress);
    // DEBUG ((DEBUG_INFO, "[GetDTbtRpDevFun] B[%x]D[%x]F[%x]  cap id %x offset %x\n", Bus, Dev, Func, RegVal16 & 0xff, *Reg));
    if (((EFI_PCI_CAPABILITY_HDR *) &RegVal16)->CapabilityID == EFI_PCI_CAPABILITY_ID_PCIEXP) {
      return EFI_SUCCESS;
    }
    else {
      *Reg = ((EFI_PCI_CAPABILITY_HDR *) &RegVal16)->NextItemPtr;
    }
  } while ((*Reg != 0) || (*Reg < 0xff));

  return EFI_NOT_FOUND;
}

/**
  Get Pch/Peg Pcie Root Port Device and Function Number for TBT by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[in]  RpBus                 Root port bus number.
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER If Invalid Root Port Number or TYPE is Passed
**/
EFI_STATUS
EFIAPI
GetDTbtRpDevFun (
  IN  UINT8   RpNumber,
  IN  UINT8   RpBus,
  OUT UINT8   *RpDev,
  OUT UINT8   *RpFunc
  )
{
  EFI_STATUS                     Status;
  PCI_DEVICE_INDEPENDENT_REGION  Hdr;
  UINT64                         PcieAddress;
  UINT32                         RegVal32;
  UINT16                         RegVal16;
  UINT8                          RegVal8;
  UINT8                          Dev;
  UINT8                          Func;
  UINT32                         OrgBus;
  BOOLEAN                        IsPresence;
  BOOLEAN                        Found;
  UINT8                          TempBusNumber;

  if ((RpDev == NULL) || (RpFunc == NULL)) {
    DEBUG ((DEBUG_ERROR, "GetDTbtRpDevFunc: Invalid parameter (%p, %p, %p)\n", RpBus, RpDev, RpFunc));
    return EFI_INVALID_PARAMETER;
  }

  Found  = FALSE;
  IsPresence = FALSE;
  *RpDev  = 0;
  *RpFunc = 0;

  DEBUG ((DEBUG_INFO, "[GetDTbtRpDevFun] RpNum = %d\n", RpNumber));

  //
  // Scan all RP to find PCI BDF for dTBT used
  //
  for (Dev = 0; Dev <= PCI_MAX_DEVICE && Found == FALSE; Dev++) {
    for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
      PcieAddress = PCI_SEGMENT_LIB_ADDRESS (0, RpBus, Dev, Func, 0);
      PciSegmentReadBuffer (PcieAddress, sizeof (Hdr), (VOID *) &Hdr);
      if (IsSkipCheckDevice (&Hdr, &IsPresence)) {
        if ((!IsPresence && Func == 0) || (IsPresence && !IS_PCI_SUPPORT_MULTI_FUNC (&Hdr))) {
          break;
        }
        continue;
      }

      //
      // Retrieve pcie capability pointer
      //
      PcieAddress = PCI_SEGMENT_LIB_ADDRESS (0, RpBus, Dev, Func, PCI_CAPBILITY_POINTER_OFFSET);
      RegVal8 = PciSegmentRead8 (PcieAddress);
      // DEBUG ((DEBUG_INFO, "[GetDTbtRpDevFun] B[%x]D[%x]F[%x]  cap offset %x\n", RpBus, Dev, Func, RegVal8));

      //
      // Find pcie capability
      //
      RegVal16 = (UINT16) RegVal8;
      Status = FindPcieCap ((UINT8) RpBus, Dev, Func, &RegVal16);

      //
      // Check RP number from pcie link capability (offset: 0xc)
      //
      if (!EFI_ERROR (Status)) {
        PcieAddress = PCI_SEGMENT_LIB_ADDRESS (0, RpBus, Dev, Func, RegVal16 + 0xc);
        RegVal32 = PciSegmentRead32 (PcieAddress);
        if (((PCI_REG_PCIE_LINK_CAPABILITY *) &RegVal32)->Bits.PortNumber == RpNumber) {
          DEBUG ((DEBUG_INFO, "[GetDTbtRpDevFun] Found RP = %d \n", RpNumber));
          Found = TRUE;
        } else {
          //
          // Not specific RP
          //
          continue;
        }
      } else {
        //
        // Not found pci-e capability
        //
        continue;
      }
      OrgBus = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (0, RpBus, Dev, Func, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
      // DEBUG ((DEBUG_INFO, "[GetDTbtRpDevFun] B[%x]D[%x]F[%x]  pBus %x\n", RpBus, Dev, Func, OrgBus));

      //
      // Set Sec/Sub buses to a temporary value and prevent bus number over 225 since above 225 is usage
      //
      TempBusNumber = 0x28;
      //if ((RpBus + TempBusNumber) > 225) {
      //  TempBusNumber -= ((UINT8) RpBus + TempBusNumber) - 225 - 16;
      //}
      PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (0, RpBus, Dev, Func, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET),
        (UINT32) (((RpBus + TempBusNumber) << 16) | ((RpBus + TempBusNumber) << 8) | RpBus));

      //
      // Check if exists TBT DP
      //
      PcieAddress = PCI_SEGMENT_LIB_ADDRESS (0, RpBus + TempBusNumber, 0, 0, PCI_DEVICE_ID_OFFSET);
      RegVal16 = PciSegmentRead16 (PcieAddress);
      // DEBUG ((DEBUG_INFO, "[GetDTbtRpDevFun] B[%x]D[%x]F[%x]  DS dev id %x\n", RpBus + TempBusNumber, 0, 0, RegVal16));

      //
      // Reset Sec/Sub buses to original value
      //
      PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (0, RpBus, Dev, Func, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), OrgBus);

      //
      // Confirm if exists TBT PPb under RP
      //
      if (Found && IsTbtHostRouter (RegVal16) == FALSE) {
        DEBUG ((DEBUG_INFO, "[GetDTbtRpDevFun] Found RP %d but not found TBT DP\n", RpNumber));
        Status = EFI_NOT_FOUND;
        Found = FALSE;
      }

      //
      // Found RP bus/dev/function number
      //
      if (Found) {
        *RpDev  = Dev;
        *RpFunc = Func;
        DEBUG ((DEBUG_INFO, "[GetDTbtRpDevFun] Found RP = %d RpBus = %x, RpDev = %x, RpFunc = %x \n", RpNumber, RpBus, *RpDev, *RpFunc));
        return EFI_SUCCESS;
      }
    }
  }

  if (Found == FALSE) {
    DEBUG ((DEBUG_INFO, "[GetDTbtRpDevFun] Cannot find TBT RP\n"));
    Status = EFI_NOT_FOUND;
  }

  return Status;
}

BOOLEAN
IsTbtHostRouter (
  IN    UINT16  DeviceID
  )
{
  switch (DeviceID) {
  case AR_HR_2C:
  case AR_HR_4C:
  case AR_HR_LP:
  case AR_HR_C0_2C:
  case AR_HR_C0_4C:
  case TR_HR_2C:
  case TR_HR_4C:
  case MR_HR_2C:
  case MR_HR_4C:
  case BR_HR_1:
  case BR_HR_2:
    return TRUE;
  }

  return FALSE;
} // IsTbtHostRouter

/**
  Query USB4 version that can be supported by the given host router's VID & PID

  @param[in] VendorId - USB4 host router Vendor ID.
  @param[in] DeviceId - USB4 host router Device ID.

  @retval USB4 version supported by host router.
**/
UINT8 DtbtQueryUsb4Ver (
  IN UINT16    VendorId,
  IN UINT16    DeviceId
  )
{
  UINT8     Usb4Ver;

  Usb4Ver = 1;

  if (VendorId == PCI_VID_INTEL) {
    switch (DeviceId) {
      case BR_HR_1:
      case BR_HR_2:
        Usb4Ver = 2;
        break;
    }
  }

  DEBUG ((DEBUG_INFO, "Discrete USB4 host router (0x%x,0x%x) - USB4 version = %d\n", VendorId, DeviceId, Usb4Ver));
  return Usb4Ver;
}