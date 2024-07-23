## @file
#  Library support to create the capsule payload.
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

import xml.etree.ElementTree as ET

from CapsuleCommon import *

from CapsuleGenerate._Global import *
from CapsuleGenerate.ApiPrivate import *

class GenCapsulePayload (object):
    def __init__ (
        self,
        IfwiPath     : Union[str, os.PathLike],
        FitToolPath  : Union[str, os.PathLike],
        IsFwuRequired: bool,
        CseImagePath : Union[str, os.PathLike],
        ) -> None:
        """ Class to generate the capsule payload.

        Note:
            (1) Decompose IFWI from FIT tool.
            (2) Generate the CSME FWUpdate image. (Conditional)

        Args:
            IfwiPath (Union[str, os.PathLike]):
                The path of IFWI image to be decomposed.
            FitToolPath (Union[str, os.PathLike]):
                The FIT tool to support to do the decomposing.
            IsFwuRequired (bool):
                True  - Need to generate the CSME FWUpdate image.
                False - No need to generate the CSME FWUpdate image.
            CseImagePath (str):
                Path of CSE image.
                (If input is corporate IFWI and need to generate the
                FWUpdate image, need is mandatory)

        Raises:
            None.

        Returns:
            None.
        """
        self.__IfwiPath     : str  = AbsPath (IfwiPath)
        self.__FitToolPath  : str  = AbsPath (FitToolPath)
        self.__IsFwuRequired: bool = IsFwuRequired
        self.__CseImagePath : str  = AbsPath (CseImagePath)

        self.__PreCheck ()

        #
        # Decompose the IFWI image
        #
        self.__IfwiDecomposeApi: IfwiDecomposeApi = \
            IfwiDecomposeApi (
              IfwiPath    = self.__IfwiPath,
              FitToolPath = self.__FitToolPath,
              )
        self.__IfwiDecomposeApi.Execute ()

        self.__FwInfo         : dict = self.__IfwiDecomposeApi.FwInfo
        self.__IfwiCfgFilePath: str  = self.__IfwiDecomposeApi.XmlCfgFile
        self.__IfwiType       : str  = self.__GetIfwiType ()
        DEBUG (
          DEBUG_TRACE,
          f'IFWI information: {self.__FwInfo}, '
          f'IFWI XML Config: {self.__IfwiCfgFilePath}, '
          f'IFWI type: {self.__IfwiType}'
          )

        #
        # Generate the CSME FWUpdate image
        #
        self.__FWUpdateImgPath: str = str ()

        if not self.__IsFwuRequired:
            DEBUG (DEBUG_TRACE, f'No need to generate the FWUpdate image.')
            return

        self.__GenFWUpdatePreCheck ()
        self.__FwuGenApi: FwuGenerateApi = \
            FwuGenerateApi (
              IfwiPath        = self.__IfwiPath,
              FitToolPath     = self.__FitToolPath,
              IfwiCfgFilePath = self.__IfwiCfgFilePath,
              CSEImagePath    = self.__CseImagePath,
              )
        self.__FwuGenApi.Execute ()

        self.__FWUpdateImgPath = self.__FwuGenApi.Path

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                IsFwuRequired is not bool type.

        Returns:
            None.
        """
        if not isinstance (self.__IsFwuRequired, bool):
            raise TypeError (f'IsFwuRequired should be bool type.')

        CheckPathList: List[str] = [
            self.__IfwiPath,
            self.__FitToolPath,
            ]

        CheckFileListExist (CheckPathList)

    def __GenFWUpdatePreCheck (self) -> None:
        """ Check the input argument meet the requirement and is valid.

        Args:
            None.

        Raises:
            ValueError:
                Corporate IFWI not input the CSE image.

        Returns:
            None.
        """
        if (self.__IfwiType == IFWI_CORPORATE) and (not self.__CseImagePath):
            raise ValueError (
                    f'Corporate IFWI require \"-cse\" to assign '
                    f'the CSE region to generate the CSME FWUpdate image.'
                    )

        if self.__CseImagePath:
            CheckPathList: List[str] = [self.__CseImagePath]

            CheckFileListExist (CheckPathList)

    def __GetIfwiType (self) -> str:
        """ Return the IFWI type is Consumer or Corporate.

        Args:
            None.

        Raises:
            ValueError:
                (1) Can not find the IFWI layout name.
                (2) Layout name is not contain Consumer or Corporate.

        Returns:
            str:
                Type of IFWI.
        """
        LayoutName: str            = str ()
        Tree      : Et.ElementTree = ET.parse (self.__IfwiCfgFilePath)
        Root      : Et.Element     = Tree.getroot ()

        LayoutName = Root.attrib.get ('layout_name')

        if not LayoutName:
            raise ValueError (f'Failed to get the IFWI layout.')
        elif IFWI_CORPORATE in LayoutName:
            return IFWI_CORPORATE
        elif IFWI_CONSUMER in LayoutName:
            return IFWI_CONSUMER
        else:
            raise ValueError (f'IFWI type is illegal.')

    def CopyFwComponent (self):
        """ Copy the firmware component from decompose/generate folder.

        Note:
            (1) Function would support to copy to temporary directory.
            (2) Replace to dummy file into real one.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        DestPath           : str  = str ()
        MappingKey         : str  = str ()
        CfgAndFwInfoMapping: dict = {
            FW_BIOS      : BIOS_COMPONENT_KEY,
            FW_EC        : EC_COMPONENT_KEY,
            FW_CSE_REGION: ME_COMPONENT_KEY,
            FW_FWUPDATE  : ME_COMPONENT_KEY,
            FW_GBE       : GBE_COMPONENT_KEY,
            FW_ISH_PDT   : ISH_PDT_COMPONENT_KEY,
            }


        for Component, SrcPath in self.__FwInfo.items ():
            if Component not in CfgAndFwInfoMapping:
                continue

            MappingKey = CfgAndFwInfoMapping[Component]
            DestPath   = JoinPath (
                           TEMPORARY_DIR_PATH,
                           gFwComponentInfo[MappingKey][FILE_NAME_KEY],
                           )

            CopyFile (SrcPath, DestPath)

        #
        # Copy FWUpdate image as ME part
        #
        if self.__FWUpdateImgPath:
            MappingKey = CfgAndFwInfoMapping[FW_FWUPDATE]
            DestPath   = JoinPath (
                           TEMPORARY_DIR_PATH,
                           gFwComponentInfo[MappingKey][FILE_NAME_KEY],
                           )
            CopyFile (self.__FWUpdateImgPath, DestPath)
