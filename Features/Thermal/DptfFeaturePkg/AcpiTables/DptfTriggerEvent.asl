/** @file
  Intel ACPI Reference Code for Intel(R) Dynamic Tuning Technology
  Used for driver debugging and validation.
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef PARENT_OF_LPCB
#define PARENT_OF_LPCB PC00
#endif

Scope (\_SB.IETM)
{
  // TEVT (Trigger EvenT)
  // Used for Intel(R) Dynamic Tuning Technology driver debugging and validation
  // This function will generate an event of the event code as given in arg1 for the participant given in parameter arg0.
  // Arguments: (2)
  //   Arg0 - The device object name string.
  //   Arg1 - An Integer containing the notification code to be used in Notify.
  // Return Value:
  //   None
  //
  Method (TEVT,2,Serialized,,,{StrObj,IntObj})
  {
    Switch (ToString (Arg0))
    {
      case ("IETM") {
        Notify (\_SB.IETM, Arg1) // Intel Extended Thermal Manager (Intel(R) Dynamic Tuning Technology Manager)
      }
      case ("TCPU") {
        Notify (\_SB.PC00.TCPU, Arg1) // TCPU Participant (SA Thermal Device)
      }
#if FixedPcdGetBool(PcdDttPchpParticipant) == 1
      case ("TPCH") {
        Notify (\_SB.IETM.TPCH, Arg1) // PCH FIVR Participant
      }
#endif
    }
    If (CondRefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC)) {
      Switch (ToString (Arg0))
      {
#if FixedPcdGetBool(PcdDttChargerParticipant) == 1
        case ("CHRG") {
          Notify (\_SB.IETM.CHRG, Arg1) // Charger Participant
        }
#endif
#if FixedPcdGetBool(PcdDttSen1Participant) == 1
        case("SEN1") {
          Notify(\_SB.IETM.SEN1, Arg1) // Sensor Participant
        }
#endif
#if FixedPcdGetBool(PcdDttSen2Participant) == 1
        case ("SEN2") {
          Notify (\_SB.IETM.SEN2, Arg1) // Sensor Participant
        }
#endif
#if FixedPcdGetBool(PcdDttSen3Participant) == 1
        case ("SEN3") {
          Notify (\_SB.IETM.SEN3, Arg1) // Sensor Participant
        }
#endif
#if FixedPcdGetBool(PcdDttSen4Participant) == 1
        case ("SEN4") {
          Notify (\_SB.IETM.SEN4, Arg1) // Sensor Participant
        }
#endif
#if FixedPcdGetBool(PcdDttSen5Participant) == 1
        case ("SEN5") {
          Notify (\_SB.IETM.SEN5, Arg1) // Sensor Participant
        }
#endif
#if FixedPcdGetBool(PcdDttFan1Participant) == 1
        case ("TFN1") {
          Notify (\_SB.IETM.TFN1, Arg1) // Fan Participant
        }
#endif
#if FixedPcdGetBool(PcdDttFan2Participant) == 1
        case ("TFN2") {
          Notify (\_SB.IETM.TFN2, Arg1) // Fan Participant
        }
#endif
#if FixedPcdGetBool(PcdDttFan3Participant) == 1
        case ("TFN3") {
          Notify (\_SB.IETM.TFN3, Arg1) // Fan Participant
        }
#endif
#if FixedPcdGetBool(PcdDttPowerParticipant) == 1
        case ("TPWR") {
          Notify (\_SB.IETM.TPWR, Arg1) // Power Participant
        }
#endif
      }
    }
  }

} // End Scope (\_SB.IETM)
