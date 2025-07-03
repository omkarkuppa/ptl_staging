## @file
#  Capsule version object for FSP firmware.
#
#  [Rule]
#  FSP Version
#  ---------------------
#  For FSP version (MM.mm.RRRR.BBBB) is 02.11.0024.4721 (HEX) in BCD.
#  MM - Major, mm - Minor, RRRR - Revision, BBBB - Build
#
#  Version Conversion   : 00244721 (HEX) -> 2377505 (DEC) [RRRRBBBB]
#  Dot Version Represent: 02.11.0024.4721 (DEC) -> 02.11.18.1271 (HEX)
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

FSP_VERSION_REGEX: str = r'^([0-9]){1,2}[.]([0-9]){1,2}[.]([0-9]){1,4}[.]([0-9]){1,4}$'

class FspCapsuleVersion (CapsuleVersion):
    def __init__ (
        self,
        VersionInfo: FspFwVersion = None,
        DotVerStr  : str          = None,
        ) -> None:
        """ Class to provide the capsule needed version information.

        Note:
            Caller should be input Header or DotVerStr.

        Args:
            VersionInfo (FspFwVersion, optional):
                FSP version info object.
                Should be FspFwVersion instance.
                Defaults to None.
            DotVerStr (str, optional):
                Caller input the dot version format.
                (Like MM.mm.RRRR.BBBB)
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
        self.__VersionInfo: FspFwVersion = VersionInfo
        self.__DotVerStr  : str          = DotVerStr
        self.__InputMode  : int          = self.__GetInputMode ()

        #
        # FSP version information.
        #
        self.__MajorVer: int = None
        self.__MinorVer: int = None
        self.__RevVer  : int = None
        self.__BuildVer: int = None

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
                (FW_HEADER_INPUT_TYPE or DOT_VERSION_HEX_INPUT_TYPE)

        Returns:
            int:
                The caller input mode.
        """
        InputMode: int = None

        FwHeaderInputCondition: bool = \
            (self.__VersionInfo is not None) and \
            (isinstance (self.__VersionInfo, FspFwVersion))
        DotVerInputCondition  : bool = \
            (self.__DotVerStr is not None) and \
            (isinstance (self.__DotVerStr, str))

        if FwHeaderInputCondition and DotVerInputCondition:
            DEBUG (
              DEBUG_WARN,
              'FspFwVersion and dot version assigned in same time. ',
              'Valid value would choose by the order.'
              )

        if FwHeaderInputCondition:
            DEBUG (DEBUG_TRACE, "Input the version information via Header.")
            InputMode = FW_HEADER_INPUT_TYPE
        elif DotVerInputCondition:
            InputMode = DOT_VERSION_HEX_INPUT_TYPE
        else:
            ErrorException (f'Need input valid FSP FW version information.')

        return InputMode

    def __InitFspVerInfo (self) -> None:
        """ Initial the FSP version for this object.

        Args:
            None.

        Raises:
            ErrorException:
                FSP dot version input is not matched the format.

        Returns:
            None.
        """
        Regex      : Pattern = re.compile (FSP_VERSION_REGEX)
        MajorVerStr: str     = None
        MinorVerStr: str     = None
        RevVerStr  : str     = None
        BuildVerStr: str     = None

        if self.__InputMode == FW_HEADER_INPUT_TYPE:
            MajorVerStr = self.__VersionInfo.Major
            MinorVerStr = self.__VersionInfo.Minor
            RevVerStr   = self.__VersionInfo.Revision
            BuildVerStr = self.__VersionInfo.Build
        elif self.__InputMode == DOT_VERSION_HEX_INPUT_TYPE:
            if not (Regex.match (self.__DotVerStr)):
                ErrorException (
                  f'Invalid FSP version [{self.__DotVerStr}]. '
                  'The format should be MM.mm.RRRR.BBBB'
                  )

            MajorVerStr, MinorVerStr, RevVerStr, BuildVerStr \
                = self.__DotVerStr.split ('.')

        self.__MajorVer = HexToDec (MajorVerStr)
        self.__MinorVer = HexToDec (MinorVerStr)
        self.__RevVer   = HexToDec (RevVerStr)
        self.__BuildVer = HexToDec (BuildVerStr)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) FSP major version is not UINT8.
                (2) FSP minor version is not UINT8.
                (3) FSP revision version is not UINT16.
                (4) FSP build version is not UINT16.

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
        elif not IsUint16 (self.__RevVer):
            raise ValueError (f'Revision version [{self.__RevVer}] not UINT16.')
        elif not IsUint16 (self.__BuildVer):
            raise ValueError (f'Minor version [{self.__BuildVer}] not UINT16.')
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
        FullHexStr: str = str ()

        FullHexStr = ''.join ([
                          DecToHex (self.__RevVer),
                          ZeroPadding (String = DecToHex (self.__BuildVer), Size = 4),
                          ])

        return FullHexStr

    def GetDecDotVersion (self) -> str:
        """ Return the firmware version format in DEC dot style.

        Note:
            The version of FSP image is BCD format.
            Hence, the output of DEC dot version should covert into HEX.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String to represent the DEC dot format version.
        """
        DecDotVerStr: str = str ()
        MajorStr    : str = None
        MinorStr    : str = None
        RevStr      : str = None
        BuildStr    : str = None

        MajorStr = ZeroPadding (String = DecToHex (self.__MajorVer), Size = 2)
        MinorStr = ZeroPadding (String = DecToHex (self.__MinorVer), Size = 2)
        RevStr   = ZeroPadding (String = DecToHex (self.__RevVer),   Size = 4)
        BuildStr = ZeroPadding (String = DecToHex (self.__BuildVer), Size = 4)

        DecDotVerStr = '.'.join ([
                                MajorStr,
                                MinorStr,
                                RevStr,
                                BuildStr,
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
        MajorStr    : str = None
        MinorStr    : str = None
        RevStr      : str = None
        BuildStr    : str = None

        MajorStr = ZeroPadding (String = DecToHex (DecToHex (self.__MajorVer)), Size = 2)
        MinorStr = ZeroPadding (String = DecToHex (DecToHex (self.__MinorVer)), Size = 2)
        RevStr   = ZeroPadding (String = DecToHex (DecToHex (self.__RevVer)),   Size = 4)
        BuildStr = ZeroPadding (String = DecToHex (DecToHex (self.__BuildVer)), Size = 4)

        HexDotVerStr = '.'.join ([
                                MajorStr,
                                MinorStr,
                                RevStr,
                                BuildStr,
                                ])

        return HexDotVerStr

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'FspCapsuleVersion',
    ]
