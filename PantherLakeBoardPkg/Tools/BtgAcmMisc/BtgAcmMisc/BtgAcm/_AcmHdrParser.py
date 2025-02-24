## @file
#  Definition of ACM header.
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
from typing import Any, Dict, Union

from BtgAcmMisc.Base._ByteBufferWrapper import *
from BtgAcmMisc.Base._StructWrapper import *

#
# typedef struct _ACM_HEADER {
#   UINT32    ModuleType;       ///< Module type
#   UINT32    HeaderLen;        ///<   4  4 Header length (in multiples of four bytes) (161 for version 0.0)
#   UINT32    HeaderVersion;    ///<   8  4 Module format version
#   UINT32    ModuleId;         ///<  12  4 Module release identifier
#   UINT32    ModuleVendor;     ///<  16  4 Module vendor identifier
#   UINT32    Date;             ///<  20  4 Creation date (BCD format: year.month.day)
#   UINT32    Size;             ///<  24  4 Module size (in multiples of four bytes)
#   UINT16    AcmSvn;           ///<  28  2 ACM-SVN Number
#   UINT16    SeSvn;            ///<  30  2 SE-SVN number
#   UINT32    CodeControl;      ///<  32  4 Authenticated code control flags
#   UINT32    ErrorEntryPoint;  ///<  36  4 Error response entry point offset (bytes)
#   UINT32    GdtLimit;         ///<  40  4 GDT limit (defines last byte of GDT)
#   UINT32    GdtBasePtr;       ///<  44  4 GDT base pointer offset (bytes)
#   UINT32    SegSel;           ///<  48  4 Segment selector initializer
#   UINT32    EntryPoint;       ///<  52  4 Authenticated code entry point offset (bytes)
#   UINT32    Reserved2[16];    ///<  56 64 Reserved for future extensions
#   UINT32    KeySize;          ///< 120  4 Module public key size less the exponent  (in multiples of four bytes - 64 for version 0.0)
#   UINT32    ScratchSize;      ///< 124  4 Scratch field size (in multiples of four bytes - 2 * KeySize + 15 for version 0.0)
# } ACM_HEADER;
#

#
# Definition of the ACM Header Structure Member.
#
ACM_HDR_MODULE_TYPE      : str = 'ModuleType'
ACM_HDR_HEADER_LEN       : str = 'HeaderLen'
ACM_HDR_HEADER_VERSION   : str = 'HeaderVersion'
ACM_HDR_MODULE_ID        : str = 'ModuleId'
ACM_HDR_MODULE_VENDOR    : str = 'ModuleVendor'
ACM_HDR_DATE             : str = 'Date'
ACM_HDR_SIZE             : str = 'Size'
ACM_HDR_ACM_SVN          : str = 'AcmSvn'
ACM_HDR_SE_SVN           : str = 'SeSvn'
ACM_HDR_CODE_CONTROL     : str = 'CodeControl'
ACM_HDR_ERROR_ENTRY_POINT: str = 'ErrorEntryPoint'
ACM_HDR_GDT_LIMIT        : str = 'GdtLimit'
ACM_HDR_GDT_BASE_PTR     : str = 'GdtBasePtr'
ACM_HDR_SEG_SEL          : str = 'SegSel'
ACM_HDR_ENTRY_POINT      : str = 'EntryPoint'
ACM_HDR_RSVD_2           : str = 'Reserved2'
ACM_HDR_KEY_SIZE         : str = 'KeySize'
ACM_HDR_SCRATCH_SIZE     : str = 'ScratchSize'

#
# Pre-defined Variable.
#
DEFAULT_MODULE_TYPE_STR  : str = '00010002'
DEFAULT_MODULE_VENDOR_STR: str = '00008086'

#
# ACM Header.
#
ACM_HEADER_FORMAT_DICT: Dict[str, Any] = {
    ACM_HDR_MODULE_TYPE      : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_HEADER_LEN       : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_HEADER_VERSION   : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_MODULE_ID        : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_MODULE_VENDOR    : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_DATE             : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_SIZE             : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_ACM_SVN          : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    ACM_HDR_SE_SVN           : { FIELD_FORMAT: 'H', FIELD_SIZE: 2 },
    ACM_HDR_CODE_CONTROL     : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_ERROR_ENTRY_POINT: { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_GDT_LIMIT        : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_GDT_BASE_PTR     : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_SEG_SEL          : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_ENTRY_POINT      : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_RSVD_2           : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_KEY_SIZE         : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    ACM_HDR_SCRATCH_SIZE     : { FIELD_FORMAT: 'L', FIELD_SIZE: 4 },
    }

ACM_HEADER_STRUCT   : str = None
ACM_HEADER_BYTE_SIZE: int = None

ACM_HEADER_STRUCT, ACM_HEADER_BYTE_SIZE = \
    GetStructAndByteSize (ACM_HEADER_FORMAT_DICT)

class AcmHdrParser (object):

    HDR_OFFSET: int = 0x0

    def __init__ (self, Buffer: ByteBuffer) -> None:
        """ Class to parse the ACM header from buffer.

        Args:
            Buffer (ByteBuffer):
                Input ACM Buffer to be parsed the ACM header.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer: ByteBuffer = Buffer

        self.__PreCheck ()

        #
        # Trim the buffer to ACM header size.
        #
        self.__HdrBuffer: ByteBuffer = copy.deepcopy (self.__Buffer)
        self.__HdrBuffer.Trim (AcmHdrParser.HDR_OFFSET, ACM_HEADER_BYTE_SIZE)

        self.__HdrData: Dict[str, str] = self.__GetAcmHdrData ()
        self.__IsValid: bool           = self.__IsAcmHdr ()

    @property
    def Valid (self) -> bool:
        """ Return the validity of the ACM header.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - ACM header is valid.
                False - ACM header is not valid.
        """
        return self.__IsValid

    @property
    def Header (self) -> Union[None, Dict[str, str]]:
        """ Return the ACM header data dictionary.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, Dict[str, str]]:
                Key-Value header data dictionary.
                If ACM header is not valid, return as None.
                str:
                    Member of struct.
                str:
                    Value of this member.
        """
        if not self.Valid:
            return None

        return self.__HdrData

    @property
    def Size (self) -> Union[None, int]:
        """ Return the size of the ACM binary.

        Note:
            It is a multiple value of 4.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, int]:
                Size of the ACM binary.
                If ACM header is not valid, return as None.
        """
        AcmSize: int = None

        if not self.Valid:
            return None

        AcmSize = int (self.Header[ACM_HDR_SIZE], 16) * 4

        return AcmSize

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                Buffer is not ByteBuffer type.
            ValueError:
                Buffer size is smaller than the header size.

        Returns:
            None.
        """
        if not isinstance (self.__Buffer, ByteBuffer):
            raise TypeError ('Buffer shall be ByteBuffer type.')

        if self.__Buffer.Size < ACM_HEADER_BYTE_SIZE:
            raise ValueError (
                    f'Buffer size [{self.__Buffer.Size}] is smaller than '
                    f'the ACM_HEADER_BYTE_SIZE [{ACM_HEADER_BYTE_SIZE}].'
                    )

    def __GetAcmHdrData (self) -> Dict[str, str]:
        """ Get the ACM header data from header buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            Dict[str, str]:
                Key-Value header data dictionary.
                str:
                    Member of struct.
                str:
                    Value of this member.
        """
        HdrData: Dict[str, str] = None

        HdrData = StructWrapper (
                    self.__HdrBuffer.Buffer,
                    ACM_HEADER_STRUCT,
                    ACM_HEADER_FORMAT_DICT,
                    ).Data

        return HdrData

    def __IsAcmHdr (self) -> bool:
        """ Check the validity of the ACM header.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - ACM header is valid.
                False - ACM header is not valid.
        """
        #
        # Check the ACM module type. (Must be S-ACM 0x00010002.)
        #
        if self.__HdrData[ACM_HDR_MODULE_TYPE] != DEFAULT_MODULE_TYPE_STR:
            return False

        #
        # Check the ACM module provider. (Must be Intel 0x8086.)
        #
        if self.__HdrData[ACM_HDR_MODULE_VENDOR] != DEFAULT_MODULE_VENDOR_STR:
            return False

        return True
