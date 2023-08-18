#include "Chip8.h"
#include <iostream>

std::string hex(uint32_t n, uint8_t d)
{
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4)
    {
        s[i] = "0123456789ABCDEF"[n & 0xF];
    }
    return s;
}

Chip8::Chip8()
{
    // setup program counter at 0x0200
    pc = 0x200;

    using a = Chip8;
    lookup = {
        { 0x0000, INSTRUCTION {"SYS", &a::SYS } },
        { 0x00E0, INSTRUCTION {"CLS", &a::CLS } },
        { 0x00EE, INSTRUCTION {"RET", &a::RET } },
        { 0x1000, INSTRUCTION {"JP", &a::JP_ADDR } },
        { 0x2000, INSTRUCTION {"CALL", &a::CALL } },
        { 0x3000, INSTRUCTION {"SE", &a::SE_BYTE } },
        { 0x4000, INSTRUCTION {"SNE", &a::SNE_BYTE } },
        { 0x5000, INSTRUCTION {"SE", &a::SE_REG } },
        { 0x6000, INSTRUCTION {"LD", &a::LD_BYTE } },
        { 0x7000, INSTRUCTION {"ADD", &a::ADD_BYTE } },
        { 0x8000, INSTRUCTION {"LD", &a::LD_REG } },
        { 0x8001, INSTRUCTION {"OR", &a::OR } },
        { 0x8002, INSTRUCTION {"AND", &a::AND } },
        { 0x8003, INSTRUCTION {"XOR", &a::XOR } },
        { 0x8004, INSTRUCTION {"ADD", &a::ADD_REG } },
        { 0x8005, INSTRUCTION {"SUB", &a::SUB_REG } },
        { 0x8006, INSTRUCTION {"SHR", &a::SHR } },
        { 0x8007, INSTRUCTION {"SUBN", &a::SUBN } },
        { 0x800E, INSTRUCTION {"SHL", &a::SHL } },
        { 0x9000, INSTRUCTION {"SNE", &a::SNE_REG } },
        { 0xA000, INSTRUCTION {"LD", &a::LDI } },
        { 0xB000, INSTRUCTION {"JP", &a::JP_OFF } },
        { 0xC000, INSTRUCTION {"RND", &a::RND } },
        { 0xD000, INSTRUCTION {"DRW", &a::DRW } },
        { 0xE09E, INSTRUCTION {"SKP", &a::SKP } },
        { 0xE0A1, INSTRUCTION {"SKNP", &a::SKNP } },
        { 0xF007, INSTRUCTION {"LD", &a::LD_DT } },
        { 0xF00A, INSTRUCTION {"LD", &a::LD_KEY } },
        { 0xF015, INSTRUCTION {"LD", &a::LDDT_REG } },
        { 0xF018, INSTRUCTION {"LD", &a::LDST } },
        { 0xF01E, INSTRUCTION {"ADD", &a::ADDI } },
        { 0xF029, INSTRUCTION {"LD", &a::LDF } },
        { 0xF033, INSTRUCTION {"LD", &a::LDB } },
        { 0xF055, INSTRUCTION {"LD", &a::LDI_ARR } },
        { 0xF065, INSTRUCTION {"LD", &a::LD_REG_ARR } },
    };
};

Chip8::~Chip8(){}

void Chip8::tick() 
{
    // Read instruction at pc
    uint16_t opcode = read(pc);
    pc += 2;

    uint8_t opcode_hi = (opcode & 0xF000) >> 8;
    uint16_t mask = opcode_hi > 0xD0 ? 0xF0FF
        : opcode_hi == 0x50 || opcode_hi == 0x80 || opcode_hi == 0x90 ? 0xF00F
        : opcode == 0x00E0 || opcode == 0x00EE ? 0xFFFF
        : 0xF000;

    nnn = opcode & 0x0FFF;
      n = opcode & 0x000F;
      x = (opcode & 0x0F00) >> 8;
      y = (opcode & 0x00F0) >> 4;
     kk = opcode & 0x00FF;

    auto i = lookup.find(opcode & mask);
    if (i != lookup.end())
    {
        (this->*lookup[opcode & mask].execute)();
        std::cout << "found instruction " << i->second.name << " - " << hex(opcode, 4) << " / " << hex(opcode & mask, 4) << std::endl;
    }
    else
    {
        std::cout << "couldn't find instruction " << hex(opcode & mask, 4) << std::endl;
    }

    if (DT > 0) DT--;
    if (ST > 0) ST--;
}

uint16_t Chip8::read(uint16_t addr) 
{
    uint8_t read_hi = memory[addr & 0xFFF];
    uint8_t read_lo = memory[(addr + 1) & 0xFFF];

    return (read_hi << 8) | read_lo;
}

uint8_t Chip8::readByte(uint16_t addr)
{
    return memory[addr & 0xFFF];
}

void Chip8::write(uint16_t addr, uint8_t data)
{
    memory[addr & 0xFFF] = data;
}

#pragma region Opcodes implementation

// Unimplemented
void Chip8::SYS() {}

void Chip8::CLS()
{
    // TODO
}

void Chip8::RET()
{
    pc = stack.top();
    stack.pop();
}

void Chip8::JP_ADDR()
{
    pc = nnn;
}

void Chip8::CALL()
{
    stack.push(pc);
    pc = nnn;
}

void Chip8::SE_BYTE()
{
    pc += V[x] == kk ? 2 : 0;
}

void Chip8::SNE_BYTE()
{
    pc += V[x] != kk ? 2 : 0;
}

void Chip8::SE_REG()
{
    pc += V[x] == V[y] ? 2 : 0;
}

void Chip8::LD_BYTE()
{
    V[x] = kk;
}

void Chip8::ADD_BYTE()
{
    V[x] += kk;
}

void Chip8::LD_REG()
{
    V[x] = V[y];
}

void Chip8::OR()
{
    V[x] |= V[y];
}

void Chip8::AND()
{
    V[x] &= V[y];
}

void Chip8::XOR()
{
    V[x] ^= V[y];
}

void Chip8::ADD_REG()
{
    uint16_t temp = V[x] + V[y];
    V[0xF] = temp > 255 ? 1 : 0;

    V[x] = temp & 0x00FF;
}

void Chip8::SUB_REG()
{
    V[0xF] = V[x] > V[y] ? 1 : 0;
    V[x] -= V[y];
}

void Chip8::SHR()
{
    V[0xF] = V[x] & 1; // Set flag if least-significant bit is 1
    V[x] >> 1;
}

void Chip8::SUBN()
{
    V[0xF] = V[y] > V[x] ? 1 : 0;
    V[y] -= V[x];
}

void Chip8::SHL()
{
    V[0xF] = V[x] & 0x80; // Set flag if most-significant bit is 1
    V[x] << 1;
}

void Chip8::SNE_REG()
{
    pc += V[x] != V[y] ? 2 : 0;
}

void Chip8::LDI()
{
    I = nnn;
}

void Chip8::JP_OFF()
{
    pc = nnn + V[0];
}

void Chip8::RND()
{
    srand(time(NULL));
    uint8_t temp = rand() % 256;

    V[x] = temp & kk;
}

void Chip8::DRW()
{
    // TODO
}

void Chip8::SKP()
{
    // TODO
}

void Chip8::SKNP()
{
    // TODO
}

void Chip8::LD_DT()
{
    V[x] = DT;
}

void Chip8::LD_KEY()
{
    // TODO
}

void Chip8::LDDT_REG()
{
    DT = V[x];
}

void Chip8::LDST()
{
    ST = V[x];
}

void Chip8::ADDI()
{
    I += V[x];
}

void Chip8::LDF()
{
    // One sprite requires 5 bit and sprites are stored starting at 0x0 so :
    // 0 => 0x0000
    // 1 => 0x0005
    // 2 => 0x000A
    // ...
    uint16_t addr = 5 * V[x];
    I = addr;
}

void Chip8::LDB()
{
    uint8_t hundreds = V[x] / 100;
    uint8_t tens = (V[x] % 100) / 10;
    uint8_t units = (V[x] % 10);

    write(I, hundreds);
    write(I + 1, tens);
    write(I + 2, units);
}

void Chip8::LDI_ARR()
{
    for (int i = 0; i <= x; i++)
    {
        write(I + i, V[i]);
    }
}

void Chip8::LD_REG_ARR()
{
    for (int i = 0; i <= x; i++)
    {
        V[i] = readByte(I + i);
    }
}

#pragma endregion
