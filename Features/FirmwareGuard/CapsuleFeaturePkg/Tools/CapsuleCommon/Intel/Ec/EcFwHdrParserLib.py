## @file
#  Parse the EC firmware image header information from binary file.
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

from CapsuleCommon.Intel.Ec.EcHdrLib import *

class EcFwHdrParser (object):
    def __init__ (
        self,
        Buffer: ByteBuffer,
        HdrVer: int = EC_FW_HDR_VER2,
        ) -> None:
        """ Class used to parse the EC header information from buffer.

        Args:
            Buffer (ByteBuffer):
                Wrapper of ByteBuffer input.
            HdrVer (int, optional):
                EC firmware header to be parsed.
                Should be one of EC_FW_HDR_VERSION_LIST.
                Defaults to EC_FW_HDR_VER2.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer     : ByteBuffer = Buffer
        self.__HdrVer     : int        = HdrVer
        self.__SigLocation: List[int]  = self.__GetSigLocation ()
        self.__FwImageInfo: dict       = self.__GetFwImageInfo ()
        self.__EcHdr      : EcFwHdr    = EcFwHdr (**self.__FwImageInfo)

    @property
    def Header (self) -> EcFwHdr:
        """ EC firmware header within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            EcFwHdr:
                The header of this EC firmware header.
                (Should be EcFwHdr object)
        """
        return self.__EcHdr

    @property
    def SigLocation (self) -> List[int]:
        """ Get the signature offset within this buffer image.

        Note:
            The normal case would have 2 signatures.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[int]:
                List of EC firmware header signature offset.
        """
        return self.__SigLocation

    def __GetSigLocation (self) -> List[int]:
        """ Find the EC firmware header offset from the buffer image.

        Args:
            None.

        Raises:
            ValueError:
                (1) Signature is not found.
                (2) Number of signature is more than expected.

        Returns:
            List[int]:
                List of EC firmware header signature offset.
        """
        Loc: List[int] = self.__Buffer.Find (EC_FW_SIGNATURE)

        if len (Loc) == 0:
            raise ValueError (f'EC firmware signature is not found.')
        elif len (Loc) == 1:
            DEBUG (DEBUG_WARN, f'Only find one EC FW signature!')
        elif len (Loc) == 2:
            DEBUG (DEBUG_INFO, f'Find two EC firmware signature!')
        else:
            raise ValueError (f'EC firmware signatures are more than expect.')

        return Loc

    def __GetFwImageInfo (self) -> dict:
        """ Get the EC firmware header information.

        Args:
            None.

        Raises:
            ValueError:
                Unsupported EC firmware header version.

        Returns:
            dict:
                The header of this EC firmware.
        """
        BeginOffset  : int       = self.__SigLocation[0]
        EndOffset    : int       = int ()
        EcFWHdrBuffer: bytearray = bytearray ()
        FormatDict   : dict      = dict ()
        EcFwInfo     : Struct    = None

        if self.__HdrVer == EC_FW_HDR_VER1:
            EndOffset  = BeginOffset + EC_FW_HDR_VER1_BYTE_SIZE
            FormatDict = EC_FW_HDR_VER1_FORMAT_DICT
        elif self.__HdrVer == EC_FW_HDR_VER2:
            EndOffset = BeginOffset + EC_FW_HDR_VER2_BYTE_SIZE
            FormatDict = EC_FW_HDR_VER2_FORMAT_DICT
        else:
            raise ValueError (f'Unsupported EC firmware header version.')

        EcFWHdrBuffer = self.__Buffer.Buffer[BeginOffset:EndOffset]

        EcFwInfo = Struct (
                     Buffer     = EcFWHdrBuffer,
                     FormatDict = FormatDict,
                     ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                     )

        return EcFwInfo.Data
