/** @file
  MRC Common / Generic functions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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
#ifndef _MrcGeneral_h_
#define _MrcGeneral_h_

#include "CMrcApi.h"
#include "MrcCommon.h"
#include "MrcCrosser.h"
#include "MrcPostCodes.h"
#include "MrcChipApi.h"

#define PCH_PWRM_BASE_ADDRESS                       0xFE000000     ///< PMC MBAR MMIO base address
#define R_PCH_PWRM_GEN_PMCON_A                      0x1020
#define B_PCH_PWRM_GEN_PMCON_A_MEM_SR_MRC           MRC_BIT21
#define B_PCH_PWRM_GEN_PMCON_A_DISB_MRC             MRC_BIT23

#define R_PMC_PWRM_FUSA_STS_CTL                     0x1F30
#define B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_EN         MRC_BIT2
#define B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_PCHMODE    MRC_BIT1
#define B_PMC_PWRM_FUSA_STS_CTL_DIAGTEST_FEATURE_EN MRC_BIT0

/*
PM 0 - Wake up PHY after reset. lock PLL.
     For Product PM0 is sent through MEMSS PMA using the MEMSS_PMA_CR_BIOS_REQ register
     For TestChip PM0 is sent through PHY PMA using the DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_REG register for SAGV0

PM 6 - Put PHY in a low power state after each SAGV point

PM 8 - Block RComp before entering PM3/6.  ACK once any Rcomp in progress finishes.
     For TestChip, send a PM sequence PM8->PM6->PM0 for all SAGV Points except for SAGV0 and when SAGV is disabled

PM 14 - Initiate save of all trained register values to SRAM.
      For PHY this will be issued using DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_REG(warm and S3 boot flows).
      For MC this will be issued using MEMSS_PMA_CR_INIT_STATE (Cold Boot)

PM 13 - Relock PHY PLL for LP5 Jedec reset / Init.This will be done using DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_REG
and it will be specific to PHY.MemSS PMA will not be involved.This is a difference from legacy projects
because Xtensa cannot send request to any block outside PHY.
*/
#define PM_MESSAGE_0                       (0)   ///< PM Message 0
#define PM_MESSAGE_0_VOLTAGE_ONLY          (100) ///< PM Message 0 Voltage Change Only
#define PM_MESSAGE_6                       (6)   ///< PM Message 6
#define PM_MESSAGE_8                       (8)   ///< PM Message 8
#define PM_MESSAGE_13                      (13)  ///< PM Message 13
#define PM_MESSAGE_14                      (14)  ///< PM Message 14
#define PM_MSG_PHY (0)    ///<PM14 will be sent to both MC and PHY. This define specifies which IP its sent to.
#define PM_MSG_MC  (1)

///
/// Define the total memory size of a channel, see MEMSS_PMA_CR_CAPABILITIES_MISC_DDRSZ
///
typedef enum {
  tcs64GB,  ///< 64 GB per channel (Unlimited)
  tcs8GB,   ///< 8 GB
  tcs4GB,   ///< 4 GB
  tcs2GB    ///< 2 GB
} MrcTotalChannelSize;

///
/// IBECC OPERATION MODE, see MC0_IBECC_CONTROL_STRUCT.OPERATION_MODE
///
typedef enum {
  IbeccPartialProtect,    ///< Functional Mode (performs range checks)
  IbeccNonProtect,        ///< Makes all requests Non protected and ignore range checks
  IbeccAllProtect,        ///< Makes all requests protected and ignore range checks
} IbeccOpMode;

extern const MrcFrequency SagvFreqPor[CALC_MRC_DDR_TYPE_MAX (MAX_MRC_DDR_TYPE)][MAX_SAGV_POINTS];
extern const UINT8 SaGvGearPor[CALC_MRC_DDR_TYPE_MAX (MAX_MRC_DDR_TYPE)][MAX_SAGV_POINTS];


/**
  Program MC/CPGC engines to either Normal mode of operation
  or CPGC training mode.

  @param[in] MrcData       - The MRC general data.
  @param[in] IsNormalMode  - TRUE for Normal mode, FALSE for CPGC mode

  @retval Returns mrcSuccess or mrcFail if CPGC Mode is locked.
**/
MrcStatus
MrcSetNormalMode (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsNormalMode
  );

/**
  This function enables Normal Mode and configures the Power Down Modes
  for the boot flows other than Cold Boot.

  @param[in] MrcData - The MRC general data.

  @retval Always returns mrcSuccess.
**/
extern
MrcStatus
MrcNormalMode (
  IN MrcParameters *const MrcData
  );

/**
  Enables IBECC if supported by CPU, and if memory controller population is symmetric

  @param[in] MrcData - Pointer to the MRC Debug structure.

  @retval mrcSuccess Ibecc task completed
**/
MrcStatus
MrcIbecc (
  IN MrcParameters *MrcData
  );

/**
  Poll for IBECC_MEMORY_INIT.INIT_ALL_RANGES to go from 1->0

  @param[in] MrcData - Include all MRC global data. include also the memory map data.

  @retval MrcStatus -  if the bit was reset within the timeout
**/
MrcStatus
MrcPollIbeccFSMInit (
  IN MrcParameters *const MrcData
  );

/**
  This function locks the memory controller and memory map registers.

  @param[in] MrcData - Include all MRC global data.
**/
extern
void
McRegistersLock (
  IN     MrcParameters *const MrcData
  );

/**
  This function sets the DISB bit in General PM Configuration 2 B:D:F 0,31,0 offset 0xA2.
**/
extern
void
MrcSetDISB (
  IN MrcParameters * const MrcData
  );

/**
  This function sets the safe mode configuration using the input override values.

  @param[in, out] MrcData - All the MRC global data.

  @retval Returns mrcSuccess if the timing overrides have been conpleted.
**/
MrcStatus
MrcSetSafeModeOverrides (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function reads the input data structure and sets the appropriate timing overrides in the output structure.

  @param[in, out] MrcData - All the MRC global data.

  @retval Returns mrcSuccess if the timing overrides have been conpleted.
**/
extern
MrcStatus
MrcSetOverrides (
  IN OUT MrcParameters *const MrcData
  );

/**
  Read the current DDRPHY temperature.
  @param[in]  MrcData     - Include all MRC global data.
  @param[out] Temperature - DDRPHY temperature in degrees C
  @retval Status - whether the temperature read was successful
**/
MrcStatus
MrcReadTemperature (
  IN  MrcParameters *const MrcData,
  OUT INT32                *Temperature
  );

#ifdef MRC_DEBUG_PRINT
/**
  Print the input parameters to the debug message output port.

  @param[in] MrcData - The MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcPrintInputParameters (
  IN MrcParameters *const MrcData
  );

/**
  Print the specified memory to the serial message debug port.

  @param[in] Debug - Serial message debug structure.
  @param[in] Start - The starting address to dump.
  @param[in] Size  - The amount of data in bytes to dump.
**/
extern
void
MrcPrintMemory (
  IN MrcDebug *const    Debug,
  IN const UINT8 *const Start,
  IN const UINT32       Size
  );
#endif


/**
  Sets IpVersion (MC, DDRIO Version, Derivative, Segment, Stepping) in MrcData.

  @param[out] MrcData     - The Mrc Host data structure

  @retval mrcSuccess if the IP Segment and IP Version is valid.  Otherwise mrcFail
**/
MrcStatus
MrcSetCpuInformation (
  OUT MrcParameters  *MrcData
  );

/**
  Preforms an unsigned rounded division operation of size UINT64.
  Follows this formula: (a, b)  (((a) + (b) / 2) / (b))
  @param[in] MrcData     - The Mrc Host data structure
  @param[in] Var1        - Numerator
  @param[in] Var2        - Denominator
  @retval Rounded value from UINT64 division
**/
UINT64
UnsignedDivideRoundx64 (
  IN MrcParameters   *MrcData,
  IN UINT64          Var1,
  IN UINT64          Var2
  );

/**
  This function return the MRC version.
  @param[in]  MrcData - include all the MRC general data.
  @param[out] Version - Location to store the MRC version.
**/
void
MrcVersionGet (
     IN  const MrcParameters *const MrcData,
     OUT       MrcVersion    *const Version
  );

/**
  Check the Blue MRC vs Green binary version
  @param[in] MrcData - MRC Host structure.
**/
void
MrcVersionCheck (
  IN MrcParameters     *MrcData
  );

/**
  Print the MRC version to the MRC output device.
  @param[in] MrcData  - MRC Host structure.
  @param[in] Version  - The MRC version.
**/
void
MrcVersionPrint (
  IN MrcParameters     *MrcData,
  IN const MrcVersion  *Version
  );

/**
  This function sets the project specific overrides in the Input and Output structure

  @param[in, out] MrcData - All the MRC global data.
 */
VOID
MrcSetPrjOverrides (
  IN OUT MrcParameters *const MrcData
  );

/**
  Modify tRDRDdg and tRDRDsg timings by 1tCK. Does not apply modification to tRDRDsg
  in BG mode.

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Enable  - TRUE to add 1tCK, FALSE to subtract.
 */
VOID
MrcModifyRdRdTimings (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN              Enable
  );

/**
 * @brief Get the Mc Ibecc Hash Mask and LSB values.
 *
 * @param[in]      MrcData  - All the MRC global data.
 * @param[in, out] HashMask - Pointer to the Hash Mask set.
 * @param[in, out] HashLsb  - Pointer to the Hash LSB to set.
 */
VOID
GetMcIbeccHash (
  IN     MrcParameters *const MrcData,
  IN OUT UINT32               *HashMask,
  IN OUT UINT32               *HashLsb
  );

/**
  Adjust Wck Mode

  @param[in] MrcData - The MRC global data.
**/
VOID
MrcAdjustWckMode (
  IN MrcParameters *MrcData
  );

/**
  Determine if SaGv is enabled from SaGv Input.
  @param[in] MrcData - include all the MRC general data.
  @retval TRUE  - If SaGv is enabled.
          FALSE - If SaGv is disabled.
**/
BOOLEAN
MrcIsSaGvEnabled (
  IN  MrcParameters *const MrcData
  );

/**
  Check for symmetric memory controller population

  @param[in] MrcData - Pointer to the MRC Debug structure.

  @retval TRUE memory controller is symmetric
  @retval FALSE memory controller is not symmetric
**/
BOOLEAN
MrcIsIbeccSymmetric (
  IN MrcParameters *MrcData
  );

/**
  This function is a wrapper for MrcGenMrsFsmCleanNonFastBoot().

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] MrData  - Pointer to an array of MR data to configure the MRS FSM with.
  @param[in] CleanAll - If set to TRUE, MrData values will be ignored and all Control Registers will be cleared

  @retval mrcFail if clean failed.
  @retval mrcSuccess otherwise.
**/
MrcStatus
MrcGenMrsFsmCleanNonFastBoot (
  IN  MrcParameters *MrcData,
  IN  MRC_GEN_MRS_FSM_MR_TYPE MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM],
  IN  BOOLEAN       CleanAll
  );

/**
  Set the Global driver grace period in MC.

  @param[in] MrcData - Pointer to the MRC Debug structure.
  @param[in] Controller    - Controller to setup
  @param[in] Channel       - Channel to setup

**/
VOID
MrcSetGlobalGraceCounter (
  IN MrcParameters *MrcData,
  IN UINT32        Controller,
  IN UINT32        Channel
  );

#endif // _MrcGeneral_h_
