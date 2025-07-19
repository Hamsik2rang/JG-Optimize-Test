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

int64_t Random::NextInt64(int64_t min, int64_t max)
{
    std::uniform_int_distribution<int64_t> dis(min, max);
    int64_t gen = dis(s_engine);

    return gen;
}

uint64_t Random::NextUInt64(uint64_t min, uint64_t max)
{
    std::uniform_int_distribution<uint64_t> dis(min, max);
    uint64_t gen = dis(s_engine);

    return gen;
}

float Random::NextFloat(float min, float max)
{
    std::uniform_real_distribution<> dis(min, max);
    float gen = static_cast<float>(dis(s_engine));
    
    return gen;
}
bool Random::NextBool()
{
	std::uniform_int_distribution<int> dis(0, 1);
	return dis(s_engine) == 1;
}

// 'HashCombineFast()' in Unreal Engine
uint32_t HashCombine(uint32_t a, uint32_t b)
{
    return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
}

uint32_t PointerHash(const void* Key)
{
    uint32_t hash = (uint32_t)((uint64_t)(Key) >> 4);
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;
    return hash;   
}