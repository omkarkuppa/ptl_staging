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

#ifdef _AMD
# define DP1_WORDLENGTH 16, 24
# define DP4_WORDLENGTH 24
#else
# ifdef DMIC_16BIT
#  define DP1_WORDLENGTH 16
#  define DP4_WORDLENGTH 16
# else
#  define DP1_WORDLENGTH 24
#  define DP4_WORDLENGTH 24
# endif
#endif


Name (_DSD, Package()   // _DSD: Device-Specific Data
{
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), /* Device Properties for _DSD */
    Package()
    {
        Package(2) {"mipi-sdw-sw-interface-revision", 0x00020001},              // Revision: v 2.1
        Package(2) {"mipi-sdw-sdca-interface-revision", MIPI_SDW_SDCA_INTERFACE_REVISION_VAL},  // Integer: SDCA 1.0
        Package(2) {"mipi-sdw-wake-up-unavailable", 0},                         // Boolean
        Package(2) {"mipi-sdw-test-mode-supported", 0},                         // Boolean
        Package(2) {"mipi-sdw-clock-stop-mode1-supported", 0},                  // Boolean
        Package(2) {"mipi-sdw-simplified-clockstopprepare-sm-supported", 0},    // Boolean
        //Package(2) {"mipi-sdw-clockstopprepare-timeout", 500},                  // Integer
        //Package(2) {"mipi-sdw-peripheral-channelprepare-timeout", 50},          // Integer
        //Package(2) {"mipi-sdw-clockstopprepare-hard-reset-behavior", 0x0},      // Bitmap
        //Package(2) {"mipi-sdw-highPHY-capable", 0},                             // Boolean
        Package(2) {"mipi-sdw-paging-supported", 1},                            // Boolean SCP_AddrPage1/2 are implemented
        //Package(2) {"mipi-sdw-bank-delay-supported", 0},                        // Boolean
        //Package(2) {"mipi-sdw-port15-read-behavior", 0},                        // Boolean
        //Package(2) {"mipi-sdw-manager-list", 0x0},                              // Bitmap
        Package(2) {"mipi-sdw-source-port-list", 0x1E},                         // Bitmap: DP1(MIC), DP2(UAJ_MIC), DP3(AMP_REF), DP4(MIC_HWKWS) (connected to OT113, OT36, OT25, OT13)
        Package(2) {"mipi-sdw-sink-port-list", 0x60},                           // Bitmap: DP5(AMP), DP6(UAJ_SPK) (connected to IT21, IT41)
        Package(2) {"mipi-sdw-dp-0-supported", 1},                              // Boolean
        Package(2) {"mipi-sdw-sdca-interrupt-register-list", 0xf},              // Bitmap
        Package(2) {"mipi-sdw-commit-register-supported", 1},                   // Boolean
#ifdef ENABLE_MULTILANE
        Package(2) {"mipi-sdw-lane-1-mapping", "mipi-sdw-manager-lane-1"},      // Package Name
        Package(2) {"mipi-sdw-lane-1-bus-holder", 0},                           // Boolean
#endif
#ifdef SIDECAR_GPIO_SPEAKER_SELECT
        Package()
        {
            "spk-id-gpios", Package()
            {
                AF01, 0, 0, 0,
            },
        },
#endif
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdw-dp-0-subproperties", "PDP0"},                     // Package Name
        Package(2) {"mipi-sdw-dp-1-source-subproperties", "PDP1"},              // Package Name
        Package(2) {"mipi-sdw-dp-2-source-subproperties", "PDPN"},              // Package Name
        Package(2) {"mipi-sdw-dp-3-source-subproperties", "PDPN"},              // Package Name
        Package(2) {"mipi-sdw-dp-4-source-subproperties", "PDP4"},              // Package Name
        Package(2) {"mipi-sdw-dp-5-sink-subproperties", "PDPN"},                // Package Name
        Package(2) {"mipi-sdw-dp-6-sink-subproperties", "PDPN"},                // Package Name
    },
}) //End _DSD

Name(PDP0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdw-port-wordlength-configs", Package() {8,16,24}},   // Package
        Package(2) {"mipi-sdw-port-bra-mode-list", 0x1},                        // Bitmap
        Package(2) {"mipi-sdw-bra-flow-controlled", 1},                         // Boolean
        Package(2) {"mipi-sdw-bra-imp-def-response-supported", 1},              // Boolean
        Package(2) {"mipi-sdw-bra-role-supported", 1},                          // Boolean
        //Package(2) {"mipi-sdw-port-channelprepare-timeout", 0},                 // Integer
        Package(2) {"mipi-sdw-simplified-channelprepare-sm", 0},                // Boolean
        Package(2) {"mipi-sdw-imp-def-dp0-interrupts-supported", 0},            // Boolean
        Package(2) {"mipi-sdw-imp-def-bpt-supported", 0},                       // Boolean
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdw-port-bra-mode-0", "BRA0"},                        // Package
    },
}) //End PDP0

Name(BRA0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        //Package(2) {"mipi-sdw-bra-mode-bus-frequency-configs", package() {}}, // Package
        Package(2) {"mipi-sdw-bra-mode-max-data-per-frame", 470},               // Integer
        //Package(2) {"mipi-sdw-bra-mode-min-us-between-transactions", 0},        // Integer
        //Package(2) {"mipi-sdw-bra-mode-max-bandwidth", 0},                      // Integer
        Package(2) {"mipi-sdw-bra-mode-block-alignment", 4},                    // Integer: 4B alignment
    },
}) //End BRA0

Name(PDPN, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdw-port-wordlength-configs", Package() {8,16,24}},   // Package
        Package(2) {"mipi-sdw-data-port-type", 0},                              // Integer
        Package(2) {"mipi-sdw-channel-number-list", Package() {0, 1} },         // Allow channels 0 and 1.
        Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3}},      // Package
        Package(2) {"mipi-sdw-port-encoding-type", 0x01},                       // PCM twos compliment
        Package(2) {"mipi-sdw-modes-supported", 0x01},                          // Isochronous mode only
        Package(2) {"mipi-sdw-max-async-buffer", 0},                            // Do not buffer
        Package(2) {"mipi-sdw-block-packing-mode ", 1},                         // Block packing modes are supported on Cohen
        Package(2) {"mipi-sdw-max-grouping-supported", 0},                      // Integer
        Package(2) {"mipi-sdw-simplified-channelprepare-sm", 0},                // Boolean
        //Package(2) {"mipi-sdw-port-channelprepare-timeout", 0},                 // Integer
        //Package(2) {"mipi-sdw-imp-def-dpn-interrupts-supported", 0x0},          // Bitmap
        //Package(2) {"mipi-sdw-block-packing-mode", 0},                          // Bitmap
        Package(2) {"mipi-sdw-port-encoding-type", 0x02},                       // Bitmap
#ifdef ENABLE_MULTILANE
        Package(2) {"mipi-sdw-lane-list", Package() {0, 1} },                   // Preferred priority for Lane use.
#endif
    },
}) //End PDPN

Name(PDP1, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdw-port-wordlength-configs", Package() {DP1_WORDLENGTH}},   // Package
        Package(2) {"mipi-sdw-data-port-type", 0},                              // Integer
        Package(2) {"mipi-sdw-channel-number-list", Package() {0, 1} },         // Allow channels 0 and 1.
        Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3}},      // Package
        Package(2) {"mipi-sdw-port-encoding-type", 0x01},                       // PCM twos compliment
        Package(2) {"mipi-sdw-modes-supported", 0x01},                          // Isochronous mode only
        Package(2) {"mipi-sdw-max-async-buffer", 0},                            // Do not buffer
        Package(2) {"mipi-sdw-block-packing-mode ", 1},                         // Block packing modes are supported on Cohen
        Package(2) {"mipi-sdw-max-grouping-supported", 0},                      // Integer
        Package(2) {"mipi-sdw-simplified-channelprepare-sm", 0},                // Boolean
        //Package(2) {"mipi-sdw-port-channelprepare-timeout", 0},                 // Integer
        //Package(2) {"mipi-sdw-imp-def-dpn-interrupts-supported", 0x0},          // Bitmap
        //Package(2) {"mipi-sdw-block-packing-mode", 0},                          // Bitmap
        Package(2) {"mipi-sdw-port-encoding-type", 0x02},                       // Bitmap
#ifdef ENABLE_MULTILANE
        Package(2) {"mipi-sdw-lane-list", Package() {0, 1} },                   // Preferred priority for Lane use.
#endif
    },
}) //End PDP1

Name(PDP4, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdw-port-wordlength-configs", Package() {DP4_WORDLENGTH}},   // Package
        Package(2) {"mipi-sdw-data-port-type", 0},                              // Integer
        Package(2) {"mipi-sdw-max-grouping-supported", 0},                      // Integer
        Package(2) {"mipi-sdw-simplified-channelprepare-sm", 0},                // Boolean
        //Package(2) {"mipi-sdw-port-channelprepare-timeout", 0},                 // Integer
        //Package(2) {"mipi-sdw-imp-def-dpn-interrupts-supported", 0x0},          // Bitmap
        Package(2) {"mipi-sdw-min-channel-number", 1},                          // Integer
        Package(2) {"mipi-sdw-max-channel-number", 2},                          // Integer
        Package(2) {"mipi-sdw-channel-combination-list", Package() {0x3}},      // Package
        Package(2) {"mipi-sdw-modes-supported", 0x0F},                          // Bitmap
        Package(2) {"mipi-sdw-max-async-buffer", 0x08},                         // Integer
        //Package(2) {"mipi-sdw-block-packing-mode", 0},                          // Bitmap
        Package(2) {"mipi-sdw-port-encoding-type", 0x02},                       // Bitmap
#ifdef ENABLE_MULTILANE
        Package(2) {"mipi-sdw-lane-list", Package() {0, 1} },                   // Preferred priority for Lane use.
#endif
    },
}) // End PDP4
