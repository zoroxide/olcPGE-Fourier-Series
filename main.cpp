#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

enum waveType {
	SQUARE,
	SAWTOOTH,
	TRIANGLE,
	CUBIC,
	NUM_WAVES
};

class Program : public olc::PixelGameEngine
{
public:
    Program()
    {
        sAppName = "Fourier Series";
    }

private:
    float fDelta = 0.0f;
    const float pi = acos(-1);
    int nDepth = 1;
    waveType wave = SQUARE;
    olc::vf2d circle_origin = {150.0f, 240.0f};
    std::vector<float> vWave;


public:
    bool OnUserCreate() override
    {
        vWave.clear();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        //Keyboard Read
        if (GetKey(olc::Key::LEFT).bPressed)
        {
            if (nDepth > 1)
                nDepth -= 1;    
        }
        if (GetKey(olc::Key::RIGHT).bPressed)
        {
            if (nDepth < 50)
                nDepth += 1;    
        }
        if (GetKey(olc::Key::SPACE).bPressed)
        {
            wave = static_cast<waveType>((wave + 1) % NUM_WAVES); 
        }

        fDelta += fElapsedTime * 2;
        olc::vf2d origin = circle_origin;

        for (int i = 0; i < nDepth; i++)
        {
            olc::vf2d pre_orig = origin; 

            int n = 0;
            float fRadius = 0.0f;
            if (wave == SQUARE)
            {
                n = 2 * i + 1;
                fRadius = 70.0f * (4 / (n * pi));
            }
            else if (wave == SAWTOOTH)
            {
                n = i + 1;
                if ( n % 2 != 0)
                    n *= -1; 
                fRadius = 70.0f * (2 / (n * pi));
            }
            else if (wave == TRIANGLE)
            {
                n = 2 * i + 1;
                fRadius = 70.0f * (8 / ((n * pi) * (n * pi)));
                if (i % 2 == 0)
                    fRadius *= -1;
            }
            else if (wave == CUBIC)
            {
                n = i + 1;
                fRadius = 5.0f * (2 * ((n * n) * (pi * pi) - 6) / (n * n * n));
                if (n % 2 != 0)
                    fRadius *= -1;
            }
            origin.x += fRadius * cos(abs(n) * fDelta);
            origin.y += fRadius * sin(abs(n) * fDelta);


            DrawCircle(pre_orig,fabs(fRadius),olc::DARK_GREY);

            FillCircle(origin,1.0f);
            DrawLine(pre_orig, origin);
        }

        vWave.insert(vWave.begin(), origin.y);

        for (int i = 0; i < vWave.size() - 1; i++)
        {
            float x = i/8 + circle_origin.x + 150;
            DrawLine(x, vWave[i], x, vWave[i+1], olc::GREEN);
        }

        DrawLine(origin.x, origin.y, circle_origin.x + 150, vWave.front(), olc::GREEN);

        if (vWave.size() > 2000)
            vWave.pop_back();

        std::string sWave;
        if (wave == SQUARE)
            sWave = "Square";
        if (wave == SAWTOOTH)
            sWave = "Sawtooth";
        if (wave == TRIANGLE)
            sWave = "Triangle";
        if (wave == CUBIC)
            sWave = "Cubic";

        DrawString(100, 20, "Fourier Series " + sWave + " Wave", olc::WHITE, 2);

        DrawString(135, 340, "n = " + std::to_string(nDepth));
        
        return true;
    }
};

int main()
{
    Program demo;
    if (demo.Construct(640, 480, 1, 1))
        demo.Start();
    return 0;
}