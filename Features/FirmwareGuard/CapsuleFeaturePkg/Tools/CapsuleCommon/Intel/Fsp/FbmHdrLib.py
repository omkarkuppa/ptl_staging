## @file
#  Definition of FSP Boot Manifest (FBM) Header.
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

FBM_SIGNATURE       : str = '__FBMS__'
FBM_STRUCT_VERSION_1: int = 0x10

#
# Part of FBM header structure:
#
# typedef struct {
#   CHAR8   StructureId[8];
#   UINT8   StructVersion;
#   UINT8   Reserved1[3];
#   UINT16  KeySignatureOffset;
#   UINT16  FspVersion;
#   UINT8   FspSvn;
#   UINT8   Reserved2;
#   UINT32  Flags;
# } FBM_IMAGE_HEADER;
#
FBM_HDR_STRUCTURE_ID     : str = 'StructureId'
FBM_HDR_STRUCTURE_VERSION: str = 'StructVersion'
FBM_HDR_RESERVED1        : str = 'Reserved1'
FBM_HDR_KEY_SIG_OFFSET   : str = 'KeySignatureOffset'
FBM_HDR_FSP_VERSION      : str = 'FspVersion'
FBM_HDR_FSP_SVN          : str = 'FspSvn'
FBM_HDR_RESERVED2        : str = 'Reserved2'
FBM_HDR_FLAGS            : str = 'Flags'

FBM_HDR_FORMAT_DICT: Dict[str, Any] = {
    FBM_HDR_STRUCTURE_ID     : { FIELD_FORMAT: '8c', FIELD_SIZE: 8 },
    FBM_HDR_STRUCTURE_VERSION: { FIELD_FORMAT: 'B' , FIELD_SIZE: 1 },
    FBM_HDR_RESERVED1        : { FIELD_FORMAT: '3B', FIELD_SIZE: 3 },
    FBM_HDR_KEY_SIG_OFFSET   : { FIELD_FORMAT: 'H',  FIELD_SIZE: 2 },
    FBM_HDR_FSP_VERSION      : { FIELD_FORMAT: '2B', FIELD_SIZE: 2 },
    FBM_HDR_FSP_SVN          : { FIELD_FORMAT: 'B' , FIELD_SIZE: 1 },
    FBM_HDR_RESERVED2        : { FIELD_FORMAT: 'B' , FIELD_SIZE: 1 },
    FBM_HDR_FLAGS            : { FIELD_FORMAT: '4B', FIELD_SIZE: 4 }
    }

FBM_HDR_STRUCT   : str = str ()
FBM_HDR_BYTE_SIZE: int = int ()
FBM_HDR_STRUCT, FBM_HDR_BYTE_SIZE = \
    GetStructFormatAndByteSize (FBM_HDR_FORMAT_DICT)

class FbmHdr (object):
    Signature: str = FBM_SIGNATURE
    StructVer: str = FBM_STRUCT_VERSION_1
    #
    # List of items need to check for input key within dict.
    #
    FBM_HDR_MANDATORY: List[str] = [
        FBM_HDR_STRUCTURE_ID,
        FBM_HDR_STRUCTURE_VERSION,
        FBM_HDR_RESERVED1,
        FBM_HDR_KEY_SIG_OFFSET,
        FBM_HDR_FSP_VERSION,
        FBM_HDR_RESERVED2,
        FBM_HDR_FLAGS
        ]

    def __init__ (self, **Kwargs) -> None:
        """ Class used to represent the FBM header.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of FBM header structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__StructId     : str = Kwargs[FBM_HDR_STRUCTURE_ID]
        self.__StructVersion: str = Kwargs[FBM_HDR_STRUCTURE_VERSION]
        self.__KeySigOffset : str = Kwargs[FBM_HDR_KEY_SIG_OFFSET]
        self.__FspVersion   : str = Kwargs[FBM_HDR_FSP_VERSION]
        self.__FspSvn       : str = Kwargs[FBM_HDR_FSP_SVN]
        self.__Flags        : str = Kwargs[FBM_HDR_FLAGS]

    @property
    def StructId (self) -> str:
        """ Get the structure ID of FBM. (8 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The StructId of FBM.
        """
        return self.__StructId

    @property
    def StructVersion (self) -> str:
        """ Get the structure version of FBM. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The StructVersion of FBM.
        """
        return self.__StructVersion

    @property
    def KeySigOffset (self) -> str:
        """ Get the key signature offset of FBM. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The KeySigOffset of FBM.
        """
        return self.__KeySigOffset

    @property
    def FspVersion (self) -> str:
        """ Get the version of FSP firmware. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The version of FSP firmware.
        """
        return self.__FspVersion

    @property
    def FspSvn (self) -> str:
        """ Get the SVN of FSP firmware. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The SVN of FSP firmware.
        """
        return self.__FspSvn

    @property
    def Flags (self) -> str:
        """ Get the flags of FBM. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The flags of FBM.
        """
        return self.__Flags

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) FBM header structure member is missing.
                (2) FBM header signature is incorrect. (FBM_HDR_STRUCTURE_ID)
                (3) FBM header structure version is incorrect. (FBM_HDR_STRUCTURE_VERSION)

        Returns:
            None.
        """
        InputSig      : str = str ()
        InputStructVer: int = None

        #
        # To check  header information meets the mandatory list.
        #
        for Member in FbmHdr.FBM_HDR_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )

        #
        # To check the parsed signature is correct.
        #
        InputSig = ByteToStr (
                     self.__Kwargs[FBM_HDR_STRUCTURE_ID],
                     IsReverse = False
                     )
        if not (InputSig == FbmHdr.Signature):
            raise ValueError (
                    f'Input signature ({InputSig}) is not '
                    f'{FbmHdr.Signature}'
                    )

        #
        # To check the parsed structure version is correct.
        #
        InputStructVer = HexToDec (self.__Kwargs[FBM_HDR_STRUCTURE_VERSION])

        if not (InputStructVer == FbmHdr.StructVer):
            raise ValueError (
                    f'Input structure version ({InputStructVer}) is not '
                    f'{FbmHdr.StructVer}'
                    )
