## @file
#  Capsule version object for ME firmware.
#
#  [Rule]
#  For the ME version (MM.mm.HH.BB) is 18.5.25.1538 (DEC)
#  MM - Major , mm - Minor, HH - Hotfix, BB - Build.
#
#  Version Conversion   : 05190602    (HEX) -> 85526018 (DEC) [mmHHBBBB]
#  Dot Version Represent: 18.5.25.1538 (DEC) -> 12.5.19.602 (HEX)
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

import re
from CapsuleCommon import *

from CapsuleGenerate.Model.CapsuleVersion import *

ME_VERSION_REGEX: str = r'^(\d){1,3}[.](\d){1,3}[.](\d){1,3}[.](\d){1,4}$'

class MeCapsuleVersion (CapsuleVersion):
    def __init__ (
        self,
        DotVerStr: str,
        ) -> None:
        """ Class to provide the capsule needed version information.

        Note:
            Caller should be input DotVerStr.

        Args:
            DotVerStr (str):
                Caller input the dot version format.
                (Like XXX.XXX.XXX.XXX)

        Raises:
            None.

        Returns:
            None.
        """
        self.__MajorVer : int = int ()
        self.__MinorVer : int = int ()
        self.__HotfixVer: int = int ()
        self.__BuildVer : int = int ()
        self.__DotVerStr: str = DotVerStr

        self.__InputMode: int = DOT_VERSION_DEC_INPUT_TYPE
        self.__PreCheck ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                Input DotVerStr is not str type.
            ValueError:
                (1) ME dot version format is incorrect.
                (2) Major version is not UINT8.
                (3) Minor version is not UINT8.
                (4) Hotfix version is not UINT8.
                (5) Build version is not UINT16.

        Returns:
            None.
        """
        Regex     : Pattern = re.compile (ME_VERSION_REGEX)
        VerStrList: list = None

        if not isinstance (self.__DotVerStr, str):
            raise TypeError (f'Input DotVerStr should be str type.')

        if not (Regex.match (self.__DotVerStr)):
            ErrorException (
              f'Invalid ME version [{self.__DotVerStr}]. '
              'The format should be XXX.XXX.XXX.XXXX'
              )

        VerStrList       = self.__DotVerStr.split ('.')
        self.__MajorVer  = VerStrList[0]
        self.__MinorVer  = VerStrList[1]
        self.__HotfixVer = VerStrList[2]
        self.__BuildVer  = VerStrList[3]

        #
        # Check the ME version meet the criteria.
        #
        if not IsUint8 (self.__MajorVer):
            raise ValueError (f'Major version [{self.__MajorVer}] not UINT8.')
        elif not IsUint8 (self.__MinorVer):
            raise ValueError (f'Minor version [{self.__MinorVer}] not UINT8.')
        elif not IsUint8 (self.__HotfixVer):
            raise ValueError (f'Hotfix version [{self.__HotfixVer}] not UINT8.')
        elif not IsUint16 (self.__BuildVer):
            raise ValueError (f'Build version [{self.__BuildVer}] not UINT16.')
        else:
            DEBUG (DEBUG_TRACE, 'ME version check passed.')

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
        return str (HexToDec (self.GetHexVersion ()))

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
        MinorVerStr : str = DecToHex (self.__MinorVer)
        HotfixVerStr: str = ZeroPadding (DecToHex (self.__HotfixVer), 2)
        BuildVerStr : str = ZeroPadding (DecToHex (self.__BuildVer),  4)

        return ''.join ([
                    MinorVerStr,
                    HotfixVerStr,
                    BuildVerStr,
                    ])

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
                     ZeroPadding (str (self.__MajorVer),  2),
                     ZeroPadding (str (self.__MinorVer),  3),
                     ZeroPadding (str (self.__HotfixVer), 3),
                     ZeroPadding (str (self.__BuildVer),  4),
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
                     ZeroPadding (DecToHex (self.__MajorVer),  2),
                     ZeroPadding (DecToHex (self.__MinorVer),  2),
                     ZeroPadding (DecToHex (self.__HotfixVer), 2),
                     ZeroPadding (DecToHex (self.__BuildVer),  4),
                     ])
#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'MeCapsuleVersion',
    ]
