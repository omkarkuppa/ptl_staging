## @file
#  Api to get the current working platform information.
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

from CapsuleGenerate._Global import *

def ApiGetPlatformName (IsUppercase: bool = False) -> str:
    """ API to get the current working platform name.

    Args:
        IsUppercase (bool, optional):
            True  - Get the platform name with uppercase force.
            False - Get the platform name with assigned string.
            Defaults to False.

    Raises:
        None.

    Returns:
        str:
            Current working platform name.
    """
    if IsUppercase:
        return PLATFORM_FULL_NAME_UPPER
    else:
        return PLATFORM_FULL_NAME

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'ApiGetPlatformName',
    ]
