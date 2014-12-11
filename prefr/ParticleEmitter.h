/*
 * ParticleEmitter.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_

#include "InterpolationSet.hpp"
#include "ElementCollection.hpp"
#include "ParticlePrototype.h"
#include "ParticleEmissionNode.h"


using namespace utils;

namespace prefr
{

  class ParticleEmitter
  {

  public:

    ParticleCollection* particles;

    vector<EmissionNode*>* emissionNodes;
    vector<int>* refEmissionNodes;

    PrototypesArray* prototypes;
    vector<int>* refPrototypes;

    vector<int> emissionNodeParticlesPerCycle;

    int maxParticles;
    int particlesPerCycle;
    float emissionRate;
    bool loop;
    bool active;

  private:

    float normalizationFactor;

  public:

    ParticleEmitter( const ParticleCollection& particlesArray, float _emissionRate, bool _loop);

    virtual ~ParticleEmitter();

    virtual void StartEmission(float deltaTime);
    virtual int EmitSingle(const tparticle_ptr i);

    virtual void EmitAll(float deltaTime);

    virtual void EmitFunction(const tparticle_ptr current, bool override = false);

    virtual void UpdateConfiguration();

  };


}



#endif /* PARTICLEEMITTER_H_ */
