## @file
#  Capsule version object for FSP firmware.
#
#  [Rule]
#  FSP Version
#  ---------------------
#  For the FSP version (MM.mm) is 2.52 (DEC)
#  MM - Major , mm - Minor.
#
#  Version Conversion   : 2052 (DEC) -> 804 (HEX)
#  Dot Version Represent: 0.0.2.052 (DEC) -> 0.0.2.34 (HEX)
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

FSP_VERSION_REGEX: str = r'^(\d){1,3}[.](\d){1,3}$'

class FspCapsuleVersion (CapsuleVersion):
    def __init__ (
        self,
        VersionInfo: FbmVersion = None,
        DotVerStr  : str        = None
        ) -> None:
        """ Class to provide the capsule needed version information.

        Note:
            Caller should be input Header or DotVerStr.

        Args:
            VersionInfo (FbmVersion, optional):
                FBM version info object.
                Should be FbmVersion instance.
                Defaults to None.
            DotVerStr (str, optional):
                Caller input the dot version format.
                (Like XXX.XXX)
                (Different header version have different format)
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        #
        # User input information.
        #
        self.__VersionInfo: FbmVersion = VersionInfo
        self.__DotVerStr  : str        = DotVerStr
        self.__InputMode  : int        = self.__GetInputMode ()

        #
        # FSP valid version information. (DEC value)
        #
        self.__MajorVer: int = int ()
        self.__MinorVer: int = int ()

        self.__PreCheck ()

    def __GetInputMode (self) -> int:
        """ Return the caller input mode.

        Note:
            FwHeader and DotVer input should not exist in same time.
            If exist, it would depend on the order within the function.

        Args:
            None.

        Raises:
            ErrorException:
                The input method is not valid.
                (FW_HEADER_INPUT_TYPE or DOT_VERSION_DEC_INPUT_TYPE)

        Returns:
            int:
                The caller input mode.
        """
        InputMode: int = None

        FwHeaderInputCondition: bool = \
            (self.__VersionInfo is not None) and \
            (isinstance (self.__VersionInfo, FbmVersion))
        DotVerInputCondition  : bool = \
            (self.__DotVerStr is not None) and \
            (isinstance (self.__DotVerStr, str))

        if FwHeaderInputCondition and DotVerInputCondition:
            DEBUG (
              DEBUG_WARN,
              'FbmVersion and dot version assigned in same time. ',
              'Valid value would choose by the order.'
              )

        if FwHeaderInputCondition:
            DEBUG (DEBUG_TRACE, "Input the version information via Header.")
            InputMode = FW_HEADER_INPUT_TYPE
        elif DotVerInputCondition:
            InputMode = DOT_VERSION_DEC_INPUT_TYPE
        else:
            ErrorException (f'Need input valid FSP FW version information.')

        return InputMode

    def __InitFspVerInfo (self) -> None:
        """ Initial the FSP version for this object.

        Note:
            Platform ID and Build version is not reference.

        Args:
            None.

        Raises:
            ErrorException:
                FSP dot version input is not matched the format.

        Returns:
            None.
        """
        Regex: Pattern = re.compile (FSP_VERSION_REGEX)

        if self.__InputMode == FW_HEADER_INPUT_TYPE:
            self.__MajorVer = self.__VersionInfo.Major
            self.__MinorVer = self.__VersionInfo.Minor
        elif self.__InputMode == DOT_VERSION_DEC_INPUT_TYPE:
            if not (Regex.match (self.__DotVerStr)):
                ErrorException (
                  f'Invalid FSP version [{self.__DotVerStr}]. '
                  'The format should be XXX.XXX'
                  )

            self.__MajorVer, self.__MinorVer = self.__DotVerStr.split ('.')

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) FSP major version is not UINT8.
                (2) FSP minor version is not UINT8.

        Returns:
            None.
        """

        self.__InitFspVerInfo ()

        #
        # Check the FSP version meet the criteria.
        #
        if not IsUint8 (self.__MajorVer):
            raise ValueError (f'Major version [{self.__MajorVer}] not UINT8.')
        elif not IsUint8 (self.__MinorVer):
            raise ValueError (f'Minor version [{self.__MinorVer}] not UINT8.')
        else:
            DEBUG (DEBUG_TRACE, 'FSP version check passed.')

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
        MajorVer  : str = str ()
        MinorVer  : str = str ()
        FullDecStr: str = str ()

        MajorVer   = str (self.__MajorVer)
        MinorVer   = ZeroPadding (str (self.__MinorVer), 3)
        FullDecStr = ''.join ([MajorVer, MinorVer])

        return FullDecStr

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
        FullHexStr: str = str ()

        FullHexStr = DecToHex (ToInt (self.GetDecVersion ()))

        return FullHexStr

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
        DecDotVerStr: str = str ()

        DecDotVerStr = '.'.join ([
                                '0',
                                '0',
                                str (self.__MajorVer),
                                ZeroPadding (str (self.__MinorVer), 3),
                                ])

        return DecDotVerStr

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
        HexDotVerStr: str = str ()

        HexDotVerStr = '.'.join ([
                                '0',
                                '0',
                                DecToHex (self.__MajorVer),
                                ZeroPadding (DecToHex (self.__MinorVer), 2),
                                ])

        return HexDotVerStr

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'FspCapsuleVersion',
    ]
