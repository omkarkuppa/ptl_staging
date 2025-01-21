/** @file
  Header file for PchCycleDecodingLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _PCH_CYCLE_DECODING_LIB_H_
#define _PCH_CYCLE_DECODING_LIB_H_

/**
  Set PCH TCO base address.
  This cycle decoding is allowed to set when DMIC.SRL is 0.
  Programming steps:
  1. set Smbus PCI offset 54h [8] to enable TCO base address.
  2. program Smbus PCI offset 50h [15:5] to TCO base address.
  3. set Smbus PCI offset 54h [8] to enable TCO base address.
  4. program "TCO Base Address" PCR[DMI] + 2778h[15:5, 1] to [Smbus PCI offset 50h[15:5], 1].

  @param[in] Address                    Address for TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
PchTcoBaseSet (
  IN  UINT16                            Address
  );

/**
  Get PCH TCO base address.

  @param[out] Address                   Address of TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid pointer passed.
**/
EFI_STATUS
PchTcoBaseGet (
  OUT UINT16                            *Address
  );

///
/// structure of LPC general IO range register
/// It contains base address, address mask, and enable status.
///
typedef struct {
  UINT32                                BaseAddr :16;
  UINT32                                Length   :15;
  UINT32                                Enable   : 1;
} PCH_LPC_GEN_IO_RANGE;

#define PCH_LPC_GEN_IO_RANGE_MAX        4
#define ESPI_CS1_GEN_IO_RANGE_MAX       1
#define ESPI_CS2_GEN_IO_RANGE_MAX       1
#define ESPI_CS3_GEN_IO_RANGE_MAX       1

///
/// structure of LPC general IO range register list
/// It lists all LPC general IO ran registers supported by PCH.
///
typedef struct {
  PCH_LPC_GEN_IO_RANGE                  Range[PCH_LPC_GEN_IO_RANGE_MAX];
} PCH_LPC_GEN_IO_RANGE_LIST;

/**
  Set PCH LPC/eSPI generic IO range.
  For generic IO range, the base address must align to 4 and less than 0xFFFF, and the length must be power of 2
  and less than or equal to 256. Moreover, the address must be length aligned.
  This function basically checks the address and length, which should not overlap with all other generic ranges.
  If no more generic range register available, it returns out of resource error.
  This cycle decoding is also required on DMI side.
  Some IO ranges below 0x100 have fixed target. The target might be ITSS,RTC,LPC,PMC or terminated inside P2SB
  but all predefined and can't be changed. IO range below 0x100 will be rejected in this function except below ranges:
    0x00-0x1F,
    0x44-0x4B,
    0x54-0x5F,
    0x68-0x6F,
    0x80-0x8F,
    0xC0-0xFF
  Steps of programming generic IO range:
  1. Program LPC/eSPI PCI Offset 84h ~ 93h of Mask, Address, and Enable.
  2. Program LPC/eSPI Generic IO Range in DMI

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcGenIoRangeSet (
  IN  UINT16                            Address,
  IN  UINTN                             Length
  );

/**
  Clear IBL eSPI generic IO range.
  Clear Generic IO range if already programmed.
  If Address/Length not match any already programmed values return EFI_NOT_FOUND status.

  Steps of programming generic IO range:
  1. Clear eSPI PCI Offset 84h ~ 93h of Mask, Address, and Enable.

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_NOT_FOUND                 Address/Length not found in alredy programmed Gen IO.
**/
EFI_STATUS
PchLpcGenIoRangeClear (
  IN  UINT16                            Address,
  IN  UINTN                             Length
  );

/**
  Set PCH eSPI CS1# generic IO range decoding.
  For generic IO range, the base address must align to 4 and less than 0xFFFF, and the length must be power of 2
  and less than or equal to 256. Moreover, the address must be length aligned.
  This function basically checks the address and length, which should not overlap with all other generic ranges.
  If no more generic range register available, it returns out of resource error.
  This cycle decoding is also required on DMI side
  Some IO ranges below 0x100 have fixed target. The target might be ITSS,RTC,LPC,PMC or terminated inside P2SB
  but all predefined and can't be changed. IO range below 0x100 will be rejected in this function except below ranges:
    0x00-0x1F,
    0x44-0x4B,
    0x54-0x5F,
    0x68-0x6F,
    0x80-0x8F,
    0xC0-0xFF
  Steps of programming generic IO range:
  1. Program eSPI PCI Offset A4h (eSPI CS1#) of Mask, Address, and Enable.
  2. Program eSPI Generic IO Range in DMI

  @param[in] Address                    Address for generic IO range decoding.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs1GenIoRangeSet (
  IN  UINT16                            Address,
  IN  UINTN                             Length
  );

/**
  Set PCH eSPI CS#2 generic IO range decoding.
  For generic IO range, the base address must align to 4 and less than 0xFFFF, and the length must be power of 2
  and less than or equal to 256. Moreover, the address must be length aligned.
  This function basically checks the address and length, which should not overlap with all other generic ranges.
  If no more generic range register available, it returns out of resource error.
  This cycle decoding is also required on DMI side
  Some IO ranges below 0x100 have fixed target. The target might be ITSS,RTC,LPC,PMC or terminated inside P2SB
  but all predefined and can't be changed. IO range below 0x100 will be rejected in this function except below ranges:
    0x00-0x1F,
    0x44-0x4B,
    0x54-0x5F,
    0x68-0x6F,
    0x80-0x8F,
    0xC0-0xFF
  Steps of programming generic IO range:
  1. Program eSPI PCI Offset 204h (eSPI CS#2) of Mask, Address, and Enable.
  2. Program eSPI Generic IO Range in DMI

  @param[in] Address                    Address for generic IO range decoding.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs2GenIoRangeSet (
  IN  UINT16                            Address,
  IN  UINTN                             Length
  );

/**
  Set PCH eSPI CS#3 generic IO range decoding.
  For generic IO range, the base address must align to 4 and less than 0xFFFF, and the length must be power of 2
  and less than or equal to 256. Moreover, the address must be length aligned.
  This function basically checks the address and length, which should not overlap with all other generic ranges.
  If no more generic range register available, it returns out of resource error.
  This cycle decoding is also required on DMI side
  Some IO ranges below 0x100 have fixed target. The target might be ITSS,RTC,LPC,PMC or terminated inside P2SB
  but all predefined and can't be changed. IO range below 0x100 will be rejected in this function except below ranges:
    0x00-0x1F,
    0x44-0x4B,
    0x54-0x5F,
    0x68-0x6F,
    0x80-0x8F,
    0xC0-0xFF
  Steps of programming generic IO range:
  1. Program eSPI PCI Offset 244h (eSPI CS#3) of Mask, Address, and Enable.
  2. Program eSPI Generic IO Range in DMI

  @param[in] Address                    Address for generic IO range decoding.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs3GenIoRangeSet (
  IN  UINT16                            Address,
  IN  UINTN                             Length
  );

/**
  Get PCH LPC/eSPI generic IO range list.
  This function returns a list of base address, length, and enable for all LPC/eSPI generic IO range registers.

  @param[out] LpcGenIoRangeList         Return all LPC/eSPI generic IO range register status.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
PchLpcGenIoRangeGet (
  OUT PCH_LPC_GEN_IO_RANGE_LIST         *LpcGenIoRangeList
  );

/**
  Get PCH eSPI CS#1 generic IO range list.
  This function returns a list of base address, length, and enable for all eSPI CS#1 generic IO range registers.

  @param[out] GenIoRangeList            eSPI generic IO range registers.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs1GenIoRangeGet (
  OUT PCH_LPC_GEN_IO_RANGE_LIST         *GenIoRangeList
  );

/**
  Get PCH eSPI CS#2 generic IO range list.
  This function returns a list of base address, length, and enable for all eSPI CS#2 generic IO range registers.

  @param[out] GenIoRangeList            eSPI generic IO range registers.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs2GenIoRangeGet (
  OUT PCH_LPC_GEN_IO_RANGE_LIST         *GenIoRangeList
  );


/**
  Get PCH eSPI CS#3 generic IO range list.
  This function returns a list of base address, length, and enable for all eSPI CS#3 generic IO range registers.

  @param[out] GenIoRangeList            eSPI generic IO range registers.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs3GenIoRangeGet (
  OUT PCH_LPC_GEN_IO_RANGE_LIST         *GenIoRangeList
  );

/**
  Set PCH LPC/eSPI memory range decoding.
  This cycle decoding is required to be set on DMI side
  Programming steps:
  1. Program bit [0] to 0b to disable memory decoding first before changing base address.
  2. Program bits [31:16, 0] to [Address, 1].
  3. Program LPC Memory Range in DMI

  @param[in] Address                    Address for memory base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcMemRangeSet (
  IN  UINT32                            Address
  );

/**
  Set PCH eSPI CS#1 memory range decoding.
  This cycle decoding is required to be set on DMI side
  Programming steps:
  1. Program bit [0] to 0b to disable memory decoding first before changing base address.
  2. Program bits [31:16, 0] to [Address, 1].
  3. Program eSPI Memory Range in DMI

  @param[in] Address                    Address for memory for decoding.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs1MemRangeSet (
  IN  UINT32                            Address
  );

/**
  Set PCH eSPI CS#2 memory range decoding.
  This cycle decoding is required to be set on DMI side
  Programming steps:
  1. Program bit [0] to 0b to disable memory decoding first before changing base address.
  2. Program bits [31:16, 0] to [Address, 1].
  3. Program eSPI Memory Range in DMI

  @param[in] Address                    Address for memory for decoding.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs2MemRangeSet (
  IN  UINT32                            Address
  );

/**
  Set PCH eSPI CS#3 memory range decoding.
  This cycle decoding is required to be set on DMI side
  Programming steps:
  1. Program bit [0] to 0b to disable memory decoding first before changing base address.
  2. Program bits [31:16, 0] to [Address, 1].
  3. Program eSPI Memory Range in DMI

  @param[in] Address                    Address for memory for decoding.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs3MemRangeSet (
  IN  UINT32                            Address
  );

/**
  Get PCH LPC/eSPI memory range decoding address.

  @param[out] Address                   Address of LPC/eSPI memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
PchLpcMemRangeGet (
  OUT UINT32                            *Address
  );

/**
  Get PCH eSPI CS#1 memory range decoding address.

  @param[out] Address                   Address of eSPI CS#1 memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs1MemRangeGet (
  OUT UINT32                            *Address
  );

/**
  Get PCH eSPI CS#2 memory range decoding address.

  @param[out] Address                   Address of eSPI CS#2 memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs2MemRangeGet (
  OUT UINT32                            *Address
  );

/**
  Get PCH eSPI CS#3 memory range decoding address.

  @param[out] Address                   Address of eSPI CS#3 memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs3MemRangeGet (
  OUT UINT32                            *Address
  );

/**
  Set PCH LPC IO decode ranges.
  Program LPC I/O Decode Ranges, PCR[DMI] + 2770h[15:0] to the same value programmed in LPC offset 80h.
  Please check EDS for detail of Lpc IO decode ranges bit definition.
  Bit  12: FDD range
  Bit 9:8: LPT range
  Bit 6:4: ComB range
  Bit 2:0: ComA range

  @param[in] LpcIoDecodeRanges          Lpc IO decode ranges bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcIoDecodeRangesSet (
  IN  UINT16                            LpcIoDecodeRanges
  );

/**
  Set PCH LPC and eSPI CS0# IO enable decoding.
  Setup I/O Enables in DMI to the same value program in LPC/eSPI PCI offset 82h.
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in LPC/eSPI PCI offset 82h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of LPC/eSPI IO decode ranges bit definition.

  @param[in] LpcIoEnableDecoding        LPC IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcIoEnableDecodingSet (
  IN  UINT16                            LpcIoEnableDecoding
  );

/**
  Set PCH eSPI CS#1 IO enable decoding.
  Setup I/O Enables in DMI to the same value program in eSPI PCI offset A0h (eSPI CS#1).
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in eSPI PCI offset A0h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of eSPI IO decode ranges bit definition.

  @param[in] IoEnableDecoding           eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMI configuration is locked
**/
EFI_STATUS
PchEspiCs1IoEnableDecodingSet (
  IN  UINT16                            IoEnableDecoding
  );

/**
  Set PCH eSPI CS#2 IO enable decoding.
  Setup I/O Enables in DMI to the same value program in eSPI PCI offset 200h (eSPI CS#2).
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in eSPI PCI offset A0h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of eSPI IO decode ranges bit definition.

  @param[in] IoEnableDecoding           eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMI configuration is locked
**/
EFI_STATUS
PchEspiCs2IoEnableDecodingSet (
  IN  UINT16                            IoEnableDecoding
  );

/**
  Set PCH eSPI CS#3 IO enable decoding.
  Setup I/O Enables in DMI to the same value program in eSPI PCI offset 240h (eSPI CS#3).
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in eSPI PCI offset A0h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of eSPI IO decode ranges bit definition.

  @param[in] IoEnableDecoding           eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMI configuration is locked
**/
EFI_STATUS
PchEspiCs3IoEnableDecodingSet (
  IN  UINT16                            IoEnableDecoding
  );

/**
  AND then OR PCH LPC and eSPI CS0# IO enable decoding.
  Setup I/O Enables in DMI to the same value program in LPC/eSPI PCI offset 82h.
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in LPC/eSPI PCI offset 82h[13:10] is always forwarded by DMI to subtractive agent for handling.

  @param[in] AndValue        LPC IO enable decoding "AND" value.
  @param[in] OrValue         LPC IO enable decoding "OR" value.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcIoEnableDecodingAndThenOr (
  IN  UINT16        AndValue,
  IN  UINT16        OrValue
  );

/**
  Set PCH IO port 80h cycle decoding to PCIE root port.
  System BIOS is likely to do this very soon after reset before PCI bus enumeration, it must ensure that
  the IO Base Address field (PCIe:1Ch[7:4]) contains a value greater than the IO Limit field (PCIe:1Ch[15:12])
  before setting the IOSE bit. Otherwise the bridge will positively decode IO range 000h - FFFh by its default
  IO range values.
  This cycle decoding is allowed to set when DMIC.SRL is 0.
  Programming steps:
  1. Program "RPR Destination ID", PCR[DMI] + 274Ch[31:16] to the Dest ID of RP.
  2. Program "Reserved Page Route", PCR[DMI] + 274Ch[11] to '1'. Use byte write on GCS+1 and leave the BILD bit which is RWO.
  3. Program IOSE bit of PCIE:Reg04h[0] to '1'  for PCH to send such IO cycles to PCIe bus for subtractive decoding.

  @param[in] RpPhyNumber                PCIE root port physical number.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
PchIoPort80DecodeSet (
  IN  UINTN                             RpPhyNumber
  );

/**
  Get IO APIC regsiters base address.

  @param[out] IoApicBase                Buffer of IO APIC regsiter address

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
PchIoApicBaseGet (
  OUT UINT32                            *IoApicBase
  );

/**
  Get HPET base address.
  This function will be unavailable after P2SB is hidden by PSF.

  @param[out] HpetBase                  Buffer of HPET base address

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchHpetBaseGet (
  OUT UINTN                            *HpetBase
  );

#endif // _PCH_CYCLE_DECODING_LIB_H_
