#!/bin/bash

## @file
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2016 Intel Corporation.
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
# Define common and platform specific environment variables.
#
cd ../../
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
export PLATFORM_SI_PACKAGE=OneSiliconPkg
export PLATFORM_BIN_PACKAGE=PantherLakeBinPkg
export FSP_PKG_NAME=PantherLakeFspPkg
export NOTIMESTAMP=

export PTL_BUILD=TRUE
export PrepRelease=DEBUG
export FSPM_COMPRESSED=FALSE
export SILENT_MODE=FALSE
CapsuleBuild=FALSE
export COMPILER=GCC
export TARGET_PLATFORM=PantherLake
export FspTargetOption=PantherLake
export TARGET_PLATFORM_SHORT=PTL
#
# Default to 64-bit PEI for FSP.
#
export FSP_ARCH=X64
export FSP64_BUILD=TRUE
export FSP_RESET=TRUE
export FSP_MODE=Dispatch
export SYMBOL_PREFIX=
export PCH_TYPE=P
export UNIVERSAL_PAYLOAD=TRUE
export UPL_SPEC_REVISION=0.9
export ROM_FILENAME_SPECIAL_BUILD_TYPE=

#
# If NUMBER_OF_PROCESSORS environment variable is uninitialized
#
if [ ! $NUMBER_OF_PROCESSORS ]; then
  if [ "$(uname)" = "Darwin" ]; then
    export NUMBER_OF_PROCESSORS=$(sysctl -n hw.ncpu)
  else
    export NUMBER_OF_PROCESSORS=$(nproc)
  fi
fi
#
# VAR_BUILD_FLAGS contains user build flags, it is not modified by build scripts
#
export EXT_BUILD_FLAGS=$VAR_BUILD_FLAGS
export BUILD_OPTION_PCD="--pcd gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperResetVectorInFsp=TRUE"
export FSP_BUILD_OPTION_PCD

export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gPcAtChipsetPkgTokenSpaceGuid.PcdMinimalValidYear=$(date +'%Y')"

function PrintUsage {
  echo "Client BIOS build script"
  echo
  echo "$0 [-f FLAG VALUE] [/f FLAG VALUE] [/s] [-s] [r] [s] [c] [rc] [gcc] [clang] [xcode] [fsp64] [cln]"
  echo
  echo "  Default build flags: build in Debug Mode; 32-bit PEI for FSP; FSP Dispatch Mode"
  echo
  echo "  r        Build in Release mode."
  echo "  s        Build in Silent mode."
  echo "  c        Build with CapsuleBuild=TRUE (Debug build). See note 1"
  echo "  rc       Build with CapsuleBuild=TRUE (Release build). See note 1"
  echo "  gcc      Build whole source with GCC. See note 1"
  echo "  clang    Build whole source with CLANG. See note 1"
  echo "  xcode    Build whole source with XCODE. See note 1"
  echo "  ptlp     To do build ptlp debug build"
  echo "  fspapi   To build using FSP API Mode."
  echo "  fsp32    To build using 32-bit PEI for FSP."
  echo "  fsp64    To build using 64-bit PEI for FSP."
  echo "  cln      Build clean."
  echo "  notimestamp To eliminate the effect of timestamp."
  echo "  chksize threshold To enable FV_SPARE_SPACE_THRESHOLD check."
  echo "           BaseTools will check every FV with the threshold."
  echo "           Build tool will report error message to say there is no enough spare space if check fail."
  echo "  non_upl   To build Non-Upl"
  echo "  -f or /f Sets a build flag to the given value,"
  echo "           can be used as many times as desired.  See note 1"
  echo "  -s or /s Useful for faster rebuilds when no changes have been made to .inf files,"
  echo "           skips the generation of makefiles."
  echo
  echo "1) Re-running $0 without these arguments cannot be used for"
  echo "   incremental build. Hence, these inputs must be supplied every time"
  echo "   $0 is run.  For consistent build flags, set VAR_BUILD_FLAGS"
  echo "   prior to $0"
  echo
}

function BuildClean {
  echo "Build Clean from BoardPkg Start"
  echo
  . $WORKSPACE_COMMON/$PLATFORM_SI_PACKAGE/Fsp/BuildFsp.sh $TARGET_PLATFORM -clean
  cd $WORKSPACE_CORE/BaseTools/Source/C
  make clean
  rm -fr $WORKSPACE/parser.out
  rm -fr $WORKSPACE/parsetab.py
  rm -fr $WORKSPACE_CORE/BaseTools/Source/C/GenBiosId
  rm -fr $WORKSPACE_CORE/BaseTools/Source/C/KeyEnroll
  rm -fr $WORKSPACE_CORE/BaseTools/BinWrappers/PosixLike/GenBiosId
  rm -fr $WORKSPACE_CORE/BaseTools/BinWrappers/PosixLike/KeyEnroll
  rm -fr $WORKSPACE/Build
  rm -fr $WORKSPACE/Conf
  rm -fr $WORKSPACE/RomImages
  echo "Build Clean from BoardPkg End"
}

#
# Parse for "cln" in argument to do a build clean
#
for ((i=1 ; i <= $# ; i++)); do
  if [ "${!i}" = "cln" ]; then
    BuildClean
    exit 0
  fi
done

#
#  Set basic environment, execute edksetup.sh
#
if [ ! -d $WORKSPACE/Conf ]; then
   mkdir $WORKSPACE/Conf
fi

if [ ! $GCC_BIN ] && [ -d /usr/local/gcc-14/bin ]; then
  export GCC_BIN=/usr/local/gcc-14/bin/
  echo "GCC_BIN becomes $GCC_BIN"
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
# Add $WORKSPACE_CORE_SILICON, $WORKSPACE_CORE_PLATFORM, WORKSPACE_CORE_FEATURES,
# $WORKSPACE_BINARIES
# $WORKSPACE_CORE and WORKSPACE_PLATFORM in PACKAGES_PATH
#
export PACKAGES_PATH=$(python3 $WORKSPACE_CORE_PLATFORM/Tools/AppendPackagesPath/GetPackagesPath.py \
        $WORKSPACE_CORE_SILICON $WORKSPACE_CORE_PLATFORM $WORKSPACE_CORE_FEATURES \
        $WORKSPACE_CORE $WORKSPACE/Intel \
        $WORKSPACE_BINARIES \
)
if [ "$PACKAGES_PATH" = "" ]; then
  echo "Error: Failed to set PACKAGES_PATH"
  exit 1
fi

cd $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE
# @todo: Add support for FSP build options (fspb, fspw) and pacm
numargs=$#
for ((i=1 ; i <= numargs ; i++)); do
  if [ "$1" = "r" ]; then
    PrepRelease=RELEASE
  elif [ "$1" = "s" ]; then
    export SILENT_MODE=TRUE
  elif [ "$1" = "c" ]; then
    CapsuleBuild=TRUE
  elif [ "$1" = "rc" ]; then
    PrepRelease=RELEASE
    CapsuleBuild=TRUE
  elif [ "$1" = "gcc" ]; then
    export COMPILER=GCC
  elif [ "$1" = "clang" ]; then
    export COMPILER=CLANG
  elif [ "$1" = "fspapi" ]; then
    export FSP_MODE=API
    export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection=1"
  elif [ "$1" = "non_upl" ]; then
    export UNIVERSAL_PAYLOAD=FALSE
    export ROM_FILENAME_SPECIAL_BUILD_TYPE=_Non_UPL
  elif [ "$1" = "fsp32" ]; then
    export FSP_ARCH=IA32
    export FSP64_BUILD=FALSE
    export FSP_RESET=FALSE
    export SYMBOL_PREFIX=_
    export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperResetVectorInFsp=FALSE"
  elif [ "$1" = "fsp64" ]; then
    export FSP_ARCH=X64
    export FSP64_BUILD=TRUE
    export FSP_RESET=TRUE
    export SYMBOL_PREFIX=
    export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperResetVectorInFsp=TRUE"
  elif [ "$1" = "fspmuncompressed" ]; then
    export FSPM_COMPRESSED=FALSE
    export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gSiPkgTokenSpaceGuid.PcdEnableFspmCompression=FALSE"
    export FSP_BUILD_OPTION_PCD="$FSP_BUILD_OPTION_PCD --pcd gSiPkgTokenSpaceGuid.PcdSecondaryDataStackSize=0x0"
  elif [ "$1" = "ptlp" ]; then
    export PTL_BUILD=TRUE
    export PCH_TYPE=P

  elif [ "$1" = "xcode" ]; then
    export COMPILER=XCODE
  elif [ "$1" = "MultiIbb" ]; then
    export ROM_FILENAME_SPECIAL_BUILD_TYPE=_MultiIbb
    export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbFeatureEnable=TRUE"
    export FLASHMAP_FDF=$WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Include/Fdf/FlashMapIncludeMultiIbb.fdf
  elif [ "$1" = "CacheMissDetect" ]; then
    export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gMultiIbbFeaturePkgTokenSpaceGuid.PcdCacheMissDetectEnable=TRUE"

    #
    # Workaround for boot issue - get assertion at "SystemFirmwareDescriptor.efi".
    #
    export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable=FALSE"
  elif [ "$1" = "perf" ]; then
    export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable=TRUE"
    export FSP_BUILD_OPTION_PCD="$FSP_BUILD_OPTION_PCD --pcd gPantherLakeFspPkgTokenSpaceGuid.PcdFspPerformanceEnable=TRUE"
    export BUILD=P
    export ROM_FILENAME_SPECIAL_BUILD_TYPE=_Performance
  elif [ "$1" = "embedded" ]; then
    export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gSiPkgTokenSpaceGuid.PcdEmbeddedEnable=0x1"
    export EMBEDDED_BUILD=TRUE
    export BIOS_ID_FILE=$BUILD_DIR/BiosIdEmb.env
    export ROM_FILENAME_SPECIAL_BUILD_TYPE=_Embedded
  elif [ "$1" = "chksize" ]; then
    FV_SPARE_SPACE_THRESHOLD=`expr $2 + 0`
    if [ "$FV_SPARE_SPACE_THRESHOLD" != "" ]; then
      if [ $FV_SPARE_SPACE_THRESHOLD -gt 0 ]; then
        export EXT_BUILD_FLAGS="$EXT_BUILD_FLAGS -D FV_SPARE_SPACE_THRESHOLD=$FV_SPARE_SPACE_THRESHOLD"
        shift 2
        continue
      fi
    fi
    echo "Invalid threshold of chksize: \"$2\""
    echo
    PrintUsage
    exit 1
  elif [ "$1" = "help" ] || [ "$1" = "--help" ] || [ "$1" = "-h" ] || [ "$1" = "/h" ] || [ "$1" = "-?" ] || [ "$1" = "/?" ]; then
    PrintUsage
    exit 0
  elif [ "$1" = "/f" ] || [ "$1" = "-f" ]; then
    if [ -z "$2" ] || [ -z "$3" ]; then
      PrintUsage
      exit 1
    fi
  elif [ "$1" = "/s" ] || [ "$1" = "-s" ]; then
    export EXT_BUILD_FLAGS="$EXT_BUILD_FLAGS -u"
  elif [ -n "$1" ]; then
    echo "Invalid input argument: $1"
    echo
    PrintUsage
    exit 1
  elif [ "$1" = "notimestamp" ]; then
    export NOTIMESTAMP=1
  fi
  shift
done

#
# Set EXT_BUILD_FLAGS with FSP env vars after checking cmdline parameters to determine 32-bit or 64-bit.
#
export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gBoardModuleTokenSpaceGuid.PcdUplEnable=$UNIVERSAL_PAYLOAD"
export EXT_BUILD_FLAGS="$EXT_BUILD_FLAGS -D FSP_ARCH=$FSP_ARCH -D FSP64_BUILD=$FSP64_BUILD"
echo "BuildIafw.sh EXT_BUILD_FLAGS: $EXT_BUILD_FLAGS"

if [ -f Prep.log ]; then
  rm Prep.log
fi

#
# Setup basic environment
#

cd $WORKSPACE

# Align BOOT_STAGE=6 behavior with prep & bld file
export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable=TRUE"
export BUILD_OPTION_PCD="$BUILD_OPTION_PCD --pcd gPantherLakeBinPkgTokenSpaceGuid.PcdStartupAcmBinEnable=TRUE"

#
# Check tool chain.
#
if [ "$COMPILER" = "GCC" ]; then
  #
  # Detect GCC Tool Chain Version
  #
  if [ $GCC_BIN ]; then
    GCC_VERSION=$($GCC_BIN/gcc --version | grep ^gcc | sed -e "s/.*) //" | sed 's/\([0-9]\+.[0-9]\).*/\1/')
  else
    GCC_VERSION=$(gcc --version | grep ^gcc | sed -e "s/.*) //" | sed 's/\([0-9]\+.[0-9]\).*/\1/')
  fi
  echo "GCC version: $GCC_VERSION!"
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
  elif [ "$GCC_VERSION" = "14.1" ]; then
    export TOOL_CHAIN_TAG=GCC
  elif [ "$GCC_VERSION" = "14.2" ]; then
    export TOOL_CHAIN_TAG=GCC
  elif [ "$GCC_VERSION_SKIP_CHECK" != "" ]; then
    export TOOL_CHAIN_TAG=GCC
  else
    echo "Unsupported GCC version!"
    exit 1
  fi
elif [ "$COMPILER" = "CLANG" ]; then
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
elif [ "$COMPILER" = "XCODE" ]; then
  #
  # Always assume the latest XCODE
  #
  export TOOL_CHAIN_TAG=XCODE5
fi

#
# Set build TARGET
#
export TARGET_ARCH="$FSP_ARCH X64"

if [ "$PrepRelease" = "DEBUG" ]; then
  export TARGET=DEBUG
  export TARGET_SHORT=D
  export FSP_BUILD_PARAMETER=-d
elif [ "$PrepRelease" = "RELEASE" ]; then
  export TARGET=RELEASE
  export TARGET_SHORT=R
  export FSP_BUILD_PARAMETER=-r
else
  echo
  echo "!!! ERROR !!! Incorrect TARGET option !!!"
  echo
  exit 1
fi

echo "BuildIafw.sh $FSP_ARCH $FSP64_BUILD $SYMBOL_PREFIX"
. $WORKSPACE_COMMON/$PLATFORM_SI_PACKAGE/Fsp/BuildFsp.sh $TARGET_PLATFORM $FspTargetOption $COMPILER $FSP_BUILD_PARAMETER
if [ $? -ne 0 ]; then
  echo "!!! ERROR:FSP build Failed !!!"
  exit 1
fi


  export FLASHMAP_FDF=$WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Include/Fdf/FlashMapInclude.fdf

# Split Fsp.fd and generate FspTopAt4G.fd, then add padding data before this binary to FspTopAt4G.fd for alignment
if [ "$FSP_RESET" = "TRUE" ]; then
  echo "call FspTopGen.py to generate FspTopAt4G.fd."
  python3 $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/BoardSupport/Tools/FspTopGen/FspTopGen.py -SplitBin $EDK_TOOLS_PATH/Source/Python/Split/Split.py
  if [ $? -ne 0 ]; then
    echo "!!! ERROR:FspTopAt4G.fd build Failed !!!"
    exit 1
  fi
fi


python3 $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/StripFlashmap.py "$TARGET" $FLASHMAP_FDF $WORKSPACE/FlashMapInclude_Temp.fdf
python3 $WORKSPACE_CORE_PLATFORM/MinPlatformPkg/Tools/Fsp/RebaseFspBinBaseAddress.py $WORKSPACE/FlashMapInclude_Temp.fdf $WORKSPACE_FSP_BIN/PantherLakeFspBinPkg Fsp.fd 0x0 $WORKSPACE_CORE/IntelFsp2Pkg/Tools/SplitFspBin.py

if [ $? -ne 0 ]
then
  echo "!!! ERROR:RebaseFspBinAddress failed!!!"
  exit 1
fi
if [ "$FSPM_COMPRESSED" = "TRUE" ]; then
  @echo "FSP-M is compressed, Rebase FSP-M"
  python3 $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/RebaseFspmBinBaseAddress.py $WORKSPACE_FSP_BIN/PantherLakeFspBinPkg Fsp_Rebased.fd $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/BoardPkgPcdUpdate.dsc $WORKSPACE_CORE/IntelFsp2Pkg/Tools/SplitFspBin.py
fi
if [ $? -ne 0 ]
then
  echo "!!! ERROR:RebaseFspmBinAddress failed!!!"
  exit 1
fi

BUILD_DIR="Build/$PLATFORM_BOARD_PACKAGE/$TARGET"
BUILD_DIR+="_$TOOL_CHAIN_TAG"
export BUILD_DIR
export BUILD_X64=$BUILD_DIR/X64
export BUILD_IA32=$BUILD_DIR/IA32
export BIOS_ID_FILE=$BUILD_DIR/BiosId.env

export PPL_SETUP_FV_LIST=$BUILD_DIR/FV/FVSETUPDATA.Fv

if [ "$CapsuleBuild" = "FALSE" ]; then
  export INTERNAL_BUILD=0

  #
  # Determine BIOS Build Type
  #
  # R - RVP/Restricted, N - Non-Restricted, I - Internal, P - Performance, S - SV, E - SLE
  #

  export BUILD=X
  echo $EXT_BUILD_FLAGS | grep "PERFORMANCE_ENABLE=TRUE"
  if [ $? -eq 0 ]; then
    export BUILD=P
  fi

  export EXT_BUILD_FLAGS="$EXT_BUILD_FLAGS $BIOS_SIZE_OPTION"
  echo
  echo "printing the build type ([U]nified, [N]on-Restricted, [I]nternal, [P]erformance, [S]V, SL[E])"
  echo "Build type ($BUILD)"
  echo

fi

cd $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE

if [ "$SILENT_MODE" = "TRUE" ]; then
  echo "************************************************************************" > Prep.log
  echo "***********           PreBuild.sh is launched here          ***********" >> Prep.log
  echo "************************************************************************" >> Prep.log
  . PreBuild.sh $PrepRelease $CapsuleBuild >> Prep.log 2>&1
else
  . PreBuild.sh $PrepRelease $CapsuleBuild
fi

if [ $? -ne 0 ]; then
  echo
  echo "!! The EDKII BIOS build has failed in PreBuild.sh!"
  echo
  exit 1
fi

#
# Run the primary build phase
#
. PriBuild.sh
ret=$?
if [ $ret -ne 0 ]; then
  exit $ret
fi
