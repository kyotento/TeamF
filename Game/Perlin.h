#pragma once
#include <vector>

static const  int listSize = 256;

class Perlin
{
public:
    static Perlin&  GetInstance()
    {
        static Perlin instance;
        return instance;
    }
private:
    Perlin();
    ~Perlin() {}
  
public:
    float PerlinNoise(float x, float y, float z);
    float OctavePerlin(float x, float y, float z, int octaves, float persistence);
    void SetRepeat(int repeat)
    {
        m_repeat = repeat;
    }
private:
    float Fade(float t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }
    int Inc(int num)
    {
        num++;
        if (m_repeat > 0)
        {
            num %= m_repeat;
        }
        return num;
    }
    float Grad(int hash, float x, float y, float z);
    float Lerp(float a, float b, float x)
    {
        return a + x * (b - a);
    }

public:
    //0`255‚Ü‚Å‚Ì’·‚³256‚Ì”z—ñ
    static const int m_permutation[];
private:
    int m_repeat = -1;
    std::vector<int> m_p;
  
 
};

static inline Perlin& GetPerlin()
{
    return Perlin::GetInstance();
}