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
# Performs the primary build
#

function BuildFail {
  cd $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE
  echo
  echo "The EDKII BIOS Build has failed!"
  echo
  exit $1
}

#
# Definition for BtGACM Support.
#
export BTG_ACM_BASE_PATH=$WORKSPACE_BINARIES/$PLATFORM_BIN_PACKAGE/Binaries/BootGuard
export BTG_ACM_OUTPUT_PATH=$WORKSPACE/$BUILD_DIR/BootGuard
export BTG_ACM_SLOT_SIZE=0x48000

python3 $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/BtgAcmMisc/BtgAcmMiscScript.py \
  Padding \
  -I $BTG_ACM_BASE_PATH \
  -S $BTG_ACM_SLOT_SIZE \
  -O $BTG_ACM_OUTPUT_PATH

if [ $ret -ne 0 ]; then
  echo "!!! ERROR: Failed to do the padding operation on BtGACM !!!"
  BuildFail $ret
fi

cd $WORKSPACE

#
# Run the actual build
#
if [ "$SILENT_MODE" = "TRUE" ]; then
  echo > Build.log
  echo "************************************************************************" >> Build.log
  echo "***********            build.sh is launched here             ***********" >> Build.log
  echo "************************************************************************" >> Build.log
  build -n $MAX_CONCURRENT_THREADS $EXT_BUILD_FLAGS $BUILD_OPTION_PCD $SI_BUILD_OPTION_PCD >> Build.log 2>&1
  ret=$?
  if [ $ret -ne 0 ]; then
    BuildFail $ret
  fi
  echo >> Build.log
  echo "Running PostBuild.sh to complete the build process." >> Build.log
  echo >> Build.log
  . $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/PostBuild.sh >> Build.log 2>&1
  ret=$?
  if [ $ret -ne 0 ]; then
    BuildFail $ret
  fi
  if [ -f EDK2.log ]; then
    rm EDK2.log
  fi
  if [ -f Prep.log ] && [ -f Build.log ]; then
    cat Prep.log  >  EDK2.log
    cat Build.log >> EDK2.log
  fi
else
  if [ "$UNIVERSAL_PAYLOAD" = "TRUE" ]; then
    export PPL_SETUP_FV_LIST="$PPL_SETUP_FV_LIST $BUILD_DIR/FV/FVPPLBDSUNCOMPACT.Fv"

    # Set DXE NX MEMORY PROTECTION POLICY
    if [[ "$SPECIAL_POOL_BUILD" == "TRUE" ]]; then
      export PCD_DXE_NX_MEMORY_PROTECTION_POLICY=0x7BD4
    else
      export PCD_DXE_NX_MEMORY_PROTECTION_POLICY=0x0
    fi

    python3 $WORKSPACE_CORE/UefiPayloadPkg/UniversalPayloadBuild.py \
    -t $TOOL_CHAIN_TAG \
    -s $UPL_SPEC_REVISION \
    -b $PrepRelease \
    -D UNIVERSAL_PAYLOAD=TRUE \
    -D DISABLE_RESET_SYSTEM=TRUE \
    -D NETWORK_DRIVER_ENABLE=TRUE \
    -D PS2_KEYBOARD_ENABLE=TRUE \
    -D SERIAL_DRIVER_ENABLE=FALSE \
    -D VARIABLE_SUPPORT=None \
    -D SECURITY_STUB_ENABLE=FALSE \
    -D CRYPTO_PROTOCOL_SUPPORT=TRUE \
    -D CRYPTO_DRIVER_EXTERNAL_SUPPORT=TRUE \
    -D NVME_ENABLE=FALSE \
    -D MEMORY_TEST=GENERIC \
    -D PERFORMANCE_MEASUREMENT_ENABLE=TRUE \
    -D MULTIPLE_DEBUG_PORT_SUPPORT=TRUE \
    -D CAPSULE_SUPPORT=TRUE \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdPs2KbdExtendedVerification=FALSE \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport=FALSE \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdPciDisableBusEnumeration=FALSE \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport=FALSE \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserSubtitleTextColor=0x0 \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserGrayOutTextStatement=TRUE \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserFieldTextColor=0x01 \
    -p gUefiCpuPkgTokenSpaceGuid.PcdCpuApLoopMode=0x2 \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdConOutUgaSupport=TRUE \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset=TRUE \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosEntryPointProvideMethod=0x01 \
    -p gPcAtChipsetPkgTokenSpaceGuid.PcdMinimalValidYear=$(date +'%Y') \
    -p gUefiCpuPkgTokenSpaceGuid.PcdCpuInitIpiDelayInMicroSeconds=100 \
    -p gEfiMdeModulePkgTokenSpaceGuid.PcdDxeNxMemoryProtectionPolicy=$PCD_DXE_NX_MEMORY_PROTECTION_POLICY
    ret=$?
    if [ $ret -ne 0 ]; then
      BuildFail $ret
    fi
    export UPL_BUILD_OPTION_PCD=$(python3 $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Features/Upl/Tools/GetUplVerFromBin.py \
      --UplBinFile $WORKSPACE/Build/UefiPayloadPkgX64/UniversalPayload.elf
    )
    ret=$?
    if [ $ret -ne 0 ]; then
      BuildFail $ret
    fi
    export UPL_BUILD_OPTION_PCD=$(echo $UPL_BUILD_OPTION_PCD | sed -e "s/gBoardModule/--pcd gBoardModule/g")
    export BUILD_OPTION_PCD="$BUILD_OPTION_PCD $(echo $UPL_BUILD_OPTION_PCD | sed -e "s/\"//g")"
  fi
  export EXT_BUILD_FLAGS="$EXT_BUILD_FLAGS -y $WORKSPACE/Build/$LATFORM_BOARD_PACKAGE/BoardPkgReport.log"
  build -n $MAX_CONCURRENT_THREADS $EXT_BUILD_FLAGS $BUILD_OPTION_PCD $SI_BUILD_OPTION_PCD
  ret=$?
  if [ $ret -ne 0 ]; then
    BuildFail $ret
  fi
  if [ "$UNIVERSAL_PAYLOAD" = "TRUE" ]; then
    python3 $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Features/Upl/Tools/PcdCheck.py
    if [ $ret -ne 0 ]; then
      BuildFail $ret
    fi
  fi
  echo
  echo "Running PostBuild.sh to complete the build process."
  echo
  . $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/PostBuild.sh
  ret=$?
  if [ $ret -ne 0 ]; then
    BuildFail $ret
  fi
fi

echo
echo "TARGET:               $TARGET"
echo "TOOL_CHAIN_TAG:       $TOOL_CHAIN_TAG"
echo "BIOS location:        $BUILD_DIR/FV"
echo "SPI Images location:  $WORKSPACE/RomImages"
echo
echo "The EDKII BIOS build has successfully completed!"
echo
