## @file
#  API to generate the parameter for FitGen tool.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2025 Intel Corporation.
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
#  @par Specification Reference:
#
##

import math
from typing import List

from BtgAcmMisc.BtgAcm._AcmProcessorIdListParser import *
from BtgAcmMisc._BtgAcmBinParser import *

def GetCmdJoinStr () -> str:
    """ Return the string to change line for single command in shell.

    Args:
        None.

    Raises:
        None.

    Returns:
        str:
            String used to change line for single command in shell.
    """
    if os.name == 'nt':
        return ' ^\n'
    else:
        return ' \n'

class BtgAcmFitEntryGenerator (object):
    def __init__ (
        self,
        Address : int,
        BinInfo : BtgAcmBinInfo,
        SlotSize: int,
        ) -> None:
        """ Class to generate the parameter for FitGen tool.

        Args:
            Address (int):
                Memory address to this BtGACM binary.
            BinInfo (BtgAcmBinInfo):
                Information of this BtGACM binary.
            SlotSize (int):
                Slot size of each BtGACM binary file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Address : int           = Address
        self.__BinInfo : BtgAcmBinInfo = BinInfo
        self.__SlotSize: int           = SlotSize

        self.__PreCheck ()

        self.__CmdList: List[str] = self.__GetCmdList ()

    @property
    def CmdList (self) -> List[str]:
        """ Return the list of command as input parameter for FitGen.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[str]:
                List of command as input parameter for FitGen.
        """
        return self.__CmdList

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Address is not int type.
                (2) BinInfo is not BtgAcmBinInfo type.
                (3) SlotSize is not int type.

        Returns:
            None.
        """
        if not isinstance (self.__Address, int):
            raise TypeError ('Address shall be int type.')
        elif not isinstance (self.__BinInfo, BtgAcmBinInfo):
            raise TypeError ('BinInfo shall be BtgAcmBinInfo type.')
        elif not isinstance (self.__SlotSize, int):
            raise TypeError ('SlotSize shall be int type.')

    def __GetCmdList (self) -> List[str]:
        """ Decode the processor ID to convert into FitGen command.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[str]:
                List of command as input parameter for FitGen.
        """
        CmdList   : List[str]      = list ()
        Count     : int            = 0
        CpuId     : AcmProcessorId = None
        Cmd       : str            = None
        AcmBase   : str            = f'0x{self.__Address:08X}'
        AcmSize   : str            = f'0x{self.__SlotSize:08X}'
        FmsVal    : str            = None
        FmsMaskVal: str            = None

        print (f'ACM Base Address = 0x{AcmBase}')
        print (f'ACM Base Size    = 0x{AcmSize}')

        for CpuId in self.__BinInfo.ProcessorIdList:
            FmsVal     = f'0x{CpuId.FMS.Value:08X}'
            FmsMaskVal = f'0x{CpuId.FMSMask.Value:08X}'

            print (f'================ [{Count}] ================')

            print (f'FMS        = {FmsVal}')
            print (f'FMSMask    = {FmsMaskVal}')

            SizeByte0  = CpuId.FMS.Family             + CpuId.FMS.Model
            SizeByte1  = CpuId.FMS.ExtModel           + CpuId.FMS.Type
            SizeByte2  = CpuId.FMSMask.Family         + CpuId.FMSMask.Model
            RsvdByte   = CpuId.FMSMask.ExtModel       + CpuId.FMSMask.Type
            ChksumByte = CpuId.FMSMask.ExtFamily[0:1] + CpuId.FMS.ExtFamily[0:1]

            print (f'SizeByte0  = 0x{SizeByte0}')
            print (f'SizeByte1  = 0x{SizeByte1}')
            print (f'SizeByte2  = 0x{SizeByte2}')
            print (f'RsvdByte   = 0x{RsvdByte}')
            print (f'ChksumByte = 0x{ChksumByte}')

            print ('=====================================')

            Cmd = f"-S {AcmBase} {AcmSize} -I {FmsVal} {FmsMaskVal}"
            CmdList.append (Cmd)

            Count += 1

        return CmdList
