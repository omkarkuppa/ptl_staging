/** @file
  SoundWire topology definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifdef AMP1_UID
# ifndef AMP1_LID
#  error "AMP1 link ID undefined!"
# endif
# ifndef AMP1_MANUFACTURER_ID
#  error "AMP1 manufacturer ID undefined!"
# endif
# ifndef AMP1_PART_ID
#  error "AMP1 part ID undefined!"
# endif
# ifndef AMP1_FUNC_ID
#  error "AMP1 function ID undefined!"
# endif
# ifndef AMP1_AMP_DEV_NAME
#  error "AMP1 AMP device string undefined!"
# endif
#endif

#ifdef AMP2_UID
# ifndef AMP2_LID
#  error "AMP2 link ID undefined!"
# endif
# ifndef AMP2_MANUFACTURER_ID
#  error "AMP2 manufacturer ID undefined!"
# endif
# ifndef AMP2_PART_ID
#  error "AMP2 part ID undefined!"
# endif
# ifndef AMP2_FUNC_ID
#  error "AMP2 function ID undefined!"
# endif
# ifndef AMP2_AMP_DEV_NAME
#  error "AMP2 AMP device string undefined!"
# endif
# ifndef AMP1_UID
#  error "AMP1 undefined!"
# endif
#endif

#ifdef AMP3_UID
# ifndef AMP3_LID
#  error "AMP3 link ID undefined!"
# endif
# ifndef AMP3_MANUFACTURER_ID
#  error "AMP3 manufacturer ID undefined!"
# endif
# ifndef AMP3_PART_ID
#  error "AMP3 part ID undefined!"
# endif
# ifndef AMP3_FUNC_ID
#  error "AMP3 function ID undefined!"
# endif
# ifndef AMP3_AMP_DEV_NAME
#  error "AMP3 AMP device string undefined!"
# endif
# ifndef AMP2_UID
#  error "AMP2 undefined!"
# endif
#endif

#ifdef AMP4_UID
# ifndef AMP4_LID
#  error "AMP4 link ID undefined!"
# endif
# ifndef AMP4_MANUFACTURER_ID
#  error "AMP4 manufacturer ID undefined!"
# endif
# ifndef AMP4_PART_ID
#  error "AMP4 part ID undefined!"
# endif
# ifndef AMP4_FUNC_ID
#  error "AMP4 function ID undefined!"
# endif
# ifndef AMP4_AMP_DEV_NAME
#  error "AMP4 AMP device string undefined!"
# endif
# ifndef AMP3_UID
#  error "AMP3 undefined!"
# endif
#endif

#ifdef AMP5_UID
# ifndef AMP5_LID
#  error "AMP5 link ID undefined!"
# endif
# ifndef AMP5_MANUFACTURER_ID
#  error "AMP5 manufacturer ID undefined!"
# endif
# ifndef AMP5_PART_ID
#  error "AMP5 part ID undefined!"
# endif
# ifndef AMP5_FUNC_ID
#  error "AMP5 function ID undefined!"
# endif
# ifndef AMP5_AMP_DEV_NAME
#  error "AMP5 AMP device string undefined!"
# endif
# ifndef AMP4_UID
#  error "AMP4 undefined!"
# endif
#endif

#ifdef AMP6_UID
# ifndef AMP6_LID
#  error "AMP6 link ID undefined!"
# endif
# ifndef AMP6_MANUFACTURER_ID
#  error "AMP6 manufacturer ID undefined!"
# endif
# ifndef AMP6_PART_ID
#  error "AMP6 part ID undefined!"
# endif
# ifndef AMP6_FUNC_ID
#  error "AMP6 function ID undefined!"
# endif
# ifndef AMP6_AMP_DEV_NAME
#  error "AMP6 AMP device string undefined!"
# endif
# ifndef AMP5_UID
#  error "AMP5 undefined!"
# endif
#endif

#ifdef AMP7_UID
# ifndef AMP7_LID
#  error "AMP7 link ID undefined!"
# endif
# ifndef AMP7_MANUFACTURER_ID
#  error "AMP7 manufacturer ID undefined!"
# endif
# ifndef AMP7_PART_ID
#  error "AMP7 part ID undefined!"
# endif
# ifndef AMP7_FUNC_ID
#  error "AMP7 function ID undefined!"
# endif
# ifndef AMP7_AMP_DEV_NAME
#  error "AMP7 AMP device string undefined!"
# endif
# ifndef AMP6_UID
#  error "AMP6 undefined!"
# endif
#endif

#ifdef AMP8_UID
# ifndef AMP8_LID
#  error "AMP8 link ID undefined!"
# endif
# ifndef AMP8_MANUFACTURER_ID
#  error "AMP8 manufacturer ID undefined!"
# endif
# ifndef AMP8_PART_ID
#  error "AMP8 part ID undefined!"
# endif
# ifndef AMP8_FUNC_ID
#  error "AMP8 function ID undefined!"
# endif
# ifndef AMP8_AMP_DEV_NAME
#  error "AMP8 AMP device string undefined!"
# endif
# ifndef AMP7_UID
#  error "AMP7 undefined!"
# endif
#endif

#ifdef CODEC1_UID
# ifndef CODEC1_LID
#  error "CODEC1 link ID undefined!"
# endif
# ifndef CODEC1_MANUFACTURER_ID
#  error "CODEC1 manufacturer ID undefined!"
# endif
# ifndef CODEC1_PART_ID
#  error "CODEC1 part ID undefined!"
# endif
# ifndef CODEC1_FUNC_ID
#  error "CODEC1 function ID undefined!"
# endif
# ifdef CODEC1_AMP
#  ifndef CODEC1_AMP_DEV_NAME
#   error "CODEC1 AMP device string undefined!"
#  endif
# endif
# ifdef CODEC1_MIC
#  ifndef CODEC1_MIC_DEV_NAME
#   error "CODEC1 MIC device string undefined!"
#  endif
# endif
# ifdef CODEC1_UAJ
#  ifndef CODEC1_UAJ_DEV_NAME
#   error "CODEC1 UAJ device string undefined!"
#  endif
# endif
#endif

#ifndef DSP_ACPI_ACD_DEVICE_NAMESTRING
#  error "DSP device string undefined!"
#endif

Scope (_SB)
{
    //Name (AUSW, 3)  // Emulates BIOS variable. bit0 - mic enable, bit1 - spk enable
    //
    //   Endpoint/AUSW     | 0 | 1 | 2 | 3 |
    // --------------------+---+---+---+---+
    // Aggregated Speaker  | x | x | o | o |
    // --------------------+---+---+---+---+
    // Internal Microphone | x | o | x | o |
    // --------------------+---+---+---+---+
    // Headphones          | o | o | o | o |
    // --------------------+---+---+---+---+
    // LineOut             | o | o | o | o |
    // --------------------+---+---+---+---+
    // HeadsetOutput*      | o | o | o | o |
    // --------------------+---+---+---+---+
    // Stereo Microphone   | x | o | x | o |
    // --------------------+---+---+---+---+
    // LineIn              | x | o | x | o |
    // --------------------+---+---+---+---+
    // HeadsetMic          | x | o | x | o |
    // --------------------+---+---+---+---+
    //
    // x - not enabled
    // o - enabled
    // * - presented as Headphones when HeadsetMic is disabled
    //

    //Name (DMSW, 1) // Emulates BIOS variable. bit0 - dmic mic enable
    //   Endpoint/DMSW     | 0 | 1 |
    // --------------------+---+---+
    // Internal Microphone | x | o |

    If (LEqual(AUSW, 3))    // all endpoints are present
    {
        Device (AUDC) // MS Audio compositor driver
        {
            Name (_HID, "ACPI0018") // INF in MS audio compositor driver looks for this HWID (UEFI-defined for audio comp.)
            // #include is used so that the preprocessor is run on the contents of the file.
            #include <AudioComposition_8xAMP_1xCODEC.asl>
            #include <AudioComposition_8xAMP_1xCODEC_all_endpoints.asl>
        }
    }
    ElseIf (LEqual(AUSW, 2))    // no capture endpoints are present
    {
        Device (AUC2) // MS Audio compositor driver
        {
            Name (_HID, "ACPI0018") // INF in MS audio compositor driver looks for this HWID (UEFI-defined for audio comp.)
            #include <AudioComposition_8xAMP_1xCODEC.asl>
            #include <AudioComposition_8xAMP_1xCODEC_no_capture.asl>
        }
    }
    ElseIf (LEqual(AUSW, 1))    // no built-in speaker endpoint is present
    {
        Device (AUC1) // MS Audio compositor driver
        {
            Name (_HID, "ACPI0018") // INF in MS audio compositor driver looks for this HWID (UEFI-defined for audio comp.)
            #include <AudioComposition_8xAMP_1xCODEC.asl>
            #include <AudioComposition_8xAMP_1xCODEC_no_spk.asl>
        }
    }
    Else    // no built-in speaker and no capture endpoints are present
    {
        Device (AUC0) // MS Audio compositor driver
        {
            Name (_HID, "ACPI0018") // INF in MS audio compositor driver looks for this HWID (UEFI-defined for audio comp.)
            #include <AudioComposition_8xAMP_1xCODEC.asl>
            #include <AudioComposition_8xAMP_1xCODEC_no_capture_no_spk.asl>
        }
    }
}
#ifndef _AMD
Scope (_SB.PC00.HDAS.IDA.SNDW)
{
#endif
#ifdef AMP1_UID
    // AMP 1
    Device (SWD2)
    {
        Name (_ADR, ((0x000<<52)|(AMP1_LID << 48)|(3<<44)|(AMP1_UID << 40)|(AMP1_MANUFACTURER_ID << 24)|(AMP1_PART_ID << 8)|0x1) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
#if AMP1_MANUFACTURER_ID==0x01FA
# if AMP1_PART_ID==0x3556
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP1_PART_ID==0x3557
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP1_PART_ID==0x3563
            #include <CS35L63_DSD.asl>  // For additional _DSD entries, common to all Buffetts
# endif
#endif

# ifdef AMP_GPIO
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

#if AMP1_MANUFACTURER_ID==0x01FA
# if AMP1_PART_ID==0x3556
        #include <CS35L56_PDP.asl>
# elif AMP1_PART_ID==0x3557
        #include <CS35L56_PDP.asl>
# elif AMP1_PART_ID==0x3563
        #include <CS35L63_PDP.asl>
# endif
#endif

        Method(_STA, 0, Serialized)
        {
            And(AUSW, 2, Local0)
            If(LEqual(Local0, 2))   // bit1 - spk enabled
            {
                return (0xf)    // show in UI
            }
            return (0xb)        // do not show in UI
        }

        If (LOr(LEqual(AUSW, 3), LEqual(AUSW, 2)))
        {
            Device(AF01)
            {
                Name(_ADR, 0x1)

# ifdef AMP1_TOP_LEFT
        // AMP1 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !AMP1_TOP_LEFT
#  ifdef AMP1_BOTTOM_LEFT
        // AMP1 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !AMP1_BOTTOM_LEFT
#   ifdef AMP1_TOP_RIGHT
        // AMP1 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !AMP1_TOP_RIGHT
#    ifdef AMP1_BOTTOM_RIGHT
        // AMP1 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !AMP1_BOTTOM_RIGHT
#     ifdef AMP1_RIGHT
        // AMP1 is RIGHT channel.
#      define CHANNEL_RIGHT
#     else  // !AMP1_RIGHT
        // By default AMP1 is LEFT channel.
#      define CHANNEL_LEFT
#     endif // AMP1_RIGHT
#    endif  // AMP1_BOTTOM_RIGHT
#   endif   // AMP1_TOP_RIGHT
#  endif    // AMP1_BOTTOM_LEFT
# endif     // AMP1_TOP_LEFT

#if AMP1_MANUFACTURER_ID==0x01FA
# if AMP1_FUNC_ID==0x3556
                #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
# elif AMP1_FUNC_ID==0x3563
                #include <CS35L63_AMP.asl>
# endif
#endif

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

# ifdef AMP_GPIO
                // Assign SPKR_ID GPIO for AMP
                Name (GPIA, ResourceTemplate ()
                {
                    // Pin37 - GPIO15
                    GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { AMP_GPIO }
                })

                Method (_CRS, 0x0, NotSerialized)
                {
                    Return (GPIA)
                }
# endif
            }
        }
    }
#endif  // AMP1_UID

#ifdef AMP2_UID
    // AMP 2
    Device (SWD3)
    {
        Name (_ADR, ((0x000<<52)|(AMP2_LID << 48)|(3<<44)|(AMP2_UID << 40)|(AMP2_MANUFACTURER_ID << 24)|(AMP2_PART_ID << 8)|0x1) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
#if AMP2_MANUFACTURER_ID==0x01FA
# if AMP2_PART_ID==0x3556
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP2_PART_ID==0x3557
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP2_PART_ID==0x3563
            #include <CS35L63_DSD.asl>  // For additional _DSD entries, common to all Buffetts
# endif
#endif

# ifdef AMP_GPIO
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

#if AMP2_MANUFACTURER_ID==0x01FA
# if AMP2_PART_ID==0x3556
        #include <CS35L56_PDP.asl>
# elif AMP2_PART_ID==0x3557
        #include <CS35L56_PDP.asl>
# elif AMP2_PART_ID==0x3563
        #include <CS35L63_PDP.asl>
# endif
#endif

        Method(_STA, 0, Serialized)
        {
            And(AUSW, 2, Local0)
            If(LEqual(Local0, 2))   // bit1 - spk enabled
            {
                return (0xf)    // show in UI
            }
            return (0xb)        // do not show in UI
        }

        If (LOr(LEqual(AUSW, 3), LEqual(AUSW, 2)))
        {
            Device(AF01)
            {
                Name(_ADR, 0x1)

# ifdef AMP2_TOP_LEFT
        // AMP2 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !AMP2_TOP_LEFT
#  ifdef AMP2_BOTTOM_LEFT
        // AMP2 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !AMP2_BOTTOM_LEFT
#   ifdef AMP2_TOP_RIGHT
        // AMP2 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !AMP2_TOP_RIGHT
#    ifdef AMP2_BOTTOM_RIGHT
        // AMP2 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !AMP2_BOTTOM_RIGHT
#     ifdef AMP2_RIGHT
        // AMP2 is RIGHT channel.
#      define CHANNEL_RIGHT
#     else  // !AMP2_RIGHT
        // By default AMP2 is LEFT channel.
#      define CHANNEL_LEFT
#     endif // AMP2_RIGHT
#    endif  // AMP2_BOTTOM_RIGHT
#   endif   // AMP2_TOP_RIGHT
#  endif    // AMP2_BOTTOM_LEFT
# endif     // AMP2_TOP_LEFT

#if AMP2_MANUFACTURER_ID==0x01FA
# if AMP2_FUNC_ID==0x3556
                #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
# elif AMP2_FUNC_ID==0x3563
                #include <CS35L63_AMP.asl>
# endif
#endif

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

# ifdef AMP_GPIO
                // Assign SPKR_ID GPIO for AMP
                Name (GPIA, ResourceTemplate ()
                {
                    // Pin37 - GPIO15
                    GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { AMP_GPIO }
                })

                Method (_CRS, 0x0, NotSerialized)
                {
                    Return (GPIA)
                }
# endif
            }
        }
    }
#endif  // AMP2_UID

#ifdef AMP3_UID
    // AMP 3
    Device (SWD4)
    {
        Name (_ADR, ((0x000<<52)|(AMP3_LID << 48)|(3<<44)|(AMP3_UID << 40)|(AMP3_MANUFACTURER_ID << 24)|(AMP3_PART_ID << 8)|0x1) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
#if AMP3_MANUFACTURER_ID==0x01FA
# if AMP3_PART_ID==0x3556
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP3_PART_ID==0x3557
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP3_PART_ID==0x3563
            #include <CS35L63_DSD.asl>  // For additional _DSD entries, common to all Buffetts
# endif
#endif

# ifdef AMP_GPIO
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

#if AMP3_MANUFACTURER_ID==0x01FA
# if AMP3_PART_ID==0x3556
        #include <CS35L56_PDP.asl>
# elif AMP3_PART_ID==0x3557
        #include <CS35L56_PDP.asl>
# elif AMP3_PART_ID==0x3563
        #include <CS35L63_PDP.asl>
# endif
#endif

        Method(_STA, 0, Serialized)
        {
            And(AUSW, 2, Local0)
            If(LEqual(Local0, 2))   // bit1 - spk enabled
            {
                return (0xf)    // show in UI
            }
            return (0xb)        // do not show in UI
        }

        If (LOr(LEqual(AUSW, 3), LEqual(AUSW, 2)))
        {
            Device(AF01)
            {
                Name(_ADR, 0x1)

# ifdef AMP3_TOP_LEFT
        // AMP3 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !AMP3_TOP_LEFT
#  ifdef AMP3_BOTTOM_LEFT
        // AMP3 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !AMP3_BOTTOM_LEFT
#   ifdef AMP3_TOP_RIGHT
        // AMP3 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !AMP3_TOP_RIGHT
#    ifdef AMP3_BOTTOM_RIGHT
        // AMP3 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !AMP3_BOTTOM_RIGHT
#     ifdef AMP3_LEFT
        // AMP3 is LEFT channel.
#      define CHANNEL_LEFT
#     else  // !AMP3_LEFT
        // By default AMP3 is RIGHT channel.
#      define CHANNEL_RIGHT
#     endif // AMP3_LEFT
#    endif  // AMP3_BOTTOM_RIGHT
#   endif   // AMP3_TOP_RIGHT
#  endif    // AMP3_BOTTOM_LEFT
# endif     // AMP3_TOP_LEFT


#if AMP3_MANUFACTURER_ID==0x01FA
# if AMP3_FUNC_ID==0x3556
                #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
# elif AMP3_FUNC_ID==0x3563
                #include <CS35L63_AMP.asl>
# endif
#endif

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

# ifdef AMP_GPIO
                // Assign SPKR_ID GPIO for AMP
                Name (GPIA, ResourceTemplate ()
                {
                    // Pin37 - GPIO15
                    GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { AMP_GPIO }
                })

                Method (_CRS, 0x0, NotSerialized)
                {
                    Return (GPIA)
                }
# endif
            }
        }
    }
#endif  // AMP3_UID

#ifdef AMP4_UID
    // AMP 4
    Device (SWD5)
    {
        Name (_ADR, ((0x000<<52)|(AMP4_LID << 48)|(3<<44)|(AMP4_UID << 40)|(AMP4_MANUFACTURER_ID << 24)|(AMP4_PART_ID << 8)|0x1) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
#if AMP4_MANUFACTURER_ID==0x01FA
# if AMP4_PART_ID==0x3556
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP4_PART_ID==0x3557
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP4_PART_ID==0x3563
            #include <CS35L63_DSD.asl>  // For additional _DSD entries, common to all Buffetts
# endif
#endif

# ifdef AMP_GPIO
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

#if AMP4_MANUFACTURER_ID==0x01FA
# if AMP4_PART_ID==0x3556
        #include <CS35L56_PDP.asl>
# elif AMP4_PART_ID==0x3557
        #include <CS35L56_PDP.asl>
# elif AMP4_PART_ID==0x3563
        #include <CS35L63_PDP.asl>
# endif
#endif

        Method(_STA, 0, Serialized)
        {
            And(AUSW, 2, Local0)
            If(LEqual(Local0, 2))   // bit1 - spk enabled
            {
                return (0xf)    // show in UI
            }
            return (0xb)        // do not show in UI
        }

        If (LOr(LEqual(AUSW, 3), LEqual(AUSW, 2)))
        {
            Device(AF01)
            {
                Name(_ADR, 0x1)

# ifdef AMP4_TOP_LEFT
        // AMP4 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !AMP4_TOP_LEFT
#  ifdef AMP4_BOTTOM_LEFT
        // AMP4 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !AMP4_BOTTOM_LEFT
#   ifdef AMP4_TOP_RIGHT
        // AMP4 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !AMP4_TOP_RIGHT
#    ifdef AMP4_BOTTOM_RIGHT
        // AMP4 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !AMP4_BOTTOM_RIGHT
#     ifdef AMP4_LEFT
        // AMP4 is LEFT channel.
#      define CHANNEL_LEFT
#     else  // !AMP4_LEFT
        // By default AMP4 is RIGHT channel.
#      define CHANNEL_RIGHT
#     endif // AMP4_LEFT
#    endif  // AMP4_BOTTOM_RIGHT
#   endif   // AMP4_TOP_RIGHT
#  endif    // AMP4_BOTTOM_LEFT
# endif     // AMP4_TOP_LEFT

#if AMP4_MANUFACTURER_ID==0x01FA
# if AMP4_FUNC_ID==0x3556
                #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
# elif AMP4_FUNC_ID==0x3563
                #include <CS35L63_AMP.asl>
# endif
#endif

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

# ifdef AMP_GPIO
                // Assign SPKR_ID GPIO for AMP
                Name (GPIA, ResourceTemplate ()
                {
                    // Pin37 - GPIO15
                    GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { AMP_GPIO }
                })

                Method (_CRS, 0x0, NotSerialized)
                {
                    Return (GPIA)
                }
# endif
            }
        }
    }
#endif  // AMP4_UID

#ifdef AMP5_UID
    // AMP 1
    Device (SWD7)
    {
        Name (_ADR, ((0x000<<52)|(AMP5_LID << 48)|(3<<44)|(AMP5_UID << 40)|(AMP5_MANUFACTURER_ID << 24)|(AMP5_PART_ID << 8)|0x1) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
#if AMP5_MANUFACTURER_ID==0x01FA
# if AMP5_PART_ID==0x3556
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP5_PART_ID==0x3557
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP5_PART_ID==0x3563
            #include <CS35L63_DSD.asl>  // For additional _DSD entries, common to all Buffetts
# endif
#endif

# ifdef AMP_GPIO
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

#if AMP5_MANUFACTURER_ID==0x01FA
# if AMP5_PART_ID==0x3556
        #include <CS35L56_PDP.asl>
# elif AMP5_PART_ID==0x3557
        #include <CS35L56_PDP.asl>
# elif AMP5_PART_ID==0x3563
        #include <CS35L63_PDP.asl>
# endif
#endif

        Method(_STA, 0, Serialized)
        {
            And(AUSW, 2, Local0)
            If(LEqual(Local0, 2))   // bit1 - spk enabled
            {
                return (0xf)    // show in UI
            }
            return (0xb)        // do not show in UI
        }

        If (LOr(LEqual(AUSW, 3), LEqual(AUSW, 2)))
        {
            Device(AF01)
            {
                Name(_ADR, 0x1)

# ifdef AMP5_TOP_LEFT
        // AMP5 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !AMP5_TOP_LEFT
#  ifdef AMP5_BOTTOM_LEFT
        // AMP5 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !AMP5_BOTTOM_LEFT
#   ifdef AMP5_TOP_RIGHT
        // AMP5 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !AMP5_TOP_RIGHT
#    ifdef AMP5_BOTTOM_RIGHT
        // AMP5 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !AMP5_BOTTOM_RIGHT
#     ifdef AMP5_RIGHT
        // AMP5 is RIGHT channel.
#      define CHANNEL_RIGHT
#     else  // !AMP5_RIGHT
        // By default AMP5 is LEFT channel.
#      define CHANNEL_LEFT
#     endif // AMP5_RIGHT
#    endif  // AMP5_BOTTOM_RIGHT
#   endif   // AMP5_TOP_RIGHT
#  endif    // AMP5_BOTTOM_LEFT
# endif     // AMP5_TOP_LEFT

#if AMP5_MANUFACTURER_ID==0x01FA
# if AMP5_FUNC_ID==0x3556
                #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
# elif AMP5_FUNC_ID==0x3563
                #include <CS35L63_AMP.asl>
# endif
#endif

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

# ifdef AMP_GPIO
                // Assign SPKR_ID GPIO for AMP
                Name (GPIA, ResourceTemplate ()
                {
                    // Pin37 - GPIO15
                    GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { AMP_GPIO }
                })

                Method (_CRS, 0x0, NotSerialized)
                {
                    Return (GPIA)
                }
# endif
            }
        }
    }
#endif  // AMP5_UID

#ifdef AMP6_UID
    // AMP 6
    Device (SWD8)
    {
        Name (_ADR, ((0x000<<52)|(AMP6_LID << 48)|(3<<44)|(AMP6_UID << 40)|(AMP6_MANUFACTURER_ID << 24)|(AMP6_PART_ID << 8)|0x1) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
#if AMP6_MANUFACTURER_ID==0x01FA
# if AMP6_PART_ID==0x3556
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP6_PART_ID==0x3557
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP6_PART_ID==0x3563
            #include <CS35L63_DSD.asl>  // For additional _DSD entries, common to all Buffetts
# endif
#endif

# ifdef AMP_GPIO
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

#if AMP6_MANUFACTURER_ID==0x01FA
# if AMP6_PART_ID==0x3556
        #include <CS35L56_PDP.asl>
# elif AMP6_PART_ID==0x3557
        #include <CS35L56_PDP.asl>
# elif AMP6_PART_ID==0x3563
        #include <CS35L63_PDP.asl>
# endif
#endif

        Method(_STA, 0, Serialized)
        {
            And(AUSW, 2, Local0)
            If(LEqual(Local0, 2))   // bit1 - spk enabled
            {
                return (0xf)    // show in UI
            }
            return (0xb)        // do not show in UI
        }

        If (LOr(LEqual(AUSW, 3), LEqual(AUSW, 2)))
        {
            Device(AF01)
            {
                Name(_ADR, 0x1)

# ifdef AMP6_TOP_LEFT
        // AMP6 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !AMP6_TOP_LEFT
#  ifdef AMP6_BOTTOM_LEFT
        // AMP6 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !AMP6_BOTTOM_LEFT
#   ifdef AMP6_TOP_RIGHT
        // AMP6 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !AMP6_TOP_RIGHT
#    ifdef AMP6_BOTTOM_RIGHT
        // AMP6 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !AMP6_BOTTOM_RIGHT
#     ifdef AMP6_LEFT
        // AMP6 is LEFT channel.
#      define CHANNEL_LEFT
#     else  // !AMP6_LEFT
        // By default AMP6 is RIGHT channel.
#      define CHANNEL_RIGHT
#     endif // AMP6_LEFT
#    endif  // AMP6_BOTTOM_RIGHT
#   endif   // AMP6_TOP_RIGHT
#  endif    // AMP6_BOTTOM_LEFT
# endif     // AMP6_TOP_LEFT

#if AMP6_MANUFACTURER_ID==0x01FA
# if AMP6_FUNC_ID==0x3556
                #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
# elif AMP6_FUNC_ID==0x3563
                #include <CS35L63_AMP.asl>
# endif
#endif

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

# ifdef AMP_GPIO
                // Assign SPKR_ID GPIO for AMP
                Name (GPIA, ResourceTemplate ()
                {
                    // Pin37 - GPIO15
                    GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { AMP_GPIO }
                })

                Method (_CRS, 0x0, NotSerialized)
                {
                    Return (GPIA)
                }
# endif
            }
        }
    }
#endif  // AMP6_UID

#ifdef AMP7_UID
    // AMP 7
    Device (SWD9)
    {
        Name (_ADR, ((0x000<<52)|(AMP7_LID << 48)|(3<<44)|(AMP7_UID << 40)|(AMP7_MANUFACTURER_ID << 24)|(AMP7_PART_ID << 8)|0x1) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
#if AMP7_MANUFACTURER_ID==0x01FA
# if AMP7_PART_ID==0x3556
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP7_PART_ID==0x3557
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP7_PART_ID==0x3563
            #include <CS35L63_DSD.asl>  // For additional _DSD entries, common to all Buffetts
# endif
#endif

# ifdef AMP_GPIO
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

#if AMP7_MANUFACTURER_ID==0x01FA
# if AMP7_PART_ID==0x3556
        #include <CS35L56_PDP.asl>
# elif AMP7_PART_ID==0x3557
        #include <CS35L56_PDP.asl>
# elif AMP7_PART_ID==0x3563
        #include <CS35L63_PDP.asl>
# endif
#endif

        Method(_STA, 0, Serialized)
        {
            And(AUSW, 2, Local0)
            If(LEqual(Local0, 2))   // bit1 - spk enabled
            {
                return (0xf)    // show in UI
            }
            return (0xb)        // do not show in UI
        }

        If (LOr(LEqual(AUSW, 3), LEqual(AUSW, 2)))
        {
            Device(AF01)
            {
                Name(_ADR, 0x1)

# ifdef AMP7_TOP_LEFT
        // AMP7 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !AMP7_TOP_LEFT
#  ifdef AMP7_BOTTOM_LEFT
        // AMP7 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !AMP7_BOTTOM_LEFT
#   ifdef AMP7_TOP_RIGHT
        // AMP7 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !AMP7_TOP_RIGHT
#    ifdef AMP7_BOTTOM_RIGHT
        // AMP7 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !AMP7_BOTTOM_RIGHT
#     ifdef AMP7_LEFT
        // AMP7 is LEFT channel.
#      define CHANNEL_LEFT
#     else  // !AMP7_LEFT
        // By default AMP7 is RIGHT channel.
#      define CHANNEL_RIGHT
#     endif // AMP7_LEFT
#    endif  // AMP7_BOTTOM_RIGHT
#   endif   // AMP7_TOP_RIGHT
#  endif    // AMP7_BOTTOM_LEFT
# endif     // AMP7_TOP_LEFT

#if AMP7_MANUFACTURER_ID==0x01FA
# if AMP7_FUNC_ID==0x3556
                #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
# elif AMP7_FUNC_ID==0x3563
                #include <CS35L63_AMP.asl>
# endif
#endif

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

# ifdef AMP_GPIO
                // Assign SPKR_ID GPIO for AMP
                Name (GPIA, ResourceTemplate ()
                {
                    // Pin37 - GPIO15
                    GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { AMP_GPIO }
                })

                Method (_CRS, 0x0, NotSerialized)
                {
                    Return (GPIA)
                }
# endif
            }
        }
    }
#endif  // AMP7_UID

#ifdef AMP8_UID
    // AMP 8
    Device (SWD10)
    {
        Name (_ADR, ((0x000<<52)|(AMP8_LID << 48)|(3<<44)|(AMP8_UID << 40)|(AMP8_MANUFACTURER_ID << 24)|(AMP8_PART_ID << 8)|0x1) )  // _ADR: Address

        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
#if AMP8_MANUFACTURER_ID==0x01FA
# if AMP8_PART_ID==0x3556
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP8_PART_ID==0x3557
            #include <CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson
# elif AMP8_PART_ID==0x3563
            #include <CS35L63_DSD.asl>  // For additional _DSD entries, common to all Buffetts
# endif
#endif

# ifdef AMP_GPIO
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

#if AMP8_MANUFACTURER_ID==0x01FA
# if AMP8_PART_ID==0x3556
        #include <CS35L56_PDP.asl>
# elif AMP8_PART_ID==0x3557
        #include <CS35L56_PDP.asl>
# elif AMP8_PART_ID==0x3563
        #include <CS35L63_PDP.asl>
# endif
#endif

        Method(_STA, 0, Serialized)
        {
            And(AUSW, 2, Local0)
            If(LEqual(Local0, 2))   // bit1 - spk enabled
            {
                return (0xf)    // show in UI
            }
            return (0xb)        // do not show in UI
        }

        If (LOr(LEqual(AUSW, 3), LEqual(AUSW, 2)))
        {
            Device(AF01)
            {
                Name(_ADR, 0x1)

# ifdef AMP8_TOP_LEFT
        // AMP8 is TOP-LEFT channel.
#  define CHANNEL_TOP_LEFT
# else      // !AMP8_TOP_LEFT
#  ifdef AMP8_BOTTOM_LEFT
        // AMP8 is BOTTOM-LEFT channel.
#   define CHANNEL_BOTTOM_LEFT
#  else     // !AMP8_BOTTOM_LEFT
#   ifdef AMP8_TOP_RIGHT
        // AMP8 is TOP-RIGHT channel.
#    define CHANNEL_TOP_RIGHT
#   else    // !AMP8_TOP_RIGHT
#    ifdef AMP8_BOTTOM_RIGHT
        // AMP8 is BOTTOM-RIGHT channel.
#     define CHANNEL_BOTTOM_RIGHT
#    else   // !AMP8_BOTTOM_RIGHT
#     ifdef AMP8_LEFT
        // AMP8 is LEFT channel.
#      define CHANNEL_LEFT
#     else  // !AMP8_LEFT
        // By default AMP8 is RIGHT channel.
#      define CHANNEL_RIGHT
#     endif // AMP8_LEFT
#    endif  // AMP8_BOTTOM_RIGHT
#   endif   // AMP8_TOP_RIGHT
#  endif    // AMP8_BOTTOM_LEFT
# endif     // AMP8_TOP_LEFT

#if AMP8_MANUFACTURER_ID==0x01FA
# if AMP8_FUNC_ID==0x3556
                #include <SdcaSmartAmp_CJAM3556_RefStream_common.asl>
# elif AMP8_FUNC_ID==0x3563
                #include <CS35L63_AMP.asl>
# endif
#endif

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

# ifdef AMP_GPIO
                // Assign SPKR_ID GPIO for AMP
                Name (GPIA, ResourceTemplate ()
                {
                    // Pin37 - GPIO15
                    GpioIo (Shared, PullNone, 0x0, 0x0, IoRestrictionInputOnly, "\\_SB.GPI0", 0, ResourceConsumer, , ) { AMP_GPIO }
                })

                Method (_CRS, 0x0, NotSerialized)
                {
                    Return (GPIA)
                }
# endif
            }
        }
    }
#endif  // AMP8_UID

#ifdef CODEC1_UID
    // Cohen UAJ [UID_0, SNDW_3]
    Device (SWD6)
    {
        Name (_ADR, ((0x000<<52)|(CODEC1_LID << 48)|(3<<44)|(CODEC1_UID << 40)|(CODEC1_MANUFACTURER_ID << 24)|(CODEC1_PART_ID << 8)|0x1) )  // _ADR: Address

# if CODEC1_MANUFACTURER_ID==0x01FA
#  if CODEC1_PART_ID==0x4243
        #include <CS42L43_DSD.asl>
#  endif
#  if CODEC1_PART_ID==0x4245
        #include <CS42L45_DSD.asl>
#  endif
# endif

# ifdef CODEC1_AMP
        Device (AF01)
        {
            Name (_ADR, 0x1) // SDCA Function Number = 0x1

            #define CHANNEL_LEFT
            #define CHANNEL_RIGHT
#  if CODEC1_MANUFACTURER_ID==0x01FA
#   if CODEC1_FUNC_ID==0x4243
            #include <Cohen_Amp.asl>
#   endif
#  endif
            #undef CHANNEL_LEFT
            #undef CHANNEL_RIGHT

#  ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
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
#  endif // SIDECAR_VARIABLE_SPEAKER_SELECT
        }
# endif // CODEC1_AMP

# ifdef CODEC1_MIC
        If (LAnd(LOr(LEqual(AUSW, 3), LEqual(AUSW, 1)), LEqual(DMSW, 1)))
        {
            Device (AF02)
            {
                Name (_ADR, 0x2) // SDCA Function Number = 0x2

#  if CODEC1_MANUFACTURER_ID==0x01FA
#   ifdef MIC_GEOMETRY_OVERLOAD
                #include <Mic_Geometry_Overload.asl>
#   endif
#   if CODEC1_FUNC_ID==0x4243
                #include <Cohen_SimpleMic.asl>
#   endif
#   if CODEC1_FUNC_ID==0x4245
                #include <Phife_SimpleMic.asl>
#   endif
#  endif

#  ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
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
#  endif // SIDECAR_VARIABLE_SPEAKER_SELECT
            }
        }
# endif // CODEC1_MIC

#  ifdef CODEC1_UAJ
        If (LOr(LEqual(AUSW, 3), LEqual(AUSW, 1)))
        {
            Device (AF03)
            {
                Name (_ADR, 0x3) // SDCA Hierarchical Function Number = 0x3

#  if CODEC1_MANUFACTURER_ID==0x01FA
#   if CODEC1_FUNC_ID==0x4243
                #include <Cohen_UAJ.asl>
                #include <Cohen_UAJ_capture_enable.asl>
#   endif
#   if CODEC1_FUNC_ID==0x4245
                #include <Phife_UAJ.asl>
                #include <Phife_UAJ_capture_enable.asl>
#   endif
#  endif

#  ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
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
#  endif // SIDECAR_VARIABLE_SPEAKER_SELECT
            }
        }
        Else
        {
            Device (AF3B)
            {
                Name (_ADR, 0x03)  // _ADR: Address

#  if CODEC1_MANUFACTURER_ID==0x01FA
#   if CODEC1_FUNC_ID==0x4243
                #include <Cohen_UAJ.asl>
                #include <Cohen_UAJ_capture_disable.asl>
#   endif
#   if CODEC1_FUNC_ID==0x4245
                #include <Phife_UAJ.asl>
                #include <Phife_UAJ_capture_disable.asl>
#   endif
#  endif

#  ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
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
#  endif // SIDECAR_VARIABLE_SPEAKER_SELECT
            }
        }

        Device (AF04)
        {
            Name (_ADR, 0x4) // SDCA Hierarchical Function Number = 0x4

#  if CODEC1_MANUFACTURER_ID==0x01FA
#   if CODEC1_FUNC_ID==0x4243
            #include <CS_HID_headset_buttons.asl>
#   endif
#   if CODEC1_FUNC_ID==0x4245
            #include <Phife_HID_headset_buttons.asl>
#   endif
#  endif
        }
# endif // CODEC1_UAJ
    }
#endif // CODEC1_UID
#ifndef _AMD
}
#endif
