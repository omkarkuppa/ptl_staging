## @file
#  File to Parse the ProcessID List from StartupAcm Binary.
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
from Utils import *

#
# typedef struct {
#   UINT32           Count;
#   ACM_PROCESSOR_ID ProcessorID[1];
# } PROCESSOR_ID_LIST;
#

ACM_PROCESSOR_ID_LIST_FORMAT_DICT = {
    'Count': {'Type': 'I', 'Size':  4},
}
ACM_PROCESSOR_ID_LIST_HEADER_STRUCT, ACM_PROCESSOR_ID_LIST_HEADER_BYTE_SIZE = GetStructAndByteSize (ACM_PROCESSOR_ID_LIST_FORMAT_DICT)

#
# typedef struct {
#   UINT32     FMS;
#   UINT32     FMSMask;
#   UINT64     PlatformID;
#   UINT64     PlatformMask;
# } ACM_PROCESSOR_ID;
#
ACM_PROCESSOR_ID_FORMAT_DICT = {
    'FMS'         : {'Type': 'I', 'Size': 4},
    'FMSMask'     : {'Type': 'I', 'Size': 4},
    'PlatformID'  : {'Type': 'Q', 'Size': 8},
    'PlatformMask': {'Type': 'Q', 'Size': 8},
}
ACM_PROCESSOR_ID_FORMAT_DICT_HEADER_STRUCT, ACM_PROCESSOR_ID_FORMAT_DICT_HEADER_BYTE_SIZE = GetStructAndByteSize (ACM_PROCESSOR_ID_FORMAT_DICT)

class AcmProcessorIdListParser (object):
    def __init__ (self, Buffer, Position):
        self.__SourceBuffer       = Buffer
        self.__Position           = Position

        self.__ListCountBuffer = self.__SourceBuffer[self.__Position:self.__Position+ACM_PROCESSOR_ID_LIST_HEADER_BYTE_SIZE]
        self.__ListCount       = self.__GetListCount ()
        self.__ListData        = self.__GetListData ()

    def Get (self):
        """ To get the Processor ID list data.

        Args:
            None.

        Returns:
            __ListData [list]: Return the list of dict format. Each dict contain Processor ID information.

        Raises:
            None.
        """
        return self.__ListData

    def __GetListCount (self):
        """ To get the Processor ID list length from header.

        Args:
            None.

        Returns:
            DataDict [int]: The number of Processor ID information contain in this header.

        Raises:
            None.
        """
        DataDict = StructParser (self.__ListCountBuffer, ACM_PROCESSOR_ID_LIST_HEADER_STRUCT, ACM_PROCESSOR_ID_LIST_FORMAT_DICT).Get ()
        return int (DataDict['Count'])

    def __GetListData (self):
        """ To get the Processor ID buffer data list.

        Args:
            None.

        Returns:
            ListData [dict]: The list of processor ID buffer data.

        Raises:
            None.
        """
        ListData = list ()
        Position = self.__Position + ACM_PROCESSOR_ID_LIST_HEADER_BYTE_SIZE

        for Cnt in range (self.__ListCount):
            Buffer   = self.__SourceBuffer[Position:Position+ACM_PROCESSOR_ID_FORMAT_DICT_HEADER_BYTE_SIZE]
            DataDict = StructParser (Buffer, ACM_PROCESSOR_ID_FORMAT_DICT_HEADER_STRUCT, ACM_PROCESSOR_ID_FORMAT_DICT).Get ()
            ListData.append (DataDict)
            Position = Position + ACM_PROCESSOR_ID_FORMAT_DICT_HEADER_BYTE_SIZE
        return ListData
