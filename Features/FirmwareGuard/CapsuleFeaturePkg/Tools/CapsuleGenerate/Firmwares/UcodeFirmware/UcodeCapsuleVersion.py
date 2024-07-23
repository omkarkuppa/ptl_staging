## @file
#  Capsule version object for uCode firmware.
#
#  [Rule]
#  For the uCode version is UINT32 format.
#
#  Version Conversion   : 20       (DEC) -> 14       (HEX)
#  Dot Version Represent: 0.0.0.20 (DEC) -> 0.0.0.14 (HEX)
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

class UcodeCapsuleVersion (CapsuleVersion):
    def __init__ (
        self,
        Version: Union[int, str],
        ) -> None:
        """ Class to provide the capsule needed version information.

        Args:
            Version (Union[int, str]):
                Version of the uCode to be converted into capsule
                version format.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Version  : Union[int, str] = Version
        self.__InputMode: int             = FULL_VERSION_INPUT_TYPE

        self.__DecVer: int = None

        self.__PreCheck ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                Version not str nor int type.
            ValueError:
                Version not UINT32.

        Returns:
            None.
        """
        if not isinstance (self.__Version, (int, str)):
            raise TypeError ('Version should be str or int type.')

        self.__DecVer = ToInt (self.__Version)

        if not IsUint32 (self.__DecVer):
            raise ValueError ('Version should be UINT32.')

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
        return str (self.__DecVer)

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
        return DecToHex (self.GetDecVersion ())

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
                     self.GetDecVersion (),
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
                     self.GetHexVersion (),
                     ])

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'UcodeCapsuleVersion',
    ]
