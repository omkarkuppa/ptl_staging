/** @file
  Header file for IocLib.

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
#ifndef _IOC_LIB_H_
#define _IOC_LIB_H_

typedef struct {
  UINT8  PsfId;
  UINT8  PortGroupId;
  UINT8  PortId;
  UINT8  ChannelId;
} IOC_ROOT_PORT_DEST_ID;

/**
  Set PCH TCO base address decoding in IOC

  @param[in] Address                    Address for TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
IocSetTcoBase (
  IN  UINT16                            Address
  );

/**
  Get PCH TCO base address.

  @retval Address                   Address of TCO base address.
**/
UINT16
IocGetTcoBase (
  VOID
  );

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
  );

/**
  Set PCH eSPI CS#1 generic IO range decoding in IOC

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetEspiCs1GenIoRange (
  IN  UINT32                            Address,
  IN  UINT32                            Length
  );

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
  );

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
  );

/**
  Clear PCH LPC/eSPI generic IO range decoding in IOC

  @param[in] RangeIndex                 Index of chosen range

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocClearLpcGenIoRange (
  IN  UINTN                             RangeIndex
  );

/**
  Clear PCH eSPI CS#1 generic IO range decoding in IOC

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocClearEspiCs1GenIoRange (
  VOID
  );

/**
  Clear PCH eSPI CS#2 generic IO range decoding in IOC

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocClearEspiCs2GenIoRange (
  VOID
  );

/**
  Clear PCH eSPI CS#1 generic IO range decoding in IOC

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocClearEspiCs3GenIoRange (
  VOID
  );

/**
  Set PCH LPC/eSPI memory range decoding in IOC

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetLpcMemRange (
  IN  UINT32                            Address
  );

/**
  Set PCH eSPI CS#1 memory range decoding in IOC

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetEspiCs1MemRange (
  IN  UINT32                            Address
  );

/**
  Set PCH eSPI CS#2 memory range decoding in IOC

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetEspiCs2MemRange (
  IN  UINT32                            Address
  );

/**
  Set PCH eSPI CS#3 memory range decoding in IOC

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetEspiCs3MemRange (
  IN  UINT32                            Address
  );

/**
  Check if Boot BIOS Strap is set for SPI.

  @retval TRUE                Boot BIOS Strap set for SPI
  @retval FALSE               Boot BIOS Strap set for LPC/eSPI
**/
BOOLEAN
IocIsBootBiosStrapSetForSpi (
  VOID
  );

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
  );

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
  );

/**
  Set PCH LPC/eSPI IO enable decoding in IOC

  @param[in] LpcIoEnableDecoding        LPC/eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetLpcIoEnable (
  IN  UINT16                            LpcIoEnableDecoding
  );

/**
  Set PCH IO port 80h cycle decoding to PCIE root port in IOC

  @param[in] IocRpDestinationId         IOC Rootport destination ID

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
IocSetIoPort80Decode (
  IN  IOC_ROOT_PORT_DEST_ID  IocRpDestinationId
  );

/**
  Determines where to send the reserved page registers
  Accesses to the I/O ranges 80h - 8Fh will be forwarded to PCIe Root Port
  with the destination ID specified in GCS.RPRDID using IOC source decode.
**/
VOID
IocSetReservedPageRegToPcieRootPort (
  VOID
  );

/**
  Determines where to send the reserved page registers
  IOC will not perform source decode on the I/O ranges 80h - 8Fh. The cycles hitting these ranges will
  end up in P2SB which will then forward the cycle to LPC or eSPI through IOSF Sideband.
**/
VOID
IocSetReservedPageRegToLpc (
  VOID
  );

/**
  uCode Patch Region Enable (UPRE). Enables memory access targeting the uCode patch region (0xFEF00000 to 0xFEFFFFFF)
  to be forwarded to SPI Flash. This can only be set if the boot flash is on SPI.
**/
VOID
IocEnableUCodePatchRegion (
  VOID
  );

/**
  Set BIOS interface Lock-Down
**/
VOID
IocSetBiosLockDown (
  VOID
  );

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
  );

#endif // _IOC_LIB_H_
