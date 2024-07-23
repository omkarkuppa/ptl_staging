## @file
#  Capsule version object for ISH PDT firmware.
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

from CapsuleCommon import *

from CapsuleGenerate.Model.CapsuleVersion import *

class IshPdtCapsuleVersion (CapsuleVersion):
    def __init__ (
        self,
        Version: Union[int, str],
        ) -> None:
        """ Class to provide the capsule needed version information.

        Note:
            Caller should be input Version. (DEC or HEX)

        Args:
            Version (Union[int, str]):
                The version of this firmware image, it should be UINT.
                If input is HEX, it should contain the 0x or 0X prefix.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Version: int = ToInt (Version)

        self.__PreCheck ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                Version is not UINT16.

        Returns:
            None.
        """
        #
        # Check the IshPdt version meet the criteria.
        #
        if not IsUint16 (self.__Version):
            raise ValueError (f'IshPdt version {self.__Version} not UINT16.')
        else:
            DEBUG (DEBUG_TRACE, 'IshPdt version check passed.')

    def GetDecVersion (self) -> str:
        """ Return the firmware version format in DEC style. (32-bit)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String to represent the DEC version.
        """
        return str (self.__Version)

    def GetHexVersion (self) -> str:
        """ Return the firmware version format in HEX style. (32-bit)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String to represent the HEX version.
        """
        return DecToHex (self.__Version)

    def GetDecDotVersion (self) -> str:
        """ Return the firmware version format in DEC dot style.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String to represent the DEC dot format version.
        """
        return '.'.join ([
                     '0',
                     '0',
                     '0',
                     str (self.__Version),
                     ])

    def GetHexDotVersion (self) -> str:
        """ Return the firmware version format in HEX dot style.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String to represent the HEX dot format version.
        """
        return '.'.join ([
                     '0',
                     '0',
                     '0',
                     DecToHex (self.__Version),
                     ])

#
# Expose variable / method / object.
#
__all__: List[str] = [
    'IshPdtCapsuleVersion',
    ]
