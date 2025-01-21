/** @file
  This file declares CPU limitated definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _OC_LIMITS_H_
#define _OC_LIMITS_H_

//
// Max number of VF point offset
//
#ifndef MAX_OC_VF_POINTS
#define MAX_OC_VF_POINTS                15
#endif

//
// Max OC Domains
//
#ifndef MAX_OC_DOMAINS
#define MAX_OC_DOMAINS                  10
#endif

//
// Max number of all Cores
//
#define MAX_OC_CORES                    64

//
// Max number of Big Cores
//
#define MAX_OC_BIG_CORES                8

//
// Max number of Big Core Turbo Ratio Limit Groups
// As per Turbo_Ratio_Limit MSR 1AEh
//
#define MAX_OC_BIG_CORE_TRL_GROUPS      8

//
// Max number of Atom Cores
//
#define MAX_OC_ATOM_CORES               32

//
// Max number of Atom Clusters
//
#define MAX_OC_ATOM_CLUSTERS            8

//
// Max ID of Atom Clusters
//
#define MAX_OC_ATOM_CLUSTERS_ID         0x10

//
// Max ID of Modules
//
#define MAX_OC_MODULE_ID                16

//
// Max number CCP Modules
//
#ifndef MAX_OC_CCP_MODULES
#define MAX_OC_CCP_MODULES              16
#endif
//
// Max number of C-DIE
//
#ifndef MAX_CDIE
#define MAX_CDIE                        2
#endif

#ifndef C_DIE_0
#define C_DIE_0                        0
#endif

#ifndef C_DIE_1
#define C_DIE_1                        1
#endif

#endif
