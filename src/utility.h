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
    static float NextFloat(float min = std::numeric_limits<float>::min(), float max = std::numeric_limits<float>::max());

private:
    static std::mt19937_64 s_engine;
};

#endif
