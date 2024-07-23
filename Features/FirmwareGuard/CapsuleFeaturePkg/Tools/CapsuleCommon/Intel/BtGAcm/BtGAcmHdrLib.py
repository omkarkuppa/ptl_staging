## @file
#  Definition of BtGAcm header tables.
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

from CapsuleCommon.Intel.BtGAcm._AcmInfoTableLib import *
from CapsuleCommon.Intel.BtGAcm._AcmVerInfoTableLib import *

class AcmInfoTable (object):
    #
    # List of items need to check for input key within dict.
    #
    ACM_INFO_TBL_MANDATORY: List[str] = [
        ACM_INFO_TBL_CHIPSET_ACM_TYPE,
        ACM_INFO_TBL_AIT_VERSION,
        ACM_INFO_TBL_AIT_LENGTH,
        ACM_INFO_TBL_CHIPSET_ID_LIST,
        ACM_INFO_TBL_OS_SINIT_TBL_VER,
        ACM_INFO_TBL_MIN_MLE_HEADER_VER,
        ACM_INFO_TBL_SINIT_CAPS,
        ACM_INFO_TBL_OFFSET_36,
        ACM_INFO_TBL_OFFSET_37,
        ACM_INFO_TBL_PROCESSOR_ID_LIST,
        ACM_INFO_TBL_TPM_INFO_LIST,
        ]

    def __init__ (
        self,
        **Kwargs: Dict[str, Any],
        ) -> None:
        """ Class used to represent the BtGAcm information table.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of BtGAcm information table structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__ChipsetAcmType : str = \
            self.__Kwargs[ACM_INFO_TBL_CHIPSET_ACM_TYPE]
        self.__AitVersion     : str = \
            self.__Kwargs[ACM_INFO_TBL_AIT_VERSION]
        self.__AitLength      : str = \
            self.__Kwargs[ACM_INFO_TBL_AIT_LENGTH]
        self.__ChipsetIdList  : str = \
            self.__Kwargs[ACM_INFO_TBL_CHIPSET_ID_LIST]
        self.__OsSinitTblVer  : str = \
            self.__Kwargs[ACM_INFO_TBL_OS_SINIT_TBL_VER]
        self.__MinMleHdrVer   : str = \
            self.__Kwargs[ACM_INFO_TBL_MIN_MLE_HEADER_VER]
        self.__SinitCaps      : str = \
            self.__Kwargs[ACM_INFO_TBL_SINIT_CAPS]
        self.__Offset36       : str = \
            self.__Kwargs[ACM_INFO_TBL_OFFSET_36]
        self.__Offset37       : str = \
            self.__Kwargs[ACM_INFO_TBL_OFFSET_37]
        self.__ProcessorIdList: str = \
            self.__Kwargs[ACM_INFO_TBL_PROCESSOR_ID_LIST]
        self.__TpmInfoList    : str = \
            self.__Kwargs[ACM_INFO_TBL_TPM_INFO_LIST]

    @property
    def ChipsetAcmType (self) -> str:
        """ Return the type of chipset ACM. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Type of chipset ACM.
        """
        return self.__ChipsetAcmType

    @property
    def AitVersion (self) -> str:
        """ Return the ACM information table header version. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Header version of ACM information table.
        """
        return self.__AitVersion

    @property
    def AitLength (self) -> str:
        """ Return the ACM information table length. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Length of the ACM information table.
        """
        return self.__AitLength

    @property
    def ChipsetIdList (self) -> str:
        """ Return the offset of chipset ID list. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Offset of chipset ID list.
        """
        return self.__ChipsetIdList

    @property
    def OsSinitDataTableVersion (self) -> str:
        """ Return the OS silicon initial data table version. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Version of the OS silicon initial data table.
        """
        return self.__OsSinitTblVer

    @property
    def MinMleHeaderVersion (self) -> str:
        """ Return the version of the minimum MLE header. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Value of the minimum MLE header.
        """
        return self.__MinMleHdrVer

    @property
    def SinitCapabilities (self) -> str:
        """ Return the value of silicon initial capabilities. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Value of silicon initial capabilities.
        """
        return self.__SinitCaps

    @property
    def Offset36 (self) -> str:
        """ Return the offset 36 value. (1 byte)

        Note:
            For ver.7, this means SinitVersion.
            For ver.9, this had been cleared as reserved.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Value of the offset 36.
        """
        return self.__Offset36

    @property
    def Offset37 (self) -> str:
        """ Return the offset 37 value. (3 bytes)

        Note:
            For ver.7, this means AitRevision[3].
            For ver.9, this had been cleared as reserved.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Value of the offset 37.
        """
        return self.__Offset37

    @property
    def ProcessorIdList (self) -> str:
        """ Return the offset of processor ID list. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Offset of processor ID list.
        """
        return self.__ProcessorIdList

    @property
    def TpmInfoList (self) -> str:
        """ Return the offset of TPM information list. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Offset of TPM information list.
        """
        return self.__TpmInfoList

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                ACM information table structure member is missing.

        Returns:
            None.
        """
        #
        # To check header information is meet the mandatory list.
        #
        for Member in AcmInfoTable.ACM_INFO_TBL_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )

class AcmVerInfoTable (object):
    Signature: str = ACM_VER_INFO_TBL_SIGNATURE
    #
    # List of items need to check for input key within dict.
    #
    ACM_INFO_TABLE_MANDATORY: List[str] = [
        ACM_VER_INFO_TBL_ID,
        ACM_VER_INFO_TBL_LENGTH,
        ACM_VER_INFO_TBL_REVISION,
        ACM_VER_INFO_TBL_ACM_INFO_TBL_VER,
        ACM_VER_INFO_TBL_GENERATION_VER,
        ACM_VER_INFO_TBL_CORE_VER,
        ACM_VER_INFO_TBL_ACM_REVISION,
        ACM_VER_INFO_TBL_ACM_SECURITY_VER,
        ]

    def __init__ (
        self,
        **Kwargs: Dict[str, Any],
        ) -> None:
        """ Class to represent the BtGAcm version information table.

        Note:
            This is valid start from ver.9.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of BtGAcm information table structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__Id            : str = \
            self.__Kwargs[ACM_VER_INFO_TBL_ID]
        self.__Length        : str = \
            self.__Kwargs[ACM_VER_INFO_TBL_LENGTH]
        self.__Revision      : str = \
            self.__Kwargs[ACM_VER_INFO_TBL_REVISION]
        self.__AcmInfoTblVer : str = \
            self.__Kwargs[ACM_VER_INFO_TBL_ACM_INFO_TBL_VER]
        self.__GenerationVer : str = \
            self.__Kwargs[ACM_VER_INFO_TBL_GENERATION_VER]
        self.__CoreVer       : str = \
            self.__Kwargs[ACM_VER_INFO_TBL_CORE_VER]
        self.__AcmRevision   : str = \
            self.__Kwargs[ACM_VER_INFO_TBL_ACM_REVISION]
        self.__AcmSecurityVer: str = \
            self.__Kwargs[ACM_VER_INFO_TBL_ACM_SECURITY_VER]

    @property
    def Id (self) -> str:
        """ Return the ID of the ACM version info table. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Signature of this table.
        """
        return self.__Id

    @property
    def Length (self) -> str:
        """ Return the length of the ACM version info table. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Length of the ACM version information table.
        """
        return self.__Length

    @property
    def Revision (self) -> str:
        """ Return the revision of this table. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Revision value of this table.
        """
        return self.__Revision

    @property
    def AcmInformationTableVersion (self) -> str:
        """ Return version of the ACM information table. (4 bytes)

        Note:
            Format: Major.Minor.Flavor ([1 byte].[1 byte].[2 bytes]).

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Value of the ACM information table.
        """
        return self.__AcmInfoTblVer

    @property
    def GenerationVersion (self) -> str:
        """ Return value of the generation version. (4 bytes)

        Note:
            Format: Major.Minor.Flavor ([1 byte].[1 byte].[2 bytes]).

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Value of the generation version.
        """
        return self.__GenerationVer

    @property
    def CoreVersion (self) -> str:
        """ Return value of the core version. (4 bytes)

        Note:
            Format: Major.Minor.Flavor ([1 byte].[1 byte].[2 bytes]).

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Value of the core version.
        """
        return self.__CoreVer

    @property
    def AcmRevision (self) -> str:
        """ Return value of the ACM revision. (4 bytes)

        Note:
            Format: Major.Minor.Build ([1 byte].[1 byte].[2 bytes]).

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Value of ACM revision. (BtGAcm version)
        """
        return self.__AcmRevision

    @property
    def AcmSecurityVersion (self) -> str:
        """ Return value of the ACM security version. (4 bytes)

        Note:
            (1) For SINIT, it means SINIT version.
            (2) For StartupAcm, it means minimal SINIT SVN.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Value of the ACM security version.
        """
        return self.__AcmSecurityVer

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) ACM version information table member is missing.
                (2) ACM version information table signature is
                    incorrect. (ACM_VER_INFO_TBL_SIGNATURE)

        Returns:
            None.
        """
        InputSig: str = str ()
        #
        # To check header information is meet the mandatory list.
        #
        for Member in AcmVerInfoTable.ACM_INFO_TABLE_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )

        #
        # To check the parsed signature is correct.
        #
        InputSig = ByteToStr (self.__Kwargs[ACM_VER_INFO_TBL_ID])
        if not (InputSig == AcmVerInfoTable.Signature):
            raise ValueError (
                    f'Input signature ({InputSig}) is not '
                    f'{AcmVerInfoTable.Signature}'
                    )
