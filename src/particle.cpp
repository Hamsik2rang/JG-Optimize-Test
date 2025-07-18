//
//  particle.cpp
//  JungleOptimizeTest
//
//  Created by Yongsik Im on 7/14/25.
//
#include "particle.h"

void update_particle_position(std::list<Particle>& particles, size_t size, float deltaTime)
{
    for (Particle& p : particles)
    {
        p.position[0] += p.velocity[0] * deltaTime;
        p.position[1] += p.velocity[1] * deltaTime;
        p.position[2] += p.velocity[2] * deltaTime;

        p.leftTime -= deltaTime;
    }
}

void update_particle_position(Particle* particles, size_t size, float deltaTime)
{
    for (size_t i = 0; i < size; i++)
    {
        Particle& p = particles[i];

        p.position[0] += p.velocity[0] * deltaTime;
        p.position[1] += p.velocity[1] * deltaTime;
        p.position[2] += p.velocity[2] * deltaTime;

        p.leftTime -= deltaTime;
    }
}
