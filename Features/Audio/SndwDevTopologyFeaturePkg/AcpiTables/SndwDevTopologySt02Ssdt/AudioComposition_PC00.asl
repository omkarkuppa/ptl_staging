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

Name(_DSD, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-interface-revision", 0x00000001},    // 0.1 Spec Version Bits 31-16: (Upper word) Major version number. Bits 15-0: (Lower word) Minor version number
       Package (2) {"acpi-acd-endpoint-count", 8},    // 8 Endpoints: MicrophoneArray, Headphones, LineOut, HeadsetOutput, Microphone, LineIn, HeadsetMic, Speaker (EP8) or Aggregated Speaker (EP0)
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-0-properties", "EP01"},     // Configuration for MicrophoneArray
       Package (2) {"acpi-acd-endpoint-1-properties", "EP02"},     // Configuration for Headphones
       Package (2) {"acpi-acd-endpoint-2-properties", "EP03"},     // Configuration for LineOut
       Package (2) {"acpi-acd-endpoint-3-properties", "EP04"},     // Configuration for HeadsetOutput
       Package (2) {"acpi-acd-endpoint-4-properties", "EP05"},     // Configuration for Microphone
       Package (2) {"acpi-acd-endpoint-5-properties", "EP06"},     // Configuration for LineIn
       Package (2) {"acpi-acd-endpoint-6-properties", "EP07"},     // Configuration for HeadsetMic
       Package (2) {"acpi-acd-endpoint-7-properties", "EP00"},     // Configuration for Aggregated Speaker
       //Package (2) {"acpi-acd-endpoint-7-properties", "EP08"},     // Configuration for 1320 Speaker
       //Package (2) {"acpi-acd-endpoint-7-properties", "EP09"},     // Configuration for 712 Speaker
    }
}) //End _DSD

Name(AC00, Package() {    // This package is shared by all DSP devices in this composition table
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"msft-acx-factory-circuit", One},    // Is an ACX Factory
       Package (2) {"msft-acx-core-circuit", Zero},    // Not an ACX Core circuit
    }
}) //End AC00

Name(AC01, Package() {    // This package is shared by all aggregated Codec devices in this composition table
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

Name(EP00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "Speaker"},
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
       Package (2) {"acpi-acd-collection-1-properties", "AG00"},    // Aggregated Speakers
    }
}) //End EC00

Name(CC00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.ISSW"},
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
       Package (2) {"acpi-vendor-sdca-terminal-type", 0x0380},
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},
       // Intel streaming properties
       Package (2) {"acpi-acd-connection-count", 2},
       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x3},
       Package (2) {"acpi-vendor-connection-1-dsp-pin", 0x2},
       Package (2) {"acpi-vendor-connection-1-stream-type", 0x0},
       Package (2) {"acpi-vendor-connection-1-peripheral-dp-number", 0x1},
       // Playback aggregation
       Package (2) {"acpi-vendor-aggregation-peripheral-count", 0x02},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-function-manufacturer-id", 0x025d},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-function-id", 0x0712},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-controller-id", 0x00},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-link-id", 0x03},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-unique-id", 0x00},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-function-number", 0x04},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-function-type", 0x01},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-dp-number", 0x03},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-function-manufacturer-id", 0x025d},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-function-id", 0x1320},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-controller-id", 0x00},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-link-id", 0x02},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-unique-id", 0x00},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-function-number", 0x04},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-function-type", 0x01},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-dp-number", 0x01},
       // Reference aggregation
       Package (2) {"acpi-vendor-amp-degraded-mode", 0x00}, // 0: Speaker protection on codec, 1: Speaker protection on DSP
       //Package (2) {"acpi-vendor-amp-degraded-mode", 0x01}, // 0: Speaker protection on codec, 1: Speaker protection on DSP
       //Package (2) {"acpi_vendor_smart_amp", 0x01}, // 0: Simple amp, 1: Smart amp
       Package (2) {"acpi_vendor_smart_amp", 0x00}, // 0: Simple amp, 1: Smart amp   //20250210 update for Intel streaming driver
       Package (2) {"acpi-vendor-feedback-aggregation-peripheral-count", 0x02},
       Package (2) {"acpi-vendor-aggregation-peripheral-0-aec-feedback-channel-mask", 0x03},
       Package (2) {"acpi-vendor-aggregation-peripheral-1-aec-feedback-channel-mask", 0x0C},
       // Realtek streaming properties
       // Package (2) {"acpi-acd-connection-count", 2}, // do not need this here, already defined in Intel Streaming properties
       Package (2) {"acpi-acd-connection-0-properties",
          Buffer()
          {
             0x01,
             0x00, 0x03, 0x00, 0x00,
             0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
             0x03 // data port
          }
       },
       Package (2) {"acpi-acd-connection-1-properties",
          Buffer()
          {
             0x01,
             0x00, 0x02, 0x00, 0x00,
             0x01, 0x20, 0x13, 0x5D, 0x02, 0x30, 0x02, 0x00,
             0x01 // data port
          }
       },
       Package (2) {"custom-acd-feedback-count", 2},
       Package (2) {"custom-acd-feedback-0-properties",
          Buffer()
          {
             0x02,
             0x00, 0x03, 0x00, 0x00,
             0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
             0x0C, 0x03 // data port, channel mask
          }
       },
       Package (2) {"custom-acd-feedback-1-properties",
          Buffer()
          {
             0x02,
             0x00, 0x02, 0x00, 0x00,
             0x01, 0x20, 0x13, 0x5D, 0x02, 0x30, 0x02, 0x00,
             0x04, 0x0C // data port, channel mask
          }
       },
    }
}) //End VN00

Name(AG00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "Speaker_Aggregation"},
       Package (2) {"acpi-acd-collection-type", 0},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 1},    // Parallel Connection
       Package (2) {"acpi-acd-collection-count", 2},    // Two SDCA Audio Functions
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC01"},    // First SDCA AMP Function
       Package (2) {"acpi-acd-collection-1-properties", "CC02"},    // Second SDCA AMP Function
    }
}) //End AG00

Name(CC01, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDA.AF04"},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x42},    // Entity id of the Analog terminal used for this endpoint
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
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDB.AF04"},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x13},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC02

Name(EP01, Package() {
   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
   Package () {
      Package (2) {"acpi-acd-endpoint-friendly-name", "MicrophoneArray"},
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
      Package (2) {"acpi-acd-config-friendly-name", "MicrophoneArray_With_DSP"},
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
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.ISSW"},
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
      Package (2) {"acpi-vendor-sdca-terminal-type", 0x0205},
      Package (2) {"acpi-acd-sdca-terminal-type", 0x0205},
      Package (2) {"acpi-acd-connection-count", 1},
      Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
      Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
      Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x8},
      Package (2) {"acpi-acd-connection-0-properties",
         Buffer()
         {
            0x02,
            0x00, 0x03, 0x00, 0x00,
            0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
            0x08
         }
      },
      Package (2) {"custom-acd-buffered-count", 1},
      Package (2) {"custom-acd-buffered-0-properties",
         Buffer()
         {
            0x02,
            0x00, 0x03, 0x00, 0x00,
            0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
            0x0A,                   // dataport
            0xFE, 0xFF,             // wFormatTag
            0x02, 0x00,             // nChannels
            0x80, 0xBB, 0x00, 0x00, // nSamplesPerSec
            0x00, 0xEE, 0x02, 0x00, // nAvgBytesPerSec
            0x04, 0x00,             // nBlockAlign
            0x10, 0x00,             // wBitsPerSample
            0x16, 0x00,             // cbSize
            0x10, 0x00,             // wValidBitsPerSample
            0x03, 0x00, 0x00, 0x00, // dwChannelMask
            0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71  // SubFormat
         }
      },
   }
}) //End VN01

Name(CC11, Package() {
   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
   Package () {
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDA.AF02"},
      Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
      Package (2) {"acpi-acd-sdca-terminal-id", 0x26},    // Entity id of the Analog terminal used for this endpoint
      Package (2) {"acpi-acd-sdca-terminal-type", 0x0205},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
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
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.ISSW"},
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
      Package (2) {"acpi-vendor-sdca-terminal-type", 0x06C0},
      Package (2) {"acpi-acd-sdca-terminal-type", 0x06C0},
      Package (2) {"acpi-acd-connection-count", 1},
      Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
      Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
      Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x1},
      Package (2) {"acpi-acd-connection-0-properties",
                        Buffer()
                        {
                           0x01,
                           0x00, 0x03, 0x00, 0x00,
                           0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
                           0x01
                        }
                  },
   }
}) //End VN02

Name(CC21, Package() {
   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
   Package () {
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDA.AF01"},
      Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
      Package (2) {"acpi-acd-sdca-terminal-id", 0x6},    // Entity id of the Analog terminal used for this endpoint
      Package (2) {"acpi-acd-sdca-terminal-type", 0x06c0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
   },
   ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
   Package () {
      Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
   }
}) //End CC21

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
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.ISSW"},
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
      Package (2) {"acpi-vendor-sdca-terminal-type", 0x0690},
      Package (2) {"acpi-acd-sdca-terminal-type", 0x0690},
      Package (2) {"acpi-acd-connection-count", 1},
      Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
      Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
      Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x1},
      Package (2) {"acpi-acd-connection-0-properties",
                        Buffer()
                        {
                           0x01,
                           0x00, 0x03, 0x00, 0x00,
                           0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
                           0x01
                        }
                  },
   }
}) //End VN03

Name(CC31, Package() {
   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
   Package () {
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDA.AF01"},
      Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
      Package (2) {"acpi-acd-sdca-terminal-id", 0x7},    // Entity id of the Analog terminal used for this endpoint
      Package (2) {"acpi-acd-sdca-terminal-type", 0x0690},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
   },
   ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
   Package () {
      Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
   }
}) //End CC31

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
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.ISSW"},
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
      Package (2) {"acpi-vendor-sdca-terminal-type", 0x06D0},
      Package (2) {"acpi-acd-sdca-terminal-type", 0x06D0},
      Package (2) {"acpi-acd-connection-count", 1},
      Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
      Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
      Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x1},
      Package (2) {"acpi-acd-connection-0-properties",
                        Buffer()
                        {
                           0x01,
                           0x00, 0x03, 0x00, 0x00,
                           0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
                           0x01
                        }
                  },
   }
}) //End VN04

Name(CC41, Package() {
   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
   Package () {
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDA.AF01"},
      Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
      Package (2) {"acpi-acd-sdca-terminal-id", 0x48},    // Entity id of the Analog terminal used for this endpoint
      Package (2) {"acpi-acd-sdca-terminal-type", 0x06d0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
   },
   ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
   Package () {
      Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
   }
}) //End CC41

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
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.ISSW"},
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
      Package (2) {"acpi-vendor-sdca-terminal-type", 0x06A0},
      Package (2) {"acpi-acd-sdca-terminal-type", 0x06A0},
      Package (2) {"acpi-acd-connection-count", 1},
      Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
      Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
      Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x4},
      Package (2) {"acpi-acd-connection-0-properties",
                        Buffer()
                        {
                           0x02,
                           0x00, 0x03, 0x00, 0x00,
                           0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
                           0x04
                        }
                  },
   }
}) //End VN05

Name(CC51, Package() {
   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
   Package () {
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDA.AF01"},
      Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
      Package (2) {"acpi-acd-sdca-terminal-id", 0x8},    // Entity id of the Analog terminal used for this endpoint
      Package (2) {"acpi-acd-sdca-terminal-type", 0x06a0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
   },
   ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
   Package () {
      Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
   }
}) //End CC51

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
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.ISSW"},
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
      Package (2) {"acpi-vendor-sdca-terminal-type", 0x0680},
      Package (2) {"acpi-acd-sdca-terminal-type", 0x0680},
      Package (2) {"acpi-acd-connection-count", 1},
      Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
      Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
      Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x4},
      Package (2) {"acpi-acd-connection-0-properties",
                        Buffer()
                        {
                           0x02,
                           0x00, 0x03, 0x00, 0x00,
                           0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
                           0x04
                        }
                  },
   }
}) //End VN06

Name(CC61, Package() {
   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
   Package () {
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDA.AF01"},
      Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
      Package (2) {"acpi-acd-sdca-terminal-id", 0x9},    // Entity id of the Analog terminal used for this endpoint
      Package (2) {"acpi-acd-sdca-terminal-type", 0x0680},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
   },
   ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
   Package () {
      Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
   }
}) //End CC61

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
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.ISSW"},
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
      Package (2) {"acpi-vendor-sdca-terminal-type", 0x06D0},
      Package (2) {"acpi-acd-sdca-terminal-type", 0x06D0},
      Package (2) {"acpi-acd-connection-count", 1},
      Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
      Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
      Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x4},
      Package (2) {"acpi-acd-connection-0-properties",
                        Buffer()
                        {
                           0x02,
                           0x00, 0x03, 0x00, 0x00,
                           0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
                           0x04
                        }
                  },
   }
}) //End VN07

Name(CC71, Package() {
   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
   Package () {
      Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDA.AF01"},
      Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
      Package (2) {"acpi-acd-sdca-terminal-id", 0x43},    // Entity id of the Analog terminal used for this endpoint
      Package (2) {"acpi-acd-sdca-terminal-type", 0x06d0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
   },
   ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
   Package () {
      Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
   }
}) //End CC71

// Name(EP08, Package() {
//    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
//    Package () {
//       Package (2) {"acpi-acd-endpoint-friendly-name", "Speaker"},
//       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
//       Package (2) {"acpi-acd-endpoint-id", 1},    // Endpoint id
//    },
//    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
//    Package () {
//       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC80"},
//    }
// }) //End EP08

// Name(EC80, Package() {
//    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
//    Package () {
//       Package (2) {"acpi-acd-config-priority", 1},
//       Package (2) {"acpi-acd-config-friendly-name", "Speaker_With_DSP"},
//       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
//       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
//       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
//    },
//    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
//    Package () {
//       Package (2) {"acpi-acd-collection-0-properties", "CC80"},    // DSP Configuration
//       Package (2) {"acpi-acd-collection-1-properties", "CC81"},    // Sdca Audio Function Configuration
//    }
// }) //End EC80

// Name(CC80, Package() {
//    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
//    Package () {
//       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.ISSW"},
//       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
//    },
//    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
//    Package () {
//       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
//       Package (2) {"acpi-acd-vendor-collection-properties", "VN08"},
//    }
// }) //End CC80

// Name(VN08, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
//    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
//    Package () {
//       Package (2) {"acpi-vendor-id", 0x1},
//       Package (2) {"acpi-vendor-config-type", "Streaming_Speaker"},
//       Package (2) {"acpi-vendor-sdca-terminal-type", 0x0380},
//       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},
//       Package (2) {"acpi-acd-connection-count", 1},
//       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
//       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
//       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x1},
//       Package (2) {"acpi-acd-connection-0-properties",
//          Buffer()
//          {
//             0x01,
//             0x00, 0x02, 0x00, 0x00,
//             0x01, 0x20, 0x13, 0x5D, 0x02, 0x30, 0x02, 0x00,
//             0x01
//          }
//       },
//    }
// }) //End VN08

// Name(CC81, Package() {
//    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
//    Package () {
//       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDB.AF04"},
//       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
//       Package (2) {"acpi-acd-sdca-terminal-id", 0x13},    // Entity id of the Analog terminal used for this endpoint
//       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
//    },
//    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
//    Package () {
//       Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
//    }
// }) //End CC81

// Name(EP09, Package() {
//    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
//    Package () {
//       Package (2) {"acpi-acd-endpoint-friendly-name", "Speaker"},
//       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
//       Package (2) {"acpi-acd-endpoint-id", 1},    // Endpoint id
//    },
//    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
//    Package () {
//       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC90"},
//    }
// }) //End EP09

// Name(EC90, Package() {
//    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
//    Package () {
//       Package (2) {"acpi-acd-config-priority", 1},
//       Package (2) {"acpi-acd-config-friendly-name", "Speaker_With_DSP"},
//       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
//       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
//       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
//    },
//    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
//    Package () {
//       Package (2) {"acpi-acd-collection-0-properties", "CC90"},    // DSP Configuration
//       Package (2) {"acpi-acd-collection-1-properties", "CC91"},    // Sdca Audio Function Configuration
//    }
// }) //End EC90

// Name(CC90, Package() {
//    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
//    Package () {
//       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.ISSW"},
//       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
//    },
//    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
//    Package () {
//       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
//       Package (2) {"acpi-acd-vendor-collection-properties", "VN09"},
//    }
// }) //End CC90

// Name(VN09, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
//    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
//    Package () {
//       Package (2) {"acpi-vendor-id", 0x1},
//       Package (2) {"acpi-vendor-config-type", "Streaming_Speaker"},
//       Package (2) {"acpi-vendor-sdca-terminal-type", 0x0380},
//       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},
//       Package (2) {"acpi-acd-connection-count", 1},
//       Package (2) {"acpi-vendor-connection-0-dsp-pin", 0x0},
//       Package (2) {"acpi-vendor-connection-0-stream-type", 0x0},
//       Package (2) {"acpi-vendor-connection-0-peripheral-dp-number", 0x3},
//       Package (2) {"acpi-acd-connection-0-properties",
//          Buffer()
//          {
//             0x01,
//             0x00, 0x03, 0x00, 0x00,
//             0x01, 0x12, 0x07, 0x5D, 0x02, 0x30, 0x03, 0x00,
//             0x03
//          }
//       },
//    }
// }) //End VN09

// Name(CC91, Package() {
//    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
//    Package () {
//       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS.IDA.SNDW.SWDA.AF04"},
//       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
//       Package (2) {"acpi-acd-sdca-terminal-id", 0x42},    // Entity id of the Analog terminal used for this endpoint
//       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
//    },
//    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
//    Package () {
//       Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
//    }
// }) //End CC91

