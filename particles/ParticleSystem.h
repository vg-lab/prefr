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

#include "ParticlePrototype.h"

#include <vector>

using namespace std;
using namespace utils;

namespace particles
{


  class ParticleSystem
  {
  public:

    ParticleCollection* particles;

    EmissionNodesArray* emissionNodes;

    PrototypesArray* prototypes;

    vector<ParticleEmitter*>* emitters;
    vector<ParticleUpdater*>* updaters;
    ParticleSorter* sorter;
    ParticleRenderer* renderer;

    vector<int> particleEmissionNodes;
    vector<int> particlePrototype;
    vector<int> particleEmitter;
    vector<int> particleUpdater;

    unsigned int aliveParticles;
    unsigned int maxParticles;
    float emissionRate;

    bool loop;

    ParticleSystem(int initialParticlesNumber, int _maxParticles, float _emissionRate, bool _loop = true);

    virtual ~ParticleSystem();

    virtual void AddEmissionNode(EmissionNode* node);
    virtual void AddPrototype(ParticlePrototype* prototype);
    virtual void AddEmitter(ParticleEmitter* emitter);
    virtual void AddUpdater(ParticleUpdater* updater);
    virtual void SetSorter(ParticleSorter* sorter);
    virtual void SetRenderer(ParticleRenderer* renderConfig);


    virtual void Start() = 0;

    // Particle updating methods
    virtual void Update(float deltaTime) = 0;

    // Particle render updating method
    virtual void UpdateRender() = 0;

    // Render method
    virtual void Render() const = 0;

  };

}

#endif /* PARTICLESYSTEM_H_ */
