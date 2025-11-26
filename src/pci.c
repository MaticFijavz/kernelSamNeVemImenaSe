#include "memory/paging/paging.h"
#include "pci.h"
#include "io/io.h"
#include "memory/paging/paging.h"
#include "kernel.h"

uint32_t pci_config_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) 
{
    uint32_t address = 0;
    address |= (1 << 31);
    address |= ((uint32_t)bus << 16);
    address |= ((uint32_t)device << 11);
    address |= ((uint32_t)function << 8);
    address |= (uint32_t)(offset & 0xFC);
    
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

void pci_check_device(uint8_t bus, uint8_t device, struct paging_4gb_chunk* chunk) 
{
    uint32_t vendor_device = pci_config_read(bus, device, 0, 0);
    uint16_t vendor_id = (uint16_t)vendor_device;
    uint16_t device_id = (uint16_t)(vendor_device >> 16);

    if (vendor_id == 0xFFFF) 
    {
        return;
    }

    uint32_t class_info = pci_config_read(bus, device, 0, 0x08);
    uint8_t class_code = (uint8_t)(class_info >> 24);
    uint8_t subclass_code = (uint8_t)(class_info >> 16);

    if (class_code == PCI_CLASS_SERIAL_BUS && subclass_code == PCI_SUBCLASS_USB) 
    {
        print("Found USB Controller!\n"); // Popravljen izpis

        uint32_t bar0 = pci_config_read(bus, device, 0, 0x10);
        uint32_t mmio_base_physical = bar0 & 0xFFFFFFF0;
        uint32_t mmio_base_virtual = 0xFFFFF000;
        
        uint32_t map_flags = PAGING_IS_PRESENT | PAGING_IS_WRITEABLE | PAGING_ACCESS_FROM_ALL | PAGING_CACHE_DISABLED; 

        paging_map_to(chunk, (void*)mmio_base_virtual, (void*)mmio_base_physical, map_flags);
        
        print("MMIO Base Physical Address: ");
        print_hex(mmio_base_physical);
        print("\n");
        
        print("HCD MMIO Base Address (Virtual): ");
        print_hex(mmio_base_virtual); 
        print("\n");
        
        return;
    }
}

void pci_init(struct paging_4gb_chunk* kernel_paging_chunk)
{
    print("PCI bus scan started...\n");
    for (uint16_t bus = 0; bus < 256; bus++) 
    {
        for (uint16_t device = 0; device < 32; device++) 
        {
            pci_check_device(bus, device, kernel_paging_chunk);
        }
    }
    print("PCI bus scan completed.\n");
}