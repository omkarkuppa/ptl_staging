/** @file
  The file is used to send and receive all HCI cmds/events.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include "BluetoothBusDxe.h"

/**
  Removes all the nodes in a doubly-linked list.

  The list is specified by the head node and the corresponding signature of the
  list.

  @param[in]  Link       The pointer to the head node of a doubly-linked list.
  @param[in]  Signature  The signature for the doubly-linked list.

  @retval TRUE  The function always returns TRUE.

**/
BOOLEAN
BtClearList (
  IN LIST_ENTRY                  *Link,
  IN UINTN                       Signature
  )
{
  EFI_TPL                             OldTpl;
  LIST_ENTRY                          *LinkWalker;
  LIST_ENTRY                          *NextLink;
  BT_LE_DEV_INFO                      *BtLeDevInfo;
  BT_LE_PAIRED_DEV_INFO               *BtLePairedDevInfoList;
  BT_LE_SB_DEV                        *BtLeSbDev;

  LinkWalker = Link->ForwardLink;
  NextLink = NULL;

  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  switch (Signature) {
    case BT_LE_DEV_INFO_SIGNATURE:
      for (;LinkWalker != Link; ) {
        BtLeDevInfo = CR(LinkWalker, BT_LE_DEV_INFO, Link, BT_LE_DEV_INFO_SIGNATURE);
        NextLink = LinkWalker->ForwardLink;
        RemoveEntryList (LinkWalker);
        FreePool (BtLeDevInfo);
        LinkWalker = NextLink;
      }
      break;
    case BT_LE_PAIRED_DEV_INFO_SIGNATURE:
      for (;LinkWalker != Link; ) {
        BtLePairedDevInfoList = CR(LinkWalker, BT_LE_PAIRED_DEV_INFO, Link, BT_LE_PAIRED_DEV_INFO_SIGNATURE);
        NextLink = LinkWalker->ForwardLink;
        RemoveEntryList (LinkWalker);
        FreePool (BtLePairedDevInfoList);
        LinkWalker = NextLink;
      }
      break;
    case BT_LE_SB_DEV_SIGNATURE:
      for (;LinkWalker != Link; ) {
        BtLeSbDev = CR(LinkWalker, BT_LE_SB_DEV, Link, BT_LE_SB_DEV_SIGNATURE);
        NextLink = LinkWalker->ForwardLink;
        BtDelLeSb (BtLeSbDev->BtHcDev, BtLeSbDev);
        LinkWalker = NextLink;
      }
      break;
    default:
      break;
  }
  gBS->RestoreTPL (OldTpl);

  return TRUE;
}

/**
  Finds a BluetoothLE Service Binding device with given Bluetooth device
  address.

  @param[in]  BtHcDev  A pointer to the BTHC_DEV structure.
  @param[in]  BDAddr   A pointer to a Bluetooth device address to search.

  @retval  A pointer to the BT_LE_SB_DEV structure, if the given Bluetooth
           device address matches a device in the BluetoothLE Service Binding
           device list.
  @retval  NULL, if the given Bluetooth device address does not match any
           device in the BluetoothLE Service Binding device list.

**/
BT_LE_SB_DEV*
BtFindLeSbDevByBDaddr (
  IN BTHC_DEV                   *BtHcDev,
  IN BLUETOOTH_LE_ADDRESS       *BDAddr
  )
{
  LIST_ENTRY    *BtLeSbDevList;
  LIST_ENTRY    *Link;
  BT_LE_SB_DEV     *BtLeSbDev;
  EFI_TPL       OldTpl;

  BtLeSbDevList = &BtHcDev->BtLeSbList;
  OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
  for (Link = BtLeSbDevList->ForwardLink;
       Link != BtLeSbDevList;
       Link = Link->ForwardLink) {
    BtLeSbDev = CR (
                  Link,
                  BT_LE_SB_DEV,
                  Link,
                  BT_LE_SB_DEV_SIGNATURE
                  );

    /* Currently checking only with BD Address as Stack in not returning the correct BD Address Type in case of Privacy 1.2.
     * TODO: Check with BD Address Type as well. */
  if (((CompareMem (&BtLeSbDev->BtLeDevInfo->BDAddr.Address, &BDAddr->Address, sizeof(BDAddr->Address)) == 0)) ||
     ((CompareMem (&BtLeSbDev->BtLeDevInfo->IDAddr.Address, &BDAddr->Address, sizeof(BDAddr->Address)) == 0))) {
      gBS->RestoreTPL (OldTpl);
      return BtLeSbDev;
    }
  }
  gBS->RestoreTPL (OldTpl);
  return NULL;
}

/**
  Finds a paired BluetoothLE device with given Bluetooth device address.

  @param[in]  BtHcDev  A pointer to the BTHC_DEV structure.
  @param[in]  BDAddr   A pointer to a Bluetooth device address to search.

  @retval  A pointer to the BT_LE_PAIRED_DEV_INFO structure, if the given
           Bluetooth device address matches a device in the BluetoothLE paired
           device list.
  @retval  NULL, if the given Bluetooth device address does not match any
           device in the BluetoothLE paired device list.

**/
BT_LE_PAIRED_DEV_INFO*
BtFindLePairedDevByBDAddr (
  IN BTHC_DEV                   *BtHcDev,
  IN BLUETOOTH_LE_ADDRESS          *BDAddr
  )
{
  LIST_ENTRY                *Link;
  LIST_ENTRY                *BtLePairedDevLink;
  BT_LE_PAIRED_DEV_INFO   *BtLePairedDevInfo;
  BtLePairedDevLink = &BtHcDev->BtLePairedList;

  for (Link = BtLePairedDevLink->ForwardLink; Link != BtLePairedDevLink; Link = Link->ForwardLink) {
    BtLePairedDevInfo = CR (Link, BT_LE_PAIRED_DEV_INFO, Link, BT_LE_PAIRED_DEV_INFO_SIGNATURE);
    if ((CompareMem (&BtLePairedDevInfo->BDAddr.Address, &BDAddr->Address, sizeof(BDAddr->Address)) == 0)
      || (CompareMem (&BtLePairedDevInfo->IDAddr.Address, &BDAddr->Address, sizeof(BDAddr->Address)) == 0))
      return BtLePairedDevInfo;
  }
  return NULL;
}

