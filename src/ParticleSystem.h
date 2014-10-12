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

  class SortUnit
  {
  public:
     int idx;
     float distance;

     static bool sort (const SortUnit& lhs, const SortUnit& rhs){return lhs.distance > rhs.distance;}
  };

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

    int aliveParticles;
    int maxParticles;
    float emissionRate;

    bool loop;

    ParticleSystem(int initialParticlesNumber, int _maxParticles, float _emissionRate, bool _loop = true)
    :
     emitters(nullptr_t)
    , updaters(nullptr_t)
    , sorter(nullptr_t)
    , renderConfigurer(nullptr_t)
    , render(nullptr_t)
    , maxParticles (_maxParticles)
    , emissionRate(_emissionRate)
    , loop(_loop)
    {
      tparticleContainer* p = new tparticleContainer(maxParticles);
      particles = new ParticleCollection(p, 0, maxParticles);

      int counter = 0;
      tparticleptr particle;
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        //TODO
//        particle = new Particle(counter, vec3zero, )
      }

      aliveParticles = initialParticlesNumber;
    }

    void UpdateUnified(float deltaTime);
    void UpdateSeparated(float deltaTime);

  };

}

#endif /* PARTICLESYSTEM_H_ */
