## @file
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2008 Intel Corporation.
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
#!/bin/sh

FSP_PKG_NAME=PantherLakeFspPkg
FSP_BIN_PKG_NAME=PantherLakeFspBinPkg
PLATFORM_SI_PACKAGE=OneSiliconPkg
FSP_T_UPD_GUID=34686CA3-34F9-4901-B82A-BA630F0714C6
FSP_M_UPD_GUID=39A250DB-E465-4DD1-A2AC-E2BD3C0E2385
FSP_S_UPD_GUID=CAE3605B-5B34-4C85-B3D7-27D54273C40F
ErrorCode=0

function USAGE()
{
  echo
  echo  "$0 \[PantherLake\(Optional\)\] \[GCC \| CLANG\]\[-h \| -? \| -r \| -tr \| -d \| -clean\] \[-header\]"
  echo
  return 1
}

function Clean(){
  echo "Build Clean from FspPkg Start"
  if [ -d $WORKSPACE/Build ]; then
    rm -r $WORKSPACE/Build
  fi
  if [ -d $WORKSPACE/Conf ]; then
    rm  -r $WORKSPACE/Conf
  fi
  if [ -f *.log ]; then
    rm *.log
  fi
  rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/*.fd
  if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/*.bsf ]; then
    rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/*.bsf
  fi
  if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FirmwareVersionInfo.h ]; then
    rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FirmwareVersionInfo.h
  fi
  for UPD_HEADER in $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/Fsp*Upd.h ; do
    rm -f $UPD_HEADER
  done
  if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/HobUsageDataHob.h ]; then
    rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/HobUsageDataHob.h
  fi
  if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/MemInfoHob.h ]; then
    rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/MemInfoHob.h
  fi
  if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/SmbiosCacheInfoHob.h ]; then
    rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/SmbiosCacheInfoHob.h
  fi
  if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/SmbiosProcessorInfoHob.h ]; then
    rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/SmbiosProcessorInfoHob.h
  fi
  if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/FspPkgPcdShare.dsc ]; then
    rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/FspPkgPcdShare.dsc
  fi
  if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FspProducerDataHeader.h ]; then
    rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FspProducerDataHeader.h
  fi
  for UPD_HEADER in $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/Fsp*Upd.h ; do
    rm -f $UPD_HEADER
  done
  if [ -f $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/MemInfoHob.h ]; then
    rm -f $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/MemInfoHob.h
  fi
  if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/GpioV2Config.h ]; then
    rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/GpioV2Config.h
  fi
  if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/GpioV2Pad.h ]; then
    rm -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/GpioV2Pad.h
  fi
  WORKSPACE=
  EDK_TOOLS_PATH=
  echo "Build Clean from FspPkg End"
  return 0
}

function  PreBuildFail(){
  if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspUpd.h ]
   then
    rm  $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspUpd.h
  fi
  if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FsptUpd.h ]
   then
    rm  $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FsptUpd.h
  fi
  if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspmUpd.h ]
   then
    rm  $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspmUpd.h
  fi
  if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspsUpd.h ]
   then
    rm  $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspsUpd.h
  fi
  #/q /f
  return 0
}

function PreBuild(){
  if [ ! -d $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include ]; then
    mkdir $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include
  fi

  cp $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Include/GpioV2Config.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/GpioV2Config.h

  cp $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Include/GpioV2Pad.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/GpioV2Pad.h

  cp $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Include/MemInfoHob.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/MemInfoHob.h

  cp $WORKSPACE_SILICON/$FSP_PKG_NAME/FspPkgPcdShare.dsc $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/FspPkgPcdShare.dsc

  cp $WORKSPACE_COMMON/$PLATFORM_SI_PACKAGE/Include/SmbiosCacheInfoHob.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/SmbiosCacheInfoHob.h

  cp $WORKSPACE_COMMON/$PLATFORM_SI_PACKAGE/Include/SmbiosProcessorInfoHob.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/SmbiosProcessorInfoHob.h

  cp $WORKSPACE_COMMON/$PLATFORM_SI_PACKAGE/Include/HobUsageDataHob.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/HobUsageDataHob.h

  cp $WORKSPACE_CORE_SILICON/IntelSiliconPkg/Include/IndustryStandard/FirmwareVersionInfo.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FirmwareVersionInfo.h

  cp $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FspProducerDataHeader.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FspProducerDataHeader.h

  python3 Edk2/IntelFsp2Pkg/Tools/GenCfgOpt.py UPDTXT \
     $WORKSPACE_SILICON/$FSP_PKG_NAME/$FSP_PKG_NAME.dsc \
     $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV \
     $BD_MACRO $FSP_BUILD_OPTION_PCD

  error=$?
  if [ $error -eq "256" ]
   then
    echo  DSC is not changed, no need to recreate MAP and BIN file
  else
   if [ $error -eq "1" ]
    then
     echo Error in generating UPD TXT file
     return 1
   fi

  echo UPD TXT file was generated successfully !

  echo Generate VPD Header File ...
  rm  $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_T_UPD_GUID.bin \
            $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_T_UPD_GUID.map
 #2>nul

  rm  $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_M_UPD_GUID.bin \
            $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_M_UPD_GUID.map
 #2>nul

  rm  $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_S_UPD_GUID.bin \
            $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_S_UPD_GUID.map
 #2>nul

  BPDG -o $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_T_UPD_GUID.bin \
       -m $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_T_UPD_GUID.map \
        $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_T_UPD_GUID.txt

      if [ $? -eq "1" ]
       then
       return 1
      fi

  BPDG -o $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_M_UPD_GUID.bin \
       -m $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_M_UPD_GUID.map \
        $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_M_UPD_GUID.txt

      if [ $? -eq "1" ]
       then
       return 1
      fi

  BPDG -o $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_S_UPD_GUID.bin \
       -m $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_S_UPD_GUID.map \
        $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/$FSP_S_UPD_GUID.txt

      if [ $? -eq "1" ]
       then
       return 1
      fi
  fi

  python3 Edk2/IntelFsp2Pkg/Tools/GenCfgOpt.py HEADER \
         $WORKSPACE_SILICON/$FSP_PKG_NAME/$FSP_PKG_NAME.dsc \
         $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV \
         $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/BootLoaderPlatformData.h \
         $BD_MACRO $FSP_BUILD_OPTION_PCD

  error=$?
  if [ $error -eq "256" ]
   then
    echo
    # No need to recreate header file
   else
      if [ $error -eq "1" ]
       then
        echo Error in generating Header file
        return 1
      fi

    echo Vpd header file was generated successfully !

    echo Generate BSF File ...
    python3 Edk2/IntelFsp2Pkg/Tools/GenCfgOpt.py GENBSF \
         $WORKSPACE_SILICON/$FSP_PKG_NAME/$FSP_PKG_NAME.dsc \
         $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV \
         $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Fsp.bsf \
         $BD_MACRO $FSP_BUILD_OPTION_PCD

    if [ $? -eq "1" ]
       then
        echo Error in generating BSF file
        return 1
    fi

    echo BSF file was generated successfully !
    if [ -f $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FspUpd.h ]
      then chmod +w $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FspUpd.h
    fi
    if [ -f $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FsptUpd.h ]
      then chmod +w $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FsptUpd.h
    fi
    if [ -f $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FspmUpd.h ]
      then chmod +w $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FspmUpd.h
    fi
    if [ -f $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FspsUpd.h ]
      then chmod +w $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FspsUpd.h
    fi
    if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspUpd.h ]
     then
      cp $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspUpd.h  $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FspUpd.h
    fi
    if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FsptUpd.h ]
     then
      cp $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FsptUpd.h  $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FsptUpd.h
    fi
    if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspmUpd.h ]
     then
      cp $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspmUpd.h  $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FspmUpd.h
    fi
    if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspsUpd.h ]
     then
      cp $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspsUpd.h  $WORKSPACE_SILICON/$PLATFORM_SI_PACKAGE/Fsp/Include/FspsUpd.h
    fi
  fi

  if [ "$PERFORMANCE_ENABLE" = "TRUE" ];then
    echo FspPerformanceEnable is enabled...
    export FSP_BUILD_OPTION_PCD="$FSP_BUILD_OPTION_PCD --pcd gPantherLakeFspPkgTokenSpaceGuid.PcdFspPerformanceEnable=TRUE"
  fi
}

function PostBuild(){
  echo Start of PostBuild ...
  echo Get UTC time as build time.
  ORIGINAL_PATH=$PWD
  cd $WORKSPACE_CORE/BaseTools/Scripts
  if [ "$NOTIMESTAMP" = "1" ];then
    YYYY=0
    MMDD=0
    HHMM=0
  else
    YYYY=$(python3 GetUtcDateTime.py --year)
    MMDD=$(python3 GetUtcDateTime.py --date)
    HHMM=$(python3 GetUtcDateTime.py --time)
  fi

  echo $YYYY
  echo $MMDD
  echo $HHMM
  cd $ORIGINAL_PATH

  echo Patch FSP-O Image ...
  python3 $WORKSPACE_CORE/IntelFsp2Pkg/Tools/PatchFv.py \
    $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV \
    FSP-O:FSP \
    "0x0000,            _BASE_FSP-O_,                                                                                       @Temporary Base" \
    "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-O Size" \
    "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-O Base" \
    "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x8000 | $FSP_BUILD_TYPE | $FSP_RELEASE_TYPE,                  @FSP-O Component Attribute"
  if [ $? -ne 0 ]
  then
    PreBuildFail
    exit 1
  fi

  echo Patch FSP-T Image ...
  #
  # @todo Change FSP.fd to Fsp.fd
  #
  python3 $WORKSPACE_CORE/IntelFsp2Pkg/Tools/PatchFv.py \
    $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV \
    FSP-T:FSP \
    "0x0000,            _BASE_FSP-T_,                                                                                       @Temporary Base" \
    "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-T Size" \
    "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-T Base" \
    "<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                                                        @FSP-T Image Attribute" \
    "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x1000 | $FSP_BUILD_TYPE | $FSP_RELEASE_TYPE,                  @FSP-T Component Attribute" \
    "<[0x0000]>+0x00B8, 70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA:0x1C - <[0x0000]>,                                             @FSP-T CFG Offset" \
    "<[0x0000]>+0x00BC, [70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA:0x14] & 0xFFFFFF - 0x001C,                                    @FSP-T CFG Size" \
    "<[0x0000]>+0x00C4, FspSecCoreT:${SYMBOL_PREFIX}TempRamInitApi - [0x0000],                                              @TempRamInit API" \
    "<[0x0000]>+0x010C, 0x$YYYY,                                                                                            @BuildTimeStamp" \
    "<[0x0000]>+0x0110, 0x$MMDD,                                                                                            @BuildTimeStamp" \
    "<[0x0000]>+0x0114, 0x$HHMM,                                                                                            @BuildTimeStamp" \
    "0x0000,            0x00000000,                                                                                         @Restore the value" \
    "FspSecCoreT:${SYMBOL_PREFIX}FspInfoHeaderRelativeOff, FspSecCoreT:${SYMBOL_PREFIX}AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-T Header Offset"
  if [ $? -ne 0 ]
  then
    PreBuildFail
    exit 1
  fi

  echo Patch FSP-M Image ...
  #
  # @todo Change FSP.fd to Fsp.fd
  #
  python3 $WORKSPACE_CORE/IntelFsp2Pkg/Tools/PatchFv.py \
    $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV \
    FSP-M:FSP \
    "0x0000,            _BASE_FSP-M_,                                                                                       @Temporary Base" \
    "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-M Size" \
    "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-M Base" \
    "<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                                                        @FSP-M Image Attribute" \
    "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x2000 | $FSP_BUILD_TYPE | $FSP_RELEASE_TYPE,                  @FSP-M Component Attribute" \
    "<[0x0000]>+0x00B8, D5B86AEA-6AF7-40D4-8014-982301BC3D89:0x1C - <[0x0000]>,                                             @FSP-M CFG Offset" \
    "<[0x0000]>+0x00BC, [D5B86AEA-6AF7-40D4-8014-982301BC3D89:0x14] & 0xFFFFFF - 0x001C,                                    @FSP-M CFG Size" \
    "<[0x0000]>+0x00D0, Fsp24SecCoreM:${SYMBOL_PREFIX}FspMemoryInitApi - [0x0000],                                          @MemoryInitApi API" \
    "<[0x0000]>+0x00D4, Fsp24SecCoreM:${SYMBOL_PREFIX}TempRamExitApi - [0x0000],                                            @TempRamExit API" \
    "<[0x0000]>+0x00E4, Fsp24SecCoreM:${SYMBOL_PREFIX}FspMultiPhaseMemoryInitApi - [0x0000],                                @FspMultiPhaseMemoryInitApi API" \
    "<[0x0000]>+0x010C, 0x$YYYY,                                                                                            @BuildTimeStamp" \
    "<[0x0000]>+0x0110, 0x$MMDD,                                                                                            @BuildTimeStamp" \
    "<[0x0000]>+0x0114, 0x$HHMM,                                                                                            @BuildTimeStamp" \
    "Fsp24SecCoreM:${SYMBOL_PREFIX}FspPeiCoreEntryOff, PeiCore:${SYMBOL_PREFIX}_ModuleEntryPoint - [0x0000],                @PeiCore Entry" \
    "0x0000,            0x00000000,                                                                                         @Restore the value" \
    "Fsp24SecCoreM:${SYMBOL_PREFIX}FspInfoHeaderRelativeOff, Fsp24SecCoreM:${SYMBOL_PREFIX}AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-M Header Offset"
  if [ $? -ne 0 ]
  then
    PreBuildFail
    exit 1
  fi

  echo Patch FSP-S Image ...
  #
  # @todo Change FSP.fd to Fsp.fd
  #

  python3 $WORKSPACE_CORE/IntelFsp2Pkg/Tools/PatchFv.py \
    $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV \
    FSP-S:FSP \
    "0x0000,            _BASE_FSP-S_,                                                                                       @Temporary Base" \
    "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-S Size" \
    "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-S Base" \
    "<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                                                        @FSP-S Image Attribute" \
    "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x3000 | $FSP_BUILD_TYPE | $FSP_RELEASE_TYPE,                  @FSP-S Component Attribute" \
    "<[0x0000]>+0x00B8, E3CD9B18-998C-4F76-B65E-98B154E5446F:0x1C - <[0x0000]>,                                             @FSP-S CFG Offset" \
    "<[0x0000]>+0x00BC, [E3CD9B18-998C-4F76-B65E-98B154E5446F:0x14] & 0xFFFFFF - 0x001C,                                    @FSP-S CFG Size" \
    "<[0x0000]>+0x00D8, Fsp24SecCoreS:${SYMBOL_PREFIX}FspSiliconInitApi - [0x0000],                                         @SiliconInit API" \
    "<[0x0000]>+0x00CC, Fsp24SecCoreS:${SYMBOL_PREFIX}NotifyPhaseApi - [0x0000],                                            @NotifyPhase API" \
    "<[0x0000]>+0x00DC, Fsp24SecCoreS:${SYMBOL_PREFIX}FspMultiPhaseSiInitApi - [0x0000],                                    @FspMultiPhaseSiInitApi API" \
    "<[0x0000]>+0x010C, 0x$YYYY,                                                                                            @BuildTimeStamp" \
    "<[0x0000]>+0x0110, 0x$MMDD,                                                                                            @BuildTimeStamp" \
    "<[0x0000]>+0x0114, 0x$HHMM,                                                                                            @BuildTimeStamp" \
    "0x0000,            0x00000000,                                                                                         @Restore the value" \
    "Fsp24SecCoreS:${SYMBOL_PREFIX}FspInfoHeaderRelativeOff, Fsp24SecCoreS:${SYMBOL_PREFIX}AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-S Header Offset"
  if [ $? -ne 0 ]
  then
    PreBuildFail
    exit 1
  fi
}

function Build(){
  make -C $WORKSPACE_CORE/BaseTools/Source/C
  chmod +w $WORKSPACE_SILICON/$FSP_PKG_NAME/$FSP_PKG_NAME.dsc

  PreBuild $*

  if [ $? -eq 1 ]
  then
    exit 1
  fi

  build $BD_ARGS $FSP_BUILD_OPTION_PCD
  if [ $? -ne 0 ]
  then
    exit 1
  fi
  PostBuild
  if [ $? -ne 0 ]
  then
    exit 1
  fi
}

function ReleaseTypeTest(){
  BD_TARGET=RELEASE
  BD_MACRO="-D TARGET="$BD_TARGET" -D CFG_OUTDIR="$OUT_DIR" "$EXT_BUILD_FLAGS
  DSCFILE="-p "$FSP_PKG_NAME/$FSP_PKG_NAME.dsc #" -y ReleaseTestBuild.log"
  BD_ARGS=$DSCFILE" -b RELEASE "$BD_MACRO" -a $FSP_ARCH -a X64 -n 1 -t "$TOOL_CHAIN" -y ReportRelease.log -Y PCD -Y LIBRARY"
  FSP_BUILD_TYPE=0x0001
  FSP_RELEASE_TYPE=0x0000
  Build $*
}

function ReleaseBuild(){
  BD_TARGET=RELEASE
  BD_MACRO="-D TARGET="$BD_TARGET" -D CFG_OUTDIR="$OUT_DIR" "$EXT_BUILD_FLAGS
  DSCFILE="-p "$FSP_PKG_NAME/$FSP_PKG_NAME.dsc #" -y ReleaseBuild.log"
  BD_ARGS=$DSCFILE" -b RELEASE "$BD_MACRO" -a $FSP_ARCH -a X64 -n 1 -t "$TOOL_CHAIN" -y ReportRelease.log -Y PCD -Y LIBRARY"
  FSP_BUILD_TYPE=0x0001
  FSP_RELEASE_TYPE=0x0002
  Build $*
}

function DebugBuild(){
  BD_TARGET=DEBUG
  BD_MACRO="-D TARGET="$BD_TARGET" -D CFG_DEBUG=1 -D DEBUG_BIOS_ENABLE=TRUE -D CFG_OUTDIR="$OUT_DIR" "$EXT_BUILD_FLAGS
  DSCFILE="-p "$FSP_PKG_NAME/$FSP_PKG_NAME.dsc   #" -y DebugBuild.log"
  #echo $DSCFILE
  BD_ARGS=$DSCFILE" -b DEBUG "$BD_MACRO" -a $FSP_ARCH -a X64 -n 1 -t "$TOOL_CHAIN" -y ReportDebug.log -Y PCD -Y LIBRARY"
  FSP_BUILD_TYPE=0x0000
  FSP_RELEASE_TYPE=0x0000
  Build $*
}

function CopyFspBinaryToBinPkg(){
    echo Copy FSP binary to $FSP_BIN_PKG_NAME

    if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Fsp.bsf ]
      then chmod +w $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Fsp.bsf
    fi
    if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Fsp.fd ]
      then chmod +w $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Fsp.fd
    fi

    cp $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FSP.fd $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Fsp.fd

    if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FspUpd.h ]
      then chmod +w $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FspUpd.h
    fi
    if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspUpd.h ]
      then cp $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspUpd.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include
    fi
    if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FsptUpd.h ]
      then chmod +w $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FsptUpd.h
    fi
    if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FsptUpd.h ]
      then cp $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FsptUpd.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include
    fi
    if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FspmUpd.h ]
      then chmod +w $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FspmUpd.h
    fi
    if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspmUpd.h ]
      then cp $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspmUpd.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include
    fi
    if [ -f $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FspsUpd.h ]
      then chmod +w $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include/FspsUpd.h
    fi
    if [ -f $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspsUpd.h ]
      then cp $OUT_DIR/$FSP_PKG_NAME/$BD_TARGET"_"$TOOL_CHAIN/FV/FspsUpd.h $WORKSPACE_SILICON/$FSP_BIN_PKG_NAME/Include
    fi
}

cd $WORKSPACE
echo BuildFv.sh EXT_BUILD_FLAGS $EXT_BUILD_FLAGS

OUT_DIR=$WORKSPACE/Build

for ((i=0 ; i <= $# ; i++)); do
  if [ "${!i}" = "-clean" ]; then
    Clean
    exit 0
  fi
done

#
# Add $WORKSPACE/Silicon and $WORKSPACE_CORE in PACKAGES_PATH
#
export WORKSPACE_CORE_SILICON=$WORKSPACE/Edk2Platforms/Silicon/Intel
export WORKSPACE_PLATFORM=$WORKSPACE/Intel
export PACKAGES_PATH=$WORKSPACE_CORE_SILICON:$WORKSPACE_CORE:$WORKSPACE_COMMON:$WORKSPACE_SILICON:$WORKSPACE_BINARIES:$WORKSPACE_PLATFORM
echo "Current PACKAGES_PATH is : $PACKAGES_PATH"

#
# SKU PCH + CPU Information
#
if [ "$1" = "PantherLake" ]; then
  echo Default PantherLake build
  shift
fi

if [ "$1" = "WildcatLake" ]; then
  echo Default WildcatLake build
  #
  # Setting PcdFspImageIdString to $WCLFSP$ for WCL
  #
  export FSP_BUILD_OPTION_PCD="$FSP_BUILD_OPTION_PCD --pcd gSiPkgTokenSpaceGuid.PcdFspImageIdString=0x245053464c435724"
fi

if [ "$1" = "GCC" ]; then
#
# Detect GCC Tool Chain Version
#
GCC_VERSION=$(gcc -dumpversion|sed 's/^\([0-9]\.[0-9]\).*/\1/')
if [ "$GCC_VERSION" = "5.4" ]; then
  export TOOL_CHAIN=GCC
elif [ "$GCC_VERSION" = "5.5" ]; then
  export TOOL_CHAIN=GCC
elif [ "$GCC_VERSION" = "11" ]; then
  export TOOL_CHAIN=GCC
elif [ "$GCC_VERSION_SKIP_CHECK" != "" ]; then
  export TOOL_CHAIN=GCC
else
  echo "Unsupported GCC version!"
  exit 1
fi
#
# Toolchain information
#
gcc -v
fi

#
# GCC Symbol Prefix is _
#
export SYMBOL_PREFIX=_

if [ "$1" = "CLANG" ]; then
  export SYMBOL_PREFIX=
  CLANG_VERSION=$($CLANG_BIN"clang" -dumpversion | sed 's/\.[0-9]*\.[0-9]*$//g')
  if [ "$CLANG_VERSION" = "" ]; then
    echo "Can't find CLANGPDB directory, please assign it by defining CLANG_BIN"
    exit 1
  elif [ "$CLANG_VERSION" -ge "9" ]; then
    export TOOL_CHAIN=CLANGPDB
  else
    echo "Unsupported CLANG version!"
    exit 1
  fi
fi

if [ -z "${TOOL_CHAIN}" ]; then
  export TOOL_CHAIN=$1
fi

if [ "$3" = "fsp64" ] || [ -z "${FSP_ARCH}" ]; then
  export FSP_ARCH=X64
  export FSP64_BUILD=TRUE
  export EXT_BUILD_FLAGS="$EXT_BUILD_FLAGS -D FSP_ARCH=X64 -D FSP64_BUILD=TRUE"
fi

#
# Check for -r32 / -tr32 / -d32 so other platform build targets don't have to change build parameters in their manifest.
#
if [ "$2" = "-r" ] || [ "$2" = "-r32" ]
 then
  ReleaseBuild
  CopyFspBinaryToBinPkg
elif [ "$2" = "-tr" ] || [ "$2" = "-tr32" ]
 then
  ReleaseTypeTest
  CopyFspBinaryToBinPkg
elif [ "$2" = "-d" ] || [ "$2" = "-d32" ]
 then
   DebugBuild
   CopyFspBinaryToBinPkg
elif [ -z "$2" ]
 then
   DebugBuild
   CopyFspBinaryToBinPkg
else
  echo
  echo  ERROR: $2 is not valid parameter.
  USAGE
fi
