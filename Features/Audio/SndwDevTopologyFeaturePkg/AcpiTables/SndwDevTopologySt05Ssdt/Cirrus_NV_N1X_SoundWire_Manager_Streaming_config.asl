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

#ifdef _NVIDIA
Device(ASD0)
{
    Name (_HID, "NVDA9022")
    Name (_UID, 0x0)
    Name (_CCA, 1)

    Method (_STA) {
      Return(0xf)
    }

    Name (_CRS, ResourceTemplate () {
        MEMORY32FIXED (ReadWrite, 0x1A690000, 0x10000, )
        Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 724 }   // INT ID=692
#ifdef ENABLE_HOST_DMIC_GPIOS
        PinFunction (Exclusive, PullUp, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x6}) { 62 } // DMIC0_DAT
        PinFunction (Exclusive, PullUp, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x6}) { 63 } // DMIC0_CLK
        PinFunction (Exclusive, PullUp, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x6}) { 64 } // DMIC1_DAT
        PinFunction (Exclusive, PullUp, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x6}) { 65 } // DMIC1_CLK
#endif
#ifdef ENABLE_HOST_I2S_GPIOS
        PinFunction (Exclusive, PullUp, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x6}) { 130, 131, 132, 133 } // I2S0
#endif
    })

    Name (_DEP, Package()  // _DEP: Dependencies
    {
        \_SB.DSP1,    // HIFI5_A
        \_SB.DSP3,    // HIFI1
    })

    Name (_DSD, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-hw-ver", 1 },
#ifndef EXCLUDE_BLE_CONFIGS
            Package (2) { "acpi-asd-endpoint-list", 0x0000003F },
#else
            Package (2) { "acpi-asd-endpoint-list", 0x0000000F },
#endif
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package() {
            Package(2) {"acpi-asd-speaker-subproperties", "SPKP" },
            Package(2) {"acpi-asd-microphone-subproperties", "MICP" },
            Package(2) {"acpi-asd-headphone-subproperties", "HPHP" },
            Package(2) {"acpi-asd-headset-mic-subproperties", "HSMP" },
#ifndef EXCLUDE_BLE_CONFIGS
            Package(2) {"acpi-asd-ble-headphone-subproperties", "BLE0" },
            Package(2) {"acpi-asd-ble-headset-mic-subproperties", "BLE1" },
#endif
        }
    })

    Name (SPKP, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-endpoint-host-pin-count", 0x1 },
            Package (2) { "acpi-asd-endpoint-bridge-pin-count", 0x1 },
            Package (2) { "acpi-asd-endpoint-interface", 0x4 },
#ifndef DISABLE_SOC_DSP_PATH
            Package (2) { "acpi-asd-endpoint-path-config", 0x2 },
            Package (2) { "acpi-asd-endpoint-offload-supported", 0x1 },
            Package (2) { "acpi-asd-endpoint-adsp-power-policy", 0x1 },
            Package (2) { "acpi-asd-endpoint-echo-destination", 0x1 },
#else
            Package (2) { "acpi-asd-endpoint-path-config", 0x0 },
            Package (2) { "acpi-asd-endpoint-offload-supported", 0x0 },
#endif
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package() {
            Package (2) { "acpi-asd-host-pin-0-subproperties", "HP00" },
            Package (2) { "acpi-asd-bridge-pin-0-subproperties", "BP00" },
        }
    })

    Name (MICP, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-endpoint-host-pin-count", 0x1 },
            Package (2) { "acpi-asd-endpoint-bridge-pin-count", 0x1 },
            Package (2) { "acpi-asd-endpoint-interface", 0x6 },
#ifndef DISABLE_SOC_DSP_PATH
            Package (2) { "acpi-asd-endpoint-path-config", 0x3 },
            Package (2) { "acpi-asd-endpoint-offload-supported", 0x1 },
            Package (2) { "acpi-asd-endpoint-adsp-power-policy", 0x1 },
            Package (2) { "acpi-asd-endpoint-echo-destination", 0x3 },
#else
            Package (2) { "acpi-asd-endpoint-path-config", 0x0 },
            Package (2) { "acpi-asd-endpoint-offload-supported", 0x0 },
#endif
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package() {
            Package (2) { "acpi-asd-host-pin-0-subproperties", "HP01" },
            Package (2) { "acpi-asd-bridge-pin-0-subproperties", "BP00" },
        }
    })

    Name (HPHP, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-endpoint-host-pin-count", 0x1 },
            Package (2) { "acpi-asd-endpoint-bridge-pin-count", 0x1 },
            Package (2) { "acpi-asd-endpoint-interface", 0x4 },
#ifndef DISABLE_SOC_DSP_PATH
            Package (2) { "acpi-asd-endpoint-path-config", 0x2 },
            Package (2) { "acpi-asd-endpoint-offload-supported", 0x1 },
            Package (2) { "acpi-asd-endpoint-adsp-power-policy", 0x1 },
            Package (2) { "acpi-asd-endpoint-echo-destination", 0x0 },
#else
            Package (2) { "acpi-asd-endpoint-path-config", 0x0 },
            Package (2) { "acpi-asd-endpoint-offload-supported", 0x0 },
#endif
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package() {
            Package (2) { "acpi-asd-host-pin-0-subproperties", "HP02" },
            Package (2) { "acpi-asd-bridge-pin-0-subproperties", "BP00" },
        }
    })

    Name (HSMP, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-endpoint-host-pin-count", 0x1 },
            Package (2) { "acpi-asd-endpoint-bridge-pin-count", 0x1 },
            Package (2) { "acpi-asd-endpoint-interface", 0x6 },
            Package (2) { "acpi-asd-endpoint-path-config", 0x0 },
            Package (2) { "acpi-asd-endpoint-offload-supported", 0x0 },
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package() {
            Package (2) { "acpi-asd-host-pin-0-subproperties", "HP03" },
            Package (2) { "acpi-asd-bridge-pin-0-subproperties", "BP00" },
        }
    })

    Name (HP00, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-process-mode-list", 0x7 },
            Package (2) { "acpi-asd-raw-data-format-count", 0x1 },
            Package (2) { "acpi-asd-default-data-format-count", 0x1 },
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package() {
            Package (2) { "acpi-asd-raw-data-format-0-subproperties", "DF00" },
            Package (2) { "acpi-asd-default-data-format-0-subproperties", "DF00" },
        }
    })

    Name (HP01, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-process-mode-list", 0x7 },
            Package (2) { "acpi-asd-raw-data-format-count", 0x1 },
            Package (2) { "acpi-asd-default-data-format-count", 0x1 },
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package() {
#ifdef DMIC_X4
            Package (2) { "acpi-asd-raw-data-format-0-subproperties", "DF03" },
            Package (2) { "acpi-asd-default-data-format-0-subproperties", "DF03" },
#else // !DMIC_x4
# ifdef DMIC_X3
            Package (2) { "acpi-asd-raw-data-format-0-subproperties", "DF04" },
            Package (2) { "acpi-asd-default-data-format-0-subproperties", "DF04" },
# else // DMIC_X2
            Package (2) { "acpi-asd-raw-data-format-0-subproperties", "DF00" },
            Package (2) { "acpi-asd-default-data-format-0-subproperties", "DF00" },
# endif // DMIC_X3
#endif // DMIC_X4
        }
    })

    Name (HP02, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-process-mode-list", 0x7 },
#ifdef UAJ_RENDER_192KHZ
            Package (2) { "acpi-asd-raw-data-format-count", 0x3 },
#else
# ifdef UAJ_RENDER_96KHZ
            Package (2) { "acpi-asd-raw-data-format-count", 0x2 },
# else
            Package (2) { "acpi-asd-raw-data-format-count", 0x1 },
# endif // UAJ_RENDER_96KHZ
#endif // UAJ_RENDER_192KHZ
            Package (2) { "acpi-asd-default-data-format-count", 0x1 },
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package() {
            Package (2) { "acpi-asd-raw-data-format-0-subproperties", "DF00" },
            Package (2) { "acpi-asd-default-data-format-0-subproperties", "DF00" },
#ifdef UAJ_RENDER_192KHZ
            Package (2) { "acpi-asd-raw-data-format-1-subproperties", "DF01" },
            Package (2) { "acpi-asd-raw-data-format-2-subproperties", "DF02" },
            Package (2) { "acpi-asd-default-data-format-1-subproperties", "DF01" },
            Package (2) { "acpi-asd-default-data-format-2-subproperties", "DF02" },
#else
# ifdef UAJ_RENDER_96KHZ
            Package (2) { "acpi-asd-raw-data-format-1-subproperties", "DF01" },
            Package (2) { "acpi-asd-default-data-format-1-subproperties", "DF01" },
# endif
#endif // UAJ_RENDER_192KHZ
        }
    })

    Name (HP03, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-process-mode-list", 0x7 },
            Package (2) { "acpi-asd-raw-data-format-count", 0x1 },
            Package (2) { "acpi-asd-default-data-format-count", 0x1 },
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package() {
             Package (2) { "acpi-asd-raw-data-format-0-subproperties", "DF00" },
             Package (2) { "acpi-asd-default-data-format-0-subproperties", "DF00" },
        }
    })

    Name (BP00, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-process-mode-list", 0x7 },
        },
    })

    // 48k,24bit,2ch
    Name (DF00, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-wave-format-tag", 0xFFFE },
            Package (2) { "acpi-asd-wave-channels", 2 },
            Package (2) { "acpi-asd-wave-samples-per-sec", 48000 },
            Package (2) { "acpi-asd-wave-avg-bytes-per-sec", 384000 },
            Package (2) { "acpi-asd-wave-block-align", 8 },
            Package (2) { "acpi-asd-wave-bits-per-sample", 32 },
            Package (2) { "acpi-asd-wave-cbsize", 22 },
            Package (2) { "acpi-asd-wave-valid-bits-per-sample", 24 },
            Package (2) { "acpi-asd-wave-channel-mask", 0x3 },
            Package (2) { "acpi-asd-wave-sub-type", "pcm" },
        },
    })

    // 96k,24bit,2ch
    Name (DF01, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-wave-format-tag", 0xFFFE },
            Package (2) { "acpi-asd-wave-channels", 2 },
            Package (2) { "acpi-asd-wave-samples-per-sec", 96000 },
            Package (2) { "acpi-asd-wave-avg-bytes-per-sec", 768000 },
            Package (2) { "acpi-asd-wave-block-align", 8 },
            Package (2) { "acpi-asd-wave-bits-per-sample", 32 },
            Package (2) { "acpi-asd-wave-cbsize", 22 },
            Package (2) { "acpi-asd-wave-valid-bits-per-sample", 24 },
            Package (2) { "acpi-asd-wave-channel-mask", 0x3 },
            Package (2) { "acpi-asd-wave-sub-type", "pcm" },
        },
    })

    // 192k,24bit,2ch
    Name (DF02, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-wave-format-tag", 0xFFFE },
            Package (2) { "acpi-asd-wave-channels", 2 },
            Package (2) { "acpi-asd-wave-samples-per-sec", 192000 },
            Package (2) { "acpi-asd-wave-avg-bytes-per-sec", 1536000 },
            Package (2) { "acpi-asd-wave-block-align", 8 },
            Package (2) { "acpi-asd-wave-bits-per-sample", 32 },
            Package (2) { "acpi-asd-wave-cbsize", 22 },
            Package (2) { "acpi-asd-wave-valid-bits-per-sample", 24 },
            Package (2) { "acpi-asd-wave-channel-mask", 0x3 },
            Package (2) { "acpi-asd-wave-sub-type", "pcm" },
        },
    })

    // 48k,24bit,4ch
    Name (DF03, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-wave-format-tag", 0xFFFE },
            Package (2) { "acpi-asd-wave-channels", 4 },
            Package (2) { "acpi-asd-wave-samples-per-sec", 48000 },
            Package (2) { "acpi-asd-wave-avg-bytes-per-sec", 768000 },
            Package (2) { "acpi-asd-wave-block-align", 16 },
            Package (2) { "acpi-asd-wave-bits-per-sample", 32 },
            Package (2) { "acpi-asd-wave-cbsize", 22 },
            Package (2) { "acpi-asd-wave-valid-bits-per-sample", 24 },
            Package (2) { "acpi-asd-wave-channel-mask", 0x0 },
            Package (2) { "acpi-asd-wave-sub-type", "pcm" },
        },
    })

    // 48k,24bit,3ch
    Name (DF04, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-wave-format-tag", 0xFFFE },
            Package (2) { "acpi-asd-wave-channels", 3 },
            Package (2) { "acpi-asd-wave-samples-per-sec", 48000 },
            Package (2) { "acpi-asd-wave-avg-bytes-per-sec", 576000 },
            Package (2) { "acpi-asd-wave-block-align", 12 },
            Package (2) { "acpi-asd-wave-bits-per-sample", 32 },
            Package (2) { "acpi-asd-wave-cbsize", 22 },
            Package (2) { "acpi-asd-wave-valid-bits-per-sample", 24 },
            Package (2) { "acpi-asd-wave-channel-mask", 0x0 },
            Package (2) { "acpi-asd-wave-sub-type", "pcm" },
        },
    })

#ifndef EXCLUDE_BLE_CONFIGS
    Name (BLE0, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-endpoint-interface", 0x1 },
#ifndef DISABLE_SOC_DSP_PATH
            Package (2) { "acpi-asd-endpoint-path-config", 0x3 },
            Package (2) { "acpi-asd-endpoint-offload-supported", 0x1 },
            Package (2) { "acpi-asd-endpoint-adsp-power-policy", 0x1 },
#else
            Package (2) { "acpi-asd-endpoint-path-config", 0x0 },
            Package (2) { "acpi-asd-endpoint-offload-supported", 0x0 },
#endif
        }
    })

    Name (BLE1, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "acpi-asd-endpoint-interface", 0x2 },
#ifndef DISABLE_SOC_DSP_PATH
            Package (2) { "acpi-asd-endpoint-path-config", 0x3 },
            Package (2) { "acpi-asd-endpoint-adsp-power-policy", 0x1 },
#else
            Package (2) { "acpi-asd-endpoint-path-config", 0x0 },
#endif
        }
    })
#endif // EXCLUDE_BLE_CONFIGS
}

//-------------------------------------------------------------------
// SoundWire Audio Controller Device #0
//-------------------------------------------------------------------
Device(CNT0)
{
    Name (_HID, "NVDA9100")
    Name (_UID, 0x0)
    Name (_S0W, 0x3)
#ifndef SSID_OVERRID
# error "SSID not defined!!"
#else
    Name (_SUB, SSID_OVERRID)
#endif

    Method (_STA) {
        Return(0xf)
    }

    Name (_CRS, ResourceTemplate () {
        MEMORY32FIXED (ReadWrite, 0x1A6A0000, 0x10000, )
        MEMORY32FIXED (ReadWrite, 0x1A6B0000, 0x10000, )
        Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake) { 722 }   // INT ID=690
        Interrupt (ResourceConsumer, Level, ActiveHigh, ExclusiveAndWake) { 723 }   // INT ID=691
        // SOUNDWIRE0_CLK, PD=1, PU=0, Driving:8mA
        PinFunction (Exclusive, 0x90, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x8}) { 66 }
        // SOUNDWIRE0_CLK, PD=1, PU=1, Driving:8mA
        PinFunction (Exclusive, 0xF0, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x8}) { 67 }
        // SOUNDWIRE1_CLK, PD=1, PU=0, Driving:8mA
        PinFunction (Exclusive, 0x90, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x8}) { 126 }
        // SOUNDWIRE1_DAT0, PD=1, PU=1, Driving:8mA
        PinFunction (Exclusive, 0xF0, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x8}) { 127 }
        // SOUNDWIRE1_DAT1, PD=1, PU=1, Driving:8mA
        PinFunction (Exclusive, 0xF0, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x8}) { 128 }
        // SOUNDWIRE1_DAT2, PD=1, PU=1, Driving:8mA
        PinFunction (Exclusive, 0xF0, 1, "\\_SB.GIO0", 0, ResourceConsumer, , RawDataBuffer (0x2) {0x4, 0x8}) { 129 }
    })

    Name (_DSD, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () { "afe-base", Package() { 0x1A690000, 0x10000 } },
            Package (2) { "acpi-scd-hw-ver", 1 },
            Package (2) { "acpi-scd-tzd-delay", 3 },
            Package (2) { "acpi-scd-controller-list", 0x00000003 },
            Package (2) { "acpi-scd-controller-id", 0x0 },
            Package (2) { "mipi-sdw-0-sw-interface-revision", 0x00020001 },
            Package (2) { "mipi-sdw-0-manager-list", 0x00000001 },
            Package (2) { "mipi-sdw-1-sw-interface-revision", 0x00020001 },
            Package (2) { "mipi-sdw-1-manager-list", 0x00000001 },
            Package (2) { "acpi-scd-sdca-inbox-supported", 1}
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"), // Hierarchical Extension
        Package() {
            Package(2) {"mipi-sdw-0-link-0-subproperties", "SWM0" },
            Package(2) {"mipi-sdw-1-link-0-subproperties", "SWM1" },
            Package(2) {"acpi-scd-0-subproperties", "SCD0" },
            Package(2) {"acpi-scd-1-subproperties", "SCD1" },
            Package(2) {"acpi-scd-sdca-perif-hid-subproperties", "SSPH" },
        }
    })

    Name (SWM0, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "mipi-sdw-sw-interface-revision", 0x00020001 },
            Package (2) { "mipi-sdw-clock-stop-mode0-supported", 0 },
            Package (2) { "mipi-sdw-clock-stop-mode1-supported", 0 },
            Package (2) { "mipi-sdw-clock-frequencies-supported", Package() { 6144000 } },
            Package (2) { "mipi-sdw-default-frame-rate", 24000 },
            Package (2) { "mipi-sdw-default-frame-row-size", 64 },
            Package (2) { "mipi-sdw-default-frame-col-size", 8 },
        },
    })

    Name (SWM1, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package (2) { "mipi-sdw-sw-interface-revision", 0x00020001 },
            Package (2) { "mipi-sdw-clock-stop-mode0-supported", 0 },
            Package (2) { "mipi-sdw-clock-stop-mode1-supported", 0 },
            Package (2) { "mipi-sdw-clock-frequencies-supported", Package() { 6144000 } },
            Package (2) { "mipi-sdw-default-frame-rate", 24000 },
            Package (2) { "mipi-sdw-default-frame-row-size", 64 },
            Package (2) { "mipi-sdw-default-frame-col-size", 8 },
        },
    })
    // SCD0, SCD1 and SSPH have been moved to OdmSoundWireSlaveDevice.asl
}
#endif // _NVIDIA
