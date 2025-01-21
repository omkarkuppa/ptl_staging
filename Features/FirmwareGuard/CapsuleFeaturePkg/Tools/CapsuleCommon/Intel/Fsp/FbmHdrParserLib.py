## @file
#  Parse the FSP boot manifest (FBM) header information from binary file.
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

from CapsuleCommon.Intel.Fsp.FbmHdrLib import *

class FbmHdrParser (object):
    def __init__ (
        self,
        Buffer: ByteBuffer
        ) -> None:
        """ Class used to parse the FBM header information from buffer.

        Args:
            Buffer (ByteBuffer):
                Wrapper of ByteBuffer input.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer     : ByteBuffer = Buffer
        self.__FbmBaseInfo: dict       = self.__GetFbmBaseInfo ()
        self.__FbmHdr     : FbmHdr     = FbmHdr (**self.__FbmBaseInfo)

    @property
    def Header (self) -> FbmHdr:
        """ Return FBM header information within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            FbmHdr:
                The FBM header information of this buffer.
                (Should be FbmHdr object)
        """
        return self.__FbmHdr

    def __GetFbmBaseInfo (self) -> dict:
        """ Get the base FBM header information.

        Note:
            (1) FBM header is varies based on each size information.
            (2) This function reported the fixed field in the beginning
                of header.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                The header of FBM.
        """
        BeginOffset : int       = 0
        EndOffset   : int       = FBM_HDR_BYTE_SIZE
        FbmHdrBuffer: bytearray = self.__Buffer.Buffer[BeginOffset:EndOffset]
        FbmInfo     : Struct    = None

        FbmInfo = Struct (
                    Buffer     = FbmHdrBuffer,
                    FormatDict = FBM_HDR_FORMAT_DICT,
                    ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                    )

        return FbmInfo.Data
