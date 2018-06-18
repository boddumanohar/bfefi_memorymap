#include "boot.h"
#include "efi.h"
#include "efilib.h"

boot_ret_t my_prestart_fn()
{
    Print(L"hello from my_prestart_fn\n");
    return BOOT_CONTINUE;
}

boot_ret_t gBS_memorymap(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    Print(L"Hello, world!\n");
    return BOOT_CONTINUE;
}


boot_ret_t register_module_hello()
{
    boot_add_prestart_fn(my_prestart_fn);
    boot_add_prestart_fn(gBS_memorymap);
    return BOOT_SUCCESS;
}
