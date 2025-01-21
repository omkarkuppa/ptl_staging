## @file
#  Entry point to initial the toolkit needed environment and decide what
#  firmware need to be built.
#
#  Note:
#    In this file should process and decide which platform configuration
#    file should be loaded.
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

from CapsuleCommon import *

#
# Pre-defined environment variable
#
ENV_DEBUG_LEVEL: str = 'DEBUG_LEVEL'

#
# Pre-config CapsuleCommon Package.
#
SetDebugLevel (os.environ[ENV_DEBUG_LEVEL])

from CapsuleGenerate import *

DEFAULT_KEY: str = 'DEFAULT'
IGNORE_KEY : str = 'IGNORE'

#
# Pre-defined OpCode
#
OPCODE_CLEAN_WORKSPACE: str = 'cln'
OPCODE_BUILD_ALL_CAP  : str = 'allcap'
OPCODE_HELP_POS_ARGV  : str = 'help'
OPCODE_HELP_FLAG_ARGV : str = '--help'

#
# Pre-defined keyword command
#
KEYWORD_CMD_CSE_IMAGE : str = '-cse'

#
# Pre-defined position command
#
POSITION_CMD_CSE_REGION: str = 'cseregion'

#
# Pre-defined global variable
#
gArgv        : ArgumentParser = ArgumentParser ()
gPositionArgv: dict           = gArgv.PositionArgv
gKeywordArgv : dict           = gArgv.KeywordArgv
gFlagArgv    : dict           = gArgv.FlagArgv

#
# Requested global variable
# Need to fill by caller self or call this script as main function.
#
COMMAND_DICT    : dict = dict ()
MAPPING_DICT    : dict = dict ()
BLD_CMD_FMP_DICT: dict = dict ()
ARGS_HELPER_DICT: dict = dict ()

def PrintHelper () -> bool:
    """ Print the correspond argument helper for build command.

    Args:
        None.

    Raises:
        None.

    Returns:
        bool:
            True  - Found the build command helper and printed.
            False - Not found the build command helper and not printed.
    """
    IsPrint     : bool = False
    PlatformName: str  = ApiGetPlatformName ()

    for Argv in gPositionArgv:
        if Argv not in ARGS_HELPER_DICT:
            continue
        elif ARGS_HELPER_DICT[Argv] is None:
            DEBUG (
              DEBUG_WARN,
              f'Build cmd \"{Argv}\" not enabled '
              f'in current {PlatformName} platform. '
              f'No helper get display.'
              )
            continue

        ARGS_HELPER_DICT[Argv].Print ()
        IsPrint = True

        sys.exit (EXIT_SUCCESS)

    return IsPrint

def IsFwBuildRequired (DeviceName: str) -> bool:
    """ Check if the firmware build is required.

    Args:
        DeviceName (str):
            The FMP device instance name.
            (Should be defined within the FmpInstance object itself)

    Raises:
        TypeError:
            DeviceName is not str type.

    Returns:
        bool:
            True  - Input firmware instance need to build.
            False - Input firmware instance no need to build.
    """
    if not isinstance (DeviceName, str):
        raise TypeError ('Device name should be str type.')

    BuildCmd  : str  = MAPPING_DICT[DeviceName]
    IsRequired: bool = BuildCmd in gPositionArgv

    return IsRequired

def IsFwBuildEnabled (DeviceName: str) -> bool:
    """ Check if the firmware build is enabled.

    Args:
        DeviceName (str):
            The FMP device instance name.
            (Should be defined within the FmpInstance object itself)

    Raises:
        TypeError:
            DeviceName is not str type.

    Returns:
        bool:
            True  - Input firmware instance is enabled.
            False - Input firmware instance is disabled.
    """
    if not isinstance (DeviceName, str):
        raise TypeError ('Device name should be str type.')

    BuildCmd: str = MAPPING_DICT[DeviceName]

    if COMMAND_DICT[BuildCmd] is None:
        return False

    return True

def IsIfwiDecomposeRequired () -> bool:
    """ Check if need to decompose the IFWI via FIT tool.

    Args:
        None.

    Raises:
        None.

    Returns:
        bool:
            True  - Require to decompose the IFWI via FIT tool.
            False - Not need to decompose the IFWI via FIT tool.
    """
    if (KEYWORD_CMD_IFWI_IMG in gKeywordArgv) and \
       (KEYWORD_CMD_FIT_TOOL in gKeywordArgv):
        return True

    if (KEYWORD_CMD_FIT_TOOL in gKeywordArgv):
        DEBUG (
          DEBUG_WARN,
          f'Assigned the \"{KEYWORD_CMD_FIT_TOOL}\" '
          f'but not assigned the \"{KEYWORD_CMD_IFWI_IMG}\". '
          )
        DEBUG (DEBUG_WARN, f'IFWI would not decompose.')

    return False

def IsFwuGenRequired () -> bool:
    """ Check if the FWUpdate payload need to generate.

    Args:
        None.

    Raises:
        None.

    Returns:
        bool:
            True  - Need to generate the FWUpdate payload image.
            False - No need to generate the FWUpdate payload image.
    """
    IsRequired: bool = False

    if KEYWORD_CMD_ME_IMG in gKeywordArgv:
        DEBUG (
          DEBUG_INFO,
          f'Assigned the \"{KEYWORD_CMD_ME_IMG}\".'
          'Build CSME component with user input.'
          )
    elif POSITION_CMD_CSE_REGION in gPositionArgv:
        DEBUG (
          DEBUG_INFO,
          f'Assigned the \"{POSITION_CMD_CSE_REGION}\".'
          'Build for CSME full region.'
          )
    elif not ( \
        IsFwBuildRequired (ME_DEVICE_NAME) or \
        IsFwBuildRequired (MONOLITHIC_DEVICE_NAME) or \
        OPCODE_BUILD_ALL_CAP in gPositionArgv
        ):
        DEBUG (
          DEBUG_INFO,
          'Capsule build is not include CSME bypass the FWU generate.'
          )
    else:
        DEBUG (DEBUG_INFO, f'CSME FWUpdate generate required.')
        IsRequired = True

    return IsRequired

def GetAllCapBuildFwList () -> list:
    """ Return list of FMP device name for building "allcap" command.

    Args:
        None

    Raises:
        None.

    Returns:
        list:
            List of FMP device name.
    """
    FwBuildList: List[str] = list ()
    DeviceName : str       = None
    DeviceInfo : dict      = None
    IsEnabled  : bool      = None
    IsIgnored  : bool      = None

    for DeviceName, DeviceInfo in BLD_ALL_CAP_FMP_DEVICE_NAME_DICT.items ():
        IsEnabled = IsFwBuildEnabled (DeviceName)
        if not IsEnabled:
            continue

        IsIgnored = (DeviceInfo[KEY_IS_IMAGE_CHECK] is True) and \
                    (DeviceInfo[KEY_IMAGE_PATH] is not None) and \
                    (CheckStringMatch (DeviceInfo[KEY_IMAGE_PATH], IGNORE_KEY))
        if IsIgnored:
            continue

        FwBuildList.append (DeviceName)

    return FwBuildList

def GetFwBuildList () -> list:
    """ Get the list of firmware FMP device name to be built.

    Note:
        (1) This pre-function is to fetch the user input command from
            terminal to check firmware build need to trigger.
        (2) To prevent duplicate build, function call would need to
            remove duplicate element within list.

    Args:
        None.

    Raises:
        None.

    Returns:
        list:
            List of FMP device name to be executed to build the capsule.
            (Each element should be mapping to FW_COMPONENT_DICT)
    """
    FmpDeviceNameList: list = list ()

    #
    # If allcap get used, add all related instance to build.
    # Standalone, Monolithic and All Capsule build check.
    #
    if OPCODE_BUILD_ALL_CAP in gPositionArgv:
        FmpDeviceNameList.extend (GetAllCapBuildFwList ())

    #
    # Specific firmware component build OP code check.
    #
    for FmpDeviceName in FW_COMPONENT_DICT:
        if IsFwBuildRequired (FmpDeviceName):
            FmpDeviceNameList.append (FmpDeviceName)

    #
    # Use set to remove the duplicate one.
    #
    FmpDeviceNameList = list (set (FmpDeviceNameList))

    return FmpDeviceNameList

def CapsuleNotBuild () -> None:
    """ Function call to process the capsule build is not built case.

    Note:
        (1) Support to output the error message.
        (2) Dump the default helper to give the usage hint to user.
        (3) Dump the user environment for debug purpose.

    Args:
        None.

    Raises:
        None.

    Returns:
        None.
    """
    ExitStatus: int = EXIT_CAPSULE_NOT_BUILD

    DEBUG (
      DEBUG_ERROR,
      'Capsule build is not completed.'
      'Please check the command in-use is correctly.'
      )

    #
    # Dump the default helper.
    #
    ARGS_HELPER_DICT[DEFAULT_KEY].Print ()

    #
    # Dump the user environment.
    #
    UserEnvironment ().Dump ()

    sys.exit (ExitStatus)

def StandaloneCapBuild (
    BuildCmd   : str,
    FwName     : str,
    FmpInstance: FmpDevice,
    ) -> None:
    """ Build operation to build the standalone capsule.

    Args:
        BuildCmd (str):
            The build command in used of this standalone capsule.
        FwName (str):
            The firmware name of this FMP device.
        FmpInstance (FmpDevice):
            The FmpDevice instance for this firmware.

    Raises:
        None.

    Returns:
        None.
    """
    PlatformName: str = ApiGetPlatformName ()

    DEBUG (DEBUG_INFO, f'>> Start to build {FwName} standalone capsule.')

    if not IsFwBuildEnabled (FwName):
        DEBUG (
          DEBUG_ERROR,
          f'{FwName} capsule build is not supported on {PlatformName}.'
          )
        CapsuleNotBuild ()

    ArgsChecker: ArgumentChecker = \
        ArgumentChecker (COMMAND_DICT[BuildCmd], gArgv)

    Params  : dict[str, Any] = ArgsChecker.Params
    FmpModel: FmpDevice      = FmpInstance (Params)
    CapBuild: ApiFmpCapBuild = ApiFmpCapBuild (FmpModel)

    #
    # Build the capsule file.
    #
    CapBuild.BuildCapsule ()

    #
    # Condition to build the Windows driver update package.
    #
    if IsWindowsPlatform ():
        DEBUG (
          DEBUG_INFO,
          'Windows platform attempt to build Windows Update Package.'
          )
        CapBuild.BuildWinUpdatePkg ()

    DEBUG (DEBUG_INFO, f'>> Finish to build {FwName} standalone capsule.')

def MonolithicCapBuild (
    BuildCmd   : str,
    FwName     : str,
    FmpInstance: FmpDevice,
    ) -> None:
    """ Build operation to build the monolithic capsule.

    Note:
        This build operation have dependency on BIOS FMP instance.

    Args:
        BuildCmd (str):
            The build command in used of this standalone capsule.
        FwName (str):
            The firmware name of this FMP device.
        FmpInstance (FmpDevice):
            The FmpDevice instance for this firmware.

    Raises:
        None.

    Returns:
        None.
    """
    PlatformName: str = ApiGetPlatformName ()

    DEBUG (DEBUG_INFO, f'>> Start to build {FwName} monolithic capsule.')

    if not IsFwBuildEnabled (FwName):
        DEBUG (
          DEBUG_ERROR,
          f'{FwName} capsule build is not supported on {PlatformName}.'
          )
        CapsuleNotBuild ()

    ArgsChecker: ArgumentChecker = \
        ArgumentChecker (COMMAND_DICT[BuildCmd], gArgv)

    Params      : dict[str, Any] = ArgsChecker.Params
    BiosFmpModel: FmpDevice      = Bios (Params)
    FmpModel    : FmpDevice      = FmpInstance (Params, BiosFmpModel)
    CapBuild    : ApiFmpCapBuild = ApiFmpCapBuild (FmpModel)

    #
    # Build the capsule file.
    #
    CapBuild.BuildCapsule ()

    #
    # Condition to build the Windows driver update package.
    #
    if IsWindowsPlatform ():
        DEBUG (
          DEBUG_INFO,
          'Windows platform attempt to build Windows Update Package.'
          )
        CapBuild.BuildWinUpdatePkg ()

    DEBUG (DEBUG_INFO, f'>> Finish to build {FwName} monolithic capsule.')

FW_COMPONENT_DICT: dict = {
    #
    # Standalone Capsule.
    #
    BIOS_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: StandaloneCapBuild,
        FW_FMP_OBJ_KEY : Bios,
        },
    EXT_BIOS_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: StandaloneCapBuild,
        FW_FMP_OBJ_KEY : ExtendedBios,
        },
    EC_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: StandaloneCapBuild,
        FW_FMP_OBJ_KEY : Ec,
        },
    ISH_PDT_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: StandaloneCapBuild,
        FW_FMP_OBJ_KEY : IshPdt,
        },
    ME_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: StandaloneCapBuild,
        FW_FMP_OBJ_KEY : Me,
        },
    #
    # Monolithic Capsule.
    #
    MONOLITHIC_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: MonolithicCapBuild,
        FW_FMP_OBJ_KEY : Monolithic,
        },
    #
    # IFWI Capsule.
    #
    IFWI_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: StandaloneCapBuild,
        FW_FMP_OBJ_KEY : Ifwi,
        },
    #
    # Modular Capsule.
    #
    BTG_ACM_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: StandaloneCapBuild,
        FW_FMP_OBJ_KEY : BtGAcm,
        },
    FSP_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: StandaloneCapBuild,
        FW_FMP_OBJ_KEY : Fsp,
        },
    UCODE_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: StandaloneCapBuild,
        FW_FMP_OBJ_KEY : Ucode,
        },
    #
    # Device Firmware Capsule
    #
    FOXVILLE_DEVICE_NAME: {
        BLD_OP_FLOW_KEY: StandaloneCapBuild,
        FW_FMP_OBJ_KEY : Foxville,
        },
    }

#
# Build command with 'allcap'.
#
KEY_IS_IMAGE_CHECK: str = 'IsImagePathCheck'
KEY_IMAGE_PATH    : str = 'ImagePath'

BLD_ALL_CAP_FMP_DEVICE_NAME_DICT: dict = {
    BIOS_DEVICE_NAME      : {
        KEY_IS_IMAGE_CHECK: True,
        KEY_IMAGE_PATH    : gKeywordArgv.get (KEYWORD_CMD_BIOS_IMG, None),
        },
    EC_DEVICE_NAME        : {
        KEY_IS_IMAGE_CHECK: True,
        KEY_IMAGE_PATH    : gKeywordArgv.get (KEYWORD_CMD_EC_IMG, None),
        },
    ME_DEVICE_NAME        : {
        KEY_IS_IMAGE_CHECK: True,
        KEY_IMAGE_PATH    : gKeywordArgv.get (KEYWORD_CMD_ME_IMG, None),
        },
    ISH_PDT_DEVICE_NAME   : {
        KEY_IS_IMAGE_CHECK: True,
        KEY_IMAGE_PATH    : gKeywordArgv.get (KEYWORD_CMD_ISH_PDT_IMG, None),
        },
    MONOLITHIC_DEVICE_NAME: {
        KEY_IS_IMAGE_CHECK: False,
        KEY_IMAGE_PATH    : None,
        }
    }

########################################################################
#                       FmpCapsuleGen Main Script                      #
########################################################################
#
# Exit code of this script.
#
EXIT_SUCCESS          : int = 0x00
EXIT_CAPSULE_NOT_BUILD: int = 0x01

if __name__ == '__main__':
    ExitStatus    : int          = EXIT_SUCCESS
    CseImagePath  : str          = str ()
    IsHelperDump  : bool         = False
    IsCapsuleBuild: bool         = False
    FwBuildList   : list         = list ()
    BuildCmd      : str          = str ()
    BuildOp       : FunctionType = None
    FmpInstance   : FmpDevice    = None

    #
    # Check if input to clean up the workspace.
    #
    if OPCODE_CLEAN_WORKSPACE in gPositionArgv:
        ApiCleanUpWorkspace ()
        sys.exit (ExitStatus)

    #
    # Initialize the capsule generate environment.
    #
    ApiInitialBuildEnv ()

    #
    # Initialize the build command/argument/helper/FmpDevice instance
    #
    COMMAND_DICT, MAPPING_DICT, BLD_CMD_FMP_DICT, ARGS_HELPER_DICT = \
        ApiInitialArgumentEnv (FW_COMPONENT_DICT)

    #
    # Helper Dump
    #
    if (OPCODE_HELP_POS_ARGV in gPositionArgv) or \
       (OPCODE_HELP_FLAG_ARGV in gFlagArgv):
        DEBUG (DEBUG_TRACE, f'Helper command get called.')
        IsHelperDump = PrintHelper ()

        if not IsHelperDump:
            ARGS_HELPER_DICT[DEFAULT_KEY].Print ()

    #
    # Check if FW component build assigned.
    #
    FwBuildList = GetFwBuildList ()
    if len (FwBuildList) == 0:
        DEBUG (DEBUG_ERROR, 'Non Capsule Build get assigned.')
        CapsuleNotBuild ()

    #
    # (1) Decompose the firmware from IFWI via FIT tool.
    #     (Generate Capsule Payload)
    # (2) Build the firmware component with build operation and its
    #     build flow.
    #
    if (IsIfwiDecomposeRequired ()):
        if KEYWORD_CMD_CSE_IMAGE in gKeywordArgv:
            CseImagePath = gKeywordArgv[KEYWORD_CMD_CSE_IMAGE]

        ApiGenCapsulePayload (
          IfwiPath      = gKeywordArgv[KEYWORD_CMD_IFWI_IMG],
          FitToolPath   = gKeywordArgv[KEYWORD_CMD_FIT_TOOL],
          IsFwuRequired = IsFwuGenRequired (),
          CseImagePath  = CseImagePath,
          )

    for FmpDeviceName in FwBuildList:
        BuildCmd    = MAPPING_DICT[FmpDeviceName]
        BuildOp     = FW_COMPONENT_DICT[FmpDeviceName][BLD_OP_FLOW_KEY]
        FmpInstance = FW_COMPONENT_DICT[FmpDeviceName][FW_FMP_OBJ_KEY]
        BuildOp (BuildCmd, FmpDeviceName, FmpInstance)

        #
        # Set the capsule build as True, mark build is succeed.
        #
        IsCapsuleBuild = True

    #
    # If capsule build is not completed, dump the help.
    #
    if not IsCapsuleBuild:
        CapsuleNotBuild ()

    sys.exit (ExitStatus)
