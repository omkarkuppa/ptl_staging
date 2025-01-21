## @file
#  Parse the uCode firmware image header information from binary file.
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

from CapsuleCommon.Base.ByteLib import *
from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

from CapsuleCommon.Intel.Ucode.UcodeHdrLib import *

UCODE_HEADER_OFFSET: int = 0x0000_0000
UCODE_FILE_NAME    : str = 'm_{ProcessorFlag}_{ProcessSig}_{UpdateRev}.{Ext}'
UCODE_DEFAULT_EXT  : str = 'pdb'

class UcodeFwHdrParser (object):
    def __init__ (self, Buffer: ByteBuffer) -> None:
        """ Class used to parse the uCode header from buffer.

        Args:
            Buffer (ByteBuffer):
                Wrapper of ByteBuffer input.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer         : ByteBuffer = Buffer
        self.__FwImageInfo    : dict       = self.__GetFwImageInfo ()
        self.__UcodeHdr       : UcodeFwHdr = UcodeFwHdr (**self.__FwImageInfo)
        self.__IsTotalCheckSum: bool       = self.__TotalCheckSum ()

    @property
    def Header (self) -> UcodeFwHdr:
        """ uCode header within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            UcodeFwHdr:
                The header of this uCode.
                (Should be UcodeFwHdr object)
        """
        return self.__UcodeHdr

    @property
    def FileName (self) -> str:
        """ Get the generic uCode file naming from header. (Lowercase)

        Note:
            (1) If uCode header get corrupted, the filename would not
                guarantee it is corrected.
            (2) Rule of basename is defined within UCODE_FILE_NAME.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Generic uCode naming.
        """
        Result       : str = None
        ProcessorFlag: str = self.Header.ProcessorFlags.lstrip ('0')
        ProcessSig   : str = self.Header.ProcessorSignature.lstrip ('0')
        UpdateRev    : str = self.Header.UpdateRevision

        Result = UCODE_FILE_NAME.format (
                                   ProcessorFlag = ProcessorFlag,
                                   ProcessSig    = ProcessSig,
                                   UpdateRev     = UpdateRev,
                                   Ext           = UCODE_DEFAULT_EXT,
                                   ).lower ()

        return Result

    @property
    def Valid (self) -> bool:
        """ Check if the image is valid uCode.

        Note:
            Check the total bytes Sum32 is 0.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - Input uCode buffer is valid.
                False - Input uCode buffer is not valid.
        """
        return self.__IsTotalCheckSum

    def __GetFwImageInfo (self) -> dict:
        """ Get the uCode header information.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                The header of this uCode.
        """
        BeginOffset   : int       = UCODE_HEADER_OFFSET
        EndOffset     : int       = BeginOffset + UCODE_FW_HDR_BYTE_SIZE
        UcodeHdrBuffer: bytearray = self.__Buffer.Buffer[BeginOffset:EndOffset]
        UcodeHdrInfo  : Struct    = None

        UcodeHdrInfo = Struct (
                         Buffer     = UcodeHdrBuffer,
                         FormatDict = UCODE_FW_HEADER_FORMAT_DICT,
                         ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                         )

        return UcodeHdrInfo.Data

    def __TotalCheckSum (self) -> None:
        """ Internal function to calculate the total checksum.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        TotalSize  : int       = HexToDec (self.__UcodeHdr.TotalSize)
        StartOffset: int       = UCODE_HEADER_OFFSET
        EndOffset  : int       = StartOffset + TotalSize
        Buffer     : bytearray = self.__Buffer.Buffer[StartOffset:EndOffset]

        if not BytesCheckSum32 (Buffer):
            DEBUG (DEBUG_WARN, f'Input uCode binary total checksum failed.')
        else:
            DEBUG (DEBUG_INFO, f'Input uCode binary total checksum passed.')
