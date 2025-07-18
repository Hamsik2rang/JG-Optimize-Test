#include "utility.h"

std::mt19937_64 Random::s_engine;

void Random::Init()
{
    s_engine.seed(std::random_device()());
}

int32_t Random::NextInt32(int min, int max)
{
    std::uniform_int_distribution<int32_t> dis(min, max);
    int32_t gen = dis(s_engine);
    
    return gen;
}
uint32_t Random::NextUInt32(uint32_t min, uint32_t max )
{
    std::uniform_int_distribution<uint32_t> dis(min, max);
    uint32_t gen = dis(s_engine);
    
    return gen;
}
float Random::NextFloat(float min, float max)
{
    std::uniform_real_distribution<> dis(min, max);
    float gen = static_cast<float>(dis(s_engine));
    
    return gen;
}
