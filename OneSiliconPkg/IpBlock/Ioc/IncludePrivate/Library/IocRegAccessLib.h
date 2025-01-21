/** @file
  Header file for IOC Register Access Lib.

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

#ifndef __IOC_REG_ACCESS_LIB_H__
#define __IOC_REG_ACCESS_LIB_H__

/**
  Gets the base address.

  This function retrieves the Base Address.

  @return The base address.

**/
VOID*
GetIocRegAccessBaseAddress (
  VOID
  );

/**
  Reads an 8-bit register.

  Reads and returns the 8-bit register specified by Address.

  @param  Address The address of the Register.

  @return The read value from the register.

**/
UINT8
EFIAPI
IocRegAccessRead8 (
  IN      UINTN                     Address
  );

/**
  Writes an 8-bit register.

  Writes the 8-bit register specified by Address with the
  value specified by Value. Value is returned.


  @param  Address The address of the Register.
  @param  Value   The value to write.

  @return The value written to the register.

**/
UINT8
EFIAPI
IocRegAccessWrite8 (
  IN      UINTN                     Address,
  IN      UINT8                     Value
  );

/**
  Performs a bitwise OR of an 8-bit register with an 8-bit value.

  Reads the 8-bit register specified by Address, performs a
  bitwise OR between the read result and the value specified by
  OrData, and writes the result to the 8-bit register
  specified by Address. The value written to the register is
  returned.


  @param  Address The address of the Register.
  @param  OrData  The value to OR with the register.

  @return The value written back to the register.

**/
UINT8
EFIAPI
IocRegAccessOr8 (
  IN      UINTN                     Address,
  IN      UINT8                     OrData
  );

/**
  Performs a bitwise AND of an 8-bit register with an 8-bit
  value.

  Reads the 8-bit register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData, and
  writes the result to the 8-bit register specified by
  Address. The value written to the register is returned.


  @param  Address The address of the Register.
  @param  AndData The value to AND with the register.

  @return The value written back to the register.

**/
UINT8
EFIAPI
IocRegAccessAnd8 (
  IN      UINTN                     Address,
  IN      UINT8                     AndData
  );

/**
  Performs a bitwise AND of an 8-bit register with an 8-bit
  value, followed a  bitwise OR with another 8-bit value.

  Reads the 8-bit register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData,
  performs a bitwise OR between the result of the AND operation and
  the value specified by OrData, and writes the result to the 8-bit
  register specified by Address. The value written to the
  register is returned.


  @param  Address The address of the Register.
  @param  AndData The value to AND with the register.
  @param  OrData  The value to OR with the result of the AND operation.

  @return The value written back to the register.

**/
UINT8
EFIAPI
IocRegAccessAndThenOr8 (
  IN      UINTN                     Address,
  IN      UINT8                     AndData,
  IN      UINT8                     OrData
  );

/**
  Reads an 16-bit register.

  Reads and returns the 16-bit register specified by Address.

  @param  Address The address of the Register.

  @return The read value from the register.

**/
UINT16
EFIAPI
IocRegAccessRead16 (
  IN      UINTN                     Address
  );

/**
  Writes an 16-bit register.

  Writes the 16-bit register specified by Address with the
  value specified by Value. Value is returned.


  @param  Address The address of the Register.
  @param  Value   The value to write.

  @return The value written to the register.

**/
UINT16
EFIAPI
IocRegAccessWrite16 (
  IN      UINTN                     Address,
  IN      UINT16                    Value
  );

/**
  Performs a bitwise OR of an 16-bit register with an 16-bit value.

  Reads the 16-bit register specified by Address, performs a
  bitwise OR between the read result and the value specified by
  OrData, and writes the result to the 16-bit register
  specified by Address. The value written to the register is
  returned.


  @param  Address The address of the Register.
  @param  OrData  The value to OR with the register.

  @return The value written back to the register.

**/
UINT16
EFIAPI
IocRegAccessOr16 (
  IN      UINTN                     Address,
  IN      UINT16                    OrData
  );

/**
  Performs a bitwise AND of an 16-bit register with an 16-bit
  value.

  Reads the 16-bit register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData, and
  writes the result to the 16-bit register specified by
  Address. The value written to the register is returned.


  @param  Address The address of the Register.
  @param  AndData The value to AND with the register.

  @return The value written back to the register.

**/
UINT16
EFIAPI
IocRegAccessAnd16 (
  IN      UINTN                     Address,
  IN      UINT16                    AndData
  );

/**
  Performs a bitwise AND of an 16-bit register with an 16-bit
  value, followed a  bitwise OR with another 16-bit value.

  Reads the 16-bit register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData,
  performs a bitwise OR between the result of the AND operation and
  the value specified by OrData, and writes the result to the 16-bit
  register specified by Address. The value written to the
  register is returned.


  @param  Address The address of the Register.
  @param  AndData The value to AND with the register.
  @param  OrData  The value to OR with the result of the AND operation.

  @return The value written back to the register.

**/
UINT16
EFIAPI
IocRegAccessAndThenOr16 (
  IN      UINTN                     Address,
  IN      UINT16                    AndData,
  IN      UINT16                    OrData
  );

/**
  Reads an 32-bit register.

  Reads and returns the 32-bit register specified by Address.

  @param  Address The address of the Register.

  @return The read value from the register.

**/
UINT32
EFIAPI
IocRegAccessRead32 (
  IN      UINTN                     Address
  );

/**
  Writes an 32-bit register.

  Writes the 32-bit register specified by Address with the
  value specified by Value. Value is returned.


  @param  Address The address of the Register.
  @param  Value   The value to write.

  @return The value written to the register.

**/
UINT32
EFIAPI
IocRegAccessWrite32 (
  IN      UINTN                     Address,
  IN      UINT32                    Value
  );

/**
  Performs a bitwise OR of an 32-bit register with an 32-bit value.

  Reads the 32-bit register specified by Address, performs a
  bitwise OR between the read result and the value specified by
  OrData, and writes the result to the 32-bit register
  specified by Address. The value written to the register is
  returned.


  @param  Address The address of the Register.
  @param  OrData  The value to OR with the register.

  @return The value written back to the register.

**/
UINT32
EFIAPI
IocRegAccessOr32 (
  IN      UINTN                     Address,
  IN      UINT32                    OrData
  );

/**
  Performs a bitwise AND of an 32-bit register with an 32-bit
  value.

  Reads the 32-bit register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData, and
  writes the result to the 32-bit register specified by
  Address. The value written to the register is returned.


  @param  Address The address of the Register.
  @param  AndData The value to AND with the register.

  @return The value written back to the register.

**/
UINT32
EFIAPI
IocRegAccessAnd32 (
  IN      UINTN                     Address,
  IN      UINT32                    AndData
  );

/**
  Performs a bitwise AND of an 32-bit register with an 32-bit
  value, followed a  bitwise OR with another 32-bit value.

  Reads the 32-bit register specified by Address, performs a
  bitwise AND between the read result and the value specified by AndData,
  performs a bitwise OR between the result of the AND operation and
  the value specified by OrData, and writes the result to the 32-bit
  register specified by Address. The value written to the
  register is returned.


  @param  Address The address of the Register.
  @param  AndData The value to AND with the register.
  @param  OrData  The value to OR with the result of the AND operation.

  @return The value written back to the register.

**/
UINT32
EFIAPI
IocRegAccessAndThenOr32 (
  IN      UINTN                     Address,
  IN      UINT32                    AndData,
  IN      UINT32                    OrData
  );

#endif
