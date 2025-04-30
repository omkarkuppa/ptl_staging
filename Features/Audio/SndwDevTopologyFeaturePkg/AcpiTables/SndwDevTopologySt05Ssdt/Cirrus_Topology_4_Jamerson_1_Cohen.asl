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

#ifndef JAMERSON_1_HWID
#define JAMERSON_1_HWID 0x3556
#endif
#ifndef JAMERSON_2_HWID
#define JAMERSON_2_HWID 0x3556
#endif
#ifndef JAMERSON_3_HWID
#define JAMERSON_3_HWID 0x3556
#endif
#ifndef JAMERSON_4_HWID
#define JAMERSON_4_HWID 0x3556
#endif

Scope (_SB)
{
    Device (AUDC) // MS Audio compositor driver
    {
        Name (_HID, "ACPI0018") // INF in MS audio compositor driver looks for this HWID (UEFI-defined for audio comp.)
        // #include is used so that the preprocessor is run on the contents of the file.
        #include <AudioComposition_Jamerson_4spk_Cohen_mic_UAJ_all_endpoints.asl>
        #include <AudioComposition_Jamerson_4spk_Cohen_mic_UAJ.asl>
    }
}

Scope (_SB.PC00.HDAS.IDA.SNDW)
{
#ifdef JAMERSON_1_LID
    // Jamerson 1
    Device (SWD2)
    {
        Name (_ADR, ((0x000<<52)|(JAMERSON_1_LID << 48)|(3<<44)|(JAMERSON_1_UID << 40)|(JAMERSON_1_HWID << 8)|0x01FA000001) )  // _ADR: Address

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

# ifdef JAMERSON_1_TOP_LEFT
        // JAMERSON_1 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !JAMERSON_1_TOP_LEFT
#  ifdef JAMERSON_1_BOTTOM_LEFT
        // JAMERSON_1 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !JAMERSON_1_BOTTOM_LEFT
#   ifdef JAMERSON_1_TOP_RIGHT
        // JAMERSON_1 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !JAMERSON_1_TOP_RIGHT
#    ifdef JAMERSON_1_BOTTOM_RIGHT
        // JAMERSON_1 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !JAMERSON_1_BOTTOM_RIGHT
#     ifdef JAMERSON_1_RIGHT
        // JAMERSON_1 is RIGHT channel.
#      define CHANNEL_RIGHT
#     else  // !JAMERSON_1_RIGHT
        // By default JAMERSON_1 is LEFT channel.
#      define CHANNEL_LEFT
#     endif // JAMERSON_1_RIGHT
#    endif  // JAMERSON_1_BOTTOM_RIGHT
#   endif   // JAMERSON_1_TOP_RIGHT
#  endif    // JAMERSON_1_BOTTOM_LEFT
# endif     // JAMERSON_1_TOP_LEFT

            #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>

#ifdef CHANNEL_LEFT
# undef CHANNEL_LEFT
#endif
#ifdef CHANNEL_RIGHT
# undef CHANNEL_RIGHT
#endif
#ifdef CHANNEL_BOTTOM_RIGHT
# undef CHANNEL_BOTTOM_RIGHT
#endif
#ifdef CHANNEL_TOP_RIGHT
# undef CHANNEL_TOP_RIGHT
#endif
#ifdef CHANNEL_BOTTOM_LEFT
# undef CHANNEL_BOTTOM_LEFT
#endif
#ifdef CHANNEL_TOP_LEFT
# undef CHANNEL_TOP_LEFT
#endif

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
    // Jamerson 2
    Device (SWD3)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_2_LID << 48 )|(3<<44)|( JAMERSON_2_UID << 40 )|(JAMERSON_2_HWID << 8)|0x01FA000001) )  // _ADR: Address

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

# ifdef JAMERSON_2_TOP_LEFT
        // JAMERSON_2 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !JAMERSON_2_TOP_LEFT
#  ifdef JAMERSON_2_BOTTOM_LEFT
        // JAMERSON_2 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !JAMERSON_2_BOTTOM_LEFT
#   ifdef JAMERSON_2_TOP_RIGHT
        // JAMERSON_2 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !JAMERSON_2_TOP_RIGHT
#    ifdef JAMERSON_2_BOTTOM_RIGHT
        // JAMERSON_2 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !JAMERSON_2_BOTTOM_RIGHT
#     ifdef JAMERSON_2_RIGHT
        // JAMERSON_2 is RIGHT channel.
#      define CHANNEL_RIGHT
#     else  // !JAMERSON_2_RIGHT
        // By default JAMERSON_2 is LEFT channel.
#      define CHANNEL_LEFT
#     endif // JAMERSON_2_RIGHT
#    endif  // JAMERSON_2_BOTTOM_RIGHT
#   endif   // JAMERSON_2_TOP_RIGHT
#  endif    // JAMERSON_2_BOTTOM_LEFT
# endif     // JAMERSON_2_TOP_LEFT

            #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>

#ifdef CHANNEL_LEFT
# undef CHANNEL_LEFT
#endif
#ifdef CHANNEL_RIGHT
# undef CHANNEL_RIGHT
#endif
#ifdef CHANNEL_BOTTOM_RIGHT
# undef CHANNEL_BOTTOM_RIGHT
#endif
#ifdef CHANNEL_TOP_RIGHT
# undef CHANNEL_TOP_RIGHT
#endif
#ifdef CHANNEL_BOTTOM_LEFT
# undef CHANNEL_BOTTOM_LEFT
#endif
#ifdef CHANNEL_TOP_LEFT
# undef CHANNEL_TOP_LEFT
#endif

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
    // Jamerson 3
    Device (SWD4)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_3_LID << 48 )|(3<<44)|( JAMERSON_3_UID << 40 )|(JAMERSON_3_HWID << 8)|0x01FA000001) )  // _ADR: Address

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

# ifdef JAMERSON_3_TOP_LEFT
        // JAMERSON_3 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !JAMERSON_3_TOP_LEFT
#  ifdef JAMERSON_3_BOTTOM_LEFT
        // JAMERSON_3 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !JAMERSON_3_BOTTOM_LEFT
#   ifdef JAMERSON_3_TOP_RIGHT
        // JAMERSON_3 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !JAMERSON_3_TOP_RIGHT
#    ifdef JAMERSON_3_BOTTOM_RIGHT
        // JAMERSON_3 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !JAMERSON_3_BOTTOM_RIGHT
#     ifdef JAMERSON_3_LEFT
        // JAMERSON_3 is LEFT channel.
#      define CHANNEL_LEFT
#     else  // !JAMERSON_3_LEFT
        // By default JAMERSON_3 is RIGHT channel.
#      define CHANNEL_RIGHT
#     endif // JAMERSON_3_LEFT
#    endif  // JAMERSON_3_BOTTOM_RIGHT
#   endif   // JAMERSON_3_TOP_RIGHT
#  endif    // JAMERSON_3_BOTTOM_LEFT
# endif     // JAMERSON_3_TOP_LEFT

            #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>

#ifdef CHANNEL_LEFT
# undef CHANNEL_LEFT
#endif
#ifdef CHANNEL_RIGHT
# undef CHANNEL_RIGHT
#endif
#ifdef CHANNEL_BOTTOM_RIGHT
# undef CHANNEL_BOTTOM_RIGHT
#endif
#ifdef CHANNEL_TOP_RIGHT
# undef CHANNEL_TOP_RIGHT
#endif
#ifdef CHANNEL_BOTTOM_LEFT
# undef CHANNEL_BOTTOM_LEFT
#endif
#ifdef CHANNEL_TOP_LEFT
# undef CHANNEL_TOP_LEFT
#endif

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
    // Jamerson 4
    Device (SWD5)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_4_LID << 48 )|(3<<44)|( JAMERSON_4_UID << 40 )|(JAMERSON_4_HWID << 8)|0x01FA000001) )  // _ADR: Address

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

# ifdef JAMERSON_4_TOP_LEFT
        // JAMERSON_4 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !JAMERSON_4_TOP_LEFT
#  ifdef JAMERSON_4_BOTTOM_LEFT
        // JAMERSON_4 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !JAMERSON_4_BOTTOM_LEFT
#   ifdef JAMERSON_4_TOP_RIGHT
        // JAMERSON_4 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !JAMERSON_4_TOP_RIGHT
#    ifdef JAMERSON_4_BOTTOM_RIGHT
        // JAMERSON_4 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !JAMERSON_4_BOTTOM_RIGHT
#     ifdef JAMERSON_4_LEFT
        // JAMERSON_4 is LEFT channel.
#      define CHANNEL_LEFT
#     else  // !JAMERSON_4_LEFT
        // By default JAMERSON_4 is RIGHT channel.
#      define CHANNEL_RIGHT
#     endif // JAMERSON_4_LEFT
#    endif  // JAMERSON_4_BOTTOM_RIGHT
#   endif   // JAMERSON_4_TOP_RIGHT
#  endif    // JAMERSON_4_BOTTOM_LEFT
# endif     // JAMERSON_4_TOP_LEFT

            #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>

#ifdef CHANNEL_LEFT
# undef CHANNEL_LEFT
#endif
#ifdef CHANNEL_RIGHT
# undef CHANNEL_RIGHT
#endif
#ifdef CHANNEL_BOTTOM_RIGHT
# undef CHANNEL_BOTTOM_RIGHT
#endif
#ifdef CHANNEL_TOP_RIGHT
# undef CHANNEL_TOP_RIGHT
#endif
#ifdef CHANNEL_BOTTOM_LEFT
# undef CHANNEL_BOTTOM_LEFT
#endif
#ifdef CHANNEL_TOP_LEFT
# undef CHANNEL_TOP_LEFT
#endif

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

#ifdef COHEN_BRIDGE
 #ifdef SIDECAR_GPIO_SPEAKER_SELECT
            Name (GPIA, ResourceTemplate () {
                SIDECAR_GPIO_RESOURCE_DESCRIPTOR
            })
            Method (_CRS, 0x0, NotSerialized) {
                Return (GPIA)
            }
 #endif

 #ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
            // _INI method: Set 01fa-spk-id-val to the value specified in
            // SIDECAR_VARIABLE_SPEAKER_SELECT_NAME. This can either be defined
            // as the four-character constant of a field within an OpRegion (for
            // instance, of the corresponding BIOS variable set by the ODM), or,
            // for testing purposes, a constant value.
            //
            // Examples:
            //   - Source the value from a field in an OpRegion named 'SIDV'
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     SIDV
            //   - Source the value from the return value of a function
            //     defined elsewhere in the ASL
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     SIDV()
            //   - Use 1 as the hardcoded value for the property
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     0x01
            Method (_INI, 0x0, NotSerialized) {
                Local1 = SIDECAR_VARIABLE_SPEAKER_SELECT_NAME
                //            +-- reference to the value part of the '01fa-spk-id-val' property package
                //            |     +-- First package inside EXT0 inner package
                //            |     |   (should be the '01fa-spk-id-val' package)
                //            |     |             +-- inner package within EXT0
                //            |     |             |
                //            |     |             v--------------------v
                //            |     v----------------------------------------v
                //            v---------------------------------------------------v
                Store(Local1, Index(DerefOf(Index(DerefOf(Index(EXT0, 1)), 0)), 1))
            }
 #endif
#endif
        }
#endif

        Device (AF02)
        {
            Name (_ADR, 0x2) // SDCA Function Number = 0x2
            #include <Cohen_SimpleMic.asl>

#ifdef COHEN_BRIDGE
 #ifdef SIDECAR_GPIO_SPEAKER_SELECT
            Name (GPIA, ResourceTemplate () {
                SIDECAR_GPIO_RESOURCE_DESCRIPTOR
            })
            Method (_CRS, 0x0, NotSerialized) {
                Return (GPIA)
            }
 #endif

 #ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
            // _INI method: Set 01fa-spk-id-val to the value specified in
            // SIDECAR_VARIABLE_SPEAKER_SELECT_NAME. This can either be defined
            // as the four-character constant of a field within an OpRegion (for
            // instance, of the corresponding BIOS variable set by the ODM), or,
            // for testing purposes, a constant value.
            //
            // Examples:
            //   - Source the value from a field in an OpRegion named 'SIDV'
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     SIDV
            //   - Use 1 as the hardcoded value for the property
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     0x01
            Method (_INI, 0x0, NotSerialized) {
                Local1 = SIDECAR_VARIABLE_SPEAKER_SELECT_NAME
                // See _INI method in AF01 for explanation of each part of this
                // expression
                Store(Local1, Index(DerefOf(Index(DerefOf(Index(EXT0, 1)), 0)), 1))
            }
 #endif
#endif
        }

        Device (AF03)
        {
            Name (_ADR, 0x3) // SDCA Hierarchical Function Number = 0x3
            #include <Cohen_UAJ.asl>
            #include <Cohen_UAJ_capture_enable.asl>

#ifdef COHEN_BRIDGE
 #ifdef SIDECAR_GPIO_SPEAKER_SELECT
            Name (GPIA, ResourceTemplate () {
                SIDECAR_GPIO_RESOURCE_DESCRIPTOR
            })
            Method (_CRS, 0x0, NotSerialized) {
                Return (GPIA)
            }
 #endif

 #ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
            // _INI method: Set 01fa-spk-id-val to the value specified in
            // SIDECAR_VARIABLE_SPEAKER_SELECT_NAME. This can either be defined
            // as the four-character constant of a field within an OpRegion (for
            // instance, of the corresponding BIOS variable set by the ODM), or,
            // for testing purposes, a constant value.
            //
            // Examples:
            //   - Source the value from a field in an OpRegion named 'SIDV'
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     SIDV
            //   - Use 1 as the hardcoded value for the property
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     0x01
            Method (_INI, 0x0, NotSerialized) {
                Local1 = SIDECAR_VARIABLE_SPEAKER_SELECT_NAME
                // See _INI method in AF01 for explanation of each part of this
                // expression
                Store(Local1, Index(DerefOf(Index(DerefOf(Index(EXT0, 1)), 0)), 1))
            }
 #endif
#endif
        }

        Device (AF04)
        {
            Name (_ADR, 0x4) // SDCA Hierarchical Function Number = 0x4
            #include <CS_HID_headset_buttons.asl>
        }
    }
#endif
}
