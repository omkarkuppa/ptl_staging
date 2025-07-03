## @file
#  Parse the GbE NVM header information from binary file.
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

from CapsuleCommon.Base.ByteLib import *
from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

from CapsuleCommon.Intel.GbE.GbENvmHdrLib import *

GBE_NVM_HEADER_OFFSET: int = 0x0000_0000

class GbeNvmHdrParser (object):
    def __init__ (self, Buffer: ByteBuffer) -> None:
        """ Class used to parse the GbE NVM header information from buffer.

        Args:
            Buffer (ByteBuffer):
                Wrapper of ByteBuffer input.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer      : ByteBuffer = Buffer
        self.__ImageHdrInfo: dict       = self.__GetImageHeaderInfo ()
        self.__GbeNvmHdr   : GbeNvmHdr  = GbeNvmHdr (**self.__ImageHdrInfo)

    @property
    def Header (self) -> GbeNvmHdr:
        """ GbE NVM header within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            GbeNvmHdr:
                The header of this GbE NVM.
                (Should be GbeNvmHdr object)
        """
        return self.__GbeNvmHdr

    def __GetImageHeaderInfo (self) -> dict:
        """ Get the GbE NVM image header information.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                The header of this GbE NVM.
        """
        BeginOffset    : int       = GBE_NVM_HEADER_OFFSET
        EndOffset      : int       = BeginOffset + GBE_NVM_HDR_BYTE_SIZE
        GbeNvmHdrBuffer: bytearray = self.__Buffer.Buffer[BeginOffset:EndOffset]
        GbeNvmHdrInfo  : Struct    = None

        GbeNvmHdrInfo = Struct (
                          Buffer     = GbeNvmHdrBuffer,
                          FormatDict = GBE_NVM_HDR_FORMAT_DICT,
                          ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                          )

        return GbeNvmHdrInfo.Data
