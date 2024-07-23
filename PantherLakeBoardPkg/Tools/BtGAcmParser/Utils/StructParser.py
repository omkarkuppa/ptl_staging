## @file
#  StructParser library to parse the C struct.
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
import struct
import sys
from .BaseLib import *

def GetStructAndByteSize (FormatDataDict):
    """ To get the structure format for struct unpack needed and byte size.

    Args:
        FormatDataDict [dict]: The format should be dict of dict.
                               { 'Guid1': {'Type': 'I'     , 'Size': 4} }

    Returns:
        Struct   [str]: The struct format strings used for unpacked.
        ByteSize [int]: The size of this struct in byte.

    Raises:
        None.
    """
    Struct   = '<'      # little-endian
    ByteSize = 0
    for Key, Value in FormatDataDict.items ():
        Struct += Value['Type']
        ByteSize += Value['Size']
    return Struct, ByteSize

def ByteReverse (Value):
    """ To reverse the hexadecimal value string with byte size. (Return value without "0x" prefix.)

    Args:
        Value [str]: Hexadecimal value string to be reversed.

    Returns:
        ReverseByte [str]: The hexadecimal value after doing the byte reversed.

    Raises:
        None.
    """
    InputValue    = RemoveHexPrefix (Value)
    InputStrLen   = len (Value)
    InputByteList = list ()
    ReverseByte   = ''

    if InputStrLen % 2 != 0:
        InputValue   = ZeroPadding (InputValue, InputStrLen + 1)
        InputStrLen += 1

    for Idx in range (int (InputStrLen / 2)):
        StartIdx = (Idx + 0) * 2
        EndIdx = (Idx + 1) * 2
        InputByteList.append (InputValue[StartIdx:EndIdx])

    InputByteList.reverse ()
    ReverseByte = ReverseByte.join (InputByteList)

    return ReverseByte

class StructParser (object):
    def __init__ (self, Buffer, Struct, FormatDataDict):
        self.__Buffer         = Buffer
        self.__Struct         = Struct
        self.__FormatDataDict = FormatDataDict

        self.__StructData = struct.unpack (self.__Struct, self.__Buffer)
        self.__Data       = self.__GetHeaderData ()

    def __ConvertToHexValue (self, Value, Type):
        """ To reverse the hexadecimal value string with byte size. (Return value without "0x" prefix.)

        Args:
            Value [str]: To covert the hex value with 0 padding.
                         This zero padding is based on struct string.
                         (It is caller responsible for providing correct format string.)
            Type  [str]: The struct format string.

        Returns:
            ReverseByte [str]: The hexadecimal value after doing the byte reversed.

        Raises:
            None.
        """
        InputType = Type.upper ()
        if InputType == 'B':
            return ZeroPadding (f"{Value:X}", (2))
        elif InputType == 'H':
            return ZeroPadding (f"{Value:X}", (4))
        elif InputType == 'I':
            return ZeroPadding (f"{Value:X}", (8))
        elif InputType == 'Q':
            return ZeroPadding (f"{Value:X}", (16))
        else:
            print ('!!! Format type is incorrect !!!')
            sys.exit (1)

    def __GetHeaderData (self):
        """ Inner method to get the header data with key and value.

        Args:
            None.

        Returns:
            Data [dict]: key - member of struct, value - value of this member.
                         e.g. { 'Guid1': '26FDAA3D', ... }

        Raises:
            None.
        """
        Data             = dict ()
        CurrentPosition  = 0

        for HeaderItem, HeaderValue in self.__FormatDataDict.items ():
            PyHdrFormatLen = len (HeaderValue['Type'])

            if PyHdrFormatLen == 0:
                print ('!!! Please assign the format type in STRUCT data dictionary !!!')
                sys.exit (1)
            elif PyHdrFormatLen == 1:
                HexValue = self.__ConvertToHexValue (self.__StructData[CurrentPosition], HeaderValue['Type'])
            else:
                HexValue = str ()
                for Idx in range (PyHdrFormatLen):
                    HexValue += (self.__ConvertToHexValue (self.__StructData[CurrentPosition+Idx], HeaderValue['Type'][Idx]))

            Data.update ({HeaderItem: HexValue})
            CurrentPosition += PyHdrFormatLen

        return Data

    def Get (self):
        """ To get the header data with key and value.

        Args:
            None.

        Returns:
            __Data [dict]: key - member of struct, value - value of this member.
                           e.g. { 'Guid1': '26FDAA3D', ... }

        Raises:
            None.
        """
        return self.__Data
