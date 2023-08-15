#include "Chip8.h"
#include <iostream>

Chip8::Chip8()
{
    // setup program counter at 0x0200
    pc = 0x200;

    using a = Chip8;
    lookup = {
        { 0x0000, INSTRUCTION {"SYS", 0x0FFF, &a::SYS } },
        { 0x00E0, INSTRUCTION {"CLS", 0x0000, &a::CLS } },
        { 0x00EE, INSTRUCTION {"RET", 0x0000, &a::RET } },
        { 0x1000, INSTRUCTION {"JP", 0x0FFF, &a::JP_ADDR } },
        { 0x2000, INSTRUCTION {"CALL", 0x0FFF, &a::CALL } },
        { 0x3000, INSTRUCTION {"SE", 0x0FFF, &a::SE_BYTE } },
        { 0x4000, INSTRUCTION {"SNE", 0x0FFF, &a::SNE_BYTE } },
        { 0x5000, INSTRUCTION {"SE", 0x0FF0, &a::SE_REG } },
        { 0x6000, INSTRUCTION {"LD", 0x0FFF, &a::LD_BYTE } },
        { 0x7000, INSTRUCTION {"ADD", 0x0FFF, &a::ADD_BYTE } },
        { 0x8000, INSTRUCTION {"LD", 0x0FF0, &a::LD_REG } },
        { 0x8001, INSTRUCTION {"OR", 0x0FF0, &a::OR } },
        { 0x8002, INSTRUCTION {"AND", 0x0FF0, &a::AND } },
        { 0x8003, INSTRUCTION {"XOR", 0x0FF0, &a::XOR } },
        { 0x8004, INSTRUCTION {"ADD", 0x0FF0, &a::ADD_REG } },
        { 0x8005, INSTRUCTION {"SUB", 0x0FF0, &a::SUB_REG } },
        { 0x8006, INSTRUCTION {"SHR", 0x0FF0, &a::SHR } },
        { 0x8007, INSTRUCTION {"SUBN", 0x0FF0, &a::SUBN } },
        { 0x800E, INSTRUCTION {"SHL", 0x0FF0, &a::SHL } },
        { 0x9000, INSTRUCTION {"SNE", 0x0FF0, &a::SNE_REG } },
        { 0xA000, INSTRUCTION {"LD", 0x0FFF, &a::LDI } },
        { 0xB000, INSTRUCTION {"JP", 0x0FFF, &a::JP_OFF } },
        { 0xC000, INSTRUCTION {"RND", 0x0FFF, &a::RND } },
        { 0xD000, INSTRUCTION {"DRW", 0x0FFF, &a::DRW } },
        { 0xE09E, INSTRUCTION {"SKP", 0x0F00, &a::SKP } },
        { 0xE0A1, INSTRUCTION {"SKNP", 0x0F00, &a::SKNP } },
        { 0xF007, INSTRUCTION {"LD", 0x0F00, &a::LD_DT } },
        { 0xF00A, INSTRUCTION {"LD", 0x0F00, &a::LD_KEY } },
        { 0xF015, INSTRUCTION {"LD", 0x0F00, &a::LDDT_REG } },
        { 0xF018, INSTRUCTION {"LD", 0x0F00, &a::LDST } },
        { 0xF01E, INSTRUCTION {"ADD", 0x0F00, &a::ADDI } },
        { 0xF029, INSTRUCTION {"LD", 0x0F00, &a::LDF } },
        { 0xF033, INSTRUCTION {"LD", 0x0F00, &a::LDB } },
        { 0xF055, INSTRUCTION {"LD", 0x0F00, &a::LDI_ARR } },
        { 0xF065, INSTRUCTION {"LD", 0x0F00, &a::LD_REG_ARR } },
    };
};

Chip8::~Chip8(){}

// TODO : Ugly
void Chip8::tick() 
{
    // Read instruction at pc
    uint16_t opcode = read(pc);
    uint16_t mask = (opcode & 0xFF00) > 0xD000 ? 0xF0FF : 0xFFFF;

    auto i = lookup.find(opcode & mask);
    if (i != lookup.end())
        std::cout << "found instruction " << i->second.name << std::endl;
    else 
    {
        mask = 0xF00F;
        i = lookup.find(opcode & mask);
        if (i != lookup.end())
            std::cout << "found instruction " << i->second.name << std::endl;
        else
        {
            mask = 0xF000;
            i = lookup.find(opcode & mask);
            if (i != lookup.end())
                std::cout << "found instruction " << i->second.name << std::endl;
            else
                std::cout << "couldn't find instruction " << (opcode & mask) << std::endl;
        }
    }

    pc += 2;
}

uint16_t Chip8::read(uint16_t addr) 
{
    uint8_t read_hi = memory[addr & 0xFFF];
    uint8_t read_lo = memory[(addr + 1) & 0xFFF];

    return (read_hi << 8) | read_lo;
}