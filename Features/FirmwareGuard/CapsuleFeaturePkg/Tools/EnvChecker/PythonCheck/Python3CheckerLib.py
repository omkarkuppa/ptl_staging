## @file
#  This library is used to check caller's environment is Python 3.
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

import sys

from EnvChecker.Const import *

class Python3Checker (object):
    def __init__ (self):
        """ Class to check caller environment is Python 3.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Major = sys.version_info.major
        self.__Minor = sys.version_info.minor

    @property
    def Major (self):
        """ Caller's Python major version.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Python major version.
        """
        return self.__Major

    @property
    def Minor (self):
        """ Caller's Python minor version.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Python minor version.
        """
        return self.__Minor

    def Check (self):
        """ Execute the check operation.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - Caller's Python major version is 3.
                False - Caller's Python major version is not 3.
        """
        if self.__Major == PYTHON_MAJOR_VERSION_3:
            return True
        else:
            return False
