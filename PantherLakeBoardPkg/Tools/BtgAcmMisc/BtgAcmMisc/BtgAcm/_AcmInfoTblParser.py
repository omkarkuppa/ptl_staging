## @file
#  Definition of ACM information table.
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
import uuid
from typing import Dict, List, Union

from BtgAcmMisc.Base._ByteBufferWrapper import *
from BtgAcmMisc.Base._StructWrapper import *

#
# typedef struct {
#   UINT32    Data1;
#   UINT16    Data2;
#   UINT16    Data3;
#   UINT8     Data4[8];
# } GUID;
#
# typedef struct {
#   EFI_GUID    Guid;
#   UINT8       ChipsetAcmType;
#   UINT8       AitVersion;
#   UINT16      AitLength;
#   UINT32      ChipsetIdList;
#   UINT32      OsSinitTblVer;
#   UINT32      MinMleHeaderVer;
#   UINT32      SinitCaps;
#   UINT8       SinitVersion;     # Ver.9 clear this as Reserved.
#   UINT8       AitRevision[3];   # Ver.9 clear this as Reserved.
#   UINT32      ProcessorIdList;
#   UINT32      TpmInfoList;
# } ACM_INFO_TABLE;
#

#
# Define of GUID Structure Member.
#
GUID_SEGMENT_1: str = 'Guid1'
GUID_SEGMENT_2: str = 'Guid2'
GUID_SEGMENT_3: str = 'Guid3'
GUID_SEGMENT_4: str = 'Guid4'
GUID_SEGMENT_5: str = 'Guid5'

#
# Define of BtGAcm Information Table Structure Member.
#
ACM_INFO_TBL_CHIPSET_ACM_TYPE  : str = 'ChipsetAcmType'
ACM_INFO_TBL_AIT_VERSION       : str = 'AitVersion'
ACM_INFO_TBL_AIT_LENGTH        : str = 'AitLength'
ACM_INFO_TBL_CHIPSET_ID_LIST   : str = 'ChipsetIdList'
ACM_INFO_TBL_OS_SINIT_TBL_VER  : str = 'OsSinitTblVer'
ACM_INFO_TBL_MIN_MLE_HEADER_VER: str = 'MinMleHeaderVer'
ACM_INFO_TBL_SINIT_CAPS        : str = 'SinitCaps'
ACM_INFO_TBL_OFFSET_36         : str = 'Offset36'
ACM_INFO_TBL_OFFSET_37         : str = 'Offset37'
ACM_INFO_TBL_PROCESSOR_ID_LIST : str = 'ProcessorIdList'
ACM_INFO_TBL_TPM_INFO_LIST     : str = 'TpmInfoList'

#
# Pre-defined Variable.
#
ACM_INFO_TBL_GUID: uuid.UUID = \
    uuid.UUID ('{7FC03AAA-46A7-18DB-2EAC-698F8D417F5A}')

#
# BtGAcm Information Table Header.
#
ACM_INFO_TBL_FORMAT_DICT = {
    GUID_SEGMENT_1                 : { FIELD_FORMAT: 'L'     , FIELD_SIZE: 4 },
    GUID_SEGMENT_2                 : { FIELD_FORMAT: 'H'     , FIELD_SIZE: 2 },
    GUID_SEGMENT_3                 : { FIELD_FORMAT: 'H'     , FIELD_SIZE: 2 },
    GUID_SEGMENT_4                 : { FIELD_FORMAT: 'BB'    , FIELD_SIZE: 2 },
    GUID_SEGMENT_5                 : { FIELD_FORMAT: 'BBBBBB', FIELD_SIZE: 6 },
    ACM_INFO_TBL_CHIPSET_ACM_TYPE  : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    ACM_INFO_TBL_AIT_VERSION       : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    ACM_INFO_TBL_AIT_LENGTH        : { FIELD_FORMAT: 'H'     , FIELD_SIZE: 2 },
    ACM_INFO_TBL_CHIPSET_ID_LIST   : { FIELD_FORMAT: 'L'     , FIELD_SIZE: 4 },
    ACM_INFO_TBL_OS_SINIT_TBL_VER  : { FIELD_FORMAT: 'L'     , FIELD_SIZE: 4 },
    ACM_INFO_TBL_MIN_MLE_HEADER_VER: { FIELD_FORMAT: 'L'     , FIELD_SIZE: 4 },
    ACM_INFO_TBL_SINIT_CAPS        : { FIELD_FORMAT: 'L'     , FIELD_SIZE: 4 },
    ACM_INFO_TBL_OFFSET_36         : { FIELD_FORMAT: 'B'     , FIELD_SIZE: 1 },
    ACM_INFO_TBL_OFFSET_37         : { FIELD_FORMAT: 'BBB'   , FIELD_SIZE: 3 },
    ACM_INFO_TBL_PROCESSOR_ID_LIST : { FIELD_FORMAT: 'L'     , FIELD_SIZE: 4 },
    ACM_INFO_TBL_TPM_INFO_LIST     : { FIELD_FORMAT: 'L'     , FIELD_SIZE: 4 },
    }

ACM_INFO_TBL_STRUCT   : str = None
ACM_INFO_TBL_BYTE_SIZE: int = None

ACM_INFO_TBL_STRUCT, ACM_INFO_TBL_BYTE_SIZE = \
    GetStructAndByteSize (ACM_INFO_TBL_FORMAT_DICT)

class AcmInfoTblParser (object):
    def __init__ (self, Buffer: ByteBuffer) -> None:
        """ Class to parse the ACM information table from buffer.

        Args:
            Buffer (ByteBuffer):
                Input ACM Buffer to be parsed the ACM information table.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer: ByteBuffer = Buffer

        self.__PreCheck ()

        self.__IsValid      : bool       = False
        self.__InfoTblOffset: int        = self.__GetInfoTblOffset ()
        self.__InfoTblBuffer: ByteBuffer = self.__GetInfoTblBuffer ()
        self.__InfoTblData  : dict       = self.__GetInfoTblData ()

    @property
    def Valid (self) -> bool:
        """ Return the validity of the ACM information table.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - ACM information table is valid.
                False - ACM information table is not valid.
        """
        return self.__IsValid

    @property
    def InfoTblData (self) -> Union[None, Dict[str, str]]:
        """ Return the ACM information table data dictionary.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, Dict[str, str]]:
                Key-Value information table data dictionary.
                If ACM information table is not valid, return as None.
                str:
                    Member of struct.
                str:
                    Value of this member.
        """
        if not self.Valid:
            return None

        return self.__InfoTblData

    @property
    def ProcessorIdListOffset (self) -> Union[None, int]:
        """ Return the offset of the processor ID list. (Count field.)

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, int]:
                Offset to the processor ID list Count field.
                If ACM information table is not valid, return as None.
        """
        Offset: int = None

        if not self.Valid:
            return None

        Offset = int (self.InfoTblData[ACM_INFO_TBL_PROCESSOR_ID_LIST], 16)

        return Offset

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                Buffer is not ByteBuffer type.
            ValueError:
                Buffer size is smaller than the info table size.

        Returns:
            None.
        """
        if not isinstance (self.__Buffer, ByteBuffer):
            raise TypeError ('Buffer shall be ByteBuffer type.')

        if self.__Buffer.Size < ACM_INFO_TBL_BYTE_SIZE:
            raise ValueError (
                    f'Buffer size [{self.__Buffer.Size}] is smaller than '
                    f'the ACM_INFO_TBL_BYTE_SIZE [{ACM_INFO_TBL_BYTE_SIZE}].'
                    )

    def __GetInfoTblOffset (self) -> Union[None, int]:
        """ Return the offset of ACM information table.

        Note:
            (1) Search the ACM information table GUID from buffer.
            (2) Expect one GUID value matched found in buffer.
            (3) If more than one, need to review the ACM binary.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Offset to the ACM information table.
                If not found, return as None.
        """
        OffsetList: List[int] = self.__Buffer.Find (ACM_INFO_TBL_GUID.bytes_le)

        if len (OffsetList) == 0:
            return None
        elif len (OffsetList) > 1:
            print ('Warning! Found the ACM Info Table GUID more than 1.')
            return OffsetList[0]
        else:
            return OffsetList[0]

    def __GetInfoTblBuffer (self) -> Union[None, ByteBuffer]:
        """ Return the ACM information table buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, ByteBuffer]:
                ACM information table buffer in ByteBuffer type.
                If information table offset not found, return as None.
        """
        InfoTblBuffer: ByteBuffer = None

        if self.__InfoTblOffset is None:
            return None

        #
        # Trim the buffer to ACM information table size.
        #
        InfoTblBuffer = copy.deepcopy (self.__Buffer)
        InfoTblBuffer.Trim (self.__InfoTblOffset, ACM_INFO_TBL_BYTE_SIZE)

        return InfoTblBuffer

    def __GetInfoTblData (self) -> Union[None, Dict[str, str]]:
        """ Return the ACM information table data.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, Dict[str, str]:
                Key-Value information table data dictionary.
                If information table offset not found, return as None.
                str:
                    Member of struct.
                str:
                    Value of this member.
        """
        InfoTblData: Dict[str, str] = None

        if self.__InfoTblOffset is None:
            return None

        InfoTblData = StructWrapper (
                        Buffer   = self.__InfoTblBuffer.Buffer,
                        Struct   = ACM_INFO_TBL_STRUCT,
                        DataDict = ACM_INFO_TBL_FORMAT_DICT,
                        ).Data

        self.__IsValid = True

        return InfoTblData
