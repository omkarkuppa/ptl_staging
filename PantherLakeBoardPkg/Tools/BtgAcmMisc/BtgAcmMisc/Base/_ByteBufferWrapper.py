## @file
#  Wrapper to manipulate the bytearray type and read the binary file.
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

import os
from typing import List, Tuple, Union

from BtgAcmMisc.Base._Utils import *

class _ToBytes (object):
    def __init__ (self, Input: Union[str, bytes, bytearray]) -> None:
        """ Class support to convert input to bytes.

        Args:
            Input (Union[str, bytes, bytearray]):
                Input to be converted into bytes.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Input    : Union[str, bytes, bytearray] = Input
        self.__InputType: str                          = type (self.__Input)
        self.__Bytes    : bytes                        = self.__GetBytes ()

    @property
    def Bytes (self) -> bytes:
        """ Return the bytes converted from the input.

        Args:
            None.

        Raises:
            None.

        Returns:
            bytes:
                Bytes converted from the input.
        """
        return self.__Bytes

    def __GetBytes (self) -> bytes:
        """ Internal dispatcher to convert the input into bytes type.

        Args:
            None.

        Raises:
            TypeError:
                Unsupported input data type.

        Returns:
            bytes:
                Bytes converted from the input.
        """
        Result         : bytes               = None
        DISPATCHER_DICT: dict[str, function] = {
            str      : self.__ProcessStr,
            bytes    : self.__ProcessBytes,
            bytearray: self.__ProcessBytes,
            }

        if self.__InputType not in DISPATCHER_DICT:
            raise TypeError (
                    f'Unsupported type shall be one of {DISPATCHER_DICT.keys}'
                    )

        Function = DISPATCHER_DICT[self.__InputType]
        Result   = Function ()

        return Result

    def __ProcessBytes (self) -> bytes:
        """ Internal function to do the type-casting.

        Args:
            None.

        Raises:
            None.

        Returns:
            bytes:
                Bytes converted from the input bytearray / bytes.
        """
        Result: bytes = bytes (self.__Input)
        return Result

    def __ProcessStr (self) -> bytes:
        """ Internal function to encode string to ASC-II encoded bytes.

        Args:
            None.

        Raises:
            None.

        Returns:
            bytes:
                ASC-II bytes converted from the input string.
        """
        Result: bytes = bytes (str (self.__Input), 'ascii')
        return Result

class ByteBuffer (object):
    def __init__ (self, FilePath: str) -> None:
        """ Class to provide the wrapper for bytes buffer operation.

        Args:
            FilePath (str):
                Path of file to be operated.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FilePath: str = os.path.abspath (FilePath)

        self.__PreCheck ()

        self.__Buffer    : bytes = None
        self.__BufferSize: int   = 0

        self.__Buffer, self.__BufferSize = self.__Read ()

        self.__Debug ()

    @property
    def Buffer (self) -> bytearray:
        """ Return the buffer wrapped in this object.

        Args:
            None.

        Raises:
            None.

        Returns:
            bytearray:
                Buffer wrapped in this object.
        """
        return bytearray (self.__Buffer)

    @property
    def Size (self) -> int:
        """ Return the size of the buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Size of the buffer.
        """
        return self.__BufferSize

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                FilePath shall be str type.
            ValueError:
                (1) FilePath is not exist.
                (2) FilePath is not a file.

        Returns:
            None.
        """
        if not isinstance (self.__FilePath, str):
            raise TypeError ('FilePath shall be str type.')

        if not os.path.exists (self.__FilePath):
            raise ValueError ('FilePath shall be exist.')
        elif not os.path.isfile (self.__FilePath):
            raise ValueError ('FilePath shall be a file.')

    def __Read (self) -> Tuple[bytes, int]:
        """ Internal function to read the binary buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            Tuple[bytes, int]:
                bytes:
                    Buffer read from assigned file path.
                int:
                    Size of the buffer.
        """
        Buffer    : bytes = None
        BufferSize: int   = None

        with open (self.__FilePath, 'rb') as File:
            Buffer     = File.read ()
            BufferSize = len (Buffer)

        return Buffer, BufferSize

    def __Debug (self) -> None:
        """ Print the debug related message on terminal.

        Args:
            None.

        Returns:
            None.

        Raises:
            None.
        """
        SizeInKiloByte = self.__BufferSize / KILOBYTE_TO_BYTE
        SizeInMegaByte = self.__BufferSize / MEGABYTE_TO_BYTE

        print (f'Binary file path: {self.__FilePath}')

        if SizeInMegaByte >= 1.0:
            print (f'Buffer size: {SizeInMegaByte} MB')
        elif SizeInKiloByte >= 1.0:
            print (f'Buffer size: {SizeInKiloByte} KB')
        else:
            print (f'Buffer size: {self.__BufferSize} Byte')

    def Trim (self, Offset: int, Size: int) -> None:
        """ Trim the buffer to the assigned offset and size.

        Args:
            Offset (int):
                Offset to trim the buffer.
            Size (int):
                Size to trim the buffer.

        Raises:
            TypeError:
                (1) Offset is not int type.
                (2) Size is not int type.
            ValueError:
                (1) Offset is not positive value.
                (2) Size is not positive value.
                (3) Start offset is not inside the buffer.
                (4) End offset is not inside the buffer.

        Returns:
            None.
        """
        if not isinstance (Offset, int):
            raise TypeError ('Offset shall be int type.')
        elif not isinstance (Size, int):
            raise TypeError ('Size shall be int type.')

        if Offset < 0:
            raise ValueError ('Offset shall be positive value.')
        elif Size < 0:
            raise ValueError ('Size shall be positive value.')
        elif Offset > self.__BufferSize:
            raise ValueError ('Start offset is outside of buffer.')
        elif (Offset + Size) > self.__BufferSize:
            raise ValueError ('End offset is outside of buffer.')

        self.__FilePath   = None
        self.__Buffer     = self.__Buffer[Offset:Offset+Size]
        self.__BufferSize = len (self.__Buffer)

    def Find (self, Input: Union[bytes, bytearray, str]) -> List[int]:
        """ Find the assigned signature inside the buffer.

        Args:
            Input (Union[bytes, bytearray, str]):
                Signature used to search inside the buffer.

        Raises:
            None.

        Returns:
            List[int]:
                List of the offset matched to the assigned signature.
        """
        Signature: bytes = _ToBytes (Input).Bytes
        Result   : list  = list ()
        Start    : int   = 0
        Location : int   = 0

        while True:
            Location = self.__Buffer.find (Signature, Start)
            Result.append (Location)

            if (Location == -1):
                break
            else:
                Start = Location + len (Signature)

        if len (Result) == 1 and Result[0] == -1:
            Result = []
            print (f'Could not find the signature in buffer.')
        elif len (Result) != 1:
            Result = Result[0:-1]
            print (f'Find the signature in buffer location {Result}')

        return Result
