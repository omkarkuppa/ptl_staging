## @file
#  API to provide the Python-Check functions.
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

from EnvChecker.Const import *
from EnvChecker.PythonCheck import *

def CheckPython3EnvApi ():
    """ API to check caller uses the Python 3.

    Args:
        None.

    Raises:
        None.

    Returns:
        int:
            STATUS_SUCCESS: Caller uses the Python 3.
            STATUS_ERROR  : Caller not uses the Python 3.
    """
    Result    = None
    PyChecker = Python3Checker ()

    print ('================ Start Check the Python 3 ================')

    if PyChecker.Check ():
        Result = STATUS_SUCCESS
        print ('Python 3 installed.')
    else:
        Result = STATUS_ERROR
        print ('Not supported Python version get used.')
        print (
          'Your system is running Python %d.%d please upgrade it.'
          % (PyChecker.Major, PyChecker.Minor)
          )

    print ('================= End Check the Python 3 =================')

    return Result

def CheckPythonVerEnvApi ():
    """ API to check caller uses the Python version.

    Args:
        None.

    Raises:
        None.

    Returns:
        bool:
            True  - Python version meet the requirement.
            False - Python version not meet the requirement.
    """
    Result    = None
    PyChecker = PythonVerChecker ()

    print ('============= Start Check the Python Version =============')

    if PyChecker.Check ():
        Result = STATUS_SUCCESS
        print ('Python Version meet requirement.')
    else:
        Result = STATUS_ERROR

    print ('============== End Check the Python Version ==============')

    return Result

def CheckPythonPathEnvApi ():
    """ API to check caller used path of Python program.

    Args:
        None.

    Raises:
        None.

    Returns:
        bool:
            True  - Python program path is valid.
            False - Python program path is not valid.
    """
    Result    = None
    PyChecker = PythonPathChecker ()

    print ('========== Start Check the Executed Python Path ==========')

    print ('Python path: %s' % (PyChecker.Path))

    if PyChecker.Check ():
        Result = STATUS_SUCCESS
        print ('Executed Python is in valid path.')
    else:
        Result = STATUS_ERROR
        print ('Executed Python is not in invalid path.')

    print ('=========== End Check the Executed Python Path ===========')

    return Result
