//
//  utility.h
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/19/25.
//
#pragma once
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Predefine.h"
#include <random>

class Random
{
public:
    static void Init();
    
    static int32_t NextInt32(int min = std::numeric_limits<int32_t>::min(), int max = std::numeric_limits<int32_t>::max());
    static uint32_t NextUInt32(uint32_t min = std::numeric_limits<uint32_t>::min(), uint32_t max = std::numeric_limits<uint32_t>::max());
    static int64_t NextInt64(int64_t min = std::numeric_limits<int64_t>::min(), int64_t max = std::numeric_limits<int64_t>::max());
    static uint64_t NextUInt64(uint64_t min = std::numeric_limits<uint64_t>::min(), uint64_t max = std::numeric_limits<uint64_t>::max());
    static float NextFloat(float min = std::numeric_limits<float>::min(), float max = std::numeric_limits<float>::max());
    static bool NextBool();

private:
    static std::mt19937_64 s_engine;
};

// HashCombineFast in Unreal Engine
uint32_t HashCombine(uint32_t a, uint32_t b);
uint32_t PointerHash(const void* key);
#endif
