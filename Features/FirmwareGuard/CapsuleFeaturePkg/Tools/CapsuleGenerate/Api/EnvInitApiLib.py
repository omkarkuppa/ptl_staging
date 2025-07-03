## @file
#  Api for initial the CapsuleGenerate build environment.
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

from CapsuleGenerate.PreProcess import *

def ApiInitialBuildEnv () -> None:
    """ API to initial the workspace environment.

    Note:
        (1) Remove the intermediate directories and files.
        (2) Touch the workspace needed directory.
        (3) Port the EDK-II Conf directory.
        (4) Create the EDK-II DSC file.
        (5) Create the dummy firmware files.

    Args:
        None.

    Raises:
        None.

    Returns:
        None.
    """
    InitializeEnv.RemoveIntermediateDirFile ()
    InitializeEnv.TouchNeededDir ()
    InitializeEnv.Edk2WorkspaceInit ()
    InitializeEnv.CreateEdk2DscFile ()
    InitializeEnv.CreateDummyFwFiles ()

def ApiInitialArgumentEnv (
        FwComponentDict: dict,
        ) -> Tuple[dict, dict ,dict ,dict]:
    """ API to initial the argument services.

    Args:
        FwComponentDict (dict):
            Firmware and FmpDevice mapping dictionary.
            e.g. Below is the sample format.
            {
                BIOS_DEVICE_NAME: {
                    BLD_OP_FLOW_KEY: StandaloneCapBuild,
                    FW_FMP_OBJ_KEY : Bios,
                },
                ...
            }

    Raises:
        None.

    Returns:
        Tuple[dict, dict ,dict ,dict]:
            dict:
                BuildCmd and list of Argument mapping.
            dict:
                FmpInstanceName and BuildCmd mapping.
            dict:
                BuildCmd and FmpInstance mapping.
            dict:
                Argument helper dictionary.
    """
    ArgumentDict      : dict = dict ()
    MappingDict       : dict = dict ()
    BldCmdFmpInstDict : dict = dict ()
    ArgumentHelperDict: dict = dict ()

    ArgumentDict, MappingDict, BldCmdFmpInstDict = \
        InitialArgument.CreateBldCmdFmpMapping (FwComponentDict)

    ArgumentHelperDict = InitialArgument.\
        CreateArgumentHelper (ArgumentDict, MappingDict)

    return ArgumentDict, MappingDict, BldCmdFmpInstDict, ArgumentHelperDict

def ApiGenCapsulePayload (
        IfwiPath     : str,
        FitToolPath  : str,
        IsFwuRequired: bool,
        CseImagePath : str,
        ) -> None:
    """ API to create the capsule payload files (firmware components).

    Args:
        IfwiPath (str):
            The path of IFWI image to be decomposed.
        FitToolPath (str):
            The FIT tool to support to do the decomposing.
        IsFwuRequired (bool):
            True  - Need to generate the CSME FWUpdate image.
            False - No need to generate the CSME FWUpdate image.
        CseImagePath (str):
            Path of CSE image.

    Raises:
        None.

    Returns:
        None.
    """
    Generator: GenCapsulePayload = \
        GenCapsulePayload (
          IfwiPath      = IfwiPath,
          FitToolPath   = FitToolPath,
          IsFwuRequired = IsFwuRequired,
          CseImagePath  = CseImagePath,
          )
    Generator.CopyFwComponent ()

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'BLD_OP_FLOW_KEY',
    'FW_FMP_OBJ_KEY',
    'ApiInitialArgumentEnv',
    'ApiInitialBuildEnv',
    'ApiGenCapsulePayload',
    ]
