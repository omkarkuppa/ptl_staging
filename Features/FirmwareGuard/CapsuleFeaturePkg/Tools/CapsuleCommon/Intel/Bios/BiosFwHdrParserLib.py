## @file
#  Parse the BIOS firmware image header information from binary file.
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

from CapsuleCommon.Intel.Bios.BiosIdHdrLib import *

class BiosFwHdrParser (object):
    def __init__ (self, Buffer: ByteBuffer) -> None:
        """ Class used to parse the BIOS ID information from buffer.

        Args:
            Buffer (ByteBuffer):
                Wrapper of ByteBuffer input.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer     : ByteBuffer = Buffer
        self.__SigLocation: List[int]  = self.__GetSigLocation ()
        self.__FwImageInfo: dict       = self.__GetFwImageInfo ()
        self.__BiosId     : BiosIdHdr  = BiosIdHdr (**self.__FwImageInfo)

    @property
    def Header (self) -> BiosIdHdr:
        """ BIOS ID header within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            BiosIdHdr:
                The header of this BIOS ID.
                (Should be BiosIdHdr object)
        """
        return self.__BiosId

    @property
    def SigLocation (self) -> List[int]:
        """ Get the signature offset within this buffer image.

        Note:
            The normal case would only have 1 or 2 signatures.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[int]:
                List of BIOS ID signature offset.
        """
        return self.__SigLocation

    def __GetSigLocation (self) -> List[int]:
        """ Find the BIOS ID offset from the buffer image.

        Args:
            None.

        Raises:
            ValueError:
                (1) Signature is not found.
                (2) Number of signature is more than expected.

        Returns:
            List[int]:
                List of BIOS ID signature offset.
        """
        Loc: List[int] = self.__Buffer.Find (BIOS_ID_SIGNATURE)

        if len (Loc) == 0:
            raise ValueError (f'BIOS ID signature is not found.')
        elif len (Loc) == 1:
            DEBUG (DEBUG_INFO, 'Only find one BIOS ID signature!')
        elif len (Loc) == 2:
            DEBUG (
              DEBUG_INFO,
              f'Find two BIOS ID signature! This is resiliency BIOS contain '
              f'two IBB inside it!'
              )
        else:
            raise ValueError (f'BIOS ID Signature is more than expected.')

        return Loc

    def __GetFwImageInfo (self) -> dict:
        """ Get the BIOS ID header information.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                The header of this BIOS ID.
        """
        BeginOffset : int       = self.__SigLocation[0]
        EndOffset   : int       = BeginOffset + BIOS_FW_HDR_BYTE_SIZE
        BiosIdBuffer: bytearray = self.__Buffer.Buffer[BeginOffset:EndOffset]
        BiosIdInfo  : Struct    = None

        BiosIdInfo = Struct (
                       Buffer     = BiosIdBuffer,
                       FormatDict = BIOS_FW_HEADER_FORMAT_DICT,
                       ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                       )

        return BiosIdInfo.Data
