/** @file
  The System Topology Library provides functionality needed to interpret and
  manipulate SoC address map data, in order to build/maintain register space
  topology for a system.

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

  SoC address map data is grouped in the format with header SOC_MAP_HEADER
  defined in header file

    RegisterAddressTranslation.h

  and is usually generated automatically by a tool (i.e. SoC Generator) from
  OSXML SoC release. It contains all information needed to access registers
  required by IP firmware. To publish the generated address map data, API

    SystemTopologyInstallAddrMapData()

  must be called first at the very early stage of BIOS boot (before any register
  access). It also build indices for all endpoints or address map group in the
  generated address map data, in order to speed up the information retrieval.

  Following APIs are used to override default MMCFG base, PCI bus mapping,
  specific PCI device address, etc., during the boot on platforms with dynamic
  resource assignment.

    SystemTopologyUpdateMmcfgBase()
    SystemTopologyUpdateBusMapping()
    SystemTopologyOverridePciCfg()
    SystemTopologyRemoveEndpoint()

  The system topology data is supposed to be used in almost all boot stages
  (PEI/DXE/SMM). Following API is designed to migrate the data from one stage
  to another.

    SystemTopologyMigrateData()
    SystemTopologySetDataPointer()

  Derived from OSXML concept, in System Topology Library, BAR is a virtual
  register which might be combined by more than one silicon registers. A cache
  mechanism is employed to speed up retrieving BAR value since it won't be
  changed often during boot. Following APIs are designed to maintain the BAR
  value.

    SystemTopologyGetBarByIndex()
    SystemTopologyGetBarCachedValue()
    SystemTopologySetBarCachedValue()
    SystemTopologyFlushBarCache()

  To access a register, the user usually calls SystemTopologyGetDataPointer()
  to get the pointer to system topology data structure, and then call
  SystemTopologyGetAccessor() to get the address map information used to
  complete the register access.

@par Specification Reference:
**/

#ifndef __SYSTEM_TOPOLOGY_LIB_H__
#define __SYSTEM_TOPOLOGY_LIB_H__

#include <RegisterAddressTranslation.h>

/**
  Invalidate all cached BAR values.

  @param[in] Socket     Socket number
**/
VOID
EFIAPI
SystemTopologyFlushBarCache (
  IN UINT8     Socket
  );

/**
  Override PCI config space address of given endpoint instance.

  @param[in] Socket     Socket number
  @param[in] Id         Endpoint ID within SoC
  @param[in] Instance   Instance of endpoint with Id
  @param[in] Bus        New PCI bus number
  @param[in] Device     New PCI device number
  @param[in] Function   New PCI function number
  @param[in] Offset     New PCI offset

  @retval EFI_SUCCESS if given endpoint instance was found and updated.
  @retval EFI_INVALID_PARAMETER if Socket is invalid.
  @retval EFI_NOT_FOUND if given endpoint instance or accessor was not found.
**/
EFI_STATUS
EFIAPI
SystemTopologyOverridePciCfg (
  IN UINT8  Socket,
  IN UINT64 Id,
  IN UINT16 Instance,
  IN UINT8  Bus,
  IN UINT8  Device,
  IN UINT8  Function,
  IN UINT16 Offset
  );

/**
  Removes one endpoint from system's topology data.

  @param[in] Id               ID of the endpoint to be removed.

  @retval EFI_SUCCESS     System topology data updated
  @retval EFI_NOT_FOUND   Could not find topology data
**/
EFI_STATUS
EFIAPI
SystemTopologyRemoveEndpoint (
  IN UINT64 Id
  );

/**
  Update stored bus mapping for one socket

  @param[in] Socket             - Socket to update busses of
  @param[in] PhysicalBus        - Hardware assigned or default bus number
  @param[in] LogicalBus         - Re-mapped bus number by KTI or equivalent

  @retval EFI_SUCCESS           - Bus map update applied successfully
  @retval EFI_NOT_FOUND         - Could not find topology data
  @retval EFI_INVALID_PARAMETER - Desired socket outside range
**/
EFI_STATUS
EFIAPI
SystemTopologyUpdateBusMapping (
  IN UINT8 Socket,
  IN UINT8 PhysicalBus,
  IN UINT8 LogicalBus
  );

/**
  Update MMCFG base address for given socket.

  @param[in] Socket             Socket number
  @param[in] MmcfgBase          New MMCFG base address

  @retval EFI_SUCCESS           Base address update applied successfully
  @retval EFI_NOT_FOUND         Could not find topology data
  @retval EFI_INVALID_PARAMETER Desired socket outside range
**/
EFI_STATUS
EFIAPI
SystemTopologyUpdateMmcfgBase (
  IN UINT8  Socket,
  IN UINT64 MmcfgBase
  );

/**
  Updates the system topology pointers, in case the map has been moved.

  Most common case will be pre-mem->post-mem, as the a PEIM containing
  the map data will have moved after shadow to memory.

  Another case is DXE->SMM since SMM code at OS Runtime cannot access memory
  outside SMRAM.

  @param[in,out] Topology       Pointer to SYSTEM_TOPOLOGY data structure.

  @retval EFI_SUCCESS           System topology pointers updated
  @retval EFI_NOT_FOUND         Could not find topology data
**/
EFI_STATUS
EFIAPI
SystemTopologyFixupDataPointers (
  IN OUT SYSTEM_TOPOLOGY      *Topology OPTIONAL
  );

/**
  Parses address map data blob and populates topology structure.

  This is the starting point for RAT based register access, as without topology
  data no addresses can be resolved.

  @param[in] AddrMapData            Pointer to address map data bytestream.
  @param[in] AddrMapDataSize        Size of data pointed by AddrMapData.
  @param[in] AddrMapStringTable     Pointer to string table of endpoint instances.
  @param[in] AddrMapStringTableUnit Size of pointer to each string in
                                    AddrMapStringTable.
  @param[in] MmcfgBases             Pointer to MMCFG base address of each socket.
  @param[in] SocketCount            Number of socket.

  @retval EFI_SUCCESS             Address map data populated.
  @retval EFI_OUT_OF_RESOURCES    Could not allocate HOB.
  @retval EFI_INVALID_PARAMETER   Invalid field in AddrMapData.
  @retval EFI_COMPROMISED_DATA    Illegal data in AddrMapData or AddrMapStringTable.
**/
EFI_STATUS
EFIAPI
SystemTopologyInstallAddrMapData (
  IN UINT8                  *AddrMapData,
  IN UINT32                 AddrMapDataSize,
  IN UINT8                  *AddrMapStringTable,
  IN UINT8                  AddrMapStringTableUnit,
  IN EFI_PHYSICAL_ADDRESS   *MmcfgBases,
  IN UINT8                  SocketCount
  );

/**
  Update the state of Posted Write in order to enable or disable it.

  @param[in]  Topology      Pointer to SYSTEM_TOPOLOGY data structure.
  @param[in]  Socket        Socket number.
  @param[in]  PostedWrite   Flag indicating the state of Posted Write. Zero
                            means disabling.

  @retval EFI_SUCCESS     if updated successfully.
          EFI_NOT_FOUND   if topology data hasn't been installed.
**/
EFI_STATUS
EFIAPI
SystemTopologyUpdatePostedWrite (
  IN SYSTEM_TOPOLOGY          *Topology,
  IN UINT8                    Socket,
  IN BOOLEAN                  PostedWrite
  );

/**
  Get register accessor descriptor of given endpoint instance.

  If Type is valid register space type other than sideband, the accessor for
  that Type will be returned, if it exists.

  If Type is one of RegisterSpaceMsgCfg/RegisterSpaceMsgMem/RegisterSpaceMsgIo,
  this function will return accessor with type of RegisterSpaceCfg/
  RegisterSpaceMemBar/RegisterSpaceIoBar respectively, if it exists. Otherwise,
  accessor for type RegisterSpaceMsgCr will be returned.

  If Type is given by RegisterSpaceAny, this function will return the lowest
  avaiable type in REGISTER_SPACE_TYPE.

  @param[in]     Topology         Pointer to SYSTEM_TOPOLOGY data structure.
  @param[in]     Id               Endpoint ID.
  @param[in]     Socket           Socket number.
  @param[in]     Instance         Index of endpoint instance.
  @param[in,out] Type             Type of register access space.
  @param[in,out] Accessor         Pointer to accessor descriptor data structure.
  @param[in,out] InstanceString   Pointer to string describing the endpoint
                                  instance.

  @retval EFI_SUCCESS     if accessor was found and valid.
          EFI_NOT_FOUND   if given endpoint instance was not found.
**/
EFI_STATUS
EFIAPI
SystemTopologyGetAccessor (
  IN SYSTEM_TOPOLOGY          *Topology,
  IN UINT64                   Id,
  IN UINT8                    Socket,
  IN UINT16                   Instance,
  OUT REGISTER_SPACE_TYPE     *Type,
  OUT ACCESSOR_DESCRIPTOR     *Accessor,
  OUT CHAR8                   **InstanceString
  );

/**
  Get MMCFG base address of given socket.

  @param[in] Topology     Pointer to SYSTEM_TOPOLOGY data structure.
  @param[in] Socket       Socket number.

  @retval Valid address or 0 if the given Socket is not valid.
**/
UINT64
EFIAPI
SystemTopologyGetMmcfgBase (
  IN SYSTEM_TOPOLOGY  *Topology,
  IN UINT8            Socket
  );

/**
  Retrieve the descriptor of BAR specified by BarIndex.

  The BAR descriptor is used to get the whole address combined by value of each
  register described in COMBINED_BAR data structure.

  @param[in] Topology     Pointer to SYSTEM_TOPOLOGY data structure.
  @param[in] Socket       Socket number.
  @param[in] BarIndex     Index to the requested BAR.

  @retval Address of COMBINED_BAR data structure; Or
          NULL if no BAR found.
**/
COMBINED_BAR*
EFIAPI
SystemTopologyGetBarByIndex (
  IN SYSTEM_TOPOLOGY    *Topology,
  IN UINT8              Socket,
  IN UINTN              BarIndex
  );

/**
  Update cached value of BAR specified by BarIndex.

  Since the BAR might be involved in calculating the base address of endpoint
  instance accessed last time, this function invalidates cached endpoint id to
  avoid out of date value.

  @param[in] Topology     Pointer to SYSTEM_TOPOLOGY data structure.
  @param[in] Socket       Socket number.
  @param[in] BarIndex     Index to the requested BAR.
  @param[in] BarValue     Value of BAR.
**/
VOID
EFIAPI
SystemTopologySetBarCachedValue (
  IN SYSTEM_TOPOLOGY    *Topology,
  IN UINT8              Socket,
  IN UINTN              BarIndex,
  IN UINT64             BarValue
  );

/**
  Get cached value of BAR given by BarIndex.

  @param[in] Topology     Pointer to SYSTEM_TOPOLOGY data structure.
  @param[in] Socket       Socket number.
  @param[in] BarIndex     Index to the requested BAR.

  @retval (UINT64)-1 if given BAR was not found; otherwise the cached value.
**/
UINT64
EFIAPI
SystemTopologyGetBarCachedValue (
  IN SYSTEM_TOPOLOGY    *Topology,
  IN UINT8              Socket,
  IN UINTN              BarIndex
  );

/**
  Migrate pointers in SYSTEM_TOPOLOGY data structure stored in HOB and/or other
  global storage once physical memory is ready.

  If ReallocMemory is TRUE, this function will allocate the same size of
  SYSTEM_TOPOLOGY before fixing the pointers. Otherwise, it simply fixes
  pointers.

  @param[in] ReallocMemory    Re-allocate memory to hold the data in old place.

  @retval EFI_SUCCESS           All pointers in SYSTEM_TOPOLOGY have been migrated.
          EFI_NOT_FOUND         No SYSTEM_TOPOLOGY found in system.
          EFI_OUT_OF_RESOURCES  Not enough memory to allocate.
**/
EFI_STATUS
EFIAPI
SystemTopologyMigrateData (
  IN  BOOLEAN     ReallocMemory
  );

/**
  Returns pointer to system topology data.

  @retval NULL if topology not found, valid pointer otherwise
**/
SYSTEM_TOPOLOGY *
EFIAPI
SystemTopologyGetDataPointer (
  VOID
  );

/**
  Store the pointer to SYSTEM_TOPOLOGY data structure to a place supporting
  quick access globally.

  @param[in] Topology     Pointer to SYSTEM_TOPOLOGY data structure.
**/
VOID
EFIAPI
SystemTopologySetDataPointer (
  IN SYSTEM_TOPOLOGY  *Topology
  );

/**
  Prototype of the UEFI Runtime Service ConvertPointer().

  @param  DebugDisposition   Supplies type information for the pointer being converted.
  @param  Address            The pointer to a pointer that is to be fixed to be the
                             value needed for the new virtual address mapping being
                             applied.

  @retval  EFI_SUCCESS            The pointer pointed to by Address was modified.
  @retval  EFI_NOT_FOUND          The pointer pointed to by Address was not found to be part of
                                  the current memory map. This is normally fatal.
  @retval  EFI_INVALID_PARAMETER  Address is NULL.
  @retval  EFI_INVALID_PARAMETER  *Address is NULL and DebugDispositio

**/
typedef
EFI_STATUS
(EFIAPI *CONVERT_FUNC) (
  IN UINTN                  DebugDisposition,
  IN OUT VOID               **Address
  );

/**
  Convert all addresses referenced in SYSTEM_TOPOLOGY to be virtual address.

  @param  ConvertAddress    Pointer to function performing the address conversion.

  @retval  EFI_SUCCESS            All addresses in SYSTEM_TOPOLOGY was modified.
  @retval  EFI_NOT_FOUND          SYSTEM_TOPOLOGY global was not found.

**/
EFI_STATUS
EFIAPI
SystemTopologyConvertPointers (
  IN CONVERT_FUNC    ConvertAddress
  );

#endif // #ifndef __SYSTEM_TOPOLOGY_LIB_H__
