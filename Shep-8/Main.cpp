#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Chip8.h"

namespace Shep8 
{
    class App : public olc::PixelGameEngine
    {
    public:
        App()
        {
            sAppName = "Shep-8 Emulator";
        }

        Chip8 emu;

        void DrawRam(int x, int y, uint16_t addr, int rows, int cols)
        {
            int ramX = x, ramY = y;
            for (int row = 0; row < rows; row++)
            {
                std::string offset = "$" + hex(addr, 4) + ":";
                for (int col = 0; col < cols; col++)
                {
                    offset += " " + hex(emu.readByte(addr), 2);
                    addr++;
                }
                DrawString(ramX, ramY, offset);
                ramY += 10;
            }
        }

        void DrawCPUState(int x, int y)
        {
            DrawString(x, y, "STATUS:");
            DrawString(x, y + 10, "PC: $" + hex(emu.pc, 4));
            DrawString(x, y + 20, "DT: $" + hex(emu.DT, 2));
            DrawString(x, y + 30, "ST: $" + hex(emu.ST, 2));
            DrawString(x, y + 40, "I: $" + hex(emu.I, 4));
            for (int i = 0; i < sizeof(emu.V) / sizeof(emu.V[0]); i++)
            {
                DrawString(x, y + i * 10 + 50, "V" + std::to_string(i) + ": $" + hex(emu.V[i], 2) + " (" + std::to_string(emu.V[i]) + ")");
            }
        }

        bool OnUserCreate() override 
        {
            /*
            // example chip-8 program which will multiply two
            // bytes stored at 0x300 and 0x301 and store the
            // resulting BCD representation at 0x305, 0x306 and 0x307

            0x300:
            05 07

            0x200:
            ld I, 0x300    (A3 00)
            ld V1, [I]     (F1 65)
            ld V2, 0x00    (62 00)
            ld V3, 0x01    (63 01)
            add V2, V0     (82 04)
            sub V1, V3     (81 35)
            se V1, 0x00    (31 00)
            jp 0x208       (12 08)
            ld I, 0x306    (A3 06)
            ld B, V2       (F2 33)
            */

            std::stringstream ss;
            ss << "A3 00 F1 65 62 00 63 01 82 04 81 35 31 00 12 08 A3 06 F2 33";
            uint16_t offset = 0x200;
            while (!ss.eof()) 
            {
                std::string b;
                ss >> b;
                emu.memory[offset++] = (uint8_t)std::stoul(b, nullptr, 16);
            }

            // set memory at number to multiply
            emu.memory[0x300] = 0x5;
            emu.memory[0x301] = 0x7;

            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override
        {
            Clear(olc::DARK_BLUE);

            if (GetKey(olc::SPACE).bPressed)
            {
                emu.tick();
            }

            DrawRam(2, 2, 0x0300, 16, 16);
            DrawRam(2, 182, 0x0200, 16, 16);
            DrawCPUState(448, 2);
            return true;
        }
    };
}

int main()
{
    Shep8::App app;
    if (app.Construct(680, 480, 2, 2))
        app.Start();

    return 0;
}
