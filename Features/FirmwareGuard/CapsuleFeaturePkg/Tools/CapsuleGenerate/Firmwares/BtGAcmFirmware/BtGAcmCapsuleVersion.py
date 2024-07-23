## @file
#  Capsule version object for BtGAcm firmware.
#
#  [Rule]
#  For the BtGAcm revision version (MM.mm.BBBB) is 1.19.12
#  MM - Major, mm - Minor. BBBB - Build.
#  Ver.7 - BB is 1 byte., Ver.9 - BBBB is 2 bytes.
#
#  Version Conversion   : 011912    (DEC) -> 01130C   (HEX)
#  Dot Version Represent: 0.1.19.12 (DEC) -> 0.1.13.C (HEX)
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

BTG_ACM_VERSION_REGEX: str = '^(\d){1,3}[.](\d){1,3}[.](\d){1,5}$'

class BtGAcmCapsuleVersion (CapsuleVersion):
    def __init__ (
        self,
        VersionInfo : BtGAcmVersion = None,
        DecDotVerStr: str           = None,
        HeaderVer   : int           = AIT_VERSION_9,
        ) -> None:
        """ Class to provide the capsule needed version information.

        Note:
            Caller should be input VersionInfo or DecDotVerStr.

        Args:
            VersionInfo (BtGAcmVersion, optional):
                BtGAcm version info object.
                Should be BtGAcmVersion instance.
                Defaults to None.
            DecDotVerStr (str, optional):
                Caller input the DEC dot version format.
                Defaults to None.
            HeaderVer (int, optional):
                The header version of the ACM information table.

        Raises:
            None.

        Returns:
            None.
        """
        self.__VersionInfo : BtGAcmVersion = VersionInfo
        self.__DecDotVerStr: str           = DecDotVerStr
        self.__HeaderVer   : int           = HeaderVer
        self.__InputMode   : int           = self.__GetInputMode ()

        #
        # BtGAcm valid version information. (DEC value)
        #
        self.__MajorVer: int = int ()
        self.__MinorVer: int = int ()
        self.__BuildVer: int = int ()

        self.__PreCheck ()

    def __GetInputMode (self) -> int:
        """ Return the caller input mode.

        Note:
            (1) VersionInfo and DecDotVerStr input should not exist
                in same time.
            (2) If exist, it would depend on the order within this
                function.

        Args:
            None.

        Raises:
            ErrorException:
                The input method is not valid.
                (FW_HEADER_INPUT_TYPE or FULL_DEC_VERSION_INPUT_TYPE)

        Returns:
            int:
                The caller input mode.
        """
        InputMode: int = None

        FwHeaderInputCondition: bool = \
            (self.__VersionInfo is not None) and \
            (isinstance (self.__VersionInfo, BtGAcmVersion))
        DecVerInputCondition  : bool = \
            (self.__DecDotVerStr is not None) and \
            (isinstance (self.__DecDotVerStr, str))

        if FwHeaderInputCondition and DecVerInputCondition:
            DEBUG (
              DEBUG_WARN,
              'BtGAcm Header and DEC version assigned in same time. ',
              'Valid value would choose by the order.'
              )

        if FwHeaderInputCondition:
            DEBUG (DEBUG_TRACE, "Input the version information via Header.")
            InputMode = FW_HEADER_INPUT_TYPE
        elif DecVerInputCondition:
            InputMode = FULL_DEC_VERSION_INPUT_TYPE
        else:
            ErrorException (f'Need input valid BtGAcm FW version information.')

        return InputMode

    def __InitBtGAcmVerInfo (self) -> None:
        """ Initial the BtGAcm version for this object.

        Args:
            None.

        Raises:
            TypeError:
                VersionInfo not BtGAcmVersion type.
            ValueError:
                BtGAcm DEC dot version input is not valid.

        Returns:
            None.
        """
        Regex     : Pattern = re.compile (BTG_ACM_VERSION_REGEX)
        VerStrList: list    = None

        if self.__InputMode == FW_HEADER_INPUT_TYPE:
            if not isinstance (self.__VersionInfo, BtGAcmVersion):
                raise TypeError (f'VersionInfo should be BtGAcmVersion type.')
            self.__MajorVer = self.__VersionInfo.Major
            self.__MinorVer = self.__VersionInfo.Minor
            self.__BuildVer = self.__VersionInfo.Build
        elif self.__InputMode == FULL_DEC_VERSION_INPUT_TYPE:
            if not (Regex.match (self.__DecDotVerStr)):
                raise ValueError (
                        f'Invalid BtGAcm version [{self.__DecDotVerStr}]. '
                        'The format should be XXX.XXX.XXXXX'
                        )

            VerStrList      = self.__DecDotVerStr.split ('.')
            self.__MajorVer = ToInt (VerStrList[0])
            self.__MinorVer = ToInt (VerStrList[1])
            self.__BuildVer = ToInt (VerStrList[2])

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) Major version not UINT8.
                (2) Minor version not UINT8.
                (3) ACM information table header version unsupported.
                (4) Build version not UINT8. (Ait version > 7)
                (5) Build version not UINT16. (Ait version > 9)

        Returns:
            None.
        """
        #
        # Initial the BIOS version
        #
        self.__InitBtGAcmVerInfo ()

        if not IsUint8 (self.__MajorVer):
            raise ValueError (f'Major version should be UINT8. [{self.__MajorVer}]')
        elif not IsUint8 (self.__MinorVer):
            raise ValueError (f'Minor version should be UINT8. [{self.__MinorVer}]')
        elif self.__HeaderVer < AIT_VERSION_7:
            raise ValueError (
                    f'Header version less than {AIT_VERSION_7}. '
                    'Not supported.'
                    )

        if (self.__HeaderVer >= AIT_VERSION_9) and (not IsUint16 (self.__BuildVer)):
            raise ValueError (f'Build version should be UINT16. [{self.__BuildVer}]')
        elif (self.__HeaderVer >= AIT_VERSION_7) and (not IsUint8 (self.__BuildVer)):
            raise ValueError (f'Build version should be UINT8. [{self.__BuildVer}]')

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
                    ZeroPadding (DecToHex (self.__MajorVer), 2),
                    ZeroPadding (DecToHex (self.__MinorVer), 2),
                    ZeroPadding (DecToHex (self.__BuildVer), 2),
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
                     '0',
                     str (self.__MajorVer),
                     str (self.__MinorVer),
                     str (self.__BuildVer),
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
                     DecToHex (self.__MajorVer),
                     DecToHex (self.__MinorVer),
                     DecToHex (self.__BuildVer),
                     ])
