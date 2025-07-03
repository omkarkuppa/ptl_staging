## @file
#  Definition of GbE Non-Volatile Memory Map Common header.
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
#  @par Specification
#    - Integrated GbE LAN Controller HAS, revision 1.1
#
##

from typing import Any, Dict

from CapsuleCommon.Constant.BitLib import *
from CapsuleCommon.Format.HexadecimalLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

#
# typedef union {
#   struct {
#     UINT16    ImageId      : 4;
#     UINT16    EepromMinVer : 8;
#     UINT16    EepromMajVer : 4;
#   } Bits;
#   UINT16    Uint16;
# } GBE_NVM_IMAGE_VER_INFO;
#
# typedef struct {
#   UINT16                     MacAddress[3];     //< WORD [ 2: 0]: Ethernet Address
#   UINT16                     SwCompatibility;   //< WORD [ 3: 3]: SW Compatibility
#   UINT16                     Reserved1;         //< WORD [ 4: 4]: Not used in GbE update
#   UINT16                     ImageVerInfo;      //< WORD [ 5: 5]: Image information 1
#   UINT16                     Reserved2[13];     //< WORD [18: 6]: Not used in GbE update
#   GBE_SHARED_INIT_CONTROL    SharedInitControl; //< WORD [19:19]: Shared Init Control Word
#   UINT16                     Reserved3[43];     //< WORD [62:20]: Not used in GbE update
#   UINT16                     CheckSum;          //< WORD [63:63]: SW checksum, bytes 00h through 7Dh
# } GBE_LAN_NVM_MAP;
#

GBE_NVM_HDR_MAC_ADDRESS_0   : str = 'MacAddress0'
GBE_NVM_HDR_MAC_ADDRESS_1   : str = 'MacAddress1'
GBE_NVM_HDR_MAC_ADDRESS_2   : str = 'MacAddress2'
GBE_NVM_HDR_SW_COMPATIBILITY: str = 'SwCompatibility'
GBE_NVM_HDR_RESERVED1       : str = 'Reserved1'
GBE_NVM_HDR_IMAGE_VER_INFO  : str = 'ImageVerInfo'
GBE_NVM_HDR_RESERVED2       : str = 'Reserved2'
GBE_NVM_HDR_SHARED_INIT_CTL : str = 'SharedInitControl'
GBE_NVM_HDR_RESERVED3       : str = 'Reserved3'
GBE_NVM_HDR_CHECKSUM        : str = 'CheckSum'

#
# Pre-defined string.
#
DEFAULT_GBE_NVM_MAC_ADDRESS: int = 0x8887_8888_8888

GBE_NVM_HDR_FORMAT_DICT: Dict[str, Any] = {
    GBE_NVM_HDR_MAC_ADDRESS_0   : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    GBE_NVM_HDR_MAC_ADDRESS_1   : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    GBE_NVM_HDR_MAC_ADDRESS_2   : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    GBE_NVM_HDR_SW_COMPATIBILITY: { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    GBE_NVM_HDR_RESERVED1       : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    GBE_NVM_HDR_IMAGE_VER_INFO  : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    GBE_NVM_HDR_RESERVED2       : { FIELD_FORMAT: '13H', FIELD_SIZE: 26 },
    GBE_NVM_HDR_SHARED_INIT_CTL : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    GBE_NVM_HDR_RESERVED3       : { FIELD_FORMAT: '43H', FIELD_SIZE: 86 },
    GBE_NVM_HDR_CHECKSUM        : { FIELD_FORMAT: 'H',   FIELD_SIZE:  2 },
    }

GBE_NVM_HDR_STRUCT   : str = str ()
GBE_NVM_HDR_BYTE_SIZE: int = int ()
GBE_NVM_HDR_STRUCT, GBE_NVM_HDR_BYTE_SIZE = \
    GetStructFormatAndByteSize (GBE_NVM_HDR_FORMAT_DICT)

class GbeNvmHdr (object):
    DefaultMacAddress: int = DEFAULT_GBE_NVM_MAC_ADDRESS

    #
    # List of items need to check for input key within dict.
    #
    GBE_NVM_HDR_MANDATORY: List[str] = [
        GBE_NVM_HDR_MAC_ADDRESS_0,
        GBE_NVM_HDR_MAC_ADDRESS_1,
        GBE_NVM_HDR_MAC_ADDRESS_2,
        GBE_NVM_HDR_SW_COMPATIBILITY,
        GBE_NVM_HDR_RESERVED1,
        GBE_NVM_HDR_IMAGE_VER_INFO,
        GBE_NVM_HDR_RESERVED2,
        GBE_NVM_HDR_SHARED_INIT_CTL,
        GBE_NVM_HDR_RESERVED3,
        GBE_NVM_HDR_CHECKSUM,
        ]

    def __init__ (self, **Kwargs) -> None:
        """ Class used to represent the GbE NVM header.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of GbE NVM header structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__MacAddress0 : str = self.__Kwargs[GBE_NVM_HDR_MAC_ADDRESS_0]
        self.__MacAddress1 : str = self.__Kwargs[GBE_NVM_HDR_MAC_ADDRESS_1]
        self.__MacAddress2 : str = self.__Kwargs[GBE_NVM_HDR_MAC_ADDRESS_2]
        self.__ImageVerInfo: str = self.__Kwargs[GBE_NVM_HDR_IMAGE_VER_INFO]

    @property
    def MacAddress (self) -> str:
        """ Get the MAC address of GbE NVM image. (6 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The MAC address of GbE NVM image.
        """
        return ''.join ([
            self.__MacAddress2,
            self.__MacAddress1,
            self.__MacAddress0,
            ])

    @property
    def ImageVerInfo (self) -> str:
        """ Get the version of GbE NVM image. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The version of GbE NVM image.
        """
        return self.__ImageVerInfo

    @property
    def ImageId (self) -> str:
        """ Get the ID of GbE NVM image. (4 bits)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The ID of GbE NVM image.
        """
        Value: int = HexToDec (self.ImageVerInfo)
        Mask : int = BIT0 | BIT1 | BIT2 | BIT3

        Value = (Value & Mask) >> 0

        return FormatHex (
                 Value,
                 IsPrefix    = False,
                 IsPadding   = True,
                 PaddingSize = 1,
                 )

    @property
    def MajorVersion (self) -> str:
        """ Get the major version of GbE NVM image. (4 bits)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The major version of GbE NVM image.
        """
        Value: int = HexToDec (self.ImageVerInfo)
        Mask : int = BIT12 | BIT13 | BIT14 | BIT15

        Value = (Value & Mask) >> 12

        return FormatHex (
                 Value,
                 IsPrefix    = False,
                 IsPadding   = True,
                 PaddingSize = 1,
                 )

    @property
    def MinorVersion (self) -> str:
        """ Get the minor version of GbE NVM image. (8 bits)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The minor version of GbE NVM image.
        """
        Value: int = HexToDec (self.ImageVerInfo)
        Mask : int = BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9 | BIT10 | BIT11

        Value = (Value & Mask) >> 4

        return FormatHex (
                 Value,
                 IsPrefix    = False,
                 IsPadding   = True,
                 PaddingSize = 2,
                 )

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) GbE NVM header structure member is missing.
                (2) GbE NVM MAC address is not default one.

        Returns:
            None.
        """
        MacAddress: str = str ()

        #
        # To check header information is meet the mandatory list.
        #
        for Member in GbeNvmHdr.GBE_NVM_HDR_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )

        #
        # To check the parsed MAC address is correct.
        #
        MacAddress = \
            ''.join ([
                 self.__Kwargs[GBE_NVM_HDR_MAC_ADDRESS_2],
                 self.__Kwargs[GBE_NVM_HDR_MAC_ADDRESS_1],
                 self.__Kwargs[GBE_NVM_HDR_MAC_ADDRESS_0],
                 ])

        if (HexToDec (MacAddress)) != GbeNvmHdr.DefaultMacAddress:
            raise ValueError (
                    f'Input MAC address ({MacAddress}) is not '
                    f'({DecToHex (GbeNvmHdr.DefaultMacAddress)})'
                    )
