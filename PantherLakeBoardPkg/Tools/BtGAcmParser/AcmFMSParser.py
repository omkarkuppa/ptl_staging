## @file
#  File to Parser the FMS information from given HEX data.
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

from Utils import *

#                        bit
# Rsvd               : [31:28]
# Extended Family EAX: [27:20]
# Extended Model EAX : [19:16]
# Rsvd               : [15:14]
# Type EAX           : [13:12]
# Family EAX         : [11: 8]
# Model EAX          : [ 7: 4]
# Stepping EAX       : [ 3: 0]
#

STRUCT_MAX_BIT_SIZE = 32

class AcmFMSParser (object):
    def __init__ (self, InputValue):
        self.__InputValue = ZeroPadding (format (int (InputValue, 0), 'b'), STRUCT_MAX_BIT_SIZE)
        self.__Family     = self.__GetBitRange (self.__InputValue, 8, 11)
        self.__Model      = self.__GetBitRange (self.__InputValue, 4, 7)
        self.__Stepping   = self.__GetBitRange (self.__InputValue, 0, 3)
        self.__Type       = self.__GetBitRange (self.__InputValue, 12, 13)
        self.__Ext_Family = self.__GetBitRange (self.__InputValue, 20, 27)
        self.__Ext_Model  = self.__GetBitRange (self.__InputValue, 16, 19)

    def Get (self):
        """ To return the Family / Model / Stepping information from given FMS data.

        Args:
            None.

        Returns:
            FMSValue [dict]: The FMS information parsing from hexadecimal value.

        Raises:
            None.
        """
        return {
            'Family'    : self.__Family,
            'Model'     : self.__Model,
            'Type'      : self.__Type,
            'Ext_Family': self.__Ext_Family,
            'Ext_Model' : self.__Ext_Model,
            'Stepping'  : self.__Stepping
        }

    def __GetBitRange (self, BinStr, Start, End):
        """ To get the specific bit range value from given hex value.

        Args:
            BinStr [str]: The hexadecimal value string without "0x" prefix.
            Start  [int]: The bit start range.
            End    [int]: The bit end range.

        Returns:
            Value [str]: The value of this bit range. (Hex format without "0x" prefix.)

        Raises:
            None.
        """
        return FormatHex (int (BinStr[::-1][Start:End+1][::-1], 2), Prefix=False)
