## @file
#  SuperClass to support to generate the correspond INI file.
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

import os
from typing import List, Union

from CapsuleCommon.Base.PathLib import *

#
# INI file comment prefix string.
#
INI_COMMENT_STR: str = ';'

#
# INI file single line type.
#
INI_ENTRY_TYPE  : int = 0x0
INI_COMMENT_TYPE: int = 0x1

#
# List of INI file single line type.
#
INI_TYPE_LIST: List[int] = [
    INI_ENTRY_TYPE,
    INI_COMMENT_TYPE,
    ]

class _Comment (object):
    def __init__ (self, Content: str) -> None:
        """ Inner class to represent the INI file comment line.

        Args:
            Content (str):
                The line of comment.
                (String would be strip)

        Raises:
            None.

        Returns:
            None.
        """
        self.Content: str = Content.strip ()

class _Entry (object):
    def __init__ (self, Key: str, Value: str) -> None:
        """ Inner class to represent the INI file data entry line.

        Args:
            Key (str):
                The key part of this entry.
                (String would be strip)
            Value (str):
                The value part of this entry.
                (String would be strip)

        Raises:
            None.

        Returns:
            None.
        """
        self.Key  : str = Key.strip ()
        self.Value: str = Value.strip ()

class _Section (object):
    def __init__ (self, Name: str) -> None:
        """ Inner class to represent the INI file section part.

        Note:
            One section could contain lots of comment or data entries.
            Key of data entry should be not duplicated.

        Args:
            Name (str):
                The name of this section.

        Raises:
            None.

        Returns:
            None.
        """
        self.Name: str  = Name
        self.Data: list = list ()

    def AddEntry (self, Key: str, Value: str, IsAllowDuplicated: bool) -> None:
        """ Add the new data entry to this section.

        Note:
            If entry is exist, would update the value.

        Args:
            Key (str):
                The key part of this entry.
            Value (str):
                The value part of this entry.
            IsAllowDuplicated (bool):
                Action to update the key is duplicated.
                True  - Add new entry directly.
                False - Update the value of the first entry get found.

        Raises:
            None.

        Returns:
            None.
        """
        UpdateData: _Entry = _Entry (Key, Value)
        IsFound   : bool   = False

        if IsAllowDuplicated:
            self.Data.append (UpdateData)
            return

        for Datum in self.Data:
            if not isinstance (Datum, _Entry):
                continue

            if Datum.Key == Key:
                Datum.Value = Value
                IsFound     = True
                break

        if not IsFound:
            self.Data.append (UpdateData)

    def AddComment (self, Key: str, Value: str) -> None:
        """ Add the new comment to this section.

        Note:
            (1) Add it directly, no need to check if duplicated.
            (2) Key and Value would be joined with on space.

        Args:
            Key (str):
                The key part of this entry.
            Value (str):
                The value part of this entry.

        Raises:
            None.

        Returns:
            None.
        """
        UpdateData: _Comment = _Comment (' '.join ([Key, Value]))

        self.Data.append (UpdateData)

class INICfgFile (object):
    def __init__ (self) -> None:
        """ SuperClass used to represent the INI configuration file.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.SectionNameList: List[str] = list ()
        self.SectionList    : list      = list ()

    def __CheckInputType (self, Key: str, Value: str) -> int:
        """ Get the input Key-Value pair type.

        Args:
            Key (str):
                The key part of this datum entry.
            Value (str):
                The value part of this datum entry.

        Raises:
            TypeError:
                (1) Entry Key is not str type.
                (2) Entry Value is not str type.
            ValueError:
                (1) Entry Key and value are empty.
                (2) Entry Key is empty.

        Returns:
            int:
                Type of this entry.
                (Should be one of INI_TYPE_LIST)
        """
        if not isinstance (Key, str):
            raise TypeError (f'Entry Key should be string.')
        elif not isinstance (Value, str):
            raise TypeError (f'Entry Value should be string.')
        elif (not Key) and (not Value):
            raise ValueError (f'Entry Key and Value cannot empty.')
        elif (not Key):
            raise ValueError (f'Entry Key cannot empty.')

        if Key.strip ().startswith (';'):
            return INI_COMMENT_TYPE
        else:
            return INI_ENTRY_TYPE

    def __FindSection (self, SectionName: str) -> _Section:
        """ Inner function to find the section by section name.

        Args:
            SectionName (str):
                The name of section to be found.

        Raises:
            None.

        Returns:
            _Section:
                The matched section with assigned section name.
                (If not found, return the None)
        """
        for Section in self.SectionList:
            if Section.Name == SectionName:
                return Section

        return None

    def AddSection (self, SectionName: str) -> None:
        """ Add the new section within configuration file.

        Note:
            If section exist, bypass the add operation.

        Args:
            SectionName (str):
                The name of section. (case-sensitive)

        Raises:
            TypeError:
                SectionName is not str type.
            ValueError:
                SectionName can not empty.

        Returns:
            None.
        """
        if not isinstance (SectionName, str):
            raise TypeError ('Section name should be str type.')
        if not SectionName:
            raise ValueError ('Section name cannot empty.')

        if SectionName not in self.SectionNameList:
            self.SectionNameList.append (SectionName)
            self.SectionList.append (_Section (SectionName))

    def UpdateDataToSection (
        self,
        SectionName      : str,
        SectionData      : list,
        IsAllowDuplicated: bool = False,
        ) -> None:
        """ Update the section data with list of key-value pair.

        Args:
            SectionName (str):
                Name of section to be updated the key-value data.
            SectionData (list):
                Key-value data represent by list of List[str, str].
            IsAllowDuplicated (bool):
                Action to update the key is duplicated.
                True  - Add new entry directly.
                False - Update the value of the first entry get found.
                Defaults to False.

        Raises:
            TypeError:
                SectionData is not list type.
            ValueError:
                (1) Failed to find the assigned section.
                (2) Unsupported type of input.

        Returns:
            None.
        """
        DatumType: int      = None
        Section  : _Section = None

        if not isinstance (SectionData, list):
            raise TypeError (
                    'Section Data should be list of List[Key, Value]'
                    )

        self.AddSection (SectionName)

        for Key, Value in SectionData:
            DatumType = self.__CheckInputType (Key, Value)
            Section   = self.__FindSection (SectionName)

            if not Section:
                raise ValueError (
                        'Unexpected error, cannot find the section.'
                        )

            if DatumType == INI_COMMENT_TYPE:
                Section.AddComment (Key, Value)
            elif DatumType == INI_ENTRY_TYPE:
                Section.AddEntry (Key, Value, IsAllowDuplicated)
            else:
                raise ValueError ('Unsupported input type.')

    @property
    def StringList (self) -> List[str]:
        """ Return the list of string of INI configuration file.

        Args:
            None.

        Raises:
            TypeError:
                Unsupported type of input.

        Returns:
            List[str]:
                List of string of INI configuration file.
        """
        SectionName: str  = str ()
        StringList : list = list ()
        MaxKeyLen  : int  = int ()

        for Section in self.SectionList:
            SectionName = Section.Name
            StringList.append (f'[{SectionName}]\n')
            MaxKeyLen   = 0

            #
            # Get the maximum length of key within this section.
            #
            for Datum in Section.Data:
                if not isinstance (Datum, _Entry):
                    continue

                if len (Datum.Key) > MaxKeyLen:
                    MaxKeyLen = len (Datum.Key)

            #
            # Append the content based on type.
            #
            for Datum in Section.Data:
                if isinstance (Datum, _Entry):
                    if Datum.Value:
                        StringList.\
                            append (
                              f'{Datum.Key.ljust(MaxKeyLen)} = {Datum.Value}\n'
                              )
                    else:
                        StringList.append (f'{Datum.Key}\n')
                elif isinstance (Datum, _Comment):
                    StringList.append (f'{Datum.Content}\n')
                else:
                    raise TypeError (f'Unsupported input type.')

            StringList.append (f'\n')

        return StringList

    def Save (
        self,
        FilePath: Union[str, os.PathLike],
        FileMode: str = 'w',
        ) -> None:
        """ Save the INI configuration file into storage.

        Args:
            FilePath (Union[str, os.PathLike]):
                Path of the INI to be saved.
            FileMode (str, optional):
                File open mode.
                Defaults to 'w'.

        Raises:
            None.

        Returns:
            None.
        """
        OutputPath: str       = AbsPath (FilePath)
        StringList: List[str] = self.StringList

        with open (OutputPath, FileMode) as Fp:
            for Line in StringList:
                Fp.write (Line)

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'INICfgFile',
    ]
