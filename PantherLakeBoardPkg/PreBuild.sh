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

function EndPreBuild {
  cd $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE
}

cd $WORKSPACE

# Check $FSP_BINARY_BUILD and set $FSP_TEST_RELEASE

export FSP_TEST_RELEASE=FALSE
if [ "$FSP_BINARY_BUILD" == "TRUE" ]; then
  if [ "$1" == "TEST_RELEASE" ]; then
    export FSP_TEST_RELEASE=TRUE
  fi
  echo "FSP BINARY_BUILD = $FSP_BINARY_BUILD"
  echo "FSP_TEST_RELEASE = $FSP_TEST_RELEASE"

  if [ "$FSP_BINARY_BUILD_ONLY" == "TRUE" ]; then
    exit 0
  fi

  echo "FSP_BINARY_BUILD = $FSP_BINARY_BUILD"
  echo "FSP_TEST_RELEASE = $FSP_TEST_RELEASE"
  echo "RESILIENCY_BUILD = $RESILIENCY_BUILD"
fi

echo
echo "Prebuild:  Set build environment."
echo

if [ ! -d $BUILD_DIR ]; then
  mkdir -p $BUILD_DIR
fi

grep -v 'ACTIVE_PLATFORM\|TARGET\|TARGET_ARCH\|TOOL_CHAIN_TAG\|BUILD_RULE_CONF' Conf/target.txt > $BUILD_DIR/target.txt
echo "ACTIVE_PLATFORM   = $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/BoardPkg.dsc" >> $BUILD_DIR/target.txt
echo "TARGET            = $TARGET"                                                  >> $BUILD_DIR/target.txt
echo "TARGET_ARCH       = $TARGET_ARCH"                                             >> $BUILD_DIR/target.txt
echo "TOOL_CHAIN_TAG    = $TOOL_CHAIN_TAG"                                          >> $BUILD_DIR/target.txt
echo "BUILD_RULE_CONF   = $WORKSPACE/Conf/build_rule.txt"                           >> $BUILD_DIR/target.txt
mv -f $BUILD_DIR/target.txt $WORKSPACE/Conf

echo
echo "Prebuild:  Create BiosId"
echo
if [ ! -d $BUILD_X64 ]; then
  mkdir -p $BUILD_X64
fi
#
# @todo: Code Coverage not supported at this time
#


#
# Create BiosId.env based on the build type
#
BIOSID_FILENAME=BiosId

cp -f $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/$BIOSID_FILENAME.env $BIOS_ID_FILE

BOARD_ID_STRING="BOARD_ID      = ${TARGET_PLATFORM_SHORT}$PCH_TYPE"
BOARD_ID_STRING+="FW$BUILD"
echo "$BOARD_ID_STRING" >> $BIOS_ID_FILE

BOARD_EXT_STRING="BOARD_EXT    = $TARGET_SHORT"
BOARD_EXT_STRING+="00"
echo "$BOARD_EXT_STRING" >> $BIOS_ID_FILE
BIOS_ID_OUTPUT_DIR=$BUILD_IA32
if [ ! -d $BIOS_ID_OUTPUT_DIR ]; then
  mkdir -p $BIOS_ID_OUTPUT_DIR
fi
if [ "$NOTIMESTAMP" = "1" ];then
  export NoTime=-nt
else
  export NoTime=
fi
python3 $WORKSPACE_CORE_PLATFORM/Tools/GenBiosId/GenBiosId.py -i $BIOS_ID_FILE -o $BIOS_ID_OUTPUT_DIR/BiosId.bin $NoTime
ret=$?
if [ $ret -ne 0 ]; then
  EndPreBuild
  exit $ret
fi

#
# Create dummy ClientBios.fd file to pass the first build. In post build, the real image will be generated
#
if [ ! -d $BUILD_DIR/FV ]; then
  mkdir -p $BUILD_DIR/FV
fi
echo 2> $BUILD_DIR/FV/CLIENTBIOS.fd

echo
echo "Prebuild is complete."
echo "  ACTIVE_PLATFORM      = $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/BoardPkg.dsc"
echo "  EFI_SOURCE           = $EFI_SOURCE"
echo "  TARGET               = $TARGET"
echo "  TARGET_ARCH          = $TARGET_ARCH"
echo "  TOOL_CHAIN_TAG       = $TOOL_CHAIN_TAG"
echo "  WORKSPACE            = $WORKSPACE"
echo "  EXT_BUILD_FLAGS      = $EXT_BUILD_FLAGS"
echo "  PERFORMANCE_BUILD    = $PERFORMANCE_BUILD"
echo "  RPMC_BUILD           = $RPMC_BUILD"
echo "  FSPV_BUILD           = $FSPV_BUILD"
echo "  SPECIAL_POOL_BUILD   = $SPECIAL_POOL_BUILD"
echo "  EXTENDEDREGION_BUILD = $EXTENDEDREGION_BUILD"
echo "  ATOM_BUILD           = $ATOM_BUILD"
echo "  FSP_SIGNED           = $FSP_SIGNED"
echo "  BUILD_OPTION_PCD     = $BUILD_OPTION_PCD"
echo "  FSP_BINARY_BUILD     = $FSP_BINARY_BUILD"
echo "  FSP_TEST_RELEASE     = $FSP_TEST_RELEASE"
echo "  FSPB_BUILD           = $FSPB_BUILD"
echo "  FSPW_BUILD           = $FSPW_BUILD"
echo
EndPreBuild
