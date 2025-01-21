## @file
#  File to parse the FV which ACM located.
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
import copy
import math
import uuid
from Utils import *

#
# typedef struct {
#   EFI_GUID                   Name;           // This GUID is the file name. It is used to uniquely identify the file.
#   EFI_FFS_INTEGRITY_CHECK    IntegrityCheck; // Used to verify the integrity of the file.
#   EFI_FV_FILETYPE            Type;           // Identifies the type of file.
#   EFI_FFS_FILE_ATTRIBUTES    Attributes;     // Declares various file attribute bits.
#   UINT8                      Size[3];        // The length of the file in bytes, including the FFS header.
#   EFI_FFS_FILE_STATE         State;          // Used to track the state of the file throughout the life of the file from creation to deletion.
# } EFI_FFS_FILE_HEADER;
#

STARTUP_AC_MODULE_GUID = uuid.UUID ('{26FDAA3D-B7ED-4714-8509-EECF1593800D}')

EFI_FFS_HEADER_FORMAT_DICT = {
    'Guid1'     : {'Type': 'I'     , 'Size': 4},
    'Guid2'     : {'Type': 'H'     , 'Size': 2},
    'Guid3'     : {'Type': 'H'     , 'Size': 2},
    'Guid4'     : {'Type': 'BB'    , 'Size': 2},
    'Guid5'     : {'Type': 'BBBBBB', 'Size': 6},
    'Header'    : {'Type': 'B'     , 'Size': 1},
    'File'      : {'Type': 'B'     , 'Size': 1},
    'Type'      : {'Type': 'B'     , 'Size': 1},
    'Attributes': {'Type': 'B'     , 'Size': 1},
    'Size'      : {'Type': 'BBB'   , 'Size': 3},
    'State'     : {'Type': 'B'     , 'Size': 1},
}
EFI_FFS_HEADER_STRUCT, EFI_FFS_HEADER_BYTE_SIZE = GetStructAndByteSize (EFI_FFS_HEADER_FORMAT_DICT)

class AcmFfsParser (object):
    def __init__ (self, Base, Buffer, AcmAlignment):
        self.__Base         = Base
        self.__Buffer       = Buffer
        self.__AcmAlignment = AcmAlignment

        self.__AcmFfsHdrPtr    = self.__FindAcmFfsGuidPosition ()
        self.__AcmFfsDataPtr   = self.__Buffer.find (bytearray (STARTUP_AC_MODULE_GUID.bytes_le)) + EFI_FFS_HEADER_BYTE_SIZE

        self.__FfsHeaderBuffer = self.__Buffer[self.__AcmFfsHdrPtr:self.__AcmFfsHdrPtr+EFI_FFS_HEADER_BYTE_SIZE]
        #
        # NOTE: Not support the Acm FFS using EFI_FFS_FILE_HEADER2.
        #
        self.__FfSHeaderData   = StructParser (self.__FfsHeaderBuffer, EFI_FFS_HEADER_STRUCT, EFI_FFS_HEADER_FORMAT_DICT).Get ()
        self.__FfsSize         = self.__GetFfsSize (self.__FfSHeaderData ['Size'])

        self.__AcmFfsBuffer  = self.__Buffer[self.__AcmFfsDataPtr:self.__AcmFfsDataPtr+self.__FfsSize]
        self.__AcmBinCnt     = math.ceil (len (self.__AcmFfsBuffer) / self.__AcmAlignment)
        print (f'There should be {self.__AcmBinCnt} S-ACM in this FV.')

        self.__AcmBufferList = self.__ParseAcmBuffer ()

    def Get (self):
        """ To get the list of S-ACM buffer based on alignment.

        Args:
            None.

        Returns:
            __AcmBufferList list[bytearray]: The list of StartupAcm byte buffer list.

        Raises:
            None.
        """
        return self.__AcmBufferList

    def __FfsHeaderCheckSum (self, Buffer):
        """ Use for checking the FFS header checksum value.

        Args:
            Buffer [bytearray]: The bytearray of the FFS header.

        Returns:
            Value [int]: The checksum value.

        Raises:
            None.
        """
        InputBuffer = bytearray (copy.deepcopy (Buffer))

        #
        # The State and IntegrityCheck.Checksum.File fields are assumed
        # to be zero and the checksum is calculated such that the entire
        # header sums to zero.
        #
        InputBuffer[17] = 0
        InputBuffer[23] = 0

        FfsHeaderChecksum = hex(sum(InputBuffer))

        return FfsHeaderChecksum[-2:]

    def __FindAcmFfsGuidPosition (self):
        """ Find the BtGAcm FFS header based on GUID.

        Args:
            None.

        Returns:
            Ptr [int]: The byte number of this FFS start location.
                       If not found any FFS would exit the program with error code.

        Raises:
            None.
        """
        BufferWrapper = ByteBuffer (ByteArray=self.__Buffer)
        Positions = BufferWrapper.Find (bytearray (STARTUP_AC_MODULE_GUID.bytes_le))

        if len (Positions) == 0:
            print ('!!! Cannot find the BtGAcm FFS GUID !!!')
            sys.exit (1)

        for Ptr in Positions:
            HdrBuffer = BufferWrapper.Get ()[Ptr:Ptr+EFI_FFS_HEADER_BYTE_SIZE]
            CheckSum  = self.__FfsHeaderCheckSum (HdrBuffer)

            if CheckSum == '00':
                print (f'Find the valid Acm FFS Header: 0x{Ptr:X}')
                return Ptr

        print ('!!! Not found the valid BtGAcm FFS !!!')
        sys.exit (1)

    def __GetFfsSize (self, Value):
        """ FFS size information need to do the byte reverse.
            (FFS size = header size part + data size part)

        Args:
            None.

        Returns:
            Value [int]: The size of this FFS in byte.

        Raises:
            None.
        """
        return int (ByteReverse (Value), 16) - EFI_FFS_HEADER_BYTE_SIZE

    def __ParseAcmBuffer (self):
        """ To parse the StartupAcm buffer from input FFS.
            (Base on the input StartupAcm alignment.)

        Args:
            None.

        Returns:
            AcmBufferList List[dict, ...]: The list of StartupAcm buffer.
                                           The format of this dict is base address/size/bytebuffer.

        Raises:
            None.
        """
        AcmBufferList = list ()

        for Cnt in range (self.__AcmBinCnt):
            StartPosition = Cnt * self.__AcmAlignment
            AcmBuffer     = self.__AcmFfsBuffer[StartPosition:StartPosition+self.__AcmAlignment]
            AcmBufferList.append ({
                'Base'  : FormatHex (self.__Base + self.__AcmFfsDataPtr + StartPosition),
                'Size'  : FormatHex (self.__AcmAlignment),
                'Buffer': AcmBuffer
            })

        return AcmBufferList
