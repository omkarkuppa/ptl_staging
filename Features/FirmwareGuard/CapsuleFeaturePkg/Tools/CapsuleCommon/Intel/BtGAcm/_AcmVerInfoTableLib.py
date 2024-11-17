## @file
#  Definition of ACM version information table.
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

from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

#
# typedef struct {
#   UINT32  Id;
#   UINT32  Length;
#   UINT32  Rev;
#   UINT32  AcmInfoTableVersion;
#   UINT32  GenerationVersion;
#   UINT32  CoreVersion;
#   UINT32  AcmRevision; (Major[1-byte].Minor[1-byte].Build[2-byte])
#   UINT32  AcmSecurityVersion;
# } ACM_VERSION_INFO_TABLE;
#

#
# Define of BtGAcm Version information table Structure Member.
#
ACM_VER_INFO_TBL_ID              : str = 'Id'
ACM_VER_INFO_TBL_LENGTH          : str = 'Length'
ACM_VER_INFO_TBL_REVISION        : str = 'Revision'
ACM_VER_INFO_TBL_ACM_INFO_TBL_VER: str = 'AcmInfoTableVersion'
ACM_VER_INFO_TBL_GENERATION_VER  : str = 'GenerationVersion'
ACM_VER_INFO_TBL_CORE_VER        : str = 'CoreVersion'
ACM_VER_INFO_TBL_ACM_REVISION    : str = 'AcmRevision'
ACM_VER_INFO_TBL_ACM_SECURITY_VER: str = 'AcmSecurityVersion'

#
# Pre-defined variable.
#
ACM_VER_INFO_TBL_SIGNATURE = 'LREV'

ACM_VER_INFO_TBL_FORMAT_DICT = {
    ACM_VER_INFO_TBL_ID              : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_VER_INFO_TBL_LENGTH          : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_VER_INFO_TBL_REVISION        : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_VER_INFO_TBL_ACM_INFO_TBL_VER: { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_VER_INFO_TBL_GENERATION_VER  : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_VER_INFO_TBL_CORE_VER        : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_VER_INFO_TBL_ACM_REVISION    : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_VER_INFO_TBL_ACM_SECURITY_VER: { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    }
ACM_VER_INFO_TBL_STRUCT, ACM_VER_INFO_TBL_BYTE_SIZE = \
    GetStructFormatAndByteSize (ACM_VER_INFO_TBL_FORMAT_DICT)
