## @file
#  This library is used to check the toolkit path.
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

from EnvChecker.Const import *

#
# Note: If folder/file level get changed, need to change here.
#
FILE_PATH         = os.path.abspath (__file__)
PATH_CHECKER_PATH = os.path.dirname (FILE_PATH)
ENV_CHECKER_PATH  = os.path.dirname (PATH_CHECKER_PATH)
TOOLKIT_PATH      = os.path.dirname (ENV_CHECKER_PATH)

class ToolkitPathChecker (object):
    def __init__ (self):
        """ Class support to check the toolkit path is valid.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        pass

    @property
    def Path (self):
        """ Get the toolkit located path.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The path of toolkit.
        """
        return TOOLKIT_PATH

    def Check (self):
        """ Execute the check operation.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - The toolkit located path is valid.
                False - The toolkit located path is not valid.
        """
        IsValid = None

        if ' ' in TOOLKIT_PATH:
            print ("[ERROR] !!! Toolkit path with space is not supported !!!")
            IsValid = False
        else:
            IsValid = True

        return IsValid
