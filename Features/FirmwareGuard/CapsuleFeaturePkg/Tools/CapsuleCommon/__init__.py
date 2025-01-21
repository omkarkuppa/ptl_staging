## @file
#  Python module package initial files for common variables and objects.
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

#
# Constant is fixed value. It should be placed in first place.
# Each file should be independent.
#
from CapsuleCommon.Constant import *
#
# Base sub-package should be placed in second place.
# It should be independent and could not have any dependency.
#
from CapsuleCommon.Base import *

from CapsuleCommon.Format import *
from CapsuleCommon.Validate import *

from CapsuleCommon.Argument import *
from CapsuleCommon.SuperClasses import *
from CapsuleCommon.Wrapper import *

#
# Outside of toolkit owned components.
#
from CapsuleCommon.Edk2 import *
from CapsuleCommon.Intel import *
from CapsuleCommon.OpenSSL import *
from CapsuleCommon.Windows import *


from CapsuleCommon._Version import *

__copyright__  : str = 'Copyright (C) 2023 Intel Corporation. All rights reserved.\n'
__version__    : str = GetPackageVersion ()
__description__: str = 'Capsule toolkit common utilities package.\n'

__doc__: str = '''
CapsuleCommon - a common utilities for capsule toolkit for Python
=================================================================
'''
