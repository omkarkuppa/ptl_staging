/** @file
  Memory Mapped IO (MMIO) space access lib - produces MMIO access protocol

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/MmioAccessLib.h>

/**
  Performs an 8-bit read access to MMIO register

  @param[in] This     Pointer to the REGISTER_ACCESS
  @param[in] Offset   Offset of MMIO register

  @return The 8-bit MMIO register value specified by offset
**/
UINT8
MmioAccessRead8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  )
{
  MMIO_ACCESS   *Private;

  Private = (MMIO_ACCESS *) This;
  return MmioRead8 ((UINTN)Private->MmioBar + Offset);
}

/**
  Writes an 8-bit value to the register

  @param[in] This    Pointer to the REGISTER_ACCESS
  @param[in] Offset  Offset of MMIO register
  @param[in] Value   Value to write to register

  @return The parameter of Value
**/
UINT8
MmioAccessWrite8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            Value
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioWrite8 ((UINTN)Private->MmioBar + Offset, Value);
}

/**
  Performs an 8-bit bitwise or operation on the register

  @param[in] This    Pointer to the REGISTER_ACCESS
  @param[in] Offset  Offset of MMIO register
  @param[in] OrData  Data with which register should be OR-ed

  @return The 8-bit register value written to register
**/
UINT8
MmioAccessOr8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            OrData
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioOr8 ((UINTN)Private->MmioBar + Offset, OrData);
}

/**
  Performs an 8-bit bitwise and operation on the register

  @param[in] This     Pointer to the REGISTER_ACCESS
  @param[in] Offset   Offset of MMIO register
  @param[in] AndData  Data with which register should be AND-ed

  @return The 8-bit register value written to register
**/
UINT8
MmioAccessAnd8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            AndData
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioAnd8 ((UINTN)Private->MmioBar + Offset, AndData);
}

/**
  Performs an 8-bit bitwsie and then bitwise or operations on the register

  @param[in] This     Pointer to the REGISTER_ACCESS
  @param[in] Offset   Offset of MMIO register
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 8-bit register value written to register
**/
UINT8
MmioAccessAndThenOr8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            AndData,
  IN UINT8            OrData
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioAndThenOr8 ((UINTN)Private->MmioBar + Offset, AndData, OrData);
}

/**
  Performs a 16-bit read access to MMIO register

  @param[in] This     Pointer to the REGISTER_ACCESS
  @param[in] Offset   Offset of MMIO register

  @return The 16-bit register value specified by offset
**/
UINT16
MmioAccessRead16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioRead16 ((UINTN)Private->MmioBar + Offset);
}

/**
  Writes a 16-bit value to the register

  @param[in] This    Pointer to the REGISTER_ACCESS
  @param[in] Offset  Offset of MMIO register
  @param[in] Value   Value to write to register

  @return The 16-bit register value written to register
**/
UINT16
MmioAccessWrite16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           Value
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioWrite16 ((UINTN)Private->MmioBar + Offset, Value);
}

/**
  Performs a 16-bit bitwise or operation on the register

  @param[in] This    Pointer to the REGISTER_ACCESS
  @param[in] Offset  Offset of MMIO register
  @param[in] OrData  Data with which register should be OR-ed

  @return The 16-bit register value written to register
**/
UINT16
MmioAccessOr16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           OrData
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioOr16 ((UINTN)Private->MmioBar + Offset, OrData);
}

/**
  Performs a 16-bit bitwise and operation on the register

  @param[in] This     Pointer to the REGISTER_ACCESS
  @param[in] Offset   Offset of MMIO register
  @param[in] AndData  Data with which register should be AND-ed

  @return The 16-bit register value written to register
**/
UINT16
MmioAccessAnd16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           AndData
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioAnd16 ((UINTN)Private->MmioBar + Offset, AndData);
}

/**
  Performs a 16-bit bitwise and then bitwise or operations on the register

  @param[in] This     Pointer to the REGISTER_ACCESS
  @param[in] Offset   Offset of MMIO register
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 16-bit register value written to register
**/
UINT16
MmioAccessAndThenOr16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           AndData,
  IN UINT16           OrData
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioAndThenOr16 ((UINTN)Private->MmioBar + Offset, AndData, OrData);
}

/**
  Performs a 32-bit read access to the register

  @param[in] This     Pointer to the REGISTER_ACCESS
  @param[in] Offset   Offset of MMIO register

  @return The 32-bit register value specified by offset
**/
UINT32
MmioAccessRead32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioRead32 ((UINTN)Private->MmioBar + Offset);
}

/**
  Writes a 32-bit value to the register

  @param[in] This    Pointer to the REGISTER_ACCESS
  @param[in] Offset  Offset of MMIO register
  @param[in] Value   Value to write to register

  @return The 32-bit register value written to register
**/
UINT32
MmioAccessWrite32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           Value
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioWrite32 ((UINTN)Private->MmioBar + Offset, Value);
}

/**
  Performs a 32-bit bitwise or operation on the register

  @param[in] This    Pointer to the REGISTER_ACCESS
  @param[in] Offset  Offset of MMIO register
  @param[in] OrData  Data with which register should be OR-ed

  @return The 32-bit register value written to register
**/
UINT32
MmioAccessOr32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           OrData
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioOr32 ((UINTN)Private->MmioBar + Offset, OrData);
}

/**
  Performs a 32-bit bitwise and operation on the register

  @param[in] This     Pointer to the REGISTER_ACCESS
  @param[in] Offset   Offset of MMIO register
  @param[in] AndData  Data with which register should be AND-ed

  @return The 32-bit register value written to register
**/
UINT32
MmioAccessAnd32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           AndData
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioAnd32 ((UINTN)Private->MmioBar + Offset, AndData);
}

/**
  Performs a 32-bit bitwise and then bitwise or operation on the register

  @param[in] This     Pointer to the REGISTER_ACCESS
  @param[in] Offset   Offset of MMIO register
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 32-bit register value written to register
**/
UINT32
MmioAccessAndThenOr32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           AndData,
  IN UINT32           OrData
  )
{
  MMIO_ACCESS   *Private;
  Private = (MMIO_ACCESS *) This;

  return MmioAndThenOr32 ((UINTN)Private->MmioBar + Offset, AndData, OrData);
}

/**
  Initializes MMIO_ACCESS structure

  @param[in]    MmioBar           MMIO base address
  @param[out]   MmioSpaceAccess   Structure to be initialized

  @retval TRUE   Initialization successful
  @retval FALSE  Failed to initialize
**/
BOOLEAN
MmioAccessInit (
  UINT64        MmioBar,
  MMIO_ACCESS   *MmioSpaceAccess
  )
{
  MmioSpaceAccess->RegAccess.Read8        = MmioAccessRead8;
  MmioSpaceAccess->RegAccess.Write8       = MmioAccessWrite8;
  MmioSpaceAccess->RegAccess.Or8          = MmioAccessOr8;
  MmioSpaceAccess->RegAccess.And8         = MmioAccessAnd8;
  MmioSpaceAccess->RegAccess.AndThenOr8   = MmioAccessAndThenOr8;
  MmioSpaceAccess->RegAccess.Read16       = MmioAccessRead16;
  MmioSpaceAccess->RegAccess.Write16      = MmioAccessWrite16;
  MmioSpaceAccess->RegAccess.Or16         = MmioAccessOr16;
  MmioSpaceAccess->RegAccess.And16        = MmioAccessAnd16;
  MmioSpaceAccess->RegAccess.AndThenOr16  = MmioAccessAndThenOr16;
  MmioSpaceAccess->RegAccess.Read32       = MmioAccessRead32;
  MmioSpaceAccess->RegAccess.Write32      = MmioAccessWrite32;
  MmioSpaceAccess->RegAccess.Or32         = MmioAccessOr32;
  MmioSpaceAccess->RegAccess.And32        = MmioAccessAnd32;
  MmioSpaceAccess->RegAccess.AndThenOr32  = MmioAccessAndThenOr32;

  MmioSpaceAccess->MmioBar = MmioBar;

  return TRUE;
}