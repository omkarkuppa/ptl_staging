## @file
#  API to parse the BtGACM FFS from input binary.
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

import uuid
from typing import List

from BtgAcmMisc.Base._ByteBufferWrapper import *
from BtgAcmMisc.Edk2._Edk2FfsParser import *

#
# Pre-defined Variable.
#
STARTUP_AC_FFS_GUID: uuid.UUID = \
    uuid.UUID ('{26FDAA3D-B7ED-4714-8509-EECF1593800D}')

class BtgAcmFfsParser (object):
    def __init__ (self, Buffer: ByteBuffer) -> None:
        """ Class to provide API to parse the BtGACM FFS.

        Args:
            Buffer (ByteBuffer):
                Input buffer include the BtGACM FFS.

        Raises:
            None.

        Returns:
            None.
        """
        self.__BiosBuffer: ByteBuffer = Buffer

        self.__PreCheck ()

        self.__Offset : int           = None
        self.__FfsInfo: Edk2FfsParser = None

        self.__Offset, self.__FfsInfo = self.__GetOffsetAndFfs ()

    @property
    def DataOffset (self) -> int:
        """ Return the offset of FFS data segment.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Offset of the FFS data segment.
        """
        return self.__Offset + self.__FfsInfo.HdrSize

    @property
    def DataSize (self) -> int:
        """ Return the size of FFS data segment.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Size of the FFS data segment.
        """
        FfsSize: int = self.__FfsInfo.FfsSize
        HdrSize: int = self.__FfsInfo.HdrSize

        return FfsSize - HdrSize

    @property
    def DataBuffer (self) -> ByteBuffer:
        """ Return the buffer of FFS data segment.

        Args:
            None.

        Raises:
            None.

        Returns:
            ByteBuffer:
                Buffer of the FFS data segment.
        """
        Buffer: ByteBuffer = copy.deepcopy (self.__BiosBuffer)
        Buffer.Trim (self.DataOffset, self.DataSize)

        return Buffer

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                Buffer is not ByteBuffer type.

        Returns:
            None.
        """
        if not isinstance (self.__BiosBuffer, ByteBuffer):
            raise TypeError ('Buffer shall be ByteBuffer type.')

    def __GetOffsetAndFfs (self) -> Tuple[int, Edk2FfsParser]:
        """ Return the first valid FFS offset and parser.

        Args:
            None.

        Raises:
            ValueError:
                BtGACM FFS is not located in the input buffer.

        Returns:
            Tuple[int, Edk2FfsParser]:
                int:
                    Offset to the BtGACM FFS.
                Edk2FfsParser:
                    Wrapper to decode the BtGACM FFS header.
        """
        GuidOffsetList: List[int]     = None
        Offset        : int           = None
        Edk2Ffs       : Edk2FfsParser = None

        GuidOffsetList = self.__BiosBuffer.Find (STARTUP_AC_FFS_GUID.bytes_le)

        #
        # Check all the possible GUID offset found.
        #
        for Offset in GuidOffsetList:
            Edk2Ffs = Edk2FfsParser (self.__BiosBuffer, Offset)

            if Edk2Ffs.Valid:
                return Offset, Edk2Ffs

        raise ValueError (f'Not found the BtGACM FFS inside the input buffer.')

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'BtgAcmFfsParser',
    ]
