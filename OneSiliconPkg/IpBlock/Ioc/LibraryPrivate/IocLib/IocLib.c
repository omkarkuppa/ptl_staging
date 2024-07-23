/** @file
  IOC library.

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
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/IocLib.h>
#include <Library/IocRegAccessLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/EspiPrivateLib.h>
#include <Register/EspiRegs.h>
#include <PcieRegs.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Register/HostBridgeRegs.h>

#define MAX_RANGE_INDEX 2 // Only 3 GPMR registers are available
/**
  Set PCH TCO base address decoding in IOC

  @param[in] Address                    Address for TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
IocSetTcoBase (
  IN  UINT16                            Address
  )
{
  IocRegAccessWrite16 ((UINTN)(TCOBASE_IOC_MCHBAR_REG), (Address | BIT1));
  return EFI_SUCCESS;
}

/**
  Get PCH TCO base address.

  @retval Address                   Address of TCO base address.
**/
UINT16
IocGetTcoBase (
  VOID
  )
{
  TCOBASE_IOC_MCHBAR_STRUCT  TcoBase;;

  TcoBase.Data = IocRegAccessRead32 ((UINTN) TCOBASE_IOC_MCHBAR_REG);
  return ((UINT16) (TcoBase.Bits.tcoba << TCOBASE_IOC_MCHBAR_TCOBA_LSB));
}

/**
  Sets IO cycles to target eSPI device

  @param[in] DeviceId                   Target eSPI device (see ESPI_DEVICE_ID)
  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.
  @param[in] RangeIndex                 Index of choosen range

  @retval EFI_SUCCESS                   Successfully completed.
**/
STATIC
EFI_STATUS
IocSetEspiGenIoRangeHelper (
  IN  ESPI_DEVICE_ID                    DeviceId,
  IN  UINT32                            Address,
  IN  UINT32                            Length,
  IN  UINT32                            RangeIndex
  )
{
  UINT32                                Data32;
  UINT32                                AlignedAddressMask;
  UINT32                                AlignedAddress;
  UINTN                                 TargetReg;

  //
  // Only range 0 is supported for CS# other than CS#0
  //
  if (DeviceId != EspiDeviceCS0 && RangeIndex > 0) {
    return EFI_UNSUPPORTED;
  }

  //
  // Per register definition, make sure address and mask are DWORD aligned
  //
  AlignedAddressMask = (Length - 1) >> 2; // Address[7:2] Mask (ADDRMASK)
  AlignedAddress     = Address >> 2;      // Address[15:2] (ADDR)

  Data32 =  (UINT32) ((AlignedAddressMask << N_ESPI_CFG_ESPI_LGIR1_ADDR_MASK) & B_ESPI_CFG_ESPI_LGIR1_ADDR_MASK);
  Data32 |= (UINT32) ((AlignedAddress << N_ESPI_CFG_ESPI_LGIR1_ADDR) & B_ESPI_CFG_ESPI_LGIR1_ADDR);
  Data32 |= B_ESPI_CFG_ESPI_LGIR1_LDE;

  switch (DeviceId) {
    case EspiDeviceCS1:
      TargetReg = SEGIR_IOC_MCHBAR_REG;
      break;
    case EspiDeviceCS2:
      TargetReg = E3GIR_IOC_MCHBAR_REG;
      break;
    case EspiDeviceCS3:
      TargetReg = E4GIR_IOC_MCHBAR_REG;
      break;
    default:
      TargetReg = LPCLGIR1_IOC_MCHBAR_REG + (RangeIndex * 4);
      break;
  }

  IocRegAccessWrite32 (TargetReg, Data32);
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI generic IO range decoding in IOC

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.
  @param[in] RangeIndex                 Index of choosen range

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetLpcGenIoRange (
  IN  UINT32                            Address,
  IN  UINT32                            Length,
  IN  UINT32                            RangeIndex
  )
{
  return IocSetEspiGenIoRangeHelper (EspiDeviceCS0, Address, Length, RangeIndex);
}

/**
  Set PCH eSPI CS1# generic IO range decoding in IOC

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetEspiCs1GenIoRange (
  IN  UINT32                            Address,
  IN  UINT32                            Length
  )
{
  return IocSetEspiGenIoRangeHelper (EspiDeviceCS1, Address, Length, 0);
}


/**
  Set PCH eSPI CS#2 generic IO range decoding in IOC

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetEspiCs2GenIoRange (
  IN  UINT32                            Address,
  IN  UINT32                            Length
  )
{
  return IocSetEspiGenIoRangeHelper (EspiDeviceCS2, Address, Length, 0);
}

/**
  Set PCH eSPI CS#3 generic IO range decoding in IOC

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetEspiCs3GenIoRange (
  IN  UINT32                            Address,
  IN  UINT32                            Length
  )
{
  return IocSetEspiGenIoRangeHelper (EspiDeviceCS3, Address, Length, 0);
}

/**
  Clear PCH LPC/eSPI generic IO range decoding in IOC

  @param[in] RangeIndex                 Index of chosen range

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocClearLpcGenIoRange (
  IN  UINTN                             RangeIndex
  )
{
  IocRegAccessWrite32 ((UINTN) (LPCLGIR1_IOC_MCHBAR_REG + RangeIndex * 4), 0);
  return EFI_SUCCESS;
}

/**
  Clear PCH eSPI CS1# generic IO range decoding in IOC

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocClearEspiCs1GenIoRange (
  VOID
  )
{
  IocRegAccessWrite32 ((UINTN)SEGIR_IOC_MCHBAR_REG, 0);
  return EFI_SUCCESS;
}

/**
  Clear PCH eSPI CS#2 generic IO range decoding in IOC

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocClearEspiCs2GenIoRange (
  VOID
  )
{
  IocRegAccessWrite32 ((UINTN)E3GIR_IOC_MCHBAR_REG, 0);
  return EFI_SUCCESS;
}

/**
  Clear PCH eSPI CS#1 generic IO range decoding in IOC

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocClearEspiCs3GenIoRange (
  VOID
  )
{
  IocRegAccessWrite32 ((UINTN)E4GIR_IOC_MCHBAR_REG, 0);
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI memory range decoding in IOC

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetLpcMemRange (
  IN  UINT32                            Address
  )
{
  IocRegAccessWrite32 ((UINTN)LPCGMR_IOC_MCHBAR_REG, (Address | B_ESPI_CFG_ESPI_LGMR_LGMRD_EN));
  return EFI_SUCCESS;
}

/**
  Set PCH eSPI CS1# memory range decoding in IOC

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetEspiCs1MemRange (
  IN  UINT32                            Address
  )
{
  IocRegAccessWrite32 ((UINTN)SEGMR_IOC_MCHBAR_REG, (Address | B_ESPI_CFG_ESPI_LGMR_LGMRD_EN));
  return EFI_SUCCESS;
}


/**
  Set PCH eSPI CS#2 memory range decoding in IOC

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetEspiCs2MemRange (
  IN  UINT32                            Address
  )
{
  IocRegAccessWrite32 ((UINTN)E3GMR_IOC_MCHBAR_REG, (Address | B_ESPI_CFG_ESPI_LGMR_LGMRD_EN));
  return EFI_SUCCESS;
}

/**
  Set PCH eSPI CS#3 memory range decoding in IOC

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetEspiCs3MemRange (
  IN  UINT32                            Address
  )
{
  IocRegAccessWrite32 ((UINTN)E4GMR_IOC_MCHBAR_REG, (Address | B_ESPI_CFG_ESPI_LGMR_LGMRD_EN));
  return EFI_SUCCESS;
}

/**
  Check if Boot BIOS Strap is set for SPI.

  @retval TRUE                Boot BIOS Strap set for SPI
  @retval FALSE               Boot BIOS Strap set for LPC/eSPI
**/
BOOLEAN
IocIsBootBiosStrapSetForSpi (
  VOID
  )
{
  GCS_IOC_MCHBAR_STRUCT  GcsStrcut;

  GcsStrcut.Data = IocRegAccessRead32 ((UINTN) GCS_IOC_MCHBAR_REG);
  return (BOOLEAN) GcsStrcut.Bits.bbs;
}

/**
  Set PCH BIOS range decoding in IOC
  Please check EDS for detail of BiosDecodeEnable bit definition.
    bit 15: F8-FF Enable
    bit 14: F0-F8 Enable
    bit 13: E8-EF Enable
    bit 12: E0-E8 Enable
    bit 11: D8-DF Enable
    bit 10: D0-D7 Enable
    bit  9: C8-CF Enable
    bit  8: C0-C7 Enable
    bit  7: Legacy F Segment Enable
    bit  6: Legacy E Segment Enable
    bit  5: Reserved
    bit  4: Reserved
    bit  3: 70-7F Enable
    bit  2: 60-6F Enable
    bit  1: 50-5F Enable
    bit  0: 40-4F Enable

  @param[in] BiosDecodeEnable           Bios decode enable setting.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetBiosDecodeEnable (
  IN  UINT16                            BiosDecodeEnable
  )
{
  IocRegAccessWrite32 ((UINTN)LPCBDE_IOC_MCHBAR_REG, BiosDecodeEnable);
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI IO decode ranges in IOC
  Please check EDS for detail of LPC/eSPI IO decode ranges bit definition.
  Bit  12: FDD range
  Bit 9:8: LPT range
  Bit 6:4: ComB range
  Bit 2:0: ComA range

  @param[in] LpcIoDecodeRanges          LPC/eSPI IO decode ranges bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetLpcIoDecodeRanges (
  IN  UINT16                            LpcIoDecodeRanges
  )
{
  IocRegAccessWrite32 ((UINTN)LPCIOD_IOC_MCHBAR_REG, LpcIoDecodeRanges);
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI IO enable decoding in IOC

  @param[in] LpcIoEnableDecoding        LPC/eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetLpcIoEnable (
  IN  UINT16                            LpcIoEnableDecoding
  )
{
  IocRegAccessWrite32 ((UINTN)LPCIOE_IOC_MCHBAR_REG, LpcIoEnableDecoding);
  return EFI_SUCCESS;
}


/**
  Set PCH IO port 80h cycle decoding to PCIE root port in IOC

  @param[in] IocRpDestinationId         IOC Rootport destination ID

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetIoPort80Decode (
  IN  IOC_ROOT_PORT_DEST_ID  IocRpDestinationId
  )
{
  UINT16                 Data16;
  GCS_IOC_MCHBAR_STRUCT  GcsStruct;

  ///
  /// IO port 80h is typically used by decoder/LED hardware for debug purposes.
  /// By default PCH will forward IO port 80h cycles to LPC bus. The Reserved Page Route (RPR) bit
  /// of General Control and Status register, located at PCR[DMI] + 274Ch[11] , allows software to
  /// re-direct IO port 80h cycles to PCIe bus so that a target (for example, a debug card) on
  /// PCIe bus can receive and claim these cycles.
  /// The "RPR Destination ID", PCR[DMI] + 274Ch[31:16] need to be set accordingly to point
  /// to the root port that decode this range. Reading from Port 80h may not return valid values
  /// if the POST-card itself do not shadow the writes. Unlike LPC, PCIe does not shadow the Port 80 writes.
  ///

  Data16 = (UINT16)((0x2 << 13) |
                    (IocRpDestinationId.PsfId << 9) |
                    (IocRpDestinationId.PortGroupId << 8) |
                    (IocRpDestinationId.PortId << 4) |
                     IocRpDestinationId.ChannelId);

  IocRegAccessWrite16 ((UINTN) GCS_IOC_MCHBAR_REG + 2, Data16);

  GcsStruct.Data = IocRegAccessRead32 ((UINTN) GCS_IOC_MCHBAR_REG);
  GcsStruct.Bits.rpr = 1;
  IocRegAccessWrite32 ((UINTN) GCS_IOC_MCHBAR_REG, GcsStruct.Data);

  return EFI_SUCCESS;
}

/**
  Determines where to send the reserved page registers
  Accesses to the I/O ranges 80h - 8Fh will be forwarded to PCIe Root Port
  with the destination ID specified in GCS.RPRDID using IOC source decode.
**/
VOID
IocSetReservedPageRegToPcieRootPort (
  VOID
  )
{
  GCS_IOC_MCHBAR_STRUCT  GcsStruct;

  GcsStruct.Data = IocRegAccessRead32 ((UINTN) GCS_IOC_MCHBAR_REG);
  GcsStruct.Bits.rpr = 1;
  IocRegAccessWrite32 ((UINTN) GCS_IOC_MCHBAR_REG, GcsStruct.Data);
}

/**
  Determines where to send the reserved page registers
  IOC will not perform source decode on the I/O ranges 80h - 8Fh. The cycles hitting these ranges will
  end up in P2SB which will then forward the cycle to LPC or eSPI through IOSF Sideband.
**/
VOID
IocSetReservedPageRegToLpc (
  VOID
  )
{
  GCS_IOC_MCHBAR_STRUCT  GcsStruct;

  GcsStruct.Data = IocRegAccessRead32 ((UINTN) GCS_IOC_MCHBAR_REG);
  GcsStruct.Bits.rpr = 0;
  IocRegAccessWrite32 ((UINTN) GCS_IOC_MCHBAR_REG, GcsStruct.Data);
}

/**
  uCode Patch Region Enable (UPRE). Enables memory access targeting the uCode patch region (0xFEF00000 to 0xFEFFFFFF)
  to be forwarded to SPI Flash. This can only be set if the boot flash is on SPI.
**/
VOID
IocEnableUCodePatchRegion (
  VOID
  )
{
  UCPR_IOC_MCHBAR_STRUCT Ucpr;

  Ucpr.Data = IocRegAccessRead32 ((UINTN) UCPR_IOC_MCHBAR_REG);
  Ucpr.Bits.upre = 1;
  IocRegAccessWrite32 ((UINTN) UCPR_IOC_MCHBAR_REG, Ucpr.Data);
}

/**
  Set BIOS interface Lock-Down
**/
VOID
IocSetBiosLockDown (
  VOID
  )
{
  GCS_IOC_MCHBAR_STRUCT Gcs;
  Gcs.Data = IocRegAccessRead32 ((UINTN) GCS_IOC_MCHBAR_REG);
  Gcs.Bits.bild = 1;
  IocRegAccessWrite32 ((UINTN) GCS_IOC_MCHBAR_REG, Gcs.Data);
}

/**
  Makes memory cycles that fall within the given range inclusive to be
  forwarded using source decode to the specified Destination ID

  @param[in]    RangeIndex      Index of which General Purpose Memory
                                Range shall be configured
  @param[in]    RangeBase       Base of Source Decode Range, bits[15:0]
                                of range are assumed to be 0000h
  @param[in]    RangeLimit      Limit of Source Decode range, bits[15:0]
                                of range are assumed to be FFFFh
  @param[in]    DestinationId   The destination ID to be used to forward
                                the cycle decode

  @retval   EFI_SUCCESS               Range was programmed and enabled
  @retval   EFI_INVALID_PARAMETER     One of arguments is incorrect
**/
EFI_STATUS
IocSetGeneralPurposeMemoryRange (
  UINT8     RangeIndex,
  UINT32    RangeBase,
  UINT32    RangeLimit,
  UINT32    DestinationId
  )
{

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  if (RangeIndex > MAX_RANGE_INDEX) {
    DEBUG ((
      DEBUG_ERROR,
      "RangeIndex is invalid. Max value is %d but requested to configure range %d.\n",
      MAX_RANGE_INDEX,
      RangeIndex
      ));
    return EFI_INVALID_PARAMETER;
  }

  // Check for valid range
  if (RangeBase >= RangeLimit) {
    DEBUG ((
      DEBUG_ERROR,
      "Invalid range for configuration. Range base must me smaller than range limit.\n"
    ));
    return EFI_INVALID_PARAMETER;
  }
  IocRegAccessWrite32 ((UINTN) GPMR1_IOC_MCHBAR_REG + (RangeIndex * 8), (UINT32) ((RangeLimit & 0xFFFF0000) | ((RangeBase >> 16) & 0xFFFF)));
  IocRegAccessWrite32 ((UINTN) GPMR1DID_IOC_MCHBAR_REG + (RangeIndex * 8), (UINT32) ((DestinationId & 0x7FFFFFFF) | (1 << GPMR1DID_IOC_MCHBAR_GPMRDE_LSB)));
  return EFI_SUCCESS;
}