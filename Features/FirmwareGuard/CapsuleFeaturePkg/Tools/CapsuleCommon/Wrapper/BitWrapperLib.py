## @file
#  Wrapper to manipulate the bitwise logic operation.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
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
#  @par Specification
#
##

from typing import List, Union

from CapsuleCommon.Base.ConvertLib import *
from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Base.PaddingLib import *
from CapsuleCommon.Format.BinaryLib import *
from CapsuleCommon.Format.HexadecimalLib import *

class Bit (object):
    def __init__ (self, Value: Union[str, int]) -> None:
        """ Class used to do the basic bitwise operation.

        Args:
            Value (Union[str, int]):
                The value to be doing the bitwise operation.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Source: int = ToInt (Value)
        self.__Target: int = ToInt (Value)

    @property
    def Source (self) -> int:
        """ Get the input value before doing the bitwise logic. (Origin)

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The int value before doing the bitwise logic.
        """
        return self.__Source

    @property
    def Target (self) -> int:
        """ Get the input value after doing the bitwise logic. (Target)

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The int value after doing the bitwise logic.
        """
        return self.__Target

    @property
    def SourceBin (self) -> str:
        """ Get the input value before doing the bitwise logic. (Origin)

        Note:
            To get the BIN format.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The str value before doing the bitwise logic.
                To get the BIN format.
        """
        return FormatBin (self.Source)

    @property
    def TargetBin (self) -> str:
        """ Get the input value after doing the bitwise logic. (Target)

        Note:
            To get the BIN format.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The str value after doing the bitwise logic.
                To get the BIN format.
        """
        return FormatBin (self.Target)

    @property
    def SourceHex (self) -> str:
        """ Get the input value before doing the bitwise logic. (Origin)

        Note:
            To get the HEX format.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The str value before doing the bitwise logic.
                To get the HEX format.
        """
        return FormatHex (self.Source)

    @property
    def TargetHex (self) -> str:
        """ Get the input value before doing the bitwise logic. (Target)

        Note:
            To get the HEX format.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The str value before doing the bitwise logic.
                To get the HEX format.
        """
        return FormatHex (self.Target)

    def __GetBitOperatePos (self, Input: int) -> List[int]:
        """ Get the bit operation position number.

        Note:
            The input should be int and the bit value is 1 to return.

        Args:
            Input (int):
                The mask value to be do the bitwise operation.

        Raises:
            TypeError:
                The input value is not int type.
            ValueError:
                The value is not 0 or 1.

        Returns:
            List[int]:
                List of bit number that be set to 1.
        """
        Result  : list = list ()
        InputStr: str  = str ()
        BitNum  : int  = 0

        if not isinstance (Input, int):
            raise TypeError (f'The input value should be int type.')

        #
        # Do the string reverse after getting the BIN value
        #
        InputStr = FormatBin (Input, IsPrefix = False)[::-1]

        for BitStr in InputStr:
            if BitStr == '0':
                pass
            elif BitStr == '1':
                Result.append (BitNum)
            else:
                raise ValueError (f'Should be 0 or 1 but get {BitStr}')

            BitNum += 1

        return Result

    def SetBit (
        self,
        *Bit: Union[int, str],
        ) -> None:
        """ Toggle the specific bit to 1.

        Note:
            Get the mask value and do OR operation.

        Args:
            *Bit (Union[int, str]):
                The mask value to toggle to 1.

        Raises:
            ValueError:
                The assigned bit is negative value.

        Returns
            None.
        """
        SrcStr : str = str ()
        DesStr : str = str ()
        MaskStr: str = str ()
        Mask   : int = 0x0000_0000_0000_0000

        for Value in Bit:
            BitValue = ToInt (Value)

            if BitValue < 0:
                raise ValueError (f'Assigned bit should >= 0')

            Mask = Mask | BitValue

        SrcStr = FormatBin (
                   self.__Target,
                   IsPrefix  = False,
                   IsPadding = True,
                   )

        MaskStr = FormatBin (
                    Mask,
                    IsPrefix  = False,
                    IsPadding = True,
                    )

        self.__Target = (self.__Target) | Mask

        DesStr = FormatBin (
                   self.__Target,
                   IsPrefix = False,
                   IsPadding = True,
                   )

        DEBUG (DEBUG_TRACE, f'Operate on BIT {self.__GetBitOperatePos (Mask)}')
        DEBUG (DEBUG_TRACE, f'Before Set Bit: {SrcStr}')
        DEBUG (DEBUG_TRACE, f'Mask Value    : {MaskStr}')
        DEBUG (DEBUG_TRACE, f'After Set Bit : {DesStr}')

    def ClearBit (
        self,
        *Bit: Union[int, str],
        ) -> None:
        """ Toggle the specific bit to 0.

        Note:
            Get the mask value do NOT first then do AND operation.

        Args:
            *Bit (Union[int, str]):
                The mask value to toggle to 0.

        Raises:
            ValueError:
                The assigned bit is negative value.

        Returns
            None.
        """
        SrcStr : str = str ()
        DesStr : str = str ()
        MaskStr: str = str ()
        Mask   : int = 0x0000_0000_0000_0000

        for Value in Bit:
            BitValue = ToInt (Value)

            if BitValue < 0:
                raise ValueError (f'Assigned bit should >= 0')

            Mask = Mask | BitValue

        SrcStr = FormatBin (
                   self.__Target,
                   IsPrefix  = False,
                   IsPadding = True,
                   )

        MaskStr = FormatBin (
                    Mask,
                    IsPrefix  = False,
                    IsPadding = True,
                    )

        self.__Target = (self.__Target) & (~Mask)

        DesStr = FormatBin (
                   self.__Target,
                   IsPrefix = False,
                   IsPadding = True,
                   )

        DEBUG (DEBUG_TRACE, f'Operate on BIT {self.__GetBitOperatePos (Mask)}')
        DEBUG (DEBUG_TRACE, f'Before Clear Bit: {SrcStr}')
        DEBUG (DEBUG_TRACE, f'Mask Value      : {MaskStr}')
        DEBUG (DEBUG_TRACE, f'After Clear Bit : {DesStr}')
