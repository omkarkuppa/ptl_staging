/** @file
  Protocol used for specifying platform related CPU information and policy setting.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#ifndef _CPU_POLICY_PROTOCOL_H_
#define _CPU_POLICY_PROTOCOL_H_

extern EFI_GUID gDxeCpuPolicyProtocolGuid;

#pragma pack (push,1)

/**
  The protocol allows the platform code to publish a set of configuration information that the
  CPU drivers will use to configure the processor in the DXE phase.
  This Policy Protocol needs to be initialized for CPU configuration.
  @note The Protocol has to be published before processor DXE drivers are dispatched.

  This member specifies the revision of the Cpu Policy protocol. This field is used to indicate backward
  compatible changes to the protocol. Any such changes to this protocol will result in an update in the revision number.

  <b>Revision 1</b>:
   - Initial version
**/

//
// DXE_CPU_POLICY_PROTOCOL revisions
//
#define DXE_CPU_POLICY_PROTOCOL_REVISION 1

typedef struct {
  /**
  Policy to obtain DGR status from setup option.
   - <b>0: DGR is disabled</b>.
   - 1: DGR is enabled.
  **/
  UINT32                         DgrEnable           : 1;
  /**
  Policy to obtain DGR-SPA status from setup option.
   - <b>0: DGR-SPA is disabled</b>.
   - 1: DGR-SPA is enabled.
  **/
  UINT32                         DgrSpaEnable        : 1;
  /**
  Policy to obtain DGR State Save support status from setup option.
   - <b>0: SMM can have Read/Write access to SMM State Save region </b>.
   - 1: SMM State Save region access is protected.
  **/
  UINT32                         DgrStateSaveProtect  : 1;
  /**
  Policy to obtain DGR Skip OEM Exception Handler from setup option.
   - <b>0: OEM exception handler will execute normal way in case of resource access violation</b>.
   - 1: OEM Exception handler will be skipped and exceution continues in case of resource access volation.
  **/
  UINT32                         DgrSkipOemExceptionHandler : 1;
  UINT32                         RsvdBit                    : 26;  ///< Reserved bits, align to multiple 32;

  UINT8                          Revision;                  ///< Current revision of policy.
  /**
  Indidates if SMM Save State saved in MSRs.
  if enabled, SMM Save State will use the MSRs instead of the memory.
   - <b>0: FALSE</b> - SMM Save State will use the memory.
   - 1: TRUE - SMM Save State will use the MSRs.
  **/
  UINT8                          SmmMsrSaveStateEnable;
  /**
  Indidates if SMM Delay feature is supported.
   - 0: FALSE - SMM Delay feature is not supported.
   - <b>1: TRUE</b> - SMM Delay feature is supported.
  **/
  UINT8                          SmmUseDelayIndication;
  /**
  Indidates if SMM Block feature is supported.
   - 0: FALSE - SMM Block feature is not supported.
   - <b>1: TRUE</b> - SMM Block feature is supported.
  **/
  UINT8                          SmmUseBlockIndication;
  /**
  Indidates if SMM Enable/Disable feature is supported.
   - 0: FALSE - SMM Enable/Disable feature is not supported.
   - <b>1: TRUE</b> - SMM Enable/Disable feature is supported.
  **/
  UINT8                          SmmUseSmmEnableIndication;
  /**
  Specifies if SMM Proccessor Trace will be Enabled.
   - <b>0: FALSE</b> - SMM Proccessor Trace will be disabled.<BR>
   - 1: TRUE - SMM Proccessor Trace will be enabled.<BR>
  **/
  UINT8                          SmmProcTraceEnable;
  UINT8                          ReservedByte[6];           ///< Reserved bytes, align to multiple 8.
} DXE_CPU_POLICY_PROTOCOL;

#pragma pack (pop)

#endif
