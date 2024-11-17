## @file
#  Main script to execute the environment check work.
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

import argparse
import os

from EnvChecker import *

__prog__        = os.path.basename (__file__)
__version__     = '%s Version %s' % (__prog__, '0.1')
__copyright__   = 'Copyright (C) 2023 Intel Corporation. All rights reserved.\n'
__description__ = 'Script to check the environment.\n'

ENV_PRE_CHECK_FUNC_LIST = [
    CheckPython3EnvApi,
    CheckPythonVerEnvApi,
    CheckPythonPathEnvApi,
    CheckToolkitPathEnvApi,
    ]

ENV_POST_CHECK_FUNC_LIST = [
    CheckPython3EnvApi,
    ]

def EnvCheck (CheckList):
    """ Function to execute list of checker.

    Note:
        If checker execute not return STATUS_SUCCESS, program would
        get exit directly.

    Args:
        CheckList (List[Function]):
            List of function to execute the check.
            Return value should be STATUS_*.

    Raises:
        None.

    Returns:
        None.
    """
    print ('> [Start] Execute the environment check')

    for Idx in range (len (CheckList)):
        print ('\n[Check#%d]' % (Idx + 1))

        StatusCode = CheckList[Idx] ()

        if StatusCode != STATUS_SUCCESS:
            sys.exit (StatusCode)

    print ('\n> [End] Execute the environment check')

if __name__ == '__main__':
    def GetArgs () -> argparse.Namespace:
        """ Get the user input arguments.

        Args:
            None.

        Raises:
            None.

        Returns:
            Args [argparse.Namespace]:
                The argument object parsing from user input.
        """
        Parser: argparse.ArgumentParser = argparse.\
            ArgumentParser (
              prog            = __prog__,
              description     = __copyright__ + __description__ + __version__,
              formatter_class = argparse.RawTextHelpFormatter,
              )
        Parser.add_argument (
                 '-V',
                 '-v',
                 '--Version',
                 action  = 'version',
                 version = __version__,
                 )
        Parser.add_argument (
                 '--pre-check',
                 help   = 'Execute environment Pre-Check work.',
                 dest   = 'PreCheck',
                 action = 'store_true',
                 )
        Parser.add_argument (
                 '--post-check',
                 help   = 'Execute environment Post-Check work.',
                 dest   = 'PostCheck',
                 action = 'store_true',
                 )

        return Parser.parse_args ()

    Args       : argparse.Namespace = GetArgs ()
    IsPreCheck : bool               = Args.PreCheck
    IsPostCheck: bool               = Args.PostCheck

    if IsPreCheck:
        print ('> Request to execute environment Pre-Check')
        EnvCheck (ENV_PRE_CHECK_FUNC_LIST)

    if IsPostCheck:
        print ('> Request to execute environment Post-Check')
        EnvCheck (ENV_POST_CHECK_FUNC_LIST)

    sys.exit (STATUS_SUCCESS)
