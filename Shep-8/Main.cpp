#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
namespace Shep8 
{
    class App : public olc::PixelGameEngine
    {
    public:
        App()
        {
            sAppName = "Shep-8 Emulator";
        }

        bool OnUserCreate() override 
        {
            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override
        {
            for (int x = 0; x < ScreenWidth(); x++)
                for (int y = 0; y < ScreenHeight(); y++)
                    Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
            return true;
        }
    };
}

int main()
{
    Shep8::App app;
    if (app.Construct(265, 240, 4, 4))
        app.Start();

    return 0;
}
