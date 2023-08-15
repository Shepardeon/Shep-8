#pragma once
#include <cstdint>
#include <stack>
#include <vector>
#include <string>
#include <map>

class Chip8
{
public:
    Chip8();
    ~Chip8();

public:
    uint16_t startAddr = 0x200; // Programs start at 0x200
    uint8_t memory[4096]; // 4KB of RAM

    uint8_t V[16]; // General 8-bit registers V0-VF
    uint16_t I;    // Special address register
    uint8_t DT;    // Special delay register
    uint8_t ST;    // Special sound register
    uint16_t pc;   // program counter
    std::stack<uint16_t> stack; // stack & stack pointer

    // Reads and execute one instruction from ram
    void tick();

private:
    struct INSTRUCTION 
    {
        std::string name;
        uint16_t mask;
        void (Chip8::* execute)(void) = nullptr;
    };
    std::map<uint16_t, INSTRUCTION> lookup;

    uint16_t read(uint16_t addr);

    // OpCodes
    void SYS();     void CLS();         void RET();     void JP_ADDR();     void CALL();
    void SE_BYTE(); void SNE_BYTE();    void SE_REG();  void LD_BYTE();     void ADD_BYTE();
    void LD_REG();  void OR();          void AND();     void XOR();         void ADD_REG();
    void SUB_REG(); void SHR();         void SUBN();    void SHL();         void SNE_REG();
    void LDI();     void JP_OFF();      void RND();     void DRW();         void SKP();
    void SKNP();    void LD_DT();       void LD_KEY();  void LDDT_REG();    void LDST();
    void ADDI();    void LDF();         void LDB();     void LDI_ARR();     void LD_REG_ARR();
};

