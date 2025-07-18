//
//  particle.h
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/18/25.
//
#pragma once
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Predefine.h"
#include "common.h"

struct Particle
{
    Vector position; // 16bytes
    Vector velocity;
    Vector color;
    float leftTime;
    float count;
};

template <size_t TSize>
struct ParticleSOA
{
    Vector position[TSize]; // 16bytes
    Vector velocity[TSize];
    Vector color[TSize];
    float leftTime[TSize];
    float count[TSize];
};

void update_particle_position(std::list<Particle>& particles, size_t size, float deltaTime);
void update_particle_position(Particle* particles, size_t size, float deltaTime);

// compile-time instantiation을 위해 헤더에 정의
template<size_t TSize>
void update_particle_position(ParticleSOA<TSize>& particles, size_t size, float deltaTime)
{
    assert(TSize == size);
    for(size_t i = 0; i < size; i++)
    {
        auto& p = particles;
        
        p.position[size][0] += p.velocity[size][0] * deltaTime;
        p.position[size][1] += p.velocity[size][1] * deltaTime;
        p.position[size][2] += p.velocity[size][2] * deltaTime;
        
        p.leftTime[size] -= deltaTime;
    }
}

#endif
