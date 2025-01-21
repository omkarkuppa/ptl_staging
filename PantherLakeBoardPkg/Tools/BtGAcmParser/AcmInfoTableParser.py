## @file
#  File to Parse the AcmInfoTable from StartupAcm Binary.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2022 Intel Corporation.
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
# @par Specification
##
import uuid
from Utils import *

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
#   UINT8    SinitVersion;
#   UINT8    AitRevision[3];
#   UINT32   ProcessorIdList;
#   UINT32   TpmInfoList;
# } ACM_INFO_TABLE;
#

ACM_INFO_TABLE_GUID = uuid.UUID ('{7FC03AAA-46A7-18DB-2EAC-698F8D417F5A}')
ACM_INFO_TABLE_HEADER_FORMAT_DICT = {
    'Guid1'          : {'Type': 'I'     , 'Size': 4},
    'Guid2'          : {'Type': 'H'     , 'Size': 2},
    'Guid3'          : {'Type': 'H'     , 'Size': 2},
    'Guid4'          : {'Type': 'BB'    , 'Size': 2},
    'Guid5'          : {'Type': 'BBBBBB', 'Size': 6},
    'ChipsetAcmType' : {'Type': 'B'     , 'Size': 1},
    'AitVersion'     : {'Type': 'B'     , 'Size': 1},
    'AitLength'      : {'Type': 'H'     , 'Size': 2},
    'ChipsetIdList'  : {'Type': 'I'     , 'Size': 4},
    'OsSinitTblVer'  : {'Type': 'I'     , 'Size': 4},
    'MinMleHeaderVer': {'Type': 'I'     , 'Size': 4},
    'SinitCaps'      : {'Type': 'I'     , 'Size': 4},
    'SinitVersion'   : {'Type': 'B'     , 'Size': 1},
    'AitRevision'    : {'Type': 'BBB'   , 'Size': 3},
    'ProcessorIdList': {'Type': 'I'     , 'Size': 4},
    'TpmInfoList'    : {'Type': 'I'     , 'Size': 4},
}
ACM_INFO_TABLE_HEADER_STRUCT, ACM_INFO_TABLE_HEADER_BYTE_SIZE = GetStructAndByteSize (ACM_INFO_TABLE_HEADER_FORMAT_DICT)

class AcmInfoTableParser (object):
    def __init__ (self, Buffer):
        self.__SourceBuffer       = Buffer
        self.__Position           = self.__SourceBuffer.find (bytearray(ACM_INFO_TABLE_GUID.bytes_le))
        self.__AcmInfoTableBuffer = self.__SourceBuffer[self.__Position:self.__Position+ACM_INFO_TABLE_HEADER_BYTE_SIZE]

        if self.__Position == -1:
            # Not found the ACM Info Table GUID, the input binary might not StartupAcm
            print ('!!! Not found the ACM INFORMATION TABLE, Please Check it is StartupAcm Binary !!!')
            self.__AcmInfoTableHeader = None
        else:
            self.__AcmInfoTableHeader = StructParser (self.__AcmInfoTableBuffer, ACM_INFO_TABLE_HEADER_STRUCT, ACM_INFO_TABLE_HEADER_FORMAT_DICT).Get ()

    def Get (self):
        """ To get the ACM information table header data.

        Args:
            None.

        Returns:
            __AcmInfoTableHeader [dict]: Key-Value pair header information.

        Raises:
            None.
        """
        return self.__AcmInfoTableHeader

    def GetProcessorIdListPtr (self):
        """ To get the Processor ID list pointer address from ACM information table header data.

        Args:
            None.

        Returns:
            Result [None|str]: The processor ID list pointer address string without "0x".
                               Related position in StartupAcm image buffer.
                               If not found would return None.

        Raises:
            None.
        """
        if self.__AcmInfoTableHeader is None:
            return None
        else:
            return FormatHex (self.__AcmInfoTableHeader['ProcessorIdList'])
