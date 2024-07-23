## @file
#  Parse the FSP boot manifest (FPM) header information from binary file.
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
from CapsuleCommon.Validate.IntegerLib import *
from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

from CapsuleCommon.Intel.Fsp.FbmHdrLib import *

class FspVersion (object):
    def __init__ (
        self,
        MajorVer: int,
        MinorVer: int,
        ) -> None:
        """ Class to represent the FSP version.

        Args:
            MajorVer (int):
                Major version of the FSP firmware.
            MinorVer (int):
                Minor version of the FSP firmware.

        Raises:
            None.

        Returns:
            None.
        """
        self.__MajorVer: int = MajorVer
        self.__MinorVer: int = MinorVer

        self.__PreCheck ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Major version not int type.
                (2) Minor version not int type.
            ValueError:
                (1) Major version not UINT8.
                (2) Minor version not UINT8.

        Returns:
            None.
        """
        if not isinstance (self.__MajorVer, int):
            raise TypeError ('Major version should be int type.')
        elif not isinstance (self.__MinorVer, int):
            raise TypeError ('Minor version should be int type.')

        if not IsUint8 (self.__MajorVer):
            raise ValueError (
                    f'Major version should be UINT8. [{self.__MajorVer}]'
                    )
        elif not IsUint8 (self.__MinorVer):
            raise ValueError (
                    f'Minor version should be UINT8. [{self.__MinorVer}]'
                    )

    @property
    def Major (self) -> int:
        """ Return the major version of the FSP firmware. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Major version of the FSP firmware.
        """
        return self.__MajorVer

    @property
    def Minor (self) -> int:
        """ Return the minor version of the FSP firmware. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Minor version of the FSP firmware.
        """
        return self.__MinorVer

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
        self.__Buffer    : ByteBuffer = Buffer
        self.__ImageInfo : dict       = self.__GetImageInfo ()
        self.__FbmHdr    : FbmHdr     = FbmHdr (**self.__ImageInfo)
        self.__FspVersion: FspVersion = self.__GetFspVersion ()

    @property
    def FspVersion (self) -> FspVersion:
        """ Return FSP version information within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            FspVersion:
                The FSP version information of this buffer.
                (Should be FspVersion object)
        """
        return self.__FspVersion

    def __GetImageInfo (self) -> dict:
        """ Get the FBM header information.

        Args:
            None.

        Raises:
            ValueError:
                Unsupported FBM header version.

        Returns:
            dict:
                The header of FBM.
        """
        BeginOffset : int       = 0
        EndOffset   : int       = FBM_HDR_BYTE_SIZE
        FormatDict  : dict      = FBM_HDR_FORMAT_DICT
        FbmHdrBuffer: bytearray = self.__Buffer.Buffer[BeginOffset:EndOffset]
        FbmInfo     : Struct    = None

        FbmInfo = Struct (
                     Buffer     = FbmHdrBuffer,
                     FormatDict = FormatDict,
                     ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                     )

        return FbmInfo.Data

    def __GetFspVersion (self) -> FspVersion:
        """ Return the FSP version information.

        Args:
            None.

        Raises:
            ValueError:
                Unsupported version of FSP information table.

        Returns:
            FspVersion:
                The FSP version information of this buffer.
                (Should be FspVersion object)
        """
        VerString: str        = None
        FspVer   : FspVersion = None

        VerString = self.__FbmHdr.FspVersion
        FspVer    = FspVersion (
                        MajorVer  = HexToDec (VerString[2:4]),
                        MinorVer  = HexToDec (VerString[0:2]),
                        )
        return FspVer
