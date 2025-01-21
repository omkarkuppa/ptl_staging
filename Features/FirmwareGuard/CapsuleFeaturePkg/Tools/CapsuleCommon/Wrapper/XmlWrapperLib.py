## @file
#  Wrapper to manipulate/parse the XML file.
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

import xml.etree.ElementTree as Et
from typing import Tuple

from CapsuleCommon.Base.PathLib import *

XML_NODE_NOT_FOUND     : None = None
XML_ATTRIBUTE_NOT_FOUND: None = None

class XmlWrapper (object):
    def __init__ (self, FilePath: str):
        """ Struct class to support common manipulate on XML file.

        Args:
            FilePath (str):
                The path of XML file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FilePath: str            = AbsPath (FilePath)
        self.__XmlTree : Et.ElementTree = Et.parse (self.__FilePath)

    @property
    def FilePath (self) -> str:
        """ Get the input XML file path.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The absolute path of input XML file.
        """
        return self.__FilePath

    @property
    def Tree (self) -> Et.ElementTree:
        """ Get the XML file ElementTree object.

        Args:
            None.

        Raises:
            None.

        Returns:
            Et.ElementTree:
                XML ElementTree object.
        """
        return self.__XmlTree

    @property
    def Root (self) -> Et.Element:
        """ Get the XML file root node.

        Args:
            None.

        Raises:
            None.

        Returns:
            Et.Element:
                The root within the XML file.
        """
        return self.__XmlTree.getroot ()

    def GetNode (
        self,
        NodeList: List[str],
        BaseNode: Union[None, Et.Element] = None,
        ) -> Tuple[bool, Union[None, Et.Element]]:
        """ Get the specific XML node by list of string.

        Args:
            NodeList (List[str]):
                The list of string to get the specific node.
                (From top to down)
            BaseNode (Union[None, Et.Element], optional):
                The start base node.
                Defaults to None. (Would use root as base node)

        Raises:
            TypeError:
                Input base not is not Element type.

        Returns:
            Tuple[bool, Union[None, Et.Element]]:
                bool:
                    True  - Found the node.
                    False - Not found the node.
                Union[None, Et.Element]:
                    If not found the node, would return the None.
                    If found the node
        """
        CurrNode: Et.Element = None

        if BaseNode is None:
            CurrNode = self.Root
        else:
            CurrNode = BaseNode

        if not isinstance (CurrNode, Et.Element):
            raise TypeError (f'Base node should be Element object.')

        for Node in NodeList:
            CurrNode = CurrNode.find (Node)

            if CurrNode == XML_NODE_NOT_FOUND:
                DEBUG (DEBUG_ERROR, f'XML node [{Node}] not found.')
                return False, None

            DEBUG (DEBUG_TRACE, f'Find the XML node [{Node}].')

        return True, CurrNode

    def GetAttribute (
        self,
        TargetNode: Et.Element,
        Attribute : str,
        ) -> Tuple[bool, Union[None, str]]:
        """ Get the attribute within the specific node.

        Args:
            TargetNode (Et.Element):
                The target XML node to be get the attribute.
            Attribute (str):
                The attribute to be found within this node.

        Raises:
            TypeError:
                (1) Input TargetNode is not Element object.
                (2) Input Attribute is not string.

        Returns:
            Tuple[bool, Union[None, str]]:
                bool:
                    True  - Found the attribute within the node.
                    False - Not found the attribute within the node.
                Union[None, str]:
                    If not found the attribute, would return the None.
                    If found the attribute, would return the correspond
                    string.
        """
        if not isinstance (TargetNode, Et.Element):
            raise TypeError (f'Input node should be Element object.')
        if not isinstance (Attribute, str):
            raise TypeError (f'Input attribute should be str.')

        Value: Union[None, str] = TargetNode.attrib.get (Attribute)

        if Value == XML_ATTRIBUTE_NOT_FOUND:
            DEBUG (
              DEBUG_ERROR,
              f'Cannot find attribute {Attribute} in node {TargetNode.tag}'
              )

            return False, None

        return True, Value
