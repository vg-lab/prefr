/*
 * ParticleSystem.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include "config.h"

#include "ElementCollection.hpp"
#include "InterpolationSet.hpp"

#include "ParticleEmitter.h"
#include "ParticleUpdater.h"
#include "ParticleSorter.h"
#include "ParticleRenderConfig.h"
#include "ParticleRender.h"

#include <vector>

using namespace std;
using namespace utils;

namespace particles
{

  typedef InterpolationSet<float> vectortfloat;
  typedef InterpolationSet<vec3> vectortvec3;
  typedef InterpolationSet<vec4> vectortvec4;

  typedef struct
  {
    float maxLife;
    float dispersion;

    vectortfloat size;

    vec3 positionOffset;

    vectortvec3 velocity;
    vectortvec4 color;

  } ParticlePrototype;

  class ParticleSystem
  {
  public:

    ParticleCollection* particles;

    vector<ParticleEmitter*>* emitters;
    vector<ParticleUpdater*>* updaters;
    ParticleSorter* sorter;
    ParticleRenderConfig* renderConfigurer;
    ParticleRender* render;

    vector<int> particleEmitter;
    vector<int> particleUpdater;

    int aliveParticles;
    int maxParticles;
    float emissionRate;

    bool loop;

    ParticleSystem(int initialParticlesNumber, int _maxParticles, float _emissionRate
                   , bool _loop = true);

    virtual ~ParticleSystem();

    void AddEmitter(ParticleEmitter* emitter);
    void AddUpdater(ParticleUpdater* updater);
    void SetSorter(ParticleSorter* sorter);
    void SetRenderConfig(ParticleRenderConfig* renderConfig);
    void SetParticleRender(ParticleRender* render);

    // Particle updating methods
    virtual void UpdateUnified(float deltaTime);
    virtual void UpdateSeparated(float deltaTime);

    // Particle render updating method
    virtual void UpdateRender();

    // Render method
    virtual void Render();

  };

}

#endif /* PARTICLESYSTEM_H_ */
