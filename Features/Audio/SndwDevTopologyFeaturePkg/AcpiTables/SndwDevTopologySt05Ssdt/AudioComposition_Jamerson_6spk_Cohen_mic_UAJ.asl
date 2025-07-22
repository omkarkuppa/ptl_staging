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

//
// _DSD package moved to a separate file.
//

#ifdef COHEN_ONLY
# define SPEAKER_RENDER_DATA_PORT       5
#else   // !COHEN_ONLY
# define SPEAKER_RENDER_DATA_PORT       1
#ifdef JAMERSON_6_LID
# define ACPI_ACD_COLLECTION_COUNT      6
#else
# ifdef JAMERSON_5_LID
#  define ACPI_ACD_COLLECTION_COUNT     5
# else
#  ifdef JAMERSON_4_LID
#   define ACPI_ACD_COLLECTION_COUNT    4
#  else
#   define ACPI_ACD_COLLECTION_COUNT    3
#  endif // JAMERSON_4_LID
# endif // JAMERSON_5_LID
#endif // JAMERSON_6_LID
#endif // COHEN_ONLY


//
// Fall back acpi-acd-device-namestring
//
#ifndef JAMERSON_1_AMP_DEV_NAME
# define JAMERSON_1_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD2.AF01"
#endif

#ifndef JAMERSON_2_AMP_DEV_NAME
# define JAMERSON_2_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD3.AF01"
#endif

#ifndef JAMERSON_3_AMP_DEV_NAME
# define JAMERSON_3_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD4.AF01"
#endif

#ifndef JAMERSON_4_AMP_DEV_NAME
# define JAMERSON_4_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD5.AF01"
#endif

#ifndef COHEN_1_AMP_DEV_NAME
# define COHEN_1_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF01"
#endif

#ifndef COHEN_1_MIC_DEV_NAME
# define COHEN_1_MIC_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF02"
#endif

#ifndef COHEN_1_UAJ_DEV_NAME
# define COHEN_1_UAJ_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF03"
#endif

#ifndef JAMERSON_5_AMP_DEV_NAME
# define JAMERSON_5_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD7.AF01"
#endif

#ifndef JAMERSON_6_AMP_DEV_NAME
# define JAMERSON_6_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD8.AF01"
#endif

#ifndef DSP_ACPI_ACD_DEVICE_NAMESTRING
# define DSP_ACPI_ACD_DEVICE_NAMESTRING "\\_SB.PC00.HDAS"
#endif

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
#ifdef COHEN_ONLY
       Package (2) {"acpi-acd-collection-1-properties", "CC01"},    // Stereo Speakers
#else
       Package (2) {"acpi-acd-collection-1-properties", "AG00"},    // Aggregated Speakers
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

Name(AC00, Package() {    // This package is shared by all DSP devices in this composition table
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"msft-acx-factory-circuit", One},    // Is an ACX Factory
       Package (2) {"msft-acx-core-circuit", Zero},    // Not an ACX Core circuit
    }
}) //End AC00

Name(VN00, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-vendor-id", 0x1},
       Package (2) {"acpi-vendor-config-type", "Streaming_Speaker"},

#ifdef _AMD
       Package (2) {"amd-sdw-default-stream-dp-number", SPEAKER_RENDER_DATA_PORT}, // Raw Stream Data port number on SPK device
#else   // !_AMD

# ifdef DEFAULT_STREAMING_SPK_VOL
       Package (2) {"acpi-vendor-endpoint-default-volume", DEFAULT_STREAMING_SPK_VOL},
# endif

       Package (2) {"acpi-vendor-sdca-terminal-type", 0x0380},
       Package (2) {"acpi-vendor-amp-degraded-mode", 0},
       Package (2) {"acpi-acd-connection-count", 2},

       // Reference stream setup
       Package (2) {"acpi_vendor_smart_amp", INTEL_DSP_AEC_ENABLE}, // enable is 1; disable is 0
       Package (2) {"acpi-vendor-aggregation-peripheral-count", INTEL_DSP_NUM_AMPS}, // no of amps
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-count", INTEL_DSP_NUM_AMPS},  // amps participating in aggregation of reference.

       // Speaker Render stream (raw)
       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", SPEAKER_RENDER_DATA_PORT},

       // Speaker Render stream (offload)
       Package (2) {"acpi-vendor-connection-1-dsp-pin", 0x02},
       Package (2) {"acpi-vendor-connection-1-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-1-peripheral-dp-number", SPEAKER_RENDER_DATA_PORT},

       // Information on peripheral participating in feedback streams aggregation
# ifdef JAMERSON_1_LID
       // Old streaming driver
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-0-controller-id", JAMERSON_1_LID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-0-unique-id",     JAMERSON_1_UID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-0-channel-mask",  JAMERSON_1_LR_CHANNEL_MASK},
       // New streaming driver
       Package (2) {"acpi-vendor-aggregation-peripheral-0-function-manufacturer-id", 0x1fa},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-function-id", 0x3556},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-controller-id", 0},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-link-id", JAMERSON_1_LID},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-unique-id", JAMERSON_1_UID},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-function-number", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-function-type", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-dp-number", SPEAKER_RENDER_DATA_PORT},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-aec-feedback-channel-mask",  JAMERSON_1_LR_CHANNEL_MASK},
# endif
# ifdef JAMERSON_2_LID
       // Old streaming driver
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-1-controller-id", JAMERSON_2_LID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-1-unique-id",     JAMERSON_2_UID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-1-channel-mask",  JAMERSON_2_LR_CHANNEL_MASK},
       // New streaming driver
       Package (2) {"acpi-vendor-aggregation-peripheral-1-function-manufacturer-id", 0x1fa},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-function-id", 0x3556},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-controller-id", 0},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-link-id", JAMERSON_2_LID},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-unique-id", JAMERSON_2_UID},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-function-number", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-function-type", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-dp-number", SPEAKER_RENDER_DATA_PORT},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-aec-feedback-channel-mask",  JAMERSON_2_LR_CHANNEL_MASK},
# endif
# ifdef JAMERSON_3_LID
       // Old streaming driver
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-2-controller-id", JAMERSON_3_LID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-2-unique-id",     JAMERSON_3_UID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-2-channel-mask",  JAMERSON_3_LR_CHANNEL_MASK},
       // New streaming driver
       Package (2) {"acpi-vendor-aggregation-peripheral-2-function-manufacturer-id", 0x1fa},
       Package (2) {"acpi-vendor-aggregation-peripheral-2-function-id", 0x3556},
       Package (2) {"acpi-vendor-aggregation-peripheral-2-controller-id", 0},
       Package (2) {"acpi-vendor-aggregation-peripheral-2-link-id", JAMERSON_3_LID},
       Package (2) {"acpi-vendor-aggregation-peripheral-2-unique-id", JAMERSON_3_UID},
       Package (2) {"acpi-vendor-aggregation-peripheral-2-function-number", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-2-function-type", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-2-dp-number", SPEAKER_RENDER_DATA_PORT},
       Package (2) {"acpi-vendor-aggregation-peripheral-2-aec-feedback-channel-mask",  JAMERSON_3_LR_CHANNEL_MASK},
# endif
# ifdef JAMERSON_4_LID
       // Old streaming driver
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-3-controller-id", JAMERSON_4_LID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-3-unique-id",     JAMERSON_4_UID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-3-channel-mask",  JAMERSON_4_LR_CHANNEL_MASK},
       // New streaming driver
       Package (2) {"acpi-vendor-aggregation-peripheral-3-function-manufacturer-id", 0x1fa},
       Package (2) {"acpi-vendor-aggregation-peripheral-3-function-id", 0x3556},
       Package (2) {"acpi-vendor-aggregation-peripheral-3-controller-id", 0},
       Package (2) {"acpi-vendor-aggregation-peripheral-3-link-id", JAMERSON_4_LID},
       Package (2) {"acpi-vendor-aggregation-peripheral-3-unique-id", JAMERSON_4_UID},
       Package (2) {"acpi-vendor-aggregation-peripheral-3-function-number", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-3-function-type", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-3-dp-number", SPEAKER_RENDER_DATA_PORT},
       Package (2) {"acpi-vendor-aggregation-peripheral-3-aec-feedback-channel-mask",  JAMERSON_4_LR_CHANNEL_MASK},
# endif
# ifdef JAMERSON_5_LID
       // Old streaming driver
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-4-controller-id", JAMERSON_5_LID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-4-unique-id",     JAMERSON_5_UID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-4-channel-mask",  JAMERSON_5_LR_CHANNEL_MASK},
       // New streaming driver
       Package (2) {"acpi-vendor-aggregation-peripheral-4-function-manufacturer-id", 0x1fa},
       Package (2) {"acpi-vendor-aggregation-peripheral-4-function-id", 0x3556},
       Package (2) {"acpi-vendor-aggregation-peripheral-4-controller-id", 0},
       Package (2) {"acpi-vendor-aggregation-peripheral-4-link-id", JAMERSON_5_LID},
       Package (2) {"acpi-vendor-aggregation-peripheral-4-unique-id", JAMERSON_5_UID},
       Package (2) {"acpi-vendor-aggregation-peripheral-4-function-number", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-4-function-type", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-4-dp-number", SPEAKER_RENDER_DATA_PORT},
       Package (2) {"acpi-vendor-aggregation-peripheral-4-aec-feedback-channel-mask",  JAMERSON_5_LR_CHANNEL_MASK},
# endif
# ifdef JAMERSON_6_LID
       // Old streaming driver
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-5-controller-id", JAMERSON_6_LID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-5-unique-id",     JAMERSON_6_UID},
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-5-channel-mask",  JAMERSON_6_LR_CHANNEL_MASK},
       // New streaming driver
       Package (2) {"acpi-vendor-aggregation-peripheral-5-function-manufacturer-id", 0x1fa},
       Package (2) {"acpi-vendor-aggregation-peripheral-5-function-id", 0x3556},
       Package (2) {"acpi-vendor-aggregation-peripheral-5-controller-id", 0},
       Package (2) {"acpi-vendor-aggregation-peripheral-5-link-id", JAMERSON_6_LID},
       Package (2) {"acpi-vendor-aggregation-peripheral-5-unique-id", JAMERSON_6_UID},
       Package (2) {"acpi-vendor-aggregation-peripheral-5-function-number", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-5-function-type", 1},
       Package (2) {"acpi-vendor-aggregation-peripheral-5-dp-number", SPEAKER_RENDER_DATA_PORT},
       Package (2) {"acpi-vendor-aggregation-peripheral-5-aec-feedback-channel-mask",  JAMERSON_6_LR_CHANNEL_MASK},
# endif
#endif  // _AMD
    }
})  //End VN00

Name(AC01, Package() {    // This package is shared by all Codec devices in this composition table
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"msft-acx-factory-circuit", Zero},    // Not an ACX Factory
       Package (2) {"msft-acx-core-circuit", One},    // ACX Core circuit
       Package (2) {"msft-acx-aggregated-circuit", One},
    }
}) //End AC01

Name(AC02, Package() {    // This package is shared by all Codec devices in this composition table
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"msft-acx-factory-circuit", Zero},    // Not an ACX Factory
       Package (2) {"msft-acx-core-circuit", One},    // ACX Core circuit
    }
}) //End AC02

#ifdef COHEN_ONLY

Name(CC01, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", COHEN_1_AMP_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0xC},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC01

#else   // !COHEN_ONLY

Name(AG00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "Speaker_Aggregation"},
       Package (2) {"acpi-acd-collection-type", 0},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 1},    // Parallel Connection
       Package (2) {"acpi-acd-collection-count", ACPI_ACD_COLLECTION_COUNT},    // N SDCA Audio Functions
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC01"},    // First SDCA AMP Function
       Package (2) {"acpi-acd-collection-1-properties", "CC02"},    // Second SDCA AMP Function
       Package (2) {"acpi-acd-collection-2-properties", "CC03"},    // Third SDCA AMP Function
#if (ACPI_ACD_COLLECTION_COUNT > 3)
       Package (2) {"acpi-acd-collection-3-properties", "CC04"},    // Fourth SDCA AMP Function
# if (ACPI_ACD_COLLECTION_COUNT > 4)
       Package (2) {"acpi-acd-collection-4-properties", "CC05"},    // Fifth SDCA AMP Function
#  if (ACPI_ACD_COLLECTION_COUNT > 5)
       Package (2) {"acpi-acd-collection-5-properties", "CC06"},    // Sixth SDCA AMP Function
#  endif // ACPI_ACD_COLLECTION_COUNT > 5
# endif // ACPI_ACD_COLLECTION_COUNT > 4
#endif // ACPI_ACD_COLLECTION_COUNT > 3
    }
}) //End AG00

Name(CC01, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", JAMERSON_1_AMP_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0xE},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC01

Name(CC02, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", JAMERSON_2_AMP_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0xE},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC02
Name(CC03, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", JAMERSON_3_AMP_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0xE},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC03

Name(CC04, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", JAMERSON_4_AMP_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0xE},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC04

Name(CC05, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", JAMERSON_5_AMP_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0xE},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC05

Name(CC06, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", JAMERSON_6_AMP_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0xE},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC06
#endif  // COHEN_ONLY

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
#ifdef _AMD
       Package (2) {"amd-sdw-default-stream-dp-number", 0x1},    // Raw Stream Data port number on Mic device
#else
       Package (2) {"acpi-acd-connection-count", 1},

       // Microphone Array Capture stream (raw)
       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x01},
#endif
    }
}) //End VN01

Name(CC11, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", COHEN_1_MIC_DEV_NAME},
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
       Package (2) {"acpi-vendor-config-type", "Streaming_Headphones"},
#ifdef _AMD
       Package (2) {"amd-sdw-default-stream-dp-number", 0x6},    // Raw Stream Data port number on SPK device
#else
       Package (2) {"acpi-vendor-sdca-terminal-type", 0x06c0},
       Package (2) {"acpi-acd-connection-count", 1},
       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x6},
#endif
    }
}) //End VN02

//
// CC21 package moved to a separate file.
//

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
#ifdef _AMD
       Package (2) {"amd-sdw-default-stream-dp-number", 0x6},    // Raw Stream Data port number on SPK device
#else
       Package (2) {"acpi-vendor-sdca-terminal-type", 0x0690},
       Package (2) {"acpi-acd-connection-count", 1},
       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x6 },
#endif
    }
}) //End VN03

//
// CC31 package moved to a separate file.
//

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
#ifdef _AMD
       Package (2) {"amd-sdw-default-stream-dp-number", 0x6},    // Raw Stream Data port number on SPK device
#else
       Package (2) {"acpi-vendor-sdca-terminal-type", 0x06D0},
       Package (2) {"acpi-acd-connection-count", 1},
       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x6},
#endif
    }
}) //End VN04

//
// CC41 package moved to a separate file.
//

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
#ifdef _AMD
       Package (2) {"amd-sdw-default-stream-dp-number", 0x2},    // Raw Stream Data port number on SPK device
#else
       Package (2) {"acpi-vendor-sdca-terminal-type", 0x06a0},
       Package (2) {"acpi-acd-connection-count", 1},
       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x2},
#endif
    }
}) //End VN05

//
// CC51 package moved to a separate file.
//

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
#ifdef _AMD
       Package (2) {"amd-sdw-default-stream-dp-number", 0x2},    // Raw Stream Data port number on SPK device
#else
       Package (2) {"acpi-vendor-sdca-terminal-type", 0x0680},
       Package (2) {"acpi-acd-connection-count", 1},
       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x2},
#endif
    }
}) //End VN06

//
// CC61 package moved to a separate file.
//

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
#ifdef _AMD
       Package (2) {"amd-sdw-default-stream-dp-number", 0x2},    // Raw Stream Data port number on SPK device
#else
       Package (2) {"acpi-vendor-sdca-terminal-type", 0x06d0},
       Package (2) {"acpi-acd-connection-count", 1},
       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x00},
       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x2},
#endif
    }
}) //End VN07

//
// CC71 package moved to a separate file.
//
