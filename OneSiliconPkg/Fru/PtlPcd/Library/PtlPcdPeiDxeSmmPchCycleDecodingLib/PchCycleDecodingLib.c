/** @file
  PCH cycle decoding configuration and query library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchDmiLib.h>
#include <Library/IocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/PchRegs.h>
#include <Register/EspiRegs.h>
#include <Register/SpiRegs.h>
#include <Register/SmbusRegs.h>
#include <Library/EspiPrivateLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <PcdSbPortIds.h>

/**
  Get Register Access Protocol (RAP) for eSPI Sideband registers access

  @param[in, out] P2SbController          P2SB controller pointer
  @param[in, out] EspiPcrAccess           P2SB Register Access to eSPI
**/
VOID
EspiGetSidebandRegAccessProtocol (
  IN OUT P2SB_CONTROLLER               *P2SbController,
  IN OUT P2SB_SIDEBAND_REGISTER_ACCESS *EspiPcrAccess
  )
{
  P2SB_PORT_16_ID                 P2SBPid;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_SPBC;
  PtlPcdGetP2SbController (P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, EspiPcrAccess);
}

/**
  Set PCH TCO base address.
  This cycle decoding is required also on DMI side
  Programming steps:
  1. set Smbus PCI offset 54h [8] to enable TCO base address.
  2. program Smbus PCI offset 50h [15:5] to TCO base address.
  3. set Smbus PCI offset 54h [8] to enable TCO base address.
  4. program "TCO Base Address" in DMI

  @param[in] Address                    Address for TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchTcoBaseSet (
  IN  UINT16 Address
  )
{
  UINT64      SmbusBase;
  EFI_STATUS  Status;

  if ((Address & ~B_SMBUS_CFG_TCOBASE_TCOBA) != 0) {
    DEBUG ((DEBUG_ERROR, "PchTcoBaseSet Error. Invalid Address: %x.\n", Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = IocSetTcoBase (Address);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  SmbusBase = SmbusPciCfgBase ();
  if (PciSegmentRead16 (SmbusBase) == 0xFFFF) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  //
  // Verify TCO base is not locked.
  //
  if ((PciSegmentRead8 (SmbusBase + R_SMBUS_CFG_TCOCTL) & B_SMBUS_CFG_TCOCTL_TCO_BASE_LOCK) != 0) {
    ASSERT (FALSE);
    return EFI_DEVICE_ERROR;
  }

  //
  // Disable TCO in SMBUS Device first before changing base address.
  // Byte access to not touch the TCO_BASE_LOCK bit
  //
  PciSegmentAnd8 (
    SmbusBase + R_SMBUS_CFG_TCOCTL + 1,
    (UINT8) ~(B_SMBUS_CFG_TCOCTL_TCO_BASE_EN >> 8)
    );
  //
  // Program TCO in SMBUS Device
  //
  PciSegmentAndThenOr16 (
    SmbusBase + R_SMBUS_CFG_TCOBASE,
    (UINT16)(~B_SMBUS_CFG_TCOBASE_TCOBA),
    Address
    );
  //
  // Enable TCO in SMBUS Device and lock TCO BASE
  //
  PciSegmentOr16 (
    SmbusBase + R_SMBUS_CFG_TCOCTL,
    B_SMBUS_CFG_TCOCTL_TCO_BASE_EN | B_SMBUS_CFG_TCOCTL_TCO_BASE_LOCK
    );

  return Status;
}

/**
  Get PCH TCO base address.

  @param[out] Address                   Address of TCO base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid pointer passed.
**/
EFI_STATUS
PchTcoBaseGet (
  OUT UINT16 *Address
  )
{
  if (Address == NULL) {
    DEBUG ((DEBUG_ERROR, "PchTcoBaseGet Error. Invalid pointer.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  *Address = IocGetTcoBase ();

  return EFI_SUCCESS;
}

/**
  Function checks if passed Generic LPC IO Address and Length meets requirements.

  @param[in] Address                    Address for generic IO range decoding.
  @param[in] Length                     Length of generic IO range.

  @retval TRUE                          Passed IO range meets requirements
  @retval FALSE                         Passed IO range does not meets requirements.
**/
STATIC
BOOLEAN
IsLpcIoRangeValid (
  IN  UINT32 Address,
  IN  UINT32 Length
  )
{
  UINT32  Index;
  UINT32  NumRanges;

  STATIC struct EXCEPT_RANGE {
    UINT8    Start;
    UINT8    Length;
  } ExceptRanges[] = {
    { 0x00, 0x20 }, { 0x44, 0x08 }, { 0x54, 0x0C }, { 0x68, 0x08 }, { 0x80, 0x10 }, { 0xC0, 0x40 }
  };

  NumRanges = ARRAY_SIZE (ExceptRanges);
  //
  // For generic IO range, the base address must align to 4 and less than 0xFFFF,
  // the length must be power of 2 and less than or equal to 256, and the address must be length aligned.
  // IO range below 0x100 will be rejected in this function except below ranges:
  //   0x00-0x1F,
  //   0x44-0x4B,
  //   0x54-0x5F,
  //   0x68-0x6F,
  //   0x80-0x8F,
  //   0xC0-0xFF
  //
  if (((Length & (Length - 1)) != 0)  ||
      ((Address & (UINT16) ~B_ESPI_CFG_ESPI_LGIR1_ADDR) != 0) ||
      (Length > 256))
  {
    return FALSE;
  }

  if (Address < 0x100) {
    for (Index = 0; Index < NumRanges; Index++) {
      if ((Address >= ExceptRanges[Index].Start) &&
          ((Address + Length) <= ((UINTN)ExceptRanges[Index].Start + (UINTN)ExceptRanges[Index].Length)))
      {
        break;
      }
    }

    if (Index >= NumRanges) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Function checks if passed Generic LPC IO Range is already in Gen IO Range list

  @param[in] Address                    Address for generic IO range decoding.
  @param[in] Length                     Length of generic IO range.
  @param[in] GenIoRangeList             Pointer to Generic IO Ranges List

  @retval TRUE                          Passed IO range alredy covered
  @retval FALSE                         Passed IO range NOT covered
**/
STATIC
BOOLEAN
IsRangeInList (
  IN  UINT32                    Address,
  IN  UINT32                    Length,
  IN  PCH_LPC_GEN_IO_RANGE_LIST *GenIoRangeList
  )
{
  UINT32  CurrentBaseAddr;
  UINT32  CurrentLength;
  UINT32  Index;

  for (Index = 0; Index < PCH_LPC_GEN_IO_RANGE_MAX; Index++) {
    CurrentBaseAddr = GenIoRangeList->Range[Index].BaseAddr;
    CurrentLength   = GenIoRangeList->Range[Index].Length;
    if (GenIoRangeList->Range[Index].Enable == 0) {
      continue;
    }

    if ((Address >= CurrentBaseAddr) && ((Address + Length) <= (CurrentBaseAddr + CurrentLength))) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Function checks if passed Generic LPC IO Range overlaps with existing range

  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.
  @param[in] GenIoRangeList             Pointer to Generic IO Ranges List

  @retval TRUE                          Passed LPC IO range overlaps with existing range
  @retval FALSE                         Passed LPC IO range NOT overlaps
**/
STATIC
BOOLEAN
FindOverlappingGenIoRange (
  IN  UINT32                    Address,
  IN  UINT32                    Length,
  IN  PCH_LPC_GEN_IO_RANGE_LIST *GenIoRangeList
  )
{
  UINT32  Index;
  UINT32  CurrentBaseAddr;
  UINT32  CurrentLength;

  for (Index = 0; Index < PCH_LPC_GEN_IO_RANGE_MAX; Index++) {
    CurrentBaseAddr = GenIoRangeList->Range[Index].BaseAddr;
    CurrentLength   = GenIoRangeList->Range[Index].Length;
    if (GenIoRangeList->Range[Index].Enable == 0) {
      continue;
    }

    if ((Address >= CurrentBaseAddr) &&
        (Address <= (CurrentBaseAddr + CurrentLength)))
    {
      return TRUE;
    } else if (((Address + Length) >= CurrentBaseAddr) &&
               ((Address + Length) <= (CurrentBaseAddr + CurrentLength)))
    {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Function look for empty Generic IO range register.
  If found return range index.

  @param[in]      GenIoRangeList        Pointer to Generic IO Ranges List
  @param[in]      ListLength            Length of passed list
  @param[out]     RangeIndex            Generic IO Range Index

  @retval TRUE                          Empty range found
  @retval FALSE                         NOT found empty range
**/
STATIC
BOOLEAN
FindEmptyGenIoRange (
  IN  PCH_LPC_GEN_IO_RANGE_LIST *GenIoRangeList,
  IN  UINT32                    ListLength,
  OUT UINT32                    *RangeIndex
  )
{
  UINT32  Index;

  for (Index = 0; Index < ListLength; Index++) {
    if (GenIoRangeList->Range[Index].Enable == 0) {
      *RangeIndex = Index;
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Get PCH LPC/eSPI and eSPI devices generic IO range list.
  This function returns a list of base address, length, and enable for all LPC/eSPI or eSPI device generic IO range registers.

  @param[in]  TargetId                  Target ID (refer to ESPI_DEVICE_ID)
  @param[out] GenIoRangeList            LPC/eSPI or eSPI CS1# generic IO range registers.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
STATIC
EFI_STATUS
LpcEspiGenIoRangeGetHelper (
  IN  ESPI_DEVICE_ID            TargetId,
  OUT PCH_LPC_GEN_IO_RANGE_LIST *GenIoRangeList
  )
{
  UINT32  Index;
  UINT64  LpcBase;
  UINT32  Data32;
  UINT32  GenIoReg;

  if ((GenIoRangeList == NULL) || (TargetId >= EspiDeviceMax)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  LpcBase = LpcPciCfgBase ();

  switch (TargetId) {
    case EspiDeviceCS1:
      GenIoReg = R_ESPI_CFG_ESPI_CS1GIR1;
      break;
    case EspiDeviceCS2:
      GenIoReg = R_ESPI_CFG_ESPI_GIR1_EXT_2;
      break;
    case EspiDeviceCS3:
      GenIoReg = R_ESPI_CFG_ESPI_GIR1_EXT_3;
      break;
    default:
      GenIoReg = R_ESPI_CFG_ESPI_LGIR1;
      break;
  }

  ZeroMem (GenIoRangeList, sizeof (PCH_LPC_GEN_IO_RANGE_LIST));
  for (Index = 0; Index < PCH_LPC_GEN_IO_RANGE_MAX; Index++) {
    // For eSPI CS# > 0 we have only one range.
    if ((TargetId > EspiDeviceCS0) && (Index > 0)) {
      break;
    }

    Data32 = PciSegmentRead32 (LpcBase + GenIoReg + Index * 4);
    GenIoRangeList->Range[Index].BaseAddr = (Data32 & B_ESPI_CFG_ESPI_LGIR1_ADDR);
    GenIoRangeList->Range[Index].Length   = ((Data32 & B_ESPI_CFG_ESPI_LGIR1_ADDR_MASK) >> 16) + 4;
    GenIoRangeList->Range[Index].Enable   = Data32 & B_ESPI_CFG_ESPI_LGIR1_LDE;
  }

  return EFI_SUCCESS;
}

/**
  Function checks if passed Generic LPC IO Range colliding
  with range alredy defined for other eSPI chiselect (CS)

  @param[in] TargetId                   Target ID (refer to ESPI_DEVICE_ID)
  @param[in] Address                    Address for generic IO range base address.
  @param[in] Length                     Length of generic IO range.

  @retval TRUE                          Passed IO range conflicting
  @retval FALSE                         There is no conflict
**/
STATIC
BOOLEAN
IsRangeColliding (
  IN  ESPI_DEVICE_ID TargetId,
  IN  UINT32         Address,
  IN  UINT32         Length
  )
{
  EFI_STATUS                 Status;
  PCH_LPC_GEN_IO_RANGE_LIST  GenIoRangeList;
  UINT8                      DeviceId;

  for (DeviceId = EspiDeviceCS0; DeviceId < EspiDeviceMax; DeviceId++) {
    if (DeviceId == TargetId) {
      continue;
    }

    Status = LpcEspiGenIoRangeGetHelper (DeviceId, &GenIoRangeList);
    if (!EFI_ERROR (Status)) {
      if (FindOverlappingGenIoRange (Address, Length, &GenIoRangeList) ||
          IsRangeInList (Address, Length, &GenIoRangeList))
      {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  Set PCH LPC/eSPI and eSPI devices generic IO range decoding.

  Steps of programming generic IO range:
  1. Program LPC/eSPI or eSPI device register - Mask, Address, and Enable.
  2. Program LPC/eSPI Generic IO Range in DMI

  @param[in] TargetId                   Target ID (refer to ESPI_DEVICE_ID)
  @param[in] Address                    Address for generic IO range decoding.
  @param[in] Length                     Length of generic IO range.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
  @retval EFI_OUT_OF_RESOURCES          No more generic range available.
  @retval EFI_UNSUPPORTED               DMI configuration is locked,
                                        GenIO range conflicting with other eSPI CS
**/
STATIC
EFI_STATUS
LpcEspiGenIoRangeSetHelper (
  IN  ESPI_DEVICE_ID TargetId,
  IN  UINT32         Address,
  IN  UINT32         Length
  )
{
  EFI_STATUS                 Status;
  PCH_LPC_GEN_IO_RANGE_LIST  GenIoRangeList;
  UINT32                     RangeIndex;
  UINT32                     Data32;
  UINT32                     GenIoReg;
  UINT32                     ListLength;

  //
  // Check if pasesed Address and Length meets all requirements
  //
  if (!IsLpcIoRangeValid (Address, Length) || (TargetId >= EspiDeviceMax)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read current Generic IO configuration
  //
  Status = LpcEspiGenIoRangeGetHelper (TargetId, &GenIoRangeList);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Check if passed Generic IO range is already covered in current configuration
  //
  if (IsRangeInList (Address, Length, &GenIoRangeList)) {
    return EFI_SUCCESS;
  }

  //
  // Check if passed Generic IO range conflicting with other eSPI CS decoding
  //
  if (IsRangeColliding (TargetId, Address, Length)) {
    return EFI_UNSUPPORTED;
  }

  switch (TargetId) {
    case EspiDeviceCS1:
      GenIoReg = R_ESPI_CFG_ESPI_CS1GIR1;
      ListLength = ESPI_CS1_GEN_IO_RANGE_MAX;
      break;
    case EspiDeviceCS2:
      GenIoReg = R_ESPI_CFG_ESPI_GIR1_EXT_2;
      ListLength = ESPI_CS2_GEN_IO_RANGE_MAX;
      break;
    case EspiDeviceCS3:
      GenIoReg = R_ESPI_CFG_ESPI_GIR1_EXT_3;
      ListLength = ESPI_CS3_GEN_IO_RANGE_MAX;
      break;
    default:
      GenIoReg = R_ESPI_CFG_ESPI_LGIR1;
      ListLength = PCH_LPC_GEN_IO_RANGE_MAX;
      break;
  }

  RangeIndex = ListLength;
  //
  // Check if there is an empty Generic IO range register
  //
  if (FindEmptyGenIoRange (&GenIoRangeList, ListLength, &RangeIndex) == FALSE) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Program decoding in IOC and LPC/eSPI registers
  //
  switch (TargetId) {
    case EspiDeviceCS0:
      Status = IocSetLpcGenIoRange (Address, Length, RangeIndex);
      break;
    case EspiDeviceCS1:
      ASSERT (RangeIndex == 0);
      Status = IocSetEspiCs1GenIoRange (Address, Length);
      break;
    case EspiDeviceCS2:
      ASSERT (RangeIndex == 0);
      Status = IocSetEspiCs2GenIoRange (Address, Length);
      break;
    case EspiDeviceCS3:
      ASSERT (RangeIndex == 0);
      Status = IocSetEspiCs3GenIoRange (Address, Length);
      break;
    default:
      Status = EFI_UNSUPPORTED;
      break;
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a - error while enabling IO range decode in IOC - %r", __FUNCTION__, Status));
    return Status;
  }

  //
  // Program LPC/eSPI generic IO range register accordingly.
  //
  Data32  = (UINT32)(((Length - 1) << 16) & B_ESPI_CFG_ESPI_LGIR1_ADDR_MASK);
  Data32 |= (UINT32)Address;
  Data32 |= B_ESPI_CFG_ESPI_LGIR1_LDE;

  //
  // Program LPC/eSPI PCI Offset General IO range in proper register for selected eSPI device
  //
  PciSegmentWrite32 (
    LpcPciCfgBase () + GenIoReg + RangeIndex * 4,
    Data32
    );

  return Status;
}

/**
  Set PCH LPC/eSPI generic IO range.
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
  IN  UINT16 Address,
  IN  UINTN  Length
  )
{
  return LpcEspiGenIoRangeSetHelper (EspiDeviceCS0, (UINT32)Address, (UINT32)Length);
}

/**
  Clear PTL eSPI generic IO range.
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
  IN  UINT16 Address,
  IN  UINTN  Length
  )
{
  PCH_LPC_GEN_IO_RANGE_LIST  LpcGenIoRangeList;
  UINT64                     LpcBase;
  UINTN                      Index;

  SetMem (&LpcGenIoRangeList, sizeof (PCH_LPC_GEN_IO_RANGE_LIST), 0x0);
  PchLpcGenIoRangeGet (&LpcGenIoRangeList);
  for (Index = 0; Index < PCH_LPC_GEN_IO_RANGE_MAX; Index++) {
    if ((LpcGenIoRangeList.Range[Index].BaseAddr == Address) &&
        (LpcGenIoRangeList.Range[Index].Length   == Length))
    {
      //
      // Clear the LPC general IO range register and DMI LPC general IO range register.
      //
      LpcBase = LpcPciCfgBase ();
      PciSegmentWrite32 (LpcBase + R_ESPI_CFG_ESPI_LGIR1 + Index * 4, 0);
      IocClearLpcGenIoRange (Index);
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Set PCH eSPI CS1# generic IO range decoding.

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
  IN  UINT16 Address,
  IN  UINTN  Length
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS1, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiGenIoRangeSetHelper (EspiDeviceCS1, (UINT32)Address, (UINT32)Length);
}

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
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS2, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiGenIoRangeSetHelper (EspiDeviceCS2, (UINT32)Address, (UINT32)Length);
}

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
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS3, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiGenIoRangeSetHelper (EspiDeviceCS3, (UINT32)Address, (UINT32)Length);
}

/**
  Get PCH LPC/eSPI generic IO range list.
  This function returns a list of base address, length, and enable for all LPC/eSPI generic IO range registers.

  @param[out] LpcGenIoRangeList         Return all LPC/eSPI generic IO range register status.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
PchLpcGenIoRangeGet (
  OUT PCH_LPC_GEN_IO_RANGE_LIST *LpcGenIoRangeList
  )
{
  return LpcEspiGenIoRangeGetHelper (EspiDeviceCS0, LpcGenIoRangeList);
}

/**
  Get PCH eSPI CS1# generic IO range list.
  This function returns a list of base address, length, and enable for all eSPI CS1# generic IO range regsiters.

  @param[out] GenIoRangeList            eSPI generic IO range registers.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs1GenIoRangeGet (
  OUT PCH_LPC_GEN_IO_RANGE_LIST *GenIoRangeList
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS1, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiGenIoRangeGetHelper (EspiDeviceCS1, GenIoRangeList);
}

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
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS2, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiGenIoRangeGetHelper (EspiDeviceCS2, GenIoRangeList);
}

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
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS3, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiGenIoRangeGetHelper (EspiDeviceCS3, GenIoRangeList);
}

/**
  Get PCH LPC/eSPI and eSPI devices memory range decoding address.

  @param[in]  TargetId                  Target ID (refer to ESPI_DEVICE_ID)
  @param[out] Address                   Address of LPC/eSPI or eSPI CS1# memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
STATIC
EFI_STATUS
LpcEspiMemRangeGetHelper (
  IN  ESPI_DEVICE_ID TargetId,
  OUT UINT32         *Address
  )
{
  UINT32  GenMemReg;

  if ((Address == NULL) || (TargetId >= EspiDeviceMax)) {
    DEBUG ((DEBUG_ERROR, "%a Error. Invalid pointer or TargetId.\n", __FUNCTION__));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  switch (TargetId) {
    case EspiDeviceCS1:
      GenMemReg = R_ESPI_CFG_ESPI_CS1GMR1;
      break;
    case EspiDeviceCS2:
      GenMemReg = R_ESPI_CFG_ESPI_GMR1_EXT_2;
      break;
    case EspiDeviceCS3:
      GenMemReg = R_ESPI_CFG_ESPI_GMR1_EXT_3;
      break;
    default:
      GenMemReg = R_ESPI_CFG_ESPI_LGMR;
      break;
  }

  *Address = PciSegmentRead32 (LpcPciCfgBase () + GenMemReg) & B_ESPI_CFG_ESPI_LGMR_MADDR;
  return EFI_SUCCESS;
}

/**
  Set PCH LPC/eSPI and eSPI devices memory range decoding.
  This cycle decoding is required to be set on DMI side
  Programming steps:
  1. Program bit [0] to 0b to disable memory decoding first before changing base address.
  2. Program bits [31:16, 0] to [Address, 1].
  3. Program LPC/eSPI Memory Range in DMI

  @param[in] TargetId                   Target ID (refer to ESPI_DEVICE_ID)
  @param[in] Address                    Address for memory for decoding.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address or length passed.
**/
STATIC
EFI_STATUS
LpcEspiMemRangeSetHelper (
  IN  ESPI_DEVICE_ID TargetId,
  IN  UINT32         Address
  )
{
  UINT64          LpcBase;
  EFI_STATUS      Status;
  UINT32          GenMemReg;
  UINT32          MemRangeAddr;
  ESPI_DEVICE_ID  DeviceId;
  BOOLEAN         RangeUsed;

  if (((Address & (~B_ESPI_CFG_ESPI_LGMR_MADDR)) != 0) || (TargetId >= EspiDeviceMax)) {
    DEBUG ((DEBUG_ERROR, "%a Error. Invalid Address: %x or invalid TargetId\n", __FUNCTION__, Address));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  LpcBase = LpcPciCfgBase ();

  MemRangeAddr = ~Address;
  RangeUsed = FALSE;

  for (DeviceId = EspiDeviceCS0; DeviceId < EspiDeviceMax; DeviceId++) {
    if (DeviceId == TargetId) {
      continue;
    }

    Status = LpcEspiMemRangeGetHelper (DeviceId, &MemRangeAddr);
    if (!EFI_ERROR (Status)) {
      if (MemRangeAddr == Address) {
        RangeUsed = TRUE;
      }
    }
  }

  if (RangeUsed) {
    DEBUG ((DEBUG_WARN, "%a - address %04X is already in use.\n", __FUNCTION__, Address));
    return EFI_INVALID_PARAMETER;
  }

  switch (TargetId) {
    case EspiDeviceCS1:
      GenMemReg = R_ESPI_CFG_ESPI_CS1GMR1;
      Status = IocSetEspiCs1MemRange (Address);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }
      break;
    case EspiDeviceCS2:
      GenMemReg = R_ESPI_CFG_ESPI_GMR1_EXT_2;
      Status = IocSetEspiCs2MemRange (Address);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }
      break;
    case EspiDeviceCS3:
      GenMemReg = R_ESPI_CFG_ESPI_GMR1_EXT_3;
      Status = IocSetEspiCs3MemRange (Address);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }
      break;
    default:
      GenMemReg = R_ESPI_CFG_ESPI_LGMR;
      Status = IocSetLpcMemRange (Address);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }
      break;
  }

  //
  // Program bit [0] to 0b to disable memory decoding first before changing base address.
  //
  PciSegmentAnd32 (
    LpcBase + GenMemReg,
    (UINT32) ~B_ESPI_CFG_ESPI_LGMR_LGMRD_EN
    );
  //
  // Program bits [31:16, 0] to [Address, 1].
  //
  PciSegmentWrite32 (
    LpcBase + GenMemReg,
    (Address | B_ESPI_CFG_ESPI_LGMR_LGMRD_EN)
    );

  return Status;
}

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
  IN  UINT32 Address
  )
{
  return LpcEspiMemRangeSetHelper (EspiDeviceCS0, Address);
}

/**
  Set PCH eSPI CS1# memory range decoding.
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
  IN  UINT32 Address
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS1, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiMemRangeSetHelper (EspiDeviceCS1, Address);
}

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
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS2, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiMemRangeSetHelper (EspiDeviceCS2, Address);
}

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
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS3, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiMemRangeSetHelper (EspiDeviceCS3, Address);
}

/**
  Get PCH LPC/eSPI memory range decoding address.

  @param[out] Address                   Address of LPC/eSPI memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
**/
EFI_STATUS
PchLpcMemRangeGet (
  OUT UINT32 *Address
  )
{
  return LpcEspiMemRangeGetHelper (EspiDeviceCS0, Address);
}

/**
  Get PCH eSPI CS1# memory range decoding address.

  @param[out] Address                   Address of eSPI CS1# memory decoding base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid base address passed.
  @retval EFI_UNSUPPORTED               eSPI secondary target not supported
**/
EFI_STATUS
PchEspiCs1MemRangeGet (
  OUT UINT32 *Address
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS1, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiMemRangeGetHelper (EspiDeviceCS1, Address);
}

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
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS2, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiMemRangeGetHelper (EspiDeviceCS2, Address);
}

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
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS3, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiMemRangeGetHelper (EspiDeviceCS3, Address);
}

/**
  Set PCH LPC/eSPI IO decode ranges.
  Program LPC/eSPI I/O Decode Ranges in DMI to the same value programmed in LPC/eSPI PCI offset 80h.
  Please check EDS for detail of LPC/eSPI IO decode ranges bit definition.
  Bit  12: FDD range
  Bit 9:8: LPT range
  Bit 6:4: ComB range
  Bit 2:0: ComA range

  @param[in] LpcIoDecodeRanges          LPC/eSPI IO decode ranges bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMIC.SRL is set.
**/
EFI_STATUS
PchLpcIoDecodeRangesSet (
  IN  UINT16 LpcIoDecodeRanges
  )
{
  UINT64      LpcBaseAddr;
  EFI_STATUS  Status;

  //
  // Note: Inside this function, don't use debug print since it's could used before debug print ready.
  //

  LpcBaseAddr = LpcPciCfgBase ();

  //
  // check if setting is identical
  //
  if (LpcIoDecodeRanges == PciSegmentRead16 (LpcBaseAddr + R_ESPI_CFG_ESPI_IOD_IOE)) {
    return EFI_SUCCESS;
  }

  Status = IocSetLpcIoDecodeRanges (LpcIoDecodeRanges);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // program LPC/eSPI PCI offset 80h.
  //
  PciSegmentWrite16 (LpcBaseAddr + R_ESPI_CFG_ESPI_IOD_IOE, LpcIoDecodeRanges);

  return Status;
}

/**
  Set PCH LPC/eSPI and eSPI devices IO enable decoding.
  Setup I/O Enables in DMI to the same value program in LPC/eSPI or eSPI devices.
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in LPC/eSPI PCI offset 82h[13:10] or A0h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of Lpc/eSPI IO decode ranges bit definition.

  @param[in] TargetId                   Target ID (refer to ESPI_DEVICE_ID)
  @param[in] IoEnableDecoding           LPC/eSPI IO enable decoding bit settings.
  @param[in] EspiPcrAccess              eSPI Sideband register access protocol

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMI configuration is locked
**/
EFI_STATUS
LpcEspiIoEnableDecodingSetHelper (
  IN  ESPI_DEVICE_ID  TargetId,
  IN  UINT16          IoEnableDecoding,
  IN  REGISTER_ACCESS *EspiPcrAccess
  )
{
  UINT64      LpcBaseAddr;
  EFI_STATUS  Status;
  UINT16      Cs0IoEnableDecodingOrg;
  UINT16      Cs1IoEnableDecodingOrg;
  UINT16      Cs2IoEnableDecodingOrg;
  UINT16      Cs3IoEnableDecodingOrg;
  UINT16      IoEnableDecodingMerged;

  LpcBaseAddr = LpcPciCfgBase ();

  Cs0IoEnableDecodingOrg = PciSegmentRead16 (LpcBaseAddr + R_ESPI_CFG_ESPI_IOE);
  Cs1IoEnableDecodingOrg = 0;
  Cs2IoEnableDecodingOrg = 0;
  Cs3IoEnableDecodingOrg = 0;

  if (IsEspiDeviceSupported (EspiDeviceCS1, EspiPcrAccess)) {
    Cs1IoEnableDecodingOrg = PciSegmentRead16 (LpcBaseAddr + R_ESPI_CFG_ESPI_CS1IORE);
  }

  if (IsEspiDeviceSupported (EspiDeviceCS2, EspiPcrAccess)) {
    Cs2IoEnableDecodingOrg = PciSegmentRead16 (LpcBaseAddr + R_ESPI_CFG_ESPI_IORE_EXT_2);
  }

  if (IsEspiDeviceSupported (EspiDeviceCS3, EspiPcrAccess)) {
    Cs3IoEnableDecodingOrg = PciSegmentRead16 (LpcBaseAddr + R_ESPI_CFG_ESPI_IORE_EXT_3);
  }

  switch (TargetId) {
    case EspiDeviceCS1:
      if (IoEnableDecoding == Cs1IoEnableDecodingOrg) {
        return EFI_SUCCESS;
      } else {
        IoEnableDecodingMerged = (Cs0IoEnableDecodingOrg | IoEnableDecoding);
      }
      break;
    case EspiDeviceCS2:
      if (IoEnableDecoding == Cs2IoEnableDecodingOrg) {
        return EFI_SUCCESS;
      } else {
        IoEnableDecodingMerged = (Cs0IoEnableDecodingOrg | IoEnableDecoding);
      }
      break;
    case EspiDeviceCS3:
      if (IoEnableDecoding == Cs3IoEnableDecodingOrg) {
        return EFI_SUCCESS;
      } else {
        IoEnableDecodingMerged = (Cs0IoEnableDecodingOrg | IoEnableDecoding);
      }
      break;
    default:
      IoEnableDecodingMerged = (Cs1IoEnableDecodingOrg | Cs2IoEnableDecodingOrg | Cs3IoEnableDecodingOrg | IoEnableDecoding);
      if (IoEnableDecodingMerged == Cs0IoEnableDecodingOrg) {
        return EFI_SUCCESS;
      }
      break;
  }

  Status = IocSetLpcIoEnable (IoEnableDecodingMerged);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // program PCI offset 82h for LPC/eSPI.
  //
  PciSegmentWrite16 (LpcBaseAddr + R_ESPI_CFG_ESPI_IOE, IoEnableDecodingMerged);

  //
  // program target eSPI device
  //
  switch (TargetId) {
    case EspiDeviceCS1:
      PciSegmentWrite16 (LpcBaseAddr + R_ESPI_CFG_ESPI_CS1IORE, IoEnableDecoding);
      break;
    case EspiDeviceCS2:
      PciSegmentWrite16 (LpcBaseAddr + R_ESPI_CFG_ESPI_IORE_EXT_2, IoEnableDecoding);
      break;
    case EspiDeviceCS3:
      PciSegmentWrite16 (LpcBaseAddr + R_ESPI_CFG_ESPI_IORE_EXT_3, IoEnableDecoding);
      break;
    default:
      break;
  }

  return Status;
}

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
  IN  UINT16 LpcIoEnableDecoding
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  return LpcEspiIoEnableDecodingSetHelper (EspiDeviceCS0, LpcIoEnableDecoding, (REGISTER_ACCESS *) &EspiPcrAccess);
}

/**
  Set PCH eSPI CS1# IO enable decoding.
  Setup I/O Enables in DMI to the same value program in eSPI PCI offset A0h (eSPI CS1#).
  Note: Bit[15:10] of the source decode register is Read-Only. The IO range indicated by the Enables field
  in eSPI PCI offset A0h[13:10] is always forwarded by DMI to subtractive agent for handling.
  Please check EDS for detail of eSPI IO decode ranges bit definition.

  @param[in] IoEnableDecoding           eSPI IO enable decoding bit settings.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_UNSUPPORTED               DMI configuration is locked
**/
EFI_STATUS
PchEspiCs1IoEnableDecodingSet (
  IN  UINT16 IoEnableDecoding
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS1, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiIoEnableDecodingSetHelper (EspiDeviceCS1, IoEnableDecoding, (REGISTER_ACCESS *) &EspiPcrAccess);
}

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
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS2, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiIoEnableDecodingSetHelper (EspiDeviceCS2, IoEnableDecoding, (REGISTER_ACCESS *) &EspiPcrAccess);
}

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
  )
{
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  EspiPcrAccess;

  EspiGetSidebandRegAccessProtocol (&P2SbController, &EspiPcrAccess);

  if (!IsEspiDeviceSupported (EspiDeviceCS3, (REGISTER_ACCESS *)&EspiPcrAccess)) {
    return EFI_UNSUPPORTED;
  }

  return LpcEspiIoEnableDecodingSetHelper (EspiDeviceCS3, IoEnableDecoding, (REGISTER_ACCESS *) &EspiPcrAccess);
}

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
  IN  UINT16 AndValue,
  IN  UINT16 OrValue
  )
{
  EFI_STATUS  Status;
  UINT64      LpcBaseAddr;
  UINT16      IoEnableDecoding;

  LpcBaseAddr = LpcPciCfgBase ();

  IoEnableDecoding = (PciSegmentRead16 (LpcBaseAddr + R_ESPI_CFG_ESPI_IOE) & AndValue) | OrValue;

  Status = IocSetLpcIoEnable (IoEnableDecoding);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // program PCI offset 82h for LPC/eSPI.
  //
  PciSegmentWrite16 (LpcBaseAddr + R_ESPI_CFG_ESPI_IOE, IoEnableDecoding);
  return EFI_SUCCESS;
}

/**
  Set PCH IO port 80h cycle decoding to PCIE root port.
  System BIOS is likely to do this very soon after reset before PCI bus enumeration.
  This cycle decoding is allowed to set when DMI is unlocked

  @param[in] RpNumber                PCIE root port physical number.

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
PchIoPort80DecodeSet (
  IN  UINTN RpNumber
  )
{
  EFI_STATUS  Status;
  IOC_ROOT_PORT_DEST_ID IocRpDestinationId;
  PSF_PORT_DEST_ID      PsfRpDestinationId;

  PsfRpDestinationId = PtlPcdPsfPcieDestinationId ((UINT32)RpNumber);

  IocRpDestinationId.PsfId       = (UINT8)PsfRpDestinationId.Fields.PsfId;
  IocRpDestinationId.PortGroupId = (UINT8)PsfRpDestinationId.Fields.PortGroupId;
  IocRpDestinationId.PortId      = (UINT8)PsfRpDestinationId.Fields.PortId;
  IocRpDestinationId.ChannelId   = (UINT8)PsfRpDestinationId.Fields.ChannelId;

  Status = IocSetIoPort80Decode (IocRpDestinationId);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Get IO APIC regsiters base address.

  @param[out] IoApicBase                Buffer of IO APIC regsiter address

  @retval EFI_SUCCESS                   Successfully completed.
**/
EFI_STATUS
PchIoApicBaseGet (
  OUT UINT32 *IoApicBase
  )
{
  *IoApicBase = PcdGet32 (PcdSiIoApicBaseAddress);
  return EFI_SUCCESS;
}

/**
  Get HPET base address.

  @param[out] HpetBase                  Buffer of HPET base address

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid offset passed.
**/
EFI_STATUS
PchHpetBaseGet (
  OUT UINTN *HpetBase
  )
{
  if (HpetBase == NULL) {
    DEBUG ((DEBUG_ERROR, "PchHpetBaseGet Error. Invalid pointer.\n"));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  *HpetBase = PcdGet32 (PcdSiHpetBaseAddress);
  return EFI_SUCCESS;
}
