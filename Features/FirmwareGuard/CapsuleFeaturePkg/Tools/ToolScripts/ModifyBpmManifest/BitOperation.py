## @file
#  File to process the bitwise logic.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2022 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
# @par Specification
##
class BitOperation (object):
    def __init__ (self, Value):
        self.__Value = int (Value, 0)

    @property
    def Value (self):
        """ To get the current value that after bit operation.

        Args:
            None.

        Returns:
            Value [int]: The value after bit operation.

        Raises:
            None.
        """
        return self.__Value

    def SetBit (self, Bit):
        """ Toggle the specific bit to 1.

        Args:
            Bit [int]: The bit position to toggle to 1.

        Returns:
            None

        Raises:
            None.
        """
        print (f"Do the Bit Operation on BIT {Bit}")
        print (f"Before Set Bit Operation: {self.__Value}")
        self.__Value = (self.__Value) | (1 << Bit)
        print (f"After Set Bit Operation: {self.__Value}")

    def ClearBit (self, Bit):
        """ Clear the specific bit to 0.

        Args:
            Bit [int]: The bit position to toggle to 0.

        Returns:
            None

        Raises:
            None.
        """
        print (f"Do the Bit Operation on BIT {Bit}")
        print (f"Before Clear Bit Operation: {self.__Value}")
        self.__Value = (self.__Value) & (~(1 << Bit))
        print (f"After Clear Bit Operation: {self.__Value}")
