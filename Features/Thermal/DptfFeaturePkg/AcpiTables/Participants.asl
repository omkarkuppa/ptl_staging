/** @file
  Intel ACPI Reference Code for including Participants at Compile time

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

ACPI_DEBUG_EXTERNAL_REFERENCE

//
// Fan participants
//
#if FixedPcdGetBool(PcdDttFan1Participant) == 1
  Include("TFN1Participant.asl")
#endif
#if FixedPcdGetBool(PcdDttFan2Participant) == 1
  Include("TFN2Participant.asl")
#endif
#if FixedPcdGetBool(PcdDttFan3Participant) == 1
  Include("TFN3Participant.asl")
#endif

//
// Participants using device sensors.
//
#if FixedPcdGetBool(PcdDttChargerParticipant) == 1
  Include("ChrgParticipant.asl")
#endif

#if FixedPcdGetBool(PcdDttPowerParticipant) == 1
  Include("TPwrParticipant.asl")
#endif

//
// Participants using battery.
//
#if FixedPcdGetBool(PcdDttBatteryParticipant) == 1
  Include("BatteryParticipant.asl")
#endif

//
// Participants using motherboard sensors.
//
#if FixedPcdGetBool(PcdDttSen1Participant) == 1
  Include("Sen1Participant.asl")
#endif
#if FixedPcdGetBool(PcdDttSen2Participant) == 1
  Include("Sen2Participant.asl")
#endif
#if FixedPcdGetBool(PcdDttSen3Participant) == 1
  Include("Sen3Participant.asl")
#endif
#if FixedPcdGetBool(PcdDttSen4Participant) == 1
  Include("Sen4Participant.asl")
#endif
#if FixedPcdGetBool(PcdDttSen5Participant) == 1
  Include("Sen5Participant.asl")
#endif

//
// Processor participant.
//
  Include("B0d4Participant.asl")
  Include("B0d4CtdpPolicy.asl")

  //
  // PCH FIVR Participant
  //
#if FixedPcdGetBool(PcdDttPchpParticipant) == 1
  Include("PchpParticipant.asl")
#endif
