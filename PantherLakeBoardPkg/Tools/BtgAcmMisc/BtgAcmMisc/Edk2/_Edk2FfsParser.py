## @file
#  Definition of EDK-II FFS header.
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

import copy
from typing import Any, Dict, Union

from BtgAcmMisc.Base._ByteBufferWrapper import *
from BtgAcmMisc.Base._StructWrapper import *

#
# Definition of EDK-II FFS header version.
#
EFI_FFS_HDR_VER_1: int = 0x1
EFI_FFS_HDR_VER_2: int = 0x2

#
# Definition of the EDK-II FFS Structure Member.
#
EFI_FFS_HDR_GUID_1              : str = 'Guid1'
EFI_FFS_HDR_GUID_2              : str = 'Guid2'
EFI_FFS_HDR_GUID_3              : str = 'Guid3'
EFI_FFS_HDR_GUID_4              : str = 'Guid4'
EFI_FFS_HDR_GUID_5              : str = 'Guid5'
EFI_FFS_HDR_INTEGRITY_CHECK_HDR : str = 'Header'
EFI_FFS_HDR_INTEGRITY_CHECK_FILE: str = 'File'
EFI_FFS_HDR_TYPE                : str = 'Type'
EFI_FFS_HDR_ATTRIBUTES          : str = 'Attributes'
EFI_FFS_HDR_SIZE                : str = 'Size'
EFI_FFS_HDR_STATE               : str = 'State'
EFI_FFS_HDR_EXTENDED_SIZE       : str = 'ExtendedSize'

#
# Pre-defined Variable.
#
UINT8_CHECK_SUM_STR                    : str = '00'
EFI_FFS_HDR_SIZE_ZERO_STR              : str = '000000'
EFI_FFS_HDR_INTEGRITY_CHECK_FILE_OFFSET: int = 17
EFI_FFS_HDR_STATE_OFFSET               : int = 23
FFS_ATTRIB_LARGE_FILE                  : int = 0x01

#
# typedef struct {
#   EFI_GUID                   Name;            // This GUID is the file name. It is used to uniquely identify the file.
#   EFI_FFS_INTEGRITY_CHECK    IntegrityCheck;  // Used to verify the integrity of the file.
#   EFI_FV_FILETYPE            Type;            // Identifies the type of file.
#   EFI_FFS_FILE_ATTRIBUTES    Attributes;      // Declares various file attribute bits.
#   UINT8                      Size[3];         // The length of the file in bytes, including the FFS header.
#   EFI_FFS_FILE_STATE         State;           // Used to track the state of the file throughout the life of the file from creation to deletion.
# } EFI_FFS_FILE_HEADER;
#
EFI_FFS_HEADER_FORMAT_DICT = {
    EFI_FFS_HDR_GUID_1              : { FIELD_FORMAT: 'L'     , FIELD_SIZE: 4 },
    EFI_FFS_HDR_GUID_2              : { FIELD_FORMAT: 'H'     , FIELD_SIZE: 2 },
    EFI_FFS_HDR_GUID_3              : { FIELD_FORMAT: 'H'     , FIELD_SIZE: 2 },
    EFI_FFS_HDR_GUID_4              : { FIELD_FORMAT: 'BB'    , FIELD_SIZE: 2 },
    EFI_FFS_HDR_GUID_5              : { FIELD_FORMAT: 'BBBBBB', FIELD_SIZE: 6 },
    EFI_FFS_HDR_INTEGRITY_CHECK_HDR : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    EFI_FFS_HDR_INTEGRITY_CHECK_FILE: { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    EFI_FFS_HDR_TYPE                : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    EFI_FFS_HDR_ATTRIBUTES          : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    EFI_FFS_HDR_SIZE                : { FIELD_FORMAT: 'BBB'   , FIELD_SIZE: 3 },
    EFI_FFS_HDR_STATE               : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    }

EFI_FFS_HEADER_STRUCT   : str = None
EFI_FFS_HEADER_BYTE_SIZE: int = None

EFI_FFS_HEADER_STRUCT, EFI_FFS_HEADER_BYTE_SIZE = \
    GetStructAndByteSize (EFI_FFS_HEADER_FORMAT_DICT)

#
# typedef struct {
#   EFI_GUID                   Name;            // This GUID is the file name. It is used to uniquely identify the file.
#   EFI_FFS_INTEGRITY_CHECK    IntegrityCheck;  // Used to verify the integrity of the file.
#   EFI_FV_FILETYPE            Type;            // Identifies the type of file.
#   EFI_FFS_FILE_ATTRIBUTES    Attributes;      // Declares various file attribute bits.
#   UINT8                      Size[3];         // The length of the file in bytes, including the FFS header.
#   EFI_FFS_FILE_STATE         State;           // Used to track the state of the file throughout the life of the file from creation to deletion.
#   UINT64                     ExtendedSize;    // If FFS_ATTRIB_LARGE_FILE is set in Attributes, then ExtendedSize exists and Size must be set to zero.
# } EFI_FFS_FILE_HEADER2;
#
EFI_FFS_HEADER_2_FORMAT_DICT = {
    EFI_FFS_HDR_GUID_1              : { FIELD_FORMAT: 'L'     , FIELD_SIZE: 4 },
    EFI_FFS_HDR_GUID_2              : { FIELD_FORMAT: 'H'     , FIELD_SIZE: 2 },
    EFI_FFS_HDR_GUID_3              : { FIELD_FORMAT: 'H'     , FIELD_SIZE: 2 },
    EFI_FFS_HDR_GUID_4              : { FIELD_FORMAT: 'BB'    , FIELD_SIZE: 2 },
    EFI_FFS_HDR_GUID_5              : { FIELD_FORMAT: 'BBBBBB', FIELD_SIZE: 6 },
    EFI_FFS_HDR_INTEGRITY_CHECK_HDR : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    EFI_FFS_HDR_INTEGRITY_CHECK_FILE: { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    EFI_FFS_HDR_TYPE                : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    EFI_FFS_HDR_ATTRIBUTES          : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    EFI_FFS_HDR_SIZE                : { FIELD_FORMAT: 'BBB'   , FIELD_SIZE: 3 },
    EFI_FFS_HDR_STATE               : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    EFI_FFS_HDR_EXTENDED_SIZE       : { FIELD_FORMAT: 'Q'     , FIELD_SIZE: 8 },
    }

EFI_FFS_HEADER_2_STRUCT   : str = None
EFI_FFS_HEADER_2_BYTE_SIZE: int = None

EFI_FFS_HEADER_2_STRUCT, EFI_FFS_HEADER_2_BYTE_SIZE = \
    GetStructAndByteSize (EFI_FFS_HEADER_2_FORMAT_DICT)

class Edk2FfsParser (object):
    def __init__ (self, Buffer: ByteBuffer, Offset: int = 0x0) -> None:
        """ Class to decode the EDK-II FFS header.

        Args:
            Buffer (ByteBuffer):
                Buffer to be parsed the EDK-II FFS header.
            Offset (int, optional):
                Offset point to the EDK-II FFS header.
                Defaults to 0x0.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer: ByteBuffer = Buffer
        self.__Offset: int        = Offset

        self.__PreCheck ()

        self.__HdrVer : int  = self.__GetFfsHdrVer ()
        self.__IsValid: bool = self.__IsHdrValid ()
        self.__HdrData: dict = None

        _, self.__HdrData = self.__GetFfsHdrData (self.__HdrVer)

    @property
    def Valid (self) -> bool:
        """ Return the validity of the EDK-II FFS header.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - EDK-II FFS header is valid.
                False - EDK-II FFS header is not valid.
        """
        return self.__IsValid

    @property
    def Version (self) -> Union[None, int]:
        """ Return the version of the FFS header.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, int]:
                Version of the FFS header.
                If FFS header is not valid, return as None.
        """
        if not self.Valid:
            return None

        return self.__HdrVer

    @property
    def HdrSize (self) -> Union[None, int]:
        """ Return the size of FFS header.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, int]:
                Size of the FFS header.
                If FFS header is not valid, return as None.
        """
        if not self.Valid:
            return None

        if self.Version == EFI_FFS_HDR_VER_1:
            return EFI_FFS_HEADER_BYTE_SIZE
        elif self.Version == EFI_FFS_HDR_VER_2:
            return EFI_FFS_HEADER_2_BYTE_SIZE

    @property
    def FfsSize (self) -> Union[None, int]:
        """ Return the size of the entire FFS file including the header.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, int]:
                Size of the entire FFS file including the header.
                If FFS header is not valid, return as None.
        """
        SizeFieldStr: str = None
        SizeHexStr  : str = None

        if not self.Valid:
            return None

        if self.Version == EFI_FFS_HDR_VER_1:
            SizeFieldStr = EFI_FFS_HDR_SIZE
            SizeHexStr   = ByteStrReverse (self.__HdrData[SizeFieldStr])
        elif self.Version == EFI_FFS_HDR_VER_2:
            SizeFieldStr = EFI_FFS_HDR_EXTENDED_SIZE
            SizeHexStr   = self.__HdrData[SizeFieldStr]

        return int (SizeHexStr, 16)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Buffer is not ByteBuffer type.
                (2) Offset is not int type.
            ValueError:
                (1) Offset is not positive value.
                (2) Start offset is not inside of input buffer.
                (3) End offset is not inside of input buffer.

        Returns:
            None.
        """
        if not isinstance (self.__Buffer, ByteBuffer):
            raise TypeError ('Buffer shall be ByteBuffer type.')
        elif not isinstance (self.__Offset, int):
            raise TypeError ('Offset shall be int type.')

        if self.__Offset < 0:
            raise ValueError ('Offset shall be positive value.')
        elif self.__Offset > self.__Buffer.Size:
            raise ValueError ('Start offset is outside of input buffer.')
        elif (self.__Offset + EFI_FFS_HEADER_BYTE_SIZE) > self.__Buffer.Size:
            raise ValueError ('End offset is outside of input buffer.')

    def __GetFfsHdrData (
        self,
        HdrVer: int = EFI_FFS_HDR_VER_1,
        ) -> Tuple[bytearray, Dict[str, str]]:
        """ Get the FFS header data.

        Args:
            HdrVer (int, optional):
                Version of FFS header to be parsed.
                Defaults to EFI_FFS_HDR_VER_1.

        Raises:
            ValueError:
                Unsupported FFS header version.

        Returns:
            Tuple[bytearray, Dict[str, str]]:
                bytearray:
                    Buffer used to parse the FFS header.
                Dict[str, str]:
                    Key-Value FFS header data dictionary.
                    str:
                        Member of struct.
                    str:
                        Value of this member.
        """
        HdrBuffer  : bytearray      = copy.deepcopy (self.__Buffer.Buffer)
        FfsHdrData : dict           = None
        ByteSize   : int            = None
        PyStructStr: str            = None
        FormatData : Dict[str, Any] = None

        #
        # Base on different version of header to parse the structure.
        #
        if HdrVer == EFI_FFS_HDR_VER_1:
            ByteSize    = EFI_FFS_HEADER_BYTE_SIZE
            PyStructStr = EFI_FFS_HEADER_STRUCT
            FormatData  = EFI_FFS_HEADER_FORMAT_DICT
        elif HdrVer == EFI_FFS_HDR_VER_2:
            ByteSize    = EFI_FFS_HEADER_2_BYTE_SIZE
            PyStructStr = EFI_FFS_HEADER_2_STRUCT
            FormatData  = EFI_FFS_HEADER_2_FORMAT_DICT
        else:
            raise ValueError ('Unsupported FFS header version.')

        HdrBuffer  = HdrBuffer[self.__Offset:self.__Offset+ByteSize]
        FfsHdrData = StructWrapper (
                       HdrBuffer,
                       PyStructStr,
                       FormatData,
                       ).Data

        return HdrBuffer, FfsHdrData

    def __GetFfsHdrVer (self) -> int:
        """ Get the FFS header version.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Version of the FFS header.
        """
        FfsHdrData: dict = None
        Attribute : int  = None

        #
        # Decode the header with version 1.
        #
        _, FfsHdrData = self.__GetFfsHdrData (HdrVer = EFI_FFS_HDR_VER_1)
        Attribute     = int (FfsHdrData[EFI_FFS_HDR_ATTRIBUTES], 16)

        #
        # Check the large file attribute is set or not.
        #
        if Attribute & FFS_ATTRIB_LARGE_FILE:
            return EFI_FFS_HDR_VER_2
        else:
            return EFI_FFS_HDR_VER_1

    def __IsHdrValid (self) -> bool:
        """ Check if the FFS header is valid.

        Args:
            None.

        Raises:
            ValueError:
                Unsupported FFS header version.

        Returns:
            bool:
                True  - FFS header is valid.
                False - FFS header is not valid.
        """
        IsValid: bool = False

        if self.__HdrVer == EFI_FFS_HDR_VER_1:
            IsValid = self.__IsHdrVer1Valid ()
        elif self.__HdrVer == EFI_FFS_HDR_VER_2:
            IsValid = self.__IsHdrVer2Valid()
        else:
            raise ValueError ('Unsupported FFS header version.')

        return IsValid

    def __IsHdrVer1Valid (self) -> bool:
        """ Check the version 1 FFS header is valid.

        Note:
            Header checksum shall be 0.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - Version 1 FFS header is valid.
                False - Version 1 FFS header is not valid.
        """
        HdrBuffer: bytearray = None
        CheckSum : str       = None

        HdrBuffer, _ = self.__GetFfsHdrData (EFI_FFS_HDR_VER_1)
        CheckSum     = self.__FfsHeaderCheckSum (HdrBuffer)

        if (CheckSum != UINT8_CHECK_SUM_STR):
            return False

        return True

    def __IsHdrVer2Valid (self) -> bool:
        """ Check the version 2 FFS header is valid.

        Note:
            (1) Header checksum shall be 0.
            (2) Size field shall be 0.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - Version 2 FFS header is valid.
                False - Version 2 FFS header is not valid.
        """
        HdrBuffer: bytearray = None
        HdrData  : dict      = None
        CheckSum : str       = None

        HdrBuffer, HdrData = self.__GetFfsHdrData (EFI_FFS_HDR_VER_2)
        CheckSum           = self.__FfsHeaderCheckSum (HdrBuffer)

        if (CheckSum != UINT8_CHECK_SUM_STR):
            return False

        if HdrData[EFI_FFS_HDR_SIZE] != EFI_FFS_HDR_SIZE_ZERO_STR:
            return False

        return True

    def __FfsHeaderCheckSum (self, HdrBuffer: bytearray) -> str:
        """ Calculate the sum of the FFS header. (UINT8)

        Args:
            HdrBuffer (bytearray):
                EDK-II header buffer to be summed.

        Raises:
            None.

        Returns:
            str:
                Sum of the input buffer in HEX format. (Without prefix.)
        """
        Buffer  : bytearray = bytearray (copy.deepcopy (HdrBuffer))
        Checksum: str       = None

        #
        # The State and IntegrityCheck.Checksum.File fields are assumed
        # to be zero and the checksum is calculated such that the entire
        # header sums to zero.
        #
        Buffer[EFI_FFS_HDR_INTEGRITY_CHECK_FILE_OFFSET] = 0
        Buffer[EFI_FFS_HDR_STATE_OFFSET]                = 0

        Checksum = hex (sum (Buffer))

        return Checksum[-2:]
