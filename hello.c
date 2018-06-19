#include "boot.h"
#include "efi.h"
#include "efilib.h"
#define PAGE_SIZE 4096
 
const CHAR16 *memory_types[] = {
    L"EfiReservedMemoryType",
    L"EfiLoaderCode",
    L"EfiLoaderData",
    L"EfiBootServicesCode",
    L"EfiBootServicesData",
    L"EfiRuntimeServicesCode",
    L"EfiRuntimeServicesData",
    L"EfiConventionalMemory",
    L"EfiUnusableMemory",
    L"EfiACPIReclaimMemory",
    L"EfiACPIMemoryNVS",
    L"EfiMemoryMappedIO",
    L"EfiMemoryMappedIOPortSpace",
    L"EfiPalCode",
};

//Read more: https://blog.fpmurphy.com/2012/08/uefi-memory-v-e820-memory.html#ixzz5Inn9K5MQ

boot_ret_t my_prestart_fn()
{
    Print(L"hello from my_prestart_fn\n");
    return BOOT_CONTINUE;
}

const CHAR16 *
memory_type_to_str(UINT32 type)
{
    if (type > sizeof(memory_types)/sizeof(CHAR16 *))
        return L"Unknown";
 
    return memory_types[type];
}

//Read more: https://blog.fpmurphy.com/2012/08/uefi-memory-v-e820-memory.html#ixzz5InmiRvN7

boot_ret_t gBS_memorymap(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    EFI_MEMORY_DESCRIPTOR *memory = NULL;
    UINTN mapsize = 4 * 4096;
    EFI_STATUS status = BS->AllocatePool(EfiLoaderData, mapsize, (void *)memory);
    if(status != EFI_SUCCESS){
        Print(L"status is not success\n");
    }

    Print(L" %d allocate page status \n", status);
    
    UINTN MapKey = 0;
    UINTN DescriptorSize = 0;
    UINT32 DescriptorVersion = 0;
    status = BS->GetMemoryMap(&mapsize, memory, &MapKey, &DescriptorSize, &DescriptorVersion);
    if(status != EFI_SUCCESS){
        Print(L"status is not success\n");
    }
    Print(L"Hello, world!\n");

    Print(L"Memory Map Size: %d\n", mapsize);
    Print(L"Map Key: %d\n", MapKey);
    Print(L"Descriptor Version: %d\n", DescriptorSize);
    Print(L"Descriptor Size: %d\n\n", DescriptorVersion);

    EFI_MEMORY_DESCRIPTOR *desc = memory;
    int i = 0;
    while((void*)desc < (void *)memory + mapsize) {
    Print(L"hello\n");
    UINTN mapping_size = desc->NumberOfPages * PAGE_SIZE;

	Print(L"[#%02d] Type: %s  Attr: 0x%x\n", i, memory_type_to_str(desc->Type), desc->Attribute);
        Print(L"      Phys: %016llx-%016llx\n", desc->PhysicalStart, desc->PhysicalStart + mapping_size);
        Print(L"      Virt: %016llx-%016llx\n\n", desc->VirtualStart, desc->VirtualStart + mapping_size);
 
        desc = (void *)desc + DescriptorSize;
        i++;

//Read more: https://blog.fpmurphy.com/2012/08/uefi-memory-v-e820-memory.html#ixzz5Inm9S8eg

}
//Read more: https://blog.fpmurphy.com/2012/08/uefi-memory-v-e820-memory.html#ixzz5Injxg2zr

    BS->FreePool(memory);
    return BOOT_CONTINUE;
}


boot_ret_t register_module_hello()
{
    boot_add_prestart_fn(my_prestart_fn);
    boot_add_prestart_fn(gBS_memorymap);
    return BOOT_SUCCESS;
}
