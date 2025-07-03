## @file
#  Capsule version object for GbE NVM firmware.
#
#  [Rule]
#  For the GbE NVM version (MM.mm) is 1.32.
#  MM - Major, mm - Minor.
#
#  Version Represent    : 0132
#  Dot Version Represent: 0.0.1.32
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
#
##

import re

from CapsuleCommon import *

from CapsuleGenerate.Model.CapsuleVersion import *

GBE_VERSION_REGEX: str = '^([0-9]+)[.]([0-9]+)+$'

class GbECapsuleVersion (CapsuleVersion):
    def __init__ (
        self,
        Header      : GbeNvmHdr = None,
        HexDotVerStr: str       = None,
        ) -> None:
        """ Class to provide the capsule needed version information.

        Note:
            Caller should be input Header or HexDotVerStr.

        Args:
            Header (GbeNvmHdr, optional):
                GbeNvmHdr header object. Should be GbeNvmHdr instance.
                Defaults to None.
            HexDotVerStr (str, optional):
                Caller input the dot version string format. (Like MM.mm)
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Header      : GbeNvmHdr = Header
        self.__HexDotVerStr: str       = HexDotVerStr
        self.__InputMode   : int       = self.__GetInputMode ()

        self.__MajorVersion: int = int ()
        self.__MinorVersion: int = int ()

        self.__PreCheck ()

    def __GetInputMode (self) -> int:
        """ Return the caller input mode.

        Note:
            FwHeader and HexVer input should not exist in same time.
            If exist, it would depend on the order within the function.

        Args:
            None.

        Raises:
            ErrorException:
                The input method is not valid.
                (FW_HEADER_INPUT_TYPE or DOT_VERSION_HEX_INPUT_TYPE)

        Returns:
            int:
                The caller input mode.
        """
        InputMode: int = None

        FwHeaderInputCondition: bool = \
            (self.__Header is not None) and \
            (isinstance (self.__Header, GbeNvmHdr))
        HexVerInputCondition  : bool = \
            (self.__HexDotVerStr is not None) and \
            (isinstance (self.__HexDotVerStr, str))

        if FwHeaderInputCondition and HexVerInputCondition:
            DEBUG (
              DEBUG_WARN,
              'GbE NVM Header and HEX version assigned in same time. ',
              'Valid value would choose by the order.'
              )

        if FwHeaderInputCondition:
            DEBUG (DEBUG_TRACE, "Input the version information via Header.")
            InputMode = FW_HEADER_INPUT_TYPE
        elif HexVerInputCondition:
            DEBUG (DEBUG_TRACE, "Input the version information via Value.")
            InputMode = DOT_VERSION_HEX_INPUT_TYPE
        else:
            ErrorException (f'Need input valid GbE NVM version information.')

        return InputMode

    def __InitGbeVerInfo (self) -> None:
        """ Initial the GbE version information.

        Args:
            None.

        Raises:
            ValueError:
                Invalid GbE version format.

        Returns:
            None.
        """
        Regex   : Pattern = re.compile (GBE_VERSION_REGEX)
        MajorVer: int     = None
        MinorVer: int     = None

        if self.__InputMode == FW_HEADER_INPUT_TYPE:
            MajorVer = HexToDec (self.__Header.MajorVersion)
            MinorVer = HexToDec (self.__Header.MinorVersion)
        elif self.__InputMode == DOT_VERSION_HEX_INPUT_TYPE:
            if not (Regex.match (self.__HexDotVerStr)):
                raise ValueError (
                        f'Invalid GbE version [{self.__HexDotVerStr}]. '
                        'The format should be MM.mm in digit.'
                        )

            MajorVer = HexToDec (self.__HexDotVerStr.split ('.')[0])
            MinorVer = HexToDec (self.__HexDotVerStr.split ('.')[1])

        self.__MajorVersion = MajorVer
        self.__MinorVersion = MinorVer

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        #
        # Initial the BIOS version
        #
        self.__InitGbeVerInfo ()

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
        return ''.join ([
                    FormatHex (self.__MajorVersion, IsPadding = True, PaddingSize = 2, IsPrefix = False),
                    FormatHex (self.__MinorVersion, IsPadding = True, PaddingSize = 2, IsPrefix = False),
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
        return self.GetHexDotVersion ()

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
                     FormatHex (self.__MajorVersion, IsPadding = True, PaddingSize = 2, IsPrefix = False),
                     FormatHex (self.__MinorVersion, IsPadding = True, PaddingSize = 2, IsPrefix = False),
                     ])

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'GbECapsuleVersion',
    ]
