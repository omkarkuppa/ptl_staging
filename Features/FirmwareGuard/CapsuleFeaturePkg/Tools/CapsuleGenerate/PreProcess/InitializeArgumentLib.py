## @file
#  Library support to initial the argument service.
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

import sys
from typing import Tuple

from CapsuleCommon import *

from CapsuleGenerate._Version import *
from CapsuleGenerate._Global import *
from CapsuleGenerate.PkgInfo import *
from CapsuleGenerate.Const import *
from CapsuleGenerate.Model.FmpDevice import *

BLD_OP_FLOW_KEY: str = 'OpFlow'
FW_FMP_OBJ_KEY : str = 'FmpDevice'

class InitialArgument (object):
    def __init__ (self):
        """ Class to initial the argument service.

        Note:
            All method define in here should be staticmethod.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        pass

    @staticmethod
    def CreateBldCmdFmpMapping (
        FwComponentDict: dict,
        ) -> Tuple[dict, dict, dict]:
        """ Return the ArgsList, Mapping, and BldCmdFmpInstance mapping.

        Args:
            FwComponentDict (dict):
                The dictionary of BuildCmd/FmpInstance etc.
                Sample:
                    FW_COMPONENT_DICT: dict = {
                        EC_DEVICE_NAME: {
                            BLD_OP_FLOW_KEY: StandaloneCapsuleBuild,
                            FW_FMP_OBJ_KEY : EcFmpDevice
                            }
                        },
                        ...
                    }

        Raises:
            None.

        Returns:
            Tuple[dict, dict, dict]:
                dict:
                    BuildCmd and list of Argument mapping.
                    Sample:
                        ArgumentDict = {
                            BUILD_EC_CMD: ArgumentList
                            ...
                        }
                dict:
                    FmpInstanceName and BuildCmd mapping.
                    Sample:
                        MappingDict = {
                            BUILD_EC_CMD      : EC_FMP_DEVICE_NAME,
                            EC_FMP_DEVICE_NAME: BUILD_EC_CMD,
                            ...
                        }
                dict:
                    BuildCmd and FmpInstance mapping.
                    Sample:
                        BldCmdFmpInstDict = {
                            BUILD_EC_CMD: EcFmpInstance
                            ...
                        }
        """
        def GetLatestFwInfo (FwInfoList: List[dict]) -> dict:
            """ Return latest build operation of firmware.

            Note:
                This internal function assume FW_VERSION_KEY is exist,
                it would raise exception if not found this key.

            Args:
                FwInfoList (List[dict]):
                    List of build operation of firmware.
                    If list is empty, return the None directly.

            Raises:
                None.

            Returns:
                dict:
                    Latest build operation version of firmware.
            """
            if len (FwInfoList) == 0:
                return None

            FwVersion : int  = int (FwInfoList[0][FW_VERSION_KEY])
            FwInfo    : dict = FwInfoList[0]
            CurrFwInfo: dict = dict ()

            for Idx in range (len (FwInfoList)):
                CurrFwInfo = FwInfoList[Idx]
                if int (CurrFwInfo[FW_VERSION_KEY]) > int (FwVersion):
                    FwVersion = CurrFwInfo[FW_VERSION_KEY]
                    FwInfo    = CurrFwInfo

            return FwInfo

        ArgsDict     : Dict[str, List[Argument]]   = dict ()
        MappingDict  : Dict[str, str]              = dict ()
        BldFmpMapping: Dict[str, FmpDevice]        = dict ()
        LatestFwInfo : dict                        = dict ()
        ArgsList     : Union[None, List[Argument]] = None
        FmpInstance  : Union[None, FmpDevice]      = None

        #
        # Iteration all the firmware
        #
        for FwName, FwInfo in gFmpFirmwareInfo.items ():
            ArgsList    = None
            FmpInstance = None

            #
            # If FwInfo is None or firmware is not enabled.
            # Set to None.
            #
            if (FwInfo is None) or \
               ((FwInfo is not None) and (not FwInfo[IS_ENABLED_KEY])):
                LatestFwInfo = GetLatestFwInfo (gCommonFmpFwInfo[FwName])
                BuildCmd     = LatestFwInfo[FW_BUILD_OP_CODE_KEY]

                DEBUG (
                  DEBUG_WARN,
                  f'FW build cmd \"{BuildCmd}\" is not enabled.'
                  )
            else:
                BuildCmd = FwInfo[FW_BUILD_OP_CODE_KEY]
                ArgsList = list ()

                #
                # Create the ArgumentList for this OpCode.
                #
                for Cmd, Datum in FwInfo[FW_CMD_SECTION_KEY].items ():
                    if Cmd in CMD_HELPER_KEY_LIST:
                        continue

                    ArgsList.append (
                               Argument (
                                 Command     = Cmd,
                                 Compulsory  = Datum[CMD_COMPULSORY_KEY],
                                 Description = Datum[CMD_DESCRIPTION_KEY],
                                 Syntax      = Datum[CMD_SYNTAX_KEY],
                                 Default     = Datum[CMD_DEFAULT_VAL_KEY],
                                 )
                               )

                if FwName in FwComponentDict.keys ():
                    FmpInstance = FwComponentDict[FwName][FW_FMP_OBJ_KEY]
                else:
                    raise ValueError (
                            f'FW {FwName} not defined the FmpDevice instance.'
                            )

            ArgsDict.update ({BuildCmd: ArgsList})
            MappingDict.update ({BuildCmd: FwName, FwName: BuildCmd})
            BldFmpMapping.update ({BuildCmd: FmpInstance})

        return ArgsDict, MappingDict, BldFmpMapping

    @staticmethod
    def CreateArgumentHelper (ArgumentDict: dict, MappingDict: dict) -> dict:
        """ Generate the ArgumentHelper for this platform.

        Args:
            ArgumentDict (dict):
                BuildCmd and list of Argument mapping.
            MappingDict (dict):
                FmpInstanceName and BuildCmd mapping.

        Raises:
            None.

        Returns:
            dict:
                Argument helper dictionary.
                Sample:
                    BldCmdFmpInstDict = {
                        BUILD_EC_CMD: EcArgumentHelper
                        ...
                    }
        """
        OpCodeList    : List[Argument] = list ()
        HelperDict    : dict           = dict ()
        Title         : str            = str ()
        Description   : str            = str ()
        SubDescription: str            = str ()

        #
        # Iterate all the build OpCode-ArgumentList mapping
        #
        for OpCode, ArgsList in ArgumentDict.items ():
            Title          = str ()
            Description    = str ()
            SubDescription = str ()

            if ArgsList is None:
                HelperDict.update ({OpCode: None})
                continue

            CmdInfo = gFmpFirmwareInfo[MappingDict[OpCode]][FW_CMD_SECTION_KEY]
            FwOpVer = gFmpFirmwareInfo[MappingDict[OpCode]][FW_VERSION_KEY]

            if CMD_HELPER_TITLE_KEY in CmdInfo:
                Title = CmdInfo[CMD_HELPER_TITLE_KEY]

            if CMD_HELPER_DESC_KEY in CmdInfo:
                Description = CmdInfo[CMD_HELPER_DESC_KEY]

            if CMD_HELPER_SUB_DESC_KEY in CmdInfo:
                SubDescription = CmdInfo[CMD_HELPER_SUB_DESC_KEY]

            #
            # Create the helper for this OpCode
            #
            HelperDict.update ({
                         OpCode:
                             ArgumentHelper (
                               OpCode         = '.'.join ([sys.argv[0], OpCode]),
                               ArgumentList   = ArgsList,
                               Title          = Title,
                               Description    = Description,
                               SubDescription = SubDescription,
                               Copyright      = STR_PKG_COPYRIGHT,
                               Version        = str (FwOpVer),
                               Style          = DEFAULT_HELPER_TYPE,
                               )
                         })
            #
            # Append this available OpCode.
            #
            OpCodeList.append (
                         Argument (
                           Command     = OpCode,
                           Compulsory  = True,
                           Description = Title,
                           Syntax      = '',
                           Default     = '',
                           )
                         )

        #
        # Force to add the pre-defined help command.
        #
        OpCodeList.append (
                     Argument (
                       Command     = 'help/--help',
                       Compulsory  = False,
                       Description = 'To get the helper for this toolkit.',
                       Syntax      = '',
                       Default     = '',
                       )
                     )

        #
        # Extend the "DEFAULT" OpCode, to expose all the OpCode.
        #
        HelperDict.update ({
                     DEFAULT:
                         ArgumentHelper (
                           OpCode         = '<OPCODE>',
                           Title          = STR_PKG_TITLE,
                           Description    = STR_PKG_DESCRIPTION,
                           ArgumentList   = OpCodeList,
                           SubDescription = STR_PKG_SUB_DESCRIPTION,
                           Copyright      = STR_PKG_COPYRIGHT,
                           Version        = GetPackageVersion (),
                           Style          = DEFAULT_HELPER_TYPE,
                           )
                     })

        return HelperDict
