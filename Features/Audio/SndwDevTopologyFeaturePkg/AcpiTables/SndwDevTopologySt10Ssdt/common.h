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
#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * Bitmasks which define which features are supported in the XU driver.
 */
#define FEATURE_BIT_HWKWS       0x00000001       /* HWKWS is supported. */
#define FEATURE_BIT_WT          0x00000002       /* Woofer Tweeter is supported. */
#define FEATURE_BIT_KNCK        0x00000004       /* Cohen + Knuckles is supported. */
#define FEATURE_BIT_NO_FUN_STS  0x00000008       /* Opt-out XU from handling Function_Status bits. */
#define FEATURE_BIT_NO_VOL_MUTE_C   0x00000010   /* Opt-out XU from creating capture volume controls. */
#define FEATURE_BIT_NO_VOL_MUTE_R   0x00000020   /* Opt-out XU from creating render volume controls. */

//
// Supported jack types mask for Cohen and Phife codecs
// (Based on eDetectedMode type.)
// To disable jack type define it as 0 before common.h file is included.
// Example:
// #define COHEN_PHIFE_UAJ_LINE_IN_EN (0)
// #include "SndwDevTopologySt10Ssdt/common.h"
//
#ifndef COHEN_PHIFE_UAJ_UNKNOWN_EN
# define COHEN_PHIFE_UAJ_UNKNOWN_EN   (1 << 1)
#endif
#ifndef COHEN_PHIFE_UAJ_HEADPHONE_EN
# define COHEN_PHIFE_UAJ_HEADPHONE_EN (1 << 3)
#endif
#ifndef COHEN_PHIFE_UAJ_HEADSET_EN
# define COHEN_PHIFE_UAJ_HEADSET_EN   (1 << 4)
#endif
#ifndef COHEN_PHIFE_UAJ_LINE_OUT_EN
# define COHEN_PHIFE_UAJ_LINE_OUT_EN  (1 << 5)
#endif
#ifndef COHEN_PHIFE_UAJ_LINE_IN_EN
# define COHEN_PHIFE_UAJ_LINE_IN_EN   (1 << 6)
#endif
#ifndef COHEN_PHIFE_UAJ_MIC_EN
# define COHEN_PHIFE_UAJ_MIC_EN       (1 << 7)
#endif

/*
 * Set the I2C address of Knuckles depending on whether it is CSP or QFN
 */
#ifdef COHEN_KNUCKLES_QFN
# define KNUCKLES_I2C_ADDRESS 0x31
// Also define COHEN_KNUCKLES to enable the Knuckles parts
# define COHEN_KNUCKLES
#else
# define KNUCKLES_I2C_ADDRESS 0x30
#endif

//
// COHEN_BRIDGE enables the Cohen amp function to drive two Cohen tweeters and
// two Jamerson woofers connected via ASP. COHEN_TWEETER_JAMERSON_WOOFER is
// the legacy define and is still supported, but differs from COHEN_BRIDGE in
// that it also implies COHEN_ONLY, which removes the non-bridge Jamersons
// from the ACPI tables.
//
#ifdef COHEN_TWEETER_JAMERSON_WOOFER
# define COHEN_BRIDGE
# define COHEN_ONLY
#endif
#ifdef COHEN_BRIDGE
# define COHEN_AMP
#endif

/*
 * Enable features based on the command line options.
 *
 * When adding new feature bits, be sure to update each asl file which references
 * these bits.  Ideally, they would be included in a single #define which was
 * included in the asl files, but IASL does not allow for substitutions within
 * #defines.  For the same reason the bits reserved with the above defines
 * cannot be used here.  However, they are kept for reference
 * and perhaps a future version will allow simple substitutions.
 */
#ifdef HWKWS
# define FEATURE_ENABLE_HWKWS 0x00000001
#else
# define FEATURE_ENABLE_HWKWS 0x00000000
#endif

#ifdef COHEN_BRIDGE
# define FEATURE_ENABLE_WT 0x00000002
#else
# define FEATURE_ENABLE_WT 0x00000000
#endif

#ifdef COHEN_KNUCKLES
# define FEATURE_ENABLE_KNCK 0x00000004
#else
# define FEATURE_ENABLE_KNCK 0x00000000
#endif

//
// To ensure compatibility with ACPI tables for already release projects,
// opt-out XU driver from handling Function_Status bits.
//
#define FEATURE_NO_FUN_STS 0x00000008

#define FEATURE_STR_HWKWS "_kws"
#define FEATURE_STR_WT    "_wt"
#define FEATURE_STR_KNCK  "_knck"

/*
 * UpX offsets
 */
#ifdef UPX_OFFSET_1000
# define UPX_OFFSET 0x1000
#endif

#ifdef UPX_OFFSET_2000
# define UPX_OFFSET 0x2000
#endif

#ifdef UPX_OFFSET_3000
# define UPX_OFFSET 0x3000
#endif

#ifdef UPX_OFFSET_4000
# define UPX_OFFSET 0x4000
#endif

#ifdef UPX_OFFSET_5000
# define UPX_OFFSET 0x5000
#endif

#ifdef UPX_OFFSET_6000
# define UPX_OFFSET 0x6000
#endif

#ifndef UPX_OFFSET
# define UPX_OFFSET 0
#endif

/*
 * Cluster and usage support is now included by default. Set DISABLE_CLUSTERS
 * and DISABLE_USAGE to override this and prevent the cluster and usage
 * controls from being added to the DisCo data.
 */
#ifndef DISABLE_CLUSTERS
# define ENABLE_CLUSTERS
#endif
#ifndef DISABLE_USAGE
# define ENABLE_USAGE
#endif

/*
 * Hardware-specific constants
 */
#define COHEN_REF_STREAM_DATA_PORT        0x3
#define COHEN_ULS_CAPTURE_DATA_PORT       0x3
#define COHEN_SECONDARY_STREAM_DATA_PORT  0x4
#define COHEN_SPEAKER_RENDER_DATA_PORT    0x5

#define JAMERSON_SPEAKER_RENDER_DATA_PORT 0x1
#define JAMERSON_US_RENDER_DATA_PORT      0x2
#define JAMERSON_REF_STREAM_DATA_PORT     0x4

#define PHIFE_MIC_CAPTURE_DATA_PORT       0x1
#define PHIFE_ULS_CAPTURE_DATA_PORT       0x2
#define PHIFE_SECONDARY_STREAM_DATA_PORT  0x3
#define PHIFE_UAJ_CAPTURE_DATA_PORT       0x4
#define PHIFE_UAJ_RENDER_DATA_PORT        0x5


#define COHEN_SDCA_AMP_FUNC_STATUS_INT            6
#define COHEN_SDCA_MIC_FUNC_STATUS_INT            7
#define COHEN_SDCA_UAJ_FUNC_STATUS_INT            8
#define COHEN_SDCA_HID_FUNC_STATUS_INT            9
#define COHEN_SDCA_HID_FDL_BUFFER_OWNER_CHNG_INT  4
#define COHEN_SDCA_UAJ_JACK_MODE_UPDATE_INT       11
#define COHEN_SDCA_MIC_TRIGGET_STATUS_INT         13

#define JAMERSON_B_SDCA_FUNC_STATUS_INT           1
#define JAMERSON_B_SDCA_FDL_CURRENT_OWNER_INT     8
#define JAMERSON_B_SDCA_PROTECTION_MODE_INT       10

#define PHIFE_SDCA_MIC_FUNC_STATUS_INT            2
#define PHIFE_SDCA_UAJ_FUNC_STATUS_INT            3
#define PHIFE_SDCA_HID_FUNC_STATUS_INT            4
#define PHIFE_SDCA_HID_FDL_BUFFER_OWNER_CHNG_INT  10
#define PHIFE_SDCA_UAJ_JACK_MODE_UPDATE_INT       12
#define PHIFE_SDCA_MIC_TRIGGET_STATUS_INT         14


/*
 * AEC
 * This assumes 4 amps, if more are needed, then this needs to be overloaded in the main dsdt file.
 */
#ifndef INTEL_DSP_NUM_AMPS
# define INTEL_DSP_NUM_AMPS            4
#endif

#ifndef DISABLE_AEC
# ifndef INTEL_DSP_AEC_ENABLE
#  define INTEL_DSP_AEC_ENABLE        1
# endif
#else
# undef INTEL_DSP_AEC_ENABLE
# define INTEL_DSP_AEC_ENABLE         0
#endif

//
// COHEN_KNUCKLES_QFN used to set COHEN_ONLY to keep the Jamersons out of the
// ACPI tables. This behavior is maintained for compatibility, but there may be
// future configuration with Knuckles and Jamersons.
//
#ifdef COHEN_KNUCKLES_QFN
# define COHEN_ONLY
#endif

//
// COHEN_ONLY enables the Cohen amp function, but also disables Jamerson
// functions from being added to ACPI. This preserves legacy behavior of
// this preprocessor symbol.
//
#ifdef COHEN_ONLY
# define COHEN_AMP
#endif

//
// COHEN_JAMERSON_AMP_AGGREGATION requires disabling the reference stream in
// Cohen and Jamerson and the ultrasonic stream in Jamerson, and also enables
// the Cohen amp function.
//
#ifdef COHEN_JAMERSON_AMP_AGGREGATION
# define COHEN_AMP
# define COHEN_DISABLE_REF_STREAM
# define JAMERSON_DISABLE_REF_STREAM
# define DISABLE_US_STREAM
#endif

#ifdef SIDECAR_GPIO_SPEAKER_SELECT
# ifndef COHEN_BRIDGE
#  error SIDECAR_GPIO_SPEAKER_SELECT requires COHEN_BRIDGE to be set
# endif
# ifndef SIDECAR_GPIO_RESOURCE_DESCRIPTOR
#  error SIDECAR_GPIO_SPEAKER_SELECT requires SIDECAR_GPIO_RESOURCE_DESCRIPTOR to be specified
# endif
#endif

//
// SIDECAR_VARIABLE_SPEAKER_SELECT allows reading the speaker set ID to use for
// the speaker source from the 01fa-spk-id-val vendor-specific DisCo property
// when the SpeakerSetSource registry entry set by the extension .inf is set to
// 3 (DisCo). The SIDECAR_VARIABLE_SPEAKER_SELECT_NAME is either the name of a
// four-character variable from which to source the ID, or a constant value for
// use in testing.
//
#ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
# ifndef COHEN_BRIDGE
#  error SIDECAR_VARIABLE_SPEAKER_SELECT requires COHEN_BRIDGE to be set
# endif
# ifndef SIDECAR_VARIABLE_SPEAKER_SELECT_NAME
#  error SIDECAR_VARIABLE_SPEAKER_SELECT requires SIDECAR_VARIABLE_SPEAKER_SELECT_NAME to be set
# endif
#endif

//
// AMP_VARIABLE_SPEAKER_SELECT allows reading the speaker set ID to use for
// the speaker source from the 01fa-spk-id-val vendor-specific DisCo property
// when the SpeakerSetSource registry entry set by the extension .inf is set to
// 3 (DisCo). The SIDECAR_VARIABLE_SPEAKER_SELECT_NAME is either the name of a
// four-character variable from which to source the ID, or a constant value for
// use in testing.
//
#ifdef AMP_VARIABLE_SPEAKER_SELECT
# ifndef AMP_VARIABLE_SPEAKER_SELECT_NAME
#  error AMP_VARIABLE_SPEAKER_SELECT requires SIDECAR_VARIABLE_SPEAKER_SELECT_NAME to be set
# endif
#endif

//
// The Linux drivers are generalized and do not have an .inf like configuration
// file for each platform to select between competing sources of information -
// only one source for the speaker ID should be enabled at a time.
//
#ifdef COHEN_BRIDGE
# ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
#  ifdef SIDECAR_GPIO_SPEAKER_SELECT
#   error Multiple SPEAKER_SELECT sources defined
#  endif
# endif
#endif

#ifdef COHEN_BRIDGE
//
// Bridge configuration, COHEN_NEED_CONFIGS = 0x800000B3
// Include GPIO and min init configurations.
//
# define COHEN_NEED_CONFIGS_SIDECAR
# define COHEN_NEED_CONFIGS_GPIO
#endif

//
// Standard configuration, COHEN_NEED_CONFIGS = 0x80000031
//
#ifndef COHEN_EXCLUDE_CONFIGS_JACK
# ifndef COHEN_NEED_CONFIGS_JACK
#  define COHEN_NEED_CONFIGS_JACK
# endif
#endif

#ifndef COHEN_NEED_CONFIGS_CODEC
# define COHEN_NEED_CONFIGS_CODEC
#endif

#ifndef COHEN_NEED_CONFIGS_PLL
# define COHEN_NEED_CONFIGS_PLL
#endif

//
// All 256 possible values of COHEN_NEED_CONFIGS_VAL_0C, the NEED_CONFIGS LSB.
//
#include <SndwDevTopologySt10Ssdt/CS42L43_NEED_CONFIGS.asl>

/*
 * Entity Types
 */
#define ENTITY_TYPE_IT    0x02
#define ENTITY_TYPE_OT    0x03
#define ENTITY_TYPE_MU    0x05
#define ENTITY_TYPE_SU    0x06
#define ENTITY_TYPE_FU    0x07
#define ENTITY_TYPE_XU    0x0A
#define ENTITY_TYPE_CS    0x0B
#define ENTITY_TYPE_CX    0x0C
#define ENTITY_TYPE_PDE   0x11
#define ENTITY_TYPE_GE    0x12
#define ENTITY_TYPE_SPE   0x13
#define ENTITY_TYPE_CRU   0x20
#define ENTITY_TYPE_UDMPU 0x21
#define ENTITY_TYPE_MFPU  0x22
#define ENTITY_TYPE_SMPU  0x23
#define ENTITY_TYPE_SAPU  0x24
#define ENTITY_TYPE_PPU   0x25
#define ENTITY_TYPE_TG    0x30
#define ENTITY_TYPE_HIDE  0x31

/*
 * Controls
 */
#define CTL_IT_MIC_BIAS                   (1 << 0x03)
#ifndef ENABLE_USAGE
# define CTL_IT_USAGE                     0
#else
# define CTL_IT_USAGE                     (1 << 0x04)
#endif
#define CTL_IT_LATENCY                    (1 << 0x08)
#ifndef ENABLE_CLUSTERS
# define CTL_IT_CLUSTERINDEX              0
#else
# define CTL_IT_CLUSTERINDEX               (1 << 0x10)
#endif
#define CTL_IT_DATAPORT_SELECTOR          (1 << 0x11)
#define CTL_IT_MATCHING_GUID              (1 << 0x12)
#define CTL_IT_KEEPALIVE                  (1 << 0x13)
#define CTL_IT_NDAI_STREAM                (1 << 0x14)
#define CTL_IT_NDAI_CATAGORY              (1 << 0x15)
#define CTL_IT_NDAI_CODINGTYPE            (1 << 0x16)
#define CTL_IT_NDAI_PACKETTYPE            (1 << 0x17)

#ifndef ENABLE_USAGE
# define CTL_OT_USAGE                      0
#else
# define CTL_OT_USAGE                      (1 << 0x04)
#endif
#define CTL_OT_LATENCY                    (1 << 0x08)
#define CTL_OT_DATAPORT_SELECTOR          (1 << 0x11)
#define CTL_OT_MATCHING_GUID              (1 << 0x12)
#define CTL_OT_KEEPALIVE                  (1 << 0x13)
#define CTL_OT_NDAI_STREAM                (1 << 0x14)
#define CTL_OT_NDAI_CATAGORY              (1 << 0x15)
#define CTL_OT_NDAI_CODINGTYPE            (1 << 0x16)
#define CTL_OT_NDAI_PACKETTYPE            (1 << 0x17)

#define CTL_CRU_LATENCY                   (1 << 0x06)
#ifndef ENABLE_CLUSTERS
# define CTL_CRU_CLUSTERINDEX             0
#else
# define CTL_CRU_CLUSTERINDEX             (1 << 0x10)
#endif

#define CTL_FU_MUTE                       (1 << 0x01)
#define CTL_FU_VOLUME                     (1 << 0x02)
#define CTL_FU_AGC                        (1 << 0x07)
#define CTL_FU_BASSBOOST                  (1 << 0x09)
#define CTL_FU_LOUDNESS                   (1 << 0x0A)
#define CTL_FU_CHANNELGAIN                (1 << 0x0B)
#define CTL_FU_LATENCY                    (1 << 0x10)

#define CTL_MFPU_BYPASS                   (1 << 0x01)
#define CTL_MFPU_ALGORITHM_READY          (1 << 0x04)
#define CTL_MFPU_ALGORITHM_ENABLE         (1 << 0x05)
#define CTL_MFPU_LATENCY                  (1 << 0x08)
#define CTL_MFPU_ALGORITHM_PREPARE        (1 << 0x09)
#ifndef ENABLE_CLUSTERS
# define CTL_MFPU_CLUSTER_INDEX           0
#else
# define CTL_MFPU_CLUSTER_INDEX           (1 << 0x10)
#endif
#define CTL_MFPU_CENTER_FREQ_INDEX        (1 << 0x11)
#define CTL_MFPU_ULTRASOUND_LEVEL         (1 << 0x12)
#define CTL_MFPU_AE_NUMBER                (1 << 0x13)
#define CTL_MFPU_AE_CURRENT_OWNER         (1 << 0x14)
#define CTL_MFPU_AE_MESSAGEOFFSET         (1 << 0x16)
#define CTL_MFPU_AE_MESSAGELENGTH         (1 << 0x17)

#define CTL_MU_MIXER                      (1 << 0x01)
#define CTL_MU_LATENCY                    (1 << 0x06)

#define CTL_PPU_LATENCY                   (1 << 0x06)
#define CTL_PPU_POSTURENUMBER             (1 << 0x10)
#define CTL_PPU_POSTUREEXTENSION          (1 << 0x11)
#define CTL_PPU_HORIZONTALBALANCE         (1 << 0x12)
#define CTL_PPU_VERTICALBALANCE           (1 << 0x13)

#define CTL_SAPU_LATENCY                  (1 << 0x05)
#define CTL_SAPU_PROTECTION_MODE          (1 << 0x10)
#define CTL_SAPU_PROTECTION_STATUS        (1 << 0x11)
#define CTL_SAPU_OPAQUESETREQ_INDEX       (1 << 0x12)
#define CTL_SAPU_DTODTX_CURRENTOWNER      (1 << 0x14)
#define CTL_SAPU_DTODTX_MESSAGEOFFSET     (1 << 0x16)
#define CTL_SAPU_DTODTX_MESSAGELENGTH     (1 << 0x17)
#define CTL_SAPU_DTODRX_CURRENTOWNER      (1 << 0x18)
#define CTL_SAPU_DTODRX_MESSAGEOFFSET     (1 << 0x1A)
#define CTL_SAPU_DTODRX_MESSAGELENGTH     (1 << 0x1B)

#define CTL_SMPU_LATENCY                  (1 << 0x06)
#define CTL_SMPU_TRIGGER_ENABLE           (1 << 0x10)
#define CTL_SMPU_TRIGGER_STATUS           (1 << 0x11)
#define CTL_SMPU_HIST_BUFFER_MODE         (1 << 0x12)
#define CTL_SMPU_HIST_BUFFER_PREAMBLE     (1 << 0x13)
#define CTL_SMPU_HIST_ERROR               (1 << 0x14)
#define CTL_SMPU_TRIGGER_EXTENSION        (1 << 0x15)
#define CTL_SMPU_TRIGGER_READY            (1 << 0x16)
#define CTL_SMPU_HIST_CURRENTOWNER        (1 << 0x18)
#define CTL_SMPU_HIST_MESSAGEOFFSET       (1 << 0x1A)
#define CTL_SMPU_HIST_MESSAGELENGTH       (1 << 0x1B)
#define CTL_SMPU_DTOCTX_CURRENTOWNER      (1 << 0x1C)
#define CTL_SMPU_DTODTX_MESSAGEOFFSET     (1 << 0x1E)
#define CTL_SMPU_DTODTX_MESSAGELENGTH     (1 << 0x1F)
#define CTL_SMPU_DTODRX_CURRENTOWNER      (1 << 0x20)
#define CTL_SMPU_DTODRX_MESSAGEOFFSET     (1 << 0x22)
#define CTL_SMPU_DTODRX_MESSAGELENGTH     (1 << 0x23)

#define CTL_SU_SELECTOR                   (1 << 0x01)
#define CTL_SU_LATENCY                    (1 << 0x02)

#define CTL_UDMPU_LATENCY                 (1 << 0x06)
#ifndef ENABLE_CLUSTERS
# define CTL_UDMPU_CLUSTERINDEX           0
#else
# define CTL_UDMPU_CLUSTERINDEX           (1 << 0x10)
#endif
#define CTL_UDMPU_ACOUSTIC_ENERGY_LEVEL_MONITOR (1 << 0x11)
#define CTL_UDMPU_ULTRASOUND_LOOP_GAIN    (1 << 0x12)

#define CTL_XU_BYPASS                     (1 << 0x01)
#define CTL_XU_LATENCY                    (1 << 0x06)
#define CTL_XU_ID                         (1 << 0x07)
#define CTL_XU_VERSION                    (1 << 0x08)
#define CTL_XU_FDL_CURRENTOWNER           (1 << 0x10)
#define CTL_XU_FDL_MESSAGEOFFSET          (1 << 0x12)
#define CTL_XU_FDL_MESSAGELENGTH          (1 << 0x13)
#define CTL_XU_FDL_STATUS                 (1 << 0x14)
#define CTL_XU_FDL_SET_INDEX              (1 << 0x15)
#define CTL_XU_FDL_HOST_REQUEST           (1 << 0x16)
#define CTL_XU_IMPDEF_GPIO                (1 << 0x30)

#define CTL_CS_CLOCK_VALID                (1 << 0x02)
#define CTL_CS_SAMPLERATEINDEX            (1 << 0x10)

#define CTL_CX_CLOCK_SELECT               (1 << 0x01)

#define CTL_GE_SELECTED_MODE              (1 << 0x01)
#define CTL_GE_DETECTED_MODE              (1 << 0x02)

#define CTL_PDE_REQUESTED_PS              (1 << 0x01)
#define CTL_PDE_ACTUAL_PS                 (1 << 0x10)

#define CTL_SPE_PRIVATE                   (1 << 0x01)
#define CTL_SPE_PRIVACY_POLICY            (1 << 0x02)
#define CTL_SPE_PRIVACY_LOCK_STATE        (1 << 0x03)
#define CTL_SPE_PRIVACY_OWNER             (1 << 0x04)
#define CTL_SPE_AUTHTX_CURRENT_OWNER      (1 << 0x10)
#define CTL_SPE_AUTHTX_MESSAGEOFFSET      (1 << 0x12)
#define CTL_SPE_AUTHTX_MESSAGELENGTH      (1 << 0x13)
#define CTL_SPE_AUTHRX_CURRENTOWNER       (1 << 0x14)
#define CTL_SPE_AUTHRX_MESSAGEOFFSET      (1 << 0x16)
#define CTL_SPE_AUTHRX_MESSAGELENGTH      (1 << 0x17)

#define CTL_TG_TONE_DIVIDER               (1 << 0x10)

#define CTL_HIDE_CURRENTOWNER             (1 << 0x10)
#define CTL_HIDE_HIDTX_MESSAGEOFFSET      (1 << 0x12)
#define CTL_HIDE_HIDTX_MESSAGELENGTH      (1 << 0x13)
#define CTL_HIDE_HIDRX_CURRENTOWNER       (1 << 0x16)
#define CTL_HIDE_HIDRX_MESSAGELENGTH      (1 << 0x17)

#define CTL_E0_COMMIT_GROUP_MASK          (1 << 0x01)
#define CTL_E0_FUNCTION_SDCA_VERSION      (1 << 0x04)
#define CTL_E0_FUNCTION_TYPE              (1 << 0x05)
#define CTL_E0_FUNCTION_MANUFACTURER_ID   (1 << 0x06)
#define CTL_E0_FUNCTION_ID                (1 << 0x07)
#define CTL_E0_FUNCTION_VERSION           (1 << 0x08)
#define CTL_E0_FUNCTION_EXTENSION_ID      (1 << 0x09)
#define CTL_E0_FUNCTION_EXTENSION_VERSION (1 << 0x0A)
#define CTL_E0_FUNCTION_STATUS            (1 << 0x10)
#define CTL_E0_FUNCTION_ACTION            (1 << 0x11)
#define CTL_E0_MATCHING_GUID              (1 << 0x12)
#define CTL_E0_DEVICE_MANUFACTURER_ID     (1 << 0x2C)
#define CTL_E0_DEVICE_PART_ID             (1 << 0x2D)
#define CTL_E0_DEVICE_VERSION             (1 << 0x2E)
#define CTL_E0_DEVICE_SDCA_VERSION        (1 << 0x2F)

/*
 * Clusters
 */

// Channel Relationship
#define CHR_GENERIC_MONO         0x01
#define CHR_GENERIC_LEFT         0x02
#define CHR_GENERIC_RIGHT        0x03
#define CHR_ARRAY                0x04
#define CHR_FRONT_LEFT           0x0B
#define CHR_FRONT_RIGHT          0x0C
#define CHR_FRONT_CENTER         0x0D
#define CHR_SIDE_LEFT            0x12
#define CHR_SIDE_RIGHT           0x13
#define CHR_BACK_LEFT            0x16
#define CHR_BACK_RIGHT           0x17
#define CHR_GENERIC_TOP          0x48
#define CHR_GENERIC_BOTTOM       0x49
#define CHR_CAPTURE_DIRECT       0x4E
#define CHR_RENDER_DIRECT        0x4F
#define CHR_PRIMARY_TRANSDUCER   0x50
#define CHR_SECONDARY_TRASNDUCER 0x51
#define CHR_MIC                  0x53
#define CHR_RAW                  0x54
#define CHR_SILENCED_MIC         0x56
#define CHR_ECHO_REF_1           0x71
#define CHR_ECHO_REF_2           0x72
#define CHR_ECHO_REF_3           0x73
#define CHR_ECHO_REF_4           0x74
#define CHR_ECHO_REF_ALL         0x75
#define CHR_ECHO_REF_LFE_ALL     0x76

// Posture channel Relationship
#define CHR_EQUIPMENT_LEFT          0x02
#define CHR_EQUIPMENT_RIGHT         0x03
#define CHR_EQUIPMENT_COMBINED      0x47
#define CHR_EQUIPMENT_TOP_LEFT      0x4A
#define CHR_EQUIPMENT_BOTTOM_LEFT   0x4B
#define CHR_EQUIPMENT_TOP_RIGHT     0x4C
#define CHR_EQUIPMENT_BOTTOM_RIGHT  0x4D

/*
 * See section 11.4.3 in in the SDCA Spec, v1.0 for Channel Purpose
 */
#define CHP_GENERIC_AUDIO    0x01
#define CHP_VOICE            0x02
#define CHP_SPEECH           0x03
#define CHP_AMBIENT          0x04
#define CHP_REFERENCE        0x05
#define CHP_ULTRASOUND       0x06
#define CHP_SENSE            0x08
#define CHP_SENSE_V1         0x09
#define CHP_SENSE_V2         0x0A
#define CHP_SENSE_V12I       0x10
#define CHP_SENSE_V21I       0x11
#define CHP_SENSE_V12P       0x12
#define CHP_SENSE_V21P       0x13
#define CHP_SENSE_V1212I     0x14
#define CHP_SENSE_V2121I     0x15
#define CHP_SENSE_V1121I     0x16
#define CHP_SENSE_V2211I     0x17
#define CHP_SENSE_V1212P     0x18
#define CHP_SENSE_V2121P     0x19
#define CHP_SENSE_V1121P     0x1A
#define CHP_SENSE_V2211P     0x1B
#define CHP_SILENCE          0xFE
#define CHP_NON_AUDIO        0xFF

/*
 * See section 7.1.8.2 in the SDCA Spec, v1.0 for Control Access Mode
 * These are numeric values.
 */
#define CAM_READ_WRITE  0
#define CAM_DUAL        1
// Read Writw 1 to Clear
#define CAM_RW1C        2
#define CAM_RO          3
// Read Write 1 to Set
#define CAM_RW1S        4
// Disco Constant
#define CAM_DC          5
/*
 * See section 7.1.9 in the SDCA Spec, v1.0 for Access Layer
 * This is a bit number or bit mask.
 */
#define CAL_USER        (1 << 0)
#define CAL_APPLICATION (1 << 1)
#define CAL_CLASS       (1 << 2)
#define CAL_PLATFORM    (1 << 3)
#define CAL_DEVICE      (1 << 4)
#define CAL_EXTENSION   (1 << 5)

//
// MSFT class driver checks for cluster definitions
// presence on SDCA 1.0 compliant audio functions.
//
#ifndef ENABLE_CLUSTERS
// Must downgrade SDCA revision supported to 0.6r42
# ifdef CTL_E0_FUNCTION_SDCA_VERSION_VAL
#  undef CTL_E0_FUNCTION_SDCA_VERSION_VAL
# endif
// Function_SDCA_Version = SDCA 0.6
# define CTL_E0_FUNCTION_SDCA_VERSION_VAL   0x06

// 'mipi-sdw-sdca-interface-revision'
// Must downgrade SDCA revision supported to 0.9r02
# ifdef MIPI_SDW_SDCA_INTERFACE_REVISION_VAL
#  undef MIPI_SDW_SDCA_INTERFACE_REVISION_VAL
# endif
# define MIPI_SDW_SDCA_INTERFACE_REVISION_VAL    0x0902
#endif

//
// MSFT class driver checks for usage definitions
// presence on SDCA 1.0 compliant audio functions.
//
#ifndef ENABLE_USAGE
// Must downgrade SDCA revision supported to 0.6r42
# ifdef CTL_E0_FUNCTION_SDCA_VERSION_VAL
#  undef CTL_E0_FUNCTION_SDCA_VERSION_VAL
# endif
// Function_SDCA_Version = SDCA 0.6
# define CTL_E0_FUNCTION_SDCA_VERSION_VAL   0x06

// 'mipi-sdw-sdca-interface-revision'
// Must downgrade SDCA revision supported to 0.9r02
# ifdef MIPI_SDW_SDCA_INTERFACE_REVISION_VAL
#  undef MIPI_SDW_SDCA_INTERFACE_REVISION_VAL
# endif
# define MIPI_SDW_SDCA_INTERFACE_REVISION_VAL    0x0902
#endif

//
// The default advertised SDCA version is 1.0
//
#ifndef MIPI_SDW_SDCA_INTERFACE_REVISION_VAL
// mipi-sdw-sdca-interface-revision = SDCA 1.0
# define MIPI_SDW_SDCA_INTERFACE_REVISION_VAL    0x1000
#endif

#ifndef CTL_E0_FUNCTION_SDCA_VERSION_VAL
// Function_SDCA_Version = SDCA 1.0
# define CTL_E0_FUNCTION_SDCA_VERSION_VAL   0x10
#endif

#ifndef CTL_E0_DEVICE_SDCA_VERSION_VAL
// Device_SDCA_Version = SDCA 1.0
# define CTL_E0_DEVICE_SDCA_VERSION_VAL   0x10
#endif

#ifdef UAJ_RENDER_192KHZ_DEFAULT
# ifndef UAJ_RENDER_192KHZ
#  define UAJ_RENDER_192KHZ
# endif
#endif

#ifdef GLOBAL_MUTE_LED_MIC_GPIO_NUM_2
#define GLOBAL_MUTE_LED_MIC_GPIO_NUM 0x2
#endif

#ifdef GLOBAL_MUTE_LED_MIC_GPIO_NUM_3
#define GLOBAL_MUTE_LED_MIC_GPIO_NUM 0x3
#endif

#ifdef GLOBAL_MUTE_LED_SPK_GPIO_NUM_2
#define GLOBAL_MUTE_LED_SPK_GPIO_NUM 0x2
#endif

#ifdef GLOBAL_MUTE_LED_SPK_GPIO_NUM_3
#define GLOBAL_MUTE_LED_SPK_GPIO_NUM 0x3
#endif

#endif /* defined _COMMON_H_ */
