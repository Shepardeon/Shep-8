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
        void (Chip8::* execute)(uint16_t params) = nullptr;
    };
    std::map<uint16_t, INSTRUCTION> lookup;

    uint16_t read(uint16_t addr);

    // OpCodes
    void SYS(uint16_t);     void CLS(uint16_t);         void RET(uint16_t);     void JP_ADDR(uint16_t);     void CALL(uint16_t);
    void SE_BYTE(uint16_t); void SNE_BYTE(uint16_t);    void SE_REG(uint16_t);  void LD_BYTE(uint16_t);     void ADD_BYTE(uint16_t);
    void LD_REG(uint16_t);  void OR(uint16_t);          void AND(uint16_t);     void XOR(uint16_t);         void ADD_REG(uint16_t);
    void SUB_REG(uint16_t); void SHR(uint16_t);         void SUBN(uint16_t);    void SHL(uint16_t);         void SNE_REG(uint16_t);
    void LDI(uint16_t);     void JP_OFF(uint16_t);      void RND(uint16_t);     void DRW(uint16_t);         void SKP(uint16_t);
    void SKNP(uint16_t);    void LD_DT(uint16_t);       void LD_KEY(uint16_t);  void LDDT_REG(uint16_t);    void LDST(uint16_t);
    void ADDI(uint16_t);    void LDF(uint16_t);         void LDB(uint16_t);     void LDI_ARR(uint16_t);     void LD_REG_ARR(uint16_t);
};

