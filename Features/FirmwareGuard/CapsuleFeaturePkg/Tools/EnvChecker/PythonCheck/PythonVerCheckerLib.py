## @file
#  This library is used to check caller's system Python environment
#  meet the toolkit requirement.
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

from EnvChecker.Config import *

class PythonVerChecker (object):
    def __init__ (self):
        """ Class to check caller Python version meet the requirement.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Major        = sys.version_info.major
        self.__Minor        = sys.version_info.minor
        self.__Micro        = sys.version_info.micro
        self.__ReleaseLevel = sys.version_info.releaselevel
        self.__Serial       = sys.version_info.serial

    def Check (self):
        """ Execute the check operation.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - Caller's Python version meet the criteria.
                False - Caller's Python version not meet the criteria.
        """
        IsValid = True

        print (
          'Current Python Version: %d.%d.%d'
          % (self.__Major, self.__Minor, self.__Micro)
          )

        if self.__Major < REQUIRED_PY_MAJOR_VERSION:
            IsValid = False
        elif self.__Minor < REQUIRED_PY_MINOR_VERSION:
            IsValid = False
        else:
            IsValid = True

        if not IsValid:
            print (
              '[ERROR] Python version is not meet %d.%d+'
              % (REQUIRED_PY_MAJOR_VERSION, REQUIRED_PY_MINOR_VERSION)
              )

        return IsValid
