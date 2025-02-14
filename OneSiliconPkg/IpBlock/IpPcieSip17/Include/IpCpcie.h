/** @file
  IP API for PCIE

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef __IP_PCIE_H__
#define __IP_PCIE_H__

#include <IpCsi.h>
#include <IpPcieConfig.h>
#if !defined (MDEPKG_NDEBUG)
#define IP_PCIE_PRINT_ALL
#endif

#define VERSION_IPFWINTFEXT_CPCIE_GEN    (0x17)
#define VERSION_IPFWINTFEXT_CPCIE_MAJOR  (0x00)
#define VERSION_IPFWINTFEXT_CPCIE_MINOR  (0x0014)

#define VERSION_IPFWINTFEXT_CPCIE  ((VERSION_IPFWINTFEXT_CPCIE_MAJOR<<16) | (VERSION_IPFWINTFEXT_CPCIE_MINOR<<0)) // FORMAT: 0x00MMmmmm
#define VERSION_IPFWINTF_CPCIE     ((VERSION_IPFWINTFEXT_CPCIE_GEN<<24) | (VERSION_IPFWINTFEXT_CPCIE & 0x00FFFFFF))   // FORMAT: 0xGGMMmmmm

typedef struct _IP_PCIE_INST IP_PCIE_INST;
struct _IP_PCIE_INST {
  IP_CSI_INST_PREFIX    Prefix;

  // IP Wrapper Register Contexts
  IP_WR_REG_CNTXT       RegCntxt_Cfg_Pri;   // PCIe Configspace primary access
  IP_WR_REG_CNTXT       RegCntxt_Mem_Rcrb;  // RCRB Register access
  IP_WR_REG_CNTXT       RegCntxt_Prvt_Cfg;  // PCR access
  IP_WR_REG_CNTXT       RegCntxt_Cfg_Sb;    // SideBand access

  // IP Wrapper Error Contexts
  IP_WR_ERROR_CNTXT     ErrorCntxt;
  IP_WR_ERROR_CNTXT     ErrorCntxtWarning;
  IP_WR_ERROR_CNTXT     ErrorAssert;

  // IP Wrapper Print Contexts
  IP_WR_PRINT_CNTXT    PrintError;
  IP_WR_PRINT_CNTXT    PrintWarning;
  IP_WR_PRINT_CNTXT    PrintLevel1;

  // IP Wrapper Version Specific Contexts
  IP_WR_VSC_CNTXT       IpWrVscCntxt;

  // IP Wrapper Timing Contexts
  IP_WR_TIME_CNTXT      TimeCntxt;

  // IP Wrapper Mem Contexts
  IP_WR_MEM_CNTXT       MemCntxt;

  IP_PCIE_SIP_VERSION   SipVersion;

  IP_PCIE_INTEGRATION  Integration;

  IP_PCIE_SIP_PHY_VERSION  SipPhyVersion;
  //
  // Unique ID used to identify root port in debug prints
  //
  UINT32            RpIndex;

  //
  // TRUE if this root port device is a main controller root port.
  // For instance in controllers with 4 root ports root port 0 is a main
  // root port which implements registers controlling behavior of the whole
  // controller.
  //
  BOOLEAN           IsController;
  //
  //
  //
  BOOLEAN           IocDecoded;
  //
  //  PCIe root port private configuration.
  //
  IP_PCIE_ROOT_PORT_PRIVATE_CONFIG  PrivateConfig;

  ///
  /// These members describe the configuration of each PCIe root port.
  ///
  IP_PCIE_ROOT_PORT_COMMON_CONFIG      PcieRpCommonConfig; ///an instance of Pcie Common Config

  //
  // Controller Instance for the PCIe root port.
  //
  void*             ControllerInst;

  // Mmio Base Address of MemRcrb
  UINT64            MemRcrbBase;

  // PCIE Private Variables
  //IP_PCIE_INST_IMPL_EXT _ImplExt;
};

typedef enum {
  // IpPcieFeatId<CapName>
  IpPcieFeatIdUnknown = 0,
  IpPcieFeatIdMps,
  IpPcieFeatIdIoApic,
  IpPcieFeatIdCcc,
  IpPcieFeatIdPtm,
  IpPcieFeatIdLtr,
  IpPcieFeatIdL1ss,
  IpPcieFeatIdAspm,
  IpPcieFeatIdMls,
  IpPcieFeatIdPds,
  IpPcieFeatIdLr,
  IpPcieFeatIdSd
} IP_PCIE_FEAT_ID;

typedef enum {
  IpPcieFeatValDis = 0,

  //<=== NOTE: add new emums to end for backwards compatability
} IP_PCIE_FEAT_VAL;

typedef enum
{
  IpPcieEventIdUnknown = 0,

  //<=== NOTE: add new emums to end for backwards compatibility
} IP_PCIE_EVENT_ID;

typedef enum
{
  IpPcieEventValUnknown = 0,

  //<=== NOTE: add new emums to end for backwards compatibility
} IP_PCIE_EVENT_VAL;

typedef enum {
  IpPcie4x1      = 0,
  IpPcie1x2_2x1  = 1,
  IpPcie2x2      = 2,
  IpPcie1x4      = 3,
  IpPcie1x8      = 5,
  IpPcie1x8_2x4  = 6,
  IpPcie1x16     = 7,
} IP_PCIE_CONTROLLER_CONFIG;

//
// PRINT_* Macros
//
// Easy PRINT_*() Macros
#if defined(IP_PCIE_PRINT_ALL) || defined(IP_PCIE_PRINT_ERROR)
#define PRINT_ERROR(...)                        \
  do {                                          \
    IpWrPrint (pInst->PrintError, __VA_ARGS__); \
    IpWrError (pInst->ErrorCntxt, 0, __LINE__); \
  } while (0)
#define PRINT_ERROR_NO_CNTXT(...)                                                 \
  do {                                                                   \
    IpWrPrint ((IP_WR_PRINT_CNTXT) NULL, __VA_ARGS__);                   \
    IpWrError ((IP_WR_ERROR_CNTXT) NULL, IpWrErrorMajorError, __LINE__); \
  } while (0)
#else
#define PRINT_ERROR(...)                        \
  do {                                          \
    IpWrError (pInst->ErrorCntxt, 0, __LINE__); \
  } while (0) // no printf
#define PRINT_ERROR_NO_CNTXT(...)                                                 \
  do {                                                                   \
    IpWrError ((IP_WR_ERROR_CNTXT) NULL, IpWrErrorMajorError, __LINE__); \
  } while (0) // no printf
#endif

#if defined(IP_PCIE_PRINT_ALL) || defined(IP_PCIE_PRINT_WARNING)
#define PRINT_WARNING(...)                             \
  do {                                                 \
    IpWrPrint (pInst->PrintWarning, __VA_ARGS__);      \
    IpWrError (pInst->ErrorCntxtWarning, 0, __LINE__); \
  } while (0)
#else
#define PRINT_WARNING(...)                             \
  do {                                                 \
    IpWrError (pInst->ErrorCntxtWarning, 0, __LINE__); \
  } while (0) //NO printf
#endif

#if defined(IP_PCIE_PRINT_ALL) || defined(IP_PCIE_PRINT_LEVEL1)
#define PRINT_LEVEL1(...) IpWrPrint (pInst->PrintLevel1, __VA_ARGS__)
#else
#define PRINT_LEVEL1(...) \
  do {                    \
  } while (0) // NOOP at compile time
#endif
//
// DELAY_* Macros
//

#define DELAY_US(MicroSeconds) IpWrDelayUs(pInst->TimeCntxt, MicroSeconds)

//
// Additional REG_PCI_CFG_* Macros
//

// AND then OR the current value of a register and write it back
// Enclosed in do{}while(0) to prevent variable naming conflicts
#define REG_PCI_CFG_AND_THEN_OR(Seg, Bus, Dev, Fun, Offset, AndData, OrData, Flags)   \
  UINT64 Data64;                                                                      \
  Data64 = RegPciCfgRead(Seg, Bus, Dev, Fun, Offset, Flags);                          \
  Data64 &= (UINT64)AndData;                                                          \
  Data64 |= (UINT64)OrData;                                                           \
  RegPciCfgWrite(Seg, Bus, Dev, Fun, Offset, Data64, Flags);

// AND the current value of a register and write it back
// Enclosed in do{}while(0) to prevent variable naming conflicts
#define REG_PCI_CFG_AND(Seg, Bus, Dev, Fun, Offset, AndData, Flags)   \
  UINT64 Data64;                                                      \
  Data64 = RegPciCfgRead(Seg, Bus, Dev, Fun, Offset, Flags);          \
  Data64 &= (UINT64)AndData;                                          \
  RegPciCfgWrite(Seg, Bus, Dev, Fun, Offset, Data64, Flags);

/**
  Get controller device to which current root port
  belongs. When called this function does not
  progresses iteration.

  @param[out] pInst   Point of pInst

  @retval IpCsiStsSuccess         Controller device found.
  @retval IpCsiStsErrorUnknownId  Didn't find controller device.
**/
IP_CSI_STATUS
IpInstGetThisController (
  IP_PCIE_INST    *OrgInst,
  IP_PCIE_INST    **pInst
  );

/**
  Reset the iteration to the index pInst.

  @param[in]       Index  the port number of pInst
  @param[out]      pInst  Point of pInst

  @retval  IpCsiStsSuccess         The function completes successfully
  @retval  IpCsiStsErrorBadParam   pInst incorrect

**/
IP_PCIE_INST*
IpInstResetToIndex (
   UINT32            Index,
   IP_PCIE_INST      *pInst
  );

/**
  Reset the iteration to the first pInst.

  @param[out]      pInst  Point of pInst

  @retval  IpCsiStsSuccess         The function completes successfully
  @retval  IpCsiStsErrorBadParam   pInst incorrect

**/
IP_PCIE_INST*
IpInstResetToFirst (
  IP_PCIE_INST      *pInst
  );

/**
  InstInit for this version of IP

  @param[in] pInst  The instance, already allocated by the FSP

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpPcieInstInit (
  IP_PCIE_INST                *pInst
);

/**
  Init IP

  @param[in]  pInst instance

  @retval         IpCsiStsSuccess on success
  @retval         !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpPcieIpInit (
  IP_PCIE_INST  *pInst
);

/**
  Pre Link Active Programming

  @param[in] pInst          *pInst

  @retval                    IpCsiStsSuccess on success
  @retval                    !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpPciePreLinkActiveProgramming (
  IP_PCIE_INST        *pInst
  );

  /**
This function disables the root ports that are either fuse disabled, disabled via setup or has No Device Attached

  @param[in] IP_PCIE_INST       *pInst

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
IpPcieDisableRootPort (
  IP_PCIE_INST         *pInst
  );

  /**
  Performs mandatory Root Port Initialization.
  This function is silicon-specific and configures proprietary registers.

  @param[in] IP_PCIE_INST       *pInst

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
IpPcieRpInit (
  IP_PCIE_INST    *pInst
  );

/**
  Initiate Speed Change

  @param[in] pInst               *pInst
  @param[in] MaxLinkSpeed         Lowest of LCAP.MLS, EndPointMaxSpeed

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
IpPcieRpSpeedChangeStart (
  IP_PCIE_INST    *pInst,
  UINT8            MaxLinkSpeed
  );


/**
  Checks for Link Active after initiating speed change in IpPcieRpSpeedChangeStart API.
  if link is not retrained sucessfully, revert target link speed to current link speed.

  @param[in] pInst               *pInst
  @param[in] MaxLinkSpeed        Lowest of LCAP.MLS, EndPointMaxSpeed
  @param[in] TimeoutValue        Timeout value to poll for link active after link retrain

  @retval  IpCsiStsSuccess       The function completes successfully
  @retval  IpCsiStsErrorNullPtr  pInst was NULL
**/
IP_CSI_STATUS
IpPcieRpSpeedChangeEnd (
  IP_PCIE_INST    *pInst,
  UINT8            MaxLinkSpeed,
  UINT32           TimeoutValue
  );

/**
  Assign root port function number in PCIe controller.

  @param[in] pInst                      pInst of Controller Rootport.
  @param[in] RpIndexInController        Which root port in controller to swap function for.
  @param[in] NewFun                     New function number.
  @param[in] SwapWithinController       If TRUE function swap is done within a single controller
  @param[in] SecondRpIndexInController  Second root port for which we want to change function number
  @param[in] SecondRpNewFun             Second root port function number
**/
void
IpPcieAssignRootPortFunctionNumber (
  IP_PCIE_INST    *pInst,
  UINT32          RpIndexInController,
  UINT8           NewFun,
  BOOLEAN         SwapWithinController,
  UINT32          SecondRpIndexInController,
  UINT8           SecondRpNewFun
  );

/**
  Enable PCIe root port in controller.

  @param[in] pInst                      Pointer to controller pInst.
  @param[in] RpIndexInController        Root port in controller to be enable.
**/
void
IpPcieEnableRootPortInController (
  IP_PCIE_INST    *pInst,
  UINT32          RpIndexInController
  );

/**
  Checks if root port is enabled in controller.

  @param[in] ControllerInst       Pointer to controller root port.
  @param[in] RpIndexInController  Root port index in controller.

  @retval TRUE   Root port is enabled.
  @retval FALSE  Root port is disabled.
**/
BOOLEAN
IpPcieIsRootPortEnabledInController (
  IP_PCIE_INST        *pInst,
  UINT32              RpIndexInController
  );

/**
  Returns the PCIe controller configuration (4x1, 1x2-2x1, 2x2, 1x4)

  @param[in] ControllerInst       Pointer to controller root port.

  @retval PCIe controller configuration
**/
IP_PCIE_CONTROLLER_CONFIG
IpPcieGetControllerConfig (
  IP_PCIE_INST  *pInst
  );

/**
  Check for device presence with timeout.

  @param[in]     pInst      *pInst
  @param[in]     TimeoutUs  Timeout in microseconds
  @param[in,out] Timer      Timer value, must be initialized to zero
                            before the first call of this function.
**/
BOOLEAN
IpPcieSipIsDevicePresent (
  IP_PCIE_INST    *pInst,
  UINT32          TimeoutUs,
  UINT32          *Timer
  );

/**
  Get version information

  @param[in]  pInst  The instance
  @param[in]  VerId  Version ID

  @retval            0 is returned upon any error
  @retval            return value define by VerId
**/
UINT32
IpPcieGetVersion (
  IP_PCIE_INST    *pInst,
  IP_CSI_VER_ID   VerId
  );

/**
  Get capability

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP. (must be 0 for PCIE)
  @param[in]      FeatureId  Cap ID
  @param[in, out] pCsiSts    Optional and may be NULL.  If non-NULL return API status

  @retval                    defined by FeatureId
**/
UINT32
IpPcieGetCapability (
  IP_PCIE_INST    *pInst,
  UINT32          IpTarget,
  IP_PCIE_FEAT_ID FeatureId,
  IP_CSI_STATUS   *pCsiSts
  );

/**
  Get status

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP. (must be 0 for PCIE)
  @param[in]      FeatureId  Status ID
  @param[in, out] pCsiSts    Optional and may be NULL.  If non-NULL return API status

  @retval                    defined by FeatureId
**/
UINT32
IpPcieGetStatus (
  IP_PCIE_INST    *pInst,
  UINT32          IpTarget,
  IP_PCIE_FEAT_ID FeatureId,
  IP_CSI_STATUS   *pCsiSts
  );

/**
  Get control

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP. (must be 0 for PCIE)
  @param[in]      FeatureId  Control ID
  @param[in, out] pApiSts    Optional and may be NULL.  If non-NULL return API status

  @retval                    defined by FeatureId
**/
UINT32
IpPcieGetControl (
  IP_PCIE_INST    *pInst,
  UINT32          IpTarget,
  IP_PCIE_FEAT_ID FeatureId,
  IP_CSI_STATUS   *pCsiSts
  );

/**
  Set control

  @param[in]  pInst       The instance
  @param[in]  IpTarget    IP defined value for target within the IP. (must be 0 for PCIE)
  @param[in]  FeatureId   Feature ID
  @param[in]  FeatureVal  Value based on FeatureId

  @retval                 IpCsiStsSuccess on success
  @retval                 !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpPcieSetControl (
  IP_PCIE_INST    *pInst,
  UINT32          IpTarget,
  IP_PCIE_FEAT_ID FeatureId,
  UINT32          FeatureVal
  );

/**
  Finds the Offset to a given Capabilities ID
  Each capability has an ID and a pointer to next Capability, so they form a linked list.
  This function walks the list of Capabilities present in device's pci cfg. If requested capability
  can be found, its offset is returned.
  If the capability can't be found or if device doesn't exist, function returns 0
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] pInst  the instance
  @param[in] CapId  CAPID to search for

  @retval    0      CAPID not found (this includes situation where device doesn't exit)
  @retval    Other  CAPID found, Offset of desired CAPID
**/
UINT8
IpPcieFindCapId (
  IP_PCIE_INST  *pInst,
  UINT8          CapId
  );

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Reporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] pInst  the instance
  @param[in] CapId  Extended CAPID to search for

  @retval    0         CAPID not found, this includes situation where device doesn't exist
  @retval    Other     CAPID found, Offset of desired CAPID
**/
UINT16
SipFindExtendedCapId (
  IP_PCIE_INST  *pInst,
  UINT16        CapId
  );


/**
  Returns SPR.SCB[2:1]) DTR State

  @param[in] pInst  the instance

  @retval    0       IpPcieDtrNotNeed
  @retval    1       IpPcieDtrNotReady
  @retval    2       IpPcieDtrReady
  @retval    3       IpPcieDtrBusy
**/
IP_PCIE_DTR_STAT
IpPcieGetDtrStat (
  IP_PCIE_INST  *pInst
  );


/**
  Update SPR.SCB[2:1]) DTR State

  @param[in] pInst  the instance
  @param[in] Stat   State value

  @retval  IpCsiStsSuccess    on success
  @retval  !IpCsiStsSuccess   on failure
**/
IP_CSI_STATUS
IpPcieSetDtrStat (
  IP_PCIE_INST  *pInst,
  IP_PCIE_DTR_STAT  Stat
  );

/**
  DTR train from Gen4 to Gen5

  @param[in] pInst               *pInst

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
IpPcieDtrGen4ToGen5 (
  IP_PCIE_INST   *pInst
  );

/**
  Perform write operation on RWO fileds to ensure locking of these registers

  @param[in] pInst  *pInst
**/
VOID
SipLockCapRegisters (
  IP_PCIE_INST  *pInst
  );
#endif
