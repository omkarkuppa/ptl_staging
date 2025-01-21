## @file
#  Script to build the External Data Representation Standard (XDR) file.
#
#  [RFC 4506]
#  ----------
#
#  UNSIGNED INTEGER
#
#     MSB                        LSB
#  +-----------------------------------+
#  | Byte 0 | Byte 1 | Byte 2 | Byte 3 |
#  +-----------------------------------+
#
#  +------------------------------------------------+
#  |   Data Length   |             Data             |
#  +------------------------------------------------+
#
#  Note: Data Length is big-endian.
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
#    - RFC 4506 - https://www.rfc-editor.org/rfc/rfc4506.
##

import os
import argparse
from typing import List, Union

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (C) 2023 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to XDR format file.\n'

#
# Keyword use to mark data length is 0.
# In variable-length opaque data element.
#
KEYWORD_EMPTY: str = 'None'

DEFAULT_XDR_OUTPUT_FILE_NAME: str = 'OutputXdr.bin'

class XdrBuilder (object):
    def __init__ (
        self,
        InputList : List[Union[None, str, bytearray, bytes]],
        ) -> None:
        """ Class used to build the XDR format buffer.

        Args:
            InputList (List[Union[None, str, bytearray, bytes]]):
                The list of input to be converted into XDR format.
                (Note: The order is the same as list)

        Raises:
            None.

        Returns:
            None.
        """
        self.__InputList: List[bytes] = self.__GetInputBuffer (InputList)
        self.__XdrBuffer: ByteBuffer  = self.__GetXdrBuffer ()

    def __GetByteArrayBuffer (
        self,
        Input: Union[None, bytes, bytearray, str],
        ) -> bytearray:
        """ To convert the input into bytearray.

        Args:
            Input (Union[None, bytes, bytearray, str]):
                Input to be converted to bytearray.
                (1) If input is None, would be converted to zero size
                    bytearray.
                (2) If input is str,
                    (a) If is 'None' keyword, would be converted to
                        zero size bytearray.
                    (b) Otherwise, would try read the file located and
                        as it to bytearray.

        Raises:
            ValueError:
                (1) The assigned path is not exist.
                (2) The assigned path is not a file.
            TypeError:
                (1) Type of input is not str/bytes/bytearray/None.

        Returns:
            bytearray:
                The bytearray type buffer.
                If it is None, would return the zero size bytearray.
        """
        Buffer: bytearray = bytearray ()
        Path  : str       = str ()

        if IsNoneType (Input):
            Buffer = bytearray ()
        elif isinstance (Input, bytearray):
            Buffer = Input
        elif isinstance (Input, bytes):
            Buffer = bytearray (Input)
        elif isinstance (Input, str):
            Path = AbsPath (Input)
            if not IsPathExist (Path):
                raise ValueError (f'Path [{Path}] is not exist!')
            elif not IsFile (Path):
                raise ValueError (f'Path [{Path}] is not file!')
            else:
                Buffer = ByteBuffer (FilePath = Path).Buffer
        else:
            raise TypeError (f'Input is not str/bytes/bytearray/None!')

        return Buffer

    def __GetInputBuffer (
        self,
        InputList: List[Union[None, str, bytearray, bytes]],
        ) -> List[bytes]:
        """ Convert the list of input into XDR format bytes.

        Note:
            (1) If it is zero size, it would append 4 bytes 0x0 value.
                To mark this section as 0 size.
            (2) Buffer should be 4-byte alignment. Function would do the
                padding in the end of file with 0x00.

        Args:
            InputList (List[Union[None, str, bytearray, bytes]]):
                List of input to be converted to XDR format bytes.
                Convert rule based on __GetByteArrayBuffer () function.

        Raises:
            None.

        Returns:
            List[bytes]:
                The list of XDR format bytes.
        """
        Buffer     : bytearray                          = bytearray ()
        Result     : List[bytearray]                    = list ()
        BufferSize : int                                = int ()
        Remainder  : int                                = int ()
        Input      : Union[None, str, bytearray, bytes] = None
        DataLenByte: bytes                              = None

        for Input in InputList:
            Buffer     = self.__GetByteArrayBuffer (Input)
            BufferSize = len (Buffer)

            if not IsUint32 (BufferSize):
                raise ValueError (f'Buffer size is larger than UINT32.')

            Remainder = BufferSize % 4
            if Remainder != 0:
                DEBUG (
                  DEBUG_TRACE,
                  f'Buffer not 4-byte alignment ({BufferSize}), padding it.'
                  )
                Buffer += bytearray ([0x00] * (4 - Remainder))

            DataLenByte = len (Buffer).to_bytes (4, byteorder = BYTE_ORDER_BIG)

            Result.append (DataLenByte + Buffer)

        return Result

    def __GetXdrBuffer (self) -> ByteBuffer:
        """ Get the ByteBuffer wrapper of XDR format buffer.

        Note: Inner function to merge the list of byte into one.

        Args:
            None.

        Raises:
            None.

        Returns:
            ByteBuffer:
                The XDR format buffer be wrapper by ByteBuffer.
        """
        Buffer   : bytearray = bytearray ()
        ByteArray: bytearray = None

        for ByteArray in self.__InputList:
            Buffer += ByteArray

        return ByteBuffer (Buffer = Buffer)

    def Save (self, OutputPath: str) -> None:
        """ Save the buffer into specific path.

        Args:
            OutputPath (str):
                The assigned output path.

        Raises:
            None.

        Returns:
            None.
        """
        Path: str = AbsPath (OutputPath)

        self.__XdrBuffer.Save (Path)

if __name__ == '__main__':
    def GetArgs () -> argparse.Namespace:
        """ Get the user input arguments.

        Args:
            None.

        Raises:
            None.

        Returns:
            Args [argparse.Namespace]:
                The argument object parsing from user input.
        """
        Parser: argparse.ArgumentParser = argparse.\
            ArgumentParser (
              prog            = __prog__,
              description     = __copyright__ + __description__ + __version__,
              formatter_class = argparse.RawTextHelpFormatter,
              )
        Parser.add_argument (
                 '-V',
                 '-v',
                 '--Version',
                 action  = 'version',
                 version = __version__,
                 )
        Parser.add_argument (
                 '-i',
                 '-I',
                 '--input',
                 dest     = 'Input',
                 help     = 'Input of binary file. (None as empty one)',
                 action   = 'append',
                 required = True,
                 )
        Parser.add_argument (
                 '-o',
                 '-O',
                 '--output',
                 dest     = 'Output',
                 help     = 'Output of XDR format file.',
                 default  = DEFAULT_XDR_OUTPUT_FILE_NAME,
                 required = False,
                 )

        return Parser.parse_args ()

    Args: argparse.Namespace = GetArgs ()

    Input : List[str] = Args.Input
    Output: str       = Args.Output

    Builder: XdrBuilder = XdrBuilder (InputList = Input)
    Builder.Save (Output)
