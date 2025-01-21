/** @file
  SoundWire topology definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

// abb 5.9.22 - Added MS Audio compositor driver to _SB Scope
Scope (_SB)
{
    Device (AUDC) // MS Audio compositor driver
    {
        Name (_HID, "ACPI0018") // INF in MS audio compositor driver looks for this HWID (UEFI-defined for audio comp.)
        // #include is used so that the preprocessor is run on the contents of the file.
        #include <AudioComposition_Jamerson_4spk_Cohen_mic_UAJ_all_endpoints.asl>
        #include <AudioComposition_Jamerson_6spk_Cohen_mic_UAJ.asl>
    }
}

Scope (_SB.PC00.HDAS.IDA.SNDW)
{
#ifdef JAMERSON_1_LID
    // Jamerson 1 (UID_0, SNDW_0)
    Device (SWD2)
    {
        Name (_ADR, ((0x000<<52)|(JAMERSON_1_LID << 48)|(3<<44)|(JAMERSON_1_UID << 40)|0x01FA355601) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamersons

# ifdef JAMERSON_GPIO
            ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), /* Device Properties for _DSD */
            Package()
            {
                Package()
                {
                    "spk-id-gpios", Package()
                    {
                        AF01, 0, 0, 0,
                    },
                },
            },
# endif
        }) // End _DSD
        #include <CS35L56_PDP.asl>

        Device(AF01)
        {
            Name(_ADR, 0x1)
            #define CHANNEL_LEFT
            #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
            #undef CHANNEL_LEFT
# ifdef JAMERSON_GPIO
            // Assign SPKR_ID GPIO for Jamerson
            Name (GPIA, ResourceTemplate ()
            {
                // Pin37 - GPIO15
                GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { JAMERSON_GPIO }
            })

            Method (_CRS, 0x0, NotSerialized)
            {
                Return (GPIA)
            }
# endif
        }
    }
#endif
#ifdef JAMERSON_2_LID
    // Jamerson 2 (UID_1, SNDW_0)
    Device (SWD3)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_2_LID << 48 )|(3<<44)|( JAMERSON_2_UID << 40 )|0x01FA355601) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamersons

# ifdef JAMERSON_GPIO
            ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), /* Device Properties for _DSD */
            Package()
            {
                Package()
                {
                    "spk-id-gpios", Package()
                    {
                        AF01, 0, 0, 0,
                    },
                },
            },
# endif
        }) // End _DSD
        #include <CS35L56_PDP.asl>

        Device(AF01)
        {
            Name(_ADR, 0x1)
            #define CHANNEL_LEFT
            #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
            #undef CHANNEL_LEFT
# ifdef JAMERSON_GPIO
            // Assign SPKR_ID GPIO for Jamerson
            Name (GPIA, ResourceTemplate ()
            {
                // Pin37 - GPIO15
                GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { JAMERSON_GPIO }
            })

            Method (_CRS, 0x0, NotSerialized)
            {
                Return (GPIA)
            }
# endif
        }
    }
#endif
#ifdef JAMERSON_3_LID
    // Jamerson 3 (UID_4, SNDW_1)
    Device (SWD4)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_3_LID << 48 )|(3<<44)|( JAMERSON_3_UID << 40 )|0x01FA355601) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamersons

# ifdef JAMERSON_GPIO
            ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), /* Device Properties for _DSD */
            Package()
            {
                Package()
                {
                    "spk-id-gpios", Package()
                    {
                        AF01, 0, 0, 0,
                    },
                },
            },
# endif
        }) // End _DSD
        #include <CS35L56_PDP.asl>

        Device(AF01)
        {
            Name(_ADR, 0x1)
            #define CHANNEL_RIGHT
            #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
            #undef CHANNEL_RIGHT
# ifdef JAMERSON_GPIO
            // Assign SPKR_ID GPIO for Jamerson
            Name (GPIA, ResourceTemplate ()
            {
                // Pin37 - GPIO15
                GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { JAMERSON_GPIO }
            })

            Method (_CRS, 0x0, NotSerialized)
            {
                Return (GPIA)
            }
# endif
        }
    }
#endif
#ifdef JAMERSON_4_LID
    // Jamerson 4 (UID_5, SNDW_1)
    Device (SWD5)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_4_LID << 48 )|(3<<44)|( JAMERSON_4_UID << 40 )|0x01FA355601) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamersons

# ifdef JAMERSON_GPIO
            ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), /* Device Properties for _DSD */
            Package()
            {
                Package()
                {
                    "spk-id-gpios", Package()
                    {
                        AF01, 0, 0, 0,
                    },
                },
            },
# endif
        }) // End _DSD
        #include <CS35L56_PDP.asl>

        Device(AF01)
        {
            Name(_ADR, 0x1)
            #define CHANNEL_RIGHT
            #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
            #undef CHANNEL_RIGHT
# ifdef JAMERSON_GPIO
            // Assign SPKR_ID GPIO for Jamerson
            Name (GPIA, ResourceTemplate ()
            {
                // Pin37 - GPIO15
                GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { JAMERSON_GPIO }
            })

            Method (_CRS, 0x0, NotSerialized)
            {
                Return (GPIA)
            }
# endif
        }
    }
#endif

#ifdef JAMERSON_5_LID
    // Jamerson 5 (UID_2, SNDW_0)
    Device (SWD7)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_5_LID << 48 )|(3<<44)|( JAMERSON_5_UID << 40 )|0x01FA355601) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamersons

# ifdef JAMERSON_GPIO
            ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), /* Device Properties for _DSD */
            Package()
            {
                Package()
                {
                    "spk-id-gpios", Package()
                    {
                        AF01, 0, 0, 0,
                    },
                },
            },
# endif
        }) // End _DSD
        #include <CS35L56_PDP.asl>

        Device(AF01)
        {
            Name(_ADR, 0x1)
            #define CHANNEL_LEFT
            #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
            #undef CHANNEL_LEFT
# ifdef JAMERSON_GPIO
            // Assign SPKR_ID GPIO for Jamerson
            Name (GPIA, ResourceTemplate ()
            {
                // Pin37 - GPIO15
                GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { JAMERSON_GPIO }
            })

            Method (_CRS, 0x0, NotSerialized)
            {
                Return (GPIA)
            }
# endif
        }
    }
#endif
#ifdef JAMERSON_6_LID
    // Jamerson 6 (UID_3, SNDW_1)
    Device (SWD8)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_6_LID << 48 )|(3<<44)|( JAMERSON_6_UID << 40 )|0x01FA355601) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamersons

# ifdef JAMERSON_GPIO
            ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), /* Device Properties for _DSD */
            Package()
            {
                Package()
                {
                    "spk-id-gpios", Package()
                    {
                        AF01, 0, 0, 0,
                    },
                },
            },
# endif
        }) // End _DSD
        #include <CS35L56_PDP.asl>

        Device(AF01)
        {
            Name(_ADR, 0x1)
            #define CHANNEL_RIGHT
            #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
            #undef CHANNEL_RIGHT
# ifdef JAMERSON_GPIO
            // Assign SPKR_ID GPIO for Jamerson
            Name (GPIA, ResourceTemplate ()
            {
                // Pin37 - GPIO15
                GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { JAMERSON_GPIO }
            })

            Method (_CRS, 0x0, NotSerialized)
            {
                Return (GPIA)
            }
# endif
        }
    }
#endif
#ifdef COHEN_1_LID
    // Cohen UAJ [UID_0, SNDW_3]
    Device (SWD6)
    {
        Name (_ADR, ((0x000<<52)|( COHEN_1_LID << 48 )|(3<<44)|( COHEN_1_UID << 40 )|0x01FA424301) )  // _ADR: Address

        #include <CS42L43_DSD.asl>

#ifdef COHEN_AMP
        Device (AF01)
        {
            Name (_ADR, 0x1) // SDCA Function Number = 0x1
            #define CHANNEL_LEFT
            #define CHANNEL_RIGHT
            #include <Cohen_Amp.asl>
            #undef CHANNEL_LEFT
            #undef CHANNEL_RIGHT
        }
#endif
        Device (AF02)
        {
            Name (_ADR, 0x2) // SDCA Function Number = 0x2
            #include <Cohen_SimpleMic.asl>
        }

        Device (AF03)
        {
            Name (_ADR, 0x3) // SDCA Hierarchical Function Number = 0x3
            #include <Cohen_UAJ.asl>
            #include <Cohen_UAJ_capture_enable.asl>
        }

        Device (AF04)
        {
            Name (_ADR, 0x4) // SDCA Hierarchical Function Number = 0x4
            #include <CS_HID_headset_buttons.asl>
        }
    }
#endif
}
