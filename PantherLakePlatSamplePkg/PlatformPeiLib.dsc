## @file
#  platform build option configuration file.
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
#  @par Specification
##

[LibraryClasses.IA32]

  PeiGuidForwardLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiGuidForwardLib/PeiGuidForwardLib.inf
  PeiPlatformRecoveryLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPlatformRecoveryLib/PeiPlatformRecoveryLib.inf
  SerialPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiSerialPortParameterLib/PeiSerialPortParameterLib.inf

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  PeiPolicyDebugLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPolicyDebugLib/PeiPolicyDebugLibApi.inf
  PeiPolicyUpdateLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPolicyUpdateLib/PeiPolicyUpdateLibApi.inf
!else
  PeiPolicyDebugLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPolicyDebugLib/PeiPolicyDebugLib.inf
  PeiPolicyUpdateLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPolicyUpdateLib/PeiPolicyUpdateLib.inf
!endif

  PeiPolicyRestrictedLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPolicyRestrictedLibNull/PeiPolicyRestrictedLibNull.inf

[LibraryClasses.IA32.PEIM]

!if $(TARGET) == DEBUG
  SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/PeiSerialIoUartDebugPropertyLib.inf
  !if gPlatformModuleTokenSpaceGuid.PcdSerialPortEnable == TRUE
    SerialPortLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseSerialPortLib/BaseSerialPortLib.inf
  !endif
  DebugPrintErrorLevelLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiDxeSmmDebugPrintErrorLevelLib/PeiDebugPrintErrorLevelLib.inf
!endif

  FspWrapperExtractGuidedLib|$(PLATFORM_FULL_PACKAGE)/FspWrapper/Library/PeiFspWrapperExtractGuidedLib/PeiFspWrapperExtractGuidedLib.inf
