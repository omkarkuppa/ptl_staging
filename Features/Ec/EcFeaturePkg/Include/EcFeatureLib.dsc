## @file
# This is a build description for the EC advanced feature.
#
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
# @par Specification Reference:
#
###

[LibraryClasses.common]
!if gEcFeaturePkgTokenSpaceGuid.PcdEcEnable == TRUE
  EcPrivateLib|EcFeaturePkg/LibraryPrivate/EcPrivateLib/EcPrivateLib.inf
  EcLib|EcFeaturePkg/Library/BaseEcLib/BaseEcLib.inf
  EcMiscLib|EcFeaturePkg/Library/BaseEcMiscLib/BaseEcMiscLib.inf
  EcTcssLib|EcFeaturePkg/Library/BaseEcTcssLib/BaseEcTcssLib.inf
  EcSmiLib|EcFeaturePkg/Library/BaseEcSmiLib/BaseEcSmiLib.inf  # @todo dummy library will clean up in future phase.
  DxeBatteryLib|EcFeaturePkg/Library/DxeBatteryLib/DxeBatteryLib.inf
!else
  EcLib|EcFeaturePkg/Library/BaseEcLibNull/BaseEcLibNull.inf
  EcMiscLib|EcFeaturePkg/Library/BaseEcMiscLibNull/BaseEcMiscLibNull.inf
  EcTcssLib|EcFeaturePkg/Library/BaseEcTcssLibNull/BaseEcTcssLibNull.inf
  EcSmiLib|EcFeaturePkg/Library/BaseEcSmiLib/BaseEcSmiLib.inf  # @todo dummy library will clean up in future phase.
  DxeBatteryLib|EcFeaturePkg/Library/DxeBatteryLibNull/DxeBatteryLibNull.inf
!endif
