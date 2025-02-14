/** @file
  Memory Initialization PEIM.

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
@todo code cleanup for txt memoryinit overlap
**/
#ifndef _MemoryInit_h_
#define _MemoryInit_h_

#include <Base.h>
#include <PiPei.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/SiPolicy.h>
#include <ConfigBlock.h>
#include <CMrcInterface.h>  // for MrcParameters
#include <MemInfoHob.h>
#include <DprInfoHob.h>
#include "CpuRegs.h"

#include <CMemoryInitPtl.h>


///
/// SMRAM range definitions
///
#define MC_ABSEG_HSEG_PHYSICAL_START  0x000A0000
#define MC_ABSEG_HSEG_LENGTH          0x00020000
#define MC_ABSEG_CPU_START            0x000A0000
#define MC_HSEG_CPU_START             0xFEDA0000

///
/// Memory Map definitions
///
#define ADDRESS_SPACE_START           0x0
#define LOWER_MEMORY_START            0x100000

///
/// Maximum number of memory ranges reported by GetMemoryMap()
///
#define MAX_RANGES    (16)

#define MEM_EQU_4GB 0x100000000ULL
#define MB_TO_GB_MUL  0x100000ULL

///
/// TPM Status and Time-out
///
#define TPM_TIME_OUT            750

#define PLATFORM_ID_MOBILE      1

// Simics Memflow Registers
#define MSR_SIMICS_MEMFLOWS_LOW   0xdeadbee0
#define MSR_SIMICS_MEMFLOWS_HIGH  0xdeadbee1

// Must match MRC definitions for TrainingStepsEn, TrainingStepsEn2 and TrainingStepsEn3
#pragma pack(push, 1)
typedef union {
  struct {
    TrainingStepsEn Low;
    TrainingStepsEn2 High;
  } Bits;
  UINT64 Data;
} SIMICS_MEMFLOW_STRUCT;

typedef union {
  struct {
    TrainingStepsEn3 Low;
    UINT32           Reserved32;
  } Bits;
  UINT64 Data;
} SIMICS_MEMFLOW2_STRUCT;
#pragma pack(pop)

///
/// Memory range types
///
typedef enum {
  DualChannelDdrMainMemory,
  DualChannelDdrSmramCacheable,
  DualChannelDdrSmramNonCacheable,
  DualChannelDdrGraphicsMemoryCacheable,
  DualChannelDdrGraphicsMemoryNonCacheable,
  DualChannelDdrReservedMemory,
  DualChannelDdrReservedWriteBackMemory,
  DualChannelDdrMaxMemoryRangeType
} PEI_DUAL_CHANNEL_DDR_MEMORY_RANGE_TYPE;

///
/// Memory map range information
///
#pragma pack(push, 1)
typedef struct {
  UINT8                                   RowNumber;
  EFI_PHYSICAL_ADDRESS                    PhysicalAddress;
  EFI_PHYSICAL_ADDRESS                    CpuAddress;
  EFI_PHYSICAL_ADDRESS                    RangeLength;
  PEI_DUAL_CHANNEL_DDR_MEMORY_RANGE_TYPE  Type;
} PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE;
#pragma pack(pop)

#define MRC_INSTANCE_SIGNATURE  SIGNATURE_32 ('M', 'R', 'C', 'I')

typedef struct _MRC_INSTANCE {
  UINT32                       Signature;
  EFI_PEI_NOTIFY_DESCRIPTOR    NotifyDescriptor;
  IN EFI_PEI_FILE_HANDLE       FileHandle;
  BOOLEAN                      MrcExecuted;
} MRC_INSTANCE;

#define MRC_INSTANCE_FROM_NOTIFY_THIS(a) CR (a, MRC_INSTANCE, NotifyDescriptor, MRC_INSTANCE_SIGNATURE)

typedef struct {
  UINT8    SpdAddress;
  UINT8    Controller;
  UINT8    Channel;
  UINT8    Dimm;
} MRC_SPD_MATCHED;

/**
  A notify callback function to determine if main memory detection function can be executed or not

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The notify callback PPI.  Not used.

  @retval EFI_SUCCESS          - MRC has been executed successfully
  @retval EFI_NOT_READY        - One of the dependency not ready yet for MRC execution.
**/
extern
EFI_STATUS
EFIAPI
PrememoryInitCallBack (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  );

/**
  Main starting point for system memory initialization.
    1. Get SysBootMode and MrcBootMode
    2. Locate SiPreMemPolicyPpi
    3. Locate S3DataPtr from MiscPeiPreMemConfig.
    4. SaveDataValid := TRUE if S3DataPtr is not NULL.
    5. If SysBootMode is BOOT_ON_S3_RESUME and S3Data is not valid:
    -> ASSERT.
    6. If MrcBootMode is Warm boot, but S3 data is not valid :
    -> change MrcBootMode to Cold boot.
    7. If MrcBootMode is Cold boot:
    -> Run MRC code
    -> Save S3 Restore Data
    Else
    -> Run MRC_S3Resume
    8. Run MRC_Done().
    9. Install EFI memory HOBs.

  @param[in] FfsHeader (or FileHandle) - Pointer to Firmware File System file header
  @param[in] PeiServices               - General purpose services available to every PEIM.
  @param[in] MrcInstance               - MRC instance structure for callback implementation

  @retval EFI_SUCCESS      - Memory initialization completed successfully.
  @retval EFI_NOT_READY    - Cannot locate SA Policy.
  @retval EFI_NOT_FOUND    - No S3 data in S3 Boot Mode.
  @retval EFI_DEVICE_ERROR - MemoryInit failed or IOSAV Memory test failed.
**/
extern
EFI_STATUS
PeimMemoryInit (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN MRC_INSTANCE              *MrcInstance
  );

/**
  This function installs memory for all paths.

  @param[in] MrcData                - Mrc data structure
  @param[in] PeiServices            - PEI Services table.
  @param[in] SysBootMode            - The specific boot path that is being followed.

  @retval EFI_SUCCESS          The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES Out of Resources.
**/
extern
EFI_STATUS
InstallEfiMemory (
  IN MrcParameters             *CONST  MrcData,
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN       EFI_BOOT_MODE       SysBootMode
  );

/**
  Determine the memory size desired based on HOB memory information.

  @param[in, out] Size        - The memory size to return.

  @retval Nothing.
**/
extern
void
RetrieveRequiredMemorySize (
  OUT UINTN                   *Size
  );

/**
  This function returns the memory ranges to be enabled, along with information
  describing how the range should be used. The MemoryMap buffer will be filled in and
  NumRanges will contain the actual number of memory ranges that are to be enabled.

  @param[in]      MrcData     - Mrc data structure
  @param[in, out] MemoryMap   - Buffer to record details of the memory ranges to be enabled.
  @param[in, out] NumRanges   - On input, this contains the maximum number of memory ranges that
                                can be described in the MemoryMap buffer.

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval EFI_BUFFER_TOO_SMALL - The specified number of ranges is too large.
**/
extern
EFI_STATUS
GetMemoryMap (
  IN MrcParameters  *CONST                        MrcData,
  IN OUT   PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE  *MemoryMap,
  IN OUT   UINT8                                  *NumRanges
  );

/**
  This function returns a pointer to the allocated hand off buffer.

  @param[in, out] Hob         - A pointer to where to store the pointer to the allocated data buffer.
  @param[in]      BlockSize   - The size of the buffer to get.
  @param[in]      Guid        - The GUID to assign to the HOB.

  @retval mrcSuccess  - Hob is successfully built.
  @retval Others      - Error occurred while creating the Hob.
**/
MrcStatus
MrcGetHobForDataStorage (
  IN OUT VOID      **Hob,
  IN     UINT16    BlockSize,
  IN     EFI_GUID  *Guid
  );

/**
  Finds the BDAT Schema List HOB if it exists.  If it does not exist, creates it

  @param[out] BdatSchemaListHob - Pointer to Schema List Hob, by reference

  @retval mrcSuccess  - Hob was successfully found or built.
  @retval Others      - Error occurred while creating or finding the Hob.
**/
MrcStatus
MrcGetBdatSchemaListHob (
  OUT  MRC_BDAT_SCHEMA_LIST_HOB  **BdatSchemaListHob
  );

/**
  This function returns a pointer to the allocated hand off buffer for the BDAT GUID.

  @param[in out] BdatMemoryHob   - Pointer to Schema List Hob, by reference
  @param[in]     BdatSchemaSize  - Size of the Bdat Schema
  @param[in]     ResultsGuid     - Pointer to GUID associated with the BDAT Memory HOB

  @retval mrcSuccess  - Hob was successfully found or built.
  @retval Others      - Error occurred while creating or finding the Hob.
**/
MrcStatus
MrcGetBdatMemoryHob (
  IN OUT VOID      **BdatMemoryHob,
  IN     UINT32    BdatSchemaSize,
  IN     EFI_GUID  *ResultsGuid
  );

/**
  This function updates the schema IDs with the Bdat GUID value.

  @param[in out] BdatSchemaHobGuidPtr  - Pointer to Schema List in a Hob, by reference
  @param[in]     BdatSchemasIdPtr      - Pointer to BdatMemoryHob Schema Id in a Hob, by reference
  @param[in]     ResultsGuid           - Pointer to GUID associated with the BDAT Memory HOB

  @retval None
**/
VOID
MrcUpdateBdatHobIds (
  IN OUT  MrcParameters *const MrcData,
  IN OUT  VOID                 *BdatSchemaHobGuidPtr,
  IN OUT  VOID                 *BdatSchemasIdPtr,
  IN EFI_GUID                  *ResultsGuid
  );

/**
  Set up the MRC OEM data structure.

  @param[in, out] MrcData            - Pointer to the MRC global data structure.
  @param[in]     MemConfig           - MEMORY CONFIGURATION structure.
**/
extern
void
MrcSetupOem (
  IN OUT MrcParameters          *CONST   MrcData,
  IN    MEMORY_CONFIGURATION    *MemConfig
  );

/**
  Set up the MRC input data structure.

  @param[in]  SysBootMode         - Boot mode of the system.
  @param[in]  BootMode            - Boot mode of the MRC.
  @param[in]  MrcData             - Pointer to the MRC global data structure
  @param[in]  MiscConfig          - SA_MISC_CONFIG structure.
  @param[in]  IGpuPreMemConfig    - GRAPHICS_CONFIG structure.
  @param[in]  MemConfig           - MEMORY_CONFIGURATION structure.
  @param[in]  MemConfigNoCrc      - MEMORY_CONFIG_NO_CRC structure.
  @param[in]  SiCpuPolicyPpi      - The Cpu Policy PPI instance.

  @retval Updated MRC_BOOT_MODE
**/
extern
MRC_BOOT_MODE
MrcSetupMrcData (
  IN CONST EFI_BOOT_MODE              SysBootMode,
  IN CONST MRC_BOOT_MODE              BootMode,
  OUT      MrcParameters              *CONST MrcData,
  IN       MEMORY_CONFIGURATION       *CONST MemConfig,
  IN       MEMORY_CONFIG_NO_CRC       *CONST MemConfigNoCrc,
  IN       SI_PREMEM_POLICY_PPI       *CONST SiPreMemPolicyPpi,
  IN       BOOLEAN                    DidPreviousTrainingFail
  );

/**
  Check to see if user defined profile is selected and if it is, then copy the
  timing settings for this profile to the timing override structure. If user
  defined profile is not selected, then set the timing override structure to 0.

  Note that even though we set timings on a DIMM by DIMM basis, the controller
  may force DIMM timings to be the same for all DIMMs in a channel.

  @param[in, out] Inputs              - The MRC Input data structure.
  @param[in]      MemConfig           - MEMORY CONFIGURATION structure.

  @retval Nothing.
**/
extern
void
CheckForTimingOverride (
  IN OUT MrcInput               *CONST  Inputs,
  IN     MEMORY_CONFIGURATION   *MemConfig
  );

/**
  Build S3 memory data HOB

  @param[in] MrcData          Pointer to Mrc Parameters
**/
VOID
BuildMemoryS3DataHob (
  IN MrcParameters            *MrcData
  );

/**
  Build SMBIOS and OC memory data HOB

  @param[in] MrcData          Pointer to Mrc Parameters
**/
VOID
BuildMemoryInfoDataHob (
  IN MrcParameters            *MrcData
  );

/**
  Build platform memory data HOB

  @param[in]      MrcData             Pointer to Mrc Parameters
  @param[in, out] MemoryPlatformData  Hob to pass memory data for platform code
**/
VOID
BuildMemoryPlatformDataHob (
  IN     MrcParameters            *MrcData,
  IN OUT MEMORY_PLATFORM_DATA_HOB *MemoryPlatformData
  );

/**
  Check if a number is a power of two

  @param[in]      UINT32           num
  @retval         BOOLEAN          TRUE if is a power of two
**/

BOOLEAN
IsThisPowerOfTwo (
  IN UINT32 num
  );

/**
  Round a number to the next power of two

  @param[in, out] UINT32 num   -  number to be rounded to the next power of two.
  @retval         UINT32 num   -  number rounded to the next power of two.
**/

UINT32
RoundToNextPow2 (
  IN UINT32 num);

/**
  Check whether the Base Address is Naturally aligned or not - Base needs to be multiple of size.
  If not then it will return the Naturally aligned Base Address

  @param[in]       EFI_PHYSICAL_ADDRESS MemBaseAddr   - Base Address
  @param[in]       UINT64               MemSize       - Size

  @retval          EFI_PHYSICAL_ADDRESS MemBaseAddr   -  Naturally Aligned Base Address
**/
EFI_PHYSICAL_ADDRESS
NaturalAlignment (
  IN  EFI_PHYSICAL_ADDRESS  MemBaseAddr,
  IN  UINT64                MemSize
  );

/**
  After BIOS determines the total physical memory size.
  Determines TOM which is defined by the total physical memory size.
  Determines TOM minus the ME memory size. The ME memory size is calculated from MESEG_BASE and MESEG_MASK.
  Determines MMIO allocation, which is system configuration dependent.

  Determines TOLUD which is the minimum value by comparing between "4GB minus MMIO size" and "TOM minus ME size".
  Determines Graphics Stolen Base, BDSM by subtracting the graphics data stolen memory size from TOLUD.
  Graphics Data Stolen Memory size is given by GMS field in GGC register.  It must be define before this stage.
  Determines Graphics GTT Stolen Base, BGSM by subtracting the GTT graphics stolen memory size from BDSM.
  GTT Stolen Memory size is given by GGMS field in GGC register.  It must be define before this stage.
  Determines TSEG Base, TSEGMB by subtracting TSEG size from BGSM.
  TSEG should be defined.
  Remove the memory hole caused by aligning TSEG to a 8MB boundary.
  Determine whether Memory Reclaim is available.  If "TOM minus ME Stolem Memory Size" is greater than the value of TOLUD, then memory reclaim is available to enable.
  Determine REMAPBASE if reclaim is enabled.  This is the maximum value by comparing between 4GB and "TOM minus ME size".
  Determine REMAPLIMIT () if reclaim is enabled.  This is the value of REMAPBASE plus "the difference between the value in TOLUD register and the lower of either 4GB or 'TOM minus ME Stolen memory size", and then minus 1 boundary.
  Determine TOUUD. TOUUD indicates the address one byte above the maximum DRAM.  If relcaim is disabled, this value is calculated by "TOM minus ME stolen size".  Otherwise, this value is set to REMAPLIMIT plus 1MB.

  @param[in, out] MrcData - Include all MRC global data. include also the memory map data.

  @retval MrcStatus -  if the reset is succeded.
**/
extern
MrcStatus
SetMemoryMap (
  MrcParameters *const MrcData
  );

/**
  Read the last SAGV point before Warm / S3 entry from a PMC register

  @param[in out]  MrcData - All the MRC global data.

**/
VOID
MrcGetSaGvPointBeforeReset (
  IN OUT  MrcParameters *const MrcData
  );

/**
  Check Status of the last IBECC operation mode

  @param[in out]  MrcData - All the MRC global data

  @return The last IBECC operation mode setting
**/
UINT8
GetLastIbeccOpMode (
  IN OUT  MrcParameters *const MrcData
  );

/**
  Set the status of the current IBECC operation mode to the SSKPD_PCU register.
  This status will survive a warm reset.

  @param[in]  IbeccOpMode - The last configured IBECC Operation Mode

  @return The last IBECC operation mode setting
**/
VOID
SetLastIbeccOpMode (
  IN      UINT8  IbeccOpMode
  );

#endif
