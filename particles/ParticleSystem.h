/*
 * ParticleSystem.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <particles/config.h>

#include "ElementCollection.hpp"

#include "ParticleEmitter.h"
#include "ParticleUpdater.h"
#include "ParticleSorter.h"
#include "ParticleRenderer.h"


#include <vector>

using namespace std;
using namespace utils;

namespace particles
{

  class ParticleSystem
  {
  public:

    ParticleCollection* particles;

    vector<ParticleEmitter*>* emitters;
    vector<ParticleUpdater*>* updaters;
    ParticleSorter* sorter;
    ParticleRenderer* renderer;

    vector<int> particleEmitter;
    vector<int> particleUpdater;

    unsigned int aliveParticles;
    unsigned int maxParticles;
    float emissionRate;

    bool loop;

    ParticleSystem(int initialParticlesNumber, int _maxParticles, float _emissionRate
                   , bool _loop = true);

    virtual ~ParticleSystem();

    void AddEmitter(ParticleEmitter* emitter);
    void AddUpdater(ParticleUpdater* updater);
    void SetSorter(ParticleSorter* sorter);
    void SetRenderer(ParticleRenderer* renderConfig);


    virtual void Start() = 0;

    // Particle updating methods
    virtual void Update(float deltaTime) = 0;

    // Particle render updating method
    virtual void UpdateRender() = 0;

    // Render method
    virtual void Render() = 0;

  };

}

#endif /* PARTICLESYSTEM_H_ */
