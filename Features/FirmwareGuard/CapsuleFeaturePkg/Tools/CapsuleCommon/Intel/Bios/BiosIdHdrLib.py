## @file
#  Definition of BIOS ID.
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

from CapsuleCommon.Edk2.ConstantLib import *
from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

#
# BIOS ID string format:
#
# $(BOARD_ID)$(BOARD_REV).$(BOARD_EXT).$(VERSION_MAJOR).$(BUILD_TYPE)$(VERSION_MINOR).YYMMDDHHMM
#     BoardId_Rev           BoardExt     VersionMajor          BuildType_VerMinor     TimeStamp
#
# Example: "TRFTCRB1.000.0008.D03.1501301017"
#
# typedef struct {
#   CHAR16    BoardId[7];       // "TRFTCRB"
#   CHAR16    BoardRev;         // "1"
#   CHAR16    Dot1;             // "."
#   CHAR16    BoardExt[3];      // "000"
#   CHAR16    Dot2;             // "."
#   CHAR16    VersionMajor[4];  // "0008"
#   CHAR16    Dot3;             // "."
#   CHAR16    BuildType;        // "D"
#   CHAR16    VersionMinor[2];  // "03"
#   CHAR16    Dot4;             // "."
#   CHAR16    TimeStamp[10];    // "YYMMDDHHMM"
#   CHAR16    NullTerminator;   // 0x0000
# } BIOS_ID_STRING;
#

#
# Define of BIOS ID Structure Member.
#
BIOS_ID_HDR_SIGNATURE : str = 'Signature'
BIOS_ID_HDR_BOARD_ID  : str = 'BoardId'
BIOS_ID_HDR_BOARD_REV : str = 'BoardRev'
BIOS_ID_HDR_DOT1      : str = 'Dot1'
BIOS_ID_HDR_BOARD_EXT : str = 'BoardExt'
BIOS_ID_HDR_DOT2      : str = 'Dot2'
BIOS_ID_HDR_MAJOR_VER : str = 'VersionMajor'
BIOS_ID_HDR_DOT3      : str = 'Dot3'
BIOS_ID_HDR_BUILD_TYPE: str = 'BuildType'
BIOS_ID_HDR_MINOR_VER : str = 'VersionMinor'
BIOS_ID_HDR_DOT4      : str = 'Dot4'
BIOS_ID_HDR_TIMESTAMP : str = 'TimeStamp'

#
# Pre-defined string.
#
BIOS_ID_SIGNATURE   : str = '$IBIOSI$'
BIOS_ID_DEBUG_TYPE  : str = 'D'
BIOS_ID_RELEASE_TYPE: str = 'R'

#
# BIOS ID Header.
#
BIOS_FW_HEADER_FORMAT_DICT: Dict[str, Any] = {
    BIOS_ID_HDR_SIGNATURE : { FIELD_FORMAT: '8c' , FIELD_SIZE:  8 },
    BIOS_ID_HDR_BOARD_ID  : { FIELD_FORMAT: '14c', FIELD_SIZE: 14 },
    BIOS_ID_HDR_BOARD_REV : { FIELD_FORMAT: '2c' , FIELD_SIZE:  2 },
    BIOS_ID_HDR_DOT1      : { FIELD_FORMAT: '2c',  FIELD_SIZE:  2 },
    BIOS_ID_HDR_BOARD_EXT : { FIELD_FORMAT: '6c',  FIELD_SIZE:  6 },
    BIOS_ID_HDR_DOT2      : { FIELD_FORMAT: '2c',  FIELD_SIZE:  2 },
    BIOS_ID_HDR_MAJOR_VER : { FIELD_FORMAT: '8c',  FIELD_SIZE:  8 },
    BIOS_ID_HDR_DOT3      : { FIELD_FORMAT: '2c',  FIELD_SIZE:  2 },
    BIOS_ID_HDR_BUILD_TYPE: { FIELD_FORMAT: '2c',  FIELD_SIZE:  2 },
    BIOS_ID_HDR_MINOR_VER : { FIELD_FORMAT: '4c',  FIELD_SIZE:  4 },
    BIOS_ID_HDR_DOT4      : { FIELD_FORMAT: '2c',  FIELD_SIZE:  2 },
    BIOS_ID_HDR_TIMESTAMP : { FIELD_FORMAT: '20c', FIELD_SIZE: 20 },
    }

BIOS_FW_HDR_STRUCT   : str = str ()
BIOS_FW_HDR_BYTE_SIZE: int = int ()
BIOS_FW_HDR_STRUCT, BIOS_FW_HDR_BYTE_SIZE = \
    GetStructFormatAndByteSize (BIOS_FW_HEADER_FORMAT_DICT)

class BiosIdHdr (object):
    Signature: str = BIOS_ID_SIGNATURE
    #
    # List of items need to check for input key within dict.
    #
    BIOS_ID_HDR_MANDATORY: List[str] = [
        BIOS_ID_HDR_SIGNATURE,
        BIOS_ID_HDR_BOARD_ID,
        BIOS_ID_HDR_BOARD_REV,
        BIOS_ID_HDR_BOARD_EXT,
        BIOS_ID_HDR_MAJOR_VER,
        BIOS_ID_HDR_BUILD_TYPE,
        BIOS_ID_HDR_MINOR_VER,
        BIOS_ID_HDR_TIMESTAMP,
        ]

    def __init__ (
        self,
        **Kwargs: Dict[str, Any],
        ) -> None:
        """ Class used to represent the BIOS ID.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of BIOS ID structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__BoardId  : str = Kwargs[BIOS_ID_HDR_BOARD_ID]
        self.__BoardRev : str = Kwargs[BIOS_ID_HDR_BOARD_REV]
        self.__BoardExt : str = Kwargs[BIOS_ID_HDR_BOARD_EXT]
        self.__MajorVer : str = Kwargs[BIOS_ID_HDR_MAJOR_VER]
        self.__MinorVer : str = Kwargs[BIOS_ID_HDR_MINOR_VER]
        self.__BuildType: str = Kwargs[BIOS_ID_HDR_BUILD_TYPE]
        self.__Timestamp: str = Kwargs[BIOS_ID_HDR_TIMESTAMP]

    @property
    def BoardId (self) -> str:
        """ Get the board ID string. (7 characters)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The board ID of BIOS ID header.
        """
        Id: str = ByteToStr (
                    self.__BoardId,
                    Encode    = 'utf-16',
                    IsReverse = False,
                    )

        return Id

    @property
    def BoardRev (self) -> str:
        """ Get the board revision string. (1 character)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The board revision of BIOS ID header.
        """
        Rev: str = ByteToStr (
                     self.__BoardRev,
                     Encode    = 'utf-16',
                     IsReverse = False,
                     )

        return Rev

    @property
    def BoardExt (self) -> str:
        """ Get the board extend string. (3 characters)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The board extend of BIOS ID header.
        """
        Ext: str = ByteToStr (
                     self.__BoardExt,
                     Encode    = 'utf-16',
                     IsReverse = False
                     )

        return Ext

    @property
    def MajorVer (self) -> str:
        """ Get the major version of BIOS. (4 characters)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The major version of BIOS ID header.
        """
        Major: str = ByteToStr (
                       self.__MajorVer,
                       Encode    = 'utf-16',
                       IsReverse = False,
                       )

        return Major

    @property
    def MinorVer (self) -> str:
        """ Get the minor version of BIOS. (2 characters)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The minor version of BIOS ID header.
        """
        Minor: str = ByteToStr (
                       self.__MinorVer,
                       Encode    = 'utf-16',
                       IsReverse = False,
                       )

        return Minor

    @property
    def BuildType (self) -> str:
        """ Get the build type of BIOS.

        Note:
            D - DEBUG.
            R - RELEASE.

        Args:
            None.

        Raises:
            ValueError:
                Build type character is not supported.

        Returns:
            str:
                The build type of BIOS ID header.
        """
        #
        # Debug build or release build is using BoardExt first string.
        #
        Build: str = self.BoardExt[0]

        if Build == BIOS_ID_DEBUG_TYPE:
            return BUILD_TARGET_DEBUG_TYPE
        elif Build == BIOS_ID_RELEASE_TYPE:
            return BUILD_TARGET_RELEASE_TYPE
        else:
            raise ValueError (f'!!! Build type is not supported !!!')

    @property
    def Timestamp (self) -> str:
        """ Get the build timestamp of BIOS. (10 characters)

        Note:
            Format is YYMMDDHHMM.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The build timestamp of BIOS ID header.
        """
        Datetime: str = ByteToStr (
                          self.__Timestamp,
                          Encode    = 'utf-16',
                          IsReverse = False,
                          )

        return Datetime

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) BIOS ID structure member is missing.
                (2) BIOS ID signature is incorrect. (BIOS_ID_SIGNATURE)

        Returns:
            None.
        """
        InputSig: str = str ()
        #
        # To check header information is meet the mandatory list.
        #
        for Member in BiosIdHdr.BIOS_ID_HDR_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )

        #
        # To check the parsed signature is correct.
        #
        InputSig = ByteToStr (
                     self.__Kwargs[BIOS_ID_HDR_SIGNATURE],
                     IsReverse = False,
                     )
        if not (InputSig == BiosIdHdr.Signature):
            raise ValueError (
                    f'Input signature ({InputSig}) is not '
                    f'{BiosIdHdr.Signature}'
                    )
