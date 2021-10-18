//
// Created by Jake on 10/16/21.
//

#include <cstdint>
#include "../cstr.h"

namespace PCI {
    const char* DeviceClasses[] {
        "Unclassified",
        "Mass Storage Controller",
        "Network Controller",
        "Display Controller",
        "Multimedia Controller",
        "Memory Controller",
        "Bridge",
        "Simple Communication Controller",
        "Base System Peripheral",
        "Input Device Controller",
        "Docking Station",
        "Processor",
        "Serial Bus Controller",
        "Wireless Controller",
        "Intelligent Controller",
        "Satellite Communication Controller",
        "Encryption Controller",
        "Signal Processing Controller",
        "Processing Accelerator",
        "Non-Essential Instrumentation"
    };

    // TODO: lookup vendor ID online once network is implemented
    const char* GetVendorName(uint16_t vendorID) {
        switch (vendorID) {
            case 0x1022:
                return "AMD";
            case 0x10de:
                return "NVIDIA Corporation";
            case 0x8086:
                return "Intel Corp";
        }
        return to_hstring(vendorID);
    }

    const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID) {
        switch (vendorID) {
            case 0x8086: // Intel Corp
                switch (deviceID) {
                    case 0x2918:
                        return "LPC Interface Controller";
                    case 0x2922:
                        return "6 port SATA Controller [AHCI mode]";
                    case 0x2930:
                        return "SMBus Controller";
                    case 0x29c0:
                        return "Express DRAM Controller";
                }
        }
        return to_hstring(deviceID);
    }

    const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode) {
        switch (classCode) {
            case 0x00: // Unclassified
                switch (subclassCode) {
                    case 0x00:
                        return "Non-VGA-Compatible Unclassified Device";
                    case 0x01:
                        return "VGA-Compatible Unclassified Device";
                }
                return to_hstring(subclassCode);
            case 0x01: // Mass Storage Controller
                switch (subclassCode) {
                    case 0x00:
                        return "SCSI Bus Controller";
                    case 0x01:
                        return "IDE Controller";
                    case 0x02:
                        return "Floppy Disk Controller";
                    case 0x03:
                        return "IPI Bus Controller";
                    case 0x04:
                        return "RAID Controller";
                    case 0x05:
                        return "ATA Controller";
                    case 0x06:
                        return "Serial ATA Controller";
                    case 0x07:
                        return "Serial Attached SCSI Controller";
                    case 0x08:
                        return "Non-Volatile Memory Controller";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x02: // Network Controller
                switch (subclassCode) {
                    case 0x00:
                        return "Ethernet Controller";
                    case 0x01:
                        return "Token Ring Controller";
                    case 0x02:
                        return "FDDI Controller";
                    case 0x03:
                        return "ATM Controller";
                    case 0x04:
                        return "ISDN Controller";
                    case 0x05:
                        return "WorldFip Controller";
                    case 0x06:
                        return "PICMG 2.14 Multi Computing Controller";
                    case 0x07:
                        return "Infiniband Controller";
                    case 0x08:
                        return "Fabric Controller";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x03: // Display Controller
                switch (subclassCode) {
                    case 0x00:
                        return "VGA Compatible Controller";
                    case 0x01:
                        return "XGA Controller";
                    case 0x02:
                        return "3D Controller (Not VGA-Compatible)";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x04: // Multimedia Controller
                switch (subclassCode) {
                    case 0x00:
                        return "Multimedia Video Controller";
                    case 0x01:
                        return "Multimedia Audio Controller";
                    case 0x02:
                        return "Computer Telephony Device";
                    case 0x03:
                        return "Audio Device";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x05: // Memory Controller
                switch (subclassCode) {
                    case 0x00:
                        return "RAM Controller";
                    case 0x01:
                        return "Flash Controller";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x06: // Bridge
                switch (subclassCode) {
                    case 0x00:
                        return "Host Bridge";
                    case 0x01:
                        return "ISA Bridge";
                    case 0x02:
                        return "EISA Bridge";
                    case 0x03:
                        return "MCA Bridge";
                    case 0x04:
                        return "PCI-to-PCI Bridge";
                    case 0x05:
                        return "PCMCIA Bridge";
                    case 0x06:
                        return "NuBus Bridge";
                    case 0x07:
                        return "CardBus Bridge";
                    case 0x08:
                        return "RACEway Bridge";
                    case 0x09:
                        return "PCI-to-PCI Bridge";
                    case 0x0a:
                        return "InfiniBand-to-PCI Host Bridge";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x07: // Simple Communication Controller
                switch (subclassCode) {
                    case 0x00:
                        return "Serial Controller";
                    case 0x01:
                        return "Parallel Controller";
                    case 0x02:
                        return "Multiport Serial Controller";
                    case 0x03:
                        return "Modem";
                    case 0x04:
                        return "IEEE 488.1/2 (GPIB) Controller";
                    case 0x05:
                        return "Smart Card Controller";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x08: // Base System Peripheral
                switch (subclassCode) {
                    case 0x00:
                        return "PIC";
                    case 0x01:
                        return "DMA Controller";
                    case 0x02:
                        return "Timer";
                    case 0x03:
                        return "RTC Controller";
                    case 0x04:
                        return "PCI Hot-Plug Controller";
                    case 0x05:
                        return "SD Host controller";
                    case 0x06:
                        return "IOMMU";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x09: // Input Device Controller
                switch (subclassCode) {
                    case 0x00:
                        return "Keyboard Controller";
                    case 0x01:
                        return "Digitizer Pen";
                    case 0x02:
                        return "Mouse Controller";
                    case 0x03:
                        return "Scanner Controller";
                    case 0x04:
                        return "Gameport Controller";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x0a: // Docking Station
                switch (subclassCode) {
                    case 0x00:
                        return "Generic";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x0b: // Processor
                switch (subclassCode) {
                    case 0x00:
                        return "386";
                    case 0x01:
                        return "486";
                    case 0x02:
                        return "Pentium";
                    case 0x03:
                        return "Pentium Pro";
                    case 0x10:
                        return "Alpha";
                    case 0x20:
                        return "PowerPC";
                    case 0x30:
                        return "MIPS";
                    case 0x40:
                        return "Co-Processor";
                }
                return to_hstring(subclassCode);
            case 0x0c: // Serial Bus Controller
                switch (subclassCode) {
                    case 0x00:
                        return "FireWire (IEEE 1394) Controller";
                    case 0x01:
                        return "ACCESS Bus Controller";
                    case 0x02:
                        return "SSA";
                    case 0x03:
                        return "USB Controller";
                    case 0x04:
                        return "Fibre Channel";
                    case 0x05:
                        return "SMBus Controller";
                    case 0x06:
                        return "InfiniBand Controller";
                    case 0x07:
                        return "IPMI Interface";
                    case 0x08:
                        return "SERCOS Interface (IEC 61491)";
                    case 0x09:
                        return "CANbus Controller";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x0d: // Wireless Controller
                switch (subclassCode) {
                    case 0x00:
                        return "iRDA Compatible Controller";
                    case 0x01:
                        return "Consumer IR Controller";
                    case 0x10:
                        return "RF Controller";
                    case 0x11:
                        return "Bluetooth Controller";
                    case 0x12:
                        return "Broadband Controller";
                    case 0x20:
                        return "Ethernet Controller (802.1a)";
                    case 0x21:
                        return "Ethernet Controller (802.1b)";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x0e: // Intelligent Controller
            if (subclassCode == 0) return "I20";
            case 0x0f: // Satellite Communication Controller
                switch (subclassCode) {
                    case 0x01:
                        return "Satellite TV Controller";
                    case 0x02:
                        return "Satellite Audio Controller";
                    case 0x03:
                        return "Satellite Voice Controller";
                    case 0x04:
                        return "Satellite Data Controller";
                }
                return to_hstring(subclassCode);
            case 0x10: // Encryption Controller
                switch (subclassCode) {
                    case 0x00:
                        return "Network and Computing Encryption/Decryption";
                    case 0x10:
                        return "Entertainment Encryption/Decryption";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
            case 0x11: // Signal Processing Controller
                switch (subclassCode) {
                    case 0x00:
                        return "DPIO Modules";
                    case 0x01:
                        return "Performance Counters";
                    case 0x10:
                        return "Communication Synchronizer";
                    case 0x20:
                        return "Signal Processing Management";
                    case 0x80:
                        return "Other";
                }
                return to_hstring(subclassCode);
        }
        return to_hstring(subclassCode);
    }

    const char* GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF) {
        switch (classCode) {
            case 0x01: // Mass Storage Controller
                switch (subclassCode) {
                    case 0x01: // IDE Controller
                        switch (progIF) {
                            case 0x00:
                                return "ISA Compatibility mode-only controller ";
                            case 0x05:
                                return "PCI native mode-only controller";
                            case 0x0a:
                                return "ISA Compatibility mode controller, supports both channels switched to PCI native mode";
                            case 0x0f:
                                return "PCI native mode controller, supports both channels switched to ISA compatibility mode";
                            case 0x80:
                                return "ISA Compatibility mode-only controller, supports bus mastering";
                            case 0x85:
                                return "PCI native mode-only controller, supports bus mastering";
                            case 0x8a:
                                return "ISA Compatibility mode controller, supports both channels switched to PCI native mode, supports bus mastering";
                            case 0x8f:
                                return "PCI native mode controller, supports both channels switched to ISA compatibility mode, supports bus mastering";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x05: // ATA Controller
                        switch (progIF) {
                            case 0x20:
                                return "Single DMA";
                            case 0x30:
                                return "Chained DMA";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x06: // Serial ATA Controller
                        switch (progIF) {
                            case 0x00:
                                return "Vendor Specific Interface";
                            case 0x01:
                                return "AHCI 1.0";
                            case 0x02:
                                return "Serial Storage Bus";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x07: //Serial Attached SCSI Controller
                        switch (progIF) {
                            case 0x00:
                                return "SAS";
                            case 0x01:
                                return "Serial Storage Bus";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x08: // Non-Volatile Memory Controller
                        switch (progIF) {
                            case 0x01:
                                return "NVMHCI";
                            case 0x02:
                                return "NVM Express";
                            default:
                                return to_hstring(progIF);
                        }
                    default:
                        return to_hstring(progIF);
                }
            case 0x03: // Display Controller
                switch (subclassCode) {
                    case 0x00: // VGA Compatible Controller
                        switch (progIF) {
                            case 0x00:
                                return "VGA Controller";
                            case 0x01:
                                return "8514-Compatible Controller";
                            default:
                                return to_hstring(progIF);
                        }
                    default:
                        return to_hstring(progIF);
                }
            case 0x06: // Bridge
                switch (subclassCode) {
                    case 0x04: // PCI-to_PCI Bridge
                        switch (progIF) {
                            case 0x00:
                                return "Normal Decode";
                            case 0x01:
                                return "Subtractive Decode";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x08: // RACEway Bridge
                        switch (progIF) {
                            case 0x00:
                                return "Transparent Mode";
                            case 0x01:
                                return "Endpoint Mode";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x09: // PCI-to-PCI Bridge
                        switch (progIF) {
                            case 0x40:
                                return "Semi-Transparent, Primary bus towards host CPU";
                            case 0x80:
                                return "Semi-Transparent, Secondary bus towards host CPU";
                            default:
                                return to_hstring(progIF);
                        }
                    default:
                        return to_hstring(progIF);
                }
            case 0x07: // Simple Communication Controller
                switch (subclassCode) {
                    case 0x00: // Serial Controller
                        switch (progIF) {
                            case 0x00:
                                return "8250-Compatible (Generic XT)";
                            case 0x01:
                                return "16450-Compatible";
                            case 0x02:
                                return "16550-Compatible";
                            case 0x03:
                                return "16650-Compatible";
                            case 0x04:
                                return "16750-Compatible";
                            case 0x05:
                                return "16850-Compatible";
                            case 0x06:
                                return "16950-Compatible";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x01: // Parallel Controller
                        switch (progIF) {
                            case 0x00:
                                return "Standard Parallel Port";
                            case 0x01:
                                return "Bi-Directional Parallel Port";
                            case 0x02:
                                return "ECP 1.X Compliant Parallel Port";
                            case 0x03:
                                return "IEEE 1284 Controller";
                            case 0xfe:
                                return "IEEE 1284 Target Device";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x03: // Modem
                        switch (progIF) {
                            case 0x00:
                                return "Generic Modem";
                            case 0x01:
                                return "Hayes 16450-Compatible Interface";
                            case 0x02:
                                return "Hayes 16550-Compatible Interface";
                            case 0x03:
                                return "Hayes 16650-Compatible Interface";
                            case 0x04:
                                return "Hayes 16750-Compatible Interface";
                            default:
                                return to_hstring(progIF);
                        }
                    default:
                        return to_hstring(progIF);
                }
            case 0x08: // Base System Peripheral
                switch (subclassCode) {
                    case 0x00: // PIC
                        switch (progIF) {
                            case 0x00:
                                return "Generic 8259-Compatible";
                            case 0x01:
                                return "ISA-Compatible";
                            case 0x02:
                                return "EISA-Compatible";
                            case 0x10:
                                return "I/O APIC Interrupt Controller";
                            case 0x20:
                                return "I/O(x) APIC Interrupt Controller";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x01: // DMA Controller
                        switch (progIF) {
                            case 0x00:
                                return "Generic 8237-Compatible";
                            case 0x01:
                                return "ISA-Compatible";
                            case 0x02:
                                return "EISA-Compatible";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x02: // Timer
                        switch (progIF) {
                            case 0x00:
                                return "Generic 8254-Compatible";
                            case 0x01:
                                return "ISA-Compatible";
                            case 0x02:
                                return "EISA-Compatible";
                            case 0x03:
                                return "HPET";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x03: // RTC Controller
                        switch (progIF) {
                            case 0x00:
                                return "Generic RTC";
                            case 0x01:
                                return "ISA-Compatible";
                            default:
                                return to_hstring(progIF);
                        }
                    default:
                        return to_hstring(progIF);
                }
            case 0x09: // Input Device Controller
                if (subclassCode == 0x04) { // Gameport Controller
                    switch (progIF) {
                        case 0x00:
                            return "Generic";
                        case 0x10:
                            return "Extended";
                        default:
                            return to_hstring(progIF);
                    }
                } else {
                    return to_hstring(progIF);
                }
            case 0x0c: // Serial Bus Controller
                switch (subclassCode) {
                    case 0x00: // FireWire (IEEE 1394) Controller
                        switch (progIF) {
                            case 0x00:
                                return "Generic";
                            case 0x10:
                                return "OHCI";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x03: // USB Controller
                        switch (progIF) {
                            case 0x00:
                                return "UHCI Controller";
                            case 0x10:
                                return "OHCI Controller";
                            case 0x20:
                                return "EHCI (USB2) Controller";
                            case 0x30:
                                return "XHCI (USB3) Controller";
                            case 0x80:
                                return "Unspecified";
                            case 0xfe:
                                return "USB Device (Not a host controller)";
                            default:
                                return to_hstring(progIF);
                        }
                    case 0x07: // IPMI Interface
                        switch (progIF) {
                            case 0x00:
                                return "SMIC";
                            case 0x01:
                                return "Keyboard Controller Style";
                            case 0x02:
                                return "Block Transfer";
                            default:
                                return to_hstring(progIF);
                        }
                    default:
                        return to_hstring(progIF);
                }
            default:
                return to_hstring(progIF);
        }
    }
}
