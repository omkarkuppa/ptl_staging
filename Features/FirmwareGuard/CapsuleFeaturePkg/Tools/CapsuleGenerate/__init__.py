## @file
#  Python module package initial files for CapsuleGenerate package.
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

from CapsuleGenerate.Api import *

from CapsuleGenerate._Version import *
from CapsuleGenerate.PkgInfo import *

__copyright__  : str = STR_PKG_COPYRIGHT
__version__    : str = GetPackageVersion ()
__description__: str = STR_PKG_SUB_DESCRIPTION

__doc__: str = '''
CapsuleGenerate - Generate Client FMP Signed Capsule Package
=================================================================

Prerequisite
------------
- Configuration the environment variable before import the package.
    - COMMON_CONFIG_FILE  : Set the path of common config file.
    - PLATFORM_CONFIG_FILE: Set the path of platform config file.
'''
