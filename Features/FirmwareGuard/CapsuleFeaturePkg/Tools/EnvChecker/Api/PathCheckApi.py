## @file
#  Api to provide the Path-Check functions.
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

from __future__ import print_function

from EnvChecker.PathChecker import *

def CheckToolkitPathEnvApi ():
    """ Check the toolkit placed in valid path.

    Args:
        None.

    Raises:
        None.

    Returns:
        int:
            STATUS_SUCCESS: Caller place the toolkit in valid path.
            STATUS_ERROR  : Caller not place the toolkit in valid path.
    """
    Result      = None
    PathChecker = ToolkitPathChecker ()

    print ('=============== Start Check the toolkit location ===============')

    print ('Toolkit path: %s' % (PathChecker.Path))

    if PathChecker.Check ():
        Result = STATUS_SUCCESS
        print ('Toolkit is placed in valid path.')
    else:
        Result = STATUS_ERROR
        print ('Toolkit is not placed in valid path.')

    print ('================ End Check the toolkit location ================')

    return Result
