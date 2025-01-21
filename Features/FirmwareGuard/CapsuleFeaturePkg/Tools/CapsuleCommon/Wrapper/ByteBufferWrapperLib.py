## @file
#  Wrapper to manipulate the bytearray type and read the binary file to
#  bytearray.
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

import codecs
import os
from copy import deepcopy
from typing import Union

from CapsuleCommon.Base.BaseLib import *
from CapsuleCommon.Base.ByteLib import ToBytes
from CapsuleCommon.Base.ConvertLib import ToInt
from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Base.FileLib import *
from CapsuleCommon.Base.PaddingLib import ZeroPadding
from CapsuleCommon.Base.PathLib import AbsPath
from CapsuleCommon.Constant.ByteLib import KILOBYTE_TO_BYTE, MEGABYTE_TO_BYTE
from CapsuleCommon.Format.HexadecimalLib import HexToDec, RemoveHexPrefix

def ByteStrToByte (Input: Union[bytes, bytearray, str]) -> bytearray:
    """ To convert hexadecimal string/bytes/bytearray with byte size.

    Args:
        Input (Union[bytes, bytearray, str]):
            The HEX string/bytes/bytearray to be converted.

    Raises:
        TypeError:
            The input type is not str/bytes/bytearray.

    Returns:
        bytearray:
            Return the converted bytearray.
    """
    HexValueStr: str       = str ()
    HexTempList: list      = list ()
    HexValueLen: int       = int ()
    Result     : bytearray = bytearray ()

    if isinstance (Input, (bytearray, bytes)):
        Result = deepcopy (bytearray (Input))
    elif isinstance (Input, (str)):
        HexValueStr = RemoveHexPrefix (Input)
        HexValueLen = len (HexValueStr)

        if (HexValueLen % 2) != 0:
            HexValueLen += 1

        HexValueStr = ZeroPadding (HexValueStr, HexValueLen)
        for Idx in range (int (HexValueLen / 2)):
            StartIdx = (Idx + 0) * 2
            EndIdx   = (Idx + 1) * 2
            HexTempList.append (HexToDec (HexValueStr[StartIdx:EndIdx]))

        Result = bytearray (HexTempList)
    else:
        raise TypeError (f'Only supported the [bytearray, bytes, str]')

    return Result

def ByteReverse (
    Input: Union[bytes, bytearray, str],
    ) -> bytearray:
    """ To reverse hexadecimal string/bytes/bytearray with byte size.

    Args:
        Input (Union[bytes, bytearray, str]):
            The HEX string/bytes/bytearray to be reversed.

    Raises:
        TypeError:
            The input type is not str/bytes/bytearray.

    Returns:
        bytearray:
            Return the reversed bytearray.
    """
    Result: bytearray = ByteStrToByte (Input)
    Result.reverse ()
    return Result

def ByteToStr (
    Input     : Union[bytes, bytearray, str],
    Encode    : str  = 'utf-8',
    IsReverse : bool = True,
    ) -> str:
    """ Encode the bytes by specific encoding.

    Args:
        Input (Union[bytes, bytearray, str]):
            The byte-string/bytes/bytearray to be encoded to string.
        Encode (str, optional):
            The encode type to encode the bytes.
            Defaults to 'utf-8'.
        IsReverse (bool, optional):
            To reverse the input by byte.
            True  - Do the byte reverse. (Since it is Little-Endian)
            False - No need to do byte reverse. (Since it is Big-Endian)
            Defaults to True.

    Raises:
        TypeError:
            IsReverse is not bool type.

    Returns:
        str:
            The string to encode bytes by specific encoding.
    """
    ByteArray: bytearray = ByteStrToByte (Input)
    Result   : str       = str ()

    if not isinstance (IsReverse, bool):
        raise TypeError (f'IsReverse should be bool type.')

    if IsReverse:
        ByteArray.reverse ()

    Result = codecs.decode (ByteArray, Encode)
    #
    # Remove the Null-terminated string
    #
    Result = Result.rstrip('\x00')

    return Result

class ByteBuffer (object):
    def __init__ (
        self,
        FilePath  : Union[str, os.PathLike]       = None,
        Buffer    : Union[bytes, bytearray, None] = None,
        ) -> None:
        """ Wrapper to support the byte-level operation.

        Note:
            Input arguments should be matched one of the following
            combination. (Only support one of these)
                - FilePath + Permission
                - Buffer.

        Args:
            FilePath (Union[str, os.PathLike], optional):
                The path of byte-based file to be read.
                Defaults to None..
            Buffer (Union[bytes, bytearray, None], optional):
                The buffer to be wrapped.
                Defaults to None.

        Raises:
            ValueError:
                (1) FilePath and Buffer cannot empty at same time.
                (2) FilePath and Buffer cannot be assigned at same time.
            TypeError:
                Input buffer should be bytes or bytearray.

        Returns:
            None.
        """
        self.__FilePath : Union[str, None]       = None
        self.__ByteArray: Union[bytearray, None] = None
        self.__FileName : str                    = str ()
        self.__Extension: str                    = str ()
        self.__IsValid  : bool                   = False

        if (not FilePath) and (not Buffer):
            raise ValueError (f'FilePath and Buffer cannot empty at same.')
        elif (FilePath) and (Buffer):
            raise ValueError (f'FilePath and Buffer cannot assign at same.')

        if FilePath is not None:
            self.__FilePath                   = AbsPath (FilePath)
            self.__FileName, self.__Extension = \
                GetFileNameAndExt (self.__FilePath)
            self.__ByteArray = ReadBufferFile (self.__FilePath)
        elif Buffer:
            if (not isinstance (Buffer, bytes)) and \
               (not isinstance (Buffer, bytearray)) :
                raise TypeError (f'Buffer type should be bytes or bytearray.')

            self.__ByteArray = bytearray (Buffer)

        self.__IsValid = True
        self.__PrintInfo ()

    @property
    def Path (self) -> str:
        """ Get the input path.

        Note:
            If input is Buffer directly, would return empty string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The input path.
        """
        return self.__FilePath

    @property
    def Name (self) -> str:
        """ The filename part from the specific path.

        Note:
            If input is Buffer directly, would return empty string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The filename part of this part.
        """
        return self.__FileName

    @property
    def Extension (self) -> str:
        """ Get the extension part from the specific path.

        Note:
            If input is Buffer directly, would return empty string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The extension part of this path.
        """
        return self.__Extension

    @property
    def Size (self) -> int:
        """ Get the number of bytes within this bytearray.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The number of bytes within this bytearray.
        """
        return len (self.__ByteArray)

    @property
    def Valid (self) -> bool:
        """ Check if the input file/buffer is valid.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - The input file/buffer is valid.
                False - The input file/buffer is not valid.
        """
        return self.__IsValid

    @property
    def Buffer (self) -> bytearray:
        """ Fetch the bytearray without any wrapper.

        Args:
            None.

        Raises:
            None.

        Returns:
            bytearray:
                The default type of bytearray (without any wrapper).
        """
        return self.__ByteArray

    def __PrintInfo (self) -> None:
        """ Print the debug related message on terminal.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        SizeInKiloByte: float = self.Size / KILOBYTE_TO_BYTE
        SizeInMegaByte: float = self.Size / MEGABYTE_TO_BYTE

        if SizeInMegaByte >= 1.0:
            DEBUG (DEBUG_TRACE, f'Binary file size: {SizeInMegaByte} MB')
        elif SizeInKiloByte >= 1.0:
            DEBUG (DEBUG_TRACE, f'Binary file size: {SizeInKiloByte} KB')
        else:
            DEBUG (DEBUG_TRACE, f'Binary file size: {self.Size} Byte')

    def Padding (
        self,
        TargetSize: Union[str, int],
        Char      : Union[bytes, bytearray, str, int] = b'\xFF',
        ) -> None:
        """ To add the padding character into bytearray.

        Args:
            TargetSize (Union[str, int]):
                The total size after doing the padding.
            Char (Union[bytes, bytearray, str, int], optional):
                The padding character assigned by user.
                Defaults to b'\xFF'.

        Raises:
            ValueError:
                (1) The length of character used for padding not 1 byte.
                (2) The current size is larger than target size.

        Returns:
            None.
        """
        Source      : int                    = self.Size
        Target      : int                    = ToInt (TargetSize)
        PaddingArray: Union[bytearray, None] = None
        PaddingChar : Union[bytes, None]     = None

        DEBUG (DEBUG_TRACE, f'Padding buffer: [{Source}]->[{Target}]')

        PaddingChar = ToBytes (Char)

        if len (PaddingChar) != 1:
            raise ValueError (f'The length of char for padding should be 1.')

        if Source > Target:
            raise ValueError (f'Current size {Source} > Target size {Target}')

        PaddingArray      = bytearray (Char * (Target - Source))
        self.__ByteArray += PaddingArray

        return STATUS_SUCCESS

    def Find (
        self,
        Input: Union[bytes, bytearray, str, int],
        ) -> Union[List[int], List[None]]:
        """ Find the specific input signature matching in the bytearray.

        Note:
            (1) Now support ANSI character in str.
            (2) Others need to input with bytes or bytearray or int.

        Args:
            Input (Union[bytes, bytearray, str, int]):
                The input data need to be found in the bytearray.

        Raises:
            None.

        Returns:
            Union[List[int], List[None]]:
                Return the list of offsets.
                If it is empty, means not found.
        """
        Signature: bytes = ToBytes (Input)
        Start    : int   = 0
        Location : int   = 0
        Result   : list  = list ()

        while True:
            Location = self.Buffer.find (Signature, Start)
            Result.append (Location)

            if (Location == -1):
                break
            else:
                Start = Location + len (Signature)

        if len (Result) == 1 and Result[0] == -1:
            Result = []
            DEBUG (DEBUG_TRACE, f'Could not find the signature in buffer.')
        elif len (Result) != 1:
            Result = Result[0:-1]
            DEBUG (
              DEBUG_TRACE,
              f'Find the signature in buffer location {Result}'
              )

        return Result

    def Write (
        self,
        Offset : Union[str, int],
        Replace: Union[bytes, bytearray, str, int],
        ) -> None:
        """ To overwrite the bytearray start from specific offset.

        Note:
            If replaced bytearray is over all the source bytearray
            length, the over size part would not take action.

        Args:
            Offset (Union[str, int]):
                The offset start to do the replacement action.
            Replace (Union[bytes, bytearray, str, int]):
                The data need to be replaced in the bytearray.

        Raises:
            None.

        Returns:
            None.
        """
        TargetOffset : int   = ToInt (Offset)
        CurrentOffset: int   = int ()
        TargetReplace: bytes = ToBytes (Replace)

        for Idx in range (len (TargetReplace)):
            CurrentOffset = TargetOffset + Idx
            if CurrentOffset > self.Size:
                DEBUG (
                  DEBUG_WARN,
                  f'Byte 0x%08{CurrentOffset} replace out of buffer size '
                  f'{self.__BufferSize} would not take action!'
                  )
            else:
                self.__FileBuffer[CurrentOffset] = TargetReplace[Idx]
                DEBUG (
                  DEBUG_TRACE,
                  f'Byte 0x%08{CurrentOffset} replace finished!'
                  )

    def Save (
        self,
        TargetPath: Union[str, os.PathLike],
        ) -> None:
        """ To save the memory buffer into file(storage).

        Args:
            TargetPath (Union[str, os.PathLike]):
                The path of byte-based file to be output.

        Raises:
            None.

        Returns:
            None.
        """
        WriteBufferFile (self.Buffer, TargetPath)
