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
    Vector position; // 16bytes(float * 4)
    Vector velocity;
    Vector color;
    float leftTime;
    uint32_t count;
};

struct ParticleSOA
{
    ParticleSOA(size_t size)
    : position(new Vector[size])
    , velocity(new Vector[size])
    , color(new Vector[size])
    , leftTime(new float[size])
    , count((uint32_t)size)
    {}
    
    Vector* position; // 16bytes
    Vector* velocity;
    Vector* color;
    float* leftTime;
    uint32_t count;
};

int update_particle_position(std::list<Particle>& particles, size_t size, float deltaTime);
int update_particle_position(Particle* particles, size_t size, float deltaTime);
int update_particle_position(ParticleSOA& particles, size_t size, float deltaTime);

#endif
