## @file
#  Script to do the configuration check on IFWI XML configuration file.
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
import xml.etree.ElementTree as Et

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (C) 2023 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to check IFWI XML configuration file setting.\n'

#
# Definition of program exit code.
#
EXIT_SUCCESS            : int = 0x00
EXIT_VALUE_MISMATCHED   : int = 0x01
EXIT_NODE_NOT_FOUND     : int = 0x02
EXIT_ATTRIBUTE_NOT_FOUND: int = 0x03
EXIT_UNKNOWN            : int = 0xFF

class IfwiXmlConfigParser (object):
    def __init__ (
        self,
        ConfigPath : Union[str, os.PathLike],
        XmlNodeList: List[str],
        Attribute  : str,
        ExpectValue: str,
        ) -> None:
        """ Class to parse the IFWI XML config file with expected value.

        Args:
            ConfigPath (Union[str, os.PathLike]):
                The path of IFWI XML configuration file.
            XmlNodeList (List[str]):
                List of XML node string.
                (Should be top to bottom)
            Attribute (str):
                The attribute of this XML node.
            ExpectValue (str):
                The expect value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__ConfigPath : str        = AbsPath (ConfigPath)
        self.__XmlNodeList: List[str]  = XmlNodeList
        self.__Attribute  : str        = Attribute
        self.__ExpectValue: str        = ExpectValue
        self.__XmlObject  : XmlWrapper = XmlWrapper (self.__ConfigPath)

    def Run (self)  -> int:
        """ To check the attribute value is matched expect value.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                EXIT_SUCCESS            : Attribute value is matched
                                          to expected one.
                EXIT_VALUE_MISMATCHED   : Attribute value is not matched
                                          to expected one.
                EXIT_NODE_NOT_FOUND     : Expected node not found.
                EXIT_ATTRIBUTE_NOT_FOUND: Attribute to check not found.
        """
        IsFound: bool                    = False
        Node   : Union[None, Et.Element] = None
        Value  : Union[None, str]        = None

        #
        # Get the node.
        #
        IsFound, Node = self.__XmlObject.GetNode (self.__XmlNodeList)
        if not IsFound:
            DEBUG (DEBUG_ERROR, f'Not found the node {self.__XmlNodeList}.')
            return EXIT_NODE_NOT_FOUND

        #
        # Get the attribute within the node.
        #
        IsFound, Value = self.__XmlObject.GetAttribute (Node, self.__Attribute)
        if not IsFound:
            DEBUG (DEBUG_ERROR, f'Not found the attribute {self.__Attribute}.')
            return EXIT_ATTRIBUTE_NOT_FOUND

        #
        # Check the attribute value is matched to expected one.
        #
        DEBUG (DEBUG_INFO, f'Attribute <{self.__Attribute}> in <{Node.tag}>:')
        DEBUG (DEBUG_INFO, f'  Current value : {Value}')
        DEBUG (DEBUG_INFO, f'  Expected value: {self.__ExpectValue}')
        if str (Value) != str (self.__ExpectValue):
            DEBUG (DEBUG_ERROR, f'XML config value is mismatched.')
            return EXIT_VALUE_MISMATCHED

        return EXIT_SUCCESS

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
                 '-i',
                 '-I',
                 '--input',
                 dest     = 'Input',
                 help     = 'IFWI XML Configuration file.',
                 required = True,
                 )
        Parser.add_argument (
                 '-n',
                 '-N',
                 '--node',
                 dest     = 'Node',
                 help     = 'The XML node of this attribute.',
                 nargs    = '+',
                 required = True,
                 )
        Parser.add_argument (
                 '-a',
                 '-A',
                 '--attribute',
                 dest     = 'Attribute',
                 help     = 'The attribute value of this setting.',
                 required = True,
                 )
        Parser.add_argument (
                 '-ev',
                 '-EV',
                 '--expect-value',
                 dest     = 'ExpectValue',
                 help     = 'The expected value.',
                 required = True,
                 )

        return Parser.parse_args ()

    Args: argparse.Namespace = GetArgs ()

    ConfigPath : str       = Args.Input
    XmlNodeList: List[str] = Args.Node
    Attribute  : str       = Args.Attribute
    ExpectValue: str       = Args.ExpectValue

    Result: int = IfwiXmlConfigParser (
                    ConfigPath  = ConfigPath,
                    XmlNodeList = XmlNodeList,
                    Attribute   = Attribute,
                    ExpectValue = ExpectValue,
                    ).Run ()

    sys.exit (Result)
