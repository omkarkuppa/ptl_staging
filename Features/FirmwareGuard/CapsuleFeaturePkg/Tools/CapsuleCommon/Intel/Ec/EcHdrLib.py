## @file
#  Definition of EC Header.
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

from typing import Any, Dict, List

from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

#
# EC firmware header version.
#
EC_FW_HDR_VER1: int = 0x0
EC_FW_HDR_VER2: int = 0x1

EC_FW_HDR_VERSION_LIST: List[int] = [
    EC_FW_HDR_VER1,
    EC_FW_HDR_VER2,
    ]

#
# Define of EC firmware Structure Member.
#
EC_FW_HDR_SIGNATURE  : str = 'Signature'
EC_FW_HDR_PLATFORM_ID: str = 'PlatId'
EC_FW_HDR_MAJOR_VER  : str = 'MajorVer'
EC_FW_HDR_MINOR_VER  : str = 'MinorVer'
EC_FW_HDR_BUILD_VER  : str = 'BuildVer'

#
# Pre-defined string.
#
EC_FW_SIGNATURE: str = 'TKSC'

#
# Part of EC image header structure:
# Version 1
#
# typedef struct {
#   UINT32  Signature;
#   UINT8   PlatId;
#   UINT8   MajorVer;
#   UINT8   MinorVer;
#   UINT8   BuildVer;
# } EC_IMAGE_HEADER_PIECE;
#
EC_FW_HDR_VER1_FORMAT_DICT: Dict[str, Any] = {
    EC_FW_HDR_SIGNATURE  : { FIELD_FORMAT: '4c', FIELD_SIZE: 4 },
    EC_FW_HDR_PLATFORM_ID: { FIELD_FORMAT: 'B',  FIELD_SIZE: 1 },
    EC_FW_HDR_MAJOR_VER  : { FIELD_FORMAT: 'B',  FIELD_SIZE: 1 },
    EC_FW_HDR_MINOR_VER  : { FIELD_FORMAT: 'B',  FIELD_SIZE: 1 },
    EC_FW_HDR_BUILD_VER  : { FIELD_FORMAT: 'B',  FIELD_SIZE: 1 },
    }

EC_FW_HDR_VER1_STRUCT   : str = str ()
EC_FW_HDR_VER1_BYTE_SIZE: int = int ()
EC_FW_HDR_VER1_STRUCT, EC_FW_HDR_VER1_BYTE_SIZE = \
    GetStructFormatAndByteSize (EC_FW_HDR_VER1_FORMAT_DICT)

#
# Part of EC image header structure:
# Version 2
#
# typedef struct {
#   UINT32  Signature;
#   UINT8   MajorVer;
#   UINT8   MinorVer;
#   UINT8   PlatId;
#   UINT8   BuildVer;
# } EC_IMAGE_HEADER_PIECE;
#
EC_FW_HDR_VER2_FORMAT_DICT: Dict[str, Any] = {
    EC_FW_HDR_SIGNATURE  : { FIELD_FORMAT: '4c', FIELD_SIZE: 4 },
    EC_FW_HDR_MAJOR_VER  : { FIELD_FORMAT: 'B',  FIELD_SIZE: 1 },
    EC_FW_HDR_MINOR_VER  : { FIELD_FORMAT: 'B',  FIELD_SIZE: 1 },
    EC_FW_HDR_PLATFORM_ID: { FIELD_FORMAT: 'B',  FIELD_SIZE: 1 },
    EC_FW_HDR_BUILD_VER  : { FIELD_FORMAT: 'B',  FIELD_SIZE: 1 },
    }

EC_FW_HDR_VER2_STRUCT   : str = str ()
EC_FW_HDR_VER2_BYTE_SIZE: int = int ()
EC_FW_HDR_VER2_STRUCT, EC_FW_HDR_VER2_BYTE_SIZE = \
    GetStructFormatAndByteSize (
      EC_FW_HDR_VER2_FORMAT_DICT,
      )

class EcFwHdr (object):
    Signature: str = EC_FW_SIGNATURE
    #
    # List of items need to check for input key within dict.
    #
    EC_FW_HDR_MANDATORY: List[str] = [
        EC_FW_HDR_SIGNATURE,
        EC_FW_HDR_MAJOR_VER,
        EC_FW_HDR_MINOR_VER,
        EC_FW_HDR_PLATFORM_ID,
        EC_FW_HDR_BUILD_VER,
        ]

    def __init__ (self, **Kwargs) -> None:
        """ Class used to represent the EC header.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of EC header structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__MajorVer: str = Kwargs[EC_FW_HDR_MAJOR_VER]
        self.__MinorVer: str = Kwargs[EC_FW_HDR_MINOR_VER]
        self.__PlatId  : str = Kwargs[EC_FW_HDR_PLATFORM_ID]
        self.__BuildVer: str = Kwargs[EC_FW_HDR_BUILD_VER]

    @property
    def MajorVer (self) -> str:
        """ Get the major version of EC firmware. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The major version of EC firmware header.
        """
        return self.__MajorVer

    @property
    def MinorVer (self) -> str:
        """ Get the minor version of EC firmware. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The minor version of EC firmware header.
        """
        return self.__MinorVer

    @property
    def BuildVer (self) -> str:
        """ Get the build version of EC firmware. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The build version of EC firmware header.
        """
        return self.__BuildVer

    @property
    def PlatformId (self) -> str:
        """ Get the platform ID of EC firmware. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The platform ID of EC firmware header.
        """
        return self.__PlatId

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) EC header structure member is missing.
                (2) EC header signature is incorrect. (EC_FW_SIGNATURE)

        Returns:
            None.
        """
        InputSig: str = str ()
        #
        # To check header information is meet the mandatory list.
        #
        for Member in EcFwHdr.EC_FW_HDR_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )

        #
        # To check the parsed signature is correct.
        #
        InputSig = ByteToStr (
                     self.__Kwargs[EC_FW_HDR_SIGNATURE],
                     IsReverse = False
                     )
        if not (InputSig == EcFwHdr.Signature):
            raise ValueError (
                    f'Input signature ({InputSig}) is not '
                    f'{EcFwHdr.Signature}'
                    )
