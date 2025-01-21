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
// NEED_CONFIGS
// NEED_CONFIGS register value LSB is set based on the configuration
//

#ifdef COHEN_NEED_CONFIGS_SIDECAR
# define COHEN_NEED_CONFIGS_SIDECAR_BIT (1 << 7)
#else
# define COHEN_NEED_CONFIGS_SIDECAR_BIT (0 << 7)
#endif

#ifdef COHEN_NEED_CONFIGS_SECURE
# define COHEN_NEED_CONFIGS_SECURE_BIT  (1 << 6)
#else
# define COHEN_NEED_CONFIGS_SECURE_BIT  (0 << 6)
#endif

#ifdef COHEN_NEED_CONFIGS_JACK
# define COHEN_NEED_CONFIGS_JACK_BIT    (1 << 5)
#else
# define COHEN_NEED_CONFIGS_JACK_BIT    (0 << 5)
#endif

#ifdef COHEN_NEED_CONFIGS_CODEC
# define COHEN_NEED_CONFIGS_CODEC_BIT   (1 << 4)
#else
# define COHEN_NEED_CONFIGS_CODEC_BIT   (0 << 4)
#endif

#ifdef COHEN_NEED_CONFIGS_SWIRE
# define COHEN_NEED_CONFIGS_SWIRE_BIT   (1 << 3)
#else
# define COHEN_NEED_CONFIGS_SWIRE_BIT   (0 << 3)
#endif

#ifdef COHEN_NEED_CONFIGS_WDOG
# define COHEN_NEED_CONFIGS_WDOG_BIT    (1 << 2)
#else
# define COHEN_NEED_CONFIGS_WDOG_BIT    (0 << 2)
#endif

#ifdef COHEN_NEED_CONFIGS_GPIO
# define COHEN_NEED_CONFIGS_GPIO_BIT    (1 << 1)
#else
# define COHEN_NEED_CONFIGS_GPIO_BIT    (0 << 1)
#endif

#ifdef COHEN_NEED_CONFIGS_PLL
# define COHEN_NEED_CONFIGS_PLL_BIT     (1 << 0)
#else
# define COHEN_NEED_CONFIGS_PLL_BIT     (0 << 0)
#endif

//
// NEED_CONFIGS:
//      7         6         5         4         3         2         1         0
// +---------+---------+---------+---------+---------+---------+---------+---------+
// | SIDECAR |  SECURE |   JACK  |  CODEC  |  SWIRE  |   WDOG  |   GPIO  |    PLL  |
// +---------+---------+---------+---------+---------+---------+---------+---------+
//
// All 256 possible values of COHEN_NEED_CONFIGS_VAL_0C, the NEED_CONFIGS LSB.
//

#ifdef COHEN_NEED_CONFIGS_SIDECAR
# ifdef COHEN_NEED_CONFIGS_SECURE
#  ifdef COHEN_NEED_CONFIGS_JACK
#   ifdef COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xFF
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xFE
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xFD
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xFC
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xFB
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xFA
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xF9
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xF8
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xF7
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xF6
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xF5
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xF4
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xF3
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xF2
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xF1
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xF0
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   else  // !COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xEF
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xEE
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xED
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xEC
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xEB
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xEA
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xE9
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xE8
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xE7
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xE6
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xE5
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xE4
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xE3
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xE2
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xE1
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xE0
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   endif // COHEN_NEED_CONFIGS_CODEC
#  else  // !COHEN_NEED_CONFIGS_JACK
#   ifdef COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xDF
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xDE
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xDD
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xDC
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xDB
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xDA
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xD9
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xD8
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xD7
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xD6
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xD5
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xD4
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xD3
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xD2
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xD1
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xD0
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   else  // !COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xCF
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xCE
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xCD
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xCC
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xCB
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xCA
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xC9
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xC8
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xC7
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xC6
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xC5
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xC4
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xC3
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xC2
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xC1
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xC0
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   endif // COHEN_NEED_CONFIGS_CODEC
#  endif // COHEN_NEED_CONFIGS_JACK
# else  // !COHEN_NEED_CONFIGS_SECURE
#  ifdef COHEN_NEED_CONFIGS_JACK
#   ifdef COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xBF
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xBE
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xBD
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xBC
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xBB
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xBA
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xB9
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xB8
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xB7
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xB6
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xB5
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xB4
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xB3
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xB2
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xB1
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xB0
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   else  // !COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xAF
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xAE
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xAD
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xAC
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xAB
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xAA
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xA9
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xA8
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xA7
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xA6
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xA5
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xA4
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xA3
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xA2
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xA1
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0xA0
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   endif // COHEN_NEED_CONFIGS_CODEC
#  else  // !COHEN_NEED_CONFIGS_JACK
#   ifdef COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x9F
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x9E
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x9D
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x9C
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x9B
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x9A
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x99
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x98
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x97
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x96
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x95
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x94
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x93
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x92
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x91
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x90
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   else  // !COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x8F
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x8E
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x8D
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x8C
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x8B
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x8A
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x89
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x88
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x87
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x86
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x85
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x84
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x83
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x82
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x81
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x80
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   endif // COHEN_NEED_CONFIGS_CODEC
#  endif // COHEN_NEED_CONFIGS_JACK
# endif // COHEN_NEED_CONFIGS_SECURE
#else  // !COHEN_NEED_CONFIGS_SIDECAR
# ifdef COHEN_NEED_CONFIGS_SECURE
#  ifdef COHEN_NEED_CONFIGS_JACK
#   ifdef COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x7F
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x7E
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x7D
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x7C
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x7B
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x7A
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x79
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x78
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x77
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x76
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x75
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x74
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x73
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x72
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x71
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x70
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   else  // !COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x6F
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x6E
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x6D
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x6C
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x6B
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x6A
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x69
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x68
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x67
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x66
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x65
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x64
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x63
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x62
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x61
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x60
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   endif // COHEN_NEED_CONFIGS_CODEC
#  else  // !COHEN_NEED_CONFIGS_JACK
#   ifdef COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x5F
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x5E
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x5D
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x5C
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x5B
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x5A
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x59
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x58
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x57
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x56
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x55
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x54
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x53
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x52
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x51
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x50
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   else  // !COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x4F
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x4E
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x4D
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x4C
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x4B
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x4A
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x49
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x48
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x47
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x46
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x45
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x44
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x43
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x42
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x41
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x40
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   endif // COHEN_NEED_CONFIGS_CODEC
#  endif // COHEN_NEED_CONFIGS_JACK
# else  // !COHEN_NEED_CONFIGS_SECURE
#  ifdef COHEN_NEED_CONFIGS_JACK
#   ifdef COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x3F
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x3E
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x3D
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x3C
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x3B
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x3A
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x39
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x38
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x37
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x36
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x35
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x34
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x33
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x32
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x31
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x30
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   else  // !COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x2F
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x2E
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x2D
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x2C
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x2B
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x2A
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x29
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x28
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x27
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x26
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x25
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x24
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x23
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x22
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x21
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x20
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   endif // COHEN_NEED_CONFIGS_CODEC
#  else  // !COHEN_NEED_CONFIGS_JACK
#   ifdef COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x1F
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x1E
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x1D
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x1C
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x1B
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x1A
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x19
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x18
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x17
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x16
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x15
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x14
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x13
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x12
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x11
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x10
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   else  // !COHEN_NEED_CONFIGS_CODEC
#    ifdef COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x0F
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x0E
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x0D
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x0C
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x0B
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x0A
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x09
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x08
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    else  // !COHEN_NEED_CONFIGS_SWIRE
#     ifdef COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x07
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x06
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x05
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x04
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     else  // !COHEN_NEED_CONFIGS_WDOG
#      ifdef COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x03
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x02
#       endif // COHEN_NEED_CONFIGS_PLL
#      else  // !COHEN_NEED_CONFIGS_GPIO
#       ifdef COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x01
#       else   // !COHEN_NEED_CONFIGS_PLL
#        define COHEN_NEED_CONFIGS_VAL_0C 0x00
#       endif // COHEN_NEED_CONFIGS_PLL
#      endif // COHEN_NEED_CONFIGS_GPIO
#     endif // COHEN_NEED_CONFIGS_WDOG
#    endif // COHEN_NEED_CONFIGS_SWIRE
#   endif // COHEN_NEED_CONFIGS_CODEC
#  endif // COHEN_NEED_CONFIGS_JACK
# endif // COHEN_NEED_CONFIGS_SECURE
#endif // COHEN_NEED_CONFIGS_SIDECAR
