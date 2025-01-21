## @file
# StripFlashmap.py
#
# This script removes unneeded lines in fdf according to Build Target.
# This is for the subsequent FSP rebase script which does not support "!if $(TARGET) == DEBUG/!else/!endif" condictions.
# Only one pair of "!if $(TARGET) == DEBUG/!else/!endif" is supported.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2020 Intel Corporation.
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
#
##

import os
import sys
import re
import subprocess

if len(sys.argv) != 4:
  print("StripFlashmap.py - Error in number of arguments received")
  print("Usage - StripFlashmap.py <Build Target - DEBUG/RELEASE> <FlashMap file path> <Outputted FlashMap file path>")
  exit(1)

buildTarget         = sys.argv[1].upper()
flashMapName        = os.path.realpath(sys.argv[2])
outputflashMapName  = os.path.realpath(sys.argv[3])

#
# Make sure argument passed or valid
#
if not (buildTarget in "DEBUG" or buildTarget in "RELEASE") :
  print("WARNING!  " + str(buildTarget) + " is not a valid build target")
  exit(1)
if not os.path.exists(flashMapName):
  print("WARNING!  " + str(flashMapName) + " is not found.")
  exit(1)

#
# Get Flash Map
#
file = open (flashMapName, "r")
data = file.read ()
file.close ()

# Get the common data before !if $(TARGET) == DEBUG
flashmapcommonhead = data.rsplit("!if $(TARGET) == DEBUG", 1)[0]

# Based on Build Target, select the section in the FlashMap file
if buildTarget in "DEBUG":
  flashmap = data.rsplit("!if $(TARGET) == DEBUG", 1)[1].split("!else #(Release)")[0]
else:
  flashmap = data.rsplit("!else #(Release)", 1)[1].split("!endif #(Release)")[0]

# Get the tail common data after !endif
flashmapcommontail = data.rsplit("!endif #(Release)", 1)[1]

strippedFlashMap = flashmapcommonhead + flashmap + flashmapcommontail


outputflashMap = open(outputflashMapName, "w")
outputflashMap.write(strippedFlashMap)
outputflashMap.close()

exit(0)
