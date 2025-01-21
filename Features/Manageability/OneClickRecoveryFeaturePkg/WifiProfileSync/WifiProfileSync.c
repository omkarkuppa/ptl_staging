/** @file
  This driver is the WiFi Profile Sync with CSME through ASF supporting WLAN using
  the WifiConnectionManager driver. This is used only when One Click Recovery driver
  triggers an HTTPS boot recovery and the platform supports WLAN HTTPS image recovery
  and boot.

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

//
// Local Headers
//
#include <PiDxe.h>
#include "WifiProfileSync.h"
#include "WpsGetAsfProfile.h"
#include "WpsSetWcmProfile.h"
#include "WifiProfileSyncUtils.h"

//
// Libraries
//
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/DxeAmtSupportLib.h>
#include <Library/BootGuardLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

//
// GUID's
//
#include <Guid/EventGroup.h>
#include <Guid/GlobalVariable.h>
#include <Guid/AuthenticatedVariableFormat.h>

//
// Protocol's
//
#include <Protocol/WiFi2.h>
#include <Protocol/AsfProtocol.h>
#include <Protocol/WiFiProfileSyncProtocol.h>

//
// WiFi Profile Sync Protocol
//
GLOBAL_REMOVE_IF_UNREFERENCED EDKII_WIFI_PROFILE_SYNC_PROTOCOL mEdkiiWiFiProfileSyncProtocol = {
  EDKII_WIFI_PROFILE_SYNC_PROTOCOL_REVISION,
  SetConnectStatus,
  GetConnectStatus,
  GetProfile
};

GLOBAL_REMOVE_IF_UNREFERENCED WIFI_PROFILE  *mSyncProfile = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STATUS    mWcmConnectionStatus;

/**
  Function to set the WiFi connection status received by the WiFiConnectionManager
  as EFI_80211_CONNECT_NETWORK_RESULT_CODE, this will get converted to EFI_STATUS
  type

  @param[in] EFI_80211_CONNECT_NETWORK_RESULT_CODE    WiFi connection attempt results
**/
VOID
EFIAPI
SetConnectStatus (
  IN EFI_80211_CONNECT_NETWORK_RESULT_CODE ConnectionStatus
  )
{
  switch (ConnectionStatus) {
    case ConnectSuccess:
      mWcmConnectionStatus = EFI_SUCCESS;
      break;
    case ConnectRefused:
      mWcmConnectionStatus = EFI_ACCESS_DENIED;
      break;
    case ConnectFailed:
      mWcmConnectionStatus = EFI_DEVICE_ERROR;
      break;
    case ConnectFailureTimeout:
      mWcmConnectionStatus = EFI_TIMEOUT;
      break;
    case ConnectFailedReasonUnspecified:
      mWcmConnectionStatus = EFI_DEVICE_ERROR;
      break;
    default:
      mWcmConnectionStatus = EFI_UNSUPPORTED;
  }
}

/**
  Function to retrieve the WiFi connection status when in OCR WLAN flow

  @retval EFI_SUCCESS               WiFi connection completed successfully
  @retval Others                    Error Occurred
**/
EFI_STATUS
EFIAPI
GetConnectStatus (
  VOID
  )
{
  return mWcmConnectionStatus;
}

/**
  Wifi Profile Sync boot variable update to included wifi driver during BDS phase check for
  network necessary for boot check.

  @param [in] NetworkSupport        Determine if we are adding or removing the WiFi device from
                                    PCI required for boot list

  @retval EFI_SUCCESS               Setup data set successfully
  @retval EFI_UNSUPPORTED           Failed to get or set the setup data
  @retval Others                    Error occurred
**/
EFI_STATUS
SetWifiProfileSyncBootSupportVariable (
  IN BOOLEAN                WifiSyncBootEnable
  )
{
  EFI_STATUS                Status;
  UINT32                    WifiProfileSyncBootAttributes;
  UINTN                     VariableSize;
  WIFI_PROFILE_SYNC_CONFIG  WifiProfileSyncBootConfigData;
  EFI_EVENT                 ReadyToBootServiceEvent;

  VariableSize = sizeof (WIFI_PROFILE_SYNC_CONFIG);
  Status = gRT->GetVariable (
                  WIFI_PROFILE_SYNC_BOOT_VARIABLE_NAME,
                  &gWifiProfileSyncBootVariableGuid,
                  &WifiProfileSyncBootAttributes,
                  &VariableSize,
                  &WifiProfileSyncBootConfigData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to get setup data EfiNetworkSupport during update.\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  WifiProfileSyncBootConfigData.WifiProfileSyncBootSupport = WifiSyncBootEnable;

  Status = gRT->SetVariable (
                  WIFI_PROFILE_SYNC_BOOT_VARIABLE_NAME,
                  &gWifiProfileSyncBootVariableGuid,
                  WifiProfileSyncBootAttributes,
                  VariableSize,
                  &WifiProfileSyncBootConfigData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to set setup data EfiNetworkSupport during update.\n", __FUNCTION__));
    ASSERT_EFI_ERROR (Status);
    return EFI_UNSUPPORTED;
  }

  if (WifiSyncBootEnable) {
    // Register Ready to Boot Event for resetting the setup data to avoid triggering Wifi connection on regular boot
    Status = EfiCreateEventReadyToBootEx (
               TPL_CALLBACK,
               DisableNetwork,
               NULL,
               &ReadyToBootServiceEvent
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to to register callback to disable network setup data.\n", __FUNCTION__));
      ASSERT_EFI_ERROR (Status);
      return EFI_DEVICE_ERROR;
    }
  }
  return EFI_SUCCESS;
}

/**
  Callback to reset EFI network data setup data

  @param[in] Event                  A pointer to the Event that triggered the callback
  @param[in] Context                A pointer to private data registered with the callback function

**/
VOID
EFIAPI
DisableNetwork (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  // Remove WiFi device from PCI device needed for boot
  SetWifiProfileSyncBootSupportVariable (FALSE);
  gBS->CloseEvent (Event);
}

/**
  Event function to clear memory and uninstall WiFi profile protocol from system once
  WiFi Connection Manager has acquired necessary profile data to during OCR WLAN boot flow.

  @retval EFI_SUCCESS               Profiles returned
  @retval EFI_UNSUPPORTED           Protocol not supported
  @retval Others                    Error Occurred
**/
EFI_STATUS
WifiProfileSyncClean (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        *HandleBuffer;
  UINTN                             NumHandle;
  UINTN                             Index;
  EDKII_WIFI_PROFILE_SYNC_PROTOCOL  *ProfileSyncProtocol;

  HandleBuffer = NULL;

  // Clean memory
  if (mSyncProfile != NULL) {
    ZeroMem (mSyncProfile, sizeof (WIFI_PROFILE));
    FreePool (mSyncProfile);
  }

  Status = gBS->LocateProtocol (&gEdkiiWiFiProfileSyncProtocolGuid, NULL, (VOID **) &ProfileSyncProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to find profile protocol with status %r\n", __FUNCTION__, Status));
    return Status;
  }

  // Get handle for WiFi profile protocol to uninstall protocol
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEdkiiWiFiProfileSyncProtocolGuid,
                  NULL,
                  &NumHandle,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to get handle to uninstall protocol with status %r\n", __FUNCTION__, Status));
    return Status;
  }
  // Uninstall the WiFi profile protocol
  for (Index = 0; Index < NumHandle; Index++) {
    Status = gBS->UninstallMultipleProtocolInterfaces (
                    HandleBuffer[Index],
                    &gEdkiiWiFiProfileSyncProtocolGuid,
                    &mEdkiiWiFiProfileSyncProtocol,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to uninstall WiFi profile protocol.\n", __FUNCTION__));
    }
  }
  FreePool (HandleBuffer);
  return Status;
}

/**
  Callback set for exit boot services to clear the profile data from memory and uninstall protocol

  @param[in] Event                  A pointer to the Event that triggered the callback
  @param[in] Context                A pointer to private data registered with the callback function

**/
VOID
EFIAPI
WifiProfileSyncCleanCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS    Status;

  Status = WifiProfileSyncClean ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to clean WiFi profile secrets!\n", __FUNCTION__));
  }
  gBS->CloseEvent (Event);
}

/**
  This API will be used by the WiFi connection manager to get the WiFi profile that ASF shared
  and was stored in WiFi profile protocol, aligning and passing the ASF data structure to WCM
  structure

  @param[in, out]  WcmProfile       WiFi Connection Manager profile structure
  @param[in, out]  MacAddress       MAC address sent to the NiC aside profile data

  @return EFI_SUCCESS               Profiles returned
  @return EFI_UNSUPPORTED           Profile protocol sharing not supported or enabled
  @return EFI_NOT_FOUND             No profiles returned
  @return Others                    Error Occurred
**/
EFI_STATUS
EFIAPI
GetProfile (
  IN OUT WIFI_MGR_NETWORK_PROFILE *WcmProfile,
  IN OUT EFI_80211_MAC_ADDRESS    MacAddress
  )
{
  EFI_STATUS      Status;

  ASSERT (WcmProfile != NULL);

  // Send Sync'd ASF profile data to WCM profile data
  AlignSsidAndMacAdr (&MacAddress, mSyncProfile->ProfileData, WcmProfile);

  // Psk Key
  Status = AlignPskKey (mSyncProfile->ProfileData, WcmProfile);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // CA Certificate
  Status = AlignCaCertificate (mSyncProfile, WcmProfile);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Client certificate and key
  Status = AlignClientCertAndKey (mSyncProfile, WcmProfile);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // 802.1x EAP
  Status = Align8021xEAP (mSyncProfile->ProfileData, WcmProfile);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Network authentication and encryption method
  Status = AlignEncryptionAndAuthentication (mSyncProfile, WcmProfile);
  return Status;
}

/**
  Retrieve WiFi profile information including client certificate and key using ASF

  @param[in, out]  Profile          WiFi profile protocol structure acquired from ASF

  @retval EFI_SUCCESS               Profiles returned
  @retval EFI_OUT_OF_RESOURCES      Not enough memory
  @retval EFI_UNSUPPORTED           Profile Sync is not supported via PCD or other parameter
  @retval Others                    Error Occurred
**/
EFI_STATUS
GetAsfWifiProfile (
  IN OUT WIFI_PROFILE   *Profile
  )
{
  EFI_STATUS              Status;

  if (Profile == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a] Invalid profile address supplied!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  // Get ASF Wifi Profile Name
  Status = GetProfileName (Profile->ProfileData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Get profile data
  Status = AsfWifiGetProfileData (Profile->ProfileData);
  if (EFI_ERROR (Status) || (Profile->ProfileData->Status != AmtStatusSuccess)) {
    return EFI_UNSUPPORTED;
  }

  // Get client data if available
  Status = GetClientCertAndKey (Profile);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Get Root CA Cert if available
  Status = GetRootCaClientCert (Profile);
  return Status;
}

/**
  Enable or disable HECI in the WiFI driver during HTTP boot flows.

  @param[in]  State                 Value corresponding to enable, TRUE, or disable, FALSE.

  @retval EFI_SUCCESS               Successfully set the flag for the WiFi driver
  @retval EFI_UNSUPPORTED           Failed to set the enable/disable flag
  @retval Others                    Error Occurred
**/
EFI_STATUS
EnableWifiAmtCoex (
  IN BOOLEAN        State
  )
{
  EFI_STATUS        Status;

  Status = gRT->SetVariable (
                  L"EnableWifiAmtCoex",
                  &gUefiIntelCnvWlanVariablesGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  sizeof (State),
                  &State
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}

/**
  Main flow for profile sync drivers getting profile from ASF

  @retval EFI_SUCCESS               Profiles returned
  @retval EFI_UNSUPPORTED           Profile Sync is not supported to retrieve ASF profile
  @retval Others                    Error Occurred
**/
EFI_STATUS
WifiProfileSyncMain (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_EVENT                         ExitBootServiceEvent;
  EDKII_WIFI_PROFILE_SYNC_PROTOCOL  *ProfileSyncProtocol;

  mWcmConnectionStatus = EFI_NOT_READY;

  Status = gBS->LocateProtocol (&gEdkiiWiFiProfileSyncProtocolGuid, NULL, (VOID **) &ProfileSyncProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to find profile protocol with status %r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  mSyncProfile = (WIFI_PROFILE *) AllocateZeroPool (sizeof (WIFI_PROFILE));
  if (mSyncProfile == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Get profile from ASF
  Status = GetAsfWifiProfile (mSyncProfile);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to get ASF WiFi profile data, certificate, and key with status %r!\n", __FUNCTION__, Status));
    WifiProfileSyncClean ();
    return EFI_LOAD_ERROR;
  }

  // Set supported network setup variable to WiFi network
  SetWifiProfileSyncBootSupportVariable (TRUE);

  // Set WiFi AMT coexistence flag true
  Status = EnableWifiAmtCoex (TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to set enable WiFi AMT coexistence flag to TRUE with status %r!\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  // Register Exit Boot Services Event to clear profile data from memory
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  WifiProfileSyncCleanCallback,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &ExitBootServiceEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to register profile protocol clean callback with status %r\n", __FUNCTION__, Status));
    WifiProfileSyncClean ();
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}

/**
  Check for Secure boot, boot guard and proper boot options and PCD's are enabling for ProfileSync.

  @retval EFI_SUCCESS               Profiles returned
  @retval EFI_UNSUPPORTED           Protocol not supported
  @retval Others                    Error Occurred
**/
BOOLEAN
IsProfileSyncSupported (
  VOID
  )
{
  EFI_STATUS              Status;
  UINT16                  SpecialCommand;
  UINT8                   SecureBootState;
  UINTN                   VarSize;
  UINT32                  VarAttributes;
  UINT64                  BootGuardBootStatus;
  UINT64                  BootGuardOperationMode;

  // Check for secure boot enabled
  VarSize = sizeof (UINT8);
  SecureBootState = SECURE_BOOT_DISABLE;
  Status = gRT->GetVariable (
                  EFI_SECURE_BOOT_ENABLE_NAME,
                  &gEfiSecureBootEnableDisableGuid,
                  &VarAttributes,
                  &VarSize,
                  &SecureBootState
                  );
  if (EFI_ERROR (Status) || (SecureBootState == SECURE_BOOT_DISABLE)) {
    DEBUG ((DEBUG_ERROR, "[%a] Unsupported - Secureboot disabled!\n", __FUNCTION__));
    return FALSE;
  }

  if (IsBootGuardSupported ()) {
    BootGuardBootStatus = (*(UINT64 *) (UINTN) (TXT_PUBLIC_BASE + R_CPU_BOOT_GUARD_BOOTSTATUS) & (BIT63 | BIT62));
    BootGuardOperationMode = AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & (B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT | B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT);

    if ((BootGuardBootStatus == V_CPU_BOOT_GUARD_LOAD_ACM_SUCCESS) && (BootGuardOperationMode != 0)) {
      SpecialCommand = AsfGetSpecialCommand ();
      if ((SpecialCommand == ASF_INTEL_OEM_FORCE_HTTPS_BOOT_CMD) || (SpecialCommand == ASF_INTEL_OEM_FORCE_PBA_BOOT_CMD)) {
        return TRUE;
      }
      if (!EFI_ERROR (Status) && AmtIsKvmSessionEnabled ()) {
        return TRUE;
      } else {
        DEBUG ((DEBUG_ERROR, "[%a] Unsupported - KVM not enabled and boot option is 0x%X\n", __FUNCTION__, SpecialCommand));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "[%a] Unsupported - Bootguard disabled!\n", __FUNCTION__));
    }
  }

  return FALSE;
}

/**
  The entry point for the Wifi Profile Sync driver.

  @param[in] ImageHandle            The firmware allocated handle for the EFI image
  @param[in] SystemTable            A pointer to the EFI System Table

  @retval EFI_SUCCESS               The entry point is executed successfully
  @retval other                     Some error occurs when executing this entry point
**/
EFI_STATUS
EFIAPI
WifiProfileSyncEntryPoint (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS      Status;
  EFI_TPL         OldTpl;

  if (!IsProfileSyncSupported ()) {
    DEBUG ((DEBUG_ERROR, "[%a] WiFi profile sync not supported!\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  Status = EnableWifiAmtCoex (FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to set enable WiFi AMT coexistence flag to FALSE with status %r!\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEdkiiWiFiProfileSyncProtocolGuid,
                  &mEdkiiWiFiProfileSyncProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to install protocol with status %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

//
// OCR BIOS and Wifi driver HECI interaction race condition when multiple HECI1 message exchanges.
// This is a temporary change and addresses only the OCR scenario by restoring TPL.
// A more robust change to handle the sync issues among diff agents talking over HECI1 will be addressed in the future.
//
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  Status = WifiProfileSyncMain ();
  gBS->RestoreTPL (OldTpl);

  return Status;
}
