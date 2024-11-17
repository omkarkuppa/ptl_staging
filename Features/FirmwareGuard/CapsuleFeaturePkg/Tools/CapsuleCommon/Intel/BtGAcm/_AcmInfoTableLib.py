## @file
#  Definition of ACM information table.
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

import uuid

from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

#
# typedef struct {
#   UINT32    Data1;
#   UINT16    Data2;
#   UINT16    Data3;
#   UINT8     Data4[8];
# } GUID;
#
# typedef struct {
#   EFI_GUID Guid;
#   UINT8    ChipsetAcmType;
#   UINT8    AitVersion;
#   UINT16   AitLength;
#   UINT32   ChipsetIdList;
#   UINT32   OsSinitTblVer;
#   UINT32   MinMleHeaderVer;
#   UINT32   SinitCaps;
#   UINT8    SinitVersion;     # Ver.9 clear this as Reserved.
#   UINT8    AitRevision[3];   # Ver.9 clear this as Reserved.
#   UINT32   ProcessorIdList;
#   UINT32   TpmInfoList;
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
# Define of BtGAcm information table Structure Member.
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
# Pre-defined variable.
#
ACM_INFO_TBL_GUID: uuid.UUID = \
    uuid.UUID ('{7FC03AAA-46A7-18DB-2EAC-698F8D417F5A}')

#
# BtGAcm information table Header.
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

ACM_INFO_TBL_STRUCT   : str = str ()
ACM_INFO_TBL_BYTE_SIZE: int = int ()
ACM_INFO_TBL_STRUCT, ACM_INFO_TBL_BYTE_SIZE = \
    GetStructFormatAndByteSize (ACM_INFO_TBL_FORMAT_DICT)
