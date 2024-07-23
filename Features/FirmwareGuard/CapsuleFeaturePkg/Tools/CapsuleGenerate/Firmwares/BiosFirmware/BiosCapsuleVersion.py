## @file
#  Capsule version object for BIOS firmware.
#
#  [Rule]
#  For the BIOS version (MMMM.mm) is 1262.01 (DEC)
#  MMMMM - Major, mm - Minor. MMMM is consist with YWWD.
#  Y - year, WW - workweek, D - workday.
#
#  Version Conversion   : 126201      (DEC) -> 1ECF9     (HEX)
#  Dot Version Represent: 0.0.1262.01 (DEC) -> 0.0.4EE.1 (HEX)
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

class BiosCapsuleVersion (CapsuleVersion):
    def __init__ (
        self,
        Header         : BiosIdHdr       = None,
        DecVer         : Union[int, str] = None,
        IsRestrictCheck: bool            = True,
        ) -> None:
        """ Class to provide the capsule needed version information.

        Note:
            Caller should be input Header or DecVer.

        Args:
            Header (BiosIdHdr, optional):
                BIOS header object. Should be BiosIdHdr instance.
                Defaults to None.
            DecVer (Union[int, str], optional):
                Caller input the DEC version format.
                Defaults to None.
            IsRestrictCheck (bool, optional):
                Switch to decide if need to check lower-level BIOS
                format. (This might be set to False for LSV)
                Defaults to True.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Header         : BiosIdHdr       = Header
        self.__DecVer         : Union[int, str] = DecVer
        self.__IsRestrictCheck: bool            = IsRestrictCheck
        self.__InputMode      : int             = self.__GetInputMode ()

        #
        # BIOS valid version information. (DEC value)
        #
        self.__Year    : int = int ()
        self.__Workweek: int = int ()
        self.__Workday : int = int ()
        self.__Build   : int = int ()

        self.__PreCheck ()

    def __GetInputMode (self) -> int:
        """ Return the caller input mode.

        Note:
            FwHeader and DecVer input should not exist in same time.
            If exist, it would depend on the order within the function.

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
            (self.__Header is not None) and \
            (isinstance (self.__Header, BiosIdHdr))
        DecVerInputCondition  : bool = \
            (self.__DecVer is not None) and \
            (isinstance (self.__DecVer, (str, int)))

        if FwHeaderInputCondition and DecVerInputCondition:
            DEBUG (
              DEBUG_WARN,
              'BIOS Header and DEC version assigned in same time. ',
              'Valid value would choose by the order.'
              )

        if FwHeaderInputCondition:
            DEBUG (DEBUG_TRACE, "Input the version information via Header.")
            InputMode = FW_HEADER_INPUT_TYPE
        elif DecVerInputCondition:
            InputMode = FULL_DEC_VERSION_INPUT_TYPE
        else:
            ErrorException (f'Need input valid BIOS FW version information.')

        return InputMode

    def __InitBiosVerInfo (self) -> None:
        """ Initial the BIOS version for this object.

        Args:
            None.

        Raises:
            ValueError:
                BIOS DEC version input is not valid.

        Returns:
            None.
        """
        VerStr     : str = None
        MajorVerStr: str = None
        MinorVerStr: str = None

        if self.__InputMode == FW_HEADER_INPUT_TYPE:
            MajorVerStr = ZeroPadding (self.__Header.MajorVer, 4)
            MinorVerStr = ZeroPadding (self.__Header.MinorVer, 2)
            VerStr      = MajorVerStr + MinorVerStr
        elif self.__InputMode == FULL_DEC_VERSION_INPUT_TYPE:
            VerStr = str (self.__DecVer)

            if len (VerStr) in range (3, 5):
                VerStr = ZeroPadding (self.__DecVer, 4)
                VerStr = ZeroPadding (self.__DecVer, 6, IsPaddingRight = True)
            elif len (VerStr) in range (5, 7):
                VerStr = ZeroPadding (self.__DecVer, 6)
            else:
                if self.__IsRestrictCheck:
                    raise ValueError (
                            f'Invalid BIOS version [{self.__DecVer}]'
                            )
                VerStr = ZeroPadding (self.__DecVer, 6)

        self.__Year     = int (VerStr[0:1])
        self.__Workweek = int (VerStr[1:3])
        self.__Workday  = int (VerStr[3:4])
        self.__Build    = int (VerStr[4:6])

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) BIOS version - year part is not within 0-9.
                (2) BIOS version - workweek part is not within 1-53.
                (3) BIOS version - workday part is not within 1-7.
                (4) BIOS version - build part is not within 0-99.

        Returns:
            None.
        """
        #
        # Initial the BIOS version
        #
        self.__InitBiosVerInfo ()

        if not self.__IsRestrictCheck:
            DEBUG (DEBUG_TRACE, f'Bypass the BIOS version check.')
            return

        #
        # Check the BIOS version meet the criteria.
        #
        if self.__Year not in range (0, 10):
            raise ValueError (f'Year part [{self.__Year}] not in 0-9.')
        elif self.__Workweek not in range (1, 54):
            raise ValueError (f'Workweek part [{self.__Workweek}] not in 1-53.')
        elif self.__Workday not in range (1, 8):
            raise ValueError (f'Workday part [{self.__Workday}] not in 1-7')
        elif self.__Build not in range (0, 100):
            raise ValueError (f'Build part [{self.__Build}] not in 0-99.')
        else:
            DEBUG (DEBUG_TRACE, 'BIOS version check passed.')

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
        return ''.join ([
                    ZeroPadding (str (self.__Year),     1),
                    ZeroPadding (str (self.__Workweek), 2),
                    ZeroPadding (str (self.__Workday),  1),
                    ZeroPadding (str (self.__Build),    2),
                    ])

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
        MajorVer: str = ''.join ([
                             ZeroPadding (str (self.__Year),     1),
                             ZeroPadding (str (self.__Workweek), 2),
                             ZeroPadding (str (self.__Workday),  1),
                             ])
        MinorVer: str = ZeroPadding (str (self.__Build), 2)
        return '.'.join ([
                     '0',
                     '0',
                     MajorVer,
                     MinorVer,
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
        MajorVer: str = ''.join ([
                             ZeroPadding (DecToHex (self.__Year),     1),
                             ZeroPadding (DecToHex (self.__Workweek), 2),
                             ZeroPadding (DecToHex (self.__Workday),  1),
                             ])
        MinorVer: str = ZeroPadding (DecToHex (self.__Build), 2)

        return '.'.join ([
                     '0',
                     '0',
                     MajorVer,
                     MinorVer,
                     ])

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'BiosCapsuleVersion',
    ]
