## @file
#  Library provided basic functions/define to do the byte operation.
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

from CapsuleCommon.Constant.DataTypeLib import *

from CapsuleCommon.Base.BaseLib import IsAscIIStr
from CapsuleCommon.Base.CheckLib import *

#
# Python build-in int type bytes order.
#
BYTE_ORDER_LITTLE: str       = 'little'
BYTE_ORDER_BIG   : str       = 'big'
BYTE_ORDER_LIST  : List[str] = [
    BYTE_ORDER_LITTLE,
    BYTE_ORDER_BIG,
    ]

def StrToBytes (Input: str) -> bytes:
    """ To convert string to bytes.

    Args:
        Input (str):
            The string to be converted to bytes.

    Raises:
        TypeError:
            The input to be trial is not str type.
        ValueError:
            The input string have non-ASC-II code.

    Returns:
        bytes:
            The bytes type after converting from str.
    """
    if not isinstance (Input, str):
        raise TypeError (f'The input type should be str.')
    elif not IsAscIIStr (Input):
        raise ValueError (f'The input exist non-ascii code.')

    return bytes (str (Input), 'ascii')

def IntToByte (Input: int) -> bytes:
    """ To convert int to bytes.

    Args:
        Input (int):
            The int to be converted to bytes.

    Raises:
        TypeError:
            The input to be trial is not int type.
        ValueError:
            The input is out of non-ASC-II code boundary. (0x00 - 0xFF)

    Returns:
        bytes:
            The bytes type after converting from int.
    """
    if not isinstance (Input, int):
        raise TypeError (f'The input type should be int.')
    elif (Input < 0) or (Input > 255):
        raise ValueError (f'The input int to bytes should between 0-255')

    return StrToBytes (str (Input))

def ToBytes (Input: Union[bytes, bytearray, str, int]) -> bytes:
    """ Universal function to convert X-type into bytes.

    Note:
        Now supported input type is bytes, bytearray, str, int.

    Args:
        Input (Union[bytes, bytearray, str, int]):
            The input to be converted into bytes.

    Returns:
        bytes:
            The bytes type after converting from X-type.
    """
    if isinstance (Input, bytes):
        return Input
    elif isinstance (Input, bytearray):
        return bytes (Input)
    elif isinstance (Input, str):
        return StrToBytes (Input)
    elif isinstance (Input, int):
        return IntToByte (Input)
    else:
        raise TypeError (f'The input type {type (Input)} is not supported.')

########################################################################
#                        CheckSum Functionality                        #
########################################################################
def BytesCalcSum (
    Input    : Union[bytes, bytearray],
    Bytes    : int = UINT32_BYTE_VALUE,
    ByteOrder: str = BYTE_ORDER_LITTLE,
    ) -> int:
    """ Calculate the bytes/bytearray summary value.

    Args:
        Input (Union[bytes, bytearray]):
            The bytes/bytearray to be calculated the sum value.
        Bytes (int, optional):
            The Bytes alignment to convert to integer number.
            Defaults to UINT32_BYTE_VALUE.
        ByteOrder (str, optional):
            The byte order.
            Defaults to BYTE_ORDER_LITTLE.

    Raises:
        TypeError:
            (1) Input is not bytearray.
            (2) Bytes is not integer.
        ValueError:
            (1) ByteOrder is not in BYTE_ORDER_LIST.
            (2) Bytes size is negative/zero value.
            (3) Buffer is 0 size.
            (4) Buffer is not alignment.
            (5) Buffer should be multiple of Bytes value.

    Returns:
        int:
            Summary of bytes value.
    """
    if not isinstance (Input, (bytes, bytearray)):
        raise TypeError (f'Input should be bytearray.')
    if not isinstance (Bytes, int):
        raise TypeError (f'Bytes should be integer number.')

    if ByteOrder not in BYTE_ORDER_LIST:
        raise ValueError (f'ByteOrder invalid. Should be {BYTE_ORDER_LIST}.')
    if Bytes <= 0:
        raise ValueError (f'Bytes size should not negative.zero value.')
    if len (Input) == 0:
        raise ValueError (f'Length of buffer should not 0.')
    elif (len (Input) % Bytes) != 0:
        raise ValueError (f'Buffer not alignment. [{Bytes}]')
    elif int (len (Input) / Bytes) == 0:
        raise ValueError (f'Buffer size should be multiple of [{Bytes}]')

    Iter       : int       = int (len (Input) / Bytes)
    Sum        : int       = 0
    Buffer     : bytearray = bytearray ()
    StartOffset: int       = int ()
    EndOffset  : int       = int ()

    for Idx in range (Iter):
        StartOffset = Bytes * Idx
        EndOffset   = Bytes * (Idx + 1)

        Buffer = bytearray (Input[StartOffset:EndOffset])
        Sum   += int.from_bytes (Buffer, byteorder = ByteOrder)

    return Sum

def BytesCheckSum8 (
    Input    : Union[bytes, bytearray],
    ByteOrder: str = BYTE_ORDER_LITTLE,
    ) -> bool:
    """ Wrapper to valid UINT8 checksum.

    Args:
        Input (Union[bytes, bytearray]):
            The bytes/bytearray to be calculated the sum value.
        ByteOrder (str, optional):
            The byte order.
            Defaults to BYTE_ORDER_LITTLE.

    Raises:
        None.

    Returns:
        bool:
            True  - CheckSum is passed.
            False - CheckSum is not passed.
    """
    Sum: int = BytesCalcSum (
                 Input,
                 Bytes     = UINT8_BYTE_VALUE,
                 ByteOrder = ByteOrder,
                 )

    return ((Sum & UINT8_MASK_VALUE) == 0)

def BytesCheckSum16 (
    Input    : Union[bytes, bytearray],
    ByteOrder: str = BYTE_ORDER_LITTLE,
    ) -> bool:
    """ Wrapper to valid UINT16 checksum.

    Args:
        Input (Union[bytes, bytearray]):
            The bytes/bytearray to be calculated the sum value.
        ByteOrder (str, optional):
            The byte order.
            Defaults to BYTE_ORDER_LITTLE.

    Raises:
        None.

    Returns:
        bool:
            True  - CheckSum is passed.
            False - CheckSum is not passed.
    """
    Sum: int = BytesCalcSum (
                 Input,
                 Bytes     = UINT16_BYTE_VALUE,
                 ByteOrder = ByteOrder,
                 )

    return ((Sum & UINT16_MASK_VALUE) == 0)

def BytesCheckSum32 (
    Input    : Union[bytes, bytearray],
    ByteOrder: str = BYTE_ORDER_LITTLE,
    ) -> bool:
    """ Wrapper to valid UINT32 checksum.

    Args:
        Input (Union[bytes, bytearray]):
            The bytes/bytearray to be calculated the sum value.
        ByteOrder (str, optional):
            The byte order.
            Defaults to BYTE_ORDER_LITTLE.

    Raises:
        None.

    Returns:
        bool:
            True  - CheckSum is passed.
            False - CheckSum is not passed.
    """
    Sum: int = BytesCalcSum (
                 Input,
                 Bytes     = UINT32_BYTE_VALUE,
                 ByteOrder = ByteOrder,
                 )

    return ((Sum & UINT32_MASK_VALUE) == 0)

def BytesCheckSum64 (
    Input    : Union[bytes, bytearray],
    ByteOrder: str = BYTE_ORDER_LITTLE,
    ) -> bool:
    """ Wrapper to valid UINT64 checksum.

    Args:
        Input (Union[bytes, bytearray]):
            The bytes/bytearray to be calculated the sum value.
        ByteOrder (str, optional):
            The byte order.
            Defaults to BYTE_ORDER_LITTLE.

    Raises:
        None.

    Returns:
        bool:
            True  - CheckSum is passed.
            False - CheckSum is not passed.
    """
    Sum: int = BytesCalcSum (
                 Input,
                 Bytes     = UINT64_BYTE_VALUE,
                 ByteOrder = ByteOrder,
                 )

    return ((Sum & UINT64_MASK_VALUE) == 0)
