## @file
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2017 Intel Corporation.
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


#
# Build FitGen tools.
#
cd $WORKSPACE_CORE_SILICON/Tools
make
if [ $? -ne 0 ]; then
  echo "Build FitGen tools fail."
fi
cd $WORKSPACE_BINARIES/Tools/KeyEnroll
make
if [ $? -ne 0 ]; then
  echo "Build KeyEnroll tools fail."
fi


#
# Add additional tools in PATH.
#
export PATH=$WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/SignFv:$PATH
export PATH=$EDK_TOOLS_PATH/Source/C/bin:$PATH
export PATH=$WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen:$PATH
export PATH=$WORKSPACE_BINARIES/Tools/KeyEnroll:$PATH
