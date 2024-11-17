## @file
#  This library is used to check caller executed Python path is valid.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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

class PythonPathChecker (object):
    def __init__ (self):
        self.__Path = sys.executable

    @property
    def Path (self) -> str:
        return self.__Path

    def Check (self):
        """ Execute the check operation.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - The executed Python path is valid.
                False - The executed Python path is not valid.
        """
        IsValid = None

        if ' ' in self.__Path:
            print ("[ERROR] !!! Executed Python path with space is not supported !!!")
            IsValid = False
        else:
            IsValid = True

        return IsValid
