# Address Decode Encode UEFI application

# Usage

```
AddrDecodeEncode [-e] [Controller] [Channel] [Dimm] [Rank] [BankGroup] [Bank] [RAS] [CAS]
  -e         Specifies input of a Dram Address to be Encoded.
  Controller  Specifies the Controller {0..1}
  Channel     Specifies the Channel    {0..1}
  Dimm        Specifies the Dimm       {0..1}
  Rank        Specifies the Rank       {0..1}
  BankGroup   Specifies the BankGroup  {0..3}
  Bank        Specifies the Bank       {0..7}
  RAS         Specifies the RAS        {0..2^17-1}
  CAS         Specifies the CAS        {0..2^10-1}
  The output of this command will be the System Address associated with the Dram Address
AddrDecodeEncode [-d] [System Address]
  -d            Specifies input of a System Address to be Decoded.
  SystemAddress  Specifies the System Address.
  The output of this command will be the Dram Address associated with the System Address
```