## @file
#  Definition of BiosGuard Update Package (BGUP) Header.
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

#
# BGUP header version.
#
BGUP_HDR_VER2: int = 0x2

#
# Constant Value.
#
DEFAULT_BGUP_HDR_VER2_PSL_MAJ_VER: int = 0x2
DEFAULT_BGUP_HDR_VER2_PSL_MIN_VER: int = 0x0

#
# Define of BiosGuard Update Package (BGUP) Structure Member.
#
BGUP_HDR_VERSION        : str = 'Version'
BGUP_HDR_RSVD_3         : str = 'Rsvd3'
BGUP_HDR_PLATID         : str = 'PlatId'
BGUP_HDR_PKG_ATTRIBUTES : str = 'PkgAttributes'
BGUP_HDR_RSVD_4         : str = 'Rsvd4'
BGUP_HDR_PSL_MAJOR_VER  : str = 'MajorVer'
BGUP_HDR_PSL_MINOR_VER  : str = 'MinorVer'
BGUP_HDR_SCRIPT_SIZE    : str = 'ScriptSize'
BGUP_HDR_DATA_SIZE      : str = 'DataSize'
BGUP_HDR_BIOS_SVN       : str = 'BiosSvn'
BGUP_HDR_EC_SVN         : str = 'EcSvn'
BGUP_HDR_VENDOR_SPECIFIC: str = 'VendorSpecific'

#
# BIOS Guard update Package Header (Ver.2)
#
# typedef struct {
#   UINT16 Version;           ///< Version of the update package header.  Must be 0x0002.
#   UINT8  Reserved3[2];      ///< Reserved bits.
#   UINT8  PlatId[16];        ///< PLAT_ID used to be compared against the one found in the BGPDT to prevent cross platform flashing.
#   /**
#   If any bit set in this field then BGUP must be signed and valid BGUPC must be provided for BGUP to be processed.
#   - BIT[0]: Indicates write/erase operations will be executed on protected flash area indicated in the BGPDT SFAM.
#   - BIT[1]: Indicates protected EC operations included.
#   **/
#   UINT16 PkgAttributes;
#   UINT8  Reserved4[2];      ///< Reserved bits.
#   UINT16 PslMajorVer;       ///< Indicates the PSL major version. Must be 2.
#   UINT16 PslMinorVer;       ///< Indicates the PSL minor version. Must be 0.
#   UINT32 ScriptSectionSize; ///< Size in bytes of the script.
#   UINT32 DataSectionSize;   ///< Size of the data region in bytes.
#   UINT32 BiosSvn;           ///< BIOS SVN.
#   UINT32 EcSvn;             ///< EC SVN.
#   UINT32 VendorSpecific;    ///< Vendor specific data.
# } BGUP_HEADER;
#
BGUP_HDR_VER2_FORMAT_DICT: Dict[str, Any] = {
    BGUP_HDR_VERSION        : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    BGUP_HDR_RSVD_3         : { FIELD_FORMAT: '2B',  FIELD_SIZE:  2 },
    BGUP_HDR_PLATID         : { FIELD_FORMAT: '16c', FIELD_SIZE: 16 },
    BGUP_HDR_PKG_ATTRIBUTES : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    BGUP_HDR_RSVD_4         : { FIELD_FORMAT: '2B',  FIELD_SIZE:  2 },
    BGUP_HDR_PSL_MAJOR_VER  : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    BGUP_HDR_PSL_MINOR_VER  : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    BGUP_HDR_SCRIPT_SIZE    : { FIELD_FORMAT: 'L',   FIELD_SIZE:  4 },
    BGUP_HDR_DATA_SIZE      : { FIELD_FORMAT: 'L',   FIELD_SIZE:  4 },
    BGUP_HDR_BIOS_SVN       : { FIELD_FORMAT: 'L',   FIELD_SIZE:  4 },
    BGUP_HDR_EC_SVN         : { FIELD_FORMAT: 'L',   FIELD_SIZE:  4 },
    BGUP_HDR_VENDOR_SPECIFIC: { FIELD_FORMAT: 'L',   FIELD_SIZE:  4 },
    }

BGUP_HDR_VER2_STRUCT   : str = str ()
BGUP_HDR_VER2_BYTE_SIZE: int = int ()
BGUP_HDR_VER2_STRUCT, BGUP_HDR_VER2_BYTE_SIZE = \
    GetStructFormatAndByteSize (BGUP_HDR_VER2_FORMAT_DICT)

class BgupHdr (object):
    #
    # List of items need to check for input key within dict.
    #
    BGUP_HDR_MANDATORY: List[str] = [
        BGUP_HDR_VERSION,
        BGUP_HDR_PLATID,
        BGUP_HDR_PKG_ATTRIBUTES,
        BGUP_HDR_PSL_MAJOR_VER,
        BGUP_HDR_PSL_MINOR_VER,
        BGUP_HDR_SCRIPT_SIZE,
        BGUP_HDR_DATA_SIZE,
        BGUP_HDR_BIOS_SVN,
        BGUP_HDR_EC_SVN,
        BGUP_HDR_VENDOR_SPECIFIC,
        ]

    def __init__ (self, **Kwargs) -> None:
        """ Class used to represent the BGUP header.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of BGUP header structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__Version       : str = Kwargs[BGUP_HDR_VERSION]
        self.__PlatId        : str = Kwargs[BGUP_HDR_PLATID]
        self.__PkgAttr       : str = Kwargs[BGUP_HDR_PKG_ATTRIBUTES]
        self.__PslMajVer     : str = Kwargs[BGUP_HDR_PSL_MAJOR_VER]
        self.__PslMinVer     : str = Kwargs[BGUP_HDR_PSL_MINOR_VER]
        self.__BgslSize      : str = Kwargs[BGUP_HDR_SCRIPT_SIZE]
        self.__DataSize      : str = Kwargs[BGUP_HDR_DATA_SIZE]
        self.__BiosSvn       : str = Kwargs[BGUP_HDR_BIOS_SVN]
        self.__EcSvn         : str = Kwargs[BGUP_HDR_EC_SVN]
        self.__VendorSpecific: str = Kwargs[BGUP_HDR_VENDOR_SPECIFIC]

    @property
    def Version (self) -> str:
        """ Get the version of BGUP header. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The version of BGUP header.
        """
        return self.__Version

    @property
    def PlatId (self) -> str:
        """ Get the platform ID of BGUP. (Max: 16 characters)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The Platform ID of BGUP header.
        """
        return ByteToStr (
                 self.__PlatId,
                 IsReverse = False
                 )

    @property
    def PkgAttributes (self) -> str:
        """ Get the update package attributes of BGUP. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The update package attributes of BGUP header.
        """
        return self.__PkgAttr

    @property
    def PslMajorVer (self) -> str:
        """ Get the BGSL major version of BGUP. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The BGSL major version of BGUP header.
        """
        return self.__PslMajVer

    @property
    def PslMinorVer (self) -> str:
        """ Get the BGSL minor version of BGUP. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The BGSL minor version of BGUP header.
        """
        return self.__PslMinVer

    @property
    def BgslSize (self) -> str:
        """ Get the BGSL size within this BGUP. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The BGSL size of BGUP header.
        """
        return self.__BgslSize

    @property
    def DataSize (self) -> str:
        """ Get the data size within this BGUP. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The data size of BGUP header.
        """
        return self.__DataSize

    @property
    def BiosSvn (self) -> str:
        """ Get the BIOS security version number of BGUP. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The BIOS SVN of BGUP header.
        """
        return self.__BiosSvn

    @property
    def EcSvn (self) -> str:
        """ Get the EC security version number of BGUP. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The EC SVN of BGUP header.
        """
        return self.__EcSvn

    @property
    def VendorSpecific (self) -> str:
        """ Get the vendor specific definition. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The vendor specific definition of BGUP header.
        """
        return self.__VendorSpecific

    def __CheckHdrVer2 (self) -> None:
        """ Check the BGUP header version 2.

        Args:
            None.

        Raises:
            ValueError:
                (1) Package BGSL version is not 2.0.
                (2) Package BGSL size is not larger than 16 bytes.
                (3) Package BGSL size is not multiple of 8.

        Returns:
            None.
        """
        PslMajVer: int = HexToDec (self.__Kwargs[BGUP_HDR_PSL_MAJOR_VER])
        PslMinVer: int = HexToDec (self.__Kwargs[BGUP_HDR_PSL_MINOR_VER])
        BgslSize : int = HexToDec (self.__Kwargs[BGUP_HDR_SCRIPT_SIZE])

        if PslMajVer != DEFAULT_BGUP_HDR_VER2_PSL_MAJ_VER:
            raise ValueError (f'Ver.2 header PSL major version should be 2.')

        if PslMinVer != DEFAULT_BGUP_HDR_VER2_PSL_MIN_VER:
            raise ValueError (f'Ver.2 header PSL minor version should be 0.')

        if BgslSize < 16:
            raise ValueError (f'Byte of BGSL size should be larger than 16.')
        elif BgslSize % 8 != 0:
            raise ValueError (f'BGSL size should be multiple of 8.')

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                BGUP header structure member is missing.

        Returns:
            None.
        """
        #
        # To check header information is meet the mandatory list.
        #
        for Member in BgupHdr.BGUP_HDR_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )

        if HexToDec (self.__Kwargs[BGUP_HDR_VERSION]) == BGUP_HDR_VER2:
            self.__CheckHdrVer2 ()
        else:
            raise ValueError (f'Header version is not supported.')
