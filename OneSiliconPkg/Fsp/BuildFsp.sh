#!/bin/bash

## @file
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

if [ -z "${WORKSPACE}" ]; then
  export WORKSPACE=`pwd`
fi

if [ -z "${WORKSPACE_CORE}" ]; then
  export WORKSPACE_CORE=$WORKSPACE/Edk2
fi

if [ -z "${WORKSPACE_COMMON}" ]; then
  export WORKSPACE_COMMON=$WORKSPACE/Intel
fi

if [ -z "${WORKSPACE_SILICON}" ]; then
  export WORKSPACE_SILICON=$WORKSPACE/Intel
fi

if [ -z "${WORKSPACE_BINARIES}" ]; then
  export WORKSPACE_BINARIES=$WORKSPACE/Binaries
fi

export EdkSetup=$WORKSPACE_CORE/edksetup.sh
export EDK_TOOLS_PATH=$WORKSPACE_CORE/BaseTools
export PERFORMANCE_ENABLE=FALSE
export SMBIOS_ENABLE=TRUE
export NOTIMESTAMP=
#
# Print usage and exit
#
function USAGE()
{
  echo  "=========================================================================================================================================="
  echo  "= Build defaults:                                                                                                                        ="
  echo  "=   32-bit PEI for FSP                                                                                                                   ="
  echo  "=========================================================================================================================================="
  echo  "=========================================================================================================================================="
  echo  "= Supported platform list:                                                                                                               ="
  echo  "=   All                                                                                                                                  ="
  echo  "=========================================================================================================================================="
  echo  "=========================================================================================================================================="
  echo  "= Supported compiler list:                                                                                                               ="
  echo  "=   GCC                                                                                                                                  ="
  echo  "=   CLANG                                                                                                                                ="
  echo  "=========================================================================================================================================="
  echo  "=========================================================================================================================================="
  echo  "= Usage:                                                                                                                                 ="
  echo  "=   BuildFsp.sh [package name] [platform name ^optional] [Compiler name] [-r | -tr | -d] [-clean] [-header] [chksize <threshold>]        ="
  echo  "=               [fsp64] [-h | -?]                                                                                                        ="
  echo  "=========================================================================================================================================="
  echo  "=========================================================================================================================================="
  echo  "= Example build for LunarLakeFspPkg:                                                                                                     ="
  echo  "=   ./Intel/OneSiliconPkg/Fsp/BuildFsp.sh LunarLake GCC -d                                                                         ="
  echo  "=========================================================================================================================================="
  echo  "= By default BuildFsp assumes below paths when executing:                                                                                ="
  echo  "=   /Edk2/                 : FSP required open source packages                                                                           ="
  echo  "=   /Intel/                : Rest of the FSP required packages                                                                           ="
  echo  "=   /Python27              : Should be installed for supporting FSP build script                                                         ="
  echo  "=   /nasm                  : Should be installed for building FSP                                                                        ="
  echo  "=========================================================================================================================================="
  echo
  exit 1
}

CLEAN_BUILD=FALSE
for ((i=2 ; i <= $# ; i++)); do
  if [ "${!i}" = "notimestamp" ]; then
    export NOTIMESTAMP=1
  elif [ "${!i}" = "chksize" ]; then
    i=`expr $i + 1`
    FV_SPARE_SPACE_THRESHOLD=`expr ${!i} + 0`
    if [ "$FV_SPARE_SPACE_THRESHOLD" != "" ]; then
      if [ $FV_SPARE_SPACE_THRESHOLD -gt 0 ]; then
        export EXT_BUILD_FLAGS="$EXT_BUILD_FLAGS -D FV_SPARE_SPACE_THRESHOLD=$FV_SPARE_SPACE_THRESHOLD"
        continue
      fi
    fi
    echo "Invalid threshold of chksize: \"${!i}\""
    echo
    USAGE
  elif [ "${!i}" = "-clean" ]; then
    CLEAN_BUILD=TRUE
    BUILD_ARGS="$BUILD_ARGS ${!i}"
  elif [ "${!i}" = "perf" ]; then
    export PERFORMANCE_ENABLE=TRUE
  elif [ "${!i}" = "skipsmbiosgen" ]; then
    export SMBIOS_ENABLE=FALSE
  else
    BUILD_ARGS="$BUILD_ARGS ${!i}"
  fi
done

echo "BuildFsp.sh EXT_BUILD_FLAGS: $EXT_BUILD_FLAGS"

if [ $1 = "-?" ]; then
  USAGE
fi
if [ $1 = "-h" ]; then
  USAGE
fi

if [ ! $GCC_BIN ] && [ -d /usr/local/gcc-14/bin ]; then
  export GCC_BIN=/usr/local/gcc-14/bin/
  echo "GCC_BIN becomes $GCC_BIN"
fi

if [ "$CLEAN_BUILD" = "FALSE" ]; then
  if [ ! -d $WORKSPACE/Conf ]
  then
     mkdir $WORKSPACE/Conf
  fi
  . $EdkSetup BaseTools
fi

FSP_PKG_NAME=$1FspPkg

#
# Is FSP package exist
#
if [ -e $WORKSPACE_SILICON/$FSP_PKG_NAME/BuildFv.sh ]; then
  echo  "The current FSP Package is : $FSP_PKG_NAME"
else
  echo
  echo  "Unsupported FSP Package !!"
  echo
  USAGE
  exit 1
fi

cd $WORKSPACE_SILICON/$FSP_PKG_NAME
echo BUILD_ARGS $BUILD_ARGS
echo EXT_BUILD_FLAGS $EXT_BUILD_FLAGS
bash ./BuildFv.sh $BUILD_ARGS
if [ $? -ne 0 ]
then
 exit 1
fi

