## @file
#  Parse the Foxville firmware image header information from binary file.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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

from CapsuleCommon.Base.ByteLib import *
from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

from CapsuleCommon.Intel.Foxville.FoxvilleHdrLib import *

FOXVILLE_HEADER_OFFSET: int = 0x0000_0000

class FoxvilleFwHdrParser (object):
    def __init__ (self, Buffer: ByteBuffer) -> None:
        """ Class used to parse the Foxville NVM header information from buffer.

        Args:
            Buffer (ByteBuffer):
                Wrapper of ByteBuffer input.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer     : ByteBuffer    = Buffer
        self.__FwImageInfo: dict          = self.__GetFwImageInfo ()
        self.__FoxvilleHdr: FoxvilleFwHdr = FoxvilleFwHdr (**self.__FwImageInfo)

    @property
    def Header (self) -> FoxvilleFwHdr:
        """ Foxville firmware NVM header within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            FoxvilleFwHdr:
                The header of this Fxoville NVM firmware header.
                (Should be FoxvilleFwHdr object)
        """
        return self.__FoxvilleHdr

    def __GetFwImageInfo (self) -> dict:
        """ Get the Foxville firmware NVM header information.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                The NVM header of this Foxville firmware.
        """
        BeginOffset  : int       = FOXVILLE_HEADER_OFFSET
        EndOffset    : int       = BeginOffset + FXVL_FW_HDR_BYTE_SIZE
        FxvlHdrBuffer: bytearray = self.__Buffer.Buffer[BeginOffset:EndOffset]
        FxvlHdrInfo  : Struct    = None

        FxvlHdrInfo = Struct (
                        Buffer     = FxvlHdrBuffer,
                        FormatDict = FXVL_FW_HDR_FORMAT_DICT,
                        ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                        )

        return FxvlHdrInfo.Data
