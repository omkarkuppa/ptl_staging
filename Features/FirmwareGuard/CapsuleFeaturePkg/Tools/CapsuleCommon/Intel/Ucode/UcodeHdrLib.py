## @file
#  Definition of uCode Header.
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

from datetime import date

from CapsuleCommon.Edk2.ConstantLib import *
from CapsuleCommon.Format.HexadecimalLib import *
from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

#
# Define of uCode Header Structure Member.
#
UCODE_FW_HDR_HDR_VER        : str = 'HeaderVersion'
UCODE_FW_HDR_UPDATE_REV     : str = 'UpdateRevision'
UCODE_FW_HDR_DATE           : str = 'Date'
UCODE_FW_HDR_PROCESSOR_SIG  : str = 'ProcessorSignature'
UCODE_FW_HDR_CHECK_SUM      : str = 'Checksum'
UCODE_FW_HDR_LOADER_REV     : str = 'LoaderRevision'
UCODE_FW_HDR_PROCESSOR_FLAGS: str = 'ProcessorFlags'
UCODE_FW_HDR_DATA_SIZE      : str = 'DataSize'
UCODE_FW_HDR_TOTAL_SIZE     : str = 'TotalSize'
UCODE_FW_HDR_RESERVED       : str = 'Reserved'

#
# Microcode Header Format definition
#
# typedef struct {
#   UINT32                             HeaderVersion;
#   UINT32                             UpdateRevision;
#   CPU_MICROCODE_DATE                 Date;                // YY-DD-MM 4 Bytes
#   CPU_MICROCODE_PROCESSOR_SIGNATURE  ProcessorSignature;  // 4 Bytes
#   UINT32                             Checksum;
#   UINT32                             LoaderRevision;
#   UINT32                             ProcessorFlags;
#   UINT32                             DataSize;
#   UINT32                             TotalSize;
#   UINT8                              Reserved[12];
# } CPU_MICROCODE_HEADER;
#

#
# uCode Header.
#
UCODE_FW_HEADER_FORMAT_DICT: Dict[str, Any] = {
    UCODE_FW_HDR_HDR_VER        : {FILED_FORMAT: 'L',   FIELD_SIZE:  4},
    UCODE_FW_HDR_UPDATE_REV     : {FILED_FORMAT: 'L',   FIELD_SIZE:  4},
    UCODE_FW_HDR_DATE           : {FILED_FORMAT: 'L',   FIELD_SIZE:  4},
    UCODE_FW_HDR_PROCESSOR_SIG  : {FILED_FORMAT: 'L',   FIELD_SIZE:  4},
    UCODE_FW_HDR_CHECK_SUM      : {FILED_FORMAT: 'L',   FIELD_SIZE:  4},
    UCODE_FW_HDR_LOADER_REV     : {FILED_FORMAT: 'L',   FIELD_SIZE:  4},
    UCODE_FW_HDR_PROCESSOR_FLAGS: {FILED_FORMAT: 'L',   FIELD_SIZE:  4},
    UCODE_FW_HDR_DATA_SIZE      : {FILED_FORMAT: 'L',   FIELD_SIZE:  4},
    UCODE_FW_HDR_TOTAL_SIZE     : {FILED_FORMAT: 'L',   FIELD_SIZE:  4},
    UCODE_FW_HDR_RESERVED       : {FILED_FORMAT: '12x', FIELD_SIZE: 12},
    }

UCODE_FW_HDR_STRUCT   : str = str ()
UCODE_FW_HDR_BYTE_SIZE: int = int ()
UCODE_FW_HDR_STRUCT, UCODE_FW_HDR_BYTE_SIZE = \
    GetStructFormatAndByteSize (UCODE_FW_HEADER_FORMAT_DICT)

class UcodeFwHdr (object):
    #
    # List of items need to check for input key within dict.
    #
    UCODE_HDR_MANDATORY: List[str] = [
        UCODE_FW_HDR_HDR_VER,
        UCODE_FW_HDR_UPDATE_REV,
        UCODE_FW_HDR_DATE,
        UCODE_FW_HDR_PROCESSOR_SIG,
        UCODE_FW_HDR_CHECK_SUM,
        UCODE_FW_HDR_LOADER_REV,
        UCODE_FW_HDR_PROCESSOR_FLAGS,
        UCODE_FW_HDR_DATA_SIZE,
        UCODE_FW_HDR_TOTAL_SIZE,
        ]

    def __init__ (self, **Kwargs: Dict[str, Any]):
        """ Class used to represent the uCode header.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of uCode header structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__HeaderVer     : str = Kwargs[UCODE_FW_HDR_HDR_VER]
        self.__UpdateRev     : str = Kwargs[UCODE_FW_HDR_UPDATE_REV]
        self.__Date          : str = Kwargs[UCODE_FW_HDR_DATE]
        self.__ProcessorSig  : str = Kwargs[UCODE_FW_HDR_PROCESSOR_SIG]
        self.__CheckSum      : str = Kwargs[UCODE_FW_HDR_CHECK_SUM]
        self.__LoaderRev     : str = Kwargs[UCODE_FW_HDR_LOADER_REV]
        self.__ProcessorFlags: str = Kwargs[UCODE_FW_HDR_PROCESSOR_FLAGS]
        self.__DataSize      : str = Kwargs[UCODE_FW_HDR_DATA_SIZE]
        self.__TotalSize     : str = Kwargs[UCODE_FW_HDR_TOTAL_SIZE]

    @property
    def HeaderVersion (self) -> str:
        """ Get the header version of uCode firmware. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Header version of uCode firmware.
        """
        return self.__HeaderVer

    @property
    def UpdateRevision(self) -> str:
        """ Get the update revision of uCode firmware. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Update revision of uCode firmware.
        """
        return self.__UpdateRev

    @property
    def Date (self) -> date:
        """ Get the build date of uCode. (4 bytes)

        Note:
            Source format is MMDDYYYY.

        Args:
            None.

        Raises:
            None.

        Returns:
            date:
                The build date of uCode within header.
        """
        DateStr: str = FormatHex (
                         self.__Date,
                         IsPrefix    = False,
                         IsPadding   = True,
                         PaddingSize = 8,
                         )

        Year : int = int (DateStr[4:8])
        Month: int = int (DateStr[0:2])
        Day  : int = int (DateStr[2:4])

        return date (Year, Month, Day)

    @property
    def ProcessorSignature (self) -> str:
        """ Get the processor signature of uCode firmware. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Processor signature of uCode firmware.
        """
        return self.__ProcessorSig

    @property
    def CheckSum (self) -> str:
        """ Get the check sum value of uCode firmware. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Check sum value of uCode firmware.
        """
        return self.__CheckSum

    @property
    def LoaderRevision (self) -> str:
        """ Get the loader revision of uCode firmware. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Loader revision of uCode firmware.
        """
        return self.__LoaderRev

    @property
    def ProcessorFlags (self) -> str:
        """ Get the processor flag of uCode firmware. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Processor flag of uCode firmware.
        """
        return self.__ProcessorFlags

    @property
    def DataSize (self) -> str:
        """ Get the data size of uCode firmware. (4 bytes)

        Note:
            Microcode = Header + Data + Extended Table Header.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Data size of uCode firmware.
        """
        return self.__DataSize

    @property
    def TotalSize (self) -> str:
        """ Get the total size of uCode firmware. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Total size of uCode firmware.
        """
        return self.__TotalSize

    def __PreCheck (self):
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                uCode header structure member is missing.

        Returns:
            None.
        """
        #
        # To check header information is meet the mandatory list.
        #
        for Member in UcodeFwHdr.UCODE_HDR_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )
