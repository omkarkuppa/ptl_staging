## @file
#  Wrapper to manipulate/parse the C-language struct.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2025 Intel Corporation.
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
#  @par Specification Reference:
#
##

import struct
from typing import Any, Dict, MutableMapping, Tuple

from BtgAcmMisc.Base._Utils import *

#
# C-Struct Byte Order
#
BYTE_ORDER_LITTLE_ENDIAN: str = '<'

#
# C-Struct Data Type
#
C_STRUCT_UINT8      : str = 'B'
C_STRUCT_UINT8_BYTE : int = 1
C_STRUCT_INT16      : str = 'h'
C_STRUCT_INT16_BYTE : int = 2
C_STRUCT_UINT16     : str = 'H'
C_STRUCT_UINT16_BYTE: int = 2
C_STRUCT_UINT32     : str = 'L'
C_STRUCT_UINT32_BYTE: int = 4
C_STRUCT_UINT64     : str = 'Q'
C_STRUCT_UINT64_BYTE: str = 8

C_STRUCT_BYTE_SIZE_DICT: MutableMapping[str, int] = {
    C_STRUCT_UINT8 : C_STRUCT_UINT8_BYTE,
    C_STRUCT_INT16 : C_STRUCT_INT16_BYTE,
    C_STRUCT_UINT16: C_STRUCT_UINT16_BYTE,
    C_STRUCT_UINT32: C_STRUCT_UINT32_BYTE,
    C_STRUCT_UINT64: C_STRUCT_UINT64_BYTE,
    }

#
# Format dictionary key.
#
FIELD_FORMAT: str = 'Type'
FIELD_SIZE  : str = 'Size'

def GetStructAndByteSize (
    DataDict: MutableMapping[str, Any],
    ) -> Tuple[str, int]:
    """ Get the structure format for struct unpack and byte size.

    Note:
        It is caller responsibility to provide the correct information.

    Args:
        DataDict (MutableMapping[str, Any]):
            The format should be dict of dict.
            {
                'Guid1': { FIELD_FORMAT: 'I', FIELD_SIZE: 4 },
                ...
            }

    Raises:
        None.

    Returns:
        Tuple[str, int]:
            str:
                The struct format string used for unpacked.
            int:
                The size of this struct in byte.
    """
    Struct  : str = BYTE_ORDER_LITTLE_ENDIAN
    ByteSize: int = 0

    for _, Value in DataDict.items ():
        Struct   += Value[FIELD_FORMAT]
        ByteSize += Value[FIELD_SIZE]

    return Struct, ByteSize

class StructWrapper (object):
    def __init__ (
        self,
        Buffer  : bytearray,
        Struct  : str,
        DataDict: MutableMapping[str, Any],
        ) -> None:
        """ Class to support common manipulate on C-type struct.

        Args:
            Buffer (bytearray):
                The buffer use to parse the C-language struct.
            Struct (str):
                The struct format string used for unpacked.
            DataDict (MutableMapping[str, Any]):
                The dictionary to represent the C-language struct.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer  : bytearray                = Buffer
        self.__Struct  : str                      = Struct
        self.__DataDict: MutableMapping[str, Any] = DataDict

        self.__StructData = struct.unpack (self.__Struct, self.__Buffer)
        self.__Data       = self.__GetStructData ()

    @property
    def Data (self) -> Dict[str, str]:
        """ Return the decoded C-structure data.

        Args:
            None.

        Raises:
            None.

        Returns:
            Dict[str, str]:
                Decoded structure data with Key-Value dictionary.
                str:
                    Member of struct.
                str:
                    Value of this member.
        """
        return self.__Data

    def __GetMemberData (self, Value: int, Type: str) -> str:
        """ Get the member of struct value from byte buffer.

        Args:
            Value (int):
                Value of this member.
            Type (str):
                Unpack format string for this member.

        Raises:
            ValueError:
                Unsupported unpack format string.

        Returns:
            str:
                The value of this member of struct from byte buffer.
        """
        MAPPING_TABLE: dict = {
            C_STRUCT_UINT8 : 2,
            C_STRUCT_UINT16: 4,
            C_STRUCT_UINT32: 8,
            C_STRUCT_UINT64: 16,
            }

        if Type not in MAPPING_TABLE.keys ():
            raise ValueError (f'Format type [{Type}] is not supported.')

        return ZeroPadding (f"{Value:X}", (MAPPING_TABLE[Type]))

    def __GetStructData (self) -> Dict[str, str]:
        """ Return the C-language parsed struct data.

        Args:
            None.

        Raises:
            ValueError:
                Format type is not assign STRUCT data.

        Returns:
            Dict[str, str]:
                Key-Value member of this structure.
                str:
                    Member of struct.
                str:
                    Value of this member.
        """
        Data     : Dict[str, str] = dict ()
        CurrPos  : int            = 0
        Idx      : int            = 0
        Key      : str            = None
        Value    : Any            = None
        FormatLen: int            = None
        HexVal   : str            = None

        for Key, Value in self.__DataDict.items ():
            FormatLen = len (Value[FIELD_FORMAT])
            HexVal    = str ()

            if FormatLen == 0:
                raise ValueError ('Format type is not assign STRUCT data.')
            elif FormatLen == 1:
                HexVal = self.__GetMemberData (
                                Value = self.__StructData[CurrPos],
                                Type  = Value[FIELD_FORMAT],
                                )
            else:
                for Idx in range (FormatLen):
                    HexVal += self.__GetMemberData (
                                     Value = self.__StructData[CurrPos+Idx],
                                     Type  = Value[FIELD_FORMAT][Idx],
                                     )

            Data.update ({ Key: HexVal })
            CurrPos += FormatLen

        return Data
