## @file
#  This library class is used to convert the search item into byte format.
#  Support to covert ANSI byte.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2022 Intel Corporation.
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
# @par Specification
##
class ByteBufferIo (object):
    def __init__ (self, Input):
        self.__Input     = Input
        self.__InputType = type (self.__Input)

    def Process (self):
        """ To mapping the input type to call the correspond method to covert to byte type.

        Args:
            None.

        Returns:
            Result  Union[None, bytes]: Should only return bytes type but default is None type.

        Raises:
            TypeError: If the input type not supported in current flow would raise this exception.
        """
        Result = None

        #
        # Based on the input type to call the mapping process method.
        #
        DISPATCHER_DICT: dict = {
            str      : self.__ProcessString,
            bytes    : self.__ProcessBytes,
            bytearray: self.__ProcessBytes,
        }

        if self.__InputType not in DISPATCHER_DICT:
            raise TypeError
        else:
            Function = DISPATCHER_DICT[self.__InputType]
            Result   = Function ()

        return Result

    def __ProcessBytes (self):
        """ Covert the byte type to byte type. (Directly return no need to process.)

        Args:
            None.

        Returns:
            __Input [bytes|bytearray]: Take the input directly to return.

        Raises:
            None.
        """
        return self.__Input

    def __ProcessString (self):
        """ Covert the string type to ANSI byte type.

        Args:
            None.

        Returns:
            Result  [bytes]: The result from the string to ANSI byte type.

        Raises:
            None.
        """
        Result = bytes (str (self.__Input), 'ANSI')
        return Result
