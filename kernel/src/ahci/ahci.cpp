//
// Created by Jake on 10/16/21.
//

#include "ahci.h"

namespace AHCI {

#define HBA_PORT_DEV_PRESENT 0x3
#define HBA_PORT_IPM_ACTIVE 0x1
#define SATA_SIG_ATAPI 0xeb140101
#define SATA_SIG_ATA 0x00000101
#define SATA_SIG_SEMB 0xc33c0101
#define SATA_SIG_PM 0x96690101


    PortType CheckPortType(HBAPort* port) {
        uint32_t sataStatus = port->sataStatus;

        GlobalRenderer->Print("test");
        uint8_t  interfacePowerManagement = (sataStatus >> 8) & 0b111;
        uint8_t deviceDetection = sataStatus & 0b111;
        if (deviceDetection != HBA_PORT_DEV_PRESENT || interfacePowerManagement != HBA_PORT_IPM_ACTIVE) {
            return PortType::None;
        }

        switch (port->signature) {
            case SATA_SIG_ATAPI:
                return PortType::SATAPI;
            case SATA_SIG_ATA:
                return PortType::SATA;
            case SATA_SIG_SEMB:
                return PortType::SEMB;
            case SATA_SIG_PM:
                return PortType::PM;
            default:
                return PortType::None;
        }
    }

    AHCIDriver::AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress) {
        this->PCIBaseAddress = pciBaseAddress;
        GlobalRenderer->Print("AHCI Driver Initialized \n");

        ABAR = (HBAMemory*)(uint64_t)((PCI::PCIHeader0*)pciBaseAddress)->BAR5;

        GlobalPageTableManager.MapMemory(ABAR, ABAR);

        ProbePorts();
    }

    AHCIDriver::~AHCIDriver() {

    }

    void AHCIDriver::ProbePorts() {
        uint32_t portsImplemented = ABAR->portsImplemented;
        for (int i = 0; i < 32; i++) {
            if (portsImplemented & (1 << i)) {
                PortType portType = CheckPortType(&ABAR->ports[i]);

                if (portType == PortType::SATA) {
                    GlobalRenderer->Print("SATA drive \n");
                } else if (portType == PortType::SATAPI) {
                    GlobalRenderer->Print("SATAPI drive \n");
                } else {
                    GlobalRenderer->Print("not interested \n");
                }
            }
        }
    }
}