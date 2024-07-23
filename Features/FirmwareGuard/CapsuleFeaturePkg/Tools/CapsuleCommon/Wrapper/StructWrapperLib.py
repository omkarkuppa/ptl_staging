## @file
#  Wrapper to manipulate/parse the C-language struct.
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

import re
import struct
from typing import Any, List, MutableMapping, Pattern, Tuple, Union

from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Base.PaddingLib import *

#
# C-Struct Byte Order
#
BYTE_ORDER_NATIVE_ALIGNMENT: str = '@'
BYTE_ORDER_NATIVE          : str = '='
BYTE_ORDER_LITTLE_ENDIAN   : str = '<'
BYTE_ORDER_BIG_ENDIAN      : str = '>'
BYTE_ORDER_NETWORK         : str = '!'  # The same as big-endian

BYTE_ORDER_TYPE_LIST: List[str] = [
    BYTE_ORDER_NATIVE_ALIGNMENT,
    BYTE_ORDER_NATIVE,
    BYTE_ORDER_LITTLE_ENDIAN,
    BYTE_ORDER_BIG_ENDIAN,
    BYTE_ORDER_NETWORK,
    ]

#
# C-Struct Data Type
#
C_STRUCT_PADDING     : str = 'x'
C_STRUCT_PADDING_BYTE: int = 1
C_STRUCT_BOOL        : str = '?'
C_STRUCT_BOOL_BYTE   : int = 1
C_STRUCT_CHAR        : str = 'c'
C_STRUCT_CHAR_BYTE   : int = 1
C_STRUCT_CHAR_ARRAY  : str = 's'
C_STRUCT_CHAR_BYTE   : int = 1
C_STRUCT_INT8        : str = 'b'
C_STRUCT_INT8_BYTE   : int = 1
C_STRUCT_UINT8       : str = 'B'
C_STRUCT_UINT8_BYTE  : int = 1
C_STRUCT_INT16       : str = 'h'
C_STRUCT_INT16_BYTE  : int = 2
C_STRUCT_UINT16      : str = 'H'
C_STRUCT_UINT16_BYTE : int = 2
C_STRUCT_INT32       : str = 'l'
C_STRUCT_INT32_BYTE  : int = 4
C_STRUCT_UINT32      : str = 'L'
C_STRUCT_UINT32_BYTE : int = 4
C_STRUCT_INT64       : str = 'q'
C_STRUCT_INT64_BYTE  : int = 8
C_STRUCT_UINT64      : str = 'Q'
C_STRUCT_UINT64_BYTE : str = 8

C_STRUCT_BYTE_SIZE_DICT: MutableMapping[str, int] = {
    C_STRUCT_BOOL      : C_STRUCT_BOOL_BYTE,
    C_STRUCT_CHAR      : C_STRUCT_CHAR_BYTE,
    C_STRUCT_CHAR_ARRAY: C_STRUCT_CHAR_BYTE,
    C_STRUCT_INT8      : C_STRUCT_INT8_BYTE,
    C_STRUCT_UINT8     : C_STRUCT_UINT8_BYTE,
    C_STRUCT_INT16     : C_STRUCT_INT16_BYTE,
    C_STRUCT_UINT16    : C_STRUCT_UINT16_BYTE,
    C_STRUCT_INT32     : C_STRUCT_INT32_BYTE,
    C_STRUCT_UINT32    : C_STRUCT_UINT32_BYTE,
    C_STRUCT_INT64     : C_STRUCT_INT64_BYTE,
    C_STRUCT_UINT64    : C_STRUCT_UINT64_BYTE,
    }

FILED_FORMAT: str = 'Type'
FIELD_SIZE  : str = 'Size'

#
# Check the format struct is valid
#
STRUCT_LEGAL_REGEX_STR   : str = '[b,c,h,l,s,q,x,B,H,L,Q,x,?]'
#
# Type 0: Digit Number + one Struct Data Type
# e.g. 12B
#
STRUCT_FORMAT_TYPE0      : int = 0x0000_0000
STRUCT_FORMAT_TYPE0_REGEX: str = f'^(0|[1-9][0-9]*{STRUCT_LEGAL_REGEX_STR})$'
#
# Type 1: Multiple struct Data Type
# e.g. BBBB
#
STRUCT_FORMAT_TYPE1      : int = 0x0000_0001
STRUCT_FORMAT_TYPE1_REGEX: str = f'^({STRUCT_LEGAL_REGEX_STR})\\1{{0,}}$'

def IsFormatStrValid (
    FormatStr: str,
    ) -> bool:
    """ Check if the format string is matched one of the valid type.

    Args:
        FormatStr (str):
            The format string to be tested.

    Raises:
        TypeError:
            Input FormatStr is not str type.

    Returns:
        bool:
            True  - Input format string is matched one of types.
            False - Input format string is not matched one of types.
    """
    Regex_TYPE0: Pattern = re.compile (STRUCT_FORMAT_TYPE0_REGEX)
    Regex_TYPE1: Pattern = re.compile (STRUCT_FORMAT_TYPE1_REGEX)

    if not isinstance (FormatStr, str):
        raise TypeError (f'FormatStr should input str type.')

    if Regex_TYPE0.match (FormatStr):
        DEBUG (DEBUG_TRACE, f'Match the type0 - [{FormatStr}]')
    elif Regex_TYPE1.match (FormatStr):
        DEBUG (DEBUG_TRACE, f'Match the type1 - [{FormatStr}]')
    else:
        DEBUG (DEBUG_ERROR, f'FormatStr is not matched [{FormatStr}]')
        return False

    return True

def GetFormatStructType (
    FormatStr: str,
    ) -> int:
    """ Get the type of FormatStr.

    Args:
        FormatStr (str):
            The format string to be tested.

    Raises:
        ValueError:
            (1) Input format is not valid.
            (2) Input format is not matched to any types.

    Returns:
        int:
            The type of struct format string.
    """
    Result     : Union[None, int] = None
    Regex_TYPE0: Pattern          = re.compile (STRUCT_FORMAT_TYPE0_REGEX)
    Regex_TYPE1: Pattern          = re.compile (STRUCT_FORMAT_TYPE1_REGEX)

    if not IsFormatStrValid (FormatStr):
        raise ValueError (f'Input format is not valid.')

    if Regex_TYPE0.match (FormatStr):
        Result = STRUCT_FORMAT_TYPE0
    elif Regex_TYPE1.match (FormatStr):
        Result = STRUCT_FORMAT_TYPE1
    else:
        raise ValueError (f'Input format is not matched to any types.')

    return Result

def GetStructFormatAndByteSize (
    FormatDataDict: MutableMapping[str, Any],
    ByteOrder     : str = BYTE_ORDER_LITTLE_ENDIAN,
    ) -> Tuple[str, int]:
    """ To get the structure format for struct unpack and byte size.

    Note:
        It is caller responsibility to provide the correct information.

    Args:
        FormatDataDict (MutableMapping[str, Any]):
            The format should be dict of dict.
            {
                'Guid1': {'Type': 'I'     , 'Size': 4},
                ...
            }
        ByteOrder (str, optional):
            The byte order.
            Defaults to BYTE_ORDER_LITTLE_ENDIAN.

    Raises:
        ValueError:
            (1) Byte order is not in available list.
            (2) 'Type' is necessary key within struct information.
            (3) 'Size' is necessary key within struct information.
        TypeError:
            (1) Input FormatDataDict should be key-value format.
            (2) 'Type' should be the str type.
            (3) 'Size' should be the int type.

    Returns:
        Tuple[str, int]:
            str:
                The struct format strings used for unpacked.
            int:
                The size of this struct in byte.
    """
    TotalFormatStr: str = str ()
    TotalByteSize : int = 0
    FormatStr     : str = str ()
    ByteSize      : int = 0

    if ByteOrder not in BYTE_ORDER_TYPE_LIST:
        raise ValueError (f'ByteOrder not located in {BYTE_ORDER_TYPE_LIST}')

    TotalFormatStr += ByteOrder

    for _, Value in FormatDataDict.items ():
        if not isinstance (Value, dict):
            raise TypeError (f'Make sure each is key-value dict type.')
        elif FILED_FORMAT not in Value:
            raise ValueError (f'{FILED_FORMAT} is necessary in struct info.')
        elif FIELD_SIZE not in Value:
            raise ValueError (f'{FIELD_SIZE} is necessary in struct info.')

        FormatStr = Value[FILED_FORMAT]
        ByteSize  = Value[FIELD_SIZE]
        if not isinstance (FormatStr, str):
            raise TypeError (f'{FILED_FORMAT} should be str type.')
        elif not isinstance (ByteSize, int):
            raise TypeError (f'{FIELD_SIZE} should be int type.')
        elif not IsFormatStrValid (FormatStr):
            raise ValueError (f'FormatStr is not valid [{FormatStr}]')

        TotalFormatStr += FormatStr
        TotalByteSize  += ByteSize

    return TotalFormatStr, TotalByteSize

class Struct (object):
    def __init__ (
        self,
        Buffer    : Union[bytes, bytearray],
        FormatDict: dict,
        ByteOrder : str = BYTE_ORDER_LITTLE_ENDIAN,
        ) -> None:
        """ Struct class to support common manipulate on C-type struct.

        Args:
            Buffer (Union[bytes, bytearray]):
                The buffer use to parse the C-language struct.
            FormatDict (dict):
                The dictionary to represent the C-language struct.
                The key would be used to represent C-struct member name.
                Type would have the struct format string (data type).
                Size would be the whole struct byte size.
                {
                    'Guid1': {'Type': 'I'     , 'Size': 4},
                    ...
                }
            ByteOrder (str, optional):
                The byte order for different hardware architecture.
                Defaults to BYTE_ORDER_LITTLE_ENDIAN.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer    : Union[bytes, bytearray] = Buffer
        self.__FormatDict: dict                    = FormatDict
        self.__ByteOrder : str                     = ByteOrder

        self.__StructFormatStr: str = str ()
        self.__ByteSize       : int = int ()
        self.__StructFormatStr, self.__ByteSize = self.__GetStructFormatStr ()

        self.__Data: MutableMapping[str, Union[None, str, bytes]] = dict ()

        self.__StructData: tuple = struct.unpack (
                                            self.__StructFormatStr,
                                            self.__Buffer,
                                            )

        self.__Data = self.__GetStructMemberData ()

    @property
    def Data (self) -> MutableMapping[str, Union[None, str, bytes]]:
        """ Get the header data with key and value.

        Args:
            None.

        Raises:
            None.

        Returns:
            MutableMapping[str, Union[None, str, bytes]]:
                e.g. { 'Guid1': '26FDAA3D', ... }
                str:
                    Key   - member of struct.
                Union[None, str, bytes]:
                    Value - Value of this member.
                            c, s -> bytes.
                            x    -> None.
        """
        return self.__Data

    @property
    def Size (self) -> int:
        """ Total byte size within this struct.

        Note:
            It is caller responsibility to provide the correct struct.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The byte size of this struct.
        """
        return self.__ByteSize

    def __GetStructFormatStr (self) -> str:
        """ Get the Python struct unpack format string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Return the Python struct unpack format string.
        """
        return GetStructFormatAndByteSize (
                 self.__FormatDict,
                 self.__ByteOrder,
                 )

    def __GetStructMemberVal (
        self,
        Format: str,
        Data  : Tuple[Union[bytes, int]],
        ) -> Union[bytes, str]:
        """ Get the member of struct value from byte buffer.

        Note:
            This function would help to concat the each bytes value to
            correspond data type.

        Args:
            Format (str):
                Unpack format string for this member.
            Data (Tuple[Union[bytes, int]]):
                Value of this member.
                c, s -> bytes. x would be not called this one.
                Others would be HEX string.

        Raises:
            None.

        Returns:
            Union[bytes, str]:
                The value of this member of struct from byte buffer.
        """
        Result: Union[bytes, str] = str ()

        if (C_STRUCT_CHAR in Format) or (C_STRUCT_CHAR_ARRAY in Format):
            if len (Data) == 1:
                Result = Data
            else:
                Result = b''.join(Data)
        else:
            for Datum in Data:
                Result += ZeroPadding (
                            f'{Datum:X}',
                            C_STRUCT_BYTE_SIZE_DICT[Format] * 2,
                            )

        return Result

    def __GetStructMemberData (
        self,
        ) -> MutableMapping[str, Union[None, str, bytes]]:
        """ Get the each member data within the struct from byte buffer.

        Args:
            None.

        Raises:
            ValueError:
                Assigned format string is not valid.

        Returns:
            MutableMapping[str, Union[None, str, bytes]]:
                e.g. { 'Guid1': '26FDAA3D', ... }
                str:
                    Key   - member of struct.
                Union[None, str, bytes]:
                    Value - Value of this member.
                            c, s -> bytes.
                            x    -> None.
        """
        Result : MutableMapping[str, Union[None, str, bytes]] = dict ()
        CurrIdx: int                                          = 0
        Value  : Union[None, bytes, str]                      = str ()

        for MemberName, MemberData in self.__FormatDict.items ():
            MemberFormatStr: str = MemberData[FILED_FORMAT]

            if not IsFormatStrValid (MemberFormatStr):
                raise ValueError (f'FormatStr [{MemberFormatStr}] not valid.')

            Cnt       : int = int ()
            Format    : str = str ()
            MemberType: int = GetFormatStructType (MemberFormatStr)

            if C_STRUCT_PADDING in MemberFormatStr:
                Cnt   = 0
                Value = None
            else:
                if MemberType == STRUCT_FORMAT_TYPE0:
                    Cnt = int (MemberFormatStr[:-1])
                elif MemberType == STRUCT_FORMAT_TYPE1:
                    Cnt = len (MemberFormatStr)
                Format = MemberFormatStr[-1]

                Value = self.__GetStructMemberVal (
                               Format,
                               self.__StructData[CurrIdx:CurrIdx+Cnt],
                               )

            CurrIdx += Cnt
            Result.update ({MemberName: Value})

        return Result
