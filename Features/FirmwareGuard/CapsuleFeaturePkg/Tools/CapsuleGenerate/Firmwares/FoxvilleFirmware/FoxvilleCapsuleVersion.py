## @file
#  Capsule version object for Foxville firmware.
#
#  [Rule]
#  For the Foxville version (MM.mm) is 2.25.
#  MM - Major, mm - Minor.
#
#  Version Represent    : 2025
#  Dot Version Represent: 0.2.02.5
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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

FOXVILLE_VERSION_REGEX: str = '^([A-Fa-f0-9]+)[.]([A-Fa-f0-9]+)+$'

class FoxvilleCapsuleVersion (CapsuleVersion):
    def __init__ (
        self,
        Header   : FoxvilleFwHdr = None,
        HexVerStr: str           = None,
        ) -> None:
        """ Class to provide the capsule needed version information.

        Note:
            Caller should be input Header or HexVerStr.

        Args:
            Header (FoxvilleFwHdr, optional):
                Foxville header object. Should be FoxvilleFwHdr instance.
                Defaults to None.
            HexVerStr (str, optional):
                Caller input the version string format. (Like MM.mm)
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Header   : FoxvilleFwHdr = Header
        self.__HexVerStr: str           = HexVerStr
        self.__InputMode: int           = self.__GetInputMode ()

        self.__UpperByte: int = int ()
        self.__LowerByte: int = int ()

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
                (FW_HEADER_INPUT_TYPE or FULL_HEX_VERSION_INPUT_TYPE)

        Returns:
            int:
                The caller input mode.
        """
        InputMode: int = None

        FwHeaderInputCondition: bool = \
            (self.__Header is not None) and \
            (isinstance (self.__Header, FoxvilleFwHdr))
        HexVerInputCondition  : bool = \
            (self.__HexVerStr is not None) and \
            (isinstance (self.__HexVerStr, str))

        if FwHeaderInputCondition and HexVerInputCondition:
            DEBUG (
              DEBUG_WARN,
              'Foxville Header and HEX version assigned in same time. ',
              'Valid value would choose by the order.'
              )

        if FwHeaderInputCondition:
            DEBUG (DEBUG_TRACE, "Input the version information via Header.")
            InputMode = FW_HEADER_INPUT_TYPE
        elif HexVerInputCondition:
            DEBUG (DEBUG_TRACE, "Input the version information via Value.")
            InputMode = FULL_HEX_VERSION_INPUT_TYPE
        else:
            ErrorException (f'Need input valid Foxville FW version information.')

        return InputMode

    def __InitFoxvilleVerInfo (self) -> None:
        """ Initial the Foxville version information.

        Args:
            None.

        Raises:
            ValueError:
                Invalid Foxville version format.

        Returns:
            None.
        """
        Regex    : Pattern = re.compile (FOXVILLE_VERSION_REGEX)
        VerStr   : str     = None
        UpperByte: int     = None
        LowerByte: int     = None

        if self.__InputMode == FW_HEADER_INPUT_TYPE:
            VerStr = self.__Header.DevStarterVer
        elif self.__InputMode == FULL_HEX_VERSION_INPUT_TYPE:
            if not (Regex.match (self.__HexVerStr)):
                raise ValueError (
                        f'Invalid Foxville version [{self.__HexVerStr}]. '
                        'The format should be MM.mm in digit.'
                        )

            VerStrList = self.__HexVerStr.split ('.')
            VerStr     = ''.join ([
                              ZeroPadding (VerStrList[0], Size = 1),
                              ZeroPadding (VerStrList[1], Size = 3),
                              ])

        UpperByte = (HexToDec (VerStr) & 0xFF00) >> 8
        LowerByte = (HexToDec (VerStr) & 0x00FF)

        self.__UpperByte = UpperByte
        self.__LowerByte = LowerByte

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
        self.__InitFoxvilleVerInfo ()

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
                    FormatHex (self.__UpperByte, PaddingSize = 2, IsPrefix = False),
                    FormatHex (self.__LowerByte, PaddingSize = 2, IsPrefix = False),
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
        Segment2: int = None
        Segment3: int = None
        Segment4: int = None

        Segment2 = (self.__UpperByte & 0xF0) >> 4
        Segment3 = ((self.__UpperByte & 0x0F) << 4) | ((self.__LowerByte & 0xF0) >> 4)
        Segment4 = (self.__LowerByte & 0x0F)

        return '.'.join ([
                     '0',
                     ZeroPadding (Segment2, 1),
                     ZeroPadding (Segment3, 2),
                     ZeroPadding (Segment4, 1),
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
        Segment2: int = None
        Segment3: int = None
        Segment4: int = None

        Segment2 = (self.__UpperByte & 0xF0) >> 4
        Segment3 = ((self.__UpperByte & 0x0F) << 4) | ((self.__LowerByte & 0xF0) >> 4)
        Segment4 = (self.__LowerByte & 0x0F)

        return '.'.join ([
                     '0',
                     ZeroPadding (Segment2, 1),
                     ZeroPadding (Segment3, 2),
                     ZeroPadding (Segment4, 1),
                     ])

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'FoxvilleCapsuleVersion',
    ]
