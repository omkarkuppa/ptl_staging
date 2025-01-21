## @file
#  Script to generate Windows driver update package INF file.
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
#   - https://github.com/tianocore/edk2-pytool-library/blob/master/edk2toollib/windows/capsule/inf_generator.py
#   - https://github.com/tianocore/edk2-pytool-library/blob/master/edk2toollib/windows/capsule/inf_generator2.py
#   - https://learn.microsoft.com/en-us/windows-hardware/drivers/bringup/authoring-an-update-driver-package
#
##

import os
import re
import time
import uuid
import argparse
from pathlib import Path
from typing import List, Tuple, Union

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (c) 2023 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to create the INF file for Windows Driver Package.\n'

#
# Windows INF Common Section.
#
SEC_VERSION           : str = 'Version'
SEC_MANUFACTURER      : str = 'Manufacturer'
SEC_FIRMWARE_PREFIX   : str = 'Firmware'
SEC_SOURCE_DISKS_NAME : str = 'SourceDisksNames'
SEC_SOURCE_DISKS_FILES: str = 'SourceDisksFiles'
SEC_DESTINATION_DIRS  : str = 'DestinationDirs'
SEC_STRINGS           : str = 'Strings'

#
# Pre-defined default Value.
#
DEFAULT_DISK_NAMES   : str = 'Firmware Update'
DEFAULT_PROVIDER     : str = 'Intel'
DEFAULT_MANUFACTURER : str = 'Intel'
DEFAULT_DIRID_WINDOWS: str = '10'
DEFAULT_REG_DWORD    : str = '0x00010001'

EXTENSION_WIN_CAT_FILE: str = 'cat'

#
# Exit code of this script.
#
EXIT_SUCCESS: int = 0x00

class _UpdatePayload (object):
    def __init__ (self) -> None:
        """ Class for common interface to update the section data.

        Note:
            (1) IsAllowDuplicated only work on the data entry.
                Comment is allowed duplicated by default.
            (2) The object provide the common interface to update the
                configuration data within specific section.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.Section          : str  = str ()
        self.MappingData      : list = list ()
        self.IsAllowDuplicated: bool = False

class _StringInfo (object):
    def __init__ (
        self,
        DiskName: str = DEFAULT_DISK_NAMES,
        Provider: str = DEFAULT_PROVIDER,
        MfgName : str = DEFAULT_MANUFACTURER,
        ) -> None:
        """ Class for the 'Strings' section.

        Args:
            DiskName (str, optional):
                The disk name for this update package.
                Defaults to DEFAULT_DISK_NAMES.
            Provider (str, optional):
                The update package provider.
                Defaults to DEFAULT_PROVIDER.
            MfgName (str, optional):
                The manufacture of the capsule correspond device.
                Defaults to DEFAULT_MANUFACTURER.

        Raises:
            None.

        Returns:
            None.
        """
        self.DiskName     : str = DiskName
        self.Provider     : str = Provider
        self.MfgName      : str = MfgName
        self.DIRID_WINDOWS: str = DEFAULT_DIRID_WINDOWS
        self.REG_DWORD    : str = DEFAULT_REG_DWORD

        #
        # List of [key, value] for user customize append.
        #
        self.__AppendVarList: list = list ()

    def AppendConstString (self, Key: str, Value: str) -> None:
        """ Expose function call to append the key-value string.

        Args:
            Key (str):
                The specific string of key part.
            Value (str):
                The specific string of value part.

        Raises:
            TypeError:
                (1) "Key" caller input is not str type.
                (2) "Value" caller input is not str type.

        Returns:
            None.
        """
        if not isinstance (Key, str):
            raise TypeError (f'The Key part should be str type.')
        elif not isinstance (Value, str):
            raise TypeError (f'The Value part should be str type.')

        self.__AppendVarList.append ([Key, Value])

    @property
    def Payload (self) -> List[_UpdatePayload]:
        """ Get the config file update payloads.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[_UpdatePayload]:
                Return the list of update payloads to update the config
                INI file by sequence.
        """
        PayloadList: list           = list ()
        Payload    : _UpdatePayload = None

        #
        # Payload for localizable strings.
        #
        Payload              = _UpdatePayload ()
        Payload.Section      = SEC_STRINGS
        Payload.MappingData  = [
            ['; localizable', ''                  ], # Comment
            ['DiskName',      f'"{self.DiskName}"'],
            ['Provider',      f'"{self.Provider}"'],
            ['MfgName',       f'"{self.MfgName}"' ],
            ]
        #
        # Add the user data to append.
        #
        Payload.MappingData += self.__AppendVarList
        PayloadList.append (Payload)

        #
        # Payload for non-localizable string.
        #
        Payload              = _UpdatePayload ()
        Payload.Section      = SEC_STRINGS
        Payload.MappingData  = [
            ['; non-localizable', '',                    ], # Comment
            ['DIRID_WINDOWS',     f'{self.DIRID_WINDOWS}'],
            ['REG_DWORD',         f'{self.REG_DWORD}'    ],
            ]
        PayloadList.append (Payload)

        return PayloadList

class _PkgInfo (object):
    def __init__ (
        self,
        DotVer     : str,
        CatFileName: str,
        Arch       : str,
        ) -> None:
        """ Class to provide the update package information.

        Args:
            DotVer (str):
                The Dot version string. (Format should be XX.XX.XX.XX)
            CatFileName (str):
                The Windows CAT file name include the extension.
            Arch (str):
                Architecture of firmware for the correspond hardware.

        Raises:
            None.

        Returns:
            None.
        """
        #
        # Local variables.
        #
        Date: str = time.strftime ('%m/%d/%Y')

        #
        # Private variables.
        #
        self.__DotVer     : str = DotVer
        self.__CatFileName: str = CatFileName
        self.__Arch       : str = Arch

        #
        # Public variables.
        #
        self.Signature   : str = '"$WINDOWS NT$"'
        self.Class       : str = 'Firmware'
        self.ClassGuid   : str = '{F2E7DD72-6468-4E36-B6F1-6488F42C1B52}'
        self.Provider    : str = '%Provider%'
        self.DriverVer   : str = f'{Date},{self.__DotVer}'
        self.PnpLockdown : str = str (int (True))
        self.CatalogFile : str = f'{self.__CatFileName}'
        self.Manufacturer: str = f'Firmware,NT{self.__Arch}'

    @property
    def Payload (self) -> List[_UpdatePayload]:
        """ Get the config file update payloads.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[_UpdatePayload]:
                Return the list of update payloads to update the config
                INI file by sequence.
        """
        PayloadList: list           = list ()
        Payload    : _UpdatePayload = None

        #
        # Version Section.
        #
        Payload             = _UpdatePayload ()
        Payload.Section     = SEC_VERSION
        Payload.MappingData = [
            ['Signature',   self.Signature  ],
            ['Class',       self.Class      ],
            ['ClassGuid',   self.ClassGuid  ],
            ['Provider',    self.Provider   ],
            ['DriverVer',   self.DriverVer  ],
            ['PnpLockdown', self.PnpLockdown],
            ['CatalogFile', self.CatalogFile],
            ]
        PayloadList.append (Payload)

        #
        # Manufacturer Section.
        #
        Payload             = _UpdatePayload ()
        Payload.Section     = SEC_MANUFACTURER
        Payload.MappingData = [
            ['%MfgName%', self.Manufacturer],
            ]
        PayloadList.append (Payload)

        return PayloadList

class _Firmware (object):
    #
    # The pre-defined property for _Firmware object.
    #
    FwInstanceId    : int = 0
    TAG_PREFIX      : str = 'Fw'
    DESC_TAG_POSTFIX: str = 'Desc'

    def __init__ (
        self,
        Guid       : str,
        HexVer     : str,
        CapFileName: str,
        Description: str,
        Arch       : str,
        IsRollback : bool = False,
        ) -> None:
        """ Class to represent the firmware object.

        Args:
            Guid (str):
                The GUID value of this FMP device.
            HexVer (str):
                The HEX version string of this firmware image.
            CapFileName (str):
                The capsule file name of this firmware image.
            Description (str):
                The description of this firmware image.
            Arch (str):
                The architecture of this firmware image.
            IsRollback (bool, optional):
                Is firmware update support the rollback.
                True  - Allow firmware to do the downgrade.
                False - Forbidden firmware to do the downgrade.
                Defaults to False.

        Raises:
            None.

        Returns:
            None.
        """
        _Firmware.FwInstanceId += 1

        self.__InstanceId    : int  = _Firmware.FwInstanceId
        self.__Guid          : str  = Guid.upper ()
        self.__HexVer        : str  = FormatHex (HexVer, IsPadding = True)
        self.__CapFileName   : str  = CapFileName
        self.__Description   : str  = Description
        self.__Arch          : str  = Arch
        self.__IsRollBack    : bool = IsRollback

    @property
    def Tag (self) -> str:
        """ Return the specific tag for firmware instance.

        Note:
            It is a combination with TAG_PREFIX and FwInstanceId.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The tag string of this firmware instance.
        """
        return f'{_Firmware.TAG_PREFIX}{self.__InstanceId}'

    @property
    def DescriptionTag (self) -> str:
        """ Return the specific tag for description string.

        Note:
            It is a combination with Tag and DESC_TAG_POSTFIX.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The description tag string of this firmware instance.
        """
        return f'{self.Tag}{_Firmware.DESC_TAG_POSTFIX}'

    @property
    def Description (self) -> str:
        """ Return the description content for this firmware instance.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The description of this firmware instance.
        """
        return self.__Description

    @property
    def Payload (self) -> List[_UpdatePayload]:
        """ Get the config file update payloads.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[_UpdatePayload]:
                Return the list of update payloads to update the config
                INI file by sequence.
        """
        FIRMWARE_SECTION: str  = f'{SEC_FIRMWARE_PREFIX}.NT{self.__Arch}'
        INSTALL_TAG     : str  = f'{self.Tag}_Install'
        INSTALL_NT_TAG  : str  = f'{INSTALL_TAG}.NT'
        COPY_FILES_TAG  : str  = f'{self.Tag}_CopyFiles'
        HW_REG_TAG      : str  = f'{INSTALL_NT_TAG}.Hw'

        PayloadList: list           = list ()
        Payload    : _UpdatePayload = None

        #
        # Firmware.NTXXXX section.
        #
        Payload             = _UpdatePayload ()
        Payload.Section     = FIRMWARE_SECTION
        Payload.MappingData = [
            [f'%{self.DescriptionTag}%', f'{INSTALL_TAG},UEFI\\RES_{{{self.__Guid}}}']
            ]
        PayloadList.append (Payload)

        #
        # XX_Install.NT section.
        #
        Payload             = _UpdatePayload ()
        Payload.Section     = INSTALL_NT_TAG
        Payload.MappingData = [['CopyFiles', COPY_FILES_TAG]]
        PayloadList.append (Payload)

        #
        # XX_CopyFiles section.
        #
        Payload             = _UpdatePayload ()
        Payload.Section     = COPY_FILES_TAG
        Payload.MappingData = [[f'{self.__CapFileName}', '']]
        PayloadList.append (Payload)

        #
        # Add the register for this firmware.
        #
        PayloadList += self.__AddRegister (HW_REG_TAG)

        #
        # SourceDisksNames section.
        #
        Payload             = _UpdatePayload ()
        Payload.Section     = SEC_SOURCE_DISKS_NAME
        Payload.MappingData = [['1', '%DiskName%']]
        PayloadList.append (Payload)

        #
        # SourceDisksFiles section.
        #
        Payload             = _UpdatePayload ()
        Payload.Section     = SEC_SOURCE_DISKS_FILES
        Payload.MappingData = [[f'{self.__CapFileName}', '1']]
        PayloadList.append (Payload)

        #
        # DestinationDirs section.
        #
        Payload             = _UpdatePayload ()
        Payload.Section     = SEC_DESTINATION_DIRS
        Payload.MappingData = [
            ['DefaultDestDir', f'{WIN_DIR_DRIVER_STORE_ID}']
            ]
        PayloadList.append (Payload)

        return PayloadList

    def __AddRegister (self, HwSection: str) -> List[_UpdatePayload]:
        """ Function to add the register operation.

        Args:
            HwSection (str):
                The hardware section name for this firmware within
                config INI file.

        Raises:
            None.

        Returns:
            List[_UpdatePayload]:
                The list of config file update payload for register.
        """
        COMMON_REG_TAG  : str  = f'{self.Tag}_AddReg'
        ROLLBACK_REG_TAG: str  = f'{self.Tag}_DowngradePolicy_AddReg'

        PayloadList: list           = list ()
        Payload    : _UpdatePayload = None

        #
        # Add Common Register
        #
        Payload                   = _UpdatePayload ()
        Payload.Section           = HwSection
        Payload.MappingData       = [['AddReg', COMMON_REG_TAG]]
        Payload.IsAllowDuplicated = True
        PayloadList.append (Payload)

        Payload             = _UpdatePayload ()
        Payload.Section     = COMMON_REG_TAG
        Payload.MappingData = [
            [f'HKR,,FirmwareVersion,%REG_DWORD%,{self.__HexVer}',                         ''],
            [f'HKR,,FirmwareId,,{{{self.__Guid}}}',                                       ''],
            [f'HKR,,FirmwareFilename,,%{WIN_DIR_DRIVER_STORE_ID}%\\{self.__CapFileName}', ''],
            ]
        PayloadList.append (Payload)

        #
        # Add Rollback Enabled Register
        #
        Payload                   = _UpdatePayload ()
        Payload.Section           = HwSection
        Payload.MappingData       = [['AddReg', ROLLBACK_REG_TAG]]
        Payload.IsAllowDuplicated = True
        if self.__IsRollBack:
            PayloadList.append (Payload)

        Payload             = _UpdatePayload ()
        Payload.Section     = ROLLBACK_REG_TAG
        Payload.MappingData = [
            [f'HKLM,SYSTEM\\CurrentControlSet\\Control\\FirmwareResources\\{{{self.__Guid}}},Policy,%REG_DWORD%,1', ''],
            ]
        if self.__IsRollBack:
            PayloadList.append (Payload)

        return PayloadList

class _WindowsDriverInf (INICfgFile):
    #
    # The pre-defined property for _WindowsDriverInf object.
    #
    TOP_ORDER   : list = [
        SEC_VERSION,
        SEC_MANUFACTURER,
        'Firmware',
        ]
    BOTTOM_ORDER: list = [
        SEC_SOURCE_DISKS_NAME,
        SEC_SOURCE_DISKS_FILES,
        SEC_DESTINATION_DIRS,
        SEC_STRINGS,
        ]

    def __init__ (
        self,
        PkgInfo     : _PkgInfo,
        StringInfo  : _StringInfo,
        FirmwareList: List[_Firmware],
        ) -> None:
        """ Class to represent the Windows Driver INF file.

        Args:
            PkgInfo (_PkgInfo):
                The object to provide the update package information.
            StringInfo (_StringInfo):
                The object to represent the 'Strings' section.
            FirmwareList (List[_Firmware]):
                The list of the _Firmware object include within this
                Windows driver update package.

        Raises:
            None.

        Returns:
            None.
        """
        super ().__init__ ()
        self.__PkgInfo     : _PkgInfo        = PkgInfo
        self.__StringInfo  : _StringInfo     = StringInfo
        self.__FirmwareList: List[_Firmware] = FirmwareList

    def __UpdatePayload (self) -> None:
        """ Inner function to update all the payload from object.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        Fw         : _Firmware            = None
        PayloadList: List[_UpdatePayload] = None
        Payload    : _UpdatePayload       = None


        AllPayloadList: List[list] = [
            self.__PkgInfo.Payload,
            self.__StringInfo.Payload,
            ]

        #
        # Append all the Firmware payload.
        #
        for Fw in self.__FirmwareList:
            AllPayloadList.append (Fw.Payload)

        #
        # Apply all the update payload into Windows INF file.
        #
        for PayloadList in AllPayloadList:
            for Payload in PayloadList:
                self.UpdateDataToSection (
                       Payload.Section,
                       Payload.MappingData,
                       Payload.IsAllowDuplicated,
                       )

    def __GetSectionAndRemain (
        self,
        SourceOrder: list,
        TargetOrder: list,
        ) -> Tuple[list, list]:
        """ Return the request target section name and remain part.

        Note:
            Find the Target section by list order.
                - If found, add them into Result.
                - If not found, return it as Remain part.

        Args:
            SourceOrder (list):
                List of source INF section name.
            TargetOrder (list):
                List of target INF section name.

        Raises:
            None.

        Returns:
            Tuple[list, list]:
                list:
                    The list of section found from source.
                list:
                    The list of section not matched to one of targe.
        """
        Result: list = list ()
        Remain: dict = dict.fromkeys (SourceOrder)
        Source: str  = None
        Target: str  = None

        for Target in TargetOrder:
            for Source in SourceOrder:
                if Source.startswith (Target):
                    Result.append (Source)

        for Section in Result:
            if Section in Remain:
                del Remain[Section]

        return Result, list (Remain.keys ())

    def __OrderSection (self) -> None:
        """ Function to re-order the section with required one.

        Note:
            The order of Windows driver INF file is by:
                - TOP_ORDER
                - Remain part
                - BOTTOM_ORDER

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        TopSection   : list = list ()
        BottomSection: list = list ()
        RemainSection: list = copy.deepcopy (self.SectionNameList)
        SectionList  : list = list ()
        SectionName  : str  = None

        TopSection, RemainSection = \
            self.__GetSectionAndRemain (
                   RemainSection,
                   _WindowsDriverInf.TOP_ORDER,
                   )
        BottomSection, RemainSection = \
            self.__GetSectionAndRemain (
                   RemainSection,
                   _WindowsDriverInf.BOTTOM_ORDER,
                   )

        #
        # Modified the SectionNameList
        #
        self.SectionNameList = TopSection + RemainSection + BottomSection

        for SectionName in self.SectionNameList:
            for Section in self.SectionList:
                if Section.Name == SectionName:
                    SectionList.append (Section)

        #
        # Modified the SectionList
        #
        self.SectionList = SectionList

    def Save (
        self,
        FilePath: str,
        FileMode: str,
        ) -> None:
        """ Save the Windows driver INF file into storage.

        Args:
            FilePath (str):
                The path to export the Windows driver INF file.
            FileMode (str):
                The mode to open the INF file.

        Raises:
            None.

        Returns:
            None.
        """
        #
        # Clean all the section to rebuild.
        #
        self.SectionNameList = list ()
        self.SectionList     = list ()

        self.__UpdatePayload ()
        self.__OrderSection ()

        super ().Save (FilePath, FileMode)

class WindowsDriverInfGenerator (object):
    DOT_VER_STR_REGEX_STR: str  = r'^(\d){1,5}[.](\d){1,5}[.](\d){1,5}[.](\d){1,5}$'
    SUPPORTED_ARCH_LIST  : list = ['amd64', 'arm', 'ARM64']

    def __init__ (
        self,
        Arch        : str,
        CatFileName : str,
        DotVer      : str,
        Provider    : str = DEFAULT_PROVIDER,
        Manufacturer: str = DEFAULT_MANUFACTURER,
        ) -> None:
        """ Class to generate the Windows Driver INF file.

        Args:
            Arch (str):
                The architecture of this firmware image.
            CatFileName (str):
                The Windows CAT file name include the extension.
            DotVer (str):
                The Dot version string. (Format should be XX.XX.XX.XX)
            Provider (str, optional):
                The update package provider.
                Defaults to DEFAULT_PROVIDER.
            Manufacturer (str, optional):
                The manufacture of the capsule correspond device.
                Defaults to DEFAULT_MANUFACTURER.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Arch        : str = Arch
        self.__CatFileName : str = CatFileName
        self.__DotVer      : str = DotVer
        self.__Provider    : str = Provider
        self.__Manufacturer: str = Manufacturer

        self.__PreCheck ()

        self.__PkgInfo: _PkgInfo = _PkgInfo (
                                     DotVer      = self.__DotVer,
                                     CatFileName = self.__CatFileName,
                                     Arch        = self.__Arch,
                                     )

        self.__StringInfo: _StringInfo = _StringInfo (
                                           Provider = self.__Provider,
                                           MfgName  = self.__Manufacturer,
                                           )

        self.__FirmwareList: List[_Firmware] = list ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) CatFileName is not str type.
                (2) DotVer is not str type.
                (3) Provider is not str type.
                (4) Manufacturer is not str type.
            ValueError:
                (1) Arch is not listed within SUPPORTED_ARCH_LIST.
                (2) DotVer string is not XXXXX.XXXXX.XXXXX.XXXXX format.
                (3) DotVer Major is not UINT16.
                (4) DotVer Minor is not UINT16.
                (5) DotVer Revision is not UINT16.
                (6) DotVer Build is not UINT16.
                (7) DotVer input 0.0.0.0 which is not valid.

        Returns:
            None.
        """
        MajorVer  : int     = int ()
        MinorVer  : int     = int ()
        Revision  : int     = int ()
        BuildVer  : int     = int ()
        VerStrList: list    = list ()
        IsFound   : bool    = bool ()
        Arch      : str     = str ()
        Regex     : Pattern = \
            re.compile (
                 WindowsDriverInfGenerator.DOT_VER_STR_REGEX_STR,
                 )

        if not isinstance (self.__CatFileName, str):
            raise TypeError ('CatFileName should be str type.')
        elif not isinstance (self.__DotVer, str):
            raise TypeError ('DotVer should be str type.')
        elif not isinstance (self.__Provider, str):
            raise TypeError ('Provider should be str type.')
        elif not isinstance (self.__Manufacturer, str):
            raise TypeError ('Manufacturer should be str type.')

        #
        # Check the Arch value and standardize it.
        #
        IsFound, Arch = \
            SearchValueInList (
              WindowsDriverInfGenerator.SUPPORTED_ARCH_LIST,
              self.__Arch,
              )
        if not IsFound:
            raise ValueError (
                    'Unsupported architecture. Supported: '
                    f'{WindowsDriverInfGenerator.SUPPORTED_ARCH_LIST}'
                    )
        self.__Arch = Arch

        #
        # Check the DOT version format.
        #
        if not (Regex.match (self.__DotVer)):
            raise ValueError (
                    f'Invalid dot version string [{self.__DotVer}]. '
                    'The format should be XXXXX.XXXXX.XXXXX.XXXXX'
                    )
        VerStrList = self.__DotVer.split ('.')
        MajorVer   = int (VerStrList[0])
        MinorVer   = int (VerStrList[1])
        Revision   = int (VerStrList[2])
        BuildVer   = int (VerStrList[3])

        if not IsUint16 (MajorVer):
            raise ValueError (f'Major version should be UINT16 [{MajorVer}]')
        elif not IsUint16 (MinorVer):
            raise ValueError (f'Minor version should be UINT16 [{MinorVer}]')
        elif not IsUint16 (Revision):
            raise ValueError (f'Revision should be UINT16 [{Revision}]')
        elif not IsUint16 (BuildVer):
            raise ValueError (f'BuildVer should be UINT16 [{BuildVer}]')
        elif ((MajorVer == 0) and (MinorVer == 0) and \
            (Revision == 0) and (BuildVer == 0)):
            raise ValueError ('0.0.0.0 is not valid dot version value.')

    def AddFirmware (
        self,
        Guid       : Union[str, uuid.UUID],
        HexVer     : str,
        CapFileName: str,
        Description: str,
        IsRollback : bool = False,
        ) -> None:
        """ Expose the API to add the capsule image into update package.

        Args:
            Guid (str):
                The GUID value of this FMP device.
            HexVer (str):
                The HEX version string of this firmware image.
            CapFileName (str):
                The capsule file name of this firmware image.
            Description (str):
                The description of this firmware image.
            IsRollback (bool, optional):
                Is firmware update support the rollback.
                True  - Allow firmware to do the downgrade.
                False - Forbidden firmware to do the downgrade.
                Defaults to False.

        Raises:
            TypeError:
                (1) GUID is not UUID/str type.
                (2) HexVer is not str type.
                (3) CapFileName is not str type.
                (4) Description is not str type.
                (5) IsRollback is not bool type.
            ValueError:
                HexVer is not UINT32.

        Returns:
            None.
        """
        EsrtGuid: uuid.UUID = None

        if not isinstance (Guid, (str, uuid.UUID)):
            raise TypeError ('GUID should be UUID/str type.')
        elif not isinstance (HexVer, str):
            raise TypeError ('HexVer should be str type.')
        elif not isinstance (CapFileName, str):
            raise TypeError ('CapFileName should be str type.')
        elif not isinstance (Description, str):
            raise TypeError ('Description should be str type.')
        elif not isinstance (IsRollback, bool):
            raise TypeError ('IsRollback should be bool type.')
        elif not IsUint32 (HexToDec (HexVer)):
            raise ValueError ('HexVer should be UINT32.')

        if isinstance (Guid, str):
            EsrtGuid = uuid.UUID (Guid) # Check GUID string could be converted.
        else:
            EsrtGuid = Guid

        Fw: _Firmware = _Firmware (
                          Guid        = str (EsrtGuid),
                          HexVer      = HexVer,
                          CapFileName = CapFileName,
                          Description = Description,
                          Arch        = self.__Arch,
                          IsRollback  = IsRollback,
                          )

        self.__FirmwareList.append (Fw)
        self.__StringInfo.AppendConstString (
                            Fw.DescriptionTag,
                            f'"{Fw.Description}"',
                            )

    def Save (self, OutputPath: Union[str, os.PathLike]) -> None:
        """ Save the Windows driver INF file into storage.

        Args:
            OutputPath (Union[str, os.PathLike]):
                The path to export the Windows driver INF file.

        Raises:
            None.

        Returns:
            None.
        """
        HeaderFile  : File = None
        HederStrList: list = list ()
        SavePath    : str  = AbsPath (OutputPath)

        Config: _WindowsDriverInf = \
            _WindowsDriverInf (
              PkgInfo       = self.__PkgInfo,
              StringInfo    = self.__StringInfo,
              FirmwareList  = self.__FirmwareList,
              )

        #
        # The header of Windows INF file.
        #
        HederStrList = [
            ';\n',
            f'; {Path (OutputPath).stem}.inf\n',
            ';\n',
            f'; Copyright (C) {time.strftime ("%Y")} Intel Corporation. All rights reserved.\n',
            ';\n',
            ]

        HeaderFile = File (FileContent = HederStrList)
        HeaderFile.Save (SavePath)

        #
        # Expose the config file content.
        #
        Config.Save (SavePath, FileMode = 'a')

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'WindowsDriverInfGenerator',
    ]

if __name__ == '__main__':
    def GetArgs () -> argparse.Namespace:
        """ Get the user input arguments.

        Args:
            None.

        Raises:
            None.

        Returns:
            Args [argparse.Namespace]:
                The argument object parsing from user input.
        """
        Parser: argparse.ArgumentParser = argparse.\
            ArgumentParser (
              prog            = __prog__,
              description     = __copyright__ + __description__ + __version__,
              formatter_class = argparse.RawTextHelpFormatter,
              )
        Parser.add_argument (
                 '-V',
                 '-v',
                 '--Version',
                 action  = 'version',
                 version = __version__,
                 )
        Parser.add_argument (
                 '-c',
                 '-C',
                 '--capsule-file',
                 dest     = 'CapsuleFilePath',
                 help     = 'Path of capsule image file.',
                 required = True,
                 )
        Parser.add_argument (
                 '-g',
                 '-G',
                 '--guid',
                 dest     = 'Guid',
                 help     = 'The GUID of the FMP device to apply the capsule.',
                 required = True,
                 )
        Parser.add_argument (
                 '-p',
                 '-P',
                 '--provider',
                 dest     = 'Provider',
                 help     = 'The provider of this Windows update package.',
                 required = True,
                 )
        Parser.add_argument (
                 '-m',
                 '-M',
                 '--manufacturer',
                 dest     = 'Manufacturer',
                 help     = 'The manufacturer of this Windows update package.',
                 required = True,
                 )
        Parser.add_argument (
                 '-d',
                 '-D',
                 '--dotver',
                 dest     = 'DotVer',
                 help     = 'Dot version of this Windows update package.',
                 required = True,
                 )
        Parser.add_argument (
                 '-hex',
                 '-Hex',
                 '--hexver',
                 dest     = 'HexVer',
                 help     = 'The HEX version of the capsule firmware image.',
                 required = True,
                 )
        Parser.add_argument (
                 '-desc',
                 '-Desc',
                 '--description',
                 dest     = 'Description',
                 help     = 'Description of this Windows update package.',
                 required = True,
                 )
        Parser.add_argument (
                 '-o',
                 '-O',
                 '--output',
                 dest     = 'OutputPath',
                 help     = 'Output path of this INF file.',
                 required = True,
                 )

        return Parser.parse_args ()

    Args: argparse.Namespace = GetArgs ()

    Arch        : str = 'amd64'
    Provider    : str = Args.Provider
    Manufacturer: str = Args.Manufacturer
    EsrtGuid    : str = Args.Guid
    DotVerStr   : str = Args.DotVer
    HexVerStr   : str = Args.HexVer
    Description : str = Args.Description
    OutputPath  : str = AbsPath (Args.OutputPath)
    CatFileName : str = \
        CombineFileName (
          GetFileNameAndExt (OutputPath)[0],
          Extension = EXTENSION_WIN_CAT_FILE,
          )
    CapFilename : str = os.path.basename (Args.CapsuleFilePath)

    DEBUG (DEBUG_INFO, f'-------- Windows Driver INF Generate Start --------')

    Generator: WindowsDriverInfGenerator = \
        WindowsDriverInfGenerator (
          Arch         = Arch,
          CatFileName  = CatFileName,
          DotVer       = DotVerStr,
          Provider     = Provider,
          Manufacturer = Manufacturer,
          )

    Generator.AddFirmware (
                Guid        = EsrtGuid,
                HexVer      = HexVerStr,
                CapFileName = CapFilename,
                Description = Description,
                IsRollback  = False,        # Force to set this option as False.
                )

    Generator.Save (OutputPath)

    DEBUG (DEBUG_INFO, f'--------- Windows Driver INF Generate End ---------')

    sys.exit (EXIT_SUCCESS)
