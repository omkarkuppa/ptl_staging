## @file
#  Parse the BtGAcm firmware image header information from binary file.
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

from CapsuleCommon.Intel.BtGAcm.BtGAcmHdrLib import *

AIT_VERSION_7: int = 0x7
AIT_VERSION_9: int = 0x9

class BtGAcmVersion (object):
    def __init__ (
        self,
        AitHdrVer: int,
        MajorVer : int,
        MinorVer : int,
        BuildVer : int,
        ) -> None:
        """ Class to represent the BtGAcm version.

        Args:
            AitHdrVer (int):
                Ait header version.
            MajorVer (int):
                Major revision of the BtGAcm buffer.
            MinorVer (int):
                Minor revision of the BtGAcm buffer.
            BuildVer (int):
                Build revision of the BtGAcm buffer.

        Raises:
            None.

        Returns:
            None.
        """
        self.__AitHdrVer: int = AitHdrVer
        self.__MajorVer : int = MajorVer
        self.__MinorVer : int = MinorVer
        self.__BuildVer : int = BuildVer

        self.__PreCheck ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Ait header version not int type.
                (2) Major version not int type.
                (3) Minor version not int type.
                (4) Build version not int type.
            ValueError:
                (1) Unsupported Ait header version.
                (2) Major version not UINT8.
                (3) Minor version not UINT8.
                (4) Build version not UINT8 (ver.7) nor UINT16 (ver.9).

        Returns:
            None.
        """
        if not isinstance (self.__AitHdrVer, int):
            raise TypeError ('Ait header version should be int type.')
        elif not isinstance (self.__MajorVer, int):
            raise TypeError ('Major version should be int type.')
        elif not isinstance (self.__MinorVer, int):
            raise TypeError ('Minor version should be int type.')
        elif not isinstance (self.__BuildVer, int):
            raise TypeError ('Build version should be int type.')

        if self.__AitHdrVer not in [AIT_VERSION_7, AIT_VERSION_9]:
            raise ValueError (
                    f'Unsupported Ait header version. [{self.__AitHdrVer}]'
                    )
        elif not IsUint8 (self.__MajorVer):
            raise ValueError (
                    f'Major version should be UINT8. [{self.__MajorVer}]'
                    )
        elif not IsUint8 (self.__MinorVer):
            raise ValueError (
                    f'Minor version should be UINT8. [{self.__MinorVer}]'
                    )

        if self.__AitHdrVer == AIT_VERSION_9:
            if not IsUint16 (self.__BuildVer):
                raise ValueError (
                        f'Build version should be UINT16. [{self.__BuildVer}]'
                        )
        elif self.__AitHdrVer == AIT_VERSION_7:
            if not IsUint8 (self.__BuildVer):
                raise ValueError (
                        f'Build version should be UINT8. [{self.__BuildVer}]'
                        )

    @property
    def Major (self) -> int:
        """ Return the major revision of the BtGAcm buffer. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Major revision of the BtGAcm buffer.
        """
        return self.__MajorVer

    @property
    def Minor (self) -> int:
        """ Return the minor revision of the BtGAcm buffer. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Minor revision of the BtGAcm buffer.
        """
        return self.__MinorVer

    @property
    def Build (self) -> int:
        """ Return the build revision of the BtGAcm buffer.

        Note:
            For ver.7, it should be UINT8. (1 byte)
            For ver.9, it should be UINT16. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Build revision of the BtGAcm buffer.
        """
        return self.__BuildVer

class BtGAcmFwHdrParser (object):
    def __init__ (
        self,
        Buffer: ByteBuffer,
        ) -> None:
        """ Class used to parse the BtGAcm information from buffer.

        Args:
            Buffer (ByteBuffer):
                Wrapper of ByteBuffer input.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer: ByteBuffer = Buffer

        #
        # ACM Information Table.
        #
        self.__AcmInfoTblInfo : dict         = self.__GetAcmInfoTblInfo ()
        self.__AcmInfoTable   : AcmInfoTable = \
            AcmInfoTable (**self.__AcmInfoTblInfo)
        self.__AcmInfoTableVer: int          = \
            HexToDec (self.__AcmInfoTable.AitVersion)

        #
        # ACM Version Information Table
        #
        self.__AcmVerInfoTblInfo: dict            = None
        self.__AcmVerInfoTable  : AcmVerInfoTable = None
        if self.__AcmInfoTableVer >= AIT_VERSION_9:
            self.__AcmVerInfoTblInfo = self.__GetAcmVerInfoTblInfo ()
            self.__AcmVerInfoTable   = \
                AcmVerInfoTable (**self.__AcmVerInfoTblInfo)

        #
        # Get the ACM revision.
        #
        self.__AcmVersion: BtGAcmVersion = self.__GetAcmVersion ()

    @property
    def AitHdrVersion (self) -> int:
        """ Return the ACM information table header version.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Version of ACM information table header.
        """
        return self.__AcmInfoTableVer

    @property
    def AcmInfoTable (self) -> AcmInfoTable:
        """ Return ACM information table within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            AcmInfoTable:
                The ACM information table of this buffer.
                (Should be AcmInfoTable object)
        """
        return self.__AcmInfoTable

    @property
    def AcmVerInfoTable (self) -> AcmVerInfoTable:
        """ Return ACM version information table within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            AcmVerInfoTable:
                The ACM version information table of this buffer.
                (Should be AcmVerInfoTable object)
        """
        return self.__AcmVerInfoTable

    @property
    def AcmVersion (self) -> BtGAcmVersion:
        """ Return ACM revision information within this buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            BtGAcmVersion:
                The ACM revision information of this buffer.
                (Should be BtGAcmVersion object)
        """
        return self.__AcmVersion

    def __GetAcmInfoTblGuidLoc (self) -> int:
        """ Find the ACM information table offset from the buffer image.

        Note:
            ACM information table GUID value is ACM_INFO_TBL_GUID.

        Args:
            None.

        Raises:
            ValueError:
                (1) Failed to find the correspond GUID.
                (2) Find more than one GUID matched within buffer.

        Returns:
            int:
                Offset of the ACM information table.
        """
        Loc: List[int] = \
            self.__Buffer.Find (bytearray (ACM_INFO_TBL_GUID.bytes_le))

        if len (Loc) == 0:
            raise ValueError ('Failed to find the ACM information table.')
        elif len (Loc) > 1:
            raise ValueError (f'Find more than one GUID within buffer. {Loc}')
        else:
            DEBUG (DEBUG_INFO, f'Find the ACM information table offset {Loc}')

        return Loc[0]

    def __GetAcmInfoTblInfo (self) -> dict:
        """ Get the ACM information table information.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                ACM information table.
                Key   - struct member.
                Value - value of struct member.
        """
        BeginOffset   : int       = self.__GetAcmInfoTblGuidLoc ()
        EndOffset     : int       = BeginOffset + ACM_INFO_TBL_BYTE_SIZE
        AcmInfoBuffer : bytearray = None
        AcmInfoTblInfo: Struct    = None

        AcmInfoBuffer  = self.__Buffer.Buffer[BeginOffset:EndOffset]
        AcmInfoTblInfo = Struct (
                           Buffer     = AcmInfoBuffer,
                           FormatDict = ACM_INFO_TBL_FORMAT_DICT,
                           ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                           )

        return AcmInfoTblInfo.Data

    def __GetAcmVerInfoTblSigLoc (self) -> int:
        """ Find the ACM version information table offset from buffer.

        Note:
            ACM version information table signature is
            ACM_VER_INFO_TBL_SIGNATURE.

        Args:
            None.

        Raises:
            ValueError:
                (1) Failed to find the correspond signature.
                (2) Find more than one signature within buffer.

        Returns:
            int:
                Offset of the ACM information table.
        """
        Loc: List[int] = self.__Buffer.Find (ACM_VER_INFO_TBL_SIGNATURE)

        if len (Loc) == 0:
            raise ValueError (
                'Failed to find the BtGAcm version information table.'
                )
        elif len (Loc) > 1:
            raise ValueError ('Find more than two signature within buffer.')
        else:
            DEBUG (
              DEBUG_INFO,
              f'Find the ACM version information table {Loc}'
              )

        return Loc[0]

    def __GetAcmVerInfoTblInfo (self) -> dict:
        """ Get the ACM version information table information.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                ACM version information table.
                Key   - struct member.
                Value - value of struct member.
        """
        BeginOffset     : int       = self.__GetAcmVerInfoTblSigLoc ()
        EndOffset       : int       = BeginOffset + ACM_VER_INFO_TBL_BYTE_SIZE
        AcmVerInfoBuffer: bytearray = None
        AcmVerInfo      : Struct    = None

        AcmVerInfoBuffer = self.__Buffer.Buffer[BeginOffset:EndOffset]
        AcmVerInfo       = Struct (
                             Buffer     = AcmVerInfoBuffer,
                             FormatDict = ACM_VER_INFO_TBL_FORMAT_DICT,
                             ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                             )

        return AcmVerInfo.Data

    def __GetAcmVersion (self) -> BtGAcmVersion:
        """ Return the ACM revision information.

        Args:
            None.

        Raises:
            ValueError:
                Unsupported version of ACM information table.

        Returns:
            BtGAcmVersion:
                The ACM revision information of this buffer.
                (Should be BtGAcmVersion object)
        """
        VerString : str           = None
        AcmVersion: BtGAcmVersion = None

        if self.__AcmInfoTableVer >= AIT_VERSION_9:
            VerString  = self.__AcmVerInfoTable.AcmRevision
            AcmVersion = BtGAcmVersion (
                           AitHdrVer = AIT_VERSION_9,
                           MajorVer  = HexToDec (VerString[6:8]),
                           MinorVer  = HexToDec (VerString[4:6]),
                           BuildVer  = HexToDec (VerString[0:4]),
                           )
        elif self.__AcmInfoTableVer >= AIT_VERSION_7:
            VerString  = self.__AcmInfoTable.Offset37
            AcmVersion = BtGAcmVersion (
                           AitHdrVer = AIT_VERSION_7,
                           MajorVer  = HexToDec (VerString[0:2]),
                           MinorVer  = HexToDec (VerString[2:4]),
                           BuildVer  = HexToDec (VerString[4:6]),
                           )
        else:
            raise ValueError (
                    'Unsupported ACM information table version. '
                    f'[{self.__AcmInfoTableVer}]'
                    )

        return AcmVersion
