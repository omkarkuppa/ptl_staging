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


        Name (_DSD, Package()   // _DSD: Device-Specific Data
        {
            #include <SndwDevTopologySt05Ssdt/CS35L56/CS35L56_DSD.asl>  // For additional _DSD entries, common to all Jamerson

#ifdef AMP_GPIO
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
#endif
        }) // End _DSD

        #include <SndwDevTopologySt05Ssdt/CS35L56/CS35L56_PDP.asl>

        Device(AF01)
        {
            Name(_ADR, 0x1)

            #include <SndwDevTopologySt05Ssdt/CS35L56/SdcaSmartAmp_CJAM3556_RefStream_common.asl>

#ifdef AMP_GPIO
            // Assign SPKR_ID GPIO for AMP
            Name (GPIA, ResourceTemplate ()
            {
                GpioIo (Shared, AMP_GPIO_PIN_CONFIG, 0x0, AMP_GPIO_DRIVE_STRENGTH, IoRestrictionInputOnly, AMP_GPIO_CTRL, 0, ResourceConsumer, , ) { AMP_GPIO }
            })

            Method (_CRS, 0x0, NotSerialized)
            {
                Return (GPIA)
            }
#endif
        }