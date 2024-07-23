/** @file
  Header file for TDX Init Library

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

#ifndef _PEI_TDX_LIB_H_
#define _PEI_TDX_LIB_H_

#include <CMrcInterface.h>
#include <PiPei.h>

#define UUID_BUFFER_SIZE                16
#define RESERVED_BUFFER_LENGTH          12
#define MAXIMUM_NUMBER_OF_RANKS         ( MAX_CONTROLLER * MAX_DDR5_CHANNEL * MAX_DIMMS_IN_CHANNEL * MAX_RANK_IN_DIMM )
#define SIZE_OF_MANIFEST_MAC            ( 32 ) // 32 bytes
#define MAXIMUM_SIZE_OF_MANIFEST        ( sizeof(DIMM_DESCRIPTION) * MAXIMUM_NUMBER_OF_RANKS + SIZE_OF_MANIFEST_MAC )
#define BANK_GROUP_BITS                 2
#define BANK_ADDRESS_BITS               2
#define COLUMN_BITS                     10
#define ROW_BITS                        16

//
// Definitions for DIMM Manifest's header
//
#define ACTM_UUID                       { 0xe4, 0x74, 0x4f, 0xe7, 0x63, 0xf7, 0x4a, 0x74, \
                                          0xb4, 0x35, 0x0d, 0x54, 0xa5, 0x48, 0x46, 0xf9 }
#define ACTM_EMPTY_SIZE                 0
#define ACTM_VERSION                    3
#define ACTM_RESERVED_SPACE             { 0 }

//
// Definition for CMR table
//
#define MCHECK_MAX_CMR_ENTRIES          32

//
// Definitions for ACTM Module
//
#define BIOS_IMAGE_HEADER_SIZE          20

typedef struct {
  UINT64 CmrBase; // Base address of CMR, aligned to 4KB, bits 11:0 are always 0
  UINT64 CmrSize; // Size of CMR in bytes, aligned to 4KB, bits 11:0 are always 0
} MCHECK_CMR; // Total: 16 bytes

typedef struct {
  // TDX 1V0
  MCHECK_CMR             Cmr[MCHECK_MAX_CMR_ENTRIES];
  UINT32                 NumCmrArrayEntries; // Actual number of populated CMRs
} MCHECK_PARAM_INFO_SEAM_DATA;

#pragma pack (push, 1)
///
/// Header for DIMM manifest
///
typedef struct {
  UINT8   Uuid[UUID_BUFFER_SIZE];
  UINT16  Size;
  UINT16  Version;
  UINT8   Reserved[RESERVED_BUFFER_LENGTH];
} ACTM_HEADER_DESCRIPTION;

///
/// Data for DIMM manifest
///
typedef struct {
  UINT8 MemoryController;
  UINT8 Channel;
  UINT8 Dimm;
  UINT8 Rank;
  UINT8 SubRankBits;
  UINT8 BankGroupBits;
  UINT8 BankAddressBits;
  UINT8 ColumnBits;
  UINT8 RowBits;
  UINT8 Asymmetric;
  UINT8 ModeRegister58;
  UINT8 ModeRegister59;
} DIMM_DESCRIPTION;

typedef struct {
  ACTM_HEADER_DESCRIPTION  Header;
  UINT8                    ManifestBlob[MAXIMUM_SIZE_OF_MANIFEST];
} ACTM_DIMM_MANIFEST;

typedef struct {
  ACTM_DIMM_MANIFEST ActmDimmManifest;
} ACTM_DIMM_MANIFEST_HOB;

typedef struct {
  UINT32 MktmeEnable;
  UINT32 TdxEnable;
  UINT32 VtdEnable;
  UINT32 VmxEnable;
} TDX_POLICY;

#pragma pack (pop)

/**
  Perform Trust Domain Execution(Tdx) initialization.

  @param[in] TdxPolicy           - TDx policy enable
  @param[in] MrcData             - MRC Parameter Structure
  @param[in] TdxActmModuleAddr   - Tdx Actm Module address
  @param[in] TdxActmModuleSize   - Tdx Actm Module Size

  @retval VOID              - No value to return
**/
VOID
TdxInit (
  IN VOID          *TdxPolicy,
  IN MrcParameters *CONST MrcData,
  IN UINTN         TdxActmModuleAddr,
  IN UINT32        TdxActmModuleSize
  );

/**
  Reports if Tdx pre-requisite supported or not

  @param[in] MktmePolicy   - Mktme policy enable
  @param[in] VtdPolicy     - Vtd policy enable
  @param[in] VmxPolicy     - Vmx policy enable

  @retval TRUE             If TDX pre-requisite is supported
  @retval FALSE            If TDX pre-requisite is not supported
**/
BOOLEAN
CheckTdxDependancy (
  UINT32 MktmePolicy,
  UINT32 VtdPolicy,
  UINT32 VmxPolicy
 );

/**
  Gets DIMM information from registers and fills DIMM manifest for
  Alias Check Trusted Module (ACTM).

  @param[in] MrcData        - MRC Parameter Structure

  @retval EFI_SUCCESS if no error
  @retval EFI_ERROR if other operations fail
  @retval EFI_OUT_OF_RESOURCES if the HOB resource allocation fail

**/
EFI_STATUS
EFIAPI
PublishActmDimmManifest (
  IN MrcParameters   *const MrcData
  );

/**
  Interrogates DIMM modules and store information from SPD to DIMM
  manifest for Alias Check Trusted Module (ACTM). The data retrieved
  from SPD represents the number of bits.

  @param[in] MrcData              - MRC Parameter Structure
  @param[out] ActmDimmManifest - Pointer to header of DIMM manifest

  @retval EFI_SUCCESS if no error
**/
EFI_STATUS
EFIAPI
ActmPopulateDimmManifest (
  IN MrcParameters   *const MrcData,
  OUT ACTM_DIMM_MANIFEST     *ActmDimmManifest
 );

/**
  Fills given Cmr array with CMR entries and sets NumCmrArrayEntries to a corresponding number of
  populated entries

  @param[in,out] CmrList                     A pointer to an MCHECK_PARAM_INFO_SEAM_DATA struture

  @retval EFI_SUCCESS           Success
  @retval EFI_OUT_OF_RESOURCES  If the resource allocation fail
**/
EFI_STATUS
FillCmrArray(
  VOID
  );

/**
  Perform sorting on memory descriptor array to get contiguous memory region if any

  @param[in,out] *ResourceDescriptorList     A pointer to an array of temporary MCHECK_CMR structures
  @param[in]     DescriptorCount             Number of memory descriptor array entries populated

  @retval VOID   - No value to return
**/
VOID
PerformSorting (
  IN OUT MCHECK_CMR      *ResourceDescriptorList,
  IN UINT8               DescriptorCount
  );

/**
  Build CMR table from temporary cmr array and merge the contiguous memory region if any

  @param[in] *ResourceDescriptorList       A pointer to an array of temporary MCHECK_CMR structures
  @param[in] DescriptorCount               Number of Cmr array entries populated
  @param[in,out] *Cmr                      A pointer to an array of MCHECK_CMR structures
  @param[in,out] *CmrIndex                 Number of actual Cmr array entries populated after merging the contiguous memory region

  @retval VOID    - No value to return
**/
VOID
BuildCmrTable (
  IN MCHECK_CMR       *ResourceDescriptorList,
  IN UINT8            DescriptorCount,
  IN OUT MCHECK_CMR   *Cmr,
  IN OUT UINT32       *CmrIndex
  );

/**
  This function will be called when MRC is done and for generating CMR entries

  @param  PeiServices      Indirect reference to the PEI Services Table.
  @param  NotifyDesc       Address of the notification descriptor data structure.
  @param  Ppi              Address of the PPI that was installed.

  @retval EFI_SUCCESS          If the function completed successfully.
  @retval EFI_OUT_OF_RESOURCES If the resource allocation fail
**/
EFI_STATUS
EFIAPI
PeiTdxMemoryNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
 );

/**
  Count the number of memory Descriptors which are available memory regions in system memory

  @retval NumbDescriptor           Number of memory Descriptors
**/
UINT8
GetNumbDescriptors(
  VOID
  );

/**
  Launches ACTM using GETSEC[ENTERACCS].

  @param[in] ActmAddress            Address to ACTM binary
  @param[in] ActmSize               Size of the ACTM binary
**/
UINT64
EFIAPI
AsmLaunchActm (
  UINTN TdxActmModuleAddr,
  UINT32 TdxActmModuleSize
  );

/**
  Reserve Tdx Memory
  @param[in,out] TopUseableMemAddr       Top usable memory address
  @param[in,out] TopMemSize              Top of memory size
  @param[in]     ResourceAttributeTested Memory tested attribute
  @param[out]    BaseAddress             Seamrr Base Address
**/
VOID
PeiTdxMemoryAllocation(
  IN OUT EFI_PHYSICAL_ADDRESS     *TopUseableMemAddr,
  IN OUT UINT64                   *Touud,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested,
  OUT EFI_PHYSICAL_ADDRESS        *BaseAddress
  );

/**
  Reserve Tdx Memory

  @param[in] SeamrrBase               Seamrr Base Address
  @param[in] SeamrrSize               Seamrr Size
  @param[in] ResourceAttributeTested  Memory tested attribute
**/
VOID
PeiTdxMemoryAllocationV2 (
  IN EFI_PHYSICAL_ADDRESS         SeamrrBase,
  IN UINT64                       SeamrrSize,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested
  );

/**
  Reserve Tdx Memory and program IMR and MSR for SeamRr

**/
VOID
PeiTdxProgramImrBelow4Gb (
  VOID
  );

#endif // _PEI_TDX_LIB_H_
