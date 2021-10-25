//
// Created by Jake on 10/16/21.
//

#include "ahci.h"
#include "../../BasicRenderer.h"
#include "../../paging/PageTableManager.h"
#include "../../memory/heap.h"
#include "../../paging/PageFrameAllocator.h"
#include "../../memory/memory.h"

#include "../../cstr.h"

namespace AHCI {

#define HBA_PORT_DEV_PRESENT 0x3
#define HBA_PORT_IPM_ACTIVE 0x1
#define SATA_SIG_ATAPI 0xeb140101
#define SATA_SIG_ATA 0x00000101
#define SATA_SIG_SEMB 0xc33c0101
#define SATA_SIG_PM 0x96690101

#define HBA_PxCMD_CR 0x8000
#define HBA_PxCMD_FRE 0x0010
#define HBA_PxCMD_ST 0x0001
#define HBA_PxCMD_FR 0x4000

#define HBA_PxIS_TFES (1 << 30)

#define ATA_CMD_READ_DMA_EX 0x25
#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

    PortType CheckPortType(HBAPort* port) {
        volatile uint32_t sataStatus = port->sataStatus;


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

    void Port::Configure() {
        StopCMD();

        void* newBase = GlobalAllocator.RequestPage();
        memset(newBase, 0, 1024);
        hbaPort->commandListBase = (uint32_t)(uint64_t)newBase;
        hbaPort->commandListBaseUpper = (uint32_t)((uint64_t)newBase >> 32);

        void* fisBase = GlobalAllocator.RequestPage();
        memset(fisBase, 0, 256);
        hbaPort->fisBaseAddress = (uint32_t)(uint64_t)fisBase;
        hbaPort->fisBaseAddressUpper = (uint32_t)((uint64_t)fisBase >> 32);

        HBACommandHeader* commandHeader = (HBACommandHeader*)((uint64_t)hbaPort->commandListBase + ((uint64_t)hbaPort->commandListBaseUpper << 32));

        for (int i = 0; i < 32; i++) {
            commandHeader[i].prdtLength = 8;

            void* commandTableAddress = GlobalAllocator.RequestPage();
            // FIXME: check that address fits into 57 bits
            memset(commandTableAddress, 0, 256);
            uint64_t address = (uint64_t)commandTableAddress + (i << 7);
            commandHeader[i].commandTableBaseAddress = (uint32_t)address;
            commandHeader[i].commandTableBaseAddressUpper = (uint32_t)(address >> 32);
        }

        StartCMD();
    }

    void Port::StopCMD() {
        hbaPort->commandStatus &= ~HBA_PxCMD_ST;
        hbaPort->commandStatus &= ~HBA_PxCMD_FRE;

        while (true) {
            if (hbaPort->commandStatus & HBA_PxCMD_FR) continue;
            if (hbaPort->commandStatus & HBA_PxCMD_CR) continue;
            break;
        }
    }

    void Port::StartCMD() {
        while (hbaPort->commandStatus & HBA_PxCMD_CR);

        hbaPort->commandStatus |= HBA_PxCMD_FRE;
        hbaPort->commandStatus |= HBA_PxCMD_ST;
    }

    // Is it up to the developer to Free() the pages once they are finished
    void* Port::Read(uint64_t sector, uint32_t sectorCount) {
        const uint8_t commandSlot = 0;

        // Start with making the buffer
        void* buffer;
        size_t pageCount = (sectorCount * 0x200 + PAGE_SIZE) / PAGE_SIZE;
        buffer = GlobalAllocator.RequestPages(pageCount);
        memset(buffer, 0, pageCount * PAGE_SIZE);

        // Then read to the buffer
        uint64_t spin = 0;

        uint32_t sectorL = (uint32_t)sector;
        uint32_t sectorH = (uint32_t)(sector >> 32);

        HBACommandHeader* commandHeader = (HBACommandHeader*)((uint64_t)hbaPort->commandListBase + ((uint64_t)hbaPort->commandListBaseUpper << 32));
        commandHeader->commandFISLength = sizeof(FIS_REG_H2D)/ sizeof(uint32_t); // command FIS size
        commandHeader->write = 0; // this is a read;
        commandHeader->prdtLength = 1;

        HBACommandTable* commandTable = (HBACommandTable*)((uint64_t)commandHeader->commandTableBaseAddress + ((uint64_t)commandHeader->commandTableBaseAddressUpper << 32));
        memset((void*)commandTable, 0, sizeof(HBACommandTable) + (commandHeader->prdtLength - 1) * sizeof(HBAPRDTEntry));

        commandTable->prdtEntry[0].dataBaseAddress = (uint32_t)(uint64_t)buffer;
        commandTable->prdtEntry[0].dataBaseAddressUpper = (uint32_t)((uint64_t)buffer >> 32);
        commandTable->prdtEntry[0].byteCount = (sectorCount << 9) - 1; // 512 bytes per sector;
        commandTable->prdtEntry[0].interruptOnCompletion = 1;

        FIS_REG_H2D* commandFIS = (FIS_REG_H2D*)(&commandTable->commandFIS);

        commandFIS->fisType = FIS_TYPE_REG_H2D;
        commandFIS->commandControl = 1; // is a command
        commandFIS->command = ATA_CMD_READ_DMA_EX;

        commandFIS->lba0 = (uint8_t)sectorL;
        commandFIS->lba1 = (uint8_t)(sectorL >> 8);
        commandFIS->lba2 = (uint8_t)(sectorL >> 16);
        commandFIS->lba3 = (uint8_t)(sectorL >> 24);
        commandFIS->lba4 = (uint8_t)sectorH;
        commandFIS->lba5 = (uint8_t)(sectorH >> 8);

        commandFIS->deviceRegister = 1 << 6; // LBA mode

        commandFIS->countLow = sectorCount & 0xFF;
        commandFIS->countHigh = (sectorCount >> 8) & 0xFF;

        while ((hbaPort->taskFileData & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000) {
            spin++;
        }
        if (spin == 1000000) {
            GlobalAllocator.FreePages(buffer, pageCount);
            return nullptr; // port is hung
        }

        hbaPort->commandIssue |= (1 << commandSlot);

        while (true) {
            if ((hbaPort->commandIssue & (1 << commandSlot)) == 0) {
                break;
            }

            if (hbaPort->interruptStatus & HBA_PxIS_TFES) {
                GlobalAllocator.FreePages(buffer, pageCount);
                return nullptr;
            }

        }

        return buffer;
    }

    // TODO: Port::Write
    bool Port::Write(uint64_t, void* buffer) {
        return false;
    }

    AHCIDriver::AHCIDriver(PCI::PCIDeviceHeader* pciBaseAddress) {
        this->PCIBaseAddress = pciBaseAddress;
        GlobalRenderer->Print("AHCI Driver Initialized \n");

        PCI::PCIHeader0* pciHeader = (PCI::PCIHeader0*)(this->PCIBaseAddress);
        ABAR = (HBAMemory*)(uint64_t)pciHeader->BAR5;

        GlobalPageTableManager.MapMemory(ABAR, ABAR);

        ProbePorts();

        for (int i = 0; i < portCount; i++) {
            Port* port = ports[i];;

            port->Configure();

        }
    }

    AHCIDriver::~AHCIDriver() {
        for (int i = 0; i < portCount; i++) {
            delete (void *)ports[i];
        }
    }

    void AHCIDriver::ProbePorts() {
        uint32_t portsImplemented = ABAR->portsImplemented;
        for (int i = 0; i < 32; i++) {
            if (portsImplemented & (1 << i)) {
                HBAPort* port = (HBAPort*)&ABAR->ports[i];
                PortType portType = CheckPortType(port);

                if (portType == PortType::SATA || portType == PortType::SATAPI) {
                    //ports[portCount] = new Port();
                    ports[portCount]->portType = portType;
                    ports[portCount]->hbaPort = port;
                    ports[portCount]->portNumber = portCount;
                    portCount++;
                }
            }
        }
    }
}