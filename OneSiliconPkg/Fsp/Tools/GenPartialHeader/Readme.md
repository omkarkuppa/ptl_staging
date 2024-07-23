# Partial header generation tool
  This tool will generate the partial UPD headers for those bootloaders do not need the full list UPD headers.

# Usage:
  - call partial_header.bat [full_list_UPD_headers_path] [required_upd_list_path]
  - required_upd_list_path: contains required UPD for bootloader, listed in TXT files.
  - full_list_UPD_headers_path: contains the full list UPD headers.

# Output:
  - PartialHeader folder will be created for generated partial headers
  - xxx_error.log will be generated if required UPD missing.
  - script will return non-zero for failure cases and 0 for success case.

