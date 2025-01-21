## @file
#  Parse the BiosGuard Update Package (BGUP) header information from
#  binary file.
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

from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

from CapsuleCommon.Intel.BiosGuard.BgupHdrLib import *

DEFAULT_BGUP_HDR_OFFSET: int = 0x0000_0000

class BgupHdrParser (object):
    def __init__ (self, Buffer: ByteBuffer) -> None:
        """ Class used to parse the BGUP information from buffer.

        Args:
            Buffer (ByteBuffer):
                Wrapper of ByteBuffer input.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer     : ByteBuffer = Buffer
        self.__FwImageInfo: dict       = self.__GetFwImageInfo ()
        self.__BgupHeader : BgupHdr    = BgupHdr (**self.__FwImageInfo)

    @property
    def Header (self) -> BgupHdr:
        """ BGUP header within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            BiosIdHdr:
                The header of this BGUP.
                (Should be BgupHdr object)
        """
        return self.__BgupHeader

    def __GetFwImageInfo (self) -> dict:
        """ Get the BGUP header information.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                The header of this BGUP.
        """
        BeginOffset  : int       = DEFAULT_BGUP_HDR_OFFSET
        EndOffset    : int       = BeginOffset + BGUP_HDR_VER2_BYTE_SIZE
        BgupHdrBuffer: bytearray = self.__Buffer.Buffer[BeginOffset:EndOffset]
        BgupHdrInfo  : Struct    = None

        BgupHdrInfo = Struct (
                        Buffer     = BgupHdrBuffer,
                        FormatDict = BGUP_HDR_VER2_FORMAT_DICT,
                        ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                        )

        return BgupHdrInfo.Data
