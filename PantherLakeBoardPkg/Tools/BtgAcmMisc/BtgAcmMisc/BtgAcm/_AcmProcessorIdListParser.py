## @file
#  Definition of processor ID list.
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
from typing import Any, Dict, List, Union

from BtgAcmMisc.Base._ByteBufferWrapper import *
from BtgAcmMisc.Base._StructWrapper import *

#
# typedef struct {
#   UINT32              Count;
#   ACM_PROCESSOR_ID    ProcessorID[1];
# } PROCESSOR_ID_LIST;
#
#
# typedef struct {
#   UINT32    FMS;
#   UINT32    FMSMask;
#   UINT64    PlatformID;
#   UINT64    PlatformMask;
# } ACM_PROCESSOR_ID;
#

#
# Definition of the Processor ID list Structure Member.
# (From the offset of Count field.)
#
ACM_CPU_ID_LIST_CNT     : str = 'Count'
ACM_CPU_ID_FMS          : str = 'FMS'
ACM_CPU_ID_FMS_MASK     : str = 'FMSMask'
ACM_CPU_ID_PLATFORM_ID  : str = 'PlatformID'
ACM_CPU_ID_PLATFORM_MASK: str = 'PlatformMask'

#
# Processor ID List.
#
ACM_CPU_ID_LIST_FORMAT_DICT: Dict[str, Any] = {
    ACM_CPU_ID_LIST_CNT: { FIELD_FORMAT: 'L', FIELD_SIZE:  4 },
    }

ACM_CPU_ID_LIST_STRUCT   : str = None
ACM_CPU_ID_LIST_BYTE_SIZE: int = None

ACM_CPU_ID_LIST_STRUCT, ACM_CPU_ID_LIST_BYTE_SIZE = \
    GetStructAndByteSize (ACM_CPU_ID_LIST_FORMAT_DICT)

#
# Processor ID.
#
ACM_CPU_ID_FORMAT_DICT: Dict[str, Any] = {
    ACM_CPU_ID_FMS          : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_CPU_ID_FMS_MASK     : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_CPU_ID_PLATFORM_ID  : { FIELD_FORMAT: 'Q', FIELD_SIZE: 8 },
    ACM_CPU_ID_PLATFORM_MASK: { FIELD_FORMAT: 'Q', FIELD_SIZE: 8 },
    }

ACM_CPU_ID_STRUCT   : str = None
ACM_CPU_ID_BYTE_SIZE: int = None

ACM_CPU_ID_STRUCT, ACM_CPU_ID_BYTE_SIZE = \
    GetStructAndByteSize (ACM_CPU_ID_FORMAT_DICT)

class AcmFms (object):
    #                        bit
    # Rsvd               : [31:28]
    # Extended Family EAX: [27:20]
    # Extended Model EAX : [19:16]
    # Rsvd               : [15:14]
    # Type EAX           : [13:12]
    # Family EAX         : [11: 8]
    # Model EAX          : [ 7: 4]
    # Stepping EAX       : [ 3: 0]
    #
    def __init__ (self, FmsValueStr: str) -> None:
        """ Class used to represent the FMS within processor ID.

        Args:
            FmsValueStr (str):
                FMS string value in HEX format. (UINT32)

        Raises:
            None.

        Returns:
            None.
        """
        self.__FmsValue      : int = int (FmsValueStr, 16)
        self.__ExtendedFamily: int = (self.__FmsValue >> 20) & 0xFF
        self.__ExtendedModel : int = (self.__FmsValue >> 16) & 0xF
        self.__Type          : int = (self.__FmsValue >> 12) & 0x3
        self.__Family        : int = (self.__FmsValue >> 8) & 0xF
        self.__Model         : int = (self.__FmsValue >> 4) & 0xF
        self.__Stepping      : int = self.__FmsValue & 0xF

    @property
    def Value (self) -> int:
        """ Return the FMS field value.

        Args:
            None.

        Raise:
            None.

        Returns:
            int:
                FMS field value.
        """
        return self.__FmsValue

    @property
    def Family (self) -> str:
        """ Return the Family field value in HEX string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Family field value from input in HEX format.
                (Without prefix.)
        """
        return f'{self.__Family:X}'

    @property
    def ExtFamily (self) -> str:
        """ Return the Extended Family field value in HEX string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Extended Family field value from input in HEX format.
                (Without prefix.)
        """
        return f'{self.__ExtendedFamily:02X}'

    @property
    def Model (self) -> str:
        """ Return the Model field value in HEX string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Model field value from input in HEX format.
                (Without prefix.)
        """
        return f'{self.__Model:X}'

    @property
    def ExtModel (self) -> str:
        """ Return the Extended Model field value in HEX string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Extended Model field value from input in HEX format.
                (Without prefix.)
        """
        return f'{self.__ExtendedModel:X}'

    @property
    def Stepping (self) -> str:
        """ Return the Stepping field value in HEX string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Stepping field value from input in HEX format.
                (Without prefix.)
        """
        return f'{self.__Stepping:X}'

    @property
    def Type (self) -> str:
        """ Return the Type field value in HEX string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Type field value from input in HEX format.
                (Without prefix.)
        """
        return f'{self.__Type:X}'

class AcmProcessorId (object):
    def __init__ (self, **Kwargs) -> None:
        """ Class used to represent the processor ID.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of processor ID structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Fms     : str = Kwargs[ACM_CPU_ID_FMS]
        self.__FmsMask : str = Kwargs[ACM_CPU_ID_FMS_MASK]
        self.__PlatId  : str = Kwargs[ACM_CPU_ID_PLATFORM_ID]
        self.__PlatMask: str = Kwargs[ACM_CPU_ID_PLATFORM_MASK]

    @property
    def FMS (self) -> AcmFms:
        """ Return the FMS field data in processor ID.

        Args:
            None.

        Raises:
            None.

        Returns:
            AcmFms:
                FMS field data in processor ID.
                (Shall be AcmFms type.)
        """
        return AcmFms (self.__Fms)

    @property
    def FMSMask (self) -> AcmFms:
        """ Return the FMSMask field data in processor ID.

        Args:
            None.

        Raises:
            None.

        Returns:
            AcmFms:
                FMSMask field data in processor ID.
                (Shall be AcmFms type.)
        """
        return AcmFms (self.__FmsMask)

    @property
    def PlatformId (self) -> str:
        """ Return the PlatformId field data in processor ID.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                PlatformId field data in HEX format. (UIN64)
        """
        return self.__PlatId

    @property
    def PlatformMask (self) -> str:
        """ Return the PlatformMask field data in processor ID.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                PlatformMask field data in HEX format. (UIN64)
        """
        return self.__PlatMask

class AcmProcessorIdListParser (object):
    def __init__ (self, Buffer: ByteBuffer, Offset: int) -> None:
        """ Class to parse the processor ID list from buffer.

        Args:
            Buffer (ByteBuffer):
                Input ACM Buffer to be parsed the processor ID list.
            Offset (int):
                Offset to the processor ID Count field.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer: ByteBuffer = Buffer
        self.__Offset: int        = Offset

        self.__PreCheck ()

        self.__Count    : int                  = self.__GetListCount ()
        self.__CpuIdData: List[AcmProcessorId] = self.__GetListData ()

    @property
    def Valid (self) -> bool:
        """ Return the validity of the processor ID list.

        Note:
            (1) Implementation is parsed the processor ID list
                from ACM information table.
            (2) Set the valid to TRUE always and not need the addition
                verification.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - Processor ID list is valid.
                False - Processor ID list is not valid.
        """
        return True

    @property
    def Count (self) -> Union[None, int]:
        """ Return the number of processor ID inside the list.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, int]:
                Number of processor ID inside the list.
                If processor ID list is not valid, return as None.
        """
        if not self.Valid:
            return None

        return self.__Count

    @property
    def Data (self) -> Union[None, List[AcmProcessorId]]:
        """ Return the list of processor ID inside the input buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, List[AcmProcessorId]]:
                List of processor ID. (AcmProcessId type.)
                If processor ID list is not valid, return as None.
        """
        if not self.Valid:
            return None

        return self.__CpuIdData

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
                (2) Start offset is not inside the input buffer.
                (3) End offset is not inside the input buffer.

        Returns:
            None.
        """
        if not isinstance (self.__Buffer, ByteBuffer):
            raise TypeError ('Buffer shall be ByteBuffer type.')
        elif not isinstance (self.__Offset, int):
            raise TypeError ('Offset shall be int type.')

        if self.__Offset < 0:
            raise ValueError ('Offset value shall be positive value.')
        elif self.__Offset > self.__Buffer.Size:
            raise ValueError ('Start offset is outside of input Buffer.')
        elif (self.__Offset + ACM_CPU_ID_LIST_BYTE_SIZE) > self.__Buffer.Size:
            raise ValueError ('End offset is outside of input Buffer.')

    def __GetListCount (self) -> int:
        """ Get the number of processor ID inside the list.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Number of processor ID inside the list.
        """
        HdrBuffer: ByteBuffer     = copy.deepcopy (self.__Buffer)
        DataDict : Dict[str, str] = None

        HdrBuffer.Trim (self.__Offset, ACM_CPU_ID_LIST_BYTE_SIZE)

        DataDict = StructWrapper (
                     Buffer   = HdrBuffer.Buffer,
                     Struct   = ACM_CPU_ID_LIST_STRUCT,
                     DataDict = ACM_CPU_ID_LIST_FORMAT_DICT,
                     ).Data

        return int (DataDict[ACM_CPU_ID_LIST_CNT], 16)

    def __GetListData (self) -> List[AcmProcessorId]:
        """ Decode the processor ID items inside the list.

        Args:
            None.

        Raises:
            ValueError:
                (1) Start offset outside of the input buffer.
                (2) End offset outside of the input buffer.

        Returns:
            List[AcmProcessorId]:
                List of processor ID. (AcmProcessId type.)
        """
        DataDict: Dict[str, str]       = None
        DataList: List[AcmProcessorId] = list ()
        Offset  : int                  = \
            self.__Offset + ACM_CPU_ID_LIST_BYTE_SIZE
        Buffer  : ByteBuffer           = None

        if Offset > self.__Buffer.Size:
            raise ValueError ('Start offset outside of input buffer.')
        elif (Offset + ACM_CPU_ID_LIST_BYTE_SIZE * self.__Count) > \
            self.__Buffer.Size:
            raise ValueError ('End offset outside of input buffer.')

        for _ in range (self.__Count):
            Buffer = copy.deepcopy (self.__Buffer)
            Buffer.Trim (Offset, ACM_CPU_ID_BYTE_SIZE)

            DataDict = StructWrapper (
                         Buffer   = Buffer.Buffer,
                         Struct   = ACM_CPU_ID_STRUCT,
                         DataDict = ACM_CPU_ID_FORMAT_DICT,
                         ).Data
            DataList.append (AcmProcessorId (**DataDict))

            Offset += ACM_CPU_ID_BYTE_SIZE

        return DataList
