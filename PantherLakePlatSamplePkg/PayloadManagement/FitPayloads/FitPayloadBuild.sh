#!/bin/bash

##  @file
#  Generate Fit Payload for PantherLake platform.
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

export PAYLOAD_PREP_FLAGS=
export FITPAYLOADS_FD_NAME=FITPAYLOADS
export INTEGRATE_PAYLOAD_BUILD=FALSE
export FIT_PAYLOAD_RESILIENCY_BUILD=FALSE
export FIT_PAYLOAD_BUILD_OPTION_PCD=
export WCL_FIT_PAYLOAD_BUILD=FALSE
export SI_BUILD_OPTION_PCD=
export TARGET=DEBUG
export PAYLOAD_COMPILER=GCC

function SetEdk2Env {
  #
  # Define common and platform specific environment variables.
  #
  cd ../../../../
  export WORKSPACE=`pwd`
  export WORKSPACE_CORE=$WORKSPACE/Edk2
  export WORKSPACE_CORE_SILICON=$WORKSPACE/Edk2Platforms/Silicon/Intel
  export WORKSPACE_CORE_PLATFORM=$WORKSPACE/Edk2Platforms/Platform/Intel
  export WORKSPACE_CORE_FEATURES=$WORKSPACE/Edk2Platforms/Features/Intel
  export EDK_TOOLS_PATH=$WORKSPACE_CORE/BaseTools
  export PYTHONPATH=$PYTHONPATH:$EDK_TOOLS_PATH/Source/Python
  export WORKSPACE_COMMON=$WORKSPACE/Intel
  export WORKSPACE_SILICON=$WORKSPACE/Intel
  export WORKSPACE_PLATFORM=$WORKSPACE/Intel
  export WORKSPACE_FSP_BIN=$WORKSPACE/Intel
  export WORKSPACE_BINARIES=$WORKSPACE/Binaries
  export EFI_SOURCE=$WORKSPACE_CORE

  export PLATFORM_PACKAGE=PantherLakePlatSamplePkg
  export PLATFORM_BOARD_PACKAGE=PantherLakeBoardPkg
  export PLATFORM_BIN_PACKAGE=PantherLakeBinPkg
}

function SetBuildEnv {
  #
  #  Set basic environment, execute edksetup.sh
  #
  if [ ! -d $WORKSPACE/Conf ]; then
    mkdir $WORKSPACE/Conf
  fi
  . $WORKSPACE_CORE/edksetup.sh BaseTools

  #
  # Build BaseTools
  #
  if [ -d $WORKSPACE_CORE/BaseTools/Source/C/bin ]; then

    echo "$WORKSPACE_CORE/BaseTools/Source/C/bin folder exist then skip build BaseTools"
  else
    echo "No $WORKSPACE_CORE/BaseTools/Source/C/bin folder then build BaseTools"
    make -C $WORKSPACE_CORE/BaseTools/Source/C
  fi
  #
  #  Build Platform Tools
  #
  . $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/BuildPlatTools.sh

  #
  # Update PATH to include tools path.
  #
  export PATH=$WORKSPACE_CORE/BaseTools/Source/C/bin:$PATH


  #
  # Check tool chain.
  #
  if [ "$PAYLOAD_COMPILER" = "GCC" ]; then
    #
    # Detect GCC Tool Chain Version
    #
    GCC_VERSION=$(gcc --version | grep ^gcc | awk '{print $4}' | sed 's/\.[0-9]*$//g')
    if [ "$GCC_VERSION" = "5.4" ]; then
      export TOOL_CHAIN_TAG=GCC
    elif [ "$GCC_VERSION" = "5.5" ]; then
      export TOOL_CHAIN_TAG=GCC
    elif [ "$GCC_VERSION" = "11.1" ]; then
      export TOOL_CHAIN_TAG=GCC
    elif [ "$GCC_VERSION" = "11.2" ]; then
      export TOOL_CHAIN_TAG=GCC
    elif [ "$GCC_VERSION" = "11.3" ]; then
      export TOOL_CHAIN_TAG=GCC
    elif [ "$GCC_VERSION" = "11.4" ]; then
      export TOOL_CHAIN_TAG=GCC
    elif [ "$GCC_VERSION_SKIP_CHECK" != "" ]; then
      export TOOL_CHAIN_TAG=GCC
    else
      echo "Unsupported GCC version!"
      exit 1
    fi
  elif [ "$PAYLOAD_COMPILER" = "CLANG" ]; then
    CLANG_VERSION=$($CLANG_BIN"clang" -dumpversion | sed 's/\.[0-9]*\.[0-9]*$//g')
    if [ "$CLANG_VERSION" = "" ]; then
      echo "Can't find CLANG directory, please assign it by defining CLANG_BIN"
      exit 1
    elif [ "$CLANG_VERSION" -ge "9" ]; then
      export TOOL_CHAIN_TAG=CLANGPDB
    else
      echo "Unsupported CLANG version!"
      exit 1
    fi
  elif [ "$PAYLOAD_COMPILER" = "XCODE" ]; then
    #
    # Always assume the latest XCODE
    #
    export TOOL_CHAIN_TAG=XCODE5
  fi

  BUILD_DIR="Build/FitPayloadsPkg/$TARGET"
  BUILD_DIR+="_$TOOL_CHAIN_TAG"
  export BUILD_DIR
  export EXT_BUILD_FLAGS="-t $TOOL_CHAIN_TAG"

  if [ ! -d $BUILD_DIR ]; then
    mkdir -p $BUILD_DIR
  fi
}

function PayloadBuildClean {
  echo "Build Clean from FitPayloads Start"
  echo

  SetEdk2Env
  cd $WORKSPACE_CORE/BaseTools/Source/C
  make clean
  rm -fr $WORKSPACE/Build
  rm -fr $WORKSPACE/Conf
  rm -fr $WORKSPACE/RomImages
  echo "Build Clean from FitPayloads End"
}

function PayloadBuild {
  ACTIVE_PLATFORM=$WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/PayloadManagement/FitPayloads/FitPayloadsPkg.dsc
  if [ "$FIT_PAYLOAD_RESILIENCY_BUILD" = "FALSE" ]; then
      ACTIVE_MICROCODE_FILE=$WORKSPACE_BINARIES/$PLATFORM_BIN_PACKAGE/Include/Fdf/FvMicrocode.fdf
  else
      ACTIVE_MICROCODE_FILE=$WORKSPACE_BINARIES/$PLATFORM_BIN_PACKAGE/Include/Fdf/FvMicrocodeDbgRes.fdf
  fi

  #
  # Call this script to pad each Microcode patch under $PLATFORM_BIN_PACKAGE/Binaries/Microcode
  # And also create MicrocodeVersion.data
  #
  python3 $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/PayloadManagement/FitPayloads/microcode_padding.py \
    --opt padding \
    --fw-version $FW_VERSION \
    --lsv $LSV \
    --fw-version-string $FW_VERSION_STRING \
    --slotsize $SLOT_SIZE \
    --fdf $ACTIVE_MICROCODE_FILE

  if [ $? -ne 0 ]; then
    echo "!!! ERROR: microcode_padding.py execute failure !!!"
    exit 1
  fi

  export FIT_PAYLOAD_BUILD_FLAGS="-p $ACTIVE_PLATFORM -b $TARGET $EXT_BUILD_FLAGS $FIT_PAYLOAD_BUILD_OPTION_PCD $SI_BUILD_OPTION_PCD"
  build $FIT_PAYLOAD_BUILD_FLAGS
  if [ $? -ne 0 ]; then
    echo "!!! Error when generating FIT Payload !!!"
    exit 1
  fi

  python3 $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/PayloadManagement/FitPayloads/microcode_padding.py --opt revert --fdf $ACTIVE_MICROCODE_FILE
  cd $PAYLOAD_WORKSPACE
}

#
# Parse for "cln" in argument to do a build clean
#
for ((i=1 ; i <= $# ; i++)); do
  if [ "${!i}" = "cln" ]; then
    PayloadBuildClean
    exit 0
  fi
done

numargs=$#
  for ((i=1 ; i <= numargs ; i++)); do
  if [ "$1" = "IntegrateBuild" ]; then
      INTEGRATE_PAYLOAD_BUILD=TRUE
  elif [ "$1" = "res" ]; then
    FIT_PAYLOAD_RESILIENCY_BUILD=TRUE
    export FIT_PAYLOAD_BUILD_OPTION_PCD="$FIT_PAYLOAD_BUILD_OPTION_PCD --pcd gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosResiliencyEnable=TRUE"
  elif [ "$1" = "r" ]; then
     export TARGET=RELEASE
  elif [ "$1" = "gcc" ]; then
    export PAYLOAD_COMPILER=GCC
  elif [ "$1" = "clang" ]; then
    export PAYLOAD_COMPILER=CLANG
  else
      echo "Invalid input arguments: $1"
      exit 1
  fi
  shift
done

# PrepareEdk2Env
if [ "$INTEGRATE_PAYLOAD_BUILD" = "TRUE" ]; then
  echo "Start payload build..."
  PayloadBuild
else
  export PAYLOAD_WORKSPACE=`pwd`
  export SLOT_SIZE=0x37000
  export FW_VERSION=0x0001
  export LSV=0x0001
  export FW_VERSION_STRING="Version 0.0.0.1"
  export TARGET_PLATFORM=PantherLake
  export TOOL_CHAIN_TAG=GCC

  SetEdk2Env
  SetBuildEnv
  PayloadBuild
    if [ ! -d $WORKSPACE/RomImages ]; then
    mkdir $WORKSPACE/RomImages
  fi
  cp $WORKSPACE/$BUILD_DIR/FV/FITPAYLOADS.fd $WORKSPACE/RomImages/FITPAYLOADS.fd
fi
