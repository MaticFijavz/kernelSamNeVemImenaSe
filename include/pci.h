#ifndef PCI_H
#define PCI_H
#include <stdint.h>
struct paging_4gb_chunk;
//PCI Configuration Space I/O Ports
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC
//USB host controller class codes
#define PCI_CLASS_SERIAL_BUS 0x0C
#define PCI_SUBCLASS_USB     0x03

struct pci_device_descriptor
{
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_code;
    uint8_t subclass_code;
    uint8_t prog_if;
    uint8_t revision_id;
};

void pci_init(struct paging_4gb_chunk* kernel_paging_chunk);
void pci_check_device(uint8_t bus, uint8_t device, struct paging_4gb_chunk* chunk);

#endif
