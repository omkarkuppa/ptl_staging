## @file
#  Library provided the EDK-II tool path.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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

from CapsuleCommon.Base.SystemLib import *
from CapsuleCommon.Edk2.ConstantLib import *

def GetEdk2ToolBinPath () -> str:
    """ Return the path of the executable binary files directory.

    Note:
        Path would be varied between different base of system.

    Args:
        None.

    Raises:
        None.

    Returns:
        str:
            Path to the executable binary files directory.
    """
    BinPath: str = None

    if IsWindowsPlatform ():
        BinPath = EDKII_BASE_TOOL_WIN_BIN_PATH
    else:
        BinPath = EDKII_BASE_TOOL_POSIX_BIN_PATH

    return BinPath

def GetEdk2ToolBinWrapperPath () -> str:
    """ Return the path of the binary wrapper directory.

    Note:
        Path would be varied between different base of system.

    Args:
        None.

    Raises:
        None.

    Returns:
        str:
            Path to the binary wrapper directory.
    """
    WrapperPath: str = None

    if IsWindowsPlatform ():
        WrapperPath = EDKII_BASE_TOOL_WIN_BIN_WRAPPER_PATH
    else:
        WrapperPath = EDKII_BASE_TOOL_POSIX_BIN_WRAPPER_PATH

    return WrapperPath

def GetEdk2ToolBinWrapperExt () -> str:
    """ Return the extension of the binary tool wrapper script.

    Args:
        None.

    Raises:
        None.

    Returns:
        str:
            Extension of the binary tool wrapper script.
    """
    ExtStr: str = str ()

    if IsWindowsPlatform ():
        ExtStr = EDKII_BASE_TOOL_WIN_BIN_WRAPPER_EXT

    return ExtStr

def GetGenFdsToolPath () -> str:
    """ Return the path of the GenFds tool.

    Args:
        None.

    Raises:
        None.

    Returns:
        str:
            Path to GenFds tool.
    """
    return JoinPath (
             GetEdk2ToolBinWrapperPath (),
             f'{EDKII_BUILD_FD_CMD}{GetEdk2ToolBinWrapperExt ()}',
             )

def GetGenCapsuleToolPath () -> str:
    """ Return the path of the GenerateCapsule tool.

    Args:
        None.

    Raises:
        None.

    Returns:
        str:
            Path to GenerateCapsule script.
    """
    return JoinPath (
             GetEdk2ToolBinWrapperPath (),
             f'{EDKII_BUILD_CAPSULE_CMD}{GetEdk2ToolBinWrapperExt ()}',
             )
