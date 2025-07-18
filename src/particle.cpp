//
//  particle.cpp
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/14/25.
//
#include "particle.h"

int update_particle_position(std::list<Particle>& particles, size_t size, float deltaTime)
{
    for (Particle& p : particles)
    {
        p.position[0] += p.velocity[0] * deltaTime;
        p.position[1] += p.velocity[1] * deltaTime;
        p.position[2] += p.velocity[2] * deltaTime;

        p.leftTime -= deltaTime;
    }
    
    return 3;
}

int update_particle_position(Particle* particles, size_t size, float deltaTime)
{
    for (size_t i = 0; i < size; i++)
    {
        Particle& p = particles[i];

        p.position[0] += p.velocity[0] * deltaTime;
        p.position[1] += p.velocity[1] * deltaTime;
        p.position[2] += p.velocity[2] * deltaTime;

        p.leftTime -= deltaTime;
    }
    
    return 3;
}

int update_particle_position(ParticleSOA& particles, size_t size, float deltaTime)
{
    assert(TSize == size);
    for(size_t i = 0; i < size; i++)
    {
        auto& p = particles;
        
        p.position[i][0] += p.velocity[i][0] * deltaTime;
        p.position[i][1] += p.velocity[i][1] * deltaTime;
        p.position[i][2] += p.velocity[i][2] * deltaTime;
        
        p.leftTime[i] -= deltaTime;
    }
    
    return 3;
}
