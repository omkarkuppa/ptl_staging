## @file
#  Api for clean up the workspace.
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

from typing import List

from CapsuleCommon import *

from CapsuleGenerate._Global import *

TARGET_DIR_NAME_LIST: List[str] = [
    TEMPORARY_DIR_NAME,
    OUTPUT_DIR_NAME,
    ]

TARGET_FILE_EXTENSION_LIST: List[str] = [
    ]

#
# Python script used for this API.
#
PY_CLEANUP_SCRIPT_PATH: str = \
    JoinPath (
      WORKSPACE_PATH,
      gPYScriptPathInfo[CLEAN_UP_SCRIPT_KEY],
      )

def ApiCleanUpWorkspace () -> int:
    """ API to clean up the workspace.

    Args:
        None.

    Raises:
        None.

    Returns:
        int:
            The exit code of workspace clean up script.
            (Detail exit code should be reference its own)
    """
    CmdList : List[str] = None
    ExitCode: int       = None

    DEBUG (DEBUG_INFO, '>>> Clean up the workspace start.')

    CmdList: List[str] = [
        PY_CLEANUP_SCRIPT_PATH,
        '-r', f'{WORKSPACE_PATH}',
        ]

    #
    # Add the target directory.
    #
    for DirName in TARGET_DIR_NAME_LIST:
        CmdList.append ('-d')
        CmdList.append (DirName)

    #
    # Add the target extension.
    #
    for Extension in TARGET_FILE_EXTENSION_LIST:
        CmdList.append ('-e')
        CmdList.append (Extension)

    ExitCode, _, _ = ExecPythonCmd (CmdList)

    DEBUG (DEBUG_INFO, '>>> Clean up the workspace end.')

    return ExitCode

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'ApiCleanUpWorkspace',
    ]
