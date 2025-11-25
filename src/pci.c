#include "pci.h"
#include "io/io.h"
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

void pci_check_device(uint8_t bus, uint8_t device) 
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
        print("Found USB Controller! Vendro device, vednorID, deviceID\n" + vendor_device + vendor_id + device_id);

        uint32_t bar0 = pci_config_read(bus, device, 0, 0x10);
        uint32_t mmio_base_physical = bar0 & 0xFFFFFFF0;
        print("MMIO Base Physical Address: " + mmio_base_physical );
    }
}

void pci_init()
{
    print("PCI bus scan started...\n");
    for (uint8_t bus = 0; bus < 256; bus++) 
    {
        for (uint16_t device = 0; device < 32; device++) 
        {
            pci_check_device(bus, device);
        }
    }
    print("PCI bus scan completed.\n");
}