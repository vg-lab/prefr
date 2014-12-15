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

    int maxParticles;
    float particlesBudget;
    float emissionRate;
    bool loop;
    bool active;

  private:

    float normalizationFactor;

    // Private variables to take profit from spatial proximity cache

  protected:

    int lastParticleNodeID;
    int lastParticlePrototypeID;

    int currentNodeID;
    int currentPrototypeID;

    EmissionNode* currentNode;
    tprototype_ptr currentPrototype;

    EmissionNode* GetCurrentNode( const int& particleID );
    tprototype_ptr GetCurrentPrototype( const int& particleID );

  public:

    ParticleEmitter( const ParticleCollection& particlesArray, float _emissionRate, bool _loop);

    virtual ~ParticleEmitter();

    virtual void StartEmission(float deltaTime);
    virtual int EmitSingle(const tparticle_ptr i);

    virtual void EmitAll(float deltaTime);

    virtual void EmitFunction(const tparticle_ptr current, bool override = false);

  };


}



#endif /* PARTICLEEMITTER_H_ */
