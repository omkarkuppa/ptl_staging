## @file
#  Capsule version object for EC firmware.
#
#  [Rule]
#  EC Header Version - 1
#  ---------------------
#  For the EC version (MM.mm) is 1.52 (DEC)
#  MM - Major , mm - Minor.
#
#  Version Conversion   : 1052 (DEC) -> 41C (HEX)
#  Dot Version Represent: 0.0.1.052 (DEC) -> 0.0.1.34 (HEX)
#
#  EC Header Version - 2
#  ---------------------
#  For the EC version (MM.mm.PP.BB) is 1.71.1.0 (DEC)
#  MM - Major, mm - Minor, PP - Platform ID, BB - Build.
#
#  Version Conversion   : 1470100 (HEX) -> 21,430,528 (DEC)
#  Dot Version Represent: 1.71.1.0 (DEC) -> 1.47.1.0 (HEX)
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

EC_VERSION_1_REGEX: str = r'^(\d){1,3}[.](\d){1,3}$'
EC_VERSION_2_REGEX: str = r'^(\d){1,3}[.](\d){1,3}[.](\d){1,3}[.](\d){1,3}$'

class EcCapsuleVersion (CapsuleVersion):
    def __init__ (
        self,
        Header   : EcFwHdr = None,
        DotVerStr: str     = None,
        EcHdrVer : int     = EC_FW_HDR_VER2,
        ) -> None:
        """ Class to provide the capsule needed version information.

        Note:
            Caller should be input Header or DotVerStr.

        Args:
            Header (EcFwHdr, optional):
                EC header object. Should be EcFwHdr instance.
                Defaults to None.
            DotVerStr (str, optional):
                Caller input the dot version format.
                (Like XXX.XXX.XXX.XXX)
                (Different header version have different format)
                Defaults to None.
            EcHdrVer (int, optional):
                EC header ver. should be one of EC_FW_HDR_VERSION_LIST.
                Defaults to EC_FW_HDR_VER2.

        Raises:
            None.

        Returns:
            None.
        """
        #
        # User input information.
        #
        self.__Header   : EcFwHdr = Header
        self.__DotVerStr: str     = DotVerStr
        self.__EcHdrVer : int     = EcHdrVer
        self.__InputMode: int     = self.__GetInputMode ()

        #
        # EC valid version information. (DEC value)
        #
        self.__MajorVer: int = int ()
        self.__MinorVer: int = int ()
        self.__PlatId  : int = int ()
        self.__BuildVer: int = int ()

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
            (self.__Header is not None) and \
            (isinstance (self.__Header, EcFwHdr))
        DotVerInputCondition  : bool = \
            (self.__DotVerStr is not None) and \
            (isinstance (self.__DotVerStr, str))

        if FwHeaderInputCondition and DotVerInputCondition:
            DEBUG (
              DEBUG_WARN,
              'EC Header and dot version assigned in same time. ',
              'Valid value would choose by the order.'
              )

        if FwHeaderInputCondition:
            DEBUG (DEBUG_TRACE, "Input the version information via Header.")
            InputMode = FW_HEADER_INPUT_TYPE
        elif DotVerInputCondition:
            InputMode = DOT_VERSION_DEC_INPUT_TYPE
        else:
            ErrorException (f'Need input valid EC FW version information.')

        return InputMode

    def __InitEcVer1Info (self) -> None:
        """ Initial the EC version for this object. (For Header Ver.1)

        Note:
            Platform ID and Build version is not reference.

        Args:
            None.

        Raises:
            ErrorException:
                EC dot version input is not matched the format.

        Returns:
            None.
        """
        Regex: Pattern = re.compile (EC_VERSION_1_REGEX)

        if self.__InputMode == FW_HEADER_INPUT_TYPE:
            self.__MajorVer = HexToDec (self.__Header.MajorVer)
            self.__MinorVer = HexToDec (self.__Header.MinorVer)
        elif self.__InputMode == DOT_VERSION_DEC_INPUT_TYPE:
            if not (Regex.match (self.__DotVerStr)):
                ErrorException (
                  f'Invalid EC version [{self.__DotVerStr}]. '
                  'The format should be XXX.XXX'
                  )

            self.__MajorVer, self.__MinorVer = self.__DotVerStr.split ('.')

        #
        # For version 1 string, capsule version mark two entry as 0.
        #
        self.__PlatId   = 0
        self.__BuildVer = 0

    def __InitEcVer2Info (self) -> None:
        """ Initial the EC version for this object. (For Header Ver.2)

        Args:
            None.

        Raises:
            ErrorException:
                EC dot version input is not matched the format.

        Returns:
            None.
        """
        Regex: Pattern = re.compile (EC_VERSION_2_REGEX)

        if self.__InputMode == FW_HEADER_INPUT_TYPE:
            self.__MajorVer = HexToDec (self.__Header.MajorVer)
            self.__MinorVer = HexToDec (self.__Header.MinorVer)
            self.__PlatId   = HexToDec (self.__Header.PlatformId)
            self.__BuildVer = HexToDec (self.__Header.BuildVer)
        elif self.__InputMode == DOT_VERSION_DEC_INPUT_TYPE:
            if not (Regex.match (self.__DotVerStr)):
                ErrorException (
                  f'Invalid EC version [{self.__DotVerStr}]. '
                  'The format should be XXX.XXX.XXX.XXX'
                  )

            self.__MajorVer, self.__MinorVer, self.__PlatId, \
                self.__BuildVer = self.__DotVerStr.split ('.')

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) EC header version is not in EC_FW_HDR_VERSION_LIST.
                (2) EC major version is not UINT8.
                (3) EC minor version is not UINT8.
                (4) EC platform ID is not UINT8.
                (5) EC build version is not UINT8.

        Returns:
            None.
        """
        #
        # Check the EC firmware header.
        #
        if self.__EcHdrVer not in EC_FW_HDR_VERSION_LIST:
            raise ValueError (
                    'Invalid EC header version. '
                    f'Should be one of {EC_FW_HDR_VERSION_LIST}.'
                    )

        #
        # Initial the EC version to assigned within this object.
        #
        if self.__EcHdrVer == EC_FW_HDR_VER2:
            self.__InitEcVer2Info ()
        else:
            self.__InitEcVer1Info ()

        #
        # Check the EC version meet the criteria.
        #
        if not IsUint8 (self.__MajorVer):
            raise ValueError (f'Major version [{self.__MajorVer}] not UINT8.')
        elif not IsUint8 (self.__MinorVer):
            raise ValueError (f'Minor version [{self.__MinorVer}] not UINT8.')
        elif not IsUint8 (self.__PlatId):
            raise ValueError (f'Platform ID [{self.__PlatId}] not UINT8.')
        elif not IsUint8 (self.__BuildVer):
            raise ValueError (f'Build version [{self.__BuildVer}] not UINT8.')
        else:
            DEBUG (DEBUG_TRACE, 'EC version check passed.')

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

        if self.__EcHdrVer == EC_FW_HDR_VER2:
            FullDecStr = str (HexToDec (self.GetHexVersion ()))
        else:
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
        MajorVer  : str = str ()
        MinorVer  : str = str ()
        PlatId    : str = str ()
        BuildVer  : str = str ()
        FullHexStr: str = str ()

        if self.__EcHdrVer == EC_FW_HDR_VER2:
            MajorVer   = DecToHex (self.__MajorVer)
            MinorVer   = ZeroPadding (DecToHex (self.__MinorVer), 2)
            PlatId     = ZeroPadding (DecToHex (self.__PlatId),   2)
            BuildVer   = ZeroPadding (DecToHex (self.__BuildVer), 2)
            FullHexStr = ''.join ([MajorVer, MinorVer, PlatId, BuildVer])
        else:
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

        if self.__EcHdrVer == EC_FW_HDR_VER2:
            DecDotVerStr = '.'.join ([
                                 str (self.__MajorVer),
                                 ZeroPadding (str (self.__MinorVer), 2),
                                 ZeroPadding (str (self.__PlatId),   2),
                                 ZeroPadding (str (self.__BuildVer), 2),
                                 ])
        else:
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

        if self.__EcHdrVer == EC_FW_HDR_VER2:
            HexDotVerStr = '.'.join ([
                                 DecToHex (self.__MajorVer),
                                 ZeroPadding (DecToHex (self.__MinorVer), 2),
                                 ZeroPadding (DecToHex (self.__PlatId),   2),
                                 ZeroPadding (DecToHex (self.__BuildVer), 2),
                                 ])
        else:
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
    'EcCapsuleVersion',
    ]
