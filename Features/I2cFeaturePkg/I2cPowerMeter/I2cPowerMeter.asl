/** @file
  ACPI I2C Power Meter SSDT table for PAC195X

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#define I2C_BUS_V2_RES_SOU_CON_STR_OFF 31 // Offset to update ResourceSource Controller number string
#define DEFAULT_RAIL_STRING   "                               "
#define DEFAULT_RSENSE_VALUE  0xFFFF
//
// Defined as an SSDT to be able to dynamically load based on BIOS
// setup options
//

DefinitionBlock (
  "I2cPowerMeter.aml",
  "SSDT",
  0x02,
  "I2PmRv",
  "I2Pm_Rvp",
  0x1000
  )
{
  External(\_SB.PC00.I2C0, DeviceObj)           //I2C0 Controller
  External(\_SB.PC00.I2C1, DeviceObj)           //I2C1 Controller
  External(\_SB.PC00.I2C3, DeviceObj)           //I2C3 Controller
  External(\_SB.PC00.I2C4, DeviceObj)           //I2C4 Controller
  External(\_SB.PC00.I2C5, DeviceObj)           //I2C5 Controller

  External (\ADBG,MethodObj)

  Scope(\_SB.PC00.I2C3)
  {
    //
    // PA01 Device: Defines how the PAC193x is connected to the I2C Controller.
    //

    Name (POME, 0xFF)

    Name (PACT, 0xFF)  // Total powermeter device count

    Name (PABN, 0xFF)  // I2C Bus number
    // Defined Per Device
    Name (PAS1, Package() { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF })  // I2C Device number
    Name (PMP0,Package(){
      Package()
      {
        // Rail Name, Resistor Value - Device 0
        DEFAULT_RAIL_STRING, // Channel 1
        DEFAULT_RAIL_STRING, // Channel 2
        DEFAULT_RAIL_STRING, // Channel 3
        DEFAULT_RAIL_STRING  // Channel 4
      },
      Package()
      {
        // Rail Name, Resistor Value - Device 1
        DEFAULT_RAIL_STRING, // Channel 1
        DEFAULT_RAIL_STRING, // Channel 2
        DEFAULT_RAIL_STRING, // Channel 3
        DEFAULT_RAIL_STRING  // Channel 4
      },
      Package()
      {
        // Rail Name, Resistor Value - Device 2
        DEFAULT_RAIL_STRING, // Channel 1
        DEFAULT_RAIL_STRING, // Channel 2
        DEFAULT_RAIL_STRING, // Channel 3
        DEFAULT_RAIL_STRING  // Channel 4
      },
      Package()
      {
        // Rail Name, Resistor Value - Device 3
        DEFAULT_RAIL_STRING, // Channel 1
        DEFAULT_RAIL_STRING, // Channel 2
        DEFAULT_RAIL_STRING, // Channel 3
        DEFAULT_RAIL_STRING  // Channel 4
      },
      Package()
      {
        // Rail Name, Resistor Value - Device 4
        DEFAULT_RAIL_STRING, // Channel 1
        DEFAULT_RAIL_STRING, // Channel 2
        DEFAULT_RAIL_STRING, // Channel 3
        DEFAULT_RAIL_STRING  // Channel 4
      },
      Package()
      {
        // Rail Name, Resistor Value - Device 5
        DEFAULT_RAIL_STRING, // Channel 1
        DEFAULT_RAIL_STRING, // Channel 2
        DEFAULT_RAIL_STRING, // Channel 3
        DEFAULT_RAIL_STRING  // Channel 4
      },
      Package()
      {
        // Rail Name, Resistor Value - Device 6
        DEFAULT_RAIL_STRING, // Channel 1
        DEFAULT_RAIL_STRING, // Channel 2
        DEFAULT_RAIL_STRING, // Channel 3
        DEFAULT_RAIL_STRING  // Channel 4
      },
      Package()
      {
        // Rail Name, Resistor Value - Device 7
        DEFAULT_RAIL_STRING, // Channel 1
        DEFAULT_RAIL_STRING, // Channel 2
        DEFAULT_RAIL_STRING, // Channel 3
        DEFAULT_RAIL_STRING  // Channel 4
      }
    })

    Name (PMP1,Package(){
      // Device 0
      Package()
      {
        // Value
        DEFAULT_RSENSE_VALUE, // Channel 1
        DEFAULT_RSENSE_VALUE, // Channel 2
        DEFAULT_RSENSE_VALUE, // Channel 3
        DEFAULT_RSENSE_VALUE  // Channel 4
      },// Device 1
      Package()
      {
        // Value
        DEFAULT_RSENSE_VALUE, // Channel 1
        DEFAULT_RSENSE_VALUE, // Channel 2
        DEFAULT_RSENSE_VALUE, // Channel 3
        DEFAULT_RSENSE_VALUE  // Channel 4
      }, // Device 2
      Package()
      {
        // Value
        DEFAULT_RSENSE_VALUE, // Channel 1
        DEFAULT_RSENSE_VALUE, // Channel 2
        DEFAULT_RSENSE_VALUE, // Channel 3
        DEFAULT_RSENSE_VALUE  // Channel 4
      }, // Device 3
      Package()
      {
        // Value
        DEFAULT_RSENSE_VALUE, // Channel 1
        DEFAULT_RSENSE_VALUE, // Channel 2
        DEFAULT_RSENSE_VALUE, // Channel 3
        DEFAULT_RSENSE_VALUE  // Channel 4
      }, // Device 4
      Package()
      {
        // Value
        DEFAULT_RSENSE_VALUE, // Channel 1
        DEFAULT_RSENSE_VALUE, // Channel 2
        DEFAULT_RSENSE_VALUE, // Channel 3
        DEFAULT_RSENSE_VALUE  // Channel 4
      }, // Device 5
      Package()
      {
        // Value
        DEFAULT_RSENSE_VALUE, // Channel 1
        DEFAULT_RSENSE_VALUE, // Channel 2
        DEFAULT_RSENSE_VALUE, // Channel 3
        DEFAULT_RSENSE_VALUE  // Channel 4
      }, // Device 6
      Package()
      {
        // Value
        DEFAULT_RSENSE_VALUE, // Channel 1
        DEFAULT_RSENSE_VALUE, // Channel 2
        DEFAULT_RSENSE_VALUE, // Channel 3
        DEFAULT_RSENSE_VALUE  // Channel 4
      }, // Device 7
      Package()
      {
        // Value
        DEFAULT_RSENSE_VALUE, // Channel 1
        DEFAULT_RSENSE_VALUE, // Channel 2
        DEFAULT_RSENSE_VALUE, // Channel 3
        DEFAULT_RSENSE_VALUE  // Channel 4
      }
    })

    Device(PA01)
    {
      Name(_HID, "MCHP1940")
      Name(_UID, 1)

      // Lowest power D-State supported by the device is D3
      Name (_S0W, 3)
      // Device Status: present, enabled, and functioning properly
      Method(_STA, 0x0, NotSerialized)
      {
        If(POME)
        {
           If (LGreaterEqual (PACT,1)) {
             Return(0x0F)
           }
        }
        Return (0x00)
      }

      //
      // Current Resources Settings
      //
      Method(_CRS, 0x0, NotSerialized)
      {
        Name (RBUF , ResourceTemplate () { I2cSerialBusV2 (0x0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2Cx", 0x00, ResourceConsumer, DEV0,) })

        /* RBUF array holds I2cSerialBusV2() encoded opcode and data parameters - which is 35 bytes array
           0x8E,0x1E,0x00,0x02,0x00,0x01,0x02,0x00,
           0x00,0x01,0x06,0x00,0x80,0x1A,0x06,0x00,
           0x00,0x00,0x5C,0x5F,0x53,0x42,0x2E,0x50,
           0x43,0x30,0x30,0x2E,0x49,0x32,0x43,0x78,
           0x00,0x79,0x00

           Target address is located at offset 16
           ResourceSource or Controller scope(\\_SB.PC00.I2Cx) is located at offset 18 to 31
        */

        CreateByteField(RBUF, I2C_BUS_V2_RES_SOU_CON_STR_OFF, PAST)     // Controller string offset
        CreateWordField (RBUF, DEV0._ADR, PAAD)                         // Device target address offset

        Store (PABN, PAST)                                              // Update controller number string
        Store (DerefOf(Index(PAS1, 0)), PAAD)                           // Update target address

        Return(RBUF)
      }

      //
      // _DSM - Device Specific Method
      //
      // This method returns configuration information that PAC194x/5x driver
      // uses for the device initial configuration.
      //
      // The DSM UUID for the Microchip PAC194x/5x: {721F1534-5D27-4B60-9DF4-41A3C4B7DA3A}.
      // This must match what the Windows PAC194x/5x driver expects.
      //
      // Returns:
      // Either: A Buffer (for supported Functions, or an error)
      // A Package containing PAC194x/5x resources (configuration values)
      //
      // Input Arguments (per _DSM standard):
      //
      //     Arg0: UUID - Function Identifier
      //     Arg1: Integer - Revision
      //     Arg2: Integer - Function Index
      //     Arg3: Package - Parameters (not used in our implementation)
      //
      Function(_DSM, {BuffObj, PkgObj}, {BuffObj, IntObj, IntObj, PkgObj})
      {
        //
        // Is PAC194x/5x DSM UUID being invoked?
        //
        if(LNotEqual(Arg0, ToUUID("721F1534-5D27-4B60-9DF4-41A3C4B7DA3A")))
        {
          return(Buffer() {0x0})  // incorrect UUID, return NULL for error
        }

        //
        // Switch based on the function number requested...
        //
        switch(ToInteger(Arg2))
        {
          //
          // Function zero returns a bit-mask of supported functions
          //
          case(0)
          {
            switch(ToInteger(Arg1)) // revision check
            {
              // Revision 0: function 1->7 are supported
              case(0) {return (Buffer() {0xFF})}
            }
            break;
          }

          // Function 1 - returns the names of the monitored power rails.
          // The names should be compliant with Windows EMI power rail taxonomy.
          // If the rail name is NULL ("") -> no EMI created for the channel ("private channel").
          case(1)
          {
            return(DerefOf(Index(PMP0, 0)))
          }

          // Function 2 - returns the sense resistor values, in micro-Ohms.
          case(2)
          {
            // Sense resistors values (micro-Ohms)
            return(DerefOf(Index(PMP1, 0)))
          }

          // Function 3 - returns the EMI enable/disable bit-mask
          case(3)
          {
            Name(BUF3, Package()
            {
              // EMI bitmask - CH1:CH2:CH3:CH4
              0xF           // CHn = 1: EMI enable for channel 'n'
                            // CHn = 0: EMI disable for channel 'n' ("private channel")
            })
            return(BUF3)
          }

          // Function 4 - returns Vsense and Vbus polarity and full scale range configs
          case(4)
          {
            Name(BUF4, Package()
            {
              // Vsense configs
              0x0, 0x0, 0x0, 0x0,   // CFG_VS1, CFG_VS2, CFG_VS3, CFG_VS4
                                    // CFG_VSn: Vsense configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
                                    // Vbus configs
              0x0, 0x0, 0x0, 0x0    // CFG_VB1, CFG_VB2, CFG_VB3, CFG_VB4
                                    // CFG_VBn: Vbus configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
            })
            return(BUF4)
          }

          // Function 5 - returns the sample frequencies for ACTIVE and IDLE modes
          case(5)
          {
            Name(BUF5, Package()
             {
              1024, // ACTIVE sps - accepted values = {1024, 256, 64, 8}
              8 // IDLE sps - accepted values = {1024, 256, 64, 8}
             })
            return(BUF5)
          }

          // Function 6 - returns the REFRESH watchdog timer interval
          case(6)
          {
            Name(BUF6, Package()
            {
              900 // seconds (min=60, max=60000)
            })
            return(BUF6)
          }

          // Function 7 - returns the Vbus multiplication factors for PAC194x/5x-2
          // PAC194x/5x-2 power meter VBUS+ pins may be connected to voltage dividers
          // to measure rail voltage higher than device Vbus FSR.
          case(7)
          {
            Name(BUF7, Package()
            { // K = 1000 * (Vrail / Vbus). Must be integer, positive value.
              1000, // Channel 1 K-factor
              1000, // Channel 2 K-factor
              1000, // Channel 3 K-factor
              1000 // Channel 4 K-factor
            })
            return(BUF7)
          }

        } // switch(Arg2)
        //
        // Return an error (a buffer with a value of zero)
        // if we didn't return anything else above
        //
        return(Buffer() {0x0})
      } // _DSM
    } // PowerMeter Device PA01 Scope End

    Device(PA02)
    {
      Name(_HID, "MCHP1940")
      Name(_UID, 2)

      // Lowest power D-State supported by the device is D3
      Name (_S0W, 3)
      // Device Status: present, enabled, and functioning properly
      Method(_STA, 0x0, NotSerialized)
      {
        If(POME)
        {
          If (LGreaterEqual (PACT,2)) {
            Return(0x0F)
          }
        }
        Return (0x00)
      }

      //
      // Current Resources Settings
      //
      Method(_CRS, 0x0, NotSerialized)
      {
        Name (RBUF , ResourceTemplate () { I2cSerialBusV2 (0x0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2Cx", 0x00, ResourceConsumer, DEV0,) })

        /* RBUF array holds I2cSerialBusV2() encoded opcode and data parameters - which is 35 bytes array
           0x8E,0x1E,0x00,0x02,0x00,0x01,0x02,0x00,
           0x00,0x01,0x06,0x00,0x80,0x1A,0x06,0x00,
           0x00,0x00,0x5C,0x5F,0x53,0x42,0x2E,0x50,
           0x43,0x30,0x30,0x2E,0x49,0x32,0x43,0x78,
           0x00,0x79,0x00

           Target address is located at offset 16
           ResourceSource or Controller scope(\\_SB.PC00.I2Cx) is located at offset 18 to 31
        */

        CreateByteField(RBUF, I2C_BUS_V2_RES_SOU_CON_STR_OFF, PAST)     // Controller string offset
        CreateWordField (RBUF, DEV0._ADR, PAAD)                         // Device target address offset

        Store (PABN, PAST)                                              // Update controller number string
        Store (DerefOf(Index(PAS1, 1)), PAAD)                           // Update target address

        Return(RBUF)
      }

      //
      // _DSM - Device Specific Method
      //
      // This method returns configuration information that PAC194x/5x driver
      // uses for the device initial configuration.
      //
      // The DSM UUID for the Microchip PAC194x/5x: {721F1534-5D27-4B60-9DF4-41A3C4B7DA3A}.
      // This must match what the Windows PAC194x/5x driver expects.
      //
      // Returns:
      //         Either: A Buffer (for supported Functions, or an error)
      //                 A Package containing PAC194x/5x resources (configuration values)
      //
      // Input Arguments (per _DSM standard):
      //
      //     Arg0: UUID - Function Identifier
      //     Arg1: Integer - Revision
      //     Arg2: Integer - Function Index
      //     Arg3: Package - Parameters (not used in our implementation)
      //
      Function(_DSM, {BuffObj, PkgObj}, {BuffObj, IntObj, IntObj, PkgObj})
      {
        //
        // Is PAC194x/5x DSM UUID being invoked?
        //
        if(LNotEqual(Arg0, ToUUID("721F1534-5D27-4B60-9DF4-41A3C4B7DA3A")))
        {
          return(Buffer() {0x0})  // incorrect UUID, return NULL for error
        }

        //
        // Switch based on the function number requested...
        //
        switch(ToInteger(Arg2))
        {
          //
          // Function zero returns a bit-mask of supported functions
          //
          case(0)
          {
            switch(ToInteger(Arg1)) // revision check
            {
              // Revision 0: function 1->7 are supported
              case(0) {return (Buffer() {0xFF})}
            }
            break;
          }

          // Function 1 - returns the names of the monitored power rails.
          // The names should be compliant with Windows EMI power rail taxonomy.
          // If the rail name is NULL ("") -> no EMI created for the channel ("private channel").
          case(1)
          {
            return(DerefOf(Index(PMP0, 1)))
          }

          // Function 2 - returns the sense resistor values, in micro-Ohms.
          case(2)
          {
            // Sense resistors values (micro-Ohms)
            return(DerefOf(Index(PMP1, 1)))
          }

          // Function 3 - returns the EMI enable/disable bit-mask
          case(3)
          {
            Name(BUF3, Package()
            {
              // EMI bitmask - CH1:CH2:CH3:CH4
              0xF           // CHn = 1: EMI enable for channel 'n'
                            // CHn = 0: EMI disable for channel 'n' ("private channel")
            })
            return(BUF3)
          }

          // Function 4 - returns Vsense and Vbus polarity and full scale range configs
          case(4)
          {
            Name(BUF4, Package()
            {
              // Vsense configs
              0x0, 0x0, 0x0, 0x0,   // CFG_VS1, CFG_VS2, CFG_VS3, CFG_VS4
                                    // CFG_VSn: Vsense configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
                                    // Vbus configs
              0x0, 0x0, 0x0, 0x0    // CFG_VB1, CFG_VB2, CFG_VB3, CFG_VB4
                                    // CFG_VBn: Vbus configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
            })
            return(BUF4)
          }

          // Function 5 - returns the sample frequencies for ACTIVE and IDLE modes
          case(5)
          {
            Name(BUF5, Package()
             {
              1024, // ACTIVE sps - accepted values = {1024, 256, 64, 8}
              8 // IDLE sps - accepted values = {1024, 256, 64, 8}
             })
            return(BUF5)
          }

          // Function 6 - returns the REFRESH watchdog timer interval
          case(6)
          {
            Name(BUF6, Package()
            {
              900 // seconds (min=60, max=60000)
            })
            return(BUF6)
          }

          // Function 7 - returns the Vbus multiplication factors for PAC194x/5x-2
          // PAC194x/5x-2 power meter VBUS+ pins may be connected to voltage dividers
          // to measure rail voltage higher than device Vbus FSR.
          case(7)
          {
            Name(BUF7, Package()
            { // K = 1000 * (Vrail / Vbus). Must be integer, positive value.
              1000, // Channel 1 K-factor
              1000, // Channel 2 K-factor
              1000, // Channel 3 K-factor
              1000 // Channel 4 K-factor
            })
            return(BUF7)
          }

        } // switch(Arg2)
        //
        // Return an error (a buffer with a value of zero)
        // if we didn't return anything else above
        //
        return(Buffer() {0x0})
      } // _DSM
    } // PowerMeter Device PA02 Scope End

    Device(PA03)
    {
      Name(_HID, "MCHP1940")
      Name(_UID, 3)

      // Lowest power D-State supported by the device is D3
      Name (_S0W, 3)
      // Device Status: present, enabled, and functioning properly
      Method(_STA, 0x0, NotSerialized)
      {
        If(POME)
        {
           If (LGreaterEqual (PACT,3)) {
             Return(0x0F)
           }
        }
        Return (0x00)
      }

      //
      // Current Resources Settings
      //
      Method(_CRS, 0x0, NotSerialized)
      {
        Name (RBUF , ResourceTemplate () { I2cSerialBusV2 (0x0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2Cx", 0x00, ResourceConsumer, DEV0,) })

        /* RBUF array holds I2cSerialBusV2() encoded opcode and data parameters - which is 35 bytes array
           0x8E,0x1E,0x00,0x02,0x00,0x01,0x02,0x00,
           0x00,0x01,0x06,0x00,0x80,0x1A,0x06,0x00,
           0x00,0x00,0x5C,0x5F,0x53,0x42,0x2E,0x50,
           0x43,0x30,0x30,0x2E,0x49,0x32,0x43,0x78,
           0x00,0x79,0x00

           Target address is located at offset 16
           ResourceSource or Controller scope(\\_SB.PC00.I2Cx) is located at offset 18 to 31
        */

        CreateByteField(RBUF, I2C_BUS_V2_RES_SOU_CON_STR_OFF, PAST)     // Controller string offset
        CreateWordField (RBUF, DEV0._ADR, PAAD)                         // Device target address offset

        Store (PABN, PAST)                                              // Update controller number string
        Store (DerefOf(Index(PAS1, 2)), PAAD)                           // Update target address

        Return(RBUF)
      }

      //
      // _DSM - Device Specific Method
      //
      // This method returns configuration information that PAC194x/5x driver
      // uses for the device initial configuration.
      //
      // The DSM UUID for the Microchip PAC194x/5x: {721F1534-5D27-4B60-9DF4-41A3C4B7DA3A}.
      // This must match what the Windows PAC194x/5x driver expects.
      //
      // Returns:
      //         Either: A Buffer (for supported Functions, or an error)
      //                 A Package containing PAC194x/5x resources (configuration values)
      //
      // Input Arguments (per _DSM standard):
      //
      //     Arg0: UUID - Function Identifier
      //     Arg1: Integer - Revision
      //     Arg2: Integer - Function Index
      //     Arg3: Package - Parameters (not used in our implementation)
      //
      Function(_DSM, {BuffObj, PkgObj}, {BuffObj, IntObj, IntObj, PkgObj})
      {
        //
        // Is PAC194x/5x DSM UUID being invoked?
        //
        if(LNotEqual(Arg0, ToUUID("721F1534-5D27-4B60-9DF4-41A3C4B7DA3A")))
        {
          return(Buffer() {0x0})  // incorrect UUID, return NULL for error
        }

        //
        // Switch based on the function number requested...
        //
        switch(ToInteger(Arg2))
        {
          //
          // Function zero returns a bit-mask of supported functions
          //
          case(0)
          {
            switch(ToInteger(Arg1)) // revision check
            {
              // Revision 0: function 1->7 are supported
              case(0) {return (Buffer() {0xFF})}
            }
            break;
          }

          // Function 1 - returns the names of the monitored power rails.
          // The names should be compliant with Windows EMI power rail taxonomy.
          // If the rail name is NULL ("") -> no EMI created for the channel ("private channel").
          case(1)
          {
            return(DerefOf(Index(PMP0, 2)))
          }

          // Function 2 - returns the sense resistor values, in micro-Ohms.
          case(2)
          {
            // Sense resistors values (micro-Ohms)
            return(DerefOf(Index(PMP1, 2)))
          }

          // Function 3 - returns the EMI enable/disable bit-mask
          case(3)
          {
            Name(BUF3, Package()
            {
              // EMI bitmask - CH1:CH2:CH3:CH4
              0xF           // CHn = 1: EMI enable for channel 'n'
                            // CHn = 0: EMI disable for channel 'n' ("private channel")
            })
            return(BUF3)
          }

          // Function 4 - returns Vsense and Vbus polarity and full scale range configs
          case(4)
          {
            Name(BUF4, Package()
            {
              // Vsense configs
              0x0, 0x0, 0x0, 0x0,   // CFG_VS1, CFG_VS2, CFG_VS3, CFG_VS4
                                    // CFG_VSn: Vsense configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
                                    // Vbus configs
              0x0, 0x0, 0x0, 0x0    // CFG_VB1, CFG_VB2, CFG_VB3, CFG_VB4
                                    // CFG_VBn: Vbus configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
            })
            return(BUF4)
          }

          // Function 5 - returns the sample frequencies for ACTIVE and IDLE modes
          case(5)
          {
            Name(BUF5, Package()
             {
              1024, // ACTIVE sps - accepted values = {1024, 256, 64, 8}
              8 // IDLE sps - accepted values = {1024, 256, 64, 8}
             })
            return(BUF5)
          }

          // Function 6 - returns the REFRESH watchdog timer interval
          case(6)
          {
            Name(BUF6, Package()
            {
              900 // seconds (min=60, max=60000)
            })
            return(BUF6)
          }

          // Function 7 - returns the Vbus multiplication factors for PAC194x/5x-2
          // PAC194x/5x-2 power meter VBUS+ pins may be connected to voltage dividers
          // to measure rail voltage higher than device Vbus FSR.
          case(7)
          {
            Name(BUF7, Package()
            { // K = 1000 * (Vrail / Vbus). Must be integer, positive value.
              1000, // Channel 1 K-factor
              1000, // Channel 2 K-factor
              1000, // Channel 3 K-factor
              1000 // Channel 4 K-factor
            })
            return(BUF7)
          }

        } // switch(Arg2)
        //
        // Return an error (a buffer with a value of zero)
        // if we didn't return anything else above
        //
        return(Buffer() {0x0})
      } // _DSM
    } // PowerMeter Device PA03 Scope End

    Device(PA04)
    {
      Name(_HID, "MCHP1940")
      Name(_UID, 4)

      // Lowest power D-State supported by the device is D3
      Name (_S0W, 3)
      // Device Status: present, enabled, and functioning properly
      Method(_STA, 0x0, NotSerialized)
      {
        If(POME)
        {
           If (LGreaterEqual (PACT,4)) {
             Return(0x0F)
           }
        }
        Return (0x00)
      }

      //
      // Current Resources Settings
      //
      Method(_CRS, 0x0, NotSerialized)
      {
        Name (RBUF , ResourceTemplate () { I2cSerialBusV2 (0x0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2Cx", 0x00, ResourceConsumer, DEV0,) })

        /* RBUF array holds I2cSerialBusV2() encoded opcode and data parameters - which is 35 bytes array
           0x8E,0x1E,0x00,0x02,0x00,0x01,0x02,0x00,
           0x00,0x01,0x06,0x00,0x80,0x1A,0x06,0x00,
           0x00,0x00,0x5C,0x5F,0x53,0x42,0x2E,0x50,
           0x43,0x30,0x30,0x2E,0x49,0x32,0x43,0x78,
           0x00,0x79,0x00

           Target address is located at offset 16
           ResourceSource or Controller scope(\\_SB.PC00.I2Cx) is located at offset 18 to 31
        */

        CreateByteField(RBUF, I2C_BUS_V2_RES_SOU_CON_STR_OFF, PAST)     // Controller string offset
        CreateWordField (RBUF, DEV0._ADR, PAAD)                         // Device target address offset

        Store (PABN, PAST)                                              // Update controller number string
        Store (DerefOf(Index(PAS1, 3)), PAAD)                           // Update target address

        Return(RBUF)
      }

      //
      // _DSM - Device Specific Method
      //
      // This method returns configuration information that PAC194x/5x driver
      // uses for the device initial configuration.
      //
      // The DSM UUID for the Microchip PAC194x/5x: {721F1534-5D27-4B60-9DF4-41A3C4B7DA3A}.
      // This must match what the Windows PAC194x/5x driver expects.
      //
      // Returns:
      //         Either: A Buffer (for supported Functions, or an error)
      //                 A Package containing PAC194x/5x resources (configuration values)
      //
      // Input Arguments (per _DSM standard):
      //
      //     Arg0: UUID - Function Identifier
      //     Arg1: Integer - Revision
      //     Arg2: Integer - Function Index
      //     Arg3: Package - Parameters (not used in our implementation)
      //
      Function(_DSM, {BuffObj, PkgObj}, {BuffObj, IntObj, IntObj, PkgObj})
      {
        //
        // Is PAC194x/5x DSM UUID being invoked?
        //
        if(LNotEqual(Arg0, ToUUID("721F1534-5D27-4B60-9DF4-41A3C4B7DA3A")))
        {
          return(Buffer() {0x0})  // incorrect UUID, return NULL for error
        }

        //
        // Switch based on the function number requested...
        //
        switch(ToInteger(Arg2))
        {
          //
          // Function zero returns a bit-mask of supported functions
          //
          case(0)
          {
            switch(ToInteger(Arg1)) // revision check
            {
              // Revision 0: function 1->7 are supported
              case(0) {return (Buffer() {0xFF})}
            }
            break;
          }

          // Function 1 - returns the names of the monitored power rails.
          // The names should be compliant with Windows EMI power rail taxonomy.
          // If the rail name is NULL ("") -> no EMI created for the channel ("private channel").
          case(1)
          {
            return(DerefOf(Index(PMP0, 3)))
          }

          // Function 2 - returns the sense resistor values, in micro-Ohms.
          case(2)
          {
            // Sense resistors values (micro-Ohms)
            return(DerefOf(Index(PMP1, 3)))
          }

          // Function 3 - returns the EMI enable/disable bit-mask
          case(3)
          {
            Name(BUF3, Package()
            {
              // EMI bitmask - CH1:CH2:CH3:CH4
              0xF           // CHn = 1: EMI enable for channel 'n'
                            // CHn = 0: EMI disable for channel 'n' ("private channel")
            })
            return(BUF3)
          }

          // Function 4 - returns Vsense and Vbus polarity and full scale range configs
          case(4)
          {
            Name(BUF4, Package()
            {
              // Vsense configs
              0x0, 0x0, 0x0, 0x0,   // CFG_VS1, CFG_VS2, CFG_VS3, CFG_VS4
                                    // CFG_VSn: Vsense configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
                                    // Vbus configs
              0x0, 0x0, 0x0, 0x0    // CFG_VB1, CFG_VB2, CFG_VB3, CFG_VB4
                                    // CFG_VBn: Vbus configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
            })
            return(BUF4)
          }

          // Function 5 - returns the sample frequencies for ACTIVE and IDLE modes
          case(5)
          {
            Name(BUF5, Package()
             {
              1024, // ACTIVE sps - accepted values = {1024, 256, 64, 8}
              8 // IDLE sps - accepted values = {1024, 256, 64, 8}
             })
            return(BUF5)
          }

          // Function 6 - returns the REFRESH watchdog timer interval
          case(6)
          {
            Name(BUF6, Package()
            {
              900 // seconds (min=60, max=60000)
            })
            return(BUF6)
          }

          // Function 7 - returns the Vbus multiplication factors for PAC194x/5x-2
          // PAC194x/5x-2 power meter VBUS+ pins may be connected to voltage dividers
          // to measure rail voltage higher than device Vbus FSR.
          case(7)
          {
            Name(BUF7, Package()
            { // K = 1000 * (Vrail / Vbus). Must be integer, positive value.
              1000, // Channel 1 K-factor
              1000, // Channel 2 K-factor
              1000, // Channel 3 K-factor
              1000 // Channel 4 K-factor
            })
            return(BUF7)
          }

        } // switch(Arg2)
        //
        // Return an error (a buffer with a value of zero)
        // if we didn't return anything else above
        //
        return(Buffer() {0x0})
      } // _DSM
    } // PowerMeter Device PA04 Scope End

    //
    // PA05 Device: Defines how the PAC193x is connected to the I2C Controller.
    //
    Device(PA05)
    {
      Name(_HID, "MCHP1940")
      Name(_UID, 5)

      // Lowest power D-State supported by the device is D3
      Name (_S0W, 3)
      // Device Status: present, enabled, and functioning properly
      Method(_STA, 0x0, NotSerialized)
      {
        If(POME)
        {
           If (LGreaterEqual (PACT,5)) {
             Return(0x0F)
           }
        }
        Return (0x00)
      }

      //
      // Current Resources Settings
      //
      Method(_CRS, 0x0, NotSerialized)
      {
        Name (RBUF , ResourceTemplate () { I2cSerialBusV2 (0x0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2Cx", 0x00, ResourceConsumer, DEV0,) })

        /* RBUF array holds I2cSerialBusV2() encoded opcode and data parameters - which is 35 bytes array
           0x8E,0x1E,0x00,0x02,0x00,0x01,0x02,0x00,
           0x00,0x01,0x06,0x00,0x80,0x1A,0x06,0x00,
           0x00,0x00,0x5C,0x5F,0x53,0x42,0x2E,0x50,
           0x43,0x30,0x30,0x2E,0x49,0x32,0x43,0x78,
           0x00,0x79,0x00

           Target address is located at offset 16
           ResourceSource or Controller scope(\\_SB.PC00.I2Cx) is located at offset 18 to 31
        */

        CreateByteField(RBUF, I2C_BUS_V2_RES_SOU_CON_STR_OFF, PAST)     // Controller string offset
        CreateWordField (RBUF, DEV0._ADR, PAAD)                         // Device target address offset

        Store (PABN, PAST)                                              // Update controller number string
        Store (DerefOf(Index(PAS1, 4)), PAAD)                           // Update target address

        Return(RBUF)
      }

      //
      // _DSM - Device Specific Method
      //
      // This method returns configuration information that PAC194x/5x driver
      // uses for the device initial configuration.
      //
      // The DSM UUID for the Microchip PAC194x/5x: {721F1534-5D27-4B60-9DF4-41A3C4B7DA3A}.
      // This must match what the Windows PAC194x/5x driver expects.
      //
      // Returns:
      //         Either: A Buffer (for supported Functions, or an error)
      //                 A Package containing PAC194x/5x resources (configuration values)
      //
      // Input Arguments (per _DSM standard):
      //
      //     Arg0: UUID - Function Identifier
      //     Arg1: Integer - Revision
      //     Arg2: Integer - Function Index
      //     Arg3: Package - Parameters (not used in our implementation)
      //
      Function(_DSM, {BuffObj, PkgObj}, {BuffObj, IntObj, IntObj, PkgObj})
      {
        //
        // Is PAC194x/5x DSM UUID being invoked?
        //
        if(LNotEqual(Arg0, ToUUID("721F1534-5D27-4B60-9DF4-41A3C4B7DA3A")))
        {
          return(Buffer() {0x0})  // incorrect UUID, return NULL for error
        }

        //
        // Switch based on the function number requested...
        //
        switch(ToInteger(Arg2))
        {
          //
          // Function zero returns a bit-mask of supported functions
          //
          case(0)
          {
            switch(ToInteger(Arg1)) // revision check
            {
              // Revision 0: function 1->7 are supported
              case(0) {return (Buffer() {0xFF})}
            }
            break;
          }

          // Function 1 - returns the names of the monitored power rails.
          // The names should be compliant with Windows EMI power rail taxonomy.
          // If the rail name is NULL ("") -> no EMI created for the channel ("private channel").
          case(1)
          {
            return(DerefOf(Index(PMP0, 4)))
          }

          // Function 2 - returns the sense resistor values, in micro-Ohms.
          case(2)
          {
            // Sense resistors values (micro-Ohms)
            return(DerefOf(Index(PMP1, 4)))
          }

          // Function 3 - returns the EMI enable/disable bit-mask
          case(3)
          {
            Name(BUF3, Package()
            {
              // EMI bitmask - CH1:CH2:CH3:CH4
              0xF           // CHn = 1: EMI enable for channel 'n'
                            // CHn = 0: EMI disable for channel 'n' ("private channel")
            })
            return(BUF3)
          }

          // Function 4 - returns Vsense and Vbus polarity and full scale range configs
          case(4)
          {
            Name(BUF4, Package()
            {
              // Vsense configs
              0x0, 0x0, 0x0, 0x0,   // CFG_VS1, CFG_VS2, CFG_VS3, CFG_VS4
                                    // CFG_VSn: Vsense configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
                                    // Vbus configs
              0x0, 0x0, 0x0, 0x0    // CFG_VB1, CFG_VB2, CFG_VB3, CFG_VB4
                                    // CFG_VBn: Vbus configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
            })
            return(BUF4)
          }

          // Function 5 - returns the sample frequencies for ACTIVE and IDLE modes
          case(5)
          {
            Name(BUF5, Package()
             {
              1024, // ACTIVE sps - accepted values = {1024, 256, 64, 8}
              8 // IDLE sps - accepted values = {1024, 256, 64, 8}
             })
            return(BUF5)
          }

          // Function 6 - returns the REFRESH watchdog timer interval
          case(6)
          {
            Name(BUF6, Package()
            {
              900 // seconds (min=60, max=60000)
            })
            return(BUF6)
          }

          // Function 7 - returns the Vbus multiplication factors for PAC194x/5x-2
          // PAC194x/5x-2 power meter VBUS+ pins may be connected to voltage dividers
          // to measure rail voltage higher than device Vbus FSR.
          case(7)
          {
            Name(BUF7, Package()
            { // K = 1000 * (Vrail / Vbus). Must be integer, positive value.
              1000, // Channel 1 K-factor
              1000, // Channel 2 K-factor
              1000, // Channel 3 K-factor
              1000 // Channel 4 K-factor
            })
            return(BUF7)
          }

        } // switch(Arg2)
        //
        // Return an error (a buffer with a value of zero)
        // if we didn't return anything else above
        //
        return(Buffer() {0x0})
      } // _DSM
    } // PowerMeter Device PA05 Scope End

    Device(PA06)
    {
      Name(_HID, "MCHP1940")
      Name(_UID, 6)

      // Lowest power D-State supported by the device is D3
      Name (_S0W, 3)
      // Device Status: present, enabled, and functioning properly
      Method(_STA, 0x0, NotSerialized)
      {
        If(POME)
        {
           If (LGreaterEqual (PACT,6)) {
             Return(0x0F)
           }
        }
        Return (0x00)
      }

      //
      // Current Resources Settings
      //
      Method(_CRS, 0x0, NotSerialized)
      {
        Name (RBUF , ResourceTemplate () { I2cSerialBusV2 (0x0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2Cx", 0x00, ResourceConsumer, DEV0,) })

        /* RBUF array holds I2cSerialBusV2() encoded opcode and data parameters - which is 35 bytes array
           0x8E,0x1E,0x00,0x02,0x00,0x01,0x02,0x00,
           0x00,0x01,0x06,0x00,0x80,0x1A,0x06,0x00,
           0x00,0x00,0x5C,0x5F,0x53,0x42,0x2E,0x50,
           0x43,0x30,0x30,0x2E,0x49,0x32,0x43,0x78,
           0x00,0x79,0x00

           Target address is located at offset 16
           ResourceSource or Controller scope(\\_SB.PC00.I2Cx) is located at offset 18 to 31
        */

        CreateByteField(RBUF, I2C_BUS_V2_RES_SOU_CON_STR_OFF, PAST)     // Controller string offset
        CreateWordField (RBUF, DEV0._ADR, PAAD)                         // Device target address offset

        Store (PABN, PAST)                                              // Update controller number string
        Store (DerefOf(Index(PAS1, 5)), PAAD)                           // Update target address

        Return(RBUF)
      }

      //
      // _DSM - Device Specific Method
      //
      // This method returns configuration information that PAC194x/5x driver
      // uses for the device initial configuration.
      //
      // The DSM UUID for the Microchip PAC194x/5x: {721F1534-5D27-4B60-9DF4-41A3C4B7DA3A}.
      // This must match what the Windows PAC194x/5x driver expects.
      //
      // Returns:
      //         Either: A Buffer (for supported Functions, or an error)
      //                 A Package containing PAC194x/5x resources (configuration values)
      //
      // Input Arguments (per _DSM standard):
      //
      //     Arg0: UUID - Function Identifier
      //     Arg1: Integer - Revision
      //     Arg2: Integer - Function Index
      //     Arg3: Package - Parameters (not used in our implementation)
      //
      Function(_DSM, {BuffObj, PkgObj}, {BuffObj, IntObj, IntObj, PkgObj})
      {
        //
        // Is PAC194x/5x DSM UUID being invoked?
        //
        if(LNotEqual(Arg0, ToUUID("721F1534-5D27-4B60-9DF4-41A3C4B7DA3A")))
        {
          return(Buffer() {0x0})  // incorrect UUID, return NULL for error
        }

        //
        // Switch based on the function number requested...
        //
        switch(ToInteger(Arg2))
        {
          //
          // Function zero returns a bit-mask of supported functions
          //
          case(0)
          {
            switch(ToInteger(Arg1)) // revision check
            {
              // Revision 0: function 1->7 are supported
              case(0) {return (Buffer() {0xFF})}
            }
            break;
          }

          // Function 1 - returns the names of the monitored power rails.
          // The names should be compliant with Windows EMI power rail taxonomy.
          // If the rail name is NULL ("") -> no EMI created for the channel ("private channel").
          case(1)
          {
            return(DerefOf(Index(PMP0, 5)))
          }

          // Function 2 - returns the sense resistor values, in micro-Ohms.
          case(2)
          {
            // Sense resistors values (micro-Ohms)
            return(DerefOf(Index(PMP1, 5)))
          }

          // Function 3 - returns the EMI enable/disable bit-mask
          case(3)
          {
            Name(BUF3, Package()
            {
              // EMI bitmask - CH1:CH2:CH3:CH4
              0xF           // CHn = 1: EMI enable for channel 'n'
                            // CHn = 0: EMI disable for channel 'n' ("private channel")
            })
            return(BUF3)
          }

          // Function 4 - returns Vsense and Vbus polarity and full scale range configs
          case(4)
          {
            Name(BUF4, Package()
            {
              // Vsense configs
              0x0, 0x0, 0x0, 0x0,   // CFG_VS1, CFG_VS2, CFG_VS3, CFG_VS4
                                    // CFG_VSn: Vsense configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
                                    // Vbus configs
              0x0, 0x0, 0x0, 0x0    // CFG_VB1, CFG_VB2, CFG_VB3, CFG_VB4
                                    // CFG_VBn: Vbus configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
            })
            return(BUF4)
          }

          // Function 5 - returns the sample frequencies for ACTIVE and IDLE modes
          case(5)
          {
            Name(BUF5, Package()
             {
              1024, // ACTIVE sps - accepted values = {1024, 256, 64, 8}
              8 // IDLE sps - accepted values = {1024, 256, 64, 8}
             })
            return(BUF5)
          }

          // Function 6 - returns the REFRESH watchdog timer interval
          case(6)
          {
            Name(BUF6, Package()
            {
              900 // seconds (min=60, max=60000)
            })
            return(BUF6)
          }

          // Function 7 - returns the Vbus multiplication factors for PAC194x/5x-2
          // PAC194x/5x-2 power meter VBUS+ pins may be connected to voltage dividers
          // to measure rail voltage higher than device Vbus FSR.
          case(7)
          {
            Name(BUF7, Package()
            { // K = 1000 * (Vrail / Vbus). Must be integer, positive value.
              1000, // Channel 1 K-factor
              1000, // Channel 2 K-factor
              1000, // Channel 3 K-factor
              1000 // Channel 4 K-factor
            })
            return(BUF7)
          }

        } // switch(Arg2)
        //
        // Return an error (a buffer with a value of zero)
        // if we didn't return anything else above
        //
        return(Buffer() {0x0})
      } // _DSM
    } // PowerMeter Device PA06 Scope End

    Device(PA07)
    {
      Name(_HID, "MCHP1940")
      Name(_UID, 7)

      // Lowest power D-State supported by the device is D3
      Name (_S0W, 3)
      // Device Status: present, enabled, and functioning properly
      Method(_STA, 0x0, NotSerialized)
      {
        If(POME)
        {
           If (LGreaterEqual (PACT,7)) {
             Return(0x0F)
           }
        }
        Return (0x00)
      }

      //
      // Current Resources Settings
      //
      Method(_CRS, 0x0, NotSerialized)
      {
        Name (RBUF , ResourceTemplate () { I2cSerialBusV2 (0x0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2Cx", 0x00, ResourceConsumer, DEV0,) })

        /* RBUF array holds I2cSerialBusV2() encoded opcode and data parameters - which is 35 bytes array
           0x8E,0x1E,0x00,0x02,0x00,0x01,0x02,0x00,
           0x00,0x01,0x06,0x00,0x80,0x1A,0x06,0x00,
           0x00,0x00,0x5C,0x5F,0x53,0x42,0x2E,0x50,
           0x43,0x30,0x30,0x2E,0x49,0x32,0x43,0x78,
           0x00,0x79,0x00

           Target address is located at offset 16
           ResourceSource or Controller scope(\\_SB.PC00.I2Cx) is located at offset 18 to 31
        */

        CreateByteField(RBUF, I2C_BUS_V2_RES_SOU_CON_STR_OFF, PAST)     // Controller string offset
        CreateWordField (RBUF, DEV0._ADR, PAAD)                         // Device target address offset

        Store (PABN, PAST)                                              // Update controller number string
        Store (DerefOf(Index(PAS1, 6)), PAAD)                           // Update target address

        Return(RBUF)
      }

      //
      // _DSM - Device Specific Method
      //
      // This method returns configuration information that PAC194x/5x driver
      // uses for the device initial configuration.
      //
      // The DSM UUID for the Microchip PAC194x/5x: {721F1534-5D27-4B60-9DF4-41A3C4B7DA3A}.
      // This must match what the Windows PAC194x/5x driver expects.
      //
      // Returns:
      //         Either: A Buffer (for supported Functions, or an error)
      //                 A Package containing PAC194x/5x resources (configuration values)
      //
      // Input Arguments (per _DSM standard):
      //
      //     Arg0: UUID - Function Identifier
      //     Arg1: Integer - Revision
      //     Arg2: Integer - Function Index
      //     Arg3: Package - Parameters (not used in our implementation)
      //
      Function(_DSM, {BuffObj, PkgObj}, {BuffObj, IntObj, IntObj, PkgObj})
      {
        //
        // Is PAC194x/5x DSM UUID being invoked?
        //
        if(LNotEqual(Arg0, ToUUID("721F1534-5D27-4B60-9DF4-41A3C4B7DA3A")))
        {
          return(Buffer() {0x0})  // incorrect UUID, return NULL for error
        }

        //
        // Switch based on the function number requested...
        //
        switch(ToInteger(Arg2))
        {
          //
          // Function zero returns a bit-mask of supported functions
          //
          case(0)
          {
            switch(ToInteger(Arg1)) // revision check
            {
              // Revision 0: function 1->7 are supported
              case(0) {return (Buffer() {0xFF})}
            }
            break;
          }

          // Function 1 - returns the names of the monitored power rails.
          // The names should be compliant with Windows EMI power rail taxonomy.
          // If the rail name is NULL ("") -> no EMI created for the channel ("private channel").
          case(1)
          {
            return(DerefOf(Index(PMP0, 6)))
          }

          // Function 2 - returns the sense resistor values, in micro-Ohms.
          case(2)
          {
            // Sense resistors values (micro-Ohms)
            return(DerefOf(Index(PMP1, 6)))
          }

          // Function 3 - returns the EMI enable/disable bit-mask
          case(3)
          {
            Name(BUF3, Package()
            {
              // EMI bitmask - CH1:CH2:CH3:CH4
              0xF           // CHn = 1: EMI enable for channel 'n'
                            // CHn = 0: EMI disable for channel 'n' ("private channel")
            })
            return(BUF3)
          }

          // Function 4 - returns Vsense and Vbus polarity and full scale range configs
          case(4)
          {
            Name(BUF4, Package()
            {
              // Vsense configs
              0x0, 0x0, 0x0, 0x0,   // CFG_VS1, CFG_VS2, CFG_VS3, CFG_VS4
                                    // CFG_VSn: Vsense configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
                                    // Vbus configs
              0x0, 0x0, 0x0, 0x0    // CFG_VB1, CFG_VB2, CFG_VB3, CFG_VB4
                                    // CFG_VBn: Vbus configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
            })
            return(BUF4)
          }

          // Function 5 - returns the sample frequencies for ACTIVE and IDLE modes
          case(5)
          {
            Name(BUF5, Package()
             {
              1024, // ACTIVE sps - accepted values = {1024, 256, 64, 8}
              8 // IDLE sps - accepted values = {1024, 256, 64, 8}
             })
            return(BUF5)
          }

          // Function 6 - returns the REFRESH watchdog timer interval
          case(6)
          {
            Name(BUF6, Package()
            {
              900 // seconds (min=60, max=60000)
            })
            return(BUF6)
          }

          // Function 7 - returns the Vbus multiplication factors for PAC194x/5x-2
          // PAC194x/5x-2 power meter VBUS+ pins may be connected to voltage dividers
          // to measure rail voltage higher than device Vbus FSR.
          case(7)
          {
            Name(BUF7, Package()
            { // K = 1000 * (Vrail / Vbus). Must be integer, positive value.
              1000, // Channel 1 K-factor
              1000, // Channel 2 K-factor
              1000, // Channel 3 K-factor
              1000 // Channel 4 K-factor
            })
            return(BUF7)
          }

        } // switch(Arg2)
        //
        // Return an error (a buffer with a value of zero)
        // if we didn't return anything else above
        //
        return(Buffer() {0x0})
      } // _DSM
    } // PowerMeter Device PA07 Scope End

    Device(PA08)
    {
      Name(_HID, "MCHP1940")
     Name(_UID, 8)

      // Lowest power D-State supported by the device is D3
      Name (_S0W, 3)
      // Device Status: present, enabled, and functioning properly
      Method(_STA, 0x0, NotSerialized)
      {
        If(POME)
        {
           If (LGreaterEqual (PACT,8)) {
             Return(0x0F)
           }
        }
        Return (0x00)
      }

      //
      // Current Resources Settings
      //
      Method(_CRS, 0x0, NotSerialized)
      {
        Name (RBUF , ResourceTemplate () { I2cSerialBusV2 (0x0, ControllerInitiated, 400000, AddressingMode7Bit,
            "\\_SB.PC00.I2Cx", 0x00, ResourceConsumer, DEV0,) })

        /* RBUF array holds I2cSerialBusV2() encoded opcode and data parameters - which is 35 bytes array
           0x8E,0x1E,0x00,0x02,0x00,0x01,0x02,0x00,
           0x00,0x01,0x06,0x00,0x80,0x1A,0x06,0x00,
           0x00,0x00,0x5C,0x5F,0x53,0x42,0x2E,0x50,
           0x43,0x30,0x30,0x2E,0x49,0x32,0x43,0x78,
           0x00,0x79,0x00

           Target address is located at offset 16
           ResourceSource or Controller scope(\\_SB.PC00.I2Cx) is located at offset 18 to 31
        */

        CreateByteField(RBUF, I2C_BUS_V2_RES_SOU_CON_STR_OFF, PAST)     // Controller string offset
        CreateWordField (RBUF, DEV0._ADR, PAAD)                         // Device target address offset

        Store (PABN, PAST)                                              // Update controller number string
        Store (DerefOf(Index(PAS1, 7)), PAAD)                           // Update target address

        Return(RBUF)
      }

      //
      // _DSM - Device Specific Method
      //
      // This method returns configuration information that PAC194x/5x driver
      // uses for the device initial configuration.
      //
      // The DSM UUID for the Microchip PAC194x/5x: {721F1534-5D27-4B60-9DF4-41A3C4B7DA3A}.
      // This must match what the Windows PAC194x/5x driver expects.
      //
      // Returns:
      //         Either: A Buffer (for supported Functions, or an error)
      //                 A Package containing PAC194x/5x resources (configuration values)
      //
      // Input Arguments (per _DSM standard):
      //
      //     Arg0: UUID - Function Identifier
      //     Arg1: Integer - Revision
      //     Arg2: Integer - Function Index
      //     Arg3: Package - Parameters (not used in our implementation)
      //
      Function(_DSM, {BuffObj, PkgObj}, {BuffObj, IntObj, IntObj, PkgObj})
      {
        //
        // Is PAC194x/5x DSM UUID being invoked?
        //
        if(LNotEqual(Arg0, ToUUID("721F1534-5D27-4B60-9DF4-41A3C4B7DA3A")))
        {
          return(Buffer() {0x0})  // incorrect UUID, return NULL for error
        }

        //
        // Switch based on the function number requested...
        //
        switch(ToInteger(Arg2))
        {
          //
          // Function zero returns a bit-mask of supported functions
          //
          case(0)
          {
            switch(ToInteger(Arg1)) // revision check
            {
              // Revision 0: function 1->7 are supported
              case(0) {return (Buffer() {0xFF})}
            }
            break;
          }

          // Function 1 - returns the names of the monitored power rails.
          // The names should be compliant with Windows EMI power rail taxonomy.
          // If the rail name is NULL ("") -> no EMI created for the channel ("private channel").
          case(1)
          {
            return(DerefOf(Index(PMP0, 7)))
          }

          // Function 2 - returns the sense resistor values, in micro-Ohms.
          case(2)
          {
            // Sense resistors values (micro-Ohms)
            return(DerefOf(Index(PMP1, 7)))
          }

          // Function 3 - returns the EMI enable/disable bit-mask
          case(3)
          {
            Name(BUF3, Package()
            {
              // EMI bitmask - CH1:CH2:CH3:CH4
              0xF           // CHn = 1: EMI enable for channel 'n'
                            // CHn = 0: EMI disable for channel 'n' ("private channel")
            })
            return(BUF3)
          }

          // Function 4 - returns Vsense and Vbus polarity and full scale range configs
          case(4)
          {
            Name(BUF4, Package()
            {
              // Vsense configs
              0x0, 0x0, 0x0, 0x0,   // CFG_VS1, CFG_VS2, CFG_VS3, CFG_VS4
                                    // CFG_VSn: Vsense configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
                                    // Vbus configs
              0x0, 0x0, 0x0, 0x0    // CFG_VB1, CFG_VB2, CFG_VB3, CFG_VB4
                                    // CFG_VBn: Vbus configuration for channel 'n'
                                    // {0x0 = unipolar; 0x1 = bipolar; 0x2 = half scale bipolar}
            })
            return(BUF4)
          }

          // Function 5 - returns the sample frequencies for ACTIVE and IDLE modes
          case(5)
          {
            Name(BUF5, Package()
             {
              1024, // ACTIVE sps - accepted values = {1024, 256, 64, 8}
              8 // IDLE sps - accepted values = {1024, 256, 64, 8}
             })
            return(BUF5)
          }

          // Function 6 - returns the REFRESH watchdog timer interval
          case(6)
          {
            Name(BUF6, Package()
            {
              900 // seconds (min=60, max=60000)
            })
            return(BUF6)
          }

          // Function 7 - returns the Vbus multiplication factors for PAC194x/5x-2
          // PAC194x/5x-2 power meter VBUS+ pins may be connected to voltage dividers
          // to measure rail voltage higher than device Vbus FSR.
          case(7)
          {
            Name(BUF7, Package()
            { // K = 1000 * (Vrail / Vbus). Must be integer, positive value.
              1000, // Channel 1 K-factor
              1000, // Channel 2 K-factor
              1000, // Channel 3 K-factor
              1000 // Channel 4 K-factor
            })
            return(BUF7)
          }

        } // switch(Arg2)
        //
        // Return an error (a buffer with a value of zero)
        // if we didn't return anything else above
        //
        return(Buffer() {0x0})
      } // _DSM
    } // PowerMeter Device PA08 Scope End
  }
}
