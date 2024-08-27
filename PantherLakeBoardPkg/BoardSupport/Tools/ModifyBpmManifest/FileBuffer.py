## @file
#  File to process the file buffer as lines.
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
# @par Specification
##
import os

OUTPUT_FILE_NAME = "%s.patched"

class FileBuffer (object):
    def __init__ (self, FileBuffer):
        self.__FileBuffer = FileBuffer
        self.__FilePath = FileBuffer.name

        self.__BasePath = os.path.dirname (self.Path)

        self.__OutputFileName = OUTPUT_FILE_NAME % (os.path.basename (self.__FilePath))
        self.__OutputFilePath = os.path.join (self.__BasePath, self.__OutputFileName)

        self.__DataLines = self.__FileBuffer.readlines ()

    @property
    def Path (self):
        """ The input file path. (Absolute path.)

        Args:
            None.

        Returns:
            Path [str]: Convert the input file path to absolute path.

        Raises:
            None.
        """
        if os.path.isabs (self.__FilePath):
            return self.__FilePath
        else:
            return os.path.abspath (self.__FilePath)

    @property
    def Length (self):
        """ Get the number of lines in this file buffer.

        Args:
            None.

        Returns:
            Length [int]: The number of lines in this file buffer.

        Raises:
            None.
        """
        return len (self.__DataLines)

    def GetLineData (self, Idx):
        """ Get the specific line content in this file buffer.

        Args:
            Idx [int]: The position to get the line content.

        Returns:
            Content [str]: Specific line content.

        Raises:
            None.
        """
        return self.__DataLines[Idx]

    def Replace (self, Idx, Origin, Revised):
        """ To replace the specific line content in this file buffer.

        Args:
            Idx     [int]: The position to do the modification of the line content.
            Origin  [str]: The specific line content before modification.
            Revised [str]: The specific line content after modification.

        Returns:
            None.

        Raises:
            None.
        """
        self.__DataLines[Idx] = self.__DataLines[Idx].replace (Origin, Revised)

    def FindKeywordInRange (self, RangeTupleList, Keyword):
        """ Find the specific keyword in the specific range list.

        Args:
            RangeTupleList [List]: The range tuple list. Format should be [(Start, End), ...]
            Keyword        [str] : The keyword string want to search in line.

        Returns:
            ResultSectionList [List] : List of find the specific keyword in range. Format should be [(Start, End), ...]
            ResultPositionList [List]: List of find the specific keyword in position.

        Raises:
            None.
        """
        ResultSectionList = list ()
        ResultPositionList = list ()

        for StartLoc, EndLoc in RangeTupleList:
            FindResult = self.FindKeywordInLine (Keyword, StartLoc, EndLoc)
            if len (FindResult) == 0:
                continue
            else:
                ResultSectionList.append ((StartLoc, EndLoc))
                for Loc in FindResult:
                    ResultPositionList.append (Loc)

        return ResultSectionList, ResultPositionList

    def FindKeywordInLine (self, String, Start=None, End=None):
        """ Find the specific keyword in the specific range.

        Args:
            String [str]: The keyword string want to search in line.
            Start  [int]: The position start to search. (Default=0)
            End    [int]: The position end to search. (Default=EOL)

        Returns:
            ResultList [List]: List of find the specific keyword in position.

        Raises:
            None.
        """
        ResultList = list ()
        StartLoc = 0
        EndLoc = len (self.__DataLines)

        if Start:
            StartLoc = Start

        if End:
            EndLoc = End

        for Idx in range (StartLoc, EndLoc):
            if str (String) in self.__DataLines[Idx]:
                ResultList.append (Idx)

        return ResultList

    def Save (self):
        """ Save the file to output path.

        Args:
            None.

        Returns:
            None.

        Raises:
            None.
        """
        print (f"Save the file to {self.__OutputFilePath}")
        with open (self.__OutputFilePath, "w") as File:
            for Line in self.__DataLines:
                File.write("%s" % Line)
