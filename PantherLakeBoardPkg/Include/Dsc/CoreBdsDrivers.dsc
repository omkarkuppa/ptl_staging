## @file
#  DSC file of core BDS driver that contain into UPL binary
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
# @par Specification
#
##

[Components.X64]
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf {
    <LibraryClasses>
      !if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
        CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibFmp/DxeCapsuleLib.inf
      !endif
      !if $(TARGET) == DEBUG
        # If want to use PeiDxeDebugLibReportStatusCode as DebugLib to save size,
        # the log of 'Welcome Admin' or 'Admin password is not set' will not shown.
        DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
      !endif
  }
  MdeModulePkg/Application/BootManagerMenuApp/BootManagerMenuApp.inf

!if gBoardModuleTokenSpaceGuid.PcdSetupEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/UiApp/UiApp.inf {
    <Defines>
      FILE_GUID = D89A7D8B-D016-4D26-93E3-EAB6B4D3B0A2
  }
!endif #PcdSetupEnable == TRUE

