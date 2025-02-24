## @file
#  Padding the BtGACM binary to meet the assigned slot size.
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
#  @par Specification
#
##

import os
import pathlib
from typing import List, Union

from BtgAcmMisc.Base._ByteBufferWrapper import *
from BtgAcmMisc.Base._Utils import *

#
# Define
#
BTG_ACM_BIN_EXT : str       = '.bin'
BTG_ACM_EXT_LIST: List[str] = [
    BTG_ACM_BIN_EXT,
    ]

BTG_ACM_PADDING_CHAR: str = b'\xFF'

class BtgAcmPadding (object):
    def __init__ (
        self,
        BtgAcmBasePath: Union[str, os.PathLike],
        SlotSize      : int,
        ) -> None:
        """ Class support to padding the BtGACM with slot size.

        Args:
            BtgAcmBasePath (Union[str, os.PathLike]):
                Path to the directory which place the BtGACM binary files.
            SlotSize (int):
                Slot size of each BtGACM binary file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__BtgAcmBasePath: str = str (os.path.realpath (BtgAcmBasePath))
        self.__SlotSize      : int = SlotSize

        self.__PreCheck ()

        self.__DebugTrace ()

    def __DebugTrace (self) -> None:
        """ Print the debug message of this object.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        print (f'BtgAcmBasePath: {self.__BtgAcmBasePath}')
        print (f'BtgAcmSlotSize: 0x{self.__SlotSize:08X}')

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) BtgAcmBasePath is not str type.
                (2) SlotSize is not int type.
            ValueError:
                (1) BtgAcmBasePath is not exist.
                (2) BtgAcmBasePath is not directory.

        Returns:
            None.
        """
        if not isinstance (self.__BtgAcmBasePath, str):
            raise TypeError ('BtgAcmBasePath shall be str type.')
        elif not isinstance (self.__SlotSize, int):
            raise TypeError ('SlotSize shall be int type.')

        if not os.path.exists (self.__BtgAcmBasePath):
            raise ValueError (f'BtgAcmBasePath: [{self.__BtgAcmBasePath}] is not exist.')
        elif not os.path.isdir (self.__BtgAcmBasePath):
            raise ValueError (f'BtgAcmBasePath: [{self.__BtgAcmBasePath}] is not directory.')

    def __Padding (
        self,
        FilePath  : Union[str, os.PathLike],
        OutputPath: Union[str, os.PathLike],
        ) -> None:
        """ Padding the BtGACM binary to assigned slot size.

        Note:
            Padding character base on the BTG_ACM_PADDING_CHAR.

        Args:
            FilePath (Union[str, os.PathLike]):
                Path to the input file that needs to be padded.
            OutputPath (Union[str, os.PathLike]):
                Path where the padded file will be written.

        Raises:
            ValueError:
                BtGACM buffer size is larger than slot size.

        Returns:
            None.
        """
        BufferWrapper: ByteBuffer = ByteBuffer (FilePath = FilePath)
        Buffer       : bytearray  = BufferWrapper.Buffer
        BufferSize   : int        = BufferWrapper.Size

        if BufferSize > self.__SlotSize:
            raise ValueError (
                    f'BtGACM buffer size - [{BufferSize}] is larger than '
                    f'assigned slot size - [{self.__SlotSize}]'
                    )

        Buffer = Buffer + BTG_ACM_PADDING_CHAR * (self.__SlotSize - BufferSize)

        TouchDir (os.path.dirname (OutputPath))

        with open (OutputPath, 'wb') as File:
            File.write (Buffer)

    def __GetBtgAcmList (self) -> List[str]:
        """ Get the all possible BtGACM binary from base path.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[str]:
                List of the file path of BtGACM binary.
        """
        Result: List[str] = list ()

        for Root, _, Files in os.walk (self.__BtgAcmBasePath, topdown = False):
            for File in Files:
                if not (pathlib.Path (File).suffix in BTG_ACM_EXT_LIST):
                    continue

                Result.append (os.path.join (Root, File))

        return Result

    def Execute (self, OutputDir: Union[str, os.PathLike]) -> None:
        """ Execute the padding process for all BtGACM binary files.

        Args:
            OutputDir (Union[str, os.PathLike]):
                The output directory where the padded BtGACM binaries
                will be saved.

        Raises:
            None.

        Returns:
            None.
        """
        BtgAcmPathList   : List[str] = self.__GetBtgAcmList ()
        RelBtgAcmPathList: List[str] = list ()
        RelBtgAcmPath    : str       = None
        InputPath        : str       = None
        OutputPath       : str       = None

        #
        # Convert the path to relative one.
        #
        for Path in BtgAcmPathList:
            RelBtgAcmPath = os.path.relpath (Path, self.__BtgAcmBasePath)
            RelBtgAcmPathList.append (RelBtgAcmPath)

        TouchDir (OutputDir)

        #
        # Add the padding to all the possible BtgACM binary.
        # Output the level of the folder as-is.
        #
        for RelBtgAcmPath in RelBtgAcmPathList:
            InputPath  = os.path.join (self.__BtgAcmBasePath, RelBtgAcmPath)
            OutputPath = os.path.join (OutputDir, RelBtgAcmPath)
            self.__Padding (
                   FilePath   = InputPath,
                   OutputPath = OutputPath,
                   )
