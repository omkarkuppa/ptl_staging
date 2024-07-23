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

#ifndef __COMMON_6CH_H__
#define __COMMON_6CH_H__

/*
 * Flags that are normally passed for the final config during compilation
 */
#define DISABLE_AEC        1
#define AIC_V2             1
#define SIX_SPEAKERS       1
#define HWKWS              1
#define UAJ_RENDER_96KHZ   1

/*
 * Bitmasks which define which features are supported in the XU driver.
 */
#define FEATURE_BIT_HWKWS  0x00000001       /* HWKWS is supported. */
#define FEATURE_BIT_WT     0x00000002       /* Woofer Tweeter is supported. */
#define FEATURE_BIT_KNCK   0x00000004       /* Cohen + Knuckles is supported. */

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

#ifdef COHEN_TWEETER_JAMERSON_WOOFER
# define FEATURE_ENABLE_WT 0x00000002
#else
# define FEATURE_ENABLE_WT 0x00000000
#endif

#ifdef COHEN_KNUCKLES
# define FEATURE_ENABLE_KNCK 0x00000004
#else
# define FEATURE_ENABLE_KNCK 0x00000000
#endif

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
// COHEN_TWEETER_JAMERSON_WOOFER takes advatage of standalone Cohen configuration.
//
#ifdef COHEN_TWEETER_JAMERSON_WOOFER
# define COHEN_ONLY
#endif

//
// COHEN_KNUCKLES takes advatage of standalone Cohen configuration.
//
#ifdef COHEN_KNUCKLES
# define COHEN_ONLY
#endif

#endif /* __COMMON_6CH_H__ */
