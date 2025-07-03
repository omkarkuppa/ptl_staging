/**@file
 I2S Audio Codec ACPI

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

// HD Audio I2S codecs
#define HDAC_I2S_DISABLED 0
#define HDAC_I2S_ALC274   1
#define HDAC_I2S_ALC1308  2
#if FixedPcdGet8 (PcdEmbeddedEnable) == 0x1
  #define HDAC_I2S_ALC5682I_VD  6
  #define HDAC_I2S_ALC5682I_VS  7
#endif

//
// HDAC 1308
//
Device (HDC1)
{
  Name (_HID, "10EC1308")
  Name (_DDN, "Intel(R) Smart Sound Technology Audio Codec")
  Name (_UID, 1)
  Name (CDIS, 0) // Codec Disabled state

  Method (_CRS, 0, NotSerialized) {

    // IICB: Method generates 'I2cSerialBusV2' descriptor buffer
    // I2SB: NVS variable with I2C controller connection
    // INTB: Method generates 'Interrupt' descriptor buffer
    // I2SI: NVS variable with codec jack detection pin
    Return (ConcatenateResTemplate (IICB (0x10, I2SB), INTB (I2SI, INT_LEVEL_TRIG, INT_ACTIVE_LOW)))
  }

  Method (_PRS, 0, NotSerialized) {
    Return (_CRS)
  }

  Method (_STA, 0, NotSerialized) {
    If (LAnd (LEqual (I2SC, HDAC_I2S_ALC1308), LNotEqual (CDIS, 1))) {
      Return (0xF)  // I2S Codec Enabled
    }
    Return (0x0)  // I2S Codec Dialed (via BIOS Policy/Setup)
  }

  Method (_SRS, 0x1, Serialized) {
    Store (0, CDIS) // Clear Disabled bit
  }

  Method (_DIS, 0x0, NotSerialized) {
    Store (1, CDIS) // Set Disabled bit
  }
}  // Device (HDC1)

//
// HDAC 274
//
Device (HDC2)
{
  Name (_HID, "INT34C2")
  Name (_DDN, "Intel(R) Smart Sound Technology Audio Codec")
  Name (_UID, 1)
  Name (CDIS, 0) // Codec Disabled state

  Method (_CRS, 0, NotSerialized) {

    // IICB: Method generates 'I2cSerialBusV2' descriptor buffer
    // I2SB: NVS variable with I2C controller connection
    // INTB: Method generates 'Interrupt' descriptor buffer
    // I2SI: NVS variable with codec jack detection pin
    Return (ConcatenateResTemplate (IICB (0x1C, I2SB), INTB (I2SI, INT_LEVEL_TRIG, INT_ACTIVE_LOW)))
  }
  Method (_PRS, 0, NotSerialized) {
    Return (_CRS)
  }
  Method (_STA, 0, NotSerialized) {
    If (LAnd (LEqual (I2SC, HDAC_I2S_ALC274), LNotEqual (CDIS, 1))) {
      Return (0xF)  // I2S Codec Enabled
    }
    Return (0x0)  // I2S Codec Disabled (via BIOS Policy/Setup)
  }

  Method (_SRS, 0x1, Serialized) {
    Store (0, CDIS) // Clear Disabled bit
  }

  Method (_DIS, 0x0, NotSerialized) {
    Store (1, CDIS) // Set Disabled bit
  }
}  // Device (HDC2)

#if FixedPcdGet8 (PcdEmbeddedEnable) == 0x1
  If ( LOr(LEqual(I2SC, HDAC_I2S_ALC5682I_VD), LEqual(I2SC, HDAC_I2S_ALC5682I_VS)) ) {
     Device(RT58)
     {
       Name (_HID, "INT00000")  // _HID: Hardware ID
       Name (_CID, "INT00000")  // _CID: Compatible ID
       Name (_UID, Zero)  // _UID: Unique ID
       Name (_DDN, "Headset Codec")  // _DDN: DOS Device Name

       Method(_INI) {
         If (LEqual(I2SC, HDAC_I2S_ALC5682I_VD)) {
           Store ("10EC5682", _HID)
           Store ("10EC5682", _CID)
         } ElseIf (LEqual(I2SC, HDAC_I2S_ALC5682I_VS)) {
           Store ("RTL5682", _HID)
           Store ("RTL5682", _CID)
         }
      }

       Method(_STA, 0, NotSerialized)  // _STA: Status
       {
         Return (0x0F)
       }

       Name (_CRS, ResourceTemplate ()
       {
         I2cSerialBusV2 (0x001A, ControllerInitiated, 0x00061A80,
             AddressingMode7Bit, "\\_SB.PC00.I2C3",
             0x00, ResourceConsumer, , Exclusive,
         )
         GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,"\\_SB.GPI5", 0x00, ResourceConsumer, ,){GPIOV2_PTL_DRIVER_xxgpp_d_17}
         GpioInt (Edge, ActiveHigh, ExclusiveAndWake, PullUp, 0x0000,"\\_SB.GPI1", 0x00, ResourceConsumer, ,){GPIOV2_PTL_DRIVER_xxgpp_f_17}
     })

       Name (_DSD, Package (0x02)  //_DSD: Device-Specific Data
       {
         ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),/*Device Properties for _DSD */
         Package (0x02)
         {
            Package (0x02)
            {
               "irq-gpios",
               Package (0x04)
               {
                 \_SB.PC00.I2C3.RT58,
                 Zero,
                 Zero,
                 Zero
               }
             },

             Package (0x02)
             {
               "realtek,jd-src",
               One
             }
         }
       })
     } // Device (RT58)
   } // ElseIf HDAC_I2S_ALC5682I_VD
#endif
