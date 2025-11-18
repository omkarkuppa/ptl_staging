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


//-----------------
// Configuration
//-----------------

#ifdef AMP6_UID
# define _NUM_AMP_AMPS 6
#else
# ifdef AMP5_UID
#  define _NUM_AMP_AMPS 5
# else
#  ifdef AMP4_UID
#   define _NUM_AMP_AMPS 4
#  else
#   ifdef AMP3_UID
#    define _NUM_AMP_AMPS 3
#   else
#    ifdef AMP2_UID
#     define _NUM_AMP_AMPS 2
#    else
#     ifdef AMP1_UID
#      define _NUM_AMP_AMPS 1
#     else
#      define _NUM_AMP_AMPS 0
#     endif  // AMP1_UID
#    endif  // AMP2_UID
#   endif  // AMP3_UID
#  endif  // AMP4_UID
# endif  // AMP5_UID
#endif  // AMP6_UID

#ifdef CODEC1_UID
# ifdef CODEC1_AMP
#  define _NUM_CODEC_AMPS 1
# else
#  define _NUM_CODEC_AMPS 0
# endif
#else
# define _NUM_CODEC_AMPS 0
#endif // CODEC1_UID

//-----------------
// HW specific configuration
//-----------------
#ifdef AMP1_UID
# if AMP1_MANUFACTURER_ID==0x01FA
#  if AMP1_PART_ID==0x3556
#   define AMP1_RENDER_DATA_PORT        0x1
#   define AMP1_US_DATA_PORT            0x2
#   define AMP1_SENSE_DATA_PORT         0x3
#   define AMP1_REF_DATA_PORT           0x4
#  else
#   if AMP1_PART_ID==0x3557
#    define AMP1_RENDER_DATA_PORT        0x1
#    define AMP1_US_DATA_PORT            0x2
#    define AMP1_SENSE_DATA_PORT         0x3
#    define AMP1_REF_DATA_PORT           0x4
#   else
#    if AMP1_PART_ID==0x3562
#     define AMP1_RENDER_DATA_PORT        0x1
#     define AMP1_US_DATA_PORT            0x2
#     define AMP1_SENSE_DATA_PORT         0x3
#     define AMP1_REF_DATA_PORT           0x4
#    else
#     if AMP1_PART_ID==0x3563
#      define AMP1_RENDER_DATA_PORT        0x1
#      define AMP1_US_DATA_PORT            0x2
#      define AMP1_SENSE_DATA_PORT         0x3
#      define AMP1_REF_DATA_PORT           0x4
#     else
#      error "AMP1 unknown part!"
#     endif
#    endif
#   endif
#  endif
# else
#  error "AMP1 unknown manufacturer!"
# endif
#endif

#ifdef AMP2_UID
# if AMP2_MANUFACTURER_ID==0x01FA
#  if AMP2_PART_ID==0x3556
#   define AMP2_RENDER_DATA_PORT        0x1
#   define AMP2_US_DATA_PORT            0x2
#   define AMP2_SENSE_DATA_PORT         0x3
#   define AMP2_REF_DATA_PORT           0x4
#  else
#   if AMP2_PART_ID==0x3557
#    define AMP2_RENDER_DATA_PORT        0x1
#    define AMP2_US_DATA_PORT            0x2
#    define AMP2_SENSE_DATA_PORT         0x3
#    define AMP2_REF_DATA_PORT           0x4
#   else
#    if AMP2_PART_ID==0x3562
#     define AMP2_RENDER_DATA_PORT        0x1
#     define AMP2_US_DATA_PORT            0x2
#     define AMP2_SENSE_DATA_PORT         0x3
#     define AMP2_REF_DATA_PORT           0x4
#    else
#     if AMP2_PART_ID==0x3563
#      define AMP2_RENDER_DATA_PORT        0x1
#      define AMP2_US_DATA_PORT            0x2
#      define AMP2_SENSE_DATA_PORT         0x3
#      define AMP2_REF_DATA_PORT           0x4
#     else
#      error "AMP2 unknown part!"
#     endif
#    endif
#   endif
#  endif
# else
#  error "AMP2 unknown manufacturer!"
# endif
#endif

#ifdef AMP3_UID
# if AMP3_MANUFACTURER_ID==0x01FA
#  if AMP3_PART_ID==0x3556
#   define AMP3_RENDER_DATA_PORT        0x1
#   define AMP3_US_DATA_PORT            0x2
#   define AMP3_SENSE_DATA_PORT         0x3
#   define AMP3_REF_DATA_PORT           0x4
#  else
#   if AMP3_PART_ID==0x3557
#    define AMP3_RENDER_DATA_PORT        0x1
#    define AMP3_US_DATA_PORT            0x2
#    define AMP3_SENSE_DATA_PORT         0x3
#    define AMP3_REF_DATA_PORT           0x4
#   else
#    if AMP3_PART_ID==0x3562
#     define AMP3_RENDER_DATA_PORT        0x1
#     define AMP3_US_DATA_PORT            0x2
#     define AMP3_SENSE_DATA_PORT         0x3
#     define AMP3_REF_DATA_PORT           0x4
#    else
#     if AMP3_PART_ID==0x3563
#      define AMP3_RENDER_DATA_PORT        0x1
#      define AMP3_US_DATA_PORT            0x2
#      define AMP3_SENSE_DATA_PORT         0x3
#      define AMP3_REF_DATA_PORT           0x4
#     else
#      error "AMP3 unknown part!"
#     endif
#    endif
#   endif
#  endif
# else
#  error "AMP3 unknown manufacturer!"
# endif
#endif

#ifdef AMP4_UID
# if AMP4_MANUFACTURER_ID==0x01FA
#  if AMP4_PART_ID==0x3556
#   define AMP4_RENDER_DATA_PORT        0x1
#   define AMP4_US_DATA_PORT            0x2
#   define AMP4_SENSE_DATA_PORT         0x3
#   define AMP4_REF_DATA_PORT           0x4
#  else
#   if AMP4_PART_ID==0x3557
#    define AMP4_RENDER_DATA_PORT        0x1
#    define AMP4_US_DATA_PORT            0x2
#    define AMP4_SENSE_DATA_PORT         0x3
#    define AMP4_REF_DATA_PORT           0x4
#   else
#    if AMP4_PART_ID==0x3562
#     define AMP4_RENDER_DATA_PORT        0x1
#     define AMP4_US_DATA_PORT            0x2
#     define AMP4_SENSE_DATA_PORT         0x3
#     define AMP4_REF_DATA_PORT           0x4
#    else
#     if AMP4_PART_ID==0x3563
#      define AMP4_RENDER_DATA_PORT        0x1
#      define AMP4_US_DATA_PORT            0x2
#      define AMP4_SENSE_DATA_PORT         0x3
#      define AMP4_REF_DATA_PORT           0x4
#     else
#      error "AMP4 unknown part!"
#     endif
#    endif
#   endif
#  endif
# else
#  error "AMP4 unknown manufacturer!"
# endif
#endif

#ifdef AMP5_UID
# if AMP5_MANUFACTURER_ID==0x01FA
#  if AMP5_PART_ID==0x3556
#   define AMP5_RENDER_DATA_PORT        0x1
#   define AMP5_US_DATA_PORT            0x2
#   define AMP5_SENSE_DATA_PORT         0x3
#   define AMP5_REF_DATA_PORT           0x4
#  else
#   if AMP5_PART_ID==0x3557
#    define AMP5_RENDER_DATA_PORT        0x1
#    define AMP5_US_DATA_PORT            0x2
#    define AMP5_SENSE_DATA_PORT         0x3
#    define AMP5_REF_DATA_PORT           0x4
#   else
#    if AMP5_PART_ID==0x3562
#     define AMP5_RENDER_DATA_PORT        0x1
#     define AMP5_US_DATA_PORT            0x2
#     define AMP5_SENSE_DATA_PORT         0x3
#     define AMP5_REF_DATA_PORT           0x4
#    else
#     if AMP5_PART_ID==0x3563
#      define AMP5_RENDER_DATA_PORT        0x1
#      define AMP5_US_DATA_PORT            0x2
#      define AMP5_SENSE_DATA_PORT         0x3
#      define AMP5_REF_DATA_PORT           0x4
#     else
#      error "AMP5 unknown part!"
#     endif
#    endif
#   endif
#  endif
# else
#  error "AMP5 unknown manufacturer!"
# endif
#endif

#ifdef AMP6_UID
# if AMP6_MANUFACTURER_ID==0x01FA
#  if AMP6_PART_ID==0x3556
#   define AMP6_RENDER_DATA_PORT        0x1
#   define AMP6_US_DATA_PORT            0x2
#   define AMP6_SENSE_DATA_PORT         0x3
#   define AMP6_REF_DATA_PORT           0x4
#  else
#   if AMP6_PART_ID==0x3557
#    define AMP6_RENDER_DATA_PORT        0x1
#    define AMP6_US_DATA_PORT            0x2
#    define AMP6_SENSE_DATA_PORT         0x3
#    define AMP6_REF_DATA_PORT           0x4
#   else
#    if AMP6_PART_ID==0x3562
#     define AMP6_RENDER_DATA_PORT        0x1
#     define AMP6_US_DATA_PORT            0x2
#     define AMP6_SENSE_DATA_PORT         0x3
#     define AMP6_REF_DATA_PORT           0x4
#    else
#     if AMP6_PART_ID==0x3563
#      define AMP6_RENDER_DATA_PORT        0x1
#      define AMP6_US_DATA_PORT            0x2
#      define AMP6_SENSE_DATA_PORT         0x3
#      define AMP6_REF_DATA_PORT           0x4
#     else
#      error "AMP6 unknown part!"
#     endif
#    endif
#   endif
#  endif
# else
#  error "AMP6 unknown manufacturer!"
# endif
#endif

#ifdef AMP7_UID
# if AMP7_MANUFACTURER_ID==0x01FA
#  if AMP7_PART_ID==0x3556
#   define AMP7_RENDER_DATA_PORT        0x1
#   define AMP7_US_DATA_PORT            0x2
#   define AMP7_SENSE_DATA_PORT         0x3
#   define AMP7_REF_DATA_PORT           0x4
#  else
#   if AMP7_PART_ID==0x3557
#    define AMP7_RENDER_DATA_PORT        0x1
#    define AMP7_US_DATA_PORT            0x2
#    define AMP7_SENSE_DATA_PORT         0x3
#    define AMP7_REF_DATA_PORT           0x4
#   else
#    if AMP7_PART_ID==0x3562
#     define AMP7_RENDER_DATA_PORT        0x1
#     define AMP7_US_DATA_PORT            0x2
#     define AMP7_SENSE_DATA_PORT         0x3
#     define AMP7_REF_DATA_PORT           0x4
#    else
#     if AMP7_PART_ID==0x3563
#      define AMP7_RENDER_DATA_PORT        0x1
#      define AMP7_US_DATA_PORT            0x2
#      define AMP7_SENSE_DATA_PORT         0x3
#      define AMP7_REF_DATA_PORT           0x4
#     else
#      error "AMP7 unknown part!"
#     endif
#    endif
#   endif
#  endif
# else
#  error "AMP7 unknown manufacturer!"
# endif
#endif

#ifdef AMP8_UID
# if AMP8_MANUFACTURER_ID==0x01FA
#  if AMP8_PART_ID==0x3556
#   define AMP8_RENDER_DATA_PORT        0x1
#   define AMP8_US_DATA_PORT            0x2
#   define AMP8_SENSE_DATA_PORT         0x3
#   define AMP8_REF_DATA_PORT           0x4
#  else
#   if AMP8_PART_ID==0x3557
#    define AMP8_RENDER_DATA_PORT        0x1
#    define AMP8_US_DATA_PORT            0x2
#    define AMP8_SENSE_DATA_PORT         0x3
#    define AMP8_REF_DATA_PORT           0x4
#   else
#    if AMP8_PART_ID==0x3562
#     define AMP8_RENDER_DATA_PORT        0x1
#     define AMP8_US_DATA_PORT            0x2
#     define AMP8_SENSE_DATA_PORT         0x3
#     define AMP8_REF_DATA_PORT           0x4
#    else
#     if AMP8_PART_ID==0x3563
#      define AMP8_RENDER_DATA_PORT        0x1
#      define AMP8_US_DATA_PORT            0x2
#      define AMP8_SENSE_DATA_PORT         0x3
#      define AMP8_REF_DATA_PORT           0x4
#     else
#      error "AMP8 unknown part!"
#     endif
#    endif
#   endif
#  endif
# else
#  error "AMP8 unknown manufacturer!"
# endif
#endif

#ifdef CODEC1_MIC_CHANNEL_CNT
# undef CODEC1_MIC_CHANNEL_CNT
#endif

#ifdef CODEC1_UID
# if CODEC1_MANUFACTURER_ID==0x01FA
#  if CODEC1_PART_ID==0x4243
#   define CODEC1_MIC_CAPTURE_DATA_PORT 0x1
#   define CODEC1_UAJ_CAPTURE_DATA_PORT 0x2
#   define CODEC1_AMP_REF_DATA_PORT     0x3
#   define CODEC1_AMP_RENDER_DATA_PORT  0x5
#   define CODEC1_UAJ_RENDER_DATA_PORT  0x6

#   if CS42L43_IT11_NUM_OF_MIC > 3
#    define CODEC1_MIC_CHANNEL_CNT 4
#   else
#    if CS42L43_IT11_NUM_OF_MIC > 2
#     define CODEC1_MIC_CHANNEL_CNT 3
#    else
#     if CS42L43_IT11_NUM_OF_MIC > 1
#      define CODEC1_MIC_CHANNEL_CNT 2
#     else
#      define CODEC1_MIC_CHANNEL_CNT 1
#     endif
#    endif
#   endif
#  else
#   if CODEC1_PART_ID==0x4245
#    define CODEC1_MIC_CAPTURE_DATA_PORT 0x1
#    define CODEC1_ULS_CAPTURE_DATA_PORT 0x2
#    define CODEC1_SEC_CAPTURE_DATA_PORT 0x3
#    define CODEC1_UAJ_CAPTURE_DATA_PORT 0x4
#    define CODEC1_UAJ_RENDER_DATA_PORT  0x5

#    if CS42L45_IT11_NUM_OF_MIC > 3
#     define CODEC1_MIC_CHANNEL_CNT 4
#    else
#     if CS42L45_IT11_NUM_OF_MIC > 2
#      define CODEC1_MIC_CHANNEL_CNT 3
#     else
#      if CS42L45_IT11_NUM_OF_MIC > 1
#       define CODEC1_MIC_CHANNEL_CNT 2
#      else
#       define CODEC1_MIC_CHANNEL_CNT 1
#      endif
#     endif
#    endif
#   else
#     error "CODEC1 unknown part!"
#   endif
#  endif
# else
#  error "CODEC1 unknown manufacturer!"
# endif
#endif

#ifdef CODEC1_MIC_CHANNEL_MASK
# undef CODEC1_MIC_CHANNEL_MASK
#endif

#ifdef CODEC1_MIC_PERIPHERAL_MASK
# undef CODEC1_MIC_PERIPHERAL_MASK
#endif

#if CODEC1_MIC_CHANNEL_CNT > 3
# define CODEC1_MIC_PERIPHERAL_MASK 0x33
# define CODEC1_MIC_CHANNEL_MASK 0xf
#else
# if CODEC1_MIC_CHANNEL_CNT > 2
#   define CODEC1_MIC_PERIPHERAL_MASK 0x13
#  define CODEC1_MIC_CHANNEL_MASK 0x7
# else
#  if CODEC1_MIC_CHANNEL_CNT > 1
#   define CODEC1_MIC_PERIPHERAL_MASK 0x3
#   define CODEC1_MIC_CHANNEL_MASK 0x3
#  else
#   define CODEC1_MIC_PERIPHERAL_MASK 0x31
#   define CODEC1_MIC_CHANNEL_MASK 0x1
#  endif
# endif
#endif

Name(AC00, Package() {    // This package is shared by all DSP devices in this composition table
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"msft-acx-factory-circuit", 1}, // Is an ACX Factory
       Package (2) {"msft-acx-core-circuit", 0},    // Not an ACX Core circuit
    }
}) //End AC00

#if (_NUM_AMP_AMPS + _NUM_CODEC_AMPS)>0
Name(EP00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-endpoint-friendly-name", "Aggregated Speaker"},
        Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
        Package (2) {"acpi-acd-endpoint-id", 1},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"acpi-acd-endpoint-config-0-properties", "EC00"}, // Configuration for DSP and Aggregated devices
    }
}) //End EP00

Name(EC00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-config-priority", 1},
        Package (2) {"acpi-acd-config-friendly-name", "Aggregated_Speaker_With_DSP"},
        Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
        Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
        Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Functions (aggregated) makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"acpi-acd-collection-0-properties", "CC00"},    // DSP Configuration
#if (_NUM_AMP_AMPS + _NUM_CODEC_AMPS)>1
        Package (2) {"acpi-acd-collection-1-properties", "AG00"},    // Aggregated Speakers
#else
# if (_NUM_AMP_AMPS==1) && (_NUM_CODEC_AMPS==0)
        Package (2) {"acpi-acd-collection-1-properties", "CC01"},    // A single AMP Speaker
# else
#  if (_NUM_AMP_AMPS==0) && (_NUM_CODEC_AMPS==1)
        Package (2) {"acpi-acd-collection-1-properties", "CC09"},    // Codec AMP Stereo Speakers
#  endif
# endif
#endif
    }
}) //End EC00

Name(CC00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-device-namestring", DSP_ACPI_ACD_DEVICE_NAMESTRING},
        Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
        Package (2) {"acpi-acd-vendor-collection-properties", "VN00"},
    }
}) //End CC00

Name(VN00, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-vendor-id", 0x1},
        Package (2) {"acpi-vendor-config-type", "Streaming_Speaker"},
#ifdef _AMD
# if (_NUM_AMP_AMPS==0) && (_NUM_CODEC_AMPS==1)
        Package (2) {"amd-sdw-default-stream-dp-number", CODEC1_AMP_RENDER_DATA_PORT},  // Raw Stream Data port number on SPK device
# endif
# if _NUM_AMP_AMPS > 1
        Package (2) {"amd-sdw-default-stream-dp-number", AMP1_RENDER_DATA_PORT},        // Raw Stream Data port number on SPK device
# endif
# ifdef DEFAULT_STREAMING_SPK_VOL
        Package (2) {"amd-sdw-dsp-default-volume-level", DEFAULT_STREAMING_SPK_VOL},
# endif
#else  // !_AMD
# ifdef _NVIDIA
        // NVidia_Arm specific information for Speaker
#  if (_NUM_AMP_AMPS==0) && (_NUM_CODEC_AMPS==1)
        Package (2) {"acpi-vendor-sdw-data-port-number", CODEC1_AMP_RENDER_DATA_PORT},  // Raw Stream Data port number on SPK device
#  endif
#  if (_NUM_AMP_AMPS>1) && (_NUM_CODEC_AMPS==0)
        Package (2) {"acpi-vendor-sdw-data-port-number", AMP1_RENDER_DATA_PORT},        // Raw Stream Data port number on SPK device
#  endif
#  if (_NUM_AMP_AMPS>1) && (_NUM_CODEC_AMPS==1) // dissimilar amp aggregation
        Package (2) {"acpi-vendor-aggregation-peripheral-count", (_NUM_AMP_AMPS + _NUM_CODEC_AMPS)},
#   if (_NUM_AMP_AMPS==4)
        // AMP #1
        Package (2) {"acpi-vendor-aggregation-peripheral-0-controller-id", AMP1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-link-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-unique-id", AMP1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-manufacturer-id", AMP1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-part-id", AMP1_PART_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-data-port-number", AMP1_RENDER_DATA_PORT},

        // AMP #2
        Package (2) {"acpi-vendor-aggregation-peripheral-1-controller-id", AMP2_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-link-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-unique-id", AMP2_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-manufacturer-id", AMP2_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-part-id", AMP2_PART_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-data-port-number", AMP2_RENDER_DATA_PORT},

        // AMP #3
        Package (2) {"acpi-vendor-aggregation-peripheral-2-controller-id", AMP3_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-link-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-unique-id", AMP3_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-manufacturer-id", AMP3_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-part-id", AMP3_PART_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-data-port-number", AMP3_RENDER_DATA_PORT},

        // AMP #4
        Package (2) {"acpi-vendor-aggregation-peripheral-3-controller-id", AMP4_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-link-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-unique-id", AMP4_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-manufacturer-id", AMP4_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-part-id", AMP4_PART_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-data-port-number", AMP4_RENDER_DATA_PORT},

        // CODEC #1
        Package (2) {"acpi-vendor-aggregation-peripheral-4-controller-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-link-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-part-id", CODEC1_PART_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-data-port-number", CODEC1_AMP_RENDER_DATA_PORT},
#   else //
#    if (_NUM_AMP_AMPS==2)
        // AMP #1
        Package (2) {"acpi-vendor-aggregation-peripheral-0-controller-id", AMP1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-link-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-unique-id", AMP1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-manufacturer-id", AMP1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-part-id", AMP1_PART_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-data-port-number", AMP1_RENDER_DATA_PORT},

        // AMP #2
        Package (2) {"acpi-vendor-aggregation-peripheral-1-controller-id", AMP2_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-link-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-unique-id", AMP2_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-manufacturer-id", AMP2_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-part-id", AMP2_PART_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-data-port-number", AMP2_RENDER_DATA_PORT},

        // CODEC #1
        Package (2) {"acpi-vendor-aggregation-peripheral-2-controller-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-link-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-part-id", CODEC1_PART_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-data-port-number", CODEC1_AMP_RENDER_DATA_PORT},
#    endif // (_NUM_AMP_AMPS==2)
#   endif // (_NUM_AMP_AMPS==4)
#  endif // (_NUM_AMP_AMPS>1) && (_NUM_CODEC_AMPS==1)
# else  // !_AMD && !_NVIDIA

#  ifdef DEFAULT_STREAMING_SPK_VOL
        Package (2) {"acpi-vendor-endpoint-default-volume", DEFAULT_STREAMING_SPK_VOL},
#  endif

        Package (2) {"acpi-vendor-sdca-terminal-type", 0x0380},
        Package (2) {"acpi-vendor-aggregation-peripheral-count", (_NUM_AMP_AMPS + _NUM_CODEC_AMPS)}, // no of amps participating in aggregation

        // Reference stream setup
        Package (2) {"acpi_vendor_smart_amp", INTEL_DSP_AEC_ENABLE}, // enable is 1; disable is 0
        Package (2) {"acpi-vendor-amp-degraded-mode", 0},   // no IV sense, speaker protection on codec
        Package (2) {"acpi-vendor-feedback-aggregation-peripheral-count", (_NUM_AMP_AMPS + _NUM_CODEC_AMPS)},  // amps participating in aggregation of reference.

        // Information on peripheral participating in aggregation
#  if _NUM_AMP_AMPS>0
        // AMP #1
        Package (2) {"acpi-vendor-aggregation-peripheral-0-function-manufacturer-id", AMP1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-function-id", AMP1_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-link-id", AMP1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-unique-id", AMP1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-dp-number", AMP1_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-aec-feedback-channel-mask", AMP1_LR_CHANNEL_MASK},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-iv-feedback-channel-mask", AMP1_LR_CHANNEL_MASK},
#  endif
#  if _NUM_AMP_AMPS>1
        // AMP #2
        Package (2) {"acpi-vendor-aggregation-peripheral-1-function-manufacturer-id", AMP2_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-function-id", AMP2_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-link-id", AMP2_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-unique-id", AMP2_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-dp-number", AMP2_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-aec-feedback-channel-mask", AMP2_LR_CHANNEL_MASK},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-iv-feedback-channel-mask", AMP2_LR_CHANNEL_MASK},
#  endif
#  if _NUM_AMP_AMPS>2
        // AMP #3
        Package (2) {"acpi-vendor-aggregation-peripheral-2-function-manufacturer-id", AMP3_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-function-id", AMP3_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-link-id", AMP3_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-unique-id", AMP3_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-dp-number", AMP3_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-aec-feedback-channel-mask", AMP3_LR_CHANNEL_MASK},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-iv-feedback-channel-mask", AMP3_LR_CHANNEL_MASK},
#  endif
#  if _NUM_AMP_AMPS>3
        // AMP #4
        Package (2) {"acpi-vendor-aggregation-peripheral-3-function-manufacturer-id", AMP4_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-function-id", AMP4_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-link-id", AMP4_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-unique-id", AMP4_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-dp-number", AMP4_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-aec-feedback-channel-mask", AMP4_LR_CHANNEL_MASK},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-iv-feedback-channel-mask", AMP4_LR_CHANNEL_MASK},
#  endif
#  if _NUM_AMP_AMPS>4
        // AMP #5
        Package (2) {"acpi-vendor-aggregation-peripheral-4-function-manufacturer-id", AMP5_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-function-id", AMP5_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-link-id", AMP5_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-unique-id", AMP5_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-dp-number", AMP5_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-aec-feedback-channel-mask", AMP5_LR_CHANNEL_MASK},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-iv-feedback-channel-mask", AMP5_LR_CHANNEL_MASK},
#  endif
#  if _NUM_AMP_AMPS>5
        // AMP #6
        Package (2) {"acpi-vendor-aggregation-peripheral-5-function-manufacturer-id", AMP6_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-function-id", AMP6_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-link-id", AMP6_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-unique-id", AMP6_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-dp-number", AMP6_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-aec-feedback-channel-mask", AMP6_LR_CHANNEL_MASK},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-iv-feedback-channel-mask", AMP6_LR_CHANNEL_MASK},
#  endif
#  if _NUM_AMP_AMPS>6
        // AMP #7
        Package (2) {"acpi-vendor-aggregation-peripheral-6-function-manufacturer-id", AMP7_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-function-id", AMP7_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-link-id", AMP7_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-unique-id", AMP7_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-dp-number", AMP7_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-aec-feedback-channel-mask", AMP7_LR_CHANNEL_MASK},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-iv-feedback-channel-mask", AMP7_LR_CHANNEL_MASK},
#  endif
#  if _NUM_AMP_AMPS>7
        // AMP #8
        Package (2) {"acpi-vendor-aggregation-peripheral-7-function-manufacturer-id", AMP8_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-function-id", AMP8_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-link-id", AMP8_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-unique-id", AMP8_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-dp-number", AMP8_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-aec-feedback-channel-mask", AMP8_LR_CHANNEL_MASK},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-iv-feedback-channel-mask", AMP8_LR_CHANNEL_MASK},
#  endif

#  ifdef CODEC1_AMP
#   if _NUM_AMP_AMPS>7
        // CODEC amp is present with aggregation to additional AMP.
        Package (2) {"acpi-vendor-aggregation-peripheral-8-function-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-8-function-id", CODEC1_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-8-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-8-link-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-8-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-8-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-8-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-8-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-8-aec-feedback-channel-mask", 0x3},
        Package (2) {"acpi-vendor-aggregation-peripheral-8-iv-feedback-channel-mask", 0x3},
#   else
#    if _NUM_AMP_AMPS>6
        // CODEC amp is present with aggregation to additional AMP.
        Package (2) {"acpi-vendor-aggregation-peripheral-7-function-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-function-id", CODEC1_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-link-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-aec-feedback-channel-mask", 0x3},
        Package (2) {"acpi-vendor-aggregation-peripheral-7-iv-feedback-channel-mask", 0x3},
#    else
#     if _NUM_AMP_AMPS>5
        // CODEC amp is present with aggregation to additional AMP.
        Package (2) {"acpi-vendor-aggregation-peripheral-6-function-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-function-id", CODEC1_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-link-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-aec-feedback-channel-mask", 0x3},
        Package (2) {"acpi-vendor-aggregation-peripheral-6-iv-feedback-channel-mask", 0x3},
#     else
#      if _NUM_AMP_AMPS>4
        // CODEC amp is present with aggregation to additional AMP.
        Package (2) {"acpi-vendor-aggregation-peripheral-5-function-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-function-id", CODEC1_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-link-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-aec-feedback-channel-mask", 0x3},
        Package (2) {"acpi-vendor-aggregation-peripheral-5-iv-feedback-channel-mask", 0x3},
#      else
#       if _NUM_AMP_AMPS>3
        // CODEC amp is present with aggregation to additional AMP.
        Package (2) {"acpi-vendor-aggregation-peripheral-4-function-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-function-id", CODEC1_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-link-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-aec-feedback-channel-mask", 0x3},
        Package (2) {"acpi-vendor-aggregation-peripheral-4-iv-feedback-channel-mask", 0x3},
#       else
#        if _NUM_AMP_AMPS>2
        // CODEC amp is present with aggregation to additional AMP.
        Package (2) {"acpi-vendor-aggregation-peripheral-3-function-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-function-id", CODEC1_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-link-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-aec-feedback-channel-mask", 0x3},
        Package (2) {"acpi-vendor-aggregation-peripheral-3-iv-feedback-channel-mask", 0x3},
#        else
#         if _NUM_AMP_AMPS>1
        // CODEC amp is present with aggregation to additional AMP.
        Package (2) {"acpi-vendor-aggregation-peripheral-2-function-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-function-id", CODEC1_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-link-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-aec-feedback-channel-mask", 0x3},
        Package (2) {"acpi-vendor-aggregation-peripheral-2-iv-feedback-channel-mask", 0x3},
#         else
#          if _NUM_AMP_AMPS>0
        // CODEC amp is present with aggregation to additional AMP.
        Package (2) {"acpi-vendor-aggregation-peripheral-1-function-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-function-id", CODEC1_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-link-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-function-number", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-aec-feedback-channel-mask", 0x3},
        Package (2) {"acpi-vendor-aggregation-peripheral-1-iv-feedback-channel-mask", 0x3},
#          else
        // Echo reference stream DisCo data for non-aggregated CODEC amp topologies
        Package (2) {"acpi-acd-connection-count", 2},
        Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
        Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
        Package (2) {"acpi-vendor-connection-1-dsp-pin", 0x02},
        Package (2) {"acpi-vendor-connection-1-stream-type", 0x0},
        Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-connection-1-peripheral-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-function-manufacturer-id", CODEC1_MANUFACTURER_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-function-id", CODEC1_FUNC_ID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-controller-id", 0},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-link-id", CODEC1_LID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-unique-id", CODEC1_UID},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-function-number", 1}, // amp function
        Package (2) {"acpi-vendor-aggregation-peripheral-0-function-type", 1},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-dp-number", CODEC1_AMP_RENDER_DATA_PORT},
        Package (2) {"acpi-vendor-aggregation-peripheral-0-aec-feedback-channel-mask", 0x3}, // left plus right channels
        Package (2) {"acpi-vendor-aggregation-peripheral-0-iv-feedback-channel-mask", 0x3},
#          endif
#         endif
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif  // CODEC1_AMP
# endif  // _NVIDIA
#endif  // _AMD
    }
})  //End VN00

Name(AC01, Package() {    // This package is shared by all Codec devices in this composition table
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),     // Device Properties UUID
    Package () {
        Package (2) {"msft-acx-factory-circuit", 0},    // Not an ACX Factory
        Package (2) {"msft-acx-core-circuit", 1},       // ACX Core circuit
#if (_NUM_AMP_AMPS + _NUM_CODEC_AMPS)>1
        Package (2) {"msft-acx-aggregated-circuit", 1},
#endif
    }
}) //End AC01

#if _NUM_AMP_AMPS>0
Name(CC01, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-device-namestring", AMP1_AMP_DEV_NAME},
        Package (2) {"acpi-acd-device-type", 1},        // 0: Generic, 1: SoundWire
        Package (2) {"acpi-acd-sdca-terminal-id", 0xE}, // Entity id of the Analog terminal used for this endpoint
        Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),     // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC01
#endif

#if (_NUM_AMP_AMPS + _NUM_CODEC_AMPS)>1
// This is the multiple AMP or AMP and CODEC aggregation ACD collection
Name(AG00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-config-priority", 1},
        Package (2) {"acpi-acd-config-friendly-name", "Speaker_Aggregation"},
        Package (2) {"acpi-acd-collection-type", 0},    // 0: Generic, 1: SoundWire
        Package (2) {"acpi-acd-collection-ordering", 1},    // Parallel Connection
        Package (2) {"acpi-acd-collection-count", (_NUM_AMP_AMPS + _NUM_CODEC_AMPS)},    // N SDCA Audio Functions
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Data Extension UUID
    Package () {
#if _NUM_AMP_AMPS>0
        Package (2) {"acpi-acd-collection-0-properties", "CC01"},   // 1st AMP's Function
#endif
#if _NUM_AMP_AMPS>1
        Package (2) {"acpi-acd-collection-1-properties", "CC02"},   // 2nd AMP's Function
#endif
#if _NUM_AMP_AMPS>2
        Package (2) {"acpi-acd-collection-2-properties", "CC03"},   // 3rd AMP's Function
#endif
#if _NUM_AMP_AMPS>3
        Package (2) {"acpi-acd-collection-3-properties", "CC04"},   // 4th AMP's Function
#endif
#if _NUM_AMP_AMPS>4
        Package (2) {"acpi-acd-collection-4-properties", "CC05"},   // 5th AMP's Function
#endif
#if _NUM_AMP_AMPS>5
        Package (2) {"acpi-acd-collection-5-properties", "CC06"},   // 6th AMP's Function
#endif
#if _NUM_AMP_AMPS>6
        Package (2) {"acpi-acd-collection-6-properties", "CC07"},   // 7th AMP's Function
#endif
#if _NUM_AMP_AMPS>7
        Package (2) {"acpi-acd-collection-7-properties", "CC08"},   // 8th AMP's Function
#endif

# ifdef CODEC1_AMP
#  if _NUM_AMP_AMPS>7
        Package (2) {"acpi-acd-collection-8-properties", "CC09"},   // 1st Codec AMP's Function
#  else
#   if _NUM_AMP_AMPS>6
        Package (2) {"acpi-acd-collection-7-properties", "CC09"},   // 1st Codec AMP's Function
#   else
#    if _NUM_AMP_AMPS>5
        Package (2) {"acpi-acd-collection-6-properties", "CC09"},   // 1st Codec AMP's Function
#    else
#     if _NUM_AMP_AMPS>4
        Package (2) {"acpi-acd-collection-5-properties", "CC09"},   // 1st Codec AMP's Function
#     else
#      if _NUM_AMP_AMPS>3
        Package (2) {"acpi-acd-collection-4-properties", "CC09"},   // 1st Codec AMP's Function
#      else
#       if _NUM_AMP_AMPS>2
        Package (2) {"acpi-acd-collection-3-properties", "CC09"},   // 1st Codec AMP's Function
#       else
#        if _NUM_AMP_AMPS>1
        Package (2) {"acpi-acd-collection-2-properties", "CC09"},   // 1st Codec AMP's Function
#        else
#         if _NUM_AMP_AMPS>0
        Package (2) {"acpi-acd-collection-1-properties", "CC09"},   // 1st Codec AMP's Function
#         else
        Package (2) {"acpi-acd-collection-0-properties", "CC09"},   // 1st Codec AMP's Function
#         endif // _NUM_AMP_AMPS>0
#        endif // _NUM_AMP_AMPS>1
#       endif // _NUM_AMP_AMPS>2
#      endif // _NUM_AMP_AMPS>3
#     endif // _NUM_AMP_AMPS>4
#    endif // _NUM_AMP_AMPS>5
#   endif // _NUM_AMP_AMPS>6
#  endif // _NUM_AMP_AMPS>7
# endif // CODEC1_AMP
    }
}) //End AG00

# if _NUM_AMP_AMPS>1
Name(CC02, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-device-namestring", AMP2_AMP_DEV_NAME},
        Package (2) {"acpi-acd-device-type", 1},        // 0: Generic, 1: SoundWire
        Package (2) {"acpi-acd-sdca-terminal-id", 0xE}, // Entity id of the Analog terminal used for this endpoint
        Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),     // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC02
# endif

# if _NUM_AMP_AMPS>2
Name(CC03, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-device-namestring", AMP3_AMP_DEV_NAME},
        Package (2) {"acpi-acd-device-type", 1},        // 0: Generic, 1: SoundWire
        Package (2) {"acpi-acd-sdca-terminal-id", 0xE}, // Entity id of the Analog terminal used for this endpoint
        Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),     // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC03
# endif

# if _NUM_AMP_AMPS>3
Name(CC04, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-device-namestring", AMP4_AMP_DEV_NAME},
        Package (2) {"acpi-acd-device-type", 1},        // 0: Generic, 1: SoundWire
        Package (2) {"acpi-acd-sdca-terminal-id", 0xE}, // Entity id of the Analog terminal used for this endpoint
        Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),     // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC04
# endif

# if _NUM_AMP_AMPS>4
Name(CC05, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-device-namestring", AMP5_AMP_DEV_NAME},
        Package (2) {"acpi-acd-device-type", 1},        // 0: Generic, 1: SoundWire
        Package (2) {"acpi-acd-sdca-terminal-id", 0xE}, // Entity id of the Analog terminal used for this endpoint
        Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),     // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC05
# endif

# if _NUM_AMP_AMPS>5
Name(CC06, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-device-namestring", AMP6_AMP_DEV_NAME},
        Package (2) {"acpi-acd-device-type", 1},        // 0: Generic, 1: SoundWire
        Package (2) {"acpi-acd-sdca-terminal-id", 0xE}, // Entity id of the Analog terminal used for this endpoint
        Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),     // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC06
# endif

# if _NUM_AMP_AMPS>6
Name(CC07, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-device-namestring", AMP7_AMP_DEV_NAME},
        Package (2) {"acpi-acd-device-type", 1},        // 0: Generic, 1: SoundWire
        Package (2) {"acpi-acd-sdca-terminal-id", 0xE}, // Entity id of the Analog terminal used for this endpoint
        Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),     // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC07
# endif

# if _NUM_AMP_AMPS>7
Name(CC08, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-device-namestring", AMP8_AMP_DEV_NAME},
        Package (2) {"acpi-acd-device-type", 1},        // 0: Generic, 1: SoundWire
        Package (2) {"acpi-acd-sdca-terminal-id", 0xE}, // Entity id of the Analog terminal used for this endpoint
        Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),     // Hierarchical Data Extension UUID
    Package () {
        Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC08
# endif
#endif  // (_NUM_AMP_AMPS + _NUM_CODEC_AMPS)>1
#endif  // (_NUM_AMP_AMPS + _NUM_CODEC_AMPS)>0

#ifdef CODEC1_UID
# ifdef CODEC1_AMP
Name(CC09, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", CODEC1_AMP_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0xC},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC09
# endif  // CODEC1_AMP

# ifdef CODEC1_MIC
Name(AC02, Package() {    // This package is shared by all Codec devices in this composition table
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"msft-acx-factory-circuit", 0},    // Not an ACX Factory
        Package (2) {"msft-acx-core-circuit", 1},       // ACX Core circuit
    }
}) //End AC02
# else // !CODEC1_MIC
#  ifdef CODEC1_UAJ
Name(AC02, Package() {    // This package is shared by all Codec devices in this composition table
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"msft-acx-factory-circuit", 0},    // Not an ACX Factory
        Package (2) {"msft-acx-core-circuit", 1},       // ACX Core circuit
    }
}) //End AC02
#  endif // CODEC1_UAJ
# endif // CODEC1_MIC

# ifdef CODEC1_MIC
Name(EP01, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "Microphone"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 2},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC10"},
    }
}) //End EP01

Name(EC10, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "Microphone_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC10"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC11"},    // Sdca Audio Function Configuration
    }
}) //End EC10

Name(CC10, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", DSP_ACPI_ACD_DEVICE_NAMESTRING},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN01"},
    }
}) //End CC10

Name(VN01, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-vendor-id", 0x1},
        Package (2) {"acpi-vendor-config-type", "Streaming_MicrophoneArray"},
#  ifdef _AMD
        Package (2) {"amd-sdw-default-stream-dp-number", CODEC1_MIC_CAPTURE_DATA_PORT},  // Raw Stream Data port number on Mic device
#  else // !_AMD
#   ifdef _NVIDIA
        // NVidia_Arm specific information for Microphone Array
        Package (2) {"acpi-vendor-sdw-data-port-number", CODEC1_MIC_CAPTURE_DATA_PORT},
#    if CODEC1_MIC_CHANNEL_CNT > 2
        Package (2) {"acpi-vendor-device-dp-channel-mask", CODEC1_MIC_CHANNEL_MASK},
        Package (2) {"acpi-vendor-device-terminal-id", 0xA}, // Entity id of the streaming terminal used for this endpoint. (OT113)
#    endif
#   else // !_NVIDIA
        Package (2) {"acpi-acd-sdca-terminal-type", 0x0205},
        Package (2) {"acpi-acd-connection-count", 1},
        // Microphone Array Capture stream (raw)
        Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
        Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
        Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", CODEC1_MIC_CAPTURE_DATA_PORT},
#    if CODEC1_MIC_CHANNEL_CNT > 2
        // multi-channel capture, applicable for > 2xDMIC
        Package ()  {"conn-info",
            Package () {
                // Per connection / stream
                Package () {"conn-agg-path-0",
                    Package () {
                        Package (2) {"sdca-stream-type", 0x0x181}, // Microphone Array Capture Stream Terminal type = Raw
                        Package (2) {"agg-channel-count", CODEC1_MIC_CHANNEL_CNT}, // Channel count of enumerated endpoint
                        Package (2) {"agg-component-count", 0x1}, // No peripheral/function aggregation
                        // Per Peripheral
                        Package () {"agg-component-0",
                            Package () {
                                Package(2) {"function-manufacturer-id", CODEC1_MANUFACTURER_ID},
                                Package(2) {"function-id", CODEC1_FUNC_ID},
                                Package(2) {"controller-id", 0x0},
                                Package(2) {"link-id", CODEC1_LID},
                                Package(2) {"unique-id", CODEC1_UID},
                                Package(2) {"function-number", 0x2},
                                Package(2) {"function-type", 0x3}, // Smart MIC
                                Package(2) {"terminal-entity-id", 0xA}, // Entity id of the streaming terminal used for this endpoint. (OT113)
                                Package(2) {"dp-map", 0x1}, // Dataport map, DP_Index_A
                                Package(2) {"dp-peripheral-mask", CODEC1_MIC_PERIPHERAL_MASK}, // Channel mask on peripheral, KSAUDIO_SPEAKER_QUAD, etc.
                                Package () {"data-port-map-0",
                                    Package () {
                                        Package (2) {"data-port-num", CODEC1_MIC_CAPTURE_DATA_PORT},
                                        Package (2) {"data-port-mode", 0x1}, // Isochronous
                                        Package (2) {"data-port-channel-mask", CODEC1_MIC_CHANNEL_MASK}, // Forms multi-channel microphone
                                    },
                                },
                            },
                        },
                    },
                },
            },
        },
#    endif
#   endif // _NVIDIA
#  endif // _AMD
    }
}) //End VN01

Name(CC11, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", CODEC1_MIC_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       // Circuit composition driver looks for this entity ID to select the graph
       // This is used in conjuntion with namestring, to differentiate speaker vs other function. For render, work backwards.
       // Specify *analog* terminal, not the streaming terminal.
       //     --> abb.: I'm using OT for playback
       Package (2) {"acpi-acd-sdca-terminal-id", 0x4},            // Entity id of the Analog terminal used for this endpoint. (IT11)
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0205},        // Sdca Terminal Type based on Sdca Version implemented by Audio Function (Microphone Array Transducer Input)
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
    }
}) //End CC11
# endif // CODEC1_MIC

# ifdef CODEC1_UAJ
Name(EP02, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "Headphones"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 3},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC20"},
    }
}) //End EP02

Name(EC20, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "Headphones_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC20"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC21"},    // Sdca Audio Function Configuration
    }
}) //End EC20

Name(CC20, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", DSP_ACPI_ACD_DEVICE_NAMESTRING},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN02"},
    }
}) //End CC20

Name(VN02, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-vendor-id", 0x1},
        Package (2) {"acpi-vendor-config-type", "Streaming_Headphones"},
#  ifdef _AMD
        Package (2) {"amd-sdw-default-stream-dp-number", CODEC1_UAJ_RENDER_DATA_PORT},   // Raw Stream Data port number on SPK device
#   else // !_AMD
#   ifdef _NVIDIA
        // NVidia_Arm specific information for Headphone
        Package (2) {"acpi-vendor-sdw-data-port-number", CODEC1_UAJ_RENDER_DATA_PORT},
#   else // !_NVIDIA
        Package (2) {"acpi-vendor-sdca-terminal-type", 0x06c0},
        Package (2) {"acpi-acd-connection-count", 1},
        Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
        Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
        Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", CODEC1_UAJ_RENDER_DATA_PORT},
#   endif // _NVIDIA
#  endif // _AMD
    }
}) //End VN02

Name(EP03, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "LineOut"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 3},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC30"},
    }
}) //End EP03

Name(EC30, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "LineOut_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC30"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC31"},    // Sdca Audio Function Configuration
    }
}) //End EC30

Name(CC30, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", DSP_ACPI_ACD_DEVICE_NAMESTRING},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN03"},
    }
}) //End CC30

Name(VN03, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-vendor-id", 0x1},
        Package (2) {"acpi-vendor-config-type", "Streaming_LineOut"},
#  ifdef _AMD
        Package (2) {"amd-sdw-default-stream-dp-number", CODEC1_UAJ_RENDER_DATA_PORT},   // Raw Stream Data port number on SPK device
#  else // !_AMD
#   ifdef _NVIDIA
        // NVidia_Arm specific information for Line-Out
        Package (2) {"acpi-vendor-sdw-data-port-number", CODEC1_UAJ_RENDER_DATA_PORT},
#   else // !_NVIDIA
        Package (2) {"acpi-vendor-sdca-terminal-type", 0x0690},
        Package (2) {"acpi-acd-connection-count", 1},
        Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
        Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
        Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", CODEC1_UAJ_RENDER_DATA_PORT},
#   endif // _NVIDIA
#  endif // _AMD
    }
}) //End VN03

Name(EP04, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "HeadsetOutput"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 3},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC40"},
    }
}) //End EP04

Name(EC40, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "HeadsetOutput_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC40"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC41"},    // Sdca Audio Function Configuration
    }
}) //End EC40

Name(CC40, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", DSP_ACPI_ACD_DEVICE_NAMESTRING},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN04"},
    }
}) //End CC40

Name(VN04, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-vendor-id", 0x1},
        Package (2) {"acpi-vendor-config-type", "Streaming_HeadsetOutput"},
#  ifdef _AMD
        Package (2) {"amd-sdw-default-stream-dp-number", CODEC1_UAJ_RENDER_DATA_PORT},   // Raw Stream Data port number on SPK device
#  else // !_AMD
#   ifdef _NVIDIA
        // NVidia_Arm specific information for Headset
        Package (2) {"acpi-vendor-sdw-data-port-number", CODEC1_UAJ_RENDER_DATA_PORT},
#   else // !_NVIDIA
        Package (2) {"acpi-vendor-sdca-terminal-type", 0x06D0},
        Package (2) {"acpi-acd-connection-count", 1},
        Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
        Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
        Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", CODEC1_UAJ_RENDER_DATA_PORT},
#   endif // _NVIDIA
#  endif // _AMD
    }
}) //End VN04

Name(EP05, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "Microphone"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 4},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC50"},
    }
}) //End EP05

Name(EC50, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "Microphone_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC50"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC51"},    // Sdca Audio Function Configuration
    }
}) //End EC50

Name(CC50, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", DSP_ACPI_ACD_DEVICE_NAMESTRING},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN05"},
    }
}) //End CC50

Name(VN05, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-vendor-id", 0x1},
        Package (2) {"acpi-vendor-config-type", "Streaming_Microphone"},
#  ifdef _AMD
        Package (2) {"amd-sdw-default-stream-dp-number", CODEC1_UAJ_CAPTURE_DATA_PORT},  // Raw Stream Data port number on SPK device
#  else // !_AMD
#   ifdef _NVIDIA
        // NVidia_Arm specific information for Microphone
        Package (2) {"acpi-vendor-sdw-data-port-number", CODEC1_UAJ_CAPTURE_DATA_PORT},
#   else // !_NVIDIA
        Package (2) {"acpi-vendor-sdca-terminal-type", 0x06a0},
        Package (2) {"acpi-acd-connection-count", 1},
        Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
        Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
        Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", CODEC1_UAJ_CAPTURE_DATA_PORT},
#   endif // _NVIDIA
#  endif // _AMD
    }
}) //End VN05

Name(EP06, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "LineIn"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 4},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC60"},
    }
}) //End EP06

Name(EC60, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "LineIn_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC60"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC61"},    // Sdca Audio Function Configuration
    }
}) //End EC60

Name(CC60, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", DSP_ACPI_ACD_DEVICE_NAMESTRING},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN06"},
    }
}) //End CC60

Name(VN06, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-vendor-id", 0x1},
        Package (2) {"acpi-vendor-config-type", "Streaming_LineIn"},
#  ifdef _AMD
        Package (2) {"amd-sdw-default-stream-dp-number", CODEC1_UAJ_CAPTURE_DATA_PORT},  // Raw Stream Data port number on SPK device
#  else // !_AMD
#   ifdef _NVIDIA
        // NVidia_Arm specific information for Line-In
        Package (2) {"acpi-vendor-sdw-data-port-number", CODEC1_UAJ_CAPTURE_DATA_PORT},
#   else // !_NVIDIA
        Package (2) {"acpi-vendor-sdca-terminal-type", 0x0680},
        Package (2) {"acpi-acd-connection-count", 1},
        Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
        Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
        Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", CODEC1_UAJ_CAPTURE_DATA_PORT},
#   endif // _NVIDIA
#  endif // _AMD
    }
}) //End VN06

Name(EP07, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "HeadsetMic"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 4},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC70"},
    }
}) //End EP07

Name(EC70, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "HeadsetMic_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC70"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC71"},    // Sdca Audio Function Configuration
    }
}) //End EC70

Name(CC70, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", DSP_ACPI_ACD_DEVICE_NAMESTRING},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN07"},
    }
}) //End CC70

Name(VN07, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-vendor-id", 0x1},
        Package (2) {"acpi-vendor-config-type", "Streaming_HeadsetMic"},
#  ifdef _AMD
        Package (2) {"amd-sdw-default-stream-dp-number", CODEC1_UAJ_CAPTURE_DATA_PORT},  // Raw Stream Data port number on SPK device
#  else // !_AMD
#   ifdef _NVIDIA
        // NVidia_Arm specific information for Headset Microphone
        Package (2) {"acpi-vendor-sdw-data-port-number", CODEC1_UAJ_CAPTURE_DATA_PORT},
#   else // !_NVIDIA
        Package (2) {"acpi-vendor-sdca-terminal-type", 0x06d0},
        Package (2) {"acpi-acd-connection-count", 1},
        Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
        Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
        Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", CODEC1_UAJ_CAPTURE_DATA_PORT},
#   endif // _NVIDIA
#  endif // _AMD
    }
}) //End VN07
# endif // CODEC1_UAJ
#endif


//
// Cleanup
//
#ifdef AMP1_RENDER_DATA_PORT
# undef AMP1_RENDER_DATA_PORT
#endif

#ifdef AMP1_US_DATA_PORT
# undef AMP1_US_DATA_PORT
#endif

#ifdef AMP1_SENSE_DATA_PORT
# undef AMP1_SENSE_DATA_PORT
#endif

#ifdef AMP1_REF_DATA_PORT
# undef AMP1_REF_DATA_PORT
#endif

#ifdef AMP2_RENDER_DATA_PORT
# undef AMP2_RENDER_DATA_PORT
#endif

#ifdef AMP2_US_DATA_PORT
# undef AMP2_US_DATA_PORT
#endif

#ifdef AMP2_SENSE_DATA_PORT
# undef AMP2_SENSE_DATA_PORT
#endif

#ifdef AMP2_REF_DATA_PORT
# undef AMP2_REF_DATA_PORT
#endif

#ifdef AMP3_RENDER_DATA_PORT
# undef AMP3_RENDER_DATA_PORT
#endif

#ifdef AMP3_US_DATA_PORT
# undef AMP3_US_DATA_PORT
#endif

#ifdef AMP3_SENSE_DATA_PORT
# undef AMP3_SENSE_DATA_PORT
#endif

#ifdef AMP3_REF_DATA_PORT
# undef AMP3_REF_DATA_PORT
#endif

#ifdef AMP4_RENDER_DATA_PORT
# undef AMP4_RENDER_DATA_PORT
#endif

#ifdef AMP4_US_DATA_PORT
# undef AMP4_US_DATA_PORT
#endif

#ifdef AMP4_SENSE_DATA_PORT
# undef AMP4_SENSE_DATA_PORT
#endif

#ifdef AMP4_REF_DATA_PORT
# undef AMP4_REF_DATA_PORT
#endif

#ifdef AMP5_RENDER_DATA_PORT
# undef AMP5_RENDER_DATA_PORT
#endif

#ifdef AMP5_US_DATA_PORT
# undef AMP5_US_DATA_PORT
#endif

#ifdef AMP5_SENSE_DATA_PORT
# undef AMP5_SENSE_DATA_PORT
#endif

#ifdef AMP5_REF_DATA_PORT
# undef AMP5_REF_DATA_PORT
#endif

#ifdef AMP6_RENDER_DATA_PORT
# undef AMP6_RENDER_DATA_PORT
#endif

#ifdef AMP6_US_DATA_PORT
# undef AMP6_US_DATA_PORT
#endif

#ifdef AMP6_SENSE_DATA_PORT
# undef AMP6_SENSE_DATA_PORT
#endif

#ifdef AMP6_REF_DATA_PORT
# undef AMP6_REF_DATA_PORT
#endif

#ifdef AMP7_RENDER_DATA_PORT
# undef AMP7_RENDER_DATA_PORT
#endif

#ifdef AMP7_US_DATA_PORT
# undef AMP7_US_DATA_PORT
#endif

#ifdef AMP7_SENSE_DATA_PORT
# undef AMP7_SENSE_DATA_PORT
#endif

#ifdef AMP7_REF_DATA_PORT
# undef AMP7_REF_DATA_PORT
#endif

#ifdef AMP8_RENDER_DATA_PORT
# undef AMP8_RENDER_DATA_PORT
#endif

#ifdef AMP8_US_DATA_PORT
# undef AMP8_US_DATA_PORT
#endif

#ifdef AMP8_SENSE_DATA_PORT
# undef AMP8_SENSE_DATA_PORT
#endif

#ifdef AMP8_REF_DATA_PORT
# undef AMP8_REF_DATA_PORT
#endif

#ifdef CODEC1_MIC_CAPTURE_DATA_PORT
# undef CODEC1_MIC_CAPTURE_DATA_PORT
#endif

#ifdef CODEC1_UAJ_CAPTURE_DATA_PORT
# undef CODEC1_UAJ_CAPTURE_DATA_PORT
#endif

#ifdef CODEC1_AMP_REF_DATA_PORT
# undef CODEC1_AMP_REF_DATA_PORT
#endif

#ifdef CODEC1_AMP_RENDER_DATA_PORT
# undef CODEC1_AMP_RENDER_DATA_PORT
#endif

#ifdef CODEC1_UAJ_RENDER_DATA_PORT
# undef CODEC1_UAJ_RENDER_DATA_PORT
#endif

#ifdef CODEC1_MIC_CHANNEL_CNT
# undef CODEC1_MIC_CHANNEL_CNT
#endif

#ifdef CODEC1_MIC_CHANNEL_MASK
# undef CODEC1_MIC_CHANNEL_MASK
#endif

#ifdef CODEC1_MIC_PERIPHERAL_MASK
# undef CODEC1_MIC_PERIPHERAL_MASK
#endif
