## @file
#  Library support to get the Common/Platform configuration.
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

import os
import copy
from typing import Any, Mapping, MutableMapping

from CapsuleCommon import *

from CapsuleGenerate.Const import *

def UpdateNestedMapping (
    Origin          : Union[None, dict],
    Target          : Union[None, dict],
    IsIgnoreNotExist: bool = True,
    ) -> dict:
    """ Function to update the nested mapping data one to another.

    Args:
        Origin (Union[None, dict]):
            The source mapping data as the base.
        Target (Union[None, dict]):
            The target mapping data as the overwrite.
        IsIgnoreNotExist (bool, optional):
            Switch to decide the way to deal with not exist key.
            True  - Bypass this key-value data.
            False - Create this key-value data.
            Defaults to True.

    Raises:
        TypeError:
            (1) The source data is not dict type. (Base one)
            (2) The target data is not dict type. (Target one)
            (3) IsIgnoreNotExist should be bool type.

    Returns:
        dict:
            The updated nested mapping data.
    """
    if (Origin is not None) and (not isinstance (Origin, dict)):
        raise TypeError (f'Origin should be dict type.')
    elif (Target is not None) and (not isinstance (Target, dict)):
        raise TypeError (f'Target should be dict type.')
    elif not isinstance (IsIgnoreNotExist, bool):
        raise TypeError (f'IsIgnoreNotExist should be bool type.')

    SourceDict     : dict = {} if Origin is None else copy.deepcopy (Origin)
    DestinationDict: dict = {} if Target is None else copy.deepcopy (Target)
    NotExistDict   : dict = dict ()

    if (Origin is None) and (IsIgnoreNotExist):
        pass
    else:
        for Key, Value in DestinationDict.items():
            if Key not in SourceDict.keys ():
                if not IsIgnoreNotExist:
                    NotExistDict.update ({Key: Value})
            else:
                if not isinstance (SourceDict[Key], Mapping):
                    SourceDict[Key] = Value
                else:
                    SourceDict[Key] = UpdateNestedMapping (
                                        SourceDict[Key],
                                        Value,
                                        IsIgnoreNotExist,
                                        )

        #
        # It is not allow to change the structure during iteration.
        # Update the NotExistDict in the end.
        #
        SourceDict.update (NotExistDict)

    return SourceDict

class CapsuleConfig (object):
    def __init__ (self) -> None:
        """ Class to operation the common/platform config data.

        Note:
            The pre-requirement to use this package need to assign
            the following environment variables.
                - ENV_COMMON_CONFIG_KEY
                - ENV_PLATFORM_CONFIG_KEY

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__EnvVariable: dict = dict (copy.deepcopy (os.environ))

        self.__PreCheck ()

        self.__CommonCfg  : Config = \
            Config (
              FilePath = AbsPath (self.__EnvVariable[ENV_COMMON_CONFIG_KEY]),
              FileType = TOML_FILE_TYPE,
              )
        self.__PlatformCfg: Config = \
            Config (
              FilePath = AbsPath (self.__EnvVariable[ENV_PLATFORM_CONFIG_KEY]),
              FileType = TOML_FILE_TYPE,
              )
        self.__CommonPlatformData: dict = copy.deepcopy (self.__CommonCfg)

        self.__CommonPlatformData = self.__Overwrite ()

    @property
    def Common (self) -> MutableMapping[str, Any]:
        """ Return the common configuration data.

        Args:
            None.

        Raises:
            None.

        Returns:
            MutableMapping[str, Any]:
                The object of common config source data.
        """
        return self.__CommonCfg.Data

    @property
    def Platform (self) -> MutableMapping[str, Any]:
        """ Return the platform configuration data.

        Args:
            None.

        Raises:
            None.

        Returns:
            MutableMapping[str, Any]:
                The object of platform config source data.
        """
        return self.__PlatformCfg.Data

    @property
    def CommonPlatformData (self) -> dict:
        """ Return the overwritten config data.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                The config data after overwrite the platform data on
                common data.
        """
        return self.__CommonPlatformData

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) Path of common config data is not assigned.
                (2) Path of platform config data is not assigned.

        Returns:
            None.
        """
        if ENV_COMMON_CONFIG_KEY not in self.__EnvVariable:
            raise ValueError (f'{ENV_COMMON_CONFIG_KEY} is not assigned.')

        if ENV_PLATFORM_CONFIG_KEY not in self.__EnvVariable:
            raise ValueError (f'{ENV_PLATFORM_CONFIG_KEY} is not assigned.')

    def __GetFwCfgDataByVersion (
        self,
        FwCfgList: List[dict],
        FwVer    : Union[None, int] = None,
        ) -> Union[None, dict]:
        """ Get the firmware config data by version.

        Note:
            (1) Mark the IsEnabled as true in the last one.
            (2) To manage the different version capsule build flow on
                different firmware. 'Firmware' section within config
                file is allowed to manage by list of dict.
            (3) Version should be managed by int number.

        Args:
            FwCfgList (List[dict]):
                The different firmware config manage by list.
            FwVer (Union[None, int], optional):
                The assigned firmware version.
                If it is None, take the latest one.
                Defaults to None.

        Raises:
            TypeError:
                (1) Firmware config is not list type.
                (2) Firmware config member is not dict type.
            ValueError:
                (1) FW_VERSION_KEY not exist within config.
                (2) Firmware config version is not int type.
                (3) Duplicate version define in firmware config.

        Returns:
            Union[None, dict]:
                The specific firmware version config dict.
                If not found, return the None.
        """
        VerCfgMapping: Dict[int, dict]   = dict ()
        CurrVer      : int               = int ()
        LatestVer    : int               = int ()
        Result       : Union[None, dict] = dict ()

        if not isinstance (FwCfgList, list):
            raise TypeError (f'FW config in common should list of dict.')

        #
        # Create the Cfg version and Cfg mapping.
        #
        for FwCfg in FwCfgList:
            if not isinstance (FwCfg, MutableMapping):
                raise TypeError (f'FW config should be dict in list.')
            elif FW_VERSION_KEY not in FwCfg.keys ():
                raise ValueError (f'FW config need {FW_VERSION_KEY} key.')
            elif not isinstance (FwCfg[FW_VERSION_KEY], int):
                raise ValueError (f'FW config version should int type.')

            CurrVer = FwCfg[FW_VERSION_KEY]
            if CurrVer in VerCfgMapping.keys ():
                raise ValueError (f'Duplicated version {CurrVer}!')

            if CurrVer > LatestVer:
                LatestVer = CurrVer

            VerCfgMapping.update ({CurrVer: FwCfg})

        #
        # Logic to get the correspond version.
        #
        if FwVer is None:
            Result = VerCfgMapping[LatestVer]
        elif FwVer in VerCfgMapping.keys ():
            Result = VerCfgMapping[FwVer]
        else:
            Result = None

        #
        # Mark the IsEnabled key as True.
        #
        if Result is not None:
            Result[IS_ENABLED_KEY] = True

        return Result

    def __ProcessFirmwareSection (
        self,
        CommonData  : MutableMapping[str, Any],
        PlatformData: MutableMapping[str, Any],
        ) -> None:
        """ Handle the 'Firmware' section within config file.

        Note:
            (1) Take the correspond firmware config version from common
                config.
            (2) If platform config do not assign the Firmware section,
                assign empty dict.
            (3) The last step is to remove the invalid firmware assigned
                within the Platform config.

        Args:
            CommonData (MutableMapping[str, Any]):
                The common config data. (Base config)
            PlatformData (MutableMapping[str, Any]):
                The platform config data. (Platform specific config)

        Raises:
            None.

        Returns:
            None.
        """
        IsInvalid    : bool              = bool ()
        IsEnabled    : bool              = bool ()
        FwCfgVer     : Union[None, int]  = None
        FwCfgData    : Union[None, dict] = None
        InValidFwList: list              = None

        if FIRMWARE_SECTION not in CommonData.keys ():
            DEBUG (DEBUG_WARN, f'{FIRMWARE_SECTION} not in Common Cfg.')
            if FIRMWARE_SECTION in PlatformData.keys ():
                DEBUG (DEBUG_WARN, f'{FIRMWARE_SECTION} in Platform Cfg.')
                del PlatformData[FIRMWARE_SECTION]
            return

        if FIRMWARE_SECTION not in PlatformData.keys ():
            DEBUG (DEBUG_WARN, f'{FIRMWARE_SECTION} not in Platform Cfg.')
            PlatformData[FIRMWARE_SECTION] = dict ()

        for FwName, FwCfgList in CommonData[FIRMWARE_SECTION].items ():
            IsInvalid = True
            IsEnabled = False
            FwCfgVer  = None
            FwCfgData = None

            #
            # The following conditions mark it FW build is disabled.
            #   (1) Platform cfg do not have Firmware section.
            #   (2) Platform Firmware section do not have specific FW.
            #   (3) Platform specific FW do not mark IsEnabled.
            #   (4) Value of IsEnabled key is not bool type.
            #
            if FIRMWARE_SECTION not in PlatformData.keys ():
                DEBUG (DEBUG_WARN, f'[1] {FIRMWARE_SECTION} not in Platform Cfg.')
            elif FwName not in PlatformData[FIRMWARE_SECTION]:
                DEBUG (DEBUG_WARN, f'[2] {FwName} not in Platform Cfg.')
            elif IS_ENABLED_KEY not in PlatformData[FIRMWARE_SECTION][FwName]:
                DEBUG (DEBUG_WARN, f'[3] {IS_ENABLED_KEY} not in {FwName}.')
            elif not \
                isinstance (
                  PlatformData[FIRMWARE_SECTION][FwName][IS_ENABLED_KEY],
                  bool,
                  ):
                DEBUG (DEBUG_WARN, f'[4] {FwName} {IS_ENABLED_KEY} not bool.')
            else:
                IsInvalid = False

            if IsInvalid:
                CommonData[FIRMWARE_SECTION][FwName] = None
                continue

            #
            # Get the FW cfg based on Platform setting.
            #
            IsEnabled = PlatformData[FIRMWARE_SECTION][FwName][IS_ENABLED_KEY]

            if not IsEnabled:
                DEBUG (DEBUG_INFO, f'Platform not enabled [{FwName}] build.')
                PlatformData[FIRMWARE_SECTION][FwName] = None
                continue

            if FW_VERSION_KEY in PlatformData[FIRMWARE_SECTION][FwName]:
                FwCfgVer = PlatformData[FIRMWARE_SECTION][FwName][FW_VERSION_KEY]
            else:
                FwCfgVer = None

            FwCfgData = self.__GetFwCfgDataByVersion (FwCfgList, FwCfgVer)

            CommonData[FIRMWARE_SECTION][FwName] = FwCfgData

        #
        # Remove the invalid firmware keys within Platform config.
        #
        InValidFwList = \
            list (
              set (PlatformData[FIRMWARE_SECTION].keys ()) - \
              set (CommonData[FIRMWARE_SECTION].keys ()) \
              )

        for FwName in InValidFwList:
            del PlatformData[FIRMWARE_SECTION][FwName]
            DEBUG (
              DEBUG_WARN, f'{FwName} is invalid, remove it '
              'from Platform config.'
              )

    def __Overwrite (self) -> dict:
        """ Merge the platform config data on common config.

        Note:
            All section within config file should be key-value.
            And not allowed the list, except the 'Firmware' section.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                The config data after overwrite the platform data on
                common data.
        """
        CommonData  : MutableMapping[str, Any] = copy.deepcopy (self.Common)
        PlatformData: MutableMapping[str, Any] = copy.deepcopy (self.Platform)
        Result      : dict                     = dict ()

        #
        # Handle the Firmware Section.
        # Firmware section would allowed nested list structure to manage
        # different version of build method.
        #
        self.__ProcessFirmwareSection (CommonData, PlatformData)

        #
        # Other section should be key-value mapping.
        # Value part is not allowed enumerated or mapping object.
        #
        Result = UpdateNestedMapping (
                   CommonData,
                   PlatformData,
                   IsIgnoreNotExist = False,
                   )

        return Result
