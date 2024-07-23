## @file
#  Definition of platform type value.
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

#
# List of using os.name
#
OS_NAME_NT   : str = 'nt'
OS_NAME_POSIX: str = 'posix'

#
# List of using sys.platform
#
SYS_PLATFORM_WINDOWS: str = 'win32'
SYS_PLATFORM_CYGWIN : str = 'cygwin'
SYS_PLATFORM_LINUX  : str = 'linux'
SYS_PLATFORM_MACOS  : str = 'darwin'

#
# List of using platform.system ()
#
PLATFORM_WINDOWS: str = 'Windows'
PLATFORM_LINUX  : str = 'Linux'
PLATFORM_MACOS  : str = 'Darwin'

WINDOWS_TYPE_PLATFORM_LIST: List[str] = [
    OS_NAME_NT,
    SYS_PLATFORM_WINDOWS,
    SYS_PLATFORM_CYGWIN,
    PLATFORM_WINDOWS,
    ]

LINUX_TYPE_PLATFORM: List[str] = [
    SYS_PLATFORM_LINUX,
    PLATFORM_LINUX,
    ]

MACOS_TYPE_PLATFORM: List[str] = [
    SYS_PLATFORM_MACOS,
    PLATFORM_MACOS,
    ]
